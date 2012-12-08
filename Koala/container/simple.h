#ifndef KOALA_SIMPLE_STRUCTS_H
#define KOALA_SIMPLE_STRUCTS_H

/* simple.h
 *
 *Proste interfejsy udajace kontenery stosu, vectora, kolejki FIFO i kolejki priorytetowej z elementami typu T
 * Interfejs wzorowany na STLu
 *Dzialaja dostarczonej w konstruktorze na zewnetrznej tablicy gotowych elementow typu T o podanym rozmiarze
 *(np. tablica statyczna, dynamiczna new lub LOCALARRAY). Ew. przekraczanie zakresow sygnalizowane wyjatkiem.
 * Interfejsy sa obiektami niekopiowalnymi.
 *Przydatne wewnatrz procedur
 */

#include <limits>
#include <cassert>
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>

#include "../base/exception.h"

namespace Koala
{

    // tworzy pare z min. i max. podanych elementow
    template< class T > std::pair< T,T > pairMinMax( T a, T b );
    template< class T > std::pair< T,T > pairMinMax( std::pair< T,T > arg )
        { return pairMinMax( arg.first,arg.second ); }

    /* StackInterface
     *
     */
    template< class Container > class StackInterface;
    template< class T > class StackInterface< T * >
    {
        T *buf;
        int siz,maxsize;

        StackInterface( const StackInterface< T * > & ) { }
        StackInterface< T * > &operator=( const StackInterface< T * > & ) { return *this; }

      public:
        typedef T ElemType;

        StackInterface( T *bufor, int max ): buf( bufor ), siz( 0 ), maxsize( max )  { }
        int size() const { return siz; }
        bool empty() const { return siz == 0; }
        bool operator!() const { return empty(); }
        void push( const T& val );
        void pop();
        T &top();
        void clear() { siz = 0; }

        template< class InputIterator > void assign( InputIterator first, InputIterator last );
    };

    /* QueueInterface
     *
     */
    template< class Container > class QueueInterface;
    template< class T > class QueueInterface< T * >
    {
        T *buf;
        int beg,end,maxsize;

        QueueInterface( const QueueInterface< T * > & ) { }
        QueueInterface< T * > &operator=( const QueueInterface< T * > & ) { return *this; }

        int prev( int x ) const { return (x) ? x - 1 : maxsize; }
        int next( int x ) const { return (x == maxsize) ? 0 : x + 1; }

      public:
        typedef T ElemType;

        // Uwaga: wymaga elementu nadmiarowego tj. bufora o jeden dluzszego, niz maksymalny rozmiar
        QueueInterface( T *bufor, int max ): buf( bufor ), beg( 0 ), end( 0 ), maxsize( max ) { }

        int size() const { return (beg <= end) ? end - beg : maxsize + 1 - (beg - end); }
        bool empty() const { return beg == end; }
        bool operator!() const { return empty(); }
        void push( const T& val );
        void pop();
        T &front();
        T &top();
        T &back();
        void clear() { beg = end = 0; }

        template< class InputIterator > void assign( InputIterator first, InputIterator last );
    };

    /* VectorInterface
     *
     */
    template< class Container > class VectorInterface;
    template< class T > class VectorInterface< T * >
    {
        T *start,*limit;
        int siz;

        VectorInterface( const VectorInterface< T * > & ) { }
        VectorInterface< T * > &operator=( const VectorInterface< T * > & ) { return *this; }

      public:
        typedef T value_type;

        VectorInterface( T *bufor, int max ): start( bufor ), limit( bufor + max ), siz( 0 ) { }

        T *begin() { return start; }
        T *end() { return start + siz; }
        int size() const { return siz; }
        int max_size() const { return limit - start; }
        int capacity() const { return limit - start; }
        bool empty() const { return siz == 0; }
        bool operator!() const { return empty(); }
        void reserve( int arg ) { koalaAssert( arg <= max_size(),ContExcFull ); }
        void resize( int arg );

        T &operator[]( int pos ) { return at( pos ); }
        T &at( int pos );
        T &front() { return at( 0 ); }
        T &back() { return at( siz - 1 ); }

        void push_back( const T &arg );
        void pop_back();
        static void rev( T* f,T *l );
        template< class InputIterator > void assign( InputIterator first, InputIterator last );
        void insert( T *where, int n, const T &x );
        void insert( T *where, const T &x ) { insert( where,1,x ); }
        template< class InputIterator > void insert( T *where, InputIterator first, InputIterator last );
        void erase( T *f, T *l );
        void erase( T* f ) { erase( f,f + 1 ); }
        void clear() { siz = 0; }
        void clearBuf( T val = T() ) { for( T *i = start; i < limit; i++ ) *i = val; }
    };

    /* PriQueueInterface
     *
     */
    template< class Container, class Comp > class PriQueueInterface;
    template< class T, class Comp > class PriQueueInterface< T *,Comp >
    {
        T *buf;
        int siz,maxsize;
        Comp comp;

        PriQueueInterface( const PriQueueInterface< T *,Comp > & ) { }
        const PriQueueInterface< T *,Comp > &operator=( const PriQueueInterface< T *,Comp > & ) { return *this; }

      public:
        typedef T ElemType;

        PriQueueInterface( T *bufor, int max, Comp acomp = Comp() ):
            buf( bufor ), siz( 0 ), maxsize( max ),  comp( acomp ) { }
        PriQueueInterface( T *bufor, T *end, int max, Comp acomp = Comp() );
        int size() const { return siz; }
        bool empty() const { return siz == 0; }
        bool operator!() const { return empty(); }
        void push( const T &val );
        void pop();
        T top() const;
        void clear() { siz = 0; }
        template< class InputIterator > void assign( InputIterator first, InputIterator last );
    };

#include "simple.hpp"
}

#endif
