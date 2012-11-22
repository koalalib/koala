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

Koala::Graph<OpisV,OpisE> g;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10];


Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PEdge,Koala::Kruskal::EdgeLabs<int> > > edgeCont;


#include "main.hpp"

    A=g.addVert(OpisV("A"));
    B=g.addVert(OpisV("B"));
    C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));
    E=g.addVert(OpisV("E"));
    F=g.addVert(OpisV("F"));
//

    g.addEdge(A,B,OpisE(1));
    g.addEdge(B,D,OpisE(4));
    g.addEdge(C,D,OpisE(100));
    g.addEdge(A,D,OpisE(2));
    g.addEdge(C,B,OpisE(1));
    //g.addEdge(E,F,OpisE(101));

//    g.clear();g.addVert();

    Koala::JoinableSets<Koala::Graph<OpisV,OpisE>::PVertex,
        Koala::AssocTable< std::map<Koala::Graph<OpisV,OpisE>::PVertex,
            Koala::JSPartDesrc<Koala::Graph<OpisV,OpisE>::PVertex> *> > > sets;

    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        edgeCont[ePt].weight=ePt->info.dlugosc;

//    Koala::Kruskal::Result<int> res=Koala::Kruskal::getForest(g,edgeCont,tabE,Koala::blackHole());
    Koala::Kruskal::Result<int> res=Koala::Kruskal::getMaxForest(g,edgeCont,tabE,sets);

    std::cout << "waga: " << res.weight << " krawedzi " << res.edgeNo << std::endl;
    for(int i=0;i<res.edgeNo;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << ":"<<tabE[i]->info.dlugosc << "}";

    std::cout << "\n" << sets.size() << " " << sets.getSetNo();
    return 0;
}
