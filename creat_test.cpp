#include <iostream>
#include <string>
#include <sstream>
#include "Koala/graph/graph.h"
#include "Koala/classes/create.h"
#include "Koala/io/g6.h"


Koala::StdRandGen<> stdgen;
//przyklad uzycia, w tej wersji zawartosc obu struktur jest dla Dijkstry nieistotna
struct OpisV;  // pewna struktura opisujaca moj wierzchlek
struct OpisE;   // struktura opisujaca krawedz

struct OpisV {
    OpisV(std::string aname="") : name(aname) {}

    std::string name;
};

struct OpisE {
    OpisE(int len=0) : dlugosc(len) {}
	OpisE(int _v1, int _v2, Koala::EdgeDirection _dir) : v1(_v1), v2(_v2), dir(_dir) {}
    bool cokolwiek;
    int dlugosc; // dla Dijkstry niepotrzebne
	int v1;
	int v2;
	Koala::EdgeDirection dir;

};

class OpisVFunctor {
    public:
    OpisVFunctor() {}
    OpisV operator()() { return OpisV(); }
    OpisV operator()(int num) { ;
		std::string name = "Vertex_";
		std::ostringstream ss;
		ss << num;
		std::string numStr = ss.str();
		OpisV * opisV = new OpisV(name+numStr);
		return *opisV ;
	}

};

class OpisEFunctor {
    public:
    OpisEFunctor() {}
    OpisE operator()() { return OpisE(); }
	OpisE operator()(int num1, int num2, Koala::EdgeDirection dir) { ;
		OpisE * opisE = new OpisE(num1, num2, dir);
		return *opisE ;
	}

};

int test2();
int test3();
void printGraph(Koala::Graph<OpisV,OpisE>& g);
void dir2str(Koala::EdgeDirection dir, std::string & dirStr );

#include "main.hpp"

	Koala::Graph<OpisV,OpisE> g1;

	Koala::Creator::clique(g1,7,Koala::Detached);

	Koala::Creator::clique(g1,7,Koala::ConstFunctor <OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::Undirected);

	Koala::Graph<OpisV,OpisE> g2;
	Koala::Creator::clique(g2,7,Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::EdAll);

	Koala::Graph<OpisV,OpisE> g3;
	Koala::Creator::path(g3,10 ,Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::EdUndir | Koala::EdDirIn);

	Koala::Creator::path(g3, 10, Koala::EdUndir | Koala::EdDirIn);

	Koala::Graph<OpisV,OpisE> g3a;
	Koala::Creator::path(g3a,10 ,Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>());

	Koala::Graph<OpisV,OpisE> g4;
	Koala::Creator::cycle(g4, 4, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::EdUndir | Koala::EdDirIn);
	Koala::Creator::cycle(g4, 4);

	Koala::Graph<OpisV,OpisE> g5;
	Koala::Creator::fan(g5, 20, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>());

	Koala::Creator::fan(g5, 20);

	Koala::Graph<OpisV,OpisE> g6;
	Koala::Creator::wheel(g6, 2000, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::EdUndir | Koala::EdDirIn);

	Koala::Creator::wheel(g6, 2000);

	Koala::Graph<OpisV,OpisE> g7;
	Koala::Creator::compBipartite(g7, 4, 7, Koala::ConstFunctor<OpisV>(), Koala::ConstFunctor<OpisE>(), Koala::EdUndir | Koala::EdDirIn);

	Koala::Creator::compBipartite(g7, 4, 7, Koala::EdUndir | Koala::EdDirIn);

	Koala::Graph<OpisV,OpisE> g8;
	std::vector<int> vec;
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(7);

	vec.begin();
	int t=0;

	std::vector<std::pair< Koala::Graph<OpisV,OpisE>::PVertex,  Koala::Graph<OpisV,OpisE>::PVertex> > vec_insert(8);

	Koala::Creator::compKPartite(g8, vec.begin(), vec.end(), vec_insert.begin(),  Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::EdUndir | Koala::EdDirIn);

	Koala::Creator::compKPartite(g8, vec.begin(), vec.end(), vec_insert.begin());

	Koala::Graph<OpisV,OpisE> g9;
	std::vector<std::pair< Koala::Graph<OpisV,OpisE>::PVertex,  Koala::Graph<OpisV,OpisE>::PVertex> > vec_insert2(14);

	Koala::Creator::compKPartite(g9, 7, 3, vec_insert2.begin(), Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(),  Koala::EdUndir | Koala::EdDirIn);

	Koala::Creator::compKPartite(g9, 7, 3, vec_insert2.begin(), Koala::EdUndir | Koala::EdDirIn);
	Koala::Graph<OpisV,OpisE> g10;

	Koala::Creator::petersen(g10, Koala::ConstFunctor<OpisV>(), Koala::ConstFunctor<OpisE>());

	Koala::Creator::petersen(g10);

	Koala::Graph<OpisV,OpisE> g11;
	std::vector<int> vec2;
	vec2.push_back(2);
	vec2.push_back(3);
	vec2.push_back(4);
	vec2.push_back(6);

	Koala::Creator::regTree(g11, vec2.begin(), vec2.end(), Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>());

	Koala::Creator::regTree(g11, vec2.begin(), vec2.end());

	Koala::Graph<OpisV,OpisE> g12;

	Koala::Creator::regTree(g12,2,5, Koala::ConstFunctor<OpisV>(), Koala::ConstFunctor<OpisE>(), Koala::EdUndir | Koala::EdDirIn );

	Koala::Graph<OpisV,OpisE> g13;

	Koala::Creator::regTree(g13, 5, 2, Koala::ConstFunctor<OpisV>(), Koala::ConstFunctor<OpisE>(), Koala::EdUndir | Koala::EdDirIn);

	Koala::Creator::regTree(g13, 5, 2);

	Koala::Graph<OpisV,OpisE> g14;

	Koala::Creator::erdRen1(stdgen,g14,10, 0.3, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>());

	Koala::Graph<OpisV,OpisE> g15;

	Koala::Creator::erdRen1(stdgen,g15, 12, 0.5, Koala::ConstFunctor<OpisV>(), Koala::ConstFunctor<OpisE>(), Koala::Directed);

	Koala::Graph<OpisV,OpisE> g16;

	Koala::Creator::erdRen2(stdgen,g16, 12, 10, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::Directed);

	Koala::Graph<OpisV,OpisE> g17;

	Koala::Creator::erdRen2(stdgen,g17, 12, 15, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>());

	Koala::Graph<OpisV,OpisE> g18;

	Koala::Creator::erdRen1(stdgen,g18,10, 1, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>());

	Koala::Graph<OpisV,OpisE> g19;

	Koala::Creator::erdRen1(stdgen,g19,10, 1, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::Directed);

	Koala::Graph<OpisV,OpisE> g20;

	Koala::Creator::erdRen1(stdgen,g20,10, 0, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::Directed);

	Koala::Graph<OpisV,OpisE> g21;

	Koala::Creator::erdRen2(stdgen,g21, 12, 0, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>(), Koala::Directed);

	Koala::Graph<OpisV,OpisE> g22;

	Koala::Creator::erdRen2(stdgen,g22, 12, 66, Koala::ConstFunctor<OpisV>(),Koala::ConstFunctor<OpisE>());

	Koala::Graph<OpisV,OpisE> g23;
	Koala::Creator::erdRen1(stdgen,g23,10, 0.7);

	Koala::Graph<OpisV,OpisE> g24;

	Koala::Creator::erdRen2(stdgen,g24, 12, 65);


	test2();
	test3();

	return 0;
}


int test2(){

	std::cout<< "--test2" <<std::endl;

	Koala::Graph<OpisV,OpisE> g1;
	OpisVFunctor vFunktor;
	OpisEFunctor eFunktor;

	Koala::Creator::clique(g1, 7, Koala::Detached);
//	g1.testGraph();
	std::cout<< "--g1" <<std::endl;
	printGraph(g1);

	Koala::Graph<OpisV,OpisE> g1a;
	Koala::Creator::clique(g1a, 7, vFunktor, eFunktor, Koala::Undirected);
//	g1a.testGraph();
	std::cout<< "--g1a" <<std::endl;
	printGraph(g1a);

	Koala::Graph<OpisV,OpisE> g2;
	Koala::Creator::path(g2, 10, vFunktor, eFunktor, Koala::EdUndir | Koala::EdDirIn);
//	g2.testGraph();
	std::cout<< "--g2" <<std::endl;
	printGraph(g2);

	Koala::Graph<OpisV,OpisE> g3;
	Koala::Creator::cycle(g3, 4, vFunktor, eFunktor, Koala::EdDirIn);
//	g3.testGraph();
	std::cout<< "--g3" <<std::endl;
	printGraph(g3);

	Koala::Graph<OpisV,OpisE> g4;
	Koala::Creator::fan(g4, 4, vFunktor, eFunktor);
//	g4.testGraph();
	std::cout<< "--g4" <<std::endl;
	printGraph(g4);

	Koala::Graph<OpisV,OpisE> g4a;
	Koala::Creator::fan(g4a, 5, vFunktor, eFunktor, Koala::EdUndir,  Koala::EdDirIn);
//	g4a.testGraph();
	std::cout<< "--g4a" <<std::endl;
	printGraph(g4a);

	Koala::Graph<OpisV,OpisE> g5;
	Koala::Creator::wheel(g5, 5, vFunktor, eFunktor, Koala::EdUndir | Koala::EdDirIn);
//	g5.testGraph();
	std::cout<< "--g5" <<std::endl;
	printGraph(g5);

	Koala::Graph<OpisV,OpisE> g5a;
	Koala::Creator::wheel(g5a, 6, vFunktor, eFunktor, Koala::EdUndir, Koala::EdDirIn);
//	g5a.testGraph();
	std::cout<< "--g5a" <<std::endl;
	printGraph(g5a);

	Koala::Graph<OpisV,OpisE> g6;
	Koala::Creator::compBipartite(g6, 2, 3, vFunktor, eFunktor, Koala::EdDirIn);
	std::cout<< "--g6" <<std::endl;
//	g6.testGraph();
	printGraph(g6);

	Koala::Graph<OpisV,OpisE> g7;

	std::vector<int> vec;
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);

	std::vector<std::pair< Koala::Graph<OpisV,OpisE>::PVertex,  Koala::Graph<OpisV,OpisE>::PVertex> > vec_insert(8);
	Koala::Creator::compKPartite(g7, vec.begin(), vec.end(), vec_insert.begin(), vFunktor, eFunktor, Koala::EdDirIn);
	std::cout<< "--g7" <<std::endl;
//	g7.testGraph();
	printGraph(g7);

	Koala::Graph<OpisV,OpisE> g8;
	Koala::Creator::petersen(g8, vFunktor, eFunktor);
	std::cout<< "--g8" <<std::endl;
//	g8.testGraph();
	printGraph(g8);

	Koala::Graph<OpisV,OpisE> g9;
	std::vector<int> vec2;
	vec2.push_back(2);
	vec2.push_back(3);
	vec2.push_back(4);

	Koala::Creator::regTree(g9, vec2.begin(), vec2.end(), vFunktor, eFunktor, Koala::EdDirIn);
	std::cout<< "--g9" <<std::endl;
//	g9.testGraph();
	printGraph(g9);

	Koala::Graph<OpisV,OpisE> g10;
	Koala::Creator::regTree(g10, 5, 2,vFunktor, eFunktor, Koala::EdDirOut);
	std::cout<< "--g10" <<std::endl;
//	g10.testGraph();
	printGraph(g10);

	Koala::Graph<OpisV,OpisE> g11;
	Koala::Creator::erdRen1(stdgen,g11,10, 0.3, vFunktor, eFunktor);
	std::cout<< "--g11" <<std::endl;
//	g11.testGraph();
	printGraph(g11);

	Koala::Graph<OpisV,OpisE> g12;
	Koala::Creator::erdRen1(stdgen,g12,10, 1, vFunktor, eFunktor, Koala::Directed);
	std::cout<< "--g12" <<std::endl;
//	g12.testGraph();
	printGraph(g12);

	Koala::Graph<OpisV,OpisE> g13;
	Koala::Creator::erdRen2(stdgen,g13, 12, 66, vFunktor, eFunktor);
	std::cout<< "--g13" <<std::endl;
//	g13.testGraph();
	printGraph(g13);

	Koala::Graph<OpisV,OpisE> g14;
	Koala::Creator::erdRen2(stdgen,g14, 12, 0, vFunktor, eFunktor, Koala::Directed);
	std::cout<< "--g14" <<std::endl;
//	g14.testGraph();
	printGraph(g14);

	Koala::Graph<OpisV,OpisE> g15;
	Koala::Creator::erdRen2(stdgen,g15, 4, 1, vFunktor, eFunktor, Koala::Directed);
	std::cout<< "--g15" <<std::endl;
//	g15.testGraph();
	printGraph(g15);

	Koala::Graph<OpisV,OpisE> g16;
	Koala::Creator::erdRen2(stdgen,g16, 4, 1, vFunktor, eFunktor,  Koala::Directed);
	std::cout<< "--g16" <<std::endl;
//	g16.testGraph();
	printGraph(g16);

	Koala::Graph<OpisV,OpisE> g16a;
	Koala::Creator::erdRen2(stdgen,g16a, 4, 1, vFunktor, eFunktor,  Koala::Directed);
	std::cout<< "--g16a" <<std::endl;
//	g16a.testGraph();
	printGraph(g16a);

	Koala::Graph<OpisV,OpisE> g17;
	Koala::Creator::erdRen2(stdgen,g17, 4, 1, vFunktor, eFunktor, Koala::Directed);
	std::cout<< "--g17" <<std::endl;
//	g17.testGraph();
	printGraph(g17);

	Koala::Graph<OpisV,OpisE> g18;
	Koala::Creator::erdRen2(stdgen,g18, 4, 1, vFunktor, eFunktor, Koala::Directed);
	std::cout<< "--g18" <<std::endl;
//	g18.testGraph();
	printGraph(g18);


	std::vector<int> vec3;
	vec3.push_back(2);
	vec3.push_back(3);
	vec3.push_back(4);

	Koala::Graph<OpisV,OpisE> g19;
	Koala::Creator::caterpillar(g19, vec3.begin(), vec3.end(), vFunktor, eFunktor, Koala::EdUndir, Koala::EdDirIn);
	std::cout<< "--g19" <<std::endl;
//	g19.testGraph();
	printGraph(g19);

	Koala::Graph<OpisV,OpisE> g20;
	Koala::Creator::caterpillar(g20, 4, 2, vFunktor, eFunktor, Koala::EdUndir, Koala::EdDirIn);
	std::cout<< "--g20" <<std::endl;
//	g20.testGraph();
	printGraph(g20);


	return 0;
}

int test3(){

	std::cout<< "--test3" <<std::endl;

	Koala::Graph<OpisV,OpisE> g1;
	OpisVFunctor vFunktor;
	OpisEFunctor eFunktor;

	Koala::Creator::clique(g1, 1, Koala::Detached);
//	g1.testGraph();
	std::cout<< "--g1" <<std::endl;
	printGraph(g1);

	Koala::Graph<OpisV,OpisE> g1a;
	Koala::Creator::clique(g1a, 1, vFunktor, eFunktor, Koala::Undirected);
//	g1a.testGraph();
	std::cout<< "--g1a" <<std::endl;
	printGraph(g1a);

	Koala::Graph<OpisV,OpisE> g2;
	Koala::Creator::path(g2, 1, vFunktor, eFunktor, Koala::EdUndir | Koala::EdDirIn);
//	g2.testGraph();
	std::cout<< "--g2" <<std::endl;
	printGraph(g2);

	Koala::Graph<OpisV,OpisE> g3;
	Koala::Creator::cycle(g3, 1, vFunktor, eFunktor, Koala::EdDirIn);
//	g3.testGraph();
	std::cout<< "--g3" <<std::endl;
	printGraph(g3);

	Koala::Graph<OpisV,OpisE> g4;
	Koala::Creator::fan(g4, 1, vFunktor, eFunktor);
//	g4.testGraph();
	std::cout<< "--g4" <<std::endl;
	printGraph(g4);

	Koala::Graph<OpisV,OpisE> g4a;
	Koala::Creator::fan(g4a, 1, vFunktor, eFunktor, Koala::EdUndir,  Koala::EdDirIn);
//	g4a.testGraph();
	std::cout<< "--g4a" <<std::endl;
	printGraph(g4a);

	Koala::Graph<OpisV,OpisE> g5;
	Koala::Creator::wheel(g5, 1, vFunktor, eFunktor, Koala::EdUndir | Koala::EdDirIn);
//	g5.testGraph();
	std::cout<< "--g5" <<std::endl;
	printGraph(g5);

	Koala::Graph<OpisV,OpisE> g5a;
	Koala::Creator::wheel(g5a, 1, vFunktor, eFunktor, Koala::EdUndir, Koala::EdDirIn);
//	g5a.testGraph();
	std::cout<< "--g5a" <<std::endl;
	printGraph(g5a);

	Koala::Graph<OpisV,OpisE> g6;
	Koala::Creator::compBipartite(g6, 1, 1, vFunktor, eFunktor, Koala::EdDirIn);
	std::cout<< "--g6" <<std::endl;
//	g6.testGraph();
	printGraph(g6);

	Koala::Graph<OpisV,OpisE> g7;

	std::vector<int> vec;
	vec.push_back(1);


	std::vector<std::pair< Koala::Graph<OpisV,OpisE>::PVertex,  Koala::Graph<OpisV,OpisE>::PVertex> > vec_insert(8);
	Koala::Creator::compKPartite(g7, vec.begin(), vec.end(), vec_insert.begin(), vFunktor, eFunktor, Koala::EdDirIn);
	std::cout<< "--g7" <<std::endl;
//	g7.testGraph();
	printGraph(g7);


	Koala::Graph<OpisV,OpisE> g9;
	std::vector<int> vec2;
	vec2.push_back(1);

	Koala::Creator::regTree(g9, vec2.begin(), vec2.end(), vFunktor, eFunktor, Koala::EdDirIn);
	std::cout<< "--g9" <<std::endl;
//	g9.testGraph();
	printGraph(g9);

	Koala::Graph<OpisV,OpisE> g10;
	Koala::Creator::regTree(g10, 0, 0,vFunktor, eFunktor, Koala::EdDirOut);
	std::cout<< "--g10" <<std::endl;
//	g10.testGraph();
	printGraph(g10);

	Koala::Graph<OpisV,OpisE> g11;
	Koala::Creator::erdRen1(stdgen,g11,1, 0.3, vFunktor, eFunktor);
	std::cout<< "--g11" <<std::endl;
//	g11.testGraph();
	printGraph(g11);

	Koala::Graph<OpisV,OpisE> g12;
	Koala::Creator::erdRen1(stdgen,g12,1, 1, vFunktor, eFunktor, Koala::Directed);
	std::cout<< "--g12" <<std::endl;
//	g12.testGraph();
	printGraph(g12);


	Koala::Graph<OpisV,OpisE> g16;
	Koala::Creator::erdRen2(stdgen,g16, 2, 2, vFunktor, eFunktor,  Koala::Directed);
	std::cout<< "--g16" <<std::endl;
//	g16.testGraph();
	printGraph(g16);

	Koala::Graph<OpisV,OpisE> g17;
	Koala::Creator::caterpillar(g17, 1, 0, vFunktor, eFunktor, Koala::EdAll );
	std::cout<< "--g17" <<std::endl;
//	g17.testGraph();
	printGraph(g17);

	Koala::Graph<OpisV,OpisE> g18;
	Koala::Creator::caterpillar(g18, 1, 1,vFunktor, eFunktor, Koala::EdAll, Koala::EdAll);
	std::cout<< "--g18" <<std::endl;
//	g18.testGraph();
	printGraph(g18);

	Koala::Graph<OpisV,OpisE> g19;
	Koala::Creator::clique(g19, 4, vFunktor, eFunktor,Koala::EdAll);
//	g19.testGraph();
	std::cout<< "--g19" <<std::endl;
	printGraph(g19);

	return 0;
}


void printGraph(Koala::Graph<OpisV,OpisE>& g){
	int i=0;
	std::string v_name;

	std::cout<< "---- Graph info start" <<std::endl;
	int n = g.getVertNo();
	int loop_num = g.getEdgeNo(Koala::EdLoop);
	int undir_num = g.getEdgeNo(Koala::EdUndir);
	int dir_num = g.getEdgeNo(Koala::EdDirIn | Koala::EdDirOut);
	std::cout << "n: " << n <<std::endl;
	std::cout << "loop num: " << loop_num  << std::endl;
	std::cout << "undir_uum: " << undir_num  << std::endl;
	std::cout << "dir_num: " << dir_num << std::endl;

	for(  Koala::Graph<OpisV,OpisE>::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
	{
		v_name = v->getInfo().name;
		std::cout<< i<< ": " << v_name << std::endl;

		Koala::Set< Koala::Graph<OpisV,OpisE>::PEdge > eSet = g.getEdgeSet( v, Koala::EdAll );
		Koala::Set< Koala::Graph<OpisV,OpisE>::PEdge >::iterator it;

		for (it=eSet.begin(); it!=eSet.end(); it++)
		{
			Koala::Graph<OpisV,OpisE>::PEdge e = *it;

			std::pair<Koala::Graph<OpisV,OpisE>::PVertex, Koala::Graph<OpisV,OpisE>::PVertex> vp = e->getEnds();

			Koala::Graph<OpisV,OpisE>::PVertex v1 = vp.first;
			Koala::Graph<OpisV,OpisE>::PVertex v2 = vp.second;
			int v1Num = g.vertPos(v1);
			int v2Num = g.vertPos(v2);
			Koala::EdgeDirection dir = e->getDir(v);

			int e_v1Info = e->getInfo().v1;
			int e_v2Info = e->getInfo().v2;

			std::string dirStr;
			dir2str(dir,dirStr);

			std::cout<<  v1Num<<"-" << v2Num<< " dir: " << dirStr << " info_v1: " <<e_v1Info << " info_v2: " << e_v2Info<< std::endl;
		}
		i++;
	}
	std::cout<< "---! Graph info end" <<std::endl;

}

void dir2str(Koala::EdgeDirection dir, std::string & dirStr){
	dirStr="";
	int intDir = (int)dir;
	switch(intDir){
		case 0x00: dirStr="EdNone"; break;
		case 0x01: dirStr="EdLoop"; break;
		case 0x02: dirStr="EdUndir"; break;
		case 0x04: dirStr="EdDirIn"; break;
		case 0x08: dirStr="EdDirOut"; break;
		default:
			std::ostringstream ss;
			ss << intDir;
			dirStr = ss.str();

	}
}
