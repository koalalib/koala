// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest.h"

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


class GreedyInnerTest : public ::testing::Test {

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


TEST_F(GreedyInnerTest, greedyInterSeqVertColoringTest)
{

	// finding greedy coloring with interchange
	int colors = SeqVertColoring::greedyInter(g, vertCont);

	EXPECT_EQ(2, colors);

	EXPECT_EQ('A', V[0]->info);
	EXPECT_EQ('B', V[1]->info);
	EXPECT_EQ('C', V[2]->info);
	EXPECT_EQ('D', V[3]->info);
	EXPECT_EQ('E', V[4]->info);
	EXPECT_EQ('F', V[5]->info);
	EXPECT_EQ('G', V[6]->info);
	EXPECT_EQ(0, vertCont[V[0]]);
	EXPECT_EQ(2, vertCont[V[1]]);
	EXPECT_EQ(1, vertCont[V[2]]);
	EXPECT_EQ(2, vertCont[V[3]]);
	EXPECT_EQ(0, vertCont[V[4]]);
	EXPECT_EQ(1, vertCont[V[5]]);
	EXPECT_EQ(1, vertCont[V[6]]);

	vertCont.clear();
	// finding greedy coloring with interchange for vertices B to E
	colors = SeqVertColoring::greedyInter(g, vertCont, V + 1, V + 5); // see Fig.

	EXPECT_EQ(2, colors);
	
	EXPECT_EQ('B', V[1]->info);
	EXPECT_EQ('C', V[2]->info);
	EXPECT_EQ('D', V[3]->info);
	EXPECT_EQ('E', V[4]->info);
	EXPECT_EQ(0, vertCont[V[1]]);
	EXPECT_EQ(1, vertCont[V[2]]);
	EXPECT_EQ(0, vertCont[V[3]]);
	EXPECT_EQ(2, vertCont[V[4]]);

	vertCont.clear();
	// finding greedy coloring with interchange for colors less than 3
	colors = SeqVertColoring::greedyInter(g, vertCont, 2); // see Fig.

	EXPECT_EQ(2, colors);

	EXPECT_EQ(7, g.getVertNo());
	EXPECT_EQ('A', V[0]->info);
	EXPECT_EQ('B', V[1]->info);
	EXPECT_EQ('C', V[2]->info);
	EXPECT_EQ('D', V[3]->info);
	EXPECT_EQ('E', V[4]->info);
	EXPECT_EQ('F', V[5]->info);
	EXPECT_EQ('G', V[6]->info);
	EXPECT_EQ(0, vertCont[V[0]]);
	EXPECT_EQ(2, vertCont[V[1]]);
	EXPECT_EQ(1, vertCont[V[2]]);
	EXPECT_EQ(2, vertCont[V[3]]);
	EXPECT_EQ(0, vertCont[V[4]]);
	EXPECT_EQ(1, vertCont[V[5]]);
	EXPECT_EQ(1, vertCont[V[6]]);

	vertCont.clear(), cout << endl;
	// finding greedy coloring with interchange for vertices B to E and colors less than 3
	colors = SeqVertColoring::greedyInter(g, vertCont, V + 1, V + 5, 2); // see Fig.

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