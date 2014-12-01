#ifndef KOALA_GRAPHBASE_H
#define KOALA_GRAPHBASE_H

/* grconst.h
 *
 */

template< class GraphType > class ConstGraphMethods;


namespace Privates {

    //NEW: sprywatyzowano
    template< class GraphType > struct GraphInternalTypes;

    template< class GraphType > struct GraphInternalTypes< ConstGraphMethods< GraphType > >
    {
        typedef typename GraphInternalTypes< GraphType >::Vertex Vertex;
        typedef typename GraphInternalTypes< GraphType >::PVertex PVertex;
        typedef typename GraphInternalTypes< GraphType >::Edge Edge;
        typedef typename GraphInternalTypes< GraphType >::PEdge PEdge;
        typedef typename GraphInternalTypes< GraphType >::VertInfoType VertInfoType;
        typedef typename GraphInternalTypes< GraphType >::EdgeInfoType EdgeInfoType;
        typedef typename GraphInternalTypes< GraphType >::GraphSettings GraphSettings;
    };

    template <class A, class B> struct SecondTypeTest {
        typedef B Type;
    };

}

/* ConstGraphMethods
 * Klasa wprowadzajaca poprzez mechanizm CRTP metody operujace na strukturze typu const GraphType do tejze struktury
 * Wykorzystywana do wzbogacania interfejsu Graph, Subgraph oraz widokow na graf
 */
/** \brief Constant Graph Methods
 *
 *  Class inherited by Graph, Subgraph and views on graph. Set of basic constant method used by graph.
 *  \ingroup DMgraph*/
template< class GraphType > class ConstGraphMethods
{
public:
	typedef typename Privates::GraphInternalTypes< ConstGraphMethods< GraphType > >::Vertex Vertex; /**< \brief Vertex of graph.*/
	typedef typename Privates::GraphInternalTypes< ConstGraphMethods< GraphType > >::PVertex PVertex; /**< \brief Pointer to vertex of graph WEN: tj. używany w procedurach identyfikator wierzchołka.*/
	typedef typename Privates::GraphInternalTypes< ConstGraphMethods< GraphType > >::Edge Edge; /**< \brief Edge of graph.*/
	typedef typename Privates::GraphInternalTypes< ConstGraphMethods< GraphType > >::PEdge PEdge; /**< \brief Pointer to edge of graph. WEN: tj. używany w procedurach identyfikator krawedzi.*/
	typedef typename Privates::GraphInternalTypes< ConstGraphMethods< GraphType > >::VertInfoType VertInfoType; /**< \brief Vertex information. WEN: tj. typ pola info */
	typedef typename Privates::GraphInternalTypes< ConstGraphMethods< GraphType > >::EdgeInfoType EdgeInfoType; /**< \brief Edge information. WEN: tj. typ pola info */
	typedef typename Privates::GraphInternalTypes< ConstGraphMethods< GraphType > >::GraphSettings GraphSettings; /**< \brief Graph settings.*/

protected:
	const GraphType &self;

	struct Parals3
	{
		PVertex v1,v2;
		EdgeDirection direct;
		int nr;
		PEdge edge;

		Parals3( PVertex av1, PVertex av2, EdgeDirection adirect, int anr, PEdge aedge):
			v1( av1 ), v2( av2 ), direct( adirect ), nr( anr ), edge( aedge )
			{ }
		Parals3()
			{ }
	};

	struct Parals3cmp
	{
		bool operator()( Parals3 a, Parals3 b )
		{
			return (a.v1 < b.v1) || (a.v1 == b.v1 && a.v2 < b.v2) ||
				(a.v1 == b.v1 && a.v2 == b.v2 && a.direct < b.direct ) ||
				(a.v1 == b.v1 && a.v2 == b.v2 && a.direct == b.direct && a.nr < b.nr);
		}
	};

	struct ParalsCount {
        std::pair<int, PEdge> dirOutCount,dirInCount,undirCount;

        ParalsCount() : dirOutCount(std::make_pair(0, (typename ConstGraphMethods< GraphType >::PEdge)0)),
                        dirInCount(std::make_pair( 0,(typename ConstGraphMethods< GraphType >::PEdge)0)),
                        undirCount(std::make_pair( 0,(typename ConstGraphMethods< GraphType >::PEdge)0))
                        {}

        std::pair<int, PEdge>& counter(EdgeDirection dir,EdgeDirection reltype)
        {
            switch (dir) {
                case EdDirOut : return dirOutCount;
                case EdDirIn :  if (reltype==EdDirOut) return dirInCount; else return dirOutCount;
                case EdUndir :  if (reltype==EdUndir) return dirOutCount; else return undirCount;
                default : assert(0);
            }
        }
	};

    EdgeDirection paralDirs(EdgeDirection dir, EdgeDirection reltype ) const;

public:
	/** \brief Constructor */
	ConstGraphMethods(): self( (const GraphType &)*this )
		{ }

	/** \brief Copy constructor*/
	ConstGraphMethods( const ConstGraphMethods & ): self( (const GraphType &)*this )
		{ }

	//Aby nie zaczelo pracowac na innym grafie
	/* \brief Copy content operator.
	 *
	 *  Does nothing to prevent change of graphs.
	 *  \return *this.
	 */
	ConstGraphMethods &operator=( const ConstGraphMethods & )
		{ return *this; }
//WEN:  gdzies trzeba podac ponizszy koment, tj. ze szablon ConstGraphMethods korzystajac w wzorca CRTP wprowadza staly pakiet metod const grafu korzystajac tylko z ponizszych
//WEN: jest to wykorzystane w glownej klasie grafu i wszystkich widokach wiew.h
//    Uwaga: metody, ktore musza byc dostarczane przez GraphType:
//         int getVertNo() const;
//         PVertex getVertNext( PVertex ) const;
//         PVertex getVertPrev( PVertex ) const;
//         int getEdgeNo( EdgeDirection  = EdAll ) const;
//         PEdge getEdgeNext( PEdge , EdgeDirection  = EdAll ) const;
//         PEdge getEdgePrev( PEdge , EdgeDirection  = EdAll ) const;
//         int getEdgeNo( PVertex , EdgeDirection  = EdAll) const;
//         PEdge getEdgeNext( PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         PEdge getEdgePrev( PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         int getEdgeNo( PVertex , PVertex , EdgeDirection = EdAll ) const;
//         PEdge getEdgeNext( PVertex , PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         PEdge getEdgePrev( PVertex , PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         EdgeType getEdgeType( PEdge  ) const;
//         std::pair< PVertex,PVertex > getEdgeEnds( PEdge ) const;
//         PVertex getEdgeEnd1( PEdge ) const;
//         PVertex getEdgeEnd2( PEdge  ) const;
//         EdgeDirection getEdgeDir( PEdge , PVertex);
//NEW: a takze
//          bool hasAdjMatrix() const;
//          static bool allowedAdjMatrix()


//    Uwaga: zasady ogolne obslugi zbiorow wierz/krawedzi struktury grafowej. Zestaw metod postaci:
//    int get...No(... arg ...) - dlugosc listy
//    template <class Iter> int get...(...) - zapis na podany iterator calej listy, zwraca jej dlugosc
//    Set<Element> get...Set(...) - wszystkie elementy listy zwracane w formie zbioru
//    Jesli te elementy struktury sa zorganizowane w liste, poruszamy sie po niej uzywajac dodatkowo
//    Element get...(...) - pierwszy element listy, NULL w przypadku listy pustej
//    Element get...Last(...) - ostatni element listy, NULL w przypadku listy pustej
//    Element get...Next(... arg ...) - kolejny po arg element listy, NULL jesli arg byl ostatni
//    Element get...Prev(... arg ...) - poprzedni po arg element listy, NULL jesli arg byl pierwszy
//        Natomiast zbior wejsciowy pobieramy z zakresu miedzy dwoma iteratorami lub przez Set



	// Lista wierzcholkow grafu
	// Liczba wierzcholkow (rzad grafu).
	/** \brief Get number of vertices.
	 *
	 *  The method gets the order of the graph i.e. the number of vertices in the graph.
	 *  \return number of vertices in the graph.
	 */
	inline int getVertNo() const
		{  return self.getVertNo(); }

	/** \brief Get next vertex.
	 *
	 *  The Graph is organized on list structures. Also the list of vertices is available. The method allows to see through all vertices in the graph. It gets the vertex next to \a v.
	 *  \param v the reference vertex, the next vertex is taken.
	 *  \return the pointer to the vertex next to \a v on the list of vertices. If \a v is the last vertex, then NULL is returned. If \a v is NULL, then the first vertex is returned.
	 */
	inline PVertex getVertNext( PVertex v ) const
		{ return self.getVertNext( v ); }

	/** \brief Get previous vertex
	 *
	 *  The graph is organized on list structures. Also the list of vertices is available. The method allows to see through all vertices in the graph. It gets the vertex prior to \a v.
	 *  \param v the pointer to the reference vertex, the previous vertex is taken.
	 *  \return pointer to the vertex prior to \a v on the list of vertices. If \a v is the first vertex, then NULL is returned. If \a v is NULL, then the last vertex on the list is returned.
	 */
	inline PVertex getVertPrev( PVertex v ) const
		{ return self.getVertPrev( v ); }

	/** \brief Get first vertex
	 *
	 *  The graph is organized on list structures. Also the list of vertices is available. The method allows to get the first vertices in the graph.
	 *  \return Pointer to the the first vertex on the list of vertices WEN: lub NULL przy grafie pustym.
	 */
	PVertex getVert() const
		{ return self.getVertNext( (PVertex)0 ); }

	/** \brief Get last vertex
	 *
	 *  The graph is organized on list structures. Also the list of vertices is available. The method allows to get to the last vertices in the graph on the list of vertices.
	 *  \return the pointer to the the last vertex on the list of vertices. WEN: lub NULL przy grafie pustym
	 */
	PVertex getVertLast() const
		{ return self.getVertPrev( (PVertex)0 ); }

	/** \brief Get vertices.
	 *
	 *  Pointers to all the vertices in graph are written to the container defined by \a iter. Any container with a defined iterator (ex. table) can be used.
	 *  \tparam OutputIterator Iterator class of container in which the method stores up vertices.
	 *  \param[out] iter the iterator of the container to which all the vertices form the graph are written. WEN: kontener na typ PVertex
	 *  \return the number of vertices.
	 */
	template< class OutputIterator > int getVerts( OutputIterator iter) const;

	/** \brief Get vertex set.
	 *
	 *  The method gets and returns the vertex set.
	 *  \return the set of all vertices of the graph.
	 */
	Set< PVertex > getVertSet() const;

	// Liczba krawêdzi typu zawartego w podanej masce
	/** \brief Get edge number.
	 *
	 *  The method gets the number of edges of the type determined by the parameter \a mask.
	 *  \param mask representing all types of considered edges. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
	 *  \returns the number of edges of certain type.
	 */
	inline int getEdgeNo( EdgeType mask = EdAll ) const
		{ return self.getEdgeNo( mask ); }

	/** \brief Get next edge.
	 *
	 *  Method allows to see through all the edges of type congruent with \a mask. The method returns the pointer to the edge next to \a e.
	 *  If the parameter \a e is set to NULL, then the first edge on the list will be taken.
	 *  \param e reference edge.
	 *  \param mask represents the types of edges concerned. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
	 *  \returns the pointer to the next edge or NULL if \a e is the last edge on the list of edges.
	 */
	inline PEdge getEdgeNext( PEdge e, EdgeType mask = EdAll ) const
		{ return self.getEdgeNext( e,mask ); }

	/** \brief Get previous edge.
	 *
	 *  The method allows to see through all the edges of type congruent with \a mask. The method returns the pointer to the edge previous to edge \a e.
	 *  If parameter \a e is set to NULL then the last edge on the list will be taken.
	 *  \param e next edge will be returned.
	 *  \param mask represents all the types of edges concerned. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
	 *  \returns the pointer to the next edge or if the edge is the last one then NULL.
	 */
	inline PEdge getEdgePrev( PEdge e, EdgeType mask = EdAll ) const
		{ return self.getEdgePrev( e,mask ); }

	/** \brief Get edges.
	 *
	 *  The method puts pointers to all the edges consistent with mask to the container defined by the iterator. Any container with a defined iterator can be taken. WEN: ale na typ PEdge
	 *  \tparam OutputIterator The iterator class of for the container in which the output edges are to be stored up.
	 *  \param[out] iter the iterator of the container in which edges are to be stored.
	 *  \param mask the type of edges which are to be taken. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
	 *  \return the number of stored edges. */
	template< class OutputIterator > int getEdges( OutputIterator iter, EdgeType mask = EdAll ) const;

	/** \brief Get set of edges.
	 *
	 *  All the edges in graph, which are consistent  with \a mask, are stored in the set.
	 *  \param mask determines the types of edges that are concerned. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
	 *  \return the set of edges.
	 */
	Set< PEdge > getEdgeSet( EdgeType mask = EdAll ) const;

	/** \brief Get first edge.
	 *
	 *  Edges in the graph are organized as lists. There is a separate list for each type of edges. If \a mask is congruent with many types of edges, lists are virtually connected.
	 *  The method allows to get the pointer to the first edge on the list.
	 *	\param mask determines the types of edges concerned. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
	 *  \return the pointer to the first edge on the list. WEN: lub NULL przy braku
	 */
	PEdge getEdge( EdgeType mask = EdAll ) const
		{ return self.getEdgeNext( (PEdge)0,mask ); }

	/** \brief Get last edge
	 *
	 *  The edges in a graph are organized as lists, there is a separate list for each type of edges. If \a mask is congruent with many types of edges, lists are virtually connected.
	 *  The method allows to get the pointer to the last edge on the list.
	 *	\param mask determines the types of edges concerned. WEN: tzn. krawedzie ktorych typ & mask!= sa taken into account
	 *  \return the pointer to the last edge on the list. WENL lub NULL przy pustej
	 */
	PEdge getEdgeLast( EdgeType mask = EdAll ) const
		{ return self.getEdgePrev( (PEdge)0,mask ); }

	// lista krawedzi sasiednich do v o orientacji wzgledem v zawartej w masce
	/** Get degree of vertex
	 *  WEN: to nie jest stopien (przypadek petli), na stopien jest inna metoda
	 *  The method gets the number of edges incident to the vertex \a v (degree of \a v).
	 *  \param v the considered vertex.
	 *  \param mask determines the direction of edges concerned. WEN: tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
	 *  \return the number of edges incident the vertex \a v with prespecified direction.
	 */
	inline int getEdgeNo( PVertex v, EdgeDirection mask = EdAll) const
		{ return self.getEdgeNo( v,mask ); }

	/** \brief Get next edge.
	 *
	 *  The method allows to see through all the edges incident to \a v, of direction congruent with \a mask.
	 *  For each vertex edges incident to it form a list. The method gets the pointer to the edge next to \a e.
	 *  If parameter \a e is set to NULL then the first edge on the list will be taken.
	 *  \param v only edges incident to \a v.
	 *  \param e next edge will be returned.
	 *  \param mask representing the types of edges. WEN: no wlasnie nie bardzo type od edge tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
	 *  \returns the pointer to the next edge or NULL if the edge is the last one.
	 */
	inline PEdge getEdgeNext( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
		{ return self.getEdgeNext( v,e,mask ); }

	/** \brief Get previous edge.
	 *
	 *  The method allows to see through all the edges incident to \a v, of direction congruent with \a mask. The method gets the pointer to the edge previous to \a e.
	 *  If parameter \a e is set to NULL, then the last edge on the list will be returned.
	 *  \param v reference vertex.
	 *  \param e reference edge.
	 *  \param mask representing the types of edges. WEN: no wlasnie nie bardzo type od edge tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
	 *  \returns pointer to the previous edge incident to \a v or if edge is the first then NULL.
	 */
	inline PEdge getEdgePrev( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
		{ return self.getEdgePrev( v,e,mask ); }

	/** \brief Get first edge.
	 *
	 *  The method gets the pointer to the first edge on the list of edges incident to vertex \a vert. Only edges with direction consistent with \a mask are considered.
	 *  \param vert reference vertex. The first edge on the list of edges incident to \a vert is returned.
	 *  \param mask represents the types of edges. WEN: no wlasnie nie bardzo type od edge tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
	 *  \returns the pointer to the first edge incident to \a vert. WEN: lub NULL przy pustej
	 */
	PEdge getEdge( PVertex vert, EdgeDirection mask= EdAll ) const
		{ return self.getEdgeNext( vert,(PEdge)0,mask ); }

	/** \brief Get last edge.
	 *
	 *  The method gets the pointer to the last edge on the list of edges incident to vertex \a vert. Only edges with direction consistent with \a mask are considered.
	 *  \param vert reference vertex. The last edge on the list of edges incident to \a vert is return.
	 *  \param mask represents the types of edges. WEN: no wlasnie nie bardzo type od edge tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
	 *  \returns pointer to the last edge incident to \a vert. WEN: lub NULL przy pustej
	 */
	PEdge getEdgeLast( PVertex vert, EdgeDirection mask = EdAll ) const
		{ return self.getEdgePrev( vert,(PEdge)0,mask );}

	/** \brief Get incident edges.
	 *
	 *  The method returns the set of all edges incident to \a v with direction congruent with mask \a direct.
	 *  \tparam OutpoutIterator the type of iterator for the container of the output set of edges. WEN: tj. na typ PEdge
	 *  \param[out] iter the iterator of the container of output edges.
	 *  \param v the reference vertex.
	 *  \param direct the mask defining the returned edges direction (with respect to \a v). WEN: tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
	 *  \return the number of edges in the set given by \a iter.
	 */
	template< class OutputIterator > int getEdges( OutputIterator, PVertex, EdgeDirection = EdAll ) const;

	/** \brief Get incident edges
	 *
	 *  The method returns the set of all edges incident to \a v with direction consistent with mask \a direct.
	 *  \param v the reference vertex.
	 *  \param direct the mask defining the returned edges direction (with respect to \a v). WEN: tzn. krawedzie ktorych sposob sasiadowania z v & mask!= sa taken into account
	 *  \return the set of edges incident to \a v.
	 */
	Set< PEdge > getEdgeSet( PVertex v, EdgeDirection direct = EdAll ) const;

    //NEW: Poniewaz nie da sie latwo bez iteratorow przegladac listy kolejnych sasiadow wierzcholka w multigrafie
    // (bo moga sie powtarzac via rozne krawedzie), wprowadzono liste rozszerzen powyzszych getEdge...(PVertex...), ktora wraz z krawedzia
    //zwraca takze drugi (procz v) wierzcholek koncowy tejze krawedzi (lub (NULL,NULL) na koncu listy).
   	inline std::pair<PEdge,PVertex> getEdgeVertNext( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
		{
		    e=self.getEdgeNext( v,e,mask );
		    PVertex u= (e) ? this->getEnd(e,v) : 0;
		    return std::pair<PEdge,PVertex> (e,u);
        }
   	inline std::pair<PEdge,PVertex> getEdgeVertPrev( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
		{
		    e=self.getEdgePrev( v,e,mask );
		    PVertex u= (e) ? this->getEnd(e,v) : 0;
		    return std::pair<PEdge,PVertex> (e,u);
        }
   	inline std::pair<PEdge,PVertex> getEdgeVert( PVertex v,  EdgeDirection mask = EdAll ) const
		{   return this->getEdgeVertNext(v,(PEdge)0,mask);   }
   	inline std::pair<PEdge,PVertex> getEdgeVertLast( PVertex v,  EdgeDirection mask = EdAll ) const
		{   return this->getEdgeVertPrev(v,(PEdge)0,mask);   }
    inline int getEdgeVertNo( PVertex v, EdgeDirection mask = EdAll) const
		{ return self.getEdgeNo( v,mask ); }
		// tu OutputIterator przyjmuje pary std::pair<PEdge,PVertex>
	template< class OutputIterator > int getEdgeVerts( OutputIterator, PVertex, EdgeDirection = EdAll ) const;


	// lista krawedzi laczacych podane wierzcholki w sposob zawarty w masce
	/** \brief Get number of parallel edges.
	 *
	 *  The method counts the number of edges between two vertices.
	 *  \param u the first vertex
	 *  \param v the second vertex
	 *  \param mask represents the edge direction. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
	 *  \returns the number of edges between \a u and \a v.
	 */
	inline int getEdgeNo( PVertex u, PVertex v, EdgeDirection mask = EdAll ) const
		{ return self.getEdgeNo( u,v,mask ); }

	/** \brief Get next parallel edge.
	 *
	 *  The pointer to the next parallel edge is returned. Mask \a direct limits considered edges. If adjacency matrix is allowed the method will use it, otherwise lists are searched through.
	 *  If parameter \a e is set to NULL then the first edge on the list will be taken.
	 *  \param u the first vertex.
	 *  \param v the second vertex.
	 *  \param e the reference edge. The next edge is returned.
	 *  \param mask represents the considered edge direction. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
	 *  \returns the pointer to the next parallel edge or NULL if \a e is the last.
	 */
	inline PEdge getEdgeNext( PVertex u, PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
		{ return self.getEdgeNext( u,v,e,mask ); }

	/** \brief Get previous parallel edge.
	 *
	 *  The pointer to the parallel edge previous to \a e is returned. The mask limiting considered edges is possible.
	 *  If the adjacency matrix is allowed the method will use it, otherwise only lists are checked.
	 *  \param u the first vertex.
	 *  \param v the second vertex.
	 *  \param e the reference edge. The previous edge is returned.
	 *  \param mask representing the edge direction. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
	 *  \returns the pointer to the next parallel edge or NULL if \a e is the first edge.
	 */
	inline PEdge getEdgePrev( PVertex u, PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
		{ return self.getEdgePrev( u,v,e,mask ); }

	/** \brief Get first edge.
	 *
	 *  The method returns the pointer to the first edge on the list of edges spanned on two vertices. A mask limiting considered edges is possible.
	 *  If the adjacency matrix is allowed the method will use it, otherwise only lists are checked.
	 *  \param vert1 the first vertex.
	 *  \param vert2 the second vertex.
	 *  \param mask represents the direction of considered edge. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
	 *  \returns the pointer to the first edge spanned on vertices \a vert1 and \a vert2. WEN: lub NULL dla pustej
	 */
	PEdge getEdge( PVertex vert1, PVertex vert2, EdgeDirection mask = EdAll ) const
		{ return self.getEdgeNext( vert1,vert2,(PEdge)0,mask ); }

	/** \brief Get last edges.
	 *
	 *  The method returns the pointer to the last edge on the list of edges spanned on two vertices. Mask limiting considered edges is possible.
	 *  If adjacency matrix is allowed method will use it, otherwise only lists are checked.
	 *  \param vert1 the first vertex.
	 *  \param vert2 the second vertex.
	 *  \param mask represents the considered edge direction. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
	 *  \returns the  pointer to the last edge spanned on vertices \a vert1 and \a vert2. WEN: lub NULL dla pustej
	 */
	PEdge getEdgeLast( PVertex vert1, PVertex vert2, EdgeDirection mask = EdAll ) const
		{ return self.getEdgePrev( vert1,vert2,(PEdge)0,mask ); }

	/** \brief Get set of parallel edges.
	 *
	 *  The method returns the set of edges spanned on vertices \a vert1 and \a vert2 with direction congruent with mask \a direct.
	 *  \param vert1 the first reference vertex.
	 *  \param vert2 the second reference vertex.
	 *  \param direct mask representing the considered edges direction. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
	 *  \return the set of edges spanned on vert1 and vert2.
	 */
	Set< PEdge > getEdgeSet( PVertex vert1, PVertex vert2, EdgeDirection direct = EdAll ) const;

	/** \brief Get set of parallel edges.
	 *
	 *  The method gets a set of edges spanned on the vertices \a vert1 and \a vert2. Only edges with direction congruent with \a direct are considered.
	 *  Any container with a defined iterator may by used.
	 *  \tparam OutputIterator Type of iterator for the container of the output set of edges.
	 *  \param[out] iter the iterator of the container with edges. WEN: tj na typ PEdge
	 *  \param vert1 the first reference vertex.
	 *  \param vert2 the second reference vertex.
	 *  \param direct the mask defining the considered edges direction. WEN: maska okresla i typ interesujacych krawedzi i (dla skierowanych) ich orientacje
	 *  \return the number of parallel edges stored in return via container represented by \a iter.
	 */
	template< class OutputIterator > int getEdges( OutputIterator iter, PVertex vert1, PVertex vert2, EdgeDirection direct = EdAll ) const;

	// zbior wierzcholkow/krawedzi spelniajacych podany chooser
	/** \brief Get set of vertices.
	 *
	 *  The method gets the set of vertices defined by the chooser \a ch.
	 *  \tparam OutputIterator iterator class of a container used to store the set of vertices returned via WEN: reference??? chyba nie, po prostu na PVertex
	 *  \tparam VChooser2 Class allowing to choose vertices automatically.
	 *  \param out the iterator of the container used to store up the vertices chosen by the chooser \a ch.
	 *  \param ch the chooser object allowing to choose vertices automatically. WEN: czyli jak? musi miec bool operator()( Elem *elem, const Graph &gr ) - jak wszystkie choosery
	 *  \return the number of vertices in the output container.
	 */
	template< class OutputIterator, class VChooser2 > int getVerts( OutputIterator out, VChooser2 ch ) const;

	/** \brief Get set of vertices.
	 *
	 *  The method returns the set of vertices defined by the chooser class.
	 *  \tparam VChooser2 the class allowing to choose vertices automatically.
	 *  \param ch the chooser object allowing to choose vertices automatically. WEN: czyli jak? musi miec bool operator()( Elem *elem, const Graph &gr ) - jak wszystkie choosery
	 *  \return the set of vertices.
	 */
	template< class VChooser2 > Set< PVertex > getVertSet( VChooser2 ch ) const;

	/** \brief Get set of edges.
	 *
	 *  The method gets the set of edges defined by the chooser object \a ch.
	 *  \tparam OutputIterator the iterator class of the container used to store the set of edges returned via reference (iterator).
	 *  \tparam EChooser2 the class allowing to choose edges automatically.
	 *  \param[out] out the iterator of the container used to store up the WEN: edges! vertices chosen by the chooser \a ch.
	 *  \param ch the chooser object allowing to choose edges automatically. WEN: czyli jak? musi miec bool operator()( Elem *elem, const Graph &gr ) - jak wszystkie choosery
	 *  \return the number of edges in out container.
	 */
	template< class OutputIterator, class EChooser2 >
        typename Privates::SecondTypeTest<typename EChooser2::ChoosersSelfType, int>::Type getEdges( OutputIterator out, EChooser2 ch ) const;

	/** \brief Get set of edges.
	 *
	 *  The method gets the set of edges defined by the chooser object \a ch.
	 *  \tparam EChooser2 the class allowing to choose edges automatically.
	 *  \param ch the chooser object allowing to choose edges automatically. WEN: czyli jak? musi miec bool operator()( Elem *elem, const Graph &gr ) - jak wszystkie choosery
	 *  \return the set of pointers to edges congruent with the chooser object \a ch.
	 */
	template< class EChooser2 >
	typename Privates::SecondTypeTest<typename EChooser2::ChoosersSelfType, Set< PEdge > >::Type getEdgeSet( EChooser2 ch ) const;

	// podobnie, ale podajemy pare chooserow (dla wierzcholka i krawedzi). Parametr bool=true - krawedz ma spelniac
	// nie tylko swoj chooser, ale takze oba konce chooser dla wierzcholkow
	/** \brief Choose edges and vertices.
	 *
	 *  The method gets the pair of sets. The set of vertices and the set of edges. The pair of choosers defines which edges and vertices should be taken.
	 *  \tparam OutputV the iterator class of a container used to keep output vertices.
	 *  \tparam OutputE the iterator class of a container used to keep output edges.
	 *  \tparam EChooser2 the class allowing to choose edges automatically.
	 *  \tparam VChooser2 the class allowing to choose vertices automatically.
	 *  \param out the standard pair of iterators used to return the containers of vertices and edges.
	 *  \param chs the pair of chooser objects allowing to choose vertices and edges automatically. WEN: czyli jak? musi miec bool operator()( Elem *elem, const Graph &gr ) - jak wszystkie choosery
	 *  \param chooseends if true for each edge not only the edge chooser must be satisfied but also both ends need to satisfy the vertex chooser.
	 *  \return the standard pair of integers that are respectively the number of chosen vertices and the number of edges.
	 */
	template< class OutputV, class OutputE, class VChooser2, class EChooser2 >
		std::pair< int,int > getChosen( std::pair< OutputV,OutputE > out,std::pair< VChooser2,EChooser2 > chs, bool chooseends = true) const;

	/** \brief Choose edges and vertices.
	 *
	 *  The method gets the pair of sets. The set of vertices and the set of edges. The pair of choosers defines which edges and vertices should be passed to function as a value.
	 *  \tparam EChooser2 the class allowing to choose edges automatically.
	 *  \tparam VChooser2 the class allowing to choose vertices automatically.
	 *  \param chs the pair of chooser objects allowing to choose vertices and edges automatically. WEN: czyli jak? musi miec bool operator()( Elem *elem, const Graph &gr ) - jak wszystkie choosery
	 *  \param chosenends if true for each edge, not only edge chooser must be satisfied but also both ends need to satisfy vertex chooser.
	 *  \return the standard pair of sets in which the first element is the set of vertices and the second the set of edges.
	 */
	template<class VChooser2,class EChooser2 >
		std::pair< Set< PVertex >,Set< PEdge > > getChosenSets( std::pair< VChooser2,EChooser2 > chs, bool chosenends = true) const;

	// Wierzcholek/krawedz o podanym numerze w grafie.
	/** \brief Get vertex by number
	 *
	 *  The list of vertices in the graph is searched through and the pointer to the \a idx-th vertex is returned.
	 *  \param idx the index of the returned vertex. WEN: numeracja od 0
	 *  \return the pointer to the \a idx-th vertex.
	 */
	PVertex vertByNo( int idx ) const;

	/** \brief Get edge by number
	 *
	 *  The list of edges in the graph is searched through. The pointer to the \a idx-th edge is returned.
	 *  \param idx the index of the returned edge. WEN: numeracja od 0
	 *  \return the pointer to the \a idx-th edge.
	 */
	PEdge edgeByNo( int idx ) const;

	// Numer wierzcho³ka/krawedzi, -1 w razie braku
	/** \brief Get index of vertex
	 *
	 *  The list of vertices in the graph is searched through. The position of \a vert is returned.
	 *  \param vert the index of the vertex \a vert is returned.
	 *  \return the position of \a vert on the list of vertices in the graph. WEN: numeracja od 0, zwraca -1 w razie bledu
	 */
	int vertPos( PVertex vert ) const;

	/** \brief Get index of edge.
	 *
	 *  The list of edges in the graph is searched through. The position of \a edge is returned.
	 *  \param edge the index of this edge is returned.
	 *  \return the position of edge on the list of edges in the graph. WEN: numeracja od 0, zwraca -1 w razie bledu
	 */int edgePos( PEdge edge ) const;

	// czy graf zawiera dany element
	/** \brief Test the existence of vertex.*/
	bool has( PVertex ) const;
	/** \brief Test the existence of edge.*/
	bool has( PEdge ) const;

	// typ krawedzi
	/** \brief Get edge type.
	 *
	 *  \param e the pointer to the considered edge.
	 *  \returns EdgeType value which represents the type of edge.
	 *  - Detached   = 0x0 WEN: wywal, bo ta wartosc nie moze sie pojawic
	 *  - Loop       = 0x1
	 *  - Undirected = 0x2
	 *  - Directed   = 0xC
	 *  \sa EdgeType */
	inline EdgeType getEdgeType( PEdge e ) const
		{ return self.getEdgeType( e ); }
	// synonim poprzedniej
	/** \brief Get edge type.
	 *
	 *  \param e the pointer to the considered edge.
	 *  \returns EdgeType value which represents the type of edge.
	 *  - Detached   = 0x0 WEN: wywal, bo ta wartosc nie moze sie pojawic
	 *  - Loop       = 0x1
	 *  - Undirected = 0x2
	 *  - Directed   = 0xC
	 *  \sa EdgeType */
	EdgeType getType( PEdge e ) const
		{ return self.getEdgeType( e ); }

	// wierzcholki koncowe krawedzi
	/** \brief Get edge ends.
	 *
	 *  The method gets the pair of vertices on which \a edge is spanned.
	 *  \param edge the considered edge
	 *  \returns the pair of vertices that are the ends of \a edge. WEN: tj. pierwsza koncowa w .first, druga w .second - krawedz ma ponumerowane konce */
	inline std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const
		{ return self.getEdgeEnds( edge ); }
	// synonim poprzedniej
	/** \brief Get edge ends.
	 *
	 *  The method gets the pair of vertices on which \a edge is spanned.
	 *  \param edge the considered edge
	 *  \returns the pair of vertices that are the ends of \a edge.	WEN: tj. pierwsza koncowa w .first, druga w .second - krawedz ma ponumerowane konce  */
	std::pair< PVertex,PVertex > getEnds( PEdge edge ) const
		{ return self.getEdgeEnds( edge ); }

	/** \brief Get first vertex.
	 *
	 *  \param edge the considered edge.
	 *  \returns the pointer to the first vertex of \a edge.  */
	inline PVertex getEdgeEnd1( PEdge edge ) const
		{ return self.getEdgeEnd1( edge ); }

	/** \brief Get second vertex.
	 *
	 *  \param edge the considered edge
	 *  \returns the pointer to the second vertex of \a edge. */
	inline PVertex getEdgeEnd2( PEdge edge ) const
		{ return self.getEdgeEnd2( edge ); }

	// orientacja krawedzi wzgledem jej konca
	/** \brief Get edge direction
	 *
	 *  The method gets direction of edge (with resprect to \a v). Possible values of EdgeDirection are:
	 *  - EdNone   = 0x00 if edge is NULL or not connected WEN: to v,
	 *  - EdLoop   = 0x01 if \a edge is a loop  WEN: connected to v,
	 *  - EdUndir  = 0x02 if \a edge is undirected,
	 *  - EdDirIn  = 0x04 if \a edge is directed and \a v is the second vertex of edge,
	 *  - EdDirOut = 0x08 if \a edge is directed and \a v is the first vertex of edge.
	 *  \param edge the considered edge.
	 *  \param v the reference vertex.
	 *  \returns the edge direction. */
	inline EdgeDirection getEdgeDir( PEdge edge, PVertex v ) const
		{ return self.getEdgeDir( edge,v ); }
	// czy wierzcholek jest koncem krawedzi
	/** \brief Test if edge consist of vertex.
	 *
	 *  \param edge the reference edge (pointer). WEN: kurde, reference to nie jest pointer
	 *  \param vert the considered vertex.
	 *  \return true if the vertex \a vert is one of the \a edge ends and false if \a vert is not \a edge end or \a edge is not a proper edge.
	 */
	bool isEdgeEnd( PEdge edge, PVertex vert ) const
		{ return edge && edge->isEnd( vert ); }
	// synonim
	/** \brief Test if edge consist of vertex.
	 *
	 *  \param edge the considered edge.
	 *  \param vert the considered vertex.
	 *  \return true if the vertex is one of the \a edge ends and false if \a vert is not \a edge end or \a edge is not a proper edge.
	 */
	bool isEnd( PEdge edge, PVertex vert ) const
		{ return edge && edge->isEnd( vert ); }
	// drugi koniec krawedzi
	/** \brief Get another end.
	 *
	 *  For \a edge, the method returns the other (than \a vert) vertex.
	 *  \param edge the considered edge.
	 *  \param vert the reference vertex.
	 *  \return the pointer to other vertex in \a edge WEN: tylko ten other moze byc nim samym w przypadku petli, co wiecej zwraca NULL dla niesasiedniej edge.
	 */
	PVertex getEdgeEnd( PEdge edge, PVertex vert) const;
	// synonim
	/** \brief Get another end.
	 *
	 *  For \a edge, the method returns the other (than \a vert) vertex.
	 *  \param edge the considered edge.
	 *  \param vert the reference vertex.
	 *  \return the pointer to the other vertex in \a edge. WEN: tylko ten other moze byc nim samym w przypadku petli, co wiecej zwraca NULL dla niesasiedniej edge.
	 */
	PVertex getEnd( PEdge edge, PVertex vert) const;

	// czy krawedzie sa incydentne
	/** \brief Test incidence
	 *
	 *  The method tests if two edges are incident i.e. have a common vertex.
	 *  \param edge1 the first considered edge.
	 *  \param edge2 the second considered edge.
	 *  \return true if edges share a vertex, false otherwise.
	 */
	inline bool incid( PEdge edge1, PEdge edge2 ) const;

	// pobranie pol info
	/** \brief Get vertex info
	 *
	 *  \param v the considered vertex.
	 *  \return the vertex info of \a v.
	 */
	VertInfoType getVertInfo( PVertex v ) const;

	/** \brief Get edge info
	 *
	 *  \param e the considered edge.
	 *  \return the edge info of \a e.
	 */
	EdgeInfoType getEdgeInfo( PEdge e ) const;

	// sasiedztwo "otwarte" tj. wierzcholki widoczne z danego v poprzez krawedzie o orientacji wzgledem v zgodnej z maska
	/** \brief Get vertex neighborhood.
	 *
	 *  The set of all adjacent vertices is returned in a container via iterator \a out. The vertex itself is not included. WEN: owszem icluded jezeli direct zawiera EdLoop i przy vert jest petla Only edges with direction consistent with the mask \a direct make adjacency.
	 *  \tparam OutputIterator the iterator class of container in which the target set of vertices is stored.
	 *  \param out the iterator of the output container.
	 *  \param vert the reference vertex.
	 *  \param direct the mask defining the direction of edges that make adjacency.
	 *  \return the number of vertices in the returned set.
	 */
	template< class OutputIterator > int getNeighs( OutputIterator out, PVertex vert, EdgeDirection direct = EdAll ) const;

	/** \brief Get vertex neighborhood.
	 *
	 *  The set of all adjacent vertices is returned. The vertex itself is not included. WEN: owszem icluded jezeli direct zawiera EdLoop i przy vert jest petla Only edges with direction consistent with the mask \a direct make adjacency.
	 *  \param vert the reference vertex.
	 *  \param direct the mask defining the direction of edges that make adjacency.
	 *  \return the set of vertices that form neighborhood of \a vert.
	 */
	Set< PVertex > getNeighSet( PVertex vert, EdgeDirection direct = EdAll ) const;

	/** \brief Get size of neighborhood.
	 *
	 *  The method gets the number of adjacent vertices. The vertex itself is not included. WEN: owszem icluded jezeli direct zawiera EdLoop i przy vert jest petla Only edges with direction consistent with the mask \a direct make adjacency.
	 *  \param vert the reference vertex.
	 *  \param mask the mask defining the direction of edges that make adjacency.
	 *  \return the number of adjacent vertices.
	 */
	int getNeighNo( PVertex vert, EdgeDirection mask = EdAll ) const
		{ return this->getNeighs( blackHole,vert,mask ); }
	// sasiedztwo "domkniete" tj. jw. ale v jest zawsze dolaczany
	/** \brief Get closed neighborhood of vertex.
	 *
	 *  The set of all adjacent vertices plus the vertex itself is returned. Only edges with direction consistent with the mask \a direct make adjacency.
	 *  \param vert the reference vertex.
	 *  \param direct the mask defining the direction of edges that make adjacency.
	 *  \return the set of vertices that form closed neighborhood of \a  vert.
	 */
	Set< PVertex > getClNeighSet( PVertex vert, EdgeDirection direct = EdAll ) const;

	/** \brief Get closed neighborhood of vertex.
	 *
	 *  The method gets the set of vertices adjacent to \a vert and \a vert itself. To store vertices any container WEN: ale na elementy typu PVertex with an iterator is used.
	 *  Only edges with direction consistent with the mask \a direct make adjacency.
	 *  \tparam OutputIterator the iterator class of container in which the target set of vertices is stored.
	 *  \param out the iterator of the output container.
	 *  \param vert the reference vertex.
	 *  \param direct the mask defining the direction of edges that make adjacency.
	 *  \return the number of vertices in container \a out.
	 */
	template< class OutputIterator > int getClNeighs( OutputIterator out, PVertex vert, EdgeDirection direct = EdAll ) const;

	/** \brief Get the size of closed neighborhood.
	 *
	 *  The method gets the size of closed neighborhood set.
	 *  \param vert the reference vertex.
	 *  \param direct the mask defining the direction of edges that make adjacency.
	 *  \return the number of adjacent vertices plus one. WEN: znow pytanie, czy przypadek petli to jest +1 czy nie
	 */
	int getClNeighNo( PVertex vert, EdgeDirection direct = EdAll ) const
		{ return this->getClNeighs( blackHole,vert,direct ); }

	// stopien wierzcholka, uwzgledniamy krawedzie wychodzace z vert w sposob okreslony maska. Roznica w porownaniu z
	// getEdgeNo(vert,direct) - petle jesli zliczane, to podwojnie
	/** \brief Get degree of vertex.
	 *
	 *  The method calculates the vertex degree. It works similarly to getEdgeNo(vert,direct), but each loop WEN: "if included in mask direct" is counted twice.
	 *  \param vert the pointer to the considered vertex.
	 *  \param direct the mask determines the type of direction (with respect to \a vert) of edges that are counted.
	 *  \return the degree of \a vert.
	 */
	inline int deg( PVertex vert, EdgeDirection direct = EdAll ) const
		{ return self.getEdgeNo( vert,direct ) + ((direct & EdLoop) ? self.getEdgeNo( vert,EdLoop ): 0); }

	// maks/min stopien
	/** \brief Get maximum degree.
	 *
	 *  The method calculates the maximum degree over all vertices in the graph.
	 *  \param direct the mask determining the types of edges. WEN: nie bardzo types np. przy EdDirOut nie zlicza krawedzi wchodzacej
	 *  \return the maximum degree of graph.
	 */
	inline int Delta( EdgeDirection direct = EdAll ) const
		{ return std::max( 0,this->maxDeg( direct ).second ); }

	/** \brief Get minimum degree
	 *
	 *  The minimum degree over all vertices in the graph is returned.
	 *  \param direct the mask determining the types of edges. WEN: nie bardzo types np. przy EdDirOut nie zlicza krawedzi wchodzacej
	 *  \return the minimum degree of graph.
	 */
	inline int delta( EdgeDirection direct = EdAll ) const
		{ return std::max( 0,this->minDeg( direct ).second );  }
	// j.w. ale zwracany jest takze wierzcholek realizujacy ekstremum. (NULL,-1) przy braku wierzcholkow
	/** \brief Get minimum degree.
	 *
	 *  Method gets the minimum degree over all vertices in the graph and one vertex of such degree.
	 *  \param direct the mask determining the types of edges. WEN: nie bardzo types np. przy EdDirOut nie zlicza krawedzi wchodzacej
	 *  \return The standard pair: minimum vertex and the minimum degree of graph.
	 */
	std::pair< PVertex,int > minDeg( EdgeDirection direct = EdAll ) const;

	/** \brief Get maximum degree.
	 *
	 *  The method gets the maximum degree over all vertices in the graph and one vertex of such degree.
	 *  \param direct the mask determining the types of edges. WEN: nie bardzo types np. przy EdDirOut nie zlicza krawedzi wchodzacej
	 *  \return the standard pair: maximum vertex and the maximum degree of graph.
	 */
	std::pair< PVertex,int > maxDeg( EdgeDirection direct = EdAll ) const;

	// zapis "macierzy sasiedztwa" z uwglednieniem krawedzi typu zawartego w masce do 2-wym. tablicy assocjacyjnej
	// (PVertex,PVertex)-> cos konwertowalne z bool
	/** \brief Get adjacency matrix
	 *
	 *  The method gets adjacency matrix that is kept in associative container \a cont.
	 *  A key of \a cont is a pair of vertices, and mapped value is of any type convertible to bool type.
	 *  A mapped value is true if there is an edge WEN: raczej connection? between the vertices in the pair.
	 *  \param cont the reference to an associative container in which keys are pairs of vertices and mapped values are convertible to bool type.
	 *  \param mask determines the types of edges to be stored in cont.
	 *  \return none.
	 */
	template< class Cont > void getAdj( Cont &cont, EdgeType mask = EdAll ) const;

	// Informacja o tym, czy krawedzie sa rownolegle.
	// rownoleglosc: 3 typy relacji rownowaznosci, okreslane maska bitowa
	// zawsze krawedz jest rownolegla do tak samo skierowanej krawedzi tego samego typu
	//reltype - dopuszczalne tylko jednobitowe: EdDirIn, EdDirOut lub EdUndir (taki jest sens parametru maski we wszystkich metodach dot. rownoleglosci)
	//EdDirOut - luk nieskierowany jest rownolegly tylko do tak samo jak on skierowanego luku o tych samych koncach
	//EdDirIn - jest on takze rownolegly do odwrotnie skierowanego luku o tych samych koncach
	//EdUndir - jest on takze rownolegly do krawedzi nieskierowanej o tych samych koncach
	/** \brief Test if parallel.
	 *
	 *  The method tests if two edges are parallel. Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices and are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \param ed1 the first considered edge.
	 *  \param ed2 the second considered edge.
	 *  \param reltype determines the type of parallelism.
	 *  \return true if edges are parallel, false otherwise.
	 */
	bool areParallel( PEdge ed1, PEdge ed2, EdgeDirection reltype = EdUndir ) const;

	// zbior krawedzi rownoleglych do zadanej z wykluczeniem jej samej
	/** \brief Get parallel edges.
	 *
	 *  The method gets edges parallel to \a ed. WEN: ale bez ed The edges are stored in a container defined by \a iter. Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *  \tparam OutputIterator the iterator class of the container used to store edges received by the method.
	 *  \param[out] iter the iterator of container used to store output edges.
	 *  \param ed the considered edge.
	 *  \param reltype determines the type of parallelism.
	 *  \return the number of parallel edges stored in the container.
	 */
	template< class OutputIterator > int getParals( OutputIterator iter, PEdge ed, EdgeDirection reltype = EdUndir ) const;

	/** \brief Get set of parallel edges.
	 *
	 *  The method gets and returns the set of edges parallel to \a ed. WEN: ale bez ed  Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *  \param ed the considered edge.
	 *  \param reltype determines the type of parallelism.
	 *  \return the set of parallel edges.
	 */
	Set< PEdge > getParalSet( PEdge ed, EdgeDirection reltype = EdUndir ) const;

	// liczba krawedzi rownoleglych do danej wliczajac ja sama
	/** \brief Number of parallel edges.
	 *
	 *  The method gets the number of edges parallel to \a ed including itself. Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if the are spanned on they same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if the are spanned on they same vertices.
	 *
	 *  \param edge the reference edge.
	 *  \param reltype determines the type of parallelism.
	 *  \return the number of parallel edges.
	 */
	int mu( PEdge edge, EdgeDirection reltype = EdUndir ) const
		{ return this->getParals( blackHole,edge,reltype ) + 1; }
	// jej maks. wartosc po wszystkich krawedziach
	/** \brief Maximum number of parallel edges.
	 *
	 *  The method gets the maximum number of parallel edges. Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \param reltype determines the type of parallelism.
	 *  \return the maximum number of parallel edges.
	 */
	int mu( EdgeDirection reltype = EdUndir ) const
		{ return maxMu( reltype ).second; }

	// jw. wraz z krawedzia realizujaca to ekstremum
	/** \brief Maximum number of parallel edges.
	 *
	 *  The method gets the maximum number of parallel edges and one of maximal edges. Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \param reltype determines the type of parallelism.
	 *  \return the standard pair consisting of a pointer to the maximal edge and the maximum number of parallel edges.
	 */
	std::pair< PEdge,int > maxMu( EdgeDirection reltype = EdUndir ) const;

	// rozdziela dany zbior krawedzi na dwa. W pierwszym jest po jednym reprezentancie z kazdej klasy relacji
	// rownoleglosci. Drugi to pozostale krawedzie z tego zbioru
	/** \brief Find parallel edges.
	 *
	 *  The method splits the given set of edges into two sets and keeps them in two containers. The first set consists of unique representatives of edges.
	 *  The second set contains all the other edges. Three types of parallelism are possible.
	 WEN: gdzie tu jest podany zwiazek representatives z paralelizmem??? Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if the are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if the are spanned on the same vertices.
	 *
	 *  \tparam IterOut1 the iterator class of the container for the first output set (unique edges).
	 *  \tparam IterOut2 the iterator class of the container for the second output set (residue).
	 *  \tparam Iterator the iterator class of the container for the input set of edges.
	 *  \param out the standard pair of iterators:\n
	 *      The first is bound with the container consisting of unique edges representatives.\n
	 *      The second is bound with the container holding the residue.
	 *  \param begin iterator to the first element of the input container.
	 *  \param end iterator to the WEN: za-last element of the input container.
	 *  \param reltype determines the type of parallelism.
	 *  \return the standard pair of integers, which corresponds with the size of the first and second output container.
	 */
	template< class IterOut1, class IterOut2, class Iterator >
		std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 > out, Iterator begin, Iterator end, EdgeType reltype = EdUndir ) const;

	// wersja odporna na ew. powtorzenia - ignorowane w ciagu wejsciowym
	/** \brief Find parallel edges.
	 * WEN: j.w.
	 *  The method splits the given set of edges into two sets and keeps them in two containers. The first set consists of unique representatives of edges.
	 *  The second set contains all the other edges. Repetition-proof version of \a  findParals. Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \tparam IterOut1 the iterator class of the container for first output set (unique edges).
	 *  \tparam IterOut2 the iterator class of the container for second output set (residue).
	 *  \tparam Iterator the iterator class of the container for input set of edges.
	 *  \param out the standard pair of iterators:\n
	 *      The first is bound with the container consisting of unique edges representatives.\n
	 *      The second is bound with the container holding the residue.
	 *  \param begin the iterator to the first element of the input container.
	 *  \param end the iterator to the WEN: za-last element of the input container.
	 *  \param reltype determines the type of parallelism.
	 *  \return the standard pair of integers which corresponds with the size of the first and second output container.
	 */
	template< class IterOut1, class IterOut2, class Iterator >
		std::pair< int,int > findParals2( std::pair< IterOut1,IterOut2 > out, Iterator begin, Iterator end, EdgeType reltype = EdUndir ) const;

	/** \brief Find parallel edges.
	 *  WEN: j.w.
	 *  The method splits the given set of edges into two sets and keeps them in two containers. The first set consists of unique representatives of edges.
	 *  The second set contains all the other edges. Three types of parallelism are possible. Depending on\a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \tparam IterOut1 the iterator class of the container for the first output set (unique edges).
	 *  \tparam IterOut2 the iterator class of the container for the second output set (residue).
	 *  \param out the standard pair of iterators:\n
	 *      The first is bound with the container consisting of unique edges representatives.\n
	 *      The second is bound with the container holding the residue.
	 *  \param eset the reference to the set of edges.
	 *  \param relType determines the type of parallelism.
	 *  \return the standard pair of integers which corresponds with the size of the first and second output container.	 */
	template< class IterOut1, class IterOut2 >
		std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 > out, const Set< PEdge > &eset,
			EdgeType relType = EdUndir ) const
			{ return this->findParals( out, eset.begin(),eset.end(),relType ); }
	// zbiorem wejsciowym sa wszystkie krawedzie przy wierzcholku
	/** \brief Find parallel edges.
	 *  WEN: j.w.
	 *  The method splits the edges incident to vertex into two sets and keeps them in two containers. The first set consists of unique representatives of edges.
	 *  The second set contains all the other edges. Three types of parallelism are possible. Depending on reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \tparam IterOut1 the iterator class of the container for first output set (unique edges).
	 *  \tparam IterOut2 the iterator class of the container for second output set (residue).
	 *  \param out the standard pair of iterators:\n
	 *      The first is bound with the container consisting of unique edges representatives.\n
	 *      The second is bound with the container holding the residue.
	 *  \param vert the reference vertex.
	 *  \param reltype determines the type of parallelism.
	 *  \return the standard pair of integers which corresponds with the size of the first and second output container.
	 */
	template< class IterOut1, class IterOut2 >
		std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 > out, PVertex vert, EdgeType reltype = EdUndir ) const;

	// zbiorem wejsciowym sa wszystkie krawedzie miedzy para wierzcholkow
	/** \brief Find parallel edges.
	 * WEJ:j.w.
	 *  The method splits the set of edges spanned on two vertices into two sets and keeps them in two containers. The first set consists of unique representatives of edges.
	 *  The second set contains all the other edges. Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \tparam IterOut1 the iterator class of the container for the first output set (unique edges).
	 *  \tparam IterOut2 the iterator class of the container for the second output set (residue).
	 *  \param out the standard pair of iterators:\n
	 *      The first is bound with the container consisting of unique edges representatives.\n
	 *      The second is bound with the container holding the residue.
	 *  \param vert1 the first reference vertex.
	 *  \param vert2 the second reference vertex.
	 *  \param reltype determines the type of parallelism.
	 *  \return the standard pair of integerw which corresponds with the size of the first and second output container.
	 */
	template< class IterOut1, class IterOut2 >
		std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 > out, PVertex vert1, PVertex vert2, EdgeType reltype = EdUndir ) const;

	// zbiorem wejsciowym sa wszystkie krawedzie
	/** \brief Find parallel edges.
	 * WEN:j.w.
	 *  The method splits the set of all edges into two sets and keeps them in two containers. The first set consists of unique representatives of edges.
	 *  The second set contains all the other edges.Three types of parallelism are possible. Depending on \a reltype:
	 *  - EdDirOut - two edges are considered to be parallel if they are spanned on the same vertices, are of the same type and direction.
	 *  - EdDirIn - edges are considered to be parallel if they are spanned on the same vertices and are of the same type.
	 *  - EdUndir - edges are considered to be parallel if they are spanned on the same vertices.
	 *
	 *  \tparam IterOut1 iterator the class of the container for the first output set (unique edges).
	 *  \tparam IterOut2 iterator the class of the container for the second output set (residue).
	 *  \param out the standard pair of iterators:\n
	 *      The first is bound with the container consisting of unique edges representatives.\n
	 *      The second is bound with the container holding the residue.
	 *  \param reltype determines the type of parallelism.
	 *  \return the standard pair of integers which corresponds with the size of the first and second output container.
	 */
	template< class IterOut1, class IterOut2 >
		std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 > out, EdgeType reltype = EdUndir ) const;

	// zbior krawedzi wychodzacych z ktoregos z wierzcholkow z podanego zbioru w sposob podany pierwsza maska
	// druga maska filtruje, czy maja to byc krawedzie prowadzace do tego zbioru (Loop), poza ten zbior (ustawiony bit
	// inny niz Loop), czy wszystkie (ustawiony Loop i ktorys inny)
	/** \brief Get incident edges
	 *
	 *  The method gets the edges incident to the set of vertices defined by iterators \a beg and \a end.
        WEN: powtorzenia w ciagu wejsciowym sa dozwolone ale ignorowane
	 *  Three modes are possible, depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, the edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is WEN: raczej equal congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which the mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \tparam Iterator the iterator class of the container for input vertices.
	 *  \tparam OutIter class of iterator of the set of returned edges.
	 *  \param[out] out the iterator of the container storing edges
	 *  \param beg the iterator of the first vertex of the vertex set.
	 *  \param end the iterator of the WEN: za-last last vertex of the vertex set.
	 *  \param type the mask determining the type of direction WEN: przy wierzcholku ze zbioru wejsciowego of considered edges.
	 *  \param kind determines the mode.
	 *  \return the number of incident edges returned via the parameter \a out.
	 */
	template< class Iterator,class OutIter >
		int getIncEdges( OutIter out, Iterator beg, Iterator end, EdgeDirection type = EdAll, EdgeType kind = Loop ) const;

	/** \brief Get incident edges
	 *  WEN:j.w.
	 *  The method gets the edges incident to the set of vertices defined by iterators \a beg and \a end.
	 *  Three modes are possible, depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, the edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which the mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \tparam Iterator the iterator class of the container for input vertices.
	 *  \param beg the iterator of the first vertex of the vertex set.
	 *  \param end the iterator of the WEN: za-last vertex of the vertex set.
	 *  \param type the mask determining the type of direction of considered edges.
	 *  \param kind determines the mode.
	 *  \return the set of incident edges.
	 */
	template< class Iterator >
		Set< PEdge > getIncEdgeSet( Iterator beg, Iterator end, EdgeDirection type  = EdAll, EdgeType kind = Loop ) const;

	/** \brief Get incident edges
	 *
	 *  The method gets the edges incident to the set of vertices defined by \a vset.
	 *  Three modes are possible, depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, the edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which the mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \param out the iterator of the container storing output vertices.
	 *  \param vset the set of vertices.
	 *  \param type the mask determining the type of direction of considered edges.
	 *  \param kind determines the mode.
	 *  \return the number of incident edges returned in parameter \a out.
	 */
	template< class OutIter >
		int getIncEdges( OutIter out, const Set< PVertex > &vset, EdgeDirection type = EdAll, EdgeType kind = Loop ) const
		{ return this->getIncEdges( out,vset.begin(),vset.end(),type,kind ); }

	/** \brief Get incident edges.
	 * WEN:j.w. oprocz powtorzen
	 *  The method gets the edges incident to the set of vertices defined by \a vset.
	 *  Three modes are possible, depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, the edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which the mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \param vset the set of vertices.
	 *  \param type the mask determining the type of direction of considered edges.
	 *  \param kind determines the mode.
	 *  \return the set of incident edges.
	 */
	Set< PEdge > getIncEdgeSet( const Set< PVertex > &vset, EdgeDirection type = EdAll, EdgeType kind = Loop ) const;

	// Podobnie j.w. ale tym razem pobieramy drugie konce (tj. wierzcholki) od takich krawedzi.
	/** \brief Get adjacent vertices.
	 *  WEN: j.w.
	 *  The method gets vertices adjacent to vertices in the set defined by the iterators \a beg and \a end.
	 *  Three modes are possible depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which the mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \tparam Iterator the iterator class of the container for input vertices.
	 *  \tparam OutIter the iterator class of the set of output vertices.
	 *  \param out the iterator of the container storing output vertices.
	 *  \param beg the iterator of the first vertex of the vertex set.
	 *  \param end the iterator of the WEN: za-last vertex of the vertex set.
	 *  \param type mask determining the type of direction of considered edges.
	 *  \param kind determines the mode.
	 *  \return the number of adjecent vertices returned in the parameter \a out.
	 */
	template< class Iterator,class OutIter >
		int getIncVerts( OutIter out, Iterator beg, Iterator end, EdgeDirection type = EdAll,EdgeType kind = Loop ) const;

	/** \brief Get adjacent vertices.
	 * WEN:j.w. oraz powtorzenia w ciagu wejsciowym sa dozwolone ale ignorowane
	 *  The method gets vertices adjacent to vertices in the set defined by the iterators \a beg and \a end.
	 *  Three modes are possible depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which the mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \tparam Iterator the iterator class for the container for input vertices.
	 *  \param beg the iterator of the first vertex of the vertex set.
	 *  \param end the iterator of the WEN:za-last vertex of the vertex set.
	 *  \param type the mask determining the type of direction of considered edges.
	 *  \param kind determines the mode.
	 *  \return the set of adjecent vertices.
	 */
	template< class Iterator >
		Set< PVertex > getIncVertSet( Iterator beg, Iterator end, EdgeDirection type = EdAll, EdgeType kind = Loop ) const;

	/** \brief Get adjacent vertices.
	 *  WEN:j.w.
	 *  The method gets vertices adjacent to vertices in the set \a vset.
	 *  Three modes are possible depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \tparam OutIter class of iterator of the set of the output vertices.
	 *  \param out the iterator of the container storing the output vertices.
	 *  \param vset the set of input vertices.
	 *  \param type the mask determining the type of direction of the considered edges.
	 *  \param kind determines the mode.
	 *  \return the number of adjecent vertices returned in the parameter \a out.  */
	template< class OutIter >
		int getIncVerts( OutIter out, const Set< PVertex > &vset, EdgeDirection type = EdAll, EdgeType kind = Loop ) const
		{ return this->getIncVerts( out,vset.begin(),vset.end(),type,kind ); }

	/** \brief Get adjacent vertices.
	 *  WEN:j.w.
	 *  The method gets vertices adjacent to the vertices in the set \a vset.
	 *  Three modes are possible depending on the \a kind:
	 *  - if \a kind is congruent with Directed or Undirected, edges with one vertex outside the vertex set are taken.
	 *  - if \a kind is congruent with Loop, the edges with both vertices inside the vertex set are taken.
	 *  - the option in which mask \a kind is congruent with both the above-mentioned is also available.
	 *
	 *  \param vset the set of input vertices.
	 *  \param type the mask determining the type of direction of the considered edges.
	 *  \param kind determines mode.
	 *  \return the number of adjecent vertices returned in the parameter \a out.  */
	Set< PVertex > getIncVertSet( const Set< PVertex > &vset, EdgeDirection type = EdAll, EdgeType kind = Loop ) const;

    //NEW:
	inline bool hasAdjMatrix() const
	{   return self.hasAdjMatrix();  }

	static bool allowedAdjMatrix()
	{   return GraphType::allowedAdjMatrix();  }
};

#include "grconst.hpp"

#endif
