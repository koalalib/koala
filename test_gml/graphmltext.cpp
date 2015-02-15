#include<map>
#include<vector>
#include<string>
#include<iostream>
#include "../Koala/graph/graph.h"
#include "../Koala/io/graphml.h"
#include"../Koala/io/text.h"
#include"../Koala/io/parsetgml.h"


typedef Koala::Graph<Koala::IO::ParSet, Koala::IO::ParSet> G;

int main() {
	G g;
	Koala::IO::GraphML gml;
	Koala::IO::GraphMLGraph *gmlg;

	Koala::IO::readGraphText(g, "2 0(x:3.14,y:4.1) 1 >1(lab:xyz) 1(x:5,y:6)", Koala::IO::RG_VertexLists);
	gml.clearGraphML();
        gmlg = gml.createGraph("gr");
        gmlg->writeGraph(g, Koala::IO::ParSetWrite<G>(), Koala::IO::ParSetWrite<G>());
        gml.writeFile("out.xml");

	g.clear();

	gml.clearGraphML();
        gml.readFile("rev.xml");
        gmlg = gml.getGraph(0);
        gmlg->readGraph(g, Koala::IO::ParSetRead(gml), Koala::IO::ParSetRead(gml));
	Koala::IO::writeGraphText(g, std::cout, Koala::IO::RG_VertexLists | Koala::IO::RG_Info);
	return 0;
	};
