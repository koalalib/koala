/* set_set.h
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

    /*
     * Set< Element >
     *     Zbiór realizujący podstawowe operacje teoriomnogościowe. Elementy po-
     * winny posiadać operatory ==, != i < (porządek liniowy).
     */

    // wypisywanie zbioru do strumienia dziala dla typu Element obslugujacego wypisywanie przez <<
    template< typename Element > std::ostream &operator<<( std::ostream &, const Set< Element > & );

    template< typename Element > class Set: public std::set< Element >, public SetElemForbidValue< Element >
    {
      public:
        // typ elementu zbioru
        typedef Element ElemType;

        // Konstruktory
        // Konstruktor tworzący zbiór pusty.
        Set(): std::set< Element >() { }
        // Konstruktory tworzące zbiór składający się z podanych elementów.
        Set( const Set< Element > &s ): std::set< Element >( s ) { }
        Set( const std::set< Element > &s ): std::set< Element >( s ) { }
        // na podstawie tablicy o zadanym rozmiarze
        Set( const Element *t, unsigned s ): std::set< Element >( t,t + s ) { }
        template< class Iter > Set( Iter b, Iter e ): std::set< Element >( b,e ) { }
        Set( const std::vector< Element > &v ): std::set< Element >( v.begin(),v.end() ) { }

        // Funkcje zastępujące zawartość zbioru podanym zakresem elementów.
        void assign( const Element *, unsigned );
        template< class Iter > void assign( Iter, Iter );

        // Operator przypisania.
        Set< Element > &operator=( const Element &e );

        // Operator przypisania zbioru o elementach innego typu (dla elementow zachodza rzutowania wartosci
        // typow T-> Element)
        template <class T>
        Set< Element > &operator=( const Set<T> &s );

        // Informacje odnośnie zbioru.
        // czy zbior jest pusty
        bool operator!() const { return this->size() == 0; }
        // Informacja o tym, czy jest podzbiorem podanego zbioru.
        bool subsetOf( const Set< Element > & ) const;
        // Informacja o tym, czy jest nadzbiorem podanego zbioru.
        bool supersetOf( const Set< Element > &s ) const { return s.subsetOf( *this ); }

        // Operacje na pojedynczych elementach zbioru.
        // Dodajemy element do zbioru, zwracając status operacji.
        bool add( const Element &e ) { return std::set< Element >::insert( e ).second; }
        Set< Element > &operator+=( const Element & );
        // Usuwamy element ze zbioru, zwracając status operacji.
        bool del( const Element &e ) { return std::set< Element >::erase( e ); }
        Set< Element > &operator-=( const Element & );
        // Sprawdzamy, czy element należy do zbioru.
        bool isElement( const Element &e ) const { return this->find( e ) != this->end(); }

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
        Element first() const;
        Element last() const;
        // zwracaja 0 gdy nie ma kolejnego elementu. Mozna podac 0, wowczas zwracaja element pierwszy/ostatni
        Element next( const Element & ) const;
        Element prev( const Element & ) const;

        Element min() const
        {
            koalaAssert( this->size(),ContExcOutpass )
            return this->first();
        }

        Element max() const
        {
            koalaAssert( this->size(),ContExcOutpass )
            return this->last();
        }


        void reserve( int ) { }
};

#include "set_set.hpp"
}
