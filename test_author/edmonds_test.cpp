#include <iostream>
#include <string>
#include "Koala/base/defs.h"
#include "Koala/graph/graph.h"
#include "Koala/algorithm/matching.h"
#include "Koala/classes/detect.h"

struct OpisV;  // pewna struktura opisujaca moj wierzcholek
struct OpisE;  // struktura opisujaca krawedz

Koala::Set<int> rnd;

int random(int scale)
{
    int res;
    do {

        res=1+(rand()% scale);
    } while (rnd.isElement(res));
    rnd+=res;
    return res;
}

struct OpisV {
    OpisV(std::string aname="") : name(aname) {}
    std::string name;
};

struct OpisE {
    OpisE() {}
};

struct OpisE2 {
    int prior1,prior2;
    std::string name;
    OpisE2(int p1,int p2,std::string aname="") : prior1(p1), prior2(p2), name(aname) {}
};

struct Cmp {
    template <class PVertex,class PEdge>
    bool operator()(PVertex v, PEdge e1,PEdge e2)
    {
        assert(e1->isEnd(v) && e2->isEnd(v));
        int p1= (v==e1->getEnd1()) ? e1->info.prior1: e1->info.prior2;
        int p2= (v==e2->getEnd1()) ? e2->info.prior1: e2->info.prior2;
        return p1<p2;
    }
};

Koala::Graph<OpisV,OpisE> g;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,tabV[32];
Koala::Graph<OpisV,OpisE>::PEdge e,tabE[32];


Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PVertex,
            Koala::Matching::VertLabs<Koala::Graph<OpisV,OpisE> > > > vertCont;


void edmTest()
{   g.clear();vertCont.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));H=g.addVert(OpisV("H"));I=g.addVert(OpisV("I"));
    J=g.addVert(OpisV("J"));K=g.addVert(OpisV("K"));

    g.addEdge(A,B);g.addEdge(A,C);
    g.addEdge(B,D);g.addEdge(B,J);
    g.addEdge(C,E);
    g.addEdge(D,F);g.addEdge(D,G);
    g.addEdge(E,I);
    g.addEdge(F,G);
    g.addEdge(G,H);g.addEdge(G,I);
    g.addEdge(H,I);

//      krawedzie rownolegle
//    g.addEdge(A,B);g.addEdge(A,C);
//    g.addEdge(B,D);g.addEdge(B,J);
//    g.addEdge(C,E);
//    g.addEdge(D,F);g.addEdge(D,G);
//    g.addEdge(E,I);
//    g.addEdge(F,G);
//    g.addEdge(G,H);g.addEdge(G,I);
//    g.addEdge(H,I);
//    g.addEdge(A,B);g.addEdge(A,C);
//    g.addEdge(B,D);g.addEdge(B,J);
//    g.addEdge(C,E);
//    g.addEdge(D,F);g.addEdge(D,G);
//    g.addEdge(E,I);
//    g.addEdge(F,G);
//    g.addEdge(G,H);g.addEdge(G,I);
//    g.addEdge(H,I);
//

    for(int i=0;i<11;i++) tabV[i]=0;
    for(int i=0;i<12;i++) tabE[i]=0;
}



void edmTest2()
{   g.clear();vertCont.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));H=g.addVert(OpisV("H"));I=g.addVert(OpisV("I"));
    J=g.addVert(OpisV("J"));K=g.addVert(OpisV("K"));L=g.addVert(OpisV("L"));
    M=g.addVert(OpisV("M"));N=g.addVert(OpisV("N"));O=g.addVert(OpisV("O"));

    P=g.addVert(OpisV("P"));

    g.addEdge(A,B);g.addEdge(A,C);g.addEdge(B,D);g.addEdge(B,J);
    g.addEdge(C,E);g.addEdge(D,F);g.addEdge(D,G);g.addEdge(E,I);
    g.addEdge(F,G);g.addEdge(G,H);g.addEdge(G,I);g.addEdge(H,I);
    g.addEdge(K,B);g.addEdge(L,G);g.addEdge(M,G);g.addEdge(N,I);
    g.addLoop(P);

//    g.addEdge(A,B);g.addEdge(A,C);g.addEdge(B,D);g.addEdge(B,J);
//    g.addEdge(C,E);g.addEdge(D,F);g.addEdge(D,G);g.addEdge(E,I);
//    g.addEdge(F,G);g.addEdge(G,H);g.addEdge(G,I);g.addEdge(H,I);
//    g.addEdge(K,B);g.addEdge(L,G);g.addEdge(M,G);g.addEdge(N,I);
//    g.addEdge(A,B);g.addEdge(A,C);g.addEdge(B,D);g.addEdge(B,J);
//    g.addEdge(C,E);g.addEdge(D,F);g.addEdge(D,G);g.addEdge(E,I);
//    g.addEdge(F,G);g.addEdge(G,H);g.addEdge(G,I);g.addEdge(H,I);
//    g.addEdge(K,B);g.addEdge(L,G);g.addEdge(M,G);g.addEdge(N,I);


    for(int i=0;i<14;i++) tabV[i]=0;
    for(int i=0;i<16;i++) tabE[i]=0;
}


void HKTest()
{   g.clear();vertCont.clear();

    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));
    G=g.addVert(OpisV("G"));H=g.addVert(OpisV("H"));I=g.addVert(OpisV("I"));
    J=g.addVert(OpisV("J"));K=g.addVert(OpisV("K"));L=g.addVert(OpisV("L"));
    M=g.addVert(OpisV("M"));N=g.addVert(OpisV("N"));O=g.addVert(OpisV("O"));
    P=g.addVert(OpisV("P"));

    g.addEdge(A,F);g.addEdge(A,G);

    g.addEdge(B,F);
    g.addEdge(C,F);g.addEdge(C,G);g.addEdge(C,H);
    g.addEdge(D,F);g.addEdge(D,G);g.addEdge(D,H);g.addEdge(D,I);g.addEdge(D,J);
    g.addEdge(E,I);
    g.addEdge(K,J);
    g.addEdge(L,J);
    g.addEdge(M,N);

//    g.addEdge(B,F);
//    g.addEdge(C,F);g.addEdge(C,G);g.addEdge(C,H);
//    g.addEdge(D,F);g.addEdge(D,G);g.addEdge(D,H);g.addEdge(D,I);g.addEdge(D,J);
//    g.addEdge(E,I);
//    g.addEdge(K,J);
//    g.addEdge(L,J);
//    g.addEdge(M,N);
//    g.addEdge(B,F);
//    g.addEdge(C,F);g.addEdge(C,G);g.addEdge(C,H);
//    g.addEdge(D,F);g.addEdge(D,G);g.addEdge(D,H);g.addEdge(D,I);g.addEdge(D,J);
//    g.addEdge(E,I);
//    g.addEdge(K,J);
//    g.addEdge(L,J);
//    g.addEdge(M,N);


    for(int i=0;i<15;i++) tabV[i]=0;
    for(int i=0;i<15;i++) tabE[i]=0;
}


#include "main.hpp"
    int edgeCoverSize;
    edmTest();
    std::cout<< "\n1a. Chcemy skojarzenie maksymalne.\n";
    int matchSize = Koala::Matching::findMax (g,vertCont,tabE,-1);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";
    std::cout<< "\nDane w vertCont\n";
    for (Koala::Graph<OpisV,OpisE>::PVertex U = g.getVert(); U; U=g.getVertNext(U))
    {
        if (vertCont[U].vMatch)
          std::cout << "{" << U->info.name << "," << vertCont[U].vMatch->info.name << "}\n";
    }

    edmTest();
    std::cout<< "\n2. Teraz chcemy skojarzenie rozmiaru 3.\n";
    matchSize = Koala::Matching::findMax (g,vertCont,tabE,3);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";

    edmTest();
    std::cout<< "\n3. Teraz chcemy skojarzenie rozmiaru 4. Chcemy zobaczyc czy sie uda. Zadnych wynikow.\n";
    matchSize = Koala::Matching::findMax (g,vertCont,blackHole,4);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;

    edmTest();
    std::cout<< "\n4. Teraz chcemy skojarzenie rozmiaru 10.\n";
    matchSize = Koala::Matching::findMax (g,vertCont,blackHole,10);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;

    edmTest();
    std::cout<< "\n5. Teraz chcemy zapoczatkowac pierwsze skojarzenie |{A,C},{E,I},{F,G},{B,D}|=4 i je rozszerzyc do 10\n";
    std::vector<Koala::Graph<OpisV,OpisE>::PEdge> tab;
    tab.push_back(g.getEdge(A,C));tab.push_back(g.getEdge(E,I));
    tab.push_back(g.getEdge(F,G));tab.push_back(g.getEdge(B,D));
    matchSize = Koala::Matching::findMax (g,vertCont,tab.begin(),tab.end(),tabE,10);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";

    edmTest();
    std::cout<< "\n6. Teraz chcemy zapoczatkowac pierwsze skojarzenie |{A,C},{E,I},{F,G},{B,D}|=4 i je rozszerzyc do 3\n";
    std::vector<Koala::Graph<OpisV,OpisE>::PEdge> tab0;
    tab0.push_back(g.getEdge(A,C));tab0.push_back(g.getEdge(E,I));
    tab0.push_back(g.getEdge(F,G));tab0.push_back(g.getEdge(B,D));
    matchSize = Koala::Matching::findMax (g,vertCont,tab0.begin(),tab0.end(),tabE,3);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";
    edmTest();
    std::cout<< "\n7. Teraz chcemy zapoczatkowac pierwsze skojarzenie |{J,B},{F,G}|=2 i je rozszerzyc maksymalnie\n";
    std::vector<Koala::Graph<OpisV,OpisE>::PEdge> tab1;
    tab1.push_back(g.getEdge(J,B));
    tab1.push_back(g.getEdge(F,G));
    matchSize = Koala::Matching::findMax (g,vertCont,tab1.begin(),tab1.end(),tabE,-1);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";

    edmTest();
    std::cout<< "\n8. Teraz chcemy zapoczatkowac pierwsze skojarzenie |{A,C}|=1 i je rozszerzyc do 4\n";
    std::vector<Koala::Graph<OpisV,OpisE>::PEdge> tab3;
    tab3.push_back(g.getEdge(A,C));
    matchSize = Koala::Matching::findMax (g,vertCont,tab3.begin(),tab3.end(),tabE,4);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";
    std::cout<< "\nDane w vertCont\n";
    for (Koala::Graph<OpisV,OpisE>::PVertex U = g.getVert(); U; U=g.getVertNext(U))
    {
        if (vertCont[U].vMatch)
          std::cout << "{" << U->info.name << "," << vertCont[U].vMatch->info.name << "}\n";
    }

    edmTest();
    std::cout<< "\n9. Chcemy skojarzenie znalezione zachlannie - rozmiar 2\n";
    matchSize = Koala::Matching::greedy (g,vertCont,tabE,2);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";

    edmTest();
    std::cout<< "\n10. Chcemy skojarzenie znalezione zachlannie - rozmiar max mozliwy przy zachlannym algorytmie\n";
    matchSize = Koala::Matching::greedy (g,vertCont,tabE,-1);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";


    edmTest();
    std::cout<< "\n11. Chcemy skojarzenie znalezione zachlannie - rozmiar max mozliwy przy zachlannym algorytmie, ale dla naszych krawedzi\n";
    std::vector<Koala::Graph<OpisV,OpisE>::PEdge> tabEdIn;
    tabEdIn.push_back(g.getEdge(J,B));
    tabEdIn.push_back(g.getEdge(A,B));
    tabEdIn.push_back(g.getEdge(A,C));
    tabEdIn.push_back(g.getEdge(H,I));
    matchSize = Koala::Matching::greedy (g,vertCont,tabEdIn.begin(),tabEdIn.end(),tabE,-1);
    std::cout<< "Rozmiar znalezionego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Znalezione skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";
    edmTest();
    std::cout<< "\n12. Chcemy sprawdzic czy krawedzie podane na wejsciu tworza skojarzenie {J,B} {A,B} {A,C} {H,I}\n";
    std::vector<Koala::Graph<OpisV,OpisE>::PEdge> tabEdIn2;
    tabEdIn2.push_back(g.getEdge(J,B));
    tabEdIn2.push_back(g.getEdge(A,B));
    tabEdIn2.push_back(g.getEdge(A,C));
    tabEdIn2.push_back(g.getEdge(H,I));
    std::cout<< "Odp: " << Koala::Matching::test (g,tabEdIn2.begin(),tabEdIn2.end());


    edmTest();
    std::cout<< "\n13. Chcemy sprawdzic czy krawedzie podane na wejsciu tworza skojarzenie {J,B} {F,G} {A,C} {H,I}\n";
    std::vector<Koala::Graph<OpisV,OpisE>::PEdge> tabEdIn3;
    tabEdIn3.push_back(g.getEdge(J,B));
    tabEdIn3.push_back(g.getEdge(F,G));
    tabEdIn3.push_back(g.getEdge(A,C));
    tabEdIn3.push_back(g.getEdge(H,I));
    std::cout<< "Odp: " << Koala::Matching::test (g,tabEdIn3.begin(),tabEdIn3.end());

    std::cout << "\n\nPokrycia krawedziowe\n\n";

    edmTest2();
    std::cout<< "\n1a. Chcemy skojarzenie maksymalne.\n";
    matchSize = Koala::Matching::findMax (g,vertCont,tabE,-1);
    std::cout<< "Rozmiar najliczniejszego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Najliczniejsze skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";

    edmTest2();
    std::cout<< "\n2. Chcemy minimalne pokrycie krawedziowe dla tego samego grafu.\n";
    edgeCoverSize = Koala::Matching::minEdgeCover (g,/*vertCont,*/tabE);
    std::cout<< "Rozmiar minimalnego pokrycia krawedziowego: " << edgeCoverSize << std::endl;
    std::cout<< "Minimalne pokrycie krawedziowe-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<edgeCoverSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";

//    return 0;

    edmTest2();
    std::cout<< "\n3. To samo, ale na wyjsciu tylko rozmiar.\n";
    edgeCoverSize = Koala::Matching::minEdgeCover (g,/*vertCont,*/blackHole);
    std::cout<< "Rozmiar minimalnego pokrycia krawedziowego: " << edgeCoverSize << std::endl;

    std::cout << "\n\nGrafy dwudzielne\n\n";

    HKTest();
    std::cout<< "\n1a. Chcemy skojarzenie maksymalne.\n";
    matchSize = Koala::Matching::findMax (g,vertCont,tabE,-1);
    std::cout<< "Rozmiar najliczniejszego skojarzenia: " << matchSize << std::endl;
    std::cout<< "Najliczniejsze skojarzenie-strumien krawedzi nalezacych do skojarzenia" << std::endl;
    for(int i=0;i<matchSize;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}\n";
    std::cout<< "\nDane w vertCont\n";
    for (Koala::Graph<OpisV,OpisE>::PVertex U = g.getVert(); U; U=g.getVertNext(U))
    {
        if (vertCont[U].vMatch)
          std::cout << "{" << U->info.name << "," << vertCont[U].vMatch->info.name << "}\n";
    }


    HKTest();
    std::cout<< "\n6. Chcemy minimalne pokrycie wierzcholkowe dla tego samego grafu, podajemy podzial wierzcholkow na wejsciu.\n";
    edgeCoverSize = Koala::IsIt::Bipartite::minVertCover (g,tabV);
    std::cout<< "Rozmiar minimalnego pokrycia wierzcholkowego: " << edgeCoverSize << std::endl;
    std::cout<< "Minimalne pokrycie wierzcholkowe-strumien wierzcholkow nalezacych do pokrycia" << std::endl;
    for(int i=0;i<edgeCoverSize;i++) std::cout<< tabV[i]->info.name << " "; std::cout<< "\n";

    edgeCoverSize = Koala::IsIt::Bipartite::maxStable(g,tabV);
    std::cout<< "Rozmiar maksymalnego zbioru niezaleznego: " << edgeCoverSize << std::endl;
    std::cout<< "strumien wierzcholkow nalezacych do j.w." << std::endl;
    for(int i=0;i<edgeCoverSize;i++) std::cout<< tabV[i]->info.name << " "; std::cout<< "\n";

    Koala::Set<Koala::Graph<OpisV,OpisE>::PVertex> vset;
    Koala::IsIt::Bipartite::maxStable(g,setInserter(vset));
    for(e=g.getEdge();e;e=g.getEdgeNext(e))
        assert(!(vset.isElement(g.getEdgeEnd1(e))&&vset.isElement(g.getEdgeEnd2(e))));

    std:: cout << "\n\nStabilne\n\n";
    {   int scope=2;
//        Koala::Graph<char,OpisE2> g;
//        Koala::Graph<char,OpisE2>::PVertex A,B,C,D,E,F,tabV[11];
//        Koala::Graph<char,OpisE2>::PEdge e,tabE[12];
//
//        Koala::AssocTable<std::map<Koala::Graph<char,OpisE2>::PVertex,
//            Koala::StableMatching::VertLabs<Koala::Graph<char,OpisE2> > > > vertCont;
//
//        A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');E=g.addVert('E');
//        Koala::Graph<char,OpisE2>::PEdge
//            eac=g.addEdge(A,C,OpisE2(1,1,"eac")),
//            ebc=g.addEdge(B,C,OpisE2(2,3,"ebc")),
//            ebd=g.addEdge(B,D,OpisE2(1,3,"ebd")),
//            ebd2=g.addEdge(B,D,OpisE2(3,1,"ebd2")),
//            ex;
//
//        Koala::Graph<char,OpisE2>::PVertex vpart[]={A,B,E};
//
//        scope = Koala::StableMatching::bipartFind(g,vpart,vpart+3,Cmp(),vertCont,tabE);
//
//        for(int i=0;i<scope;i++) std::cout << '{' <<g.getEdgeEnd1(tabE[i])->info <<","<<g.getEdgeEnd2(tabE[i])->info
//            << ",\"" << tabE[i]->info.name << "\"} ";
//        std::cout << "\n"<< std::boolalpha << Koala::StableMatching::test(g,Cmp(),tabE,tabE+scope).first;

//        tabE[0]=ebd;
//        tabE[1]=ebd2;

//        std::cout << std::boolalpha << Koala::StableMatching::test(g,Cmp(),tabE,tabE+scope).first<<
//            std::endl << ((!Koala::StableMatching::test(g,Cmp(),tabE,tabE+scope).second) ? std::string("NULL") :
//                         Koala::StableMatching::test(g,Cmp(),tabE,tabE+scope).second->info.name);

    }
    {

        Koala::Graph<char,char> g;
        Koala::Graph<char,char>::PVertex A,B,C,D,E,F,tabV[11];
//        Koala::Graph<char,char>::PEdge eab,eac,edc,tabE[12];

        A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');

        Koala::Graph<char,char>::PEdge
            eab=g.addEdge(A,B),
            eac=g.addEdge(A,C),
            edc=g.addEdge(D,C),
            edc2=g.addEdge(D,C);
        assert(g.getEdgeEnd1(eab)==A && g.getEdgeEnd1(eac)==A);
        assert(g.getEdgeEnd2(eac)==C && g.getEdgeEnd2(edc)==C);

        Koala::AssocTable<std::map<Koala::Graph<char,char>::PEdge,std::pair<int,int>  >  > eCont;
        eCont[eab]=std::make_pair(1,0);
        eCont[eac]=std::make_pair(2,1);
        eCont[edc]=std::make_pair(1,2);
        eCont[edc2]=std::make_pair(4,5);
            Koala::StableMatching::CompEdgeCont<Koala::Graph<char,char> > c(g,eCont);
            std::cout << std::boolalpha;

            std::cout << c(A,eac,eab) << ' ' << c(C,eac,edc) << ' ' << c(C,edc2,edc);

        {

            Koala::StableMatching::CompEdgeCont<Koala::Graph<char,char> > c2=c;
            std::cout << c2(A,eac,eab) << ' ' << c2(C,eac,edc);
            Koala::StableMatching::CompEdgeCont<Koala::Graph<char,char> > c3(g,eCont);
            c3=c2;
            std::cout << c3(A,eac,eab) << ' ' << c3(C,eac,edc);
        }
            std::cout << c(A,eac,eab) << ' ' << c(C,eac,edc);
            std::cout << Koala::StableMatching::compEdgeCont(g,eCont)(A,eac,eab) << ' ' <<
                Koala::StableMatching::compEdgeCont(g,eCont)(C,eac,edc);
            c=Koala::StableMatching::compEdgeCont(g,eCont);
            std::cout << c(A,eac,eab) << ' ' << c(C,eac,edc);

            Koala::Graph<char,char>::PEdge tabA[]={eab,eac}, tabC[]={eac,edc,edc2},tabD[]={edc,edc2};
            Koala::AssocTable<std::map<Koala::Graph<char,char>::PVertex,std::pair<Koala::Graph<char,char>::PEdge*,Koala::Graph<char,char>::PEdge*>  >  >
                vCont;
            vCont[A].first=tabA;vCont[A].second=tabA+(sizeof(tabA)/sizeof(void*));
            vCont[C].first=tabC;vCont[C].second=tabC+(sizeof(tabC)/sizeof(void*));
            vCont[D].first=tabD;vCont[D].second=tabD+(sizeof(tabD)/sizeof(void*));
            c=Koala::StableMatching::compEdgeIters(g,vCont);
            std::cout << "\n"<< c(A,eac,eab) << ' ' << c(C,edc,eac) <<' ' << c(D,edc2,edc);

            std::cout<< "\n--------\n";
            Koala::Graph<char,char>::PEdge match[2]={edc2,eab};
            std::cout<<Koala::StableMatching::test(g,c,match,match+2).first;
            std::cout<< "\n----!!!----\n";
            g.clear();eCont.clear();
            const int n1=8,n2=11;
            Koala::Graph<char,char>::PVertex V1[n1],V2[n2];
            Koala::Graph<char,char>::PEdge etab[n1*n2];
            int ll=0;
            for(int i=0;i<n1;i++) V1[i]=g.addVert('A'+i);
            for(int i=0;i<n2;i++) V2[i]=g.addVert('A'+i+n1);
            for(int i=0;i<n1;i++) for(int j=0;j<n2;j++) if (rand()%100<70)
                eCont[etab[ll++]=g.addEdge(V1[i],V2[j])]=std::make_pair(random(10000),random(10000));
            for(int i=0;i<ll;i++) if (rand() & 1)
            eCont[g.addEdge(g.getEdgeEnd1(etab[i]),g.getEdgeEnd2(etab[i]))]
                =std::make_pair(random(10000),random(10000));
            c=Koala::StableMatching::compEdgeCont(g,eCont);
            Koala::AssocTable<std::map<Koala::Graph<char,char>::PVertex,
            Koala::Matching::VertLabs<Koala::Graph<char,char> > > > vertCont;

            std::cout << g.getVertNo() << ' ' << g.getEdgeNo() <<'\n';
            std::cout << (matchSize = Koala::StableMatching::bipartFind (g,V1,V1+n1,c,blackHole,etab)) << "\n";
            std::cout << Koala::Matching::test(g,etab,etab+matchSize)<< ' ';
            std::cout << Koala::StableMatching::test(g,c,etab,etab+matchSize).first;


    }

    return 0;
}
