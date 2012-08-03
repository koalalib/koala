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


Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PVertex,
            Koala::DAGCritPath::VertLabs<int, Koala::Graph<OpisV,OpisE> > > > vertCont;
Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PEdge,Koala::DAGCritPath::EdgeLabs<int> > > edgeCont;


void dijTest()
{   g.clear();vertCont.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

    g.addArch(A,B,OpisE(1));g.addArch(B,D,OpisE(1));g.addArch(A,C,OpisE(1));g.addArch(C,D,OpisE(1));
    g.addArch(C,E,OpisE(1));g.addArch(B,C,OpisE(1));g.addArch(D,E,OpisE(1));
    g.addArch(F,D,OpisE(1));

    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}

int main() {

//    g.makeAdjMatrix();
    dijTest();

    std::cout<< "Odleglosc: "<< Koala::DAGCritPath::critPathLength(g,vertCont,edgeCont,U,V)<<":";
//       - mozna i bez ostatniego arg. - liczymy z U do wszystkich
    std::cout << "\nE:" << vertCont[E].distance << " F:" << vertCont[F].distance << std::endl;

    // pobieramy wyznaczona wczesniej sicezke

    if (V) {
        int l;

        std::cout<< std::endl;
        l=Koala::DAGCritPath::getPath(g,vertCont,V,Koala::DAGCritPath::outPath(tabV,tabE));
        std::cout<<"Liczba krawedzi: "<<l<<std::endl;
        for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }


    if (V) {
        Koala::DAGCritPath::PathLengths<int> res;

        std::cout<< std::endl;g.clear(); dijTest();
        res=Koala::DAGCritPath::findPath(g,edgeCont,U,V,Koala::DAGCritPath::outPath(tabV,tabE));
        std::cout<<"Odleglosc: "<< res.length<<std::endl;
        for(int i=0;i<=res.edgeNo;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<res.edgeNo;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

    if (V) {
        Koala::DAGCritPath::PathLengths<int> res;

        std::cout<< std::endl;g.clear(); dijTest();
        res=Koala::DAGCritPath::findPath(g,edgeCont,U,V,Koala::DAGCritPath::outPath(blackHole,tabE));
        std::cout<<"Odleglosc: "<< res.length<<std::endl;
        for(int i=0;i<res.edgeNo;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

    std::cout<< std::endl;
    dijTest();
    std::cout<< "Odleglosc: "<< Koala::DAGCritPath::critPathLength(g,blackHole,Koala::DAGCritPath::UnitLengthEdges<int>(),U,V)<<":";

    return 0;
}
