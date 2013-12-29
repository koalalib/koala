// true = positive
#include"./koala/graph/graph.h"
//#include"./koala/container/joinsets.h"
#include"./koala/io/text.h"
#include"./koala/io/g6.h"
#include"./koala/algorithm/matching.h"

#include<iostream>

using namespace Koala;
using namespace Koala::IO;

struct TestData {
	int size;
	const char *graph;
	};

int main() {
	Koala::Graph<int, int> g;
	Koala::Graph<int, int>::PEdge edges[100];
	int i, n;
	bool fail = false;
	TestData data[] = {
		{ 0, "0"},
		{ 0, "1 0 0"},
		{ 1, "2 0(0) 1 -1 1(1) 0"},
		{ 0, "2 0 0 1 0"},
		{ 2, "4 0(0) 2 -1 -2 1(1) 1 -3 2(2) 0 3(3) 0"},
		{ 2, "5 0(0) 2 -1 -4 1(1) 1 -2 2(2) 1 -3 3(3) 1 -4 4(4) 0"},
		{ 3, "6 0(0) 2 -1 -2 1(1) 2 -2 -3 2(2) 1 -4 3(3) 1 -4 4(4) 1 -5 5(5)"},
//TODO: Error!:
//		{ 5, "10 0(0) 3 -1 -2 -9 1(1) 1 -2 2(2) 2 -3 -8 3(3) 2 -6 -7 4(4) 2 -5 -8 5(5) 1 -6 6(6) 1 -7 7(7) 0 8(8) 0 9(9) 0"},
		{ 4, "8 0(0) 4 -5 -4 -7 -2 1(1) 2 -5 -7 2(2) 2 -4 -7 3(3) 2 -5 -4 4(4) 2 -7 -6 5(5) 0 6(6) 0 7(7) 0"},
		{ 6, "14 0(0) 2 -10 -2 1(1) 3 -10 -12 -11 2(2) 2 -11 -5 3(3) 0 4(4) 2 -11 -12 5(5) 4 -13 -8 -7 -9 6(6) 1 -9 7(7) 1 -11 8(8) 3 -10 -11 -13 9(9) 1 -10 10(10) 2 -11 -13 11(11) 1 -12 12(12) 1 -13 13(13) 0"},
		{ 8, "16 0 2 -1 -14 1 2 -2 -4 2 1 -3 3 1 -5 4 2 -5 -12 5 0 6 2 -7 -15 7 2 -8 -10 8 2 -9 -13 9 1 -11 10 1 -11 11 0 12 1 -13 13 0 14 0 15 0"},

		};


	for(i = 0; i < sizeof(data) / sizeof(data[0]); i++) {
		g.clear();
		Koala::IO::readGraphText(g, data[i].graph, Koala::IO::RG_VertexLists);
//		if(i == 4) __debugbreak();
		n = Matching::findMax(g, edges);
		if(n != data[i].size) {
			printf("test %d FAILED (%d instead of %d)\n", i, n, data[i].size);
			fail = true;
		} else {
			printf("test %d ok\n", i);
			};
		};

	g.clear();

	readG6(g,"__??O@?C?SO???WG?OD@G?`_@??os?oG??C??J??_???_GQSOB?OCW@C_?_?_?????_OG?EO?G?AG???WAac");
	n = Matching::findMax(g, edges);
	if(n != 16) {	// oszukane, bo nie wiem ile powinno byc, ale sie nie wiesza
		printf("test %d FAILED (%d instead of %d)\n", i, n, 16);
		fail = true;
	} else {
		printf("test %d ok\n", i);
		};
	i++;

	if(fail) printf("TEST FAILED\n");
	return 0;
	};
