// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest-spi.h"

#include "koala/graph/graph.h"
#include "koala/coloring/vertex.h"

/* TEST(test_case_name, test_name) is a predefined macro.
These are ordinary C++ functions that don't return a value.
In this function, along with any valid C++ statements you want to include,
use the various Google Test assertions to check values.
The test's result is determined by the assertions;
if any assertion in the test fails (either fatally or non-fatally),
or if the test crashes, the entire test fails. Otherwise, it succeeds.
*/

using namespace Koala;

TEST(BrooksTest, BrooksColoringTest)
{
	typedef Graph<char, int> MyGraph;

	const int N = 7;

	MyGraph g;
	MyGraph::PVertex V[N];

	AssocArray<MyGraph::PVertex, int> vertCont;

	V[0] = g.addVert('A'), V[1] = g.addVert('B'), V[2] = g.addVert('C'), V[3] = g.addVert('D');
	V[4] = g.addVert('E'), V[5] = g.addVert('F'), V[6] = g.addVert('G');

	g.addEdge(V[0], V[2]), g.addEdge(V[0], V[3]), g.addEdge(V[0], V[5]), g.addEdge(V[0], V[6]),
		g.addEdge(V[1], V[2]), g.addEdge(V[1], V[4]), g.addEdge(V[1], V[5]), g.addEdge(V[1], V[6]),
		g.addEdge(V[2], V[3]), g.addEdge(V[2], V[4]), g.addEdge(V[3], V[4]);

	// finding Brooks coloring
	int colors = SeqVertColoring::brooks(g, vertCont);

	EXPECT_EQ(7, g.getVertNo());
	EXPECT_EQ('A', V[0]->info);
	EXPECT_EQ('B', V[1]->info);
	EXPECT_EQ('C', V[2]->info);
	EXPECT_EQ('D', V[3]->info);
	EXPECT_EQ('E', V[4]->info);
	EXPECT_EQ('F', V[5]->info);
	EXPECT_EQ('G', V[6]->info);
	EXPECT_EQ(1, vertCont[V[0]]);
	EXPECT_EQ(2, vertCont[V[1]]);
	EXPECT_EQ(0, vertCont[V[2]]);
	EXPECT_EQ(2, vertCont[V[3]]);
	EXPECT_EQ(1, vertCont[V[4]]);
	EXPECT_EQ(0, vertCont[V[5]]);
	EXPECT_EQ(0, vertCont[V[6]]);
}

TEST(BrooksTest, BrooksColoringTest2)
{
	typedef Graph<int, int> MyGraph;
	typedef MyGraph::PVertex GVert;

	MyGraph graph;
	int res;
	AssocArray<GVert, int> colors;

	//we take the graph
	GVert verts[7];
	verts[0] = graph.addVert(1); verts[1] = graph.addVert(2); verts[2] = graph.addVert(3);
	verts[3] = graph.addVert(4); verts[4] = graph.addVert(5); verts[5] = graph.addVert(6);
	verts[6] = graph.addVert(7);
	graph.addEdge(verts[0], verts[2]); graph.addEdge(verts[0], verts[3]);
	graph.addEdge(verts[0], verts[5]); graph.addEdge(verts[0], verts[6]);
	graph.addEdge(verts[1], verts[2]); graph.addEdge(verts[1], verts[4]);
	graph.addEdge(verts[1], verts[5]); graph.addEdge(verts[1], verts[6]);
	graph.addEdge(verts[2], verts[3]); graph.addEdge(verts[2], verts[4]);
	graph.addEdge(verts[3], verts[4]);

	//color using the Brooks algorithm
	res = SeqVertColoring::brooks(graph, colors);

	GVert vv = graph.getVert();
	EXPECT_EQ(1, vv->info);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(2, vv->info);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(3, vv->info);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(4, vv->info);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(5, vv->info);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(6, vv->info);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(7, vv->info);

	vv = graph.getVert();
	EXPECT_EQ(1, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(2, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(0, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(2, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(1, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(0, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(0, colors[vv]);
}