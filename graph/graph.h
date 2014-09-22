#ifndef KOALA_GRAPH_H
#define KOALA_GRAPH_H

/* graph.h
 * Moduł definujący graf oraz wszystkie podstawowe operacje wykonywane na grafach.
 */

#include <algorithm>
#include <cstdio>
#include <cassert>

#include "../base/defs.h"

namespace Koala
{

    namespace Privates {

        template <class T> class RepsDeleterBase {

                RepsDeleterBase(const RepsDeleterBase&) {}
                RepsDeleterBase& operator=(const RepsDeleterBase&) {}

            public:

                T* buf;
                int len;

                 RepsDeleterBase() : buf(0), len(0) {}
                ~RepsDeleterBase()
                {   if (buf) delete [] buf;  }
        };

    }

    template <class T,class Settings> class AssocRepsDeleterBase : public Privates::RepsDeleterBase<T>
    {
        public:

        template <class Iter>
        void init(Iter beg, Iter end, int n)
        {
            typename Settings::template VertEdgeAssocCont< T,EmptyVertInfo >::Type res(n);
            for(Iter i=beg;i!=end;++i) if (*i) res[*i]=Koala::EmptyVertInfo();
            this->buf = new T[res.size()];
            this->len=res.getKeys(this->buf);
        }

        template <class Iter>
        static int clear(Iter beg, Iter end)
        {
            typename Settings::template VertEdgeAssocCont< T,EmptyVertInfo >::Type res(end-beg);
            for(Iter i=beg;i!=end;++i) if (*i) res[*i]=Koala::EmptyVertInfo();
            return res.getKeys(beg);
        }
    };

    template <class T,class Settings> class SortRepsDeleterBase : public Privates::RepsDeleterBase<T>
    {
        public:

            template <class Iter>
            void init(Iter beg, Iter end, int n)
            {
                this->len=0;
                for(Iter i=beg;i!=end;++i) if (*i) this->len++;
                this->buf = new T[this->len];
                this->len=0;
                for(Iter i=beg;i!=end;++i) if (*i)
                {
                    this->buf[this->len++]=*i;
                }
                Settings::template sort( this->buf,this->buf + this->len );
                this->len =std::unique( this->buf,this->buf + this->len ) - this->buf;
            }

            template <class Iter>
            static int clear(Iter beg, Iter end)
            {
                Settings::template sort( beg,end);
                return std::unique( beg,end ) - beg;
            }
    };


	// Domyslne wytyczne parametryzujace strukture i dzialanie metod klasy grafu
	// TODO: przetestowac dzialanie grafu przy roznych ustawieniach

	/* GrDefaultSettings
	 * edAllow - maska okreslajaca dopuszczalne typy krawedzi
	 * adjMatrixAllowed - czy jest dopuszczalne tworzenie macierzy sasiedztwa
	 */
	/** \brief Default graph settings
	 *
	 *  Class stores all parameters of graph. In basic it sets types of edges and allows or not adjacency matrix.
	 *  \tparam edAllow Determines allowed edge types. Parameter works as a mask and different bits respond for different properties.
	 *  - Detached   = 0x0;
	 *  - Loop       = 0x1;
	 *  - Undirected = 0x2;
	 *  - Directed   = 0xC;
	 *  \tparam  adjMatrixAllowed Parameter decides whether adjacency matrix is allowed.
	 * \ingroup DMgraph*/
	template< EdgeType edAllow, bool adjMatrixAllowed > class GrDefaultSettings
	{
	public:

		// maska okreslajaca dopuszczalne typy krawedzi
		//WEN: opis
		enum { EdAllow = edAllow };
		// czy jest dopuszczalne tworzenie macierzy sasiedztwa
		//WEN: opis
		enum { AdjMatrixAllowed = adjMatrixAllowed };
        //NEW: ustawiona flaga wprowadza do wierzcholka i krawedzi metode getGraph() - wskaznik staly na macierzysty graf
        //WEN: opis
		enum { VertEdgeGraphPtr = false };

		// typ klasy tablicy asocjacyjnej uzywanej w metodach grafu przypisujacej wierzcholkom wartosci typu B
		//NEW: zmiana nazwy
		/** \brief Association table for vertices
		 *
		 *  The type of association table for vertices is definded.
		 *  \tparam A pointer to vertex (PVert).
		 *  \tparam B type of values associated to vertices.
		 */
		template< class A, class B > class VertEdgeAssocCont
		{
		public:
			typedef AssocArray< A,B > Type;

			//Nie usuwac komentarzy (przykladowe uzycia) Inne mozliwosci:

			//  typedef AssocTable < BiDiHashMap<A,B> > Type;

            //  typedef AssocTable < HashMap<A,B> > Type;

            //  typedef AssocTable < std::map<A,B> > Type;
		};


		// typ klasy tablicy asocjacyjnej uzywanej w metodach grafu przypisujacej wierzcholkom innego grafu (np.
		// innego typu) wartosci typu B
		/** \brief Association table for vertices of other graphs
		 *
		 *  The type of association table for vertices is definded.
		 *  \tparam A pointer to vertex (PVertex).
		 *  \tparam B type of values associated to vertices.
		 */
		template< class A, class B > class ExtVertAssocCont
		{
		public:
			typedef AssocArray< A,B > Type;
			//Nie usuwac komentarzy (przykladowe uzycia) Inne mozliwosci:

			//  typedef AssocTable < BiDiHashMap<A,B> > Type;

            //  typedef AssocTable < HashMap<A,B> > Type;

            //  typedef AssocTable < std::map<A,B> > Type;
		};

		// typ macierzy sasiedztwa dla krawedzi skierowanych - zbedne przy edAllow=false
		//WEN: opis
		template< class K, class V > class AdjMatrixDirEdges
		{
		public:
			typedef SimpleAssocMatrix< K,V,AMatrNoDiag > Type;
			//Nie usuwac komentarzy (przykladowe uzycia) Inne mozliwosci:
			// por.  komentarze w defs.h klasa domyslnych wytycznych algorytmow AlgsDefaultSettings

            //  typedef SimpleAssocMatrix<K,V,AMatrNoDiag,std::vector< std::vector<typename SimpleAssocMatrixInternalTypes<K,V>::BlockType> >,Privates::PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
            //  typedef SimpleAssocMatrix<K,V,AMatrNoDiag,std::vector< std::vector<typename SimpleAssocMatrixInternalTypes<K,V>::BlockType> >,Privates::PseudoAssocArray<K,int,AssocTable<HashMap<K,int> > > > Type;
            //  typedef SimpleAssocMatrix<K,V,AMatrNoDiag,std::vector< std::vector<typename SimpleAssocMatrixInternalTypes<K,V>::BlockType> >,Privates::PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;

			// typedef AssocMatrix< K,V,AMatrNoDiag > Type;

			//  typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< BlockOfAssocMatrix<V> >,Privates::PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
            //  typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< BlockOfAssocMatrix<V> >,Privates::PseudoAssocArray<K,int,AssocTable<HashMap<K,int> > > > Type;
            //  typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< BlockOfAssocMatrix<V> >,Privates::PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;

            //  typedef  Assoc2DimTable< AMatrNoDiag, std::map<std::pair<K,K>, V > > Type;
            //  typedef  Assoc2DimTable< AMatrNoDiag, BiDiHashMap<std::pair<K,K>, V > > Type;
            //  typedef  Assoc2DimTable< AMatrNoDiag, HashMap<std::pair<K,K>, V > > Type;
		};

		// typ macierzy sasiedztwa dla krawedzi nieskierowanych - zbedne przy edAllow=false
		//WEN: opis
		template< class K, class V > class AdjMatrixUndirEdges
		{
		public:
			typedef SimpleAssocMatrix< K,V,AMatrTriangle > Type;
			// - przyklad uzycia, nie usuwac
			//Nie usuwac komentarzy (przykladowe uzycia) Inne mozliwosci:

            //  typedef SimpleAssocMatrix<K,V,AMatrTriangle,std::vector< std::vector<typename SimpleAssocMatrixInternalTypes<K,V>::BlockType> >,Privates::PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
            //  typedef SimpleAssocMatrix<K,V,AMatrTriangle,std::vector< std::vector<typename SimpleAssocMatrixInternalTypes<K,V>::BlockType> >,Privates::PseudoAssocArray<K,int,AssocTable<HashMap<K,int> > > > Type;
            //  typedef SimpleAssocMatrix<K,V,AMatrTriangle,std::vector< std::vector<typename SimpleAssocMatrixInternalTypes<K,V>::BlockType> >,Privates::PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;

			// typedef AssocMatrix< K,V,AMatrTriangle > Type;

			//  typedef AssocMatrix<K,V,AMatrTriangle,std::vector< BlockOfAssocMatrix<V> >,Privates::PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
            //  typedef AssocMatrix<K,V,AMatrTriangle,std::vector< BlockOfAssocMatrix<V> >,Privates::PseudoAssocArray<K,int,AssocTable<HashMap<K,int> > > > Type;
            //  typedef AssocMatrix<K,V,AMatrTriangle,std::vector< BlockOfAssocMatrix<V> >,Privates::PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;

            //  typedef  Assoc2DimTable< AMatrTriangle, std::map<std::pair<K,K>, V > > Type;
            //  typedef  Assoc2DimTable< AMatrTriangle, BiDiHashMap<std::pair<K,K>, V > > Type;
            //  typedef  Assoc2DimTable< AMatrTriangle, HashMap<std::pair<K,K>, V > > Type;
		};

		// Dodatkowe pola (poza info) wprowadzane do obiektu wierzcholka
		//WEN: opis
		template< class VertInfo, class EdgeInfo, class Settings > struct VertAdditData
		{
			// - w tej wersji umozliwia korzystanie z AssocArray
			AssocKeyContReg assocReg;
		};

		// Dodatkowe pola (poza info) wprowadzane do obiektu krawedzi
		//WEN: opis
		template< class VertInfo, class EdgeInfo, class Settings > struct EdgeAdditData
		{
			// - w tej wersji umozliwia korzystanie z AssocArray
			AssocKeyContReg assocReg;
		};

		// czy dostosowywac rozmiar pamieci wyjsciowych tablic asocjacyjnych
		//enum { ReserveOutAssocCont = true };

		// wybrany do uzytku wewnetrznego algorytm sortowania tablic
		// Wybrany do użytku wewnętrznego algorytm sortowania tablic. Domsyslnie sort. kopcowe
		/**  \brief Table sorting algorithm*/
		template< class Iterator > static void sort( Iterator first, Iterator last )
		{
            std::make_heap( first,last );
            std::sort_heap( first,last );
        }
		// ... i to samo z funkcją porównującą.
		/** \brief Table sorting algorithm
		 *
		 *  \tparam Iterator the iterator class. WEN: a opis przedzialu poczatkowo-zakoncowego?
		 *  \tparam Comp the comparison object function. WEN: powinien byc strict weak order, jak w std::sort
		 */
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
//       }

        //NEW: klasa usuwajaca ew. powtorzenia i wartosci zerowe z ciagow wejsciowych podanych miedzy iteratorami - filtruje
        // wejscie w roznych metodach

        template <class T,class Settings> class RepsDeleter
            : public AssocRepsDeleterBase<T, Settings>
        //Inna mozliwosc
        //  : public SortRepsDeleterBase<T, Settings>
        {};

	};

	namespace Privates
	{

        template <class T,class Settings> class GraphRepsDeleter : public Settings:: template RepsDeleter<T, Settings>
        {
            public:

                template <class Iter>
                GraphRepsDeleter(Iter beg, Iter end)
                {
                    int size=0;
                    for(Iter i=beg;i!=end;++i) size++;
                    this->init(beg,end,size);
                }

                GraphRepsDeleter(T* beg,T* end)
                {
                    this->init(beg,end,end-beg);
                }

                GraphRepsDeleter(const T* beg,const T* end)
                {
                    this->init(beg,end,end-beg);
                }

        };

		/* DummyVar
		 * Do uzytku wewnetrznego, typ zmiennej udajacej int lub wskaznik, przypisywalny z i konwertowalny do takiego
		 * typu
		 */
		template< class T > struct DummyVar
		{
			DummyVar< T > operator=( const T& arg ) const;
			operator T() const { return 0; }
		};

	}
}

#include "adjmatrix.h"
#include "edge.h"
#include "vertex.h"

/** \brief Main namespace of library.
 *
 *	All the methods, classes, structures so the whole library is in this namespace.
 */
namespace Koala
{
	/* SubgraphBase
	 * Klasa pomocnicza, umozliwia tworzenie hierarchii podgrafow i widokow na graf
	 */
	/** \brief Subgraph
	 *
	 *  Auxiliary class enabling the usage of views (also subgraphs) without creating new graph.
	 *  \ingroup DMgraph*/
	class SubgraphBase
	{
	public:
		// tworzy pusty, niepowiazany obiekt
		/** \brief Constructor
		 *
		 *  Creates empty subgraph. */
		 SubgraphBase(): parent( NULL ), next( NULL ), child( NULL )
		{ }
		// przylacza sie jako obiekt podrzedny rodzica podanego obiektu
		/** \brief Constructor
		 *
		 *  Create empty subgraph ancillary to parent \a x.
		 *  \param x created object is ancillary to \a x. */
		SubgraphBase( const SubgraphBase &x ): parent( NULL ), next( NULL ), child( NULL )
		{ link( x.parent ); }
		// j.w. przylacza jako podgraf rodzica x
		/** \brief Connect.
		 *
		 *  Connect the subgraph to the parent \a x.	 */
		 //WEN: jakie x?
		inline SubgraphBase &operator=( const SubgraphBase & );

		// odlacza sie od rodzica (jesli istnial) i odlacza od siebie wszystkie swoje dzieci
		inline ~SubgraphBase();

	protected:
		mutable const SubgraphBase *parent,*next,*child;

		// wypisuje sie z listy potomkow obiektu nadrzednego, jednak nie usuwa powiazan ze swoimi obiektami podrzednymi
		inline bool unlink();
		// dolacza sie jako obiekt podrzedny podanego obiektu
		inline bool link(const SubgraphBase * = NULL );
	};

	namespace Privates
	{

		/* EdgeCounterLoop
		 *
		 */
		template< EdgeType EdgeAllow > class EdgeCounterLoop
		{
		protected:
			typedef int &NoType;

			mutable int n;
			int &no() const
				{ return n; }
			EdgeCounterLoop(): n( 0 )
				{ }
		};

		template <> class EdgeCounterLoop< 0 >
		{
		protected:
			typedef DummyVar< int > NoType;

			DummyVar< int > no() const
				{ return DummyVar< int >(); }
		};

		/* EdgeCounterDir
		 *
		 */
		template< EdgeType EdgeAllow > class EdgeCounterDir;
		template <> class EdgeCounterDir<EdDirIn|EdDirOut>
		{
		protected:
			typedef int &NoType;

			mutable int n;
			int &no() const
				{ return n; }
			EdgeCounterDir(): n( 0 )
				{ }
		};

		template <> class EdgeCounterDir< 0 >
		{
		protected:
			typedef DummyVar< int > NoType;

			DummyVar< int > no() const
				{ return DummyVar< int >(); }
		};

		/* EdgeCounterUndir
		 *
		 */
		template< EdgeType EdgeAllow > class EdgeCounterUndir
		{
		protected:
			typedef int &NoType;

			mutable int n;
			int &no() const
				{ return n; }
			EdgeCounterUndir(): n( 0 )
				{ }
		};

		template <> class EdgeCounterUndir< 0 >
		{
		protected:
			typedef DummyVar< int > NoType;

			DummyVar< int > no() const
				{ return DummyVar< int >(); }
		};


    //NEW: sprywatyzowano
	/* Graph
	 * Graf wraz z wszystkimi podstawowymi operacjami wykonywanymi na grafach. Parametry grafu to klasy, których
	 * instancje będą etykietami wierzchołków i krawędzi. Settings - wytyczne parametryzujace strukture i dzialanie
	 * metod klasy grafu.
	 */
        template< class GraphType > struct GraphInternalTypes;
        template< class VertInfo, class EdgeInfo, class Settings >
            struct GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >
        {
            typedef Koala::Vertex< VertInfo,EdgeInfo,Settings > Vertex;
            typedef Vertex* PVertex;
            typedef Koala::Edge< VertInfo,EdgeInfo,Settings > Edge;
            typedef Edge* PEdge;
            typedef VertInfo VertInfoType;
            typedef EdgeInfo EdgeInfoType;
            typedef Settings GraphSettings;
        };

	}

#include "grconst.h"
	/** \brief Main class.
	 *
	 *  Main structure of library representing graph and some the basic operations.
	 *  \ingroup DMgraph*/
	template< class VertInfo = EmptyVertInfo, class EdgeInfo = EmptyVertInfo,
		class Settings = GrDefaultSettings< EdAll,true > > class Graph:
		public SubgraphBase, protected Privates::EdgeCounterLoop< Settings::EdAllow & Loop >,
		protected Privates::EdgeCounterDir< Settings::EdAllow & (EdDirIn|EdDirOut) >,
		protected Privates::EdgeCounterUndir< Settings::EdAllow & Undirected >,
		public ConstGraphMethods< Graph< VertInfo,EdgeInfo,Settings > >
	{
	public:
		friend class Koala::Edge< VertInfo,EdgeInfo,Settings >;

		typedef typename Privates::GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::Vertex Vertex;/**< \brief Type of vertex*/
		typedef typename Privates::GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::PVertex PVertex;/**< \brief Type of pointer to vertex WEN: tj. używany w procedurach identyfikator wierzchołka.*/
		typedef typename Privates::GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::Edge Edge;/**< \brief Type of edge*/
		typedef typename Privates::GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::PEdge PEdge;/**< \brief Type of pointer to edge WEN: tj. używany w procedurach identyfikator krawedzi.*/
		typedef typename Privates::GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::VertInfoType VertInfoType; /**< \brief Type of vertex info class WEN: tj. typ pola info*/
		typedef typename Privates::GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::EdgeInfoType EdgeInfoType; /**< \brief Type of edge info class WEN: tj. typ pola info*/
		typedef typename Privates::GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::GraphSettings GraphSettings; /**< \brief Type of class with settings.*/
		typedef Graph< VertInfo,EdgeInfo,Settings > GraphType; /**< \brief Current graph type */

		// Do wspolpracy z podgrafami
		typedef Graph< VertInfo,EdgeInfo,Settings > RootGrType; /**< \brief Current graph type */


        //NEW: umozliwia oddelegowanie alokowania wierzcholkow/krawedzi do pul SimplArrPool o ograniczonej
        //pojemnosci - na uzytek grafow lokalnych w procedurach
		// Konstruktory
		// Tworzy pusty graf, bez krawędzi i wierzchołków.
		/** \brief Constructor
		 * WEN: bez ew. macierzy sasiedztwa
		 *  Creates a new graph without any edges or vertices.
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		Graph(SimplArrPool<Koala::Vertex< VertInfo,EdgeInfo,Settings > > *valloc=0,
            SimplArrPool<Koala::Edge< VertInfo,EdgeInfo,Settings > > *ealloc=0);

		// Konstruktor kopiujący.
		/** \brief Copy constructor
		 * WEN: powstaje bez ew. macierzy sasiedztwa
		 *  Creates a new graph which is a copy of \a graph.
		 *  \param graph reference to the copied graph.  */
		Graph( const Graph &graph );

		// Destruktor
		~Graph();/**< \brief Destructor*/

		// Usuwamy wszystkie wierzchołki i krawędzie grafu.
		/** \brief Clear graph.
		 *
		 *  Deletes all the edges and vertices from the graph.
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		void clear();
		// Usuwamy wszystkie wierzchołki grafu.
		/** \brief Clear edges.
		 *
		 *  Deletes all the edges from the graph.
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		void clearEdges();

		// Operacje na całym grafie
		// Operator przypisania.
		/** \brief Copy content operator
		 *
		 *  Overloaded operator = assigns the new graph stored in gr.
		 *  \param gr the graph assigned to the base graph.
		 *  \return reference to the assigned graph.*/
		Graph< VertInfo,EdgeInfo,Settings > &operator=( const Graph< VertInfo,EdgeInfo,Settings > &gr );

		// Dodanie kopii grafu do naszego grafu
		/** \brief Add graph.
		 *  WEN: nie zmienia ew. posiadania macierzy sasiedztwa
		 *  Operator += allows to add a copy of the graph \a gr to the considered one. WEN: jako osobna skladowa, nowe wierzcholki wprowadzane sa na koncu listy
		 *  \param gr the added graph.
		 *  \returns the sum of graphs.
		 */
		Graph< VertInfo,EdgeInfo,Settings > &operator+=( const Graph< VertInfo,EdgeInfo,Settings > &gr );

		// Przenosimy wszystkie wierzchołki i krawędzie do naszego grafu. Zwraca pierwszy wprowadzony wierzcholek
		/** \brief Move vertices and edges.
		 *  WEN: jako osobna skladowa, nowe wierzcholki wprowadzane sa na koncu listy
		 WEN: nie zmienia ew. posiadania macierzy sasiedztwa
		 *  Edges and vertices from the graph \a gr are moved to the considered graph. (without reallocation)
		 *  \param gr the moved graph.
		 *  \returns Pointer to the first inserted vertex.
		 */
		typename GraphType::PVertex move( Graph< VertInfo,EdgeInfo,Settings > &gr );

        //NEW: przenosi vert z innego grafu do naszego (bez zmiany identyfikatora). Drugi graf musi byc rozny od naszego, wszystkie styczne z nim
        //krawedzie (nawet petle) sa usuwane.
		bool move( Graph< VertInfo,EdgeInfo,Settings > &gr, PVertex vert );
		//NEW: przenosi do naszego grafu wierzcholki z grafu gr podane w przedziale iteratorw. Wraz z nimi przenoszone
		//sa krawedzie podgrafu indukowanego w gr przez te wierzcholki o typie zgodnym z mask. Inne krawedzie
		//styczne z tymi wierzcholkami w gr sa kasowane. gr musi byc rozny od naszego grafu. Zwraca liczby
		//wierz/kraw. ktore ulegly przeniesieniu.
		template< class Iterator > std::pair<int,int>
            move( Graph< VertInfo,EdgeInfo,Settings > &gr, Iterator beg, Iterator end, EdgeType mask=EdAll );
        template< class Iterator > std::pair<int,int>
        //NEW: j.w. ale wersja odporna na powtorzenia wierzcholkow w sekwencji
            move2( Graph< VertInfo,EdgeInfo,Settings > &gr, Iterator beg, Iterator end, EdgeType mask=EdAll );

		// dopuszczalne typy krawedzi grafu
		/** \brief Check Edge types
		 *  WEN: tu jest wazne, ze mowimy o rodzajach dopuszczalnych przez typ grafu, a nie to co w nim jest w danej chwili
		 *  \return the mask representing allowed types of edges (EdgeType).
		 */
		static EdgeType allowedEdgeTypes()
			{ return Settings::EdAllow ; }

		// metody przekazywane do ConstGraphMethods
		//------------- Methods sent to ConstGraphMethods -----------------------------------------

		/** \brief Get number of vertices.
		 *
		 *  Gets the order of the graph.
		 *  \return the number of vertices in graph.
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		int getVertNo() const
			{ return this->no_vert; }
		/** \brief Get next vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the next vertex on the list.
		 *  If parameter \a vert is set to NULL then the first vertex on the list will be taken.
		 *  \param vert the reference vertex.
		 *  \returns a pointer to the next vertex on the vertex list or NULL if the vertex was last.	 */
		inline typename GraphType::PVertex getVertNext( PVertex vert ) const;

		/** \brief Get previous vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the vertex prior to the one pointed by PVertex.
		 *  If parameter \a vert is set to NULL then the last vertex on the list will be taken.
		 *  \param vert the reference vertex.
		 *  \returns a pointer to the previous vertex on the vertex list or NULL if the vertex was the first. */
		inline typename GraphType::PVertex getVertPrev( PVertex vert ) const;

		/** \brief Get edge number.
		 *
		 *  The method gets the number of edges of type determined by the parameter \a direct.
		 *  \param direct the mask representing all types of the considered edges.
		 *  \returns the number of edges of certain type. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		int getEdgeNo( EdgeType direct = EdAll ) const;

		/** \brief Get next edge
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask \a direct. The method gets the pointer to the edge next to \a e.
		 *  If parameter e is set to NULL then the first edge on the list is taken.
		 *  \param e the reference edge.
		 *  \param direct the mask representing all the types of considered edges. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
		 *  \returns pointer to the next edge or if \a e is the last edge then NULL. */
		inline typename GraphType::PEdge getEdgeNext( PEdge e, EdgeType direct = EdAll ) const;

		/** \brief Get previous edge.
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask direct. The method gets the pointer to the edge previous to \a edge.
		 *  If parameter \a edge is set to NULL then the last edge on the list will be taken.
		 *  \param edge the reference edge.
		 *  \param direct the mask representing all the types of considered edges. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
		 *  \returns pointer to the previous edge or if edge is the first edge then NULL. */
		inline typename GraphType::PEdge getEdgePrev( PEdge edge, EdgeType direct = EdAll ) const;

		/** \brief Get vertex degree.
		 *  WEN: por. koment. w grconst.h
		 *  Gets the number of edges incident to the vertex of direction (with respect to the vertex \a vert) prespecified by the mask direct.
		 *  \param vert the pointer to the reference vertex.
		 *  \param direct the mask representing the direction of considered edges.
		 *  \returns the number of edges directed as required in \a direct.
		 */
		inline int getEdgeNo( PVertex vert, EdgeDirection direct = EdAll ) const;

		/** \brief Get next edge.
		 *
		 *  The method allows to see through all the edges incident to \a vert, of direction congruent with the mask \a direct.
		 *  For each vertex the edges incident to it form a list. The method gets the pointer to the edge next to \a e.
		 *  If the parameter \a e is set to NULL then the first edge on the list is taken.
		 *  \param vert only the edges incident to \a vert are considered.
		 *  \param e the reference edge.
		 *  \param direct the mask representing the types of edges. WEN: no wlasnie nie bardzo type od edge tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
		 *  \returns the pointer to the next edge or if the edge is the last edge then NULL.
		 */
		inline typename GraphType::PEdge getEdgeNext( PVertex vert, PEdge e, EdgeDirection direct = EdAll ) const;

		/** \brief Get previous edge
		 *
		 *  The method allows to see through all the edges incident to \a vert, of direction congruent with the mask \a direct. The method gets the pointer to the edge previous to \a ed.
		 *  If the parameter \a ed is set to NULL then the last edge on the list will be returned.
		 *  \param vert the reference vertex.
		 *  \param ed the reference edge.
		 *  \param direct the mask representing the types of edges. WEN: no wlasnie nie bardzo type od edge tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
		 *  \returns Pointer to the previous edge or if the edge is the first then NULL.
		 */
		inline typename GraphType::PEdge getEdgePrev( PVertex vert, PEdge ed, EdgeDirection direct = EdAll ) const;

		/** \brief Get number of parallel edges.
		 *
		 *  The method counts the number of edges between two vertices. Only edges directed in the way consistent with the mask \a direct are considered.
		 *  \param vert1 the first vertex
		 *  \param vert2 the second vertex
		 *  \param direct the mask representing the direction of considered edges. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
		 *  \returns the number of edges between \a vert1 and \a vert2. */
		int getEdgeNo( PVertex vert1, PVertex vert2, EdgeDirection direct = EdAll ) const;

		/** \brief Get next parallel edges.
		 *
		 *  The pointer to the next parallel edge is returned. The mask \a direct limits considered edges. If adjacency matrix is allowed the method will use it, otherwise lists are searched through.
		 *  If the parameter \a ed is set to NULL then the first edge on the list will be taken.
		 *  \param vert1 the first vertex.
		 *  \param vert2 the second vertex.
		 *  \param ed the reference edge
		 *  \param direct the mask representing the direction of considered edges. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
		 *  \returns the pointer to the next parallel edge or NULL if \a ed is the last. */
		typename GraphType::PEdge getEdgeNext( PVertex vert1, PVertex vert2, PEdge ed, EdgeDirection diretction = EdAll ) const;

		/** \brief Get previous parallel edges.
		 *
		 *  The pointer to the parallel edge previous to \a ed is returned. The mask limiting considered edges is possible.
		 *  If the adjacency matrix is allowed the method will use it, otherwise only lists are checked.
		 *  \param vert1 the first vertex.
		 *  \param vert2 the second vertex.
		 *  \param ed the reference edge.
		 *  \param direct the mask representing direction of the considered edges. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
		 *  \returns the pointer to the previous parallel edge or NULL if \a ed is the first edge. */
		typename GraphType::PEdge getEdgePrev( PVertex vert1, PVertex vert2, PEdge ed, EdgeDirection direct = EdAll ) const;

		/** \brief Get edge type.
		 *
		 *  \param e the pointer to considered edge.
		 *  \return the EdgeType value which is a mask representing the type of edge.
		 *  - Detached   = 0x0 WEN: wywal, bo ta wartosc nie moze sie pojawic
		 *  - Loop       = 0x1
		 *  - Undirected = 0x2
		 *  - Directed   = 0xC
		 *  \sa Koala::EdgeType */
		inline EdgeType getEdgeType( PEdge e ) const;

		/** \brief Get edge ends
		 *  WEN: tj. pierwsza koncowa w .first, druga w .second - krawedz ma ponumerowane konce
		 *  The method gets the pair of vertices on which the edge \a ed is spanned.
		 *  \param ed the considered edge.
		 *  \returns the pair of the vertices that are the ends of the edge \a ed.	 */
		inline std::pair< typename GraphType::PVertex,typename GraphType::PVertex > getEdgeEnds( PEdge ed ) const;

		/** \brief Get the first vertex.
		 *
		 *  \param ed the considered edge.
		 *  \returns the pointer to the first vertex of \a ed.
		 */
		inline typename GraphType::PVertex getEdgeEnd1( PEdge ed ) const;

		/** \brief Get the second vertex.
		 *
		 *  \param ed the considered edge
		 *  \returns the pointer to the second vertex of \a ed.
		 */
		inline typename GraphType::PVertex getEdgeEnd2( PEdge ed ) const;

		/** \brief Get arc direction
		 *
		 *  The method gets the edge direction. Possible values of EdgeDirection are:
		 *  - EdNone   = 0x00 if ed is NULL WEN: to v,
		 *  - EdLoop   = 0x01 if ed is a loop, WEN: connected to v,
		 *  - EdUndir  = 0x02 if ed is undirected,
		 *  - EdDirIn  = 0x04 if ed is directed and vert is the second vertex of ed,
		 *  - EdDirOut = 0x08 if ed is directed and vert is the first vertex of ed.
		 *  \param ed considered edge.
		 *  \param vert reference vertex.
		 *  \returns direction of edge \a ed.
		 */
		inline EdgeDirection getEdgeDir( PEdge ed ,PVertex vert ) const;

		// ustawianie pol info
		/** \brief Set vertex Information.
		 *
		 *  The method sets the new VertInfoType object as the information about \a vert. WEN: tj. pole info
		 *  \param vert the vertex whose info is going to be changed.
		 *  \param info the object representing new information.
		 */
		inline void setVertInfo( PVertex vert, VertInfo info = VertInfo() ) const;

		/** \brief Set edge information.
		 *
		 *  The method sets the new EdgeInfoType object as the information about the edge \a ed. WEN: tj. pole info
		 *  \param ed the edge whose info is going to be changed.
		 *  \param info the object representing new information.
		 */
		inline void setEdgeInfo( PEdge ed, EdgeInfo info = EdgeInfo() ) const;

		// Usuwamy wierzchołek z grafu.
		// wersja z false skuteczna tylko dla wierzcholka izolowanego
		/** \brief Delete vertex from graph.
		 *
		 *  Vertex \a vert is deleted form the graph. If the parameter \a force is set to false, \a vert is deleted only if there are no edges incident to it.
		 *  \param vert the deleted vertex.
		 *  \param force the boolean flag saying that deletion should be forced, even if there are some edges incident to \a vert. WEN: wtedy te krawedzie sa usuwane razem z nim
		 *  \returns none
		 */
		inline void del( PVertex vert, bool force = true )
			{ this->delVert( vert,force ); }
		// synonim
		/** \brief Delete vertex from graph.
		 *
		 *  Vertex \a vert is deleted form the graph. If the parameter \a force is set to false, \a vert is deleted only if there are no edges incident to it.
		 *  \param vert the deleted vertex.
		 *  \param force the boolean flag saying that deletion should be forced, even if there are some edges incident to \a vert. WEN: wtedy te krawedzie sa usuwane razem z nim
		 *  \returns none */
		void delVert( PVertex vert , bool force = true );
		// usuwanie zbioru wierzcholkow, zwraca ich liczbe
		/** \brief Delete vertices.
		 *
		 *  Iterators \a begin and \a end define a set of vertices. All vertices from this set are to be deleted (no matter if the are incident with some edges).
		 *  Notice that method is a template function and iterators can be of any type used by the user, for example a simple table, but also some STL container. WEN: byle na typ PVertex
		 *  \param begin the iterator pointing to the first element of the deleted set of vertices.
		 *  \param end the iterator pointing to the next element after the  last deleted vertex.
		 *  \returns the number of deleted vertices. */
		template< class Iterator > int delVerts( Iterator begin, Iterator end );

		// wersja odporna na powtorzenia, ktore sa pmijane
		/** \brief Delete vertices
		 *
		 *  Iterators \a begin and \a end define a set of vertices. All vertices from this set are to be deleted (no matter if they are incident with some edges ).
		 *  Notice that method is a template function and iterators can be of any type used by the user, for example a simple table, but also some STL container. WEN: byle na typ PVertex The repetition-proof version of delVerts. WEN: ktore sa ignorowane
		 *  \param begin the iterator pointing to the first element of the deleted set of vertices.
		 *  \param end the iterator pointing to the next element after the last deleted vertex.
		 *  \returns the number of unique and deleted vertices. */
		template< class Iterator > int delVerts2( Iterator begin, Iterator end );

		/** \brief Delete vertices
		 *
		 *  All vertices from set \a s are to be deleted (no matter if the are incident with some edges).
		 *  \param s the set of vertices which are to be deleted.
		 *  \returns the number of deleted vertices. */
		inline int delVerts( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > &s )
			{ return delVerts( s.begin(),s.end() ); }

		// Usuwamy wszystkie wierzchołki z grafu.
		/** \brief Delete all vertices
		 *
		 *  All vertices are deleted from the graph. No matter if they are incident with edges or not. The method works similarly to clear(), the only difference is that it returns the number of vertices.
		 *  \return the number of vertices.	 */
		inline int delVerts();

		// Usuwamy krawędź z grafu.
		/** \brief Delete edge
		 *
		 *  Deletes the edge form the graph.
		 *  \param e the deleted edge
		 */
		inline void del( PEdge ed);

		// synonim
		/** \brief Delete edge.
		 *
		 *  Deletes the edge form the graph.
		 *  \param edge the deleted edge
		 */
		inline void delEdge( PEdge edge )
			{ this->del( edge ); }
		// usuwanie zbiorow krawedzi
		// krawedzie przy danym wierzcholku o podanej orientacji wgledem niego
		/** \brief Delete edges.
		 *
		 *  For a vertex \a vert, each edge incident to it is deleted as long as its direction is consistent with the mask \a direct.
		 *  \param vert all the edges incident to this vertex and of direction (relative to \a vert) consistent with \a direct are to be deleted.
		 *  \param direct the mask representing the relative direction of edges which should be deleted. Default value EdAll. WEN: no wlasnie relative direction trzeba wyjasnic
		 *  \return the number of deleted edges.
		 */
		int delEdges( PVertex vert, EdgeDirection direct = EdAll );

		// krawedzie miedzy danymi wierzcholkami o podanej orientacji
		/** \brief Delete edges
		 *
		 *  Deletes all the edges between \a vert1 and \a vert2 of direction consistent with the mask direct. WEN: consistent direction make zrozumialym
		 *  \param vert1 the first reference vertex.
		 *  \param vert2 the second reference vertex.
		 *  \param direct the mask determining the direction of the deleted edges. By default all edges are deleted.
		 *  \return the number of deleted edges.
		 */
		int delEdges( PVertex vert1, PVertex vert2, EdgeDirection direct = EdAll );

		// podajemy zbior do usuniecia, uwzglednia tylko krawedzie o typie zawartym w masce
		/** \brief Delete set of edges.
		 *
		 *  Let us consider the container of edges defined by iterators begin and end. An edge from this container is deleted as long as its WEN: tu akurat rodzaj krawedzi direction is consistent with the mask \a direct. Any container with a defined iterator can be used as set. WEN: byle na typ PEdge
		 *  \param begin the iterator of the first element of the set.
		 *  \param end the iterator of WEN: za-last last element of the set.
		 *  \param direct the mask determining the direction WEN: rodzaj krawedzi of the deleted edges.
		 *  \return the number of deleted edges.
		 */
		template< class Iterator > int delEdges( Iterator begin, Iterator end, EdgeType direct = EdAll );

		// wersja odporna na powtorzenia, ktore sa pmijane
		/** \brief Delete set of edges.
		 * WEN: j.w.
		 *  Let us consider the container of edges defined by iterators \a begin and \a end.
		 *  An edge from this container is deleted as long as its direction is consistent with the mask \a direct. Any container with the defined iterator can be used as theset.
		 *  Works similarly to delEdges, but here repeated elements are omitted
		 *  \param begin the iterator of the first element of the set.
		 *  \param end the iterator of WEN: za-last element of the set.
		 *  \param direct the mask determining the direction of the deleted edges.
		 *  \return the number of deleted edges.
		 */
		template< class Iterator > int delEdges2( Iterator begin, Iterator end, EdgeType direct = EdAll );

		/** \brief Delete set of edges
		 *
		 *  Let us consider a set of edges. An edge from this set is deleted as long as its direction WEN: rodzaj krawedzi is consistent with the mask \a direct.
		 *  \param s the set of deleted edges.
		 *  \param direct the mask determining the direction WEN: rodzaj krawedzi of deleted edges.
		 *  \return number of deleted edges.
		 */
		inline int delEdges( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &s,
			EdgeType direct = EdAll )
			{ return delEdges( s.begin(),s.end(),direct ); }
		// Usuwamy krawędzie z grafu o typie zawartym w masce
		/** \brief Delete all edges
		 *
		 *  Ale the edges of prespecified direction WEN: rodzaj krawedzi are deleted from the graph.
		 *  \param direct the mask determining the direction WEN: rodzaj krawedzi of the deleted edges.
		 *  \return the number of deleted edges.
		 */
		int delEdges( EdgeType direct = EdAll );

		// Dodajemy nowy wierzchołek do grafu (z etykietą info). Zwraca jego wskaznik
		/** \brief Add vertex.
		 *
		 *  A new vertex is created and added to the graph.
		 *  \param info the information stored in the new-created vertex. WEN: po prostu pole info
		 *  \return the pointer to the created vertex.
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		inline typename GraphType::PVertex addVert( VertInfo info = VertInfo() );

		// Uwaga: proba dodania krawedzi typu niezawartego w allowedEdgeTypes() wywoluje blad
		// Dodajemy nową krawędź do grafu (bez etykiety - wartosc domyslna typu EdgeInfo).
		/** \brief Add edge.
		 *
		 *  A new edge (directed or not depending on \a direct) spanned on two vertices \a vert1 and \a vert2 is added to the graph.
		 *  \param vert1 the first vertex of the new-created edge.
		 *  \param vert2 the second vertex of the new-created edge.
		 *  \param direct determines the edge direction. Default value EdUndir. If the edge is directed, the parameter is obligatory regardless of the type of graph.
		 *  \return pointer the new-created edge.
		 *  WEN: nie do konca: dla EdLoop musi byc vert1=vert1, w innych przypadkach vert1!=vert2. Przy EdDirOut, EdUndir i Directed vert1 staje sie pierwsza koncowka, a vert2 druga. Przy EdDirIn odwrotnie. tj. krawedz skierowana wychodzi z pierwszej koncowki
		 *  [See example](examples/graph/graph_clear.html).
		 */
		inline typename GraphType::PEdge addEdge( PVertex vert1, PVertex vert2, EdgeDirection direct = EdUndir );

		// Dodajemy nową krawędź do grafu (z etykietą).
		/** \brief Add edge.
		 * WEN: j.w. odnosnie maski
		 *  A new edge (directed or not depending on direct) spanned on two vertices is added to the graph.
		 *  \param vert1 the first vertex of the new-created edge.
		 *  \param vert2 the second vertex of the new-created edge.
		 *  \param info additional information stored in the edge. WEN: tj. w polu info
		 *  \param direct determines the edge direction. Default value EdUndir. If the edge is directed, the parameter is obligatory regardless of the type of graph.
		 *  \return pointer the new-created edge.
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		inline typename GraphType::PEdge addEdge( PVertex vert1, PVertex vert2, EdgeInfo info, EdgeDirection direct= EdUndir );
		/** \brief Add undirected edge.
		 *
		 *  A new undirected edge spanned on two vertices is added to the graph.
		 *  \param u the first vertex of the new-created edge.
		 *  \param v the second vertex of the new-created edge.
		 *  \param info additional information stored in the edge. WEN: tj. w polu info
		 *  \return pointer the new-created edge.
		 */
		inline typename GraphType::PEdge addLink( PVertex u,PVertex v,EdgeInfo info= EdgeInfo() )
			{   return addEdge(u,v,info,EdUndir); }
		// Dodajemy nowy łuk do grafu (z etykietą).
		/** \brief Add arc. WEN: a czy wiadomo, co to arc?
		 *
		 *  \param vert1 the first vertex (tail) of the new-created arc.
		 *  \param vert2 the second vertex (head) of the new-created arc.
		 *  \param info the additional information stored in the edge. WEN: tj. w polu info
		 *  \return the pointer the new-created arc.
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		inline typename GraphType::PEdge addArc( PVertex vert1, PVertex vert2, EdgeInfo info = EdgeInfo() );

		// Dodajemy nową pętlę do grafu (z etykietą)
		/** \brief Add Loop.
		 *
		 *  Add a new loop that is the edge that connects the vertex to itself.
		 *  \param vert the reference vertex.
		 *  \param info the additional information stored in the loop.
		 *  \return the pointer to the new-created edge. WEN: tj. w polu info
		 *
		 *  [See example](examples/graph/graph_clear.html).
		 */
		inline typename GraphType::PEdge addLoop( PVertex vert, EdgeInfo info = EdgeInfo() );

		// Przenosimy krawędź w inne miejsce z ew. zmiana typu
		/** \brief Move edge
		 * WEN: ten sam komentarz, co przy addedge
		 *  Edge is reconnected to new vertices.
		 *  \param ed the reference edge.
		 *  \param vert1 the pointer to the first vertex to which the edge will be connected.
		 *  \param vert2 the pointer to the second vertex to which the edge will be connected.
		 *  \param direct the target direction of the reconnected edge. Default value EdUndir. Warning! The direction is not copied automatically, that is way the directed arc is changed to undirected if default value is used.
		 *  \return true if success, false otherwise.
		 */
		inline bool moveEdge( PEdge ed, PVertex vert1,PVertex vert2, EdgeDirection direct = EdUndir );

		// zmiana krawedzi skierowanej na nieskierowana, wynik false gdy krawedz nie byla nieskierowana
		/** \brief Change to undirected.
		 *  WEN: numeracja koncowek pozostaje bez zmian
		 *  The type of the edge \a ed is changed to EdUndir i.e. The arc is changed to the undirected edge.
		 *  \param ed the pointer to the reference edge.
		 *  \return true if change is accomplished, false if it was not possible or necessary.
		 *
		 *  [See example](examples/graph/graph_ch2undir.html).
		 */
		bool ch2Undir( PEdge ed );

		// zamiana na nieskierowane krawedzi z podanego zbioru, wynik - liczba udanych zmian
		// podany jest zbior krawedzi
		/** \brief Change to undirected.
		 *  WEN: numeracja koncowek pozostaje bez zmian
		 *  The type of all the edges in the set is changed to undirected. Any container with edges, which has the implemented iterator, can be used.
		 *  \param begin the first element of the set.
		 *  \param end the last WEN: za last element of the set.
		 *  \return the number of successfully changed directions. WEN: tj. undirected i loop moga wystepowac, ale sa ignorowane i nie sa zliczane
		 *
		 *  [See example](examples/graph/graph_ch2undir.html).
		 */
		template< class Iterator > int ch2Undir( Iterator begin, Iterator end );

		/** \brief Change to undirected
		 *  WEN: numeracja koncowek pozostaje bez zmian
		 *  The type of all edges in the set is changed to undirected. Any container with edges that has implemented iterator can be used.
		 *  \param s the set of edges whose direction is going to be changed to undirected.
		 *  \return the number of successfully changed directions. WEN: tj. undirected i loop moga wystepowac, ale sa ignorowane i nie sa zliczane
		 *
		 *  [See example](examples/graph/graph_ch2undir.html).
		 */
		inline int ch2Undir( const Set< typename GraphType::PEdge > &s )
			{ return ch2Undir( s.begin(),s.end() ); }
		// wszystkie krawedzie przy danym wierzcholku o podanej orientacji wzgledem niego
		/** \brief Change to undirected
		 *
		 *  The direction of the edges incident to the vertex \a vert is changed to undirected as long as their direction (with respect to \a vert) is consistent with the mask \a direct
		 *  WEN: numeracja koncowek pozostaje bez zmian
		 *  \param vert the reference vertex.
		 *  \param direct the mask determining the direction (related to vert) of edges which are to be changed to undirected.
		 *  \return the number of successfully changed directions. WEN: tj. undirected i loop moga wystepowac, ale sa ignorowane i nie sa zliczane
		 *
		 *  [See example](examples/graph/graph_ch2undir.html).
		 */
		int ch2Undir( PVertex vert, EdgeDirection direct = EdDirIn|EdDirOut );

		// wszystkie krawedzie miedzy danymi wierzcholkami o podanej orientacji
		/** \brief Change to undirected.
		 *  WEN: numeracja koncowek pozostaje bez zmian
		 *  Direction of all edges spanned on two vertices is changed to undirected.
		 *  \param vert1 the pointer to the first vertex.
		 *  \param vert2 the pointer to the second vertex.
		 *  \param direct the mask determining the initial direction of the changed edges. WEN: jak zwykle koment. o zgodnosci typu/kierunku z maska
		 *  \return the number of successfully changed directions. WEN: tj. undirected i loop moga wystepowac, ale sa ignorowane i nie sa zliczane
		 *
		 *  [See example](examples/graph/graph_ch2undir.html).
		 */
		int ch2Undir( PVertex vert1, PVertex vert2, EdgeDirection direct = EdDirIn|EdDirOut );

		// wszystkie w grafie
		/** \brief Change to undirected.
		 *  WEN: numeracja koncowek pozostaje bez zmian
		 *  The direction of all edges (arcs) in graph is changed to undirected.
		 *  \return the number of successfully changed directions. WEN: tj. undirected i loop moga wystepowac, ale sa ignorowane i nie sa zliczane
		 *
		 *  [See example](examples/graph/graph_ch2undir.html).
		 */
		int ch2Undir();

		// Zmieniamy kierunek krawędzi skierowanej, wynik - czy z sukcesem
		/** \brief Reverse arc.
		 *
		 *  The direction of the edge (arcs) is changed to the opposite. WEN: tj. zmiana numeracji koncowek
		 *  \param ed the considered edge.
		 *  \return true if successful, false otherwise.
		 *
		 *  [See example](examples/graph/graph_rev.html).
		 */
		inline bool rev( PEdge ed );
		// Odwrocenie krawedzi skierowanych z podanego zbioru, wynik - liczba udanych zmian
		// podany zbior krawedzi
		/** \brief Reverse arcs.
		 *
		 *  The direction of all the arcs form the set defined by iterators \a begin and \a end is changed to opposite. WEN: tj. zmiana numeracji koncowek
		 *  Any container of edges, which has a defined iterator, can be used.
		 *  \param begin the iterator of the first element of the set of arcs.
		 *  \param end the iterator of the WEN: za-last last element of the set of arcs. WEN: powtorzenia w ciagu wejsciowym zakazane
		 *  \return the number of successfully reversed arcs.
		 *
		 *  [See example](examples/graph/graph_rev.html).
		 */
		template< class Iterator > int rev( Iterator begin, Iterator end );

		/** \brief Reverse arcs.
		 *
		 *  The direction of all the arcs from the set \a s is changed to the opposite. WEN: tj. zmiana numeracji koncowek
		 *  \param s the set of arcs.
		 *  \return the number of successfully reversed arcs.
		 *
		 *  [See example](examples/graph/graph_rev.html).
		 */
		inline int rev( const Set< typename GraphType::PEdge > &s )
			{ return rev( s.begin(),s.end() ); }

		// wersja odporna na powtorzenia (ignorowane)
		/** \brief Reverse arcs
		 *
		 *  The direction of all the arcs from the set defined by iterators begin and end is changed to the opposite. WEN: tj. zmiana numeracji koncowek
		 *  Any container of edges, which has a defined iterator, can be used.
		 *  In this version repetitions are ignored.
		 *  \param begin the iterator of the first element of the set of arcs.
		 *  \param end the iterator of the WEN: za-last element of the set of arcs.
		 *  \return the number of successfully reversed arcs.
		 */
		 template< class Iterator > int rev2( Iterator begin, Iterator end );
		// wszystkie krawedzie przy danym wierzcholku o podanej orientacji wzgledem niego
		/** \brief Reverse arcs.
		 *
		 *  The direction of all arcs incident to \a vert and of direction (with respect to \a vert) defined by \a direct is changed to the opposite.
		 *  \param vert the reference vertex.
		 *  \param direct the mask defining the WEN: wyjasnic relative direction of arcs which are to be reversed.
		 *  \return the number of successfully reversed arcs.
		 *
		 *  [See example](examples/graph/graph_rev.html).
		 */
		int rev( PVertex vert, EdgeDirection direct = EdDirIn|EdDirOut );

		// wszystkie krawedzie miedzy danymi wierzcholkami o podanej orientacji
		/** \brief Reverse arcs
		 *
		 *  All the arcs between two distinct vertices are reversed.
		 *  \param vert1 the first reference vertex.
		 *  \param vert2 the second reference vertex.
		 *  \param direct the mask determining the type WEN: nie type, tylko rodzaj i ew. kierunek of edges that are to be reversed.
		 *  \return the number of successfully reversed arcs.
		 *
		 *  [See example](examples/graph/graph_rev.html).
		 */
		int rev( PVertex vert1, PVertex vert2, EdgeDirection direct = EdDirIn|EdDirOut );

		// wszystkie w grafie
		/** \brief Reverse edge.
		 *
		 *  All the arcs in the graph (digraph) are reversed.
		 *  \return the number of successfully reversed arcs.
		 *
		 *  [See example](examples/graph/graph_rev.html).
		 */
		int rev();

		// zmiana krawedzi nieskierowanej na luk o zadanej orientacji wzgledem jej podanej koncowki, wynik - czy
		// skuteczne
		/** \brief Change to directed.
		 *
		 *  The undirected edge is changed to directed. Direction (with respect to \a vert) is determined by \a direct.
		 *
		 *  \param ed the edge changed to arc.
		 *  \param vert the reference vertex. WEN: musi byc sasiedni z ed
		 *  \param direct determines the direction of the arc. WEN: dopuszczalne tylko EdDirIn lub EdDirOut
		 *  \return true if change is successful, false otherwise.
		 *
		 *  [See example](examples/graph/graph_ch2dir.html).
		 */
		inline bool ch2Dir( PEdge ed, PVertex vert, EdgeDirection direct = EdDirOut );

		// jw. dla wszystkich krawedzi nieskierowanych przy danym wierzcholku, wynik - liczba zmian
		/** \brief Change to directed.
		 *
		 *  All undirected and incident to vertex \vert edges are changed to directed.
		 *  \param vert reference vertex.
		 *  \param direct determines the direction (with respect to \a vert) of edges converted to arcs. WEN: dopuszczalne tylko EdDirIn lub EdDirOut
		 *  \return the number of changes.
		 *
		 *  [See example](examples/graph/graph_ch2dir.html).
		 */
		int ch2Dir( PVertex vert, EdgeDirection direct = EdDirOut );

		// jw. dla wszystkich krawedzi nieskierowanych miedzy wierzcholkami, wynik - liczba zmian
		/** \brief Change to directed.
		 *
		 *  The type of all undirected and spanned on two prespecified vertices edges is changed to directed.
		 *  \param vert1 the first reference vertex.
		 *  \param vert2 the second reference vertex
		 *  \param direct determines the direction (with respect to \a vert1 and vert2) of edges converted to arcs. WEN: dopuszczalne tylko EdDirIn lub EdDirOut
		 *  \return the number of changes.
		 *
		 *  [See example](examples/graph/graph_ch2dir.html).
		 */
		int ch2Dir( PVertex vert1, PVertex vert2, EdgeDirection direct = EdDirOut );

		// Zmieniamy krawędź nieskierowana w dwa łuki. Nowa krawedz przejmuje info oryginalu i jest zwracana. 0
		// w razie porazki
		/** \brief Change to arcs.
		 *
		 *  The undirected edge is converted into two arcs. More precisely, the edge is changed to directed WEN: zachowujac numeracje koncowek and a new additional arc with opposite direction is created and added to the graph. WEN: jego info jest kopiowane z ed
		 *  \param ed the changed edge.
		 *  \return the pointer to the new arc. WEN: lub 0 gdy ed nie byla nieskierowana
		 *
		 *  [See example](examples/graph/graph_ch2Arcs.html).
		 */
		inline typename GraphType::PEdge ch2Arcs( PEdge ed );

		// j.w.  dla wszystkich krawedzi z danego zbioru, zwraca liczbe sukcesow
		/** \brief Change to arcs.
		 *  WEN: powtorzenia dozwolone i ignorowane
		 *  All the undirected edges in the set are converted into two arcs. More precisely, each WEN: undirected edge is changed to directed and a new arc with the opposite direction WEN: i tym samym info (kopiowanie) is added to the graph.
		 *  \param begin the first element of set.
		 *  \param end the WEN: za-last last edge in the set.
		 *  \return the number of changed edges.
		 *
		 *  [See example](examples/graph/graph_ch2Arcs.html).
		 */
		template< class Iterator > int ch2Arcs( Iterator begin, Iterator end );

		/** \brief Change to arcs.
		 *
		 *  All the undirected edges in the set \a s are converted into two arcs. More precisely, each WEN: undirected edge is changed to directed and a new arc with the opposite direction WEN: i tym samym info (kopiowanie) is created and added to the graph.
		 *  \param s the reference to the set of edges.
		 *  \return the number of changed edges.
		 *
		 *  [See example](examples/graph/graph_ch2Arcs.html).
		 */
		inline int ch2Arcs( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &s )
			{ return ch2Arcs( s.begin(),s.end() ); }

		// j.w. dla wszystkich w grafie
		/** \brief Change to arcs.
		 *
		 *  All the undirected edges in the graph are converted into two arcs. More precisely, each edge is changed to directed and a new arc with opposite direction WEN: i tym samym info (kopiowanie) is added to the graph.
		 *  \return the number of changed edges.
		 *
		 *  [See example](examples/graph/graph_ch2Arcs.html).
		 */
		int ch2Arcs();

		// j.w. dla wszystkich nieskierowanych przy wierzcholku
		/** \brief Change to arcs.
		 *
		 *  All the undirected edges incident to a certain vertex are converted into two arcs. More precisely, each edge is changed to directed and a new arc with opposite direction WEN: i tym samym info (kopiowanie) is added to the graph.
		 *  \param vert the reference vertex.
		 *  \return the number of changed edges.
		 *
		 *  [See example](examples/graph/graph_ch2Arcs.html).
		 */
		int ch2Arcs( PVertex vert );

		// j.w. dla wszystkich nieskierowanych przy miedzy wierzcholkami
		/** \brief Change to arcs.
		 *
		 *  All the undirected edges spanned on two prespecified vertices are converted into two arcs.
		 *  More precisely, each edge is changed to directed and a new arc with opposite direction WEN: i tym samym info (kopiowanie) is added to the graph.
		 *  \param vert1 the first reference vertex.
		 *  \param vert2 the second reference vertex.
		 *  \return the number of changed edges.
		 *
		 *  [See example](examples/graph/graph_ch2Arcs.html).
		 */
		int ch2Arcs( PVertex vert1, PVertex vert2 );

		// ustawienie nowego wierzcholka na srodku istniejacej krawedzi, ktora rozdziela sie na dwie. Podajemy info
		// nowego wierzcholka, ktory jest zwracany
		/** \brief Put vertex in edge.
		 *
		 *  Edge is divided into two edges by one additional vertex. WEN: ed jest kasowana, a dwie nowe otrzymuja jego info. Rodzaje krawedzi: petla->undirected, undir->undir, dir->dir z zachowaniem kierunku
		 *  \param ed the split edge.
		 *  \param info vertex information stored in the new-created vertex.
		 *  \return the pointer to the new-created vertex
		 *
		 *  [See example](examples/graph/graph_put.html).
		 */
		typename GraphType::PVertex putVert( PEdge ed, VertInfo info = VertInfo() );

		// Dla wierzcholka o maks. 2 sasiadach - zdjecie wierzcholka , po czym pozostaje jedna nowa (zwracana)
		// krawedz laczaca tych sasiadow
		/** \brief Pick vertex.
		 *
		 *  If a vertex has at most two neighbors, it is deleted together with all the incident edges. One edge linking two neighbors is created. WEN: przy jednym sasiedzie powstaje petla, przy 2 wybierana jest undirected lub skierowanie odpowiednio do skierowania usuwanych krawedzi
		 *  \param vert the eliminated vertex.
		 *  \param info the information for the new-created edge.
		 *  \return the pointer to the new-created edge. WEN: lub NULL w razie bledu
		 *
		 *  [See example](examples/graph/graph_pick.html).
		 */
		typename GraphType::PEdge pickVert( PVertex vert, EdgeInfo info = EdgeInfo() );

		// sklejanie grupy wierzcholkow w jeden wierzcholek docelowy (zwracany).
		// Mozna wybrac ten wierzcholek sposrod sklejanej grupy. makeloops decyduje, czy krawedzie wewnatrz grupy
		// maja byc usuwane, czy zamieniane w petle
		/** \brief Merge vertices WEN: fachowo to sie nazywa szrinkink :-)
		 *  WEN: oprocz ew. petli nie sa usuwane powstajace przy sciagnieciu kraw. rownolegle
		 *  The set of vertices is merged into one. The target vertices in the set can be chosen.
		 *  \param begin the iterator of the first element of the merged group of vertices.
		 *  \param end   the iterator of the WEN: za-last last element of the merged group of vertices. WEN: powtorzenia niedozwolone
		 *  \param makeloops decides if the edges inside the set are to be converted to loops or WEN: deleted not.
		 *  \param vert the vertex with which all the others will be unified. If NULL any vertex is chosen. WEN: Zawsze zachowywane jest jego info
		 *  \return the pointer to the target vertex.
		 *
		 *  [See example](examples/graph/graph_glue.html).
		 */
		template< class Iterator > typename GraphType::PVertex
			glue( Iterator begin, Iterator end, bool makeloops = false, PVertex vert = NULL );

		/** \brief Merge vertices
		 * WEN: j.w. oprocz powtorzen
		 *  The set of vertices is merged into one. The target vertex in the set can be chosen.
		 *  \param s the reference to the set of vertices which are to be merged.
		 *  \param makeloops decides if the edges inside the set are to be converted to loops or not.
		 *  \param vert the vertex with which all the others will be unified. If NULL any vertex is chosen.
		 *  \return the pointer to the target vertex.
		 *
		 *  [See example](examples/graph/graph_glue.html).
		 */
		inline typename GraphType::PVertex
			glue( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > &s, bool makeloops = false,
				  PVertex res = NULL )
				{ glue( s.begin(),s.end(),makeloops,res ); }
		// wersja odporna na powtorzenia (ignorowane)
		/** \brief Merge vertices
		 * WEN: j.w. + powtorzenia
		 *  Repetition-proof version of the glue method.
		 *  The set of vertices is merged into one. The target vertex in the set can be chosen.
		 *  \param begin the iterator of the first element of the merged group of vertices.
		 *  \param end the iterator of the WEN: za-last element of the merged group of vertices.
		 *  \param makeloops the decides if the edges inside the set are to be converted to loops or not.
		 *  \param vert the vertex with which all the others will be unified. If NULL any vertex is chosen.
		 *  \return the pointer to the target vertex.
		 *
		 *  [See example](examples/graph/graph_glue2.html).
		 */
		template< class Iterator > typename GraphType::PVertex
			glue2( Iterator, Iterator, bool makeloops = false, PVertex = NULL );

		// jw. dla 2 wierzcholkow
		/** \brief Merge vertices
		 * WEN: jw. bez powtorzen
		 *  Two vertices are merged into one. Depending on makeloops edges between them can be converted into loops or deleted.
		 *  \param vert1 the first vertex.
		 *  \param vert2 the last vertex.
		 *  \param makeloops decides if the edges between the two vertices are to be converted into loops or not.
		 *  \return pointer to the target vertex.
		 *
		 *  [See example](examples/graph/graph_glue.html).
		 */
		inline typename GraphType::PVertex glue( PVertex vert1, PVertex vert2, bool makeloops = false );

		// usuwanie krawedzi z danego zbioru rownoleglych do podanej, maska okresla rodzaj relacji rownoleglosci.
		// Wynik - liczba usuniec
		/** \brief Delete paralel edges.
		 *
		 *  All the edges from certain set and parallel to predefined edge WEN: procz niej samej are deleted from graph.
		 *  Set can be given in any form of container that has an iterator defined.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param begin the fist element of the set of edges.
		 *  \param end the WEN: za-last lest element of the set of edges. WEN: powtorzenia zabronione
		 *  \param ed the edge to which all the others are reduced.
		 *  \param reltype detrmines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delParals.html).
		 */
		template< class Iterator > int delParals( Iterator begin, Iterator end, PEdge ed, EdgeDirection reltype = EdUndir );

		/** \brief Delete parallel edges.
		 *
		 *  All the edges from a certain set and parallel to the predefined edge WEN: procz niej samej, are deleted from the graph.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param s the set of edges that are going to be deleted.
		 *  \param edge the edge to which all the others are reduced.
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delParals.html).
		 */
		inline int delParals( const Set< typename GraphType::PEdge > &s, PEdge edge, EdgeDirection reltype = EdUndir )
			{ return delParals( s.begin(),s.end(),edge,reltype ); }

		// wersja odporna na powtorzenia (ignorowane)
		/** \brief Delete parallel edges.
		 *
		 *  All the edges a from certain set and parallel to the predefined edge WEN: procz niej samej are deleted from the graph.
		 *  The set can be given in any form of the container that has an iterator defined.
		 *  This version is repetition-proof.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param begin the first element of the set of edges.
		 *  \param end the WEN: za-last lest element of the set of edges.
		 *  \param ed the edge to which all the others are reduced.
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delParals2.html).
		 */
		template< class Iterator > int delParals2( Iterator begin, Iterator end, PEdge ed, EdgeDirection reltype = EdUndir );

		// wszystkie krawedzie rownolegle
		/** \brief Delete parallel edges.
		 *
		 *  All the edges parallel to the predefined edge WEN: procz niej samej are deleted from the graph.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param ed the edge to which all the others are reduced.
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delParals.html).
		 */
		int delParals( PEdge ed, EdgeDirection reltype = EdUndir );

		// z grupy krawedzi zostawia jedynie nierownoleglych wzgledem siebie reprezentantow. Znaczenie parametrow
		// analogiczne jak w metodach findParals. Zwraca liczbe usuniec.
		/** \brief Delete parallel edges.
		 * WEN: gdzie tu jest podany zwiazek representatives z paralelizmem???
		 *  From the set of edges only unique representatives are left, all the repetitions are deleted. The set of edges can be defined as any container with an iterator.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param begin the first element of the set of edges.
		 *  \param end the WEN: za-lest element of the set of edges. WEN: powtorzenia zabronione
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delAllParals.html).
		 */
		template< class Iterator > int delAllParals( Iterator begin, Iterator end, EdgeType reltype = EdUndir );

		/** \brief Delete parallel edges.
		 *
		 *  From the set of edges only unique representatives are left, all the repetitions are deleted. The set of edges can be defined by any container with an iterator.
		 *  Repetition-proof version of delParals.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param begin the iterator of the fist element of the set of edges.
		 *  \param end the iterator of the WEN: za-lest element of the set of edges.
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delAllParals2.html).
		 */
		template< class Iterator > int delAllParals2( Iterator begin, Iterator end, EdgeType reltype = EdUndir );

		/** \brief Delete parallel edges.
		 *
		 *  From the set of edges only unique representatives are left, all the repetitions are deleted.
		 WEN: gdzie tu jest podany zwiazek representatives z paralelizmem???
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param eset the reference to the set of edges.
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delAllParals.html).
		 */
		int delAllParals( const Set< typename GraphType::PEdge > &eset, EdgeType relType = EdUndir )
			{ return delAllParals( eset.begin(),eset.end(),relType ); }

		/** \brief Delete parallel edges.
		 *
		 *  Delete all the parallel edges incident to the vertex \a vert. Only single representatives are left.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param vert all the parallel edges incident to this vertex are to be deleted.
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delAllParals.html).
		 */
		int delAllParals( PVertex vert, EdgeType reltype = EdUndir );

		/** \brief Delete parallel edges.
		 *
		 *  All the parallel edges between two vertices are deleted, only a single representative is left.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param vert1 the fist vertex.
		 *  \param vert2 the second vertex.
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delAllParals.html).
		 */
		int delAllParals( PVertex vert1, PVertex vert2, EdgeType reltype = EdUndir );

		/** \brief Delete parallel edges.
		 *
		 *  All the parallel edges in the graph are deleted, only unique representatives are left.
		 *  Three types of parallelism are possible. Depending on \a reltype:
		 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
		 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
		 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
		 *
		 *  \param reltype determines the type of parallelism.
		 *  \return the number of deleted edges.
		 *
		 *  [See example](examples/graph/graph_delAllParals.html).
		 */
		int delAllParals( EdgeType reltype = EdUndir );

		// usuwa krawedzie, ktore przy tym samym ustawieniu parametrow zwracalyby metody getIncEdges. Zwraca liczbe
		// usuniec
		/** \brief Delete incident edges.
		 * WEN: por. komentarz przy findParals
		 *  Deletes edges incident to the vertices from the set defined by the iterators \a beg and \a end.
		 *  Three modes are possible. Depending on the \a kind:
		 *  - if \a kind is congruent with Directed or Undirected, edges with one vertex outside the vertex set are taken.
		 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
		 *  - the option in which mask \a kind is congruent with both the above-mentioned is also available.
		 *
		 *  \param beg the iterator to the first element of the set of vertices.
		 *  \param end the iterator to the WEN: za-last element of the set of vertices.
		 *  \param type the mask determining the direction (relative to vertices in set) of edges which are to be deleted.
		 *  \param kind the mask determining the mode.
		 *  \return the number of deleted edges.
		 */
		template< class Iterator > int delIncEdges( Iterator beg, Iterator end, EdgeDirection type = EdAll, EdgeType kind = Loop );

		/** \brief Delete incident edges.
		 *  WEN: por. komentarz przy findParals
		 *  Deletes edges incident to the vertices from the set \a s.
		 *  Three modes are possible. Depending on the \a kind:
		 *  - if \a kind is congruent with Directed or Undirected, edges with one vertex outside the vertex set are taken.
		 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
		 *  - the option in which mask \a kind is congruent with both the above-mentioned is also available.
		 *
		 *  \param s the set of vertices.
		 *  \param type the mask determining the direction (relative to vertices in set) of edges which are to be deleted.
		 *  \param kind the maks determining the mode.
		 *  \return the number of deleted edges.
		 */
		int delIncEdges( const Set< typename GraphType::PVertex > &s, EdgeDirection type = EdAll, EdgeType kind = Loop );

		// dopelnienia podgrafu indukowanego na zadanych wierzcholkach. Dziala "osobno" na typach krawedzi zawartych
		// w masce wesje z EdInfoGen - typ funktora wyliczajacego pola info nowych krawedzi, wywolanie: EdgeInfo
		// operator()(const GraphType&,PVertex,PVertex,EdgeDirection) wersje z EdgeInfo nadaja tym polom ta sama
		// podana wartosc
		/** \brief Complement of graph.
		 *
		 *  Subgraph induced by the set of vertices is converted to its complement. Only edges with WEN: raczej typ i co to znaczy consistent direction consistent with mask \a direct are taken.
		 *  Any container that has implemented iterator can be used.
		 *  \tparam Iterator the class of iterator for the container storing the set of vertices.
		 *  \tparam EdInfoGen the class generating automatically information for edges. WEN: jak? obiekt funkc. ma miec EdgeInfo operator()(const Graph&,PVertexPVertex,EdDirection)
		 *  \param begin the iterator of the thirst element of the set of vertices.
		 *  \param end the iterator of the WEN: za-last element of the set of vertices. WEN: powtorzenia dozwolone lecz ingnorowane
		 *  \param direct the mask determining the types of the considered edges. WEN: tu raczej trzeba wyjasnic, ze brana jest negacja osobno po Loop, Directed i Undirected
		 *  \param inforGen the automatic EdgeInfo generator for new edges.
		 *  \return none.
		 */
		template< class Iterator, class EdInfoGen > void neg( Iterator begin, Iterator end, EdgeType direct, EdInfoGen infoGen);

		/** \brief Complement of graph.
		 * WEN: jw. tylko bez funktora generujacego infa
		 *  Subgraph induced by the set of vertices is converted to its complement. Only edges with direction consistent with the mask \a direct are taken.
		 *  Any container that has implemented iterator can be used.
		 *  \tparam Iterator the class of the iterator for the container storing the set of vertices.
		 *  \param begin the iterator of the thirst element of the set of vertices.
		 *  \param end the iterator of the WEN: za-last element of the set of vertices.
		 *  \param direct the mask determining the types of considered edges.
		 *  \param info the additional information assigned to the new-created edges.
		 *  \return none.
		 */
		template< class Iterator > void neg( Iterator beg, Iterator end, EdgeType type = Settings::EdAllow,
			EdgeInfo info = EdgeInfo() )
			{ neg( beg,end,type,ConstFunctor< EdgeInfo >( info ) ); }

		/** \brief Complement of graph.
		 * WEN: jak 2 metody wyzej, tylko bez powtorzen
		 *  Subgraph induced by the set of vertices is converted to its complement. Only edges with direction consistent with the mask \a direct are taken.
		 *  The set of vertices is given by \a vset.
		 *  \tparam EdInfoGen the class generating automatically information for edges.
		 *  \param vset the set of vertices.
		 *  \param direct the mask determining the types of the considered edges.
		 *  \param infoGen the additional information assigned to new edges.
		 *  \return none.
		 */
		template< class EdInfoGen > void neg( const Set< PVertex > &vset, EdgeType type, EdInfoGen infoGen )
			{ neg( vset.begin(),vset.end(),type,infoGen ); }

		/** \brief Complement of graph.
		 * WEN: jak 2 metody wyzej, tylko bez powtorzen
		 *  Subgraph induced by the set of vertices is converted to its complement. Only edges with direction consistent with the mask \a direct are taken.
		 *  \param vset the set of vertices which induce the subgraph that is going to be converted into its complement.
		 *  \param direct the mask determining the types of considered edges.
		 *  \param inforGen the automatic EdgeInfo generator for new edges.
		 *  \return none.
		 */
		void neg( const Set< PVertex > &vset, EdgeType direc = Settings::EdAllow, EdgeInfo infoGen = EdgeInfo() );

		// ... i dopelnienie calego grafu
		/** \brief Complement of graph.
		 * WEN: jw.
		 *  Graph is converted to its complement. Only edges with direction consistent with mask \a type are considered.
		 *  \tparam EdInfoGen class generating automatically information for edges.
		 *  \param type mask determining the types of edges that are considered.
		 *  \param infoGen automatic EdgeInfo generator for new edges.
		 *  \return none.
		 */
		template< class EdInfoGen > void neg( EdgeType type, EdInfoGen infoGen );

		/** \brief Complement of graph.
		 * WEN:jw.
		 *  Graph is converted to its complement. Only edges with type consistent with the mask \a type are considered.
		 *  \param type the mask determining the types of edges that are considered.
		 *  \param info the information assigned to new edges.
		 *  \return none.
		 */
		void neg( EdgeType type = Settings::EdAllow, EdgeInfo info = EdgeInfo() )
			{ neg( type,ConstFunctor< EdgeInfo >() ); }

		// Kopiowanie do naszego innego grafu (np. innego typu). Wierzcholki dotychczasowe pozostaja na liscie
		// przed skopiowanymi. W najogolniejszej wersji mozna podac
		// - pare chooserow wybierajacych podgraf wierzcholkow i krawedzi do skopiowania (konce krawedzi takze musza
		// spelniac chooser wierzcholkowy)
		// - pare casterow wyliczajacych infa nowych wierzcholkow i krawedzi na podstawie oryginalow
		// - pare linkerow (dla wierzcholkow i krawedzi) ustalajacych polaczenia miedzy oryginalami i kopiami.
		// Elementy dotychczasowe tego grafu oraz nieskopiowane grafu kopiowanego sa linkowane z NULL
		// Zwraca pierwszy wprowadzony wierzcholek

		/** \brief Copy graph.
		 * WEN: nie zmienia ew. posiadania macierzy sasiedztwa
		 *  The method adds to the current graph a copy of the graph \a agraph. WEN: wierzcholki nowe dodawane na koncu jako nowa czesc
		 *  \param agraph the copied graph.
		 *  \return the pointer to the first new-created vertex.
		 */
		template< class ExtGraph > typename GraphType::PVertex copy( const ExtGraph &agraph );

        //NEW: wersja z HardCaster zamiast standadowego, podobnie w substitute
		template< class ExtGraph > typename GraphType::PVertex copy2( const ExtGraph &agraph );

		/** \brief Copy graph.
		 * WEN: nie zmienia ew. posiadania macierzy sasiedztwa
		 *  The method adds to the current graph a copy of the graph \a agraph. WEN: wierzcholki nowe dodawane na koncu jako nowa czesc
		 *  \param agraph the copied graph.
		 *  \param choosers the standard pair of choosers which allow to choose vertices and edges to copy. Additionally both ends of each copied edge need to satisfy the VChooser. WEN: jakis link do ogolnego opisu chooserow
		 *  \return the pointer to the first new-created vertex.
		 */

		template< class ExtGraph, class VChooser, class EChooser >
			typename GraphType::PVertex copy( const ExtGraph &agraph, std::pair< VChooser,EChooser > choosers );

		template< class ExtGraph, class VChooser, class EChooser >
			typename GraphType::PVertex copy2( const ExtGraph &agraph, std::pair< VChooser,EChooser > choosers );


		/** \brief Copy graph.
		 * WEN: nie zmienia ew. posiadania macierzy sasiedztwa
		 *  The method adds to the current graph a copy of the graph \a agraph.
		 *  \param agraph the copied graph. WEN: wierzcholki nowe dodawane na koncu jako nowa czesc
		 *  \param choosers the standard pair of choosers which allow to choose vertices and edges to copy. Additionally both ends of each copied edge need to satisfy the VChooser. See \ref DMchooser. WEN: jakis link do ogolnego opisu chooserow
		 *  \param casters the standard pair of methods allowing to generate a vertex info and an edge info automatically basing on the infos in the added graph \a agraph. See \ref DMcaster.
		 *  \return the pointer to the first new-created vertex.
		 */
		template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster >
			typename GraphType::PVertex copy( const ExtGraph &agraph, std::pair< VChooser,EChooser > choosers,
				std::pair< VCaster,ECaster > casters );

		/** \brief Copy graph.
		 *
		 *  The method adds to the current graph a copy of the graph \a agraph.
		 *  \param agraph the copied graph. WEN: wierzcholki nowe dodawane na koncu jako nowa czesc
		 *  \param choosers the standard pair of choosers which allow to choose vertices and edges to copy. Additionally both ends of each copied edge need to satisfy the VChooser. See \ref DMchooser.
		 *  \param casters the standard pair of methods allowing to generate a vertex info and an edge info automatically basing on the infos in the added graph \a agraph. WEN: jakis link do ogolnego opisu casterow
		 *  \param linkers the standard pair of linkers, which are the objects that define the way of joining the initial entities with the copied ones. For more details see \ref DMlinker.
		 *  \return the pointer to the first new-created vertex.
		 */
		template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster, class VLinker,
			class ELinker > typename GraphType::PVertex copy( ExtGraph &, std::pair< VChooser,EChooser >,
				std::pair< VCaster,ECaster >, std::pair< VLinker,ELinker > linkers );

		// Podstawienie kopii drugiego grafu za dotychczasowy wierzcholek (ktory jest usuwany). Znaczenie parametrow
		// parametry podobne jak w copy
		// Zwraca pierwszy wprowadzony wierzcholek
		/** \brief Substitute graph for vertex.
		 * WEN: we wszystkich substitute te same uwagi, co w copy oraz: graf w argumencie nie moze byc naszym. Typ/orientacja i info krawedzi z czesci nowej do starej sa dziedziczone po wzorcowej krawedzi z vert do czesci starej, ale ew. petle przy wierzcholku podstawianym znikaja
		 *  The method substitute a copy of the graph \a graph for the vertex \a vert. The vertex \a vert is deleted form graph.
		 *  \param vert the substituted vertex
		 *  \param graph the copied graph.
		 *  \return the pointer to the first new-created vertex. */
		template< class ExtGraph > typename GraphType::PVertex substitute( typename GraphType::PVertex, const ExtGraph & );

		template< class ExtGraph > typename GraphType::PVertex substitute2( typename GraphType::PVertex, const ExtGraph & );
		/** \brief Substitute graph for vertex.
		 *
		 *  The method substitute a copy of the graph \a graph for the vertex \a vert. The vertex \a vert is deleted form graph.
		 *  \param vert the substituted vertex
		 *  \param graph the copied graph.
		 *  \param choosers the standard pair of choosers, which allow to choose vertices and edges to copy. Additionally both ends of each copied edge need to satisfy the VChooser. See \ref DMchooser.
		 *  \return the pointer to the first new-created vertex. */
		template< class ExtGraph, class VChooser, class EChooser >
			typename GraphType::PVertex substitute( PVertex, const ExtGraph &, std::pair< VChooser,EChooser > );

		template< class ExtGraph, class VChooser, class EChooser >
			typename GraphType::PVertex substitute2( PVertex, const ExtGraph &, std::pair< VChooser,EChooser > );


		/** \brief Substitute graph for vertex.
		 *
		 *  The method substitute a copy of the graph \a graph for the vertex \a vert. The vertex \a vert is deleted form graph.
		 *  \param vert the substituted vertex
		 *  \param graph the copied graph.
		 *  \param choosers the standard pair of choosers, which allow to choose vertices and edges to copy. Additionally both ends of each copied edge need to satisfy the VChooser. See \ref DMchooser.
		 *  \param casters the standard pair of methods allowing to generate a vertex info and an edge info automatically basing on the infos in the copied graph \a agraph. See \ref DMcaster.
		 *  \return the pointer to the first new-created vertex. */
		template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster >
			typename GraphType::PVertex substitute( PVertex, const ExtGraph &, std::pair< VChooser,EChooser >,
				std::pair< VCaster,ECaster > );

		/** \brief Substitute graph for vertex.
		 *
		 *  The method substitute a copy of the graph \a graph for the vertex \a vert. The vertex \a vert is deleted form graph.
		 *  \param vert the substituted vertex
		 *  \param graph the copied graph.
		 *  \param choosers the standard pair of choosers, which allow to choose vertices and edges to copy. Additionally both ends of each copied edge need to satisfy the VChooser. See \ref DMchooser.
		 *  \param casters the standard pair of methods allowing to generate a vertex info and an edge info automatically basing on the infos in the copied graph \a agraph. See \ref DMcaster.
		 *  \param linkers the standard pair of linkers, which are the objects that define the way of joining the initial entities with the copied ones. For more details see \ref DMlinker.
		 *  \return the pointer to the first new-created vertex. */
		template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster, class VLinker,
			class ELinker > typename GraphType::PVertex substitute( typename GraphType::PVertex vert, ExtGraph &graph,
				std::pair< VChooser,EChooser > choosers, std::pair< VCaster,ECaster > casters, std::pair< VLinker,ELinker > linkers );

		// Operacje na macierzy sąsiedztwa
		// Tworzymy macierz sąsiedztwa.
		/** \brief Make adjacency matrix.
		 *
		 *  The adjacency matrix is created. For some algorithmic purposes it is better to use the adjacency matrix. WEN: mozna podac dokladnie w ktorych, jest jedna taka lista
		 *  \return true if successful, false otherwise (if adjacency matrix is not allowed or already exists).
		 */
		bool makeAdjMatrix();

		// Usuwamy macierz sąsiedztwa.
		/** \brief Delete adjacency matrix.
		 * WEN: tylko ta i nastepna metoda moze zmiec stan posiadania macierzy sasiedztwa
		 *  The adjacency matrix is deleted.
		 *  \return true if successful, false otherwise (if adjacency matrix does not exist).
		 */
		inline bool delAdjMatrix();

		// Informacja o tym, czy graf ma macierz sąsiedztwa.
		/** \brief Check the existence of adjacency matrix.
		 *
		 *  Test whether the adjacency matrix exists.
		 *  \return true if there is an adjacency matrix, false otherwise.
		 *
		 *  [See example](examples/graph/graph_adjmatrix.html).
		 */
		inline bool hasAdjMatrix() const;

		// czy ten typ grafu obsluguje macierz sasiedztwa
		/** \brief Check if adjacency matrix is allowed.
		 *
		 *  Test whether the adjacency matrix is allowed. WEN: by Settings tj. typ grafu
		 *  \return true if an adjacency matrix is allowed, false otherwise.
		 *
		 *  [See example](examples/graph/graph_adjmatrix.html).
		 */
		static bool allowedAdjMatrix()
			{ return Settings::AdjMatrixAllowed; }
		// alokacja pamieci na podana liczbe wierzcholkow
		/** \brief Reserve memory for adjacency matrix.
		 * WEN: w razie braku macierzy nic nie robi
		 *  The method allows to allocate sufficient memory area for adjacency matrix, thanks to which excessive reallocations may be avoided.
		 *  \param size possible dimension of matrix which fits into reserved area.
		 *
		 *  [See example](examples/graph/graph_adjmatrix.html).
		 */
		inline void reserveAdjMatrix( int size );

		// do wspolpracy z hierarchia podgrafow i widokow.
		/** \brief Get root graph
		 *
		 *  The method used by views.*/
		const Graph< VertInfo,EdgeInfo,Settings > *getRootPtr() const { return this; }

		/** \brief Get root graph
		 *
		 *  The method used by views.*/
		const Graph< VertInfo,EdgeInfo,Settings >& root() const
			{ return *this; }

		/** \brief Check vertex.
		 *
		 *  The method used by views.*/
		bool good( PVertex, bool = false ) const
			{ return true; }

		/** \brief Check vertex.
		 *
		 *  The method used by views.*/
		bool good( PEdge, bool = false ) const
			{ return true; }

        //NEW: adresy uzywanych alokatorow wierzcholkow/krawedzi - 0 przy ich braku
        std::pair<  SimplArrPool<Koala::Vertex< VertInfo,EdgeInfo,Settings > > *,
                    SimplArrPool<Koala::Edge< VertInfo,EdgeInfo,Settings > > *>
        getAllocs() const
        {
            return std::make_pair(vallocator,eallocator);
        }

	private:
		AdjMatrix< VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed > *pAdj;

		PVertex first_vert,last_vert;
		PEdge first_edge,last_edge;

		typename Privates::EdgeCounterLoop< Settings::EdAllow &Loop >::NoType
			no_loop_edge() const { return this->Privates::EdgeCounterLoop< Settings::EdAllow &Loop >::no(); }
		typename Privates::EdgeCounterDir< Settings::EdAllow & (EdDirIn|EdDirOut) >::NoType
			no_dir_edge() const { return this->Privates::EdgeCounterDir< Settings::EdAllow & (EdDirIn|EdDirOut) >::no(); }
		typename Privates::EdgeCounterUndir< Settings::EdAllow & Undirected >::NoType
			no_undir_edge() const { return this->Privates::EdgeCounterUndir< Settings::EdAllow & Undirected >::no(); }
		int no_vert;

		SimplArrPool<Koala::Vertex< VertInfo,EdgeInfo,Settings > > *vallocator;
		SimplArrPool<Koala::Edge< VertInfo,EdgeInfo,Settings > > *eallocator;

		template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster, class VLinker,
			class ELinker > typename GraphType::PVertex copy( ExtGraph &, std::pair< VChooser,EChooser >,
				std::pair< VCaster,ECaster >, std::pair< VLinker,ELinker > linkers, PVertex );

		inline typename GraphType::PVertex attach( PVertex );/* < Vertex is attached at the beginning of the list of vertex. */
		inline typename GraphType::PVertex detach( PVertex );/* < If vertex is isolated */
		inline typename GraphType::PEdge attach( PEdge, PVertex, PVertex, EdgeDirection );
		typename GraphType::PEdge attach_undir( PEdge, PVertex, PVertex );
		typename GraphType::PEdge attach_dir( PEdge, PVertex, PVertex );
		typename GraphType::PEdge attach_loop( PEdge, PVertex );
		typename GraphType::PEdge detach( PEdge );
	} ;

#include "graph.hpp"
}

#endif
