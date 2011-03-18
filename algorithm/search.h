#ifndef DEF_SEARCH_H
#define DEF_SEARCH_H

#include <vector>
#include <map>
#include <list>
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


    class EmptyVisitor {
    public:
        EmptyVisitor() {};

        template<class GraphType>
        bool operator()(GraphType & ,			// graph
                typename GraphType::PVertex ,	// 'from' vertex
                VisitVertLabs<GraphType>&)
            {
                return true;
            };
    };

    class EndVertVisitor {
        void *ptr;
    public:
        EndVertVisitor(void* arg) :ptr(arg) {};

        template<class GraphType>
        bool operator()(GraphType & ,			// graph
                typename GraphType::PVertex u,	// 'from' vertex
                VisitVertLabs<GraphType>&)
            {
                return (void*)u!=ptr;
            };
    };

    template<class OutVIter>
    class StoreTargetToVIter {
    public:
        StoreTargetToVIter(OutVIter& i): m_iter(i) {};

        template <class GraphType>
        bool operator()(GraphType & ,
                typename GraphType::PVertex u,
                VisitVertLabs<GraphType>&)
            {
                *m_iter = u;
                ++m_iter;
                return true;
            };
    private:
        OutVIter& m_iter;
    };

    template <class VIter,class CIter> class StoreCompVisitor {

        public:
        struct State {
            int vpos,comp,startpos;
            VIter vit;
            CIter cit;

            State(CompStore<CIter,VIter> rec) :
                    vpos(-1), comp(0), startpos(0), vit(rec.vertIter), cit(rec.compIter) {}
        } * state;

        StoreCompVisitor(State *ptr) :  state(ptr)
        { }

        template<class GraphType>
        bool operator()(GraphType & g,typename GraphType::PVertex v,VisitVertLabs<GraphType>& rec)
            {   state->vpos++;
                if (v) { *state->vit=v; ++state->vit; }
                if (rec.component!=state->comp)
                {
                    *state->cit=state->vpos; ++state->cit;
                    state->comp++;state->startpos=state->vpos;
                }
                return true;
            };
    };


    template <class V1, class V2> struct ComposedVisisitor {
            V1 v1;
            V2 v2;

            ComposedVisisitor(V1 a, V2 b) : v1(a), v2(b) {}
            template<class GraphType>
            bool operator()(GraphType & g,			// graph
                typename GraphType::PVertex v,	// 'from' vertex
                VisitVertLabs<GraphType>& r)
            {
                bool r1=v1(g,v,r);
                bool r2=v2(g,v,r);
                return r1 && r2;
            };

        };
        // funkcja tworzaca, analogia make_pair
    template <class V1, class V2>
    static ComposedVisisitor<V1,V2> compVisitor(V1 a, V2 b) { return ComposedVisisitor<V1,V2>(a,b); }



    protected:

    class DefaultStructs {
        public:

        template <class A, class B> class AssocCont {
            public:
            typedef AssocArray<A,B> Type;
        };

    };

};

template<class SearchImpl>
class GraphSearchBase : public ShorPathStructs, public SearchStructs {
    protected:

	/** visit vertices in order depending on the Container class
	 * (DFS for stack, BFS for queue)
	 * @param[in] g graph to visit
	 * @param[in] visited container to store data
	 * @param[in] first vertex to start with (any vertex if NULL)
	 * @param[in] type and direction of traversed edges
	 * @param[in] visit visitor called for each vertex (parameters are: vertex, bool (true for on open, false on close), number of the component)
	 * @param[in] componentid number of the current component (counting from 0; passed to visitor)
	 * @return number of visited vertices or -1 after visitor return false
	 */
	template<class Container,			// stos/kolejka/etc.
		 class GraphType,
		 class VertContainer,
		 class Visitor,
		 bool preorder>				// preorder/postorder
	static int graphVisitBase(GraphType &g,
					   VertContainer &vTab,
					   typename GraphType::PVertex first,
					   EdgeDirection mask,
					   Visitor visit,
					   int componentid
    )
    {   bool open;
        int depth, n, retVal;
        typename GraphType::PEdge e;
        typename GraphType::PVertex u, v;
        std::pair<std::pair<typename GraphType::PVertex,bool>, VisitVertLabs<GraphType> > LOCALARRAY(buf,2*g.getEdgeNo()+2);
        Container cont(buf,2*g.getEdgeNo()+1);

        mask&=(~EdLoop);
        n = g.getVertNo();
        if(n == 0) return 0;
        retVal = 0;
        if(first == NULL) first = g.getVert();

        cont.push(std::make_pair(std::make_pair(first,true),VisitVertLabs<GraphType>(0,0,0,componentid)));

        while(!cont.empty()) {
            u = cont.top().first.first;
            open = cont.top().first.second;
            depth = cont.top().second.distance;

            if(open) {
                if (vTab.hasKey(u)) { cont.pop(); continue; }
                cont.top().second.get(vTab[u]);
                if(preorder)
                {
                    if (!visit(g, u, vTab[u])) return -1;
                    retVal++;
                    cont.pop();
                } else { cont.top().first.second = false; };
            } else if(!preorder) {
                if (!visit(g, u, vTab[u])) return -1;
                retVal++;
                cont.pop();
                continue;
            };

            // for each neighbour
            for(e = g.getEdge(u, mask); e != NULL; e = g.getEdgeNext(u, e, mask)) {
                v = g.getEdgeEnd(e, u);
                if(vTab.hasKey(v)) continue;
                //visited[v] = TreeNode<GraphType>(u, e, depth + 1);
                cont.push(std::make_pair(std::make_pair(v, true),VisitVertLabs<GraphType>(u,e,depth+1,componentid)));
            };

        }; // while(!cont.empty())

        return retVal;
	};

    public:

	/** visit all vertices in a graph
	 * @param[in] g graph containing vertices to visit
	 * @param[in] visited container to store data
	 * @param[in] type and direction of traversed edges
	 * @param[in] visit visitor called for each vertex (parameters are: vertex, bool (true for on open, false on close), number of the component)
	 * @return number of components or -1 after visitor return false
	 */
	template<class GraphType,
            class VertContainer,
            class Visitor>
	static int visitAll(GraphType &g,
					 VertContainer &vTab,
					 EdgeDirection mask,
					 Visitor visit)
    {
        int component;
        typename GraphType::PVertex first;
//TODO:  mask|=(mask & (EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0; ???

        component = 0;
        first = g.getVert();
//        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v)) vTab.delKey(v);
        while(first != NULL) {
            if (SearchImpl::visit(g, vTab, first, mask, visit, component)==-1) return -1;
            do {
                first = g.getVertNext(first);
            } while(first != NULL && vTab.hasKey(first));
            component++;
            };
        return component;
    };

	/** build a visit-from tree rooted at 'root'
	 * @param[in] graph graph containing vertices to visit
	 * @param[out] tree map with VisitVertLabs<GraphType> values to store vertices in
	 * @param[in] root vertex to start from
	 * @param[in] type and direction of traversed edges
	 * @return number of reached vertices
	 */

	template<class GraphType,
		 class VertContainer, class Iter>
	static int scanAttainable(const GraphType &ag,
                    VertContainer &atree,
                    Iter iter,
				    typename GraphType::PVertex root,
				    EdgeDirection mask=EdDirOut|EdUndir
				    )
        {   GraphType& g= const_cast<GraphType&>(ag);
            assert(root);
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type localtree;
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type >::Type &
                tree=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type >
                    ::get(atree,localtree);

//            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v)) tree.delKey(v);
            return SearchImpl::visit(g, tree, root, mask, StoreTargetToVIter<Iter>(iter), 0);
		};

	template<class GraphType,
		 class VertContainer,class Iter>
	static int scan(const GraphType &ag,
                    VertContainer &atree,
                    Iter iter,
				    EdgeDirection mask=EdDirOut|EdUndir|EdDirIn
				    )
        {   GraphType& g= const_cast<GraphType&>(ag);
            mask|=(mask & (EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0;
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type localtree;
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type >::Type &
                tree=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type >
                    ::get(atree,localtree);

            return visitAll(g, tree, mask, StoreTargetToVIter<Iter>(iter));
		};


    template<class GraphType>
	static  Set<typename GraphType::PVertex>
            getAttainableSet(const GraphType &g,
				    typename GraphType::PVertex root,
				    EdgeDirection mask=EdDirOut|EdUndir)
    {   assert(root);
        Set<typename GraphType::PVertex> res;
        scanAttainable(g,blackHole(),setInserter(res),root,mask);
        return res;
    }

	template<class GraphType,
		 class OutVIter,
		 class OutEIter>
	static int getPath(const GraphType &ag,OutPath<OutVIter, OutEIter> path,
				    typename GraphType::PVertex start,
				    typename GraphType::PVertex end,
				    EdgeDirection mask=EdUndir|EdDirOut)
    {   assert(start && end);
        GraphType& g= const_cast<GraphType&>(ag);
		typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type tree;
		if (SearchImpl::visit(g, tree, start, mask, EndVertVisitor(end), 0)!=-1) return -1;
		int res=tree[end].distance;
		if (!isBlackHole(path.vertIter) || !isBlackHole(path.edgeIter))
            getOutPath(g,tree,path,end);
		return res;
    }


    template<class VertContainer, class GraphType, class CompIter, class VertIter>
    static int getComponents(
		const GraphType &ag,
		VertContainer &avtab,
		CompStore<CompIter,VertIter> iters,
		EdgeDirection mask=EdDirOut|EdUndir|EdDirIn)
    {   GraphType& g= const_cast<GraphType&>(ag);
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type localtab;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template
                AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type >::Type & vtab=
            BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex, VisitVertLabs<GraphType> >::Type >
                ::get(avtab,localtab);

        *iters.compIter=0;++iters.compIter;
        if (g.getVertNo()==0)
        {
//            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v)) vtab.delKey(v);
            return 0;
        }
        typename StoreCompVisitor<VertIter,CompIter>::State st(iters);
        StoreCompVisitor<VertIter,CompIter> visit(&st);
        mask|=(mask & (EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0;
        int res=visitAll(g, vtab, mask, visit);
        VisitVertLabs<GraphType> x;
        visit(g,0,x);
        return res;
    }

};


class BFS: public GraphSearchBase<BFS> {
public:
	template<class GraphType,
            class VertContainer,
            class Visitor>
	static int visit(GraphType &g,
                      VertContainer& vTab,
				      typename GraphType::PVertex start,
				      EdgeDirection mask,
				      Visitor visit,
				      int compId)
        {
		return graphVisitBase<QueueInterface<std::pair<std::pair<typename GraphType::PVertex, bool>, VisitVertLabs<GraphType> > * >,
				      GraphType,
				      VertContainer,
				      Visitor,
				      true>(g, vTab, start, mask, visit, compId);
		};
};


/*
 * Depth-First-Search postorder
 */
class DFSPostord: public GraphSearchBase<DFSPostord> {
public:
	template<class GraphType,
            class VertContainer,
            class Visitor>
	static int visit(GraphType &g,
                      VertContainer& vTab,
				      typename GraphType::PVertex start,
				      EdgeDirection mask,
				      Visitor visit,
				      int compId)
        {
		return graphVisitBase<StackInterface<std::pair<std::pair<typename GraphType::PVertex, bool>, VisitVertLabs<GraphType> > * >,
				      GraphType,
				      VertContainer,
				      Visitor,
				      false>(g, vTab, start, mask, visit, compId);
		};
};
/*
 * Depth-First-Search preorder
 */
class DFSPreord: public GraphSearchBase<DFSPreord> {
public:
	template<class GraphType,
            class VertContainer,
            class Visitor>
	static int visit(GraphType &g,
                      VertContainer& vTab,
				      typename GraphType::PVertex start,
				      EdgeDirection mask,
				      Visitor visit,
				      int compId)
        {
		return graphVisitBase<StackInterface<std::pair<std::pair<typename GraphType::PVertex, bool>, VisitVertLabs<GraphType> > * >,
				      GraphType,
				      VertContainer,
				      Visitor,
				      true>(g, vTab, start, mask, visit, compId);
		};
};


// TODO: czemu to dziala bez sprawdzania struktury grafu? Sprawdzic!
//class LexBFS: public GraphSearchBase<LexBFS> {
//public:
//
//    template<class Graph>
//    class LexVisitContainer {
//        private:
//            typedef std::list<typename Graph::PVertex> LexList;
//            typedef std::pair<LexList, typename LexList::iterator> Node;
//            typedef std::list<Node> NodeList;
//            typedef AssocArray<typename Graph::PVertex, std::pair<typename NodeList::iterator, typename LexList::iterator> > MapStruct;
//            NodeList m_sets;
//            typename std::list<typename NodeList::iterator> m_splits;
//            MapStruct m_vertexToListPos;
//        public:
//            LexVisitContainer(): m_sets(), m_splits(), m_vertexToListPos()	{};
//            ~LexVisitContainer()		{ clear(); };
//
//            void clear()			{ m_sets.clear(); m_splits.clear(); /*m_vertexToListPos.clear();*/ };
//            bool empty()			{ return m_sets.empty(); };
//
//
//            void initialize(Graph &g, typename Graph::PVertex first)
//            {
//                unsigned int i;
//                typename Graph::PVertex v;
//                typename NodeList::iterator np;
//
//                clear();
//                m_sets.push_back(Node());
//                np = --(m_sets.end());
//
//                np->first.push_back(first);
//                m_vertexToListPos[first] = make_pair(np, --(np->first.end()));
//
//                for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) {
//                    if(v == first) continue;
//                    np->first.push_back(v);
//                    m_vertexToListPos[v] = make_pair(np, --(np->first.end()));
//                    };
//                np->second = np->first.begin();
//            };
//
//
//            typename Graph::PVertex top()
//            {
//                return m_sets.front().first.front();
//            };
//
//
//            void pop()
//            {
//                while(!m_sets.empty() && m_sets.front().first.empty()) m_sets.pop_front();
//                if(m_sets.size() == 0) return;
//                m_sets.front().first.front();
//                m_sets.front().first.pop_front();
//                m_sets.front().second = m_sets.front().first.begin();
//                while(!m_sets.empty() && m_sets.front().first.empty()) m_sets.pop_front();
//            };
//
//
//            void push(typename Graph::PVertex v)
//            {
//                typename LexList::iterator vertListPos;
//                typename NodeList::iterator np, vertNode;
//
//                if(!m_vertexToListPos.hasKey(v))
//                {
//                    if(m_sets.empty()) {
//                        m_sets.push_back(Node());
//                        np = --(m_sets.end());
//                        np->first.push_back(v);
//                        np->second = np->first.begin();
//                        m_vertexToListPos[v] = std::make_pair(np, --(np->first.end()));
//                        return;
//                    };
//                    np = --(m_sets.end());
//                    np->first.push_back(v);
//                    m_vertexToListPos[v] = std::make_pair(np, --(np->first.end()));
//                };
//
//                vertNode = m_vertexToListPos[v].first;
//                vertListPos = m_vertexToListPos[v].second;
//
//                LexList &vertList = vertNode->first;
//                typename LexList::iterator &splitPos = vertNode->second;
//
//                if(splitPos == vertListPos) {
//                    m_splits.push_back(vertNode);
//                    ++splitPos;
//                } else {
//                    if(splitPos == vertList.begin()) m_splits.push_back(vertNode);
//                    vertList.splice(splitPos, vertList, vertListPos);
//                    };
//            };
//
//
//            void step() {
//                typename LexList::iterator lit, le;
//                typename NodeList::iterator newSet;
//                typename std::list<typename NodeList::iterator>::iterator e, it;
//
//                e = m_splits.end();
//                it = m_splits.begin();
//                while(it != e) {
//                    if((*it)->second != (*it)->first.begin() &&
//                       (*it)->second != (*it)->first.end()) {
//                        newSet = m_sets.insert(*it, Node());
//                        newSet->first.splice(newSet->first.begin(), (*it)->first,
//                                     (*it)->first.begin(), (*it)->second);
//                        newSet->second = newSet->first.begin();
//
//                        le = newSet->first.end();
//                        for(lit = newSet->first.begin(); lit != le; ++lit)
//                            m_vertexToListPos[*lit] = std::make_pair(newSet, lit);
//                        };
//                    ++it;
//                    };
//
//                m_splits.clear();
//                };
//
//
//    //		void dump() {
//    //			typename NodeList::iterator it, e;
//    //			typename LexList::iterator lit, le, split;
//    //			e = m_sets.end();
//    //			for(it = m_sets.begin(); it != e; ++it) {
//    //				split = it->second;
//    //				printf("<");
//    //				le = it->first.end();
//    //				for(lit = it->first.begin(); lit != le; ++lit) {
//    //					if(lit == split) printf(" |");
//    //					printf(" %u", ((unsigned int)(*lit) & 0xffff) / 8);
//    //					};
//    //				printf(" >  ");
//    //				};
//    //			printf("\n");
//    //			fflush(stdout);
//    //			};
//
//    };
//
//	template<class GraphType,
//            class VertContainer,
//            class Visitor>
//	static int visit(GraphType &g,
//                      VertContainer& vTab,
//				      typename GraphType::PVertex start,
//				      EdgeDirection mask,
//				      Visitor visit,
//				      int compId)
//        {
//            int depth, n, retVal;
//            typename GraphType::PEdge e;
//            typename GraphType::PVertex u, v;
//            LexVisitContainer<GraphType> cont;
//
//            mask&=(~EdLoop);
//            n = g.getVertNo();
//            if(n == 0) return 0;
//            retVal = 0;
//            if(start == NULL) start = g.getVert();
//
//            vTab[start] = VisitVertLabs<GraphType>(NULL, NULL, 0,compId);
//            cont.push(start);
//
//
//            while(!cont.empty()) {
//                u = cont.top();
//                depth = vTab[u].distance;
//
//                if (!visit(g, u, vTab[u])) return -1;
//                retVal++;
//                cont.pop();
//
//                // for each neighbour
//                for(e = g.getEdge(u, mask); e != NULL; e = g.getEdgeNext(u, e, mask)) {
//                    v = g.getEdgeEnd(e, u);
//                    if(vTab.hasKey(v)) continue;
//                    vTab[v] = VisitVertLabs<GraphType>(u,e,depth+1,compId);
//                    cont.push(v);
//                };
//                cont.step();
//
//            }; // while(!cont.empty())
//
//            return retVal;
//
//        }
//
//};


}

#endif

