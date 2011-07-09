
#include<stdio.h>
#include<string.h>

#include<string>
#include <fstream>

#include"./koala/graph/graph.h"
#include"./koala/io/text.h"
#include"./koala/container/set.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

Graph<char,string>::PEdge tabE[10];

int main() {
	string buf;
	Graph<char,string> g;
	Graph<char,string>::PVertex A,B,C,D;
    Graph<char,string>::PEdge d,e,f,h,p,q,x,y,z;
    char text[10000];

	A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
//	C=g.addVert();B=g.addVert();A=g.addVert();D=g.addVert();

	d=g.addEdge(A,B,"ab");
	e=g.addEdge(A,C,"ac");
	f=g.addArch(B,C,"bc");
	h=g.addLoop(C,"cc");
	g.addLoop(C,"cc2");
	g.addLoop(C,"cc3");
	x=g.addArch(A,C,"ac1");
	y=g.addArch(C,A,"ac2");
	z=g.addEdge(C,B,"bc2");
	g.addArch(C,A,"ac3");
	g.addArch(A,C,"ac4");
//	g.addEdge(A,B);g.addArch(B,C);g.addLoop(C);
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

    cout << writeGraphText(g, cout, RG_VertexLists);
    cout << endl;
    cout << writeGraphText(g, cout, RG_VertexLists,form);
    cout << endl;cout << endl;

    cout << writeGraphText(g, text, 10000, RG_EdgeList,std::make_pair(true,true));
    cout << "text=" << text << "text"<<endl;
    g.clear();
    cout << readGraphText(g, text, RG_EdgeList);
    cout << writeGraphText(g, cout, RG_VertexLists,std::make_pair(true,true));
    cout << endl;cout << endl;


//	writeGraphText(g, cout, RG_UndirectedVertexLists);
//	cout << endl;
//    writeGraphText(g, cout, RG_UndirectedVertexLists,form);
//	cout << endl;cout << endl;

	writeGraphText(g, cout, RG_EdgeList);
	cout << endl << endl;
    writeGraphText(g, cout, RG_EdgeList,form);
	cout << endl;cout << endl;

	cout << "\n----------------\n";
	writeGraphText(g, cout, RG_VertexLists,std::make_pair(true,true));
	ofstream pliko("plik.txt");
	writeGraphText(g, pliko, RG_EdgeList,form);
//	pliko << '\n';
	writeGraphText(g, pliko, RG_EdgeList,std::make_pair(true,true));
	pliko.close();
	cout << "\n----------------\n";

	ifstream pliki("plik.txt");
	g.clear();
	readGraphText(g, pliki, RG_EdgeList);
	g.clear();
	readGraphText(g, pliki, RG_EdgeList);
	pliki.close();
	writeGraphText(g, cout, RG_VertexLists);

	Graph<> gc; gc.copy(g);
	writeGraphText(gc, cout, RG_VertexLists);

//
//	writeGraphText(g, cout, RG_UndirectedEdgeList);
//	cout << endl;
//    writeGraphText(g, cout, RG_UndirectedEdgeList,form);
//	cout << endl;cout << endl;


}
