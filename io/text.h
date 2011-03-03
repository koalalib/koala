/* functions:
 *
 * readGraphText(graph, text, format, [vertexmap, [edgemap]])
 *  read graph from text in a given format
 *  graph	- graph to read to (will not be cleared before reading)
 *  text	- descrioption of graph (const char *, string or std::istream)
 *  format	- one of RG_* constants
 *  vertexmap	- place to store selected vertices (with output-id), usually
 *		  table or map (need to implement "writable" operator[](unsigned int)
 *  edgemap	- place to store selected edges (with output-id), usually
 *		  table or map (need to implement "writable" operator[](unsigned int)
 *
 * writeGraphText(graph, text, format)
 *  write graph in a given format (RG_*) to text
 *  graph	- graph to write
 *  text	- output buffer (string or std::ostream)
 *  format	- one of RG_* constants
 *
 * writeGraphText(graph, text, maxlength, format)
 *  write graph in a given format (RG_*) to text
 *  graph	- graph to write
 *  text	- char *
 *  maxlength	- size of the text buffer
 *  format	- one of RG_* constants
 *
 *
 * supported formats:
 *
 * VertexLists:
 *  <vertex-1> <k_1> <edge-1> <edge-2> ... <edge-k_1>
 *  ...
 *  <vertex-n> <k_n> <edge-1> <edge-2> ... <edge-k_n>
 *
 *  each line descibes edges with coming from vertex-i
 *
 *
 *  vertex-i can have one of the following formats:
 *  <vertex-id> ( <vertex-info> ) @ <output-id>
 *  <vertex-id>: (nonnegative integer) vertex identifier (arbitrary)
 *  <vertex-info>: string with matched parentheses (use \ to escape unmatched ones)
 *                 describing the VertexInfo (istream >> VertexInfo should work)
 *                 (optional)
 *  <output-id>: (nonnegative integer) key in the output map of the vertex
 *  		(optional)
 *
 *  edge-i can have of the following formats:
 *  <type> <vertex-id> ( <edge-info> ) @ <output-id>
 *  <type> <vertex-id>: (nonnegative integer) vertex identifier (arbitrary)
 *  <type>: (optional) d - directed, u - undirected
 *  <edge-info>: string with matched parentheses (use \ to escape unmatched ones)
 *               describing the EdgeInfo (istream >> EdgeInfo should work)
 *               (optional)
 *  <output-id>: (nonnegative integer) key in the output map of the edge
 *               (optional)
 *
 *  RG_UndirectedVertexLists - edges without type are undirected
 *  RG_DirectedVertexLists - edges without type are directed
 *
 *  examples:
 *  0 3 1 2 3		- edges (0, 1), (0, 2), (0, 3) with default type
 *  0 4 d1 d2 u3	0	- directed edges (0, 1), (0, 2), undirected edge (0, 3)
 *  			  and loop (0, 0)
 *  3(v3) 1 d0(e0)@5	- vertex 3 has vertex info "v3", add directed edge
 *  			  (3, 0) with edge info "e0" and remember that edge
 *  			  under key 5
 *  2(vertex-2)@1 0	- vertex with info "vertex-2" and no edge, remembered
 *  			  under key 1
 *
 * EdgeList:
 *  a list of edges, followed by a list of vertices:
 *  <vertex-id-1a> <direction> <vertex-id-1b> <edge-info> <output-id>
 *  ...
 *  <vertex-id-ma> <direction> <vertex-id-mb> <edge-info> <output-id>
 *  <vertex-1> <vertex-info> <output-id>
 *  ...
 *  <vertex-n> <vertex-info> <output-id>
 *
 *  vertex-i has the same format as in VertexLists
 *
 *  <vertex-id-ia>, <vertex-id-ib>	- identifiers of edge ends
 *  <direction>				- "<-", "->" (directed edge)
 *   					  "<->" (undirected edge)
 *   					  if absentm use default direction
 *
 *  examples:
 *  1 2			- edge from 1 to 2
 *  1 2 (edge-0)	- edge from 1 to 2 with info "edge-0"
 *  1 2 @3		- edge from 1 to 2 to be remembered under key 3
 *  1 2 (edge-0) @3	- edge from 1 to 2 to be remembered under key 3 with info "edge-0"
 *  1 -> 2		- directed edge from 1 to 2
 *  1 <- 2		- directed edge from 2 to 1
 *  1 <-> 2		- undirected edge between 1 and 2
 *  1 <- 1		- loop
 *  1 -> 1		- loop
 *  1 <-> 1		- loop
 *
 *  RG_UndirectedEdgeList - edges without direction are undirected
 *  RG_DirectedEdgeList - edges without direction are directed
 *
 */
#ifndef KOALA_IO_TEXT_H
#define KOALA_IO_TEXT_H

#include<stdio.h>
#include"../graph/graph.h"

#include<string>
#include<sstream>
#include<iostream>



namespace Koala {
namespace IO {


enum RG_Format {
	RG_DirectedVertexLists,
	RG_UndirectedVertexLists,
	RG_DirectedEdgeList,
	RG_UndirectedEdgeList,
	};


template<class V>
class EmptyMap {
public:
	V &operator[](unsigned int v)	{ return m_dummy; };
	V m_dummy;
	};


template<class Graph, class VMap, class EMap>
bool readGraphText(Graph &g, std::istream &s, RG_Format format,
		   VMap &vertexMap, EMap &edgeMap);


template<class Graph, class VMap, class EMap>
bool readGraphText(Graph &g, const std::string &desc, RG_Format format,
		   VMap &vertexMap, EMap &edgeMap) {
	std::istringstream s;
	s.str(desc);
	return readGraphText(g, s, format, vertexMap, edgeMap);
	};


template<class Graph, class VMap, class EMap>
bool readGraphText(Graph &g, const char *desc, RG_Format format,
		   VMap &vertexMap, EMap &edgeMap) {
	std::istringstream s;
	s.str(std::string(desc));
	return readGraphText(g, s, format, vertexMap, edgeMap);
	};


template<class Graph>
bool readGraphText(Graph &g, std::istream &s, RG_Format format) {
	EmptyMap<typename Graph::PVertex> tv;
	EmptyMap<typename Graph::PEdge> te;
	return readGraphText(g, s, format, tv, te);
	};


template<class Graph>
bool readGraphText(Graph &g, const std::string &desc, RG_Format format) {
	std::istringstream s;
	s.str(desc);
	return readGraphText(g, s, format);
	};


template<class Graph>
bool readGraphText(Graph &g, const char *desc, RG_Format format) {
	std::istringstream s;
	s.str(std::string(desc));
	return readGraphText(g, s, format);
	};


template<class Graph, class DESC, class VMap>
bool readGraphText(Graph &g, DESC desc, RG_Format format, VMap &vertexMap) {
	EmptyMap<typename Graph::PEdge> te;
	return readGraphText(g, desc, format, vertexMap, te);
	};


template<class Graph>
bool writeGraphText(const Graph &g, std::ostream &out, RG_Format format);


template<class Graph>
bool writeGraphText(const Graph &g, std::string &out, RG_Format format) {
	bool rv;
	std::ostringstream s;
	rv = writeGraphText(g, s, format);
	out = s.str();
	return rv;
	};


template<class Graph>
bool writeGraphText(const Graph &g, char *out, unsigned int maxlength, RG_Format format) {
	bool rv;
	const char *o;
	unsigned int i;
	std::string str;
	std::ostringstream s;

	if(out == NULL || maxlength == 0) return false;

	rv = writeGraphText(g, s, format);
	if(!rv) return false;

	str = s.str();
	o = str.c_str();
	maxlength--;
	for(i = 0; o[i] && i < maxlength; i++) out[i] = o[i];
	out[i] = 0;
	return true;
	};

}; // namespace InOut
}; // namespace Koala

#include"text.hpp"
#endif
