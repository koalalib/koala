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
#include <functional>

#include "../container/simple.h"
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

template< class VertInfo, class EdgeInfo, class Settings> class Graph;
template< class VertInfo, class EdgeInfo, class Settings> class Vertex;
template< class VertInfo, class EdgeInfo, class Settings> class Edge;


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

#define blackHole   (   (  *((Koala::BlackHole*)(&std::cout)) )  )
//int _KoalaEmptyVertDegree=0;
//void* _KoalaEmptyEdgePoiner=0;

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


template <class T>
struct DummyVar {

    DummyVar<T> operator=(const T& arg) const
    {
        assert(arg==0);
        return *this;
    }

    operator T() const { return 0; }
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
    Elem val;
    typedef ValChooser<Elem> ChoosersSelfType;
    ValChooser(Elem arg) : val(arg) {}
    template<class Graph> bool operator()(Elem elem,const Graph&) const
	{ return elem==val; }
};

template <class Elem>
ValChooser<Elem> stdValChoose(Elem arg) { return ValChooser<Elem>(arg); }


template<class Elem> struct SetChooser {
    Koala::Set<Elem*> set;
    typedef SetChooser<Elem> ChoosersSelfType;
    SetChooser(const Koala::Set<Elem*>&  arg=Koala::Set<Elem*>()) : set(arg) {}
    template<class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return set.isElement(elem); }
};

template <class Elem>
SetChooser<Elem> stdChoose(const Koala::Set<Elem*>& arg) { return SetChooser<Elem>(arg); }


template <class Iter> struct ContainerChooser {
    Iter begin, end;
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
	T val;
    typedef FieldValChooser<Info,T> ChoosersSelfType;
	FieldValChooser(T Info::* arg=0, T arg2=T()) : wsk(arg), val(arg2) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return elem->info.*wsk==val; }
};

template <class Info, class T>
FieldValChooser<Info,T> fieldChoose(T Info::* wsk,T arg)
    { return FieldValChooser<Info,T>(wsk,arg); }

template <class Info, class T> struct FieldValChooserL {
	T Info::* wsk;
	T val;
    typedef FieldValChooserL<Info,T> ChoosersSelfType;
	FieldValChooserL(T Info::* arg=0, T arg2=T()) : wsk(arg), val(arg2) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return elem->info.*wsk<val; }
};

template <class Info, class T>
FieldValChooserL<Info,T> fieldChooseL(T Info::* wsk,T arg)
    { return FieldValChooserL<Info,T>(wsk,arg); }

template <class Info, class T> struct FieldValChooserG {
	T Info::* wsk;
	T val;
    typedef FieldValChooserG<Info,T> ChoosersSelfType;
	FieldValChooserG(T Info::* arg=0, T arg2=T()) : wsk(arg), val(arg2) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return elem->info.*wsk>val; }
};

template <class Info, class T>
FieldValChooserG<Info,T> fieldChooseG(T Info::* wsk,T arg)
    { return FieldValChooserG<Info,T>(wsk,arg); }

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
	Koala::Set<Z> set;
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
	Iter begin, end;
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
    Cont cont;
    typedef AssocHasChooser<Cont> ChoosersSelfType;
    AssocHasChooser(const Cont& arg=Cont()) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem); }
};

template <class Cont>
AssocHasChooser<Cont> assocKeyChoose(const Cont& arg)
{ return AssocHasChooser<Cont>(arg); }


template<class Cont> struct AssocBoolChooser {
    Cont cont;
    typedef AssocBoolChooser<Cont> ChoosersSelfType;
    AssocBoolChooser(const Cont& arg=Cont()) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && (bool)cont[elem]; }
};

template <class Cont>
AssocBoolChooser<Cont> assocChoose(const Cont& arg) { return AssocBoolChooser<Cont>(arg); }


template<class Cont> struct AssocValChooser {
    Cont cont;
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

template<class Cont> struct AssocValChooserL {
    Cont cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooserL<Cont> ChoosersSelfType;
    AssocValChooserL(const Cont& arg=Cont(),typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && cont[elem]<val; }
};

template <class Cont>
AssocValChooserL<Cont> assocChooseL(const Cont& arg, typename Cont::ValType val)
    { return AssocValChooserL<Cont>(arg,val); }

template<class Cont> struct AssocValChooserG {
    Cont cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooserG<Cont> ChoosersSelfType;
    AssocValChooserG(const Cont& arg=Cont(),typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && cont[elem]>val; }
};

template <class Cont>
AssocValChooserG<Cont> assocChooseG(const Cont& arg, typename Cont::ValType val)
    { return AssocValChooserG<Cont>(arg,val); }

template<class Cont> struct AssocSetChooser {
    Cont cont;
    Koala::Set<typename Cont::ValType> set;
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
    Iter begin, end;
    Cont cont;
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
    Cont cont;
    typedef AssocObjChooser<Cont,Obj> ChoosersSelfType;
    AssocObjChooser(const Cont& acont=Cont(), Obj arg=Obj()) : cont(acont), funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return cont.hasKey(elem) && (bool)funktor(cont[elem]); }
};

template <class Cont, class Obj>
AssocObjChooser<Cont,Obj> assocFChoose(const Cont& cont, Obj arg) { return AssocObjChooser<Cont,Obj>(cont,arg); }

// ... i tablica asocjacyjna jest poza chooserem - przekazywana przez wskaznik

template<class Cont> struct AssocHasChooser<Cont*> {
    const Cont *cont;
    typedef AssocHasChooser<Cont*> ChoosersSelfType;
    AssocHasChooser(const Cont* arg=0) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont->hasKey(elem); }
};

template <class Cont>
AssocHasChooser<Cont*> extAssocKeyChoose(const Cont* arg)
{ return AssocHasChooser<Cont*>(arg); }


template<class Cont> struct AssocBoolChooser<Cont*> {
    const Cont *cont;
    typedef AssocBoolChooser<Cont*> ChoosersSelfType;
    AssocBoolChooser(const Cont* arg=0) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont->hasKey(elem) && (bool)cont->operator[](elem); }
};

template <class Cont>
AssocBoolChooser<Cont*> extAssocChoose(const Cont* arg) { return AssocBoolChooser<Cont*>(arg); }


template<class Cont> struct AssocValChooser<Cont*> {
    const Cont *cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooser<Cont*> ChoosersSelfType;
    AssocValChooser(const Cont* arg=0,typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont->hasKey(elem) && cont->operator[](elem)==val; }
};

template <class Cont>
AssocValChooser<Cont*> extAssocChoose(const Cont* arg, typename Cont::ValType val)
    { return AssocValChooser<Cont*>(arg,val); }


template<class Cont> struct AssocValChooserG<Cont*> {
    const Cont *cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooserG<Cont*> ChoosersSelfType;
    AssocValChooserG(const Cont* arg=0,typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont->hasKey(elem) && cont->operator[](elem)>val; }
};

template <class Cont>
AssocValChooserG<Cont*> extAssocChooseG(const Cont* arg, typename Cont::ValType val)
    { return AssocValChooserG<Cont*>(arg,val); }

template<class Cont> struct AssocValChooserL<Cont*> {
    const Cont *cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooserL<Cont*> ChoosersSelfType;
    AssocValChooserL(const Cont* arg=0,typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont->hasKey(elem) && cont->operator[](elem)<val; }
};

template <class Cont>
AssocValChooserL<Cont*> extAssocChooseL(const Cont* arg, typename Cont::ValType val)
    { return AssocValChooserL<Cont*>(arg,val); }

template<class Cont> struct AssocSetChooser<Cont*> {
    const Cont * cont;
    Koala::Set<typename Cont::ValType> set;
    typedef AssocSetChooser<Cont*> ChoosersSelfType;
    AssocSetChooser(const Cont* arg=0,
                    const Koala::Set<typename Cont::ValType>& aset=Koala::Set<typename Cont::ValType>())
                    : cont(arg), set(aset) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont->hasKey(elem) && set.isElement(cont->operator[](elem)); }
};

template <class Cont>
AssocSetChooser<Cont*> extAssocChoose(const Cont* arg,const Koala::Set<typename Cont::ValType>& set)
    { return AssocSetChooser<Cont*>(arg,set); }


template <class Cont, class Iter> struct AssocContainerChooser<Cont*,Iter> {
    Iter begin, end;
    const Cont* cont;
    typedef AssocContainerChooser<Cont*,Iter> ChoosersSelfType;
    AssocContainerChooser(const Cont* acont=0, Iter abegin=Iter(), Iter aend=Iter())
            : cont(acont), begin(abegin), end(aend) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont->hasKey(elem) && std::find(begin,end,cont->operator[](elem))!=end; }
};

template <class Cont, class Iter>
AssocContainerChooser<Cont*,Iter> extAssocChoose(const Cont* cont, Iter begin,Iter end)
    {return AssocContainerChooser<Cont*,Iter>(cont,begin,end); }


template <class Cont, class Obj> struct AssocObjChooser<Cont*,Obj> {
    mutable Obj funktor;
    const Cont* cont;
    typedef AssocObjChooser<Cont*,Obj> ChoosersSelfType;
    AssocObjChooser(const Cont* acont=0, Obj arg=Obj()) : cont(acont), funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return cont->hasKey(elem) && (bool)funktor(cont->operator[](elem)); }
};

template <class Cont, class Obj>
AssocObjChooser<Cont*,Obj> extAssocFChoose(const Cont* cont, Obj arg) { return AssocObjChooser<Cont*,Obj>(cont,arg); }


// choosery operacji logicznych na prostszych chooserach
template <class Ch1, class Ch2> struct OrChooser {
	Ch1 ch1;
	Ch2 ch2;
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
	Ch1 ch1;
	Ch2 ch2;
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
	Ch1 ch1;
	Ch2 ch2;
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
	Ch1 ch1;
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

struct VertDegValChooserL {
    int deg;
    Koala::EdgeDirection type;
    typedef VertDegValChooserL ChoosersSelfType;
    VertDegValChooserL(int adeg=0, Koala::EdgeDirection atype=Koala::EdAll): deg(adeg), type(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, const Graph& g) const
    { return g.deg(v,type)<deg; }
};

VertDegValChooserL vertDegChooseL(int adeg, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegValChooserL(adeg,atype); }

struct VertDegValChooserG {
    int deg;
    Koala::EdgeDirection type;
    typedef VertDegValChooserG ChoosersSelfType;
    VertDegValChooserG(int adeg=0, Koala::EdgeDirection atype=Koala::EdAll): deg(adeg), type(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, const Graph& g) const
    { return g.deg(v,type)>deg; }
};

VertDegValChooserG vertDegChooseG(int adeg, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegValChooserG(adeg,atype); }

template <class Int> struct VertDegSetChooser {
    Koala::Set<Int> set;
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
    Iter begin,end;
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
    Ch ch;
    typedef EdgeFirstEndChooser<Ch> ChoosersSelfType;
    EdgeFirstEndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnds(e).first,g); }
};

template <class Ch>
EdgeFirstEndChooser<Ch> edgeFirstEndChoose(Ch ch) { return EdgeFirstEndChooser<Ch>(ch); }


template <class Ch> struct EdgeSecondEndChooser {
    Ch ch;
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
    Ch ch;
    typedef Edge0EndChooser<Ch> ChoosersSelfType;
    Edge0EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return !ch(g.getEdgeEnds(e).first,g) && !ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge0EndChooser<Ch> edge0EndChoose(Ch ch) { return Edge0EndChooser<Ch>(ch); }


template <class Ch> struct Edge1EndChooser {
    Ch ch;
    typedef Edge1EndChooser<Ch> ChoosersSelfType;
    Edge1EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnds(e).first,g) != ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge1EndChooser<Ch> edge1EndChoose(Ch ch) { return Edge1EndChooser<Ch>(ch); }


template <class Ch> struct Edge2EndChooser {
    Ch ch;
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



}


#endif
