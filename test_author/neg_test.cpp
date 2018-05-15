#include <iostream>
#include <string>
#include "Koala/base/defs.h"
#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"
#include "Koala/io/text.h"


using namespace std;
using namespace Koala;

Graph<char,string> g1;

    Vertex<char,string> *A=g1.addVert('A');
    Vertex<char,string> *B=g1.addVert('B');
    Vertex<char,string> *C=g1.addVert('C');
//    Vertex<char,string> *D=g1.addVert('D');

Graph<Vertex<char,string> *,Edge<char,string> *> gcopy;

string fun(const Graph<char,string>& g,Vertex<char,string> *u,Vertex<char,string> *v,EdgeDirection dir)
{   char tab[2]={0,0};
    tab[0]=u->info;
    string su=tab;
    if (dir==EdLoop) return su;
    tab[0]=v->info;
    string sv=tab;
    if (dir==EdUndir) tab[0]='-';
    if (dir==EdDirOut) tab[0]='>';
    string s=tab;
    return su+s+sv;
}

#include "main.hpp"

//    Vertex<char,string> *tabV[7]={B,A,A,B,A,C,D};
    Vertex<char,string> *tabV3[5]={C,B,C,C,B};
    Vertex<char,string> *tabV2[5];
    Edge<char,string> *tabE[50];
    Set<Vertex<char,string>*> vset;
    Set<Edge<char,string>*> eset;
    vset+=A;
    vset+=B;

//    g1.makeAdjMatrix();

//    g1.addLoop(A,"A");g1.addLoop(A,"A");
//    g1.addEdge(A,B,"A--B");g1.addEdge(A,B,"A--B");
//    g1.addArc(B,D,"B->D");
//    g1.addEdge(B,C,"B--C");
//
////    g1.addArc(B,D,"B->D");g1.addArc(B,D,"B->D");g1.addArc(B,D,"B->D");
////    g1.addLoop(A,"A");
////    g1.addEdge(A,B,"A--B");g1.addEdge(A,B,"A--B");g1.addEdge(A,B,"A--B");
////    g1.addArc(B,A,"B->A");g1.addArc(B,A,"B->A");
////    g1.addArc(A,B,"A->B");g1.addArc(A,B,"A->B");g1.addArc(B,A,"B->A");g1.addArc(B,A,"B->A");
////    g1.addLoop(C,"C");g1.addLoop(C,"C");
////    g1.addEdge(B,C,"B--C");g1.addEdge(B,C,"B--C");
//    g1.delVert(C);g1.delVert(D);
//    IO::writeGraphText(g1,cout,IO::RG_EdgeList);
////    g1.del(A);
//    cout << endl;
////    g1.neg(tabV,tabV+5,EdAll,fun);
//    g1.neg(EdAll,fun);
//    IO::writeGraphText(g1,cout,IO::RG_EdgeList);


//    g1.addLoop(A,"A");g1.addLoop(B,"B");g1.addLoop(C,"C");
//    g1.addEdge(A,B,"A-B");g1.addEdge(A,C,"A-C");g1.addEdge(B,C,"B-C");g1.addEdge(A,D,"A-D");
//    g1.addArc(A,B,"A>B");g1.addArc(A,C,"A>C");g1.addArc(B,C,"B>C");g1.addArc(B,D,"B>D");
//    g1.addArc(B,A,"B>A");g1.addArc(C,A,"C>A");g1.addArc(C,B,"C>B");g1.addArc(D,B,"D>B");
//    g1.addLoop(A,"A");g1.addLoop(B,"B");g1.addLoop(C,"C");
//    g1.addEdge(A,B,"A-B");g1.addEdge(A,C,"A-C");g1.addEdge(B,C,"B-C");g1.addEdge(A,D,"A-D");
//    g1.addArc(A,B,"A>B");g1.addArc(A,C,"A>C");g1.addArc(B,C,"B>C");g1.addArc(B,D,"B>D");
//    g1.addArc(B,A,"B>A");g1.addArc(C,A,"C>A");g1.addArc(C,B,"C>B");g1.addArc(D,B,"D>B");


//    g1.addLoop(A,"A");g1.addLoop(B,"B");g1.addLoop(A,"A");g1.addLoop(B,"B");
    //g1.addLink(A,B,"A-B");
    g1.addLink(C,B,"B-C"); // g1.addLink(B,C,"B-C");//g1.addLink(A,B,"A-B");g1.addLink(A,C,"A-C");
    g1.addArc(A,C,"A>C");    g1.addArc(C,A,"C>A");
    g1.addArc(B,C,"B>C");
//    g1.addArc(C,A,"C>A");
//    g1.addArc(B,A,"B>A");g1.addArc(B,A,"B>A");g1.addArc(B,A,"B>A");
//    g1.addArc(B,A,"B>A");
//    g1.addLink(B,C,"B-C");g1.addLink(B,C,"B-C");g1.addLink(B,C,"B-C");
    g1.addLoop(B,"B");g1.addLoop(A,"A");

    IO::writeGraphText(g1,cout,IO::RG_EdgeList|IO::RG_Info);
    cout << "\n------------\n";
    g1.neg(
//           tabV3, tabV3+5,
           EdAll,fun);
    IO::writeGraphText(g1,cout,IO::RG_EdgeList|IO::RG_Info);

    cout << endl;

    cout << "\n!!!!!!!!!!!!!!!!!\n";

    gcopy.copy(g1,stdChoose(true)&stdChoose(true),valCast( )& valCast( ),
               Std2Linker<Privates::Std1PtrLinker,Std1NoLinker>(Privates::Std1PtrLinker(),Std1NoLinker())
               &
               stdLink(false,false)
//               Std2Linker<Privates::Std1PtrLinker,Std1NoLinker>(Privates::Std1PtrLinker(),Std1NoLinker())
               );

    IO::writeGraphText(gcopy,cout,IO::RG_EdgeList|IO::RG_Info);
    cout << "\n" << A << ' ' << B << ' '<<C << ' '<< g1.getEdge();

    cout << "\n!!!!!!!!!!!!!!!!!\n";

    g1.clearEdges();


    g1.addLoop(B,"B");
//    g1.addLink(C,B,"B-C"); //g1.addLink(B,C,"B-C");//g1.addLink(A,B,"A-B");g1.addLink(A,C,"A-C");
    g1.addArc(C,A,"C>A");//g1.addArc(C,A,"C>A");g1.addArc(A,C,"A>C");g1.addArc(A,C,"A>C");
    g1.addArc(A,C,"A>C");    g1.addArc(A,C,"A>C");
//    g1.addArc(B,C,"B>C");
    g1.addLink(B,C,"B-C");g1.addLink(B,C,"B-C");g1.addLink(B,C,"B-C");g1.addLink(A,C,"A-C");
//    g1.clearEdges();
    g1.addLoop(C,"C");g1.addLoop(C,"C");g1.addLoop(C,"C");g1.addLoop(C,"C");

    EdgeDirection dir=EdDirIn;
    cout <<g1.maxMu(dir).second ;
    cout << '\t' <<g1.maxMu(dir).first->info ;

//    for(int i=0;i<g1.getConEdges(tabE,tabV3,tabV3+5,tabV,tabV+5,EdAll);i++) cout << tabE[i]->info << '\n';

//    char mask=EdAll;
//
//    cout << endl <<endl;
//    cout << "\n-------\n";
//    eset.clear();
//    g1.getIndEdges(setInserter(eset),tabV,tabV+5,mask);
//    cout << eset << endl;
//
//
//    cout << g1.getIndEdgeSet(tabV,tabV+5,mask) << endl;
//
//
//    eset.clear();
//    g1.getIndEdges(setInserter(eset),vset,mask);
//    cout << eset << endl;
//
//    cout << g1.getIndEdgeSet(vset,mask) << endl;
//    cout << "\n-------\n";
//        eset.clear();
//    makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdges(setInserter(eset),tabV,tabV+5,mask);
//    cout << eset << endl;
//
//
//    cout << makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdgeSet(tabV,tabV+5,mask) << endl;
//
//
//    eset.clear();
//    makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdges(setInserter(eset),vset,mask);
//    cout << eset << endl;
//
//    cout << makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdgeSet(vset,mask) << endl;
//    cout << "\n-------\n";
//
//
////    cout << g1.delIndEdges(vset,mask) << endl;
//    cout << g1.getIndEdgeSet(vset.begin(),vset.end(),mask) << endl;
////    cout << g1.delIndEdges(vset,mask) << endl;
//    cout << g1.getIndEdgeSet(vset.begin(),vset.end(),mask) << endl;
//
//    cout << "\n!!!!!!!!!!\n";
//    cout << g1.getParalsSet(g1.getEdge(A,B))<< endl << makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getParalsSet(g1.getEdge(A,B));
//
//    {
//        g1.clear();
//        A=g1.addVert('A');
//        B=g1.addVert('B');
//        C=g1.addVert('C');
//        D=g1.addVert('D');
//        g1.addEdge(A,B);
////        cout << endl<<endl<< g1.getChosen(make_pair(blackHole,blackHole),make_pair(stdChoose(false),stdChoose(true)),false).first;
////            cout << endl<<endl<< makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getEdgeSet(stdChoose(true)) << g1.getEdge();
//        cout << endl<<endl<< makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getChosenSets(make_pair(stdChoose(true),stdChoose(true)),false).first<< endl<<g1.getVertSet()<<endl<<g1.getEdge() << endl;
//    }
//
//    g1.addArc(B,A);g1.addEdge(C,B);g1.addLoop(A);g1.addLoop(A);
//    vset=A;vset+=B;
//    cout << vset << endl << makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdgeSet(vset,EdAll);


    return 0;
}

