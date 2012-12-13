#include <iostream>
#include <string>
#include "Koala/base/defs.h"
#include "Koala/algorithm/search.h"
#include "Koala/graph/graph.h"

// To tylko proba ... algorytmu na tablicy asocjacyjnej z lokalnym buforem (bez new)

namespace Koala {

class DijDefault1 {
    public:

    template <class A, class B> class AssocCont {
        public:
        typedef AssocArray<A,B> Type;
//        typedef AssocTable < BiDiHashMap<A,B> > Type;
//            typedef PseudoAssocArray<A,B,AssocTable<std::map<A,int> > > Type;

    };


    // Specjalizacje dla wlasnych klas numerycznych (np. liczb wymiernych) pozwola uzywac ich jako danych
    // w algorytmach (np. dlugosci krawedzi). Dlatego w kodach nawet zerowosc jakiejs etykiety sprawdzam metoda
    template <class T> class NumberTypeBounds {
        public:

        static T plusInfty() { return std::numeric_limits<T>::max(); }
        static bool isPlusInfty(T arg) {return arg==plusInfty(); }
        static T minusInfty() { return std::numeric_limits<T>::min(); }
        static bool isMinusInfty(T arg) {return arg==minusInfty(); }
        static T zero() { return (T)0; }
        static bool isZero(T arg) {return arg==zero(); }
    };


};


class DijDefault2 {
    public:

    template <class A, class B> class AssocCont {
        public:
        typedef AssocArray<A,B,VectorInterface<Privates::BlockOfBlockList< BlockOfAssocArray< A,B > >*> > Type;
//        typedef AssocTable < BiDiHashMap<A,B> > Type;
//            typedef PseudoAssocArray<A,B,AssocTable<std::map<A,int> > > Type;

    };


    // Specjalizacje dla wlasnych klas numerycznych (np. liczb wymiernych) pozwola uzywac ich jako danych
    // w algorytmach (np. dlugosci krawedzi). Dlatego w kodach nawet zerowosc jakiejs etykiety sprawdzam metoda
    template <class T> class NumberTypeBounds {
        public:

        static T plusInfty() { return std::numeric_limits<T>::max(); }
        static bool isPlusInfty(T arg) {return arg==plusInfty(); }
        static T minusInfty() { return std::numeric_limits<T>::min(); }
        static bool isMinusInfty(T arg) {return arg==minusInfty(); }
        static T zero() { return (T)0; }
        static bool isZero(T arg) {return arg==zero(); }
    };


};


template <class DefaultStructs>
class DijkstraPar : public ShortPathStructs {

    public:


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

        VertLabs() :
        distance(DefaultStructs:: template NumberTypeBounds<DType>::plusInfty()),
        vPrev(0), ePrev(0) {}
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
        const typename EdgeContainer::ValType::DistType Zero=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::zero();
        const typename EdgeContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();

        int bufsize=Privates::AssocArrayVectIntSwitch<typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::isAAVI() ? g.getVertNo():0;
        Privates::BlockOfBlockList< BlockOfAssocArray< typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> > >
                    LOCALARRAY(buforl,(isBlackHole(avertTab) ? bufsize: 0));
        Privates::BlockOfBlockList< BlockOfAssocArray< typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> > >
                    LOCALARRAY(buforQ,bufsize);
        {
            typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                    VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type
                        localvertTab((isBlackHole(avertTab)? g.getVertNo():0),(typename Privates::AssocArrayVectIntSwitch<typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                            VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::BufType)buforl),
                        Q(g.getVertNo(),(typename Privates::AssocArrayVectIntSwitch<typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                            VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::BufType)buforQ);
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::Type &
                        vertTab=
                    BlackHoleSwitch<VertContainer,typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                    VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::get(avertTab,localvertTab);

            typename GraphType::PVertex U,V;

            if (isBlackHole(avertTab)) vertTab.reserve(g.getVertNo());

            Q[start].vPrev=0;Q[start].ePrev=0;
            Q[start].distance=Zero;

            while(!Q.empty()){
                typename EdgeContainer::ValType::DistType
                        d=PlusInfty,nd;
                for(V=Q.firstKey();V;V=Q.nextKey(V)) if (Q[V].distance<d) d=Q[U=V].distance;
                vertTab[U]=Q[U]; Q.delKey(U);
                if (U==end) return vertTab[end].distance;

                for(typename GraphType::PEdge E=g.getEdge(U,Koala::EdDirOut|Koala::EdUndir);
                    E;E=g.getEdgeNext(U,E,Koala::EdDirOut|Koala::EdUndir))
                        if (!vertTab.hasKey(V=g.getEdgeEnd(E,U)))
                            if ((nd=vertTab[U].distance+edgeTab[E].length)<Q[V].distance)
                        { Q[V].distance=nd; Q[V].ePrev=E; Q[V].vPrev=U; }
            }
        }

        return end ? PlusInfty : Zero;
    }



    template <class GraphType, class VertContainer, class VIter, class EIter>
    static int
        getPath(
        const GraphType& g,
        VertContainer& vertTab, // tablica asoc. z ustawionymi wskaznikami poprzednikow - rezultat poprzedniej funkcji
        typename GraphType::PVertex end,
        ShortPathStructs::OutPath<VIter,EIter> iters)
    {   assert(end);
        const typename VertContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename VertContainer::ValType::DistType>::plusInfty();

        if (PlusInfty==vertTab[end].distance)
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
        ShortPathStructs::OutPath<VIter,EIter> iters,bool useHeap=false)
    {   assert(start && end);
        const typename EdgeContainer::ValType::DistType PlusInfty=
            DefaultStructs:: template NumberTypeBounds<typename EdgeContainer::ValType::DistType>::plusInfty();

        typename EdgeContainer::ValType::DistType dist;
        Privates::BlockOfBlockList< BlockOfAssocArray< typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> > >
                    LOCALARRAY(bufor,(Privates::AssocArrayVectIntSwitch<typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::isAAVI() ? g.getVertNo():0));

        typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type
                    vertTab(g.getVertNo(),(typename Privates::AssocArrayVectIntSwitch<typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                        VertLabs<typename EdgeContainer::ValType::DistType ,GraphType> >::Type >::BufType)bufor);

        dist=distances(g,vertTab,edgeTab,start,end);

        if (PlusInfty==dist)
            return PathLengths<typename EdgeContainer::ValType::DistType> (dist,-1); // end nieosiagalny

        int len=getPath(g,vertTab,end,iters);
        return PathLengths<typename EdgeContainer::ValType::DistType>(dist,len);
        // dlugosc najkr. siezki i jej liczba krawedzi
    }

};

class Dijkstra : public DijkstraPar<DijDefault2> {};

}

//przyklad uzycia, w tej wersji zawartosc obu struktur jest dla Dijkstry nieistotna
struct OpisV;  // pewna struktura opisujaca moj wierzchlek
struct OpisE;   // struktura opisujaca krawedz

struct OpisV {
    OpisV(std::string aname="") : name(aname) {}

    std::string name;
};

struct OpisE {
    OpisE(int len=0) : dlugosc(len) {}
    bool cokolwiek;
    int dlugosc; // dla Dijkstry niepotrzebne
};

Koala::Graph<OpisV,OpisE> g;
Koala::Graph<OpisV,OpisE>::PVertex A,B,C,D,E,F,V,U,tabV[10];
Koala::Graph<OpisV,OpisE>::PEdge tabE[10];


Koala::AssocArray<Koala::Graph<OpisV,OpisE>::PVertex,
            Koala::Dijkstra::VertLabs<int, Koala::Graph<OpisV,OpisE> > >  vertCont;
Koala::AssocArray<Koala::Graph<OpisV,OpisE>::PEdge,Koala::Dijkstra::EdgeLabs<int> >  edgeCont;

//Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PVertex,
//            Koala::Dijkstra::VertLabs<int, Koala::Graph<OpisV,OpisE> > > > vertCont;
//Koala::AssocTable<std::map<Koala::Graph<OpisV,OpisE>::PEdge,Koala::Dijkstra::EdgeLabs<int> > > edgeCont;


void dijTest()
{   g.clear();vertCont.clear();
    A=g.addVert(OpisV("A"));B=g.addVert(OpisV("B"));C=g.addVert(OpisV("C"));
    D=g.addVert(OpisV("D"));E=g.addVert(OpisV("E"));F=g.addVert(OpisV("F"));

    g.addEdge(A,B,OpisE(1));g.addEdge(B,D,OpisE(5));g.addEdge(A,C,OpisE(4));g.addEdge(D,C,OpisE(2));
    g.addEdge(C,E,OpisE(1));g.addEdge(B,C,OpisE(2),Koala::EdDirOut);g.addEdge(D,E,OpisE(1),Koala::EdDirOut);
    g.addEdge(F,D,OpisE(2),Koala::EdDirOut);

    for(Koala::Graph<OpisV,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        edgeCont[ePt].length=ePt->info.dlugosc;

    U=A; V=E;
    for(int i=0;i<10;i++) {tabV[i]=0; tabE[i]=0; }
}

int main() {

    g.makeAdjMatrix();
    dijTest();

    std::cout<< "Odleglosc: "<< Koala::Dijkstra::distances(g,vertCont,edgeCont,U,V)<<":";
//       - mozna i bez ostatniego arg. - liczymy z U do wszystkich
    std::cout << "\nE:" << vertCont[E].distance << " F:" << vertCont[F].distance << std::endl;

    // pobieramy wyznaczona wczesniej sicezke

    if (V) {
        int l;

        std::cout<< std::endl;
//        l=Koala::Dijkstra::getOutPath(g,vertCont,Koala::Dijkstra::outPath(tabV,tabE),V,F);
        l=Koala::Dijkstra::getPath(g,vertCont,V,Koala::Dijkstra::outPath(tabV,tabE));
        std::cout<<"Liczba krawedzi: "<<l<<std::endl;
        for(int i=0;i<=l;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<l;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

//    std::cout << Koala::Dijkstra::getUsedEdgesSet(g,vertCont);

    // ten sam graf raz jeszcze, szukamy sciezki od nowa

    if (V) {
        Koala::Dijkstra::PathLengths<int> res;

        std::cout<< std::endl;g.clear(); dijTest();
        res=Koala::Dijkstra::findPath(g,edgeCont,U,V,Koala::Dijkstra::outPath(tabV,tabE));
        std::cout<<"Odleglosc: "<< res.length<<std::endl;
        for(int i=0;i<=res.edgeNo;i++) std::cout<< tabV[i]->info.name; std::cout<< std::endl;
        for(int i=0;i<res.edgeNo;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }


    // ten sam graf raz jeszcze, kogos nie interesowala sekwencja wierzcholkow

    if (V) {
        Koala::Dijkstra::PathLengths<int> res;

        std::cout<< std::endl;g.clear(); dijTest();
        res=Koala::Dijkstra::findPath(g,edgeCont,U,V,Koala::Dijkstra::outPath(blackHole,tabE));
        std::cout<<"Odleglosc: "<< res.length<<std::endl;
        for(int i=0;i<res.edgeNo;i++) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info.name <<
                            "," << g.getEdgeEnds(tabE[i]).second->info.name << "}";
        std::cout<< std::endl;
    }

    // ... interesowala go tylko odleglosc, nie chcialo mu sie zakladac kontenera asoc. dla wierzcholkow
    std::cout<< std::endl;
    dijTest();
    std::cout<< "Odleglosc: "<< Koala::Dijkstra::distances(g,blackHole,edgeCont,U,V)<<":";

}
