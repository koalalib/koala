// DijkstraBasePar

template< class DefaultStructs > template< class DType, class GraphType > template< class Rec >
	void DijkstraBasePar< DefaultStructs >::VertLabs< DType,GraphType >::copy( Rec &rec ) const
{
	rec.distance = distance;
	rec.vPrev = vPrev;
	rec.ePrev = ePrev;

}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EdgeContainer >
	typename EdgeContainer::ValType::DistType DijkstraBasePar< DefaultStructs >::distances( const GraphType &g,
		VertContainer &avertTab, const EdgeContainer &edgeTab, typename GraphType::PVertex start,
		typename GraphType::PVertex end )
{
	koalaAssert( start,AlgExcNullVert );
	const typename EdgeContainer::ValType::DistType Zero = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::zero();
	const typename EdgeContainer::ValType::DistType PlusInfty = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::plusInfty();
	int n;
	typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type localvertTab, Q( n = g.getVertNo() );
	typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::Type &vertTab =
			BlackHoleSwitch< VertContainer,typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
			VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::get( avertTab,localvertTab );

	typename GraphType::PVertex U,V;
	if (DefaultStructs::ReserveOutAssocCont || isBlackHole( avertTab )) vertTab.reserve( n );

	Q[start].vPrev = 0;
	Q[start].ePrev = 0;
	Q[start].distance = Zero;

	while (!Q.empty())
	{
		typename EdgeContainer::ValType::DistType d = PlusInfty, nd;
		for( V = Q.firstKey(); V; V = Q.nextKey( V ) )
			if (Q[V].distance < d) d = Q[U = V].distance;
		Q[U].copy( vertTab[U] );
		Q.delKey( U );
		if (U == end) return vertTab[end].distance;

		for( typename GraphType::PEdge E = g.getEdge( U,Koala::EdDirOut | Koala::EdUndir ); E;
			E = g.getEdgeNext( U,E,Koala::EdDirOut | Koala::EdUndir ) )
			if (!vertTab.hasKey( V = g.getEdgeEnd( E,U ) ))
				if ((nd = vertTab[U].distance + edgeTab[E].length) < Q[V].distance)
				{
					Q[V].distance = nd;
					Q[V].ePrev = E;
					Q[V].vPrev = U;
				}
	}
	return end ? PlusInfty : Zero;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class VIter, class EIter >
	int DijkstraBasePar< DefaultStructs >::getPath( const GraphType &g, const VertContainer &vertTab,
		typename GraphType::PVertex end, ShortPathStructs::OutPath< VIter,EIter > iters )
{
	koalaAssert( end,AlgExcNullVert );
	const typename VertContainer::ValType::DistType PlusInfty = DefaultStructs:: template
		NumberTypeBounds< typename VertContainer::ValType::DistType >::plusInfty();

	if (PlusInfty == vertTab[end].distance)
		return -1; // wierzcholek end jest nieosiagalny

	return ShortPathStructs::getOutPath( g,vertTab,iters,end );
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EdgeContainer >
	typename EdgeContainer::ValType::DistType DijkstraBasePar< DefaultStructs >::distancesOnHeap( const GraphType &g,
		VertContainer &avertTab, const EdgeContainer &edgeTab, typename GraphType::PVertex start,
		typename GraphType::PVertex end )
{
	koalaAssert( start,AlgExcNullVert );
	const typename EdgeContainer::ValType::DistType Zero = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::zero();
	const typename EdgeContainer::ValType::DistType PlusInfty = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::plusInfty();
	int n;
	typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type localvertTab;
	typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
		VertLabsQue< typename EdgeContainer::ValType::DistType,GraphType > >::Type Q( n = g.getVertNo() );
	typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::Type &vertTab =
			BlackHoleSwitch< VertContainer,typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
			VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::get( avertTab,localvertTab );

	typename GraphType::PVertex U,V;

	if (DefaultStructs::ReserveOutAssocCont || isBlackHole( avertTab )) vertTab.reserve( n );

	Privates::BlockListAllocator< typename DefaultStructs:: template
		HeapCont< typename GraphType::PVertex,void,void >::NodeType > alloc( n );
	typename DefaultStructs:: template HeapCont< typename GraphType::PVertex,
		Cmp< typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
		VertLabsQue< typename EdgeContainer::ValType::DistType,GraphType > >::Type >,
		Privates::BlockListAllocator< typename DefaultStructs:: template
		HeapCont< typename GraphType::PVertex,void,void>::NodeType > >::Type heap( &alloc,makeCmp( Q ) );

	Q[start].vPrev = 0;
	Q[start].ePrev = 0;
	Q[start].distance = Zero;
	Q[start].repr = heap.push( start );

	while (!Q.empty())
	{
		typename EdgeContainer::ValType::DistType d,nd;
		U = heap.top();
		d = Q[U].distance;
		Q[U].copy( vertTab[U] );
		heap.del( (typename DefaultStructs::template HeapCont< typename GraphType::PVertex,void,void >::NodeType*)Q[U].repr );
		Q.delKey( U );
		if (U == end) return vertTab[end].distance;

		for( typename GraphType::PEdge E = g.getEdge( U,Koala::EdDirOut | Koala::EdUndir ); E;
			E = g.getEdgeNext( U,E,Koala::EdDirOut | Koala::EdUndir ) )
			if (!vertTab.hasKey( V = g.getEdgeEnd( E,U ) ))
				if ((nd = vertTab[U].distance + edgeTab[E].length) < Q[V].distance)
				{
					if (Q[V].repr)
						heap.del( (typename DefaultStructs::template HeapCont<
							typename GraphType::PVertex,void,void >::NodeType*)Q[V].repr );
					Q[V].distance = nd;
					Q[V].ePrev = E;
					Q[V].vPrev = U;
					Q[V].repr = heap.push( V );
				}
	}
	return end ? PlusInfty : Zero;
}


// DAGCritPathPar

template< class DefaultStructs > template< class DType > template< class T >
	typename DAGCritPathPar< DefaultStructs >::template UnitLengthEdges< DType >::ValType
	DAGCritPathPar< DefaultStructs >::UnitLengthEdges< DType >::operator[]( T e ) const
{
	ValType res;
	res.length = DefaultStructs:: template NumberTypeBounds< DType >::one();
	return res;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EdgeContainer >
	typename EdgeContainer::ValType::DistType DAGCritPathPar< DefaultStructs >::critPathLength(
		const GraphType &g, VertContainer &avertTab, const EdgeContainer &edgeTab,
		typename GraphType::PVertex start, typename GraphType::PVertex end )
{
	const typename EdgeContainer::ValType::DistType Zero = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::zero();
	const typename EdgeContainer::ValType::DistType MinusInfty = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::minusInfty();

	typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type localvertTab;
	typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::Type &vertTab =
			BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
			VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::get( avertTab,localvertTab );

	typename GraphType::PVertex U,V;
	typename EdgeContainer::ValType::DistType nd;
	int ibeg,iend,n = g.getVertNo();

	if (DefaultStructs::ReserveOutAssocCont || isBlackHole( avertTab )) vertTab.reserve( n );

	if (start)
	{
		vertTab[start].vPrev = 0;
		vertTab[start].ePrev = 0;
		vertTab[start].distance = Zero;
		if (start == end) return Zero;
	}

	typename DefaultStructs::template AssocCont< typename GraphType::PVertex,char >::Type followers( start ? n : 0 );
	typename GraphType::PVertex LOCALARRAY( tabV,n );
	if (start)
	{
		Koala::BFSPar< DefaultStructs >::scanAttainable( g,start,assocInserter( followers,constFun< char >( 0 )),EdDirOut );
		Koala::DAGAlgsPar< DefaultStructs >::topOrd( makeSubgraph( g,std::make_pair( assocKeyChoose( followers ),
			stdChoose( true ) ) ),tabV );
		ibeg = 1;
		iend = followers.size();
	}
	else
	{
		Koala::DAGAlgsPar< DefaultStructs >::topOrd( g,tabV );
		ibeg = 0;
		iend = n;
	}

	for( int i = ibeg; i < iend; i++ )
	{
		U = tabV[i];
		vertTab[U].vPrev = 0;
		vertTab[U].ePrev = 0;
		vertTab[U].distance = (g.getEdgeNo( U,EdDirIn )) ? MinusInfty : Zero;

		for( typename GraphType::PEdge E = g.getEdge( U,Koala::EdDirIn ); E; E = g.getEdgeNext( U,E,Koala::EdDirIn ) )
		{
			V = g.getEdgeEnd( E,U );
			if ((!start) || followers.hasKey( V ))
			{
				nd = vertTab[V].distance + edgeTab[E].length;
				if (nd > vertTab[U].distance)
				{
					vertTab[U].distance = nd;
					vertTab[U].ePrev = E;
					vertTab[U].vPrev = V;
				}
			}
		}
		if (U == end) return vertTab[U].distance;
	}
	return end ? MinusInfty : Zero;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class VIter, class EIter > int
	DAGCritPathPar< DefaultStructs >::getPath( GraphType &g, const VertContainer &vertTab,
		typename GraphType::PVertex end, ShortPathStructs::OutPath< VIter,EIter > iters )
{
	koalaAssert( end,AlgExcNullVert );
	if (DefaultStructs:: template NumberTypeBounds< typename VertContainer::ValType::DistType >
		::isMinusInfty( vertTab[end].distance )) return -1;
	return ShortPathStructs::getOutPath( g,vertTab,iters,end );
}

// BellmanFordPar

template< class DefaultStructs > template< class GraphType, class VertContainer, class EdgeContainer >
	typename EdgeContainer::ValType::DistType BellmanFordPar< DefaultStructs >::distances( const GraphType &g,
		VertContainer &avertTab, const EdgeContainer &edgeTab, typename GraphType::PVertex start,
		typename GraphType::PVertex end )
{
	koalaAssert( start,AlgExcNullVert );
	typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type localvertTab;
	typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
		VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::Type &vertTab =
			BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
			VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type >::get( avertTab,localvertTab );

	typename GraphType::PVertex U,V;

	const typename EdgeContainer::ValType::DistType inf = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::plusInfty();
	const typename EdgeContainer::ValType::DistType zero = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::zero();
	const typename EdgeContainer::ValType::DistType minusInf = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::minusInfty();
	typename EdgeContainer::ValType::DistType nd;
	int n = g.getVertNo();
	vertTab.reserve( n );

	bool existNegCycle = false;

	//sprawdzenie czy nie ma petli ujemnych
	for( typename GraphType::PEdge E = g.getEdge( Koala::EdLoop | Koala::EdUndir ); E;
		E = g.getEdgeNext( E,Koala::EdLoop | Koala::EdUndir ) )
		if (edgeTab[E].length < zero) return minusInf;

	//inicjalizacja
	//for each v: d[v] <- INF (to jest zrealizowane juz przy tworzeniu vertTab)
	//f[s] <- NIL
	vertTab[start].vPrev = 0;
	vertTab[start].ePrev = 0;
	//d[s] <- 0
	vertTab[start].distance = zero;

	//for 1 to n-1:
	//  for each (u,v):
	//      if  d[u]+w(u,v) < d[v]:
	//          d[v] <- d[u]+w(u,v) and vPrev[v] <- u and ePrev[v] <- (u,v)
	for( int i = 1; i < n; i++ )
	{
		//relaksacja krawedzi nieskierowanych
		for( typename GraphType::PEdge E = g.getEdge( Koala::EdUndir ); E; E = g.getEdgeNext( E,Koala::EdUndir ) )
		{
			if (vertTab[U = g.getEdgeEnd1( E )].distance < inf && (nd = vertTab[U].distance + edgeTab[E].length) <
				vertTab[V = g.getEdgeEnd2( E )].distance)
			{
				vertTab[V].distance = nd;
				vertTab[V].ePrev = E;
				vertTab[V].vPrev = U;
			}
			else if (vertTab[U = g.getEdgeEnd2( E )].distance < inf && (nd = vertTab[U].distance + edgeTab[E].length) <
				vertTab[V = g.getEdgeEnd1( E )].distance)
			{
				vertTab[V].distance = nd;
				vertTab[V].ePrev = E;
				vertTab[V].vPrev = U;
			}
		}
		//relaksacja krawedzi (u,v) skierowanych u->v
		for( typename GraphType::PEdge E = g.getEdge( Koala::EdDirOut ); E; E = g.getEdgeNext( E,Koala::EdDirOut ) )
			if (vertTab[U = g.getEdgeEnd1( E )].distance < inf && (nd = vertTab[U].distance + edgeTab[E].length) <
				vertTab[V = g.getEdgeEnd2( E )].distance)
			{
				vertTab[V].distance = nd;
				vertTab[V].ePrev = E;
				vertTab[V].vPrev = U;
			}
	}

	//sprawdzenie czy nie ma cykli ujemnych
	for( typename GraphType::PEdge E = g.getEdge( Koala::EdUndir ); E; E = g.getEdgeNext( E,Koala::EdUndir ) )
		if (vertTab[U = g.getEdgeEnd1( E )].distance < inf && (nd = vertTab[U].distance + edgeTab[E].length) <
			vertTab[V = g.getEdgeEnd2( E )].distance)
		{
			existNegCycle = true;
			break;
		}
		else if (vertTab[U = g.getEdgeEnd2( E )].distance < inf && (nd = vertTab[U].distance + edgeTab[E].length) <
			vertTab[V = g.getEdgeEnd1( E )].distance)
		{
			existNegCycle = true;
			break;
		}

	if (!existNegCycle)
		for( typename GraphType::PEdge E = g.getEdge( Koala::EdDirOut ); E; E = g.getEdgeNext( E,Koala::EdDirOut ) )
			if (vertTab[U = g.getEdgeEnd1( E )].distance < inf && (nd = vertTab[U].distance + edgeTab[E].length) <
				vertTab[V = g.getEdgeEnd2(E)].distance)
			{
				existNegCycle = true;
				break;
			}

	if (existNegCycle) return minusInf;
	//jezeli nie ma cykli ujemnych to mozemy zwrocic wynik
	return end ? vertTab[end].distance : zero;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class VIter, class EIter > int
	BellmanFordPar< DefaultStructs >::getPath( const GraphType &g, VertContainer &vertTab,
		typename GraphType::PVertex end, ShortPathStructs::OutPath< VIter,EIter > iters )
{
	koalaAssert( end,AlgExcNullVert );
	if (DefaultStructs:: template NumberTypeBounds< typename VertContainer::ValType::DistType >
		::isPlusInfty(vertTab[end].distance)) return -1; // wierzcholek end jest nieosiagalny
	else if (DefaultStructs:: template NumberTypeBounds< typename VertContainer::ValType::DistType >
		::isMinusInfty(vertTab[end].distance)) return -2; // w grafie jest cykl ujemny
	return ShortPathStructs::getOutPath( g,vertTab,iters,end );
}


template< class DefaultStructs > template< class GraphType, class TwoDimVertContainer, class VIter, class EIter > int
	FloydPar< DefaultStructs >::getOutPathFromMatrix( const GraphType &g, const TwoDimVertContainer &vertMatrix,
	OutPath< VIter,EIter > iters, typename GraphType::PVertex start, typename GraphType::PVertex end )
{
	koalaAssert( end,AlgExcNullVert );
	typename GraphType::PVertex v = vertMatrix( start,end ).vPrev;
	typename GraphType::PEdge e = vertMatrix( start,end ).ePrev;
	typename GraphType::PVertex LOCALARRAY( tabV,g.getVertNo() );
	typename GraphType::PEdge LOCALARRAY( tabE,g.getVertNo() );
	int len = 0;

	if (end != start)
		for( ; v; len++ )
		{
			tabV[len] = v;
			tabE[len] = e;
			e = vertMatrix( start,v ).ePrev;
			v = (v == start) ? 0 : vertMatrix( start,v ).vPrev;
		}

	for( int i = len - 1; i >= 0; i-- )
	{
		*iters.vertIter = tabV[i];
		*iters.edgeIter = tabE[i];
		++iters.vertIter;
		++iters.edgeIter;
	}
	*iters.vertIter = end;
	++iters.vertIter;
	return len;
}

template< class DefaultStructs > template< class DType > template< class T >
	typename FloydPar< DefaultStructs >::template UnitLengthEdges< DType >::ValType
	FloydPar< DefaultStructs >::UnitLengthEdges< DType >::operator[]( T e ) const
{
	ValType res;
	res.length = DefaultStructs:: template NumberTypeBounds< DType >::one();
	return res;
}

template< class DefaultStructs > template< class GraphType, class TwoDimVertContainer, class EdgeContainer > bool
	FloydPar< DefaultStructs >::distances( const GraphType &g, TwoDimVertContainer &vertMatrix,
		const EdgeContainer &edgeTab )
{
	const typename EdgeContainer::ValType::DistType inf = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::plusInfty();
	const typename EdgeContainer::ValType::DistType zero = DefaultStructs:: template
		NumberTypeBounds< typename EdgeContainer::ValType::DistType >::zero();
	bool existNegCycle = false; //jezeli existNegCycle jest ustawiona to w grafie istnieje cykl ujemny

	//sprawdzenie czy nie ma petli ujemnych
	for( typename GraphType::PEdge E = g.getEdge( Koala::EdLoop | Koala::EdUndir ); E;
		E = g.getEdgeNext( E,Koala::EdLoop | Koala::EdUndir ) )
		if (edgeTab[E].length < zero) return false;

	vertMatrix.reserve( g.getVertNo() );

	//inicjalizacja - ustawiam wartosci poczatkowe odleglosci w tablicy asocjacyjnej
	for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
		for( typename GraphType::PVertex V = g.getVert(); V; V = g.getVertNext( V ) )
			if (U == V) vertMatrix( U,U ).distance = zero;
			else vertMatrix( U,V ).distance = inf;

	for( typename GraphType::PEdge E = g.getEdge( Koala::EdDirOut ); E;
		E = g.getEdgeNext( E,Koala::EdDirOut ) )
	{
		typename GraphType::PVertex U = g.getEdgeEnd1( E ), V = g.getEdgeEnd2( E );
		if (edgeTab[E].length < vertMatrix(U,V).distance)
		{
			vertMatrix( U,V ).distance = edgeTab[E].length;
			vertMatrix( U,V ).ePrev = E;
			vertMatrix(U,V).vPrev = U;
		}
	}

	for( typename GraphType::PEdge E = g.getEdge( Koala::EdUndir ); E; E = g.getEdgeNext( E,Koala::EdUndir ) )
	{
		typename GraphType::PVertex U = g.getEdgeEnd1( E ), V = g.getEdgeEnd2( E ), X;
		if (edgeTab[E].length < vertMatrix( U,V ).distance)
		{
			vertMatrix( U,V ).distance = edgeTab[E].length;
			vertMatrix( U,V ).ePrev = E;
			vertMatrix( U,V ).vPrev = U;
		}
		X = U;
		U = V;
		V = X;
		if (edgeTab[E].length < vertMatrix( U,V ).distance)
		{
			vertMatrix( U,V ).distance = edgeTab[E].length;
			vertMatrix( U,V ).ePrev = E;
			vertMatrix( U,V ).vPrev = U;
		}
	}

	//run Floyd()
	//szukam min{vertMatrix[vi][vj].distance, vertMatrix[vi][vl].distance+vertMatrix[vl][vj].distance}
	typename EdgeContainer::ValType::DistType nd;
	for( typename GraphType::PVertex Vl = g.getVert(); Vl && !existNegCycle; Vl = g.getVertNext( Vl ) )
		for( typename GraphType::PVertex Vi = g.getVert(); Vi; Vi = g.getVertNext( Vi ) )
		{
			//if (vertMatrix(Vi,Vl).distance < inf) mozna tak albo tak jak ponizej
			if (inf != vertMatrix(Vi,Vl).distance)
				for( typename GraphType::PVertex Vj = g.getVert(); Vj; Vj = g.getVertNext( Vj ) )
					if (inf > vertMatrix(Vl,Vj).distance &&
					   ((nd = vertMatrix( Vi,Vl ).distance + vertMatrix( Vl,Vj ).distance) < vertMatrix(Vi,Vj).distance))
					{
						vertMatrix( Vi,Vj ).distance = nd;
						vertMatrix( Vi,Vj ).ePrev = vertMatrix( Vl,Vj ).ePrev;
						vertMatrix( Vi,Vj ).vPrev = vertMatrix( Vl,Vj ).vPrev;
					}
			//sprawdzenie czy nie ma cykli ujemnych
			if (zero > vertMatrix( Vi,Vi ).distance)
			{
				existNegCycle = true;
				break;
			}
		}
	if (existNegCycle) return false;
	return true;
}

template< class DefaultStructs > template< class GraphType, class TwoDimVertContainer, class VIter, class EIter > int
	FloydPar< DefaultStructs >::getPath( const GraphType &g, const TwoDimVertContainer &vertMatrix,
		typename GraphType::PVertex start, typename GraphType::PVertex end, PathStructs::OutPath< VIter,EIter > iters )
{
	koalaAssert( start && end,AlgExcNullVert );
	if (DefaultStructs:: template NumberTypeBounds< typename TwoDimVertContainer::ValType::DistType >
		::isPlusInfty(vertMatrix( start,end ).distance)) return -1; // wierzcholek end jest nieosiagalny
	return getOutPathFromMatrix( g,vertMatrix,iters,start,end );
}

