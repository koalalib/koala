
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

Graph<char,string>::PEdge tabE[10];

struct Dummy {
    int a;
};

struct bezop {
    int a,b;
};

//template <class A, class B>
//void operator << (const A&,const B&) {}

#include "main.hpp"
	string buf;
	Graph<char,string> g;
	Graph<char,string>::PVertex A,B,C,D;
    Graph<char,string>::PEdge d,e,f,h,p,q,x,y,z;
    char text[10000];

    AssocTable<std::map<Graph<char,string>::PVertex,int> > v2int;
    AssocTable<std::map<Graph<char,string>::PEdge,int> > e2int;
    std::map<int,Graph<char,string>::PVertex> int2v;
    std::map<int,Graph<char,string>::PEdge> int2e;


	A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
//	C=g.addVert();B=g.addVert();A=g.addVert();D=g.addVert();


	d=g.addEdge(A,B,"ab");
	e=g.addEdge(A,C,"ac");
	f=g.addArc(B,C,"bc");
	h=g.addLoop(C,"cc");
	g.addLoop(C,"cc2");
	g.addLoop(C,"cc3");
	x=g.addArc(A,C,"ac1");
	y=g.addArc(C,A,"ac2");
	z=g.addEdge(C,B,"bc2");
	g.addArc(C,A,"ac3");
	g.addArc(A,C,"ac4");
//	g.addEdge(A,B);g.addArc(B,C);g.addLoop(C);
    z=g.addLoop(D,"DD");g.addLoop(D,"dd");


	cout << std::boolalpha;
	cout << writeGraphText(g, cout, RG_VertexLists);
	cout << endl;

    cout << g.getParals(tabE,z,EdUndir);
    for(int i=0;i<10;i++) if (tabE[i]) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info <<
                            "," << g.getEdgeEnds(tabE[i]).second->info << "}";

//	for(z=g.getEdge(C,EdUndir|EdDirIn|EdDirOut);z;z=g.getEdgeNext(C,z,EdUndir|EdDirIn|EdDirOut))
//        cout << z->info << "\n";

//    Graph<char,string>::PEdge tabe[5]={e,e,x,f,y};
//    Set<Graph<char,string>::PEdge> s; s+=x; s+=y; s+=z;

//	cout << "\n:" <<g.delParals(C,EdUndir) << endl;
	cout << writeGraphText(g, cout, RG_VertexLists);

    cout << "\n******************\n";


    std::pair<bool,bool> form =std::make_pair(false,false);
    RG_Format eform = (RG_Format)(((form.first) ? RG_VInfo : 0)+((form.second) ? RG_EInfo : 0));

    cout << writeGraphText(g, cout, RG_VertexLists);
    cout << endl;
    cout << writeGraphText(g, cout, (RG_Format)(RG_VertexLists|eform));
    cout << endl;cout << endl;

    cout << writeGraphText(g, text, 10000, RG_EdgeList|RG_Info);
    cout << "text=" << text << "text"<<endl;
    g.clear();
    cout << readGraphText(g, text, RG_EdgeList);
    cout << writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout << endl;cout << endl;


//	writeGraphText(g, cout, RG_UndirectedVertexLists);
//	cout << endl;
//    writeGraphText(g, cout, RG_UndirectedVertexLists,form);
//	cout << endl;cout << endl;

	writeGraphText(g, cout, RG_EdgeList);
	cout << endl << endl;
    writeGraphText(g, cout, RG_EdgeList+eform);
	cout << endl;cout << endl;

	cout << "\n----------------\n";

    v2int[g.getVertNext(g.getVert())]=99;
    e2int[g.getEdge(EdLoop)]=102;

	writeGraphText(g, cout, RG_EdgeList|RG_Info,v2int,e2int);
    cout << "\n!!!!!!!!!!!!!!!!\n";
	writeGraphText(g, cout, RG_EdgeList|RG_Info,blackHole,blackHole);
	ofstream pliko("plik.txt");
//	writeGraphText(g, pliko, RG_VertexLists,form,v2int,e2int);
	writeGraphText(g, pliko, RG_EdgeList+eform,v2int,e2int);
//	pliko << '\n';
//	writeGraphText(g, pliko, RG_VertexLists,std::make_pair(true,true),v2int,e2int);
	writeGraphText(g, pliko, RG_EdgeList|RG_Info);
	pliko.close();
	cout << "\n----------------\n";

	ifstream pliki("plik.txt");
	g.clear();
	readGraphText(g, pliki, RG_EdgeList);
	g.clear();
//	readGraphText(g, pliki, RG_VertexLists,int2v,int2e);
	readGraphText(g, pliki, RG_EdgeList);
	pliki.close();
	writeGraphText(g, cout, RG_VertexLists);

    cout << "\n----------------\n";

#define VT float
#define ET double
#define Mask RG_VertexLists


    Graph<VT,ET> gr;
    gr.addVert();gr.addVert();
    gr.addArc(gr.getVert(),gr.getVertLast());
    ofstream pliko2("plik2.txt");
    writeGraphText(gr, cout, Mask|RG_Info );
	writeGraphText(gr, pliko2, Mask);
	pliko2.close();
    cout << endl;
	ifstream pliki2("plik2.txt");
	gr.clear();
	readGraphText(gr, pliki2, Mask);
	pliki2.close();
	g.clear();

	writeGraphText(gr, cout, Mask|RG_Info );
	pliki.open("plik.txt");
	readGraphText(g, pliki, RG_EdgeList,int2v,int2e);
    cout << "\n!!!!!!!!!!!!!!!!\n";
	writeGraphText(g, cout, RG_EdgeList|RG_Info );
    g.clear();
	readGraphText(g, pliki, RG_EdgeList,blackHole,blackHole);
	writeGraphText(g, cout, RG_EdgeList );


    cout << "\n\n" << int2v[99]->info << ' ' << int2e[102]->info;


//	Graph<> gc; gc.copy(g);
//	writeGraphText(gc, cout, RG_VertexLists);

//
//	writeGraphText(g, cout, RG_UndirectedEdgeList);
//	cout << endl;
//    writeGraphText(g, cout, RG_UndirectedEdgeList,form);
//	cout << endl;cout << endl;

        bezop bz1,bz2;
        {   char tab[100];
            g.clear();
            A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
            g.addArc(A,B);g.addArc(B,C);
//            g.addEdge(A,B);g.addEdge(B,C);
            writeG6(g,tab,99);
            cout << "\n" << tab;
        }

//    cin << bz1;


return 0;
}
