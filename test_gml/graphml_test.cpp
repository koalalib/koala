#include <stdio.h>
#include <time.h>
#include <iostream>
#include <map>
#include "../Koala/io/graphml.h"
#include "../Koala/classes/create.h"
#include"../Koala/io/text.h"

Koala::StdRandGen<> stdgen;

typedef Koala::Graph<int,int> GraphII;
struct Gene {
	int operator()(int i) {return i;}
	int operator()(int i, int j, Koala::EdgeDirection x) {return i+j;}
};

struct VertRead {
	int operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		return gmlkr->getInt("kolorV");
	}
};
struct EdgeRead {
	int operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		return gmlkr->getInt("kolorE");
	}
};

struct VertWrite {
	void operator()( GraphII::PVertex vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setInt("kolorV", vert->info);
	}
};

struct EdgeWrite {
	void operator()( GraphII::PEdge edge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setInt("kolorE", edge->info);
	}
};

std::map<std::string,Koala::IO::GraphMLKeyTypes::Type> keymap;

	static char what(Koala::IO::GraphMLKeyTypes::ForKey arg)
	{
	    switch (arg)
	    {
	        case Koala::IO::GraphMLKeyTypes::All : return 'a';
	        case Koala::IO::GraphMLKeyTypes::GraphML : return 'f';
	        case Koala::IO::GraphMLKeyTypes::Graph : return 'g';
	        case Koala::IO::GraphMLKeyTypes::Node : return 'v';
	        case Koala::IO::GraphMLKeyTypes::Edge : return 'e';
	        case Koala::IO::GraphMLKeyTypes::Unknown : return '?';
	    }
	return '?';
	}

	static char what(Koala::IO::GraphMLKeyTypes::Type arg)
	{
	    switch (arg)
	    {
	        case Koala::IO::GraphMLKeyTypes::Bool : return 'b';
	        case Koala::IO::GraphMLKeyTypes::Int : return 'i';
	        case Koala::IO::GraphMLKeyTypes::Long : return 'l';
	        case Koala::IO::GraphMLKeyTypes::Float : return 'f';
	        case Koala::IO::GraphMLKeyTypes::Double : return 'd';
	        case Koala::IO::GraphMLKeyTypes::String : return 's';
	        case Koala::IO::GraphMLKeyTypes::NotDefined : return '?';
	    }
	}


struct OpisV {
    int licz;
    char lit;

    OpisV(int i=0, char c=' ') : licz(i), lit(c) {}
};

std::ostream& operator<<(std::ostream& strumien, OpisV arg)
{
    return strumien << arg.licz << ',' << arg.lit;
}


struct OpisE {
    double licz;
    char lit;

    OpisE(double i=0, char c=' ') : licz(i), lit(c) {}
};

std::ostream& operator<<(std::ostream& strumien, OpisE arg)
{
    return strumien << arg.licz << ',' << arg.lit;
}


struct OpisVRead {
	OpisV operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
	    keymap.clear();
	    gmlkr->getKeys(keymap);
	    std::cout<<"\n";
	    for(std::map<std::string,Koala::IO::GraphMLKeyTypes::Type>::const_iterator i=keymap.begin();i!=keymap.end();++i)
        {
            std::cout << "\t vread:"<< i->first << ' ' << std::boolalpha << gmlkr->isValue(i->first.c_str()) << '\n';
        }
        std::cout<<"\n";
        std:: cout << "\t liczint: " << what(gmlkr->getKeyType("liczint")) << ' ' << what(gmlkr->getKeyFor("liczint")) <<"  ";
        std:: cout << "\t vlitera: " << what(gmlkr->getKeyType("vlitera")) << ' ' << what(gmlkr->getKeyFor("vlitera")) <<"  ";
        std:: cout << "\t liczdouble: " << what(gmlkr->getKeyType("liczdouble")) << ' ' << what(gmlkr->getKeyFor("liczdouble")) <<"  ";
        std:: cout << "\t elitera: " << what(gmlkr->getKeyType("elitera")) << ' ' << what(gmlkr->getKeyFor("elitera")) <<"\n\n";



		return OpisV(gmlkr->getInt("liczint"),(char)gmlkr->getInt("vlitera"));
	}
};


struct OpisVWrite {
void *skip;
    OpisVWrite(void* toskip=0) : skip(toskip) {}
    template <class T>
	void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setInt("liczint", vert->info.licz);
		if (vert!=skip) gmlkw->setInt("vlitera", vert->info.lit);
	}
};

struct OpisERead {
	OpisE operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
	    keymap.clear();
	    gmlkr->getKeys(keymap);
	    std::cout<<"\n";
	    for(std::map<std::string,Koala::IO::GraphMLKeyTypes::Type>::const_iterator i=keymap.begin();i!=keymap.end();++i)
        {
            std::cout << "\t eread:"<< i->first << ' ' << std::boolalpha << gmlkr->isValue(i->first.c_str()) << '\n';
        }
        std::cout<<"\n";
        std:: cout << "\t liczint: " << what(gmlkr->getKeyType("liczint")) << ' ' << what(gmlkr->getKeyFor("liczint")) <<"  ";
        std:: cout << "\t vlitera: " << what(gmlkr->getKeyType("vlitera")) << ' ' << what(gmlkr->getKeyFor("vlitera")) <<"  ";
        std:: cout << "\t liczdouble: " << what(gmlkr->getKeyType("liczdouble")) << ' ' << what(gmlkr->getKeyFor("liczdouble")) <<"  ";
        std:: cout << "\t elitera: " << what(gmlkr->getKeyType("elitera")) << ' ' << what(gmlkr->getKeyFor("elitera")) <<"\n\n";

		return OpisE(gmlkr->getDouble("liczdouble"),(char)gmlkr->getInt("elitera"));
	}

};


struct OpisEWrite {
    void *skip;
    OpisEWrite(void* toskip=0) : skip(toskip) {}
    template <class T>
	void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setDouble("liczdouble", vert->info.licz);
		if (vert!=skip) gmlkw->setInt("elitera", vert->info.lit);
	}
};

int main() {
	typedef  GraphII::PVertex Vert;
	typedef  GraphII::PEdge Edge;
	GraphII g1;
	Koala::IO::GraphML gml;
	Koala::IO::GraphMLGraph *gmlg;
	gml.clearGraphML();

//	gml.newKey("kv", "kolorV", Koala::IO::GraphMLKeyTypes::Int,
//		Koala::IO::GraphMLKeyTypes::Node);
//	gml.newKey("ke", "kolorE", Koala::IO::GraphMLKeyTypes::Int,
//		Koala::IO::GraphMLKeyTypes::Edge);
//	gml.newKey("dg", "graphType", Koala::IO::GraphMLKeyTypes::Int,
//		Koala::IO::GraphMLKeyTypes::Graph);
//	gml.newKey("dgml", "graphML", Koala::IO::GraphMLKeyTypes::Int,
//		Koala::IO::GraphMLKeyTypes::GraphML);

	Koala::Creator::erdRen1(stdgen,g1, 10, 0.1, Gene(), Gene(), Koala::EdUndir);
	for(Vert vv = g1.getVert(); vv; vv = g1.getVertNext(vv)) {
		printf("%d : ", vv->info);
		for(Edge ee = g1.getEdge(vv); ee; ee = g1.getEdgeNext(vv, ee)) {
			Vert uu = g1.getEdgeEnd(ee, vv);
			printf("%d(%d), ", ee->info, uu->info);
		}
		printf("\n");
	}
	gmlg = gml.createGraph("first");
	gmlg->writeGraph(g1, VertWrite(), EdgeWrite());
	gmlg->setInt("graphType", 2);
	printf("%d %d\n", g1.getVertNo(), g1.getEdgeNo());

	gml.setInt("graphML", 4);
	gml.writeFile("xyz.xml");

	gml.clearGraphML();
	g1.clear();

	gml.readFile("xyz.xml");
	gmlg = gml.getGraph("first");
	printf("%d %d\n", gml.getInt("graphML"), gmlg->getInt("graphType"));
	gmlg->readGraph(g1, VertRead(), EdgeRead());
	for(Vert vv = g1.getVert(); vv; vv = g1.getVertNext(vv)) {
		printf("%d : ", vv->info);
		for(Edge ee = g1.getEdge(vv); ee; ee = g1.getEdgeNext(vv, ee)) {
			Vert uu = g1.getEdgeEnd(ee, vv);
			printf("%d(%d), ", ee->info, uu->info);
		}
		printf("\n");
	}

	{ std::cout << "\n\n************************\n\n";
        Koala::Graph<OpisV,OpisE> g,g1;
        Koala::Vertex<OpisV,OpisE> *u=g.addVert(OpisV(1,'A')), *v=g.addVert(OpisV(2,'B'));
        Koala::Edge<OpisV,OpisE> *e=g.addEdge(u,v,OpisE(0,'e')), *d=g.addArc(v,u,OpisE(0.5,'d')), *f=g.addLoop(v,OpisE(1.5,'f'));

        Koala::IO::writeGraphText(g,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

        std::cout << "\n\n***\n\n";
        gml.clearGraphML();
        gmlg = gml.createGraph("first");
        gmlg->writeGraph(g,OpisVWrite(v),OpisEWrite(d));
        gml.writeFile("xyz.xml");

        gml.clearGraphML();
        gml.readFile("xyz.xml");
        gmlg = gml.getGraph(0);
        gmlg->readGraph(g1,OpisVRead(),OpisERead());
        Koala::IO::writeGraphText(g1,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

	}
}
