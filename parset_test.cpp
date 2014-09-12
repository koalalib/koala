
#include<stdio.h>
#include<string.h>

#include<string>
#include <fstream>
#include <sstream>

#include"./Koala/graph/graph.h"
#include"./Koala/io/g6.h"
#include"./Koala/io/text.h"
#include"./Koala/container/set.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;


//#define format RG_VertexLists
#define format RG_EdgeList

#include "main.hpp"
	string buf;
	Graph<ParSet,ParSet> g;
	Graph<ParSet,ParSet>::PVertex A,B,C,D;
    Graph<ParSet,ParSet>::PEdge d,e,f,h,p,q,x,y,z;
    char text[10000];

    stringstream ss (stringstream::in | stringstream::out);
    ParSet xx,yy;
    xx.set("name","Ala").set("liczba",1).set("float",-3.);
    cout << xx <<"\n";
    ss << xx <<"\n";
    deque<string> keytable;
    std::string keytable2[10];

    xx.getKeys(keytable2);
    cout << '\n' << keytable2[0] << endl;

//    char comma;
//    do {
//			if(!cin.get(comma)) break;
//			cout << '*'<< ((comma=='\n')? '@' : comma)<<'-';
//		} while(comma != ',');
//    cout << "\n\n*" << ((comma=='\n')? '@' : comma)<<'-';
//
    cin>>yy;
    cout << yy << "|"<< yy.size();
//    return 0;
    cout << boolalpha;
    cout << (xx.isDouble("float")) << ' '<< (yy.isDouble("float")) << "\n\n\n";

	A=g.addVert();B=g.addVert();C=g.addVert();


	d=g.addEdge(A,B);
	e=g.addEdge(A,C);

    A->info.set("name","Ala").set("liczba",1).set("float",-3.);
    B->info.set("name","Bla").set("liczba",2).set("float",3.14);
    C->info.set("name","Celina").set("liczba",3).set("float",3.14);

    d->info.set("name","AB").set("liczba",-1).set("float",3.11);

	cout << std::boolalpha;
	cout << writeGraphText(g, cout, format|RG_Info);
	cout << endl;

    ofstream pliko("plik.txt");
//	writeGraphText(g, pliko, RG_VertexLists,form,v2int,e2int);
	writeGraphText(g, pliko, format|RG_Info);
	pliko.close();
	cout << "\n----------------\n";

	ifstream pliki("plik.txt");
	g.clear();
	readGraphText(g, pliki, format|RG_Info);
	pliki.close();
	writeGraphText(g, cout, format|RG_Info);


return 0;
}
