#ifndef KOALA_DEF_STRUCT_H
#define KOALA_DEF_STRUCT_H

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


//Glowne procedury biblioteki leza w namespace Koala
//Procedury wejscia/wyjscia w Koala::IO
//Pewne wewnetrzne mechanizmy, w ktorych uzytkownik grzebac nie powinien zamykamy w podnamespaceach Privates
namespace Koala {

typedef unsigned char EdgeType; // mozliwe typy krawedzi grafu
typedef unsigned char EdgeDirection; // ... i ich orientacje wzgledem wierzcholka

static const EdgeDirection EdNone = 0x00; ///< Edge direction is not specified.
static const EdgeDirection EdLoop = 0x01; ///< Edge is a loop.
static const EdgeDirection EdUndir = 0x02; ///< Edge is undirected
static const EdgeDirection EdDirIn = 0x04; ///< Edge is directed in a vertex.
static const EdgeDirection EdDirOut = 0x08; ///< Edge is directed out of a vertex.
static const EdgeDirection EdAll = 0x0F; ///< All directions are included.

static const EdgeType Detached=0x0;
static const EdgeType Loop=0x1;
static const EdgeType Undirected=0x2;
static const EdgeType Directed=0xC;

// Domyslne struktury (puste) pola info z krawedzi/wierzcholka
struct EmptyVertInfo {};
// ich operatory odczytu/zapisu ze strumienia (na potrzeby formatu tekstowego grafu z modulu text.h)
//std::istream& operator>>(std::istream& is,EmptyVertInfo arg) { return is; }
//std::ostream& operator<<(std::ostream& os,EmptyVertInfo arg) { return os; }

struct EmptyEdgeInfo {};
//std::istream& operator>>(std::istream& is,EmptyEdgeInfo arg) { return is; }
//std::ostream& operator<<(std::ostream& os,EmptyEdgeInfo arg) { return os; }


// W glownej strukturze grafu wierzcholki i krawedzie maja pole publiczne (dane skojarzone z wierz/kraw)
// typu Vert/EdgeInfo. Klasa Settings dostarcza wytycznych parametryzujacych wewnetrzne procedury grafu
template< class VertInfo, class EdgeInfo, class Settings> class Graph;
template< class VertInfo, class EdgeInfo, class Settings> class Vertex;
template< class VertInfo, class EdgeInfo, class Settings> class Edge;

template <EdgeType mask, bool matr> class GrDefaultSettings;

//Wytyczne parametryzujace dzialanie algorytmow biblioteki. Z reguly klasy z procedurami maja postac
//NazwaPar<DefaultStructs> oraz pochodna klase Nazwa dzialajaca z ustawieniem DefaultStructs=AlgsDefaultSettings
class AlgsDefaultSettings {
    public:

    // typ klasy tablicy asocjacyjnej przypisujacej np. wierz/kraw wartosci typu B
    template <class A, class B> class AssocCont {
        public:
        typedef AssocArray<A,B> Type;
// TODO: nie usuwac komentarzy (przykladowe uzycia)
//        typedef AssocTable < BiDiHashMap<A,B> > Type;
//            typedef PseudoAssocArray<A,B,AssocTable<std::map<A,int> > > Type;

    };

    // typ 2-wymiarowej tablicy assoc. o kluczu A i wartosci B. Kluczami sa pary uporzadkowane o roznych elementach
    template <class A, class B> class TwoDimNoDiagAssocCont {
        public:
        typedef AssocMatrix<A,B,AMatrNoDiag> Type;
    };

    // typ 2-wymiarowej tablicy assoc. o kluczu A i wartosci B. Kluczami sa dowolne pary uporzadkowane
    template <class A, class B> class TwoDimFullAssocCont {
        public:
        typedef AssocMatrix<A,B,AMatrFull> Type;
    };


    // j.w. dla kluczy - par nieuporzadkowanych
    template <class A, class B> class TwoDimTriangleAssocCont {
        public:
        typedef AssocMatrix<A,B,AMatrTriangle> Type;
    };

    // typ 2-wymiarowej tablicy assoc. o kluczu A i wartosci B. Kluczami sa dowolne pary uporzadkowane
    template <class A, class B> class TwoDimClTriangleAssocCont {
        public:
        typedef AssocMatrix<A,B,AMatrClTriangle> Type;
    };

    // typ grafu pomocniczego stosowanego wewnatrz procedury
    template <class A, class B,EdgeType mask,bool matr> class LocalGraph {
        public:
        typedef Graph<A,B,GrDefaultSettings<mask,matr> > Type;

    };

    // czy dostosowywac rozmiar pamieci wyjsciowych tablic asocjacyjnych
    enum { ReserveOutAssocCont=true };

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

    // wybrany do uzytku wewnetrznego algorytm sortowania tablic
    template <class Iterator>
    static void sort ( Iterator first, Iterator last )
    { std::make_heap(first,last); std::sort_heap(first,last);}

    // ... i to samo z dostarczonym porownywaczem
    template <class Iterator, class Comp>
    static void sort ( Iterator first, Iterator last, Comp comp )
    { std::make_heap(first,last,comp); std::sort_heap(first,last,comp);}

};


//Przydatny funktor domyslny tam, gdzie jakas metoda chce dostac np. funktor generujacy  info dla
// wierzcholka lub krawedzi. Zwraca wartosc podana w argumencie konstruktora. Dziala dla liczb argumentow 0-6
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

// funkcja tworzaca powyzszy funktor
template <class T>
ConstFunctor<T> constFun(const T& a=T()) { return ConstFunctor<T>(a); }

//Jesli metoda chce dostac argument wyjsciowy (np. iterator do zapisu ciagu, tablice asocjacyjna)
// a nas te wartosci nie interesuja, tylko inne efekty uboczne procedury
// Uniwersalna zaslepka w wielu procedurach biblioteki, pozwala uniknac zbednych przeciazen nazw z roznymi
// zestawami parametrow
struct BlackHole : public std::iterator<std::output_iterator_tag,void,void,void,void> {
    template <class T>
    BlackHole& operator=(T)  { return *this; }
    BlackHole& operator* ()  { return *this; }
    BlackHole& operator++()  { return *this; }
    BlackHole operator++ (int)  { return *this; }
    BlackHole() {}

    // BlackHole rowniez moze sluzyc jako zaslepka dla nie interesujacego nas kontenera asocjacyjnego wymaganego w procedurze
    // te metody nigdy nie powinny byc wykonane, sa potrzebne jedynie by kod sie kompilowal
    template <class T>
        BlackHole& operator[](T) { assert(0); return *this; }
    template <class T, class R>
        BlackHole& operator()(T,R) { assert(0); return *this; }
    // BlackHole "potrafi" przekonwertowac sie na dowolny typ - uwaga j.w.
    template <class T>
        operator T() { assert(0); return T(); }

    void reserve(int) {}

};

// makra blackHole mozna uzyawc jak zmiennej globalnej typu BlackHole
#define blackHole   (   (  *((Koala::BlackHole*)(&std::cout)) )  )

// sprawdzanie w szablonie, czy argument jest typu BlackHole
template <class T> bool isBlackHole(const T&) { return false; }
bool isBlackHole(const BlackHole&) { return true; }

// Jesli szablon procedury dostal BlackHole zamiast argumentu (kontenera), a procedura potrzebuje do dzialania
// tego kontenera - tworzy go sobie sama lokalnie. Ta klasa pozwala sprawdzic, czy taki przypadek zaszedl i przelaczyc
// kontener na odpowiednio: dostarczony lub lokalny (nie bedacy BlackHolem).
template <class Cont1, class Cont2> struct BlackHoleSwitch {
    typedef Cont1 Type; // typ kontenera, na ktorym bedziemy dzialac
    static Cont1& get(Cont1& a, Cont2& b) { return a; } // i referencja do niego
};

template <class Cont2> struct BlackHoleSwitch<BlackHole, Cont2 > {
    typedef Cont2 Type;
    static Cont2& get(BlackHole& a, Cont2& b) { return b; }
};


// Choosery sa strukturami funkcyjnymi zwracajacymi true/false (poprzez operator()(Element,const Graph&) ) dla wierzcholkow/krawedzi grafu. Sluza np.
// w procedurach wybierania podgrafow, kopiowania podgrafow ... W kodzie powinno sie je tworzyc ich "funkcjami twozacymi"


// Choosery uniwersalne tj. mozna stosowac dla wierzcholkow i krawedzi

struct BoolChooser { // zwraca ustalona wartosc true lub false
    bool val;
    typedef BoolChooser ChoosersSelfType; // kazdy chooser ma swoj wlasny typ zdefiniowany jako ChoosersSelfType
    BoolChooser(bool arg=false) : val(arg) {}
    template<class Elem, class Graph> bool operator()(Elem*,const Graph&) const { return val;} // glowny operator choosera, testujacy prawdziwosc jego predykatu
};

// jego funkcja tworzaca
// TODO: sprawdzic, czy rozne przeciazenia funkcji tworzacych nie wywoluja niejednoznacznosci w rozstrzyganiu przeciazen
BoolChooser stdChoose(bool arg) { return BoolChooser(arg); }


template<class Elem> struct ValChooser { // sprawdza, czy testowany element to podana (ustalona) wartosc
    Elem val;
    typedef ValChooser<Elem> ChoosersSelfType;
    ValChooser(Elem arg) : val(arg) {}
    template<class Graph> bool operator()(Elem elem,const Graph&) const
	{ return elem==val; }
};

template <class Elem>
ValChooser<Elem> stdValChoose(Elem arg) { return ValChooser<Elem>(arg); }


template<class Elem> struct SetChooser { // sprawdza, czy testowany element jest w podanym zbiorze (Koala::Set)
    Koala::Set<Elem*> set;
    typedef SetChooser<Elem> ChoosersSelfType;
    SetChooser(const Koala::Set<Elem*>&  arg=Koala::Set<Elem*>()) : set(arg) {}
    template<class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return set.isElement(elem); }
};

template <class Elem>
SetChooser<Elem> stdChoose( Koala::Set<Elem*> arg) { return SetChooser<Elem>(arg); }


template <class Iter> struct ContainerChooser { // sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
    Iter begin, end;
    typedef ContainerChooser<Iter> ChoosersSelfType;
    ContainerChooser(Iter abegin=Iter(), Iter aend=Iter()) : begin(abegin), end(aend) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return std::find(begin,end,elem)!=end; }
};

template <class Iter>
ContainerChooser<Iter> stdChoose(Iter begin,Iter end)
    {return ContainerChooser<Iter>(begin,end); }


template <class Obj> struct ObjChooser { // sprawdza wartosc podanego obiektu funkcyjnego dla testowanego elementu
    // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
    mutable Obj funktor;
    typedef ObjChooser<Obj> ChoosersSelfType;
    ObjChooser(Obj arg=Obj()) : funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return (bool)funktor(elem,graph); }
};

template <class Obj> // liera F w nazwie - dla chooserow dzialajaych z funktorami
ObjChooser<Obj> stdFChoose(Obj arg) { return ObjChooser<Obj>(arg); }

// te choosery juz zagladaja do konkretnych pol rekordow info wierz/kraw
template <class Info, class T> struct FieldValChooser { // test, czy pole w info ma podana wartosc
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

template <class Info, class T> struct FieldValChooserL { // test, czy pole w info jest mniejsze od podanej wartosci
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

template <class Info, class T> struct FieldValChooserG { // test, czy pole w info jest wieksze od podanej wartosci
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

template <class Info, class T> struct FielBoolChooser { // test, czy pole w info ma wartosc prawda (jego typ musi byc konwertowalny do bool)
	T Info::* wsk;
    typedef FielBoolChooser<Info,T> ChoosersSelfType;
	FielBoolChooser(T Info::* arg=0) : wsk(arg){}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return bool(elem->info.*wsk); }
};

template <class Info, class T>
FielBoolChooser<Info,T> fieldChoose(T Info::* wsk) { return FielBoolChooser<Info,T>(wsk); }

// wlasny obiekt lub funkcja, ktora ma sie wykonywac dla konkretnego pola z info
// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
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


template <class Info, class T, class Z> struct FieldSetChooser { // test, czy podane pole w info ma wartosc z danego zbioru (Koala::Set)
	T Info::* wsk;
	Koala::Set<Z> set;
    typedef FieldSetChooser<Info,T,Z> ChoosersSelfType;
	FieldSetChooser(T Info::* awsk=0,const Koala::Set<Z>& aset=Koala::Set<Z>()) : wsk(awsk), set(aset) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return set.isElement(elem->info.*wsk); }
};

template <class Info, class T, class Z>
FieldSetChooser<Info,T,Z> fieldChoose(T Info::*wsk, Koala::Set<Z> set)
    { return FieldSetChooser<Info,T,Z>(wsk,set); }


template <class Info, class T, class Iter> struct FieldContainerChooser {  // sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
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
template<class Cont> struct AssocHasChooser { // test, czy element jest kluczem znajdujacym sie w tablicy
    Cont cont;
    typedef AssocHasChooser<Cont> ChoosersSelfType;
    AssocHasChooser(const Cont& arg=Cont()) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem); }
};

template <class Cont>
AssocHasChooser<Cont> assocKeyChoose( Cont arg)
{ return AssocHasChooser<Cont>(arg); }


template<class Cont> struct AssocBoolChooser {  // test, czy element jest kluczem znajdujacym sie w tablicy,
//    a przypisana w niej wartosc odpowiada true (typ wartosci musi byc konwertowalny do bool)
    Cont cont;
    typedef AssocBoolChooser<Cont> ChoosersSelfType;
    AssocBoolChooser(const Cont& arg=Cont()) : cont(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && (bool)cont[elem]; }
};

template <class Cont>
AssocBoolChooser<Cont> assocChoose( Cont arg) { return AssocBoolChooser<Cont>(arg); }


template<class Cont> struct AssocValChooser { // test, czy element jest kluczem znajdujacym sie w tablicy,
//    a przypisana w niej wartosc jest rowna zadanej
    Cont cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooser<Cont> ChoosersSelfType;
    AssocValChooser(const Cont& arg=Cont(),typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && cont[elem]==val; }
};

template <class Cont>
AssocValChooser<Cont> assocChoose( Cont arg, typename Cont::ValType val)
    { return AssocValChooser<Cont>(arg,val); }

template<class Cont> struct AssocValChooserL { // test, czy element jest kluczem znajdujacym sie w tablicy,
//    a przypisana w niej wartosc jest mniejsza od zadanej
    Cont cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooserL<Cont> ChoosersSelfType;
    AssocValChooserL(const Cont& arg=Cont(),typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && cont[elem]<val; }
};

template <class Cont>
AssocValChooserL<Cont> assocChooseL( Cont arg, typename Cont::ValType val)
    { return AssocValChooserL<Cont>(arg,val); }

template<class Cont> struct AssocValChooserG { // test, czy element jest kluczem znajdujacym sie w tablicy,
//    a przypisana w niej wartosc jest wieksza od zadanej
    Cont cont;
    typename Cont::ValType val;
    typedef typename Cont::ValType SelfValType;
    typedef AssocValChooserG<Cont> ChoosersSelfType;
    AssocValChooserG(const Cont& arg=Cont(),typename Cont::ValType aval=SelfValType()) : cont(arg), val(aval) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && cont[elem]>val; }
};

template <class Cont>
AssocValChooserG<Cont> assocChooseG( Cont arg, typename Cont::ValType val)
    { return AssocValChooserG<Cont>(arg,val); }

template<class Cont> struct AssocSetChooser { // test, czy element jest kluczem znajdujacym sie w tablicy,
//    a przypisana w niej wartosc jest elementem podanego zbioru (Koala::Set)
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
AssocSetChooser<Cont> assocChoose( Cont arg,const Koala::Set<typename Cont::ValType>& set)
    { return AssocSetChooser<Cont>(arg,set); }


template <class Cont, class Iter> struct AssocContainerChooser { // test, czy element jest kluczem znajdujacym sie w tablicy,
//    a przypisana w niej wartosc lezy w przedziale miedzy podanymi iteratorami - uzywa STLowego find
    Iter begin, end;
    Cont cont;
    typedef AssocContainerChooser<Cont,Iter> ChoosersSelfType;
    AssocContainerChooser(const Cont& acont=Cont(), Iter abegin=Iter(), Iter aend=Iter())
            : cont(acont), begin(abegin), end(aend) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph&) const
	{ return cont.hasKey(elem) && std::find(begin,end,cont[elem])!=end; }
};

template <class Cont, class Iter>
AssocContainerChooser<Cont,Iter> assocChoose( Cont cont, Iter begin,Iter end)
    {return AssocContainerChooser<Cont,Iter>(cont,begin,end); }


template <class Cont, class Obj> struct AssocObjChooser { // sprawdza wartosc podanemgo obiektu funkcyjnego przypisana w tablicy asocjacyjnej testowanemu elementowi
    // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
    mutable Obj funktor;
    Cont cont;
    typedef AssocObjChooser<Cont,Obj> ChoosersSelfType;
    AssocObjChooser(const Cont& acont=Cont(), Obj arg=Obj()) : cont(acont), funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return cont.hasKey(elem) && (bool)funktor(cont[elem]); }
};

template <class Cont, class Obj>
AssocObjChooser<Cont,Obj> assocFChoose( Cont cont, Obj arg) { return AssocObjChooser<Cont,Obj>(cont,arg); }

// Wszystkie choosery operujace na tablicach asocjacyjnych maja wersje dzialajace z zewnetrzna tablica
// podawana przez wskaznik. Unikamy kopiowania tablic, uzywajac trzeba uwazac, by tablica wciaz zyla
// przez caly czas zycia choosera. Funkcje tworzace maja przedrostek ext i pobieraja adres tablicy

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
    // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
    mutable Obj funktor;
    const Cont* cont;
    typedef AssocObjChooser<Cont*,Obj> ChoosersSelfType;
    AssocObjChooser(const Cont* acont=0, Obj arg=Obj()) : cont(acont), funktor(arg) {}
    template<class Elem, class Graph> bool operator()(Elem* elem,const Graph& graph) const
	{ return cont->hasKey(elem) && (bool)funktor(cont->operator[](elem)); }
};

template <class Cont, class Obj>
AssocObjChooser<Cont*,Obj> extAssocFChoose(const Cont* cont, Obj arg) { return AssocObjChooser<Cont*,Obj>(cont,arg); }


// Predykaty chooserow mozna laczyc operatorami logicznymi.
// Choosery operacji logicznych na prostszych chooserach
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

// w kodzie funkcje tworzace zlozonych chooserow mozna zastapic odpowiednimi operatorami logicznymi
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


// choosery dow wybierania wierzcholkow
struct VertDegValChooser { // testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma podana wartosc
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

struct VertDegValChooserL { // testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc mniejsza od zadanej
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

struct VertDegValChooserG { // testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc wieksza od zadanej
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

template <class Int> struct VertDegSetChooser { // testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc z podanego zbioru
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
VertDegSetChooser<Int> vertDegChoose( Koala::Set<Int> aset, Koala::EdgeDirection atype=Koala::EdAll)
{ return VertDegSetChooser<Int>(aset,atype); }


template <class Iter> struct VertDegContainerChooser { // testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
    // z zakresu miedzy podanymi iteratorami - uzywa STLowego find
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

// decyzja podejmowana na podstawie wartosci obiektu funktora policzonego na stopniu wierzcholka
// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
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

// choosery do wybierania krawedzi
struct EdgeTypeChooser { // testuje, czy typ krawedzi spelnia podana maske
    Koala::EdgeDirection mask;
    typedef EdgeTypeChooser ChoosersSelfType;
    EdgeTypeChooser(Koala::EdgeDirection amask=Koala::EdAll) : mask(amask)
    {  mask|=( mask& Directed) ? Directed : 0;    }
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return bool(g.getType(e) & mask); }
};

EdgeTypeChooser edgeTypeChoose(Koala::EdgeDirection mask) { return EdgeTypeChooser(mask); }

// choosery zlozone dla krawedzi, sprawdzajace warunkek definiowany dla wierzcholka sa prawda dla jej koncow

template <class Ch> struct EdgeFirstEndChooser { // test pierwszego konca krawedzi
    Ch ch;
    typedef EdgeFirstEndChooser<Ch> ChoosersSelfType;
    EdgeFirstEndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnd1(e),g); }
};

template <class Ch>
EdgeFirstEndChooser<Ch> edgeFirstEndChoose(Ch ch) { return EdgeFirstEndChooser<Ch>(ch); }


template <class Ch> struct EdgeSecondEndChooser { // test drugiego konca krawedzi
    Ch ch;
    typedef EdgeSecondEndChooser<Ch> ChoosersSelfType;
    EdgeSecondEndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnd2(e),g); }
};

template <class Ch>
EdgeSecondEndChooser<Ch> edgeSecondEndChoose(Ch ch) { return EdgeSecondEndChooser<Ch>(ch); }


template <class Ch> struct Edge0EndChooser { // test, czy zaden koniec nie spelnia warunku
    Ch ch;
    typedef Edge0EndChooser<Ch> ChoosersSelfType;
    Edge0EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return !ch(g.getEdgeEnd1(e),g) && !ch(g.getEdgeEnd2(e),g); }
};

template <class Ch>
Edge0EndChooser<Ch> edge0EndChoose(Ch ch) { return Edge0EndChooser<Ch>(ch); }


template <class Ch> struct Edge1EndChooser { // test, czy jeden koniec spelnia warunkek
    Ch ch;
    typedef Edge1EndChooser<Ch> ChoosersSelfType;
    Edge1EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnd1(e),g) != ch(g.getEdgeEnd2(e),g); }
};

template <class Ch>
Edge1EndChooser<Ch> edge1EndChoose(Ch ch) { return Edge1EndChooser<Ch>(ch); }


template <class Ch> struct Edge2EndChooser { // test, czy oba konice spelniaja warunkek
    Ch ch;
    typedef Edge2EndChooser<Ch> ChoosersSelfType;
    Edge2EndChooser(Ch funktor=Ch()) : ch(funktor) {}
    template <class Graph>
    bool operator()(typename Graph::PEdge e, const Graph& g) const
    { return ch(g.getEdgeEnd1(e),g) && ch(g.getEdgeEnd2(e),g); }
};

template <class Ch>
Edge2EndChooser<Ch> edge2EndChoose(Ch ch) { return Edge2EndChooser<Ch>(ch); }


//Castery to funktory ustalajace wartosci pol info w nowych wierz/kraw tworzonych podczas np. kopiowania grafow.
//Wartosci te powstaja (w rozny sposob) na podstawie inf oryginalnych

// caster zwyklego rzutowania miedzy dwoma strukturami
struct StdCaster {
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest,InfoSour sour) { dest=(InfoDest)sour; }
};

// i jego funkcja tworzaca
StdCaster stdCast() {return StdCaster(); }

// caster ustawiajacy wartosc domyslna i ignorujacy oryginalny parametr
// wspolpracuje z produktami grafow (stad takze operator 3-argumentowy)
struct NoCastCaster {
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest,InfoSour sour) { dest=InfoDest(); }

    template <class InfoDest, class InfoSour1,class InfoSour2>
	void operator()(InfoDest& dest,InfoSour1 sour1,InfoSour2 sour2) { dest=InfoDest(); }
};

// funkcja tworzaca - dopuszczalny jedynie argument false
NoCastCaster stdCast(bool arg) { assert(!arg); return NoCastCaster(); }

// wyliczenie wartosci nowego info poprzez podany funktor
// wspolpracuje z produktami grafow (stad takze operator 3-argumentowy) jesli funktor je obsluguje
// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
template <class Fun> struct ObjCaster {
    mutable Fun funktor;
    ObjCaster(Fun afun=Fun()) : funktor(afun) {}

    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest, InfoSour sour) { dest=(InfoDest)funktor(sour); }

    template <class InfoDest, class InfoSour1,class InfoSour2>
	void operator()(InfoDest& dest,InfoSour1 sour1,InfoSour2 sour2)	{ dest=(InfoDest)funktor(sour1,sour2); }

};

template <class Funktor>
ObjCaster<Funktor> stdCast(Funktor f) { return ObjCaster<Funktor>(f); }

// Caster wpisujacy ustalona wartosc
// wspolpracuje z produktami grafow (stad takze operator 3-argumentowy)
template <class T> struct ValueCaster {
    T val;
    ValueCaster(T arg=T()) : val(arg) {}
    template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest, InfoSour sour) { dest=(InfoDest)val; }

    template <class InfoDest, class InfoSour1,class InfoSour2>
	void operator()(InfoDest& dest,InfoSour1 sour1,InfoSour2 sour2) { dest=(InfoDest)val; }
};

// funkcja tworzaca - podajemy stala wartosc
template <class T>
ValueCaster<T> valCast(T arg=T()) { return ValueCaster<T>(arg); }



//Linkery dzialajace np. podczas kopiowania grafow, wiaza nowo tworzone wierzch/kraw. z ich oryginalami,
//przez co mozna latwo sprawdzic, ktory element odpowiada ktoremu

// polowki pelnego linkera, tworza powiazanie  w jedna strone

// tylko false jest dopuszczalne - brak polaczenia
struct Std1NoLinker {
    Std1NoLinker(bool arg=false) { assert(!arg);}
    template <class Dest,class Sour>
        void operator()(Dest*,Sour*) {}
};

// ustawia wskaznik na dolaczany obiekt w srodku struktury info obiektu docelowego (o ile nie byl on NULLem)
template <class Info, class T> struct Std1FieldLinker {
    T Info::* pt;
	Std1FieldLinker(T Info::* awsk=0) : pt(awsk) {}
	template <class Dest, class Sour>
        void operator()(Dest* wsk, Sour* w) {  if (pt && wsk) wsk->info.*pt=(T) w; }
};

// dopisuje powiazania do zewnetrznej! tablicy asocjacyjnej podanej w funkcji tworzacej stdLink (o ile argument nie byl NULLem)
template <class Map> struct Std1AssocLinker{
    Map& map;
    Std1AssocLinker(Map& amap) : map(amap) {}
	template <class Dest, class Sour>
        void operator()(Dest* wsk, Sour* w) {   if (wsk) map[wsk]=w; }
};

// pelny linker, zawiera obiekty laczace (polowki linkera - j.w.) nowy element ze starym i odwrotnie
template <class Link1, class Link2> struct Std2Linker {
        mutable Link1 dest2sour;
        mutable Link2 sour2dest;
        Link1& first() {    return dest2sour; };
        Link2& second() {    return sour2dest; };
        Std2Linker(Link1 al1, Link2 al2) : dest2sour(al1), sour2dest(al2) {}
        template <class Dest, class Sour>
            void operator()(Dest* wsk, Sour* w) { dest2sour(wsk,w); sour2dest(w,wsk); }
};

// funkcje tworzace polowki linkera - raczej do uzytku wewnetrznego

Std1NoLinker stdLink(bool a1) { return Std1NoLinker(a1); }

template <class Info1, class T1>
Std1FieldLinker<Info1,T1> stdLink(T1 Info1:: *awsk1)
{ return Std1FieldLinker<Info1,T1>(awsk1); }

template <class Map1>
Std1AssocLinker<Map1> stdLink(Map1& tab1)
{ return Std1AssocLinker<Map1>(tab1); }

// funkcje tworzace pelny linker, argumenty podaja wymagany sposob polaczenia elementow nowych z oryginalami (pierwszy argument) i odwrotnie (drugi)
// argument bool moze byc tylko false (brak polaczenia)

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
