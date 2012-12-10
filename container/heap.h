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
	/** \brief Binominal heal node.
	 *
	 *  An auxiliary object representing single key (node) of binominal heap.
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
		/**\brief Constructor*/
		BinomHeapNode( const Key &key = Key() ): parent( 0 ), child( 0 ), next( 0 ), degree( 0 ), key( key )
			{ }
		/**\brief Get key.*/
		Key get()
			{ return key; }
	};

	/* BinomHeap
	 *  Kolejka 2-mianowa dla kluczy typu Key porownywanych podanym kompartorem typu Compare. Domyslnie
	 *  obiekty dla kluczy sa wewnatrz alokoawne przez new i delete. Mozna jednak uzyc wlasnego alokatora
	 *  typu Allocator podajac w konstruktorze wskaznik na alokator (wartosc domyslna 0 - rowniez new i delete).
	 *  Zewnetrzny alokator musi zyc przez caly czas istnienia kolejki a jego wskaznika w kontenerze nie mozna
	 *  zmienic.
	 */
	/** \brief Binominal heap.
	 *
	 *  Standard binominal heap structure.
	 *  \tparam Key the class of stored objects.
	 *  \tparam Compre the comparator, the class allowing to compare two objects of Key type, by default std::less<Key>.
	 *  \tparam Allocator the class allows to use own memory allocator.
	 *  \ingroup cont
	 *
	 *  [See example](examples/heap/example_BinomHeap.html).
	 */
	template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
		class BinomHeap
	{
	public:
		typedef BinomHeapNode< Key > Node;/**\brief Node of heap. */
		typedef BinomHeapNode< Key > *Repr;/**\brief Pointer to heap node. */

	protected:
		Node *root,*minimum;
		unsigned nodes;
		Compare function;
		Allocator* allocator;

	public:
		// podajemy komparator, alokacja new/delete???
		/** \brief Constructor.*/
		inline BinomHeap( const Compare &function = Compare() ):
			root( 0 ), minimum( 0 ), nodes( 0 ), function( function ), allocator( 0 )
				{ }
		// podajemy komparator, kolejka powiazana z alokatorem zewnetrznym
		/** \brief Constructor.
		*
		*   The costructor allows to use external memory.
		*   \param all memory buffer.
		*   \param function the comparison functor should define strict weak ordering on set of keys. */
		inline BinomHeap( Allocator *all, const Compare &function = Compare() ):
			root( 0 ), minimum( 0 ), nodes( 0 ), function( function ), allocator( all )
				{ }
		// konstruktor kopiujacy, obiekt wiaze sie z tym samym alokatorem, co oryginal
		/** \brief Copy constructor.*/
		inline BinomHeap( const BinomHeap< Key,Compare,Allocator > & );
		/** \brief Copy content operator.*/
		BinomHeap &operator=( const BinomHeap &X );
		~BinomHeap()
			{ clear(); }

		// metody STLowego kopca maja to samo znaczenie
		// najmniejszy klucz
		/** \brief Get top key.
		 *
		 *  The method gets the top key of the heap. If default std::less functor is used the method gets the minimum key.
		 *  \return the top key.*/
		Key top() const;
		// i jego reprezentant
		/** \brief Get top node.
		 *
		 *  The method gets the top heap node. If default std::less functor is used the method gets one with the minimum key.
		 *  \return the top key.*/
		 Node* topRepr() const
			{ return minimum; }

		// wstawienie klucza, zwracany wskaznik to jego reprezentant wewnatrz kolejki
		/**\brief Insert key.
		 *
		 * The method insertes \a key on heap.
		 * \return the reference to the new-created node for a key.*/
		Node* push( const Key &key );
		// usun najmniejszy
		/** \brief Remove top element.
		 *
		 *  The method removes the top element from the heap.*/
		void pop();

		 // zmniejszenie klucza danego wezla, podanie wiekszej wartosci rzuca blad
		/** \brief Decrease top element.
		 *
		 *  The method decreses the key of the node \a A to \a key. The new key needs to be smaller than the previous one, if not an exception is thrown.
		 *  \param A the modified node
		 *  \param key the new key.*/
		void decrease( Node *A, const Key &key );
		// usun wezel z kluczem
		/** \brief Delete node.
		 *
		 *  The node A is deleted from heap.
		 *  \param the deleted node.*/
		void del( Node *A );

		// dopisuje klucze z drugiej kolejki, ktora jest czyszczona
		/** \brief Merge heaps.
		 *
		 *  The keys from \a heap are moved to the current heap. All the keys from \a heap are deleted.
		 *  \param A the moved heap.*/
		void merge( BinomHeap & );
		/** \brief Clear heap.*/
		void clear();

		/** \brief Assign heap content.
		 *
		 *  The method clears the container and assigns new content from container defined by the iterators \a beg and \a end.
		 *  \param beg the iterator to the first element of the container with new content.
		 *  \param end the iterator to the past-the-end element of the container with new content. */
		template< class InputIterator > void assign( InputIterator beg, InputIterator end );

		/** \brief Number of nodes.*/
		unsigned size() const
			{ return nodes; }

		/** \brief Test if empty.
		 *
		 *  The method only checks if there is no elements in the heap. The content remains intact.
		 *  \return true if heap is empty, false if there is as least one element in heap. */
		bool empty() const
			{ return root == 0; }


	protected:
		Node* copy( Node*,Node* );

		inline Node* join( Node*,Node* );
		inline Node* reverse( Node* );
		inline Node* cut( Node* );
		void clear( Node* );

		Node *newNode( Key key );
		void delNode( Node *node );

		// TODO: rozwazyc usuniecie w finalnej wersji biblioteki
		bool check() const;

		template< class InputIterator > void assign2( InputIterator &first, int len );
	};

	/** FibonHeapNode DOCUMENT
	 *  Kopiec Fibonacziego, komentarze te same co w 2-mianowym
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
		FibonHeapNode( const Key &_key = Key() )
			{ init( _key ); }
	};

	/** FibonHeap
	 *
	 *
	 *  [See example](examples/heap/example_FibonHeap.html).
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
			root( 0 ), nodes( 0 ), function( function ), allocator( 0 )
				{ }
		inline FibonHeap( Allocator *all, const Compare &function = Compare() ):
			root( 0 ), nodes( 0 ), function( function ), allocator( all )
				{ }
		inline FibonHeap( const FibonHeap< Key,Compare,Allocator > & );
		FibonHeap& operator=( const FibonHeap< Key,Compare,Allocator > &X );
		~FibonHeap()
			{ clear(); }

		Key top() const;
		Node *topRepr() const
			{ return root; }
		Node *push( const Key & );
		void pop();

		void decrease( Node *, const Key & );
		void del( Node * );

		void merge( FibonHeap & );
		void clear();

		template< class InputIterator > void assign( InputIterator first, InputIterator last );

		unsigned size() const { return nodes; }
		bool empty() const { return !root; }

	protected:
		Node* copy( Node *, Node * );
		// TODO: rozwazyc usuniecie w finalnej wersji biblioteki
		bool check() const;
	};



	/** PairHeapNode DOCUMENT
	 *  Kopiec paruj¹cy, komentarze te same co w 2-mianowym
	 */
	template <class Key>
	class PairHeapNode
	{
		template< class K, class Comp, class Alloc > friend class PairHeap;

		PairHeapNode< Key > *parent,*child,*previous,*next;
		unsigned degree;
		Key key;

		inline void insert( PairHeapNode< Key > * );
		inline void remove();
		bool check() const;
		void init( const Key & =Key() );

	public:
		Key get()
			{ return key; }
		PairHeapNode( const Key &_key = Key() )
			{ init( _key ); }
	};

	/** PairHeap
	 *
	 *  [See example](examples/heap/example_FibonHeap.html).
	 */
	template <class Key, class Compare = std::less<Key>,class Allocator=Privates::DefaultCPPAllocator >
	class PairHeap
	{
	public:
		typedef PairHeapNode<Key> Node;
		typedef PairHeapNode<Key> * Repr;

	protected:
		Node *root;
		unsigned nodes;
		Compare function;
		Allocator* allocator;

		Node* newNode( Key key );
		void delNode( Node *node );
		void clear( Node * );
	public:
		inline PairHeap( const Compare &function = Compare() ):
			root( 0 ), nodes( 0 ), function( function ), allocator( 0 )
				{ }
		inline PairHeap( Allocator *all, const Compare &function = Compare() ):
			root( 0 ), nodes( 0 ), function( function ), allocator( all )
				{ }
		inline PairHeap( const PairHeap< Key,Compare,Allocator > & );
		PairHeap& operator=( const PairHeap< Key,Compare,Allocator > &X );
		~PairHeap()
			{ clear(); }

		Key top() const;
		Node *topRepr() const
			{ return root; }
		Node *push( const Key & );
		void pop();

		void decrease( Node *, const Key & );
		void del( Node * );

		void merge( PairHeap & );
		void clear();

		template< class InputIterator > void assign( InputIterator first, InputIterator last );

		unsigned size() const
			{ return nodes; }
		bool empty() const
			{ return !root; }

	protected:
		Node* copy( Node *, Node * );
		// TODO: rozwazyc usuniecie w finalnej wersji biblioteki
		bool check() const;
	};


#include "heap.hpp"
}

#endif
