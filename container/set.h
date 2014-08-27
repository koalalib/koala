#ifndef KOALA__SET__H__
#define KOALA__SET__H__

/* set.h
 *
 */

// Set jest szablonem matematycznego zbioru z dostepnymi operacjami mnogosciowymi.
//Z zalozenia nie jest zbyt efektywny, ale wygodny (przeznaczony dla uzytkownika), dlatego unikamy
//jego stosowania we wlasnych procedurach
//Sa trzy implementacje:
//dziedziczaca publicznie stlowy set (domyslna), zawierajca vector (z wartosciami uporzadkowanymi)
// oraz bazujaca na koalowej tablicy haszujacej
//Wersje niedomyslne wlacza zdefiniowanie stalej czasu kompilacji KOALA_SET_ON_VECTOR lub KOALA_SET_ON_HASHSET

// TODO: przebadac obie implementacje pod katem efektywnosci np. niepotrzebne kopiowanie kontenerow

#include <limits>

#include "localarray.h"
#include "../base/exception.h"

namespace Koala
{

    /* SetElemForbidValue
     * Pierwotnie Set dzialal tylko dla typow wskaznikowych
     * Teraz mozna go uzywac takze z numerycznymi i innymi, jesli dopisze sie odpowiednia specjalizacje tej klasy
     */
    template< class Element > struct SetElemForbidValue
    {
        // wartosc, ktora nie moze byc uzywana jako element zbioru - na uzytek metod next,prev,first,last w Set
        static Element badValue() { return std::numeric_limits< Element >::max(); }
        static bool isBad( Element arg ) { return arg == badValue(); }
    };

    template< class Element > struct SetElemForbidValue< Element * >
    {
        // wartosc, ktora nie moze byc uzywana jako element zbioru - na uzytek metod next,prev,first,last w Set
        static Element *badValue() { return 0; }
        static bool isBad( Element *arg ) { return arg == badValue(); }
    };

    template< typename Element > class Set;
    //WEN: a opisy?
    template< typename Element > bool operator==( const Set< Element > &, const Set< Element > & );
    template< typename Element > bool operator!=( const Set< Element > &s1, const Set< Element > &s2 )
        { return !(s1 == s2); }
    template< typename Element > Set< Element > operator+( const Set< Element > &, const Set< Element > & );
    template< typename Element > Set< Element > operator*( const Set< Element > &, const Set< Element > & );
    template< typename Element > Set< Element > operator-( const Set< Element > &, const Set< Element > & );
    template< typename Element > Set< Element > operator^( const Set< Element > &, const Set< Element > & );
}

#ifdef KOALA_SET_ON_VECTOR
#include "set_vector.h"
#elif defined(KOALA_SET_ON_HASHSET)
#include "set_hashset.h"
#else
#include "set_set.h"
#endif

namespace Koala
{

	template< class Element > class SetInserter;
	//Iterator wstawiajacy elementy do podanego przez adres zewnetrznego zbioru
	/** \briefn Set inserter.
	 *
	 * The iterator with ability to insert elements to set  given by pointer WEN: chyba referencje.
	 * \ingroup cont
	 *
	 *  [See example](examples/set/setInserter.html)
	 */
	template< class Element > class SetInserter< Set< Element > >:
		public std::iterator< std::output_iterator_tag,void,void,void,void >
	{
	protected:
		Set< Element > *container;

	public:
		typedef Set< Element > container_type;/**<\brief Type of container.*/
		/**\brief Constructor.*/
		SetInserter( Set< Element > &x ): container( &x ) { }
		/**\brief Copy content operator*/
		SetInserter< Set< Element > > &operator= ( const Element &value );
		/**\brief Dereference operator*/
		SetInserter< Set< Element > > &operator*() { return *this; }
		/**\brief Increment operator*/
		SetInserter< Set< Element > > &operator++() { return *this; }
		SetInserter< Set<Element> > operator++( int ) { return *this; }
	};

	// funkcja tworzaca interator wstawiajacy do podanego zbioru
	// WEN: a gdzie opis?
	template< class Element > SetInserter< Set< Element > > setInserter( Set< Element > &x )
		{ return SetInserter< Set< Element > >( x ); }

	// wylicza obraz zbioru w podanym przeksztalceniu
	// niestety sama nie zajdzie dedukcja typu wyniku przy wywolaniu ( trzeba np. imageSet<double>(iset,kwadrat);)
	/** \brief Image of set
	 *
	 * The method generate the image given by functor \a f of the set \a arg. The function is unable to guess the type of returned value so calls like <tt>imageSet<double>(iset,kwadrat);</tt> are obligatory.
	 * \ingroup cont
	 *
	 *  [See example](examples/set/setFunction.html)
	 */
	template< class ValType, class ArgType, class Funktor >
		Set< ValType > imageSet( const Set< ArgType > &arg, Funktor f );
	// wylicza przeciwobraz zbioru w podanym przeksztalceniu i danej dziedzinie
	/** \brief Preimage
	 *
	 * The method gets the preimage of the given set \a domain and the functor \a f.
	 * WEN: reszta komentarza jest bez sensu, bo funkcja owszem zgaduje zwracany typ i jest 3-argumentowa, por. set_test.cpp
	 * The function is unable to guess the type of returned value so calls like <tt>imageSet<double>(iset,kwadrat);</tt> are obligatory.
	 * \ingroup cont
	 *
	 *  [See example](examples/set/setFunction.html)
	 */
	template< class ValType, class ArgType, class Funktor >
		Set< ArgType > preimageSet( const Set< ValType > &arg, const Set< ArgType > &domain, Funktor f );

// hashset ma implementacje operatorow inne niz te stl-owe
#include "set.hpp"
}

#endif
