#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "Koala/base/def_struct.h"
#include "Koala/graph/graph.h"
#include "Koala/graph/subgraph.h"
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

int main()
{   string strN="N";


    g1.addArch(A,B,"AB");
    g1.addArch(B,C,"BC");
//    g1.addArch(C,A,"CA");
//    g1.addArch(D,C,"DC");
    g1.addLoop(B,"*B");
    g1.addLoop(C,"*C");




//    g1.neg(Directed,strN);

    IO::writeGraphText(g1,cout,IO::RG_EdgeList);
    cout<<endl<<endl;

    RelDiagram::transClousure(g1,"N");

    IO::writeGraphText(g1,cout,IO::RG_EdgeList);


    vec.clear();vec[0].resize(2);vec[1].resize(2);
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
        f=g.addArch(A,B,"AB");
        g.addArch(B,C,"BC");
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

        Graph<string,char> lg;

        AssocTable<std::map<Edge<char,string> *,Vertex<string,char>*> > e2vmap;
        AssocTable<std::map<Vertex<string,char>*,Edge<char,string> *> > v2emap;

        Edge<char,string> *e,*f;

        Vertex<char,string> *A=g.addVert('A');
        Vertex<char,string> *B=g.addVert('B');
        Vertex<char,string> *C=g.addVert('C');
        Vertex<char,string> *D=g.addVert('D');


        g.addArch(A,B,"AB");
        g.addEdge(D,C,"DC");

        g.addArch(B,A,"BA");
//        g.addArch(B,C,"BC");


    //    cout <<boolalpha << g.isEdgeEnd(e,B) <<endl;


        IO::writeGraphText(g,cout,IO::RG_EdgeList);
        cout<<endl<<endl;

        LineGraph::dir(g,lg,make_pair(stdCast(),stdCast()),stdLink(v2emap ,e2vmap));
    //    LineGraph::undir(g,lg,make_pair(stdCast(),stdCast()));
        IO::writeGraphText(lg,cout,IO::RG_EdgeList);

        for(Vertex<string,char> *v=lg.getVert();v;v=lg.getVertNext(v))
        {  assert((v2emap[v])->info==v->info); }
        for(Edge<char,string> *e=g.getEdge();e;e=g.getEdgeNext(e))
        {  assert((e2vmap[e])->info==e->info); }

        Graph<char,string> g2;

    }

    return EXIT_SUCCESS;
}
