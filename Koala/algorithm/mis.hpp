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

// MISPar

template< class DefaultStructs > template< class GraphType, typename Iterator >
    bool MISPar< DefaultStructs >::isStable( const GraphType &g, Iterator first, Iterator last )
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
    bool MISPar< DefaultStructs >::isMaxStable(const GraphType &g, Iterator first, Iterator last, bool stabilitytest )
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

template< class DefaultStructs > template< class GraphType, class OutputIterator >
    unsigned MISPar< DefaultStructs >::get( GraphType &g, OutputIterator out )
{
    typedef typename DefaultStructs::template LocalGraph< typename GraphType::PVertex,char,Undirected >::Type
        ImageGraph;
    ImageGraph ig;
    for( typename GraphType::PVertex u = g.getVert(); u; u = g.getVertNext( u ) ) ig.addVert( u );

    for( typename ImageGraph::PVertex u = ig.getVert(); u != ig.getVertLast(); u = ig.getVertNext( u ) )
    {
        for( typename ImageGraph::PVertex v = ig.getVertNext( u ); v; v = ig.getVertNext( v ) )
            if (g.getEdge( u->info,v->info )) ig.addEdge( u,v );
    }
    ig.makeAdjMatrix();
    // ALG: copy contains all vertices and edges
    assert( g.getEdgeNo() == ig.getEdgeNo() && g.getVertNo() == ig.getVertNo() );
    return __getMaximumIndependentSet( ig,out,false, isBlackHole(out) );
}

template< class DefaultStructs > template< class GraphType, class OutputIterator >
    unsigned MISPar< DefaultStructs >::__getMaximumIndependentSet( GraphType &g, OutputIterator out,
        bool isConnectedComponent, bool outblackhole )
{
    // ==============================
    // (1) empty graph
    if (g.getVertNo() == 0) return 0;

    if (g.getVertNo() == 1)
    {
        *out = (g.getVert())->info;
        ++out;
        return 1;
    }

    // ==============================
    // (2) connected components
    if (!isConnectedComponent)
    {
        // Optimization note:
        //  If we do not have certainty that it is a connected component,
        //  calculate all connected components and run the algorithm for each of them.
        unsigned vertNo = g.getVertNo() + 1;
        int LOCALARRAY( compTab,vertNo );
        typename GraphType::PVertex LOCALARRAY( tabV,vertNo );
        typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
            SearchStructs::VisitVertLabs< GraphType > >::Type vd;
        unsigned components = Koala::DFSPostorderPar< DefaultStructs >::split(g,
            Koala::SearchStructs::compStore( compTab,tabV ),EdAll,vd );

        // if there is more then one connected component in graph
        if (components > 1)
        {
            unsigned cResult = 0;

            // run the algorithm for each connected component
            for( unsigned i = 0; i < components; ++i )
            {
                // make a new graph from this connected component
                typedef typename DefaultStructs::template
                    LocalGraph< typename GraphType::VertInfoType,char,Undirected >::Type ImageGraph;
                ImageGraph gComponent;
                gComponent.makeAdjMatrix();

                typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
                    typename ImageGraph::PVertex >::Type vMapping;

                for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ))
                    if (vd[v].component == i) vMapping[v] = gComponent.addVert( v->info );
                for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) )
                {
                    std::pair< typename GraphType::PVertex,typename GraphType::PVertex > ends = g.getEdgeEnds( e );
                    typename ImageGraph::PVertex iv = vMapping[ends.first];
                    typename ImageGraph::PVertex iu = vMapping[ends.second];
                    if (iv && iu) gComponent.addEdge( iv,iu );
                }

                typename GraphType::VertInfoType LOCALARRAY( outInner,g.getVertNo() );
                unsigned MISInner = __getMaximumIndependentSet( gComponent,outInner,true, outblackhole );
                for( unsigned i = 0; i < MISInner; i++ )
                {
                    *out = outInner[i];
                    ++out;
                }
                cResult += MISInner;
            }
            return cResult;
        }
    }

    // ==============================
    // (3) domination
    // note that domination could be related
    // only if both of them are neighbors
    for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) )
    {
        if (g.getClNeighSet( g.getEdgeEnd1( e ) ).subsetOf( g.getClNeighSet( g.getEdgeEnd2( e ) ) ))
        {
            g.delVert( g.getEdgeEnd2( e ) );
            typename GraphType::VertInfoType LOCALARRAY( outInner,g.getVertNo() );
            unsigned MISInner = __getMaximumIndependentSet( g,outInner,false, outblackhole );
            for( unsigned i = 0; i < MISInner; i++ )
            {
                *out = outInner[i];
                ++out;
            }
            return MISInner;
        }
        if (g.getClNeighSet( g.getEdgeEnd2( e ) ).subsetOf( g.getClNeighSet( g.getEdgeEnd1( e ) ) ))
        {
            g.delVert( g.getEdgeEnd1( e ) );
            typename GraphType::VertInfoType LOCALARRAY( outInner,g.getVertNo() );
            unsigned MISInner = __getMaximumIndependentSet( g,outInner,false, outblackhole );
            for( unsigned i = 0; i < MISInner; i++ )
            {
                *out = outInner[i];
                ++out;
            }
            return MISInner;
        }
    }

    // ==============================
    // (4) folding
    unsigned int minFoldableDeg = DefaultStructs:: template NumberTypeBounds<unsigned int>::plusInfty();
    unsigned int degree = 0;
    typename GraphType::PVertex minFoldableVert = NULL;

    for(typename GraphType::PVertex pV = g.getVert(); pV; pV = g.getVertNext(pV)) {
        degree = g.deg(pV);

        if( degree < minFoldableDeg ) {
            // Note #1: All verties of degree at most two are foldable.
            // Note #2: Folding veritces of degree > 1 implies adding a new vertex to the graph.
            //          Such vertex might be later chosen to the final solution, what is correct
            //          from the result point of view. Unfortunately such solution might not
            //          correspond to the original graph vertices.
            //          So if we need both, size of the maximum independent set and the subset
            //          of verices belonging to it, we have to assure that there will be no new
            //          vertices added to the graph. For this approach we might fold vertices
            //          only if their degree is <= 1.
            if( ( !outblackhole && degree <= 1 ) ||
                (  outblackhole && (degree <= 2 || isFoldable(g, pV)) ) ) {
                minFoldableVert = pV;
                minFoldableDeg = degree;
            }

        }
    }

    // if there is a foldable vertex
    if (minFoldableVert != NULL)
    {
        Set< typename GraphType::PVertex > minFoldableNeigh = g.getClNeighSet( minFoldableVert );

        // fold(minFoldableVert)
        // steps 1-3 are obligatory only for d >= 2
        // otherwise we only delete N[v]
        if (minFoldableDeg >= 2)
        {
            // 1. add vertex Uij for each anti-edge in N(v)
            Set< typename GraphType::PVertex > neighbors = g.getNeighSet( minFoldableVert );
            Set< typename GraphType::PVertex > uijs;
            typename GraphType::PVertex Uij;
            for( typename GraphType::PVertex pVi = neighbors.first(); pVi; pVi = neighbors.next( pVi ) )
            {
                for( typename GraphType::PVertex pVj = neighbors.next( pVi ); pVj; pVj = neighbors.next( pVj ) )
                {
                    // if anti-edge
                    if (!g.getEdge( pVi,pVj ))
                    {
                        Uij = g.addVert();
                        uijs.add( Uij );

                        // 2. add an edge between each Uij and N(Ui); Uij and N(Uj)
                        Set< typename GraphType::PVertex > neighborsUiUj = g.getNeighSet( pVi ) + g.getNeighSet( pVj );
                        for( typename GraphType::PVertex pNUiUj = neighborsUiUj.first(); pNUiUj;
                            pNUiUj = neighborsUiUj.next( pNUiUj )) g.addEdge( pNUiUj,Uij );
                    }
                }
            }

            // 3. add an edge between each new vertex
            for( typename GraphType::PVertex pVi = uijs.first() ; pVi; pVi = uijs.next( pVi ))
                for(typename GraphType::PVertex pVj = uijs.next( pVi ) ; pVj; pVj = uijs.next( pVj ))
                    g.addEdge( pVi,pVj );
        }

        // 4. delete N[v]
        *out = minFoldableVert->info;
        ++out;
        g.delVerts( minFoldableNeigh );
        typename GraphType::VertInfoType LOCALARRAY( outInner,g.getVertNo() );
        unsigned MISInner = __getMaximumIndependentSet( g,outInner,false, outblackhole);
        for( unsigned i = 0; i < MISInner; i++)
        {
            *out = outInner[i];
            ++out;
        }
            return MISInner + 1;
    }

    // ==============================
    // (5) branching
    unsigned a = 0;
    unsigned b = 0;

    typedef typename DefaultStructs::template LocalGraph< typename GraphType::VertInfoType,char,Undirected >::Type
        ImageGraph;
    ImageGraph graph1;

    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,typename ImageGraph::PVertex >::Type
        vMappingA;

    for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v )) vMappingA[v] = graph1.addVert( v->info );

    for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) )
    {
        std::pair< typename GraphType::PVertex,typename GraphType::PVertex > ends = g.getEdgeEnds( e );
        typename ImageGraph::PVertex ivA,iuA;
        ivA = vMappingA[ends.first];
        iuA = vMappingA[ends.second];
        if (ivA && iuA) graph1.addEdge( ivA,iuA );
    }

    graph1.makeAdjMatrix();

    typename GraphType::VertInfoType LOCALARRAY( aOut,graph1.getVertNo() );
    typename GraphType::VertInfoType LOCALARRAY( bOut,g.getVertNo() );

    *aOut = graph1.getVert()->info;
    ++aOut;
    graph1.delVerts( graph1.getClNeighSet( graph1.getVert() ) );
    a = __getMaximumIndependentSet( graph1,aOut,false, outblackhole ) + 1;

    g.delVerts( getMirrors( g,g.getVert() ) );
    b = __getMaximumIndependentSet( g,bOut,false, outblackhole );

    if (a > b)
        for( unsigned int i = 0; i < a - 1; i++)
        {
            *out = aOut[i];
            ++out;
        }
    else
        for( unsigned int i = 0; i < b; i++ )
        {
            *out = bOut[i];
            ++out;
        }

    return (a > b) ? a : b;
}

template< class DefaultStructs > template< class GraphType > bool MISPar< DefaultStructs >::isDominated(
    const GraphType &g, typename GraphType::PVertex v, typename GraphType::PVertex w )
{
    Set< typename GraphType::PVertex > neighborsV = g.getNeighSet( v );
    Set< typename GraphType::PVertex > neighborsW = g.getNeighSet( w );

    // compare both neighborhoods
    if (neighborsV.size() >= neighborsW.size())
    {
        for( typename GraphType::PVertex pV = neighborsV.first(); pV; pV = neighborsV.next( pV ))
            if (!neighborsW.isElement( pV ) ) return false;
    }
    return false;
}

template< class DefaultStructs > template< class GraphType > Koala::Set< typename GraphType::PVertex >
    MISPar< DefaultStructs >::getMirrors( const GraphType &g, typename GraphType::PVertex v )
{
    Set< typename GraphType::PVertex > mirrors;
    Set< typename GraphType::PVertex > N2 = getSecondNeighSet( g,v );
    Set< typename GraphType::PVertex > Nv = g.getNeighSet( v );

    // only second neighborhood vertices could be mirrors
    for( typename GraphType::PVertex pU = N2.first(); pU; pU = N2.next( pU ) )
    {
        Set< typename GraphType::PVertex > Nu = g.getNeighSet( pU );
        Set< typename GraphType::PVertex > NvMinusNu = Nv - Nu;

        if (isClique( g,NvMinusNu )) mirrors.add( pU );
    }

    // add the vertex it self
    mirrors.add( v );

    return mirrors;
}

template< class DefaultStructs > template< class GraphType > bool MISPar< DefaultStructs >::isFoldable(
    const GraphType &g, typename GraphType::PVertex v )
{
    Set< typename GraphType::PVertex > neighbors = g.getNeighSet( v );
    unsigned int gSize = neighbors.size();

    // Note that only d <= 4 verties are potentially foldable
    // there are no possible anti-triangles
    if (gSize < 3) return true;

    if (gSize <= 4)
    {
        unsigned int antiedges = 0;

        // set max possible antiedges for now
        switch (gSize)
        {
            case 3:
                antiedges = 3;
                break;
            case 4:
                antiedges = 6;
                break;
            default: // ALG: only 3 or 4 vertices expected here
                assert(0);
        }

        // count the anti-edges in neighborhood
        for( typename GraphType::PVertex pVi = neighbors.first(); pVi; pVi = neighbors.next( pVi ))
            for(typename GraphType::PVertex pVj = neighbors.next(pVi); pVj; pVj = neighbors.next(pVj))
                if (g.getEdge( pVi,pVj )) --antiedges;

        // vertex is foldable if there are less than four of them
        if (antiedges == 3 && gSize == 3) return false;

        if (antiedges < 4) return true;
    }

    return false;
}

template< class DefaultStructs > template< class GraphType > bool MISPar< DefaultStructs >::isClique(
    const GraphType &g, const Set< typename GraphType::PVertex > &verts )
{
    for( typename GraphType::PVertex pV = verts.first(); pV; pV = verts.next( pV ) )
        for( typename GraphType::PVertex pU = verts.first(); pU; pU = verts.next( pU ) )
            if (g.getEdge( pV,pU ) == NULL) return false;
    return true;
}

template< class DefaultStructs > template< class GraphType > Set< typename GraphType::PVertex >
    MISPar< DefaultStructs >::getSecondNeighSet( const GraphType &g, typename GraphType::PVertex vert )
{
    Set< typename GraphType::PVertex > n1 = g.getNeighSet( vert );
    Set< typename GraphType::PVertex > neigh;
    Set< typename GraphType::PVertex > n2;

    for( typename GraphType::PVertex pV = n1.first(); pV; pV = n1.next( pV ) )
    {
        neigh = g.getNeighSet( pV );
        for( typename GraphType::PVertex pW = neigh.first(); pW; pW = neigh.next( pW ) )
            if (pW != vert) n2.add( pW );
    }
    return n2;
}
