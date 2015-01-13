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
    sg1=Koala::stdChoose(rset)&Koala::stdChoose(true),
    sg2;
   sg2.plug(g); sg2.setChoose (Koala::stdChoose(gset)&Koala::stdChoose(true));

Koala::Subgraph<Koala::Subgraph<Koala::Graph<OpisV,OpisE>,Koala::SetChooser<Koala::Vertex<OpisV,OpisE> >,Koala::BoolChooser>,
    Koala::SetChooser<Koala::Vertex<OpisV,OpisE> >,Koala::BoolChooser>
        ssg=std::make_pair(Koala::stdChoose(gset),Koala::stdChoose(true));

    Koala::makeSubgraph(g,std::make_pair(Koala::stdChoose(rset),Koala::stdChoose(true))).getParentPtr();
    Koala::makeSubgraph(sg1,std::make_pair(Koala::stdChoose(gset),Koala::stdChoose(true)));
    Koala::IO::writeGraphText(Koala::makeSubgraph(Koala::makeSubgraph(g,Koala::stdChoose(rset)&Koala::stdChoose(true)),std::make_pair(Koala::stdChoose(gset),Koala::stdChoose(true))),std::cout,Koala::IO::RG_EdgeList);
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
        if (Koala::makeRevView(ssg).good(v,true)) std::cout << v->info.name;
    std::cout << "\n";
    for(Koala::Graph<OpisV,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        if (Koala::makeRevView(ssg).good(e,true)) std::cout << '(' << e->getEnds().first->info.name << e->getEnds().second->info.name <<')';
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



    std::cout << "\n****************\n\n";
    Koala::IO::writeGraphText(g,std::cout,Koala::IO::RG_EdgeList);

    std::cout << "\n"<< g.getVertNo() << " "<< g.getEdgeNo();

    Koala::Subgraph<Koala::Graph<OpisV,OpisE>,Koala::SetChooser<Koala::Vertex<OpisV,OpisE> >,Koala::SetChooser<Koala::Edge<OpisV,OpisE> > >
        sgb,sgb2;
    Koala::Graph<int,int,Koala::GrDefaultSettings< Koala::EdAll,false > > g_bez;

    makeSubgraph(g_bez,Koala::stdChoose(true)&Koala::stdChoose(true));
    g.makeAdjMatrix();g_bez.makeAdjMatrix();
#define gr2test makeSubgraph(g_bez,Koala::stdChoose(true)&Koala::stdChoose(true))
    std::cout << "\n make:" << gr2test.makeAdjMatrix();
    std::cout << "\n has:" << gr2test.hasAdjMatrix();
    std::cout << "\n del:" << gr2test.delAdjMatrix();
    gr2test.reserveAdjMatrix(100);
    std::cout << "\n allow:" << gr2test.allowedAdjMatrix();
    std::cout << "\n has:" << gr2test.hasAdjMatrix() << "\n";


//    sgb2=sgb;
    sgb=makeSubgraph(g,std::make_pair(Koala::stdChoose(g.getVertSet()),Koala::stdChoose(g.getEdgeSet())),std::make_pair(true,true));
    std::cout << std::boolalpha <<sgb.frozenNos().first << ' '<< sgb.frozenNos().second;

    std::cout << "\n****************\n\n";
    Koala::IO::writeGraphText(sgb,std::cout,Koala::IO::RG_EdgeList);
    sgb.echoose.set-=g.getEdge();
    sgb.echoose.set-=g.getEdgeLast();
    sgb.resetNos(std::make_pair(true,true));

    std::cout << "\n****************\n\n";
    Koala::IO::writeGraphText(sgb,std::cout,Koala::IO::RG_EdgeList);

    std::cout << "\n****************\n\n";

    g.clear();
    A=g.addVert(OpisV("A"));
    B=g.addVert(OpisV("B"));
    C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));

    g.addEdge(A,B,OpisE(1));
    g.addEdge(A,B,OpisE(1));
    g.addArc(A,B,OpisE(1));
    g.addArc(B,A,OpisE(1));

    g.addEdge(A,C,OpisE(1));
    g.addEdge(A,C,OpisE(1));
    g.addArc(A,C,OpisE(1));
    g.addArc(C,A,OpisE(1));

    g.addLoop(C,OpisE(1));
    g.addLoop(C,OpisE(1));

    Koala::Graph<OpisV,OpisE> gg;
    gg.addVert(OpisV("A"));
    gg.addVert(OpisV("B"));
    gg.addEdge(gg.getVert(),gg.getVertLast(),OpisE(1));
    gg.addArc(gg.getVert(),gg.getVertLast(),OpisE(1));


    Koala::IO::writeGraphText(g,std::cout,Koala::IO::RG_EdgeList);

    Koala::SimpleView<Koala::Graph<OpisV,OpisE> > gsimp(Koala::EdUndir,gg,std::make_pair(true,true));



    gsimp.refresh(Koala::EdDirOut);

//    gsimp.plug(g);
    gsimp=makeSimpleView(Koala::EdUndir,g);
    gsimp.resetNos();
    std::cout << "\n\n\n";
    Koala::IO::writeGraphText(gsimp,std::cout,Koala::IO::RG_EdgeList);


    return 0;
}
