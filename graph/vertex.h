#ifndef KOALA_VERTEX_H
#define KOALA_VERTEX_H


namespace Koala {



namespace Privates {

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct NormalVertLink {
            Edge<VertInfo,EdgeInfo,EdAllow> *first, *last;
            int degree;

            NormalVertLink(): first(NULL), last(NULL), degree(0) {}
            Edge<VertInfo,EdgeInfo,EdAllow>* &getFirst() { return first; }
            Edge<VertInfo,EdgeInfo,EdAllow>* &getLast() { return last; }
            int& getDegree() { return degree; }
};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct EmptyVertLink {

            EmptyVertLink() {}
            Edge<VertInfo,EdgeInfo,EdAllow>* &getFirst()
                { return *(Edge<VertInfo,EdgeInfo,EdAllow>**)(&_KoalaEmptyEdgePoiner); }
            Edge<VertInfo,EdgeInfo,EdAllow>* &getLast()
                { return *(Edge<VertInfo,EdgeInfo,EdAllow>**)(&_KoalaEmptyEdgePoiner); }
            int& getDegree() { return _KoalaEmptyVertDegree; }
};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow, EdgeType Present>
struct VertLinkEdDirIn;

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdDirIn<VertInfo,EdgeInfo,EdAllow,EdDirIn> : public NormalVertLink<VertInfo,EdgeInfo,EdAllow>
    {};

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdDirIn<VertInfo,EdgeInfo,EdAllow,0> : public EmptyVertLink<VertInfo,EdgeInfo,EdAllow>
    {};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow, EdgeType Present>
struct VertLinkEdDirOut;

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdDirOut<VertInfo,EdgeInfo,EdAllow,EdDirOut> : public NormalVertLink<VertInfo,EdgeInfo,EdAllow>
    {};

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdDirOut<VertInfo,EdgeInfo,EdAllow,0> : public EmptyVertLink<VertInfo,EdgeInfo,EdAllow>
    {};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow, EdgeType Present>
struct VertLinkEdUndir;

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdUndir<VertInfo,EdgeInfo,EdAllow,EdUndir> : public NormalVertLink<VertInfo,EdgeInfo,EdAllow>
    {};

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdUndir<VertInfo,EdgeInfo,EdAllow,0> : public EmptyVertLink<VertInfo,EdgeInfo,EdAllow>
    {};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow, EdgeType Present>
struct VertLinkEdLoop;

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdLoop<VertInfo,EdgeInfo,EdAllow,EdLoop> : public NormalVertLink<VertInfo,EdgeInfo,EdAllow>
    {};

template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
struct VertLinkEdLoop<VertInfo,EdgeInfo,EdAllow,0> : public EmptyVertLink<VertInfo,EdgeInfo,EdAllow>
    {};

}



template<class VertInfo=EmptyVertInfo, class EdgeInfo=EmptyEdgeInfo, EdgeType EdAllow=EdAll|AdjMatrixAllowed>
class Vertex :
    private Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,EdAllow,EdDirIn&EdAllow>,
    private Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,EdAllow,EdDirOut&EdAllow>,
    private Privates::VertLinkEdUndir<VertInfo,EdgeInfo,EdAllow,EdUndir&EdAllow>,
    private Privates::VertLinkEdLoop<VertInfo,EdgeInfo,EdAllow,EdLoop&EdAllow>,
    public GraphClassDefaultSettings::template VertAdditData<VertInfo,EdgeInfo,EdAllow>
{
	friend class Graph<VertInfo,EdgeInfo,EdAllow>;
	friend class Edge<VertInfo,EdgeInfo,EdAllow>;

public:

	VertInfo info; ///< Additional user information in the vertex.
    VertInfo getInfo() { return info; }
    void setInfo(const VertInfo& info) { this->info=info; }

private:


	/** Standard constructor*/
	Vertex();
	/** Constructor sets info variable */
	Vertex(const VertInfo &);

	~Vertex() {}

	Vertex *next, *prev;
};

#include "vertex.hpp"
}
#endif
