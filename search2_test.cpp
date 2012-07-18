#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <cassert>
#include "Koala/graph/graph.h"
#include "Koala/base/def_struct.h"
#include "Koala/algorithm/search.h"


using namespace std;
using namespace Koala;



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
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,G,H,I,V,U,W,tabV[30];
Koala::Graph<OpisV,OpisE>::PEdge tabE[30],EE;


Koala::AssocArray<Koala::Graph<OpisV,OpisE>::PVertex,Koala::Blocks::VertData> vertCont;
Koala::AssocArray<Koala::Graph<OpisV,OpisE>::PEdge,int> edgeCont;

void searchTest_()
{   vertCont.clear();g.clear();
    A=g.addVert(OpisV("A"));

    B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));H=g.addVert(OpisV("H"));I=g.addVert(OpisV("I"));

    g.addEdge(A,B,OpisE(1));g.addEdge(A,C,OpisE(4));g.addEdge(B,D,OpisE(5));g.addEdge(B,C,OpisE(2));
    g.addEdge(C,D,OpisE(1));g.addEdge(C,E,OpisE(2),Koala::EdDirOut);g.addEdge(C,F,OpisE(1),Koala::EdDirOut);
    g.addEdge(D,G,OpisE(2),Koala::EdDirOut);
    g.addArch(G,H,OpisE(2));

    g.addArch(E,F,OpisE(2));

//    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
//        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}


void searchTest()
{
    vertCont.clear();
    g.clear();
    A=g.addVert(OpisV("A"));

    B=g.addVert(OpisV("B"));
    C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));
    E=g.addVert(OpisV("E"));
    F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));
    H=g.addVert(OpisV("H"));
    I=g.addVert(OpisV("I"));

    g.addEdge(A,B,OpisE(1));
    g.addEdge(A,C,OpisE(4));
    g.addEdge(B,D,OpisE(5));
    g.addEdge(B,C,OpisE(2));
    g.addEdge(C,D,OpisE(1));
    g.addEdge(C,E,OpisE(2));
    g.addEdge(C,F,OpisE(1));
    g.addEdge(D,G,OpisE(2));
    g.addEdge(G,H,OpisE(2));
    g.addEdge(G,H,OpisE(2));
    g.addLoop(G,OpisE(2));

    g.addEdge(E,F,OpisE(2));    //g.addArch(H,F,OpisE(2));
//    g.addEdge(H,I,OpisE(2));g.addEdge(D,I,OpisE(2));
//    g.addLoop(C);
//    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
//        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}


void searchTest2()
{   vertCont.clear();g.clear();
    A=g.addVert(OpisV("A"));

    B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));

    g.addEdge(A,B,OpisE(1));g.addEdge(A,C,OpisE(4));g.addEdge(A,D,OpisE(5));g.addEdge(B,C,OpisE(2));
    g.addEdge(B,D,OpisE(1));g.addEdge(C,D,OpisE(2));

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
}


int main() {

{

    searchTest();
    int licz;


    int comptab[2][100];

    std::cout<<std::endl;
    std::cout <<Koala::Blocks::split(g,blackHole,blackHole,Koala::SearchStructs::compStore(blackHole,blackHole),
                                   blackHole)<<std::endl;

    int compno = Koala::Blocks::splitComp(g,C,vertCont,edgeCont,Koala::SearchStructs::compStore(comptab[0],tabV),
                                   comptab[1]);
    std::cout << compno<<endl;
    for(int i=0;i<compno;i++)
    {   cout << comptab[0][i] << ':';
        for(int j=comptab[0][i];j<comptab[0][i+1];j++) cout << tabV[j]->info.name << " ";
        cout << endl;
    }
//    std::cout << endl;
//    for(int i=0;i<compno;i++)
//    {   cout << comptab[1][i] << ':';
//        for(int j=comptab[1][i];j<comptab[1][i+1];j++) cout << '(' << tabE[j]->getEnds().first->info.name << tabE[j]->getEnds().second->info.name << ')';
//        cout << endl;
//    }
    std::cout << std::endl;
    for(W=g.getVert();W;W=g.getVertNext(W))
        {   std::cout << W->info.name << "-" << vertCont[W].blockNo << ':';
            for(int j=0;j<vertCont[W].blockNo;j++) std::cout<< comptab[1][vertCont[W].firstBlock+j] << ' ';
            std::cout << std::endl;
        }
    std::cout << std::endl;
    for(EE=g.getEdge();EE;EE=g.getEdgeNext(EE))
        std::cout << " {" << EE->getEnds().first->info.name << EE->getEnds().second->info.name << "}:" << edgeCont[EE];
}
{
    searchTest();
    int licz;


    int comptab[2][100];
    Koala::SearchStructs::VectCompStore<Koala::Graph<OpisV,OpisE>::PVertex> vec;

    std::cout<<std::endl;

    int compno = Koala::Blocks::splitComp(g,C,vertCont,edgeCont,vec.input(),comptab[1]);
    std::cout << "\nsize:" <<vec.size()<<endl;
    for(int i=0;i<vec.size();i++)
    {
        for(int j=0;j<vec.size(i);j++) cout << vec[i][j]->info.name << " ";
        cout << endl;
    }
//    std::cout << endl;
//    for(int i=0;i<compno;i++)
//    {   cout << comptab[1][i] << ':';
//        for(int j=comptab[1][i];j<comptab[1][i+1];j++) cout << '(' << tabE[j]->getEnds().first->info.name << tabE[j]->getEnds().second->info.name << ')';
//        cout << endl;
//    }
    std::cout << std::endl;
    for(W=g.getVert();W;W=g.getVertNext(W))
        {   std::cout << W->info.name << "-" << vertCont[W].blockNo << ':';
            for(int j=0;j<vertCont[W].blockNo;j++) std::cout<< comptab[1][vertCont[W].firstBlock+j] << ' ';
            std::cout << std::endl;
        }
    std::cout << std::endl;
    for(EE=g.getEdge();EE;EE=g.getEdgeNext(EE))
        std::cout << " {" << EE->getEnds().first->info.name << EE->getEnds().second->info.name << "}:" << edgeCont[EE];

}

    std::cout << "\n\n------------\n\n";
    searchTest();//g.addEdge(I,A);
    Koala::SearchStructs::VectCompStore<Koala::Graph<OpisV,OpisE>::PVertex> vec;

    std::cout << vec.size() << std::endl;
    std::cout<<"\n"<<Koala::SCC::get(g,vec.input(),blackHole) << "\n\n";
    for(int i=0;i<vec.size();i++)
    {
        for(int j=0;j<vec.size(i);j++) cout << vec[i][j]->info.name << " ";
        cout << endl;
    }



    return 0;
}
