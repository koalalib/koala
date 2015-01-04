#define KOALA_SET_ON_HASHSET

#include <iostream>
#include "../Koala/coloring/interval.h"
#include "../Koala/io/graphml.h"

using namespace std;

struct GMLEdge {
	string name;
	int color;
	int weight;
	GMLEdge(const string &n, const int c, const int w):
		name(n), color(c), weight(w) {}
};

typedef Koala::Graph<string, GMLEdge> Graph;
typedef Graph::PVertex Vert;
typedef Graph::PEdge Edge;
typedef Koala::IntervalEdgeColoring::Color IntervalColor;
typedef Koala::AssocTable< std::map< Edge,IntervalColor > > Map;

struct FunktorReadV {
	Graph::VertInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		if(keysHolder->isValue("vName"))
			return keysHolder->getString("vName");
		return "brak";
	}
};

struct FunktorReadE {
	Graph::EdgeInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		int color = -1;
		if(keysHolder->isValue("color"))
			color = keysHolder->getInt("color");

		int weight = 1;
		if(keysHolder->isValue("weight"))
			weight = keysHolder->getInt("weight");

		if(keysHolder->isValue("eName"))
			return GMLEdge(keysHolder->getString("eName"), color, weight);
		return GMLEdge("brak", color, weight);
	}
};

void print(const Graph &graph, const Map &colors) {
	for(Edge ee = graph.getEdge(); ee; ee = graph.getEdgeNext(ee)) {
		cout << ee->info.name << " [";
		if(colors.hasKey(ee)) cout << colors[ee].left <<", " <<colors[ee].right << ") ";
		else cout << " ]";
		cout << " <";
		Vert v = graph.getEdgeEnd1(ee);
		cout << v->info << ", ";
		v = graph.getEdgeEnd2(ee);
		cout << v->info << ">\n";
	}
}

int main(int argc, char **argv) {
	Graph graph;
	Koala::IO::GraphML gmlIn;
	Koala::AssocArray<Edge, int> weights;
	Map colors, tmpColors;

	if(!gmlIn.readFile("intervEdgeCol_test.xml") ) {
		cout << "read error";
		return 1;
	}

	int testEnd = 0;
	int testBeg = 0;
	switch(argc) {
		case 3:
			testBeg = atoi(argv[1]);
			testEnd = atoi(argv[2]);
			break;
		case 2:
			testEnd = atoi(argv[1]);
		case 1:
			testEnd = gmlIn.graphNo();
	}

	//-------------------------------------------------------------------------
	for(int testId = testBeg; testId<testEnd; testId++) {
		cout<<"\nTEST " << testId;
		graph.clear();
		colors.clear();
		bool testPass = true;
		Koala::IO::GraphMLGraph *gmlg = gmlIn.getGraph(testId);
		if(gmlg==NULL) {
			cout << "error - graph[" << testId << "] not read";
			return 2;
		}
		if(!gmlg->readGraph(graph, FunktorReadV(), FunktorReadE())) {
			cout << "error - graph[" << testId << "] not read";
			return 3;
		}
		cout<<" "<<gmlg->getName()<<"\n";

		int param = gmlg->getInt("show");
		for(Edge ee = graph.getEdge(); ee; ee = graph.getEdgeNext(ee)) {
			if(ee->info.color>=0) colors[ee] = ee->info.color;
			weights[ee] = ee->info.weight;
		}

		if( !(gmlg->getInt("testColor")&1) ) {
			cout << "IntervalEdgeColoring::test\n";
			int res = Koala::IntervalEdgeColoring::testPart(graph, weights, colors);
			if(res==0) {
				cout<<"TEST " << testId << " - initial coloring is not correct\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("greedy")&1) ) {
			cout << "IntervalEdgeColoring::greedy\n";
			tmpColors = colors;
			int res = Koala::IntervalEdgeColoring::greedy(graph, weights, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::IntervalEdgeColoring::test(graph, weights, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - IntervalEdgeColoring::greedy wrong coloring\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("lf")&1) ) {
			cout << "IntervalEdgeColoring::lf\n";
			tmpColors = colors;
			int res = Koala::IntervalEdgeColoring::lf(graph, weights, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::IntervalEdgeColoring::test(graph, weights, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - IntervalEdgeColoring::lf wrong coloring\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("li")&1) ) {
			cout << "IntervalEdgeColoring::li\n";
			tmpColors = colors;
			int res = Koala::IntervalEdgeColoring::li(graph, weights, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::IntervalEdgeColoring::test(graph, weights, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - IntervalEdgeColoring::li wrong coloring\n";
				testPass = false;
			}
		}

		if(testPass) cout<<"TEST " << testId << " - test passed\n";
		else cout<<"TEST " << testId << " - test failed\n";
	}
	return 0;
}

