#ifndef DEF_FLOWS_H
#define DEF_FLOWS_H

#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <limits>
#include "../base/def_struct.h"
//#include "../graph/graph.h"
#include "../graph/subgraph.h"
#include "../algorithm/search.h"



namespace Koala {



class FlowAlgsDefaultStructs : public AlgorithmsDefaultSettings {
    public:

    static bool useFulkersonFord() { return false; };
    static bool useCostFulkersonFord() { return true; };
};


template <class DefaultStructs>
class FlowPar : public PathStructs {

    public:

    template <class DType, class CType=DType> struct EdgeLabs {

        typedef DType CapacType;
        typedef CType CostType;

        CapacType capac,flow;
        CostType cost;

        EdgeLabs(CapacType arg=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                 CostType arg2=DefaultStructs:: template NumberTypeBounds<CostType>::zero()) :
                                capac(arg), cost(arg2),
                                flow(DefaultStructs:: template NumberTypeBounds<CapacType>::zero())
            {}
    };


    template <class CapacType> struct EdgeCut {

            CapacType capac;
            int vertNo,edgeNo;

            EdgeCut() : capac(DefaultStructs:: template NumberTypeBounds<CapacType>::zero()),
                        vertNo(0), edgeNo(0)
            {}
    };


    template <class GraphType, class CapacType> struct EdgeCut2 {

            CapacType capac;
            typename GraphType::PVertex first, second;
            int vertNo,edgeNo;

            EdgeCut2() : capac(DefaultStructs:: template NumberTypeBounds<CapacType>::zero()),
                         vertNo(0), edgeNo(0)
            {}
    };


//    protected:
    public:


    template <class GraphType,class CapacType> struct VertLabs {

        int distance,backdist;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;
        CapacType mass, inPot, outPot;
        bool used;

        VertLabs(typename GraphType::PVertex pv=0,typename GraphType::PEdge pe=0,
                 int d=std::numeric_limits<int>::max(), int bd=std::numeric_limits<int>::max())
            : vPrev(pv), ePrev(pe), distance(d), backdist(bd),
            mass(DefaultStructs:: template NumberTypeBounds<CapacType>::zero()),
            inPot(DefaultStructs:: template NumberTypeBounds<CapacType>::plusInfty()),
            outPot(DefaultStructs:: template NumberTypeBounds<CapacType>::plusInfty()),
            used(false)
             {}
    };


    protected:


    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    usedCap (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PEdge e,
        typename GraphType::PVertex v, bool out)
    {
        EdgeDirection dir=g.getEdgeDir(e,v);
        assert(dir!=EdNone && dir!=EdLoop);
        if (out)
            switch (dir)
            {
                case EdDirOut   : return edgeTab[e].capac-edgeTab[e].flow;
                case EdDirIn    : return edgeTab[e].flow;
                default         : if (g.getEdgeEnd1(e)==v) return edgeTab[e].capac-edgeTab[e].flow;
                                    else return edgeTab[e].capac+edgeTab[e].flow;
            }
        else return usedCap(g,edgeTab,e,g.getEdgeEnd(e,v),true);
    }


    template <class GraphType, class EdgeContainer>
    static void
    addFlow (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PEdge e,
        typename GraphType::PVertex v,
        typename EdgeContainer::ValType::CapacType delta, bool out)
    {
        EdgeDirection dir=g.getEdgeDir(e,v);
        assert(dir!=EdNone && dir!=EdLoop);
        if (out)
            switch (dir)
            {
                case EdDirOut   : edgeTab[e].flow+=delta; assert(edgeTab[e].flow<=edgeTab[e].capac);
                                  break;
                case EdDirIn    : edgeTab[e].flow-=delta; assert(edgeTab[e].flow>=
                                                DefaultStructs:: template NumberTypeBounds
                                                <typename EdgeContainer::ValType::CapacType>::zero());
                                  break;
                default         : if (g.getEdgeEnd1(e)==v) edgeTab[e].flow+=delta;
                                    else edgeTab[e].flow-=delta;
                                    assert(edgeTab[e].flow
                                           <=edgeTab[e].capac && edgeTab[e].flow>=-edgeTab[e].capac);
                                  break;
            }
        else addFlow(g,edgeTab,e,g.getEdgeEnd(e,v),delta,true);
    }


    template <class GraphType, class VertContainer, class EdgeContainer,class Iter>
    static bool BFSFlow(
    const GraphType &g, EdgeContainer &edgeTab, VertContainer &visited,
        typename GraphType::PVertex first, typename GraphType::PVertex last,
        bool out,Iter& iter)
    {
        EdgeDirection mask=EdDirIn | EdDirOut | EdUndir;
        int depth;
        typename GraphType::PEdge e;
        typename GraphType::PVertex u, v;
        typename GraphType::PVertex LOCALARRAY( buf,g.getEdgeNo() + 3 );
        QueueInterface< typename GraphType::PVertex* > cont( buf,g.getEdgeNo() + 3 );

//        visited[first]=(VertLabs<GraphType,typename EdgeContainer::ValType::CapacType>
//                                                    (NULL,NULL,0);

        visited[first].vPrev=0;visited[first].ePrev=0;
        if (out) visited[first].distance=0; else visited[first].backdist=0;

        cont.push(first);

        while (!cont.empty())
        {
            u = cont.top();
            depth = (out) ? visited[u].distance : visited[u].backdist;
            cont.pop();

            for( e = g.getEdge( u,mask ); e != NULL; e = g.getEdgeNext( u,e,mask ))
            if (!DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::CapacType>
                        ::isZero(usedCap(g,edgeTab,e,u,out)))
            {
                v = g.getEdgeEnd( e,u );
                if (!DefaultStructs:: template NumberTypeBounds<int>::isPlusInfty(
                                            (out) ? visited[v].distance : visited[v].backdist))
                    continue;

//                visited[v] = VertLabs< GraphType,typename EdgeContainer::ValType::CapacType >
//                                                                        ( u,e,depth + 1 );
                visited[v].vPrev=u;visited[v].ePrev=e;
                if (out) visited[v].distance=depth+1; else visited[v].backdist=depth+1;


                cont.push( v );
            }
            *iter=u; ++iter;
            if (u==last) return true;
        }
        return false;
    }


    template <class GraphType, class VertContainer, class EdgeContainer, class Iter>
    static bool layers(
         const GraphType &g, EdgeContainer &edgeTab, VertContainer &visited,
         typename GraphType::PVertex first, typename GraphType::PVertex last,
         Iter& iterout)
    {   int res;
        typename GraphType::PVertex *iter;
        typename GraphType::PVertex  LOCALARRAY(buf,g.getVertNo());
        iter=buf;
        if (!BFSFlow(g,edgeTab,visited,last,first,false,blackHole)) return false;
        BFSFlow(g,edgeTab,visited,first,last,true,iter);
        res=visited[last].distance;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if (visited[v].distance<=res && visited[v].backdist<=res
                && visited[v].distance+visited[v].backdist==res)
                visited[v].used=true;
            else visited[v].used=false;
        for(typename GraphType::PVertex* i=buf;i<iter;i++)
            if (visited[*i].used) { *iterout=*i; ++iterout; }
        return true;
    }


    template <class GraphType, class VertContainer, class EdgeContainer>
    static void findPot(
            const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab,
            std::pair<typename GraphType::PVertex,typename GraphType::PVertex> ends,
            typename GraphType::PVertex v,bool pin, bool pout)
    {   assert(vertTab[v].used);
        const typename EdgeContainer::ValType::CapacType PlusInfty
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty();
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();

        EdgeDirection mask=EdDirIn | EdDirOut | EdUndir;
        typename EdgeContainer::ValType::CapacType d;
        typename GraphType::PVertex u;

        if (pin)
        {   vertTab[v].inPot=(v==ends.first) ? PlusInfty : Zero;
            if (v!=ends.first)
            for(typename GraphType::PEdge e=g.getEdge(v,mask);e;e=g.getEdgeNext(v,e,mask))
                if (vertTab[u=g.getEdgeEnd(e,v)].used && vertTab[u].distance==vertTab[v].distance-1 &&
                (Zero!=(d=usedCap(g,edgeTab,e,v,false))))
                vertTab[v].inPot+=d;
        }
        if (pout)
        {   vertTab[v].outPot=(v==ends.second) ? PlusInfty : Zero;
            if (v!=ends.second)
            for(typename GraphType::PEdge e=g.getEdge(v,mask);e;e=g.getEdgeNext(v,e,mask))
                if (vertTab[u=g.getEdgeEnd(e,v)].used && vertTab[u].distance==vertTab[v].distance+1 &&
                (Zero!=(d=usedCap(g,edgeTab,e,v,true))))
                vertTab[v].outPot+=d;
        }
    }


    template <class GraphType, class VertContainer, class EdgeContainer>
    static void push(
    const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab,
        typename GraphType::PVertex v,bool front)
    {
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
        const EdgeDirection mask=EdDirIn | EdDirOut | EdUndir;

        typename EdgeContainer::ValType::CapacType d;
        typename GraphType::PVertex u;

        if (Zero==vertTab[v].mass) return;
        for(typename GraphType::PEdge e=g.getEdge(v,mask);e;e=g.getEdgeNext(v,e,mask))
        if (vertTab[u=g.getEdgeEnd(e,v)].used && vertTab[u].distance
                                                ==vertTab[v].distance+((front) ? 1 : -1))
        {   if (Zero==vertTab[v].mass) break;
            d=std::min(usedCap(g,edgeTab,e,v,front),vertTab[v].mass);
            if (Zero==d) continue;

            vertTab[v].mass-=d;vertTab[u].mass+=d;

            if (front) vertTab[v].outPot-=d; else vertTab[v].inPot-=d;
            if (front) vertTab[u].inPot-=d; else vertTab[u].outPot-=d;
            addFlow(g,edgeTab,e,v,d,front);
        }
       assert(Zero==vertTab[v].mass);


    }


    template<class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    onevert( const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab,
        typename GraphType::PVertex *tab,int size,
        typename EdgeContainer::ValType::CapacType limit)
    {
        const typename EdgeContainer::ValType::CapacType PlusInfty
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty();
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
        const EdgeDirection mask=EdDirIn | EdDirOut | EdUndir;

        typename EdgeContainer::ValType::CapacType minpot=PlusInfty;
        typename GraphType::PVertex u;
        int minpos=-1;

        for(int i=0;i<size;i++)
        if (vertTab[tab[i]].used)
        {   vertTab[tab[i]].mass=Zero;
            if (minpot>std::min(vertTab[tab[i]].inPot,vertTab[tab[i]].outPot))
            {   minpot=std::min(vertTab[tab[i]].inPot,vertTab[tab[i]].outPot); minpos=i; }
        }
        assert(minpos>=0 && minpos<size);
        if (minpot>Zero)
        {   minpot=std::min(minpot,limit);
            vertTab[tab[minpos]].mass=minpot;
            for(int i=minpos;i<size-1;i++) if (vertTab[tab[i]].used) push(g,edgeTab,vertTab,tab[i],true);
            vertTab[tab[minpos]].mass=minpot;
            for(int i=minpos;i>0;i--) if (vertTab[tab[i]].used) push(g,edgeTab,vertTab,tab[i],false);
        }
        vertTab[tab[minpos]].used=false;
        for(typename GraphType::PEdge e=g.getEdge(tab[minpos],mask);e;e=g.getEdgeNext(tab[minpos],e,mask))
        if (vertTab[u=g.getEdgeEnd(e,tab[minpos])].used)
        { if (vertTab[u].distance==vertTab[tab[minpos]].distance+1)
                    findPot(g,edgeTab,vertTab,std::make_pair(tab[0],tab[size-1]),u,true,false);
          if (vertTab[u].distance==vertTab[tab[minpos]].distance-1)
                    findPot(g,edgeTab,vertTab,std::make_pair(tab[0],tab[size-1]),u,false,true);
        }
        return minpot;
    }


    template <class GraphType, class EdgeContainer,class VertContainer>
    static typename EdgeContainer::ValType::CapacType
    layerFlow (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        VertContainer& vertTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit)
    {   assert(start);
        assert(end);
        assert(start!=end);
        typename GraphType::PVertex LOCALARRAY(buf,g.getVertNo());
        typename GraphType::PVertex* bufend=buf;
        typename EdgeContainer::ValType::CapacType res=
                        DefaultStructs:: template NumberTypeBounds
                        <typename EdgeContainer::ValType::CapacType>::zero();

        vertTab.clear();
        if (!layers(g,edgeTab,vertTab,start,end,bufend)) return res;
        int size=bufend-buf;
        for(int i=0;i<size;i++) if (vertTab[buf[i]].used)
            findPot(g,edgeTab,vertTab,std::make_pair(start,end),buf[i],true,true);
        do
        {   typename EdgeContainer::ValType::CapacType x=onevert(g,edgeTab,vertTab,buf,size,limit);
            res+=x;limit-=x;
        }
        while (vertTab[start].used && vertTab[end].used && limit>DefaultStructs:: template NumberTypeBounds
                        <typename EdgeContainer::ValType::CapacType>::zero());

        return res;
    }


    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    maxFlowMKM (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit)
    {   assert(start);
        assert(end);
        assert(start!=end);
        const typename EdgeContainer::ValType::CapacType PlusInfty
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty();
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();

        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<GraphType,typename EdgeContainer::ValType::CapacType> >::Type vertTab(g.getVertNo());

        typename EdgeContainer::ValType::CapacType res,add;
        res=Zero;

        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        {
            edgeTab[e].flow=Zero; assert(edgeTab[e].capac>=Zero);
        }
        if (limit==Zero) return Zero;
        while (Zero!=(add=layerFlow(g,edgeTab,vertTab,start,end,limit-res)))
        {   res+=add;
            if (limit==res) break;
        }
        return res;
    }


    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    maxFlowFF (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit)
    {   assert(start);
        assert(end);
        assert(start!=end);
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();


        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<GraphType,typename EdgeContainer::ValType::CapacType> >::Type vertTab(g.getVertNo());
        typename EdgeContainer::ValType::CapacType res=Zero;
        typename GraphType::PVertex LOCALARRAY(vTab,g.getVertNo());
        typename GraphType::PEdge LOCALARRAY(eTab,g.getVertNo());

        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        {
            edgeTab[e].flow=Zero;
            assert(edgeTab[e].capac>=Zero);
        }

        while (res<limit && BFSFlow(g,edgeTab,vertTab,start,end,true,blackHole))
        {   ShortPathStructs::getOutPath( g,vertTab,outPath(vTab,eTab),end );
            typename EdgeContainer::ValType::CapacType delta=limit-res;
            for(int i=0;i<vertTab[end].distance;i++)
                              delta=std::min(delta,usedCap(g,edgeTab,eTab[i],vTab[i],true));
            for(int i=0;i<vertTab[end].distance;i++) addFlow(g,edgeTab,eTab[i],vTab[i],delta,true);
            res+=delta;
            vertTab.clear();
        }
        return res;
    }


    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    usedCapCost (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PEdge e,typename GraphType::PVertex v)
    {
        EdgeDirection dir=g.getEdgeDir(e,v);
        assert(dir!=EdNone && dir!=EdLoop);
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();

        switch (dir)
        {
            case EdDirOut   : return edgeTab[e].capac-edgeTab[e].flow;
            case EdDirIn    : return edgeTab[e].flow;
            default         : if (g.getEdgeEnd1(e)==v)
                            {   if (edgeTab[e].flow>Zero ) return edgeTab[e].capac-edgeTab[e].flow;
                                else if (edgeTab[e].flow<Zero ) return -edgeTab[e].flow;
                                else return edgeTab[e].capac;
                            }
                              else
                            {   if (edgeTab[e].flow>Zero ) return edgeTab[e].flow;
                                else if (edgeTab[e].flow<Zero ) return edgeTab[e].capac+edgeTab[e].flow;
                                else return edgeTab[e].capac;
                            }
        }
    }


    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CostType
    costFlow (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PEdge e,typename GraphType::PVertex v)
    {
        EdgeDirection dir=g.getEdgeDir(e,v);
        assert(dir!=EdNone && dir!=EdLoop);
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();

        switch (dir)
        {
            case EdDirOut   : return edgeTab[e].cost;
            case EdDirIn    : return -edgeTab[e].cost;
            default         : if (g.getEdgeEnd1(e)==v)
                                    if (edgeTab[e].flow>=Zero ) return edgeTab[e].cost;
                                    else return -edgeTab[e].cost;
                              else
                                    if (edgeTab[e].flow>Zero ) return -edgeTab[e].cost;
                                    else return edgeTab[e].cost;
        }
    }


    template <class GraphType,class CostType> struct VertLabsCost {

        CostType distance,pot;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        VertLabsCost() : vPrev(0), ePrev(0),
                    distance(DefaultStructs:: template NumberTypeBounds<CostType>::plusInfty()),
                    pot(DefaultStructs:: template NumberTypeBounds<CostType>::zero()) {}
    };


    template <class GraphType, class EdgeContainer, class VertContainer>
    static typename EdgeContainer::ValType::CostType
    costFlowDij (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        VertContainer& vertTab, // i tablica dlugosci krawedzi
        typename GraphType::PEdge e,typename GraphType::PVertex v)
    {   return costFlow(g,edgeTab,e,v)+vertTab[v].pot-vertTab[g.getEdgeEnd(e,v)].pot; }


    template <class GraphType, class VertContainer, class EdgeContainer>
    static bool DijkstraFlow (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        VertContainer& vertTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end)
    {   assert(start && end && start!=end);
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabsCost<GraphType, typename EdgeContainer::ValType::CostType> >::Type Q(g.getVertNo());
        typename GraphType::PVertex U,V;

        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            vertTab[v].distance=DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::CostType> ::plusInfty();
        Q[start].vPrev=0;Q[start].ePrev=0;
        Q[start].distance=DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::CostType> ::zero();
       while(!Q.empty()){
            typename EdgeContainer::ValType::CostType
                    d=DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::CostType> ::plusInfty(),nd;
            for(V=Q.firstKey();V;V=Q.nextKey(V)) if (d>Q[V].distance) d=Q[U=V].distance;
            {   vertTab[U].distance=Q[U].distance;
                vertTab[U].ePrev=Q[U].ePrev;
                vertTab[U].vPrev=Q[U].vPrev;
                Q.delKey(U);
            }
            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdUndir|Koala::EdDirIn|Koala::EdDirOut);
                E;E=g.getEdgeNext(U,E,Koala::EdUndir|Koala::EdDirIn|Koala::EdDirOut))
                if (usedCapCost(g,edgeTab,E,U)>DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::CapacType> ::zero())
                    if (vertTab[V=g.getEdgeEnd(E,U)].distance==DefaultStructs:: template NumberTypeBounds
                                    <typename EdgeContainer::ValType::CostType> ::plusInfty())
                        if ((nd=vertTab[U].distance+costFlowDij(g,edgeTab,vertTab,E,U))<Q[V].distance)
                    { Q[V].distance=nd; Q[V].ePrev=E; Q[V].vPrev=U; }
        }
        bool res=DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::CostType> ::plusInfty()>vertTab[end].distance;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            vertTab[v].distance+=vertTab[v].pot-vertTab[start].pot;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            vertTab[v].pot=vertTab[v].distance;
        return res;
    }


    template <class GraphType,class CostType> struct CycLabs {

        CostType dist;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        CycLabs() : vPrev(0), ePrev(0),
                    dist(DefaultStructs:: template NumberTypeBounds<CostType>::plusInfty())
             {}
    };


    template <class GraphType, class EdgeContainer, class EIter, class VIter>
    static int minMeanCycle (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        OutPath<VIter,EIter> iters)
    {
        const typename EdgeContainer::ValType::CostType PlusInfty
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::plusInfty();
        const typename EdgeContainer::ValType::CostType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::zero();

        CycLabs< GraphType, typename EdgeContainer::ValType::CostType> LOCALARRAY(
                                                            buf,g.getVertNo()*g.getVertNo()+g.getVertNo());
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                std::pair<CycLabs< GraphType, typename EdgeContainer::ValType::CostType>*,int> >
                    ::Type vTab(g.getVertNo());

        typename GraphType::PVertex LOCALARRAY(vBuf,g.getVertNo()+1);
        typename GraphType::PEdge LOCALARRAY(eBuf,g.getVertNo()+1);
        int n=g.getVertNo();
        // TODO: chyba zbedne:
        for(int i=0;i<n*n+n;i++) buf[i]=CycLabs< GraphType, typename EdgeContainer::ValType::CostType>();

        CycLabs< GraphType, typename EdgeContainer::ValType::CostType> *buf2=buf;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
        {   vTab[v].second=0; vTab[v].first=buf2; buf2+=g.getVertNo()+1; }

        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            vTab[v].first->dist=Zero;
        for(int k=1;k<=n;k++) for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
        {   typename EdgeContainer::ValType::CostType nd;
            typename GraphType::PVertex U;
            (vTab[v].first+k)->dist=PlusInfty;
            for(typename GraphType::PEdge E=g.getEdge(v,Koala::EdUndir|Koala::EdDirIn|Koala::EdDirOut);
                E;E=g.getEdgeNext(v,E,Koala::EdUndir|Koala::EdDirIn|Koala::EdDirOut))
                if ((vTab[U=g.getEdgeEnd(E,v)].first+k-1)->dist<PlusInfty &&
                    usedCapCost(g,edgeTab,E,U)>DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::CapacType> ::zero() &&
                    (nd=(vTab[U].first+k-1)->dist+costFlow(g,edgeTab,E,U))<(vTab[v].first+k)->dist)
                    {   (vTab[v].first+k)->dist=nd;
                        (vTab[v].first+k)->ePrev=E;
                        (vTab[v].first+k)->vPrev=U;
                    }
        }

        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if ((vTab[v].first+n)->dist<PlusInfty)
            for(int k=1;k<=n-1;k++)
                if (((vTab[v].first+n)->dist-(vTab[v].first+k)->dist)*(n-vTab[v].second)>
                    ((vTab[v].first+n)->dist-(vTab[v].first+vTab[v].second)->dist)*(n-k))
                vTab[v].second=k;

        std::pair<typename EdgeContainer::ValType::CostType,int> minval,tmpval;
        typename GraphType::PVertex minv=0;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if ((vTab[v].first+n)->dist<PlusInfty)
            {   if (!minv)
                {   minv=v;
                    minval=std::make_pair((vTab[v].first+n)->dist-(vTab[v].first+vTab[v].second)->dist,
                                                                    n-vTab[v].second);
                } else
                {   tmpval=std::make_pair((vTab[v].first+n)->dist-(vTab[v].first+vTab[v].second)->dist,
                                                                    n-vTab[v].second);
                    if (minval.first*tmpval.second>minval.second*tmpval.first)
                    { minval=tmpval; minv=v; }
                }
            }

        if (!minv || minval.first>=Zero) return 0;

        int licz=0;
        for(int i=n;i>0;i--)
        {   vBuf[licz]=minv; eBuf[licz]=(vTab[minv].first+i)->ePrev;
//            std::cout << minv->info;
            minv=(vTab[minv].first+i)->vPrev; licz++;
        }
        vBuf[licz]=minv;
//        std::cout << minv->info << "\n";

        int fpos=-1,lpos=-1;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v)) vTab[v].second=-1;
        for(int i=0;i<=n;i++)
            if (vTab[vBuf[i]].second==-1) vTab[vBuf[i]].second=i;
            else { fpos=vTab[vBuf[i]].second; lpos=i; break; }
        assert(fpos!=-1 && lpos!=-1 && fpos<lpos);
        licz=0;
        for(int i=lpos;i>fpos;i--)
        {   *iters.vertIter=vBuf[i]; ++iters.vertIter;
            *iters.edgeIter=eBuf[i-1]; ++iters.edgeIter;
            licz++;
        }

        return licz;

    }


//    template <class GraphType, class VertContainer, class EdgeContainer>
//    static bool BellmanFordFlow (
//        const GraphType & g,
//        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
//        VertContainer& vertTab,
//        typename GraphType::PVertex start, typename GraphType::PVertex end)
//    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
//    {   assert(start && end && start!=end);
//        const typename EdgeContainer::ValType::CostType
//                inf=DefaultStructs:: template NumberTypeBounds
//                                        <typename EdgeContainer::ValType::CostType>::plusInfty();
//        const typename EdgeContainer::ValType::CostType
//                zero=DefaultStructs:: template NumberTypeBounds
//                                        <typename EdgeContainer::ValType::CostType> ::zero();
//        const EdgeDirection mask=Koala::EdUndir | Koala::EdDirIn | Koala::EdDirOut;
//
//        typename EdgeContainer::ValType::CostType nd;
//        typename GraphType::PVertex U,V;
//
//        //inicjalizacja
//        //for each v: d[v] <- INF (to jest zrealizowane juz przy tworzeniu vertTab)
//        //f[s] <- NIL
//        vertTab[start].vPrev=0;vertTab[start].ePrev=0;
//        //d[s] <- 0
//        vertTab[start].distance=zero;
//
//        //for 1 to n-1:
//        //  for each (u,v):
//        //      if  d[u]+w(u,v) < d[v]:
//        //          d[v] <- d[u]+w(u,v) and vPrev[v] <- u and ePrev[v] <- (u,v)
//        int n=g.getVertNo();
//        for(int i=1;i<n;i++){
//            //relaksacja krawedzi nieskierowanych
//            for(typename GraphType::PEdge E=g.getEdge(mask); E; E=g.getEdgeNext(E, mask))
//            {
//                if (usedCapCost(g,edgeTab,E,U=g.getEdgeEnds(E).first)>
//                    DefaultStructs:: template NumberTypeBounds
//                                        <typename EdgeContainer::ValType::CapacType> ::zero()
//                    && vertTab[U].distance < inf
//                    && (nd=vertTab[U].distance+costFlow(g,edgeTab,E,U))<vertTab[V=g.getEdgeEnds(E).second].distance)
//                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
//                if (usedCapCost(g,edgeTab,E,U=g.getEdgeEnds(E).second)>
//                    DefaultStructs:: template NumberTypeBounds
//                                        <typename EdgeContainer::ValType::CapacType> ::zero()
//                    && vertTab[U].distance < inf
//                    && (nd=vertTab[U].distance+costFlow(g,edgeTab,E,U))<vertTab[V=g.getEdgeEnds(E).first].distance)
//                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
//            }
//        }
//
//            for(typename GraphType::PEdge E=g.getEdge(mask); E; E=g.getEdgeNext(E, mask))
//            {
//                if (usedCapCost(g,edgeTab,E,U=g.getEdgeEnds(E).first)>
//                    DefaultStructs:: template NumberTypeBounds
//                                        <typename EdgeContainer::ValType::CapacType> ::zero()
//                    && vertTab[U].distance < inf
//                    && (nd=vertTab[U].distance+costFlow(g,edgeTab,E,U))<vertTab[V=g.getEdgeEnds(E).second].distance)
//                        assert(0);
//                if (usedCapCost(g,edgeTab,E,U=g.getEdgeEnds(E).second)>
//                    DefaultStructs:: template NumberTypeBounds
//                                        <typename EdgeContainer::ValType::CapacType> ::zero()
//                    && vertTab[U].distance < inf
//                    && (nd=vertTab[U].distance+costFlow(g,edgeTab,E,U))<vertTab[V=g.getEdgeEnds(E).first].distance)
//                        assert(0);
//            }
//
//        return vertTab[end].distance < inf;
//    }


    template <class GraphType, class EdgeContainer>
    static std::pair<typename EdgeContainer::ValType::CostType,
                     typename EdgeContainer::ValType::CapacType>
    minCostFlowFF (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType val=
                    DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty())
    {
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();


        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabsCost<GraphType, typename EdgeContainer::ValType::CostType> >::Type vertTab(g.getVertNo());
        typename EdgeContainer::ValType::CapacType res=Zero;
        typename EdgeContainer::ValType::CostType res2=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::zero();
        typename GraphType::PVertex LOCALARRAY(vTab,g.getVertNo());
        typename GraphType::PEdge LOCALARRAY(eTab,g.getVertNo());


        while (DijkstraFlow(g,edgeTab,vertTab,start,end))
        {   int len=ShortPathStructs::getOutPath( g,vertTab,outPath(vTab,eTab),end );
            typename EdgeContainer::ValType::CapacType delta=val-res;
            for(int i=0;i<len;i++)
                              delta=std::min(delta,usedCapCost(g,edgeTab,eTab[i],vTab[i]));

            for(int i=0;i<len;i++) addFlow(g,edgeTab,eTab[i],vTab[i],delta,true);
            if ((res+=delta)==val) break;
            vertTab.clear();
        }
        return std::make_pair(res2,res);
    }


    template <class GraphType, class EdgeContainer>
    static std::pair<typename EdgeContainer::ValType::CostType,
                     typename EdgeContainer::ValType::CapacType>
    minCostFlowGT (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType val=
                    DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty())
    {
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
        const typename EdgeContainer::ValType::CapacType PlusInfty
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty();


        typename EdgeContainer::ValType::CapacType res=Zero;
        typename EdgeContainer::ValType::CostType res2=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::zero();
        typename GraphType::PVertex LOCALARRAY(vTab,g.getVertNo());
        typename GraphType::PEdge LOCALARRAY(eTab,g.getVertNo());


        int len;
        res=maxFlow(g,edgeTab,start,end,val);
        while (len=minMeanCycle(g,edgeTab,outPath(vTab,eTab)))
        {
            typename EdgeContainer::ValType::CapacType delta=PlusInfty;
            for(int i=0;i<len;i++)
                              delta=std::min(delta,usedCapCost(g,edgeTab,eTab[i],vTab[i]));

            for(int i=0;i<len;i++) addFlow(g,edgeTab,eTab[i],vTab[i],delta,true);
        }
        return std::make_pair(res2,res);
    }


    public:


    template <class GraphType, class EdgeContainer>
    static bool testFlow (
        const GraphType & g,
        const EdgeContainer& edgeTab,
        typename GraphType::PVertex S, typename GraphType::PVertex T)
    {   const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
        assert(S && T && S!=T);
        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            if (std::max(edgeTab[e].flow,-edgeTab[e].flow)<Zero ||
                std::max(edgeTab[e].flow,-edgeTab[e].flow)>edgeTab[e].capac) return false;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (v!=S && v!=T)
        {   typename EdgeContainer::ValType::CapacType sum=Zero;
            for(typename GraphType::PEdge e=g.getEdge(v,EdDirIn);e;e=g.getEdgeNext(v,e,EdDirIn))
                sum-=edgeTab[e].flow;
            for(typename GraphType::PEdge e=g.getEdge(v,EdDirOut);e;e=g.getEdgeNext(v,e,EdDirOut))
                sum+=edgeTab[e].flow;
            for(typename GraphType::PEdge e=g.getEdge(v,EdUndir);e;e=g.getEdgeNext(v,e,EdUndir))
            if (g.getEdgeEnd1(e)==v) sum+=edgeTab[e].flow; else sum-=edgeTab[e].flow;
            if (sum!=Zero) return false;
        }
        return true;

    }


    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    maxFlow (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty())
    {   assert(start && limit>=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero());
        if (DefaultStructs::useFulkersonFord()) return maxFlowFF(g,edgeTab,start,end,limit);
        else return maxFlowMKM(g,edgeTab,start,end,limit);
    }


    template <class GraphType, class EdgeContainer>
    static std::pair<typename EdgeContainer::ValType::CostType,
                     typename EdgeContainer::ValType::CapacType>
    minCostFlow (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType val=
                    DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty())
    {   assert(start);
        assert(end);
        assert(start!=end);
        assert(val>=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero());
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();


        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        {
            edgeTab[e].flow=Zero;
            assert(edgeTab[e].capac>=Zero);
        }
        for(typename GraphType::PEdge E=g.getEdge(EdDirIn | EdDirOut | EdUndir);E;
                                            E=g.getEdgeNext(E,EdDirIn | EdDirOut | EdUndir))
            assert(edgeTab[E].cost >= DefaultStructs:: template NumberTypeBounds
                            <typename EdgeContainer::ValType::CostType>::zero());

        std::pair<typename EdgeContainer::ValType::CostType,typename EdgeContainer::ValType::CapacType> res;
        if (DefaultStructs::useCostFulkersonFord()) res= minCostFlowFF(g,edgeTab,start,end,val);
        else res= minCostFlowGT(g,edgeTab,start,end,val);

        for(typename GraphType::PEdge e=g.getEdge(EdLoop);e;e=g.getEdgeNext(e,EdLoop))
            if (edgeTab[e].cost<
                DefaultStructs:: template NumberTypeBounds
                <typename EdgeContainer::ValType::CostType>::zero())
                    edgeTab[e].flow=edgeTab[e].capac;
        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
                res.first+=std::max(edgeTab[e].flow,-edgeTab[e].flow)*edgeTab[e].cost;
        return res;
    }


    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static EdgeCut<typename EdgeContainer::ValType::CapacType>
    minEdgeCut (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        OutPath<VIter,EIter> iters)
    {   EdgeCut<typename EdgeContainer::ValType::CapacType> res;
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<GraphType,typename EdgeContainer::ValType::CapacType> >::Type vertTab(g.getVertNo());
        res.capac=maxFlow(g,edgeTab,start,end);
        BFSFlow(g,edgeTab,vertTab,start,end,true,blackHole);
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (std::numeric_limits<int>::max()>vertTab[v].distance)
        {   res.vertNo++;
            if (!isBlackHole(iters.vertIter)) { *iters.vertIter=v;++iters.vertIter; }
            for(typename GraphType::PEdge e=g.getEdge(v,EdDirOut|EdUndir);e;e=g.getEdgeNext(v,e,EdDirOut|EdUndir))
            if (vertTab[g.getEdgeEnd(e,v)].distance==std::numeric_limits<int>::max())
            {   res.edgeNo++;
                if (!isBlackHole(iters.edgeIter)) { *iters.edgeIter=e;++iters.edgeIter; }
            }
        }
        return res;
    }

    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static EdgeCut2<GraphType, typename EdgeContainer::ValType::CapacType>
    minEdgeCut (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        OutPath<VIter,EIter> iters)
    {   assert(g.getVertNo()>=2);
        EdgeCut<typename EdgeContainer::ValType::CapacType> res,buf;
        typename GraphType::PVertex a,b;
        typename GraphType::PVertex LOCALARRAY(vres,g.getVertNo()-1);
        typename GraphType::PVertex LOCALARRAY(vbuf,g.getVertNo()-1);
        typename GraphType::PEdge LOCALARRAY(eres,g.getEdgeNo());
        typename GraphType::PEdge LOCALARRAY(ebuf,g.getEdgeNo());
        res.capac=DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::CapacType>
                    ::plusInfty();

        for(typename GraphType::PVertex s=g.getVert();s!=g.getVertLast();s=g.getVertNext(s))
            for(typename GraphType::PVertex t=g.getVertNext(s);t;t=g.getVertNext(t))
        {
            if (isBlackHole(iters.vertIter) && isBlackHole(iters.edgeIter))
                buf=minEdgeCut(g,edgeTab,s,t,outPath(blackHole,blackHole));
            else if (isBlackHole(iters.vertIter) && !isBlackHole(iters.edgeIter))
                buf=minEdgeCut(g,edgeTab,s,t,outPath(blackHole,ebuf));
            else if (!isBlackHole(iters.vertIter) && isBlackHole(iters.edgeIter))
                buf=minEdgeCut(g,edgeTab,s,t,outPath(vbuf,blackHole));
            else buf=minEdgeCut(g,edgeTab,s,t,outPath(vbuf,ebuf));
            if (buf.capac<res.capac)
            {   res=buf; a=s; b=t;
                if (!isBlackHole(iters.vertIter)) for(int i=0;i<buf.vertNo;i++) vres[i]=vbuf[i];
                if (!isBlackHole(iters.edgeIter)) for(int i=0;i<buf.edgeNo;i++) eres[i]=ebuf[i];
            }
            if (g.getEdgeNo(EdDirIn|EdDirOut))
            {
                if (isBlackHole(iters.vertIter) && isBlackHole(iters.edgeIter))
                    buf=minEdgeCut(g,edgeTab,t,s,outPath(blackHole,blackHole));
                else if (isBlackHole(iters.vertIter) && !isBlackHole(iters.edgeIter))
                    buf=minEdgeCut(g,edgeTab,t,s,outPath(blackHole,ebuf));
                else if (!isBlackHole(iters.vertIter) && isBlackHole(iters.edgeIter))
                    buf=minEdgeCut(g,edgeTab,t,s,outPath(vbuf,blackHole));
                else buf=minEdgeCut(g,edgeTab,t,s,outPath(vbuf,ebuf));
//                buf=minEdgeCut(g,edgeTab,t,s,outPath(vbuf,ebuf));
                if (buf.capac<res.capac)
                {   res=buf; a=t; b=s;
                    if (!isBlackHole(iters.vertIter)) for(int i=0;i<buf.vertNo;i++) vres[i]=vbuf[i];
                    if (!isBlackHole(iters.edgeIter)) for(int i=0;i<buf.edgeNo;i++) eres[i]=ebuf[i];
                }
            }
        }
        if (!isBlackHole(iters.vertIter))
            for(int i=0;i<res.vertNo;i++) { *iters.vertIter=vres[i]; ++iters.vertIter; }
        if (!isBlackHole(iters.edgeIter))
            for(int i=0;i<res.edgeNo;i++) { *iters.edgeIter=eres[i]; ++iters.edgeIter; }
        EdgeCut2<GraphType,typename EdgeContainer::ValType::CapacType> res2;
        res2.capac=res.capac; res2.edgeNo=res.edgeNo; res2.vertNo=res.vertNo;
        res2.first=a; res2.second=b;
        return res2;
    }


    template <class DType> struct VertLoss {

        typedef DType CapacType;
        CapacType hi,lo;

        VertLoss(CapacType alo=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                 CapacType ahi=DefaultStructs:: template NumberTypeBounds<CapacType>::zero())
                    : hi(ahi) , lo(alo) {}
    };


    template <class DType,class CType=DType> struct EdgeBound {

        typedef DType CapacType;
        typedef CType CostType;
        CapacType hi,lo,flow;
        CostType cost;

        EdgeBound(CapacType alo=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                  CapacType ahi=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                  CostType c=DefaultStructs:: template NumberTypeBounds<CostType>::zero())
                    : hi(ahi) , lo(alo), cost(c) {}
    };


    template <class GraphType, class EdgeContainer, class VertContainer>
    static bool
    transship (GraphType & g, EdgeContainer& edgeTab,const VertContainer& vertTab)
    {
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();

        typename GraphType::PVertex s,t,u,v;
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                 typename EdgeContainer::ValType::CapacType >::Type exc(g.getVertNo()+3);
        typename DefaultStructs:: template AssocCont<typename GraphType::PEdge,
                 EdgeLabs<typename EdgeContainer::ValType::CapacType > >::Type edgeLabs(2*g.getVertNo()+g.getEdgeNo()+2);
        typename EdgeContainer::ValType::CapacType sum
                            =Zero;

        for(typename GraphType::PEdge e=g.getEdge(EdUndir);e;e=g.getEdgeNext(e,EdUndir))
            {   assert(Zero==edgeTab[e].lo); }

        u=g.addVert();
        for(v=g.getVert();v;v=g.getVertNext(v))
            if (v!=u) edgeTab[g.addArch(v,u)]=
                EdgeBound<typename EdgeContainer::ValType::CapacType>(vertTab[v].lo,vertTab[v].hi);

        for(v=g.getVert();v;v=g.getVertNext(v))
        {   exc[v]=Zero;
            for(typename GraphType::PEdge e=g.getEdge(v,EdDirIn);e;e=g.getEdgeNext(v,e,EdDirIn))
                exc[v]+=edgeTab[e].lo;
            for(typename GraphType::PEdge e=g.getEdge(v,EdDirOut);e;e=g.getEdgeNext(v,e,EdDirOut))
                exc[v]-=edgeTab[e].lo;
            if (exc[v]>Zero) sum+=exc[v];
        }

        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut|EdUndir);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut|EdUndir))
            edgeLabs[e].capac=edgeTab[e].hi-edgeTab[e].lo;
        s=g.addVert(); t=g.addVert();
        for(v=g.getVert();v;v=g.getVertNext(v)) if (v!=s && v!=t)
        {   if (exc[v]>Zero) edgeLabs[g.addArch(s,v)].capac=exc[v];
            else if (exc[v]<Zero) edgeLabs[g.addArch(v,t)].capac=-exc[v];
        }
        bool res= maxFlow(g,edgeLabs,s,t)==sum;

        for(typename GraphType::PEdge e=g.getEdge(u);e;e=g.getEdgeNext(u,e)) edgeTab.delKey(e);
        for(typename GraphType::PEdge e=g.getEdge(s);e;e=g.getEdgeNext(s,e)) edgeTab.delKey(e);
        for(typename GraphType::PEdge e=g.getEdge(t);e;e=g.getEdgeNext(t,e)) edgeTab.delKey(e);
        g.delVert(u); g.delVert(s); g.delVert(t);
        if (!res) return false;
        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut|EdUndir);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut|EdUndir))
            edgeTab[e].flow=edgeLabs[e].flow+edgeTab[e].lo;
        for(typename GraphType::PEdge e=g.getEdge(EdLoop);e;e=g.getEdgeNext(e,EdLoop))
            edgeTab[e].flow=edgeTab[e].lo;
        return true;
    }


    template <class GraphType, class EdgeContainer, class VertContainer>
    static typename EdgeContainer::ValType::CostType
    minCostTransship (GraphType & g, EdgeContainer& edgeTab,const VertContainer& vertTab)
    {
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();

        typename GraphType::PVertex s,t,u,v;
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                 typename EdgeContainer::ValType::CapacType >::Type exc(g.getVertNo()+3);
        typename DefaultStructs:: template AssocCont<typename GraphType::PEdge,
                 EdgeLabs<typename EdgeContainer::ValType::CapacType > >::Type edgeLabs(2*g.getVertNo()+g.getEdgeNo()+2);
        typename EdgeContainer::ValType::CapacType sum
                            =Zero;

        for(typename GraphType::PEdge e=g.getEdge(EdUndir);e;e=g.getEdgeNext(e,EdUndir))
            {   assert(Zero==edgeTab[e].lo); }

        u=g.addVert();
        for(v=g.getVert();v;v=g.getVertNext(v))
            if (v!=u) edgeTab[g.addArch(v,u)]=
                EdgeBound<typename EdgeContainer::ValType::CapacType>(vertTab[v].lo,vertTab[v].hi);

        for(v=g.getVert();v;v=g.getVertNext(v))
        {   exc[v]=Zero;
            for(typename GraphType::PEdge e=g.getEdge(v,EdDirIn);e;e=g.getEdgeNext(v,e,EdDirIn))
                exc[v]+=edgeTab[e].lo;
            for(typename GraphType::PEdge e=g.getEdge(v,EdDirOut);e;e=g.getEdgeNext(v,e,EdDirOut))
                exc[v]-=edgeTab[e].lo;
            if (exc[v]>Zero) sum+=exc[v];
        }

        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut|EdUndir);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut|EdUndir))
        {
            edgeLabs[e].capac=edgeTab[e].hi-edgeTab[e].lo;
            edgeLabs[e].cost=edgeTab[e].cost;
        }
        s=g.addVert(); t=g.addVert();
        for(v=g.getVert();v;v=g.getVertNext(v)) if (v!=s && v!=t)
        {   if (exc[v]>Zero) edgeLabs[g.addArch(s,v)].capac=exc[v];
            else if (exc[v]<Zero) edgeLabs[g.addArch(v,t)].capac=-exc[v];
        }
        if (minCostFlow(g,edgeLabs,s,t,sum).second!=sum) return DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::plusInfty();

        for(typename GraphType::PEdge e=g.getEdge(u);e;e=g.getEdgeNext(u,e)) edgeTab.delKey(e);
        for(typename GraphType::PEdge e=g.getEdge(s);e;e=g.getEdgeNext(s,e)) edgeTab.delKey(e);
        for(typename GraphType::PEdge e=g.getEdge(t);e;e=g.getEdgeNext(t,e)) edgeTab.delKey(e);
        g.delVert(u); g.delVert(s); g.delVert(t);
        typename EdgeContainer::ValType::CostType res=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::zero();
        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut|EdUndir);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut|EdUndir))
        {
             edgeTab[e].flow=edgeLabs[e].flow+edgeTab[e].lo;
             res+=(g.getEdgeType(e)!=Undirected) ? edgeTab[e].flow*edgeTab[e].cost
                            : std::max(edgeTab[e].flow,-edgeTab[e].flow)*edgeTab[e].cost;
        }
        for(typename GraphType::PEdge e=g.getEdge(EdLoop);e;e=g.getEdgeNext(e,EdLoop))
        {
            if (edgeTab[e].cost>=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::zero())
                edgeTab[e].flow=edgeTab[e].lo; else edgeTab[e].flow=edgeTab[e].hi;
                res+=edgeTab[e].flow*edgeTab[e].cost;
        }

        return res;
    }


    template <class GraphType,class CType>
    struct GHTreeEdge {
        typedef CType CapacType;

        typename GraphType::PVertex first,second;
        CapacType capac;

        GHTreeEdge(typename GraphType::PVertex f=0,typename GraphType::PVertex s=0,
                   CapacType c=DefaultStructs:: template NumberTypeBounds<CapacType>::zero()) :
            first(f), second(s), capac(c)
        {}
    };


    protected:


    template <class GraphType, class EdgeContainer, class SetContainer>
    static void ghtree(GraphType & g, EdgeContainer& edgeTab, SetContainer& setTab,
                Set<typename GraphType::PVertex>& V, Set<typename GraphType::PVertex>& R,
                GHTreeEdge<GraphType,typename EdgeContainer::ValType::CapacType>* out)
    {
        typename EdgeContainer::ValType::CapacType capac;

        Set<typename GraphType::PVertex> W1,W2,Vnew,Rnew;
        if (R.size()==1) { setTab[R.first()]=V; return; }
        typename GraphType::PVertex s=R.first(), t=R.last();
        if (R.size()==2) {
            capac=minEdgeCut(makeSubgraph(g,std::make_pair(stdChoose(V),edgeTypeChoose(Undirected))),
                   edgeTab,s,t,outPath(setInserter(W1),blackHole)).capac; W2=V-W1;
            setTab[s]=W1;setTab[t]=W2;
            *out=GHTreeEdge<GraphType,typename EdgeContainer::ValType::CapacType>(s,t,capac);
            return;
        }

        capac=minEdgeCut(makeSubgraph(g,std::make_pair(stdChoose(V),edgeTypeChoose(Undirected))),
                   edgeTab,s,t,outPath(setInserter(W1),blackHole)).capac; W2=V-W1;

        typename GraphType::PVertex vnew=g.addVert();
        for(typename GraphType::PVertex v=W1.first();;v=W1.next(v))
        {   typename GraphType::PEdge enew=g.addEdge(v,vnew);
            edgeTab[enew].capac=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
            for(typename GraphType::PEdge e=g.getEdge(v,EdUndir);e;e=g.getEdgeNext(v,e,EdUndir))
            if (W2.isElement(g.getEdgeEnd(e,v))) edgeTab[enew].capac+=edgeTab[e].capac;
            if (v==W1.last()) break;
        }
        Vnew=W1;Vnew+=vnew;Rnew=R*W1;
        ghtree(g,edgeTab,setTab,Vnew,Rnew,out);

        GHTreeEdge<GraphType,typename EdgeContainer::ValType::CapacType>* out2=out+Rnew.size()-1;
        for(s=Rnew.first();s;s=Rnew.next(s))
            if (setTab[s].isElement(vnew)) { setTab[s]-=vnew; break; }
        for(typename GraphType::PEdge e=g.getEdge(vnew,EdUndir);e;e=g.getEdgeNext(vnew,e,EdUndir))
            edgeTab.delKey(e);
        g.delEdges(vnew);

        for(typename GraphType::PVertex v=W2.first();;v=W2.next(v))
        {   typename GraphType::PEdge enew=g.addEdge(v,vnew);
            edgeTab[enew].capac=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
            for(typename GraphType::PEdge e=g.getEdge(v,EdUndir);e;e=g.getEdgeNext(v,e,EdUndir))
            if (W1.isElement(g.getEdgeEnd(e,v))) edgeTab[enew].capac+=edgeTab[e].capac;
            if (v==W2.last()) break;
        }
        Vnew=W2;Vnew+=vnew;Rnew=R*W2;
        ghtree(g,edgeTab,setTab,Vnew,Rnew,out2);

        for(t=Rnew.first();t;t=Rnew.next(t))
            if (setTab[t].isElement(vnew)) { setTab[t]-=vnew; break; }
        for(typename GraphType::PEdge e=g.getEdge(vnew,EdUndir);e;e=g.getEdgeNext(vnew,e,EdUndir))
            edgeTab.delKey(e);
        setTab.delKey(vnew);
        g.delVert(vnew,true);

        assert(V.isElement(s)&&V.isElement(t));
        *(out2+Rnew.size()-1)=GHTreeEdge<GraphType,typename EdgeContainer::ValType::CapacType>(s,t,capac);
    }


    public:


    template <class GraphType, class EdgeContainer>
    static void getGHTree(GraphType & g, EdgeContainer& edgeTab,
                GHTreeEdge<GraphType,typename EdgeContainer::ValType::CapacType>* out)
    {   assert(g.getVertNo()>1);
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                Set<typename GraphType::PVertex> >::Type setMap(g.getVertNo());
        Set<typename GraphType::PVertex> V=g.getVertSet(),R=V;
        ghtree(g,edgeTab,setMap,V,R,out);
    }


};

class Flow : public FlowPar<FlowAlgsDefaultStructs> {};



class ConnectAlgsDefaultStructs : public FlowAlgsDefaultStructs {
    public:

    template <class A, class B> class LocalGraph {
        public:
        typedef Graph<A,B,DefaultGrSettings<EdDirIn|EdDirOut,false> > Type;
//              typedef Graph<A,B> Type; // wersja bez trzeciego par. szablonu grafu
    };

};

template <class DefaultStructs>
class ConnectPar : public PathStructs {

    public:

    template <class GraphType> struct EdgeCut {

            typename GraphType::PVertex first, second;
            int edgeNo;

            EdgeCut() {}
    };


    protected:

    struct EdgeLabs {

        int capac,flow;

        EdgeLabs() : capac(1), flow(0)
            {}
    };

    public:

    template <class GraphType, class EIter>
    static int
    minEdgeCut (
        const GraphType & g,
        typename GraphType::PVertex start, typename GraphType::PVertex end, EIter iter)
    {   typename DefaultStructs::   template AssocCont<typename GraphType::PEdge,
                                    typename FlowPar<DefaultStructs>:: template EdgeLabs<int> >::Type
                                        edgeLabs(g.getEdgeNo());
        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeLabs[e].capac=1;
        return FlowPar<DefaultStructs>::minEdgeCut(g,edgeLabs,start,end,
                                        FlowPar<DefaultStructs>::outPath(blackHole,iter)).capac;
    }


    template <class GraphType, class EIter>
    static EdgeCut<GraphType>
    minEdgeCut (const GraphType & g, EIter iter)
    {   EdgeCut<GraphType> res;
        typename DefaultStructs::   template AssocCont<typename GraphType::PEdge,
                                    typename FlowPar<DefaultStructs>:: template EdgeLabs<int> >::Type
                                        edgeLabs(g.getEdgeNo());
        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeLabs[e].capac=1;
        typename FlowPar<DefaultStructs>:: template EdgeCut2<GraphType,int> res2
                =FlowPar<DefaultStructs>:: template minEdgeCut(g,edgeLabs,
                                    FlowPar<DefaultStructs>::template outPath(blackHole,iter));
        res.edgeNo=res2.capac; res.first=res2.first; res.second=res2.second;
        return res;
    }


    template <class GraphType, class VIter, class EIter, class LenIter>
    static int
    edgeDisjPaths( GraphType & g,
                  typename GraphType::PVertex start, typename GraphType::PVertex end,
                  OutPath< VIter,EIter > iters, LenIter liter,bool vertsoutflag=false)
    {   typename DefaultStructs::   template AssocCont<typename GraphType::PEdge,
                                    typename FlowPar<DefaultStructs>:: template EdgeLabs<int> >::Type
                                        edgeTab(g.getEdgeNo());
        typename DefaultStructs::   template AssocCont<typename GraphType::PEdge,
                            std::pair<typename GraphType::PVertex,typename GraphType::PVertex> >::Type
                                        undirs(g.getEdgeNo(EdUndir));
        typename DefaultStructs::   template AssocCont<typename GraphType::PEdge,int>::Type
                                        paths(2*g.getEdgeNo());
        typename GraphType::PEdge LOCALARRAY(euler,2*g.getEdgeNo());
        typename GraphType::PEdge LOCALARRAY(eout,g.getEdgeNo());
        typename GraphType::PVertex LOCALARRAY(vout,g.getVertNo());

        *liter=0; ++liter;
        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut|EdUndir);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut|EdUndir))
            edgeTab[e].capac=1;
        int res=FlowPar<DefaultStructs>:: template maxFlow(g,edgeTab,start,end);
        if (!res) return 0;
        for(typename GraphType::PEdge e=edgeTab.firstKey();e;e=edgeTab.nextKey(e))
            if (g.getEdgeType(e)==Undirected && edgeTab[e].flow)
            {
                undirs[e]=g.getEdgeEnds(e);
                if (edgeTab[e].flow>0) g.moveEdge(e,undirs[e].first,undirs[e].second,EdDirOut);
                else g.moveEdge(e,undirs[e].second,undirs[e].first,EdDirOut);
            }
        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut|EdUndir);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut|EdUndir))
            if (edgeTab[e].flow) paths[e]=0;
        for(int i=0;i<res;i++) paths[g.addArch(end,start)]=-1;
            EulerPar<DefaultStructs>:: template getDirCycle(
                makeSubgraph(g,std::make_pair(stdChoose(true),edgeTypeChoose(Directed) && extAssocKeyChoose(&(paths)))),
                start,outPath(blackHole,euler));
        int r=0;
        for(int i=0;i<paths.size();i++)
            if (paths[euler[i]]!=-1) paths[euler[i]]=r; else r++;
        int l=0;
        for(r=0;r<res;r++)
        {   int j=BFSPar<DefaultStructs>:: template getPath(
                makeSubgraph(g,std::make_pair(stdChoose(true),extAssocChoose(&(paths),r))),
                start,end,outPath(vout,eout),EdDirOut);
            l+=j + ((vertsoutflag)?1:0); *liter=l; ++liter;
            if (!isBlackHole(iters.edgeIter))
                for(int k=0;k<j;k++) { *iters.edgeIter=eout[k]; ++iters.edgeIter;}
            if (!isBlackHole(iters.vertIter))
                for(int k=0;k<=j;k++) { *iters.vertIter=vout[k]; ++iters.vertIter;}
        }

        for(typename GraphType::PEdge e=undirs.firstKey();e;e=undirs.nextKey(e))
            g.moveEdge(e,undirs[e].first,undirs[e].second,EdUndir);
        l=paths.getKeys(euler);
        for(int i=0;i<l;i++) if (paths[euler[i]]==-1) g.delEdge(euler[i]);

        return res;
    }


    protected:


    template <class GraphType, class ImageType, class Linker>
    static void makeImage(const GraphType& g, ImageType& ig, Linker& images)
    {
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
        {   images[v].first=ig.addVert(v);images[v].second=ig.addVert(v);
            ig.addArch(images[v].first,images[v].second,
                       std::pair<typename GraphType::PVertex,typename GraphType::PEdge>(v,(typename GraphType::PEdge)0));
        }
        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut))
            ig.addArch(images[g.getEdgeEnd1(e)].second,
                                 images[g.getEdgeEnd2(e)].first,
                       std::pair<typename GraphType::PVertex,typename GraphType::PEdge>((typename GraphType::PVertex)0,e));
        for(typename GraphType::PEdge e=g.getEdge(EdUndir);e;e=g.getEdgeNext(e,EdUndir))
        {   ig.addArch(images[g.getEdgeEnd1(e)].second,
                                 images[g.getEdgeEnd2(e)].first,
                       std::pair<typename GraphType::PVertex,typename GraphType::PEdge>((typename GraphType::PVertex)0,e));
            ig.addArch(images[g.getEdgeEnd2(e)].second,
                                 images[g.getEdgeEnd1(e)].first,
                       std::pair<typename GraphType::PVertex,typename GraphType::PEdge>((typename GraphType::PVertex)0,e));
        }
    }


    public:


    template <class GraphType, class VIter>
    static int
    minVertCut (
        const GraphType & g,
        typename GraphType::PVertex start, typename GraphType::PVertex end, VIter iter)
    {   assert(start && end && (start!=end) );
        typedef typename DefaultStructs::template LocalGraph<typename GraphType::PVertex,
                std::pair<typename GraphType::PVertex,typename GraphType::PEdge> >:: Type Image;
        Image ig;
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                            std::pair<typename Image::PVertex,typename Image::PVertex> >::Type
                                images(g.getVertNo());
        typename DefaultStructs::   template AssocCont<typename Image::PEdge,
                                    typename FlowPar<DefaultStructs>:: template EdgeLabs<int> >::Type
                                        imageFlow(2*g.getEdgeNo()+2*g.getVertNo());
        typename Image::PEdge LOCALARRAY(icut,g.getVertNo());

        makeImage(g,ig,images);
        for(typename Image::PEdge e=ig.getEdge();e;e=ig.getEdgeNext(e))
            imageFlow[e].capac = (e->info.first) ? 1 : 2;

        int res=FlowPar<DefaultStructs>::minEdgeCut(ig,imageFlow,images[start].second,images[end].first,
                                        FlowPar<DefaultStructs>::outPath(blackHole,icut)).edgeNo;
        for(int i=0;i<res;i++)
        {   assert(icut[i]->info.first);
            *iter=icut[i]->info.first;++iter;
        }
        return res;
    }


    template <class GraphType, class VIter>
    static int
    minVertCut (const GraphType & g,VIter iter)
    {   typedef typename DefaultStructs::template LocalGraph<typename GraphType::PVertex,
                std::pair<typename GraphType::PVertex,typename GraphType::PEdge> >:: Type Image;
        Image ig;
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                            std::pair<typename Image::PVertex,typename Image::PVertex> >::Type
                                images(g.getVertNo());
        typename DefaultStructs::   template AssocCont<typename Image::PEdge,
                                    typename FlowPar<DefaultStructs>:: template EdgeLabs<int> >::Type
                                        imageFlow(2*g.getEdgeNo()+2*g.getVertNo());
        typename Image::PEdge LOCALARRAY(icut,g.getVertNo());
        typename Image::PEdge LOCALARRAY(bestcut,g.getVertNo());

        makeImage(g,ig,images);
        for(typename Image::PEdge e=ig.getEdge();e;e=ig.getEdgeNext(e))
            imageFlow[e].capac = (e->info.first) ? 1 : 2;


        int best=std::numeric_limits<int>::max()-1,iv=0;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
        {   ++iv;
            for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u)) if (u!=v)
            {
                if (!g.getEdge(u,v,EdDirOut|EdUndir))
                {
//                    for(typename Image::PEdge e=ig.getEdge();e;e=ig.getEdgeNext(e)) imageFlow[e].flow=0;
                    int res=FlowPar<DefaultStructs>::minEdgeCut(ig,imageFlow,images[u].second,
                                                                            images[v].first,
                                        FlowPar<DefaultStructs>::outPath(blackHole,icut)).edgeNo;
                    if (res<best) { best=res; for(int i=0;i<res;i++) bestcut[i]=icut[i];}
                }
                if (!g.getEdge(v,u,EdDirOut|EdUndir))
                {
//                    for(typename Image::PEdge e=ig.getEdge();e;e=ig.getEdgeNext(e)) imageFlow[e].flow=0;
                    int res=FlowPar<DefaultStructs>::minEdgeCut(ig,imageFlow,images[v].second,
                                                                            images[u].first,
                                        FlowPar<DefaultStructs>::outPath(blackHole,icut)).edgeNo;
                    if (res<best) { best=res; for(int i=0;i<res;i++) bestcut[i]=icut[i];}
                }
                if (iv>best+1)
                {
                    for(int i=0;i<best;i++)
                    {   assert(bestcut[i]->info.first);
                        *iter=bestcut[i]->info.first;++iter;
                    }
                    return best;
                }
            }
        }
        return -1;
    }


    template <class GraphType, class VIter, class EIter, class LenIter>
    static int
    vertDisjPaths(const GraphType & g,
                  typename GraphType::PVertex start, typename GraphType::PVertex end,
                  OutPath< VIter,EIter > iters, LenIter liter,bool vertsoutflag=false)
    {   assert(start && end && start!=end);
        typedef typename DefaultStructs::template LocalGraph<typename GraphType::PVertex,
                std::pair<typename GraphType::PVertex,typename GraphType::PEdge> >:: Type Image;
        Image ig;
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                            std::pair<typename Image::PVertex,typename Image::PVertex> >::Type
                                images(g.getVertNo());

        typename Image::PEdge LOCALARRAY(impaths,g.getVertNo()+2*g.getEdgeNo());
        int LOCALARRAY(impos,2*g.getEdgeNo()+2);


        makeImage(g,ig,images);
        for(typename Image::PEdge e=ig.getEdge(images[start].second,images[end].first);e;
            e=ig.getEdge(images[start].second,images[end].first)) ig.delEdge(e);
        for(typename Image::PEdge e=ig.getEdge(images[end].second,images[start].first);e;
            e=ig.getEdge(images[end].second,images[start].first)) ig.delEdge(e);


        int res=edgeDisjPaths(ig,images[start].second,images[end].first,outPath(blackHole,impaths),
                                                                                    impos,false);
        *liter=0; ++liter;
        int vpos=0,epos=0;
        for(int i=0;i<res;i++)
        {   bool ed=true;
            *iters.vertIter=start; ++iters.vertIter;++vpos;
            for(int j=impos[i];j<impos[i+1];j++)
            {   if (ed)
                { *iters.edgeIter=impaths[j]->info.second; ++iters.edgeIter;++epos;}
                else
                { *iters.vertIter=impaths[j]->info.first; ++iters.vertIter;++vpos;}
                ed=!ed;
            }
            *iters.vertIter=end; ++iters.vertIter;++vpos;
            if (vertsoutflag) { *liter=vpos; ++liter; }
            else { *liter=epos; ++liter; }
        }
        res+=g.getEdgeNo(start,end,EdDirOut|EdUndir);
        for(typename GraphType::PEdge e=g.getEdge(start,end,EdDirOut|EdUndir);e;
                e=g.getEdgeNext(start,end,e,EdDirOut|EdUndir))
        {   *iters.vertIter=start; ++iters.vertIter;++vpos;
            *iters.edgeIter=e; ++iters.edgeIter;++epos;
            *iters.vertIter=end; ++iters.vertIter;++vpos;
            if (vertsoutflag) { *liter=vpos; ++liter; }
            else { *liter=epos; ++liter; }
        }

        return res;
    }


};

class Connect : public ConnectPar<ConnectAlgsDefaultStructs> {};


}

#endif

