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
	Graph<int, int> g,g2;
	Graph<int, int>::PVertex v,u,w,y,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,tabV[100],tabV2[100];
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
//    u10=g.addVert(10);
//


//    g.addEdge(u1,u2);
//    g.addEdge(u1,u3);
//    g.addEdge(u2,u3);
//    g.addEdge(u1,u4);
//    g.addEdge(u4,u3);
//    g.addEdge(u2,u4);
//    g.addEdge(u1,u5);

//xxx
        g.addEdge(u1,u4);
        g.addEdge(u1,u6);
        g.addEdge(u3,u4);
        g.addEdge(u5,u4);
        g.addEdge(u4,u6);
        g.addEdge(u5,u6);
        g.addEdge(u2,u7);g.addEdge(u7,u3);
        g.addEdge(u1,u8);
        g.addEdge(u2,u3);


//        g.addEdge(u2,u1);
//        g.addEdge(u8,u9);
//        g.addEdge(u1,u9);

//    g.clearEdges();
//    g.addEdge(u4,u6);g.addEdge(u1,u6);g.addEdge(u2,u6);g.addEdge(u6,u3);
//    g.addEdge(u5,u6);g.addEdge(u7,u6);g.addEdge(u9,u6);g.addEdge(u6,u8);


    cout << IsIt::chordal(g) << "\nOrder: ";
    IsIt::Chordal::getOrder(g,tabV);
    for(int i=0;i<g.getVertNo();i++) cout << tabV[i]->info << ' ';

    int q =IsIt::Chordal::maxClique(g,tabV);
    cout << endl<< endl << "maxCliqueSize:" << q << endl;
    for(int i=0;i<q;i++) cout << tabV[i]->info << ' ';



	Graph<int, int>::PVertex cq[128], cq2[128];
//	int ncq;
//	if (IsIt::chordal(g))
	{
        int cl=IsIt::Chordal::maxCliques(g,IsIt::compStore(comptab,tabV),tabpar);
        cout << endl << "maxCliquesTreeSize: " << cl << endl;
        for(int i=0;i<cl;i++)
        {   cout << i << ":";
            for(int j=comptab[i];j<comptab[i+1];j++) cout << tabV[j]->info;
          cout  << ' ';
        }
        cout << endl;
        for(int i=0;i<cl-1;i++) cout << tabpar[i].first << tabpar[i].second << ' ';
        int res;

        Graph<int, int>::PVertex u10=g.addVert(10);
        Graph<int, int>::PVertex u11=g.addVert(11);
        Graph<int, int>::PVertex u12=g.addVert(12);
        Graph<int, int>::PVertex u13=g.addVert(13);
//
        g.addEdge(u10,u11);
        g.addEdge(u12,u13);
        g.addEdge(u11,u12); g.addEdge(u11,u13);
      g.addEdge(u10,u12);
        g.addEdge(u10,u13);


        cout << endl << "maxStable: ";
        cout << endl << endl << (res=IsIt::Chordal::maxStable(g,tabV2))<< endl;
        for(int i=0;i<res;i++) cout << tabV2[i]->info << ' ';

	}

//    return 0;

//    cout <<IsIt::caterpillar(g) << " " <<
//        (IsIt::Caterpillar::getSpineEnds(g).first ? IsIt::Caterpillar::getSpineEnds(g).first->info : -1) << ' ' <<
//        (IsIt::Caterpillar::getSpineEnds(g).second ? IsIt::Caterpillar::getSpineEnds(g).second->info : -1);

//    cout << IsIt::compKPartite(g) << ' ' << (k=IsIt::CompKPartite::getParts(g,comptab,tabV)) <<"\n";



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

    {
        cout << "\n............\n";
        Graph<char, char> g;
        Graph<char, char>::PVertex u,v,tabV[20];
        int tabi[20];
        Koala::AssocArray<Koala::Graph<char,char>::PVertex,int> mapa;


        g.addVert('A');
        g.addVert('B');
        g.addVert('C');
        g.addVert('A');
        g.addVert('A');
        g.addVert('B');
        g.addVert('C');

        for(u=g.getVert();u!=g.getVertLast();u=g.getVertNext(u))
            for(v=g.getVertNext(u);v;v=g.getVertNext(v)) if (v->info!=u->info) g.addEdge(u,v);

        int k=IsIt::CompKPartite::split(g,mapa, SearchStructs::compStore(tabi,tabV));
        IsIt::CompKPartite::split(g,mapa);
        cout << endl;
        for(int i=0;i<k;i++)
        { for(int j=tabi[i];j<tabi[i+1];j++) cout << tabV[j]->info;
          cout  << ' ';
        }
        cout << endl;
        for(u=g.getVert();u;u=g.getVertNext(u)) cout << u->info << ' '<< mapa[u] << endl;

    }
     {
        cout << "\n............\n";
        Graph<char, char> g;
        Graph<char, char>::PVertex u,v,w;



        u=g.addVert('A');
        v=g.addVert('B');
        w=g.addVert('C');
        g.addEdge(u,v);
        g.addEdge(u,w);
        cout << u << ' '<< v<<'\n';
        cout << IsIt::Caterpillar::spineEnds(g).first << ' ' <<IsIt::Caterpillar::spineEnds(g).second;
        g.clear();
        cout << '\n' << IsIt::tree(g) << "\n\n";
        u=g.addVert('A');
        v=g.addVert('B');
        w=g.addVert('C');
        g.addEdge(u,v);
        g.addEdge(u,w);
//        g.addEdge(v,w);
//        g.addEdge(v,w);
        g.addLoop(v);
        g.addLoop(v);
        cout << '\n' << IsIt::almostTree(g,false) << "\n\n";

        int tab[]={1,3,-1,3,1};

        GrDefaultSettings<EdAll,true>::sort(tab,tab+5,std::less<int>());
        for(int i=0;i<5;i++) cout << tab[i]<<' ';

        Graph<char, char>::PVertex tabvv[]={u,v,w,v,u};
        Graph<char, char>::PEdge tabee[]={g.getEdge(),g.getEdgeLast(),g.getEdge()};
        for (int i=0;i<5;i++) cout << tabvv[i] <<' ';
        cout << "\n";
        Koala::Privates::GraphRepsDeleter<Graph<char, char>::PVertex, GrDefaultSettings<EdAll,true > > reps(tabvv,tabvv+5);
        for (int i=0;i<reps.len;i++) cout << reps.buf[i] <<' ';
        g.rev2(tabee,tabee+3);
       g.findParals2(std::make_pair(blackHole,blackHole),tabee,tabee+3);
        g.delEdges2(tabee,tabee+2);
        g.delVerts2(tabvv,tabvv+2);

     }

{
        cout << "\n............\n";
        Graph<char, char> g;
        Graph<char, char>::PVertex u,v,w,x,tabv[10];



        u=g.addVert('A');
        v=g.addVert('B');
        w=g.addVert('C');
        x=g.addVert('D');
        g.addEdge(u,v);
        g.addEdge(u,w);
        g.addEdge(u,x);
        g.addEdge(u,v);
        g.addEdge(u,w);
        g.addLoop(u);

        int res=g.getClNeighs(tabv,u);
        cout  << res;
        for(int i=0;i<res;i++) cout << tabv[i]->info << ' ';


     }


}
