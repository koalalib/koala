#ifndef DEF_SEARCH_H
#define DEF_SEARCH_H

#include <vector>
#include <map>
#include <list>
#include <stack>
#include <cassert>
#include "../graph/graph.h"
#include "../base/def_struct.h"


namespace Koala {



class SearchStructs {
    public:

    template<class GraphType>
    struct VisitVertLabs {
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge ePrev;
        int distance,component;

        VisitVertLabs(      typename GraphType::PVertex vp=0,
                            typename GraphType::PEdge ep=0,
                            int dist=NumberTypeBounds<int>::plusInfty(),
                            int comp=-1) : vPrev(vp), ePrev(ep),
            distance(dist), component(comp)
        {};

        template <class Rec>
        void get(Rec& r) { r.vPrev=vPrev; r.ePrev=ePrev; r.distance=distance; r.component=component; }
    };


    template <class CIter, class VIter> struct CompStore {
            CIter compIter;
            VIter vertIter;

            CompStore(CIter ac, VIter av) : vertIter(av), compIter(ac) {}
        };
        // funkcja tworzaca, analogia make_pair
    template <class CIter, class VIter>
    static CompStore< CIter, VIter> compStore(CIter ac, VIter av) { return CompStore< CIter, VIter>(ac,av); }


    protected:

    class DefaultStructs {
        public:

        template <class A, class B> class AssocCont {
            public:
            typedef AssocArray<A,B> Type;
        };

    };


public:
	template<class GraphType>
	class VisitedMap: public DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type {};



};


/*
 * Visitors
 */
class Visitors: public SearchStructs {
public:
	class component_visitor_tag					{};
	class no_component_visitor_tag					{};
	class simple_visitor_tag					{};
	class simple_preorder_visitor_tag: public simple_visitor_tag	{};
	class simple_postorder_visitor_tag: public simple_visitor_tag	{};
	class complex_visitor_tag					{};

	// preorder visiting
	template<class GraphType, class Visitor>
	static bool visitVertexPre(const GraphType &g,
				   Visitor &v,
				   typename GraphType::PVertex u,
				   VisitVertLabs<GraphType> &r,
				   simple_preorder_visitor_tag &)	{ return v(g, u, r); };

	template<class GraphType, class Visitor>
	static bool visitVertexPre(const GraphType &,
				   Visitor &,
				   typename GraphType::PVertex,
				   VisitVertLabs<GraphType> &,
				   simple_postorder_visitor_tag &)	{ return true; };

	template<class GraphType, class Visitor>
	static bool visitVertexPre(const GraphType &g,
				   Visitor &v,
				   typename GraphType::PVertex u,
				   VisitVertLabs<GraphType> &r,
				   complex_visitor_tag &)		{ return v.visitVertexPre(g, u, r); };

	template<class GraphType, class Visitor>
	static bool visitVertexPre(const GraphType &g,
				   Visitor &v,
				   typename GraphType::PVertex u,
				   VisitVertLabs<GraphType> &r,
				   ...)					{ return v.operator()(g, u, r); };

	// postorder visiting
	template<class GraphType, class Visitor>
	static bool visitVertexPost(const GraphType &g,
				    Visitor &v,
				    typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &r,
				    simple_preorder_visitor_tag &)	{ return true; };

	template<class GraphType, class Visitor>
	static bool visitVertexPost(const GraphType &g,
				    Visitor &v,
				    typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &r,
				    simple_postorder_visitor_tag &)	{ return v(g, u, r); };

	template<class GraphType, class Visitor>
	static bool visitVertexPost(const GraphType &g,
				    Visitor &v,
				    typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &r,
				    complex_visitor_tag &)		{ return v.visitVertexPost(g, u, r); };

	template<class GraphType, class Visitor>
	static bool visitVertexPost(const GraphType &g,
				    Visitor &v,
				    typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &r,
				    ...)				{ return v.operator()(g, u, r); };

	// preorder edge visitor
	template<class GraphType, class Visitor>
	static bool visitEdgePre(const GraphType &g,
				 Visitor &v,
				 typename GraphType::PEdge e,
				 typename GraphType::PVertex u,
				 complex_visitor_tag &)	{ return v.visitEdgePre(g, e, u); };

	template<class GraphType, class Visitor>
	static bool visitEdgePre(const GraphType &,
				 Visitor &,
				 typename GraphType::PEdge,
				 typename GraphType::PVertex,
				 simple_visitor_tag &)	{ return true; };

	template<class GraphType, class Visitor>
	static bool visitEdgePre(const GraphType &,
				 Visitor &,
				 typename GraphType::PEdge,
				 typename GraphType::PVertex,
				 ...)			{ return true; };

	// postorder edge visitor
	template<class GraphType, class Visitor>
	static bool visitEdgePost(const GraphType &g,
				  Visitor &v,
				  typename GraphType::PEdge e,
				  typename GraphType::PVertex u,
				  complex_visitor_tag &) { return v.visitEdgePost(g, e, u); };

	template<class GraphType, class Visitor>
	static bool visitEdgePost(const GraphType &,
				  Visitor &,
				  typename GraphType::PEdge ,
				  typename GraphType::PVertex ,
				  simple_visitor_tag &)	{ return true; };

	template<class GraphType, class Visitor>
	static bool visitEdgePost(const GraphType &,
				  Visitor &,
				  typename GraphType::PEdge ,
				  typename GraphType::PVertex ,
				  ...)			{ return true; };

	// component visitors
	template<class GraphType, class Visitor>
	static bool beginComponent(const GraphType &g,
				   Visitor &v,
				   unsigned int comp,
				   component_visitor_tag &) { return v.beginComponent(g, comp); };

	template<class GraphType, class Visitor>
	static bool beginComponent(const GraphType &g,
				   Visitor &v,
				   unsigned int comp,
				   no_component_visitor_tag &) { return true; };

	template<class GraphType, class Visitor>
	static bool beginComponent(const GraphType &g,
				   Visitor &v,
				   unsigned int comp,
				   ...) { return true; };

	static bool beginComponent(...) { return true; };

	template<class GraphType, class Visitor>
	static bool endComponent(const GraphType &g,
				 Visitor &v,
				 unsigned int comp,
				 component_visitor_tag &) { return v.endComponent(g, comp); };

	template<class GraphType, class Visitor>
	static bool endComponent(const GraphType &g,
				 Visitor &v,
				 unsigned int comp,
				 no_component_visitor_tag &) { return true; };

	template<class GraphType, class Visitor>
	static bool endComponent(const GraphType &g,
				 Visitor &v,
				 unsigned int comp,
				 ...) { return true; };

	static bool endComponent(...) { return true; };
	/*
	 * EmptyVisitor
	 */
	class EmptyVisitor: public simple_postorder_visitor_tag,
			    public no_component_visitor_tag {
	public:
		EmptyVisitor()	{};
		template<class GraphType>
		bool operator()(const GraphType &,
				typename GraphType::PVertex,
				VisitVertLabs<GraphType> &) {
			return true;
			};
		};

	/*
	 * EndVertVisitor
	 * terminate algorithm when reaching given vertex
	 */
	class EndVertVisitor: public complex_visitor_tag,
			      public no_component_visitor_tag {
	public:
		EndVertVisitor(void* arg): ptr(arg) {};

		template<class GraphType>
		bool visitVertexPre(const GraphType &g, typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &data)	{ return true; };
		template<class GraphType>
		bool visitVertexPost(const GraphType &, typename GraphType::PVertex ,
				     VisitVertLabs<GraphType> &)	{ return true; };
		template<class GraphType>
		bool visitEdgePre(const GraphType &, typename GraphType::PEdge ,
				  typename GraphType::PVertex u)		{ return (void*)u!=ptr; };
		template<class GraphType>
		bool visitEdgePost(const GraphType &g, typename GraphType::PEdge e,
				   typename GraphType::PVertex u)	{ return !e || (void*)g.getEdgeEnd(e,u)!=ptr; };


	private:
		void *ptr;
		};

	/*
	 * StoreTargetToVertIter
	 * store vertices to output iterator
	 */
	template<class VertIter>
	class StoreTargetToVertIter: public simple_postorder_visitor_tag,
				     public no_component_visitor_tag {
	public:
		StoreTargetToVertIter(VertIter& i): m_iter(i)	{};

		template <class GraphType>
		bool operator()(const GraphType & ,
				typename GraphType::PVertex u,
				VisitVertLabs<GraphType>&) {
			*m_iter = u;
			++m_iter;
			return true;
			};
	private:
		VertIter &m_iter;
		};

	/*
	 * StoreCompVisitor
	 */
	template<class CompIter, class VertIter>
	class StoreCompVisitor: public simple_postorder_visitor_tag,
				public component_visitor_tag {
	public:
		typedef struct _State {
			CompStore<CompIter, VertIter> iters;
			unsigned int p;
			unsigned int id;
			_State(CompStore<CompIter, VertIter> i): iters(i), p(0), id(0) {
				*(iters.compIter) = 0;
				++(iters.compIter);
				};
			} State;

		StoreCompVisitor(State &st): m_st(st) {
			};

		template<class GraphType>
		bool operator()(const GraphType & ,
				typename GraphType::PVertex u,
				VisitVertLabs<GraphType> &) {
			*(m_st.iters.vertIter) = u;
			++(m_st.iters.vertIter);
			++(m_st.p);
			return true;
			};

		template<class GraphType>
		bool beginComponent(const GraphType &, unsigned int ) {
			return true;
			};

		template<class GraphType>
		bool endComponent(const GraphType &, unsigned int ) {
			*(m_st.iters.compIter) = m_st.p;
			++(m_st.iters.compIter);
			return true;
			};

	private:
		State &m_st;
		};


	/*
	 * ComplexPreorderVisitor
	 */
	template<class Visitor>
	class ComplexPreorderVisitor: public complex_visitor_tag {
	public:
		ComplexPreorderVisitor(Visitor &v): visit(v)		{};
		template<class GraphType>
		bool visitVertexPre(const GraphType &g, typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &data)	{ return visit.operator()(g, u, data); };
		template<class GraphType>
		bool visitVertexPost(const GraphType &, typename GraphType::PVertex ,
				     VisitVertLabs<GraphType> &)	{ return true; };
		template<class GraphType>
		bool visitEdgePre(const GraphType &, typename GraphType::PEdge ,
				  typename GraphType::PVertex )		{ return true; };
		template<class GraphType>
		bool visitEdgePost(const GraphType &, typename GraphType::PEdge ,
				   typename GraphType::PVertex )	{ return true; };
	private:
		Visitor &visit;
		};

	/*
	 * ComplexPostorderVisitor
	 */
	template<class Visitor>
	class ComplexPostorderVisitor: public complex_visitor_tag {
	public:
		ComplexPostorderVisitor(Visitor &v): visit(v)		{};
		template<class GraphType>
		bool visitVertexPre(const GraphType &, typename GraphType::PVertex ,
				    VisitVertLabs<GraphType> &)		{ return true; };
		template<class GraphType>
		bool visitVertexPost(const GraphType &g, typename GraphType::PVertex u,
				     VisitVertLabs<GraphType> &data)	{ return visit.operator()(g, u, data); };
		template<class GraphType>
		bool visitEdgePre(const GraphType &, typename GraphType::PEdge ,
				  typename GraphType::PVertex )		{ return true; };
		template<class GraphType>
		bool visitEdgePost(const GraphType &, typename GraphType::PEdge ,
				   typename GraphType::PVertex )	{ return true; };
	private:
		Visitor &visit;
		};

};


template<class SearchImpl>
class GraphSearchBase : public ShortPathStructs, public SearchStructs {


    public:

	/** visit all vertices in a graph
	 * @param[in] g graph containing vertices to visit
	 * @param[in] visited container to store data
	 * @param[in] visit visitor called for each vertex (parameters are: vertex, bool (true for on open, false on close), number of the component)
	 * @param[in] mask edges to consider (bitwise or of Ed* constants)
	 * @return number of components */
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitAllBase(const GraphType &g,
				VertContainer &visited,
				Visitor visit,
				EdgeDirection mask) {
		int rv;
		unsigned int component;
		typename GraphType::PVertex first;

		component = 0;
		first = g.getVert();
		while(first != NULL) {
			Visitors::beginComponent(g, visit, component, visit);
			rv = SearchImpl::visitBase(g, first, visited, visit, mask, component);
			Visitors::endComponent(g, visit, component, visit);
			component++;
			if(rv < 0) break;
			do {
				first = g.getVertNext(first);
			} while(first != NULL && visited.hasKey(first));
			};
		return component;
    };


//	/** visit all vertices in the same component as 'root'
//	 * @param[in] g graph containing vertices to visit
//	 * @param[in] root vertex to start from
//	 * @param[in] visit visitor called for each vertex (parameters are: vertex, bool (true for on open, false on close), number of the component)
//	 * @param[in] mask edges to consider (bitwise or of Ed* constants, default = EdUndir | EdDirOut)
//	 * @return number of visited vertices */
//	template<class GraphType,
//		 class Visitor>
//	static int visit(GraphType &g,
//			 typename GraphType::PVertex root,
//			 Visitor visit,
//			 EdgeDirection mask = EdUndir | EdDirOut) {
//		VisitedMap<GraphType> visited;
//		return SearchImpl::visitBase(g, root, visited, visit, mask, 0);
//    };
//
//
//	/** visit all vertices in a graph
//	 * @param[in] g graph containing vertices to visit
//	 * @param[in] visit visitor called for each vertex (parameters are: vertex, bool (true for on open, false on close), number of the component)
//	 * @param[in] mask edges to consider (bitwise or of Ed* constants, default = EdUndir | EdDirOut)
//	 * @return number of components */
//	template<class GraphType,
//		 class Visitor>
//	static int visitAll(GraphType &g,
//			    Visitor visit,
//			    EdgeDirection mask=EdUndir | EdDirOut)
//    {
//		VisitedMap<GraphType> visited;
//        mask|=(mask & (EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0;
//		return visitAllBase(g, visited, visit, mask);
//	};


	template<class GraphType,
		 class VertContainer, class Iter>
	static int scanAttainable(const GraphType &g,
				  typename GraphType::PVertex root,
				  Iter comp,
				  EdgeDirection mask,
				  VertContainer &cont)
        {
	    int rv;
            assert(root);
            mask&=~EdLoop;
            rv = SearchImpl::visitBase(g, root, cont, Visitors::StoreTargetToVertIter<Iter>(comp), mask, 0);
	    if(rv < 0) return -rv;  //TODO: to zawsze falsz
	    return rv;
		};

	template<class GraphType,
		 class VertIter>
	static int scanAttainable(const GraphType &g,
				  typename GraphType::PVertex root,
				  VertIter comp,
				  EdgeDirection mask = EdUndir | EdDirOut) {
		VisitedMap<GraphType> cont;
		return scanAttainable(g, root, comp, mask, cont);
	};


	/** find all vertices in the graph;
	 * root will be included in the output
	 * @param[in] g graph
	 * @param[in] root vertex identifying a component
	 * @param[in] comp output iterator to store result
	 * @param[in] mask edges to consider (bitwise or of Ed* constants, default = EdUndir | EdDirOut | EdLoop | EdDirIn)
	 * @param[in] cont container to store intermediate data (default = use own)
	 * @return number visited components */
	template<class GraphType,
		 class VertContainer,
		 class VertIter>
	static int scan(const GraphType &g,
			VertIter iter,
			EdgeDirection mask,
			VertContainer &tree)
        {
            mask|=(mask & (EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0;
            mask&=~EdLoop;
            return visitAllBase(g, tree,  Visitors::StoreTargetToVertIter<VertIter>(iter),mask);
		};

	template<class GraphType,
		 class VertIter>
	static int scan(const GraphType &g,
			VertIter iter,
			EdgeDirection mask=EdDirOut|EdUndir|EdDirIn) {
		VisitedMap<GraphType> cont;
		return scan(g, iter, mask, cont);
    };


    template<class GraphType>
	static  Set<typename GraphType::PVertex>
            getAttainableSet(const GraphType &g,
				    typename GraphType::PVertex root,
				    EdgeDirection mask=EdDirOut|EdUndir)
    {   assert(root);
        Set<typename GraphType::PVertex> res;
        scanAttainable(g,root,setInserter(res),mask);
        return res;
    }

	/** distance (number of edges) between two vertices
	 * @param[in] g graph containing start and end vertices
	 * @param[in] start vertex to start from
	 * @param[in] end vertex to reach
	 * @param[out] path OutPath object to write path to
	 * @param[in] mask edges to consider (bitwise or of Ed* constants, default = EdUndir | EdDirOut)
	 * @return distance (number of edges on the path) or -1 if there is
	 * 		    no path between start and end */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static int getPath(const GraphType &g,
			   typename GraphType::PVertex start,
			   typename GraphType::PVertex end,
			   OutPath<VertIter, EdgeIter> path,
			   EdgeDirection mask = EdUndir|EdDirOut)
    {   assert(start && end);
        mask&=~EdLoop;
		VisitedMap<GraphType> tree;
//		if (SearchImpl::visitBase(g, start,tree, Visitors::EndVertVisitor(end),mask, 0)<0) return -1;
//		teraz wynik < 0 oznacza tylko, �e visitor przerwa� wykonywanie algorytmu
//		if (SearchImpl::visitBase(g, start,tree, Visitors::EndVertVisitor(end),mask, 0)>=0) return -1;
        SearchImpl::visitBase(g, start,tree, Visitors::EndVertVisitor(end),mask, 0);
		int res=tree[end].distance;
        if (NumberTypeBounds<int>::isPlusInfty(res)) return -1;
		if (!isBlackHole(path.vertIter) || !isBlackHole(path.edgeIter))
            getOutPath(g,tree,path,end);
		return res;
    }


	/** split graph into components
	 * @param[in] g graph to split
	 * @param[in] iters CompStore object to store component data
	 * @param[in] mask edges to consider (bitwise or of Ed* constants, default = EdUndir | EdDirOut | EdDirIn)
	 * @param[in] cont container to store intermediate data (default = use own)
	 * @return number of components
	 */
	template<class GraphType,
		 class VertContainer,
		 class CompIter,
		 class VertIter>
	static int getComponents(const GraphType &g,
				 CompStore<CompIter, VertIter> iters,
				 EdgeDirection mask,
				 VertContainer &cont)
//		EdgeDirection mask=EdDirOut|EdUndir|EdDirIn)
    {
        mask|=(mask & (EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0;
        mask&=~EdLoop;
		typename Visitors::StoreCompVisitor<CompIter, VertIter>::State st(iters);
		return visitAllBase(g, cont, Visitors::StoreCompVisitor<CompIter, VertIter>(st), mask);
    }

	template<class GraphType,
		 class CompIter,
		 class VertIter>
	static int getComponents(const GraphType &g,
				 CompStore<CompIter, VertIter> iters,
				 EdgeDirection mask = EdUndir | EdDirOut | EdDirIn) {
		VisitedMap<GraphType> cont;
		return getComponents(g, iters, mask,cont);
    };


};


/*
 * DFSParamBlock
 */
template<class GraphType, class VertContainer, class Visitor>
struct DFSParamBlock: public SearchStructs {
	const GraphType &g;
	VertContainer &visited;
	Visitor &visit;
	EdgeDirection mask;
	int component;

	DFSParamBlock(const GraphType &_g, VertContainer &_v, Visitor &_vis,
		      EdgeDirection _m, int _c):
		g(_g), visited(_v), visit(_vis), mask(_m), component(_c) {
		};
	};


/*
 * DFSBase
 */
template<class SearchImpl>
class DFSBase: public GraphSearchBase<SearchImpl> {
private:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int dfsDoVisit(DFSParamBlock<GraphType, VertContainer, Visitor> &params,
			      typename GraphType::PVertex u,
			      unsigned int depth) {
		int t, retVal = 0;
		typename GraphType::PEdge e;
		typename GraphType::PVertex v;

		if(!Visitors::visitVertexPre(params.g, params.visit, u, params.visited[u], params.visit)) return 1;
		retVal++;

		// for each neighbour
		for(e = params.g.getEdge(u, params.mask); e != NULL; e = params.g.getEdgeNext(u, e, params.mask)) {
			if(!Visitors::visitEdgePre(params.g, params.visit, e, u, params.visit)) continue;

			v = params.g.getEdgeEnd(e, u);
			if(params.visited.hasKey(v)) continue;

			params.visited[v] = SearchStructs::VisitVertLabs<GraphType>(u, e, depth + 1, params.component);
			t = dfsDoVisit<GraphType, VertContainer, Visitor>(params, v, depth + 1);
			if(t < 0) { retVal += -t; return -retVal; };
			retVal += t;
			if(!Visitors::visitEdgePost(params.g, params.visit, e, u, params.visit)) return -retVal;
			}; // for each neighbour

		if(!Visitors::visitVertexPost(params.g, params.visit, u, params.visited[u], params.visit)) return -retVal;
		return retVal;
		};

public:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int dfsVisitBase(const GraphType &g,
				typename GraphType::PVertex first,
				VertContainer &visited,
				Visitor visit,
				EdgeDirection mask,
				int component) {
		DFSParamBlock<GraphType, VertContainer, Visitor> params(g, visited, visit, mask, component);
		if(g.getVertNo() == 0) return 0;
		if(first == NULL) first = g.getVert();
		visited[first] = SearchStructs::VisitVertLabs<GraphType>(NULL, NULL, 0,component);
		return DFSBase<SearchImpl>::template dfsDoVisit<GraphType, VertContainer, Visitor>(params, first, 0);
		};
	};


/*
 * Depth-First-Search
 */
class DFS: public DFSBase<DFS> {
public:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitBase(const GraphType &g,
			     typename GraphType::PVertex start,
			     VertContainer &cont,
			     Visitor visit,
			     EdgeDirection mask,
			     int component) {
		return dfsVisitBase<GraphType, VertContainer, Visitor>(g, start, cont, visit, mask, component);
		};
};


/*
 * Preorder Depth-First-Search
 */
class DFSPreorder: public DFSBase<DFSPreorder> {
private:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int dfsPreVisitBase(const GraphType &g,
				   typename GraphType::PVertex start,
				   VertContainer &visited,
				   Visitor visit,
				   EdgeDirection mask,
				   int component,
				   Visitors::complex_visitor_tag &) {
		return dfsVisitBase<GraphType, VertContainer, Visitor>(g, start, visited, visit, mask, component);
		};

	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int dfsPreVisitBase(const GraphType &g,
				   typename GraphType::PVertex start,
				   VertContainer &visited,
				   Visitor visit,
				   EdgeDirection mask,
				   int component,
				   Visitors::simple_visitor_tag &) {
		return dfsVisitBase<GraphType,
				    VertContainer,
				    Visitors::ComplexPreorderVisitor<Visitor> >
				    (g, start, visited, Visitors::ComplexPreorderVisitor<Visitor>(visit), mask, component);
		};

public:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitBase(const GraphType &g,
			     typename GraphType::PVertex start,
			     VertContainer &visited,
			     Visitor visit,
			     EdgeDirection mask,
			     int component) {
		return dfsPreVisitBase<GraphType, VertContainer, Visitor>(g, start, visited, visit, mask, component, visit);
		};
};


/*
 * Postorder Depth-First-Search
 */
class DFSPostorder: public DFSBase<DFSPostorder> {
private:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int dfsPostVisitBase(const GraphType &g,
				    typename GraphType::PVertex start,
				    VertContainer &visited,
				    Visitor visit,
				    EdgeDirection mask,
				    int component,
				    Visitors::complex_visitor_tag &) {
		return dfsVisitBase<GraphType, VertContainer, Visitor>(g, start, visited, visit, mask, component);
		};

	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int dfsPostVisitBase(const GraphType &g,
				    typename GraphType::PVertex start,
				    VertContainer &visited,
				    Visitor visit,
				    EdgeDirection mask,
				    int component,
				    Visitors::simple_visitor_tag &) {
		return dfsVisitBase<GraphType,
				    VertContainer,
				    Visitors::ComplexPostorderVisitor<Visitor> >
				    (g, start, visited, Visitors::ComplexPostorderVisitor<Visitor>(visit), mask, component);
		};

public:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitBase(const GraphType &g,
			     typename GraphType::PVertex start,
			     VertContainer &visited,
			     Visitor visit,
			     EdgeDirection mask,
			     int component) {
		return dfsPostVisitBase<GraphType, VertContainer, Visitor>(g, start, visited, visit, mask, component, visit);
		};
};



/*
 * Breadth-First-Search
 */
class BFS: public GraphSearchBase<BFS> {
private:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int bfsDoVisit(const GraphType &g,
			      typename GraphType::PVertex first,
			      VertContainer &visited,
			      Visitor visit,
			      EdgeDirection mask,
			      int component) {
		unsigned int depth, n, retVal;
		typename GraphType::PEdge e;
		typename GraphType::PVertex u, v;
		typename GraphType::PVertex LOCALARRAY(buf,g.getEdgeNo()+3);
		QueueInterface<typename GraphType::PVertex*> cont(buf,g.getEdgeNo()+3);

		n = g.getVertNo();
		if(n == 0) return 0;
		if(first == NULL) first = g.getVert();

		visited[first] = SearchStructs::VisitVertLabs<GraphType>(NULL, NULL, 0, component);
		cont.push(first);
		retVal = 0;

		while(!cont.empty()) {
			u = cont.top();
			depth = visited[u].distance;
			cont.pop();

			if(!Visitors::visitVertexPre(g, visit, u, visited[u], visit)) {
				retVal++;
				continue;
				};

			// for each neighbour
			for(e = g.getEdge(u, mask); e != NULL; e = g.getEdgeNext(u, e, mask)) {
				v = g.getEdgeEnd(e, u);
				if(!Visitors::visitEdgePre(g, visit, e, u, visit)) continue;
				if(visited.hasKey(v)) continue;
				visited[v] = SearchStructs::VisitVertLabs<GraphType>(u, e, depth + 1, component);
				cont.push(v);
				if(!Visitors::visitEdgePost(g, visit, e, u, visit)) return -retVal;
				};
			retVal++;
			if(!Visitors::visitVertexPost(g, visit, u, visited[u], visit)) return -retVal;

			}; // while(!cont.empty())

		return retVal;
	};

    public:
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitBase(const GraphType &g,
			     typename GraphType::PVertex start,
			     VertContainer &visited,
			     Visitor visit,
			     EdgeDirection mask,
			     int component) {
		return bfsDoVisit<GraphType, VertContainer, Visitor>(g, start, visited, visit, mask, component);
		};
};



/*
 * lexicographical Breadth-First-Search
 */
class LexBFS: public GraphSearchBase<LexBFS> {
public:

	template<class Graph>
	class LexVisitContainer {
	public:
		LexVisitContainer(): m_sets(), m_splits(), m_vertexToListPos()	{};
		~LexVisitContainer()		{ clear(); };

		void clear()			{ m_sets.clear(); m_splits.clear(); /*m_vertexToListPos.clear();*/ };
		bool empty()			{ return m_sets.empty(); };


		void initialize(const Graph &g, typename Graph::PVertex first) {
			unsigned int i;
			typename Graph::PVertex v;
			typename NodeList::iterator np;

			clear();
			m_sets.push_back(Node());
			np = --(m_sets.end());
			m_lastSet = np;

/*			np->first.push_back(first);
			m_vertexToListPos[first] = std::make_pair(np, --(np->first.end()));

			for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) {
				if(v == first) continue;
				np->first.push_back(v);
				m_vertexToListPos[v] = std::make_pair(np, --(np->first.end()));
				};
			np->second = np->first.begin();
*/			};


		typename Graph::PVertex top() {
			return m_sets.front().first.front();
			};


		void pop() {
			while(!m_sets.empty() && m_sets.front().first.empty()) {
				if(m_sets.begin() == m_lastSet) {
					m_lastSet = m_sets.end();
					};
				m_sets.pop_front();
				};
			if(m_sets.size() == 0) return;
			m_sets.front().first.front();
			m_sets.front().first.pop_front();
			m_sets.front().second = m_sets.front().first.begin();
			while(!m_sets.empty() && m_sets.front().first.empty()) {
				if(m_sets.begin() == m_lastSet) {
					m_lastSet = m_sets.end();
					};
				m_sets.pop_front();
				};
			};


		void push(typename Graph::PVertex v) {
			typename LexList::iterator vertListPos;
			typename NodeList::iterator np, vertNode;

			if(!m_vertexToListPos.hasKey(v)) {
				if(m_lastSet == m_sets.end()) {
					m_sets.push_back(Node());
					np = --(m_sets.end());
					m_lastSet = np;
					};
				np = m_lastSet;
				if(np->first.empty()) {
					np->first.push_back(v);
					np->second = np->first.begin();
				} else {
					np->first.push_back(v);
					};
				m_vertexToListPos[v] = std::make_pair(np, --(np->first.end()));
				return;
				};

			vertNode = m_vertexToListPos[v].first;
			vertListPos = m_vertexToListPos[v].second;

			LexList &vertList = vertNode->first;
			typename LexList::iterator &splitPos = vertNode->second;

			if(splitPos == vertListPos) {
				m_splits.push_back(vertNode);
				++splitPos;
			} else {
				if(splitPos == vertList.begin()) m_splits.push_back(vertNode);
				vertList.splice(splitPos, vertList, vertListPos);
				};
			};


		void step() {
			typename LexList::iterator lit, le;
			typename NodeList::iterator newSet;
			typename std::list<typename NodeList::iterator>::iterator e, it;

			e = m_splits.end();
			it = m_splits.begin();
			while(it != e) {
				if((*it)->second != (*it)->first.begin() &&
				   (*it)->second != (*it)->first.end()) {
					newSet = m_sets.insert(*it, Node());
					newSet->first.splice(newSet->first.begin(), (*it)->first,
							     (*it)->first.begin(), (*it)->second);
					newSet->second = newSet->first.begin();

					le = newSet->first.end();
					for(lit = newSet->first.begin(); lit != le; ++lit)
						m_vertexToListPos[*lit] = std::make_pair(newSet, lit);
					};
				++it;
				};

			m_splits.clear();
			};


		void dump() {
			typename NodeList::iterator it, e;
			typename LexList::iterator lit, le, split;
			e = m_sets.end();
			for(it = m_sets.begin(); it != e; ++it) {
				split = it->second;
				printf("<");
				if(it == m_lastSet) printf("<");
				le = it->first.end();
				for(lit = it->first.begin(); lit != le; ++lit) {
					if(lit == split) printf(" |");
					printf(" %u", ((unsigned int)(*lit) & 0xffff) / 8);
					};
				if(it == m_lastSet) printf(" >>  ");
				else printf(" >  ");
				};
			printf("\n");
			fflush(stdout);
			};

	private:
		typedef std::list<typename Graph::PVertex> LexList;
		typedef std::pair<LexList, typename LexList::iterator> Node;
		typedef std::list<Node> NodeList;
		typedef AssocArray<typename Graph::PVertex, std::pair<typename NodeList::iterator, typename LexList::iterator> > MapStruct;
		NodeList m_sets;
		typename NodeList::iterator m_lastSet;
		typename std::list<typename NodeList::iterator> m_splits;
		MapStruct m_vertexToListPos;
    };


	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitBase(const GraphType &g,
			     typename GraphType::PVertex start,
			     VertContainer &visited,
			     Visitor visit,
			     EdgeDirection mask,
			     int component) {
		unsigned int depth, n, retVal;
		typename GraphType::PEdge e;
		typename GraphType::PVertex u, v;
		LexVisitContainer<GraphType> cont;

		n = g.getVertNo();
		if(n == 0) return 0;
		if(start == NULL) start = g.getVert();

		cont.initialize(g, start);

		visited[start] = SearchStructs::VisitVertLabs<GraphType>(NULL, NULL, 0, component);
		cont.push(start);
		retVal = 0;

		while(!cont.empty()) {
			u = cont.top();
			depth = visited[u].distance;

			if(!Visitors::visitVertexPre(g, visit, u, visited[u], visit)) {
				retVal++;
				continue;
				};
			cont.pop();

			// for each neighbour
			for(e = g.getEdge(u, mask); e != NULL; e = g.getEdgeNext(u, e, mask)) {
				v = g.getEdgeEnd(e, u);
				if(!Visitors::visitEdgePre(g, visit, e, u, visit)) continue;
				if(visited.hasKey(v)) continue;
				visited[v] = SearchStructs::VisitVertLabs<GraphType>(u, e, depth + 1, component);
				cont.push(v);
				if(!Visitors::visitEdgePost(g, visit, e, u, visit)) return -retVal;
				};
			cont.step();

			retVal++;
			if(!Visitors::visitVertexPost(g, visit, u, visited[u], visit)) return -retVal;

			}; // while(!cont.empty())

		return retVal;
		};
};



/*
 * Cheriyan–Mehlhorn/Gabow algorithm
 */
class SCC: protected SearchStructs {
protected:
	struct SCCData {
		int preIdx;
		bool assigned;
		SCCData(int p = 0, bool a = false): preIdx(p), assigned(a)	{};
		};

	// stan przechowywany podczas przegl¹dania grafu dfs-em
	template<class GraphType,
		 class CompIter,
		 class VertIter,
		 class CompMap>
	class SCCState {

	public:
		SCCState(CompStore<CompIter, VertIter> _i,
			 CompMap &cm,typename GraphType::PVertex * buf1, typename GraphType::PVertex * buf2,int n):
			vmap(), s(buf1,n), p(buf2,n), iters(_i),  idx(0),
			count(0), c(0), compMap(cm) { };

		// dodanie wierzcho³ka do bie¿¹cej sk³adowej
		void addVert(typename GraphType::PVertex v) {
			*(iters.vertIter) = v;
			++(iters.vertIter);
			if (!isBlackHole(compMap)) compMap[v] = count;
			vmap[v].assigned = true;
			idx++;
        };


		// zakoñczenie starej i rozpoczêcie nowej sk³adowej
		void newComp() {
			*(iters.compIter) = idx;
			++(iters.compIter);
			++count;
        };

		typename DefaultStructs::template AssocCont<typename GraphType::PVertex, SCCData>::Type vmap; // dane dla ka¿dego wierzcho³ka
		StackInterface<typename GraphType::PVertex*> s,p;
		CompStore<CompIter, VertIter> iters;	// iteratory wyjœciowe
		CompMap &compMap;			// mapa vertex -> componentid
		unsigned int idx;		// iloœæ dodanych wierzcho³ków
		int count;			// iloœæ dodanych ca³ych sk³adowych
		int c;
    }; // SCCState


	template<class GraphType,
		 class CompIter,
		 class VertIter,
		 class CompMap>
	class SCCVisitor: public Visitors::complex_visitor_tag,
			  public Visitors::no_component_visitor_tag {
	public:
		SCCVisitor(SCCState<GraphType, CompIter, VertIter, CompMap> &s):
			state(s) {
			state.c = 0;
			state.idx = 0;
			state.newComp();
			state.count = 0;
			};


		bool visitVertexPre(const GraphType &/*g*/,
				    typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &) {
			state.vmap[u] = SCCData(state.c, false);
			state.c++;
			state.p.push(u);
			state.s.push(u);
			return true;
        };


		bool visitVertexPost(const GraphType &,
				     typename GraphType::PVertex u,
				     VisitVertLabs<GraphType> &) {
			if(state.p.empty() || state.p.top() != u) return true;
			while(!state.s.empty() && state.s.top() != u) {
				state.addVert(state.s.top());
				state.s.pop();
            };
			state.addVert(state.s.top());
			state.newComp();
			state.s.pop();
			state.p.pop();
			return true;
        };


		bool visitEdgePre(const GraphType &g, typename GraphType::PEdge e,
				  typename GraphType::PVertex u) {
			typename GraphType::PVertex v;
			v = g.getEdgeEnd(e, u);
			if(!state.vmap.hasKey(v)) return true;
			if(!state.vmap[v].assigned) {
				while(!state.p.empty() && state.vmap[state.p.top()].preIdx > state.vmap[v].preIdx)
					state.p.pop();
			};
			return false;
        };


		bool visitEdgePost(const GraphType &, typename GraphType::PEdge e,
				   typename GraphType::PVertex ) {
			return true;
			};

	private:
		SCCState<GraphType, CompIter, VertIter, CompMap> &state;
    };


public:
	/** split graph g into strongly connected components
	 * @param[in] g graph to split
	 * @param[out] out CompStore object to store vertex components
	 * @param[out] compMap map from vertex to number of its component
	 * @return number of components */
	template<class GraphType,
		 class CompIter,
		 class VertIter,
		 class CompMap>
	static int get(const GraphType &g,
				 CompStore<CompIter, VertIter> out,
				 CompMap &compMap) {
		int rv;
		VisitedMap<GraphType> vertCont;
		typename GraphType::PVertex LOCALARRAY(buf1,g.getVertNo()+1);
        typename GraphType::PVertex LOCALARRAY(buf2,g.getVertNo()+1);
		SCCState<GraphType, CompIter, VertIter, CompMap> state(out, compMap,buf1,buf2,g.getVertNo());
		SCCVisitor<GraphType, CompIter, VertIter, CompMap> visit(state);
		rv = DFS::visitAllBase(g, vertCont,visit, EdDirOut | EdUndir);
		if(rv < 0) return rv;
		return state.count;
    };


}; // class SCC



class DAGAlgs: protected SearchStructs {

    public:

    /** topological sorting of a graph;
	 * @param[in] g graph to sort
	 * @param[out] out iterator to write vertices in topological order
	 * @return test if g is DAG
	 */
	template<class GraphType,
		 class VertIter>
	static void topOrd(const GraphType &g, VertIter out) {
	    VisitedMap<GraphType> visited;
	    DFSPostorder::visitAllBase(g,visited,Visitors::StoreTargetToVertIter<typename GraphType::PVertex*>(out),EdDirIn);
	}

	template<class GraphType, class Iter>
    static bool isDAG(const GraphType &g, Iter beg,Iter end)
    {
        if (g.getEdgeNo(EdUndir|EdLoop)) return false;
	    if (!g.getVertNo()) return true;
	    typename DefaultStructs::AssocCont<typename GraphType::PVertex, int >::Type topord;
	    int licz=0;
	    for(Iter i=beg;i!=end;++i)
	        topord[*i]=licz++;
        assert(topord.size()==g.getVertNo());
	    for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            if (topord[e->getEnds().first]>topord[e->getEnds().second]) return false;
        return true;
    }

	template<class GraphType>
    static bool isDAG(const GraphType &g)
    {
        typename GraphType::PVertex LOCALARRAY(buf,g.getVertNo());
        topOrd(g,buf);
        return isDAG(g,buf,buf+g.getVertNo());
    }


};



/*
 * Blocks -- biconnected components
 */
class Blocks: protected SearchStructs {
protected:
	// dane przechowywane dla każdego wierzchołka
	template<class GraphType>
	struct BiConVData {
		unsigned int depth;	// głębokość w dfs-tree
		unsigned int lowpoint;	// lowpoint
		unsigned int count;	// liczba składowych w których siedzi
		bool iscut;		// czy jest cut-vertex-em
		int visited;		// nr składowej, w której był ostatnio
					// odwiedzony
		int sons;		// liczba potomków w dfs-tree
		int vblFirst;		// indeks do VertBlockList
		};

	// informacje o blokach, do kt�rych nale�y wierzcho�ek
	struct VertBlockList {
		int block;		// numer bloku
		int next;		// indeks nast�pnego bloku
		};			// next < 0 oznacza brak nast�pnego


	// stan przechowywany podczas przeglądania grafu dfs-em
	template<class GraphType,
		 class CompIter,
		 class VertIter,
		 class EdgeMap>
	class BiConState {
	public:
		typedef typename GraphType::PVertex PVertex;

		BiConState(CompStore<CompIter, VertIter> _i,
			   EdgeMap &em,
			   EdgeDirection _m,
			   std::pair<typename GraphType::PEdge *, int> st,
			   VertBlockList *_vbl):
			vmap(), emap(), estk(st.first, st.second), iters(_i),
			idx(0), count(0), mask(_m), vbl(_vbl), vblAlloc(0),
			outEMap(em) { };

		// dodanie wierzchołka do bieżącej składowej
		void addVert(typename GraphType::PVertex v) {
			*(iters.vertIter) = v;
			++(iters.vertIter);
			vmap[v].count++;
			vbl[vblAlloc].block = count;
			vbl[vblAlloc].next = vmap[v].vblFirst;
			vmap[v].vblFirst = vblAlloc;
			vblAlloc++;
			idx++;
			};

		// dodanie kraw�dzi do bie��cej sk�adowej
		void addEdge(typename GraphType::PEdge e) {
			if (!isBlackHole(outEMap)) outEMap[e] = count;
			};

		// zakończenie starej i rozpoczęcie nowej składowej
		void newComp() {
			*(iters.compIter) = idx;
			++(iters.compIter);
			++count;
			};

		typename DefaultStructs::AssocCont<typename GraphType::PVertex, BiConVData<GraphType> >::Type vmap; // dane dla każdego wierzchołka
		typename DefaultStructs::AssocCont<typename GraphType::PEdge, bool>::Type emap; // dane dla każdego wierzchołka
		StackInterface<typename GraphType::PEdge*> estk;	// stos odwiedzanych krawędzi
		CompStore<CompIter, VertIter> iters;	// iteratory wyjściowe
		EdgeMap &outEMap;		// mapa wyj�ciowa edge -> block id
		VertBlockList *vbl;
		int vblAlloc;
		unsigned int idx;		// ilość dodanych wierzchołków
		int count;			// ilość dodanych całych składowych
		EdgeDirection mask;		// maska typu przeglądanych krawędzi
    }; // BiConState


	template<class GraphType,
		 class CompIter,
		 class VertIter,
		 class EdgeMap>
	class BiConVisitor: public Visitors::complex_visitor_tag,
			    public Visitors::no_component_visitor_tag {
	public:
		BiConVisitor(BiConState<GraphType, CompIter, VertIter, EdgeMap> &s):
			state(s) {
			state.idx = 0;
			state.newComp();
			state.count = 0;
			};


		bool visitVertexPre(const GraphType &/*g*/,
				    typename GraphType::PVertex u,
				    VisitVertLabs<GraphType> &data) {
			state.vmap[u].count = 0;
			state.vmap[u].iscut = false;
			state.vmap[u].visited = -1;
			state.vmap[u].lowpoint = data.distance;
			state.vmap[u].depth = data.distance;
			state.vmap[u].sons = 0;
			state.vmap[u].vblFirst = -1;
			return true;
			};


		bool visitVertexPost(const GraphType &g,
				     typename GraphType::PVertex u,
				     VisitVertLabs<GraphType> &data) {
			if(g.getEdgeNo(u, state.mask) == 0) {
				state.addVert(u);	// sk�adowa z izolowanym
				state.newComp();	// wierzcho�kiem
				return true;
				};
			if(!state.estk.empty() && data.distance == 0) {
				std::pair<typename GraphType::PVertex, typename GraphType::PVertex> ends;
				while(!state.estk.empty()) {
					ends = g.getEdgeEnds(state.estk.top());
					state.addEdge(state.estk.top());
					if(state.vmap[ends.first].visited < state.count) {
						state.addVert(ends.first);
						state.vmap[ends.first].visited = state.count;
						};
					if(state.vmap[ends.second].visited < state.count) {
						state.addVert(ends.second);
						state.vmap[ends.second].visited = state.count;
						};
					state.estk.pop();
					};
				state.newComp();
				};
			return true;
			};


		bool visitEdgePre(const GraphType &g, typename GraphType::PEdge e,
				  typename GraphType::PVertex u) {
			EdgeType tp;
			typename GraphType::PVertex v;
			if(state.emap.hasKey(e)) return false;
			state.emap[e] = true;
			tp = g.getEdgeType(e);
			if(tp == Loop) {
				state.addEdge(e);
				state.addVert(u);
				state.newComp();
				return false;
				};
			v = g.getEdgeEnd(e, u);
			if(state.vmap.hasKey(v)) {
				if(state.vmap[v].depth < state.vmap[u].depth) {
					state.estk.push(e);
					};
				state.vmap[u].lowpoint = min(state.vmap[u].lowpoint,
							     state.vmap[v].depth);
				return false;
				};
			state.estk.push(e);
			state.vmap[u].sons++;
			return true;
			};


		bool visitEdgePost(const GraphType &g, typename GraphType::PEdge e,
				   typename GraphType::PVertex u) {
			typename GraphType::PEdge se;
			typename GraphType::PVertex v;
			v = g.getEdgeEnd(e, u);

			state.vmap[u].lowpoint = min(state.vmap[u].lowpoint,
						     state.vmap[v].lowpoint);

			if((state.vmap[v].lowpoint >= state.vmap[u].depth && state.vmap[u].depth > 0)
			   || (state.vmap[u].depth == 0 && state.vmap[u].sons > 1)) {
				state.vmap[u].iscut = true;
				std::pair<typename GraphType::PVertex, typename GraphType::PVertex> ends;
				while(!state.estk.empty()) {
					se = state.estk.top();
					ends = g.getEdgeEnds(se);
					state.addEdge(se);
					if(state.vmap[ends.first].visited < state.count) {
						state.addVert(ends.first);
						state.vmap[ends.first].visited = state.count;
						};
					if(state.vmap[ends.second].visited < state.count) {
						state.addVert(ends.second);
						state.vmap[ends.second].visited = state.count;
						};
					state.estk.pop();

					if(se == e) break;

//					if((ends.first == v && ends.second == u)
//					   || (ends.first == u && ends.second == v)) {
//						break;
//						};
					};
				state.newComp();
				};
			return true;
			};

	private:
		BiConState<GraphType, CompIter, VertIter, EdgeMap> &state;
		};


	template<class State,
		 class VertMap,
		 class VertBlockIter>
	static void storeBlocksData(State &state,
				    VertBlockList *vertBlockList,
				    VertMap &vertMap,
				    VertBlockIter &vertBlocks) {
		if(true/*!isBlackHole(vertMap)*/) {
			int outIdx = 0, p;
			for(typename State::PVertex v = state.vmap.firstKey(),
						    e = state.vmap.lastKey();
			    1;
			    v = state.vmap.nextKey(v)) {
                if (!isBlackHole(vertMap))
                    vertMap[v] = VertData(state.vmap[v].count, outIdx);
				p = state.vmap[v].vblFirst;
				while(p >= 0) {
					*vertBlocks = vertBlockList[p].block;
					++vertBlocks;
					++outIdx;
					p = vertBlockList[p].next;
					};
				if(v == e) break;
				};
			};
		};


public:
	/** data in VertDataMap assigned to vertex */
	struct VertData {
		int blockNo;	/**< number of blocks vertex belongs to */
		int firstBlock;	/**< index of the first block number in vertexBlocks */
//		VertData()						{};
		VertData(int b=0, int f=-1): blockNo(b), firstBlock(f)	{};
		};

	/** split graph g into biconnected components
	 * @param[in] g graph to split
	 * @param[out] vertMap map from vertex to number of components it belongs to
	 * @param[out] edgeMap map from edge to component it belongs to
	 * @param[out] blocks CompStore object to store vertex components
	 * @param[out] eout CompStore object to store edge components
	 * @param[in] mask edges to consider (bitwise or of Ed* constants, default = EdUndir | EdDirOut | EdDirIn | EdLoop)
	 * @return number of components */
	template<class GraphType,
		 class VertDataMap,
		 class EdgeDataMap,
		 class CompIter,
		 class VertIter,
		 class VertBlockIter>
	static int split(const GraphType &g,
			 VertDataMap &vertMap,
			 EdgeDataMap &edgeMap,
			 CompStore<CompIter, VertIter> blocks,
			 VertBlockIter vertBlocks,
			 EdgeDirection mask = EdAll) {
		int rv;
		VisitedMap<GraphType> visited;
		typename GraphType::PEdge LOCALARRAY(stbuf, g.getEdgeNo() + 1);
		VertBlockList LOCALARRAY(vertBlockList, g.getEdgeNo() * 2 + g.getVertNo());
		BiConState<GraphType, CompIter, VertIter, EdgeDataMap> state(blocks, edgeMap, mask, std::make_pair(stbuf, g.getEdgeNo() + 1), vertBlockList);
		BiConVisitor<GraphType, CompIter, VertIter, EdgeDataMap> visit(state);

//		typename DefaultStructs::AssocCont<typename GraphType::PVertex,VertData >::Type localvertMap;
//        typename BlackHoleSwitch<VertDataMap,typename DefaultStructs::AssocCont<typename GraphType::PVertex,VertData >::Type >::Type &
//                    vertMap=
//             BlackHoleSwitch<VertDataMap,typename DefaultStructs::AssocCont<typename GraphType::PVertex,VertData >::Type >::get(avertMap,localvertMap);


		if(mask & (EdDirIn | EdDirOut)) mask |= EdDirOut | EdDirIn;

		rv = DFS::visitAllBase(g, visited, visit, mask);
		if(rv < 0) return rv;

		storeBlocksData(state, vertBlockList, vertMap, vertBlocks);

		return state.count;
		};


	/** split a component containing vertex u in graph g into biconnected components
	 * @param[in] g graph to split
	 * @param[in] u vertex identifying component
	 * @param[out] out CompStore object to store vertex components
	 * @param[out] eout CompStore object to store edge components
	 * @param[out] countMap map from vertex to number of components it belongs to
	 * @param[in] mask edges to consider (bitwise or of Ed* constants, default = EdUndir | EdDirOut | EdDirIn | EdLoop)
	 * @return number of components */
	template<class GraphType,
		 class VertDataMap,
		 class EdgeDataMap,
		 class CompIter,
		 class VertIter,
		 class VertBlockIter>
	static int splitComp(const GraphType &g,
			     typename GraphType::PVertex u,
			     VertDataMap &vertMap,
			     EdgeDataMap &edgeMap,
			     CompStore<CompIter, VertIter> blocks,
			     VertBlockIter vertBlocks,
			     EdgeDirection mask = EdAll) {
		int rv;
		VisitedMap<GraphType> visited;
		typename GraphType::PEdge LOCALARRAY(stbuf, g.getEdgeNo() + 1);
		VertBlockList LOCALARRAY(vertBlockList, g.getEdgeNo() * 2 + g.getVertNo());
		BiConState<GraphType, CompIter, VertIter, EdgeDataMap> state(blocks, edgeMap, mask, std::make_pair(stbuf, g.getEdgeNo() + 1), vertBlockList);
		BiConVisitor<GraphType, CompIter, VertIter, EdgeDataMap> visit(state);

//		typename DefaultStructs::AssocCont<typename GraphType::PVertex,VertData >::Type localvertMap;
//        typename BlackHoleSwitch<VertDataMap,typename DefaultStructs::AssocCont<typename GraphType::PVertex,VertData >::Type >::Type &
//                    vertMap=
//             BlackHoleSwitch<VertDataMap,typename DefaultStructs::AssocCont<typename GraphType::PVertex,VertData >::Type >::get(avertMap,localvertMap);

		if(mask & (EdDirIn | EdDirOut)) mask |= EdDirOut | EdDirIn;

		rv = DFS::visitBase(g, u, visited, visit, mask, 0);
		if(rv != 0) {
			storeBlocksData(state, vertBlockList, vertMap, vertBlocks);
			return state.count;
		} else return 0;
		};

}; // class Blocks



} // namespace Koala

#endif
