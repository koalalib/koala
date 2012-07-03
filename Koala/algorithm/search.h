#ifndef DEF_SEARCH_H
#define DEF_SEARCH_H

/* ------------------------------------------------------------------------- *
 * search.h
 *
 * ------------------------------------------------------------------------- */

#include "../base/def_struct.h"
#include "../graph/subgraph.h"


namespace Koala {

/* ------------------------------------------------------------------------- *
 * SearchStructs
 *
 * ------------------------------------------------------------------------- */


class SearchStructs {

    public:
        template< class GraphType > struct VisitVertLabs
        {
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge ePrev;

            int distance, component;

            VisitVertLabs(
                typename GraphType::PVertex vp = 0,
                typename GraphType::PEdge ep = 0,
                int dist = std::numeric_limits<int>::max(),
                int comp = -1 );

//            template< class Rec > void get( Rec &r );
        } ;


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

/** visitor's code is called by searching algorithms (BFS, DFS, LexBFS)
 * visitor should inherit from one of:
 * - simple_visitor_tag
 * - simple_preorder_visitor_tag
 * - simple_postorder_visitor_tag
 * - complex_visitor_tag
 * and one of:
 * - component_visitor_tag
 * - no_component_visitor_tag
 *
 * simple visitors (inheriting from simple_*_tag) have operator() called
 * with the following arguments:
 * template< class GraphType >
 * bool operator()(const GraphType &g,
 * 		   typename GraphType::PVertex u,
 * 		   VisitVertLabs< GraphType > &data)
 * where g - graph containing vertex, u - visited vertex, data - visited vertex's data
 * returning true allows to continue search, retrurning false terminate
 * searching algorithm
 * simple_preorder_visitor_tag indicate to visit vertex before its neighbourrs
 * simple_postorder_visitor_tag indicate to visit vertex after its neighbourrs
 * simple_visitor_tag do not specify the order
 *
 * complex visitors are notified by a searching algorithm using following methods:
 * (g - graph containing vertex, u - visited vertex,
 *  data - visited vertex's data, e - visited edge, v - vertex from which we
 *  visit edge e)
 *
 * template< class GraphType >
 * bool visitVertexPre(const GraphType &g,
 * 		       typename GraphType::PVertex u,
 * 		       VisitVertLabs< GraphType > &data);
 * called before visiting u's neighbours
 * returning false prevents visiting u's neighbours
 *
 * template< class GraphType >
 * bool visitVertexPost(const GraphType &g,
 * 			typename GraphType::PVertex u,
 * 			VisitVertLabs< GraphType > &data);
 * called after visiting u's neighbours
 * returning false terminates searching
 *
 * template< class GraphType >
 * bool visitEdgePre(const GraphType &g,
 * 		     typename GraphType::PEdge e,
 * 		     typename GraphType::PVertex v);
 * called before visiting other end of e
 * returning false prevents visiting vertex on the other end of e
 *
 * template< class GraphType >
 * bool visitEdgePost(const GraphType &g,
 * 		      typename GraphType::PEdge e,
 * 		      typename GraphType::PVertex v);
 * called after visiting other end of e
 * returning false terminates searching
 *
 * visitors with component_visitor_tag are notified about components with
 * methods:
 * template< class GraphType >
 * bool beginComponent(const GraphType &g, unsigned compid);
 * template< class GraphType >
 * bool endComponent(const GraphType &g, unsigned compid);
 * where g is the visited graph and compid is the component number (starting from 0)
 * return values are ignored (TODO: u�y� albo zmieni� na void)
 *
 */
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

template< class SearchImpl, class DefaultStructs >
class GraphSearchBase: public ShortPathStructs, public SearchStructs
{
    public:

        template< class GraphType >
        class VisitedMap: public DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, VisitVertLabs< GraphType > >::Type
            {   public:
                VisitedMap(int asize) :
                    DefaultStructs:: template AssocCont<typename GraphType::PVertex, VisitVertLabs< GraphType > >
                        ::Type ( asize) {}
            } ;

	/** visit all vertices in a graph
	 * @param[in] g graph containing vertices to visit
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider
	 * @return number of components
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitAllBase(
            const GraphType &g, VertContainer &visited, Visitor visitor, EdgeDirection dir);

	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[out] out iterator to write vertices to
	 * @param[in] dir direction of edges to consider
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class Iter >
        static int scanAttainable(
            const GraphType &, typename GraphType::PVertex, Iter,
            EdgeDirection, VertContainer & );

	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[out] out iterator to write vertices to
	 * @param[in] dir direction of edges to consider
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertIter >
        static int scanAttainable(
            const GraphType &g, typename GraphType::PVertex src, VertIter out,
            EdgeDirection dir = EdUndir | EdDirOut );

	/** visit all vertices in a graph
	 * @param[in] g graph containing vertices to visit
	 * @param[out] out iterator to write vertices to
	 * @param[in] dir direction of edges to consider
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class VertIter >
        static int scan(
            const GraphType &g, VertIter out, EdgeDirection dir, VertContainer &visited);

	/** visit all vertices in a graph
	 * @param[in] g graph containing vertices to visit
	 * @param[out] out iterator to write vertices to
	 * @param[in] dir direction of edges to consider
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertIter >
        static int scan(
            const GraphType &g, VertIter out,
            EdgeDirection dir = EdDirOut | EdUndir | EdDirIn );

	/** TODO */
        template< class GraphType>
        static int cyclNo(
            const GraphType &, EdgeDirection = EdAll );

	/** return all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @return set of vertices in the component
	 */
        template< class GraphType >
        static Set< typename GraphType::PVertex > getAttainableSet(
            const GraphType &g, typename GraphType::PVertex src,
            EdgeDirection dir = EdDirOut | EdUndir );

	/** find a path between vertices
	 * @param[in] g graph to search path in
	 * @param[in] src starting vertex
	 * @param[in] dest target vertex
	 * @param[out] path found path
	 * @param[in] dir direction of edges to consider
	 * @return length of the path
	 */
        template< class GraphType, class VertIter, class EdgeIter >
        static int getPath(
            const GraphType &g, typename GraphType::PVertex src,
            typename GraphType::PVertex dest, OutPath< VertIter,EdgeIter >path,
            EdgeDirection dir = EdUndir | EdDirOut );

	/** split graph into components
	 * @param[in] g graph to split
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[in] dir direction of edges to consider
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @return number of components
	 */
        template< class GraphType, class VertContainer, class CompIter, class VertIter >
        static int getComponents(
            const GraphType &g, CompStore< CompIter,VertIter > out,
	    EdgeDirection dir, VertContainer &visited );

	/** split graph into components
	 * @param[in] g graph to split
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[in] dir direction of edges to consider
	 * @return number of components
	 */
        template< class GraphType, class CompIter, class VertIter >
        static int getComponents(
            const GraphType &g, CompStore< CompIter,VertIter > out,
            EdgeDirection dir = EdUndir | EdDirOut | EdDirIn );
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

template< class SearchImpl, class DefaultStructs >
class DFSBase: public GraphSearchBase< SearchImpl , DefaultStructs >
{
    private:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsDoVisit(
            DFSParamBlock< GraphType,VertContainer,Visitor > &,
            typename GraphType::PVertex, unsigned );

    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsVisitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

/*
 * Depth-First-Search
 */

template <class DefaultStructs >
class DFSPar: public DFSBase< DFSPar<DefaultStructs >, DefaultStructs >
{
    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;


class DFS : public DFSPar<AlgorithmsDefaultSettings> {};

/*
 * Preorder Depth-First-Search
 */

template <class DefaultStructs>
class DFSPreorderPar: public DFSBase< DFSPreorderPar<DefaultStructs>, DefaultStructs >
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
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

class DFSPreorder : public DFSPreorderPar<AlgorithmsDefaultSettings> {};

/*
 * Postorder Depth-First-Search
 */

template <class DefaultStructs>
class DFSPostorderPar: public DFSBase< DFSPostorderPar<DefaultStructs >, DefaultStructs >
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
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

class DFSPostorder: public DFSPostorderPar<AlgorithmsDefaultSettings> {};

/*
 * Breadth-First-Search
 */

template <class DefaultStructs>
class BFSPar: public GraphSearchBase< BFSPar<DefaultStructs >, DefaultStructs >
{
    private:
        template< class GraphType, class VertContainer, class Visitor >
        static int bfsDoVisit(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );

    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

class BFS: public BFSPar<AlgorithmsDefaultSettings> {};

/*
 * lexicographical Breadth-First-Search
 */


namespace Privates {

    class ListDefaultCPPAllocator {
    public:
        template<class T> T *allocate()			{ return new T(); };
        template<class T> void deallocate(T *p)		{ delete p; };
        template<class T> T *allocate(size_t n)		{ return new T[n]; };
        template<class T> void deallocate(T *p, size_t)	{ delete[] p; };
        } ;




    template <class T> struct ListNode;
    template <class T>	struct BaseListNode {
            ListNode<T> *next;
            ListNode<T> *prev;
            };
    template <class T>	struct ListNode: public BaseListNode<T> {
            T elem;
            };

    template <class T>
    class ListBlockListAllocator
    {

        private:
            typedef ListNode<T> Element;
            BlockOfBlockList< Element >* wsk;
            BlockList<Element,VectorInterface<BlockOfBlockList< Element > *> > *manager;
    //        mutable int licz;

            ListBlockListAllocator(const ListBlockListAllocator& ){}
            const ListBlockListAllocator& operator=(const ListBlockListAllocator&) {}

            template <class U,class W> class TestEqTypes;
            template <class U> class TestEqTypes<U,U> {};

            Element* alloc() { return &(manager->operator[](manager->newPos())); }

            void dealloc(Element* ptr)
            {   if (!ptr) return;
                int pos=(((char*)ptr)-((char*)wsk))/sizeof(BlockOfBlockList< Element >);
                assert(manager->ready(pos));
                manager->delPos(pos);
            }



        public:
            ListBlockListAllocator(int n)  // : licz(0)
            {   wsk=new BlockOfBlockList< Element >[n];
                manager=new BlockList<Element,VectorInterface<BlockOfBlockList< Element > *> >(wsk,n);
            }

            ~ListBlockListAllocator()
            {
                delete manager;
                delete [] wsk;
            }



            template<class U> U *allocate()
           { TestEqTypes<U,Element> x; return alloc(); };
           template<class U> void deallocate(U *p)
           { TestEqTypes<U,Element> x; dealloc(p); };
    };



    /*
     * List_iterator
     */
    template<class T>
    struct List_iterator {
    //public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;

        List_iterator(): ptr(NULL)				{};
        List_iterator(const List_iterator &i): ptr(i.ptr)	{};

        List_iterator next()					{ return List_iterator(ptr->next); };
        List_iterator prev()					{ return List_iterator(ptr->prev); };

        List_iterator &operator =(const List_iterator &i)	{ ptr = i.ptr; return *this; };

        bool operator ==(const List_iterator &i) const		{ return ptr == i.ptr; };
        bool operator !=(const List_iterator &i) const		{ return ptr != i.ptr; };

        T &operator *()			{ return ptr->elem; };
        const T &operator *() const	{ return ptr->elem; };
        T *operator ->()		{ return &(ptr->elem); };
        const T *operator ->() const	{ return &(ptr->elem); };

        List_iterator &operator++() {
            ptr = ptr->next;
            return *this;
            };

        List_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
            };

        List_iterator operator++(int) {
            List_iterator it;
            it = *this;
            ++(*this);
            return it;
            };

        List_iterator operator--(int) {
            List_iterator it;
            it = *this;
            --(*this);
            return it;
            };

    //	friend class List<T, Allocator>;
    //	friend class List_const_iterator<T, Allocator>;

    //private:
        List_iterator(ListNode<T> *p): ptr(p)	{};

    //private:
        ListNode<T> *ptr;
        };


    /*
     * List_const_iterator
     */
    template<class T>
    struct List_const_iterator {
    //public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;

        List_const_iterator(): ptr(NULL)				{};
        List_const_iterator(const List_const_iterator &i): ptr(i.ptr)	{};
        List_const_iterator(const List_iterator<T> &i): ptr(i.ptr)		{};

        List_const_iterator next()					{ return List_const_iterator(ptr->next); };
        List_const_iterator prev()					{ return List_const_iterator(ptr->prev); };

        List_const_iterator &operator =(const List_const_iterator &i)	{ ptr = i.ptr; return *this; };

        bool operator ==(const List_const_iterator &i) const		{ return ptr == i.ptr; };
        bool operator !=(const List_const_iterator &i) const		{ return ptr != i.ptr; };

        T &operator *()			{ return ptr->elem; };
        const T &operator *() const	{ return ptr->elem; };
        T *operator ->()		{ return &(ptr->elem); };
        const T *operator ->() const	{ return &(ptr->elem); };

        List_const_iterator &operator++() {
            ptr = ptr->next;
            return *this;
            };

        List_const_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
            };

        List_const_iterator operator++(int) {
            List_const_iterator it;
            it = *this;
            ++(*this);
            return it;
            };

        List_const_iterator operator--(int) {
            List_const_iterator it;
            it = *this;
            --(*this);
            return it;
            };

    //	friend class List<T, Allocator>;

    //private:
        List_const_iterator(ListNode<T> *p): ptr(p)	{};

    //private:
        ListNode<T> *ptr;
        };


    /*
     * List
     * lista podobna do stl-owej
     * size ma z³o¿onoœæ O(1) (std::list ma O(n))
     * interfejs jest ubo¿szy (nie z przyczyn technicznych -- czêœæ metod nie
     * by³a potrzebna)
     * dodatkowe metody:
     * insert_after(iterator, value)
     * insert_before(iterator, value)
     * move_after(iterator, iterator) -- przeniesienie wewn¹trz jednej listy
     * move_before(iterator, iterator) -- przeniesienie wewn¹trz jednej listy
     *
     */
    template<class T, class Allocator = ListDefaultCPPAllocator>
    class List {
    public:
        typedef T value_type;
        typedef List_iterator<T> iterator;
        typedef List_const_iterator<T> const_iterator;
    //	typedef ListNode<T> ListNode;

    	List(): allocator()	{ Zero(); }; // niebezpieczny, do tworzenia tablic list, nalezy natychmiast wykonac init
    	void init(Allocator &a) { allocator=&a; }
        List(Allocator &a): allocator(&a)		{ Zero(); };
        List(const List &lst): allocator(lst.allocator)	{ Zero(); *this = lst; };
        ~List()						{ clear(); };

        iterator begin()			{ return iterator(m_root.next); };
        const_iterator begin() const		{ return const_iterator(m_root.next); };
        iterator end()				{ return iterator((ListNode<T> *)(&m_root)); };
        const_iterator end() const		{ return const_iterator((ListNode<T> *)&m_root); };

        T &front()				{ return m_root.next->elem; };
        const T &front() const			{ return m_root.next->elem; };

        T &back()				{ return m_root.prev->elem; };
        const T &back() const			{ return m_root.prev->elem; };

        bool empty() const			{ return m_count == 0; };

        size_t size() const			{ return m_count; };

        iterator find(const T &v) 		{ return iterator(_find(v)); };
        const_iterator find(const T &v) const	{ return const_iterator(_find(v)); };

        void push_front(const T &v)		{ _insert_before(m_root.next, v); };
        void pop_front()			{ _erase(m_root.next); };
        void push_back(const T &v)		{ _insert_after(m_root.prev, v); };
        void pop_back()				{ _erase(m_root.prev); };

        void clear()				{ while(!empty()) pop_front(); };

        iterator insert(const_iterator pos, const T &v)		{ return iterator(_insert_before(pos.ptr, v)); };
        iterator insert_before(const_iterator pos, const T &v)	{ return iterator(_insert_before(pos.ptr, v)); };
        iterator insert_after(const_iterator pos, const T &v)	{ return iterator(_insert_after(pos.ptr, v)); };

        // przesuniêcie wewn¹trz kontenera, a NIE miêdzy kontenerami!
        void move_before(iterator pos, iterator elem) {
            _unlink(elem.ptr);
            _link_before(pos.ptr, elem.ptr);
            };

        // przesuniêcie wewn¹trz kontenera, a NIE miêdzy kontenerami!
        void move_after(iterator pos, iterator elem) {
            _unlink(elem.ptr);
            _link_after(pos.ptr, elem.ptr);
            };

        void erase(iterator pos)				{ _erase(pos.ptr); };

        template <class Alloc>
        void copy(const List<T, Alloc> &lst) {
            const_iterator it, e;
            if(this == (List*)(&lst)) return;
            clear();
            for(it = lst.begin(), e = lst.end(); it != e; ++it) push_back(*it);
            };

        List &operator = (const List<T, Allocator> &lst) {
            const_iterator it, e;
            if(this == &lst) return *this;
            if (!allocator) allocator=lst.allocator;
            clear();
            for(it = lst.begin(), e = lst.end(); it != e; ++it) push_back(*it);
            return *this;
            };

        void swap(List &o) {
            size_t t;
            ListNode<T> *p;
            p = m_root.next; m_root.next = o.m_root.next; o.m_root.next = p;
            p = m_root.prev; m_root.prev = o.m_root.prev; o.m_root.prev = p;
            t = m_count; m_count = o.m_count; o.m_count = t;
            };

        void sort() {
            ListNode<T> *a, *b, *pa;
            size_t i;
            if(m_count < 2) return;
            if(m_count == 2) {
                if(m_root.prev->elem < m_root.next->elem) {
                    a = m_root.next;
                    b = m_root.prev;
                    b->next = a;
                    b->prev = (ListNode<T> *)&m_root;
                    a->prev = b;
                    a->next = (ListNode<T> *)&m_root;
                    m_root.next = b;
                    m_root.prev = a;
                    };
                return;
                };
            a = b = m_root.next;
            i = m_count / 2;
            while(i) { b = b->next; i--; };
            a = b->prev;
            a->next = NULL;
            b->prev = NULL;
            List<T, Allocator> other(b, m_root.prev, m_count - m_count / 2,allocator);
            m_count = m_count / 2;
            m_root.prev = a;
            a->next = (ListNode<T> *)&m_root;
            sort();
            other.sort();
            merge(other);
            };

        // czyœci zawartoœæ o
        void merge(List &o) {
            ListNode<T> *a, *b, *e, *ae, *be;
            if(&o == this) return;
            if(o.empty()) return;
            if(empty()) { swap(o); return; };
            a = m_root.next;
            b = o.m_root.next;
            ae = (ListNode<T> *)&m_root;
            be = (ListNode<T> *)&(o.m_root);
            if(a->elem < b->elem) {
                m_root.next = e = a;
                a->prev = (ListNode<T> *)&m_root;
                a = a->next;
            } else {
                m_root.next = e = b;
                b->prev = (ListNode<T> *)&m_root;
                b = b->next;
                };
            if(a != ae && b != be) {
                while(true) {
                    if(a->elem < b->elem) {
                        e->next = a;
                        a->prev = e;
                        e = a;
                        if((a = a->next) == NULL) break;
                    } else {
                        e->next = b;
                        b->prev = e;
                        e = b;
                        if((b = b->next) == be) break;
                        };
                    };
                };
            if(a == ae) {
                e->next = b;
                b->prev = e;
                m_root.prev = o.m_root.prev;
                o.m_root.prev->next = (ListNode<T> *)&m_root;
            } else {
                e->next = a;
                a->prev = e;
                };
            m_count = m_count + o.m_count;
            o.Zero();
            };

        friend struct List_iterator<T>;
        friend struct List_const_iterator<T>;

    private:
        List(ListNode<T> *n, ListNode<T> *p, size_t c,Allocator *a): allocator(a) {
            m_root.next = n;
            m_root.prev = p;
            n->prev = (ListNode<T> *)&m_root;
            p->next = (ListNode<T> *)&m_root;
            m_count = c;
            };

        void Zero() {
            m_root.next = (ListNode<T> *)&m_root;
            m_root.prev = (ListNode<T> *)&m_root;
            m_count = 0;
            };

        ListNode<T> *NewElem() {
            return allocator->template allocate<ListNode<T> >();
            };

        ListNode<T> *_find(const T &v) {
            ListNode<T> *n = m_root.next;
            while(n != NULL) {
                if(n->elem == v) break;
                n = n->next;
                };
            return n;
            };

        void _link_before(ListNode<T> *ptr, ListNode<T> *p) {
            p->next = ptr;
            p->prev = ptr->prev;
            p->next->prev = p;
            p->prev->next = p;
            };

        void _link_after(ListNode<T> *ptr, ListNode<T> *p) {
            p->next = ptr->next;
            p->prev = ptr;
            p->next->prev = p;
            p->prev->next = p;
            };

        void _unlink(ListNode<T> *p) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            };

        ListNode<T> *_insert_before(ListNode<T> *ptr, const T &v) {
            ListNode<T> *p = NewElem();
            if(p == NULL) return NULL;
            _link_before(ptr, p);
            p->elem = v;
            m_count++;
            return p;
            };

        ListNode<T> *_insert_after(ListNode<T> *ptr, const T &v) {
            ListNode<T> *p = NewElem();
            if(p == NULL) return NULL;
            _link_after(ptr, p);
            p->elem = v;
            m_count++;
            return p;
            };

        void _erase(ListNode<T> *p) {
            if(empty()) return;
            if(p == (ListNode<T> *)&m_root) return;
            _unlink(p);
            allocator->deallocate(p);
            m_count--;
            };


    private:
        BaseListNode<T> m_root;
        size_t m_count;
        Allocator* allocator;
        };


    template<class T, class Allocator>
    std::ostream &operator <<(std::ostream &strm, const List<T, Allocator> &lst) {
        typename List<T, Allocator>::const_iterator b, e, it;
        strm << "[";
        b = lst.begin();
        e = lst.end();
        for(it = b; it != e; ++it) {
            if(it != b) strm << ", ";
            else strm << " ";
            strm << *it;
            };
        strm << " ]";
        return strm;
        };


}


template<class DefaultStructs>
class LexBFSPar: public GraphSearchBase<LexBFSPar<DefaultStructs>,
					 DefaultStructs> {
public:
    template <class Graph>
    struct LVCNode {
			typename Graph::PVertex v;
			Privates::List_iterator<LVCNode> block;

			LVCNode(typename Graph::PVertex _v = NULL): v(_v) { };
			LVCNode(typename Graph::PVertex _v,Privates::List_iterator<LVCNode> it): v(_v), block(it) { };
			};

	template<class Graph, class Allocator, class ContAllocator>
	class LexVisitContainer {
	public:

		typedef LVCNode<Graph> Node;

		class Container: public Privates::List<Node,ContAllocator>	{
		    public:
            Container(ContAllocator& a) : Privates::List<Node,ContAllocator>(a) {}
		};

		Container m_data;
		Privates::List_iterator<Node>  m_openBlock;
		Privates::List<Privates::List_iterator<Node>,Allocator> m_splits;
		typename DefaultStructs::template AssocCont<typename Graph::PVertex, Privates::List_iterator<Node> >::Type m_vertexToPos;

	public:
		LexVisitContainer(Allocator& a, ContAllocator& ca,int n): m_data(ca), m_splits(a), m_openBlock(), m_vertexToPos(n)	{ }
		~LexVisitContainer()						{ clear(); }

		void clear() {
			m_data.clear();
			m_splits.clear();
			};

		void initialize(const Graph &g) {
			clear();
			m_data.push_back(Node(NULL));
			m_data.back().block = m_data.end();
			m_openBlock = m_data.begin();
			};

		void initialize(const Graph &g, size_t n, typename Graph::PVertex *tab) {
			initialize(g);
			for(size_t i = 0; i < n; i++) push(tab[i]);
			};

		void initializeAddAll(const Graph &g) {
			typename Graph::PVertex v;
			initialize(g);
			for(v = g.getVert(); v != NULL; v = g.getVertNext(v))
				push(v);
			};

		void cleanup() {
			if(m_data.size() < 2) return;
			while(m_data.begin().next()->v == NULL) {
				if(m_data.begin() == m_openBlock) m_openBlock = m_data.end();
				m_data.pop_front();
				};
			};

		bool empty() {
			cleanup();
			return m_data.size() < 2;
			};

		typename Graph::PVertex top() {
			cleanup();
			return m_data.begin().next()->v;
			};

		void pop() {
			m_data.erase(m_data.begin().next());
			cleanup();
			};

		void push(typename Graph::PVertex v) {
			if(m_openBlock == m_data.end()) {
				m_data.push_back(Node(NULL));
				m_data.back().block = m_data.end().prev();
				};
			m_data.push_back(Node(v, m_openBlock));
			m_vertexToPos[v] = m_data.end().prev();
			};

		void move(typename Graph::PVertex v) {
			Privates::List_iterator<Node> grp, newGrp;
			Privates::List_iterator<Node> elem;
			if(!m_vertexToPos.hasKey(v)) push(v);
			elem = m_vertexToPos[v];
			grp = elem->block;
			newGrp = grp->block;
			if(newGrp == m_data.end()) {
				newGrp = m_data.insert_before(grp, Node(NULL));
				grp->block = newGrp;
				m_splits.push_back(grp);
				newGrp->block = m_data.end();
				};
			m_data.move_before(grp, elem);
			elem->block = newGrp;
			};

		void remove(typename Graph::PVertex v) {
			m_data.erase(m_vertexToPos[v]);
			};

		void done() {
			Privates::List_iterator<Privates::List_iterator<Node> > it, e;
			for(it = m_splits.begin(), e = m_splits.end(); it != e; ++it) {
				(*it)->block = m_data.end();
				};
			m_splits.clear();
			};

		void dump() {
			Privates::List_iterator<Node> it;
			for(it = m_data.begin(); it != m_data.end(); ++it) {
				if(it->v == NULL) printf(" |");
				else printf(" %p", it->v);
				};
			printf("\n");
			};
		};

    public:

	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs)
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices
	 */
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitBase(const GraphType & g,
			     typename GraphType::PVertex start,
			     VertContainer &visited,
			     Visitor visit,
			     EdgeDirection mask,
			     int component) {
		unsigned int depth, n, retVal;
		typename GraphType::PEdge e;
		typename GraphType::PVertex u, v;
//		ListDefaultCPPAllocator allocat;
//        ListDefaultCPPAllocator2 allocat2;
//		LexVisitContainer<GraphType, ListDefaultCPPAllocator,ListDefaultCPPAllocator2> cont(allocat,allocat2);

        assert(((mask&Directed)==0)||((mask&Directed)==Directed)); // TODO: LexBFS ma sens dla gr. nieskierowanych, chyba?
		Privates::ListBlockListAllocator<Privates::List_iterator<LVCNode<GraphType> > > allocat(g.getVertNo()+1); //TODO: size? - spr
        Privates::ListBlockListAllocator<LVCNode<GraphType> > allocat2(2*g.getVertNo()+1); //TODO: size? - spr 2n+1 -> n+1 - oj! raczej nie!
		LexVisitContainer<GraphType, Privates::ListBlockListAllocator<Privates::List_iterator<LVCNode<GraphType> > >,Privates::ListBlockListAllocator<LVCNode<GraphType> > >
            cont(allocat,allocat2,g.getVertNo());


		n = g.getVertNo();
		if(n == 0) return 0;
		if(start == NULL) start = g.getVert();

		cont.initialize(g);

		visited[start] = SearchStructs::VisitVertLabs<GraphType>(NULL, NULL, 0, component);
		cont.push(start);
		retVal = 0;

		while(!cont.empty()) {
//			printf("before: ");
//			cont.dump();
			u = cont.top();
			depth = visited[u].distance;
			visited[u].component = component;

			if(!Visitors::visitVertexPre(g, visit, u, visited[u], visit)) {
				retVal++;
				continue;
				};
			cont.pop();

			for(e = g.getEdge(u, mask); e != NULL; e = g.getEdgeNext(u, e, mask)) {
				v = g.getEdgeEnd(e, u);
				if(!Visitors::visitEdgePre(g, visit, e, u, visit)) continue;
//				if(visited.hasKey(v)) continue;
//				visited[v] = SearchStructs::VisitVertLabs<GraphType>(u, e, depth + 1, component);
				if(visited.hasKey(v)) {
					if(visited[v].component == -1) {
						cont.move(v);
						};
					continue;
					};
				visited[v] = SearchStructs::VisitVertLabs<GraphType>(u, e, depth + 1, -1);
				cont.move(v);
				if(!Visitors::visitEdgePost(g, visit, e, u, visit)) return -retVal;
				};
			cont.done();
//			printf("after:  ");
//			cont.dump();

			retVal++;
			if(!Visitors::visitVertexPost(g, visit, u, visited[u], visit))
				return -retVal;
			};
		return retVal;
		};



	template<class GraphType>
	struct OrderData {
		typename GraphType::PVertex v;
		int vertId;	// kogo jest s¹siadem (numer s¹siada w porz¹dku)
		int orderId;	// numer w porz¹dku
		};


	/** order vertices with LexBFS order, starting with a given sequence
	 * @param[in] g graph
	 * @param[in] in number of vertices in table tab
	 * @param[in] tab table containing initial order of vertices
	 * @param[in] mask direction of edges to consider
	 * @param[out] out iterator to write ordered vertices
	 * @return number of vertices written to out
	 */
	template<class GraphType,
		 class OutVertIter>
	static int order2(const GraphType & g,
			  size_t in,
			  typename GraphType::PVertex *tab,
			  EdgeDirection mask,
			  OutVertIter out) {

		int i, j, o, n, m, retVal;
		EdgeDirection bmask = mask;
		typename GraphType::PEdge e;
		typename GraphType::PVertex u, v;
		typename DefaultStructs::template AssocCont<typename GraphType::PVertex, std::pair<int, int> >::Type orderData(g.getVertNo());
		Privates::ListBlockListAllocator<Privates::List_iterator<LVCNode<GraphType> > > allocat(g.getVertNo()+1); //TODO: size? - spr
        Privates::ListBlockListAllocator<LVCNode<GraphType> > allocat2(2*g.getVertNo()+1); // j.w. 2n+1 -> n + 1 - oj! raczej nie!
		LexVisitContainer<GraphType, Privates::ListBlockListAllocator<Privates::List_iterator<LVCNode<GraphType> > >,Privates::ListBlockListAllocator<LVCNode<GraphType> > >
            cont(allocat,allocat2,g.getVertNo());

		bmask &= ~EdLoop;
//		if(bmask & EdDirOut) bmask &= ~EdDirIn; //TODO: watpliwe
        assert(((bmask&Directed)==0)||((bmask&Directed)==Directed)); // TODO: LexBFS ma sens dla gr. nieskierowanych, chyba?

		n = g.getVertNo();
		assert(in == n);
		m = g.getEdgeNo(bmask);
		int LOCALARRAY(first, n + 1);
		OrderData<GraphType> LOCALARRAY(neigh, m * 2);
		OrderData<GraphType> LOCALARRAY(neigh2, m * 2);

		for(o = 0; o < n; o++) orderData[tab[o]].second = o;

		i = j = 0;
		for(o = 0; o < n; o++) {
			u = tab[o];
			first[i] = j;
			orderData[u].first = 0;
			orderData[u].second = o;
			for(e = g.getEdge(u, bmask); e != NULL; e = g.getEdgeNext(u, e, bmask)) {
				v = g.getEdgeEnd(e, u);
				neigh[j].v = v;
				neigh[j].orderId = orderData[v].second;
				neigh[j].vertId = o;
				j++;
				};
			i++;
			};
		first[i] = j;

		LexBFSPar<DefaultStructs>::StableRadixSort(neigh, j, n, &OrderData<GraphType>::orderId, neigh2);
		LexBFSPar<DefaultStructs>::StableRadixSort(neigh2, j, n, &OrderData<GraphType>::vertId, neigh);

		retVal = 0;
		cont.initialize(g, in, tab);

//		cont.dump();

		while(!cont.empty()) {
			u = cont.top();
			cont.pop();
			orderData[u].first = 2;
			*out = u;
			++out;
			++retVal;

			j = orderData[u].second;
			for(i = first[j]; i < first[j + 1]; i++) {
				v = neigh[i].v;
//				if(orderData[v].first) continue;
//				orderData[v].first = true;
				if(orderData[v].first > 0) {
					if(orderData[v].first == 1) {
						cont.move(v);
						};
					continue;
					};
				orderData[v].first = 1;
				cont.move(v);
				};
			cont.done();
//		cont.dump();
			};
		return retVal;
		};

	template<class T>
	static void StableRadixSort(T *data,
				    int n,
				    int nb,
				    int T::*field,
				    T *out) {
		int LOCALARRAY(bucketFirst, nb);
		int LOCALARRAY(next, n);
		int i, bp;
		for(i = 0; i < nb; i++) bucketFirst[i] = -1;
		for(i = 0; i < n; i++) {
			bp = data[i].*field;
			if(bucketFirst[bp] < 0) {
				next[i] = i;
			} else {
				next[i] = next[bucketFirst[bp]];
				next[bucketFirst[bp]] = i;
				};
			bucketFirst[bp] = i;
			};
		for(bp = 0; bp < nb; bp++) {
			i = bucketFirst[bp];
			if(i < 0) continue;
			do {
				i = next[i];
				*out = data[i];
				++out;
			} while(i != bucketFirst[bp]);
			};
		};

	};


class LexBFS: public LexBFSPar<AlgorithmsDefaultSettings> {};


/*
 * Cheriyan/Mehlhorn/Gabow algorithm
 */

template <class DefaultStructs>
class SCCPar: protected SearchStructs
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

                typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,SCCData >::Type vmap;
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
	/** split graph into strongly connected components
	 * @param[in] g graph to split
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[out] vtoc map (PVertex -> int indexOfItsComponent(zero based))
	 * @return number of components
	 */
        template< class GraphType, class CompIter, class VertIter, class CompMap >
        static int get(
            const GraphType &g, CompStore< CompIter,VertIter > out, CompMap & vtoc);

        template< class GraphType, class CompMap, class PairIter >
        static int connections(const GraphType &, CompMap &, PairIter );
} ;

class SCC : public SCCPar<AlgorithmsDefaultSettings> {};


template <class DefaultStructs>
class DAGAlgsPar: protected SearchStructs
{
    public:
        template< class GraphType, class VertIter >
        static void topOrd( const GraphType &, VertIter );

        template< class GraphType, class Iter >
        static bool isDAG( const GraphType &, Iter, Iter );

        template< class GraphType >
        static bool isDAG( const GraphType & );

        template< class GraphType, class Iter >
        static int transEdges(const GraphType &, Iter);

        template< class GraphType>
        static void makeHasse(GraphType & );

} ;

class DAGAlgs : public DAGAlgsPar<AlgorithmsDefaultSettings> {};

/*
 * Blocks -- biconnected components
 */

template <class DefaultStructs>
class BlocksPar : protected SearchStructs
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
                        VertBlockList *,int );

                void addVert( typename GraphType::PVertex );
                void addEdge( typename GraphType::PEdge );

                void newComp();

                typename DefaultStructs:: template AssocCont<
                    typename GraphType::PVertex,BiConVData< GraphType > >::Type vmap;
                typename DefaultStructs:: template AssocCont<
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

	/** split graph into blocks
	 * @param[in] g graph to split
	 * @param[out] vmap ???
	 * @param[out] emap ???
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[out] viter ???
	 * @param[in] type type of edges to consider
	 * @return number of components
	 */
        template< class GraphType, class VertDataMap, class EdgeDataMap,
            class CompIter, class VertIter, class VertBlockIter >
        static int split(
            const GraphType &g, VertDataMap &, EdgeDataMap &,
            CompStore< CompIter,VertIter > out, VertBlockIter, EdgeType type = EdAll );

	/** split a component containing given vertex into blocks
	 * @param[in] g graph to split
	 * @param[in] src given vertex
	 * @param[out] vmap ???
	 * @param[out] emap ???
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[out] viter ???
	 * @param[in] type type of edges to consider
	 * @return number of components
	 */
        template< class GraphType, class VertDataMap, class EdgeDataMap,
            class CompIter, class VertIter, class VertBlockIter >
        static int splitComp(
            const GraphType &g, typename GraphType::PVertex src, VertDataMap &vmap,
            EdgeDataMap &emap, CompStore< CompIter,VertIter > out, VertBlockIter viter,
            EdgeType type = EdAll );


        template<class GraphType,class Iterator>
        static int core(const GraphType &g,Iterator out, EdgeType mask=EdAll)
        {   mask |= (mask & (EdDirIn | EdDirOut)) ? EdDirIn | EdDirOut : 0;
            typename DefaultStructs:: template AssocCont<
                typename GraphType::PVertex, int >::Type degs(g.getVertNo());
            std::pair<int, typename GraphType::PVertex> LOCALARRAY(buf,2*g.getVertNo());
            PriQueueInterface< std::pair<int, typename GraphType::PVertex>*,
                    std::greater<std::pair<int, typename GraphType::PVertex> > > q(buf,2*g.getVertNo());
            if (!g.getVertNo()) return 0;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                q.push(std::make_pair(degs[v]=g.deg(v,mask),v));
            while(!q.empty() && q.top().first<=1)
            {   if (!degs.hasKey(q.top().second)) { q.pop(); continue; }
                typename GraphType::PVertex v=q.top().second,u;
                typename GraphType::PEdge e=g.getEdge(v,mask);
                degs.delKey(v);q.pop();
                if (e && degs.hasKey(u=g.getEdgeEnd(e,v))) q.push(std::make_pair(--degs[u],u));
            }
            for(typename GraphType::PVertex v=degs.firstKey();v;v=degs.nextKey(v))
            {   *out=v; ++ out;   }
            return degs.size();
        }

} ;

class Blocks : public BlocksPar<AlgorithmsDefaultSettings> {};

/*
 * Eulerian cycle and path
 */

template <class DefaultStructs>
class EulerPar: public PathStructs, protected SearchStructs {
private:
	template<class GraphType>
	struct EulerState {
		const GraphType &g;
		StackInterface<std::pair<typename GraphType::PVertex, typename GraphType::PEdge> *> stk;
		typename DefaultStructs:: template AssocCont<typename GraphType::PEdge, bool>::Type edgeVisited;
		EdgeDirection mask;

		EulerState(const GraphType &_g,
			   std::pair<typename GraphType::PVertex, typename GraphType::PEdge> *_stk,
			   int nv,
			   EdgeDirection m):
			g(_g), stk(_stk, nv), edgeVisited(_g.getEdgeNo()), mask(m)
                {};
		};

	template<class GraphType>
	static void eulerEngine(typename GraphType::PVertex u,
				typename GraphType::PEdge ed,
				EulerState<GraphType> &state) {
		typename GraphType::PEdge e;
		typename GraphType::PVertex v;
		for(e = state.g.getEdge(u, state.mask);
		    e != NULL;
		    e = state.g.getEdgeNext(u, e, state.mask)) {
			if(state.edgeVisited.hasKey(e)) continue;
			state.edgeVisited[e] = true;
//			if(state.g.getEdgeType(e) == Loop) {
//				state.stk.push(std::make_pair(u, e));
//				continue;
//				};
			v = state.g.getEdgeEnd(e, u);
			eulerEngine(v, e, state);
			};
		state.stk.push(std::make_pair(u, ed));
		};


	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static void eulerResult(EulerState<GraphType> &state,
				OutPath<VertIter, EdgeIter> &out) {
		std::pair<typename GraphType::PVertex,
			  typename GraphType::PEdge> p;
		p = state.stk.top();
		state.stk.pop();
		*(out.vertIter) = p.first;
		++(out.vertIter);
		while(!state.stk.empty()) {
			p = state.stk.top();
			state.stk.pop();
			*(out.vertIter) = p.first;
			++(out.vertIter);
			*(out.edgeIter) = p.second;
			++(out.edgeIter);
			};
		};

public:

    template<class GraphType>
	static std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
    ends(const GraphType &g,EdgeType mask=Undirected)
    {
        EdgeDirection symmask = mask | ((mask&(EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0);
        bool dir= (mask&(EdDirIn|EdDirOut))==EdDirIn || (mask&(EdDirIn|EdDirOut))==EdDirOut;
        assert(!(dir && (mask&EdUndir)));
        std::pair<typename GraphType::PVertex,typename GraphType::PVertex> zero((typename GraphType::PVertex)0,(typename GraphType::PVertex)0),res((typename GraphType::PVertex)0,(typename GraphType::PVertex)0);
        typename GraphType::PVertex x;
        int licz=0;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if (g.getEdge(v,symmask)) { licz++; x=v; }
        if (licz==0) return zero;
        if (licz!=BFSPar<DefaultStructs>::scanAttainable(g,x,blackHole,symmask & ~EdLoop)) return zero;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if (!dir)
            {
                if (g.deg(v,symmask)&1)
                    if (res.first==0) res.first=v;
                    else if (res.second==0) res.second=v;
                        else return zero;
            }
            else
                switch (g.deg(v,EdDirOut)-g.deg(v,EdDirIn))
                {   case 1: if (res.first==0) res.first=v; else return zero; break;
                    case 0: break;
                    case -1:if (res.second==0) res.second=v; else return zero; break;
                    default: return zero;
                }

        if (res.first)
            if (dir && (mask&EdDirIn)) return std::make_pair(res.second,res.first);
            else return res;
        else return std::pair<typename GraphType::PVertex,typename GraphType::PVertex>(x,x) ;
    }

	/** test if graph has an Eulerian cycle
	 * @param[in] g graph
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasCycle(const GraphType &g,
			     EdgeType mask = Undirected) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,mask);
		return res.first!=0 && res.first==res.second;
	};

	/** test if graph has an Eulerian path
	 * @param[in] g graph
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasPath(const GraphType &g,
			    EdgeType mask = Undirected) {
        std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,mask);
		return res.first!=0 && res.first!=res.second;
    };

	/** test if graph has an Eulerian path starting at vertex u
	 * @param[in] g graph
	 * @param[in] u starting vertex
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasPath(const GraphType &g,
			    typename GraphType::PVertex u,
			    EdgeType mask = Undirected) {
        assert(u);
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,mask);
		bool dir= (mask&(EdDirIn|EdDirOut))==EdDirIn || (mask&(EdDirIn|EdDirOut))==EdDirOut;
		return (res.first==u ||(!dir && res.second==u));
		};

	/** test if graph has an Eulerian cycle containing vertex u
	 * @param[in] g graph
	 * @param[in] u given vertex
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasCycle(const GraphType &g,
			    typename GraphType::PVertex u,
			    EdgeType mask = Undirected) {
        assert(u);
		return hasCycle(g,mask) && g.deg(u,mask);
		};

	/** get Eulerian cycle
	 * @param[in] g graph
	 * @param[out] out found cycle
	 * @param[in] mask type of edges to consider
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getCycle(const GraphType &g,
			    OutPath<VertIter, EdgeIter> out,
			    EdgeType mask = Undirected) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,mask);
		if (res.first==0 || res.first!=res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, mask);
		eulerEngine<GraphType>(res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};

	/** get Eulerian cycle
	 * @param[in] g graph
	 * @param[in] prefstart preferred starting vertex
	 * @param[out] out found cucle
	 * @param[in] mask type of edges to consider
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getCycle(const GraphType &g,
			    typename GraphType::PVertex prefstart,
			    OutPath<VertIter, EdgeIter> out,
			    EdgeType mask = Undirected) {

		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,mask);
		if (res.first==0 || res.first!=res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, mask);
		eulerEngine<GraphType>(g.getEdge(prefstart,mask) ? prefstart : res.first, NULL, state);
		eulerResult(state, out);
		return true;
        };


	/** get Eulerian path
	 * @param[in] g graph
	 * @param[out] out found path
	 * @param[in] mask type of edges to consider
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getPath(const GraphType &g,
			   OutPath<VertIter, EdgeIter> out,
			   EdgeType mask = Undirected) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,mask);
		if (res.first==0 || res.first==res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, mask);
		eulerEngine<GraphType>(res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};


	/** get Eulerian path
	 * @param[in] g graph
	 * @param[in] prefstart preferred starting vertex
	 * @param[out] out found path
	 * @param[in] mask type of edges to consider
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getPath(const GraphType &g,
			    typename GraphType::PVertex prefstart,
			   OutPath<VertIter, EdgeIter> out,
			   EdgeType mask = Undirected) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,mask);
		if (res.first==0 || res.first==res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, mask);
		bool dir= (mask&(EdDirIn|EdDirOut))==EdDirIn || (mask&(EdDirIn|EdDirOut))==EdDirOut;
		eulerEngine<GraphType>((prefstart==res.second && !dir) ? res.second : res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};

};

class Euler : public EulerPar<AlgorithmsDefaultSettings> {};

#include "../algorithm/search.hpp"




}

#endif