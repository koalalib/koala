#ifndef KOALA_JOINABLE_SETS_H
#define KOALA_JOINABLE_SETS_H

/* joinsets.h
 * 
 */

#include <cassert>
#include <map>

#include "../container/assoctab.h"
#include "../container/localarray.h"
#include "../container/set.h"

namespace Koala
{

    /* JSPartDesrc
     * struktura pomocnicza dla zbiorow zlaczalnych o elementach typu Klucz
     */
    template< class Klucz > class JSPartDesrc
    {   
        template< class K,class Cont > friend class JoinableSets;

        JSPartDesrc *parent,*next,*first,*last;
        unsigned int deg,size;
        Klucz key;
    };

    template< class Container > class VectorInterface;

    /* JoinableSetsInternalTypes
     * 
     */
    template< class ITEM > struct JoinableSetsInternalTypes
    {
        typedef Privates::BlockOfBlockList< BlockOfAssocArray< ITEM,JSPartDesrc< ITEM > * > > AssocBlockType;
        typedef JSPartDesrc< ITEM > BufElementType;
    } ;

    /* JoinableSets 
     * Klasa zbioru rozlacznych zbiorow zlaczalnych o elementach typu wskaznikowego ITEM
     * AssocContainer to typ wewnetrznej tablicy asoc. ITEM->JSPartDesrc< ITEM > *
     */
    template< class ITEM, class AssocContainer = AssocArray< ITEM,JSPartDesrc< ITEM > * > > class JoinableSets
    {
      protected:
        AssocContainer mapa;
        JSPartDesrc< ITEM > *bufor;
        size_t siz,part_no,maxsize;

      public:
        // typ reprezentanta pojedynczego zbioru
        typedef JSPartDesrc< ITEM > *Repr; 
        // typ elementow zbiorow
        typedef ITEM ElemType; 

        // pierwszy argument - maks. liczba elementow wszystkich zbiorow, pozostale arg. ignorowane
        JoinableSets( unsigned int = 0, void* p = 0, void *q = 0 ); 
        JoinableSets( const JoinableSets< ITEM,AssocContainer > & );
        JoinableSets &operator=( const JoinableSets< ITEM,AssocContainer > & );
        ~JoinableSets() { resize( 0 ); }

        // Mozliwosc tworzenia kontenera operujacego na dostarczonych zewnetrznych tablicach
        // Wowczas AssocContainer powinien opierac sie na VectorInterface
        // TODO: sprawdzic niekopiowalnosc obiektu w tym przypadku
        // TODO: rozwazyc usuniecie z finalnej wersji biblioteki
        JoinableSets( unsigned int n, JSPartDesrc< ITEM > * buf,
            typename JoinableSetsInternalTypes< ITEM >::AssocBlockType * mapbuf ): mapa( n,mapbuf ), siz( 0 ),
                bufor( buf ), part_no( 0 ), maxsize( n ) { }

        // czysci kontener i zmienia maks. liczbe elementow wszystkich zbiorow
        void resize( unsigned int ); 

        // liczba wszystkich elementow w kontenerze
        int size() const { return siz; } 
        // liczba elementow w zbiorze o podanym reprezentancie
        int size( typename JoinableSets< ITEM >::Repr ) const; 
        // liczba elementow zbioru zawierajacego argument, 0 jesli argument nie byl jeszcze wprowadzony
        int size( const ITEM & ) const; 

        // sprawdzenie, czy kontener jest pusty
        bool empty() const { return siz == 0; }
        bool operator!() const { return empty(); }

        void clear() { resize( 0 ); }
        // liczba zbiorow
        int getSetNo() const { return part_no; } 
        // wpisuje elementy wszystkich zbiorow na podany iterator, zwraca size()
        template< class Iter > int getElements( Iter ) const; 

        // wypisuje na podany iterator reprezentatow wszystkich zbiorow
        template< class Iter > int getSetIds( Iter ) const; 
        // wypisuje na podany iterator elementy zbioru o podanym reprezencie
        template< class Iter > int getSet( typename JoinableSets< ITEM >::Repr, Iter ) const; 
        // wypisuje na podany iterator elementy zbioru zawierajaccego podany element
        template< class Iter > int getSet( const ITEM &i, Iter iter ) const { return getSet( getSetId( i ),iter ); }

        // tworzy nowy 1-elementowy zbior, zwraca jego reprezentanta lub 0 jesli element juz byl w jakims zbiorze
        // Jedyna metoda wprowadzajaca  zbiory z nowymi elementami do przechowywanej rodziny
        typename JoinableSets< ITEM >::Repr makeSinglet( const ITEM & );

        // zwraca reprezentanta zbioru zawierajacego dany element, lub 0 w razie jego braku
        typename JoinableSets<ITEM>::Repr getSetId( const ITEM & ) const;
        // zwraca reprezentanta zbioru zawierajacego zbior o danym reprezentancie np. z wczesniejszego etapu scalania
        typename JoinableSets<ITEM>::Repr getSetId( typename JoinableSets< ITEM >::Repr ) const;

        // zlacznia dwoch zbiorow podanych przez zadanie reprezentanta lub ktoregos z elementow
        // zwracaja reprezentanta wynikowego scalonego zbioru lub 0 w razie porazki (np. brak zbioru do scalenia, albo scalamy ze soba ten sam zbior)
        typename JoinableSets<ITEM>::Repr join( typename JoinableSets< ITEM >::Repr,
            typename JoinableSets< ITEM >::Repr );
        typename JoinableSets< ITEM >::Repr join( const ITEM &, const ITEM & );
        typename JoinableSets< ITEM >::Repr join( typename JoinableSets< ITEM >::Repr, const ITEM & );
        typename JoinableSets< ITEM >::Repr join( const ITEM &, typename JoinableSets< ITEM >::Repr );
    };

    // wypisywanie zawartosci zbiorow zlaczalnych do strumienia
    template< typename Element, typename Cont >
        std::ostream &operator<<( std::ostream &,const JoinableSets< Element,Cont > & );

    /* JoinableSetsVectIntSwitch
     * test, czy kontener dziala na tablicach zewnetrznych
     */
    // TODO: rozwazyc usuniecie z finalnej wersji biblioteki
    template< class T > struct JoinableSetsVectIntSwitch
    {
        typedef void* BufType;
        typedef void* MapBufType;
        static bool isJSVI() { return false; }
    };

    template< class ITEM > struct JoinableSetsVectIntSwitch< JoinableSets<ITEM, AssocArray<ITEM, JSPartDesrc<ITEM> *,
        VectorInterface< Privates::BlockOfBlockList< BlockOfAssocArray< ITEM,JSPartDesrc<ITEM> * > > * > > > >
    {
        typedef JSPartDesrc< ITEM > * BufType;
        typedef Privates::BlockOfBlockList< BlockOfAssocArray< ITEM,JSPartDesrc< ITEM > * > > *MapBufType;
        static bool isJSVI() { return true; }
    };

#include "joinsets.hpp"
}

#endif
