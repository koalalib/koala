#define KOALA_SET_ON_HASHSET

#include <iostream>
#include "../Koala/coloring/edge.h"
#include "../Koala/io/graphml.h"

using namespace std;

typedef Koala::Graph<string, pair<string,int> > Graph;
typedef Graph::PVertex Vert;
typedef Graph::PEdge Edge;
typedef Koala::AssocTable< std::map< Edge,int > > Map;

struct FunktorReadV {
	Graph::VertInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		if(keysHolder->isValue("vName"))
			return keysHolder->getString("vName");
		return "brak";
	}
};

struct FunktorReadE {
	Graph::EdgeInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		int tmp = -1;
		if(keysHolder->isValue("color"))
			tmp = keysHolder->getInt("color");
		if(keysHolder->isValue("eName"))
			return make_pair(keysHolder->getString("eName"), tmp);
		return make_pair("brak", tmp);
	}
};

void print(const Graph &graph, const Map &colors) {
	for(Edge ee = graph.getEdge(); ee; ee = graph.getEdgeNext(ee)) {
		Vert v1 = graph.getEdgeEnd1(ee);
		Vert v2 = graph.getEdgeEnd2(ee);
		cout << ee->info.first << "(";

		if(colors.hasKey(ee)) cout << colors[ee];
		else cout << " ";
		cout << ") [" <<v1->info<<","<<v2->info<<"]\n";
	}
}

int main(int argc, char **argv) {
	Graph graph;
	Koala::IO::GraphML gmlIn;
	Map colors, tmpColors;

	if(!gmlIn.readFile("edgeCol_test.xml") ) {
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
		for(Edge ee = graph.getEdge(); ee; ee = graph.getEdgeNext(ee))
			if(ee->info.second>=0) colors[ee] = ee->info.second;

		if( !(gmlg->getInt("testColor")&1) ) {
			cout << "SeqEdgeColoring::test\n";
			int res = Koala::SeqEdgeColoring::testPart(graph, colors);
			if(res==0) {
				cout<<"TEST " << testId << " - initial coloring is not correct\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("greedy")&1) ) {
			cout << "SeqEdgeColoring::greedy\n";
			tmpColors = colors;
			int res = Koala::SeqEdgeColoring::greedy(graph, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::SeqEdgeColoring::test(graph, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - SeqEdgeColoring::greedy wrong coloring\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("greedyInter")&1) ) {
			cout << "SeqEdgeColoring::greedyInter\n";
			tmpColors = colors;
			int res = Koala::SeqEdgeColoring::greedyInter(graph, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::SeqEdgeColoring::test(graph, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - SeqEdgeColoring::greedyInter wrong coloring\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("vizingSimple")&1) ) {
			cout << "SeqEdgeColoring::vizingSimple\n";
			tmpColors = colors;
			int res = Koala::SeqEdgeColoring::vizingSimple(graph, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::SeqEdgeColoring::test(graph, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - SeqEdgeColoring::vizingSimple wrong coloring\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("vizing")&1) ) {
			cout << "SeqEdgeColoring::vizing\n";
			tmpColors = colors;
			int res = Koala::SeqEdgeColoring::vizing(graph, tmpColors);
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::SeqEdgeColoring::test(graph, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - SeqEdgeColoring::vizing wrong coloring\n";
				testPass = false;
			}
		}

		if(testPass) cout<<"TEST " << testId << " - test passed\n";
		else cout<<"TEST " << testId << " - test failed\n";
	}

	return 0;
}
