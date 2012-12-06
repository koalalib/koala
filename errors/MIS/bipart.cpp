#include <iostream>
#include <fstream>
#include <string>
#include "../../Koala/base/defs.h"
#include "../../Koala/graph/graph.h"
#include "../../Koala/classes/detect.h"
#include "../../Koala/io/text.h"
#include "../../Koala/io/g6.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

ifstream pliki;


Koala::Graph<int,int> g;
Koala::Graph<int,int>::PVertex A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,tabV[1000];
Koala::Graph<int,int>::PEdge e,tabE[1000];


Koala::AssocTable<std::map<Koala::Graph<int,int>::PVertex,
            Koala::Matching::VertLabs<Koala::Graph<int,int> > > > vertCont;


#include "../../main.hpp"

    IO::readG6(g,"M???O?KGA_@?_?O??");
    Koala::IO::writeGraphText(g, cout, RG_VertexLists|RG_Info);
    int matchSize;

    matchSize = Koala::IsIt::Bipartite::maxStable(g,tabV);

    cout << "\n\n" << matchSize;


//    for(Koala::Graph<int,int>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) cout << e->info << endl;

    return 0;
}
