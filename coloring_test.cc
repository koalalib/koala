#include <iostream>
#include <string>

#include "Koala/graph/graph.h"
#include "Koala/algorithm/coloring.h"

typedef Koala::Graph< std::string,std::string > Graph;
typedef Graph::PVertex Vertex;
typedef Koala::AssocTable< std::map< Vertex,int > > Map;

#include "main.hpp"
    Graph g;
    Vertex v[20] = {
        g.addVert( "A" ),g.addVert( "B" ),g.addVert( "C" ),g.addVert( "D" ),g.addVert( "E" ),
        g.addVert( "F" ),g.addVert( "G" ),g.addVert( "H" ),g.addVert( "I" ),g.addVert( "J" )
    };
    std::vector< Vertex > w( v,v + 10 );
    Map c,cc;

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (i != j) g.addEdge( v[i],v[j],"" );
    std::cout << "Graf pelny, kolorowanie wierzcholek po wierzcholku:";
    for( unsigned i = 0; i < 10; i++ )
        std::cout << " " << Koala::SeqVertColoring::greedy( g,c,v[i] );
    std::cout << std::endl;
    c.clear();
    g.delEdges();

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (i != j && i % 2 != j % 2) g.addEdge( v[i],v[j],"" );
    std::cout << "Graf dwudzielny pelny, kolorowanie calosci:";
    Koala::SeqVertColoring::greedy( g,c );
    for( unsigned i = 0; i < 10; i++ ) std::cout << " " << c[v[i]];
    std::cout << std::endl;
    c.clear();
    g.delEdges();

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (i != j && i % 3 != j % 3) g.addEdge( v[i],v[j],"" );
    std::cout << "Graf trojdzielny pelny, kolorowanie zakresu 3-8:";
    Koala::SeqVertColoring::greedy( g,c,w.begin() + 2,w.end() - 2 );
    for( unsigned i = 0; i < 10; i++ )
        if (c.hasKey( v[i] ) ) std::cout << " " << c[v[i]];
        else std::cout << " -";
    std::cout << std::endl;
    c.clear();
    g.delEdges();

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (j == i + 1 || i == j + 1) g.addEdge( v[i],v[j],"" );
    std::cout << "Ścieżka, kolorowanie całości LF-em:";
    Koala::SeqVertColoring::lf( g,c );
    for( unsigned i = 0; i < 10; i++ ) std::cout << " " << c[v[i]];
    std::cout << std::endl;
    c.clear();
    g.delEdges();

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (j == i + 1 || i == j + 1) g.addEdge( v[i],v[j],"" );
    for(int i=10;i<20;i++)  v[i]=v[i-10];
    std::cout << "Ścieżka, kolorowanie całości LF-em:";
    Koala::SeqVertColoring::lf( g,c,v,v+20 );
    for( unsigned i = 0; i < 10; i++ ) std::cout << " " << c[v[i]];
    std::cout << std::endl;
    c.clear();
    g.delEdges();

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (i != j && i % 3 != j % 3) g.addEdge( v[i],v[j],"" );
    std::cout << "Graf trojdzielny pelny, kolorowanie SL-em:";
    Koala::SeqVertColoring::sl( g,c );
    for( unsigned i = 0; i < 10; i++ )
        if (c.hasKey( v[i] ) ) std::cout << " " << c[v[i]];
        else std::cout << " -";
    std::cout << std::endl;
    c.clear();
    g.delEdges();

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (i != j && i % 3 != j % 3) g.addEdge( v[i],v[j],"" );
    std::cout << "Graf trojdzielny pelny, kolorowanie SLF-em:";
    Koala::SeqVertColoring::slf( g,c );
    for( unsigned i = 0; i < 10; i++ )
        if (c.hasKey( v[i] ) ) std::cout << " " << c[v[i]];
        else std::cout << " -";
    std::cout << std::endl;
    c.clear();
    g.delEdges();

    for( unsigned i = 0; i < 10; i++ )
        for( unsigned j = 0; j < 10; j++ ) if (i != j) g.addEdge( v[i],v[j],"" );
    std::cout << "Graf pelny, kolorowanie wierzcholek po wierzcholku (z interchangem):";
    Koala::SeqVertColoring::greedyInter( g,c );
    for( unsigned i = 0; i < 10; i++ )
        std::cout << " " << c[v[i]];
    std::cout << std::endl;
    c.clear();
    g.delEdges();

//      g.addEdge(v[0],v[1],""); g.addEdge(v[0],v[2],""); g.addEdge(v[3],v[1],"");
//      g.addEdge(v[3],v[2],"");g.addEdge(v[0],v[4],"");
//
//      c[v[2]]=c[v[1]]=2; c[v[3]]=2; c[v[4]]=3;
//       std::cout << '\n' << Koala::SeqVertColoring::interchangeComponents(g,c,v[0],cc,2,3);
//
//      for(Vertex v=cc.firstKey();v;v=cc.nextKey(v))
//          std::cout << "\n (" << v->info << ':' << c[v] << ' ' << cc[v] << ')';
//

    return 0;
}
