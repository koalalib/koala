#include <iostream>
#include <fstream>
#include <string>
#include "../../Koala/base/defs.h"
#include "../../Koala/graph/graph.h"
#include "../../Koala/algorithm/matching.h"
#include "../../Koala/io/text.h"
#include "../../Koala/io/g6.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

ifstream pliki;


Koala::Graph<int,int> g;
Koala::Graph<int,int>::PVertex A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,tabV[32];
Koala::Graph<int,int>::PEdge e,tabE[1000];


Koala::AssocTable<std::map<Koala::Graph<int,int>::PVertex,
            Koala::Matching::VertLabs<Koala::Graph<int,int> > > > vertCont;


#include "../../main.hpp"

    pliki.open("ed2.txt");
    Koala::IO::readGraphText(g, pliki, RG_VertexLists|RG_Info);
    pliki.close();
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    int matchSize;
//    matchSize = Koala::Matching::findMax (g,vertCont,tabE,-1);

    g.clear();
    IO::readG6(g,"__??O@?C?SO???WG?OD@G?`_@??os?oG??C??J??_???_GQSOB?OCW@C_?_?_?????_OG?EO?G?AG???WAac");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    matchSize = Koala::Matching::findMax (g,vertCont,tabE,-1);
    return 0;
}
