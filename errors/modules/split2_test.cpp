#define KOALA_SET_ON_VECTOR 1

#include<stdio.h>
#include<iostream>
#include"../../Koala/base/defs.h"
#include"../../Koala/graph/graph.h"
#include"../../Koala/algorithm/search.h"
#include"../../Koala/io/text.h"
#include "../../Koala/classes/detect.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

const char* print(ModPartType x)
{
    switch (x) {
        case mpTrivial : return "Trivial";
        case mpConnected : return "Connected";
        case mpDisconnected : return "Disconnected";
        case mpPrime : return "Prime";
    };
    assert(0);
    return NULL;
}

int vinfo(Graph<int>::PVertex v)
{
    return v->info;
}

#include "../../main.hpp"
	Graph<int> g;
	IO::readGraphText(g,
        "11\n"
        "0 3 -1 -2 -3\n"
        "1 4 -2 -4 -5 -6\n"
        "2 4 -3 -4 -5 -6\n"
        "3 3 -4 -5 -6\n"
        "4 2 -5 -6\n"
        "5 4 -7 -8 -9 -10\n"
        "6 4 -7 -8 -9 -10\n"
        "7 3 -8 -9 -10\n"
        "8 2 -9 -10\n"
        "9 0\n"
        "10 0\n"
        ,RG_VertexLists);

        for(int i=0;i<g.getVertNo();i++) g.vertByNo(i)->info=1+i;
        writeGraphText(g, cout, RG_VertexLists|RG_VInfo);

    cout << "\n\n********\n" << g.vertByNo(5)->info <<":"
        << imageSet<int>(g.getNeighSet(g.vertByNo(5)),vinfo)
        << "\n" <<g.vertByNo(6)->info <<":"
        << imageSet<int>(g.getNeighSet(g.vertByNo(6)),vinfo)
        << "\n\n\n";
	SearchStructs::CompStoreTool<Graph<int>::PVertex> tool;
	AssocTable<std::map<Graph<int>::PVertex,int> > vertTab;

#define _g g
	Modules::Partition res=Modules::split(_g,tool.input(),vertTab);


    cout << "size: "<< res.size << " type: " << print(res.type) << endl;
    for(Graph<int>::PVertex v=_g.getVert();v;v=_g.getVertNext(v)) cout << v->info << ":" << vertTab[v] <<"\t";
    cout << "\nsize: " << tool.size() << "\tlen:" <<tool.length();
    for(int i=0;i<tool.size();i++)
    {
        cout << "\n"<<tool.size(i) << ":";
        for(int j=0;j<tool.size(i);j++) cout << ' ' << tool[i][j]->info;
    }





//    writeGraphText(g, cout, RG_VertexLists|RG_VInfo);



	return 0;
	};
