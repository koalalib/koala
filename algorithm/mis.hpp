// MISStrategy::Rand

template< class GraphType, class VertContainer > typename GraphType::PVertex
    MISStrategy::Rand::operator()( const GraphType &g, const VertContainer& vertTab )
{
    unsigned int random = rand() % g.getVertNo();
    typename GraphType::PVertex v = g.getVert();
    while (random--) v = g.getVertNext( v );
    // ALG: no vertex chosen
    assert( v != NULL );
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

    typedef typename DefaultStructs::template LocalGraph< typename GraphType::PVertex,char,Undirected >::Type
        ImageGraph;
    ImageGraph ig;
    for( typename GraphType::PVertex u = g.getVert(); u; u = g.getVertNext( u ) ) ig.addVert( u );

    for( typename ImageGraph::PVertex u = ig.getVert(); u != ig.getVertLast(); u = ig.getVertNext( u ) )
    {
        for( typename ImageGraph::PVertex v = ig.getVertNext( u ); v ; v = ig.getVertNext( v ) )
            if (g.getEdge( u->info,v->info )) ig.addEdge( u,v );
    }
    ig.makeAdjMatrix();
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

    typedef typename DefaultStructs::template LocalGraph< typename GraphType::PVertex,char,Undirected >::Type
        ImageGraph;
    ImageGraph ig;
    for( typename GraphType::PVertex u = g.getVert(); u; u = g.getVertNext( u )) ig.addVert( u );

    for( typename ImageGraph::PVertex u = ig.getVert(); u != ig.getVertLast(); u = ig.getVertNext( u ) )
    {
        for( typename ImageGraph::PVertex v = ig.getVertNext( u ); v; v = ig.getVertNext( v ) )
            if (g.getEdge( u->info,v->info )) ig.addEdge( u,v );
    }
    ig.makeAdjMatrix();
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

    for( Iterator pV = first; pV != last; pV++ )
    {
        Iterator pU = last;
        for( --pU; pU != pV; pU-- )
            if (g.getEdge( *pV,*pU )) return false;
    }
    return true;
}

template< class DefaultStructs > template< class GraphType, typename Iterator >
    bool MISHeuristicPar< DefaultStructs >::isMaxStable(const GraphType &g, Iterator first, Iterator last, bool stabilitytest )
{
    koalaAssert( g.getEdgeNo( EdLoop ) == 0,AlgExcWrongConn );

    if ( stabilitytest && (!isStable( g,first,last ))) return false;
    for( typename GraphType::PVertex pV = g.getVert(); pV; pV = g.getVertNext( pV ) )
    {
        bool isVertIndependent = true;
        for( Iterator ipU = first; ipU != last; ipU++ )
        {
            if (pV == *ipU || g.getEdge( pV,*ipU ))
            {
                isVertIndependent = false;
                break;
            }
        }
        if (isVertIndependent) return false;
    }
    return true;
}
