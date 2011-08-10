#ifndef __COLORING__H__
#define __COLORING__H__

/* ------------------------------------------------------------------------- *
 * coloring.h
 *
 * Różne metody kolorowania grafów.
 * ------------------------------------------------------------------------- */

#include "../base/def_struct.h"
#include "../graph/subgraph.h"
#include "search.h"
#include <algorithm>
#include <cassert>

namespace Koala {

/* ------------------------------------------------------------------------- *
 * SeqVertColoring
 *
 * Zachłanne wierzchołkowe kolorowanie grafów.
 * ------------------------------------------------------------------------- */

class SeqVertColoring {
    protected:
        template< typename Pair > struct LfCmp
        {
            bool operator() ( Pair a, Pair b );
        } ;

        template< typename Pair > struct SlCmp
        {
            bool operator() ( Pair a, Pair b );
        } ;

        template< typename Triple > struct SlfCmp
        {
            bool operator() ( Triple a, Triple b );
        } ;

        template< typename Graph, typename ColorMap >
            static int satDeg(
                const Graph &g, ColorMap &c, typename Graph::PVertex v );

        template< typename Graph, typename ColorMap, typename CompMap >
            static int interchangeComponents(
                const Graph &g, ColorMap &c, typename Graph::PVertex v,
                CompMap &map, int c1, int c2 );
        template< typename Graph, typename ColorMap >
            static int colorInterchange(
                const Graph &g, ColorMap &c, typename Graph::PVertex v );

        template< typename Graph, typename ColorMap >
            static int maxColor( const Graph &g, ColorMap &c );

    public:
        // Zachłanne kolorowanie pojedynczego wierzchołka.
        template< typename Graph, typename ColorMap >
            static int greedy(
                const Graph &g, ColorMap &c, typename Graph::PVertex v );

        // Zachłanne kolorowanie pojedynczego wierzchołka, z zamianą kolorów, jeśli
        // uzyskany kolor przekracza podany limit.
        template< typename Graph, typename ColorMap >
            static int greedyInter(
                const Graph &g, ColorMap &c, typename Graph::PVertex v, int l );

        // Kolorowanie podanego zakresu wierzchołków.
        template< typename Graph, typename ColorMap, typename Iterator >
            static int greedy(
                const Graph &g, ColorMap &c, Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków (z zamianą kolorów).
        template< typename Graph, typename ColorMap, typename Iterator >
            static int greedyInter(
                const Graph &g, ColorMap &c, Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków (z zamianą kolorów).
        template< typename Graph, typename ColorMap, typename Iterator >
            static int greedyInter(
                const Graph &g, ColorMap &c, Iterator b, Iterator e, int l );

        // Kolorowanie całego grafu.
        template< typename Graph, typename ColorMap >
            static int greedy( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int greedyInter( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int greedyInter( const Graph &g, ColorMap &c, int l );

        // Kolorowanie całego grafu, metoda Largest First.
        template< typename Graph, typename ColorMap >
            static int lf( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu, metoda Largest First (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int lfInter( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu, metoda Largest First (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int lfInter( const Graph &g, ColorMap &c, int l );

        // Kolorowanie podanego zakresu wierzchołków, metoda Largest First.
        template < typename Graph, typename ColorMap, typename Iterator >
            static int lf( const Graph &g, ColorMap &c , Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków, metoda Largest First
        // (z zamianą kolorów).
        template < typename Graph, typename ColorMap, typename Iterator >
            static int lfInter(
                const Graph &g, ColorMap &c , Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków, metoda Largest First
        // (z zamianą kolorów).
        template < typename Graph, typename ColorMap, typename Iterator >
            static int lfInter(
                const Graph &g, ColorMap &c , Iterator b, Iterator e, int l );

        // Kolorowanie całego grafu, metoda Smallest Last.
        template< typename Graph, typename ColorMap >
            static int sl( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu, metoda Smallest Last (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int slInter( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu, metoda Smallest Last (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int slInter( const Graph &g, ColorMap &c, int l );

        // Kolorowanie podanego zakresu wierzchołków, metoda Smallest Last.
        template < typename Graph, typename ColorMap, typename Iterator >
            static int sl( const Graph &g, ColorMap &c , Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków, metoda Smallest Last
        // (z zamianą kolorów).
        template < typename Graph, typename ColorMap, typename Iterator >
            static int slInter(
                const Graph &g, ColorMap &c , Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków, metoda Smallest Last
        // (z zamianą kolorów).
        template < typename Graph, typename ColorMap, typename Iterator >
            static int slInter(
                const Graph &g, ColorMap &c , Iterator b, Iterator e, int l );

        // Kolorowanie całego grafu, metoda Saturation Largest First.
        template< typename Graph, typename ColorMap >
            static int slf( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu, metoda Saturation Largest First
        // (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int slfInter( const Graph &g, ColorMap &c );

        // Kolorowanie całego grafu, metoda Saturation Largest First
        // (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
            static int slfInter( const Graph &g, ColorMap &c, int l );

        // Kolorowanie podanego zakresu wierzchołków, metoda Saturation Largest First.
        template < typename Graph, typename ColorMap, typename Iterator >
            static int slf( const Graph &g, ColorMap &c , Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków, metoda Saturation Largest First
        // (z zamianą kolorów).
        template < typename Graph, typename ColorMap, typename Iterator >
            static int slfInter(
                const Graph &g, ColorMap &c , Iterator b, Iterator e );

        // Kolorowanie podanego zakresu wierzchołków, metoda Saturation Largest First
        // (z zamianą kolorów).
        template < typename Graph, typename ColorMap, typename Iterator >
            static int slfInter(
                const Graph &g, ColorMap &c , Iterator b, Iterator e, int l );
} ;

#include "coloring.hpp"

}

#endif
