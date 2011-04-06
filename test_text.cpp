
#include<stdio.h>
#include<string.h>

#include<string>

#include"./koala/graph/graph.h"
#include"./koala/io/text.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;


int main() {
	string buf;
	Graph<char,int> g;
	Graph<char,int>::PVertex A,B,C,D;

	C=g.addVert('C');B=g.addVert('B');A=g.addVert('A');D=g.addVert('D');
//	C=g.addVert();B=g.addVert();A=g.addVert();D=g.addVert();

	g.addEdge(A,B,1);g.addArch(B,C,2);g.addLoop(C,3);
//	g.addEdge(A,B);g.addArch(B,C);g.addLoop(C);


	writeGraphText(g, cout, RG_DirectedVertexLists);
	cout << endl;

	writeGraphText(g, cout, RG_UndirectedVertexLists);
	cout << endl;

	writeGraphText(g, cout, RG_DirectedEdgeList);
	cout << endl;

	writeGraphText(g, cout, RG_UndirectedEdgeList);
	cout << endl;


}
