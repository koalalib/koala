template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
bool ListVertColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PVertex vert,
	ColorChooser chooser)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	koalaAssert( colLists.hasKey(vert), AlgExcWrongArg );
	if( colors.hasKey(vert) ) {
		int col = colors[vert];
		if( col>=0 && colLists[vert].isElement(col) )
			return true;
	}
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

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListVertColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PVertex vert)
{
	return colorChoose(graph, colLists, colors, vert, FirstFit());
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap,
	typename VIter, typename ColorChooser>
int ListVertColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, VIter beg, VIter end, ColorChooser chooser)
{
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getVertNo());
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
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getVertNo());
	int cnt = 0;
	while(beg!=end) {
		if(!color(graph, colLists, colors, *beg))
			return cnt;
		++beg;
		++cnt;
	}
	return cnt;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
int ListVertColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, ColorChooser chooser)
{
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getVertNo());
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
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getVertNo());
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
	typedef typename ColLists::ValType ListType; //ListType has the Set interface
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
	typedef typename ColLists::ValType ListType; //ListType has the Set interface
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
	int maxVal = avalColors.max();
	for(Edge ee = graph.getEdge(vert, Mask); ee;
		ee = graph.getEdgeNext(vert, ee, Mask))
	{
		Vert u = graph.getEdgeEnd(ee, vert);
		if(!colorMap.hasKey(u))
			continue;
		avalColors.del( colorMap[u] );
	}
	if(avalColors.size()>0) return avalColors.min();
	else return maxVal+1;
}

//==============================================================================
//============================= edge coloring ==================================
//==============================================================================

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
bool ListEdgeColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PEdge edge,
	ColorChooser chooser )
{
	koalaAssert( colLists.hasKey(edge), AlgExcWrongArg );
	if(colors.hasKey(edge)) {
		int col = colors[edge];
		if(col>=0 && colLists[edge].isElement(col))
			return true;
	}
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

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
bool ListEdgeColoringPar<DefaultStructs>::color(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, typename Graph::PEdge edge)
{
	return colorChoose(graph, colLists, colors, edge, FirstFit());
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename EIter,
	typename ColorChooser>
int ListEdgeColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, EIter beg, EIter end, ColorChooser chooser)
{
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getEdgeNo(Mask));
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
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getEdgeNo(Mask));
	int cnt=0;
	while(beg!=end) {
		if(!color(graph, colLists, colors, *beg))
			return cnt;
		++beg;
		++cnt;
	}
	return cnt;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
int ListEdgeColoringPar<DefaultStructs>::colorChoose(const Graph &graph,
	const ColLists &colLists, ColorMap &colors, ColorChooser chooser)
{
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getEdgeNo(Mask));
	typedef typename Graph::PEdge Edge;
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
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getEdgeNo(Mask));
	typedef typename Graph::PEdge Edge;
	int cnt=0;
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		if(!color(graph, colLists, colors, ee))
			return cnt;
		++cnt;
	}
	return cnt;
}

template <class DefaultStructs>
template<typename Graph, typename ColLists, typename ColorMap>
int ListEdgeColoringPar<DefaultStructs>::colorBipartite(const Graph &graph,
	const ColLists &colLists, ColorMap &colors)
{
	/* 1. test bipartiteness
	   2. color edges of the graph (edge coloring)
	   3. foreach uncolored edge
	     a) take a unused color c from the list of colors available to the edge
	     b) create a subgraph induced by the color c (from vertices lists)
	     c) find the stable matching of the subgraph
	     d) color edges from the matching by color c
	*/
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;
	if(DefaultStructs::ReserveOutAssocCont)
		colors.reserve(graph.getEdgeNo(Mask));
	int lenTabV1, n=graph.getVertNo();
	typename Graph::PVertex LOCALARRAY(tabV1, n);
	typedef typename DefaultStructs::template AssocCont<typename Graph::PEdge, int>::Type EVWeight;
	EVWeight evWeight( graph.getEdgeNo(Mask) ); //the edge coloring(not list coloring)
	Set<int> usedColors;
	typename DefaultStructs::template AssocCont<typename Graph::PVertex, EmptyVertInfo>::Type setV1(n);
//	Set<typename Graph::PVertex> setV1;

	// 1.
	lenTabV1 = IsItPar<DefaultStructs>::Bipartite::getPart(graph, tabV1, true);
	if(lenTabV1<0) return -1;
	for(int i=0;i<lenTabV1;i++) setV1[tabV1[i]];
//	setV1.assign(tabV1, lenTabV1);

	typename Graph::PEdge LOCALARRAY(tabStMatch, lenTabV1);
	// 2.
	int colNo = SeqEdgeColoringPar<DefaultStructs>::greedyInter(graph, evWeight); //proper edge coloring
	EVOrderBipartite<EVWeight, typename DefaultStructs::template AssocCont<typename Graph::PVertex, EmptyVertInfo>::Type >
        evComparator(&evWeight, &setV1); //compare edges
	// 3.
	typename Graph::PEdge ee = graph.getEdge(Mask);
	int cnt = 0;
	while( ee ) {
		if(colors.hasKey(ee)) {
			ee = graph.getEdgeNext(ee, Mask);
			continue;
		}
		// a)
		typename ColLists::ValType eColList = colLists[ee]; //do zmiany??
		if(eColList.size()<=0) return -1;
		int curColor = eColList.first();
		while(1) {
			if(!usedColors.isElement(curColor))
				break;
			if(curColor==eColList.last()) return -1;
			curColor = eColList.next(curColor);
		}
		// b)
		// c)
		int lenStMatch = StableMatchingPar<DefaultStructs>::bipartFind(
				makeSubgraph(graph, std::make_pair(stdChoose(true),
						!extAssocKeyChoose(&colors) && !edgeTypeChoose(Loop)
                        && extAssocFChoose(&colLists,EColorTakeBipart(curColor)))),
				tabV1, tabV1+lenTabV1,
				evComparator, blackHole, tabStMatch);
		// d)
		if(lenStMatch<0) return -1;
		while(lenStMatch) {
			--lenStMatch;
			colors[ tabStMatch[lenStMatch] ] = curColor;
			cnt++;
		}
		usedColors += curColor;
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
		ee = graph.getEdgeNext(ee, Mask);
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
	int maxVal = avalColors.max();

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

	return avalColors.min();
}

