//--------------------begin of VertColoringTest---------------------------
template< typename Graph, typename ColorMap >
int VertColoringTest::maxColor(const Graph &graph, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	int col = -1;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(!colors.hasKey(vv)) continue;
		int tmp = colors[vv];
		if(col<tmp) col = tmp;
	}
	return col;
}

template<typename Graph, typename ColorMap>
bool VertColoringTest::testPart(const Graph &graph, const ColorMap &colors) {
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(!colors.hasKey(vv)) continue;
		int usedColor = colors[vv];
		if(usedColor<0) continue;

		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee=graph.getEdgeNext(ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(u<vv) continue;
			if(colors.hasKey(u) && colors[u]==usedColor)
				return false;
		}
	}
	return true;
}

template<typename Graph, typename ColorMap>
bool VertColoringTest::test(const Graph &graph, const ColorMap &colors) {
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(!colors.hasKey(vv)) return false;
		int usedColor = colors[vv];
		if(usedColor<0) return false;

		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee=graph.getEdgeNext(ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(u<vv) continue;
			if(colors.hasKey(u) && colors[u]==usedColor)
				return false;
		}
	}
	return true;
}

//---------------------end of VertColoringTest----------------------------

//--------------------begin of SeqVertColoringPar-------------------------
template <class DefaultStructs> template<typename S>
inline bool SeqVertColoringPar<DefaultStructs>::LfCmp<S>::
	operator() (const S &a, const S &b)
{
	return a.deg > b.deg || (a.deg == b.deg && a.v < b.v);
}

template <class DefaultStructs> template<typename S>
inline bool SeqVertColoringPar<DefaultStructs>::SlCmp<S>::
	operator() (const S &a, const S &b)
{
	return a.deg < b.deg || (a.deg==b.deg && a.v<b.v);
}

template <class DefaultStructs> template<typename S>
inline bool SeqVertColoringPar<DefaultStructs>::SlfCmp<S>::
	operator() (const S &a, const S &b)
{
	return a.sat>b.sat || (a.sat==b.sat&&(a.deg>b.deg
			|| (a.deg==b.deg && a.v<b.v)));
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::satDeg(const Graph &graph,
	const ColorMap &colors, typename Graph::PVertex vert)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int deg = graph.deg(vert, Mask);
	int LOCALARRAY(tabColors, deg);
	int lenColors = 0;

	for(Edge ee = graph.getEdge(vert, Mask); ee;
		ee = graph.getEdgeNext(vert, ee, Mask))
	{
		Vert vv = graph.getEdgeEnd(ee, vert);
		if(!colors.hasKey(vv)) continue;
		int col = colors[vv];
		if(col<0) continue;
		tabColors[lenColors++] = col;
	}

	DefaultStructs::sort(tabColors, tabColors+lenColors); //counting different colors
	return std::unique(tabColors, tabColors+lenColors) - tabColors;
}

template <class DefaultStructs>
template< typename Graph, typename ColorMap, typename CompMap >
int SeqVertColoringPar<DefaultStructs>::interchangeComponents(const Graph &graph,
	const ColorMap &colors, typename Graph::PVertex vert,
	CompMap &compMap, int color1, int color2 )
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	assert( vert && !colors.hasKey( vert ) );

	int nComp = 0;
	for(Edge ee = graph.getEdge(vert, Mask); ee;
		ee = graph.getEdgeNext(vert, ee, Mask) )
	{
		Vert u = graph.getEdgeEnd( ee, vert );
		if( !colors.hasKey(u) ) continue;
		int color = colors[u];
		//check components created from vertices colored by color1 or color2
		if ((color == color1 || color == color2) && !compMap.hasKey( u )) {
			BFSPar<DefaultStructs>::scanAttainable(
				makeSubgraph( graph ,
					std::make_pair(extAssocChoose( &colors,color1 )
						|| extAssocChoose( &colors,color2 ),stdChoose( true ))),
				u, assocInserter( compMap,constFun( nComp ) ), Mask );
			nComp++;
		}
	}
	return nComp;
}

template <class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqVertColoringPar<DefaultStructs>::colorInterchange(const Graph &graph,
	ColorMap &colors, typename Graph::PVertex vert )
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int oldColor = colors[ vert ];
	colors.delKey( vert );

	typename DefaultStructs::template
		AssocCont<Vert, int>::Type compMap(graph.getVertNo());
	bool LOCALARRAY(matchedColors, graph.getVertNo());

	for(int c1 = 0; c1<oldColor; c1++) {
		for(int c2 = c1+1; c2<oldColor; c2++)
		{
			compMap.clear();
			int cntMapCol = interchangeComponents(graph, colors, vert,
					compMap, c1, c2);

			for(int j = 0; j < cntMapCol; j++)
				matchedColors[j] = false;

			for(Edge ee = graph.getEdge(vert, Mask); ee;
					ee = graph.getEdgeNext(vert, ee, Mask))
			{
				Vert u = graph.getEdgeEnd(ee, vert);
				if(compMap.hasKey( u ) && colors[u] == c1)
					matchedColors[ compMap[u] ] = true;
			}

			bool found = false;
			for(Edge ee = graph.getEdge(vert, Mask); ee;
					ee = graph.getEdgeNext(vert, ee, Mask))
			{
				Vert u = graph.getEdgeEnd(ee, vert);
				if(compMap.hasKey(u) && colors[u] == c2
					&& matchedColors[ compMap[u] ])
				{
					found = true; //if one component meet v with color c1 and c2 (can't be recolored)
					break;
				}
			}

			if (!found) {
				for(Vert u = compMap.firstKey(); u; u = compMap.nextKey( u )) {
					if (matchedColors[ compMap[u] ])
						colors[u] = (colors[u] == c1) ? c2 : c1;
				}
				return colors[vert] = c1;
			}
		}
	}

	return colors[vert] = oldColor; //recoloring failed
}

template <class DefaultStructs>
template<typename Graph, typename VInOutIter>
int SeqVertColoringPar<DefaultStructs>::lfSort(const Graph &graph,
	VInOutIter beg, VInOutIter end)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typedef VertDeg<Vert> LfStr;

	assert(beg!=end);
	LfStr LOCALARRAY(lfStr, end - beg);
	int lenLfStr = 0;

	for(VInOutIter cur=beg; cur!=end; ++cur) {
		lfStr[lenLfStr++] = LfStr(*cur, graph.deg(*cur, Mask));
	}
	DefaultStructs::sort(lfStr, lfStr+lenLfStr, LfCmp<LfStr>());
	lenLfStr = std::unique(lfStr, lfStr+lenLfStr) - lfStr;

	for(int iLfStr = 0; iLfStr<lenLfStr; iLfStr++)
		*(beg++) = lfStr[iLfStr].v;

	return lenLfStr;
}

template <class DefaultStructs>
template<typename Graph, typename VInOutIter>
int SeqVertColoringPar<DefaultStructs>::slSort(const Graph &graph,
	VInOutIter beg, VInOutIter end)
{
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	typedef VertDeg<Vert> SlStruct;
	typedef typename DefaultStructs::template
		HeapCont< SlStruct, SlCmp<SlStruct> >::Type PriQueue;
	typedef typename DefaultStructs::template
		AssocCont<Vert, typename PriQueue::Node*>::Type VertToQueue;

	assert(beg!=end);
	PriQueue priQueue;
	VertToQueue vertToQueue(end - beg);

	int lenVerts = 0;
	for(VInOutIter cur = beg; cur!=end; ++cur) {
		if(vertToQueue.hasKey(*cur)) continue;
		vertToQueue[*cur] = priQueue.push( SlStruct(*cur, graph.deg(*cur,Mask)) );
		lenVerts++;
	}

	VInOutIter cur = beg+lenVerts;
	int iVerts = lenVerts;
	while(priQueue.size()>0) {
		SlStruct t = priQueue.top();
		priQueue.pop();
		Vert vv = t.v;

		--cur;
		*cur = vv;
		vertToQueue.delKey(vv);

		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if( !vertToQueue.hasKey(u) ) continue;
			typename PriQueue::Node *node = vertToQueue[u];
			SlStruct tmp = node->get();
			assert(tmp.deg>0);
			--tmp.deg;
			priQueue.decrease(node, tmp);
		}
	}
	return lenVerts;
}


//-------brooks algorithm - components:

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
SeqVertColoringPar<DefaultStructs>::BrooksState<Graph, ColorMap>::
BrooksState (const Graph &g, ColorMap &c): graph(g), colors(c), vertDepth(g.getVertNo())
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	begVert = endVert = g.getVertNo()*3;
	vertStack = new Vert[endVert];

	begEdge = endEdge = g.getVertNo() + g.getEdgeNo(Mask);
	edgeStack = new Edge[endEdge];

	curVertStack = curEdgeStack = 0;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
SeqVertColoringPar<DefaultStructs>::BrooksState<Graph, ColorMap>::
~BrooksState()
{
	delete [] vertStack;
	delete [] edgeStack;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
void SeqVertColoringPar<DefaultStructs>::BrooksState<Graph, ColorMap>::
biconnected(int bVert, int bEdge) {
	//copy vertStack[bVert:curVertStack] to vertStack[...:begVert] (almost)
	while(bVert<curVertStack) {
		--curVertStack;
		--begVert;
		vertStack[begVert] = vertStack[curVertStack];
	}
	--begVert;
	vertStack[begVert] = NULL; //mark end of copy
	//copy edgeStack[bEdge:curEdgeStack] to edgeStack[...:begEdge] (almost)
	while(bEdge<curEdgeStack) {
		--curEdgeStack;
		--begEdge;
		edgeStack[begEdge] = edgeStack[curEdgeStack];
	}
	--begEdge;
	edgeStack[begEdge] = NULL; //mark end of copy
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::brooks(
	typename SeqVertColoringPar<DefaultStructs>::template BrooksState<Graph, ColorMap> &bState,
	typename Graph::PVertex vert, int depth)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	++depth;
	bState.vertDepth[vert] = depth;
	int low = depth;
	for(Edge ee = bState.graph.getEdge(vert, Mask); ee;
		ee = bState.graph.getEdgeNext(vert, ee, Mask))
	{
		Vert vv = bState.graph.getEdgeEnd(ee, vert);
		if(bState.vertDepth.hasKey(vv)) {
			int d = bState.vertDepth[vv];
			if(depth > d)
				bState.edgeStack[bState.curEdgeStack++] = ee;
			if(low > d)
				low = d;
			continue;
		}

		int curVertStack = bState.curVertStack;
		int curEdgeStack = bState.curEdgeStack;
		int m = brooks(bState, vv, depth);
		if(m<low)
			low = m;
		if(m>=depth) {
			bState.vertStack[bState.curVertStack++] = vert;
			bState.biconnected(curVertStack, curEdgeStack);
		}
	}
	bState.vertStack[bState.curVertStack++] = vert;
	return low;
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap>
void SeqVertColoringPar<DefaultStructs>::brooksBiconnected(
	typename SeqVertColoringPar<DefaultStructs>::template BrooksState<Graph, ColorMap> &bState)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	typedef std::pair<Vert, Vert> VertPair;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	typedef typename DefaultStructs::template
		LocalGraph<int, Koala::EmptyEdgeInfo, Undirected, false>::Type Subgraph;
	typedef typename Subgraph::PVertex VertSub;
	typedef typename Subgraph::PEdge EdgeSub;
	typename DefaultStructs::template
		AssocCont<Vert, VertSub>::Type mapVert(bState.graph.getVertNo());
	std::set< std::pair<Vert, Vert> > simple; // !CHANGE! - don't use std::set

	Subgraph subgraph; //subgraph is biconnected
	while(bState.begVert!=bState.endVert) {
		//make a subgraph
		subgraph.clear();

		++bState.begVert; //skip NULL
		int begVert = bState.begVert;
		while(bState.begVert!=bState.endVert) {
			Vert vert = bState.vertStack[bState.begVert];
			if(vert==NULL) break;
			VertSub vv = subgraph.addVert(0);
			mapVert[vert] = vv;
			++bState.begVert;
		}
		Vert vertCol = bState.vertStack[bState.begVert-1]; //colored vertex is always last
		if(!bState.colors.hasKey(vertCol))
			vertCol = NULL;

		//we force to have simple graph (subgraph)
		++bState.begEdge; //skip NULL
		while(bState.begEdge!=bState.endEdge) {
			Edge edge = bState.edgeStack[bState.begEdge];
			if(edge==NULL) break;
			Vert v1 = bState.graph.getEdgeEnd1(edge);
			Vert v2 = bState.graph.getEdgeEnd2(edge);

			VertPair tmp;
			if(v1<v2) tmp = std::make_pair(v1,v2);
			else tmp = std::make_pair(v2,v1);
			if(simple.find(tmp)!=simple.end()) continue;
			simple.insert(tmp);

			subgraph.addEdge(mapVert[v1], mapVert[v2]);
			++bState.begEdge;
		}
		simple.clear();

		//searching minimum and almost maximum vertex degree
		//  'almost' means that we exclude vertices of degree n-1
		int degFull = subgraph.getVertNo()-1;
		int minDeg = subgraph.getEdgeNo(), maxDeg = 0;
		VertSub maxVert = NULL;
		for(VertSub vv = subgraph.getVert(); vv;
			vv = subgraph.getVertNext(vv))
		{
			int deg = subgraph.deg(vv);
			if(minDeg>deg)
				minDeg = deg;
			if(maxDeg<deg && deg<degFull) {
				maxDeg = deg;
				maxVert = vv;
			}
		}
		if(minDeg==degFull) {
			//complete graph coloring
			int col = vertCol!=NULL
				? bState.colors[vertCol] : subgraph.getVertNo()+1;
			int c=0;
			for(; begVert!=bState.begVert; ++begVert) {
				Vert vv = bState.vertStack[begVert];
				if(vv==vertCol) continue;
				if(c==col) c++;
				bState.colors[vv] = c++;
			}
			continue; //next subgraph
		}

		VertSub vX, vA, vB; //description below
		if(minDeg==2 && maxDeg==2) {
			//cycle graph coloring or K_{1,1,n-2}
			EdgeSub ee = subgraph.getEdge(maxVert);
			VertSub vv = subgraph.getEdgeEnd(ee, maxVert);
			//if(subgraph.deg(vv)>2) it's K_{1,1,n-2}
			vX = vv;
			vA = vB = maxVert;
			goto coloring; //skip following if
		}

		//we choose vX, vA, vB such that vA, vB are neighbours of vX but vA is not a neighbour of vB
		//moreover subgraph-{vA,vB} is connected
		//vA and vB color by 0, DFS begin in vX
		//here forall v in subgraph v->info==0
		if(brooksBiconnectedTest(subgraph, maxVert)) {
			//graph subgraph-maxVert is biconnected
			//so here also all v->info==0;
			vA = maxVert;
			//mark close neighbourhood of vA (v->info=1)
			vA->info = 1;
			for(EdgeSub ee = subgraph.getEdge(vA); ee;
				ee = subgraph.getEdgeNext(vA, ee))
			{
				VertSub vv = subgraph.getEdgeEnd(ee, vA);
				vv->info = 1;
			}
			//search for a vertex at distance 2 from vA
			for(EdgeSub ee = subgraph.getEdge(vA); ee;
				ee = subgraph.getEdgeNext(vA, ee))
			{
				vX = subgraph.getEdgeEnd(ee, vA);
				EdgeSub ff;
				for(ff = subgraph.getEdge(vX); ff;
					ff = subgraph.getEdgeNext(vX, ff))
				{
					vB = subgraph.getEdgeEnd(ff, vX);
					if(vB->info==0)
						break; //break from 2 loops
				}
				if(ff!=NULL) //true iff vB->info!=0
					break;
			}
		} else {
			//graph subgraph-maxVert is not biconnected
			//v->info==1 iff v is a cut vertex
			//we will set v->info==2 if v is adjacent to vX
			vX = maxVert;
			//count vX adjacent vertices which are not cut vertices
			int cntAdj = 0;
			for(EdgeSub ee = subgraph.getEdge(vX); ee;
				ee = subgraph.getEdgeNext(vX, ee))
			{
				VertSub vv = subgraph.getEdgeEnd(ee, vX);
				if(vv->info!=0) continue;
				vv->info = 2;
				cntAdj++;
			}
			//info==1 cut vertex; info==2 vertex adjacent to vX and not cut vertex
			//vA and vB cannot be cut vertices
			for(EdgeSub ee = subgraph.getEdge(vX); ee;
				ee = subgraph.getEdgeNext(vX, ee))
			{
				vA = subgraph.getEdgeEnd(ee, vX);
				if(vA->info!=2) continue;
				//check how many vertices adjacent to vX is also adjacent to vA
				int iAdj = 1;
				for(EdgeSub ff = subgraph.getEdge(vA); ff;
					ff = subgraph.getEdgeNext(vA, ff))
				{
					VertSub vv = subgraph.getEdgeEnd(ff, vA);
					if(vv->info!=2) continue;
					iAdj++;
				}
				if(iAdj!=cntAdj)
					break;
			}
			//there exists vertex adjacent to vX and not adjacent to vA
			for(EdgeSub ee = subgraph.getEdge(vA); ee;
				ee = subgraph.getEdgeNext(vA, ee))
			{
				VertSub vv = subgraph.getEdgeEnd(ee, vA);
				vv->info = 1;
			}
			vA->info = 1;
			//now info==1 iff it is a cut vertex or it is vertex adjacent to vA
			for(EdgeSub ee = subgraph.getEdge(vX); ee;
				ee = subgraph.getEdgeNext(vX, ee))
			{
				vB = subgraph.getEdgeEnd(ee, vX);
				if(vB->info!=1) //can be replaced by vB->info==2
					break;
			}
		}

		coloring:
		//now coloring
		for(VertSub vv = subgraph.getVert(); vv;
			vv = subgraph.getVertNext(vv))
		{
			vv->info = -1;
		}

		//color subgraph
		vA->info = vB->info = 0;
		brooksBiconnectedColor(subgraph, vX);

		//transformation between subgraph coloring and graph coloring
		int col1, col2;
		if(vertCol==NULL) {
			col1 = col2 = 0; //don't change coloring
		} else {
			col1 = bState.colors[vertCol];
			col2 = mapVert[vertCol]->info;
		}

		for(; begVert!=bState.begVert; ++begVert) {
			Vert vv = bState.vertStack[begVert];
			int col = mapVert[vv]->info;
			if(col == col1)
				bState.colors[vv] = col2;
			else if(col == col2)
				bState.colors[vv] = col1;
			else
				bState.colors[vv] = col;
		}
	}
	return;
}

template<class DefaultStructs>
template<typename Graph>
void SeqVertColoringPar<DefaultStructs>::brooksBiconnectedColor(
	const Graph &graph, typename Graph::PVertex vert)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;

	vert->info = -2;
	int deg = 0;
	for(Edge ee = graph.getEdge(vert); ee;
		ee = graph.getEdgeNext(vert, ee))
	{
		++deg;
		Vert vv = graph.getEdgeEnd(ee, vert);
		if(vv->info != -1) continue;
		brooksBiconnectedColor(graph, vv);
	}

	bool LOCALARRAY(neighCol, deg);
	for(int i = 0; i < deg; i++) neighCol[i] = false;

	for(Edge ee = graph.getEdge(vert); ee;
		ee = graph.getEdgeNext(vert, ee))
	{
		Vert vv = graph.getEdgeEnd(ee, vert);
		if(vv->info<0||vv->info>=deg) continue;
		neighCol[ vv->info ] = true;
	}

	int col = 0;
	while( neighCol[col] ) col++;
	vert->info = col;
}

template <class DefaultStructs>
template<typename Graph>
bool SeqVertColoringPar<DefaultStructs>::brooksBiconnectedTest(
	const Graph &graph, typename Graph::PVertex vertExc)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const int CutVert = 0x80000000;

	//v->info - depth of the vertex (0 if not visited)
	//v->info&CutVert - true if v is a cut vertex
	Vert vert = graph.getVert();
	if(vert==vertExc)
		vert = graph.getVertNext(vert);

	int cnt=0;
	vert->info = 1; //first vertex has depth 1
	for(Edge ee = graph.getEdge(vert); ee;
		ee = graph.getEdgeNext(vert, ee))
	{
		Vert vv = graph. getEdgeEnd(ee, vert);
		if(vv == vertExc) continue; //exclude vertExc from graph
		if(vv->info!=0)
			continue;

		cnt++;
		vv->info = 2;
		brooksBiconnectedTest(graph, vertExc, vv);
	}

	bool ans = true;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if((vv->info&CutVert)==0) {
			vv->info = 0; //vv is not a cut vertex
		} else {
			vv->info = 1; //vv is a cut vertex
			ans = false;
		}
	}
	if(cnt>1) {
		vert->info = 1;
		ans = false;
	} else {
		vert->info = 0;
	}
	return ans;
}

template <class DefaultStructs>
template<typename Graph>
int SeqVertColoringPar<DefaultStructs>::brooksBiconnectedTest(
	const Graph &graph, typename Graph::PVertex vertExc, typename Graph::PVertex vert)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const int CutVert = 0x80000000;

	int depth = vert->info&(~CutVert);
	int low = depth;
	for(Edge ee = graph.getEdge(vert); ee; ee = graph.getEdgeNext(vert, ee)) {
		Vert vv = graph.getEdgeEnd(ee, vert);
		if(vv==vertExc) continue; //exclude vertExc from graph
		int d = vv->info&(~CutVert);
		if(d!=0) { //the vertex was visited
			if(low > d)
				low = d;
			continue;
		}

		vv->info = depth+1;
		int m = brooksBiconnectedTest(graph, vertExc, vv);
		if(m<low)
			low = m;
		if(m>=depth) //it's a cut vertex
			vert->info |= CutVert;
	}
	return low;
}

//-------------------- greedy methods---------------------------
template <class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqVertColoringPar<DefaultStructs>::greedy(const Graph &graph,
	ColorMap &colors, typename Graph::PVertex vert)
{
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	if(colors.hasKey(vert) && colors[vert]>=0)
		return -1;

	int deg = graph.getEdgeNo(vert, Mask)+1;
	bool LOCALARRAY(neighCol, deg);
	for(int i = 0; i < deg; i++) neighCol[i] = false;

	for(Edge ee = graph.getEdge(vert, Mask); ee;
		ee = graph.getEdgeNext(vert, ee, Mask))
	{
		typename Graph::PVertex u = graph.getEdgeEnd(ee, vert);
		if(!colors.hasKey(u)) continue;
		int col = colors[u];
		if(col>=0 && col<deg)
			neighCol[ col ] = true;
	}

	int col = 0;
	while( neighCol[col] ) col++;
	return colors[ vert ] = col;
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	typename Graph::PVertex vert)
{
	int maxCol = maxColor(graph, colors);
	return greedyInter(graph, colors, vert, maxCol);
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap >
int SeqVertColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	typename Graph::PVertex vert, int maxCol )
{
	int col = greedy(graph, colors, vert);
	return (col <= maxCol) ? col : colorInterchange(graph , colors, vert);
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::greedy(const Graph &graph, ColorMap &colors, VInIter beg, VInIter end)
{
	int locMax = -1;
	while (beg != end) {
		int col = greedy(graph, colors, *beg++);
		if(col > locMax)
			locMax = col;
	}
	return locMax;
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	VInIter beg, VInIter end)
{
	int locMax = -1, maxCol = maxColor(graph, colors);
	while(beg != end) {
		int col = greedyInter(graph , colors, *beg++, maxCol);
		if (col > maxCol) maxCol = col;
		if (col > locMax) locMax = col;
	}
	return locMax;
}

template <class DefaultStructs>
template< typename Graph, typename ColorMap, typename VInIter >
int SeqVertColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	VInIter beg, VInIter end, int maxCol)
{
	int locMax = -1;
	while(beg != end) {
		int col = greedyInter(graph, colors, *beg++, maxCol);
		if(col > maxCol) maxCol = col;
		if(col > locMax) locMax = col;
	}
	return locMax;
}

template <class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqVertColoringPar<DefaultStructs>::greedy(const Graph &graph, ColorMap &colors)
{
	int locMax = -1;
	for(typename Graph::PVertex vv = graph.getVert(); vv;
		vv = graph.getVertNext(vv))
	{
		int col = greedy(graph, colors, vv);
		if(col > locMax) locMax = col;
	}
	return locMax;
}

template <class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqVertColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	int locMax = -1, maxCol = maxColor(graph, colors);
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv) ) {
		int col = greedyInter(graph, colors, vv, maxCol);
		if(col > locMax) locMax = col;
		if(col > maxCol) maxCol = col;
	}
	return locMax;
}

template <class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqVertColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors, int maxCol)
{
	typedef typename Graph::PVertex Vert;
	int locMax = -1;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		int col = greedyInter(graph ,colors , vv, maxCol);
		if(col > locMax) locMax = col;
		if(col > maxCol) maxCol = col;
	}
	return locMax;
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::lf(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	lenVerts = lfSort(graph, verts, verts+lenVerts);
	return greedy(graph, colors, verts, verts+lenVerts);
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::lfInter(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	lenVerts = lfSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts);
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::lfInter(const Graph &graph,
	ColorMap &colors, int maxCol)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	lenVerts = lfSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts, maxCol);
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::lf(const Graph &graph, ColorMap &colors,
	VInIter beg, VInIter end)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);

	Vert LOCALARRAY(verts, n);
	int lenVerts =0;
	for(; beg!=end; ++beg) {
		if(colors.hasKey(*beg) && colors[*beg]>=0)
			continue;
		verts[lenVerts++] = *beg;
	}

	lenVerts = lfSort(graph, verts, verts+lenVerts);
	return greedy(graph, colors, verts, verts+lenVerts);
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::lfInter(const Graph &graph, ColorMap &colors,
	VInIter beg, VInIter end)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);

	Vert LOCALARRAY(verts, n);
	int lenVerts =0;
	for(; beg!=end; ++beg) {
		if(colors.hasKey(*beg) && colors[*beg]>=0)
			continue;
		verts[lenVerts++] = *beg;
	}

	lenVerts = lfSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts);
}

template <class DefaultStructs>
template < typename Graph, typename ColorMap, typename VInIter >
int SeqVertColoringPar<DefaultStructs>::lfInter(const Graph &graph, ColorMap &colors,
	VInIter beg, VInIter end, int maxCol)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);

	Vert LOCALARRAY(verts, n);
	int lenVerts =0;
	for(; beg!=end; ++beg) {
		if(colors.hasKey(*beg) && colors[*beg]>=0)
			continue;
		verts[lenVerts++] = *beg;
	}

	lenVerts = lfSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts, maxCol);
}

template <class DefaultStructs>
template < typename Graph, typename VInIter, typename VOutIter>
int SeqVertColoringPar<DefaultStructs>::lfSort(const Graph &graph,
	VInIter beg, VInIter end, VOutIter out)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typedef VertDeg<Vert> LfStr;

	int n=0;
	for(VInIter cur=beg; cur!=end; ++cur, ++n);

	LfStr LOCALARRAY(lfStr, n);
	int lenLfStr = 0;

	for(VInIter cur=beg; cur!=end; ++cur) {
		lfStr[lenLfStr++] = LfStr(*cur, graph.deg(*cur, Mask));
	}
	DefaultStructs::sort(lfStr, lfStr+lenLfStr, LfCmp<LfStr>());
	lenLfStr = std::unique(lfStr, lfStr+lenLfStr) - lfStr;

	for(int iLfStr = 0; iLfStr<lenLfStr; iLfStr++)
		*(out++) = lfStr[iLfStr].v;

	return lenLfStr;
}

//-------------------- SL methods---------------------------
//sl(Graph, ColorMap) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::sl(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	lenVerts = slSort(graph, verts, verts+lenVerts);
	return greedy(graph, colors, verts, verts+lenVerts);
}

//slInter(Graph, ColorMap) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::slInter(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	lenVerts = slSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts);
}

//slInter(Graph, ColorMap, int) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::slInter(const Graph &graph,
	ColorMap &colors, int maxCol)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	lenVerts = slSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts, maxCol);
}

//sl(Graph, ColorMap, VInIter, VInIter) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::sl(const Graph &graph, ColorMap &colors,
	VInIter beg, VInIter end)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);

	Vert LOCALARRAY(verts, n);
	int lenVerts = 0;
	for(; beg!=end; ++beg) {
		if(colors.hasKey(*beg) && colors[*beg]>=0)
			continue;
		verts[lenVerts++] = *beg;
	}

	lenVerts = slSort(graph, verts, verts+lenVerts);
	return greedy(graph, colors, verts, verts+lenVerts);
}

//slInter(Graph, ColorMap, VInIter, VInIter) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::slInter(const Graph &graph,
	ColorMap &colors, VInIter beg, VInIter end)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);

	Vert LOCALARRAY(verts, n);
	int lenVerts =0;
	for(; beg!=end; ++beg) {
		if(colors.hasKey(*beg) && colors[*beg]>=0)
			continue;
		verts[lenVerts++] = *beg;
	}

	lenVerts = slSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts);
}

//slInter(Graph, ColorMap, VInIter, VInIter, int) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::slInter(const Graph &graph,
	ColorMap &colors, VInIter beg, VInIter end, int maxCol)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);

	Vert LOCALARRAY(verts, n);
	int lenVerts =0;
	for(; beg!=end; ++beg) {
		if(colors.hasKey(*beg) && colors[*beg]>=0)
			continue;
		verts[lenVerts++] = *beg;
	}

	lenVerts = slSort(graph, verts, verts+lenVerts);
	return greedyInter(graph, colors, verts, verts+lenVerts, maxCol);
}

template <class DefaultStructs>
template < typename Graph, typename VInIter, typename VOutIter>
int SeqVertColoringPar<DefaultStructs>::slSort(const Graph &graph,
	VInIter beg, VInIter end, VOutIter out)
{
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;

	int n=0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);

	Vert LOCALARRAY(tab, n);
	for(int i=0; i<n; ++beg, ++i)
		tab[i] = *beg;

	n = slSort(graph, tab, tab+n);
	for(int i=0; i<n; i++)
		*(out++) = tab[i];

	return n;
}

//-------------------- SLF methods---------------------------
//slf(Graph, ColorMap) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::slf(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	return slf(graph, colors, verts, verts+lenVerts);
}

//slfInter(Graph, ColorMap) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::slfInter(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	return slfInter(graph, colors, verts, verts+lenVerts);
}

//slfInter(Graph, ColorMap, int) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::slfInter(const Graph &graph,
	ColorMap &colors, int maxCol)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int lenVerts = 0;
	Vert LOCALARRAY(verts, graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(colors.hasKey(vv) && colors[vv]>=0)
			continue;
		verts[lenVerts++] = vv;
	}

	return slfInter(graph, colors, verts, verts+lenVerts, maxCol);
}

//slf(Graph, ColorMap, VInIter, VInIter) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::slf(const Graph &graph,
	ColorMap &colors, VInIter beg, VInIter end)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	typedef VertDegSat<Vert> SlfStruct;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typedef typename DefaultStructs::template
		HeapCont< SlfStruct, SlfCmp<SlfStruct> >::Type PriQueue;
	typedef typename DefaultStructs::template
		AssocCont<Vert, typename PriQueue::Node*>::Type VertToQueue;
	typedef typename DefaultStructs::template
		AssocCont<Vert, Set<int> >::Type VertColNeigh;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);
	assert(beg!=end);
	PriQueue priQueue;

	VertToQueue vertToQueue(n);
	VertColNeigh vertColNeigh(n);

	Set< int > colNeigh;
	for(VInIter cur = beg; cur!=end; ++cur) {
		if(colors.hasKey(*cur) && colors[*cur]>=0)
			continue;
		if(vertToQueue.hasKey(*cur))
			continue;
		colNeigh.clear();
		Vert vv = *cur;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(!colors.hasKey(u))
				continue;
			int col = colors[u];
			if( col>=0 )
				colNeigh += col;
		}

		vertColNeigh[vv] = colNeigh; //all adjacent colors
		vertToQueue[vv] = priQueue.push(
			SlfStruct(vv, graph.deg(vv, Mask), colNeigh.size()) );
	}

	int locMax = -1;
	while(priQueue.size()>0) {
		SlfStruct t = priQueue.top();
		priQueue.pop();
		Vert vv = t.v;
		vertToQueue.delKey(vv);

		if(colors.hasKey(vv) && colors[vv]>=0)
			continue; //just in case - maybe it should be changed to throw

		int col = greedy(graph, colors, vv);
		if(col>locMax) locMax = col;

		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(!vertToQueue.hasKey(u) || vertColNeigh[u].isElement(col) )
				continue;
			vertColNeigh[u] += col;
			typename PriQueue::Node *node = vertToQueue[u];
			SlfStruct tmp = node->get();
			++tmp.sat;
			priQueue.decrease(node, tmp);
		}
	}
	return locMax;
}

//slfInter(Graph, ColorMap, VInIter, VInIter) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::slfInter(const Graph &graph,
	ColorMap &colors, VInIter beg, VInIter end)
{
	return slfInter(graph, colors, beg, end, maxColor(graph, colors));
}

//slfInter(Graph, ColorMap, VInIter, VInIter, int) series
template<class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int SeqVertColoringPar<DefaultStructs>::slfInter(const Graph &graph,
	ColorMap &colors, VInIter beg, VInIter end, int maxCol)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	typedef VertDegSat<Vert> SlfStruct;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typedef typename DefaultStructs::template
		HeapCont< SlfStruct, SlfCmp<SlfStruct> >::Type PriQueue;
	typedef typename DefaultStructs::template
		AssocCont<Vert, typename PriQueue::Node*>::Type VertToQueue;
	typedef typename DefaultStructs::template
		AssocCont<Vert, Set<int> >::Type VertColNeigh;

	int n = 0;
	for(VInIter cur = beg; cur!=end; ++cur, ++n);
	assert(beg!=end);
	PriQueue priQueue;
	VertToQueue vertToQueue(n);
	VertColNeigh vertColNeigh(n);

	Set< int > colNeigh;
	for(VInIter cur = beg; cur!=end; ++cur) {
		if(colors.hasKey(*cur) && colors[*cur]>=0)
			continue;
		if(vertToQueue.hasKey(*cur))
			continue;
		colNeigh.clear();
		Vert vv = *cur;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(!colors.hasKey(u))
				continue;
			int col = colors[u];
			if( col>=0 )
				colNeigh += col;
		}

		vertColNeigh[vv] = colNeigh;
		vertToQueue[vv] = priQueue.push(
			SlfStruct(vv, graph.deg(vv, Mask), colNeigh.size()) );
	}

	int locMax = -1;
	while(priQueue.size()>0) {
		SlfStruct t = priQueue.top();
		priQueue.pop();
		Vert vv = t.v;
		vertToQueue.delKey(vv);

		if(colors.hasKey(vv) && colors[vv]>=0)
			continue; //just in case - maybe it should be changed to throw

		int col = greedy(graph, colors, vv);
		if( col>maxCol ) { //new color exceeds maxCol
			col = colorInterchange(graph, colors, vv);
			if(col>maxCol) { //if the graph can not be recolored
				maxCol = col;
			} else { //if the graph was recolored
				if(col>locMax) locMax = col;
				if(priQueue.size()==0)
					return locMax;
				//recreate queue
				priQueue.clear();
				vv = vertToQueue.firstKey();
				while(1) {
					colNeigh.clear();
					for(Edge ee = graph.getEdge(vv, Mask); ee;
						ee = graph.getEdgeNext(vv, ee, Mask))
					{
						Vert u = graph.getEdgeEnd(ee, vv);
						if(!colors.hasKey(u))
							continue;
						int col = colors[u];
						if( col>=0 )
							colNeigh += col;
					}

					vertColNeigh[vv] = colNeigh;
					vertToQueue[vv] = priQueue.push(
						SlfStruct(vv, graph.deg(vv, Mask), colNeigh.size()) );

					if(vertToQueue.lastKey()==vv)
						break;
					vv = vertToQueue.nextKey(vv);
				}
				continue; //next element from the queue
			}
		}

		if(col>locMax) locMax = col;

		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(!vertToQueue.hasKey(u) || vertColNeigh[u].isElement(col) )
				continue;
			vertColNeigh[u] += col;
			typename PriQueue::Node *node = vertToQueue[u];
			SlfStruct tmp = node->get();
			++tmp.sat;
			priQueue.decrease(node, tmp);
		}
	}
	return locMax;
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqVertColoringPar<DefaultStructs>::brooks(const Graph &graph,
	ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	BrooksState<Graph, ColorMap> bState(graph, colors);

	for(Vert vert = graph.getVert(); vert;
		vert = graph.getVertNext(vert))
	{
		if(bState.vertDepth.hasKey(vert))
			continue;
		bState.vertDepth[vert] = 1;
		Edge ee = graph.getEdge(vert, Mask);
		if(ee==NULL) { //isolated vertex
			bState.colors[vert] = 0;
			continue;
		}

		for(; ee; ee = graph.getEdgeNext(vert, ee, Mask)) {
			Vert vv = graph. getEdgeEnd(ee, vert);
			if(bState.vertDepth.hasKey(vv))
				continue;
			brooks(bState, vv, 1);
			bState.vertStack[bState.curVertStack++] = vert;
			bState.biconnected(0, 0);
			brooksBiconnected(bState);
		}
	}
	return maxColor(graph, colors);
}

//---------------------end of SeqVertColoringPar--------------------------

//--------------------begin of GisVertColoringPar-------------------------
//calculate maximal independence set; @return the set cardinality
template <class DefaultStructs>
template<typename Graph, typename ColorMap>
int GisVertColoringPar<DefaultStructs>::color(const Graph &graph,
	ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int lenVerts = graph.getVertNo();

	typedef typename DefaultStructs::template
		LocalGraph<Vert, Koala::EmptyEdgeInfo, Undirected, false>::Type Subgraph;
	typedef typename Subgraph::PVertex VertSub;
	typedef typename Subgraph::PEdge EdgeSub;

	Subgraph subgraph;
	typedef typename DefaultStructs::template
		AssocCont<Vert, VertSub>::Type Map;

	Map map(graph.getVertNo());

	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		VertSub uu = subgraph.addVert(vv);
		map[vv] = uu;
	}

	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		Vert v1 = graph.getEdgeEnd1(ee);
		Vert v2 = graph.getEdgeEnd2(ee);
		subgraph.addEdge(map[v1], map[v2]);
	}

	int col = -1;
	Subgraph procGraph;
	while(subgraph.getVertNo()>0) {
		++col;

		procGraph.clear();
		procGraph.copy(subgraph,
			std::make_pair( stdChoose( true ),stdChoose( true ) ),
			std::make_pair( stdCast(),stdCast() ));

		while(procGraph.getVertNo()>0) {
			std::pair<VertSub,int> minDeg = procGraph.minDeg();

			Vert vert = procGraph.getVertInfo( minDeg.first );

			colors[vert] = col;
			VertSub vv = map[vert];
			subgraph.delVert(vv);

			vv = minDeg.first;
			EdgeSub ee;
			while( (ee = procGraph.getEdge(vv, Mask))!=NULL ) {
				VertSub uu = procGraph.getEdgeEnd(ee, vv);
				procGraph.delVert(uu);
			}
			procGraph.delVert(vv);
		}
	}
	return col;
}

template <class DefaultStructs>
template<typename Graph, typename ColorMap, typename VInIter>
int GisVertColoringPar<DefaultStructs>::color(const Graph &graph,
	ColorMap &colors, VInIter beg, VInIter end)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int lenVerts = graph.getVertNo();

	typedef Koala::Graph<Vert, Koala::EmptyEdgeInfo> Subgraph;
	typedef typename Subgraph::PVertex VertSub;
	typedef typename Subgraph::PEdge EdgeSub;

	Subgraph subgraph;
	typedef typename DefaultStructs::template
		AssocCont<Vert, VertSub>::Type Map;

	Map map(graph.getVertNo());

	for(;beg!=end; ++beg) {
		VertSub uu = subgraph.addVert(*beg);
		map[*beg] = uu;
	}

	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		Vert v1 = graph.getEdgeEnd1(ee);
		Vert v2 = graph.getEdgeEnd2(ee);
		subgraph.addEdge(map[v1], map[v2]);
	}

	int col = -1;
	Subgraph procGraph;
	while(subgraph.getVertNo()>0) {
		++col;

		procGraph.clear();
		procGraph.copy(subgraph,
			std::make_pair( stdChoose( true ),stdChoose( true ) ),
			std::make_pair( stdCast(),stdCast() ));

		while(procGraph.getVertNo()>0) {
			std::pair<VertSub,int> minDeg = procGraph.minDeg();

			Vert vert = procGraph.getVertInfo( minDeg.first );

			colors[vert] = col;
			VertSub vv = map[vert];
			subgraph.delVert(vv);

			vv = minDeg.first;
			EdgeSub ee;
			while( (ee = procGraph.getEdge(vv, Mask))!=NULL ) {
				VertSub uu = procGraph.getEdgeEnd(ee, vv);
				procGraph.delVert(uu);
			}
			procGraph.delVert(vv);
		}
	}
	return col;
}

//----------------------end of MaxISetColoring-------------------------

//--------------------begin of EdgeColoringTest-------------------------

template<class DefaultStructs>
template <typename Graph, typename ColorMap>
int EdgeColoringTest<DefaultStructs>::maxColor(const Graph &graph, const ColorMap &colors)
{
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int col = -1;
	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask) )
	{
		if(!colors.hasKey(ee)) continue;
		int tmp = colors[ee];
		if(tmp>col) col = tmp;
	}
	return col;
}

template<class DefaultStructs>
template <typename Graph, typename ColorMap>
bool EdgeColoringTest<DefaultStructs>::testPart(const Graph &graph, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int degree = graph.Delta(Mask);
	int LOCALARRAY(kolory, degree+1);
	int lenKolory = 0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		lenKolory = 0;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(!colors.hasKey(ee)) continue;
			int col = colors[ee];
			if(col<0) continue;
			kolory[lenKolory++] = col;
		}
		DefaultStructs::sort(kolory, kolory+lenKolory);
		int tmpLen = std::unique(kolory, kolory+lenKolory)-kolory;
		if(tmpLen!=lenKolory)
			return false;
	}
	return true;
}

template<class DefaultStructs>
template <typename Graph, typename ColorMap>
bool EdgeColoringTest<DefaultStructs>::test(const Graph &graph, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int degree = graph.Delta(Mask);
	int LOCALARRAY(kolory, degree+1);
	int lenKolory = 0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		lenKolory = 0;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(!colors.hasKey(ee)) return false;
			int col = colors[ee];
			if(col<0) return false;
			kolory[lenKolory++] = col;
		}
		DefaultStructs::sort(kolory, kolory+lenKolory);
		int tmpLen = std::unique(kolory, kolory+lenKolory)-kolory;
		if(tmpLen!=lenKolory)
			return false;
	}
	return true;
}

//----------------------end of EdgeColoringTest-------------------------

//--------------------begin of SeqEdgeColoring-------------------------

// state elements
template <class DefaultStructs> template<typename Graph, typename ColorMap>
SeqEdgeColoringPar<DefaultStructs>::VizingState<Graph,ColorMap>::
VizingState(const Graph &g, ColorMap &c, int maxCol):
	graph(g), delta(g.Delta(EdDirIn|EdDirOut|EdUndir)),
	colors(c), notColored(maxCol), vertToTab( g.getVertNo() ),
	edgeToList( g.getEdgeNo(EdDirIn|EdDirOut|EdUndir) )
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	tabVert = new TabVert[ g.getVertNo() ];
	colorToList = new int[ maxCol+1 ];
	listEdgeCol = new ListEdgeCol[ g.getEdgeNo(Mask)+1 ];
	maxColor = -1;

	//init all lists
	int ii = 0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		vertToTab[vv] = ii;
		tabVert[ii++].freeColor = 0;
	}
	for(int i=0; i<notColored; i++)
		colorToList[i] = 0;
	colorToList[notColored] = 1;
	ii=1; //we assume that colors are not set to any edge
	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask), ii++)
	{
		edgeToList[ee] = ii;
		listEdgeCol[ii].next = ii+1;
		listEdgeCol[ii].prev = ii-1;
		listEdgeCol[ii].edge = ee;
	}
	listEdgeCol[ii-1].next = 0;
	if(colors.size()<=0) return;

	int LOCALARRAY(tmpTab, notColored);
	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		if(!colors.hasKey(ee)) continue;
		int tmpCol = colors[ee];
		if(tmpCol<0 || tmpCol>=notColored)
			continue;

		changeColor(ee, notColored, tmpCol);
		if(maxColor<tmpCol) maxColor = tmpCol;
	}
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
SeqEdgeColoringPar<DefaultStructs>::VizingState<Graph,ColorMap>::
~VizingState()
{
	delete [] tabVert;
	delete [] colorToList;
	delete [] listEdgeCol;
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
void SeqEdgeColoringPar<DefaultStructs>::VizingState<Graph,ColorMap>::
setColor(typename Graph::PEdge edge, int color)
{
	int oldColor = notColored;
	if(colors.hasKey(edge)) {
		oldColor = colors[edge];
		if(oldColor<0 || notColored<=oldColor)
			oldColor = notColored;
	}
	changeColor(edge, oldColor, color);
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
void SeqEdgeColoringPar<DefaultStructs>::VizingState<Graph,ColorMap>::
changeColor(typename Graph::PEdge edge, int oldColor, int newColor)
{
	int id = edgeToList[edge];
	int next = listEdgeCol[id].next;
	int prev = listEdgeCol[id].prev;
	if(colorToList[oldColor]==id)
		colorToList[oldColor]=next;
	listEdgeCol[next].prev = prev;
	listEdgeCol[prev].next = next;
	listEdgeCol[id].next = next = colorToList[newColor];
	listEdgeCol[id].prev = 0;
	listEdgeCol[next].prev = id;
	colorToList[newColor] = id;
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
void SeqEdgeColoringPar<DefaultStructs>::VizingState<Graph,ColorMap>::
subgraph(int color1, int color2)
{
	typedef typename Graph::PEdge Edge;
	for(int i=0; i<graph.getVertNo(); i++)
		tabVert[i].vc[0] = tabVert[i].vc[1] = -1;

	colSubgraph[0] = color1;
	colSubgraph[1] = color2;

	for(int i=0; i<2; i++) {
		int iCol = colorToList[ colSubgraph[i] ];
		while(iCol!=0) {
			Edge ee = listEdgeCol[iCol].edge;
			int iv1 = vertToTab[ graph.getEdgeEnd1(ee) ];
			int iv2 = vertToTab[ graph.getEdgeEnd2(ee) ];
			tabVert[iv1].vc[i] = iv2;
			tabVert[iv2].vc[i] = iv1;
			tabVert[iv1].ec[i] = tabVert[iv2].ec[i] = ee;
			iCol = listEdgeCol[iCol].next;
		}
	}
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::VizingState<Graph,ColorMap>::
altPathWalk(int ivert, int col)
{
	int tmp = ivert;
	int iCol = (col==colSubgraph[0]) ? 0 : 1;
	assert(colSubgraph[iCol]==col);
	while(tabVert[ivert].vc[iCol]>=0) {
		ivert = tabVert[ivert].vc[iCol];
		assert(ivert!=tmp);
		iCol ^= 1;
	}
	return ivert;
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::VizingState<Graph,ColorMap>::
altPathRecoloring(int ivert, int col)
{
	typedef typename Graph::PEdge Edge;
	int iCol = (col==colSubgraph[0]) ? 0 : 1;
	assert(colSubgraph[iCol]==col);
	assert(tabVert[ivert].vc[iCol^1]<0); //it's a begining of the alternating path

	tabVert[ivert].freeColor = colSubgraph[iCol];
	if(tabVert[ivert].vc[iCol]>=0) {
		while(tabVert[ivert].vc[iCol]>=0) {
			Edge ee = tabVert[ivert].ec[iCol];
			int oldColor = colSubgraph[iCol];
			int newColor = colSubgraph[iCol^1];
			colors[ee] = newColor;
			changeColor(ee, oldColor, newColor);

			ivert = tabVert[ivert].vc[iCol];
			iCol ^= 1;
		}
		tabVert[ivert].freeColor = colSubgraph[iCol^1];
	}
	return ivert;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
void SeqEdgeColoringPar<DefaultStructs>::vizingSimple(
	typename SeqEdgeColoringPar<DefaultStructs>::template VizingState<Graph, ColorMap> &vState,
	typename Graph::PEdge edge)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int degree = vState.graph.Delta(Mask);
	//now graph is partial colored, now we can use Vizing algorithm
	int idEdge;
	int LOCALARRAY(tmpTab, vState.notColored);
	Edge LOCALARRAY(colorsToEdge1, vState.notColored);
	Edge LOCALARRAY(colorsToEdge2, vState.notColored);
	Edge LOCALARRAY(fan, degree); //as in Vizing's proof (colors that are used in fan)

	idEdge = vState.edgeToList[edge];
	Vert vert1 = vState.graph.getEdgeEnd1(edge);
	Vert vert2 = vState.graph.getEdgeEnd2(edge);
	int idVert1 = vState.vertToTab[vert1];
	int idVert2 = vState.vertToTab[vert2];

	for(int i=0; i<vState.notColored; i++)
		colorsToEdge1[i] = colorsToEdge2[i] = NULL;
	for(Edge e1 = vState.graph.getEdge(vert1, Mask); e1;
		e1 = vState.graph.getEdgeNext(vert1, e1, Mask))
	{
		if(!vState.colors.hasKey(e1)) continue;
		int tmpCol = vState.colors[e1];
		if(tmpCol<0 || tmpCol>=vState.notColored) continue;
		colorsToEdge1[ tmpCol ] = e1;
	}
	for(Edge e2 = vState.graph.getEdge(vert2, Mask); e2;
		e2 = vState.graph.getEdgeNext(vert2, e2, Mask))
	{
		if(!vState.colors.hasKey(e2)) continue;
		int tmpCol = vState.colors[e2];
		if(tmpCol<0 || tmpCol>=vState.notColored) continue;
		colorsToEdge2[ tmpCol ] = e2;
	}
	//making fan at vertex vert1
	for(int i=0; i<vState.notColored; i++) tmpTab[i] = -1;
	int fanLen = 0;
	int colFree = vState.tabVert[idVert2].freeColor;
	fan[fanLen++] = edge;
	Vert vv;
	//tmpTab is used to check if there are the same free colors at the end of each edge in the fan
	while( colorsToEdge1[colFree]!=NULL && tmpTab[colFree]<0 ) {
		tmpTab[colFree] = fanLen;
		fan[fanLen++] = colorsToEdge1[colFree]; //colFree;
		vv = vState.graph.getEdgeEnd(colorsToEdge1[colFree], vert1);
		colFree = vState.tabVert[ vState.vertToTab[vv] ].freeColor;
	}
	int ii;
	if(tmpTab[colFree]<0) {
		// only change colors in fan
		for(int iFan = fanLen-1; iFan>0; --iFan) {
			Edge ee = fan[iFan];
			int colOld = vState.colors[ee];

			vState.colors[ ee ] = colFree;
			vState.changeColor(ee, colOld, colFree);
			colorsToEdge1[colFree] = ee;
			if(colFree > vState.maxColor )
				vState.maxColor = colFree;

			int iv = vState.vertToTab[ vState.graph.getEdgeEnd(ee, vert1) ];
			vState.tabVert[iv].freeColor = colOld;

			colFree = colOld;
		}
		vState.colors[edge] = colFree;
		vState.changeColor(edge, vState.notColored, colFree);
		colorsToEdge1[colFree] = edge;
		colorsToEdge2[colFree] = edge;

		//fix freeColor on vertices vert1 and vert2
		for(ii=0; ii<degree; ii++)
			if(colorsToEdge1[ii]==NULL) break;
		vState.tabVert[idVert1].freeColor = ii;
		for(ii=0; ii<degree; ii++)
			if(colorsToEdge2[ii]==NULL) break;
		vState.tabVert[idVert2].freeColor = ii;
		return;
	}
	//here tmpTab[colFree]>=0
	//  path recoloring colors vState.vertFreeCol[vert1] and colFree
	//subgraph creation
	int color[2] = {vState.tabVert[ idVert1 ].freeColor, colFree};
	vState.subgraph(color[0], color[1]);
	//travel from vertex vert1...
	int iv = vState.altPathWalk(idVert1, color[1]);

	//in fan there are two edge ends which missed the same color
	//we check if one of them is connected by alternating path with vertex vert1
	int endVert;
	endVert = vState.vertToTab[ vState.graph.getEdgeEnd( fan[ tmpTab[colFree]-1 ], vert1) ];

	if(iv==endVert) {
		endVert = vState.vertToTab[vv];
	} else {
		fanLen = tmpTab[colFree];
	}
	//change colors by path
	vState.altPathRecoloring(endVert, color[0]);

	colFree = color[0]; //it's equal to tabVert[ idVert1 ].freeColor
	// change colors in fan
	for(int iFan = fanLen-1; iFan>0; --iFan) {
		Edge ee = fan[iFan];
		int colOld = vState.colors[ee];

		vState.colors[ ee ] = colFree;
		vState.changeColor(ee, colOld, colFree);
		colorsToEdge1[colFree] = ee;
		if(colFree > vState.maxColor)
			vState.maxColor = colFree;

		int iv = vState.vertToTab[ vState.graph.getEdgeEnd(ee, vert1) ];
		vState.tabVert[iv].freeColor = colOld;

		colFree = colOld;
	}
	vState.colors[edge] = colFree;
	vState.changeColor(edge, vState.notColored, colFree);
	colorsToEdge1[colFree] = edge;
	colorsToEdge2[colFree] = edge;

	//fix freeColor on vertices vert1 and vert2
	for(int i=0; i<vState.notColored; i++) tmpTab[i] = 0;
	for(Edge e1 = vState.graph.getEdge(vert1, Mask); e1;
		e1 = vState.graph.getEdgeNext(vert1, e1, Mask))
	{
		if(!vState.colors.hasKey(e1)) continue;
		int tmpCol = vState.colors[e1];
		if(tmpCol<0 || tmpCol>=vState.notColored) continue;
		tmpTab[ tmpCol ] = 1;
	}
	for(Edge e2 = vState.graph.getEdge(vert2, Mask); e2;
		e2 = vState.graph.getEdgeNext(vert2, e2, Mask))
	{
		if(!vState.colors.hasKey(e2)) continue;
		int tmpCol = vState.colors[e2];
		if(tmpCol<0 || tmpCol>=vState.notColored) continue;
		tmpTab[ tmpCol ] |= 2;
	}
	for(ii=0; ii<vState.notColored; ii++)
		if((tmpTab[ii]&1)==0) break;
	vState.tabVert[idVert1].freeColor = ii;
	for(ii=0; ii<vState.notColored; ii++)
		if((tmpTab[ii]&2)==0) break;
	vState.tabVert[idVert2].freeColor = ii;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
void SeqEdgeColoringPar<DefaultStructs>::vizing(
	typename SeqEdgeColoringPar<DefaultStructs>::template VizingState<Graph, ColorMap> &vState,
	typename Graph::PEdge edge)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int vertNo = vState.graph.getVertNo();
	int degree = vState.graph.Delta(Mask);

	int freeColorX; //color not incident to X; always freeColorX <= maxColor
	Edge LOCALARRAY(usedColorX, vState.notColored); //edges incident to X (by colors)
	int LOCALARRAY(usedColorY, vState.notColored);  //if colors are incident to Y_k (yes/no)
	int LOCALARRAY(freeColorYY, vState.notColored); /*freeColorYY[i]==j
			means that color i is not incident with the vertex
			described in fan[j] (fan[j]- edge (v,u) one vertex is vertX and second is the described one)*/
	Edge LOCALARRAY(fan, degree); //as in Vizing's proof (edges that are used in fan)
	int fanLen, ii;

	int idEdge = vState.edgeToList[edge];
	Vert vertX = vState.graph.getEdgeEnd1(edge);
	Vert vertY = vState.graph.getEdgeEnd2(edge);
	int idVertX = vState.vertToTab[vertX];
	int idVertY = vState.vertToTab[vertY];

	for(ii=0; ii<=vState.maxColor; ii++)
		usedColorX[ii] = NULL;
	for(Edge eX = vState.graph.getEdge(vertX, Mask); eX;
		eX = vState.graph.getEdgeNext(vertX, eX, Mask))
	{
		if(!vState.colors.hasKey(eX)) continue;
		int tmpCol = vState.colors[eX];
		if(tmpCol<0 || tmpCol>=vState.notColored) continue;
		usedColorX[ tmpCol ] = eX;
	}
	for(ii=0; ii<=vState.maxColor; ii++)
		if(usedColorX[ii]==NULL) break;
	freeColorX = ii;

	for(int i=0; i<=vState.maxColor; i++)
		freeColorYY[i] = -1; //clear all free color of all Y_k (for all k)
	fanLen = 0;
	//long while
	while(1) { //creating fan
		vertY = vState.graph.getEdgeEnd(edge, vertX); //take Y_k
		for(int i=0; i<=vState.maxColor; i++) //reset used colors
			usedColorY[i] = 0;
		for(Edge eY = vState.graph.getEdge(vertY, Mask); eY;
			eY = vState.graph.getEdgeNext(vertY, eY, Mask))
		{
			if(!vState.colors.hasKey(eY)) continue;
			int tmpCol = vState.colors[eY];
			if(tmpCol<0 || tmpCol>=vState.notColored) continue;
			usedColorY[ tmpCol ] = 1;
		}

		fan[fanLen++] = edge; //create new entry in fan of X

		//check if fan recoloring is enough
		for(ii=0; ii<=vState.maxColor; ii++) {
			if(usedColorY[ii]!=0 || usedColorX[ii]!=NULL)
				continue;
			//if there is unused color in X and Y_k:
			if(fanLen>1) {
				int j = fanLen-1;
				do {
					int prevColor = vState.colors[ fan[j] ];
					vState.changeColor(fan[j], prevColor, ii);
					vState.colors[ fan[j] ] = ii;
					ii = prevColor;
				} while( (j=freeColorYY[ii])>0 ); //recoloring in fan (2)
			}
			vState.changeColor(fan[0], vState.notColored, ii);
			vState.colors[ fan[0] ] = ii;
			return;
		}

		for(ii=0; ii<=vState.maxColor; ii++) {
			if( usedColorY[ii]>0 ) continue; //we don't watch colors from neighbourhood of Y
			if( freeColorYY[ii]>=0 ) { //there are 2 vertices in fan that miss color 'ii'
				//a)recoloring by path; [next b)recoloring by fan]
				//subgraph creation
				vState.subgraph(freeColorX, ii);
				//travel from vertex vertX...
				int iV = vState.altPathWalk(idVertX, ii);
				int endVert = vState.vertToTab[ vState.graph.getEdgeEnd( fan[ freeColorYY[ii] ] , vertX) ];
				if(iV==endVert) {
					endVert = vState.vertToTab[ vertY ];
					fanLen--;
				} else {
					fanLen = freeColorYY[ii];
				}
				//path recoloring (starts in endVert)
				vState.altPathRecoloring(endVert, freeColorX);

				// fan recoloring
				ii = freeColorX;
				for(int i=fanLen; i>0;) { //recoloring in fan (2)
					int prevColor = vState.colors[ fan[i] ];
					vState.changeColor(fan[i], prevColor, ii);
					vState.colors[ fan[i] ] = ii;
					ii = prevColor;
					i = freeColorYY[ii];
				}
				vState.changeColor(fan[0], vState.notColored, ii);
				vState.colors[ fan[0] ] = ii;
				return; //double break
			} else //add information about free colors in Y_k
				freeColorYY[ii] = fanLen-1;
		}

		//we search edge incident to X that has color missed to edges of the fan
		for(Edge ee = vState.graph.getEdge(vertX, Mask); ee;
			ee = vState.graph.getEdgeNext(vertX, ee, Mask))
		{
			Vert v = vState.graph.getEdgeEnd(ee, vertX);
			vState.tabVert[ vState.vertToTab[v] ].vc[0] = 0;
		}
		for(ii=0; ii<fanLen; ii++) {
			Vert v = vState.graph.getEdgeEnd(fan[ii], vertX);
			vState.tabVert[ vState.vertToTab[v] ].vc[0] = 1;
		}
		for(ii=0; ii<=vState.maxColor; ii++) {
			if( freeColorYY[ii]<0 || usedColorX[ii]==NULL ) continue;
			Vert v = vState.graph.getEdgeEnd( usedColorX[ii], vertX );
			if( vState.tabVert[ vState.vertToTab[v] ].vc[0]==1 )
				continue;
			edge = usedColorX[ii];
			break;
		}
		if(ii>vState.maxColor) {
			//we use new color ( ++maxColor )
			++vState.maxColor;
			vState.changeColor(fan[0], vState.notColored, vState.maxColor);
			vState.colors[ fan[0] ] = vState.maxColor;
			return;
		}
	}
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::colorInterchange(const Graph &graph, ColorMap &colors,
	typename Graph::PEdge edge)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int oldColor = colors[ edge ];
	colors.delKey( edge );

//	typename DefaultStructs::template
//		AssocCont<Vert, int>::Type compMap(graph.getVertNo());
	Vert vert1 = graph.getEdgeEnd1(edge);
	Vert vert2 = graph.getEdgeEnd2(edge);
	int deg = graph.deg(vert1, Mask) + graph.deg(vert2, Mask);
	char LOCALARRAY(matchedColors, deg);
	for(int i=0; i<deg; i++) matchedColors[i] = 0;

	for(Edge ee = graph.getEdge(vert1, Mask); ee;
		ee = graph.getEdgeNext(vert1, ee, Mask))
	{
		if(!colors.hasKey(ee)) continue;
		int col = colors[ee];
		if(col<0 || col>=deg) continue;
		matchedColors[col] |= 1;
	}
	for(Edge ee = graph.getEdge(vert2, Mask); ee;
		ee = graph.getEdgeNext(vert2, ee, Mask))
	{
		if(!colors.hasKey(ee)) continue;
		int col = colors[ee];
		if(col<0 || col>=deg) continue;
		matchedColors[col] |= 2;
	}

	VizingState<Graph, ColorMap> vState(graph, colors, oldColor);
	int idVert1 = vState.vertToTab[vert1];
	int idVert2 = vState.vertToTab[vert2];
	for(int c1 = 0; c1<oldColor; c1++) {
		for(int c2 = c1+1; c2<oldColor; c2++)
		{
			if((matchedColors[c1]&1)!=0 && (matchedColors[c2]&1)!=0)
				continue;
			if((matchedColors[c1]&2)!=0 && (matchedColors[c2]&2)!=0)
				continue;
			int begPath, endPath, colPath;

			if((matchedColors[c1]&1)!=0) {
				begPath = idVert1;
				endPath = idVert2;
				colPath = c1;
			} else if((matchedColors[c2]&1)!=0) {
				begPath = idVert1;
				endPath = idVert2;
				colPath = c2;
			} else if((matchedColors[c1]&2)!=0) {
				begPath = idVert2;
				endPath = idVert1;
				colPath = c1;
			} else if((matchedColors[c2]&2)!=0) {
				begPath = idVert2;
				endPath = idVert1;
				colPath = c2;
			}

			vState.subgraph(c1, c2);
			if(vState.altPathWalk(begPath, colPath)==endPath)
				continue;

			vState.altPathRecoloring(begPath, colPath);
			colors[edge] = colPath;
			return colPath;
		}
	}

	return colors[edge] = oldColor; //recoloring failed
}
//----------coloring---------
template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::vizingSimple(const Graph &graph,
	ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	int degree = graph.Delta(Mask);
	VizingState<Graph, ColorMap> vState(graph, colors, degree+1);

	int LOCALARRAY(tmpTab, vState.notColored);
	//freeColor set
	if(colors.size()>0) { //tmpTab - is used to checking free color availability
		for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
			for(int i=0; i<vState.notColored; i++)
				tmpTab[i] = 0;
			for(Edge ee=graph.getEdge(vv, Mask); ee;
				ee = graph.getEdgeNext(vv, ee, Mask))
			{
				if(!colors.hasKey(ee)) continue;
				int tmpCol = colors[ee];
				if(tmpCol<0 || tmpCol>=vState.notColored) continue;
				tmpTab[ tmpCol ] = 1;
			}
			int i=0;
			while(i<vState.notColored && tmpTab[i]==1) i++;
			vState.tabVert[ vState.vertToTab[vv] ].freeColor = i;
		}
	}
	//init edges by degree colors
	int ii;
	for(Edge edge = graph.getEdge(Mask); edge;
		edge = graph.getEdgeNext(edge, Mask))
	{
		if(colors.hasKey(edge)&&colors[edge]>=0) continue;
		Vert vert1 = graph.getEdgeEnd1(edge);
		Vert vert2 = graph.getEdgeEnd2(edge);
		for(int i=0; i<degree; i++)
			tmpTab[i] = 0;
		for(Edge ee = graph.getEdge(vert1, Mask); ee;
			ee = graph.getEdgeNext(vert1, ee, Mask))
		{
			if(!colors.hasKey(ee)) continue;
			int tmpCol = colors[ee];
			if(tmpCol<0 || tmpCol>=vState.notColored) continue;
			tmpTab[ tmpCol ] = 1;
		}
		for(Edge ee=graph.getEdge(vert2, Mask); ee;
			ee=graph.getEdgeNext(vert2, ee, Mask))
		{
			if(!colors.hasKey(ee)) continue;
			int tmpCol = colors[ee];
			if(tmpCol<0 || tmpCol>=vState.notColored) continue;
			tmpTab[ tmpCol ] |= 2;
		}

		for(int i=0; i<degree; i++) {
			if(tmpTab[i]!=0) continue;

			colors[edge] = i;
			vState.changeColor(edge, vState.notColored, i);
			tmpTab[i] = 3; // 3=1|2
			if(i > vState.maxColor)
				vState.maxColor = i;
			break;
		}
		//setting free colors at vertices
		for(ii=0; ii<degree; ii++)
			if((tmpTab[ii]&1)==0) break;
		int id = vState.vertToTab[vert1];
		vState.tabVert[id].freeColor = ii;
		for(ii=0; ii<degree; ii++)
			if((tmpTab[ii]&2)==0) break;
		id = vState.vertToTab[vert2];
		vState.tabVert[id].freeColor = ii;
	}
	//edge coloring
	int idFree;
	while( (idFree=vState.colorToList[vState.notColored])>0 ) {
		Edge ee = vState.listEdgeCol[idFree].edge;
		vizingSimple(vState, ee);
	}
	return vState.maxColor;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::vizing(const Graph &graph,
	ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int vertNo = graph.getVertNo();
	int degree = graph.Delta(Mask);
	int mu = graph.mu();
	VizingState<Graph, ColorMap> vState(graph, colors, degree+mu);
	if(degree-1>vState.maxColor)
		vState.maxColor = degree-1;

	//edge coloring
	int idFree;
	while( (idFree=vState.colorToList[vState.notColored])>0 ) {
		Edge ee = vState.listEdgeCol[idFree].edge;
		vizing(vState, ee);
	}
	return vState.maxColor;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::vizing(const Graph &graph,
	ColorMap &colors, typename Graph::PEdge edge)
{
	if(colors.hasKey(edge)&&colors[edge]>=0)
		return -1;

	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int vertNo = graph.getVertNo();
	int degree = graph.Delta(Mask);
	int mu = graph.mu();
	VizingState<Graph, ColorMap> vState(graph, colors, degree+mu);
	if(degree-1>vState.maxColor)
		vState.maxColor = degree-1;

	//edge coloring
	vizing(vState, edge);
	return colors[edge];
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::vizing(const Graph &graph,
	ColorMap &colors, typename Graph::PEdge edge, int maxCol)
{
	if(colors.hasKey(edge)&&colors[edge]>=0)
		return -1;

	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int vertNo = graph.getVertNo();
	int degree = graph.Delta(Mask);
	int mu = graph.mu();
	VizingState<Graph, ColorMap> vState(graph, colors, degree+mu);
	vState.maxColor = maxCol;

	//edge coloring
	vizing(vState, edge);
	return colors[edge];
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqEdgeColoringPar<DefaultStructs>::greedy(const Graph &graph, ColorMap &colors,
	typename Graph::PEdge edge)
{
	if(colors.hasKey(edge) && colors[edge]>=0)
		return -1;

	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	Vert v1 = graph.getEdgeEnd1(edge);
	Vert v2 = graph.getEdgeEnd2(edge);

	int deg = graph.deg(v1, Mask)+graph.deg(v2, Mask);
	bool LOCALARRAY(neighCol, deg);
	for(int i = 0; i < deg; i++) neighCol[i] = false;

	for(Edge ee = graph.getEdge(v1, Mask); ee;
		ee = graph.getEdgeNext(v1, ee, Mask))
	{
		if(!colors.hasKey(ee)) continue;
		int col = colors[ee];
		if(col>=0 && col<deg)
			neighCol[ col ] = true;
	}

	for(Edge ee = graph.getEdge(v2, Mask); ee;
		ee = graph.getEdgeNext(v2, ee, Mask))
	{
		if(!colors.hasKey(ee)) continue;
		int col = colors[ee];
		if(col>=0 && col<deg)
			neighCol[ col ] = true;
	}

	int col = 0;
	while( neighCol[col] ) col++;
	return colors[ edge ] = col;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int SeqEdgeColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	typename Graph::PEdge edge)
{
	int maxCol = maxColor(graph, colors);
	return greedyInter(graph, colors, edge, maxCol);
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqEdgeColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	typename Graph::PEdge edge, int maxCol)
{
	int col = greedy(graph, colors, edge);
	return (col <= maxCol) ? col : colorInterchange(graph , colors, edge);
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap, typename EInIter >
int SeqEdgeColoringPar<DefaultStructs>::greedy(const Graph &graph, ColorMap &colors,
	EInIter beg, EInIter end)
{
	int locMax = -1;
	while (beg != end) {
		int col = greedy(graph, colors, *beg++);
		if(col > locMax)
			locMax = col;
	}
	return locMax;
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap, typename EInIter >
int SeqEdgeColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	EInIter beg, EInIter end)
{
	int locMax = -1, maxCol = maxColor(graph, colors);
	while(beg != end) {
		int col = greedyInter(graph , colors, *beg++, maxCol);
		if (col > maxCol) maxCol = col;
		if (col > locMax) locMax = col;
	}
	return locMax;
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap, typename EInIter >
int SeqEdgeColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	EInIter beg, EInIter end, int maxCol)
{
	int locMax = -1;
	while(beg != end) {
		int col = greedyInter(graph, colors, *beg++, maxCol);
		if(col > maxCol) maxCol = col;
		if(col > locMax) locMax = col;
	}
	return locMax;
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqEdgeColoringPar<DefaultStructs>::greedy(const Graph &graph, ColorMap &colors)
{
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int locMax = -1;
	for(typename Graph::PEdge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		int col = greedy(graph, colors, ee);
		if(col > locMax) locMax = col;
	}
	return locMax;
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqEdgeColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors)
{
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int locMax = -1, maxCol = maxColor(graph, colors);
	for(typename Graph::PEdge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		int col = greedyInter(graph, colors, ee, maxCol);
		if(col > locMax) locMax = col;
		if(col > maxCol) maxCol = col;
	}
	return locMax;
}

template<class DefaultStructs>
template< typename Graph, typename ColorMap >
int SeqEdgeColoringPar<DefaultStructs>::greedyInter(const Graph &graph, ColorMap &colors,
	int maxCol)
{
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int locMax = -1;
	for(typename Graph::PEdge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		int col = greedyInter(graph ,colors , ee, maxCol);
		if(col > locMax) locMax = col;
		if(col > maxCol) maxCol = col;
	}
	return locMax;
}
//----------------------end of SeqEdgeColoring-------------------------

//--------------------begin of ListVertColoring-------------------------
template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
bool ListVertColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PVertex vert,
	ColorChooser chooser)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	int res = chooser(graph, colLists[vert], colors, vert);
	if(res<0 || !colLists[vert].isElement(res))
		return false;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	for(Edge ee = graph.getEdge(vert, Mask); ee;
		ee=graph.getEdgeNext(vert,ee, Mask))
	{
		Vert u = graph.getEdgeEnd(ee, vert);
		if(colors.hasKey(u)&&colors[u]==res)
			return false;
	}
	colors[vert] = res;
	return true;
}
//, gdzie Mapa:PVertex->Set<int> a CholorChooser choose to obiekt funkcyjny podany przez uzytkownika, dzialajacy jako:
//int chooser(const Graph&,PVertex V,const& Set<int>) - dla niepusteg zbioru kolorow dozwolnych przy v wybiera jeden element.
//Funkcja color zwraca true jesli udalo sie wybrac poprawny kolor

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListVertColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PVertex vert)
{
	return colorChoose(graph, colLists, colors, vert, FirstFit());
}
//takie samo dzialanie, jakby z funktorem zwracajacym jakikolwiek element dozwolonego zbioru barw

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap,
	typename VIter, typename ColorChooser>
int ListVertColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, VIter beg, VIter end, ColorChooser chooser)
{
	int cnt = 0;
	while(beg!=end) {
		if(!colorChoose(graph, colLists, colors, *beg, chooser))
			return cnt;
		++beg;
		++cnt;
	}
	return cnt;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename VIter>
int ListVertColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, VIter beg, VIter end)
{
	int cnt = 0;
	while(beg!=end) {
		if(!color(graph, colLists, colors, *beg))
			return cnt;
		++beg;
		++cnt;
	}
	return cnt;
}
// - dla zakresow kolejno kolorowanych wierzcholkow.	Zwraca: czy udalo sie pokolorowac caly zakres?

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
int ListVertColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, ColorChooser chooser)
{
	typedef typename Graph::PVertex Vert;
	int cnt=0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(!colorChoose(graph, colLists, colors, vv, chooser))
			return cnt;
		++cnt;
	}
	return cnt;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
int ListVertColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	int cnt=0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(!color(graph, colLists, colors, vv))
			return cnt;
		++cnt;
	}
	return cnt;
}

//testing if graph is properly colored
template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListVertColoringPar<DefaultStructs>::testPart(const Graph &graph,
	const ColLists &colLists, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(!colors.hasKey(vv))
			continue;
		int color = colors[vv];
		if( !colLists[vv].isElement(color) ) //color is not from
			return false;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(u<vv) continue;
			if(colors.hasKey(u)&&colors[u]==color)
				return false;
		}
	}
	return true;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListVertColoringPar<DefaultStructs>::test(const Graph &graph,
	const ColLists &colLists, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if(!colors.hasKey(vv))
			return false;
		int color = colors[vv];
		if( !colLists[vv].isElement(color) ) //color is not from
			return false;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(u<vv) continue;
			if(!colors.hasKey(u)||colors[u]==color)
				return false;
		}
	}
	return true;
}

//extremal elements of the lists
template <class DefaultStructs>
template<typename Graph, typename ColLists>
std::pair<int,int> ListVertColoringPar<DefaultStructs>::listMinMax(
	const Graph &graph, const ColLists &colLists)
{
	typedef typename Graph::PVertex Vert;
	Vert vv = graph.getVert();
	int min = colLists[vv].first(), max = colLists[vv].last();
	vv = graph.getVertNext(vv);
	while(vv) {
		int tmp = colLists[vv].first();
		if(min>tmp) min = tmp;
		tmp = colLists[vv].last();
		if(max<tmp) max = tmp;
		vv = graph.getVertNext(vv);
	}
	return std::make_pair(min, max);
}

//sorted sequence of the numbers used in the lists elements (unique numbers)
//@return the sequence length
template <class DefaultStructs>
template<typename Graph, typename ColLists, typename Iter>
int ListVertColoringPar<DefaultStructs>::listColors(
	const Graph &graph, const ColLists &colLists, Iter out)
{
	typedef typename Graph::PVertex Vert;
	typedef typename ColLists::ValType ListType; //ListType have the Set interface
	Set<int> colSet;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		ListType lt = colLists[vv];
		lt.getElements( setInserter(colSet) );
	}
	return colSet.getElements( out );
}

//set of the numbers used in the lists elements
//ColLists should have interface like AssocTabInterface
template <class DefaultStructs>
template<typename Graph, typename ColLists>
Set<int> ListVertColoringPar<DefaultStructs>::listColorsSet(
	const Graph &graph, const ColLists &colLists)
{
	typedef typename Graph::PVertex Vert;
	typedef typename ColLists::ValType ListType; //ListType have the Set interface
	Set<int> res;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		ListType lt = colLists[vv];
		lt.getElements( setInserter(res) );
	}
	return res;
}

template <class DefaultStructs>
template<typename Graph, typename ColList, typename ColorMap>
int ListVertColoringPar<DefaultStructs>::FirstFit::operator()(
	const Graph &graph, const ColList &colList,
	const ColorMap &colorMap, typename Graph::PVertex vert)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	Set<int> avalColors = colList;
	int maxVal = avalColors.last();
	for(Edge ee = graph.getEdge(vert, Mask); ee;
		ee = graph.getEdgeNext(vert, ee, Mask))
	{
		Vert u = graph.getEdgeEnd(ee, vert);
		if(!colorMap.hasKey(u))
			continue;
		avalColors.del( colorMap[u] );
	}
	if(avalColors.size()>0) return avalColors.first();
	else return maxVal+1;
}

//----------------------end of ListVertColoring-------------------------

//--------------------begin of ListEdgeColoring-------------------------

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
bool ListEdgeColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PEdge edge,
	ColorChooser chooser )
{
	int res = chooser(graph, colLists[edge], colors, edge);
	if(res<0 || !colLists[edge].isElement(res))
		return false;
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	Vert vv = graph.getEdgeEnd1(edge);
	for(Edge ee = graph.getEdge(vv, Mask); ee;
		ee=graph.getEdgeNext(vv,ee, Mask))
	{
		if(colors.hasKey(ee) && colors[ee]==res)
			return false;
	}
	vv = graph.getEdgeEnd2(edge);
	for(Edge ee = graph.getEdge(vv, Mask); ee;
		ee=graph.getEdgeNext(vv,ee, Mask))
	{
		if(colors.hasKey(ee) && colors[ee]==res)
			return false;
	}
	colors[edge] = res;
	return true;
}
//, gdzie Mapa:PVertex->Set<int> a CholorChooser choose to obiekt funkcyjny podany przez uzytkownika, dzialajacy jako:
//int chooser(const Graph&,PVertex V,const& Set<int>) - dla niepusteg zbioru kolorow dozwolnych przy v wybiera jeden element.
//Funkcja color zwraca true jesli udalo sie wybrac poprawny kolor

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListEdgeColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PEdge edge)
{
	return colorChoose(graph, colLists, colors, edge, FirstFit());
}
//takie samo dzialanie, jakby z funktorem zwracajacym jakikolwiek element dozwolonego zbioru barw

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename EIter,
	typename ColorChooser>
int ListEdgeColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, EIter beg, EIter end, ColorChooser chooser)
{
	int cnt=0;
	while(beg!=end) {
		if(!colorChoose(graph, colLists, colors, *beg, chooser))
			return cnt;
		++beg;
		++cnt;
	}
	return cnt;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename EIter>
int ListEdgeColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, EIter beg, EIter end)
{
	int cnt=0;
	while(beg!=end) {
		if(!color(graph, colLists, colors, *beg))
			return cnt;
		++beg;
		++cnt;
	}
	return cnt;
}
// - dla zakresow kolejno kolorowanych wierzcholkow.	Zwraca: czy udalo sie pokolorowac caly zakres?

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
int ListEdgeColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, ColorChooser chooser)
{
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int cnt=0;
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		if(!colorChoose(graph, colLists, colors, ee, chooser))
			return cnt;
		++cnt;
	}
	return cnt;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
int ListEdgeColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors)
{
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int cnt=0;
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		if(!color(graph, colLists, colors, ee))
			return cnt;
		++cnt;
	}
	return cnt;
}

//testing if graph is properly colored
template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListEdgeColoringPar<DefaultStructs>::testPart(const Graph &graph,
	const ColLists &colLists, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	//does color of edge is in edge color list
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		if(!colors.hasKey(ee)) continue;
		if( !colLists[ee].isElement( colors[ee] ) ) return false;
	}
	Set<int> usedColors;
	for(Vert vv = graph.getVert(); vv; vv=graph.getVertNext(vv)) {
		usedColors.clear();
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if( !colors.hasKey(ee) )
				continue;
			int color = colors[ee];
			if( usedColors.isElement( color ) ) return false;
			usedColors.add( color );
		}
	}
	return true;
}

//testing if graph is properly colored
template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListEdgeColoringPar<DefaultStructs>::test(const Graph &graph,
	const ColLists &colLists, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	//does color of edge is in edge color list
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		if(!colors.hasKey(ee)) return false;
		if( !colLists[ee].isElement( colors[ee] ) ) return false;
	}
	Set<int> usedColors;
	for(Vert vv = graph.getVert(); vv; vv=graph.getVertNext(vv)) {
		usedColors.clear();
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if( !colors.hasKey(ee) )
				return false;
			int color = colors[ee];
			if( usedColors.isElement( color ) ) return false;
			usedColors.add( color );
		}
	}
	return true;
}

//extremal elements of the lists
template <class DefaultStructs>
template<typename Graph, typename ColLists>
std::pair<int,int> ListEdgeColoringPar<DefaultStructs>::listMinMax(
		const Graph &graph, const ColLists& colLists)
{
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	Edge ee = graph.getEdge(Mask);
	int min = colLists[ee].first(), max = colLists[ee].last();
	ee = graph.getEdgeNext(ee, Mask);
	while(ee) {
		int tmp = colLists[ee].first();
		if(min>tmp) min = tmp;
		tmp = colLists[ee].last();
		if(max<tmp) max = tmp;
		ee = graph.getVertNext(ee, Mask);
	}
	return std::make_pair(min, max);
}

//sorted sequence of the numbers used in the lists elements (unique numbers)
//@return the sequence length
template <class DefaultStructs>
template<typename Graph, typename ColLists, typename Iter>
int ListEdgeColoringPar<DefaultStructs>::listColors(
		const Graph &graph, const ColLists &colLists, Iter out)
{
	typedef typename Graph::PEdge Edge;
	typedef typename ColLists::ValType ListType; //ListType have the Set interface
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	Set<int> colSet;
	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		ListType lt = colLists[ee];
		lt.getElements( setInserter(colSet) );
	}
	return colSet.getElements( out );
}

//set of the numbers used in the lists elements
//ColLists should have interface like AssocTabInterface
template <class DefaultStructs>
template<typename Graph, typename ColLists>
Set<int> ListEdgeColoringPar<DefaultStructs>::listColorsSet(
	const Graph &graph, const ColLists& colLists)
{
	typedef typename Graph::PEdge Edge;
	typedef typename ColLists::ValType ListType; //ListType have the Set interface
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	Set<int> res;
	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask))
	{
		ListType lt = colLists[ee];
		lt.getElements( setInserter(res) );
	}
	return res;
}

template <class DefaultStructs>
template<typename Graph, typename ColList, typename ColorMap>
int ListEdgeColoringPar<DefaultStructs>::FirstFit::operator()(const Graph &graph,
		const ColList &colList, const ColorMap &colors, typename Graph::PEdge edge)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	Set<int> avalColors = colList;
	int maxVal = avalColors.last();

	Vert vv = graph.getEdgeEnd1( edge );
	for(Edge ee = graph.getEdge(vv, Mask); ee;
		ee = graph.getEdgeNext(vv, ee, Mask))
	{
		if(!colors.hasKey(ee))
			continue;
		avalColors.del( colors[ee] );
		if(avalColors.size()<=0) return maxVal+1;
	}

	vv = graph.getEdgeEnd2( edge );
	for(Edge ee = graph.getEdge(vv, Mask); ee;
		ee = graph.getEdgeNext(vv, ee, Mask))
	{
		if(!colors.hasKey(ee))
			continue;
		avalColors.del( colors[ee] );
		if(avalColors.size()<=0) return maxVal+1;
	}

	return avalColors.first();
}

//----------------------end of ListEdgeColoring-------------------------

//--------------------begin of IntervalVertColoring-------------------------
//Weights: Graph::PVertex -> int (length of interval)
//ColorMap: Graph::PVertex -> IntervalVertColoringPar::Color

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
int IntervalVertColoringPar<DefaultStructs>::greedy(const Graph &graph,
	const Weights &weights, ColorMap &colors, typename Graph::PVertex vert)
{
	Color col = simulColor(graph, weights, colors, vert);
	colors[vert] = col;
	return col.max-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap, typename VIter>
int IntervalVertColoringPar<DefaultStructs>::greedy(const Graph &graph,
	const Weights &weights, ColorMap &colors, VIter beg, VIter end)
{
	int maxCol = 0;
	while(beg!=end) {
		Color col = simulColor(graph, weights, colors, *beg);
		colors[*beg] = col;
		++beg;
		if(maxCol<col.max)
			maxCol = col.max;
	}
	return maxCol-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
int IntervalVertColoringPar<DefaultStructs>::greedy(const Graph &graph,
	const Weights &weights, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	int maxCol = 0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		Color col = simulColor(graph, weights, colors, vv);
		colors[vv] = col;
		if(maxCol<col.max)
			maxCol = col.max;
	}
	return maxCol-1;
}

//Regula LI:
//1. Znajdź jeszcze niepokolorowany wierzchołek v o najmniejszej możliwej liczbie c,
//takiej że nadanie v przedziału {c,...,c+w(v)–1} nie spowoduje konfliktu z wierzchołkami już pokolorowanymi.
//Przypisz v ten przedział. Remisy rozstrzygaj wybierajac wierzcholek o większej wadze.
//2. Powtarzaj punkt 1 aż do wyczerpania się niepokolorowanych wierzchołków.
//- dane jest pokolorowanie czesciowe, badamy zakres niepokolorowanych wierzcholkow
template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap, typename VIter>
int IntervalVertColoringPar<DefaultStructs>::li(const Graph &graph,
		const Weights &weights, ColorMap &colors, VIter beg, VIter end)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typename DefaultStructs::template
		AssocCont<Vert, int>::Type vertId(graph.getVertNo());

	VIter cur = beg;
	int lenVertTab = 0;
	for(VIter cur = beg; cur!=end; ++cur)
		vertId[*cur] = lenVertTab++;

	std::pair<Vert,Color> LOCALARRAY(freeColors, lenVertTab);
	for(VIter cur = beg; cur!=end; ++cur) {
		int id = vertId[*cur];
		freeColors[id] = std::make_pair(*cur,
				simulColor(graph, weights, colors, *cur));
	}

	Color curColor;
	int maxCol = 0;
	while(lenVertTab>0) {
		int idMinColor = 0;
		for(int ii=1; ii<lenVertTab; ii++) {
			int mOld = freeColors[idMinColor].second.min;
			int mNew = freeColors[ii].second.min;
			if(mNew<mOld || (mNew==mOld
					&& freeColors[ii].second.size()>freeColors[idMinColor].second.size())
			)
				idMinColor = ii;
		}

		Vert curVert = freeColors[idMinColor].first;
		colors[curVert] = curColor = freeColors[idMinColor].second;
		if(maxCol<curColor.max)
			maxCol = curColor.max;

		vertId.delKey(curVert);
		--lenVertTab;
		if(idMinColor<lenVertTab) {
			freeColors[idMinColor] = freeColors[lenVertTab];
			vertId[ freeColors[idMinColor].first ] = idMinColor;
		}

		for(Edge ee = graph.getEdge(curVert, Mask); ee;
			ee = graph.getEdgeNext(curVert, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, curVert);
			if( !vertId.hasKey(u) ) continue;
			int idU = vertId[u];
			Color tmpColor = freeColors[idU].second;
			if(tmpColor.min<curColor.max && curColor.min<tmpColor.max)
				freeColors[ idU ].second = simulColor(graph, weights, colors, u);
		}
	}
	return maxCol-1;
}

//- badamy wszystkie niepokolorowane wierzcholki
template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
int IntervalVertColoringPar<DefaultStructs>::li(const Graph &graph,
		const Weights &weights, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typename DefaultStructs::template
		AssocCont<Vert, int>::Type vertId(graph.getVertNo());

	int lenVertTab = 0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv))
		vertId[vv] = lenVertTab++;

	std::pair<Vert,Color> LOCALARRAY(freeColors, lenVertTab);
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		int id = vertId[vv];
		freeColors[id] = std::make_pair(vv,
				simulColor(graph, weights, colors, vv));
	}

	Color curColor;
	int maxCol = 0;
	while(lenVertTab>0) {
		int idMinColor = 0;
		for(int ii=1; ii<lenVertTab; ii++) {
			int mOld = freeColors[idMinColor].second.min;
			int mNew = freeColors[ii].second.min;
			if(mNew<mOld || (mNew==mOld
					&& freeColors[ii].second.size()>freeColors[idMinColor].second.size())
			)
				idMinColor = ii;
		}

		Vert curVert = freeColors[idMinColor].first;
		colors[curVert] = curColor = freeColors[idMinColor].second;
		if(maxCol<curColor.max)
			maxCol = curColor.max;

		vertId.delKey(curVert);
		--lenVertTab;
		if(idMinColor<lenVertTab) {
			freeColors[idMinColor] = freeColors[lenVertTab];
			vertId[ freeColors[idMinColor].first ] = idMinColor;
		}

		for(Edge ee = graph.getEdge(curVert, Mask); ee;
			ee = graph.getEdgeNext(curVert, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, curVert);
			if( !vertId.hasKey(u) ) continue;
			int idU = vertId[u];
			Color tmpColor = freeColors[idU].second;
			if(tmpColor.min<curColor.max && curColor.min<tmpColor.max)
				freeColors[ idU ].second = simulColor(graph, weights, colors, u);
		}
	}
	return maxCol-1;
}

//- test poprawnosci podanego kolorowania
template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
bool IntervalVertColoringPar<DefaultStructs>::testPart(const Graph &graph,
	const Weights &weights, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if( !colors.hasKey(vv) ) continue;
		Color curColor = colors[vv];
		if(weights[vv]!=curColor.size()) return false;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(u<vv) continue;
			if(!colors.hasKey(u)) continue;
			Color tstColor = colors[u];
			if( curColor.min<tstColor.max && tstColor.min<curColor.max) return false;
		}
	}
	return true;
}

//- test poprawnosci podanego kolorowania
template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
bool IntervalVertColoringPar<DefaultStructs>::test(const Graph &graph,
	const Weights &weights, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		if( !colors.hasKey(vv) ) return false;
		Color curColor = colors[vv];
		if(weights[vv]!=curColor.size()) return false;
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			Vert u = graph.getEdgeEnd(ee, vv);
			if(u<vv) continue;
			if(!colors.hasKey(u)) return false;
			Color tstColor = colors[u];
			if( curColor.min<tstColor.max && tstColor.min<curColor.max) return false;
		}
	}
	return true;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int IntervalVertColoringPar<DefaultStructs>::maxColor(
		const Graph &graph, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	Vert vv = graph.getVert();
	int maxCol = 0;
	for(Vert vv = graph.getVert(); vv;
		vv = graph.getVertNext(vv))
	{
		if(!colors.hasKey(vv)) continue;
		Color col = colors[vv];
		if(maxCol<col.max)
			maxCol = col.max;
	}
	return maxCol;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
typename IntervalVertColoringPar<DefaultStructs>::Color
IntervalVertColoringPar<DefaultStructs>::simulColor(const Graph &graph,
	const Weights &weights, const ColorMap &colors, typename Graph::PVertex vert)
{
	if(colors.hasKey(vert))
		return colors[vert];

	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int range = weights[ vert ];
	Color LOCALARRAY(interv, graph.getEdgeNo(vert, Mask));
	int lenInterv = 0;

	for(Edge ee = graph.getEdge(vert, Mask); ee;
		ee = graph.getEdgeNext(vert, ee, Mask))
	{
		Vert vv = graph.getEdgeEnd(ee, vert);
		if( !colors.hasKey(vv) ) continue;
		interv[lenInterv++] = colors[vv];
	}
	DefaultStructs::sort(interv, interv+lenInterv);

	int colBase = 0;
	for(int iInterv = 0; iInterv<lenInterv; ++iInterv) {
		if(colBase+range<=interv[iInterv].min)
			break;
		else
			if(colBase<interv[iInterv].max)
				colBase = interv[iInterv].max;
	}
	return Color(colBase, colBase+range);
}
//----------------------end of IntervalVertColoring-------------------------

//--------------------begin of IntervalEdgeColoring-------------------------
//Weights: Graph::PEdge -> int (length of interval)
//ColorMap: Graph::PEdge -> IntervalEdgeColoringPar::Color

//Kolorowanie sekwencyjne krawedzi przedzialami liczb calkowitych nieujemnych,
//funkcja wagi krawedzi (liczby naturalne) podaje moc jej przedzialu.
template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
int IntervalEdgeColoringPar<DefaultStructs>::greedy(const Graph &graph,
	const Weights &weights, ColorMap &colors, typename Graph::PEdge edge)
{
	Color col = simulColor(graph, weights, colors, edge);
	colors[ edge ] = col;
	return col.max-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap, typename EIter>
int IntervalEdgeColoringPar<DefaultStructs>::greedy(const Graph &graph,
	const Weights &weights, ColorMap &colors, EIter beg, EIter end)
{
	int maxCol = 0;
	while(beg!=end) {
		Color col = simulColor(graph, weights, colors, *beg);
		colors[*beg] = col;
		++beg;
		if(maxCol<col.max)
			maxCol=col.max;
	}
	return maxCol-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
int IntervalEdgeColoringPar<DefaultStructs>::greedy(const Graph &graph,
	const Weights &weights, ColorMap &colors)
{
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int maxCol = 0;
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		Color col = simulColor(graph, weights, colors, ee);
		colors[ee] = col;
		if(maxCol<col.max)
			maxCol = col.max;
	}
	return maxCol-1;
}

//regula HF: podany zakres lub wszystkie niepokolorowane wierzcholki koloruj zachlannie wg. sekwencji nierosnacych wag
template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap, typename EIter>
int IntervalEdgeColoringPar<DefaultStructs>::hf(const Graph &graph,
	const Weights &weights, ColorMap &colors, EIter beg, EIter end)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	std::pair<int, Edge> LOCALARRAY(edgeTab, graph.getEdgeNo(Mask));
	EIter cur = beg;
	int lenEdgeTab = 0;
	for(EIter cur = beg; cur!=end; ++cur)
		edgeTab[lenEdgeTab++] = std::make_pair( weights[*cur], *cur );
	DefaultStructs::sort(edgeTab, edgeTab+lenEdgeTab);

	int maxCol = 0;
	for(int iEdgeTab=lenEdgeTab-1; iEdgeTab>=0; --iEdgeTab) {
		Edge ee = edgeTab[iEdgeTab].second;
		Color col = simulColor(graph, weights, colors, ee);
		colors[ee] = col;
		if(maxCol<col.max)
			maxCol = col.max;
	}
	return maxCol-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
int IntervalEdgeColoringPar<DefaultStructs>::hf(const Graph &graph,
	const Weights &weights, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	std::pair<int, Edge> LOCALARRAY(edgeTab, graph.getEdgeNo(Mask));
	int lenEdgeTab = 0;
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask))
		edgeTab[lenEdgeTab++] = std::make_pair( weights[ee], ee );
	DefaultStructs::sort(edgeTab, edgeTab+lenEdgeTab);

	int maxCol = 0;
	for(int iEdgeTab=lenEdgeTab-1; iEdgeTab>=0; --iEdgeTab) {
		Edge ee = edgeTab[iEdgeTab].second;
		Color col = simulColor(graph, weights, colors, ee);
		colors[ee] = col;
		if(maxCol<col.max)
			maxCol = col.max;
	}
	return maxCol-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap, typename EIter>
int IntervalEdgeColoringPar<DefaultStructs>::li(const Graph &graph,
	const Weights &weights, ColorMap &colors, EIter beg, EIter end)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typename DefaultStructs::template
		AssocCont<Edge, int>::Type edgeId(graph.getEdgeNo(Mask));

	EIter cur = beg;
	int lenEdgeTab = 0;
	for(EIter cur = beg; cur!=end; ++cur)
		edgeId[*cur] = lenEdgeTab++;

	std::pair<Edge,Color> LOCALARRAY(freeColors, lenEdgeTab);
	//create minimal colorings for each edge (it's not coloring yet)
	for(EIter cur = beg; cur!=end; ++cur) {
		int id = edgeId[*cur];
		freeColors[id] = std::make_pair(*cur,
				simulColor(graph, weights, colors, *cur));
	}

	Color curColor;
	int maxCol = 0;
	while(lenEdgeTab>0) {
		int idMinColor = 0;
		//from colorings choose smallest one (smallest first number of interval
		//  or if first number is the same then interval with smallest size)
		for(int ii=1; ii<lenEdgeTab; ii++) {
			int mOld = freeColors[idMinColor].second.min;
			int mNew = freeColors[ii].second.min;
			if(mNew<mOld || (mNew==mOld
					&& freeColors[ii].second.size()<freeColors[idMinColor].second.size())
			)
				idMinColor = ii;
		}

		//set color to the minimal edge
		Edge curEdge = freeColors[idMinColor].first;
		colors[curEdge] = curColor = freeColors[idMinColor].second;
		if(maxCol<curColor.max)
			maxCol = curColor.max;

		//delete the edge from sequence
		edgeId.delKey(curEdge);
		--lenEdgeTab;
		if(idMinColor<lenEdgeTab) {
			freeColors[idMinColor] = freeColors[lenEdgeTab];
			edgeId[ freeColors[idMinColor].first ] = idMinColor;
		}

		//fix minimal colorings in freeColors sequence
		Vert vv = graph.getEdgeEnd1(curEdge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(!edgeId.hasKey(ee))
				continue;
			int idEE = edgeId[ee];
			Color tmpColor = freeColors[idEE].second;
			if(tmpColor.min<curColor.max && curColor.min<tmpColor.max)
				freeColors[ idEE ].second = simulColor(graph, weights, colors, ee);
		}
		vv = graph.getEdgeEnd2(curEdge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(!edgeId.hasKey(ee))
				continue;
			int idEE = edgeId[ee];
			Color tmpColor = freeColors[idEE].second;
			if(tmpColor.min<curColor.max && curColor.min<tmpColor.max)
				freeColors[ idEE ].second = simulColor(graph, weights, colors, ee);
		}
	}
	return maxCol-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
int IntervalEdgeColoringPar<DefaultStructs>::li(const Graph &graph,
	const Weights &weights, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	typename DefaultStructs::template
		AssocCont<Edge, int>::Type edgeId(graph.getEdgeNo(Mask));
	std::pair<Edge,Color> LOCALARRAY(freeColors, graph.getEdgeNo(Mask));
	// freeColors[ edgeId[ee] ].first == ee

	//create minimal colorings for each edge (it's not coloring yet)
	int lenEdgeTab = 0;
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		edgeId[ee] = lenEdgeTab;
		freeColors[lenEdgeTab] = std::make_pair(ee,
				simulColor(graph, weights, colors, ee));
		++lenEdgeTab;
	}

	Color curColor;
	int maxCol=0;
	while(lenEdgeTab>0) {
		int idMinColor = 0;
		//from colorings choose smallest one (smallest first number of interval
		//  or if first number is the same then interval with smallest size)
		for(int ii=1; ii<lenEdgeTab; ii++) {
			int mOld = freeColors[idMinColor].second.min;
			int mNew = freeColors[ii].second.min;
			if(mNew<mOld || (mNew==mOld
					&& freeColors[ii].second.size()<freeColors[idMinColor].second.size())
			)
				idMinColor = ii;
		}

		//set color to the minimal edge
		Edge curEdge = freeColors[idMinColor].first;
		colors[curEdge] = curColor = freeColors[idMinColor].second;
		if(maxCol<curColor.max)
			maxCol = curColor.max;

		//delete the edge from sequence
		edgeId.delKey(curEdge);
		--lenEdgeTab;
		if(idMinColor<lenEdgeTab) {
			freeColors[idMinColor] = freeColors[lenEdgeTab];
			edgeId[ freeColors[idMinColor].first ] = idMinColor;
		}

		//fix minimal colorings in freeColors sequence
		Vert vv = graph.getEdgeEnd1(curEdge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(!edgeId.hasKey(ee))
				continue;
			int idEE = edgeId[ee];
			Color tmpColor = freeColors[idEE].second;
			if(tmpColor.min<curColor.max && curColor.min<tmpColor.max)
				freeColors[ idEE ].second = simulColor(graph, weights, colors, ee);
		}
		vv = graph.getEdgeEnd2(curEdge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(!edgeId.hasKey(ee))
				continue;
			int idEE = edgeId[ee];
			Color tmpColor = freeColors[idEE].second;
			if(tmpColor.min<curColor.max && curColor.min<tmpColor.max)
				freeColors[ idEE ].second = simulColor(graph, weights, colors, ee);
		}
	}
	return maxCol-1;
}

//- test poprawnosci podanego kolorowania
template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
bool IntervalEdgeColoringPar<DefaultStructs>::testPart(const Graph &graph,
		const Weights &weights, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	for(Edge edge = graph.getEdge(Mask); edge;
		edge = graph.getEdgeNext(edge, Mask))
	{
		if( !colors.hasKey(edge) ) continue;
		Color curColor = colors[edge];
		if(weights[edge]!=curColor.size()) return false;
		Vert vv = graph.getEdgeEnd1(edge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(ee==edge || !colors.hasKey(ee))
				continue;
			Color tstColor = colors[ee];
			if( curColor.min<tstColor.max && tstColor.min<curColor.max)
				return false;
		}
		vv = graph.getEdgeEnd2(edge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(ee==edge || !colors.hasKey(ee))
				continue;
			Color tstColor = colors[ee];
			if( curColor.min<tstColor.max && tstColor.min<curColor.max)
				return false;
		}
	}
	return true;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
bool IntervalEdgeColoringPar<DefaultStructs>::test(const Graph &graph,
		const Weights &weights, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	for(Edge edge = graph.getEdge(Mask); edge;
		edge = graph.getEdgeNext(edge, Mask))
	{
		if( !colors.hasKey(edge) ) return false;
		Color curColor = colors[edge];
		if(weights[edge]!=curColor.size()) return false;
		Vert vv = graph.getEdgeEnd1(edge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(ee==edge)
				continue;
			if(!colors.hasKey(ee)) return false;
			Color tstColor = colors[ee];
			if( curColor.min<tstColor.max && tstColor.min<curColor.max)
				return false;
		}
		vv = graph.getEdgeEnd2(edge);
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
		{
			if(ee==edge)
				continue;
			if(!colors.hasKey(ee)) return false;
			Color tstColor = colors[ee];
			if( curColor.min<tstColor.max && tstColor.min<curColor.max)
				return false;
		}
	}
	return true;
}

template<class DefaultStructs>
template<typename Graph, typename ColorMap>
int IntervalEdgeColoringPar<DefaultStructs>::maxColor(const Graph &graph,
	const ColorMap &colors)
{
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int maxCol = 0;
	for(Edge edge = graph.getEdge(Mask); edge;
		edge = graph.getEdgeNext(edge, Mask))
	{
		if( !colors.hasKey(edge) ) continue;
		Color col = colors[edge];
		if(maxCol<col.max)
			maxCol = col.max;
	}
	return maxCol-1;
}

template<class DefaultStructs>
template<typename Graph, typename Weights>
int IntervalEdgeColoringPar<DefaultStructs>::getWDegs(
	const Graph &graph, const Weights& weights)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int max = 0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		int tmp = 0;
		for(Edge ee = graph.getEdge(vv); ee;
			ee = graph.getEdgeNext(vv, ee))
		{
			tmp += weights[ee];
		}
		if(tmp>max) max = tmp;
	}
	return max;
}

template<class DefaultStructs>
template<typename Graph, typename Weights, typename ColorMap>
typename IntervalEdgeColoringPar<DefaultStructs>::Color
IntervalEdgeColoringPar<DefaultStructs>::simulColor(const Graph &graph,
	const Weights &weights, const ColorMap &colors, typename Graph::PEdge edge)
{
	if(colors.hasKey(edge))
		return colors[edge];

	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	int range = weights[ edge ];
	Vert v = graph.getEdgeEnd1(edge);
	int lenInterv = graph.deg(v);
	v = graph.getEdgeEnd2(edge);
	lenInterv += graph.deg(v);
	Color LOCALARRAY(interv, lenInterv);
	lenInterv = 0;

	v = graph.getEdgeEnd1(edge);
	for(Edge ee = graph.getEdge(v, Mask); ee;
		ee = graph.getEdgeNext(v, ee, Mask))
	{
		if( ee==edge || !colors.hasKey(ee) ) continue;
		interv[lenInterv++] = colors[ee];
	}
	v = graph.getEdgeEnd2(edge);
	for(Edge ee = graph.getEdge(v, Mask); ee;
		ee = graph.getEdgeNext(v, ee, Mask))
	{
		if( ee==edge || !colors.hasKey(ee) ) continue;
		interv[lenInterv++] = colors[ee];
	}
	DefaultStructs::sort(interv, interv+lenInterv);

	int colBase = 0;
	for(int iInterv = 0; iInterv<lenInterv; ++iInterv) {
		if(colBase+range<=interv[iInterv].min)
			break;
		else
			if(colBase<interv[iInterv].max)
				colBase = interv[iInterv].max;
	}
	return Color(colBase, colBase+range);
}




