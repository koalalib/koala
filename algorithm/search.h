#ifndef KOALA_DEF_SEARCH_H
#define KOALA_DEF_SEARCH_H

/* search.h
 *
 */

#include "../base/defs.h"
#include "../graph/view.h"

namespace Koala
{

    /* SearchStructs
     *
     */
    struct PathStructs
    {
        // Do odczytu sciezki miedzy wierzcholkiem a korzeniem, gdy droga wyznaczona jest w postaci
        // tablicy asocjacyjnej PVertex -> rekord z polami vPrev, ePrev (wierzcholek poprzedni i krawedz do niego).
        // Przydatne w roznych algorytmach najkrotszej sciezki
        // Uzytkownik podaje, pare iteratorow, gdzie wpisac wierzcholki i krawedzie najkrotszej sciezki
        template< class VIter, class EIter > struct OutPath
        {
            VIter vertIter;
            EIter edgeIter;

            OutPath( VIter av, EIter ei ): vertIter( av ), edgeIter( ei ) { }
        };

        // funkcja tworzaca, analogia make_pair
        // Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
        template< class VIter, class EIter > static OutPath< VIter,EIter > outPath( VIter av, EIter ei )
            { return OutPath< VIter,EIter >( av,ei ); }

        //        OutPath moze wspolpracowac z dowolnymi sekwencyjnymi kontenerami, ale ponizsza struktura
        //        ulatwia obrobke takich danych
        template< class Graph > class OutPathTool
        {
          private:
            std::vector< typename Graph::PVertex > verts;
            std::vector< typename Graph::PEdge > edges;

          public:
            typedef typename Graph::PVertex PVertex;
            typedef typename Graph::PEdge PEdge;
            typedef std::back_insert_iterator< std::vector< typename Graph::PEdge > > EdgeIterType;
            typedef std::back_insert_iterator< std::vector< typename Graph::PVertex > > VertIterType;

            OutPathTool()
                { clear(); }
            void clear();
            // dlugosc wpisanej sciezki, -1 w razie bledu (jeszcze zadnej nie wpisano)
            int length() const
                { return verts.size() - 1; }
            // i-ta krawedz
            PEdge edge( int i ) const;
            // i-ta wierzcholek
            PVertex vertex( int i) const;
            //Umiesczamy wywolanie funkcji w miejsu outPath a pozniej przetwarzamy zebrane ciagi
            // czysci kontener
            OutPath< std::back_insert_iterator< std::vector< typename Graph::PVertex > >,
                std::back_insert_iterator< std::vector< typename Graph::PEdge > > > input();
        };

    };

    /* ShortPathStructs
     *
     */
    struct ShortPathStructs: public PathStructs
    {
        // zwraca liczbe krawedzi sciezki
        // vertTab, tablica asoc. przypisujaca wierzcholkom rekordy z ustawionymi
        // wskaznikami do poprzedniego wierzcholka (vPrev) i krawedzi don prowadzacej (ePrev, NULL dla korzenia)
        // W tej formie zwraca wynik wiele procedur tworzacych w grafie drzewo (las) skierowany do korzenia np. Dijkstra, BFS itp
        // OutPath, miejsce gdzie sciezka zostanie zapisana
        // sciezka prowadzi "pod prad" tj. od korzenia do tego wierzcholka
        // ew. wczesniejszy punkt koncowy na sciezce miedzy end a korzeniem
        template< class GraphType, class VertContainer, class VIter, class EIter >
            static int getOutPath( const GraphType &g, const VertContainer &vertTab, OutPath< VIter,EIter > iters,
            typename GraphType::PVertex end, typename GraphType::PVertex start = 0 );

        // Zapisuje na podany iterator wszystkie krawedzie nalezace do drzewa (lasu) tj. uzyte jako wartosci pol ePrev
        // Zwraca ich liczbe
        template< class GraphType, class VertContainer, class Iter > static int getUsedEdges( const GraphType &g,
            const VertContainer &vertTab, Iter iter );
        // jw. ale zwraca zbior krawedzi
        template< class GraphType, class VertContainer > static Set< typename GraphType::PEdge >
            getUsedEdgeSet( const GraphType &g, const VertContainer &vertTab );
    };


    /* SearchStructs
     *
     */
    class SearchStructs
    {
      public:
        // Struktura wartosci przypisywanej wierzcholkowi w procedurach przeszukiwania grafu
        template< class GraphType > struct VisitVertLabs
        {
            // rodzic danego wierzcholka w drzewie (lesie), NULL dla korzenia
            typename GraphType::PVertex vPrev;
            // krawedz prowadzaca do rodzica
            typename GraphType::PEdge ePrev;

            // odleglosc od korzenia (liczba krawedzi) i numer skladowej spojnosci (od 0)
            int distance,component;

            VisitVertLabs( typename GraphType::PVertex vp = 0, typename GraphType::PEdge ep = 0,
                int dist = std::numeric_limits< int >::max(), int comp = -1 ):
                    vPrev( vp ), ePrev( ep ), distance( dist ), component( comp )
                { }
        };

        //        Para iteratorow wyjsciowych do kodowania ciagu ciagow (zazwyczaj wierzcholki/kraw)
        //        kolejne elementy kolejnych ciagow wysylane sa na vertIter
        //        na compIter wyrzucane sa inty: pozycje (numeracja od 0) poczatkow kolejnych ciagow
        //        oraz na koniec sumaryczna dlugosc wszystkich ciagow
        //        (a wiec lacznie o 1 wiecej liczb, niz ilosc ciagow)
        template< class CIter, class VIter > struct CompStore
        {
            CIter compIter;
            VIter vertIter;

            CompStore( CIter ac, VIter av ): compIter( ac ), vertIter( av )
                { }
        } ;

        // Funkcja tworzaca, analogia make_pair
        // Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
        template< class CIter, class VIter > static CompStore< CIter,VIter > compStore( CIter ac, VIter av )
            { return CompStore< CIter,VIter >( ac,av ); }

        // Odwrotne mapowanie ciagu dlugosci size ciagow o elementach typu T o poczatkach w iteratorach
        // begin (kolejne rozmiary) i sbegin (wartosci) - por. powyzszy format
        // out, wyjsciowy ciag numerow (od 0) ciagow: i-ty ciag to numery
        // w ktorych wystepuje i-ty element poslany na eout (w razie powtorzen ten sam numer ciagu powtarza sie
        // odpowiednia ilosc razy.
        // eout, iterator wyjsciowy na wszystkie elementy w wystepujace ciagach (kazdy jeden raz)
        // zwraca ilosc elementow wystepujacych w ciagach (kazdy jeden raz) tj. dlugosc sekwencji poslanej na eout
        // uwaga: przy wywolaniach nie dedukuje typu elementow ciagow T (tzreba podac jawnie)
        template< class T, class InputIter, class VertInputIter, class CIter, class IntIter, class ElemIter >
            static int revCompStore( InputIter begin, VertInputIter sbegin, int size, CompStore< CIter,IntIter > out,
                ElemIter eout );
        // j.w., ale zakladamy, ze elementy ciagow (a wiec sbegin) sa przechowywane w tablicy, ktorej wskaznik
        // poczatku dostarczamy do procedury (wowczas typ T jest dedukowany).
        template< class T, class InputIter, class CIter, class IntIter, class ElemIter >
            static int revCompStore( InputIter begin, const T *sbegin, int size, CompStore< CIter,IntIter > out,
                ElemIter eout )
            {
                return revCompStore< T,InputIter,const T *,CIter,IntIter,ElemIter >( begin,sbegin,size,out,eout );
            }

        //        CompStore moze wspolpracowac z dowolnymi sekwencyjnymi kontenerami, ale ponizsza struktura
        //        ulatwia obrobke takich danych
        // T typ elementu ciagow skladowych
        template< class T > class CompStoreTool
        {
          private:
            std::vector< int > idx;
            std::vector< T > data;

          public:
            typedef T ValueType;
            typedef std::back_insert_iterator< std::vector< int > > CIterType;
            typedef std::back_insert_iterator< std::vector< T > > VIterType;

            CompStoreTool()
                { clear(); }
            void clear();
             // liczba zebranych ciagow
            int size() const;
             // dlugosc i-tego ciagu
            int size( int i ) const;
             // laczna dlugosc wszystkich ciagow
            int length() const;
            // wskaznik poczatku i-tego ciagu lub 0 gdy ten ciag jest pusty
            T *operator[]( int i );
            // wskaznik poczatku i-tego ciagu lub 0 gdy ten ciag jest pusty
            const T *operator[]( int i ) const;
            // umieszcza nowy ciag pusty na pozycji i.
            void insert( int i );
            // kasuje ciag na pozycji i
            void del( int i );
            // zmienia dlugosc i-tego ciagu.
            void resize( int i, int asize );
            //Umiesczamy wywolanie funkcji w miejsu compStore a pozniej przetwarzamy zebrane ciagi
            // czysci kontener
            CompStore< std::back_insert_iterator< std::vector< int > >,std::back_insert_iterator< std::vector< T > > >
                input();
        };

    };

    /* Visitors
     * visitor's code is called by searching algorithms (BFS, DFS, LexBFS)
     * visitor should inherit from one of:
     * - simple_visitor_tag
     * - simple_preorder_visitor_tag
     * - simple_postorder_visitor_tag
     * - complex_visitor_tag
     * and one of:
     * - component_visitor_tag
     * - no_component_visitor_tag
     *
     * simple visitors (inheriting from simple_*_tag) have operator() called
     * with the following arguments:
     * template< class GraphType >
     * bool operator()(const GraphType &g,
     *         typename GraphType::PVertex u,
     *         VisitVertLabs< GraphType > &data)
     * where g - graph containing vertex, u - visited vertex, data - visited vertex's data
     * returning true allows to continue search, retrurning false terminate
     * searching algorithm
     * simple_preorder_visitor_tag indicate to visit vertex before its neighbourrs
     * simple_postorder_visitor_tag indicate to visit vertex after its neighbourrs
     * simple_visitor_tag do not specify the order
     *
     * complex visitors are notified by a searching algorithm using following methods:
     * (g - graph containing vertex, u - visited vertex,
     *  data - visited vertex's data, e - visited edge, v - vertex from which we
     *  visit edge e)
     *
     * template< class GraphType >
     * bool visitVertexPre(const GraphType &g,
     *             typename GraphType::PVertex u,
     *             VisitVertLabs< GraphType > &data);
     * called before visiting u's neighbours
     * returning false prevents visiting u's neighbours
     *
     * template< class GraphType >
     * bool visitVertexPost(const GraphType &g,
     *          typename GraphType::PVertex u,
     *          VisitVertLabs< GraphType > &data);
     * called after visiting u's neighbours
     * returning false terminates searching
     *
     * template< class GraphType >
     * bool visitEdgePre(const GraphType &g,
     *           typename GraphType::PEdge e,
     *           typename GraphType::PVertex v);
     * called before visiting other end of e
     * returning false prevents visiting vertex on the other end of e
     *
     * template< class GraphType >
     * bool visitEdgePost(const GraphType &g,
     *            typename GraphType::PEdge e,
     *            typename GraphType::PVertex v);
     * called after visiting other end of e
     * returning false terminates searching
     *
     * visitors with component_visitor_tag are notified about components with
     * methods:
     * template< class GraphType >
     * bool beginComponent(const GraphType &g, unsigned compid);
     * template< class GraphType >
     * bool endComponent(const GraphType &g, unsigned compid);
     * where g is the visited graph and compid is the component number (starting from 0)
     * return values are ignored
     * TODO: uzyc albo zmienic na void
     *
     */
     //TODO: vizytory nie powinny zakladac, ze dostaja rekord VisitVertLabs< GraphType > tylko cokolwiek o takiej
     //strukturze tzn. dodac parametr szablonu. To samo dotyczy implementacji konkretnych przeszukiwan
    class Visitors: public SearchStructs
    {
      public:
        class component_visitor_tag { } ;
        class no_component_visitor_tag { } ;
        class simple_visitor_tag { } ;
        class simple_preorder_visitor_tag: public simple_visitor_tag { } ;
        class simple_postorder_visitor_tag: public simple_visitor_tag { } ;
        class complex_visitor_tag { } ;

        template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_preorder_visitor_tag &s )
             { (void)(s); return v( g,u,r ); }

        template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_postorder_visitor_tag &s )
            { (void)(g); (void)(v); (void)(u); (void)(r); (void)(s); return true; }

        template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, complex_visitor_tag &c )
            { (void)(c); return v.visitVertexPre( g,u,r ); }

        template< class GraphType, class Visitor > static bool visitVertexPre( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, ... )
            { return v.operator()( g,u,r ); }

        template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_preorder_visitor_tag &s )
            { (void)(g); (void)(v); (void)(u); (void)(r); (void)(s); return true; }

        template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, simple_postorder_visitor_tag &s )
            { (void)(s); return v( g,u,r ); }

        template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, complex_visitor_tag &c )
            { (void)(c); return v.visitVertexPost( g,u,r); }

        template< class GraphType, class Visitor > static bool visitVertexPost( const GraphType &g, Visitor &v,
            typename GraphType::PVertex u, VisitVertLabs< GraphType > &r, ... )
            { return v.operator()( g,u,r ); }

        template< class GraphType, class Visitor > static bool visitEdgePre( const GraphType &g, Visitor &v,
            typename GraphType::PEdge e, typename GraphType::PVertex u, complex_visitor_tag &c )
            { (void)(c); return v.visitEdgePre( g,e,u ); }

        template< class GraphType, class Visitor > static bool visitEdgePre( const GraphType &g, Visitor &v,
            typename GraphType::PEdge e, typename GraphType::PVertex u, simple_visitor_tag &s )
            { (void)(g); (void)(v); (void)(e); (void)(u); (void)(s); return true; }

        template< class GraphType, class Visitor > static bool visitEdgePre( const GraphType &g, Visitor &v,
            typename GraphType::PEdge e, typename GraphType::PVertex u, ... )
            { (void)(g); (void)(v); (void)(e); (void)(u); return true; }

        template< class GraphType, class Visitor > static bool visitEdgePost( const GraphType &g, Visitor &v,
            typename GraphType::PEdge e, typename GraphType::PVertex u, complex_visitor_tag &c )
            { (void)(c); return v.visitEdgePost( g,e,u ); }

        template< class GraphType, class Visitor > static bool visitEdgePost( const GraphType &g, Visitor &v,
            typename GraphType::PEdge e, typename GraphType::PVertex u, simple_visitor_tag &s )
            { (void)(g); (void)(v); (void)(e); (void)(u); (void)(s); return true; }

        template< class GraphType, class Visitor > static bool visitEdgePost( const GraphType &g, Visitor &v,
            typename GraphType::PEdge e, typename GraphType::PVertex u, ... )
            { (void)(g); (void)(v); (void)(e); (void)(u); return true; }

        template< class GraphType, class Visitor > static bool beginComponent( const GraphType &g, Visitor &v,
            unsigned comp, component_visitor_tag &c )
            { (void)(c); return v.beginComponent( g,comp ); }

        template< class GraphType, class Visitor > static bool beginComponent( const GraphType &g, Visitor &v,
            unsigned comp, no_component_visitor_tag &c )
            { (void)(g); (void)(v); (void)(comp); (void)(c); return true; }

        template< class GraphType, class Visitor > static bool beginComponent( const GraphType &g, Visitor &v,
            unsigned comp, ... )
            { (void)(g); (void)(v); (void)(comp); return true; }

        static bool beginComponent( ... )
        { return true; }

        template< class GraphType, class Visitor > static bool endComponent( const GraphType &g, Visitor &v,
            unsigned comp, component_visitor_tag &c )
            { (void)(c); return v.endComponent( g,comp ); }

        template< class GraphType, class Visitor > static bool endComponent( const GraphType &g, Visitor &v,
            unsigned comp, no_component_visitor_tag &c )
            { (void)(g); (void)(v); (void)(comp); (void)(c); return true; }

        template< class GraphType, class Visitor > static bool endComponent( const GraphType &g, Visitor &v,
            unsigned, ... )
            { (void)(g); (void)(v); return true; }

        static bool endComponent( ... )
        { return true; }

        /*
         * EmptyVisitor
         */
        class EmptyVisitor: public simple_postorder_visitor_tag, public no_component_visitor_tag
        {
          public:
            EmptyVisitor()
                { }

            template< class GraphType >  bool operator()( const GraphType &g, typename GraphType::PVertex u,
                VisitVertLabs< GraphType > &r )
                { return true; }
         };

        /*
         * EndVertVisitor
         * terminate algorithm when reaching given vertex
         */
        class EndVertVisitor: public complex_visitor_tag, public no_component_visitor_tag
        {
          public:
            EndVertVisitor( void *arg ): ptr( arg )
                { }

            template< class GraphType > bool visitVertexPre( const GraphType &g, typename GraphType::PVertex u,
                VisitVertLabs< GraphType > &data )
                { return true; }

            template< class GraphType > bool visitVertexPost( const GraphType &g, typename GraphType::PVertex u,
                VisitVertLabs< GraphType > &v )
                { return true; }

            template< class GraphType > bool visitEdgePre( const GraphType &g, typename GraphType::PEdge e,
                typename GraphType::PVertex u )
                { return (void*)u != ptr; }

            template< class GraphType > bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e,
                typename GraphType::PVertex u )
                { return !e || (void*)g.getEdgeEnd( e,u ) != ptr; }

          private:
            void *ptr;
        };

        /*
         * StoreTargetToVertIter
         */
        template< class VertIter > class StoreTargetToVertIter: public simple_postorder_visitor_tag,
            public no_component_visitor_tag
        {
          public:
            StoreTargetToVertIter( VertIter &i ): m_iter( i )
            { }

            template< class GraphType > bool operator()( const GraphType &, typename GraphType::PVertex,
                VisitVertLabs< GraphType > & );

          private:
            VertIter &m_iter;
        } ;

        /*
         * StoreCompVisitor
         */
        template< class CompIter, class VertIter > class StoreCompVisitor: public simple_postorder_visitor_tag,
            public component_visitor_tag
        {
          public:
            typedef struct _State
            {
                CompStore< CompIter,VertIter > iters;
                unsigned p, id;

                _State( CompStore< CompIter,VertIter > i );
            } State;

            StoreCompVisitor( State &st ): m_st( st )
            { }

            template< class GraphType > bool operator()( const GraphType &, typename GraphType::PVertex,
                VisitVertLabs< GraphType > & );

            template< class GraphType > bool beginComponent( const GraphType &g, unsigned u )
                { (void)(g); (void)(u); return true; }
            template< class GraphType > bool endComponent( const GraphType &, unsigned );

          private:
            State &m_st;
        };

        /*
         * ComplexPreorderVisitor
         */
        template< class Visitor > class ComplexPreorderVisitor: public complex_visitor_tag
        {
          public:
            ComplexPreorderVisitor( Visitor &v ): visit( v )
            { }

            template< class GraphType > bool visitVertexPre( const GraphType &g, typename GraphType::PVertex u,
                VisitVertLabs< GraphType > &data )
                { return visit.operator()( g,u,data ); }

            template< class GraphType > bool visitVertexPost( const GraphType &g, typename GraphType::PVertex u,
                VisitVertLabs< GraphType > &data )
                { return true; }

            template< class GraphType > bool visitEdgePre( const GraphType &g, typename GraphType::PEdge e,
                typename GraphType::PVertex u )
                { return true; }

            template< class GraphType > bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e,
                typename GraphType::PVertex u )
                { return true; }

          private:
            Visitor &visit;
        };

        /*
         * ComplexPostorderVisitor
         */
        template< class Visitor > class ComplexPostorderVisitor: public complex_visitor_tag
        {
          public:
            ComplexPostorderVisitor( Visitor &v ): visit( v )
                { }

            template< class GraphType > bool visitVertexPre( const GraphType &g, typename GraphType::PVertex u,
                VisitVertLabs<GraphType> &data )
                { return true; }

            template< class GraphType > bool visitVertexPost( const GraphType &g, typename GraphType::PVertex u,
                VisitVertLabs< GraphType > &data )
                { return visit.operator()( g,u,data ); }

            template< class GraphType > bool visitEdgePre( const GraphType &g, typename GraphType::PEdge e,
                typename GraphType::PVertex u )
                { return true; }

            template< class GraphType > bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e,
                typename GraphType::PVertex u )
                { return true; }

          private:
            Visitor &visit;
        };
    };

    /* GraphSearchBase
     * Ogolna implementacja przeszukiwania grafu roznymi strategiami (DFS, BFS, LexBFS)
     * Strategie dostarcza metoda visitBase z klasy SearchImpl, ktora odwiedza wierzcholki stosownie do swojej klasy
     * DefaultStructs dostarcza wytycznych dla wewnetrznych procedur por. np. AlgsDefaultSettings z def_struct.h
     *
     */
    template< class SearchImpl, class DefaultStructs > class GraphSearchBase: public ShortPathStructs, public SearchStructs
    {
      protected:
        // Typowy kontener dla wierzcholkow uzyteczny w tych procedurach
        // mapa PVertex -> VisitVertLabs< GraphType >
        template< class GraphType > class VisitedMap: public DefaultStructs:: template AssocCont<
            typename GraphType::PVertex,VisitVertLabs< GraphType > >::Type
            {
              public:
                // inicjujemy przewidywanym potrzebnym rozmiarem
                VisitedMap( int asize ): DefaultStructs:: template
                    AssocCont< typename GraphType::PVertex,VisitVertLabs< GraphType > >::Type( asize ) { }
            };

      public:
        typedef SearchImpl SearchStrategy;

        //TODO: przeszukiwania powinny dzialac na lokalnej mapie i pozwalac na blackHole - mniej przeciazen

        /** visit all vertices in a graph tj. w kolejnosci zgodnej ze strategia SearchImpl
         * @param[in] g graph containing vertices to visit
         * @param[in] visited container to store data (np. map PVertex -> VisitVertLabs), BlackHole niedozwolony.
         Po wykonaniu postac lasu przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
         component zawiera numer skladowej lasu
         * @param[in] visitor visitor called for each vertex
         * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
         zgodnych z maska, a pozostale kraw. ignorowane. Mozna
         sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
         * @return number of components
         */
        template< class GraphType, class VertContainer, class Visitor >
            static int visitAllBase( const GraphType &g, VertContainer &visited, Visitor visitor, EdgeDirection dir );

        /** visit all vertices in the same component as a given vertex tj. w kolejnosci zgodnej ze strategia SearchImpl
         * @param[in] g graph containing vertices to visit
         * @param[in] src given vertex
         * @param[out] out iterator to write visited vertices to tj. w kolejnosci zgodnej ze strategia SearchImpl
         * @param[in] dir direction of edges to consider + uwaga jak wyzej, ponadto bit petli ignorowany
         * @param[in] visited container to store data (map PVertex -> VisitVertLabs) , BlackHole niedozwolony
         Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
         component=0
         * @return number of visited vertices
         */
        template< class GraphType, class VertContainer, class Iter > static int scanAttainable( const GraphType &,
            typename GraphType::PVertex, Iter, EdgeDirection, VertContainer & );

        /** visit all vertices in the same component as a given vertex
         Tzn. dziala jak wyzej, ale na wlasnej wewnetrznej mapie wierzcholkow
         * @param[in] g graph containing vertices to visit
         * @param[in] src given vertex
         * @param[out] out iterator to write vertices to tj. w kolejnosci zgodnej ze strategia SearchImpl
         * @param[in] dir direction of edges to consider + uwaga jak wyzej, ponadto bit petli ignorowany
         * @return number of visited vertices
         */
        template< class GraphType, class VertIter > static int scanAttainable( const GraphType &g,
            typename GraphType::PVertex src, VertIter out, EdgeDirection dir = EdUndir | EdDirOut );

        /** visit all vertices in a graph
         * @param[in] g graph containing vertices to visit
         * @param[out] out iterator to write visited vertices to tj. w kolejnosci zgodnej ze strategia SearchImpl
         * @param[in] dir direction of edges to consider, petle ignorowane
         * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
                 Po wykonaniu postac lasu przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
         component zawiera numer skladowej lasu
         * @param[in] sym dla true maska
           jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
         * @return number of components
         */
        template< class GraphType, class VertContainer, class VertIter > static int scan( const GraphType &g,
            VertIter out, EdgeDirection dir, VertContainer &visited, bool sym = true );

        /** visit all vertices in a graph
         * @param[in] g graph containing vertices to visit
         * @param[out] out iterator to write vertices to, tj. w kolejnosci zgodnej ze strategia SearchImpl
         Petle ignorowane a maska
         jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
         * @param[in] dir direction of edges to consider
         * @return number of components
         */
        template< class GraphType, class VertIter > static int scan( const GraphType &g, VertIter out,
            EdgeDirection dir = EdDirOut | EdUndir | EdDirIn );

        /** Liczba cyklomatyczna podgrafu zlozonego z krawedzi zgodnych z maska */
        template< class GraphType > static int cyclNo( const GraphType &g, EdgeDirection mask = EdAll )
            { return g.getEdgeNo( mask ) - g.getVertNo() + scan( g,blackHole,mask ); }

        /** return all vertices in the same component as a given vertex
         * @param[in] g graph containing vertices to visit
         * @param[in] src given vertex
         * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
         zgodnych z maska, a pozostale kraw. ignorowane. Mozna
         sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem, pod prad lub w obie strony,
         ponadto bit petli ignorowany
         * @return set of vertices in the component
         */
        template< class GraphType > static Set< typename GraphType::PVertex > getAttainableSet( const GraphType &g,
            typename GraphType::PVertex src, EdgeDirection dir = EdDirOut | EdUndir );

        /** find a path between vertices
         * @param[in] g graph to search path in
         * @param[in] src starting vertex
         * @param[in] dest target vertex
         * @param[out] path found path
         * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
         zgodnych z maska, a pozostale kraw. ignorowane. Mozna
         sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem, pod prad lub w obie strony,
          petle ignorowane
         * @return length of the path, -1 gdy nie ma polaczenia
         */
        template< class GraphType, class VertIter, class EdgeIter > static int getPath( const GraphType &g,
            typename GraphType::PVertex src, typename GraphType::PVertex dest, OutPath< VertIter,EdgeIter > path,
            EdgeDirection dir = EdUndir | EdDirOut );

        /** split graph into components
         * @param[in] g graph to split
         * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
         * @param[in] dir direction of edges to consider, petle ignorowane a maska
         jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
         * @param[in] visited container to store data (map PVertex -> VisitVertLabs), blackHole niedozwolony
         Po wykonaniu postac lasu przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
         component zawiera numer skladowej lasu
         * @return number of components
         */
        template< class GraphType, class VertContainer, class CompIter, class VertIter > static int split(
            const GraphType &g, CompStore< CompIter,VertIter > out, EdgeDirection dir, VertContainer &visited );

        /** split graph into components
         Tzn. jak wyzej, tylko dziala na wewnetrznej lokalnej mapie
         * @param[in] g graph to split
         * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
         * @param[in] dir direction of edges to consider, petle ignorowane a maska
         jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
         * @return number of components
         */
        template< class GraphType, class CompIter, class VertIter > static int split( const GraphType &g,
            CompStore< CompIter,VertIter > out, EdgeDirection dir = EdUndir | EdDirOut | EdDirIn );
    };

    /*
    * DFSParamBlock
    */
    template< class GraphType, class VertContainer, class Visitor > struct DFSParamBlock: public SearchStructs
    {
        const GraphType &g;
        VertContainer &visited;
        Visitor &visit;
        EdgeDirection mask;
        int component;

        DFSParamBlock( const GraphType &_g, VertContainer &_v, Visitor &_vis, EdgeDirection _m, int _c ):
            g( _g ), visited( _v ), visit( _vis ), mask( _m ), component( _c )
            { }
    };

    /*
    * DFSBase
    * DefaultStructs - wytyczne dla wewnetrznych procedur
    */
    template< class SearchImpl, class DefaultStructs > class DFSBase: public GraphSearchBase< SearchImpl,DefaultStructs >
    {
      protected:
        template< class GraphType, class VertContainer, class Visitor > static int dfsDoVisit(
            DFSParamBlock< GraphType,VertContainer,Visitor > &, typename GraphType::PVertex, unsigned );

      public:
        /** visit all vertices in the same component as a given vertex
        * @param[in] g graph containing vertices to visit
        * @param[in] src given vertex
        * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
        Po wykonaniu  postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
        component jest ustawiane na compid
        * @param[in] visitor visitor called for each vertex
        * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
        sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
        * @param[in] compid component identifier (give 0 if don't know)
        * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
        */
        template< class GraphType, class VertContainer, class Visitor > static int dfsVisitBase( const GraphType &g,
            typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
    } ;


    /*
    * Preorder Depth-First-Search
    * DefaultStructs - wytyczne dla wewnetrznych procedur
    */
    template< class DefaultStructs > class DFSPreorderPar: public DFSBase< DFSPreorderPar< DefaultStructs >,DefaultStructs >
    {
      protected:
        template< class GraphType, class VertContainer, class Visitor > static int dfsPreVisitBase( const GraphType &,
            typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int,
            Visitors::complex_visitor_tag & );

        template< class GraphType, class VertContainer, class Visitor > static int dfsPreVisitBase( const GraphType &,
            typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int,
            Visitors::simple_visitor_tag & );

      public:
        /** visit all vertices in the same component as a given vertex
        * @param[in] g graph containing vertices to visit
        * @param[in] src given vertex
        * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
        Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
        component jest ustawiane na compid
        * @param[in] visitor visitor called for each vertex
        * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
        sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
        * @param[in] compid component identifier (give 0 if don't know)
        * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
        */
        template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType &g,
            typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
    } ;

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class DFSPreorder: public DFSPreorderPar< AlgsDefaultSettings > { };

    /*
    * Postorder Depth-First-Search
    */
    template< class DefaultStructs > class DFSPostorderPar: public DFSBase< DFSPostorderPar< DefaultStructs >,DefaultStructs >
    {
      protected:
        template< class GraphType, class VertContainer, class Visitor > static int dfsPostVisitBase( const GraphType &,
            typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int,
            Visitors::complex_visitor_tag & );

        template< class GraphType, class VertContainer, class Visitor > static int dfsPostVisitBase( const GraphType &,
            typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int, Visitors::simple_visitor_tag & );

      public:
        /** visit all vertices in the same component as a given vertex
        * @param[in] g graph containing vertices to visit
        * @param[in] src given vertex
        * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
        Po wykonaniu  postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
        component jest ustawiane na compid
        * @param[in] visitor visitor called for each vertex
        * @param[in] dir direction of edges to considertzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
        sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
        * @param[in] compid component identifier (give 0 if don't know)
        * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
        */
        template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType &g,
            typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
    } ;

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class DFSPostorder: public DFSPostorderPar< AlgsDefaultSettings > { };

    /*
    * Breadth-First-Search
    */
    template< class DefaultStructs > class BFSPar: public GraphSearchBase< BFSPar< DefaultStructs >,DefaultStructs >
    {
      protected:
        template< class GraphType, class VertContainer, class Visitor > static int bfsDoVisit( const GraphType &,
            typename GraphType::PVertex, VertContainer &, Visitor, EdgeDirection, int );

      public:
        /** visit all vertices in the same component as a given vertex
        * @param[in] g graph containing vertices to visit
        * @param[in] src given vertex
        * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
        Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
        component jest ustawiane na compid
        * @param[in] visitor visitor called for each vertex
        * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
        sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
        * @param[in] compid component identifier (give 0 if don't know)
        * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
        */
        template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType &g,
            typename GraphType::PVertex src, VertContainer &visited, Visitor visitor, EdgeDirection dir, int compid );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class BFS: public BFSPar< AlgsDefaultSettings > { };

    /*
    * lexicographical Breadth-First-Search
    */
    template< class DefaultStructs > class LexBFSPar:
        public GraphSearchBase< LexBFSPar< DefaultStructs >,DefaultStructs >
    {
      protected:
        template< class Graph > struct LVCNode
        {
            typename Graph::PVertex v;
            Privates::List_iterator< LVCNode > block;

            LVCNode( typename Graph::PVertex _v = NULL): v( _v ) { }
            LVCNode( typename Graph::PVertex _v, Privates::List_iterator< LVCNode > it ): v( _v ), block( it )
                { }
        };

        template< class Graph, class Allocator, class ContAllocator > class LexVisitContainer
        {
          public:
            typedef LVCNode< Graph > Node;

            class Container: public Privates::List< Node,ContAllocator >
            {
              public:
                Container( ContAllocator &a): Privates::List< Node,ContAllocator >( a )
                { }
            };

            Container m_data;
            Privates::List_iterator< Node > m_openBlock;
            Privates::List< Privates::List_iterator< Node >,Allocator > m_splits;
            typename DefaultStructs::template
                AssocCont< typename Graph::PVertex,Privates::List_iterator< Node > >::Type m_vertexToPos;

            LexVisitContainer( Allocator& a, ContAllocator& ca, int n):
                m_data( ca ), m_openBlock(), m_splits( a ),  m_vertexToPos( n )
                { }

            ~LexVisitContainer()
                { clear(); }

            void clear();
            void initialize( const Graph &g );
            void initialize( const Graph &g, size_t n, typename Graph::PVertex *tab );
            void initializeAddAll( const Graph &g );
            void cleanup();
            bool empty();
            typename Graph::PVertex top();
            void pop();
            void push( typename Graph::PVertex v );
            void move( typename Graph::PVertex v );
            void remove( typename Graph::PVertex v )
                { m_data.erase( m_vertexToPos[v] ); }
            void done();
            void dump();
        };

        template< class GraphType > struct OrderData
        {
            typename GraphType::PVertex v;
            // kogo jest s¹siadem (numer s¹siada w porz¹dku)
            int vertId;
            // numer w porz¹dku
            int orderId;
        };

      public:
        /** order vertices with LexBFS order, starting with a given sequence
        * @param[in] g graph
        * @param[in] in number of vertices in table tab
        * @param[in] tab table containing initial order of vertices
        * @param[in] mask direction of edges to consider, LexBFS akceptuje tylko maski symetryczne
        * @param[out] out iterator to write ordered vertices
        * @return number of vertices written to out
        */
        template< class GraphType, class OutVertIter > static int order2( const GraphType & g, size_t in,
            typename GraphType::PVertex *tab, EdgeDirection mask, OutVertIter out );

        template< class T > static void StableRadixSort( T *data, int n, int nb, int T::*field, T *out );

        /** visit all vertices in the same component as a given vertex
        * @param[in] g graph containing vertices to visit
        * @param[in] src given vertex
        * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
                Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
        component jest ustawiane na component
        * @param[in] visitor visitor called for each vertex
        * @param[in] dir direction of edges to consider, uwaga: LexBFS akceptuje tylko maski symetryczne
        * @param[in] compid component identifier (give 0 if don't know)
        * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
        */
        template< class GraphType, class VertContainer, class Visitor > static int visitBase( const GraphType & g,
            typename GraphType::PVertex start, VertContainer &visited, Visitor visit, EdgeDirection mask,
                int component );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class LexBFS: public LexBFSPar< AlgsDefaultSettings > { };

    /*
     * Cheriyan/Mehlhorn/Gabow algorithm
     * Wyszukiwanie skladowych silnie spojnych grafu skierowanego
     */
    template< class DefaultStructs > class SCCPar: protected SearchStructs
    {
      protected:
        struct SCCData
        {
            int preIdx;
            bool assigned;

            SCCData( int p = 0, bool a = false ): preIdx( p ), assigned( a )
                { }
        };

        template< class GraphType, class CompIter, class VertIter, class CompMap > class SCCState
        {
          public:
            SCCState( CompStore< CompIter,VertIter > _i, CompMap &cm, typename GraphType::PVertex *buf1,
                typename GraphType::PVertex *buf2, int n ): vmap( n ), s( buf1,n ), p( buf2,n ), iters( _i ),
                 compMap( cm ), idx( 0 ), count( 0 ), c( 0 )
                    { }

            void addVert( typename GraphType::PVertex );
            void newComp();

            typename DefaultStructs::template AssocCont< typename GraphType::PVertex,SCCData >::Type vmap;
            StackInterface< typename GraphType::PVertex * > s,p;
            CompStore< CompIter,VertIter > iters;
            CompMap &compMap;
            unsigned idx;
            int count,c;
        };

        template< class GraphType, class CompIter, class VertIter, class CompMap > class SCCVisitor:
            public Visitors::complex_visitor_tag, public Visitors::no_component_visitor_tag
        {
          public:
            SCCVisitor( SCCState< GraphType,CompIter,VertIter,CompMap > & );

            bool visitVertexPre( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
            bool visitVertexPost( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
            bool visitEdgePre( const GraphType &, typename GraphType::PEdge, typename GraphType::PVertex );
            bool visitEdgePost( const GraphType &g, typename GraphType::PEdge e, typename GraphType::PVertex u )
                { return true; }

          private:
            SCCState< GraphType,CompIter,VertIter,CompMap > &state;
        };

      public:
        /** split graph into strongly connected components, uwzglednia krawedzie wszystkich typow
        * @param[in] g graph to split
        * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
        * @param[out] vtoc map (PVertex -> int indexOfItsComponent(zero based)), lub BlackHole
        * @return number of components
        */
        template< class GraphType, class CompIter, class VertIter, class CompMap > static int
            split( const GraphType &g, CompStore< CompIter,VertIter > out, CompMap & vtoc );

        //    Korzysta z mapy CompMap z poprzedniej procedury. Wyrzuca na iterator wartosci std::pair<int,int> - wszystkie
        //    pary numerow skladowych silnie spojnych, ktore sa polaczone choc jednym bezposrednim lukiem. Zwraca dlugos
        //    ciagu wynikowego
        template< class GraphType, class CompMap, class PairIter >
            static int connections(const GraphType &, CompMap &, PairIter );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class SCC: public SCCPar< AlgsDefaultSettings > { };

    /* DAGAlgsPar
     * Procedury na digrafach acykliczych
     */
    template< class DefaultStructs > class DAGAlgsPar: protected SearchStructs
    {
      public:
        // wyrzuca na iterator wierzcholki grafu w porzadku topologicznym
        template< class GraphType, class VertIter > static void topOrd( const GraphType &, VertIter );

        // sprawdza, czy graf jest DAGiem korzystajac z podanego para iteratorow ciagu wierzcholkow z wyjscia poprzedniej procedury
        template< class GraphType, class Iter > static bool isDAG( const GraphType &, Iter, Iter );

        // sprawdza, czy graf jest DAGiem, samodzielna
        template< class GraphType > static bool isDAG( const GraphType & );

        // wyrzuca na iterator wszystkie luki przechodnie DAGa, zwraca dlugosc ciagu
        template< class GraphType, class Iter > static int transEdges(const GraphType &, Iter);

        // usuwa luki przechodnie DAGa
        template< class GraphType > static void makeHasse( GraphType & );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class DAGAlgs: public DAGAlgsPar< AlgsDefaultSettings > { };

    /*
     * Blocks -- biconnected components
     * Wykrywanie skladowych 2-spojnych (blokow) grafu
    */
    template< class DefaultStructs > class BlocksPar: public SearchStructs
    {
      protected:
        template< class GraphType > struct BiConVData
        {
            unsigned depth;
            unsigned lowpoint;
            unsigned count;
            bool iscut;
            int visited;
            int sons;
            int vblFirst;
        };

        struct VertBlockList
        {
            int block;
            int next;
        };

        template< class GraphType, class CompIter, class VertIter, class EdgeMap > class BiConState
        {
          public:
            typedef typename GraphType::PVertex PVertex;

            BiConState( CompStore< CompIter,VertIter > _i, EdgeMap &em, EdgeDirection _m,
                std::pair< typename GraphType::PEdge *,int > st, VertBlockList *_vbl, int vno ):
                vmap( vno ), emap( st.second - 1 ), estk( st.first,st.second ), iters( _i ), outEMap( em ),
                vbl( _vbl ), vblAlloc( 0 ), idx( 0 ), count( 0 ), mask( _m )
                { }

            void addVert( typename GraphType::PVertex );
            void addEdge( typename GraphType::PEdge e )
                { if (!isBlackHole( outEMap )) outEMap[e] = count; }

            void newComp();

            typename DefaultStructs::template AssocCont< typename GraphType::PVertex,BiConVData< GraphType > >::Type
                vmap;
            typename DefaultStructs::template AssocCont< typename GraphType::PEdge,bool >::Type emap;
            StackInterface< typename GraphType::PEdge* > estk;
            CompStore< CompIter,VertIter > iters;
            EdgeMap &outEMap;
            VertBlockList *vbl;
            int vblAlloc;
            unsigned idx;
            int count;
            EdgeDirection mask;
        };

        template< class GraphType, class CompIter, class VertIter, class EdgeMap > class BiConVisitor:
            public Visitors::complex_visitor_tag, public Visitors::no_component_visitor_tag
        {
          public:
            BiConVisitor( BiConState< GraphType,CompIter,VertIter,EdgeMap > & );

            bool visitVertexPre( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
            bool visitVertexPost( const GraphType &, typename GraphType::PVertex, VisitVertLabs< GraphType > & );
            bool visitEdgePre( const GraphType &, typename GraphType::PEdge, typename GraphType::PVertex );
            bool visitEdgePost( const GraphType &, typename GraphType::PEdge, typename GraphType::PVertex );

            private:
                BiConState< GraphType,CompIter,VertIter,EdgeMap > &state;
        };

        template< class State, class VertMap, class VertBlockIter >
            static void storeBlocksData( State &, VertBlockList *, VertMap &, VertBlockIter & );

      public:
        // wynikowa etykieta wierzcholka
        struct VertData {
            // w ilu blokach lezy ten wierzcholek
            int blockNo;
            // pozycja pierwszego w sekwencji numerow blokow (por. viter nizej) bloku zawierajacego
            int firstBlock;
            // ten wierzcholek (jego pozostale bloki wystepuja kolejno za nim)
            VertData( int b = 0, int f = -1 ): blockNo( b ), firstBlock( f )
                { }

            template <class T> void copy(T& arg) const
            {
                arg.blockNo=blockNo;
                arg.firstBlock=firstBlock;
            }
            void copy(BlackHole&) const
                { }
        };

        /** split graph into blocks
        * @param[in] g graph to split, uwzglednia wszystkie krawedzie traktujac luki jak nieskierowane
        * @param[out] vmap mapa PVertex->VertData powiazana z ciagiem zapisanym na viter (lub BlackHole)
        * @param[out] emap mapa PEdge->int - w ktorym bloku znajduje sie dana krawedz (lub BlackHole)
        * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
            CompStore do ktorego zapisuje sie ciagi wierzcholkow z kolejnych blokow
        * @param[out] viter Iterator na ktory wypisywana jest ciag numerow blokow (byc moze z powtorzeniami) - por. VertData
        * @return number of biconnected components
        */
        template< class GraphType, class VertDataMap, class EdgeDataMap, class CompIter, class VertIter,
            class VertBlockIter > static int split( const GraphType &g, VertDataMap &, EdgeDataMap &,
                CompStore< CompIter,VertIter > out, VertBlockIter );

        /** split a component containing given vertex into blocks
        * @param[in] g graph to split, uwzglednia wszystkie krawedzie traktujac luki jak nieskierowane
        * @param[in] src given vertex
        * @param[out] vmap mapa PVertex->VertData powiazana z ciagiem zapisanym na viter (lub BlackHole)
        * @param[out] emap mapa PEdge->int - w ktorym bloku znajduje sie dana krawedz (lub BlackHole)
        * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
                CompStore do ktorego zapisuje sie ciagi wierzcholkow z kolejnych blokow
        * @param[out] viter Iterator na ktory wypisywana jest ciag numerow blokow (byc moze z powtorzeniami) - por. VertData
        * @return number of biconnected components in a component containing given vertex
        */
        template< class GraphType, class VertDataMap, class EdgeDataMap, class CompIter, class VertIter,
            class VertBlockIter > static int splitComp( const GraphType &g, typename GraphType::PVertex src,
            VertDataMap &vmap, EdgeDataMap &emap, CompStore< CompIter,VertIter > out, VertBlockIter viter );

        // wyrzuca na iterator ciag wierzcholkow tworzacych rdzen grafu tj. podgraf pozostajacy po sukcesywnym
        // usuwaniu wierzcholkow stopnia < 2. Zwraca dlugosc sekwencji
        // TODO: przejsc na kopiec
        template< class GraphType,class Iterator > static int getCore( const GraphType &g, Iterator out );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class Blocks: public BlocksPar< AlgsDefaultSettings > { };

    /*
    * Eulerian cycle and path
    */
    template< class DefaultStructs > class EulerPar: public PathStructs, protected SearchStructs
    {
      protected:
        template< class GraphType > struct EulerState
        {
            const GraphType &g;
            StackInterface< std::pair< typename GraphType::PVertex,typename GraphType::PEdge > *> stk;
            typename DefaultStructs::template AssocCont< typename GraphType::PEdge,bool >::Type edgeVisited;
            EdgeDirection mask;

            EulerState( const GraphType &_g, std::pair< typename GraphType::PVertex,typename GraphType::PEdge > *_stk,
                int nv, EdgeDirection m ): g( _g ), stk( _stk,nv ), edgeVisited( _g.getEdgeNo() ), mask( m )
                { }
        };

        template< class GraphType > static void eulerEngine( typename GraphType::PVertex u,
            typename GraphType::PEdge ed, EulerState< GraphType > &state );

        template< class GraphType, class VertIter, class EdgeIter > static void
            eulerResult( EulerState< GraphType > &state, OutPath< VertIter,EdgeIter > &out );

        template< class GraphType > static  void
            _ends( const GraphType &g, EdgeType mask, typename GraphType::PVertex &,typename GraphType::PVertex &);

      public:
        // Uwaga: wersje bez dir uwzgledniaja jedynie podgraf zlozony z petli i krawedzi nieskierowanych ignorujac reszte
        // Wersje z dir uwzgledniaja jedynie podgraf zlozony z petli i lukow ignorujac reszte
        // para zawierajaca 2 razy ten sam wierzcholek - jesli graf ma nieskierowany cykl Eulera
        // para zawierajaca 2 rozne wierzcholki - konce nieskierowanej sciezki Eulera - jesli ta istnieje
        // (NULL,NULL) w przciwnym razie
        template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
            ends( const GraphType &g )
            {
                std::pair< typename GraphType::PVertex, typename GraphType::PVertex > res;
                _ends( g,EdUndir | EdLoop, res.first,res.second );
                return res;
            }

        // para zawierajaca 2 razy ten sam wierzcholek - jesli graf ma skierowany cykl Eulera
        // para zawierajaca 2 rozne wierzcholki - konce skierowanej sciezki Eulera - jesli ta istnieje
        // (NULL,NULL) w przciwnym razie
        template< class GraphType > static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
            dirEnds( const GraphType &g )
            {
                std::pair< typename GraphType::PVertex, typename GraphType::PVertex > res;
                _ends( g,EdUndir | EdLoop, res.first,res.second );
                return res;
            }

        /** test if graph has an undirected Eulerian cycle
        * @param[in] g graph
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasCycle( const GraphType &g );

        /** test if graph has a directed Eulerian cycle
        * @param[in] g graph
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasDirCycle( const GraphType &g );

        /** test if graph has an undirected Eulerian path
        * @param[in] g graph
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasPath( const GraphType &g );

        /** test if graph has a directed Eulerian path
        * @param[in] g graph
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasDirPath( const GraphType &g );

        /** test if graph has an undirected Eulerian path starting at vertex u
        * @param[in] g graph
        * @param[in] u starting vertex
        * @param[in] mask type of edges to consider
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasPath( const GraphType &g, typename GraphType::PVertex u );

        /** test if graph has an directed Eulerian path starting at vertex u
        * @param[in] g graph
        * @param[in] u starting vertex
        * @param[in] mask type of edges to consider
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasDirPath( const GraphType &g, typename GraphType::PVertex u );

        /** test if graph has an undirected Eulerian cycle containing vertex u
        * @param[in] g graph
        * @param[in] u given vertex
        * @param[in] mask type of edges to consider
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasCycle( const GraphType &g, typename GraphType::PVertex u );

        /** test if graph has an directed Eulerian cycle containing vertex u
        * @param[in] g graph
        * @param[in] u given vertex
        * @param[in] mask type of edges to consider
        * @return true if it has, false otherwise */
        template< class GraphType > static bool hasDirCycle( const GraphType &g, typename GraphType::PVertex u );

        /** get undirected Eulerian cycle
        * @param[in] g graph
        * @param[out] out found cycle
        * @return true if graph has an Eulerian cycle, false otherwise */
        template< class GraphType, class VertIter, class EdgeIter >
            static bool getCycle( const GraphType &g, OutPath< VertIter,EdgeIter > out );

        /** get directed Eulerian cycle
        * @param[in] g graph
        * @param[out] out found cycle
        * @return true if graph has an Eulerian cycle, false otherwise */
        template< class GraphType, class VertIter, class EdgeIter >
            static bool getDirCycle( const GraphType &g, OutPath< VertIter,EdgeIter > out );

        /** get undirected Eulerian cycle
        * @param[in] g graph
        * @param[in] prefstart preferred starting vertex, ale jesli cykl E. nie
        przechodzi przez ten wierzcholek, preferencja bedzie zignorowana
        * @param[out] out found cucle
        * @return true if graph has an Eulerian cycle, false otherwise */
        template< class GraphType, class VertIter, class EdgeIter > static bool
            getCycle( const GraphType &g, typename GraphType::PVertex prefstart, OutPath< VertIter,EdgeIter> out );

        /** get directed Eulerian cycle
        * @param[in] g graph
        * @param[in] prefstart preferred starting vertex, ale jesli cykl E. nie
        przechodzi przez ten wierzcholek, preferencja bedzie zignorowana
        * @param[out] out found cucle
        * @return true if graph has an Eulerian cycle, false otherwise */
        template< class GraphType, class VertIter, class EdgeIter > static bool getDirCycle( const GraphType &g,
            typename GraphType::PVertex prefstart, OutPath< VertIter,EdgeIter > out);

        /** get undirected Eulerian path
        * @param[in] g graph
        * @param[out] out found path
        * @return true if graph has an Eulerian path, false otherwise */
        template< class GraphType, class VertIter, class EdgeIter >
            static bool getPath( const GraphType &g, OutPath< VertIter,EdgeIter > out );

        /** get directed Eulerian path
        * @param[in] g graph
        * @param[out] out found path
        * @return true if graph has an Eulerian path, false otherwise */
        template< class GraphType, class VertIter, class EdgeIter >
            static bool getDirPath( const GraphType &g, OutPath< VertIter,EdgeIter > out );

        /** get undirected Eulerian path
        * @param[in] g graph
        * @param[in] prefstart preferred starting vertex, ale jesli sciezka E. nie
        zaczyna sie od tego wierzcholka, preferencja bedzie zignorowana
        * @param[out] out found path
        * @param[in] mask type of edges to consider
        * @return true if graph has an Eulerian path, false otherwise */
        template< class GraphType, class VertIter, class EdgeIter > static bool getPath(
            const GraphType &g, typename GraphType::PVertex prefstart, OutPath<VertIter, EdgeIter> out );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class Euler: public EulerPar< AlgsDefaultSettings > { };

    // Typ najwyzszego wezla drzewa dekompozycji modulowej grafu
    // graf 1-wierzcholkowy
    // graf spojny o niespojnym dopelnieniu
    // graf niespojny
    // strong modules rozpinaja graf pierwszy
    enum ModPartType { mpTrivial,mpConnected,mpDisconnected,mpPrime };

    /* ModulesPar
     * Znajdowanie robicia grafow na maksymalne silne moduly
     */
    template< class DefaultStructs > class ModulesPar: public SearchStructs
    {
      public:
        // opis najwyzszego wezla drzewa dekompozycji modulowej grafu
        struct Partition
        {
            // size ile maksymalnych silnych modulow zawiera graf
            int size;
            // j.w.
            ModPartType type;

            Partition( int s, ModPartType t ): size( s ), type( t ) { }
        };

        // znajduje rozbicie grafu na maksymalne silne moduly
        // g, badany graf, powinien byc prosty, nieskierowany
        // out,  iteratory wyjsciowe z zawartoscia analogiczna jak w getComponents
        // tyle ze chodzi o rozbicie na moduly, a nie skladowe spojnosci
        // avmap, wyjsciowa tablica asocjacyjna PVertex->int do ktorej zapisuje sie numery modulow,
        // do ktorych naleza wierzcholki (lub BlackHole)
        // skipifprime, pomija badanie modulow jesli wynik ma type=mpPrime
        template< class GraphType, class CompIter, class VertIter, class CompMap > static Partition split(
            const GraphType &g, CompStore< CompIter,VertIter > out, CompMap &avmap, bool skipifprime = false );
    };

    // wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
    class Modules: public ModulesPar< AlgsDefaultSettings > { };

#include "search.hpp"
}

#endif
