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

#include "../base/def_struct.h"
#include "vertex.h"
#include "edge.h"

#include <utility>

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
	void delVert(PVertex,bool=true); // alias poprzedniej

	VertInfoType getVertInfo(PVertex);
	void setVertInfo(PVertex,const VertInfo &);

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

	EdgeInfoType getEdgeInfo(PEdge);
	void setEdgeInfo(PEdge,const EdgeInfo &);

	void del(PEdge);
	void delEdge(PEdge); // alias poprzedniej
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
	EdgeType getType(PEdge); // alias poprzedniej
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
	Set<PVertex> getNeighSet(PVertex, EdgeDirection=EdAll);
	template<class Chooser> Set<PVertex> getNeighSet(PVertex,
			EdgeDirection, Chooser);
	template<class OutputIterator>
			int getNeigh(OutputIterator, PVertex,
			EdgeDirection=EdAll);
	template<class OutputIterator, class Chooser>
			int getNeigh(OutputIterator, PVertex,
			EdgeDirection, Chooser);
	int getNeighNo(PVertex, EdgeDirection=EdAll);

	Set<PVertex> getClNeighSet(PVertex, EdgeDirection=EdAll);
	template<class Chooser> Set<PVertex> getClNeighSet(PVertex,
			EdgeDirection, Chooser);
	template<class OutputIterator>
			int getClNeigh(OutputIterator, PVertex,
			EdgeDirection=EdAll);
	template<class OutputIterator, class Chooser>
			int getClNeigh(OutputIterator, PVertex,
			EdgeDirection, Chooser);
	int getClNeighNo(PVertex, EdgeDirection=EdAll);
	//-------------------vertices from edge---------------------------
	std::pair<PVertex,PVertex> getEdgeEnds(PEdge);
	std::pair<PVertex,PVertex> getEnds(PEdge); // alias poprzedniej
	bool isEdgeEnd(PEdge, PVertex);
	bool isEnd(PEdge, PVertex); // alias poprzedniej
	PVertex getEdgeEnd(PEdge, PVertex);
	PVertex getEnd(PEdge, PVertex); // alias poprzedniej
	EdgeDirection getEdgeDir(PEdge,PVertex);
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
	//-------------------graph and element's parameters------------
	int deg(PVertex, EdgeDirection=EdAll);
	std::pair<PVertex,int> minDeg(EdgeDirection=EdAll);
	std::pair<PVertex,int> maxDeg(EdgeDirection=EdAll);
	int Delta(EdgeDirection=EdAll);
	int delta(EdgeDirection=EdAll);
	//-------------------group modifications----------------------------
	int delVerts();
	template <class Iterator>
        int delVerts(Iterator begin,Iterator end);
    template <class Iterator>   // j.w. ale zaklada, ze elementy nie powtarzaja sie
        int delVerts2(Iterator begin,Iterator end);
    int delVerts(const Set<PVertex>&);
    int delEdges(EdgeDirection=EdAll);
    int delEdges(PVertex,EdgeDirection=EdAll);
    int delEdges(PVertex,PVertex,EdgeDirection=EdAll);
    template <class Iterator>
        int delEdges(Iterator begin,Iterator end,EdgeDirection=EdAll);
    template <class Iterator>   // j.w. ale zaklada, ze elementy nie powtarzaja sie
        int delEdges2(Iterator begin,Iterator end,EdgeDirection=EdAll);
    int delEdges(const Set<PEdge>&,EdgeDirection=EdAll);
    template <class Iterator>
        int delEdges(PVertex,Iterator begin,Iterator end,EdgeDirection=EdAll);
    template <class Iterator>   // j.w. ale zaklada, ze elementy nie powtarzaja sie
        int delEdges2(PVertex,Iterator begin,Iterator end,EdgeDirection=EdAll);
    int delEdges(PVertex,const Set<PEdge>&,EdgeDirection=EdAll);
    template <class Iterator>
        int delEdges(PVertex,PVertex,Iterator begin,Iterator end,EdgeDirection=EdAll);
    template <class Iterator>   // j.w. ale zaklada, ze elementy nie powtarzaja sie
        int delEdges2(PVertex,PVertex,Iterator begin,Iterator end,EdgeDirection=EdAll);
    int delEdges(PVertex,PVertex,const Set<PEdge>&,EdgeDirection=EdAll);
    PEdge ch2Archs(PEdge);
    int ch2Archs();
    template <class Iterator>
        int ch2Archs(Iterator begin,Iterator end);
    int ch2Archs(const Set<PEdge>&);
    bool areParallel(PEdge,PEdge,EdgeDirection=EdUndir);
    PVertex putVert(PEdge,const VertInfo &);
    PVertex putVert(PEdge);
    PEdge pickVert(PVertex,const EdgeInfo &);
    PEdge pickVert(PVertex);
    PVertex glue(PVertex,PVertex, bool makeloops=false);
    template <class Iterator>
        PVertex glue(Iterator,Iterator,bool makeloops=false,PVertex=0);
    template <class Iterator> // j.w. ale zaklada, ze elementy nie powtarzaja sie
        PVertex glue2(Iterator,Iterator,bool makeloops=false,PVertex=0);
    PVertex glue(const Set<PVertex>&,bool makeloops=false,PVertex=0);

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
	/** All vertices and edges are copied to the graph.
		@arg Graph from which vertices and edges are copied.
		@return &this???
	*/
	/** Chosen vertices and edges are copied to the graph.
		@arg Graph from which vertices and edges are copied.
		@arg Function object which take vertex and edge pointer and return copy information.
	*/
	template<class VertInfoExt,class EdgeInfoExt>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser,class VCaster, class ECaster>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>, std::pair<VCaster,ECaster>);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser,class VCaster, class ECaster, class VLinker, class ELinker>
		PVertex copy(Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>, std::pair<VCaster,ECaster>, std::pair<VLinker,ELinker>);

	template<class VertInfoExt,class EdgeInfoExt>
		PVertex substitute(PVertex,Graph<VertInfoExt,EdgeInfoExt> &);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser>
		PVertex substitute(PVertex,Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser,class VCaster, class ECaster>
		PVertex substitute(PVertex,Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>, std::pair<VCaster,ECaster>);
	template<class VertInfoExt,class EdgeInfoExt,class VChooser, class EChooser,class VCaster, class ECaster, class VLinker, class ELinker>
		PVertex substitute(PVertex,Graph<VertInfoExt,EdgeInfoExt> &, std::pair<VChooser,EChooser>, std::pair<VCaster,ECaster>, std::pair<VLinker,ELinker>);

    Graph<VertInfo,EdgeInfo>& operator=(Graph<VertInfo,EdgeInfo>&);
    Graph<VertInfo,EdgeInfo>& operator+=(Graph<VertInfo,EdgeInfo>&);

    //----------Adjacency matrix--------

    bool makeAdjMatrix();
    bool delAdjMatrix();
    bool hasAdjMatrix();
    bool defragAdjMatrix();

	//----------TEST METHODS------------
	bool testGraph(); //may throw an exception

private:
	PVertex first_vert, last_vert;
	PEdge first_edge, last_edge;
	int no_vert, no_loop_edge, no_dir_edge, no_undir_edge; //number of vertices, number of edges

    struct Parals {
            typename Koala::Edge<VertInfo,EdgeInfo> *first, *last;
            int degree;
            Parals(): first(NULL), last(NULL), degree(0) {}
        };

	struct AdjMatrix {
        AssocMatrix<typename Koala::Vertex<VertInfo,EdgeInfo>*, Parals, AMatrNoDiag> dirs;
        AssocMatrix<typename Koala::Vertex<VertInfo,EdgeInfo>*, Parals, AMatrTriangle> undirs;
        AdjMatrix(int asize=0) : dirs(asize), undirs(asize) {}

        void clear() { dirs.clear(); undirs.clear(); }
        void defrag() { dirs.defrag(); undirs.defrag(); }
        void add(PEdge edge)
        {   if (!edge) return;
            if (edge->type==Directed)
            {   std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex>
                                ends(edge->vert[0].vert, edge->vert[1].vert);
                edge->pParal = dirs(ends.first,ends.second).last;
                edge->nParal = NULL;
                if(edge->pParal)
                    edge->pParal->nParal = edge;
                else
                    dirs(ends.first,ends.second).first = edge;
                dirs(ends.first,ends.second).last = edge;
                dirs(ends.first,ends.second).degree++;
            }
            else if (edge->type==Undirected)
            {   std::pair<typename Graph<VertInfo,EdgeInfo>::PVertex,typename Graph<VertInfo,EdgeInfo>::PVertex>
                                ends(edge->vert[0].vert, edge->vert[1].vert);
                edge->pParal = undirs(ends.first,ends.second).last;
                edge->nParal = NULL;
                if(edge->pParal)
                    edge->pParal->nParal = edge;
                else
                    undirs(ends.first,ends.second).first = edge;
                undirs(ends.first,ends.second).last = edge;
                undirs(ends.first,ends.second).degree++;
            }
        }
	};

	AdjMatrix* pAdj;

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
