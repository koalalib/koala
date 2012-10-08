#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "Koala/base/defs.h"
#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"
#include "Koala/classes/create.h"
#include "Koala/io/text.h"




using namespace std;
using namespace Koala;


Graph<char,string> g1;
Graph<string,char> lg1;


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

vector<vector<int> > vec(2);

int itab[]={0,1};
int& f(int i,int j)
{
    return vec[i][j];
}

#include "main.hpp"
   string strN="N";


    g1.addArc(A,B,"AB");
    g1.addArc(B,C,"BC");
//    g1.addArc(C,A,"CA");
//    g1.addArc(D,C,"DC");
    g1.addLoop(B,"*B");
    g1.addLoop(C,"*C");




//    g1.neg(Directed,strN);

    IO::writeGraphText(g1,cout,IO::RG_EdgeList);
    cout<<endl<<endl;



    RelDiagram::transClousure(g1,"N");

    IO::writeGraphText(g1,cout,IO::RG_EdgeList);


    vec.clear(); vec.resize(2); vec[0].resize(2);vec[1].resize(2);
    vec[0][1]=1;
    cout <<'\n'<< vec[0][0] << ' ' << vec[0][1] << '\n' << vec[1][0] << ' ' << vec[1][1] << endl<<endl;
//    RelDiagram::MatrixForm::inv(f,itab,itab+2);
    RelDiagram::MatrixForm::inv(vec,2);
    cout <<'\n'<< vec[0][0] << ' ' << vec[0][1] << '\n' << vec[1][0] << ' ' << vec[1][1];

    {   cout << "\n****************************\n\n";
        Graph<char,string> g;

        Graph<string,char> lg;

        AssocTable<std::map<Edge<char,string> *,Vertex<string,char>*> > e2vmap;
        AssocTable<std::map<Vertex<string,char>*,Edge<char,string> *> > v2emap;

        Edge<char,string> *e,*f;

        Vertex<char,string> *A=g.addVert('A');
        Vertex<char,string> *B=g.addVert('B');
        Vertex<char,string> *C=g.addVert('C');
        Vertex<char,string> *D=g.addVert('D');

        e=g.addEdge(A,B,"AB");
        f=g.addArc(A,B,"AB");
        g.addArc(B,C,"BC");
        g.addLoop(C,"C");

    //    cout <<boolalpha << g.isEdgeEnd(e,B) <<endl;


        IO::writeGraphText(g,cout,IO::RG_EdgeList);
        cout<<endl<<endl;

    //    LineGraph::undir(g,lg,make_pair(stdCast(),stdCast()),stdLink(v2emap ,e2vmap));
        LineGraph::undir(g,lg,make_pair(stdCast(),stdCast()));
        IO::writeGraphText(lg,cout,IO::RG_EdgeList);


    //    for(Vertex<string,char> *v=lg.getVert();v;v=lg.getVertNext(v))
    //    {  assert((v2emap[v])->info==v->info); }
    //    for(Edge<char,string> *e=g.getEdge();e;e=g.getEdgeNext(e))
    //    {  assert((e2vmap[e])->info==e->info); }

    }

    {   cout << "\n****************************\n\n";
        Graph<char,string> g;

        Graph<string,char> lg,lg2;

        AssocTable<std::map<Edge<char,string> *,Vertex<string,char>*> > e2vmap;
        AssocTable<std::map<Vertex<string,char>*,Edge<char,string> *> > v2emap;

        AssocTable<std::map<Vertex<char,string> *,Edge<string,char>*> > lv2emap;
        AssocTable<std::map<Edge<string,char>*,Vertex<char,string> *> > le2vmap;


        Edge<char,string> *e,*f;

        Vertex<char,string> *A=g.addVert('A');
        Vertex<char,string> *B=g.addVert('B');
        Vertex<char,string> *C=g.addVert('C');
        Vertex<char,string> *D=g.addVert('D');


        g.addArc(A,B,"AB");
        g.addEdge(D,C,"DC");

        g.addArc(B,A,"BA");
//        g.addArc(B,C,"BC");


    //    cout <<boolalpha << g.isEdgeEnd(e,B) <<endl;


        IO::writeGraphText(g,cout,IO::RG_EdgeList);
        cout<<endl<<endl;

        LineGraph::dir(g,lg,make_pair(stdCast(),stdCast()),make_pair(stdLink(v2emap ,e2vmap),stdLink(le2vmap ,lv2emap)));
        LineGraph::dir(g,lg2);
    //    LineGraph::undir(g,lg,make_pair(stdCast(),stdCast()));
        IO::writeGraphText(lg,cout,IO::RG_EdgeList);

        for(Vertex<string,char> *v=lg.getVert();v;v=lg.getVertNext(v))
        {  assert((v2emap[v])->info==v->info); }
        for(Edge<char,string> *e=g.getEdge();e;e=g.getEdgeNext(e))
        {  assert((e2vmap[e])->info==e->info); }

        for(Edge<string,char> *e=lg.getEdge();e;e=lg.getEdgeNext(e))
        {  assert((le2vmap[e])->info==e->info); }
        for(Vertex<char,string> *v=g.getVert();v;v=g.getVertNext(v))
        {  assert(lv2emap[v]==0 || (lv2emap[v])->info==v->info); }


        Graph<char,string> g2;

    }

    return EXIT_SUCCESS;
}
