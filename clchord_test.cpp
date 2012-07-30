#include<stdio.h>
#include<utility>
#include<iostream>

#include <iostream>
#include <iomanip>
#include "Koala/graph/graph.h"
#include "Koala/io/text.h"
#include "Koala/classes/detect.h"


using namespace Koala;
using namespace Koala::IO;
using namespace std;

#define TEST_2
// TEST_ 1-7

int main() {
	bool rv;
	Graph<int, int> g;
	Graph<int, int>::PVertex v;



	readGraphText(g,
#if defined(TEST_1)
		"8\n"
		"0 2 -1 -3\n"
		"1 4 -3 -6 -4 -2\n"
		"2 1 -4\n"
		"3 2 -5 -6\n"
		"4 2 -6 -7\n"
		"5 1 -6\n"
		"6 1 -7\n"
		"7 0\n",
#endif
#if defined(TEST_2)

		"5\n"
		"0 4 -1 -3 -4 -2\n"
		"1 3 -4 -3 -2\n"
		"2 2 -3 -4\n"
		"3 1 -4\n"
		"4 0\n",
#endif
#if defined(TEST_3)
		"4\n"
		"0 3 -1 -2 -3\n"
		"1 2 -2 -3\n"
		"2 1 -3 \n"
		"3 0\n",
#endif
#if defined(TEST_4)
		"4\n"
		"0 3 -1 -2 -3\n"
		"1 0\n"
		"2 0\n"
		"3 0\n",
#endif
#if defined(TEST_5)
		"4\n"
		"0 3 -1 -2 -3\n"
		"1 2 -2 -3\n"
		"2 0n"
		"3 0\n",
#endif
#if defined(TEST_6)
		"5\n"
		"0 2 -1 -2\n"
		"1 1 -2\n"
		"2 0\n"
		"3 1 -4\n"
		"4 0\n",
#endif
#if defined(TEST_7)
		"5\n"
		"0 2 -3 -4\n"
		"1 2 -3 -4\n"
		"2 2 -3 -4\n"
		"3 0\n"
		"4 0\n",
#endif
		RG_VertexLists);

	for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) {
		printf("%p\n", v);
		};

	rv = IsIt::chordal(g);
	printf("RESULT: ");
	if(rv) printf("true"); else printf("false");
	printf("\n\n-----------------------\n");

//	rv = Chordal::IsCoChordal(g);
//	printf("RESULT: ");
//	if(rv) printf("true"); else printf("false");
//	printf("\n\n-----------------------\n");

	Graph<int, int>::PVertex cq[128], cq2[128];
	int ncq;
	if (rv)
	{ printf("Orders: \n"); IsIt::Chordal::getOrder(g, cq2);
//        for(int i = 0; i < g.getVertNo(); i++) printf("%p\n", cq[i]);
        printf("\n");
        for(int i = 0; i < g.getVertNo(); i++) printf("%p\n", cq2[i]);
	}


	return 0;
}
