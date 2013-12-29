#define KOALA_SET_ON_VECTOR 1

#include<stdio.h>
#include<iostream>
#include<string>
#include"Koala/base/defs.h"
#include"Koala/graph/graph.h"
#include"Koala/coloring/vertex.h"
#include"Koala/coloring/edge.h"
#include"Koala/coloring/equitable.h"
#include"Koala/io/text.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

Graph<char,std::string> g;
Graph<char,std::string>::PVertex A,B,C,D,E,F;
Graph<char,std::string>::PEdge a,b,c,d,e,f;
AssocArray<Graph<char,std::string>::PVertex,int > vcolor;
AssocArray<Graph<char,std::string>::PEdge,int > ecolor;

struct MyClass {

    string s1, s2;
  MyClass(const char* n1,const char* n2) : s1(n1), s2(n2) {std::cout <<"MyClass constructed\n";}
  void pisz()
  {
      cout << '\n' << s1 << ' ' << s2 << '\t' << s1+s2 << '\n';
  }
   ~MyClass() {std::cout <<"MyClass destroyed\n";}
};


struct MyClass2 : public MyClass {

    MyClass2(const char* n1) : MyClass(n1,"Ola") {}

   ~MyClass2() {std::cout <<"MyClass2 destroyed\n";}
};


namespace Koala {



}

#include "main.hpp"
	Graph<char,std::string> gk3;
	Graph<char,std::string>::PVertex tabV[20],tabV2[20],F,G,H;
	A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
	D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');

	a=g.addEdge(A,B); b=gk3.addEdge(B,C);c=gk3.addEdge(C,D);
	d=g.addEdge(D,E); e=gk3.addEdge(E,F);f=gk3.addEdge(F,A);

	vcolor[A]=0;vcolor[B]=1;vcolor[C]=8;vcolor[D]=1;vcolor[E]=0;vcolor[F]=1;

	ecolor[a]=0;ecolor[b]=1;ecolor[c]=8;ecolor[d]=1;ecolor[e]=0;ecolor[f]=1;

	//EquitVertColoring::repairSimple(g,vcolor);

	cout << boolalpha << SeqVertColoring::test(g,vcolor) <<"\n";
	for(Graph<char,std::string>::PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << ": " << vcolor[v] <<"\n";

	cout << "\n\n*************\n\n";

	cout << SeqEdgeColoring::test(g,ecolor) <<"\n";
	for(Graph<char,std::string>::PEdge v=g.getEdge();v;v=g.getEdgeNext(v)) cout << e->info << ": " << ecolor[v] <<"\n";


    cout << "\n\n*************\n\n";
    char tab[100];

    cout << (void*) tab;

    MyClass *ptr=new (tab) MyClass2("Proba");

    cout << (void*) ptr;


    ptr->pisz();

    cout << "\n\n*************\n\n";

    Koala::SimplArrPool<MyClass2> pool(4);
    cout << (pool.throwIfNotEmpty=false);

    MyClass2 * mytab[10];

    int nr=0;
    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Pierwszy");
    mytab[nr++]->pisz();


    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Drugi");
    mytab[nr++]->pisz();


    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Trzeci");
    mytab[nr++]->pisz();


    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Czwarty");
    mytab[nr++]->pisz();

    cout << pool.size() <<' '<< pool.busy() << endl;

    pool.dealloc(mytab[1]);
    cout << pool.size() <<' '<< pool.busy() << endl;

    pool.dealloc(mytab[2]);
    cout << pool.size() <<' '<< pool.busy() << endl;

//        pool.dealloc(mytab[1]);

    pool.dealloc(mytab[0]);
    cout << pool.size() <<' '<< pool.busy() << endl;

    pool.dealloc(mytab[3]);
    cout << pool.size() <<' '<< pool.busy() << endl;

    nr=0;
    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Pierwszy");
    mytab[nr++]->pisz();


    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Drugi");
    mytab[nr++]->pisz();


    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Trzeci");
    mytab[nr++]->pisz();


    cout << pool.size() <<' '<< pool.busy() << endl;
    mytab[nr]=new (pool.alloc()) MyClass2("Czwarty");
    mytab[nr++]->pisz();

    cout << pool.size() <<' '<< pool.busy() << endl;

     pool.dealloc(mytab[1]);
    cout << pool.size() <<' '<< pool.busy() << endl;

//    mytab[nr]=new (pool.alloc()) MyClass2("Piaty");
//    mytab[nr++]->pisz();

//    ptr->~MyClass();

	return 0;
	};
