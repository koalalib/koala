#ifndef KOALA_GRAPH_H
#define KOALA_GRAPH_H

/* ------------------------------------------------------------------------- *
 * graph.h
 *
 * Moduł definujący graf oraz wszystkie podstawowe operacje wykonywane na
 * grafach.
 *
 * ------------------------------------------------------------------------- */

#include <algorithm>
#include <cstdio>
#include <cassert>


#include "../base/def_struct.h"


namespace Koala {

template <EdgeType edAllow, bool adjMatrixAllowed>
class DefaultGrSettings {
    public:

    enum  {  EdAllow=edAllow };
    enum  {  AdjMatrixAllowed=adjMatrixAllowed };

    template <class A, class B> class VertAssocCont {
        public:
        typedef AssocArray<A,B> Type;
//        typedef AssocTable<std::map<A,B> > Type;
    };

    template <class A, class B> class ExtVertAssocCont {
        public:
        typedef AssocArray<A,B> Type;
//            typedef AssocTable<std::map<A,B> > Type;
    };

    template <class K, class V> class AdjMatrixDirEdges {
        public:
        typedef AssocMatrix<K,V,AMatrNoDiag> Type;
//        typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
    };

    template <class K, class V> class AdjMatrixUndirEdges {
        public:
        typedef AssocMatrix<K,V,AMatrTriangle> Type;
//        typedef AssocMatrix<K,V,AMatrTriangle,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
    };

    template<class VertInfo, class EdgeInfo, class Settings > struct VertAdditData {
        AssocKeyContReg assocReg;
    };

    template<class VertInfo, class EdgeInfo, class Settings > struct EdgeAdditData {
        AssocKeyContReg assocReg;
    };


    template <class Iterator>
    static void sort ( Iterator first, Iterator last )
    { std::make_heap(first,last); std::sort_heap(first,last);}

    template <class Iterator, class Comp>
    static void sort ( Iterator first, Iterator last, Comp comp )
    { std::make_heap(first,last,comp); std::sort_heap(first,last,comp);}

};


}


#include "adjmatrix.h"
#include "vertex.h"
#include "edge.h"

namespace Koala
{

/* ------------------------------------------------------------------------- *
 * SubgraphBase
 *
 * Widok na podgraf, umożliwiający przeglądanie podgrafu bez kopiowania.
 *
 * ------------------------------------------------------------------------- */

class SubgraphBase
{
    public:
        SubgraphBase(): parent( NULL ), next( NULL ), child( NULL ) { }
        SubgraphBase( const SubgraphBase & );
        SubgraphBase &operator=( const SubgraphBase & );

        ~SubgraphBase();

    protected:
        mutable const SubgraphBase *parent, *next, *child;

        bool unlink();
        bool link(const SubgraphBase * = NULL );
} ;


namespace Privates {


template <EdgeType EdgeAllow>
class EdgeCounterLoop
{   protected:

    typedef int& NoType;

    mutable int n;
    int & no() const { return n; }
    EdgeCounterLoop(): n(0) {}
};

template <>
class EdgeCounterLoop<0>
{   protected:

    typedef DummyVar<int> NoType;

//    int & no() const { return _KoalaEmptyVertDegree; }
    DummyVar<int> no() const { return DummyVar<int>(); }
};

template <EdgeType EdgeAllow>
class EdgeCounterDir;

template <>
class EdgeCounterDir<EdDirIn|EdDirOut>
{   protected:

    typedef int& NoType;

    mutable int n;
    int & no() const { return n; }
    EdgeCounterDir(): n(0) {}
};

template <>
class EdgeCounterDir<0>
{   protected:

    typedef DummyVar<int> NoType;

//    int & no() const { return _KoalaEmptyVertDegree; }
    DummyVar<int> no() const { return DummyVar<int>(); }
};

template <EdgeType EdgeAllow>
class EdgeCounterUndir
{   protected:

    typedef int& NoType;

    mutable int n;
    int & no() const { return n; }
    EdgeCounterUndir(): n(0) {}
};

template <>
class EdgeCounterUndir<0>
{   protected:

    typedef DummyVar<int> NoType;

//    int & no() const { return _KoalaEmptyVertDegree; }
    DummyVar<int> no() const { return DummyVar<int>(); }
};

}



/* ------------------------------------------------------------------------- *
 * Graph< VertInfo,EdgeInfo,EdAllow >
 *
 * Graf wraz z wszystkimi podstawowymi operacjami wykonywanymi na grafach.
 * Parametry grafu to klasy, których instancje będą etykietami wierzchołków
 * i krawędzi.
 *
 * ------------------------------------------------------------------------- */

template< class VertInfo = EmptyVertInfo, class EdgeInfo = EmptyVertInfo, class Settings = DefaultGrSettings<EdAll,true> >
class Graph: public SubgraphBase,
    protected Privates::EdgeCounterLoop<Settings::EdAllow & Loop>,
    protected Privates::EdgeCounterDir<Settings::EdAllow & (EdDirIn|EdDirOut)>,
    protected Privates::EdgeCounterUndir<Settings::EdAllow & Undirected>
{
    public:
        friend class Edge< VertInfo,EdgeInfo,Settings >;

        typedef Koala::Vertex< VertInfo,EdgeInfo,Settings> Vertex;
        typedef Vertex* PVertex;
        typedef Koala::Edge< VertInfo,EdgeInfo,Settings > Edge;
        typedef Edge* PEdge;
        typedef Graph< VertInfo,EdgeInfo,Settings > GraphType;
        typedef Graph< VertInfo,EdgeInfo,Settings > RootGrType;
        typedef VertInfo VertInfoType;
        typedef EdgeInfo EdgeInfoType;
        typedef Settings GraphSettings;

    // Konstruktory
        // Tworzy pusty graf, bez krawędzi i wierzchołków.
        Graph();
        // Konstruktor kopiujący.
        Graph( const Graph & );
    // Destruktor
        ~Graph();
        // Usuwamy wszystkie wierzchołki i krawędzie grafu.
        void clear();
        // Usuwamy wszystkie wierzchołki grafu.
        void clearEdges();
            // Operacje na całym grafie
        // Operator przypisania.
        Graph< VertInfo,EdgeInfo,Settings > &operator=( const Graph< VertInfo,EdgeInfo,Settings > & );
        // Suma grafów.
        Graph< VertInfo,EdgeInfo,Settings > &operator+=( const Graph< VertInfo,EdgeInfo,Settings > & );
        // Przenosimy wszystkie wierzchołki i krawędzie do naszego grafu.
        PVertex move( Graph< VertInfo,EdgeInfo,Settings > & );


        static EdgeType allowedEdgeTypes() { return Settings::EdAllow ; }


        // Liczba wierzchołków (rząd grafu).
        inline int getVertNo() const;
        // Pierwszy wierzchołek grafu.
        inline PVertex getVert() const;
        // Ostatni wierzchołek grafu.
        inline PVertex getVertLast() const;
        // Następny i poprzedni wierzchołek w grafie.
        inline PVertex getVertNext( PVertex ) const;
        inline PVertex getVertPrev( PVertex ) const;
        // Zbiór wierzchołków grafu.
        Set< PVertex > getVertSet() const;
        // Kopiujemy wierzchołki grafu do iteratora.
        template< class OutputIterator > int getVerts( OutputIterator ) const;
        // wierzcholki i krawedzie wybierane z grafu chooserami
        template< class OutputIterator,class VChooser > int getVerts(OutputIterator out, VChooser ch) const;
        template< class VChooser> Set< PVertex > getVertSet(VChooser ch)    const;
        // Wierzchołek o podanym numerze.
        PVertex vertByNo( int ) const;
        // Numer wierzchołka.
        int vertPos( PVertex ) const;


        // Liczba krawędzi podanego typu.
        inline int getEdgeNo( EdgeType = EdAll ) const;
        // Pierwsza krawędź podanego typu.
        inline PEdge getEdge( EdgeType = EdAll ) const;
        // Ostatnia krawędź podanego typu.
        inline PEdge getEdgeLast( EdgeType = EdAll )const;
        // Następna i poprzednia krawędź podanego typu.
        inline PEdge getEdgeNext( PEdge,EdgeType = EdAll ) const;
        inline PEdge getEdgePrev( PEdge,EdgeType = EdAll ) const;
        // Zbiór krawędzi podanego typu.
        Set< PEdge > getEdgeSet( EdgeType = EdAll ) const;
        // Kopiujemy krawędzie podanego typu do iteratora.
        template< class OutputIterator >
        int getEdges( OutputIterator, EdgeType = EdAll ) const;
        template< class OutputIterator,class EChooser > int getEdges(OutputIterator out, EChooser ch) const;
        template< class EChooser> Set< PEdge > getEdgeSet(EChooser ch)    const;
        // Krawędź o podanym numerze.
        PEdge edgeByNo( int ) const;
        // Numer krawędzi.
        int edgePos( PEdge ) const;



        template< class OutputV,class OutputE,class VChooser,class EChooser >
            std::pair<int,int> getChosen(std::pair<OutputV,OutputE>,std::pair<VChooser,EChooser>,bool =true) const;
        template<class VChooser,class EChooser >
            std::pair<Set<PVertex>,Set<PEdge> >getChosenSets(std::pair<VChooser,EChooser>,bool =true) const;


    // Relacje pomiędzy wierzchołkami a krawędziami
// Liczba krawędzi incydentnych z wierzchołkiem.
        inline int getEdgeNo( PVertex, EdgeDirection = EdAll ) const;
        // Pierwsza krawędź incydentna z wierzchołkiem.
        inline PEdge getEdge( PVertex, EdgeDirection = EdAll ) const;
        // Ostatnia krawędź incydentna z wierzchołkiem.
        inline PEdge getEdgeLast( PVertex, EdgeDirection = EdAll ) const;
        // Następna krawędź incydentna z wierzchołkiem.
        PEdge getEdgeNext( PVertex, PEdge, EdgeDirection = EdAll ) const;
        // Poprzednia krawędź incydentna z wierzchołkiem.
        PEdge getEdgePrev( PVertex, PEdge, EdgeDirection = EdAll ) const;
        // Zbiór krawędzi incydentnych z wierzchołkiem.
        Set< PEdge > getEdgeSet( PVertex, EdgeDirection = EdAll ) const;
        // Kopiujemy krawędzie incydentne z wierzchołkiem do iteratora.
        template< class OutputIterator > int getEdges(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;


        // Liczba krawędzi łączących parę wierzchołków.
        int getEdgeNo( PVertex, PVertex, EdgeDirection = EdAll ) const;
        // Pierwsza krawędź łącząca parę wierzchołków.
        inline PEdge getEdge( PVertex, PVertex, EdgeDirection = EdAll ) const;
        // Ostatnia krawędź łącząca parę wierzchołków.
        inline PEdge getEdgeLast( PVertex, PVertex, EdgeDirection = EdAll )const;
        // Następna krawędź łącząca parę wierzchołków.
        PEdge getEdgeNext( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        // Poprzednia krawędź łącząca parę wierzchołków.
        PEdge getEdgePrev( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        // Zbiór krawędzi łączących parę wierzchołków.
        Set< PEdge > getEdgeSet( PVertex, PVertex, EdgeDirection = EdAll ) const;
        // Kopiujemy krawędzie łączące parę wierzchołków do iteratora.
        template< class OutputIterator > int getEdges(
            OutputIterator, PVertex, PVertex, EdgeDirection = EdAll ) const;


        // Końce krawędzi.
        // Wpisuje do kontenera relacje sasiedztwa miedzy wierzcholkami
        template <class Cont>
            void getAdj(Cont &,EdgeType mask=EdAll) const;


        inline VertInfoType getVertInfo( PVertex ) const;
        // Zmieniamy etykietę wierzchołka.
        inline void setVertInfo( PVertex, const VertInfo & = VertInfo() ) const;
        // Etykieta krawędzi.
        inline EdgeInfoType getEdgeInfo( PEdge ) const;
        // Zmieniamy etykietę krawędzi.
        inline void setEdgeInfo( PEdge, const EdgeInfo & = EdgeInfo() ) const;


        // Typ krawędzi.
        inline EdgeType getEdgeType( PEdge ) const;
        inline EdgeType getType( PEdge ) const;
        inline std::pair< PVertex,PVertex > getEdgeEnds( PEdge ) const;
        inline std::pair< PVertex,PVertex > getEnds( PEdge ) const;
        inline PVertex getEdgeEnd1( PEdge ) const;
        inline PVertex getEdgeEnd2( PEdge ) const;
        // Informacja o tym, czy wierzchołek jest incydentny z krawędzią.
        inline bool isEdgeEnd( PEdge, PVertex ) const;
        inline bool isEnd( PEdge, PVertex ) const;
        // Drugi z końców krawędzi.
        inline PVertex getEdgeEnd( PEdge, PVertex ) const;
        inline PVertex getEnd( PEdge, PVertex ) const;
        // Typ krawędzi.
        inline EdgeDirection getEdgeDir( PEdge, PVertex ) const;
        inline bool incid(PEdge,PEdge) const;


        // Stopień wierzchołka.
        inline int deg( PVertex, EdgeDirection = EdAll ) const;
        inline int Delta( EdgeDirection = EdAll ) const;
        inline int delta( EdgeDirection = EdAll ) const;
        // Wierzchołek o minimalnym stopniu (i jego stopień).
        std::pair< PVertex,int > minDeg( EdgeDirection = EdAll ) const;
        // Wierzchołek o maksymalnym stopniu (i jego stopień).
        std::pair< PVertex,int > maxDeg( EdgeDirection = EdAll ) const;
        // Stopień i minimalny stopień w grafie.


        // Sąsiedztwo wierzchołka.
        Set< PVertex > getNeighSet( PVertex, EdgeDirection = EdAll ) const;
        // Kopiujemy sąsiedztwo wierzchołka do iteratora.
        template< class OutputIterator > int getNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        // Liczba wierzchołków w sąsiedztwie.
        inline int getNeighNo( PVertex, EdgeDirection = EdAll ) const;
        // Domknięte sąsiedztwo wierzchołka.
        Set< PVertex > getClNeighSet( PVertex, EdgeDirection = EdAll ) const;
        // Kopiujemy domknięte sąsiedztwo do iteratora.
        template< class OutputIterator > int getClNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        // Liczba wierzchołków w domkniętym sąsiedztwie.
        inline int getClNeighNo( PVertex, EdgeDirection = EdAll ) const;


        // Informacja o tym, czy krawędzie są równoległe.
        //relacja rownowaznosci, 3 mozliwosci
        //reltype - dopuszczalne tylko jednobitowe: EdDirIn, EdDirOut lub EdUndir
        //EdDirOut - luk nieskierowany jest rownolegly tylko do tak samo jak on skierowanego luku o tych samych koncach
        //EdDirIn - jest on takze rownolegly do odwrotnie skierowanego luku o tych samych koncach
        //EdUndir - jest on takze rownolegly do krawedzi nieskierowanej o tych samych koncach
        // Czy dwie krawedzie sa rownolegle w powyzszym sensie
        bool areParallel( PEdge, PEdge, EdgeDirection = EdUndir ) const;
        // Pobierz ze zbioru krawedzie rownolegle do podanej (bez niej samej, jesli w nim wystepuje)
        template< class OutputIterator > int getParals( OutputIterator, PEdge, EdgeDirection = EdUndir ) const;
        Set<PEdge> getParalSet(PEdge, EdgeDirection = EdUndir) const;
        // liczba krawedzi rownoleglych do podanej (z nia wlacznie)
        inline int mu( PEdge, EdgeDirection = EdUndir ) const;
        // maksymalna liczba krawedzi rownoleglych (w powyzszym sensie)
        inline int mu( EdgeDirection = EdUndir ) const;
        std::pair< PEdge,int > maxMu( EdgeDirection = EdUndir ) const;


        template<class IterOut1, class IterOut2, class Iterator >
            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,Iterator,Iterator, EdgeType = EdUndir ) const;
        template<class IterOut1, class IterOut2, class Iterator >
            std::pair<int,int> findParals2(std::pair<IterOut1,IterOut2>,Iterator,Iterator, EdgeType = EdUndir ) const;
        template<class IterOut1, class IterOut2 >
            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,const Set<PEdge>&, EdgeType = EdUndir ) const;
        template<class IterOut1, class IterOut2 >
            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,PVertex, EdgeType = EdUndir ) const;
        template<class IterOut1, class IterOut2 >
            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,PVertex,PVertex, EdgeType = EdUndir ) const;
        template<class IterOut1, class IterOut2 >
            std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>, EdgeType = EdUndir ) const;


        // krawedzie danego typu incydentne z danym zbiorem wierzcholkow
        template <class Iterator,class OutIter>
            int getIncEdges(OutIter,Iterator, Iterator, EdgeDirection = EdAll,EdgeType=Loop ) const;
        template <class Iterator>
            Set< PEdge > getIncEdgeSet(Iterator beg, Iterator end, EdgeDirection = EdAll,EdgeType=Loop ) const;
        template <class OutIter>
            int getIncEdges(OutIter,const Set<PVertex>&, EdgeDirection = EdAll,EdgeType=Loop ) const;
        Set< PEdge > getIncEdgeSet(const Set<PVertex>&, EdgeDirection = EdAll, EdgeType=Loop ) const;


        // ... i ich drugie konce
        template <class Iterator,class OutIter> int getIncVerts(OutIter,Iterator, Iterator, EdgeDirection = EdAll,EdgeType=Loop ) const;
        template <class Iterator> Set< PVertex > getIncVertSet(Iterator beg, Iterator end, EdgeDirection = EdAll,EdgeType=Loop ) const;
        template <class OutIter> int getIncVerts(OutIter,const Set<PVertex>&, EdgeDirection = EdAll,EdgeType=Loop ) const;
        Set< PVertex > getIncVertSet(const Set<PVertex>&, EdgeDirection = EdAll,EdgeType=Loop ) const;


        // Usuwamy wierzchołek z grafu.
        inline void del( PVertex, bool = true );
        void delVert( PVertex, bool = true );
        template< class Iterator > int delVerts( Iterator begin, Iterator end );
        template< class Iterator > int delVerts2( Iterator begin, Iterator end );
        inline int delVerts( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex> & );
        // Usuwamy wierzchołki z grafu.
        inline int delVerts();


        // Usuwamy krawędź z grafu.
        inline void del( PEdge );
        inline void delEdge( PEdge );
        int delEdges( PVertex, EdgeDirection = EdAll );
        int delEdges( PVertex, PVertex, EdgeDirection = EdAll );
        template< class Iterator > int delEdges(
            Iterator begin, Iterator end, EdgeType = EdAll );
        template< class Iterator > int delEdges2(
            Iterator begin, Iterator end, EdgeType = EdAll );
        inline int delEdges(
            const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &,
            EdgeType = EdAll );
        // Usuwamy krawędzie z grafu.
        int delEdges( EdgeType = EdAll );


    // Operacje na wierzchołkach grafu
        // Dodajemy nowy wierzchołek do grafu (z etykietą).
        inline PVertex addVert( const VertInfo & = VertInfo() );
        // Etykieta wierzchołka.


    // Operacje na krawędziach grafu
        // Dodajemy nową krawędź do grafu (bez etykiety).
        inline PEdge addEdge( PVertex, PVertex, EdgeDirection = EdUndir );
        // Dodajemy nową krawędź do grafu (z etykietą).
        inline PEdge addEdge( PVertex, PVertex, const EdgeInfo &, EdgeDirection = EdUndir );
        // Dodajemy nowy łuk do grafu (z etykietą).
        inline PEdge addArch( PVertex, PVertex, const EdgeInfo & = EdgeInfo() );
        // Dodajemy nową pętlę do grafu (bez etykiety).
        inline PEdge addLoop( PVertex, const EdgeInfo & = EdgeInfo() );


        // Przenosimy krawędź w inne miejsce.
        inline bool moveEdge( PEdge, PVertex, PVertex, EdgeDirection = EdUndir );

        // zmiana krawedzi skierowanej na nieskierowana
        bool ch2Undir(PEdge);
        int ch2Undir(PVertex, EdgeDirection=EdDirIn|EdDirOut);
        int ch2Undir(PVertex,PVertex, EdgeDirection=EdDirIn|EdDirOut);
        template <class Iterator> int ch2Undir(Iterator,Iterator);
        inline int ch2Undir(const Set<PEdge>&);
        int ch2Undir();

        // Zmieniamy kierunek krawędzi skierowanej
        inline bool revert( PEdge );
        int revert(PVertex, EdgeDirection=EdDirIn|EdDirOut);
        int revert(PVertex,PVertex, EdgeDirection=EdDirIn|EdDirOut);
        template <class Iterator> int revert(Iterator,Iterator);
        template <class Iterator> int revert2(Iterator,Iterator);
        inline int revert(const Set<PEdge>&);
        int revert();

        // zmiana krawedzi nieskierowanej na luk w danym kierunku
        inline bool ch2Dir(PEdge,PVertex,EdgeDirection=EdDirOut);
        int ch2Dir(PVertex,EdgeDirection=EdDirOut);
        int ch2Dir(PVertex,PVertex,EdgeDirection=EdDirOut);

        // Zmieniamy krawędź nieskierowana w dwa łuki.
        inline PEdge ch2Archs( PEdge );
        int ch2Archs(PVertex);
        int ch2Archs(PVertex,PVertex);
        template< class Iterator > int ch2Archs( Iterator begin, Iterator end );
        inline int ch2Archs( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> & );
        int ch2Archs();


        // ustawienie nowego wierzcholka na srodku istniejacej krawedzi
        PVertex putVert( PEdge,const VertInfo & = VertInfo() );
        // .. i zdjecie wierzcholka z krawedzi, po czym pozostaje jedna krawedz
        PEdge pickVert( PVertex, const EdgeInfo & = EdgeInfo() );


        // sklejanie grupy wierzcholkow w jeden wierzcholek docelowy (mozna wybrac ten wierzcholek sposrod sklejanej grupy)
        inline PVertex glue( PVertex, PVertex, bool makeloops = false );
        template< class Iterator > PVertex glue(
            Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        template< class Iterator > PVertex glue2(
            Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        inline PVertex glue(
            const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex> &,
            bool makeloops = false, PVertex = NULL );


        // usuwanie krawedzi rownoleglych do podanej
        int delParals(PEdge,EdgeDirection = EdUndir);
        template< class Iterator > int delParals(Iterator begin,Iterator end,PEdge,EdgeDirection = EdUndir);
        template< class Iterator > int delParals2(Iterator begin,Iterator end,PEdge,EdgeDirection = EdUndir);
        inline int delParals(const Set<PEdge>&,PEdge,EdgeDirection = EdUndir);


        template<class Iterator > int delAllParals(Iterator,Iterator, EdgeType = EdUndir );
        template<class Iterator > int delAllParals2(Iterator,Iterator, EdgeType = EdUndir );
        int delAllParals(const Set<PEdge>&, EdgeType = EdUndir );
        int delAllParals(PVertex, EdgeType = EdUndir );
        int delAllParals(PVertex,PVertex, EdgeType = EdUndir );
        int delAllParals(EdgeType = EdUndir );


        template <class Iterator> int delIncEdges(Iterator, Iterator, EdgeDirection = EdAll, EdgeType=Loop );
        int delIncEdges(const Set<PVertex>&, EdgeDirection = EdAll, EdgeType=Loop );


        // dopelnienia podgrafu indukowanego na zadanych wierzcholkach
        template <class Iterator,class EdInfoGen> void neg(Iterator, Iterator, EdgeType, EdInfoGen);
        template <class Iterator> void neg(Iterator, Iterator, EdgeType=Settings::EdAllow,EdgeInfo=EdgeInfo());
        template <class EdInfoGen> void neg(const Set<PVertex>&, EdgeType,EdInfoGen infoGen);
        void neg(const Set<PVertex>& vset, EdgeType=Settings::EdAllow,EdgeInfo=EdgeInfo());
        // ... i calego grafu
        template <class EdInfoGen> void neg(EdgeType type, EdInfoGen infoGen);
        void neg(EdgeType type=Settings::EdAllow, EdgeInfo=EdgeInfo());


        // Kopiujemy wierzchołki i krawędzie do innego grafu.
        template< class ExtGraph > PVertex copy( const ExtGraph & );
        template< class ExtGraph, class VChooser, class EChooser >
            PVertex copy( const ExtGraph &, std::pair< VChooser,EChooser > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster,
            class ECaster >
            PVertex copy( const ExtGraph &, std::pair< VChooser,EChooser >,
                std::pair< VCaster,ECaster > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster,
            class ECaster, class VLinker, class ELinker >
            PVertex copy( ExtGraph &, std::pair< VChooser,EChooser >,
                std::pair< VCaster,ECaster >, std::pair< VLinker,ELinker > );
        // Wstawiamy do grafu inny graf zamiast wierzchołka.
        template< class ExtGraph > PVertex substitute( PVertex, ExtGraph & );
        template< class ExtGraph, class VChooser, class EChooser >
            PVertex substitute( PVertex, ExtGraph &, std::pair< VChooser,EChooser > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster,
            class ECaster >
            PVertex substitute( PVertex, ExtGraph &, std::pair< VChooser,EChooser >,
                std::pair< VCaster,ECaster > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster,
            class ECaster, class VLinker, class ELinker >
            PVertex substitute( PVertex, ExtGraph &, std::pair< VChooser,EChooser >,
                std::pair< VCaster,ECaster >, std::pair< VLinker,ELinker > );

    // Operacje na macierzy sąsiedztwa
        // Tworzymy macierz sąsiedztwa.
        bool makeAdjMatrix();
        // Usuwamy macierz sąsiedztwa.
        inline bool delAdjMatrix();
        // Informacja o tym, czy graf ma macierz sąsiedztwa.
        inline bool hasAdjMatrix() const;
        // porzadkowanie macierzy sasiedztwa
        inline bool defragAdjMatrix();
        // czy ten typ grafu obsluguje macierz sasiedztwa
        static bool allowedAdjMatrix() { return Settings::AdjMatrixAllowed; }
        inline void reserveAdjMatrix(int);

    // ??
        const Graph< VertInfo,EdgeInfo,Settings > *getRootPtr() const { return this; }
        bool good( PVertex,bool=false ) const { return true; }
        bool good( PEdge,bool=false ) const { return true; }


    private:

        AdjMatrix <VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed>* pAdj;

       template <class Container> int delParals(PVertex,EdgeDirection,Container &);

        struct Parals3 {
            typename Koala::Vertex< VertInfo,EdgeInfo,Settings > *v1,*v2;
            EdgeDirection direct;
            typename Koala::Edge< VertInfo,EdgeInfo,Settings > *edge;

            Parals3(typename Koala::Vertex< VertInfo,EdgeInfo,Settings > *av1,
                    typename Koala::Vertex< VertInfo,EdgeInfo,Settings > *av2,
                    EdgeDirection adirect,
                    typename Koala::Edge< VertInfo,EdgeInfo,Settings > *aedge) :
                    v1(av1), v2(av2), direct(adirect), edge(aedge) {}
            Parals3() {}
        };

        struct Parals3cmp {
            bool operator()(Parals3 a, Parals3 b)
            {
                return (a.v1 < b.v1) ||
                (a.v1 == b.v1 && a.v2 < b.v2) ||
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct < b.direct ) ||
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct == b.direct && a.edge<b.edge);
            }

        };

        PVertex first_vert, last_vert;
        PEdge first_edge, last_edge;
//        int&
        typename Privates::EdgeCounterLoop<Settings::EdAllow & Loop>::NoType
            no_loop_edge() const { return this->Privates::EdgeCounterLoop<Settings::EdAllow & Loop>::no(); }
//        int&
        typename Privates::EdgeCounterDir<Settings::EdAllow & (EdDirIn|EdDirOut)>::NoType
            no_dir_edge() const { return this->Privates::EdgeCounterDir<Settings::EdAllow & (EdDirIn|EdDirOut)>::no(); }
//        int&
        typename Privates::EdgeCounterUndir<Settings::EdAllow & Undirected>::NoType
            no_undir_edge() const { return this->Privates::EdgeCounterUndir<Settings::EdAllow & Undirected>::no(); }
        int no_vert;

        inline PVertex attach( PVertex );
        inline PVertex detach( PVertex );
        inline PEdge attach( PEdge, PVertex, PVertex, EdgeDirection );
        PEdge attach_undir( PEdge, PVertex, PVertex );
        PEdge attach_dir( PEdge, PVertex, PVertex );
        PEdge attach_loop( PEdge, PVertex );
        PEdge detach( PEdge );
} ;

#include "graph.hpp"
//#include "graph_test.hpp"

}

#endif
