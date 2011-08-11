#include <iostream>
#include <string>
#include <cstdio>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/conflows.h"
#include "Koala/algorithm/conflows.h"
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
Koala::Graph<char,OpisE>::PEdge tabE[20];

Koala::AssocArray<Koala::Graph<char,OpisE>::PEdge,Koala::Flow::EdgeLabs<int> > edgeCont;

Koala::AssocArray<Koala::Graph<char,OpisE>::PEdge,Koala::Flow::EdgeBound<int> > tedgeCont;
Koala::AssocArray<Koala::Graph<char,OpisE>::PVertex,Koala::Flow::VertLoss<int> > tvertCont;

void dijTest()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    g.addArch(S,A,OpisE(20));g.addArch(S,B,OpisE(50));g.addArch(B,A,OpisE(10));
    g.addArch(A,D,OpisE(50));g.addArch(A,C,OpisE(60));g.addArch(B,C,OpisE(30));
    g.addArch(C,D,OpisE(10));g.addArch(C,T,OpisE(20));
    g.addArch(D,T,OpisE(40));g.addArch(D,E,OpisE(80));g.addArch(E,T,OpisE(40));
//    g.addArch(F,S,OpisE(40));
//    g.addArch(T,S,OpisE(100));

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

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addEdge(S,A,OpisE(20));g.addEdge(S,B,OpisE(50));
    g.addEdge(B,A,OpisE(10));
    g.addEdge(D,A,OpisE(50));g.addEdge(A,C,OpisE(60));g.addEdge(B,C,OpisE(30));
    g.addEdge(C,D,OpisE(10));g.addEdge(C,T,OpisE(20));
    g.addEdge(D,T,OpisE(40));g.addEdge(D,E,OpisE(80));g.addEdge(E,T,OpisE(40));

//    g.addArch(S,T,OpisE(40));g.addArch(S,T,OpisE(40));g.addArch(T,S,OpisE(40));g.addEdge(T,S,OpisE(40));

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


void dijTest4()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addArch(S,A,OpisE(1));g.addArch(S,B,OpisE(1));g.addEdge(A,B,OpisE(1));
    g.addArch(A,D,OpisE(1));g.addArch(A,C,OpisE(1));g.addArch(B,C,OpisE(1));
    g.addArch(C,D,OpisE(1));g.addArch(C,T,OpisE(1));
    g.addArch(D,T,OpisE(1));g.addArch(D,E,OpisE(1));g.addArch(E,T,OpisE(1));
    g.addArch(F,S,OpisE(1));

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

    g.addArch(S,A,OpisE(20));g.addArch(S,B,OpisE(50));g.addArch(B,A,OpisE(10));
    g.addArch(A,D,OpisE(50));g.addArch(A,C,OpisE(60));g.addArch(B,C,OpisE(30));
    g.addArch(C,D,OpisE(10));g.addArch(C,T,OpisE(20));
    g.addArch(D,T,OpisE(40));g.addArch(D,E,OpisE(80));g.addArch(E,T,OpisE(40));
    g.addArch(F,S,OpisE(40));
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


void conTest()
{   g.clear();edgeCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addArch(S,A,OpisE(20));g.addArch(S,B,OpisE(50));g.addEdge(A,B,OpisE(10));
    g.addArch(A,D,OpisE(50));g.addArch(A,C,OpisE(60));g.addArch(B,C,OpisE(30));
    g.addArch(C,D,OpisE(10));g.addArch(C,T,OpisE(20));
    g.addArch(D,T,OpisE(40));g.addArch(D,E,OpisE(80));g.addArch(E,T,OpisE(40));
    g.addArch(F,S,OpisE(40));


    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


int main() {

//    g.makeAdjMatrix();
    dijTest();
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists,std::make_pair(true,true));
    cout << "\n***=\n";
//    cout << Koala::Flow::usedCap(g,edgeCont,g.getEdge(B,C),B,true) << '\n';



//    edgeCont[g.getEdge(A,D)].flow=40;edgeCont[g.getEdge(A,C)].flow=50;

    cout << "\n***\n";
    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20]; sprintf(buf," %d",edgeCont[ePt].flow); ePt->info.name.append(buf); }

    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists,std::make_pair(true,true));

    cout << "\n***\n" << boolalpha;
    cout << Koala::Flow::maxFlow(g,edgeCont,S,T) << '\n';
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';

    cout << "\n\n-----------\n\n";
    dijTest3();
        for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCont[e].capac=1;
    Koala::Flow::EdgeCut<int> res=Koala::Flow::minEdgeCut(g,edgeCont,S,T,Koala::Flow::outPath(tabV,tabE));
    cout << res.capac << endl;
    for(int i=0;i<res.vertNo;i++) cout << tabV[i]->info;
    cout << endl;
    for(int i=0;i<res.edgeNo;i++) cout << tabE[i]->info.name << ' ';

    cout << "\n\n////\n\n";
    dijTest3();

    Koala::Flow::EdgeCut2<Koala::Graph<char,OpisE>, int> res2
        =Koala::Flow::minEdgeCut(g,edgeCont,Koala::Flow::outPath(tabV,tabE));
    cout << res2.capac << ' ' << res2.first->info << res2.second->info << endl;
    for(int i=0;i<res2.vertNo;i++) if (tabV[i]) cout << tabV[i]->info;
    cout << endl;
    for(int i=0;i<res2.edgeNo;i++) if (tabE[i]) cout << tabE[i]->info.name << ' ';


    cout << "\n\n(((((((((((\n\n";
    dijTest4();
    res2
        =Koala::Flow::minEdgeCut(g,edgeCont,Koala::Flow::outPath(tabV,tabE));
    cout << res2.capac << ' ' << res2.first->info << res2.second->info << endl;
    for(int i=0;i<res2.vertNo;i++) if (tabV[i]) cout << tabV[i]->info;
    cout << endl;
    for(int i=0;i<res2.edgeNo;i++) if (tabE[i]) cout << tabE[i]->info.name << ' ';
    cout << "\n\ntttt\n\n";

    tdijTest();
    cout << Koala::Flow::transship(g,tedgeCont,tvertCont) << '\n';
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << tedgeCont[e].flow << ' ' <<tedgeCont[e].hi<<'\n';

    cout << "\n\ncon con con con\n\n";

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
    bool vflag=true;
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists,std::make_pair(true,true));
    int itab[20];
    int p=Koala::Connect::vertDisjPaths(g,S,T,Koala::Connect::outPath(tabV,tabE),itab,vflag);
    cout << p << endl;
    for(int i=0;i<=p;i++) cout << itab[i] << ' ';
    cout <<  endl;
    if (!vflag)
    for(int i=0;i<p;i++)
    {   for(int j=itab[i];j<itab[i+1];j++) cout << tabE[j]->getEnds().first->info
                            << tabE[j]->getEnds().second->info << ' ';
        cout << endl;
    } else for(int i=0;i<p;i++)
    {   for(int j=itab[i];j<itab[i+1];j++) cout << tabV[j]->info << ' ';
        cout << endl;
    }
    cout << endl;
//    for(int i=0;i<20;i++) if (tabV[i]) cout << tabV[i]->info;
//    cout << endl;
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists,std::make_pair(true,true));

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


}
