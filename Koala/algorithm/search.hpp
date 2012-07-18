template< class GraphType >
SearchStructs::VisitVertLabs< GraphType >::VisitVertLabs(
    typename GraphType::PVertex vp, typename GraphType::PEdge ep,
    int dist, int comp ):
    vPrev( vp ),
    ePrev( ep ),
    distance( dist ),
    component( comp )
    { }

//template< class GraphType > template< class Rec >
//void SearchStructs::VisitVertLabs< GraphType >::get( Rec &r )
//{
//    r.vPrev = vPrev;
//    r.ePrev = ePrev;
//    r.distance = distance;
//    r.component = component;
//}

template< class CIter, class VIter >
SearchStructs::CompStore< CIter,VIter >
SearchStructs::compStore( CIter ac, VIter av )
{
    return CompStore< CIter,VIter >( ac,av );
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPre(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, simple_preorder_visitor_tag &s )
{
    return v( g,u,r );
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPre(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, simple_postorder_visitor_tag &s )
{
    return true;
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPre(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, complex_visitor_tag &c )
{
    return v.visitVertexPre( g,u,r );
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPre(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, ... )
{
    return v.operator()( g,u,r );
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPost(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, simple_preorder_visitor_tag &s )
{
    return true;
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPost(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, simple_postorder_visitor_tag &s )
{
    return v( g,u,r );
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPost(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, complex_visitor_tag &c )
{
    return v.visitVertexPost( g,u,r);
}

template< class GraphType, class Visitor >
bool Visitors::visitVertexPost(
    const GraphType &g, Visitor &v, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r, ... )
{
    return v.operator()( g,u,r );
}

template< class GraphType, class Visitor >
bool Visitors::visitEdgePre(
    const GraphType &g, Visitor &v, typename GraphType::PEdge e,
    typename GraphType::PVertex u, complex_visitor_tag &c )
{
    return v.visitEdgePre( g,e,u );
}

template< class GraphType, class Visitor >
bool Visitors::visitEdgePre(
    const GraphType &g, Visitor &v, typename GraphType::PEdge e,
    typename GraphType::PVertex u, simple_visitor_tag &s )
{
    return true;
}

template< class GraphType, class Visitor >
bool Visitors::visitEdgePre(
    const GraphType &g, Visitor &v, typename GraphType::PEdge e,
    typename GraphType::PVertex u, ... )
{
    return true;
}

template< class GraphType, class Visitor >
bool Visitors::visitEdgePost(
    const GraphType &g, Visitor &v, typename GraphType::PEdge e,
    typename GraphType::PVertex u, complex_visitor_tag &c )
{
    return v.visitEdgePost( g,e,u );
}

template< class GraphType, class Visitor >
bool Visitors::visitEdgePost(
    const GraphType &g, Visitor &v, typename GraphType::PEdge e,
    typename GraphType::PVertex u, simple_visitor_tag &s )
{
    return true;
}

template< class GraphType, class Visitor >
bool Visitors::visitEdgePost(
    const GraphType &g, Visitor &v, typename GraphType::PEdge e,
    typename GraphType::PVertex u, ... )
{
    return true;
}

template< class GraphType, class Visitor >
bool Visitors::beginComponent(
    const GraphType &g, Visitor &v, unsigned comp, component_visitor_tag &c )
{
    return v.beginComponent( g,comp );
}

template< class GraphType, class Visitor >
bool Visitors::beginComponent(
    const GraphType &g, Visitor &v, unsigned comp, no_component_visitor_tag &c )
{
    return true;
}

template< class GraphType, class Visitor >
bool Visitors::endComponent(
    const GraphType &g, Visitor &v, unsigned comp, component_visitor_tag &c )
{
    return v.endComponent( g,comp );
}

template< class GraphType, class Visitor >
bool Visitors::endComponent(
    const GraphType &g, Visitor &v, unsigned comp, no_component_visitor_tag &c )
{
    return true;
}

template< class GraphType >
bool Visitors::EndVertVisitor::visitEdgePost(
    const GraphType &g, typename GraphType::PEdge e, typename GraphType::PVertex u )
{
    return !e || (void*)g.getEdgeEnd( e,u ) != ptr;
}

template< class VertIter > template< class GraphType >
bool Visitors::StoreTargetToVertIter< VertIter >::operator()(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r )
{
    *m_iter = u;
    ++m_iter;
    return true;
}

template< class CompIter, class VertIter >
Visitors::StoreCompVisitor< CompIter,VertIter >::_State::_State(
    CompStore< CompIter,VertIter > i ):
    iters( i ),
    p( 0 ),
    id( 0 )
{
    *(iters.compIter) = 0;
    ++(iters.compIter);
}

template< class CompIter, class VertIter > template< class GraphType >
bool Visitors::StoreCompVisitor< CompIter,VertIter >::operator()(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r )
{
    *(m_st.iters.vertIter) = u;
    ++(m_st.iters.vertIter);
    ++(m_st.p);
    return true;
}

template< class CompIter, class VertIter > template< class GraphType >
bool Visitors::StoreCompVisitor< CompIter,VertIter >::beginComponent(
    const GraphType &g, unsigned u )
{
    return true;
}

template< class CompIter, class VertIter > template< class GraphType >
bool Visitors::StoreCompVisitor< CompIter,VertIter >::endComponent(
    const GraphType &g, unsigned u )
{
    *(m_st.iters.compIter) = m_st.p;
    ++(m_st.iters.compIter);
    return true;
}

template< class Visitor > template< class GraphType >
bool Visitors::ComplexPreorderVisitor< Visitor >::visitVertexPre(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &data )
{
    return visit.operator()( g,u,data );
}

template< class Visitor > template< class GraphType >
bool Visitors::ComplexPreorderVisitor< Visitor >::visitVertexPost(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &data )
{
    return true;
}

template< class Visitor > template< class GraphType >
bool Visitors::ComplexPreorderVisitor< Visitor >::visitEdgePre(
    const GraphType &g, typename GraphType::PEdge e,
    typename GraphType::PVertex u )
{
    return true;
}

template< class Visitor > template< class GraphType >
bool Visitors::ComplexPreorderVisitor< Visitor >::visitEdgePost(
    const GraphType &g, typename GraphType::PEdge e,
    typename GraphType::PVertex u )
{
    return true;
}

template< class Visitor > template< class GraphType >
bool Visitors::ComplexPostorderVisitor< Visitor >::visitVertexPost(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &data )
{
    return visit.operator()( g,u,data );
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertContainer,
    class Visitor >
int GraphSearchBase< SearchImpl,  DefaultStructs >::visitAllBase(
    const GraphType &g, VertContainer &visited, Visitor visit, EdgeDirection mask )
{
    int rv;
    unsigned component;
    typename GraphType::PVertex first;

    component = 0;
    first = g.getVert();
    while (first != NULL)
    {
        Visitors::beginComponent( g,visit,component,visit );
        rv = SearchImpl::visitBase( g,first,visited,visit,mask,component );
        Visitors::endComponent( g,visit,component,visit );
        component++;
        if (rv < 0) break;
        do
            first = g.getVertNext( first );
        while (first != NULL && visited.hasKey( first ));
     }
     return component;
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertContainer,
    class Iter >
int GraphSearchBase< SearchImpl, DefaultStructs >::scanAttainable(
    const GraphType &g, typename GraphType::PVertex root, Iter comp,
    EdgeDirection mask, VertContainer &cont )
{
    int rv;
    assert( root );
    mask &= ~EdLoop;
    rv = SearchImpl::visitBase( g,root,cont,
        Visitors::StoreTargetToVertIter< Iter >( comp ),mask,0 );
    return (rv < 0) ? -rv : rv;
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertIter >
int GraphSearchBase< SearchImpl, DefaultStructs >::scanAttainable(
    const GraphType &g, typename GraphType::PVertex root, VertIter comp,
    EdgeDirection mask )
{
    VisitedMap< GraphType > cont(g.getVertNo());
    return scanAttainable( g,root,comp,mask,cont );
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertContainer,
    class VertIter >
int GraphSearchBase< SearchImpl, DefaultStructs >::scan(
    const GraphType &g, VertIter iter, EdgeDirection mask, VertContainer &tree )
{
    mask |= (mask & (EdDirIn | EdDirOut)) ? EdDirIn | EdDirOut : 0;
    mask &= ~EdLoop;
    return visitAllBase( g,tree,
        Visitors::StoreTargetToVertIter< VertIter >( iter ),mask );
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertIter >
int GraphSearchBase< SearchImpl, DefaultStructs >::scan(
    const GraphType &g, VertIter iter, EdgeDirection mask )
{
    VisitedMap< GraphType > cont(g.getVertNo());
    return scan( g,iter,mask,cont );
}

template< class SearchImpl, class DefaultStructs > template< class GraphType>
int GraphSearchBase< SearchImpl, DefaultStructs >::cyclNo(
    const GraphType &g, EdgeDirection mask )
{
    return g.getEdgeNo(mask) - g.getVertNo() + scan( g,blackHole,mask );
}

template< class SearchImpl, class DefaultStructs > template< class GraphType >
Set< typename GraphType::PVertex > GraphSearchBase< SearchImpl, DefaultStructs >::getAttainableSet(
    const GraphType &g, typename GraphType::PVertex root, EdgeDirection mask )
{
    assert( root );
    Set< typename GraphType::PVertex > res;
    scanAttainable( g,root,setInserter( res ),mask );
    return res;
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertIter,
    class EdgeIter >
int GraphSearchBase< SearchImpl, DefaultStructs >::getPath(
    const GraphType &g, typename GraphType::PVertex start,
    typename GraphType::PVertex end, OutPath< VertIter,EdgeIter > path,
    EdgeDirection mask )
{
    assert( start && end );
    mask &= ~EdLoop;
    VisitedMap< GraphType > tree(g.getVertNo());
    SearchImpl::visitBase( g,start,tree,Visitors::EndVertVisitor( end ),mask,0 );
    int res = tree[end].distance;
    if (std::numeric_limits<int>::max()== res ) return -1;
    if (!isBlackHole( path.vertIter ) || !isBlackHole( path.edgeIter ))
        getOutPath( g,tree,path,end );
    return res;
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertContainer,
    class CompIter, class VertIter >
int GraphSearchBase< SearchImpl, DefaultStructs >::getComponents(
    const GraphType &g, CompStore< CompIter,VertIter > iters, EdgeDirection mask,
    VertContainer &cont )
{
    mask |= (mask & (EdDirIn | EdDirOut)) ? EdDirIn | EdDirOut : 0;
    mask &= ~EdLoop;
    typename Visitors::StoreCompVisitor< CompIter,VertIter >::State st( iters );
    return visitAllBase( g,cont,
        Visitors::StoreCompVisitor< CompIter,VertIter >( st ),mask );
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class CompIter,
    class VertIter >
int GraphSearchBase< SearchImpl, DefaultStructs >::getComponents(
    const GraphType &g, CompStore< CompIter,VertIter > iters, EdgeDirection mask )
{
    VisitedMap< GraphType > cont(g.getVertNo());
    return getComponents( g,iters,mask,cont );
}

template< class GraphType, class VertContainer, class Visitor >
DFSParamBlock< GraphType,VertContainer,Visitor >::DFSParamBlock(
    const GraphType &_g, VertContainer &_v, Visitor &_vis, EdgeDirection _m,
    int _c):
    g( _g ),
    visited( _v ),
    visit( _vis ),
    mask( _m ),
    component( _c )
{
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertContainer,
    class Visitor >
int DFSBase< SearchImpl, DefaultStructs >::dfsDoVisit(
    DFSParamBlock< GraphType,VertContainer,Visitor > &params,
    typename GraphType::PVertex u, unsigned depth )
{
    int t, retVal = 0;
    typename GraphType::PEdge e;
    typename GraphType::PVertex v;

    if (!Visitors::visitVertexPre( params.g,params.visit,u,params.visited[u],
        params.visit )) return 1;
    retVal++;

    e = params.g.getEdge( u,params.mask );
    for( ; e != NULL; e = params.g.getEdgeNext( u,e,params.mask ) )
    {
        if (!Visitors::visitEdgePre( params.g,params.visit,e,u,params.visit ))
            continue;

        v = params.g.getEdgeEnd( e,u );
        if(params.visited.hasKey( v )) continue;

        params.visited[v] =
            SearchStructs::VisitVertLabs< GraphType >( u,e,depth + 1,
                params.component );
        t = dfsDoVisit< GraphType,VertContainer,Visitor >( params,v,depth + 1 );
        if (t < 0)
        {
            retVal += -t;
            return -retVal;
        }
        retVal += t;
        if (!Visitors::visitEdgePost( params.g,params.visit,e,u,params.visit ))
            return -retVal;
    };

    if(!Visitors::visitVertexPost( params.g,params.visit,u,params.visited[u],
        params.visit )) return -retVal;
    return retVal;
}

template< class SearchImpl, class DefaultStructs > template< class GraphType, class VertContainer,
    class Visitor >
int DFSBase< SearchImpl, DefaultStructs >::dfsVisitBase(
    const GraphType &g, typename GraphType::PVertex first, VertContainer &visited,
    Visitor visit, EdgeDirection mask, int component )
{
    DFSParamBlock< GraphType,VertContainer,Visitor >
        params( g,visited,visit,mask,component );
    if (g.getVertNo() == 0) return 0;
    if (first == NULL) first = g.getVert();
    visited[first] = SearchStructs::VisitVertLabs< GraphType >( NULL,NULL,0,component );
    return DFSBase< SearchImpl,DefaultStructs >:: template dfsDoVisit< GraphType,VertContainer,Visitor>(
        params,first,0 );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int DFSPar<DefaultStructs >::visitBase(
    const GraphType &g, typename GraphType::PVertex start, VertContainer &cont,
    Visitor visit, EdgeDirection mask, int component )
{
    return DFSBase< DFSPar<DefaultStructs >, DefaultStructs >:: template dfsVisitBase< GraphType,VertContainer,Visitor >( g,start,cont,visit,
        mask,component );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int DFSPreorderPar <DefaultStructs >::dfsPreVisitBase(
    const GraphType &g, typename GraphType::PVertex start,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component,
    Visitors::complex_visitor_tag & )
{
    return DFSBase< DFSPreorderPar<DefaultStructs>, DefaultStructs >:: template dfsVisitBase< GraphType,VertContainer,Visitor >( g,start,visited,
        visit,mask,component );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int DFSPreorderPar<DefaultStructs >::dfsPreVisitBase(
    const GraphType &g, typename GraphType::PVertex start,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component,
    Visitors::simple_visitor_tag & )
{
    return DFSBase< DFSPreorderPar<DefaultStructs>, DefaultStructs >:: template dfsVisitBase< GraphType,VertContainer,
        Visitors::ComplexPreorderVisitor< Visitor > >( g,start,visited,
            Visitors::ComplexPreorderVisitor< Visitor >( visit ),mask,component );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int DFSPreorderPar <DefaultStructs >::visitBase(
    const GraphType &g, typename GraphType::PVertex start,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component )
{
    return dfsPreVisitBase< GraphType,VertContainer,Visitor >( g,start,visited,
        visit,mask,component,visit );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int DFSPostorderPar <DefaultStructs >::dfsPostVisitBase(
    const GraphType &g, typename GraphType::PVertex start,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component,
    Visitors::complex_visitor_tag & )
{
    return DFSBase< DFSPostorderPar<DefaultStructs >, DefaultStructs >:: template
        dfsVisitBase< GraphType,VertContainer,Visitor >( g,start,visited,
        visit,mask,component );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int DFSPostorderPar <DefaultStructs >::dfsPostVisitBase(
    const GraphType &g, typename GraphType::PVertex start,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component,
    Visitors::simple_visitor_tag & )
{
    return DFSBase< DFSPostorderPar<DefaultStructs >, DefaultStructs >:: template dfsVisitBase< GraphType,VertContainer,
        Visitors::ComplexPostorderVisitor< Visitor > >( g,start,visited,
            Visitors::ComplexPostorderVisitor< Visitor >( visit ),mask,component );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int DFSPostorderPar <DefaultStructs >::visitBase(
    const GraphType &g, typename GraphType::PVertex start,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component )
{
    return dfsPostVisitBase< GraphType,VertContainer,Visitor >( g,start,
        visited,visit,mask,component,visit );
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int BFSPar <DefaultStructs >::bfsDoVisit(
    const GraphType &g, typename GraphType::PVertex first,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component )
{
    unsigned depth, n, retVal;
    typename GraphType::PEdge e;
    typename GraphType::PVertex u, v;
    typename GraphType::PVertex LOCALARRAY( buf,g.getEdgeNo() + 3 );    //TODO: size?
    QueueInterface< typename GraphType::PVertex* > cont( buf,g.getEdgeNo() + 3 );   //TODO: size?

    n = g.getVertNo();
    if (n == 0) return 0;
    if (first == NULL) first = g.getVert();

    visited[first] = SearchStructs::VisitVertLabs<GraphType>( NULL,NULL,0,component );
    cont.push(first);
    retVal = 0;

    while (!cont.empty())
    {
        u = cont.top();
        depth = visited[u].distance;
        cont.pop();

        if (!Visitors::visitVertexPre( g,visit,u,visited[u],visit ))
        {
            retVal++;
            continue;
        }

        for( e = g.getEdge( u,mask ); e != NULL; e = g.getEdgeNext( u,e,mask ))
        {
            v = g.getEdgeEnd( e,u );
            if (!Visitors::visitEdgePre( g,visit,e,u,visit )) continue;
            if (visited.hasKey( v )) continue;
            visited[v] = SearchStructs::VisitVertLabs< GraphType >( u,e,
                depth + 1,component );
            cont.push( v );
            if (!Visitors::visitEdgePost( g,visit,e,u,visit )) return -retVal;
        }
        retVal++;
        if (!Visitors::visitVertexPost( g,visit,u,visited[u],visit ))
            return -retVal;
    }
    return retVal;
}

template <class DefaultStructs >
template< class GraphType, class VertContainer, class Visitor >
int BFSPar <DefaultStructs >::visitBase(
    const GraphType &g, typename GraphType::PVertex start,
    VertContainer &visited, Visitor visit, EdgeDirection mask, int component )
{
    return bfsDoVisit< GraphType,VertContainer,Visitor >( g,start,visited,
        visit,mask,component );
}


template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
SCCPar < DefaultStructs >::SCCState< GraphType,CompIter,VertIter,CompMap >::SCCState(
    CompStore< CompIter,VertIter > _i, CompMap &cm,
    typename GraphType::PVertex *buf1, typename GraphType::PVertex *buf2,
    int n ):
    vmap(n),
    s( buf1,n ),
    p( buf2,n ),
    iters( _i ),
    idx( 0 ),
    count( 0 ),
    c( 0 ),
    compMap( cm )
{
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
void SCCPar <DefaultStructs >::SCCState< GraphType,CompIter,VertIter,CompMap >::addVert(
    typename GraphType::PVertex v )
{
    *(iters.vertIter) = v;
    ++(iters.vertIter);
    if (!isBlackHole( compMap )) compMap[v] = count;
    vmap[v].assigned = true;
    idx++;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
void SCCPar <DefaultStructs >::SCCState< GraphType,CompIter,VertIter,CompMap >::newComp()
{
    *(iters.compIter) = idx;
    ++(iters.compIter);
    ++count;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
SCCPar <DefaultStructs >::SCCVisitor< GraphType,CompIter,VertIter,CompMap >::SCCVisitor(
    SCCState< GraphType,CompIter,VertIter,CompMap > &s ):
    state( s )
{
    state.c = 0;
    state.idx = 0;
    state.newComp();
    state.count = 0;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
bool SCCPar <DefaultStructs >::SCCVisitor< GraphType,CompIter,VertIter,CompMap >::visitVertexPre(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r )
{
    state.vmap[u] = SCCData( state.c,false );
    state.c++;
    state.p.push( u );
    state.s.push( u );
    return true;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
bool SCCPar <DefaultStructs >::SCCVisitor< GraphType,CompIter,VertIter,CompMap >::visitVertexPost(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &r )
{
    if (state.p.empty() || state.p.top() != u) return true;
    while (!state.s.empty() && state.s.top() != u)
    {
        state.addVert( state.s.top() );
        state.s.pop();
    }
    state.addVert( state.s.top() );
    state.newComp();
    state.s.pop();
    state.p.pop();
    return true;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
bool SCCPar <DefaultStructs >::SCCVisitor< GraphType,CompIter,VertIter,CompMap >::visitEdgePre(
    const GraphType &g, typename GraphType::PEdge e,
    typename GraphType::PVertex u )
{
    typename GraphType::PVertex v;
    v = g.getEdgeEnd( e,u );
    if (!state.vmap.hasKey( v )) return true;
    if (!state.vmap[v].assigned)
        while (!state.p.empty() &&
            state.vmap[state.p.top()].preIdx > state.vmap[v].preIdx)
            state.p.pop();
    return false;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
bool SCCPar <DefaultStructs >::SCCVisitor< GraphType,CompIter,VertIter,CompMap >::visitEdgePost(
    const GraphType &g, typename GraphType::PEdge e,
    typename GraphType::PVertex u )
{
    return true;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class CompMap >
int SCCPar <DefaultStructs >::get(
    const GraphType &g, CompStore< CompIter,VertIter > out, CompMap &compMap )
{
    int rv;
    typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, VisitVertLabs< GraphType > >::Type vertCont(g.getVertNo());
    typename GraphType::PVertex LOCALARRAY( buf1,g.getVertNo() + 1 );   //TODO: size?
    typename GraphType::PVertex LOCALARRAY( buf2,g.getVertNo() + 1 );   //TODO: size?
    SCCState< GraphType,CompIter,VertIter,CompMap > state( out,compMap,buf1,
        buf2,g.getVertNo() );
    SCCVisitor< GraphType,CompIter,VertIter,CompMap > visit( state );
    rv = DFSPar<DefaultStructs>::visitAllBase( g,vertCont,visit,EdDirOut | EdUndir );
    if (rv < 0) return rv;
    return state.count;
}

template <class DefaultStructs >
template< class GraphType, class CompMap, class PairIter >
int SCCPar <DefaultStructs >::connections(const GraphType &g, CompMap & comp, PairIter iter)
{
    int n=0;
    std::pair<int,int> LOCALARRAY(buf,g.getEdgeNo(EdDirIn|EdDirOut));
    for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut))
    {
        std::pair <typename GraphType::PVertex,typename GraphType::PVertex> ends=g.getEdgeEnds(e);
        if (comp[ends.first]!=comp[ends.second])  buf[n++]=std::make_pair(comp[ends.first],comp[ends.second]);
    }
    DefaultStructs::sort( buf,buf + n );
    n = std::unique( buf,buf + n ) - buf;
    for(int i=0;i<n;i++)
    {
        *iter=buf[i];
        ++iter;
    }
    return n;
}

template <class DefaultStructs >
template< class GraphType, class VertIter >
void DAGAlgsPar <DefaultStructs >::topOrd( const GraphType &g, VertIter out )
{
    typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, VisitVertLabs< GraphType > >::Type visited(g.getVertNo());
    DFSPostorderPar<DefaultStructs>:: template visitAllBase( g,visited,
        Visitors::StoreTargetToVertIter< typename GraphType::PVertex* >( out ),
            EdDirIn );
}

template <class DefaultStructs >
template<class GraphType, class Iter>
bool DAGAlgsPar <DefaultStructs >::isDAG( const GraphType &g, Iter beg, Iter end )
{
    if (g.getEdgeNo( EdUndir|EdLoop )) return false;
    if (!g.getVertNo()) return true;
    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,int >::Type
        topord(g.getVertNo());
    int licz = 0;
    for( Iter i = beg; i != end; ++i ) topord[*i] = licz++;
    assert( topord.size() == g.getVertNo() );
    for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) )
        if (topord[g.getEdgeEnd1(e)] > topord[g.getEdgeEnd2(e)]) return false;
    return true;
}

template <class DefaultStructs >
template< class GraphType >
bool DAGAlgsPar <DefaultStructs >::isDAG( const GraphType &g )
{
    typename GraphType::PVertex LOCALARRAY( buf,g.getVertNo() );
    topOrd( g,buf );
    return isDAG( g,buf,buf + g.getVertNo() );
}

template <class DefaultStructs >
template< class GraphType, class Iter >
int DAGAlgsPar <DefaultStructs >::transEdges(const GraphType & g, Iter out)
{
    int res=0;
    for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut))
    {
        std::pair<typename GraphType::PVertex,typename GraphType::PVertex> ends=g.getEdgeEnds(e);
        if (BFS::getPath(
                makeSubgraph(g,std::make_pair(stdChoose(true),
                                    !stdValChoose(e))),
                            ends.first,ends.second,PathStructs::outPath(blackHole,blackHole),EdDirOut)!=-1)
        {
            *out=e; ++out; ++res;
        }
    }

    return res;
}

template <class DefaultStructs >
template< class GraphType>
void DAGAlgsPar <DefaultStructs >::makeHasse(GraphType & g)
{
    typename GraphType::PEdge LOCALARRAY(buf,g.getEdgeNo());
    int res=transEdges(g,buf);
    for(int i=0;i<res;i++) g.delEdge(buf[i]);
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
BlocksPar <DefaultStructs >::BiConState< GraphType,CompIter,VertIter,EdgeMap >::BiConState(
    CompStore< CompIter,VertIter > _i, EdgeMap &em, EdgeDirection _m,
        std::pair< typename GraphType::PEdge *,int > st, VertBlockList *_vbl,int vno ):
    vmap(vno),
    emap(st.second-1),
    estk( st.first,st.second ),
    iters( _i ),
    idx( 0 ),
    count( 0 ),
    mask( _m ),
    vbl( _vbl ),
    vblAlloc( 0 ),
    outEMap( em )
{
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
void BlocksPar <DefaultStructs >::BiConState< GraphType,CompIter,VertIter,EdgeMap >::addVert(
    typename GraphType::PVertex v )
{
    *(iters.vertIter) = v;
    ++(iters.vertIter);
    vmap[v].count++;
    vbl[vblAlloc].block = count;
    vbl[vblAlloc].next = vmap[v].vblFirst;
    vmap[v].vblFirst = vblAlloc;
    vblAlloc++;
    idx++;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
void BlocksPar <DefaultStructs >::BiConState< GraphType,CompIter,VertIter,EdgeMap >::addEdge(
    typename GraphType::PEdge e )
{
    if (!isBlackHole( outEMap )) outEMap[e] = count;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
void BlocksPar <DefaultStructs >::BiConState< GraphType,CompIter,VertIter,EdgeMap >::newComp() {
    *(iters.compIter) = idx;
    ++(iters.compIter);
    ++count;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
BlocksPar <DefaultStructs >::BiConVisitor< GraphType,CompIter,VertIter,EdgeMap >::BiConVisitor(
    BiConState< GraphType,CompIter,VertIter,EdgeMap > &s ):
    state( s )
{
    state.idx = 0;
    state.newComp();
    state.count = 0;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
bool BlocksPar  <DefaultStructs >::BiConVisitor< GraphType,CompIter,VertIter,EdgeMap >::visitVertexPre(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &data )
{
    state.vmap[u].count = 0;
    state.vmap[u].iscut = false;
    state.vmap[u].visited = -1;
    state.vmap[u].lowpoint = data.distance;
    state.vmap[u].depth = data.distance;
    state.vmap[u].sons = 0;
    state.vmap[u].vblFirst = -1;
    return true;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
bool BlocksPar <DefaultStructs >::BiConVisitor< GraphType,CompIter,VertIter,EdgeMap >::visitVertexPost(
    const GraphType &g, typename GraphType::PVertex u,
    VisitVertLabs< GraphType > &data )
{
    if (g.getEdgeNo( u,state.mask ) == 0)
    {
        state.addVert( u );
        state.newComp();
        return true;
    }
    if (!state.estk.empty() && data.distance == 0)
    {
        std::pair< typename GraphType::PVertex,typename GraphType::PVertex > ends;
        while (!state.estk.empty())
        {
            ends = g.getEdgeEnds( state.estk.top() );
            state.addEdge( state.estk.top() );
            if (state.vmap[ends.first].visited < state.count)
            {
                state.addVert( ends.first );
                state.vmap[ends.first].visited = state.count;
            }
            if (state.vmap[ends.second].visited < state.count)
            {
                state.addVert( ends.second );
                state.vmap[ends.second].visited = state.count;
            }
            state.estk.pop();
        }
        state.newComp();
    }
    return true;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
bool BlocksPar <DefaultStructs >::BiConVisitor< GraphType,CompIter,VertIter,EdgeMap >::visitEdgePre(
    const GraphType &g, typename GraphType::PEdge e, typename GraphType::PVertex u )
{
    EdgeType tp;
    typename GraphType::PVertex v;
    if (state.emap.hasKey( e )) return false;
    state.emap[e] = true;
    tp = g.getEdgeType( e );
    if (tp == Loop)
    {
        state.addEdge( e );
        state.addVert( u );
        state.newComp();
        return false;
    }
    v = g.getEdgeEnd( e,u );
    if (state.vmap.hasKey( v ))
    {
        if (state.vmap[v].depth < state.vmap[u].depth) state.estk.push( e );
        state.vmap[u].lowpoint = std::min( state.vmap[u].lowpoint,state.vmap[v].depth );
        return false;
    }
    state.estk.push( e );
    state.vmap[u].sons++;
    return true;
}

template <class DefaultStructs >
template< class GraphType, class CompIter, class VertIter, class EdgeMap >
bool BlocksPar <DefaultStructs >::BiConVisitor< GraphType,CompIter,VertIter,EdgeMap >::visitEdgePost(
    const GraphType &g, typename GraphType::PEdge e,
    typename GraphType::PVertex u )
{
    typename GraphType::PEdge se;
    typename GraphType::PVertex v;
    v = g.getEdgeEnd( e,u );

    state.vmap[u].lowpoint = std::min( state.vmap[u].lowpoint,state.vmap[v].lowpoint );

    if ((state.vmap[v].lowpoint >= state.vmap[u].depth && state.vmap[u].depth > 0)
        || (state.vmap[u].depth == 0 && state.vmap[u].sons > 1))
    {
        state.vmap[u].iscut = true;
        std::pair< typename GraphType::PVertex,typename GraphType::PVertex > ends;
        while (!state.estk.empty())
        {
            se = state.estk.top();
            ends = g.getEdgeEnds( se );
            state.addEdge( se );
            if (state.vmap[ends.first].visited < state.count)
            {
                state.addVert( ends.first );
                state.vmap[ends.first].visited = state.count;
            }
            if (state.vmap[ends.second].visited < state.count)
            {
                state.addVert( ends.second );
                state.vmap[ends.second].visited = state.count;
            }
            state.estk.pop();

            if( se == e) break;
        }
        state.newComp();
    }
    return true;
}

template <class DefaultStructs >
template< class State, class VertMap, class VertBlockIter >
void BlocksPar <DefaultStructs >::storeBlocksData(
    State &state, VertBlockList *vertBlockList, VertMap &vertMap,
    VertBlockIter &vertBlocks )
{
    int outIdx = 0, p;
    typename State::PVertex v = state.vmap.firstKey(), e = state.vmap.lastKey();
    for( ; 1; v = state.vmap.nextKey( v ))
    {
        if (!isBlackHole( vertMap ))
            vertMap[v] = VertData( state.vmap[v].count,outIdx );
        p = state.vmap[v].vblFirst;
        while (p >= 0)
        {
            *vertBlocks = vertBlockList[p].block;
            ++vertBlocks;
            ++outIdx;
            p = vertBlockList[p].next;
        }
        if (v == e) break;
    }
}

template <class DefaultStructs >
template< class GraphType, class VertDataMap, class EdgeDataMap,
    class CompIter, class VertIter, class VertBlockIter >
int BlocksPar <DefaultStructs >::split(
    const GraphType &g, VertDataMap &vertMap, EdgeDataMap &edgeMap,
    CompStore< CompIter,VertIter > blocks, VertBlockIter vertBlocks)
{
    int rv;
    const EdgeType mask =EdAll;
    typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, VisitVertLabs< GraphType > >::Type visited(g.getVertNo());
    typename GraphType::PEdge LOCALARRAY( stbuf,g.getEdgeNo() + 1 );    // TODO: size?
    VertBlockList LOCALARRAY( vertBlockList,g.getEdgeNo() * 2 + g.getVertNo() );    // TODO: size?
    BiConState< GraphType,CompIter,VertIter,EdgeDataMap > state(
//        blocks,edgeMap,mask,std::make_pair( stbuf,g.getEdgeNo() + 1 ),vertBlockList,g.getVertNo() );
        blocks,edgeMap,mask,std::make_pair( stbuf,g.getEdgeNo() + 1 ),vertBlockList,g.getEdgeNo() * 2 + g.getVertNo() );
    BiConVisitor< GraphType,CompIter,VertIter,EdgeDataMap > visit( state );

    rv = DFSPar<DefaultStructs>::visitAllBase( g,visited,visit,mask );
    if (rv < 0) return rv;

    storeBlocksData( state,vertBlockList,vertMap,vertBlocks );

    return state.count;
}

template <class DefaultStructs >
template< class GraphType, class VertDataMap, class EdgeDataMap,
    class CompIter, class VertIter, class VertBlockIter >
int BlocksPar  <DefaultStructs >::splitComp(
    const GraphType &g, typename GraphType::PVertex u, VertDataMap &vertMap,
    EdgeDataMap &edgeMap, CompStore< CompIter,VertIter > blocks,
    VertBlockIter vertBlocks)
{
    int rv;
    const EdgeType mask =EdAll;//if (mask & (EdDirIn | EdDirOut)) mask |= EdDirOut | EdDirIn;
    typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, VisitVertLabs< GraphType > >::Type visited(g.getVertNo());
    typename GraphType::PEdge LOCALARRAY( stbuf,g.getEdgeNo() + 1 );    // TODO: size?
    VertBlockList LOCALARRAY( vertBlockList,g.getEdgeNo() * 2 + g.getVertNo() );    // TODO: size?
    BiConState< GraphType,CompIter,VertIter,EdgeDataMap > state(
//        blocks,edgeMap,mask,std::make_pair( stbuf,g.getEdgeNo() + 1 ),vertBlockList,g.getVertNo() );
        blocks,edgeMap,mask,std::make_pair( stbuf,g.getEdgeNo() + 1 ),vertBlockList,g.getEdgeNo() * 2 + g.getVertNo() );
    BiConVisitor< GraphType,CompIter,VertIter,EdgeDataMap > visit( state );


    rv = DFSPar<DefaultStructs>::visitBase( g,u,visited,visit,mask,0 );
    if (rv != 0)
    {
        storeBlocksData( state,vertBlockList,vertMap,vertBlocks );
        return state.count;
    }
    return 0;
}
