//Set jest szablonem matematycznego zbioru z dostepnymi operacjami mnogosciowymi.
//Z zalozenia nie jest zbyt efektywny, ale wygodny (przeznaczony dla uzytkownika), dlatego unikamy
//jego stosowania we wlasnych procedurach
//Sa dwie implementacje:
//dziedziczaca publicznie stlowy set (domyslna) i zawierajca vector (z wartosciami uporzadkowanymi).
//Wersje na vectorze wlacza zdefiniowanie stalej czasu kompilacji KOALA_SET_ON_VECTOR


// TODO: przebadac obie implementacje pod katem efektywnosci np. niepotrzebne kopiowanie kontenerow

#ifndef KOALA__SET__H__
#define KOALA__SET__H__

#include <limits>

namespace Koala {

// Dotychczas Set dzialal tylko dla typow wskaznikowych
// Teraz mozna go uzywac takze z numerycznymi i innymi, jesli dopisze sie odpowiednia specjalizacje tej klasy
template <class Element>
struct SetElemForbidValue {

    // wartosc, ktora nie moze byc uzywana jako element zbioru - na uzytek metod next,prev,first,last w Set
    static Element badValue() { return std::numeric_limits<Element>::max(); }
    static bool isBad(Element arg) { return arg==badValue(); }
};

template <class Element>
struct SetElemForbidValue<Element*> {

    // wartosc, ktora nie moze byc uzywana jako element zbioru - na uzytek metod next,prev,first,last w Set
    static Element* badValue() { return 0; }
    static bool isBad(Element* arg) { return arg==badValue(); }
};

}


#ifdef KOALA_SET_ON_VECTOR
#include "set_vector.h"
#else
#include "set_set.h"
#endif

namespace Koala {

//Iterator wstawiajacy elementy do podanego przez adres zewnetrznego zbioru
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

// funkcja tworzaca interator wstawiajacy do podanego zbioru

template< class Element >
SetInserter< Set< Element > > setInserter( Set< Element > & x)
{
    return SetInserter< Set< Element > >( x );
}

}

#endif
