#include <string>
// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest.h"

#include "koala/graph/graph.h"
#include "koala/algorithm/weights.h"

/* TEST(test_case_name, test_name) is a predefined macro.
These are ordinary C++ functions that don't return a value.
In this function, along with any valid C++ statements you want to include,
use the various Google Test assertions to check values.
The test's result is determined by the assertions;
if any assertion in the test fails (either fatally or non-fatally),
or if the test crashes, the entire test fails. Otherwise, it succeeds.
*/

using namespace std;
using namespace Koala;

class All2AllDistsTest : public ::testing::Test {

public:

	typedef Koala::Graph<char, string> MyGraph;		// graph type declaration

	// graph definition
	MyGraph graph;
	MyGraph::PVertex a, b, c, d, e, f, g, h, i, j;	// vertex definition

	MyGraph::PEdge	ab, aj, ae, cb, bc, ch, de, dg, dc,
		fe, fg, gh, hi, ib, ji, jf;
	// edge definition

	AssocArray<MyGraph::PEdge, DAGCritPath::EdgeLabs<int> > edgeMap;

	void buildWeightedGraphWith10VerticlesAnd15Edges() {
		// adding vertices
		a = graph.addVert('a');
		b = graph.addVert('b');
		c = graph.addVert('c');
		d = graph.addVert('d');
		e = graph.addVert('e');
		f = graph.addVert('f');
		g = graph.addVert('g');
		h = graph.addVert('h');
		i = graph.addVert('i');
		j = graph.addVert('j');

		// adding edges
		ab = graph.addArc(a, b, "a>b");
		aj = graph.addArc(a, j, "a>j");
		ae = graph.addArc(a, e, "a>e");
		cb = graph.addArc(c, b, "c>b");
		ch = graph.addArc(c, h, "c>h");
		de = graph.addArc(d, e, "d>e");
		dg = graph.addArc(d, g, "d>g");
		dc = graph.addArc(d, c, "d>c");
		fe = graph.addArc(f, e, "f>e");
		fg = graph.addArc(f, g, "f>g");
		gh = graph.addArc(g, h, "g>h");
		hi = graph.addArc(h, i, "h>i");
		ib = graph.addArc(i, b, "i>b");
		ji = graph.addArc(j, i, "j>i");
		jf = graph.addArc(j, f, "j>f");

		// adding weights
		edgeMap[ab].length = 1;
		edgeMap[aj].length = -3;
		edgeMap[ae].length = 3;
		edgeMap[cb].length = 2;
		edgeMap[ch].length = 4;
		edgeMap[de].length = -1;
		edgeMap[dg].length = 2;
		edgeMap[dc].length = 2;
		edgeMap[fe].length = 4;
		edgeMap[fg].length = 4;
		edgeMap[gh].length = -1;
		edgeMap[hi].length = -2;
		edgeMap[ib].length = 3;
		edgeMap[ji].length = 3;
		edgeMap[jf].length = 1;
	}
	
	void buildGraphWithNegativeCycle() {
		
		// adding vertices
		a = graph.addVert('a');
		b = graph.addVert('b');
		c = graph.addVert('c');
		d = graph.addVert('d');
		e = graph.addVert('e');
		f = graph.addVert('f');
		g = graph.addVert('g');
		h = graph.addVert('h');
		i = graph.addVert('i');
		j = graph.addVert('j');

		// adding edges
		ab = graph.addArc(a, b, "a>b");
		aj = graph.addArc(a, j, "a>j");
		ae = graph.addArc(a, e, "a>e");
		bc = graph.addArc(b, c, "b>c");
		ch = graph.addArc(c, h, "c>h");
		de = graph.addArc(d, e, "d>e");
		dg = graph.addArc(d, g, "d>g");
		dc = graph.addArc(d, c, "d>c");
		fe = graph.addArc(f, e, "f>e");
		fg = graph.addArc(f, g, "f>g");
		gh = graph.addArc(g, h, "g>h");
		hi = graph.addArc(h, i, "h>i");
		ib = graph.addArc(i, b, "i>b");
		ji = graph.addArc(j, i, "j>i");
		jf = graph.addArc(j, f, "j>f");

		edgeMap[ab].length = 2;
		edgeMap[aj].length = 3;
		edgeMap[ae].length = 2;
		edgeMap[bc].length = -3;
		edgeMap[ch].length = 1;
		edgeMap[de].length = -2;
		edgeMap[dg].length = 6;
		edgeMap[dc].length = -1;
		edgeMap[fe].length = 1;
		edgeMap[fg].length = 2;
		edgeMap[gh].length = 3;
		edgeMap[hi].length = -7;
		edgeMap[ib].length = 8;
		edgeMap[ji].length = -1;
		edgeMap[jf].length = -3;
	}

	// virtual void TearDown() {}
};

TEST_F(All2AllDistsTest, MethodFloydTest)
{
	buildWeightedGraphWith10VerticlesAnd15Edges();
	AssocMatrix<MyGraph::PVertex, All2AllDists::VertLabs<int, MyGraph >, AMatrFull> vertMatrix;
	EXPECT_TRUE(All2AllDists::floyd(graph, vertMatrix, edgeMap));
}



TEST_F(All2AllDistsTest, MethodgetPathLenTest)
{
	buildWeightedGraphWith10VerticlesAnd15Edges();
	AssocMatrix<MyGraph::PVertex, All2AllDists::VertLabs<int, MyGraph >, AMatrFull> vertMatrix;
	int n = graph.getVertNo();
	int m = graph.getEdgeNo();
	MyGraph::PVertex LOCALARRAY(tabV, n);
	MyGraph::PEdge LOCALARRAY(tabE, m);
	MyGraph::PVertex src, dst;
	src = a; 	// start vertex
	dst = i;	// target vertex
	EXPECT_EQ(-1, All2AllDists::getPath(graph, vertMatrix, src, dst, All2AllDists::outPath(tabV, tabE)));
}

TEST_F(All2AllDistsTest, MethodgetPathLenTestUnrechableVertex)
{
	buildWeightedGraphWith10VerticlesAnd15Edges();
	AssocMatrix<MyGraph::PVertex, All2AllDists::VertLabs<int, MyGraph >, AMatrFull> vertMatrix;
	int n = graph.getVertNo();
	int m = graph.getEdgeNo();
	MyGraph::PVertex LOCALARRAY(tabV, n);
	MyGraph::PEdge LOCALARRAY(tabE, m);
	MyGraph::PVertex src, dst;
	src = j;	// start vertex
	dst = a;	// target vertex
	EXPECT_EQ(-1, All2AllDists::getPath(graph, vertMatrix, src, dst, All2AllDists::outPath(tabV, tabE)));
}

TEST_F(All2AllDistsTest, MethodDistanceTest)
{
	buildWeightedGraphWith10VerticlesAnd15Edges();
	AssocMatrix<MyGraph::PVertex, All2AllDists::VertLabs<int, MyGraph >, AMatrFull> vertMatrix;
	int n = graph.getVertNo();
	int m = graph.getEdgeNo();
	MyGraph::PVertex LOCALARRAY(tabV, n);
	MyGraph::PVertex src, dst;
	MyGraph::PEdge LOCALARRAY(tabE, m);

	src = a; 	// start vertex
	dst = i;	// target vertex
	All2AllDists::getPath(graph, vertMatrix, src, dst, All2AllDists::outPath(tabV, tabE));

	cout << "The shortest path from '" << src->info << "' to '" << dst->info
		<< "' [total length = " << vertMatrix(src, dst).distance << "]: ";
	for (int k = 0; k < 5; ++k) {
		cout << tabE[k]->info << ' ';
	}

	const char *expectVal = "a>b";
	char *actualValTrue = new char[tabE[0]->info.size() + 1];
	//strncpy(actualValTrue, tabE[0]->info.c_str(), tabE[1]->info.size());

	EXPECT_STREQ(expectVal, actualValTrue);
}
