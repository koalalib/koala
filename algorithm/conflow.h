#ifndef KOALA_DEF_FLOWS_H
#define KOALA_DEF_FLOWS_H

/* conflow.h
 *
 */

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <vector>
#include <utility>

#include "../algorithm/search.h"
#include "../base/defs.h"
#include "../graph/view.h"

namespace Koala
{

    /* FlowAlgsDefaultSettings
     * Domyslne wytyczne dla procedur przeplywowych:
     * czy do wyznaczania maks. przeplywu uzywac FulkersonaForda, czy MKM
     */
    template< bool FF = false > class FlowAlgsDefaultSettings: public AlgsDefaultSettings
    {
      public:
        enum { useFulkersonFord = FF };
    };


    /* FlowPar
     * Algorytmy znajdowania przeplywu, najwiekszego przeplywu, najtanszego przeplywu, rozciec, transhipments itd.
     * DefaultStructs - wytyczne dla wewnetrznych procedur
     */
    template< class DefaultStructs > class FlowPar: public PathStructs
    {
      public:
        // rekord z danymi (in-out) opisujacy krawedz
        template< class DType, class CType = DType > struct EdgeLabs
        {
            // typ liczbowy przepustowosci luku i objetosci przeplywu
            typedef DType CapacType;
            // typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
            typedef CType CostType;

            // przepustowosc (dana wejsciowa), wartosc znalezionego przeplywu w kierunku getEdgeEnd1->getEdgeEnd2
            CapacType capac,flow;
            // koszt jednostkowy przeplywu przez luk (dana wejsciowa)
            CostType cost;

            // agrs: przepustowosc i koszt krawedzi
            EdgeLabs():
                    capac(DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    flow( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    cost( DefaultStructs:: template NumberTypeBounds< CostType >::zero() )
                { }
            EdgeLabs( CapacType arg):
                    capac( arg ), flow( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    cost(DefaultStructs:: template NumberTypeBounds< CostType >::zero())
                { }
            EdgeLabs( CapacType arg, CostType arg2):
                    capac( arg ), flow( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    cost( arg2 )
                { }
        };

        // j.w. ale nadaje domyslne jednostkowe przepustowosci i koszty
        template< class DType, class CType = DType > struct UnitEdgeLabs
        {
            // typ liczbowy przepustowosci luku i objetosci przeplywu
            typedef DType CapacType;
            // typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
            typedef CType CostType;

            // przepustowosc (dana wejsciowa), wartosc znalezionego przeplywu w kierunku getEdgeEnd1->getEdgeEnd2
            CapacType capac,flow;
            // koszt jednostkowy przeplywu przez luk (dana wejsciowa)
            CostType cost;

            // agrs: przepustowosc i koszt krawedzi
            UnitEdgeLabs():
                    capac(DefaultStructs:: template NumberTypeBounds< CapacType >::one() ),
                    flow( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    cost( DefaultStructs:: template NumberTypeBounds< CostType >::one() )
                { }
            UnitEdgeLabs( CapacType arg):
                    capac( arg ), flow( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    cost(DefaultStructs:: template NumberTypeBounds< CostType >::one())
                { }
            UnitEdgeLabs( CapacType arg, CostType arg2):
                    capac( arg ), flow( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    cost( arg2 )
                { }
        };

        // rekord wynikowy opisujacy rozciecie krawedziowe w grafie miedzy para wierzcholkow
        template< class CapacType > struct EdgeCut
        {
            // typ liczbowy przepustowosci luku i objetosci przeplywu
            CapacType capac;
            // liczba wierzcholkow osiagalnych z poczatkowego po usunieciu rozciecia
            int vertNo;
            // liczba krawedzi rozciecia
            int edgeNo;

            EdgeCut():  capac( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                        vertNo( 0 ), edgeNo( 0 )
                { }
        };

        // j.w. ale podaje rowniez 2 wierzcholki - z obu czesci rozcietego grafu
        template< class GraphType, class CapacType > struct EdgeCut2: public EdgeCut< CapacType >
        {
            typename GraphType::PVertex first,second;
        };

        // Do odczytu rozciecia krawedziowego miedzy para wierzcholkow
        // Uzytkownik podaje, pare iteratorow, gdzie wpisac krawedzie znalezionego rozciecia
        // i wierzcholki osiagalne ze startowego po usunieciu krawedzi rozciecia
        template< class VIter, class EIter > struct OutCut
        {
            VIter vertIter;
            EIter edgeIter;

            OutCut( VIter av, EIter ei ): vertIter( av ), edgeIter( ei ) { }
        };

        // funkcja tworzaca, analogia make_pair
        // Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
        template< class VIter, class EIter > static OutCut< VIter,EIter > outCut( VIter av, EIter ei )
            { return OutCut< VIter,EIter >( av,ei ); }

        // "krawedz" drzewa Gomory-Hu
        template< class GraphType, class CType > struct GHTreeEdge
        {
            // typ liczbowy przepustowosci luku
            typedef CType CapacType;
            // koncowki krawedzi
            typename GraphType::PVertex first,second;
            // przepustowosc "krawedzi"
            CapacType capac;

            // args: oba wierzcholki koncowe i przepustowosc
            GHTreeEdge( ): first( 0 ), second( 0 ),
                capac( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() )
                { }
            GHTreeEdge( typename GraphType::PVertex f ): first( f ), second( 0 ),
                capac( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() )
                { }
            GHTreeEdge( typename GraphType::PVertex f, typename GraphType::PVertex s, CapacType c  ):
                first( f ), second( s ), capac( c )
                { }

        };

        // Transshipment - uogolnienie przeplywu (por. Schrijver)
        // rekord wejsciowy opisujacy wierzcholek dla wyszukiwania transhipmentow
        template< class DType > struct TrsVertLoss
        {
            // typ liczbowy przepustowosci luku i objetosci przeplywu
            typedef DType CapacType;
            // maksymalny i minimalny dopuszczalny przy danym wierzcholku laczny wplyw
            CapacType hi,lo;

            //(dopuszczalne dodatnie i ujemne np. przeplyw to trans. z hi=lo=0 wszedzie poza zrodlem i ujsciem)
            TrsVertLoss():
                hi(DefaultStructs:: template NumberTypeBounds< CapacType >::zero()),
                lo(DefaultStructs:: template NumberTypeBounds< CapacType >::zero())
                { }
            TrsVertLoss( CapacType alo):
                hi(DefaultStructs:: template NumberTypeBounds< CapacType >::zero()),
                lo( alo )
                { }
            TrsVertLoss( CapacType alo, CapacType ahi): hi( ahi ), lo( alo )
                 { }
        };

        // rekord  opisujacy krawedz dla wyszukiwania transhipmentow
        template< class DType, class CType = DType > struct TrsEdgeLabs
        {
            // typ liczbowy przepustowosci luku i objetosci przeplywu
            typedef DType CapacType;
            // typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
            typedef CType CostType;
            // wymagane gorne i dolne ograniczenie na wielkosc przeplywu przez ta krawedz.
            //Dla nieskierowanych wymaga sie lo=0
            // TODO: sprawdzic, czy moga byc ujemne dla lukow
            CapacType hi,lo;
            // wartosc znalezionego przeplywu (transship.) w kierunku getEdgeEnd1->getEdgeEnd2
            CapacType flow;
            // koszt jednostkowy przeplywu dla luku, jesli wymagany to wymagany nieujemny z wyjatkiem petli
            CostType cost;

            TrsEdgeLabs():
	            hi(DefaultStructs:: template NumberTypeBounds< CapacType >::zero()),
	            lo(DefaultStructs:: template NumberTypeBounds< CapacType >::zero()),
                    cost(DefaultStructs:: template NumberTypeBounds< CostType >::zero())
                    { }
            TrsEdgeLabs( CapacType alo):
	            hi(DefaultStructs:: template NumberTypeBounds< CapacType >::zero()),
	            lo( alo ),
                    cost(DefaultStructs:: template NumberTypeBounds< CostType >::zero())
                    { }
            TrsEdgeLabs( CapacType alo, CapacType ahi):
	            hi( ahi ),
		    lo( alo ),
                    cost(DefaultStructs:: template NumberTypeBounds< CostType >::zero())
                    { }
            TrsEdgeLabs( CapacType alo,
                CapacType ahi,
                CostType c): hi( ahi ), lo( alo ), cost( c )
                    { }
        };

      protected:
        // rekord pomocniczy opisujacy wierzcholek
        template< class GraphType, class CapacType > struct VertLabs
        {
            // do BFS w sieci tymczasowej
            int distance,backdist;
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge  ePrev;

            // potencjaly przesylu (na uzytek maxFlowMKM)
            CapacType mass,inPot,outPot;
            // czy wierzcholek jest w sieci warstwowej (na uzytek maxFlowMKM)
            bool used;

            VertLabs( typename GraphType::PVertex pv = 0, typename GraphType::PEdge pe = 0,
                int d = std::numeric_limits< int >::max(), int bd = std::numeric_limits< int >::max() ):
                    distance( d ), backdist( bd ),vPrev( pv ), ePrev( pe ),
                    mass( DefaultStructs:: template NumberTypeBounds< CapacType >::zero() ),
                    inPot( DefaultStructs:: template NumberTypeBounds< CapacType >::plusInfty() ),
                    outPot( DefaultStructs:: template NumberTypeBounds< CapacType >::plusInfty() ),
                    used( false )
                    { }
        };

        // pomocnicza etykieta wierzcholka na uzytek alg. Dijkstry w sieci tymczasowej
        template< class GraphType, class CostType > struct VertLabsCost
        {
            CostType distance,pot;
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge  ePrev;

            VertLabsCost():
                distance( DefaultStructs:: template NumberTypeBounds< CostType >::plusInfty() ),
                pot( DefaultStructs:: template NumberTypeBounds< CostType >::zero() ),
                vPrev( 0 ), ePrev( 0 )
                { }
        };

        // przepustowosc uzyteczna krawedzi w sieci tymczasowej z danymi przeplywami
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            usedCap( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PEdge e,
                typename GraphType::PVertex v, bool out );
        // zwiekszanie przeplywu przez krawedz e od delta w kierunku od v lub do v (zmienna out)
        template< class GraphType, class EdgeContainer > static void
            addFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PEdge e,
                typename GraphType::PVertex v, typename EdgeContainer::ValType::CapacType delta, bool out );
        // BFS w sieci tymczasowej przy danych przepustowosciach i przeplywie
        // Sekwencja wyjsciowa wierzcholkow idzie na iter
        // Poruszamy sie "do przodu" lub "do tylu" (out) odpowiednio uzywajac pol distance lub backdistance rekodru
        template< class GraphType, class VertContainer, class EdgeContainer, class Iter > static bool
            BFSFlow( const GraphType &g, EdgeContainer &edgeTab, VertContainer &visited,
                typename GraphType::PVertex first, typename GraphType::PVertex last, bool out, Iter &iter );
        // pomocnicza dla maxFlowMKM
        // identyfikacja wierzcholkow warstw miedzy first a last sieci tymczasowej (pola used, distance, backdistane)
        // zwraca: czy jest przejscie first->last
        template< class GraphType, class VertContainer, class EdgeContainer, class Iter > static bool
            layers( const GraphType &g, EdgeContainer &edgeTab, VertContainer &visited,
                typename GraphType::PVertex first, typename GraphType::PVertex last, Iter &iterout );
        // pomocnicza dla maxFlowMKM
        // wyznaczanie potencjalow przeplywu wierzcholkow w sieci warstwowej miedzy ends
        template< class GraphType, class VertContainer, class EdgeContainer > static void
            findPot( const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab,
                typename GraphType::PVertex fends,typename GraphType::PVertex sends,
                typename GraphType::PVertex v, bool pin, bool pout );
        // na uzytek maxFlowMKM
        // przepchniecie przeplywu z wierzcholka v do przodu lub do tylu
        template< class GraphType, class VertContainer, class EdgeContainer > static void
            push( const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab, typename GraphType::PVertex v,
                bool front );
        // na uzytek maxFlowMKM
        template< class GraphType, class VertContainer, class EdgeContainer > static
            typename EdgeContainer::ValType::CapacType onevert( const GraphType &g, EdgeContainer &edgeTab,
                VertContainer &vertTab, typename GraphType::PVertex *tab, int size,
                typename EdgeContainer::ValType::CapacType limit );
        // na uzytek MKM, znajduje maksymalny (ale nie przekraczajacy limit) przeplyw miedzy start a end
        template< class GraphType, class EdgeContainer, class VertContainer > static
            typename EdgeContainer::ValType::CapacType layerFlow( const GraphType &g, EdgeContainer &edgeTab,
            VertContainer &vertTab, typename GraphType::PVertex start, typename GraphType::PVertex end,
            typename EdgeContainer::ValType::CapacType limit );
        // Znajdowanie maksymalnego (ale nie przekraczajacego limit) przeplywu miedzy start a end
        // Algorytm MKM
        // Zwraca wielkosc przeplywu
        // Parametry: badany graf, tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac (wymagane nieujemne),
        // wyniki zwracane do pol flow. gorny limit wielkosci znalezionego przeplywu
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            maxFlowMKM( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType limit );
        // Znajdowanie maksymalnego (ale nie przekraczajacego limit) przeplywu miedzy start a end
        // Algorytm FulkersonaFodra
        // Zwraca wielkosc przeplywu. Parametry: badany graf, tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol
        // capac (wymagane nieujemne), wyniki zwracane do pol flow, gorny limit wielkosci znalezionego przeplywu
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            maxFlowFF( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType limit );
        // podobna do usedCap na uzytek przeplywow z kosztem
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            usedCapCost( const GraphType &g, EdgeContainer& edgeTab, typename GraphType::PEdge e,
                typename GraphType::PVertex v );
        // koszt jednostkowy przeplywu przez krawedz od danej koncowki w sieci tymczasowej
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CostType
            costFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PEdge e,
                typename GraphType::PVertex v );

        // Korekta kosztu luku na uzytek Dijkstry z potencjalami wierzcholkow (nowe koszty nie wprowadzaja ujemnych cykli)
        template< class GraphType, class EdgeContainer, class VertContainer >
            static typename EdgeContainer::ValType::CostType costFlowDij( const GraphType &g, EdgeContainer &edgeTab,
                VertContainer& vertTab, typename GraphType::PEdge e, typename GraphType::PVertex v )
            {
                return costFlow( g,edgeTab,e,v ) + vertTab[v].pot - vertTab[g.getEdgeEnd( e,v )].pot;
            }
        // Dijkstra w sieci tymczasowej z kosztami lukow skorygowanymi przez potencjaly wierzcholkow
        // Zwraca: czy istnieje przejscie start->end
        // TODO: rozwazyc przejscie na kopce
        template< class GraphType, class VertContainer, class EdgeContainer > static bool
            DijkstraFlow( const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab,
                typename GraphType::PVertex start, typename GraphType::PVertex end );
        // znajdowanie przeplywu start->end o maksymalnej objetosci (ale nie przekraczajacej limitu val) i najmniejszym koszcie
        // procedura pseudowielomianowa (sciezki powiekszajace)
        // zwraca pare wielkosc przeplywu
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            minCostFlowFF( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType val = DefaultStructs::
                template NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty() );
        // TODO: nieefektywna, zrezygnowac z Setow
        template< class GraphType, class EdgeContainer, class AssocSub >
            static void ghtree( GraphType &g, EdgeContainer &edgeTab,
                Set< typename GraphType::PVertex > &V, Set< typename GraphType::PVertex > &R,
                GHTreeEdge< GraphType,typename EdgeContainer::ValType::CapacType > *out, AssocSub& vsub );

      public:
        // objetosc przeplywu przy wierzcholku v
        // type=EdDirOut - objetosc wyplywajaca, type=EdDirIn - objetosc wplywajaca, type=EdUndir - bilans wyplywajacy
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            vertFlow( const GraphType &g, const EdgeContainer &edgeTab, typename GraphType::PVertex v,
                EdgeDirection type = EdUndir );
        // sprawdzanie poprawnosci przeplywu S->T
        template< class GraphType, class EdgeContainer > static bool testFlow( const GraphType &g,
            const EdgeContainer &edgeTab, typename GraphType::PVertex S, typename GraphType::PVertex T );
        // znajduje maksymalny przeplyw start->end (ale nie wiekszy, niz limit)
        // zwraca jego wielkosc
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            maxFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end)
            {
		        return maxFlow(g, edgeTab, start, end, DefaultStructs::
                        template NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty() );
            }
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
            maxFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType limit);
        // zwraca koszt podanego przeplywu lub transship.
        template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CostType
            flowCost( const GraphType &g, const EdgeContainer &edgeTab );
        // znajduje najtanszy przeplyw start->end o maks. wielkosci (ale nie wiekszy, niz val)
        // zwraca (jego koszt, wielkosc)
        template< class GraphType, class EdgeContainer > static
            std::pair< typename EdgeContainer::ValType::CostType,typename EdgeContainer::ValType::CapacType >
            minCostFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end)
            {
                return minCostFlow(g, edgeTab, start, end, DefaultStructs::
                         template NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty() );
            }
        template< class GraphType, class EdgeContainer > static
            std::pair< typename EdgeContainer::ValType::CostType,typename EdgeContainer::ValType::CapacType >
            minCostFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType val);
        // znajdowanie minimalnego (pod wzgledem objetosci) rozciecia krawedziowego start-end
        template< class GraphType, class EdgeContainer, class VIter, class EIter > static
            EdgeCut< typename EdgeContainer::ValType::CapacType > minEdgeCut( const GraphType &g, EdgeContainer &edgeTab,
                typename GraphType::PVertex start, typename GraphType::PVertex end, OutCut< VIter,EIter > iters )
                // Implementacja przeniesiona do czesci definicyjnej ze wzgledu na bledy kompilatorow VS <2010
                {
                    EdgeCut< typename EdgeContainer::ValType::CapacType > res;
                    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
                        VertLabs< GraphType,typename EdgeContainer::ValType::CapacType > >::Type vertTab( g.getVertNo() );
                    res.capac = maxFlow( g,edgeTab,start,end );
                    BFSFlow( g,edgeTab,vertTab,start,end,true,blackHole );
                    for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
                        if (std::numeric_limits< int >::max() > vertTab[v].distance)
                        {
                            res.vertNo++;
                            if (!isBlackHole( iters.vertIter ))
                            {
                                *iters.vertIter = v;
                                ++iters.vertIter;
                            }
                            for( typename GraphType::PEdge e = g.getEdge( v,EdDirOut | EdUndir ); e;
                                e = g.getEdgeNext( v,e,EdDirOut | EdUndir ) )
                                if (vertTab[g.getEdgeEnd( e,v )].distance == std::numeric_limits< int >::max())
                                {
                                    res.edgeNo++;
                                    if (!isBlackHole( iters.edgeIter ))
                                    {
                                        *iters.edgeIter = e;
                                        ++iters.edgeIter;
                                    }
                            }
                        }
                    return res;
                }
        // j.w. ale szuka najmniejszego rozciecia miedzy kazda para wierzcholkow (zwracane w polach first, second)
        template< class GraphType, class EdgeContainer, class VIter, class EIter > static
            EdgeCut2< GraphType,typename EdgeContainer::ValType::CapacType > minEdgeCut( const GraphType &g,
                EdgeContainer &edgeTab, OutCut< VIter,EIter > iters )
                // Implementacja przeniesiona do czesci definicyjnej ze wzgledu na bledy kompilatorow VS <2010
                {
                    int n,m;
                    koalaAssert( g.getVertNo() >= 2,AlgExcWrongArg );
                    EdgeCut< typename EdgeContainer::ValType::CapacType > res,buf;
                    typename GraphType::PVertex a,b;
                    typename GraphType::PVertex LOCALARRAY( vres,(n = g.getVertNo()) - 1 );
                    typename GraphType::PVertex LOCALARRAY( vbuf,n - 1 );
                    typename GraphType::PEdge LOCALARRAY( eres,m = g.getEdgeNo(Directed | Undirected) );
                    typename GraphType::PEdge LOCALARRAY( ebuf,m );
                    res.capac = DefaultStructs:: template NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty();

                    for( typename GraphType::PVertex s = g.getVert(); s != g.getVertLast(); s = g.getVertNext( s ) )
                        for( typename GraphType::PVertex t = g.getVertNext( s ); t; t = g.getVertNext( t ) )
                        {
                            if (isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
                                buf = minEdgeCut( g,edgeTab,s,t,outCut( blackHole,blackHole ) );
                            else if (isBlackHole( iters.vertIter ) && !isBlackHole( iters.edgeIter ))
                                buf = minEdgeCut( g,edgeTab,s,t,outCut( blackHole,ebuf ) );
                            else if (!isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
                                buf = minEdgeCut( g,edgeTab,s,t,outCut( vbuf,blackHole ) );
                            else buf = minEdgeCut( g,edgeTab,s,t,outCut( vbuf,ebuf ) );
                            if (buf.capac < res.capac)
                            {
                                res = buf;
                                a = s;
                                b = t;
                                if (!isBlackHole( iters.vertIter ))
                                    for( int i = 0; i < buf.vertNo; i++ ) vres[i] = vbuf[i];
                                if (!isBlackHole( iters.edgeIter ))
                                    for( int i = 0; i < buf.edgeNo; i++ ) eres[i] = ebuf[i];
                            }
                            if (g.getEdgeNo( EdDirIn | EdDirOut ))
                            {
                                if (isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
                                    buf = minEdgeCut( g,edgeTab,t,s,outCut( blackHole,blackHole ) );
                                else if (isBlackHole( iters.vertIter ) && !isBlackHole( iters.edgeIter ))
                                    buf = minEdgeCut( g,edgeTab,t,s,outCut( blackHole,ebuf ) );
                                else if (!isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
                                    buf = minEdgeCut( g,edgeTab,t,s,outCut( vbuf,blackHole ) );
                                else buf = minEdgeCut( g,edgeTab,t,s,outCut( vbuf,ebuf ) );
                                if (buf.capac < res.capac)
                                {
                                    res = buf;
                                    a = t;
                                    b = s;
                                    if (!isBlackHole( iters.vertIter ))
                                        for( int i = 0; i < buf.vertNo; i++ ) vres[i] = vbuf[i];
                                    if (!isBlackHole( iters.edgeIter ))
                                        for( int i = 0; i < buf.edgeNo; i++ ) eres[i] = ebuf[i];
                                }
                            }
                        }
                    if (!isBlackHole( iters.vertIter ))
                        for( int i = 0; i < res.vertNo; i++ )
                        {
                            *iters.vertIter = vres[i];
                            ++iters.vertIter;
                        }
                    if (!isBlackHole( iters.edgeIter ))
                        for( int i = 0; i < res.edgeNo; i++ )
                        {
                            *iters.edgeIter = eres[i];
                            ++iters.edgeIter;
                        }
                    EdgeCut2< GraphType,typename EdgeContainer::ValType::CapacType > res2;
                    res2.capac = res.capac;
                    res2.edgeNo = res.edgeNo;
                    res2.vertNo = res.vertNo;
                    res2.first = a;
                    res2.second = b;
                    return res2;
                }
        // szuka transship. w grafie o podanych warunkach na wierzcholki i krawedzie
        // zwraca false w razie braku
        template< class GraphType, class EdgeContainer, class VertContainer > static bool transship( GraphType &g,
            EdgeContainer &edgeTab, const VertContainer &vertTab );
        // szuka najtanszego transship. w grafie o podanych warunkach na wierzcholki i krawedzie
        // zwraca jego koszt lub nieskonczonosc w razie braku
        template< class GraphType, class EdgeContainer, class VertContainer > static
            typename EdgeContainer::ValType::CostType minCostTransship( GraphType &g, EdgeContainer &edgeTab,
                const VertContainer &vertTab );
        // znajduje drzewo Gomory-Hu grafu
        // TODO: niezbyt efektywne, moze zrezygnowac z Setow
        template< class GraphType, class EdgeContainer, class IterOut > static void findGHTree( GraphType &g,
            EdgeContainer& edgeTab, IterOut out );
    };

    // wersja dzialajaca na DefaultStructs=FlowAlgsDefaultSettings
    template< bool FF > class FlowFl: public FlowPar< FlowAlgsDefaultSettings< FF > > { };

    // i z domyslnyna flaga wyboru algorytmow
    class Flow: public FlowFl< false > { };

    /* ConnectPar
     * Procedury badania spojnosci grafu (bez wag na wierz/kraw)
     * DefaultStructs - wytyczne dla wewnetrznych procedur
     */
    template< class DefaultStructs > class ConnectPar: public SearchStructs
    {
      public:
        // rekord wyjsciowy opisujacy rozciecie krawedziowe w grafie
        template< class GraphType > struct EdgeCut
        {
            // dwa wierzcholki po obu stronach rozciecia
            typename GraphType::PVertex first,second;
            // liczba krawedzi
            int edgeNo;

            EdgeCut()
                { }
        };

      protected:

        struct EdgeLabs
        {
            int capac,flow;

            EdgeLabs(): capac( 1 ), flow( 0 )
                { }
        };

        // tworzy w ig pomocnicza siec na podstawie g. Mapa images laczy wierzcholki g z parami odpowiednich krawedzi w ig
        template< class GraphType, class ImageType, class Linker >
            static void makeImage( const GraphType &g, ImageType &ig, Linker &images );

      public:
        // znajduje najmniejsze rozciecie krawedziowe miedzy start->end
        // zwraca jego wielkosc, krawedzie sa wypisywane na iter
        template< class GraphType, class EIter > static int minEdgeCut( const GraphType &g,
            typename GraphType::PVertex start, typename GraphType::PVertex end, EIter iter );
        // znajduje najmniejsze rozciecie krawedziowe miedzy para roznych wierzcholkow
        // krawedzie sa wypisywane na iter
        template< class GraphType, class EIter > static EdgeCut< GraphType > minEdgeCut( const GraphType &g, EIter iter )
        // Implementacja przeniesiona do czesci definicyjnej ze wzgledu na bledy kompilatorow VS <2010
        {
            EdgeCut< GraphType > res;
            typename DefaultStructs:: template AssocCont< typename GraphType::PEdge,
                typename FlowPar< DefaultStructs >:: template EdgeLabs< int > >::Type edgeLabs( g.getEdgeNo() );
            for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) ) edgeLabs[e].capac = 1;
            typename FlowPar< DefaultStructs >:: template EdgeCut2< GraphType,int > res2 =
                FlowPar< DefaultStructs >:: template minEdgeCut( g,edgeLabs,FlowPar< DefaultStructs >::template
                    outCut( blackHole,iter ) );
            res.edgeNo = res2.capac;
            res.first = res2.first;
            res.second = res2.second;
            return res;
        }
        // znajduje najwiekszy zbior krawedziowo rozlaczych sciezek start->end
        // zwraca ich liczbe. Parametry: badany graf,
        // iteratory wyjsciowe na kolejne wierz znalezionych sciezek
        // iteratory wyjsciowe na kolejne kraw znalezionych sciezek
        template< class GraphType, class VIter, class EIter, class LenIterV, class LenIterE > static int
            edgeDisjPaths( GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
            CompStore< LenIterV,VIter > voutiter, CompStore< LenIterE,EIter > eoutiter );
        // znajduje najmniejsze rozciecie wierzcholkowe miedzy start->end
        // zwraca jego wielkosc (-1 w razie braku), wierzcholki sa wypisywane na iter
        template< class GraphType, class VIter > static int minVertCut( const GraphType &g,
            typename GraphType::PVertex start, typename GraphType::PVertex end, VIter iter );
        // znajduje najmniejsze rozciecie wierzcholkowe w grafie
        // zwraca jego wielkosc (-1 w razie braku), wierzcholki sa wypisywane na iter
        template< class GraphType, class VIter > static int minVertCut( const GraphType &g, VIter iter );
        // znajduje najwiekszy zbior wewnetrznie wierzcholkowo rozlaczych sciezek start->end
        // zwraca ich liczbe
        template< class GraphType, class VIter, class EIter, class LenIterV, class LenIterE > static int
            vertDisjPaths( const GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
            CompStore< LenIterV,VIter > voutiter, CompStore< LenIterE,EIter > eoutiter );
    };

    // wersja dzialajaca na DefaultStructs=FlowAlgsDefaultSettings<false, true>
    class Connect: public ConnectPar< FlowAlgsDefaultSettings< false > > { };

#include "conflow.hpp"
}

#endif
