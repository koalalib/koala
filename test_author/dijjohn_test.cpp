#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"
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

Koala::SimplArrPool<Koala::Vertex<OpisV,OpisE> > vpool(10);
Koala::SimplArrPool<Koala::Edge<OpisV,OpisE> > epool(50);

Koala::Graph<OpisV,OpisE> g(&vpool,&epool);
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10];


struct VLab {
    Koala::Graph<OpisV,OpisE>::PVertex vPrev;
    Koala::Graph<OpisV,OpisE>::PEdge ePrev;
    int distance;

    typedef int DistType;
};

Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PVertex, Koala::All2AllDists::VertLabs<int, Koala::Graph<OpisV,OpisE> > > > vertCont;
//            Koala::Dijkstra::VertLabs<int, Koala::Graph<OpisV,OpisE> > > > vertCont;

Koala::AssocMatrix<Koala::Graph<OpisV,OpisE>::PVertex,
        Koala::All2AllDists::VertLabs<int, Koala::Graph<OpisV,OpisE> >,
            Koala::AMatrFull> twoDimVertCont;

Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PEdge,Koala::All2AllDists::EdgeLabs<int> > > edgeCont;


void dijTest()
{   g.clear();vertCont.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2));g.addEdge(D,E,OpisE(1));
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);

    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}


#include "main.hpp"
    g.makeAdjMatrix();

    dijTest();
    std::cout<< "Odleglosc: "<< Koala::DijkstraHeap::distances(g,vertCont,edgeCont,U)<<":";
//       - mozna i bez ostatniego arg. - liczymy z U do wszystkich
    std::cout << "\nE:" << vertCont[E].distance << " F:" << vertCont[F].distance << std::endl;

    // pobieramy wyznaczona wczesniej sicezke

    if (V) {
        int l;

        std::cout<< std::endl;
        l=Koala::Dijkstra::getPath(g,vertCont,V,Koala::Dijkstra::outPath(tabV,tabE));

        std::cout<<"Liczba krawedzi: "<<l<<std::endl;
        for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

    std::cout<< "jjjjjjjjjjjj" << std::endl;

    Koala::All2AllDists::johnson(g,twoDimVertCont,edgeCont);
    vertCont.clear(); twoDimVertCont.slice1(U,vertCont);
    std::cout << "\nE:" << vertCont[E].distance << " F:" << vertCont[F].distance << std::endl;

    // pobieramy wyznaczona wczesniej sicezke

    if (V) {
        int l;

        std::cout<< std::endl;
        l=Koala::Dijkstra::getPath(g,vertCont,V,Koala::Dijkstra::outPath(tabV,tabE));

        std::cout<<"Liczba krawedzi: "<<l<<std::endl;
        for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

    std::cout<< "fffffffffffff" << std::endl;

    twoDimVertCont.clear();
    Koala::All2AllDists::floyd(g,twoDimVertCont,edgeCont);
    vertCont.clear(); twoDimVertCont.slice1(U,vertCont);
    std::cout << "\nE:" << vertCont[E].distance << " F:" << vertCont[F].distance << std::endl;

    // pobieramy wyznaczona wczesniej sicezke

    if (V) {
        int l;

        std::cout<< std::endl;
        l=Koala::Dijkstra::getPath(g,vertCont,V,Koala::Dijkstra::outPath(tabV,tabE));

        std::cout<<"Liczba krawedzi: "<<l<<std::endl;
        for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }


    return 0;
}
