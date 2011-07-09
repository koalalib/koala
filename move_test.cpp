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

struct InfoE1 {
    int a;
    string name;

    InfoE1(int n, string aname) : a(n), name(aname) {}
    InfoE1() {}
};



int main() {


    Graph<InfoV1,InfoE1> g1;


    g1.makeAdjMatrix();

    Vertex<InfoV1,InfoE1> *beg,*end,*A=g1.addVert(InfoV1(0,"A"));
    Vertex<InfoV1,InfoE1> *B=g1.addVert(InfoV1(1,"B"));
    Vertex<InfoV1,InfoE1> *C=g1.addVert(InfoV1(2,"C"));
    Vertex<InfoV1,InfoE1> *D=g1.addVert(InfoV1(2,"D"));
    Edge<InfoV1,InfoE1> *e,*en,*e2,*en2,*ee;

    g1.addArch(B,D,InfoE1(0,"BD"));

    g1.addEdge(A,B,InfoE1(0,"AB1"));
    g1.addEdge(A,B,InfoE1(0,"AB2"));
//    ee=g1.addArch(A,B,InfoE1(0,"A->B1"));
    //g1.addArch(A,B,InfoE1(0,"A->B2"));
//    g1.addArch(B,A,InfoE1(0,"B->A1"));
    g1.addArch(B,A,InfoE1(0,"B->A2"));
    g1.addLoop(A,InfoE1(0,"A1"));
    g1.addLoop(A,InfoE1(0,"A2"));

//    g1.del(ee);
    g1.delAdjMatrix();

    EdgeDirection mask=0;
    mask|=EdDirIn;
    mask|=EdDirOut;
    mask|=EdUndir;
    mask|=EdLoop;

    EdgeDirection mask2=0;
//    mask2|=EdDirIn;
    mask2|=EdDirOut;
    mask2|=EdUndir;
    mask2|=EdLoop;

    mask2=mask;


    beg=A;end=B;

    e=g1.getEdge(beg);
//    g1.makeAdjMatrix(); e2=g1.getEdge(beg,end,mask);g1.delAdjMatrix();
    while(e)
    {   e2=e;
        cout << endl << ((e)?e->info.name : string("NULL")) << " " << ((e2)?e2->info.name : string("NULL"));
        en=g1.getEdgeNext(beg,e,mask2);
//        g1.makeAdjMatrix(); en2=g1.getEdgeNext(beg,end,e,mask2);g1.delAdjMatrix();
        e=en;e2=en;
    }
    cout << endl << ((e==en) ? "=" : "!");
    cout << "\n*Back:";

    e=g1.getEdgeLast(beg,mask);
//    g1.makeAdjMatrix(); e2=g1.getEdgeLast(beg,end,mask);g1.delAdjMatrix();
    while(e)
    {   e2=e;
        cout << endl << ((e)?e->info.name : string("NULL")) << " " << ((e2)?e2->info.name : string("NULL"));
        en=g1.getEdgePrev(beg,e,mask2);
//        g1.makeAdjMatrix(); en2=g1.getEdgePrev(beg,end,e,mask2);g1.delAdjMatrix();
        e=en;e2=en;
    }
    cout << endl << ((e==en) ? "=" : "!");

    cout << "\n Pary:";

    e=g1.getEdge(beg,end,mask);g1.makeAdjMatrix(); e2=g1.getEdge(beg,end,mask);g1.delAdjMatrix();
    while(e)
    {
        cout << endl << ((e)?e->info.name : string("NULL")) << " " << ((e2)?e2->info.name : string("NULL"));
        en=g1.getEdgeNext(beg,end,e,mask2);
        g1.makeAdjMatrix(); en2=g1.getEdgeNext(beg,end,e,mask2);g1.delAdjMatrix();
        e=en;e2=en2;
    }
    cout << endl << ((e==en) ? "=" : "!");
    cout << "\n*Back:";

    e=g1.getEdgeLast(beg,end,mask);g1.makeAdjMatrix(); e2=g1.getEdgeLast(beg,end,mask);g1.delAdjMatrix();
    while(e)
    {
        cout << endl << ((e)?e->info.name : string("NULL")) << " " << ((e2)?e2->info.name : string("NULL"));
        en=g1.getEdgePrev(beg,end,e,mask2);
        g1.makeAdjMatrix(); en2=g1.getEdgePrev(beg,end,e,mask2);g1.delAdjMatrix();
        e=en;e2=en2;
    }
    cout << endl << ((e==en) ? "=" : "!");

    cout << "\n Koniec";

    cout << "\n Odwr:";
    beg=B;end=A;

    e=g1.getEdge(beg,end,mask);g1.makeAdjMatrix(); e2=g1.getEdge(beg,end,mask);g1.delAdjMatrix();
    while(e)
    {
        cout << endl << ((e)?e->info.name : string("NULL")) << " " << ((e2)?e2->info.name : string("NULL"));
        en=g1.getEdgeNext(beg,end,e,mask2);
        g1.makeAdjMatrix(); en2=g1.getEdgeNext(beg,end,e,mask2);g1.delAdjMatrix();
        e=en;e2=en2;
    }
    cout << endl << ((e==en) ? "=" : "!");
    cout << "\n*Back:";

    e=g1.getEdgeLast(beg,end,mask);g1.makeAdjMatrix(); e2=g1.getEdgeLast(beg,end,mask);g1.delAdjMatrix();
    while(e)
    {
        cout << endl << ((e)?e->info.name : string("NULL")) << " " << ((e2)?e2->info.name : string("NULL"));
        en=g1.getEdgePrev(beg,end,e,mask2);
        g1.makeAdjMatrix(); en2=g1.getEdgePrev(beg,end,e,mask2);g1.delAdjMatrix();
        e=en;e2=en2;
    }
    cout << endl << ((e==en) ? "=" : "!");

    cout << "\n Koniec\n";

//    cout << g1.glue(g1.getClNeighSet(B),true,A);

}

