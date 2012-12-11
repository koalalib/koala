#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/conflow.h"
#include "Koala/classes/create.h"
#include "Koala/io/text.h"

using namespace std;
using namespace Koala;

const int N=15;

typedef FlowPar< FlowAlgsDefaultSettings< false,true > >  MyFlow;
typedef FlowPar< FlowAlgsDefaultSettings< false,false > >  MyFlow2;

Koala::Graph<int,Koala::IO::ParSet> g;
Koala::Graph<int,Koala::IO::ParSet>::PVertex A,B,C,D,E,F,V,U,S,T,tabV[N+1];
Koala::Graph<int,Koala::IO::ParSet>::PEdge tabE[10*N*(N-1)];

AssocArray<Koala::Graph<int,Koala::IO::ParSet>::PEdge,MyFlow::EdgeLabs<int> > edgeCont;
AssocArray<Koala::Graph<int,Koala::IO::ParSet>::PEdge,int > edgeCap;
AssocArray<Koala::Graph<int,Koala::IO::ParSet>::PEdge,int > edgeCost;

int mod(int a)
{
    return (a>0) ? a : -a;
}

void copydata()
{
    edgeCont.clear();
    for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
    {
        edgeCont[e].capac=edgeCap[e];
        edgeCont[e].cost=edgeCost[e];
        e->info.set("cap",edgeCont[e].capac);
        e->info.set("cost",edgeCont[e].cost);
    }
}

void test0()
{
    for(int k=0;k<5000;k++)
    {   const double P=0.3;

        g.clear();
        for(int i=0;i<N;i++) g.addVert(i);
        assert(g.getVertNo()==N);

        for(int i=0;i<N-1;i++) for(int j=i+1;j<N;j++)
            if (((double)rand() / (double)RAND_MAX)<P) g.addEdge(g.vertByNo(i),g.vertByNo(j));
        for(int i=0;i<N-1;i++) for(int j=i+1;j<N;j++)
            if (((double)rand() / (double)RAND_MAX)<P) g.addEdge(g.vertByNo(i),g.vertByNo(j));
        for(int i=0;i<N;i++) for(int j=1;j<N;j++) if (i!=j)
            if (((double)rand() / (double)RAND_MAX)<P) g.addArc(g.vertByNo(i),g.vertByNo(j));
        for(int i=0;i<N-1;i++) for(int j=1;j<N;j++) if (i!=j)
            if (((double)rand() / (double)RAND_MAX)<P)
             if (!g.getEdgeNo(g.vertByNo(i),g.vertByNo(j)))
                g.addArc(g.vertByNo(i),g.vertByNo(j));

//        IO::writeGraphText(g, cout, IO::RG_VertexLists );
        Graph<int,Koala::IO::ParSet>::PVertex u=g.vertByNo(rand()%N),v;
        do { v=g.vertByNo(rand()%N); } while (v==u);

        cout << boolalpha;
        if (!BFS::getAttainableSet(g,u).isElement(v))
        {
//          cout << "---\n";
          continue;
        }
        for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCap[e]=1+(rand()%10);
        for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCost[e]=3+(rand()%20);


        copydata();
        int fres=MyFlow::maxFlow(g,edgeCont,u,v);
        MyFlow::EdgeCut<int> cres=MyFlow::minEdgeCut(g,edgeCont,u,v,MyFlow::outCut(blackHole,tabE));
        int cres2=0;
        Set<Graph<int,Koala::IO::ParSet>::PEdge> eset;
        eset.clear();
        for(int i=0;i<cres.edgeNo;i++)
        {
            cres2+=edgeCont[tabE[i]].capac;
            eset+=tabE[i];
        }
        eset=g.getEdgeSet()-eset;
        assert((fres==cres2)&& (fres==cres.capac)&&!BFS::getAttainableSet(makeSubgraph(g,stdChoose(true)&stdChoose(eset)),u).isElement(v));

         cout<< k << ' ';
    }
}


void test1()
{
    for(int k=0;k<50000;k++)
    {   const double P=0.2;

        g.clear();
        for(int i=0;i<N;i++) g.addVert(i);
        assert(g.getVertNo()==N);
        assert(N==g.getVertNo());
        for(int i=0;i<N-1;i++) for(int j=i+1;j<N;j++)
            if (((double)rand() / (double)RAND_MAX)<P) g.addEdge(g.vertByNo(i),g.vertByNo(j));
        for(int i=0;i<N-1;i++) for(int j=i+1;j<N;j++)
            if (((double)rand() / (double)RAND_MAX)<P) g.addEdge(g.vertByNo(i),g.vertByNo(j));
        for(int i=0;i<N;i++) for(int j=1;j<N;j++) if (i!=j)
            if (((double)rand() / (double)RAND_MAX)<P) g.addArc(g.vertByNo(i),g.vertByNo(j));
        for(int i=0;i<N-1;i++) for(int j=1;j<N;j++) if (i!=j)
            if (((double)rand() / (double)RAND_MAX)<P)
             if (!g.getEdgeNo(g.vertByNo(i),g.vertByNo(j)))
                g.addArc(g.vertByNo(i),g.vertByNo(j));

//        IO::writeGraphText(g, cout, IO::RG_VertexLists );
        Graph<int,Koala::IO::ParSet>::PVertex u=g.vertByNo(rand()%N),v;
        do { v=g.vertByNo(rand()%N); } while (v==u);

        cout << boolalpha;
        if (!BFS::getAttainableSet(g,u).isElement(v))
        {
//          cout << "---\n";
          continue;
        }
        for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCap[e]=1+(rand()%10);
        for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCost[e]=3+(rand()%20);


        copydata();
        int fres=MyFlow::maxFlow(g,edgeCont,u,v);
        MyFlow::EdgeCut<int> cres=MyFlow::minEdgeCut(g,edgeCont,u,v,MyFlow::outCut(blackHole,tabE));
        int cres2=0;
        Set<Graph<int,Koala::IO::ParSet>::PEdge> eset;
        eset.clear();
        for(int i=0;i<cres.edgeNo;i++)
        {
            cres2+=edgeCont[tabE[i]].capac;
            eset+=tabE[i];
        }
        eset=g.getEdgeSet()-eset;
        assert((fres==cres2)&& (fres==cres.capac)&&!BFS::getAttainableSet(makeSubgraph(g,stdChoose(true)&stdChoose(eset)),u).isElement(v));

        copydata();
        std::pair<int,int> cfres=MyFlow::minCostFlow(g,edgeCont,u,v);
        assert(cfres.second==fres);
        int cost=0;
        for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) cost+=mod(edgeCont[e].flow)*edgeCont[e].cost;
        assert(cfres.first==cost);

        copydata();
        std::pair<int,int> cfres2=MyFlow2::minCostFlow(g,edgeCont,u,v);
        cost=0;
        for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) cost+=mod(edgeCont[e].flow)*edgeCont[e].cost;
//        assert(cfres.first==cost);
        assert(MyFlow::flowCost(g,edgeCont)==cfres2.first);
//        assert(cfres.first==cfres2.first);
//        assert(cfres.second==cfres2.second);

        cout << g.getVertNo() << ' ' << fres << ' ' << cfres.second << ' '<< cfres.first<<' ' << cfres2.second << ' '<< cfres2.first
            << ' ' << (g.getEdgeNo(u,v));
//        assert(
        if (!(
                 cfres.first==cfres2.first
            ))
        {
            cout << " Blad_1!!!\n";
            cout << g.getVertNo() << " start: " << u->info << " end: " << v->info << endl;
            IO::writeGraphText(g, cout, IO::RG_VertexLists | IO::RG_EInfo);
            return ;
        }
//               );
        if (!(
                 cfres.second==cfres2.second
            )) cout << " Blad_2!!!";


         cout<< endl;
    }
}

void test2()
{
    g.clear(); edgeCont.clear();
    readGraphText(g,
    "8\n"
    "0 2 >3(cap:10,cost:11) >5(cap:8,cost:6)\n"
    "1 1 >4(cap:6,cost:7)\n"
    "2 2 >4(cap:2,cost:11) >7(cap:10,cost:12)\n"
    "3 3 >1(cap:9,cost:8) >2(cap:10,cost:8) >5(cap:1,cost:6)\n"
    "4 0\n"
    "5 2 >1(cap:10,cost:11) >6(cap:9,cost:15)\n"
    "6 3 >1(cap:1,cost:15) >3(cap:8,cost:8) >4(cap:8,cost:19)\n"
    "7 0\n"
    ,IO::RG_VertexLists | IO::RG_EInfo);
    Koala::Graph<int,Koala::IO::ParSet>::PVertex u=g.vertByNo(0), v=g.vertByNo(4);
    for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
    {
        edgeCont[e].capac=e->info.getInt("cap");
        edgeCont[e].cost=e->info.getInt("cost");
    }
    cout << endl;
    IO::writeGraphText(g, cout, IO::RG_VertexLists | IO::RG_EInfo);
    std::pair<int,int> cfres=MyFlow::minCostFlow(g,edgeCont,u,v);
    assert(MyFlow::testFlow(g,edgeCont,u,v) && MyFlow::vertFlow(g,edgeCont,u)==cfres.second);
    for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) e->info.set("flow",edgeCont[e].flow);


    edgeCont.clear();
    for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
    {
        edgeCont[e].capac=e->info.getInt("cap");
        edgeCont[e].cost=e->info.getInt("cost");
    }
    IO::writeGraphText(g, cout, IO::RG_VertexLists | IO::RG_EInfo);
    std::pair<int,int> cfres2=MyFlow2::minCostFlow(g,edgeCont,u,v);
    assert(MyFlow::testFlow(g,edgeCont,u,v) && MyFlow::vertFlow(g,edgeCont,u)==cfres2.second);
    cout << endl;
    for(Graph<int,Koala::IO::ParSet>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) e->info.set("flow",edgeCont[e].flow);
    IO::writeGraphText(g, cout, IO::RG_VertexLists | IO::RG_EInfo);
    cout << cfres.first << ' ' << cfres.second << endl;
    cout << cfres2.first << ' ' << cfres2.second << endl;

}

#include "main.hpp"

    test1();

    return 0;
}
