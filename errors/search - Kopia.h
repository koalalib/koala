#ifndef KOALA_DEF_SEARCH_H
#define KOALA_DEF_SEARCH_H

/* search.h
 *
 */

#include "../base/defs.h"
#include "../graph/view.h"

namespace Koala
{

	/* SearchStructs
	 *
	 */
	struct PathStructs
	{
		// Do odczytu sciezki miedzy wierzcholkiem a korzeniem, gdy droga wyznaczona jest w postaci
		// tablicy asocjacyjnej PVertex -> rekord z polami vPrev, ePrev (wierzcholek poprzedni i krawedz do niego).
		// Przydatne w roznych algorytmach najkrotszej sciezki
		// Uzytkownik podaje, pare iteratorow, gdzie wpisac wierzcholki i krawedzie najkrotszej sciezki
		/** \brief Path structure.
		 *
		 *  The structure used by various algorithms. It is designe to keep a path i.e. a sequence of vertices and edges.
		 *  \n
		 *
		 *  [See example](examples/search/euler/euler.html).
		 */
		template< class VIter, class EIter > struct OutPath
		{
			VIter vertIter;/**<\brief the iterator to the container with vertices. */
			EIter edgeIter;/**<\brief the iterator to the container with edges. */
			/** \brief Constructor.*/
			OutPath( VIter av, EIter ei ): vertIter( av ), edgeIter( ei ) { }
		};

		// funkcja tworzaca, analogia make_pair
		// Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
		/**\brief The generating function for OutPath, takes two iterators and returnes OutPath*/
		template< class VIter, class EIter > static OutPath< VIter,EIter > outPath( VIter av, EIter ei )
			{ return OutPath< VIter,EIter >( av,ei ); }

		// OutPath moze wspolpracowac z dowolnymi sekwencyjnymi kontenerami, ale ponizsza struktura
		//  ulatwia obrobke takich danych
		/** \brief  Path tool.
		 *
		 *  The more complexet (then OutPath) class for representation of path.
		 *  It is easier to create object of this class and pass it to funcion via parameter using input() method.
		 *
		 */
		template< class Graph > class OutPathTool
		{
		private:
			std::vector< typename Graph::PVertex > verts;/**< \brief Vector of vertices.*/
			std::vector< typename Graph::PEdge > edges;/**< \brief Vector of edges.*/

		public:
			typedef typename Graph::PVertex PVertex;
			typedef typename Graph::PEdge PEdge;
			typedef std::back_insert_iterator< std::vector< typename Graph::PEdge > > EdgeIterType;
			typedef std::back_insert_iterator< std::vector< typename Graph::PVertex > > VertIterType;

			/** \brief Empty constructor*/
			OutPathTool()
				{ clear(); }
			/** \brief Clear path.*/
			void clear();
			// dlugosc wpisanej sciezki, -1 w razie bledu (jeszcze zadnej nie wpisano)
			/** \brief Length of path, the number of vertices - 1*/
			int length() const
				{ return verts.size() - 1; }
			// i-ta krawedz
			/** \brief Get i-th edge.*/
			PEdge edge( int i ) const;
			// i-ta wierzcholek
			/** \brief Get i-th vertex.*/
			PVertex vertex( int i) const;
			//Umiesczamy wywolanie funkcji w miejsu outPath a pozniej przetwarzamy zebrane ciagi
			// czysci kontener
			/** \brief Create input.
			 *
			 *  The method creates input for a function that requires OutPath. */
			OutPath< std::back_insert_iterator< std::vector< typename Graph::PVertex > >,
				std::back_insert_iterator< std::vector< typename Graph::PEdge > > > input();
		};

	};

	/* ShortPathStructs
	 *
	 */
	struct ShortPathStructs: public PathStructs
	{
		// zwraca liczbe krawedzi sciezki
		// vertTab, tablica asoc. przypisujaca wierzcholkom rekordy z ustawionymi
		// wskaznikami do poprzedniego wierzcholka (vPrev) i krawedzi don prowadzacej (ePrev, NULL dla korzenia)
		// W tej formie zwraca wynik wiele procedur tworzacych w grafie drzewo (las) skierowany do korzenia np. Dijkstra, BFS itp
		// OutPath, miejsce gdzie sciezka zostanie zapisana
		// sciezka prowadzi "pod prad" tj. od korzenia do tego wierzcholka
		// ew. wczesniejszy punkt koncowy na sciezce miedzy end a korzeniem
		template< class GraphType, class VertContainer, class VIter, class EIter >
			static int getOutPath( const GraphType &g, const VertContainer &vertTab, OutPath< VIter,EIter > iters,
			typename GraphType::PVertex end, typename GraphType::PVertex start = 0 );

		// Zapisuje na podany iterator wszystkie krawedzie nalezace do drzewa (lasu) tj. uzyte jako wartosci pol ePrev
		// Zwraca ich liczbe
		template< class GraphType, class VertContainer, class Iter > static int getUsedEdges( const GraphType &g,
			const VertContainer &vertTab, Iter iter );
		// jw. ale zwraca zbior krawedzi
		template< class GraphType, class VertContainer > static Set< typename GraphType::PEdge >
			getUsedEdgeSet( const GraphType &g, const VertContainer &vertTab );
	};


	/* SearchStructs
	 *
	 */
	class SearchStructs
	{
	public:
		// Struktura wartosci przypisywanej wierzcholkowi w procedurach przeszukiwania grafu
		/** \brief Auxiliary visit vertex structure
		 *
		 *  Structure used by search procedures. */
		template< class GraphType > struct VisitVertLabs
		{
			// rodzic danego wierzcholka w drzewie (lesie), NULL dla korzenia
			/** \brief Parent of vertex.*/
			typename GraphType::PVertex vPrev;
			// krawedz prowadzaca do rodzica
			/** \brief Edge leading to parent of vertex.*/
			typename GraphType::PEdge ePrev;

			// odleglosc od korzenia (liczba krawedzi) i numer skladowej spojnosci (od 0)
			int distance;/**< \brief Distance (number of edges) from root.*/
			int component;/**< \brief Index of connected component.*/

			template <class T> void copy(T& arg) const
			{
				arg.vPrev=vPrev;
				arg.ePrev=ePrev;
				arg.distance=distance;
				arg.component=component;
			}
			/** \brief Copy.*/
			void copy(BlackHole&) const
				{ }


			/**\brief Constructor*/
			VisitVertLabs( typename GraphType::PVertex vp = 0, typename GraphType::PEdge ep = 0,
				int dist = std::numeric_limits< int >::max(), int comp = -1 ):
					vPrev( vp ), ePrev( ep ), distance( dist ), component( comp )
				{ }
		};

		// Para iteratorow wyjsciowych do kodowania ciagu ciagow (zazwyczaj wierzcholki/kraw)
		// kolejne elementy kolejnych ciagow wysylane sa na vertIter
		// na compIter wyrzucane sa inty: pozycje (numeracja od 0) poczatkow kolejnych ciagow
		// oraz na koniec sumaryczna dlugosc wszystkich ciagow
		// (a wiec lacznie o 1 wiecej liczb, niz ilosc ciagow)
		/** \brief Joined sequences container.
		 *
		 *  The structure consist of two iterators. \a vertIter point to the concatenated sequenced of objects. \a compIter point to the container with integers such that each integer is a position of starting point of next sequence, the last integer represents the number of all elements in the \a vertIter.	 */
		template< class CIter, class VIter > struct CompStore
		{
			CIter compIter;/**<\brief the iterator to the container with concatenated sequences.*/
			VIter vertIter;/**< \brief the iterator to the container with position of starting points of consecutive sequences.*/

			/**\brief Constructor*/
			CompStore( CIter ac, VIter av ): compIter( ac ), vertIter( av )
				{ }
		} ;

		// Funkcja tworzaca, analogia make_pair
		// Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
		/** \brief Generating function for CompStore.
		 *
		 *  \param ac the iterator to the container with concatenated sequences of entities.
		 *  \param av the iterator to the container with integers representing the positions of first elements of consecutive sequences.
		 *  \return the CompStore object associated with the sequence of sequences. */
		template< class CIter, class VIter > static CompStore< CIter,VIter > compStore( CIter ac, VIter av )
			{ return CompStore< CIter,VIter >( ac,av ); }

		// Odwrotne mapowanie ciagu dlugosci size ciagow o elementach typu T o poczatkach w iteratorach
		// begin (kolejne rozmiary) i sbegin (wartosci) - por. powyzszy format
		// out, wyjsciowy ciag numerow (od 0) ciagow: i-ty ciag to numery
		// w ktorych wystepuje i-ty element poslany na eout (w razie powtorzen ten sam numer ciagu powtarza sie
		// odpowiednia ilosc razy.
		// eout, iterator wyjsciowy na wszystkie elementy w wystepujace ciagach (kazdy jeden raz)
		// zwraca ilosc elementow wystepujacych w ciagach (kazdy jeden raz) tj. dlugosc sekwencji poslanej na eout
		// uwaga: przy wywolaniach nie dedukuje typu elementow ciagow T (tzreba podac jawnie)
		template< class T, class InputIter, class VertInputIter, class CIter, class IntIter, class ElemIter >
			static int revCompStore( InputIter begin, VertInputIter sbegin, int size, CompStore< CIter,IntIter > out,
				ElemIter eout );
		// j.w., ale zakladamy, ze elementy ciagow (a wiec sbegin) sa przechowywane w tablicy, ktorej wskaznik
		// poczatku dostarczamy do procedury (wowczas typ T jest dedukowany).
		template< class T, class InputIter, class CIter, class IntIter, class ElemIter >
			static int revCompStore( InputIter begin, const T *sbegin, int size, CompStore< CIter,IntIter > out,
				ElemIter eout )
			{
				return revCompStore< T,InputIter,const T *,CIter,IntIter,ElemIter >( begin,sbegin,size,out,eout );
			}

		// CompStore moze wspolpracowac z dowolnymi sekwencyjnymi kontenerami, ale ponizsza struktura
		// ulatwia obrobke takich danych
		// T typ elementu ciagow skladowych
		template< class T > class CompStoreTool
		{
		private:
			std::vector< int > idx;
			std::vector< T > data;

		public:
			typedef T ValueType;
			typedef std::back_insert_iterator< std::vector< int > > CIterType;
			typedef std::back_insert_iterator< std::vector< T > > VIterType;

			CompStoreTool()
				{ clear(); }
			void clear();
			 // liczba zebranych ciagow
			int size() const;
			 // dlugosc i-tego ciagu
			int size( int i ) const;
			 // laczna dlugosc wszystkich ciagow
			int length() const;
			// wskaznik poczatku i-tego ciagu lub 0 gdy ten ciag jest pusty
			T *operator[]( int i );
			// wskaznik poczatku i-tego ciagu lub 0 gdy ten ciag jest pusty
			const T *operator[]( int i ) const;
			// umieszcza nowy ciag pusty na pozycji i.
			void insert( int i );
			// kasuje ciag na pozycji i
			void del( int i );
			// zmienia dlugosc i-tego ciagu.
			void resize( int i, int asize );
			//Umiesczamy wywolanie funkcji w miejsu compStore a pozniej przetwarzamy zebrane ciagi
			// czysci kontener
			CompStore< std::back_insert_iterator< std::vector< int > >,std::back_insert_iterator< std::vector< T > > >
				input();
		};

	};

	/** \brief Visitors.
	 *
	 *  The visitor's code is called by searching algorithms (BFS, DFS, LexBFS).
	 *  Allows to give code that should be called in various stages of search algorithm.\n
	 *  visitor should inherit from one of:
	 *  - simple_visitor_tag
	 *  - simple_preorder_visitor_tag
	 *  - simple_postorder_visitor_tag
	 *  - complex_visitor_tag
	 *
	 *  and one of:
	 *  - component_visitor_tag
	 *  - no_component_visitor_tag
	 *
	 *  Simple visitors (inheriting from simple_*_tag) have overloaded operator() called
	 *  with the following arguments:\n
	 *  <tt>template< class GraphType >
	 *  bool operator()(const GraphType &g,
	 *         typename GraphType::PVertex u,
	 *         VisitVertLabs< GraphType > &data) </tt>\n
	 *  where: \p g - the graph containing vertex, \p u - visited vertex, \p data - visited vertex's data\n
	 *  operator returns: true if it is allowed to continue search, false if terminate searching algorithm.\n
	 *  simple_preorder_visitor_tag indicate to visit vertex before its neighbourrs.\n
	 *  simple_postorder_visitor_tag indicate to visit vertex after its neighbourrs.\n
	 *  simple_visitor_tag do not specify the order.
	 *
	 *  complex visitors are notified by a searching algorithm using following methods
	 *  (\a g - graph containing vertex, \a u - visited vertex,
	 *   \a data - visited vertex's data, \a e - visited edge, \a v - vertex from which we
	 *   visit edge \a e):
	 *  - <tt>template< class GraphType >
	 *  bool visitVertexPre(const GraphType &g,
	 *             typename GraphType::PVertex u,
	 *             VisitVertLabs< GraphType > &data);</tt>\n
	 *  called before visiting u's neighbours
	 *  returning false prevents visiting u's neighbours.
	 *  - <tt>template< class GraphType >
	 *  bool visitVertexPost(const GraphType &g,
	 *          typename GraphType::PVertex u,
	 *          VisitVertLabs< GraphType > &data);</tt>\n
	 *  called after visiting u's neighbours
	 *  returning false terminates searching
	 *  - <tt>template< class GraphType >
	 *  bool visitEdgePre(const GraphType &g,
	 *           typename GraphType::PEdge e,
	 *           typename GraphType::PVertex v);</tt>\n
	 *  called before visiting other end of e
	 *  returning false prevents visiting vertex on the other end of e
	 *  - <tt>template< class GraphType >
	 *  bool visitEdgePost(const GraphType &g,
	 *            typename GraphType::PEdge e,
	 *            typename GraphType::PVertex v);</tt>\n
	 *  called after visiting other end of e
	 *  returning false terminates searching.
	 *
	 *  visitors with component_visitor_tag are notified about components with
	 *  methods:
	 *  - <tt>template< class GraphType >
	 *  bool beginComponent(const GraphType &g, unsigned compid);</tt>
	 *  - <tt>template< class GraphType >
	 *  bool endComponent(const GraphType &g, unsigned compid);</tt>
	 *
	 *  where \a g is the visited graph and \a compid is the component number (starting from 0)
	 *  return values are ignored.
	 *
	 */
	//TODO: vizytory nie powinny zakladac, ze dostaja rekord VisitVertLabs< GraphType > tylko cokolwiek o takiej
	//strukturze tzn. dodac parametr szablonu. To samo dotyczy implementacji konkretnych przeszukiwan
	class Visitors: public SearchStructs
	{
	public:
		class component_visitor_tag { } ;
		class no_component_visitor_tag { } ;
		class simple_visitor_tag { } ;
		class simple_preorder_visitor_tag: public simple_visitor_tag { } ;
		class simple_postorder_visitor_tag: public simple_visitor_tag { } ;
		class complex_visitor_tag { } ;

		template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_preorder_visitor_tag &s )
			{ (void)(s); return v( g,u,r ); }

		template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_postorder_visitor_tag &s )
			{ (void)(g); (void)(v); (void)(u); (void)(r); (void)(s); return true; }

		template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, complex_visitor_tag &c )
			{ (void)(c); return v.visitVertexPre( g,u,r ); }

		template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, ... )
			{ return v.operator()( g,u,r ); }

		template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_preorder_visitor_tag &s )
			{ (void)(g); (void)(v); (void)(u); (void)(r); (void)(s); return true; }

		template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_postorder_visitor_tag &s )
			{ (void)(s); return v( g,u,r ); }

		template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, complex_visitor_tag &c )
			{ (void)(c); return v.visitVertexPost( g,u,r); }

		template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
			typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, ... )
			{ return v.operator()( g,u,r ); }

		template< class GraphType, class Visitor > static bool visitEdgePre( const GraphType &g, Visitor &v,
			typename GraphType::PEdge e, typename GraphType::PVertex u, complex_visitor_tag &c )
			{ (void)(c); return v.visitEdgePre( g,e,u ); }

		template< class GraphType, class Visitor > static bool visitEdgePre( const GraphType &g, Visitor &v,
			typename GraphType::PEdge e, typename GraphType::PVertex u, simple_visitor_tag &s )
			{ (void)(g); (void)(v); (void)(e); (void)(u); (void)(s); return true; }

		template< class GraphType, class Visitor > static bool visitEdgePre( const GraphType &g, Visitor &v,
			typename GraphType::PEdge e, typename GraphType::PVertex u, ... )
			{ (void)(g); (void)(v); (void)(e); (void)(u); return true; }

		template< class GraphType, class Visitor > static bool visitEdgePost( const GraphType &g, Visitor &v,
			typename GraphType::PEdge e, typename GraphType::PVertex u, complex_visitor_tag &c )
			{ (void)(c); return v.visitEdgePost( g,e,u ); }

		template< class GraphType, class Visitor > static bool visitEdgePost( const GraphType &g, Visitor &v,
			typename GraphType::PEdge e, typename GraphType::PVertex u, simple_visitor_tag &s )
			{ (void)(g); (void)(v); (void)(e); (void)(u); (void)(s); return true; }

		template< class GraphType, class Visitor > static bool visitEdgePost( const GraphType &g, Visitor &v,
			typename GraphType::PEdge e, typename GraphType::PVertex u, ... )
			{ (void)(g); (void)(v); (void)(e); (void)(u); return true; }

		template< class GraphType, class Visitor > static bool beginComponent( const GraphType &g, Visitor &v,
			unsigned comp, component_visitor_tag &c )
			{ (void)(c); return v.beginComponent( g,comp ); }

		template< class GraphType, class Visitor > static bool beginComponent( const GraphType &g, Visitor &v,
			unsigned comp, no_component_visitor_tag &c )
			{ (void)(g); (void)(v); (void)(comp); (void)(c); return true; }

		template< class GraphType, class Visitor > static bool beginComponent( const GraphType &g, Visitor &v,
			unsigned comp, ... )
			{ (void)(g); (void)(v); (void)(comp); return true; }

		static bool beginComponent( ... )
		{ return true; }

		template< class GraphType, class Visitor > static bool endComponent( const GraphType &g, Visitor &v,
			unsigned comp, component_visitor_tag &c )
			{ (void)(c); return v.endComponent( g,comp ); }

		template< class GraphType, class Visitor > static bool endComponent( const GraphType &g, Visitor &v,
			unsigned comp, no_component_visitor_tag &c )
			{ (void)(g); (void)(v); (void)(comp); (void)(c); return true; }

		template< class GraphType, class Visitor > static bool endComponent( const GraphType &g, Visitor &v,
			unsigned, ... )
			{ (void)(g); (void)(v); return true; }

		static bool endComponent( ... )
		{ return true; }

		/** \brief Empty (exemplary) visitor.  */
		class EmptyVisitor: public simple_postorder_visitor_tag, public no_component_visitor_tag
		{
		public:
			EmptyVisitor()
				{ }

			template< class GraphType >  bool operator()( const GraphType &g, typename GraphType::PVertex u,
				VisitVertLabs< GraphType > &r )
				{ return true; }
		 };

		/** \brief Visitor (exemplary) terminate algorithm when reaching given vertex */
		class EndVertVisitor: public complex_visitor_tag, public no_component_visitor_tag
		{
		public:
			EndVertVisitor( void *arg ): ptr( arg )
				{ }

			template< class GraphType > bool visitVertexPre( const GraphType &g, typename GraphType::PVertex u,
				VisitVertLabs< GraphType > &data )
				{ return true; }

			template< class GraphType > bool visitVertexPost( const GraphType &g, typename GraphType::PVertex u,
				VisitVertLabs< GraphType > &v )
				{ return true; }

			template< class GraphType > bool visitEdgePre( const GraphType &g, typename GraphType::PEdge e,
				typename GraphType::PVertex u )
				{ return (void*)u != ptr; }

			template< class GraphType > bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e,
				typename GraphType::PVertex u )
				{ return !e || (void*)g.getEdgeEnd( e,u ) != ptr; }

		private:
			void *ptr;
		};

		/** \brief Visitor stores visited vertices to iterator VertIter.  */
		template< class VertIter > class StoreTargetToVertIter: public simple_postorder_visitor_tag,
			public no_component_visitor_tag
		{
		public:
			StoreTargetToVertIter( VertIter &i ): m_iter( i )
			{ }

			template< class GraphType > bool operator()( const GraphType &, typename GraphType::PVertex,
				VisitVertLabs< GraphType > & );

		private:
			VertIter &m_iter;
		} ;

		/** \brief Visitor stores visited vertices to CompStore.
		 */
		template< class CompIter, class VertIter > class StoreCompVisitor: public simple_postorder_visitor_tag,
			public component_visitor_tag
		{
		public:
			typedef struct _State
			{
				CompStore< CompIter,VertIter > iters;
				unsigned p, id;

				_State( CompStore< CompIter,VertIter > i );
			} State;

			StoreCompVisitor( State &st ): m_st( st )
			{ }

			template< class GraphType > bool operator()( const GraphType &, typename GraphType::PVertex,
				VisitVertLabs< GraphType > & );

			template< class GraphType > bool beginComponent( const GraphType &g, unsigned u )
				{ (void)(g); (void)(u); return true; }
			template< class GraphType > bool endComponent( const GraphType &, unsigned );

		private:
			State &m_st;
		};

		/* \brief Preorder visitor.
		 *
		 *  Modifies simple visitor to behave like preorder visitor.
		 */
		template< class Visitor > class ComplexPreorderVisitor: public complex_visitor_tag
		{
		public:
			ComplexPreorderVisitor( Visitor &v ): visit( v )
			{ }

			template< class GraphType > bool visitVertexPre( const GraphType &g, typename GraphType::PVertex u,
				VisitVertLabs< GraphType > &data )
				{ return visit.operator()( g,u,data ); }

			template< class GraphType > bool visitVertexPost( const GraphType &g, typename GraphType::PVertex u,
				VisitVertLabs< GraphType > &data )
				{ return true; }

			template< class GraphType > bool visitEdgePre( const GraphType &g, typename GraphType::PEdge e,
				typename GraphType::PVertex u )
				{ return true; }

			template< class GraphType > bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e,
				typename GraphType::PVertex u )
				{ return true; }

		private:
			Visitor &visit;
		};

		 /* \brief Postorder visitor.
		 *
		 *  Modifies simple visitor to behave like postorder visitor.
		 */
		template< class Visitor > class ComplexPostorderVisitor: public complex_visitor_tag
		{
		public:
			ComplexPostorderVisitor( Visitor &v ): visit( v )
				{ }

			template< class GraphType > bool visitVertexPre( const GraphType &g, typename GraphType::PVertex u,
				VisitVertLabs<GraphType> &data )
				{ return true; }

			template< class GraphType > bool visitVertexPost( const GraphType &g, typename GraphType::PVertex u,
				VisitVertLabs< GraphType > &data )
				{ return visit.operator()( g,u,data ); }

			template< class GraphType > bool visitEdgePre( const GraphType &g, typename GraphType::PEdge e,
				typename GraphType::PVertex u )
				{ return true; }

			template< class GraphType > bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e,
				typename GraphType::PVertex u )
				{ return true; }

		private:
			Visitor &visit;
		};
	};

	/* GraphSearchBase
	 * Ogolna implementacja przeszukiwania grafu roznymi strategiami (DFS, BFS, LexBFS)
	 * Strategie dostarcza metoda visitBase z klasy SearchImpl, ktora odwiedza wierzcholki stosownie do swojej klasy
	 * DefaultStructs dostarcza wytycznych dla wewnetrznych procedur por. np. AlgsDefaultSettings z def_struct.h
	 *
	 */
	/** \brief Basic graph search algorithms (parametrized).
	 *
	 *  The general implementation of graph search strategies (DFS, BFS, LexBFS).
	 *  \tparam SearchImpl the class should deliver a method visitBase, which decides about the order of visiting vertices.
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \ingroup search */
	template< class SearchImpl, class DefaultStructs > class GraphSearchBase: public ShortPathStructs, public SearchStructs
	{
	protected:
		// Typowy kontener dla wierzcholkow uzyteczny w tych procedurach
		// mapa PVertex -> VisitVertLabs< GraphType >
		template< class GraphType > class VisitedMap: public DefaultStructs:: template AssocCont<
			typename GraphType::PVertex,VisitVertLabs< GraphType > >::Type
			{
			public:
				// inicjujemy przewidywanym potrzebnym rozmiarem
				VisitedMap( int asize ): DefaultStructs:: template
					AssocCont< typename GraphType::PVertex,VisitVertLabs< GraphType > >::Type( asize ) { }
			};

	public:
		typedef SearchImpl SearchStrategy;
		//TODO: przeszukiwania powinny dzialac na lokalnej mapie i pozwalac na blackHole - mniej przeciazen
		/** \brief Visit all vertices.
		 *
		 *  Visit all vertices in a graph in order given by the strategy SearchImpl
		 *  @param[in] g the graph containing vertices to visit
		 *  @param[in] visited the container to store data (np. map PVertex -> VisitVertLabs), BlackHole forbidden.
		 *   After the execution of the method, the associative container represent the search tree (forst)
		 *   where fields vPrev and ePrev keep the previous vertex and edge, and field distance keeps the distance from the root.
		 *   finally field component keeps the index of the connected component of graph.
		 *  @param[in] visitor object that delivers a set of functions which are called for each vertex and for various stages of algorithm.
		 *  @param[in] dir the direction of edges to consider.
		 *  @return the number of components.
		 *  \sa SearchStructs::VisitVertLabs
		 *  \sa Visitors */
		template< class GraphType, class VertContainer, class Visitor >
			static int visitAllBase( const GraphType &g, VertContainer &visited, Visitor visitor, EdgeDirection dir );

		//Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia, component=0
		/** \brief Visit attainable.
		 *
		 *  Visit all vertices in the same component as a given vertex in order given by the strategy SearchImpl
		 *  @param[in] g the graph containing vertices to visit
		 *  @param[in] src the given vertex
		 *  @param[out] out the iterator to write visited vertices in order given by the strategy  SearchImpl.
		 *  @param[in] dir the direction of edges to consider, loops are ignored regardless of the mask.
		 *  @param[out] visited the container to store data (map PVertex -> VisitVertLabs) , BlackHole forbidden.
		 *   After the execution of the method, the associative container represent the search tree (forst)
		 *   where fields \p vPrev and \p ePrev keep the previous vertex and edge, and the field \p distance keeps the distance from the root.
		 *   finally \p field component = 0.
		 *  @return the number of visited vertices.
		 *  \sa SearchStructs::VisitVertLabs
		 *
		 * [See example](examples/search/search/search.html).
		 */
		template< class GraphType, class VertContainer, class Iter > static int scanAttainable( const GraphType &,
			typename GraphType::PVertex, Iter, EdgeDirection, VertContainer & );

		/** \brief Visit attainable.
		 *
		 *  Visit all vertices in the same component as a given vertex. Behaves similarlly previous method by uses own map PVertex -> VisitVertLabs.
		 *  @param[in] g the graph containing vertices to visit
		 *  @param[in] src the given vertex
		 *  @param[out] out the iterator to write vertices to, in order given by the strategy SearchImpl
		 *  @param[in] dir the direction of edges to consider, loops are ignored regardless of the mask.
		 *  @return the number of visited vertices. */
		template< class GraphType, class VertIter > static int scanAttainable( const GraphType &g,
			typename GraphType::PVertex src, VertIter out, EdgeDirection dir = EdUndir | EdDirOut );

		/** \brief Visit all vertices.
		 *
		 *  Visit all vertices in a graph
		 *  @param[in] g the graph containing vertices to visit
		 *  @param[out] out the iterator to write visited vertices to, in order given by the strategy SearchImpl
		 *  @param[in] dir the direction of edges to consider, loops are ignored regardless of the mask.
		 *  @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole forbidden.
		 *   After the execution of the method, the associative container represent the search tree (forest)
		 *   where fields vPrev and ePrev keep the previous vertex and edge, and field distace keeps the distance from the root.
		 *   finally field component keeps the index of the connected component of graph.
		 *  @param[in] sym if true arcs are treated as undirected edges.
		 *  @return the number of components.
		 *  \sa SearchStructs::VisitVertLabs */
		template< class GraphType, class VertContainer, class VertIter > static int scan( const GraphType &g,
			VertIter out, EdgeDirection dir, VertContainer &visited, bool sym = true );

		/** \brief Visit all vertices.
		 *
		 *  Visit all vertices in a graph.
		 *  @param[in] g the graph containing vertices to visit
		 *  @param[out] out iterator to write vertices to, in order given by the strategy SearchImpl
		 *  @param[in] dir the direction of edges to consider, loops are ignored regardless of the mask.
		 *  @return the number of components.*/
		template< class GraphType, class VertIter > static int scan( const GraphType &g, VertIter out,
			EdgeDirection dir = EdDirOut | EdUndir | EdDirIn );

		/* Liczba cyklomatyczna podgrafu zlozonego z krawedzi zgodnych z maska */
		/** \brief Cyclomatic number of graph.
		 *
		 *  The method gets the cyclomatic number of graph, concerning only edges congruent with \a mask.
		 *  \param g the considered graph.
		 *  \param mask determines the types and direction of edges to be considered.
		 *  \return the cyclomatic number of graph.  */
		template< class GraphType > static int cyclNo( const GraphType &g, EdgeDirection mask = EdAll )
			{ return g.getEdgeNo( mask ) - g.getVertNo() + scan( g,blackHole,mask ); }

		/** \brief Get contected component.
		 *
		 *  The method returns all vertices in the same component as a given vertex.
		 *  @param[in] g graph containing vertices to visit.
		 *  @param[in] src the given vertex.
		 *  @param[in] dir the direction of edges to consider. Loops are ignored.
		 *  @return the set of vertices in the component.  */
		template< class GraphType > static Set< typename GraphType::PVertex > getAttainableSet( const GraphType &g,
			typename GraphType::PVertex src, EdgeDirection dir = EdDirOut | EdUndir );

		/** \brief Get path.
		 *
		 *  The method finds a path between vertices
		 *  @param[in] g the graph to search path in.
		 *  @param[in] src the starting vertex.
		 *  @param[in] dest the target vertex.
		 *  @param[out] path found path
		 *  @param[in] dir the mask determining the direction of edges to consider, loops are ignored.
		 *  @return the length of the path, -1 if there is no connection.
		 *  \sa PathStructs::OutPath
		 *  \sa PathStructs::OutPathTool
		 */
		template< class GraphType, class VertIter, class EdgeIter > static int getPath( const GraphType &g,
			typename GraphType::PVertex src, typename GraphType::PVertex dest, OutPath< VertIter,EdgeIter > path,
			EdgeDirection dir = EdUndir | EdDirOut );

		/** \brief Split into components.
		 *
		 *  The method splits graph into connected components.
		 *  @param[in] g the graph to split.
		 *  @param[out] out CompStore object that is a pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
		 *  @param[in] dir direction of edges to consider, loops are ignored.
		 *  @param[out] visited container to store data (map PVertex -> VisitVertLabs), BlackHole forbidden.
		 *   After the execution of the method, the associative container represent the search tree (forst)
		 *   where fields vPrev and ePrev keep the previous vertex and edge, and field distance keeps the distance from the root.
		 *   finally field component keeps the index of the connected component of graph.
		 *  @return the number of components.
		 *  \sa CompStore
		 *  \sa Visitors
		 *
		 * [See example](examples/search/search/search.html).
		 */
		template< class GraphType, class VertContainer, class CompIter, class VertIter > static int split(
			const GraphType &g, CompStore< CompIter,VertIter > out, EdgeDirection dir, VertContainer &visited );

		/** \brief Split graph.
		 *
		 *  The method splits graph into connected components. The simpler version of the above method. It uses its own map for search tree.
		 *  @param[in] g the graph to split.
		 *  @param[out] out the CopmStore object that is a pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
		 *  @param[in] dir direction of edges to consider, loops are ignored.
		 *  @return the number of components.
		 *  \sa CompStore */
		template< class GraphType, class CompIter, class VertIter > static int split( const GraphType &g,
			CompStore< CompIter,VertIter > out, EdgeDirection dir = EdUndir | EdDirOut | EdDirIn );
	};

	/*
	* DFSParamBlock
	*/
	template< class GraphType, class VertContainer, class Visitor > struct DFSParamBlock: public SearchStructs
	{
		const GraphType &g;
		VertContainer &visited;
		Visitor &visit;
		EdgeDirection mask;
		int component;

		DFSParamBlock( const GraphType &_g, VertContainer &_v, Visitor &_vis, EdgeDirection _m, int _c ):
			g( _g ), visited( _v ), visit( _vis ), mask( _m ), component( _c )
			{ }
	};

	/*
	* DFSBase
	* DefaultStructs - wytyczne dla wewnetrznych procedur
	*/
	template< class SearchImpl, class DefaultStructs > class DFSBase: public GraphSearchBase< SearchImpl,DefaultStructs >
	{
	protected:
		template< class GraphType, class VertContainer, class Visitor > static int dfsDoVisit(
			DFSParamBlock< GraphType,VertContainer,Visitor > &, typename GraphType::PVertex, unsigned );

	public:
		/** \brief Visit all vertices in given component.
		 *
		 *  Visit all vertices in the same component as a given vertex
		 *  @param[in] g graph containing vertices to visit
		 *  @param[in] src given vertex
		 *  @param[in] visited container to store data (map PVertex -> VisitVertLabs),  (BlackHole forbidden). The search tree may be reconstructed from fields vPrev and ePrev in VisitVertLabs, also distance from the root and number of component (compid) is kept there.
		 *  @param[in] visitor visitor called for each vertex
		 *  @param[in] dir direction of edges to consider.
		 * - EdDirOut arcs are traversed according to their direction,
		 * - EdDirIn arcs are traversed upstream,
		 * - Directed arcs may be traversed in both directions.
		 *  @param[in] compid component identifier (give 0 if don't know)
		 *  @return number of visited vertices or -number if  visitor interrupted the search.
		 */
		template< class GraphType, class VertContainer, class Visitor > static int dfsVisitBase( const GraphType &g,
			typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
	};


	/** \brief Preorder Depth-First-Search (parametrized)
	 *
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \ingroup search
	 */
	template< class DefaultStructs > class DFSPreorderPar: public DFSBase< DFSPreorderPar< DefaultStructs >,DefaultStructs >
	{
	protected:
		template< class GraphType, class VertContainer, class Visitor > static int dfsPreVisitBase( const GraphType &,
			typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int,
			Visitors::complex_visitor_tag & );

		template< class GraphType, class VertContainer, class Visitor > static int dfsPreVisitBase( const GraphType &,
			typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int,
			Visitors::simple_visitor_tag & );

	public:
		/** \brief Visit all vertexes from component.
		*
		*  The method visits all the vertices in the same component as a given vertex.
		* @param[in] g the graph containing vertices to visit
		* @param[in] src the given vertex
		* @param[in] visited the container to store data (map PVertex -> VisitVertLabs), (BlackHole forbidden). The search tree may be reconstructed from fields vPrev and ePrev in VisitVertLabs, also distance from the root and number of component (compid) is kept there.
		* @param[in] visitor visitor called for each vertex
		* @param[in] dir direction of edges to consider
		 * - EdDirOut arcs are traversed according to their direction,
		 * - EdDirIn arcs are traversed upstream,
		 * - Directed arcs may be traversed in both directions.
		* @param[in] compid component identifier (give 0 if don't know)
		* @return number of visited vertices or -number if the visitor interrupted the search.
		*/
		template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType &g,
			typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
	} ;

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Preorder Depth-First-Search (default).
	 *
	 *  By default DefaultStructs = AlgsDefaultSettings.
	 *  \ingroup search
	 */
	class DFSPreorder: public DFSPreorderPar< AlgsDefaultSettings > { };

	/** \brief Postorder Depth-First-Search (parametrized)
	 *
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algoritms.
	 *  \ingroup search
	 */
	template< class DefaultStructs > class DFSPostorderPar: public DFSBase< DFSPostorderPar< DefaultStructs >,DefaultStructs >
	{
	protected:
		template< class GraphType, class VertContainer, class Visitor > static int dfsPostVisitBase( const GraphType &,
			typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int,
			Visitors::complex_visitor_tag & );

		template< class GraphType, class VertContainer, class Visitor > static int dfsPostVisitBase( const GraphType &,
			typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int, Visitors::simple_visitor_tag & );

	public:
		/** \brief Visit all vertexes of component.
		*
		* Visit all vertices in the same component as a given vertex.
		* @param[in] g graph containing vertices to visit
		* @param[in] src given vertex
		* @param[in] visited container to store data (map PVertex -> VisitVertLabs),  (BlackHole forbidden). The search tree may be reconstructed from fields vPrev and ePrev in VisitVertLabs, also distance from the root and number of component (compid) is kept there.
		* @param[in] visitor visitor called for each vertex
		* @param[in] dir direction of edges to consider.
		 * - EdDirOut arcs are traversed according to their direction,
		 * - EdDirIn arcs are traversed upstream,
		 * - Directed arcs may be traversed in both directions.
		* @param[in] compid component identifier (give 0 if don't know)
		* @return number of visited vertices of -number if the visitor interrupted the search.
		*/
		template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType &g,
			typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Postorder Depth-First-Search (default)
	 *
	 *  By default DefaultStructs = AlgsDefaultSettings.
	 *  \ingroup search
	 */
	class DFSPostorder: public DFSPostorderPar< AlgsDefaultSettings > { };

	/** \brief Breadth-First-Search (parametrized).
	 *
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \ingroup search
	 */
	template< class DefaultStructs > class BFSPar: public GraphSearchBase< BFSPar< DefaultStructs >,DefaultStructs >
	{
	protected:
		template< class GraphType, class VertContainer, class Visitor > static int bfsDoVisit( const GraphType &,
			typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int );

	public:
		/** \brief Visit all vertexes from component.
		*
		* visit all vertices in the same component as a given vertex.
		* @param[in] g graph containing vertices to visit
		* @param[in] src given vertex
		* @param[in] visited container to store data (map PVertex -> VisitVertLabs), (BlackHole forbidden). The search tree may be reconstructed from fields vPrev and ePrev in VisitVertLabs, also distance from the root and number of component (compid) is kept there.
		* @param[in] visitor visitor called for each vertex
		* @param[in] dir direction of edges to consider.
		 * - EdDirOut arcs are traversed according to their direction,
		 * - EdDirIn arcs are traversed upstream,
		 * - Directed arcs may be traversed in both directions.
		* @param[in] compid component identifier (give 0 if don't know)
		* @return number of visited vertices of -number if the visitor interrupted the search.
		*/
		template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType &g,
			typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Breadth-First-Search (default).
	 *
	 *  By default DefaultStructs = AlgsDefaultSettings.
	 *  \ingroup search */
	class BFS: public BFSPar< AlgsDefaultSettings > { };

	/*
	* lexicographical Breadth-First-Search
	*/
	/** \brief Lexicographical Breadth-First-Search (parametrized).
	 *
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \ingroup search */
	template< class DefaultStructs > class LexBFSPar:
		public GraphSearchBase< LexBFSPar< DefaultStructs >,DefaultStructs >
	{
	protected:
		template< class Graph > struct LVCNode
		{
			typename Graph::PVertex v;
			Privates::List_iterator< LVCNode > block;

			LVCNode( typename Graph::PVertex _v = NULL): v( _v ) { }
			LVCNode( typename Graph::PVertex _v, Privates::List_iterator< LVCNode > it ): v( _v ), block( it )
				{ }
		};

		template< class Graph, class Allocator, class ContAllocator > class LexVisitContainer
		{
		public:
			typedef LVCNode< Graph > Node;

			class Container: public Privates::List< Node,ContAllocator >
			{
			public:
				Container( ContAllocator &a): Privates::List< Node,ContAllocator >( a )
				{ }
			};

			Container m_data;
			Privates::List_iterator< Node > m_openBlock;
			Privates::List< Privates::List_iterator< Node >,Allocator > m_splits;
			typename DefaultStructs::template
				AssocCont< typename Graph::PVertex,Privates::List_iterator< Node > >::Type m_vertexToPos;

			LexVisitContainer( Allocator& a, ContAllocator& ca, int n):
				m_data( ca ), m_openBlock(), m_splits( a ),  m_vertexToPos( n )
				{ }

			~LexVisitContainer()
				{ clear(); }

			void clear();
			void initialize( const Graph &g );
			void initialize( const Graph &g, size_t n, typename Graph::PVertex *tab );
			void initializeAddAll( const Graph &g );
			void cleanup();
			bool empty();
			typename Graph::PVertex top();
			void pop();
			void push( typename Graph::PVertex v );
			void move( typename Graph::PVertex v );
			void remove( typename Graph::PVertex v )
				{ m_data.erase( m_vertexToPos[v] ); }
			void done();
			void dump();
		};

		template< class GraphType > struct OrderData
		{
			typename GraphType::PVertex v;
			// kogo jest s¹siadem (numer s¹siada w porz¹dku)
			int vertId;
			// numer w porz¹dku
			int orderId;
		};

	public:
		/** Get LexBFS sequence.
		 *
		 *  The method arranges vertices with LexBFS order, starting with a given sequence and writes it down to the \a out container.
		 *  @param[in] g the considered graph,
		 *  @param[in] in the number of vertices in table \a tab.
		 *  @param[in] tab table containing initial order of vertices
		 *  @param[in] mask the direction of edges to consider, LexBFS only symmetric masks are allowed.
		 *  @param[out] out the iterator to write the output ordered vertices sequence.
		 *  @return the number of vertices written to \a out. */
		template< class GraphType, class OutVertIter > static int order2( const GraphType & g, size_t in,
			typename GraphType::PVertex *tab, EdgeDirection mask, OutVertIter out );

		template< class T > static void StableRadixSort( T *data, int n, int nb, int T::*field, T *out );

		/** \brief Visit all vertexes of component.
		*
		* Visit all vertices in the same component as a given vertex.
		* @param[in] g graph containing vertices to visit
		* @param[in] src given vertex
		* @param[in] visited container to store data (map PVertex -> VisitVertLabs),  (BlackHole forbidden). The search tree may be reconstructed from fields vPrev and ePrev in VisitVertLabs, also distance from the root and number of component (compid) is kept there.
		* @param[in] visitor visitor called for each vertex
		* @param[in] dir direction of edges to consider, only symmetric masks are acceptable.
		* @param[in] compid component identifier (give 0 if don't know)
		* @return number of visited vertices or -number if the visitor interrupted the search.
		*/
		template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType & g,
			typename GraphType::PVertex start, VertContainer &visited, Visitor visit, EdgeDirection mask,
				int component );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Lexicographical Breadth-First-Search (default)
	 *
	 *  By default DefaultStructs = AlgsDefaultSettings.
	 *  \ingroup search   */
	class LexBFS: public LexBFSPar< AlgsDefaultSettings > { };

	/*
	 * Cheriyan/Mehlhorn/Gabow algorithm
	 * Wyszukiwanie skladowych silnie spojnych grafu skierowanego
	 */
	/** \brief Cheriyan/Mehlhorn/Gabow algorithm (parametrized).
	 *
	 *  The algorithm for searching strongly connected components of directed graph.
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algoritms.
	 *  \ingroup search
	 */
	template< class DefaultStructs > class SCCPar: protected SearchStructs
	{
	protected:
		struct SCCData
		{
			int preIdx;
			bool assigned;

			SCCData( int p = 0, bool a = false ): preIdx( p ), assigned( a )
				{ }
		};

		template< class GraphType, class CompIter, class VertIter, class CompMap > class SCCState
		{
		public:
			SCCState( CompStore< CompIter,VertIter > _i, CompMap &cm, typename GraphType::PVertex *buf1,
				typename GraphType::PVertex *buf2, int n ): vmap( n ), s( buf1,n ), p( buf2,n ), iters( _i ),
				 compMap( cm ), idx( 0 ), count( 0 ), c( 0 )
					{ }

			void addVert( typename GraphType::PVertex );
			void newComp();

			typename DefaultStructs::template AssocCont< typename GraphType::PVertex,SCCData >::Type vmap;
			StackInterface< typename GraphType::PVertex * > s,p;
			CompStore< CompIter,VertIter > iters;
			CompMap &compMap;
			unsigned idx;
			int count,c;
		};

		template< class GraphType, class CompIter, class VertIter, class CompMap > class SCCVisitor:
			public Visitors::complex_visitor_tag, public Visitors::no_component_visitor_tag
		{
		public:
			SCCVisitor( SCCState< GraphType,CompIter,VertIter,CompMap > & );

			bool visitVertexPre( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
			bool visitVertexPost( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
			bool visitEdgePre( const GraphType &, typename GraphType::PEdge, typename GraphType::PVertex );
			bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e, typename GraphType::PVertex u )
				{ return true; }

		private:
			SCCState< GraphType,CompIter,VertIter,CompMap > &state;
		};

	public:
		/** \brief Get the strongly connected components of graph.
		 *
		 *  The method splits graph into strongly connected components, all types of edges are considered.
		 *  @param[in] g the graph to split
		 *  @param[out] out strongly connected components of the graph \a g in the CompStore object that keeps a  pair of output iterators (elements of first iterator will point to first vertex in component in second iterator).
		 *  @param[out] vtoc map (PVertex -> int indexOfItsComponent(zero based)), or BlackHole.
		 *  @return the number of components.
		 *  \sa CompStore
		 *
		 *  [See example](examples/search/scc/scc.html).
		 */
		template< class GraphType, class CompIter, class VertIter, class CompMap > static int
			split( const GraphType &g, CompStore< CompIter,VertIter > out, CompMap & vtoc );

		//    Korzysta z mapy CompMap z poprzedniej procedury. Wyrzuca na iterator wartosci std::pair<int,int> - wszystkie
		//    pary numerow skladowych silnie spojnych, ktore sa polaczone choc jednym bezposrednim lukiem. Zwraca dlugos
		//    ciagu wynikowego
		/** \brief Get connections between components.
		 *
		 *  The method gets connections between strongly connected components and writes it down to the container \a iter
		 *    as pairs of indexes of components in \a comp.
		 *  \param g the considered graph
		 *  \param comp the map achieved by the above \p split method.
		 *  \param[out] iter the iterator to the container with pairs of integers that represent the numbers of components in
		 *   \a comp that share a vertex.
		 *  \return the number of pairs in \a iter.	 */
		template< class GraphType, class CompMap, class PairIter >
			static int connections(const GraphType &g, CompMap &comp, PairIter iter );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings (default)
	/** \brief Cheriyan/Mehlhorn/Gabow algorithm
	 *
	 *  The algorithm for searching strongly connected components of directed graph.
	 *  By default DefaultStructs = AlgsDefaultSettings.
	 *  \ingroup search
	 */
	class SCC: public SCCPar< AlgsDefaultSettings > { };

	//Procedury na digrafach acykliczych
	/** \brief Procedures for directed acyclic graphs (DAG).
	 *
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \sa RelDiagramPar
	 *  \ingroup search    */
	template< class DefaultStructs > class DAGAlgsPar: protected SearchStructs
	{
	public:
		// wyrzuca na iterator wierzcholki grafu w porzadku topologicznym
		/** \brief Get topological order.
		 *
		 *  The method searches the graph \a g in postorder DFS. The result is written to out iterator.
		 *  \param g the considered graph.
		 *  \param out the iterator of container with output sequence of vertices.
		 *  \n
		 *
		 *  [See example](examples/search/dagalgs/dagalgs.html).
		 */
		template< class GraphType, class VertIter > static void topOrd( const GraphType &g, VertIter out );

		// sprawdza, czy graf jest DAGiem korzystajac z podanego para iteratorow ciagu wierzcholkow z wyjscia poprzedniej procedury
		/** \brief Test if directed acyclic graph.
		 *
		 *  The method uses the sequence of vertices achieved by the above topOrd function to test if the graph \a g is a directed acyclic graph.
		 *  \param g the tested graph.
		 *  \param beg the iterator to the first element of the container with topological order.
		 *  \param end the iterator to the past-the-end element of the container with topological order.
		 *  \return true if \a g is directed acyclic graph, false otherwise.
		 *  \n
		 *
		 *  [See example](examples/search/dagalgs/dagalgs.html).
		 */
		template< class GraphType, class Iter > static bool isDAG( const GraphType &g, Iter beg, Iter end );

		// sprawdza, czy graf jest DAGiem, samodzielna
		/** \brief Test if directed acyclic graph.
		 *
		 *  The method test if the graph \a g is a directed acyclic graph.
		 *  \param g the tested graph.
		 *  \return true if \a g is directed acyclic graph, false otherwise.
		 *  \n
		 *
		 *  [See example](examples/search/dagalgs/dagalgs.html).
		 */
		template< class GraphType > static bool isDAG( const GraphType &g );

		// wyrzuca na iterator wszystkie luki przechodnie DAGa, zwraca dlugosc ciagu
		/** \brief Get transitive edges.
		 *
		 *  The method gets all transitive edges and saves them to the iterator \a out.
		 *  \param g the considered graph.
		 *  \param out the iterator to the container with transitive edges.
		 *  \return the number of transitive edges.*/
		template< class GraphType, class Iter > static int transEdges(const GraphType &g, Iter out);

		// usuwa luki przechodnie DAGa
		/** \brief Make Hasse diagram.
		 *
		 *  The method deletes all the transitive edges from graph.
		 *  \param g the considered graph.
		 *  \n
		 *
		 *  [See example](examples/search/dagalgs/dagalgs.html).
		 */
		template< class GraphType > static void makeHasse( GraphType &g );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Procedures for directed acyclic graphs (DAG) (default).
	 *
	 *  The simpler default  version of DAGAlgsPar.
	 *  \sa RelDiagramPar \sa DAGAlgsPar
	 *  \ingroup search    */
	class DAGAlgs: public DAGAlgsPar< AlgsDefaultSettings > { };

	/** \brief Searching blocks = biconnected components.
	 *
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \ingroup search    */
	template< class DefaultStructs > class BlocksPar: public SearchStructs
	{
	protected:
		template< class GraphType > struct BiConVData
		{
			unsigned depth;
			unsigned lowpoint;
			unsigned count;
			bool iscut;
			int visited;
			int sons;
			int vblFirst;
		};

		struct VertBlockList
		{
			int block;
			int next;
		};

		template< class GraphType, class CompIter, class VertIter, class EdgeMap > class BiConState
		{
		public:
			typedef typename GraphType::PVertex PVertex;

			BiConState( CompStore< CompIter,VertIter > _i, EdgeMap &em, EdgeDirection _m,
				std::pair< typename GraphType::PEdge *,int > st, VertBlockList *_vbl, int vno ):
				vmap( vno ), emap( st.second - 1 ), estk( st.first,st.second ), iters( _i ), outEMap( em ),
				vbl( _vbl ), vblAlloc( 0 ), idx( 0 ), count( 0 ), mask( _m )
				{ }

			void addVert( typename GraphType::PVertex );
			void addEdge( typename GraphType::PEdge e )
				{ if (!isBlackHole( outEMap )) outEMap[e] = count; }

			void newComp();

			typename DefaultStructs::template AssocCont< typename GraphType::PVertex,BiConVData< GraphType > >::Type
				vmap;
			typename DefaultStructs::template AssocCont< typename GraphType::PEdge,bool >::Type emap;
			StackInterface< typename GraphType::PEdge* > estk;
			CompStore< CompIter,VertIter > iters;
			EdgeMap &outEMap;
			VertBlockList *vbl;
			int vblAlloc;
			unsigned idx;
			int count;
			EdgeDirection mask;
		};

		template< class GraphType, class CompIter, class VertIter, class EdgeMap > class BiConVisitor:
			public Visitors::complex_visitor_tag, public Visitors::no_component_visitor_tag
		{
		public:
			BiConVisitor( BiConState< GraphType,CompIter,VertIter,EdgeMap > & );

			bool visitVertexPre( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
			bool visitVertexPost( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
			bool visitEdgePre( const GraphType &, typename GraphType::PEdge, typename GraphType::PVertex );
			bool visitEdgePost( const GraphType &, typename GraphType::PEdge, typename GraphType::PVertex );

			private:
				BiConState< GraphType,CompIter,VertIter,EdgeMap > &state;
		};

		template< class State, class VertMap, class VertBlockIter >
			static void storeBlocksData( State &, VertBlockList *, VertMap &, VertBlockIter & );

	public:
		// wynikowa etykieta wierzcholka
		/**\brief Vertex data used to represent blocks. */
		struct VertData {
			// w ilu blokach lezy ten wierzcholek
			int blockNo; /**<\brief Number of blocks the vertex is in.*/
			// pozycja pierwszego w sekwencji numerow blokow (por. viter nizej) bloku zawierajacego
			/** \brief First block position.
			 *
			 *  The position of the first block the vertex belongs to in the sequence \a viter in \p split method.*/
			int firstBlock;
			// ten wierzcholek (jego pozostale bloki wystepuja kolejno za nim)
			VertData( int b = 0, int f = -1 ): blockNo( b ), firstBlock( f )
				{ }
			/** \brief Copy.*/
			template <class T> void copy(T& arg) const
			{
				arg.blockNo=blockNo;
				arg.firstBlock=firstBlock;
			}
			/** \brief Copy.*/
			void copy(BlackHole&) const
				{ }
		};

		/** \brief Get blocks.
		 *
		 *  The method splits graph into blocks. All the edges are treated as undirected.
		 *  @param[in] g the graph to split.
		 *  @param[out] vmap the map PVertex->VertData sqould be considered together with sequence viter (BlackHole possible).
		 *  @param[out] emap the map PEdge->int associating each edge with a block number. (BlackHole possible)
		 *  @param[out] out the CompStore object with a pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
		 *  @param[out] viter the iterator to the container with concatenated sequences of blocks to which the each vertex belongs to.
		 *   For each vertex the starting point of sequence of blocks is given by \a vmap in the VertData field firstBlock.
		 *  @return the number of biconnected components.
		 *  \sa CompStore   */
		template< class GraphType, class VertDataMap, class EdgeDataMap, class CompIter, class VertIter,
			class VertBlockIter > static int split( const GraphType &g, VertDataMap &, EdgeDataMap &,
				CompStore< CompIter,VertIter > out, VertBlockIter viter );

		/** \brief Get blocks of connected component.
		 *
		 *  The method splits a component containing a given vertex into blocks. All the edges are treated as undirected.
		 *  @param[in] g the graph to split.
		 *  @param[in] src the reference vertex.
		 *  @param[out] vmap the map PVertex->VertData sqould be considered together with sequence viter (BlackHole possible).
		 *  @param[out] emap the map PEdge->int associating each edge with a block number. (BlackHole possible)
		 *  @param[out] out the CompStore object with a pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
		 *  @param[out] viter the iterator to the container with concatenated sequences of blocks to which the each vertex belongs to.
		 *   For each vertex the starting point of sequence of blocks is given by \a vmap in the VertData field firstBlock.
		 *  @return the number of biconnected components in the connected component given by vertex \a src.
		 *  \sa CompStore
		 *
		 *  [See example](examples/search/blocks/blocks.html).
		 */
		template< class GraphType, class VertDataMap, class EdgeDataMap, class CompIter, class VertIter,
			class VertBlockIter > static int splitComp( const GraphType &g, typename GraphType::PVertex src,
			VertDataMap &vmap, EdgeDataMap &emap, CompStore< CompIter,VertIter > out, VertBlockIter viter );

		// wyrzuca na iterator ciag wierzcholkow tworzacych rdzen grafu tj. podgraf pozostajacy po sukcesywnym
		// usuwaniu wierzcholkow stopnia < 2. Zwraca dlugosc sekwencji
		// TODO: przejsc na kopiec
		/** \brief Get core.
		 *
		 *  The method writes to \a out a sequence of vertex that make a core of graph i.e. remains after recursive deletions of vertices of deg < 2.
		 *  \param g the considered graph
		 *  \param out the iterator to the container with vertices of the core of graph.
		 *  \return the number of vertices in the core of graph.*/
		template< class GraphType,class Iterator > static int getCore( const GraphType &g, Iterator out );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings

	/** \brief Searching blocks = biconnected components (default).
	 *
	 *  The simpler default  version of BlocksPar in which DefaultStructs = AlgsDefaultSettings.
	 *  \ingroup search    */
	class Blocks: public BlocksPar< AlgsDefaultSettings > { };

	/** Algorithms for Eulerian cycle and path.
	 *
	 * \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  Warning: for graphs with overt 4000 edges you may need to increase the program stuck size.
	 *  \ingroup search */
	template< class DefaultStructs > class EulerPar: public PathStructs, protected SearchStructs
	{
	protected:
		template< class GraphType > struct EulerState
		{
			const GraphType &g;
			StackInterface< std::pair< typename GraphType::PVertex,typename GraphType::PEdge > *> stk;
			typename DefaultStructs::template AssocCont< typename GraphType::PEdge,bool >::Type edgeVisited;
			EdgeDirection mask;

			EulerState( const GraphType &_g, std::pair< typename GraphType::PVertex,typename GraphType::PEdge > *_stk,
				int nv, EdgeDirection m ): g( _g ), stk( _stk,nv ), edgeVisited( _g.getEdgeNo() ), mask( m )
				{ }
		};

		template< class GraphType > static void eulerEngine( typename GraphType::PVertex u,
			typename GraphType::PEdge ed, EulerState< GraphType > &state );

		template< class GraphType, class VertIter, class EdgeIter > static void
			eulerResult( EulerState< GraphType > &state, OutPath< VertIter,EdgeIter > &out );

		template< class GraphType > static  void
			_ends( const GraphType &g, EdgeType mask, typename GraphType::PVertex &,typename GraphType::PVertex &);

	public:
		// Uwaga: wersje bez dir uwzgledniaja jedynie podgraf zlozony z petli i krawedzi nieskierowanych ignorujac reszte
		// Wersje z dir uwzgledniaja jedynie podgraf zlozony z petli i lukow ignorujac reszte
		// para zawierajaca 2 razy ten sam wierzcholek - jesli graf ma nieskierowany cykl Eulera
		// para zawierajaca 2 rozne wierzcholki - konce nieskierowanej sciezki Eulera - jesli ta istnieje
		// (NULL,NULL) w przciwnym razie
		/** \brief Get Eulerian path end.
		 *
		 *  The method gets the ends of Eulerian path and returns it as standard pair (u,v). If there exists an Eulerian cycle u == v.
		 *  If the Eulerian path doesn't exist pair (NULL,NULL) is returned.\n
		 *  The method considered only undirected edges and loops, directed edges are ignored.
		 *  \param g the considered graph.
		 *  \return the standard pair of pointers to vertices that are the ends of the Euler path. If the Euler path does not exist the pair (NULL,NULL) is returned. */
		template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
			ends( const GraphType &g )
			{
				std::pair< typename GraphType::PVertex, typename GraphType::PVertex > res;
				_ends( g,EdUndir | EdLoop, res.first,res.second );
				return res;
			}

		// para zawierajaca 2 razy ten sam wierzcholek - jesli graf ma skierowany cykl Eulera
		// para zawierajaca 2 rozne wierzcholki - konce skierowanej sciezki Eulera - jesli ta istnieje
		// (NULL,NULL) w przciwnym razie
		/** \brief Get directed Eulerian path end.
		 *
		 *  The method gets the ends of an directed Eulerian path and returns it as standard pair (u,v). If there exists an Eulerian cycle u == v.
		 *  If the directed Eulerian path doesn't exist pair (NULL,NULL) is returned.
		 *  \param g the considered graph.
		 *  \return the standard pair of pointers to vertices that are the ends of the directed Euler path. If the Euler path does not exist the pair (NULL,NULL) is returned. */
		template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
			dirEnds( const GraphType &g )
			{
				std::pair< typename GraphType::PVertex, typename GraphType::PVertex > res;
				_ends( g,EdUndir | EdLoop, res.first,res.second );
				return res;
			}

		/** \brief Test if Eulerian.
		 *
		 *  The method tests if the graph has an undirected Eulerian cycle.
		 *  @param[in] g the tested graph.
		 *  @return true if it has Eulerian cycle, false otherwise.
		 *  \n
		 *
		 *  [See example](examples/search/euler/euler.html).
		 */
		template< class GraphType > static bool hasCycle( const GraphType &g );

		/** \brief Test if directed Eulerian.
		 *
		 *  The method tests if the graph has a directed Eulerian cycle.
		 *  @param[in] g the considered graph.
		 *  @return true if it has a directed Eulerian cycle, false otherwise.
		 *  \n
		 *
		 *  [See example](examples/search/euler/euler.html).
		 *
		 */
		template< class GraphType > static bool hasDirCycle( const GraphType &g );

		/** \brief Test if semi-Eulerian.

		* The method tests if the graph \a g has an undirected Eulerian path.
		* @param[in] g the considered graph.
		* @return true if it has an undirected Eulerian path, false otherwise
		* \n
		*
		* [See example](examples/search/euler/euler.html).
		*/
		template< class GraphType > static bool hasPath( const GraphType &g );

		/** \brief Test if directed semi-Eulerian.
		 *
		 *  The method tests if graph has a directed Eulerian path
		 *  @param[in] g graph
		 *  @return true if it has a directed Eulerian path, false otherwise.
		 *  \n
		 *
		 *  [See example](examples/search/euler/euler.html).
		 */
		template< class GraphType > static bool hasDirPath( const GraphType &g );

		/** \brief Test the beginning of  undirected Eulerian path.
		 *
		 *  The method tests if the graph \a g has an undirected Eulerian path starting at the vertex \a u.
		 *  @param[in] g the considered graph.
		 *  @param[in] u the starting vertex.
		 *  @return true if it has an undirected Eulerian path starting at the vertex \a u, false otherwise.*/
		template< class GraphType > static bool hasPath( const GraphType &g, typename GraphType::PVertex u );

		/** \brief Test the beginning of directed Eulerian path.
		 *
		 *  The method tests if the graph \a g has an directed Eulerian path starting at the vertex \a u.
		 *  @param[in] g the considered graph.
		 *  @param[in] u the starting vertex.
		 *  @return true if it has an directed Eulerian path starting at the vertex \a u, false otherwise */
		template< class GraphType > static bool hasDirPath( const GraphType &g, typename GraphType::PVertex u );

		/** \brief Test if Eulerian cycle containing \a u.
		 *
		 *  The method tests if the graph \a g has an undirected Eulerian cycle containing the vertex \a u.
		 *  @param[in] g the considered graph.
		 *  @param[in] u the given vertex.
		 *  @return true if it has an undirected Eulerian cycle containing the vertex \a u, false otherwise */
		template< class GraphType > static bool hasCycle( const GraphType &g, typename GraphType::PVertex u );

		/** \brief Test if directed Eulerian cycle containing \a u.
		 *
		 *  The method tests if the graph \a g has an directed Eulerian cycle containing the vertex \a u.
		 *  @param[in] g the considered graph.
		 *  @param[in] u the given vertex.
		 *  @return true if it has an directed Eulerian cycle containing the vertex \a u, false otherwise */
		template< class GraphType > static bool hasDirCycle( const GraphType &g, typename GraphType::PVertex u );

		/** \brief Get undirected Eulerian cycle
		 *
		 *  The method gets an undirected Eulerian cycle of the graph \a g.
		 *  @param[in] g the considered graph.
		 *  @param[out] out the OutPath object with found cycle.
		 *  @return true if the graph has an Eulerian cycle, false otherwise.
		 *  \sa SearchStructs::OutPath
		 *
		 *  [See example](examples/search/euler/euler.html).
		 */
		template< class GraphType, class VertIter, class EdgeIter >
			static bool getCycle( const GraphType &g, OutPath< VertIter,EdgeIter > out );

		/** \brief Get directed Eulerian cycle.
		 *
		 *  @param[in] g the considered graph
		 *  @param[out] out the OutPath object with found cycle.
		 *  @return true if the graph has an Eulerian cycle, false otherwise.
		 *  \sa SearchStructs::OutPath
		 *
		 *  [See example](examples/search/euler/euler.html).
		 */
		template< class GraphType, class VertIter, class EdgeIter >
			static bool getDirCycle( const GraphType &g, OutPath< VertIter,EdgeIter > out );

		/** \brief Get undirected Eulerian cycle.
		 *
		 *  @param[in] g the considered graph.
		 *  @param[in] prefstart the preferred starting vertex, but if the Eulerian cycle does not contain this vertex it is ignored.
		 *  @param[out] out the OutPath object with found cycle.
		 *  @return true if graph has an Eulerian cycle, false otherwise
		 *  \sa SearchStructs::OutPath */
		template< class GraphType, class VertIter, class EdgeIter > static bool
			getCycle( const GraphType &g, typename GraphType::PVertex prefstart, OutPath< VertIter,EdgeIter> out );

		/** \brief Get directed Eulerian cycle.
		 *
		 *  @param[in] g the considered graph.
		 *  @param[in] prefstart preferred starting vertex, but if the Eulerian cycle does not contain this vertex it is ignored.
		 *  @param[out] out the OutPath object with found cycle.
		 *  @return true if graph has an Eulerian cycle, false otherwise
		 *  \sa SearchStructs::OutPath */
		template< class GraphType, class VertIter, class EdgeIter > static bool getDirCycle( const GraphType &g,
			typename GraphType::PVertex prefstart, OutPath< VertIter,EdgeIter > out);

		/** \brief Get undirected Eulerian path.
		 *
		 *  @param[in] g the considered graph
		 *  @param[out] out the OutPath object with found path.
		 *  @return true if graph has an Eulerian path, false otherwise
		 *  \sa SearchStructs::OutPath
		 *
		 *  [See example](examples/search/euler/euler.html).
		 */
		template< class GraphType, class VertIter, class EdgeIter >
			static bool getPath( const GraphType &g, OutPath< VertIter,EdgeIter > out );

		/** \brief Get directed Eulerian path.
		 *
		 *  @param[in] g the considered graph.
		 *  @param[out] out the OutPath object with found path.
		 *  @return true if graph has an Eulerian path, false otherwise
		 *  \sa SearchStructs::OutPath
		 *
		 *  [See example](examples/search/euler/euler.html).
		 */
		template< class GraphType, class VertIter, class EdgeIter >
			static bool getDirPath( const GraphType &g, OutPath< VertIter,EdgeIter > out );

		/** \brief Get undirected Eulerian path.
		 *
		 *  @param[in] g the considered graph.
		 *  @param[in] prefstart preferred starting vertex, but if the Eulerian path does not contain this vertex it is ignored.
		 *  @param[out] out the OutPath object with found path.
		 *  @param[in] mask type of edges to consider.
		 *  @return true if graph has an Eulerian path, false otherwise.
		 *  \sa SearchStructs::OutPath */
		template< class GraphType, class VertIter, class EdgeIter > static bool getPath(
			const GraphType &g, typename GraphType::PVertex prefstart, OutPath<VertIter, EdgeIter> out );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings

	/** Algorithms for Eulerian cycle and path. (default)
	 *
	 *  The simpler default  version of EulerPar in which DefaultStructs = AlgsDefaultSettings.
	 *  Warning: for graphs with overt 4000 edges you may need to increase the program stuck size.
	 *  \ingroup search */
	class Euler: public EulerPar< AlgsDefaultSettings > { };

	// Typ najwyzszego wezla drzewa dekompozycji modulowej grafu
	// graf 1-wierzcholkowy
	// graf spojny o niespojnym dopelnieniu
	// graf niespojny
	// strong modules rozpinaja graf pierwszy
	enum ModPartType { mpTrivial,mpConnected,mpDisconnected,mpPrime };

	// Znajdowanie robicia grafow na maksymalne silne moduly
	/** \brief Maximal strong modules decomposition (parametrized).
	 *
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \ingroup search
	 */
	template< class DefaultStructs > class ModulesPar: public SearchStructs
	{
	public:
		// opis najwyzszego wezla drzewa dekompozycji modulowej grafu
		/** \brief The top node in modular decomposition tree.*/
		struct Partition
		{
			// size ile maksymalnych silnych modulow zawiera graf
			/**\brief the number of maximum strong modules in graph. */
			int size;
			// j.w.
			/** \brief Type of top node.
			 *
			 *  Possible values:
			 * - mpTrivial = 0 - only one vertex,
			 * - mpConnected = 1 - connected but with disconnected complement,
			 * - mpDisconnected = 2 - disconnected,
			 * - mpPrime = 3 - strong modules unfold prime graph.
			 */
			ModPartType type;

			Partition( int s, ModPartType t ): size( s ), type( t ) { }
		};

		// znajduje rozbicie grafu na maksymalne silne moduly
		// g, badany graf, powinien byc prosty, nieskierowany
		// out,  iteratory wyjsciowe z zawartoscia analogiczna jak w getComponents
		// tyle ze chodzi o rozbicie na moduly, a nie skladowe spojnosci
		// avmap, wyjsciowa tablica asocjacyjna PVertex->int do ktorej zapisuje sie numery modulow,
		// do ktorych naleza wierzcholki (lub BlackHole)
		// skipifprime, pomija badanie modulow jesli wynik ma type=mpPrime???
		/** \brief Get modular decomposithon of graph.
		 *
		 *  The method splits the vertices of \a g into maximal strong modules.
		 *  \param g the teste graph, should be simple and undirected.
		 *  \param[out] out an CompStore object storing the modular decomposition.
		 *  \param[out] avmap the associative table PVertex->int, where integers represent the index of module to which vertex belongs to. (BlackHole possible)
		 *  \param skipifprine if true the modules with the outcome type mpPrime are skipped.
		 *  \return a Partition object.
		 *  \n
		 *
		 *  [See example](examples/search/modules/modules.html).
		 */
		template< class GraphType, class CompIter, class VertIter, class CompMap > static Partition split(
			const GraphType &g, CompStore< CompIter,VertIter > out, CompMap &avmap, bool skipifprime = false );
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Maximal strong modules decomposition (default).
	 *
	 *  Version of ModulesPar with default settings.
	 *  \ingroup search
	 */
	class Modules: public ModulesPar< AlgsDefaultSettings > { };

#include "search.hpp"
}

#endif