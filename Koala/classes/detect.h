#ifndef KOALA_DETECTOR_H
#define KOALA_DETECTOR_H

#include "../container/simple.h"
#include "../graph/graph.h"
#include "../graph/view.h"
#include "../algorithm/search.h"
#include "../algorithm/weights.h"
#include "../algorithm/matching.h"
#include "../algorithm/conflow.h"

namespace Koala
{
	//Algorytmy rozpoznawania rodzin grafow. Dla rodziny family jest metoda rozpoznajaca bool family(const& graph).
	//Jesli family ma sens takze dla multigrafow, bool family(const& graph, bool allowmulti) gdzie flaga bool podaje
	//czy zezwalay na krawedzie rownolegle i petle. Jesli dla klasy family sa jakies szczegolne procedury (tylko dla grafow tego typu),
	//wprowadza sie dodatkowo podklase Family z metodami realizujacymi te funkcje. Generalnie nie maja one obowiazku
	// sprawdzania poprawnosci danych (tj. przynaleznosci grafu do tej rodziny), choc czasem tak czynia.

	/* IsItPar
	 * DefaultStructs - wytyczne dla wewnetrznych procedur
	 */
	/** \brief Test classes (parametrized).
	 *
	 *  The class delivers the range of methods which check if a graph belongs to various families of graphs. \n
	 *  By default it is assumed that considered graphs are simple. But there are some families of graphs which make sense for multigraps. Then the flag \a allowmulti is available. \n
	 *  In the class also some specialized algorithms which work only on certain families of graph are implemented.
	 *  \tparam DefaultStructs the class used to parametrize algorithms. It decides about basic algorithms and structs that are used by algorithms.
	 *  \ingroup detect
	 */
	template< class DefaultStructs > class IsItPar: public SearchStructs
	{
	public:
		// Generalnie zaklada sie, ze poprawny (dla IsItPar) graf wejsciowy nie ma petli ani lukow oraz n>0
		// Ta metoda jest wyjatkiem, sprawdza czy n=0
		/** \brief Test if empty vertex set.
		 *
		 *  The method tests if the graph has the empty vertex set.
		 *  \param g the considered graph.
		 *  \return true if the graph has an empty vertex set, otherwise false. */
		template< class GraphType > static bool zero( const GraphType &g )
			{ return !g.getVertNo(); }

		// czy graf jest prosty (tj. nie ma krawedzi rownoleglych)
		/** \brief Test if simple/multigraph.
		 *
		 *  The method tests if a graph has only unique undirected edges (simple graph). Or if \a allowmulti is true, the method tests if the graph is an undirected multigraph.
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if \a g is a simple graph (allowmulti false) or multigraph (allowmulti true), otherwise false. */
		template< class GraphType > static bool undir( const GraphType &g, bool allowmulti = false );

		// czy spojny
		/** \brief Test if connected
		 *
		 *  The method tests if the graph \a g is connected i.e. for each pair of vertices there exists a path between the vertices.
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if connected,  otherwise false. */
		template< class GraphType > static bool connected( const GraphType &g, bool allowmulti = false );

		// czy bezkrawedziowy
		/** \brief Test if empty.
		 *
		 *  The method tests if the graph has no edges.
		 *  \param g the considered graph.
		 *  \return true if there are no edges,  otherwise false. */
		template< class GraphType > static bool empty( const GraphType &g )
			{ return g.getVertNo() && !g.getEdgeNo(); }

		// drzewo
		/** \brief Test if tree.
		 *
		 *  The method tests if the graph \a g is a tree i.e. if the graph is connected and the number of vertices equals  the number of edges plus one.
		 *  \param g the considered graph.
		 *  \return true if tree, false otherwise. */
		template< class GraphType > static bool tree( const GraphType &g )
			{ return connected( g,true ) && (g.getVertNo() - 1 == g.getEdgeNo()); }

		// las
		/** \brief Test if forest.
		 *
		 *  The method tests if the graph \a g is a forest, in other words, is it a set of trees.
		 *  \param g the considered graph.
		 *  \return true if the graph is a forest, false otherwise. */
		template< class GraphType > static bool forest( const GraphType &g )
			{ return undir( g,true ) && !BFSPar< DefaultStructs >::cyclNo( g ); }

		// klika
		/** \brief Test if clique.
		 *
		 *  The method tests if the graph \a g is a clique. In other words, each pair of the set of vertices  forms an edge.
		 *  \param g the considered graph.
		 *  \return true if the graph is a clique, false otherwise. */
		template< class GraphType > static bool clique( const GraphType &g );

		// suma rozlacznych klik
		/** \brief Test if union of disjoint cliques.
		 *
		 *  The method tests if the graph is a union of disjoint cliques.
		 *  \param g the considered graph.
		 *  \return true if the graph \a g is a union of disjoint cliques, false otherwise. */
		template< class GraphType > static bool cliques( const GraphType &g );

		// regularny
		/** \brief Test if regular.
		 *
		 *  The method tests if each vertex of the graph has the same degree.
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if regular, otherwise false. */
		template< class GraphType > static bool regular( const GraphType &g, bool allowmulti = false );

		/* Path
		 *
		 */
		/** \brief Methods for paths. */
		class Path
		{
		public:
			// Konce podanej sciezki, (NULL,NULL) w razie bledu
			/** \brief Get ends.
			 *
			 *  The method gets the ends of the path.
			 *  \param g the considered graph.
			 *  \return the standard pair of vertices, which are the ends of path. If any error occur pair (NULL,NULL) is returnd.*/
			template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
				ends( const GraphType &g );
		};

		// sciezka
		/** \brief Test if path.
		 *
		 *  The method tests if the graph \a g is a path.
		 *  \param g the considered graph.
		 *  \return true if the graph is a path, otherwise false. */
		template< class GraphType > static bool path( const GraphType &g )
			{ return Path::ends( g ).first; }

		/* Caterpillar
		 *
		 */
		/** \brief Methods for caterpillars. */
		class Caterpillar
		{
		public:
			// Konce grzbietu gasienicy, (NULL,NULL) w razie bledu
			/** \brief Get central path ends.
			 *
			 *  The method gets the ends of the central path of the caterpillar \a g.
			 *  \param g the considered graph.
			 *  \return the standard pair of vertices, which are the ends of the central path. If any error occur, a pair (NULL,NULL) is returnd.*/
			template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
				spineEnds( const GraphType &g );
		};

		// gasienica
		/** \brief Test if caterpillar.
		 *
		 *  The method tests if the graph \a g is a caterpillar.
		 *  \param g the tested graph.
		 *  \return true if caterpillar, otherwise false. */
		template< class GraphType > static bool caterpillar( const GraphType &g )
			{ return Caterpillar::spineEnds( g ).first; }

		// cykl,
		/** \brief Test if cycle.
		 *
		 *  The method tests if the considered graph \a g is a cycle.
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if the graph \a g is a cycle, false  otherwise. */
		template< class GraphType > static bool cycle( const GraphType &g, bool allowmulti = false )
			{ return connected( g,allowmulti ) && g.deg( g.getVert() ) == 2 && regular( g,true ); }

		// zbior niezalezny krawedzi
		/** \brief Test if matching.
		 *
		 *  The method tests if the considered graph \a g is a matching (is of degree 1).
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if the graph \a g is a matching, false  otherwise. */
		template< class GraphType > static bool matching( const GraphType &g )
			{ return undir( g,true ) && g.Delta() <= 1; }

		// podkubiczny
		/** \brief Test if subcubic.
		 *
		 *  The method tests if the considered graph \a g is subcubic (the maximum degree is not grater then three).
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if the graph \a g is subcubic, false  otherwise. */
		template< class GraphType > static bool subcubic( const GraphType &g, bool allowmulti = false )
			{ return undir( g,allowmulti ) && g.Delta() <= 3; }

		// kubiczny
		/** \brief Test if cubic.
		 *
		 *  The method tests if the considered graph \a g is cubic (the degree of each vertex is exactly three).
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if the graph \a g is cubic, false  otherwise. */
		template< class GraphType > static bool cubic( const GraphType &g, bool allowmulti = false )
			{ return g.getVert() && g.deg( g.getVert() ) == 3 && regular( g,allowmulti ); }

		// graf o wszystkich skladowych 2-spojnych bedacych klikami
		/** \brief Test if block graph.
		 *
		 *  The method tests if every biconnected component of the graph \a g is a clique.
		 *  \param g the considered graph.
		 *  \return true if the graph \a g is a block graph, false  otherwise. */
		template< class GraphType > static bool block( const GraphType &g );

		// maks. liczba cyklomatyczna skladowej 2-spojnej grafu prostego. -1 w razie bledu.
		/** \brief Test if almost K-tree.
		 *
		 *  The method checks for which smallest  \a K a graph is an almost K-tree.
		 *  \param g the considered graph.
		 *  \return the smallest \a K for which  the graph \a g is an almost K-tree.  If any error occurs -1 is returned.*/
		template< class GraphType > static int almostTree( const GraphType &g );

		/* Bipartite
		 *
		 */
		/** \brief Methods for bipartite graphs. */
		class Bipartite
		{
		public:
			// wypisuje na iterator wierzcholki jednej partycji grafu dwudzielnego. Zwraca licznosc partycji (-1 w razie bledu)
			/** \brief Get partition.
			 *
			 *  The method gets a partition of \a g  and stores up in the container represented by iterator \a out.
			 *  \param g the considered graph.
			 *  \param out the iterator of the container keeping the output partition.
			 *  \param allowmulti the flag allowing or disabling multiple edges.
			 *  \return the number of vertices in set \a out or if any error occur -1.*/
			template< class GraphType, class Iter >
				static int getPart( const GraphType &g, Iter out, bool allowmulti = false );

			// znajduje najwiekszy zbior niezalezny, zwraca jego rozmiar
			/** \brief Get maximal independent set.
			 *
			 *  The method gets the maximal independent (stable) set of the graph \a g.
			 *  \param g the considered graph.
			 *  \param out the iterator of the container keeping the output set (partition).
			 *  \return the number of vertices in the maximal stable set kept in the container \a out.*/ 
			template< class GraphType, class Iter > static int maxStable( const GraphType &g, Iter out );

			// znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
			/** \brief Get minimal vertex cover.
			 *
			 *  The method gets the minimal vertex set such that each edge of the graph is incident to at least one vertex in the set.
			 *  \param g the considered graph.
			 *  \param out the iterator of the container keeping the output minimal vertex cover.
			 *  \return the number of vertices in the output set.*/ 
			template< class GraphType, class Iter > static int minVertCover( const GraphType &g, Iter out );

		};

		// czy dwudzielny
		/** \brief Test if bipartite.
		 *
		 *  The method tests if vertices can be partitioned into two sets such that vertices in each set are independent.
		 *  \param g the considered graph.
		 *  \param allowmulti the flag allowing or disabling multiple edges.
		 *  \return true if the graph \a g is bipartite, false  otherwise. */
		template< class GraphType > static bool bipartite( const GraphType &g, bool allowmulti = false )
			{ return Bipartite::getPart( g,blackHole,allowmulti ) != -1; }

		/* CompBipartite
		 *
		 */
		/** \brief Methods for complete bipartite graphs. */
		class CompBipartite
		{
		public:
			// wypisuje na iterator wierzcholki jednej partycji grafu pelnego dwudzielnego. Zwraca licznosc partycji (-1 w razie bledu)
			/** \brief Get partition.
			 *
			 *  The method gets the set of vertices that make a partition of \a g.
			 *  \param g the considered graph.
			 *  \param out the iterator of a container with all the vertices of the partition.
			 *  \return the number of element in the output set \a out.*/
			template< class GraphType, class Iter > static int getPart( const GraphType &g, Iter out );
		};

		// czy pelny dwudzielny
		/** \brief Test if complete bipartite graph.
		 *
		 *  The method tests if the graph is a complete bipartite graph.
		 *  \param g the tested graph.
		 *  \return true if the graph is a complete bipartite graph, false otherwise. */
		template< class GraphType > static bool compBipartite( const GraphType &g )
			{ return CompBipartite::getPart( g,blackHole ) != -1; }

		/* CompMPartite
		 *
		 */
		/** \brief Methods for complete M-partite graph. */
		class CompMPartite
		{
		public:
			// wyrzuca na out ciagi wierzcholkow tworzacych partycje grafu pelnego M-dzielnego. Zwraca liczbe partycji M lub -1 w razie bledu
			//TODO: argument w postaci kontenera wyjsciowego PVertex->int (tj. numer partycji)
			/** \brief Get partitions.
			 *
			 *  The method gets all the partitions of graph and stores it up in the CompStore \a out.
			 *  \param g the considered graph.
			 *  \param[out] out the CompStore iterator of a container with the partitions.
			 *  \return the number of partitions or -1 if any error occur.
			 *  \sa CompStore*/
			template< class GraphType, class Iter, class VIter >
				static int split( const GraphType &g, CompStore< Iter,VIter > out );
		};

		// czy pelny M-dzielny dla pewnego M>0
		/** \brief Test if complete M-partite.
		 *
		 *  The method tests if the graph is a complete M-partite graph.
		 *  \param g the considered graph.
		 *  \return true if the graph \a g is a complete M-partite, false  otherwise. */
		template< class GraphType > static bool compMPartite( const GraphType &g )
			{ return CompMPartite::split( g,compStore( blackHole,blackHole )) != -1; }

		protected:

		  template <class Cont>
			struct AssocArrSwitch {

				typedef Cont Type;
			};

			template <class K, class E, class Cont>
			struct AssocArrSwitch<AssocArray<K,E,Cont> > {

				typedef AssocTable<std::map< K, E> > Type;
			};

		public:

		/* Chordal
		 * M. Habib, R. McConnel, C. Paul, L.Viennot
		 * Lex-BFS and Partition Refinement, with Applications to Transitive
		 * Orientation, Interval Graph Recognition and Consecutive Ones Testing
		 * Obsluga chordali
		 */
		/** \brief Methods for chordal graphs.
		 *
		 *  The algorithms are based on the article: M. Habib, R. McConnel, C. Paul, L.Viennot
		 *  Lex-BFS and Partition Refinement, with Applications to Transitive
		 *  Orientation, Interval Graph Recognition and Consecutive Ones Testing*/
		class Chordal
		{
		protected:
			static void RadixSort( std::pair< int,int > *src, int size, int n, int *hints, int *out );

			// porz¹dkuje wêz³y drzewa zdefiniowanego przez relacjê parent
			// wszyscy potomkowie p wyst¹pi¹ przed wyst¹pieniem p, ale
			// nie koniecznie bêdzie to kolejnoœæ wygenerowana przez DFSa na
			// drzewie, np. dla:
			//     D
			//    / \
			//   C   E
			//  / \
			// A   B
			// DFSPostorder da: A B C E D
			// a poni¿sza funkcja mo¿e zwróciæ np.: A E B C D

			static void SemiPostOrderTree( int *parent, int n, int *out );

			template< class Graph > struct QTRes
			{
				int size;
				Set< typename Graph::PVertex > trees;
				QTRes(): size( 0 ), trees()
					{ }
			};

		public:

			// wyrzuca na iterator odwrotny perf. ellimination order chordal grafu tj. porzadek doklejania nowych wierzcholkow za podkliki
			// false gdy graf nie byl chordal
			/** \brief Get perfect elimination order.
			 *
			 *  The method gets the reversed perfect elimination order of \a g, which exist if and only if \a g is chordal. That is why the method returns false if the graph is not chordal.
			 *  \param g the considered graph.
			 *  \param out the iterator of a container with the reversed perfect elimination order.
			 *  \return true if sucesfull, false if \a g is not a chordal graph.*/
			template< class Graph, class VIter2 > static bool getOrder( const Graph &g, VIter2 riter );

			// Tworzy reprezentacje chordala w postaci drzewa jego maksymalnych klik
			// zwraca liczbe maksymalnych klik
			/** \brief Get maximum cliques tree representation.
			 *
			 *  The method gets the maximum cliques tree representation basing on the perfect elimination order of \a g.
			 *  \param g the considered graph.
			 *  \param begin the iterator to the first element of the container with prefect elimination order of \a g.
			 *  \param end the iterator to the last element of the container with prefect elimination order of \a g.
			 *  \param[out] out the CompStore object that keeps maximal cliques.
			 *  \param[out] qte the iterator of the container with pairs of integers. The pair (\a i , \a j ) stands for the connection between \a i-th and \a j-th clique. \a i and \a j are the positions of clique in  \a out.
			 *  \return the number of cliques. */
			template< class Graph, class VIter, class VIterOut, class QIter, class QTEIter >
				static int maxCliques( const Graph &g, VIter begin, VIter end, CompStore< QIter,VIterOut > out, QTEIter qte );

			// j.w. ale samodzielna. Pobiera tylko graf, jesli nie byl chordal, zwraca -1
			/** \brief Get maximum cliques tree representation.
			 *
			 *  The method gets the maximum cliques tree representation.
			 *  \param g the considered graph.
			 *  \param[out] out the CompStore object that keeps maximal cliques.
			 *  \param[out] qte the iterator of the container with pairs of integers. The pair (\a i , \a j ) stands for the connection between \a i-th and \a j-th clique. \a i and \a j are the positions of clique in  \a out.
			 *  \return the number of cliques.*/
			template< class Graph, class VIterOut, class QIter, class QTEIter >
				static int maxCliques( const Graph &g, CompStore< QIter,VIterOut > out, QTEIter qte );

			// znajduje najwieksza klike w grafie, zwraca jej rozmiar
			/** \brief Get maximal clique.
			 *
			 *  The method gets the maximal clique of \a g basing on the perfect elimination order.
			 *  \param g the considered graph.
			 *  \param begin the iterator to the first element of the container with prefect elimination order.
			 *  \param end the iterator to the last element of the container with prefect elimination order.
			 *  \param out the iterator of a container with all the vertices of clique.
			 *  \return the maximal clique size.*/
			template< class Graph, class VIter, class VIterOut >
				static int maxClique( const Graph &g, VIter begin, VIter end, VIterOut out );

			// j.w. ale samodzielna. Pobiera tylko graf, jesli nie byl chordal, zwraca -1
			/** \brief Get maximal clique.
			 *
			 *  The method gets the maximal clique of \a g.
			 *  \param g the considered graph.
			 *  \param out the iterator of a container with all the vertices of the clique.
			 *  \return the maximal clique size.*/
			template< class Graph, class VIterOut > static int maxClique( const Graph &g, VIterOut out );

			// znajduje najwiekszy zbior niezalezny (wypuszczany na out), zwraca jego rozmiar
			// korzysta z rozkladu na drzewo klik por. maxCliques
			/** \brief Get maximal stable set.
			 *
			 *  The method gets the set of vertices that make a maximal stable (independent) set of \a g.
			 *  The method uses the maximum clique tree representation.
			 *  \param g the considered graph.
			 *  \param gn the number of cliques in the clique tree representation.
			 *  \param begin the iterator of the container with the positions of first elements of cliques in the concatenated sequences kept in \a vbegin.
			 *  \param vbegin the iterator of the container with the concatenated sequence of vertices that are the maximum clique representation. \a vbegin together with \a begin make the maximal clique representation.
			 *  \param ebegin iterator of container with pairs of integers (i,j) which represent the connection between i-th and j-th clique in the clique tree representation.
			 *  \param out the iterator of a container with all the vertices of output the stable set.
			 *  \return the number of element in the output set \a out.*/
			template< class Graph, class QIter, class VIter, class QTEIter, class IterOut >
				static int maxStable( const Graph &g, int qn, QIter begin, VIter vbegin, QTEIter ebegin, IterOut out );

			// znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
			// sens parametrow j.w.
			/** \brief Get minimal vertex cover.
			 *
			 *  The method gets the set of vertices that make a minimal vertex cover of \a g.
			 *  \param g the considered graph.
			 *  \param gn the number of cliques in the clique tree representation.
			 *  \param begin the iterator of the container with the positions of first elements of cliques in the concatenated sequences kept in \a vbegin.
			 *  \param vbegin the iterator of the container with the concatenated sequence of vertices that are the maximum clique representation.  \a vbegin together with \a begin make the maximal clique representation.
			 *  \param ebegin iterator of container with pairs of integers (i,j) which represent the connection between i-th and j-th clique in the clique tree representation.
			 *  \param out the iterator of a container with all the vertices of the cover.
			 *  \return the number of element in the output set \a out.*/
			template< class Graph, class QIter, class VIter, class QTEIter, class IterOut >
				static int minVertCover( const Graph &g, int qn, QIter begin, VIter vbegin, QTEIter ebegin, IterOut out );

			// znajduje najwiekszy zbior niezalezny (wypuszczany na out), zwraca jego rozmiar lub -1 w razie bledu
			// samodzielna
			/** \brief Get  maximal stable set.
			 *
			 *  The method gets the set of vertices that make a maximal stable (independent set  of \a g.
			 *  \param g the considered graph.
			 *  \param out the iterator of the output container with all the vertices of the stable set.
			 *  \return the number of element in the output set \a out or -1 if any error occur.*/
			template< class Graph, class IterOut > static int maxStable( const Graph &g, IterOut out );

			// znajduje najwieksze pokrycie wierzcholkowe (wypuszczane na out), zwraca jego rozmiar lub -1 w razie bledu
			// samodzielna
			/** \brief Get minimal vertex cover.
			 *
			 *  The method gets the set of vertices that make a minimal vertex cover of \a g.
			 *  \param g the considered graph.
			 *  \param out the iterator of a container with all the vertices of the minimal vertex cover.
			 *  \return the number of element in the output set \a out.*/
			template< class Graph, class IterOut > static int minVertCover( const Graph &g, IterOut out );
		};

		/** \brief Test if chordal.
		 *
		 *  The method tests if the graph is chordal.
		 *  @param[in] g the graph to test.
		 *  @return true if the graph is chordal, false otherwise. */
		template< class GraphType > static bool chordal( const GraphType &g )
		{ return Chordal::getOrder( g,blackHole ); }

		// czy graf jest dopelnieniem chordala
		/** \brief Test if chordal graph complement.
		 *
		 *  The method tests if \a g is a complement of a chordal graph.
		 *  \param g the considered graph.
		 *  \return true if the graph is a complement of a chordal graph, false otherwise.*/
		template< class GraphType > static bool cochordal( const GraphType &g );

		// czy splitgraph
		/** \brief Test if split graph.
		 *
		 *  The method tests if the graph is a split graph.
		 *  \param g the considered graph.
		 *  \return true if the graph \a g is a split graph, false  otherwise.*/
		template< class GraphType > static bool split( const GraphType &g )
		{ return chordal( g ) && cochordal( g ); }

		/* Comparability
		 *
		 */
		/** \brief Methods for comparability graphs*/
		class Comparability
		{
		protected:

			struct EDir: public std::pair< int,int >
			{
				EDir(): std::pair< int,int >( 0,0 )
					{ }

				int& operator()( EdgeDirection arg )
				{
					switch (arg)
					{
						case EdDirIn: return first;
						case EdDirOut: return second;
						default: assert( 0 ); return first;
					}
				}
			};

			class FlowDefaultStructs: public DefaultStructs
			{
			public:
				enum { useFulkersonFord = false };
			};


		public:
			/* M.C. Golumbic
			 * The Complexity of Comparability Graph Recognition and Coloring
			 * Computing 18, 199-208 (1977)
			 */
			// Kompleksowa obsluga comparability grafu
			// zwraca liczbe chromatyczna lub -1 jesli graf nie byl comparability
			// dirmap - wysciowa tablica asocjacyjna PVertex->EdgeDirection z przykladowa comparability orientation krawedzi
			// (kierunek krawedzi miedzy getEdgeEnd1 a getEdgeEnd2). Lub BlackHole.
			// aheightmap- wysciowa tablica asocjacyjna PVertex->int z optymalnym pokolorowaniem wierzcholkowym. Lub BlackHole.
			// cliqueiter - iterator wyjsciowy, na ktory zostaje zapisana najwieksza klika
			/** \brief Comprehensive service for comparability graphs.
			 *
			 *
			 *  The method bases on <em> M.C. Golumbic, The Complexity of Comparability Graph Recognition and Coloring Computing 18, 199-208 (1977)\n
			 *  It finds and exemplary orientation of edges, optimal coloring and maximal clique.
			 *  \param g the considered graph.
			 *  \param[out] dirmap an associative container (PEdge  -> EdgeDirection) which determines an exemplary partial order.
			 *  \param[out] aheightmap an associative container (PVertex -> int) with an exemplary optimal coloring.
			 *  \param[out] cliqueiter the iterator of the container with the vertices of the maximal clique.
			 *  \return the chromatic number of \a g or -1 if graph was not comparability.*/
			template< class Graph, class DirMap, class OutMap, class OutIter >
				static int explore( const Graph &g, DirMap &dirmap, OutMap &aheightmap, OutIter cliqueiter );

			// sprawdza, czy graf byl comparability
			// adirmap - wysciowa tablica asocjacyjna PVertex->EdgeDirection z przykladowa comparability orientation krawedzi
			// (kierunek krawedzi miedzy getEdgeEnd1 a getEdgeEnd2). Lub BlackHole
			/** \brief Test if comparability.
			 *
			 *  The method tests if the graph \a g is a comparability graph and finds the orientation of edges.
			 *  \param g the considered graph.
			 *  \param[out] dirmap an associative container (PEdge  -> EdgeDirection) which determines an exemplary partial order.
			 *  \return true if \a g is a comparability graph, false otherwise.*/
			template< class Graph, class DirMap > static bool getDirs( const Graph &g, DirMap& adirmap )
				{ return explore( g,adirmap,blackHole,blackHole ) != -1; }

			// sprawdza, czy graf byl comparability. Jesli tak, nadaje krawedziom wlasciwa orientacje
			/** \brief Convert to partial order.
			 *
			 *  The method tests if the graph \a g is a comparability graph. If positive, converts all the edges to arcs that represent an exemplary partial order.
			 *  \param g the considered graph.
			 *  \return true if \a g is a comparability graph, false otherwise and the graph remains unchanged. */
			template< class Graph > static bool getDirs( Graph &g );

			// zwraca liczbe chromatyczna lub -1 jesli graf nie byl comparability
			// avmap - wysciowa tablica asocjacyjna PVertex->int z optymalnym pokolorowaniem wierzcholkowym. Lub BlackHole.
			/** \brief Get optimal coloring.
			 *
			 *  The method gets an optimal vertex coloring of \a g.
			 *  \param g the considered graph.
			 *  \param[out] avmap the associative container (PVert -> int) with the optimal coloring of \a g.
			 *  \return the chromatic number of \g or -1 if \a g is not a comparability graph.*/
			template< class Graph, class OutMap > static int color( const Graph &g, OutMap &avmap )
				{ return explore( g,blackHole,avmap,blackHole ); }

			/** \brief Get maximal clique.
			 *  The method finds a largest clique in a comparability graph
			 *  @param[in] g the considered graph
			 *  @param[out] iter the iterator to write the vertices of clique or BlackHole.
			 *  @return the number of vertices in the maximal clique */
			template< class Graph, class OutIter > static int maxClique( const Graph &g, OutIter iter )
				{ return explore( g,blackHole,blackHole,iter ); }

			// znajduje najwiekszy zbior niezalezny, zwraca jego rozmiar
			/** \brief Get maximal stable (independent) set.
			 *
			 *  The method gets the set of vertices that make a stable set of \a g.
			 *  \param g the considered graph.
			 *  \param[out] out the iterator of a container with all the vertices of the stable set.
			 *  \return the cardinality of stable set i.e. the number of element in the output set \a out.*/
			template< class GraphType, class VIterOut > static int maxStable( const GraphType &g, VIterOut out );

			// znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
			/** \brief Get minimal vertex cover.
			 *
			 *  The method gets the set of vertices that make a minimal vertex cover of \a g.
			 *  \param g the considered graph.
			 *  \param[out] out the iterator of a container with all the vertices of the cover.
			 *  \return the size of minimal vertex cover i.e. the number of element in the output set \a out.*/
			template< class GraphType, class Iter > static int minVertCover( const GraphType &g, Iter out );
		};

		/** \brief Test if comparability graph.
		 *
		 *  @param[in] g the graph to test.
		 *  @return true if the graph is a comparability graph, false otherwise. */
		template< class GraphType > static bool comparability( const GraphType &g )
			{ return Comparability::explore( g,blackHole,blackHole,blackHole ) != -1; }

		// czy graf jest dopelnieniem comparability
		/** \brief Test if complement of comparability graph.
		 *
		 *  \param g the considered graph.
		 *  \return true if the graph \a g is a comparability graph complement,  false  otherwise.*/
		template< class GraphType > static bool cocomparability( const GraphType &g );

		/* Interval
		 *
		 */
		/** \brief Methods for interval graphs.*/
		class Interval: protected LexBFSPar< DefaultStructs >
		{
		public:
			// Struktura reprezentujaca przedzial domkniety na prostej o koncach calkowitych (dopuszczalna dlugosc 0)
			/** \brief Line segment.
			 *
			 *  Structure represents a closed line segment between two integer points.
			 */
			struct Segment
			{
				int left, right;
				Segment( int l = 0, int r = 1 ): left( l ), right( r )
				{ }
			};

			// czy dwa takie przedzialy tna sie niepusto
			/** \brief Test line segments intersection.
			 *
			 *  \param a the first line segment.
			 *  \param b the second line segment.
			 *  \return true if the line segments \a a and \a b intersect, false otherwise.*/
			static bool touch( Segment a, Segment b )
				{ return std::max( a.left,b.left ) <= std::min( a.right,b.right ); }

			// konwersja zbior przedzialow-> interval graph
			// pobiera spomiedzy 2 iteratorow ciag przedzialow (struktur typu segment)
			// dopisuje do danego grafu odpowiadajacy mu graf przeciec (zwraca pierwszy stworzony wierzcholek)
			// pola info wierzcholkow i krawedzi tworzy funktorami vinfo(int), einfo(int,int) - argumenty to
			// numery przedzialow w ciagu wejsciowym
			// na iterator out zwraca sekwencje stworzonych wierzcholkow zgodna z cagiem przedzialow
			/** \brief Convert intervals to graph.
			 *
			 *  The method add to \a g an interval graph generated from the set of line segments kept in the container represented by iterators \a begin and \a end.
			 *  \param g[out] the target graph.
			 *  \param begin the iterator to the first element of the container with the line segments set.
			 *  \param end the second iterator of the line segments set.
			 *  \param out[out] the iterator of a container with all the new-created vertices (the same order as the line segments).
			 *  \param vinfo the function object automatically generating the info for vertex. Should allow to call vinfo(int), where the parameter are the position of the line segment.
			 *  \param einfo the function object automatically generating the info for edge. Should allow to call vinfo(int, int), where the integers are the position of the corresponding  line segments.
			 *  \return the first new-created vertex.*/
			template< class GraphType, class Iter, class IterOut, class VInfoGen, class EInfoGen >
				static typename GraphType::PVertex segs2graph( GraphType &g, Iter begin, Iter end, IterOut out,
					VInfoGen vinfo, EInfoGen einfo );
			// j.w. ale polom info nadawane sa wartosci domyslne
			/** \brief Convert intervals to graph.
			 *
			 *  The method add to \a g an interval graph generated from the set of line segments kept in the container  represented by the iterators \a begin and \a end.
			 *  \param[out] g the target graph.
			 *  \param begin the iterator to the first element of the container with the line segments set.
			 *  \param end the second iterator of the line segments set.
			 *  \param[out] out the iterator of a container with all the new-created vertices (the same order as the line segments).
			 *  \return the first new-created vertex.*/
			template< class GraphType, class Iter, class IterOut >
				static typename GraphType::PVertex segs2graph( GraphType &g, Iter begin, Iter end, IterOut out );

			/** \brief Convert interval graph to its interval representation.
			 *
			 *  @param g graph
			 *  @param[out] outmap map (PVertex -> Segment) or BlackHole (if redundant).
			 *  @return true if \a g is interval, false otherwise */
			template< class GraphType, class IntMap > static bool graph2segs( const GraphType &g, IntMap &outmap );

		protected:
			struct Sets
			{
				struct Elem
				{
					int value;
					Privates::List< Elem,Privates::BlockListAllocator< Privates::ListNode< Elem > > > *cont;
					Privates::List_iterator< Elem > next;
					Elem( int _v = -1 ): value( _v ), cont( NULL ), next()
					{ }
				};
				typedef Privates::List< Elem,Privates::BlockListAllocator< Privates::ListNode< Elem > > > Entry;

				Sets( std::pair< Entry,Privates::List_iterator< Elem> > *data, size_t n,
					Privates::BlockListAllocator< Privates::ListNode< Elem > > &a );

				// dodaje trg do zbioru id
				void add( int id, int trg );
				// usuwa id ze wszystkich zbiorów
				void remove( int id );
				// iloœæ elementów w zbiorze id
				int count( int id )
					{ return m_data[id].first.size(); }
				// czy zbiór id pusty
				bool empty( int id )
					{ return m_data[id].first.empty(); };
				// pierwszy element ze zbioru id (w praktyce -- dowolny)
				int first( int id )
					{ return m_data[id].first.front().value; }

				std::pair< Entry,Privates::List_iterator< Elem > > *m_data;
			};

			struct IvData
			{
				unsigned int posSigmap;
				unsigned int ip;
				unsigned int posSigmapp;
				unsigned int ipp;
				unsigned int posSigma;
			};

			template< class GraphType, class Map > static void CalculateI( const GraphType &g,
				typename GraphType::PVertex *order, Map &data, unsigned int IvData::*pos, unsigned int IvData::*ifn );

			// BuildSet
			template< class GraphType, class Map, class SetsType > static void BuildSet( const GraphType &g,
				SetsType &sets, Map &data, unsigned int IvData::*order, unsigned int IvData::*ifn );

			template< class GraphType > struct OrderData
			{
				typename GraphType::PVertex v;
				// kogo jest s¹siadem (numer s¹siada w porz¹dku)
				int vertId;
				// numer w porz¹dku
				int orderId;
			};

			struct LBSData
			{
				int visiteda;
				int visitedb;
				int aOrder;
				int bOrder;
			};

			// LBFSStar
			template< class GraphType, class MapType, class SetsType, class OutIter, class Alloc1, class Alloc2 >
				static void LBFSStar( const GraphType &g, SetsType &A, SetsType &B, MapType &data,
					typename GraphType::PVertex *sigmap, typename GraphType::PVertex *sigmapp, OutIter out,
					Alloc1& allocat,Alloc2& allocat2 );

			// UmbrellaTestVertex
			template< class GraphType, class Map >
				static bool UmbrellaTestVertex( const GraphType &g, typename GraphType::PVertex u, Map &data,
					typename GraphType::PVertex *order );

			// IsUmbrellaFree
			template< class GraphType, class Map >
				static bool IsUmbrellaFree( const GraphType &g, Map &data, typename GraphType::PVertex *order );
			// reverse
			template< class T > static void reverse( T *tab, size_t n );
		};

		/** \brief Test if interval graph
		 *
		 *  The method tests if the graph \a g is an interval graph.
		 *  @param[in] g graph
		 *  @return true if \a g is interval, false otherwise. */
		template< class GraphType > static bool interval( const GraphType &g )
		//     { return chordal( g ) && cocomparability( g ); }
			{ return Interval::graph2segs(g, blackHole); }

		// czy pierwszy
		/** \brief Test if prime graph.
		 *
		 *  The method test if the graph \a g is a prime graph.
		 *  \param g the tested graph.
		 *  \return true if \a g is prime, false otherwise .*/
		template< class GraphType > static bool prime( const GraphType &g );

		/* Cograph
		*
		*/
		/** \brief Methods for cographs.*/
		class Cograph
		{
			template< class Defs > friend class IsItPar;

		public:
			/** \brief Get the maximal clique.
			 *
			 *  The method gets the maximal clique of \a g and writes  it (the vertices) down to the container represented by the iterator \a out.
			 *  \param g the considered graph.
			 *  \param out the iterator of a container with all the vertices of the clique.
			 *  \return the number of element in the output set \a out.*/
			template< class GraphType, class VIterOut > static int maxClique( const GraphType &g, VIterOut out );
			/** \brief Get the maximal stable set.
			 *
			 *  The method gets the maximal stable (independent) set of \a g.
			 *  \param g the considered graph.
			 *  \param out the iterator of the output container with all the vertices of the stable set.
			 *  \return the number of element in the output set \a out i.e. the cardinality of the stable set of \a g.*/
			template< class GraphType, class VIterOut > static int maxStable( const GraphType &g, VIterOut out );
			// znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
			/** \brief Get the minimal vertex cover.
			 *
			 *  The method gets the set of vertices that make a vertex cover of \a g.
			 *  \param g the considered graph.
			 *  \param out the iterator of the output container with all the vertices of the vertex cover.
			 *  \return the number of element in the output set \a out i.e. the cardinality of the vertex cover of \a g.*/
			template< class GraphType, class Iter > static int minVertCover( const GraphType &g, Iter out );

	//        TODO: template<class Graph, class AssocTab>
	//        static int color(const Graph &g,AssocTab out);

		protected:
			template< class GraphType, class Assoc > static bool cograph( const GraphType &ag, Assoc &subset );
			template< class GraphType, class Assoc, class Iter >
				static int maxClique2( const GraphType &ag, Assoc &subset, Iter& out );
			template< class GraphType, class Assoc, class Iter >
				static int maxStable2( const GraphType &ag, Assoc &subset, Iter& out );
		};

		// czy cograph
		/** \brief Test if cograph.
		 *
		 *  The method tests if the graph \a g is a cograph.
		 *  \param g the tested graph.
		 *  \return true if \a g is a cograph, false otherwise .*/
		template< class GraphType > static bool cograph( const GraphType &g );


	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	class IsIt: public IsItPar< AlgsDefaultSettings > { };
#include "detect.hpp"
}

#endif