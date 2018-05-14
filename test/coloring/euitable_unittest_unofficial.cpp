// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest-spi.h"

#include "koala/coloring/equitable.h"

/* TEST(test_case_name, test_name) is a predefined macro.
These are ordinary C++ functions that don't return a value.
In this function, along with any valid C++ statements you want to include,
use the various Google Test assertions to check values.
The test's result is determined by the assertions;
if any assertion in the test fails (either fatally or non-fatally),
or if the test crashes, the entire test fails. Otherwise, it succeeds.
*/

using namespace Koala;
using namespace std;

typedef Graph<int, int> MyGraph;
typedef MyGraph::PVertex GVert;
typedef MyGraph::PEdge GEdge;

TEST(EquitableTest, S1MethodTest)
{
	MyGraph graph;
	AssocArray<GVert, int> colors;

	//we take the graph see Graph
	GVert verts[6];
	verts[0] = graph.addVert(1); verts[1] = graph.addVert(2);
	verts[2] = graph.addVert(3); verts[3] = graph.addVert(4);
	verts[4] = graph.addVert(5); verts[5] = graph.addVert(6);
	graph.addEdge(verts[0], verts[1]);
	graph.addEdge(verts[1], verts[2]);
	graph.addEdge(verts[2], verts[3]);
	graph.addEdge(verts[3], verts[4]);
	graph.addEdge(verts[4], verts[5]);
	graph.addEdge(verts[5], verts[0]);
	graph.addEdge(verts[0], verts[2]);

	//first of all we have to color edges of the graph
	SeqVertColoring::sl(graph, colors);

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

	int color[6];

	vv = graph.getVert();
	color[0] = colors[vv];
	vv = graph.getVertNext(vv);
	color[1] = colors[vv];
	vv = graph.getVertNext(vv);
	color[2] = colors[vv];
	vv = graph.getVertNext(vv);
	color[3] = colors[vv];
	vv = graph.getVertNext(vv);
	color[4] = colors[vv];
	vv = graph.getVertNext(vv);
	color[5] = colors[vv];

	EXPECT_NE(color[0], color[1]);
	EXPECT_NE(color[0], color[2]);
	EXPECT_NE(color[0], color[5]);
	EXPECT_NE(color[1], color[2]);
	EXPECT_NE(color[2], color[3]);
	EXPECT_NE(color[3], color[4]);
	EXPECT_NE(color[4], color[5]);

}

TEST(EquitableTest, Repair1MethodTest)
{
	MyGraph graph;
	AssocArray<GVert, int> colors;

	//we take the graph see Graph
	GVert verts[6];
	verts[0] = graph.addVert(1); verts[1] = graph.addVert(2);
	verts[2] = graph.addVert(3); verts[3] = graph.addVert(4);
	verts[4] = graph.addVert(5); verts[5] = graph.addVert(6);
	graph.addEdge(verts[0], verts[1]);
	graph.addEdge(verts[1], verts[2]);
	graph.addEdge(verts[2], verts[3]);
	graph.addEdge(verts[3], verts[4]);
	graph.addEdge(verts[4], verts[5]);
	graph.addEdge(verts[5], verts[0]);
	graph.addEdge(verts[0], verts[2]);

	//first of all we have to color edges of the graph
	SeqVertColoring::sl(graph, colors);

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

	//then we can fix coloring
	cout << "Colors balance before fix " << EquitVertColoring::test(graph, colors) << "\n";
	EquitVertColoring::repair1(graph, colors);

	int color[6];
	int colorCount[3];

	vv = graph.getVert();
	colorCount[colors[vv]]++;
	color[0] = colors[vv];
	vv = graph.getVertNext(vv);
	colorCount[colors[vv]]++;
	color[1] = colors[vv];
	vv = graph.getVertNext(vv);
	colorCount[colors[vv]]++;
	color[2] = colors[vv];
	vv = graph.getVertNext(vv);
	colorCount[colors[vv]]++;
	color[3] = colors[vv];
	vv = graph.getVertNext(vv);
	colorCount[colors[vv]]++;
	color[4] = colors[vv];
	vv = graph.getVertNext(vv);
	colorCount[colors[vv]]++;
	color[5] = colors[vv];

	EXPECT_NE(color[0], color[1]);
	EXPECT_NE(color[0], color[2]);
	EXPECT_NE(color[0], color[5]);
	EXPECT_NE(color[1], color[2]);
	EXPECT_NE(color[2], color[3]);
	EXPECT_NE(color[3], color[4]);
	EXPECT_NE(color[4], color[5]);

	int colorCountDifference = 1;

	EXPECT_GT(colorCount[0] + colorCountDifference, colorCount[1]);
	EXPECT_LT(colorCount[0], colorCount[1] + colorCountDifference);
	EXPECT_GT(colorCount[1] + colorCountDifference, colorCount[2]);
	EXPECT_LT(colorCount[1], colorCount[2] + colorCountDifference);
	EXPECT_GT(colorCount[2] + colorCountDifference, colorCount[0]);
	EXPECT_LT(colorCount[2], colorCount[0] + colorCountDifference);
}