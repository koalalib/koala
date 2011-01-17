#ifndef KOALA_GRAPH_H
#define KOALA_GRAPH_H

namespace Koala {
template<class VertInfo, class EdgeInfo>
class Graph;

template<class VertInfo, class EdgeInfo>
class Vertex;

template<class VertInfo, class EdgeInfo>
class Edge;
}

#include <algorithm>
#include <cstdio>

#include "vertex.h"
#include "edge.h"

#include <utility>
#include "../base/def_struct.h"
#include "../base/except.h"
#include "../container/set.h"

namespace Koala {

template<class VertInfo=EmptyVertInfo, class EdgeInfo=EmptyVertInfo>
class Graph {
public:
	typedef Koala::Vertex<VertInfo,EdgeInfo> Vertex;
	typedef Vertex* PVertex;
	typedef Koala::Edge<VertInfo,EdgeInfo> Edge;
	typedef Edge* PEdge;
	typedef Graph<VertInfo,EdgeInfo>* PGraph;
	typedef VertInfo VertInfoType;
	typedef EdgeInfo EdgeInfoType;
	/** Create empty graph. */
	Graph();
	/** Create copy of graph.*/
	Graph(Graph &);
	~Graph();

	//----------------vertices--------------------
	PVertex addVert();
	PVertex addVert(const VertInfo &);
	void del(PVertex,bool=true);
	void delVert(PVertex,bool=true);
	//vertex motion
	PVertex getVert();
	PVertex getVertLast();
	PVertex getVertNext(PVertex);
	PVertex getVertPrev(PVertex);
	//sets
	Set<PVertex> getVertSet();
	template<class Chooser> Set<PVertex> getVertSet(Chooser);
	template<class OutputIterator>
			int getVerts(OutputIterator);
	template<class OutputIterator, class Chooser>
			int getVerts(OutputIterator, Chooser);
	//other
	int getVertNo(); ///< Vertex number (graph order)

	//-----------------edges----------------------
	PEdge addEdge(PVertex, PVertex, EdgeDirection=EdUndir);
	PEdge addEdge(PVertex, PVertex, const EdgeInfo &, EdgeDirection=EdUndir);
	PEdge addArch(PVertex, PVertex); //out->in
	PEdge addArch(PVertex, PVertex, const EdgeInfo&);
	PEdge addLoop(PVertex);
	PEdge addLoop(PVertex, const EdgeInfo&);

	void del(PEdge);
	void delEdge(PEdge);
	//edge motion
	PEdge getEdge(EdgeDirection=EdAll);
	PEdge getEdgeLast(EdgeDirection=EdAll);
	PEdge getEdgeNext(PEdge, EdgeDirection=EdAll);
	PEdge getEdgePrev(PEdge, EdgeDirection=EdAll);
	//sets
	Set<PEdge> getEdgeSet(EdgeDirection=EdAll);
	template<class Chooser> Set<PEdge> getEdgeSet(
			EdgeDirection, Chooser);
	template<class OutputIterator>
			int getEdges(OutputIterator, EdgeDirection=EdAll);
	template<class OutputIterator, class Chooser>
			int getEdges(OutputIterator, EdgeDirection,
			Chooser);
	//edge properity
	EdgeType getEdgeType(PEdge);
	EdgeType getType(PEdge);
	int getEdgeNo(EdgeDirection=EdAll);

	//----------vertices and edges connection--------------
	// IncidEdge ...
	//edge motion
	PEdge getEdge(PVertex, EdgeDirection=EdAll);
	PEdge getEdgeLast(PVertex, EdgeDirection=EdAll);
	PEdge getEdgeNext(PVertex, PEdge, EdgeDirection=EdAll);
	PEdge getEdgePrev(PVertex, PEdge, EdgeDirection=EdAll);
	//sets
	Set<PEdge> getEdgeSet(PVertex, EdgeDirection=EdAll);
	template<class Chooser> Set<PEdge> getEdgeSet(PVertex,
			EdgeDirection, Chooser);
	template<class OutputIterator>
			int getEdges(OutputIterator, PVertex,
			EdgeDirection=EdAll);
	template<class OutputIterator, class Chooser>
			int getEdges(OutputIterator, PVertex,
			EdgeDirection, Chooser);
	//other
	int getEdgeNo(PVertex, EdgeDirection=EdAll);
	//------------------edge between vertices-------------------------
	// ConnEdge ...
	PEdge getEdge(PVertex, PVertex,
			EdgeDirection=EdAll);
	PEdge getEdgeLast(PVertex, PVertex,
			EdgeDirection=EdAll);
	PEdge getEdgeNext(PVertex, PVertex,
			PEdge, EdgeDirection=EdAll);
	PEdge getEdgePrev(PVertex, PVertex,
			PEdge, EdgeDirection=EdAll);
	//sets
	Set<PEdge> getEdgeSet(PVertex, PVertex, EdgeDirection=EdAll);
	template<class Chooser> Set<PEdge> getEdgeSet(PVertex, PVertex,
			EdgeDirection, Chooser);
	template<class OutputIterator>
			int getEdges(OutputIterator, PVertex, PVertex,
			EdgeDirection=EdAll);
	template<class OutputIterator, class Chooser>
			int getEdges(OutputIterator, PVertex, PVertex,
			EdgeDirection, Chooser);
	//other
	int getEdgeNo(PVertex, PVertex,
			EdgeDirection=EdAll);
	//------------neighbourhood-----------------------------------
	Set<PVertex> getNeighbSet(PVertex, EdgeDirection=EdAll);
	template<class Chooser> Set<PVertex> getNeighbSet(PVertex,
			EdgeDirection, Chooser);
	template<class OutputIterator>
			int getNeighb(OutputIterator, PVertex,
			EdgeDirection=EdAll);
	template<class OutputIterator, class Chooser>
			int getNeighb(OutputIterator, PVertex,
			EdgeDirection, Chooser);
	int getNeighNo(PVertex, EdgeDirection=EdAll);

	Set<PVertex> getClNeighbSet(PVertex, EdgeDirection=EdAll);
	template<class Chooser> Set<PVertex> getClNeighbSet(PVertex,
			EdgeDirection, Chooser);
	template<class OutputIterator>
			int getClNeighb(OutputIterator, PVertex,
			EdgeDirection=EdAll);
	template<class OutputIterator, class Chooser>
			int getClNeighb(OutputIterator, PVertex,
			EdgeDirection, Chooser);
	int getClNeighNo(PVertex, EdgeDirection=EdAll);
	//-------------------vertices from edge---------------------------
	std::pair<PVertex,PVertex> getEdgeEnds(PEdge);
	bool isEdgeEnd(PEdge, PVertex);
	PVertex getEdgeEnd(PEdge, PVertex);
	//-------------------edge modifiers---------------------------
	/** Changes the type of the edge. For now possible changes are: directed<->undirected.*/
	bool chEdgeType(PEdge, EdgeType);
	bool revertEdge(PEdge);
	PEdge moveEdge(PEdge, PVertex, PVertex,
			EdgeDirection=EdUndir);
	//-------------------vertices and edges vs numbers-------------
	PVertex vertByNo(int);
	PEdge edgeByNo(int);
	int vertPos(PVertex);
	int edgePos(PEdge);

	//-----------------clear graph-------------------------
	/** Removes all vertices and edges from the graph. */
	void clear();
	/** Removes all edges from the graph. */
	void clearEdges();

	//-------------------copy/move methods-----------------------
	/** All vertices and edges are moved to the graph.
		@arg Graph from which vertices and edges are taken.
		@return &this???
	*/
	PVertex move(Graph<VertInfo,EdgeInfo> &);
//	template <class Chooser> bool move(Graph *, Chooser);
	/** All vertices and edges are copied to the graph.
		@arg Graph from which vertices and edges are copied.
		@return &this???
	*/
	/** Chosen vertices and edges are copied to the graph.
		@arg Graph from which vertices and edges are copied.
		@arg Function object which take vertex and edge pointer and return copy information.
	*/
	//TODO: Check if next copy methods can be joined.
	template<class VertInfoExt,class EdgeInfoExt>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser,class VCaster, class ECaster>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>, std::pair<VCaster,ECaster>);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser,class VCaster, class ECaster, class VLinker, class ELinker>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>, std::pair<VCaster,ECaster>, std::pair<VLinker,ELinker>);

    Graph<VertInfo,EdgeInfo>& operator=(Graph<VertInfo,EdgeInfo>&);

	//----------TEST METHODS------------
	bool testGraph(); //may throw an exception

private:
	//TODO: move operetor= to public
//	Graph& operator=(const Graph&);
	PVertex first_vert, last_vert;
	PEdge first_edge, last_edge;
	int no_vert, no_loop_edge, no_dir_edge, no_undir_edge; //number of vertices, number of edges
	//------------------attach/detach methods--------------------
	// ! are main methods for graph changes/adding/deleting/moving/
	PVertex attach(PVertex);
	PVertex detach(PVertex);
	PEdge attach(PEdge,PVertex,PVertex, EdgeDirection);
	PEdge attach_undir(PEdge,PVertex,PVertex); //out->in
	PEdge attach_dir(PEdge,PVertex,PVertex);
	PEdge attach_loop(PEdge,PVertex);
	PEdge detach(PEdge);

};
#include "graph.hpp"
// test methods - to check if all internal assumptions are correct
#include "graph_test.hpp"
// iterators
//#include "graph_iterator.hpp"
// getters
//#include "graph_get.hpp"
}
#endif
