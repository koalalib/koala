#include <iostream>
#include "../Koala/coloring/list.h"
#include "../Koala/io/graphml.h"

using namespace std;

struct GMLVert {
	string name;
	int color;
	string list;
	GMLVert(const string &n, const int c, const string &l):
		name(n), color(c), list(l) {}
};

typedef Koala::Graph<GMLVert, string> Graph;
typedef Graph::PVertex Vert;
typedef Graph::PEdge Edge;
typedef Koala::AssocTable< std::map< Vert,int > > MapColor;
typedef Koala::Set<int> List;
typedef Koala::AssocTable< std::map< Vert, List > > MapList;

struct FunktorReadV {
	Graph::VertInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		int color = -1;
		if(keysHolder->isValue("color"))
			color = keysHolder->getInt("color");

		string list;
		if(keysHolder->isValue("list"))
			list = keysHolder->getString("list");

		if(keysHolder->isValue("vName"))
			return GMLVert(keysHolder->getString("vName"), color, list);
		return GMLVert("brak", color, list);
	}
};

struct FunktorReadE {
	Graph::EdgeInfoType operator()(Koala::IO::GraphMLKeysRead *keysHolder) {
		if(keysHolder->isValue("eName"))
			return keysHolder->getString("eName");
		return "brak";
	}
};

struct RandomChooser {
	int operator()(const Graph &graph, const List &list,
		const MapColor &colors, Vert vv)
	{
		int len = list.size();
		int LOCALARRAY(kolory, len);
		list.getElements(kolory);
		while(len>0) {
			int ch = rand()%len;
			int chCol = kolory[ch];
			--len;
			kolory[ch] = kolory[len];

			Edge ee = graph.getEdge(vv);
			for(;ee; ee = graph.getEdgeNext(ee)) {
				Vert u = graph.getEdgeEnd(ee, vv);
				if(!colors.hasKey(u)) continue;
				int col = colors[u];
				if(col==chCol) break;
			}
			if(ee==NULL) return chCol;
		}
		return -1;
	}
};

void print(const Graph &graph, const MapColor &colors) {
	for(Vert vv = graph.getVert(); vv; vv = graph.getVertNext(vv)) {
		cout << vv->info.name << " (";
		if(colors.hasKey(vv)) cout << colors[vv];
		else cout << " ";
		cout << ") <";
		for(Edge ee = graph.getEdge(vv); ee; ee = graph.getEdgeNext(vv,ee)) {
			Vert u = graph.getEdgeEnd(ee,vv);
			cout << u->info.name <<", ";
		}
		cout << "> [[" << vv->info.list << "]]\n";
	}
}

List strToList(string str) {
	List list;
	int len = str.length();
	int i = 0;
	while(i<len) {
		if(!isdigit(str[i])) {
			i++;
			continue;
		}
		int tmp=0;
		while(i<len&&isdigit(str[i])) {
			tmp*=10;
			tmp+=str[i]-'0';
			i++;
		}
		list.add(tmp);
	}
	return list;
}

int main(int argc, char **argv) {
	Graph graph;
	Koala::IO::GraphML gmlIn;
	MapList mapLists;
	MapColor colors, tmpColors;

	srand(1); //constant number to get the same results
	if(!gmlIn.readFile("listVertCol_test.xml") ) {
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
		mapLists.clear();
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
			mapLists[vv] = strToList(vv->info.list);
		}

		if( !(gmlg->getInt("testColor")&1) ) {
			cout << "ListVertColoring::test\n";
			int res = Koala::ListVertColoring::testPart(graph, mapLists, colors);
			if(res==0) {
				cout<<"TEST " << testId << " - initial coloring is not correct\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("colorA")&1) ) {
			cout << "ListVertColoring::color\n";
			tmpColors = colors;
			int res = Koala::ListVertColoring::color(graph, mapLists, tmpColors);
			cout << res << endl;
			if(param==1) print(graph, tmpColors);
			res = Koala::ListVertColoring::testPart(graph, mapLists, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - ListVertColoring::color wrong coloring\n";
				testPass = false;
			}
		}

		if( !(gmlg->getInt("colorChoose")&1) ) {
			cout << "ListVertColoring::colorChoose\n";
			tmpColors = colors;
			int res = Koala::ListVertColoring::colorChoose(graph, mapLists,
				tmpColors, RandomChooser());
			cout << res << endl;
			if(param==1) print(graph, tmpColors);
			res = Koala::ListVertColoring::testPart(graph, mapLists, tmpColors);
			if(res==0) {
				cout<<"TEST " << testId << " - ListVertColoring::colorChoose wrong coloring\n";
				testPass = false;
			}
		}

		if(testPass) cout<<"TEST " << testId << " - test passed\n";
		else cout<<"TEST " << testId << " - test failed\n";
	}
	return 0;
}


