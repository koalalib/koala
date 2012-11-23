#ifndef KOALA_DEFS_H
#define KOALA_DEFS_H

/* defs.h
 *
 */

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <stack>
#include <utility>

#include "exception.h"
#include "../container/assoctab.h"
#include "../container/hashcont.h"
#include "../container/heap.h"
#include "../container/localarray.h"
#include "../container/simple.h"
#include "../container/set.h"

namespace Koala
{
    // Mozliwe typy krawędzi grafu ...
    typedef unsigned char EdgeType;
    // ... i ich orientacje względem wierzchołka.
    typedef unsigned char EdgeDirection;

    // Dopuszczalne wartości orientacji: brak, pętla, nieskierowana, wchodząca do wierzchołka, wychodząca z
    // wierzchołka, wszystkie możliwe orientacje.
    static const EdgeDirection EdNone   = 0x00;
    static const EdgeDirection EdLoop   = 0x01;
    static const EdgeDirection EdUndir  = 0x02;
    static const EdgeDirection EdDirIn  = 0x04;
    static const EdgeDirection EdDirOut = 0x08;
    static const EdgeDirection EdAll    = 0x0F;

    static const EdgeType Detached   = 0x0;
    static const EdgeType Loop       = 0x1;
    static const EdgeType Undirected = 0x2;
    static const EdgeType Directed   = 0xC;

    // Domyślne struktury (puste) z opisem krawędzi/wierzchołka.
    struct EmptyVertInfo { } ;
    struct EmptyEdgeInfo { } ;

    template< class VertInfo, class EdgeInfo, class Settings > class Graph;
    template< class VertInfo, class EdgeInfo, class Settings > class Vertex;
    template< class VertInfo, class EdgeInfo, class Settings > class Edge;
    template< EdgeType mask, bool matr > class GrDefaultSettings;

    /* AlgsDefaultSettings
     * Wytyczne parametryzujace dzialanie algorytmow biblioteki. Z reguly klasy z procedurami maja postac
     * NazwaPar<DefaultStructs> oraz pochodna klase Nazwa dzialajaca z ustawieniem DefaultStructs=AlgsDefaultSettings
     */
    class AlgsDefaultSettings
    {
      public:
        // Typ klasy tablicy asocjacyjnej przypisującej np. wierzchołkowi/krawędzi wartości typu B.
        template< class A, class B > class AssocCont
        {
          public:
            typedef AssocArray< A,B > Type;
            // TODO: nie usuwac komentarzy (przykladowa uzycia)
            //   typedef AssocTable < BiDiHashMap<A,B> > Type;
            //   typedef PseudoAssocArray<A,B,AssocTable<std::map<A,int> > > Type;
        };

        // Typ 2-wymiarowej tablicy assocjacyjnej o kluczu A i wartości B. Kluczami są pary uporzadkowane lub
        // nieuporzadkowane (por. assoctab.h).
        template< class A, class B, AssocMatrixType type > class TwoDimAssocCont
        {
          public:
            typedef AssocMatrix< A,B,type > Type;
            // TODO: nie usuwac komentarza (przykladowe uzycia)
            // typedef AssocMatrix<A,B,type,std::vector< BlockOfAssocMatrix<B> >,PseudoAssocArray<A,int,AssocTable<BiDiHashMap<A,int> > > > Type;
        };

        // typ struktury kopca i jego wezla
        template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
            class HeapCont
        {
          public:
            typedef FibonHeap< Key,Compare,Allocator > Type;
            typedef FibonHeapNode< Key > NodeType;
        };

        // Czy grafy pomocnicze maja uzywac macierzy sasiedztwa?
        enum { LocalGraphAdjMatr = false };

        // Typ grafu pomocniczego stosowanego wewnątrz procedury.
        template< class A, class B, EdgeType mask> class LocalGraph
        {
          public:
            typedef Graph< A,B,GrDefaultSettings< mask,bool(LocalGraphAdjMatr) > > Type;
        };

        // Czy dostosowywać rozmiar pamięci wyjściowych tablic asocjacyjnych?
        enum { ReserveOutAssocCont = true };

        // Specjalizacje dla wlasnych klas numerycznych (np. liczb wymiernych) pozwola uzywac ich jako danych w
        // algorytmach (np. dlugosci krawedzi). Dlatego w kodach nawet zerowosc jakiejs etykiety sprawdzam metoda.
        template< class T > class NumberTypeBounds
        {
          public:
            static T plusInfty() { return std::numeric_limits< T >::max(); }
            static bool isPlusInfty( T arg ) { return arg == plusInfty(); }
            static T minusInfty() { return std::numeric_limits< T >::min(); }
            static bool isMinusInfty( T arg ) { return arg == minusInfty(); }
            static T zero() { return (T)0; }
            static T one() { return (T)1; }
            static bool isZero( T arg ) { return arg == zero(); }
        };

        // Wybrany do użytku wewnętrznego algorytm sortowania tablic.
        template< class Iterator > static void sort( Iterator first, Iterator last );
        // ... i to samo z funkcją porównującą.
        template< class Iterator, class Comp > static void sort( Iterator first, Iterator last, Comp comp );
    };


    /* ConstFunctor
     *
     * Funktor domyślny przydatny tam, gdzie jakaś metoda chce dostać np. funktor generujacy info dla wierzchołka lub
     * krawędzi. Zwraca wartość podaną w argumencie konstruktora. Dziala dla od 0 do 6 argumentow.
     */
    template< class T > class ConstFunctor
    {
        const T val;

      public:
        ConstFunctor( const T &aval = T() ): val( aval ) { }

        T operator()() { return val; }

        template< class A > T operator()(const A&) { return val; }
        template< class A, class B > T operator()(const A&, const B& ) { return val; }
        template< class A, class B, class C > T operator()( const A&,const B&,const C& ) { return val; }
        template< class A, class B, class C, class D > T operator()( const A&,const B&,const C&,const D& )
                { return val; }
        template< class A, class B, class C,class D, class E > T operator()( const A&,const B&,const C&,const D&,const E& )
                { return val; }
        template< class A, class B, class C,class D, class E, class F > T operator()( const A&,const B&,const C&,const D&,const E&,const F& )
                { return val; }
    };

    // Funkcja tworząca powyższy funktor.
    template< class T > ConstFunctor< T > constFun( const T &a = T() ) { return ConstFunctor< T >( a ); }

    /* BlackHole
     * Jesli metoda chce dostac argument wyjsciowy (np. iterator do zapisu ciagu, tablice asocjacyjna) a nas te
     * wartosci nie interesuja, tylko inne efekty uboczne procedury. Uniwersalna zaślepka w wielu procedurach
     * biblioteki, pozwala uniknąć zbędnych przeciążeń nazw z różnymi zestawami parametrów.
     */
    struct BlackHole: public std::iterator< std::output_iterator_tag,void,void,void,void >
    {
        template< class T > BlackHole &operator=( T ) { return *this; }

        BlackHole &operator*() { return *this; }
        BlackHole &operator++() { return *this; }
        BlackHole operator++( int ) { return *this; }

        BlackHole() {}

        // BlackHole również może służyć jako zaślepka dla nie interesujacego nas kontenera asocjacyjnego wymaganego
        // w procedurze; te metody nigdy nie powinny być wykonane, są potrzebne jedynie by kod się kompilował.
        template< class T > BlackHole &operator[]( T );
        template< class T, class R > BlackHole &operator()( T,R );

        // BlackHole potrafi przekonwertować się na dowolny typ - uwaga j.w.
        template< class T > operator T();

        void reserve( int ) { }
    };


    // Makra blackHole można używać jak zmiennej globalnej typu BlackHole.
    #define blackHole ((*((Koala::BlackHole*)( &std::cout ))))

    // Test na to, czy argument jest typu BlackHole.
    template< class T > bool isBlackHole( const T & ) { return false; }
    bool isBlackHole( const BlackHole & ) { return true; }

    /* BlackHoleSwitch
     * Jeśli szablon procedury dostal BlackHole zamiast argumentu (kontenera), a procedura potrzebuje do działania
     * tego kontenera - tworzy go sobie sama lokalnie. Ta klasa pozwala sprawdzić, czy taki przypadek zaszedł i
     * przełączyć kontener na odpowiednio: dostarczony lub lokalny (nie będący BlackHolem).
     */
    template< class Cont1, class Cont2 > struct BlackHoleSwitch
    {
        // Typ kontenera, na którym będziemy działać.
        typedef Cont1 Type;

        static Cont1 &get( Cont1 &a, Cont2 &b ) { return a; }
        static const Cont1 &get(const Cont1 &a, const Cont2 &b ) {  return a; }
    };

    template< class Cont2 > struct BlackHoleSwitch< BlackHole,Cont2 >
    {
        typedef Cont2 Type;

        static Cont2 &get( BlackHole &a, Cont2 &b ) { return b; }
        static const Cont2 &get(const BlackHole &a,const Cont2 &b ) {  return b; }
    };

    // Choosery są strukturami funkcyjnymi zwracającymi true/false (poprzez operator()) dla wierzchołków/krawędzi
    // grafu. Służą np. w procedurach wybierania podgrafow, kopiowania podgrafow... W kodzie powinno się je tworzyć
    // ich "funkcjami tworzącymi".

    // Choosery uniwersalne, tj. można je stosować dla wierzchołków i krawędzi.

    /* BoolChooser
     * zwraca ustalona wartosc true lub false
     */
    struct BoolChooser
    {
        bool val;

        // Każdy chooser ma swój wlasny typ zdefiniowany jako ChoosersSelfType.
        typedef BoolChooser ChoosersSelfType;

        BoolChooser( bool arg = false ): val( arg ) { }

        // Główny operator choosera, testujący prawdziwość jego predykatu.
        template< class Elem, class Graph > bool operator()( Elem *, const Graph & ) const { return val; }
    };


    // Funkcja tworząca chooser typu BoolChooser.
    // TODO: sprawdzic, czy rozne przeciazenia funkcji tworzacych nie wywoluja niejednoznacznosci w rozstrzyganiu przeciazen
    BoolChooser stdChoose( bool arg ) { return BoolChooser( arg ); }

    /* ValChooser
     * sprawdza, czy testowany element to podana (ustalona) wartosc
     */
    template< class Elem > struct ValChooser
    {
        Elem val;

        typedef ValChooser< Elem > ChoosersSelfType;

        ValChooser( Elem arg ): val( arg ) { }

        template< class Graph > bool operator()( Elem elem, const Graph & ) const { return elem == val; }
    };

    template< class Elem > ValChooser< Elem > stdValChoose( Elem arg ) { return ValChooser< Elem >( arg ); }

    /* SetChooser
     * sprawdza, czy testowany element jest w podanym zbiorze (Koala::Set)
     */
    template< class Elem > struct SetChooser
    {
        Koala::Set< Elem * > set;

        typedef SetChooser< Elem > ChoosersSelfType;

        SetChooser( const Koala::Set< Elem * > &arg = Koala::Set< Elem * >() ): set( arg ) { }

        template< class Graph > bool operator()( Elem *elem, const Graph & ) const { return set.isElement( elem ); }
    };

    template< class Elem >
        SetChooser< Elem > stdChoose( Koala::Set< Elem * > arg ) { return SetChooser< Elem >( arg ); }

    /* ContainerChooser
     * sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
     */
    template< class Iter > struct ContainerChooser
    {
        Iter begin, end;

        typedef ContainerChooser< Iter > ChoosersSelfType;

        ContainerChooser( Iter abegin = Iter(), Iter aend = Iter() ): begin( abegin ), end( aend ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph & ) const { return std::find( begin,end,elem ) != end; }
    };

    template< class Iter >
        ContainerChooser< Iter > stdChoose( Iter begin, Iter end ) { return ContainerChooser< Iter >( begin,end ); }

    /* ObjChooser
     * sprawdza wartosc podanego obiektu funkcyjnego dla testowanego elementu
     */
    template< class Obj > struct ObjChooser
    {
        // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
        mutable Obj funktor;

        typedef ObjChooser< Obj > ChoosersSelfType;

        ObjChooser( Obj arg = Obj() ): funktor( arg ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &graph ) const { return (bool)funktor( elem,graph ); }
    };

    // liera F w nazwie - dla chooserow dzialajaych z funktorami
    template< class Obj > ObjChooser< Obj > stdFChoose( Obj arg ) { return ObjChooser< Obj >( arg ); }

    //te choosery juz zagladaja do konkretnych pol rekordow info wierz/krawedzi

    /* FieldValChooser
     * test, czy pole w info ma podana wartosc
     */
    template< class Info, class T > struct FieldValChooser
    {
        T Info:: *wsk;
        T val;

        typedef FieldValChooser< Info,T > ChoosersSelfType;

        FieldValChooser( T Info:: *arg = 0, T arg2 = T() ): wsk( arg ), val( arg2 ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &graph ) const { return elem->info.*wsk == val; }
    };

    template< class Info, class T >
        FieldValChooser< Info,T > fieldChoose( T Info:: *wsk, T arg ) { return FieldValChooser< Info,T >( wsk,arg ); }

    /* FieldValChooserL
     * test, czy pole w info jest mniejsze od podanej wartosci
     */
    template< class Info, class T > struct FieldValChooserL
    {
        T Info:: *wsk;
        T val;

        typedef FieldValChooserL< Info,T > ChoosersSelfType;

        FieldValChooserL( T Info:: *arg = 0, T arg2 = T() ): wsk( arg ), val( arg2 ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &graph ) const { return elem->info.*wsk < val; }
    };

    template< class Info, class T > FieldValChooserL< Info,T >
        fieldChooseL( T Info:: *wsk, T arg ) { return FieldValChooserL< Info,T >( wsk,arg ); }

    /* FieldValChooserG
     * test, czy pole w info jest wieksze od podanej wartosci
     */
    template< class Info, class T > struct FieldValChooserG
    {
        T Info:: *wsk;
        T val;

        typedef FieldValChooserG< Info,T > ChoosersSelfType;

        FieldValChooserG( T Info:: *arg = 0, T arg2 = T() ): wsk( arg ), val( arg2 ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &graph ) const { return elem->info.*wsk > val; }
    };

    template< class Info, class T > FieldValChooserG< Info,T >
        fieldChooseG( T Info:: *wsk, T arg ) { return FieldValChooserG< Info,T >( wsk,arg ); }

    /* FielBoolChooser
     * test, czy pole w info ma wartosc prawda (jego typ musi byc konwertowalny do bool)
     */
    template< class Info, class T > struct FielBoolChooser
    {
        T Info:: *wsk;

        typedef FielBoolChooser< Info,T > ChoosersSelfType;

        FielBoolChooser( T Info::*arg = 0): wsk( arg ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &graph ) const { return bool( elem->info.*wsk ); }
    };

    template< class Info, class T >
        FielBoolChooser< Info,T > fieldChoose( T Info:: *wsk ) { return FielBoolChooser< Info,T >(wsk); }

    /* FielObjChooser
     * wlasny obiekt lub funkcja, ktora ma sie wykonywac dla konkretnego pola z info
     */
    // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
    template< class Info, class T, class Obj > struct FieldObjChooser
    {
        T Info:: *wsk;
        mutable Obj funktor;

        typedef FieldObjChooser< Info,T,Obj > ChoosersSelfType;

        FieldObjChooser( T Info:: *awsk = 0, Obj afun = Obj() ): wsk( awsk ), funktor( afun ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &graph ) const { return (bool)funktor( elem->info.*wsk ); }
    };

    template< class Info, class T, class Obj > FieldObjChooser< Info,T,Obj >
        fieldFChoose( T Info::*wsk, Obj obj ) { return FieldObjChooser< Info,T,Obj >( wsk,obj ); }

    /* FielSetChooser
     * test, czy podane pole w info ma wartosc z danego zbioru (Koala::Set)
     */
    template< class Info, class T, class Z > struct FieldSetChooser
    {
        T Info:: *wsk;
        Koala::Set< Z > set;

        typedef FieldSetChooser< Info,T,Z > ChoosersSelfType;

        FieldSetChooser( T Info:: *awsk = 0, const Koala::Set< Z > &aset = Koala::Set< Z >() ):
            wsk( awsk ), set( aset ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &graph ) const { return set.isElement( elem->info.*wsk ); }
    };

    template< class Info, class T, class Z > FieldSetChooser< Info,T,Z >
        fieldChoose( T Info::*wsk, Koala::Set< Z > set ) { return FieldSetChooser< Info,T,Z >( wsk,set ); }

    /* FielContainerChooser
     * sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
     */
    template< class Info, class T, class Iter > struct FieldContainerChooser
    {
        T Info:: *wsk;
        Iter begin, end;

        typedef FieldContainerChooser< Info,T,Iter > ChoosersSelfType;

        FieldContainerChooser( T Info:: *awsk = 0, Iter a = Iter(), Iter b = Iter() ):
            wsk( awsk ), begin( a ), end( b ) {}

        template< class Elem, class Graph > bool
            operator()( Elem *elem, const Graph &graph ) const { return std::find( begin,end,elem->info.*wsk ) != end; }
    };

    template< class Info, class T, class Iter > FieldContainerChooser< Info,T,Iter >
        fieldChoose( T Info::*wsk, Iter b, Iter e ) { return FieldContainerChooser< Info,T,Iter >( wsk,b,e ); }

    // choosery decydujace na podstawie wartosci przypisanej elementowi w podanej tablicy asocjacyjnej

    /* AssocHasChooser
     * test, czy element jest kluczem znajdujacym sie w tablicy
     */
    template< class Cont > struct AssocHasChooser
    {
        Cont cont;

        typedef AssocHasChooser< Cont > ChoosersSelfType;

        AssocHasChooser( const Cont &arg = Cont() ): cont( arg ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ); }
    };

    template< class Cont >
        AssocHasChooser< Cont > assocKeyChoose( Cont arg ) { return AssocHasChooser< Cont >( arg ); }

    /* AssocBoolChooser
     * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc odpowiada true (typ
     * wartosci musi byc konwertowalny do bool)
     */
    template< class Cont > struct AssocBoolChooser
    {
        Cont cont;

        typedef AssocBoolChooser< Cont > ChoosersSelfType;

        AssocBoolChooser( const Cont &arg = Cont() ): cont( arg ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && (bool)cont[elem]; }
    };

    template< class Cont >
        AssocBoolChooser< Cont > assocChoose( Cont arg ) { return AssocBoolChooser< Cont >( arg ); }

    /* AssocValChooser
     * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest rowna zadanej
     */
    template< class Cont > struct AssocValChooser
    {
        Cont cont;

        typename Cont::ValType val;
        typedef typename Cont::ValType SelfValType;
        typedef AssocValChooser< Cont > ChoosersSelfType;

        AssocValChooser( const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType() ):
            cont( arg ),val( aval ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] == val; }
    };

    template< class Cont > AssocValChooser< Cont >
        assocChoose( Cont arg, typename Cont::ValType val ) { return AssocValChooser< Cont >( arg,val ); }

    /* AssocValChooserL
     * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest mniejsza od zadanej
     */
    template< class Cont > struct AssocValChooserL
    {
        Cont cont;

        typename Cont::ValType val;
        typedef typename Cont::ValType SelfValType;
        typedef AssocValChooserL< Cont > ChoosersSelfType;

        AssocValChooserL( const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType() ):
            cont( arg ),val( aval ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] < val; }
    };

    template< class Cont > AssocValChooserL< Cont >
        assocChooseL( Cont arg, typename Cont::ValType val ) { return AssocValChooserL< Cont >( arg,val ); }

    /* AssocValChooserG
     * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest wieksza od zadanej
     */
    template< class Cont > struct AssocValChooserG
    {
        Cont cont;

        typename Cont::ValType val;
        typedef typename Cont::ValType SelfValType;
        typedef AssocValChooserG< Cont > ChoosersSelfType;

        AssocValChooserG( const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType() ):
            cont( arg ),val( aval ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] > val; }
    };

    template< class Cont > AssocValChooserG< Cont >
        assocChooseG( Cont arg, typename Cont::ValType val ) { return AssocValChooserG< Cont >( arg,val ); }

    /* AssocSetChooser
     * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest elementem podanego
     * zbioru (Koala::Set)
     */
    template< class Cont > struct AssocSetChooser
    {
        Cont cont;

        Koala::Set< typename Cont::ValType > set;
        typedef AssocSetChooser< Cont > ChoosersSelfType;

        AssocSetChooser( const Cont &arg = Cont(), const Koala::Set< typename Cont::ValType > &aset =
            Koala::Set< typename Cont::ValType >() ): cont( arg ),set( aset ) { }

        template< class Elem, class Graph > bool
            operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && set.isElement( cont[elem] ); }
    };

    template< class Cont > AssocSetChooser< Cont > assocChoose( Cont arg,
        const Koala::Set< typename Cont::ValType > &set ) { return AssocSetChooser< Cont >( arg,set ); }

    /* AssocContainerChooser
     * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc lezy w przedziale miedzy
     * podanymi iteratorami - uzywa STLowego find
     */
    template< class Cont, class Iter > struct AssocContainerChooser
    {
        Iter begin, end;
        Cont cont;

        typedef AssocContainerChooser< Cont,Iter > ChoosersSelfType;
        AssocContainerChooser( const Cont &acont = Cont(), Iter abegin = Iter(), Iter aend = Iter() ):
            cont( acont ), begin( abegin ), end( aend ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
            { return cont.hasKey( elem ) && std::find( begin,end,cont[elem] ) != end; }
    };

    template< class Cont, class Iter > AssocContainerChooser< Cont,Iter >
        assocChoose( Cont cont, Iter begin, Iter end ) { return AssocContainerChooser< Cont,Iter >( cont,begin,end ); }

    /* AssocObjChooser
     * sprawdza wartosc podanemgo obiektu funkcyjnego przypisana w tablicy asocjacyjnej testowanemu elementowi
     */
    template< class Cont, class Obj > struct AssocObjChooser
    {
        // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
        mutable Obj funktor;
        Cont cont;

        typedef AssocObjChooser< Cont,Obj > ChoosersSelfType;
        AssocObjChooser( const Cont &acont = Cont(), Obj arg = Obj() ): cont( acont ), funktor( arg ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
            { return cont.hasKey( elem ) && (bool)funktor( cont[elem] ); }
    };

    template< class Cont, class Obj > AssocObjChooser< Cont,Obj >
        assocFChoose( Cont cont, Obj arg ) { return AssocObjChooser< Cont,Obj >( cont,arg ); }

    // Wszystkie choosery operujace na tablicach asocjacyjnych maja wersje dzialajace z zewnetrzna tablica
    // podawana przez wskaznik. Unikamy kopiowania tablic, uzywajac trzeba uwazac, by tablica wciaz zyla przez
    // caly czas zycia choosera. Funkcje tworzace maja przedrostek ext i pobieraja adres tablicy

    /* AssocHasChooser
     *
     */
    template< class Cont > struct AssocHasChooser< Cont * >
    {
        const Cont *cont;

        typedef AssocHasChooser< Cont * > ChoosersSelfType;

        AssocHasChooser( const Cont *arg = 0): cont( arg ) { }

        template< class Elem, class Graph >
            bool operator()( Elem *elem, const Graph &) const { return cont->hasKey( elem ); }
    };

    template< class Cont >
        AssocHasChooser< Cont * > extAssocKeyChoose( const Cont *arg ) { return AssocHasChooser< Cont * >( arg ); }

    /* AssocBoolChooser
     *
     */
    template< class Cont > struct AssocBoolChooser< Cont * >
    {
        const Cont *cont;

        typedef AssocBoolChooser< Cont * > ChoosersSelfType;

        AssocBoolChooser( const Cont *arg = 0): cont( arg ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
            { return cont->hasKey( elem ) && (bool)cont->operator[]( elem ); }
    };

    template< class Cont >
        AssocBoolChooser< Cont * > extAssocChoose( const Cont *arg ) { return AssocBoolChooser< Cont * >( arg ); }

    /* AssocValChooser
     *
     */
    template< class Cont > struct AssocValChooser< Cont * >
    {
        const Cont *cont;

        typename Cont::ValType val;
        typedef typename Cont::ValType SelfValType;
        typedef AssocValChooser< Cont * > ChoosersSelfType;

        AssocValChooser( const Cont *arg = 0, typename Cont::ValType aval = SelfValType() ):
            cont( arg ), val( aval ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
            { return cont->hasKey( elem ) && cont->operator[]( elem ) == val; }
    };

    template< class Cont > AssocValChooser< Cont * >
        extAssocChoose( const Cont *arg, typename Cont::ValType val ) { return AssocValChooser< Cont * >( arg,val ); }

    /* AssocValChooserG
     *
     */
    template< class Cont > struct AssocValChooserG< Cont * >
    {
        const Cont *cont;

        typename Cont::ValType val;
        typedef typename Cont::ValType SelfValType;
        typedef AssocValChooserG< Cont * > ChoosersSelfType;

        AssocValChooserG( const Cont *arg = 0, typename Cont::ValType aval = SelfValType() ):
            cont( arg ), val( aval ) { }

        template< class Elem,class Graph > bool operator()( Elem *elem, const Graph & ) const
            { return cont->hasKey( elem ) && cont->operator[]( elem ) > val; }
    };

    template< class Cont > AssocValChooserG< Cont * >
        extAssocChooseG( const Cont *arg, typename Cont::ValType val ) { return AssocValChooserG< Cont * >( arg,val ); }

    /* AssocValChooserL
     *
     */
    template< class Cont > struct AssocValChooserL< Cont * >
    {
        const Cont *cont;

        typename Cont::ValType val;
        typedef typename Cont::ValType SelfValType;
        typedef AssocValChooserL< Cont * > ChoosersSelfType;

        AssocValChooserL( const Cont *arg = 0, typename Cont::ValType aval = SelfValType() ):
            cont( arg ), val( aval ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
            { return cont->hasKey( elem ) && cont->operator[]( elem ) < val; }
    };

    template< class Cont > AssocValChooserL< Cont * >
        extAssocChooseL( const Cont *arg, typename Cont::ValType val ) { return AssocValChooserL< Cont * >( arg,val ); }

    /* AssocSetChooser
     *
     */
    template< class Cont > struct AssocSetChooser< Cont * >
    {
        const Cont *cont;

        Koala::Set< typename Cont::ValType > set;
        typedef AssocSetChooser< Cont * > ChoosersSelfType;

        AssocSetChooser(const Cont *arg = 0, const Koala::Set< typename Cont::ValType > &aset =
            Koala::Set< typename Cont::ValType>() ): cont( arg ), set( aset ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
            { return cont->hasKey( elem ) && set.isElement( cont->operator[]( elem ) ); }
    };

    template< class Cont > AssocSetChooser< Cont * > extAssocChoose( const Cont *arg,
        const Koala::Set< typename Cont::ValType > &set ) { return AssocSetChooser< Cont * >( arg,set ); }

    /* AssocContainerChooser
     *
     */
    template< class Cont, class Iter > struct AssocContainerChooser< Cont *,Iter >
    {
        Iter begin, end;
        const Cont *cont;

        typedef AssocContainerChooser< Cont *,Iter > ChoosersSelfType;

        AssocContainerChooser( const Cont *acont = 0, Iter abegin = Iter(), Iter aend=Iter() ):
            cont( acont ), begin( abegin ), end( aend ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const;
    };

    template< class Cont, class Iter > AssocContainerChooser< Cont *,Iter >
        extAssocChoose(const Cont *cont, Iter begin, Iter end )
    { return AssocContainerChooser< Cont *,Iter >( cont,begin,end ); }

    /* AssocObjChooser
     *
     */
    template< class Cont, class Obj > struct AssocObjChooser< Cont *,Obj >
    {
        // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
        mutable Obj funktor;
        const Cont *cont;

        typedef AssocObjChooser< Cont *,Obj > ChoosersSelfType;

        AssocObjChooser( const Cont *acont = 0, Obj arg = Obj() ): cont( acont ), funktor( arg ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
            { return cont->hasKey( elem ) && (bool)funktor( cont->operator[]( elem ) ); }
    };

    template< class Cont, class Obj > AssocObjChooser< Cont *,Obj >
        extAssocFChoose( const Cont *cont, Obj arg ) { return AssocObjChooser< Cont *,Obj >( cont,arg ); }

    // Predykaty chooserow mozna laczyc operatorami logicznymi. Choosery operacji logicznych na prostszych chooserach

    /* OrChooser
     *
     */
    template< class Ch1, class Ch2 > struct OrChooser
    {
        Ch1 ch1;
        Ch2 ch2;

        typedef OrChooser< Ch1,Ch2 > ChoosersSelfType;

        OrChooser( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
            { return (ch1( elem,graph ) || ch2( elem,graph )); }
    };

    template< class  Ch1, class Ch2 > OrChooser< Ch1,Ch2 >
        orChoose( Ch1 a, Ch2 b ) { return OrChooser< Ch1,Ch2 >( a,b ); }

    // w kodzie funkcje tworzace zlozonych chooserow mozna zastapic odpowiednimi operatorami logicznymi

    template <class  Ch1, class Ch2> OrChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
        operator||( Ch1 a, Ch2 b ) { return OrChooser< Ch1,Ch2 >( a,b ); }

    /* AndChooser
     *
     */
    template< class Ch1, class Ch2 > struct AndChooser
    {
        Ch1 ch1;
        Ch2 ch2;

        typedef AndChooser< Ch1,Ch2 > ChoosersSelfType;

        AndChooser( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
            { return (ch1( elem,graph ) && ch2( elem,graph )); }
    };

    template< class  Ch1, class Ch2 > AndChooser< Ch1,Ch2 >
        andChoose( Ch1 a, Ch2 b ) { return AndChooser< Ch1,Ch2 >( a,b ); }

    template< class Ch1, class Ch2 > AndChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
        operator&&( Ch1 a, Ch2 b ) { return AndChooser< Ch1,Ch2 >( a,b ); }

    /* XorChooser
     *
     */
    template< class Ch1, class Ch2 > struct XorChooser
    {
        Ch1 ch1;
        Ch2 ch2;

        typedef XorChooser< Ch1,Ch2 > ChoosersSelfType;

        XorChooser( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

        template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
            { return (ch1( elem,graph ) != ch2( elem,graph )); }
    };

    template< class Ch1, class Ch2 > XorChooser< Ch1,Ch2 >
        xorChoose( Ch1 a, Ch2 b ) { return XorChooser< Ch1,Ch2 >( a,b ); }

    template< class Ch1, class Ch2 > XorChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
        operator^( Ch1 a, Ch2 b ) { return XorChooser< Ch1,Ch2 >( a,b ); }

    /* NotChooser
     *
     */
    template< class Ch1 > struct NotChooser
    {
        Ch1 ch1;

        typedef NotChooser< Ch1 > ChoosersSelfType;

        NotChooser( Ch1 a = Ch1() ): ch1( a ) { }

        template< class Elem, class Graph >  bool operator()( Elem *elem, const Graph &graph) const
            { return !ch1( elem,graph ); }
    };

    template< class  Ch1 >
        NotChooser< Ch1 > notChoose( Ch1 a ) { return NotChooser< Ch1 >( a ); }

    template< class  Ch1 > NotChooser< typename Ch1::ChoosersSelfType >
        operator!( Ch1 a ) { return NotChooser< Ch1 >( a ); }

    // choosery dow wybierania wierzcholkow

    /* VertDegValChooser
     * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma podana
     * wartosc
     */
    struct VertDegValChooser
    {
        int deg;
        Koala::EdgeDirection type;

        typedef VertDegValChooser ChoosersSelfType;

        VertDegValChooser( int adeg = 0, Koala::EdgeDirection atype = Koala::EdAll ): deg( adeg ), type( atype ) { }

        template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
            { return g.deg( v,type ) == deg; }
    };

    VertDegValChooser vertDegChoose( int adeg, Koala::EdgeDirection atype = Koala::EdAll )
        { return VertDegValChooser( adeg,atype ); }

    /* VertDegValChooserL
     * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
     * mniejsza od zadanej
     */
    struct VertDegValChooserL
    {
        int deg;
        Koala::EdgeDirection type;

        typedef VertDegValChooserL ChoosersSelfType;

        VertDegValChooserL( int adeg = 0, Koala::EdgeDirection atype = Koala::EdAll ): deg( adeg ), type( atype ) { }

        template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
            { return g.deg( v,type ) < deg; }
    };

    VertDegValChooserL vertDegChooseL( int adeg, Koala::EdgeDirection atype = Koala::EdAll )
        { return VertDegValChooserL( adeg,atype ); }

    /* VertDegValChooserG
     * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
     * wieksza od zadanej
     */
    struct VertDegValChooserG
    {
        int deg;
        Koala::EdgeDirection type;

        typedef VertDegValChooserG ChoosersSelfType;

        VertDegValChooserG( int adeg = 0, Koala::EdgeDirection atype = Koala::EdAll ): deg( adeg ), type( atype ) { }

        template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
            { return g.deg( v,type ) > deg; }
    };

    VertDegValChooserG vertDegChooseG( int adeg, Koala::EdgeDirection atype = Koala::EdAll )
        { return VertDegValChooserG( adeg,atype ); }

    /* VertDegSetChooser
     * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc z
     * podanego zbioru
     */
    template< class Int > struct VertDegSetChooser
    {
        Koala::Set< Int > set;
        Koala::EdgeDirection type;

        typedef VertDegSetChooser< Int > ChoosersSelfType;

        VertDegSetChooser( const Koala::Set< Int > &aset = Koala::Set< Int >(),
            Koala::EdgeDirection atype = Koala::EdAll ): set( aset ), type( atype ) { }

        template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
            { return set.isElement( g.deg( v,type ) ); }
    };

    template< class Int > VertDegSetChooser< Int > vertDegChoose( Koala::Set< Int > aset,
        Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegSetChooser< Int >( aset,atype ); }

    /* VertDegContainerChooser
     * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
     * z zakresu miedzy podanymi iteratorami - uzywa STLowego find
     */
    template< class Iter > struct VertDegContainerChooser
    {
        Iter begin, end;
        Koala::EdgeDirection typ;

        typedef VertDegContainerChooser< Iter > ChoosersSelfType;

        VertDegContainerChooser( Iter abeg = Iter(), Iter aend = Iter(), Koala::EdgeDirection atype = Koala::EdAll ):
            begin( abeg ), end( aend ), typ( atype ) { }

        template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
            { return std::find( begin,end,g.deg( v,typ ) ) != end; }
    };

    template< class Iter > VertDegContainerChooser< Iter > vertDegChoose( Iter begin, Iter end,
        Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegContainerChooser< Iter >( begin,end,atype ); }

    /* VertDegFunctorChooser
     * decyzja podejmowana na podstawie wartosci obiektu funktora policzonego na stopniu wierzcholka
     */
    // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
    template< class Obj > struct VertDegFunctorChooser
    {
        mutable Obj funktor;
        Koala::EdgeDirection typ;

        typedef VertDegFunctorChooser< Obj > ChoosersSelfType;

        VertDegFunctorChooser( Obj afun = Obj(), Koala::EdgeDirection atype = Koala::EdAll ):
            funktor( afun ), typ( atype ) { }

        template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
            { return (bool)funktor( g.deg( v,typ ) ); }
    };

    template< class Obj > VertDegFunctorChooser< Obj > vertDegFChoose( Obj afun,
        Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegFunctorChooser< Obj >( afun,atype ); }

    // choosery do wybierania krawedzi

    /* EdgeTypeChooser
     * testuje, czy typ krawedzi spelnia podana maske
     */
    struct EdgeTypeChooser
    {
        Koala::EdgeDirection mask;

        typedef EdgeTypeChooser ChoosersSelfType;

        EdgeTypeChooser( Koala::EdgeDirection amask = Koala::EdAll ): mask( amask )
            { mask |= (mask & Directed) ? Directed : 0; }

        template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
            { return bool( g.getType( e ) & mask ); }
    };

    EdgeTypeChooser edgeTypeChoose( Koala::EdgeDirection mask ) { return EdgeTypeChooser( mask ); }

    // choosery zlozone dla krawedzi, sprawdzajace warunkek definiowany dla wierzcholka sa prawda dla jej koncow

    /* EdgeFirstEndChooser
     * test pierwszego konca krawedzi
     */
    template< class Ch > struct EdgeFirstEndChooser
    {
        Ch ch;

        typedef EdgeFirstEndChooser< Ch > ChoosersSelfType;

        EdgeFirstEndChooser( Ch funktor = Ch() ): ch( funktor ) { }

        template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
            { return ch( g.getEdgeEnd1( e ),g ); }
    };

    template< class Ch > EdgeFirstEndChooser< Ch >
        edgeFirstEndChoose( Ch ch ) { return EdgeFirstEndChooser< Ch >( ch ); }

    /* EdgeSecondEndChooser
     * test drugiego konca krawedzi
     */
    template <class Ch> struct EdgeSecondEndChooser
    {
        Ch ch;

        typedef EdgeSecondEndChooser< Ch > ChoosersSelfType;

        EdgeSecondEndChooser( Ch funktor = Ch() ): ch( funktor ) { }

        template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
            { return ch( g.getEdgeEnd2( e ),g ); }
    };

    template< class Ch > EdgeSecondEndChooser< Ch >
        edgeSecondEndChoose( Ch ch ) { return EdgeSecondEndChooser< Ch >( ch ); }

    /* Edge0EndChooser
     * test, czy zaden koniec nie spelnia warunku
     */
    template< class Ch > struct Edge0EndChooser
    {
        Ch ch;

        typedef Edge0EndChooser< Ch > ChoosersSelfType;

        Edge0EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

        template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
            { return !ch( g.getEdgeEnd1( e ),g ) && !ch( g.getEdgeEnd2( e ),g ); }
    };

    template< class Ch > Edge0EndChooser< Ch > edge0EndChoose( Ch ch ) { return Edge0EndChooser< Ch >( ch ); }

    /* Edge1EndChooser
     * test, czy jeden koniec spelnia warunkek
     */
    template< class Ch > struct Edge1EndChooser
    {
        Ch ch;

        typedef Edge1EndChooser< Ch > ChoosersSelfType;

        Edge1EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

        template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
            { return ch( g.getEdgeEnd1( e ),g ) != ch( g.getEdgeEnd2( e ),g ); }
    };

    template< class Ch > Edge1EndChooser< Ch > edge1EndChoose( Ch ch ) { return Edge1EndChooser< Ch >( ch ); }

    /* Edge2EndChooser
     * test, czy oba konice spelniaja warunkek
     */
    template< class Ch > struct Edge2EndChooser
    {
        Ch ch;

        typedef Edge2EndChooser< Ch > ChoosersSelfType;

        Edge2EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

        template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
            { return ch( g.getEdgeEnd1( e ),g ) && ch( g.getEdgeEnd2( e ),g ); }
    };

    template< class Ch > Edge2EndChooser< Ch > edge2EndChoose( Ch ch ) { return Edge2EndChooser< Ch >( ch ); }

    //Castery to funktory ustalajace wartosci pol info w nowych wierz/kraw tworzonych podczas np. kopiowania grafow.
    // Wartosci te powstaja (w rozny sposob) na podstawie inf oryginalnych

    /* StdCaster
     * caster zwyklego rzutowania miedzy dwoma strukturami
     */
    struct StdCaster
    {
        typedef StdCaster CastersSelfType;

        template< class InfoDest, class InfoSour >
            void operator()( InfoDest &dest, InfoSour sour ) { dest = (InfoDest)sour; }
    };

    // i jego funkcja tworzaca
    StdCaster stdCast() { return StdCaster(); }

    /* NoCastCaster
     * caster ustawiajacy wartosc domyslna i ignorujacy oryginalny parametr wspolpracuje z produktami grafow (stad
     * takze operator 3-argumentowy)
     */
    struct NoCastCaster
    {
        typedef NoCastCaster CastersSelfType;

        template< class InfoDest, class InfoSour >
            void operator()( InfoDest &dest, InfoSour sour ) { dest = InfoDest(); }

        template< class InfoDest, class InfoSour1, class InfoSour2 >
            void operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) { dest = InfoDest(); }
    };

    // funkcja tworzaca - dopuszczalny jedynie argument false
    NoCastCaster stdCast( bool arg );

    /* ObjCaster
     * wyliczenie wartosci nowego info poprzez podany funktor wspolpracuje z produktami grafow (stad takze operator
     * 3-argumentowy) jesli funktor je obsluguje
     */
    // TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
    template< class Fun > struct ObjCaster
    {
        typedef ObjCaster< Fun > CastersSelfType;

        mutable Fun funktor;
        ObjCaster( Fun afun = Fun() ): funktor( afun ) { }

        template< class InfoDest, class InfoSour >
            void operator()( InfoDest &dest, InfoSour sour ) { dest = (InfoDest)funktor( sour ); }

        template< class InfoDest, class InfoSour1, class InfoSour2 > void
            operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) { dest = (InfoDest)funktor( sour1,sour2 ); }
    };

    template< class Funktor > ObjCaster< Funktor > stdCast( Funktor f ) { return ObjCaster< Funktor >( f ); }

    /* ValueCaster
     * Caster wpisujacy ustalona wartosc wspolpracuje z produktami grafow (stad takze operator 3-argumentowy)
     */
    template< class T > struct ValueCaster
    {
        typedef ValueCaster< T > CastersSelfType;

        T val;

        ValueCaster( T arg = T() ): val( arg ) { }

        template< class InfoDest, class InfoSour >
            void operator()( InfoDest &dest, InfoSour sour ) { dest = (InfoDest)val; }

        template< class InfoDest, class InfoSour1, class InfoSour2 >
            void operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) { dest = (InfoDest)val; }
    };

    // funkcja tworzaca - podajemy stala wartosc
    template< class T > ValueCaster< T > valCast( T arg = T() ) { return ValueCaster< T >( arg ); }

    //Linkery dzialajace np. podczas kopiowania grafow, wiaza nowo tworzone wierzch/kraw. z ich oryginalami, przez co
    // mozna latwo sprawdzic, ktory element odpowiada ktoremu

    // polowki pelnego linkera, tworza powiazanie  w jedna strone

    /* Std1NoLinker
     * tylko false jest dopuszczalne - brak polaczenia
     */
    struct Std1NoLinker
    {
        Std1NoLinker( bool arg = false ) { koalaAssert( !arg,ExcBase ); }
        template< class Dest, class Sour > void operator()( Dest *, Sour * ) { }
    };

    /* Std1FieldLinker
     * ustawia wskaznik na dolaczany obiekt w srodku struktury info obiektu docelowego (o ile nie byl on NULLem)
     */
    template< class Info, class T > struct Std1FieldLinker
    {
        T Info:: *pt;

        Std1FieldLinker( T Info:: *awsk = 0): pt( awsk ) { }

        template< class Dest, class Sour > void operator()( Dest *wsk, Sour *w )
            { if (pt && wsk) wsk->info.*pt = (T) w; }
    };

    /* Std1AssocLinker
     * dopisuje powiazania do zewnetrznej! tablicy asocjacyjnej podanej w funkcji tworzacej stdLink (o ile argument
     * nie byl NULLem)
     */
    template< class Map > struct Std1AssocLinker
    {
        Map &map;

        Std1AssocLinker( Map &amap ): map( amap ) { }

        template< class Dest, class Sour > void operator()( Dest *wsk, Sour *w ) { if (wsk) map[wsk] = w; }
    };

    /* Std2Linker
     * pelny linker, zawiera obiekty laczace (polowki linkera - j.w.) nowy element ze starym i odwrotnie
     */
    template< class Link1, class Link2 > struct Std2Linker
    {
        typedef Std2Linker< Link1, Link2> LinkersSelfType;

        mutable Link1 dest2sour;
        mutable Link2 sour2dest;

        Link1 &first() { return dest2sour; }
        Link2 &second() { return sour2dest; }

        Std2Linker( Link1 al1, Link2 al2 ): dest2sour( al1 ), sour2dest( al2 ) { }

        template< class Dest, class Sour > void operator()( Dest *wsk, Sour *w );
    };

    // funkcje tworzace polowki linkera - raczej do uzytku wewnetrznego

    Std1NoLinker stdLink( bool a1 ) { return Std1NoLinker( a1 ); }

    template< class Info1, class T1 >
        Std1FieldLinker< Info1,T1 > stdLink( T1 Info1:: *awsk1 ) { return Std1FieldLinker< Info1,T1 >( awsk1 ); }

    template< class Map1 >
        Std1AssocLinker< Map1 > stdLink( Map1 &tab1 ) { return Std1AssocLinker< Map1 >( tab1 ); }

    // funkcje tworzace pelny linker, argumenty podaja wymagany sposob polaczenia elementow nowych z oryginalami
    // (pierwszy argument) i odwrotnie (drugi) argument bool moze byc tylko false (brak polaczenia)

    Std2Linker< Std1NoLinker,Std1NoLinker > stdLink( bool a1, bool a2 );

    template< class Info,class T >
        Std2Linker< Std1NoLinker,Std1FieldLinker< Info,T > > stdLink( bool a1, T Info:: *awsk );

    template< class Map >
        Std2Linker< Std1NoLinker,Std1AssocLinker< Map > > stdLink( bool a1, Map &tab );

    template< class Info1, class T1 >
        Std2Linker< Std1FieldLinker< Info1,T1 >,Std1NoLinker > stdLink( T1 Info1:: *awsk1, bool a2 );

    template< class Info1, class T1, class Info, class T >
        Std2Linker< Std1FieldLinker< Info1,T1 >,Std1FieldLinker< Info,T > > stdLink( T1 Info1:: *awsk1, T Info:: *awsk );

    template< class Info1, class T1, class Map >
        Std2Linker< Std1FieldLinker< Info1,T1 >,Std1AssocLinker< Map > > stdLink( T1 Info1:: *awsk1, Map &tab);

    template< class Map1 >
        Std2Linker< Std1AssocLinker< Map1 >,Std1NoLinker > stdLink( Map1 &tab1, bool a2 );

    template< class Map1, class Info, class T >
        Std2Linker< Std1AssocLinker< Map1 >,Std1FieldLinker< Info,T > > stdLink( Map1 &tab1, T Info:: *awsk );

    template< class Map1, class Map >
        Std2Linker< Std1AssocLinker< Map1 >,Std1AssocLinker< Map > > stdLink( Map1 &tab1, Map &tab );


    // wygodne laczenie chooserow, casterow i linkerow w pary za pomoca &
    template <class  Ch1, class Ch2> std::pair< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
        operator&( Ch1 a, Ch2 b )
        { return std::pair< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >(a,b); }

    template <class  Ch1, class Ch2> std::pair< typename Ch1::CastersSelfType,typename Ch2::CastersSelfType >
        operator&( Ch1 a, Ch2 b )
        { return std::pair< typename Ch1::CastersSelfType,typename Ch2::CastersSelfType >(a,b); }

    template <class  Ch1, class Ch2> std::pair< typename Ch1::LinkersSelfType,typename Ch2::LinkersSelfType >
        operator&( Ch1 a, Ch2 b )
        { return std::pair< typename Ch1::LinkersSelfType,typename Ch2::LinkersSelfType >(a,b); }


#include "defs.hpp"
}

#endif