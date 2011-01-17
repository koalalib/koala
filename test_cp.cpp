#include <iostream>
#include <string>
#include "Koala/container/assoctab.h"
#include "Koala/graph/graph.h"
#include "Koala/base/def_struct.h"

using namespace std;
using namespace Koala;

struct InfoV1 {
    int a;
    string name;

    InfoV1(int n, string aname) : a(n), name(aname) {}
    InfoV1(){}
};

struct InfoV2 {
    int b;
    string napis;

    InfoV2(int n, string aname) : b(n), napis(aname) {}
    InfoV2(){}
};

InfoV2 vtrans(InfoV1 arg) { return InfoV2(0,arg.name); }

InfoV2 (*vt)(InfoV1)=vtrans;

struct InfoE1 {
    int a;
    string name;

    InfoE1(int n, string aname) : a(n), name(aname) {}
    InfoE1() {}
};

struct InfoE2 {
    int b;
    string napis;

    InfoE2(int n, string aname) : b(n), napis(aname) {}
    InfoE2(){}
};

InfoE2 vtrans(InfoE1 arg) { return InfoE2(0,arg.name); }

int main() {

    Graph<InfoV1,InfoE1> g1;
    Graph<InfoV2,InfoE2> g2,g2p;

    map<Vertex<InfoV1,InfoE1>*,Vertex<InfoV2,InfoE2>* > m12;
    AssocTabInterface<map<Vertex<InfoV1,InfoE1>*,Vertex<InfoV2,InfoE2>* > > am12=m12;
    map<Vertex<InfoV2,InfoE2>*,Vertex<InfoV1,InfoE1>* > m21;
    AssocTabInterface< map<Vertex<InfoV2,InfoE2>*,Vertex<InfoV1,InfoE1>* > > am21=m21;

    map<Edge<InfoV1,InfoE1>*,Edge<InfoV2,InfoE2>* > n12;
    AssocTabInterface<map<Edge<InfoV1,InfoE1>*,Edge<InfoV2,InfoE2>* > > an12=n12;
    map<Edge<InfoV2,InfoE2>*,Edge<InfoV1,InfoE1>* > n21;
    AssocTabInterface< map<Edge<InfoV2,InfoE2>*,Edge<InfoV1,InfoE1>* > > an21=n21;

    Vertex<InfoV1,InfoE1> *A=g1.addVert(InfoV1(0,"A"));
    Vertex<InfoV1,InfoE1> *B=g1.addVert(InfoV1(1,"B"));
    Vertex<InfoV1,InfoE1> *C=g1.addVert(InfoV1(2,"C"));
    Edge<InfoV1,InfoE1> *e=g1.addEdge(C,B,InfoE1(0,"BC"));
    g2.copy(g1,make_pair(fieldChoose(&InfoV1::a),stdChoose(true)),make_pair(stdCast(vt),stdCast(false))
            , make_pair(stdLink(am21,am12),stdLink(an21,an12)));


    for(Vertex<InfoV2,InfoE2>* p=g2.getVert();p;p=g2.getVertNext(p))
        cout << ((void*)p) << "  " << p->info.napis << "  " << m21[p]->info.name<<  endl;

    for(Vertex<InfoV1,InfoE1>* p=g1.getVert();p;p=g1.getVertNext(p))
        cout <<  ((m12[p]) ? m12[p]->info.napis : " null" )<<  endl;

    cout << n21[g2.getEdge()]->info.name;
    cout << n12.size();
    g2p=g2;
    cout << endl << g2p.getVertNo();
    g1.copy(g1);
    cout << endl << g1.getVertNo();


}

