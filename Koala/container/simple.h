#ifndef KOALA_SIMPLE_STRUCTS_H
#define KOALA_SIMPLE_STRUCTS_H

/* simple.h
 *
 *Proste interfejsy udajace kontenery stosu, vectora, kolejki FIFO i kolejki priorytetowej z elementami typu T
 * Interfejs wzorowany na STLu
 *Dzialaja dostarczonej w konstruktorze na zewnetrznej tablicy gotowych elementow typu T o podanym rozmiarze
 *(np. tablica statyczna, dynamiczna new lub LOCALARRAY). Ew. przekraczanie zakresow testowane assertami.
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
	/** \brief Make sorted pair.*/
	template< class T > std::pair< T,T > pairMinMax( T a, T b );
	/** \brief Make sorted pair.*/
	template< class T > std::pair< T,T > pairMinMax( std::pair< T,T > arg )
		{ return pairMinMax( arg.first,arg.second ); }

    //NEW:
	template< class T > std::pair< T,T > pairMaxMin( T a, T b );
	/** \brief Make sorted pair.*/
	template< class T > std::pair< T,T > pairMaxMin( std::pair< T,T > arg )
		{ return pairMaxMin( arg.first,arg.second ); }


    //NEW: wylecial VectorInterface

	/* StackInterface
	 *
	 */
	template< class Container > class StackInterface;
	/** \brief Stack
	 *
	 *  Simple stack structure based on a table. The interface is similar to the one in STL.
	 *  \tparam T the type of stored variables.
	 *  \ingroup DMsimplecont*/
	template< class T > class StackInterface< T * >
	{
		T *buf; /*<\brief Pointer to the underlying table.*/
		int siz,maxsize;

		StackInterface( const StackInterface< T * > & ) { }
		StackInterface< T * > &operator=( const StackInterface< T * > & ) { return *this; }

	public:
		typedef T ElemType;/**<\brief Type of stored element.*/

		/** \brief Constructor.
		 *
		 *  \param bufor the pointer to the underlying table.
		 *  \param max the maximal size of stack (the size of underlying table).*/
		StackInterface( T *bufor, int max ): buf( bufor ), siz( 0 ), maxsize( max )  { }
		/**\brief Get number of elements.
		 *
		 *  \return the number of elements in the container.*/
		int size() const { return siz; }

		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool empty() const { return siz == 0; }
		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool operator!() const { return empty(); }

		/** \brief Push
		 *
		 *  The method inserts the value \a val on the top of stack.
		 *  \param val the inserted value.*/
		void push( const T& val );

		/** \brief Pop
		 *
		 *  The method takes of the top element from the stack.*/
		void pop();
		/** \brief Access top.
		 *
		 *  \returns the reference to the top element on the stack. */
		T &top();

		/** \brief Clear stack.*/
		void clear() { siz = 0; }

		/** \brief Assign new content.
		 *
		 *  The method clears the stack and  inserts values of elements form the container represented by iterator \a first and \a last.
		 *  \param first the iterator to the first element of the assigned container.
		 *  \param last the iterator to the past-the-end element of the assigned container.*/
		template< class InputIterator > void assign( InputIterator first, InputIterator last );
	};

	/* QueueInterface
	 *
	 */
	template< class Container > class QueueInterface;
	/** \brief Queue.
	 *
	 *  Simple queue structure based on a table. The interface is similar to the one in STL.
	 *  \tparam T the type of stored variables.
	 *  \ingroup DMsimplecont*/
	template< class T > class QueueInterface< T * >
	{
		T *buf;
		int beg,end,maxsize;

		QueueInterface( const QueueInterface< T * > & ) { }
		QueueInterface< T * > &operator=( const QueueInterface< T * > & ) { return *this; }

		int prev( int x ) { return (x) ? x - 1 : maxsize; }
		int next( int x ) { return (x == maxsize) ? 0 : x + 1; }

	public:
		typedef T ElemType;/**<\brief Type of stored element.*/

		// Uwaga: wymaga elementu nadmiarowego tj. bufora o jeden dluzszego, niz maksymalny rozmiar
		/** \brief Constructor.
		 *
		 *  \param bufor the pointer to the underlying table.
		 *  \param max the maximal size of stack (should have one extra element).*/
		QueueInterface( T *bufor, int max ): buf( bufor ), beg( 0 ), end( 0 ), maxsize( max ) { }

		/**\brief Get number of elements.
		 *
		 *  \return the actual number of elements in the container.*/
		int size() const { return (beg <= end) ? end - beg : maxsize + 1 - (beg - end); }
		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool empty() const { return beg == end; }
		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool operator!() const { return empty(); }
		/** \brief Push
		 *
		 *  The method inserts the value \a val on the end of queue.
		 *  \param val the inserted value.*/
		void push( const T& val );
		/** \brief Pop
		 *
		 *  The method takes of the top (first) element from the queue.*/
		void pop();
		/**\brief Access first element.
		 *
		 *  \return the reference to the first element in the queue.*/
		T &front();
		/**\brief Access first element.
		 *
		 *  \return the reference to the first element in the queue.*/
		T &top();
		/**\brief Access last element.
		 *
		 *  \return the reference to the last element in the queue.*/
		T &back();
		/** \brief Clear stack.*/
        //NEW:
		void pushFront( const T& val );
        //NEW:
		void popBack();

		void clear() { beg = end = 0; }

		/** \brief Assign new content.
		 *
		 *  The method clears the queue and inserts values of elements form the container represented by iterator \a first and \a last.
		 *  \param first the iterator to the first element of the assigned container.
		 *  \param last the iterator to the past-the-end element of the assigned container.*/
		template< class InputIterator > void assign( InputIterator first, InputIterator last );
	};


	/* PriQueueInterface
	 *
	 */
	template< class Container, class Comp > class PriQueueInterface;
	/** \brief Priority queue.
	 *
	 *  Priority queue structure based on a table (heap). The interface is similar to the one in STL.
	 *  \tparam T the type of stored variables.
	 *  \ingroup DMsimplecont*/
	template< class T, class Comp > class PriQueueInterface< T *,Comp >
	{
		T *buf;
		int siz,maxsize;
		Comp comp;

		PriQueueInterface( const PriQueueInterface< T *,Comp > & ) { }
		const PriQueueInterface< T *,Comp > &operator=( const PriQueueInterface< T *,Comp > & ) { return *this; }

	public:
		typedef T ElemType;/**<\brief Type of stored element.*/

		/** \brief Constructor.
		 *
		 *  \param bufor the pointer to the underlying table.
		 *  \param max the maximal size of priority queue limited by the size of buffer.
		 *  \param acomp the comparison function object, should be strict weak ordering*/
		PriQueueInterface( T *bufor, int max, Comp acomp = Comp() ):
			buf( bufor ), siz( 0 ), maxsize( max ),  comp( acomp ) { }

		/** \brief Constructor.
		 *
		 *  The method initializes the heap and rearranges (in heap order) the elements between iterators (pointers) \a bufor and \a end.
		 *  \param bufor the pointer to the underlying table.
		 *  \param end the pointer to the past-the-end element of the container with elements to initialize queue.
		 *  \param max the maximal size of priority queue limited by the size of buffer.
		 *  \param acomp the comparison function object, should be strict weak ordering*/
		PriQueueInterface( T *bufor, T *end, int max, Comp acomp = Comp() );

		/**\brief Get number of elements.
		 *
		 *  \return the actual number of elements in the container.*/
		int size() const { return siz; }
		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool empty() const { return siz == 0; }
		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool operator!() const { return empty(); }
		/** \brief Insert element.
		 *
		 *  \param val the inserted value.*/
		void push( const T &val );
		/** \brief Delete top element.*/
		void pop();
		/** \brief Get top value.*/
		T top() const;
		/** \brief Clear priority queue.*/
		void clear() { siz = 0; }
		/** \brief Assign new content.
		 *
		 *  The method clears the queue and inserts the values of elements from the container represented by iterator \a first and \a last.
		 *  \param first the iterator to the first element of the assigned container.
		 *  \param last the iterator to the past-the-end element of the assigned container.*/
		template< class InputIterator > void assign( InputIterator first, InputIterator last );
	};



    //NEW: wspolna tablicowa pula pamieci dla lokalnych kontenerow, ktore posluguja sie wieloma alokowanymi
    //skladnikami tj. LocalGraph (wierzcholki/krawedzie), List, Heaps.
    template <class Elem> class SimplArrPool {
    private:

        struct Block {
            Elem elem;
            int next;
        };

        char* buf;
        int siz,used,first;

        Block* blocks() const
        {
            return (Block*) buf;
        }

        SimplArrPool(const SimplArrPool &) {}
        SimplArrPool& operator=(const SimplArrPool &) {}

    public:

        // Flagi: czy rzucac wyjatek przy przepelnieniu (albo zwracac NULL)
        // Czy rzucac wyjatek przy zamykaniu puli, jesli nie wszystko zostalo zdealokowane
        bool throwIfFull, throwIfNotEmpty;

        SimplArrPool(int n) : siz(n), used(0), first(0), throwIfFull(true), throwIfNotEmpty(true)
        {
            buf = new char[n*sizeof(Block)];
            for(int i=0;i<siz-1;i++) blocks()[i].next=i+1;
            if (n) blocks()[siz-1].next=-1;
        }

        ~SimplArrPool()
        {
//            if (!(used==0 || !throwIfNotEmpty)) std::cout << "\nErr!: " << this->size() << ' ' << this->used << "\n";
            koalaAssert(used==0 || !throwIfNotEmpty,ContExcPoolNotEmpty);
            if (used)
                for(int i=0;i<siz;i++) if (blocks()[i].next==-2) blocks()[i].elem.~Elem();
            delete [] buf;
        }

        int size() const
        {
            return siz;
        }

        // Liczba zaalokowanych elementow
        int busy() const
        {
            return used;
        }
        // zwraca adres z puli na uzytek placement new (nie tworzy obiektu!)
        void* alloc()
        {
            koalaAssert(used<siz || !throwIfFull,ContExcFull);
            if (used==siz) return 0;
            used++;
            Block* ptr=blocks()+first;
            first=ptr->next;
            ptr->next=-2;
            return &(ptr->elem);
        }
        // dealokuje obiekt z wczesniej przydzielonego adresu
        void dealloc(Elem* wsk)
        {
            char* chwsk=(char*) wsk;
            koalaAssert(chwsk>=buf && chwsk<buf+siz*sizeof(Block),ContExcWrongArg);
            int pos=(chwsk-buf)/sizeof(Block);
            koalaAssert((chwsk==(char*)(&blocks()[pos].elem) && blocks()[pos].next==-2),ContExcWrongArg);
            blocks()[pos].next=first;
            first=pos;
            used--;
            blocks()[pos].elem.~Elem();
        }

    };


#include "simple.hpp"
}

#endif
