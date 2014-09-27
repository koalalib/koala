#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "Koala/tools/drawcreator.h"
//#include "Koala/tools/drawcreator.cpp"
#include "Koala/io/text.h"
#include "Koala/graph/graph.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

int graphTests();
int lineGraphTests();
int productTests();
int copyParamLineGraphTests();
int copyParamProductTests();
void createIdsForVertices(Graph<ParSet, ParSet> &g, const char * name, const char * prefix);
void createIdsForEdges(Graph<ParSet, ParSet> &g, const char * name, const char * prefix);

int main()
{

	graphTests();
	lineGraphTests();
	productTests();
	copyParamLineGraphTests();
	copyParamProductTests();

	return 0;
}

int graphTests() {

	std::vector<int> vec;
	Koala::Graph<ParSet, ParSet> g;
	std::ofstream out;
	Koala::StdRandGen<> rgen;

	//Create clique
	Koala::DrawCreator::clique(g, 7, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph0-clique.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create empty graph
	g.clear();
	Koala::DrawCreator::empty(g, 7, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph0b-empty.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create cycle
	g.clear();
	Koala::DrawCreator::cycle(g, 10, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph1-cycle.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();


	//Create 1-vertex cycle
	g.clear();
	Koala::DrawCreator::cycle(g, 1, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph1b-cycle.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create 1-vertex cycle with all types of edges = 4 loops
	g.clear();
	Koala::DrawCreator::cycle(g, 1, 300, EdAll);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph1c-cycle.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create 1-vertex cycle with no edges = empty graph with 1 vertex
	g.clear();
	Koala::DrawCreator::cycle(g, 1, 300, EdNone);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph1d-cycle.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create Petersen's graph
	g.clear();
	Koala::DrawCreator::petersen(g, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph2-petersen.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create horizontal path
	g.clear();
	Koala::DrawCreator::path(g, 8, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph3-path-horiz.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create vertical path
	g.clear();
	Koala::DrawCreator::path(g, 8, 300, false);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph3-path-vert.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();


	//Create wheel
	g.clear();
	Koala::DrawCreator::wheel(g, 11, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph4-wheel.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create fan
	g.clear();
	Koala::DrawCreator::fan(g, 9, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph5-fan.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create complete bipartite graph
	g.clear();
	Koala::DrawCreator::compBipartite(g, 5, 3, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph6-bipartite-graph.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create complete k-partite graph
	g.clear();
	Koala::DrawCreator::compKPartite(g, 3, 4, 300);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph7- k-partite-graph.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create regular tree
	g.clear();
	Koala::DrawCreator::regTree(g, 5, 3, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph8-regular-tree.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	Koala::StdRandGen<> stdgen;
	//Create a graph according to ErdosRenyi model G(n,p)
	g.clear();
	Koala::DrawCreator::erdRen1(rgen, g, 5, 0.7, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph9-rand-erd-ren1.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();


	//Create a graph according to ErdosRenyi model G(n,m).
	g.clear();
	Koala::DrawCreator::erdRen2(rgen, g, 8, 12, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph10-rand-erd-ren2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create a caterpillar
	g.clear();
	Koala::DrawCreator::caterpillar(g, 5, 8, 500);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph11-caterpillar.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create tree 2

	vec.push_back(5);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);

	g.clear();
	Koala::DrawCreator::tree(g, vec, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph12-tree.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create k-partite graph 2
	vec.clear();
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);

	g.clear();
	Koala::DrawCreator::compKPartite(g, vec, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph13-k-partite-graph2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create k-partite graph 3
	vec.clear();
	vec.push_back(2);

	g.clear();
	Koala::DrawCreator::compKPartite(g, vec, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph14-k-partite-graph3.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create k-partite graph 4
	vec.clear();
	vec.push_back(2);
	vec.push_back(1);

	g.clear();
	Koala::DrawCreator::compKPartite(g, vec, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph15-k-partite-graph4.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create k-partite graph 5
	vec.clear();
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);

	g.clear();
	Koala::DrawCreator::compKPartite(g, vec, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph16-k-partite-graph5.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create a caterpillar 2
	vec.clear();
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);

	g.clear();
	Koala::DrawCreator::caterpillar(g, vec, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph17-caterpillar2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create a caterpillar 3
	g.clear();
	Koala::DrawCreator::caterpillar(g, 5, 1, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph18-caterpillar3.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create a caterpillar 4
	g.clear();
	Koala::DrawCreator::caterpillar(g, 5, 2, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph19-caterpillar4.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create a caterpillar 5
	g.clear();
	Koala::DrawCreator::caterpillar(g, 5, 3, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph20-caterpillar5.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create a caterpillar 6
	g.clear();
	Koala::DrawCreator::caterpillar(g, 5, 4, 400);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-graph21-caterpillar6.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	return 0;
}


int lineGraphTests() {

	Koala::Graph<ParSet, ParSet> g;
	Koala::Graph<ParSet, ParSet> lg;
	std::ofstream out;

	g.clear();
	Koala::DrawCreator::clique(g, 5, 300);
	Koala::Graph<ParSet, ParSet>::PVertex v = g.vertByNo(0);

	g.addLoop(v, ParSet());
	g.addLoop(v, ParSet());
	g.addLoop(v, ParSet());

	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("test-line-graph-clique-in.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	lg.clear();
	Koala::DrawCreator::undir(g, lg, 300);

	Koala::IO::writeGraphText(lg, std::cout, Koala::IO::RG_VInfo);
	out.open("test-line-graph-clique1-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(lg, out, Koala::IO::RG_VInfo);
	out.close();

	lg.clear();
	Koala::DrawCreator::undir(g, lg, 300, "", "");

	Koala::IO::writeGraphText(lg, std::cout, Koala::IO::RG_VInfo);
	out.open("test-line-graph-clique2-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(lg, out, Koala::IO::RG_VInfo);
	out.close();

	return 0;
}


int productTests() {

	Koala::Graph<ParSet, ParSet> g1;
	Koala::Graph<ParSet, ParSet> g2;
	Koala::Graph<ParSet, ParSet> gp;
	std::ofstream out;


	g1.clear();
	g2.clear();
	Koala::DrawCreator::path(g1, 3, 300);
	Koala::DrawCreator::path(g2, 4, 300, false);

	Koala::IO::writeGraphText(g1, std::cout, Koala::IO::RG_Info);
	out.open("test-graph-product-path1-1-in.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g1, out, Koala::IO::RG_Info);
	out.close();

	Koala::IO::writeGraphText(g2, std::cout, Koala::IO::RG_Info);
	out.open("test-graph-product-path2-1-in.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g2, out, Koala::IO::RG_Info);
	out.close();

	DrawCreator::strong(g1, g2, gp, 300, 300);

	Koala::IO::writeGraphText(gp, std::cout, Koala::IO::RG_Info);
	out.open("test-graph-product-strong-1-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	g1.clear();
	g2.clear();
	gp.clear();
	Koala::DrawCreator::path(g1, 3, 300);
	Koala::DrawCreator::cycle(g2, 5, 300);
	DrawCreator::strong(g1, g2, gp, 300, 300);
	out.open("test-graph-product-strong-2-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	gp.clear();
	DrawCreator::cart(g1, g2, gp, 300, 300);
	out.open("test-graph-product-cart-2-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	gp.clear();
	DrawCreator::tensor(g1, g2, gp, 300, 300);
	out.open("test-graph-product-tensor-2-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	gp.clear();
	DrawCreator::lex(g1, g2, gp, 300, 300);
	out.open("test-graph-product-lex-2-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	g1.clear();
	g2.clear();
	gp.clear();
	Koala::DrawCreator::cycle(g1, 4, 300);
	Koala::DrawCreator::cycle(g2, 5, 300);
	DrawCreator::strong(g1, g2, gp, 300, 300);
	out.open("test-graph-product-strong-3-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	return 0;
}

int copyParamLineGraphTests() {

	Koala::Graph<ParSet, ParSet> g;
	Koala::Graph<ParSet, ParSet> lg;
	std::ofstream out;

	g.clear();
	Koala::DrawCreator::clique(g, 5, 300);

	createIdsForVertices(g, "v-id", "v");
	createIdsForEdges(g, "e-id", "e");
	out.open("test-copy-param-lineGraph1-in.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	Koala::DrawCreator::undir(g, lg, 300, "v-id","e-id");
	out.open("test-copy-param-lineGraph1-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(lg, out, Koala::IO::RG_Info);
	out.close();

	return 0;
}

int copyParamProductTests() {

	Koala::Graph<ParSet, ParSet> g1;
	Koala::Graph<ParSet, ParSet> g2;
	Koala::Graph<ParSet, ParSet> gp;
	std::ofstream out;


	Koala::DrawCreator::path(g1, 3, 300);
	createIdsForVertices(g1, "v1", "v1-");
	createIdsForEdges(g1, "e1", "e1-");

	Koala::DrawCreator::path(g2, 4, 300, false);
	createIdsForVertices(g2, "v2", "v2-");
	createIdsForEdges(g2, "e2", "e2-");

	out.open("test-copy-param-product1-in1.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g1, out, Koala::IO::RG_Info);
	out.close();

	out.open("test-copy-param-product1-in2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g2, out, Koala::IO::RG_Info);
	out.close();

	gp.clear();
	DrawCreator::cart(g1, g2, gp, 300, 300, std::make_pair("v1", "v2"), std::make_pair("e1", "e2"));

	out.open("test-copy-param-product1a-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	gp.clear();
	DrawCreator::strong(g1, g2, gp, 300, 300, std::make_pair("v1", "v2"), std::make_pair("e1", "e2"));

	out.open("test-copy-param-product1b-out.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(gp, out, Koala::IO::RG_Info);
	out.close();

	return 0;
}

void createIdsForVertices(Graph<ParSet, ParSet> &g, const char * name, const char * prefix) {
	int num = 0;
	for (Graph<ParSet, ParSet>::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		Koala::IO::ParSet vInfo = g.getVertInfo(v);
		if (strlen(prefix) == 0)
		{
			vInfo.set(std::string(name), num);
		}
		else
		{
			std::string id(prefix);
			char str[10];
			sprintf(str, "%d", num);
			id.append(str);
			vInfo.set(std::string(name), id);
		}
		g.setVertInfo(v, vInfo);
		++num;
	}
}

void createIdsForEdges(Graph<ParSet, ParSet> &g, const char * name, const char * prefix) {
	int num = 0;
	for (Graph<ParSet, ParSet>::PEdge e = g.getEdge(); e; e = g.getEdgeNext(e))
	{
		Koala::IO::ParSet eInfo = g.getEdgeInfo(e);
		if (strlen(prefix) == 0)
		{
			eInfo.set(std::string(name), num);
		}
		else
		{
			std::string id(prefix);
			char str[10];
			sprintf(str, "%d", num);
			id.append(str);
			eInfo.set(std::string(name), id);
		}
		g.setEdgeInfo(e, eInfo);
		++num;
	}
}
