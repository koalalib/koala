#include<stdio.h>
#include<stdlib.h>
#include<iostream>

#include<utility>
#include<iostream>
#include<map>

#include"Koala/graph/graph.h"
#include"Koala/graph/view.h"
#include "Koala/io/text.h"
#include "Koala/classes/detect.h"
#include "Koala/classes/create.h"
#include "Koala/algorithm/mis.h"


using namespace Koala;
using namespace Koala::IO;
using namespace std;

#define TEST_1
// TEST_ 1-3

/*
 * main
 */
#include "main.hpp"
	bool rv;
	Graph<int, int> g;
	Graph<int, int>::PVertex v,A,B,C;

	readGraphText(g,
#if defined(TEST_1)
		"4\n"
		"0 3 -1 -2 -3\n"
		"1 0\n"
		"2 1 -3\n"
		"3 0\n",
#endif
#if defined(TEST_2)
		"5\n"
		"0 4 -1 -2 -3 -4\n"
		"1 3 -2 -3 -4\n"
		"2 1 -3\n"
		"3 0\n"
		"4 0\n",
#endif
#if defined(TEST_3)
		"6\n"
		"0 1 -1\n"
		"1 2 -2 -3\n"
		"2 2 -3 -4\n"
		"3 1 -5\n"
		"4 0\n"
		"5 0\n",
#endif
		RG_VertexLists);


//	rv = TestsPar<AlgorithmsDefaultSettings>::IsChordal(g);
//	rv = Comparability<AlgorithmsDefaultSettings>::IsComparabilityGraph(g);
    //g.clearEdges();//g.neg(EdUndir);
    A=g.getVert();B=g.getVertLast();C=g.getVertNext(A);
    //g.addLink(A,B);//g.addLink(C,B);//g.addLink(C,A);
	rv = IsIt::comparability(g);
	printf("RESULT: ");
	if(rv) {
		printf("true");
		printf("\n");
		int q;
		Graph<int, int>::PVertex clq[1024];
//		q = Comparability<AlgorithmsDefaultSettings>::MaxClique(g, clq);
		q = IsIt::Comparability::maxClique(g, clq);
		printf("RESULT: ");
		printf("%d: ", q);
		for(int i = 0; i < q; i++) printf("%p ", clq[i]);
		printf("\n");

		AssocTable<map<Graph<int, int>::PVertex, int> > colors;
		map<Graph<int, int>::PVertex, int>::iterator ci;
		q = IsIt::Comparability::chi(g, colors);
		printf("COLORING: ");
		for(ci = colors.cont.begin(); ci != colors.cont.end(); ++ci) {
			printf("%p: %d  ", ci->first, ci->second);
			};
		printf("\n");
		int x,xx;
		cout << "maxstable: "<< (x=IsIt::Comparability::maxStable(g,clq)) << "\n";
		assert(MaxStableHeur::testMax(g,clq,clq+x));
		colors.clear();
		cout << (xx=IsIt::Comparability::coChi(g,colors));
		assert(x==xx);
		for(v=g.getVert();v;v=g.getVertNext(v)) assert( colors[v]>=0 && colors[v]<xx);
		for(int i=0;i<xx;i++) assert(IsIt::clique(makeSubgraph(g,assocChoose(colors,i)& stdChoose(true))));
	} else printf("false");

	cout << "\n ----------\n";

       int res;
        Graph<int, int>::PVertex clq[1024];
        AssocTable<map<Graph<int, int>::PVertex, int> > colors;
        AssocTable<map<Graph<int, int>::PEdge, EdgeDirection> > dirs;
//        AssocArray <Graph<int, int>::PEdge, EdgeDirection> dirs;
        map<Graph<int, int>::PVertex, int>::iterator ci;
        if(-1!=(res=IsIt::Comparability::explore(g,dirs,colors,clq))) {
            printf("RESULT: ");
            printf("true");
            printf("\n");
            printf("RESULT: ");
            printf("%d: ", res);
            for(int i = 0; i < res; i++) printf("%p ", clq[i]);
            printf("\n");

            printf("COLORING: ");
            for(ci = colors.cont.begin(); ci != colors.cont.end(); ++ci) {
                printf("%p: %d  ", ci->first, ci->second);
                };
            printf("\n");
        } else printf("false");


    cout << endl;
    writeGraphText(g, cout, RG_VertexLists);
    cout << "\n!!!!!!!!!!!!!\n" << IsIt::Comparability::getDirs(g) << endl<< endl;
    writeGraphText(g, cout, RG_VertexLists);

    g.clear();g.addVert();g.addVert();g.addEdge(g.getVert(),g.getVertLast());
//    g.addVert();
    SearchStructs::CompStoreTool<Graph<int, int>::PVertex> out;
    cout << "\n" << boolalpha << IsIt::CompKPartite::split(g,blackHole,out.input()) << "\n" << IsIt::compKPartite(g);
    cout << "\n" << out.size() << ' ' << out.size(0);
    cout  << ' ' << out.size(1);

    cout <<"\n---------------------------\n";
    {
        Graph<char,int> g;
        g.delAdjMatrix();
        Graph<char,int>::PVertex A,B,C,D,E,F,G,H,I,J,K,L,tabV[20];

        A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
        E=g.addVert('E');
        F=g.addVert('F');G=g.addVert('G');H=g.addVert('H');I=g.addVert('I');
//        g.addArc(A,B);
//        g.addArc(B,A);
        g.addArc(A,B);
        g.addArc(A,B);g.addArc(A,B);
        g.addEdge(A,B);
//        g.addArc(B,A);
//              g.addLoop(A);
//                g.addEdge(C,A);
//        g.addEdge(C,D);
//        g.addEdge(D,E);
//        g.addArc(G,F);g.addArc(F,I);

//        g.addArc(E,F);g.addArc(D,F);
//        g.addArc(H,J);g.addArc(I,J);
//        g.addArc(F,L);g.addArc(J,K);
//        g.del(A);g.del(L);g.del(K);g.del(G);g.del(H);g.del(I);g.del(J);
        Graph<char,int> g1;
        g1.makeAdjMatrix();
        g1=g;
        writeGraphText(g, cout, RG_VertexLists|RG_VInfo);
        cout << "\n*********\n";
        RelDiagram::symmClosure(g);
        writeGraphText(g, cout, RG_VertexLists|RG_VInfo);
//        g.ch2Undir();
        cout << "\n:::::::::::::::\n";
       RelDiagram::symmClosure(g1);
        writeGraphText(g1, cout, RG_VertexLists|RG_VInfo);

        cout << boolalpha << endl << g.hasAdjMatrix() << g1.hasAdjMatrix()<< endl << endl;

        Graph<string,string> g2;
        g2.copy(g1);
               writeGraphText(g2, cout, RG_VertexLists|RG_VInfo);

//        cout << boolalpha<< BFS::getAttainableSet(g,A).isElement(A);
////
////        g.del(D);
//        int res;
//        cout << (res=IsIt::Comparability::maxStable(g,tabV))<<endl;
//        for(int i=0;i<res ;i++) cout << tabV[i]->info << ' ';

    }

	return 0;
	};


//       Graph<char> g;
//        Graph<char>::PVertex A,B,C,D,E,F,G,H,I,J,K,L;
//        A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');E=g.addVert('E');
//        g.addArc(A,B);
//        g.addArc(B,C);
//        g.addArc(C,E);
//        RelDiagram::transClosure(g);
//        g.ch2Undir();
//        cout << IsIt::comparability(g);
