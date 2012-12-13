#define KOALA_SET_ON_VECTOR 1

#include<stdio.h>
#include<iostream>
#include"Koala/base/defs.h"
#include"Koala/graph/graph.h"
#include"Koala/algorithm/search.h"
#include"Koala/io/text.h"
#include "Koala/classes/detect.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

const char* print(ModPartType x)
{
    switch (x) {
        case mpTrivial : return "Trivial";
        case mpConnected : return "Connected";
        case mpDisconnected : return "Disconnected";
        case mpPrime : return "Prime";
    };
    assert(0);
    return NULL;
}

#include "main.hpp"
	Graph<char> gk3;
	Graph<char>::PVertex tabV[20],tabV2[20],F,G,H;
	F=gk3.addVert('F');G=gk3.addVert('G');H=gk3.addVert('H');
	gk3.addEdge(F,G); gk3.addEdge(F,H);gk3.addEdge(H,G);

	Graph<char> gn4;
	Graph<char>::PVertex I,J,K,L;
	I=gn4.addVert('I');J=gn4.addVert('J');K=gn4.addVert('K');L=gn4.addVert('L');

	Graph<char> gp4;
	Graph<char>::PVertex M,N,O,P;
	M=gp4.addVert('M');N=gp4.addVert('N');O=gp4.addVert('O');P=gp4.addVert('P');
	gp4.addEdge(O,N);gp4.addEdge(M,N);gp4.addEdge(M,P);



	Graph<char> g;
	Graph<char>::PVertex A,B,C,D;

	A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');//E=g.addVert('E');
	g.addEdge(B,D);
	g.addEdge(A,B);
	g.addEdge(C,A);//g.addEdge(C,D);


//	g.substitute(A,gp4,make_pair(stdChoose(true),stdChoose(true)),make_pair(stdCast(),stdCast()));
    g.substitute(C,gk3,make_pair(stdChoose(true),stdChoose(true)),make_pair(stdCast(),stdCast()));
	g.substitute(A,gp4,make_pair(stdChoose(true),stdChoose(true)),make_pair(stdCast(),stdCast()));
	g.substitute(B,gp4,make_pair(stdChoose(true),stdChoose(true)),make_pair(stdCast(),stdCast()));
	g.substitute(D,gk3,make_pair(stdChoose(true),stdChoose(true)),make_pair(stdCast(),stdCast()));

//	g.addVert('X');
//g.addEdge(g.getVert(),g.getVertLast());

//	g.addEdge(C,D);

	SearchStructs::CompStoreTool<Graph<char>::PVertex> tool;
	AssocTable<std::map<Graph<char>::PVertex,int> > vertTab;

//    cout << "\n\n" << boolalpha << print(Modules::split(gp4,tool.input(),vertTab).type) << endl;
//    gp4.substitute(gp4.getVertLast(),gk3,make_pair(stdChoose(true),stdChoose(true)),make_pair(stdCast(),stdCast()));
#define _g g
	Modules::Partition res=Modules::split(_g,tool.input(),vertTab);


    cout << "size: "<< res.size << " type: " << print(res.type) << endl;
    for(Graph<char>::PVertex v=_g.getVert();v;v=_g.getVertNext(v)) cout << v->info << ":" << vertTab[v] <<"\t";
    cout << "\nsize: " << tool.size() << "\tlen:" <<tool.length();
    for(int i=0;i<tool.size();i++)
    {
        cout << "\n"<<tool.size(i) << ":";
        for(int j=0;j<tool.size(i);j++) cout << ' ' << tool[i][j]->info;
    }

    cout << "\n\ncograph\n";

    g.clear();

    g.copy(gk3,stdChoose(true) & stdChoose(true),stdCast() & stdCast());
    g.substitute(g.getVertLast(),gn4,stdChoose(true) & stdChoose(true),stdCast() & stdCast());
//    gk3.del(gk3.getVert());
//    g.substitute(g.getVert(),gk3,stdChoose(true) & stdChoose(true),stdCast() & stdCast());

    cout << "\n\n" << boolalpha << IsIt::cograph(g);
    int q=IsIt::Cograph::maxStable(g,tabV);
    cout << endl << q << endl;
    for(int i=0;i<q;i++) cout << tabV[i]->info << ' ';

    g.neg();

    q=IsIt::Cograph::maxClique(g,tabV);
    cout << endl << q << endl;
    for(int i=0;i<q;i++) cout << tabV[i]->info << ' ';




//    writeGraphText(g, cout, RG_VertexLists|RG_VInfo);



	return 0;
	};
