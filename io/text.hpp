#include<set>
#include<map>
#include<utility>

namespace Koala {
namespace IO {

/** move object info in '(info)' format from input stream to string stream
 * and strip parentheses
 * @param[in] strm stream to read from
 * @param[out] out string stream to put info to
 * @return true on success, false otherwise
 */
static bool readObjectInfo(std::istream &strm, std::istringstream &out) {
	int parCount = 0;
	char c;
	bool escape;
	std::string text;

	if(!(bool)(strm >> c)) return false;
	if(c != '(') { strm.unget(); return false; };

	c = strm.get();		// switch to unformated reading to keep spaces
	while(strm.good()) {
		if(c == '\\') {
			escape = true;
			c = strm.get();
			if(!strm.good()) return false;
		} else escape = false;
		if(!escape && c == '(') parCount++;
		else if(!escape && c == ')') {
			parCount--;
			if(parCount < 0) {
				out.clear();
				out.str(text);
				return true;
				};
			};
		text.push_back(c);
		c = strm.get();
		};
	return false;
	};


/** try to read output id in '@id' format from stream
 * @param[in] strm stream to read from
 * @param[out] id place to put the read id to
 * @return true when id was read, false if there is no id to read or on error
 */
bool readOutputId(std::istream &strm, unsigned int &id) {
	char at;
	if(!(bool)(strm >> at)) return false;
	if(at != '@') { strm.unget(); return false; };
	if(!(bool)(strm >> id)) return false;
	return true;
	};


/** append a graph described by a stream in a Vertex-List format
 * @param[out] graph to read to (it will NOT be cleared)
 * @param[in] strm stream to read graph from
 * @param[in] directed if true then edges will be directed by default,
 * 	      if false then edges will be undirected by default
 * @param[out] vertexMap place to store id-ed vertices
 * @param[out] edgeMap place to store id-ed edges
 * @return true on success, false otherwise
 */
template<class Graph, class VMap, class EMap>
bool readGraphVL(Graph &g, std::istream &strm, bool directed,
		 VMap &vertexMap, EMap &edgeMap) {
	char c;
	unsigned int i, id, m, iu, iv;
	std::istringstream ostrm;
	EdgeDirection dir;
	typename Graph::PEdge e;
	typename Graph::PVertex u, v;
	std::map<unsigned int, typename Graph::PVertex > idxToPtr;
	typename std::map<unsigned int, typename Graph::PVertex >::iterator it;

	while((strm >> iu)) {
		it = idxToPtr.find(iu);
		if(it == idxToPtr.end()) idxToPtr[iu] = u = g.addVert();
		else u = it->second;

		if(readObjectInfo(strm, ostrm) && !(bool)(ostrm >> (u->info))) return false;
		if(readOutputId(strm, id)) vertexMap[id] = u;

		if(!(bool)(strm >> m)) return false;
		for(i = 0; i < m; i++) {
			if(!(bool)(strm >> c)) return false;
			if(c == '-') dir = EdUndir;
			else if(c == '>') dir = EdDirOut;
			else {
				strm.unget();
				dir = directed ? EdDirOut : EdUndir;
				};

			if(!(bool)(strm >> iv)) return false;
			it = idxToPtr.find(iv);
			if(it == idxToPtr.end()) idxToPtr[iv] = v = g.addVert();
			else v = it->second;

			if(u == v) dir = EdLoop;
			e = g.addEdge(u, v, dir);

			if(readObjectInfo(strm, ostrm) && !(bool)(ostrm >> (e->info))) return false;
			if(readOutputId(strm, id)) edgeMap[id] = e;
			};
		};
	return true;
	};


/** append a graph described by a stream in a Edge-List format
 * @param[out] graph to read to (it will NOT be cleared)
 * @param[in] strm stream to read graph from
 * @param[in] directed if true then added edges will be directed (from 1 to 2),
 * 		if false then added edges will be undirected
 * @param[out] vertexMap
 * @return true on success, false otherwise
 */
template<class Graph, class VMap, class EMap>
bool readGraphEL(Graph &g, std::istream &strm, bool directed,
		 VMap &vertexMap, EMap &edgeMap) {
	char c;
	bool vertexMode;
	std::string str;
	std::istringstream ostrm;
	EdgeDirection dir;
	unsigned int id, iu, iv;
	typename Graph::PEdge e;
	typename Graph::PVertex u, v;
	std::map<unsigned int, typename Graph::PVertex> idxToPtr;
	typename std::map<unsigned int, typename Graph::PVertex>::iterator it;

	vertexMode = false;
	while((strm >> iu)) {
		it = idxToPtr.find(iu);
		if(it == idxToPtr.end()) idxToPtr[iu] = u = g.addVert();
		else u = it->second;

		if(vertexMode) {	// read vertex data
			if(readObjectInfo(strm, ostrm) && !(bool)(ostrm >> (u->info))) return false;
			if(readOutputId(strm, id)) vertexMap[id] = u;
		} else {		// read edges with edge data
			if(!(bool)(strm >> c)) return false;
			if(c == '<' || c == '_') {
				strm >> str;
				str = c + str;
				if(str == "->") dir = EdDirOut;
				else if(str == "<-") dir = EdDirIn;
				else if(str == "<->") dir = EdUndir;
				else if(str == "-") dir = EdUndir;
				else return false;
			} else {
				strm.unget();
				dir = directed ? EdDirOut : EdUndir;
				};

			it = idxToPtr.find(iv);
			if(it == idxToPtr.end()) idxToPtr[iv] = v = g.addVert();
			else v = it->second;

			if(u == v) dir = EdLoop;
			e = g.addEdge(u, v, dir);

			if(readObjectInfo(strm, ostrm) && !(bool)(ostrm >> (e->info))) return false;
			if(readOutputId(strm, id)) edgeMap[id] = e;
			};
		};
	return true;
	};


/** append a graph described by a stream
 * requires overloading >> operator for std::istream for VertexInfo and EdgeInfo
 * @param[out] graph to read to (it will NOT be cleared)
 * @param[in] strm stream to read graph from
 * @param[in] format describes format of the stream (RG_* values)
 * @param[out] vertexMap
 * @return true on success, false otherwise
 */
template<typename Graph, class VMap, class EMap>
bool readGraphText(Graph &g, std::istream &strm, RG_Format format,
		   VMap &vertexMap, EMap &edgeMap) {
	switch(format) {
		case RG_DirectedVertexLists:	return readGraphVL(g, strm, true, vertexMap, edgeMap);
		case RG_UndirectedVertexLists:	return readGraphVL(g, strm, false, vertexMap, edgeMap);
		case RG_DirectedEdgeList:	return readGraphEL(g, strm, true, vertexMap, edgeMap);
		case RG_UndirectedEdgeList:	return readGraphEL(g, strm, false, vertexMap, edgeMap);
		};
	return false;
	};


/** write a graph to the stream in a Vertex-List format
 * @param[out] graph to write
 * @param[in] strm stream to write graph to
 * @param[in] directed if true then added edges will be directed, if false then
 * 		added edges will be undirected
 * @return true on success, false otherwise
 */
template<class Graph>
bool writeGraphVL(const Graph &g, std::ostream &out, bool directed) {
	unsigned int i;
	EdgeDirection flags;
	typename Graph::PEdge e;
	typename Graph::PVertex u, v;
	std::set<typename Graph::PEdge> used;
	std::map<typename Graph::PVertex , unsigned int> ptrToIdx;
	std::pair<typename Graph::PVertex , typename Graph::PVertex> vs;

	for(u = g.getVert(), i = 0; u != NULL; u = g.getVertNext(u))
		ptrToIdx[u] = i++;

	flags = EdLoop | EdDirOut | EdUndir;

	for(u = g.getVert(); u != NULL; u = g.getVertNext(u)) {
		out << ptrToIdx[u] << '(' << u->info << ')';

		for(i = 0, e = g.getEdge(u, flags); e != NULL; e = g.getEdgeNext(u, e, flags)) {
			vs = g.getEdgeEnds(e);
			if(g.getType(e) == Directed && vs.first != u) continue;
			if(used.find(e) != used.end()) continue;
			i++;
			};

		out << ' ' << i;

		for(e = g.getEdge(u, flags); e != NULL; e = g.getEdgeNext(u, e, flags)) {
			if(used.find(e) != used.end()) continue;
			vs = g.getEdgeEnds(e);
			if(g.getType(e) == Directed && vs.first != u) continue;
			if(vs.first == u) v = vs.second;
			else v = vs.first;
			out << ' ';
			if(g.getType(e) == Undirected) out << '-';
			else out << '>';
			out << ptrToIdx[v] << '(' << e->info << ')';
			used.insert(e);
			};
		out << '\n';
		};
	out.flush();
	return true;
	};


/** append a graph described by a stream in a Edge-List format
 * @param[out] graph to read to (it will NOT be cleared)
 * @param[in] strm stream to read graph from
 * @param[in] directed if true then added edges will be directed (from 1 to 2),
 * 		if false then added edges will be undirected
 * @param[out] vertexMap
 * @return true on success, false otherwise
 */
template<class Graph>
bool writeGraphEL(const Graph &g, std::ostream &out, bool directed) {
	unsigned int idx;
	typename Graph::PEdge e;
	typename Graph::PVertex u;
	std::pair<typename Graph::PVertex , typename Graph::PVertex > vs;
	std::map<typename Graph::PVertex , unsigned int> ptrToIdx;

	idx = 0;
	for(e = g.getEdge(); e != NULL; e = g.getEdgeNext(e)) {
		vs = g.getEdgeEnds(e);
		if(ptrToIdx.find(vs.first) == ptrToIdx.end()) ptrToIdx[vs.first] = idx++;
		if(ptrToIdx.find(vs.second) == ptrToIdx.end()) ptrToIdx[vs.second] = idx++;
		out << ptrToIdx[vs.first] << ' ';
		if(g.getType(e) == Undirected) out << "<->";
		else out << "->";	// loop and undirected
		out << ' ' << ptrToIdx[vs.second];
		out << "(" << e->info << ")\n";
		};

	for(u = g.getVert(); u != NULL; u = g.getVertNext(u)) {
		if(ptrToIdx.find(u) == ptrToIdx.end()) ptrToIdx[u] = idx++;
		out << ptrToIdx[u] << '(' << u->info << ")\n";
		};
	return true;
	};



/** output a graph to the given stream
 * requires overloading << operator for std::ostream for VertexInfo and EdgeInfo
 * @param[out] graph to read to (it will NOT be cleared)
 * @param[in] strm stream to read graph from
 * @param[in] format describes format of the stream (RG_* values)
 * @return true on success, false otherwise
 */
template<class Graph>
bool writeGraphText(const Graph &g, std::ostream &out, RG_Format format) {
	switch(format) {
		case RG_DirectedVertexLists:	return writeGraphVL(g, out, true);
		case RG_UndirectedVertexLists:	return writeGraphVL(g, out, false);
		case RG_DirectedEdgeList:	return writeGraphEL(g, out, true);
		case RG_UndirectedEdgeList:	return writeGraphEL(g, out, false);
		};
	return false;
	};

}; // namespace InOut
}; // namespace Koala
