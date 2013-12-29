// MISStrategy::Rand

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::Rand::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int random = rand() % g.getVertNo();
	typename GraphType::PVertex v = g.getVert();
	while (random--) v = g.getVertNext( v );
	// ALG: no vertex chosen
	assert( v != NULL );
	(void)(vertTab);
	return v;
}

// MISStrategy::GMin

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::GMin::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int nMaxs = 0, randomIndex = 0, deg = 0;
	double max = std::numeric_limits< double >::min(), value = 0.0;
	typename GraphType::PVertex LOCALARRAY( vMaxs,g.getVertNo() );

	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		deg = g.deg( v );
		if ( deg == 0 ) return v;

		value = 1.0 / (double)(deg + 1);

		if (value == max)
		{
			vMaxs[nMaxs] = v;
			nMaxs++;
		}
		else if (value > max)
		{
			max = value;
			nMaxs = 0;
			vMaxs[nMaxs++] = v;
		}
	}

	// ALG: no vertex chosen
	(void)(vertTab);
	assert( nMaxs > 0 );
	randomIndex = rand() % nMaxs;
	return vMaxs[randomIndex];
}

// MISStrategy::GWMin

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::GWMin::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int nMaxs = 0, randomIndex = 0, deg = 0;
	double max = std::numeric_limits< double >::min(), value = 0.0;

	typename GraphType::PVertex LOCALARRAY( vMaxs,g.getVertNo() );

	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		deg = g.deg( v );

		if (deg == 0) return v;

		value = (double)vertTab[v->info] / (double)(deg + 1);

		if (value == max)
		{
			vMaxs[nMaxs] = v;
			nMaxs++;
		}
		else if (value > max)
		{
			max = value;
			nMaxs = 0;
			vMaxs[nMaxs++] = v;
		}
	}

	 // ALG: no vertex chosen
	assert( nMaxs > 0 );
	randomIndex = rand() % nMaxs;
	return vMaxs[randomIndex];
}

// MISStrategy::GGWMin

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::GGWMin::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int nMaxs = 0;
	unsigned int randomIndex = 0;
	unsigned int deg = 0;
	typename GraphType::PVertex LOCALARRAY( vMaxs,g.getVertNo() );

	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		deg = g.deg( v );

		if (deg == 0) return v;

		// iterate neighbors
		Set< typename GraphType::PVertex > neighbors = g.getNeighSet( v );
		double degree;
		double nSum = 0.0;
		for( typename GraphType::PVertex pVi = neighbors.first(); pVi; pVi = neighbors.next( pVi ))
		{
			degree = g.deg( pVi );
			nSum += (double)vertTab[pVi->info] / (double)(degree + 1);
		}

		if (nSum <= deg)
		{
			vMaxs[nMaxs] = v;
			nMaxs++;
		}
	}

	// ALG: no vertex chosen
	assert( nMaxs > 0 );
	randomIndex = rand() % nMaxs;
	return vMaxs[randomIndex];
}

// MISStrategy::GWMin2

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::GWMin2::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int nMaxs = 0;
	unsigned int randomIndex = 0;
	unsigned int deg = 0;
	typename GraphType::PVertex LOCALARRAY( vMaxs,g.getVertNo() );

	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		deg = g.deg( v );

		if (deg == 0) return v;

		// iterate neighbors
		Set< typename GraphType::PVertex > neighbors = g.getNeighSet( v );
		double degree;
		double nSum = 0.0;
		for( typename GraphType::PVertex pVi = neighbors.first(); pVi; pVi = neighbors.next( pVi ) )
		{
			degree = g.deg( pVi );
			nSum += (double)vertTab[pVi->info];
		}

		nSum = (double)deg / (nSum + (double)deg);

		if (nSum <= deg)
		{
			vMaxs[nMaxs] = v;
			nMaxs++;
		}
	}

	// ALG: no vertex chosen
	assert( nMaxs > 0 );
	randomIndex = rand() % nMaxs;
	return vMaxs[randomIndex];
}

// MISStrategy::GMax

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::GMax::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int nMins = 0, randomIndex = 0, deg = 0;
	double min = std::numeric_limits< double >::max(), value = 0.0;
	typename GraphType::PVertex LOCALARRAY( vMins,g.getVertNo() );

	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		deg = g.deg( v );

		if (deg == 0) continue;
		value = 1.0 / (double)(deg * (deg + 1));

		if (value == min)
		{
			vMins[nMins] = v;
			nMins++;
		}
		else if (value < min)
		{
			min = value;
			nMins = 0;
			vMins[nMins++] = v;
		}
	}

	// ALG: no vertex chosen
	assert( nMins > 0 );
	randomIndex = rand() % nMins;
	(void)vertTab;
	return vMins[randomIndex];
}

// MISStrategy::GWMax

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::GWMax::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int nMins = 0, randomIndex = 0, deg = 0;
	double min = std::numeric_limits< double >::max(), value = 0.0;
	typename GraphType::PVertex LOCALARRAY( vMins,g.getVertNo() );

	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		deg = g.deg( v );

		if (deg == 0) continue;

		value = (double)vertTab[v->info] / (double)(deg * (deg + 1));

		if (value == min) {
			vMins[nMins] = v;
			nMins++;
		}
		else if (value < min)
		{
			min = value;
			nMins = 0;
			vMins[nMins++] = v;
		}
	}

	// ALG: no vertex chosen
	assert( nMins > 0 );
	randomIndex = rand() % nMins;
	return vMins[randomIndex];
}

// MISStrategy::GGWMax

template< class GraphType, class VertContainer > typename GraphType::PVertex
	MISStrategy::GGWMax::operator()( const GraphType &g, const VertContainer& vertTab )
{
	unsigned int nMins = 0;
	unsigned int randomIndex = 0;
	unsigned int deg = 0;
	double value = 0.0;
	typename GraphType::PVertex LOCALARRAY( vMins,g.getVertNo() );

	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		deg = g.deg( v );

		if (deg == 0) continue;

		// iterate neighbors
		Set< typename GraphType::PVertex > neighbors = g.getNeighSet( v );
		double degree;
		double nSum = 0.0;

		for( typename GraphType::PVertex pVi = neighbors.first(); pVi; pVi = neighbors.next( pVi ) )
		{
			degree = g.deg( pVi );
			nSum += (double)vertTab[pVi->info] / (double)(degree * ( degree + 1 ));
		}

		value = (double)vertTab[v->info] / (double)(deg + 1);

		if (nSum >= value) vMins[nMins++] = v;
	}

	// ALG: no vertex chosen
	assert( nMins > 0 );
	randomIndex = rand() % nMins;
	return vMins[randomIndex];
}

// MISHeuristicPar

template< class DefaultStructs >
	template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
	unsigned MISHeuristicPar< DefaultStructs >::getWMin(
		const GraphType &g, OutputIterator out, ChoiceFunction choose, const VertContainer &vertTab )
{
	// USR: directed graphs and loops are not allowed
	koalaAssert( g.getEdgeNo( EdLoop ) == 0,AlgExcWrongConn );

	typedef typename DefaultStructs::template LocalGraph< typename GraphType::PVertex,EmptyEdgeInfo,Undirected >::Type
		ImageGraph;

    SimplArrPool<typename ImageGraph::Vertex> valloc(g.getVertNo());
    SimplArrPool<typename ImageGraph::Edge> ealloc(g.getEdgeNo());
	ImageGraph ig(&valloc,&ealloc);
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex, typename ImageGraph::PVertex>
        ::Type org2im( g.getVertNo() );
	for( typename GraphType::PVertex u = g.getVert(); u; u = g.getVertNext( u ) ) org2im[u]=ig.addVert( u );
	for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) )
        ig.addEdge(org2im[g.getEdgeEnd1(e)],org2im[g.getEdgeEnd2(e)]);
	// ALG: copy contains all vertices and edges
	assert( g.getEdgeNo() == ig.getEdgeNo() && g.getVertNo() == ig.getVertNo() );
	return TemplateWMIN( ig,out,choose,vertTab);
}

template< class DefaultStructs >
	template< class GraphType, class OutputIterator, class ChoiceFunction, class VertContainer >
	unsigned MISHeuristicPar< DefaultStructs >::getWMax(
		const GraphType &g, OutputIterator out, ChoiceFunction choose, const VertContainer &vertTab )
{
	koalaAssert( g.getEdgeNo( EdLoop ) == 0,AlgExcWrongConn );

	typedef typename DefaultStructs::template LocalGraph< typename GraphType::PVertex,EmptyEdgeInfo,Undirected >::Type
		ImageGraph;

    SimplArrPool<typename ImageGraph::Vertex> valloc(g.getVertNo());
    SimplArrPool<typename ImageGraph::Edge> ealloc(g.getEdgeNo());
	ImageGraph ig(&valloc,&ealloc);
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex, typename ImageGraph::PVertex>
        ::Type org2im( g.getVertNo() );
	for( typename GraphType::PVertex u = g.getVert(); u; u = g.getVertNext( u ) ) org2im[u]=ig.addVert( u );
	for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) )
        ig.addEdge(org2im[g.getEdgeEnd1(e)],org2im[g.getEdgeEnd2(e)]);
	// ALG: copy contains all vertices and edges
	assert( g.getEdgeNo() == ig.getEdgeNo() && g.getVertNo() == ig.getVertNo() );
	return TemplateWMAX( ig,out,choose,vertTab );
}

template< class DefaultStructs >
	template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
	unsigned MISHeuristicPar< DefaultStructs >::TemplateWMIN(
		GraphType &g, OutputIterator out, ChoiceFunction choose, const VertContainer &vertTab )
{
	unsigned res = 0;
	typename GraphType::PVertex v;

	// empty input graph
	if (g.getVertNo() == 0) return res;

	while (g.getVertNo())
	{
		// choose a vertex
		v = choose( g,vertTab );

		// add chosen vertex to independent set
		++res;

		*out = v->info;
		++out;

		// delete the vertex with its neighborhood
		g.delVerts( g.getClNeighSet( v ) );
	}

	return res;
}

template< class DefaultStructs >
	template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
	unsigned MISHeuristicPar< DefaultStructs >::TemplateWMAX(
		GraphType &g, OutputIterator out, ChoiceFunction choose, const VertContainer &vertTab )
{
	unsigned res = 0;
	typename GraphType::PVertex v;

	// empty input graph
	if (g.getVertNo() == 0) return res;

	while (g.getEdgeNo())
	{
		// choose vertex
		v = choose( g,vertTab );

		// delete it
		g.delVert( v );
	}

	// copy left vertices to output
	for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		++res;
		*out = v->info;
		++out;
	}

	return res;
}



template< class DefaultStructs > template< class GraphType, typename Iterator >
	bool MISHeuristicPar< DefaultStructs >::isStable( const GraphType &g, Iterator first, Iterator last )
{
	koalaAssert( g.getEdgeNo( EdLoop ) == 0,AlgExcWrongConn );

    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex, EmptyVertInfo> ::Type s( g.getVertNo() );
	for( Iterator pV = first; pV != last; pV++ ) s[*pV]=EmptyVertInfo();
	for( Iterator pV = first; pV != last; pV++ )
        for( typename GraphType::PEdge e=g.getEdge(*pV); e; e=g.getEdgeNext(*pV,e))
            if (s.hasKey(g.getEdgeEnd(e,*pV))) return false;
    return true;

}

template< class DefaultStructs > template< class GraphType, typename Iterator >
	bool MISHeuristicPar< DefaultStructs >::isMaxStable(const GraphType &g, Iterator first, Iterator last, bool stabilitytest )
{
	koalaAssert( g.getEdgeNo( EdLoop ) == 0,AlgExcWrongConn );

	if ( stabilitytest && (!isStable( g,first,last ))) return false;
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex, EmptyVertInfo> ::Type s( g.getVertNo() );
	for( Iterator it = first; it != last; it++ ) s[*it]=EmptyVertInfo();

	for( typename GraphType::PVertex pV = g.getVert(); pV; pV = g.getVertNext( pV ) )
        if (!s.hasKey(pV))
        {
            bool flag=false;
            for( typename GraphType::PEdge e=g.getEdge(pV); e; e=g.getEdgeNext(pV,e))
                if (s.hasKey(g.getEdgeEnd(e,pV)))
                {
                    flag=true;
                    break;
                }
            if (!flag) return false;
        }
    return true;
}
