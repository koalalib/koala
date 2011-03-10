#include <iostream>
#include <string>

#include "Koala/graph/graph.h"
#include "Koala/algorithm/coloring.h"


typedef Koala::Graph< std::string,std::string > Graph;
typedef Graph::PVertex Vertex;
typedef Koala::AssocTable< std::map< Vertex,int > > Map;

int main() {
    Graph g;
    Vertex v[] = {
        g.addVert( "A" ),g.addVert( "B" ),g.addVert( "C" ),g.addVert( "D" ),g.addVert( "E" ),
        g.addVert( "F" ),g.addVert( "G" ),g.addVert( "H" ),g.addVert( "I" ),g.addVert( "J" )
    };
    std::vector< Vertex > w( v,v + 10 );
    Map c;

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

    return 0;
}
