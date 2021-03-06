#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/weights.h"


//przyklad uzycia, w tej wersji zawartosc obu struktur jest dla Dijkstry nieistotna
struct OpisV;  // pewna struktura opisujaca moj wierzchlek
struct OpisE;   // struktura opisujaca krawedz

struct OpisV {
    OpisV(std::string aname="") : name(aname) {}

    std::string name;
};

struct OpisE {
    OpisE(int len=0) : dlugosc(len) {}
    bool cokolwiek;
    int dlugosc; // dla Dijkstry niepotrzebne
};

const Koala::EdgeType Allow=Koala::EdDirIn|Koala::EdDirOut;
const bool AdjAllow=true;

Koala::Graph<OpisV,OpisE,Koala::GrDefaultSettings<Allow,AdjAllow> > g;
Koala::Graph<OpisV,OpisE,Koala::GrDefaultSettings<Allow,AdjAllow> >::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE,Koala::GrDefaultSettings<Allow,AdjAllow> >::PEdge tabE[10];



#include "main.hpp"

    g.makeAdjMatrix();

    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

//    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
//    g.addEdge(C,E,OpisE(1));

    g.addEdge(B,C,OpisE(2),Koala::EdDirOut);
    g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);
//    g.addEdge(F,D);
//    g.addLoop(F,OpisE(2));

    std::cout << std::boolalpha << g.hasAdjMatrix();
    std::cout << '\n' << g.getEdgeNo(Koala::EdDirOut|Koala::EdDirIn);

    g.neg();

    return 0;
}
