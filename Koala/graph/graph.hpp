// Koala_Graph constructor and destructor
template<class VertInfo, class EdgeInfo>
Graph<VertInfo,EdgeInfo>::Graph(): first_vert(NULL), last_vert(NULL),
		first_edge(NULL), last_edge(NULL),
		no_vert(0), no_loop_edge(0), no_dir_edge(0), no_undir_edge(0) {}

template<class VertInfo, class EdgeInfo>
Graph<VertInfo,EdgeInfo>::Graph(Graph &graph)
	:first_vert(NULL), last_vert(NULL),
	first_edge(NULL), last_edge(NULL),
	no_vert(0), no_loop_edge(0), no_dir_edge(0), no_undir_edge(0)
{
	this->copy(graph);
}

template<class VertInfo, class EdgeInfo>
Graph<VertInfo,EdgeInfo>::~Graph() {
	this->clear();
}
//----------------------------------------------------
//------methods for adding vertices------
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::addVert() {
	PVertex tmp_vert = new Vertex();
	if( !attach(tmp_vert) ) {
		delete tmp_vert;
		return NULL;
	}
	return tmp_vert;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::addVert(const VertInfo &infoExt) {
	PVertex tmp_vert = new Vertex(infoExt);
	if( !attach(tmp_vert) ) {
		delete tmp_vert;
		return NULL;
	}
	return tmp_vert;
}
//-----------------delete vertex------------------
template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::delVert(PVertex vert, bool force) {
	if(!vert)
		return;
	if(force) {
		PEdge edge = getEdge(vert);
		while(edge) {
			detach(edge);
			delete edge;
			edge = getEdge(vert);
		}
	}
	if(detach(vert)) {
		delete vert;
	}
}

template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::del(PVertex vert, bool force)
{
    this->delVert(vert,force);
}
//-------------------vertex motions----------------------
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::getVert() {
	return this->first_vert;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::getVertLast() {
	return this->last_vert;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::getVertNext(PVertex vert) {
	if(!vert)
		return this->first_vert;
	return vert->next;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::getVertPrev(PVertex vert) {
	if(!vert)
		return this->last_vert;
	return vert->prev;
}

//sets
template<class VertInfo, class EdgeInfo>
Set<typename Graph<VertInfo,EdgeInfo>::PVertex >
Graph<VertInfo,EdgeInfo>::getVertSet() {
	Set< typename Graph<VertInfo,EdgeInfo>::PVertex > ans;
	PVertex vert = this->first_vert;
	while(vert) {
		ans.add(vert);
		vert = vert->next;
	}
	return ans;
}

template<class VertInfo, class EdgeInfo>
template<class Chooser>
Set<typename Graph<VertInfo,EdgeInfo>::PVertex >
Graph<VertInfo,EdgeInfo>::getVertSet(Chooser chooser) {
	Set< typename Graph<VertInfo,EdgeInfo>::PVertex > ans;
	PVertex vert = this->first_vert;
	while(vert) {
		if( chooser(vert,*this) ) {
			ans.add(vert);
		}
		vert = vert->next;
	}
	return ans;
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator>
int
Graph<VertInfo,EdgeInfo>::getVerts(OutputIterator out)
{
	int ans = 0;
	PVertex vert = this->first_vert;
	while(vert) {
		*out = vert;
		++out;
		++ans;
		vert = vert->next;
	}
	return ans;
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator, class Chooser>
int
Graph<VertInfo,EdgeInfo>::getVerts(OutputIterator out,
		Chooser chooser)
{
	int ans = 0;
	PVertex vert = this->first_vert;
	while(vert) {
		if( chooser(vert,*this) ) {
			*out = vert;
			++out;
			++ans;
		}
		vert = vert->next;
	}
	return ans;
}

//other
template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::getVertNo() {
	return this->no_vert;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::VertInfoType
Graph<VertInfo,EdgeInfo>::getVertInfo(PVertex vert)
{
    return vert->info;
}

template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::setVertInfo(PVertex vert,const VertInfo& info)
{
    vert->info=info;
}

//-----------------edges----------------------
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::addEdge(PVertex vert1, PVertex vert2,
		EdgeDirection direct)
{
	PEdge tmp_edge = new Edge();
	if( !attach(tmp_edge,vert1,vert2,direct) ) {
		delete tmp_edge;
		return NULL;
	}
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::addEdge(PVertex vert1, PVertex vert2,
		const EdgeInfo &infoExt, EdgeDirection direct)
{
	PEdge tmp_edge = new Edge(infoExt);
	if( !attach(tmp_edge,vert1,vert2,direct) ) {
		delete tmp_edge;
		return NULL;
	}
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::addArch(PVertex v_out, PVertex v_in) {
	PEdge tmp_edge = new Edge();
	if( !attach_dir(tmp_edge, v_out, v_in) ) {
		delete tmp_edge;
		return NULL;
	}
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::addArch(PVertex v_out, PVertex v_in,
		const EdgeInfo &infoExt)
{
	PEdge tmp_edge = new Edge(infoExt);
	if( !attach_dir(tmp_edge, v_out, v_in) ) {
		delete tmp_edge;
		return NULL;
	}
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::addLoop(PVertex vert) {
	PEdge tmp_edge = new Edge();
	if( !attach_loop(tmp_edge, vert) ) {
		delete tmp_edge;
		return NULL;
	}
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::addLoop(PVertex vert,
		const EdgeInfo &infoExt)
{
	PEdge tmp_edge = new Edge(infoExt);
	if( !attach_loop(tmp_edge, vert) ) {
		delete tmp_edge;
		return NULL;
	}
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::EdgeInfoType
Graph<VertInfo,EdgeInfo>::getEdgeInfo(PEdge edge)
{
    return edge->info;
}

template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::setEdgeInfo(PEdge edge,const EdgeInfo& info)
{
    edge->info=info;
}

template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::del(PEdge edge) {
	if(detach(edge))
		delete edge;
}

template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::delEdge(PEdge edge)
{
	this->del(edge);
}
//--------------------edge motion--------------------
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdge(EdgeDirection direct) {
	PEdge edge = this->first_edge;
	while(edge
	&&!((int)edge->type&direct) )
		edge = edge->next;
	return edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgeLast(EdgeDirection direct) {
	PEdge tmp_edge = this->last_edge;
	while(tmp_edge
	&&!((int)tmp_edge->type&direct) )
		tmp_edge = tmp_edge->prev;
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgeNext(PEdge edge,
		EdgeDirection direct)
{
	PEdge tmp_edge = edge ? edge->next : this->first_edge;
	while(tmp_edge
	&&!((int)tmp_edge->type&direct) )
		tmp_edge = tmp_edge->next;
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgePrev(PEdge edge,
		EdgeDirection direct)
{
	PEdge tmp_edge = edge ? edge->prev : this->last_edge;
	while(tmp_edge
	&&!((int)tmp_edge->type&direct) )
		tmp_edge = tmp_edge->prev;
	return tmp_edge;
}

//edge properity
template<class VertInfo, class EdgeInfo>
EdgeType
Graph<VertInfo,EdgeInfo>::getEdgeType(PEdge edge) {
	return edge->type;
}

template<class VertInfo, class EdgeInfo>
EdgeType
Graph<VertInfo,EdgeInfo>::getType(PEdge edge) {
	return edge->type;
}

//sets
template<class VertInfo, class EdgeInfo>
Set< typename Graph<VertInfo,EdgeInfo>::PEdge >
Graph<VertInfo,EdgeInfo>::getEdgeSet(EdgeDirection direct)
{
	return getEdgeSet(direct, stdChoose(true));
}

template<class VertInfo, class EdgeInfo>
template<class Chooser>
Set< typename Graph<VertInfo,EdgeInfo>::PEdge >
Graph<VertInfo,EdgeInfo>::getEdgeSet(
		EdgeDirection direct, Chooser chooser)
{
	Set< typename Graph<VertInfo,EdgeInfo>::PEdge > ans;
	PEdge edge = this->first_edge;
	while(edge) {
		if( (edge->type&direct)
		&& chooser(edge,*this) )
		{
			ans.add(edge);
		}
		edge = edge->next;
	}
	return ans;
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator>
int
Graph<VertInfo,EdgeInfo>::getEdges(OutputIterator out,
		EdgeDirection direct)
{
	return getEdges(out, direct, stdChoose(true));
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator, class Chooser>
int
Graph<VertInfo,EdgeInfo>::getEdges(OutputIterator out,
		EdgeDirection direct, Chooser chooser)
{
	int ans = 0;
	PEdge edge = this->first_edge;
	while(edge) {
		if(  (edge->type&direct)
		&& chooser(edge,*this) )
		{
			*out = edge;
			++out;
			++ans;
		}
		edge = edge->next;
	}
	return ans;
}

//other
template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::getEdgeNo(EdgeDirection direct) {
	int ans=0;
	if(direct&(EdLoop))
		ans += this->no_loop_edge;
	if(direct&(EdUndir))
		ans += this->no_undir_edge;
	if(direct&(EdDirIn|EdDirOut))
		ans += this->no_dir_edge;
	return ans;
}

//----------vertices and edges connection--------------
//edge motion
//getIncidEdge
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdge(PVertex vert,
		EdgeDirection direct)
{
	if(!vert)
		return NULL;

	int tmp_v = 0;
	int tmp_dir = 1;
	while(tmp_v<4) {
		PEdge tmp_edge = vert->edges[tmp_v].first;
		if( (tmp_dir&direct) && tmp_edge )
			return tmp_edge;
		++tmp_v;
		tmp_dir <<= 1;
	}
	return NULL;
}

//getIncidEdgeLast
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgeLast(PVertex vert,
		EdgeDirection direct)
{
	if(!vert)
		return NULL;

	int tmp_v = 3;
	int tmp_dir = 0x8;
	while(tmp_v>=0) {
		PEdge tmp_edge = vert->edges[tmp_v].last;
		if( (tmp_dir&direct) && tmp_edge )
			return tmp_edge;
		--tmp_v;
		tmp_dir >>= 1;
	}
	return NULL;
}

//getIncidEdgeNext
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgeNext(PVertex vert,
		PEdge edge, EdgeDirection direct)
{
	if(!vert)
		return NULL;
	if(edge && !isEdgeEnd(edge,vert))
		return NULL;

	int tmp_v = 0;
	int tmp_dir = 1;
	PEdge tmp_edge = NULL;

	if(!edge) {
		while(tmp_v<4) {
			PEdge tmp_edge = vert->edges[tmp_v].first;
			if( (tmp_dir&direct) && tmp_edge )
				return tmp_edge;
			++tmp_v;
			tmp_dir <<= 1;
		}
		return NULL;
	}

	switch(edge->type) {
	case Loop:
		tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::loop;
		tmp_dir = EdLoop;
		if( direct&tmp_dir )
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next;
		break;
	case Directed:
		tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::dir_in;
		tmp_dir = EdDirIn;
		if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert==vert) {
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next;
		} else {
			tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::dir_out;
			tmp_dir = EdDirOut;
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next;
		}
		break;
	case Undirected:
		tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::undir;
		tmp_dir = EdUndir;
		if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert==vert) {
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next;
		} else {
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next;
		}
		break;
	}
	//if next is end then we search for another begining
	if(!tmp_edge) {
		++tmp_v;
		tmp_dir <<= 1;
		while(tmp_v<4) {
			tmp_edge = vert->edges[tmp_v].first;
			if( (tmp_dir&direct) && tmp_edge )
				return tmp_edge;
			++tmp_v;
			tmp_dir <<= 1;
		}
	}
	return tmp_edge;
}

//getIncidEdgePrev
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgePrev(PVertex vert,
		PEdge edge, EdgeDirection direct)
{
	if(!vert)
		return NULL;
	if(edge && !isEdgeEnd(edge,vert))
		return NULL;

	int tmp_v = 3;
	int tmp_dir = 0x8;
	PEdge tmp_edge = NULL;

	if(!edge) {
		while(tmp_v>=0) {
			PEdge tmp_edge = vert->edges[tmp_v].last;
			if( (tmp_dir&direct) && tmp_edge )
				return tmp_edge;
			--tmp_v;
			tmp_dir >>= 1;
		}
		return NULL;
	}

	switch(edge->type) {
	case Loop:
		tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::loop;
		tmp_dir = EdLoop;
		if( direct&tmp_dir )
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev;
		break;
	case Directed:
		tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::dir_in;
		tmp_dir = EdDirIn;
		if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert==vert) {
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev;
		} else {
			tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::dir_out;
			tmp_dir = EdDirOut;
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev;
		}
		break;
	case Undirected:
		tmp_v = Graph<VertInfo,EdgeInfo>::Vertex::undir;
		tmp_dir = EdUndir;
		if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert==vert) {
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev;
		} else {
			tmp_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev;
		}
		break;
	}
	//if prev is end then we search for another begining
	if(!tmp_edge) {
		--tmp_v;
		tmp_dir >>= 1;
		while(tmp_v>=0) {
			tmp_edge = vert->edges[tmp_v].last;
			if( (tmp_dir&direct) && tmp_edge )
				return tmp_edge;
			--tmp_v;
			tmp_dir >>= 1;
		}
	}
	return tmp_edge;
}

//sets
//getIncidEdges
template<class VertInfo, class EdgeInfo>
Set< typename Graph<VertInfo,EdgeInfo>::PEdge >
Graph<VertInfo,EdgeInfo>::getEdgeSet(PVertex vert,
		EdgeDirection direct)
{
	return getEdgeSet(vert, direct, stdChoose(true));
}

//getIncidEdges
template<class VertInfo, class EdgeInfo>
template<class Chooser>
Set< typename Graph<VertInfo,EdgeInfo>::PEdge >
Graph<VertInfo,EdgeInfo>::getEdgeSet(PVertex vert,
		EdgeDirection direct, Chooser chooser)
{
	Set< typename Graph<VertInfo,EdgeInfo>::PEdge > ans;
	PEdge edge = this->getEdge(vert, direct);
	while(edge) {
		if( chooser(edge,*this) ) {
			ans.add(edge);
		}
		edge = this->getEdgeNext(vert, edge, direct);
	}
	return ans;
}

//getIncidEdges
template<class VertInfo, class EdgeInfo>
template<class OutputIterator>
int
Graph<VertInfo,EdgeInfo>::getEdges(OutputIterator out,
		PVertex vert, EdgeDirection direct)
{
	return getEdges(out, vert, direct, stdChoose(true));
}

//getIncidEdges
template<class VertInfo, class EdgeInfo>
template<class OutputIterator, class Chooser>
int
Graph<VertInfo,EdgeInfo>::getEdges(OutputIterator out,
		PVertex vert, EdgeDirection direct, Chooser chooser)
{
	int ans = 0;
	PEdge edge = this->getEdge(vert, direct);
	while(edge) {
		if( chooser(edge,*this) ) {
			*out = edge;
			++out;
			++ans;
		}
		edge = this->getEdgeNext(vert, edge, direct);
	}
	return ans;
}

//other
//getIncidEdgeNo
template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::getEdgeNo(PVertex vert,
		EdgeDirection direct)
{
	int ans=0;
	if( direct&EdLoop )
		ans += vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].degree;
	if( direct&EdUndir )
		ans += vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree;
	if( direct&EdDirIn )
		ans += vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].degree;
	if( direct&EdDirOut )
		ans += vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].degree;
	return ans;
}

//------------------edge between vertices-------------------------
//getConnEdge
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdge(PVertex vert1, PVertex vert2,
		EdgeDirection direct)
{
	if(!vert1||!vert2)
		return NULL;
	if(vert1>vert2) {
		PVertex tmp_v = (PVertex)vert1;
		vert1 = (PVertex)vert2;
		vert2 = (PVertex)tmp_v;
		int mask = EdDirIn|EdDirOut;
		if( (direct&mask) && (direct&mask)!=mask )
			direct ^= mask;
	}

	PEdge edge = getEdge(vert1, direct);
	while(edge && getEdgeEnd(edge,vert1)!=vert2)
		edge = getEdgeNext(vert1, edge, direct);
	return edge;
}

//getConnEdgeLast
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgeLast(
		PVertex vert1, PVertex vert2,
		EdgeDirection direct)
{
	if(!vert1||!vert2)
		return NULL;
	if(vert1>vert2) {
		PVertex tmp_v = vert1;
		vert1 = vert2;
		vert2 = tmp_v;
		int mask = EdDirIn|EdDirOut;
		if( (direct&mask) && (direct&mask)!=mask )
			direct ^= mask;
	}

	PEdge edge = getEdgeLast(vert1, direct);
	while(edge && getEdgeEnd(edge,vert1)!=vert2)
		edge = getEdgePrev(vert1, edge, direct);
	return edge;
}

//getConnEdgeNext
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgeNext(PVertex vert1, PVertex vert2,
		PEdge edge, EdgeDirection direct)
{
	if(!vert1||!vert2)
		return NULL;
	if(edge && (!isEdgeEnd(edge,vert1) || !isEdgeEnd(edge,vert2)) )
		return NULL;

	if(vert1>vert2) {
		PVertex tmp_v = vert1;
		vert1 = vert2;
		vert2 = tmp_v;
		int mask = EdDirIn|EdDirOut;
		if( (direct&mask) && (direct&mask)!=mask )
			direct ^= mask;
	}

	if(!edge)
		return getEdge(vert1,vert2,direct);
	PEdge tmp_edge = (PEdge)edge;
	do {
		tmp_edge = getEdgeNext(vert1,tmp_edge,direct);
	} while(tmp_edge && getEdgeEnd(tmp_edge,vert1)!=vert2);
	return tmp_edge;
}

//getConnEdgePrev
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::getEdgePrev(PVertex vert1, PVertex vert2,
		PEdge edge, EdgeDirection direct)
{
	if(!vert1||!vert2)
		return NULL;
	if(edge && (!isEdgeEnd(edge,vert1) || !isEdgeEnd(edge,vert2)) )
		return NULL;

	if(vert1>vert2) {
		PVertex tmp_v = vert1;
		vert1 = vert2;
		vert2 = tmp_v;
		int mask = EdDirIn|EdDirOut;
		if( (direct&mask) && (direct&mask)!=mask )
			direct ^= mask;
	}

	if(!edge)
		return getEdgeLast(vert1,vert2,direct);
	do {
		edge = getEdgePrev(vert1,edge,direct);
	} while(edge && getEdgeEnd(edge,vert1)!=vert2);
	return edge;
}

//sets
//getConnEdges
template<class VertInfo, class EdgeInfo>
Set< typename Graph<VertInfo,EdgeInfo>::PEdge >
Graph<VertInfo,EdgeInfo>::getEdgeSet(PVertex vert1, PVertex vert2,
		EdgeDirection direct)
{
	return getEdgeSet(vert1, vert2, direct, stdChoose(true));
}

//getConnEdges
template<class VertInfo, class EdgeInfo>
template<class Chooser>
Set< typename Graph<VertInfo,EdgeInfo>::PEdge >
Graph<VertInfo,EdgeInfo>::getEdgeSet(PVertex vert1, PVertex vert2,
		EdgeDirection direct, Chooser chooser)
{
	Set< typename Graph<VertInfo,EdgeInfo>::PEdge > ans;
	PEdge edge = this->getEdge(vert1, vert2, direct);
	while(edge) {
		if( chooser(edge,*this) ) {
			ans.add(edge);
		}
		edge = this->getEdgeNext(vert1, vert2, edge, direct);
	}
	return ans;
}

//getConnEdges
template<class VertInfo, class EdgeInfo>
template<class OutputIterator>
int
Graph<VertInfo,EdgeInfo>::getEdges(OutputIterator out,
		PVertex vert1, PVertex vert2,
		EdgeDirection direct)
{
	return getEdges(out, vert1, vert2, direct, stdChoose(true));
}

//getConnEdges
template<class VertInfo, class EdgeInfo>
template<class OutputIterator, class Chooser>
int
Graph<VertInfo,EdgeInfo>::getEdges(OutputIterator out,
		PVertex vert1, PVertex vert2,
		EdgeDirection direct, Chooser chooser)
{
	int ans = 0;
	PEdge edge = this->getEdge(vert1, vert2, direct);
	while(edge) {
		if( chooser(edge,*this) ) {
			*out = edge;
			++out;
			++ans;
		}
		edge = this->getEdgeNext(vert1, vert2, edge, direct);
	}
	return ans;
}

//other
//getConnEdgeNo
template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::getEdgeNo(PVertex vert1, PVertex vert2,
		EdgeDirection direct)
{
	int ans = 0;
	PEdge tmp_edge = getEdge(vert1,vert2,direct);
	while(tmp_edge) {
		if(getEdgeEnd(tmp_edge,vert1)==vert2)
			++ans;
		tmp_edge = getEdgeNext(vert1,tmp_edge,direct);
	}
	return ans;
}

//------------neighbourhood-----------------------------------
template<class VertInfo, class EdgeInfo>
Set<typename Graph<VertInfo,EdgeInfo>::PVertex>
Graph<VertInfo,EdgeInfo>::getNeighSet(PVertex vert,
		EdgeDirection direct)
{
	return getNeighSet(vert, direct, stdChoose(true));
}

template<class VertInfo, class EdgeInfo>
template<class Chooser>
Set<typename Graph<VertInfo,EdgeInfo>::PVertex>
Graph<VertInfo,EdgeInfo>::getNeighSet(PVertex vert,
		EdgeDirection direct, Chooser choose)
{
	Set<PVertex> ans;
	PEdge edge = getEdge(vert, direct);
	while(edge) {
		if( choose(edge,*this) ) {
			ans.add( getEdgeEnd(edge,vert) );
		}
		edge = getEdgeNext(vert, edge, direct);
	}
	return ans;
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator, class Chooser>
int
Graph<VertInfo,EdgeInfo>::getNeigh(OutputIterator out, PVertex vert,
		EdgeDirection direct, Chooser choose)
{   typename Graph<VertInfo,EdgeInfo>::PVertex ans[getEdgeNo(vert, direct)];
    int size=0,res=0;
	PEdge edge = getEdge(vert, direct);
	while(edge) {
		if( choose(edge,*this) ) {
			ans[size++]=getEdgeEnd(edge,vert);
		}
		edge = getEdgeNext(vert, edge, direct);
	}
	make_heap(ans,ans+size); sort_heap(ans,ans+size);
	for(int i=0;i<size;i++) if (i==0 || ans[i-1]!=ans[i])
	{
	    *out=ans[i];
	    ++out; ++res;
	}
	return res;
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator, class Chooser>
int
Graph<VertInfo,EdgeInfo>::getClNeigh(OutputIterator out, PVertex vert,
		EdgeDirection direct, Chooser choose)
{   typename Graph<VertInfo,EdgeInfo>::PVertex ans[getEdgeNo(vert, direct)+1];
    ans[0]=vert;
    int size=1,res=0;
	PEdge edge = getEdge(vert, direct);
	while(edge) {
		if( choose(edge,*this) ) {
			ans[size++]=getEdgeEnd(edge,vert);
		}
		edge = getEdgeNext(vert, edge, direct);
	}
	make_heap(ans,ans+size); sort_heap(ans,ans+size);
	for(int i=0;i<size;i++) if (i==0 || ans[i-1]!=ans[i])
	{
	    *out=ans[i];
	    ++out; ++res;
	}
	return res;
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator>
int
Graph<VertInfo,EdgeInfo>::getNeigh(OutputIterator out, PVertex vert,
		EdgeDirection direct)
{
	return getNeigh(out, vert, direct, stdChoose(true));
}


template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::getNeighNo(PVertex vert,
		EdgeDirection direct)
{
	return getNeigh(blackHole(),vert,direct,stdChoose(true));
}

template<class VertInfo, class EdgeInfo>
Set<typename Graph<VertInfo,EdgeInfo>::PVertex>
Graph<VertInfo,EdgeInfo>::getClNeighSet(PVertex vert,
		EdgeDirection direct)
{
	return getClNeighSet(vert, direct, stdChoose(true));
}

template<class VertInfo, class EdgeInfo>
template<class Chooser>
Set<typename Graph<VertInfo,EdgeInfo>::PVertex>
Graph<VertInfo,EdgeInfo>::getClNeighSet(PVertex vert,
		EdgeDirection direct, Chooser choose)
{
	Set<PVertex> ans = getNeighSet(vert, direct, choose);
	ans.add((PVertex)vert);
	return ans;
}

template<class VertInfo, class EdgeInfo>
template<class OutputIterator>
int
Graph<VertInfo,EdgeInfo>::getClNeigh(OutputIterator out, PVertex vert,
		EdgeDirection direct)
{
	return getClNeigh(out, vert, direct, stdChoose(true));
}

template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::getClNeighNo(PVertex vert,
		EdgeDirection direct)
{
	return getClNeigh(blackHole(),vert,direct,stdChoose(true));
}

//-------------------vertices from edge---------------------------
template<class VertInfo, class EdgeInfo>
std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex>
Graph<VertInfo,EdgeInfo>::getEdgeEnds(PEdge edge) {
	return std::make_pair(edge->vert[0].vert, edge->vert[1].vert);
}

template<class VertInfo, class EdgeInfo>
std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex>
Graph<VertInfo,EdgeInfo>::getEnds(PEdge edge) {
	return this->getEdgeEnds(edge);
}

template<class VertInfo, class EdgeInfo>
bool
Graph<VertInfo,EdgeInfo>::isEdgeEnd(PEdge edge, PVertex vert) {
	return edge->vert[0].vert==vert || edge->vert[1].vert==vert;
}

template<class VertInfo, class EdgeInfo>
bool
Graph<VertInfo,EdgeInfo>::isEnd(PEdge edge, PVertex vert) {
	return this->isEdgeEnd(edge,vert);
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::getEdgeEnd(PEdge edge,
		PVertex vert)
{
	if( edge->vert[0].vert == vert )
		return edge->vert[1].vert;
	if( edge->vert[1].vert == vert )
		return edge->vert[0].vert;
	return NULL;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::getEnd(PEdge edge,
		PVertex vert)
{
	return this->getEdgeEnd(edge,vert);
}


//----------------------change edge properities------------------
template<class VertInfo, class EdgeInfo>
bool
Graph<VertInfo,EdgeInfo>::chEdgeType(PEdge edge, EdgeType type) {
    if (!edge) return false;
	if( edge->type == type )
		return true;
	if(edge->type==Directed
	&& type==Undirected) {
		PVertex vert1 = edge->vert[0].vert;
		PVertex vert2 = edge->vert[1].vert;
		if( !detach(edge) )
			return false; //probably should be throw
		return (bool)attach_undir(edge,vert1,vert2);
	}
	if(edge->type==Undirected
	&& type==Directed) {
		PVertex vert1 = edge->vert[0].vert;
		PVertex vert2 = edge->vert[1].vert;
		if( !detach(edge) )
			return false; //probably should be throw
		return (bool)attach_dir(edge,vert1,vert2);
	}
	return false;
}

template<class VertInfo, class EdgeInfo>
bool
Graph<VertInfo,EdgeInfo>::revertEdge(PEdge edge) {
	if(edge->type==Directed) {
		PVertex vert_in = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert;
		PVertex vert_out = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert;
		if( !detach(edge) )
			return false; //probably should be throw
		return (bool)attach_dir(edge, vert_in, vert_out);
	}
	return false;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::moveEdge(PEdge edge,
		PVertex vert1, PVertex vert2,
		EdgeDirection direct)
{
	return attach(edge, vert1, vert2, direct);
}

//-------------------vertices and edges vs numbers-------------
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::vertByNo(int idx) {
	PVertex tmp_vert = first_vert;
	while(idx && tmp_vert) {
		tmp_vert = tmp_vert->next;
		--idx;
	}
	return tmp_vert;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::edgeByNo(int idx) {
	PEdge tmp_edge = first_edge;
	while(idx && tmp_edge) {
		tmp_edge = tmp_edge->next;
		--idx;
	}
	return tmp_edge;
}

template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::vertPos(PVertex vert) {
	int idx=0;
	PVertex tmp_vert = first_vert;
	while(tmp_vert && tmp_vert!=vert) {
		tmp_vert = tmp_vert->next;
		++idx;
	}
	return tmp_vert ? idx : -1;
}

template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::edgePos(PEdge edge) {
	int idx=0;
	PEdge tmp_edge = first_edge;
	while(tmp_edge && tmp_edge!=edge) {
		tmp_edge = tmp_edge->next;
		++idx;
	}
	return tmp_edge ? idx : -1;
}

//-------------------graph and element's parameters-------------
template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::deg(PVertex vert,
		EdgeDirection direct)
{
	int ans=0;
	if( direct&EdLoop )
		ans += 2*vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].degree;
	if( direct&EdUndir )
		ans += vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree;
	if( direct&EdDirIn )
		ans += vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].degree;
	if( direct&EdDirOut )
		ans += vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].degree;
	return ans;
}

template<class VertInfo, class EdgeInfo>
std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,int>
Graph<VertInfo,EdgeInfo>::minDeg(EdgeDirection direct)
{
    std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,int> res(0,-1);
    if (!getVertNo()) return res;
        else { res.first=getVert(); res.second=deg(res.first,direct); }
    int degree;
    for(typename Graph<VertInfo,EdgeInfo>::PVertex vert=getVertNext(res.first);vert;vert=getVertNext(vert))
        if ((degree=deg(vert,direct))<res.second) res=std::make_pair(vert,degree);
    return res;
}

template<class VertInfo, class EdgeInfo>
std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,int>
Graph<VertInfo,EdgeInfo>::maxDeg(EdgeDirection direct)
{
    std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,int> res(0,-1);
    if (!getVertNo()) return res;
        else { res.first=getVert(); res.second=deg(res.first,direct); }
    int degree;
    for(typename Graph<VertInfo,EdgeInfo>::PVertex vert=getVertNext(res.first);vert;vert=getVertNext(vert))
        if ((degree=deg(vert,direct))>res.second) res=std::make_pair(vert,degree);
    return res;
}

template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::Delta(EdgeDirection direct)
{
    return max(0,maxDeg(direct).second);
}

template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::delta(EdgeDirection direct)
{
    return max(0,minDeg(direct).second);
}

//-------------------group modifications----------------------------
template<class VertInfo, class EdgeInfo>
int Graph<VertInfo,EdgeInfo>::delVerts()
{   int res=getVertNo();
    clear();
    return res;
}

template<class VertInfo, class EdgeInfo>
template <class Iterator>
int Graph<VertInfo,EdgeInfo>::delVerts(Iterator begin,Iterator end)
{   int res=0,size=0;
    for(Iterator iter=begin;iter!=end;iter++) size++;
    typename Graph<VertInfo,EdgeInfo>::PVertex buf[size];
    size=0;
    for(Iterator iter=begin;iter!=end;iter++) buf[size++]=*iter;
    make_heap(buf,buf+size); sort_heap(buf,buf+size);
    for(int i=0;i<size;i++)
        if (buf[i] && (i==0 || buf[i-1]!=buf[i]))
                { del(buf[i]); res++; }
    return res;
}

template<class VertInfo, class EdgeInfo>
int Graph<VertInfo,EdgeInfo>::delEdges(EdgeDirection direct)
{   int res=0;
    typename Graph<VertInfo,EdgeInfo>::PEdge e=getEdge(direct),enext;
    while (e) {
        enext=getEdgeNext(e,direct);
        del(e); res++;
        e=enext;
    }
    return res;
}

template<class VertInfo, class EdgeInfo>
int Graph<VertInfo,EdgeInfo>::delEdges(PVertex vert,EdgeDirection direct)
{   int res=0;
    if (!vert) return 0;
    typename Graph<VertInfo,EdgeInfo>::PEdge e=getEdge(vert,direct),enext;
    while (e) {
        enext=getEdgeNext(vert,e,direct);
        del(e); res++;
        e=enext;
    }
    return res;
}

template<class VertInfo, class EdgeInfo>
int Graph<VertInfo,EdgeInfo>::delEdges(PVertex vert1,PVertex vert2,EdgeDirection direct)
{   int res=0;
    if (!vert1 || !vert2) return 0;
    typename Graph<VertInfo,EdgeInfo>::PEdge e=getEdge(vert1,vert2,direct),enext;
    while (e) {
        enext=getEdgeNext(vert1,vert2,e,direct);
        del(e); res++;
        e=enext;
    }
    return res;
}

template<class VertInfo, class EdgeInfo>
template <class Iterator>
int Graph<VertInfo,EdgeInfo>::delEdges(Iterator begin,Iterator end,EdgeDirection direct)
{   int res=0,size=0;
    for(Iterator iter=begin;iter!=end;iter++) size++;
    typename Graph<VertInfo,EdgeInfo>::PEdge buf[size];
    size=0;
    for(Iterator iter=begin;iter!=end;iter++) buf[size++]=*iter;
    make_heap(buf,buf+size); sort_heap(buf,buf+size);
    for(int i=0;i<size;i++)
        if (buf[i] && (buf[i]->type&direct) && (i==0 || buf[i-1]!=buf[i]))
                { del(buf[i]); res++; }
    return res;
}

template<class VertInfo, class EdgeInfo>
template <class Iterator>
int Graph<VertInfo,EdgeInfo>::delEdges(PVertex vert,Iterator begin,Iterator end,EdgeDirection direct)
{   int res=0,size=0;
    for(Iterator iter=begin;iter!=end;iter++) size++;
    typename Graph<VertInfo,EdgeInfo>::PEdge buf[size];
    size=0;
    for(Iterator iter=begin;iter!=end;iter++) buf[size++]=*iter;
    make_heap(buf,buf+size); sort_heap(buf,buf+size);
    for(int i=0;i<size;i++)
        if (buf[i] && (buf[i]->type&direct) && (i==0 || buf[i-1]!=buf[i]))
            if (    (   (buf[i]->type==Undirected||buf[i]->type==Loop)&& isEdgeEnd(buf[i],vert)  ) ||
                    (   (buf[i]->type==Directed) &&
                        (   ((direct&EdDirOut) && getEdgeEnds(buf[i]).first==vert) ||
                            ((direct&EdDirIn) && getEdgeEnds(buf[i]).second==vert)
                        )
                     )
                )
                    { del(buf[i]); res++; }
    return res;
}

template<class VertInfo, class EdgeInfo>
template <class Iterator>
int
Graph<VertInfo,EdgeInfo>::delEdges(PVertex vert1,PVertex vert2,Iterator begin,Iterator end,EdgeDirection direct)
{   int res=0,size=0;
    for(Iterator iter=begin;iter!=end;iter++) size++;
    typename Graph<VertInfo,EdgeInfo>::PEdge buf[size];
    size=0;
    for(Iterator iter=begin;iter!=end;iter++) buf[size++]=*iter;
    make_heap(buf,buf+size); sort_heap(buf,buf+size);
    for(int i=0;i<size;i++)
        if (buf[i] && (buf[i]->type&direct) && (i==0 || buf[i-1]!=buf[i]))
            if (    (   (buf[i]->type==Loop)&& isEdgeEnd(buf[i],vert1) && vert1==vert2  ) ||
                    (   (buf[i]->type==Undirected)&& isEdgeEnd(buf[i],vert1)&& isEdgeEnd(buf[i],vert2)
                        && vert1!=vert2
                    ) ||
                    (   (buf[i]->type==Directed) &&
                        (   ((direct&EdDirOut) && getEdgeEnds(buf[i]).first==vert1 &&
                                getEdgeEnds(buf[i]).second==vert2
                            ) ||
                            ((direct&EdDirIn) && getEdgeEnds(buf[i]).first==vert2 &&
                                getEdgeEnds(buf[i]).second==vert1
                            )
                         )
                     )
                )
                    { del(buf[i]); res++; }
    return res;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::ch2Archs(PEdge edge)
{   if (!edge || edge->type!=Undirected) return 0;
    std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex>
                ends=getEdgeEnds(edge);
    chEdgeType(edge,Directed);
    return addArch(ends.second,ends.first,edge->info);
}

template<class VertInfo, class EdgeInfo>
template <class Iterator>
int
Graph<VertInfo,EdgeInfo>::ch2Archs(Iterator begin,Iterator end)
{   int res=0,size=0;
    for(Iterator iter=begin;iter!=end;iter++) size++;
    typename Graph<VertInfo,EdgeInfo>::PEdge buf[size];
    size=0;
    for(Iterator iter=begin;iter!=end;iter++) buf[size++]=*iter;
    make_heap(buf,buf+size); sort_heap(buf,buf+size);
    for(int i=0;i<size;i++)
        if ((i==0 || buf[i-1]!=buf[i]) && ch2Archs(buf[i])) res++;
    return res;
}

template<class VertInfo, class EdgeInfo>
int
Graph<VertInfo,EdgeInfo>::ch2Archs()
{   int res=getEdgeNo(Undirected);
    typename Graph<VertInfo,EdgeInfo>::PEdge e=getEdge(Undirected),enext;
    for(;e;e=enext)
    {   enext=getEdgeNext(e,Undirected);
        ch2Archs(e);
    }
    return res;
}

//relacja rownowaznosci, 3 mozliwosci
//reltype - dopuszczalne tylko jednobitowe: EdDirIn, EdDirOut lub EdUndir
//EdDirOut - luk nieskierowany jest rownolegly tylko do tak samo jak on skierowanego luku o tych samych koncach
//EdDirIn - jest on takze rownolegly do odwrotnie skierowanego luku o tych samych koncach
//EdUndir - jest on takze rownolegly do krawedzi nieskierowanej o tych samych koncach
template<class VertInfo, class EdgeInfo>
bool
Graph<VertInfo,EdgeInfo>::areParallel(PEdge e1,PEdge e2,EdgeDirection reltype)
{   if (!e1 || !e2 || !(reltype==EdDirIn||reltype==EdDirOut||reltype==EdUndir)) return false;
    std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex>
                        ends1=getEdgeEnds(e1),ends2=getEdgeEnds(e2);
    if (e1==e2) return true;
    else if (e1->type==Loop) return e2->type==Loop && ends1.first==ends2.first;
    else if (e2->type==Loop) return e1->type==Loop && ends1.first==ends2.first;
    else if (   (e1->type==Undirected && e2->type==Undirected)  ||
                (e1->type!=e2->type && reltype==EdUndir)    ||
                (e1->type==Directed && e2->type==Directed && (reltype==EdUndir||reltype==EdDirIn))
            )
        return  (ends1.first==ends2.first && ends1.second==ends2.second)
                    ||(ends1.second==ends2.first && ends1.first==ends2.second);
    else return e1->type==e2->type && ends1.first==ends2.first && ends1.second==ends2.second;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::putVert(PEdge edge,const VertInfo & info)
{   if (!edge) return 0;
    typename Graph<VertInfo,EdgeInfo>::PVertex res=addVert(info);
    std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex, typename Graph<VertInfo,EdgeInfo>::PVertex>
                    ends=getEdgeEnds(edge);
    EdgeType type=(getEdgeType(edge)==Directed) ? Directed : Undirected;
    addEdge(ends.first,res,edge->info,type);addEdge(res,ends.second,edge->info,type);
    del(edge);
    return res;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::putVert(PEdge edge)
{
    return putVert(edge,typename Graph<VertInfo,EdgeInfo>::VertInfoType());
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::pickVert(PVertex vert,const EdgeInfo & info)
{   if (!vert) return 0;
    int no;
    typename Graph<VertInfo,EdgeInfo>::PVertex neig[getEdgeNo(vert,EdDirIn|EdDirOut|EdUndir)];
    no=getNeigh(neig,vert,EdDirIn|EdDirOut|EdUndir);
    if (!(no==0 || no==1 || no==2)) return 0;
    if (no==0) { del(vert); return 0; }
    if (no==1) { del(vert); return addLoop(neig[0],info); }
    int delta=  (getEdgeNo(vert,neig[0],EdDirOut)-getEdgeNo(vert,neig[0],EdDirIn))-
                (getEdgeNo(vert,neig[1],EdDirOut)-getEdgeNo(vert,neig[1],EdDirIn));
    del(vert);
    return addEdge(neig[1],neig[0],info,(delta==0) ? EdUndir : ( (delta>0) ? EdDirOut : EdDirIn));
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::pickVert(PVertex vert)
{
    return pickVert(vert,typename Graph<VertInfo,EdgeInfo>::EdgeInfoType());
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::glue(PVertex vert1,PVertex vert2, bool makeloops)
{   if (!vert1) return vert2;
    if (!vert2) return vert1;
    if (vert1==vert2) return vert1;
    typename Graph<VertInfo,EdgeInfo>::PVertex args[2]={vert1,vert2};
    return glue(args,args+2,makeloops,vert1);
}

template<class VertInfo, class EdgeInfo>
template <class Iterator>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::glue(Iterator begin,Iterator end,bool makeloops,PVertex res)
{   int size=0;
    bool present=false;
    for(Iterator iter=begin;iter!=end;iter++)
    {   if (!res && *iter!=0) { res=*iter; present=true; }
        if (res && res==*iter) present=true;
        size++;
    }
    if (!present) return 0;
    typename Graph<VertInfo,EdgeInfo>::PVertex buf[size];
    size=0;
    for(Iterator iter=begin;iter!=end;iter++) if (*iter!=0 && res!=*iter) buf[size++]=*iter;
    make_heap(buf,buf+size); sort_heap(buf,buf+size); size=unique(buf,buf+size)-buf;
    if (!size) return res;
    for(int i=0;i<size;i++)
    {   typename Graph<VertInfo,EdgeInfo>::PEdge e,enext;
        for(e=getEdge(buf[i],EdLoop);e;e=enext)
        {   enext=getEdgeNext(buf[i],e,EdLoop);
            moveEdge(e,res,res,EdLoop);
        }
        for(e=getEdge(buf[i],EdDirIn|EdDirOut|EdUndir);e;e=enext)
        {   enext=getEdgeNext(buf[i],e,EdDirIn|EdDirOut|EdUndir);
            std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex>
                                                                                        ends=getEdgeEnds(e);
            if (ends.first==buf[i]) ends.first=res;
            if (ends.second==buf[i]) ends.second=res;
            if (ends.first==ends.second)
                if (makeloops) moveEdge(e,res,res,Loop);
                else del(e);
            else moveEdge(e,ends.first,ends.second,e->type==Undirected ? EdUndir : EdDirOut);
        }
        del(buf[i]);
    }
    return res;
}

//---------------clear methods-----------------
template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::clear() {
	PVertex cur_vert = this->first_vert;
	while(cur_vert) {
		PVertex tmp_vert = cur_vert->next;
		delete cur_vert;
		cur_vert = tmp_vert;
	}
	PEdge cur_edge = first_edge;
	while(cur_edge) {
		PEdge tmp_edge = cur_edge->next;
		delete cur_edge;
		cur_edge = tmp_edge;
	}
	first_vert = last_vert = NULL;
	first_edge = last_edge = NULL;
	no_vert = no_loop_edge = no_dir_edge = no_undir_edge = 0;
}

template<class VertInfo, class EdgeInfo>
void
Graph<VertInfo,EdgeInfo>::clearEdges() {
	PEdge cur_edge = first_edge;
	while(cur_edge) {
		PEdge tmp_edge = cur_edge->next;
		delete cur_edge;
		cur_edge = tmp_edge;
	}
	first_edge = last_edge = NULL;
	no_loop_edge = no_dir_edge = no_undir_edge = 0;

	PVertex cur_vert = first_vert;
	while(cur_vert) {
		for(int i=0; i<4; i++) {
			cur_vert->edges[i].first = NULL;
			cur_vert->edges[i].last = NULL;
			cur_vert->edges[i].degree = 0;
		}
		cur_vert = cur_vert->next;
	}
}

//---------------------move/copy methods----------------------------
template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::move(Graph<VertInfo,EdgeInfo> &graph) {
	if(&graph==this)
		return 0;

    typename Graph<VertInfo,EdgeInfo>::PVertex res=getVertLast();

	if(this->last_vert) {
		if(graph.first_vert) {
			this->last_vert->next = graph.first_vert;
			graph.first_vert->prev = this->last_vert;
			this->last_vert = graph.last_vert;
		}
	} else {
		this->first_vert = graph.first_vert;
		this->last_vert = graph.last_vert;
	}

	if(this->last_edge) {
		if(graph.first_edge) {
			this->last_edge->next = graph.first_edge;
			graph.first_edge->prev = this->last_edge;
			this->last_edge = graph.last_edge;
		}
	} else {
		this->first_edge = graph.first_edge;
		this->last_edge = graph.last_edge;
	}
	this->no_vert += graph.no_vert;
	this->no_dir_edge += graph.no_dir_edge;
	this->no_loop_edge += graph.no_loop_edge;
	this->no_undir_edge += graph.no_undir_edge;

	graph.first_vert = graph.last_vert = NULL;
	graph.first_edge = graph.last_edge = NULL;
	graph.no_vert = graph.no_dir_edge = graph.no_loop_edge
			= graph.no_undir_edge = 0;

    return res;
}


template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::copy(Graph<VertInfoExt,EdgeInfoExt> &graph)
{
	return copy(graph, std::make_pair(stdChoose(true),stdChoose(true)),
             std::make_pair(stdCast(false),stdCast(false)),
             std::make_pair(stdLink(false,false),stdLink(false,false)));
}

template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt, class VChooser, class EChooser>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::copy(Graph<VertInfoExt,EdgeInfoExt> &graph,
		std::pair<VChooser,EChooser> choosers)
{
	return copy(graph, choosers, std::make_pair(stdCast(false),stdCast(false)),
             std::make_pair(stdLink(false,false),stdLink(false,false)));
}

template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt, class VChooser, class EChooser, class VCaster, class ECaster>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::copy(Graph<VertInfoExt,EdgeInfoExt> &graph,
		std::pair<VChooser,EChooser> choosers, std::pair<VCaster,ECaster> casters)
{
	return copy(graph, choosers, casters, std::make_pair(stdLink(false,false),stdLink(false,false)));
}

template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt, class VChooser, class EChooser, class VCaster, class ECaster, class VLinker, class ELinker>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::copy(Graph<VertInfoExt,EdgeInfoExt> &graph,
		std::pair<VChooser,EChooser> choosers, std::pair<VCaster,ECaster> casters,
		std::pair<VLinker,ELinker> linkers)
{
    typename Graph<VertInfo,EdgeInfo>::PVertex res=getVertLast();

	if((void*)&graph==(void*)this)
	{   Graph<VertInfo,EdgeInfo> gr;
        gr.copy(graph,choosers,casters,linkers);
        return this->move(gr);
	}

	typedef typename Graph<VertInfoExt,EdgeInfoExt>::PVertex NPVertex;
	typedef typename Graph<VertInfoExt,EdgeInfoExt>::PEdge NPEdge;
	//create new vertices
	AssocArray<NPVertex,PVertex> ptr(graph.getVertNo());
	NPVertex vert = graph.getVert();
	VertInfo vertI;
	while(vert) {
		if( choosers.first(vert,graph) ) {
		    casters.first(vertI,vert->info);
			ptr[vert]=addVert( vertI );
//			vert->ptrVert() = tmp_vert;
			linkers.first(ptr[vert],vert);
		} else {
			ptr[vert] = NULL;
			linkers.first((PVertex)NULL,vert);
		}
		vert = graph.getVertNext(vert); // vert->next;
	}

	//create new edges
	NPEdge edge = graph.getEdge();
	EdgeInfo edgeI;
	while(edge) {
		std::pair<NPVertex,NPVertex> verts = graph.getEdgeEnds(edge);
		if(ptr[verts.first]
		&& ptr[verts.second]
		&& choosers.second(edge,graph)
		) { casters.second(edgeI,edge->info);
			PEdge tmp_edge = this->addEdge(ptr[verts.first],
					ptr[verts.second], edgeI, graph.getEdgeType(edge) );
			linkers.second(tmp_edge,edge);
		} else {
			linkers.second((PEdge)NULL,edge);
		}
		edge = graph.getEdgeNext(edge); //edge->next;
	}
	return res;
}


template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::substitute(PVertex vert,Graph<VertInfoExt,EdgeInfoExt> &graph)
{
	return substitute(vert,graph, std::make_pair(stdChoose(true),stdChoose(true)),
             std::make_pair(stdCast(false),stdCast(false)),
             std::make_pair(stdLink(false,false),stdLink(false,false)));
}

template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt, class VChooser, class EChooser>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::substitute(PVertex vert, Graph<VertInfoExt,EdgeInfoExt> &graph,
		std::pair<VChooser,EChooser> choosers)
{
	return substitute(vert,graph, choosers, std::make_pair(stdCast(false),stdCast(false)),
             std::make_pair(stdLink(false,false),stdLink(false,false)));
}

template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt, class VChooser, class EChooser, class VCaster, class ECaster>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::substitute(PVertex vert,Graph<VertInfoExt,EdgeInfoExt> &graph,
		std::pair<VChooser,EChooser> choosers, std::pair<VCaster,ECaster> casters)
{
	return substitute(vert,graph, choosers, casters, std::make_pair(stdLink(false,false),stdLink(false,false)));
}

template<class VertInfo, class EdgeInfo>
template<class VertInfoExt, class EdgeInfoExt, class VChooser, class EChooser, class VCaster, class ECaster, class VLinker, class ELinker>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::substitute(typename Graph<VertInfo,EdgeInfo>::PVertex vert,Graph<VertInfoExt,EdgeInfoExt> &graph,
		std::pair<VChooser,EChooser> choosers, std::pair<VCaster,ECaster> casters,
		std::pair<VLinker,ELinker> linkers)
{   if (!vert || (void*)this==(void*)&graph) return 0; // TODO: throw
    typename Graph<VertInfo,EdgeInfo>::PVertex res=this->copy(graph,choosers,casters,linkers);
    for(typename Graph<VertInfo,EdgeInfo>::PVertex v=getVertNext(res);v;v=getVertNext(v))
        for(typename Graph<VertInfo,EdgeInfo>::PEdge e=getEdge(vert,EdDirIn|EdDirOut|EdUndir);
                                            e;e=getEdgeNext(vert,e,EdDirIn|EdDirOut|EdUndir))
        {   std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex> ends;
            ends=getEdgeEnds(e); if (ends.first==vert) ends.first=v; else ends.second=v;
            addEdge(ends.first,ends.second,e->info,getEdgeType(e));
        }
    if (res==vert) res=getVertPrev(res);
    del(vert);
    return res;
}

template<class VertInfo, class EdgeInfo>
Graph<VertInfo,EdgeInfo>&
Graph<VertInfo,EdgeInfo>::operator=(Graph<VertInfo,EdgeInfo>& gr)
{   if (&gr==this) return *this;
    clear(); copy(gr);
    return *this;
}


template<class VertInfo, class EdgeInfo>
Graph<VertInfo,EdgeInfo>&
Graph<VertInfo,EdgeInfo>::operator+=(Graph<VertInfo,EdgeInfo>& gr)
{   copy(gr);
    return *this;
}


//-----------------attach/detach methods-----------------------

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::attach(PVertex vert) {
	if(!vert)
		return NULL;
	if(vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree!=-1) //equal to -1 means not attached
		return NULL;
	for(int i=0; i<4; i++)
		vert->edges[i].degree = 0;
	vert->prev = last_vert;
	vert->next = NULL;
	if(last_vert) {
		last_vert->next = vert;
	} else
		first_vert = vert;
	last_vert = vert;
	++no_vert;
	return vert;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PVertex
Graph<VertInfo,EdgeInfo>::detach(PVertex vert) {
	if(!vert)
		return NULL;
	if(vert->edges[0].degree!=0
	|| vert->edges[1].degree!=0
	|| vert->edges[2].degree!=0
	|| vert->edges[3].degree!=0
	)
		return NULL;
	vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree = -1;

	if(vert->next)
		vert->next->prev = vert->prev;
	else
		last_vert = vert->prev;

	if(vert->prev)
		vert->prev->next = vert->next;
	else
		first_vert = vert->next;

	vert->next = vert->prev = NULL;
	--no_vert;
	return vert;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::attach(PEdge edge,
		PVertex vert1, PVertex vert2, EdgeDirection direct)
{
	switch(direct) {
	case EdDirIn:
		return attach_dir(edge,vert2,vert1);
	case EdDirOut: case EdDirIn|EdDirOut:
		return attach_dir(edge,vert1,vert2);
	case EdUndir:
		return attach_undir(edge,vert1,vert2);
	case Loop:
		if(vert1==vert2 || !vert2)
			return attach_loop(edge,vert1);
		return NULL;
	}
	return NULL;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::attach_undir(PEdge edge, PVertex vertU, PVertex vertV) {
	if(!edge)
		return NULL;
	if(!vertU)
		return NULL;
	if(!vertV)
		return NULL;
	if(vertU==vertV)
		return NULL;
	if(edge->type!=Detached)
		detach(edge);

	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert = vertU;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert = vertV;
	edge->type = Undirected;

	//adding to global list
	edge->prev = last_edge;
	edge->next = NULL;
	if(last_edge)
		last_edge->next = edge;
	else
		first_edge = edge;
	last_edge = edge;
	++no_undir_edge;

	//adding to local list (vertU)
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev = vertU->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next = NULL;
	if(vertU->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last) {
		PEdge tmp_edge = vertU->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last;
		if(tmp_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert==vertU)
			tmp_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next = edge;
		else
			tmp_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next = edge;
	} else
		vertU->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].first = edge;
	vertU->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last = edge;
	++vertU->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree;

	//adding to local list (vertV)
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev = vertV->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next = NULL;
	if(vertV->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last) {
		PEdge tmp_edge = vertV->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last;
		if(tmp_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert==vertV)
			tmp_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next = edge;
		else tmp_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next = edge;
	} else
		vertV->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].first = edge;
	vertV->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last = edge;
	++vertV->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree;

	return edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::attach_dir(PEdge edge, PVertex vert_out, PVertex vert_in) {
	if(!edge)
		return NULL;
	if(!vert_out)
		return NULL;
	if(!vert_in)
		return NULL;
	if(vert_out==vert_in)
		return NULL;
	if(edge->type!=Detached)
		detach(edge);

	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert = vert_out;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert = vert_in;
	edge->type = Directed;

	//adding to global list
	edge->prev = last_edge;
	edge->next = NULL;
	if(last_edge)
		last_edge->next = edge;
	else
		first_edge = edge;
	last_edge = edge;
	++no_dir_edge;

	//adding to local list (vert_out)
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev = vert_out->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].last;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next = NULL;
	if(vert_out->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].last)
		vert_out->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].last->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next = edge;
	else
		vert_out->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].first = edge;
	vert_out->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].last = edge;
	++vert_out->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].degree;

	//adding to local list (vert_in)
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev = vert_in->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].last;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next = NULL;
	if(vert_in->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].last)
		vert_in->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].last->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next = edge;
	else
		vert_in->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].first = edge;
	vert_in->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].last = edge;
	++vert_in->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].degree;

	return edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::attach_loop(PEdge edge, PVertex vert) {
	if(!edge)
		return NULL;
	if(!vert)
		return NULL;
	if(edge->type!=Detached)
		detach(edge);

	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].vert = vert;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_Nloop].vert = vert;
	edge->type = Loop;

	//adding to global list
	edge->prev = last_edge;
	edge->next = NULL;
	if(last_edge)
		last_edge->next = edge;
	else
		first_edge = edge;
	last_edge = edge;
	++no_loop_edge;

	//adding to local list (vert)
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev = vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].last;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next = NULL;
	if(vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].last)
		vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].last->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next = edge;
	else
		vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].first = edge;
	vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].last = edge;
	++vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].degree;

	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_Nloop].next = NULL;
	edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_Nloop].prev = NULL;

	return edge;
}

template<class VertInfo, class EdgeInfo>
typename Graph<VertInfo,EdgeInfo>::PEdge
Graph<VertInfo,EdgeInfo>::detach(PEdge edge) {
	if(!edge)
		return NULL;
	if(edge->type==Detached)
		return NULL;

	switch(edge->type) {
		case Loop:
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next) {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev;
			} else {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].last
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev;
			}

			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev) {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].prev->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next;
			} else {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].first
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].next;
			}

			--edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_loop].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::loop].degree;
			--no_loop_edge;
			break;
		case Directed:
			//unlink from local list (out)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next) {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev;
			} else {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].last
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev;
			}

			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev) {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].prev->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next;
			} else {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].first
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].next;
			}

			--edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_out].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_out].degree;

			//unlink from local list (in)
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next) {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev;
			} else {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].last
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev;
			}

			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev) {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].prev->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next;
			} else {
				edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].first
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].next;
			}

			--edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_in].vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::dir_in].degree;
			--no_dir_edge;
			break;
		case Undirected: {
			//unlink from local list (vertU)
			PVertex vert = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert;
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next) {
				PEdge next_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next;
				if(next_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert==vert) {
					next_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev;
				} else {
					next_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev;
				}
			} else {
				vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev;
			}

			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev) {
				PEdge prev_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev;
				if(prev_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert==vert) {
					prev_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next;
				} else {
					prev_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next;
				}
			} else {
				vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].first
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next;
			}
			--vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree;

			//unlink from local list (vertV)
			vert = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].vert;
			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next) {
				PEdge next_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next;
				if(next_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert==vert) {
					next_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].prev
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev;
				} else {
					next_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev;
				}
			} else {
				vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].last
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev;
			}

			if(edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev) {
				PEdge prev_edge = edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].prev;
				if(prev_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].vert==vert) {
					prev_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_U].next
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next;
				} else {
					prev_edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next
							= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next;
				}
			} else {
				vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].first
						= edge->vert[Graph<VertInfo,EdgeInfo>::Edge::V_V].next;
			}

			--vert->edges[Graph<VertInfo,EdgeInfo>::Vertex::undir].degree;
			--no_undir_edge;
			} break;
		default:
			return NULL;
	}
	//unlink edge from global list
	if(edge->next)
		edge->next->prev = edge->prev;
	else
		last_edge = edge->prev;

	if(edge->prev)
		edge->prev->next = edge->next;
	else
		first_edge = edge->next;
    edge->type=Detached;
	return edge;
}

