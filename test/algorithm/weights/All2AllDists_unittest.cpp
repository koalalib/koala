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

	MyGraph::PEdge	ab, ac, aj, ae, ag, cb, ce, cg, bc, ch, dc, de, dg, dh, eh,
		fe, fg, gh, hi, ib, ji, jf;
	// edge definition

	AssocArray<MyGraph::PEdge, DAGCritPath::EdgeLabs<int> > edgeMap;

	void buildWeightedGraphWith8VerticlesAnd10Edges() {

		graph.clear();
		a = graph.addVert('a');			// adding vertices
		b = graph.addVert('b');
		c = graph.addVert('c');
		d = graph.addVert('d');
		e = graph.addVert('e');
		f = graph.addVert('f');
		g = graph.addVert('g');
		h = graph.addVert('h');

		ac = graph.addEdge(a, c, "ac");		// adding edges
		ag = graph.addEdge(a, g, "ag");
		ae = graph.addEdge(a, e, "ae");
		bc = graph.addEdge(b, c, "bc");
		ce = graph.addEdge(c, e, "ce");
		cg = graph.addEdge(c, g, "cg");
		dh = graph.addEdge(d, h, "dh");
		eh = graph.addEdge(e, h, "eh");
		fg = graph.addEdge(f, g, "fg");
		gh = graph.addEdge(g, h, "gh");
	}

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

	int len;	// len is edge count in shortest path

	src = a; 	// start vertex
	dst = i;	// target vertex
	len = All2AllDists::getPath(graph, vertMatrix, src, dst, All2AllDists::outPath(tabV, tabE));

	//EXPECT_EQ(5, len);

	//EXPECT_EQ(/*5*/2147483647, vertMatrix(src, dst).distance);
	//EXPECT_EQ(aj, tabE[0]);
	//EXPECT_EQ(jf, tabE[1]);
	//EXPECT_EQ(fg, tabE[2]);
	//EXPECT_EQ(gh, tabE[3]);
	//EXPECT_EQ(hi, tabE[4]);
}


TEST_F(All2AllDistsTest, ClassBFSMethodscanNearTest) {

	buildWeightedGraphWith8VerticlesAnd10Edges();

	int n = graph.getVertNo();        // number of vertices in graph
	MyGraph::PVertex LOCALARRAY(vert_out, n);    // output sequence       
	AssocArray<MyGraph::PVertex, SearchStructs::VisitVertLabs<MyGraph> > vert_help(n);

	int radius = 2;
	EXPECT_EQ(7, BFS::scanNear(graph, a, radius, vert_help, EdUndir));
	MyGraph::PVertex v = vert_help.firstKey();
	EXPECT_EQ(a, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(c, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(g, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(e, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(b, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(f, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(h, v);
}

TEST_F(All2AllDistsTest, ClassDFSPostorderMethodscanNearTest) {

	buildWeightedGraphWith8VerticlesAnd10Edges();

	int n = graph.getVertNo();        // number of vertices in graph
	MyGraph::PVertex LOCALARRAY(vert_out, n);    // output sequence       
	AssocArray<MyGraph::PVertex, SearchStructs::VisitVertLabs<MyGraph> > vert_help(n);

	int radius = 2;
	EXPECT_EQ(/*7*/5, DFSPostorder::scanNear(graph, a, radius, vert_help, EdUndir));
	MyGraph::PVertex v = vert_help.firstKey();
	EXPECT_EQ(a, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(c, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(b, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(e, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(g, v);
	//v = vert_help.nextKey(v);
	//EXPECT_EQ(f, v);
	//v = vert_help.nextKey(v);
	//EXPECT_EQ(h, v);
}

TEST_F(All2AllDistsTest, ClassDFSPreorderMethodscanNearTest) {

	buildWeightedGraphWith8VerticlesAnd10Edges();

	int n = graph.getVertNo();        // number of vertices in graph
	MyGraph::PVertex LOCALARRAY(vert_out, n);    // output sequence       
	AssocArray<MyGraph::PVertex, SearchStructs::VisitVertLabs<MyGraph> > vert_help(n);

	int radius = 2;
	EXPECT_EQ(/*7*/5, DFSPreorder::scanNear(graph, a, radius, vert_help, EdUndir));
	MyGraph::PVertex v = vert_help.firstKey();
	EXPECT_EQ(a, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(c, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(b, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(e, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(g, v);
	//v = vert_help.nextKey(v);
	//EXPECT_EQ(f, v);
	//v = vert_help.nextKey(v);
	//EXPECT_EQ(h, v);
}

TEST_F(All2AllDistsTest, ClassBFSMethodscanAttainableTest) {

	buildWeightedGraphWith8VerticlesAnd10Edges();

	int n = graph.getVertNo();        // number of vertices in graph
	MyGraph::PVertex LOCALARRAY(vert_out, n);    // output sequence       
	AssocArray<MyGraph::PVertex, SearchStructs::VisitVertLabs<MyGraph> > vert_help(n);

	int radius = 2;
	EXPECT_EQ(8, BFS::scanAttainable(graph, a, vert_help, vert_out, EdUndir));
	MyGraph::PVertex v = vert_help.firstKey();
	EXPECT_EQ(a, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(c, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(g, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(e, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(b, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(f, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(h, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(d, v);
}

TEST_F(All2AllDistsTest, ClassDFSPreorderMethodscanAttainableTest) {

	buildWeightedGraphWith8VerticlesAnd10Edges();

	int n = graph.getVertNo();        // number of vertices in graph
	MyGraph::PVertex LOCALARRAY(vert_out, n);    // output sequence       
	AssocArray<MyGraph::PVertex, SearchStructs::VisitVertLabs<MyGraph> > vert_help(n);

	int radius = 2;
	EXPECT_EQ(8, DFSPreorder::scanAttainable(graph, a, vert_help, vert_out, EdUndir));
	MyGraph::PVertex v = vert_help.firstKey();
	EXPECT_EQ(a, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(c, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(b, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(e, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(h, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(d, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(g, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(f, v);
}

TEST_F(All2AllDistsTest, ClassDFSPostorderMethodscanAttainableTest) {

	buildWeightedGraphWith8VerticlesAnd10Edges();

	int n = graph.getVertNo();        // number of vertices in graph
	MyGraph::PVertex LOCALARRAY(vert_out, n);    // output sequence       
	AssocArray<MyGraph::PVertex, SearchStructs::VisitVertLabs<MyGraph> > vert_help(n);

	int radius = 2;
	EXPECT_EQ(8, DFSPostorder::scanAttainable(graph, a, vert_help, vert_out, EdUndir));
	MyGraph::PVertex v = vert_help.firstKey();
	EXPECT_EQ(a, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(c, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(b, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(e, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(h, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(d, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(g, v);
	v = vert_help.nextKey(v);
	EXPECT_EQ(f, v);
}

