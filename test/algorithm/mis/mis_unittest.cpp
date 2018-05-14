// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest-spi.h"

#include "koala/graph/graph.h"
#include "koala/algorithm/mis.h"

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


class MISTest : public ::testing::Test {

public:

	typedef Koala::Graph < char, string > MyGraph;

	MyGraph g;
	MyGraph::PVertex A, B, C, D, E, F, G, H;
	MyGraph::PEdge ab, ac, bd, bc, bh, cd, ce, de, df;

	AssocArray < MyGraph::PVertex, double > vertMap; // input container
	MyGraph::PVertex tabV[20]; // output

	virtual void SetUp() {
		A = g.addVert('A'); B = g.addVert('B'); C = g.addVert('C');
		D = g.addVert('D'); E = g.addVert('E'); F = g.addVert('F');
		G = g.addVert('G'); H = g.addVert('H');

		ab = g.addEdge(A, B, "ab"); ac = g.addEdge(A, C, "ac");
		bc = g.addEdge(B, C, "bc"); bd = g.addEdge(B, D, "bd"); bh = g.addEdge(B, H, "bh");
		cd = g.addEdge(D, C, "cd"); ce = g.addEdge(C, E, "ce");
		de = g.addEdge(D, E, "de"); df = g.addEdge(F, D, "df");

		vertMap[A] = 1.0; vertMap[B] = 2.0; vertMap[C] = 4.0; vertMap[D] = 3.0;
		vertMap[E] = 3.0; vertMap[F] = 1.0; vertMap[G] = 1.0; vertMap[H] = 2.0;

		misWeight = 0.0;
	}

	// virtual void TearDown() {}

	double misWeight;
	unsigned result;
};

TEST_F(MISTest, GetWMaxWithMISStrategyFirstTest)
{

	result = MaxStableHeur::getWMax(g, tabV, MaxStableStrategy::First(), vertMap);
	ASSERT_EQ(4, result);

	EXPECT_EQ('E', tabV[0]->info);
	EXPECT_EQ('F', tabV[1]->info);
	EXPECT_EQ('G', tabV[2]->info);
	EXPECT_EQ('H', tabV[3]->info);

	for (unsigned int i = 0; i<result; ++i) misWeight += vertMap[tabV[i]];

	EXPECT_EQ(7, misWeight);
}

TEST_F(MISTest, GetWMaxWithMISStrategyGWMaxTest)
{

	/*result = MaxStableHeur::getWMax(g, tabV, MaxStableStrategy::GGWMax(), vertMap);
	ASSERT_EQ(4, result);

	EXPECT_EQ('E', tabV[0]->info);
	EXPECT_EQ('F', tabV[1]->info);
	EXPECT_EQ('G', tabV[2]->info);
	EXPECT_EQ('H', tabV[3]->info);

	for (unsigned int i = 0; i<result; ++i) misWeight += vertMap[tabV[i]];

	EXPECT_EQ(7, misWeight);*/
}