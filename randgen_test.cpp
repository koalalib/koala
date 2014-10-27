#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "Koala/io/text.h"
#include "Koala/graph/graph.h"
#include "Koala/base/rnd.h"
#include "Koala/classes/create.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

int randGraphTests();
Koala::IO::ParSet vInfoFun1(int num);
Koala::IO::ParSet eInfoFun1(int num1, int num2, EdgeDirection dir);
void printDegreeStats(Graph<ParSet, ParSet> &g, std::ostream &out, bool skipZeros = false);

int main()
{
	randGraphTests();
	return 0;
}

int randGraphTests() {

	std::vector<int> vec;
	Koala::Graph<ParSet, ParSet> g;
	std::ofstream out;
	Koala::StdRandGen<> rgen;
	cout << "Start randGraphTests" << endl;
	//Create a graph according to ErdosRenyi model G(n,p)
	g.clear();
	Koala::Creator::erdRen1(rgen, g, 5, 0.7);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("erd-ren1-1.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	g.clear();
	Koala::Creator::erdRen1(rgen, g, 10, 0.7);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("erd-ren1-2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	//Create a graph according to ErdosRenyi model G(n,m).
	g.clear();
	Koala::Creator::erdRen2(rgen, g, 8, 12);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("erd-ren2-1.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();

	g.clear();
	Koala::Creator::erdRen2(rgen, g, 12, 65);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VInfo);
	out.open("erd-ren2-2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();


	//barAlb
	g.clear();
	Koala::Creator::barAlb(rgen, g, 100, 3);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("barb-alb1.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_VInfo);
	out.close();
	out.open("barb-alb1-stat.txt", std::ios::out | std::ios::trunc);
	printDegreeStats(g, out);
	out.close();

	g.clear();
	Koala::Creator::barAlb(rgen, g, 20, 1, Directed);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("barb-alb2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	g.clear();
	Koala::Creator::barAlb(rgen, g, 40, 3, vInfoFun1, eInfoFun1, Undirected, true);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("barb-alb3.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();


	g.clear();
	Koala::Creator::barAlb(rgen, g, 30, 1, vInfoFun1, eInfoFun1, EdDirOut, true);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("barb-alb4.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();


	g.clear();
	Koala::Creator::barAlb(rgen, g, 50, 4, vInfoFun1, eInfoFun1, Directed, false);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("barb-alb5.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	//wattStrog1
	g.clear();
	Koala::Creator::wattStrog1(rgen, g, 12, 4, 0);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong1-1.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	g.clear();
	Koala::Creator::wattStrog1(rgen, g, 12, 4, 0.5);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong1-2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	g.clear();
	Koala::Creator::wattStrog1(rgen, g, 12, 4, 1 - 0.000001);
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong1-3.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	g.clear();
	Koala::Creator::wattStrog1(rgen, g, 120, 40, 0.7);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong1-4.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	//wattStrog2
	g.clear();
	Koala::Creator::wattStrog2(rgen, g, 12, 4, 0);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong2-1.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	g.clear();
	Koala::Creator::wattStrog2(rgen, g, 12, 4, 0.5);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong2-2.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	g.clear();
	Koala::Creator::wattStrog2(rgen, g, 12, 4, 1, vInfoFun1, eInfoFun1, Directed, true);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong2-3.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	g.clear();
	Koala::Creator::wattStrog2(rgen, g, 120, 40, 0.7);
	//Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_Info);
	out.open("watt-strong2-4.txt", std::ios::out | std::ios::trunc);
	Koala::IO::writeGraphText(g, out, Koala::IO::RG_Info);
	out.close();

	cout << "End of randGraphTests" << endl;
	return 0;
}


Koala::IO::ParSet vInfoFun1(int num) {

	Koala::IO::ParSet info;
	string name = "num";
	info.set(name, num);
	return info;
}

Koala::IO::ParSet eInfoFun1(int num1, int num2, EdgeDirection dir) {

	Koala::IO::ParSet info;
	string name1 = "num1";
	string name2 = "num2";
	string name3 = "dir";
	info.set(name1, num1);
	info.set(name2, num2);
	info.set(name3, dir);
	return info;
}

void printDegreeStats(Graph<ParSet, ParSet> &g, std::ostream &out, bool skipZeros)
{
	int m = g.getEdgeNo(EdUndir | EdDirIn | EdDirOut);
	int ml = g.getEdgeNo(EdLoop);
	int degMaxPos = m + 2 * ml, d, e, l;
	int LOCALARRAY(deg, degMaxPos + 1);
	for (int i = 0; i < degMaxPos; ++i) deg[i] = 0;
	int maxDeg = 0;
	for (Graph<ParSet, ParSet>::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		e = g.getEdgeVertNo(v, EdUndir | EdDirIn | EdDirOut);
		l = g.getEdgeVertNo(v, EdLoop);
		d = e + 2 * l;
		deg[d]++;
		if (d > maxDeg) maxDeg = d;
	}

	for (int i = 0; i <= maxDeg; ++i)
	{
		d = deg[i];
		if (d == 0 && skipZeros) continue;
		out << i << "\t" << d << endl;
	}
}
