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
{}

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

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeNext(
    PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgeNext( e,mask );
    while (e && !(vchoose( up().getEdgeEnd1(e),up() )
        && vchoose( up().getEdgeEnd2(e),up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeNext(
    PVertex vert, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgeNext( vert,e,mask );
//    while (e && !(vchoose( up().getEdgeEnd1(e),up() )
//        && vchoose( up().getEdgeEnd2(e),up() ) && echoose( e,up() )));
    while (e && !(vchoose( e->getEnd(vert),up() ) && echoose( e,up() )));

    return e;
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgePrev(
    typename Subgraph< Graph,VChooser,EChooser >::PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgePrev( e,mask );
    while (e && !(vchoose( up().getEdgeEnd1(e),up() )
        && vchoose( up().getEdgeEnd2(e),up() ) && echoose( e,up() )));

    return e;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getVertNo( ) const
{   bool b;
    if (isBoolChooser(vchoose,b))
        if (b) return up().getVertNo();
        else return 0;
    else return this->getVerts( blackHole );
}


template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getEdgeNo( EdgeDirection mask ) const
{   bool bv,be;
    EdgeType amask;
    if (isBoolChooser(vchoose,bv) && isBoolChooser(echoose,be))
    {   if (!bv || !be) return 0;
        return up().getEdgeNo(mask);
    }
    if (isBoolChooser(vchoose,bv) && isEdgeTypeChooser(echoose,amask))
    {   if (!bv) return 0;
        return up().getEdgeNo(mask&amask);
    }
    return this->getEdges( blackHole,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgePrev(
    PVertex vert, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgePrev( vert,e,mask );
    while (e && !(vchoose( e->getEnd(vert),up() ) && echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getEdgeNo(
    PVertex vert, EdgeDirection mask ) const
{   bool bv,be;
    EdgeType amask;
    if (isBoolChooser(vchoose,bv) && isBoolChooser(echoose,be))
    {   if (!bv || !be) return 0;
        return up().getEdgeNo(vert,mask);
    }
    if (isBoolChooser(vchoose,bv) && isEdgeTypeChooser(echoose,amask))
    {   if (!bv) return 0;
        return up().getEdgeNo(vert,mask&amask);
    }
    return this->getEdges( blackHole,vert,mask );
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgeNext(
    PVertex vert1, PVertex vert2, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgeNext( vert1,vert2,e,mask );
    while (e && !(
//                  vchoose( up().getEdgeEnd1(e),up() ) && vchoose( up().getEdgeEnd2(e),up() ) &&
                  echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
typename Subgraph< Graph,VChooser,EChooser >::PEdge
Subgraph< Graph,VChooser,EChooser >::getEdgePrev(
    PVertex vert1, PVertex vert2, PEdge e, EdgeDirection mask ) const
{
    do
        e = up().getEdgePrev( vert1,vert2,e,mask );
    while (e && !(
//                  vchoose( up().getEdgeEnd1(e),up() ) && vchoose( up().getEdgeEnd2(e),up() ) &&
                  echoose( e,up() )));
    return e;
}

template< class Graph, class VChooser, class EChooser >
int Subgraph< Graph,VChooser,EChooser >::getEdgeNo(
    PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{   bool bv,be;
    EdgeType amask;
    if (isBoolChooser(vchoose,bv) && isBoolChooser(echoose,be))
    {   if (!bv || !be) return 0;
        return up().getEdgeNo(vert1,vert2,mask);
    }
    if (isBoolChooser(vchoose,bv) && isEdgeTypeChooser(echoose,amask))
    {   if (!bv) return 0;
        return up().getEdgeNo(vert1,vert2,mask&amask);
    }
    return this->getEdges( blackHole,vert1,vert2,mask );
}



template< class Graph, class VChooser, class EChooser >
Subgraph< Graph,VChooser,EChooser > makeSubgraph(
    const Graph &g, const std::pair< VChooser,EChooser > &chs )
{
    return Subgraph< Graph,VChooser,EChooser >( g,chs );
}
