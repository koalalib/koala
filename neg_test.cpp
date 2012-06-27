#include <iostream>
#include <string>
#include "Koala/base/def_struct.h"
#include "Koala/graph/graph.h"
#include "Koala/graph/subgraph.h"
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

int main() {

    Vertex<char,string> *tabV[5]={B,A,A,B,A};
    Vertex<char,string> *tabV2[5];
    Set<Vertex<char,string>*> vset;
    Set<Edge<char,string>*> eset;
    vset+=A;
    vset+=B;

    g1.addLoop(A,"A");
    g1.addLoop(B,"B");
    g1.addLoop(C,"C");
    g1.addEdge(A,B,"A--B");
    g1.addArch(B,D,"B->D");
    g1.addEdge(B,C,"B--C");

//    g1.addArch(B,D,"B->D");g1.addArch(B,D,"B->D");g1.addArch(B,D,"B->D");
//    g1.addLoop(A,"A");
    g1.addEdge(A,B,"A--B");g1.addEdge(A,B,"A--B");g1.addEdge(A,B,"A--B");
    g1.addArch(B,A,"B->A");
//    g1.addLoop(C,"C");g1.addLoop(C,"C");
//    g1.addEdge(B,C,"B--C");g1.addEdge(B,C,"B--C");
    IO::writeGraphText(g1,cout,IO::RG_EdgeList);
//    g1.del(A);

//    g1.neg(tabV,tabV+5,EdLoop,fun);
    g1.neg(EdAll,fun);
    IO::writeGraphText(g1,cout,IO::RG_EdgeList);

    char mask=EdAll;

    cout << endl <<endl;
    cout << "\n-------\n";
    eset.clear();
    g1.getIndEdges(setInserter(eset),tabV,tabV+5,mask);
    cout << eset << endl;


    cout << g1.getIndEdgeSet(tabV,tabV+5,mask) << endl;


    eset.clear();
    g1.getIndEdges(setInserter(eset),vset,mask);
    cout << eset << endl;

    cout << g1.getIndEdgeSet(vset,mask) << endl;
    cout << "\n-------\n";
        eset.clear();
    makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdges(setInserter(eset),tabV,tabV+5,mask);
    cout << eset << endl;


    cout << makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdgeSet(tabV,tabV+5,mask) << endl;


    eset.clear();
    makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdges(setInserter(eset),vset,mask);
    cout << eset << endl;

    cout << makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getIndEdgeSet(vset,mask) << endl;
    cout << "\n-------\n";


//    cout << g1.delIndEdges(vset,mask) << endl;
    cout << g1.getIndEdgeSet(vset.begin(),vset.end(),mask) << endl;
//    cout << g1.delIndEdges(vset,mask) << endl;
    cout << g1.getIndEdgeSet(vset.begin(),vset.end(),mask) << endl;

    cout << "\n!!!!!!!!!!\n";
    cout << g1.getParalsSet(g1.getEdge(A,B))<< endl << makeSubgraph(g1,make_pair(stdChoose(true),stdChoose(true))).getParalsSet(g1.getEdge(A,B));


}

