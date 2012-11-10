#ifndef KOALA_ASSOCTAB
#define KOALA_ASSOCTAB

/* assoctab.h
 *
 */

#include <map>
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

#include "localarray.h"
#include "privates.h"

//tablice asocjacyjne w Koali uzywaja jako kluczy typow wskaznikowych, klucz NULL ma specjalne znaczenie i jest zabroniony

namespace Koala
{
    //Klasa wrapper dostarczajaca wspolnego interfejsu koalowej tablicy asocjacyjnej dla zewnetrznego stalego kontenera.
    //Wprowadzajac do biblioteki nowy typ takiego kontenera nalezy dlan zdefiniowac AssocTabConstInterface
    template< class Container > class AssocTabConstInterface;

    // interfejs do kontenera nie-stalego, jest tworzony automatycznie na podstawie metod z AssocTabConstInterface
    template< class Container > class AssocTabInterface;

    namespace Privates
    {
        // kontrola zgodnosci typow kluczy dla przypisan miedzy roznymi tablicami asocjacyjnymi
        template< class Key > class AssocTabTag { };

    }

    /* AssocTabInterface
     * wrapper dla stl-owej mapy, podobna postac powinny miec wszystkie takie wrappery
     * K - typ kluczy, V - typ wartosci przypisywanych
    */
    template< class K, class V > class AssocTabConstInterface< std::map< K,V > >: public Privates::AssocTabTag< K >
    {
      public:
        // konstruktor pobiera oryginalna mape, ktorej sluzy za interfejs
        AssocTabConstInterface( const std::map< K,V > &acont ): cont( acont ) {}

        // typ klucza tablicy
        typedef K KeyType;
        // typ przypisywanych wartosci
        typedef V ValType;
        typedef std::map< K,V > OriginalType;

        // test, czy mapa zawiera dany klucz
        bool hasKey( K arg ) const { return cont.find( arg ) != cont.end(); }

        // metody do iterowania po kolejnych kluczach tablicy. Zwracaja 0 gdy odpowiedniego klucza nie ma
        K firstKey() const;
        K lastKey() const;
        // mozna podac 0, by uzyskac ostatni/pierwszy klucz
        K prevKey( K ) const;
        K nextKey( K ) const;

        V operator[]( K arg ) const;
        unsigned size() const { return cont.size(); }
        bool empty() const { return this->size() == 0; }
        bool operator!() const { return empty(); }
        // zapisuje klucze tablicy pod zadany iterator
        template< class Iterator > int getKeys( Iterator ) const;

        // referencja do oryginalnego kontenera, na ktorym operuje wrapper
        const std::map< K,V > &cont;
        //  pojemnosc kontenera
        int capacity () const { return std::numeric_limits< int >::max(); }

      protected:
        // metody dzialajace na nie-stalym obiekcie, uzywane do tworzenia metod w AssocTabInterface
        // nie-stala ref. na oryginalny obiekt
        std::map< K,V >& _cont() { return const_cast< std::map< K,V >& >( cont ); }

        // rezerwowanie pojemnosci kontenera docelowego. Z zalozenia kontener powinien
        // dzialac efektywnie (np. bez wewnetrznych realokacji) dopoki liczba kluczy nie przekroczy argumentu
        // tej metody.
        void reserve( int ) { }
        void clear() { _cont().clear(); }
        // usuwanie klucza, zwraca true jesli klucz byl
        bool delKey( K );
        // referencja na wartosc przypisana do arg, wpis z wartoscia domyslna typu ValType jest tworzony jesli klucza nie bylo
        V &get( K arg );
        // wskaznik do wartosci przypisanej do arg, NULL w razie braku klucza
        ValType *valPtr( K arg );
    };

    //Funkcja tworzaca wrapper do podanego kontenera
    template< class T > AssocTabConstInterface< T > assocTabInterf( const T &cont )
        { return AssocTabConstInterface< T >( cont ); }

    // wrapper do kontenera asocjacyjnego nie-stalego, metody tworzone sa na podstawie metod chronionych z AssocTabConstInterface
    template< class T > class AssocTabInterface: public AssocTabConstInterface< T >
    {
      public:
        typedef typename AssocTabConstInterface< T >::KeyType KeyType;
        typedef typename AssocTabConstInterface< T >::ValType ValType;

        // referencja do oryginalnego kontenera
        T &cont;

        // W konstruktorze podajemy oryginalny kontener
        AssocTabInterface( T &acont ): AssocTabConstInterface< T >( acont ), cont( acont ) { }
        AssocTabInterface< T > &operator=( const AssocTabInterface< T > &arg );
        AssocTabInterface< T > &operator=( const AssocTabConstInterface< T > &arg );
        template< class AssocCont > AssocTabInterface< T > &operator=( const AssocCont &arg );

        void reserve( int arg ) { AssocTabConstInterface< T >::reserve( arg ); }
        void clear() { AssocTabConstInterface< T >::clear(); }
        bool delKey( KeyType arg) { return AssocTabConstInterface< T >::delKey( arg ); }
        ValType* valPtr( KeyType arg ) { return AssocTabConstInterface< T >::valPtr( arg ); }
        ValType operator[]( KeyType arg ) const { return AssocTabConstInterface< T >::operator[]( arg ); }

        // w przypadku obiektu nie-stalego zwraca referencje do przypisanej kluczowi wartosci. Nowy klucz dostaje wartosc domyslna typu ValType
        ValType &operator[]( KeyType arg ) { return AssocTabConstInterface< T >::get( arg ); }
    };

    // sprawdzenie adresu oryginalnego kontenera dla interfejsow asocjacyjnych
    namespace Privates
    {
        template< class T > void *asssocTabInterfTest( const T & ) { return 0; }
        template< class T > void *asssocTabInterfTest( const AssocTabConstInterface< T > &arg) { return (void*)(&arg.cont); }
        template< class T > void *asssocTabInterfTest( const AssocTabInterface< T > &arg) { return (void*)(&arg.cont); }
    }

    /* AssocTable
     * Opakowanie dla kontenera asocjacyjnego typu spoza Koali, udostepnia taki kontener w polu cont, jednoczesnie
     * operujac na nim metodami z AssocTabInterface. Typ tworzony automatycznie na podstawie AssocTabConstInterface
     */
    template< class T > class AssocTable: public Privates::AssocTabTag< typename AssocTabInterface< T >::KeyType >
    {
      public:
        T cont;

        AssocTable(): cont(), inter( cont ) { }

        // konstruktory kopiujace i operatory przypisania
        AssocTable( const T &acont ): cont( acont ), inter( cont ) { }
        AssocTable( const AssocTable< T > &X ): cont( X.cont ), inter( cont ) {}
        AssocTable< T > &operator=( const AssocTable< T > & );
        AssocTable< T > &operator=( const T & );
        template< class AssocCont > AssocTable< T > &operator=( const AssocCont &arg );

        typedef typename AssocTabInterface< T >::KeyType KeyType;
        typedef typename AssocTabInterface< T >::ValType ValType;
        typedef typename AssocTabInterface< T >::OriginalType OriginalType;

        bool hasKey( KeyType arg ) const { return inter.hasKey( arg ); }
        ValType* valPtr( KeyType arg ) { return inter.valPtr( arg ); }
        bool delKey( KeyType arg ) { return inter.delKey( arg ); }
        KeyType firstKey() const { return inter.firstKey(); }
        KeyType lastKey() const { return inter.lastKey(); }
        KeyType prevKey( KeyType arg ) const { return inter.prevKey( arg ); }
        KeyType nextKey( KeyType arg ) const { return inter.nextKey( arg ); }
        ValType &operator[]( KeyType arg) { return inter[arg]; }
        ValType operator[]( KeyType arg ) const { return ((AssocTabConstInterface< T >&)inter).operator[]( arg ); }
        unsigned size() const { return inter.size(); }
        bool empty() const { return inter.empty(); }
        bool operator!() const { return empty(); }
        void clear() { inter.clear(); }

        template< class Iterator > int getKeys( Iterator iter ) const { return inter.getKeys( iter ); }

        // nieobowiazkowe, moze byc nieobslugiwane przez niektore kontenery
        int capacity() const { return inter.capacity(); }
        void reserve( int n ) { inter.reserve( n ); }
        AssocTable( int n, void* p = 0 ): cont(), inter( cont ) { inter.reserve( n ); }

    private:
        // interfejs na pole cont
        AssocTabInterface< T > inter;
    };

    // Funkcja tworzaca kopie podanego kontenera opakowana w AssocTable
    template< class T > AssocTable< T > assocTab( const T &cont ) { return AssocTable< T >( cont ); }

    //Klasy pomocnicze dla szybkich tablic asocjacyjnych (AssocArray). Klucz musi byc typem wskaznikowym zawierajacym
    //pole AssocKeyContReg assocReg, a w chwili wywolania kazdej metody z argumentem Klucz, argument ten musi
    // byc aktualnie wskaznikiem na aktualnie istniejacy (zywy) obiekt odpowiedniego typu.
    // Kontener dziedziczy po klasie abstrakcyjnej AssocContBase - wowczas obiekt zawierajacy assocReg
    //w chwili smierci automatycznie wypisuje klucze bedace wskazaniami na niego ze wszystkich takich kontenerow.
    //W Koali glownie wykorzystywane dla tablic asocj. wierz/krawedzi

    /* AssocContBase
     *
     */
    struct AssocContReg;
    class AssocContBase
    {
      public:
        virtual void DelPosCommand( int ) = 0;
        virtual AssocContReg &getReg( int ) = 0;
    };

    /* AssocContReg
     *
     */
    class AssocContReg
    {
        template< class K, class E, class Cont > friend class AssocArray;
        friend class AssocKeyContReg;

        int nextPos;
        AssocContBase *next;
    };

    /* AssocKeyContReg
     *
     * Pole publiczne assocReg tego typu powinny miec obiekty, wskazniki na ktore moga byc kluczami w AssocArray
     */
    class AssocKeyContReg: public AssocContReg
    {
        template< class K, class E, class Cont > friend class AssocArray;

      public:
        AssocKeyContReg() { next = 0; }
        AssocKeyContReg( const AssocKeyContReg & ) { next = 0; }
        AssocKeyContReg &operator=( const AssocKeyContReg & );
        ~AssocKeyContReg() { deregister(); }

      private:
        AssocContReg *find( AssocContBase *cont );
        void deregister();
    };

    /* BlockOfAssocArray
     *
     */
    template< class Klucz, class Elem > struct BlockOfAssocArray
    {
        Elem val;
        Klucz key;
        AssocContReg assocReg;
    };

    namespace Privates
    {

        // test czy Klucz jest wskaznikiem zawierajacym pole AssocKeyContReg assocReg - tylko wowczas kompiluje sie konstruktor domyslny
        template< class Klucz > class KluczTest
        {
          public:
            KluczTest( Klucz v = 0 ) { AssocKeyContReg *ptr = &v->assocReg; }
        } ;
    }

    /* AssocArray
     *
     */
    template< class Klucz, class Elem > struct AssocArrayInternalTypes
    {
        typedef Privates::BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > BlockType;
    };

    // Szybka tablica asocjacyjna np. dla wierz/kraw
    // Wiekszosc interfejsu jak w innych tabl. assocjacyjnych
    template< class Klucz, class Elem, class Container = std::vector< typename
        AssocArrayInternalTypes<Klucz,Elem>::BlockType > > class AssocArray: public AssocContBase,
            protected Privates::KluczTest< Klucz >, public Privates::AssocTabTag< Klucz >
    {
      protected:
        mutable Privates::BlockList< BlockOfAssocArray< Klucz,Elem >,Container > tab;

        virtual void DelPosCommand( int pos ) { tab.delPos( pos ); }
        virtual AssocContReg &getReg( int pos ) { return tab[pos].assocReg; }

      public:
        typedef Klucz KeyType;
        typedef Elem ValType;

        typedef Container ContainerType;

        // asize - rozmiar poczatkowy, drugi arg. ignorowany
        AssocArray( int asize = 0, void *p = 0 ): tab( asize ) { }
        AssocArray( const AssocArray< Klucz,Elem,Container > & );

        AssocArray< Klucz,Elem,Container > &operator=( const AssocArray< Klucz,Elem,Container > & );

        template< class AssocCont > AssocArray &operator=( const AssocCont &arg );

        // tylko dla Container==VectorInterface<BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > >*>
        // pozwala zorganizowac AssocArray bez uzywania pamieci dynamicznej, na zewnetrznej, dostarczonej w konstruktorze tablicy
        // TODO: sprawdzic niekopiowalnosc tablicy w tym przypadku
        // TODO: rozwazyc usuniecie w finalnej wersji biblioteki
        AssocArray( int asize, typename AssocArrayInternalTypes< Klucz,Elem >::BlockType *wsk ): tab( wsk,asize ) { }

        int size() const { return tab.size(); }
        bool empty() const { return tab.empty(); }
        bool operator!() const { return empty(); }
        void reserve( int arg ) { tab.reserve( arg ); }
        int capacity() const { return tab.capacity(); }
        bool hasKey( Klucz v ) const { return keyPos( v ) != -1; }
        Elem *valPtr( Klucz v );
        // pozycja klucza w wewnetrznym indeksie tablicy, -1 w razie jego braku
        int keyPos( Klucz ) const;
        bool delKey( Klucz );
        Klucz firstKey() const;
        Klucz lastKey() const;
        Klucz nextKey( Klucz ) const;
        Klucz prevKey( Klucz ) const;
        Elem &operator[]( Klucz );
        Elem operator[]( Klucz ) const;
        // porzadkowanie indeksu kontenera, wszystkie klucze uzyskuja kolejne numery poczatkowe
        void defrag();
        void clear();

        template< class Iterator > int getKeys( Iterator ) const;

        ~AssocArray() { clear(); }
    };

    // Test w kodzie szablonu, czy typ kontenera to AssocArray zorganizowany na zewnwetrznej tablicy.
    // TODO: rozwazyc usuniecie w finalnej wersji biblioteki
    template< class T > struct AssocArrayVectIntSwitch
    {
        typedef void* BufType;
        static bool isAAVI() { return false; }
    };

    template< class K, class E > struct AssocArrayVectIntSwitch< AssocArray< K,E,VectorInterface<
        typename AssocArrayInternalTypes< K,E >::BlockType * > > >
    {
        typedef typename AssocArrayInternalTypes< K,E >::BlockType *BufType;
        static bool isAAVI() { return true; }
    };

// Kontener udajacy AssocArray np. dla kluczy nie wspolpracujacych z tym kontenerem. Uzywa dodatkowego
// wewnetrznego kontenera typu AssocCont (mapa: Klucz->int). Nie oferuje automatycznego wypisywania kluczy bedacych
// wskaznikami na znikajace obiekty. Chyba jedyne sensowne zastosowanie, to wykorzystanie jako IndexContainer
// w ponizszejh AssocMatrix w sytuacji, gdy jej klucz nie obsluguje AssocArray

// TODO: sprawdzic, czy ponizsza AssocMatrix nadal dziala wyposazona w indeks tego typu
template< class Klucz, class Elem, class AssocCont, class Container =
    std::vector< typename AssocArrayInternalTypes< Klucz,Elem >::BlockType > > class PseudoAssocArray:
        public Privates::AssocTabTag< Klucz >
    {
      protected:
        mutable Privates::BlockList< BlockOfAssocArray< Klucz,Elem >,Container > tab;
        AssocCont assocTab;

      public:
        typedef Klucz KeyType;
        typedef Elem ValType;

        typedef Container ContainerType;
        typedef AssocCont AssocContainerType;

        PseudoAssocArray( int asize = 0, void *p = 0 ): tab( asize ), assocTab( asize ) { }

        template< class AssocCont2 >
            PseudoAssocArray< Klucz,Elem,AssocCont,Container > &operator=( const AssocCont2 &arg );
        int size() const { return tab.size(); }
        bool empty() const  { return tab.empty(); }
        bool operator!() const { return empty(); }
        void reserve( int arg );
        int capacity() const { return tab.capacity(); }
        bool hasKey( Klucz v ) const { return keyPos( v ) != -1; }
        Elem *valPtr( Klucz v );
        int keyPos( Klucz ) const;
        bool delKey( Klucz );
        Klucz firstKey() const;
        Klucz lastKey() const;
        Klucz nextKey( Klucz ) const;
        Klucz prevKey( Klucz ) const;
        Elem &operator[]( Klucz );
        Elem operator[]( Klucz ) const;
        void defrag();
        void clear();

        template< class Iterator > int getKeys( Iterator ) const;

        ~PseudoAssocArray() { clear(); }
    };

    // Typy pomocnicze 2-wymiarowych tablic asocjacyjnych (oba wymiary tego samego typu)
    enum AssocMatrixType
    {
        AMatrFull, // pelna tablica 2-wymiarowa
        AMatrNoDiag, // tablica 2-wymiarowa nie akceptujaca kluczy o obu wspolrzednych rownych (kluczami sa tylko 2-elementowe pary)
        AMatrTriangle, // tablica traktujaca klucze jako pary nieuporzadkowane tj. klucz (a,b) jest utozsamiany z (b,a)
        AMatrClTriangle  // j.w. ale dopusza sie klucze "jednoelementowe" tj. postaci (a,a)
    };

    // klasa wspomagajaca operowanie na kluczach tablic 2-wymiarowych
    // Uzywana wewnatrz tego kontenera
    template< AssocMatrixType > class AssocMatrixAddr;

    /* AssocMatrixAddr
     *
     */
    template<> class AssocMatrixAddr< AMatrFull >
    {
      public:
        // dlugosc wewnetrznego bufora dla podanej liczby kluczy
        static int bufLen( int n ) { return n * n; }
        // przerabia pare numerow elementow klucza (w indeksie kluczy) na pozycje bufora wewnetrznego
        inline int wsp2pos( std::pair< int,int > ) const;
        // ... i odwrotnie
        inline std::pair< int,int > pos2wsp( int ) const;
        // test, czy klucz danej postaci jest akceptowany przez ten typ tablicy
        template< class T > bool correctPos( T, T ) const { return true; }

        // przerabia klucz 2-wymiarowy (tj. pare kluczy) na postac standardowa dla danego typu tablicy
        template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
            { return std::pair< Klucz,Klucz >( u,v ); }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const { return k; }
    };

    template<> class AssocMatrixAddr< AMatrNoDiag >
    {
        public:
            static int bufLen( int n ) { return n * (n - 1); }
            inline int wsp2pos( std::pair< int,int > ) const ;
            inline std::pair< int,int > pos2wsp( int ) const ;
            template< class T > bool correctPos( T u, T v )  const  { return u != v; }
            template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
                { return std::pair< Klucz,Klucz >( u,v ); }
            template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const
                { return k; }
    };

    template<> class AssocMatrixAddr< AMatrClTriangle >
    {
      public:
        static int bufLen( int n )  { return n * (n + 1) / 2; }
        inline int wsp2pos( std::pair< int,int > ) const ;
        inline std::pair< int,int > pos2wsp( int ) const ;
        template< class T > bool correctPos( T, T ) const  { return true; }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
            { return pairMinMax( u,v ); }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const
            { return pairMinMax( k.first,k.second ); }
    };

    template <> class AssocMatrixAddr< AMatrTriangle >
    {
      public:
        static int bufLen( int n ) { return n * (n - 1) / 2; }
        inline int wsp2pos( std::pair< int,int > ) const ;
        inline std::pair< int,int > pos2wsp( int ) const ;
        template< class T > bool correctPos( T u, T v )  const { return u != v; }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
            { return pairMinMax( u,v ); }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const
            { return pairMinMax( k.first,k.second ); }
    };

    // BlockOfAssocMatrix
    template< class Elem > struct BlockOfAssocMatrix
    {
        Elem val;
        int next,prev;
        bool present() const { return next || prev; }
        BlockOfAssocMatrix(): next( 0 ), prev( 0 ), val() { }
    };

    namespace Privates
    {
        // kontrola zgodnosci typow kluczy dla przypisan miedzy roznymi tablicami asocjacyjnymi
        template< class Key,AssocMatrixType > class AssocMatrixTag { };
    }

    /* AssocMatrix
     *
     */
    template <class Klucz, class Elem>
    struct AssocMatrixInternalTypes
    {
        typedef BlockOfAssocMatrix< Elem > BlockType;
        typedef Privates::BlockOfBlockList< BlockOfAssocArray< Klucz,int > > IndexBlockType;
    };

// 2-wymiarowa tablica asocjacyjna. Uwaga: wersja z IndexContainer = IndexContainer = AssocArray< Klucz,...>
// ma te same ograniczenia odnosnie uzywanych kluczy, co AssocArray (j.w.)
    template< class Klucz, class Elem, AssocMatrixType aType, class Container =
        std::vector< typename AssocMatrixInternalTypes<Klucz,Elem>::BlockType >, class IndexContainer =
            AssocArray< Klucz,int,std::vector< typename AssocMatrixInternalTypes< Klucz,Elem >::IndexBlockType > > >
// Container - typ wewnetrznego bufora - tablicy przechowujacej opakowany ciag wartosci przypisanych roznym parom kluczy
// IndexContainer - typ indeksu tj. tablicy asocjacyjnej przypisujacej pojedynczym kluczom ich liczby wystapien we wpisach oraz (rozne) numery.
            class AssocMatrix: public AssocMatrixAddr< aType >, public Privates::AssocMatrixTag< Klucz,aType >
    {
        template< class A, class B, AssocMatrixType C, class D, class E > friend class AssocMatrix;

      private:
        class AssocIndex: public IndexContainer
        {
          public:
            AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > *owner;

            // rozmiar poczatkowy
            AssocIndex( int asize = 0 ): IndexContainer( asize ) { }
            // umozliwia stworzenie indeksu dzialajacego w zewnetrznym buforze tablicowym
            // Dziala, gdy IndexContainer to AssocArray oparta na VectorInterface
            // TODO: rozwazyc usuniecie w finalnej wersji biblioteki
            AssocIndex( int asize, typename AssocMatrixInternalTypes< Klucz,Elem >::IndexBlockType *indbuf ):
                IndexContainer( asize,indbuf ) { }
            // konwersja klucza na jego numer, -1 w razie braku
            int klucz2pos( Klucz );
            // i odwrotnie
            Klucz pos2klucz( int );
            virtual void DelPosCommand( int );

            friend class AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >;
        };

        // wewnetrzny indeks pojedynczych kluczy (mapa: Klucz->int)
        mutable AssocIndex index;

        friend class AssocIndex;

        // glowny bufor z wartosciami
        mutable Container bufor;
        // wartosci przypisane parom (kluczom) sa w buforze powiazane w liste 2-kierunkowa
        int siz,first,last;

        // usuniecie wpisu dla kluczy o podanych numerach
        void delPos( std::pair< int,int >  );

      protected:
        struct DefragMatrixPom
        {
            Klucz u,v;
            Elem val;
        };

      public:
        // typ klucza
        typedef Klucz KeyType;
        // typ wartosci przypisywanej parze (tj. kluczowi 2-wymiarowemu)
        typedef Elem ValType;

        typedef Container ContainerType;
        typedef IndexContainer IndexContainerType;
        enum { shape = aType };

        // pierwszy arg. - poczatkowy rozmiar (tj. pojemnosc indeksu pojedynczych kluczy), dalsze arg. ignorowane
        AssocMatrix( int = 0, void *p = 0, void *q = 0 );
        AssocMatrix( const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > &X ):
            index( X.index ), bufor( X.bufor ), siz( X.siz ), first( X.first ), last( X.last ) { index.owner = this; }

        AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >
            &operator=( const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > & );
        template< class MatrixContainer > AssocMatrix &operator=( const MatrixContainer &X );

        // Umozliwia dzialanie tablicy w 2 zewnetrznych buforach tablicowych (bufora i indeksu)
        // Wowczas Container i IndexContainer powinny opierac sie na VectorInterface
        // TODO: sprawdzic czy nadal dziala i czy kontener jest wtedy nieprzepisywalny.
        // TODO: rozwazyc usuniecie w finalnej wersji biblioteki
        AssocMatrix( int asize, typename AssocMatrixInternalTypes<Klucz,Elem>::BlockType* contBuf,
                    typename AssocMatrixInternalTypes<Klucz,Elem>::IndexBlockType* indBuf );

        // operowanie na indeksie zawierajacym pojedyncze klucze
        bool hasInd( Klucz v ) const { return index.hasKey( v ); }
        // usuwa takze wszystkie wpisy zawierajace argument jako jeden z elementow pary kluczy
        bool delInd( Klucz );
        Klucz firstInd() const { return index.firstKey(); }
        Klucz lastInd() const { return index.lastKey(); }
        Klucz nextInd( Klucz v )const  { return index.nextKey( v ); }
        Klucz prevInd( Klucz v ) const { return index.prevKey( v ); }
        int indSize() const { return index.size(); }

        // zapis do zewnetrznej mapy (Klucz->ValType) wartosci wszystkich wpisow dla kluczy 2-wymiarowych zawierajacych argument na pierwszej pozycji
        template< class Elem2, class ExtCont > int slice1( Klucz, ExtCont & ) const;
        // ... i drugiej pozycji
        template< class Elem2, class ExtCont > int slice2( Klucz, ExtCont & ) const;

        //W operacjach dzialajacych na kluczach 2-wymiarowych mozna poslugiwac sie para kluczy lub jej dwoma wspolrzednymi

        // czy wpis dla danej pary kluczy jest w tablicy
        bool hasKey( Klucz, Klucz ) const;
        bool hasKey( std::pair< Klucz,Klucz > k ) const { return hasKey( k.first,k.second ); }

        // usun wspis, zwraca false jesli klucza nie bylo
        bool delKey( Klucz, Klucz );
        bool delKey( std::pair< Klucz,Klucz > k ) { return delKey( k.first,k.second ); }

        // referencja do wartosc przypisanej kluczowi 2-wymiarowemu, jesli klucza nie bylo - tworzona jest wartosc domyslna typu ValType
        Elem &operator()( Klucz, Klucz );
        Elem &operator()( std::pair< Klucz,Klucz > k ) { return operator()( k.first,k.second ); }

        // wartosc przypisana kluczowi 2-wymiarowemu, jesli klucza nie bylo - wartosc domyslna typu ValType
        Elem operator()( Klucz, Klucz ) const ;
        Elem operator()( std::pair< Klucz,Klucz > k ) const { return operator()( k.first,k.second ); }

        // wskaznik do wartosci przypisanej kluczowi 2-wymiarowemu, NULL w razie braku wpisu
        Elem* valPtr( Klucz, Klucz );
        Elem* valPtr( std::pair< Klucz,Klucz > k ) { return valPtr(k.first,k.second); }

        // iterowanie po kolejnych kluczach 2-wymiarowych zawartych w tablicy
        std::pair< Klucz,Klucz > firstKey() const ; // dla tablicy pustej zwraca dwa zera
        std::pair< Klucz,Klucz > lastKey() const ;  // dla tablicy pustej zwraca dwa zera
        std::pair< Klucz,Klucz > nextKey( Klucz, Klucz ) const ; // dla pary zerowej zwraca pierwszy klucz
        std::pair< Klucz,Klucz > nextKey( std::pair< Klucz,Klucz > k ) const { return nextKey( k.first,k.second ); }
        // dla pary zerowej zwraca ostatni klucz
        std::pair< Klucz,Klucz > prevKey( Klucz, Klucz ) const ;
        std::pair< Klucz,Klucz > prevKey( std::pair< Klucz,Klucz > k ) const { return prevKey( k.first,k.second ); }

        // liczba wpisow
        int size()  const { return siz; }
        bool empty()  const { return !siz; }
        bool operator!() const { return empty(); }
        void clear();
        // zaalokowuje pamiec na podana liczbe kluczy (1-wymiarowych)
        void reserve( int );

        // porzadkowanie macierzy
        void defrag();

        // zapis par (kluczy 2-wymiarowych) na podany iterator, zwraca rozmiar tablicy
        template< class Iterator > int getKeys( Iterator ) const;
    };

    // Klasa testujaca, czy typ T jest macierza asocjacyjna zorganizowana na zewnetrznych buforach tablicowych
    // TODO: rozwazyc usuniecie w finalnej wersji biblioteki
    template< class T > struct AssocMatrixVectIntSwitch
    {
        typedef void *BufType;
        typedef void *IndBufType;
        static bool isAMVI() { return false; }
    };

    template< class K, class E, AssocMatrixType aType > struct
        AssocMatrixVectIntSwitch< AssocMatrix< K,E,aType,
        VectorInterface< typename AssocMatrixInternalTypes< K,E >::BlockType * >,AssocArray<K,int,
        VectorInterface< typename AssocMatrixInternalTypes< K,E >::IndexBlockType * > > > >
    {
        typedef typename AssocMatrixInternalTypes< K,E >::BlockType *BufType;
        typedef typename AssocMatrixInternalTypes< K,E >::IndexBlockType *IndBufType;
        static bool isAMVI() { return true; }
    };

    /* AssocInserter
     * iterator wstawiajacy do podanego przez adres kontenera asocjacyjnego pary (klucz, wartosc)
     */
    template< class T > class AssocInserter: public std::iterator< std::output_iterator_tag,void,void,void,void >
    {
      protected:
        T* container;

      public:
        typedef T container_type;
        AssocInserter( T &x ): container( &x ) { }
        template< class K, class V > AssocInserter< T > &operator=( const std::pair< K,V > & );
        AssocInserter< T > &operator*() { return *this; }
        AssocInserter< T > &operator++() { return *this; }
        AssocInserter< T > operator++( int ) { return *this; }
    };

    // ... i jego funkcja tworzaca dla podanego kontenera
    template< class T > AssocInserter< T > assocInserter( T &x ) { return AssocInserter< T >( x ); }

    /* AssocFunktorMatrix
     * iterator wstawiajacy do podanego przez adres kontenera asocjacyjnego klucze, ktorym przypisywane sa wartosci
     * wyznaczane (na podstawie kluzca) zadanym funktorem
     * TODO: sprawdzic, czy dziala ze zwyklymi funkcjami C  pobierajacymi argument przez wartosc, referencje lub const ref
     */
    template< class T, class Fun > class AssocFunktorInserter:
        public std::iterator< std::output_iterator_tag,void,void,void,void >
    {
      protected:
        T* container;
        mutable Fun funktor;

      public:
        typedef T container_type;
        typedef Fun FunktorType;
        AssocFunktorInserter( T &x, Fun f ): container( &x ), funktor( f ) { }
        template< class K > AssocFunktorInserter< T,Fun > &operator=( const K & );
        AssocFunktorInserter< T,Fun > &operator*() { return *this; }
        AssocFunktorInserter< T,Fun > &operator++() { return *this; }
        AssocFunktorInserter< T,Fun > operator++( int ) { return *this; }
    };

    // ... i jego funkcja tworzaca dla podanego kontenera i funktora (typu KeyType -> ValType)
    template< class T, class F > AssocFunktorInserter< T,F > assocInserter( T &x, F f )
        { return AssocFunktorInserter< T,F >( x,f ); }

    namespace Privates
    {
        template< class Cont > std::ostream &printAssoc( std::ostream &out, const Cont &cont );
    };

    // wypisywanie do strumienia wszystkich wpisow w podanych tablicach asocjacyjnych, dziala jesli typ wartosci takze obsluguje <<

    template< typename T > std::ostream &operator<<( std::ostream &out, const AssocTabConstInterface< T > & cont )
        { return Privates::printAssoc( out,cont ); }

    template< typename T > std::ostream &operator<<( std::ostream &out, const AssocTable< T > & cont )
        { return Privates::printAssoc( out,cont ); }

    template< class K, class V,class C > std::ostream &operator<<( std::ostream &out, const AssocArray< K,V,C > & cont )
        { return Privates::printAssoc( out,cont ); }

    template< typename K, typename V, typename A, typename C >
        std::ostream &operator<<( std::ostream &out, const PseudoAssocArray< K,V,A,C > & cont )
        { return Privates::printAssoc( out,cont ); }

    template< class Klucz, class Elem, AssocMatrixType aType, class C, class IC >
        std::ostream &operator<<( std::ostream &out, const AssocMatrix< Klucz,Elem,aType,C,IC > & cont );

#include "assoctab.hpp"
}

#endif
