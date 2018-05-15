#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"


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

Koala::Graph<OpisV,OpisE> g;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10];


void dijTest()
{   g.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2),Koala::EdDirOut);g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}

#include "main.hpp"

    Koala::EdgeDirection mask=Koala::EdDirIn;
    Koala::Subgraph<Koala::Graph<OpisV,OpisE>, Koala::BoolChooser, Koala::BoolChooser>
            sg(g,std::make_pair(Koala::stdChoose(true),Koala::stdChoose(true)));
//    g.makeAdjMatrix();
    dijTest();

    g.addEdge(C,D);g.addEdge(C,D);//g.addEdge(C,D);
    g.addArc(C,B);g.addArc(C,B);

    g.addLoop(E);g.addLoop(E);g.addLoop(E);
//    g.clearEdges();g.addLoop(E);


    std::pair<Koala::Graph<OpisV,OpisE>::PEdge, int> res=sg.maxMu(mask);
    std::cout<< res.second << " : ";
    if (res.first) std::cout <<  "{"<< sg.getEdgeEnds(res.first).first->info.name <<
                            "," << sg.getEdgeEnds(res.first).second->info.name << "}";
                    else std::cout << "NULL";
    std::cout<< '\n' << sg.mu(mask) << ' ' << sg.mu(res.first,mask);
    std::cout <<sg.getParals(blackHole,sg.getEdge(E,E),mask);
    return 0;
}
