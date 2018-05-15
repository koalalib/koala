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

class AssocMatrixTest : public ::testing::Test {

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

TEST_F(AssocMatrixTest, MethodClearTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	std::pair<GVert, GVert>k = myMatrix.firstKey();
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(1, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(2, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(3, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(4, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(5, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(6, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('C', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(7, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('D', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(8, myMatrix(k));

	myMatrix.clear();
	myMatrix(A, A) = 5;
	myMatrix(A, B) = 6;

	k = myMatrix.firstKey();
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(5, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(6, myMatrix(k));
}

TEST_F(AssocMatrixTest, ConstructorAMatrClTriangleTest)
{
	AssocMatrix<GVert, int, AMatrClTriangle> first;

	first(A, A) = 1;
	first(A, B) = 2;
	first(A, C) = 3;
	first(A, D) = 4;
	first(B, A) = 5;	// the same as A,B
	first(B, B) = 6;
	first(C, D) = 7;
	first(D, C) = 8;	// the same as C,D

	AssocMatrix<GVert, int, AMatrClTriangle> second(first);

	EXPECT_EQ(6, first.size());
	EXPECT_EQ(6, second.size());
}

TEST_F(AssocMatrixTest, ConstructorAMatrFullTest)
{
	AssocMatrix<GVert, int, AMatrFull> first;

	first(A, A) = 1;
	first(A, B) = 2;
	first(A, C) = 3;
	first(A, D) = 4;
	first(B, A) = 5;
	first(B, B) = 6;
	first(C, D) = 7;
	first(D, C) = 8;

	AssocMatrix<GVert, int, AMatrFull> second(first);

	EXPECT_EQ(8, first.size());
	EXPECT_EQ(8, second.size());
}

TEST_F(AssocMatrixTest, ConstructorAMatrNoDiagTest)
{
	AssocMatrix<GVert, int, AMatrNoDiag> first;

	//first(A,A)=1; - elements on diagonal are prohibited
	first(A, B) = 2;
	first(A, C) = 3;
	first(A, D) = 4;
	first(B, A) = 5;
	//first(B,B)=6; - elements on diagonal are prohibited
	first(C, D) = 7;
	first(D, C) = 8;

	AssocMatrix<GVert, int, AMatrNoDiag> second(first);

	EXPECT_EQ(6, first.size());
	EXPECT_EQ(6, second.size());
}

//TEST_F(AssocMatrixTest, ConstructorAMatrNoDiagTest2)
//{
//	AssocMatrix<GVert, int, AMatrNoDiag> first;
//
//	EXPECT_ANY_THROW(first(A, A) = 1);
//
//}

TEST_F(AssocMatrixTest, ConstructorAMatrTriangleTest)
{
	AssocMatrix<GVert, int, AMatrTriangle> first;

	//first(A,A)=1; - elements on diagonal are prohibited
	first(A, B) = 2;
	first(A, C) = 3;
	first(A, D) = 4;
	first(B, A) = 5;	// the same as A,B
	//first(B,B)=6; - elements on diagonal are prohibited
	first(C, D) = 7;
	first(D, C) = 8;	// the same as C,D

	AssocMatrix<GVert, int, AMatrTriangle> second(first);

	EXPECT_EQ(4, first.size());
	EXPECT_EQ(4, second.size());
}

TEST_F(AssocMatrixTest, MethodCorrectPosTest)
{
	AssocMatrix<GVert, int, AMatrNoDiag> myMatrix;

	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_FALSE(myMatrix.correctPos(A, A));
	EXPECT_TRUE(myMatrix.correctPos(A, B));
	EXPECT_TRUE(myMatrix.correctPos(C, A));
}

TEST_F(AssocMatrixTest, MethodDefragTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	myMatrix.delKey(A, B);
	myMatrix.defrag();

	std::pair<GVert, GVert>k = myMatrix.firstKey();
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(1, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(3, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(4, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(5, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(6, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('C', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(7, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('D', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(8, myMatrix(k));
}

TEST_F(AssocMatrixTest, MethodDelIndTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_TRUE(myMatrix.delInd(B));
	EXPECT_FALSE(myMatrix.delInd(B));

	std::pair<GVert, GVert>k = myMatrix.firstKey();
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(1, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(3, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(4, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('C', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(7, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('D', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(8, myMatrix(k));
}


TEST_F(AssocMatrixTest, MethodDelKeyTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_TRUE(myMatrix.delKey(A, B));
	EXPECT_FALSE(myMatrix.delKey(A, B));

	std::pair<GVert, GVert>k = myMatrix.firstKey();
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(1, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(3, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(4, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(5, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(6, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('C', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(7, myMatrix(k));
	k = myMatrix.nextKey(k);
	EXPECT_EQ('D', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(8, myMatrix(k));
}

TEST_F(AssocMatrixTest, MethodEmptyTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;
	
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_FALSE(myMatrix.empty());
	myMatrix.delKey(myMatrix.firstKey());
	EXPECT_TRUE(myMatrix.empty());
}

TEST_F(AssocMatrixTest, MethodFirstIndTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(C, D) = 3;
	myMatrix(D, C) = 4;

	GVert k = myMatrix.firstInd();
	EXPECT_EQ('A', k->info);
	k = myMatrix.nextInd(k);
	EXPECT_EQ('B', k->info);
	k = myMatrix.nextInd(k);
	EXPECT_EQ('C', k->info);
	k = myMatrix.nextInd(k);
	EXPECT_EQ('D', k->info);
}

TEST_F(AssocMatrixTest, MethodGetKeysTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	std::pair<GVert, GVert> *keysTab = new std::pair<GVert, GVert>[myMatrix.size()];
	EXPECT_EQ(8, myMatrix.getKeys(keysTab));
	
	EXPECT_EQ('A', keysTab[0].first->info);
	EXPECT_EQ('A', keysTab[0].second->info);
	EXPECT_EQ('A', keysTab[1].first->info);
	EXPECT_EQ('B', keysTab[1].second->info);
	EXPECT_EQ('A', keysTab[2].first->info);
	EXPECT_EQ('C', keysTab[2].second->info);
	EXPECT_EQ('A', keysTab[3].first->info);
	EXPECT_EQ('D', keysTab[3].second->info);
	EXPECT_EQ('B', keysTab[4].first->info);
	EXPECT_EQ('A', keysTab[4].second->info);
	EXPECT_EQ('B', keysTab[5].first->info);
	EXPECT_EQ('B', keysTab[5].second->info);
	EXPECT_EQ('C', keysTab[6].first->info);
	EXPECT_EQ('D', keysTab[6].second->info);
	EXPECT_EQ('D', keysTab[7].first->info);
	EXPECT_EQ('C', keysTab[7].second->info);
}

TEST_F(AssocMatrixTest, MethodHasIndTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_TRUE(myMatrix.hasInd(A));

	myMatrix.delInd(A);

	EXPECT_FALSE(myMatrix.hasInd(A));
}


TEST_F(AssocMatrixTest, MethodHasKeyTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_TRUE(myMatrix.hasKey(A, B));

	myMatrix.delKey(A, B);

	EXPECT_FALSE(myMatrix.hasKey(A, B));
}

TEST_F(AssocMatrixTest, MethodIndSizeTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_EQ(4, (int)myMatrix.indSize());
}

TEST_F(AssocMatrixTest, MethodKeyTest)
{
	pair<GVert, GVert> k = pair<GVert, GVert>(A, B);
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_EQ('A', myMatrix.key(k).first->info);
	EXPECT_EQ('B', myMatrix.key(k).second->info);
	EXPECT_EQ(2, myMatrix(myMatrix.key(k)));
}

TEST_F(AssocMatrixTest, MethodLastIndTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(C, D) = 3;
	myMatrix(D, C) = 4;

	GVert k = myMatrix.lastInd();
	EXPECT_EQ('D', k->info);
	k = myMatrix.prevInd(k);
	EXPECT_EQ('C', k->info);
	k = myMatrix.prevInd(k);
	EXPECT_EQ('B', k->info);
	k = myMatrix.prevInd(k);
	EXPECT_EQ('A', k->info);
	k = myMatrix.prevInd(k);
}

TEST_F(AssocMatrixTest, MethodLastKeyTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	std::pair<GVert, GVert>k = myMatrix.lastKey();
	EXPECT_EQ('D', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(8, myMatrix(k));
	k = myMatrix.prevKey(k);
	EXPECT_EQ('C', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(7, myMatrix(k));
	k = myMatrix.prevKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(6, myMatrix(k));
	k = myMatrix.prevKey(k);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(5, myMatrix(k));
	k = myMatrix.prevKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(4, myMatrix(k));
	k = myMatrix.prevKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(3, myMatrix(k));
	k = myMatrix.prevKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(2, myMatrix(k));
	k = myMatrix.prevKey(k);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(1, myMatrix(k));
}

TEST_F(AssocMatrixTest, OperatorAssignmentTest)
{
	AssocMatrix<GVert, int, AMatrFull> first;
	AssocMatrix<GVert, int, AMatrFull> second;

	first(A, A) = 1;
	first(A, B) = 2;
	first(A, C) = 3;
	first(A, D) = 4;
	first(B, A) = 5;
	first(B, B) = 6;
	first(C, D) = 7;
	first(D, C) = 8;

	second = first;	// second now contains 8 verticles
	first = AssocMatrix<GVert, int, AMatrFull>();	// and first is now empty

	EXPECT_EQ(0, first.size());
	EXPECT_EQ(8, second.size());
	EXPECT_EQ(1, second(A, A));
	EXPECT_EQ(2, second(A, B));
	EXPECT_EQ(3, second(A, C));
	EXPECT_EQ(4, second(A, D));
	EXPECT_EQ(5, second(B, A));
	EXPECT_EQ(6, second(B, B));
	EXPECT_EQ(7, second(C, D));
	EXPECT_EQ(8, second(D, C));
}



TEST_F(AssocMatrixTest, OperatorBracketsTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;
	myMatrix(A, A) = 1;
	myMatrix(A, B) = myMatrix(A, A);
	EXPECT_EQ(1, myMatrix(A, A));
	EXPECT_EQ(1, myMatrix(A, B));
}



TEST_F(AssocMatrixTest, OperatorBracketsTest2)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = myMatrix(A, C);

	EXPECT_EQ(1, myMatrix(A, A));
	EXPECT_EQ(2, myMatrix(A, B));
	EXPECT_EQ(3, myMatrix(A, C));
	EXPECT_EQ(3, myMatrix(A, D));

	EXPECT_EQ(4, myMatrix.size());
}

TEST_F(AssocMatrixTest, OperatorNegationTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	EXPECT_TRUE(!myMatrix);

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;

	EXPECT_FALSE(!myMatrix);
}

TEST_F(AssocMatrixTest, MethodShapeTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrixFull;
	AssocMatrix<GVert, int, AMatrNoDiag> myMatrixNoDiag;
	AssocMatrix<GVert, int, AMatrTriangle> myMatrixTriangle;
	AssocMatrix<GVert, int, AMatrClTriangle> myMatrixClTriangle;

	EXPECT_EQ(0, myMatrixFull.shape);
	EXPECT_EQ(1, myMatrixNoDiag.shape);
	EXPECT_EQ(2, myMatrixTriangle.shape);
	EXPECT_EQ(3, myMatrixClTriangle.shape);

	EXPECT_EQ(AMatrFull, myMatrixFull.shape);
	EXPECT_EQ(AMatrNoDiag, myMatrixNoDiag.shape);
	EXPECT_EQ(AMatrTriangle, myMatrixTriangle.shape);
	EXPECT_EQ(AMatrClTriangle, myMatrixClTriangle.shape);
}

TEST_F(AssocMatrixTest, MethodSizeTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	EXPECT_EQ(8, (int)myMatrix.size());
}

void check(map<Graph<char, string>::PVertex, int>::iterator it, bool* keyExist)
{
	switch ((*it).first->info)
	{
	case 'A':
		EXPECT_EQ((*it).second, 1);
		EXPECT_FALSE(keyExist[0]);
		keyExist[0] = true;
		break;
	case 'B':
		EXPECT_EQ((*it).second, 2);
		EXPECT_FALSE(keyExist[1]);
		break;
	case 'C':
		EXPECT_EQ((*it).second, 3);
		EXPECT_FALSE(keyExist[2]);
		break;
	case 'D':
		EXPECT_EQ((*it).second, 4);
		EXPECT_FALSE(keyExist[3]);
		break;
	}
}

//TEST_F(AssocMatrixTest, MethodSlice1)
//{
//	AssocMatrix<GVert, int, AMatrFull> myMatrix;
//	map<GVert, int> slice1;
//	map<GVert, int>::iterator it;
//
//	myMatrix(A, A) = 1;
//	myMatrix(A, B) = 2;
//	myMatrix(A, C) = 3;
//	myMatrix(A, D) = 4;
//	myMatrix(B, A) = 5;
//	myMatrix(B, B) = 6;
//	myMatrix(C, D) = 7;
//	myMatrix(D, C) = 8;
//
//	myMatrix.slice1<GVert, map<GVert, int>>(A, slice1);
//
//	bool keyExist[4];
//	for (int i = 0; i < 4; i++) keyExist[i] = false;
//
//	it = slice1.begin();
//	check(it, keyExist);
//	it++;
//	check(it, keyExist);
//	it++;
//	check(it, keyExist);
//	it++;
//	check(it, keyExist);
//	
//}
//
//TEST_F(AssocMatrixTest, MethodSlice2)
//{
//	AssocMatrix<GVert, int, AMatrFull> myMatrix;
//	map<GVert, int> slice2;
//	map<GVert, int>::iterator it;
//
//	myMatrix(A, A) = 1;
//	myMatrix(A, B) = 2;
//	myMatrix(A, C) = 3;
//	myMatrix(A, D) = 4;
//	myMatrix(B, A) = 5;
//	myMatrix(B, B) = 6;
//	myMatrix(C, D) = 7;
//	myMatrix(D, C) = 8;
//
//	myMatrix.slice1<GVert, map<GVert, int>>(A, slice2);
//
//	bool keyExist[4];
//	for (int i = 0; i < 4; i++) keyExist[i] = false;
//
//	it = slice2.begin();
//	check(it, keyExist);
//	it++;
//	check(it, keyExist);
//	it++;
//	check(it, keyExist);
//	it++;
//	check(it, keyExist);
//}

TEST_F(AssocMatrixTest, MethodValPtrTest)
{
	AssocMatrix<GVert, int, AMatrFull> myMatrix;

	myMatrix(A, A) = 1;
	myMatrix(A, B) = 2;
	myMatrix(A, C) = 3;
	myMatrix(A, D) = 4;
	myMatrix(B, A) = 5;
	myMatrix(B, B) = 6;
	myMatrix(C, D) = 7;
	myMatrix(D, C) = 8;

	std::pair<GVert, GVert>k = myMatrix.firstKey();
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(1, myMatrix(k.first, k.second));
	k = myMatrix.nextKey(k.first, k.second);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(2, myMatrix(k.first, k.second));
	k = myMatrix.nextKey(k.first, k.second);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(3, myMatrix(k.first, k.second));
	k = myMatrix.nextKey(k.first, k.second);
	EXPECT_EQ('A', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(4, myMatrix(k.first, k.second));
	k = myMatrix.nextKey(k.first, k.second);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('A', k.second->info);
	EXPECT_EQ(5, myMatrix(k.first, k.second));
	k = myMatrix.nextKey(k.first, k.second);
	EXPECT_EQ('B', k.first->info);
	EXPECT_EQ('B', k.second->info);
	EXPECT_EQ(6, myMatrix(k.first, k.second));
	k = myMatrix.nextKey(k.first, k.second);
	EXPECT_EQ('C', k.first->info);
	EXPECT_EQ('D', k.second->info);
	EXPECT_EQ(7, myMatrix(k.first, k.second));
	k = myMatrix.nextKey(k.first, k.second);
	EXPECT_EQ('D', k.first->info);
	EXPECT_EQ('C', k.second->info);
	EXPECT_EQ(8, myMatrix(k.first, k.second));
}