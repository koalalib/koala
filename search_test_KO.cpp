#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <cassert>
#include "Koala/graph/graph.h"
#include "Koala/base/defs.h"
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

template< class GraphType > struct VisitLabs
{
    // rodzic danego wierzcholka w drzewie (lesie), NULL dla korzenia
    /** \brief Parent of vertex.*/
    typename GraphType::PVertex vPrev;
    // krawedz prowadzaca do rodzica
    /** \brief Edge leading to parent of vertex.*/
    typename GraphType::PEdge ePrev;

    // odleglosc od korzenia (liczba krawedzi) i numer skladowej spojnosci (od 0)
    int distance;/**< \brief Distance (number of edges) from root.*/
    int component;/**< \brief Index of connected component.*/

    bool dummy;

    /**\brief Constructor*/
    VisitLabs( typename GraphType::PVertex vp = 0, typename GraphType::PEdge ep = 0,
        int dist = std::numeric_limits< int >::max(), int comp = -1 ):
            vPrev( vp ), ePrev( ep ), distance( dist ), component( comp ), dummy(true)
        { }
};


Koala::Graph<OpisV,OpisE> g;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,G,H,I,V,U,W,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10];


Koala::AssocArray<Koala::Graph<OpisV,OpisE>::PVertex,
            VisitLabs<Koala::Graph<OpisV,OpisE> > > vertCont;
//            Koala::SearchStructs::VisitVertLabs<Koala::Graph<OpisV,OpisE> > > vertCont;

void searchTest()
{   vertCont.clear();g.clear();
    A=g.addVert(OpisV("A"));

    B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));H=g.addVert(OpisV("H"));I=g.addVert(OpisV("I"));

    g.addEdge(A,B,OpisE(1));g.addEdge(A,C,OpisE(4));g.addEdge(B,D,OpisE(5));g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2),Koala::EdDirOut);g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirIn);
    g.addArc(G,H,OpisE(2));

    g.addArc(G,H,OpisE(2));g.addArc(G,H,OpisE(2));

//    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
//        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}

void searchTest2()
{   vertCont.clear();g.clear();
    A=g.addVert(OpisV("A"));
    B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));
//    E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
//    G=g.addVert(OpisV("G"));

    g.addEdge(A,B,OpisE(1));g.addEdge(A,C,OpisE(4));g.addEdge(B,C,OpisE(5));g.addEdge(B,D,OpisE(2));
    g.addEdge(C,D,OpisE(1));

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}



//#define STRATEGY LexBFS
//#define STRATEGY BFS
//#define STRATEGY DFSPreorder
#define STRATEGY DFSPostorder

#include "main.hpp"

    searchTest();
    int licz;
    std::cout << (licz=Koala::STRATEGY::visitBase(g,A,vertCont,Visitors::EndVertVisitor(E),EdAll,1)) << std::endl;
    licz=g.getVertNo();
    for(W=g.getVert();W;W=g.getVertNext(W))
        std::cout << W->info.name << ":: dist:" << vertCont[W].distance << " pred:"
        << ( (vertCont[W].vPrev) ? vertCont[W].vPrev->info.name : string(" NULL") )<<
        " compon:" << vertCont[W].component << std::endl;
    std::cout << "\n***\n";
    for(int i=0;i<licz;i++) std::cout << ((tabV[i]) ? tabV[i]->info.name : string(" NULL"));
    std::cout << "\n***\n";

    searchTest();

    int radius=2;
    std::cout << (licz=Koala::STRATEGY::visitBase(g,A,vertCont,Visitors::NearVertsVisitor(radius),EdAll,1)) << std::endl;
    licz=g.getVertNo();
    for(W=g.getVert();W;W=g.getVertNext(W))
        std::cout << W->info.name << ":: dist:" << vertCont[W].distance << " pred:"
        << ( (vertCont[W].vPrev) ? vertCont[W].vPrev->info.name : string(" NULL") )<<
        " compon:" << vertCont[W].component << std::endl;
    std::cout << "\n***\n";
    for(int i=0;i<licz;i++) std::cout << ((tabV[i]) ? tabV[i]->info.name : string(" NULL"));

    return 0;

}
