#ifndef DEF_MIS_H
#define DEF_MIS_H

#include "../base/defs.h"
#include "../graph/graph.h"
#include "search.h"


namespace Koala
{

    /*
    * MaxIndepSetFunctions namespace contains functors
    * that each of chooses a vertex based on some specific rules.
    * Such rules could be: first vertex, random vertex, vertex that meets
    * the specific requirements (has the largest degree for example).
    * These functions are used within WMIN and WMAX templates
    * to choose one vertex in each algorithm step.
    */
    namespace MISStrategy
    {

        /* ----------------------------------------------------------------------
        *
        * Template:        N/A
        * Choice function: GetFirst
        * Function:        v[0]
        *
        */
        class First
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab ) { return g.getVert(); }
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        N/A
        * Choice function: RandVertex
        * Function:        v[?]
        *
        */
        class Rand
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        WMIN
        * Choice function: GMIN
        * Function:        1/( deg(v) + 1 )
        *
        * Notes:           Does not require vertices weights.
        *
        */
        class GMin
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        WMIN
        * Choice function: GWMIN
        * Function:        W(v)/( deg(v) + 1 )
        *
        */
        class GWMin
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        WMIN
        * Choice function: GGWMIN
        * Function:        All vertices in I satisfy:
        *                  SUM of all u in N[v]  W(u)/(deg(u) + 1) <= W(v)
        *
        */
        class GGWMin
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        WMIN
        * Choice function: GWMIN2
        * Function:        W(v) / ( SUM of all u in N[v]  W(u) )
        *
        */
        class GWMin2
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        WMAX
        * Choice function: GMAX
        * Function:        1/( deg(v) * (deg(v) + 1) )
        *
        * Notes:           Does not require vertices weights.
        *
        */
        class GMax
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        WMAX
        * Choice function: GWMAX
        * Function:        W(v)/( deg(v) * (deg(v) + 1) )
        *
        */
        class GWMax
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };

        /* ----------------------------------------------------------------------
        *
        * Template:        WMAX
        * Choice function: GGWMAX
        * Function:        All vertices in I satisfy:
        *                  SUM of all u in N[v]  W(u) /( deg(u) * (deg(u) + 1) )  >=  W(v) /( deg(v) + 1 )
        *
        */
        class GGWMax
        {
          public:
            template< class GraphType, class VertContainer > typename GraphType::PVertex
                operator()( const GraphType &g, const VertContainer& vertTab );
        };
    }

    /*
    * Class for max independent set.
    *
    * Contains methods for two main templates: WMIN and WMAX.
    * It has to be initialized by a class containing a vertex
    * choice function called "choose".
    *
    */
    template< class DefaultStructs > class MISHeuristicPar
    {
      public:
        /*
        * Calculate max independent set using WMIN template.
        */
        template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
            static unsigned getWMin( const GraphType &g, OutputIterator out, ChoiceFunction choose,
                const VertContainer & vertTab );

        /*
        * Calculate max independent set using WMAX template.
        */
        template< class GraphType, class OutputIterator, class ChoiceFunction, class VertContainer >
            static unsigned getWMax( const GraphType &g, OutputIterator out, ChoiceFunction choose,
                const VertContainer &vertTab );

      private:
        /*
        * Template:    WMIN
        *
        * In general in each step it (until the graph has no more vertices):
        *   - chooses a vertex according to the choice function
        *   - adds this vertex to the independent set
        *   - removes the closed neighbourhood of this vertex
        * After all it outputs the independent set which is maximal.
        *
        */
        template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
            static unsigned TemplateWMIN( GraphType &g, OutputIterator out, ChoiceFunction choose,
                const VertContainer &vertTab );

        /*
        * Template:    WMAX
        *
        * In general in each step it (until the graph has no more edges):
        *   - chooses a vertex according to the choice function
        *   - removes the choosen vertex with adjacent edges
        * All vertices not removed are outputed and this is an independent set wich is maximal.
        *
        */
        template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
            static unsigned TemplateWMAX( GraphType &g, OutputIterator out, ChoiceFunction choose,
                const VertContainer &vertTab );
    };

    /*
    * Max independent set.
    */
    class MISHeuristic: public MISHeuristicPar< Koala::AlgsDefaultSettings > {};

    /*
    * Class for maximum independent set - exact algorithm.
    *
    * Based on:
    * F. V. Fomin, F. Grandoni i D. Kratsch.
    * Measure & conquer: A simple O(2^0.288n) independent set algorithm.
    * ACM-SIAM Symposium on Discrete Algorithms (SODA), 18–25, 2006.
    *
    */
    template< class DefaultStructs > class MISPar
    {
      public:
        /*
        * Calculate maximum independent set.
        *
        * Input:
        *   g    - graph to process
        *   out  - iterator to the output independent set
        *
        * Output:
        *   number of vertices in the maximum independent set
        *
        */
        template< class GraphType, class OutputIterator > static unsigned
            get( GraphType & g, OutputIterator out );

        /*
        * Determinate if a set of vertices is independent.
        *
        * Input:
        *   g     - graph to process
        *   first - first vertex from the potentially independent set
        *   last  - last vertex from the potentially independent set
        *
        * Output:
        *   bool - true is the given set is independent, false otherwise
        *
        */
        template< class GraphType, typename Iterator >
            static bool isStable( const GraphType &g, Iterator first, Iterator last );

        /*
        * Determinate if a set of vertices is maximal independent.
        *
        * Input:
        *   g     - graph to process
        *   first - first vertex from the potentially independent set
        *   last  - last vertex from the potentially independent set
        *
        * Output:
        *   bool - true is the given set is maximal independent, false otherwise
        *
        */
        template< class GraphType, typename Iterator >
            static bool isMaxStable( const GraphType &g, Iterator first, Iterator last, bool stabilitytest=true );

      private:

        /*
        * Maximum independent set - inner, recursive.
        */
        template< class GraphType, class OutputIterator > static unsigned
            __getMaximumIndependentSet( GraphType &g, OutputIterator out, bool isConnectedComponent );

        template< class GraphType > static bool isDominated( const GraphType &g,
            typename GraphType::PVertex v, typename GraphType::PVertex w );
        template< class GraphType > static Koala::Set< typename GraphType::PVertex >
            getMirrors( const GraphType &g, typename GraphType::PVertex v );
        template< class GraphType > static bool isFoldable( const GraphType &g, typename GraphType::PVertex v );
        template< class GraphType > static bool isClique( const GraphType &g, const Set< typename GraphType::PVertex > &verts );
        template< class GraphType > static Set< typename GraphType::PVertex >
            getSecondNeighSet( const GraphType &g, typename GraphType::PVertex vert );
    };

    class MIS: public MISPar< Koala::AlgsDefaultSettings > {};

#include "mis.hpp"
}

#endif
