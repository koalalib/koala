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


namespace Privates {

template<class VertInfo, class EdgeInfo, class Settings>
class NormalParalLink {
    protected:
            Edge<VertInfo,EdgeInfo,Settings> *n, *p;

    public:
            NormalParalLink(): n(NULL), p(NULL) {}
            Edge<VertInfo,EdgeInfo,Settings>* &nParal() { return n; }
            Edge<VertInfo,EdgeInfo,Settings>* &pParal() { return p; }
};


template<class VertInfo, class EdgeInfo, class Settings>
struct EmptyParalLink {

            EmptyParalLink() {}
            Edge<VertInfo,EdgeInfo,Settings>* &nParal()
                { return *(Edge<VertInfo,EdgeInfo,Settings>**)(&_KoalaEmptyEdgePoiner); }
            Edge<VertInfo,EdgeInfo,Settings>* &pParal()
                { return *(Edge<VertInfo,EdgeInfo,Settings>**)(&_KoalaEmptyEdgePoiner); }
};

template<class VertInfo, class EdgeInfo, class Settings, bool Present>
struct ParalLink;

template<class VertInfo, class EdgeInfo, class Settings>
struct ParalLink<VertInfo,EdgeInfo,Settings,true> : public NormalParalLink<VertInfo,EdgeInfo,Settings>
    {};

template<class VertInfo, class EdgeInfo, class Settings>
struct ParalLink<VertInfo,EdgeInfo,Settings,false> : public EmptyParalLink<VertInfo,EdgeInfo,Settings>
    {};

}


template<class VertInfo=EmptyVertInfo, class EdgeInfo=EmptyEdgeInfo, class Settings = DefaultGrSettings<EdAll,true> >
class Edge : public EdgeConst,
             public Settings::template EdgeAdditData<VertInfo,EdgeInfo,Settings>,
             public Privates::ParalLink<VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed>
{
	friend class Graph<VertInfo, EdgeInfo,Settings>;
	friend class Vertex<VertInfo, EdgeInfo,Settings>;
	friend class AdjMatrix<VertInfo, EdgeInfo,Settings,Settings::AdjMatrixAllowed>;

public:

	EdgeInfo info; ///< Additional user information in the edge.

    std::pair<Vertex<VertInfo,EdgeInfo,Settings>*, Vertex<VertInfo,EdgeInfo,Settings>*>
        getEnds() { return std::make_pair(vert[0].vert, vert[1].vert); }

    Vertex<VertInfo,EdgeInfo,Settings>* getEnd1() { return vert[0].vert; }
    Vertex<VertInfo,EdgeInfo,Settings>* getEnd2() { return vert[1].vert; }
    bool isEnd(Vertex<VertInfo,EdgeInfo,Settings>* v) { return vert[0].vert == v || vert[1].vert == v; }

    Vertex<VertInfo,EdgeInfo,Settings>* getEnd(Vertex<VertInfo,EdgeInfo,Settings>* v)
    {
        if (vert[0].vert == v) return vert[1].vert;
        if (vert[1].vert == v) return vert[0].vert;
        return NULL;
    }

    EdgeDirection getDir(Vertex<VertInfo,EdgeInfo,Settings>* v)
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
		Vertex<VertInfo,EdgeInfo,Settings> *vert;
		Edge *next, *prev;
		EdgeLink(): vert(NULL), next(NULL), prev(NULL) {}
	};
	/** Standard constructor. */
	Edge();
	/** Constructor sets info variable. */
	Edge(const EdgeInfo &);

	// klasa niekopiowalna
	Edge(const Edge& X) {}
	Edge& operator=(const Edge& X) {}

	~Edge() {}

	Edge *next, *prev;
	EdgeLink vert[2]; //0==U==out; 1==V==in;
    EdgeType type;
};

#include "edge.hpp"
}
#endif
