#ifndef DEF_STRUCT_H
#define DEF_STRUCT_H

#include <limits>
#include <cassert>
#include <iostream>
#include <utility>
#include <map>
#include <list>
#include <stack>
#include <iterator>
#include <algorithm>

#include "../container/localarray.h"
#include "../container/assoctab.h"
#include "../container/hashcont.h"
#include "../container/set.h"


namespace Koala {

typedef unsigned char EdgeType;
typedef unsigned char EdgeDirection;

//do not change constans
static const EdgeDirection EdNone = 0x00; ///< Edge direction is not specified.
static const EdgeDirection EdLoop = 0x01; ///< Edge is a loop.
static const EdgeDirection EdUndir = 0x02; ///< Edge is undirected
static const EdgeDirection EdDirIn = 0x04; ///< Edge is directed in a vertex.
static const EdgeDirection EdDirOut = 0x08; ///< Edge is directed out of a vertex.
static const EdgeDirection EdAll = 0x0F; ///< All directions are included.

static const EdgeDirection AdjMatrixAllowed = 0x10;

static const EdgeType Detached=0x0;
static const EdgeType Loop=0x1;
static const EdgeType Undirected=0x2;
static const EdgeType Directed=0xC;


struct EmptyVertInfo {};
std::istream& operator>>(std::istream& is,EmptyVertInfo arg) { return is; }
std::ostream& operator<<(std::ostream& os,EmptyVertInfo arg) { return os; }

struct EmptyEdgeInfo {};
std::istream& operator>>(std::istream& is,EmptyEdgeInfo arg) { return is; }
std::ostream& operator<<(std::ostream& os,EmptyEdgeInfo arg) { return os; }


template< class VertInfo, class EdgeInfo, EdgeType > class Graph;
template< class VertInfo, class EdgeInfo, EdgeType > class Vertex;
template< class VertInfo, class EdgeInfo, EdgeType > class Edge;


class AlgorithmsDefaultSettings {
    public:

    template <class A, class B> class AssocCont {
        public:
        typedef AssocArray<A,B> Type;
//        typedef AssocTable < BiDiHashMap<A,B> > Type;
//            typedef PseudoAssocArray<A,B,AssocTable<std::map<A,int> > > Type;

    };


    // Specjalizacje dla wlasnych klas numerycznych (np. liczb wymiernych) pozwola uzywac ich jako danych
    // w algorytmach (np. dlugosci krawedzi). Dlatego w kodach nawet zerowosc jakiejs etykiety sprawdzam metoda
    template <class T> class NumberTypeBounds {
        public:

        static T plusInfty() { return std::numeric_limits<T>::max(); }
        static bool isPlusInfty(T arg) {return arg==plusInfty(); }
        static T minusInfty() { return std::numeric_limits<T>::min(); }
        static bool isMinusInfty(T arg) {return arg==minusInfty(); }
        static T zero() { return (T)0; }
        static bool isZero(T arg) {return arg==zero(); }
    };


    template <class Iterator>
    static void sort ( Iterator first, Iterator last )
    { std::make_heap(first,last); std::sort_heap(first,last);}

    template <class Iterator, class Comp>
    static void sort ( Iterator first, Iterator last, Comp comp )
    { std::make_heap(first,last,comp); std::sort_heap(first,last,comp);}

};


//Przydatny funktor domyslny tam, gdzie metoda chce dostac funktor generujacy jakies info dla
// wierzcholka lub krawedzi
template <class T> class ConstFunctor {
    const T val;
    public:
    ConstFunctor(const T& aval=T()) : val(aval) {}
    T operator()() { return T(); }
    template <class A> T operator()(A) { return val; }
    template <class A, class B> T operator()(A,B) { return val; }
    template <class A, class B, class C> T operator()(A,B,C) { return val; }
    template <class A, class B, class C,class D> T operator() (A,B,C,D) { return val; }
    template <class A, class B, class C,class D, class E> T operator() (A,B,C,D,E) { return val; }
    template <class A, class B, class C,class D, class E, class F> T operator() (A,B,C,D,E,F) { return val; }
};

template <class T>
ConstFunctor<T> constFun(const T& a=T()) { return ConstFunctor<T>(a); }

//Jesli metoda chce iterator do zapisu ciagu, a nas sam ciag nie interesuje, tylko inne efekty uboczne procedury
struct BlackHole : public std::iterator<std::output_iterator_tag,void,void,void,void> {
    template <class T>
    BlackHole& operator=(T) { return *this; }
    BlackHole& operator* () { return *this; }
    BlackHole& operator++() { return *this; }
    BlackHole operator++ (int) { return *this; }
    BlackHole() {}

    // rowniez moze sluzyc jako zaslepka dla nie interesujacego nas kontenera asocjacyjnego wymaganego w procedurze
    template <class T>
        BlackHole& operator[](T) { assert(0); return *this; }
    template <class T, class R>
        BlackHole& operator()(T,R) { assert(0); return *this; }
    template <class T>
        operator T() { assert(0); return T(); }

};

BlackHole blackHole;
int _KoalaEmptyVertDegree=0;
void* _KoalaEmptyEdgePoiner=0;

//extern BlackHole blackHole;
//extern int _KoalaEmptyVertDegree;
//extern void* _KoalaEmptyEdgePoiner;


template <class T> bool isBlackHole(const T&) { return false; }
bool isBlackHole(const BlackHole&) { return true; }

template <class Cont1, class Cont2> struct BlackHoleSwitch {
    typedef Cont1 Type;
    static Cont1& get(Cont1& a, Cont2& b) { return a; }
};

template <class Cont2> struct BlackHoleSwitch<BlackHole, Cont2 > {
    typedef Cont2 Type;
    static Cont2& get(BlackHole& a, Cont2& b) { return b; }
};


template <class Container> class StackInterface;

template <class T> class StackInterface<T*> {
    T* buf;
    int siz,maxsize;

    public:
        typedef T ElemType;

        StackInterface(T* bufor, int max) : buf(bufor), maxsize(max), siz(0) {}
        int size() { return siz; }
        bool empty() { return siz==0; }
        void push(const T& val) { assert(siz<maxsize); buf[siz++]=val; }
        void pop() { assert(siz); siz--; }
        T& top() { assert(siz); return buf[siz-1]; }
        void clear() { siz=0; }
};


template <class Container> class QueueInterface;

template <class T> class QueueInterface<T*> {
    T* buf;
    int beg,end,maxsize;

    int prev(int x) { return (x) ? x-1 : maxsize; }
    int next(int x) { return (x==maxsize) ? 0 : x+1; }
    public:
        typedef T ElemType;

        // wymaga elementu nadmiarowego!
        QueueInterface(T* bufor, int max) : buf(bufor), maxsize(max), beg(0), end(0) {}

        int size() { return (beg<=end) ? end-beg : maxsize+1-(beg-end); }
        bool empty() { return beg==end; }
        void push(const T& val) { buf[end]=val; end=next(end); assert(end!=beg); }
        void pop() { assert(beg!=end); beg=next(beg); }
        T& front() { assert(!empty()); return buf[beg]; }
        T& top() { assert(!empty()); return buf[beg]; }
        T& back() { assert(!empty()); return buf[prev(end)]; }
        void clear() { beg=end=0; }
};


template <class Container> class VectorInterface;

template <class T> class VectorInterface<T*> {
    T *start,*limit;
    int siz;

    public:
    typedef T value_type;

    VectorInterface(T* bufor, int max) : start(bufor), limit(bufor+max), siz(0) {}
//    void link(T* newbuf, int newmax) { start=newbuf; limit=start+newmax; siz=0; }

    T* begin() { return start ; }
    T* end()   { return start+siz ; }
    int size() { return siz ; }
    int max_size() { return limit-start; }
    int capacity() { return limit-start ; }
    bool empty() { return siz==0 ; }
    void reserve(int arg) { assert(arg<=max_size()); }
    void resize(int arg) {   assert(arg<=max_size()); }

    T& operator[] (int pos) { return at(pos); }
    T& at(int pos) { assert(pos<capacity()); return start[pos]; }
    T& front() { return at(0); }
    T& back() { return at(siz-1); }

    void push_back(const T& arg) { assert(siz<capacity()); start[siz++]=arg; }
    void pop_back() { assert(siz); siz--; }
    static void revert(T* f,T *l)
    {   T z; l--; while(f<l) { z=*f;*f=*l;*l=z; f++;l--; }  }
    template <class InputIterator>
        void assign (InputIterator first, InputIterator last)
    { clear(); for(;first!=last;first++) push_back(*first);  }
    void insert (T* where, int n, const T& x )
    {   assert(siz+n<=max_size());
        for(int i=siz-1;i>=where-start;i--) start[i+n]=start[i];
        for(int i=0;i<n;i++) where[i]=x;
        siz+=n;
    }
    void insert (T* where,const T& x) { insert(where,1,x); }
    template <class InputIterator>
        void insert (T* where, InputIterator first, InputIterator last)
    {   int ile=0;
        for(;first!=last;first++) { push_back(*first); ile++; } if (!ile) return;
        revert(where,start+siz); revert(where,where+ile); revert(where+ile,start+siz);
    }
    void erase(T* f, T* l)
    {   int ile=l-f; for(;f+ile<start+siz;f++) *f=f[ile];
        siz-=ile;
    }
    void erase(T* f) { erase(f,f+1); }
    void clear() { siz=0; }
};


template <class Container, class Comp> class PriQueueInterface;

template <class T, class Comp> class PriQueueInterface<T*,Comp> {
    T* buf;
    int siz,maxsize;
    Comp comp;

    public:
        typedef T ElemType;

        PriQueueInterface(T* bufor, int max, Comp acomp=Comp()) : buf(bufor), maxsize(max), siz(0), comp(acomp) {}
        PriQueueInterface(T* bufor, T* end, int max, Comp acomp=Comp()) :
            buf(bufor), maxsize(max), siz(end-bufor), comp(acomp)
        { assert(siz>=0 && siz<=maxsize); std::make_heap(buf,end,comp); }
        int size() { return siz; }
        bool empty() { return siz==0; }
        void push(const T& val) { assert(siz<maxsize); buf[siz++]=val; std::push_heap(buf,buf+siz,comp); }
        void pop() { assert(siz); std::pop_heap(buf,buf+siz,comp); siz--; }
        T top() { assert(siz); return buf[0]; }
        void clear() { siz=0; }
};


struct PathStructs {
    // Do odczytu sciezki miedzy wierzcholkiem a korzeniem, gdy droga wyznaczona jest w postaci
    // tablicy asocjacyjnej PVertex -> rekord z polami vPrev, ePrev (wierzcholek poprzedni i krawedz do niego).
    // Przydatne w roznych algorytmach najkrotszej sciezki
    // Uzytkownik podaje, gdzie wpisac wierzcholki i krawedzie najkrotszej sciezki
    template <class VIter, class EIter> struct OutPath {
            VIter vertIter;
            EIter edgeIter;

            OutPath(VIter av, EIter ei) : vertIter(av), edgeIter(ei) {}
        };
        // funkcja tworzaca, analogia make_pair
    template <class VIter, class EIter>
    static OutPath<VIter,EIter> outPath(VIter av, EIter ei) { return OutPath<VIter,EIter>(av,ei); }

};


struct ShortPathStructs : public PathStructs {

    template <class GraphType, class VertContainer, class VIter, class EIter>
    static int
        getOutPath(const GraphType& g,
            VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
            OutPath<VIter,EIter> iters,
            typename GraphType::PVertex end,
            typename GraphType::PVertex start=0) // ew. wczesniejszy punkt koncowy sciezki
        {   assert(end);
            typename GraphType::PVertex u,v=vertTab[end].vPrev;
            typename GraphType::PEdge  e=vertTab[end].ePrev;
            typename GraphType::PVertex LOCALARRAY(tabV,g.getVertNo());
            typename GraphType::PEdge LOCALARRAY(tabE,g.getVertNo());
            int len=0;

            if (end!=start) for(;v;len++)
               { tabV[len]=v; tabE[len]=e; e=vertTab[v].ePrev; v=(v==start) ? 0 : vertTab[v].vPrev; }

            for(int i=len-1;i>=0;i--)
                { *iters.vertIter=tabV[i];*iters.edgeIter=tabE[i];++iters.vertIter;++iters.edgeIter; }
            *iters.vertIter=end;++iters.vertIter;
            return len;
    }

    template <class GraphType, class VertContainer,class Iter>
    static int getUsedEdges(const GraphType &g, VertContainer& vertTab,Iter iter)
    {   int l=0;
        if (vertTab.empty()) return 0;
        for(typename VertContainer::KeyType v=vertTab.firstKey();;v=vertTab.nextKey(v))
        {   typename GraphType::PEdge e;
            if (v && (e=vertTab[v].ePrev)) {*iter=e; ++iter; l++; }
            if (v==vertTab.lastKey()) break;
        }
        return l;
    }

    template <class GraphType, class VertContainer>
    static Set<typename GraphType::PEdge> getUsedEdgesSet(const GraphType &g, VertContainer& vertTab)
    {   Set<typename GraphType::PEdge> res;
        getUsedEdges(g,vertTab,setInserter(res));
        return res;
    }

};

// te choosery mozna stosowac dla wierzcholkow i krawedzi
struct BoolChooser {
    bool val;
    typedef BoolChooser ChoosersSelfType;
    BoolChooser(bool arg=false) : val(arg) {}
    template<class Elem, class Graph> bool operator()(Elem*,const Graph&) const { return val;}
};

BoolChooser stdChoose(bool arg) { return BoolChooser(arg); }


template<class Elem> struct ValChooser {
    mutable Elem val;
    typedef ValChooser<Elem> ChoosersSelfType;
    ValChooser(Elem arg) : val(arg) {}
    template<class Graph> bool operator()(Elem elem,const Graph&) const
	{ return elem==val; }
};

template <class Elem>
ValChooser<Elem> stdValChoose(Elem arg) { return ValChooser<Elem>(arg); }


template<class Elem> struct SetChooser {
    mutable Koala::Set<Elem*> set;
    typedef SetChooser<Elem> ChoosersSelfType;
    SetChooser(const Koala::Set<Elem*>&  arg=Koala::Set<Elem*>()) : set(arg) {}
    template<class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return set.isElement(elem); }
};

template <class Elem>
SetChooser<Elem> stdChoose(const Koala::Set<Elem*>& arg) { return SetChooser<Elem>(arg); }


template <class Iter> struct ContainerChooser {
    mutable Iter begin, end;
    typedef ContainerChooser<Iter> ChoosersSelfType;
    ContainerChooser(Iter abegin=Iter(), Iter aend=Iter()) : begin(abegin), end(aend) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return std::find(begin,end,elem)!=end; }
};

template <class Iter>
ContainerChooser<Iter> stdChoose(Iter begin,Iter end)
    {return ContainerChooser<Iter>(begin,end); }

// jesli ktos chce choosera na podstawie wlasnego obiektu lub zwyklej funkcji
template <class Obj> struct ObjChooser {
    mutable Obj funktor;
    typedef ObjChooser<Obj> ChoosersSelfType;
    ObjChooser(Obj arg=Obj()) : funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return (bool)funktor(elem,graph); }
};

template <class Obj>
ObjChooser<Obj> stdFChoose(Obj arg) { return ObjChooser<Obj>(arg); }

// te choosery juz zagladaja do konkretnych pol rekordow info
template <class Info, class T> struct FieldValChooser {
	T Info::* wsk;
	mutable T val;
    typedef FieldValChooser<Info,T> ChoosersSelfType;
	FieldValChooser(T Info::* arg=0, T arg2=T()) : wsk(arg), val(arg2) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return elem->info.*wsk==val; }
};

template <class Info, class T>
FieldValChooser<Info,T> fieldChoose(T Info::* wsk,T arg)
    { return FieldValChooser<Info,T>(wsk,arg); }

template <class Info, class T> struct FielBoolChooser {
	T Info::* wsk;
    typedef FielBoolChooser<Info,T> ChoosersSelfType;
	FielBoolChooser(T Info::* arg=0) : wsk(arg){}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return bool(elem->info.*wsk); }
};

template <class Info, class T>
FielBoolChooser<Info,T> fieldChoose(T Info::* wsk) { return FielBoolChooser<Info,T>(wsk); }

// wlasny obiekt lub funkcja, ktora ma sie wykonywac dla konkretnego pola z info
template <class Info, class T, class Obj> struct FieldObjChooser {
	T Info::* wsk;
	mutable Obj funktor;
    typedef FieldObjChooser<Info,T,Obj> ChoosersSelfType;
	FieldObjChooser(T Info::* awsk=0,Obj afun=Obj()) : wsk(awsk), funktor(afun) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return (bool)funktor(elem->info.*wsk); }
};

template <class Info, class T, class Obj>
FieldObjChooser<Info,T,Obj> fieldFChoose(T Info::*wsk, Obj obj)
    { return FieldObjChooser<Info,T,Obj>(wsk,obj); }


template <class Info, class T, class Z> struct FieldSetChooser {
	T Info::* wsk;
	mutable Koala::Set<Z> set;
    typedef FieldSetChooser<Info,T,Z> ChoosersSelfType;
	FieldSetChooser(T Info::* awsk=0,const Koala::Set<Z>& aset=Koala::Set<Z>()) : wsk(awsk), set(aset) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return set.isElement(elem->info.*wsk); }
};

template <class Info, class T, class Z>
FieldSetChooser<Info,T,Z> fieldChoose(T Info::*wsk,const Koala::Set<Z>& set)
    { return FieldSetChooser<Info,T,Z>(wsk,set); }


template <class Info, class T, class Iter> struct FieldContainerChooser {
	T Info::* wsk;
	mutable Iter begin, end;
    typedef FieldContainerChooser<Info,T,Iter> ChoosersSelfType;
    FieldContainerChooser(T Info::* awsk=0, Iter a=Iter(), Iter b=Iter()): wsk(awsk), begin(a), end(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return std::find(begin,end,elem->info.*wsk)!=end; }
};

template <class Info, class T, class Iter>
FieldContainerChooser<Info,T,Iter> fieldChoose(T Info::*wsk, Iter b, Iter e)
    { return FieldContainerChooser<Info,T,Iter>(wsk,b,e); }


// choosery decydujace na podstawie wartosci przypisanej elementowi w podanej tablicy asocjacyjnej
template<class Cont> struct AssocHasChooser {
    mutable Cont cont;
    typedef AssocHasChooser<Cont> ChoosersSelfType;
    AssocHasChooser(const Cont& arg=Cont()) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem); }
};

template <class Cont>
AssocHasChooser<Cont> assocKeyChoose(const Cont& arg)
{ return AssocHasChooser<Cont>(arg); }


template<class Cont> struct AssocBoolChooser {
    mutable Cont cont;
    typedef AssocBoolChooser<Cont> ChoosersSelfType;
    AssocBoolChooser(const Cont& arg=Cont()) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && (bool)cont[elem]; }
};

template <class Cont>
AssocBoolChooser<Cont> assocChoose(const Cont& arg) { return AssocBoolChooser<Cont>(arg); }


template<class Cont> struct AssocValChooser {
    mutable Cont cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooser<Cont> ChoosersSelfType;
    AssocValChooser(const Cont& arg=Cont(),typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && cont[elem]==val; }
};

template <class Cont>
AssocValChooser<Cont> assocChoose(const Cont& arg, typename Cont::ValType val)
    { return AssocValChooser<Cont>(arg,val); }


template<class Cont> struct AssocSetChooser {
    mutable Cont cont;
    mutable Koala::Set<typename Cont::ValType> set;
    typedef AssocSetChooser<Cont> ChoosersSelfType;
    AssocSetChooser(const Cont& arg=Cont(),
                    const Koala::Set<typename Cont::ValType>& aset=Koala::Set<typename Cont::ValType>())
                    : cont(arg), set(aset) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && set.isElement(cont[elem]); }
};

template <class Cont>
AssocSetChooser<Cont> assocChoose(const Cont& arg,const Koala::Set<typename Cont::ValType>& set)
    { return AssocSetChooser<Cont>(arg,set); }


template <class Cont, class Iter> struct AssocContainerChooser {
    mutable Iter begin, end;
    mutable Cont cont;
    typedef AssocContainerChooser<Cont,Iter> ChoosersSelfType;
    AssocContainerChooser(const Cont& acont=Cont(), Iter abegin=Iter(), Iter aend=Iter())
            : cont(acont), begin(abegin), end(aend) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && std::find(begin,end,cont[elem])!=end; }
};

template <class Cont, class Iter>
AssocContainerChooser<Cont,Iter> assocChoose(const Cont& cont, Iter begin,Iter end)
    {return AssocContainerChooser<Cont,Iter>(cont,begin,end); }


template <class Cont, class Obj> struct AssocObjChooser {
    mutable Obj funktor;
    mutable Cont cont;
    typedef AssocObjChooser<Cont,Obj> ChoosersSelfType;
    AssocObjChooser(const Cont& acont=Cont(), Obj arg=Obj()) : cont(acont), funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return cont.hasKey(elem) && (bool)funktor(cont[elem]); }
};

template <class Cont, class Obj>
AssocObjChooser<Cont,Obj> assocFChoose(const Cont& cont, Obj arg) { return AssocObjChooser<Cont,Obj>(cont,arg); }


// choosery operacji logicznych na prostszych chooserach
template <class Ch1, class Ch2> struct OrChooser {
	mutable Ch1 ch1;
	mutable Ch2 ch2;
    typedef OrChooser<Ch1,Ch2> ChoosersSelfType;
	OrChooser(Ch1 a=Ch1(), Ch2 b=Ch2()) : ch1(a), ch2(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return (ch1(elem,graph) || ch2(elem,graph)); }
};

template <class  Ch1, class Ch2>
OrChooser<Ch1,Ch2> orChoose(Ch1 a, Ch2 b) { return OrChooser<Ch1,Ch2>(a,b); }

template <class  Ch1, class Ch2>
OrChooser<typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType>
    operator||(Ch1 a, Ch2 b) { return OrChooser<Ch1,Ch2>(a,b); }


template <class Ch1, class Ch2> struct AndChooser {
	mutable Ch1 ch1;
	mutable Ch2 ch2;
    typedef AndChooser<Ch1,Ch2> ChoosersSelfType;
	AndChooser(Ch1 a=Ch1(), Ch2 b=Ch2()) : ch1(a), ch2(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return (ch1(elem,graph) && ch2(elem,graph)); }
};

template <class  Ch1, class Ch2>
AndChooser<Ch1,Ch2> andChoose(Ch1 a, Ch2 b) { return AndChooser<Ch1,Ch2>(a,b); }

template <class  Ch1, class Ch2>
AndChooser<typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType>
    operator&&(Ch1 a, Ch2 b) { return AndChooser<Ch1,Ch2>(a,b); }


template <class Ch1, class Ch2> struct XorChooser {
	mutable Ch1 ch1;
	mutable Ch2 ch2;
    typedef XorChooser<Ch1,Ch2> ChoosersSelfType;
	XorChooser(Ch1 a=Ch1(), Ch2 b=Ch2()) : ch1(a), ch2(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return (ch1(elem,graph) != ch2(elem,graph)); }
};

template <class  Ch1, class Ch2>
XorChooser<Ch1,Ch2> xorChoose(Ch1 a, Ch2 b) { return XorChooser<Ch1,Ch2>(a,b); }

template <class  Ch1, class Ch2>
XorChooser<typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType>
    operator^(Ch1 a, Ch2 b) { return XorChooser<Ch1,Ch2>(a,b); }


template <class Ch1> struct NotChooser {
	mutable Ch1 ch1;
    typedef NotChooser<Ch1> ChoosersSelfType;
	NotChooser(Ch1 a=Ch1()) : ch1(a) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return !ch1(elem,graph); }
};

template <class  Ch1>
NotChooser<Ch1> notChoose(Ch1 a) { return NotChooser<Ch1>(a); }

template <class  Ch1>
NotChooser<typename Ch1::ChoosersSelfType>
    operator!(Ch1 a) { return NotChooser<Ch1>(a); }


// choosery dla wierzcholkow
struct VertDegValChooser {
    int deg;
    Koala::EdgeDirection type;
    typedef VertDegValChooser ChoosersSelfType;
    VertDegValChooser(int adeg=0, Koala::EdgeDirection atype=Koala::EdAll): deg(adeg), type(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, const Graph& g) const
    { return g.deg(v,type)==deg; }
};

VertDegValChooser vertDegChoose(int adeg, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegValChooser(adeg,atype); }


template <class Int> struct VertDegSetChooser {
    mutable Koala::Set<Int> set;
    Koala::EdgeDirection type;
    typedef VertDegSetChooser<Int> ChoosersSelfType;
    VertDegSetChooser(const Koala::Set<Int>& aset=Koala::Set<Int>(), Koala::EdgeDirection atype=Koala::EdAll):
                                            set(aset), type(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, const Graph& g) const
    { return set.isElement(g.deg(v,type)); }
};

template <class Int>
VertDegSetChooser<Int> vertDegChoose(const Koala::Set<Int>& aset, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegSetChooser<Int>(aset,atype); }


template <class Iter> struct VertDegContainerChooser {
    mutable Iter begin,end;
    Koala::EdgeDirection typ;
    typedef VertDegContainerChooser<Iter> ChoosersSelfType;
    VertDegContainerChooser(Iter abeg=Iter(), Iter aend=Iter(), Koala::EdgeDirection atype=Koala::EdAll):
                                            begin(abeg), end(aend), typ(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, const Graph& g) const
    { return std::find(begin,end,g.deg(v,typ))!=end; }
};

template <class Iter>
VertDegContainerChooser<Iter> vertDegChoose(Iter begin, Iter end, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegContainerChooser<Iter>(begin,end,atype); }

// decyzja podejmowana na podstawie wartosci funkcji lub obiektu funktora policzonego na stopniu wierzcholka
template <class Obj> struct VertDegFunctorChooser {
    mutable Obj funktor;
    Koala::EdgeDirection typ;
    typedef VertDegFunctorChooser<Obj> ChoosersSelfType;
    VertDegFunctorChooser(Obj afun=Obj(), Koala::EdgeDirection atype=Koala::EdAll):
                                            funktor(afun), typ(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, const Graph& g) const
    { return (bool)funktor(g.deg(v,typ)); }
};

template <class Obj>
VertDegFunctorChooser<Obj> vertDegFChoose(Obj afun, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegFunctorChooser<Obj>(afun,atype); }

// choosery dla krawedzi
struct EdgeTypeChooser {
    Koala::EdgeDirection mask;
    typedef EdgeTypeChooser ChoosersSelfType;
    EdgeTypeChooser(Koala::EdgeDirection amask=Koala::EdAll) : mask(amask) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return bool(g.getType(e) & mask); }
};

EdgeTypeChooser edgeTypeChoose(Koala::EdgeDirection mask) { return EdgeTypeChooser(mask); }

// choosery dla krawedzi sprawdzajace warunki dla jej koncow
template <class Ch> struct EdgeFirstEndChooser {
    mutable Ch ch;
    typedef EdgeFirstEndChooser<Ch> ChoosersSelfType;
    EdgeFirstEndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnds(e).first,g); }
};

template <class Ch>
EdgeFirstEndChooser<Ch> edgeFirstEndChoose(Ch ch) { return EdgeFirstEndChooser<Ch>(ch); }


template <class Ch> struct EdgeSecondEndChooser {
    mutable Ch ch;
    typedef EdgeSecondEndChooser<Ch> ChoosersSelfType;
    EdgeSecondEndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
EdgeSecondEndChooser<Ch> edgeSecondEndChoose(Ch ch) { return EdgeSecondEndChooser<Ch>(ch); }

// ... i na podstawie tego, ile sposrod koncow spelnia warunek
template <class Ch> struct Edge0EndChooser {
    mutable Ch ch;
    typedef Edge0EndChooser<Ch> ChoosersSelfType;
    Edge0EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return !ch(g.getEdgeEnds(e).first,g) && !ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge0EndChooser<Ch> edge0EndChoose(Ch ch) { return Edge0EndChooser<Ch>(ch); }


template <class Ch> struct Edge1EndChooser {
    mutable Ch ch;
    typedef Edge1EndChooser<Ch> ChoosersSelfType;
    Edge1EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnds(e).first,g) != ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge1EndChooser<Ch> edge1EndChoose(Ch ch) { return Edge1EndChooser<Ch>(ch); }


template <class Ch> struct Edge2EndChooser {
    mutable Ch ch;
    typedef Edge2EndChooser<Ch> ChoosersSelfType;
    Edge2EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnds(e).first,g) && ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge2EndChooser<Ch> edge2EndChoose(Ch ch) { return Edge2EndChooser<Ch>(ch); }


// caster zwyklego rzutowania miedzy dwoma strukturami info
struct StdCaster {
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest,InfoSour sour) { dest=(InfoDest)sour; }
};

StdCaster stdCast() {return StdCaster(); }

// caster ustawiajacy wartosc domyslna i ignorujacy drugi parametr
struct NoCastCaster {
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest,InfoSour sour) { dest=InfoDest(); }
};

NoCastCaster stdCast(bool arg) { assert(!arg); return NoCastCaster(); }

// wyliczenie wartosci nowego info poprzez wlasna funkcje lub funktor
template <class Fun> struct ObjCaster {
    mutable Fun funktor;
    ObjCaster(Fun afun=Fun()) : funktor(afun) {}
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest, InfoSour sour) { dest=(InfoDest)funktor(sour); }
};

template <class Funktor>
ObjCaster<Funktor> stdCast(Funktor f) { return ObjCaster<Funktor>(f); }

// polowki pelnego linkera, dzialaja w jedna strone

// tylko false jest dopuszczalne - brak polaczenia
struct Std1NoLinker {
    Std1NoLinker(bool arg=false) { assert(!arg);}
    template <class Dest,class Sour>
        void operator()(Dest*,Sour*) {}
};
// ustawia wskaznik na dolaczany obiekt w srodku struktury info obiektu docelowego
template <class Info, class T> struct Std1FieldLinker {
    T Info::* pt;
	Std1FieldLinker(T Info::* awsk=0) : pt(awsk) {}
	template <class Dest, class Sour>
        void operator()(Dest* wsk, Sour* w) {  if (pt && wsk) wsk->info.*pt=(T) w; }
};
// dopisuje powiazania do zewnetrznej! tablicy asocjacyjnej podanej w funkcji tworzacej stdLink
template <class Map> struct Std1AssocLinker{
    Map& map;
    Std1AssocLinker(Map& amap) : map(amap) {}
	template <class Dest, class Sour>
        void operator()(Dest* wsk, Sour* w) {   if (wsk) map[wsk]=w; }
};

// pelny linker, zawiera obiekty laczace nowy element ze starym i odwrotnie
template <class Link1, class Link2> struct Std2Linker {
        mutable Link1 dest2sour;
        mutable Link2 sour2dest;
        Std2Linker(Link1 al1, Link2 al2) : dest2sour(al1), sour2dest(al2) {}
        template <class Dest, class Sour>
            void operator()(Dest* wsk, Sour* w) { dest2sour(wsk,w); sour2dest(w,wsk); }
};

// i jego funkcje tworzace

Std1NoLinker stdLink(bool a1) { return Std1NoLinker(a1); }

template <class Info1, class T1>
Std1FieldLinker<Info1,T1> stdLink(T1 Info1:: *awsk1)
{ return Std1FieldLinker<Info1,T1>(awsk1); }

template <class Map1>
Std1AssocLinker<Map1> stdLink(Map1& tab1)
{ return Std1AssocLinker<Map1>(tab1); }

Std2Linker<Std1NoLinker,Std1NoLinker> stdLink(bool a1, bool a2)
{ return Std2Linker<Std1NoLinker,Std1NoLinker>(Std1NoLinker(a1),Std1NoLinker(a2)); }

template <class Info,class T>
Std2Linker<Std1NoLinker,Std1FieldLinker<Info,T> > stdLink(bool a1, T Info::* awsk)
{ return Std2Linker<Std1NoLinker,Std1FieldLinker<Info,T> >(Std1NoLinker(a1),Std1FieldLinker<Info,T>(awsk)); }

template <class Map>
Std2Linker<Std1NoLinker,Std1AssocLinker<Map> > stdLink(bool a1, Map& tab)
{ return Std2Linker<Std1NoLinker,Std1AssocLinker<Map> >(Std1NoLinker(a1),Std1AssocLinker<Map>(tab)); }


template <class Info1, class T1>
Std2Linker<Std1FieldLinker<Info1,T1>,Std1NoLinker> stdLink(T1 Info1:: *awsk1, bool a2)
{ return Std2Linker<Std1FieldLinker<Info1,T1>,Std1NoLinker>(Std1FieldLinker<Info1,T1>(awsk1),Std1NoLinker(a2)); }

template <class Info1, class T1, class Info,class T>
Std2Linker<Std1FieldLinker<Info1,T1>,Std1FieldLinker<Info,T> > stdLink(T1 Info1::* awsk1, T Info::* awsk)
{ return Std2Linker<Std1FieldLinker<Info1,T1>,Std1FieldLinker<Info,T> >(Std1FieldLinker<Info1,T1>(awsk1),Std1FieldLinker<Info,T>(awsk)); }

template <class Info1, class T1,class Map>
Std2Linker<Std1FieldLinker<Info1,T1>,Std1AssocLinker<Map> > stdLink(T1 Info1::* awsk1, Map& tab)
{ return Std2Linker<Std1FieldLinker<Info1,T1>,Std1AssocLinker<Map> >(Std1FieldLinker<Info1,T1>(awsk1),Std1AssocLinker<Map>(tab)); }


template <class Map1>
Std2Linker<Std1AssocLinker<Map1>,Std1NoLinker> stdLink(Map1& tab1, bool a2)
{ return Std2Linker<Std1AssocLinker<Map1>,Std1NoLinker>(Std1AssocLinker<Map1>(tab1),Std1NoLinker(a2)); }

template <class Map1, class Info,class T>
Std2Linker<Std1AssocLinker<Map1>,Std1FieldLinker<Info,T> > stdLink(Map1& tab1, T Info::* awsk)
{ return Std2Linker<Std1AssocLinker<Map1>,Std1FieldLinker<Info,T> >(Std1AssocLinker<Map1>(tab1),Std1FieldLinker<Info,T>(awsk)); }

template <class Map1, class Map>
Std2Linker<Std1AssocLinker<Map1>,Std1AssocLinker<Map> > stdLink(Map1& tab1, Map& tab)
{ return Std2Linker<Std1AssocLinker<Map1>,Std1AssocLinker<Map> >(Std1AssocLinker<Map1>(tab1),Std1AssocLinker<Map>(tab)); }


class RelClosure {
    public:

    template <class Cont>
    static void empty(Cont& cont, int size)
    {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  cont[i][j]=false;  }

    template <class Cont, class Iter>
    static void empty(Cont& cont, Iter beg,Iter end)
    {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  cont(*i,*j)=false;  }

    template <class Cont>
    static void total(Cont& cont, int size)
    {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  cont[i][j]=true;   }

    template <class Cont,class Iter>
    static void total(Cont& cont, Iter beg,Iter end)
    {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  cont(*i,*j)=true; }

    template <class Cont1, class Cont2, class Cont3>
    static void reland(Cont1& cont1, Cont2& cont2,Cont3& res,int size)
    {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  res[i][j]=cont1[i][j] && cont2[i][j];  }

    template <class Cont1, class Cont2, class Cont3,class Iter>
    static void reland(Cont1& cont1, Cont2& cont2,Cont3& res,Iter beg, Iter end)
    {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  res(*i,*j)=cont1(*i,*j) && cont2(*i,*j);  }

    template <class Cont1, class Cont2, class Cont3>
    static void relor(Cont1& cont1, Cont2& cont2,Cont3& res,int size)
    {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  res[i][j]=cont1[i][j] || cont2[i][j];  }

    template <class Cont1, class Cont2, class Cont3,class Iter>
    static void relor(Cont1& cont1, Cont2& cont2,Cont3& res,Iter beg, Iter end)
    {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  res(*i,*j)=cont1(*i,*j) || cont2(*i,*j);  }

    template <class Cont1, class Cont2, class Cont3>
    static void relxor(Cont1& cont1, Cont2& cont2,Cont3& res,int size)
    {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  res[i][j]=(cont1[i][j] != cont2[i][j]);    }

    template <class Cont1, class Cont2, class Cont3,class Iter>
    static void relxor(Cont1& cont1, Cont2& cont2,Cont3& res,Iter beg, Iter end)
    {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  res(*i,*j)=cont1(*i,*j) != cont2(*i,*j);  }

    template <class Cont1, class Cont2>
    static void relnot(Cont1& cont1, Cont2& res,int size)
    {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  res[i][j]=!cont1[i][j];    }

    template <class Cont1, class Cont2, class Iter>
    static void relnot(Cont1& cont1, Cont2& res,Iter beg, Iter end)
    {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  res(*i,*j)=!cont1(*i,*j);  }

    template <class Cont>
    static void reflexive(Cont& cont, int size)
    {   for(int i=0;i<size;++i) cont[i][i]=true;    }

    template <class Cont, class Iter>
    static void reflexive(Cont& cont,Iter beg, Iter end)
    {   for(Iter i=beg;i!=end;++i) cont(*i,*i)=true; }

    template <class Cont>
    static void symmetric(Cont& cont, int size)
    {   for(int i=0;i<size-1;++i) for(int j=i+1;j<size;++j)  cont[i][j]=cont[j][i]=cont[i][j] || cont[j][i];    }

    template <class Cont,class Iter>
    static void symmetric(Cont& cont,Iter beg, Iter end)
    {   for(Iter i=beg;i!=end;++i) for(Iter j=i;j!=end;++j)
            if (i!=j) cont(*i,*j)=cont(*j,*i)= cont(*i,*j)||cont(*j,*i);    }

    template <class Cont>
    static void transitive(Cont& cont,int size)
    {   for(int k=0;k<size;++k) for(int i=0;i<size;++i) for(int j=0;j<size;++j)
            cont[i][j]=cont[i][j] || (cont[i][k] && cont[k][j]);
    }

    template <class Cont, class Iter>
    static void transitive(Cont& cont,Iter beg, Iter end)
    {   for(Iter k=beg;k!=end;++k) for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)
            cont(*i,*j)=
                cont(*i,*j) || (cont(*i,*k) && cont(*k,*j));
    }

    template <class Cont>
    static void print(Cont& cont, int size,std::ostream& out)
    {   for(int i=0;i<size;++i)
        {   for(int j=0;j<size;++j)  out<< (cont[i][j] ? 1 : 0) << ' ';
            out << '\n';
        }
    }

    template <class Cont,class Iter>
    static void print(Cont& cont, Iter beg, Iter end,std::ostream& out)
    {   for(Iter i=beg;i!=end;++i)
        {   for(Iter j=beg;j!=end;++j)  out<< (cont(*i,*j) ? 1 : 0) << ' ';
            out << '\n';
        }
    }
};

}


#endif
