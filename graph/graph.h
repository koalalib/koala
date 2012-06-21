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


#ifndef KOALA_GRAPH_PARAMS_DEFINED

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

#endif

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

    mutable int n;
    int & no() const { return n; }
    EdgeCounterLoop(): n(0) {}
};

template <>
class EdgeCounterLoop<0>
{   protected:

    int & no() const { return _KoalaEmptyVertDegree; }
};

template <EdgeType EdgeAllow>
class EdgeCounterDir;

template <>
class EdgeCounterDir<EdDirIn|EdDirOut>
{   protected:

    mutable int n;
    int & no() const { return n; }
    EdgeCounterDir(): n(0) {}
};

template <>
class EdgeCounterDir<0>
{   protected:

    int & no() const { return _KoalaEmptyVertDegree; }
};

template <EdgeType EdgeAllow>
class EdgeCounterUndir
{   protected:

    mutable int n;
    int & no() const { return n; }
    EdgeCounterUndir(): n(0) {}
};

template <>
class EdgeCounterUndir<0>
{   protected:

    int & no() const { return _KoalaEmptyVertDegree; }
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

    // Parametry grafu
        static EdgeType allowedEdgeTypes() { return Settings::EdAllow ; }
        // Liczba wierzchołków (rząd grafu).
        inline int getVertNo() const;
        // Liczba krawędzi podanego typu.
        inline int getEdgeNo( EdgeDirection = EdAll ) const;
        // Wierzchołek o minimalnym stopniu (i jego stopień).
        std::pair< PVertex,int > minDeg( EdgeDirection = EdAll ) const;
        // Wierzchołek o maksymalnym stopniu (i jego stopień).
        std::pair< PVertex,int > maxDeg( EdgeDirection = EdAll ) const;
        // Stopień i minimalny stopień w grafie.
        inline int Delta( EdgeDirection = EdAll ) const;
        inline int delta( EdgeDirection = EdAll ) const;

    // Operacje na całym grafie
        // Usuwamy wszystkie wierzchołki i krawędzie grafu.
        void clear();
        // Usuwamy wszystkie wierzchołki grafu.
        void clearEdges();
        // Operator przypisania.
        Graph< VertInfo,EdgeInfo,Settings > &operator=( const Graph< VertInfo,EdgeInfo,Settings > & );
        // Suma grafów.
        Graph< VertInfo,EdgeInfo,Settings > &operator+=( const Graph< VertInfo,EdgeInfo,Settings > & );
        // Przenosimy wszystkie wierzchołki i krawędzie do naszego grafu.
        PVertex move( Graph< VertInfo,EdgeInfo,Settings > & );
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

    // Operacje na wierzchołkach grafu
        // Dodajemy nowy wierzchołek do grafu (z etykietą).
        inline PVertex addVert( const VertInfo & = VertInfo() );
        // Usuwamy wierzchołek z grafu.
        inline void del( PVertex, bool = true );
        void delVert( PVertex, bool = true );
        // Etykieta wierzchołka.
        inline VertInfoType getVertInfo( PVertex ) const;
        // Zmieniamy etykietę wierzchołka.
        inline void setVertInfo( PVertex, const VertInfo & = VertInfo() ) const;
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
        // Liczba krawędzi incydentnych z wierzchołkiem.
        inline int getEdgeNo( PVertex, EdgeDirection = EdAll ) const;
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
        // Stopień wierzchołka.
        inline int deg( PVertex, EdgeDirection = EdAll ) const;
        // Wierzchołek o podanym numerze.
        PVertex vertByNo( int ) const;
        // Numer wierzchołka.
        int vertPos( PVertex ) const;
        // Usuwamy wierzchołki z grafu.
        inline int delVerts();
        template< class Iterator > int delVerts( Iterator begin, Iterator end );
        template< class Iterator > int delVerts2( Iterator begin, Iterator end );
        inline int delVerts( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex> & );

    // Operacje na krawędziach grafu
        // Dodajemy nową krawędź do grafu (bez etykiety).
        inline PEdge addEdge( PVertex, PVertex, EdgeDirection = EdUndir );
        // Dodajemy nową krawędź do grafu (z etykietą).
        inline PEdge addEdge( PVertex, PVertex, const EdgeInfo &, EdgeDirection = EdUndir );
        // Dodajemy nowy łuk do grafu (z etykietą).
        inline PEdge addArch( PVertex, PVertex, const EdgeInfo & = EdgeInfo() );
        // Dodajemy nową pętlę do grafu (bez etykiety).
        inline PEdge addLoop( PVertex, const EdgeInfo & = EdgeInfo() );
        // Etykieta krawędzi.
        inline EdgeInfoType getEdgeInfo( PEdge ) const;
        // Zmieniamy etykietę krawędzi.
        inline void setEdgeInfo( PEdge, const EdgeInfo & = EdgeInfo() ) const;
        // Usuwamy krawędź z grafu.
        inline void del( PEdge );
        inline void delEdge( PEdge );
        // Pierwsza krawędź podanego typu.
        inline PEdge getEdge( EdgeDirection = EdAll ) const;
        // Ostatnia krawędź podanego typu.
        inline PEdge getEdgeLast( EdgeDirection = EdAll )const;
        // Następna i poprzednia krawędź podanego typu.
        inline PEdge getEdgeNext( PEdge,EdgeDirection = EdAll ) const;
        inline PEdge getEdgePrev( PEdge,EdgeDirection = EdAll ) const;
        // Zbiór krawędzi podanego typu.
        Set< PEdge > getEdgeSet( EdgeDirection = EdAll ) const;
        // Kopiujemy krawędzie podanego typu do iteratora.
        template< class OutputIterator >
        int getEdges( OutputIterator, EdgeDirection = EdAll ) const;
        // Typ krawędzi.
        inline EdgeType getEdgeType( PEdge ) const;
        inline EdgeType getType( PEdge ) const;
        // Krawędź o podanym numerze.
        PEdge edgeByNo( int ) const;
        // Numer krawędzi.
        int edgePos( PEdge ) const;
        // Zmieniamy typ krawędzi.
        bool chEdgeType( PEdge, EdgeType );
        // Zmieniamy kierunek krawędzi.
        inline bool revertEdge( PEdge );
        // Przenosimy krawędź w inne miejsce.
        inline PEdge moveEdge( PEdge, PVertex, PVertex, EdgeDirection = EdUndir );
        // Usuwamy krawędzie z grafu.
        int delEdges( EdgeType = EdAll );
        int delEdges( PVertex, EdgeDirection = EdAll );
        int delEdges( PVertex, PVertex, EdgeDirection = EdAll );
        template< class Iterator > int delEdges(
            Iterator begin, Iterator end, EdgeType = EdAll );
        template< class Iterator > int delEdges2(
            Iterator begin, Iterator end, EdgeType = EdAll );
        inline int delEdges(
            const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &,
            EdgeType = EdAll );

        // zmiana typu podanych krawedzi na wartosc ostatniego argumentu
        template< class Iterator > int chEdgesType(Iterator begin, Iterator end, EdgeType);
        inline int chEdgesType(const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge > &, EdgeType);
        int chEdgesType(EdgeType);
        int chEdgesType(PVertex,EdgeType);
        int chEdgesType(PVertex,PVertex,EdgeType);

        // Zmieniamy krawędź nieskierowana w dwa łuki.
        inline PEdge ch2Archs( PEdge );
        // Zmieniamy krawędzie w łuki.
        int ch2Archs();
        template< class Iterator > int ch2Archs( Iterator begin, Iterator end );
        inline int ch2Archs( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> & );
        int ch2Archs(PVertex);
        // Informacja o tym, czy krawędzie są równoległe.
        //relacja rownowaznosci, 3 mozliwosci
        //reltype - dopuszczalne tylko jednobitowe: EdDirIn, EdDirOut lub EdUndir
        //EdDirOut - luk nieskierowany jest rownolegly tylko do tak samo jak on skierowanego luku o tych samych koncach
        //EdDirIn - jest on takze rownolegly do odwrotnie skierowanego luku o tych samych koncach
        //EdUndir - jest on takze rownolegly do krawedzi nieskierowanej o tych samych koncach
        bool areParallel( PEdge, PEdge, EdgeDirection = EdUndir ) const;

        // usuwanie krawedzi rownoleglych do podanej
        int delParals(PEdge,EdgeDirection = EdUndir);
        template< class Iterator > int delParals(Iterator begin,Iterator end,PEdge,EdgeDirection = EdUndir);
        template< class Iterator > int delParals2(Iterator begin,Iterator end,PEdge,EdgeDirection = EdUndir);
        inline int delParals(const Set<PEdge>&,PEdge,EdgeDirection = EdUndir);
        inline int delParals(PVertex,EdgeDirection = EdUndir);
        int delParals(PVertex,PVertex,EdgeDirection = EdUndir);
        inline int delParals(EdgeDirection = EdUndir);
        template< class OutputIterator > int getParals( OutputIterator, PEdge, EdgeDirection = EdUndir ) const;
        Set<PEdge> getParalsSet(PEdge, EdgeDirection = EdUndir) const;
        inline int mu( PEdge, EdgeDirection = EdUndir ) const;
        inline int mu( EdgeDirection = EdUndir ) const;
        std::pair< PEdge,int > maxMu( EdgeDirection = EdUndir ) const;

        // krawedzie indukowane przez dane wierzcholki
        template <class Iterator,class OutIter> int getIndEdges(OutIter,Iterator, Iterator, EdgeType = EdAll ) const;
        template <class Iterator> Set< PEdge > getIndEdgeSet(Iterator beg, Iterator end, EdgeType = EdAll ) const;
        template <class OutIter> int getIndEdges(OutIter,const Set<PVertex>&, EdgeType = EdAll ) const;
        Set< PEdge > getIndEdgeSet(const Set<PVertex>&, EdgeType = EdAll ) const;

        template <class Iterator> int delIndEdges(Iterator, Iterator, EdgeType = EdAll );
        int delIndEdges(const Set<PVertex>&, EdgeType = EdAll );


        // dopelnienia podgrafu indukowanego na zadanych wierzcholkach
        template <class Iterator,class EdInfoGen> void neg(Iterator, Iterator, EdgeType, EdInfoGen);
        template <class Iterator> void neg(Iterator, Iterator, EdgeType=Settings::EdAllow,EdgeInfo=EdgeInfo());
        template <class EdInfoGen> void neg(const Set<PVertex>&, EdgeType,EdInfoGen infoGen);
        void neg(const Set<PVertex>& vset, EdgeType=Settings::EdAllow,EdgeInfo=EdgeInfo());
        // ... i calego grafu
        template <class EdInfoGen> void neg(EdgeType type, EdInfoGen infoGen);
        void neg(EdgeType type=Settings::EdAllow, EdgeInfo=EdgeInfo());

    // Relacje pomiędzy wierzchołkami a krawędziami
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
        // Liczba krawędzi łączących parę wierzchołków.
        int getEdgeNo( PVertex, PVertex, EdgeDirection = EdAll ) const;
        // Końce krawędzi.
        // Wpisuje do kontenera relacje sasiedztwa miedzy wierzcholkami
        template <class Cont>
            void getAdj(Cont &) const;

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

    // Operacje na macierzy sąsiedztwa
        // Tworzymy macierz sąsiedztwa.
        bool makeAdjMatrix();
        // Usuwamy macierz sąsiedztwa.
        inline bool delAdjMatrix();
        // Informacja o tym, czy graf ma macierz sąsiedztwa.
        inline bool hasAdjMatrix() const;
        // ??
        inline bool defragAdjMatrix();

        static bool allowedAdjMatrix() { return Settings::AdjMatrixAllowed; }

    // ??
        const Graph< VertInfo,EdgeInfo,Settings > *getRootPtr() const { return this; }
        bool good( PVertex,bool=false ) const { return true; }
        bool good( PEdge,bool=false ) const { return true; }
//        bool testGraph();
        PVertex putVert( PEdge,const VertInfo & = VertInfo() );
        PEdge pickVert( PVertex, const EdgeInfo & = EdgeInfo() );
        inline PVertex glue( PVertex, PVertex, bool makeloops = false );
        template< class Iterator > PVertex glue(
            Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        template< class Iterator > PVertex glue2(
            Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        inline PVertex glue(
            const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex> &,
            bool makeloops = false, PVertex = NULL );

    private:

        AdjMatrix <VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed>* pAdj;

        struct Parals2 {
            typename Koala::Edge< VertInfo,EdgeInfo,Settings > *tab[3];
            char size;

            Parals2() : size(0) { tab[0]=tab[1]=tab[2]=0; }
        };
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
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct < b.direct );
            }

        };

        PVertex first_vert, last_vert;
        PEdge first_edge, last_edge;
        int& no_loop_edge() const { return this->Privates::EdgeCounterLoop<Settings::EdAllow & Loop>::no(); }
        int& no_dir_edge() const { return this->Privates::EdgeCounterDir<Settings::EdAllow & (EdDirIn|EdDirOut)>::no(); }
        int& no_undir_edge() const { return this->Privates::EdgeCounterUndir<Settings::EdAllow & Undirected>::no(); }
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
