#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "Koala/graph/graph.h"
#include "Koala/algorithm/sat2cnf.h"

//#include "test_utils.h"

using namespace std;
using namespace Koala;

int sat2cnfTests1();
void readFromFile(std::vector<Sat2CNF::Clause> &inst, string fileName);
bool runTest(int numTest, std::vector<Sat2CNF::Clause> &inst, bool solution, bool printVars = false);

int main()
{
	sat2cnfTests1();
	return 0;
}

int sat2cnfTests1() {

	printf("Start sat2cnfTests1\n");
	std::vector<Sat2CNF::Clause> inst;
	bool isAllOK = true;
	int nn, numTest = 1;

	//test 1
	inst.clear();
	inst.push_back(std::make_pair(std::make_pair(10, true), std::make_pair(10, true)));
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	//test 2
	inst.clear();
	inst.push_back(std::make_pair(std::make_pair(12, false), std::make_pair(12, false)));
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	//test 3
	inst.clear();
	inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(0, true)));
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	//test 4
	inst.clear();
	inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(0, false)));
	inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(0, true)));
	isAllOK &= runTest(numTest, inst, false);
	numTest++;

	//test 5
	inst.clear();
	inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(1,true)));
	inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(2, true)));
	inst.push_back(std::make_pair(std::make_pair(35, true), std::make_pair(1, false)));
	inst.push_back(std::make_pair(std::make_pair(3, true), std::make_pair(1, true)));
	inst.push_back(std::make_pair(std::make_pair(4, false), std::make_pair(0, true)));
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	//test 6
	inst.clear();
	inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(2, true)));
	inst.push_back(std::make_pair(std::make_pair(35, true), std::make_pair(1, false)));
	inst.push_back(std::make_pair(std::make_pair(4, false), std::make_pair(0, true)));
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	//test 7
	inst.clear();
	nn = 1000;
	for (int i = 1; i <= nn; ++i)
	{
		inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(i, true)));
		inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(i, true)));
	}
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	//test 8
	inst.clear();
	nn = 10000;
	for (int i = 1; i <= nn; ++i)
	{
		inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(0, true)));
		inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(i, false)));
		inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(i, false)));
	}
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	//test 9
	inst.clear();
	for (int i = 1; i <= nn; ++i)
	{
		inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(i, true)));
		inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(i, true)));
		inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(i, false)));
		inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(i, false)));
	}
	isAllOK &= runTest(numTest, inst, false);
	numTest++;

	//test 10
	//http://www.boolsat.com
	//(x or not z)  and (not x or y) and (not y or not x) and (y or not x) and (not x and not y) and (x or z)
	//==x or not z and not x or y and not y or not x and y or not x and not x and not y and x or z
	inst.clear();
	inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(2, false)));
	inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(1, true)));
	inst.push_back(std::make_pair(std::make_pair(1, false), std::make_pair(0, false)));
	inst.push_back(std::make_pair(std::make_pair(1, true), std::make_pair(0, false)));
	inst.push_back(std::make_pair(std::make_pair(0, false), std::make_pair(1, false)));
	inst.push_back(std::make_pair(std::make_pair(0, true), std::make_pair(2, true)));
	isAllOK &= runTest(numTest, inst, false);
	numTest++;

	//test 11
	inst.clear();
	readFromFile(inst, "zzz_345_345_3-3-true.cnf");
	isAllOK &= runTest(numTest, inst, true);
	numTest++;

	if (isAllOK)
		printf("All tests OK.\n");
	else
		printf("Some tests failed!\n");

	printf("End of sat2cnfTests1\n");
	return 0;
}

bool runTest(int numTest, std::vector<Sat2CNF::Clause> &inst, bool solution, bool printVars)
{
	printf("Test num: %d, ", numTest);
	std::vector<int> vars(2 * inst.size(), 0);
	std::vector<bool> outVal(2 * inst.size(), false);
	bool isOK;
	int num = Sat2CNF::vars(inst.begin(), inst.end(), vars.begin());
	bool sol = Sat2CNF::solve(inst.begin(), inst.end(), outVal.begin());
	bool ev = Sat2CNF::eval(inst.begin(), inst.end(), outVal.begin(), outVal.begin() + outVal.size());

	printf("clauses: %d, vars: %d, minVar: %d, maxVar: %d, sol: %d. ", inst.size(), num, vars[0], vars[num - 1], sol);

	if (ev == sol && sol == solution)
	{
		isOK = true;
		printf("Test OK.\n");
	}
	else
	{
		printf("Test NOT OK!\n");
		isOK = false;
//		koalaAssert(false, ExcTestFailed);
	}

	if (printVars && sol)
	{
		for (int i = 0; i < num; ++i) printf("var: %d, value: %s\n", vars[i], outVal[i] ? "true" : "false");
	}
	return isOK;
}


void readFromFile(std::vector<Sat2CNF::Clause> &inst, string fileName)
{
	ifstream myfile(fileName.c_str());
	string line;
	int a, b, c;
	bool a_, b_;
	inst.clear();
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line[0] == 'c' || line[0] == 'p') continue;
			sscanf(line.c_str(), "%d %d %d", &a, &b, &c);
			if (a < 0) 	a_ = false, a = -a;
			else a_ = true;
			if (b < 0) 	b_ = false, b = -b;
			else b_ = true;
			inst.push_back(std::make_pair(std::make_pair(a, a_), std::make_pair(b, b_)));

		}
		myfile.close();
	}

}
