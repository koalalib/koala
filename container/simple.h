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

	//TODO: przewidywano interfejsy innych typow, niz tylko dzialajace na buforze tablicowym, stad deklaracja ogolna
	//i specjalizacja klas dla T*. Jednak raczej pozostaniemy przy tablicach. Nalezy zamienic te szablony np. na
	//template <class T> class StackInterface<T> (bez specjalizacji). Wymaga modyfikacji deklaracji we wszystkich modulach
	//biblioteki korzystajacych z tych interfejsow!
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

	/* QueueInterface DOCUMENT
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
		void clear() { beg = end = 0; }

		/** \brief Assign new content.
		 *
		 *  The method clears the queue and inserts values of elements form the container represented by iterator \a first and \a last.
		 *  \param first the iterator to the first element of the assigned container.
		 *  \param last the iterator to the past-the-end element of the assigned container.*/
		template< class InputIterator > void assign( InputIterator first, InputIterator last );
	};

	/* VectorInterface DOCUMENT
	 *
	 */
	template< class Container > class VectorInterface;
	/** \brief Vector interface.
	 *
	 *  Simple vector structure based on a table. The interface is similar to the one in STL.
	 *  \tparam T the type of stored variables.
	 *  \ingroup DMsimplecont*/
	template< class T > class VectorInterface< T * >
	{
		T *start,*limit;
		int siz;

		VectorInterface( const VectorInterface< T * > & ) { }
		VectorInterface< T * > &operator=( const VectorInterface< T * > & ) { return *this; }

	public:
		typedef T value_type;/**<\brief Type of stored element.*/

		/** \brief Constructor.
		 *
		 *  \param bufor the pointer to the underlying table.
		 *  \param max the maximal size of vector limited by the size of buffer (should have one extra element).*/
		VectorInterface( T *bufor, int max ): start( bufor ), limit( bufor + max ), siz( 0 ) { }

		/** \brief Return iterator (pointer) to beginning*/
		T *begin() { return start; }
		/** \brief Return iterator (pointer) to the past-the-end element.*/
		T *end() { return start + siz; }

		/**\brief Get number of elements.
		 *
		 *  \return the actual number of elements in the container.*/
		int size() const { return siz; }

		/**\brief Get maximal number of elements.
		 *
		 *  \return the maximal size of container (limited by the size of buffer table).*/
		int max_size() const { return limit - start; }

		/**\brief Get maximal number of elements.
		 *
		 *  \return the maximal size of container (limited by the size of buffer table).*/
		int capacity() const { return limit - start; }
		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool empty() const { return siz == 0; }
		/** \brief Test if empty.
		 *
		 *  \return true if the container is empty, false otherwise.*/
		bool operator!() const { return empty(); }
		/** \brief Does nothing.
		 *
		 *  Since the maximum size of container is limited by the buffer, the resize method only checks if \a arg is no greater then max_size() and throws the appropriate exception if necessary.  */
		void reserve( int arg ) { koalaAssert( arg <= max_size(),ContExcFull ); }
		/** \brief Change vector size.
		 *
		 *  If  \a arg > max_size() exception if thrown.\n
		 *  If size() < \a arg < max_size() the method expands the container by pushing zeros (empty constructors of T).\n
		 *  If size() >= \a arg the method does nothing.*/
		void resize( int arg );

		/** \brief Access element operator.*/
		T &operator[]( int pos ) { return at( pos ); }
		/** \brief Access element.*/
		T &at( int pos );
		/** \brief Access front element.*/
		T &front() { return at( 0 ); }
		/** \brief Access last element.*/
		T &back() { return at( siz - 1 ); }

		/** \brief Insert at the end.
		 *
		 *  The method inserts the value of \a arg at the end of vector.
		 *  \param arg the inserted value.*/
		void push_back( const T &arg );
		/** \brief Delete last.*/
		void pop_back();
		/** \brief Reverse elements
		 *
		 *  \param f the iterator (pointer) to the first element of reversed table.
		 *  \param l the iterator (pointer) to the past-the-end element of reversed table.*/
		static void rev( T* f,T *l );

		/** \brief Assign new content.
		 *
		 *  The method clears the vector and  inserts the values of elements form the container represented by iterator \a first and \a last.
		 *  \param first the iterator to the first element of the assigned container.
		 *  \param last the iterator to the past-the-end element of the assigned container.*/
		template< class InputIterator > void assign( InputIterator first, InputIterator last );

		/** \brief Insert elements.
		 *
		 *  The method inserts to the vector \a n elements of value \a x on the position \a where.
		 *  \param where the position to insert values.
		 *  \param n the number of inserted values.
		 *  \param x the value to insert. 	*/
		void insert( T *where, int n, const T &x );
		/** \brief Insert element.
		 *
		 *  The method inserts to the vector the value \a x on the position \a where.
		 *  \param where the position to insert values.
		 *  \param x the value to insert. 	*/
		void insert( T *where, const T &x ) { insert( where,1,x ); }
		/** \brief Insert elements.
		 *
		 *  The method inserts to the vector elements from container (\a first - \a last) on the position \a where.
		 *  \param where the place (pointer) to insert values.
		 *  \param first the iterator to the first element of the container with values to insert.
		 *  \param last iterator to the past-the-end element of the container with values to insert.*/
		template< class InputIterator > void insert( T *where, InputIterator first, InputIterator last );
		/** \brief Erase elements.
		 *
		 *  The method erases the element between the pointers (iterators) \a f and \a l.
		 *  \param f the pointer to the first erased element.
		 *  \param l the pointer to the past-the-end element of vector part with elements to erase.*/
		void erase( T *f, T *l );
		/** \brief Erase element.
		 *
		 *  \param f the pointer to the erased element.*/
		void erase( T* f ) { erase( f,f + 1 ); }
		/**\brief Clear vector.*/
		void clear() { siz = 0; }
		/** \brief Clear buffer.
		 *
		 *  The method assigns the value \a val to all the elements of the underlying table.
		 *  \param val the assigned value.*/
		void clearBuf( T val = T() ) { for( T *i = start; i < limit; i++ ) *i = val; }
	};

	/* PriQueueInterface DOCUMENT
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

#include "simple.hpp"
}

#endif
