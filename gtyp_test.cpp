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

Koala::Graph<OpisV,OpisE,Allow> g;
Koala::Graph<OpisV,OpisE,Allow>::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE,Allow>::PEdge tabE[10];



int main() {

//    g.makeAdjMatrix();

    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

//    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
//    g.addEdge(C,E,OpisE(1));

    g.addEdge(B,C,OpisE(2),Koala::EdDirOut);
    g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);
//    g.addLoop(F,OpisE(2));
//    g.makeAdjMatrix();
//    std::cout << g.hasAdjMatrix();

    return 0;
}
