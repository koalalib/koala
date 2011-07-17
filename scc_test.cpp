#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <cassert>
#include "Koala/graph/graph.h"
#include "Koala/base/def_struct.h"
#include "Koala/algorithm/search.h"



using namespace std;
using namespace Koala;



struct OpisV;  // pewna struktura opisujaca moj wierzchlek
struct OpisE;   // struktura opisujaca krawedz

struct OpisV {
    OpisV(std::string aname="") : name(aname) {}

    std::string name;
};

struct OpisE {
    OpisE(int len=0) : dlugosc(len) {}
    bool cokolwiek;
    int dlugosc;
};

Koala::Graph<OpisV,OpisE> g;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,G,H,I,V,U,W,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10], EE;
int comptab[2][10];


Koala::AssocArray<Koala::Graph<OpisV,OpisE>::PVertex, int > vertCont;

void searchTest()
{   vertCont.clear();g.clear();
    A=g.addVert(OpisV("A"));
    B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));H=g.addVert(OpisV("H"));

    g.addArch(B,A,OpisE(1));g.addArch(B,C,OpisE(4));g.addArch(C,A,OpisE(5));g.addArch(E,C,OpisE(2));
    g.addArch(D,E,OpisE(1));g.addArch(F,G,OpisE(2)); g.addArch(D,C,OpisE(2)); g.addArch(E,A,OpisE(2));
    g.addArch(D,A,OpisE(2));

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}


void searchTest2()
{   vertCont.clear();g.clear();
    A=g.addVert(OpisV("A"));
    B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));
    H=g.addVert(OpisV("H"));

    g.addArch(A,B,OpisE(1));g.addArch(A,C,OpisE(4));g.addArch(B,C,OpisE(5));g.addArch(C,D,OpisE(2));
    g.addArch(B,D,OpisE(1)); g.addArch(E,F,OpisE(1));
    g.addArch(F,G,OpisE(1));
    g.addArch(G,E,OpisE(1)); g.addArch(D,A,OpisE(1)); g.addArch(C,F,OpisE(1)); g.addArch(D,E,OpisE(1));
    g.addLoop(F,OpisE(1));

    g.addArch(E,H,OpisE(1)); g.addArch(G,H,OpisE(1)); //g.addArch(A,H,OpisE(1));

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}


int main() {

    std::cout << std::boolalpha;

    searchTest2();

    std::cout<<std::endl;
    int compno=Koala::SCC::get(g,Koala::SearchStructs::compStore(comptab[0],tabV),vertCont);
    std::cout << compno<<endl;
    for(int i=0;i<=compno;i++) cout << comptab[0][i] << ' '; cout << endl;
    cout << endl;
    for(int i=0;i<g.getVertNo();i++) cout << tabV[i]->info.name << " ";
    cout<< "\n";
    for(W=g.getVert();W;W=g.getVertNext(W))
        std::cout << W->info.name << ":: " << vertCont[W] << std::endl;

    cout << "\n-------------\n";
    std::pair<int,int> conns[100];
    int cc=Koala::SCC::connections(g,vertCont,conns);
    cout << cc << '\n';
    for(int i=0;i<cc;i++) cout << '(' << conns[i].first << ',' << conns[i].second << ") ";

//    int res;
//    searchTest();
//    Koala::DAGAlgs::topOrd(g,tabV);
//    std::cout << Koala::DAGAlgs::isDAG(g,tabV,tabV+g.getVertNo()) << "\n";
//
//    for(int i=0;i<g.getVertNo();i++)  cout << ((tabV[i]) ? tabV[i]->info.name : "NULL") << " ";
//
//    int r=DAGAlgs::transEdges(g,tabE);
//    std::cout << '\n' << r << '\n';
//    for(int i=0;i<r;i++) std::cout << tabE[i]->getEnds().first->info.name << ' ' << tabE[i]->getEnds().second->info.name << '\n';
//
//    cout << "\n-------------\n";
//    DAGAlgs::makeHasse(g);
//    for(EE=g.getEdge();EE;EE=g.getEdgeNext(EE)) std::cout << EE->getEnds().first->info.name << ' ' << EE->getEnds().second->info.name << '\n';

    return 0;
}
