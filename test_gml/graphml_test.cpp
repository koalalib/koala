#include <iostream>
#include "../Koala/graph/graph.h"
#include "../Koala/base/def_struct.h"
#include "../Koala/io/graphml.h"
#define TEST1

using namespace std;
using namespace Koala;

typedef Graph<int,int> GraphI;
typedef Graph<double,double> GraphD;

struct FunktorReadV {
	GraphI::VertInfoType operator()(IO::KeysHolderRead *keysHolder) {
		GraphI::VertInfoType tmp;
		keysHolder->getId("vrt");
		int id = keysHolder->getId("nn");
		tmp = id ? keysHolder->getInt(id) : 0;
		return tmp;
	}
};
struct FunktorReadE {
	GraphI::EdgeInfoType operator()(IO::KeysHolderRead *keysHolder) {
		GraphI::EdgeInfoType tmp;
		keysHolder->getId("edg");
		int id = keysHolder->getId("ee");
		tmp = id ? keysHolder->getInt(id) : 0;
		return tmp;
	}
};

struct FunktorWrite {
	void operator()(GraphI::PVertex vert, IO::KeysHolderWrite *keysHolder) {
		keysHolder->getIdCr("VRT");
		int id = keysHolder->getIdCr("NN");
		char zm[30];
		sprintf(zm,"%d",vert->info);
		keysHolder->setString(id, zm);
	}
	void operator()(GraphI::PEdge edge, IO::KeysHolderWrite *keysHolder) {
		keysHolder->getIdCr("EDG");
		int id = keysHolder->getIdCr("EE");
		char zm[30];
		sprintf(zm,"%d", edge->info);
		keysHolder->setString(id, zm);
	}
	void operator()(GraphI &graph, IO::KeysHolderWrite *keysHolder) {
		keysHolder->getIdCr("GRH");
		int id = keysHolder->getIdCr("GG");
		char zm[30];
		sprintf(zm, "%d", (int)&graph);
		keysHolder->setString(id, zm);
	}
};

#ifdef TEST0
int main() {
	IO::GraphML graphml;
	GraphI graph;
	graphml.writeFile("graph_none.xml");
	graphml.writeGraph(graph, "empty");
	graphml.writeFile("empty_graph.xml");
	return 0;
}
#endif

#ifdef TEST1
int main() {
	IO::GraphML graphml;
	GraphI graph;
/*	graphml.readFile("graph_in1.xml");
	graph.clear();
	graphml.readGraph(graph,0);
	graphml.clear();
	graphml.writeGraph(graph,"test");
	graphml.writeGraph(graph,"test1");
	graphml.writeFile("graph_out1.xml"); //*/

	// testy z kluczami
	graphml.readFile("graph_in2.xml");
	graphml.readGraph(graph, 0, FunktorReadV(), FunktorReadE());
	graphml.clear();
	graphml.writeGraph(graph, "test", FunktorWrite(), FunktorWrite());
	graphml.writeFile("graph_out2.xml");
//*/
/*	graphml.readFile("graph1.xml");
	graphml.getGraph(graph, 0);
	graph.testGraph();
	graphml.setGraph(graph, "ala");
	graphml.writeFile("graph00.xml");*/
//	graphml.readFile("graph_int.xml");
//	graphml.getGraph(graph,0);
//	graphml.getGraph(graph,0,FunktorGet());
//	graph.testGraph();
//	graphml.setGraph(graph, "nowy", FunktorSet());
//	graphml.writeFile("graph_int_out.xml");
    return 0;
}
#endif


#ifdef TEST2
int main() {
	IO::GraphML graphml;
	GraphI graph;
	IO::KeysHolderGraph khGraph;

	graphml.readFile("graph_in2.xml");
	graphml.readGraphParam(0, khGraph);

	int id = khGraph.getIdCr("nowyEl");
	khGraph.setInt(id, 456);
	id = khGraph.getIdCr("nn");
	khGraph.setInt(id, 567);
	id = khGraph.getIdCr("ee");
	khGraph.setInt(id, 678);
	id = khGraph.getIdCr("gg");
	khGraph.setInt(id, 789);
	graphml.writeGraphParam(0, khGraph);

	graphml.writeFile("graph_out2.xml");

    return 0;
}
#endif

#ifdef TEST3
int main() {
	IO::GraphML graphml;
	GraphI graph;
	IO::KeysHolderGraph khGraph;

	graphml.readFile("graph_in2.xml");
	graphml.readGraphParam(0, khGraph);

	int id = khGraph.getIdNoCr("nowyEl");
	khGraph.setInt(id, 456);
	id = khGraph.getIdNoCr("nn");
	khGraph.setInt(id, 567);
	id = khGraph.getIdNoCr("ee");
	khGraph.setInt(id, 678);
	id = khGraph.getIdNoCr("gg");
	khGraph.setInt(id, 789);
	graphml.writeGraphParam(0, khGraph);

	graphml.writeFile("graph_out2.xml");

    return 0;
}
#endif


