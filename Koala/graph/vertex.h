#ifndef KOALA_VERTEX_H
#define KOALA_VERTEX_H


namespace Koala {



namespace Privates {

template<class VertInfo, class EdgeInfo, class Settings>
struct NormalVertLink {
            Edge<VertInfo,EdgeInfo,Settings> *first, *last;
            int degree;

            NormalVertLink(): first(NULL), last(NULL), degree(0) {}
            Edge<VertInfo,EdgeInfo,Settings>* &getFirst() { return first; }
            Edge<VertInfo,EdgeInfo,Settings>* &getLast() { return last; }
            int& getDegree() { return degree; }
};


template<class VertInfo, class EdgeInfo, class Settings>
struct EmptyVertLink {

            EmptyVertLink() {}

            DummyVar< Edge<VertInfo,EdgeInfo,Settings>* > getFirst()
                { return DummyVar< Edge<VertInfo,EdgeInfo,Settings>* >(); }
            DummyVar< Edge<VertInfo,EdgeInfo,Settings>* > getLast()
                { return DummyVar< Edge<VertInfo,EdgeInfo,Settings>* >(); }
            DummyVar< int > getDegree() { return DummyVar< int >(); }

};


template<class VertInfo, class EdgeInfo, class Settings, EdgeType Present>
struct VertLinkEdDirIn;

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn> : public NormalVertLink<VertInfo,EdgeInfo,Settings>
    {};

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,0> : public EmptyVertLink<VertInfo,EdgeInfo,Settings>
    {};


template<class VertInfo, class EdgeInfo, class Settings, EdgeType Present>
struct VertLinkEdDirOut;

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut> : public NormalVertLink<VertInfo,EdgeInfo,Settings>
    {};

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,0> : public EmptyVertLink<VertInfo,EdgeInfo,Settings>
    {};


template<class VertInfo, class EdgeInfo, class Settings, EdgeType Present>
struct VertLinkEdUndir;

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir> : public NormalVertLink<VertInfo,EdgeInfo,Settings>
    {};

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdUndir<VertInfo,EdgeInfo,Settings,0> : public EmptyVertLink<VertInfo,EdgeInfo,Settings>
    {};


template<class VertInfo, class EdgeInfo, class Settings, EdgeType Present>
struct VertLinkEdLoop;

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop> : public NormalVertLink<VertInfo,EdgeInfo,Settings>
    {};

template<class VertInfo, class EdgeInfo, class Settings>
struct VertLinkEdLoop<VertInfo,EdgeInfo,Settings,0> : public EmptyVertLink<VertInfo,EdgeInfo,Settings>
    {};

}


// Klasa wierzcholka glownej struktury grafu
template<class VertInfo=EmptyVertInfo, class EdgeInfo=EmptyEdgeInfo, class Settings = DefaultGrSettings<EdAll,true> >
class Vertex :
    private Privates::VertLinkEdDirIn<VertInfo,EdgeInfo,Settings,EdDirIn&Settings::EdAllow>,
    private Privates::VertLinkEdDirOut<VertInfo,EdgeInfo,Settings,EdDirOut&Settings::EdAllow>,
    private Privates::VertLinkEdUndir<VertInfo,EdgeInfo,Settings,EdUndir&Settings::EdAllow>,
    private Privates::VertLinkEdLoop<VertInfo,EdgeInfo,Settings,EdLoop&Settings::EdAllow>,
    public  Settings::template VertAdditData<VertInfo,EdgeInfo,Settings>
{
	friend class Graph<VertInfo,EdgeInfo,Settings>;
	friend class Edge<VertInfo,EdgeInfo,Settings>;

public:

	VertInfo info; ///< Additional user information in the vertex.
    VertInfo getInfo() { return info; }
    void setInfo(const VertInfo& info) { this->info=info; }

private:
    	// klasa jest niekopiowalna, obiekty mozna tworzyc i usuwac jedynie z metod klas zaprzyjaznionych

	/** Standard constructor*/
	Vertex();
	/** Constructor sets info variable */
	Vertex(const VertInfo &);


	Vertex(const Vertex& X) {}
	Vertex& operator=(const Vertex& X) {}

	~Vertex() {}

	Vertex *next, *prev;
};

#include "vertex.hpp"
}
#endif
