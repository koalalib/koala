#ifndef DEF_STRUCT_H
#define DEF_STRUCT_H

#include <algorithm>
#include <limits>
#include <cassert>
#include <iostream>
#include "../container/assoctab.h"
#include "../container/set.h"


namespace Koala {

typedef unsigned char EdgeDirection; ///< Direction of a edge.
//do not change constans
static const EdgeDirection EdNone = 0x00; ///< Edge direction is not specified.
static const EdgeDirection EdLoop = 0x01; ///< Edge is a loop.
static const EdgeDirection EdUndir = 0x02; ///< Edge is undirected
static const EdgeDirection EdDirIn = 0x04; ///< Edge is directed in a vertex.
static const EdgeDirection EdDirOut = 0x08; ///< Edge is directed out of a vertex.
static const EdgeDirection EdAll = 0x0F; ///< All directions are included.
enum EdgeType {Detached=0x0, Loop=0x1, Undirected=0x2, Directed=0xC}; ///< Types of an edge.


struct EmptyVertInfo {};
std::istream& operator>>(std::istream& is,EmptyVertInfo arg) { return is; }
std::ostream& operator<<(std::ostream& os,EmptyVertInfo arg) { return os; }

struct EmptyEdgeInfo {};
std::istream& operator>>(std::istream& is,EmptyEdgeInfo arg) { return is; }
std::ostream& operator<<(std::ostream& os,EmptyEdgeInfo arg) { return os; }

//W kodzie wlasciwie nie tworzymy obiektow tych klas, tylko wolamy odopwiednie funkcje tworzace
//tam, gdzie metoda wymaga np. choosera, castera czy iteratora

//TODO: sprywatyzowac konstruktory i zafriendowac do odpowiednich funkcji tworzacych


//Przydatny funktor domyslny tam, gdzie metoda chce dostac funktor generujacy jakies info dla
// wierzcholka lub krawedzi
template <class T> class EmptyStruct {
    public:
    T operator()() { return T(); }
    template <class A> T operator()(A) { return T(); }
    template <class A, class B> T operator()(A,B) { return T(); }
    template <class A, class B, class C> T operator()(A,B,C) { return T(); }
    template <class A, class B, class C,class D> T operator() (A,B,C,D) { return T(); }
};

template <class T>
EmptyStruct<T> emptyStruct(T) { EmptyStruct<T>(); }

static char __blackhole;

//Jesli metoda chce iterator do zapisu ciagu, a nas sam ciag nie interesuje, tylko inne efekty uboczne procedury
template <class A> struct BlackHole {
    template <class T>
        BlackHole& operator=(T) { return *this; }
    BlackHole& operator* () { return *this; }
    BlackHole& operator++() { return *this; }
    BlackHole operator++ (int) { return *this; }

    // rowniez moze sluzyc jako zaslepka dla nie interesujacego nas kontenera asocjacyjnego wymaganego w procedurze
    template <class T>
        A& operator[](T) { assert(0); return *((A*)(&__blackhole)); }
    template <class T, class R>
        A& operator()(T,R) { assert(0); return *((A*)(&__blackhole)); }

};

BlackHole<char>& blackHole() { return *((BlackHole<char>*)(&__blackhole)); }
template <class A> BlackHole<A>& blackHole() { return *((BlackHole<A>*)(&__blackhole)); }
template <class A> BlackHole<A>& blackHole(A) { return blackHole<A>(); }
template <class T> bool isBlackHole(const T&) { return false; }
template <class T> bool isBlackHole(const BlackHole<T>& ) { return true; }


// Specjalizacje dla wlasnych klas numerycznych (np. liczb wymiernych) pozwola uzywac ich jako danych
// w algorytmach (np. dlugosci krawedzi). Dlatego w kodach nawet zerowosc jakiejs etykiety sprawdzam metoda
template <class T> class NumberTypeBounds {
    public:

    static T plusInfty() { return std::numeric_limits<T>::max(); }
    static bool isPlusInfty(T arg) {return arg==plusInfty(); }
    static T minusInfty() { return std::numeric_limits<T>::min(); }
    static bool isMinusInfty(T arg) {return arg==minusInfty(); }
    static T zero() { return (T)0; }
    bool isZero(T arg) {return arg==zero(); }
};


// te choosery mozna stosowac dla wierzcholkow i krawedzi
class BoolChooser {
    const bool val;
    public:
    typedef BoolChooser ChoosersSelfType;
    BoolChooser(bool arg) : val(arg) {}
    template<class Elem, class Graph> bool operator()(Elem*,Graph&) { return val;}
};

BoolChooser stdChoose(bool arg) { return BoolChooser(arg); }


template<class Elem> class SetChooser {
    Koala::Set<Elem*> set;
    public:
    typedef SetChooser<Elem> ChoosersSelfType;
    SetChooser(Koala::Set<Elem*>  arg) : set(arg) {}
// albo  SetChooser(Koala::Set<Elem*>& arg)
    template<class Graph> bool operator()(Elem* elem,Graph&)
	{ return set.isElement(elem); }
};

template <class Elem>
SetChooser<Elem> stdChoose( Koala::Set<Elem*> arg) { return SetChooser<Elem>(arg); }


template <class Iter> class ContainerChooser {
    Iter begin, end;
    public:
    typedef ContainerChooser<Iter> ChoosersSelfType;
    ContainerChooser(Iter abegin, Iter aend) : begin(abegin), end(aend) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph&)
	{ return std::find(begin,end,elem)!=end; }
};

template <class Iter>
ContainerChooser<Iter> stdChoose(Iter begin,Iter end)
    {return ContainerChooser<Iter>(begin,end); }

// jesli ktos chce choosera na podstawie wlasnego obiektu lub zwyklej funkcji
template <class Obj> class ObjChooser {
    Obj funktor;
    public:
    typedef ObjChooser<Obj> ChoosersSelfType;
    ObjChooser(Obj arg) : funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return (bool)funktor(elem,graph); }
};

template <class Obj>
ObjChooser<Obj> stdChoose(Obj arg) { return ObjChooser<Obj>(arg); }

// te choosery juz zagladaja do konkretnych pol rekordow info
template <class Info, class T> class FieldValChooser {
	T Info::* wsk;
	T val;
    public:
    typedef FieldValChooser<Info,T> ChoosersSelfType;
	FieldValChooser(T Info::* arg, T arg2) : wsk(arg), val(arg2) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return elem->info.*wsk==val; }
};

template <class Info, class T>
FieldValChooser<Info,T> fieldChoose(T Info::* wsk,T arg)
    { return FieldValChooser<Info,T>(wsk,arg); }

template <class Info, class T> class FielBoolChooser {
	T Info::* wsk;
    public:
    typedef FielBoolChooser<Info,T> ChoosersSelfType;
	FielBoolChooser(T Info::* arg) : wsk(arg){}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return bool(elem->info.*wsk); }
};

template <class Info, class T>
FielBoolChooser<Info,T> fieldChoose(T Info::* wsk) { return FielBoolChooser<Info,T>(wsk); }

// wlasny obiekt lub funkcja, ktora ma sie wykonywac dla konkretnego pola z info
template <class Info, class T, class Obj> class FieldObjChooser {
	T Info::* wsk;
	Obj funktor;
    public:
    typedef FieldObjChooser<Info,T,Obj> ChoosersSelfType;
	FieldObjChooser(T Info::* awsk,Obj afun) : wsk(awsk), funktor(afun) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return (bool)funktor(elem->info.*wsk); }
};

template <class Info, class T, class Obj>
FieldObjChooser<Info,T,Obj> fieldChoose(T Info::*wsk, Obj obj)
    { return FieldObjChooser<Info,T,Obj>(wsk,obj); }


template <class Info, class T, class Z> class FieldSetChooser {
	T Info::* wsk;
	Koala::Set<Z> set;
    public:
    typedef FieldSetChooser<Info,T,Z> ChoosersSelfType;
	FieldSetChooser(T Info::* awsk,Koala::Set<Z> aset) : wsk(awsk), set(aset) {}
//albo FieldSetChooser(T Info::* awsk,Koala::Set<Z>& aset) : wsk(awsk), set(aset) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return set.isElement(elem->info.*wsk); }
};

template <class Info, class T, class Z>
FieldSetChooser<Info,T,Z> fieldChoose(T Info::*wsk, Koala::Set<Z> set)
    { return FieldSetChooser<Info,T,Z>(wsk,set); }


template <class Info, class T, class Iter> class FieldContainerChooser {
	T Info::* wsk;
	Iter begin, end;
    public:
    typedef FieldContainerChooser<Info,T,Iter> ChoosersSelfType;
    FieldContainerChooser(T Info::* awsk, Iter a, Iter b): wsk(awsk), begin(a), end(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return std::find(begin,end,elem->info.*wsk)!=end; }
};

template <class Info, class T, class Iter>
FieldContainerChooser<Info,T,Iter> fieldChoose(T Info::*wsk, Iter b, Iter e)
    { return FieldContainerChooser<Info,T,Iter>(wsk,b,e); }


// choosery decydujace na podstawie wartosci przypisanej elementowi w podanej tablicy asocjacyjnej
template<class Cont> class AssocHasChooser {
    Cont cont;
    public:
    typedef AssocHasChooser<Cont> ChoosersSelfType;
    AssocHasChooser(Cont arg) : cont(arg) {}
// albo  &
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph&)
	{ return cont.hasKey(elem); }
};

template <class Cont>
AssocHasChooser<Cont> assocKeyChoose(Cont arg)
{ return AssocHasChooser<Cont>(arg); }


template<class Cont> class AssocBoolChooser {
    Cont cont;
    public:
    typedef AssocBoolChooser<Cont> ChoosersSelfType;
    AssocBoolChooser(Cont arg) : cont(arg) {}
// albo  &
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph&)
	{ return cont.hasKey(elem) && (bool)cont[elem]; }
};

template <class Cont>
AssocBoolChooser<Cont> assocChoose(Cont arg) { return AssocBoolChooser<Cont>(arg); }


template<class Cont> class AssocValChooser {
    Cont cont;
    typename Cont::ValType val;
    public:
    typedef AssocValChooser<Cont> ChoosersSelfType;
    AssocValChooser(Cont arg,typename Cont::ValType aval) : cont(arg), val(aval) {}
// albo  &
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph&)
	{ return cont.hasKey(elem) && cont[elem]==val; }
};

template <class Cont>
AssocValChooser<Cont> assocChoose(Cont arg, typename Cont::ValType val)
    { return AssocValChooser<Cont>(arg,val); }


template<class Cont> class AssocSetChooser {
    Cont cont;
    Koala::Set<typename Cont::ValType> set;
    public:
    typedef AssocSetChooser<Cont> ChoosersSelfType;
    AssocSetChooser(Cont arg,Koala::Set<typename Cont::ValType> aset) : cont(arg), set(aset) {}
// albo  &
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph&)
	{ return cont.hasKey(elem) && set.isElement(cont[elem]); }
};

template <class Cont>
AssocSetChooser<Cont> assocChoose(Cont arg, Koala::Set<typename Cont::ValType> set)
    { return AssocSetChooser<Cont>(arg,set); }


template <class Cont, class Iter> class AssocContainerChooser {
    Iter begin, end;
    Cont cont;
    public:
    typedef AssocContainerChooser<Cont,Iter> ChoosersSelfType;
    AssocContainerChooser(Cont acont, Iter abegin, Iter aend) : cont(acont), begin(abegin), end(aend) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph&)
	{ return cont.hasKey(elem) && std::find(begin,end,cont[elem])!=end; }
};

template <class Cont, class Iter>
AssocContainerChooser<Cont,Iter> assocChoose(Cont cont, Iter begin,Iter end)
    {return AssocContainerChooser<Cont,Iter>(cont,begin,end); }


template <class Cont, class Obj> class AssocObjChooser {
    Obj funktor;
    Cont cont;
    public:
    typedef AssocObjChooser<Cont,Obj> ChoosersSelfType;
    AssocObjChooser(Cont acont, Obj arg) : cont(acont), funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return cont.hasKey(elem) && (bool)funktor(cont[elem]); }
};

template <class Cont, class Obj>
AssocObjChooser<Cont,Obj> assocChoose(Cont cont, Obj arg) { return AssocObjChooser<Cont,Obj>(cont,arg); }


// choosery operacji logicznych na prostszych chooserach
template <class Ch1, class Ch2> class OrChooser {
	Ch1 ch1;
	Ch2 ch2;
    public:
    typedef OrChooser<Ch1,Ch2> ChoosersSelfType;
	OrChooser(Ch1 a, Ch2 b) : ch1(a), ch2(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return (ch1(elem,graph) || ch2(elem,graph)); }
};

template <class  Ch1, class Ch2>
OrChooser<Ch1,Ch2> orChoose(Ch1 a, Ch2 b) { return OrChooser<Ch1,Ch2>(a,b); }

template <class  Ch1, class Ch2>
OrChooser<typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType>
    operator||(Ch1 a, Ch2 b) { return OrChooser<Ch1,Ch2>(a,b); }


template <class Ch1, class Ch2> class AndChooser {
	Ch1 ch1;
	Ch2 ch2;
    public:
    typedef AndChooser<Ch1,Ch2> ChoosersSelfType;
	AndChooser(Ch1 a, Ch2 b) : ch1(a), ch2(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return (ch1(elem,graph) && ch2(elem,graph)); }
};

template <class  Ch1, class Ch2>
AndChooser<Ch1,Ch2> andChoose(Ch1 a, Ch2 b) { return AndChooser<Ch1,Ch2>(a,b); }

template <class  Ch1, class Ch2>
AndChooser<typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType>
    operator&&(Ch1 a, Ch2 b) { return AndChooser<Ch1,Ch2>(a,b); }


template <class Ch1, class Ch2> class XorChooser {
	Ch1 ch1;
	Ch2 ch2;
    public:
    typedef XorChooser<Ch1,Ch2> ChoosersSelfType;
	XorChooser(Ch1 a, Ch2 b) : ch1(a), ch2(b) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return (ch1(elem,graph) != ch2(elem,graph)); }
};

template <class  Ch1, class Ch2>
XorChooser<Ch1,Ch2> xorChoose(Ch1 a, Ch2 b) { return XorChooser<Ch1,Ch2>(a,b); }

template <class  Ch1, class Ch2>
XorChooser<typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType>
    operator^(Ch1 a, Ch2 b) { return XorChooser<Ch1,Ch2>(a,b); }


template <class Ch1> class NotChooser {
	Ch1 ch1;
    public:
    typedef NotChooser<Ch1> ChoosersSelfType;
	NotChooser(Ch1 a) : ch1(a) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,Graph& graph)
	{ return !ch1(elem,graph); }
};

template <class  Ch1>
NotChooser<Ch1> notChoose(Ch1 a) { return NotChooser<Ch1>(a); }

template <class  Ch1>
NotChooser<typename Ch1::ChoosersSelfType>
    operator!(Ch1 a) { return NotChooser<Ch1>(a); }


// choosery dla wierzcholkow
class VertDegValChooser {
    int deg;
    Koala::EdgeDirection type;
    public:
    typedef VertDegValChooser ChoosersSelfType;
    VertDegValChooser(int adeg, Koala::EdgeDirection atype): deg(adeg), type(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, Graph& g)
    { return g.deg(v,type)==deg; }
};

VertDegValChooser vertDegChoose(int adeg, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegValChooser(adeg,atype); }


template <class Int> class VertDegSetChooser {
    Koala::Set<Int> set;
    Koala::EdgeDirection type;
    public:
    typedef VertDegSetChooser<Int> ChoosersSelfType;
    VertDegSetChooser(Koala::Set<Int> aset, Koala::EdgeDirection atype):
// albo  VertDegSetChooser(Koala::Set<Int>& aset, Koala::EdgeDirection atype=Koala::EdAll):
                                            set(aset), type(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, Graph& g)
    { return set.isElement(g.deg(v,type)); }
};

template <class Int>
VertDegSetChooser<Int> vertDegChoose(Koala::Set<Int> aset, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegSetChooser<Int>(aset,atype); }


template <class Iter> class VertDegContainerChooser {
    Iter begin,end;
    Koala::EdgeDirection typ;
    public:
    typedef VertDegContainerChooser<Iter> ChoosersSelfType;
    VertDegContainerChooser(Iter abeg, Iter aend, Koala::EdgeDirection atype):
                                            begin(abeg), end(aend), typ(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, Graph& g)
    { return std::find(begin,end,g.deg(v,typ))!=end; }
};

template <class Iter>
VertDegContainerChooser<Iter> vertDegChoose(Iter begin, Iter end, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegContainerChooser<Iter>(begin,end,atype); }

// decyzja podejmowana na podstawie wartosci funkcji lub obiektu funktora policzonego na stopniu wierzcholka
template <class Obj> class VertDegFunctorChooser {
    Obj funktor;
    Koala::EdgeDirection typ;
    public:
    typedef VertDegFunctorChooser<Obj> ChoosersSelfType;
    VertDegFunctorChooser(Obj afun, Koala::EdgeDirection atype):
                                            funktor(afun), typ(atype) {}
    template <class Graph>
    bool operator()(typename Graph::PVertex v, Graph& g)
    { return (bool)funktor(g.deg(v,typ)); }
};

template <class Obj>
VertDegFunctorChooser<Obj> vertDegChoose(Obj afun, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegFunctorChooser<Obj>(afun,atype); }

// choosery dla krawedzi
class EdgeTypeChooser {
    Koala::EdgeDirection mask;
    public:
    typedef EdgeTypeChooser ChoosersSelfType;
    EdgeTypeChooser(Koala::EdgeDirection amask) : mask(amask) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, Graph& g)
    { return bool(g.getType(e) & mask); }
};

EdgeTypeChooser edgeTypeChoose(Koala::EdgeDirection mask) { return EdgeTypeChooser(mask); }

// choosery dla krawedzi sprawdzajace warunki dla jej koncow
template <class Ch> class EdgeFirstEndChooser {
    Ch ch;
    public:
    typedef EdgeFirstEndChooser<Ch> ChoosersSelfType;
    EdgeFirstEndChooser(Ch funktor) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, Graph& g)
    { return ch(g.getEdgeEnds(e).first,g); }
};

template <class Ch>
EdgeFirstEndChooser<Ch> edgeFirstEndChoose(Ch ch) { return EdgeFirstEndChooser<Ch>(ch); }


template <class Ch> class EdgeSecondEndChooser {
    Ch ch;
    public:
    typedef EdgeSecondEndChooser<Ch> ChoosersSelfType;
    EdgeSecondEndChooser(Ch funktor) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, Graph& g)
    { return ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
EdgeSecondEndChooser<Ch> edgeSecondEndChoose(Ch ch) { return EdgeSecondEndChooser<Ch>(ch); }

// ... i na podstawie tego, ile sposrod koncow spelnia warunek
template <class Ch> class Edge0EndChooser {
    Ch ch;
    public:
    typedef Edge0EndChooser<Ch> ChoosersSelfType;
    Edge0EndChooser(Ch funktor) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, Graph& g)
    { return !ch(g.getEdgeEnds(e).first,g) && !ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge0EndChooser<Ch> edge0EndChoose(Ch ch) { return Edge0EndChooser<Ch>(ch); }


template <class Ch> class Edge1EndChooser {
    Ch ch;
    public:
    typedef Edge1EndChooser<Ch> ChoosersSelfType;
    Edge1EndChooser(Ch funktor) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, Graph& g)
    { return ch(g.getEdgeEnds(e).first,g) != ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge1EndChooser<Ch> edge1EndChoose(Ch ch) { return Edge1EndChooser<Ch>(ch); }


template <class Ch> class Edge2EndChooser {
    Ch ch;
    public:
    typedef Edge2EndChooser<Ch> ChoosersSelfType;
    Edge2EndChooser(Ch funktor) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, Graph& g)
    { return ch(g.getEdgeEnds(e).first,g) && ch(g.getEdgeEnds(e).second,g); }
};

template <class Ch>
Edge2EndChooser<Ch> edge2EndChoose(Ch ch) { return Edge2EndChooser<Ch>(ch); }


// caster zwyklego rzutowania miedzy dwoma strukturami info
class StdCaster {
    public:
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest,InfoSour sour) { dest=(InfoDest)sour; }
};

StdCaster stdCast() {return StdCaster(); }

// caster ustawiajacy wartosc domyslna i ignorujacy drugi parametr
class NoCastCaster {
    public:
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest,InfoSour sour) { dest=InfoDest(); }
};

NoCastCaster stdCast(bool arg) { assert(!arg); return NoCastCaster(); }

// wyliczenie wartosci nowego info poprzez wlasna funkcje lub funktor
template <class Fun> class ObjCaster {
    Fun funktor;
    public:
    ObjCaster(Fun afun) : funktor(afun) {}
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest, InfoSour sour) { dest=(InfoDest)funktor(sour); }
};

template <class Funktor>
ObjCaster<Funktor> stdCast(Funktor f) { return ObjCaster<Funktor>(f); }

// polowki pelnego linkera, dzialaja w jedna strone

// tylko false jest dopuszczalne - brak polaczenia
class pomNoLinker {
    public:
    pomNoLinker(bool arg) { assert(!arg);}
    template <class Dest,class Sour>
        void operator()(Dest*,Sour*) {}
};
// ustawia wskaznik na dolaczany obiekt w srodku struktury info obiektu docelowego
template <class Info, class T> class pomFieldLinker {
    T Info::* pt;
    public:
	pomFieldLinker(T Info::* awsk) : pt(awsk) {}
	template <class Dest, class Sour>
        void operator()(Dest* wsk, Sour* w) {  if (pt && wsk) wsk->info.*pt=(T) w; }
};
// dopisuje powiazania do zewnetrznej! tablicy asocjacyjnej podanej w funkcji tworzacej stdLink
template <class Map> class pomAssocLinker{
    public:
    Map& map;
    pomAssocLinker(Map& amap) : map(amap) {}
	template <class Dest, class Sour>
        void operator()(Dest* wsk, Sour* w) {   if (wsk) map[wsk]=w; }
};

// pelny linker, zawiera obiekty laczace nowy element ze starym i odwrotnie
template <class Link1, class Link2> class Std2Linker {
    public:
        Link1 dest2sour;
        Link2 sour2dest;
        Std2Linker(Link1 al1, Link2 al2) : dest2sour(al1), sour2dest(al2) {}
        template <class Dest, class Sour>
            void operator()(Dest* wsk, Sour* w) { dest2sour(wsk,w); sour2dest(w,wsk); }
};

// i jego funkcje tworzace
Std2Linker<pomNoLinker,pomNoLinker> stdLink(bool a1, bool a2)
{ return Std2Linker<pomNoLinker,pomNoLinker>(pomNoLinker(a1),pomNoLinker(a2)); }

template <class Info,class T>
Std2Linker<pomNoLinker,pomFieldLinker<Info,T> > stdLink(bool a1, T Info::* awsk)
{ return Std2Linker<pomNoLinker,pomFieldLinker<Info,T> >(pomNoLinker(a1),pomFieldLinker<Info,T>(awsk)); }

template <class Map>
Std2Linker<pomNoLinker,pomAssocLinker<Map> > stdLink(bool a1, Map& tab)
{ return Std2Linker<pomNoLinker,pomAssocLinker<Map> >(pomNoLinker(a1),pomAssocLinker<Map>(tab)); }


template <class Info1, class T1>
Std2Linker<pomFieldLinker<Info1,T1>,pomNoLinker> stdLink(T1 Info1:: *awsk1, bool a2)
{ return Std2Linker<pomFieldLinker<Info1,T1>,pomNoLinker>(pomFieldLinker<Info1,T1>(awsk1),pomNoLinker(a2)); }

template <class Info1, class T1, class Info,class T>
Std2Linker<pomFieldLinker<Info1,T1>,pomFieldLinker<Info,T> > stdLink(T1 Info1::* awsk1, T Info::* awsk)
{ return Std2Linker<pomFieldLinker<Info1,T1>,pomFieldLinker<Info,T> >(pomFieldLinker<Info1,T1>(awsk1),pomFieldLinker<Info,T>(awsk)); }

template <class Info1, class T1,class Map>
Std2Linker<pomFieldLinker<Info1,T1>,pomAssocLinker<Map> > stdLink(T1 Info1::* awsk1, Map& tab)
{ return Std2Linker<pomFieldLinker<Info1,T1>,pomAssocLinker<Map> >(pomFieldLinker<Info1,T1>(awsk1),pomAssocLinker<Map>(tab)); }


template <class Map1>
Std2Linker<pomAssocLinker<Map1>,pomNoLinker> stdLink(Map1& tab1, bool a2)
{ return Std2Linker<pomAssocLinker<Map1>,pomNoLinker>(pomAssocLinker<Map1>(tab1),pomNoLinker(a2)); }

template <class Map1, class Info,class T>
Std2Linker<pomAssocLinker<Map1>,pomFieldLinker<Info,T> > stdLink(Map1& tab1, T Info::* awsk)
{ return Std2Linker<pomAssocLinker<Map1>,pomFieldLinker<Info,T> >(pomAssocLinker<Map1>(tab1),pomFieldLinker<Info,T>(awsk)); }

template <class Map1, class Map>
Std2Linker<pomAssocLinker<Map1>,pomAssocLinker<Map> > stdLink(Map1& tab1, Map& tab)
{ return Std2Linker<pomAssocLinker<Map1>,pomAssocLinker<Map> >(pomAssocLinker<Map1>(tab1),pomAssocLinker<Map>(tab)); }



}


#endif
