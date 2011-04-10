#ifndef DEF_SEARCH_H
#define DEF_SEARCH_H

/* ------------------------------------------------------------------------- *
 * search.h
 *
 * ------------------------------------------------------------------------- */

#include <vector>
#include <map>
#include <list>
#include <stack>
#include <cassert>
#include "../graph/graph.h"
#include "../base/def_struct.h"

namespace Koala {

/* ------------------------------------------------------------------------- *
 * SearchStructs
 *
 * ------------------------------------------------------------------------- */
    
class SearchStructs {
    protected:
        class DefaultStructs
        {
            public:
                template< class A, class B > class AssocCont
                {
                    public:
                        typedef AssocArray< A,B > Type;
                } ;

        } ;

    public:
        template< class GraphType > struct VisitVertLabs
        {
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge ePrev;
            
            int distance, component;

            VisitVertLabs(
                typename GraphType::PVertex vp = 0,
                typename GraphType::PEdge ep = 0,
                int dist = NumberTypeBounds< int >::plusInfty(),
                int comp = -1 );

            template< class Rec > void get( Rec &r );
        } ;

        template< class GraphType >
        class VisitedMap: public DefaultStructs::AssocCont<
            typename GraphType::PVertex, VisitVertLabs< GraphType > >::Type { } ;
        
        template< class CIter, class VIter > struct CompStore
        {
            CIter compIter;
            VIter vertIter;

            CompStore( CIter ac, VIter av ): vertIter( av ), compIter( ac ) { }
        } ;

        template< class CIter, class VIter >
        static CompStore< CIter,VIter > compStore( CIter, VIter );
} ;

/* ------------------------------------------------------------------------- *
 * Visitors
 *
 * ------------------------------------------------------------------------- */

class Visitors: public SearchStructs {
    public:
        class component_visitor_tag { } ;
        class no_component_visitor_tag { } ;
        class simple_visitor_tag { } ;
        class simple_preorder_visitor_tag: public simple_visitor_tag { } ;
        class simple_postorder_visitor_tag: public simple_visitor_tag { } ;
        class complex_visitor_tag { } ;

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_preorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_postorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, ... );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_preorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_postorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, ... );

        template< class GraphType, class Visitor >
        static bool visitEdgePre(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePre(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, simple_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePre(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, ... );

        template< class GraphType, class Visitor >
        static bool visitEdgePost(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePost(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, simple_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePost(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, ... );

        template< class GraphType, class Visitor >
        static bool beginComponent(
            const GraphType &, Visitor &, unsigned, component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool beginComponent(
            const GraphType &, Visitor &, unsigned, no_component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool beginComponent(
            const GraphType &g, Visitor &v, unsigned comp, ... ) { return true; }

        static bool beginComponent( ... ) { return true; }

        template< class GraphType, class Visitor >
        static bool endComponent(
            const GraphType &, Visitor &, unsigned, component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool endComponent(
            const GraphType &, Visitor &, unsigned, no_component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool endComponent(
            const GraphType &g, Visitor &v, unsigned, ... ) { return true; }
                                 
        static bool endComponent( ... ) { return true; }

        /*
         * EmptyVisitor
         */
        
        class EmptyVisitor: public simple_postorder_visitor_tag,
            public no_component_visitor_tag
        {
            public:
                EmptyVisitor() { }
                
                template< class GraphType >
                bool operator()(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs< GraphType > & r) { return true; }
         } ;

        /*
         * EndVertVisitor
         * terminate algorithm when reaching given vertex
         */

        class EndVertVisitor: public complex_visitor_tag,
            public no_component_visitor_tag
        {
            public:
                EndVertVisitor( void *arg ): ptr( arg ) { }

                template< class GraphType >
                bool visitVertexPre(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs< GraphType > &data ) { return true; }
                
                template< class GraphType >
                bool visitVertexPost(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs< GraphType > &v ) { return true; }
                    
                template< class GraphType >
                bool visitEdgePre(
                    const GraphType &g, typename GraphType::PEdge e,
                    typename GraphType::PVertex u ) { return (void*)u != ptr; }
                    
                template< class GraphType >
                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

            private:
                void *ptr;
        } ;

        /*
         * StoreTargetToVertIter
         */
        
        template< class VertIter >
        class StoreTargetToVertIter: public simple_postorder_visitor_tag,
            public no_component_visitor_tag
        {
            public:
                StoreTargetToVertIter( VertIter &i ): m_iter( i ) { }

                template< class GraphType >
                bool operator()(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );
                    
            private:
                VertIter &m_iter;
        } ;

        /*
         * StoreCompVisitor
         */
        
        template< class CompIter, class VertIter >
        class StoreCompVisitor: public simple_postorder_visitor_tag,
            public component_visitor_tag
        {
            public:
                typedef struct _State
                {
                    CompStore< CompIter,VertIter > iters;
                    unsigned p, id;
                    
                    _State( CompStore< CompIter,VertIter > i );
                } State;

                StoreCompVisitor( State &st ): m_st( st ) { }

            template< class GraphType >
            bool operator()(
                const GraphType &, typename GraphType::PVertex,
                VisitVertLabs< GraphType > & );

            template< class GraphType >
            bool beginComponent( const GraphType &, unsigned );

            template< class GraphType >
            bool endComponent( const GraphType &, unsigned );

            private:
                State &m_st;
        } ;

        /*
         * ComplexPreorderVisitor
         */
        
        template< class Visitor >
        class ComplexPreorderVisitor: public complex_visitor_tag
        {
            public:
                ComplexPreorderVisitor( Visitor &v ): visit( v ) { }
                
                template< class GraphType >
                bool visitVertexPre(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );
                
                template< class GraphType >
                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );
                    
                template< class GraphType >
                bool visitEdgePre(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );
                    
                template< class GraphType >
                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );
                    
            private:
                Visitor &visit;
        } ;

        /*
         * ComplexPostorderVisitor
         */
        
        template< class Visitor >
        class ComplexPostorderVisitor: public complex_visitor_tag
        {
            public:
                ComplexPostorderVisitor( Visitor &v ): visit( v ) { }
                
                template< class GraphType >
                bool visitVertexPre(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs<GraphType> &data ) { return true; }
                
                template< class GraphType >
                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );
                    
                template< class GraphType >
                bool visitEdgePre(
                    const GraphType &g, typename GraphType::PEdge e,
                    typename GraphType::PVertex u ) { return true; }
                    
                template< class GraphType >
                bool visitEdgePost(
                    const GraphType &g, typename GraphType::PEdge e,
                    typename GraphType::PVertex u ) { return true; }
                    
            private:
                Visitor &visit;
        } ;

};

template< class SearchImpl >
class GraphSearchBase: public ShortPathStructs, public SearchStructs
{
    public:
        template< class GraphType, class VertContainer, class Visitor >
        static int visitAllBase(
            const GraphType &, VertContainer &, Visitor, EdgeDirection );
        template< class GraphType, class VertContainer, class Iter >
        static int scanAttainable(
            const GraphType &, typename GraphType::PVertex, Iter,
            EdgeDirection, VertContainer & );

        template< class GraphType, class VertIter >
        static int scanAttainable(
            const GraphType &, typename GraphType::PVertex, VertIter,
            EdgeDirection = EdUndir | EdDirOut );
        
        template< class GraphType, class VertContainer, class VertIter >
        static int scan(
            const GraphType &, VertIter, EdgeDirection, VertContainer & );

        template< class GraphType, class VertIter >
        static int scan(
            const GraphType &, VertIter,
            EdgeDirection = EdDirOut | EdUndir | EdDirIn );

        template< class GraphType >
        static Set< typename GraphType::PVertex > getAttainableSet(
            const GraphType &, typename GraphType::PVertex,
            EdgeDirection = EdDirOut | EdUndir );

        template< class GraphType, class VertIter, class EdgeIter >
        static int getPath(
            const GraphType &, typename GraphType::PVertex,
            typename GraphType::PVertex, OutPath< VertIter,EdgeIter >,
            EdgeDirection = EdUndir | EdDirOut );

        template< class GraphType, class VertContainer, class CompIter, class VertIter >
        static int getComponents(
            const GraphType &, CompStore< CompIter,VertIter >, EdgeDirection,
            VertContainer & );

        template< class GraphType, class CompIter, class VertIter >
        static int getComponents(
            const GraphType &, CompStore< CompIter,VertIter >,
            EdgeDirection = EdUndir | EdDirOut | EdDirIn );
} ;

/*
 * DFSParamBlock
 */

template< class GraphType, class VertContainer, class Visitor >
struct DFSParamBlock: public SearchStructs
{
    const GraphType &g;
    VertContainer &visited;
    Visitor &visit;
    EdgeDirection mask;
    int component;

    DFSParamBlock(
        const GraphType &, VertContainer &, Visitor &, EdgeDirection, int );
} ;

/*
 * DFSBase
 */

template< class SearchImpl >
class DFSBase: public GraphSearchBase< SearchImpl >
{
    private:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsDoVisit(
            DFSParamBlock< GraphType,VertContainer,Visitor > &,
            typename GraphType::PVertex, unsigned );
            
    public:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );
} ;

/*
 * Depth-First-Search
 */

class DFS: public DFSBase< DFS >
{
    public:
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );
} ;

/*
 * Preorder Depth-First-Search
 */

class DFSPreorder: public DFSBase< DFSPreorder >
{
    private:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPreVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::complex_visitor_tag & );

        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPreVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::simple_visitor_tag & );

    public:
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );
} ;

/*
 * Postorder Depth-First-Search
 */

class DFSPostorder: public DFSBase< DFSPostorder >
{
    private:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPostVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::complex_visitor_tag & );

        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPostVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::simple_visitor_tag & );

    public:
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );
} ;

/*
 * Breadth-First-Search
 */

class BFS: public GraphSearchBase< BFS >
{
    private:
        template< class GraphType, class VertContainer, class Visitor >
        static int bfsDoVisit(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );

    public:
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );
} ;

/*
 * lexicographical Breadth-First-Search
 */

class LexBFS: public GraphSearchBase< LexBFS >
{
    public:
        template< class Graph > class LexVisitContainer
        {
            public:
                LexVisitContainer(): m_sets(), m_splits(), m_vertexToListPos() { }
                ~LexVisitContainer() { clear(); }

                void clear();
                bool empty() { return m_sets.empty(); }

                void initialize( const Graph &, typename Graph::PVertex );

                typename Graph::PVertex top() { return m_sets.front().first.front(); }

                void pop();
                void push( typename Graph::PVertex v );

                void step();
                void dump();

            private:
                typedef std::list< typename Graph::PVertex > LexList;
                typedef std::pair< LexList,typename LexList::iterator > Node;
                typedef std::list< Node > NodeList;
                typedef AssocArray< typename Graph::PVertex,
                    std::pair< typename NodeList::iterator,typename LexList::iterator > >
                    MapStruct;
                NodeList m_sets;
                typename NodeList::iterator m_lastSet;
                typename std::list< typename NodeList::iterator > m_splits;
                MapStruct m_vertexToListPos;
        } ;

        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );
} ;

/*
 * Cheriyan–Mehlhorn/Gabow algorithm
 */

class SCC: protected SearchStructs
{
    protected:
        struct SCCData {
            int preIdx;
            bool assigned;
            SCCData( int p = 0, bool a = false ): preIdx( p ), assigned( a ) { }
        } ;

        template< class GraphType, class CompIter, class VertIter, class CompMap >
        class SCCState
        {
            public:
                SCCState(
                    CompStore< CompIter,VertIter >, CompMap &,
                    typename GraphType::PVertex *, typename GraphType::PVertex*,
                    int );

                void addVert( typename GraphType::PVertex );
                void newComp();

                typename DefaultStructs::AssocCont< typename GraphType::PVertex,SCCData >::Type vmap;
                StackInterface< typename GraphType::PVertex* > s, p;
                CompStore< CompIter,VertIter > iters;
                CompMap &compMap;
                unsigned idx;
                int count, c;
        } ;

        template< class GraphType, class CompIter, class VertIter, class CompMap >
        class SCCVisitor: public Visitors::complex_visitor_tag,
            public Visitors::no_component_visitor_tag
        {
            public:
                SCCVisitor( SCCState< GraphType,CompIter,VertIter,CompMap > & );

                bool visitVertexPre(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );
                    
                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                bool visitEdgePre(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

            private:
                SCCState< GraphType,CompIter,VertIter,CompMap > &state;
        } ;

    public:
        template< class GraphType, class CompIter, class VertIter, class CompMap >
        static int get(
            const GraphType &, CompStore< CompIter,VertIter >, CompMap & );
} ;

class DAGAlgs: protected SearchStructs
{
    public:
        template< class GraphType, class VertIter >
        static void topOrd( const GraphType &, VertIter );

        template< class GraphType, class Iter >
        static bool isDAG( const GraphType &, Iter, Iter );
        
        template< class GraphType >
        static bool isDAG( const GraphType & );
} ;

/*
 * Blocks -- biconnected components
 */

class Blocks: protected SearchStructs
{
    protected:
        template< class GraphType >
        struct BiConVData {
            unsigned depth;
            unsigned lowpoint;
            unsigned count;
            bool iscut;
            int visited;
            int sons;
            int vblFirst;
        } ;

        struct VertBlockList {
            int block;
            int next;
        } ;

        template< class GraphType, class CompIter, class VertIter, class EdgeMap >
        class BiConState
        {
            public:
                typedef typename GraphType::PVertex PVertex;

                BiConState(
                    CompStore< CompIter,VertIter >, EdgeMap &, EdgeDirection,
                        std::pair< typename GraphType::PEdge *,int >,
                        VertBlockList * );

                void addVert( typename GraphType::PVertex );
                void addEdge( typename GraphType::PEdge );

                void newComp();

                typename DefaultStructs::AssocCont<
                    typename GraphType::PVertex,BiConVData< GraphType > >::Type vmap;
                typename DefaultStructs::AssocCont<
                    typename GraphType::PEdge,bool >::Type emap;
                StackInterface< typename GraphType::PEdge* > estk;
                CompStore< CompIter,VertIter > iters;
                EdgeMap &outEMap;
                VertBlockList *vbl;
                int vblAlloc;
                unsigned idx;
                int count;
                EdgeDirection mask;
        } ;

        template< class GraphType, class CompIter, class VertIter, class EdgeMap >
        class BiConVisitor: public Visitors::complex_visitor_tag,
            public Visitors::no_component_visitor_tag
        {
            public:
                BiConVisitor( BiConState< GraphType,CompIter,VertIter,EdgeMap > & );

                bool visitVertexPre(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );
                    
                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                bool visitEdgePre(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

            private:
                BiConState< GraphType,CompIter,VertIter,EdgeMap > &state;
        } ;

        template< class State, class VertMap, class VertBlockIter >
        static void storeBlocksData(
            State &, VertBlockList *, VertMap &, VertBlockIter & );

    public:
        struct VertData {
            int blockNo;
            int firstBlock;
            VertData( int b = 0, int f = -1 ): blockNo( b ), firstBlock( f ) { }
        } ;

        template< class GraphType, class VertDataMap, class EdgeDataMap,
            class CompIter, class VertIter, class VertBlockIter >
        static int split(
            const GraphType &, VertDataMap &, EdgeDataMap &,
            CompStore< CompIter,VertIter >, VertBlockIter, EdgeDirection = EdAll );

        template< class GraphType, class VertDataMap, class EdgeDataMap,
            class CompIter, class VertIter, class VertBlockIter >
        static int splitComp(
            const GraphType &, typename GraphType::PVertex, VertDataMap &,
            EdgeDataMap &, CompStore< CompIter,VertIter >, VertBlockIter,
            EdgeDirection = EdAll );
} ;

#include "search.hpp"

}

#endif
