/* functions:
 *
 * readGraphText(graph, text, format, [vertexmap, [edgemap]])
 *  read graph from text in a given format
 *  graph	- graph to read to (will not be cleared before reading)
 *  text	- descrioption of graph (const char *, string or std::istream)
 *  format	- combination of RG_* constants
 *  vertexmap	- place to store selected vertices (with output-id), usually
 *		  table or map (need to implement "writable" operator[](unsigned int)
 *  edgemap	- place to store selected edges (with output-id), usually
 *		  table or map (need to implement "writable" operator[](unsigned int)
 *
 * writeGraphText(graph, text, format)
 *  write graph in a given format (RG_*) to text
 *  graph	- graph to write
 *  text	- output buffer (string or std::ostream)
 *  format	- combination of RG_* constants
 *
 * writeGraphText(graph, text, maxlength, format)
 *  write graph in a given format (RG_*) to text
 *  graph	- graph to write
 *  text	- char *
 *  maxlength	- size of the text buffer
 *  format	- combination of RG_* constants
 *
 *
 * supported formats:
 *
 * RG_VertexLists:
 *  n
 *  <vertex-1> <k_1> <edge-1> <edge-2> ... <edge-k_1>
 *  ...
 *  <vertex-n> <k_n> <edge-1> <edge-2> ... <edge-k_n>
 *
 *  first line defines number of vertices
 *  each other line descibes edges adjecent to vertex-i
 *
 *  Each edge should appear exactly once (since parallel edges are allowed).
 *  To describe P2 use:
 *  2
 *  0 1 -1
 *  1 0
 *  the following text:
 *  2
 *  0 1 -1
 *  1 1 -0
 *  gives graph with two vertices and two parallel edges between them
 *
 *
 *  vertex-i can have one of the following formats:
 *  <vertex-id> ( <vertex-info> ) @ <output-id>
 *  where:
 *  <vertex-id>: vertex identifier (nonnegative integer in range 0..n-1)
 *  <vertex-info>: string with matched parentheses (use \ to escape unmatched ones)
 *                 describing the VertexInfo (istream >> VertexInfo should work)
 *                 (optional)
 *  <output-id>: key in the output map of the vertex (nonnegative integer, optional)
 *
 *  edge-i can have of the following formats:
 *  <type> <vertex-id> ( <edge-info> ) @ <output-id>
 *  <type>: defines edge direction: - (undirected), > (to vertex-id), < (from vertex-id), * (loop)
 *  <vertex-id>: vertex identifier of the second end of the edge (loop should have starting vertex repeated here)
 *  <edge-info>: string with matched parentheses (use \ to escape unmatched ones)
 *               describing the EdgeInfo (istream >> EdgeInfo should work)
 *               (optional)
 *  <output-id>: key in the output map of the edge (nonnegative integer, optional)
 *
 *  examples:
 *  0 3 -1 -2 -3	- undirected edges (0, 1), (0, 2), (0, 3)
 *  0 4 >1 >2 -3 *0	- directed edges (0, 1), (0, 2), undirected edge (0, 3)
 *  			  and loop (0, 0)
 *  3(v3) 1 <0(e0)@5	- vertex 3 has vertex info "v3", add directed edge
 *  			  (0, 3) with edge info "e0" and remember that edge
 *  			  under key 5 in output edge map
 *  2(vertex-2)@1 0	- vertex with info "vertex-2" and no edges, remembered
 *  			  under key 1 in output vertex map
 *
 * EdgeList:
 *  a list of edges, followed by a list of vertices:
 *  n m
 *  <vertex-id-1a> <direction> <vertex-id-1b> <edge-info> <output-id>
 *  ...
 *  <vertex-id-ma> <direction> <vertex-id-mb> <edge-info> <output-id>
 *  <vertex-1> <vertex-info> <output-id>
 *  ...
 *  <vertex-n> <vertex-info> <output-id>
 *
 *  first line defines number of vertices (n) and number of edges (m)
 *  following m lines describe edges and another n lines define vertices
 *
 *  vertex-i has the same format as in VertexLists
 *
 *  <vertex-id-ia>, <vertex-id-ib>	- identifiers of edge ends (integers from range 0..n-1)
 *  <direction>				- "<", ">" (directed edge, < means edge ib to ia, > means edge from ia to ib)
 *   					  "-" (undirected edge)
 *   					  "@" (loop)
 *
 *  examples:
 *  1 - 2		- undirected edge between 1 to 2
 *  1 - 2 (edge-0)	- undirected edge between 1 to 2 with info "edge-0"
 *  1 - 2 @3		- undirected edge between 1 to 2 to be remembered under key 3
 *  1 - 2 (edge-0) @3	- undirected edge between 1 to 2 to be remembered under key 3 with info "edge-0"
 *  1 > 2		- directed edge from 1 to 2
 *  1 < 2		- directed edge from 2 to 1
 *  1 @ 1		- loop attached to vertex 1
 *
 * writeGraph* functions allow you to specify, whether the vertex and edge info
 * should be written, eg.:
 * writeGraphText(..., ..., RG_VertexLists)
 *   will print graph in vertex lists format without information
 * writeGraphText(..., ..., RG_VertexLists | RG_Info)
 *   will print graph in vertex lists format with vertex and edge infos
 * writeGraphText(..., ..., RG_VertexLists | RG_EInfo)
 *   will print graph in vertex lists format with edge info only
 * writeGraphText(..., ..., RG_VertexLists | RG_VInfo)
 *   will print graph in vertex lists format with vertex info only
 *
 */
#ifndef KOALA_IO_TEXT_H
#define KOALA_IO_TEXT_H

#include<stdio.h>
#include"../graph/graph.h"

#include<string>
#include<sstream>
#include<iostream>
#include<utility>



namespace Koala {
namespace IO {


enum RG_Format {
	RG_VertexLists=0,
	RG_EdgeList=1,
	RG_VInfo = 2,
	RG_EInfo = 4,
	RG_Info = 6
	};



namespace Privates {

template<class V>
class EmptyMap {
public:
	V &operator[](unsigned int) const	{ return m_dummy; };
	mutable V m_dummy;
	};

class EmptyMap2 {
public:
    template <class T>
        bool hasKey(T) const { return false; }
    template <class T>
        int &operator[](T)	const { return m_dummy; };
	mutable int m_dummy;
	};

}


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
	Privates::EmptyMap<typename Graph::PVertex> tv;
	Privates::EmptyMap<typename Graph::PEdge> te;
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


template<class Graph, class VMap, class EMap>
bool writeGraphText(const Graph &g, std::ostream &out, int format,const VMap& vmap,const EMap& emap);

template<class Graph>
bool writeGraphText(const Graph &g, std::ostream &out, int format)
{
    Privates::EmptyMap2 em;
    return writeGraphText(g,out,format,em,em);
}


template<class Graph, class VMap, class EMap>
bool writeGraphText(const Graph &g, std::string &out, int format,
                    const VMap& vmap,const EMap& emap) {
	bool rv;
	std::ostringstream s;
	rv = writeGraphText(g, s, format,vmap,emap);
	out = s.str();
	return rv;
	};


template<class Graph>
bool writeGraphText(const Graph &g, std::string &out, int format)
{
    Privates::EmptyMap2 em;
    return writeGraphText(g,out,format,em,em);

}

template<class Graph,class VMap, class EMap>
bool writeGraphText(const Graph &g, char *out, unsigned int maxlength, int format,
                    const VMap& vmap,const EMap& emap)
{
	bool rv;
	const char *o;
	unsigned int i;
	std::string str;
	std::ostringstream s;

	if(out == NULL || maxlength == 0) return false;

	rv = writeGraphText(g, s, format, vmap,emap);
	if(!rv) return false;

	str = s.str();
	o = str.c_str();
	maxlength--;
	for(i = 0; o[i] && i < maxlength; i++) out[i] = o[i];
	out[i] = 0;
	return true;
	};

template<class Graph>
bool writeGraphText(const Graph &g, char *out, unsigned int maxlength, int format)
{
    Privates::EmptyMap2 em;
    return writeGraphText(g,out,maxlength,format,em,em);
}


}; // namespace InOut
}; // namespace Koala

#include"text.hpp"
#endif
