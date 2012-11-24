#ifndef KOALA_DEF_WEIGHTS_H
#define KOALA_DEF_WEIGHTS_H

/* weights.h
 *
 */

#include "../algorithm/search.h"
#include "../base/defs.h"
#include "../container/heap.h"
#include "../container/joinsets.h"
#include "../graph/view.h"

namespace Koala
{
    /* DijkstraBasePar
     *
     */
    template< class DefaultStructs > class DijkstraBasePar: public ShortPathStructs
    {
      public:
        // rekord wejsciowy opisujacy krawedz
        template< class DType > struct EdgeLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType DistType;
            // dlugosc krawedzi
            DistType length;
        };

        // rekord wyjsciowy opisujacy wierzcholek
        template< class DType, class GraphType > struct VertLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType DistType;
            // znaleziona odleglosc
            DType distance;
            // element sciezki, analogicznie jak VisitVertLabs w search.h
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge ePrev;

            VertLabs():
                distance( DefaultStructs::template NumberTypeBounds< DType >::plusInfty() ),
                vPrev( 0 ), ePrev( 0 )
                { }

            template< class Rec > void copy( Rec &rec ) const;
        };

        // wlasciwa procedura: odleglosc miedzy para wierzcholkow
        // avertTab, wyjsciowa tablica asocjacyjna PVertex->VertLabs poszczegolnych wierzcholkow
        // edgeTab, wejsciowa tablica asocjacyjna PEdge->EdgeLabs dlugosci krawedzi
        template< class GraphType, class VertContainer, class EdgeContainer > static
            typename EdgeContainer::ValType::DistType distances( const GraphType &g, VertContainer &avertTab,
                const EdgeContainer &edgeTab, typename GraphType::PVertex start, typename GraphType::PVertex end = 0 );

        // korzystajac z vertTab wyliczonej  metoda distances odczytuje najkrotsza sciezke prowadzaca do end
        // vertTab, tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat distance
        // end, wierzcholek docelowy
        // iters, iteratory do zapisu sciezki
        // zwraca liczbe krawedzi sciezki lub -1 gdy  wierzcholek end jest nieosiagalny
        template< class GraphType, class VertContainer, class VIter, class EIter > static int
            getPath( const GraphType &g, const VertContainer& vertTab, typename GraphType::PVertex end,
                ShortPathStructs::OutPath< VIter,EIter > iters );

      protected:

        // Dijkstra na kopcu
        template< typename Container > struct Cmp
        {
            Container *cont;
            Cmp( Container &acont ): cont( &acont )
                { }

            template< class T > bool operator()( T a, T b ) const
                { return cont->operator[]( a ).distance < cont->operator[]( b ).distance; }
        };

        template< typename Container > static Cmp< Container > makeCmp( Container &acont )
            { return Cmp< Container >( acont ); }

        template< class DType, class GraphType > struct VertLabsQue: public VertLabs< DType,GraphType >
        {
            void* repr;
            VertLabsQue(): VertLabs< DType,GraphType >(), repr( 0 )
                { }
        };

        template< class GraphType, class VertContainer, class EdgeContainer >
            static typename EdgeContainer::ValType::DistType distancesOnHeap( const GraphType &g,
                VertContainer &avertTab, const EdgeContainer &edgeTab, typename GraphType::PVertex start,
                typename GraphType::PVertex end = 0 );
    };

    /* DijkstraHeapBasePar
     *
     */
    template< class DefaultStructs > class DijkstraHeapBasePar: public DijkstraBasePar< DefaultStructs >
    {
      public:
        template< class GraphType, class VertContainer, class EdgeContainer >
            static typename EdgeContainer::ValType::DistType distances( const GraphType &g, VertContainer &avertTab,
            const EdgeContainer &edgeTab, typename GraphType::PVertex start, typename GraphType::PVertex end = 0 )
            { return DijkstraBasePar< DefaultStructs >::distancesOnHeap( g,avertTab,edgeTab,start,end ); }
    };

    /* DijkstraMainPar
     *
     */
    template< class DefaultStructs, class DijBase > class DijkstraMainPar: public DijBase
    {
      public:
        // Rekord wyjsciowy zawierajacy od razu dlugosc najkr. sciezki i jej liczbe krawedzi
        template< class DistType > struct PathLengths
        {
            DistType length;
            int edgeNo;

            PathLengths( DistType alen, int ano ): length( alen ), edgeNo( ano )
                { }
            PathLengths()
                { }
        };

        // zapisuje od razu sciezke  start->end (wierzcholki i krawedzie) pod pare podanych iteratorow
        // Znajduje wszystko w jedym etapie
        template< class GraphType, class EdgeContainer, class VIter, class EIter > static
            PathLengths< typename EdgeContainer::ValType::DistType > findPath( const GraphType& g,
                const EdgeContainer &edgeTab, typename GraphType::PVertex start, typename GraphType::PVertex end,
                ShortPathStructs::OutPath< VIter,EIter > iters )
                {
                    koalaAssert( start && end,AlgExcNullVert );
                    const typename EdgeContainer::ValType::DistType PlusInfty = DefaultStructs:: template
                	NumberTypeBounds< typename EdgeContainer::ValType::DistType >::plusInfty();

                    typename EdgeContainer::ValType::DistType dist;
                    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,typename DijBase::
                	template VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type vertTab( g.getVertNo() );

                    dist = DijBase::distances( g,vertTab,edgeTab,start,end );

                    if (PlusInfty == dist)
                	return PathLengths< typename EdgeContainer::ValType::DistType >( dist,-1 ); // end nieosiagalny

                    int len = DijBase::getPath( g,vertTab,end,iters );
                    return PathLengths< typename EdgeContainer::ValType::DistType >( dist,len );
                    // dlugosc najkr. siezki i jej liczba krawedzi
                }
    };

    /* DijkstraPar
     * Algorytm Dijkstry na prostej tablicy
     */
    template< class DefaultStructs > class DijkstraPar:
        public DijkstraMainPar< DefaultStructs,DijkstraBasePar< DefaultStructs > > { };

    /* DijkstraHeapPar
     * Algorytm Dijkstry na kopcu
     */
    template< class DefaultStructs > class DijkstraHeapPar:
        public DijkstraMainPar< DefaultStructs,DijkstraHeapBasePar< DefaultStructs > > { };

    // wersje dzialajaca na DefaultStructs=AlgsDefaultSettings
    class Dijkstra: public DijkstraPar< AlgsDefaultSettings > { };
    class DijkstraHeap: public DijkstraHeapPar< AlgsDefaultSettings > { };

    /* DAGCritPathPar
     * najdluzsze sciezki w DAGu z wagami na krawedziach
     */
    template< class DefaultStructs > class DAGCritPathPar: public ShortPathStructs
    {
      public:
        // rekord wejsciowy opisujacy krawedz
        template< class DType > struct EdgeLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType DistType;
            // dlugosc krawedzi
            DistType length;
        };

        // rekord wyjsciowy opisujacy wierzcholek
        template< class DType, class GraphType > struct VertLabs
        {
            // typ wagi liczbowej na krawedzi`
            typedef DType DistType;
            // znaleziona odleglosc
            DType distance;
            // element sciezki, analogicznie jak VisitVertLabs w search.h
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge  ePrev;

            VertLabs():
                distance( DefaultStructs:: template NumberTypeBounds< DType >::minusInfty() ),
                vPrev( 0 ), ePrev( 0 )
                { }
        };

        // mozna stosowac jako kontener opisujacy krawedz w przypadkach, gdy chcemy wsystkim krawedziom nadac wagi jednostkowe
        template< class DType > struct UnitLengthEdges
        {
            struct  ValType
            {
                typedef DType DistType;
                DistType length;
            };

            template< class T > ValType operator[]( T e ) const;
        };

        // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
        // start=NULL - szukamy najdluzszych sciezek w grafie o dowolnym poczatku
        // zwraca przy podanym end : maks. dlugosc sciezki start->end lub -niesk. gdy end jest nieosiagalny
        // przy end=NULL zwraca 0
        template< class GraphType, class VertContainer, class EdgeContainer > static
            typename EdgeContainer::ValType::DistType critPathLength( const GraphType &g, VertContainer &avertTab,
                const EdgeContainer &edgeTab, typename GraphType::PVertex start, typename GraphType::PVertex end = 0 );

        // korzystajac z vertTab wyliczonej poprzednia procedura odczytuje najdluzsza sciezke prowadzaca do end
        template< class GraphType, class VertContainer, class VIter, class EIter > static int getPath(
            GraphType &g, const VertContainer &vertTab, typename GraphType::PVertex end,
            ShortPathStructs::OutPath< VIter,EIter > iters );

        // Rekord wyjsciowy zawierajacy od razu dlugosc najdluzszej sciezki i jej liczbe krawedzi
        template< class DistType > struct PathLengths
        {
            DistType length;
            int edgeNo;

            PathLengths( DistType alen, int ano ): length( alen ), edgeNo( ano )
                { }
            PathLengths()
                { }
        };

        // zapisuje od razu sciezke krytyczna (wierzcholki i krawedzie) pod pare podanych iteratorow
        // Znajduje wszystko w jedym etapie
        template< class GraphType, class EdgeContainer, class VIter, class EIter > static
            PathLengths< typename EdgeContainer::ValType::DistType > findPath( const GraphType &g,
                const EdgeContainer& edgeTab, typename GraphType::PVertex start, typename GraphType::PVertex end,
                ShortPathStructs::OutPath< VIter,EIter > iters )
                {
                    const typename EdgeContainer::ValType::DistType MinusInfty = DefaultStructs:: template
                	NumberTypeBounds< typename EdgeContainer::ValType::DistType >::minusInfty();

                    typename EdgeContainer::ValType::DistType dist;
                    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,VertLabs< typename
                	EdgeContainer::ValType::DistType,GraphType > >::Type vertTab( g.getVertNo() );

                    if (MinusInfty == (dist = critPathLength( g,vertTab,edgeTab,start,end )))
                	return PathLengths< typename EdgeContainer::ValType::DistType >( dist,-1 ); // end nieosiagalny

                    int len = getPath( g,vertTab,end,iters );
                    return PathLengths< typename EdgeContainer::ValType::DistType >( dist,len );
                }
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class DAGCritPath: public DAGCritPathPar< AlgsDefaultSettings > { };

    /* BellmanFordPar
     *
     */
    template< class DefaultStructs > class BellmanFordPar: public ShortPathStructs
    {
      public:
        // rekord wejsciowy opisujacy krawedz
        template< class DType > struct EdgeLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType DistType;
            // dlugosc krawedzi
            DistType length;
        };

        // rekord wyjsciowy opisujacy wierzcholek
        template< class DType, class GraphType > struct VertLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType DistType;
            // znaleziona odleglosc
            DType distance;
            // element sciezki, analogicznie jak VisitVertLabs w search.h
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge ePrev;

            VertLabs():
                distance( DefaultStructs:: template NumberTypeBounds< DType >::plusInfty() ),
                vPrev( 0 ), ePrev( 0 )
                { }
        };

        // wlasciwa procedura: odleglosc miedzy para wierzcholkow
        // zwraca przy podanym end : min. dlugosc sciezki start->end lub niesk. gdy end jest nieosiagalny
        // przy end=NULL zwraca 0, zas do avertTab zapisuje m. in. odleglosci wierzcholkow od start
        // w razie wykrycia ujemnego cyklu zwraca -niesk.
        template< class GraphType, class VertContainer, class EdgeContainer > static
            typename EdgeContainer::ValType::DistType distances( const GraphType &g, VertContainer &avertTab,
            const EdgeContainer &edgeTab, typename GraphType::PVertex start, typename GraphType::PVertex end = 0 );

        // wlasciwa procedura: zapisuje najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow,
        // zwraca liczbe krawedzi najkrotszej sciezki. Korzysta z kontenera vertTab z danymi z poprzedniej funkcji
        // zwraca liczbe krawedzi sciezki, -1 jesli end jest nieosiagalny, -2 w razie wykrycia ujemnego cyklu
        template< class GraphType, class VertContainer, class VIter, class EIter > static int getPath(
            const GraphType &g, VertContainer &vertTab, typename GraphType::PVertex end,
            ShortPathStructs::OutPath< VIter,EIter > iters );

        // Rekord wyjsciowy zawierajacy od razu dlugosc najkr. sciezki i jej liczbe krawedzi
        template< class DistType > struct PathLengths
        {
            DistType length;
            int edgeNo;

            PathLengths( DistType alen, int ano ): length( alen ), edgeNo( ano )
                { }
            PathLengths()
                { }
        };

        // zapisuje od razu sciezke krytyczna (wierzcholki i krawedzie) pod pare podanych iteratorow
        // Znajduje wszystko w jedym etapie
        // zwraca rekord PathLengths z parametrami sciezki (dlugosc najdl siezki i jej liczba krawedzi)
        // lub (niesk,-1) jesli end jest nieosiagalny, a (-niesk,-2) w razie wykrycia ujemnego cyklu
        template< class GraphType, class EdgeContainer, class VIter, class EIter > static
            PathLengths< typename EdgeContainer::ValType::DistType > findPath( const GraphType &g,
                const EdgeContainer &edgeTab, typename GraphType::PVertex start, typename GraphType::PVertex end,
                ShortPathStructs::OutPath< VIter,EIter > iters )
                {
                    koalaAssert( start && end,AlgExcNullVert );
                    typename EdgeContainer::ValType::DistType dist;
                    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
                	VertLabs< typename EdgeContainer::ValType::DistType,GraphType > >::Type vertTab( g.getVertNo() );

                    if (DefaultStructs:: template NumberTypeBounds< typename EdgeContainer::ValType::DistType >
                	::isPlusInfty(dist = distances( g,vertTab,edgeTab,start,end )))
                	return PathLengths< typename EdgeContainer::ValType::DistType >( dist,-1 ); // end nieosiagalny
                    else if (DefaultStructs:: template NumberTypeBounds< typename EdgeContainer::ValType::DistType >
                	::isMinusInfty( dist ))
                	return PathLengths< typename EdgeContainer::ValType::DistType >( dist,-2 ); // w grafie jest cykl ujemny

                    int len = getPath( g,vertTab,end,iters );
                    return PathLengths< typename EdgeContainer::ValType::DistType >( dist,len );
                    // dlugosc najkr. siezki i jej liczba krawedzi
                }
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class BellmanFord: public BellmanFordPar< AlgsDefaultSettings > { };

    /* FloydPar
     * algorytm liczy najkrotsza sciezke pomiedzy kazda para wierzcholków zostal zaproponowany przez Floyda i oparty na twierdzeniu Warshalla)
     */
    template< class DefaultStructs > class FloydPar: public PathStructs
    {
      protected:
        template< class GraphType, class TwoDimVertContainer, class VIter, class EIter > static int
            getOutPathFromMatrix( const GraphType &g, const TwoDimVertContainer &vertMatrix,
                OutPath< VIter,EIter > iters, typename GraphType::PVertex start, typename GraphType::PVertex end );

      public:
        // rekord wejsciowy opisujacy krawedz
        template< class DType > struct EdgeLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType DistType;
            // dlugosc krawedzi
            DistType length;
        };

        // rekord wyjsciowy opisujacy wierzcholek
        template< class DType, class GraphType > struct VertLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType DistType;
            // znaleziona odleglosc
            DType distance;
            // element sciezki, analogicznie jak VisitVertLabs w search.h
            typename GraphType::PVertex vPrev;
            typename GraphType::PEdge ePrev;

            VertLabs():
                    distance( DefaultStructs:: template NumberTypeBounds< DType >::plusInfty() ),
                    vPrev( 0 ), ePrev( 0 )
                    { }
        };

        // mozna stosowac jako kontener opisujacy krawedz w przypadkach, gdy chcemy wsystkim krawedziom nadac wagi jednostkowe
        template< class DType > struct UnitLengthEdges
        {
            struct  ValType
            {
                typedef DType DistType;
                DistType length;
            };

            template< class T > ValType operator[]( T e ) const;
        };

        // wlasciwa procedura: odleglosc miedzy kazda para wierzcholkow
        // false - wykryto ujemny cykl, wowczas wyniki z vertMatrix nie nadaja sie do uzytku
        template< class GraphType, class TwoDimVertContainer, class EdgeContainer > static bool distances(
            const GraphType &g, TwoDimVertContainer &vertMatrix, const EdgeContainer &edgeTab );

        // wlasciwa procedura: zapisuje najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow,
        // zwraca liczbe krawedzi najkrotszej sciezki lub -1 gdy end jest nieosiagalny. Korzysta z kontenera vertMatrix z danymi z poprzedniej funkcji
        template< class GraphType, class TwoDimVertContainer, class VIter, class EIter > static int getPath(
            const GraphType &g, const TwoDimVertContainer &vertMatrix, typename GraphType::PVertex start,
            typename GraphType::PVertex end, PathStructs::OutPath< VIter,EIter > iters );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class Floyd: public FloydPar< AlgsDefaultSettings > { };

    /* KruskalPar
     * najlzejsze lub najciezsze lasy w grafie
     */
    template< class DefaultStructs > class KruskalPar
    {
      public:
        // rekord wejsciowy opisujacy krawedz
        template< class DType > struct EdgeLabs
        {
            // typ wagi liczbowej na krawedzi
            typedef DType WeightType;
            // wagakrawedzi
            WeightType weight;
        };

        template< class DType > struct Result
        {
            // waga znalezionego lasu
            DType weight;
            // jego liczba krawedzi
            int edgeNo;
        };

      protected:
        template< class GraphType, class EdgeContainer, class Iter, class VertCompContainer > static
            Result< typename EdgeContainer::ValType::WeightType > getForest( const GraphType &g,
                const EdgeContainer &edgeTab, Iter out, VertCompContainer &asets, int edgeNo, bool minWeight )
                {
                    JoinableSets< typename GraphType::PVertex,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
                	JSPartDesrc< typename GraphType::PVertex > *>::Type > localSets;
                    typename BlackHoleSwitch< VertCompContainer,JoinableSets< typename GraphType::PVertex,
                	typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
                	JSPartDesrc< typename GraphType::PVertex > *>::Type > >::Type &sets =
                	    BlackHoleSwitch< VertCompContainer,JoinableSets< typename GraphType::PVertex,
                	    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
                	    JSPartDesrc< typename GraphType::PVertex> *>::Type > >::get( asets,localSets );

                    Result< typename EdgeContainer::ValType::WeightType > res;
                    res.edgeNo = 0;
                    res.weight = DefaultStructs:: template NumberTypeBounds< typename EdgeContainer::ValType::WeightType >::zero();
                    const EdgeDirection mask = Directed | Undirected;
                    int n,m = g.getEdgeNo( mask );
                    sets.resize( n = g.getVertNo() );
                    if (n == 0) return res;
                    for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ))
                	sets.makeSinglet( v );

                    edgeNo = (edgeNo >= 0) ? edgeNo : n-1;
                    if (m == 0|| edgeNo == 0) return res;

                    std::pair< typename EdgeContainer::ValType::WeightType,typename GraphType::PEdge > LOCALARRAY( edges,m );
                    int i = 0;
                    typename GraphType::PEdge e;
                    for( e = g.getEdge( mask ); e != NULL; e = g.getEdgeNext( e,mask ) )
                	edges[i++] = std::make_pair( edgeTab[e].weight,e );
                    DefaultStructs::sort( edges,edges + m );
                    if (!minWeight) std::reverse( edges,edges + m );

                    for( i = 0; i < m && edgeNo > 0; i++ )
                    {
                	std::pair< typename GraphType::PVertex,typename GraphType::PVertex > ends;
                	e = edges[i].second;
                	ends = g.getEdgeEnds( e );
                	if (sets.getSetId( ends.first ) != sets.getSetId( ends.second ))
                	{
                	    res.weight = res.weight + edgeTab[e].weight;
                	    res.edgeNo++;
                	    sets.join( ends.first,ends.second );
                	    *out = e;
                	    ++out;
                	    edgeNo--;
                	}
                    }

                    return res;
                }

      public:
        // znajduje najlzejszy las
        // g, badany graf, petle sa ignorowane a luki i kr. nieskierowane traktowane jednakow
        // edgeTab, wejsciowa tablica asocjacyjna PEdge->EdgeLabs wag krawedzi
        // out, iterator do zapisu ciagu krawedzi lasu
        // asets, wynikowa struktura JoinableSets<PVertex> ze skladowymi spojnosci znalezionego lasu (lub BlackHole)
        // edgeNo, limit liczby krawedzi - znaleziony las bedzie mial najwieksza mozliwa liczbe krawedzi nie przekraczajaca tego parametru
        // pominiecie parametru - znaleziony las bedzie mial najwieksza mozliwa liczbe krawedzi
        template< class GraphType, class EdgeContainer, class Iter, class VertCompContainer > static
            Result< typename EdgeContainer::ValType::WeightType > getMinForest( const GraphType &g,
                const EdgeContainer &edgeTab, Iter out, VertCompContainer &asets, int edgeNo = -1 )
            { return getForest( g,edgeTab,out,asets,edgeNo,true ); }

        // znajduje najciezszy las
        template< class GraphType, class EdgeContainer, class Iter, class VertCompContainer > static
            Result< typename EdgeContainer::ValType::WeightType > getMaxForest( const GraphType &g,
                const EdgeContainer &edgeTab, Iter out, VertCompContainer &asets, int edgeNo = -1 )
            { return getForest( g,edgeTab,out,asets,edgeNo,false ); }
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class Kruskal: public KruskalPar< AlgsDefaultSettings > { };
#include "weights.hpp"
}

#endif

