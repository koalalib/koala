#ifndef KOALA_EDGE_H
#define KOALA_EDGE_H

#include "../base/def_struct.h"

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

template<class VertInfo=EmptyVertInfo, class EdgeInfo=EmptyEdgeInfo>
class Edge : public EdgeConst {
	friend class Graph<VertInfo, EdgeInfo>;
	friend class Vertex<VertInfo, EdgeInfo>;
//	friend class Vertex;
public:
//	/** Reference to public info variable. */
//	EdgeInfo& operator*();
//	/** Pointer to public info variable. */
//	EdgeInfo* operator->();
//	KoalaPEdge& edge_ptr();
	EdgeInfo info; ///< Additional user information in the edge.
    AssocKeyContReg assocReg;
private:
	struct EdgeLink {
		Vertex<VertInfo,EdgeInfo> *vert;
		Edge *next, *prev;
		EdgeLink(): vert(NULL), next(NULL), prev(NULL) {}
	};
	/** Standard constructor. */
	Edge();
	/** Constructor sets info variable. */
	Edge(const EdgeInfo &);

	Edge *next, *prev, *nParal, *pParal;
	EdgeLink vert[2]; //0==U==out; 1==V==in;
	enum EdgeType type;
};

#include "edge.hpp"
}
#endif
