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

	int lenVertTab = 0;
	for(VIter cur = beg; cur!=end; ++cur) {
		if( vertId.hasKey(*cur) ) continue;
		vertId[*cur] = lenVertTab++;
	}

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

	int lenEdgeTab = 0;
	for(EIter cur = beg; cur!=end; ++cur, ++lenEdgeTab);

	std::pair<int, Edge> LOCALARRAY(edgeTab, lenEdgeTab);
	lenEdgeTab = 0;
	for(EIter cur = beg; cur!=end; ++cur) {
		if( !(cur->getType()&Mask) ) continue;
		edgeTab[lenEdgeTab++] = std::make_pair( weights[*cur], *cur );
	}
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

	int lenEdgeTab = 0;
	for(EIter cur = beg; cur!=end; ++cur) {
		if( edgeId.hasKey(*cur) ) continue;
		edgeId[*cur] = lenEdgeTab++;
	}

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
		for(Edge ee = graph.getEdge(vv, Mask); ee;
			ee = graph.getEdgeNext(vv, ee, Mask))
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

