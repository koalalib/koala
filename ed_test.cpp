#include <iostream>
#include <string>
#include "Koala/base/defs.h"
#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"
#include "Koala/algorithm/search.h"
#include "Koala/io/text.h"


using namespace std;
using namespace Koala;

Graph<char,string> g1;

    Vertex<char,string> *A=g1.addVert('A');
    Vertex<char,string> *B=g1.addVert('B');
    Vertex<char,string> *C=g1.addVert('C');
    Vertex<char,string> *D=g1.addVert('D');

string fun(Graph<char,string>& g,Vertex<char,string> *u,Vertex<char,string> *v,EdgeDirection dir)
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

    Vertex<char,string> *tabV[5]={B,A,A,B,A};
    Vertex<char,string> *tabV3[5]={C,B,B,C,B};
    Vertex<char,string> *tabV2[5];
    Edge<char,string> *tabE[50],*e,*f,*tabE2[50],*tabE3[50];
    Set<Vertex<char,string>*> vset;
    Set<Edge<char,string>*> eset;
    vset+=A;
    vset+=B;

    g1.makeAdjMatrix();





//    g1.addLoop(A,"A");
//    g1.addLoop(B,"B");
//    g1.addLoop(C,"C");
//
//    g1.addEdge(A,B,"A-B");
//    g1.addEdge(A,C,"A-C");
//    g1.addEdge(B,C,"B-C");
//    g1.addArc(A,B,"A>B");
//    g1.addArc(A,C,"A>C");
//    g1.addArc(B,C,"B>C");
//    g1.addArc(B,A,"B>A");
//    g1.addArc(C,A,"C>A");
//    g1.addArc(C,B,"C>B");
//    g1.addLoop(A,"A");g1.addLoop(B,"B");g1.addLoop(C,"C");
//    g1.addEdge(A,B,"A-B");g1.addEdge(A,C,"A-C");g1.addEdge(B,C,"B-C");
//    g1.addArc(A,B,"A>B");g1.addArc(A,C,"A>C");g1.addArc(B,C,"B>C");
//    g1.addArc(B,A,"B>A");g1.addArc(C,A,"C>A");g1.addArc(C,B,"C>B");


    cout <<boolalpha;
//    for(int i=0;i<g1.getConEdges(tabE,tabV3,tabV3+5,tabV,tabV+5,EdAll);i++) cout << tabE[i]->info << '\n';
//    g1.addLoop(B,"*B");
    e=g1.addArc(B,A,"B>A");
//    g1.addLoop(A,"*A");
    g1.addArc(B,C,"B>C");
//    g1.addEdge(B,C,"B-C");
    f=g1.addEdge(B,A,"B-A");
//    g1.addEdge(A,B,"A-B");
    //g1.addEdge(C,B,"C-B");
//    g1.addArc(C,B,"C>B");
    g1.addLoop(B,"*B");g1.addLoop(B,"*B");g1.addLoop(B,"*B");
    g1.addArc(C,B,"C>B");
    g1.addArc(C,B,"C>B");
    g1.addEdge(A,B,"A-B");
//    g1.addArc(B,C,"B>C");
    IO::writeGraphText(g1,cout,IO::RG_EdgeList);
    cout<< endl<<endl;

//    g1.addArc(A,D,"A>D");
    #define mask EdDirIn
    vset=A; vset+=B;
//    int res=g1.getIncEdges(tabE,vset.begin(),vset.end(),EdDirOut,EdUndir);
//    cout << res<<endl;
//    for(int i=0;i<res;i++) cout << tabE[i]->info << endl;
    for(int i=0;i<g1.getIncVerts(tabV,vset,EdDirIn,Directed);i++) cout << tabV[i]->info << ' ';
//    g1.getIncVerts(tabV,vset.begin(),vset.end(),mask,mask);
//    g1.getIncVertSet(vset.begin(),vset.end(),mask,mask);
//    g1.getIncVertSet(vset,mask,mask);

    std::cout << "\n\n!!!!!!!!!!!!!!\n\n";

    cout << g1.getChosenSets(std::make_pair(stdChoose(true),stdChoose(false))).first  << ' ' << g1.getEdgeEnds(g1.getEdge()).first;
    cout <<endl << boolalpha << g1.getIncVertSet(vset);
    vset+=C;vset+=D;
    cout << endl << (makeSubgraph(g1,std::make_pair(stdChoose(true),edgeTypeChoose(Directed))).getEdgeNo(B,C,EdAll) );

    std::cout << "\n\n!!!!!!!!!!!!!!\n\n";
    IO::writeGraphText(g1,cout,IO::RG_EdgeList);
    cout << endl << endl;
//    IO::writeGraphText(makeSubgraph(makeRevView(g1),std::make_pair(stdChoose(vset),stdChoose(true))),cout,IO::RG_EdgeList);
    IO::writeGraphText(makeSubgraph(makeRevView(g1),std::make_pair(stdChoose(vset),stdChoose(true))),cout,IO::RG_EdgeList);
    cout << endl << endl;
    cout << "\n!:"<<makeRevView(g1).getEdgeNo(Directed) << endl;

    AssocArray<Vertex<char,string>*,bool> vset2;
    vset2[A];vset2[B];vset2[C];vset2[D];
    std::cout << "\n\n-------------\n\n";
    cout << makeSubgraph(makeRevView(g1),std::make_pair(assocKeyChoose(vset2),stdChoose(true))).getEdges(blackHole,EdAll);
    std::cout << "\n\n!!!\n\n";
    IO::writeGraphText(makeSubgraph(makeRevView(g1),std::make_pair(assocKeyChoose(vset2),stdChoose(true))),cout,IO::RG_EdgeList);//.getEdgeNo(Directed);
    g1.clearEdges();
    UndirView<Graph<char,string> > undg;
//    IO::writeGraphText(undg,cout,IO::RG_EdgeList);
    undg.plug(g1);
    IO::writeGraphText(undg,cout,IO::RG_EdgeList);

    cout << g1.addArc(B,A);g1.addArc(D,C);g1.addEdge(C,B);g1.addLoop(B);
    cout << endl << BFS::getPath(makeRevView(g1),A,D,BFS::outPath(blackHole,blackHole));
    cout << endl << makeRevView(g1).getEdge(A,EdDirOut);

//    std::cout << "\n\n!!!!!!!!!!!!!!\n\n";
//
//    std::pair<int,int> intp=g1.findParals(make_pair(tabE,tabE2),EdUndir);
//    for(int i=0;i<intp.first;i++) cout << tabE[i]->info << '\n';
//    cout<< endl<<endl;
//    for(int i=0;i<intp.second;i++) cout << tabE2[i]->info << '\n';
//
//    g1.getEdges(tabE3);g1.getEdges(tabE3+g1.getEdgeNo());
//    cout<< endl<<endl<<g1.delAllParals2(tabE3,tabE3+2*g1.getEdgeNo(),EdUndir)<<endl;
//
//    intp=g1.findParals(make_pair(tabE,tabE2),EdUndir);
//    for(int i=0;i<intp.first;i++) cout << tabE[i]->info << '\n';
//    cout<< endl<<endl;
//    for(int i=0;i<intp.second;i++) cout << tabE2[i]->info << '\n';
//
//    cout << g1.findParals(make_pair(tabE,tabE2),A,B,EdUndir).first;

//    Edge<char,string> *tab[50]={e,0,e};
//    vset=A;vset+=B;
//    int n=g1.getIndEdges(tab,vset);
//    for(int i=0;i<n;i++) cout << tab[i]->info << '\n';

//    cout << endl<<endl;
//    IO::writeGraphText(g1,cout,IO::RG_EdgeList);

//    char mask=EdAll;
//
//    cout << endl <<endl;
//    cout << "\n-------\n";
//    eset.clear();
//    g1.getIncEdges(setInserter(eset),tabV,tabV+5,mask);
//    cout << eset << endl;
//
//
//    cout << g1.getIncEdgeSet(tabV,tabV+5,mask) << endl;
//
//
//    eset.clear();
//    g1.getIncEdges(setInserter(eset),vset,mask);
//    cout << eset << endl;
//
//    cout << g1.getIncEdgeSet(vset,mask) << endl;
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

