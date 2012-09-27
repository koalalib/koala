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



namespace Privates {

template <class T>
bool isCharType()
{
    return false;
}

template <>
bool isCharType<char>()
{
    return true;
}

template <>
bool isCharType<unsigned char>()
{
    return true;
}


struct GMLDummyField {

    template <class T>
    void read(T& arg,Koala::IO::GraphMLKeysRead *gmlkr)
        {}

    template <class T>
    void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw)
        {}

};


template <class Info, class FieldType, Koala::IO::GraphMLKeyTypes::Type gmlType, class Next >
struct GMLField {
    std::string name;
    FieldType Info::*wsk;
    Next next;

    GMLField(FieldType Info::*awsk, std::string aname,Next anext) : name(aname), wsk(awsk), next(anext) {}


    void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr)
    {   next.read(arg,gmlkr);
        switch (gmlType)
        {
            case (Koala::IO::GraphMLKeyTypes::Bool) : arg.*wsk=(FieldType)gmlkr->getBool(name.c_str()); break;
            case (Koala::IO::GraphMLKeyTypes::Int) : arg.*wsk=(FieldType)gmlkr->getInt(name.c_str()); break;
            case (Koala::IO::GraphMLKeyTypes::Double) : arg.*wsk=(FieldType)gmlkr->getDouble(name.c_str()); break;
            case (Koala::IO::GraphMLKeyTypes::Long) : arg.*wsk=(FieldType)gmlkr->getLong(name.c_str()); break;
            case (Koala::IO::GraphMLKeyTypes::String) : koalaAssert(Privates::isCharType<FieldType>(),ContExcWrongArg);
                                                arg.*wsk=(FieldType)(gmlkr->getString(name.c_str())).at(0); break;
            default: koalaAssert(0,ContExcWrongArg);
        }

    }

    Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
        Info res;
        read(res,gmlkr);
        return res;
    }

    template <class T>
    void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
        switch (gmlType)
        {
            case (Koala::IO::GraphMLKeyTypes::Bool) : gmlkw->setBool(name.c_str(), (bool)(vert->info.*wsk)); break;
            case (Koala::IO::GraphMLKeyTypes::Int) : gmlkw->setInt(name.c_str(), (int)(vert->info.*wsk)); break;
            case (Koala::IO::GraphMLKeyTypes::Double) : gmlkw->setDouble(name.c_str(), (double)(vert->info.*wsk)); break;
            case (Koala::IO::GraphMLKeyTypes::Long) : gmlkw->setLong(name.c_str(), (int64_t)(vert->info.*wsk)); break;
            case (Koala::IO::GraphMLKeyTypes::String) :
                    {   koalaAssert(Privates::isCharType<FieldType>(),ContExcWrongArg);
                        std::string res;
                        res+=(char)(vert->info.*wsk);
                        gmlkw->setString(name.c_str(), res); break;
                    }
            default: koalaAssert(0,ContExcWrongArg);
        }
		next(vert,gmlkw);
	}


};


template <class Info, class Next >
struct GMLField <Info,std::string, Koala::IO::GraphMLKeyTypes::String, Next>
{
    std::string name;
    std::string Info::*wsk;
    Next next;

    GMLField(std::string Info::*awsk, std::string aname,Next anext) : name(aname), wsk(awsk), next(anext) {}


    void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr)
    {   next.read(arg,gmlkr);
        arg.*wsk=gmlkr->getString(name.c_str());
    }

    Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
        Info res;
        read(res,gmlkr);
        return res;
    }

    template <class T>
    void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
        gmlkw->setString(name.c_str(), vert->info.*wsk);
		next(vert,gmlkw);
	}

};


template <class Info, class Char, class Next, int N >
struct GMLCharField
{
    std::string name;
    Char (Info::*wsk)[N];
    Next next;

    GMLCharField(Char (Info::*awsk)[N], std::string aname,Next anext) : name(aname), wsk(awsk), next(anext) {}


    void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr)
    {   next.read(arg,gmlkr);
        std::strcpy((char*)(arg.*wsk),(gmlkr->getString(name.c_str())).c_str());
    }

    Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
        Info res;
        read(res,gmlkr);
        return res;
    }

    template <class T>
    void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
        gmlkw->setString(name.c_str(), (char*)(vert->info.*wsk));
		next(vert,gmlkw);
	}
};


template <class Info, class Next, int N >
struct GMLField <Info,char[N], Koala::IO::GraphMLKeyTypes::String, Next>
        : public GMLCharField <Info, char, Next, N>
{
    GMLField(char (Info::*awsk)[N], std::string aname,Next anext)
        : GMLCharField <Info, char, Next, N> (awsk,aname,anext)
         {}
};


template <class Info, class Next, int N >
struct GMLField <Info,unsigned char[N], Koala::IO::GraphMLKeyTypes::String, Next>
        : public GMLCharField <Info, char, Next, N>
{
    GMLField(unsigned char (Info::*awsk)[N], std::string aname,Next anext)
        : GMLCharField <Info, unsigned char, Next, N> (awsk,aname,anext)
         {}
};


}

template <class Info, class FieldType, class Next>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Int,Next>
gmlIntField(FieldType Info::*wsk,std::string name, Next next)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Int,Next>(wsk,name,next);
}


template <class Info, class FieldType>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Int,Privates::GMLDummyField >
gmlIntField(FieldType Info::*wsk,std::string name)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Int,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
}


template <class Info, class FieldType, class Next>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Double,Next>
gmlDoubleField(FieldType Info::*wsk,std::string name, Next next)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Double,Next>(wsk,name,next);
}

template <class Info, class FieldType>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Double,Privates::GMLDummyField >
gmlDoubleField(FieldType Info::*wsk,std::string name)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Double,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
}


template <class Info, class FieldType, class Next>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Bool,Next>
gmlBoolField(FieldType Info::*wsk,std::string name, Next next)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Bool,Next>(wsk,name,next);
}

template <class Info, class FieldType>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Bool,Privates::GMLDummyField >
gmlBoolField(FieldType Info::*wsk,std::string name)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Bool,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
}


template <class Info, class FieldType, class Next>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Long,Next>
gmlLongField(FieldType Info::*wsk,std::string name, Next next)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Long,Next>(wsk,name,next);
}

template <class Info, class FieldType>
Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Long,Privates::GMLDummyField >
gmlLongField(FieldType Info::*wsk,std::string name)
{
    return Privates::GMLField<Info,FieldType,Koala::IO::GraphMLKeyTypes::Long,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
}


template <class Info, class Next>
Privates::GMLField<Info,std::string,Koala::IO::GraphMLKeyTypes::String,Next>
gmlStringField(std::string Info::*wsk,std::string name, Next next)
{
    return Privates::GMLField<Info,std::string,Koala::IO::GraphMLKeyTypes::String,Next>(wsk,name,next);
}

template <class Info>
Privates::GMLField<Info,std::string,Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >
gmlStringField(std::string Info::*wsk,std::string name)
{
    return Privates::GMLField<Info,std::string,Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
}

template <class Info, class Next, int N>
Privates::GMLField<Info,char[N],Koala::IO::GraphMLKeyTypes::String,Next>
gmlStringField(char (Info::*wsk)[N],std::string name, Next next)
{
    return Privates::GMLField<Info,char[N],Koala::IO::GraphMLKeyTypes::String,Next>(wsk,name,next);
}

template <class Info,int N>
Privates::GMLField<Info,char[N],Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >
gmlStringField(char (Info::*wsk)[N],std::string name)
{
    return Privates::GMLField<Info,char [N],Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
}


template <class Info, class Next>
Privates::GMLField<Info,char,Koala::IO::GraphMLKeyTypes::String,Next>
gmlStringField(char Info::*wsk,std::string name, Next next)
{
    return Privates::GMLField<Info,char,Koala::IO::GraphMLKeyTypes::String,Next>(wsk,name,next);
}

template <class Info>
Privates::GMLField<Info,char,Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >
gmlStringField(char Info::*wsk,std::string name)
{
    return Privates::GMLField<Info,char,Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
}

template <class Info, class Next>
Privates::GMLField<Info,unsigned char,Koala::IO::GraphMLKeyTypes::String,Next>
gmlStringField(unsigned char Info::*wsk,std::string name, Next next)
{
    return Privates::GMLField<Info,unsigned char,Koala::IO::GraphMLKeyTypes::String,Next>(wsk,name,next);
}

template <class Info>
Privates::GMLField<Info,unsigned char,Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >
gmlStringField(unsigned char Info::*wsk,std::string name)
{
    return Privates::GMLField<Info,unsigned char,Koala::IO::GraphMLKeyTypes::String,Privates::GMLDummyField >(wsk,name,Privates::GMLDummyField());
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
        Koala::Edge<OpisV,OpisE> *e=g.addEdge(u,v,OpisE(0.1,'e',"Ala")), *d=g.addArch(v,u,OpisE(0.5,'d',"Ola")), *f=g.addLoop(v,OpisE(1.5,'f',"Ewa"));

        Koala::IO::writeGraphText(g,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

        std::cout << "\n\n***\n\n";
        gml.newGraphML();
        gmlg = gml.createGraph("first");
        gmlg->writeGraph(g,
                         gmlLongField(&OpisV::licz,"vint", gmlStringField(&OpisV::lit,"vchar", gmlBoolField(&OpisV::flag,"vflag",gmlStringField(&OpisV::buf,"vbuf") ) ) )
                         ,
                        gmlDoubleField(&OpisE::licz,"edoub", gmlIntField(&OpisE::lit,"echar", gmlStringField(&OpisE::name,"ename") ) )
                         );
        gml.writeFile("abc.xml");

        gml.newGraphML();
        gml.readFile("abc.xml");
        gmlg = gml.getGraph(0);
        gmlg->readGraph(g1,
                         gmlLongField(&OpisV::licz,"vint", gmlStringField(&OpisV::lit,"vchar", gmlBoolField(&OpisV::flag,"vflag",gmlStringField(&OpisV::buf,"vbuf") ) ) )
                        ,
                        gmlDoubleField(&OpisE::licz,"edoub", gmlIntField(&OpisE::lit,"echar", gmlStringField(&OpisE::name,"ename") ) )
                        );
        Koala::IO::writeGraphText(g1,std::cout,Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

	}
}
