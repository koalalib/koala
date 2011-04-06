#ifndef KOALA_GRAPHML_H
#define KOALA_GRAPHML_H

#include "../graph/graph.h"
#include "../tinyxml/tinyxml.h"
#include "../base/kstd.h"
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/** This module uses TinyXML "http://sourceforge.net/projects/tinyxml/"
	You have to add to project four files:
	tinystr.cpp, tinyxml.cpp, tinyxmlerror.cpp, tinyxmlparser.cpp.
	Files are in Koala directory in tinyxml.
*/
namespace Koala {
namespace IO {

class GraphML;

struct GraphMLKeyVal {
	std::string id, name; // <graph> attributes id="" and attr.name=""
	enum Type {None, NotDefined, Bool, Int, Long, Float, Double, String} type;
	//NotDefined when there is no attr.type; None when Key is created
	enum ForKey {All, Graph, Node, Edge} forKey;
	bool is_def;
	union {
		int intVal;
		double dblVal;
		int64_t longVal;
	} pDef, pUsr; // pDef - default value, pUsr - user value
	std::string sDef,sUsr; // as above
	int count; //count is used to check if it should be used default value or user value
	GraphMLKeyVal();
	std::string print(bool isUsr);
	bool setDef(const char *val);
	bool setUsr(const char *val);
};

typedef std::vector<GraphMLKeyVal> GraphMLKeys;

/* KeyHolder holds keys while reading from GraphML.
	You should use only get.... methods.
	Key has name and type. It also has sometimes default
*/
class KeysHolderBase {
	friend class GraphML;
	int count;

	void next();
	void clearCount();
	void setValUsr();
	void clear();
protected:
	GraphMLKeys *defs;

	void touch(int id);
	bool modified(int id);
	std::string print(int id, bool isUsr);

	int createKey(const char *keyId, const char *keyName);
	int createKey(const char *keyName); //random keyId; @return 0 - can't create key
	int newVal(GraphMLKeyVal&);
//	void setVal(int id, GraphMLKeyVal &);

	int getId(const char *keyName); //it's just a number not the id from xml
	//if there is no key with keyName name, then return 0;
	int getIdFromKeyId(const char *keyId);
	std::string getKeyId(int id);
	std::string getKeyName(int id);

	GraphMLKeyVal::Type getKeyType(int id);
	GraphMLKeyVal::ForKey getKeyFor(int id);

	std::string getString(int id);
	int getInt(int id);
	int64_t getLong(int id);
	double getDouble(int id);

	void setKeyType(int id, GraphMLKeyVal::Type type);
	void setKeyFor(int id, GraphMLKeyVal::ForKey forKey);

	void setVal(int id, const char *val);
	void setVal(int id, std::string &val);
	void setVal(int id, int val);
	void setVal(int id, int64_t val);
	void setVal(int id, double val);

	void setValUsr(int id, const char *val);
	void setValDef(int id, const char *val);
public:
	KeysHolderBase();
	KeysHolderBase(const KeysHolderBase &khBase);
	~KeysHolderBase();
	int getKeysNo();
	int getCount(); //get internal counter
};

class KeysHolderRead : public KeysHolderBase {
	friend class GraphML;
	char type; //0 - without checking, 1 - graph, 2 - vertex, 3 - edge
public:
	KeysHolderRead(const KeysHolderBase &);
	~KeysHolderRead();
	int getId(const char *keyName); //it's just a number not the id from xml
	//if there is no key with keyName name, then return 0;
	bool valid(int id);

	std::string getKeyId(int id); //get id from xml <key> node
	std::string getKeyName(int id);
	GraphMLKeyVal::Type getKeyType(int id);
	GraphMLKeyVal::ForKey getKeyFor(int id);

	bool getBool(int id);
	int getInt(int id);
	int64_t getLong(int id);
	double getDouble(int id);
	std::string getString(int id);
};

class KeysHolderWrite : public KeysHolderBase {
	friend class GraphML;
	char type; //0 - without checking, 1 - graph, 2 - vertex, 3 - edge
public:
	KeysHolderWrite(const KeysHolderBase &khBase);
	~KeysHolderWrite();
	int getIdCr(const char *keyName); //it's just a number not the id from xml
	//if there is no key with keyName name, then there is created one
	//return 0 if there is key but with different forKey
	int getIdNoCr(const char *keyName); //like getId but without creation
	bool valid(int id);

	std::string getKeyId(int id); //get id from xml <key> node
	std::string getKeyName(int id);
	GraphMLKeyVal::Type getKeyType(int id);
	GraphMLKeyVal::ForKey getKeyFor(int id);

	bool setNotDef(int id, const char *val);
	bool setBool(int id, bool val);
	bool setInt(int id, int val);
	bool setLong(int id, int64_t val);
	bool setDouble(int id, double val);
	bool setString(int id, const char *val);
};

/** Class is only for writing and reading parameters of a graph.*/
class KeysHolderGraph : public KeysHolderBase {
	friend class GraphML;
public:
	KeysHolderGraph();
	~KeysHolderGraph();
	int getIdCr(const char *keyName);//it's just a number not the id from xml
	//if there is no key with keyName name, then there is created one
	//return 0 if there is key but with different forKey
	int getIdNoCr(const char *keyName); //like getId but without creation
	bool valid(int id);

	std::string getKeyId(int id); //get id from xml <key> node
	std::string getKeyName(int id);
	GraphMLKeyVal::Type getKeyType(int id);
	GraphMLKeyVal::ForKey getKeyFor(int id);

	bool setBool(int id, bool val);
	bool setInt(int id, int val);
	bool setLong(int id, int64_t val);
	bool setDouble(int id, double val);
	bool setString(int id, const char *val);

	bool getBool(int id);
	int getInt(int id);
	int64_t getLong(int id);
	double getDouble(int id);
	std::string getString(int id);
};


class GraphML {
public:
	GraphML();
	~GraphML();
	bool readFile(const char*); ///@arg name of a file @return if reading succseed
	bool writeFile(const char*); ///@arg name of a file @return if writeing succseed
	void clear(); /// Clears all entries from GraphML
	int graphNo(); /// @return Number of graphs in GraphML object

	std::string getGraphName(int); /// @return Name of the n-th graph from graphMl object. Name is empty if method failed.
	int isGraphName(const char *); /// @return Graph named 'name' position. Negative value if method failed.

	template<class Graph> bool readGraph(Graph&, const char*); ///Check if graph named 'name' is in GraphML object. If it is, then reads the graph from GraphML.
	template<class Graph> bool readGraph(Graph&, int); ///Reads n-th graph from GraphML.
	//readGraph methods return false if(or):
	//1. there is no graphml file read
	//2. graphml file has improper format
	//3. there is no graph with name 'name'(graphml don't contain n-th graph)
	template<class Graph> bool writeGraph(const Graph&, const char*); ///Write graph to GraphML object. The graph named 'name'.
	//writeGraph methods return false if(or):
	//1. GraphML object couldn't create initial structure
	//2. imported graphML file was corrupted
	//3. there is graph in GraphML object with name 'name'

	template<class Graph, class InfoVertex, class InfoEdge>
			bool readGraph(Graph&, const char*, InfoVertex, InfoEdge); ///Like prevous readGraph. It reads also vertices and edges properties.
	template<class Graph, class InfoVertex, class InfoEdge>
			bool readGraph(Graph&, int, InfoVertex, InfoEdge); ///Like prevous readGraph. It reads also vertices and edges properties.
	template<class Graph, class InfoVertex, class InfoEdge>
			bool writeGraph(const Graph&, const char*, InfoVertex, InfoEdge); ///Like prevous writeGraph. It writes also vertices and edges properties.

	bool deleteGraph(const char *); ///Deletes graph from GraphML object. @return false is there is no graph with name 'name'
	//read parameters
	bool readGraphParam(const char*, KeysHolderGraph&);
	bool readGraphParam(int, KeysHolderGraph&);
	/** Between KeysHolder data initialization and writeGraphParam there shouldn't be any GraphML changes. */
	bool writeGraphParam(const char*, KeysHolderGraph&);
	bool writeGraphParam(int, KeysHolderGraph&);
private:
	KeysHolderBase keysHolder;
	TiXmlDocument *doc;
	template<class Graph> bool readGraph(Graph&, TiXmlElement*);
	template<class Graph, class InfoVertex, class InfoEdge>
			bool readGraph(Graph&, TiXmlElement*, InfoVertex, InfoEdge);
	bool readGraphParam(TiXmlElement*, KeysHolderGraph&);
	bool writeGraphParam(TiXmlElement*, KeysHolderGraph&);
	bool createInitial();
	bool readKeys();
	void writeKeys();
};

#include "graphml.hpp"
} //namespace IO
} //namespace Koala

#endif
