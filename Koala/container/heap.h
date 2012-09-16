#ifndef KOALA_BINOMIALHEAP_H
#define KOALA_BINOMIALHEAP_H

/* heap.h
 * 
 */

#include <cassert>
#include <functional>

#include "localarray.h"
#include "privates.h"

//Kontenery kolejek priorytetowych szybko zlaczalnych dla porownywalnych typow kluczy: dwumianowej i Fibonacziego.
//
//TODO: rozwazyc zamiane we wszystkich algorytmach PriQueueInterface -> dwumianowa (mozliwe usprawnienie algorytmow
//dzieki brakujacej w STL operacji usuwania klucza)

namespace Koala
{

    /* BinomHeapNode
     * Obiekt pomocniczy kolejki 2-mianowej przechowujacy pojedynczy klucz. Klucz wstawiony do kolejki dostaje
     * reprezentanta bedacego wskaznikiem na obiekt tego typu. Nie uzywany bezposrednio przez uzytkownika.
     * Reprezentant nie zmienia sie przez caly czas istnienia kolejki, bez wzgledu na jej ew. zmiany
     */
    template< class Key > class BinomHeapNode
    {   
        template< class K, class Comp, class Alloc > friend class BinomHeap;
        BinomHeapNode< Key > *parent, *child, *next;
        unsigned degree;
        Key key;

        inline void insert( BinomHeapNode< Key > * );
        bool check() const;

      public:

        BinomHeapNode( const Key &key = Key() ): parent( 0 ), child( 0 ), next( 0 ), degree( 0 ), key( key ) { }
        Key get() { return key; }
    };

    /* BinomHeap
     *  Kolejka 2-mianowa dla kluczy typu Key porownywanych podanym kompartorem typu Compare. Domyslnie
     *  obiekty dla kluczy sa wewnatrz alokoawne przez new i delete. Mozna jednak uzyc wlasnego alokatora
     *  typu Allocator podajac w konstruktorze wskaznik na alokator (wartosc domyslna 0 - rowniez new i delete).
     *  Zewnetrzny alokator musi zyc przez caly czas istnienia kolejki a jego wskaznika w kontenerze nie mozna
     *  zmienic.
     */
    template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
        class BinomHeap
    {
      public:
        typedef BinomHeapNode< Key > Node;
        typedef BinomHeapNode< Key > *Repr;

      protected:
        Node *root,*minimum;
        unsigned nodes;
        Compare function;
        Allocator* allocator;
        
      public:
        // podajemy komparator, alokacja new/delete
        inline BinomHeap( const Compare &function = Compare() ):
            root( 0 ), minimum( 0 ), nodes( 0 ), allocator( 0 ), function( function ) { }
        // podajemy komparator, kolejka powiazana z alokatorem zewnetrznym
        inline BinomHeap( Allocator *all, const Compare &function = Compare() ):
            root( 0 ), minimum( 0 ), nodes( 0 ), allocator( all ), function( function ) { }
        // konstruktor kopiujacy, obiekt wiaze sie z tym samym alokatorem, co oryginal
        inline BinomHeap( const BinomHeap< Key,Compare,Allocator > & ); 
        BinomHeap &operator=( const BinomHeap &X );
        ~BinomHeap() { clear(); }

        // metody STLowego kopca maja to samo znaczenie
        // najmniejszy klucz
        Key top() const; 
        // i jego reprezentant
        Node* topRepr() const { return minimum; }

        // wstawienie klucza, zwracany wskaznik to jego reprezentant wewnatrz kolejki
        Node* push( const Key & );
        // usun najmniejszy
        void pop(); 

         // zmniejszenie klucza danego wezla, podanie wiekszej wartosci rzuca blad
        void decrease( Node *, const Key & );
        // usun wezel z kluczem
        void del( Node * ); 

        // dopisuje klucze z drugiej kolejki, ktora jest czyszczona
        void merge( BinomHeap & ); 
        void clear();

        template< class InputIterator > void assign( InputIterator first, InputIterator last );

        unsigned size() const { return nodes; }

        bool empty() const { return root == 0; }

        bool check() const; // TODO: rozwazyc usuniecie w finalnej wersji biblioteki

      protected:
        Node* copy( Node*,Node* );

        inline Node* join( Node*,Node* );
        inline Node* reverse( Node* );
        inline Node* cut( Node* );
        void clear( Node* );

        Node *newNode( Key key );
        void delNode( Node *node );
        
        template< class InputIterator > void assign2( InputIterator &first, int len );
    };

    /* FibonHeapNode
     * Kopiec Fibonacziego, komentarze te same co w 2-mianowym
     */
    template< class Key > class FibonHeapNode
    {   
        template< class K, class Comp, class Alloc > friend class FibonHeap;

        FibonHeapNode< Key > *parent,*child,*previous,*next;
        unsigned flag;
        Key key;
        
        inline void insert( FibonHeapNode< Key > * );
        inline void remove();
        bool check() const;
        void init( const Key & =Key() );

      public:
        Key get() { return key; }
        FibonHeapNode( const Key &_key = Key() ) { init( _key ); }
    };

    /* FibonHeap
     * 
     */
    template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
        class FibonHeap
    {   
      public:
        typedef FibonHeapNode< Key > Node;
        typedef FibonHeapNode< Key > *Repr;

      private:
        Node *root;
        unsigned nodes;
        Compare function;
        Allocator* allocator;

        Node* newNode( Key key );
        void delNode( Node *node );
        void clear( Node * );

      public:
        inline FibonHeap( const Compare &function = Compare() ):
            root( 0 ), nodes( 0 ), allocator( 0 ), function( function ) { }
        inline FibonHeap( Allocator *all, const Compare &function = Compare() ):
            root( 0 ), nodes( 0 ), allocator( all ), function( function ) { }
        inline FibonHeap( const FibonHeap< Key,Compare,Allocator > & );
        FibonHeap& operator=( const FibonHeap< Key,Compare,Allocator > &X );    
        ~FibonHeap() { clear(); }

        Key top() const;
        Node *topRepr() const { return root; }
        Node *push( const Key & );
        void pop();

        void decrease( Node *, const Key & );
        void del( Node * );

        void merge( FibonHeap & );
        void clear();
        
        template< class InputIterator > void assign( InputIterator first, InputIterator last );

        unsigned size() const { return nodes; }
        bool empty() const { return !root; }
        bool check() const;
        
      protected:
        Node* copy( Node *, Node * );
    };

#include "heap.hpp"
}

#endif
