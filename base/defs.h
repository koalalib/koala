#ifndef KOALA_DEFS_H
#define KOALA_DEFS_H

/* \file defs.h \brief Auxiliary definitons
 *
 *  File consists of many useful baisic structures.
 */

#include <algorithm>
#include <cstdlib>
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
	 *  - Loop       = 0x1  - edges with only one vertex.
	 *  - Undirected = 0x2  - undirected edges .
	 *  - Directed   = 0xC  - arcs - directed edge.
	 *  \ingroup DMgraph */
	typedef unsigned char EdgeType;

	// ... i ich orientacje względem wierzchołka.
	/** \brief Edge direction.
	 *
	 *  The type used for variables storing the information about direction of edge.
	 *  Variables of this type can be used as masks.\n
	 *  Bits meaning:
	 *  -  EdLoop   = 0x01  - edge with only one vertex
	 *  -  EdUndir  = 0x02  - undirected edge
	 *  -  EdDirIn  = 0x04  - for directed edge in direction
	 *  -  EdDirOut = 0x08  - for directed edge out direction
	 *  -  EdAll    = 0x0F  - for all above options
	 *   \ingroup DMgraph */
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
	/** \brief Structures for empty vertex info.
	 *
	 *  The empty structure often used as default value for info attributes in vertices.
	 *  \ingroup DMdef*/
	struct EmptyVertInfo { } ;
	/**\brief Structures for empty edge info. 
	 *
	 *  The empty structure often used as default value for info attributes in edges.
	 *  \ingroup DMdef*/
	struct EmptyEdgeInfo { } ;

	template< class VertInfo, class EdgeInfo, class Settings > class Graph;
	template< class VertInfo, class EdgeInfo, class Settings > class Vertex;
	template< class VertInfo, class EdgeInfo, class Settings > class Edge;
	template< EdgeType mask, bool matr > class GrDefaultSettings;


	// Specjalizacje dla wlasnych klas numerycznych (np. liczb wymiernych) pozwola uzywac ich jako danych w
		// algorytmach (np. dlugosci krawedzi). Dlatego w kodach nawet zerowosc jakiejs etykiety sprawdzam metoda.
		/** \brief Numeric types specialization.
		 *  WEN?: to jest taka wersja std::numeric_limits ograniczona do potrzeb algorytmow Koali
		 *  Class allows to choose own numeric types for data in internal Koala algorithms.*/
    template< class T > class NumberTypeBounds
    {
    public:
        //WEN: opisy
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

	/* AlgsDefaultSettings
	 * Wytyczne parametryzujace dzialanie algorytmow biblioteki. Z reguly klasy z procedurami maja postac
	 * NazwaPar<DefaultStructs> oraz pochodna klase Nazwa dzialajaca z ustawieniem DefaultStructs=AlgsDefaultSettings
	 */
	/** \brief Default algorithms settings.
	 *
	 *  This is an useful plug-in that allows to parameterize some algorithms in this library with default values.
	 *  An usual class is declared  <tt> SomeNamePar<class DefaultStructs> </tt> In most cases there is also
	 *  a class \a SomeName which is achieved from the original one by simply setting <tt>DefaultStructs = AlgsDefaultSettings</tt>
	 *  \ingroup DMdef */
	class AlgsDefaultSettings
	{
	public:
		// Typ klasy tablicy asocjacyjnej przypisującej np. wierzchołkowi/krawędzi wartości typu B.
		/** \brief Type of associative container
		 *
		 *  The class is most often used to assign some values (colors weights priority etc.) to vertices (PVertex) or edges (PEdge).
		 *  \tparam A key type.
		 *  \tparam B mapped value type. */
		template< class A, class B > class AssocCont
		{
		public:
			/**\brief Associative container type.
			 *
			 * Define this type as other associative container in order to introduce changes.
			 * Exemplary other possibilities:
			 * - typedef AssocTable < BiDiHashMap<A,B> > Type;
			 * - typedef AssocTable < HashMap<A,B> > Type;
			 * - typedef AssocTable < std::map<A,B> > Type;	 */
			typedef AssocArray< A,B > Type;

			//Nie usuwac komentarzy (przykladowe uzycia) Inne mozliwosci:

			//  typedef AssocTable < BiDiHashMap<A,B> > Type;

            //  typedef AssocTable < HashMap<A,B> > Type;

            //  typedef AssocTable < std::map<A,B> > Type;
		};

		// Typ 2-wymiarowej tablicy assocjacyjnej o kluczu A i wartości B. Kluczami są pary uporzadkowane lub
		// nieuporzadkowane (por. assoctab.h).
		/** \brief Two dimensional associative array.
		 *
		 *  \tparam A the key type.
		 *  \tparam B the mapped value type.
		 *  \tparam type the kind of associative matrix (Koala::AssocMatrixType). */
		template< class A, class B, AssocMatrixType type > class TwoDimAssocCont
		{
		public:
			typedef SimpleAssocMatrix< A,B,type > Type;/**<\brief Define own if intend to change.*/

			//Nie usuwac komentarzy (przykladowe uzycia) Inne mozliwosci:

            //  typedef SimpleAssocMatrix<A,B,type,std::vector< std::vector<typename Privates::SimpleAssocMatrixInternalTypes<A,B>::BlockType> >,Privates::PseudoAssocArray<A,int,AssocTable<BiDiHashMap<A,int> > > > Type;
            //  typedef SimpleAssocMatrix<A,B,type,std::vector< std::vector<typename Privates::SimpleAssocMatrixInternalTypes<A,B>::BlockType> >,Privates::PseudoAssocArray<A,int,AssocTable<HashMap<A,int> > > > Type;
            // mozliwy ale nie zalecany - dostep logarytmiczny:
            //  typedef SimpleAssocMatrix<A,B,type,std::vector< std::vector<typename Privates::SimpleAssocMatrixInternalTypes<A,B>::BlockType> >,Privates::PseudoAssocArray<A,int,AssocTable<std::map<A,int> > > > Type;

			// typedef AssocMatrix< A,B,type > Type;

			//  typedef AssocMatrix<A,B,type,std::vector< Privates::BlockOfAssocMatrix<B> >,Privates::PseudoAssocArray<A,int,AssocTable<BiDiHashMap<A,int> > > > Type;
            //  typedef AssocMatrix<A,B,type,std::vector< Privates::BlockOfAssocMatrix<B> >,Privates::PseudoAssocArray<A,int,AssocTable<HashMap<A,int> > > > Type;
            // mozliwy ale nie zalecany - dostep logarytmiczny:
            //  typedef AssocMatrix<A,B,type,std::vector< Privates::BlockOfAssocMatrix<B> >,Privates::PseudoAssocArray<A,int,AssocTable<std::map<A,int> > > > Type;

            // to sa tak na prawde opakowania do map 2-wymiarowych zorganizowanych tak, jak pokazuje drugi param. szablonu:
            //  typedef  Assoc2DimTable< type, std::map<std::pair<A,A>, B > > Type;
            //  typedef  Assoc2DimTable< type, BiDiHashMap<std::pair<A,A>, B > > Type;
            //  typedef  Assoc2DimTable< type, HashMap<std::pair<A,A>, B > > Type;
		};

		// typ struktury kopca i jego wezla
		/** \brief Heap container.
		 *
		 *  \tparam key the key class.
		 *  \tparam Compare the comparison object function.*/
		template< class Key, class Compare = std::less< Key > >
			class HeapCont
		{
		public:
			typedef FibonHeap< Key,Compare > Type;/**<\brief Define own if intend to change.WEN?:*/
                    typedef FibonHeapNode< Key > NodeType;/**<\brief Define own if intend to change.???*/

			//Nie usuwac komentarzy (przykladowe uzycia) Inne mozliwosci:
            //typedef BinomHeap< Key,Compare > Type;/**<\brief Define own if intend to change.???*/
                    //typedef BinomHeapNode< Key > NodeType;/**<\brief Define own if intend to change.???*/

            //typedef PairHeap< Key,Compare > Type;/**<\brief Define own if intend to change.???*/
                    //typedef PairHeapNode< Key > NodeType;/**<\brief Define own if intend to change.???*/
		};

		// Typ grafu pomocniczego stosowanego wewnątrz procedury.
		/** \brief Auxiliary graph.
		 *  WEN: opis parametrow szablonu (podobnie jak w grafie glownym)
		 *  The structure is used for internal purposes in various procedures.
		 *  \tparam A VertInfo type.
		 *  \tparam B EdgeInfo type
		 *  \tparam mask Edge types in graph. \wikipath{EdgeType,See wiki page for EdgeType.}*/
		template< class A, class B, EdgeType mask> class LocalGraph
		{
		public:
			/** \brief Define in order to set own */
			typedef Graph< A,B,GrDefaultSettings< mask,false > > Type;
		};

		// Czy dostosowywać rozmiar pamięci wyjściowych tablic asocjacyjnych?
		/** \brief Should the out associative container be allocated at the beginning?*/
		enum { ReserveOutAssocCont = true };

		// Wybrany do użytku wewnętrznego algorytm sortowania tablic.
		/**  \brief Container sorting algorithm
		 *
		 * The functions sorts elements in container given by iterators first last. Define own in order to introduce changes.
		 * \tparam Iterator the iterator class for container.
		 * \param first the iterator of the first element in container.
		 * \param last the iterator of the past the last element in container.*/
		template< class Iterator > static void sort( Iterator first, Iterator last )
		{
            std::make_heap( first,last );
            std::sort_heap( first,last );
        }
		// ... i to samo z funkcją porównującą.
		/**  \brief Container sorting algorithm
		*
		* The functions sorts elements in container given by iterators first last. The object function comp is used for comparing elements.
		* Define own in order to introduce changes.
		* \tparam Iterator the iterator class for container.
		* \param first the iterator of the first element in container.
		* \param last the iterator of the past the last element in container.
		* \tparam Comp the class of comparison object function. The functor should give strict weak order.  Similar to one in std::sort.
		* \param comp the comparison object function.*/
		template< class Iterator, class Comp > static void sort( Iterator first, Iterator last, Comp comp )
		{
            std::make_heap( first,last,comp );
            std::sort_heap( first,last,comp );
        }

        //inna mozliwosc: z std::sort
//		template< class Iterator > static void sort( Iterator first, Iterator last )
//		{
//            std::sort( first,last );
//        }
//		template< class Iterator, class Comp > static void sort( Iterator first, Iterator last, Comp comp )
//		{
//            std::sort( first,last,comp );
//        }

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
	 *   \tparam T The type of returned object, the type have minimal requirements similar to STL objects. I.e. it must implement:
	 *   - empty constructor
	 *   - copy constructor
	 *   - destructor
	 *   - operator=
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
		inline T operator()()
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
		template< class A, class B, class C, class D > T operator()(  const A&,const B&,const C&,const D& )
				{ return val; }
		/** \brief Five arguments functor. */
		template< class A, class B, class C,class D, class E > T operator()( const A&,const B&,const C&,const D&,const E& )
				{ return val; }
		/** \brief Six arguments functor. */
		template< class A, class B, class C,class D, class E, class F > T operator()( const A&,const B&,const C&,const D&,const E&,const F& )
				{ return val; }
	};

	// Funkcja tworząca powyższy funktor.
	/** \brief Generating function for constant functor. 
	 * \relates ConstFuctor
	 * \ingroup def*/
	template< class T > ConstFunctor< T > constFun( const T &a = T() )
				{ return ConstFunctor< T >( a ); }

	/* BlackHole
	 * Jesli metoda chce dostac argument wyjsciowy (np. iterator do zapisu ciagu, tablice asocjacyjna) a nas te
	 * wartosci nie interesuja, tylko inne efekty uboczne procedury. Uniwersalna zaślepka w wielu procedurach
	 * biblioteki, pozwala uniknąć zbędnych przeciążeń nazw z różnymi zestawami parametrów.
	 */
	/** \brief Black hole.
	 *
	 *  Sometimes method does more than the user wants. Than the class succor. It can supersede \wikipath{insert iterators, Output_iterator} 
	 *  of containers or associative tables as long as the result is never used by the user.
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
        bool operator==(BlackHole)
            { return true; }
        bool operator!=(BlackHole)
            { return false; }

		BlackHole()
			{}

		// BlackHole również może służyć jako zaślepka dla nie interesujacego nas kontenera asocjacyjnego wymaganego
		// w procedurze; te metody nigdy nie powinny być wykonane, są potrzebne jedynie by kod się kompilował.
		template< class T > inline BlackHole &operator[]( T );
		template< class T, class R > inline BlackHole &operator()( T,R );

		// BlackHole potrafi przekonwertować się na dowolny typ - uwaga j.w.
		template< class T > inline operator T();

		template< class T > inline bool hasKey(T) const;
		inline BlackHole  firstKey() const;
		inline BlackHole  lastKey() const;
		template< class T > inline BlackHole nextKey(T) const;
		template< class T > inline BlackHole prevKey(T) const;
		template< class T > inline int getKeys(T) const;
		void reserve( int )
			{ }
		bool empty() const
			{ return true; }
		bool operator!() const
			{ return true; }
		inline unsigned size() const;
		inline int capacity() const;
		template< class T > bool delKey(T)
			{ return false; };
		void clear()
			{ }

	};


	// Makra blackHole można używać jak zmiennej globalnej typu BlackHole.
	/** \brief BlackHole macro.
	 *
	 *  The macro inserts BlackHole object \wikipath{blackhole}.
	 *  \related BlackHole */
	#define blackHole ((*((Koala::BlackHole*)( &std::cout ))))

	// Test na to, czy argument jest typu BlackHole.
	/** \brief Test if black hole.
	 *
	 *  The	method tests if type \a T is BlackHole. Although it always returns false, 
	 *  there is a specialization of it available for BlackHole type, which returns true. 
	 *  \return false unless the specialization for BlackHole is called.
	 *  \related BlackHole */
	template< class T > bool isBlackHole( const T & )
		{ return false; }
	/* \brief Test if black hole.
	 *
	 *  \return true if the tested type is BlackHole.
	 *  \related BlackHole*/
	inline bool isBlackHole( const BlackHole & )
		{ return true; }

	/* BlackHoleSwitch
	 * Jeśli szablon procedury dostal BlackHole zamiast argumentu (kontenera), a procedura potrzebuje do działania
	 * tego kontenera - tworzy go sobie sama lokalnie. Ta klasa pozwala sprawdzić, czy taki przypadek zaszedł i
	 * przełączyć kontener na odpowiednio: dostarczony lub lokalny (nie będący BlackHolem).
	 */
	/** \brief Switch blackHole to local container
	 *
	 *  If Cont1 is BlackHole method get delivers container of type Cont2 otherwise it returns object of type Cont1. */
	template< class Cont1, class Cont2 > struct BlackHoleSwitch
	{
		// Typ kontenera, na którym będziemy działać.
		typedef Cont1 Type;

		/** \brief Get container.
		 *
		 *  
		 */
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
	 *  This chooser should be used whenever each (or none) object is to be chosen. The chooser works with both edges and vertices. 
	 *  \wikipath{Chooser, See for more details about choosers.}
	 *  \ingroup DMchooser */
	struct BoolChooser 
	{
		bool val;/**<\brief Logic value fixed in constructor returned by each call of function object. */

		// Każdy chooser ma swój wlasny typ zdefiniowany jako ChoosersSelfType.
		/** \brief Chooser obligatory type.
		 *  
		 *  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef BoolChooser ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns value to field \a val. */
		BoolChooser( bool arg = false ): val( arg ) { }

		// Główny operator choosera, testujący prawdziwość jego predykatu.
		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning Boolean value \a val (the same in each call of operator). 
		 *  \param elem the considered object.
		 *  \param gr reference to considered graph (not used in this chooser).
		 *  \return the value \a val. */
		template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &gr ) const { return val; }
	};


	// Funkcja tworząca chooser typu BoolChooser.
	// TODO: sprawdzic, czy rozne przeciazenia funkcji tworzacych nie wywoluja niejednoznacznosci w rozstrzyganiu przeciazen
	/** \brief Generating  function of fixed chooser (BoolChooser).
	 *
	 *  The function generates BoolChooser function object, that returns value \a arg for each call of operator is called.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \param arg Boolean that will be returned by each call of the chooser.
	 *  \ingroup DMchooser 
	 *  \related BoolChooser */
	inline BoolChooser stdChoose( bool arg ) { return BoolChooser( arg ); }

	/* ValChooser
	 * sprawdza, czy testowany element to podana (ustalona) wartosc
	 */
	/** \brief Value chooser
	 *
	 *  Function object that compares the fixed value \a val defined in constructor to the one given by parameter \a elem in calls of overloaded operator().
	 *  Chooser should be used whenever simple comparison to fixed value is necessary,
	 *   for example only one object is to be chosen. The chooser works with both edges and vertices. 
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Elem class of compared value.
	 *  \ingroup DMchooser */
	template< class Elem > struct ValChooser 
	{
		Elem val; /**< \brief value fixed in constructor */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
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
	 *
	 *  The function generates ValChooser function object that tests whether checked element equals \a arg. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Elem the type of tested element, possibly deduced from \a arg
	 *  \param arg the value for which chooser returns true.
	 *  \ingroup DMchooser
	 *  \related ValChooser */
	template< class Elem > ValChooser< Elem > stdValChoose( Elem arg ) { return ValChooser< Elem >( arg ); }

	/* SetChooser
	 * sprawdza, czy testowany element jest w podanym zbiorze (Koala::Set)
	 */
	/** \brief Set chooser
	 *
	 *  Function object that checks if \a elem (the parameter in call of overloaded operator()) belongs to the set defined in constructor.
	 *  Chooser should be used whenever elements from given set are to be chosen. It works with both edges and vertices. 
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Elem the class of compared value.
	 *  \ingroup DMchooser*/
	template< class Elem > struct SetChooser
	{
		Koala::Set< Elem * > set;/**< \brief Fixed in constructor set of elements of \a Elem type. */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef SetChooser< Elem > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Determines the set of elements. */
		SetChooser( const Koala::Set< Elem * > &arg = Koala::Set< Elem * >() ): set( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if object \a elem belongs to the set defined in constructor and false otherwise.
		 *  \param elem the checked element.
		 *  \param gr reference to considered graph (not used in this chooser).
		 *  \return true if \a elem belongs to the \a set, false otherwise.	 */
		template< class Graph > bool operator()( Elem *elem, const Graph &gr ) const { return set.isElement( elem ); }
	};
	/** \brief Generating function of set chooser (SetChooser).
	 *
	 *  The function generates SetChooser object function that for call of function call operator returns true as long as 
	 *  element \a arg belong to Set. \wikipath{chooser, Get more information about choosers.}  
	 *  \tparam Elem the type of tested elements.
	 *  \param arg the set of elements (pointers) for which chooser function call returns true.
	 *  \related SetChooser
	 *  \ingroup DMchooser*/
	template< class Elem >
		SetChooser< Elem > stdChoose( Koala::Set< Elem * > arg ) { return SetChooser< Elem >( arg ); }

	/* ContainerChooser
	 * sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Container element chooser
	 *
	 *  Function object that checks if parameter \a elem in call of overloaded operator() belongs to the container defined in constructor.
	 *  The container is given be iterators to the first and to the past-the-last element. It should like STL containers serve std::find algorithm. 
	 *  Since the container is not copied it is users prerogative to keep iterators up to date.
	 *  The chooser should be used whenever elements from given container should be considered.
	 *  It works with both edges and vertices. 
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Iter the iterator class used to access a container.
	 *  \ingroup DMchooser*/
	template< class Iter > struct ContainerChooser
	{
		Iter begin; /**< \brief iterator to the first element of container */
		Iter end; /**< \brief iterator to the past-the-end element of container */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef ContainerChooser< Iter > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns value to iterators defining the container.
		 *  \param abegin the iterator to the first element in the container with elements. 
		 *  \param aend past-the-end element of the container with elements.*/
		ContainerChooser( Iter abegin = Iter(), Iter aend = Iter() ): begin( abegin ), end( aend ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if objects \a elem belongs to the container defined in constructor and false otherwise.
		 *  \param elem the checked element.
		 *  \param gr the considered graph (not used in this chooser).
		 *  \return true if \a elem belongs to the container, false otherwise. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return std::find( begin,end,elem ) != end; }
	};

	/** \brief Generating function of container element chooser (ContainerChooser).
	 *
	 *  The function generates chooser object function ContainerChooser that chooses only elements from container
	 *  given by iterators \a begin and \a end.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Iter the type of container iterator.
	 *  \param begin the iterator to the first element of container.
	 *  \param end the iterator to the past-the-end element of container.
	 *  \ingroup DMchooser
	 *  \related ContainerChooser*/
	template< class Iter >
		ContainerChooser< Iter > stdChoose( Iter begin, Iter end ) { return ContainerChooser< Iter >( begin,end ); }

	/* ObjChooser
	 * sprawdza wartosc podanego obiektu funkcyjnego dla testowanego elementu
	 */
	/** \brief Function object chooser.
	 *
	 *  Wraps self-made function object.
	 *  Function object should take two parameters chosen element and considered graph. It should return value convertible to Boolean type.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Obj the function object class used to choose entities.
	 *  \ingroup DMchooser */
	template< class Obj > struct ObjChooser
	{
		mutable Obj funktor; /**< \brief Function object defined in constructor.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef ObjChooser< Obj > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns function object to functor. */
		ObjChooser( Obj arg = Obj() ): funktor( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  The function call operator returning Boolean value the same as the function object defined in the constructor.
		 *  \param elem the checked element.
		 *  \param graph the considered graph.
		 *  \return the same vales (casted on bool) as functor \a arg. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return (bool)funktor( elem,graph ); }
	};

	// liera F w nazwie - dla chooserow dzialajaych z funktorami
	/** \brief Generating function of function object chooser (ObjChooser).
	 *
	 *  The function wraps object function \a arg and generates chooser object function. 
	 *  \tparam Obj the type of wrapped object function.
	 *  \param arg the wrapped object function.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser
	 *  \related ObjChooser*/
	template< class Obj > ObjChooser< Obj > stdFChoose( Obj arg ) { return ObjChooser< Obj >( arg ); }

	//te choosery juz zagladaja do konkretnych pol rekordow info wierz/krawedzi

	/* FieldValChooser
	 * test, czy pole w info ma podana wartosc
	 */
	/** \brief Info field value chooser
	 *
	 *  Function object that checks if the attribute \a val matches an element of info object field pointed by \a wsk.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FieldValChooser
	{ 
		T Info:: *wsk; /**<\brief Pointer to member.*/
		T val; /**< \brief Desired value. */

		/** \brief Chooser obligatory type.
		 *
		 *  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef FieldValChooser< Info, T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns values to \a val and pointer to member \a wsk i.e. decides which attribute of info are to be checked and what value are they to be equal to.	 
		 *  \param arg pointer to member in Info object.
		 *  \param arg2 the desired value of tested field.*/
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

	/** \brief Generating function of field value chooser (FieldValChooser).
	 * 
	 *  The function generates chooser elements in which info object filed pointed by \a wsk math \a arg.
	 *  \tparam Info the type of Info object.
	 *  \tparam T the type tested member.
	 *  \param wsk pointer to tested member in Info object.
	 *  \param arg desired value of tested field.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \related FieldValChooser
	 *  \ingroup DMchooser*/
	template< class Info, class T >
		FieldValChooser< Info,T > fieldChoose( T Info:: *wsk, T arg ) { return FieldValChooser< Info,T >( wsk,arg ); }

	/* FieldValChooserL
	 * test, czy pole w info jest mniejsze od podanej wartosci
	 */
	/** \brief Less info field value chooser
	 *
	 *  Function object that chooses elements for which the attribute pointed by \a wsk in info object is 
	 *  lower (operator< on type T is used) then  \a val (set in constructor).
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FieldValChooserL
	{
		T Info:: *wsk; /**< \brief Pointer to member.*/
		T val; /**< \brief Value to compare to fixed in constructor */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef FieldValChooserL< Info, T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns values to \a val which is compared to proper attribute in info object
		 *  and sets pointer to member \a wsk i.e. decides which members of info is to be considered.
		 *  \param arg pointer to member
		 *  \param arg2 the compared value*/
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

	/** \brief Generating function of less field value chooser (FieldValChooserL).
	 *
	 *  The function generates FieldValChooserL chooser object function that returns true if and only if field pointed by \a wsk in \a Info object
	 *  is lower then \a arg. 
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \param wsk pointer to tested member in \a Info object.
	 *  \param arg the desired value of approved elements.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser*/
	template< class Info, class T > FieldValChooserL< Info,T >
		fieldChooseL( T Info:: *wsk, T arg ) { return FieldValChooserL< Info,T >( wsk,arg ); }

	/* FieldValChooserG
	 * test, czy pole w info jest wieksze od podanej wartosci
	 */
	/** \brief Greater info field value chooser
	 *
	 *  Function object that chooses elements for which the attribute pointed by \a wsk in info object is
	 *  greater (operator< on type T is used) then \a val (set in constructor).
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FieldValChooserG 
	{
		T Info:: *wsk;/**< \brief Pointer to member.*/
		T val;/**< \brief Value to compare to fixed in constructor */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef FieldValChooserG< Info, T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Assigns values to \a val that is compared to proper info attribute 
		 *  and pointer to member \a wsk that chooses the compared attribute.
		 *  \param arg pointer to member
		 *  \param arg2 the compared value */
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

	/** \brief Generating function of greater field value chooser (FieldValChooserG).
	 *
	 *  The function generates FieldValChooserG chooser object function that returns true if and only if field pointed by \a wsk in \a Info object
	 *  greater than \a arg. \wikipath{chooser, Get more information about choosers.} 
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \param wsk pointer to tested member in \a Info object.
	 *  \param arg the desired value of approved elements.
	 *  \related FieldValChooserG
	 *  \ingroup DMchooser*/
	template< class Info, class T > FieldValChooserG< Info,T >
	
		fieldChooseG( T Info:: *wsk, T arg ) { return FieldValChooserG< Info,T >( wsk,arg ); }

	/* FielBoolChooser
	 * test, czy pole w info ma wartosc prawda (jego typ musi byc konwertowalny do bool)
	 */
	/** \brief Boolean info field chooser
	 *
	 *  Function object that checks if certain filed of element \a elem from
	 *  overloaded \p operator() is convertible to true value.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info class of object info.
	 *  \tparam T type of compared field.
	 *  \ingroup DMchooser */
	template< class Info, class T > struct FieldBoolChooser 
	{
		T Info:: *wsk;/**< \brief Pointer to tested member.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef FieldBoolChooser< Info,T > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Decides which members of info are to be checked. 
		 *  \param arg pointer to tested member.*/
		FieldBoolChooser( T Info::*arg = 0): wsk( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true as long as filed pointed by \a wsk in \a elem info object is convertible to true.
		 *  \param elem the checked object.
		 *  \param graph the considered graph.
		 *  \return true if pointed by \a wsk member of info in object \a elem is true.
		 *  (convertible to true value), false otherwise. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return bool( elem->info.*wsk ); }
	};

	/** \brief Generating function of bool field chooser (FielBoolChooser).
	 *
	 *  The function generates function object FielBoolChooser that tests if info filed pointed by \a wsk is convertible to true.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \param wsk pointer to tested member in \a Info object.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \related FielBoolChooser
	 *  \ingroup DMchooser*/
	template< class Info, class T >
		FieldBoolChooser< Info,T > fieldChoose( T Info:: *wsk ) { return FieldBoolChooser< Info,T >(wsk); }

	/* FielObjChooser
	 * wlasny obiekt lub funkcja, ktora ma sie wykonywac dla konkretnego pola z info
	 */
	/** \brief Functor wrapper.
	 *
	 *  Function object that checks if the given \a functor returns value convertible to true for a certain (pointed by \a wsk) field of info object.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Obj the function object class that provides function testing the field. The object function must work with type of pointed member. 
	 *  \ingroup DMchooser */
	template< class Info, class T, class Obj > struct FieldObjChooser 
	{
		T Info:: *wsk; /**< \brief the pointer to tested member.*/
		mutable Obj funktor; /**< \brief functor testing the member. */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef FieldObjChooser< Info, T, Obj > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  \param awsk to pointer to tested member
		 *  \param afun the wrapped object function */
		FieldObjChooser( T Info:: *awsk = 0, Obj afun = Obj() ): wsk( awsk ), funktor( afun ) { }
        
		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if wrapped function object returns true for filed pointed by \a wsk in \a elem info object.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return the same value as wrapped objet. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return (bool)funktor( elem->info.*wsk ); }
	};

	/** \brief Generating  function of FieldObjChooser.
	 *
	 *  The function generates function object FieldObjChooser that tests if info filed pointed by \a wsk is accepted by functor \a obj.
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Obj the type of wrapped function object.
	 *  \param wsk pointer to tested member in \a Info object.
	 *  \param obj the wrapped function object. Should implement overloaded function call operator for single parameter of pointed member type   
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \related FieldObjChooser
	 *  \ingroup DMchooser*/
	template< class Info, class T, class Obj > FieldObjChooser< Info,T,Obj >
		fieldFChoose( T Info::*wsk, Obj obj ) { return FieldObjChooser< Info,T,Obj >( wsk,obj ); }

	/* FielSetChooser
	 * test, czy podane pole w info ma wartosc z danego zbioru (Koala::Set)
	 */
	/** \brief Info field value belongs to set chooser.
	 *
	 *  Function object that checks if prespecified attribute of info object belongs to the set defined in constructor.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Z the type of objects in set.
	 *  \ingroup DMchooser */
	template< class Info, class T, class Z > struct FieldSetChooser 
	{
		T Info:: *wsk; /**<\brief pointer to tested member in info object*/
		Koala::Set< Z > set;/**<\brief the set of allowed values*/

		/** \brief Chooser obligatory type.
		 *
		 *  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef FieldSetChooser< Info, T, Z > ChoosersSelfType;

		/**\brief Constructor
		 *
		 * The constructor sets pointer to tested member \a awsk and initializes set of allowed values \a aset.*/
		FieldSetChooser( T Info:: *awsk = 0, const Koala::Set< Z > &aset = Koala::Set< Z >() ):
			wsk( awsk ), set( aset ) { }
		
		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if and only if value pointed by \a wsk in info attribute of \a elem belongs to \a set.
		 *  \param elem the checked object.
		 *  \param graph the considered graph.
		 *  \return true if and only if pointed attribute in \a elem info object belong to \a set. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &graph ) const { return set.isElement( elem->info.*wsk ); }
	};

	/** \brief Generating  function of FieldSetChooser.
	 * 
	 *  FieldSetChooser function object is generated. The functor test if field pointed by \a wsk
	 *  in info object of tested element belongs to \a set.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Z the type of objects in set.
	 *  \param wsk pointer to tested member.
	 *  \param set the set of approved values.
	 *  \related FieldSetChooser
	 *  \ingroup DMchooser*/
	template< class Info, class T, class Z > FieldSetChooser< Info,T,Z >
		fieldChoose( T Info::*wsk, Koala::Set< Z > set ) { return FieldSetChooser< Info,T,Z >( wsk,set ); }

	/* FielContainerChooser
	 * sprawdza, czy testowany element jest w przedziale miedzy podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Info field value belong to container chooser.
	 *
	 *  Function object that checks if certain field of info object belongs to the container given by iterators. 
	 *  Container should behave like stl one, function std::find must be applicable. 
	 *  The container is not copied and it is users prerogative to keep iterators valid.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Iter the type of iterator for container with tested elements.
	 *  \ingroup DMchooser */
	template< class Info, class T, class Iter > struct FieldContainerChooser 
	{
		T Info:: *wsk;/**< \brief Pointer to tested member.*/
		Iter begin/**< \brief The iterator to the first element of container.*/, end;/**< \brief The iterator to the past-the-end element of container.*/

		/** \brief Chooser obligatory type.
		 *
		 *  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef FieldContainerChooser< Info, T, Iter > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The constructor sets pointer to tested member and determines the container with compared elements.
		 *  \param awsk the pointer to tested member
		 *  \param a The iterator to the first element of container.
		 *  \param b The iterator to the past-the-end element of container.*/
		FieldContainerChooser( T Info:: *awsk = 0, Iter a = Iter(), Iter b = Iter() ):
			wsk( awsk ), begin( a ), end( b ) {}
        
		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if value pointed by \a wsk in info attribute of \a elem belongs to container.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if pointed attribute in \a elem info object belong to the container. */
		template< class Elem, class Graph > bool
			operator()( Elem *elem, const Graph &graph ) const { return std::find( begin,end,elem->info.*wsk ) != end; }
	};

	/** \brief Generating  function of FielContainerChooser.
	 * 
	 *  FieldContainerChooser function object is generated. The functor test if field pointed by \a wsk
	 *  in info object of tested element belongs to container given by iterators.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Info the class of object info.
	 *  \tparam T the type of compared field.
	 *  \tparam Iter the iterator type.
	 *  \param wsk pointer to tested member.
	 *  \param b the iterator to the first element of container.
	 *  \param e the iterator to the past-the-last element of container.
	 *  \related FieldContainerChooser
	 *  \ingroup DMchooser*/
	template< class Info, class T, class Iter > FieldContainerChooser< Info,T,Iter >
		fieldChoose( T Info::*wsk, Iter b, Iter e ) { return FieldContainerChooser< Info,T,Iter >( wsk,b,e ); }

	// choosery decydujace na podstawie wartosci przypisanej elementowi w podanej tablicy asocjacyjnej

	/* AssocHasChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy
	 * kontener asocjacyjny jest kopiowany do choosera: zaleta - nie zalezy od dalszego stanu kontenera, 
	 * wada - oczywiscie czas i pamiec. Istniejaca dalej specjalizacja posluguje sie natomiast wskaznikiem do zewnetrzengo kont.
	 */
	/** \brief Is key in associative container chooser. 
	 *
	 *  Function object that checks if the element (pointer) is  a key in associative container defined in constructor.
	 *  Mind that associative container is copied, which is a waste of resources, however result is not influenced by further changes in container.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocHasChooser 
	{
		Cont cont;/**<\brief The container with approved elements.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocHasChooser< Cont > ChoosersSelfType;

		/**\brief Constructor
		 *
		 * The constructor sets the associative container.*/
		AssocHasChooser( const Cont &arg = Cont() ): cont( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if and only if \a elem is a key in the container.
		 *  \param elem the checked object.
		 *  \param graph the considered graph.
		 *  \return true if and only if \a elem is a key in the container. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ); }
	};

	/** \brief Generating  function of AssocHasChooser.
	 * 
	 *  AssocHasChooser function object is generated. The functor test if element is a key in associative container.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \param arg the associative container with approved elements.
	 *  \related AssocHasChooser
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocHasChooser< Cont > assocKeyChoose( Cont arg ) { return AssocHasChooser< Cont >( arg ); }

	/* AssocBoolChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc odpowiada true (typ
	 * wartosci musi byc konwertowalny do bool)
	 */
	/** \brief Has true mapped value chooser.
	 *
	 *  Function object that checks if the element has a mapped value convertible to true in the associative container defined in constructor.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocBoolChooser 
	{
		Cont cont;/**<\brief The container with approved elements.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocBoolChooser< Cont > ChoosersSelfType;

		/**\brief Constructor
		 *
		 * The constructor sets the associative container.*/
		AssocBoolChooser(const Cont &arg = Cont()) : cont(arg) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if and only if \a elem is a key and the mapped value is convertible to true.
		 *  \param elem the checked object.
		 *  \param graph the considered graph.
		 *  \return true if and only if \a elem is a key in the container and the mapped value is true (or convertible to true). */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && (bool)cont[elem]; }
	};

	/** \brief Generating  function of AssocBoolChooser.
	 * 
	 *  AssocBoolChooser function object is generated. The functor test if element is a key in associative container 
	 *  and if the mapped value is convertible to true.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \param arg the associative container that maps elements to values convertible to Boolean type.
	 *  \related AssocBoolChooser
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocBoolChooser< Cont > assocChoose( Cont arg ) { return AssocBoolChooser< Cont >( arg ); }

	/* AssocValChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest rowna zadanej
	 * kontener asocjacyjny jest kopiowany do choosera: zaleta - nie zalezy od dalszego stanu kontenera, wada - oczywiscie czas i pamiec. Istniejaca dalej specjalizacja posluguje sie natomiast wskaznikiem do zewnetrzengo kont.*/
	/** \brief Mapped value chooser.
	 *
	 *  The chooser is equipped with associative container. Each call of function call operator test if element mapped value matches the given value.
	 *  Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  The associative container is copied. Hence, further change in container do not influence the effect.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooser
	{
		Cont cont;/**<\brief The container with approved elements.*/

		typename Cont::ValType val;/**<\brief The desired value of mapped value.*/
		typedef typename Cont::ValType SelfValType;
		/** \brief Chooser obligatory type.
		 *
		 *  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocValChooser< Cont > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The constructor sets up the associative container and chosen mapped value.*/
		AssocValChooser( const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType() ):
			cont( arg ),val( aval ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and mapped value matches \a val.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value equals \a val. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] == val; }
	};

	/** \brief Generating  function of AssocValChooser.
	 * 
	 *  AssocValChooser function object is generated. The functor test if element is a key in associative container 
	 *  and if the mapped value equals \a val.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \param arg the associative container.
	 *  \related AssocValChooser
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooser< Cont >
		assocChoose( Cont arg, typename Cont::ValType val ) { return AssocValChooser< Cont >( arg,val ); }

    /* AssocValChooserL
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest mniejsza od zadanej
	 */
	/** \brief Choose elements for which mapped value less then common value.
	 *
	 *  The functor is equipped with associative container. Each call of function call operator tests if element mapped value is smaller then prespcified value. 
	 *  Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  The chooser works for both edges and vertices. Mind that mapped values in container must allow operator<. 
	 *  The associative container is copied. Hence, further change in container do not influence the effect.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserL
	{
		Cont cont;/**<\brief The associative container for elements.*/

		typename Cont::ValType val;/**<\brief The desired value of mapped value.*/
		typedef typename Cont::ValType SelfValType;

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocValChooserL< Cont > ChoosersSelfType;

		/** \brief Constructor
		*
		*  The constructor sets up the associative container and the value to compare.*/
		AssocValChooserL(const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType()) :
			cont( arg ),val( aval ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and mapped value is lower then \a val.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value is smaller then \a val. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] < val; }
	};

	/** \brief Generating  function of AssocValChooserL.
	*
	*  AssocValChooserL function object is generated. The functor test if element is a key in associative container
	*  and if the mapped value is lower then \a val.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \param arg the associative container.
	*  \related AssocValChooserL
	*  \ingroup DMchooser*/
	template< class Cont > AssocValChooserL< Cont >
		assocChooseL( Cont arg, typename Cont::ValType val ) { return AssocValChooserL< Cont >( arg,val ); }

    // wszystko jak wyzej + Cont::ValType musi obsluzyc >
	/* AssocValChooserG
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest wieksza od zadanej
	 */
	/** \brief Choose elements for which mapped value greater then common value.
	*
	*  The functor is equipped with associative container. Each call of function call operator tests if element mapped value is greater then prespcified value.
	*  Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	*  The chooser works for both edges and vertices. Mind that mapped values in container must allow operator>.
	*  The associative container is copied. Hence, further change in container do not influence the effect.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserG
	{
		Cont cont;/**<\brief The associative container with elements and tested mapped values.*/

		typename Cont::ValType val;/**<\brief The desired value of mapped value.*/
		typedef typename Cont::ValType SelfValType;

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocValChooserG< Cont > ChoosersSelfType;

		/** \brief Constructor
		*
		*  The constructor sets up the associative container and the value to compare.*/
		AssocValChooserG(const Cont &arg = Cont(), typename Cont::ValType aval = SelfValType()) :
			cont( arg ),val( aval ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and the mapped value is greater then \a val.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value is greater then \a val. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && cont[elem] > val; }
	};

	/** \brief Generating  function of AssocValChooserG.
	*
	*  AssocValChooserG function object is generated. The functor test if element is a key in associative container
	*  and if the mapped value is greater then \a val.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \param arg the associative container.
	*  \related AssocValChooserG
	*  \ingroup DMchooser*/
	template< class Cont > AssocValChooserG< Cont >
		assocChooseG( Cont arg, typename Cont::ValType val ) { return AssocValChooserG< Cont >( arg,val ); }

    // wszystko jak wyzej
	/* AssocSetChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc jest elementem podanego
	 * zbioru (Koala::Set)
	 */
	/** \brief Choose if mapped value belongs to set.
	 *
	 *  Function object that checks if the element mapped value belongs to the prespecified set. 
	 *  Furthermore, the element needs to be a key in the associative container. 
	 *  Both, the associative container and the set are defined in the constructor.
	 *  They are copied. Hence, their further change do not influence the effect.
	 *  The chooser works with both edges and vertices.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocSetChooser
	{
		Cont cont;/**<\brief The associative container for elements.*/

		Koala::Set< typename Cont::ValType > set;/**<\brief The set with approved mapped values.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocSetChooser< Cont > ChoosersSelfType;

		/** \brief Constructor
		*
		*  The constructor sets up the associative container and the set of approved mapped values to compare.*/
		AssocSetChooser(const Cont &arg = Cont(), const Koala::Set< typename Cont::ValType > &aset =
			Koala::Set< typename Cont::ValType >() ): cont( arg ),set( aset ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and the mapped value belongs to set \a set.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value belongs to set \a set. */
		template< class Elem, class Graph > bool
			operator()( Elem *elem, const Graph & ) const { return cont.hasKey( elem ) && set.isElement( cont[elem] ); }
	};

	/** \brief Generating  function of AssocSetChooser.
	*
	*  AssocSetChooser function object is generated. The functor test if element is a key in associative container
	*  and if the mapped value belongs to the set.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \param arg the associative container.
	*  \param set the set with approved mapped values.
	*  \related AssocSetChooser
	*  \ingroup DMchooser*/
	template< class Cont > AssocSetChooser< Cont > assocChoose(Cont arg,
		const Koala::Set< typename Cont::ValType > &set ) { return AssocSetChooser< Cont >( arg,set ); }

    //chodzi o STL-like containers tj. obslugujace algorytm std::find
    //jest odpowiedzialnoscia uzytkownika, by przedzial iteratorow byl wazny podczas uzywania choosera (kontener nie jest kopiowany)
	/* AssocContainerChooser
	 * test, czy element jest kluczem znajdujacym sie w tablicy, a przypisana w niej wartosc lezy w przedziale miedzy
	 * podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Choose if mapped value belongs to container.
	 *
	 *  Function object that checks if the element mapped value belongs to the prespecified another container 
	 *  (given with iterators \wikipath{iterator, Get more data about iterators). 
	 *  Furthermore, the element needs to be a key in the associative container. 
	 *  Both, the associative container and the container are defined in the constructor.
	 *  The associative array is copied. But the container with approved mapped values is not and it is user prerogative to keep it valid.
	 *  The container should be stl-like and must allow stl::find algorithm.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \tparma Iter the iterator type of container to search mapped values in for.
	 *  \ingroup DMchooser */
	template< class Cont, class Iter > struct AssocContainerChooser
	{
		Iter begin/**<\brief the iterator to the first element of the container.*/, end/**<\brief the iterator to the past-the-end element of the container.*/;
		Cont cont;/**<\brief The associative container*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocContainerChooser< Cont, Iter > ChoosersSelfType;

		/**\brief Constructor
		 *
		 *  In the constructor the associative container is copied and the container with approved mapped values is set up (via iterators.
		 *  \param acont the associative container.
		 *  \param abegin the iterator to the first element of container with approved mapped values.
		 *  \param aend the iterator to the past-the-last element of container with approved mapped values.*/
		AssocContainerChooser( const Cont &acont = Cont(), Iter abegin = Iter(), Iter aend = Iter() ):
			cont( acont ), begin( abegin ), end( aend ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if and only if \a elem is a key in associative array and the mapped value belongs container given by iterators.
		 *  \param elem the checked object.
		 *  \param graph the considered graph.
		 *  \return true if and only if \a elem is a key in the container and the mapped value belongs to the container with approved mapped values. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &) const
			{ return cont.hasKey( elem ) && std::find( begin,end,cont[elem] ) != end; }
	};

	/** \brief Generating  function of AssocContainerChooser.
	 *
	 *  AssocContainerChooser function object is generated. The functor test if element is a key in associative container
	 *  and if the mapped value is an element in contaier given by iterators \a begin \a end.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \tparam Iter the iterator type.
	 *  \param cont the associative container.
	 *  \param begin the iterator to the first element of container with approved mapped values.
	 *  \param end the iterator to the past-the-last element of container with approved mapped values.
	 *  \related AssocContainerChooser
	 *  \ingroup DMchooser*/
	template< class Cont, class Iter > AssocContainerChooser< Cont, Iter >
		assocChoose( Cont cont, Iter begin, Iter end ) { return AssocContainerChooser< Cont,Iter >( cont,begin,end ); }

    /* AssocObjChooser
	 * sprawdza wartosc podanemgo obiektu funkcyjnego przypisana w tablicy asocjacyjnej testowanemu elementowi
	 */
	/** \brief Choose if functor returns true for mapped value.
	 *
	 *  The function object is equipped with functor and associative container both set up in constructor.
	 *  The element to be chosen must be a key in associative container and function should return value convertible to true for mapped  value.
	 *  The chooser works for both edges and vertices.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \tparma Obj the functor class.
	 *  \ingroup DMchooser */
	template< class Cont, class Obj > struct AssocObjChooser
	{
		mutable Obj funktor;/**<\brief The function object.*/
		Cont cont;/**<\brief The associative container.*/

		/** \brief Chooser obligatory type.
		 *
		 *  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocObjChooser< Cont, Obj > ChoosersSelfType;
		/**\brief Constructor
		 *
		 * The associative container and testing function object are set up. */
		AssocObjChooser( const Cont &acont = Cont(), Obj arg = Obj() ): cont( acont ), funktor( arg ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning true if and only if \a elem is a key in associative array and the function object returns true for mapped value.
		 *  \param elem the checked object.
		 *  \param graph the considered graph.
		 *  \return true if and only if \a elem is a key in the container and the functor returns true for mapped value. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &graph) const
			{ return cont.hasKey( elem ) && (bool)funktor( cont[elem] ); }
	};

	/** \brief Generating  function of AssocObjChooser.
	*
	*  AssocObjChooser function object is generated. The functor test if the element is a key in associative container
	*  and if functor \a arg returns valeu convertible to true for mapped value.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \tparam Obj the function object type.
	*  \param cont the associative container.
	*  \param arg the function object
	*  \related AssocObjChooser
	*  \ingroup DMchooser*/
	template< class Cont, class Obj > AssocObjChooser< Cont, Obj >
		assocFChoose( Cont cont, Obj arg ) { return AssocObjChooser< Cont,Obj >( cont,arg ); }

	// Wszystkie choosery operujace na tablicach asocjacyjnych maja wersje dzialajace z zewnetrzna tablica
	// podawana przez wskaznik. Unikamy kopiowania tablic, uzywajac trzeba uwazac, by tablica wciaz zyla przez
	// caly czas zycia choosera. Funkcje tworzace maja przedrostek ext i pobieraja adres tablicy
	/* AssocHasChooser
	 *
	 */
	/** \brief Is key in associative container chooser.
	 *
	 *  Function object that checks if the element (pointer) is  a key in associative container defined in constructor.
	 *  This is specialization of AssocHasChooser for pointers and that associative container is not copied.
	 *  It is users prerogative to keep the container up to date.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocHasChooser< Cont * >
	{
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocHasChooser< Cont * > ChoosersSelfType;

		/**\brief Container
		 *
		 *  The constructor sets the associative container.*/
		AssocHasChooser(const Cont *arg = 0) : cont(arg) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in the container.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container. */
		template< class Elem, class Graph >
			bool operator()( Elem *elem, const Graph &) const { return cont->hasKey( elem ); }
	};

	/** \brief Generating  function of AssocHasChooser< Cont * >.
	 *
	 *  AssocHasChooser< Cont * > function object is generated. The functor test if element is a key in associative container.
	 *  The chooser is a specialized for pointers version of AssocHasChooser which do not copy the associative container.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \param arg the pointer to associative container with approved elements.
	 *  \related AssocHasChooser
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocHasChooser< Cont * > extAssocKeyChoose( const Cont *arg ) { return AssocHasChooser< Cont * >( arg ); }

	/** \brief Has true mapped value chooser.
	 *
	 *  Function object that checks if the element has a mapped value convertible to true in the associative container defined in constructor.
	 *  The chooser works for both edges and vertices. 
	 *  This is specialization of AssocBoolChooser for pointers and that associative container is not copied.
	 *  It is users prerogative to keep the container up to date.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocBoolChooser< Cont * >
	{
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocBoolChooser< Cont * > ChoosersSelfType;

		/**\brief Constructor
		*
		* The constructor sets the associative container.*/
		AssocBoolChooser(const Cont *arg = 0) : cont(arg) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key and the mapped value is convertible to true.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value is true (or convertible to true). */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &) const
			{ return cont->hasKey( elem ) && (bool)cont->operator[]( elem ); }
	};

	/** \brief Generating  function of AssocBoolChooser< Cont * >.
	 * 
	 *  AssocBoolChooser< Cont * > function object is generated. The functor test if element is a key in associative container 
	 *  and if the mapped value is convertible to true. The chooser is specialized version of AssocBoolChooser for pointers
	 *  which do not copy the container.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \param arg the pointer to associative container that maps elements to values convertible to Boolean type.
	 *  \related AssocBoolChooser
	 *  \ingroup DMchooser*/
	template< class Cont >
		AssocBoolChooser< Cont * > extAssocChoose( const Cont *arg ) { return AssocBoolChooser< Cont * >( arg ); }

	/* AssocValChooser
	 *
	 */
	/** \brief Mapped value chooser.
	 *
	 *  The chooser is equipped with associative container. Each call of function call operator test if element mapped value matches the given value.
	 *  Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  This is specialization of AssocHasChooser for pointers and that associative container is not copied.
	 *  It is users prerogative to keep the container up to date.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooser< Cont * >
	{
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		typename Cont::ValType val;/**<\brief The desired value of mapped value.*/
		typedef typename Cont::ValType SelfValType;

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocValChooser< Cont * > ChoosersSelfType;

		/** \brief Constructor
		*
		*  The constructor sets up the associative container and chosen mapped value.*/
		AssocValChooser(const Cont *arg = 0, typename Cont::ValType aval = SelfValType()) :
			cont( arg ), val( aval ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and mapped value matches \a val.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value equals \a val. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &) const
			{ return cont->hasKey( elem ) && cont->operator[]( elem ) == val; }
	};

	/** \brief Generating  function of AssocValChooser< Cont * >.
	 * 
	 *  AssocValChooser< Cont * > function object is generated. The functor test if element is a key in associative container 
	 *  and if the mapped value equals \a val. This a specialization of AssocValChooser for pointers
	 *  and the associative container is not copied.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \param arg the pointer to associative container.
	 *  \related AssocValChooser
	 *  \ingroup DMchooser*/
	template< class Cont > AssocValChooser< Cont * >
		extAssocChoose( const Cont *arg, typename Cont::ValType val ) { return AssocValChooser< Cont * >( arg,val ); }

	/* AssocValChooserG
	 *
	 */
	/** \brief Choose elements for which mapped value greater then common value.
	 *
	 *  The functor is equipped with associative container. Each call of function call operator tests if element mapped value is greater then prespcified value.
	 *  Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  The chooser works for both edges and vertices. Mind that mapped values in container must allow operator>.
	 *  This is specialization of AssocValChooserG for pointers and that associative container is not copied.
	 *  It is users prerogative to keep the container up to date.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserG< Cont * >
	{
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		typename Cont::ValType val;/**<\brief The desired value of mapped value.*/
		typedef typename Cont::ValType SelfValType;

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocValChooserG< Cont * > ChoosersSelfType;

		/** \brief Constructor
		*
		*  The constructor sets up the associative container and the value to compare.*/
		AssocValChooserG(const Cont *arg = 0, typename Cont::ValType aval = SelfValType()) :
			cont( arg ), val( aval ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and the mapped value is greater then \a val.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value is greater then \a val. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &) const
			{ return cont->hasKey( elem ) && cont->operator[]( elem ) > val; }
	};

	/** \brief Generating  function of AssocValChooserG< Cont * >.
	*
	*  AssocValChooserG< Cont * > function object is generated. The functor test if element is a key in associative container
	*  and if the mapped value is greater then \a val. This is a specialized version of AssocValChooserG for pointers which 
	*  do not copy the associative container.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \param arg the pointer to associative container.
	*  \related AssocValChooserG
	*  \ingroup DMchooser*/
	template< class Cont > AssocValChooserG< Cont * >
		extAssocChooseG( const Cont *arg, typename Cont::ValType val ) { return AssocValChooserG< Cont * >( arg,val ); }

	/* AssocValChooserL
	 *
	 */
	/** \brief Choose elements for which mapped value less then common value.
	 *
	 *  The functor is equipped with pointer to associative container. Each call of function call operator tests if element mapped value is smaller then prespcified value. 
	 *  Furthermore, the element needs to be a key in the container. Both, the associative container and the value to compare are set up in constructor.
	 *  The chooser works for both edges and vertices. Mind that mapped values in container must allow operator<. 
	 *  This is specialization of AssocValChooserL for pointers and that associative container is not copied.
	 *  It is users prerogative to keep the container up to date.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocValChooserL< Cont * >
	{
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		typename Cont::ValType val;/**<\brief The desired value of mapped value.*/
		typedef typename Cont::ValType SelfValType;

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocValChooserL< Cont * > ChoosersSelfType;

		/** \brief Constructor
		*
		*  The constructor sets up the associative container and the value to compare.*/
		AssocValChooserL(const Cont *arg = 0, typename Cont::ValType aval = SelfValType()) :
			cont( arg ), val( aval ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and mapped value is lower then \a val.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value is smaller then \a val. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &) const
			{ return cont->hasKey( elem ) && cont->operator[]( elem ) < val; }
	};

		/** \brief Generating  function of AssocValChooserL< Cont * >.
		*
		*  AssocValChooserL< Cont * > function object is generated. The functor test if element is a key in associative container
		*  and if the mapped value is lower then \a val. The chooser is a specialized version of AssocValChooserL
		*  for pointers which do not copy the associative container.
		*  \wikipath{chooser, Get more information about choosers.}
		*  \tparam Cont the type of associative container.
		*  \param arg the pointer to associative container.
		*  \related AssocValChooserL
		*  \ingroup DMchooser*/
		template< class Cont > AssocValChooserL< Cont * >
		extAssocChooseL( const Cont *arg, typename Cont::ValType val ) { return AssocValChooserL< Cont * >( arg,val ); }

	/** \brief Choose if mapped value belongs to set.
	 *
	 *  Function object that checks if the element mapped value belongs to the prespecified set. 
	 *  Furthermore, the element needs to be a key in the associative container. 
	 *  Both, the associative container and the set are defined in the constructor.
	 *  This is specialization of AssocSetChooser for pointers and that associative container is not copied.
	 *  It is users prerogative to keep the container up to date. However, that the set is copied.
	 *  The chooser works with both edges and vertices.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \ingroup DMchooser */
	template< class Cont > struct AssocSetChooser< Cont * >
	{
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		Koala::Set< typename Cont::ValType > set;/**<\brief The set with approved mapped values.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocSetChooser< Cont * > ChoosersSelfType;

		/** \brief Constructor
		*
		*  The constructor sets up the associative container and the set of approved mapped values to compare.*/
		AssocSetChooser(const Cont *arg = 0, const Koala::Set< typename Cont::ValType > &aset =
			Koala::Set< typename Cont::ValType>() ): cont( arg ), set( aset ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and the mapped value belongs to set \a set.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value belongs to set \a set. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &) const
			{ return cont->hasKey( elem ) && set.isElement( cont->operator[]( elem ) ); }
	};

	/** \brief Generating  function of AssocSetChooser< Cont * >.
	*
	*  AssocSetChooser< Cont * > function object is generated. The functor test if element is a key in associative container
	*  and if the mapped value belongs to the set. The chooser is a specialized version of AssocSetChooser for which only 
	*  the set is copied and the associative container is delivered by pointer.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \param arg the pointer to associative container.
	*  \param set the set with approved mapped values.
	*  \related AssocSetChooser
	*  \ingroup DMchooser*/
	template< class Cont > AssocSetChooser< Cont * > extAssocChoose( const Cont *arg,
		const Koala::Set< typename Cont::ValType > &set ) { return AssocSetChooser< Cont * >( arg,set ); }

	/** \brief Choose if mapped value belongs to container.
	*
	*  Function object that checks if the element mapped value belongs to the prespecified another container
	*  (given with iterators \wikipath{iterator, Get more data about iterators).
	*  Furthermore, the element needs to be a key in the associative container.
	*  Both, the associative container and the container are defined in the constructor.
	*  The container with approved mapped values is not copied.
	*  Since, this is a specialization of AssocContainerChooser for pointers also that associative container is not copied.
	*  It is users prerogative to keep the containers up to date.
 	*  The container with approved values should be stl-like and must allow stl::find algorithm.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \tparma Iter the iterator type of container to search mapped values in for.
	*  \ingroup DMchooser */
	template< class Cont, class Iter > struct AssocContainerChooser< Cont *, Iter >
	{
		Iter begin/**<\brief the iterator to the first element of the container.*/, end/**<\brief the iterator to the past-the-end element of the container.*/;
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocContainerChooser< Cont *, Iter > ChoosersSelfType;

		/**\brief Constructor
		*
		*  In the constructor the associative container is copied and the container with approved mapped values is set up (via iterators.
		*  \param acont the associative container.
		*  \param abegin the iterator to the first element of container with approved mapped values.
		*  \param aend the iterator to the past-the-last element of container with approved mapped values.*/
		AssocContainerChooser(const Cont *acont = 0, Iter abegin = Iter(), Iter aend = Iter()) :
			cont( acont ), begin( abegin ), end( aend ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and the mapped value belongs container given by iterators.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the mapped value belongs to the container with approved mapped values. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &) const;
	};

	/** \brief Generating  function of AssocContainerChooser< Cont *,Iter >.
	 *
	 *  AssocContainerChooser< Cont *,Iter > function object is generated. The functor test if element is a key in associative container
	 *  and if the mapped value is an element in contaier given by iterators \a begin \a end. 
	 *  In this specialization of AssocContainerChooser the associative container \a cont is not copied. 
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \tparam Iter the iterator type.
	 *  \param cont the pointer associative container.
	 *  \param begin the iterator to the first element of container with approved mapped values.
	 *  \param end the iterator to the past-the-last element of container with approved mapped values.
	 *  \related AssocContainerChooser
	 *  \ingroup DMchooser*/
	template< class Cont, class Iter > AssocContainerChooser< Cont *,Iter >
		extAssocChoose(const Cont *cont, Iter begin, Iter end )
	{ return AssocContainerChooser< Cont *,Iter >( cont,begin,end ); }

	/* AssocObjChooser
	 *
	 */
	/** \brief Choose if functor returns true for mapped value.
	 *
	 *  The function object is equipped with functor and associative container both set up in constructor.
	 *  The element to be chosen must be a key in associative container and function should return value convertible to true for mapped  value.
	 *  The chooser works for both edges and vertices.
	 *  This is specialization of AssocObjChooser for pointers and that associative container is not copied.
	 *  It is users prerogative to keep the container up to date.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Cont the type of associative container.
	 *  \tparma Obj the functor class.
	 *  \ingroup DMchooser */
	template< class Cont, class Obj > struct AssocObjChooser< Cont *,Obj >
	{
		mutable Obj funktor;/**<\brief The function object.*/
		const Cont *cont;/**<\brief The pointer to the associative container.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AssocObjChooser< Cont *, Obj > ChoosersSelfType;

		/**\brief Constructor
		*
		* The associative container and testing function object are set up. */
		AssocObjChooser(const Cont *acont = 0, Obj arg = Obj()) : cont(acont), funktor(arg) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if \a elem is a key in associative array and the function object returns true for mapped value.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if \a elem is a key in the container and the functor returns true for mapped value. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &graph) const
			{ return cont->hasKey( elem ) && (bool)funktor( cont->operator[]( elem ) ); }
	};

	/** \brief Generating  function of AssocObjChooser< Cont *,Obj >.
	*
	*  AssocObjChooser< Cont *,Obj > function object is generated. The functor test if the element is a key in associative container
	*  and if functor \a arg returns valeu convertible to true for mapped value. The chooser is a specialized version of AssocObjChooser
	*  which do not copy the associative container.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Cont the type of associative container.
	*  \tparam Obj the function object type.
	*  \param cont the pointer to associative container.
	*  \param arg the function object
	*  \related AssocObjChooser
	*  \ingroup DMchooser*/
	template< class Cont, class Obj > AssocObjChooser< Cont *,Obj >
		extAssocFChoose( const Cont *cont, Obj arg ) { return AssocObjChooser< Cont *,Obj >( cont,arg ); }

	// Predykaty chooserow mozna laczyc operatorami logicznymi. Choosery operacji logicznych na prostszych chooserach

	/** \brief Or chooser.
	 *
	 *  The function object that joins two choosers. It returns true value if and only if the first one or the second one return true.
	 *  The chooser works for both edges and vertices. \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Ch1 the first chooser class.
	 *  \tparma Ch2 the second chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1, class Ch2 > struct OrChooser 
	{
		Ch1 ch1;/**<\brief The first chooser. */
		Ch2 ch2;/**<\brief The second chooser. */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef OrChooser< Ch1, Ch2 > ChoosersSelfType;
        /**\brief Constructor
		 *
		 * The constructor sets up the choosers. */
		OrChooser( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if chooser \a ch1 or \a ch2 returns true for given element \a elem.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if choosers \a ch1 or \a ch2 return true. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &graph) const
			{ return (ch1( elem,graph ) || ch2( elem,graph )); }
	};

	/** \brief Generating  function of OrChooser.
	*
	*  OrChooser function object is generated. The functor chooses elements that are chosen for at least one of choosers \a ch1 or \a ch2.  
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Ch1 the type of the first chooser.
	*  \tparam Ch2 the type of the second chooser.
	*  \param a the first chooser.
	*  \param b the second chooser
	*  \related OrChooser
	*  \ingroup DMchooser*/
	template< class  Ch1, class Ch2 > OrChooser< Ch1, Ch2 >
		orChoose( Ch1 a, Ch2 b ) { return OrChooser< Ch1,Ch2 >( a,b ); }

	// w kodzie funkcje tworzace zlozonych chooserow mozna zastapic odpowiednimi operatorami logicznymi

	/** \brief The overloaded operator||. Chooser alternative.
	 * 
	 *  The operator calls the generating  function of OrChooser i.e. generate chooser that joins two choosers with logic or operator.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \related OrChooser
	 *  \ingroup DMchooser*/
	template <class  Ch1, class Ch2> OrChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator||( Ch1 a, Ch2 b ) { return OrChooser< Ch1,Ch2 >( a,b ); }

	/** \brief And chooser.
	 *
	 *  The function object that joins two choosers. It returns true value if and only if both choosers return true when called with the element.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Ch1 the first chooser class.
	 *  \tparma Ch2 the second chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1, class Ch2 > struct AndChooser
	{
		Ch1 ch1;/**<\brief The first chooser. */
		Ch2 ch2;/**<\brief The second chooser. */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef AndChooser< Ch1, Ch2 > ChoosersSelfType;

		/**\brief Constructor
		*
		* The constructor sets up the choosers. */
		AndChooser(Ch1 a = Ch1(), Ch2 b = Ch2()) : ch1(a), ch2(b) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if both choosers \a ch1 and \a ch2 return true for given element \a elem.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if choosers \a ch1 and \a ch2 return true. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &graph) const
			{ return (ch1( elem,graph ) && ch2( elem,graph )); }
	};

	/** \brief Generating  function of AndChooser.
	*
	*  AndChooser function object is generated. The functor chooses elements that are chosen for both choosers \a ch1 and \a ch2.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Ch1 the type of the first chooser.
	*  \tparam Ch2 the type of the second chooser.
	*  \param a the first chooser.
	*  \param b the second chooser
	*  \related AndChooser
	*  \ingroup DMchooser*/
	template< class  Ch1, class Ch2 > AndChooser< Ch1, Ch2 >
		andChoose( Ch1 a, Ch2 b ) { return AndChooser< Ch1,Ch2 >( a,b ); }

	/** \brief The overloaded operator&&. Chooser conjunction.
	 *
	 *  The operator calls the generating  function of AndChooser i.e. it generates chooser that joins two choosers with logic and operator.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \related AndChooser
	 *  \ingroup DMchooser*/
	template< class Ch1, class Ch2 > AndChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator&&( Ch1 a, Ch2 b ) { return AndChooser< Ch1,Ch2 >( a,b ); }

	/** \brief Xor chooser.
	 *
	 *  The function object that joins two choosers. It returns true value if and only if either the first one or the second one return true
	 *  (both may not be true).
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Ch1 the first chooser class.
	 *  \tparma Ch2 the second chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1, class Ch2 > struct XorChooser
	{
		Ch1 ch1;/**<\brief The first chooser. */
		Ch2 ch2;/**<\brief The second chooser. */

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef XorChooser< Ch1, Ch2 > ChoosersSelfType;

		/**\brief Constructor
		*
		* The constructor sets up the choosers. */
		XorChooser(Ch1 a = Ch1(), Ch2 b = Ch2()) : ch1(a), ch2(b) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only if exactly one chooser either \a ch1 or \a ch2 return true for given element \a elem.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return true if and only if only one of choosers \a ch1 and \a ch2 return true. */
		template< class Elem, class Graph > bool operator()(Elem *elem, const Graph &graph) const
			{ return (ch1( elem,graph ) != ch2( elem,graph )); }
	};

	/** \brief Generating  function of XorChooser.
	*
	*  XorChooser function object is generated. The functor chooses elements for which exactly one of choosers \a ch1 and \a ch2 returns true.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Ch1 the type of the first chooser.
	*  \tparam Ch2 the type of the second chooser.
	*  \param a the first chooser.
	*  \param b the second chooser
	*  \related XorChooser
	*  \ingroup DMchooser*/
	template< class Ch1, class Ch2 > XorChooser< Ch1, Ch2 >
		xorChoose( Ch1 a, Ch2 b ) { return XorChooser< Ch1,Ch2 >( a,b ); }

	/** \brief The overloaded operator^. Chooser exclusive or.
	 *
	 *  The operator calls the generating  function of XorChooser which generates chooser that joins two choosers with logic exclusive or operation.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Ch1 the type of the first chooser.
	 *  \tparam Ch2 the type of the second chooser.
	 *  \param a the first chooser.
	 *  \param b the second chooser
	 *  \related XorChooser
	 *  \ingroup DMchooser*/
	template< class Ch1, class Ch2 > XorChooser< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator^( Ch1 a, Ch2 b ) { return XorChooser< Ch1,Ch2 >( a,b ); }

	/** \brief Not chooser.
	 *
	 *  The function object that gives the opposite result to the given chooser.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Ch1 the chooser class.
	 *  \ingroup DMchooser */
	template< class Ch1 > struct NotChooser
	{
		Ch1 ch1;/**<\brief The negated chooser type.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef NotChooser< Ch1 > ChoosersSelfType;

		/**\brief Constructor
		 *
		 * The constructor sets up the negated chooser.*/
		NotChooser( Ch1 a = Ch1() ): ch1( a ) { }

		/** \brief Overloaded operator()
		*
		*  Function call operator returning true if and only chooser \a ch1 returns false.
		*  \param elem the checked object.
		*  \param graph the considered graph.
		*  \return negation of chooser \a ch1. */
		template< class Elem, class Graph >  bool operator()(Elem *elem, const Graph &graph) const
			{ return !ch1( elem,graph ); }
	};

	/** \brief Generating  function of NotChooser.
	*
	*  NotChooser function object is generated. The functor chooses elements that are not chosen by \a ch1.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \tparam Ch1 the chooser type.
	*  \param a the chooser.
	*  \related NotChooser
	*  \ingroup DMchooser*/
	template< class  Ch1 >
		NotChooser< Ch1 > notChoose( Ch1 a ) { return NotChooser< Ch1 >( a ); }

	/** \brief The overloaded operator!. Chooser negation.
	 *
	 *  The operator calls the generating  function of NotChooser i.e. generate chooser that negates the given chooser.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Ch1 the chooser type.
	 *  \param a the chooser.
	 *  \related NotChooser
	 *  \ingroup DMchooser*/
	template< class  Ch1 > NotChooser< typename Ch1::ChoosersSelfType >
		operator!( Ch1 a ) { return NotChooser< Ch1 >( a ); }

	// choosery dow wybierania wierzcholkow

	/* VertDegValChooser
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma podana
	 * wartosc
	 */
	/** \brief Choose vertices of given degree.
	 *
	 *  The function object that checks if the vertex degree equals given common value.
	 *  The degree is calculated with respect to Koala::EdgeDirection (\wikipath{EdgeDirection, Read more about EdgeDirection} mask like in method ConstGraphMethods::deg. 
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	struct VertDegValChooser 
	{
		int deg; /**< \brief the desired degree.*/
		Koala::EdgeDirection type; /**< \brief the considered edge direction.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
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
	 *
	 *  \param adeg the defined degree.
	 *  \param atype type of direction used for degree calculation.
	 *  \return chooser of type VertDegValChooser, which chooses vertices of degree \a adag exclusively.  
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \related VertDegValChooser
	 *  \ingroup DMchooser*/
	inline VertDegValChooser vertDegChoose( int adeg, Koala::EdgeDirection atype = Koala::EdAll )
		{ return VertDegValChooser( adeg,atype ); }

	/* VertDegValChooserL
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
	 * mniejsza od zadanej
	 */
	/** \brief Choose vertices of degree less then.
	 *
	 *  The function object that checks if the vertex degree is less then the prespecified value.
	 *  The degree is calculated with respect to Koala::EdgeDirection (\wikipath{EdgeDirection, Read more about EdgeDirection} mask like in method ConstGraphMethods::deg. 
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	struct VertDegValChooserL
	{
		int deg; /**< \brief the strict upper bond for degree*/
		Koala::EdgeDirection type;/**< \brief the considered edge direction.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef VertDegValChooserL ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The strict upper bond for degree and the edge direction to consider are defined here.
		 *  \param adeg the expected degree.
		 *  \param atype the mask of considered edge directions.*/
		VertDegValChooserL( int adeg = 0, Koala::EdgeDirection atype = Koala::EdAll ): deg( adeg ), type( atype ) { }

		/** \brief Overloaded operator()
		 *
		 *  Function call operator returning boolean value true if vertex \a v degree is smaller than \a deg.
		 *  \param v the tested vertex.
		 *  \param g reference to considered graph.
		 *  \return the true if degree of the vertex is smaller than \a deg, false otherwise. */
		template< class Graph > bool operator()( typename Graph::PVertex v, const Graph &g ) const
			{ return g.deg( v,type ) < deg; }
	};

	/** \brief Generating  function of VertDegValChooserL.
	*
	*  \param adeg the defined degree.
	*  \param atype type of direction used for degree calculation.
	*  \return chooser of type VertDegValChooserL, which chooses vertices of degree less than \a adag.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \related VertDegValChooserL
	*  \ingroup DMchooser*/
	inline VertDegValChooserL vertDegChooseL(int adeg, Koala::EdgeDirection atype = Koala::EdAll)
		{ return VertDegValChooserL( adeg,atype ); }

	/* VertDegValChooserG
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
	 * wieksza od zadanej
	 */
	/** \brief Choose vertices of degree greater then.
	 *
	 *  The function object that checks if the vertex degree is greater then the prespecified value.
	 *  The degree is calculated with respect to Koala::EdgeDirection (\wikipath{EdgeDirection, Read more about EdgeDirection} mask like in method ConstGraphMethods::deg.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	struct VertDegValChooserG
	{
		int deg;/**< \brief the strict lower bond degree*/
		Koala::EdgeDirection type;/**< \brief the considered edge direction.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
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

	/** \brief Generating  function of VertDegValChooserG.
	*
	*  \param adeg the defined degree.
	*  \param atype type of direction used for degree calculation.
	*  \return chooser of type VertDegValChooserL, which chooses vertices of degree greater than \a adag.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \related VertDegValChooserG
	*  \ingroup DMchooser*/
	inline VertDegValChooserG vertDegChooseG(int adeg, Koala::EdgeDirection atype = Koala::EdAll)
		{ return VertDegValChooserG( adeg,atype ); }

    //WEN: jw.
	/* VertDegSetChooser
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc z
	 * podanego zbioru
	 */
	/** \brief Vertex degree from set chooser.
	 *
	 *  The function object that checks if the vertex degree concerning the edge direction belongs the set prespecified in constructor. (also the direction to considered is defined there)
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	template< class Int > struct VertDegSetChooser
	{
		Koala::Set< Int > set;/**< \brief the set with acceptable degrees.*/
		Koala::EdgeDirection type;/**< \brief the considered edge direction.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
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
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser*/
	template< class Int > VertDegSetChooser< Int > vertDegChoose( Koala::Set< Int > aset,
		Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegSetChooser< Int >( aset,atype ); }

    // WEN: jw oraz
    //WEN: chodzi o STL-like containers tj. obslugujace algorytm std::find
    //WEN: jest odpowiedzialnoscia uzytkownika, by przedzial iteratorow byl wazny podczas uzywania choosera (kontener nie jest kopiowany)
	/* VertDegContainerChooser
	 * testuje, czy stopien wierzcholka (wyliczany z uwzglednieniem maski kierunku krawedzi sasiednich) ma wartosc
	 * z zakresu miedzy podanymi iteratorami - uzywa STLowego find
	 */
	/** \brief Vertex degree from container chooser.
	 *
	 *  The function object that checks if the vertex degree concerning the edge direction is an element of the container prespecified in constructor. (also the direction to considered is defined there)
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	template< class Iter > struct VertDegContainerChooser
	{
		Iter begin/**\brief iterator to the first element of container*/, end/**\brief iterator to the past-the-end element of container*/;
		Koala::EdgeDirection typ;/**< \brief the considered edge direction.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
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
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser*/
	template< class Iter > VertDegContainerChooser< Iter > vertDegChoose( Iter begin, Iter end,
		Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegContainerChooser< Iter >( begin,end,atype ); }

    //WEN: jw. oraz powinno sie podac sygnature tj. jak dziala funktor
	/* VertDegFunctorChooser
	 * decyzja podejmowana na podstawie wartosci obiektu funktora policzonego na stopniu wierzcholka
	 */
	/** \brief Vertex degree functor chooser.
	 *
	 *  The function object that for a given vertex tests if the vertex degree satisfy the functor defined in the constructor.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	template< class Obj > struct VertDegFunctorChooser
	{
		mutable Obj funktor;/** \brief the object function qualifying degrees.*/
		Koala::EdgeDirection typ;/**< \brief the considered edge direction.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
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
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser*/
	template< class Obj > VertDegFunctorChooser< Obj > vertDegFChoose( Obj afun,
		Koala::EdgeDirection atype = Koala::EdAll ) { return VertDegFunctorChooser< Obj >( afun,atype ); }

	// choosery do wybierania krawedzi

	/* EdgeTypeChooser
	 * testuje, czy typ krawedzi spelnia podana maske
	 */
	/** \brief Edge direction chooser. WEN: chyba rodzaj nie direction, dot. calej dokumentacji choosera
	 *
	 *  The function object chooses the edges with WEN: chyba rodzaj nie direction direction congruent with the mask prespecified in constructor.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	struct EdgeTypeChooser //WEN: dla krawedzi
	{
		Koala::EdgeDirection mask;/**< \brief the considered edge direction.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef EdgeTypeChooser ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The direction of edge to choose is defined here.
		 *  \param amask the mask of considered edge directions. WEN: ustawienie ktoregos z bitow Directed dziala jak ustawienie calego Directed */
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
	WEN: opis param.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser*/
	inline EdgeTypeChooser edgeTypeChoose( Koala::EdgeDirection mask ) { return EdgeTypeChooser( mask ); }

	// choosery zlozone dla krawedzi, sprawdzajace warunkek definiowany dla wierzcholka sa prawda dla jej koncow

	/* EdgeFirstEndChooser
	 * test pierwszego konca krawedzi
	 */
	/** \brief Edge first end satisfy functor chooser.
	 *
	 *  The function object chooses the edges with first end satisfy a functor (ex. some vertex chooser)  defined in constructor.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	template< class Ch > struct EdgeFirstEndChooser //WEN: dla krawedzi
	{
		Ch ch;/**< \brief the function object that checks the first end of edge.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef EdgeFirstEndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here. WEN: po prostu chooser dla wierzcholka
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
	//WEN: opis param.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser*/
	template< class Ch > EdgeFirstEndChooser< Ch >
		edgeFirstEndChoose( Ch ch ) { return EdgeFirstEndChooser< Ch >( ch ); }

	/* EdgeSecondEndChooser
	 * test drugiego konca krawedzi
	 */
	/** \brief Edge second end satisfy functor chooser.
	 *
	 *  The function object chooses the edges with second end satisfy a functor (ex. some vertex chooser)  defined in constructor.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	template <class Ch> struct EdgeSecondEndChooser //WEN: dla krawedzi
	{
		Ch ch;/**< \brief the function object that checks the second end of edge.*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef EdgeSecondEndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here. WEN: po prostu chooser dla wierzcholka
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
	WEN: opis param.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser*/
	template< class Ch > EdgeSecondEndChooser< Ch >
		edgeSecondEndChoose( Ch ch ) { return EdgeSecondEndChooser< Ch >( ch ); }

	/* Edge0EndChooser
	 * test, czy zaden koniec nie spelnia warunku
	 */
	/** \brief Choose if none of edge ends satisfy functor.
	 *
	 *  The function object chooses the edges in with none of its ends satisfies a functor (ex. some vertex chooser)  defined in constructor.
	 *  The chooser works only with edges.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \tparam Ch The type of chooser for vertices.
	 *  \ingroup DMchooser */
	template< class Ch > struct Edge0EndChooser // dla krawedzi
	{
		Ch ch;/**< \brief the function object that tests vertices (defined in constructor).*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef Edge0EndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  Vertex chooser \a ch is defined here.
		 *  \param funktor the chooser assigned to attribute \a ch.*/
		Edge0EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return true if and only if none of edge \a e ends satisfies the functor \a ch. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return !ch( g.getEdgeEnd1( e ),g ) && !ch( g.getEdgeEnd2( e ),g ); }
	};

	/** \brief Generating  function of Edge0EndChooser.
	 *
	 *  The function generates function object that chooses only those edges for which none of ends satisfy functor.
	 *  \tparam Ch the type of vertec chooser.
	 *  \param vertex chooser.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \related Edge0EndChooser
	 *  \ingroup DMchooser*/
	template< class Ch > Edge0EndChooser< Ch > edge0EndChoose(Ch ch) { return Edge0EndChooser< Ch >(ch); }

	/* Edge1EndChooser
	 * test, czy jeden koniec spelnia warunkek
	 */
	/** \brief Choose if one edge end satisfy functor.
	 *
	 *  The function object chooses the edges in with one of its ends satisfies a functor (ex. some vertex chooser)  defined in constructor.
	 *  The chooser works for edges only.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	template< class Ch > struct Edge1EndChooser
	{
		Ch ch;/**< \brief the function object that tests vertices (defined in constructor).*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef Edge1EndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here.
		 *  \param funktor the vertex chooser assigned to attribute \a ch.*/
		Edge1EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return true if and only if one of edge \a e ends satisfies the functor \a ch. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return ch( g.getEdgeEnd1( e ),g ) != ch( g.getEdgeEnd2( e ),g ); }
	};

	/** \brief Generating  function of Edge1EndChooser.
	*
	*  The function generates Edge1EndChooser function object that chooses only those edges for which exactly one of ends satisfy functor.
	*  \tparam Ch the type of vertec chooser.
	*  \param vertex chooser.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \related Edge1EndChooser
	*  \ingroup DMchooser*/
	template< class Ch > Edge1EndChooser< Ch > edge1EndChoose(Ch ch) { return Edge1EndChooser< Ch >(ch); }
    
	/* Edge2EndChooser
	 * test, czy oba konice spelniaja warunkek
	 */
	/** \brief Choose if both edge ends satisfy functor.
	 *
	 *  The function object chooses the edges in with both ends satisfy a functor (ex. some vertex chooser)  defined in constructor.
	 *  The chooser works for edges only.
	 *  \wikipath{chooser, Get more information about choosers.}
	 *  \ingroup DMchooser */
	template< class Ch > struct Edge2EndChooser
	{
		Ch ch;/**< \brief the function object that tests vertices (defined in constructor).*/

		/** \brief Chooser obligatory type.
		*
		*  The type is obligatory for choosers in Koala. Logic operations (&&, ||, !, ^)  work properly as long as it is defined. */
		typedef Edge2EndChooser< Ch > ChoosersSelfType;

		/** \brief Constructor
		 *
		 *  The \a ch functor is defined here.
		 *  \param funktor the vertex chooser assigned to attribute \a ch.*/
		Edge2EndChooser( Ch funktor = Ch() ): ch( funktor ) { }

		/** \brief Overloaded operator()
		 *
		 *  \param e the tested edge.
		 *  \param g reference to considered graph.
		 *  \return true if and only if both edge \a e ends satisfy the functor \a ch. */
		template< class Graph > bool operator()( typename Graph::PEdge e, const Graph &g ) const
			{ return ch( g.getEdgeEnd1( e ),g ) && ch( g.getEdgeEnd2( e ),g ); }
	};

	/** \brief Generating  function of Edge2EndChooser.
	*
	*  The function generates Edge2EndChooser function object that chooses only those edges for which both ends satisfy functor.
	*  \tparam Ch the type of vertec chooser.
	*  \param vertex chooser.
	*  \wikipath{chooser, Get more information about choosers.}
	*  \related Edge2EndChooser
	*  \ingroup DMchooser*/
	template< class Ch > Edge2EndChooser< Ch > edge2EndChoose(Ch ch) { return Edge2EndChooser< Ch >(ch); }

	//Castery to funktory ustalajace wartosci pol info w nowych wierz/kraw tworzonych podczas np. kopiowania grafow.
	// Wartosci te powstaja (w rozny sposob) na podstawie inf oryginalnych

    namespace Privates {

        namespace KonwTestSpace {


            template <class Dest,class Source> struct Przec {

                struct Lapacz {
                    int a;
    //                template <class T>
    //                Lapacz(T) : a(1) {}
                        Lapacz(Source) : a(1) {}
                };


                static char przec(Dest,int)
                {
                    return 'A';
                }

                static double przec(Lapacz,...)
                {
                    return 12.3;
                }

            };

            template <class Dest,class Sour> struct Przec<Dest*,Sour*> {

                template <class T>
                static char przec(T,int)
                {
                    return 'A';
                }

            };


            template <int arg> struct Cast {

                template <class Sour,class Dest>
                static void make(Dest& d,const Sour& s)
                {
                    d=Dest();
                }
            };

            template <> struct Cast<1> {

                template <class Sour,class Dest>
                static void make(Dest& d,const Sour& s)
                {
                    d=(Dest)s;
                }
            };

        }
    }



	/* StdCaster
	 * caster zwyklego rzutowania miedzy dwoma strukturami
	 */
	/** \brief Standard caster.
	 *  WEN: wykorzystywane w metodach copy, substitute z grafu oraz linegrafach i produktach (create.h) do tworzenia inf nowych elementow na podstawie oryginalow - to dot. wszystkich casterow
	 *  The structure overload call function operator for two parameters. The value of source is simply casted on the destination. The inbuilt type conversion is used.
	 *  \ingroup DMcaster*/
	struct StdCaster // NEW: tu opis nie bedzie sie zgadzal, tera to robi HardCaster
	{
		typedef StdCaster CastersSelfType;

		/** \brief Call function operator.
		 *
		 *  The overloaded call function operator with two parameters, that uses the inbuilt type conversion of \a sour to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour the source object. */
		template< class InfoDest, class InfoSour >
			void operator()( InfoDest &dest, InfoSour sour )
			{
			    //dest = (InfoDest)sour;
                Koala::Privates::KonwTestSpace::Cast<
                        sizeof(Koala::Privates::KonwTestSpace::Przec<InfoDest,InfoSour>::przec(sour,12))
                    >::make(dest,sour);
			}
	};

	// i jego funkcja tworzaca
	/** \brief Generating function for standard caster (StdCaster).
	 *  \ingroup DMcaster*/
	inline StdCaster stdCast() { return StdCaster(); }

    //NEW: stdCaster probuje przekonwertowac InfoSour->InfoDest, a jesli sie nie uda, inicjuje InfoDest wart. domyslna
    //HardCaster bezposrednio rzutuje (InfoDest)InfoSour, co jesli jest nielegalne - wywola blad kompilacji
	struct HardCaster
	{
		typedef HardCaster CastersSelfType;

		/** \brief Call function operator.
		 * WEN: wykorzystywane w metodach copy, substitute z grafu oraz linegrafach i produktach (create.h) do tworzenia inf nowych elementow na podstawie oryginalow - to dot. wszystkich casterow
		 *  The overloaded call function operator with two parameters, that uses the inbuilt type conversion of \a sour to \a dest.
		 *  \param dest the reference to the destination object.
		 *  \param sour the source object. */
		template< class InfoDest, class InfoSour >
			void operator()( InfoDest &dest, InfoSour sour )
			{
			    dest = (InfoDest)sour;
			}
	};

	// i jego funkcja tworzaca
	/** \brief Generating function for standard caster (StdCaster).
	 *  \ingroup DMcaster*/
	inline HardCaster hardCast() { return HardCaster(); }


	/* NoCastCaster
	 * caster ustawiajacy wartosc domyslna i ignorujacy oryginalny parametr wspolpracuje z produktami grafow (stad
	 * takze operator 3-argumentowy)
	 */
	/** \brief No cast caster.
	 * WEN: wykorzystywane w metodach copy, substitute z grafu oraz linegrafach i produktach (create.h) do tworzenia inf nowych elementow na podstawie oryginalow - to dot. wszystkich casterow
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
	inline NoCastCaster stdCast( bool arg );

	/* ObjCaster
	 * wyliczenie wartosci nowego info poprzez podany funktor wspolpracuje z produktami grafow (stad takze operator
	 * 3-argumentowy) jesli funktor je obsluguje
	 */

	/** \brief Functor caster.
	 * WEN: wykorzystywane w metodach copy, substitute z grafu oraz linegrafach i produktach (create.h) do tworzenia inf nowych elementow na podstawie oryginalow - to dot. wszystkich casterow
	 *  The caster calls the functor defined in constructor to produce the info object. WEN: ale jaki funktor? 1-argumentowy, bierzemy jego wynik ...
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
	 * WEN: wykorzystywane w metodach copy, substitute z grafu oraz linegrafach i produktach (create.h) do tworzenia inf nowych elementow na podstawie oryginalow - to dot. wszystkich casterow
	 * The caster assigns the same value to info WEN: ignorujac argument. The value is set up in constructor.
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
	 * WEN: rowniez jakis opis do czego sa uzywane - wszystkie linkery
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
	 * WEN: uzywa mapy zewnetrznej, a wiec user jest odpowiedzialny za stan obiektu mapy podczas dzialania linkera
	 *  This linker uses the associative array which is a map that associate pointer of destination element with the pointer of source. WEN: tj. te asocjacje dopiero zostana wpisane
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
	inline Std1NoLinker stdLink( bool a1 ) { return Std1NoLinker( a1 ); }

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
	inline Std2Linker< Std1NoLinker,Std1NoLinker > stdLink( bool a1, bool a2 );

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
	 * Overloaded operator& allows to create easily a std::pair of choosers \a a and \a b.*/
	template <class  Ch1, class Ch2> std::pair< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >
		operator&( Ch1 a, Ch2 b )
		{
			return std::pair< typename Ch1::ChoosersSelfType,typename Ch2::ChoosersSelfType >(a,b);
		}


	/**\brief Make pair of casters.
	 *
	 * Overloaded operator& allows to create easily a std::pair of casters \a a and \a b.*/
		template <class  Ch1, class Ch2> std::pair< typename Ch1::CastersSelfType,typename Ch2::CastersSelfType >
		operator&( Ch1 a, Ch2 b )
		{
			return std::pair< typename Ch1::CastersSelfType,typename Ch2::CastersSelfType >(a,b);
		}

	/**\brief Make pair of linkers.
	 *
	 * Overloaded operator& allows to create easily a pair of std::linkers \a a and \a b.*/
	template <class  Ch1, class Ch2> std::pair< typename Ch1::LinkersSelfType,typename Ch2::LinkersSelfType >
		operator&( Ch1 a, Ch2 b )
		{
			return std::pair< typename Ch1::LinkersSelfType,typename Ch2::LinkersSelfType >(a,b);
		}


#include "defs.hpp"
}

#endif