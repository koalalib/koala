#include <stdio.h>
#include <cstring>
#include <time.h>
#include <iostream>
#include <map>
#include "../Koala/io/graphml.h"
#include "../Koala/classes/create.h"
#include"../Koala/io/text.h"

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
    int64_t licz;
    char lit;
    bool flag;
    char buf[10];


    OpisV(int64_t i=0, char c=' ', bool aflag=false, const char* nap="") : licz(i), lit(c), flag(aflag)
    { strcpy(buf,nap); }
};

std::ostream& operator<<(std::ostream& strumien, OpisV arg)
{
    return strumien << arg.licz << ',' << arg.lit << std::boolalpha << ',' << arg.flag<< ','<< arg.buf;
}


struct OpisE {
    double licz;
    char lit;
    std::string name;

    OpisE(double i=0, char c=' ', std::string aname="") : licz(i), lit(c), name(aname) {}
};

std::ostream& operator<<(std::ostream& strumien, OpisE arg)
{
    return strumien << arg.licz << ',' << arg.lit <<','<<arg.name;
}




int main() {
	typedef  GraphII::PVertex Vert;
	typedef  GraphII::PEdge Edge;
	GraphII g1;
	Koala::IO::GraphML gml;
	Koala::IO::GraphMLGraph *gmlg;
	gml.newGraphML();



	{ std::cout << "\n\n------------------\n\n";
        Koala::Graph<OpisV,OpisE> g,g1;
        Koala::Vertex<OpisV,OpisE> *u=g.addVert(OpisV(5,'A',true,"Adam")), *v=g.addVert(OpisV(7,'B',false,"Piotr"));
        Koala::Edge<OpisV,OpisE> *e=g.addEdge(u,v,OpisE(0.1,'e',"Ala")), *d=g.addArc(v,u,OpisE(0.5,'d',"Ola")), *f=g.addLoop(v,OpisE(1.5,'f',"Ewa"));

        Koala::IO::writeGraphText(g,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

        std::cout << "\n\n***\n\n";
        gml.newGraphML();
        gmlg = gml.createGraph("first");
        gmlg->writeGraph(g,
                         Koala::IO::gmlLongField(&OpisV::licz,"vint") & Koala::IO::gmlStringField(&OpisV::lit,"vchar") & Koala::IO::gmlBoolField(&OpisV::flag,"vflag") & Koala::IO::gmlStringField(&OpisV::buf,"vbuf")
                         ,
                        Koala::IO::gmlDoubleField(&OpisE::licz,"edoub") & Koala::IO::gmlIntField(&OpisE::lit,"echar") & Koala::IO::gmlStringField(&OpisE::name,"ename")
                         );
        gml.writeFile("abc.xml");

        gml.newGraphML();
        gml.readFile("abc.xml");
        gmlg = gml.getGraph(0);
        gmlg->readGraph(g1,
                         Koala::IO::gmlLongField(&OpisV::licz,"vint") & Koala::IO::gmlStringField(&OpisV::lit,"vchar") & Koala::IO::gmlBoolField(&OpisV::flag,"vflag") & Koala::IO::gmlStringField(&OpisV::buf,"vbuf")
                        ,
                        Koala::IO::gmlDoubleField(&OpisE::licz,"edoub") &  Koala::IO::gmlIntField(&OpisE::lit,"echar") & Koala::IO::gmlStringField(&OpisE::name,"ename")
                        );
        Koala::IO::writeGraphText(g1,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

	}

	{ std::cout << "\n\n------------------\n\n";
        Koala::Graph<int,double> g,g1;
        Koala::Vertex<int,double> *u=g.addVert('A'), *v=g.addVert('B');
        Koala::Edge<int,double> *e=g.addEdge(u,v,1.1), *d=g.addArc(v,u,2.2), *f=g.addLoop(v,3.3);

        Koala::IO::writeGraphText(g,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

        std::cout << "\n\n***\n\n";
        gml.newGraphML();
        gmlg = gml.createGraph("first");
        gmlg->writeGraph(g,
                         Koala::IO::gmlIntField("vchar")
                         ,
                        Koala::IO::gmlDoubleField("edoub")
                         );
        gml.writeFile("abcd.xml");

        gml.newGraphML();
        gml.readFile("abcd.xml");
        gmlg = gml.getGraph(0);
        gmlg->readGraph(g1,
                         Koala::IO::gmlIntField("vchar")
                        ,
                        Koala::IO::gmlDoubleField("edoub")
                        );
        Koala::IO::writeGraphText(g1,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

	}

	{ std::cout << "\n\n------------------\n\n";
        Koala::Graph<char,std::string> g,g1;
        Koala::Vertex<char,std::string> *u=g.addVert('A'), *v=g.addVert('B');
        Koala::Edge<char,std::string> *e=g.addEdge(u,v,"Ala"), *d=g.addArc(v,u,"Ola"), *f=g.addLoop(v,"Ewa");

        Koala::IO::writeGraphText(g,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

        std::cout << "\n\n***\n\n";
        gml.newGraphML();
        gmlg = gml.createGraph("first");
        gmlg->writeGraph(g,
                         Koala::IO::gmlStringField("vchar")
                         ,
                        Koala::IO::gmlStringField("edoub")
                         );
        gml.setKeyAttrName("edoub","attrname");
        gml.writeFile("abcde.xml");

        gml.newGraphML();
        gml.readFile("abcde.xml");
        gmlg = gml.getGraph(0);
        gmlg->readGraph(g1,
                         Koala::IO::gmlStringField("vchar")
                        ,
                        Koala::IO::gmlStringField("edoub")
                        );
        Koala::IO::writeGraphText(g1,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

        std::string sname=gml.getKeyAttrName("edoub");
        std::cout << "\n\n" << sname;
	}
}
