#include <iostream>
#include "../Koala/algorithm/coloring.h"
#include "../Koala/io/graphml.h"

using namespace std;

struct GMLVert {
	string name;
	int color;
	int weight;
	GMLVert(const string &n, const int c, const int w):
		name(n), color(c), weight(w) {}
};

typedef Koala::Graph<GMLVert, string> Graph;
typedef Graph::PVertex Vert;
typedef Graph::PEdge Edge;
typedef Koala::IntervalVertColoring::Color IntervalColor;
typedef Koala::AssocTable< std::map< Vert,IntervalColor > > Map;

struct FunktorReadV {
	Graph::VertInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		int color = -1;
		if(keysHolder->isValue("color"))
			color = keysHolder->getInt("color");

		int weight = 1;
		if(keysHolder->isValue("weight"))
			weight = keysHolder->getInt("weight");
		if(keysHolder->isValue("vName"))
			return GMLVert(keysHolder->getString("vName"), color, weight);
		return GMLVert("brak", color, weight);
	}
};

struct FunktorReadE {
	Graph::EdgeInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		if(keysHolder->isValue("eName"))
			return keysHolder->getString("eName");
		return "brak";
	}
};

void print(const Graph &graph, const Map &colors) {
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		cout << vv->info.name << " [";
		if(colors.hasKey(vv)) cout << colors[vv].min <<", " <<colors[vv].max << ") ";
		else cout << " ]";
		cout << " <";
		for(Edge ee = graph.getEdge(vv); ee; ee = graph.getEdgeNext(vv,ee)) {
			Vert u = graph.getEdgeEnd(ee,vv);
			cout << u->info.name <<", ";
		}
		cout << ">\n";
	}
}
int main(int argc, char **argv) {
	Graph graph;
	Koala::IO::GraphML gmlIn;
	Koala::AssocArray<Vert, int> weights;
	Map colors, tmpColors;

	if(!gmlIn.readFile("intervVertCol_test.xml") ) {
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
			break;
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
		for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
			if(vv->info.color>=0) colors[vv] = vv->info.color;
			weights[vv] = vv->info.weight;
		}

		if( !(gmlg->getInt("testColor")&1) ) {
			cout << "IntervalVertColoring::test\n";
			int res = Koala::IntervalVertColoring::testPart(graph, weights, colors);
			if(res==0) {
				cout<<"TEST " << testId << " - initial coloring is not correct\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("greedy")&1) ) {
			cout << "IntervalVertColoring::greedy\n";
			tmpColors = colors;
			int res = Koala::IntervalVertColoring::greedy(graph, weights, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::IntervalVertColoring::test(graph, weights, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - IntervalVertColoring::greedy wrong coloring\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("li")&1) ) {
			cout << "IntervalVertColoring::li\n";
			tmpColors = colors;
			int res = Koala::IntervalVertColoring::li(graph, weights, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::IntervalVertColoring::test(graph, weights, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - IntervalVertColoring::li wrong coloring\n";
				testPass = false;
			}
		}

		if(testPass) cout<<"TEST " << testId << " - test passed\n";
		else cout<<"TEST " << testId << " - test failed\n";
	}
	return 0;
}

