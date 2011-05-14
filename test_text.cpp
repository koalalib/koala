
#include<stdio.h>
#include<string.h>

#include<string>

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


	writeGraphText(g, cout, RG_DirectedVertexLists);
	cout << endl;

    cout << g.getParals(tabE,z,EdUndir);
    for(int i=0;i<10;i++) if (tabE[i]) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info <<
                            "," << g.getEdgeEnds(tabE[i]).second->info << "}";

//	for(z=g.getEdge(C,EdUndir|EdDirIn|EdDirOut);z;z=g.getEdgeNext(C,z,EdUndir|EdDirIn|EdDirOut))
//        cout << z->info << "\n";

//    Graph<char,string>::PEdge tabe[5]={e,e,x,f,y};
//    Set<Graph<char,string>::PEdge> s; s+=x; s+=y; s+=z;

	cout << "\n:" <<g.delParals(C,EdUndir) << endl;
	writeGraphText(g, cout, RG_DirectedVertexLists);

    cout << "\n******************\n";

    std::pair<bool,bool> form =std::make_pair(false,false);

    writeGraphText(g, cout, RG_DirectedVertexLists);
    cout << endl;
    writeGraphText(g, cout, RG_DirectedVertexLists,form);
    cout << endl;cout << endl;

	writeGraphText(g, cout, RG_UndirectedVertexLists);
	cout << endl;
    writeGraphText(g, cout, RG_UndirectedVertexLists,form);
	cout << endl;cout << endl;

	writeGraphText(g, cout, RG_DirectedEdgeList);
	cout << endl;
    writeGraphText(g, cout, RG_DirectedEdgeList,form);
	cout << endl;cout << endl;

	writeGraphText(g, cout, RG_UndirectedEdgeList);
	cout << endl;
    writeGraphText(g, cout, RG_UndirectedEdgeList,form);
	cout << endl;cout << endl;


}
