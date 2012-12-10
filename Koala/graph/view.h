#ifndef KOALA_SUBGRAPH_H
#define KOALA_SUBGRAPH_H

//Widoki na graf pozwalaja operowac na czesci danego grafu lub na jego przeksztalconej formie
//jak na zwyklym grafie stalym (const) bez wykonywania jego kopii. Metody widoku odwoluja sie
//do grafu macierzystego i dzialaja na jego wierz/kraw. Mozna tworzych hierarchicznie widoki
//do widokow itd.
// Widoki dzialaja poprawnie, o ile ich struktury nadrzedne nadal zyja.
// Smierc ktorejs ze struktur nadrzednych powoduje odlaczenie od niej wszystkich dzieci,
// niemniej nizsze hierarchiczne powiazania utworzone przez te obiekty pozostaja
// (np. ich ponowne podlaczenie do innego grafu sprawi, ze nizej podpiete widoki beda dzialac).

#include "graph.h"

namespace Koala
{
	/* SubGraph
	 *
	 */
	template< class Graph, class VChooser, class EChooser > class Subgraph;

	template< class GraphType > struct GraphInternalTypes;
	template< class Graph, class VChooser, class EChooser >
		struct GraphInternalTypes< Subgraph < Graph, VChooser, EChooser> >
	{
		typedef typename Graph::Vertex Vertex;
		typedef typename Graph::PVertex PVertex;
		typedef typename Graph::Edge Edge;
		typedef typename Graph::PEdge PEdge;
		typedef typename Graph::VertInfoType VertInfoType;
		typedef typename Graph::EdgeInfoType EdgeInfoType;
		typedef typename Graph::GraphSettings GraphSettings;
	};

	// Klasa podgrafu struktury grafowej typu Graph, do podgrafu wybierane sa wierzcholki spelniajace
	// chooser vchoose i krawedzie spelniajace echoose z obydwoma koncami spelniajacymi vchoose
	/** \brief Subgraph (view).
	 *
	 *  Class allows to isolate and use only part of the graph without allocating new graph. The original graph is called and there is no need to create a copy.
	 *  Classes VChooser and EChooser allow to choose vertex and edges for subgraph.
	 *  \tparam Graph the type of graph
	 *  \tparam VChooser the class allowing to choose vertices automatically.
	 *  \tparam EChooser the class allowing to choose edges automatically.
	 *  \ingroup DMview */
	template< class Graph, class VChooser, class EChooser > class Subgraph:
		public SubgraphBase, public ConstGraphMethods< Subgraph< Graph, VChooser, EChooser> >
	{
	public:
		// choosery predykatow definiujacych podgraf
		/** \brief Chooser object for vertices.
		 *
		 *  The object function defines the vertices in graph. And only vertices that satisfy the chooser are visible in the subgraph. For more details about choosers see \ref DMchooser.*/
		mutable VChooser vchoose;
		/**\brief Chooser object for edges.
		 *
		 *  The object function defines the edges in graph. And only edges that satisfy the chooser are visible in the subgraph. For more details about choosers see \ref DMchooser.*/
		mutable EChooser echoose;

		// te typy sa takie same, jak w grafie nadrzednym
		typedef typename GraphInternalTypes< Subgraph< Graph, VChooser, EChooser> >::Vertex Vertex; /**< \brief Vertex of graph.*/
		typedef typename GraphInternalTypes< Subgraph< Graph, VChooser, EChooser > >::PVertex PVertex; /**< \brief Pointer to vertex of graph.*/
		typedef typename GraphInternalTypes< Subgraph< Graph, VChooser, EChooser > >::Edge Edge; /**< \brief Edge of graph.*/
		typedef typename GraphInternalTypes< Subgraph< Graph, VChooser, EChooser > >::PEdge PEdge; /**< \brief Pointer to edge of graph.*/
		typedef typename GraphInternalTypes< Subgraph< Graph, VChooser, EChooser > >::VertInfoType VertInfoType; /**< \brief Vertex information.*/
		typedef typename GraphInternalTypes< Subgraph< Graph, VChooser, EChooser > >::EdgeInfoType EdgeInfoType; /**< \brief Edge information.*/
		typedef typename GraphInternalTypes< Subgraph< Graph, VChooser, EChooser> >::GraphSettings GraphSettings; /**< \brief Graph settings.*/

		// typ tej struktury
		typedef Subgraph< Graph,VChooser,EChooser > GraphType; /**< \brief The current graph type.*/
		// typ najwyzszego wierzcholka hierarchii
		typedef typename Graph::RootGrType RootGrType; /**< \brief Root (initial) graph type.*/
		// typ grafu nadrzednego
		typedef Graph ParentGrType; /**< \brief Superior (parent) graph type.*/

		// Konstruktory
		// tworzy podgraf nie podlaczony do zadnego grafu
		/** \brief Constructor
		 *
		 *  New subgraph is created without any connection to a graph.	 */
		Subgraph()
			{ }
		// tworzy podgraf danego grafu i ustawia wartosci obu chooserow
		/** \brief Constructor
		 *
		 *  New subgraph of \a g is created. The method assigns the attributes \a vchoose and \a echoose that determine the vertices and edges of subgraph. See \ref DMchooser.
		 *  \param g the parent graph (or subgraph).
		 *  \param chs standard pair of choosers, first of which choose vertices second edges.   */
		Subgraph( const Graph &g, std::pair< VChooser,EChooser > chs = std::make_pair( VChooser(),EChooser() ) );

		// ustawia wartosci obu chooserow, tworzy podgraf niepodlaczony
		/** \brief Constructor
		 *
		 *  New unconnected subgraph is created. The method assigns the attributes \a vchoose and \a echoose that determine the vertices and edges of subgraph. See \ref DMchooser.
		 *  \param chs standard pair of choosers, first of which choose vertices second edges. 	 */
		Subgraph( std::pair< VChooser,EChooser > );

		// ustawienie wartosci chooserow
		/** \brief Set choose.
		 *
		 *  The method assigns the attributes \a vchoose and \a echoose which determine the vertices and edges of subgraph. See \ref DMchooser.
		 *  \param chs reference to standard pair of choosers, first of which choose vertices second edges.  */
		void setChoose( const std::pair< VChooser,EChooser > &chs );

		// dopuszczalne typy krawedzi
		/** \brief Check allowed edge types.
		 *
		 *  \returns allowed types (EdgeType) of edges in the root graph. */
		 static EdgeType allowedEdgeTypes()
			{ return ParentGrType::allowedEdgeTypes(); }

		// rozlacza sie od rodzica (jesli taki istnial) i podlacza do podanego grafu
		/** \brief Plug to \a g
		 *
		 * The method plugs the current graph as a child to \a g.*/
		void plug( const Graph &g )
			{ SubgraphBase::link( &g ); }
		// sprawia, ze podgraf staje sie niepodlaczony do zadnego rodzica
		/** \brief Unplug graph.
		*
		*  The method unplug the current subgraph from the parent.
		*  \return true if the parent existed, false otherwise.  */
		bool unplug()
			{ return SubgraphBase::unlink(); }
		// adres najwyzszej struktury hierarchii lub NULL w razie braku
		/** \brief Get root graph.
		*
		*  The method tests if the graph has any superior graph if true gets the top graph in hierarchy of graphs.
		*  \return the pointer to the root if the parent existed, NULL otherwise. */
		const RootGrType *getRootPtr() const
			{ return parent ? ((const ParentGrType*)parent)->getRootPtr() : NULL; }
		// adres grafu, do ktorego obiekt jest bezposrednio podlaczony, lub NULL w razie braku
		/** \brief Get parent graph.
		*
		*  The method tests if the graph has any superior graph if true gets the parent graph .
		*  \return the pointer to the parent if it existed, NULL otherwise. */
		const ParentGrType *getParentPtr() const
			{ return (const ParentGrType*)parent; }
		// j.w. ale referencje
		/** \brief Get root graph.
		*
		*  The method tests if the graph has any superior graph if true gets the top graph in hierarchy of graphs.
		*  \return the reference to the root if the parent existed, NULL otherwise. */
		const RootGrType &root() const;

		/** \brief Get parent graph.
		*
		*  The method tests if the graph has any superior graph if true gets the parent graph .
		*  \return the reference to the parent if it existed, NULL otherwise. */
		const ParentGrType &up() const;

		// Test przynaleznosci wierzcholka z rodzica do podgrafu. Dla NULL zawsze prawda.
		// w przeciwnym razie sprawdzany jest predykat vchoose. Flaga true wymusza tez analogiczne sprawdzanie we
		// wszystkich strukturach az do korzenia
		/** \brief Check vertex presence.
		 *
		 *  The method tests if the vertex form ancestor belongs to the current subgraph i.e. if it satisfy the \a vchoose of current subgraph. If the flag \a deep is set to true all the ancestors choosers are tested.
		 *  \param vert the tested vertex.
		 *  \param deep the flag determining if all choosers of ancestors are checked.
		 *  \return true if vertex belongs to subgraph, false otherwise.*/
		bool good( PVertex vert, bool deep = false ) const;

		// j.w. ale dla krawedzi - sprawdzany jest predykat echoose i vchoose dla obu koncow
		/** \brief Check edge presence.
		 *
		 *  The method tests if the edge form ancestor belongs to the current subgraph i.e. if it satisfy the \a echoose of current subgraph and both ends satisfy \a vchoose. If the flag \a deep is set to true all the ancestors choosers are tested.
		 *  \param edge the tested edge.
		 *  \param deep the flag determining if all choosers of ancestors are checked.
		 *  \return true if edge belongs to subgraph, false otherwise.*/
		bool good( PEdge edge, bool deep = false ) const;

		// metody dla ConstGraphMethods
		//------------- Methods sent to ConstGraphMethods --------------------------------------

		/** \brief Get number of vertices.
		 *
		 *  Gets the order of the graph.
		 *  \return the number of vertices in graph.	 */
		int getVertNo() const;

		/** \brief Get next vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the next vertex on the list.
		 *  If parameter \a vert is set to NULL then the first vertex on the list will be taken.
		 *  \param vert the reference vertex.
		 *  \returns a pointer to the next vertex on the vertex list or NULL if the vertex was last. */
		PVertex getVertNext( PVertex vert ) const;

		/** \brief Get previous vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the vertex prior to the one pointed by PVertex.
		 *  If parameter \a vert is set to NULL then the last vertex on the list will be taken.
		 *  \param vert the reference vertex.
		 *  \returns a pointer to the previous vertex on the vertex list or NULL if the vertex was the first. */
		PVertex getVertPrev( PVertex vert ) const;

		/** \brief Get edge number.
		 *
		 *  The method gets the number of edges of type determined by the parameter \a direct.
		 *  \param direct the mask representing all types of the considered edges.
		 *  \returns the number of edges of certain type. */
		int getEdgeNo( EdgeDirection direct = EdAll ) const;

		/** \brief Get next edge .
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask \a direct. The method gets the pointer to the edge next to \a e.
		 *  If parameter e is set to NULL then the first edge on the list is taken.
		 *  \param e the reference edge.
		 *  \param direct the mask representing all the types of considered edges.
		 *  \returns pointer to the next edge or if \a e is the last edge then NULL. */
		PEdge getEdgeNext( PEdge e, EdgeDirection direct = EdAll ) const;

		/** \brief Get previous edge.
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask direct. The method gets the pointer to the edge previous to \a edge.
		 *  If parameter \a edge is set to NULL then the last edge on the list will be taken.
		 *  \param edge the reference edge.
		 *  \param direct the mask representing all the types of considered edges.
		 *  \returns pointer to the previous edge or if edge is the first edge then NULL.*/
		PEdge getEdgePrev( PEdge edge, EdgeDirection direct = EdAll ) const;

		/** \brief Get next edge.
		 *
		 *  The method allows to see through all the edges incident to \a vert, of direction congruent with the mask \a direct.
		 *  For each vertex the edges incident to it form a list. The method gets the pointer to the edge next to \a e.
		 *  If the parameter \a e is set to NULL then the first edge on the list is taken.
		 *  \param vert only the edges incident to \a vert are considered.
		 *  \param e the reference edge.
		 *  \param direct the mask representing the types of edges.
		 *  \returns the pointer to the next edge or if the edge is the last edge then NULL.
		 */
		PEdge getEdgeNext( PVertex vert, PEdge e, EdgeDirection direct = EdAll ) const;

		/** \brief Get previous edge
		 *
		 *  The method allows to see through all the edges incident to \a vert, of direction congruent with the mask \a direct. The method gets the pointer to the edge previous to \a ed.
		 *  If the parameter \a ed is set to NULL then the last edge on the list will be returned.
		 *  \param vert the reference vertex.
		 *  \param ed the reference edge.
		 *  \param direct the mask representing the types of edges.
		 *  \returns Pointer to the previous edge or if the edge is the first then NULL.
		 */
		PEdge getEdgePrev( PVertex vert, PEdge ed, EdgeDirection direct = EdAll ) const;

		/** \brief Get vertex degree.
		 *
		 *  Gets the number of edges incident to the vertex of direction (with respect to the vertex \a vert) prespecified by the mask direct.
		 *  \param vert the pointer to the reference vertex.
		 *  \param direct the mask representing the direction of considered edges.
		 *  \returns the number of edges directed as required in \a direct. */
		int getEdgeNo( PVertex vert, EdgeDirection direct = EdAll) const;

		/** \brief Get next parallel edges.
		 *
		 *  The pointer to the next parallel edge is returned. The mask \a direct limits considered edges. If adjacency matrix is allowed the method will use it, otherwise lists are searched through.
		 *  If the parameter \a ed is set to NULL then the first edge on the list will be taken.
		 *  \param vert1 the first vertex.
		 *  \param vert2 the second vertex.
		 *  \param ed the reference edge
		 *  \param direct the mask representing the direction of considered edges.
		 *  \returns the pointer to the next parallel edge or NULL if \a ed is the last. */
		PEdge getEdgeNext( PVertex vert1, PVertex vert2, PEdge ed, EdgeDirection direct = EdAll ) const;

		/** \brief Get previous parallel edges.
		 *
		 *  The pointer to the parallel edge previous to \a ed is returned. The mask limiting considered edges is possible.
		 *  If the adjacency matrix is allowed the method will use it, otherwise only lists are checked.
		 *  \param vert1 the first vertex.
		 *  \param vert2 the second vertex.
		 *  \param ed the reference edge.
		 *  \param direct the mask representing direction of the considered edges.
		 *  \returns the pointer to the previous parallel edge or NULL if \a ed is the first edge. */
		PEdge getEdgePrev( PVertex vert1, PVertex vert2, PEdge ed, EdgeDirection direct = EdAll ) const;

		/** \brief Get number of parallel edges.
		 *
		 *  The method counts the number of edges between two vertices. Only edges directed in the way consistent with the mask \a direct are considered.
		 *  \param vert1 the first vertex
		 *  \param vert2 the second vertex
		 *  \param direct the mask representing the direction of considered edges.
		 *  \returns the number of edges between \a vert1 and \a vert2. */
		int getEdgeNo( PVertex vert1, PVertex vert2, EdgeDirection direct = EdAll ) const;

		/** \brief Get edge type.
		 *
		 *  \param e the pointer to considered edge.
		 *  \return the Koala::EdgeType value which is a mask representing the type of edge.
		 *  \sa Koala::EdgeType */
		EdgeType getEdgeType( PEdge e ) const
			{ return up().getEdgeType( e ); }

		/** \brief Get edge ends
		 *
		 *  The method gets the pair of vertices on which the edge is spanned.
		 *  \param edge the considered edge.
		 *  \returns the pair of the vertices that are the ends of the edge.	 */
		std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const
			{ return up().getEdgeEnds( edge ); }

		/** \brief Get the first vertex.
		 *
		 *  \param edge the considered edge.
		 *  \returns the pointer to the first vertex of the \a edge.  */
		PVertex getEdgeEnd1( PEdge edge ) const
			{ return up().getEdgeEnd1( edge ); }

		/** \brief Get the second vertex.
		 *
		 *  \param edge the considered edge
		 *  \returns the pointer to the second vertex of the \a edge. */
		PVertex getEdgeEnd2( PEdge edge ) const
			{ return up().getEdgeEnd2( edge ); }

		/** \brief Get arc direction
		 *
		 *  The method gets the edge direction. Possible values of EdgeDirection are:
		 *  - EdNone   = 0x00 if the edge is NULL,
		 *  - EdLoop   = 0x01 if the edge is a loop,
		 *  - EdUndir  = 0x02 if the edge is undirected,
		 *  - EdDirIn  = 0x04 if the edge is directed and \a v is the second vertex of the edge,
		 *  - EdDirOut = 0x08 if the edge is directed and \a v is the first vertex of the edge.
		 *  \param edge considered edge.
		 *  \param v reference vertex.
		 *  \returns direction of edge \a edge. */
		EdgeDirection getEdgeDir( PEdge edge, PVertex v) const
			{ return up().getEdgeDir( edge,v ); }

	protected:
		template <class T> static bool isEdgeTypeChooser( const T &x, Koala::EdgeDirection &val )
			{ return false; }
		static bool isEdgeTypeChooser( const EdgeTypeChooser &x, Koala::EdgeDirection &val );

		template <class T> static bool isBoolChooser( const T &x, bool &val )
			{ (void)(x); (void)(val); return false; }
		static bool isBoolChooser( const BoolChooser &x, bool &val );
	};

	// zwraca podgraf danego grafu
	/** \brief Subgraph generating function.
	 *
	 *  For a given graph \a g and a pair of choosers (vertex chooser and edge chooser) view on graph is returned.
	 *  \param g the considered graph.
	 *  \param chs the standard pair of choosers, the first one chooses vertices to view, the second one chooses edges (both ends of edge need to satisfy  vertex chooser) \ref DMchooser.
	 *  \return the new-created view (subgraph) on graph (view).
	 *  \ingroup DMview */
	template< class Graph, class VChooser, class EChooser > Subgraph< Graph,VChooser,EChooser >
		makeSubgraph( const Graph &, const std::pair< VChooser,EChooser > & );

		template< class Graph > class UndirView;
		template< class GraphType > struct GraphInternalTypes;
		template< class Graph  > struct GraphInternalTypes< UndirView<Graph> >
		{
			typedef typename Graph::Vertex Vertex;
			typedef typename Graph::PVertex PVertex;
			typedef typename Graph::Edge Edge;
			typedef typename Graph::PEdge PEdge;
			typedef typename Graph::VertInfoType VertInfoType;
			typedef typename Graph::EdgeInfoType EdgeInfoType;
			typedef typename Graph::GraphSettings GraphSettings;
		};

		/* UndirView
		 * widok na graf typu Graph, przy czym wszystkie krawedzie rozne od petli sa widziane jako nieskierowane
		 */
	/** \brief Undirected view.
	 *
	 *  The class allows to create the view on graph in which all the edges (except loops) are undirected. The class let us use the graph as undirected, but without allocation of new graph. The interface (except the process of creation) is the same as in Koala::Subgraph.
	 *  \ingroup DMview */
	template< class Graph > class UndirView: public SubgraphBase, public ConstGraphMethods< UndirView< Graph> >
	{
	public:
		// ten sam sens typow, co dla podgrafu
		typedef typename GraphInternalTypes< UndirView< Graph > >::Vertex Vertex; /**< \brief Vertex of graph.*/
		typedef typename GraphInternalTypes< UndirView< Graph > >::PVertex PVertex;/**< \brief Pointer to vertex of graph.*/
		typedef typename GraphInternalTypes< UndirView< Graph > >::Edge Edge;/**< \brief Edge of graph.*/
		typedef typename GraphInternalTypes< UndirView< Graph > >::PEdge PEdge;/**< \brief Pointer to edge of graph.*/
		typedef typename GraphInternalTypes< UndirView< Graph > >::VertInfoType VertInfoType; /**< \brief Vertex information.*/
		typedef typename GraphInternalTypes< UndirView< Graph > >::EdgeInfoType EdgeInfoType;/**< \brief Edge information.*/
		typedef typename GraphInternalTypes< UndirView< Graph > >::GraphSettings GraphSettings;/**< \brief Graph settings.*/

		typedef UndirView< Graph > GraphType;/**< \brief The current graph (view) type.*/
		typedef typename Graph::RootGrType RootGrType;  /**< \brief Root (initial) graph type.*/
		typedef Graph ParentGrType; /**< \brief Superior (parent) graph type.*/

		// Konstruktory
		// tworzy widok niepodlaczony do zadnej struktury
		/** \brief Constructor.
		 *
		 *  Undirected view on graph is created but it isn't connected to any graph.*/
		UndirView()
			{ }
		/** \brief Constructor.
		 *
		 *  Undirected view on the graph \a g is created.*/
		UndirView( const Graph &g )
			{ SubgraphBase::link( &g ); }

		/** \brief Check allowed edge types.
		 *
		 *  \returns allowed types (EdgeType) of edges in the root graph. */
		static EdgeType allowedEdgeTypes()
			{ return (((~EdLoop)&ParentGrType::allowedEdgeTypes()) ? Undirected :0 )
			| ((EdLoop&ParentGrType::allowedEdgeTypes()) ? EdLoop : 0 ); }

		/** \brief Get root graph.
		*
		*  The method tests if the graph has any superior graph if true gets the top graph in hierarchy of graphs.
		*  \return the pointer to the root if the parent existed, NULL otherwise. */
		const RootGrType* getRootPtr() const
			{ return parent ? ((const ParentGrType *)parent)->getRootPtr() : NULL; }
		/** \brief Get parent graph.
		*
		*  The method tests if the graph has any superior graph if true gets the parent graph .
		*  \return the pointer to the parent if it existed, NULL otherwise. */
		const ParentGrType* getParentPtr() const
			{ return (const ParentGrType*)parent; }
		/** \brief Get parent graph.
		*
		*  The method tests if the graph has any superior graph if true gets the parent graph .
		*  \return the reference to the parent if it existed, NULL otherwise. */
		const ParentGrType &up() const;
		 /** \brief Get root graph.
		*
		*  The method tests if the graph has any superior graph if true gets the top graph in hierarchy of graphs.
		*  \return the reference to the root if the parent existed, NULL otherwise. */
		const RootGrType &root() const;
		// rozlacza obiekt od jego rodzica (jesli taki istnial) i podlacza do podanego grafu
		/** \brief Plug to \a g
		 *
		 *  The method plugs the current view as a child of \a g. If the view was plugged to another view it is unplugged first.
		 *  \param g the new parent.*/
		void plug( const Graph &g )
			{ SubgraphBase::link( &g ); }
		// sprawia, ze podgraf staje sie niepodlaczony do zadnego rodzica
		/** \brief Unplug graph.
		*
		*  The method unplug the current subgraph from the parent.
		*  \return true if the parent existed, false otherwise.  */
		bool unplug()
			{ return SubgraphBase::unlink(); }

		// do wspolpracy z podgrafami
		/** \brief Check vertex presence.
		 *
		 *  The method tests if the vertex form ancestor belongs to the current subgraph i.e. if it satisfy the \a vchoose of current subgraph. If the flag \a deep is set to true all the ancestors choosers are tested.
		 *  \param vert the tested vertex.
		 *  \param deep the flag determining if all choosers of ancestors are checked.
		 *  \return true if vertex belongs to subgraph, false otherwise.*/
		bool good( PVertex, bool = false ) const
			{ return true; }
		/** \brief Check edge presence.
		 *
		 *  The method tests if the edge form ancestor belongs to the current subgraph i.e. if it satisfy the \a echoose of current subgraph and both ends satisfy \a vchoose. If the flag \a deep is set to true all the ancestors choosers are tested.
		 *  \param edge the tested edge.
		 *  \param deep the flag determining if all choosers of ancestors are checked.
		 *  \return true if edge belongs to subgraph, false otherwise.*/
		bool good( PEdge, bool = false ) const
			{ return true; }

		// na uzytek ConstGraphMethods
		//------------- Methods sent to ConstGraphMethods --------------------------------------

		/** \brief Get number of vertices.
		 *
		 *  Gets the order of the graph.
		 *  \return the number of vertices in graph.	 */
		int getVertNo() const
			{ return up().getVertNo(); }
		/** \brief Get next vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the next vertex on the list.
		 *  If parameter \a v is set to NULL then the first vertex on the list will be taken.
		 *  \param v the reference vertex.
		 *  \returns a pointer to the next vertex on the vertex list or NULL if the vertex was last. */
		PVertex getVertNext( PVertex v ) const
			{ return up().getVertNext(v); }
		/** \brief Get previous vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the vertex prior to the one pointed by PVertex.
		 *  If parameter \a v is set to NULL then the last vertex on the list will be taken.
		 *  \param v the reference vertex.
		 *  \returns a pointer to the previous vertex on the vertex list or NULL if the vertex was the first. */
		PVertex getVertPrev( PVertex v ) const
			{ return up().getVertPrev(v); }

		/** \brief Get edge number.
		 *
		 *  The method gets the number of edges of type determined by the parameter \a mask.
		 *  \param mask the mask representing all types of the considered edges.
		 *  \returns the number of edges of certain type. */
		int getEdgeNo( EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNo( transl( mask ) ); }
		/** \brief Get next edge .
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask. The method gets the pointer to the edge next to \a e.
		 *  If parameter e is set to NULL then the first edge on the list is taken.
		 *  \param e the reference edge.
		 *  \param mask the mask representing all the types of considered edges.
		 *  \returns pointer to the next edge or if \a e is the last edge then NULL. */
		PEdge getEdgeNext( PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNext( e,transl(mask) ); }
		/** \brief Get previous edge.
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask. The method gets the pointer to the edge previous to \a e.
		 *  If parameter \a e is set to NULL then the last edge on the list will be taken.
		 *  \param e the reference edge.
		 *  \param mask the mask representing all the types of considered edges.
		 *  \returns pointer to the previous edge or if edge is the first one then NULL.*/
		 PEdge getEdgePrev( PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgePrev( e,transl(mask) ); }

		/** \brief Get next edge.
		 *
		 *  The method allows to see through all the edges incident to \a v, of direction congruent with the mask \a mask.
		 *  For each vertex the edges incident to it form a list. The method gets the pointer to the edge next to \a e.
		 *  If the parameter \a e is set to NULL then the first edge on the list is taken.
		 *  \param v only the edges incident to \a v are considered.
		 *  \param e the reference edge.
		 *  \param mask the mask representing the types of edges.
		 *  \returns the pointer to the next edge or if the edge is the last edge then NULL. */
		PEdge getEdgeNext( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNext( v,e,transl( mask ) ); }
		/** \brief Get previous edge
		 *
		 *  The method allows to see through all the edges incident to \a v, of direction congruent with the mask \a mask. The method gets the pointer to the edge previous to \a e.
		 *  If the parameter \a e is set to NULL then the last edge on the list will be returned.
		 *  \param v the reference vertex.
		 *  \param e the reference edge.
		 *  \param mask the mask representing the types of edges.
		 *  \returns Pointer to the previous edge or NULL if the edge is the first one. */
		PEdge getEdgePrev( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgePrev( v,e,transl( mask ) ); }
		/** \brief Get vertex degree.
		 *
		 *  Gets the number of edges incident to the vertex of direction (with respect to the vertex \a v) prespecified by the mask.
		 *  \param v the pointer to the reference vertex.
		 *  \param mask the mask representing the direction of considered edges.
		 *  \returns the number of edges directed as required in \a mask. */
		int getEdgeNo( PVertex v, EdgeDirection mask = EdAll) const
			{ return up().getEdgeNo( v,transl( mask ) ); }

		/** \brief Get next parallel edges.
		 *
		 *  The pointer to the next parallel edge is returned. The mask \a mask limits considered edges. If adjacency matrix is allowed the method will use it, otherwise lists are searched through.
		 *  If the parameter \a e is set to NULL then the first edge on the list will be taken.
		 *  \param v the first vertex.
		 *  \param u the second vertex.
		 *  \param e the reference edge
		 *  \param mask the mask representing the direction of considered edges.
		 *  \returns the pointer to the next parallel edge or NULL if \a e is the last. */
		PEdge getEdgeNext( PVertex v, PVertex u, PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNext( v,u,e,transl( mask ) ); }
		/** \brief Get previous parallel edges.
		 *
		 *  The pointer to the parallel edge previous to \a e is returned. The mask limiting considered edges is possible.
		 *  If the adjacency matrix is allowed the method will use it, otherwise only lists are checked.
		 *  \param v the first vertex.
		 *  \param u the second vertex.
		 *  \param e the reference edge.
		 *  \param mask the mask representing direction of the considered edges.
		 *  \returns the pointer to the previous parallel edge or NULL if \a e is the first edge. */
		PEdge getEdgePrev( PVertex v, PVertex u, PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgePrev( v,u,e,transl( mask ) ); }
		/** \brief Get number of parallel edges.
		 *
		 *  The method counts the number of edges between two vertices. Only edges directed in the way consistent with the mask \a mask are considered.
		 *  \param v the first vertex
		 *  \param u the second vertex
		 *  \param mask the mask representing the direction of considered edges.
		 *  \returns the number of edges between \a v and \a u. */
		int getEdgeNo( PVertex v, PVertex u, EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNo( v,u,transl( mask ) ); }

		/** \brief Get edge type.
		 *
		 *  \param e the pointer to considered edge.
		 *  \return the Koala::EdgeType value which is a mask representing the edge type.
		 *  \sa Koala::EdgeType */
		EdgeType getEdgeType( PEdge e ) const
			{ return (up().getEdgeType( e ) == EdLoop) ? EdLoop : EdUndir; }
		/** \brief Get edge ends
		 *
		 *  The method gets the pair of vertices on which the edge is spanned.
		 *  \param edge the considered edge.
		 *  \returns the pair of the vertices that are the ends of the edge.	 */
		std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const
			{ return up().getEdgeEnds( edge ); }
		/** \brief Get the first vertex.
		 *
		 *  \param edge the considered edge.
		 *  \returns the pointer to the first vertex of the \a edge.  */
		PVertex getEdgeEnd1( PEdge edge ) const
			{ return up().getEdgeEnd1( edge ); }
		/** \brief Get the second vertex.
		 *
		 *  \param edge the considered edge
		 *  \returns the pointer to the second vertex of the \a edge. */
		PVertex getEdgeEnd2( PEdge edge ) const
			{ return up().getEdgeEnd2( edge ); }
		/** \brief Get arc direction
		 *
		 *  The method gets the edge direction. Possible values of EdgeDirection are:
		 *  - EdNone   = 0x00 if the edge is NULL,
		 *  - EdLoop   = 0x01 if the edge is a loop,
		 *  - EdUndir  = 0x02 if the edge is undirected,
		 *  - EdDirIn  = 0x04 if the edge is directed and \a v is the second vertex of the edge,
		 *  - EdDirOut = 0x08 if the edge is directed and \a v is the first vertex of the edge.
		 *  \param edge considered edge.
		 *  \param v reference vertex.
		 *  \returns direction of edge \a edge. */
		EdgeDirection getEdgeDir( PEdge edge, PVertex v ) const;

	protected:
		static EdgeDirection transl( EdgeDirection mask )
			{ return ((mask & EdLoop) ? EdLoop : 0) | ((mask & EdUndir) ? (Directed | Undirected) : 0); }
	};

	// Tworzy widok dla podanego grafu
	/** \brief Undirected view generating function.
	 *
	 *  For a given graph \a g view in which all the edges are undirected is generated.
	 *  \param g the considered graph.
	 *  \return the new-created undirected view on the graph (view).
	 *  \ingroup DMview */
	template< class Graph > UndirView< Graph > makeUndirView( const Graph &g )
		{ return UndirView< Graph>( g ); }

	/* RevView
	 *
	 */
	template< class Graph > class RevView;
	template< class GraphType > struct GraphInternalTypes;
	template< class Graph > struct GraphInternalTypes< RevView< Graph> >
	{
		typedef typename Graph::Vertex Vertex;
		typedef typename Graph::PVertex PVertex;
		typedef typename Graph::Edge Edge;
		typedef typename Graph::PEdge PEdge;
		typedef typename Graph::VertInfoType VertInfoType;
		typedef typename Graph::EdgeInfoType EdgeInfoType;
		typedef typename Graph::GraphSettings GraphSettings;
	};

	// widok grafu typy Graph z odwroconymi wszystkimi krawedziami skierowanymi

	/** \brief Reversed view.
	 *
	 *  The class allows to create the view on graph in which all the arc are reversed. Hence it lets us use the reversed graph, but without allocation of new graph. The interface (except the process of creation) is the same as in Koala::Subgraph.
	 *  \ingroup DMview */
	template< class Graph > class RevView: public SubgraphBase, public ConstGraphMethods< RevView< Graph> >
	{
	public:
		typedef typename GraphInternalTypes< RevView< Graph > >::Vertex Vertex; /**< \brief Vertex of graph.*/
		typedef typename GraphInternalTypes< RevView< Graph > >::PVertex PVertex;/**< \brief Pointer to vertex of graph.*/
		typedef typename GraphInternalTypes< RevView< Graph > >::Edge Edge;/**< \brief Edge of graph.*/
		typedef typename GraphInternalTypes< RevView< Graph > >::PEdge PEdge;/**< \brief Pointer to edge of graph.*/
		typedef typename GraphInternalTypes< RevView< Graph > >::VertInfoType VertInfoType;/**< \brief Vertex information.*/
		typedef typename GraphInternalTypes< RevView< Graph > >::EdgeInfoType EdgeInfoType;/**< \brief Edge information.*/
		typedef typename GraphInternalTypes< RevView< Graph > >::GraphSettings GraphSettings;/**< \brief Graph settings.*/

		typedef RevView< Graph > GraphType;/**< \brief The current graph (view) type.*/
		typedef typename Graph::RootGrType RootGrType; /**< \brief Root (initial) graph type.*/
		typedef Graph ParentGrType; /**< \brief Superior (parent) graph type.*/

		// Konstruktory
		/** \brief Constructor.
		 *
		 *  The reversed view is created but it is unconnected to any graph.*/
		RevView()
			{ }
		/** \brief Constructor.
		 *
		 *  The reversed view connected to graph \a g is created.*/
		RevView( const Graph &g )
			{ SubgraphBase::link( &g ); }

		/** \brief Check allowed edge types.
		 *
		 *  \returns allowed types (EdgeType) of edges in the root graph. */
		static EdgeType allowedEdgeTypes()
			{ return ParentGrType::allowedEdgeTypes(); }
		/** \brief Get root graph.
		 *
		 *  The method tests if the graph has any superior graph if true gets the top graph in hierarchy of graphs.
		 *  \return the pointer to the root if the parent existed, NULL otherwise. */
		const RootGrType *getRootPtr() const
			{ return parent ? ((const ParentGrType*)parent)->getRootPtr() : NULL; }
		/** \brief Get parent graph.
		 *
		 *  The method tests if the graph has any superior graph if true gets the parent graph .
		 *  \return the pointer to the parent if it existed, NULL otherwise. */
		const ParentGrType *getParentPtr() const
			{ return (const ParentGrType*)parent; }
		/** \brief Get parent graph.
		 *
		 *  The method tests if the graph has any superior graph if true gets the parent graph .
		 *  \return the reference to the parent if it existed, NULL otherwise. */
		const ParentGrType &up() const;
		/** \brief Get root graph.
		 *
		 *  The method tests if the graph has any superior graph if true gets the top graph in hierarchy of graphs.
		 *  \return the reference to the root if the parent existed, NULL otherwise. */
		const RootGrType &root() const;
		// rozlacza obiekt od jego rodzica (jesli taki istnial) i podlacza do podanego grafu
		/** \brief Plug to \a g
		 *
		 * The method plugs the current view as a child of \a g. If the view was plugged to another view it is unplugged first.
		 *  \param g the new parent.*/
		void plug( const Graph &g )
			{ SubgraphBase::link( &g ); }
		// sprawia, ze podgraf staje sie niepodlaczony do zadnego rodzica
		/** \brief Unplug graph.
		*
		*  The method unplug the current view from the parent.
		*  \return true if the parent existed, false otherwise.  */
		bool unplug()
			{ return SubgraphBase::unlink(); }

		// do wspolpracy z podgrafami
		/** \brief Check vertex presence.
		 *
		 *  The method tests if the vertex form ancestor belongs to the current subgraph i.e. if it satisfy the \a vchoose of current subgraph. If the flag \a deep is set to true all the ancestors choosers are tested.
		 * \param vert the tested vertex.
		 * \param deep the flag determining if all choosers of ancestors are checked.
		 *  \return true if vertex belongs to subgraph, false otherwise.*/
		bool good( PVertex, bool = false ) const
			{ return true; }
		/** \brief Check edge presence.
		 *
		 *  The method tests if the edge form ancestor belongs to the current subgraph i.e. if it satisfy the \a echoose of current subgraph and both ends satisfy \a vchoose. If the flag \a deep is set to true all the ancestors choosers are tested.
		 * \param edge the tested edge.
		 * \param deep the flag determining if all choosers of ancestors are checked.
		 *  \return true if edge belongs to subgraph, false otherwise.*/
		bool good( PEdge, bool = false ) const
			{ return true; }

		// na uzytek ConstGraphMethods
		//------------- Methods sent to ConstGraphMethods --------------------------------------

		/** \brief Get number of vertices.
		 *
		 *  Gets the order of the graph.
		 *  \return the number of vertices in graph.	 */
		int getVertNo() const
			{ return up().getVertNo(); }
		/** \brief Get next vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the next vertex on the list.
		 *  If parameter \a v is set to NULL then the first vertex on the list will be taken.
		 *  \param v the reference vertex.
		 *  \returns a pointer to the next vertex on the vertex list or NULL if the vertex was last. */
		PVertex getVertNext( PVertex v ) const
			{ return up().getVertNext(v); }
		/** \brief Get previous vertex.
		 *
		 *  Since the vertex set is organized as a list, it is necessary to include a method returning the vertex prior to the one pointed by PVertex.
		 *  If parameter \a v is set to NULL then the last vertex on the list will be taken.
		 *  \param v the reference vertex.
		 *  \returns a pointer to the previous vertex on the vertex list or NULL if the vertex was the first. */
		PVertex getVertPrev( PVertex v ) const
			{ return up().getVertPrev(v); }

		/** \brief Get edge number.
		 *
		 *  The method gets the number of edges of type determined by the parameter \a mask.
		 *  \param mask the mask representing all types of the considered edges.
		 *  \returns the number of edges of certain type. */
		int getEdgeNo( EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNo( mask ); }
		/** \brief Get next edge .
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask. The method gets the pointer to the edge next to \a e.
		 *  If parameter e is set to NULL then the first edge on the list is taken.
		 *  \param e the reference edge.
		 *  \param mask the mask representing all the types of considered edges.
		 *  \returns pointer to the next edge or if \a e is the last edge then NULL. */
		PEdge getEdgeNext( PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNext( e,(mask) ); }
		/** \brief Get previous edge.
		 *
		 *  The method allows to see through all the edges of the type congruent with the mask. The method gets the pointer to the edge previous to \a e.
		 *  If parameter \a e is set to NULL then the last edge on the list will be taken.
		 *  \param e the reference edge.
		 *  \param mask the mask representing all the types of considered edges.
		 *  \returns pointer to the previous edge or if edge is the first one then NULL.*/
		PEdge getEdgePrev( PEdge e, EdgeDirection mask = EdAll ) const
			{ return up().getEdgePrev( e,(mask) ); }

		//          nie usuwac komentarza - waham sie co do wersji
		//        { return up().getEdgeNext(v,e,transl(mask)); }
		/** \brief Get next edge.
		 *
		 *  The method allows to see through all the edges incident to \a v, of direction congruent with the mask \a mask.
		 *  For each vertex the edges incident to it form a list. The method gets the pointer to the edge next to \a e.
		 *  If the parameter \a e is set to NULL then the first edge on the list is taken.
		 *  \param v only the edges incident to \a v are considered.
		 *  \param e the reference edge.
		 *  \param mask the mask representing the types of edges.
		 *  \returns the pointer to the next edge or if the edge is the last edge then NULL. */
		 PEdge getEdgeNext( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
			{ return getNext( v,e,transl( mask ) ); }

		//          nie usuwac komentarza - waham sie co do wersji
		//        { return up().getEdgePrev(v,e,transl(mask)); }
		/** \brief Get previous edge
		 *
		 *  The method allows to see through all the edges incident to \a v, of direction congruent with the mask \a mask. The method gets the pointer to the edge previous to \a e.
		 *  If the parameter \a e is set to NULL then the last edge on the list will be returned.
		 *  \param v the reference vertex.
		 *  \param e the reference edge.
		 *  \param mask the mask representing the types of edges.
		 *  \returns Pointer to the previous edge or NULL if the edge is the first one. */
		PEdge getEdgePrev( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
			{ return getPrev( v,e,transl( mask )); }

		/** \brief Get vertex degree.
		 *
		 *  Gets the number of edges incident to the vertex of direction (with respect to the vertex \a v) prespecified by the mask.
		 *  \param v the pointer to the reference vertex.
		 *  \param mask the mask representing the direction of considered edges.
		 *  \returns the number of edges directed as required in \a mask. */

		int getEdgeNo( PVertex v, EdgeDirection mask = EdAll) const
			{ return up().getEdgeNo( v,transl( mask ) ); }

		//          nie usuwac komentarza - waham sie co do wersji
		//        { return up().getEdgeNext(v,u,e,transl(mask)); }
		/** \brief Get next parallel edges.
		 *
		 *  The pointer to the next parallel edge is returned. The mask \a mask limits considered edges. If adjacency matrix is allowed the method will use it, otherwise lists are searched through.
		 *  If the parameter \a e is set to NULL then the first edge on the list will be taken.
		 *  \param v the first vertex.
		 *  \param u the second vertex.
		 *  \param e the reference edge
		 *  \param mask the mask representing the direction of considered edges.
		 *  \returns the pointer to the next parallel edge or NULL if \a e is the last. */
		PEdge getEdgeNext( PVertex v, PVertex u, PEdge e, EdgeDirection mask = EdAll ) const
			{ return getNext( v,u,e,transl( mask ) ); }
		//          nie usuwac komentarza - waham sie co do wersji
		//        { return up().getEdgePrev(v,u,e,transl(mask)); }
		/** \brief Get previous parallel edges.
		 *
		 *  The pointer to the parallel edge previous to \a e is returned. The mask limiting considered edges is possible.
		 *  If the adjacency matrix is allowed the method will use it, otherwise only lists are checked.
		 *  \param v the first vertex.
		 *  \param u the second vertex.
		 *  \param e the reference edge.
		 *  \param mask the mask representing direction of the considered edges.
		 *  \returns the pointer to the previous parallel edge or NULL if \a e is the first edge. */
		PEdge getEdgePrev( PVertex v, PVertex u, PEdge e, EdgeDirection mask = EdAll ) const
			{ return getPrev( v,u,e,transl( mask ) ); }
		/** \brief Get number of parallel edges.
		 *
		 *  The method counts the number of edges between two vertices. Only edges directed in the way consistent with the mask \a mask are considered.
		 *  \param v the first vertex
		 *  \param u the second vertex
		 *  \param mask the mask representing the direction of considered edges.
		 *  \returns the number of edges between \a v and \a u. */
		int getEdgeNo( PVertex v, PVertex u, EdgeDirection mask = EdAll ) const
			{ return up().getEdgeNo( v,u,transl( mask )); }

		/** \brief Get edge type.
		 *
		 *  \param e the pointer to considered edge.
		 *  \return the Koala::EdgeType value which is a mask representing the edge type.
		 *  \sa Koala::EdgeType */
		EdgeType getEdgeType( PEdge e ) const { return up().getEdgeType( e ); }

		/** \brief Get edge ends
		 *
		 *  The method gets the pair of vertices on which the edge is spanned.
		 *  \param edge the considered edge.
		 *  \returns the pair of the vertices that are the ends of the edge.	 */
		std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const;
		/** \brief Get the first vertex.
		 *
		 *  \param edge the considered edge.
		 *  \returns the pointer to the first vertex of the \a edge.  */
		PVertex getEdgeEnd1( PEdge edge ) const;
		/** \brief Get the second vertex.
		 *
		 *  \param edge the considered edge
		 *  \returns the pointer to the second vertex of the \a edge. */
		PVertex getEdgeEnd2( PEdge edge ) const;
		/** \brief Get arc direction
		 *
		 *  The method gets the edge direction. Possible values of EdgeDirection are:
		 *  - EdNone   = 0x00 if the edge is NULL,
		 *  - EdLoop   = 0x01 if the edge is a loop,
		 *  - EdUndir  = 0x02 if the edge is undirected,
		 *  - EdDirIn  = 0x04 if the edge is directed and \a v is the second vertex of the edge,
		 *  - EdDirOut = 0x08 if the edge is directed and \a v is the first vertex of the edge.
		 *  \param edge considered edge.
		 *  \param v reference vertex.
		 *  \returns direction of edge \a edge. */
		EdgeDirection getEdgeDir( PEdge edge, PVertex v ) const;

	protected:
		static EdgeDirection transl( EdgeDirection mask );
		static EdgeDirection nextDir( EdgeDirection dir );
		static EdgeDirection prevDir( EdgeDirection dir );

		PEdge getNext(PVertex vert, PEdge edge, EdgeDirection direct ) const;
		PEdge getPrev( PVertex vert, PEdge edge, EdgeDirection direct ) const;
		PEdge getPrev( PVertex vert1, PVertex vert2, PEdge edge, EdgeDirection direct ) const;
		PEdge getNext( PVertex vert1, PVertex vert2, PEdge edge, EdgeDirection direct ) const;
	};

	// tworzy widok do podanego grafu
	/** \brief Reversed view generating function.
	 *
	 *  For a given graph \a g view in which all the arcs have opposite direction.
	 *  \param g the considered graph (view).
	 *  \return the new-created reversed view on the graph.
	 *  \ingroup DMview */
	template< class Graph > RevView< Graph > makeRevView( const Graph &g )
		{ return RevView< Graph>( g ); }

#include "view.hpp"
}

#endif
