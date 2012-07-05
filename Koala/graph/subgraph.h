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
        typedef typename Graph::GraphSettings GraphSettings;

    // Konstruktory
        Subgraph() {}
        Subgraph(
            const Graph &,
            std::pair< VChooser,EChooser > = std::make_pair( VChooser(),EChooser() ) );
        Subgraph( std::pair< VChooser,EChooser > );

    // ??
        EdgeType allowedEdgeTypes() const { return root().allowedEdgeTypes(); }
        void plug(const Graph &g ) { SubgraphBase::link( &g ); }
        bool unplug() { return SubgraphBase::unlink(); }
        void setChoose( const std::pair< VChooser,EChooser > & );
        const RootGrType* getRootPtr() const;
        const ParentGrType* getParentPtr() const { return (const ParentGrType*)parent; }
        const ParentGrType &up() const;
        const RootGrType &root() const;
        bool good( PVertex, bool = false ) const;
        bool good( PEdge, bool = false ) const;


        int getVertNo() const { return getVerts( blackHole ); }
        PVertex getVert() const { return getVertNext( 0 ); }
        PVertex getVertLast() const { return getVertPrev( 0 ); }
        PVertex getVertNext( PVertex ) const;
        PVertex getVertPrev( PVertex ) const;
        template< class OutputIterator > int getVerts( OutputIterator ) const;
        Set< PVertex > getVertSet() const;
        template< class OutputIterator,class VChooser2 > int getVerts(OutputIterator out, VChooser2 ch) const;
        template< class VChooser2> Set< PVertex > getVertSet(VChooser2 ch)    const;
        PVertex vertByNo( int ) const;
        int vertPos( PVertex ) const;



        int getEdgeNo( EdgeDirection = EdAll ) const;
        PEdge getEdge( EdgeDirection mask = EdAll ) const { return getEdgeNext( 0,mask ); }
        PEdge getEdgeLast( EdgeDirection mask = EdAll ) const { return getEdgePrev( 0,mask ); }
        PEdge getEdgeNext( PEdge, EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PEdge, EdgeDirection = EdAll ) const;
        template <class OutputIterator> int getEdges(
            OutputIterator, EdgeDirection = EdAll ) const;
        template< class OutputIterator,class EChooser2 > int getEdges(OutputIterator out, EChooser2 ch) const;
        template< class EChooser2> Set< PEdge > getEdgeSet(EChooser2 ch)    const;
        PEdge edgeByNo( int ) const;
        int edgePos( PEdge ) const;
        template< class OutputV,class OutputE,class VChooser2,class EChooser2 >
            std::pair<int,int> getChosen(std::pair<OutputV,OutputE>,std::pair<VChooser2,EChooser2>,bool =true) const;
        template<class VChooser2,class EChooser2 >
            std::pair<Set<PVertex>,Set<PEdge> >getChosenSets(std::pair<VChooser2,EChooser2>,bool =true) const;


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
            void getAdj(Cont &,EdgeType mask=EdAll) const;


        VertInfoType getVertInfo( PVertex v ) const { return root().getVertInfo(v); }
//        inline void setVertInfo( PVertex, const VertInfo & = VertInfo() ) const;
        EdgeInfoType getEdgeInfo( PEdge e ) const { return root().getEdgeInfo(e); }
//        inline void setEdgeInfo( PEdge, const EdgeInfo & = EdgeInfo() ) const;



        EdgeType getEdgeType( PEdge e ) const { return up().getEdgeType(e); }
        EdgeType getType( PEdge e ) const { return up().getType(e); }
        std::pair< typename Graph::PVertex,typename Graph::PVertex >
            getEdgeEnds( PEdge edge ) const { return up().getEdgeEnds(edge); }
        std::pair< typename Graph::PVertex,typename Graph::PVertex >
            getEnds( PEdge edge ) const { return up().getEnds(edge); }
        PVertex getEdgeEnd1( PEdge edge ) const { return up().getEdgeEnd1(edge); }
        PVertex getEdgeEnd2( PEdge edge ) const { return up().getEdgeEnd2(edge); }
        bool isEdgeEnd( PEdge edge, PVertex vert ) const { return root().isEnd( edge,vert ); }
        bool isEnd( PEdge edge, PVertex vert ) const { return root().isEnd( edge,vert ); }
        typename Graph::PVertex getEdgeEnd( PEdge, PVertex ) const;
        typename Graph::PVertex getEnd( PEdge, PVertex ) const;
        EdgeDirection getEdgeDir( PEdge, PVertex ) const;
//        inline bool incid(PEdge,PEdge) const;


        int deg( PVertex, EdgeDirection = EdAll ) const;
        std::pair< PVertex,int > minDeg( EdgeDirection = EdAll ) const;
        std::pair< PVertex,int > maxDeg( EdgeDirection = EdAll ) const;
        int Delta( EdgeDirection = EdAll ) const;
        int delta( EdgeDirection = EdAll ) const;



        template< class OutputIterator > int getNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        Set< PVertex > getNeighSet( PVertex, EdgeDirection = EdAll ) const;
        int getNeighNo( PVertex, EdgeDirection = EdAll ) const;
        Set< typename Graph::PVertex > getClNeighSet(
            PVertex, EdgeDirection = EdAll ) const;
        template< class OutputIterator > int getClNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        int getClNeighNo( PVertex, EdgeDirection = EdAll ) const;


        bool areParallel( PEdge, PEdge, EdgeDirection = EdUndir ) const;
        template< class OutputIterator > int getParals( OutputIterator, typename Graph::PEdge, EdgeDirection = EdUndir ) const;
        Set<typename Graph::PEdge> getParalSet(typename Graph::PEdge, EdgeDirection = EdUndir) const;
        int mu( typename Graph::PEdge, EdgeDirection = EdUndir ) const;
        int mu( EdgeDirection = EdUndir ) const;
        std::pair< typename Graph::PEdge,int > maxMu( EdgeDirection = EdUndir ) const;

//  TODO: brakuje:
//        template<class IterOut1, class IterOut2, class Iterator >
//            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,Iterator,Iterator, EdgeType = EdUndir ) const;
//        template<class IterOut1, class IterOut2, class Iterator >
//            std::pair<int,int> findParals2(std::pair<IterOut1,IterOut2>,Iterator,Iterator, EdgeType = EdUndir ) const;
//        template<class IterOut1, class IterOut2 >
//            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,const Set<PEdge>&, EdgeType = EdUndir ) const;
//        template<class IterOut1, class IterOut2 >
//            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,PVertex, EdgeType = EdUndir ) const;
//        template<class IterOut1, class IterOut2 >
//            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,PVertex,PVertex, EdgeType = EdUndir ) const;
//        template<class IterOut1, class IterOut2 >
//            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>, EdgeType = EdUndir ) const;


//        template <class Iterator,class OutIter> int getOutEdges(OutIter,Iterator, Iterator, EdgeDirection = EdDirOut ) const;
//        template <class Iterator> Set< PEdge > getOutEdgeSet(Iterator beg, Iterator end, EdgeDirection = EdDirOut ) const;
//        template <class OutIter> int getOutEdges(OutIter,const Set<PVertex>&, EdgeDirection = EdDirOut ) const;
//        Set< PEdge > getOutEdgeSet(const Set<PVertex>&, EdgeDirection = EdDirOut ) const;
//
//        template <class Iterator,class OutIter> int getOutEnds(OutIter,Iterator, Iterator, EdgeDirection = EdDirOut ) const;
//        template <class Iterator> Set< PVertex > getOutEndSet(Iterator beg, Iterator end, EdgeDirection = EdDirOut ) const;
//        template <class OutIter> int getOutEnds(OutIter,const Set<PVertex>&, EdgeDirection = EdDirOut ) const;
//        Set< PVertex > getOutEndSet(const Set<PVertex>&, EdgeDirection = EdDirOut ) const;


        template <class Iterator,class OutIter> int getIndEdges(OutIter,Iterator, Iterator, EdgeType = EdAll ) const;
        template <class Iterator> Set< typename Graph::PEdge > getIndEdgeSet(Iterator beg, Iterator end, EdgeType = EdAll ) const;
        template <class OutIter> int getIndEdges(OutIter,const Set<typename Graph::PVertex>&, EdgeType = EdAll ) const;
        Set< typename Graph::PEdge > getIndEdgeSet(const Set<typename Graph::PVertex>&, EdgeType = EdAll ) const;



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
