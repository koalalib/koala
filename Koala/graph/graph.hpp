// ----------------------------------------------------------------------------
// SubgraphBase

SubgraphBase::SubgraphBase( const SubgraphBase &x ):
    child( NULL ),
    parent( NULL ),
    next( NULL )
{
    link( x.parent );
}

SubgraphBase &SubgraphBase::operator=( const SubgraphBase &x )
{
    if (&x != this) link( x.parent );
    return *this;
}

SubgraphBase::~SubgraphBase()
{
    unlink();
    const SubgraphBase *ch, *chnext;
    for( ch = child; ch; ch = chnext )
    {
        ch->parent = NULL;
        chnext = ch->next;
        ch->next = NULL;
    }
}

bool SubgraphBase::unlink()
{
    if (!parent) return false;
    if (parent->child == this) parent->child = next;
    else
    {
        const SubgraphBase *wsk;
        for( wsk = parent->child; wsk->next != this; wsk = wsk->next ) ;
        wsk->next = next;
    }
    parent = next = NULL;
    return true;
}

bool SubgraphBase::link(const SubgraphBase *wsk )
{
    unlink();
    if (!wsk) return false;
    next = wsk->child;
    parent = wsk;
    wsk->child = this;
    return true;
}

// ----------------------------------------------------------------------------
// Graph< VertInfo,EdgeInfo,Settings >

template< class VertInfo, class EdgeInfo, class Settings >
Graph< VertInfo,EdgeInfo,Settings >::Graph():
    first_vert( NULL ),
    last_vert( NULL ),
    first_edge( NULL ),
    last_edge( NULL ),
    no_vert( 0 ),
    pAdj( NULL ),
    SubgraphBase()
{
}

template< class VertInfo, class EdgeInfo, class Settings >
Graph< VertInfo,EdgeInfo,Settings >::Graph( const Graph< VertInfo,EdgeInfo,Settings > &graph ):
    first_vert( NULL ),
    last_vert( NULL ),
    first_edge( NULL ),
    last_edge( NULL ),
    no_vert( 0 ),
    pAdj( NULL ),
    SubgraphBase()
{
    this->copy( graph );
}

template< class VertInfo, class EdgeInfo, class Settings >
Graph< VertInfo,EdgeInfo,Settings >::~Graph()
{
    if (pAdj)
    {
        delete pAdj;
        pAdj = 0;
    }
    this->clear();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::getVertNo() const
{
    return this->no_vert;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::getEdgeNo( EdgeDirection direct ) const
{
    int ans = 0;
    if (direct & EdLoop) ans += this->no_loop_edge();
    if (direct & EdUndir) ans += this->no_undir_edge();
    if (direct & (EdDirIn | EdDirOut)) ans += this->no_dir_edge();
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings >
void Graph< VertInfo,EdgeInfo,Settings >::clear()
{
    if (pAdj) pAdj->clear();
    PVertex cur_vert = this->first_vert;
    while (cur_vert)
    {
        PVertex tmp_vert = cur_vert->next;
        delete cur_vert;
        cur_vert = tmp_vert;
    }
    PEdge cur_edge = first_edge;
    while (cur_edge)
    {
        PEdge tmp_edge = cur_edge->next;
        delete cur_edge;
        cur_edge = tmp_edge;
    }
    first_vert = last_vert = NULL;
    first_edge = last_edge = NULL;
    no_vert = no_loop_edge() = no_dir_edge() = no_undir_edge() = 0;
}

template< class VertInfo, class EdgeInfo, class Settings >
void Graph< VertInfo,EdgeInfo,Settings >::clearEdges()
{
    if (pAdj) pAdj->clear();
    PEdge cur_edge = first_edge;
    while (cur_edge)
    {
        PEdge tmp_edge = cur_edge->next;
        delete cur_edge;
        cur_edge = tmp_edge;
    }
    first_edge = last_edge = NULL;
    no_loop_edge() = no_dir_edge() = no_undir_edge() = 0;

    PVertex cur_vert = first_vert;
    while (cur_vert)
    {
//        for( int i = 0; i < 4; i++ )
        {   cur_vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getFirst()=
            cur_vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getFirst()=
            cur_vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getFirst()=
            cur_vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getFirst()=
            cur_vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getLast()=
            cur_vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast()=
            cur_vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getLast()=
            cur_vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getLast()= NULL;

            cur_vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getDegree()=
            cur_vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree()=
            cur_vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getDegree()=
            cur_vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getDegree()= 0;
        }
        cur_vert = cur_vert->next;
    }
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::addVert( const VertInfo &infoExt )
{
    PVertex tmp_vert = new Vertex( infoExt );
    if(!attach( tmp_vert ))
    {
        delete tmp_vert;
        return NULL;
    }
    return tmp_vert;
}

template< class VertInfo, class EdgeInfo, class Settings >
void Graph< VertInfo,EdgeInfo,Settings >::delVert( PVertex vert, bool force )
{
    if (!vert) return;
    if (force)
    {
        PEdge edge = getEdge( vert );
        while (edge)
        {
            detach( edge );
            delete edge;
            edge = getEdge( vert );
        }
    }
    if (detach( vert )) delete vert;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline void Graph< VertInfo,EdgeInfo,Settings >::del( PVertex vert, bool force )
{
    this->delVert( vert,force );
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::VertInfoType
Graph< VertInfo,EdgeInfo,Settings >::getVertInfo( PVertex vert ) const
{
    return vert->getInfo();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline void Graph< VertInfo,EdgeInfo,Settings >::setVertInfo( PVertex vert, const VertInfo &info ) const
{
    vert->setInfo( info );
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getVert() const
{
    return this->first_vert;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getVertLast() const
{
    return this->last_vert;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getVertNext( PVertex vert ) const
{
    if (!vert) return this->first_vert;
    return vert->next;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getVertPrev( PVertex vert ) const
{
    if (!vert) return this->last_vert;
    return vert->prev;
}

template< class VertInfo,class EdgeInfo , class Settings>
Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex >
Graph< VertInfo,EdgeInfo,Settings >::getVertSet() const
{
    Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > ans;
    PVertex vert = this->first_vert;
    while (vert)
    {
        ans.add( vert );
        vert = vert->next;
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class OutputIterator >
int Graph< VertInfo,EdgeInfo,Settings >::getVerts( OutputIterator out ) const
{
    int ans = 0;
    PVertex vert = this->first_vert;
    while (vert)
    {
        *out = vert;
        ++out;
        ++ans;
        vert = vert->next;
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::addEdge(
    PVertex vert1, PVertex vert2, EdgeDirection direct )
{
    PEdge tmp_edge = new Edge();
    if (!attach( tmp_edge,vert1,vert2,direct ))
    {
        delete tmp_edge;
        return NULL;
    }
    return tmp_edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::addEdge(
    PVertex vert1, PVertex vert2, const EdgeInfo &infoExt, EdgeDirection direct )
{
    PEdge tmp_edge = new Edge( infoExt );
    if (!attach( tmp_edge,vert1,vert2,direct ))
    {
        delete tmp_edge;
        return NULL;
    }
    return tmp_edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::addArch(
    PVertex v_out, PVertex v_in, const EdgeInfo &infoExt )
{
    PEdge tmp_edge = new Edge( infoExt );
    if (!attach_dir( tmp_edge,v_out,v_in ))
    {
        delete tmp_edge;
        return NULL;
    }
    return tmp_edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::addLoop( PVertex vert, const EdgeInfo &infoExt )
{
    PEdge tmp_edge = new Edge( infoExt );
    if (!attach_loop( tmp_edge,vert ))
    {
        delete tmp_edge;
        return NULL;
    }
    return tmp_edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::EdgeInfoType
Graph< VertInfo,EdgeInfo,Settings >::getEdgeInfo( PEdge edge ) const
{
    return edge->getInfo();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline void Graph< VertInfo,EdgeInfo,Settings >::setEdgeInfo( PEdge edge, const EdgeInfo& info ) const
{
    edge->setInfo( info );
}

template< class VertInfo, class EdgeInfo, class Settings >
inline void Graph< VertInfo,EdgeInfo,Settings >::del( PEdge edge )
{
    if (detach( edge )) delete edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline void Graph< VertInfo,EdgeInfo,Settings >::delEdge( PEdge edge )
{
    this->del( edge );
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdge( EdgeDirection direct ) const
{
    PEdge edge = this->first_edge;
    while (edge && !((int)edge->type & direct)) edge = edge->next;
    return edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgeLast( EdgeDirection direct ) const
{
    PEdge tmp_edge = this->last_edge;
    while (tmp_edge && !((int)tmp_edge->type & direct)) tmp_edge = tmp_edge->prev;
    return tmp_edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgeNext(
    PEdge edge, EdgeDirection direct ) const
{
    PEdge tmp_edge = edge ? edge->next : this->first_edge;
    while (tmp_edge && !((int)tmp_edge->type & direct)) tmp_edge = tmp_edge->next;
    return tmp_edge;
}

template< class VertInfo,class EdgeInfo , class Settings>
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgePrev( PEdge edge, EdgeDirection direct ) const
{
    PEdge tmp_edge = edge ? edge->prev : this->last_edge;
    while (tmp_edge && !((int)tmp_edge->type & direct)) tmp_edge = tmp_edge->prev;
    return tmp_edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge >
Graph< VertInfo,EdgeInfo,Settings >::getEdgeSet( EdgeDirection direct ) const
{
    Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge > ans;
    PEdge edge = this->first_edge;
    while (edge)
    {
        if (edge->type & direct) ans.add( edge );
        edge = edge->next;
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class OutputIterator >
int Graph< VertInfo,EdgeInfo,Settings >::getEdges( OutputIterator out, EdgeDirection direct ) const
{
    int ans = 0;
    PEdge edge = this->first_edge;
    while (edge) {
        if (edge->type & direct)
        {
            *out = edge;
            ++out;
            ++ans;
        }
        edge = edge->next;
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline EdgeType Graph< VertInfo,EdgeInfo,Settings >::getEdgeType( PEdge edge ) const
{
    return edge->getType();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline EdgeType Graph< VertInfo,EdgeInfo,Settings >::getType( PEdge edge ) const
{
    return edge->getType();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdge( PVertex vert, EdgeDirection direct ) const
{
    return getEdgeNext( vert,NULL,direct );
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgeLast( PVertex vert, EdgeDirection direct ) const
{
    return getEdgePrev( vert,NULL,direct );
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgeNext(
    PVertex vert, PEdge edge, EdgeDirection direct ) const
{
    if (!vert || !direct) return NULL;
    if (edge && !isEdgeEnd( edge,vert )) return NULL;
    EdgeDirection type = getEdgeDir( edge,vert );
    EdgeDirection nexttype = (type == EdNone) ? EdLoop : type << 1;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge res;
    if (edge && (type & direct))
        if (edge->vert[0].vert == vert) res = edge->vert[0].next;
        else res = edge->vert[1].next;
    else res = 0;
    if (res) return res;
    switch (nexttype)
    {
        case EdLoop:
            if (direct & EdLoop) res = vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getFirst();
            if (res) return res;
        case EdUndir:
            if (direct & EdUndir) res = vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getFirst();
            if (res) return res;
        case EdDirIn:
            if (direct & EdDirIn) res = vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getFirst();
            if (res) return res;
        case EdDirOut:
            if (direct & EdDirOut) res = vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getFirst();
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgePrev(
    PVertex vert, PEdge edge, EdgeDirection direct ) const
{
    if (!vert || !direct) return NULL;
    if (edge && !isEdgeEnd( edge,vert )) return NULL;
    EdgeDirection type = getEdgeDir( edge,vert );
    EdgeDirection nexttype = (type == EdNone) ? EdDirOut : type >> 1;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge res;
    if (edge && (type & direct))
        if (edge->vert[0].vert == vert) res = edge->vert[0].prev;
        else res = edge->vert[1].prev;
    else res = 0;
    if (res) return res;
    switch (nexttype)
    {
        case EdDirOut:
            if (direct & EdDirOut) res = vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getLast();
            if (res) return res;
        case EdDirIn:
            if (direct & EdDirIn) res = vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getLast();
            if (res) return res;
        case EdUndir:
            if (direct & EdUndir) res = vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast();
            if (res) return res;
        case EdLoop:
            if (direct & EdLoop) res = vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getLast();
            if (res) return res;
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge >
Graph< VertInfo,EdgeInfo,Settings >::getEdgeSet( PVertex vert, EdgeDirection direct ) const
{
    Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge > ans;
    PEdge edge = this->getEdge( vert,direct );
    while (edge)
    {
        ans.add( edge );
        edge = this->getEdgeNext( vert,edge,direct );
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class OutputIterator >
int Graph< VertInfo,EdgeInfo,Settings >::getEdges(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
    int ans = 0;
    PEdge edge = this->getEdge( vert,direct );
    while (edge)
    {
        *out = edge;
        ++out;
        ++ans;
        edge = this->getEdgeNext( vert,edge,direct );
    }
    return ans;
}

template< class VertInfo,class EdgeInfo , class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::getEdgeNo( PVertex vert, EdgeDirection direct ) const
{
    int ans = 0;
    if (direct & EdLoop)
        ans += vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getDegree();
    if (direct & EdUndir)
        ans += vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree();
    if (direct & EdDirIn)
        ans += vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getDegree();
    if (direct & EdDirOut)
        ans += vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getDegree();
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdge(
    PVertex vert1, PVertex vert2, EdgeDirection direct ) const
{
    return getEdgeNext( vert1,vert2,NULL,direct );
}

template< class VertInfo,class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgeLast(
    PVertex vert1, PVertex vert2, EdgeDirection direct ) const
{
    return getEdgePrev( vert1,vert2,NULL,direct );
}

template< class VertInfo,class EdgeInfo , class Settings>
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgeNext(
    PVertex vert1, PVertex vert2, PEdge edge, EdgeDirection direct ) const
{
    if (!vert1 || !vert2) return NULL;
    if (edge && (!isEdgeEnd( edge,vert1 ) || !isEdgeEnd( edge,vert2 ))) return NULL;
    if (vert1 == vert2)
        if (direct & EdLoop) return getEdgeNext( vert1,edge,EdLoop );
        else return 0;
    else direct &= ~EdLoop;
    if (!direct) return 0;
    if (vert1 > vert2)
    {
        PVertex tmp_v = vert1;
        vert1 = vert2;
        vert2 = tmp_v;
        int mask = EdDirIn | EdDirOut;
        if ((direct & mask) && (direct & mask) != mask) direct ^= mask;
    }
    if (pAdj)
    {
        Privates::AdjMatrixParals<VertInfo,EdgeInfo,Settings>* p;
        EdgeDirection type = getEdgeDir( edge,vert1 );
        EdgeDirection nexttype = (type == EdNone) ? EdLoop : type << 1;
        typename Graph< VertInfo,EdgeInfo,Settings >::PEdge res;
        if (edge && (type & direct)) res = edge->nParal();
        else res = 0;
        if (res) return res;
        switch (nexttype)
        {
            case EdLoop:
            case EdUndir:
                if (direct & EdUndir)
                {
                    p = pAdj->undirspresentValPtr( vert1,vert2 );
                    if (!p) res = 0;
                    else res = p->first;
                }
                if (res) return res;
            case EdDirIn:
                if (direct & EdDirIn)
                {
                    p = pAdj->dirspresentValPtr( vert2,vert1 );
                    if (!p) res = 0;
                    else res = p->first;
                }
                if (res) return res;
            case EdDirOut:
                if (direct & EdDirOut)
                {
                    p = pAdj->dirspresentValPtr( vert1,vert2 );
                    if (!p) res = 0;
                    else res = p->first;
                }
        }
        return res;
    }

    do
        edge = getEdgeNext( vert1,edge,direct );
    while (edge && getEdgeEnd( edge,vert1 ) != vert2);
    return edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::getEdgePrev(
    PVertex vert1, PVertex vert2, PEdge edge, EdgeDirection direct ) const
{
    if (!vert1 || !vert2) return NULL;
    if (edge && (!isEdgeEnd( edge,vert1 ) || !isEdgeEnd( edge,vert2 ))) return NULL;
    if (vert1 == vert2)
        if (direct & EdLoop) return getEdgePrev( vert1,edge,EdLoop );
        else return 0;
    else direct &= ~EdLoop;
    if (!direct) return 0;
    if (vert1 > vert2)
    {
        PVertex tmp_v = vert1;
        vert1 = vert2;
        vert2 = tmp_v;
        int mask = EdDirIn | EdDirOut;
        if ((direct & mask) && (direct & mask) != mask) direct ^= mask;
    }
    if (pAdj)
    {
        Privates::AdjMatrixParals<VertInfo,EdgeInfo,Settings>* p;
        EdgeDirection type = getEdgeDir( edge,vert1 );
        EdgeDirection nexttype = (type == EdNone) ? EdDirOut : type >> 1;
        typename Graph< VertInfo,EdgeInfo,Settings >::PEdge res;
        if (edge && (type & direct)) res = edge->pParal();
        else res = 0;
        if (res) return res;
        switch (nexttype) {
            case EdDirOut:
                if (direct & EdDirOut)
                {
                    p = pAdj->dirspresentValPtr( vert1,vert2 );
                    if (!p) res = 0;
                    else res = p->last;
                }
                if (res) return res;
            case EdDirIn:
                if (direct & EdDirIn)
                {
                    p = pAdj->dirspresentValPtr( vert2,vert1 );
                    if (!p) res = 0;
                    else res = p->last;
                }
                if (res) return res;
            case EdUndir:
                if (direct & EdUndir)
                {
                    p = pAdj->undirspresentValPtr( vert1,vert2 );
                    if (!p) res = 0;
                    else res = p->last;
                }
                if (res) return res;
            case EdLoop:
                return res;
        }
        return res;
    }

    do
        edge = getEdgePrev( vert1,edge,direct );
    while (edge && getEdgeEnd( edge,vert1 ) != vert2);
    return edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge >
Graph< VertInfo,EdgeInfo,Settings >::getEdgeSet(
    PVertex vert1, PVertex vert2, EdgeDirection direct ) const
{
    Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge > ans;
    PEdge edge = this->getEdge( vert1,vert2,direct );
    while (edge)
    {
        ans.add( edge );
        edge = this->getEdgeNext( vert1,vert2,edge,direct );
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class OutputIterator >
int Graph< VertInfo,EdgeInfo,Settings >::getEdges(
    OutputIterator out, PVertex vert1, PVertex vert2, EdgeDirection direct ) const
{
    int ans = 0;
    PEdge edge = this->getEdge( vert1,vert2,direct );
    while (edge)
    {
        *out = edge;
        ++out;
        ++ans;
        edge = this->getEdgeNext( vert1,vert2,edge,direct );
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::getEdgeNo(
    PVertex vert1, PVertex vert2, EdgeDirection direct ) const
{
    if (vert1 == vert2)
        if (direct & EdLoop) return getEdgeNo( vert1,EdLoop );
        else return 0;
    else
        if (direct & EdLoop) direct &= ~EdLoop;
    if (!direct) return 0;
    if (pAdj)
        return ((direct & EdDirIn) ? pAdj->vald( vert2,vert1 ).degree : 0) +
               ((direct & EdDirOut) ? pAdj->vald( vert1,vert2 ).degree : 0) +
               ((direct & EdUndir) ? pAdj->valund( vert2,vert1 ).degree : 0);
    int ans = 0;
    PEdge tmp_edge = getEdge( vert1,vert2,direct );
    while (tmp_edge)
    {
        if (getEdgeEnd( tmp_edge,vert1 ) == vert2) ++ans;
        tmp_edge = getEdgeNext( vert1,tmp_edge,direct );
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings > template <class Cont>
void
Graph< VertInfo,EdgeInfo,Settings >::getAdj(Cont& cont) const
{
    std::pair<PVertex,PVertex> ends;
    for(PEdge e=getEdge();e;e=getEdgeNext(e))
    {   ends=getEdgeEnds(e);
        cont(ends.first,ends.second)=true;
        if (getEdgeType(e)==Undirected)
            cont(ends.second,ends.first)=true;
    }
}

template< class VertInfo, class EdgeInfo, class Settings >
inline std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex >
Graph< VertInfo,EdgeInfo,Settings >::getEdgeEnds( PEdge edge ) const
{
    return edge->getEnds();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex >
Graph< VertInfo,EdgeInfo,Settings >::getEnds( PEdge edge ) const
{
    return edge->getEnds();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getEdgeEnd1( PEdge edge ) const
{
    return edge->getEnd1();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getEdgeEnd2( PEdge edge ) const
{
    return edge->getEnd2();
}

template< class VertInfo, class EdgeInfo, class Settings >
inline bool Graph< VertInfo,EdgeInfo,Settings >::isEdgeEnd( PEdge edge, PVertex vert ) const
{
    return edge->isEnd(vert);
}

template< class VertInfo, class EdgeInfo, class Settings >
inline bool Graph< VertInfo,EdgeInfo,Settings >::isEnd( PEdge edge, PVertex vert ) const
{
    return edge->isEnd(vert);
}

template< class VertInfo,class EdgeInfo , class Settings>
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getEdgeEnd( PEdge edge, PVertex vert ) const
{
    return edge->getEnd(vert);
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::getEnd( PEdge edge, PVertex vert ) const
{
    return edge->getEnd(vert);
}

template< class VertInfo, class EdgeInfo, class Settings >
inline EdgeDirection Graph< VertInfo,EdgeInfo,Settings >::getEdgeDir( PEdge edge, PVertex vert ) const
{
    if (!edge) return EdNone; else return edge->getDir(vert);
}

template< class VertInfo, class EdgeInfo, class Settings >
Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex >
Graph< VertInfo,EdgeInfo,Settings >::getNeighSet( PVertex vert, EdgeDirection direct ) const
{
    Set< PVertex > ans;
    PEdge edge = getEdge( vert,direct );
    while (edge)
    {
        ans.add( getEdgeEnd( edge,vert ) );
        edge = getEdgeNext( vert,edge,direct );
    }
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class OutputIterator >
int Graph< VertInfo,EdgeInfo,Settings >::getNeigh(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
        LOCALARRAY( ans,getEdgeNo( vert,direct) );
    int size = 0, res = 0;
    PEdge edge = getEdge( vert,direct );
    while (edge)
    {
        ans[size++] = getEdgeEnd( edge,vert );
        edge = getEdgeNext( vert,edge,direct );
    }
    Settings::sort( ans,ans+size );
    for( int i = 0; i < size; i++)
        if (i == 0 || ans[i - 1] != ans[i])
        {
            *out = ans[i];
            ++out;
            ++res;
        }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::getNeighNo( PVertex vert, EdgeDirection direct ) const
{
    return getNeigh( blackHole,vert,direct );
}

template< class VertInfo, class EdgeInfo, class Settings >
Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex >
Graph< VertInfo,EdgeInfo,Settings >::getClNeighSet( PVertex vert, EdgeDirection direct ) const
{
    Set< PVertex > ans = getNeighSet( vert,direct );
    ans.add( (PVertex)vert );
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class OutputIterator >
int Graph< VertInfo,EdgeInfo,Settings >::getClNeigh(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
        LOCALARRAY( ans,getEdgeNo( vert,direct ) + 1 );
    ans[0] = vert;
    int size = 1, res = 0;
    PEdge edge = getEdge( vert,direct );
    while (edge)
    {
        ans[size++] = getEdgeEnd( edge,vert );
        edge = getEdgeNext( vert,edge,direct );
    }
    Settings::sort( ans,ans+size );
    for( int i = 0; i < size; i++)
        if (i == 0 || ans[i - 1] != ans[i])
        {
            *out = ans[i];
            ++out;
            ++res;
        }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::getClNeighNo( PVertex vert, EdgeDirection direct ) const
{
    return getClNeigh( blackHole,vert,direct );
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::deg( PVertex vert, EdgeDirection direct ) const
{
    int ans = 0;
    if (direct & EdLoop)
        ans += 2 * vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getDegree();
    if (direct & EdUndir)
        ans += vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree();
    if (direct & EdDirIn)
        ans += vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getDegree();
    if (direct & EdDirOut)
        ans += vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getDegree();
    return ans;
}

template< class VertInfo, class EdgeInfo, class Settings >
std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,int >
Graph< VertInfo,EdgeInfo,Settings >::minDeg( EdgeDirection direct ) const
{
    std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,int > res( 0,-1 );
    if (!getVertNo()) return res;
    res.first = getVert();
    res.second = deg( res.first,direct );
    int degree;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex vert = getVertNext( res.first );
    for( ; vert; vert = getVertNext( vert ) )
        if ((degree = deg( vert,direct )) < res.second)
            res = std::make_pair( vert,degree );
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,int >
Graph< VertInfo,EdgeInfo,Settings >::maxDeg( EdgeDirection direct ) const
{
    std::pair<typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,int > res( 0,-1 );
    if (!getVertNo()) return res;
    res.first = getVert();
    res.second = deg( res.first,direct );
    int degree;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex vert = getVertNext( res.first );
    for( ; vert; vert = getVertNext( vert ))
        if ((degree = deg( vert,direct )) > res.second)
            res = std::make_pair( vert,degree );
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::Delta( EdgeDirection direct ) const
{
    return std::max( 0,maxDeg( direct ).second );
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::delta( EdgeDirection direct ) const
{
    return std::max( 0,minDeg( direct ).second );
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::vertByNo( int idx ) const
{
    PVertex tmp_vert = first_vert;
    while (idx && tmp_vert)
    {
        tmp_vert = tmp_vert->next;
        --idx;
    }
    return tmp_vert;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::edgeByNo( int idx ) const
{
    PEdge tmp_edge = first_edge;
    while (idx && tmp_edge)
    {
        tmp_edge = tmp_edge->next;
        --idx;
    }
    return tmp_edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::vertPos( PVertex vert ) const
{
    int idx = 0;
    PVertex tmp_vert = first_vert;
    while (tmp_vert && tmp_vert != vert)
    {
        tmp_vert = tmp_vert->next;
        ++idx;
    }
    return tmp_vert ? idx : -1;
}

template< class VertInfo,class EdgeInfo , class Settings>
int Graph< VertInfo,EdgeInfo,Settings >::edgePos( PEdge edge ) const {
    int idx = 0;
    PEdge tmp_edge = first_edge;
    while (tmp_edge && tmp_edge != edge)
    {
        tmp_edge = tmp_edge->next;
        ++idx;
    }
    return tmp_edge ? idx : -1;
}

template< class VertInfo, class EdgeInfo, class Settings >
bool Graph< VertInfo,EdgeInfo,Settings >::chEdgeType( PEdge edge, EdgeType type )
{
    if (!edge) return false;
    if (edge->type == type) return true;
    if (edge->type == Directed && type == Undirected)
    {
        PVertex vert1 = edge->vert[0].vert;
        PVertex vert2 = edge->vert[1].vert;
        if (!detach( edge )) return false;
        return (bool)attach_undir( edge,vert1,vert2 );
    }
    if (edge->type == Undirected && type == Directed)
    {
        PVertex vert1 = edge->vert[0].vert;
        PVertex vert2 = edge->vert[1].vert;
        if (!detach( edge )) return false;
        return (bool)attach_dir( edge,vert1,vert2 );
    }
    return false;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline bool Graph< VertInfo,EdgeInfo,Settings >::revertEdge( PEdge edge )
{
    if (edge->type == Directed)
    {
        PVertex vert_in = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].vert;
        PVertex vert_out = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].vert;
        if (!detach( edge )) return false;
        return (bool)attach_dir( edge,vert_in,vert_out );
    }
    return false;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::moveEdge(
    PEdge edge, PVertex vert1, PVertex vert2, EdgeDirection direct )
{
    return attach( edge,vert1,vert2,direct );
}

template< class VertInfo,class EdgeInfo , class Settings>
inline int Graph< VertInfo,EdgeInfo,Settings >::delVerts()
{
    int res = getVertNo();
    clear();
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings > template < class Iterator >
int Graph< VertInfo,EdgeInfo,Settings >::delVerts2( Iterator begin, Iterator end )
{
    int size = 0;
    for( Iterator iter = begin; iter != end; iter++ ) size++;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex LOCALARRAY( buf,size );
    size = 0;
    for( Iterator iter = begin; iter != end; iter++) buf[size++] = *iter;
    Settings::sort( buf,buf + size );
    size = std::unique( buf,buf + size ) - buf;
    return delVerts( buf,buf + size );
}

template< class VertInfo, class EdgeInfo, class Settings > template < class Iterator >
int Graph< VertInfo,EdgeInfo,Settings >::delVerts( Iterator begin, Iterator end )
{
    int res = 0;
    for( Iterator i = begin; i != end; i++)
        if (*i)
        {
            del( *i );
            res++;
        }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::delVerts(
    const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex> &s )
{
    return delVerts( s.begin(),s.end() );
}

template< class VertInfo,class EdgeInfo , class Settings>
int Graph< VertInfo,EdgeInfo,Settings >::delEdges( EdgeType direct )
{
    int res = 0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e = getEdge( direct ), enext;
    while (e)
    {
        enext = getEdgeNext( e,direct );
        del( e );
        res++;
        e = enext;
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::delEdges( PVertex vert, EdgeDirection direct )
{
    int res = 0;
    if (!vert) return 0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e = getEdge( vert,direct ), enext;
    while (e)
    {
        enext = getEdgeNext( vert,e,direct );
        del( e );
        res++;
        e = enext;
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph<VertInfo,EdgeInfo,Settings>::delEdges(
    PVertex vert1, PVertex vert2, EdgeDirection direct )
{
    int res = 0;
    if (!vert1 || !vert2) return 0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e = getEdge( vert1,vert2,direct );
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge enext;
    while (e)
    {
        enext = getEdgeNext( vert1,vert2,e,direct );
        del( e );
        res++;
        e = enext;
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class Iterator >
int Graph< VertInfo,EdgeInfo,Settings >::delEdges2(
    Iterator begin, Iterator end, EdgeType direct )
{
    int size = 0;
    for( Iterator iter = begin; iter != end; iter++ ) size++;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY( buf,size );
    size = 0;
    for( Iterator iter = begin; iter != end; iter++ ) buf[size++] = *iter;
    Settings::sort( buf,buf + size );
    size = std::unique( buf,buf + size ) - buf;
    return delEdges( buf,buf + size,direct );
}

template< class VertInfo,class EdgeInfo, class Settings > template< class Iterator >
int Graph< VertInfo,EdgeInfo,Settings >::delEdges(
    Iterator begin, Iterator end, EdgeType direct )
{
    int res = 0;
    for( Iterator i = begin; i != end; i++ )
        if (*i && (*i)->type & direct)
        {
            del( *i );
            res++;
        }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::delEdges(
    const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge > &s, EdgeType direct )
{
    return delEdges( s.begin(),s.end(),direct );
}


template< class VertInfo, class EdgeInfo, class Settings > template< class Iterator >
int Graph< VertInfo,EdgeInfo,Settings >::chEdgesType(
    Iterator begin, Iterator end, EdgeType type)
{
    int res = 0;
    for( Iterator i = begin; i != end; i++ )
        if (*i && getEdgeType(*i)!=type && chEdgeType(*i,type))
            res++;
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::chEdgesType(
    const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &s, EdgeType type)
{
    return chEdgesType( s.begin(),s.end(), type );
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::chEdgesType(EdgeType type)
{
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY( buf,getEdgeNo()-getEdgeNo(type) );
    int size=getEdges(buf,EdAll-type),good=getEdgeNo(type);
    return good+chEdgesType(buf,buf+size,type);
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::chEdgesType(PVertex vert,EdgeType type)
{
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY( buf,getEdgeNo(vert,EdAll & (~type)) );
    int size=getEdges(buf,vert,EdAll & (~type)),good=getEdgeNo(vert,type);
    return good+chEdgesType(buf,buf+size,type);
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::chEdgesType(
    PVertex vert1,PVertex vert2,EdgeType type)
{
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY( buf,std::min(getEdgeNo(vert1),getEdgeNo(vert2)) );
    int size=getEdges(buf,vert1,vert2);
    return chEdgesType(buf,buf+size,type);
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::ch2Archs( PEdge edge )
{
    if (!edge || edge->type != Undirected) return 0;
    std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,
        typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > ends = getEdgeEnds( edge );
    chEdgeType( edge,Directed );
    return addArch( ends.second,ends.first,edge->info );
}

template< class VertInfo, class EdgeInfo, class Settings > template < class Iterator >
inline int Graph< VertInfo,EdgeInfo,Settings >::ch2Archs( Iterator begin,Iterator end )
{
    int res = 0;
    for( Iterator iter = begin; iter != end; iter++ )
        if ((*iter) && ch2Archs( *iter )) res++;
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::ch2Archs(
    const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &s )
{
    return ch2Archs( s.begin(),s.end() );
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::ch2Archs()
{
    int res = getEdgeNo( Undirected );
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e = getEdge( Undirected ),enext;
    for( ; e; e = enext )
    {
        enext = getEdgeNext( e,Undirected );
        ch2Archs( e );
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::ch2Archs(PVertex v)
{
    int res = getEdgeNo(v, Undirected );
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e = getEdge(v, Undirected ),enext;
    for( ; e; e = enext )
    {
        enext = getEdgeNext(v, e,Undirected );
        ch2Archs( e );
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
bool Graph< VertInfo,EdgeInfo,Settings >::areParallel(
    PEdge e1, PEdge e2, EdgeDirection reltype ) const
{
    if(!(e1 && e2 && (reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir))) return false;
    std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,
        typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > ends1 = getEdgeEnds( e1 ),
        ends2 = getEdgeEnds( e2 );
    if (e1 == e2) return true;
    else if (e1->type == Loop) return e2->type == Loop && ends1.first == ends2.first;
    else if (e2->type == Loop) return false;
    else if ((e1->type == Undirected && e2->type == Undirected)
        || (e1->type != e2->type && reltype == EdUndir)
        || (e1->type == Directed && e2->type == Directed
        && (reltype == EdUndir || reltype == EdDirIn)))
        return (ends1.first == ends2.first && ends1.second == ends2.second)
                || (ends1.second == ends2.first && ends1.first == ends2.second);
    else return e1->type == e2->type && ends1.first == ends2.first
        && ends1.second == ends2.second;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class Iterator >
int Graph< VertInfo,EdgeInfo,Settings >::delParals2(
    Iterator begin, Iterator end, PEdge edge, EdgeDirection reltype)
{   if (!edge) return 0;
    int size = 0;
    for( Iterator iter = begin; iter != end; iter++ ) size++;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY( buf,size );
    size = 0;
    for( Iterator iter = begin; iter != end; iter++ )
        if (*iter) buf[size++] = *iter;
    Settings::sort( buf,buf + size );
    size = std::unique( buf,buf + size ) - buf;
    return delParals( buf,buf + size,edge,reltype );
}

template< class VertInfo, class EdgeInfo, class Settings > template< class Iterator >
int Graph< VertInfo,EdgeInfo,Settings >::delParals(
    Iterator begin, Iterator end, PEdge edge, EdgeDirection reltype)
{   if (!edge) return 0;
    int res = 0;
    for( Iterator i = begin; i != end; i++ )
        if (*i && *i!=edge && areParallel(*i,edge,reltype))
            { del(*i); res++; }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::delParals(
    const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &s, PEdge edge, EdgeDirection reltype)
{
    return delParals( s.begin(),s.end(), edge, reltype );
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::delParals(PEdge edge, EdgeDirection reltype)
{   if (!edge) return 0;
    std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex, typename Graph< VertInfo,EdgeInfo,Settings >::PVertex>
        ends=getEnds(edge);
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY( buf,std::min(getEdgeNo(ends.first),getEdgeNo(ends.second)) );
    int size=getEdges(buf,ends.first,ends.second);
    return delParals(buf,buf+size,edge,reltype);
}

template< class VertInfo, class EdgeInfo, class Settings > template <class Container>
int Graph< VertInfo,EdgeInfo,Settings >::delParals(PVertex vert, EdgeDirection reltype, Container & tab)
{   if (!vert || !(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir)) return 0;
    int licz=0;
    tab.clear();

    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e=getEdge(vert,EdLoop),f;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex u;

    if (e)
        while (f=getEdgeNext(vert,e,EdLoop))
            { del(f); licz++; }

    for(e=getEdge(vert,EdUndir|EdDirIn|EdDirOut);e;e=f)
    {
        u=getEdgeEnd(e,vert);
        f=getEdgeNext(vert,e,EdUndir|EdDirIn|EdDirOut);
        bool found=false;
        for(int i=0;i<tab[u].size;i++)
            if (found=areParallel(e,tab[u].tab[i],reltype))
                {
                    del(e); licz++; break;
                }
        if (!found) { tab[u].tab[tab[u].size++]=e; }
    }

    return licz;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::delParals(PVertex vert, EdgeDirection reltype)
{
    typename Settings:: template VertAssocCont
            <typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,Parals2>
            ::Type tab(getEdgeNo(vert,EdUndir|EdDirIn|EdDirOut));
    return delParals(vert,reltype,tab);
}


template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::delParals(PVertex vert1,PVertex vert2, EdgeDirection reltype)
{   if (!vert1 || !vert2 || !(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir)) return 0;
    int licz=0;
    if (vert1==vert2)
    {   PEdge e=getEdge(vert1,EdLoop),f;
        if (e)
            while (f=getEdgeNext(vert1,e,EdLoop))
                { del(f); licz++; }
        return licz;
    }
    for(EdgeDirection dir=EdUndir;dir<=EdDirOut;dir<<=1)
    {   PEdge e=getEdge(vert1,vert2,dir),f;
        if (e)
            while (f=getEdgeNext(vert1,vert2,e,dir))
                { del(f); licz++; }
    }
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
        eout=getEdge(vert1,vert2,EdDirOut),
        ein=getEdge(vert1,vert2,EdDirIn),
        eund=getEdge(vert1,vert2,EdUndir);
    if ((reltype==EdDirIn || reltype==EdUndir) && ein && eout)
        { del(eout); eout=0; licz++; }
    if (reltype==EdUndir && eund)
    {
        if (eout) { del(eout); licz++; }
        if (ein) { del(ein); licz++; }
    }
    return licz;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::delParals(EdgeDirection reltype)
{
    int licz=0;
    typename Settings:: template VertAssocCont
            <typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,Parals2>
            ::Type tab(Delta(EdUndir|EdDirIn|EdDirOut));

    for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex v=getVert();v;v=getVertNext(v))
        licz+=delParals(v,reltype,tab);
    return licz;
}

template< class VertInfo, class EdgeInfo, class Settings > template <class OutputIterator>
int Graph< VertInfo,EdgeInfo,Settings >::getParals(  OutputIterator iter, PEdge edge, EdgeDirection reltype) const
{
    if (!edge || !(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir)) return 0;
    int licz=0;
    std::pair<typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,typename Graph< VertInfo,EdgeInfo,Settings >::PVertex>
            ends=getEdgeEnds(edge);
    for(typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e=getEdge(ends.first,ends.second,EdAll);e;
                e=getEdgeNext(ends.first,ends.second,e,EdAll))
        if (e!=edge && areParallel(e,edge,reltype))
        {   *iter=e;
            ++iter;
            licz++;
        }
    return licz;
}

template< class VertInfo, class EdgeInfo, class Settings >
Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge >
Graph< VertInfo,EdgeInfo,Settings >::getParalsSet(PEdge edge, EdgeDirection reltype) const
{
    Set<typename Graph< VertInfo,EdgeInfo,Settings >::PEdge > res;
    getParals(setInserter(res),edge,reltype);
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::mu( PEdge edge, EdgeDirection reltype) const
{
    if (!(edge && (reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir))) return 0;
    return getParals(blackHole,edge,reltype)+1;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline int Graph< VertInfo,EdgeInfo,Settings >::mu( EdgeDirection reltype) const
{
    return maxMu(reltype).second;
}

template< class VertInfo, class EdgeInfo, class Settings >
std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge,int >
Graph< VertInfo,EdgeInfo,Settings >::maxMu( EdgeDirection reltype) const
{
    std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge,int > res(0,0);
    if (!(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir) || !getEdgeNo(EdAll)) return res;
    Parals3 LOCALARRAY(edges,getEdgeNo(EdAll));
    int i=0,l=0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge edge;
    for (typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e=getEdge(EdAll); e; e=getEdgeNext(e,EdAll) )
        edges[i++]=Parals3(std::min(e->getEnd1(),e->getEnd2()),
                           std::max(e->getEnd1(),e->getEnd2()),
                           getEdgeDir(e,std::min(e->getEnd1(),e->getEnd2())),e);
    Settings::sort(edges,edges+i,Parals3cmp());
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


template< class VertInfo, class EdgeInfo, class Settings > template <class Iterator, class OutIter>
int Graph< VertInfo,EdgeInfo,Settings >::getIndEdges(OutIter out,Iterator beg, Iterator end, EdgeType type ) const
{   int licze=0;
    typename Settings:: template VertAssocCont
            <typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,bool>::Type vset(getVertNo());
    for(Iterator i=beg;i!=end;++i ) vset[*i]=true;
    for(typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e=getEdge(type );e;e=getEdgeNext(e,type ))
        if (vset.hasKey(getEdgeEnd1(e)) && vset.hasKey(getEdgeEnd2(e)))
            { *out=e; ++out; ++ licze; }
    return licze;
}

template< class VertInfo, class EdgeInfo, class Settings > template <class Iterator>
Set<typename Graph< VertInfo,EdgeInfo,Settings >::PEdge>
Graph< VertInfo,EdgeInfo,Settings >::getIndEdgeSet(Iterator beg, Iterator end, EdgeType type ) const
{
    Set<typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> res;
    getIndEdges(setInserter(res),beg,end,type);
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings > template <class OutIter>
int Graph< VertInfo,EdgeInfo,Settings >::getIndEdges(OutIter out,const Set<typename Graph< VertInfo,EdgeInfo,Settings >::PVertex>& vset, EdgeType type ) const
{
    return getIndEdges(out,vset.begin(),vset.end(),type);
}

template< class VertInfo, class EdgeInfo, class Settings >
Set<typename Graph< VertInfo,EdgeInfo,Settings >::PEdge>
Graph< VertInfo,EdgeInfo,Settings >::getIndEdgeSet(const Set<typename Graph< VertInfo,EdgeInfo,Settings >::PVertex>& vset, EdgeType type ) const
{
    Set<typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> res;
    getIndEdges(setInserter(res),vset.begin(),vset.end(),type);
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings > template <class Iterator>
int Graph< VertInfo,EdgeInfo,Settings >::delIndEdges(Iterator beg, Iterator end, EdgeType type )
{   typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY(tab,getEdgeNo(type));
    return delEdges(tab,tab+getIndEdges(tab,beg,end,type));
}

template< class VertInfo, class EdgeInfo, class Settings >
int Graph< VertInfo,EdgeInfo,Settings >::delIndEdges(const Set<typename Graph< VertInfo,EdgeInfo,Settings >::PVertex>& vset, EdgeType type )
{   typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY(tab,getEdgeNo(type));
    return delEdges(tab,tab+getIndEdges(tab,vset,type));
}


template< class VertInfo, class EdgeInfo, class Settings >
template <class Iterator,class EdInfoGen>
void Graph< VertInfo,EdgeInfo,Settings >::neg(Iterator beg, Iterator end, EdgeType type, EdInfoGen infoGen)
{
    typename Settings:: template VertAssocCont
            <typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,bool>::Type vset(getVertNo());
    for(Iterator i=beg;i!=end;++i ) vset[*i]=true;
    for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex u=vset.firstKey();u;u=vset.nextKey(u))
    {   typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e;
        if (type&EdLoop)
        {   if (getEdge(u,EdLoop))
            { while (e=getEdge(u,EdLoop)) delEdge(e); }
            else addLoop(u,infoGen(*this,u,u,EdLoop));
        }
    }
    typename Settings:: template AdjMatrixDirEdges<typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,char >::Type
        dirs((type & Directed) ? vset.size() : 0);
    typename Settings:: template AdjMatrixUndirEdges <typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,char >::Type
        undirs((type & Undirected) ? vset.size() : 0);
    int elicz=0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge LOCALARRAY( ebuf,getEdgeNo(type & (~EdLoop)));
    for(typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e=getEdge(type & (~EdLoop));e;e=getEdgeNext(e,type & (~EdLoop)))
        if (vset.hasKey(getEdgeEnd1(e)) && vset.hasKey(getEdgeEnd2(e)))
        {   ebuf[elicz++]=e;
            if ((type&Directed) && getType(e)==Directed) dirs(getEdgeEnd1(e),getEdgeEnd2(e))=1;
            if ((type&Undirected) && getType(e)==Undirected) undirs(getEdgeEnd1(e),getEdgeEnd2(e))=1;
        }
    for(int i=0;i<elicz;i++) delEdge(ebuf[i]);
    if (type&Directed)
        for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex u=vset.firstKey();u;u=vset.nextKey(u))
            for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex v=vset.firstKey();v;v=vset.nextKey(v))
                if (u!=v && !dirs.hasKey(u,v)) addArch(u,v,infoGen(*this,u,v,EdDirOut));
    if (type&Undirected)
        for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex u=vset.firstKey();u!=vset.lastKey();u=vset.nextKey(u))
            for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex v=vset.nextKey(u);v;v=vset.nextKey(v))
                if (!undirs.hasKey(u,v)) addEdge(u,v,infoGen(*this,u,v,EdUndir));
}

template< class VertInfo, class EdgeInfo, class Settings > template <class Iterator>
void Graph< VertInfo,EdgeInfo,Settings >::neg(Iterator beg, Iterator end, EdgeType type,EdgeInfo info)
{   neg(beg,end,type,ConstFunctor<EdgeInfo>(info)); }

template< class VertInfo, class EdgeInfo, class Settings > template <class EdInfoGen>
void Graph< VertInfo,EdgeInfo,Settings >::neg(const Set<PVertex>& vset, EdgeType type,EdInfoGen infoGen)
{   neg(vset.begin(),vset.end(),type,infoGen);  }

template< class VertInfo, class EdgeInfo, class Settings >
void Graph< VertInfo,EdgeInfo,Settings >::neg(const Set<PVertex>& vset, EdgeType type,EdgeInfo info)
{   neg(vset.begin(),vset.end(),type,ConstFunctor<EdgeInfo>(info));  }

template< class VertInfo, class EdgeInfo, class Settings > template <class EdInfoGen>
void Graph< VertInfo,EdgeInfo,Settings >::neg(EdgeType type, EdInfoGen infoGen)
{
    if (type&EdLoop) for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex v=getVert();v;v=getVertNext(v))
    {   typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e;
        if (getEdge(v,EdLoop))
        { while (e=getEdge(v,EdLoop)) delEdge(e); }
        else addLoop(v,infoGen(*this,v,v,EdLoop));
    }
    typename Settings:: template AdjMatrixDirEdges<typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,char >::Type
        dirs((type & Directed) ? getVertNo() : 0);
    typename Settings:: template AdjMatrixUndirEdges <typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,char >::Type
        undirs((type & Undirected) ? getVertNo() : 0);
    for(typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e=getEdge(type & (~EdLoop));e;e=getEdgeNext(e,type & (~EdLoop)))
    {
        if ((type&Directed) && getType(e)==Directed) dirs(getEdgeEnd1(e),getEdgeEnd2(e))=1;
        if ((type&Undirected) && getType(e)==Undirected) undirs(getEdgeEnd1(e),getEdgeEnd2(e))=1;
    }
    delEdges(type & (~EdLoop));
    if (type&Directed)
        for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex u=getVert();u;u=getVertNext(u))
            for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex v=getVert();v;v=getVertNext(v))
                if (u!=v && !dirs.hasKey(u,v)) addArch(u,v,infoGen(*this,u,v,EdDirOut));
    if (type&Undirected)
        for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex u=getVert();u!=getVertLast();u=getVertNext(u))
            for(typename Graph< VertInfo,EdgeInfo,Settings >::PVertex v=getVertNext(u);v;v=getVertNext(v))
                if (!undirs.hasKey(u,v)) addEdge(u,v,infoGen(*this,u,v,EdUndir));
}

template< class VertInfo, class EdgeInfo, class Settings >
void Graph< VertInfo,EdgeInfo,Settings >::neg(EdgeType type, EdgeInfo info)
{
    neg(type,ConstFunctor<EdgeInfo>());
}


template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::putVert( PEdge edge, const VertInfo &info )
{
    if (!edge) return 0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex res = addVert( info );
    std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,
        typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > ends = getEdgeEnds( edge );
    EdgeType type = (getEdgeType(edge) == Directed) ? Directed : Undirected;
    addEdge( ends.first,res,edge->info,type );
    addEdge( res,ends.second,edge->info,type );
    del( edge );
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::pickVert( PVertex vert, const EdgeInfo & info )
{
    if (!vert) return 0;
    int no;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
        LOCALARRAY( neig,getEdgeNo( vert,EdDirIn | EdDirOut | EdUndir ) );
    no = getNeigh( neig,vert,EdDirIn | EdDirOut | EdUndir );
    if (!(no == 0 || no == 1 || no == 2)) return 0;
    if (no == 0)
    {
        del( vert );
        return 0;
    }
    if (no == 1)
    {
        del( vert );
        return addLoop( neig[0],info );
    }
    int delta = (getEdgeNo( vert,neig[0],EdDirOut )
        - getEdgeNo( vert,neig[0],EdDirIn) ) - (getEdgeNo( vert,neig[1],EdDirOut )
        - getEdgeNo( vert,neig[1],EdDirIn ));
    del( vert );
    return addEdge( neig[1],neig[0],info,
        (delta == 0) ? EdUndir : ((delta > 0) ? EdDirOut : EdDirIn));
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::glue( PVertex vert1, PVertex vert2, bool makeloops )
{
    if (!vert1) return vert2;
    if (!vert2) return vert1;
    if (vert1 == vert2) return vert1;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex args[2] = { vert1,vert2 };
    return glue( args,args + 2,makeloops,vert1 );
}

template< class VertInfo, class EdgeInfo, class Settings > template< class Iterator >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::glue2(
    Iterator begin, Iterator end, bool makeloops, PVertex res )
{
    int size = 0;
    for( Iterator iter = begin; iter != end; iter++ ) size++;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex LOCALARRAY( buf,size );
    size = 0;
    for( Iterator iter = begin; iter != end; iter++ ) buf[size++] = *iter;
    Settings::sort( buf,buf + size );
    size = std::unique( buf,buf + size ) - buf;
    return glue( buf,buf + size,makeloops,res );
}

template< class VertInfo, class EdgeInfo, class Settings > template< class Iterator >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::glue(
    Iterator begin, Iterator end, bool makeloops, PVertex res )
{
    bool present = false;
    for( Iterator iter = begin; iter != end; iter++ )
    {
        if (!res && *iter != 0)
        {
            res = *iter;
            present = true;
        }
        if (!present && res && res == *iter) present = true;
    }
    if (!present) return 0;
    for( Iterator i = begin; i != end; i++ )
        if (res != (*i) && (*i) != 0)
        {
            typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e, enext;
            for( e = getEdge( (*i),EdLoop ); e; e = enext )
            {
                enext = getEdgeNext( (*i),e,EdLoop );
                moveEdge( e,res,res,EdLoop );
            }
            for( e = getEdge( (*i),EdDirIn | EdDirOut | EdUndir ); e; e = enext )
            {
                enext = getEdgeNext( (*i),e,EdDirIn | EdDirOut | EdUndir );
                std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,
                    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex >
                    ends = getEdgeEnds( e );
                if (ends.first == (*i)) ends.first = res;
                if (ends.second == (*i)) ends.second = res;
                if (ends.first == ends.second)
                    if (makeloops) moveEdge( e,res,res,EdLoop );
                    else del( e );
                else moveEdge( e,ends.first,ends.second,
                    e->type == Undirected ? EdUndir : EdDirOut );
            }
            del( (*i) );
        }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::glue(
    const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex> &s,
    bool makeloops, PVertex res )
{
    glue( s.begin(),s.end(),makeloops,res );
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::move( Graph< VertInfo,EdgeInfo,Settings > &graph )
{
    if (&graph == this) return 0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex res = getVertLast();
    if (graph.pAdj) graph.pAdj->clear();
    if (pAdj) {
        typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e = graph.first_edge;
        for( ; e; e = e->next) pAdj->add( e );
    }
    if (this->last_vert)
    {
        if (graph.first_vert)
        {
            this->last_vert->next = graph.first_vert;
            graph.first_vert->prev = this->last_vert;
            this->last_vert = graph.last_vert;
        }
    }
    else
    {
        this->first_vert = graph.first_vert;
        this->last_vert = graph.last_vert;
    }
    if (this->last_edge)
    {
        if (graph.first_edge)
        {
            this->last_edge->next = graph.first_edge;
            graph.first_edge->prev = this->last_edge;
            this->last_edge = graph.last_edge;
        }
    }
    else
    {
        this->first_edge = graph.first_edge;
        this->last_edge = graph.last_edge;
    }
    this->no_vert += graph.no_vert;
    this->no_dir_edge() += graph.no_dir_edge();
    this->no_loop_edge() += graph.no_loop_edge();
    this->no_undir_edge() += graph.no_undir_edge();
    graph.first_vert = graph.last_vert = NULL;
    graph.first_edge = graph.last_edge = NULL;
    graph.no_vert = graph.no_dir_edge() = graph.no_loop_edge() = graph.no_undir_edge() = 0;
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class ExtGraph >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::copy( const ExtGraph &agraph )
{
    ExtGraph &graph = const_cast< ExtGraph & >( agraph );
    return copy( graph,std::make_pair( stdChoose( true ),stdChoose( true ) ),
             std::make_pair( stdCast( false ),stdCast( false ) ),
             std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
}

template< class VertInfo, class EdgeInfo, class Settings >
template< class ExtGraph, class VChooser, class EChooser >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::copy(
    const ExtGraph &agraph,
    std::pair< VChooser,EChooser > choosers )
{
    ExtGraph &graph = const_cast< ExtGraph & >( agraph );
    return copy( graph,choosers,std::make_pair( stdCast( false ),stdCast( false ) ),
             std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
}

template< class VertInfo, class EdgeInfo, class Settings >
template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::copy(
    const ExtGraph &agraph,
    std::pair< VChooser,EChooser > choosers, std::pair< VCaster,ECaster > casters )
{
    ExtGraph &graph= const_cast< ExtGraph & >( agraph );
    return copy( graph,choosers,casters,std::make_pair( stdLink( false,false ),
        stdLink( false,false ) ) );
}

template< class VertInfo, class EdgeInfo, class Settings >
template< class ExtGraph, class VChooser, class EChooser, class VCaster,
    class ECaster, class VLinker, class ELinker >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::copy(
    ExtGraph &graph,std::pair< VChooser,EChooser > choosers,
    std::pair< VCaster,ECaster > casters, std::pair< VLinker,ELinker > linkers )
{
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex res = getVertLast();
    if ((void*)&graph == (void*)this)
    {
        Graph< VertInfo,EdgeInfo,Settings > gr;
        gr.copy( graph,choosers,casters,linkers );
        return this->move( gr );
    }
    typedef typename ExtGraph::PVertex NPVertex;
    typedef typename ExtGraph::PEdge NPEdge;

    typename Settings:: template ExtVertAssocCont <NPVertex,PVertex>
            ::Type  ptr( graph.getVertNo() );

    NPVertex vert = graph.getVert();
    VertInfo vertI;
    while (vert)
    {
        if (choosers.first( vert,graph ))
        {
            casters.first( vertI,graph.getVertInfo( vert ) );
            ptr[vert] = addVert( vertI );
            linkers.first( ptr[vert],vert );
        }
        else
        {
            ptr[vert] = NULL;
            linkers.first( (PVertex)NULL,vert );
        }
        vert = graph.getVertNext( vert );
    }
    NPEdge edge = graph.getEdge();
    EdgeInfo edgeI;
    while (edge)
    {
        std::pair< NPVertex,NPVertex > verts = graph.getEdgeEnds( edge );
        if (ptr[verts.first] && ptr[verts.second] && choosers.second( edge,graph ))
        {
            casters.second( edgeI,graph.getEdgeInfo( edge ) );
            PEdge tmp_edge = this->addEdge( ptr[verts.first],ptr[verts.second],
                                edgeI,graph.getEdgeType( edge ) );
            linkers.second(tmp_edge,edge);
        }
        else linkers.second( (PEdge)NULL,edge );
        edge = graph.getEdgeNext( edge );
    }
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings > template< class ExtGraph >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::substitute( PVertex vert, ExtGraph &graph )
{
    return substitute( vert,graph,std::make_pair( stdChoose( true ),
             stdChoose( true ) ),std::make_pair( stdCast( false ),stdCast( false ) ),
             std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
}

template< class VertInfo, class EdgeInfo, class Settings >
template< class ExtGraph, class VChooser, class EChooser >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::substitute(
    PVertex vert, ExtGraph &graph, std::pair< VChooser,EChooser > choosers )
{
    return substitute( vert,graph,choosers,std::make_pair( stdCast( false ),
             stdCast( false ) ),std::make_pair( stdLink( false,false ),
             stdLink( false,false ) ) );
}

template< class VertInfo, class EdgeInfo, class Settings >
template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::substitute(
    PVertex vert, ExtGraph &graph,
    std::pair< VChooser,EChooser > choosers, std::pair< VCaster,ECaster > casters )
{
    return substitute( vert,graph,choosers,casters,
            std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
}

template< class VertInfo, class EdgeInfo, class Settings >
template< class ExtGraph, class VChooser, class EChooser, class VCaster,
    class ECaster, class VLinker, class ELinker >
typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::substitute(
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex vert,ExtGraph &graph,
    std::pair< VChooser,EChooser > choosers, std::pair< VCaster,ECaster > casters,
    std::pair< VLinker,ELinker > linkers )
{
    if (!vert || (void*)this == (void*)&graph) return 0;
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
        res = this->copy( graph,choosers,casters,linkers );
    typename Graph< VertInfo,EdgeInfo,Settings >::PVertex v = getVertNext( res );
    for( ; v; v = getVertNext( v ) )
    {
        typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
            e = getEdge( vert,EdDirIn | EdDirOut | EdUndir );
        for( ; e; e = getEdgeNext( vert,e,EdDirIn | EdDirOut | EdUndir ) )
        {
            std::pair< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex,
                typename Graph< VertInfo,EdgeInfo,Settings >::PVertex> ends;
            ends = getEdgeEnds( e );
            if (ends.first == vert) ends.first = v;
            else ends.second = v;
            addEdge( ends.first,ends.second,e->info,getEdgeType( e ) );
        }
    }
    if (res == vert) res = getVertPrev( res );
    del( vert );
    return res;
}

template< class VertInfo, class EdgeInfo, class Settings >
Graph< VertInfo,EdgeInfo,Settings > &Graph< VertInfo,EdgeInfo,Settings >::operator=(
    const Graph< VertInfo,EdgeInfo,Settings > &gr )
{
    if (&gr != this) {
        clear();
        copy( gr );
    }
    return *this;
}


template< class VertInfo, class EdgeInfo, class Settings >
Graph<VertInfo,EdgeInfo,Settings> &Graph< VertInfo,EdgeInfo,Settings >::operator+=(
    const Graph< VertInfo,EdgeInfo,Settings > &gr )
{
    copy(gr);
    return *this;
}

template< class VertInfo, class EdgeInfo, class Settings >
bool Graph< VertInfo,EdgeInfo,Settings >::makeAdjMatrix()
{   if ((Settings::AdjMatrixAllowed)==0) return false;
    if (pAdj) return false;
    pAdj = new AdjMatrix< VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed >( this->no_vert );
    typename Graph< VertInfo,EdgeInfo,Settings >::PEdge edge = first_edge;
    for( ; edge; edge = edge->next ) pAdj->add( edge );
    return true;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline bool Graph< VertInfo,EdgeInfo,Settings >::hasAdjMatrix() const
{
    return pAdj;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline bool Graph< VertInfo,EdgeInfo,Settings >::delAdjMatrix()
{
    if (!pAdj) return false;
    delete pAdj;
    pAdj = 0;
    return true;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline bool Graph< VertInfo,EdgeInfo,Settings >::defragAdjMatrix()
{
    if (!pAdj) return false;
    pAdj->defrag();
    return true;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::attach( PVertex vert )
{
    if (!vert) return NULL;
//    if (vert->edgesd(Graph< VertInfo,EdgeInfo,Settings >::Vertex::undir) != -1)
//        return NULL;
//    for( int i = 0; i < 4; i++)

//        vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings>::getDegree() =
//        vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&EdAllow>::getDegree() =
//        vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,EdAllow,EdDirIn&EdAllow>::getDegree() =
//        vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,EdAllow,EdDirOut&EdAllow>::getDegree() = 0;
    vert->prev = last_vert;
    vert->next = NULL;
    if (last_vert) last_vert->next = vert;
    else first_vert = vert;
    last_vert = vert;
    ++no_vert;
    return vert;
}

template< class VertInfo, class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PVertex
Graph< VertInfo,EdgeInfo,Settings >::detach( PVertex vert )
{
    if (!vert) return NULL;
    if (vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getDegree() != 0 ||
        vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree() != 0 ||
        vert->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getDegree() != 0 ||
        vert->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getDegree() != 0)
        return NULL;
//    vert->edgesd(Graph< VertInfo,EdgeInfo,Settings >::Vertex::undir) = -1;
    if (vert->next) vert->next->prev = vert->prev;
    else last_vert = vert->prev;
    if (vert->prev) vert->prev->next = vert->next;
    else first_vert = vert->next;
    vert->next = vert->prev = NULL;
    --no_vert;
    if (pAdj) pAdj->delVert(vert);
    return vert;
}

template< class VertInfo,class EdgeInfo, class Settings >
inline typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::attach(
    PEdge edge, PVertex vert1, PVertex vert2, EdgeDirection direct )
{
    switch(direct)
    {
        case EdDirIn:
            return attach_dir( edge,vert2,vert1 );
        case EdDirOut:
        case EdDirIn | EdDirOut:
            return attach_dir( edge,vert1,vert2 );
        case EdUndir:
            return attach_undir( edge,vert1,vert2 );
        case Loop:
            if (vert1 == vert2 || !vert2) return attach_loop( edge,vert1 );
            return NULL;
    }
    return NULL;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::attach_undir( PEdge edge, PVertex vertU, PVertex vertV )
{   assert(Settings::EdAllow & EdUndir);
    if (!edge) return NULL;
    if (!vertU) return NULL;
    if (!vertV) return NULL;
    if (vertU == vertV) return NULL;
    if (edge->type != Detached) detach( edge );
//    if (vertU>vertV)
//    {
//            PVertex tmpvert=vertU; vertU=vertV; vertV=tmpvert;
//    }
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert = vertU;
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].vert = vertV;
    edge->type = Undirected;
    edge->prev = last_edge;
    edge->next = NULL;
    if (last_edge) last_edge->next = edge;
    else first_edge = edge;
    last_edge = edge;
    ++no_undir_edge();
    if (pAdj) pAdj->add( edge );
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].prev =
        vertU->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next = NULL;
    if (vertU->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast())
    {
        PEdge tmp_edge = vertU->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast();
        if (tmp_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert == vertU)
            tmp_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next = edge;
        else tmp_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next = edge;
    }
    else vertU->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getFirst() = edge;
    vertU->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast() = edge;
    ++vertU->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree();

    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].prev =
        vertV->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next = NULL;
    if (vertV->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast())
    {
        PEdge tmp_edge = vertV->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast();
        if (tmp_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].vert == vertV)
            tmp_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next = edge;
        else tmp_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next = edge;
    }
    else vertV->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getFirst() = edge;
    vertV->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast() = edge;
    ++vertV->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree();
    return edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::attach_dir( PEdge edge, PVertex vert_out, PVertex vert_in )
{   assert(Settings::EdAllow & EdDirOut);
    if (!edge) return NULL;
    if (!vert_out) return NULL;
    if (!vert_in) return NULL;
    if (vert_out == vert_in) return NULL;
    if (edge->type != Detached) detach( edge );
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].vert = vert_out;
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].vert = vert_in;
    edge->type = Directed;
    edge->prev = last_edge;
    edge->next = NULL;
    if (last_edge) last_edge->next = edge;
    else first_edge = edge;
    last_edge = edge;
    ++no_dir_edge();
    if (pAdj) pAdj->add( edge );

    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].prev =
        vert_out->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getLast();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].next = NULL;
    if (vert_out->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getLast())
        vert_out->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getLast()
            ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].next = edge;
    else vert_out->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getFirst() = edge;
    vert_out->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getLast() = edge;
    ++vert_out->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getDegree();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].prev =
        vert_in->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getLast();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].next = NULL;
    if (vert_in->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getLast())
        vert_in->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getLast()
            ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].next = edge;
    else vert_in->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getFirst() = edge;
    vert_in->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getLast() = edge;
    ++vert_in->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getDegree();
    return edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::attach_loop( PEdge edge, PVertex vert )
{   assert(Settings::EdAllow & EdLoop);
    if (!edge) return NULL;
    if (!vert) return NULL;
    if (edge->type != Detached) detach( edge );
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].vert = vert;
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_Nloop].vert = vert;
    edge->type = Loop;
    edge->prev = last_edge;
    edge->next = NULL;
    if (last_edge) last_edge->next = edge;
    else first_edge = edge;
    last_edge = edge;
    ++no_loop_edge();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].prev =
        vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getLast();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].next = NULL;
    if(vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getLast())
        vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getLast()
            ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].next = edge;
    else vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getFirst() = edge;
    vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getLast() = edge;
    ++vert->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getDegree();
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_Nloop].next = NULL;
    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_Nloop].prev = NULL;
    return edge;
}

template< class VertInfo, class EdgeInfo, class Settings >
typename Graph< VertInfo,EdgeInfo,Settings >::PEdge
Graph< VertInfo,EdgeInfo,Settings >::detach( PEdge edge )
{
    if (!edge) return NULL;
    if (edge->type == Detached) return NULL;
    switch (edge->type)
    {
        case Loop:
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].next)
                edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].next
                        ->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_loop].prev
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_loop].prev;
            else edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].vert
                        ->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getLast()
                    = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].prev;
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].prev)
                edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].prev
                        ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].next
                    = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].next;
            else edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].vert
                        ->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getFirst()
                    = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].next;
            --edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_loop].vert
                        ->Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>::getDegree();
            --no_loop_edge();
            break;
        case Directed:
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].next)
                edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].next
                        ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].prev
                    = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].prev;
            else edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].vert
                        ->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getLast()
                    = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].prev;
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].prev)
                edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].prev
                        ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].next
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_out].next;
            else edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].vert
                        ->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getFirst()
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_out].next;
            --edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].vert
                        ->Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>::getDegree();
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].next)
                edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].next
                            ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].prev
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_in].prev;
            else edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].vert
                        ->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getLast()
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_in].prev;
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].prev)
                edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].prev
                        ->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].next
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_in].next;
            else edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].vert
                        ->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getFirst()
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_in].next;
            --edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].vert
                        ->Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>::getDegree();
            --no_dir_edge();
            if (pAdj)
            {
                Privates::AdjMatrixParals<VertInfo,EdgeInfo,Settings> &pole
                                    = pAdj->vald( edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_out].vert,
                                    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_in].vert );
                if (edge->nParal()) edge->nParal()->pParal() = edge->pParal();
                else pole.last = edge->pParal();
                if(edge->pParal()) edge->pParal()->nParal() = edge->nParal();
                else pole.first = edge->nParal();
                pole.degree--;
            }
            break;
        case Undirected:
        {
            PVertex vert = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert;
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next)
            {
                PEdge next_edge =
                    edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next;
                if (next_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert == vert)
                    next_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].prev
                        = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_U].prev;
                else next_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].prev
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_U].prev;
            }
            else vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast()
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_U].prev;
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].prev)
            {
                PEdge prev_edge = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].prev;
                if (prev_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert == vert)
                    prev_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next
                        = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next;
                else prev_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next
                        = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next;
            }
            else vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getFirst()
                    = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next;
            --vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree();
            vert = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].vert;
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next)
            {
                PEdge next_edge = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next;
                if (next_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert == vert)
                    next_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].prev
                        = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].prev;
                else next_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].prev
                        = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].prev;
            }
            else vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getLast()
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_V].prev;
            if (edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].prev)
            {
                PEdge prev_edge = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].prev;
                if (prev_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert == vert)
                    prev_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].next
                        = edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next;
                else prev_edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].next
                        = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_V].next;
            }
            else vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getFirst()
                    = edge->vert[Graph<VertInfo,EdgeInfo,Settings>::Edge::V_V].next;
            --vert->Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>::getDegree();
            --no_undir_edge();
            if (pAdj)
            {
                Privates::AdjMatrixParals<VertInfo,EdgeInfo,Settings> &pole
                                    = pAdj->valund( edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_V].vert,
                                   edge->vert[Graph< VertInfo,EdgeInfo,Settings >::Edge::V_U].vert );
                if (edge->nParal()) edge->nParal()->pParal() = edge->pParal();
                else pole.last = edge->pParal();
                if (edge->pParal()) edge->pParal()->nParal() = edge->nParal();
                else pole.first = edge->nParal();
                pole.degree--;
            }
        }
            break;
        default:
            return NULL;
    }
    if (edge->next) edge->next->prev = edge->prev;
    else last_edge = edge->prev;
    if (edge->prev) edge->prev->next = edge->next;
    else first_edge = edge->next;
    edge->type=Detached;
    return edge;
}
