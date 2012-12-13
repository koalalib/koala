#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/weights.h"

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
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[20];
Koala::Graph<OpisV,OpisE>::PEdge tabE[20];

Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PEdge,Koala::Floyd::EdgeLabs<int> > > edgeCont;

//dwuwymiarowa tablica asocjacyjna - zawiera dlugosci najkrotszych sciezek pomiedzy kazda para wierzcholkow
//twoDimVertCont(A,B).distance zawierac bedzie najkrotsza sciezke z A do B
Koala::AssocMatrix<Koala::Graph<OpisV,OpisE>::PVertex,
        Koala::Floyd::VertLabs<int, Koala::Graph<OpisV,OpisE> >,
            Koala::AMatrFull> twoDimVertCont;

void floydTest()
{   g.clear();twoDimVertCont.clear();
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
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}


#include "main.hpp"

    floydTest();

    std::cout<< "Odleglosci pomiedzy kazda para wierzcholkow" << std::endl << std::endl;

    g.clear();floydTest();Koala::Floyd::distances(g,twoDimVertCont,edgeCont);
    for(Koala::Graph<OpisV,OpisE>::PVertex u=g.getVert();u;u=g.getVertNext(u)){
        for(Koala::Graph<OpisV,OpisE>::PVertex v=g.getVert();v;v=g.getVertNext(v)){
            std::cout << u->info.name << v->info.name << ":" << twoDimVertCont(u,v).distance << "  ";
        }
        std::cout<< std::endl;
    }

    // pobieramy wyznaczone wczesniej sciezki
    std::cout<< "--------------------------------------------------------------------------------" << std::endl;
    std::cout<< "Liczba krawedzi, odwiedzone wierzcholki, odwiedzone krawedzie" << std::endl;
    for(Koala::Graph<OpisV,OpisE>::PVertex v_start=g.getVert();v_start;v_start=g.getVertNext(v_start)){
        int l;
        for(Koala::Graph<OpisV,OpisE>::PVertex v_end=g.getVert();v_end;v_end=g.getVertNext(v_end)){
            std::cout<< std::endl; for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
            l=Koala::Floyd::getPath(g,twoDimVertCont,v_start,v_end,Koala::Floyd::outPath(tabV,tabE));
            std::cout << v_start->info.name << v_end->info.name << ":" << l;
            if(l>=0) std::cout<< ", ";
            for(int i=0;i<20;i++) if (tabV[i]) std::cout<< tabV[i]->info.name;
            if(l>0) std::cout<< ", ";
            for(int i=0;i<20;i++) if (tabE[i]) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                                "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        }
        std::cout<< std::endl;
    }


    //to samo jak powyzej pobieramy wyznaczone wczesniej sciezki, ale nie isterusuje nas sekwencja wierzcholkow, wiec uzywamy blackHole
    std::cout<< "--------------------------------------------------------------------------------" << std::endl;
    std::cout<< "Liczba krawedzi, odwiedzone krawedzie" << std::endl;
    for(Koala::Graph<OpisV,OpisE>::PVertex v_start=g.getVert();v_start;v_start=g.getVertNext(v_start)){
        int l;
        for(Koala::Graph<OpisV,OpisE>::PVertex v_end=g.getVert();v_end;v_end=g.getVertNext(v_end)){
            std::cout<< std::endl;
            l=Koala::Floyd::getPath(g,twoDimVertCont,v_start,v_end,Koala::Floyd::outPath(blackHole,tabE));
            std::cout << v_start->info.name << v_end->info.name << ":" << l;
            if(l>0) std::cout<< ", ";
            for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                                "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        }
        std::cout<< std::endl;
    }


    //to samo jak powyzej pobieramy wyznaczone wczesniej sciezki, ale nie isterusuje nas sekwencja krawêdzi, wiec uzywamy blackHole
    std::cout<< "--------------------------------------------------------------------------------" << std::endl;
    std::cout<< "Liczba krawedzi, odwiedzone wierzcholki" << std::endl;
    for(Koala::Graph<OpisV,OpisE>::PVertex v_start=g.getVert();v_start;v_start=g.getVertNext(v_start)){
        int l;
        for(Koala::Graph<OpisV,OpisE>::PVertex v_end=g.getVert();v_end;v_end=g.getVertNext(v_end)){
            std::cout<< std::endl;
            l=Koala::Floyd::getPath(g,twoDimVertCont,v_start,v_end,Koala::Floyd::outPath(tabV,blackHole));
            std::cout << v_start->info.name << v_end->info.name << ":" << l;
            if(l>=0) std::cout<< ", ";
            for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name;
        }
        std::cout<< std::endl;
    }


    return 0;
}
