#include<stdio.h>
#include<stdlib.h>
#include<utility>
#include<iostream>

#include"Koala/graph/graph.h"
#include"Koala/algorithm/search.h"
#include "Koala/classes/detect.h"
#include"Koala/io/text.h"


using namespace Koala;
using namespace Koala::IO;
using namespace std;


void Test(bool res, const char *id, const char *graph) {
	bool rv;
	Graph<int, int> g;
	Graph<int, int>::PVertex v,u1,u2,u3;
	readGraphText(g, graph, RG_VertexLists);

//	u1=g.addVert();u2=g.addVert();u3=g.addVert();
//	g.addEdge(u1,u3);g.addEdge(u3,u2);g.addEdge(u1,u2);

//	printf("Testing %s\n", id);
//	writeGraphText(g, std::cout, RG_VertexLists);
//	for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) printf("%p  ", v);
//	printf("\n");


	rv = IsIt::interval(g);
	if(rv != res) {
		printf("%s FAILED\n", id);
		exit(1);
	} else {
		printf("%s OK\n", id);
		};
	};

int main() {
/*
	Sets<TableDefaultCPPAllocator> m_sets;
	TableDefaultCPPAllocator all;
	m_sets.resize(6, all);
	for(int i = 0; i < 6; i++) {
		printf("i = %d\n", i);
		for(int j = i + 1; j < 6; j++) m_sets.add(i, j);
		};
	m_sets.dump();
	m_sets.remove(2);
	m_sets.dump();
	m_sets.remove(4);
	m_sets.dump();
	m_sets.remove(5);
	m_sets.dump();
	m_sets.remove(1);
	m_sets.dump();
	m_sets.remove(3);
	m_sets.dump();
	m_sets.remove(0);
	m_sets.dump();
	*/
	Test(true, "K1", "1\n0 0");
	Test(true, "P2", "2\n0 1 -1\n1 0");
	Test(true, "P3", "3\n0 1 -1\n1 1 -2\n2 0");
	Test(true, "P4", "4\n0 1 -1\n1 1 -2\n2 1 -3\n3 0");
	Test(true, "K3", "3\n0 2 -1 -2\n1 1 -2\n2 0");
	Test(false, "C4", "4\n0 2 -1 -3\n1 1 -2\n2 1 -3\n3 0");
	Test(false, "C5", "5\n0 2 -1 -4\n1 1 -2\n2 1 -3\n3 1 -4\n4 0");
	Test(true, "K4", "4\n0 3 -1 -2 -3\n1 2 -2 -3\n2 1 -3\n3 0");
	Test(true, "wiki'", "6\n"
			   "0 3 -1 -2 -3\n"
			   "1 1 -2\n"
			   "2 3 -3 -4 -5\n"
			   "3 2 -4 -5\n"
			   "4 0\n"
			   "5 0\n"
			   );
	Test(true, "wiki", "7\n"
			   "0 3 -1 -2 -3\n"
			   "1 1 -2\n"
			   "2 3 -3 -4 -5\n"
			   "3 2 -4 -5\n"
			   "4 0\n"
			   "5 1 -6\n"
			   "6 0");


//	cout << endl << endl;
//	Graph<int, int> g;
//	g.clear();
//	IsIt::Interval::Segment tabs[3]={{4,7},{1,14},{6,7}};
//	IsIt::Interval::segs2graph(g,tabs,tabs+3,blackHole);
//	cout << g.getVertNo() << ' ' << g.getEdgeNo();

	return 0;
	};
