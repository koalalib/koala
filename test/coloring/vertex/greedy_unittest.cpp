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

using namespace std;
using namespace Koala;


class GreedyTest1 : public ::testing::Test {

public:

	typedef Graph<char, int> MyGraph;

	MyGraph g;
	MyGraph::PVertex V[7];

	AssocArray<MyGraph::PVertex, int> vertCont;

	virtual void SetUp() {

		V[0] = g.addVert('A'), V[1] = g.addVert('B'), V[2] = g.addVert('C'), V[3] = g.addVert('D');
		V[4] = g.addVert('E'), V[5] = g.addVert('F'), V[6] = g.addVert('G');

		g.addEdge(V[0], V[2]), g.addEdge(V[0], V[3]), g.addEdge(V[0], V[5]), g.addEdge(V[0], V[6]),
			g.addEdge(V[1], V[2]), g.addEdge(V[1], V[4]), g.addEdge(V[1], V[5]), g.addEdge(V[1], V[6]),
			g.addEdge(V[2], V[3]), g.addEdge(V[2], V[4]), g.addEdge(V[3], V[4]);
	}

	// virtual void TearDown() {}

};

class GreedyTest2 : public ::testing::Test {

public:

	typedef Graph<int, int> MyGraph;
	typedef MyGraph::PVertex GVert;

	MyGraph graph;
	int res;
	AssocArray<GVert, int> colors;
	GVert verts[5];

	virtual void SetUp() {

		//we take the graph see Graph		
		verts[1] = graph.addVert(2); verts[0] = graph.addVert(1); verts[2] = graph.addVert(3);
		verts[4] = graph.addVert(5); verts[3] = graph.addVert(4);
		graph.addEdge(verts[0], verts[1]); graph.addEdge(verts[0], verts[2]);
		graph.addEdge(verts[0], verts[3]); graph.addEdge(verts[0], verts[4]);
		graph.addEdge(verts[1], verts[2]); graph.addEdge(verts[2], verts[3]);
		graph.addEdge(verts[3], verts[4]);
	}

	// virtual void TearDown() {}

};

TEST_F(GreedyTest1, GreedySeqVertColoringTest1)
{

	int colors = SeqVertColoring::greedy(g, vertCont);

	EXPECT_EQ(3, colors);

	EXPECT_EQ(7, g.getVertNo());
	EXPECT_EQ('A', V[0]->info);
	EXPECT_EQ('B', V[1]->info);
	EXPECT_EQ('C', V[2]->info);
	EXPECT_EQ('D', V[3]->info);
	EXPECT_EQ('E', V[4]->info);
	EXPECT_EQ('F', V[5]->info);
	EXPECT_EQ('G', V[6]->info);
	EXPECT_EQ(0, vertCont[V[0]]);
	EXPECT_EQ(0, vertCont[V[1]]);
	EXPECT_EQ(1, vertCont[V[2]]);
	EXPECT_EQ(2, vertCont[V[3]]);
	EXPECT_EQ(3, vertCont[V[4]]);
	EXPECT_EQ(1, vertCont[V[5]]);
	EXPECT_EQ(1, vertCont[V[6]]);

	vertCont.clear(), cout << endl;
	// finding greedy coloring for vertices B to E
	colors = SeqVertColoring::greedy(g, vertCont, V + 1, V + 5);

	EXPECT_EQ(2, colors);

	EXPECT_EQ('B', V[1]->info);
	EXPECT_EQ('C', V[2]->info);
	EXPECT_EQ('D', V[3]->info);
	EXPECT_EQ('E', V[4]->info);
	EXPECT_EQ(0, vertCont[V[1]]);
	EXPECT_EQ(1, vertCont[V[2]]);
	EXPECT_EQ(0, vertCont[V[3]]);
	EXPECT_EQ(2, vertCont[V[4]]);
}

TEST_F(GreedyTest2, GreedySeqVertColoringTest2)
{

	//color it
	res = SeqVertColoring::greedy(graph, colors);
	//and show the coloring
	EXPECT_EQ(3, res);

	GVert vv = graph.getVert();
	EXPECT_EQ(2, vv->info);
	EXPECT_EQ(0, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(1, vv->info);
	EXPECT_EQ(1, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(3, vv->info);
	EXPECT_EQ(2, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(5, vv->info);
	EXPECT_EQ(0, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(4, vv->info);
	EXPECT_EQ(3, colors[vv]);


	//we can also erase color of any vertex
	colors[graph.getVert()] = -1; //or colors.delKey(graph.getVert())

	//coloring tests
	EXPECT_EQ(0, SeqVertColoring::test(graph, colors));
	//false because whole graph is not colored
	EXPECT_EQ(1, SeqVertColoring::testPart(graph, colors));
	//true because graph induced by colored vertices is properly colored

	//and color whole graph again
	res = SeqVertColoring::greedy(graph, colors);
	EXPECT_EQ(0, res);
	//in fact greedy(..) leave old coloring and colors only vertices that are not colored

	//if we erase color again
	colors[graph.getVert()] = -1;
	//we may color only the uncolored vertex
	SeqVertColoring::greedy(graph, colors, graph.getVert());

	colors.clear();
	//at last we can color graph by any vertex sequence, for example:
	res = SeqVertColoring::greedy(graph, colors, verts, verts + 5);
	vv = graph.getVert();
	EXPECT_EQ(2, vv->info);
	EXPECT_EQ(1, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(1, vv->info);
	EXPECT_EQ(0, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(3, vv->info);
	EXPECT_EQ(2, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(5, vv->info);
	EXPECT_EQ(2, colors[vv]);
	vv = graph.getVertNext(vv);
	EXPECT_EQ(4, vv->info);
	EXPECT_EQ(1, colors[vv]);

}