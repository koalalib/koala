
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
    int dlugosc;
};

Koala::Graph<OpisV,OpisE> g;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10];


Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PVertex,
            Koala::BellmanFord::VertLabs<int, Koala::Graph<OpisV,OpisE> > > > vertCont;
Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PEdge,Koala::BellmanFord::EdgeLabs<int> > > edgeCont;

void bfTest()
{   g.clear();vertCont.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2),Koala::EdDirOut);g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);

    //po dodaniu petli ujemnej mamy cykl ujemny
    //g.addEdge(B,B,OpisE(-1),Koala::EdLoop);

    //po dodaniu tej krawedzi tez mamy cykl ujemny
    //g.addEdge(C,A,OpisE(-4),Koala::EdDirOut);

    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}

int main() {

    bfTest();

    std::cout<< "Odleglosc: "<< Koala::BellmanFord::distances(g,vertCont,edgeCont,U,V)<<":";

//       - mozna i bez ostatniego arg. - liczymy z U do wszystkich
    std::cout << "\nA:" << vertCont[A].distance << " B:" << vertCont[B].distance << std::endl;
    std::cout << "C:" << vertCont[C].distance << " D:" << vertCont[D].distance << std::endl;
    std::cout << "E:" << vertCont[E].distance << " F:" << vertCont[F].distance << std::endl;

    // pobieramy wyznaczona wczesniej sciezke

    if (V) {
        int l;

        std::cout<< std::endl;
        l=Koala::BellmanFord::getPath(g,vertCont,V,Koala::BellmanFord::outPath(tabV,tabE));
        std::cout<<"Liczba krawedzi: "<<l<<std::endl;
        for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }


    // ten sam graf raz jeszcze, szukamy sciezki od nowa
    if (V) {
        Koala::BellmanFord::PathLengths<int> res;

        std::cout<< std::endl;g.clear(); bfTest();
        res=Koala::BellmanFord::findPath(g,edgeCont,U,V,Koala::BellmanFord::outPath(tabV,tabE));
        std::cout<<"Odleglosc: "<< res.length<<std::endl;
        for(int i=0;i<=res.edgeNo;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<res.edgeNo;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }


    // ten sam graf raz jeszcze, kogos nie interesowala sekwencja wierzcholkow
    if (V) {
        Koala::BellmanFord::PathLengths<int> res;

        std::cout<< std::endl;g.clear(); bfTest();
        res=Koala::BellmanFord::findPath(g,edgeCont,U,V,Koala::BellmanFord::outPath(blackHole,tabE));
        std::cout<<"Odleglosc: "<< res.length<<std::endl;
        for(int i=0;i<res.edgeNo;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

    // ... interesowala go tylko odleglosc, nie chcialo mu sie zakladac kontenera asoc. dla wierzcholkow
    std::cout<< std::endl;
    bfTest();
    std::cout<< "Odleglosc: "<< Koala::BellmanFord::distances(g,blackHole,edgeCont,U,V);

    return 0;
}
