#include <iostream>
#include <fstream>
#include <string>
#include "../../Koala/base/defs.h"
#include "../../Koala/graph/graph.h"
#include "../../Koala/algorithm/matching.h"
#include "../../Koala/io/text.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

ifstream pliki;


Koala::Graph<int,int> g;
Koala::Graph<int,int>::PVertex A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,tabV[32];
Koala::Graph<int,int>::PEdge e,tabE[32];


Koala::AssocTable<std::map<Koala::Graph<int,int>::PVertex,
            Koala::Matching::VertLabs<Koala::Graph<int,int> > > > vertCont;


#include "../../main.hpp"

    pliki.open("ed2.txt");
    Koala::IO::readGraphText(g, pliki, RG_VertexLists|RG_Info);
    pliki.close();
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    int matchSize = Koala::Matching::findMax (g,vertCont,tabE,-1);

    return 0;
}
