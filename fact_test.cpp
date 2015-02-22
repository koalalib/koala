
#include<stdio.h>
#include<string.h>

#include<string>
#include <fstream>

#include"./Koala/graph/graph.h"
#include"./Koala/io/g6.h"
#include"./Koala/io/text.h"
#include"./Koala/algorithm/matching.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

Graph<char,string>::PEdge tabE[10];
std::pair<Graph<char,string>::PEdge,Graph<char,string>::PVertex> tabPair[10];



#include "main.hpp"
	string buf;
	Graph<char,string> g,g2;
	Graph<char,string>::PVertex A,B,C,D,E,F,tabV[10];
    Graph<char,string>::PEdge d,e,f,h,p,q,x,y,z,tabE[100];
    char text[10000];

    AssocTable<std::map<Graph<char,string>::PVertex,int> > v2int;
    AssocTable<std::map<Graph<char,string>::PVertex,Factor::DegRange> > v2seg;
    AssocTable<std::map<Graph<char,string>::PVertex,Segment> > v2Seg;
    AssocTable<std::map<Graph<char,string>::PEdge,int> > e2int;


	A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
//	C=g.addVert();B=g.addVert();A=g.addVert();D=g.addVert();


	d=g.addEdge(A,B,"ab");
	e=g.addEdge(A,C,"ac");
	f=g.addArc(B,C,"bc");
	h=g.addLoop(C,"cc");
	g.addLoop(C,"cc2");
	g.addLoop(C,"cc3");
	x=g.addArc(A,C,"ac1");
	y=g.addArc(C,A,"ac2");
//	z=g.addEdge(C,B,"bc2");
//	g.addArc(C,A,"ac3");
//	g.addArc(A,C,"ac4");
////	g.addEdge(A,B);g.addArc(B,C);g.addLoop(C);
//    z=g.addLoop(D,"DD");g.addLoop(D,"dd");


	cout << std::boolalpha;
	cout << writeGraphText(g, cout, RG_VertexLists|RG_Info);
	cout << endl;

    v2int[A]=1;v2int[B]=0;v2int[C]=3;v2int[D]=0;
//    v2seg[A]=Factor::DegRange(1,5,Factor::DegEven);v2seg[B]=Segment(1,1);
//    v2seg[C]=Factor::DegRange(3,25,Factor::DegOdd);v2seg[D]=Segment(0,0);

    v2Seg[A]=Segment(2,2);v2Seg[B]=Segment(1,1);
    v2Seg[C]=Segment(3,5);v2Seg[D]=Segment(0,0);


	int res=Factor::segFind(g,v2Seg,Segment(4,4),tabE);
	if (res==-1) cout <<"\nBrak\n"; else cout << "res=" << res << "\n";
	for(int i=0;i<res;i++) cout << tabE[i]->info << ' ';

return 0;
}
