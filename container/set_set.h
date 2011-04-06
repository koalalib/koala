#include <set>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>

namespace Koala {

/* ------------------------------------------------------------------------- *
 * Set< Element >
 *     Zbiór realizujący podstawowe operacje teoriomnogościowe. Elementy po-
 * winny posiadać operatory ==, != i < (porządek liniowy).
 * ------------------------------------------------------------------------- */

template< typename Element > class Set;

template< typename Element >
    bool operator==( const Set< Element > &, const Set< Element > & );
template< typename Element >
    bool operator!=( const Set< Element > &, const Set< Element > & );
template< typename Element >
    Set< Element > operator+( const Set< Element > &, const Set< Element > & );
template< typename Element >
    Set< Element > operator*( const Set< Element > &, const Set< Element > & );
template< typename Element >
    Set< Element > operator-( const Set< Element > &, const Set< Element > & );
template< typename Element >
    Set< Element > operator^( const Set< Element > &, const Set< Element > & );
template< typename Element >
    std::ostream& operator<<(std::ostream& ,const Set<Element> &);

template< typename Element > class Set: public std::set< Element > {
    public:
        typedef Element ElemType;
    // Konstruktory
        // Konstruktor tworzący zbiór pusty.
        Set(): std::set< Element >() { }
        // Konstruktory tworzące zbiór składający się z podanych elementów.
        Set( const Set<Element> & );
        Set( const std::set< Element > & );
        Set( const Element *, unsigned );
        template <class Iter>
            Set( Iter, Iter );
        Set( const std::vector< Element > & );

    // Funkcje zastępujące zawartość zbioru podanym zakresem elementów.
        void assign( const Element *, unsigned );
        template <class Iter>
            void assign( Iter, Iter );

    // Operator przypisania.
        Set<Element> &operator=( const Element &e );

    // Informacje odnośnie zbioru.
        bool operator!() const;
        // Informacja o tym, czy jest podzbiorem podanego zbioru.
        bool subsetOf( const Set<Element> & ) const;
        // Informacja o tym, czy jest nadzbiorem podanego zbioru.
        bool supersetOf( const Set<Element> & ) const;
        // Czy zbiory są identyczne/różne.

    // Operacje na pojedynczych elementach zbioru.
        // Dodajemy element do zbioru, zwracając status operacji.
        bool add( const Element & );
        Set<Element> &operator+=( const Element & );
        // Usuwamy element ze zbioru, zwracając status operacji.
        bool del( const Element & );
        Set<Element> &operator-=( const Element & );
        // Sprawdzamy, czy element należy do zbioru.
        bool isElement( const Element & ) const;

    // Operacje na całych zbiorach.
        // Suma zbiorów.
        Set<Element> &operator+=( const Set<Element> & );
        // Część wspólna zbiorów.
        Set<Element> &operator*=( const Set<Element> & );
        // Różnica zbiorów.
        Set<Element> &operator-=( const Set<Element> & );
        // Różnica symetryczna zbiorów.
        Set<Element> &operator^=( const Set<Element> & );
        // Podzbiór elementów/usunięcie elementów spełniających/nie spełniających podanego
        // predykatu.
        template <class Funktor>
            Set<Element> subset( Funktor );
        template <class Funktor>
            void truncate( Funktor );
        template <class Iter>
            int getElements( Iter );

        Element first() const;
        Element last() const;
        Element next( const Element & ) const;
        Element prev( const Element & ) const;
} ;

template< class Element > class SetInserter;

template< class Element >
class SetInserter< Set< Element > >: public std::iterator< std::output_iterator_tag,void,void,void,void >
{
    protected:
        Set< Element > *container;
        
    public:
        typedef Set< Element > container_type;
        SetInserter( Set< Element > &x ): container( &x ) { }
        SetInserter< Set< Element > > &operator= (const Element & value)
    { (*container)+=value; return *this; }
        SetInserter< Set< Element > > &operator*() { return *this; }
        SetInserter< Set< Element > > &operator++() { return *this; }
        SetInserter< Set<Element> > operator++(int) { return *this; }
} ;

template< class Element >
SetInserter< Set< Element > > setInserter( Set< Element > & );

#include "set_set.hpp"

}
