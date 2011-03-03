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
Koala::Graph<OpisV,OpisE>::PEdge tabE[10];


Koala::AssocArray<Koala::Graph<OpisV,OpisE>::PVertex,
            Koala::SearchStructs::VisitVertLabs<Koala::Graph<OpisV,OpisE> > > vertCont;

void searchTest()
{   vertCont.clear();g.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));H=g.addVert(OpisV("H"));I=g.addVert(OpisV("I"));

    g.addEdge(A,B,OpisE(1));g.addEdge(A,C,OpisE(4));g.addEdge(B,D,OpisE(5));g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2),Koala::EdDirOut);g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);
    g.addArch(G,H,OpisE(2));

//    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
//        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}


int main() {

    searchTest();
    int licz;
    std::cout << (licz=Koala::BFS::scan(g,vertCont,tabV,EdAll)) << std::endl;
    licz=g.getVertNo();
    for(W=g.getVert();W;W=g.getVertNext(W))
        std::cout << W->info.name << ":: dist:" << vertCont[W].distance << " pred:"
        << ( (vertCont[W].vPrev) ? vertCont[W].vPrev->info.name : string("NULL") )<<
        " compon:" << vertCont[W].component << std::endl;
    std::cout << "\n***\n";
    for(int i=0;i<licz;i++) std::cout << ((tabV[i]) ? tabV[i]->info.name : string("NULL"));
    std::cout << "\n***\n";

            searchTest();
    std::cout<< Koala::BFS::getAttainableSet(g,A,EdAll);
    std::cout<< Koala::BFS::getPath(g,Koala::BFS::outPath(tabV,tabE),A,F,EdAll);

        searchTest();

    int comptab[2][10];
    comptab[1][0]=0;
    vertCont.clear();

//    g.del(I); g.del(G); g.del(H);

    std::cout<<std::endl;
    int compno=Koala::BFS::getComponents(g,blackHole(),Koala::SearchStructs::compStore(comptab[0],tabV),EdAll);
    std::cout << compno<<endl;
    for(int i=0;i<=compno;i++) cout << comptab[0][i] << ' '; cout << endl;
    cout << endl;
    for(int i=0;i<g.getVertNo();i++) cout << tabV[i]->info.name << " ";
    cout<< "\n" << vertCont.size();


    return 0;
}
