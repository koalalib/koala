#ifndef DEF_WEIGHTS_H
#define DEF_WEIGHTS_H

#include <vector>
#include <map>
#include <cassert>
#include "../graph/graph.h"
#include "../base/def_struct.h"


namespace Koala {

//Nazewnictwo javowe: nazwy wieloczlonowe - kazdy czlon z duzej bez podkerslen. Typy z z duzej litery, a inne
//obiekty z malej. Prosze o krotkie nazwy.
// TODO:Jak wymyslimy koalowa hierarchie wyjatkow, wszystkie asserty trzeba bedzie zastapic throwami


//A tak wyobrazam sobie wszystkie nasze algorytmy - spokrewnione procedury i uzywane struktury
//zamykamy w jednej klasie. Same procedury sa szablonami jej metod statycznych.

class Dijkstra {
// namespace Dijkstra { - druga mozliwosc, wtedy mozna by pozbyc sie staticow

    // wszystkie rodzaje nietrywialnych struktur uzywanych wewnatrz procedur okreslamy tutaj, aby w razie
    // ich wymiany wystarczylo zmieniac w jednym miejscu
    class DefaultStructs {
        public:

        template <class A, class B> class AssocCont {
            public:
            typedef AssocArray<A,B> Type;
            //AssocTabWrapper<std::map<A,B> > Type; //oczywiscie docelowo nie beda to mapy :-)
        };

        template <class A> class ArrayCont {
            public:
            typedef std::vector<A> Type;
        };
    };

    public:

    // Struktury opisujace elementy grafu, na ktorych dziala uzytkownik wprowadzajac dane wejsciowe i ew.
    // odbierajac wyniki.

    // rekord wejsciowy opisujacy krawedz
    template <class DType> struct EdgeLabs {
        public:

        typedef DType DistType;
        DistType length;
    };

    // ... i dla wierzcholka
    template <class DType, class GraphType> struct VertLabs {
        public:

        typedef DType DistType;
        DType distance;
        typename GraphType::PVertex vPrev;
        typename GraphType::PEdge  ePrev;
    };

    // wlasciwa procedura: odleglosc miedzy para wierzcholkow
    template <class GraphType, class VertContainer, class EdgeContainer>
    static typename EdgeContainer::ValType::DistType
    distances (
        GraphType & g,
        VertContainer& vertTab, // tablica asocjacyjna z VertLabs poszczegolnych wierzcholkow
        EdgeContainer& edgeTab, // i tablica dlugosci krawedzi
        typename GraphType::PVertex start, typename GraphType::PVertex end=0)
    // pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
    {   assert(start);
        int n=g.getVertNo();
        // w ramach swiatecznego pojednania postanowilem zaprzyjaznic sie z pamiecia dynamiczna,
        // uzywanie tylko stosu byloby mordega, ale zakladam, ze na poczatku alokujemy w kontenerach
        // raz i tyle, by pozniejsze realokacje byly zbedne.
        typename DefaultStructs::ArrayCont<typename GraphType::PVertex>::Type Q(n);
        typename GraphType::PVertex U=g.getVert(),V;

        for(int i=0;U;U=g.getVertNext(U))
        {   Q[i++]=U; vertTab[U].distance=
                    NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();
            vertTab[U].vPrev=0;vertTab[U].ePrev=0;
        }
        vertTab[start].distance=
                    NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        for(;n;n--){
            int i=n-1;
            typename EdgeContainer::ValType::DistType d=vertTab[Q[i]].distance,nd;
            for(int j=n-2;j>=0;j--) if (vertTab[Q[j]].distance<d) d=vertTab[Q[i=j]].distance;
            if (Q[i]==end) return vertTab[end].distance;
            if (NumberTypeBounds<typename EdgeContainer::ValType::DistType>::isPlusInfty(d))
                    return end ? d : NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();

            U=Q[i];
            for(int j=i;j<n-1;j++) Q[j]=Q[j+1];
            for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirOut|Koala::EdUndir);
                E;E=g.getEdgeNext(U,E,Koala::EdDirOut|Koala::EdUndir))
                    if ((nd=vertTab[U].distance+edgeTab[E].length)<
                         vertTab[V=g.getEdgeEnd(E,U)].distance)
                    { vertTab[V].distance=nd; vertTab[V].ePrev=E; vertTab[V].vPrev=U; }
        }
        return NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        // jesli wyznaczamy odleglosc do wszystkich wierzcholkow, a nie konkretnego
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


    // Uzytkownik podaje, gdzie wpisac wierzcholki i krawedzie najkrotszej sciezki
    template <class VIter, class EIter> struct OutPath {
        VIter vertIter;
        EIter edgeIter;

        OutPath(VIter av, EIter ei) : vertIter(av), edgeIter(ei) {}
    };
    // funkcja tworzaca, analogia make_pair
    template <class VIter, class EIter>
        static OutPath<VIter,EIter> outPath(VIter av, EIter ei) { return OutPath<VIter,EIter>(av,ei); }

// Ze wzgledow efektywnosciowych w przypadku procedur wieloetapowych, mozna rozdzielac je na kolejne fazy
// i zakladac, ze uzytkownik wywola je we wlasciwej kolejnosci, korzystajac z wczesniej uzyskanych danych.

    // wlasciwa procedura: zapisuje najkrotsza sciezke (wierzcholki i krawedzie) pod pare podanych iteratorow,
    // zwraca liczbe krawedzi najkrotszej sciezki. Korzysta z kontenera vertTab z danymi z poprzedniej funkcji
    template <class GraphType, class VertContainer, class VIter, class EIter>
    static int
        getPath(
        GraphType& g,
        VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        OutPath<VIter,EIter> iters)
    {   assert(start && end);
        if (NumberTypeBounds<typename VertContainer::ValType::DistType>::isPlusInfty(vertTab[end].distance))
            return -1; // wierzcholek end jest nieosiagalny

        typename GraphType::PVertex u,v=vertTab[end].vPrev;
        typename GraphType::PEdge  e=vertTab[end].ePrev;
        typename DefaultStructs::ArrayCont<typename GraphType::PVertex>::Type tabV(g.getVertNo());
        typename DefaultStructs::ArrayCont<typename GraphType::PEdge>::Type tabE(g.getVertNo());
        int len;

        for(len=0;v;len++)
           { tabV[len]=v; tabE[len]=e; e=vertTab[v].ePrev; v=vertTab[v].vPrev; }

        for(int i=len-1;i>=0;i--)
            { *iters.vertIter=tabV[i];*iters.edgeIter=tabE[i];++iters.vertIter;++iters.edgeIter; }
        *iters.vertIter=end;++iters.vertIter;
        return len;
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
        GraphType& g,
        EdgeContainer& edgeTab,
        typename GraphType::PVertex start, typename GraphType::PVertex end,
        OutPath<VIter,EIter> iters)
    {   assert(start && end);
        typename EdgeContainer::ValType::DistType dist;
        typename DefaultStructs::AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type vertTab;

        if (NumberTypeBounds<typename EdgeContainer::ValType::DistType >::isPlusInfty(dist
                                                            =distances(g,vertTab,edgeTab,start,end)))
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=getPath(g,vertTab,start,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};


}

#endif

