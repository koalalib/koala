#ifndef KOALA_SUBGRAPH_H
#define KOALA_SUBGRAPH_H

#include "graph.h"

namespace Koala {

template <class Graph, class VChooser, class EChooser>
class Subgraph : public SubgraphBase {
    public:
    VChooser vchoose;
    EChooser echoose;

	typedef typename Graph::Vertex Vertex;
	typedef typename Graph::PVertex PVertex;
	typedef typename Graph::Edge Edge;
	typedef typename Graph::PEdge PEdge;
	typedef Subgraph<Graph,VChooser,EChooser> GraphType;
	typedef typename Graph::RootGrType RootGrType;
	typedef Graph ParentGrType;
	typedef typename Graph::VertInfoType VertInfoType;
	typedef typename Graph::EdgeInfoType EdgeInfoType;

    Subgraph() {}
	Subgraph(Graph& g, std::pair<VChooser,EChooser> chs=std::make_pair(VChooser(),EChooser())) :
            vchoose(chs.first), echoose(chs.second), SubgraphBase()
            { SubgraphBase::link(&g); }
	Subgraph(std::pair<VChooser,EChooser> chs) : vchoose(chs.first), echoose(chs.second), SubgraphBase()
            { }


	void plug(Graph& g) { SubgraphBase::link(&g); }
	bool unplug() { return SubgraphBase::unlink(); }
	void setChoose(const std::pair<VChooser,EChooser>& chs) { vchoose=chs.first; echoose=chs.second; }
	RootGrType* getRootPtr() { return parent ? ((ParentGrType*)parent)->getRootPtr() : 0; }
	ParentGrType* getParentPtr() { return (ParentGrType*)parent; }
    ParentGrType& up() { ParentGrType* res=getParentPtr(); assert(res); return *res; }
    RootGrType& root() { RootGrType* res=getRootPtr(); assert(res); return *res; }
    bool good(PVertex vert,bool deep=false)
    {   if (!vert) return true;
        return vchoose(vert,up()) && (!deep || up().good(vert,true));
    }
    bool good(PEdge edge,bool deep=false)
    {   if (!edge) return true;
        std::pair<PVertex,PVertex> ends=edge->getEnds();
        return vchoose(ends.first,up()) && vchoose(ends.second,up()) && echoose(edge,up())
                && (!deep || up().good(edge,true));
    }


    PVertex getVert() { return getVertNext(0); }
    PVertex getVertLast() { return getVertPrev(0); }
    PVertex getVertNext(PVertex v)
    {   do v=up().getVertNext(v);
            while (v && !vchoose(v,up()));
        return v;
    }
    PVertex getVertPrev(PVertex v)
    {   do v=up().getVertPrev(v);
            while (v && !vchoose(v,up()));
        return v;
    }
    template<class OutputIterator>
			int getVerts(OutputIterator iter)
			{   typename Graph::PVertex LOCALARRAY(buf,root().getVertNo());
                int a=0,res=0;
                a=up().getVerts(buf);
                for(int i=0;i<a;i++)
                    if (good(buf[i],false)) { *iter=buf[i]; ++iter; ++res; }
			    return res;
			}
    Set<PVertex> getVertSet()
    {   Set<typename Graph::PVertex> res;
        getVerts(setInserter(res));
        return res;
    }
    int getVertNo() { return getVerts(blackHole()); }
    VertInfoType getVertInfo(PVertex v) { return v->getInfo(); }


    EdgeInfoType getEdgeInfo(PEdge e) { return e->getInfo(); }
	EdgeType getEdgeType(PEdge e) { return e->getType(); }
	EdgeType getType(PEdge e) { return e->getType(); }
    std::pair<typename Graph::PVertex,typename Graph::PVertex> getEdgeEnds(PEdge edge) { return edge->getEnds(); }
    std::pair<typename Graph::PVertex,typename Graph::PVertex> getEnds(PEdge edge) { return edge->getEnds(); }
    bool isEdgeEnd(PEdge edge, PVertex vert) { return root().isEnd(edge,vert); }
    bool isEnd(PEdge edge, PVertex vert) { return root().isEnd(edge,vert); }
    typename Graph::PVertex getEdgeEnd(PEdge edge, PVertex vert) { return root().getEdgeEnd(edge,vert); }
    typename Graph::PVertex getEnd(PEdge edge,PVertex vert) { return root().getEdgeEnd(edge,vert); }
    EdgeDirection getEdgeDir(PEdge edge,PVertex vert) { return root().getEdgeDir(edge,vert); }


    PEdge getEdge(EdgeDirection mask=EdAll) { return getEdgeNext(0,mask); }
    PEdge getEdgeLast(EdgeDirection mask=EdAll) { return getEdgePrev(0,mask); }
    PEdge getEdgeNext(PEdge e,EdgeDirection mask=EdAll)
    {   do e=up().getEdgeNext(e,mask);
            while (e && !(vchoose(e->getEnds().first,up()) && vchoose(e->getEnds().second,up())
                          && echoose(e,up())));
        return e;
    }
    PEdge getEdgePrev(PEdge e,EdgeDirection mask=EdAll)
    {   do e=up().getEdgePrev(e,mask);
            while (e && !(vchoose(e->getEnds().first,up()) && vchoose(e->getEnds().second,up())
                          && echoose(e,up())));
        return e;
    }
    template<class OutputIterator>
			int getEdges(OutputIterator iter,EdgeDirection mask=EdAll)
			{   typename Graph::PEdge LOCALARRAY(buf,root().getEdgeNo(mask));
                int a=0,res=0;
                a=up().getEdges(buf,mask);
                for(int i=0;i<a;i++)
                    if (good(buf[i],false)) { *iter=buf[i]; ++iter; ++res; }
			    return res;
			}
    Set<PEdge> getEdgeSet(EdgeDirection mask=EdAll)
    {   Set<typename Graph::PEdge> res;
        getEdges(setInserter(res),mask);
        return res;
    }
    int getEdgeNo(EdgeDirection mask=EdAll) { return getEdges(blackHole(),mask); }


	PEdge getEdge(PVertex vert, EdgeDirection mask=EdAll) { return getEdgeNext(vert,NULL,mask); }
	PEdge getEdgeLast(PVertex vert, EdgeDirection mask=EdAll) { return getEdgePrev(vert,NULL,mask); }
	PEdge getEdgeNext(PVertex vert, PEdge e, EdgeDirection mask=EdAll)
    {   do e=up().getEdgeNext(vert,e,mask);
            while (e && !(vchoose(e->getEnds().first,up()) && vchoose(e->getEnds().second,up())
                          && echoose(e,up())));
        return e;
    }
	PEdge getEdgePrev(PVertex vert, PEdge e, EdgeDirection mask=EdAll)
    {   do e=up().getEdgePrev(vert,e,mask);
            while (e && !(vchoose(e->getEnds().first,up()) && vchoose(e->getEnds().second,up())
                          && echoose(e,up())));
        return e;
    }
	Set<PEdge> getEdgeSet(PVertex vert, EdgeDirection mask=EdAll)
    {   Set<typename Graph::PEdge> res;
        getEdges(setInserter(res),vert,mask);
        return res;
    }
	template<class OutputIterator>
			int getEdges(OutputIterator iter, PVertex vert, EdgeDirection mask=EdAll)
			{   typename Graph::PEdge LOCALARRAY(buf,root().getEdgeNo(vert,mask));
                int a=0,res=0;
                a=up().getEdges(buf,vert,mask);
                for(int i=0;i<a;i++)
                    if (good(buf[i],false)) { *iter=buf[i]; ++iter; ++res; }
			    return res;
			}
	int getEdgeNo(PVertex vert, EdgeDirection mask=EdAll) { return getEdges(blackHole(),vert,mask); }


	PEdge getEdge(PVertex vert1,PVertex vert2, EdgeDirection mask=EdAll) { return getEdgeNext(vert1,vert2,NULL,mask); }
	PEdge getEdgeLast(PVertex vert1,PVertex vert2,EdgeDirection mask=EdAll) { return getEdgePrev(vert1,vert2,NULL,mask); }
	PEdge getEdgeNext(PVertex vert1,PVertex vert2, PEdge e, EdgeDirection mask=EdAll)
    {   do e=up().getEdgeNext(vert1,vert2,e,mask);
            while (e && !(vchoose(e->getEnds().first,up()) && vchoose(e->getEnds().second,up())
                          && echoose(e,up())));
        return e;
    }
	PEdge getEdgePrev(PVertex vert1,PVertex vert2, PEdge e, EdgeDirection mask=EdAll)
    {   do e=up().getEdgePrev(vert1,vert2,e,mask);
            while (e && !(vchoose(e->getEnds().first,up()) && vchoose(e->getEnds().second,up())
                          && echoose(e,up())));
        return e;
    }
	Set<PEdge> getEdgeSet(PVertex vert1,PVertex vert2, EdgeDirection mask=EdAll)
    {   Set<typename Graph::PEdge> res;
        getEdges(setInserter(res),vert1,vert2,mask);
        return res;
    }
	template<class OutputIterator>
			int getEdges(OutputIterator iter, PVertex vert1, PVertex vert2,EdgeDirection mask=EdAll)
			{   typename Graph::PEdge LOCALARRAY(buf,root().getEdgeNo(vert1,mask));
                int a=0,res=0;
                a=up().getEdges(buf,vert1,vert2,mask);
                for(int i=0;i<a;i++)
                    if (good(buf[i],false)) { *iter=buf[i]; ++iter; ++res; }
			    return res;
			}
	int getEdgeNo(PVertex vert1,PVertex vert2, EdgeDirection mask=EdAll) { return getEdges(blackHole(),vert1,vert2,mask); }


	template<class OutputIterator>
			int getNeigh(OutputIterator out, PVertex vert,EdgeDirection direct=EdAll)
			{   typename Graph::PVertex LOCALARRAY(ans,root().getEdgeNo(vert, direct));
                int size=0,res=0;
                PEdge edge = getEdge(vert, direct);
                while(edge) {
                    ans[size++]=getEdgeEnd(edge,vert);
                    edge = getEdgeNext(vert, edge, direct);
                }
                std::make_heap(ans,ans+size); std::sort_heap(ans,ans+size);
                for(int i=0;i<size;i++) if (i==0 || ans[i-1]!=ans[i])
                {
                    *out=ans[i];
                    ++out; ++res;
                }
                return res;
			}
    Set<PVertex> getNeighSet(PVertex vert,EdgeDirection mask=EdAll)
    {   Set<typename Graph::PVertex> res;
        getNeigh(setInserter(res),vert,mask);
        return res;
    }
    int getNeighNo(PVertex vert, EdgeDirection mask=EdAll) { return getNeigh(blackHole(),vert,mask); }


    Set<typename Graph::PVertex> getClNeighSet(PVertex vert,EdgeDirection direct=EdAll)
    {
        Set<typename Graph::PVertex> ans = getNeighSet(vert, direct);
        ans.add(vert);
        return ans;
    }
    template<class OutputIterator>
    int getClNeigh(OutputIterator out, PVertex vert, EdgeDirection direct=EdAll)
    {
      typename Graph::PVertex LOCALARRAY(ans,root().getEdgeNo(vert, direct)+1);
        ans[0]=vert;
        int size=1,res=0;
        PEdge edge = getEdge(vert, direct);
        while(edge)
        {
                ans[size++]=getEdgeEnd(edge,vert);
                edge = getEdgeNext(vert, edge, direct);
        }
        std::make_heap(ans,ans+size); std::sort_heap(ans,ans+size);
        for(int i=0;i<size;i++) if (i==0 || ans[i-1]!=ans[i])
        {
            *out=ans[i];
            ++out; ++res;
        }
        return res;
    }
    int getClNeighNo(PVertex vert, EdgeDirection direct=EdAll) {  return getClNeigh(blackHole(),vert,direct); }


   	PVertex vertByNo(int idx)
   	{   PVertex tmp_vert = getVert();
        while(idx && tmp_vert) {
            tmp_vert = getVertNext(tmp_vert);
            --idx;
        }
        return tmp_vert;
    }
   	PEdge edgeByNo(int idx)
    {   PEdge tmp_edge = getEdge();
        while(idx && tmp_edge) {
            tmp_edge = getEdgeNext(tmp_edge);
            --idx;
        }
        return tmp_edge;
    }
	int vertPos(PVertex vert)
    {   int idx=0;
        PVertex tmp_vert = getVert();
        while(tmp_vert && tmp_vert!=vert) {
            tmp_vert = getVertNext(tmp_vert);
            ++idx;
        }
        return tmp_vert ? idx : -1;
    }
	int edgePos(PEdge edge)
	{   int idx=0;
        PEdge tmp_edge = getEdge();
        while(tmp_edge && tmp_edge!=edge) {
            tmp_edge = getEdgeNext(tmp_edge);
            ++idx;
        }
        return tmp_edge ? idx : -1;
    }


    int deg(PVertex vert,EdgeDirection direct=EdAll)
    { return getEdgeNo(vert,direct) + ((direct&EdLoop) ? getEdgeNo(vert,EdLoop): 0); }
    std::pair<typename Graph::PVertex,int> minDeg(EdgeDirection direct=EdAll)
    {
        std::pair<typename Graph::PVertex,int> res(0,-1);
        if (!getVertNo()) return res;
            else { res.first=getVert(); res.second=deg(res.first,direct); }
        int degree;
        for(typename Graph::PVertex vert=getVertNext(res.first);vert;vert=getVertNext(vert))
            if ((degree=deg(vert,direct))<res.second) res=std::make_pair(vert,degree);
        return res;
    }
    std::pair<typename Graph::PVertex,int> maxDeg(EdgeDirection direct=EdAll)
    {
        std::pair<typename Graph::PVertex,int> res(0,-1);
        if (!getVertNo()) return res;
            else { res.first=getVert(); res.second=deg(res.first,direct); }
        int degree;
        for(typename Graph::PVertex vert=getVertNext(res.first);vert;vert=getVertNext(vert))
            if ((degree=deg(vert,direct))>res.second) res=std::make_pair(vert,degree);
        return res;
    }
    int Delta(EdgeDirection direct=EdAll)
    {
        return std::max(0,maxDeg(direct).second);
    }
    int delta(EdgeDirection direct=EdAll)
    {
        return std::max(0,minDeg(direct).second);
    }

    bool areParallel(PEdge e1,PEdge e2,EdgeDirection reltype=EdUndir)
    {   if (!e1 || !e2 || !(reltype==EdDirIn||reltype==EdDirOut||reltype==EdUndir)) return false;
        std::pair<typename Graph::PVertex,typename Graph::PVertex>
                            ends1=getEdgeEnds(e1),ends2=getEdgeEnds(e2);
        if (e1==e2) return true;
        else if (e1->getType()==Loop) return e2->getType()==Loop && ends1.first==ends2.first;
        else if (e2->getType()==Loop) return false;
        else if (   (e1->getType()==Undirected && e2->getType()==Undirected)  ||
                    (e1->getType()!=e2->getType() && reltype==EdUndir)    ||
                    (e1->getType()==Directed && e2->getType()==Directed && (reltype==EdUndir||reltype==EdDirIn))
                )
            return  (ends1.first==ends2.first && ends1.second==ends2.second)
                        ||(ends1.second==ends2.first && ends1.first==ends2.second);
        else return e1->getType()==e2->getType() && ends1.first==ends2.first && ends1.second==ends2.second;
    }

};


template <class Graph, class VChooser, class EChooser>
Subgraph<Graph,VChooser,EChooser> makeSubgraph(Graph& g,const std::pair<VChooser,EChooser>& chs)
{   return Subgraph<Graph,VChooser,EChooser>(g,chs); }


}

#endif
