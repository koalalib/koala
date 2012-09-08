#ifndef KOALA_DEF_FACTOR_H
#define KOALA_DEF_FACTOR_H

#include "../base/defs.h"
#include "../graph/view.h"

namespace Koala
{
    /* MatchingPar
     * algorytm szukajacy najliczniejszego skojarzenia w grafie dowolnym
     */
    template< class DefaultStructs > class MatchingPar
    {
      public:
        template< class GraphType > struct VertLabs
        {
                friend class MatchingPar<DefaultStructs>;
                //wierzcholek skojarzany
                typename GraphType::PVertex vMatch;
                //i krawedz skojarzenia
                typename GraphType::PEdge eMatch;

                VertLabs(): vMatch( 0 ), eMatch( 0 ), vLabS( 0 ), vLabB( 0 ), bS( false ), bT( false )
                    { vLabT[0] = vLabT[1] = 0; }

            protected:
                // parametry do uzytku wewnetrznego algorytmu
                //cecha T (VertLabs[Vi].T=Vj oznacza, ze istnieje parzysta droga naprzemienna z wierzcholka wolnego Vr do Vi, gdzie poprzednikiem Vi jest Vj)
                typename GraphType::PVertex vLabT[2];
                //cecha S (VertLabs[Vi].S=Vj -||- nieparzysta droga -||-)
                typename GraphType::PVertex vLabS;
                //cecha b (VertLabs[Vi].b=Vj oznacza, ze najbardziej zewnetrzny kielich, do ktorego nalezy wierzcholek Vi jest reprezentowany przez wierzcholek Vj, ktory jest podstawa tego kielicha)
                typename GraphType::PVertex vLabB;
                //jezeli S niezbadana true, jezeli nie ma S lub S zbadana false (moze powinnam to trzymac na liscie cech?)
                bool bS;
                //jezeli T niezbadana true, jezeli nie ma T lub T zbadana false (moze to powinnam trzymac na liscie cech?)
                bool bT;

        };

      protected:
        //procedura wypelnia vertTabNeights - kazdemu wierzcholkowi przybisuje zbior jego sasiadow
        template< class GraphType, class AssocTab >
            static void fillVertTabNeights( const GraphType &g, AssocTab &vertTabNeights );
        //skojarzenie poczatkowe
        template< class GraphType, class VertContainer, class AssocTab > static int firstMatching( const GraphType &g,
            VertContainer &vertTab, AssocTab &vertTabNeights, int matchSize );
        //utworzenie skojarzenia poczatkowego ze podanego na wejsciu strumienia wierzcholkow tworzacych skojarzone krawedzie
        template< class GraphType, class VertContainer, class EIterIn > static int firstMaching( const GraphType &g,
            VertContainer &vertTab, EIterIn begin, EIterIn end );
        //funcja czysci wszystkie etykiety na wierzcholkach
        template< class GraphType, class VertContainer > static void clearLabels( const GraphType &g,
            VertContainer &vertTab );
        //funkcja realizuje powiekszenie skojarzenia, gdy zostala znaleziona naprzeminna droga powiekszajaca
        template< class GraphType, class VertContainer >  static int augmentation( const GraphType &g,
            VertContainer &vertTab, typename GraphType::PVertex tabPath1[], int len1,
            typename GraphType::PVertex tabPath2[], int len2, int expo );
        //reverse table
        template< class PVertex > static void reverseTable( PVertex tabPath[], int from, int to );
        //funkcja, ktora zwraca korzen oraz zapisuje do tabPath liste wierzcholkow na drodze od vCurr do Root
        // Parametry: graf, wierzcholki, tablica do ktorej zapisujemy kolejne wierzcholki na sciezce do korzenia,
        // pozycja w tablicy tabPath do ktorej teraz bedziemy zapisywac kolejny wierzcholek,
        //od tego wierzcholka zaczynamy wedrowke, jezeli true - zaczynamy od cechy S w vStart, jezeli false to od cechy T
        //warunek stopu - gdy dotrzemy do wierzcholka vStop konczymy wedrowke po wierzcholkach w danym kierunku,jezeli vStop==0 to konczymy gdy dotrzemy do korzenia
        template< class GraphType, class VertContainer > static typename GraphType::PVertex backtracking(
            const GraphType &g, VertContainer& vertTab, typename GraphType::PVertex tabPath[], int &iCurr,
            typename GraphType::PVertex vStart, bool isS, typename GraphType::PVertex vStop );
        //obrobka znalezionego kielicha
        template< class GraphType, class VertContainer > static void blossoming( const GraphType &g,
            VertContainer &vertTab, typename GraphType::PVertex tabPath1[], int len1,
            typename GraphType::PVertex tabPath2[], int len2 );
        //etykietowanie kolejnych wierzcholkow
        template< class GraphType, class VertContainer, class AssocTab > static int labeling( const GraphType &g,
            VertContainer &vertTab, AssocTab &vertTabNeights, bool isS, typename GraphType::PVertex vCurr, int expo );
        //algorytm Edmondsa
        template< class GraphType, class VertContainer, class AssocTab > static int edmonsAlg( const GraphType &g,
            VertContainer &vertTab, AssocTab &vertTabNeights, int matchSize, int &expo );
        //wpisanie wyniku do edgeIterOut dla maxMatching and minEdgeCover, zwraca liczbe wpisan
        //jezeli ifEdgeCover ustawiony (true) to rozszerzemy skojarzenie do pokrycia krawedziowego
        template< class GraphType, class VertContainer, class EIterOut > static int printResultToIterators(
            const GraphType &g, VertContainer &vertTab, EIterOut edgeIterOut, bool ifEdgeCover = false );

      public:
        // wlasciwa procedura - szuka skojarzenia o zadanym rozmiarze matchSize, zwraca znaleziony rozmiar skojarzenia
        // Parametry: graf, wyjsciowa tablica asocjacyjna PVertex->VertLabs, w plach vMatch tych rekordow jest wierzcholek skojarzony z danym lub 0 w razie braku (lub BlackHole)
        //iterator wyjsciowy, na ktory wyrzucamy krawedzie matchingu
        //Do ilu krawedzi wlacznie szukac. Tj. ilo-krawedziowy matching znalezc lub najwiekszy, gdy jest mniejszy od tego parametru. Gdy matchSize=-1 to znajdz najwiekszy.
        template< class GraphType, class VertContainer, class EIterOut > static int findMax( const GraphType &g,
            VertContainer &avertTab, EIterOut edgeIterOut, int matchSize = -1 );
        // wlasciwa procedura - szuka skojarzenia o zadanym rozmiarze matchSize, zaczynajac od podanego skojarzenia poczatkowego, zwraca znaleziony rozmiar skojarzenia
        // vertIterInBegin iterator wejsciowy, zawiera poczatek strumienia z wierzcholkami tworzacymi poczatkowy matching
        // edgeIterOut iterator wyjsciowy, na ktory wyrzucamy krawedzie matchingu
        // edgeIterOut, iterator wyjsciowy, na ktory wyrzucamy krawedzie matchingu
        template< class GraphType, class VertContainer, class EIterIn, class EIterOut > static int findMax(
            const GraphType &g, VertContainer &avertTab, EIterIn vertIterInBegin, EIterIn vertIterInEnd,
            EIterOut edgeIterOut, int matchSize = -1 );
        // wlasciwa procedura - szuka minimalnego pokrycia krawedziowego
        // najpierw realizuje Edmondsa - szuka maksymalnego skojarzenia, nastepnie rozszerza je do pokrycia krawedziowego
        template< class GraphType, class VertContainer, class EIterOut > static int minEdgeCover( const GraphType &g,
            VertContainer &avertTab, EIterOut edgeIterOut );
        // wlasciwa procedura - zachlannie szuka skojarzenia
        template< class GraphType, class VertContainer, class EIterOut > static int greedy( const GraphType &g,
            VertContainer &avertTab, EIterOut edgeIterOut, int matchSize = -1 );
        // wlasciwa procedura - zachlannie szuka skojarzenia, rozwarzajac krawedzie podane na wejsciu
        template< class GraphType, class VertContainer, class EIterIn, class EIterOut > static int greedy(
            const GraphType &g, VertContainer &avertTab, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd,
            EIterOut edgeIterOut, int matchSize = -1 );
        // wlasciwa procedura - testuje czy podane na wejsciu krawedzie tworza skojarzenie
        template< class GraphType, class EIterIn > static bool test( const GraphType &g, EIterIn edgeIterInBegin,
            EIterIn edgeIterInEnd );
    };

    /* Matching
     *
     */
    class Matching: public MatchingPar< AlgsDefaultSettings > { };

    /* StableMatchingPar
     * algorytmy dla problemu stabilnego skojarzenia
     */
    template< class DefaultStructs > class StableMatchingPar
    {
      protected:
        template< class Graph, class Cmp > struct SortCmp
        {
            mutable typename Graph::PVertex v;
            mutable Cmp comp;

            SortCmp( typename Graph::PVertex av, Cmp acmp ): v( av ), comp( acmp )
                { }
            template< class T > bool operator()( T e,T f ) const
                { return !comp( v,e,f ); }
        };

      public:
        template< class GraphType > struct VertLabs
        {
            //wierzcholek skojarzany lub 0 w razie braku
            typename GraphType::PVertex vMatch;
            // i krawedz skojarzenia
            typename GraphType::PEdge eMatch;

            VertLabs( typename GraphType::PVertex v = 0, typename GraphType::PEdge e = 0 ): vMatch( v ), eMatch( e )
                { }
        };

        // wlasciwa procedura - testuje czy podane na wejsciu krawedzie tworza skojarzenie stabilne
        // wynik.first- odpowiedz. Jesli jest to matching ale istnieje krawedz rozbijajaca, zostaje ona zwrocona w .second
        // compare, funktor podajacy preferencje wierzcholkow odnosnie ich krawedzi, wolanie
        // bool compare(v,e1,e2) jest true jesli e2 jest lepsze od e1 z punktu widzenia ich koncowki v
        template< class GraphType, class EIterIn, class Comp > static std::pair< bool,typename GraphType::PEdge >
            test( const GraphType &g, Comp compare, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd );
        template< class GraphType, class VIterIn, class Comp, class vertCont, class EIterOut >
            static int bipartFind( const GraphType &g, VIterIn begin, VIterIn end, Comp compare, vertCont &verttab,
                EIterOut out );
    };

    /* StableMatching
     *
     */
    class StableMatching: public StableMatchingPar< AlgsDefaultSettings > { };

#include "factor.hpp"
}

#endif
