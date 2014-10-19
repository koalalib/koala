#ifndef KOALA_ASSOCTAB
#define KOALA_ASSOCTAB

/* assoctab.h
 *
 */

#include <map>
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

#include "localarray.h"
#include "privates.h"

//tablice asocjacyjne w Koali uzywaja jako kluczy typow wskaznikowych, klucz NULL ma specjalne znaczenie i jest zabroniony

namespace Koala
{
	//Klasa wrapper dostarczajaca wspolnego interfejsu koalowej tablicy asocjacyjnej dla zewnetrznego stalego kontenera.
	//Wprowadzajac do biblioteki nowy typ takiego kontenera nalezy dlan zdefiniowac AssocTabConstInterface
	template< class Container > class AssocTabConstInterface;

	// interfejs do kontenera nie-stalego, jest tworzony automatycznie na podstawie metod z AssocTabConstInterface
	template< class Container > class AssocTabInterface;

	namespace Privates
	{
		// kontrola zgodnosci typow kluczy dla przypisan miedzy roznymi tablicami asocjacyjnymi
		template< class Key > class AssocTabTag { };

		template< class Key> struct ZeroAssocKey
		{
		    static Key zero() { return (Key)0; }
		    static bool isZero(Key k) { return k==(Key)0; }
        };

        template< class Key> struct ZeroAssocKey<std::pair<Key,Key> >
		{
		    static std::pair<Key,Key> zero() { return std::pair<Key,Key>((Key)0,(Key)0); };
		    static bool isZero(std::pair<Key,Key> k) { return k==std::pair<Key,Key>((Key)0,(Key)0); }
        };
	}

	/* AssocTabInterface
	 * wrapper dla stl-owej mapy, podobna postac powinny miec wszystkie takie wrappery
	 * K - typ kluczy, V - typ wartosci przypisywanych
	*/
	/** \brief Constant STL map wrapper
	 *
	 *  This is the class of the constant object that wraps an STL map.
	 *  This interface delivers the standard constant methods for containers in Koala.
	 *  \tparam K the class for keys, usually pointers to objects.
	 *  \tparam V the class for matched values.
	 *  \ingroup cont
	 */
	template< class K, class V > class AssocTabConstInterface< std::map< K,V > >: public Privates::AssocTabTag< K >
	{
	public:
		// konstruktor pobiera oryginalna mape, ktorej sluzy za interfejs
		/** \brief Constructor
		 *
		 *  Assigns the STL map container \a acont to the member \a cont.
		 *  \param acont the original container.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_constructor.html">See example</a>.
		 */
		AssocTabConstInterface( const std::map< K,V > &acont ): cont( acont ) {}

		// typ klucza tablicy
		typedef K KeyType; /**< \brief Type of key. */
		// typ przypisywanych wartosci
		typedef V ValType; /**< \brief Type of mapped value.*/
		typedef std::map< K,V > OriginalType; /**< \brief Type of wrapped container.*/

		// test, czy mapa zawiera dany klucz
		/** \brief Test existence of key.
		 *
		 *  \param arg the tested key.
		 *  \return true if the key exists in the container, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_hasKey.html">See example</a>.
		 */
		bool hasKey( K arg ) const { return cont.find( arg ) != cont.end(); }

		// metody do iterowania po kolejnych kluczach tablicy. Zwracaja 0 gdy odpowiedniego klucza nie ma
		/** \brief Get the first key.
		 *
		 *  \return the key of the first element in the container. WEN: lub 0 kontener pusty
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_firstKey.html">See example</a>.
		 */
		K firstKey() const;

		/** \brief Get the last key.
		 *
		 *  \return the key of the last element in the container. WEN: lub 0 kontener pusty
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_lastKey.html">See example</a>.
		 */
		K lastKey() const;

		// mozna podac 0, by uzyskac ostatni/pierwszy klucz
		/** \brief Get previous key.
		 *
		 *  \param arg the reference key.
		 *  \return the key prior to \a arg.  If \a arg == 0, the last key is returned.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_prevKey.html">See example</a>.
		 */
		K prevKey( K arg ) const;

		/** \brief Get next key.
		 *
		 *  \param arg the reference key.
		 *  \return the key next to \a arg. If \a arg == 0, the first key is returned.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_nextKey.html">See example</a>.
		 */
		K nextKey( K arg ) const;

		/** \brief Get element.
		 *
		 *	If \a arg matches any key in the container, the matched value is returned, otherwise the empty constructor of \a ValType is called.
		 *  \param arg the searched key.
		 *  \return the mapped value associated with key \a arg.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_operator_brackets.html">See example</a>.
		 */
		V operator[]( K arg ) const;

		/** \brief Get size.
		 *
		 *	\return the number of elements in the container.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_size.html">See example</a>.
		 */
		unsigned size() const { return cont.size(); }

		/** \brief Test if empty.
		 *
		 *  \return the boolean value, true if the container has no elements, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_empty.html">See example</a>.
		 */
		bool empty() const { return this->size() == 0; }

		/** \brief Test if empty.
		 *
		 *  The overloaded operator!, tests if the container is empty.
		 *  \return the boolean value, true if the container has no elements, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_operator_negation.html">See example</a>.
		 */
		bool operator!() const { return empty(); }

		// zapisuje klucze tablicy pod zadany iterator
		/** \brief Get keys.
		 *
		 *  All the keys in the container are stored in another container with a defined iterator.
		 *  \tparam Iterator the class of iterator for the container storing the output set keys.
		 *  \param[out] iter the iterator connected with the container of output keys.
		 *  \return the number of keys.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_getKeys.html">See example</a>.
		 */
		template< class Iterator > int getKeys( Iterator iter ) const;

		// referencja do oryginalnego kontenera, na ktorym operuje wrapper
		/** \brief Original container. WEN: referencja do zewnetrznego!
		 *
		 *	The reference to the original container. The one the class wraps.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_cont.html">See example</a>.
		 */
		const std::map< K,V > &cont;

		//  pojemnosc kontenera
		/** \brief Get capacity.
		 *
		 *	The method gets the container capacity i.e. the number of elements which fit in the container without reallocation.
		 *  \return the capacity of the container.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabConstInterface_capacity.html">See example</a>.
		 */
		int capacity () const { return std::numeric_limits< int >::max(); }

	protected:
		// metody dzialajace na nie-stalym obiekcie, uzywane do tworzenia metod w AssocTabInterface
		// nie-stala ref. na oryginalny obiekt
		std::map< K,V >& _cont() { return const_cast< std::map< K,V >& >( cont ); }

		// rezerwowanie pojemnosci kontenera docelowego. Z zalozenia kontener powinien
		// dzialac efektywnie (np. bez wewnetrznych realokacji) dopoki liczba kluczy nie przekroczy argumentu
		// tej metody.
		void reserve( int ) { }
		void clear() { _cont().clear(); }
		// usuwanie klucza, zwraca true jesli klucz byl
		bool delKey( K );
		// referencja na wartosc przypisana do arg, wpis z wartoscia domyslna typu ValType jest tworzony jesli klucza nie bylo
		V &get( K arg );
		// wskaznik do wartosci przypisanej do arg, NULL w razie braku klucza
		ValType *valPtr( K arg );
	};

	//Funkcja tworzaca wrapper do podanego kontenera
	/** \brief Generating function of constant wrapper of STL map object. WEN: nie tylko STL
	*  \ingroup cont*/
	template< class T > AssocTabConstInterface< T > assocTabInterf( const T &cont )
		{ return AssocTabConstInterface< T >( cont ); }

	// wrapper do kontenera asocjacyjnego nie-stalego, metody tworzone sa na podstawie metod chronionych z AssocTabConstInterface
	/** \brief STL map wrapper. WEN: tak na prawde to nie tylko STL, AssocTabInterface jest tworzony na podstawie dobrze zrobionego (wraz z metodami protected)
        AssocTabConstInterface. Nie trzeba go pisac dla kazdej mapy od zera. Por. np. koncowka hashcont.h
	 *
	 *  This is the class of objects that wraps an STL map objects.
	 *  This interface delivers the standard WEN: not only const! non-constant  methods for containers in koala and
	 *   together with the  AssocTabConstInterface methods make the whole interface of the container.
	 *  \ingroup cont
	 */
	template< class T > class AssocTabInterface: public AssocTabConstInterface< T >
	{
	public:
		typedef typename AssocTabConstInterface< T >::KeyType KeyType;
		typedef typename AssocTabConstInterface< T >::ValType ValType;

		// referencja do oryginalnego kontenera
		/** \brief Original container. WEN: referencja do zewnetrznego
		 *
		 *	The reference to the original container. The one wrapped by the class.
		 *
		 *  <a href="examples/assoctab/assocTabConstInterface/assocTabInterface_cont.html">See example</a>.
		 */
		T &cont;

		// W konstruktorze podajemy oryginalny kontener
		/** \brief Constructor.
		 *
		 *  Assigns the original container \a acont (for example STL map) to the current WEN: reference container \a cont.
		 *	\param acont the original container.
		 *
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_constructor.html">See example</a>.
		 */
		AssocTabInterface( T &acont ): AssocTabConstInterface< T >( acont ), cont( acont ) { }

		/** \brief Copy content of container.
		 *
		 *  Overloaded operator= copies the content of \a arg to the current container.
		 *  \param arg  the copied container.
		 *
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_operator_assignment.html">See example</a>.
		 */
		AssocTabInterface< T > &operator=( const AssocTabInterface< T > &arg );

		/** \brief Copy content of container.
		 *
		 *  Overloaded operator= copies the content of \a arg to the current container.
		 *  \param arg  the copied container.
		 *
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_operator_assignment.html">See example</a>.
		 */
		AssocTabInterface< T > &operator=( const AssocTabConstInterface< T > &arg );
		//WEN: czym sie rozni od poprzedniego? To jest unikatowe w Koali, ze mozesz np. przypisac AssocArray<...> = AssocTable<...> jesli zgadzaja sie typy kluczy, a wartosci mapowane daja sie przepisac
		template< class AssocCont > AssocTabInterface< T > &operator=( const AssocCont &arg );

		/** \brief Reserve memory.
		 *
		 *  The method reserves the amount of memory sufficient for \a arg elements.
		 *  As long as the number of elements is not grater than \a arg, reallocation is not necessary. It is recommended to use when beginning the work with the object.
		 WEN: moze nic nie robic dla pewnych typow T
		 *  \param arg the number of elements for which memory is allocated.
		 *
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_reserve.html">See example</a>.
		 */
		void reserve( int arg ) { AssocTabConstInterface< T >::reserve( arg ); }

		/** \brief Clear container.
		 *
		 *  The method deletes all the elements from the container.
		 *
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_clear.html">See example</a>.
		 */
		void clear() { AssocTabConstInterface< T >::clear(); }

		/** \brief Delete element.
		 *
		 *  The method deletes the element associated with the key \a arg.
		 *  \param arg the key of the considered element.
		 *  \return true if the element existed.
		 *
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_delKey.html">See example</a>.
		 */
		bool delKey( KeyType arg) { return AssocTabConstInterface< T >::delKey( arg ); }

		/** \brief Get pointer to value.
		 *
		 *	The method gets the pointer to the value associated with the key \a arg.
		 *  \param arg the key of the searched element.
		 *  \return the pointer to the mapped value associated with the key \a arg. WEN: lub NULL gdy klucza nie bylo
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_valPtr.html">See example</a>.
		 */
		ValType* valPtr( KeyType arg ) { return AssocTabConstInterface< T >::valPtr( arg ); }

		/** \brief Get value.
		 *
		 *  The constant method gets the value associated with \a arg.WEN: lub wartosc domyslna przy braku klucza*/
		ValType operator[]( KeyType arg ) const { return AssocTabConstInterface< T >::operator[]( arg ); }

		// w przypadku obiektu nie-stalego zwraca referencje do przypisanej kluczowi wartosci. Nowy klucz dostaje wartosc domyslna typu ValType
		/** \brief Access element.
		 *
		 *  If the key \a arg exists the reference to the mapped value is returned, otherwise a new element associated with the \a arg is created with default mapped value gained from the call of the empty constructor of ValType.
		 *  \param arg the considered key.
		 *  \return the reference to the mapped value associated with the key \a arg or if the key does not exist the reference to the new-created element.
		 *
		 *  <a href="examples/assoctab/assocTabInterface/assocTabInterface_operator_brackets.html">See example</a>.
		 */
		ValType &operator[]( KeyType arg ) { return AssocTabConstInterface< T >::get( arg ); }
	};

	// sprawdzenie adresu oryginalnego kontenera dla interfejsow asocjacyjnych
	namespace Privates
	{
		template< class T > void *asssocTabInterfTest( const T & ) { return 0; }
		template< class T > void *asssocTabInterfTest( const AssocTabConstInterface< T > &arg) { return (void*)(&arg.cont); }
		template< class T > void *asssocTabInterfTest( const AssocTabInterface< T > &arg) { return (void*)(&arg.cont); }
	}

	/* AssocTable
	 * Opakowanie dla kontenera asocjacyjnego typu spoza Koali, udostepnia taki kontener w polu cont, jednoczesnie
	 * operujac na nim metodami z AssocTabInterface. Typ tworzony automatycznie na podstawie AssocTabConstInterface
	 */
	/** \brief Wrapper for external container.
	 *
	 *  This is the class of objects that wraps any container implemented outside the Koala WEN: nie bardzo any (asocjacyjny), nie bardzo outside np. BiDiHashMap.
	 *  Methods are similar to the ones in AssocTabInterface and AssocTabConstInterface.
	 *  \ingroup cont
	 */
	template< class T > class AssocTable: public Privates::AssocTabTag< typename AssocTabInterface< T >::KeyType >
	{
	public:
		/** \brief Wrapped container.*/
		T cont;

		/** \brief Constructor
		 *
		 *  Runs the empty constructor of \a cont.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_constructor.html">See example</a>.
		 */
		AssocTable(): cont(), inter( cont ) { }

		// konstruktory kopiujace i operatory przypisania
		/** \brief Copy constructor.
		 *
		 *  Copies the container \a acont to the container \a cont.
		 *  \param acont the copied container.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_constructor.html">See example</a>.
		 */
		AssocTable( const T &acont ): cont( acont ), inter( cont ) { }
		/** \brief Copy constructor.
		 *
		 *  Copies the original container \a cont of the wrapper object \a X to the container \a cont.
		 *  \param X the reference to copied AssocTable.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_constructor.html">See example</a>.
		 */
		AssocTable( const AssocTable< T > &X ): cont( X.cont ), inter( cont ) {}
		/** \brief Copy content of container.
		 *
		 *  Overloaded operator= copies the content of AssocCont \a X to the current container.
		 *  \param X  the copied container.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_operator_assignment.html">See example</a>.
		 */
		AssocTable< T > &operator=( const AssocTable< T > &X );
		/** \brief Copy content of container.
		 *
		 *  Overloaded operator= copies the container \a arg to the member \a cont.
		 *  \param arg  the copied container.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_operator_assignment.html">See example</a>.
		 */
		AssocTable< T > &operator=( const T &arg );
		/** \brief Copy content of container.
		 *
		 *  Overloaded operator= copies the container \a arg to the member \a cont. WEN: roznica z poprzednim? Dopuszcza kopiowanie miedzy roznymi typami tablic
		 WEN: To jest unikatowe w Koali, ze mozesz np. przypisac AssocArray<...> = AssocTable<...> jesli zgadzaja sie typy kluczy, a wartosci mapowane daja sie przepisac
		 *  \param arg  the copied container.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_operator_assignment.html">See example</a>.
		 */
		template< class AssocCont > AssocTable< T > &operator=( const AssocCont & );

		typedef typename AssocTabInterface< T >::KeyType KeyType;/**< \brief Type of key. */
		typedef typename AssocTabInterface< T >::ValType ValType;/**< \brief Type of mapped value.*/
		typedef typename AssocTabInterface< T >::OriginalType OriginalType;/**< \brief Type of wrapped container.*/

		/** \brief Test existence of key.
		 *
		 *  \param arg the tested key.
		 *  \return true if the key exists in the container, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_hasKey.html">See example</a>.
		 */
		bool hasKey( KeyType arg ) const { return inter.hasKey( arg ); }
		/** \brief Get pointer to value.
		 *
		 *  The method gets the pointer to the value associated with the key \a arg.
		 *  \param arg the key of the searched element.
		 *  \return the pointer to the mapped value associated with the key \a arg. WEN: albo NULL gdy klucza nie ma
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_valPtr.html">See example</a>.
		 */
		ValType* valPtr( KeyType arg ) { return inter.valPtr( arg ); }
		/** \brief Delete element.
		 *
		 *  The method deletes the element associated with the key \a arg.
		 *  \param arg the key of the considered element.
		 *  \return true if the element existed.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_delKey.html">See example</a>.
		 */
		bool delKey( KeyType arg ) { return inter.delKey( arg ); }
		/** \brief Get the first key.
		 *
		 *  \return the key of the first element in the container. WEN: lub 0 gdy kontener jest pusty
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_firstKey.html">See example</a>.
		 */
		KeyType firstKey() const { return inter.firstKey(); }
		/** \brief Get the last key.
		 *
		 *  \return the key of the last element in the container. WEN: lub 0 gdy kontener jest pusty
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_lastKey.html">See example</a>.
		 */
		KeyType lastKey() const { return inter.lastKey(); }

		/** \brief Get previous key.
		 *
		 *  \param arg the reference key.
		 *  \return the key prior to \a arg.  If \a arg == 0, the last key is returned.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_prevKey.html">See example</a>.
		 */
		KeyType prevKey( KeyType arg ) const { return inter.prevKey( arg ); }
		/** \brief Get next key.
		 *
		 *  \param arg the reference key.
		 *  \return the key next to \a arg. If \a arg == 0, the first key is returned.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_nextKey.html">See example</a>.
		 */
		KeyType nextKey( KeyType arg ) const { return inter.nextKey( arg ); }

		/** \brief Access element.
		 *
		 *  If the key \a arg exists the reference to the mapped value is returned, otherwise a new element associated with the \a arg is created with default mapped value gained from the call of the empty constructor of ValType.
		 *  \param arg the considered key.
		 *  \return the reference to the mapped value associated with the key \a arg or if the key does not exist the reference to the new-created element.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_operator_brackets.html">See example</a>.
		 */
		ValType &operator[]( KeyType arg) { return inter[arg]; }

		/** \brief Get element.
		 *
		 *  If \a arg matches any key in the container, the matched value is returned, otherwise the empty constructor of \a ValType is called.
		 *  \param arg the considered key.
		 *  \return the mapped value associated with key \a arg.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_operator_brackets.html">See example</a>.
		 */
		ValType operator[]( KeyType arg ) const { return ((AssocTabConstInterface< T >&)inter).operator[]( arg ); }

		/** \brief Get size.
		 *
		 *  \return the number of elements in the container.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_size.html">See example</a>.
		 */
		unsigned size() const { return inter.size(); }
		/** \brief Test if empty.
		 *
		 *  \return the boolean value, true if the container has no elements, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_empty.html">See example</a>.
		 */
		bool empty() const { return inter.empty(); }
		/** \brief Test if empty.
		 *
		 *  The overloaded operator!, tests if the container is empty.
		 *  \return the boolean value, true if the container has no elements, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_operator_negation.html">See example</a>.
		 */
		bool operator!() const { return empty(); }
		/** \brief Clear container.
		 *
		 *  The method deletes all the elements from the container.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_clear.html">See example</a>.
		 */
		void clear() { inter.clear(); }

		/** \brief Get keys.
		 *
		 *  All the keys in the container are stored in another container with a defined iterator.
		 *  \tparam Iterator the class of iterator for the container storing the output set keys.
		 *  \param[out] iter the iterator connected with the container of output keys.
		 *  \return the number of keys.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_getKeys.html">See example</a>.
		 */
		template< class Iterator > int getKeys( Iterator iter ) const { return inter.getKeys( iter ); }

		// nieobowiazkowe, moze byc nieobslugiwane przez niektore kontenery
		/** \brief Get capacity.
		 *
		 *  The method gets the container capacity i.e. the number of elements which fit in the container without reallocation.
		 WEN: moze nic nie robic dla niektorych typow kontenera T
		 *  \return the capacity of the container.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_capacity.html">See example</a>.
		 */
		int capacity() const { return inter.capacity(); }

		/** \brief Reserve memory.
		 *
		 *  The method reserves the amount of memory sufficient for \a arg elements.
		 *  As long as the number of elements is not grater than \a arg, reallocation is not necessary. It is recommended to use when beginning the work with the object.
		 WEN: moze nic nie robic dla niektorych typow kontenera T
		 *  \param arg the number of elements for which memory is allocated.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_reserve.html">See example</a>.
		 */
		void reserve( int n ) { inter.reserve( n ); }

        //NEW: generalnie: rezygnacja z tworzenia map z zewnetrzna tablica pamieci (konstruktory z dodatkowymi
        // parametrami wskaznikowymi. Zarowno tu, jak i w dalszych mapach.
		/** \brief Constructor
		 *
		 *  Runs the empty constructor of \a cont and reserves memory for \a n elements.
		 *
		 *  <a href="examples/assoctab/assocTable/assocTable_constructor.html">See example</a>.
		 */
		AssocTable( int n): cont(), inter( cont ) { inter.reserve( n ); }

	private:
		// interfejs na pole cont
		AssocTabInterface< T > inter;
	};

	// Funkcja tworzaca kopie podanego kontenera opakowana w AssocTable
	/** \brief Generating function for AssocTable.
	 *  \ingroup cont*/
	template< class T > AssocTable< T > assocTab( const T &cont ) { return AssocTable< T >( cont ); }

	//Klasy pomocnicze dla szybkich tablic asocjacyjnych (AssocArray). Klucz musi byc typem wskaznikowym zawierajacym
	//pole AssocKeyContReg assocReg, a w chwili wywolania kazdej metody z argumentem Klucz, argument ten musi
	// byc aktualnie wskaznikiem na aktualnie istniejacy (zywy) obiekt odpowiedniego typu.
	// Kontener dziedziczy po klasie abstrakcyjnej AssocContBase - wowczas obiekt zawierajacy assocReg
	//w chwili smierci automatycznie wypisuje klucze bedace wskazaniami na niego ze wszystkich takich kontenerow.
	//W Koali glownie wykorzystywane dla tablic asocj. wierz/krawedzi

	/* AssocContBase
	 *
	 */
	class AssocContReg;
	class AssocContBase
	{
	public:
		virtual void DelPosCommand( int ) = 0;
		virtual AssocContReg &getReg( int ) = 0;
	};

	/* AssocContReg
	 *
	 */
	class AssocContReg
	{
		template< class K, class E, class Cont > friend class AssocArray;
		friend class AssocKeyContReg;

		int nextPos;
		AssocContBase *next;
	};

	/* AssocKeyContReg
	 *
	 * Pole publiczne assocReg tego typu powinny miec obiekty, wskazniki na ktore moga byc kluczami w AssocArray
	 */
	class AssocKeyContReg: public AssocContReg
	{
		template< class K, class E, class Cont > friend class AssocArray;

	public:
		AssocKeyContReg() { next = 0; }
		AssocKeyContReg( const AssocKeyContReg & ) { next = 0; }
		inline AssocKeyContReg &operator=( const AssocKeyContReg & );
		~AssocKeyContReg() { deregister(); }

	private:
		inline AssocContReg *find( AssocContBase *cont );
		inline void deregister();
	};

	/* BlockOfAssocArray
	 *
	 */
	template< class Klucz, class Elem > struct BlockOfAssocArray
	{
		Elem val;
		Klucz key;
		AssocContReg assocReg;

		BlockOfAssocArray() : val(), key() {}
	};

	namespace Privates
	{

		// test czy Klucz jest wskaznikiem zawierajacym pole AssocKeyContReg assocReg - tylko wowczas kompiluje sie konstruktor domyslny
		template< class Klucz > class KluczTest
		{
		public:
			KluczTest( Klucz v = 0 ) { AssocKeyContReg *ptr = &v->assocReg; (void)(ptr);}
		} ;
	}

	/* AssocArray
	 *
	 */
	template< class Klucz, class Elem > struct AssocArrayInternalTypes
	{
		typedef Privates::BlockOfBlockList< BlockOfAssocArray< Klucz,Elem > > BlockType;
	};

	// Szybka tablica asocjacyjna np. dla wierz/kraw
	// Wiekszosc interfejsu jak w innych tabl. assocjacyjnych
	/** \brief Associative container.
	 *
	 *  The fast associative container.
	 *  The interface similar to other containers.
	 WEN: wskazywany przez klucz obiekt musi miec pole AssocKeyContReg assocReg
	 *  \ingroup cont
	 */
	template< class Klucz, class Elem, class Container = std::vector< typename
		AssocArrayInternalTypes<Klucz,Elem>::BlockType > > class AssocArray:
			public AssocContBase,
			protected Privates::KluczTest< Klucz >, public Privates::AssocTabTag< Klucz >
	{
	protected:
		mutable Privates::BlockList< BlockOfAssocArray< Klucz,Elem >,Container > tab;

		inline virtual void DelPosCommand( int pos ) { tab.delPos( pos ); }
		inline virtual AssocContReg &getReg( int pos ) { return tab[pos].assocReg; }

	public:
		typedef Klucz KeyType; /**< \brief Type of key. */
		typedef Elem ValType; /**< \brief Type of mapped value.*/

		typedef Container ContainerType;/**< \brief Type of container.*/

		// asize - rozmiar poczatkowy, drugi arg. ignorowany
		/** \brief Constructor
		 *
		 *  Reserves memory necessary for \a asize elements.
		 *  \param p should be ignored. WEN: jakie p?
		 *  \param asize the size of allocated memory.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_constructor.html">See example</a>.
		 */
		AssocArray( int asize = 0): tab( asize ) { }
		/** \brief Copy constructor.
		 *
		 *  Copies the container \a X to the new-created one.
		 *  \param X the copied container.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_constructor.html">See example</a>.
		 */
		AssocArray( const AssocArray< Klucz,Elem,Container > & );

		/** \brief Copy content of container.
		 *
		 *  Overloaded operator= copies the content of AssocArray \a X to the current container.
		 *  \param X  the copied container. WEN: zwraca referencje do siebie
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_operator_assignment.html">See example</a>.
		 */
		AssocArray< Klucz,Elem,Container > &operator=( const AssocArray< Klucz,Elem,Container > &X );

		/** \brief Copy content of container.
		 *
		 *  Overloaded operator= copies the content of a container AssocCont \a arg to the current container.
		 *  \param X  the copied container. WEN: roznica z poprzednim? Dopuszcza kopiowanie miedzy roznymi typami tablic
		 * WEN: czym sie rozni od poprzedniego? To jest unikatowe w Koali, ze mozesz np. przypisac AssocArray<...> = AssocTable<...> jesli zgadzaja sie typy kluczy, a wartosci mapowane daja sie przepisac
		 *  <a href="examples/assoctab/assocArray/assocArray_operator_assignment.html">See example</a>.
		 */
		template< class AssocCont > AssocArray &operator=( const AssocCont &arg );

		/** \brief Test if empty. WEN: bzdura!
		 *
		 *  \return the boolean value, true if the container has no elements, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_size.html">See example</a>.
		 */
		int size() const
			{ return tab.size(); }
        //NEW: ilosc obecnie zaalokowanych miejsc na elementy - w tym zakresie moga byc wartosci funkcji keyPos
        // zaalokowanych elementow
        int contSize() const
                { return tab.contSize(); }

		/** \brief Test if empty.
		 *
		 *  \return the boolean value, true if the container has no elements, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_empty.html">See example</a>.
		 */
		bool empty() const
			{ return tab.empty(); }

		/** \brief Test if empty.
		 *
		 *  The overloaded operator!, tests if the container is empty.
		 *  \return the boolean value, true if the container has no elements, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_operator_negation.html">See example</a>.
		 */
		bool operator!() const
			{ return empty(); }
		/** \brief Reserve memory.
		 *
		 *  The method reserves the amount of memory sufficient for \a arg elements.
		 *  As long as the number of elements is not grater than \a arg, reallocation is not necessary. It is recommended to use when beginning the work with the object.
		 *  \param arg the number of elements for which memory is allocated.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_reserve.html">See example</a>.
		 */
		void reserve( int arg )
			{ tab.reserve( arg ); }
		/** \brief Get capacity.
		 *
		 *  The method gets the container capacity i.e. the number of elements which fit in the container without reallocation.
		 *  \return the capacity of the container.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_capacity.html">See example</a>.
		 */
		int capacity() const
			{ return tab.capacity(); }
		/** \brief Get pointer to value. WEN: bzdura!
		 *
		 *  The method gets the pointer to the value associated with the key \a v.
		 *  \param v the key of the searched element.
		 *  \return the pointer to the mapped value associated with the key \a v.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_hasKey.html">See example</a>.
		 */
		bool hasKey( Klucz v ) const
			{ return keyPos( v ) != -1; }
		/** \brief Get pointer to value.
		 *
		 *  The method gets the pointer to the value associated with the key \a v.
		 *  \param v the key of the searched element.
		 *  \return the pointer to the mapped value associated with the key \a v. WEN: albo NULL gdy klucza nie ma
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_valPtr.html">See example</a>.
		 */
		Elem *valPtr( Klucz v );
		// pozycja klucza w wewnetrznym indeksie tablicy, -1 w razie jego braku
		/** \brief Get position of key.
		 *
		 *  \param v the key for which the position is calculated.
		 *  \return the position of the key \a v in the container or -1 if any error occurs.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_keyPos.html">See example</a>.
		 */
		inline int keyPos( Klucz v ) const;

		/** \brief Delete element.
		 *
		 *  The method deletes the element associated with the key \a v. WEN: no, sam klucz tez kasuje
		 *  \param v the key of the considered element.
		 *  \return true if the element existed.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_delKey.html">See example</a>.
		 */
		bool delKey( Klucz v );

		/** \brief Get the first key.
		 *
		 *  \return the key of the first element in the container.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_firstKey.html">See example</a>.
		 */
		Klucz firstKey() const;

		/** \brief Get the last key.
		 *
		 *  \return the key of the last element in the container.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_lastKey.html">See example</a>.
		 */
		Klucz lastKey() const;

		/** \brief Get next key.
		 *
		 *  \param v the reference key.
		 *  \return the key next to \a v. If \a v == 0, the first key is returned. WEN: dla ostatniego klucza zwraca 0
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_nextKey.html">See example</a>.
		 */
		Klucz nextKey( Klucz v ) const;

		/** \brief Get previous key.
		 *
		 *  \param v the reference key.
		 *  \return the key prior to \a v.  If \a v == 0, the last key is returned. WEN: dla pierwszego klucza zwraca 0
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_prevKey.html">See example</a>.
		 */
		Klucz prevKey( Klucz v ) const;

		/** \brief Access element.
		 *
		 *  If the key \a v exists the reference to the mapped value is returned, otherwise a new element associated with the \a v is created with default mapped value gained from the call of the empty constructor of ValType.
		 *  \param v the considered key.
		 *  \return the reference to the mapped value associated with the key \a v or if the key does not exist the reference to the new-created element.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_operator_brackets.html">See example</a>.
		 */
		inline Elem &operator[]( Klucz v );

		/** \brief Get element.
		 *
		 *  If \a v matches any key in the container, the matched value is returned, otherwise the empty constructor of \a ValType is called.
		 *  \param v the considered key.
		 *  \return the mapped value associated with key \a v.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_operator_brackets.html">See example</a>.
		 */
		inline Elem operator[]( Klucz v ) const;

		// porzadkowanie indeksu kontenera, wszystkie klucze uzyskuja kolejne numery poczatkowe
		/** \brief Reorder the numbers.
		 *
		 *  All the keys obtain consecutive numbers. The method is useful after multiple deletions.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_defrag.html">See example</a>.
		 */
		void defrag();

		/** \brief Clear container.
		 *
		 *  The method deletes all the elements from the container.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_clear.html">See example</a>.
		 */
		void clear();

		/** \brief Get keys.
		 *
		 *  All the keys in the container are stored in another container with a defined iterator.
		 *  \tparam Iterator the class of iterator for the container storing the output set keys.
		 *  \param[out] iter the iterator connected with the container of output keys.
		 *  \return the number of keys.
		 *
		 *  <a href="examples/assoctab/assocArray/assocArray_getKeys.html">See example</a>.
		 */
		template< class Iterator > int getKeys( Iterator ) const;

		~AssocArray()
			{ clear(); }
	};



	namespace Privates {

	// Kontener udajacy AssocArray np. dla kluczy nie wspolpracujacych z tym kontenerem. Uzywa dodatkowego
	// wewnetrznego kontenera typu AssocCont (mapa: Klucz->int). Nie oferuje automatycznego wypisywania kluczy bedacych
	// wskaznikami na znikajace obiekty. Chyba jedyne sensowne zastosowanie, to wykorzystanie jako IndexContainer
	// w ponizszejh AssocMatrix w sytuacji, gdy jej klucz nie obsluguje AssocArray

	/** \brief Pseudo associative array.
	 *
	 *  The class pretending to be the AssocArray for keys that are not designed to work with AssocArray. The interface remains the same.
	 *  May be useful if the usage of AssocMatrix is necessary.
	 *  \ingroup cont
	 */
	template< class Klucz, class Elem, class AssocCont, class Container =
		std::vector< typename AssocArrayInternalTypes< Klucz,Elem >::BlockType > > class PseudoAssocArray:
			public Privates::AssocTabTag< Klucz >
		{
		protected:
			mutable Privates::BlockList< BlockOfAssocArray< Klucz,Elem >,Container > tab;
			AssocCont assocTab;

		public:
			typedef Klucz KeyType;
			typedef Elem ValType;

			typedef Container ContainerType;
			typedef AssocCont AssocContainerType;

			PseudoAssocArray( int asize = 0): tab( asize ), assocTab( asize ) { }

			template< class AssocCont2 >
				PseudoAssocArray< Klucz,Elem,AssocCont,Container > &operator=( const AssocCont2 &arg );
			int size() const
				{ return tab.size(); }
            int contSize() const
                { return tab.contSize(); }
			bool empty() const
				{ return tab.empty(); }
			bool operator!() const
				{ return empty(); }
			void reserve( int arg );
			int capacity() const
				{ return tab.capacity(); }
			bool hasKey( Klucz v ) const
				{ return keyPos( v ) != -1; }
			Elem *valPtr( Klucz v );
			int keyPos( Klucz ) const;
			bool delKey( Klucz );
			Klucz firstKey() const;
			Klucz lastKey() const;
			Klucz nextKey( Klucz ) const;
			Klucz prevKey( Klucz ) const;
			Elem &operator[]( Klucz );
			Elem operator[]( Klucz ) const;
			void defrag();
			void clear();

			template< class Iterator > int getKeys( Iterator ) const;

			~PseudoAssocArray()
				{ clear(); }
		};

	}

	// Typy pomocnicze 2-wymiarowych tablic asocjacyjnych (oba wymiary tego samego typu)
	/** \brief Associative matrix type.
	 *
	 *  Option used to parametrize the Associative matrix.
	 *  \ingroup cont */
	enum AssocMatrixType
	{
		/** \brief Full 2-dimensional matrix.*/
		AMatrFull, // pelna tablica 2-wymiarowa
		/** \brief 2-dimensional matrix without elements on diagonal, identical coordinates are forbidden.*/
		AMatrNoDiag, // tablica 2-wymiarowa nie akceptujaca kluczy o obu wspolrzednych rownych (kluczami sa tylko 2-elementowe pary)
		/** \brief Triangular matrix, elements (a,b) are regarded as (b,a).*/
		AMatrTriangle, // tablica traktujaca klucze jako pary nieuporzadkowane tj. klucz (a,b) jest utozsamiany z (b,a)
		/** \brief Diagonal matrix, only elements on the diagonal are allowed i.e. only elements (a,a) are in the matrix and it becomes "1-dimensional".*/
		//WEN: bzdura, dopuszcza sie rowniez (a,a) w przeciwienstwie do AMatrTriangle, reszta tak samo jak w AMatrTriangle
		AMatrClTriangle  // j.w. ale dopusza sie klucze "jednoelementowe" tj. postaci (a,a)
	};

	// klasa wspomagajaca operowanie na kluczach tablic 2-wymiarowych
	// Uzywana wewnatrz tego kontenera
	template< AssocMatrixType > class Assoc2DimTabAddr;

	/* Assoc2DimTabAddr
	 *
	 */
	template<> class Assoc2DimTabAddr< AMatrFull >
	{
	protected:
		// dlugosc wewnetrznego bufora dla podanej liczby kluczy
		static int bufLen( int n ) { return n * n; }
		// przerabia pare numerow elementow klucza (w indeksie kluczy) na pozycje bufora wewnetrznego
		inline int wsp2pos( std::pair< int,int > ) const;
		// ... i odwrotnie
		inline std::pair< int,int > pos2wsp( int ) const;

		inline int colSize(int i,int n) const { return n; }
		inline std::pair< int,int > wsp2pos2(std::pair< int,int > arg) const { return arg; }
		// test, czy klucz danej postaci jest akceptowany przez ten typ tablicy
    public:
		template< class T > bool correctPos( T, T ) const
			{ return true; }
		// przerabia klucz 2-wymiarowy (tj. pare kluczy) na postac standardowa dla danego typu tablicy
		template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
			{ return std::pair< Klucz,Klucz >( u,v ); }
		template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const
			{ return k; }
	};

	template<> class Assoc2DimTabAddr< AMatrNoDiag >
	{
		protected:
			static int bufLen( int n ) { return n * (n - 1); }
			inline int wsp2pos( std::pair< int,int > ) const ;
			inline std::pair< int,int > pos2wsp( int ) const ;

		inline int colSize(int i,int n) const { return n-1; }
		inline std::pair< int,int > wsp2pos2(std::pair< int,int > arg) const
		{ return std::pair< int,int >(arg.first,arg.second -(arg.second>arg.first)); }

        public:
			template< class T > bool correctPos( T u, T v )  const  { return u != v; }
			template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
				{ return std::pair< Klucz,Klucz >( u,v ); }
			template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const
				{ return k; }
	};

	template<> class Assoc2DimTabAddr< AMatrClTriangle >
	{
	protected:
		static int bufLen( int n )  { return n * (n + 1) / 2; }
		inline int wsp2pos( std::pair< int,int > ) const ;
		inline std::pair< int,int > pos2wsp( int ) const ;

		inline int colSize(int i,int n) const { return i+1; }
		inline std::pair< int,int > wsp2pos2(std::pair< int,int > arg) const { return pairMaxMin(arg); }

    public:
		template< class T > bool correctPos( T, T ) const  { return true; }
		template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
			{ return pairMinMax( u,v ); }
		template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const
			{ return pairMinMax( k.first,k.second ); }
	};

	template <> class Assoc2DimTabAddr< AMatrTriangle >
	{
	protected:
		static int bufLen( int n ) { return n * (n - 1) / 2; }
		inline int wsp2pos( std::pair< int,int > ) const ;
		inline std::pair< int,int > pos2wsp( int ) const ;

		inline int colSize(int i,int n) const { return i; }
		inline std::pair< int,int > wsp2pos2(std::pair< int,int > arg) const { return pairMaxMin(arg); }

    public:
		template< class T > bool correctPos( T u, T v )  const { return u != v; }
		template< class Klucz > inline std::pair< Klucz,Klucz > key( Klucz u, Klucz v ) const
			{ return pairMinMax( u,v ); }
		template< class Klucz > inline std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k ) const
			{ return pairMinMax( k.first,k.second ); }
	};

	// BlockOfAssocMatrix
	template< class Elem > struct BlockOfAssocMatrix
	{
		Elem val;
		int next,prev;
		bool present() const { return next || prev; }
		BlockOfAssocMatrix(): val(), next( 0 ), prev( 0 ) { }
	};

	namespace Privates
	{
		// kontrola zgodnosci typow kluczy dla przypisan miedzy roznymi tablicami asocjacyjnymi
		template< class Key,AssocMatrixType > class Assoc2DimTabTag { };
	}

	/* AssocMatrix
	 *
	 */
	template <class Klucz, class Elem>
	struct AssocMatrixInternalTypes
	{
		typedef BlockOfAssocMatrix< Elem > BlockType;
		typedef Privates::BlockOfBlockList< BlockOfAssocArray< Klucz,int > > IndexBlockType;
	};

	// 2-wymiarowa tablica asocjacyjna. Uwaga: wersja z IndexContainer = IndexContainer = AssocArray< Klucz,...>
	// ma te same ograniczenia odnosnie uzywanych kluczy, co AssocArray (j.w.)
	/** \brief Associative matrix.
	 *
	 *  Two-dimensional associative container. That assigns an element to the pair of keys.
	 *  \tparam aType decides over the type of matrix (AssocMatrixType).
	 *  \tparam Container the type of internal container used to store mapped values.
	 *  \tparam IndexContainer the type of internal associative table used to assign various date (numbers) to single keys.
	 WEN: uwaga - domyslnie jest to  AssocArray, ale wowczas WEN: wskazywany przez klucz obiekt musi mieæ pole AssocKeyContReg assocReg
	 Jesli tak nie jest, mozna zastapic AssocArray udajaca ja mapa PseudoAssocArray, ktora zapewnia podobna numeracje, lecz jest inaczej zorganizowana
	 *  \sa Koala::AssocMatrixType
	 *  \ingroup cont*/
	template< class Klucz, class Elem, AssocMatrixType aType, class Container =
		std::vector< typename AssocMatrixInternalTypes<Klucz,Elem>::BlockType >, class IndexContainer =
			AssocArray< Klucz,int,std::vector< typename AssocMatrixInternalTypes< Klucz,Elem >::IndexBlockType > > >
	// Container - typ wewnetrznego bufora - tablicy przechowujacej opakowany ciag wartosci przypisanych roznym parom kluczy
	// IndexContainer - typ indeksu tj. tablicy asocjacyjnej przypisujacej pojedynczym kluczom ich liczby wystapien we wpisach oraz (rozne) numery.
	class AssocMatrix: public Assoc2DimTabAddr< aType >, public Privates::Assoc2DimTabTag< Klucz,aType >
	{
		template< class A, class B, AssocMatrixType C, class D, class E > friend class AssocMatrix;

	private:
		class AssocIndex: public IndexContainer
		{
		public:
			AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > *owner;

			// rozmiar poczatkowy
			AssocIndex( int asize = 0 ): IndexContainer( asize ) { }


			// konwersja klucza na jego numer, -1 w razie braku
			int klucz2pos( Klucz v)
			{
				if (!v) return -1;
				return IndexContainer::keyPos( v );
			}
			// i odwrotnie
			Klucz pos2klucz( int );
			inline virtual void DelPosCommand( int );

			friend class AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >;
		};

		// wewnetrzny indeks pojedynczych kluczy (mapa: Klucz->int)
		mutable AssocIndex index;

		friend class AssocIndex;

		// glowny bufor z wartosciami
		mutable Container bufor;
		// wartosci przypisane parom (kluczom) sa w buforze powiazane w liste 2-kierunkowa
		int siz,first,last;

		// usuniecie wpisu dla kluczy o podanych numerach
		void delPos( std::pair< int,int >  );

	protected:
		struct DefragMatrixPom
		{
			Klucz u,v;
			Elem val;
		};

	public:
		// typ klucza
		typedef Klucz KeyType; /**< \brief Type of key.*/
		// typ wartosci przypisywanej parze (tj. kluczowi 2-wymiarowemu)
		typedef Elem ValType;/**< \brief Type of mapped value*/

		typedef Container ContainerType;/**<\brief The type of internal container used to store maped values. */
		typedef IndexContainer IndexContainerType; /**<\brief The type of internal associative.*/
		enum { shape = aType };/**< \brief Matrix type \sa AssocMatrixType*/

		// pierwszy arg. - poczatkowy rozmiar (tj. pojemnosc indeksu pojedynczych kluczy), dalsze arg. ignorowane
		/** \brief Constructor.
		 *
		 *  Creates the associative matrix and allocates memory for \a asize elements.
		 *  \param asize the number of element that can be added to matrix without reallocation.
		 *  \param p should be ignored. WEN: juz ich nie ma
		 *  \param q should be ignored.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_constructor_AMatrClTriangle.html">See example with AMatrFull</a>.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_constructor_AMatrClTriangle.html">See example with AMatrNoDiag</a>.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_constructor_AMatrClTriangle.html">See example with AMatrTriangle</a>.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_constructor_AMatrClTriangle.html">See example with AMatrClTriangle</a>.
		 */
		AssocMatrix( int = 0);
		/** \brief Copy contructor.*/
		AssocMatrix( const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > &X ):
			index( X.index ), bufor( X.bufor ), siz( X.siz ), first( X.first ), last( X.last )
			{   index.owner = this; }

		/** \brief Copy content operator.
		 *
		 * \param X the copied matrix.*/
		AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >
			&operator=( const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > & );
    //WEN: czym sie rozni od poprzedniego? To jest unikatowe w Koali, ze mozesz np. przypisac AssocArray<...> = AssocTable<...> jesli zgadzaja sie typy kluczy, a wartosci mapowane daja sie przepisac
		template< class MatrixContainer > AssocMatrix &operator=( const MatrixContainer &X );


		// operowanie na indeksie zawierajacym pojedyncze klucze
		/** \brief Test whether key appear in the matrix.
		 *
		 *  The method test whether the single key \a v appears in any pair of keys.
		 *  \param v the tested key.
		 *  \return true if there exist an element for which \a v is one of keys, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_hasInd.html">See example</a>.
		 */
		bool hasInd( Klucz v ) const { return index.hasKey( v ); }

		// usuwa takze wszystkie wpisy zawierajace argument jako jeden z elementow pary kluczy
		/** \brief Delete single key.
		 *
		 *  The method deletes all the elements for which WEN: chyba co najmniej jeden? rowniez on of the keys is \a v.
		 *  \param the eliminated key.
		 *  \return true if at least one element was deleted.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_delInd.html">See example</a>.
		 */
		bool delInd( Klucz v );

		/** \brief Get first key.
		 *
		 *  The method allows to get to the first element on the list of single keys that appear in associative matrix.
		 *  \return  the first key on the list of all single keys. WEN: lub 0 przy pustym kont.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_firstInd.html">See example</a>.
		 */
		Klucz firstInd() const { return index.firstKey(); }

		/** \brief Get last key.
		 *
		 *  The method allows to get to the last element on the list of single keys that appear in associative matrix.
		 *  \return  the last key on the list of all single keys. WEN: lub 0 przy pustym kont.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_lastInd.html">See example</a>.
		 */
		Klucz lastInd() const { return index.lastKey(); }

		/** \brief Get next key.
		 *
		 *  The method allows to get to the next element after \a v from the list of single keys that appear in associative matrix.
		 *  \return  the next key on the list of all single keys. WEN: lub NULL dla ostatniego, lub pierwszy indeks dla arg=0
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_nextInd.html">See example</a>.
		 */
		Klucz nextInd( Klucz v )const  { return index.nextKey( v ); }

		/** \brief Get previous key.
		 *
		 *  The method allows to get to the element previous to \a v from the list of single keys that appear in associative matrix.
		 *  \return  the previous key on the list of all single keys. WEN: lub NULL dla pierwszego, lub ostatni indeks dla arg=0
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_prevInd.html">See example</a>.
		 */
		Klucz prevInd( Klucz v ) const { return index.prevKey( v ); }

		/** \brief Get size of single keys list.
		 *
		 *  \return  the number of single keys that appear in the matrix.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_indSize.html">See example</a>.
		 */
		int indSize() const { return index.size(); }

        //WEN: opis?
		template< class Iterator > int getInds( Iterator iter ) const {   return index.getKeys(iter); }

		// zapis do zewnetrznej mapy (Klucz->ValType) wartosci wszystkich wpisow dla kluczy 2-wymiarowych zawierajacych argument na pierwszej pozycji
		/** \brief Slice by first key.
		 *
		 *  The method stores up the elements of the matrix such that the first coordinate of element is \a v ("gets v-th row").
		 *  The result is kept in one dimension associative table (Key->ValType) that associates the second Key with element.
		 *  \param v the distinguished key.
		 *  \param[out] tab the output table (Key->ValType) that associates the second Key with element.
		 *  \return the number of elements in the output container \a out.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_slice1.html">See example</a>.
		 */
		template< class ExtCont > int slice1( Klucz, ExtCont & ) const;

		// ... i drugiej pozycji
		/** \brief Slice by second key.
		 *
		 *  The method stores up the elements of the matrix such that the second coordinate of element is \a v ("gets v-th column").
		 *  The result is kept in one dimension associative table (Key->ValType) that associates the second Key with element.
		 *  \param v the distinguished key.
		 *  \param[out] tab the output table (Key->ValType) that associates the first Key with element.
		 *  \return the number of elements in the output container \a out.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_slice2.html">See example</a>.
		 */
		template<class ExtCont > int slice2( Klucz, ExtCont & ) const;

		//W operacjach dzialajacych na kluczach 2-wymiarowych mozna poslugiwac sie para kluczy lub jej dwoma wspolrzednymi

		// czy wpis dla danej pary kluczy jest w tablicy
		/** \brief Test whether a pair of keys associate an element in matrix.
		 *
		 *  \param u the first key of the searched pair.
		 *  \param v the second key of the searched pair.
		 *  \return true if there is an element associated with the pair of keys \a u and \a v, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_hasKey.html">See example</a>.
		 */
		bool hasKey( Klucz u, Klucz v ) const;

		/** \brief Test whether a pair of keys associate an element in matrix.
		 *
		 *  \param k the searched pair, the standard pair of keys.
		 *  \return true if there is an element associated with the pair \a k, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_hasKey.html">See example</a>.
		 */
		bool hasKey( std::pair< Klucz,Klucz > k ) const { return hasKey( k.first,k.second ); }

		// usun wspis, zwraca false jesli klucza nie bylo
		/** \brief Delete element.
		 *
		 *  \param u the first key of the deleted pair.
		 *  \param v the second key of the deleted pair.
		 *  \return true if there was an element associated with the pair of keys \a u and \a v, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_delKey.html">See example</a>.
		 */
		bool delKey( Klucz u, Klucz v);
		/** \brief Delete element.
		 *
		 *  \param k the deleted pair.
		 *  \return true if there was an element associated with the pair of keys \a u and \a v, false otherwise.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_dekKey.html">See example</a>.
		 */
		bool delKey( std::pair< Klucz,Klucz > k ) { return delKey( k.first,k.second ); }

		// referencja do wartosc przypisanej kluczowi 2-wymiarowemu, jesli klucza nie bylo - tworzona jest wartosc domyslna typu ValType
		/** \brief Access element.
		 *
		 *  The method gets the reference of the element associated with the pair \p (u,v). If there wasn't any new element is created.
		 *  The mapped value of the element is assigned by the empty constructor of ValType.
		 *  \param u the first key of the key pair of element.
		 *  \param v the second key of the key pair of element.
		 *  \return the reference to the element associated with pair \p (u,v).
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_operator_brackets.html">See example</a>.
		 */
		Elem &operator()( Klucz u, Klucz v );
		/** \brief Access element.
		 *
		 *  The method gets the reference of the element associated with the pair \a k. If there wasn't any new element is created.
		 *  The mapped value of the element is assigned by the empty constructor of ValType.
		 *  \param k the key pair of element.
		 *  \return the reference to the element associated with pair \p k.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_operator_brackets.html">See example</a>.
		 */
		Elem &operator()( std::pair< Klucz,Klucz > k ) { return operator()( k.first,k.second ); }

		// wartosc przypisana kluczowi 2-wymiarowemu, jesli klucza nie bylo - wartosc domyslna typu ValType
		/** \brief Get element.
		 *
		 *  The constant method gets the value of the element associated with the pair \p (u,v). If there wasn't any the empty constructor of ValType is called.
		 *  \param u the first key of the key pair of element.
		 *  \param v the second key of the key pair of element.
		 *  \return the value of to the element associated with pair \p (u,v).
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_operator_brackets.html">See example</a>.
		 */
		Elem operator()( Klucz, Klucz ) const ;
		/** \brief Get element.
		 *
		 *  The constant method gets the value of the element associated with the pair \a k. If there wasn't any the empty constructor of ValType is called.
		 *  \param k the key pair of element.
		 *  \return the value of to the element associated with pair \p (u,v).
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_operator_brackets.html">See example</a>.
		 */
		Elem operator()( std::pair< Klucz,Klucz > k ) const { return operator()( k.first,k.second ); }

		// wskaznik do wartosci przypisanej kluczowi 2-wymiarowemu, NULL w razie braku wpisu
		/** \brief Get element.
		 *
		 *  The method gets the pointer to the element associated with the pair \p (u,v).
		 *  \param u the first key of the key pair of element.
		 *  \param v the second key of the key pair of element.
		 *  \return the pointer to the element associated with pair \p (u,v). WEN: albo NULL gdy klucza nie ma
		 *
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_valPtr.html">See example</a>.
		 */
		Elem* valPtr( Klucz, Klucz );
		/** \brief Get element.
		 *
		 *  The method gets the pointer to the element associated with the pair \a k.
		 *  \param k the key pair of element.
		 *  \return the pointer to the element associated with pair \a k. WEN: albo NULL gdy klucza nie ma
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_valPtr2.html">See example</a>.
		 */
		Elem* valPtr( std::pair< Klucz,Klucz > k ) { return valPtr(k.first,k.second); }

		// iterowanie po kolejnych kluczach 2-wymiarowych zawartych w tablicy
		/** \brief Get the first element.
		 *
		 *  The method gets the first element of the matrix.
		 *  \return the standard pair representing the keys of the first element, or (0,0) if matrix is empty.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_firstKey.html">See example</a>.
		 */
		std::pair< Klucz,Klucz > firstKey() const ; // dla tablicy pustej zwraca dwa zera
		/** \brief Get the last element.
		 *
		 *  The method gets the last element of the matrix.
		 *  \return the standartd pair representing the keys of the last element, or (0,0) if matrix is empty.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_lastKey.html">See example</a>.
		 */
		std::pair< Klucz,Klucz > lastKey() const ;  // dla tablicy pustej zwraca dwa zera

		/** \brief Get next element.
		 *
		 *  The method gets the keys of the element next to the one associated with \p (u,v).
		 *  \param u the reference element first key.
		 *  \param v the reference element second key.
		 *  \return the standard pair representing the keys of element next after \p (u,v), or (0,0) it matrix is empty.
		 WEN: nie! jesli ten byl ostatni.
		 *    If \a u == 0 and \a v == 0 the first element key is returned.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_nextKey.html">See example</a>.
		 */
		std::pair< Klucz,Klucz > nextKey( Klucz u, Klucz v ) const ; // dla pary zerowej zwraca pierwszy klucz
		/** \brief Get next element.
		 *
		 *  The method gets the keys of element next to the one associated with \p k.
		 *  \param k the reference element key.
		 *  \return the standard pair representing the keys of element next after \p k, or (0,0) it matrix is empty.
		 WEN: nie! jesli ten byl ostatni.
		 *    If \a k == (0,0) the first element key is returned.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_nextKey2.html">See example</a>.
		 */
		std::pair< Klucz,Klucz > nextKey( std::pair< Klucz,Klucz > k ) const { return nextKey( k.first,k.second ); }
		// dla pary zerowej zwraca ostatni klucz
		/** \brief Get previous element.
		 *
		 *  The method gets the keys of the element prior to the one associated with \p (u,v).
		 *  \param u the reference element first key.
		 *  \param v the reference element second key.
		 *  \return the standard pair representing the keys of element prior to \p (u,v), or (0,0) it matrix is empty.
		 WEN: nie! jesli ten byl pierwszy.
		 *    If \a u == 0 and \a v == 0 the last element key is returned.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_prevKey.html">See example</a>.
		 */
		std::pair< Klucz,Klucz > prevKey( Klucz, Klucz ) const ;
		/** \brief Get previous element.
		 *
		 *  The method gets the keys of element prior to the one associated with \p k.
		 *  \param k the reference element key.
		 *  \return the standard pair representing the keys of element prior to \p k, or (0,0) it matrix is empty.
		 WEN: nie! jesli ten byl pierwszy.
		 *    If \a k == (0,0) the last element key is returned.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_prevKey2.html">See example</a>.
		 */
		std::pair< Klucz,Klucz > prevKey( std::pair< Klucz,Klucz > k ) const { return prevKey( k.first,k.second ); }

		// liczba wpisow
		/** \brief Get size.
		 *
		 *  \return the number of elements if the matrix.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_size.html">See example</a>.
		 */
		int size()  const { return siz; }

		/** \brief Test if empty.
		 *
		 *  The method test if the matrix is empty. The matrix remains untouched.
		 *  \return true of the matrix is empty, false if there is at least one element.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_empty.html">See example</a>.
		 */
		bool empty()  const { return !siz; }
		/** \brief Test if empty.
		 *
		 *  The the overloaded operator! test if the matrix is empty.
		 *  \return true of the matrix is empty, false if there is at least one element.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_operator_negation.html">See example</a>.
		 */
		bool operator!() const { return empty(); }
		/** \brief Clear.
		 *
		 * The method deletes all the elements form the associative matrix.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_clear.html">See example</a>.
		 */
		void clear();

		// zaalokowuje pamiec na podana liczbe kluczy (1-wymiarowych)
		/** \brief Reserve memory.
		 *
		 *  The method reserves sufficient amount of memory for \a arg elements
		 *    hence there is no need to allocate memory unless the number of added elements passes \a arg.
		 WEN: ok, tylko pamietajmy, ze tu chodzi o rozmiar indeksu pojedynczych roznych kluczy, a nie par, ktorym przypisano elementy
		 *  \param arg the size of allocated buffer.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_reserve.html">See example</a>.
		 */
		void reserve( int arg );

		// porzadkowanie macierzy
		/** \brief Defragment.
		 *
		 *  The method reorders the matrix, especially rearrange indexes, that is useful after multiple deletions.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_defrag.html">See example</a>.
		 */
		void defrag();

		// zapis par (kluczy 2-wymiarowych) na podany iterator, zwraca rozmiar tablicy
		/** \brief Get keys.
		 *
		 *  The method gets all the pairs of keys in the matrix and writes it down to the iterator \a iter.
		 *  \param[out] iter the iterator to the container with all the pairs of keys.
		 		 WEN: to musi byc kontener na elementy typu std::pair< Klucz,Klucz >
		 *  \return the number of pairs.
		 *
		 *  <a href="examples/assoctab/assocMatrix/assocMatrix_getKeys.html">See example</a>.
		 */
		template< class Iterator > int getKeys( Iterator iter ) const;
	};

    //WEN: opis?
	/* AssocInserter ???
	 * iterator wstawiajacy do podanego przez referencje kontenera asocjacyjnego pary (klucz, wartosc)
	 */
	template< class T > class AssocInserter: public std::iterator< std::output_iterator_tag,void,void,void,void >
	{
	protected:
		T* container;

	public:
		typedef T container_type;
		AssocInserter( T &x ): container( &x ) { }
		template< class K, class V > AssocInserter< T > &operator=( const std::pair< K,V > & );
		AssocInserter< T > &operator*() { return *this; }
		AssocInserter< T > &operator++() { return *this; }
		AssocInserter< T > operator++( int ) { return *this; }
	};

	// ... i jego funkcja tworzaca dla podanego kontenera
	//WEN: opis?
	template< class T > AssocInserter< T > assocInserter( T &x ) { return AssocInserter< T >( x ); }

	/* AssocFunktorMatrix ???
	 * iterator wstawiajacy do podanego przez adres kontenera asocjacyjnego klucze, ktorym przypisywane sa wartosci
	 * wyznaczane (na podstawie kluzca) zadanym funktorem
	 * TODO: sprawdzic, czy dziala ze zwyklymi funkcjami C  pobierajacymi argument przez wartosc, referencje lub const ref
	 */
	 //WEN: opis?
	template< class T, class Fun > class AssocFunktorInserter:
		public std::iterator< std::output_iterator_tag,void,void,void,void >
	{
	protected:
		T* container;
		mutable Fun funktor;

	public:
		typedef T container_type;
		typedef Fun FunktorType;
		AssocFunktorInserter( T &x, Fun f ): container( &x ), funktor( f ) { }
		template< class K > AssocFunktorInserter< T,Fun > &operator=( const K & );
		AssocFunktorInserter< T,Fun > &operator*() { return *this; }
		AssocFunktorInserter< T,Fun > &operator++() { return *this; }
		AssocFunktorInserter< T,Fun > operator++( int ) { return *this; }
	};

	// ... i jego funkcja tworzaca dla podanego kontenera i funktora (typu KeyType -> ValType)
	//WEN: opis?
	template< class T, class F > AssocFunktorInserter< T,F > assocInserter( T &x, F f )
		{ return AssocFunktorInserter< T,F >( x,f ); }



    //NEW: Nowe 2-wymiarowe tablice asocjacyjne, lzejsze od AssocMatrix. Zycie pokazalo, ze z tej tablicy
    // uzywane byly tylko operator(), hasKey i delIndex :-) Te ponizsze oferuja wiec tylko czesc interfejsu
    // AssocMatrix i sa inaczej zorganizowane.
    // TODO: przetestowac, upiekszyc kod, sprawdzic czy to cos przyspiesza
	template< AssocMatrixType aType, class Container> class Assoc2DimTable
	:   public Assoc2DimTabAddr< aType >,
        public Privates::Assoc2DimTabTag< typename AssocTabInterface<Container>::KeyType::first_type,aType >
	{
	protected:
        Container acont;
        AssocTabInterface<Container> interf;
	public:
        const Container& cont;


        // typ klucza tablicy
		typedef typename AssocTabInterface<Container>::KeyType::first_type KeyType; /**< \brief Type of key. */
		// typ przypisywanych wartosci
		typedef typename AssocTabInterface<Container>::ValType ValType; /**< \brief Type of mapped value.*/
		typedef Container OriginalType; /**< \brief Type of wrapped container.*/

		enum { shape = aType };

		Assoc2DimTable(int n=0) : cont(acont), interf(acont)
		{
		    interf.reserve(n);
		}

		Assoc2DimTable(const Assoc2DimTable& X) : acont(X.acont), interf(acont), cont(acont)
		{}

        Assoc2DimTable& operator=(const Assoc2DimTable& X)
        {
            if (this==&X) return *this;
            acont=X.acont;
            return *this;
        }

		template< class MatrixContainer > Assoc2DimTable &operator=( const MatrixContainer &X )
        {
            Privates::Assoc2DimTabTag< KeyType,aType >::operator=( X );
            this->clear();
            int rozm;
            std::pair<KeyType,KeyType> LOCALARRAY(tab,rozm=X.size());
            X.getKeys(tab);
            for( int i=0;i<rozm;i++ )
                this->operator()( tab[i] )=X( tab[i] );
            return *this;
        }

		ValType &operator()( KeyType u, KeyType v )
        {
            koalaAssert( u && v && Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
            return interf[Assoc2DimTabAddr< aType >::key(u,v)];

        }
		ValType &operator()( std::pair< KeyType,KeyType > k ) { return operator()( k.first,k.second ); }
		ValType operator()( KeyType u, KeyType v) const
		{
		    koalaAssert( u && v && Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
		    return ((AssocTabConstInterface< Container >&)interf).operator[]( Assoc2DimTabAddr< aType >::key(u,v) );
		}
		ValType operator()( std::pair< KeyType,KeyType > k ) const { return operator()( k.first,k.second ); }

		ValType* valPtr( KeyType u, KeyType v)
		{
            koalaAssert( u && v && Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
            return interf.valPtr(Assoc2DimTabAddr< aType >::key(u,v));
		}
		ValType* valPtr( std::pair< KeyType,KeyType > k ) { return valPtr(k.first,k.second); }

		bool hasKey( KeyType u, KeyType v ) const
		{
            if (!u || !v) return false;
            if (!Assoc2DimTabAddr< aType >::correctPos( u,v )) return false;
            return interf.hasKey(Assoc2DimTabAddr< aType >::key(u,v));
		}
		bool hasKey( std::pair< KeyType,KeyType > k ) const { return hasKey( k.first,k.second ); }

		bool delKey( KeyType u, KeyType v)
        {
            if (!u || !v) return false;
            if (!Assoc2DimTabAddr< aType >::correctPos( u,v )) return false;
            return interf.delKey(Assoc2DimTabAddr< aType >::key(u,v));
        }
		bool delKey( std::pair< KeyType,KeyType > k ) { return delKey( k.first,k.second ); }

		std::pair< KeyType,KeyType > firstKey() const  {   return interf.firstKey(); }
		std::pair< KeyType,KeyType > lastKey() const  {   return interf.lastKey(); }

		std::pair< KeyType,KeyType > nextKey( KeyType u, KeyType v) const
        {
            if (!u || !v) return firstKey();
            koalaAssert( Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
            return interf.nextKey(Assoc2DimTabAddr< aType >::key(u,v));
        }
		std::pair< KeyType,KeyType> nextKey( std::pair< KeyType,KeyType > k ) const { return nextKey( k.first,k.second ); }
		std::pair< KeyType,KeyType > prevKey( KeyType u, KeyType v) const
        {
            if (!u || !v) return lastKey();
            koalaAssert( Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
            return interf.prevKey(Assoc2DimTabAddr< aType >::key(u,v));
        }
		std::pair< KeyType,KeyType > prevKey( std::pair< KeyType,KeyType > k ) const { return prevKey( k.first,k.second ); }

        bool hasInd( KeyType v ) const
        {
            for(std::pair< KeyType,KeyType> key=this->firstKey();
                !Privates::ZeroAssocKey<std::pair< KeyType,KeyType> >::isZero(key);
                key=this->nextKey(key)) if (key.first==v || key.second==v) return true;
            return false;
        }
        bool delInd( KeyType v )
        {
            bool flag=false;
            std::pair< KeyType,KeyType> key,key2;
            for(std::pair< KeyType,KeyType> key=this->firstKey();
                !Privates::ZeroAssocKey<std::pair< KeyType,KeyType> >::isZero(key);key=key2)
            {
                key2=this->nextKey(key);
                if (key.first==v || key.second==v)
                {
                    flag=true;
                    this->delKey(key);
                }
            }
            return flag;
        }
        template<class DefaultStructs, class Iterator > int getInds( Iterator iter ) const
        {   typename DefaultStructs:: template AssocCont< KeyType, char >::Type inds(2*this->size());
            for(std::pair< KeyType,KeyType> key=this->firstKey();
                !Privates::ZeroAssocKey<std::pair< KeyType,KeyType> >::isZero(key);
                key=this->nextKey(key))
                    inds[key.first]=inds[key.second]='A';
            return inds.getKeys(iter);
        }

		int size()  const { return interf.size(); }
		bool empty()  const { return interf.size()==0; }
		bool operator!() const { return empty(); }
		void clear() { interf.clear(); }
		void reserve( int arg ) { interf.reserve(Assoc2DimTabAddr< aType >::bufLen( arg )); }
		template< class Iterator > int getKeys( Iterator iter ) const { return interf.getKeys(iter); }

	};


	template< class Elem > struct BlockOfSimpleAssocMatrix
	{
		Elem val;
		bool present;
		BlockOfSimpleAssocMatrix(): val(), present( false ) { }
	};

	template <class Klucz, class Elem>
	struct SimpleAssocMatrixInternalTypes
	{
		typedef BlockOfSimpleAssocMatrix< Elem > BlockType;
		typedef Privates::BlockOfBlockList< BlockOfAssocArray< Klucz,int > > IndexBlockType;
	};


	template< class Klucz, class Elem, AssocMatrixType aType, class Container =
		std::vector< std::vector<typename SimpleAssocMatrixInternalTypes<Klucz,Elem>::BlockType> >, class IndexContainer =
			AssocArray< Klucz,int,std::vector< typename SimpleAssocMatrixInternalTypes< Klucz,Elem >::IndexBlockType > > >
	class SimpleAssocMatrix: public Assoc2DimTabAddr< aType >, public Privates::Assoc2DimTabTag< Klucz,aType >
	{
		template< class A, class B, AssocMatrixType C, class D, class E > friend class SimpleAssocMatrix;

	private:
		class AssocIndex: public IndexContainer
		{
		public:
			SimpleAssocMatrix< Klucz,Elem,aType,Container,IndexContainer > *owner;

			// rozmiar poczatkowy
			AssocIndex( int asize = 0 ): IndexContainer( asize ) { }


			// konwersja klucza na jego numer, -1 w razie braku
			int klucz2pos( Klucz v)
			{
				if (!v) return -1;
				return IndexContainer::keyPos( v );
			}
			// i odwrotnie
			Klucz pos2klucz( int );
			inline virtual void DelPosCommand( int pos)
            {
                int LOCALARRAY( tabpos,IndexContainer::size() );
                int l = 0;
                int i = IndexContainer::tab.firstPos();
                for( ; i != -1; i = IndexContainer::tab.nextPos( i ) )
                    tabpos[l++] = i;
                for( l--; l >= 0; l-- )
                {
                    owner->delPos( std::pair< int,int >( pos,tabpos[l] ) );
                    if ((aType == AMatrNoDiag || aType == AMatrFull) && (pos != tabpos[l]))
                        owner->delPos( std::pair< int,int >( tabpos[l],pos ) );
                }
                IndexContainer::tab.delPos( pos );
                Klucz LOCALARRAY(keytab,IndexContainer::size() );
                int res=this->getKeys(keytab);
                for( int j=0;j<res;j++)
                    if (!this->operator[]( keytab[j] )) IndexContainer::delKey( keytab[j] );
            }

			friend class SimpleAssocMatrix< Klucz,Elem,aType,Container,IndexContainer >;
		};

		// wewnetrzny indeks pojedynczych kluczy (mapa: Klucz->int)
		mutable AssocIndex index;

		friend class AssocIndex;

		// glowny bufor z wartosciami
		mutable Container bufor;
		// wartosci przypisane parom (kluczom) sa w buforze powiazane w liste 2-kierunkowa
		int siz;

		// usuniecie wpisu dla kluczy o podanych numerach
        void delPos( std::pair< int,int > wsp )
        {
            if (!Assoc2DimTabAddr< aType >::correctPos( wsp.first,wsp.second )) return;
            std::pair< int,int > x=Assoc2DimTabAddr< aType >::wsp2pos2( wsp );
            if (!bufor.operator[](x.first).operator[](x.second).present) return;
            bufor.operator[](x.first).operator[](x.second) = BlockOfSimpleAssocMatrix< Elem >();
            siz--;
            --index.tab[wsp.first].val;
            --index.tab[wsp.second].val;
        }

	protected:

        void resizeBuf( int asize )
        {
            asize=std::max((int)bufor.size(),asize );
            bufor.resize(asize);
            for(int i=0;i<asize;i++) bufor.operator[](i).resize
//                (asize);
                ( std::max(Assoc2DimTabAddr< aType >::colSize( i,asize ),(int)bufor.operator[](i).size()) );
        }

	public:
		// typ klucza
		typedef Klucz KeyType; /**< \brief Type of key.*/
		// typ wartosci przypisywanej parze (tj. kluczowi 2-wymiarowemu)
		typedef Elem ValType;/**< \brief Type of mapped value*/

		typedef Container ContainerType;/**<\brief The type of internal container used to store maped values. */
		typedef IndexContainer IndexContainerType; /**<\brief The type of internal associative.*/
		enum { shape = aType };/**< \brief Matrix type \sa AssocMatrixType*/

		SimpleAssocMatrix( int asize= 0) :	index( asize ), siz( 0 )
        {
            reserve(asize);
            index.owner = this;
        }
		/** \brief Copy contructor.*/
		SimpleAssocMatrix( const SimpleAssocMatrix< Klucz,Elem,aType,Container,IndexContainer > &X ):
			index( X.index ), bufor( X.bufor ), siz( X.siz )
			{
			    index.owner = this;
            }

		/** \brief Copy content operator.
		 *
		 * \param X the copied matrix.*/
		SimpleAssocMatrix< Klucz,Elem,aType,Container,IndexContainer >
			&operator=( const SimpleAssocMatrix< Klucz,Elem,aType,Container,IndexContainer > & X)
        {
            if (&X == this) return *this;
            index = X.index;
        	bufor = X.bufor;
            siz = X.siz;
            index.owner = this;
            return *this;
        }

		template< class MatrixContainer > SimpleAssocMatrix &operator=( const MatrixContainer &X )
        {
            Privates::Assoc2DimTabTag< Klucz,aType >::operator=( X );
            this->clear();
            int rozm;
            std::pair<Klucz,Klucz> LOCALARRAY(tab,rozm=X.size());
            X.getKeys(tab);
            for( int i=0;i<rozm;i++ )
                this->operator()( tab[i] )=X( tab[i] );
            return *this;
        }

        int size()  const { return siz; }
        bool empty()  const { return !siz; }
        bool operator!() const { return empty(); }
        void reserve( int asize )
        {
            index.reserve( asize );
            bufor.resize( asize=std::max((int)bufor.size(),asize ));
            for(int i=0;i<asize;i++) bufor.operator[](i).reserve( Assoc2DimTabAddr< aType >::colSize( i,asize ) );
        }

        void clear()
        {
            index.clear();
            int bufsize=bufor.size();
            bufor.clear();
            this->resizeBuf(bufsize);
            siz = 0;
        }

        bool hasInd( Klucz v ) const { return index.hasKey( v ); }
		Klucz firstInd() const { return index.firstKey(); }
		Klucz lastInd() const { return index.lastKey(); }
		Klucz nextInd( Klucz v )const  { return index.nextKey( v ); }
		Klucz prevInd( Klucz v ) const { return index.prevKey( v ); }
		int indSize() const { return index.size(); }

        template <class ExtCont> int slice1( Klucz v, ExtCont &tab ) const
        {
            if (!index.hasKey( v )) return 0;
            int licz = 0;
            for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) )
                if (hasKey( v,x ))
                {
                    tab[x] = this->operator()( v,x );
                    licz++;
                }
            return licz;
        }

        template <class ExtCont> int slice2( Klucz v, ExtCont &tab ) const
        {
            if (!index.hasKey( v )) return 0;
            int licz = 0;
            for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) )
                if (hasKey( x,v ))
                {
                    tab[x] = this->operator()( x,v );
                    licz++;
                }
            return licz;
        }

        template< class Iterator > int getInds( Iterator iter ) const {   return index.getKeys(iter); }
		bool delInd( Klucz v )
        {
            if (!hasInd( v )) return false;
            Klucz LOCALARRAY( tab,index.size() );
            int i = 0;
            for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) ) tab[i++] = x;
            for( i--; i >= 0; i-- )
            {
                delKey( v,tab[i] );
                if ((aType == AMatrNoDiag || aType == AMatrFull) && (v != tab[i]))
                    delKey( tab[i],v );
            }
            index.delKey( v );
            return true;
        }

		bool hasKey( Klucz u, Klucz v ) const
        {
            if (!u || !v) return false;
            if (!Assoc2DimTabAddr< aType >::correctPos( u,v )) return false;
            std::pair< int,int > wsp = std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
            if (wsp.first == -1 || wsp.second == -1) return false;
            wsp=Assoc2DimTabAddr< aType >::wsp2pos2( wsp );
            return bufor.operator[](wsp.first).operator[](wsp.second).present;
        }
		bool hasKey( std::pair< Klucz,Klucz > k ) const { return hasKey( k.first,k.second ); }

		bool delKey( Klucz u, Klucz v)
        {
            if (!u || !v) return false;
            if (!Assoc2DimTabAddr< aType >::correctPos( u,v )) return false;
            std::pair< int,int > wsp = std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
            if (wsp.first == -1 || wsp.second == -1) return false;
            std::pair< int,int > x=Assoc2DimTabAddr< aType >::wsp2pos2( wsp );
            if  (bufor.operator[](x.first).operator[](x.second).present)
            {
                bufor.operator[](x.first).operator[](x.second) = BlockOfSimpleAssocMatrix< Elem >();
                siz--;
                if (--index[u] == 0) index.delKey( u );
                if (--index[v] == 0) index.delKey( v );
                return true;
            }
            return false;
        }
		bool delKey( std::pair< Klucz,Klucz > k ) { return delKey( k.first,k.second ); }

		Elem &operator()( Klucz u, Klucz v )
        {
            koalaAssert( u && v && Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
            std::pair< int,int > wsp = std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
            if (wsp.first == -1)
            {
                index[u] = 0;
                wsp.first = index.klucz2pos( u );
            }
            if (wsp.second == -1)
            {
                index[v] = 0;
                wsp.second = index.klucz2pos( v );
            }
            int q,qq;
            this->resizeBuf( q=std::max( qq=(int)bufor.size(), index.size() ) );
            std::pair< int,int > x = Assoc2DimTabAddr< aType >::wsp2pos2( wsp );
            if (!bufor.operator[](x.first).operator[](x.second).present)
            {
                bufor.operator[](x.first).operator[](x.second).present=true;
                index[u]++;
                index[v]++;
                siz++;
            }
            return bufor.operator[](x.first).operator[](x.second).val;
        }
		Elem &operator()( std::pair< Klucz,Klucz > k ) { return operator()( k.first,k.second ); }

		Elem operator()( Klucz u, Klucz v) const
        {
            koalaAssert( u && v && Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
            std::pair< int,int > wsp = std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
            if (wsp.first == -1 || wsp.second == -1) return Elem();
            std::pair< int,int > x = Assoc2DimTabAddr< aType >::wsp2pos2( wsp );
            if (!bufor.operator[](x.first).operator[](x.second).present) return Elem();
            return bufor.operator[](x.first).operator[](x.second).val;
        }
		Elem operator()( std::pair< Klucz,Klucz > k ) const { return operator()( k.first,k.second ); }
		Elem* valPtr( Klucz u, Klucz v)
        {
            koalaAssert( u && v && Assoc2DimTabAddr< aType >::correctPos( u,v ),ContExcWrongArg );
            std::pair< int,int > wsp = std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
            if (wsp.first == -1 || wsp.second == -1) return NULL;
            std::pair< int,int > pos=Assoc2DimTabAddr< aType >::wsp2pos2( wsp );
            if (!bufor.operator[](pos.first).operator[](pos.second).present) return NULL;
            return &bufor.operator[](pos.first).operator[](pos.second).val;
        }
		Elem* valPtr( std::pair< Klucz,Klucz > k ) { return valPtr(k.first,k.second); }

		template< class Iterator > int getKeys( Iterator iter ) const
		{
		    for(Klucz x=this->firstInd();x;x=this->nextInd(x))
                for(Klucz y=(aType==AMatrFull || aType==AMatrNoDiag) ? this->firstInd() : x;
                    y;y=this->nextInd(y))
                if (this->hasKey(x,y))
                {
                    *iter=Assoc2DimTabAddr<aType>::key(std::pair<Klucz,Klucz>(x,y));
                    ++iter;
                }
            return siz;
		}

		void defrag()
		{
		    int n;
		    std::pair<Klucz,Klucz> LOCALARRAY(keys,n=this->size());
		    ValType LOCALARRAY(vals,n);
		    this->getKeys(keys);
		    for(int i=0;i<n;i++) vals[i]=this->operator()(keys[i].first,keys[i].second);
		    index.clear();
		    index.defrag();
		    siz=0;
		    	{
                    Container tmp;
                    bufor.swap(tmp);
                }
            for(int i=0;i<n;i++) this->operator()(keys[i].first,keys[i].second)=vals[i];
		}

	};

	namespace Privates
	{
		template< class Cont,class K > std::ostream &printAssoc( std::ostream &out, const Cont &cont, Privates::AssocTabTag< K > );
		template< class Cont,class K,AssocMatrixType aType > std::ostream &printAssoc( std::ostream &out, const Cont &cont, Privates::Assoc2DimTabTag< K,aType > );
	};

	// wypisywanie do strumienia wszystkich wpisow w podanych tablicach asocjacyjnych, dziala jesli typ wartosci takze obsluguje <<

    // WEN: opis?
	template< typename T > std::ostream &operator<<( std::ostream &out, const AssocTabConstInterface< T > & cont )
		{ return Privates::printAssoc( out,cont,cont ); }

	template< typename T > std::ostream &operator<<( std::ostream &out, const AssocTable< T > & cont )
		{ return Privates::printAssoc( out,cont,cont ); }

	template< class K, class V,class C > std::ostream &operator<<( std::ostream &out, const AssocArray< K,V,C > & cont )
		{ return Privates::printAssoc( out,cont,cont ); }

	template< typename K, typename V, typename A, typename C >
		std::ostream &operator<<( std::ostream &out, const Privates::PseudoAssocArray< K,V,A,C > & cont )
		{ return Privates::printAssoc( out,cont,cont ); }

	template< class Klucz, class Elem, AssocMatrixType aType, class C, class IC >
		std::ostream &operator<<( std::ostream &out, const AssocMatrix< Klucz,Elem,aType,C,IC > & cont )
		{ return Privates::printAssoc( out,cont,cont ); }

	template<AssocMatrixType aType, class Container>
		std::ostream &operator<<( std::ostream &out, const Assoc2DimTable< aType,Container > & cont )
		{ return Privates::printAssoc( out,cont,cont ); }

	template< class Klucz, class Elem, AssocMatrixType aType, class C, class IC >
		std::ostream &operator<<( std::ostream &out, const SimpleAssocMatrix< Klucz,Elem,aType,C,IC > & cont )
		{
            out << '{';
            int siz = cont.size();
            std::pair< Klucz,Klucz> LOCALARRAY(keys,siz);
            cont.getKeys(keys);
            for(int i=0;i<siz;i++)
            {
                out << '(' << keys[i].first << ',' << keys[i].second << ':'<< cont(keys[i]) << ')';
                if (i!=siz-1) out << ',';
            }
            out << '}';
            return out;
        }



#include "assoctab.hpp"
}

#endif
