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

template< class Container > class AssocTabConstInterface;
template< class Container > class AssocTabInterface;



/* ------------------------------------------------------------------------- *
 * AssocTabInterface< M >
 *
 *
 * ------------------------------------------------------------------------- */


template< class K, class V > class AssocTabConstInterface< std::map< K,V > >
{
    public:
        AssocTabConstInterface( const std::map< K,V > &acont ): cont( acont ) {}

        typedef K KeyType;
        typedef V ValType;

        bool hasKey( K arg ) const { return cont.find( arg ) != cont.end(); }

        K firstKey() const;
        K lastKey() const;
        K prevKey( K ) const;
        K nextKey( K ) const;

        V operator[]( K arg ) const
            { typename std::map< K,V >::const_iterator i;
              i=cont.find(arg);
              if (i==cont.end()) return V(); else return i->second;
            }
        unsigned size() const { return cont.size(); }
        bool empty() const { return this->size() == 0; }
        template< class Iterator > int getKeys( Iterator ) const;

        const std::map< K,V > &cont;
        int capacity () const { return std::numeric_limits<int>::max(); }

        protected:

        std::map< K,V >& _cont() { return const_cast<std::map< K,V >&>( cont ); }

        void reserve(int) {  }
        void clear() { _cont().clear(); }
        bool delKey( K );
        ValType* valPtr(K arg)
        {   typename std::map<K,V>::iterator i=_cont().find( arg );
            if (i==_cont().end()) return NULL; else return &(_cont())[arg];
        }
        V &get( K arg ) { return (_cont())[arg]; }
} ;


template <class T> class AssocTabInterface : public AssocTabConstInterface<T>
{   public:

        typedef typename AssocTabConstInterface<T>::KeyType KeyType;
        typedef typename AssocTabConstInterface<T>::ValType ValType;


        T & cont;

        AssocTabInterface( T &acont ): AssocTabConstInterface<T>(acont), cont( acont )  {}

        void reserve(int arg) { AssocTabConstInterface<T>::reserve(arg); }
        void clear() { AssocTabConstInterface<T>::clear(); }
        bool delKey( KeyType arg) { AssocTabConstInterface<T>::delKey(arg); }
        ValType* valPtr(KeyType arg) { return AssocTabConstInterface< T>::valPtr(arg); }
        ValType &operator[]( KeyType arg ) { return AssocTabConstInterface< T>::get(arg); }
        ValType operator[]( KeyType arg ) const { return AssocTabConstInterface< T>::operator[](arg); }
};


//template< class T >
//const AssocTabInterface< T >& assocTabInterf( T &cont )
//{
//    return AssocTabInterface< T >( cont );
//}

template< class T >
AssocTabConstInterface< T > assocTabInterf(const T &cont )
{
    return AssocTabConstInterface< T >( cont );
}



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

        bool hasKey( KeyType arg ) const { return inter.hasKey( arg ); }
        ValType* valPtr(KeyType arg) { return inter.valPtr(arg); }
        bool delKey( KeyType arg ) { return inter.delKey( arg ); }
        KeyType firstKey()  const { return inter.firstKey(); }
        KeyType lastKey()  const { return inter.lastKey(); }
        KeyType prevKey( KeyType arg )  const { return inter.prevKey( arg ); }
        KeyType nextKey( KeyType arg )  const { return inter.nextKey( arg ); }
        ValType &operator[]( KeyType arg) { return inter[arg]; }
        ValType operator[]( KeyType arg) const { return ((AssocTabConstInterface< T >&)inter).operator[](arg); }
        unsigned size()  const { return inter.size(); }
        bool empty()  const { return inter.empty(); }
        void clear() { inter.clear(); }

        template< class Iterator > int getKeys( Iterator iter)  const { return inter.getKeys( iter ); }

        // nieobowiazkowe, moze byc nieobslugiwane przez niektore kontenery
        int capacity ()  const  { return inter.capacity(); }
        void reserve(int n) { inter.reserve(n); }
        AssocTable(int n,void* p=0): cont(), inter( cont ) { inter.reserve(n); }

    private:
        AssocTabInterface< T > inter;
};

template< class T > AssocTable< T > assocTab( const T & cont)
{
    return AssocTable<T>( cont );
}


/* ------------------------------------------------------------------------- *
 * BlockList< Element,Container >
 *
 *
 * ------------------------------------------------------------------------- */

template <class Container> class VectorInterface;

template< class Element > struct BlockOfBlockList
{
    public:
        Element blob;
        int prev, next;
} ;


namespace Privates {

template <class T> class BlockListVectInerfTest {};

template <class T> class BlockListVectInerfTest<VectorInterface<T> >
{   public:
    BlockListVectInerfTest(bool) {}
    private:
    BlockListVectInerfTest(const BlockListVectInerfTest<VectorInterface<T> >&) {}
};

}


template< class Element, class Container = std::vector< BlockOfBlockList< Element > > >
class BlockList : protected Privates::BlockListVectInerfTest<Container>
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

        // tylko dla Container==VectorInterface<BlockOfBlockList< Element > *>
        BlockList(BlockOfBlockList< Element >* wsk, int asize)
        :   cont(wsk,asize) ,siz( 0 ),first( -1 ),last( -1 ),ffree( -1 ),
            Privates::BlockListVectInerfTest<Container>(true)
        { cont.clear(); }

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
        void deregister();
        ~AssocKeyContReg() { deregister(); }
} ;

template< class Klucz, class Elem > struct BlockOfAssocArray
{
    Elem val;
    Klucz key;
    AssocContReg assocReg;
} ;

namespace Privates {

template< class Klucz > class KluczTest
{
    public:
        KluczTest( Klucz v = 0 ) { AssocKeyContReg *ptr = &v->assocReg; }
} ;

}

/* ------------------------------------------------------------------------- *
 * AssocArray
 *
 *
 * ------------------------------------------------------------------------- */


template< class Klucz, class Elem, class Container = std::vector< BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > > >
class AssocArray: public AssocContBase, protected Privates::KluczTest< Klucz >
{
    protected:
        mutable BlockList< BlockOfAssocArray< Klucz,Elem >,Container > tab;

        virtual void DelPosCommand( int pos ) { tab.delPos( pos ); }
        virtual AssocContReg &getReg( int pos ) { return tab[pos].assocReg; }

    public:
        typedef Klucz KeyType;
        typedef Elem ValType;
        typedef BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > BlockType;

        AssocArray( int asize = 0,void* p=0 ): tab( asize ) { }
        AssocArray( const AssocArray< Klucz,Elem,Container > & );

        AssocArray< Klucz,Elem,Container > &operator=(
            const AssocArray< Klucz,Elem,Container > & );

        // tylko dla Container==VectorInterface<BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > >*>
        AssocArray(int asize , BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > >* wsk ): tab(wsk, asize )
        { }

        int size() const { return tab.size(); }
        bool empty() const { return tab.empty(); }
        void reserve( int arg ) { tab.reserve( arg ); }
        int capacity() const { return tab.capacity(); }
        bool hasKey( Klucz v ) const { return keyPos( v ) != -1; }
        Elem* valPtr(Klucz v)
        {   int x=keyPos(v);
            if (x==-1) return NULL; else return &tab[x].val;
        }
        int keyPos( Klucz ) const;
        bool delKey( Klucz );
        Klucz firstKey() const ;
        Klucz lastKey() const ;
        Klucz nextKey( Klucz ) const ;
        Klucz prevKey( Klucz ) const ;
        Elem &operator[]( Klucz );
        Elem operator[]( Klucz ) const;
        void defrag();
        void clear();

        template< class Iterator > int getKeys( Iterator )  const ;

        ~AssocArray() { clear(); }
} ;


template <class T>
struct AssocArrayVectIntSwitch
{
    typedef void* BufType;
    static bool isAAVI() { return false; }
};

template <class K, class E>
struct AssocArrayVectIntSwitch<AssocArray<K,E,VectorInterface<BlockOfBlockList< BlockOfAssocArray< K,E > >*> > >
{
    typedef BlockOfBlockList< BlockOfAssocArray< K,E > >* BufType;
    static bool isAAVI() { return true; }
};




template< class Klucz, class Elem, class AssocCont, class Container = std::vector< BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > > >
class PseudoAssocArray
{
    protected:
        mutable BlockList< BlockOfAssocArray< Klucz,Elem >,Container > tab;
        AssocCont assocTab;

    public:
        typedef Klucz KeyType;
        typedef Elem ValType;
        typedef BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > BlockType;

        PseudoAssocArray( int asize = 0,void* p=0 ): tab( asize ), assocTab(asize) { }

        int size() const { return tab.size(); }
        bool empty() const  { return tab.empty(); }
        void reserve( int arg ) { tab.reserve( arg ); assocTab.reserve(arg); }
        int capacity() const { return tab.capacity(); }
        bool hasKey( Klucz v ) const { return keyPos( v ) != -1; }
        Elem* valPtr(Klucz v)
        {   int x=keyPos(v);
            if (x==-1) return NULL; else return &tab[x].val;
        }
        int keyPos( Klucz ) const ;
        bool delKey( Klucz );
        Klucz firstKey() const ;
        Klucz lastKey() const ;
        Klucz nextKey( Klucz ) const ;
        Klucz prevKey( Klucz ) const ;
        Elem &operator[]( Klucz );
        Elem operator[]( Klucz ) const;
        void defrag();
        void clear() { tab.clear(); assocTab.clear(); }

        template< class Iterator > int getKeys( Iterator ) const ;

        ~PseudoAssocArray() { clear(); }
} ;



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
        inline int wsp2pos( std::pair< int,int > ) const ;
        inline std::pair< int,int > pos2wsp( int )  const ;

        template< class T > bool correctPos( T, T )  const { return true; }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz, Klucz )  const ;
        template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > )  const ;
} ;

template <> class AssocMatrixAddr< AMatrNoDiag >
{
    public:
        static int bufLen( int n ) { return n * (n - 1); }
        inline int wsp2pos( std::pair< int,int > ) const ;
        inline std::pair< int,int > pos2wsp( int ) const ;
        template< class T > bool correctPos( T u, T v )  const  { return u != v; }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz, Klucz ) const ;
        template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz> ) const ;
} ;

template <> class AssocMatrixAddr< AMatrClTriangle >
{
    public:
        static int bufLen( int n )  { return n * (n + 1) / 2; }
        inline int wsp2pos( std::pair< int,int > ) const ;
        inline std::pair< int,int > pos2wsp( int ) const ;
        template< class T > bool correctPos( T, T ) const  { return true; }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz, Klucz ) const ;
        template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > ) const ;
} ;


template <> class AssocMatrixAddr< AMatrTriangle >
{
    public:
        static int bufLen( int n ) { return n * (n - 1) / 2; }
        inline int wsp2pos( std::pair< int,int > ) const ;
        inline std::pair< int,int > pos2wsp( int ) const ;
        template< class T > bool correctPos( T u, T v )  const { return u != v; }
        template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz, Klucz ) const ;
        template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > ) const ;
} ;


template< class Elem > struct BlockOfAssocMatrix
{
    Elem val;
    int next, prev;
    bool present() const { return next || prev; }
    BlockOfAssocMatrix(): next( 0 ), prev( 0 ), val() { }
} ;



/* ------------------------------------------------------------------------- *
 * AssocMatrix
 *
 *
 * ------------------------------------------------------------------------- */


template< class Klucz, class Elem, AssocMatrixType aType,
          class Container = std::vector< BlockOfAssocMatrix< Elem > >,
          class IndexContainer = AssocArray<Klucz,int,std::vector< BlockOfBlockList< BlockOfAssocArray< Klucz,int > > > > >
class AssocMatrix: public AssocMatrixAddr< aType >
{
    private:
        class AssocIndex: public IndexContainer
        {
            public:
                AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > *owner;

                AssocIndex( int = 0 );
                AssocIndex( int, BlockOfBlockList< BlockOfAssocArray< Klucz,int > >*);
                int klucz2pos( Klucz );
                Klucz pos2klucz( int );
                virtual void DelPosCommand( int );

                friend class AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >;
        };

        mutable AssocIndex index;

        friend class AssocIndex;

        mutable Container bufor;
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
//        typedef BlockOfAssocMatrix< Elem > BlockType;
//        typedef BlockOfBlockList< BlockOfAssocArray< Klucz,int > > IndexBlockType;

        AssocMatrix( int = 0,void* p=0, void* q=0 );
        AssocMatrix( const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > & );
        AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > &operator=(
            const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > & );

        AssocMatrix( int asize, BlockOfAssocMatrix< Elem >* contBuf,
                    BlockOfBlockList< BlockOfAssocArray< Klucz,int > >* indBuf ):
            index( asize,indBuf),siz( 0 ),first( -1 ),last( -1 ),
            bufor(contBuf,AssocMatrixAddr< aType >::bufLen( asize ))
        {
            bufor.clear();
            bufor.reserve( AssocMatrixAddr< aType >::bufLen( asize ) );
            index.owner = this;
        }

        bool hasInd( Klucz v ) const { return index.hasKey( v ); }
        bool delInd( Klucz );
        Klucz firstInd() const { return index.firstKey(); }
        Klucz lastInd() const { return index.lastKey(); }
        Klucz nextInd( Klucz v )const  { return index.nextKey( v ); }
        Klucz prevInd( Klucz v ) const { return index.prevKey( v ); }
        int indSize() const { return index.size(); }

        template< class Elem2, class ExtCont > int slice1(Klucz, ExtCont & ) const;
        template< class Elem2, class ExtCont > int slice2(Klucz, ExtCont & ) const;

        bool hasKey( Klucz, Klucz ) const;
        bool hasKey( std::pair< Klucz,Klucz > k ) const { return hasKey( k.first,k.second ); }

        bool delKey( Klucz, Klucz );
        bool delKey( std::pair< Klucz,Klucz > k ) { return delKey( k.first,k.second ); }

        Elem &operator()( Klucz, Klucz );
        Elem &operator()( std::pair< Klucz,Klucz > k ) { return operator()( k.first,k.second ); }
        Elem operator()( Klucz, Klucz ) const ;
        Elem operator()( std::pair< Klucz,Klucz > k ) const { return operator()( k.first,k.second ); }

//        Elem getVal ( Klucz, Klucz ) const;
//        Elem getVal ( std::pair< Klucz,Klucz > k ) const { return getVal( k.first,k.second ); }
//        Elem& getVal ( Klucz a, Klucz b) { return this->operator()( a,b ); }
//        Elem& getVal ( std::pair< Klucz,Klucz > k )  { return this->operator()( k.first,k.second ); }


        Elem* valPtr( Klucz, Klucz );
        std::pair< Klucz,Klucz > firstKey() const ;
        std::pair< Klucz,Klucz > lastKey() const ;
        std::pair< Klucz,Klucz > nextKey( Klucz, Klucz ) const ;
        std::pair< Klucz,Klucz > nextKey( std::pair< Klucz,Klucz > ) const ;

        std::pair< Klucz,Klucz > prevKey( Klucz, Klucz ) const ;
        std::pair< Klucz,Klucz > prevKey( std::pair< Klucz,Klucz > ) const ;

        int size()  const { return siz; }
        bool empty()  const { return !siz; }
        void clear();
        void reserve( int );

        void defrag();

        template< class Iterator > int getKeys( Iterator )  const ;
};


template <class T>
struct AssocMatrixVectIntSwitch
{
    typedef void* BufType;
    typedef void* IndBufType;
    static bool isAMVI() { return false; }
};

template <class K, class E,AssocMatrixType aType>
struct AssocMatrixVectIntSwitch<AssocMatrix<K,E,aType,VectorInterface< BlockOfAssocMatrix< E > *>,
       AssocArray<K,int,VectorInterface< BlockOfBlockList< BlockOfAssocArray< K,int > >* > > > >
{
    typedef BlockOfAssocMatrix< E >* BufType;
    typedef BlockOfBlockList< BlockOfAssocArray< K,int > >* IndBufType;
    static bool isAMVI() { return true; }
};


template<class Klucz, class Elem, AssocMatrixType aType, class C,class IC >
std::ostream &operator<<(std::ostream &out,const AssocMatrix< Klucz,Elem,aType,C,IC > & cont );


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
//        AssocTabInterface< T > test;

    public:
        typedef T container_type;
        AssocInserter( T &x ): container( &x ) /*, test( x )*/ { }
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
//        Koala::AssocTabInterface< T > test;

    public:
        typedef T container_type;
        typedef Fun FunktorType;
        AssocFunktorInserter( T &x, Fun f ): container( &x ), funktor( f )/*, test( x )*/ { }
        template< class K > AssocFunktorInserter< T,Fun > &operator=( const K & );
        AssocFunktorInserter< T,Fun > &operator*() { return *this; }
        AssocFunktorInserter< T,Fun > &operator++() { return *this; }
        AssocFunktorInserter< T,Fun > operator++( int ) { return *this; }
} ;


namespace Privates {
    template <class Cont>
    std::ostream &printAssoc(std::ostream &out, const Cont & cont)
    {
        out << '{';
        int siz = cont.size();
        typename Cont::KeyType key = cont.firstKey();
        for( ; siz; siz-- )
        {
            out << '(' << key << ',' << cont[key] << ')';
            if (key != cont.lastKey())
            {
                key = cont.nextKey( key );
                out << ',';
            }
        }
        out << '}';
        return out;
    }

};

template< class T, class F > AssocFunktorInserter< T,F > assocInserter( T &, F );

template< typename T > std::ostream &operator<<(
    std::ostream &out, const AssocTabConstInterface< T > & cont )
    {   return Privates::printAssoc(out,cont);  }

template< typename T > std::ostream &operator<<(
    std::ostream &out, const AssocTable< T > & cont )
        {   return Privates::printAssoc(out,cont);  }

template< class K, class V,class C > std::ostream &operator<<(
    std::ostream &out, const AssocArray< K,V,C > & cont )
    {   return Privates::printAssoc(out,cont);  }

template< typename K, typename V, typename A, typename C  > std::ostream &operator<<(
    std::ostream &out, const PseudoAssocArray< K,V,A,C > & cont )
    {   return Privates::printAssoc(out,cont);  }


#include "assoctab.hpp"

}

#endif
