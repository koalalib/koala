#ifndef DEF_WEIGHTS_H
#define DEF_WEIGHTS_H

#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include "../base/def_struct.h"
#include "../graph/graph.h"
#include "../graph/subgraph.h"
#include "search.h"
#include "../container/joinsets.h"
#include "../container/localarray.h"


namespace Koala {

//Nazewnictwo javowe: nazwy wieloczlonowe - kazdy czlon z duzej bez podkerslen. Typy z z duzej litery, a inne
//obiekty z malej. Prosze o krotkie nazwy.
// TODO:Jak wymyslimy koalowa hierarchie wyjatkow, wszystkie asserty trzeba bedzie zastapic throwami


//A tak wyobrazam sobie wszystkie nasze algorytmy - spokrewnione procedury i uzywane struktury
//zamykamy w jednej klasie. Same procedury sa szablonami jej metod statycznych.

class Dijkstra : public ShortPathStructs {
// namespace Dijkstra { - druga mozliwosc, wtedy mozna by pozbyc sie staticow

    // wszystkie rodzaje nietrywialnych struktur uzywanych wewnatrz procedur okreslamy tutaj, aby w razie
    // ich wymiany wystarczylo zmieniac w jednym miejscu
    class DefaultStructs {
        public:

        template <class A, class B> class AssocCont {
            public:
            typedef AssocArray<A,B> Type;
        };

    };

    public:

    // Struktury opisujace elementy grafu, na ktorych dziala uzytkownik wprowadzajac dane wejsciowe i ew.
    // odbierajac wyniki.

    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {

        typedef DType DistType;
        DistType length;
    };

    // ... i dla wierzcholka
    template <class DType, class GraphType> struct VertLabs {

        typedef DType DistType;
        DType distance;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        VertLabs() : vPrev(0), ePrev(0), distance(NumberTypeBounds<DType>::plusInfty()) {}
    };

    // wlasciwa procedura: odleglosc miedzy para wierzcholkow
    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    distances (
        const GraphType & g,
        VertContainer& avertTab, // tablica asocjacyjna z VertLabs poszczegolnych wierzcholkow
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {   assert(start);
        typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab, Q;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;

        Q[start].vPrev=0;Q[start].ePrev=0;
        Q[start].distance=NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();

        while(!Q.empty()){
            typename EdgeContainer::ValType::DistType
                    d=NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty(),nd;
            for(V=Q.firstKey();V;V=Q.nextKey(V)) if (Q[V].distance<d) d=Q[U=V].distance;
            vertTab[U]=Q[U]; Q.delKey(U);
            if (U==end) return vertTab[end].distance;

            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirOut|Koala::EdUndir);
                E;E=g.getEdgeNext(U,E,Koala::EdDirOut|Koala::EdUndir))
                    if (!vertTab.hasKey(V=g.getEdgeEnd(E,U)))
                        if ((nd=vertTab[U].distance+edgeTab[E].length)<Q[V].distance)
                    { Q[V].distance=nd; Q[V].ePrev=E; Q[V].vPrev=U; }
        }

        return end ? NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty()
                    : NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
    }


//Jesli nasze procedury zwracaja ciagi lub zbiory, niech uzytkownik podaje iteratory-wstawiacze, pod ktore maja
//byc kolejno zapisywane elementy. Gdyby byly to ciagi struktur, ktore same zawieraja ciagi lub zbiory
//(np. ciag znalezionych skladowych spojnych, a kazda z nich ma zbior wierzcholkow), wyjscie procedur raczej nie
//powinno zawierac struktur z zagniezdzonymi w polach nietrywialnymi strukturami np. zbiorami. Wyobrazam sobie,
//ze wtedy zapisujemy te elementy ciurkiem (przyklad j.w. wierzcholki) do drugiej tablicy (podanej przez iterator),
//zas rekord opisujacy pojedynczy element ciagu posiada tez pozycje (typu int, a nie iterator) okreslajace zakres
//ze swoimi elementami (poczatek i zakoniec) w tej tablicy.
// A wiec zakladam, ze w naszym przykladzie wyjsciem sa 2 tablice: z opisami kolejnych skladowych spojnosci
// i ze wszystkimi wierzcholkami ulozonymi tak, ze kolejne przedzialy tej tablicy to zbiory wierzcholkow
// w kolejnych skladowych
//O iteratorach-wstawiaczach nie zakladam nic, w szczegolnosci moga to byc zwykle wskazniki na pole tablicy
//zaalokowanej przez uzytkownika do odbioru danych.



// Ze wzgledow efektywnosciowych w przypadku procedur wieloetapowych, mozna rozdzielac je na kolejne fazy
// i zakladac, ze uzytkownik wywola je we wlasciwej kolejnosci, korzystajac z wczesniej uzyskanych danych.

    // wlasciwa procedura: zapisuje najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow,
    // zwraca liczbe krawedzi najkrotszej sciezki. Korzysta z kontenera vertTab z danymi z poprzedniej funkcji
    template <class GraphType, class VertContainer, class VIter, class EIter>
    static int
        getPath(
        const GraphType& g,
        VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
        typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
    {   assert(end);
        if (NumberTypeBounds<typename VertContainer::ValType::DistType>::isPlusInfty(vertTab[end].distance))
            return -1; // wierzcholek end jest nieosiagalny

        return ShortPathStructs::getOutPath(g,vertTab,iters,end);
    }


    // Rekord wyjsciowy zawierajacy od razu dlugosc najkr. sciezki i jej liczbe krawedzi
    template <class DistType> struct PathLengths {
        DistType length;
        int edgeNo;

        PathLengths(DistType alen, int ano) : length(alen), edgeNo(ano) {}
        PathLengths() {}
    };

    // wlasciwa procedura: zapisuje od razu najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow
    // Znajduje wszystko w jedym etapie
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static PathLengths<typename EdgeContainer::ValType::DistType>
        findPath(
        const GraphType& g,
        EdgeContainer& edgeTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
    {   assert(start && end);
        typename EdgeContainer::ValType::DistType dist;
        typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab;

        if (NumberTypeBounds<typename EdgeContainer::ValType::DistType >::isPlusInfty(dist
                                                            =distances(g,vertTab,edgeTab,start,end)))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};



class DAGCritPath : public ShortPathStructs {

    class DefaultStructs {
        public:

        template <class A, class B> class AssocCont {
            public:
            typedef AssocArray<A,B> Type;
        };

    };

    public:

    template <class DType> struct EdgeLabs {

        typedef DType DistType;
        DistType length;
    };


    template <class DType, class GraphType> struct VertLabs {

        typedef DType DistType;
        DType distance;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        VertLabs() : vPrev(0), ePrev(0), distance(NumberTypeBounds<DType>::minusInfty()) {}
    };


    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    critPathLength (
        const GraphType & g,
        VertContainer& avertTab, // tablica asocjacyjna z VertLabs poszczegolnych wierzcholkow
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {   assert(start);
        typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;
        typename EdgeContainer::ValType::DistType nd;

        vertTab[start].vPrev=0;vertTab[start].ePrev=0;
        vertTab[start].distance=NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        if (start==end) return NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();

        typename DefaultStructs::AssocCont<typename GraphType::PVertex, char >::Type followers;
        typename GraphType::PVertex LOCALARRAY(tabV,g.getVertNo());
        Koala::BFS::scanAttainable(g,start,assocInserter(followers,emptyStruct((char)0)),EdDirOut);
        Koala::DAGAlgs::topOrd(makeSubgraph(const_cast<GraphType&>(g),std::make_pair(assocKeyChoose(followers),stdChoose(true))),tabV);

        for(int i=1;i<followers.size();i++) {
            U=tabV[i];
            vertTab[U].vPrev=0;vertTab[U].ePrev=0;
            vertTab[U].distance=NumberTypeBounds<typename EdgeContainer::ValType::DistType>::minusInfty();

            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirIn);
                E;E=g.getEdgeNext(U,E,Koala::EdDirIn))
                    if (followers.hasKey(V=g.getEdgeEnd(E,U)))
                        if ((nd=vertTab[V].distance+edgeTab[E].length)>vertTab[U].distance)
                    { vertTab[U].distance=nd; vertTab[U].ePrev=E; vertTab[U].vPrev=V;  }
            if (U==end) return vertTab[U].distance;
        }

        return end ? NumberTypeBounds<typename EdgeContainer::ValType::DistType>::minusInfty()
                    : NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
    }


    template <class GraphType, class VertContainer, class VIter, class EIter>
    static int
        const getPath(
        GraphType& g,
        VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
        typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
    {   assert(end);
        if (NumberTypeBounds<typename VertContainer::ValType::DistType>::isMinusInfty(vertTab[end].distance))
            return -1; // wierzcholek end jest nieosiagalny

        return ShortPathStructs::getOutPath(g,vertTab,iters,end);
    }


    // Rekord wyjsciowy zawierajacy od razu dlugosc najkr. sciezki i jej liczbe krawedzi
    template <class DistType> struct PathLengths {
        DistType length;
        int edgeNo;

        PathLengths(DistType alen, int ano) : length(alen), edgeNo(ano) {}
        PathLengths() {}
    };

    // wlasciwa procedura: zapisuje od razu sciezke krytyczna (wierzcholki i krawedzie) pod pare podanych iteratorow
    // Znajduje wszystko w jedym etapie
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static PathLengths<typename EdgeContainer::ValType::DistType>
        findPath(
        const GraphType& g,
        EdgeContainer& edgeTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
    {   assert(start && end);
        typename EdgeContainer::ValType::DistType dist;
        typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab;

        if (NumberTypeBounds<typename EdgeContainer::ValType::DistType >::isMinusInfty(dist
                                                            =critPathLength(g,vertTab,edgeTab,start,end)))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};



class Kruskal {

    class DefaultStructs {
        public:

        template <class A, class B> class AssocCont {
            public:
            typedef AssocArray<A,B> Type;
        };

    };


    public:


    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {

        typedef DType WeightType;
        WeightType weight;
    };


  template <class DType> struct Result {

        DType weight;
        int edgeNo;
    };


    protected:

    template <class GraphType, class EdgeContainer, class Iter, class VertCompContainer>
    static Result<typename EdgeContainer::ValType::WeightType>
    getForest(
        const GraphType & g,
        EdgeContainer& edgeTab,
        Iter out,
        VertCompContainer& asets,
        int edgeNo, //ujemne oznacza maksymalnie duzo
        EdgeDirection mask,
        bool minWeight)
        {   JoinableSets<typename GraphType::PVertex, typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                JSPartDesrc<typename GraphType::PVertex> *>::Type > localSets;
            typename BlackHoleSwitch<VertCompContainer,JoinableSets<typename GraphType::PVertex,
                typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                    JSPartDesrc<typename GraphType::PVertex> *>::Type > >::Type &
                        sets    = BlackHoleSwitch<VertCompContainer,JoinableSets<typename GraphType::PVertex,
                typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                    JSPartDesrc<typename GraphType::PVertex> *>::Type > >
                    ::get(asets,localSets);

            Result<typename EdgeContainer::ValType::WeightType> res;
            res.edgeNo=0; res.weight=NumberTypeBounds<typename EdgeContainer::ValType::WeightType>::zero();

            sets.clear(); sets.resize(g.getVertNo());
            if (g.getVertNo()==0) return res;
            for(typename GraphType::PVertex v = g.getVert(); v ; v = g.getVertNext(v)) sets.makeSinglet(v);

            mask&=(~EdLoop);
            edgeNo = (edgeNo>=0) ? edgeNo : g.getVertNo()-1;
            if (g.getEdgeNo(mask)==0||edgeNo==0) return res;

            std::pair<typename EdgeContainer::ValType::WeightType, typename GraphType::PEdge>
                            LOCALARRAY(edges,g.getEdgeNo(mask));
            int i = 0;
            typename GraphType::PEdge e;
            for(e = g.getEdge(mask); e != NULL; e = g.getEdgeNext(e, mask))
                edges[i++] = std::make_pair(edgeTab[e].weight, e);
            std::make_heap(edges,edges+g.getEdgeNo(mask)); std::sort_heap(edges,edges+g.getEdgeNo(mask));
            if (!minWeight) std::reverse(edges,edges+g.getEdgeNo(mask));

            for(i = 0; i < g.getEdgeNo(mask) && edgeNo>0; i++)
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

    template <class GraphType, class EdgeContainer, class Iter, class VertCompContainer>
    static Result<typename EdgeContainer::ValType::WeightType>
    getMinForest(
        const GraphType & g,
        EdgeContainer& edgeTab,
        Iter out,
        VertCompContainer& asets,
        int edgeNo=-1, //ujemne oznacza maksymalnie duzo
        EdgeDirection mask=EdUndir)
    {
        return getForest(g,edgeTab,out,asets,edgeNo,mask,true);
    }


    template <class GraphType, class EdgeContainer, class Iter>
    static Result<typename EdgeContainer::ValType::WeightType>
    getMinForest(const GraphType & g,EdgeContainer& edgeTab,Iter out)
    {
        return getMinForest(g,edgeTab,out,blackHole());
    }


    template <class GraphType, class EdgeContainer, class Iter, class VertCompContainer>
    static Result<typename EdgeContainer::ValType::WeightType>
    getMaxForest(
        const GraphType & g,
        EdgeContainer& edgeTab,
        Iter out,
        VertCompContainer& asets,
        int edgeNo=-1, //ujemne oznacza maksymalnie duzo
        EdgeDirection mask=EdUndir)
    {
        return getForest(g,edgeTab,out,asets,edgeNo,mask,false);
    }


    template <class GraphType, class EdgeContainer, class Iter>
    static Result<typename EdgeContainer::ValType::WeightType>
    getMaxForest(const GraphType & g,EdgeContainer& edgeTab,Iter out)
    {
        return getMaxForest(g,edgeTab,out,blackHole());
    }


};

}

#endif

