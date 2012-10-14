#include<stdio.h>
#include<stdlib.h>
#include<iostream>

#include<utility>
#include<iostream>
#include<map>

#include"Koala/graph/graph.h"
#include "Koala/io/text.h"
#include "Koala/classes/detect.h"
#include "Koala/classes/create.h"


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
	Graph<int, int>::PVertex v;

	readGraphText(g,
#if defined(TEST_1)
		"4\n"
		"0 3 -1 -2 -3\n"
		"1 0\n"
		"2 1 -3n"
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
		q = IsIt::Comparability::color(g, colors);
		printf("COLORING: ");
		for(ci = colors.cont.begin(); ci != colors.cont.end(); ++ci) {
			printf("%p: %d  ", ci->first, ci->second);
			};
		printf("\n");
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
    cout << "\n" << boolalpha << IsIt::CompMPartite::split(g,out.input()) << "\n" << IsIt::compMPartite(g);
    cout << "\n" << out.size() << ' ' << out.size(0);
    cout  << ' ' << out.size(1);

    cout <<"\n---------------------------\n";
    {
        Graph<char> g;
        Graph<char>::PVertex A,B,C,D,E,F,G,H,I,J,K,L,tabV[20];

        A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
        E=g.addVert('E');
        F=g.addVert('F');G=g.addVert('G');H=g.addVert('H');I=g.addVert('I');
        g.addArc(A,B);
        g.addArc(B,C);g.addArc(D,F);
        g.addArc(B,E);
        g.addArc(G,F);g.addArc(F,I);

//        g.addArc(E,F);g.addArc(D,F);
//        g.addArc(H,J);g.addArc(I,J);
//        g.addArc(F,L);g.addArc(J,K);
//        g.del(A);g.del(L);g.del(K);g.del(G);g.del(H);g.del(I);g.del(J);
        RelDiagram::transClousure(g);
        g.ch2Undir();
        writeGraphText(g, cout, RG_VertexLists|RG_VInfo);
//
//        g.del(D);
        int res;
        cout << (res=IsIt::Comparability::maxStable(g,tabV))<<endl;
        for(int i=0;i<res ;i++) cout << tabV[i]->info << ' ';

    }

	return 0;
	};


//       Graph<char> g;
//        Graph<char>::PVertex A,B,C,D,E,F,G,H,I,J,K,L;
//        A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');E=g.addVert('E');
//        g.addArc(A,B);
//        g.addArc(B,C);
//        g.addArc(C,E);
//        RelDiagram::transClousure(g);
//        g.ch2Undir();
//        cout << IsIt::comparability(g);
