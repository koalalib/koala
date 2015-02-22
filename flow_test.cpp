#include <iostream>
#include <string>
#include <cstdio>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/conflow.h"
#include "Koala/io/text.h"

using namespace std;
using namespace Koala;

struct OpisE {
    string name;
    int cap;

    OpisE(int arg=0): cap (arg), name("") {}
};

std::ostream& operator<<(std::ostream& os,OpisE arg) { os << arg.name; return os; }

Koala::Graph<char,OpisE> g;
Koala::Graph<char,OpisE>::PVertex A,B,C,D,E,F,V,U,S,T,tabV[20],*tabVit;
Koala::Graph<char,OpisE>::PEdge tabE[20],ee;

typedef int CapType;
typedef int CostType;

Koala::AssocTable<Koala::BiDiHashMap<Koala::Graph<char,OpisE>::PEdge,Koala::Flow::EdgeLabs<CapType,CostType> > >edgeCont;
Koala::AssocTable<Koala::BiDiHashMap<Koala::Graph<char,OpisE>::PEdge,Koala::Flow::UnitEdgeLabs<int> > > unitedgeCont;

Koala::AssocTable<Koala::BiDiHashMap<Koala::Graph<char,OpisE>::PEdge,Koala::Flow::TrsEdgeLabs<CapType,CostType> > > tedgeCont;
Koala::AssocTable<Koala::BiDiHashMap<Koala::Graph<char,OpisE>::PVertex,Koala::Flow::TrsVertLoss<CapType> > > tvertCont;
Koala::AssocTable<Koala::BiDiHashMap<Koala::Graph<char,OpisE>::PVertex,Koala::Flow::TrsEdgeLabs<CapType,CostType> > > t2vertCont;


template <class T>
void printInfo(const SearchStructs::CompStoreTool<T>& cont)
{
    cout << "\nsize: " << cont.size() << "\tlen:" <<cont.length();;
    for(int i=0;i<cont.size();i++)
    {
        cout << "\n"<<cont.size(i) << ":";
        for(int j=0;j<cont.size(i);j++) cout << ' ' << cont[i][j]->info;
    }
}

template <class T>
void printInfoName(const SearchStructs::CompStoreTool<T>& cont)
{
    cout << "\nsize: " << cont.size() << "\tlen:" <<cont.length();;
    for(int i=0;i<cont.size();i++)
    {
        cout << "\n"<<cont.size(i) << ":";
        for(int j=0;j<cont.size(i);j++) cout << ' ' << cont[i][j]->info.name;
    }
}

void dijTest()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    g.addArc(S,A,OpisE(20));g.addArc(S,B,OpisE(50));g.addArc(B,A,OpisE(10));
    g.addArc(A,D,OpisE(50));g.addArc(A,C,OpisE(60));g.addArc(B,C,OpisE(30));
    g.addArc(C,D,OpisE(10));g.addArc(C,T,OpisE(20));
    g.addArc(D,T,OpisE(40));g.addArc(D,E,OpisE(80));g.addArc(E,T,OpisE(40));
//    g.addArc(F,S,OpisE(40));
//    g.addArc(T,S,OpisE(100));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}

void dijTest2()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addArc(S,A,OpisE(2));g.addArc(S,B,OpisE(2));g.addArc(S,C,OpisE(2));
    g.addArc(A,D,OpisE(5));g.addArc(A,E,OpisE(1));g.addArc(B,D,OpisE(1));
    g.addArc(B,F,OpisE(1));g.addArc(C,E,OpisE(1));
    g.addArc(B,E,OpisE(3));g.addArc(D,T,OpisE(3));g.addArc(E,T,OpisE(2));
    g.addArc(F,T,OpisE(4));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void dijTest3()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addEdge(S,A,OpisE(20));g.addEdge(S,B,OpisE(50));
    g.addEdge(B,A,OpisE(10));
    g.addEdge(D,A,OpisE(50));g.addEdge(A,C,OpisE(60));g.addEdge(B,C,OpisE(30));
    g.addEdge(C,D,OpisE(10));g.addEdge(C,T,OpisE(20));
    g.addEdge(D,T,OpisE(40));g.addEdge(D,E,OpisE(80));g.addEdge(E,T,OpisE(40));

//    g.addArc(S,T,OpisE(40));g.addArc(S,T,OpisE(40));g.addArc(T,S,OpisE(40));g.addEdge(T,S,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArc(D,F,OpisE(40));g.addArc(S,C,OpisE(40));g.addArc(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void dijTest4()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addArc(S,A,OpisE(1));g.addArc(S,B,OpisE(1));g.addEdge(A,B,OpisE(1));
    g.addArc(A,D,OpisE(1));g.addArc(A,C,OpisE(1));g.addArc(B,C,OpisE(1));
    g.addArc(C,D,OpisE(1));g.addArc(C,T,OpisE(1));
    g.addArc(D,T,OpisE(1));g.addArc(D,E,OpisE(1));g.addArc(E,T,OpisE(1));
    g.addArc(F,S,OpisE(1));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}

void tdijTest()
{   g.clear();tedgeCont.clear();tvertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    tvertCont[S].lo=-170;tvertCont[S].hi=-160;
    tvertCont[T].lo=130;tvertCont[T].hi=170;

    g.addArc(S,A,OpisE(20));g.addArc(S,B,OpisE(50));g.addArc(B,A,OpisE(10));
    g.addArc(A,D,OpisE(50));g.addArc(A,C,OpisE(60));g.addArc(B,C,OpisE(30));
    g.addArc(C,D,OpisE(10));g.addArc(C,T,OpisE(20));
    g.addArc(D,T,OpisE(40));g.addArc(D,E,OpisE(80));g.addArc(E,T,OpisE(40));
    g.addArc(F,S,OpisE(40));
    g.addEdge(T,S,OpisE(100));
    g.addLoop(A,OpisE(30));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            tedgeCont[ePt].hi=ePt->info.cap; tedgeCont[ePt].lo=0;
            ePt->info.name=buf;
        }
    tedgeCont[g.getEdge(EdLoop)].lo=20;
}

void tdijTest2()
{   g.clear();tedgeCont.clear();tvertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    tvertCont[S].lo=-170;tvertCont[S].hi=-160;
    tvertCont[T].lo=130;tvertCont[T].hi=170;

    g.addEdge(A,S,OpisE(20));g.addEdge(S,B,OpisE(50));g.addEdge(B,A,OpisE(10));
    g.addEdge(A,D,OpisE(50));g.addEdge(A,C,OpisE(60));g.addEdge(B,C,OpisE(30));
    g.addEdge(C,D,OpisE(10));g.addEdge(C,T,OpisE(20));
    g.addEdge(T,D,OpisE(40));g.addEdge(D,E,OpisE(80));g.addEdge(T,E,OpisE(40));
    g.addEdge(F,S,OpisE(40));
    g.addEdge(T,S,OpisE(100));
    g.addLoop(A,OpisE(30));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            tedgeCont[ePt].hi=ePt->info.cap; tedgeCont[ePt].lo=0;
            ePt->info.name=buf;
        }
    tedgeCont[g.getEdge(EdLoop)].lo=20;
}


void tdijTest3()
{   g.clear();tedgeCont.clear();tvertCont.clear();

//ttt
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');

    tvertCont[A].lo=-100;tvertCont[A].hi=0;
    tvertCont[B].lo=0;tvertCont[B].hi=100;
    tvertCont[C].lo=tvertCont[D].lo=0;tvertCont[C].hi=tvertCont[D].hi=1;

    t2vertCont[A].lo=0;t2vertCont[A].hi=1000; t2vertCont[A].cost=1;
    t2vertCont[B].lo=0;t2vertCont[B].hi=1000; t2vertCont[B].cost=1;
    t2vertCont[C].lo=39;t2vertCont[C].hi=40;t2vertCont[D].lo=39;t2vertCont[D].hi=40;t2vertCont[C].cost=1;t2vertCont[D].cost=1;


    tedgeCont[ee=g.addArc(A,C,OpisE(20))].lo=0;tedgeCont[ee].hi=30;tedgeCont[ee].cost=1;
    tedgeCont[ee=g.addArc(C,B,OpisE(50))].lo=20;tedgeCont[ee].hi=30;tedgeCont[ee].cost=1;
    tedgeCont[ee=g.addArc(A,D,OpisE(20))].lo=20;tedgeCont[ee].hi=30;tedgeCont[ee].cost=1;
    tedgeCont[ee=g.addArc(D,B,OpisE(50))].lo=20;tedgeCont[ee].hi=30;tedgeCont[ee].cost=1;
    tedgeCont[ee=g.addLoop(C,OpisE(10))].lo=0;tedgeCont[ee].hi=10;tedgeCont[ee].cost=1;
    tedgeCont[ee=g.addLoop(D,OpisE(10))].lo=0;tedgeCont[ee].hi=20;tedgeCont[ee].cost=1;

    bool flag;
    cout << "\n\n**********\n\n" << boolalpha << (flag=Koala::Flow::transship(g,tedgeCont,tvertCont, t2vertCont)) ;

    if (flag){
        for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
            {   char buf[20];
                cout << endl << ePt->getEnds().first->info << ePt->getEnds().second->info << ":" <<tedgeCont[ePt].flow;
                tedgeCont[ePt].flow=0;
            }
        for(Koala::Graph<char,OpisE>::PVertex ePt=g.getVert();ePt;ePt=g.getVertNext(ePt))
            {   char buf[20];
                cout << endl << ePt->info << ":" <<t2vertCont[ePt].flow;
                t2vertCont[ePt].flow=0;
            }
    } else
    {
        for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
                tedgeCont[ePt].flow=0;
        for(Koala::Graph<char,OpisE>::PVertex ePt=g.getVert();ePt;ePt=g.getVertNext(ePt))
                t2vertCont[ePt].flow=0;
    }
    cout << "\n\n**********\n\n" << Koala::Flow::minCostTransship(g,tedgeCont,tvertCont, t2vertCont) <<endl;
       for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
            {   char buf[20];
                cout << endl << ePt->getEnds().first->info << ePt->getEnds().second->info << ":" <<tedgeCont[ePt].flow;
                tedgeCont[ePt].flow=0;
            }
        for(Koala::Graph<char,OpisE>::PVertex ePt=g.getVert();ePt;ePt=g.getVertNext(ePt))
            {   char buf[20];
                cout << endl << ePt->info << ":" <<t2vertCont[ePt].flow;
                t2vertCont[ePt].flow=0;
            }
}


void conTest()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addArc(S,A,OpisE(20));g.addArc(S,B,OpisE(50));g.addEdge(A,B,OpisE(10));
    g.addArc(A,D,OpisE(50));g.addArc(A,C,OpisE(60));g.addArc(B,C,OpisE(30));
    g.addArc(C,D,OpisE(10));g.addArc(C,T,OpisE(20));
    g.addArc(D,T,OpisE(40));g.addArc(D,E,OpisE(80));g.addArc(E,T,OpisE(40));
    g.addArc(F,S,OpisE(40));


    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


#include "main.hpp"

    g.makeAdjMatrix();
    dijTest();
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);
    cout << "\n***=\n";
//    cout << Koala::Flow::usedCap(g,edgeCont,g.getEdge(B,C),B,true) << '\n';



//    edgeCont[g.getEdge(A,D)].flow=40;edgeCont[g.getEdge(A,C)].flow=50;

    cout << "\n***\n";
    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20]; sprintf(buf," %d",edgeCont[ePt].flow); ePt->info.name.append(buf); }

    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

    cout << "\n***\n" << boolalpha;
    cout << Koala::Flow::maxFlow(g,edgeCont,S,T,12) << '=';
    cout << Koala::Flow::vertFlow(g,edgeCont,S) << '\n';
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\n\n-----------\n\n";

    cout << Koala::Flow::maxFlow(g,edgeCont,S,T,59) << '=';
    cout << Koala::Flow::vertFlow(g,edgeCont,S) << '\n';
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\n\n-----------\n\n";

    cout << Koala::Flow::maxFlow(g,edgeCont,S,T,59) << '=';
    cout << Koala::Flow::vertFlow(g,edgeCont,S) << '\n';
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\n\n-----------\n\n";

    cout << Koala::Flow::maxFlow(g,edgeCont,S,T) << '=';
    cout << Koala::Flow::vertFlow(g,edgeCont,S) << '\n';
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\n\n-----------\n\n";

//    return 0;
    dijTest3();
        for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCont[e].capac=1;
    Koala::Flow::EdgeCut<CapType> res=Koala::Flow::minEdgeCut(g,edgeCont,S,T,Koala::Flow::outCut(tabV,tabE));
    Koala::Flow::minEdgeCut(g,unitedgeCont,S,T,Koala::Flow::outCut(blackHole,blackHole));
    cout << res.capac << endl;
    for(int i=0;i<res.vertNo;i++) cout << tabV[i]->info;
    cout << endl;
    for(int i=0;i<res.edgeNo;i++) cout << tabE[i]->info.name << ' ';

    cout << "\n\n////\n\n";
    dijTest3();

    Koala::Flow::EdgeCut2<Koala::Graph<char,OpisE>, CapType> res2
        =Koala::Flow::minEdgeCut(g,edgeCont,Koala::Flow::outCut(tabV,tabE));
    cout << res2.capac << ' ' << res2.first->info << res2.second->info << endl;
    for(int i=0;i<res2.vertNo;i++) if (tabV[i]) cout << tabV[i]->info;
    cout << endl;
    for(int i=0;i<res2.edgeNo;i++) if (tabE[i]) cout << tabE[i]->info.name << ' ';


    cout << "\n\n(((((((((((\n\n";
    dijTest4();
    res2
        =Koala::Flow::minEdgeCut(g,edgeCont,Koala::Flow::outCut(tabV,tabE));
    cout << res2.capac << ' ' << res2.first->info << res2.second->info << endl;
    for(int i=0;i<res2.vertNo;i++) if (tabV[i]) cout << tabV[i]->info;
    cout << endl;
    for(int i=0;i<res2.edgeNo;i++) if (tabE[i]) cout << tabE[i]->info.name << ' ';
    cout << "\n\ntttt\n\n";

    tdijTest();
    cout << Koala::Flow::transship(g,tedgeCont,tvertCont) << '\n';
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << tedgeCont[e].flow << ' ' <<tedgeCont[e].hi<<'\n';
    cout << "S " << Koala::Flow::vertFlow(g,tedgeCont,S) << "T " << Koala::Flow::vertFlow(g,tedgeCont,T);

    cout << "\n\ncon con con con\n\n";
//    tedgeCont[g.getEdge()].flow+=100;
    cout <<  Koala::Flow::testTransship(g,tedgeCont,tvertCont) << endl;

    conTest();
    int ires=Koala::Connect::minEdgeCut(g,A,B,tabE);
    cout << ires << endl;
    for(int i=0;i<ires;i++) cout << tabE[i]->info.name << ' ';

    cout << "\n\n----\n\n";
    conTest();
    Koala::Connect::EdgeCut<Koala::Graph<char,OpisE> > cres
        =Koala::Connect::minEdgeCut(g,tabE);
    cout << cres.edgeNo << ' ' << cres.first->info << cres.second->info << endl;
    for(int i=0;i<cres.edgeNo;i++) if (tabE[i]) cout << tabE[i]->info.name << ' ';

    cout << "\n\n&&&&&&&&&&&&&&&&\n\n";
    dijTest3();
    bool vflag=false;
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);
    int itab[20];


    SearchStructs::CompStoreTool<Koala::Graph<char,OpisE>::PVertex> vstore;
    SearchStructs::CompStoreTool<Koala::Graph<char,OpisE>::PEdge> estore;
    int p=Koala::Connect::vertDisjPaths(g,S,T,vstore.input(),estore.input());
    Koala::Connect::edgeDisjPaths(g,S,T,blackHole,estore.input());
//                : Koala::Connect::vertDisjPaths(g,S,T,Koala::Connect::outPath(tabV,tabE),
//                                    std::make_pair(blackHole,itab));


    cout << p << endl;
    printInfo(vstore);
    cout <<  endl;
    printInfoName(estore);
//    for(int i=0;i<=p;i++) cout << itab[i] << ' ';
//    cout <<  endl;
//    if (!vflag)
//    for(int i=0;i<p;i++)
//    {   for(int j=itab[i];j<itab[i+1];j++) cout << tabE[j]->getEnds().first->info
//                            << tabE[j]->getEnds().second->info << ' ';
//        cout << endl;
//    } else for(int i=0;i<p;i++)
//    {   for(int j=itab[i];j<itab[i+1];j++) cout << tabV[j]->info << ' ';
//        cout << endl;
//    }
    cout << endl;
//    for(int i=0;i<20;i++) if (tabV[i]) cout << tabV[i]->info;
//    cout << endl;
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

    cout << "\n\n^^^^^^^^^^^^^^\n\n";
    dijTest3();
//    cout << (p=Koala::Connect::minVertCut(g,S,F,blackHole)) << endl;
        cout << (p=Koala::Connect::minVertCut(g,S,T,tabV)) << endl;
    for(int i=0;i<p;i++) cout << tabV[i]->info << ' ';

    cout << "\n\n###########\n\n";
    dijTest3();
//    cout << (p=Koala::Connect::minVertCut(g,S,F,blackHole)) << endl;
        cout << (p=Koala::Connect::minVertCut(g,tabV)) << endl;
    for(int i=0;i<p;i++) cout << tabV[i]->info << ' ';

    tdijTest3();

    return 0;
}
