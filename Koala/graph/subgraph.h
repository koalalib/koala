#ifndef KOALA_SUBGRAPH_H
#define KOALA_SUBGRAPH_H

/* ------------------------------------------------------------------------- *
 * subgraph.h
 *
 * ??
 *
 * ------------------------------------------------------------------------- */

#include "graph.h"

namespace Koala
{

/* ------------------------------------------------------------------------- *
 * SubGraph< Graph,VChooser,EChooser >
 *
 * ??
 *
 * ------------------------------------------------------------------------- */

template< class Graph, class VChooser, class EChooser >
class Subgraph: public SubgraphBase
{
    public:

         mutable VChooser vchoose;
         mutable EChooser echoose;

        typedef typename Graph::Vertex Vertex;
        typedef typename Graph::PVertex PVertex;
        typedef typename Graph::Edge Edge;
        typedef typename Graph::PEdge PEdge;
        typedef Subgraph< Graph,VChooser,EChooser > GraphType;
        typedef typename Graph::RootGrType RootGrType;
        typedef Graph ParentGrType;
        typedef typename Graph::VertInfoType VertInfoType;
        typedef typename Graph::EdgeInfoType EdgeInfoType;

    // Konstruktory
        Subgraph() {}
        Subgraph(
            const Graph &,
            std::pair< VChooser,EChooser > = std::make_pair( VChooser(),EChooser() ) );
        Subgraph( std::pair< VChooser,EChooser > );

    // ??
        void plug(const Graph &g ) { SubgraphBase::link( &g ); }
        bool unplug() { return SubgraphBase::unlink(); }
        void setChoose( const std::pair< VChooser,EChooser > & );
        const RootGrType* getRootPtr() const;
        const ParentGrType* getParentPtr() const { return (const ParentGrType*)parent; }
        const ParentGrType &up() const;
        const RootGrType &root() const;
        bool good( PVertex, bool = false ) const;
        bool good( PEdge, bool = false ) const;

        PVertex getVert() const { return getVertNext( 0 ); }
        PVertex getVertLast() const { return getVertPrev( 0 ); }
        PVertex getVertNext( PVertex ) const;
        PVertex getVertPrev( PVertex ) const;

        template< class OutputIterator > int getVerts( OutputIterator ) const;
        Set< PVertex > getVertSet() const;
        int getVertNo() const { return getVerts( blackHole ); }
        VertInfoType getVertInfo( PVertex v ) const { return v->getInfo(); }
        EdgeInfoType getEdgeInfo( PEdge e ) const { return e->getInfo(); }
        EdgeType getEdgeType( PEdge e ) const { return e->getType(); }
        EdgeType getType( PEdge e ) const { return e->getType(); }
        std::pair< typename Graph::PVertex,typename Graph::PVertex >
            getEdgeEnds( PEdge edge ) const { return edge->getEnds(); }
        std::pair< typename Graph::PVertex,typename Graph::PVertex >
            getEnds( PEdge edge ) const { return edge->getEnds(); }
        bool isEdgeEnd( PEdge edge, PVertex vert ) const { return root().isEnd( edge,vert ); }
        bool isEnd( PEdge edge, PVertex vert ) const { return root().isEnd( edge,vert ); }
        typename Graph::PVertex getEdgeEnd( PEdge, PVertex ) const;
        typename Graph::PVertex getEnd( PEdge, PVertex ) const;
        EdgeDirection getEdgeDir( PEdge, PVertex ) const;

        PEdge getEdge( EdgeDirection mask = EdAll ) const { return getEdgeNext( 0,mask ); }
        PEdge getEdgeLast( EdgeDirection mask = EdAll ) const { return getEdgePrev( 0,mask ); }
        PEdge getEdgeNext( PEdge, EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PEdge, EdgeDirection = EdAll ) const;
        template <class OutputIterator> int getEdges(
            OutputIterator, EdgeDirection = EdAll ) const;
        int getEdgeNo( EdgeDirection = EdAll ) const;

        PEdge getEdge( PVertex, EdgeDirection = EdAll ) const;
        PEdge getEdgeLast( PVertex, EdgeDirection = EdAll ) const;
        PEdge getEdgeNext( PVertex, PEdge, EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PVertex, PEdge, EdgeDirection = EdAll ) const;
        Set< PEdge > getEdgeSet( PVertex, EdgeDirection = EdAll ) const;
        template< class OutputIterator > int getEdges(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        int getEdgeNo( PVertex, EdgeDirection = EdAll) const;

        PEdge getEdge( PVertex, PVertex, EdgeDirection = EdAll ) const;
        PEdge getEdgeLast( PVertex, PVertex, EdgeDirection = EdAll ) const;
        PEdge getEdgeNext( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        Set< PEdge > getEdgeSet( PVertex, PVertex, EdgeDirection = EdAll ) const;

        template< class OutputIterator > int getEdges(
            OutputIterator, PVertex, PVertex, EdgeDirection = EdAll ) const;
        int getEdgeNo( PVertex, PVertex, EdgeDirection = EdAll ) const;
        template <class Cont>
            void getAdj(Cont &) const;

        template< class OutputIterator > int getNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        Set< PVertex > getNeighSet( PVertex, EdgeDirection = EdAll ) const;
        int getNeighNo( PVertex, EdgeDirection = EdAll ) const;

        Set< typename Graph::PVertex > getClNeighSet(
            PVertex, EdgeDirection = EdAll ) const;
        template< class OutputIterator > int getClNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        int getClNeighNo( PVertex, EdgeDirection = EdAll ) const;

        PVertex vertByNo( int ) const;
        PEdge edgeByNo( int ) const;
        int vertPos( PVertex ) const;
        int edgePos( PEdge ) const;

        int deg( PVertex, EdgeDirection = EdAll ) const;
        std::pair< typename Graph::PVertex,int > minDeg( EdgeDirection = EdAll ) const;
        std::pair< typename Graph::PVertex,int > maxDeg( EdgeDirection = EdAll ) const;
        int Delta( EdgeDirection = EdAll ) const;
        int delta( EdgeDirection = EdAll ) const;
        bool areParallel( PEdge, PEdge, EdgeDirection = EdUndir ) const;
        template< class OutputIterator > int getParals( OutputIterator, typename Graph::PEdge, EdgeDirection = EdUndir ) const;
        int mu( typename Graph::PEdge, EdgeDirection = EdUndir ) const;
        int mu( EdgeDirection = EdUndir ) const;
        std::pair< typename Graph::PEdge,int > maxMu( EdgeDirection = EdUndir ) const;


        protected:

            struct Parals3 {
            typename Graph::PVertex v1,v2;
            EdgeDirection direct;
            typename Graph::PEdge edge;

            Parals3(typename Graph::PVertex av1,typename Graph::PVertex av2,
                    EdgeDirection adirect,
                    typename Graph::PEdge aedge) :
                    v1(av1), v2(av2), direct(adirect), edge(aedge) {}
            Parals3() {}
        };

        struct Parals3cmp {
            bool operator()(Parals3 a, Parals3 b)
            {
                return (a.v1 < b.v1) ||
                (a.v1 == b.v1 && a.v2 < b.v2) ||
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct < b.direct );
            }

        };

};

template< class Graph, class VChooser, class EChooser >
Subgraph< Graph,VChooser,EChooser > makeSubgraph(
     const Graph &, const std::pair< VChooser,EChooser > & );

#include "subgraph.hpp"

}

#endif
