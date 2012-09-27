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


/* ------------------------------------------------------------------------- *
 * GraphML
 *
 * ------------------------------------------------------------------------- */


#include "graphml.hpp"
}
}

#endif
