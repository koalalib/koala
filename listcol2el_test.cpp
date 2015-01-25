#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "Koala/graph/graph.h"
#include "Koala/coloring/list.h"
#include "Koala/base/rnd.h"
#include "Koala/classes/create.h"
//#include "test_utils.h"

using namespace std;
using namespace Koala;

typedef Koala::Graph<int, int> G;
typedef Koala::AssocArray<G::PVertex, Koala::Set<int> > GLists;
typedef Koala::AssocArray<G::PEdge, Koala::Set<int> > GeLists;

int vertListCol2elTests1();
int vertListCol2elTests2();
int vertListCol2elTests3();
int vertListCol2elTests4();

int edgeListCol2elTests1();
int edgeListCol2elTests2();
int edgeListCol2elTests3();
int edgeListCol2elTests4();
int edgeListCol2elTests5();
int main()
{

	vertListCol2elTests1();
	vertListCol2elTests2();
	vertListCol2elTests3();
	vertListCol2elTests4();

	edgeListCol2elTests1();
	edgeListCol2elTests2();
	edgeListCol2elTests3();
	edgeListCol2elTests4();
	edgeListCol2elTests5();

	return 0;
}

int vertListCol2elTests1() {

	cout << "Start vertListCol2elTests1" << endl;
	G g;
	Koala::Set<int> list;
	Koala::AssocArray<G::PVertex, int> colors;
	GLists colLists;
	int colCnt = 0;

	G::PVertex v1 = g.addVert(1);
	G::PVertex v2 = g.addVert(2);
	g.addEdge(v1, v2, 0);

	list.clear();
	list.add(0);
	colLists[v1] = list;

	list.clear();
	list.add(0); list.add(1);
	colLists[v2] = list;
	cout << "Positive result expected:" << endl;
	cout << "\nColors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	cout << "Positive result expected:" << endl;
	colCnt = ListVertColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
	cout << "Colored vertices: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	bool isOk = ListVertColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;
	assert(isOk == true);
	assert(colCnt > 0);


	colors.clear();
	colors[v2] = 0;

	cout << "\nColors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	cout << "Negative result expected:" << endl;
	colCnt = ListVertColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
	cout << "Colored vertices: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	assert(colCnt < 0);

	cout << "End of vertListCol2elTests1" << endl << endl;
	return 0;
}

int vertListCol2elTests2() {

	cout << "Start vertListCol2elTests2" << endl;

	G g;
	Koala::Set<int> list;
	Koala::AssocArray<G::PVertex, int> colors;
	GLists colLists;
	int colCnt = 0;

	G::PVertex v1 = g.addVert(1);
	G::PVertex v2 = g.addVert(2);
	G::PVertex v3 = g.addVert(3);
	G::PVertex v4 = g.addVert(4);
	G::PVertex v5 = g.addVert(5);
	g.addEdge(v1, v2, 0);
	g.addEdge(v1, v3, 0);
	g.addEdge(v2, v3, 0);
	g.addEdge(v1, v4, 0);
	g.addEdge(v3, v4, 0);
	g.addEdge(v3, v5, 0);
	g.addEdge(v4, v5, 0);

	list.clear();
	list.add(0); list.add(1);
	colLists[v1] = list;

	list.clear();
	list.add(2); list.add(3);
	colLists[v2] = list;

	list.clear();
	list.add(0); list.add(3);
	colLists[v3] = list;

	list.clear();
	list.add(2); list.add(3);
	colLists[v4] = list;

	list.clear();
	list.add(0); list.add(2);
	colLists[v5] = list;

	colCnt = ListVertColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
	cout << "Colored vertices: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	bool isOk = ListVertColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;
	cout << "End of vertListCol2elTests2" << endl << endl;
	assert(isOk == true);
	assert(colCnt > 0);
	return 0;
}

int vertListCol2elTests3() {

	cout << "Start vertListCol2elTests3" << endl;
	G g;
	Koala::Set<int> list;
	Koala::AssocArray<G::PVertex, int> colors;
	GLists colLists;
	int colCnt = 0;

	G::PVertex v1 = g.addVert(1);
	G::PVertex v2 = g.addVert(2);
	G::PVertex v3 = g.addVert(3);
	G::PVertex v4 = g.addVert(4);
	G::PVertex v5 = g.addVert(5);
	g.addEdge(v1, v2, 0);
	g.addEdge(v1, v3, 0);
	g.addEdge(v2, v3, 0);
	g.addEdge(v1, v4, 0);
	g.addEdge(v3, v4, 0);
	g.addEdge(v3, v5, 0);
	g.addEdge(v4, v5, 0);

	list.clear();
	list.add(0); list.add(1);
	colLists[v1] = list;

	list.clear();
	list.add(2); list.add(3);
	colLists[v2] = list;

	list.clear();
	list.add(0); list.add(3);
	colLists[v3] = list;

	list.clear();
	list.add(2); list.add(3);
	colLists[v4] = list;

	list.clear();
	list.add(0); list.add(2);
	colLists[v5] = list;

	colors[v3] = 3;
	colors[v5] = 2;

	std::vector<G::PVertex> vec;
	vec.clear();
	vec.push_back(v1);
	vec.push_back(v2);
	vec.push_back(v3);

	cout << "Colors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	cout << "Positive result expected:" << endl;
	colCnt = ListVertColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors, vec.begin(), vec.end());
	cout << "Colored vertices: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	bool isOk = ListVertColoringPar<AlgsDefaultSettings>::testPart(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;
	assert(isOk == true);
	assert(colCnt > 0);

	colors.clear();
	colors[v3] = 3;
	colors[v5] = 2;

	vec.clear();
	vec.push_back(v3);
	vec.push_back(v4);
	vec.push_back(v5);

	cout << "\nColors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}
	cout << "Negative result expected:" << endl;
	colCnt = ListVertColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors, vec.begin(), vec.end());
	cout << "Colored vertices: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		int col = -777;
		if (colors.hasKey(v))
			col = colors[v];
		cout << "Vert: " << g.getVertInfo(v) << " color: " << col << endl;

	}

	assert(colCnt < 0);

	cout << "End of vertListCol2elTests3" << endl << endl;
	return 0;
}


int vertListCol2elTests4() {
	cout << "Start vertListCol2elTests4" << endl;
	int n = 10;
	int maxVNum = 200;
	Koala::StdRandGen<> rgen;
	G g;
	Koala::Set<int> list, list1, list2;
	Koala::AssocArray<G::PVertex, int> colors;
	GLists colLists;
	list.add(1);
	list.add(2);

	list1.add(1);
	list2.add(2);

	bool allOk = true;
	for (int i = 0; i < n; ++i)
	{
		int n1 = Koala::Privates::getRandom(rgen, maxVNum - 1) + 1;
		int n2 = Koala::Privates::getRandom(rgen, maxVNum - 1) + 1;
		g.clear();
		Koala::Creator::compBipartite(g, n1, n2);

		int singleColorNum1 = Koala::Privates::getRandom(rgen, n1);
		int singleColorNum2 = Koala::Privates::getRandom(rgen, n2);

		int nn = 0;
		colors.clear();
		colLists.clear();
		for (G::PVertex v = g.getVert(); v; v = g.getVertNext(v))
		{
			if (nn < singleColorNum1) colLists[v] = list1;
			else
			{
				if (nn > n1 && nn < n1 + singleColorNum2) colLists[v] = list2;
				else colLists[v] = list;
			}
			nn++;
		}
		printf("T: %d, params: n1=%d, n1se=%d, n2=%d, n2se=%d, m=%d. ", i, n1, singleColorNum1, n2, singleColorNum2, g.getEdgeNo());
		int colCnt = ListVertColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
		printf("C-vert: %d. ", colCnt);
		bool isOk = ListVertColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
		if (isOk && colCnt > 0)
		{
			printf("Test OK.\n");
		}
		else
		{
			printf("Test NOT OK!");
			allOk = false;
//			koalaAssert(false, ExcTestFailed);
		}
	}

	if (allOk)
		printf("Test passed!\n");
	else
		printf("Test failed!!!\n");

	cout << "End of vertListCol2elTests4" << endl << endl;
}

int edgeListCol2elTests1() {

	cout << "Start edgeListCol2elTests1" << endl;

	G g;
	Koala::Set<int> list;
	Koala::AssocArray<G::PEdge, int> colors;
	GeLists colLists;
	int colCnt = 0;
	const EdgeDirection Mask = EdDirIn | EdDirOut | EdUndir;

	G::PVertex v1 = g.addVert(1);
	G::PVertex v2 = g.addVert(2);
	G::PVertex v3 = g.addVert(3);
	G::PVertex v4 = g.addVert(4);
	G::PEdge e12 =  g.addEdge(v1, v2, 12);
	G::PEdge e23 = g.addEdge(v2, v3, 23);
	G::PEdge e13 = g.addEdge(v1, v3, 13);

	list.clear();
	list.add(1);
	list.add(2);
	colLists[e12] = list;

	list.clear();
	list.add(1);
	list.add(2);
	colLists[e23] = list;

	list.clear();
	list.add(3);
	list.add(2);
	colLists[e13] = list;

	colCnt = ListEdgeColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
	cout << "Colored edges: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}
	bool isOk = ListEdgeColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;
	assert(isOk == true);
	assert(colCnt > 0);
	cout << "End of edgeListCol2elTests1" << endl << endl;
	return 0;
}

int edgeListCol2elTests2() {

	cout << "Start edgeListCol2elTests2" << endl;

	G g;
	Koala::Set<int> list;
	Koala::AssocArray<G::PEdge, int> colors;
	GeLists colLists;
	int colCnt = 0;
	const EdgeDirection Mask = EdDirIn | EdDirOut | EdUndir;

	G::PVertex v1 = g.addVert(1);
	G::PVertex v2 = g.addVert(2);
	G::PVertex v3 = g.addVert(3);
	G::PEdge e12 = g.addEdge(v1, v2, 12);
	G::PEdge e23_1 = g.addEdge(v2, v3, 231);
	G::PEdge e23_2 = g.addEdge(v2, v3, 232);
	G::PEdge e23_3 = g.addEdge(v2, v3, 233);

	list.clear();
	list.add(1);
	list.add(2);
	colLists[e12] = list;

	list.clear();
	list.add(3);
	list.add(4);
	colLists[e23_1] = list;

	list.clear();
	list.add(2);
	list.add(3);
	colLists[e23_2] = list;

	list.clear();
	list.add(1);
	colLists[e23_3] = list;

	colCnt = ListEdgeColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
	cout << "Colored edges: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}
	bool isOk = ListEdgeColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;
	assert(isOk == true);
	assert(colCnt > 0);
	cout << "End of edgeListCol2elTests2" << endl << endl;
	return 0;
}

int edgeListCol2elTests3() {

	cout << "Start edgeListCol2elTests3" << endl;

	G g;
	Koala::Set<int> list;
	Koala::AssocArray<G::PEdge, int> colors;
	GeLists colLists;
	int colCnt = 0;
	const EdgeDirection Mask = EdDirIn | EdDirOut | EdUndir;

	G::PVertex v1 = g.addVert(1);
	G::PVertex v2 = g.addVert(2);
	G::PVertex v3 = g.addVert(3);
	G::PEdge e12 = g.addEdge(v1, v2, 12);
	G::PEdge e23_1 = g.addEdge(v2, v3, 231);
	G::PEdge e23_2 = g.addEdge(v2, v3, 232);
	G::PEdge e23_3 = g.addEdge(v2, v3, 233);

	list.clear();
	list.add(1);
	list.add(2);
	colLists[e12] = list;

	list.clear();
	list.add(3);
	list.add(4);
	colLists[e23_1] = list;

	list.clear();
	list.add(2);
	list.add(3);
	colLists[e23_2] = list;

	list.clear();
	list.add(1);
	colLists[e23_3] = list;

	colors[e23_1] = 4;

	colCnt = ListEdgeColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
	cout << "Positive result expected:" << endl;
	cout << "Colored edges: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}
	bool isOk = ListEdgeColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;

	assert(isOk == true);
	assert(colCnt >= 0);

	colors.clear();
	colors[e23_1] = 3;

	colCnt = ListEdgeColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
	cout << "Negative result expected:" << endl;
	cout << "Colored edges: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}

	isOk = ListEdgeColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;
	assert(isOk == false);
	assert(colCnt < 0);
	cout << "End of edgeListCol2elTests3" << endl << endl;
	return 0;
}

int edgeListCol2elTests4() {

	cout << "Start edgeListCol2elTests4" << endl;

	G g;
	Koala::Set<int> list;
	Koala::AssocArray<G::PEdge, int> colors;
	GeLists colLists;
	int colCnt = 0;
	const EdgeDirection Mask = EdDirIn | EdDirOut | EdUndir;

	G::PVertex v1 = g.addVert(1);
	G::PVertex v2 = g.addVert(2);
	G::PVertex v3 = g.addVert(3);
	G::PEdge e12 = g.addEdge(v1, v2, 12);
	G::PEdge e23_1 = g.addEdge(v2, v3, 231);
	G::PEdge e23_2 = g.addEdge(v2, v3, 232);
	G::PEdge e23_3 = g.addEdge(v2, v3, 233);

	list.clear();
	list.add(1);
	list.add(2);
	colLists[e12] = list;

	list.clear();
	list.add(3);
	list.add(4);
	colLists[e23_1] = list;

	list.clear();
	list.add(2);
	list.add(3);
	colLists[e23_2] = list;

	list.clear();
	list.add(1);
	colLists[e23_3] = list;

	colors[e23_1] = 3;

	std::vector<G::PEdge> vec;

	vec.push_back(e23_1);
	vec.push_back(e23_2);
	vec.push_back(e23_3);

	cout << "Colors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = -777;
		if (colors.hasKey(e))
			col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}

	colCnt = ListEdgeColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors, vec.begin(), vec.end());
	cout << "Positive result expected:" << endl;
	cout << "Colored edges: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = -777;
		if (colors.hasKey(e))
			col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}
	bool isOk = ListEdgeColoringPar<AlgsDefaultSettings>::testPart(g, colLists, colors);
	cout << "Coloring test: " << isOk << endl;

	assert(isOk == true);
	assert(colCnt >= 0);

	colors.clear();
	colors[e12] = 1;

	vec.clear();
	vec.push_back(e23_3);

	cout << "\nColors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = -777;
		if (colors.hasKey(e))
			col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}

	colCnt = ListEdgeColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors, vec.begin(), vec.end());
	cout << "Negative result expected:" << endl;
	cout << "Colored edges: " << colCnt << endl;
	cout << "Colors map size: " << colors.size() << endl;
	for (G::PEdge e = g.getEdge(Mask); e; e = g.getEdgeNext(e, Mask))
	{
		int col = -777;
		if (colors.hasKey(e))
			col = colors[e];
		cout << "Edge: " << g.getEdgeInfo(e) << " color: " << col << endl;

	}
	assert(colCnt < 0);

	cout << "End of edgeListCol2elTests4" << endl << endl;
	return 0;
}


int edgeListCol2elTests5()
{
	cout << "Start edgeListCol2elTests5" << endl;
	int n = 10;
	int maxVNum = 1000;
	Koala::StdRandGen<> rgen;
	G g;
	Koala::Set<int> list, list1, list2;
	Koala::AssocArray<G::PEdge, int> colors;
	GeLists colLists;
	list.add(1);
	list.add(2);

	list1.add(1);
	list2.add(2);

	bool allOk = true;
	for (int i = 0; i < n; ++i)
	{
		int k = Koala::Privates::getRandom(rgen, maxVNum - 2) + 2;
		int kk = 2 * k;
		g.clear();
		Koala::Creator::cycle(g, kk);

		int singleColorNum1 = Koala::Privates::getRandom(rgen, k);
		int singleColorNum2 = Koala::Privates::getRandom(rgen, k);

		colors.clear();
		colLists.clear();
		int nn = 0;
		int sn1 = 0;
		int sn2 = 0;
		for (G::PEdge e = g.getEdge(); e; e = g.getEdgeNext(e))
		{
			if (nn % 2 == 0)
			{
				if (sn1 < singleColorNum1)
				{
					colLists[e] = list1;
					sn1++;
				}
				else
				{
					colLists[e] = list;
				}
			}
			else
			{
				if (sn2 < singleColorNum2)
				{
					colLists[e] = list2;
					sn2++;
				}
				else
				{
					colLists[e] = list;
				}
			}
			nn++;
		}
		printf("T: %d, params: n=%d, m1se=%d, m2se=%d, m=%d. ", i, g.getVertNo(), singleColorNum1, singleColorNum2, g.getEdgeNo());
		int colCnt = ListEdgeColoringPar<AlgsDefaultSettings>::color2ElemLists(g, colLists, colors);
		printf("C-edge: %d. ", colCnt);
		bool isOk = ListEdgeColoringPar<AlgsDefaultSettings>::test(g, colLists, colors);
		if (isOk && colCnt > 0)
		{
			printf("Test OK.\n");
		}
		else
		{
			printf("Test NOT OK!");
			allOk = false;
//			koalaAssert(false, ExcTestFailed);
		}
	}

	if (allOk)
		printf("Test passed!\n");
	else
		printf("Test failed!!!\n");

	cout << "End of edgeListCol2elTests5" << endl << endl;
}
