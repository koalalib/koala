#ifndef KOALA_GRAPHML_H
#define KOALA_GRAPHML_H

/* ------------------------------------------------------------------------- *
 * graphml.h
 *
 * ------------------------------------------------------------------------- */

#include "../graph/graph.h"
#include "../tinyxml/tinyxml.h"
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__)
#define atoll		_atoi64
typedef __int64 int64_t;
#endif

namespace Koala {
namespace IO {

class GraphMLGraph;
class GraphMLKeysRead;
class GraphMLKeysWrite;

struct GraphMLKeyTypes {
	enum Type { NotDefined, Bool, Int, Long, Float, Double, String };
	enum ForKey { Unknown, All, GraphML, Graph, Node, Edge };
};

class GraphML {
	friend class GraphMLGraph;
	friend class GraphMLKeysRead;
	friend class GraphMLKeysWrite;
public:
	GraphML();
	~GraphML();

	//use this method to create new GraphML structure
	void newGraphML();

	int graphNo(); //number of graphs in GraphML structure
	std::string getGraphName(int);
	int getGraphNo(const char *name); //get graph number, -1 if there is no graph with id==name
	bool isGraphName(const char *name); //test if there is a graph with id==name
	//createGraph: get graph named name or create one if there is no in GraphML
	// the new graph is set at the end of GraphML
	GraphMLGraph* createGraph(const char *name);
	GraphMLGraph* getGraph(const char *name); //is there is no graph with id==name then it returns NULL
	GraphMLGraph* getGraph(int n); //nth graph
	bool deleteGraph(const char *graphName);

	//faster way to read/write graph than getGraph, readGraph/writeGraph (GraphMLGraph)
	template< class Graph >
	bool readGraph( Graph &graph, const char *name);
	template<typename Graph, typename InfoVertex, typename InfoEdge>
	bool readGraph(Graph &graph, InfoVertex infoVert, InfoEdge infoEdge, const char *name);
	template< class Graph >
	bool writeGraph(const Graph &graph, const char *name); //@return false if there is no graph named name
	template<typename Graph, typename InfoVertex, typename InfoEdge>
	bool writeGraph(const Graph &graph, InfoVertex infoVert, InfoEdge infoEdge, const char *name);

	GraphMLKeyTypes::Type getKeyType(const char *name);
	GraphMLKeyTypes::ForKey getKeyFor(const char *name);

	//return all defined keys for GraphML or All
	//res is a map : string->GraphMLKeyTypes::Type
	template <class AssocCont> void getKeys(AssocCont& res);
	//reading and writing graphML properties
	bool setBool(const char *name, bool val);
	bool setInt(const char *name, int val);
	bool setLong(const char *name, int64_t val);
	bool setDouble(const char *name, double val);
	bool setString(const char *name, const char *val);
	bool setString(const char *name, const std::string &val);

	bool isValue(const char *name); //check if value is set
	bool getBool(const char *name);
	int getInt(const char *name);
	int64_t getLong(const char *name);
	double getDouble(const char *name);
	std::string getString(const char *name);

	//key type modifications
	bool delKeyGlobal(const char *name);

	//GraphML read/write
	bool readFile( const char *fileName );
	bool writeFile( const char *fileName );
/*	friend std::istream& operator>>(std::istream &in, GraphML &gml);
	friend std::ostream& operator<<(std::ostream &out, const GraphML &gml);
	friend std::string& operator<<(std::string &out, const GraphML &gml);*/
private:
	template<GraphMLKeyTypes::Type, typename InType>
	bool set(const char *name, InType val);
	template<typename InOutType>
	InOutType get(const char *name, const InOutType def);

	void clear();
	void createInitial();
	bool newKey(const char *name, GraphMLKeyTypes::Type type,
		GraphMLKeyTypes::ForKey forKey);
	bool readXML();
	bool readXMLKey(TiXmlElement *xml);
	bool readXMLGraph(TiXmlElement *xml);
	bool readXMLData(TiXmlElement *xml);

	struct KeyHolder {
		GraphMLKeyTypes::Type type;
		union {
			int intVal;
			double dblVal;
			int64_t longVal;
		} uVal;
		std::string sVal;
		std::string print();
		bool set(bool val);
		bool set(int val);
		bool set(int64_t val);
		bool set(double val);
		bool set(const char *val);
		bool set(const std::string &val);
		template<typename T> T get();
		/*bool get(); int get(); int64_t get();
			double get(); std::string get();*/
	};
	struct NameDef : public KeyHolder {
		std::string id, name;
		TiXmlElement *xml;
		GraphMLKeyTypes::ForKey forKey;
		bool isDef;
	};
	struct NameVal : public KeyHolder {
		union {
			TiXmlElement *xml;
			int cnt;
		};
	};

	TiXmlDocument *doc;
	TiXmlElement *xml;
	GraphMLGraph *graphs; //cyclic list
	typedef std::map<std::string, GraphMLGraph*> NameGraph;
	typedef std::map<std::string, NameDef> NameDefs;
	typedef std::map<std::string, NameVal> NameVals;
	NameGraph nameGraph;
	NameDefs nameDefs;
	NameVals nameVals;
};

/* ------------------------------------------------------------------------- *
 * GraphMLGraph
 *
 * ------------------------------------------------------------------------- */

class GraphMLGraph {
	friend class GraphML;
public:
	std::string getName();

	//read from graph to graphml
	template<typename Graph>
	bool readGraph(Graph &graph);
	//InfoVertex has to have Graph::VertInfoType operator()(GraphMLKeysRead *)
	//InfoEdge has to have Graph::EdgeInfoType operator()(GraphMLKeysRead *)
	template<typename Graph, typename InfoVertex, typename InfoEdge>
	bool readGraph(Graph &graph, InfoVertex infoVert, InfoEdge infoEdge);
	template<typename Graph>
	bool readGraph(Graph &graph, BlackHole, BlackHole);
	template<typename Graph, typename InfoEdge>
	bool readGraph(Graph &graph, BlackHole, InfoEdge infoEdge);
	template<typename Graph, typename InfoVertex>
	bool readGraph(Graph &graph, InfoVertex infoVert, BlackHole);

	//write to graph from graphml
	template<typename Graph>
	bool writeGraph(const Graph &graph);
	//InfoVertex has to have void operator()(Graph::PVertex, GraphMLKeysWrite *)
	//InfoEdge has to have void operator()(Graph::PEdge, GraphMLKeysRead *)
	template<typename Graph, typename InfoVertex, typename InfoEdge>
	bool writeGraph(const Graph &graph, InfoVertex infoVert, InfoEdge infoEdge);
	template<typename Graph>
	bool writeGraph(const Graph &graph, BlackHole, BlackHole);
	template<typename Graph, typename InfoEdge>
	bool writeGraph(const Graph &graph, BlackHole, InfoEdge infoEdge);
	template<typename Graph, typename InfoVertex>
	bool writeGraph(const Graph &graph, InfoVertex infoVert, BlackHole);

	GraphMLKeyTypes::Type getKeyType(const char *name);
	GraphMLKeyTypes::ForKey getKeyFor(const char *name);

	//return all defined keys for Graph or All
	//res is a map : string->GraphMLKeyTypes::Type
	template <class AssocCont> void getKeys(AssocCont& res);
	//reading and writing graphML properties
	bool setBool(const char *name, bool val);
	bool setInt(const char *name, int val);
	bool setLong(const char *name, int64_t val);
	bool setDouble(const char *name, double val);
	bool setString(const char *name, const char *val);
	bool setString(const char *name, const std::string &val);

	bool isValue(const char *name);
	bool getBool(const char *name);
	int getInt(const char *name);
	int64_t getLong(const char *name);
	double getDouble(const char *name);
	std::string getString(const char *name);
private:
	GraphMLGraph();
	~GraphMLGraph();
	template<GraphMLKeyTypes::Type, typename InType>
	bool set(const char *name, InType val);
	template<typename InOutType>
	InOutType get(const char *name, const InOutType def);
	void readXML();

	GraphMLGraph *prev, *next;
	TiXmlElement *xml;
	GraphML *graphML;
	GraphML::NameVals nameVals;
};

/* ------------------------------------------------------------------------- *
 * GraphMLKeyVal
 *
 * ------------------------------------------------------------------------- */

class GraphMLKeysRead {
	friend class GraphMLGraph;
public:
	GraphMLKeyTypes::Type getKeyType(const char *name);
	GraphMLKeyTypes::ForKey getKeyFor(const char *name);

	//return all defined keys for this->forKey or All
	//res is a map : string->GraphMLKeyTypes::Type
	template <class AssocCont> void getKeys(AssocCont& res);
	bool isValue(const char *name);
	bool getBool(const char *name);
	int getInt(const char *name);
	int64_t getLong(const char *name);
	double getDouble(const char *name);
	std::string getString(const char *name);
private:
	template<typename InOutType>
	InOutType get(const char *name, const InOutType def);
	bool set(const char *key, const char *val);
	GraphMLKeyTypes::ForKey forKey;
	GraphML *graphML;
	GraphML::NameVals nameVals;
	int cnt;
};

class GraphMLKeysWrite {
	friend class GraphMLGraph;
public:
	GraphMLKeyTypes::Type getKeyType(const char *name);
	GraphMLKeyTypes::ForKey getKeyFor(const char *name);

	//return all defined keys for this->forKey or All
	//res is a map : string->GraphMLKeyTypes::Type
	template <class AssocCont> void getKeys(AssocCont& res);
	bool setBool(const char *name, bool val);
	bool setInt(const char *name, int val);
	bool setLong(const char *name, int64_t val);
	bool setDouble(const char *name, double val);
	bool setString(const char *name, const char *val);
	bool setString(const char *name, const std::string &val);
private:
	template<GraphMLKeyTypes::Type, typename InType>
	bool set(const char *name, InType val);
	std::string print(); ////not yet
	GraphMLKeyTypes::ForKey forKey;
	GraphML *graphML;
	GraphML::NameVals nameVals;
	int cnt;
};


namespace Privates {

template< class Ch1, class Ch2 > struct GMLSumField
{
	Ch1 ch1;
	Ch2 ch2;

	typedef GMLSumField< Ch1,Ch2 > GMLFieldSelfType;
	typedef typename Ch1::Type Type;

	GMLSumField( Ch1 a = Ch1(), Ch2 b = Ch2() ): ch1( a ), ch2( b ) { }

	void read(Type &arg, Koala::IO::GraphMLKeysRead *gmlkr) {
		ch1.read(arg, gmlkr);
		ch2.read(arg, gmlkr);
	}

	typename Ch1::Type operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		typename Ch1::Type res;
		read(res, gmlkr);
		return res;
	}

	template <class T>
	void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
		ch1(vert, gmlkw);
		ch2(vert, gmlkw);
	}
};

template< class Ch1, class Ch2 >
GMLSumField< typename Ch1::GMLFieldSelfType,typename Ch2::GMLFieldSelfType >
operator&( Ch1 a, Ch2 b ) { return GMLSumField< Ch1,Ch2 >( a,b ); }

struct GMLBoolFieldPlain {
	std::string name;
	GMLBoolFieldPlain(std::string aname) : name(aname) {}
	bool operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		return gmlkr->getBool(name.c_str());
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setBool(name.c_str(), (bool)(vertedge->info));
	}
};

struct GMLIntFieldPlain {
	std::string name;
	GMLIntFieldPlain(std::string aname) : name(aname) {}
	int operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		return gmlkr->getInt(name.c_str());
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setInt(name.c_str(), (int)(vertedge->info));
	}
};

struct GMLDoubleFieldPlain {
	std::string name;
	GMLDoubleFieldPlain(std::string aname) : name(aname) {}
	double operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		return gmlkr->getDouble(name.c_str());
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setDouble(name.c_str(), (double)(vertedge->info));
	}
};

struct GMLLongFieldPlain {
	std::string name;
	GMLLongFieldPlain(std::string aname) : name(aname) {}
	int64_t operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		return gmlkr->getLong(name.c_str());
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setLong(name.c_str(), (int64_t)(vertedge->info));
	}
};

struct GMLStringPlain {
	std::string str;
	GMLStringPlain(const std::string &s): str(s) {}
	operator char() const { return str.at(0); }
	operator unsigned char() const { return str.at(0); }
	operator std::string() const { return str; }
};

struct GMLStringFieldPlain {
	std::string name;
	GMLStringFieldPlain(std::string aname) : name(aname) {}
	GMLStringPlain operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		return gmlkr->getString(name.c_str());
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
	    std::string str;
	    str+=vertedge->info;
		gmlkw->setString(name.c_str(), str);
	}
};

template <class Info, class FieldType>
struct GMLBoolField {
	std::string name;
	FieldType Info::*wsk;
	typedef GMLBoolField<Info, FieldType> GMLFieldSelfType;
	typedef Info Type;

	GMLBoolField(FieldType Info::*awsk, std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		arg.*wsk=(FieldType)gmlkr->getBool(name.c_str());
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setBool(name.c_str(), (bool)(vertedge->info.*wsk));
	}
};

template <class Info, class FieldType>
struct GMLIntField {
	std::string name;
	FieldType Info::*wsk;
	typedef GMLIntField<Info, FieldType> GMLFieldSelfType;
	typedef Info Type;

	GMLIntField(FieldType Info::*awsk, std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		arg.*wsk=(FieldType)gmlkr->getInt(name.c_str());
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setInt(name.c_str(), (int)(vertedge->info.*wsk));
	}
};

template <class Info, class FieldType>
struct GMLDoubleField {
	std::string name;
	FieldType Info::*wsk;
	typedef GMLDoubleField<Info, FieldType> GMLFieldSelfType;
	typedef Info Type;

	GMLDoubleField(FieldType Info::*awsk, std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		arg.*wsk=(FieldType)gmlkr->getDouble(name.c_str());
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setDouble(name.c_str(), (double)(vertedge->info.*wsk));
	}
};

template <class Info, class FieldType>
struct GMLLongField {
	std::string name;
	FieldType Info::*wsk;
	typedef GMLLongField<Info, FieldType> GMLFieldSelfType;
	typedef Info Type;

	GMLLongField(FieldType Info::*awsk, std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		arg.*wsk=(FieldType)gmlkr->getLong(name.c_str());
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setLong(name.c_str(), (int64_t)(vertedge->info.*wsk));
	}
};

template <class Info, class FieldType>
struct GMLStringField {
	std::string name;
	FieldType Info::*wsk;
	typedef GMLStringField< Info, FieldType> GMLFieldSelfType;
	typedef Info Type;

	GMLStringField(FieldType Info::*awsk, std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		arg.*wsk=(FieldType)(gmlkr->getString(name.c_str()));
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setString(name.c_str(), (const std::string)(vertedge->info.*wsk));
	}
};

template<class Info>
struct GMLStringField<Info, char> {
	std::string name;
	char Info::*wsk;
	typedef GMLStringField< Info, char> GMLFieldSelfType;
	typedef Info Type;

	GMLStringField(char Info::*awsk, std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		arg.*wsk=(char)(gmlkr->getString(name.c_str())).at(0);
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		std::string str;
		str+=vertedge->info.*wsk;
		gmlkw->setString(name.c_str(), str);
	}
};

template<class Info>
struct GMLStringField<Info, unsigned char> {
	std::string name;
	unsigned char Info::*wsk;
	typedef GMLStringField< Info, unsigned char> GMLFieldSelfType;
	typedef Info Type;

	GMLStringField(unsigned char Info::*awsk, std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		arg.*wsk=(unsigned char)(gmlkr->getString(name.c_str())).at(0);
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vertedge, Koala::IO::GraphMLKeysWrite *gmlkw) {
		std::string str;
		str+=(char)(vertedge->info.*wsk);
		gmlkw->setString(name.c_str(), str);
	}
};

template <class Info, class Char, int N >
struct GMLCharField {
	std::string name;
	Char (Info::*wsk)[N];
	typedef GMLCharField<Info, Char, N> GMLFieldSelfType;
	typedef Info Type;

	GMLCharField(Char (Info::*awsk)[N], std::string aname) : name(aname), wsk(awsk) {}
	void read(Info& arg,Koala::IO::GraphMLKeysRead *gmlkr) {
		std::strncpy((char*)(arg.*wsk),(gmlkr->getString(name.c_str())).c_str(),N);
		(arg.*wsk)[N-1] = 0;
	}
	Info operator()(Koala::IO::GraphMLKeysRead *gmlkr) {
		Info res;
		read(res, gmlkr);
		return res;
	}
	template <class T>
	void operator()( T vert, Koala::IO::GraphMLKeysWrite *gmlkw) {
		gmlkw->setString(name.c_str(), (char*)(vert->info.*wsk));
	}
};

template <class Info, int N >
struct GMLStringField <Info, char[N]> : public GMLCharField <Info, char, N>
{
	typedef GMLStringField <Info,char[N]> GMLFieldSelfType;
	typedef Info Type;

	GMLStringField(char (Info::*awsk)[N], std::string aname)
		: GMLCharField <Info, char, N> (awsk,aname)
		{}
};

template <class Info, int N >
struct GMLStringField<Info,unsigned char[N]> : public GMLCharField <Info, char, N>
{
	typedef GMLStringField <Info,unsigned char[N]> GMLFieldSelfType;
	typedef Info Type;

	GMLStringField(unsigned char (Info::*awsk)[N], std::string aname)
		: GMLCharField <Info, unsigned char, N> (awsk,aname)
		{}
};

} //namespace Privates

//these function can be used in readGraph/writeGraph as InfoVertex/InfoEdge
Privates::GMLBoolFieldPlain
gmlBoolField(std::string name) {
	return Privates::GMLBoolFieldPlain(name);
}
template <class Info, class FieldType>
Privates::GMLBoolField<Info,FieldType>
gmlBoolField(FieldType Info::*wsk,std::string name) {
	return Privates::GMLBoolField<Info,FieldType>(wsk,name);
}

Privates::GMLIntFieldPlain
gmlIntField(std::string name) {
	return Privates::GMLIntFieldPlain(name);
}
template <class Info, class FieldType>
Privates::GMLIntField<Info,FieldType>
gmlIntField(FieldType Info::*wsk,std::string name) {
	return Privates::GMLIntField<Info,FieldType>(wsk,name);
}

Privates::GMLDoubleFieldPlain
gmlDoubleField(std::string name) {
	return Privates::GMLDoubleFieldPlain(name);
}
template <class Info, class FieldType>
Privates::GMLDoubleField<Info,FieldType>
gmlDoubleField(FieldType Info::*wsk,std::string name) {
	return Privates::GMLDoubleField<Info,FieldType>(wsk,name);
}

Privates::GMLLongFieldPlain
gmlLongField(std::string name) {
	return Privates::GMLLongFieldPlain(name);
}
template <class Info, class FieldType>
Privates::GMLLongField<Info,FieldType>
gmlLongField(FieldType Info::*wsk,std::string name) {
	return Privates::GMLLongField<Info,FieldType>(wsk,name);
}

Privates::GMLStringFieldPlain
gmlStringField(std::string name) {
	return Privates::GMLStringFieldPlain(name);
}
template <class Info, class FieldType>
Privates::GMLStringField<Info,FieldType>
gmlStringField(FieldType Info::*wsk,std::string name) {
	return Privates::GMLStringField<Info,FieldType>(wsk,name);
}
template <class Info,int N>
Privates::GMLStringField<Info,char[N]>
gmlStringField(char (Info::*wsk)[N],std::string name) {
	return Privates::GMLStringField<Info,char [N]>(wsk,name);
}
template <class Info,int N>
Privates::GMLStringField<Info,unsigned char[N]>
gmlStringField(unsigned char (Info::*wsk)[N],std::string name) {
	return Privates::GMLStringField<Info,unsigned char [N]>(wsk,name);
}


/* ------------------------------------------------------------------------- *
 * GraphML
 *
 * ------------------------------------------------------------------------- */


#include "graphml.hpp"
}
}

#endif
