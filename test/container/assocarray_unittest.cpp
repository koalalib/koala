#include <string>
// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest.h"

#include "koala/graph/graph.h"
#include "koala/container/assoctab.h"

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

class AssocArrayTest : public ::testing::Test {

public:

	typedef Graph<char, string> MyGraph;
	typedef MyGraph::PVertex GVert;

	virtual void SetUp() {
		A = g.addVert('A'); B = g.addVert('B'); C = g.addVert('C'); D = g.addVert('D');
	}

	// virtual void TearDown() {}

	// constructing Graph
	MyGraph g;
	GVert A, B, C, D;
};

TEST_F(AssocArrayTest, MethodCapacityTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	myArray.delKey(B);

	EXPECT_EQ(3, (int)(myArray.size()));
	EXPECT_EQ(4, (int)(myArray.capacity()));
}

TEST_F(AssocArrayTest, MethodClearTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;
	myArray.clear();
	myArray[A] = 5;
	myArray[B] = 6;

	EXPECT_EQ(2, (int)(myArray.size()));
	EXPECT_EQ(4, (int)(myArray.capacity()));
}

TEST_F(AssocArrayTest, MethodDefragTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	myArray.delKey(B);

	GVert k = myArray.firstKey();
	EXPECT_EQ('A', k->info);
	EXPECT_EQ(1, myArray[k]);
	EXPECT_EQ(0, myArray.keyPos(k));
	k = myArray.nextKey(k);
	EXPECT_EQ('C', k->info);
	EXPECT_EQ(3, myArray[k]);
	EXPECT_EQ(2, myArray.keyPos(k));
	k = myArray.nextKey(k);
	EXPECT_EQ('D', k->info);
	EXPECT_EQ(4, myArray[k]);
	EXPECT_EQ(3, myArray.keyPos(k));

	myArray.defrag();

	k = myArray.firstKey();
	EXPECT_EQ('A', k->info);
	EXPECT_EQ(1, myArray[k]);
	EXPECT_EQ(0, myArray.keyPos(k));
	k = myArray.nextKey(k);
	EXPECT_EQ('C', k->info);
	EXPECT_EQ(3, myArray[k]);
	EXPECT_EQ(1, myArray.keyPos(k));
	k = myArray.nextKey(k);
	EXPECT_EQ('D', k->info);
	EXPECT_EQ(4, myArray[k]);
	EXPECT_EQ(2, myArray.keyPos(k));
}

TEST_F(AssocArrayTest, MethodDelKeyTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	EXPECT_TRUE(myArray.delKey(B));
	EXPECT_FALSE(myArray.delKey(B));

	GVert k = myArray.firstKey();
	EXPECT_EQ('A', k->info);
	EXPECT_EQ(1, myArray[k]);
	k = myArray.nextKey(k);
	EXPECT_EQ('C', k->info);
	EXPECT_EQ(3, myArray[k]);
	k = myArray.nextKey(k);
	EXPECT_EQ('D', k->info);
	EXPECT_EQ(4, myArray[k]);
}

TEST_F(AssocArrayTest, MethodEmptyTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;
	
	EXPECT_FALSE(myArray.empty());
	myArray.delKey(myArray.firstKey());
	EXPECT_FALSE(myArray.empty());
	myArray.delKey(myArray.firstKey());
	EXPECT_FALSE(myArray.empty());
	myArray.delKey(myArray.firstKey());
	EXPECT_FALSE(myArray.empty());
	myArray.delKey(myArray.firstKey());
	EXPECT_TRUE(myArray.empty());

}

TEST_F(AssocArrayTest, MethodGetKeysTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	GVert *keysTab = new GVert[myArray.size()];
	int size = myArray.getKeys(keysTab);

	EXPECT_EQ('A', keysTab[0]->info);
	EXPECT_EQ('B', keysTab[1]->info);
	EXPECT_EQ('C', keysTab[2]->info);
	EXPECT_EQ('D', keysTab[3]->info);
}

TEST_F(AssocArrayTest, MethodHasKeyTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	EXPECT_TRUE(myArray.hasKey(B));

	myArray.delKey(B);

	EXPECT_FALSE(myArray.hasKey(B));

}

TEST_F(AssocArrayTest, OperatorAssignmentTest)
{
	AssocArray<GVert, int> first;
	AssocArray<GVert, int> second;

	first[A] = 1;
	first[B] = 2;
	first[C] = 3;
	first[D] = 4;

	second = first;	// second now contains 4 verticles
	first = AssocArray<GVert, int>();	// and first is now empty

	EXPECT_EQ(0, first.size());
	EXPECT_EQ(4, second.size());

}

TEST_F(AssocArrayTest, OperatorNegationTest)
{
	AssocArray<GVert, int> myArray;


	EXPECT_TRUE(!myArray);

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	EXPECT_FALSE(!myArray);

}

TEST_F(AssocArrayTest, MethodPrevKeyTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	GVert k = myArray.lastKey();
	EXPECT_EQ('D', k->info);
	EXPECT_EQ(4, myArray[k]);
	k = myArray.prevKey(k);
	EXPECT_EQ('C', k->info);
	EXPECT_EQ(3, myArray[k]);
	k = myArray.prevKey(k);
	EXPECT_EQ('B', k->info);
	EXPECT_EQ(2, myArray[k]);
	k = myArray.prevKey(k);
	EXPECT_EQ('A', k->info);
	EXPECT_EQ(1, myArray[k]);
}

TEST_F(AssocArrayTest, MethodReserveTest)
{
	int AssocArraySize = 100;
	AssocArray<GVert, int> myArray;

	myArray.reserve(AssocArraySize);

	EXPECT_EQ(100, (int)myArray.capacity());
}

TEST_F(AssocArrayTest, MethodValPtrTest)
{
	AssocArray<GVert, int> myArray;

	myArray[A] = 1;
	myArray[B] = 2;
	myArray[C] = 3;
	myArray[D] = 4;

	GVert k = myArray.firstKey();
	EXPECT_EQ('A', k->info);
	EXPECT_EQ(1, *myArray.valPtr(k));
	k = myArray.nextKey(k);
	EXPECT_EQ('B', k->info);
	EXPECT_EQ(2, *myArray.valPtr(k));
	k = myArray.nextKey(k);
	EXPECT_EQ('C', k->info);
	EXPECT_EQ(3, *myArray.valPtr(k));
	k = myArray.nextKey(k);
	EXPECT_EQ('D', k->info);
	EXPECT_EQ(4, *myArray.valPtr(k));
}
