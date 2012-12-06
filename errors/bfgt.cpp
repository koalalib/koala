#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include<map>
#include<string>
#include<fstream>
#include<iostream>
#include"./koala/graph/graph.h"
#include"./koala/classes/detect.h"
#include"./koala/classes/create.h"
#include"./koala/coloring/vertex.h"
#include"./koala/coloring/edge.h"
#include"./koala/io/text.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;


int main() {
	int i, n, m, z;
	int t[10], f[10], tot;

	srand(time(NULL));
	tot = 0;
	for(int i = 0; i < 10; i++) t[i] = f[i] = 0;
	z = 0;
	while(!kbhit()) {
		n = rand() % 128;
		if(n < 2) m = 0;
		else m = rand() % (n * (n - 1) / 2);

		vector<int> indexes;
		vector<int> vertList2;
		Graph<int, int> g;
		Graph<int, int>::PVertex u, v;
		vector<Graph<int, int>::PEdge> edgeList;
		vector<Graph<int, int>::PVertex> vertList;
		AssocTable<map<Graph<int, int>::PVertex, SearchStructs::VisitVertLabs<Graph<int, int> > > > cont;
		AssocTable<std::map<Graph<int, int>::PEdge, int > > emap;
	 	AssocTable<std::map<Graph<int, int>::PVertex, Blocks::VertData > > vmapblk;
	 	AssocTable<std::map<Graph<int, int>::PVertex, int > > vmap;


		bool newGraph = false;
		if(newGraph) {
			Creator::erdRen2(g, n, m, Undirected);

			i = 0;
			for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) v->info = i++;

			fstream fileGraph;
			fileGraph.open("tested.txt", ios_base::out);
			writeGraphText(g, fileGraph, RG_VertexLists | RG_Info);
			fileGraph.close();
		} else {
			fstream fileGraph;
			fileGraph.open("tested.txt", ios_base::in);
			readGraphText(g, fileGraph, RG_VertexLists);
			fileGraph.close();
			};

		u = g.getVert();
		indexes.resize(g.getVertNo() + 1);
		vertList.resize(g.getVertNo() * g.getVertNo() + g.getEdgeNo() * 2 + 1);
		vertList2.resize(g.getVertNo() * g.getVertNo() + g.getEdgeNo() * 2 + 1);
		edgeList.resize(g.getVertNo() * g.getVertNo() + g.getEdgeNo() * 2 + 1);

		fprintf(stderr, "\r%3d %5d 1 ", n, m);
		if(IsIt::chordal(g)) t[0]++; else f[0]++;
		fprintf(stderr, "\r%3d %5d 2 ", n, m);
		if(IsIt::cochordal(g)) t[1]++; else f[1]++;
		fprintf(stderr, "\r%3d %5d 3 ", n, m);
		if(IsIt::comparability(g)) t[2]++; else f[2]++;
		fprintf(stderr, "\r%3d %5d 4 ", n, m);
		if(IsIt::interval(g)) t[3]++; else f[3]++;
		fprintf(stderr, "\r%3d %5d 5 ", n, m);
		if(IsIt::bipartite(g)) t[4]++; else f[4]++;
		if(u != NULL) {
			fprintf(stderr, "\r%3d %5d 6 ", n, m);
			BFS::scanAttainable(g, u, vertList.begin(), EdAll, cont);
			fprintf(stderr, "\r%3d %5d 7 ", n, m);
			DFSPostorder::scanAttainable(g, u, vertList.begin(), EdAll, cont);
			fprintf(stderr, "\r%3d %5d 8 ", n, m);
			DFSPreorder::scanAttainable(g, u, vertList.begin(), EdAll, cont);
			fprintf(stderr, "\r%3d %5d 9 ", n, m);
			LexBFS::scanAttainable(g, u, vertList.begin(), EdAll, cont);
			};
		fprintf(stderr, "\r%3d %5d 10", n, m);
		z += Blocks::split(g, vmapblk, emap, SearchStructs::compStore(indexes.begin(), vertList.begin()), vertList2.begin());
		fprintf(stderr, "\r%3d %5d 11", n, m);
		z += BFS::split(g, SearchStructs::compStore(indexes.begin(), vertList.begin()), EdUndir);
		fprintf(stderr, "\r%3d %5d 12", n, m);
		z += DFSPostorder::split(g, SearchStructs::compStore(indexes.begin(), vertList.begin()), EdUndir);
		fprintf(stderr, "\r%3d %5d 13", n, m);
		z += DFSPreorder::split(g, SearchStructs::compStore(indexes.begin(), vertList.begin()), EdUndir);
		fprintf(stderr, "\r%3d %5d 14", n, m);
		z += LexBFS::split(g, SearchStructs::compStore(indexes.begin(), vertList.begin()), EdUndir);
		fprintf(stderr, "\r%3d %5d 15", n, m);
		z += BFS::scan(g, vertList.begin(), EdUndir);
		fprintf(stderr, "\r%3d %5d 16", n, m);
		z += DFSPreorder::scan(g, vertList.begin(), EdUndir);
		fprintf(stderr, "\r%3d %5d 17", n, m);
		z += DFSPostorder::scan(g, vertList.begin(), EdUndir);
		fprintf(stderr, "\r%3d %5d 18", n, m);
		z += LexBFS::scan(g, vertList.begin(), EdUndir);
		fprintf(stderr, "\r%3d %5d 19", n, m);
		z += SCC::split(g, Koala::SearchStructs::compStore(indexes.begin(), vertList.begin()), blackHole);
/*/STACKOV*/	fprintf(stderr, "\r%3d %5d 20", n, m);
/*/STACKOV*/	z += Euler::getPath(g, Euler::outPath(vertList.begin(),edgeList.begin()));
/*/STACKOV*/	fprintf(stderr, "\r%3d %5d 21", n, m);
/*/STACKOV*/	z += Euler::getCycle(g, Euler::outPath(vertList.begin(),edgeList.begin()));
		fprintf(stderr, "\r%3d %5d 22", n, m);
		Modules::split(g, SearchStructs::compStore(indexes.begin(), vertList.begin()), blackHole);
/*/SLOW*/	fprintf(stderr, "\r%3d %5d 23", n, m);
/*/SLOW*/	vector<int> vidx, eidx;
/*/SLOW*/	vector<Graph<int, int>::PVertex> vpath;
/*/SLOW*/	vector<Graph<int, int>::PEdge> epath;
/*/SLOW*/	vidx.resize(g.getVertNo() + 1);
/*/SLOW*/	eidx.resize(g.getEdgeNo() + 1);
/*/SLOW*/	vpath.resize(g.getVertNo() * 3 + 1);
/*/SLOW*/	epath.resize(g.getEdgeNo() + 1);
//error/*/SLOW*/	Connect::minEdgeCut(g, edgeList.begin());
/*/SLOW*/	fprintf(stderr, "\r%3d %5d 24", n, m);
/*/SLOW*/	Connect::minVertCut(g, vertList.begin());
		fprintf(stderr, "\r%3d %5d 25", n, m);
		vmap.clear();
		z += SeqVertColoring::greedy(g, vmap);
		fprintf(stderr, "\r%3d %5d 26", n, m);
		vmap.clear();
		z += SeqVertColoring::greedyInter(g, vmap);
/**/		fprintf(stderr, "\r%3d %5d 27", n, m);
		vmap.clear();
//error/**/		z += SeqVertColoring::lf(g, vmap);
/**/		fprintf(stderr, "\r%3d %5d 28", n, m);
		vmap.clear();
//error/**/		z += SeqVertColoring::lfInter(g, vmap);
/**/		fprintf(stderr, "\r%3d %5d 29", n, m);
		vmap.clear();
//error/**/		z += SeqVertColoring::sl(g, vmap);
/**/		fprintf(stderr, "\r%3d %5d 30", n, m);
		vmap.clear();
//error/**/		z += SeqVertColoring::slInter(g, vmap);
/**/		fprintf(stderr, "\r%3d %5d 31", n, m);
		vmap.clear();
//error/**/		z += SeqVertColoring::slf(g, vmap);
/**/		fprintf(stderr, "\r%3d %5d 32", n, m);
		vmap.clear();
//error/**/		z += SeqVertColoring::slfInter(g, vmap);
		fprintf(stderr, "\r%3d %5d 33", n, m);
		vmap.clear();
		z += SeqVertColoring::brooks(g, vmap);
		fprintf(stderr, "\r%3d %5d 34", n, m);
		vmap.clear();
		z += GisVertColoring::color(g, vmap);
		fprintf(stderr, "\r%3d %5d 35", n, m);
		vmap.clear();
//error/**/		z += SeqEdgeColoring::vizing(g, emap);
/**/		fprintf(stderr, "\r%3d %5d 36", n, m);
		vmap.clear();
		z += SeqEdgeColoring::greedyInter(g, emap);
		fprintf(stderr, "\r%3d %5d 37", n, m);
		vmap.clear();
		z += SeqEdgeColoring::greedy(g, emap);

		fprintf(stderr, "\r%3d %5d 38", n, m);
		if(IsIt::split(g)) z++;
		fprintf(stderr, "\r%3d %5d 39", n, m);
		if(IsIt::prime(g)) z++;
//		fprintf(stderr, "\r%3d %5d 40", n, m);
//		if(IsIt::planar(g)) z++;
//		fprintf(stderr, "\r%3d %5d 41", n, m);
//		if(IsIt::outerplanar(g)) z++;
		fprintf(stderr, "\r%3d %5d 42", n, m);
		if(IsIt::cograph(g)) z++;
		fprintf(stderr, "\r%3d %5d 43", n, m);
		if(IsIt::cocomparability(g)) z++;
		fprintf(stderr, "\r%3d %5d 44", n, m);
		if(IsIt::compMPartite(g)) z++;
		fprintf(stderr, "\r%3d %5d 45", n, m);
		if(IsIt::compBipartite(g)) z++;
		fprintf(stderr, "\r%3d %5d 46", n, m);
		if(IsIt::almostTree(g)) z++;
		fprintf(stderr, "\r%3d %5d 47", n, m);
		if(IsIt::block(g)) z++;
		fprintf(stderr, "\r%3d %5d 48", n, m);
		if(IsIt::cubic(g)) z++;
		fprintf(stderr, "\r%3d %5d 49", n, m);
		if(IsIt::subcubic(g)) z++;
		fprintf(stderr, "\r%3d %5d 50", n, m);
		if(IsIt::caterpillar(g)) z++;
		fprintf(stderr, "\r%3d %5d 51", n, m);
		if(IsIt::regular(g)) z++;
		fprintf(stderr, "\r%3d %5d 52", n, m);
		if(IsIt::connected(g)) z++;
		fprintf(stderr, "\r%3d %5d 53", n, m);
		if(IsIt::forest(g)) z++;
		fprintf(stderr, "\r%3d %5d 54", n, m);
		if(IsIt::undir(g)) z++;
		fprintf(stderr, "\r%3d %5d 55", n, m);
		if(IsIt::empty(g)) z++;
		fprintf(stderr, "\r%3d %5d 56", n, m);
		if(IsIt::tree(g)) z++;
		fprintf(stderr, "\r%3d %5d 57", n, m);
		if(IsIt::clique(g)) z++;
		fprintf(stderr, "\r%3d %5d 58", n, m);
		if(IsIt::cliques(g)) z++;
		fprintf(stderr, "\r%3d %5d 59", n, m);
		if(IsIt::path(g)) z++;
		fprintf(stderr, "\r%3d %5d 60", n, m);
		if(IsIt::cycle(g)) z++;
		fprintf(stderr, "\r%3d %5d 61", n, m);
		if(IsIt::matching(g)) z++;
/*/SLOW*/		fprintf(stderr, "\r%3d %5d 62", n, m);
/*/SLOW*/		AssocTable<BiDiHashMap<Graph<int, int>::PEdge,Flow::UnitEdgeLabs<int> > > flowEdgeCont;
//error/*/SLOW*/		z += Flow::minEdgeCut(g, flowEdgeCont, Flow::outCut(blackHole, edgeList.begin())).capac;
		fprintf(stderr, "\r%3d %5d 63", n, m);
//		AssocTable<BiDiHashMap<Graph<int, int>::PVertex,Matching::VertLabs<Graph<int, int>> > > matchEC;
		AssocTable<std::map<Graph<int, int>::PVertex,Matching::VertLabs<Graph<int, int> > > > matchEC;
		z += Matching::findMax(g, matchEC, edgeList.begin());

		tot++;
		if(tot % 100 == 0) {
			fprintf(stderr, "\r                      %d", tot);
			};
		};
	int s = z;
	for(i = 0; i < 10; i++) s += f[i] + 2 * t[i];
	return s ? 1 : 0;
	};
