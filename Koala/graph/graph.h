#ifndef KOALA_GRAPH_H
#define KOALA_GRAPH_H

/* graph.h
 * Moduł definujący graf oraz wszystkie podstawowe operacje wykonywane na grafach.
 */

#include <algorithm>
#include <cstdio>
#include <cassert>

#include "../base/defs.h"

namespace Koala
{
    // Domyslne wytyczne parametryzujace strukture i dzialanie metod klasy grafu
    // TODO: przetestowac dzialanie grafu przy roznych ustawieniach
    
    /* GrDefaultSettings
     * edAllow - maska okreslajaca dopuszczalne typy krawedzi
     * adjMatrixAllowed - czy jest dopuszczalne tworzenie macierzy sasiedztwa
     */
    template< EdgeType edAllow, bool adjMatrixAllowed > class GrDefaultSettings
    {
      public:
        // maska okreslajaca dopuszczalne typy krawedzi
        enum { EdAllow = edAllow }; 
        // czy jest dopuszczalne tworzenie macierzy sasiedztwa
        enum { AdjMatrixAllowed = adjMatrixAllowed }; 

        // typ klasy tablicy asocjacyjnej uzywanej w metodach grafu przypisujacej wierzcholkom wartosci typu B
        template< class A, class B > class VertAssocCont
        {
          public:
            typedef AssocArray< A,B > Type;
            // typedef AssocTable<std::map<A,B> > Type; // - przyklad uzycia, nie usuwac
        };

        // typ klasy tablicy asocjacyjnej uzywanej w metodach grafu przypisujacej wierzcholkom innego grafu (np.
        // innego typu) wartosci typu B
        template< class A, class B > class ExtVertAssocCont
        {
          public:
            typedef AssocArray< A,B > Type;
            // typedef AssocTable<std::map<A,B> > Type; // - przyklad uzycia, nie usuwac
        };

        // typ macierzy sasiedztwa dla krawedzi skierowanych - zbedne przy edAllow=false
        template< class K, class V > class AdjMatrixDirEdges
        {
          public:
            typedef AssocMatrix< K,V,AMatrNoDiag > Type;
            // - przyklad uzycia, nie usuwac
            // typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
        };

        // typ macierzy sasiedztwa dla krawedzi nieskierowanych - zbedne przy edAllow=false
        template< class K, class V > class AdjMatrixUndirEdges
        {
          public:
            typedef AssocMatrix< K,V,AMatrTriangle > Type;
            // - przyklad uzycia, nie usuwac
            // typedef AssocMatrix<K,V,AMatrTriangle,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
        };

        // Dodatkowe pola (poza info) wprowadzane do obiektu wierzcholka
        template< class VertInfo, class EdgeInfo, class Settings > struct VertAdditData
        {
            // - w tej wersji umozliwia korzystanie z AssocArray
            AssocKeyContReg assocReg; 
        };

        // Dodatkowe pola (poza info) wprowadzane do obiektu krawedzi
        template< class VertInfo, class EdgeInfo, class Settings > struct EdgeAdditData
        {
            // - w tej wersji umozliwia korzystanie z AssocArray
            AssocKeyContReg assocReg; 
        };

        // czy dostosowywac rozmiar pamieci wyjsciowych tablic asocjacyjnych
        enum { ReserveOutAssocCont = true };

        // wybrany do uzytku wewnetrznego algorytm sortowania tablic
        template< class Iterator > static void sort( Iterator first, Iterator last );
        // ... i to samo z dostarczonym porownywaczem
        template< class Iterator, class Comp > static void sort ( Iterator first, Iterator last, Comp comp );
    };

    namespace Privates
    {
        /* DummyVar
         * Do uzytku wewnetrznego, typ zmiennej udajacej int lub wskaznik, przypisywalny z i konwertowalny do takiego
         * typu
         */
        template< class T > struct DummyVar
        {
            DummyVar< T > operator=( const T& arg ) const;
            operator T() const { return 0; }
        };

    }
}

#include "adjmatrix.h"
#include "edge.h"
#include "vertex.h"

namespace Koala
{
    /* SubgraphBase
     * Klasa pomocnicza, umozliwia tworzenie hierarchii podgrafow i widokow na graf
     */
    class SubgraphBase
    {
      public:
        // tworzy pusty, niepowiazany obiekt
        SubgraphBase(): parent( NULL ), next( NULL ), child( NULL ) { }
        // przylacza sie jako obiekt podrzedny rodzica podanego obiektu
        SubgraphBase( const SubgraphBase &x ): child( NULL ), parent( NULL ), next( NULL ) { link( x.parent ); }
        // j.w.
        SubgraphBase &operator=( const SubgraphBase & ); 

        // odlacza sie od rodzica (jesli istnial) i odlacza od siebie wszystkie swoje dzieci
        ~SubgraphBase(); 

      protected:
        mutable const SubgraphBase *parent,*next,*child;

        // wypisuje sie z listy potomkow obiektu nadrzednego, jednak nie usuwa powiazan ze swoimi obiektami podrzednymi
        bool unlink(); 
        // dolacza sie jako obiekt podrzedny podanego obiektu
        bool link(const SubgraphBase * = NULL ); 
    };

    namespace Privates
    {
        /* EdgeCounterLoop
         * 
         */
        template< EdgeType EdgeAllow > class EdgeCounterLoop
        {
          protected:
            typedef int &NoType;

            mutable int n;
            int &no() const { return n; }
            EdgeCounterLoop(): n( 0 ) { }
        };

        template <> class EdgeCounterLoop< 0 >
        {
          protected:
            typedef DummyVar< int > NoType;

            DummyVar< int > no() const { return DummyVar< int >(); }
        };

        /* EdgeCounterDir
         * 
         */
        template< EdgeType EdgeAllow > class EdgeCounterDir;
        template <> class EdgeCounterDir<EdDirIn|EdDirOut>
        { 
          protected:
            typedef int &NoType;

            mutable int n;
            int &no() const { return n; }
            EdgeCounterDir(): n( 0 ) { }
        };

        template <> class EdgeCounterDir< 0 >
        { 
          protected:
            typedef DummyVar< int > NoType;

            DummyVar< int > no() const { return DummyVar< int >(); }
        };

        /* EdgeCounterUndir
         * 
         */
        template< EdgeType EdgeAllow > class EdgeCounterUndir
        { 
          protected:
            typedef int &NoType;

            mutable int n;
            int &no() const { return n; }
            EdgeCounterUndir(): n( 0 ) { }
        };

        template <> class EdgeCounterUndir< 0 >
        {
          protected:
            typedef DummyVar< int > NoType;
    
            DummyVar< int > no() const { return DummyVar< int >(); }
        };

    }

    /* Graph
     * Graf wraz z wszystkimi podstawowymi operacjami wykonywanymi na grafach. Parametry grafu to klasy, których
     * instancje będą etykietami wierzchołków i krawędzi. Settings - wytyczne parametryzujace strukture i dzialanie
     * metod klasy grafu.
     */
    template< class GraphType > struct GraphInternalTypes;
    template< class VertInfo, class EdgeInfo, class Settings >
        struct GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >
    {
        typedef Koala::Vertex< VertInfo,EdgeInfo,Settings > Vertex;
        typedef Vertex* PVertex;
        typedef Koala::Edge< VertInfo,EdgeInfo,Settings > Edge;
        typedef Edge* PEdge;
        typedef VertInfo VertInfoType;
        typedef EdgeInfo EdgeInfoType;
        typedef Settings GraphSettings;
    };

#include "grconst.h"

    template< class VertInfo = EmptyVertInfo, class EdgeInfo = EmptyVertInfo,
        class Settings = GrDefaultSettings< EdAll,true > > class Graph:
        public SubgraphBase, protected Privates::EdgeCounterLoop< Settings::EdAllow & Loop >,
        protected Privates::EdgeCounterDir< Settings::EdAllow & (EdDirIn|EdDirOut) >,
        protected Privates::EdgeCounterUndir< Settings::EdAllow & Undirected >,
        public ConstGraphMethods< Graph< VertInfo,EdgeInfo,Settings > >
    {
      public:
        friend class Koala::Edge< VertInfo,EdgeInfo,Settings >;

        typedef typename GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::Vertex Vertex;
        typedef typename GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::PVertex PVertex;
        typedef typename GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::Edge Edge;
        typedef typename GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::PEdge PEdge;
        typedef typename GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::VertInfoType VertInfoType;
        typedef typename GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::EdgeInfoType EdgeInfoType;
        typedef typename GraphInternalTypes< Graph< VertInfo,EdgeInfo,Settings > >::GraphSettings GraphSettings;
        typedef Graph< VertInfo,EdgeInfo,Settings > GraphType;

        // Do wspolpracy z podgrafami
        typedef Graph< VertInfo,EdgeInfo,Settings > RootGrType; 

        // Konstruktory
        // Tworzy pusty graf, bez krawędzi i wierzchołków.
        Graph();
        // Konstruktor kopiujący.
        Graph( const Graph &graph );
        // Destruktor
        ~Graph();
        // Usuwamy wszystkie wierzchołki i krawędzie grafu.
        void clear();
        // Usuwamy wszystkie wierzchołki grafu.
        void clearEdges();
        
        // Operacje na całym grafie
        // Operator przypisania.
        Graph< VertInfo,EdgeInfo,Settings > &operator=( const Graph< VertInfo,EdgeInfo,Settings > & );
        // Dodanie kopii grafu do naszego grafu
        Graph< VertInfo,EdgeInfo,Settings > &operator+=( const Graph< VertInfo,EdgeInfo,Settings > & );
        // Przenosimy wszystkie wierzchołki i krawędzie do naszego grafu. Zwraca pierwszy wprowadzony wierzcholek
        typename GraphType::PVertex move( Graph< VertInfo,EdgeInfo,Settings > & );

        // dopuszczalne typy krawedzi grafu
        static EdgeType allowedEdgeTypes() { return Settings::EdAllow ; }

        // metody przekazywane do ConstGraphMethods
        int getVertNo() const { return this->no_vert; }
        inline typename GraphType::PVertex getVertNext( PVertex ) const;
        inline typename GraphType::PVertex getVertPrev( PVertex ) const;
        int getEdgeNo( EdgeType = EdAll ) const;
        inline typename GraphType::PEdge getEdgeNext( PEdge,EdgeType = EdAll ) const;
        inline typename GraphType::PEdge getEdgePrev( PEdge,EdgeType = EdAll ) const;
        inline int getEdgeNo( PVertex, EdgeDirection = EdAll ) const;
        typename GraphType::PEdge getEdgeNext( PVertex, PEdge, EdgeDirection = EdAll ) const;
        typename GraphType::PEdge getEdgePrev( PVertex, PEdge, EdgeDirection = EdAll ) const;
        int getEdgeNo( PVertex, PVertex, EdgeDirection = EdAll ) const;
        typename GraphType::PEdge getEdgeNext( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        typename GraphType::PEdge getEdgePrev( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        inline EdgeType getEdgeType( PEdge ) const;
        inline std::pair< typename GraphType::PVertex,typename GraphType::PVertex > getEdgeEnds( PEdge ) const;
        inline typename GraphType::PVertex getEdgeEnd1( PEdge ) const;
        inline typename GraphType::PVertex getEdgeEnd2( PEdge ) const;
        inline EdgeDirection getEdgeDir( PEdge,PVertex ) const;

        // ustawianie pol info
        inline void setVertInfo( PVertex, VertInfo = VertInfo() ) const;
        inline void setEdgeInfo( PEdge, EdgeInfo = EdgeInfo() ) const;

        // Usuwamy wierzchołek z grafu.
        // wersja z false skuteczna tylko dla wierzcholka izolowanego
        inline void del( PVertex vert, bool force = true ) { this->delVert( vert,force ); }
        // synonim
        void delVert( PVertex, bool = true ); 
        // usuwanie zbioru wierzcholkow, zwraca ich liczbe
        template< class Iterator > int delVerts( Iterator begin, Iterator end );
        // wersja odporna na powtorzenia, ktore sa pmijane
        template< class Iterator > int delVerts2( Iterator begin, Iterator end );
        inline int delVerts( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > &s )
            { return delVerts( s.begin(),s.end() ); }
        // Usuwamy wszystkie wierzchołki z grafu.
        inline int delVerts();

        // Usuwamy krawędź z grafu.
        inline void del( PEdge );
        // synonim
        inline void delEdge( PEdge edge ) { this->del( edge ); }
        // usuwanie zbiorow krawedzi
        // krawedzie przy danym wierzcholku o podanej orientacji wgledem niego
        int delEdges( PVertex, EdgeDirection = EdAll );
        // krawedzie miedzy danymi wierzcholkami o podanej orientacji
        int delEdges( PVertex,PVertex, EdgeDirection = EdAll );
        // podajemy zbior do usuniecia, uwzglednia tylko krawedzie o typie zawartym w masce
        template< class Iterator > int delEdges( Iterator begin, Iterator end, EdgeType = EdAll );
        // wersja odporna na powtorzenia, ktore sa pmijane
        template< class Iterator > int delEdges2( Iterator begin, Iterator end, EdgeType = EdAll );
        inline int delEdges( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &s,
            EdgeType direct = EdAll ) { return delEdges( s.begin(),s.end(),direct ); }
        // Usuwamy krawędzie z grafu o typie zawartym w masce
        int delEdges( EdgeType = EdAll );

        // Dodajemy nowy wierzchołek do grafu (z etykietą info). Zwraca jego wskaznik
        inline typename GraphType::PVertex addVert( VertInfo  = VertInfo() );

        // Uwaga: proba dodania krawedzi typu niezawartego w allowedEdgeTypes() wywoluje blad
        // Dodajemy nową krawędź do grafu (bez etykiety - wartosc domyslna typu EdgeInfo).
        inline typename GraphType::PEdge addEdge( PVertex,PVertex, EdgeDirection = EdUndir );
        // Dodajemy nową krawędź do grafu (z etykietą).
        inline typename GraphType::PEdge addEdge( PVertex,PVertex,EdgeInfo, EdgeDirection = EdUndir );
        // Dodajemy nowy łuk do grafu (z etykietą).
        inline typename GraphType::PEdge addArch( PVertex,PVertex,EdgeInfo = EdgeInfo() );
        // Dodajemy nową pętlę do grafu (z etykietą)
        inline typename GraphType::PEdge addLoop( PVertex,EdgeInfo = EdgeInfo() );

        // Przenosimy krawędź w inne miejsce z ew. zmiana typu
        inline bool moveEdge( PEdge,PVertex,PVertex, EdgeDirection = EdUndir );

        // zmiana krawedzi skierowanej na nieskierowana, wynik false gdy krawedz nie byla nieskierowana
        bool ch2Undir( PEdge );
        // zamiana na nieskierowane krawedzi z podanego zbioru, wynik - liczba udanych zmian
        // podany jest zbior krawedzi
        template< class Iterator > int ch2Undir( Iterator,Iterator );
        inline int ch2Undir( const Set< typename GraphType::PEdge > &s ) { return ch2Undir( s.begin(),s.end() ); }
        // wszystkie krawedzie przy danym wierzcholku o podanej orientacji wzgledem niego
        int ch2Undir( PVertex, EdgeDirection = EdDirIn|EdDirOut );
        // wszystkie krawedzie miedzy danymi wierzcholkami o podanej orientacji
        int ch2Undir( PVertex,PVertex, EdgeDirection = EdDirIn|EdDirOut );
        // wszystkie w grafie
        int ch2Undir();

        // Zmieniamy kierunek krawędzi skierowanej, wynik - czy z sukcesem
        inline bool revert( PEdge );
        // Odwrocenie krawedzi skierowanych z podanego zbioru, wynik - liczba udanych zmian
        // podany zbior krawedzi
        template< class Iterator > int revert( Iterator,Iterator );
        inline int revert( const Set< typename GraphType::PEdge > &s ){ return revert( s.begin(),s.end() ); }

        // wersja odporna na powtorzenia (ignorowane)
        template< class Iterator > int revert2( Iterator,Iterator ); 
        // wszystkie krawedzie przy danym wierzcholku o podanej orientacji wzgledem niego
        int revert( PVertex, EdgeDirection = EdDirIn|EdDirOut );
        // wszystkie krawedzie miedzy danymi wierzcholkami o podanej orientacji
        int revert( PVertex,PVertex, EdgeDirection = EdDirIn|EdDirOut );
        // wszystkie w grafie
        int revert();

        // zmiana krawedzi nieskierowanej na luk o zadanej orientacji wzgledem jej podanej koncowki, wynik - czy
        // skuteczne
        inline bool ch2Dir( PEdge,PVertex,EdgeDirection = EdDirOut );
        // jw. dla wszystkich krawedzi nieskierowanych przy danym wierzcholku, wynik - liczba zmian
        int ch2Dir( PVertex,EdgeDirection = EdDirOut );
        // jw. dla wszystkich krawedzi nieskierowanych miedzy wierzcholkami, wynik - liczba zmian
        int ch2Dir( PVertex,PVertex,EdgeDirection = EdDirOut );

        // Zmieniamy krawędź nieskierowana w dwa łuki. Nowa krawedz przejmuje info oryginalu i jest zwracana. 0
        // w razie porazki
        inline typename GraphType::PEdge ch2Archs( PEdge );
        // j.w.  dla wszystkich krawedzi z danego zbioru, zwraca liczbe sukcesow
        template< class Iterator > int ch2Archs( Iterator begin, Iterator end );
        inline int ch2Archs( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PEdge> &s )
            { return ch2Archs( s.begin(),s.end() ); }
        // j.w. dla wszystkich w grafie
        int ch2Archs();
        // j.w. dla wszystkich nieskierowanych przy wierzcholku
        int ch2Archs( PVertex );
        // j.w. dla wszystkich nieskierowanych przy miedzy wierzcholkami
        int ch2Archs( PVertex,PVertex );

        // ustawienie nowego wierzcholka na srodku istniejacej krawedzi, ktora rozdziela sie na dwie. Podajemy info
        // nowego wierzcholka, ktory jest zwracany
        typename GraphType::PVertex putVert( PEdge, VertInfo = VertInfo() );
        // Dla wierzcholka o maks. 2 sasiadach - zdjecie wierzcholka , po czym pozostaje jedna nowa (zwracana)
        // krawedz laczaca tych sasiadow
        typename GraphType::PEdge pickVert( PVertex, EdgeInfo = EdgeInfo() );

        // sklejanie grupy wierzcholkow w jeden wierzcholek docelowy (zwracany).
        // Mozna wybrac ten wierzcholek sposrod sklejanej grupy. makeloops decyduje, czy krawedzie wewnatrz grupy
        // maja byc usuwane, czy zamieniane w petle
        template< class Iterator > typename GraphType::PVertex
            glue( Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        inline typename GraphType::PVertex
            glue( const Set< typename Graph< VertInfo,EdgeInfo,Settings >::PVertex > &s, bool makeloops = false,
                  PVertex res = NULL ) { glue( s.begin(),s.end(),makeloops,res ); }
        // wersja odporna na powtorzenia (ignorowane)
        template< class Iterator > typename GraphType::PVertex
            glue2( Iterator, Iterator, bool makeloops = false, PVertex = NULL );
        // jw. dla 2 wierzcholkow
        inline typename GraphType::PVertex glue( PVertex, PVertex, bool makeloops = false );

        // usuwanie krawedzi z danego zbioru rownoleglych do podanej, maska okresla rodzaj relacji rownoleglosci.
        // Wynik - liczba usuniec
        template< class Iterator > int delParals( Iterator begin, Iterator end, PEdge, EdgeDirection = EdUndir );
        inline int delParals( const Set< typename GraphType::PEdge > &s, PEdge edge, EdgeDirection reltype = EdUndir )
            { return delParals( s.begin(),s.end(),edge,reltype ); }
        // wersja odporna na powtorzenia (ignorowane)
        template< class Iterator > int delParals2( Iterator begin, Iterator end, PEdge, EdgeDirection = EdUndir );
        // wszystkie krawedzie rownolegle
        int delParals( PEdge, EdgeDirection = EdUndir );

        // z grupy krawedzi zostawia jedynie nierownoleglych wzgledem siebie reprezentantow. Znaczenie parametrow
        // analogiczne jak w metodach findParals. Zwraca liczbe usuniec.
        template< class Iterator > int delAllParals( Iterator,Iterator, EdgeType = EdUndir );
        template< class Iterator > int delAllParals2( Iterator,Iterator, EdgeType = EdUndir );
        int delAllParals( const Set< typename GraphType::PEdge > &eset, EdgeType relType = EdUndir )
            { return delAllParals( eset.begin(),eset.end(),relType ); }
        int delAllParals( PVertex, EdgeType = EdUndir );
        int delAllParals( PVertex,PVertex, EdgeType = EdUndir );
        int delAllParals( EdgeType = EdUndir );

        // usuwa krawedzie, ktore przy tym samym ustawieniu parametrow zwracalyby metody getIncEdges. Zwraca liczbe
        // usuniec
        template< class Iterator > int delIncEdges( Iterator,Iterator, EdgeDirection = EdAll, EdgeType = Loop );
        int delIncEdges( const Set< typename GraphType::PVertex > &, EdgeDirection = EdAll, EdgeType = Loop );

        // dopelnienia podgrafu indukowanego na zadanych wierzcholkach. Dziala "osobno" na typach krawedzi zawartych
        // w masce wesje z EdInfoGen - typ funktora wyliczajacego pola info nowych krawedzi, wywolanie: EdgeInfo
        // operator()(const GraphType&,PVertex,PVertex,EdgeDirection) wersje z EdgeInfo nadaja tym polom ta sama
        // podana wartosc
        template< class Iterator, class EdInfoGen > void neg( Iterator, Iterator, EdgeType, EdInfoGen );
        template< class Iterator > void neg( Iterator beg, Iterator end, EdgeType type = Settings::EdAllow,
            EdgeInfo info = EdgeInfo() ) { neg( beg,end,type,ConstFunctor< EdgeInfo >( info ) ); }
        template< class EdInfoGen > void neg( const Set< PVertex > &vset, EdgeType type, EdInfoGen infoGen )
            { neg( vset.begin(),vset.end(),type,infoGen ); }
        void neg( const Set< PVertex > &vset, EdgeType = Settings::EdAllow, EdgeInfo = EdgeInfo() );
        // ... i dopelnienie calego grafu
        template< class EdInfoGen > void neg( EdgeType type, EdInfoGen infoGen );
        void neg( EdgeType type = Settings::EdAllow, EdgeInfo = EdgeInfo() ) { neg( type,ConstFunctor< EdgeInfo >() ); }

        // Kopiowanie do naszego innego grafu (np. innego typu). Wierzcholki dotychczasowe pozostaja na liscie
        // przed skopiowanymi. W najogolniejszej wersji mozna podac
        // - pare chooserow wybierajacych podgraf wierzcholkow i krawedzi do skopiowania (konce krawedzi takze musza
        // spelniac chooser wierzcholkowy)
        // - pare casterow wyliczajacych infa nowych wierzcholkow i krawedzi na podstawie oryginalow
        // - pare linkerow (dla wierzcholkow i krawedzi) ustalajacych polaczenia miedzy oryginalami i kopiami.
        // Elementy dotychczasowe tego grafu oraz nieskopiowane grafu kopiowanego sa linkowane z NULL
        // Zwraca pierwszy wprowadzony wierzcholek
        template< class ExtGraph > typename GraphType::PVertex copy( const ExtGraph & );
        template< class ExtGraph, class VChooser, class EChooser >
            typename GraphType::PVertex copy( const ExtGraph &, std::pair< VChooser,EChooser > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster >
            typename GraphType::PVertex copy( const ExtGraph &, std::pair< VChooser,EChooser >,
                std::pair< VCaster,ECaster > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster, class VLinker,
            class ELinker > typename GraphType::PVertex copy( ExtGraph &, std::pair< VChooser,EChooser >,
                std::pair< VCaster,ECaster >, std::pair< VLinker,ELinker > );
        // Podstawienie kopii drugiego grafu za dotychczasowy wierzcholek (ktory jest usuwany). Znaczenie parametrow
        // parametry podobne jak w copy
        // Zwraca pierwszy wprowadzony wierzcholek
        template< class ExtGraph > typename GraphType::PVertex substitute( typename GraphType::PVertex, ExtGraph & );
        template< class ExtGraph, class VChooser, class EChooser >
            typename GraphType::PVertex substitute( PVertex, ExtGraph &, std::pair< VChooser,EChooser > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster >
            typename GraphType::PVertex substitute( PVertex, ExtGraph &, std::pair< VChooser,EChooser >,
                std::pair< VCaster,ECaster > );
        template< class ExtGraph, class VChooser, class EChooser, class VCaster, class ECaster, class VLinker,
            class ELinker > typename GraphType::PVertex substitute( typename GraphType::PVertex, ExtGraph &,
                std::pair< VChooser,EChooser >, std::pair< VCaster,ECaster >, std::pair< VLinker,ELinker > );

        // Operacje na macierzy sąsiedztwa
        // Tworzymy macierz sąsiedztwa.
        bool makeAdjMatrix();
        // Usuwamy macierz sąsiedztwa.
        inline bool delAdjMatrix();
        // Informacja o tym, czy graf ma macierz sąsiedztwa.
        inline bool hasAdjMatrix() const;

        // czy ten typ grafu obsluguje macierz sasiedztwa
        static bool allowedAdjMatrix() { return Settings::AdjMatrixAllowed; }
        // alokacja pamieci na podana liczbe wierzcholkow
        inline void reserveAdjMatrix( int );

        // do wspolpracy z hierarchia podgrafow i widokow.
        const Graph< VertInfo,EdgeInfo,Settings > *getRootPtr() const { return this; }
        const Graph< VertInfo,EdgeInfo,Settings >& root() const { return *this; }
        bool good( PVertex, bool = false ) const { return true; }
        bool good( PEdge, bool = false ) const { return true; }

      private:
        AdjMatrix< VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed > *pAdj;

        PVertex first_vert,last_vert;
        PEdge first_edge,last_edge;
        //        int&
        typename Privates::EdgeCounterLoop< Settings::EdAllow &Loop >::NoType
            no_loop_edge() const { return this->Privates::EdgeCounterLoop< Settings::EdAllow &Loop >::no(); }
        //        int&
        typename Privates::EdgeCounterDir< Settings::EdAllow & (EdDirIn|EdDirOut) >::NoType
            no_dir_edge() const { return this->Privates::EdgeCounterDir< Settings::EdAllow & (EdDirIn|EdDirOut) >::no(); }
        //        int&
        typename Privates::EdgeCounterUndir< Settings::EdAllow & Undirected >::NoType
            no_undir_edge() const { return this->Privates::EdgeCounterUndir< Settings::EdAllow & Undirected >::no(); }
        int no_vert;

        inline typename GraphType::PVertex attach( PVertex );
        inline typename GraphType::PVertex detach( PVertex );
        inline typename GraphType::PEdge attach( PEdge, PVertex, PVertex, EdgeDirection );
        typename GraphType::PEdge attach_undir( PEdge, PVertex, PVertex );
        typename GraphType::PEdge attach_dir( PEdge, PVertex, PVertex );
        typename GraphType::PEdge attach_loop( PEdge, PVertex );
        typename GraphType::PEdge detach( PEdge );
    } ;
    
#include "graph.hpp"
}

#endif
    