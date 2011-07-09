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


class WeightAlgsDefaultStructs {
    public:

    template <class A, class B> class AssocCont {
        public:
        typedef AssocArray<A,B> Type;

    };

};

template <class DefaultStructs>
class DijkstraPar : public ShortPathStructs {
// namespace Dijkstra { - druga mozliwosc, wtedy mozna by pozbyc sie staticow

    // wszystkie rodzaje nietrywialnych struktur uzywanych wewnatrz procedur okreslamy tutaj, aby w razie
    // ich wymiany wystarczylo zmieniac w jednym miejscu

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
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab, Q;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
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
        ShortPathStructs::OutPath<VIter,EIter> iters,bool useHeap)
    {   assert(start && end);
        typename EdgeContainer::ValType::DistType dist;
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab;

        if (!useHeap) dist=distances(g,vertTab,edgeTab,start,end);
        else dist=distances2(g,vertTab,edgeTab,start,end);

        if (NumberTypeBounds<typename EdgeContainer::ValType::DistType >::isPlusInfty(dist))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

    // klasyczny Dijkstra
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static PathLengths<typename EdgeContainer::ValType::DistType>
        findPath(
        const GraphType& g,
        EdgeContainer& edgeTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
    {   return findPath(g,edgeTab,start,end,iters,false); }

    // Dijkstra na kopcu
    template <class GraphType, class EdgeContainer, class VIter, class EIter>
    static PathLengths<typename EdgeContainer::ValType::DistType>
        findPath2(
        const GraphType& g,
        EdgeContainer& edgeTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
    {   return findPath(g,edgeTab,start,end,iters,true); }


// Dijkstra na kopcu

    protected:

    template< typename Pair > struct Cmp        {            bool operator() ( Pair a, Pair b )
            {   return a.second > b.second || (a.second == b.second && a.first < b.first); }        } ;

    template <class Key, class Container>
    class Queue : public Container {

        private:

            PriQueueInterface<std::pair<Key,typename Container::ValType::DistType>*,
                Cmp<std::pair<Key,typename Container::ValType::DistType> > > q;

        public:

            Queue(std::pair<Key,typename Container::ValType::DistType>* wsk,int max) : q(wsk,max) {}

            void setDist(Key v,typename Container::ValType::DistType d)
            {  (operator[](v)).distance=d;
                q.push(std::make_pair(v,d));
            }

            typename Container::ValType::DistType getDist(Key v)
            {
                if (!hasKey(v)) return NumberTypeBounds<typename Container::ValType::DistType>::plusInfty();
                else return (operator[](v)).distance;
            }

            std::pair<Key,typename Container::ValType::DistType> min()
            {
                std::pair<Key,typename Container::ValType::DistType> res;
                while (!hasKey( (res=q.top()).first)) q.pop();
                return res;
            }
    };

    public:

    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    distances2 (
        const GraphType & g,
        VertContainer& avertTab, // tablica asocjacyjna z VertLabs poszczegolnych wierzcholkow
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {   assert(start);
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab;

        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        std::pair<typename GraphType::PVertex, typename EdgeContainer::ValType::DistType> LOCALARRAY(qbufor,2*g.getEdgeNo()+2);
        Queue<typename GraphType::PVertex, typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type> Q(qbufor,2*g.getEdgeNo()+1);

        typename GraphType::PVertex U,V;

        Q[start].vPrev=0;Q[start].ePrev=0;
        Q.setDist(start,NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero());

        while(!Q.empty()){
            typename EdgeContainer::ValType::DistType d,nd;
            std::pair<typename GraphType::PVertex,typename EdgeContainer::ValType::DistType> res=Q.min();
            U=res.first; d=res.second;
            vertTab[U]=Q[U]; Q.delKey(U);
            if (U==end) return vertTab[end].distance;

            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirOut|Koala::EdUndir);
                E;E=g.getEdgeNext(U,E,Koala::EdDirOut|Koala::EdUndir))
                    if (!vertTab.hasKey(V=g.getEdgeEnd(E,U)))
                        if ((nd=vertTab[U].distance+edgeTab[E].length)<Q.getDist(V))
                            { Q.setDist(V,nd); Q[V].ePrev=E; Q[V].vPrev=U; }
        }

        return end ? NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty()
                    : NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
    }

};

class Dijkstra : public DijkstraPar<WeightAlgsDefaultStructs> {};



template <class DefaultStructs>
class DAGCritPathPar : public ShortPathStructs {


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
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;
        typename EdgeContainer::ValType::DistType nd;

        vertTab[start].vPrev=0;vertTab[start].ePrev=0;
        vertTab[start].distance=NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        if (start==end) return NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();

        typename DefaultStructs::template AssocCont<typename GraphType::PVertex, char >::Type followers;
        typename GraphType::PVertex LOCALARRAY(tabV,g.getVertNo());
        Koala::BFS::scanAttainable(g,start,assocInserter(followers,constFun<char>(0)),EdDirOut);
        Koala::DAGAlgs::topOrd(makeSubgraph(g,std::make_pair(assocKeyChoose(followers),stdChoose(true))),tabV);

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
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab;

        if (NumberTypeBounds<typename EdgeContainer::ValType::DistType >::isMinusInfty(dist
                                                            =critPathLength(g,vertTab,edgeTab,start,end)))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};

class DAGCritPath : public DAGCritPathPar<WeightAlgsDefaultStructs> {};



template <class DefaultStructs>
class BellmanFordPar : public ShortPathStructs {

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
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type localvertTab;
        typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                    vertTab=
                BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

        typename GraphType::PVertex U,V;

        typename EdgeContainer::ValType::DistType
                inf=NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty(),nd;
        typename EdgeContainer::ValType::DistType
                zero=NumberTypeBounds<typename EdgeContainer::ValType::DistType> ::zero();
        typename EdgeContainer::ValType::DistType
                minusInf=NumberTypeBounds<typename EdgeContainer::ValType::DistType> ::minusInfty();
        bool existNegCycle = false;


        //sprawdzenie czy nie ma petli ujemnych
        for(typename GraphType::PEdge E=g.getEdge(Koala::EdLoop|Koala::EdUndir);E;E=g.getEdgeNext(E, Koala::EdLoop|Koala::EdUndir))
            if (edgeTab[E].length < zero)
            {
                for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                    vertTab[v].distance=minusInf;
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
        int n=g.getVertNo();
        for(int i=1;i<n;i++){
            //relaksacja krawedzi nieskierowanych
            for(typename GraphType::PEdge E=g.getEdge(Koala::EdUndir); E; E=g.getEdgeNext(E, Koala::EdUndir)){
                if (vertTab[U=g.getEdgeEnds(E).first].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnds(E).second].distance)
                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
                else if (vertTab[U=g.getEdgeEnds(E).second].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnds(E).first].distance)
                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
            }
            //relaksacja krawedzi (u,v) skierowanych u->v
            for(typename GraphType::PEdge E=g.getEdge(Koala::EdDirOut); E; E=g.getEdgeNext(E, Koala::EdDirOut))
                if (vertTab[U=g.getEdgeEnds(E).first].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnds(E).second].distance)
                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
        }

        //sprawdzenie czy nie ma cykli ujemnych
        for(typename GraphType::PEdge E=g.getEdge(Koala::EdUndir); E; E=g.getEdgeNext(E, Koala::EdUndir))
            if (vertTab[U=g.getEdgeEnds(E).first].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnds(E).second].distance)
                { existNegCycle=true; break; }
            else if (vertTab[U=g.getEdgeEnds(E).second].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnds(E).first].distance)
                { existNegCycle=true; break; }

        if (!existNegCycle)
            for(typename GraphType::PEdge E=g.getEdge(Koala::EdDirOut); E; E=g.getEdgeNext(E, Koala::EdDirOut))
                if (vertTab[U=g.getEdgeEnds(E).first].distance < inf && (nd=vertTab[U].distance+edgeTab[E].length) < vertTab[V=g.getEdgeEnds(E).second].distance)
                    { existNegCycle=true; break; }

        //jesli ponizsze ustawianie wszystkich odleglosci na minusInf pominac to dotychczasowe obliczenia zostana, trzeba zdecydowac:
        //1 podejscie: ustawic wszystkie odleglosci na minusInf - oczywista informacja dla uzytkownika
        //2 podejscie: funkcja zwroci minusInf, ale pomijamy kasowanie dotychczasowych obliczen - uzytkownik musi spr czy funkcja distances nie zwraca minusInf
        //zanim zacznie cokolwiek innego robic
        if (existNegCycle){
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                vertTab[v].distance=minusInf;
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
    {   assert(end);
        if (NumberTypeBounds<typename VertContainer::ValType::DistType>::isPlusInfty(vertTab[end].distance))
            return -1; // wierzcholek end jest nieosiagalny
        else if (NumberTypeBounds<typename VertContainer::ValType::DistType>::isMinusInfty(vertTab[end].distance))
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
        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab;

        if (NumberTypeBounds<typename EdgeContainer::ValType::DistType >::isPlusInfty(dist
                                                            =distances(g,vertTab,edgeTab,start,end)))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny
        else if (NumberTypeBounds<typename EdgeContainer::ValType::DistType >::isMinusInfty(dist))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-2); // w grafie jest cykl ujemny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};

class BellmanFord : public BellmanFordPar<WeightAlgsDefaultStructs> {};


//algorytm liczy najkrotsza sciezke pomiedzy kazda para wierzcholków zostal zaproponowany przez Floyda i oparty na twierdzeniu Warshalla)
class Floyd : public PathStructs {

    template <class GraphType, class TwoDimVertContainer, class VIter, class EIter>
    static int
        getOutPathFromMatrix(const GraphType& g,
            TwoDimVertContainer& vertMatrix, // dwuwymiarowa tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
            OutPath<VIter,EIter> iters,
            typename GraphType::PVertex start,
            typename GraphType::PVertex end)
        {   assert(end);
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

    template <class DType> struct EdgeLabs {

        typedef DType DistType;
        DistType length;
    };

    template <class DType, class GraphType> struct VertLabs {

        typedef DType DistType;
        DType distance;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;

        VertLabs() : vPrev(0), ePrev(0), distance(NumberTypeBounds<DType>::plusInfty()) {}
    };

    // wlasciwa procedura: odleglosc miedzy para wierzcholkow
    template <class GraphType, class TwoDimVertContainer, class EdgeContainer>
    static bool // false - wykryto ujemny cykl
    distances (
        const GraphType & g,
        TwoDimVertContainer& vertMatrix, // dwuwymiarowa tablica asocjacyjna z VertLabs poszczegolnych wierzcholkow
        EdgeContainer& edgeTab) // i tablica dlugosci krawedzi
    // i tak liczymy odleglosci pomiedzy wszystkimi parami
    {
        typename EdgeContainer::ValType::DistType
                inf=NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();
        typename EdgeContainer::ValType::DistType
                zero=NumberTypeBounds<typename EdgeContainer::ValType::DistType> ::zero();
//        typename EdgeContainer::ValType::DistType
//                minusInf=NumberTypeBounds<typename EdgeContainer::ValType::DistType> ::minusInfty();
        bool existNegCycle = false; //jezeli existNegCycle jest ustawiona to w grafie istnieje cykl ujemny

        //sprawdzenie czy nie ma petli ujemnych
        for(typename GraphType::PEdge E=g.getEdge(Koala::EdLoop|Koala::EdUndir);E;E=g.getEdgeNext(E, Koala::EdLoop|Koala::EdUndir))
            if (edgeTab[E].length < zero) return false;

        //inicjalizacja - ustawiam wartosci poczatkowe odleglosci w tablicy asocjacyjnej
        for(typename GraphType::PVertex U=g.getVert();U;U=g.getVertNext(U))
            for(typename GraphType::PVertex V=g.getVert();V;V=g.getVertNext(V))
                if (U==V) vertMatrix(U,U).distance=zero; else vertMatrix(U,V).distance=inf;


        for(typename GraphType::PEdge E=g.getEdge(Koala::EdDirOut);
                        E;E=g.getEdgeNext(E,Koala::EdDirOut))
        {   typename GraphType::PVertex U=E->getEnds().first, V=E->getEnds().second;
            if (edgeTab[E].length<vertMatrix(U,V).distance)
            { vertMatrix(U,V).distance=edgeTab[E].length; vertMatrix(U,V).ePrev=E; vertMatrix(U,V).vPrev=U; }
        }

        for(typename GraphType::PEdge E=g.getEdge(Koala::EdUndir);
                        E;E=g.getEdgeNext(E,Koala::EdUndir))
        {   typename GraphType::PVertex U=E->getEnds().first, V=E->getEnds().second,X;
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
                 if (!NumberTypeBounds<typename TwoDimVertContainer::ValType::DistType>::isPlusInfty(vertMatrix(Vi,Vl).distance))
                    for(typename GraphType::PVertex Vj=g.getVert();Vj;Vj=g.getVertNext(Vj))
                        if (vertMatrix(Vl,Vj).distance < inf && ((nd=vertMatrix(Vi,Vl).distance+vertMatrix(Vl,Vj).distance) < vertMatrix(Vi,Vj).distance))
                            { vertMatrix(Vi,Vj).distance=nd; vertMatrix(Vi,Vj).ePrev=vertMatrix(Vl,Vj).ePrev; vertMatrix(Vi,Vj).vPrev=vertMatrix(Vl,Vj).vPrev; }
                //sprawdzenie czy nie ma cykli ujemnych
                if (vertMatrix(Vi,Vi).distance < zero)
                    { existNegCycle=true; break; }
            }

        //jezeli jest cykl ujemny to ustawiamy wszystkie odleglosci na minusInf i zwracamy wartosc minusInf
        if (existNegCycle) return false;
        //jezeli nie ma cykli ujemnych to zwracamy zero
        return true;
    }


    // wlasciwa procedura: zapisuje najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow,
    // zwraca liczbe krawedzi najkrotszej sciezki. Korzysta z kontenera vertMatrix z danymi z poprzedniej funkcji
    template <class GraphType, class TwoDimVertContainer, class VIter, class EIter>
    static int
        getPath(
        const GraphType& g,
        TwoDimVertContainer& vertMatrix, // tablica asoc. - rezultat poprzedniej funkcji
        typename GraphType::PVertex start,
        typename GraphType::PVertex end,
        PathStructs::OutPath<VIter,EIter> iters)
    {   assert(start && end);
        if (NumberTypeBounds<typename TwoDimVertContainer::ValType::DistType>::isPlusInfty(vertMatrix(start,end).distance))
            return -1; // wierzcholek end jest nieosiagalny
        return getOutPathFromMatrix(g,vertMatrix,iters,start,end);
    }
};



template <class DefaultStructs>
class KruskalPar {


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
        return getMinForest(g,edgeTab,out,blackHole);
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
        return getMaxForest(g,edgeTab,out,blackHole);
    }


};

class Kruskal : public KruskalPar<WeightAlgsDefaultStructs> {};

}

#endif

