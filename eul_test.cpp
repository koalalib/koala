#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/search.h"


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
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[20];
Koala::Graph<OpisV,OpisE>::PEdge tabE[20];
std::pair<Koala::Graph<OpisV,OpisE>::PVertex,Koala::Graph<OpisV,OpisE>::PVertex> para;


void dijTest()
{   g.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));

    F=g.addVert(OpisV("F"));

//    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
//    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2));g.addEdge(D,E,OpisE(1));
//    g.addEdge(A,D,OpisE(2));

    g.addArch(A,B,OpisE(1));g.addArch(D,B,OpisE(5));g.addArch(A,C,OpisE(4));g.addArch(C,D,OpisE(2));
    g.addArch(C,E,OpisE(1));g.addArch(B,C,OpisE(2));g.addArch(E,D,OpisE(1));
    g.addArch(D,A,OpisE(2));

//
//    g.addArch(F,A,OpisE(2));g.addArch(B,F,OpisE(2));
    g.addEdge(F,A,OpisE(2));g.addEdge(F,B,OpisE(2));
    g.addLoop(E);g.addLoop(E);g.addLoop(D);

    g.addArch(B,A,OpisE(2));
//    g.addArch(A,B,OpisE(2));

    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}

Koala::EdgeDirection mask=Koala::EdUndir|Koala::EdLoop;

#include "main.hpp"

    dijTest();

    para=Koala::Euler::ends(g);
    std::cout << "\n[";
    if (para.first) std::cout << para.first->info.name; else std::cout << "NULL";
    if (para.second) std::cout << para.second->info.name; else std::cout << "NULL";
    std::cout << "]\n";

    std::cout <<"C:"<< Koala::Euler::hasCycle(g)<<std::endl;
    std::cout<<"P:"<< Koala::Euler::hasPath(g)<<std::endl;

    std::cout << "\n\n\n";


        std::cout<< "C:" << Koala::Euler::getDirCycle(g,Koala::Euler::outPath(tabV,tabE))<<std::endl;
        for(int i=0;tabV[i];i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;tabE[i];i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";

        dijTest();


        std::cout<< "\n\nP:" <<Koala::Euler::getDirPath(g,Koala::Euler::outPath(tabV,tabE))<<std::endl;
        for(int i=0;tabV[i];i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;tabE[i];i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;

        g.clear();
        A=g.addVert(); B=g.addVert(); C=g.addVert();
        g.addArch(A,B); g.addEdge(B,A); //g.addLoop(C);

        std::cout << g.mu(g.getEdge(),Koala::EdUndir);

    return 0;
}
