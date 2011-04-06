#ifndef KOALA_JOINABLE_SETS_H
#define KOALA_JOINABLE_SETS_H

#include <map>
#include <cassert>
#include "set.h"
#include "localarray.h"

namespace Koala {

template< class Klucz > struct JSPartDesrc
{
    JSPartDesrc *parent, *next, *first, *last;
    unsigned int deg, size;
    Klucz key;
} ;

template< class ITEM, class AssocContainer = AssocArray< ITEM,JSPartDesrc< ITEM > * > >
class JoinableSets
{
    protected:
        AssocContainer mapa;
        JSPartDesrc< ITEM > *bufor;
        size_t siz, part_no, maxsize;

    public:
        typedef JSPartDesrc< ITEM > *Repr;
        typedef ITEM ElemType;

        JoinableSets( unsigned int = 0 );
        JoinableSets( const JoinableSets< ITEM,AssocContainer > & );
        JoinableSets &operator=( const JoinableSets< ITEM,AssocContainer > & );
        ~JoinableSets() { resize( 0 ); }

        void resize( unsigned int );

        int size() { return siz; }
        bool empty() { return siz == 0; }
        bool operator!() { return empty(); }
        void clear() { resize( 0 ); }
        int getSetNo() { return part_no; }
        template< class Iter > int getElements( Iter );

        template< class Iter > int getSetIds( Iter );
        template< class Iter > int getSet( typename JoinableSets< ITEM >::Repr, Iter );
        template< class Iter > int getSet( const ITEM &, Iter );

        int size( typename JoinableSets< ITEM >::Repr );
        int size( const ITEM & );

        typename JoinableSets< ITEM >::Repr makeSinglet( const ITEM & );
        typename JoinableSets<ITEM>::Repr getSetId( const ITEM & );
        typename JoinableSets<ITEM>::Repr getSetId(typename JoinableSets< ITEM >::Repr );
        typename JoinableSets<ITEM>::Repr join(
            typename JoinableSets< ITEM >::Repr, typename JoinableSets< ITEM >::Repr );
        typename JoinableSets< ITEM >::Repr join( const ITEM &, const ITEM & );
        typename JoinableSets< ITEM >::Repr join(
            typename JoinableSets< ITEM >::Repr, const ITEM & );
        typename JoinableSets< ITEM >::Repr join(
            const ITEM &, typename JoinableSets< ITEM >::Repr );
} ;

template< typename Element, typename Cont >
std::ostream &operator<<( std::ostream &, JoinableSets< Element,Cont > & );

#include "joinsets.hpp"

}

#endif
