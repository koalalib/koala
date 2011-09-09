// ----------------------------------------------------------------------------
// Subgraph

template< class Graph, class VChooser, class EChooser >
Subgraph< Graph,VChooser,EChooser >::Subgraph(
    const Graph &g, std::pair< VChooser,EChooser > chs ):
    vchoose( chs.first ),
    echoose( chs.second ),
    SubgraphBase()
{
    SubgraphBase::link( &g );
}

template< class Graph, class VChooser, class EChooser >
Subgraph< Graph,VChooser,EChooser >::Subgraph( std::pair< VChooser,EChooser > chs ):
    vchoose( chs.first ),
    echoose( chs.second ),
    SubgraphBase()
{
}

template< class Graph, class VChooser, class EChooser >
void Subgraph< Graph,VChooser,EChooser >::setChoose(
    const std::pair< VChooser,EChooser > &chs )
{
    vchoose = chs.first;
    echoose = chs.second;
}

template< class Graph, class VChooser, class EChooser >
const typename Subgraph< Graph,VChooser,EChooser >::RootGrType
*Subgraph< Graph,VChooser,EChooser >::getRootPtr() const
{
    return parent ? (( const ParentGrType*)parent)->getRootPtr() : NULL;
}

template< class Graph, class VChooser, class EChooser >
const typename Subgraph< Graph,VChooser,EChooser >::ParentGrType
&Subgraph< Graph,VChooser,EChooser >::up() const
{
    const ParentGrType *res = getParentPtr();
    assert( res );
    return *res;
}

template< class Graph, class VChooser, class EChooser >
const typename Subgraph< Graph,VChooser,EChooser >::RootGrType
&Subgraph< Graph,VChooser,EChooser >::root() const
{
    const RootGrType *res = getRootPtr();
    assert( res );
    return *res;
}

template< class Graph, class VChooser, class EChooser >
bool Subgraph< Graph,VChooser,EChooser >::good( PVertex vert, bool deep ) const
{
    if (!vert) return true;
    return vchoose( vert,up() ) && (!deep || up().good( vert,true ));
}

template< class Graph, class VChooser, class EChooser >
bool Subgraph< Graph,VChooser,EChooser >::good( PEdge edge, bool deep ) const
{
    if (!edge) return true;
    std::pair< PVertex,PVertex > ends = edge->getEnds();
    return vchoose( ends.first,up() ) && vchoose( ends.second,up() )
        && echoose( edge,up() ) && (!deep || up().good( edge,true ));
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PVertex
Subgraph< Graph,VChooser,EChooser >::getVertNext( PVertex v ) const
{
    do
        v = up().getVertNext( v );
    while (v && !vchoose( v,up() ));
    return v;
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PVertex
Subgraph< Graph,VChooser,EChooser >::getVertPrev( PVertex v ) const
{
    do
        v = up().getVertPrev( v );
    while (v && !vchoose( v,up() ));
    return v;
}

template< class Graph, class VChooser, class EChooser > template< class OutputIterator >
int Subgraph< Graph,VChooser,EChooser >::getVerts( OutputIterator iter ) const
{
    typename Graph::PVertex LOCALARRAY( buf,root().getVertNo() );
    int a = up().getVerts( buf ), res = 0;
    for( int i = 0; i < a; i++ )
        if (good( buf[i],false ))
        {
            *iter = buf[i];
            ++iter;
            ++res;
        }
    return res;
}

template< class Graph, class VChooser, class EChooser >
Set< typename Subgraph< Graph,VChooser,EChooser >::PVertex >
Subgraph< Graph,VChooser,EChooser >::getVertSet() const
{
    Set< typename Graph::PVertex > res;
    getVerts( setInserter( res ) );
    return res;
}

template< class Graph, class VChooser, class EChooser >
typename Graph::PVertex Subgraph< Graph,VChooser,EChooser >::getEdgeEnd(
    PEdge edge, PVertex vert ) const
{
    return root().getEdgeEnd( edge,vert );
}

template< class Graph, class VChooser, class EChooser >
typename Graph::PVertex Subgraph< Graph,VChooser,EChooser >::getEnd(
    PEdge edge, PVertex vert ) const
{
    return root().getEdgeEnd( edge,vert );
}

template< class Graph, class VChooser, class EChooser >
EdgeDirection Subgraph< Graph,VChooser,EChooser >::getEdgeDir(
    PEdge edge, PVertex vert ) const
{
    return root().getEdgeDir( edge,vert );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeNext(
    PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgeNext( e,mask );
    while (e && !(vchoose( e->getEnds().first,up() )
        && vchoose( e->getEnds().second,up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::Delta( EdgeDirection direct ) const
{
    return std::max( 0,maxDeg( direct ).second );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeNext(
    PVertex vert, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgeNext( vert,e,mask );
    while (e && !(vchoose( e->getEnds().first,up() )
        && vchoose( e->getEnds().second,up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgePrev(
    typename Subgraph< Graph,VChooser,EChooser >::PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgePrev( e,mask );
    while (e && !(vchoose( e->getEnds().first,up() )
        && vchoose( e->getEnds().second,up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
bool Subgraph< Graph,VChooser,EChooser >::areParallel(
    typename Subgraph< Graph,VChooser,EChooser >::PEdge e1,
    typename Subgraph< Graph,VChooser,EChooser >::PEdge e2, EdgeDirection reltype ) const
{
    if (!e1 || !e2 || !(reltype == EdDirIn || reltype == EdDirOut
        || reltype == EdUndir)) return false;
    std::pair< typename Graph::PVertex,typename Graph::PVertex >
        ends1 = getEdgeEnds( e1 ), ends2 = getEdgeEnds( e2 );
    if (e1 == e2) return true;
    else if (e1->getType() == Loop)
        return e2->getType() == Loop && ends1.first == ends2.first;
    else if (e2->getType() == Loop) return false;
    else if ((e1->getType() == Undirected && e2->getType() == Undirected)
        || (e1->getType() != e2->getType() && reltype == EdUndir)
        || (e1->getType() == Directed && e2->getType() == Directed
        && (reltype == EdUndir || reltype == EdDirIn)))
        return (ends1.first == ends2.first && ends1.second == ends2.second)
                || (ends1.second == ends2.first && ends1.first == ends2.second);
    else return e1->getType() == e2->getType() && ends1.first == ends2.first
        && ends1.second == ends2.second;
}

template< class Graph, class VChooser, class EChooser > template <class OutputIterator>
int Subgraph< Graph,VChooser,EChooser >::getParals(
                OutputIterator iter, typename Graph::PEdge edge, EdgeDirection reltype) const
{
    if (!edge || !(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir)) return 0;
    int licz=0;
    std::pair<typename Graph::PVertex,typename Graph::PVertex>
            ends=getEdgeEnds(edge);
    for(typename Graph::PEdge e=getEdge(ends.first,ends.second,EdAll);e;
                e=getEdgeNext(ends.first,ends.second,e,EdAll))
        if (e!=edge && areParallel(e,edge,reltype))
        {   *iter=e;
            ++iter;
            licz++;
        }
    return licz;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::mu( typename Graph::PEdge edge, EdgeDirection reltype) const
{
    if (!(edge && (reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir))) return 0;
    return getParals(blackHole,edge,reltype)+1;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::mu( EdgeDirection reltype) const
{
    return maxMu(reltype).second;
}

template< class Graph, class VChooser, class EChooser >
std::pair< typename Graph::PEdge,int >
Subgraph< Graph,VChooser,EChooser >::maxMu( EdgeDirection reltype) const
{
    std::pair< typename Graph::PEdge,int > res(0,0);
    if (!(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir) || !getEdgeNo(EdAll)) return res;
    Parals3 LOCALARRAY(edges,getEdgeNo(EdAll));
    int i=0,l=0;
    typename Graph::PEdge edge;
    for (typename Graph::PEdge e=getEdge(EdAll); e; e=getEdgeNext(e,EdAll) )
        edges[i++]=Parals3(std::min(e->getEnds().first,e->getEnds().second),
                           std::max(e->getEnds().first,e->getEnds().second),
                           getEdgeDir(e,std::min(e->getEnds().first,e->getEnds().second)),e);
    GraphClassDefaultSettings::sort(edges,edges+i,Parals3cmp());
    for(i=0;i<getEdgeNo(EdAll);i++)
    {
        if (i==0 || !areParallel(edges[i-1].edge,edges[i].edge,reltype))
        {
            l=1; edge=edges[i].edge;
        } else l++;
        if (l>res.second) res=std::make_pair(edge,l);
    }
    return res;
}


template< class Graph, class VChooser, class EChooser >
Subgraph< Graph,VChooser,EChooser > makeSubgraph(
    const Graph &g, const std::pair< VChooser,EChooser > &chs )
{
    return Subgraph< Graph,VChooser,EChooser >( g,chs );
}

template< class Graph, class VChooser, class EChooser >
std::pair< typename Graph::PVertex,int >
Subgraph< Graph,VChooser,EChooser >::maxDeg( EdgeDirection direct ) const
{
    std::pair< typename Graph::PVertex,int > res( 0,-1 );
    if (!getVertNo()) return res;
    else
    {
        res.first = getVert();
        res.second = deg( res.first,direct );
    }
    int degree;
    typename Graph::PVertex vert = getVertNext( res.first );
    for( ; vert; vert = getVertNext( vert ) )
        if ((degree = deg( vert,direct )) > res.second)
            res = std::make_pair( vert,degree );
    return res;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::delta( EdgeDirection direct ) const
{
    return std::max( 0,minDeg( direct ).second );
}

template< class Graph, class VChooser, class EChooser >
std::pair< typename Graph::PVertex,int >
Subgraph< Graph,VChooser,EChooser >::minDeg( EdgeDirection direct ) const
{
    std::pair< typename Graph::PVertex,int > res( 0,-1 );
    if (!getVertNo()) return res;
    else
    {
        res.first = getVert();
        res.second = deg( res.first,direct );
    }
    int degree;
    typename Graph::PVertex vert = getVertNext( res.first );
    for( ; vert; vert = getVertNext( vert ) )
        if ((degree = deg( vert,direct )) < res.second)
            res = std::make_pair( vert,degree );
    return res;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::deg( PVertex vert, EdgeDirection direct ) const
{
    return getEdgeNo( vert,direct ) + ((direct & EdLoop) ? getEdgeNo( vert,EdLoop ): 0);
}

template< class Graph, class VChooser, class EChooser > template< class OutputIterator >
int Subgraph< Graph,VChooser,EChooser >::getEdges(
    OutputIterator iter, EdgeDirection mask ) const
{
    typename Graph::PEdge LOCALARRAY( buf,root().getEdgeNo( mask ) );
    int a = up().getEdges( buf,mask ), res = 0;
    for( int i = 0; i < a; i++ )
        if (good( buf[i],false ))
        {
            *iter = buf[i];
            ++iter;
            ++res;
        }
    return res;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getEdgeNo( EdgeDirection mask ) const
{
    return getEdges( blackHole,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdge( PVertex vert, EdgeDirection mask ) const
{
    return getEdgeNext( vert,NULL,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeLast( PVertex vert, EdgeDirection mask ) const
{
    return getEdgePrev( vert,NULL,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgePrev(
    PVertex vert, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgePrev( vert,e,mask );
    while (e && !(vchoose( e->getEnds().first,up() )
        && vchoose( e->getEnds().second,up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
Set< typename Subgraph< Graph,VChooser,EChooser >::PEdge >
Subgraph< Graph,VChooser,EChooser >::getEdgeSet( PVertex vert, EdgeDirection mask ) const
{
    Set< typename Graph::PEdge > res;
    getEdges( setInserter( res ),vert,mask );
    return res;
}

template< class Graph, class VChooser, class EChooser > template< class OutputIterator >
int Subgraph< Graph,VChooser,EChooser >::getEdges(
    OutputIterator iter, PVertex vert, EdgeDirection mask ) const
{
    typename Graph::PEdge LOCALARRAY( buf,root().getEdgeNo( vert,mask ) );
    int a = up().getEdges( buf,vert,mask ), res = 0;
    for( int i = 0; i < a; i++ )
        if (good( buf[i],false ))
        {
            *iter = buf[i];
            ++iter;
            ++res;
        }
    return res;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getEdgeNo(
    PVertex vert, EdgeDirection mask ) const
{
    return getEdges( blackHole,vert,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdge(
    PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{
    return getEdgeNext( vert1,vert2,NULL,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeLast(
    PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{
    return getEdgePrev( vert1,vert2,NULL,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeNext(
    PVertex vert1, PVertex vert2, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgeNext( vert1,vert2,e,mask );
    while (e && !(vchoose( e->getEnds().first,up() )
        && vchoose( e->getEnds().second,up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgePrev(
    PVertex vert1, PVertex vert2, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgePrev( vert1,vert2,e,mask );
    while (e && !(vchoose( e->getEnds().first,up() )
        && vchoose( e->getEnds().second,up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
Set< typename Subgraph< Graph,VChooser,EChooser >::PEdge >
Subgraph< Graph,VChooser,EChooser >::getEdgeSet(
    PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{
    Set< typename Graph::PEdge > res;
    getEdges( setInserter( res ),vert1,vert2,mask );
    return res;
}

template< class Graph, class VChooser, class EChooser > template< class OutputIterator >
int Subgraph< Graph,VChooser,EChooser >::getEdges(
    OutputIterator iter, PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{
    typename Graph::PEdge LOCALARRAY( buf,root().getEdgeNo( vert1,mask ) );
    int a = up().getEdges( buf,vert1,vert2,mask ), res = 0;
    for( int i = 0; i < a; i++ )
        if (good( buf[i],false ))
        {
            *iter = buf[i];
            ++iter;
            ++res;
        }
    return res;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getEdgeNo(
    PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{
    return getEdges( blackHole,vert1,vert2,mask );
}

template< class Graph, class VChooser, class EChooser > template <class Cont>
void
Subgraph< Graph,VChooser,EChooser >::getAdj(Cont& cont) const
{
    std::pair<PVertex,PVertex> ends;
    for(PEdge e=getEdge();e;e=getEdgeNext(e))
    {   ends=getEdgeEnds(e);
        cont(ends.first,ends.second)=true;
        if (getEdgeType(e)==Undirected)
            cont(ends.second,ends.first)=true;
    }
}

template< class Graph, class VChooser, class EChooser > template< class OutputIterator >
int Subgraph< Graph,VChooser,EChooser >::getNeigh(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
    typename Graph::PVertex LOCALARRAY( ans,root().getEdgeNo( vert,direct ) );
    int size = 0, res = 0;
    PEdge edge = getEdge( vert,direct );
    while (edge)
    {
        ans[size++] = getEdgeEnd( edge,vert );
        edge = getEdgeNext( vert,edge,direct );
    }
    GraphClassDefaultSettings::sort( ans,ans + size );
    for( int i = 0; i < size; i++ )
        if (i == 0 || ans[i - 1] != ans[i])
        {
            *out = ans[i];
            ++out;
            ++res;
        }
    return res;
}

template< class Graph, class VChooser, class EChooser >
Set< typename Subgraph< Graph,VChooser,EChooser >::PVertex >
Subgraph< Graph,VChooser,EChooser >::getNeighSet(
    PVertex vert, EdgeDirection mask ) const
{
    Set< typename Graph::PVertex > res;
    getNeigh( setInserter( res ),vert,mask );
    return res;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getNeighNo(
    PVertex vert, EdgeDirection mask ) const
{
    return getNeigh( blackHole,vert,mask );
}

template< class Graph, class VChooser, class EChooser >
Set< typename Graph::PVertex > Subgraph< Graph,VChooser,EChooser >::getClNeighSet(
    PVertex vert, EdgeDirection direct ) const
{
    Set< typename Graph::PVertex > ans = getNeighSet( vert,direct );
    ans.add( vert );
    return ans;
}

template< class Graph, class VChooser, class EChooser > template< class OutputIterator >
int Subgraph< Graph,VChooser,EChooser >::getClNeigh(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
    typename Graph::PVertex LOCALARRAY( ans,root().getEdgeNo( vert,direct ) + 1 );
    ans[0] = vert;
    int size = 1, res = 0;
    PEdge edge = getEdge( vert,direct );
    while (edge)
    {
        ans[size++] = getEdgeEnd( edge,vert );
        edge = getEdgeNext( vert,edge,direct );
    }
    GraphClassDefaultSettings::sort( ans,ans + size );
    for( int i = 0; i < size; i++ )
        if (i == 0 || ans[i - 1] != ans[i])
        {
            *out = ans[i];
            ++out;
            ++res;
        }
    return res;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getClNeighNo(
    PVertex vert, EdgeDirection direct ) const
{
    return getClNeigh( blackHole,vert,direct );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PVertex
Subgraph< Graph,VChooser,EChooser >::vertByNo( int idx ) const
{
    typename Subgraph< Graph,VChooser,EChooser >::PVertex tmp_vert = getVert();
    while (idx && tmp_vert)
    {
        tmp_vert = getVertNext( tmp_vert );
        --idx;
    }
    return tmp_vert;
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::edgeByNo( int idx ) const
{
    typename Subgraph< Graph,VChooser,EChooser >::PEdge tmp_edge = getEdge();
    while (idx && tmp_edge)
    {
        tmp_edge = getEdgeNext( tmp_edge );
        --idx;
    }
    return tmp_edge;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::vertPos( PVertex vert ) const
{
    int idx = 0;
    typename Subgraph< Graph,VChooser,EChooser >::PVertex tmp_vert = getVert();
    while (tmp_vert && tmp_vert != vert)
    {
        tmp_vert = getVertNext( tmp_vert );
        ++idx;
    }
    return tmp_vert ? idx : -1;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::edgePos( PEdge edge ) const
{
    int idx = 0;
    typename Subgraph< Graph,VChooser,EChooser >::PEdge tmp_edge = getEdge();
    while (tmp_edge && tmp_edge != edge)
    {
        tmp_edge = getEdgeNext( tmp_edge );
        ++idx;
    }
    return tmp_edge ? idx : -1;
}
