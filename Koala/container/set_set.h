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

// wypisywanie zbioru do strumienia dziala dla typu Element obslugujacego wypisywanie przez <<
template< typename Element >
    std::ostream& operator<<(std::ostream& ,const Set<Element> &);

template< typename Element > class Set: public std::set< Element >, public SetElemForbidValue<Element> {
    public:
        typedef Element ElemType; // typ elementu zbioru
    // Konstruktory
        // Konstruktor tworzący zbiór pusty.
        Set(): std::set< Element >() { }
        // Konstruktory tworzące zbiór składający się z podanych elementów.
        Set( const Set<Element> & );
        Set( const std::set< Element > & );
        Set( const Element *, unsigned ); // na podstawie tablicy o zadanym rozmiarze
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
        bool operator!() const; // czy zbior jest pusty
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
            Set<Element> subset( Funktor ) const;
        template <class Funktor>
            void truncate( Funktor );
        // zapis elementow zbioru na podany iterator
        template <class Iter>
            int getElements( Iter ) const;

        // Metody iterujace po kolejnych elementach zbioru. Brak kolejnego elementu lub lista pusta - wartosc badValue()
        Element first() const;
        Element last() const;
        Element next( const Element & ) const; // zwracaja 0 gdy nie ma kolejnego elementu. Mozna podac 0, wowczas zwracaja element pierwszy/ostatni
        Element prev( const Element & ) const;

        void reserve(int) {}
} ;


#include "set_set.hpp"

}
