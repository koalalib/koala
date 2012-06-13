#include<stdio.h>
#include<utility>
#include<iostream>

#include"./Koala/graph/graph.h"
#include"./Koala/algorithm/search.h"
#include"./Koala/io/text.h"
#include"./Koala/classes/detect.h"


using namespace Koala;
using namespace Koala::IO;

struct ChTest {
	bool ans, coans;
	int clq;
	char *graph;
	};

ChTest tests[] = {
	{true, true, 1, "1 0 0"},			// 0
	{true, true, 1, "2 0 0 1 0"},			// 1
	{true, true, 2, "2 0 1 -1 1 0"},		// 2
	{true, true, 1, "3 0 0 1 0 2 0"},		// 3
	{true, true, 2, "3 0 1 -1 1 0 2 0"},		// 4
	{true, true, 2, "3 0 1 -1 1 1 -2 2 0"},		// 5
	{true, true, 3, "3 0 2 -1 -2 1 1 -2 2 0"},	// 6

	{true, true, 1, "4 0 0 1 0 2 0 3 0"},		// 7
	{true, true, 2, "4 0 1 -1 1 0 2 0 3 0"},	// 8
	{true, true, 2, "4 0 1 -1 1 1 -2 2 0 3 0"},	// 9
	{true, true, 3, "4 0 2 -1 -2 1 1 -2 2 0 3 0"},	// 10
	{true, true, 2, "4 0 3 -1 -2 -3 1 0 2 0 3 0"},	// 11
	{true, true, 3, "4 0 3 -1 -2 -3 1 1 -3 2 1 -3 3 0"},	// 12
	{true, true, 4, "4 0 3 -1 -2 -3 1 2 -2 -3 2 1 -3 3 0"},	// 13

	{false, true, -1, "4 0 2 -1 -3 1 1 -2 2 1 -3 3 0"},	// 14	// c4
	{true, false, 2, "4 0 1 -1 1 0 2 1 -3 3 0"},		// 15	// inv c4

	{true, true, 2, "4 0 1 -1 1 1 -2 2 1 -3 3 0"},		// 16	// p4
	{true, true, 3, "4 0 3 -1 -2 -3 1 1 -2 2 0 3 0"},	// 17
	
	{true, true, 3,						// 18
		"6\n"
		"0 2 -3 -5\n"
		"1 0\n"
		"2 1 -3\n"
		"3 2 -4 -5\n"
		"4 1 -5\n"
		"5 0\n"},
	
	{true, true, 4, "5 0 3 -1 -2 -3 1 2 -2 -3 2 1 -3 3 1 -4 4 0"},	// 19 K4+leaf
	{true, true, 2, "5 0 1 -4 1 1 -4 2 1 -4 3 0 4 0"},	// 20 inv K4+leaf

	{true, true, 3, "5\n0 4 -1 -2 -3 -4\n1 1 -2\n2 1 -3\n3 1 -4\n4 0\n"},	// 21
	{true, true, 3, "5 0 4 -1 -2 -3 -4 1 3 -2 -3 -4 2 0 3 0 4 0"},	// 22
	};


int main() {
	int i, ncq;
	bool rv;
	Graph<int, int> g, g2;
	Graph<int, int>::PVertex v, cq[1024];

	for(i = 0; i < sizeof(tests) / sizeof(ChTest); i++) {
		printf("test %d\n", i);
		g.clear();
		readGraphText(g, tests[i].graph, RG_VertexLists);
//		writeGraphText(g, std::cout, RG_VertexLists);
//		for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) printf("%p\n", v);
		rv = IsIt::chordal(g);
		if(rv != tests[i].ans) {
			printf("bad IsChordal answer (%s instead of %s)\n",
			       rv ? "true" : "false", tests[i].ans ? "true" : "false");
			};
		rv = IsIt::cochordal(g);
		if(rv != tests[i].coans) {
			printf("bad IsCoChordal answer (%s instead of %s)\n",
			       rv ? "true" : "false", tests[i].ans ? "true" : "false");
			};
//		ncq = IsIt::Chordal::maxClique(g, cq);
//		if(ncq != tests[i].clq) {
//			printf("bad MaxClique answer (%d instead of %d)\n",
//			       ncq, tests[i].clq);
//			};
		};

	return 0;
	};
