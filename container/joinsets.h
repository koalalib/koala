#ifndef KOALA_JOINABLE_SETS_H
#define KOALA_JOINABLE_SETS_H

#include <map>
#include <cassert>
#include "../container/set.h"
#include "../container/localarray.h"
#include "../container/assoctab.h"

namespace Koala {

template< class Klucz > struct JSPartDesrc
{
    JSPartDesrc *parent, *next, *first, *last;
    unsigned int deg, size;
    Klucz key;
} ;


template <class Container> class VectorInterface;


template <class ITEM>
struct JoinableSetsInternalTypes {

    typedef Privates::BlockOfBlockList< BlockOfAssocArray< ITEM,JSPartDesrc< ITEM > * > > AssocBlockType;
    typedef JSPartDesrc< ITEM >  BufElementType;
};


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

        JoinableSets( unsigned int = 0, void* p=0,void* q=0 );
        JoinableSets( const JoinableSets< ITEM,AssocContainer > & );
        JoinableSets &operator=( const JoinableSets< ITEM,AssocContainer > & );
        ~JoinableSets() { resize( 0 ); }

        JoinableSets( unsigned int n, JSPartDesrc< ITEM > * buf,
                      typename JoinableSetsInternalTypes<ITEM>::AssocBlockType * mapbuf ) :
                mapa(n,mapbuf), siz(0), bufor(buf), part_no(0), maxsize(n)
        { }

        void resize( unsigned int );

        int size() const { return siz; }
        bool empty() const { return siz == 0; }
        bool operator!() const { return empty(); }
        void clear() { resize( 0 ); }
        int getSetNo() const { return part_no; }
        template< class Iter > int getElements( Iter ) const;

        template< class Iter > int getSetIds( Iter ) const;
        template< class Iter > int getSet( typename JoinableSets< ITEM >::Repr, Iter ) const;
        template< class Iter > int getSet( const ITEM &, Iter ) const;

        int size( typename JoinableSets< ITEM >::Repr ) const;
        int size( const ITEM & ) const;

        typename JoinableSets< ITEM >::Repr makeSinglet( const ITEM & );
        typename JoinableSets<ITEM>::Repr getSetId( const ITEM & ) const;
        typename JoinableSets<ITEM>::Repr getSetId(typename JoinableSets< ITEM >::Repr ) const;
        typename JoinableSets<ITEM>::Repr join(
            typename JoinableSets< ITEM >::Repr, typename JoinableSets< ITEM >::Repr );
        typename JoinableSets< ITEM >::Repr join( const ITEM &, const ITEM & );
        typename JoinableSets< ITEM >::Repr join(
            typename JoinableSets< ITEM >::Repr, const ITEM & );
        typename JoinableSets< ITEM >::Repr join(
            const ITEM &, typename JoinableSets< ITEM >::Repr );
} ;

template< typename Element, typename Cont >
std::ostream &operator<<( std::ostream &,const JoinableSets< Element,Cont > & );


template <class T>
struct JoinableSetsVectIntSwitch
{
    typedef void* BufType;
    typedef void* MapBufType;
    static bool isJSVI() { return false; }
};

template <class ITEM>
struct JoinableSetsVectIntSwitch< JoinableSets<ITEM, AssocArray<ITEM, JSPartDesrc<ITEM>*,
            VectorInterface<Privates::BlockOfBlockList< BlockOfAssocArray< ITEM,JSPartDesrc<ITEM>* > >*> > > >
{
    typedef JSPartDesrc< ITEM > * BufType;
    typedef Privates::BlockOfBlockList< BlockOfAssocArray< ITEM,JSPartDesrc< ITEM > * > >* MapBufType;
    static bool isJSVI() { return true; }
};


#include "../container/joinsets.hpp"

}

#endif
