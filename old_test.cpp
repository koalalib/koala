#include <iostream>
#include "Koala/base/def_struct.h"
#include "Koala/graph/graph.h"


using namespace std;
using namespace Koala;

typedef Graph<int,int> GraphI;
typedef Graph<double,double> GraphD;
//typedef Graph::PVertex Viter;
//typedef GraphD::PVertex ViterD;

struct Rzutownik {
	double operator()(int x) {
		return x;
	}
};
struct Rzutownik_int {
	int operator() (int x) {
		return x;
	}
};

struct Prawdzik {
	bool operator()(GraphI::PVertex, const GraphI*) {
		return true;
	}
	bool operator()(GraphI::PEdge, const GraphI*) {
		return true;
	}
};
#define DEB printf("%d\n",__LINE__); fflush(stdout);



int main() {

    Graph<> ggg;
	GraphI graph;
	GraphI::PVertex v1 = graph.addVert(3);
	GraphI::PVertex v2 = graph.addVert();
//	graph.testGraph();
	GraphI::PEdge e1 = graph.addEdge(v1,v2,Undirected);
	printf("%p\n",e1);
//	graph.testGraph();
	cout << graph.getEdgeNo() << " " << graph.getEdgeNo(Undirected) <<endl;
	e1 = graph.addEdge(v1,v2,10);
	printf("%p\n",e1);
	e1 = graph.addEdge(v1,v2);
	printf("%p\n",e1);

	printf("Loop\n");
	e1 = graph.addLoop(v1);
	printf("%p\n",e1);
	e1 = graph.addLoop(v1);
	printf("%p\n",e1);
	e1 = graph.addLoop(v1);
	printf("%p\n",e1);

	printf("Dir out\n");
	e1 = graph.addArch(v1,v2);
	printf("%p\n",e1);
	e1 = graph.addArch(v1,v2);
	printf("%p\n",e1);
	e1 = graph.addArch(v1,v2);
	printf("%p\n",e1);

	printf("Dir in\n");
	e1 = graph.addArch(v2,v1);
	printf("%p\n",e1);
	e1 = graph.addArch(v2,v1);
	printf("%p\n",e1);
	e1 = graph.addArch(v2,v1);
	printf("%p\n",e1);

	printf("TEST\n");
//	graph.testGraph();

	printf("Szukanie krawedzi\n");
	e1 = graph.getEdgePrev(v1,NULL,EdLoop|EdDirIn);
	while(e1) {
		printf("%p %d\n",e1, graph.getEdgeType(e1));
		e1 = graph.getEdgePrev(v1,e1,EdAll);
	}
	printf("KONIEC\n");
	e1 = graph.getEdge(v1,v1);
	while(e1) {
		printf("%p %d\n",e1, graph.getEdgeType(e1));
		e1 = graph.getEdgeNext(v1,v1,e1);
	}

	GraphD graphd;
	graphd.copy(graph);
//	graphd.testGraph();

	GraphI grapha = graph;
//	grapha.testGraph();
	GraphI::PVertex wierzcholki[30];
	int len_wer = graph.getVerts(wierzcholki);
	for(int i=0; i<len_wer; i++)
		printf("%d %p\n",i,wierzcholki[i]);

	graph.getNeighSet(v1);
    return 0;
}

