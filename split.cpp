#include<stdio.h>
#include"koala/graph/graph.h"
#include"koala/algorithm/search.h"
#include"koala/io/text.h"

using namespace Koala;
using namespace Koala::IO;

template<class GraphType>
void PrintSubsets(const GraphType & g, EdgeDirection mask = EdUndir) {
	unsigned int depth, n, retVal;
	typename GraphType::PEdge e;
	typename GraphType::PVertex u, v;

	Koala::Privates::ListBlockListAllocator<Koala::Privates::List_iterator<LexBFS::LVCNode<GraphType> > > allocat(2*g.getVertNo()+1); //TODO: size?
	Koala::Privates::ListBlockListAllocator<LexBFS::LVCNode<GraphType> > allocat2(2*g.getVertNo()+1); //TODO: size?
	LexBFS::LexVisitContainer<GraphType, Koala::Privates::ListBlockListAllocator<Koala::Privates::List_iterator<LexBFS::LVCNode<GraphType> > >,Koala::Privates::ListBlockListAllocator<LexBFS::LVCNode<GraphType> > > cont(allocat,allocat2,g.getVertNo());
	typename AlgorithmsDefaultSettings::template AssocCont<typename GraphType::PVertex, int>::Type visited;


	n = g.getVertNo();
	if(n == 0) return;

	cont.initializeAddAll(g);

	retVal = 0;

	while(!cont.empty()) {
//		cont.dump();
		if(cont.m_data.size() > 2) {
			if(cont.m_data.begin().next().next()->v == NULL) {
				printf(" |");
				};
			};
		u = cont.top();
		printf(" %d", u->info);
		visited[u] = 1;

		cont.pop();

		for(e = g.getEdge(u, mask); e != NULL; e = g.getEdgeNext(u, e, mask)) {
			v = g.getEdgeEnd(e, u);
			if(visited.hasKey(v)) {
				if(visited[v] == -1) cont.move(v);
				continue;
				};
			visited[v] = -1;
			cont.move(v);
			};
		cont.done();
		};
	printf("\n");
	};

int main() {
	Graph<int, int> g;
	readGraphText(g,
//			 "7\n"
//			 "0(0) 3 -1 -2 -3\n"
//			 "1(1) 2 -2 -3\n"
//			 "2(2) 2 -3 -5\n"
//			 "3(3) 0\n"
//			 "4(4) 2 -5 -6\n"
//			 "5(5) 1 -6\n"
//			 "6(6) 0\n",

			 "8\n"
			 "0(0) 4 -1 -2 -3 -4\n"
			 "1(1) 2 -2 -3\n"
			 "2(2) 1 -3\n"
			 "3(3) 0\n"
			 "4(4) 3 -5 -6 -7\n"
			 "5(5) 2 -6 -7\n"
			 "6(6) 1 -7\n"
			 "7(7) 0\n",
			 RG_VertexLists);
	PrintSubsets(g);
	return 0;
	};
