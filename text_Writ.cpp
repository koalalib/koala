
#include<stdio.h>
#include<string.h>

#include<string>
#include <fstream>

#include"./Koala/graph/graph.h"
#include"./Koala/io/g6.h"
#include"./Koala/io/text.h"
#include"./Koala/container/set.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

    std::pair<bool,bool> form =std::make_pair(true,true);
    RG_Format bform=RG_EdgeList;

    RG_Format eform = (RG_Format)(((form.first) ? RG_VInfo : 0)+((form.second) ? RG_EInfo : 0));

    struct Klasa {
        int a;
        string s;

//        Klasa(int arg, string sarg="1"): a(arg), s(sarg) {}
        Klasa(string sarg="1"): a(0), s(sarg) {}
        Klasa(const char* sarg="1"): a(0), s(string(sarg)) {}
        operator int() { return 10; }
    };

ostream& operator<<(ostream& out, Klasa x)
{
    out << x.s;
    return out;
}

#include "main.hpp"
	string buf;
//	Graph<string,string> g;
//	Graph<string,string>::PVertex A,B,C,D;
//    Graph<string,string>::PEdge d,e,f,h,p,q,x,y,z;
//
//    AssocTable<std::map<Graph<string,string>::PVertex,int> > v2int;
//    AssocTable<std::map<Graph<string,string>::PEdge,int> > e2int;
//    std::map<int,Graph<string,string>::PVertex> int2v;
//    std::map<int,Graph<string,string>::PEdge> int2e;

{
    using namespace std;


    Graph<string,int> g;

    Vertex<string,int>* u;
    // albo tak:
    Graph<string,int>::PVertex v;

    Edge<string,int>* e;

    u=g.addVert("Gdansk");
    v=g.addVert("Olsztyn");
    e=g.addArc(u,v,179);
    // albo tak:
    // e=g.addEdge(u,v,179,EdDirOut);

    writeGraphText(g, cout, bform+eform);


    return 0;

}
	Graph<Klasa,Klasa> g;
	Graph<Klasa,Klasa>::PVertex A,B,C,D;
    Graph<Klasa,Klasa>::PEdge d,e,f,h,p,q,x,y,z;

    AssocTable<std::map<Graph<Klasa,Klasa>::PVertex,int> > v2int;
    AssocTable<std::map<Graph<Klasa,Klasa>::PEdge,int> > e2int;
    std::map<int,Graph<Klasa,Klasa>::PVertex> int2v;
    std::map<int,Graph<Klasa,Klasa>::PEdge> int2e;

	A=g.addVert("1");B=g.addVert("2");C=g.addVert("3");D=g.addVert("4");


	d=g.addEdge(A,B,"10200");
	e=g.addEdge(A,C,"10300");
	f=g.addArc(B,C,"20300");
	h=g.addLoop(C,"30300");
	g.addLoop(C,"30301");
	g.addLoop(C,"30302");
	x=g.addArc(A,C,"10301");
    z=g.addLoop(D,"40400");g.addLoop(D,"40401");



    v2int[g.getVertNext(g.getVert())]=99;
    e2int[g.getEdge(EdLoop)]=102;

	writeGraphText(g, cout, bform+eform,v2int,e2int);
//    cout << "\n!!!!!!!!!!!!!!!!\n";
//	writeGraphText(g, cout, form|RG_Info,blackHole,blackHole);
	ofstream pliko("plik.txt");
//	writeGraphText(g, pliko, RG_VertexLists,form,v2int,e2int);
	writeGraphText(g, pliko, bform+eform,v2int,e2int);
//	pliko << '\n';
//	writeGraphText(g, pliko, RG_VertexLists,std::make_pair(true,true),v2int,e2int);
//	writeGraphText(g, pliko, bform|RG_Info);
	pliko.close();
	cout << "\n----------------\n";




return 0;
}
