#ifndef KOALA_DEF_WEIGHTS_H
#define KOALA_DEF_WEIGHTS_H

#include "../base/def_struct.h"
//#include "../graph/graph.h"
#include "../graph/view.h"
#include "../algorithm/search.h"
#include "../container/joinsets.h"
#include "../container/heap.h"




namespace Koala {


template <class DefaultStructs>
class DijkstraBasePar : public ShortPathStructs {
    public:

    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {

        typedef DType DistType; // typ wagi liczbowej na krawedzi
        DistType length;    // dlugosc krawedzi
    };

    // rekord wyjsciowy opisujacy wierzcholek
    template <class DType, class GraphType> struct VertLabs {

        typedef DType DistType; // typ wagi liczbowej na krawedzi
        DType distance; // znaleziona odleglosc
        typename GraphType::PVertex vPrev;  // element sciezki, analogicznie jak VisitVertLabs w search.h
        typename GraphType::PEdge  ePrev;

        VertLabs() : vPrev(0), ePrev(0),
        distance(DefaultStructs:: template NumberTypeBounds<DType>::plusInfty()) {}

        template <class Rec>
            void copy(Rec& rec) const { rec.distance=distance; rec.vPrev=vPrev; rec.ePrev=ePrev; }
    };

    // wlasciwa procedura: odleglosc miedzy para wierzcholkow
    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    distances (
        const GraphType & g,
        VertContainer& avertTab, // wyjsciowa tablica asocjacyjna PVertex->VertLabs poszczegolnych wierzcholkow
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {   koalaAssert(start,AlgExcNullVert);
        const typename EdgeContainer::ValType::DistType Zero=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        const typename EdgeContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();
        int n;
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab, Q(n=g.getVertNo());
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;
        if (DefaultStructs::ReserveOutAssocCont || isBlackHole(avertTab)) vertTab.reserve(n);

        Q[start].vPrev=0;Q[start].ePrev=0;
        Q[start].distance=Zero;

        while(!Q.empty()){
            typename EdgeContainer::ValType::DistType
                    d=PlusInfty,nd;
            for(V=Q.firstKey();V;V=Q.nextKey(V)) if (Q[V].distance<d) d=Q[U=V].distance;
            Q[U].copy(vertTab[U]);
            Q.delKey(U);
            if (U==end) return vertTab[end].distance;

            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirOut|Koala::EdUndir);
                E;E=g.getEdgeNext(U,E,Koala::EdDirOut|Koala::EdUndir))
                    if (!vertTab.hasKey(V=g.getEdgeEnd(E,U)))
                        if ((nd=vertTab[U].distance+edgeTab[E].length)<Q[V].distance)
                    { Q[V].distance=nd; Q[V].ePrev=E; Q[V].vPrev=U; }
        }

        return end ? PlusInfty : Zero;
    }


    template <class GraphType, class VertContainer, class VIter, class EIter>
    // korzystajac z vertTab wyliczonej  metoda distances odczytuje najkrotsza sciezke prowadzaca do end
    static int
        getPath(
        const GraphType& g,
        const VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat distance
        typename GraphType::PVertex end, // wierzcholek docelowy
        ShortPathStructs::OutPath<VIter,EIter> iters) // iteratory do zapisu sciezki
        // zwraca liczbe krawedzi sciezki lub -1 gdy  wierzcholek end jest nieosiagalny
    {   koalaAssert(end,AlgExcNullVert);
        const typename VertContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename VertContainer::ValType::DistType>::plusInfty();

        if (PlusInfty==vertTab[end].distance)
            return -1; // wierzcholek end jest nieosiagalny

        return ShortPathStructs::getOutPath(g,vertTab,iters,end);
    }

    protected:

    // Dijkstra na kopcu

    template< typename Container > struct Cmp        {            Container *cont;
            Cmp(Container& acont) : cont(&acont) {}

            template <class T> bool operator()  ( T a, T b ) const
            {   return cont->operator[](a).distance<cont->operator[](b).distance; }        } ;

    template < typename Container >
    static Cmp<Container> makeCmp(Container& acont) { return Cmp<Container>(acont); }

    template <class DType, class GraphType> struct VertLabsQue : public  VertLabs <DType,GraphType>
    {
        void* repr;
        VertLabsQue() : VertLabs<DType,GraphType>(), repr(0) {}
    };


    template <class GraphType, class VertContainer, class EdgeContainer,
        template <class Key,class Comparator,class Allocator> class Heap, template <class _Key> class Block>
    static typename EdgeContainer::ValType::DistType
    distancesOnHeap (
        const GraphType & g,
        VertContainer& avertTab, // tablica asocjacyjna z VertLabs poszczegolnych wierzcholkow
        const EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {    koalaAssert(start,AlgExcNullVert);
        const typename EdgeContainer::ValType::DistType Zero=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        const typename EdgeContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();
        int n;
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab;
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabsQue<typename EdgeContainer::ValType::DistType ,GraphType> >::Type Q(n=g.getVertNo());
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;

        if (DefaultStructs::ReserveOutAssocCont || isBlackHole(avertTab)) vertTab.reserve(n);

        Privates::BlockListAllocator<Block<typename GraphType::PVertex> > alloc(n);
        Heap<typename GraphType::PVertex,Cmp<typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabsQue<typename EdgeContainer::ValType::DistType ,GraphType> >::Type>,
                Privates::BlockListAllocator< Block<typename GraphType::PVertex> > > heap(&alloc,makeCmp(Q));

        Q[start].vPrev=0;Q[start].ePrev=0;
        Q[start].distance=Zero;
        Q[start].repr=heap.push(start);

        while(!Q.empty()){
            typename EdgeContainer::ValType::DistType d,nd;
//            for(V=Q.firstKey();V;V=Q.nextKey(V)) if (Q[V].distance<d) d=Q[U=V].distance;
            U=heap.top(); d=Q[U].distance;
//            vertTab[U]=Q[U];
            Q[U].copy(vertTab[U]);
            heap.del(( Block<typename GraphType::PVertex>*)Q[U].repr);
            Q.delKey(U);
            if (U==end) return vertTab[end].distance;

            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirOut|Koala::EdUndir);
                E;E=g.getEdgeNext(U,E,Koala::EdDirOut|Koala::EdUndir))
                    if (!vertTab.hasKey(V=g.getEdgeEnd(E,U)))
                        if ((nd=vertTab[U].distance+edgeTab[E].length)<Q[V].distance)
                        {   if (Q[V].repr) heap.del(( Block<typename GraphType::PVertex>*)Q[V].repr);
                            Q[V].distance=nd; Q[V].ePrev=E; Q[V].vPrev=U;
                            Q[V].repr=heap.push(V);
                        }
        }

        return end ? PlusInfty : Zero;
    }

};


template <class DefaultStructs>
class DijkstraHeapBasePar : public DijkstraBasePar<DefaultStructs> {

    public:

    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    distances (
        const GraphType & g,
        VertContainer& avertTab, // wyjsciowa tablica asocjacyjna PVertex->VertLabs poszczegolnych wierzcholkow
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {
        return DijkstraBasePar<DefaultStructs>::template distancesOnHeap
                <GraphType,VertContainer,EdgeContainer,BinomHeap,BinomHeapNode>
                (g,avertTab,edgeTab,start,end);
    }

};


template <class DefaultStructs>
class DijkstraFibonBasePar : public DijkstraBasePar<DefaultStructs> {

    public:

    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    distances (
        const GraphType & g,
        VertContainer& avertTab, // wyjsciowa tablica asocjacyjna PVertex->VertLabs poszczegolnych wierzcholkow
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {
        return DijkstraBasePar<DefaultStructs>::template distancesOnHeap
                <GraphType,VertContainer,EdgeContainer,FibonHeap,FibonHeapNode>
                (g,avertTab,edgeTab,start,end);
    }

};


template <class DefaultStructs, class DijBase>
class DijkstraMainPar : public DijBase {

    public:

    // Rekord wyjsciowy zawierajacy od razu dlugosc najkr. sciezki i jej liczbe krawedzi
    template <class DistType> struct PathLengths {
        DistType length;
        int edgeNo;

        PathLengths(DistType alen, int ano) : length(alen), edgeNo(ano) {}
        PathLengths() {}
    };


    // zapisuje od razu sciezke  start->end (wierzcholki i krawedzie) pod pare podanych iteratorow
    // Znajduje wszystko w jedym etapie
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static PathLengths<typename EdgeContainer::ValType::DistType>
        findPath(
        const GraphType& g, // badany graf
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters) // para iteratorow do zapisu sciezki
        // zwraca rekord PathLengths z parametrami sciezki (dlugosc najkr. siezki i jej liczba krawedzi)
        // lub pare (niesk,-1) jesli end jest nieosiagalny
    {   koalaAssert(start && end,AlgExcNullVert);
        const typename EdgeContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();

        typename EdgeContainer::ValType::DistType dist;
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                typename DijBase:: template VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab(g.getVertNo());

        dist=DijBase::distances(g,vertTab,edgeTab,start,end);

        if (PlusInfty==dist)
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=DijBase::getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};


// Algorytm Dijkstry na prostej tablicy
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DijkstraPar : public DijkstraMainPar<DefaultStructs,DijkstraBasePar<DefaultStructs> > {};


// Algorytm Dijkstry na kopcu
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DijkstraHeapPar : public DijkstraMainPar<DefaultStructs,DijkstraHeapBasePar<DefaultStructs> > {};


// Algorytm Dijkstry na kopcu Fibonacziego
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DijkstraFibonPar : public DijkstraMainPar<DefaultStructs,DijkstraFibonBasePar<DefaultStructs> > {};


// wersje dzialajaca na DefaultStructs=AlgsDefaultSettings
class Dijkstra : public DijkstraPar<AlgsDefaultSettings> {};
class DijkstraHeap : public DijkstraHeapPar<AlgsDefaultSettings> {};
class DijkstraFibon : public DijkstraFibonPar<AlgsDefaultSettings> {};


// najdluzsze sciezki w DAGu z wagami na krawedziach
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DAGCritPathPar : public ShortPathStructs {


    public:

    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {

        typedef DType DistType; // typ wagi liczbowej na krawedzi
        DistType length;    // dlugosc krawedzi
    };


    // rekord wyjsciowy opisujacy wierzcholek
    template <class DType, class GraphType> struct VertLabs {

        typedef DType DistType; // typ wagi liczbowej na krawedzi
        DType distance; // znaleziona odleglosc
        typename GraphType::PVertex vPrev; // element sciezki, analogicznie jak VisitVertLabs w search.h
        typename GraphType::PEdge  ePrev;

        VertLabs() : vPrev(0), ePrev(0),
                    distance(DefaultStructs:: template NumberTypeBounds<DType>::minusInfty()) {}
    };


    // mozna stosowac jako kontener opisujacy krawedz w przypadkach, gdy chcemy wsystkim krawedziom nadac wagi jednostkowe
    template <class DType> struct UnitLengthEdges {
        struct  ValType {
            typedef DType DistType;
            DistType length;
        };

        template <class T> ValType operator[](T e) const
        { ValType res; res.length=DefaultStructs:: template NumberTypeBounds<DType>::one();
        return res; }
    };


    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    critPathLength (
        const GraphType & g, // badany graf, powinien byc DAGiem, krawedzie inne niz luki sa ignorowane
        VertContainer& avertTab, // wynikowa tablica asocjacyjna PVertex->VertLabs poszczegolnych wierzcholkow (lub BlackHole)
        const EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs tj. dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    // start=NULL - szukamy najdluzszych sciezek w grafie o dowolnym poczatku
    // zwraca przy podanym end : maks. dlugosc sciezki start->end lub -niesk. gdy end jest nieosiagalny
    // przy end=NULL zwraca 0
    {
        const typename EdgeContainer::ValType::DistType Zero=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        const typename EdgeContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();
        const typename EdgeContainer::ValType::DistType MinusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::minusInfty();

        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;
        typename EdgeContainer::ValType::DistType nd;
        int ibeg,iend,n=g.getVertNo();

        if (DefaultStructs::ReserveOutAssocCont || isBlackHole(avertTab)) vertTab.reserve(n);

        if (start)
        {
            vertTab[start].vPrev=0;vertTab[start].ePrev=0;
            vertTab[start].distance=Zero;
            if (start==end) return Zero;
        }


        typename DefaultStructs::template AssocCont<typename GraphType::PVertex, char >
                ::Type followers(start ? n : 0);
        typename GraphType::PVertex LOCALARRAY(tabV,n);
        if (start)
        {   Koala::BFSPar<DefaultStructs>::scanAttainable(g,start,assocInserter(followers,constFun<char>(0)),EdDirOut);
            Koala::DAGAlgsPar<DefaultStructs>::topOrd(makeSubgraph(g,std::make_pair(assocKeyChoose(followers),stdChoose(true))),tabV);
            ibeg=1;iend=followers.size();
        } else
        {   Koala::DAGAlgsPar<DefaultStructs>::topOrd(g,tabV);
            ibeg=0; iend=n;
        }


        for(int i=ibeg;i<iend;i++) {
            U=tabV[i];
            vertTab[U].vPrev=0;vertTab[U].ePrev=0;
            vertTab[U].distance=(g.getEdgeNo(U,EdDirIn)) ? MinusInfty : Zero;

            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirIn);
                E;E=g.getEdgeNext(U,E,Koala::EdDirIn))
                {   V=g.getEdgeEnd(E,U);
                    if ((!start) || followers.hasKey(V))
                        if ((nd=vertTab[V].distance+edgeTab[E].length)>vertTab[U].distance)
                    { vertTab[U].distance=nd; vertTab[U].ePrev=E; vertTab[U].vPrev=V;  }
                }
            if (U==end) return vertTab[U].distance;
        }

        return end ? MinusInfty : Zero;
    }


    template <class GraphType, class VertContainer, class VIter, class EIter>
    // korzystajac z vertTab wyliczonej poprzednia procedura odczytuje najdluzsza sciezke prowadzaca do end
    static int
        getPath(
        GraphType& g,
        const VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
        typename GraphType::PVertex end, // wierzcholek docelowy
        ShortPathStructs::OutPath<VIter,EIter> iters) // iteratory do zapisu sciezki
        // zwraca liczbe krawedzi sciezki lub -1 gdy  wierzcholek end jest nieosiagalny
    {   koalaAssert(end,AlgExcNullVert);
        if (DefaultStructs:: template NumberTypeBounds<typename VertContainer::ValType::DistType>
                        ::isMinusInfty(vertTab[end].distance))
            return -1;

        return ShortPathStructs::getOutPath(g,vertTab,iters,end);
    }


    // Rekord wyjsciowy zawierajacy od razu dlugosc najdluzszej sciezki i jej liczbe krawedzi
    template <class DistType> struct PathLengths {
        DistType length;
        int edgeNo;

        PathLengths(DistType alen, int ano) : length(alen), edgeNo(ano) {}
        PathLengths() {}
    };

    // zapisuje od razu sciezke krytyczna (wierzcholki i krawedzie) pod pare podanych iteratorow
    // Znajduje wszystko w jedym etapie
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static PathLengths<typename EdgeContainer::ValType::DistType>
        findPath(
        const GraphType& g,
        const EdgeContainer& edgeTab, // jak w critPathLength
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters) // iteratory do zapisania sciezki
        // zwraca rekord PathLengths z parametrami sciezki (dlugosc najdl siezki i jej liczba krawedzi)
        // lub (-nisk,-1) jesli end jest nieosiagalny
    {
        const typename EdgeContainer::ValType::DistType MinusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::minusInfty();

        typename EdgeContainer::ValType::DistType dist;
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab(g.getVertNo());

        if (MinusInfty==(dist=critPathLength(g,vertTab,edgeTab,start,end)))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);

    }

};

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class DAGCritPath : public DAGCritPathPar<AlgsDefaultSettings> {};



template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class BellmanFordPar : public ShortPathStructs {

    public:

    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {

        typedef DType DistType;  // typ wagi liczbowej na krawedzi
        DistType length;  // dlugosc krawedzi
    };

    // rekord wyjsciowy opisujacy wierzcholek
    template <class DType, class GraphType> struct VertLabs {

        typedef DType DistType; // typ wagi liczbowej na krawedzi
        DType distance;  // znaleziona odleglosc
        typename GraphType::PVertex vPrev;  // element sciezki, analogicznie jak VisitVertLabs w search.h
        typename GraphType::PEdge  ePrev;

        VertLabs() : vPrev(0), ePrev(0),
                    distance(DefaultStructs:: template NumberTypeBounds<DType>::plusInfty()) {}
    };

    // wlasciwa procedura: odleglosc miedzy para wierzcholkow
    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    distances (
        const GraphType & g, // badany graf
        VertContainer& avertTab, // wynikowa tablica asocjacyjna PVertex->VertLabs poszczegolnych wierzcholkow (lub BlackHole)
        const EdgeContainer& edgeTab, // tablica asocjacyjna PEdge->EdgeLabs tj. dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // start=NULL - szukamy najdluzszych sciezek w grafie o dowolnym poczatku
    // zwraca przy podanym end : min. dlugosc sciezki start->end lub niesk. gdy end jest nieosiagalny
    // przy end=NULL zwraca 0
    // w razie wykrycia ujemnego cyklu zwraca -niesk.

    {   koalaAssert(start,AlgExcNullVert);
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;

        const typename EdgeContainer::ValType::DistType
                inf=DefaultStructs:: template NumberTypeBounds
                                    <typename EdgeContainer::ValType::DistType>::plusInfty();
        const typename EdgeContainer::ValType::DistType
                zero=DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::DistType> ::zero();
        const typename EdgeContainer::ValType::DistType
                minusInf=DefaultStructs:: template NumberTypeBounds
                                        <typename EdgeContainer::ValType::DistType> ::minusInfty();
        typename EdgeContainer::ValType::DistType nd;
          int n=g.getVertNo();
        if (DefaultStructs::ReserveOutAssocCont || isBlackHole(avertTab)) vertTab.reserve(n);

        bool existNegCycle = false;


        //sprawdzenie czy nie ma petli ujemnych
        for(typename GraphType::PEdge E=g.getEdge(Koala::EdLoop|Koala::EdUndir);E;E=g.getEdgeNext(E, Koala::EdLoop|Koala::EdUndir))
            if (edgeTab[E].length < zero)
            {
//                for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
//                    vertTab[v].distance=minusInf;
                return minusInf;
            }

        //inicjalizacja
        //for each v: d[v] <- INF (to jest zrealizowane juz przy tworzeniu vertTab)
        //f[s] <- NIL
        vertTab[start].vPrev=0;vertTab[start].ePrev=0;
        //d[s] <- 0
        vertTab[start].distance=zero;

        //for 1 to n-1:
        //  for each (u,v):
        //      if  d[u]+w(u,v) < d[v]:
        //          d[v] <- d[u]+w(u,v) and vPrev[v] <- u and ePrev[v] <- (u,v)

        for(int i=1;i<n;i++){
            //relaksacja krawedzi nieskierowanych
            for(typename GraphType::PEdge E=g.getEdge(Koala::EdUndir); E; E=g.getEdgeNext(E, Koala::EdUndir)){
                if (vertTab[U=g.getEdgeEnd1(E)].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnd2(E)].distance)
                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
                else if (vertTab[U=g.getEdgeEnd2(E)].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnd1(E)].distance)
                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
            }
            //relaksacja krawedzi (u,v) skierowanych u->v
            for(typename GraphType::PEdge E=g.getEdge(Koala::EdDirOut); E; E=g.getEdgeNext(E, Koala::EdDirOut))
                if (vertTab[U=g.getEdgeEnd1(E)].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnd2(E)].distance)
                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
        }

        //sprawdzenie czy nie ma cykli ujemnych
        for(typename GraphType::PEdge E=g.getEdge(Koala::EdUndir); E; E=g.getEdgeNext(E, Koala::EdUndir))
            if (vertTab[U=g.getEdgeEnd1(E)].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnd2(E)].distance)
                { existNegCycle=true; break; }
            else if (vertTab[U=g.getEdgeEnd2(E)].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnd1(E)].distance)
                { existNegCycle=true; break; }

        if (!existNegCycle)
            for(typename GraphType::PEdge E=g.getEdge(Koala::EdDirOut); E; E=g.getEdgeNext(E, Koala::EdDirOut))
                if (vertTab[U=g.getEdgeEnd1(E)].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnd2(E)].distance)
                    { existNegCycle=true; break; }

        if (existNegCycle){
            return minusInf;
        }
        //jezeli nie ma cykli ujemnych to mozemy zwrocic wynik
        return end ? vertTab[end].distance : zero;
    }


    // wlasciwa procedura: zapisuje najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow,
    // zwraca liczbe krawedzi najkrotszej sciezki. Korzysta z kontenera vertTab z danymi z poprzedniej funkcji
    template <class GraphType, class VertContainer, class VIter, class EIter>
    static int
        getPath(
        const GraphType& g,
        VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
        typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
        // zwraca liczbe krawedzi sciezki, -1 jesli end jest nieosiagalny, -2 w razie wykrycia ujemnego cyklu
    {   koalaAssert(end,AlgExcNullVert);
        if (DefaultStructs:: template NumberTypeBounds<typename VertContainer::ValType::DistType>
                                                    ::isPlusInfty(vertTab[end].distance))
            return -1; // wierzcholek end jest nieosiagalny
        else if (DefaultStructs:: template NumberTypeBounds<typename VertContainer::ValType::DistType>
                                                    ::isMinusInfty(vertTab[end].distance))
            return -2; // w grafie jest cykl ujemny
        return ShortPathStructs::getOutPath(g,vertTab,iters,end);
    }


    // Rekord wyjsciowy zawierajacy od razu dlugosc najkr. sciezki i jej liczbe krawedzi
    template <class DistType> struct PathLengths {
        DistType length;
        int edgeNo;

        PathLengths(DistType alen, int ano) : length(alen), edgeNo(ano) {}
        PathLengths() {}
    };

    // zapisuje od razu sciezke krytyczna (wierzcholki i krawedzie) pod pare podanych iteratorow
    // Znajduje wszystko w jedym etapie
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static PathLengths<typename EdgeContainer::ValType::DistType>
        findPath(
        const GraphType& g,
        const EdgeContainer& edgeTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters) // para iteratorow do zapisu sciezki
        // zwraca rekord PathLengths z parametrami sciezki (dlugosc najdl siezki i jej liczba krawedzi)
        // lub (niesk,-1) jesli end jest nieosiagalny, a (-niesk,-2) w razie wykrycia ujemnego cyklu
    {   koalaAssert(start && end,AlgExcNullVert);
        typename EdgeContainer::ValType::DistType dist;
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab(g.getVertNo());

        if (DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType >
                            ::isPlusInfty(dist=distances(g,vertTab,edgeTab,start,end)))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny
        else if (DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType >
                            ::isMinusInfty(dist))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-2); // w grafie jest cykl ujemny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class BellmanFord : public BellmanFordPar<AlgsDefaultSettings> {};


//algorytm liczy najkrotsza sciezke pomiedzy kazda para wierzcholków zostal zaproponowany przez Floyda i oparty na twierdzeniu Warshalla)
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class FloydPar : public PathStructs {

    protected:

    template <class GraphType, class TwoDimVertContainer, class VIter, class EIter>
    static int
        getOutPathFromMatrix(const GraphType& g,
            const TwoDimVertContainer& vertMatrix, // dwuwymiarowa tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
            OutPath<VIter,EIter> iters,
            typename GraphType::PVertex start,
            typename GraphType::PVertex end)
        {   koalaAssert(end,AlgExcNullVert);
            typename GraphType::PVertex u,v=vertMatrix(start,end).vPrev;
            typename GraphType::PEdge  e=vertMatrix(start,end).ePrev;
            typename GraphType::PVertex LOCALARRAY(tabV,g.getVertNo());
            typename GraphType::PEdge LOCALARRAY(tabE,g.getVertNo());
            int len=0;

            if (end!=start) for(;v;len++)
               { tabV[len]=v; tabE[len]=e; e=vertMatrix(start,v).ePrev; v=(v==start) ? 0 : vertMatrix(start,v).vPrev; }

            for(int i=len-1;i>=0;i--)
                { *iters.vertIter=tabV[i];*iters.edgeIter=tabE[i];++iters.vertIter;++iters.edgeIter; }
            *iters.vertIter=end;++iters.vertIter;
            return len;
        }

    public:

    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {

        typedef DType DistType;// typ wagi liczbowej na krawedzi
        DistType length;// dlugosc krawedzi
    };

    // rekord wyjsciowy opisujacy wierzcholek
    template <class DType, class GraphType> struct VertLabs {

        typedef DType DistType;  // typ wagi liczbowej na krawedzi
        DType distance; // znaleziona odleglosc
        typename GraphType::PVertex vPrev; // element sciezki, analogicznie jak VisitVertLabs w search.h
        typename GraphType::PEdge  ePrev;

        VertLabs() : vPrev(0), ePrev(0),
                    distance(DefaultStructs :: template NumberTypeBounds<DType>::plusInfty()) {}
    };

    // mozna stosowac jako kontener opisujacy krawedz w przypadkach, gdy chcemy wsystkim krawedziom nadac wagi jednostkowe
    template <class DType> struct UnitLengthEdges {
        struct  ValType {
            typedef DType DistType;
            DistType length;
        };

        template <class T> ValType operator[](T e) const
        { ValType res; res.length=DefaultStructs:: template NumberTypeBounds<DType>::one();
        return res; }

    };


    // wlasciwa procedura: odleglosc miedzy kazda para wierzcholkow
    template <class GraphType, class TwoDimVertContainer, class EdgeContainer>
    static bool // false - wykryto ujemny cykl, wowczas wyniki z vertMatrix nie nadaja sie do uzytku
    distances (
        const GraphType & g, // badany graf
        TwoDimVertContainer& vertMatrix, // wyjsciowa dwuwymiarowa tablica asocjacyjna (PVertex,PVertex)-> VertLabs poszczegolnych wierzcholkow
        const EdgeContainer& edgeTab) // i tablica dlugosci krawedzi
    {
        const typename EdgeContainer::ValType::DistType
                inf=DefaultStructs:: template NumberTypeBounds
                            <typename EdgeContainer::ValType::DistType>::plusInfty();
        const typename EdgeContainer::ValType::DistType
                zero=DefaultStructs:: template NumberTypeBounds
                            <typename EdgeContainer::ValType::DistType> ::zero();
        bool existNegCycle = false; //jezeli existNegCycle jest ustawiona to w grafie istnieje cykl ujemny

        //sprawdzenie czy nie ma petli ujemnych
        for(typename GraphType::PEdge E=g.getEdge(Koala::EdLoop|Koala::EdUndir);E;E=g.getEdgeNext(E, Koala::EdLoop|Koala::EdUndir))
            if (edgeTab[E].length < zero) return false;

        if (DefaultStructs::ReserveOutAssocCont) vertMatrix.reserve(g.getVertNo());

        //inicjalizacja - ustawiam wartosci poczatkowe odleglosci w tablicy asocjacyjnej
        for(typename GraphType::PVertex U=g.getVert();U;U=g.getVertNext(U))
            for(typename GraphType::PVertex V=g.getVert();V;V=g.getVertNext(V))
                if (U==V) vertMatrix(U,U).distance=zero; else vertMatrix(U,V).distance=inf;


        for(typename GraphType::PEdge E=g.getEdge(Koala::EdDirOut);
                        E;E=g.getEdgeNext(E,Koala::EdDirOut))
        {   typename GraphType::PVertex U=g.getEdgeEnd1(E), V=g.getEdgeEnd2(E);
            if (edgeTab[E].length<vertMatrix(U,V).distance)
            { vertMatrix(U,V).distance=edgeTab[E].length; vertMatrix(U,V).ePrev=E; vertMatrix(U,V).vPrev=U; }
        }

        for(typename GraphType::PEdge E=g.getEdge(Koala::EdUndir);
                        E;E=g.getEdgeNext(E,Koala::EdUndir))
        {   typename GraphType::PVertex U=g.getEdgeEnd1(E), V=g.getEdgeEnd2(E),X;
            if (edgeTab[E].length<vertMatrix(U,V).distance)
            { vertMatrix(U,V).distance=edgeTab[E].length; vertMatrix(U,V).ePrev=E; vertMatrix(U,V).vPrev=U; }
            X=U;U=V;V=X;
            if (edgeTab[E].length<vertMatrix(U,V).distance)
            { vertMatrix(U,V).distance=edgeTab[E].length; vertMatrix(U,V).ePrev=E; vertMatrix(U,V).vPrev=U; }
        }

        //run Floyd()
        //szukam min{vertMatrix[vi][vj].distance, vertMatrix[vi][vl].distance+vertMatrix[vl][vj].distance}
        typename EdgeContainer::ValType::DistType nd;
        for(typename GraphType::PVertex Vl=g.getVert();Vl && !existNegCycle;Vl=g.getVertNext(Vl))
            for(typename GraphType::PVertex Vi=g.getVert();Vi;Vi=g.getVertNext(Vi))
            {
                 //if (vertMatrix(Vi,Vl).distance < inf) mozna tak albo tak jak ponizej
                 if (inf!=vertMatrix(Vi,Vl).distance)
                    for(typename GraphType::PVertex Vj=g.getVert();Vj;Vj=g.getVertNext(Vj))
                        if (inf > vertMatrix(Vl,Vj).distance && ((nd=vertMatrix(Vi,Vl).distance+vertMatrix(Vl,Vj).distance) < vertMatrix(Vi,Vj).distance))
                            { vertMatrix(Vi,Vj).distance=nd; vertMatrix(Vi,Vj).ePrev=vertMatrix(Vl,Vj).ePrev; vertMatrix(Vi,Vj).vPrev=vertMatrix(Vl,Vj).vPrev; }
                //sprawdzenie czy nie ma cykli ujemnych
                if (zero > vertMatrix(Vi,Vi).distance)
                    { existNegCycle=true; break; }
            }

        if (existNegCycle) return false;
        return true;
    }


    // wlasciwa procedura: zapisuje najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow,
    // zwraca liczbe krawedzi najkrotszej sciezki lub -1 gdy end jest nieosiagalny. Korzysta z kontenera vertMatrix z danymi z poprzedniej funkcji
    template <class GraphType, class TwoDimVertContainer, class VIter, class EIter>
    static int
        getPath(
        const GraphType& g, // badany graf
        const TwoDimVertContainer& vertMatrix, // tablica asoc. - rezultat poprzedniej funkcji
        typename GraphType::PVertex start,
        typename GraphType::PVertex end,
        PathStructs::OutPath<VIter,EIter> iters) // para iteratorow do zapisu znalezionej sciezki
    {   koalaAssert(start && end,AlgExcNullVert);
        if (DefaultStructs:: template NumberTypeBounds<typename TwoDimVertContainer::ValType::DistType>
                ::isPlusInfty(vertMatrix(start,end).distance))
            return -1; // wierzcholek end jest nieosiagalny
        return getOutPathFromMatrix(g,vertMatrix,iters,start,end);
    }
};

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class Floyd : public FloydPar<AlgsDefaultSettings> {};


// najlzejsze lub najciezsze lasy w grafie
template <class DefaultStructs>
class KruskalPar {


    public:


    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {

        typedef DType WeightType; // typ wagi liczbowej na krawedzi
        WeightType weight;// wagakrawedzi
    };


  template <class DType> struct Result {

        DType weight;   // waga znalezionego lasu
        int edgeNo; // jego liczba krawedzi
    };


    protected:

    template <class GraphType, class EdgeContainer, class Iter, class VertCompContainer>
    static Result<typename EdgeContainer::ValType::WeightType>
    getForest(
        const GraphType & g,
        const EdgeContainer& edgeTab,
        Iter out,
        VertCompContainer& asets,
        int edgeNo, //ujemne oznacza maksymalnie duzo
        bool minWeight)
        {   JoinableSets<typename GraphType::PVertex, typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                JSPartDesrc<typename GraphType::PVertex> *>::Type > localSets;
            typename BlackHoleSwitch<VertCompContainer,JoinableSets<typename GraphType::PVertex,
                typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    JSPartDesrc<typename GraphType::PVertex> *>::Type > >::Type &
                        sets    = BlackHoleSwitch<VertCompContainer,JoinableSets<typename GraphType::PVertex,
                typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    JSPartDesrc<typename GraphType::PVertex> *>::Type > >
                    ::get(asets,localSets);

            Result<typename EdgeContainer::ValType::WeightType> res;
            res.edgeNo=0; res.weight=DefaultStructs:: template NumberTypeBounds
                                    <typename EdgeContainer::ValType::WeightType>::zero();
            const EdgeDirection mask=Directed|Undirected;
            int n,m=g.getEdgeNo(mask);
            sets.resize(n=g.getVertNo());
            if (n==0) return res;
            for(typename GraphType::PVertex v = g.getVert(); v ; v = g.getVertNext(v)) sets.makeSinglet(v);


            edgeNo = (edgeNo>=0) ? edgeNo : n-1;
            if (m==0||edgeNo==0) return res;

            std::pair<typename EdgeContainer::ValType::WeightType, typename GraphType::PEdge>
                            LOCALARRAY(edges,m);
            int i = 0;
            typename GraphType::PEdge e;
            for(e = g.getEdge(mask); e != NULL; e = g.getEdgeNext(e, mask))
                edges[i++] = std::make_pair(edgeTab[e].weight, e);
            DefaultStructs::sort(edges,edges+m);
            if (!minWeight) std::reverse(edges,edges+m);

            for(i = 0; i < m && edgeNo>0; i++)
            {   std::pair<typename GraphType::PVertex, typename GraphType::PVertex> ends;
                e = edges[i].second;
                ends = g.getEdgeEnds(e);
                if(sets.getSetId(ends.first) != sets.getSetId(ends.second))
                {
                    res.weight = res.weight+edgeTab[e].weight;
                    res.edgeNo++;
                    sets.join(ends.first, ends.second);
                    *out = e; ++out; edgeNo--;
                };
            };

            return res;
    }

    public:

    // znajduje najlzejszy las
    template <class GraphType, class EdgeContainer, class Iter, class VertCompContainer>
    static Result<typename EdgeContainer::ValType::WeightType>
    getMinForest(
        const GraphType & g, // badany graf, petle sa ignorowane a luki i kr. nieskierowane traktowane jednakow
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs wag krawedzi
        Iter out, // iterator do zapisu ciagu krawedzi lasu
        VertCompContainer& asets, // wynikowa struktura JoinableSets<PVertex> ze skladowymi spojnosci znalezionego lasu (lub BlackHole)
        int edgeNo=-1) //limit liczby krawedzi - znaleziony las bedzie mial najwieksza mozliwa liczbe krawedzi nie przekraczajaca tego parametru
        // pominiecie parametru - znaleziony las bedzie mial najwieksza mozliwa liczbe krawedzi
    {
        return getForest(g,edgeTab,out,asets,edgeNo,true);
    }


    // znajduje najciezszy las
    template <class GraphType, class EdgeContainer, class Iter, class VertCompContainer>
    static Result<typename EdgeContainer::ValType::WeightType>
    getMaxForest(
        const GraphType & g, // badany graf, petle sa ignorowane a luki i kr. nieskierowane traktowane jednakow
        const EdgeContainer& edgeTab, // wejsciowa tablica asocjacyjna PEdge->EdgeLabs wag krawedzi
        Iter out, // iterator do zapisu ciagu krawedzi lasu
        VertCompContainer& asets, // wynikowa struktura JoinableSets<PVertex> ze skladowymi spojnosci znalezionego lasu (lub BlackHole)
        int edgeNo=-1) //limit liczby krawedzi - znaleziony las bedzie mial najwieksza mozliwa liczbe krawedzi nie przekraczajaca tego parametru
        // pominiecie parametru - znaleziony las bedzie mial najwieksza mozliwa liczbe krawedzi
    {
        return getForest(g,edgeTab,out,asets,edgeNo,false);
    }


};

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class Kruskal : public KruskalPar<AlgsDefaultSettings> {};

}

#endif

