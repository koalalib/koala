//Set jest szablonem matematycznego zbioru z dostepnymi operacjami mnogosciowymi.
//Z zalozenia nie jest zbyt efektywny, ale wygodny (przeznaczony dla uzytkownika), dlatego unikamy
//jego stosowania we wlasnych procedurach
//Sa dwie implementacje:
//dziedziczaca publicznie stlowy set (domyslna) i zawierajca vector (z wartosciami uporzadkowanymi).
//Wersje na vectorze wlacza zdefiniowanie stalej czasu kompilacji KOALA_SET_ON_VECTOR


// TODO: Operatory
//template< typename Element >
//    bool operator==( const Set< Element > &, const Set< Element > & );
//template< typename Element >
//    bool operator!=( const Set< Element > &, const Set< Element > & );
//template< typename Element >
//    Set< Element > operator+( const Set< Element > &, const Set< Element > & );
//template< typename Element >
//    Set< Element > operator*( const Set< Element > &, const Set< Element > & );
//template< typename Element >
//    Set< Element > operator-( const Set< Element > &, const Set< Element > & );
//template< typename Element >
//    Set< Element > operator^( const Set< Element > &, const Set< Element > & );
//dla Seta powinny miec wspolny kod w set.h, niezalezny od wyboru implementacji Seta (na
//vectorze lub secie). Po co dublowac kod, ktory i tak jest zrobiony na const_iteratorach?

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

// wylicza obraz zbioru w podanym przeksztalceniu
// niestety sama nie zajdzie dedukcja typu wyniku przy wywolaniu ( trzeba np. imageSet<double>(iset,kwadrat);)
template <class ValType, class ArgType, class Funktor>
Set<ValType> imageSet(const Set<ArgType>& arg, Funktor f)
{   Set<ValType> res;
    for(ArgType i=arg.first();!arg.isBad(i);i=arg.next(i)) res+=(ValType)f(i);
    return res;
}

// wylicza przeciwobraz zbioru w podanym przeksztalceniu i danej dziedzinie
template <class ValType, class ArgType, class Funktor>
Set<ArgType> preimageSet(const Set<ValType>& arg, const Set<ArgType>& domain, Funktor f)
{   Set<ArgType> res;
    for(ArgType i=domain.first();!domain.isBad(i);i=domain.next(i))
        if (arg.isElement((ValType)f(i))) res+=i;
    return res;
}

}

#endif
