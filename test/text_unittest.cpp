// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest.h"

#include"koala/graph/graph.h"
#include"koala/io/text.h"

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
using namespace Koala::IO;

TEST(readGraphText_ReadGraphFromTextFormat)
{
	Graph<int, int> g;

	EXPECT_TRUE(true,
		readGraphText(g, "6\n"
			"0 1 -1\n"
			"1 2 -2 >3\n"
			"2 4 -3 >4 >4 @2\n"
			"3 1 -5\n"
			"4 0\n"
			"5 0\n", RG_VertexLists);
	);
}




