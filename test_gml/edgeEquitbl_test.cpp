#include <iostream>
#include "../Koala/coloring/equitable.h"
#include "../Koala/io/graphml.h"
#include "../Koala/classes/create.h"
#include <time.h>
#include <math.h>

#define XYZ
using namespace std;

struct CurEdgeInfo {
	std::string name;
	int color;
};
#ifdef XYZ
typedef Koala::Graph<string, CurEdgeInfo> Graph;
#else
typedef Koala::Graph<int, int> Graph;
#endif
typedef Graph::PVertex Vert;
typedef Graph::PEdge Edge;
typedef Koala::AssocArray< Edge,int > Map;

struct Gene {
	int operator()(int i) {return i;}
	int operator()(int i, int j, Koala::EdgeDirection x) {return i+j;}
};

#ifdef XYZ
void print(const Graph &graph, const Map &colors) {
	for(Edge ee = graph.getEdge(); ee; ee = graph.getEdgeNext(ee)) {
		cout << ee->info.name << "(";
		if(colors.hasKey(ee)) cout << colors[ee];
		else cout << " ";
		cout << ") [";
		Vert u = graph.getEdgeEnd1(ee);
		cout << u->info <<", ";
		u = graph.getEdgeEnd2(ee);
		cout << u->info <<"]\n";
	}
}
#endif

#ifdef XYZ
int main(int argc, char **argv) {
	Graph graph;
	Koala::IO::GraphML gmlIn;
	Map colors, tmpColors;

	if(!gmlIn.readFile("edgeEquitbl_test.xml") ) {
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
		bool readRes = gmlg->readGraph(graph, Koala::IO::gmlStringField("vName"),
				Koala::IO::gmlStringField(&CurEdgeInfo::name, "eName")
					& Koala::IO::gmlIntField(&CurEdgeInfo::color, "color")
				);
		if(!readRes) {
			cout << "error - graph[" << testId << "] not read";
			return 3;
		}
		cout<<" "<<gmlg->getName()<<"\n";

		int param = gmlg->getInt("show");
		for(Edge ee = graph.getEdge(); ee; ee = graph.getEdgeNext(ee))
			if(ee->info.color>=0) colors[ee] = ee->info.color;

		if( !(gmlg->getInt("testColor")&1) ) {
			cout << "EquitEdgeColoring::test\n";
			int res = Koala::EquitEdgeColoring::test(graph, colors);
			cout<<"TEST " << testId << " - test answer: " << res << "\n";
			if(res<0) testPass = false;
		}

		if(testPass) {
			cout << "EquitEdgeColoring::repair\n";
			tmpColors = colors;
			int res = Koala::EquitEdgeColoring::repair(graph, tmpColors);
			if(param==1) cout << "repair response: " << res << "\n";
			else if(param==2) print(graph, tmpColors);
			res = Koala::EquitEdgeColoring::test(graph, tmpColors);
			cout<<"TEST " << testId << " - test answer: " << res << "\n";
			if(res<0) testPass = false;
		}

		if(testPass) cout<<"TEST " << testId << " - test passed\n";
		else cout<<"TEST " << testId << " - test failed\n";
	}
	return 0;
}

#else

int main(int argc, char **argv) {
	Graph graph;
	Koala::IO::GraphML gmlIn;
	Map colors;

	//-------------------------------------------------------------------------
	srand(0);
	int lll=50;
	if(argc>1) {
		lll = atoi(argv[1]);
		if(lll<=0) lll=50;
	}
	double ddd=0.5;
	if(argc>2) {
		ddd = atof(argv[2]);
		if(ddd<=0||ddd>1) ddd=0.5;
	}
	for(int testId = 0; testId<10; testId++) {
		bool testPass = true;
		graph.clear();
		colors.clear();

		Koala::Creator::erdRen1(graph, lll, ddd, Gene(), Gene(), Koala::EdUndir);
		//Koala::SeqEdgeColoring::greedy(graph, colors);
		Koala::SeqEdgeColoring::vizing(graph, colors);

		int res = Koala::EquitEdgeColoring::test(graph, colors);
		cout<<"TEST " << testId << " - test answer: " << res;
		if(res<0) testPass = false;
		res = Koala::SeqEdgeColoring::maxColor(graph, colors);
		cout<<"(" << res << ")\n";

		if(testPass) {
			cout << "EquitEdgeColoring::repair: ";
			int res = Koala::EquitEdgeColoring::repair(graph, colors);
			cout << res;
			res = Koala::EquitEdgeColoring::test(graph, colors);
			cout << " - test answer: " << res;
			if(res<0) testPass = false;
			res = Koala::SeqEdgeColoring::maxColor(graph, colors);
			cout<<"(" << res << ")\n";
		}

		if(testPass) cout<<"OK\n";
		else cout<<"TEST " << testId << " - test failed\n";
	}
	return 0;
}

#endif


