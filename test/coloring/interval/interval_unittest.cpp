// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest.h"

//#include "koala/coloring/interval.h"

/* TEST(test_case_name, test_name) is a predefined macro.
These are ordinary C++ functions that don't return a value.
In this function, along with any valid C++ statements you want to include,
use the various Google Test assertions to check values.
The test's result is determined by the assertions;
if any assertion in the test fails (either fatally or non-fatally),
or if the test crashes, the entire test fails. Otherwise, it succeeds.
*/

//using namespace Koala;
//using namespace std;

/*
typedef Graph<int, int> MyGraph;
typedef MyGraph::PVertex GVert;
typedef MyGraph::PEdge GEdge;
*/

TEST(EquitableTest, S1MethodTest)
{
	/*
	MyGraph graph;
	int res;
	AssocArray<GVert, IntervalVertColoring::Color> colors;
	AssocArray<GVert, int> weights;

	//we take the graph see Graph
	GVert verts[5];
	verts[0] = graph.addVert(1); weights[verts[0]] = 2;
	verts[1] = graph.addVert(2); weights[verts[1]] = 3;
	verts[2] = graph.addVert(3); weights[verts[2]] = 4;
	verts[3] = graph.addVert(4); weights[verts[3]] = 1;
	verts[4] = graph.addVert(5); weights[verts[4]] = 2;
	graph.addEdge(verts[0], verts[1]);
	graph.addEdge(verts[0], verts[2]);
	graph.addEdge(verts[0], verts[3]);
	graph.addEdge(verts[0], verts[4]);
	graph.addEdge(verts[1], verts[2]);
	graph.addEdge(verts[2], verts[3]);
	graph.addEdge(verts[3], verts[4]);

	//color it greedily
	res = IntervalVertColoring::greedy(graph, weights, colors);
	EXPECT_EQ(8, res);

	GVert vv = graph.getVert();
	EXPECT_EQ(1, vv->info);
	EXPECT_EQ(0, colors[vv].min);
	EXPECT_EQ(1, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(2, vv->info);
	EXPECT_EQ(2, colors[vv].min);
	EXPECT_EQ(4, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(3, vv->info);
	EXPECT_EQ(5, colors[vv].min);
	EXPECT_EQ(8, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(4, vv->info);
	EXPECT_EQ(2, colors[vv].min);
	EXPECT_EQ(2, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(5, vv->info);
	EXPECT_EQ(3, colors[vv].min);
	EXPECT_EQ(4, colors[vv].max);

	colors.clear();
	//or by using LI heuristic
	res = IntervalVertColoring::li(graph, weights, colors);
	EXPECT_EQ(8, res);

	vv = graph.getVert();
	EXPECT_EQ(1, vv->info);
	EXPECT_EQ(7, colors[vv].min);
	EXPECT_EQ(8, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(2, vv->info);
	EXPECT_EQ(4, colors[vv].min);
	EXPECT_EQ(6, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(3, vv->info);
	EXPECT_EQ(0, colors[vv].min);
	EXPECT_EQ(3, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(4, vv->info);
	EXPECT_EQ(4, colors[vv].min);
	EXPECT_EQ(4, colors[vv].max);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(5, vv->info);
	EXPECT_EQ(0, colors[vv].min);
	EXPECT_EQ(1, colors[vv].max);
	*/
}