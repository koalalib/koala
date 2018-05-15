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
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,G,V,U,tabV[20];
Koala::Graph<OpisV,OpisE>::PEdge tabE[20];
std::pair<Koala::Graph<OpisV,OpisE>::PVertex,Koala::Graph<OpisV,OpisE>::PVertex> para;


void dijTest()
{   g.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));

    F=g.addVert(OpisV("F"));G=g.addVert(OpisV("G"));

//    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
//    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2));g.addEdge(D,E,OpisE(1));
//    g.addEdge(A,D,OpisE(2));

    g.addArc(A,B,OpisE(1));g.addArc(D,B,OpisE(5));g.addArc(A,C,OpisE(4));g.addArc(C,D,OpisE(2));
    g.addArc(C,E,OpisE(1));g.addArc(B,C,OpisE(2));g.addArc(E,D,OpisE(1));
    g.addArc(D,A,OpisE(2));

//
//    g.addArc(F,A,OpisE(2));g.addArc(B,F,OpisE(2));
    g.addEdge(F,A,OpisE(2));g.addEdge(F,B,OpisE(2));
    g.addLoop(E);g.addLoop(E);g.addLoop(D);

    g.addArc(B,A,OpisE(2));
//    g.addArc(A,B,OpisE(2));

    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}

void dijTest2()
{   g.clear();
    A=g.addVert(OpisV("A"));
//    g.addLoop(A);
    E=g.addVert(OpisV("E"));

    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}

void dijTest3()
{   g.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));

g.addArc(G,A,OpisE(1));
    g.addArc(A,G,OpisE(1));g.addLoop(A,OpisE(1));g.addLoop(A,OpisE(1));
    g.addArc(A,G,OpisE(1));g.addArc(G,A,OpisE(1));


    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}


Koala::EdgeDirection mask=Koala::EdUndir|Koala::EdLoop;

#include "main.hpp"

//#define HASP hasPath
//#define HASC hasCycle
//#define GETP getPath
//#define GETC getCycle
//#define ENDS ends

#define HASP hasDirPath
#define HASC hasDirCycle
#define GETP getDirPath
#define GETC getDirCycle
#define ENDS dirEnds

    dijTest3();
//    g.clearEdges();

    para=Koala::Euler:: ENDS (g);
    std::cout << "\n[" << std::boolalpha;
    if (para.first) std::cout << para.first->info.name; else std::cout << "NULL";
    if (para.second) std::cout << para.second->info.name; else std::cout << "NULL";
    std::cout << "]\n";

    std::cout <<"Cycle:"<< Koala::Euler:: HASC (g,G)<<std::endl;
    std::cout<<"Path:"<< Koala::Euler:: HASP (g,G)<<std::endl;

    std::cout << "\n\n\n";


        std::cout<< "Cycle:" << Koala::Euler:: GETC (g,G,Koala::Euler::outPath(tabV,tabE))<<std::endl;
        for(int i=0;tabV[i];i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;tabE[i];i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";

        dijTest3();


        std::cout<< "\n\nPath:" <<Koala::Euler:: GETP (g,Koala::Euler::outPath(tabV,tabE))<<std::endl;
        for(int i=0;tabV[i];i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;tabE[i];i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;

        g.clear();
        A=g.addVert(); B=g.addVert(); C=g.addVert();
        g.addArc(A,B); g.addEdge(B,A); //g.addLoop(C);

//        std::cout << g.mu(g.getEdge(),Koala::EdUndir);


    return 0;
}
