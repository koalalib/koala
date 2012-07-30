#ifndef KOALA_DEF_FLOWS_H
#define KOALA_DEF_FLOWS_H

#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <limits>
#include "../base/def_struct.h"
//#include "../graph/graph.h"
#include "../graph/view.h"
#include "../algorithm/search.h"



namespace Koala {


// Domyslne wytyczne dla procedur przeplywowych
template <bool FF=false, // czy do wyznaczania maks. przeplywu uzywac FulkersonaForda, czy MKM
        bool costFF=true> // czy do wyznaczania najtanszego przeplywu uzywac sciezek powiekszajacych (pseudowielomianowa),
        // czy cykli z min. srednia dlugoscia (wielomianowa)
class FlowAlgsDefaultSettings : public AlgsDefaultSettings {
    public:

    enum { useFulkersonFord=FF  };
    enum { useCostFulkersonFord=costFF };
};


//Algorytmy znajdowania przeplywu, najwiekszego przeplywu, najtanszego przeplywu, rozciec, transhipments itd.
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class FlowPar : public PathStructs {

    public:

    // rekord z danymi (in-out) opisujacy krawedz
    template <class DType, class CType=DType> struct EdgeLabs {

        typedef DType CapacType; // typ liczbowy przepustowosci luku i objetosci przeplywu
        typedef CType CostType; // typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu

        CapacType capac, // przepustowosc (dana wejsciowa)
                    flow;   // wartosc znalezionego przeplywu w kierunku getEdgeEnd1->getEdgeEnd2
        CostType cost;  // koszt jednostkowy przeplywu przez luk (dana wejsciowa)

        // agrs: przepustowosc i koszt krawedzi
        EdgeLabs(CapacType arg=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                 CostType arg2=DefaultStructs:: template NumberTypeBounds<CostType>::zero()) :
                                capac(arg), cost(arg2),
                                flow(DefaultStructs:: template NumberTypeBounds<CapacType>::zero())
            {}
    };

    // j.w. ale nadaje domyslne jednostkowe przepustowosci i koszty
    template <class DType, class CType=DType> struct UnitEdgeLabs : public EdgeLabs<DType,CType> {

        UnitEdgeLabs(DType arg=(DType)1, CType arg2=(CType)1) : EdgeLabs<DType,CType>(arg,arg2)
            {}
    };

    // rekord wynikowy opisujacy rozciecie krawedziowe w grafie miedzy para wierzcholkow
    template <class CapacType> struct EdgeCut {

            CapacType capac; // typ liczbowy przepustowosci luku i objetosci przeplywu
            int vertNo; // liczba wierzcholkow osiagalnych z poczatkowego po usunieciu rozciecia
            int edgeNo; // liczba krawedzi rozciecia

            EdgeCut() : capac(DefaultStructs:: template NumberTypeBounds<CapacType>::zero()),
                        vertNo(0), edgeNo(0)
            {}
    };

    // j.w. ale podaje rowniez 2 wierzcholki - z obu czesci rozcietego grafu
    template <class GraphType, class CapacType> struct EdgeCut2 : public EdgeCut<CapacType> {

            typename GraphType::PVertex first, second;
    };


    // "krawedz" drzewa Gomory-Hu
    template <class GraphType,class CType>
    struct GHTreeEdge {
        typedef CType CapacType; // typ liczbowy przepustowosci luku

        typename GraphType::PVertex first,second; // koncowki krawedzi
        CapacType capac; // przepustowosc "krawedzi"

        // args: oba wierzcholki koncowe i przepustowosc
        GHTreeEdge(typename GraphType::PVertex f=0,typename GraphType::PVertex s=0,
                   CapacType c=DefaultStructs:: template NumberTypeBounds<CapacType>::zero()) :
            first(f), second(s), capac(c)
        {}
    };

    // Transshipment - uogolnienie przeplywu (por. Schrijver)

    // rekord wejsciowy opisujacy wierzcholek dla wyszukiwania transhipmentow
    template <class DType> struct TrsVertLoss {

        typedef DType CapacType;  // typ liczbowy przepustowosci luku i objetosci przeplywu
        CapacType hi,lo; // maksymalny i minimalny dopuszczalny przy danym wierzcholku laczny wyplyw
        //(dopuszczalne dodatnie i ujemne np. przeplyw to trans. z hi=lo=0 wszedzie poza zrodlem i ujsciem)

        TrsVertLoss(CapacType alo=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                 CapacType ahi=DefaultStructs:: template NumberTypeBounds<CapacType>::zero())
                    : hi(ahi) , lo(alo) {}
    };

    // rekord  opisujacy krawedz dla wyszukiwania transhipmentow
    template <class DType,class CType=DType> struct TrsEdgeLabs {

        typedef DType CapacType; // typ liczbowy przepustowosci luku i objetosci przeplywu
        typedef CType CostType; // typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
        CapacType hi,lo; // wymagane gorne i dolne ograniczenie na wielkosc przeplywu przez ta krawedz.
            //Dla nieskierowanych wymaga sie lo=0
            // TODO: sprawdzic, czy moga byc ujemne dla lukow
        CapacType flow; // wartosc znalezionego przeplywu (transship.) w kierunku getEdgeEnd1->getEdgeEnd2
        CostType cost; // koszt jednostkowy przeplywu dla luku, jesli wymagany to wymagany nieujemny z wyjatkiem petli

        TrsEdgeLabs(CapacType alo=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                  CapacType ahi=DefaultStructs:: template NumberTypeBounds<CapacType>::zero(),
                  CostType c=DefaultStructs:: template NumberTypeBounds<CostType>::zero())
                    : hi(ahi) , lo(alo), cost(c) {}
    };



// TODO: w ostatecznej wersji   protected: (ale na razie wystepuje w testach)
    public:


    // rekord pomocniczy opisujacy wierzcholek
    template <class GraphType,class CapacType> struct VertLabs {

        int distance,backdist; // do BFS w sieci tymczasowej
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        CapacType mass, inPot, outPot; // potencjaly przesylu (na uzytek maxFlowMKM)
        bool used; // czy wierzcholek jest w sieci warstwowej (na uzytek maxFlowMKM)

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

    // przepustowosc uzyteczna krawedzi w sieci tymczasowej z danymi przeplywami
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    usedCap (
        const GraphType & g,
        EdgeContainer& edgeTab,
        typename GraphType::PEdge e,
        typename GraphType::PVertex v,
        bool out // czy chodzi o przejscie wzdluz e od v czy do v
        )
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


    // zwiekszanie przeplywu przez krawedz e od delta w kierunku od v lub do v (zmienna out)
    template <class GraphType, class EdgeContainer>
    static void
    addFlow (
        const GraphType & g,
        EdgeContainer& edgeTab,
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


    // BFS w sieci tymczasowej przy danych przepustowosciach i przeplywie
    // Sekwencja wyjsciowa wierzcholkow idzie na iter
    // Poruszamy sie "do przodu" lub "do tylu" (out) odpowiednio uzywajac pol distance lub backdistance rekodru
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

    // pomocnicza dla maxFlowMKM
    // identyfikacja wierzcholkow warstw miedzy first a last sieci tymczasowej (pola used, distance, backdistane)
    // zwraca: czy jest przejscie first->last
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

    // pomocnicza dla maxFlowMKM
    // wyznaczanie potencjalow przeplywu wierzcholkow w sieci warstwowej miedzy ends
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

    // na uzytek maxFlowMKM
    // przepchniecie przeplywu z wierzcholka v do przodu lub do tylu
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

    // na uzytek maxFlowMKM
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

    // na uzytek MKM, znajduje maksymalny (ale nie przekraczajacy limit) przeplyw miedzy start a end
    template <class GraphType, class EdgeContainer,class VertContainer>
    static typename EdgeContainer::ValType::CapacType
    layerFlow (
        const GraphType & g,
        EdgeContainer& edgeTab,
        VertContainer& vertTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit)
    {   assert(start); // TODO: throw
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


   // Znajdowanie maksymalnego (ale nie przekraczajacego limit) przeplywu miedzy start a end
   // Algorytm MKM
    // Zwraca wielkosc przeplywu
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    maxFlowMKM (
        const GraphType & g, // badany graf
        EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac (wymagane nieujemne)
                                // wyniki zwracane do pol flow
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit // gorny limit wielkosci znalezionego przeplywu
        )
    {   // TODO: throw
        assert(start);
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
            edgeTab[e].flow=Zero;
            assert(edgeTab[e].capac>=Zero);// TODO: throw
        }
        if (limit==Zero) return Zero;
        while (Zero!=(add=layerFlow(g,edgeTab,vertTab,start,end,limit-res)))
        {   res+=add;
            if (limit==res) break;
        }
        return res;
    }


   // Znajdowanie maksymalnego (ale nie przekraczajacego limit) przeplywu miedzy start a end
   // Algorytm FulkersonaFodra
    // Zwraca wielkosc przeplywu
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    maxFlowFF (
        const GraphType & g,// badany graf
        EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac (wymagane nieujemne)
                                // wyniki zwracane do pol flow
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit)// gorny limit wielkosci znalezionego przeplywu
    {   // TODO: throw
        assert(start);
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
            assert(edgeTab[e].capac>=Zero); // TODO: throw
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

    // podobna do usedCap na uzytek przeplywow z kosztem
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


    // koszt jednostkowy przeplywu przez krawedz od danej koncowki w sieci tymczasowej
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


    // pomocnicza etykieta wierzcholka na uzytek alg. Dijkstry w sieci tymczasowej
    template <class GraphType,class CostType> struct VertLabsCost {

        CostType distance,pot;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        VertLabsCost() : vPrev(0), ePrev(0),
                    distance(DefaultStructs:: template NumberTypeBounds<CostType>::plusInfty()),
                    pot(DefaultStructs:: template NumberTypeBounds<CostType>::zero()) {}
    };


    // Korekta kosztu luku na uzytek Dijkstry z potencjalami wierzcholkow (nowe koszty nie wprowadzaja ujemnych cykli)
    template <class GraphType, class EdgeContainer, class VertContainer>
    static typename EdgeContainer::ValType::CostType
    costFlowDij (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        VertContainer& vertTab, // i tablica dlugosci krawedzi
        typename GraphType::PEdge e,typename GraphType::PVertex v)
    {   return costFlow(g,edgeTab,e,v)+vertTab[v].pot-vertTab[g.getEdgeEnd(e,v)].pot; }


    // Dijkstra w sieci tymczasowej z kosztami lukow skorygowanymi przez potencjaly wierzcholkow
    // Zwraca: czy istnieje przejscie start->end
    // TODO: rozwazyc przejscie na kopce
    template <class GraphType, class VertContainer, class EdgeContainer>
    static bool DijkstraFlow (
        const GraphType & g,
        EdgeContainer& edgeTab,
        VertContainer& vertTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end)
    {   assert(start && end && start!=end); // TODO: throw
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
        // powrot do "prawdziwego" dystansu - usuniecie wkladu potencjalow
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            vertTab[v].distance+=vertTab[v].pot-vertTab[start].pot;
        // nowe potencjaly wierzcholkow na uzytek kolejnej iteracji procedury
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            vertTab[v].pot=vertTab[v].distance;
        return res;
    }


    // pomocniczza etykieta wierzcholka dla minMeanCycle
    template <class GraphType,class CostType> struct CycLabs {

        CostType dist;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        CycLabs() : vPrev(0), ePrev(0),
                    dist(DefaultStructs:: template NumberTypeBounds<CostType>::plusInfty())
             {}
    };

    // znajdowanie w sieci tymczasowej cyklu o najmniejszej sredniej dlugosci krawedzi
    // TODO: nie testowane, sprawdzic!
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
        //for(int i=0;i<n*n+n;i++) buf[i]=CycLabs< GraphType, typename EdgeContainer::ValType::CostType>();

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

    // znajdowanie przeplywu start->end o maksymalnej objetosci (ale nie przekraczajacej limitu val) i najmniejszym koszcie
    // procedura pseudowielomianowa (sciezki powiekszajace typu FulkersonFord)
    // zwraca pare wielkosc przeplywu
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    minCostFlowFF (
        const GraphType & g, // badany graf
        EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac i cost (wymagane nieujemne)
                                // wyniki zwracane do pol flow
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType val=
                    DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty()) // limit wielkosci przeplywu
    {
        const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();


        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabsCost<GraphType, typename EdgeContainer::ValType::CostType> >::Type vertTab(g.getVertNo());
        typename EdgeContainer::ValType::CapacType res=Zero;
//        typename EdgeContainer::ValType::CostType res2=DefaultStructs:: template NumberTypeBounds
//                    <typename EdgeContainer::ValType::CostType>::zero();
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
        return res;
    }


    // procedura o interfejsie j.w. ale wielomianowa
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CostType
    minCostFlowGT (
        const GraphType & g,
        EdgeContainer& edgeTab,
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
//        typename EdgeContainer::ValType::CostType res2=DefaultStructs:: template NumberTypeBounds
//                    <typename EdgeContainer::ValType::CostType>::zero();
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
        return res;
    }

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

    // objetosc przeplywu przy wierzcholku v
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType vertFlow (
        const GraphType & g, // badany graf
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z flow
        typename GraphType::PVertex v,
        EdgeDirection type=EdUndir)
        // type=EdDirOut - objetosc wyplywajaca, type=EdDirIn - objetosc wplywajaca, type=EdUndir - bilans wyplywajacy
    {
        assert(v && (type==EdDirOut ||type==EdDirIn ||type==EdUndir)); // TODO: throw
           const typename EdgeContainer::ValType::CapacType Zero=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
            typename EdgeContainer::ValType::CapacType sum=Zero;
            switch (type)
            {
                case EdDirOut:
                    for(typename GraphType::PEdge e=g.getEdge(v,EdDirOut|EdLoop);e;e=g.getEdgeNext(v,e,EdDirOut|EdLoop))
                    sum+=edgeTab[e].flow;
                    for(typename GraphType::PEdge e=g.getEdge(v,EdUndir);e;e=g.getEdgeNext(v,e,EdUndir))
                    if (g.getEdgeEnd1(e)==v) sum+=std::max(edgeTab[e].flow,Zero); else sum+=std::max(-edgeTab[e].flow,Zero);
                    return sum;
               case EdDirIn:
                    for(typename GraphType::PEdge e=g.getEdge(v,EdDirIn|EdLoop);e;e=g.getEdgeNext(v,e,EdDirIn|EdLoop))
                    sum+=edgeTab[e].flow;
                    for(typename GraphType::PEdge e=g.getEdge(v,EdUndir);e;e=g.getEdgeNext(v,e,EdUndir))
                    if (g.getEdgeEnd2(e)==v) sum+=std::max(edgeTab[e].flow,Zero); else sum+=std::max(-edgeTab[e].flow,Zero);
                    return sum;
            }
            return vertFlow(g,edgeTab,v,EdDirOut)-vertFlow(g,edgeTab,v,EdDirIn);
//            for(typename GraphType::PEdge e=g.getEdge(v,EdDirIn);e;e=g.getEdgeNext(v,e,EdDirIn))
//                sum-=edgeTab[e].flow;
//            for(typename GraphType::PEdge e=g.getEdge(v,EdDirOut);e;e=g.getEdgeNext(v,e,EdDirOut))
//                sum+=edgeTab[e].flow;
//            for(typename GraphType::PEdge e=g.getEdge(v,EdUndir);e;e=g.getEdgeNext(v,e,EdUndir))
//            if (g.getEdgeEnd1(e)==v) sum+=edgeTab[e].flow; else sum-=edgeTab[e].flow;
//        return sum;
    }


    // sprawdzanie poprawnosci przeplywu S->T
    template <class GraphType, class EdgeContainer>
    static bool testFlow (
        const GraphType & g, // badany graf
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac i  flow
        typename GraphType::PVertex S, typename GraphType::PVertex T)
    {   const typename EdgeContainer::ValType::CapacType Zero
                    = DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero();
        assert(S && T && S!=T); // TODO: throw
        for(typename GraphType::PEdge e=g.getEdge(EdUndir);e;e=g.getEdgeNext(e,EdUndir))
            if (std::max(edgeTab[e].flow,-edgeTab[e].flow)<Zero ||
                std::max(edgeTab[e].flow,-edgeTab[e].flow)>edgeTab[e].capac) return false;
        for(typename GraphType::PEdge e=g.getEdge(Directed|Loop);e;e=g.getEdgeNext(e,Directed|Loop))
            if (edgeTab[e].flow<Zero ||edgeTab[e].flow>edgeTab[e].capac) return false;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if (v!=S && v!=T && vertFlow(g,edgeTab,v)!=Zero) return false;
        return true;
    }

    // znajduje maksymalny przeplyw start->end (ale nie wiekszy, niz limit)
    // zwraca jego wielkosc
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CapacType
    maxFlow (
        const GraphType & g, // badany graf
        EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac (wymagane nieujemne)
                                // wyniki zwracane do pol flow
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType limit=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty()) // gorny limit wielkosci przeplywu
    {   // TODO: throw
        assert(start && end && limit>=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::zero());
        if (DefaultStructs::useFulkersonFord) return maxFlowFF(g,edgeTab,start,end,limit);
        else return maxFlowMKM(g,edgeTab,start,end,limit);
    }


    // zwraca koszt podanego przeplywu lub transship.
    template <class GraphType, class EdgeContainer>
    static typename EdgeContainer::ValType::CostType
    flowCost (
        const GraphType & g,// badany graf
        const EdgeContainer& edgeTab) // wejsciowa tablica asocjacyjna PEdge->EdgeLabs lub TrsEdgeLabss. Korzystamy z pol flow  i cost
    {   typename EdgeContainer::ValType::CostType res=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::zero();
                for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
                if (g.getEdgeType(e)==Undirected)
                    res+=std::max(edgeTab[e].flow,-edgeTab[e].flow)*edgeTab[e].cost;
                else res+=edgeTab[e].flow*edgeTab[e].cost;
    }

    // znajduje najtanszy przeplyw start->end o maks. wielkosci (ale nie wiekszy, niz val)
    // zwraca (jego koszt, wielkosc)
    template <class GraphType, class EdgeContainer>
    static std::pair<typename EdgeContainer::ValType::CostType,
                     typename EdgeContainer::ValType::CapacType>
    minCostFlow (
        const GraphType & g,// badany graf
        EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac  i cost (wymagane nieujemne, z wyjatkiem kosztow petli)
                                // wyniki zwracane do pol flow
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        typename EdgeContainer::ValType::CapacType val=// gorny limit wielkosci szukanego przeplywu
                    DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CapacType>::plusInfty())
    {   // TODO: throw
        assert(start);
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
            // TODO: throw
            assert(edgeTab[e].capac>=Zero);
        }
        for(typename GraphType::PEdge E=g.getEdge(EdDirIn | EdDirOut | EdUndir);E;
                                            E=g.getEdgeNext(E,EdDirIn | EdDirOut | EdUndir))
            // TODO: throw
            assert(edgeTab[E].cost >= DefaultStructs:: template NumberTypeBounds
                            <typename EdgeContainer::ValType::CostType>::zero());

        std::pair<typename EdgeContainer::ValType::CostType,typename EdgeContainer::ValType::CapacType> res;
        if (DefaultStructs::useCostFulkersonFord) res.second= minCostFlowFF(g,edgeTab,start,end,val);
        else res.second= minCostFlowGT(g,edgeTab,start,end,val);

        // dla petli o ujemnym koszcie przeplyw jest ustalany na przepustowosc
        for(typename GraphType::PEdge e=g.getEdge(EdLoop);e;e=g.getEdgeNext(e,EdLoop))
            if (edgeTab[e].cost<
                DefaultStructs:: template NumberTypeBounds
                <typename EdgeContainer::ValType::CostType>::zero())
                    edgeTab[e].flow=edgeTab[e].capac;
        // wyliczanie kosztu
        res.first=flowCost(g,edgeTab);
        return res;
    }

    // znajdowanie minimalnego (pod wzgledem objetosci) rozciecia krawedziowego start-end
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static EdgeCut<typename EdgeContainer::ValType::CapacType>
    minEdgeCut (
        const GraphType & g, // badany graf
        EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac
        typename GraphType::PVertex start, typename GraphType::PVertex end, // wierzcholki miedzy ktorymi rozcinamy
        OutPath<VIter,EIter> iters) // para iteratorow na wierzcholki i krawedzie wynikowe (por. opis EdgeCut)
    {
        EdgeCut<typename EdgeContainer::ValType::CapacType> res;
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

    // j.w. ale szuka najmniejszego rozciecia miedzy kazda para wierzcholkow (zwracane w polach first, second)
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static EdgeCut2<GraphType, typename EdgeContainer::ValType::CapacType>
    minEdgeCut (
        const GraphType & g,
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        OutPath<VIter,EIter> iters)
    {   assert(g.getVertNo()>=2); // TODO: throw
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

    // szuka transship. w grafie o podanych warunkach na wierzcholki i krawedzie
    // zwraca false w razie braku
    template <class GraphType, class EdgeContainer, class VertContainer>
    static bool
    transship (GraphType & g, // badany graf
               EdgeContainer& edgeTab, //  tablica asocjacyjna PEdge->TrsEdgeLabs. Korzystamy z pol wejsciowych hi i lo, wyniki trafiaja do flow
               const VertContainer& vertTab //  wejsciowa tablica asocjacyjna PVertex->TrsVertLoss.
               )
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
            {   assert(Zero==edgeTab[e].lo); // TODO: throw
            }

        if (DefaultStructs::ReserveOutAssocCont) edgeTab.reserve(g.getEdgeNo()+g.getVertNo());

        u=g.addVert();
        for(v=g.getVert();v;v=g.getVertNext(v))
            if (v!=u) edgeTab[g.addArch(v,u)]=
                TrsEdgeLabs<typename EdgeContainer::ValType::CapacType>(vertTab[v].lo,vertTab[v].hi);

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

    // szuka najtanszego transship. w grafie o podanych warunkach na wierzcholki i krawedzie
    // zwraca jego koszt lub nieskonczonosc w razie braku
    template <class GraphType, class EdgeContainer, class VertContainer>
    static typename EdgeContainer::ValType::CostType
    minCostTransship (GraphType & g, // badany graf
                      EdgeContainer& edgeTab, //  tablica asocjacyjna PEdge->TrsEdgeLabs. Korzystamy z pol wejsciowych hi, lo i cost; wyniki trafiaja do flow
                      const VertContainer& vertTab) //  wejsciowa tablica asocjacyjna PVertex->TrsVertLoss.
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
            {   assert(Zero==edgeTab[e].lo); // TODO: throw
            }

        if (DefaultStructs::ReserveOutAssocCont) edgeTab.reserve(g.getEdgeNo()+g.getVertNo());

        u=g.addVert();
        for(v=g.getVert();v;v=g.getVertNext(v))
            if (v!=u) edgeTab[g.addArch(v,u)]=
                TrsEdgeLabs<typename EdgeContainer::ValType::CapacType>(vertTab[v].lo,vertTab[v].hi);

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
        for(typename GraphType::PEdge e=g.getEdge(EdDirIn|EdDirOut|EdUndir);e;e=g.getEdgeNext(e,EdDirIn|EdDirOut|EdUndir))
             edgeTab[e].flow=edgeLabs[e].flow+edgeTab[e].lo;
//             res+=(g.getEdgeType(e)!=Undirected) ? edgeTab[e].flow*edgeTab[e].cost
//                            : std::max(edgeTab[e].flow,-edgeTab[e].flow)*edgeTab[e].cost;
        for(typename GraphType::PEdge e=g.getEdge(EdLoop);e;e=g.getEdgeNext(e,EdLoop))
            if (edgeTab[e].cost>=DefaultStructs:: template NumberTypeBounds
                    <typename EdgeContainer::ValType::CostType>::zero())
                edgeTab[e].flow=edgeTab[e].lo; else edgeTab[e].flow=edgeTab[e].hi;
//                res+=edgeTab[e].flow*edgeTab[e].cost;

        return flowCost(g,edgeTab);
    }

    // znajduje drzewo Gomory-Hu grafu
    template <class GraphType, class EdgeContainer, class IterOut>
    static void getGHTree(GraphType & g, // badany graf nieskierowany
                          EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac (wymagane nieujemne)
                IterOut out) // iterator wyjsciowy, na ktory wyrzucamy struktury GHTreeEdge "krawedzi" drzewa
    {   assert(g.getVertNo()>1); // TODO: throw
        GHTreeEdge<GraphType,typename EdgeContainer::ValType::CapacType> LOCALARRAY(buf,g.getVertNo());
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                Set<typename GraphType::PVertex> >::Type setMap(g.getVertNo());
        Set<typename GraphType::PVertex> V=g.getVertSet(),R=V;
        // TODO: if (DefaultStructs::ReserveOutAssocCont) edgeTab.reserve(???);
        ghtree(g,edgeTab,setMap,V,R,buf);
        for(int i=0;i<g.getVertNo()-1;i++) { *out=buf[i]; ++out; }
    }


};


// wersja dzialajaca na DefaultStructs=FlowAlgsDefaultSettings
template <bool FF, bool costFF>
class FlowFl : public FlowPar<FlowAlgsDefaultSettings<FF, costFF> > {};

// i z domyslnymi flagami wyboru algorytmow
class Flow : public FlowFl<false,true> {};



// Domyslne wytyczne dla procedur badania spojnosci grafu
class ConnectAlgsDefaultSettings : public FlowAlgsDefaultSettings<false, true> {
    public:

    // typ pomocniczej sieci lokalnej
    template <class A, class B> class LocalGraph {
        public:
        typedef Graph<A,B,GrDefaultSettings<EdDirIn|EdDirOut,false> > Type;

    };

};


// Procedury badania spojnosci grafu (bez wag na wierz/kraw)
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class ConnectPar : public PathStructs {

    public:

    // rekord wyjsciowy opisujacy rozciecie krawedziowe w grafie
    template <class GraphType> struct EdgeCut {

            typename GraphType::PVertex first, second; // dwa wierzcholki po obu stronach rozciecia
            int edgeNo; // liczba krawedzi

            EdgeCut() {}
    };


    protected:

    struct EdgeLabs {

        int capac,flow;

        EdgeLabs() : capac(1), flow(0)
            {}
    };

    // tworzy w ig pomocnicza siec na podstawie g. Mapa images laczy wierzcholki g z parami odpowiednich krawedzi w ig
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

    // znajduje najmniejsze rozciecie krawedziowe miedzy start->end
    // zwraca jego wielkosc, krawedzie sa wypisywane na iter
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

    // znajduje najmniejsze rozciecie krawedziowe miedzy para roznych wierzcholkow
    // krawedzie sa wypisywane na iter
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


    // znajduje najwiekszy zbior krawedziowo rozlaczych sciezek start->end
    // zwraca ich liczbe
    template <class GraphType, class VIter, class EIter, class LenIterV, class LenIterE>
    static int
    edgeDisjPaths( GraphType & g,// badany graf
                  typename GraphType::PVertex start, typename GraphType::PVertex end,
                  OutPath< VIter,EIter > iters, // iteratory wyjsciowe na kolejne wierz/kraw znalezionych sciezek
                  std::pair<LenIterV,LenIterE> liter) // iteratory na ktore wypisywane sa pozycje poczatkow kolejnych sciezek
                  // i laczna dlugosc ciagow (por. komentarz przy strukturze CompStore) z search.h
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

        *liter.first=0; ++liter.first;*liter.second=0; ++liter.second;
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
        int lv=0,le=0;
        for(r=0;r<res;r++)
        {   int j=BFSPar<DefaultStructs>:: template getPath(
                makeSubgraph(g,std::make_pair(stdChoose(true),extAssocChoose(&(paths),r))),
                start,end,outPath(vout,eout),EdDirOut);
            lv+=j + 1; le+=j; /*((vertsoutflag)?1:0);*/
            *liter.first=lv; *liter.second=le;++liter.first;++liter.second;
            if (!isBlackHole(iters.edgeIter))
                for(int k=0;k<j;k++) { *iters.edgeIter=eout[k]; ++iters.edgeIter;}
            if (!isBlackHole(iters.vertIter))
                for(int k=0;k<=j;k++) { *iters.vertIter=vout[k]; ++iters.vertIter;}
        }

        for(typename GraphType::PEdge e=undirs.firstKey();e;e=undirs.nextKey(e))
            g.moveEdge(e,undirs[e].first,undirs[e].second,EdUndir);
        le=paths.getKeys(euler);
        for(int i=0;i<le;i++) if (paths[euler[i]]==-1) g.delEdge(euler[i]);

        return res;
    }

    // znajduje najmniejsze rozciecie wierzcholkowe miedzy start->end
    // zwraca jego wielkosc (-1 w razie braku), wierzcholki sa wypisywane na iter
    template <class GraphType, class VIter>
    static int
    minVertCut (
        const GraphType & g,// badany graf
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        VIter iter) // iterator wyjsciowy
    {   assert(start && end && (start!=end) ); // TODO: throw
        if (g.getEdge(start,end,EdDirOut|EdUndir)) return -1;
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
        if (!isBlackHole(iter)) for(int i=0;i<res;i++)
        {   assert(icut[i]->info.first);
            *iter=icut[i]->info.first;++iter;
        }
        return res;
    }

    // znajduje najmniejsze rozciecie wierzcholkowe w grafie
    // zwraca jego wielkosc (-1 w razie braku), wierzcholki sa wypisywane na iter
    template <class GraphType, class VIter>
    static int
    minVertCut (const GraphType & g,VIter iter)
    {
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
                {   if (!isBlackHole(iter))
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

    // znajduje najwiekszy zbior wewnetrznie wierzcholkowo rozlaczych sciezek start->end
    // zwraca ich liczbe
    template <class GraphType, class VIter, class EIter, class LenIterV,class LenIterE>
    static int
    vertDisjPaths(const GraphType & g,// badany graf
                  typename GraphType::PVertex start, typename GraphType::PVertex end,
                  OutPath< VIter,EIter > iters, // iteratory wyjsciowe na kolejne wierz/kraw znalezionych sciezek
                  std::pair<LenIterV,LenIterE> liter)// iterator na ktory wypisywane sa pozycje poczatkow kolejnych sciezek
                  // i laczna dlugosc ciagow (por. komentarz przy strukturze CompStore) z search.h
//                  bool vertsoutflag=false)
    {   assert(start && end && start!=end);//TODO: throw
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
                                                                                std::make_pair(blackHole,impos));
        *liter.first=0; ++liter.first;*liter.second=0; ++liter.second;
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
            /*if (vertsoutflag) */
            { *liter.first=vpos; ++liter.first; }
            /*else*/
            { *liter.second=epos; ++liter.second; }
        }
        // wszystkie krawedzie start->end sa tez traktowane jako szukane sciezki
        res+=g.getEdgeNo(start,end,EdDirOut|EdUndir);
        for(typename GraphType::PEdge e=g.getEdge(start,end,EdDirOut|EdUndir);e;
                e=g.getEdgeNext(start,end,e,EdDirOut|EdUndir))
        {   *iters.vertIter=start; ++iters.vertIter;++vpos;
            *iters.edgeIter=e; ++iters.edgeIter;++epos;
            *iters.vertIter=end; ++iters.vertIter;++vpos;
//            if (vertsoutflag)
            { *liter.first=vpos; ++liter.first; }
//            else
            { *liter.second=epos; ++liter.second; }
        }

        return res;
    }


};

// wersja dzialajaca na DefaultStructs=ConnectAlgsDefaultSettings
class Connect : public ConnectPar<ConnectAlgsDefaultSettings> {};


}

#endif

