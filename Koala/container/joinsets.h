#ifndef KOALA_JOINABLE_SETS_H
#define KOALA_JOINABLE_SETS_H

/* joinsets.h
 *
 */

#include <cassert>
#include <map>

#include "../container/assoctab.h"
#include "../container/localarray.h"
#include "../container/set.h"

namespace Koala
{


	/* JSPartDesrc
	 * struktura pomocnicza dla zbiorow zlaczalnych o elementach typu Klucz
	 */
	template< class Klucz > class JSPartDesrc
	{
		template< class K,class Cont > friend class JoinableSets;

		JSPartDesrc *parent,*next,*first,*last;
		unsigned int deg,size;
		Klucz key;

        public:
		JSPartDesrc() {}
	};


	/* JoinableSetsInternalTypes
	 *
	 */
	template< class ITEM > struct JoinableSetsInternalTypes
	{
		typedef Privates::BlockOfBlockList< BlockOfAssocArray< ITEM,JSPartDesrc< ITEM > * > > AssocBlockType;
		typedef JSPartDesrc< ITEM > BufElementType;
	} ;


    namespace Privates {

        template <class Klucz> struct JoinSetsIntPseudoMap {

            typedef JSPartDesrc< Klucz > * ValType;

            std::vector<std::pair<ValType,bool> > cont;

            JoinSetsIntPseudoMap(int asize=0) : cont(asize) {}

            void clear()
            {   cont.clear();   }

            void reserve(int asize)
            {   cont.resize(asize); }

            bool hasKey(int arg) const
            {
                koalaAssert( arg>=0 && arg < cont.size(),ContExcOutpass );
                return cont[arg].second;
            }

            ValType& operator[](int arg)
            {
                koalaAssert( arg>=0 && arg < cont.size(),ContExcOutpass );
                cont[arg].second=true;
                return cont[arg].first;
            }

            ValType operator[](int arg) const
            {
                koalaAssert( arg>=0 && arg < cont.size(),ContExcOutpass );
                if (!cont[arg].second) return ValType();
                return cont[arg].first;
            }
        };

        template <class Klucz> struct JoinSetsAssocContSwitch {
            typedef JoinSetsIntPseudoMap< Klucz > Type;
        };

        template <class T> struct JoinSetsAssocContSwitch<T*> {
            typedef AssocArray< T*,JSPartDesrc< T* > * >  Type;
        };

    }


	/* JoinableSets.
	 * Klasa zbioru rozlacznych zbiorow zlaczalnych o elementach typu wskaznikowego ITEM
	 * AssocContainer to typ wewnetrznej tablicy asoc. ITEM->JSPartDesrc< ITEM > *
	 */
	/** \brief Joinable Sets.
	 *
	 *  Class of disjoint sets. Useful when:
	 *  - the set of elements is known in advance,
	 *  - fast operations of joining two or more sets is required.
	 *
	 *  In other words JoinableSets class can by used to represent various partitions of set with fast union.
	 *  The structure is used for example in the implementation of Kruskal algorithm for minimal weight spanning tree.
	 *  \tparam ITEM class of stored element.
	 *  AssocContainer type of internal associative table. <tt>ITEM->JSPartDesrc< ITEM > *</tt>
	 *  \ingroup cont*/
//	template< class ITEM, class AssocContainer = AssocArray< ITEM,JSPartDesrc< ITEM > * > >
    template< class ITEM, class AssocContainer = typename Privates::JoinSetsAssocContSwitch<ITEM>::Type >
	class JoinableSets
	{
	protected:
		AssocContainer mapa;
		JSPartDesrc< ITEM > *bufor;
		size_t siz,part_no,maxsize;

	public:
		// typ reprezentanta pojedynczego zbioru
		typedef JSPartDesrc< ITEM > *Repr; /**\brief Identifier of set. */
		// typ elementow zbiorow
		typedef ITEM ElemType; /** \brief Element of set.*/

		// pierwszy argument - maks. liczba elementow wszystkich zbiorow, pozostale arg. ignorowane
		/** \brief Constructor.
		 *
		 *  \param n the size of the set of all elements.
		 *  \param p ignored
		 *  \param q ignored	*/
		JoinableSets( unsigned int n = 0 );
		/** \brief Copy constructor.*/
		JoinableSets( const JoinableSets< ITEM,AssocContainer > & );
		/** \brief Content copy operator.*/
		JoinableSets &operator=( const JoinableSets< ITEM,AssocContainer > & );
		~JoinableSets() { resize( 0 ); }

        //NEW: rezygnacja z tworzenia kontenera zewnetrzna tablica pamieci (podobnie jak w mapach)

		// czysci kontener i zmienia maks. liczbe elementow wszystkich zbiorow
		/** \brief Resize.
		 *
		 *  The method clears the set and change the maximal number of elements.
		 *  \param n the new number of elements.*/
		void resize( unsigned int n );

		// liczba wszystkich elementow w kontenerze
		/** \brief Get number of elements.*/
		int size() const { return siz; }
		// liczba elementow w zbiorze o podanym reprezentancie
		/** \brief Get number of elements in set identified by \a s*/
		int size( typename JoinableSets< ITEM >::Repr s) const;
		// liczba elementow zbioru zawierajacego argument, 0 jesli argument nie byl jeszcze wprowadzony
		/** \brief Get number of elements in set element \a i is in.*/
		int size( const ITEM &i ) const;

		// sprawdzenie, czy kontener jest pusty
		/** \brief Test if empty.*/
		bool empty() const { return siz == 0; }
		/** \brief Test if empty.*/
		bool operator!() const { return empty(); }

		/** \brief Delete all elements*/
		void clear() { resize( 0 ); }
		// liczba zbiorow
		/** \brief Get the number of parts.*/
		int getSetNo() const { return part_no; }
		// wpisuje elementy wszystkich zbiorow na podany iterator, zwraca size()
		/** \brief Get elements.
		 *
		 *  The method gets all the elements from and writes them down in \a iter.
		 *  \param[out] iter the iterator to the container with all elements.
		 *  \return the number of elements.*/
		template< class Iter > int getElements( Iter iter ) const;

		// wypisuje na podany iterator reprezentatow wszystkich zbiorow
		/** \brief Get identifiers.
		 *
		 *  The method puts the identifiers of sets to \a iter.
		 *  \param[out] iter the iterator to the container with identifiers.
		 *  \return the number of parts.*/
		template< class Iter > int getSetIds( Iter iter) const;
		// wypisuje na podany iterator elementy zbioru o podanym reprezencie
		/** \brief Get elements of part.
		 *
		 *  The method gets all the elements of part identifier \a s. The result is kept in container \a iter.
		 *  \param s the identifier (representative) of set part (subset).
		 *  \param[out] iter the iterator to the container with all the elements of part \a s.
		 *  \return the number of elements in the part.*/
		template< class Iter > int getSet( typename JoinableSets< ITEM >::Repr s, Iter iter ) const;
		// wypisuje na podany iterator elementy zbioru zawierajaccego podany element
		/** \brief Get elements of part.
		 *
		 *  The method gets all the elements of part containing the element \a i. The result is kept in container \a iter.
		 *  \param[out] iter the iterator to the container with all the elements of part \a i.
		 *  \return the number of elements in the part.*/
		template< class Iter > int getSet( const ITEM &i, Iter iter ) const { return getSet( getSetId( i ),iter ); }

		// tworzy nowy 1-elementowy zbior, zwraca jego reprezentanta lub 0 jesli element juz byl w jakims zbiorze
		// Jedyna metoda wprowadzajaca  zbiory z nowymi elementami do przechowywanej rodziny
		/** \brief Make single element.
		 *
		 *  The method creates new part with new single element. This is the only method of adding new elements to joinable set.
		 *  \param i the added element.
		 *  \return the identifier of the new created part or 0 if the element \a i already belongs to any part. */
		inline typename JoinableSets< ITEM >::Repr makeSinglet( const ITEM & );

		// zwraca reprezentanta zbioru zawierajacego dany element, lub 0 w razie jego braku
		/** \brief Get set identifier.
		 *
		 *  The method gets the identifier of part the element \a i belongs to.
		 *  \param i the considered element.
		 *  \return the identifier of part the element belongs to or 0 if there is no such element in set like \a i.*/
		inline typename JoinableSets<ITEM>::Repr getSetId( const ITEM &i ) const;
		// zwraca reprezentanta zbioru zawierajacego zbior o danym reprezentancie np. z wczesniejszego etapu scalania
		/** \brief Get set identifier.
		 *
		 *  The method gets the current identifier of part, the subset represented by identifier \a s belongs to.
		 *  \param s the tested part identifier.
		 *  \return the identifier of part, the block \a s is subset of.*/
		inline typename JoinableSets<ITEM>::Repr getSetId( typename JoinableSets< ITEM >::Repr s ) const;

		// zlacznia dwoch zbiorow podanych przez zadanie reprezentanta lub ktoregos z elementow
		// zwracaja reprezentanta wynikowego scalonego zbioru lub 0 w razie porazki (np. brak zbioru do scalenia, albo scalamy ze soba ten sam zbior)
		/** \brief Join parts.
		 *
		 *  The method joins two parts represented by the identifiers \a a and \a b.
		 *  \param a the identifier of the first part.
		 *  \param b the identifier of the second part.
		 *  \return the identifier of new joined set. */
		inline typename JoinableSets<ITEM>::Repr join( typename JoinableSets< ITEM >::Repr a,
			typename JoinableSets< ITEM >::Repr b );
		/** \brief Join parts.
		 *
		 *  The method joins two parts represented by the identifiers \a a and \a b.
		 *  \param a the element from the first part.
		 *  \param b the element from the second part.
		 *  \return the identifier of new joined set. */
		inline typename JoinableSets< ITEM >::Repr join( const ITEM &a, const ITEM &b );
		/** \brief Join parts.
		 *
		 *  The method joins two parts represented by the identifier \a a and element \a b.
		 *  \param a the identifier of the first part.
		 *  \param b the element from the second part.
		 *  \return the identifier of new joined set. */
		inline typename JoinableSets< ITEM >::Repr join( typename JoinableSets< ITEM >::Repr a, const ITEM &b );
		/** \brief Join parts.
		 *
		 *  The method joins two parts represented by the identifiers \a a and \a b.
		 *  \param a the element from the first part.
		 *  \param b the identifier of the second part.
		 *  \return the identifier of new joined set. */
		inline typename JoinableSets< ITEM >::Repr join( const ITEM &a, typename JoinableSets< ITEM >::Repr b );
	};

	// wypisywanie zawartosci zbiorow zlaczalnych do strumienia
	template< typename Element, typename Cont >
		std::ostream &operator<<( std::ostream &,const JoinableSets< Element,Cont > & );


#include "joinsets.hpp"
}

#endif
