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
Koala::Graph<char,OpisE>::PVertex A,B,C,D,E,F,V,U,S,T,G,tabV[20],*tabVit;
Koala::Graph<char,OpisE>::PEdge tabE[20];

Koala::AssocArray<Koala::Graph<char,OpisE>::PEdge,Koala::Flow::EdgeLabs<int> >  edgeCont;
Koala::AssocArray<Koala::Graph<char,OpisE>::PVertex,Koala::Flow::VertLabs<Koala::Graph<char,OpisE>,int> > vertCont;

Koala::AssocArray<Koala::Graph<char,OpisE>::PEdge,Koala::Flow::TrsEdgeLabs<int> > tedgeCont;
Koala::AssocArray<Koala::Graph<char,OpisE>::PVertex,Koala::Flow::TrsVertLoss<int> > tvertCont;


void dijTest()
{   g.clear();edgeCont.clear();
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');G=g.addVert('G');

    g.addArch(S,A,OpisE(20));g.addArch(S,B,OpisE(50));g.addArch(B,A,OpisE(10));
    g.addArch(A,D,OpisE(50));g.addArch(A,C,OpisE(60));g.addArch(B,C,OpisE(30));
    g.addArch(C,D,OpisE(10));g.addArch(C,T,OpisE(20));
    g.addArch(D,T,OpisE(40));g.addArch(D,E,OpisE(80));g.addArch(E,T,OpisE(40));
    g.addArch(F,S,OpisE(40));
    g.addArch(T,G,OpisE(100));


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
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addArch(S,A,OpisE(2));g.addArch(S,B,OpisE(2));g.addArch(S,C,OpisE(2));
    g.addArch(A,D,OpisE(5));g.addArch(A,E,OpisE(1));g.addArch(B,D,OpisE(1));
    g.addArch(B,F,OpisE(1));g.addArch(C,E,OpisE(1));
    g.addArch(B,E,OpisE(3));g.addArch(D,T,OpisE(3));g.addArch(E,T,OpisE(2));
    g.addArch(F,T,OpisE(4));

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
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addEdge(S,A,OpisE(20));g.addEdge(S,B,OpisE(50));g.addEdge(B,A,OpisE(10));
    g.addEdge(D,A,OpisE(50));g.addEdge(A,C,OpisE(60));g.addEdge(B,C,OpisE(30));
    g.addEdge(C,D,OpisE(10));g.addEdge(C,T,OpisE(20));
    g.addEdge(D,T,OpisE(40));g.addEdge(D,E,OpisE(80));g.addEdge(E,T,OpisE(40));

//    g.addArch(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArch(D,F,OpisE(40));g.addArch(S,C,OpisE(40));g.addArch(B,D,OpisE(40));

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


void costTest()
{   g.clear();edgeCont.clear();
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    edgeCont[g.addArch(S,A,OpisE(0))]=Koala::Flow::EdgeLabs<int>(18,3);
    edgeCont[g.addArch(S,C,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,8);
    edgeCont[g.addArch(A,C,OpisE(10))]=Koala::Flow::EdgeLabs<int>(15,4);
    edgeCont[g.addArch(B,A,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,5);
    edgeCont[g.addArch(C,B,OpisE(60))]=Koala::Flow::EdgeLabs<int>(12,8);
    edgeCont[g.addArch(B,T,OpisE(30))]=Koala::Flow::EdgeLabs<int>(14,5);
    edgeCont[g.addArch(C,T,OpisE(10))]=Koala::Flow::EdgeLabs<int>(17,3);
//    edgeCont[g.addLoop(C,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,1);
//    edgeCont[g.addLoop(B,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,-10);

//    edgeCont[g.addArch(S,T,OpisE(40))]=Koala::Flow::EdgeLabs<int>(100,1);

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArch(D,F,OpisE(40));g.addArch(S,C,OpisE(40));g.addArch(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void costTest2()
{   g.clear();edgeCont.clear();
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    edgeCont[g.addEdge(A,S,OpisE(0))]=Koala::Flow::EdgeLabs<int>(18,3);
    edgeCont[g.addEdge(S,C,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,8);
    edgeCont[g.addEdge(A,C,OpisE(10))]=Koala::Flow::EdgeLabs<int>(15,4);
    edgeCont[g.addEdge(B,A,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,5);
    edgeCont[g.addEdge(C,B,OpisE(60))]=Koala::Flow::EdgeLabs<int>(12,8);
    edgeCont[g.addEdge(B,T,OpisE(30))]=Koala::Flow::EdgeLabs<int>(14,5);
    edgeCont[g.addEdge(C,T,OpisE(10))]=Koala::Flow::EdgeLabs<int>(17,3);
//    edgeCont[g.addLoop(B,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,-10);

//    g.addArch(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArch(D,F,OpisE(40));g.addArch(S,C,OpisE(40));g.addArch(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void costTest3()
{   g.clear();edgeCont.clear();
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    edgeCont[g.addArch(S,A,OpisE(0))]=Koala::Flow::EdgeLabs<int>(18,3);
    edgeCont[g.addArch(S,C,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,8);
    edgeCont[g.addArch(A,C,OpisE(10))]=Koala::Flow::EdgeLabs<int>(15,4);
    edgeCont[g.addArch(A,B,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,5);
    edgeCont[g.addArch(C,B,OpisE(60))]=Koala::Flow::EdgeLabs<int>(12,8);
    edgeCont[g.addArch(B,T,OpisE(30))]=Koala::Flow::EdgeLabs<int>(14,5);
    edgeCont[g.addArch(C,T,OpisE(10))]=Koala::Flow::EdgeLabs<int>(17,3);


    edgeCont[g.addArch(A,S,OpisE(0))]=Koala::Flow::EdgeLabs<int>(18,3);
    edgeCont[g.addArch(C,S,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,8);
    edgeCont[g.addArch(C,A,OpisE(10))]=Koala::Flow::EdgeLabs<int>(15,4);
    edgeCont[g.addArch(B,A,OpisE(50))]=Koala::Flow::EdgeLabs<int>(20,5);
    edgeCont[g.addArch(B,C,OpisE(60))]=Koala::Flow::EdgeLabs<int>(12,8);
    edgeCont[g.addArch(T,B,OpisE(30))]=Koala::Flow::EdgeLabs<int>(14,5);
    edgeCont[g.addArch(T,C,OpisE(10))]=Koala::Flow::EdgeLabs<int>(17,3);

//    edgeCont[g.addLoop(B,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,-10);

//    g.addArch(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArch(D,F,OpisE(40));g.addArch(S,C,OpisE(40));g.addArch(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void transTest()
{   g.clear();edgeCont.clear();
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    tedgeCont[g.addArch(S,A,OpisE(0))]=Koala::Flow::TrsEdgeLabs<int>(0,18,3);
    tedgeCont[g.addArch(S,C,OpisE(50))]=Koala::Flow::TrsEdgeLabs<int>(0,20,8);
    tedgeCont[g.addArch(A,C,OpisE(10))]=Koala::Flow::TrsEdgeLabs<int>(0,15,4);
    tedgeCont[g.addEdge(B,A,OpisE(50))]=Koala::Flow::TrsEdgeLabs<int>(0,20,5);
    tedgeCont[g.addArch(C,B,OpisE(60))]=Koala::Flow::TrsEdgeLabs<int>(0,12,8);
    tedgeCont[g.addArch(B,T,OpisE(30))]=Koala::Flow::TrsEdgeLabs<int>(0,14,5);
    tedgeCont[g.addArch(C,T,OpisE(10))]=Koala::Flow::TrsEdgeLabs<int>(0,17,3);
    tedgeCont[g.addLoop(C,OpisE(10))]=Koala::Flow::TrsEdgeLabs<int>(1,10,-1);
//    edgeCont[g.addLoop(B,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,-10);

//    g.addArch(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArch(D,F,OpisE(40));g.addArch(S,C,OpisE(40));g.addArch(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }


    for(Koala::Graph<char,OpisE>::PVertex ePt=g.getVert();ePt;ePt=g.getVertNext(ePt))
        tvertCont[ePt]=Koala::Flow::TrsVertLoss<int>(0,0);
    tvertCont[S].lo=-34;
    tvertCont[S].hi=-31;
    tvertCont[T].lo=31;
    tvertCont[T].hi=100;
}


void ghTest()
{   g.clear();edgeCont.clear();
    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    F=g.addVert('F');//G=g.addVert('G');

    g.addEdge(B,A,OpisE(1));g.addEdge(A,C,OpisE(7));g.addEdge(B,C,OpisE(1));
    g.addEdge(B,D,OpisE(3));g.addEdge(B,E,OpisE(2));g.addEdge(E,C,OpisE(4));
    g.addEdge(E,D,OpisE(1));g.addEdge(D,F,OpisE(6));
    g.addEdge(E,F,OpisE(2));


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


void ghTest2()
{   g.clear();edgeCont.clear();
    vertCont.clear();

    for(int i=0;i<10;i++) g.addVert();
    for(U=g.getVert();U!=g.getVertLast();U=g.getVertNext(U))
        for(V=g.getVertNext(U);V;V=g.getVertNext(V))
        if (rand()%5==0) edgeCont[g.addEdge(U,V)].capac=rand()%10;

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


int main() {

//    g.makeAdjMatrix();
    dijTest();
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);
    cout << "\n***=\n" << boolalpha;
//    cout << Koala::Flow::usedCap(g,edgeCont,g.getEdge(B,C),B,true) << '\n';



//    edgeCont[g.getEdge(A,D)].flow=40;edgeCont[g.getEdge(A,C)].flow=50;

    cout << "\n***\n";
//    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
//        {   char buf[20]; sprintf(buf," %d",edgeCont[ePt].flow); ePt->info.name.append(buf); }
//
//    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists,std::make_pair(true,true));
//

    cout << "\n***\n" << boolalpha;
    cout << Koala::Flow::maxFlow(g,edgeCont,S,T) << '\n';
    assert(Koala::Flow::vertFlow(g,edgeCont,S)==Koala::Flow::maxFlow(g,edgeCont,S,T));
//    cout << Koala::Flow::layerFlow(g,edgeCont,S,T) << '\n';
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';


    costTest2();
    cout << "\n\n-----------\n\n"<< Koala::Flow::maxFlow(g,edgeCont,S,T) << "\n";
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));

    std::pair<int,int> pres=Koala::Flow::minCostFlow(g,edgeCont,S,T,25);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "vol: " << pres.second << " cost: " << pres.first << "\n\n";
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';


    transTest();
    cout << "\n\ntttttttttt\n\n"<< Koala::Flow::transship(g,tedgeCont,tvertCont) << "\n";

    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << tedgeCont[e].flow << '\n';

    cout << "\n\n*************\n\n";
    dijTest();
    int a=Koala::Flow::maxFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "maxf=" << a << '\n';
    for(int i=0;i<=a+1;i++)
    {
        int b=Koala::Flow::maxFlow(g,edgeCont,S,T,i);
        assert(Koala::Flow::testFlow(g,edgeCont,S,T));
        cout << (b==i) << '\n';
    }

    cout << "\n\n*************\n\n";
    costTest2();
    a=Koala::Flow::maxFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "maxf=" << a << '\n';
    for(int i=0;i<=a+1;i++)
    {   int last=-10;
        std::pair<int,int> b=Koala::Flow::minCostFlow(g,edgeCont,S,T,i);
        assert(Koala::Flow::testFlow(g,edgeCont,S,T));
        for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCont[e].flow=0;
//        std::pair<int,int> bb=Koala::Flow::minCostFlow2(g,edgeCont,S,T,i);
//        assert(Koala::Flow::testFlow(g,edgeCont,S,T));
//        assert(b==bb);
        cout << (b.second==i) << ' '<< (b.second>=last) << ' '<< b.first <<'\n';
        last=b.second;
    }

    costTest2();
    Koala::Flow::maxFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
//    edgeCont[g.getEdge(S,A)].flow=2;
//    edgeCont[g.getEdge(A,B)].flow=2;
//    edgeCont[g.getEdge(B,T)].flow=2;
//    edgeCont[g.getEdge(S,C)].flow=2;
//    edgeCont[g.getEdge(C,T)].flow=2;
    int b;
//    cout << (b=Koala::Flow::minMeanCycle(g,edgeCont,Koala::Flow::outPath(tabV,tabE)));
//    for(int i=0;i<b;i++) cout << tabV[i]->info;
//    cout << endl;
//    for(int i=0;i<b;i++) cout << tabE[i]->info.name << ' ';
        cout << "\n\nHHHHHHHHHHHHHHHH\n\n";
    ghTest();
    Koala::Flow::GHTreeEdge<Koala::Graph<char,OpisE>,int> pairTab[1000];

    cout << "\n\n";
    Koala::Flow::findGHTree(g,edgeCont,pairTab);

    cout << "\n\n";
    for(int i=0;i<g.getVertNo()-1;i++)
        std::cout << pairTab[i].first->info << pairTab[i].second->info << ' ' <<pairTab[i].capac <<'\n';

    cout << g.getVertNo();
    cout << "\n\nAAAAAAAAAAAAAAAA\n\n";

    ghTest2();
    Koala::Graph<char,int> ghcopy;
    Koala::Graph<char,int>::PEdge ecopytab[100];
    int gtmin,truemin;
    Koala::AssocTable<std::map<Koala::Graph<char,OpisE>::PVertex,Koala::Graph<char,int>::PVertex> > ghlink;


    {   ghcopy.clear(); ghlink.clear();
        for(Koala::Graph<char,OpisE>::PVertex v=g.getVert();v;v=g.getVertNext(v))
            ghlink[v]=ghcopy.addVert();

        Koala::Flow::findGHTree(g,edgeCont,pairTab);

        for(int i=0;i<g.getVertNo()-1;i++)
            ghcopy.addEdge(ghlink[pairTab[i].first],ghlink[pairTab[i].second],pairTab[i].capac);

    }

    for(U=g.getVert();U!=g.getVertLast();U=g.getVertNext(U))
        for(V=g.getVertNext(U);V;V=g.getVertNext(V))
    //U=A; V=D;
    {   int len=Koala::BFS::getPath(ghcopy,ghlink[U],ghlink[V],Koala::BFS::outPath(blackHole,ecopytab));
        gtmin=std::numeric_limits<int>::max();
        for(int i=0;i<len;i++) gtmin=std::min(gtmin,ecopytab[i]->info);
        truemin=Koala::Flow::minEdgeCut(g,edgeCont,U,V,Koala::Flow::outPath(blackHole,blackHole)).capac;

        std::cout << truemin << ' ' << gtmin << "\n";
        assert(truemin==gtmin);

    }

    cout << g.getVertNo() << ' ' << g.getEdgeNo();
}
