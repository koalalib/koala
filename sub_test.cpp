#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"
#include "Koala/io/text.h"


//przyklad uzycia, w tej wersji zawartosc obu struktur jest dla Dijkstry nieistotna
struct OpisV;  // pewna struktura opisujaca moj wierzchlek
struct OpisE;   // struktura opisujaca krawedz

struct OpisV {
    OpisV(std::string aname="") : name(aname) {}

    std::string name;
};

std::ostream& operator<<(std::ostream& str,const OpisV &rec)
{
    str<< '\"' << rec.name << '\"';
    return str;
}

struct OpisE {
    OpisE(int len=0) : dlugosc(len) {}
    bool cokolwiek;
    int dlugosc; // dla Dijkstry niepotrzebne
};

std::ostream& operator<<(std::ostream& str,const OpisE &rec)
{
    str<<  rec.dlugosc;
    return str;
}


Koala::Graph<OpisV,OpisE> g,*wg;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10],EE;

Koala::Set<Koala::Graph<OpisV,OpisE>::PVertex> rset,gset;


#include "main.hpp"


    A=g.addVert(OpisV("A"));
    B=g.addVert(OpisV("B"));
    C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));

    g.addEdge(A,B,OpisE(1));
    EE=g.addEdge(A,C,OpisE(4));
    g.addEdge(A,D,OpisE(1));
    g.addEdge(B,C,OpisE(2));
    g.addEdge(B,D,OpisE(1));
    g.addEdge(C,D,OpisE(101));
//    g.addEdge(A,A,OpisE(5),Koala::EdLoop);

    rset+=A; rset+=B; rset+=C; gset+=A; gset+=C; gset+=D;
    std::make_pair(Koala::stdChoose(true),Koala::stdChoose(true));

Koala::Subgraph<Koala::Graph<OpisV,OpisE>,Koala::SetChooser<Koala::Vertex<OpisV,OpisE> >,Koala::BoolChooser>
    sg1=std::make_pair(Koala::stdChoose(rset),Koala::stdChoose(true)),
    sg2;
   sg2.plug(g); sg2.setChoose (std::make_pair(Koala::stdChoose(gset),Koala::stdChoose(true)));

Koala::Subgraph<Koala::Subgraph<Koala::Graph<OpisV,OpisE>,Koala::SetChooser<Koala::Vertex<OpisV,OpisE> >,Koala::BoolChooser>,
    Koala::SetChooser<Koala::Vertex<OpisV,OpisE> >,Koala::BoolChooser>
        ssg=std::make_pair(Koala::stdChoose(gset),Koala::stdChoose(true));

    Koala::makeSubgraph(g,std::make_pair(Koala::stdChoose(rset),Koala::stdChoose(true))).getParentPtr();
    Koala::makeSubgraph(sg1,std::make_pair(Koala::stdChoose(gset),Koala::stdChoose(true)));
    Koala::IO::writeGraphText(Koala::makeSubgraph(Koala::makeSubgraph(g,std::make_pair(Koala::stdChoose(rset),Koala::stdChoose(true))),std::make_pair(Koala::stdChoose(gset),Koala::stdChoose(true))),std::cout,Koala::IO::RG_EdgeList);
    std::cout<< "\n---------";

//    g.clear();g.addVert();

    wg=new Koala::Graph<OpisV,OpisE>();


    sg1.plug(g);

    std::cout << "\n";
    std::cout << "g: " << &g << "\n";
    std::cout << "wg: " << wg << "\n";
    std::cout << " sg1: " << &sg1 << ",par:" << sg1.getParentPtr() << ",root:"<< sg1.getRootPtr() << "\n";
    std::cout << " sg2: " << &sg2 << ",par:" << sg2.getParentPtr() << ",root:"<< sg2.getRootPtr() << "\n";
    std::cout << " sgs: " << &ssg << ",par:" << ssg.getParentPtr() << ",root:"<< ssg.getRootPtr() << "\n";


    std::cout << "\n";
    std::cout << "g: " << &g << "\n";
    std::cout << "wg: " << wg << "\n";
    std::cout << " sg1: " << &sg1 << ",par:" << sg1.getParentPtr() << ",root:"<< sg1.getRootPtr() << "\n";
    std::cout << " sg2: " << &sg2 << ",par:" << sg2.getParentPtr() << ",root:"<< sg2.getRootPtr() << "\n";
    std::cout << " sgs: " << &ssg << ",par:" << ssg.getParentPtr() << ",root:"<< ssg.getRootPtr() << "\n";


    std::cout << "\n";
    for(Koala::Graph<OpisV,OpisE>::PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (sg1.good(v)) std::cout << v->info.name;
    std::cout << "\n";
    for(Koala::Graph<OpisV,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        if (sg1.good(e)) std::cout << '(' << e->getEnds().first->info.name << e->getEnds().second->info.name <<')';
    std::cout << "\n";

    std::cout << "\n";
    for(Koala::Graph<OpisV,OpisE>::PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (sg2.good(v)) std::cout << v->info.name;
    std::cout << "\n";
    for(Koala::Graph<OpisV,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        if (sg2.good(e)) std::cout << '(' << e->getEnds().first->info.name << e->getEnds().second->info.name <<')';
    std::cout << "\n";

    ssg.plug(sg1);

    std::cout << "\n";
    for(Koala::Graph<OpisV,OpisE>::PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (ssg.good(v,true)) std::cout << v->info.name;
    std::cout << "\n";
    for(Koala::Graph<OpisV,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        if (ssg.good(e,true)) std::cout << '(' << e->getEnds().first->info.name << e->getEnds().second->info.name <<')';
    std::cout << "\n";

    std::cout << "\n";
    std::cout << ssg.Delta();
    std::cout << "\n";
    ssg.plug(sg1);

//    ssg.allowedEdgeTypes();

    std:: cout << ssg.areParallel(ssg.getEdgeLast(),ssg.getEdgeLast());
    std::cout << "\n\n";

    Koala::IO::writeGraphText(Koala::makeSubgraph(g,
                std::make_pair(Koala::stdChoose(rset),
                               !Koala::stdChoose(Koala::Set<Koala::Edge<OpisV,OpisE>* >()+=EE)))
                ,std::cout,Koala::IO::RG_EdgeList);

}
