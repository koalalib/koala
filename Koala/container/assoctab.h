#ifndef KOALA_ASSOCTAB
#define KOALA_ASSOCTAB

#include <map>
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include "localarray.h"

namespace Koala
{

template< class Container > class AssocTabInterface;

template< class T >
AssocTabInterface< T > assocTabInterf( T &cont );

/* ------------------------------------------------------------------------- *
 * AssocTabInterface< M >
 *
 *
 * ------------------------------------------------------------------------- */

template< class K, class V > class AssocTabInterface< std::map< K,V > >
{
    public:
        AssocTabInterface( std::map< K,V > &acont ): cont( acont ) {}

        typedef K KeyType;
        typedef V ValType;

        bool hasKey( K arg ) { return cont.find( arg ) != cont.end(); }
        bool delKey( K );

        K firstKey();
        K lastKey();
        K prevKey( K );
        K nextKey( K );

        V &operator[]( K arg ) { return cont[arg]; }
        unsigned size() { return cont.size(); }
        bool empty() { return this->size() == 0; }
        void clear() { cont.clear(); }
        template< class Iterator > int getKeys( Iterator );

        std::map< K,V > &cont;
        int capacity () { return std::numeric_limits<int>::max(); }
        void reserve(int) {  }
} ;



template< class T > class AssocTabInterface< AssocTabInterface< T > >
{
    public:
        AssocTabInterface( AssocTabInterface< T > &acont ): cont( acont ) { }

        typedef typename AssocTabInterface< T >::KeyType KeyType;
        typedef typename AssocTabInterface< T >::ValType ValType;

        bool hasKey( KeyType arg ) { return cont.hasKey( arg ); }
        bool delKey( KeyType arg ) { return cont.delKey( arg ); }
        KeyType firstKey() { return cont.firstKey(); }
        KeyType lastKey() { return cont.lastKey(); }
        KeyType prevKey( KeyType arg ) {return cont.prevKey( arg ); }
        KeyType nextKey( KeyType arg ) {return cont.nextKey( arg ); }
        ValType &operator[]( KeyType arg ) { return cont[arg]; }
        unsigned size() { return cont.size(); }
        bool empty() { return cont.empty(); }
        void clear() { cont.clear(); }

        template< class Iterator > int getKeys( Iterator );

        // nieobowiazkowe, moze byc nieobslugiwane przez niektore kontenery
        int capacity () { return cont.capacity(); }
        void reserve(int n) { cont.reserve(n); }

        AssocTabInterface< T > &cont;
};

template< class T > AssocTabInterface< T > assocTabInterf( T & );

/* ------------------------------------------------------------------------- *
 * AssocTable< T >
 *
 *
 * ------------------------------------------------------------------------- */

template< class T > class AssocTable
{
    public:
        T cont;

        AssocTable(): cont(), inter( cont ) { }
        AssocTable( const T &acont ): cont( acont ), inter( cont ) { }
        AssocTable( const AssocTable< T > &X ): cont( X.cont ), inter( cont ) {}
        AssocTable< T > &operator=( const AssocTable< T > & );
        AssocTable< T > &operator=( const T & );

        typedef typename AssocTabInterface< T >::KeyType KeyType;
        typedef typename AssocTabInterface< T >::ValType ValType;

        bool hasKey( KeyType arg ) { return inter.hasKey( arg ); }
        bool delKey( KeyType arg ) { return inter.delKey( arg ); }
        KeyType firstKey() { return inter.firstKey(); }
        KeyType lastKey() { return inter.lastKey(); }
        KeyType prevKey( KeyType arg ) { return inter.prevKey( arg ); }
        KeyType nextKey( KeyType arg ) { return inter.nextKey( arg ); }
        ValType &operator[]( KeyType arg) { return inter[arg]; }
        unsigned size() { return inter.size(); }
        bool empty() { return inter.empty(); }
        void clear() { inter.clear(); }

        template< class Iterator > int getKeys( Iterator );

        // nieobowiazkowe, moze byc nieobslugiwane przez niektore kontenery
        int capacity () { return inter.capacity(); }
        void reserve(int n) { inter.reserve(n); }
        AssocTable(int n): cont(), inter( cont ) { inter.reserve(n); }

    private:
        AssocTabInterface< T > inter;
};

template< class T > AssocTable< T > assocTab( const T & );

/* ------------------------------------------------------------------------- *
 * AssocTabInterface< T >
 *
 *
 * ------------------------------------------------------------------------- */

template< class T > class AssocTabInterface< AssocTable< T > >
{
    public:
        AssocTabInterface( AssocTable< T > &acont ): cont( acont ) { }

        typedef typename AssocTable< T >::KeyType KeyType;
        typedef typename AssocTable< T >::ValType ValType;

        bool hasKey( KeyType arg ) { return cont.hasKey( arg ); }
        bool delKey( KeyType arg ) { return cont.delKey( arg ); }
        KeyType firstKey() { return cont.firstKey(); }
        KeyType lastKey() { return cont.lastKey(); }
        KeyType prevKey( KeyType arg ) { return cont.prevKey( arg ); }
        KeyType nextKey( KeyType arg ) { return cont.nextKey( arg ); }
        ValType &operator[]( KeyType arg ) { return cont[arg]; }
        unsigned size() { return cont.size(); }
        bool empty() { return cont.empty(); }
        void clear() { cont.clear(); }

        template< class Iterator > int getKeys( Iterator );
        // nieobowiazkowe, moze byc nieobslugiwane przez niektore kontenery
        int capacity () { return cont.capacity(); }
        void reserve(int n) { cont.reserve(n); }

        AssocTable< T > &cont;
};

/* ------------------------------------------------------------------------- *
 * BlockList< Element,Container >
 *
 *
 * ------------------------------------------------------------------------- */

template< class Element > struct BlockOfBlockList
{
    public:
        Element blob;
        int prev, next;
} ;

template< class Element, class Container = std::vector< BlockOfBlockList< Element > > >
class BlockList
{
    protected:
        int siz, first, last, ffree;
        Container cont;

    public:
        typedef Element ElementType;
        typedef BlockOfBlockList< Element > BlockType;

        BlockList( int = 0 );
        BlockList( const BlockList< Element,Container > & );
        BlockList< Element,Container > &operator=(
            const BlockList< Element,Container > & );

        bool ready( int );
        void clear();
        int size() { return siz; }
        bool empty() { return !siz; }
        void reserve( int arg ) { cont.reserve( arg ); }
        bool full() { return ffree == -1; }
        int capacity() { return cont.capacity(); }
        int firstPos()  { return first; }
        int lastPos()   { return last; }
        int nextPos( int );
        int prevPos( int );
        Element &operator[]( int i ) { return cont[i].blob; }
        int newPos( int = -1 );
        void delPos( int );
        void defrag();
        bool test();
};

/* ------------------------------------------------------------------------- *
 * AssocContBase
 *
 *
 * ------------------------------------------------------------------------- */

struct AssocContReg;

class AssocContBase
{
    public:
        virtual void DelPosCommand( int ) = 0;
        virtual AssocContReg &getReg( int ) = 0;
} ;

/* ------------------------------------------------------------------------- *
 * AssocContReg
 *
 *
 * ------------------------------------------------------------------------- */

struct AssocContReg
{
    int nextPos;
    AssocContBase* next;
};

/* ------------------------------------------------------------------------- *
 * AssocKeyContReg
 *
 *
 * ------------------------------------------------------------------------- */

class AssocKeyContReg: public AssocContReg
{
    public:
        AssocKeyContReg() { next = 0; }
        AssocKeyContReg( const AssocKeyContReg & ) { next = 0; }
        AssocKeyContReg &operator=( const AssocKeyContReg & );

        AssocContReg *find( AssocContBase *cont );
        ~AssocKeyContReg();
} ;

template< class Klucz, class Elem > struct BlockOfAssocArray
{
    Elem val;
    Klucz key;
    AssocContReg assocReg;
} ;

template< class Klucz > class KluczTest
{
    public:
        KluczTest( Klucz v = 0 ) { AssocKeyContReg *ptr = &v->assocReg; }
} ;

/* ------------------------------------------------------------------------- *
 * AssocArray
 *
 *
 * ------------------------------------------------------------------------- */

template< class Klucz, class Elem, class Container = std::vector< BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > > >
class AssocArray: public AssocContBase, public KluczTest< Klucz >
{
    protected:
        BlockList< BlockOfAssocArray< Klucz,Elem >,std::vector< BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > > > tab;

        virtual void DelPosCommand( int pos ) { tab.delPos( pos ); }
        virtual AssocContReg &getReg( int pos ) { return tab[pos].assocReg; }

    public:
        typedef Klucz KeyType;
        typedef Elem ValType;
        typedef BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > BlockType;

        AssocArray( int asize = 0 ): tab( asize ) { }
        AssocArray( const AssocArray< Klucz,Elem,Container > & );

        AssocArray< Klucz,Elem,Container > &operator=(
            const AssocArray< Klucz,Elem,Container > & );

        int size() { return tab.size(); }
        bool empty() { return tab.empty(); }
        void reserve( int arg ) { tab.reserve( arg ); }
        int capacity() { return tab.capacity(); }
        bool hasKey( Klucz v ) { return keyPos( v ) != -1; }
        int keyPos( Klucz );
        bool delKey( Klucz );
        Klucz firstKey();
        Klucz lastKey();
        Klucz nextKey( Klucz );
        Klucz prevKey( Klucz );
        Elem &operator[]( Klucz );
        void defrag();
        void clear();

        template< class Iterator > int getKeys( Iterator );

        ~AssocArray() { clear(); }
} ;

/* ------------------------------------------------------------------------- *
 * AssocTabInterface< AssocArray< K,V,C > >
 *
 *
 * ------------------------------------------------------------------------- */

template< class K, class V, class C > class AssocTabInterface< AssocArray< K,V,C > >
{
    public:
        AssocTabInterface( AssocArray< K,V,C > &acont ): cont( acont ) { }

        typedef K KeyType;
        typedef V ValType;

        bool hasKey( KeyType arg ) { return cont.hasKey( arg ); }
        bool delKey( KeyType arg ) { return cont.delKey( arg ); }
        KeyType firstKey() { return cont.firstKey(); }
        KeyType lastKey() { return cont.lastKey(); }
        KeyType prevKey( KeyType arg ) { return cont.prevKey( arg ); }
        KeyType nextKey( KeyType arg ) { return cont.nextKey( arg ); }
        ValType &operator[]( KeyType arg ) { return cont[arg]; }
        unsigned size() { return cont.size(); }
        bool empty() { return cont.empty(); }
        void clear() { cont.clear(); }

        template< class Iterator > int getKeys( Iterator );

        int capacity () { return cont.capacity(); }
        void reserve(int n) { cont.reserve(n); }

        AssocArray< K,V,C > &cont;
};

struct DummyVal {};

template< class Klucz, class Container >
void sum(
    AssocArray< Klucz,DummyVal,Container > &,
    AssocArray< Klucz,DummyVal,Container > &,
    AssocArray< Klucz,DummyVal,Container > & );

template< class Klucz, class Container >
void diff(
    AssocArray< Klucz,DummyVal,Container > &,
    AssocArray< Klucz,DummyVal,Container > &,
    AssocArray< Klucz,DummyVal,Container > & );

enum AssocMatrixType { AMatrFull, AMatrNoDiag, AMatrTriangle, AMatrClTriangle };
template< AssocMatrixType > class AssocMatrixAddr;

/* ------------------------------------------------------------------------- *
 * AssocMatrixAddr< A >
 *
 *
 * ------------------------------------------------------------------------- */

template <> class AssocMatrixAddr< AMatrFull >
{
    public:
        static int bufLen( int n ) { return n * n; }
        int wsp2pos( std::pair< int,int > );
        std::pair< int,int > pos2wsp( int );

        template< class T > bool correctPos( T, T ) { return true; }
        template< class Klucz > std::pair< Klucz,Klucz > key( Klucz, Klucz );
        template< class Klucz > std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > );
} ;

template <> class AssocMatrixAddr< AMatrNoDiag >
{
    public:
        static int bufLen( int n ) { return n * (n - 1); }
        int wsp2pos( std::pair< int,int > );
        std::pair< int,int > pos2wsp( int );
        template< class T > bool correctPos( T u, T v ) { return u != v; }
        template< class Klucz > std::pair< Klucz,Klucz > key( Klucz, Klucz );
        template< class Klucz > std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz> );
} ;

template <> class AssocMatrixAddr< AMatrClTriangle >
{
    public:
        static int bufLen( int n )  { return n * (n + 1) / 2; }
        int wsp2pos( std::pair< int,int > );
        std::pair< int,int > pos2wsp( int );
        template< class T > bool correctPos( T, T ) { return true; }
        template< class Klucz > std::pair< Klucz,Klucz > key( Klucz, Klucz );
        template< class Klucz > std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > );
} ;


template <> class AssocMatrixAddr< AMatrTriangle >
{
    public:
        static int bufLen( int n ) { return n * (n - 1) / 2; }
        int wsp2pos( std::pair< int,int > );
        std::pair< int,int > pos2wsp( int );
        template< class T > bool correctPos( T u, T v ) { return u != v; }
        template< class Klucz > std::pair< Klucz,Klucz > key( Klucz, Klucz );
        template< class Klucz > std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > );
} ;

template< class Elem > struct BlockOfAssocMatrix
{
    Elem val;
    int next, prev;
    bool present() { return next || prev; }
    BlockOfAssocMatrix(): next( 0 ), prev( 0 ), val() { }
} ;

/* ------------------------------------------------------------------------- *
 * AssocMatrix
 *
 *
 * ------------------------------------------------------------------------- */

template< class Klucz, class Elem, AssocMatrixType aType,
          class Container = std::vector< BlockOfAssocMatrix< Elem > >,
          class IndexContainer = std::vector< BlockOfBlockList< BlockOfAssocArray< Klucz,int > > > >
class AssocMatrix: public AssocMatrixAddr< aType >
{
    private:
        class AssocIndex: public AssocArray< Klucz,int,IndexContainer >
        {
            public:
                AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > *owner;

                AssocIndex( int = 0 );
                int klucz2pos( Klucz );
                Klucz pos2klucz( int );
                virtual void DelPosCommand( int );

                friend class AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >;
        } index;

        friend class AssocIndex;

        Container bufor;
        int siz, first, last;

        void delPos( std::pair< int,int >  );

    protected:
        struct DefragMatrixPom
        {
            Klucz u, v;
            Elem val;
        } ;

    public:
        typedef Klucz KeyType;
        typedef Elem ValType;
        typedef BlockOfAssocMatrix< Elem > BlockType;
        typedef BlockOfBlockList< BlockOfAssocArray< Klucz,int > > IndexBlockType;

        AssocMatrix( int = 0 );
        AssocMatrix( const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > & );
        AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > &operator=(
            const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > & );

        bool hasInd( Klucz v ) { return index.hasKey( v ); }
        bool delInd( Klucz );
        Klucz firstInd() { return index.firstKey(); }
        Klucz lastInd() { return index.lastKey(); }
        Klucz nextInd( Klucz v ) { return index.nextKey( v ); }
        Klucz prevInd( Klucz v ) { return index.prevKey( v ); }
        int indSize() { return index.size(); }

        template< class Elem2, class ExtCont > int slice1(
            Klucz, AssocArray< Klucz,Elem2,ExtCont > & );
        template< class Elem2, class ExtCont > int slice2(
            Klucz, AssocArray< Klucz,Elem2,ExtCont > & );

        bool hasKey( Klucz, Klucz );
        bool hasKey( std::pair< Klucz,Klucz > k ) { return hasKey( k.first,k.second ); }

        bool delKey( Klucz, Klucz );
        bool delKey( std::pair< Klucz,Klucz > k ) { return delKey( k.first,k.second ); }

        Elem &operator()( Klucz, Klucz );
        Elem &operator()( std::pair< Klucz,Klucz > k ) { return operator()( k.first,k.second ); }

        Elem* presentValPtr( Klucz, Klucz );
        std::pair< Klucz,Klucz > firstKey();
        std::pair< Klucz,Klucz > lastKey();
        std::pair< Klucz,Klucz > nextKey( Klucz, Klucz );
        std::pair< Klucz,Klucz > nextKey( std::pair< Klucz,Klucz > );

        std::pair< Klucz,Klucz > prevKey( Klucz, Klucz );
        std::pair< Klucz,Klucz > prevKey( std::pair< Klucz,Klucz > );

        int size() { return siz; }
        bool empty() { return !siz; }
        void clear();
        void reserve( int );

        void defrag();

        template< class Iterator > int getKeys( Iterator );
};

/* ------------------------------------------------------------------------- *
 * AssocInserter
 *
 *
 * ------------------------------------------------------------------------- */

template< class T >
class AssocInserter: public std::iterator< std::output_iterator_tag,void,void,void,void >
{
    protected:
        T* container;
        AssocTabInterface< T > test;

    public:
        typedef T container_type;
        AssocInserter( T &x ): container( &x ), test( x ) { }
        template< class K, class V > AssocInserter< T > &operator=(
            const std::pair< K,V > & );
        AssocInserter< T > &operator*() { return *this; }
        AssocInserter< T > &operator++() { return *this; }
        AssocInserter< T > operator++( int ) { return *this; }
};

template< class T >
AssocInserter< T > assocInserter( T &x ) { return AssocInserter< T >( x ); }

/* ------------------------------------------------------------------------- *
 * AssocFunktorMatrix
 *
 *
 * ------------------------------------------------------------------------- */

template< class T, class Fun >
class AssocFunktorInserter: public std::iterator< std::output_iterator_tag,void,void,void,void >
{
    protected:
        T* container;
        Fun funktor;
        Koala::AssocTabInterface< T > test;

    public:
        typedef T container_type;
        typedef Fun FunktorType;
        AssocFunktorInserter( T &x, Fun f ): container( &x ), funktor( f ), test( x ) { }
        template< class K > AssocFunktorInserter< T,Fun > &operator=( const K & );
        AssocFunktorInserter< T,Fun > &operator*() { return *this; }
        AssocFunktorInserter< T,Fun > &operator++() { return *this; }
        AssocFunktorInserter< T,Fun > operator++( int ) { return *this; }
} ;

template< class T, class F > AssocFunktorInserter< T,F > assocInserter( T &, F );

template< typename T > std::ostream &operator<<(
    std::ostream &out, AssocTabInterface< T > cont );

#include "assoctab.hpp"

}

#endif
