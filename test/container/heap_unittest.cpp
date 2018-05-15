// This header file defines the public API for Google Test.
//It should be included by any test program that uses Google Test.
#include "gtest/gtest.h"

#include "koala/container/heap.h"

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

#define  LIMIT 10

TEST(HeapTest, MethodCapacityTest)
{
	// construct two heaps
	BinomHeap< int > h1, h2;

	// push LIMIT elements into the heaps
	for (int i = 0; i < LIMIT; i++) {
		h1.push(i);
		h2.push(-i);
	}

	//// print sizes
	//cout << "My 1st heap contains " << h1.size() << " elements." << endl;
	//if (!h1.empty()) cout << h1.top() << " is at its top, it's node is at " << h1.topRepr() << "." << endl;
	//cout << "My 1st heap contains " << h2.size() << " elements." << endl;
	//if (!h2.empty()) cout << h2.top() << " is at its top, it's node is at " << h2.topRepr() << "." << endl;        

	//// merge heaps into the 1st one
	//h1.merge(h2);

	//// print sizes, again
	//cout << "My 1st heap contains " << h1.size() << " elements." << endl;
	//cout << "My 2nd heap contains " << h2.size() << " elements." << endl;       

	//// remove 2/3 of elements from the 1st heap
	//for (int i = 0; i <= 4 * LIMIT / 3; i++) {
	//	cout << "Removing " << h1.top() << " from the 1st heap." << endl;
	//	h1.pop();
	//}       

	// clear the heaps
	h1.clear();
	h2.clear();

	EXPECT_EQ(LIMIT, h1.size());
}

