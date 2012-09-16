/* set_vector.h
 * 
 */

#include <set>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>

namespace Koala
{

    /* Set< Element >
    *     Zbiór realizujący podstawowe operacje teoriomnogościowe. Elementy po-
    * winny posiadać operatory ==, != i < (porządek liniowy).
    */

    // wypisywanie zbioru do strumienia dziala dla typu Element obslugujacego wypisywanie przez <<
    template< typename Element > std::ostream& operator<<( std::ostream &, const Set< Element > & );

    template< typename Element > class Set: protected std::vector< Element >, public SetElemForbidValue< Element >
    {
      public:
        typedef Element ElemType; // typ elementu zbioru

        // Set udostepnia stale STL-owe iteratory do poruszania sie po elementach zbioru
        typedef typename std::vector< Element >::const_iterator const_iterator;
        const_iterator begin() const { return std::vector< Element >::begin(); }
        const_iterator end() const { return std::vector< Element >::end(); }

        // Konstruktory
        // Konstruktor tworzący zbiór pusty.
        Set(): std::vector< Element >() { }
        // Konstruktory tworzące zbiór składający się z podanych elementów.
        Set( const Set< Element > &s ): std::vector< Element >( s ) { }
        Set( const std::set< Element > &s ): std::vector< Element >( s.begin(),s.end() ) { }
        // na podstawie tablicy o zadanym rozmiarze
        Set( const Element *t, unsigned s ) { this->assign( t,t + s ); }
        template< class Iter > Set( Iter b, Iter e ) { this->assign( b,e ); }
        Set( const std::vector< Element > &v ) { this->assign( v.begin(),v.end() ); }

        // Funkcje zastępujące zawartość zbioru podanym zakresem elementów.
        void assign( const Element *t, unsigned s ) { this->assign( t,t+s ); }
        template< class Iter > void assign( Iter, Iter );

        // dodanie do zbioru elementow z podanego zakresu
        template< class Iter > void insert( Iter, Iter );

        // Operator przypisania.
        Set< Element > &operator=( const Element & );

        // Informacje odnośnie zbioru.
        // czy zbior jest pusty
        bool operator!() const { return this->size() == 0; }
        bool empty() const { return this->size() == 0; }

        unsigned size() const { return std::vector< Element >::size(); }
        void clear() { return std::vector< Element >::clear(); }

        // Informacja o tym, czy jest podzbiorem podanego zbioru.
        bool subsetOf( const Set< Element > & ) const;
        // Informacja o tym, czy jest nadzbiorem podanego zbioru.
        bool supersetOf( const Set< Element > &s ) const { return s.subsetOf( *this ); }

        // Operacje na pojedynczych elementach zbioru.
        // Dodajemy element do zbioru, zwracając status operacji.
        bool add( const Element & );
        Set< Element > &operator+=( const Element & );
        // Usuwamy element ze zbioru, zwracając status operacji.
        bool del( const Element & );
        Set< Element > &operator-=( const Element & );
        // Sprawdzamy, czy element należy do zbioru.
        bool isElement( const Element & ) const;

        // Operacje na całych zbiorach.
        // Suma zbiorów.
        Set< Element > &operator+=( const Set< Element > & );
        // Część wspólna zbiorów.
        Set< Element > &operator*=( const Set< Element > & );
        // Różnica zbiorów.
        Set< Element > &operator-=( const Set< Element > & );
        // Różnica symetryczna zbiorów.
        Set< Element > &operator^=( const Set< Element > &s ) { return *this = *this ^ s; }
        // Podzbiór elementów/usunięcie elementów spełniających/nie spełniających podanego
        // predykatu.
        template< class Funktor > Set< Element > subset( Funktor ) const;
        template< class Funktor > void truncate( Funktor fun ) { *this = subset( fun ); }

        // zapis elementow zbioru na podany iterator
        template< class Iter > int getElements( Iter ) const;

        // Metody iterujace po kolejnych elementach zbioru. Brak kolejnego elementu lub lista pusta - wartosc badValue()
        // - obie zwracaja 0 dla zbioru pustego
        Element first() const;
        Element last() const; 

        // zwracaja 0 gdy nie ma kolejnego elementu. Mozna podac 0, wowczas zwracaja element pierwszy/ostatni
        Element next( const Element & ) const; 
        Element prev( const Element & ) const;

        using std::vector< Element >::reserve;
};

#include "set_vector.hpp"
}
