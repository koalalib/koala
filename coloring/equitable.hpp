template<class DefaultStructs> template <typename Graph, typename ColorMap>
int EquitVertColoringPar<DefaultStructs>::
repairSimple(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn | EdDirOut | EdUndir;

	const int VertNo = graph.getVertNo();
	int LOCALARRAY(colCount, VertNo); //number of vertices colored by index of table
	Vert LOCALARRAY(used, VertNo);
	int maxCol=0;

	for (int i = 0; i < VertNo; ++i) {
		colCount[i] = 0;
		used[i] = NULL;
	}
	for (typename ColorMap::KeyType k = colors.firstKey();
		k; k = colors.nextKey(k))
	{
		int col = colors[k];
		assert(col<VertNo);
		if(maxCol<col) maxCol = col;
		++colCount[ col ];
	}

	while (true) { //main loop
		int minCnt = colCount[0];
		int maxCnt = colCount[0];
		for (int i = 1; i <= maxCol; ++i) {
			if (colCount[i] > maxCnt) {
				maxCnt = colCount[i];
			} else if (colCount[i] < minCnt) {
				minCnt = colCount[i];
			}
		}

		if (minCnt + 1 >= maxCnt) //stop main loop
			break;

		Vert vertLastMax = NULL;
		for (Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
			//search vertex with color of greatest cardinality
			if (colCount[ colors[vv] ] != maxCnt)
				continue;

			for (Edge ee = graph.getEdge(vv, Mask); ee;
				ee = graph.getEdgeNext(vv, ee, Mask))
			{
				Vert u = graph.getEdgeEnd(ee, vv);
				used[ colors[u] ] = vv; // forbidden colors for vv
			}
			int cv;
			for (cv = 0; cv <= maxCol; ++cv) {
				if (used[cv] != vv && colCount[cv] == minCnt)
					break;
			}
			if (cv <= maxCol) {
				--colCount[ colors[vv] ];
				++colCount[ cv ];
				colors[vv] = cv;
				vertLastMax = NULL;
				break;
			}
			vertLastMax = vv;
		}
		if (vertLastMax) {
			--colCount[ colors[vertLastMax] ];
			colors[vertLastMax] = ++maxCol;
			colCount[maxCol] = 1;
		}
	}

	return maxCol;
}

template<class DefaultStructs> template <typename Graph, typename ColorMap>
int EquitVertColoringPar<DefaultStructs>::
repairSubgraph(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	const EdgeDirection Mask = EdDirIn | EdDirOut | EdUndir;
	const int VertNo = graph.getVertNo();

	int maxCol = 0;
	int LOCALARRAY(colCount, VertNo);
	Vert LOCALARRAY(vertTab, VertNo);
	int LOCALARRAY(compTab, VertNo);
	CompDiff LOCALARRAY(sortedComp, VertNo);

	for (int i = 0; i < VertNo; ++i)
		colCount[i] = 0;

	for (typename ColorMap::KeyType k = colors.firstKey();
		k; k = colors.nextKey(k))
	{
		int col = colors[k];
		assert(col<VertNo);
		++colCount[col];
		if( maxCol<col ) maxCol = col;
	}

	int prevMinColId = -1, prevMaxColId = -1;
	int compNo = -1, curCompId = -1;

	while(true) { //main loop
		int minColCntId = 0;
		int maxColCntId = 0;

		for(int i = 1; i <= maxCol; ++i) {
			if (colCount[i] > colCount[maxColCntId]) {
				maxColCntId = i;
			} else if (colCount[i] < colCount[minColCntId]) {
				minColCntId = i;
			}
		}

		if (colCount[minColCntId] + 1 >= colCount[maxColCntId]) //stop main loop
			break;

		if ((minColCntId==prevMinColId && maxColCntId==prevMaxColId)
			|| (minColCntId==prevMaxColId && maxColCntId==prevMinColId))
		{
			++curCompId; //??
		} else {
			curCompId = 0;

			compNo = BFSPar<DefaultStructs>::split(
				makeSubgraph(
					graph,
					std::make_pair(
						assocChoose(colors, minColCntId)
							|| assocChoose(colors, maxColCntId),
						stdChoose(true)
					)
				),
				SearchStructs::compStore(compTab, vertTab),
				Mask
			);

			for (int i = 0; i < compNo; ++i) {
				int diff = 0;
				for (int j = compTab[i]; j < compTab[i + 1]; ++j) {
					diff += (colors[ vertTab[j] ] == maxColCntId) ? 1 : -1;
				}
				sortedComp[i] = CompDiff(i, diff);
			}

			DefaultStructs::sort(sortedComp, sortedComp + compNo);
		}

		if (curCompId < compNo && sortedComp[curCompId].diff > 0) {
			const int i = sortedComp[curCompId].comp;
			for (int j = compTab[i]; j < compTab[i + 1]; ++j) {
				bool toMax = colors[ vertTab[j] ] == minColCntId;
				colors[ vertTab[j] ] = toMax ? maxColCntId : minColCntId;
				colCount[maxColCntId] += toMax ? 1 : -1;
				colCount[minColCntId] += toMax ? -1 : 1;
			}
		} else {
			for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
				if (colors[vv] != maxColCntId)
					continue;
				colors[vv] = ++maxCol;
				++colCount[maxCol];
				--colCount[maxColCntId];
				minColCntId = maxColCntId = -1;
				break;
			}
		}
		prevMinColId = minColCntId;
		prevMaxColId = maxColCntId;
	}

	return maxCol;
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
int EquitVertColoringPar<DefaultStructs>::test(const Graph &graph, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	//const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	if( !SeqVertColoring::test(graph, colors) )
		return -1;

	std::map<int, int> cntMap;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		int col = colors[vv];
		++cntMap[col];
	}

	std::map<int, int>::iterator beg, end;
	beg = cntMap.begin();
	end = cntMap.end();
	if(beg==end) return 0;
	int minCnt, maxCnt;
	minCnt = maxCnt = beg->second;
	while(beg!=end) {
		if(minCnt>beg->second)
			minCnt = beg->second;
		if(maxCnt<beg->second)
			maxCnt = beg->second;
		++beg;
	}
	return maxCnt-minCnt;
}



//let's assume that color is map with continues numbers
// from 0 till some 'n'
template <class DefaultStructs> template<typename Graph, typename ColorMap>
int EquitEdgeColoringPar<DefaultStructs>::repair(const Graph &graph, ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	struct TabVert {
		int vc[2];
		Edge ec[2];
	} *tabVert;
	struct ListEdgeCol {
		int next, prev;
		Edge edge;
	} *listEdgeCol; //0 element is a guard

	const int VertNo = graph.getVertNo();
	const int EdgeNo = graph.getEdgeNo( Mask );

	typename DefaultStructs::template
		AssocCont<typename Graph::PVertex, int>::Type vertToTab( VertNo );
	typename DefaultStructs::template
		AssocCont<typename Graph::PEdge, int>::Type edgeToList( EdgeNo );

	tabVert = new TabVert[ VertNo ];
	listEdgeCol = new ListEdgeCol[ EdgeNo+1 ];

	//init all lists
	int ii = 0;
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv), ++ii)
		vertToTab[vv] = ii;

	typedef std::map<int, CountStruct> MapCountStruct;
	MapCountStruct cntCol;
	typename MapCountStruct::iterator iBeg, iCur;
	int cnt = 0;

	ii=1; //we assume that colors are not set to any edge
	int maxCol = -1; //return value
	for(Edge ee = graph.getEdge(Mask); ee;
		ee = graph.getEdgeNext(ee, Mask), ii++)
	{
		int col = colors[ee];
		if(col>maxCol) maxCol = col;
		iBeg = cntCol.find(col);
		if( iBeg==cntCol.end() ) {
			cntCol[col];
			iBeg = cntCol.find(col);
			iBeg->second.cnt = 0;
			iBeg->second.listEdge = 0;
		}
		edgeToList[ee] = ii;
		listEdgeCol[ iBeg->second.listEdge ].prev = ii;
		listEdgeCol[ii].next = iBeg->second.listEdge;
		listEdgeCol[ii].prev = 0;
		listEdgeCol[ii].edge = ee;

		iBeg->second.listEdge = ii;

		++iBeg->second.cnt;
		++cnt;
	}

	//zliczanie elementow
	const int MinColCnt = cnt / cntCol.size(); //number of vertices in partition
	const int MaxColCnt = MinColCnt + 1; //number of vertices in partition

	const int MaxColPar = cnt % cntCol.size(); //number of partitions
	const int MinColPar = cntCol.size()-MaxColPar; //number of partions

	int minPar=0, maxPar=0; //number of partitions
	for(iBeg = cntCol.begin(); iBeg!=cntCol.end(); iBeg++) {
		cnt = iBeg->second.cnt;
		if(cnt == MinColCnt) minPar++;
		else if(cnt == MaxColCnt) maxPar++;
	}
	//przechodzenie po kolorach

	typename MapCountStruct::iterator colSubgraph[2];
	iBeg = iCur = cntCol.begin();
	while(minPar!=MinColPar || maxPar!=MaxColPar) {
		while( (minPar<=MinColPar && iBeg->second.cnt>=MinColCnt)
			|| (minPar>MinColPar && iBeg->second.cnt>MinColCnt) )
		{
			++iBeg;
			assert(iBeg!=cntCol.end());
		}
		while( (maxPar<=MaxColPar && iCur->second.cnt<=MaxColCnt)
			|| (maxPar>MaxColPar && iCur->second.cnt<MaxColCnt) )
		{
			++iCur;
			assert(iCur!=cntCol.end());
		}
		//creating subgraph
		for(int i=0; i<VertNo; i++)
			tabVert[i].vc[0] = tabVert[i].vc[1] = -1;

		colSubgraph[0] = iBeg;
		colSubgraph[1] = iCur;

		for(int i=0; i<2; i++) {
			int iCol = colSubgraph[i]->second.listEdge;
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

		for(int i=0; i<VertNo; i++) {
			if(tabVert[i].vc[0]>=0) continue;
			if(tabVert[i].vc[1]<0) continue;

			int iV = i;
			int iCol = 1;
			while(tabVert[iV].vc[iCol]>=0) {
				iV = tabVert[iV].vc[iCol];
				assert(iV!=i);
				iCol ^= 1;
			}
			if(iCol!=0) continue; //even path

			//recoloring
			iV = i;
			iCol = 1;
			while( tabVert[iV].vc[iCol]>=0 ) {
				Edge ee = tabVert[iV].ec[iCol];
				colors[ee] = colSubgraph[iCol^1]->first;

				int id = edgeToList[ee];
				int next = listEdgeCol[id].next;
				int prev = listEdgeCol[id].prev;
				//del from one list
				if(prev==0)
					colSubgraph[iCol]->second.listEdge = next;
				listEdgeCol[next].prev = prev;
				listEdgeCol[prev].next = next;
				//add to another list
				listEdgeCol[id].next = next = colSubgraph[iCol^1]->second.listEdge;
				listEdgeCol[id].prev = 0;
				listEdgeCol[next].prev = id;
				colSubgraph[iCol^1]->second.listEdge = id;

				int tmpV = tabVert[iV].vc[iCol];
				tabVert[iV].vc[0] = -1;
				tabVert[iV].vc[1] = -1;
				iV = tmpV;
				iCol ^= 1;
			}
			tabVert[iV].vc[0] = -1;
			tabVert[iV].vc[1] = -1;

			/* TEST: table checking
			{
				int tCnt = 0;
				for(int t=0; t<VertNo; ++t) {
					int vc = tabVert[t].vc[0];
					if(vc>=0) {
						assert(tabVert[vc].vc[0]==t);
						assert(tabVert[vc].ec[0]==tabVert[t].ec[0]);
					}
					vc = tabVert[t].vc[1];
					if(vc>=0) {
						assert(tabVert[vc].vc[1]==t);
						assert(tabVert[vc].ec[1]==tabVert[t].ec[1]);
					}
				}
			}
			//*/

			//fix counters
			if(colSubgraph[0]->second.cnt == MinColCnt) --minPar;
			else if(colSubgraph[0]->second.cnt == MaxColCnt) --maxPar;
			if(colSubgraph[1]->second.cnt == MinColCnt) --minPar;
			else if(colSubgraph[1]->second.cnt == MaxColCnt) --maxPar;
			colSubgraph[0]->second.cnt++;
			colSubgraph[1]->second.cnt--;
			if(colSubgraph[0]->second.cnt == MinColCnt) ++minPar;
			else if(colSubgraph[0]->second.cnt == MaxColCnt) ++maxPar;
			if(colSubgraph[1]->second.cnt == MinColCnt) ++minPar;
			else if(colSubgraph[1]->second.cnt == MaxColCnt) ++maxPar;
			//check if it should be more recoloring

			/* TEST: list checking
			{
				int tCnt = 0;
				int tMaxPar = 0, tMinPar = 0;
				for(typename MapZlicz::iterator iter = cntCol.begin();
					iter!=cntCol.end(); iter++)
				{
					int col = iter->first;
					int iE = iter->second.listEdge;
					int cnt = 0;
					assert(iter->second.cnt>0);
					assert(listEdgeCol[iE].prev == 0);
					while(iE>0) {
						++cnt;
						assert(col == colors[ listEdgeCol[iE].edge ]);
						int tmp = listEdgeCol[iE].next;
						assert(tmp==0 || listEdgeCol[tmp].prev==iE); //list
						iE = tmp;
					}
					tCnt += cnt;
					assert(cnt == iter->second.cnt);
					if(cnt==MinColCnt) tMinPar++;
					if(cnt==MaxColCnt) tMaxPar++;
				}
				assert(tCnt==EdgeNo);
				assert(tMinPar==minPar);
				assert(tMaxPar==maxPar);
			}
			//*/

			/* Description for comments below:
			*  1(2)-element lower(equal) than MinColCnt,
			*  3(4)-element equal(greater) than MaxColCnt:
			*  a - value of colSubgraph[0]->second.cnt
			*  b - value of colSubgraph[1]->second.cnt
			*  x - colSubgraph[0]->second.cnt==colSubgraph[1]->second.cnt
			*  |  1  |2|3|  4  |
			*  |  a  | | |  b  | (1) continue
			*  |     |a| |  b  | (2) conditional (obtained from (1))
			*  |     | |a|  b  | (3) break (obtained from (2))
			*  |  a  | |b|     | (4) conditional (obtained from (1))
			*  |  a  |b| |     | (5) break (obtained from (4))
			*  |     |a|b|     | (6) break (obtained from (1))
			*  |     |x| |     | (7) break (obtained from (4))
			*  |     | |x|     | (8) break (obtained from (2))
			*/

			if( colSubgraph[1]->second.cnt>MaxColCnt ) { //(1), (2) or (3)
				if( colSubgraph[0]->second.cnt<MinColCnt )
					continue; //(1)
				if( colSubgraph[0]->second.cnt>MinColCnt )
					break; //(3)
				//here colSubgraph[0]->second.cnt == MinColCnt (2)
				if(minPar>MinColPar) continue; //conditional
				break;
			}
			if( colSubgraph[0]->second.cnt<MinColCnt ) { //(1), (4) or (5)
				//colSubgraph[1]->second.cnt<=MaxColCnt from previous if; (1)
				if( colSubgraph[1]->second.cnt<MaxColCnt)
					break; //(5)
				//here colSubgraph[1]->second.cnt == MaxColCnt (4)
				if(maxPar>MaxColPar) continue;
				break;
			}
			break; //(6), (7), (8)
		}
	}

	delete [] tabVert;
	delete [] listEdgeCol;
	return maxCol;
}

template <class DefaultStructs> template<typename Graph, typename ColorMap>
int EquitEdgeColoringPar<DefaultStructs>::test(const Graph &graph, const ColorMap &colors)
{
	typedef typename Graph::PVertex Vert;
	typedef typename Graph::PEdge Edge;
	const EdgeDirection Mask = EdDirIn|EdDirOut|EdUndir;

	if( !SeqEdgeColoring::test(graph, colors) )
		return -1;

	std::map<int, int> cntMap;
	for(Edge ee = graph.getEdge(Mask); ee; ee = graph.getEdgeNext(ee, Mask)) {
		int col = colors[ee];
		++cntMap[col];
	}

	std::map<int, int>::iterator beg, end;
	beg = cntMap.begin();
	end = cntMap.end();
	if(beg==end) return 0;
	int minCnt, maxCnt;
	minCnt = maxCnt = beg->second;
	while(beg!=end) {
		if(minCnt>beg->second)
			minCnt = beg->second;
		if(maxCnt<beg->second)
			maxCnt = beg->second;
		++beg;
	}
	return maxCnt-minCnt;
}
