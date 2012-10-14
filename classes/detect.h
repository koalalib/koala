#ifndef KOALA_DETECTOR_H
#define KOALA_DETECTOR_H

#include "../container/simple.h"
#include "../graph/graph.h"
#include "../graph/view.h"
#include "../algorithm/search.h"
#include "../algorithm/weights.h"
#include "../algorithm/matching.h"
#include "../algorithm/conflow.h"
#include "../container/joinsets.h"

namespace Koala
{
    //Algorytmy rozpoznawania rodzin grafow. Dla rodziny family jest metoda rozpoznajaca bool family(const& graph).
    //Jesli family ma sens takze dla multigrafow, bool family(const& graph, bool allowmulti) gdzie flaga bool podaje
    //czy zezwalay na krawedzie rownolegle i petle. Jesli dla klasy family sa jakies szczegolne procedury (tylko dla grafow tego typu),
    //wprowadza sie dodatkowo podklase Family z metodami realizujacymi te funkcje. Generalnie nie maja one obowiazku
    // sprawdzania poprawnosci danych (tj. przynaleznosci grafu do tej rodziny), choc czasem tak czynia.

    /* IsItPar
     * DefaultStructs - wytyczne dla wewnetrznych procedur
     */
    template< class DefaultStructs > class IsItPar: public SearchStructs
    {
      public:
        // Generalnie zaklada sie, ze poprawny (dla IsItPar) graf wejsciowy nie ma petli ani lukow oraz n>0
        // Ta metoda jest wyjatkiem, sprawdza czy n=0
        template< class GraphType > static bool zero( const GraphType &g )
            { return !g.getVertNo(); }

        // czy graf jest prosty (tj. nie ma krawedzi rownoleglych)
        template< class GraphType > static bool undir( const GraphType &g, bool allowmulti = false );

        // czy spojny
        template< class GraphType > static bool connected( const GraphType &g, bool allowmulti = false );

        // czy bezkrawedziowy
        template< class GraphType > static bool empty( const GraphType &g )
            { return g.getVertNo() && !g.getEdgeNo(); }

        // drzewo
        template< class GraphType > static bool tree( const GraphType &g )
            { return connected( g,true ) && (g.getVertNo() - 1 == g.getEdgeNo()); }

        // las
        template< class GraphType > static bool forest( const GraphType &g )
            { return undir( g,true ) && !BFSPar< DefaultStructs >::cyclNo( g ); }
        // klika
        template< class GraphType > static bool clique( const GraphType &g );

        // suma rozlacznych klik
        template< class GraphType > static bool cliques( const GraphType &g );

        // regularny
        template< class GraphType > static bool regular( const GraphType &g, bool allowmulti = false );

        /* Path
         *
         */
        class Path
        {
          public:
            // Konce podanej sciezki, (NULL,NULL) w razie bledu
            template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
                ends( const GraphType &g );
        };

        // sciezka
        template< class GraphType > static bool path( const GraphType &g )
            { return Path::ends( g ).first; }

        /* Caterpillar
         *
         */
        class Caterpillar
        {
          public:
            // Konce grzbietu gasienicy, (NULL,NULL) w razie bledu
            template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
                spineEnds( const GraphType &g );
        };

        // gasienica
        template< class GraphType > static bool caterpillar( const GraphType &g )
            { return Caterpillar::spineEnds( g ).first; }

        // cykl,
        template< class GraphType > static bool cycle( const GraphType &g, bool allowmulti = false )
            { return connected( g,allowmulti ) && g.deg( g.getVert() ) == 2 && regular( g,true ); }

        // zbior niezalezny krawedzi
        template< class GraphType > static bool matching( const GraphType &g )
            { return undir( g,true ) && g.Delta() <= 1; }

        // podkubiczny
        template< class GraphType > static bool subcubic( const GraphType &g, bool allowmulti = false )
            { return undir( g,allowmulti ) && g.Delta() <= 3; }

        // kubiczny
        template< class GraphType > static bool cubic( const GraphType &g, bool allowmulti = false )
            { return g.getVert() && g.deg( g.getVert() ) == 3 && regular( g,allowmulti ); }

        // graf o wszystkich skladowych 2-spojnych bedacych klikami
        template< class GraphType > static bool block( const GraphType &g );

        // maks. liczba cyklomatyczna skladowej 2-spojnej grafu prostego. -1 w razie bledu.
        template< class GraphType > static int almostTree( const GraphType &g );

        /* Bipartite
         *
         */
        class Bipartite
        {
          public:
            // wypisuje na iterator wierzcholki jednej partycji grafu dwudzielnego. Zwraca licznosc partycji (-1 w razie bledu)
            template< class GraphType, class Iter >
                static int getPart( const GraphType &g, Iter out, bool allowmulti = false );

            // znajduje najwiekszy zbior niezalezny, zwraca jego rozmiar
            template< class GraphType, class Iter > static int maxStable( const GraphType &g, Iter out );

            // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
            template< class GraphType, class Iter > static int minVertCover( const GraphType &g, Iter out );

        };

        // czy dwudzielny
        template< class GraphType > static bool bipartite( const GraphType &g, bool allowmulti = false )
            { return Bipartite::getPart( g,blackHole,allowmulti ) != -1; }

        /* CompBipartite
         *
         */
        class CompBipartite
        {
          public:
            // wypisuje na iterator wierzcholki jednej partycji grafu pelnego dwudzielnego. Zwraca licznosc partycji (-1 w razie bledu)
            template< class GraphType, class Iter > static int getPart( const GraphType &g, Iter out );
        };

        // czy pelny dwudzielny
        template< class GraphType > static bool compBipartite( const GraphType &g )
            { return CompBipartite::getPart( g,blackHole ) != -1; }

        /* CompMPartite
         *
         */
        class CompMPartite
        {
          public:
            // wyrzuca na out ciagi wierzcholkow tworzacych partycje grafu pelnego M-dzielnego. Zwraca liczbe partycji M lub -1 w razie bledu
            template< class GraphType, class Iter, class VIter >
                static int split( const GraphType &g, CompStore< Iter,VIter > out );
        };

        // czy pelny M-dzielny dla pewnego M>0
        template< class GraphType > static bool compMPartite( const GraphType &g )
            { return CompMPartite::split( g,compStore( blackHole,blackHole )) != -1; }

        /* Chordal
         * M. Habib, R. McConnel, C. Paul, L.Viennot
         * Lex-BFS and Partition Refinement, with Applications to Transitive
         * Orientation, Interval Graph Recognition and Consecutive Ones Testing
         * Obsluga chordali
         */
        class Chordal
        {
          protected:
            static void RadixSort( std::pair< int,int > *src, int size, int n, int *hints, int *out );

            // porz¹dkuje wêz³y drzewa zdefiniowanego przez relacjê parent
            // wszyscy potomkowie p wyst¹pi¹ przed wyst¹pieniem p, ale
            // nie koniecznie bêdzie to kolejnoœæ wygenerowana przez DFSa na
            // drzewie, np. dla:
            //     D
            //    / \
            //   C   E
            //  / \
            // A   B
            // DFSPostorder da: A B C E D
            // a poni¿sza funkcja mo¿e zwróciæ np.: A E B C D

            static void SemiPostOrderTree( int *parent, int n, int *out );

            template< class Graph > struct QTRes
            {
                int size;
                Set< typename Graph::PVertex > trees;
                QTRes(): size( 0 )
                    { }
            };

          public:
            // wyrzuca na iterator odwrotny perf. ellimination order chordal grafu tj. porzadek doklejania nowych wierzcholkow za podkliki
            // false gdy graf nie byl chordal
            template< class Graph, class VIter2 > static bool getOrder( const Graph &g, VIter2 riter );

            // Tworzy reprezentacje chordala w postaci drzewa jego maksymalnych klik
            // zwraca liczbe maksymalnych klik
            // TODO: masakra! uproscic
            template< class Graph, class VIter, class VIterOut, class QIter, class QTEIter >
                static int maxCliques( const Graph &g, VIter begin, VIter end, CompStore< QIter,VIterOut > out, QTEIter qte );
            // j.w. ale samodzielna. Pobiera tylko graf, jesli nie byl chordal, zwraca -1
            template< class Graph, class VIterOut, class QIter, class QTEIter >
                static int maxCliques( const Graph &g, CompStore< QIter,VIterOut > out, QTEIter qte );

            // znajduje najwieksza klike w grafie, zwraca jej rozmiar
            template< class Graph, class VIter, class VIterOut >
                static int maxClique( const Graph &g, VIter begin, VIter end, VIterOut out );
            // j.w. ale samodzielna. Pobiera tylko graf, jesli nie byl chordal, zwraca -1
            template< class Graph, class VIterOut > static int maxClique( const Graph &g, VIterOut out );

            // znajduje najwiekszy zbior niezalezny (wypuszczany na out), zwraca jego rozmiar
            // korzysta z rozkladu na drzewo klik por. maxCliques
            template< class Graph, class QIter, class VIter, class QTEIter, class IterOut >
                static int maxStable( const Graph &g, int qn, QIter begin, VIter vbegin, QTEIter ebegin, IterOut out );

            // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
            // sens parametrow j.w.
            template< class Graph, class QIter, class VIter, class QTEIter, class IterOut >
                static int minVertCover( const Graph &g, int qn, QIter begin, VIter vbegin, QTEIter ebegin, IterOut out );

            // znajduje najwiekszy zbior niezalezny (wypuszczany na out), zwraca jego rozmiar lub -1 w razie bledu
            // samodzielna
            template< class Graph, class IterOut > static int maxStable( const Graph &g, IterOut out );

            // znajduje najwieksze pokrycie wierzcholkowe (wypuszczane na out), zwraca jego rozmiar lub -1 w razie bledu
            // samodzielna
            template< class Graph, class IterOut > static int minVertCover( const Graph &g, IterOut out );
        };

        /** test if graph is chordal
         * @param[in] g graph to test
         * @return true if graph is chordal, false otherwise */
        template< class GraphType > static bool chordal( const GraphType &g )
        { return Chordal::getOrder( g,blackHole ); }

        // czy graf jest dopelnieniem chordala
        template< class GraphType > static bool cochordal( const GraphType &g );

        // czy splitgraph
        template< class GraphType > static bool split( const GraphType &g )
        { return chordal( g ) && cochordal( g ); }

        /* Comparability
         *
         */
        class Comparability
        {
          protected:

            struct EDir: public std::pair< int,int >
            {
                EDir(): std::pair< int,int >( 0,0 ) { }

                int& operator()( EdgeDirection arg )
                {
                    switch (arg)
                    {
                        case EdDirIn: return first;
                        case EdDirOut: return second;
                        default: assert( 0 );
                    }
                }
            };

            class FlowDefaultStructs: public DefaultStructs
            {
              public:
                enum { useFulkersonFord = false };
            };


          public:
            /* M.C. Golumbic
             * The Complexity of Comparability Graph Recognition and Coloring
             * Computing 18, 199-208 (1977)
             */
            // Kompleksowa obsluga comparability grafu
            // zwraca liczbe chromatyczna lub -1 jesli graf nie byl comparability
            // dirmap - wysciowa tablica asocjacyjna PVertex->EdgeDirection z przykladowa comparability orientation krawedzi
            // (kierunek krawedzi miedzy getEdgeEnd1 a getEdgeEnd2). Lub BlackHole.
            // aheightmap- wysciowa tablica asocjacyjna PVertex->int z optymalnym pokolorowaniem wierzcholkowym. Lub BlackHole.
            // cliqueiter - iterator wyjsciowy, na ktory zostaje zapisana najwieksza klika
            template< class Graph, class DirMap, class OutMap, class OutIter >
                static int explore( const Graph &g, DirMap &dirmap, OutMap &aheightmap, OutIter cliqueiter );

            // sprawdza, czy graf byl comparability
            // adirmap - wysciowa tablica asocjacyjna PVertex->EdgeDirection z przykladowa comparability orientation krawedzi
            // (kierunek krawedzi miedzy getEdgeEnd1 a getEdgeEnd2). Lub BlackHole
            template< class Graph, class DirMap > static bool getDirs( const Graph &g, DirMap& adirmap )
                { return explore( g,adirmap,blackHole,blackHole ) != -1; }

            // sprawdza, czy graf byl comparability. Jesli tak, nadaje krawedziom wlasciwa orientacje
            template< class Graph > static bool getDirs( Graph &g );

            // zwraca liczbe chromatyczna lub -1 jesli graf nie byl comparability
            // avmap - wysciowa tablica asocjacyjna PVertex->int z optymalnym pokolorowaniem wierzcholkowym. Lub BlackHole.
            template< class Graph, class OutMap > static int color( const Graph &g, OutMap &avmap )
                { return explore( g,blackHole,avmap,blackHole ); }

            /** find a largest clique in a comparability graph
             * @param[in] g graph
             * @param[out] iter iterator to write clique's vertices. Lub BlackHole.
             * @return number of vertices in the clique */
            template< class Graph, class OutIter > static int maxClique( const Graph &g, OutIter iter )
                { return explore( g,blackHole,blackHole,iter ); }

            // znajduje najwiekszy zbior niezalezny, zwraca jego rozmiar
            template< class GraphType, class VIterOut > static int maxStable( const GraphType &g, VIterOut out );
            // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
            template< class GraphType, class Iter > static int minVertCover( const GraphType &g, Iter out );
        };

        /** test if graph is a comparability graph
         * @param[in] g graph to test
         * @return true if graph is a comparability graph, false otherwise */
        template< class GraphType > static bool comparability( const GraphType &g )
            { return Comparability::explore( g,blackHole,blackHole,blackHole ) != -1; }

        // czy graf jest dopelnieniem comparability
        template< class GraphType > static bool cocomparability( const GraphType &g );

        /* Interval
         *
         */
        class Interval: protected LexBFSPar< DefaultStructs >
        {
          public:
            // Struktura reprezentujaca przedzial domkniety na prostej o koncach calkowitych (dopuszczalna dlugosc 0)
            struct Segment
            {
                int left, right;
                Segment( int l = 0, int r = 1 ): left( l ), right( r )
                { }
            };

            // czy dwa takie przedzialy tna sie niepusto
            static bool touch( Segment a, Segment b )
                { return std::max( a.left,b.left ) <= std::min( a.right,b.right ); }

            // konwersja zbior przedzialow-> interval graph
            // pobiera spomiedzy 2 iteratorow ciag przedzialow (struktur typu segment)
            // dopisuje do danego grafu odpowiadajacy mu graf przeciec (zwraca pierwszy stworzony wierzcholek)
            // pola info wierzcholkow i krawedzi tworzy funktorami vinfo(int), einfo(int,int) - argumenty to
            // numery przedzialow w ciagu wejsciowym
            // na iterator out zwraca sekwencje stworzonych wierzcholkow zgodna z cagiem przedzialow
            template< class GraphType, class Iter, class IterOut, class VInfoGen, class EInfoGen >
                static typename GraphType::PVertex segs2graph( GraphType &g, Iter begin, Iter end, IterOut out,
                    VInfoGen vinfo, EInfoGen einfo );
            // j.w. ale polom info nadawane sa wartosci domyslne
            template< class GraphType, class Iter, class IterOut >
                static typename GraphType::PVertex segs2graph( GraphType &g, Iter begin, Iter end, IterOut out );

            /** convert interval graph to its interval representation
             * @param[in] g graph
             * @param[out] outmap map (PVertex -> Segment), Lub BlackHole.
             * @return true if g is interval, false otherwise */
            template< class GraphType, class IntMap > static bool graph2segs( const GraphType &g, IntMap &outmap );

          protected:
            struct Sets
            {
                struct Elem
                {
                    int value;
                    Privates::List< Elem,Privates::BlockListAllocator< Privates::ListNode< Elem > > > *cont;
                    Privates::List_iterator< Elem > next;
                    Elem( int _v = -1 ): value( _v ), next(), cont( NULL )
                    { }
                };
                typedef Privates::List< Elem,Privates::BlockListAllocator< Privates::ListNode< Elem > > > Entry;

                Sets( std::pair< Entry,Privates::List_iterator< Elem> > *data, size_t n,
                    Privates::BlockListAllocator< Privates::ListNode< Elem > > &a );

                // dodaje trg do zbioru id
                void add( int id, int trg );
                // usuwa id ze wszystkich zbiorów
                void remove( int id );
                // iloœæ elementów w zbiorze id
                int count( int id )
                    { return m_data[id].first.size(); }
                // czy zbiór id pusty
                bool empty( int id )
                    { return m_data[id].first.empty(); };
                // pierwszy element ze zbioru id (w praktyce -- dowolny)
                int first( int id )
                    { return m_data[id].first.front().value; }

                std::pair< Entry,Privates::List_iterator< Elem > > *m_data;
            };

            struct IvData
            {
                unsigned int posSigmap;
                unsigned int ip;
                unsigned int posSigmapp;
                unsigned int ipp;
                unsigned int posSigma;
            };

            template< class GraphType, class Map > static void CalculateI( const GraphType &g,
                typename GraphType::PVertex *order, Map &data, unsigned int IvData::*pos, unsigned int IvData::*ifn );

            // BuildSet
            template< class GraphType, class Map, class SetsType > static void BuildSet( const GraphType &g,
                SetsType &sets, Map &data, unsigned int IvData::*order, unsigned int IvData::*ifn );

            template< class GraphType > struct OrderData
            {
                typename GraphType::PVertex v;
                // kogo jest s¹siadem (numer s¹siada w porz¹dku)
                int vertId;
                // numer w porz¹dku
                int orderId;
            };

            struct LBSData
            {
                bool visiteda;
                bool visitedb;
                int aOrder;
                int bOrder;
            };

            // LBFSStar
            template< class GraphType, class MapType, class SetsType, class OutIter, class Alloc1, class Alloc2 >
                static void LBFSStar( const GraphType &g, SetsType &A, SetsType &B, MapType &data,
                    typename GraphType::PVertex *sigmap, typename GraphType::PVertex *sigmapp, OutIter out,
                    Alloc1& allocat,Alloc2& allocat2 );

            // UmbrellaTestVertex
            template< class GraphType, class Map >
                static bool UmbrellaTestVertex( const GraphType &g, typename GraphType::PVertex u, Map &data,
                    typename GraphType::PVertex *order );

            // IsUmbrellaFree
            template< class GraphType, class Map >
                static bool IsUmbrellaFree( const GraphType &g, Map &data, typename GraphType::PVertex *order );
            // reverse
            template< class T > static void reverse( T *tab, size_t n );
        };

        /** test if graph is an interval graph
         * @param[in] g graph
         * @return true if g is interval, false otherwise */
        template< class GraphType > static bool interval( const GraphType &g )
            { return chordal( g ) && cocomparability( g ); }

        // czy pierwszy
        template< class GraphType > static bool prime( const GraphType &g );

        /* Cograph
        *
        */
        class Cograph
        {
            template< class Defs > friend class IsItPar;

          public:
            template< class GraphType, class VIterOut > static int maxClique( const GraphType &g, VIterOut out );
            template< class GraphType, class VIterOut > static int maxStable( const GraphType &g, VIterOut out );
            // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
            template< class GraphType, class Iter > static int minVertCover( const GraphType &g, Iter out );

    //        TODO: template<class Graph, class AssocTab>
    //        static int color(const Graph &g,AssocTab out);

          protected:
            template< class GraphType, class Assoc > static bool cograph( const GraphType &ag, Assoc &subset );
            template< class GraphType, class Assoc, class Iter >
                static int maxClique2( const GraphType &ag, Assoc &subset, Iter& out );
            template< class GraphType, class Assoc, class Iter >
                static int maxStable2( const GraphType &ag, Assoc &subset, Iter& out );
        };

        // czy cograph
        template< class GraphType > static bool cograph( const GraphType &g );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class IsIt: public IsItPar< AlgsDefaultSettings > { };
#include "detect.hpp"
}

#endif
