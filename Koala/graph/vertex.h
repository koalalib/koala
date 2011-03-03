#ifndef KOALA_VERTEX_H
#define KOALA_VERTEX_H

#include "../base/def_struct.h"

namespace Koala {

class VertexConst {
protected:
	//do not change constans
	static const int loop = 0;
	static const int undir = 1;
	static const int dir_in = 2;
	static const int dir_out = 3;
};

template<class VertInfo=EmptyVertInfo, class EdgeInfo=EmptyEdgeInfo>
class Vertex : public VertexConst {
	friend class Graph<VertInfo,EdgeInfo>;
	friend class Edge<VertInfo,EdgeInfo>;
public:
	VertInfo info; ///< Additional user information in the vertex.
    AssocKeyContReg assocReg;
    VertInfo getInfo() { return info; }
    void setInfo(const VertInfo& info) { this->info=info; }
private:
    struct VertLink {
            Edge<VertInfo,EdgeInfo> *first, *last;
            int degree;
            VertLink(): first(NULL), last(NULL), degree(-1) {}
        };

	/** Standard constructor*/
	Vertex();
	/** Constructor sets info variable */
	Vertex(const VertInfo &);

	Vertex *next, *prev;
	VertLink edges[4]; //undir, dir_out, dir_in, loop
};

#include "vertex.hpp"
}
#endif
