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
#include <utility>

namespace Koala
{

template< class VertInfo, class EdgeInfo > class Graph;
template< class VertInfo, class EdgeInfo > class Vertex;
template< class VertInfo, class EdgeInfo > class Edge;

}

#include "../base/def_struct.h"
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

/* ------------------------------------------------------------------------- *
 * Graph< VertInfo,EdgeInfo >
 *
 * Graf wraz z wszystkimi podstawowymi operacjami wykonywanymi na grafach.
 * Parametry grafu to klasy, których instancje będą etykietami wierzchołków
 * i krawędzi.
 *
 * ------------------------------------------------------------------------- */

template< class VertInfo = EmptyVertInfo, class EdgeInfo = EmptyVertInfo >
class Graph: public SubgraphBase
{
    public:
        friend class Edge< VertInfo,EdgeInfo >;

        typedef Koala::Vertex< VertInfo,EdgeInfo > Vertex;
        typedef Vertex* PVertex;
        typedef Koala::Edge< VertInfo,EdgeInfo > Edge;
        typedef Edge* PEdge;
        typedef Graph< VertInfo,EdgeInfo > GraphType;
        typedef Graph< VertInfo,EdgeInfo > RootGrType;
        typedef VertInfo VertInfoType;
        typedef EdgeInfo EdgeInfoType;

    // Konstruktory
        // Tworzy pusty graf, bez krawędzi i wierzchołków.
        Graph();
        // Konstruktor kopiujący.
        Graph( const Graph & );

    // Destruktor
        ~Graph();

    // Parametry grafu
        // Liczba wierzchołków (rząd grafu).
        int getVertNo() const;
        // Liczba krawędzi podanego typu.
        int getEdgeNo( EdgeDirection = EdAll ) const;
        // Wierzchołek o minimalnym stopniu (i jego stopień).
        std::pair< PVertex,int > minDeg( EdgeDirection = EdAll ) const;
        // Wierzchołek o maksymalnym stopniu (i jego stopień).
        std::pair< PVertex,int > maxDeg( EdgeDirection = EdAll ) const;
        // Stopień i minimalny stopień w grafie.
        int Delta( EdgeDirection = EdAll ) const;
        int delta( EdgeDirection = EdAll ) const;

    // Operacje na całym grafie
        // Usuwamy wszystkie wierzchołki i krawędzie grafu.
        void clear();
        // Usuwamy wszystkie wierzchołki grafu.
        void clearEdges();
        // Operator przypisania.
        Graph< VertInfo,EdgeInfo > &operator=( const Graph< VertInfo,EdgeInfo > & );
        // Suma grafów.
        Graph< VertInfo,EdgeInfo > &operator+=( const Graph< VertInfo,EdgeInfo > & );
        // Przenosimy wszystkie wierzchołki i krawędzie do naszego grafu.
        PVertex move( Graph< VertInfo,EdgeInfo > & );
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
        PVertex addVert( const VertInfo & = VertInfo() );
        // Usuwamy wierzchołek z grafu.
        void del( PVertex, bool = true );
        void delVert( PVertex, bool = true );
        // Etykieta wierzchołka.
        VertInfoType getVertInfo( PVertex ) const;
        // Zmieniamy etykietę wierzchołka.
        void setVertInfo( PVertex, const VertInfo & = VertInfo() ) const;
        // Pierwszy wierzchołek grafu.
        PVertex getVert() const;
        // Ostatni wierzchołek grafu.
        PVertex getVertLast() const;
        // Następny i poprzedni wierzchołek w grafie.
        PVertex getVertNext( PVertex ) const;
        PVertex getVertPrev( PVertex ) const;
        // Zbiór wierzchołków grafu.
        Set< PVertex > getVertSet() const;
        // Kopiujemy wierzchołki grafu do iteratora.
        template< class OutputIterator > int getVerts( OutputIterator ) const;
        // Liczba krawędzi incydentnych z wierzchołkiem.
        int getEdgeNo( PVertex, EdgeDirection = EdAll ) const;
        // Sąsiedztwo wierzchołka.
        Set< PVertex > getNeighSet( PVertex, EdgeDirection = EdAll ) const;
        // Kopiujemy sąsiedztwo wierzchołka do iteratora.
        template< class OutputIterator > int getNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        // Liczba wierzchołków w sąsiedztwie.
        int getNeighNo( PVertex, EdgeDirection = EdAll ) const;
        // Domknięte sąsiedztwo wierzchołka.
        Set< PVertex > getClNeighSet( PVertex, EdgeDirection = EdAll ) const;
        // Kopiujemy domknięte sąsiedztwo do iteratora.
        template< class OutputIterator > int getClNeigh(
            OutputIterator, PVertex, EdgeDirection = EdAll ) const;
        // Liczba wierzchołków w domkniętym sąsiedztwie.
        int getClNeighNo( PVertex, EdgeDirection = EdAll ) const;
        // Stopień wierzchołka.
        int deg( PVertex, EdgeDirection = EdAll ) const;
        // Wierzchołek o podanym numerze.
        PVertex vertByNo( int ) const;
        // Numer wierzchołka.
        int vertPos( PVertex ) const;
        // Usuwamy wierzchołki z grafu.
        int delVerts();
        template< class Iterator > int delVerts( Iterator begin, Iterator end );
        template< class Iterator > int delVerts2( Iterator begin, Iterator end );
        int delVerts( const Set< typename Graph< VertInfo,EdgeInfo >::PVertex> & );

    // Operacje na krawędziach grafu
        // Dodajemy nową krawędź do grafu (bez etykiety).
        PEdge addEdge( PVertex, PVertex, EdgeDirection = EdUndir );
        // Dodajemy nową krawędź do grafu (z etykietą).
        PEdge addEdge( PVertex, PVertex, const EdgeInfo &, EdgeDirection = EdUndir );
        // Dodajemy nowy łuk do grafu (z etykietą).
        PEdge addArch( PVertex, PVertex, const EdgeInfo & = EdgeInfo() );
        // Dodajemy nową pętlę do grafu (bez etykiety).
        PEdge addLoop( PVertex, const EdgeInfo & = EdgeInfo() );
        // Etykieta krawędzi.
        EdgeInfoType getEdgeInfo( PEdge ) const;
        // Zmieniamy etykietę krawędzi.
        void setEdgeInfo( PEdge, const EdgeInfo & = EdgeInfo() ) const;
        // Usuwamy krawędź z grafu.
        void del( PEdge );
        void delEdge( PEdge );
        // Pierwsza krawędź podanego typu.
        PEdge getEdge( EdgeDirection = EdAll ) const;
        // Ostatnia krawędź podanego typu.
        PEdge getEdgeLast( EdgeDirection = EdAll )const;
        // Następna i poprzednia krawędź podanego typu.
        PEdge getEdgeNext( PEdge,EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PEdge,EdgeDirection = EdAll ) const;
        // Zbiór krawędzi podanego typu.
        Set< PEdge > getEdgeSet( EdgeDirection = EdAll ) const;
        // Kopiujemy krawędzie podanego typu do iteratora.
        template< class OutputIterator >
        int getEdges( OutputIterator, EdgeDirection = EdAll ) const;
        // Typ krawędzi.
        EdgeType getEdgeType( PEdge ) const;
        EdgeType getType( PEdge ) const;
        // Krawędź o podanym numerze.
        PEdge edgeByNo( int ) const;
        // Numer krawędzi.
        int edgePos( PEdge ) const;
        // Zmieniamy typ krawędzi.
        bool chEdgeType( PEdge, EdgeType );
        // Zmieniamy kierunek krawędzi.
        bool revertEdge( PEdge );
        // Przenosimy krawędź w inne miejsce.
        PEdge moveEdge( PEdge, PVertex, PVertex, EdgeDirection = EdUndir );
        // Usuwamy krawędzie z grafu.
        int delEdges( EdgeDirection = EdAll );
        int delEdges( PVertex, EdgeDirection = EdAll );
        int delEdges( PVertex, PVertex, EdgeDirection = EdAll );
        template< class Iterator > int delEdges(
            Iterator begin, Iterator end, EdgeDirection = EdAll );
        template< class Iterator > int delEdges2(
            Iterator begin, Iterator end, EdgeDirection = EdAll );
        int delEdges(
            const Set< typename Graph< VertInfo,EdgeInfo >::PEdge> &,
            EdgeDirection = EdAll );
        template< class Iterator > int delEdges(
            PVertex, Iterator begin, Iterator end, EdgeDirection = EdAll );
        template< class Iterator > int delEdges2(
            PVertex, Iterator begin, Iterator end, EdgeDirection = EdAll );
        int delEdges(
            PVertex, const Set< typename Graph< VertInfo,EdgeInfo >::PEdge > &,
            EdgeDirection = EdAll );
        template< class Iterator > int delEdges(
            PVertex, PVertex, Iterator begin, Iterator end, EdgeDirection = EdAll );
        template< class Iterator > int delEdges2(
            PVertex, PVertex, Iterator begin, Iterator end, EdgeDirection = EdAll );
        int delEdges(
            PVertex, PVertex,
            const Set< typename Graph< VertInfo,EdgeInfo >::PEdge > &,
            EdgeDirection = EdAll );

        // zmiana typu podanych krawedzi na wartosc ostatniego argumentu
        template< class Iterator > int chEdgesType(Iterator begin, Iterator end, EdgeType);
        template< class Iterator > int chEdgesType2(Iterator begin, Iterator end, EdgeType);
        int chEdgesType(const Set< typename Graph< VertInfo,EdgeInfo >::PEdge > &, EdgeType);
        int chEdgesType(EdgeType);
        int chEdgesType(PVertex,EdgeType);
        int chEdgesType(PVertex,PVertex,EdgeType);

        // Zmieniamy krawędź nieskierowana w dwa łuki.
        PEdge ch2Archs( PEdge );
        // Zmieniamy krawędzie w łuki.
        int ch2Archs();
        template< class Iterator > int ch2Archs( Iterator begin, Iterator end );
        int ch2Archs( const Set< typename Graph< VertInfo,EdgeInfo >::PEdge> & );
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
        int delParals(const Set<PEdge>&,PEdge,EdgeDirection = EdUndir);
        int delParals(PVertex,EdgeDirection = EdUndir);
        int delParals(PVertex,PVertex,EdgeDirection = EdUndir);
        int delParals(EdgeDirection = EdUndir);
        template< class OutputIterator > int getParals( OutputIterator, PEdge, EdgeDirection = EdUndir ) const;
        int mu( PEdge, EdgeDirection = EdUndir ) const;
        int mu( EdgeDirection = EdUndir ) const;
        std::pair< PEdge,int > maxMu( EdgeDirection = EdUndir ) const;

    // Relacje pomiędzy wierzchołkami a krawędziami
        // Pierwsza krawędź incydentna z wierzchołkiem.
        PEdge getEdge( PVertex, EdgeDirection = EdAll ) const;
        // Ostatnia krawędź incydentna z wierzchołkiem.
        PEdge getEdgeLast( PVertex, EdgeDirection = EdAll ) const;
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
        PEdge getEdge( PVertex, PVertex, EdgeDirection = EdAll ) const;
        // Ostatnia krawędź łącząca parę wierzchołków.
        PEdge getEdgeLast( PVertex, PVertex, EdgeDirection = EdAll )const;
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
        std::pair< PVertex,PVertex > getEdgeEnds( PEdge ) const;
        std::pair< PVertex,PVertex > getEnds( PEdge ) const;
        // Informacja o tym, czy wierzchołek jest incydentny z krawędzią.
        bool isEdgeEnd( PEdge, PVertex ) const;
        bool isEnd( PEdge, PVertex ) const;
        // Drugi z końców krawędzi.
        PVertex getEdgeEnd( PEdge, PVertex ) const;
        PVertex getEnd( PEdge, PVertex ) const;
        // Typ krawędzi.
        EdgeDirection getEdgeDir( PEdge, PVertex ) const;

    // Operacje na macierzy sąsiedztwa
        // Tworzymy macierz sąsiedztwa.
        bool makeAdjMatrix();
        // Usuwamy macierz sąsiedztwa.
        bool delAdjMatrix();
        // Informacja o tym, czy graf ma macierz sąsiedztwa.
        bool hasAdjMatrix() const;
        // ??
        bool defragAdjMatrix();

    // ??
        const Graph< VertInfo,EdgeInfo > *getRootPtr() const { return this; }
        bool good( PVertex,bool=false ) const { return true; }
        bool good( PEdge,bool=false ) const { return true; }
        bool testGraph();
        PVertex putVert( PEdge,const VertInfo & = VertInfo() );
        PEdge pickVert( PVertex, const EdgeInfo & = EdgeInfo() );
        PVertex glue( PVertex, PVertex, bool makeloops = false );
        template< class Iterator > PVertex glue(
            Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        template< class Iterator > PVertex glue2(
            Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        PVertex glue(
            const Set< typename Graph< VertInfo,EdgeInfo >::PVertex> &,
            bool makeloops = false, PVertex = NULL );

    private:
        struct Parals {
            typename Koala::Edge< VertInfo,EdgeInfo > *first, *last;
            int degree;

            Parals(): first( NULL ), last( NULL ), degree( 0 ) {}
        };

        class AdjMatrix {
            public:
                AssocMatrix< typename Koala::Vertex< VertInfo,EdgeInfo > *,Parals,AMatrNoDiag > dirs;
                AssocMatrix< typename Koala::Vertex< VertInfo,EdgeInfo > *,Parals,AMatrTriangle > undirs;
                AdjMatrix( int asize = 0 ): dirs( asize ), undirs( asize ) { }

                void clear() { dirs.clear(); undirs.clear(); }
                void defrag() { dirs.defrag(); undirs.defrag(); }
                void add( PEdge edge );
        } *pAdj;

        struct Parals2 {
            typename Koala::Edge< VertInfo,EdgeInfo > *tab[3];
            char size;

            Parals2() : size(0) { tab[0]=tab[1]=tab[2]=0; }
        };
        template <class Container> int delParals(PVertex,EdgeDirection,Container &);

        struct Parals3 {
            typename Koala::Vertex< VertInfo,EdgeInfo > *v1,*v2;
            EdgeDirection direct;
            typename Koala::Edge< VertInfo,EdgeInfo > *edge;

            Parals3(typename Koala::Vertex< VertInfo,EdgeInfo > *av1,
                    typename Koala::Vertex< VertInfo,EdgeInfo > *av2,
                    EdgeDirection adirect,
                    typename Koala::Edge< VertInfo,EdgeInfo > *aedge) :
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
        int no_vert, no_loop_edge, no_dir_edge, no_undir_edge;

        PVertex attach( PVertex );
        PVertex detach( PVertex );
        PEdge attach( PEdge, PVertex, PVertex, EdgeDirection );
        PEdge attach_undir( PEdge, PVertex, PVertex );
        PEdge attach_dir( PEdge, PVertex, PVertex );
        PEdge attach_loop( PEdge, PVertex );
        PEdge detach( PEdge );
} ;

#include "graph.hpp"
#include "adjmatrix.hpp"
#include "graph_test.hpp"

}

#endif
