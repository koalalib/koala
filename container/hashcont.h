#ifndef KOALA__HASHCONTAINERS__H__
#define KOALA__HASHCONTAINERS__H__

/* hashcont.h
 *
 */

#include <stddef.h>
#include <new>
#include <string>
#include <utility>
#include <iterator>
#include <cassert>
#include <limits>

#include "../base/exception.h"

#if !defined(__GNUC__) && !defined(__INTEL_COMPILER)
typedef unsigned __int64    uint64_t;
typedef unsigned __int32    uint32_t;
#endif
#if defined(__INTEL_COMPILER) || defined(__GNUC__)
#include <stdint.h>
#endif

namespace Koala
{

#ifdef HASHSETDEBUG
#define COLLISION()     collisions++;
#else
#define COLLISION()
#endif

// domyslny rozmiar map asocjacyjnych (jesli w konstruktorze pominiemy rozmiar)
#ifndef HASHMAPDEFAULTSIZE
#define HASHMAPDEFAULTSIZE 1021
#endif

#define HASHSETEMPTYPTR     ((void *)0)
#define HASHSETNONEXTPTR    ((void *)1)
#define HASHSETSENTRYPTR    ((void *)2)
#define HASHSETVALIDPTR     ((void *)3)

    template< class T, class H, class A > class HashSet;
    template< class T, class H, class A > class HashSet_const_iterator;

    /*
    * HashDefaultCPPAllocator
    */
    class HashDefaultCPPAllocator
    {
      public:
        template< class T > T *allocate() { return new T(); }
        template< class T > T *allocate( size_t n ) { return new T[n]; }
        template< class T > void deallocate( T *p ) { delete p; }
        template< class T > void deallocate( T *p, size_t ) { delete[] p; }
    };

    namespace Privates {

        /*
        * HSNode
        */
        template< class KeyType > struct HSNode
        {
            KeyType key;
            HSNode *next;
        };

        /*
         * HashSetTableList
         */
        template< class KeyType > class HashSetTableList
        {
          public:
            HashSetTableList< KeyType > *next;
            size_t size;
            HSNode< KeyType > array[];
        };
    }

    /*
    * HashSet_const_iterator
    */
    template< class KeyType, class HashFunction, class Allocator > class HashSet_const_iterator:
        public std::forward_iterator_tag
    {
      public:
        typedef std::forward_iterator_tag iterator_category;
        typedef KeyType value_type;
        typedef ptrdiff_t difference_type;
        typedef KeyType *pointer;
        typedef KeyType &reference;

        HashSet_const_iterator() { }
        HashSet_const_iterator( const HashSet_const_iterator &it) { *this = it; }
        ~HashSet_const_iterator() { }

        KeyType &operator*() { return m_cur->key; }
        KeyType *operator->() { return &(m_cur->key); }

        HashSet_const_iterator &operator++();
        HashSet_const_iterator operator++(int);
        HashSet_const_iterator &operator--();
        HashSet_const_iterator operator--(int);
        HashSet_const_iterator &operator=( const HashSet_const_iterator &it );

        bool operator==( const HashSet_const_iterator &it ) { return m_slot == it.m_slot && m_cur == it.m_cur; }
        bool operator!=( const HashSet_const_iterator &it ) { return m_slot != it.m_slot || m_cur != it.m_cur; }

      private:
        HashSet_const_iterator( Privates::HSNode< KeyType > *slot );
        HashSet_const_iterator( Privates::HSNode< KeyType > *slot, bool ) { m_slot = m_cur = slot; }
        HashSet_const_iterator( Privates::HSNode< KeyType > *slot, Privates::HSNode< KeyType > *cur );

        void advance_if_needed() { if (m_cur->next == HASHSETEMPTYPTR) advance(); }
        void advance();
        void recede();

        Privates::HSNode< KeyType > *m_slot,*m_cur;
        friend class HashSet< KeyType,HashFunction,Allocator >;
    };

    /*
    * default hash functions
    */
    template< class KeyType > class DefaultHashFunction { };

    namespace Privates {
        /*
        * Int32Hash
        *  default hash for int and pointer types
        */
        template< class KeyType > class Int32Hash
        {
          public:
            // explicit pointer truncation in 64bit
            size_t operator()( KeyType key, size_t m ) const
                { return ((uint64_t)(uint32_t)(((uint32_t)(size_t)key) * 2654435769u) * m) >> 32; }
        };

        /*
         * FloatHash
         *  default hash for floating point types
         */
        template<class KeyType>
        class FloatHash {
          public:
            size_t operator ()(KeyType key, size_t m) const {
                // explicit pointer truncation in 64bit
                uint64_t iv = key * 2654435769u;
                iv = iv ^ (iv >> 32);
                return ((uint64_t)((uint32_t)iv) * m) >> 32;
                };
            };

        /*
        * StringHash
        *  default hash for strings
        */
        class StringHash
        {
        public:
            size_t operator()( const std::string &key, size_t m ) const;
        };

        /*
        * CStringHash
        *  default hash for char *, const char *, wchar_t *, const wchar_t *
        */
        template< class CTYPE > class CStringHash
        {
        public:
            size_t operator()( const CTYPE *key, size_t m ) const;
        };
    }

    template<> class DefaultHashFunction< int >: public Privates::Int32Hash< int > { };
    template<> class DefaultHashFunction< long >: public Privates::Int32Hash< long > { };
    template<> class DefaultHashFunction< short >: public Privates::Int32Hash< short > { };
    template<> class DefaultHashFunction< unsigned int >: public Privates::Int32Hash< unsigned int > { };
    template<> class DefaultHashFunction< unsigned long >: public Privates::Int32Hash< unsigned long >  { };
    template<> class DefaultHashFunction< unsigned short >: public Privates::Int32Hash< unsigned short > { };
    template< class T > class DefaultHashFunction< T * >: public Privates::Int32Hash< T * > { };

    template<> class DefaultHashFunction<double>: public Privates::FloatHash<double> { };

    template<> class DefaultHashFunction< char * >: public Privates::CStringHash< char > { };
    template<> class DefaultHashFunction< const char * >: public Privates::CStringHash< char > { };
    template<> class DefaultHashFunction< wchar_t * >: public Privates::CStringHash< wchar_t > { };
    template<> class DefaultHashFunction< const wchar_t * >: public Privates::CStringHash< wchar_t > { };
    template<> class DefaultHashFunction< std::string >: public Privates::StringHash { };

    /*
    * HashSet
    * KeyType - type of element
    * HashFunction - hash object
    *  -> size_t operator()(const KeyType &key, size_t m) const
    *     or size_t operator()(KeyType key, size_t m) const
    */
    template< class KeyType, class HashFunction = DefaultHashFunction< KeyType >, class Allocator = HashDefaultCPPAllocator >
        class HashSet
    {
      public:
        static const size_t node_size = sizeof( Privates::HSNode< KeyType > );

        typedef KeyType value_type;
        typedef KeyType key_type;
        typedef KeyType &reference;
        typedef const KeyType &const_reference;
        typedef KeyType *pointer;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        typedef HashSet_const_iterator< KeyType,HashFunction,Allocator > iterator;
        typedef HashSet_const_iterator< KeyType,HashFunction,Allocator > const_iterator;

      public:
        HashSet(): m_count( 0 ), m_size( 0 ), m_resizeFactor( 205 ) { initialize( 8 ); }
        HashSet( size_t size ): m_count( 0 ), m_size( 0 ), m_resizeFactor( 205 ) { initialize( size ); }
        HashSet( const HashSet &t );
        template< class HF, class Alloc > HashSet( const HashSet< KeyType,HF,Alloc > &t );

        ~HashSet() { free( true ); }

        HashSet &operator=( const HashSet &t );

        template< class SetType > HashSet &operator=( const SetType &t );

        iterator begin() { return iterator( m_table ); }
        iterator end() { return iterator( m_table + m_size,true ); }
        const_iterator begin() const { return const_iterator( m_table ); }
        const_iterator end() const { return const_iterator( m_table + m_size,true ); }

        size_t size() const { return m_count; }
        size_t capacity() const { return (size_t)(-1); }
        size_t slots() const { return m_size; };

        bool empty() const { return m_count == 0; }

        iterator find( const KeyType &key ) { return Find( key ); }
        const_iterator find( const KeyType &key ) const { return const_iterator( Find( key ) ); }
        std::pair< iterator,bool > insert( const KeyType &key ) { return find_or_insert( key ); }

        void resize( size_t size );
        void reserve( size_t size ) { if (size > m_size) this->resize( size ); }
        void clear();
        void swap( HashSet &other );
        std::pair< iterator,bool > find_or_insert( const KeyType &key );
        bool contains( const KeyType &key ) const;
        void erase( const KeyType &key );
        void erase( const_iterator pos ) { erase( pos.m_cur->key ); }

        void set_threshold( double value ) { if( value <= 0 ) m_resizeFactor = value <= 0 ? 0 : value * 256 + 0.5; };
        double get_threshold() { return (double)m_resizeFactor / 256.0; };

      private:
        Privates::HSNode< KeyType > *make_overflow_node();

        void initialize( size_t size );
        void free( bool deleteFirst );
        void eraseAll();
        iterator Find( const KeyType &key ) const;

        Privates::HashSetTableList< KeyType > *CreateTable( size_t size );
        bool EnlargeIfNeeded();

#ifdef HASHSETDEBUG
      public:
        mutable size_t collisions;
#endif

      private:
        Privates::HSNode< KeyType > *m_table;
        size_t m_count;
        size_t m_size;
        size_t m_resizeFactor;  // fill rate in range 0-256, 0 = off
        Privates::HSNode< KeyType > *m_firstFree;
        Privates::HashSetTableList< KeyType > *m_tables;
        size_t m_overflowFirst;
        size_t m_overflowSize;
        HashFunction hashfn;
        Allocator allocate;
    };

    template< class KeyType, class ValueType, class HashFunction, class Allocator > class HashMap;
    template< class KeyType, class ValueType, class HashFunction, class Allocator > class BiDiHashMap;
    template< class KeyType, class ValueType, class HashFunction, class Allocator > class BiDiHashMap_const_iterator;

    namespace Privates
    {
        template< class KeyType, class ValueType > class HashMapPair
        {
          public:
            KeyType first;
            ValueType second;

            HashMapPair(): first(), second() { }
            HashMapPair( const HashMapPair &p ): first( p.first ), second( p.second ) { }
            HashMapPair( const KeyType &f, const ValueType &s ): first( f ), second( s ) { }
            HashMapPair &operator=( const HashMapPair &p );

            bool operator==( const HashMapPair &other ) const { return first == other.first; }
            bool operator!=( const HashMapPair &other) const { return first != other.first; }
        };

        template< class HashFn, class K, class V > class HashMapHashWrapper
        {
          public:
            size_t operator()( const HashMapPair< K,V > &key, size_t m ) const { return hashfn( key.first,m ); }
          private:
            HashFn hashfn;
        };

        /*
        * SetToMap adapter
        */
        template< class KeyType, class ValueType, class PairType, class SetType > class SetToMap: public SetType
        {
          public:
            typedef KeyType key_type;
            typedef ValueType data_type;
            typedef PairType value_type;
            typedef typename SetType::iterator iterator;
            typedef typename SetType::const_iterator const_iterator;

            SetToMap(): SetType(), m_defaultValue() { }
            SetToMap( ValueType defVal ): SetType(), m_defaultValue( defVal ) { }
            SetToMap( const SetToMap &m ): SetType( (const SetType &)m ), m_defaultValue( m.m_defaultValue ) { }

            ~SetToMap() { }

            SetToMap &operator=( const SetToMap &t );
            ValueType &operator[]( const KeyType &key );
            const ValueType &operator[]( const KeyType &key ) const;

            std::pair< typename SetType::iterator,bool > insert( const KeyType &key, const ValueType &value )
                { return SetType::insert( PairType( key,value ) ); }
            std::pair< typename SetType::iterator,bool > insert( const std::pair< KeyType,ValueType > &p )
                { return SetType::insert( PairType( p.first,p.second ) ); }

            typename SetType::iterator find( const KeyType &key ) { return SetType::find( PairType( key,m_defaultValue ) ); }
            typename SetType::const_iterator find( const KeyType &key ) const { return SetType::find( PairType( key,m_defaultValue ) ); }

            void erase( const KeyType &k ) { SetType::erase( find( k ) ); }
            void erase( const_iterator it ) { SetType::erase( find( it->first ) ); }
            void swap( SetToMap &other );

          protected:
            ValueType m_defaultValue;
        };
    }

    /*
    * HashMap
    */
    template< typename KeyType, typename ValueType, class HashFunction = DefaultHashFunction< KeyType >,
        class Allocator = HashDefaultCPPAllocator > class HashMap: public
        Privates::SetToMap< KeyType,ValueType,Privates::HashMapPair< KeyType,ValueType >,
            HashSet< Privates::HashMapPair< KeyType,ValueType >,
            Privates::HashMapHashWrapper< HashFunction,KeyType,ValueType >,Allocator > >
    {
      private:
        typedef Privates::SetToMap< KeyType,ValueType,Privates::HashMapPair< KeyType,ValueType >,
            HashSet< Privates::HashMapPair< KeyType,ValueType >,
            Privates::HashMapHashWrapper< HashFunction,KeyType,ValueType >,Allocator> > baseType;
      public:
        HashMap( size_t size = HASHMAPDEFAULTSIZE ): baseType() { baseType::resize( size ); }
        HashMap( size_t size, const ValueType &defVal ): baseType( defVal ) { baseType::resize( size ); }
        HashMap( const HashMap &t ): baseType( (const baseType &)t ) { }

        ~HashMap() { }
    };

    namespace Privates
    {
        /*
        * BiDiHashMapPair
        */
        template< class KeyType, class ValueType > class BiDiHashMapPair
        {
          public:
            KeyType first;
            ValueType second;
            mutable const BiDiHashMapPair *prev;
            mutable const BiDiHashMapPair *next;

          public:
            BiDiHashMapPair(): first(), second(), prev( NULL ), next( NULL ) { }
            BiDiHashMapPair( const KeyType &k, const ValueType &v ): first( k ), second( v ), prev( NULL ), next( NULL ) { }
            BiDiHashMapPair( const BiDiHashMapPair &k ):
                first( k.first ), second( k.second ), prev( k.prev ), next( k.next ) { }

            BiDiHashMapPair &operator=( const BiDiHashMapPair &k );

            bool operator==( const BiDiHashMapPair &other ) const { return first == other.first; }
            bool operator!=( const BiDiHashMapPair &other ) const { return first != other.first; }
        };

        template< class HashFn, class KeyType, class ValueType > class BiDiHashMapHashWrapper
        {
          public:
            size_t operator()( const BiDiHashMapPair< KeyType,ValueType > &key, size_t m ) const
                { return hashfn( key.first,m ); }
          private:
            HashFn hashfn;
        };
    }

    /*
    * BiDiHashMap_const_iterator
    */
    template< class KeyType, class ValueType, class HashFunction, class Allocator > class BiDiHashMap_const_iterator:
        public std::bidirectional_iterator_tag
    {
      private:
        typedef Privates::SetToMap< KeyType,ValueType,Privates::BiDiHashMapPair< KeyType,ValueType >,
            HashSet< Privates::BiDiHashMapPair< KeyType,ValueType >,
            Privates::BiDiHashMapHashWrapper< HashFunction,KeyType,ValueType >,Allocator > > mapBase;
      public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef Privates::BiDiHashMapPair< KeyType,ValueType > value_type;
        typedef ptrdiff_t difference_type;
        typedef const Privates::BiDiHashMapPair< KeyType,ValueType > *pointer;
        typedef Privates::BiDiHashMapPair< KeyType,ValueType > &reference;

        BiDiHashMap_const_iterator() { }
        BiDiHashMap_const_iterator( const BiDiHashMap_const_iterator &it ) { *this = it; }
        BiDiHashMap_const_iterator( typename mapBase::const_iterator &it ) { m_cur = &(*it); }

        ~BiDiHashMap_const_iterator() { }

        reference operator*() { return *m_cur; }
        pointer operator->() { return m_cur; }

        BiDiHashMap_const_iterator &operator++();
        BiDiHashMap_const_iterator operator++( int );
        BiDiHashMap_const_iterator &operator--();
        BiDiHashMap_const_iterator operator--( int );

        BiDiHashMap_const_iterator &operator=( const BiDiHashMap_const_iterator &it );
        BiDiHashMap_const_iterator &operator=( typename mapBase::const_iterator &it );

        bool operator==( const BiDiHashMap_const_iterator &it) { return m_cur == it.m_cur; }
        bool operator!=( const BiDiHashMap_const_iterator &it ) { return m_cur != it.m_cur; }

      private:
        BiDiHashMap_const_iterator( const Privates::BiDiHashMapPair< KeyType,ValueType > *elem ) { m_cur = elem; }

        const Privates::BiDiHashMapPair< KeyType,ValueType > *m_cur;
        friend class BiDiHashMap< KeyType,ValueType,HashFunction,Allocator >;
    };

    /*
    * BiDiHashMap
    */
    template< typename KeyType, typename ValueType, class HashFunction = DefaultHashFunction< KeyType >,
        class Allocator = HashDefaultCPPAllocator > class BiDiHashMap: public
        Privates::SetToMap< KeyType,ValueType,Privates::BiDiHashMapPair< KeyType,ValueType >,
        HashSet< Privates::BiDiHashMapPair< KeyType,ValueType >,
        Privates::BiDiHashMapHashWrapper< HashFunction,KeyType,ValueType >,Allocator > >
    {
      private:
        typedef Privates::SetToMap< KeyType,ValueType,Privates::BiDiHashMapPair< KeyType,ValueType >,
            HashSet< Privates::BiDiHashMapPair< KeyType,ValueType >,
            Privates::BiDiHashMapHashWrapper< HashFunction,KeyType,ValueType >,Allocator > > baseType;

      public:
        typedef BiDiHashMap_const_iterator< KeyType,ValueType,HashFunction,Allocator > iterator;
        typedef BiDiHashMap_const_iterator< KeyType,ValueType,HashFunction,Allocator > const_iterator;

        BiDiHashMap( size_t size = HASHMAPDEFAULTSIZE );
        BiDiHashMap( size_t size, const ValueType &defVal );
        BiDiHashMap( const BiDiHashMap &t );

        ~BiDiHashMap() { };

        BiDiHashMap &operator=( const BiDiHashMap &t );

        template< class MapType > BiDiHashMap &operator=( const MapType &t );

        iterator begin() { return iterator( m_begin.next ); }
        iterator end() { return iterator( &m_end ); }
        const_iterator begin() const { return const_iterator( m_begin.next ); }
        const_iterator end() const { return const_iterator( &m_end ); }

        ValueType &operator[]( const KeyType &key );

        const ValueType &operator[]( const KeyType &key ) const;

        std::pair< iterator,bool > insert( const KeyType &key, const ValueType &value );
        std::pair< iterator,bool > insert( const std::pair< KeyType,ValueType > &elem );
        const_iterator find( const KeyType &key ) const;

        void clear();
        void erase( const KeyType &key ) { erase( find( key ) ); }
        void erase( const_iterator pos );
        void resize( size_t size );
        void reserve( size_t size ) { if (size > this->slots()) this->resize( size ); }
        void swap( BiDiHashMap &other );

        void set_threshold( double value ) { if( value <= 0 ) m_resizeFactor = value <= 0 ? 0 : value * 256 + 0.5; };
        double get_threshold() { return (double)m_resizeFactor / 256.0; };

      private:
        void initialize();
        void AddToList( const Privates::BiDiHashMapPair< KeyType,ValueType > *ptr );
        void DelFromList( const Privates::BiDiHashMapPair< KeyType,ValueType > *ptr );
        void EnlargeIfNeeded();

      private:
        Privates::BiDiHashMapPair< KeyType,ValueType > m_begin,m_end;
        size_t m_resizeFactor;
    };

    template< class T > class AssocTabConstInterface;
    namespace Privates
    {
        template< class T > class AssocTabTag;
    }

    template< class K, class V > class AssocTabConstInterface< BiDiHashMap< K,V > >: public Privates::AssocTabTag< K >
    {
      public:
        AssocTabConstInterface( const BiDiHashMap< K,V > &acont ): cont( acont ) { }

        typedef K KeyType;
        typedef V ValType;

        typedef BiDiHashMap< K,V > OriginalType;

        bool hasKey( K arg ) const { return cont.find( arg ) != cont.end(); }

        K firstKey() const;
        K lastKey() const;
        K prevKey( K )const ;
        K nextKey( K )const ;

        V operator[]( K arg );
        unsigned size() const { return cont.size(); }
        bool empty() const { return this->size() == 0; }
        template< class Iterator > int getKeys( Iterator ) const;
        int capacity () const { return std::numeric_limits< int >::max(); }

        const BiDiHashMap< K,V > &cont;

      protected:
        BiDiHashMap< K,V > &_cont() { return const_cast< BiDiHashMap< K,V > & >( cont ); }
        void reserve( int n ) { _cont().reserve(n); }
        void clear() { _cont().clear(); }
        bool delKey( K );
        ValType *valPtr( K arg );
        V &get( K arg ) { return (_cont())[arg]; }
    };

    template< class K, class V > class AssocTabConstInterface< HashMap< K,V > >: public Privates::AssocTabTag< K >
    {
      public:
        AssocTabConstInterface( HashMap< K,V > &acont ): cont( acont ) { }

        typedef K KeyType;
        typedef V ValType;

        typedef HashMap< K,V > OriginalType;

        bool hasKey( K arg ) const { return cont.find( arg ) != cont.end(); }

        K firstKey() const;
        K lastKey() const;
        K prevKey( K ) const;
        K nextKey( K ) const;

        V operator[]( K arg );
        unsigned size() const { return cont.size(); }
        bool empty() const { return this->size() == 0; }
        int capacity () const { return std::numeric_limits< int >::max(); }
        template< class Iterator > int getKeys( Iterator ) const;

        const HashMap< K,V > &cont;

      protected:
        HashMap< K,V > &_cont() { return const_cast< HashMap< K,V > & >( cont ); }
        ValType *valPtr( K arg );
        void reserve( int n ) { _cont().reserve( n ); }
        bool delKey( K );
        void clear() { _cont().clear(); }
        V &get( K arg ) { return (_cont())[arg]; }
    };

#include "hashcont.hpp"
}

#endif
