#include<stdio.h>
#include<utility>
#include<iostream>

#include <iostream>
#include <iomanip>
#include "Koala/graph/graph.h"
#include "Koala/io/text.h"
#include "Koala/classes/detect.h"


using namespace Koala;
using namespace Koala::IO;
using namespace std;


#include "main.hpp"

	bool rv;
	Graph<int, int> g;
	Graph<int, int>::PVertex v,u,w,y,u1,u2,u3,u4,u5,u6,u7,u8,u9,tabV[100],tabV2[100];
	AssocArray<Koala::Graph<int,int>::PEdge,int> edgeCont;
	Koala::AssocArray<Koala::Graph<int,int>::PVertex,
            Koala::SearchStructs::VisitVertLabs<Koala::Graph<int,int> > > vertCont;
	int comptab[10]={-1,-1,-1,-1,-1,-1};
	pair<int,int> tabpar[100];
	int k;

	cout << boolalpha;

    u1=g.addVert(1);
    u2=g.addVert(2);
    u3=g.addVert(3);
    u4=g.addVert(4);
    u5=g.addVert(5);
    u6=g.addVert(6);
    u7=g.addVert(7);
    u8=g.addVert(8);
    u9=g.addVert(9);
//
//

//
//    g.addEdge(u1,u2);g.addEdge(u1,u3);g.addEdge(u2,u3);g.addEdge(u1,u4);//g.addEdge(u8,u2);g.addEdge(u8,u3);
//    g.addEdge(u7,u2);g.addEdge(u7,u3);g.addEdge(u4,u2);g.addEdge(u4,u3);
//    g.addEdge(u7,u1);g.addEdge(u7,u8);g.addEdge(u4,u1);g.addEdge(u4,u8);
//    g.addEdge(u5,u1);g.addEdge(u5,u2);g.addEdge(u5,u3);g.addEdge(u5,u4);g.addEdge(u5,u7);g.addEdge(u5,u8);
//    g.addEdge(u6,u1);g.addEdge(u6,u2);g.addEdge(u6,u3);g.addEdge(u6,u4);g.addEdge(u6,u7);g.addEdge(u6,u8);
//    g.addEdge(u6,u5);

//    g.addEdge(u1,u2);
//    g.addEdge(u3,u4);
//    g.addEdge(u1,u4);
////    g.addEdge(u1,u3);
//    g.addEdge(u4,u2);
//    g.addEdge(u5,u2);
//    g.addEdge(u6,u2);
//    g.addEdge(u4,u5);
//    g.addEdge(u4,u6);
//    g.addEdge(u3,u5);
//   g.addLoop(u1);
//	v=g.addVert();u=g.addVert();w=g.addVert();
//	y=g.addVert();
//	g.addLoop(v); g.addLoop(u);//g.addLoop(u);

//	g.addEdge(v,u);g.addEdge(u,w);//g.addEdge(v,w);
    //g.addEdge(u,y);

//xxx
    g.addEdge(u1,u4);
    g.addEdge(u1,u6);
    g.addEdge(u3,u4);g.addEdge(u5,u4);
    g.addEdge(u4,u6);
    g.addEdge(u5,u6);
    g.addEdge(u2,u7);g.addEdge(u7,u3);
    g.addEdge(u1,u8);
    g.addEdge(u2,u3);


//        g.addEdge(u8,u9);

//g.addEdge(u5,u1);g.addEdge(u5,u2);
//g.addEdge(u4,u3);

//    g.addEdge(u1,u2);g.addEdge(u1,u5);g.addEdge(u2,u3);g.addEdge(u3,u4);g.addEdge(u5,u4);g.addEdge(u1,u3);
//
//	rv = IsIt::comparability(g);
//	printf("RESULT: ");
//	if(rv) printf("true"); else printf("false");
//	printf("\n\n-----------------------\n");


//    g.addEdge(u1,u4);g.addEdge(u1,u6);
//    g.addEdge(u3,u4);g.addEdge(u5,u4);g.addEdge(u4,u6);
//    g.addEdge(u5,u6);

//    g.addEdge(u1,u2);g.addEdge(u1,u3);g.addEdge(u3,u2);
//    g.addEdge(u1,u5);g.addEdge(u5,u3);g.addEdge(u5,u2);
//    g.addEdge(u2,u6);g.addEdge(u6,u3);g.addEdge(u4,u2);
//    g.addEdge(u8,u7);g.addEdge(u7,u9);
//
//    g.addEdge(u8,u9);
//    g.addEdge(u2,u9);g.addEdge(u8,u2);

    cout << IsIt::chordal(g) << "\n";
    IsIt::Chordal::getOrder(g,tabV);
    for(int i=0;i<g.getVertNo();i++) cout << tabV[i]->info;

    int q =IsIt::Chordal::maxClique(g,tabV);
    cout << endl<< endl << q << endl;
    for(int i=0;i<q;i++) cout << tabV[i]->info << ' ';



	Graph<int, int>::PVertex cq[128], cq2[128];
//	int ncq;
//	if (IsIt::chordal(g))
	{
        int cl=IsIt::Chordal::maxCliques(g,IsIt::compStore(comptab,tabV),tabpar);
        cout << endl << cl << endl;
        for(int i=0;i<cl;i++)
        { for(int j=comptab[i];j<comptab[i+1];j++) cout << tabV[j]->info;
          cout  << ' ';
        }
        cout << endl;
        for(int i=0;i<cl-1;i++) cout << tabpar[i].first << tabpar[i].second << ' ';
        int res;
        cout << endl << endl << (res=IsIt::Chordal::maxStable(g,tabV2))<< endl;
        for(int i=0;i<res;i++) cout << tabV2[i]->info << ' ';
	}



//    cout <<IsIt::caterpillar(g) << " " <<
//        (IsIt::Caterpillar::getSpineEnds(g).first ? IsIt::Caterpillar::getSpineEnds(g).first->info : -1) << ' ' <<
//        (IsIt::Caterpillar::getSpineEnds(g).second ? IsIt::Caterpillar::getSpineEnds(g).second->info : -1);

//    cout << IsIt::compMPartite(g) << ' ' << (k=IsIt::CompMPartite::getParts(g,comptab,tabV)) <<"\n";



//    cout << IsIt::block(g)  <<"\n";

//    for(Graph<int, int>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        e->info=g.getEdgeEnds(e).first->info*10+g.getEdgeEnds(e).second->info;
//	cout << (k=Blocks::split(g,blackHole,edgeCont,SearchStructs::compStore(comptab,tabV),blackHole,EdUndir)) << endl;
//	for(int i=0;i<=k;i++) cout << comptab[i] << ' ';
//	cout << endl;
//	for(int i=0;i<k;i++)
//	{ for(int j=comptab[i];j<comptab[i+1];j++) cout << tabV[j]->info;
//	  cout  << ' ';
//	}
//    cout << endl;
//    for(Graph<int, int>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) cout << e->info << ":" << edgeCont[e] << ' ';

	return 0;
}
