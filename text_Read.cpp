
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
    RG_Format bform=RG_VertexLists;

    RG_Format eform = (RG_Format)(((form.first) ? RG_VInfo : 0)+((form.second) ? RG_EInfo : 0));


#include "main.hpp"
	string buf;
	Graph<int,int> g;
	Graph<int,int>::PVertex A,B,C,D;
    Graph<int,int>::PEdge d,e,f,h,p,q,x,y,z;

    AssocTable<std::map<Graph<int,int>::PVertex,int> > v2int;
    AssocTable<std::map<Graph<int,int>::PEdge,int> > e2int;
    std::map<int,Graph<int,int>::PVertex> int2v;
    std::map<int,Graph<int,int>::PEdge> int2e;

    ifstream pliki;
	pliki.open("plik.txt");
	cout << boolalpha << readGraphText(g, pliki, bform+eform,int2v,int2e) << endl ;
    writeGraphText(g, cout, bform+RG_VInfo+RG_EInfo);

    cout <<endl << endl <<endl<< int2v[99] << ' ' << int2v[102]<< endl;
    cout << int2e[99] << ' ' << int2e[102];
//    cin << bz1;


return 0;
}
