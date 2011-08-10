#ifndef KOALA_EDGE_H
#define KOALA_EDGE_H

namespace Koala {


class EdgeConst {
protected:
	//do not change constans
	static const int V_U = 0;
	static const int V_out = 0;
	static const int V_loop = 0;
	static const int V_V = 1;
	static const int V_in = 1;
	static const int V_Nloop = 1;
};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
class NormalParalLink {
    protected:
            Edge<VertInfo,EdgeInfo,EdAllow> *n, *p;

    public:
            NormalParalLink(): n(NULL), p(NULL) {}
            Edge<VertInfo,EdgeInfo,EdAllow>* &nParal() { return n; }
            Edge<VertInfo,EdgeInfo,EdAllow>* &pParal() { return p; }
};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct EmptyParalLink {

            EmptyParalLink() {}
            Edge<VertInfo,EdgeInfo,EdAllow>* &nParal()
                { return *(Edge<VertInfo,EdgeInfo,EdAllow>**)(&_KoalaEmptyEdgePoiner); }
            Edge<VertInfo,EdgeInfo,EdAllow>* &pParal()
                { return *(Edge<VertInfo,EdgeInfo,EdAllow>**)(&_KoalaEmptyEdgePoiner); }
};

template<class VertInfo, class EdgeInfo, EdgeType EdAllow, EdgeType Present>
struct ParalLink;

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct ParalLink<VertInfo,EdgeInfo,EdAllow,AdjMatrixAllowed> : public NormalParalLink<VertInfo,EdgeInfo,EdAllow>
    {};

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct ParalLink<VertInfo,EdgeInfo,EdAllow,EdNone> : public EmptyParalLink<VertInfo,EdgeInfo,EdAllow>
    {};


template<class VertInfo=EmptyVertInfo, class EdgeInfo=EmptyEdgeInfo, EdgeType EdAllow=EdAll|AdjMatrixAllowed>
class Edge : public EdgeConst,
             public GraphClassDefaultSettings::template EdgeAdditData<VertInfo,EdgeInfo,EdAllow>,
             public ParalLink<VertInfo,EdgeInfo,EdAllow,EdAllow&AdjMatrixAllowed>
{
	friend class Graph<VertInfo, EdgeInfo,EdAllow>;
	friend class Vertex<VertInfo, EdgeInfo,EdAllow>;
	friend  class Graph<VertInfo, EdgeInfo,EdAllow>::AdjMatrix;

public:

	EdgeInfo info; ///< Additional user information in the edge.

    std::pair<Vertex<VertInfo,EdgeInfo,EdAllow>*, Vertex<VertInfo,EdgeInfo,EdAllow>*>
        getEnds() { return std::make_pair(vert[0].vert, vert[1].vert); }

    Vertex<VertInfo,EdgeInfo,EdAllow>* getEnd1() { return vert[0].vert; }
    Vertex<VertInfo,EdgeInfo,EdAllow>* getEnd2() { return vert[1].vert; }
    bool isEnd(Vertex<VertInfo,EdgeInfo,EdAllow>* v) { return vert[0].vert == v || vert[1].vert == v; }

    Vertex<VertInfo,EdgeInfo,EdAllow>* getEnd(Vertex<VertInfo,EdgeInfo,EdAllow>* v)
    {
        if (vert[0].vert == v) return vert[1].vert;
        if (vert[1].vert == v) return vert[0].vert;
        return NULL;
    }

    EdgeDirection getDir(Vertex<VertInfo,EdgeInfo,EdAllow>* v)
    {
        if (!v || !isEnd( v )) return EdNone;
        switch (type)
        {
            case Loop:
                return EdLoop;
            case Undirected:
                return EdUndir;
        }
        return (vert[0].vert == v) ? EdDirOut : EdDirIn;
    }

    EdgeType getType() { return type; }

    EdgeInfo getInfo() { return info; }

    void setInfo(const EdgeInfo& info) { this->info=info; }

private:
	struct EdgeLink {
		Vertex<VertInfo,EdgeInfo,EdAllow> *vert;
		Edge *next, *prev;
		EdgeLink(): vert(NULL), next(NULL), prev(NULL) {}
	};
	/** Standard constructor. */
	Edge();
	/** Constructor sets info variable. */
	Edge(const EdgeInfo &);

	Edge *next, *prev;
	EdgeLink vert[2]; //0==U==out; 1==V==in;
    EdgeType type;
};

#include "edge.hpp"
}
#endif
