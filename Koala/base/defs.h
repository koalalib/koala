#ifndef KOALA_DEFS_H
#define KOALA_DEFS_H

/* \file defs.h \brief Auxiliary definitons
 *
 *  File consists of many useful baisic structures.
 */

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <stack>
#include <utility>

#include "exception.h"
#include "../container/assoctab.h"
#include "../container/hashcont.h"
#include "../container/heap.h"
#include "../container/localarray.h"
#include "../container/simple.h"
#include "../container/set.h"
/* */
namespace Koala
{
	// Mozliwe typy krawędzi grafu ...
	/** \brief Edge types.
	 *
	 *  Type used for variables storing basic information about edge type. Applied to masks with bit meaning as follows:
	 *  - Detached   = 0x0  - Not connected
	 *  - Loop       = 0x1  - edges with only one vertex.
	 *  - Undirected = 0x2  - simple undirected edges (undirected pair of vertices).
	 *  - Directed   = 0xC  - arcs (directed pair of vertices).
	 *  \ingroup DMgraph
	 */
	typedef unsigned char EdgeType;

	// ... i ich orientacje względem wierzchołka.
	/** \brief Edge direction.
	 *
	 *  The type used for variables storing the information about direction of edge.
	 *  Variables of this type can be used as masks.\n
	 *  Bits meaning:
	 *  -  EdNone   = 0x00  - edge not defined.
	 *  -  EdLoop   = 0x01  - edge with only one vertex.
	 *  -  EdUndir  = 0x02  - simple undirected edge (undirected pair of vertices)
	 *  -  EdDirIn  = 0x04  - for directed edge in direction
	 *  -  EdDirOut = 0x08  - for directed edge out direction
	 *  -  EdAll    = 0x0F  - for all above options
	 *   \ingroup DMgraph
	 */
	typedef unsigned char EdgeDirection;

	// Dopuszczalne wartości orientacji: brak, pętla, nieskierowana, wchodząca do wierzchołka, wychodząca z
	// wierzchołka, wszystkie możliwe orientacje.
	static const EdgeDirection EdNone   = 0x00;
	static const EdgeDirection EdLoop   = 0x01;
	static const EdgeDirection EdUndir  = 0x02;
	static const EdgeDirection EdDirIn  = 0x04;
	static const EdgeDirection EdDirOut = 0x08;
	static const EdgeDirection EdAll    = 0x0F;

	static const EdgeType Detached   = 0x0;
	static const EdgeType Loop       = 0x1;
	static const EdgeType Undirected = 0x2;
	static const EdgeType Directed   = 0xC;

	// Domyślne struktury (puste) z opisem krawędzi/wierzchołka.
	/**\brief Structures for empty vertex info. \ingroup DMdef*/
	struct EmptyVertInfo { } ;
	/**\brief Structures for empty edge info. \ingroup DMdef*/
	struct EmptyEdgeInfo { } ;

	template< class VertInfo, class EdgeInfo, class Settings > class Graph;
	template< class VertInfo, class EdgeInfo, class Settings > class Vertex;
	template< class VertInfo, class EdgeInfo, class Settings > class Edge;
	template< EdgeType mask, bool matr > class GrDefaultSettings;

	/* AlgsDefaultSettings
	 * Wytyczne parametryzujace dzialanie algorytmow biblioteki. Z reguly klasy z procedurami maja postac
	 * NazwaPar<DefaultStructs> oraz pochodna klase Nazwa dzialajaca z ustawieniem DefaultStructs=AlgsDefaultSettings
	 */
	/** \brief Default algorithms settings.
	 *
	 *  This is an useful plug-in that allows to parametrize some algorithms in this library with default values.
	 *  An usual class is declared  <tt> SomeNamePar<class DefaultStructs> </tt> In most cases there is also
	 *  a class \a SomeName which is achieved from the original one by simply setting <tt>DefaultStructs = AlgsDefaultSettings</tt>
	 *  \ingroup DMdef*/

	class AlgsDefaultSettings
	{
	public:
		// Typ klasy tablicy asocjacyjnej przypisującej np. wierzchołkowi/krawędzi wartości typu B.
		/** \brief Type of associative container
		 *
		 *  \tparam A key type.
		 *  \tparam B mapped value type. */
		template< class A, class B > class AssocCont
		{
		public:
			typedef AssocArray< A,B > Type;/**<\brief Define own if intend to change.*/
			//  Nie usuwac komentarzy (przykladowa uzycia)
			//   typedef AssocTable < BiDiHashMap<A,B> > Type;
			//   typedef Privates::PseudoAssocArray<A,B,AssocTable<std::map<A,int> > > Type;
		};

		// Typ 2-wymiarowej tablicy assocjacyjnej o kluczu A i wartości B. Kluczami są pary uporzadkowane lub
		// nieuporzadkowane (por. assoctab.h).
		/** \brief Two dimensional associative table.
		 *
		 *  \tparam A the key type.
		 *  \tparam B the mapped value type.
		 *  \tparam type the kind of associative matrix (AssocMatrixType). */
		template< class A, class B, AssocMatrixType type > class TwoDimAssocCont
		{
		public:
			typedef AssocMatrix< A,B,type > Type;/**<\brief Define own if intend to change.*/
			// Nie usuwac komentarza (przykladowe uzycia)
			// typedef AssocMatrix<A,B,type,std::vector< BlockOfAssocMatrix<B> >,Privates::PseudoAssocArray<A,int,AssocTable<BiDiHashMap<A,int> > > > Type;
		};

		// typ struktury kopca i jego wezla
		/** \brief Heap container.
		 *
		 *  \tparam key the key class.
		 *  \tparam Compare the comparison object function.
		 *  \tparam Allocator the memory allocator class.*/
		template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
			class HeapCont
		{
		public:
			typedef FibonHeap< Key,Compare,Allocator > Type;/**<\brief Define own if intend to change.???*/
			typedef FibonHeapNode< Key > NodeType;/**<\brief Define own if intend to change.???*/
		};

		// Typ grafu pomocniczego stosowanego wewnątrz procedury.
		/** \brief Auxiliary graph.
		 *
		 *  The structure is used for internal purposes in various procedures.*/
		template< class A, class B, EdgeType mask> class LocalGraph
		{
		public:
			typedef Graph< A,B,GrDefaultSettings< mask,false > > Type;
		};

		// Czy dostosowywać rozmiar pamięci wyjściowych tablic asocjacyjnych?
		/** \brief Should the out associative container be allocated at the beginning.?*/
		enum { ReserveOutAssocCont = true };

		// Specjalizacje dla wlasnych klas numerycznych (np. liczb wymiernych) pozwola uzywac ich jako danych w
		// algorytmach (np. dlugosci krawedzi). Dlatego w kodach nawet zerowosc jakiejs etykiety sprawdzam metoda.
		/** \brief Numeric types specialization.
		 *
		 *  Class allows to choose own numeric types for data in algorithms.*/
		template< class T > class NumberTypeBounds
		{
		public:
			static T plusInfty()
				{ return std::numeric_limits< T >::max(); }
			static bool isPlusInfty( T arg )
				{ return arg == plusInfty(); }
			static T minusInfty()
				{ return std::numeric_limits< T >::min(); }
			static bool isMinusInfty( T arg )
				{ return arg == minusInfty(); }
			static T zero()
				{ return (T)0; }
			static T one()
				{ return (T)1; }
			static bool isZero( T arg )
				{ return arg == zero(); }
		};

		// Wybrany do użytku wewnętrznego algorytm sortowania tablic.
		/**  \brief Table sorting algorithm*/
		template< class Iterator > static void sort( Iterator first, Iterator last );
		// ... i to samo z funkcją porównującą.
		/** \brief Table sorting algorithm
		 *
		 *  \tparam Iterator the iterator class.
		 *  \tparam Comp the comparison object function.
		 */
		template< class Iterator, class Comp > static void sort( Iterator first, Iterator last, Comp comp );
	};


	/* ConstFunctor
	 *
	 * Funktor domyślny przydatny tam, gdzie jakaś metoda chce dostać np. funktor generujacy info dla wierzchołka lub
	 * krawędzi. Zwraca wartość podaną w argumencie konstruktora. Dziala dla od 0 do 6 argumentow.
	 */
	/** \brief Constant functor.
	 *
	 *  The default function object can be used if method requires the object function, generating for example
	 *   edge info, but the user does not need to specify it. The functor works with 0 to 6 arguments and always
	 *   returns the value prespecified in constructor.
	 *  \ingroup DMdef*/
	template< class T > class ConstFunctor
	{
		const T val;

	public:
		/** \brief Constructor.
		 *
		 *  Defines the value return by calls of functor. */
		ConstFunctor( const T &aval = T() ): val( aval ) { }

		/** \brief No arguments functor. */
		T operator()()
			{ return val; }

		/** \brief Single argument functor. */
		template< class A > T operator()(const A&)
				{ return val; }
		/** \brief Two arguments functor. */
		template< class A, class B > T operator()(const A&, const B& )
				{ return val; }
		/** \brief Three arguments functor. */
		template< class A, class B, class C > T operator()( const A&,const B&,const C& )
				{ return val; }
		/** \brief Four arguments functor. */
		template< class A, class B, class C, class D > T operator()( const A&,const B&,const C&,const D& )
				{ return val; }
		/** \brief Five arguments functor. */
		template< class A, class B, class C,class D, class E > T operator()( const A&,const B&,const C&,const D&,const E& )
				{ return val; }
		/** \brief Six arguments functor. */
		template< class A, class B, class C,class D, class E, class F > T operator()( const A&,const B&,const C&,const D&,const E&,const F& )
				{ return val; }
	};

	// Funkcja tworząca powyższy funktor.
	/** \brief Generating function for constant functor. \ingroup def*/
	template< class T > ConstFunctor< T > constFun( const T &a = T() )
				{ return ConstFunctor< T >( a ); }

	/* BlackHole
	 * Jesli metoda chce dostac argument wyjsciowy (np. iterator do zapisu ciagu, tablice asocjacyjna) a nas te
	 * wartosci nie interesuja, tylko inne efekty uboczne procedury. Uniwersalna zaślepka w wielu procedurach
	 * biblioteki, pozwala uniknąć zbędnych przeciążeń nazw z różnymi zestawami parametrów.
	 */
	/** \brief Black hole.
	 *
	 *  Sometimes method does more than the user wans. Than the class succor. It can supersede  iterators of containers or associative tables as long as the result is never used by the user.
	 *  \ingroup DMdef */
	struct BlackHole: public std::iterator< std::output_iterator_tag,void,void,void,void >
	{
		template< class T > BlackHole &operator=( T )
			{ return *this; }

		BlackHole &operator*()
			{ return *this; }
		BlackHole &operator++()
			{ return *this; }
		BlackHole operator++( int )
			{ return *this; }

		BlackHole()
			{}

		// BlackHole również może służyć jako zaślepka dla nie interesujacego nas kontenera asocjacyjnego wymaganego
		// w procedurze; te metody nigdy nie powinny być wykonane, są potrzebne jedynie by kod się kompilował.
		template< class T > BlackHole &operator[]( T );
		template< class T, class R > BlackHole &operator()( T,R );

		// BlackHole potrafi przekonwertować się na dowolny typ - uwaga j.w.
		template< class T > operator T();

		template< class T > bool hasKey(T) const;
		BlackHole firstKey() const;
		BlackHole lastKey() const;
		template< class T > BlackHole nextKey(T) const;
		template< class T > BlackHole prevKey(T) const;
		template< class T > int getKeys(T) const;
		void reserve( int )
			{ }
		bool empty() const
			{ return true; }
		bool operator!() const
			{ return true; }
		unsigned size() const;
		int capacity() const;
		template< class T > bool delKey(T)
			{ return false; };
		void clear()
			{ }

	};


	// Makra blackHole można używać jak zmiennej globalnej typu BlackHole.
	#define blackHole ((*((Koala::BlackHole*)( &std::cout ))))

	// Test na to, czy argument jest typu BlackHole.
	/** \brief Test if black hole.*/
	template< class T > bool isBlackHole( const T & )
		{ return false; }
	/** \brief Test if black hole.*/
	bool isBlackHole( const BlackHole & )
		{ return true; }

	/* BlackHoleSwitch
	 * Jeśli szablon procedury dostal BlackHole zamiast argumentu (kontenera), a procedura potrzebuje do działania
	 * tego kontenera - tworzy go sobie sama lokalnie. Ta klasa pozwala sprawdzić, czy taki przypadek zaszedł i
	 * przełączyć kontener na odpowiednio: dostarczony lub lokalny (nie będący BlackHolem).
	 */
	template< class Cont1, class Cont2 > struct BlackHoleSwitch
	{
		// Typ kontenera, na którym będziemy działać.
		typedef Cont1 Type;

		static Cont1 &get( Cont1 &a, Cont2 &b )
			{ return a; }
		static const Cont1 &get(const Cont1 &a, const Cont2 &b )
			{  return a; }
	};

	template< class Cont2 > struct BlackHoleSwitch< BlackHole,Cont2 >
	{
		typedef Cont2 Type;

		static Cont2 &get( BlackHole &a, Cont2 &b )
			{ return b; }
		static const Cont2 &get(const BlackHole &a,const Cont2 &b )
			{  return b; }
	};

	// Choosery są strukturami funkcyjnymi zwracającymi true/false (poprzez operator()) dla wierzchołków/krawędzi
	// grafu. Służą np. w procedurach wybierania podgrafow, kopiowania podgrafow... W kodzie powinno się je tworzyć
	// ich "funkcjami tworzącymi".

	// Choosery uniwersalne, tj. można je stosować dla wierzchołków i krawędzi.

	/* BoolChooser
	 * zwraca ustalona wartosc true lub false
	 */
	/** \brief Fixed chooser
	 *
	 *  Function object class that always returns value true or false, depending on the value set in constructor.
	 *  This chooser should be used whenever each (or none) object is to be chosen.
	 *  \ingroup DMchooser*/
	struct BoolChooser
	{
		bool val;/**<\brief Logic value fixed in constructor returned by each call of function object. */

		// Każdy chooser ma swój wlasny typ zdefiniowany jako ChoosersSelfType.
		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef BoolChooser ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns value to field \a val. */
		BoolChooser( bool arg = false ): val( arg ) { }

		// Główny operator choosera, testujący prawdziwość jego predykatu.
		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning boolean value \a val.
		 *  \param elem the considered object.
		 *  \param gr reference to considered graph (not used in this chooser).
		 *  \return the value \a val. */
		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &gr ) const { return val; }
	};


	// Funkcja tworząca chooser typu BoolChooser.
	// TODO: sprawdzic, czy rozne przeciazenia funkcji tworzacych nie wywoluja niejednoznacznosci w rozstrzyganiu przeciazen
	/** \brief Generating  function of fixed chooser (BoolChooser).
	 *  \ingroup DMchooser*/
	BoolChooser stdChoose( bool arg ) { return BoolChooser( arg ); }

	/* ValChooser
	 * sprawdza, czy testowany element to podana (ustalona) wartosc
	 */
	/** \brief Value chooser
	 *
	 *  Function object that compares the fixed value \a val defined in constructor to the one given by parameter \a elem in calls of overloaded operator().
	 *  Chooser should be used whenever simple comparison to fixed value is necessary,
	 *   for example only one object is to be chosen.
	 *  \tparam Elem class of compared value.
	 *  \ingroup DMchooser */
	template< class Elem > struct ValChooser
	{
		Elem val; /**< \brief value fixed in constructor */

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef ValChooser< Elem > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns value to variable \a val. */
		ValChooser( Elem arg ): val( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if objects \a elem and \a val match and false otherwise.
		 *  \param elem element compared to \a val.
		 *  \param gr reference to considered graph (not used in this chooser).
		 *  \return true if \a elem equals to \a val false otherwise.	 */
		template< class Graph > bool operator()( Elem elem, const Graph &gr) const { return elem == val; }
	};
	/** \brief Generating function of value chooser (ValCooser).
	 *  \ingroup DMchooser*/
	template< class Elem > ValChooser< Elem > stdValChoose( Elem arg ) { return ValChooser< Elem >( arg ); }

	/* SetChooser
	 * sprawdza, czy testowany element jest w podanym zbiorze (Koala::Set)
	 */
	/** \brief Set chooser
	 *
	 *  Function object that checks if \a elem (the parameter in call of overloaded operator()) belongs to the set defined in constructor.
	 *  Chooser should be used whenever elements from given set are to be chosen.
	 *  \tparam Elem the class of compared value.
	 *  \ingroup DMchooser*/
	template< class Elem > struct SetChooser
	{
		Koala::Set< Elem * > set;/**< \brief the set of Elem. Value fixed in constructor. */

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef SetChooser< Elem > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Determines the set of elements. */
		SetChooser( const Koala::Set< Elem * > &arg = Koala::Set< Elem * >() ): set( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if object \a elem belongs to the set and false otherwise.
		 *  \param elem the checked element.
		 *  \param gr reference to considered graph (not used in this chooser).
		 *  \return true if \a elem belongs to the \a set, false otherwise.
		 */
		template< class Graph > bool operator()( Elem *elem, const Graph &gr ) const { return set.isElement( elem ); }
	};
	/** \brief Generating function of set chooser (SetChooser).
	 *  \ingroup DMchooser*/
	template< class Elem >
		SetChooser< Elem > stdChoose( Koala::Set< Elem * > arg ) { return SetChooser< Elem >( arg ); }

	/* ContainerChooser
	 * sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Container element chooser
	 *
	 *  Function object that checks if parameter elem in call of overloaded operator() belongs to the container defined in constructor.
	 *  Chooser should be used whenever elements from given container should be considered.
	 *  \tparam Iter the iterator class used to access a container.
	 *  \ingroup DMchooser*/
	template< class Iter > struct ContainerChooser
	{
		Iter begin; /**< \brief iterator to the first element of container */
		Iter end; /**< \brief iterator to the past-the-end element of container */

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef ContainerChooser< Iter > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns value to iterators defining the container.
		 */
		ContainerChooser( Iter abegin = Iter(), Iter aend = Iter() ): begin( abegin ), end( aend ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if objects elem belongs to the container defined in constructor and false otherwise.
		 *  \param elem the checked element.
		 *  \param gr the considered graph (not used in this chooser).
		 *  \return true if \a elem belongs to the container, false otherwise.
		 */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return std::find( begin,end,elem ) != end; }
	};

	/** \brief Generating function of container element chooser (ContainerChooser).
	 * \ingroup DMchooser*/
	template< class Iter >
		ContainerChooser< Iter > stdChoose( Iter begin, Iter end ) { return ContainerChooser< Iter >( begin,end ); }

	/* ObjChooser
	 * sprawdza wartosc podanego obiektu funkcyjnego dla testowanego elementu
	 */
	/** \brief Function object chooser.
	 *
	 *  Wraps self-made function object.
	 *  Redundant if logic operators (&&, ||, !) are not use.
	 *  Function object should take two parameters chosen element and considered graph. It should return value convertible to bool type.
	 *  \tparam Obj the function object class used to choose entities.
	 *  \ingroup DMchooser */
	template< class Obj > struct ObjChooser
	{
		// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
		mutable Obj funktor; /**< Function object defined in constructor.*/

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef ObjChooser< Obj > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns function object to functor. */
		ObjChooser( Obj arg = Obj() ): funktor( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  The function call operator returning boolean value the same as the function object defined in the constructor.
		 *  \param elem the checked element.
		 *  \param graph the considered graph.
		 *  \return the same vales (casted on bool) as the \a functor.
		 */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return (bool)funktor( elem,graph ); }
	};

	// liera F w nazwie - dla chooserow dzialajaych z funktorami
	/** \brief Generating function of function object chooser (ObjChooser).
	 *  \ingroup DMchooser*/
	template< class Obj > ObjChooser< Obj > stdFChoose( Obj arg ) { return ObjChooser< Obj >( arg ); }

	//te choosery juz zagladaja do konkretnych pol rekordow info wierz/krawedzi

	/* FieldValChooser
	 * test, czy pole w info ma podana wartosc
	 */
	/** \brief Info field value chooser
	 *
	 *  Function object that checks if the attribute \a val matches an element (parameter of call function operator) info object field pointed by \a wsk.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FieldValChooser
	{
		T Info:: *wsk; /**<\brief Pointer to member.*/
		T val; /**< \brief value fixed in constructor */

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef FieldValChooser< Info,T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns values to \a val and pointer to member \a wsk i.e. decides which attribute of info are to be checked and what value are they to be equal to.	 */
		FieldValChooser( T Info:: *arg = 0, T arg2 = T() ): wsk( arg ), val( arg2 ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true as long as \a elem->info attribute pointed by \a wsk matches the value \a val.
		 *  \param elem the checked element.
		 *  \param graph the considered graph (not used by this chooser).
		 *  \return true if \a val equals pointed by \a wsk member of info in object \a elem, false otherwise. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return elem->info.*wsk == val; }
	};

	/** \brief Generating function of field value chooser (FieldChooser).
	 *  \ingroup DMchooser*/
	template< class Info, class T >
		FieldValChooser< Info,T > fieldChoose( T Info:: *wsk, T arg ) { return FieldValChooser< Info,T >( wsk,arg ); }

	/* FieldValChooserL
	 * test, czy pole w info jest mniejsze od podanej wartosci
	 */
	/** \brief Less info field value chooser
	 *
	 *  Function object that checks if the attribute \a val greater than an element (parameter of call function operator) info object field pointed by \a wsk.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FieldValChooserL
	{
		T Info:: *wsk; /**< \brief Pointer to member.*/
		T val; /**< \brief Value to compare to fixed in constructor */

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef FieldValChooserL< Info,T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns values to \a val and pointer to member \a wsk i.e. decides which members of info are to be checked and what value they are to be less to.	 */
		FieldValChooserL( T Info:: *arg = 0, T arg2 = T() ): wsk( arg ), val( arg2 ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true as long as field of \a elem pointed by pointer to member \a wsk is less than \a val.
		 *  \param elem the checked element.
		 *  \param graph the reference to considered graph.
		 *  \return true if \a val is greater than pointed by the \a wsk member of info in the object \a elem, false otherwise. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return elem->info.*wsk < val; }
	};

	/** \brief Generating function of less field value chooser (FieldChooser).
	 *  \ingroup DMchooser*/
	template< class Info, class T > FieldValChooserL< Info,T >
		fieldChooseL( T Info:: *wsk, T arg ) { return FieldValChooserL< Info,T >( wsk,arg ); }

	/* FieldValChooserG
	 * test, czy pole w info jest wieksze od podanej wartosci
	 */
	/** \brief Greater info field value chooser
	 *
	 *  Function object that checks if attribute \a val is smaller then an element (parameter of call function operator) info object field pointed by \a wsk.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FieldValChooserG
	{
		T Info:: *wsk;/**< \brief Pointer to member.*/
		T val;/**< \brief value to compare to fixed in constructor */

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef FieldValChooserG< Info,T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns values to \a val and pointer to member \a wsk i.e. decides which
		 *  members of info are to be checked and what value they are to be greater. */
		FieldValChooserG( T Info:: *arg = 0, T arg2 = T() ): wsk( arg ), val( arg2 ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true as long as the field of \a elem
		 *  pointed by \a wsk is greater than \a val.
		 *  \param elem pointer to checked object.
		 *  \param graph the considered graph.
		 *  \return true if \a val equals pointed by \a wsk member of info in object \a elem, false otherwise. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return elem->info.*wsk > val; }
	};

	/** \brief Generating function of greater field value chooser (FieldChooser).
	 * \ingroup DMchooser*/
	template< class Info, class T > FieldValChooserG< Info,T >
		fieldChooseG( T Info:: *wsk, T arg ) { return FieldValChooserG< Info,T >( wsk,arg ); }

	/* FielBoolChooser
	 * test, czy pole w info ma wartosc prawda (jego typ musi byc konwertowalny do bool)
	 */
	/** \brief Boolean info field chooser
	 *
	 *  Function object that checks if certain filed of the parameter \a elem call in
	 *  overladed \p operator() is convertible to true value.
	 *  \tparam Info class of object info.
	 *  \tparam T type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FielBoolChooser
	{
		T Info:: *wsk;/**<  \brief Pointer to tested member.*/

		/** \brief  Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef FielBoolChooser< Info,T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Decides which members of info are to be checked. */
		FielBoolChooser( T Info::*arg = 0): wsk( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true as long as pointed by \a wsk field of \a elem is convertible to true.
		 *  \param elem the checked object.
		 *  \param graph the considered graph.
		 *  \return true if pointed by \a wsk member of info in object \a elem is true.
		 *  (convertible to true value), false otherwise. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return bool( elem->info.*wsk ); }
	};

	/** \brief Generating function of bool field chooser (FielBoolChooser).
	 *  \ingroup DMchooser*/
	template< class Info, class T >
		FielBoolChooser< Info,T > fieldChoose( T Info:: *wsk ) { return FielBoolChooser< Info,T >(wsk); }

	/* FielObjChooser
	 * wlasny obiekt lub funkcja, ktora ma sie wykonywac dla konkretnego pola z info
	 */
	// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
	/** \brief Info field value chooser with functor.
	 *
	 *  Function object that checks if the given \a functor returns value convertible to true for a certain field of info object.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Obj the function object class that provides function testing the field.
	 *  \ingroup DMchooser */
	template< class Info, class T, class Obj > struct FieldObjChooser
	{
		T Info:: *wsk; /**< \brief the pointer to tested member.*/
		mutable Obj funktor; /**< \brief functor testing the member. */

		/** \brief  Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef FieldObjChooser< Info,T,Obj > ChoosersSelfType;

		FieldObjChooser( T Info:: *awsk = 0, Obj afun = Obj() ): wsk( awsk ), funktor( afun ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return (bool)funktor( elem->info.*wsk ); }
	};

	/** \brief Generating  function of FieldObjChooser.
	 *  \ingroup DMchooser*/
	template< class Info, class T, class Obj > FieldObjChooser< Info,T,Obj >
		fieldFChoose( T Info::*wsk, Obj obj ) { return FieldObjChooser< Info,T,Obj >( wsk,obj ); }

	/* FielSetChooser
	 * test, czy podane pole w info ma wartosc z danego zbioru (Koala::Set)
	 */
	/** \brief Info field value belongs to set chooser.
	 *
	 *  Function object that checks if the prespecified filed of info object belongs to the set defined in constructor.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Z the type of object in set.
	 *  \ingroup DMchooser */
	template< class Info, class T, class Z > struct FieldSetChooser
	{
		T Info:: *wsk;
		Koala::Set< Z > set;

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef FieldSetChooser< Info,T,Z > ChoosersSelfType;

		FieldSetChooser( T Info:: *awsk = 0, const Koala::Set< Z > &aset = Koala::Set< Z >() ):
			wsk( awsk ), set( aset ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return set.isElement( elem->info.*wsk ); }
	};

	/** \brief Generating  function of FieldSetChooser.
	 *  \ingroup DMchooser*/
	template< class Info, class T, class Z > FieldSetChooser< Info,T,Z >
		fieldChoose( T Info::*wsk, Koala::Set< Z > set ) { return FieldSetChooser< Info,T,Z >( wsk,set ); }

	/* FielContainerChooser
	 * sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Info field value belong to container chooser.
	 *
	 *  Function object that checks if the given \a certain field of info object belongs to the container defined in constructor and given by iterators.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Iter the type of iterator for container with tested elements.
	 *  \ingroup DMchooser */
	template< class Info, class T, class Iter > struct FieldContainerChooser
	{
		T Info:: *wsk;
		Iter begin, end;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef FieldContainerChooser< Info,T,Iter > ChoosersSelfType;

		FieldContainerChooser( T Info:: *awsk = 0, Iter a = Iter(), Iter b = Iter() ):
			wsk( awsk ), begin( a ), end( b ) {}

		template< class Elem, class Graph > bool
			operator()( Elem *elem, const Graph &graph ) const { return std::find( begin,end,elem->info.*wsk ) != end; }
	};

	/** \brief Generating  function of FielContainerChooser.
	 *  \ingroup DMchooser*/
	template< class Info, class T, class Iter > FieldContainerChooser< Info,T,Iter >
		fieldChoose( T Info::*wsk, Iter b, Iter e ) { return FieldContainerChooser< Info,T,Iter >( wsk,b,e ); }

	// choosery decydujace na podstawie wartosci przypisanej elementowi w podanej tablicy asocjacyjnej

	/* AssocHasChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy
	 */
	/** \brief Is key in hashmap chooser.
	 *
	 *  Function object that checks if the element is  a key in hashmap defined in constructor.
	 *  \tparam Cont the type of hashmap
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocHasChooser
	{
		Cont cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocHasChooser< Cont > ChoosersSelfType;

		AssocHasChooser( const Cont &arg = Cont() ): cont( arg ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ); }
	};

	/** \brief Generating  function of AssocHasChooser.
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocHasChooser< Cont > assocKeyChoose( Cont arg ) { return AssocHasChooser< Cont >( arg ); }

	/* AssocBoolChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc odpowiada true (typ
	 * wartosci musi byc konwertowalny do bool)
	 */
	/** \brief Has true mapped value chooser.
	 *
	 *  Function object that checks if the element has a mapped value convertible to true in the associative container defined in constructor. Obviously the element needs to be a key in the associative container.
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocBoolChooser
	{
		Cont cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocBoolChooser< Cont > ChoosersSelfType;

		AssocBoolChooser( const Cont &arg = Cont() ): cont( arg ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && (bool)cont[elem]; }
	};

	/** \brief Generating  function of AssocBoolChooser.
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocBoolChooser< Cont > assocChoose( Cont arg ) { return AssocBoolChooser< Cont >( arg ); }

	/* AssocValChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest rowna zadanej
	 */
	/** \brief Has mapped value chooser.
	 *
	 *  Function object that checks if the element mapped value equals the prespecified value. Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooser
	{
		Cont cont;

		typename Cont::ValType val;
		typedef typename Cont::ValType SelfValType;
		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocValChooser< Cont > ChoosersSelfType;

		AssocValChooser( const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType() ):
			cont( arg ),val( aval ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] == val; }
	};

	/** \brief Generating  function of AssocValChooser.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooser< Cont >
		assocChoose( Cont arg, typename Cont::ValType val ) { return AssocValChooser< Cont >( arg,val ); }

	/* AssocValChooserL
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest mniejsza od zadanej
	 */
	/** \brief Has mapped value less chooser.
	 *
	 *  Function object that checks if the element mapped value is less than the prespecified value. Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserL
	{
		Cont cont;

		typename Cont::ValType val;
		typedef typename Cont::ValType SelfValType;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocValChooserL< Cont > ChoosersSelfType;

		AssocValChooserL( const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType() ):
			cont( arg ),val( aval ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] < val; }
	};

	/** \brief Generating  function of AssocValChooserL.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooserL< Cont >
		assocChooseL( Cont arg, typename Cont::ValType val ) { return AssocValChooserL< Cont >( arg,val ); }

	/* AssocValChooserG
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest wieksza od zadanej
	 */
	/** \brief Has mapped value greater chooser.
	 *
	 *  Function object that checks if the element mapped value is greater than the prespecified value. Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserG
	{
		Cont cont;

		typename Cont::ValType val;
		typedef typename Cont::ValType SelfValType;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocValChooserG< Cont > ChoosersSelfType;

		AssocValChooserG( const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType() ):
			cont( arg ),val( aval ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] > val; }
	};

	/** \brief Generating  function of AssocValChooserG.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooserG< Cont >
		assocChooseG( Cont arg, typename Cont::ValType val ) { return AssocValChooserG< Cont >( arg,val ); }

	/* AssocSetChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest elementem podanego
	 * zbioru (Koala::Set)
	 */
	/** \brief Has mapped value in set chooser.
	 *
	 *  Function object that checks if the element mapped value belongs to the prespecified set. Furthermore, the element needs to be a key in the associative container. Both, the associative container and the set are defined in the constructor.
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocSetChooser
	{
		Cont cont;

		Koala::Set< typename Cont::ValType > set;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocSetChooser< Cont > ChoosersSelfType;

		AssocSetChooser( const Cont &arg = Cont(), const Koala::Set< typename Cont::ValType > &aset =
			Koala::Set< typename Cont::ValType >() ): cont( arg ),set( aset ) { }

		template< class Elem, class Graph > bool
			operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && set.isElement( cont[elem] ); }
	};

	/** \brief Generating  function of AssocSetChooser.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocSetChooser< Cont > assocChoose( Cont arg,
		const Koala::Set< typename Cont::ValType > &set ) { return AssocSetChooser< Cont >( arg,set ); }

	/* AssocContainerChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc lezy w przedziale miedzy
	 * podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Has mapped value in container chooser.
	 *
	 *  Function object that checks if the element mapped value belongs to the prespecified another container (with iterator). Furthermore, the element needs to be a key in the associative container. Both, the associative container and the container are defined in the constructor.
	 *  \tparam Cont the type of associative container.
	 *  \tparma Iter the iterator type of container to search mapped values in for.
	 *  \ingroup DMchooser */
	template< class Cont, class Iter > struct AssocContainerChooser
	{
		Iter begin/**\brief the iterator to the first element of the container.*/, end/**\brief the iterator to the past-the-end element of the container.*/;
		Cont cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocContainerChooser< Cont,Iter > ChoosersSelfType;
		AssocContainerChooser( const Cont &acont = Cont(), Iter abegin = Iter(), Iter aend = Iter() ):
			cont( acont ), begin( abegin ), end( aend ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
			{ return cont.hasKey( elem ) && std::find( begin,end,cont[elem] ) != end; }
	};

	/** \brief Generating  function of AssocContainerChooser.
	 *  \ingroup DMchooser*/
	template< class Cont, class Iter > AssocContainerChooser< Cont,Iter >
		assocChoose( Cont cont, Iter begin, Iter end ) { return AssocContainerChooser< Cont,Iter >( cont,begin,end ); }

	/* AssocObjChooser
	 * sprawdza wartosc podanemgo obiektu funkcyjnego przypisana w tablicy asocjacyjnej testowanemu elementowi
	 */
	/** \brief Test mapped value with another functor chooser.
	 *
	 *  The function object that passes the mapped value of element to the functor, and returns the same result.
	 *  Both, the associative container and the functor are defined in the constructor.
	 *  \tparam Cont the type of associative container.
	 *  \tparma Obj the functor class.
	 *  \ingroup DMchooser */
	template< class Cont, class Obj > struct AssocObjChooser
	{
		// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
		mutable Obj funktor;
		Cont cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocObjChooser< Cont,Obj > ChoosersSelfType;
		AssocObjChooser( const Cont &acont = Cont(), Obj arg = Obj() ): cont( acont ), funktor( arg ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
			{ return cont.hasKey( elem ) && (bool)funktor( cont[elem] ); }
	};

	/** \brief Generating  function of AssocObjChooser.
	 *  \ingroup DMchooser*/
	template< class Cont, class Obj > AssocObjChooser< Cont,Obj >
		assocFChoose( Cont cont, Obj arg ) { return AssocObjChooser< Cont,Obj >( cont,arg ); }

	// Wszystkie choosery operujace na tablicach asocjacyjnych maja wersje dzialajace z zewnetrzna tablica
	// podawana przez wskaznik. Unikamy kopiowania tablic, uzywajac trzeba uwazac, by tablica wciaz zyla przez
	// caly czas zycia choosera. Funkcje tworzace maja przedrostek ext i pobieraja adres tablicy

	/* AssocHasChooser
	 *
	 */
	 /** \brief Is key in hashmap chooser.
	 *
	 *  Function object that checks if the element is  a key in hashmap defined in constructor. (external array)
	 *  \tparam Cont the type of hashmap (pointer)
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocHasChooser< Cont * >
	{
		const Cont *cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocHasChooser< Cont * > ChoosersSelfType;

		AssocHasChooser( const Cont *arg = 0): cont( arg ) { }

		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &) const { return cont->hasKey( elem ); }
	};

	/** \brief Generating  function of AssocHasChooser with external container.
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocHasChooser< Cont * > extAssocKeyChoose( const Cont *arg ) { return AssocHasChooser< Cont * >( arg ); }

	/* AssocBoolChooser
	 *
	 */
	/** \brief Has true mapped value chooser.
	 *
	 *  Function object that checks if the element has a mapped value convertible to true in the associative container defined in constructor (external array). Obviously the element needs to be a key in the associative container.
	 *  \tparam Cont the type of associative container (pointer).
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocBoolChooser< Cont * >
	{
		const Cont *cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocBoolChooser< Cont * > ChoosersSelfType;

		AssocBoolChooser( const Cont *arg = 0): cont( arg ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
			{ return cont->hasKey( elem ) && (bool)cont->operator[]( elem ); }
	};

	/** \brief Generating  function of AssocBoolChooser with external container.
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocBoolChooser< Cont * > extAssocChoose( const Cont *arg ) { return AssocBoolChooser< Cont * >( arg ); }

	/* AssocValChooser
	 *
	 */
	/** \brief Has mapped value chooser.
	 *
	 *  Function object that checks if the element mapped value equals the prespecified value. Furthermore, the element needs to be a key in the container (external container). Both, the associative container and the value to compare are set up in constructor.
	 *  \tparam Cont the type of associative container. (pointer)
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooser< Cont * >
	{
		const Cont *cont;

		typename Cont::ValType val;
		typedef typename Cont::ValType SelfValType;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocValChooser< Cont * > ChoosersSelfType;

		AssocValChooser( const Cont *arg = 0, typename Cont::ValType aval = SelfValType() ):
			cont( arg ), val( aval ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
			{ return cont->hasKey( elem ) && cont->operator[]( elem ) == val; }
	};

	/** \brief Generating  function of AssocValChooser with external container.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooser< Cont * >
		extAssocChoose( const Cont *arg, typename Cont::ValType val ) { return AssocValChooser< Cont * >( arg,val ); }

	/* AssocValChooserG
	 *
	 */
	/** \brief Has mapped value greater chooser.
	 *
	 *  Function object that checks if the element mapped value is greater than the prespecified value. Furthermore, the element needs to be a key in the container (external container). Both, the associative container and the value to compare are set up in constructor.
	 *  \tparam Cont the type of associative container.  (pointer)
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserG< Cont * >
	{
		const Cont *cont;

		typename Cont::ValType val;
		typedef typename Cont::ValType SelfValType;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocValChooserG< Cont * > ChoosersSelfType;

		AssocValChooserG( const Cont *arg = 0, typename Cont::ValType aval = SelfValType() ):
			cont( arg ), val( aval ) { }

		template< class Elem,class Graph > bool operator()( Elem *elem, const Graph & ) const
			{ return cont->hasKey( elem ) && cont->operator[]( elem ) > val; }
	};

	/** \brief Generating  function of AssocValChooserG with external container.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooserG< Cont * >
		extAssocChooseG( const Cont *arg, typename Cont::ValType val ) { return AssocValChooserG< Cont * >( arg,val ); }

	/* AssocValChooserL
	 *
	 */
	/** \brief Has mapped value less chooser.
	 *
	 *  Function object that checks if the element mapped value is less than the prespecified value. Furthermore, the element needs to be a key in the container (external container). Both, the associative container and the value to compare are set up in constructor.
	 *  \tparam Cont the type of associative container. (pointer)
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserL< Cont * >
	{
		const Cont *cont;

		typename Cont::ValType val;
		typedef typename Cont::ValType SelfValType;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocValChooserL< Cont * > ChoosersSelfType;

		AssocValChooserL( const Cont *arg = 0, typename Cont::ValType aval = SelfValType() ):
			cont( arg ), val( aval ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
			{ return cont->hasKey( elem ) && cont->operator[]( elem ) < val; }
	};

	/** \brief Generating  function of AssocValChooserL with external container.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooserL< Cont * >
		extAssocChooseL( const Cont *arg, typename Cont::ValType val ) { return AssocValChooserL< Cont * >( arg,val ); }

	/* AssocSetChooser
	 *
	 */
	/** \brief Has mapped value in set chooser.
	 *
	 *  Function object that checks if the element mapped value belongs to the prespecified set. Furthermore, the element needs to be a key in the associative container (external container). Both, the associative container and the set are defined in the constructor.
	 *  \tparam Cont the type of associative container. (pointer)
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocSetChooser< Cont * >
	{
		const Cont *cont;

		Koala::Set< typename Cont::ValType > set;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocSetChooser< Cont * > ChoosersSelfType;

		AssocSetChooser(const Cont *arg = 0, const Koala::Set< typename Cont::ValType > &aset =
			Koala::Set< typename Cont::ValType>() ): cont( arg ), set( aset ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const
			{ return cont->hasKey( elem ) && set.isElement( cont->operator[]( elem ) ); }
	};

	/** \brief Generating  function of AssocSetChooser with external container.
	 *  \ingroup DMchooser*/
	template< class Cont > AssocSetChooser< Cont * > extAssocChoose( const Cont *arg,
		const Koala::Set< typename Cont::ValType > &set ) { return AssocSetChooser< Cont * >( arg,set ); }

	/* AssocContainerChooser
	 *
	 */
	/** \brief Has mapped value in container chooser.
	 *
	 *  Function object that checks if the element mapped value belongs to the prespecified another container (with iterator). Furthermore, the element needs to be a key in the associative container (external container). Both, the associative container and the container are defined in the constructor.
	 *  \tparam Cont the type of associative container (pointer).
	 *  \tparma Iter the iterator type of container to search mapped values in for.
	 *  \ingroup DMchooser */
	template< class Cont, class Iter > struct AssocContainerChooser< Cont *,Iter >
	{
		Iter begin, end;
		const Cont *cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocContainerChooser< Cont *,Iter > ChoosersSelfType;

		AssocContainerChooser( const Cont *acont = 0, Iter abegin = Iter(), Iter aend=Iter() ):
			cont( acont ), begin( abegin ), end( aend ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph & ) const;
	};

	/** \brief Generating  function of AssocContainerChooser with external container.
	 *  \ingroup DMchooser*/
	template< class Cont, class Iter > AssocContainerChooser< Cont *,Iter >
		extAssocChoose(const Cont *cont, Iter begin, Iter end )
	{ return AssocContainerChooser< Cont *,Iter >( cont,begin,end ); }

	/* AssocObjChooser
	 *
	 */
	/** \brief Test mapped value with another functor chooser.
	 *
	 *  The function object that passes the mapped value of element to the functor, and returns the same result.
	 *  Both, the associative container (external container) and the functor are defined in the constructor.
	 *  \tparam Cont the type of associative container (pointer).
	 *  \tparma Obj the functor class.
	 *  \ingroup DMchooser */
	template< class Cont, class Obj > struct AssocObjChooser< Cont *,Obj >
	{
		// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
		mutable Obj funktor;
		const Cont *cont;

		/** Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef AssocObjChooser< Cont *,Obj > ChoosersSelfType;

		AssocObjChooser( const Cont *acont = 0, Obj arg = Obj() ): cont( acont ), funktor( arg ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
			{ return cont->hasKey( elem ) && (bool)funktor( cont->operator[]( elem ) ); }
	};

	/** \brief Generating  function of AssocObjChooser with external container.
	 *  \ingroup DMchooser*/
	template< class Cont, class Obj > AssocObjChooser< Cont *,Obj >
		extAssocFChoose( const Cont *cont, Obj arg ) { return AssocObjChooser< Cont *,Obj >( cont,arg ); }

	// Predykaty chooserow mozna laczyc operatorami logicznymi. Choosery operacji logicznych na prostszych chooserach

	/* OrChooser
	 *
	 */
	/** \brief Or chooser.
	 *
	 *  The function object that joins two choosers. It returns true value if and only if the first one or the second one return true.
	 *  \tparam Ch1 the first chooser class.
	 *  \tparma Ch2 the second chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1, class Ch2 > struct OrChooser
	{
		Ch1 ch1;
		Ch2 ch2;

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef OrChooser< Ch1,Ch2 > ChoosersSelfType;

		OrChooser( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
			{ return (ch1( elem,graph ) || ch2( elem,graph )); }
	};

	/** \brief Generating  function of OrChooser.
	 *  \ingroup DMchooser*/
	template< class  Ch1, class Ch2 > OrChooser< Ch1,Ch2 >
		orChoose( Ch1 a, Ch2 b ) { return OrChooser< Ch1,Ch2 >( a,b ); }

	// w kodzie funkcje tworzace zlozonych chooserow mozna zastapic odpowiednimi operatorami logicznymi

	/** \brief The overloaded operator||. Calls the generating  function of OrChooser i.e. generate chooser that joins two choosers with logic or operator.
	 *  \ingroup DMchooser*/
	template <class  Ch1, class Ch2> OrChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator||( Ch1 a, Ch2 b ) { return OrChooser< Ch1,Ch2 >( a,b ); }

	/* AndChooser
	 *
	 */
	/** \brief And chooser.
	 *
	 *  The function object that joins two choosers. It returns true value if and only if the first one gives the opposite result to the second one.
	 *  \tparam Ch1 the first chooser class.
	 *  \tparma Ch2 the second chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1, class Ch2 > struct AndChooser
	{
		Ch1 ch1;
		Ch2 ch2;

		typedef AndChooser< Ch1,Ch2 > ChoosersSelfType;

		AndChooser( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
			{ return (ch1( elem,graph ) && ch2( elem,graph )); }
	};

	/** \brief Generating  function of AndChooser.
	 *  \ingroup DMchooser*/
	template< class  Ch1, class Ch2 > AndChooser< Ch1,Ch2 >
		andChoose( Ch1 a, Ch2 b ) { return AndChooser< Ch1,Ch2 >( a,b ); }

	/** \brief The overloaded operator&&. Calls the generating  function of AndChooser i.e. generate chooser that joins two choosers with logic and operator.
	 *  \ingroup DMchooser*/
	template< class Ch1, class Ch2 > AndChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator&&( Ch1 a, Ch2 b ) { return AndChooser< Ch1,Ch2 >( a,b ); }

	/* XorChooser
	 *
	 */
	/** \brief Xor chooser.
	 *
	 *  The function object that joins two choosers. It returns true value if and only if the first one or the second one return true.
	 *  \tparam Ch1 the first chooser class.
	 *  \tparma Ch2 the second chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1, class Ch2 > struct XorChooser
	{
		Ch1 ch1;
		Ch2 ch2;

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef XorChooser< Ch1,Ch2 > ChoosersSelfType;

		XorChooser( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &graph ) const
			{ return (ch1( elem,graph ) != ch2( elem,graph )); }
	};

	/** \brief Generating  function of XorChooser.
	 *  \ingroup DMchooser*/
	template< class Ch1, class Ch2 > XorChooser< Ch1,Ch2 >
		xorChoose( Ch1 a, Ch2 b ) { return XorChooser< Ch1,Ch2 >( a,b ); }

	/** \brief The overloaded operator^. Calls the generating  function of XorChooser i.e. generate chooser that joins two choosers with logic exclusive or operator.
	 *  \ingroup DMchooser*/
	template< class Ch1, class Ch2 > XorChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator^( Ch1 a, Ch2 b ) { return XorChooser< Ch1,Ch2 >( a,b ); }

	/* NotChooser
	 *
	 */
	/** \brief Not chooser.
	 *
	 *  The function object that gives the opposite result to the given chooser.
	 *  \tparam Ch1 the chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1 > struct NotChooser
	{
		Ch1 ch1;

		typedef NotChooser< Ch1 > ChoosersSelfType;

		NotChooser( Ch1 a = Ch1() ): ch1( a ) { }

		template< class Elem, class Graph >  bool operator()( Elem *elem, const Graph &graph) const
			{ return !ch1( elem,graph ); }
	};

	/** \brief Generating  function of NotChooser.
	 *  \ingroup DMchooser*/
	template< class  Ch1 >
		NotChooser< Ch1 > notChoose( Ch1 a ) { return NotChooser< Ch1 >( a ); }

	/** \brief The overloaded operator!. Calls the generating  function of NotChooser i.e. generate chooser that negates the given chooser.
	 *  \ingroup DMchooser*/
	template< class  Ch1 > NotChooser< typename Ch1::ChoosersSelfType >
		operator!( Ch1 a ) { return NotChooser< Ch1 >( a ); }

	// choosery dow wybierania wierzcholkow

	/* VertDegValChooser
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma podana
	 * wartosc
	 */
	/** \brief Vertex degree value chooser.
	 *
	 *  The function object that checks if the vertex degree concerning the edge direction type is equal to the prespecified value.
	 *  \ingroup DMchooser */
	struct VertDegValChooser
	{
		int deg; /**< \brief the desired degree*/
		Koala::EdgeDirection type; /**< \brief the considered edge direction.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef VertDegValChooser ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The expected degree and the edge direction to consider are defined here.
		 *  \param adeg the expected degree.
		 *  \param atype the mask of considered edge directions.*/
		VertDegValChooser( int adeg = 0, Koala::EdgeDirection atype = Koala::EdAll ): deg( adeg ), type( atype ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning boolean value true if the vertex \a v is of degree \a deg.
		 *  \param v the tested vertex.
		 *  \param g reference to considered graph.
		 *  \return the true if degree of the vertex matches \a adeg, false otherwise. */
		template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
			{ return g.deg( v,type ) == deg; }
	};

	/** \brief Generating  function of VertDegChoose.
	 *  \ingroup DMchooser*/
	VertDegValChooser vertDegChoose( int adeg, Koala::EdgeDirection atype = Koala::EdAll )
		{ return VertDegValChooser( adeg,atype ); }

	/* VertDegValChooserL
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
	 * mniejsza od zadanej
	 */
	/** \brief Vertex degree less chooser.
	 *
	 *  The function object that checks if the vertex degree concerning the edge direction type is less then the prespecified value.
	 *  \ingroup DMchooser */
	struct VertDegValChooserL
	{
		int deg; /**< \brief the strict upper bond for degree*/
		Koala::EdgeDirection type;/**< \brief the considered edge direction.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef VertDegValChooserL ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The strict upper bond for degree and the edge direction to consider are defined here.
		 *  \param adeg the expected degree.
		 *  \param atype the mask of considered edge directions.*/
		VertDegValChooserL( int adeg = 0, Koala::EdgeDirection atype = Koala::EdAll ): deg( adeg ), type( atype ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning boolean value true if the vertex \a v degree is smaller than \a deg.
		 *  \param v the tested vertex.
		 *  \param g reference to considered graph.
		 *  \return the true if degree of the vertex is smaller than \a deg, false otherwise. */
		template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
			{ return g.deg( v,type ) < deg; }
	};

	/** \brief Generating  function of vertDegChooseL.
	 *  \ingroup DMchooser*/
	VertDegValChooserL vertDegChooseL( int adeg, Koala::EdgeDirection atype = Koala::EdAll )
		{ return VertDegValChooserL( adeg,atype ); }

	/* VertDegValChooserG
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
	 * wieksza od zadanej
	 */
	/** \brief Vertex degree greater chooser.
	 *
	 *  The function object that checks if the vertex degree concerning the edge direction type is greater then the prespecified value.
	 *  \ingroup DMchooser */
	struct VertDegValChooserG
	{
		int deg;/**< \brief the strict lower bond degree*/
		Koala::EdgeDirection type;/**< \brief the considered edge direction.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef VertDegValChooserG ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The strict lover bond for degree and the edge direction to consider are defined here.
		 *  \param adeg the expected degree.
		 *  \param atype the mask of considered edge directions.*/
		VertDegValChooserG( int adeg = 0, Koala::EdgeDirection atype = Koala::EdAll ): deg( adeg ), type( atype ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning boolean value true if the vertex \a v degree is greater than \a deg.
		 *  \param v the tested vertex.
		 *  \param g reference to considered graph.
		 *  \return the true if degree of the vertex is greater than \a deg, false otherwise. */
		template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
			{ return g.deg( v,type ) > deg; }
	};

	/** \brief Generating  function of vertDegChooseG.
	 *  \ingroup DMchooser*/
	VertDegValChooserG vertDegChooseG( int adeg, Koala::EdgeDirection atype = Koala::EdAll )
		{ return VertDegValChooserG( adeg,atype ); }

	/* VertDegSetChooser
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc z
	 * podanego zbioru
	 */
	/** \brief Vertex degree from set chooser.
	 *
	 *  The function object that checks if the vertex degree concerning the edge direction belongs the set prespecified in constructor. (also the direction to considered is defined there)
	 *  \ingroup DMchooser */
	template< class Int > struct VertDegSetChooser
	{
		Koala::Set< Int > set;/**< \brief the set with acceptable degrees.*/
		Koala::EdgeDirection type;/**< \brief the considered edge direction.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef VertDegSetChooser< Int > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The set with acceptable degree and the edge direction to consider are defined here.
		 *  \param aset the set with degrees.
		 *  \param atype the mask of considered edge directions.*/
		VertDegSetChooser( const Koala::Set< Int > &aset = Koala::Set< Int >(),
			Koala::EdgeDirection atype = Koala::EdAll ): set( aset ), type( atype ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning boolean value true if the vertex \a v degree belongs to \a set.
		 *  \param v the tested vertex.
		 *  \param g reference to considered graph.
		 *  \return the true if degree of the vertex belongs to \a set, false otherwise. */
		template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
			{ return set.isElement( g.deg( v,type ) ); }
	};

	/** \brief Generating  function of VertDegSetChooser.
	 *  \ingroup DMchooser*/
	template< class Int > VertDegSetChooser< Int > vertDegChoose( Koala::Set< Int > aset,
		Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegSetChooser< Int >( aset,atype ); }

	/* VertDegContainerChooser
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
	 * z zakresu miedzy podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Vertex degree from container chooser.
	 *
	 *  The function object that checks if the vertex degree concerning the edge direction is an element of the container prespecified in constructor. (also the direction to considered is defined there)
	 *  \ingroup DMchooser */
	template< class Iter > struct VertDegContainerChooser
	{
		Iter begin/**\brief iterator to the first element of container*/, end/**\brief iterator to the past-the-end element of container*/;
		Koala::EdgeDirection typ;/**< \brief the considered edge direction.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef VertDegContainerChooser< Iter > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The container with acceptable degree and the edge direction to consider are defined here.
		 *  \param abeg the iterator to the first element of container.
		 *  \param aend the iterator to the past-the-end element of container.
		 *  \param atype the mask of considered edge directions.*/
		VertDegContainerChooser( Iter abeg = Iter(), Iter aend = Iter(), Koala::EdgeDirection atype = Koala::EdAll ):
			begin( abeg ), end( aend ), typ( atype ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning boolean value true if the vertex \a v degree belongs to the container.
		 *  \param v the tested vertex.
		 *  \param g reference to considered graph.
		 *  \return the true if degree of the vertex belongs to the container, false otherwise. */
		template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
			{ return std::find( begin,end,g.deg( v,typ ) ) != end; }
	};

	/** \brief Generating  function of VertDegContainerChooser.
	 *  \ingroup DMchooser*/
	template< class Iter > VertDegContainerChooser< Iter > vertDegChoose( Iter begin, Iter end,
		Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegContainerChooser< Iter >( begin,end,atype ); }

	/* VertDegFunctorChooser
	 * decyzja podejmowana na podstawie wartosci obiektu funktora policzonego na stopniu wierzcholka
	 */
	// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref
	/** \brief Vertex degree functor chooser.
	 *
	 *  The function object that for a given vertex tests if the vertex degree satisfy the functor defined in the constructor.
	 *  \ingroup DMchooser */
	template< class Obj > struct VertDegFunctorChooser
	{
		mutable Obj funktor;/** \brief the object function qualifying degrees.*/
		Koala::EdgeDirection typ;/**< \brief the considered edge direction.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef VertDegFunctorChooser< Obj > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The functor and the edge direction to consider are defined here.
		 *  \param afun the object function qualifying degrees.
		 *  \param atype the mask of considered edge directions.*/
		VertDegFunctorChooser( Obj afun = Obj(), Koala::EdgeDirection atype = Koala::EdAll ):
			funktor( afun ), typ( atype ) { }

		/** \brief Overloaded operator()
		 *
		 *  The function call operator returning converted to bool value returned the functor .
		 *  \param v the tested vertex.
		 *  \param g reference to considered graph.
		 *  \return the value returned by \a functor. */
		template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
			{ return (bool)funktor( g.deg( v,typ ) ); }
	};

	/** \brief Generating  function of VertDegFunctorChooser.
	 *  \ingroup DMchooser*/
	template< class Obj > VertDegFunctorChooser< Obj > vertDegFChoose( Obj afun,
		Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegFunctorChooser< Obj >( afun,atype ); }

	// choosery do wybierania krawedzi

	/* EdgeTypeChooser
	 * testuje, czy typ krawedzi spelnia podana maske
	 */
	/** \brief Edge direction chooser.
	 *
	 *  The function object chooses the edges with direction congruent with the mask prespecified in constructor.
	 *  \ingroup DMchooser */
	struct EdgeTypeChooser
	{
		Koala::EdgeDirection mask;/**< \brief the considered edge direction.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef EdgeTypeChooser ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The direction of edge to choose is defined here.
		 *  \param amask the mask of considered edge directions.*/
		EdgeTypeChooser( Koala::EdgeDirection amask = Koala::EdAll ): mask( amask )
			{ mask |= (mask & Directed) ? Directed : 0; }

		/** \brief Overloaded operator()
		 *
		 *  The function call operator returning true if \a e direction is congruent with \a mask.
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return true if \a e direction is congruent with the attribute \a mask, false otherwise. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return bool( g.getType( e ) & mask ); }
	};

	/** \brief Generating  function of EdgeTypeChooser.
	 *  \ingroup DMchooser*/
	EdgeTypeChooser edgeTypeChoose( Koala::EdgeDirection mask ) { return EdgeTypeChooser( mask ); }

	// choosery zlozone dla krawedzi, sprawdzajace warunkek definiowany dla wierzcholka sa prawda dla jej koncow

	/* EdgeFirstEndChooser
	 * test pierwszego konca krawedzi
	 */
	/** \brief Edge first end satisfy functor chooser.
	 *
	 *  The function object chooses the edges with first end satisfy a functor (ex. some vertex chooser)  defined in constructor.
	 *  \ingroup DMchooser */
	template< class Ch > struct EdgeFirstEndChooser
	{
		Ch ch;/**< \brief the function object that checks the first end of edge.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef EdgeFirstEndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here.
		 *  \param funktor the functor assigned to attribute \a ch.*/
		EdgeFirstEndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return the same value as functor called on the first end of \a e and the graph \a g. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return ch( g.getEdgeEnd1( e ),g ); }
	};

	/** \brief Generating  function of EdgeFirstEndChooser.
	 *  \ingroup DMchooser*/
	template< class Ch > EdgeFirstEndChooser< Ch >
		edgeFirstEndChoose( Ch ch ) { return EdgeFirstEndChooser< Ch >( ch ); }

	/* EdgeSecondEndChooser
	 * test drugiego konca krawedzi
	 */
	/** \brief Edge second end satisfy functor chooser.
	 *
	 *  The function object chooses the edges with second end satisfy a functor (ex. some vertex chooser)  defined in constructor.
	 *  \ingroup DMchooser */
	template <class Ch> struct EdgeSecondEndChooser
	{
		Ch ch;/**< \brief the function object that checks the second end of edge.*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef EdgeSecondEndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here.
		 *  \param funktor the functor assigned to attribute \a ch.*/
		EdgeSecondEndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return the same value as functor called on the second end of \a e and graph \a g. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return ch( g.getEdgeEnd2( e ),g ); }
	};

	/** \brief Generating  function of EdgeSecondEndChooser.
	 *  \ingroup DMchooser*/
	template< class Ch > EdgeSecondEndChooser< Ch >
		edgeSecondEndChoose( Ch ch ) { return EdgeSecondEndChooser< Ch >( ch ); }

	/* Edge0EndChooser
	 * test, czy zaden koniec nie spelnia warunku
	 */
	/** \brief Edge none end satisfy functor chooser.
	 *
	 *  The function object chooses the edges in with none of its ends satisfies a functor (ex. some vertex chooser)  defined in constructor.
	 *  \ingroup DMchooser */
	template< class Ch > struct Edge0EndChooser
	{
		Ch ch;/**< \brief the function object that tests vertices (defined in constructor).*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef Edge0EndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here.
		 *  \param funktor the functor assigned to attribute \a ch.*/
		Edge0EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return true if none of \a e ends satisfies the functor \a ch. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return !ch( g.getEdgeEnd1( e ),g ) && !ch( g.getEdgeEnd2( e ),g ); }
	};

	template< class Ch > Edge0EndChooser< Ch > edge0EndChoose( Ch ch ) { return Edge0EndChooser< Ch >( ch ); }

	/* Edge1EndChooser
	 * test, czy jeden koniec spelnia warunkek
	 */
	/** \brief Edge one end satisfy functor chooser.
	 *
	 *  The function object chooses the edges in with one of its ends satisfies a functor (ex. some vertex chooser)  defined in constructor.
	 *  \ingroup DMchooser */
	template< class Ch > struct Edge1EndChooser
	{
		Ch ch;/**< \brief the function object that tests vertices (defined in constructor).*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef Edge1EndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here.
		 *  \param funktor the functor assigned to attribute \a ch.*/
		Edge1EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return true if one of \a e ends satisfies the functor \a ch. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return ch( g.getEdgeEnd1( e ),g ) != ch( g.getEdgeEnd2( e ),g ); }
	};

	template< class Ch > Edge1EndChooser< Ch > edge1EndChoose( Ch ch ) { return Edge1EndChooser< Ch >( ch ); }

	/* Edge2EndChooser
	 * test, czy oba konice spelniaja warunkek
	 */
	/** \brief Edge both ends satisfy functor chooser.
	 *
	 *  The function object chooses the edges in with both ends satisfy a functor (ex. some vertex chooser)  defined in constructor.
	 *  \ingroup DMchooser */
	template< class Ch > struct Edge2EndChooser
	{
		Ch ch;/**< \brief the function object that tests vertices (defined in constructor).*/

		/** \brief Type obligatory for chooser in Koala. If the type is defined, logic operations (&&, ||, !)  work properly. */
		typedef Edge2EndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here.
		 *  \param funktor the functor assigned to attribute \a ch.*/
		Edge2EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return true if both \a e ends satisfy the functor \a ch. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return ch( g.getEdgeEnd1( e ),g ) && ch( g.getEdgeEnd2( e ),g ); }
	};

	template< class Ch > Edge2EndChooser< Ch > edge2EndChoose( Ch ch ) { return Edge2EndChooser< Ch >( ch ); }

	//Castery to funktory ustalajace wartosci pol info w nowych wierz/kraw tworzonych podczas np. kopiowania grafow.
	// Wartosci te powstaja (w rozny sposob) na podstawie inf oryginalnych

	/* StdCaster
	 * caster zwyklego rzutowania miedzy dwoma strukturami
	 */
	/** \brief Standard caster.
	 *
	 *  The structure overload call function operator for two parameters. The value of source is simply casted on the destination. The inbuilt type conversion is used.
	 *  \ingroup DMcaster*/
	struct StdCaster
	{
		typedef StdCaster CastersSelfType;

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with two parameters, that uses the inbuilt type conversion of \a sour to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour the source object. */
		template< class InfoDest, class InfoSour >
			void operator()( InfoDest &dest, InfoSour sour ) { dest = (InfoDest)sour; }
	};

	// i jego funkcja tworzaca
	/** \brief Generating function for standard caster (StdCaster).
	 *  \ingroup DMcaster*/
	StdCaster stdCast() { return StdCaster(); }

	/* NoCastCaster
	 * caster ustawiajacy wartosc domyslna i ignorujacy oryginalny parametr wspolpracuje z produktami grafow (stad
	 * takze operator 3-argumentowy)
	 */
	/** \brief No cast caster.
	 *
	 *  The caster ignores the sources and simply calls the default values. Also three parameter call function is overloaded here.
	 *  \ingroup DMcaster*/
	struct NoCastCaster
	{
		typedef NoCastCaster CastersSelfType;

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with two parameters. The empty constructor of InfoDest is called and the result is assigned to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour the source object (ignored). */
		template< class InfoDest, class InfoSour >
			void operator()( InfoDest &dest, InfoSour sour ) { dest = InfoDest(); }

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with three parameters. The empty constructor of InfoDest is called and the result is assigned to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour1 the first source object (ignored).
		 *  \param sour2 the second source object (ignored). */
		template< class InfoDest, class InfoSour1, class InfoSour2 >
			void operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) { dest = InfoDest(); }
	};

	// funkcja tworzaca - dopuszczalny jedynie argument false
	/** \brief Generating function for no cast caster (NoCastCaster).
	 *
	 *  \param arg only false values are allowed.
	 *  \ingroup DMcaster*/
	NoCastCaster stdCast( bool arg );

	/* ObjCaster
	 * wyliczenie wartosci nowego info poprzez podany funktor wspolpracuje z produktami grafow (stad takze operator
	 * 3-argumentowy) jesli funktor je obsluguje
	 */
	// TODO: sprawdzic, czy nadal dziala ze zwyklymi funkcjami C pobierajacymi argument przez wartosc, referencje lub const ref

	/** \brief Functor caster.
	 *
	 *  The caster calls the functor defined in constructor to produce the info object.
	 *  \ingroup DMcaster*/
	template< class Fun > struct ObjCaster
	{
		typedef ObjCaster< Fun > CastersSelfType;

		mutable Fun funktor;/**< \brief the functor defined in constructor.*/
		/**\brief Constructor.
		 *
		 *  The constructor assigns the value to \a functor.*/
		ObjCaster( Fun afun = Fun() ): funktor( afun ) { }

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with two parameters. The method passes \a sour1 to \a functor and cast the result to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour1 the first source object.
		 *  \param sour2 the second source object. */
		template< class InfoDest, class InfoSour >
			void operator()( InfoDest &dest, InfoSour sour ) { dest = (InfoDest)funktor( sour ); }

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with three parameters. The method passes \a sour1 and \a sour2 to \a functor and cast the result to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour1 the first source object.
		 *  \param sour2 the second source object. */
		template< class InfoDest, class InfoSour1, class InfoSour2 > void
			operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) { dest = (InfoDest)funktor( sour1,sour2 ); }
	};

	/** \brief Generating function for functor caster (ObjCaster).
	 *
	 *  \param f the object function that generates the appropriate info.
	 *  \ingroup DMcaster*/
	template< class Funktor > ObjCaster< Funktor > stdCast( Funktor f ) { return ObjCaster< Funktor >( f ); }

	/* ValueCaster
	 * Caster wpisujacy ustalona wartosc wspolpracuje z produktami grafow (stad takze operator 3-argumentowy)
	 */
	/** \brief Fixed value caster.
	 *
	 * The caster assigns the same value to info. The value is set up in constructor.
	 *  \ingroup DMcaster*/
	template< class T > struct ValueCaster
	{
		typedef ValueCaster< T > CastersSelfType;

		T val;/**<\brief the fixed value set up in constructor.*/
		/** \brief Constructor.
		 *
		 *  Sets the value \a val up.*/
		ValueCaster( T arg = T() ): val( arg ) { }

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with two parameters. The method casts the \a val to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour2 the second source object (ignored). */
		template< class InfoDest, class InfoSour >
			void operator()( InfoDest &dest, InfoSour sour ) { dest = (InfoDest)val; }

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with three parameters. The method casts the \a val to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour1 the first source object (ignored).
		 *  \param sour2 the second source object (ignored). */
		template< class InfoDest, class InfoSour1, class InfoSour2 >
			void operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) { dest = (InfoDest)val; }
	};

	// funkcja tworzaca - podajemy stala wartosc
	/** \brief Generating function for fixed value caster (ObjCaster).
	 *
	 *  \param arg the value assigned to each info.
	 *  \ingroup DMcaster*/
	template< class T > ValueCaster< T > valCast( T arg = T() ) { return ValueCaster< T >( arg ); }

	//Linkery dzialajace np. podczas kopiowania grafow, wiaza nowo tworzone wierzch/kraw. z ich oryginalami, przez co
	// mozna latwo sprawdzic, ktory element odpowiada ktoremu

	// polowki pelnego linkera, tworza powiazanie  w jedna strone

	/* Std1NoLinker
	 * tylko false jest dopuszczalne - brak polaczenia
	 */
	/** \brief No link.
	 *
	 *  This linker should be used in in cases when user doesn't need a link and doesn't want to create one but function in Koala require some.
	 *
	 *  \ingroup DMlinker */
	struct Std1NoLinker
	{
		/** \brief Constructor.
		 *
		 * \param arg always false.*/
		Std1NoLinker( bool arg = false ) { koalaAssert( !arg,ExcBase ); }
		/** \brief Function call operator.
		 *
		 *  Does nothing.*/
		template< class Dest, class Sour > void operator()( Dest *, Sour * ) { }
	};

	/* Std1FieldLinker
	 * ustawia wskaznik na dolaczany obiekt w srodku struktury info obiektu docelowego (o ile nie byl on NULLem)
	 */
	/** \brief Object info one direction linker.
	 *
	 *  This linker uses the member of the object info that should be the pointer to the linked object.
	 *  The modified field of the object info is set in constructor via pointer to member.
	 *  \ingroup DMlinker
	 */
	template< class Info, class T > struct Std1FieldLinker
	{
		T Info:: *pt;/**< \brief the pointer to member in the info object.*/

		/** \brief Constructor.
		 *
		 *  Sets \a pt as a pointer to expected member in info of element.*/
		Std1FieldLinker( T Info:: *awsk = 0): pt( awsk ) { }

		/** \brief Function call operator.
		 *
		 *  The function makes connection between the source \a w and the destination \a wsk. In a result the info of \a wsk is modified.*/
		template< class Dest, class Sour > void operator()( Dest *wsk, Sour *w )
			{ if (pt && wsk) wsk->info.*pt = (T) w; }
	};

	/* Std1AssocLinker
	 * dopisuje powiazania do zewnetrznej! tablicy asocjacyjnej podanej w funkcji tworzacej stdLink (o ile argument
	 * nie byl NULLem)
	 */
	/** \brief Associative array one direction linker.
	 *
	 *  This linker uses the associative array which is a map that associate pointer of destination element with the pointer of source.
	 *  \ingroup DMlinker
	 */
	template< class Map > struct Std1AssocLinker
	{
		Map &map;/**< \brief the associative container (destination -> source).*/

		/** \brief Constructor.
		 *
		 *  Sets up the argument \a map value (to the parameter \a amap).*/
		Std1AssocLinker( Map &amap ): map( amap ) { }

		/** \brief Function call operator.
		 *
		 *  The function makes connection between the source \a w and the destination \a wsk. Create an element in \a amap which associates the key \a wsk with the mapped value \a w.*/
		template< class Dest, class Sour > void operator()( Dest *wsk, Sour *w ) { if (wsk) map[wsk] = w; }
	};

	/* Std2Linker
	 * pelny linker, zawiera obiekty laczace (polowki linkera - j.w.) nowy element ze starym i odwrotnie
	 */
	/** \brief Two directions linker .
	 *
	 *  This linker joins two linkers in order to create two way connection.\n
	 *  \tparam Link1 the type of linker for destination -> source connection.
	 *  \tparam Link2 the type of linker source -> destination connection.
	 *  \ingroup DMlinker
	 */
	template< class Link1, class Link2 > struct Std2Linker
	{
		typedef Std2Linker< Link1, Link2> LinkersSelfType;

		mutable Link1 dest2sour;/**< \brief The fist linker (destination -> source).*/
		mutable Link2 sour2dest;/**< \brief The second linker (source -> destination).*/

		Link1 &first() { return dest2sour; } /**<\brief Get first linker (destination -> source).*/
		Link2 &second() { return sour2dest; }/**<\brief Get second  linker (source -> destination).*/

		/** \brief Constructor.
		 *
		 *  Sets up the linkers \a dest2sour (\a al1) and \a sour2dest (\a al2).*/
		Std2Linker( Link1 al1, Link2 al2 ): dest2sour( al1 ), sour2dest( al2 ) { }

		/** \brief Function call operator.
		 *
		 *  The function makes connection between the source \a w and the destination \a wsk and another way round. */
		template< class Dest, class Sour > void operator()( Dest *wsk, Sour *w );
	};

	// funkcje tworzace polowki linkera - raczej do uzytku wewnetrznego
	/** \brief Generating function of no linker (Std1NoLinker).
	 *
	 *  \param a1 always false
	 *  \return the linker that does not make a connection.
	 *  \ingroup DMlinker */
	Std1NoLinker stdLink( bool a1 ) { return Std1NoLinker( a1 ); }

	/** \brief Generating function of one direction info linker (Std1FieldLinker).
	 *
	 *  The function generates and returns the one direction linker object the uses the info object of elements.
	 *  \param awsk1 the pointer to the member of info object.
	 *  \return the linker function object that is able to connect two elements.
	 *  \ingroup DMlinker
	 */
	template< class Info1, class T1 >
		Std1FieldLinker< Info1,T1 > stdLink( T1 Info1:: *awsk1 ) { return Std1FieldLinker< Info1,T1 >( awsk1 ); }

	/** \brief Generating function of standard one direction linker (Std1AssocLinker).
	 *
	 *  The function generates and returns the one direction linker object the uses the associative table \a tab.
	 *  \param tab1 the reference to the associative table that keeps all the connections.
	 *  \return the linker function object that is able to connect two elements.
	 *  \ingroup DMlinker     */
	template< class Map1 >
		Std1AssocLinker< Map1 > stdLink( Map1 &tab1 ) { return Std1AssocLinker< Map1 >( tab1 ); }

	// funkcje tworzace pelny linker, argumenty podaja wymagany sposob polaczenia elementow nowych z oryginalami
	// (pierwszy argument) i odwrotnie (drugi) argument bool moze byc tylko false (brak polaczenia)
	//
	/** \brief Generating function for no linker based on Std2Linker.
	 *
	 *   Both boolean parameters take only false value, then the link is not created.
	 *  \ingroup DMlinker     */
	Std2Linker< Std1NoLinker,Std1NoLinker > stdLink( bool a1, bool a2 );

	/** \brief Generating function of one way linker based on Std2Linker.
	 *
	 *  \param a1 boolean parameter take only false,
	 *  \param awsk pointer to member 
	 *  \return the linker with one way connection source to destination, using field pointed by \a awsk in source info object.
	 *  \ingroup DMlinker     */
	template< class Info,class T >
		Std2Linker< Std1NoLinker,Std1FieldLinker< Info,T > > stdLink( bool a1, T Info:: *awsk );

	/** \brief Generating function of one way linker based on Std2Linker.
	 *
	 *  \param a1 boolean parameter take only false,
	 *  \param tab associative container assigning  destination to source. 
	 *  \return the linker with one way connection source to destination, basing on associative container.
	 *  \ingroup DMlinker     */
	template< class Map >
		Std2Linker< Std1NoLinker,Std1AssocLinker< Map > > stdLink( bool a1, Map &tab );

	/** \brief Generating function for one way linker based on Std2Linker.
	 *
	 *  \param awsk1 pointer to member 
	 *  \param a2 boolean parameter take only false,
	 *  \return the linker with one way connection destination to source, using field pointed by \a awsk1 in destination info object.
	 *  \ingroup DMlinker     */
	template< class Info1, class T1 >
		Std2Linker< Std1FieldLinker< Info1,T1 >,Std1NoLinker > stdLink( T1 Info1:: *awsk1, bool a2 );

	/** \brief Generating function for two way linker based on Std2Linker.
	 *
	 *  \param awsk1 the pointer to member in destination info
	 *  \param awsk2 the pointer to member in source info.
	 *  \return the linker with two way connection, using field pointed by \a awsk1 in destination info object and the field pointed by \a awsk2 in source info object.
	 *  \ingroup DMlinker     */
	template< class Info1, class T1, class Info, class T >
		Std2Linker< Std1FieldLinker< Info1,T1 >,Std1FieldLinker< Info,T > > stdLink( T1 Info1:: *awsk1, T Info:: *awsk );

	/** \brief Generating function for two way linker based on Std2Linker.
	 *
	 *  \param awsk1 the pointer to member in destination info
	 *  \param tab the associative container assigning destination to source.
	 *  \return the linker with two way connection, using field pointed by \a awsk1 in destination info object and associative container.
	 *  \ingroup DMlinker     */
	template< class Info1, class T1, class Map >
		Std2Linker< Std1FieldLinker< Info1,T1 >,Std1AssocLinker< Map > > stdLink( T1 Info1:: *awsk1, Map &tab);

	/** \brief Generating function of one way linker based on Std2Linker.
	 *
	 *  \param a2 boolean parameter take only false,
	 *  \param tab1 associative container assigning  source to destination. 
	 *  \return the linker with one way connection destination to source, basing on associative container.
	 *  \ingroup DMlinker     */
	template< class Map1 >
		Std2Linker< Std1AssocLinker< Map1 >,Std1NoLinker > stdLink( Map1 &tab1, bool a2 );

	/** \brief Generating function for two way linker based on Std2Linker.
	 *
	 *  \param tab1 the associative container assigning source to destination.
	 *  \param awsk the pointer to member in source info
	 *  \return the linker with two way connection, using field pointed by \a awsk in the source info object and associative container.
	 *  \ingroup DMlinker     */
	template< class Map1, class Info, class T >
		Std2Linker< Std1AssocLinker< Map1 >,Std1FieldLinker< Info,T > > stdLink( Map1 &tab1, T Info:: *awsk );

	/** \brief Generating function for two way linker based on Std2Linker.
	 *
	 *  \param tab1 the associative container assigning source to destination .
	 *  \param tab the associative container assigning destination to source.
	 *  \return the linker with two way connection, based on two associative containers.
	 *  \ingroup DMlinker     */
	template< class Map1, class Map >
		Std2Linker< Std1AssocLinker< Map1 >,Std1AssocLinker< Map > > stdLink( Map1 &tab1, Map &tab );

	// wygodne laczenie chooserow, casterow i linkerow w pary za pomoca &
	/**\brief Make pair of choosers.
	 *
	 * Overloaded operator& allows to create easily a pair of choosers \a a and \a b.*/
	template <class  Ch1, class Ch2> std::pair< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator&( Ch1 a, Ch2 b )
		{
			return std::pair< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >(a,b);
		}

	
	/**\brief Make pair of casters.
	 *
	 * Overloaded operator& allows to create easily a pair of casters \a a and \a b.*/
		template <class  Ch1, class Ch2> std::pair< typename Ch1::CastersSelfType,typename Ch2::CastersSelfType >
		operator&( Ch1 a, Ch2 b )
		{
			return std::pair< typename Ch1::CastersSelfType,typename Ch2::CastersSelfType >(a,b);
		}

	/**\brief Make pair of linkers.
	 *
	 * Overloaded operator& allows to create easily a pair of linkers \a a and \a b.*/
	template <class  Ch1, class Ch2> std::pair< typename Ch1::LinkersSelfType,typename Ch2::LinkersSelfType >
		operator&( Ch1 a, Ch2 b )
		{
			return std::pair< typename Ch1::LinkersSelfType,typename Ch2::LinkersSelfType >(a,b);
		}


#include "defs.hpp"
}

#endif