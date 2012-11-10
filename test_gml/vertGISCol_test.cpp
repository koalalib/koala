#define KOALA_SET_ON_HASHSET

#include <iostream>
#include "../Koala/coloring/vertex.h"
#include "../Koala/io/graphml.h"

using namespace std;

typedef Koala::Graph<pair<string,int>, string> Graph;
typedef Graph::PVertex Vert;
typedef Graph::PEdge Edge;
typedef Koala::AssocTable< std::map< Vert,int > > Map;

struct FunktorReadV {
	Graph::VertInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		int tmp = -1;
		if(keysHolder->isValue("color"))
			tmp = keysHolder->getInt("color");
		if(keysHolder->isValue("vName"))
			return make_pair(keysHolder->getString("vName"), tmp);
		return make_pair("brak", tmp);
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

		cout << vv->info.first << " (";
		if(colors.hasKey(vv)) cout << colors[vv];
		else cout << " ";
		cout << ") [";
		for(Edge ee = graph.getEdge(vv); ee; ee = graph.getEdgeNext(vv,ee)) {
			Vert u = graph.getEdgeEnd(ee,vv);
			cout << u->info.first <<", ";
		}
		cout << "]\n";
	}
}
int main(int argc, char **argv) {
	Graph graph;
	Koala::IO::GraphML gmlIn;
	Map colors, tmpColors;

	if(!gmlIn.readFile("vertGISCol_test.xml") ) {
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
		for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv))
			if(vv->info.second>=0) colors[vv] = vv->info.second;

		if( !(gmlg->getInt("testColor")&1) ) {
			cout << "GisVertColoring::test\n";
			int res = Koala::GisVertColoring::testPart(graph, colors);
			if(res==0) {
				cout<<"TEST " << testId << " - initial coloring is not correct\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("colorA")&1) ) {
			cout << "GisVertColoring::color\n";
			tmpColors = colors;
			int res = Koala::GisVertColoring::color(graph, tmpColors);
			if(res>graph.Delta()) {
				cout<<"TEST " << testId << " - SeqVertColoring::color to many colors\n";
				testPass = false;
			}
			if(param==1) cout<<"maxColor:"<<res<<"\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::GisVertColoring::test(graph, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - GisVertColoring::color wrong coloring\n";
				testPass = false;
			}
		}

		if(testPass) cout<<"TEST " << testId << " - test passed\n";
		else cout<<"TEST " << testId << " - test failed\n";
	}
	return 0;
}
