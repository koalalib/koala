#include <iostream>
#include <string>
//#include <random>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/mis.h"

#define MAX_OUT_SIZE 64

using namespace std;

#define RandGenType
//std::default_random_engine

Koala::StdRandGen<> stdgen;
//std::default_random_engine stdgen;

struct OpisV;  // pewna struktura opisujaca moj wierzchlek
struct OpisE;  // struktura opisujaca krawedz

struct OpisV {
    OpisV(std::string aname="", int aweight=0) :
        name(aname),
		weight(aweight) {}

    std::string name;
    int weight;
};

struct OpisE {
    OpisE(int len=0) : dlugosc(len) {}
    int dlugosc;
};

Koala::Graph<OpisV,OpisE> g,gneg;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,G,H,tabV[MAX_OUT_SIZE];
Koala::Graph<OpisV,OpisE>::PVertex I,J,K,L,M,N,O,P,R,S, Q,V,T,U,W,X,Y,Z;

Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PVertex,double> > vertCont,vertContNeg;

void clearOutput() {
    for(unsigned i=0; i<MAX_OUT_SIZE; ++i) {
        tabV[i] = NULL;
    }
}

int main(void) {

    unsigned MaxStable = 0;
    unsigned result = 0;
    string isIndependentStr;
    string isMaxIndependentStr;

    // -------------------------------------------------------------------
    std::cout << "Graph1" << std::endl;
    // -------------------------------------------------------------------
    g.clear();
    A=g.addVert(OpisV("A",1));
    B=g.addVert(OpisV("B",1));
    C=g.addVert(OpisV("C",1));
    D=g.addVert(OpisV("D",1));
    E=g.addVert(OpisV("E",1));
    F=g.addVert(OpisV("F",1));

    G=g.addVert(OpisV("G",1));
    H=g.addVert(OpisV("H",1));
    I=g.addVert(OpisV("I",1));
    J=g.addVert(OpisV("J",1));
    K=g.addVert(OpisV("K",1));
    L=g.addVert(OpisV("L",1));

    M=g.addVert(OpisV("M",1));
    N=g.addVert(OpisV("N",1));
    O=g.addVert(OpisV("O",1));
    P=g.addVert(OpisV("P",1));
    R=g.addVert(OpisV("R",1));
    S=g.addVert(OpisV("S",1));


    g.addEdge(A,B,OpisE(1));
    g.addEdge(A,C,OpisE(1));
    g.addEdge(A,D,OpisE(1));
    g.addEdge(A,E,OpisE(1));
    g.addEdge(A,F,OpisE(1));

    g.addEdge(G,H,OpisE(1));
    g.addEdge(G,I,OpisE(1));
    g.addEdge(G,J,OpisE(1));
    g.addEdge(G,K,OpisE(1));
    g.addEdge(G,L,OpisE(1));

    g.addEdge(M,N,OpisE(1));
    g.addEdge(M,O,OpisE(1));
    g.addEdge(M,P,OpisE(1));
    g.addEdge(M,R,OpisE(1));
    g.addEdge(M,S,OpisE(1));


    g.addEdge(B,C,OpisE(1));
    g.addEdge(C,D,OpisE(1));
    g.addEdge(D,E,OpisE(1));
    g.addEdge(E,F,OpisE(1));
    g.addEdge(B,F,OpisE(1));

    g.addEdge(H,I,OpisE(1));
    g.addEdge(I,J,OpisE(1));
    g.addEdge(J,K,OpisE(1));
    g.addEdge(K,L,OpisE(1));
    g.addEdge(H,L,OpisE(1));

    g.addEdge(N,O,OpisE(1));
    g.addEdge(O,P,OpisE(1));
    g.addEdge(P,R,OpisE(1));
    g.addEdge(R,S,OpisE(1));
    g.addEdge(N,S,OpisE(1));


    g.addEdge(B,H,OpisE(1));
    g.addEdge(H,N,OpisE(1));

    g.addEdge(C,I,OpisE(1));
    g.addEdge(I,O,OpisE(1));

    g.addEdge(D,J,OpisE(1));
    g.addEdge(J,P,OpisE(1));

    g.addEdge(E,K,OpisE(1));
    g.addEdge(K,R,OpisE(1));

    g.addEdge(F,L,OpisE(1));
    g.addEdge(L,S,OpisE(1));
    // -------------------------------------------------------------------
    // Exact:

//    std::cout << "Exact: " << std::endl;
//    MaxStable = Koala::MaxStable::findMax(g, tabV);
//    std::cout << "wierzcholkow: " << MaxStable << std::endl;
//    for(unsigned i = 0; i < MaxStable; i++) {
//        std::cout << tabV[i]->info.name << ", ";
//    } std::cout << std::endl;
//
//    // TODO: try with set output
//    // TODO: make (it, number) functions?
//    isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[MaxStable])) ? "YES" : "NO";
//    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[MaxStable])) ? "YES" : "NO";
//    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
//    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
//
//    std::cout << std::endl; clearOutput();

    // -------------------------------------------------------------------
    std::cout << std::endl << "Graph2:" << std::endl;
    // -------------------------------------------------------------------

    g.clear();
    A=g.addVert(OpisV("A",1));
    B=g.addVert(OpisV("B",1));
    C=g.addVert(OpisV("C",1));
    D=g.addVert(OpisV("D",1));
    E=g.addVert(OpisV("E",1));
    F=g.addVert(OpisV("F",1));
	G=g.addVert(OpisV("G",2));
	H=g.addVert(OpisV("H",3));

    g.addEdge(A,B,OpisE(1));
    g.addEdge(B,D,OpisE(5));
    g.addEdge(A,C,OpisE(4));
    g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));
    g.addEdge(B,C,OpisE(2));
    g.addEdge(D,E,OpisE(1));
    g.addEdge(F,D,OpisE(2));
	g.addEdge(G,H,OpisE(1));

	    gneg=g;gneg.neg(Koala::EdUndir);

    for(Koala::Graph<OpisV,OpisE>::PVertex vPt=g.getVert();vPt;vPt=g.getVertNext(vPt))
        vertCont[vPt] = vPt->info.weight;
    for(Koala::Graph<OpisV,OpisE>::PVertex vPt=gneg.getVert();vPt;vPt=gneg.getVertNext(vPt))
        vertContNeg[vPt] = vPt->info.weight;


    // -------------------------------------------------------------------
    // Exact:

//    std::cout << "Exact: " << std::endl;
//    MaxStable = Koala::MaxStable::findMax(g, tabV);
//    std::cout << "wierzcholkow: " << MaxStable << std::endl;
//    for(unsigned i = 0; i < MaxStable; i++) {
//        std::cout << tabV[i]->info.name << ", ";
//    } std::cout << std::endl;
//
//    isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[MaxStable])) ? "YES" : "NO";
//    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[MaxStable])) ? "YES" : "NO";
//    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
//    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
//
//    std::cout << std::endl; clearOutput();



    // -------------------------------------------------------------------
    // WMIN:

    std::cout << "WMIN: " << std::endl;
    result = Koala::MaxStableHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::GMin< RandGenType >(stdgen),
        vertCont/*NULL*/);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
   isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMin(
        gneg,
        tabV,
        Koala::MaxStableStrategy::GMin< RandGenType >(stdgen),
        vertContNeg/*NULL*/);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl << "--\n";
    clearOutput();

    result = Koala::MaxStableHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::GWMin<RandGenType>(stdgen),
        vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
   isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMin(
        gneg,
        tabV,
        Koala::MaxStableStrategy::GWMin<RandGenType>(stdgen),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl;
    clearOutput();

    result = Koala::MinVertCoverHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::GWMin<RandGenType>(stdgen),
        vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
   isIndependentStr = (Koala::MinVertCover::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MinVertCover::testMin(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs cover set: " << isIndependentStr << std::endl;
    std::cout << "\tIs min cover set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl << "--\n"; clearOutput();

    result = Koala::MaxStableHeur::getWMin(
            g,
            tabV,
            Koala::MaxStableStrategy::GWMin2<RandGenType>(stdgen),
            vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl; isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMin(
        gneg,
        tabV,
        Koala::MaxStableStrategy::GWMin2<RandGenType>(stdgen),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl << "--\n";
    clearOutput();

    result = Koala::MaxStableHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::GGWMin<RandGenType>(stdgen),
        vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMin(
        gneg,
        tabV,
        Koala::MaxStableStrategy::GGWMin<RandGenType>(stdgen),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl << "--\n";

    result = Koala::MaxStableHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::First(),
        blackHole);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMin(
        gneg,
        tabV,
        Koala::MaxStableStrategy::First(),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl << "--\n";

    result = Koala::MaxCliqueHeur::getWMin(
        gneg,
        tabV,
        Koala::MaxStableStrategy::First(),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl << "--\n";

    result = Koala::MaxStableHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::Rand<RandGenType >(stdgen),
        vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl; clearOutput();



    // -------------------------------------------------------------------
    // WMAX:

    std::cout << "WMAX: " << std::endl;
    result = Koala::MaxStableHeur::getWMax(
        g,
        tabV,
        Koala::MaxStableStrategy::GMax<RandGenType>(stdgen),
        blackHole/*NULL*/);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
   isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMax(
        gneg,
        tabV,
        Koala::MaxStableStrategy::GMax<RandGenType>(stdgen),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl << "--\n";
    clearOutput();

    result = Koala::MaxStableHeur::getWMax(
        g,
        tabV,
        Koala::MaxStableStrategy::GWMax<RandGenType>(stdgen),
        vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
   isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMax(
        gneg,
        tabV,
        Koala::MaxStableStrategy::GWMax<RandGenType>(stdgen),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl << "--\n";
    clearOutput();

    result = Koala::MaxStableHeur::getWMax(
        g,
        tabV,
        Koala::MaxStableStrategy::GGWMax<RandGenType>(stdgen),
        vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;    isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl; clearOutput();
    result = Koala::MaxCliqueHeur::getWMax(
        gneg,
        tabV,
        Koala::MaxStableStrategy::GGWMax<RandGenType>(stdgen),
        vertContNeg);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxClique::test(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxClique::testMax(gneg, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl ;
    clearOutput();
    result = Koala::MinVertCoverHeur::getWMax(
        g,
        tabV,
        Koala::MaxStableStrategy::GGWMax<RandGenType>(stdgen),
        vertCont);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
   isIndependentStr = (Koala::MinVertCover::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MinVertCover::testMin(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs cover set: " << isIndependentStr << std::endl;
    std::cout << "\tIs min cover set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl << "--\n"; clearOutput();


    // -------------------------------------------------------------------
    std::cout << "Graph3" << std::endl;
    // -------------------------------------------------------------------
    g.clear();
    A=g.addVert(OpisV("A",1));
    B=g.addVert(OpisV("B",1));
    C=g.addVert(OpisV("C",1));
    D=g.addVert(OpisV("D",1));
    E=g.addVert(OpisV("E",1));
    F=g.addVert(OpisV("F",1));

    G=g.addVert(OpisV("G",1));
    H=g.addVert(OpisV("H",1));
    I=g.addVert(OpisV("I",1));
    J=g.addVert(OpisV("J",1));
    K=g.addVert(OpisV("K",1));
    L=g.addVert(OpisV("L",1));

    M=g.addVert(OpisV("M",1));
    N=g.addVert(OpisV("N",1));
    O=g.addVert(OpisV("O",1));
    P=g.addVert(OpisV("P",1));
    R=g.addVert(OpisV("R",1));
    S=g.addVert(OpisV("S",1));

    Q=g.addVert(OpisV("Q",1));
    V=g.addVert(OpisV("V",1));
    T=g.addVert(OpisV("T",1));
    U=g.addVert(OpisV("U",1));
    W=g.addVert(OpisV("W",1));
    X=g.addVert(OpisV("X",1));
    Y=g.addVert(OpisV("Y",1));
    Z=g.addVert(OpisV("Z",1));


    g.addEdge(A,B,OpisE(1));
    g.addEdge(A,E,OpisE(1));
    g.addEdge(A,F,OpisE(1));
    g.addEdge(A,N,OpisE(1));
    g.addEdge(A,P,OpisE(1));
    g.addEdge(A,Y,OpisE(1));
    g.addEdge(A,Z,OpisE(1));

    g.addEdge(B,D,OpisE(1));
    g.addEdge(B,E,OpisE(1));
    g.addEdge(B,S,OpisE(1));
    g.addEdge(B,Q,OpisE(1));

    g.addEdge(C,D,OpisE(1));
    g.addEdge(C,F,OpisE(1));
    g.addEdge(C,G,OpisE(1));
    g.addEdge(C,Z,OpisE(1));

    g.addEdge(D,F,OpisE(1));
    g.addEdge(D,H,OpisE(1));
    g.addEdge(D,I,OpisE(1));
    g.addEdge(D,J,OpisE(1));
    g.addEdge(D,M,OpisE(1));
    g.addEdge(D,N,OpisE(1));
    g.addEdge(D,Q,OpisE(1));
    g.addEdge(D,T,OpisE(1));

    g.addEdge(E,Q,OpisE(1));
    g.addEdge(E,Z,OpisE(1));

    g.addEdge(F,G,OpisE(1));
    g.addEdge(F,J,OpisE(1));
    g.addEdge(F,K,OpisE(1));
    g.addEdge(F,P,OpisE(1));
    g.addEdge(F,Q,OpisE(1));
    g.addEdge(F,Y,OpisE(1));
    g.addEdge(F,Z,OpisE(1));

    g.addEdge(G,H,OpisE(1));
    g.addEdge(G,I,OpisE(1));
    g.addEdge(G,J,OpisE(1));
    g.addEdge(G,K,OpisE(1));
    g.addEdge(G,L,OpisE(1));

    g.addEdge(H,J,OpisE(1));
    g.addEdge(H,N,OpisE(1));

    g.addEdge(J,N,OpisE(1));
    g.addEdge(J,O,OpisE(1));
    g.addEdge(J,Y,OpisE(1));

    g.addEdge(K,Q,OpisE(1));

    g.addEdge(L,V,OpisE(1));
    g.addEdge(L,U,OpisE(1));
    g.addEdge(L,W,OpisE(1));
    g.addEdge(L,Z,OpisE(1));

    g.addEdge(M,N,OpisE(1));
    g.addEdge(M,S,OpisE(1));

    g.addEdge(O,Q,OpisE(1));

    g.addEdge(P,S,OpisE(1));
    g.addEdge(P,Z,OpisE(1));

    g.addEdge(R,S,OpisE(1));
    g.addEdge(R,T,OpisE(1));
    g.addEdge(R,W,OpisE(1));

    g.addEdge(S,X,OpisE(1));

    g.addEdge(T,U,OpisE(1));
    g.addEdge(T,W,OpisE(1));

    g.addEdge(Q,Z,OpisE(1));

    g.addEdge(Y,Z,OpisE(1));


    // -------------------------------------------------------------------
    // Exact:

//    std::cout << "Exact: " << std::endl;
//    MaxStable = Koala::MaxStable::findMax(g, tabV);
//    std::cout << "wierzcholkow: " << MaxStable << std::endl;
//    for(unsigned i = 0; i < MaxStable; i++) {
//        std::cout << tabV[i]->info.name << ", ";
//    } std::cout << std::endl;
//    isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[MaxStable])) ? "YES" : "NO";
//    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[MaxStable])) ? "YES" : "NO";
//    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
//    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
//    std::cout << std::endl; clearOutput();

    std::cout << "WMIN: " << std::endl;
    result = Koala::MaxStableHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::GMin<RandGenType>(stdgen),
        vertCont/*NULL*/);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
    std::cout << std::endl; clearOutput();
    g.neg(Koala::EdUndir);
    result = Koala::MaxCliqueHeur::getWMin(
        g,
        tabV,
        Koala::MaxStableStrategy::GMin<RandGenType>(stdgen),
        vertCont/*NULL*/);
    std::cout << "wierzcholkow: " << result << std::endl;
    for(unsigned int i=0; i<result; ++i) {
        std::cout << tabV[i]->info.name << ", ";
    } std::cout << std::endl;
    isIndependentStr = (Koala::MaxCliqueHeur::test(g, tabV, &tabV[result])) ? "YES" : "NO";
    isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
    std::cout << "\tIs clique set: " << isIndependentStr << std::endl;
    std::cout << "\tIs maximal clique set: " << isMaxIndependentStr << std::endl;



    {
            std::cout << "\n\t***************\n";

        g.clear();
        A=g.addVert(OpisV("A",1));
        B=g.addVert(OpisV("B",1));
        C=g.addVert(OpisV("C",1));
        D=g.addVert(OpisV("D",1));

        g.addLoop(A,OpisE(1));
        g.addLoop(D,OpisE(1));
        g.addLoop(B,OpisE(1));
//        g.addLoop(C,OpisE(1));
//        g.addEdge(A,B,OpisE(1));
//        g.addEdge(C,B,OpisE(1));
//        g.addEdge(C,D,OpisE(1));

        std::cout << "WMIN: " << std::endl;

//        result = Koala::MaxStableHeur::getWMax(
//            g,
//            tabV,
//            Koala::MaxStableStrategy::GGWMax<RandGenType>(stdgen),
////            Koala::MaxStableStrategy::GMin<RandGenType>(stdgen),
//            vertCont/*NULL*/);

        result = Koala::MaxStable::findMax(
            g,
            tabV,1);


        std::cout << "wierzcholkow: " << result << std::endl;
        for(unsigned int i=0; i<result; ++i) {
            std::cout << tabV[i]->info.name << ", ";
        } std::cout << std::endl;
        isIndependentStr = (Koala::MaxStable::test(g, tabV, &tabV[result])) ? "YES" : "NO";
        isMaxIndependentStr = (Koala::MaxStable::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
        std::cout << "\tIs independent set: " << isIndependentStr << std::endl;
        std::cout << "\tIs maximal independent set: " << isMaxIndependentStr << std::endl;
        std::cout << std::endl; clearOutput();
    }

    {
            std::cout << "\n\t!!!!!!!!!!!!!!!\n";

        g.clear();
        A=g.addVert(OpisV("A",1));
        B=g.addVert(OpisV("B",1));
        C=g.addVert(OpisV("C",1));
        D=g.addVert(OpisV("D",1));

        g.addLoop(C,OpisE(1));
        g.addEdge(D,B,OpisE(1));
        g.addEdge(A,B,OpisE(1));
        g.addEdge(A,B,OpisE(1));
        g.addEdge(C,B,OpisE(1));
        g.addEdge(C,D,OpisE(1));

        std::cout << "WMIN: " << std::endl;

//        result = Koala::MaxStableHeur::getWMax(
//            g,
//            tabV,
//            Koala::MaxStableStrategy::GGWMax<RandGenType>(stdgen),
////            Koala::MaxStableStrategy::GMin<RandGenType>(stdgen),
//            vertCont/*NULL*/);

        result = Koala::MaxClique::findMax(
            g,
            tabV);

//        result = 2;
////        tabV[0]=C;
//        tabV[1]=B;
//        tabV[0]=D;


        std::cout << "wierzcholkow: " << result << std::endl;
        for(unsigned int i=0; i<result; ++i) {
            std::cout << tabV[i]->info.name << ", ";
        } std::cout << std::endl;
        isIndependentStr = (Koala::MaxCliqueHeur::test(g, tabV, &tabV[result])) ? "YES" : "NO";
        isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
        std::cout << "\tIs clique: " << isIndependentStr << std::endl;
        std::cout << "\tIs maximal clique: " << isMaxIndependentStr << std::endl;
        std::cout << std::endl; clearOutput();

        result = Koala::MaxClique::findSome(
            g,
            tabV,1);

        std::cout << "wierzcholkow: " << result << std::endl;
        for(unsigned int i=0; i<result; ++i) {
            std::cout << tabV[i]->info.name << ", ";
        } std::cout << std::endl;
        isIndependentStr = (Koala::MaxCliqueHeur::test(g, tabV, &tabV[result])) ? "YES" : "NO";
        isMaxIndependentStr = (Koala::MaxCliqueHeur::testMax(g, tabV, &tabV[result])) ? "YES" : "NO";
        std::cout << "\tIs clique: " << isIndependentStr << std::endl;
        std::cout << "\tIs maximal clique: " << isMaxIndependentStr << std::endl;
        std::cout << std::endl; clearOutput();
    }

    {
        std::cout << "\n\tcccccccccccccccccccc\n";

        g.clear();
        A=g.addVert(OpisV("A",1));
        B=g.addVert(OpisV("B",1));
        C=g.addVert(OpisV("C",1));
        D=g.addVert(OpisV("D",1));

//        g.addLoop(B,OpisE(1));
//        g.addLoop(D,OpisE(1));
        g.addEdge(A,B,OpisE(1));
        g.addEdge(C,B,OpisE(1));
        g.addEdge(C,D,OpisE(1));

        result=0;
        tabV[result]=A;  result++;
        tabV[result]=D;  result++;
        tabV[result]=C;  result++;
        tabV[result]=B;  result++;
//                result=0;
//                g.clearEdges();
//                g.addLoop(A,OpisE(1));
//                g.addLoop(B,OpisE(1));

        std::cout << std::boolalpha <<"test: " << Koala::MinVertCover::test(g, tabV, tabV+result)
//        << "\n"
            << "\ntestMax: " << Koala::MinVertCover::testMin(g, tabV, tabV+result)
            << "\n";

        clearOutput();
//        g.clearEdges();

        result = Koala::MinVertCover::findMin(
            g,
            tabV);

        std::cout << "wierzcholkow: " << result << std::endl;
        for(unsigned int i=0; i<result; ++i) {
            std::cout << tabV[i]->info.name << ", ";
        } std::cout << std::endl;
        isIndependentStr = (Koala::MinVertCover::test(g, tabV, &tabV[result])) ? "YES" : "NO";
        isMaxIndependentStr = (Koala::MinVertCover::testMin(g, tabV, &tabV[result])) ? "YES" : "NO";
        std::cout << "\tIs cover " << isIndependentStr << std::endl;
        std::cout << "\tIs minimal cover " << isMaxIndependentStr << std::endl;
        std::cout << std::endl; clearOutput();

    }

    Koala::StdRandGen<> stdgen2=stdgen;
    stdgen2=stdgen;
}





