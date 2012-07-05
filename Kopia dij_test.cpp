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


struct VLab {
    Koala::Graph<OpisV,OpisE>::PVertex vPrev;
    Koala::Graph<OpisV,OpisE>::PEdge ePrev;
    int distance;

    typedef int DistType;
};

std::map<Koala::Graph<OpisV,OpisE>::PVertex, VLab> vertCont;
Koala::AssocTabInterface<std::map<Koala::Graph<OpisV,OpisE>::PVertex, VLab> > ivert=vertCont;
//            Koala::Dijkstra::VertLabs<int, Koala::Graph<OpisV,OpisE> > > > vertCont;

std::map<Koala::Graph<OpisV,OpisE>::PEdge,Koala::Dijkstra::EdgeLabs<int> > edgeCont;


void dijTest()
{   g.clear();vertCont.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2),Koala::EdDirOut);g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);

    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        Koala::assocTabInterf(edgeCont)[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}

int main() {

    g.makeAdjMatrix();
    dijTest();

    std::cout<< "Odleglosc: "<< Koala::Dijkstra::distances(g,ivert,Koala::assocTabInterf(edgeCont),U,V)<<":";
//       - mozna i bez ostatniego arg. - liczymy z U do wszystkich
    std::cout << "\nE:" << vertCont[E].distance << " F:" << vertCont[F].distance << std::endl;

    // pobieramy wyznaczona wczesniej sicezke

    if (V) {
        int l;

        std::cout<< std::endl;
//        l=Koala::Dijkstra::getOutPath(g,vertCont,Koala::Dijkstra::outPath(tabV,tabE),V,F);
        l=Koala::Dijkstra::getPath(g,Koala::assocTabInterf(vertCont),V,Koala::Dijkstra::outPath(tabV,tabE));
        std::cout<<"Liczba krawedzi: "<<l<<std::endl;
        for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

//    std::cout << Koala::Dijkstra::getUsedEdgesSet(g,vertCont);

    // ten sam graf raz jeszcze, szukamy sciezki od nowa


    return 0;
}
