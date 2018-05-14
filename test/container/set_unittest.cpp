// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest-spi.h"

#include "koala/container/set.h"

/* TEST(test_case_name, test_name) is a predefined macro.
These are ordinary C++ functions that don't return a value.
In this function, along with any valid C++ statements you want to include,
use the various Google Test assertions to check values.
The test's result is determined by the assertions;
if any assertion in the test fails (either fatally or non-fatally),
or if the test crashes, the entire test fails. Otherwise, it succeeds.
*/

//using namespace std;
using namespace Koala;

class SetTest : public ::testing::Test {

public:

	virtual void SetUp() {
		int intValuesTab1[] = { 1, 2, 3, 4, 5 };
		int intValuesTab2[] = { 2, 1, 6, 3, 4 };
		int intValuesTab3[] = { 5, 3, 2, 1, 4 };

		//creating sets from tables
		setX = Set<int>(intValuesTab1, sizeof(intValuesTab1) / sizeof(int));
		setY = Set<int>(intValuesTab2, sizeof(intValuesTab2) / sizeof(int));
		setZ = Set<int>(intValuesTab3, sizeof(intValuesTab3) / sizeof(int));
	}

	const static int setSize = 5;
	int intValuesTab1[setSize], intValuesTab2[setSize], intValuesTab3[setSize];

	// virtual void TearDown() {}

	Set< int > setX, setY, setZ;
	Set< int >::iterator it1, it2;
};  


TEST_F(SetTest, MethodsFirstNextTest)
{

	int i = setX.first();
	EXPECT_EQ(1, i);
	i = setX.next(i);
	EXPECT_EQ(2, i);
	i = setX.next(i);
	EXPECT_EQ(3, i);
	i = setX.next(i);
	EXPECT_EQ(4, i);
	i = setX.next(i);
	EXPECT_EQ(5, i);

	i = setY.first();
	EXPECT_EQ(1, i);
	i = setY.next(i);
	EXPECT_EQ(2, i);
	i = setY.next(i);
	EXPECT_EQ(3, i);
	i = setY.next(i);
	EXPECT_EQ(4, i);
	i = setY.next(i);
	EXPECT_EQ(6, i);

	i = setZ.first();
	EXPECT_EQ(1, i);
	i = setZ.next(i);
	EXPECT_EQ(2, i);
	i = setZ.next(i);
	EXPECT_EQ(3, i);
	i = setZ.next(i);
	EXPECT_EQ(4, i);
	i = setZ.next(i);
	EXPECT_EQ(5, i);
}

TEST_F(SetTest, OperatorEqualityTest)
{
	EXPECT_FALSE(setX == setY);
	EXPECT_FALSE(setY == setZ);
	EXPECT_TRUE(setX == setZ);
	EXPECT_TRUE(setX != setY);
	EXPECT_TRUE(setY != setZ);
	EXPECT_FALSE(setX != setZ);
}


TEST_F(SetTest, DefineTest)
{

#define KOALA_SET_ON_VECTOR

}