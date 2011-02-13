#ifndef __SET__H__
#define __SET__H__

#include <set>
#include <vector>

/* ------------------------------------------------------------------------- *
 * Set< Element >
 *     Zbiór realizujący podstawowe operacje teoriomnogościowe. Elementy po-
 * winny posiadać operatory ==, != i < (porządek liniowy).
 * ------------------------------------------------------------------------- */

template< typename Element > class Set: private std::vector< Element > {
    public:
    // Konstruktory
        // Konstruktor tworzący zbiór pusty.
        Set(): std::vector< Element >() { }
        // Konstruktory tworzące zbiór składający się z podanych elementów.
        Set( const Set & );
        Set( const std::set< Element > & );
        Set( const Element *, unsigned );
        Set( const std::vector< Element > & );
        
    // Informacje odnośnie zbioru.
        // Moc zbioru.
        unsigned cardinality() const;
        // Informacja o tym, czy zbiór jest pusty.
        bool isEmpty() const;
        // Informacja o tym, czy jest podzbiorem/podzbiorem właściwym podanego zbioru.
        bool operator<=( const Set & ) const;
        bool operator<( const Set & ) const;
        // Informacja o tym, czy jest nadzbiorem/nadzbiorem właściwym podanego zbioru.
        bool operator>=( const Set & ) const;
        bool operator>( const Set & ) const;
        // Czy zbiory są identyczne/różne.
        bool operator==( const Set & ) const;
        bool operator!=( const Set & ) const;
    
    // Operacje na pojedynczych elementach zbioru.
        // Dodajemy element do zbioru, zwracając status operacji.
        bool operator+=( const Element & );
        // Usuwamy element ze zbioru, zwracając status operacji.
        bool operator-=( const Element & );
        // Sprawdzamy, czy element należy do zbioru.
        bool isElement( const Element & );
    
    // Iterowanie po zbiorze.
        typedef typename std::vector< Element >::const_iterator const_iterator;
        typedef typename std::vector< Element >::iterator iterator;
        // Początek i koniec zbioru.
        const_iterator begin() const { return std::vector< Element >::begin(); }
        const_iterator end() const { return std::vector< Element >::end(); }
    
    // Operacje na całych zbiorach.
        // Suma zbiorów.
        Set operator+( const Set & );
        Set &operator+=( const Set & );
        // Część wspólna zbiorów.
        Set operator*( const Set & );
        Set &operator*=( const Set & );
        // Różnica zbiorów.
        Set operator-( const Set & );
        Set &operator-=( const Set & );
        // Różnica symetryczna zbiorów.
        Set operator^( const Set & );
        Set &operator^=( const Set & );
} ;

#include "set.hpp"
#endif
