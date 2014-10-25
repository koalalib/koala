#ifndef DEF_MIS_H
#define DEF_MIS_H

#include "../base/defs.h"
#include "../base/rnd.h"
#include "../graph/graph.h"
#include "search.h"

namespace Koala
{

    //NEW: generalna zmiana nazw: MIS(Heuristic)(Par) -> MaxStable(Heur)(Par) oraz MISStrategy->MaxStableStrategy
	/** \brief Vertex choosing strategies (for maximal independent set) .
	 *
	 *  The namespace contains functors
	 *  that chooses a vertex basing on some specific rules.
	 *  Such rules could be: first vertex, random vertex, vertex that meets
	 *  the specific requirements (has the largest degree for example).
	 *  These functions are used within \a getWMin and \a getWMax methods in Koala::MaxStableHeurPar
	 *  to choose one vertex in each algorithm step.
	 *
	 *  Each object function overload two parameter call function operator that takes \a g the considered graph and associative container \a vertTab, which assigns weight to each vertex. The functor returns one chosen vertex.
	 *  \ingroup DMmis */
	namespace MaxStableStrategy
	{
	    namespace Privates {
            struct WMin_Strategy_tag {};
            struct WMax_Strategy_tag {};
            struct Strategy_tag : public WMin_Strategy_tag, WMax_Strategy_tag{};
	    }

		/* ----------------------------------------------------------------------
		*
		* Template:        N/A
		* Choice function: GetFirst
		* Function:        v[0]
		*
		*/
		/** \brief Get first vertex functor.
		 *
		 *  The for a graph functor returns always the first vertex on vertex list. Functor can be used in both approaches.*/
		class First : public Privates::Strategy_tag
		{
		public:
			/* \brief Call function operator.
			 *
			 *  \param g the copy of considered graph with pointer of original vertices in info field.
			 *  \param vertTab the associative table that assigns weights to vertices.*/
			template< class GraphType, class VertContainer > typename GraphType::PVertex
				operator()( const GraphType &g, const VertContainer& vertTab )
				{
					return g.getVert();
				}
		};

//NEW: w ponizszych stragegiach Rand GMin GWMin GGWMin GWMin2 GMax GWMax GGWMax nowy parametr
//szablonu klasy - typ wybranego generatora liczb losowych. Zewnetrzny obiekt uzywanego generatora
// podaje sie w konstruktorze strategii
		/* ----------------------------------------------------------------------
		*
		* Template:        N/A
		* Choice function: RandVertex
		* Function:        v[?]
		*
		*/
		/** \brief Get random vertex functor.
		 *
		 *  The for a graph functor returns random vertex.  Functor can be used in both approaches.*/
		template <class RndGen=Koala::StdRandGen<> >
		class Rand : public Privates::Strategy_tag
		{
		public:
		    RndGen* rgen;
		    Rand(RndGen& rg) : rgen(&rg) {}

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
		/** \brief Get minimum degree vertex functor.
		 *
		 *  The for a graph (weights are ignored) functor returns vertex with minimal degree. It is advised to use with method \a getWMin.*/
		template <class RndGen=Koala::StdRandGen<> >
		class GMin : public Privates::WMin_Strategy_tag
		{
		public:
		    RndGen* rgen;
		    GMin(RndGen& rg) : rgen(&rg) {}

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
		/** \brief Get minimum degree and maximum weight vertex functor.
		*
		*  The for a graph and weights functor returns vertex for which W(v)/( deg(v) +1) is maximal. It is advised to use with method \a getWMin.*/
		template <class RndGen=Koala::StdRandGen<> >
		class GWMin: public Privates::WMin_Strategy_tag
		{
		public:
		    RndGen* rgen;
		    GWMin(RndGen& rg) : rgen(&rg) {}

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
		/** \brief Get minimum degree vertex with weight functor.
		 *
		 *  The for a graph and weights functor returns vertices \a v for which Σ<sub>u ∈ N[v]</sub>W(u)/( deg(u) +1) ≤ W(v) . It is advised to use with method \a getWMin.*/
		template <class RndGen=Koala::StdRandGen<> >
		class GGWMin : public Privates::WMin_Strategy_tag
		{
		public:
		    RndGen* rgen;
		    GGWMin(RndGen& rg) : rgen(&rg) {}

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
		/** \brief Get maximum weight and minimum sum of neighbors weights vertex functor.
		 *
		 *  The for a graph and weights functor returns vertex for which W(v)/Σ<sub>u ∈ N[v]</sub>W(u) is maximal. It is advised to use with method \a getWMin.*/
		template <class RndGen=Koala::StdRandGen<> >
		class GWMin2 : public Privates::WMin_Strategy_tag
		{
		public:
		    RndGen* rgen;
		    GWMin2(RndGen& rg) : rgen(&rg) {}

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
		/** \brief Get maximum degree vertex functor.
		 *
		 *  The for a graph gets the vertex with maximum degree. It is advised to use this functor with function \a getWMax.*/
		template <class RndGen=Koala::StdRandGen<> >
		class GMax : public Privates::WMax_Strategy_tag
		{
		public:
		    RndGen* rgen;
		    GMax(RndGen& rg) : rgen(&rg) {}

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
		/** \brief Get maximum degree and minimal weight vertex functor.
		 *
		 *  The for a graph gets the vertex for which the function W(v) / (deg(v)*(deg(v)-1)) is minimal. It is advised to use this functor with function \a getWMax.*/
		template <class RndGen=Koala::StdRandGen<> >
		class GWMax: public Privates::WMax_Strategy_tag
		{
		public:
		    RndGen* rgen;
		    GWMax(RndGen& rg) : rgen(&rg) {}

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
		/** \brief Get maximum degree and minimal weight vertex functor.
		 *
		 *  The for a graph and weights gets the vertices v that satisfy  Σ<sub>u ∈ N[v]</sub> W(u) / (deg(u)*(deg(u)-1)) ≥ W(v) /( deg(v) + 1 ). It is advised to use this functor with function \a getWMax.*/
		template <class RndGen=Koala::StdRandGen<> >
		class GGWMax: public Privates::WMax_Strategy_tag
		{
		public:
		    RndGen* rgen;
		    GGWMax(RndGen& rg) : rgen(&rg) {}

			template< class GraphType, class VertContainer > typename GraphType::PVertex
				operator()( const GraphType &g, const VertContainer& vertTab );
		};
	}

	/** \brief Maximal independent set heuristics (parametrized).
	 *
	 *  Class for max independent set.
	 *
	 *  Contains methods for two main templates: WMIN and WMAX.
	 *  It has to be initialized by a class containing a vertex
	 *  choice function called "choose".
	 *
	 *  \ingroup DMmis */
	template< class DefaultStructs > class MaxStableHeurPar
	{
	public:
		/** \brief Search maximal independent set (WMin technique).
		 *
		 *  The method searches for maximal independent set using the following heuristic,
		 *  In each step (until the graph has no more vertices):
		 *   - chooses a vertex according to the choice function (Koala::MaxStableStrategy),
		 *   - adds this vertex to the independent set,
		 *   - removes the closed neighborhood of this vertex.
		 *
		 *  The method outputs the independent set which is maximum in the sense of inclusion.\n
		 *  \param g the considered graph.
		 *  \param out the iterator to the container with the output set of vertices.
		 *  \param choose the strategy (\ref Koala::MaxStableStrategy) of choosing vertices (one in each step) .
		 *  \param vertTab the associative container that assigns weight to each vertex.
		 *  \return the number of vertices in the output set \a out.
		 *  \n
		 *
		 *  [See example](examples/mis/example_mis_getWMin.html).
		 */
		template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
			static unsigned getWMin( const GraphType &g, OutputIterator out, ChoiceFunction choose,
				const VertContainer & vertTab );

		/** \brief Search maximal independent set (WMax technique).
		 *
		 *  The method searches for maximal independent set using the following heuristic,
		 *  In each step (until the graph has no more edges):
		 *   - chooses a vertex according to the choice function (\ref Koala::MaxStableStrategy) ,
		 *   - removes the chosen vertex with adjacent edges.
		 *
		 *  The method outputs the remaining independent vertices.
		 *  \param g the considered graph.
		 *  \param out the iterator to the container with the output set of vertices.
		 *  \param choose the strategy (\ref Koala::MaxStableStrategy)  of choosing vertices (one in each step).
		 *  \param vertTab the associative container that assigns weight to each vertex.
		 *  \return the number of vertices in the output set \a out.
		 *  \n
		 *
		 *  [See example](examples/mis/example_mis_getWMax.html).
		 */
		template< class GraphType, class OutputIterator, class ChoiceFunction, class VertContainer >
			static unsigned getWMax( const GraphType &g, OutputIterator out, ChoiceFunction choose,
				const VertContainer &vertTab );

        //NEW: zmiana nazwy metody, bylo isStable
		/** \brief Test if stable
		 *
		 * Determinate if a set of vertices is independent.
		 *  \param g     - graph to process
		 *  \param first - first vertex from the potentially independent set
		 *  \param last  - last vertex from the potentially independent set
		 * \return true is the given set is independent, false otherwise. */
		template< class GraphType, typename Iterator >
			static bool test( const GraphType &g, Iterator first, Iterator last );

		//NEW: zmiana nazwy metody, bylo isMaxStable
		/**\brief Test if max stable.
		 *
		 *Determinate if a set of vertices is maximal (in the sense of inclusion) independent.
		 * \param  g     - graph to process
		 * \param first - first vertex from the potentially independent set
		 * \param last  - last vertex from the potentially independent set
		 * \retrun true is the given set is maximal (in the sense of inclusion) independent, false otherwise.*/
		template< class GraphType, typename Iterator >
			static bool testMax( const GraphType &g, Iterator first, Iterator last, bool stabilitytest=true );

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
	/** \brief Maximal independent set heuristics (default).
	 *
	 *  Class for max independent set.
	 *
	 *  Contains methods for two main templates: WMIN and WMAX.
	 *  It has to be initialized by a class containing a vertex
	 *  choice function called "choose".
	 *
	 *  \ingroup DMmis */
	class MaxStableHeur: public MaxStableHeurPar< Koala::AlgsDefaultSettings > {};

    /*
    * Class for maximum independent set - exact algorithm.
    *
    * Based on:
    * F. V. Fomin, F. Grandoni i D. Kratsch.
    * Measure & conquer: A simple O(2^0.288n) independent set algorithm.
    * ACM-SIAM Symposium on Discrete Algorithms (SODA), 18–25, 2006.
    *
    */

    class LocalGrAdjMatrSettings : public AlgsDefaultSettings
	{
	public:

		template< class A, class B, EdgeType mask> class LocalGraph
		{
		public:
			typedef Graph< A,B,GrDefaultSettings< mask,true > > Type;
		};

	};

    //NEW:
    template< class DefaultStructs > class MaxStablePar : private MaxStableHeurPar<DefaultStructs>
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
        //NEW: znajduje najliczniejszy zbior niezalezny (procedura niewielomianowa). Jesli w trakcie szukania
        //wykryje, ze rozmiar minSize jest nieosioagalny, przerywa zwracajac -1
        template< class GraphType, class OutputIterator > static int
            get( GraphType & g, OutputIterator out, int minSize = 0);

        //NEW: znajduje zbior niezalezny mocy >= minSize (procedura niewielomianowa). Jesli w trakcie szukania
        //wykryje, ze rozmiar minSize jest nieosioagalny, przerywa zwracajac -1
        template< class GraphType, class OutputIterator > static int
            getSome( GraphType & g, OutputIterator out, int minSize);

            //NEW: testy na zb. niezalezny dziedziczone z MaxStableHeurPar
            using MaxStableHeurPar<DefaultStructs>::test;
            using MaxStableHeurPar<DefaultStructs>::testMax;

      private:
        /*
        * Maximum independent set - inner, recursive.
        */
        template< class GraphType, class OutputIterator > static int
            get( GraphType & g, OutputIterator out, int minSize, bool skipsearchiffound);
        template< class GraphType, class OutputIterator > static int
            getRecursive( GraphType &g, OutputIterator out, bool isConnectedComponent, bool outblackhole, int minSize, bool skipsearchiffound );
		template< class GraphType, class OutputIterator > static int
			getMirrors( const GraphType & g, typename GraphType::PVertex v, OutputIterator out);
        template< class GraphType, class InputIterator > static bool
			isClique( const GraphType &g, InputIterator beg, InputIterator end );
        template< class GraphType > static bool
			isDominated( const GraphType &g, typename GraphType::PVertex u, typename GraphType::PVertex v );
        template< class GraphType > static bool
			isFoldable( const GraphType &g, typename GraphType::PVertex v );
    };

    class MaxStable: public MaxStablePar< Koala::LocalGrAdjMatrSettings > {};
    //class MaxStable: public MaxStablePar< Koala::AlgsDefaultSettings > {};


    template< class DefaultStructs > class MaxCliqueHeurPar
	{
	public:

		template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
			static unsigned getWMin( const GraphType &g, OutputIterator out, ChoiceFunction choose,
				const VertContainer & vertTab );

		template< class GraphType, class OutputIterator, class ChoiceFunction, class VertContainer >
			static unsigned getWMax( const GraphType &g, OutputIterator out, ChoiceFunction choose,
				const VertContainer &vertTab );

		template< class GraphType, typename Iterator >
			static bool test( const GraphType &g, Iterator first, Iterator last );

		template< class GraphType, typename Iterator >
			static bool testMax( const GraphType &g, Iterator first, Iterator last, bool stabilitytest=true );

    protected:

        template< class Graph1, class Graph2 >
        static void copyneg(const Graph1& g, Graph2& h);

        template <class Cont>
        struct InfoPseudoAssoc {
            const Cont* cont;

            InfoPseudoAssoc(const Cont& arg) : cont(&arg) {}

            template <class Key>
            typename Cont::ValType operator[](Key key) const
            {
                return (*cont)[key->info];
            }
        };

	};

//	class MaxCliqueHeur: public MaxCliqueHeurPar< Koala::AlgsDefaultSettings > {};
	class MaxCliqueHeur: public MaxCliqueHeurPar< Koala::LocalGrAdjMatrSettings > {};

    //NEW:
    template< class DefaultStructs > class MaxCliquePar : private MaxCliqueHeurPar<DefaultStructs>
    {
      public:

        template< class GraphType, class OutputIterator > static int
            get( GraphType & g, OutputIterator out, int minSize = 0);
        template< class GraphType, class OutputIterator > static int
            getSome( GraphType & g, OutputIterator out, int minSize);

            using MaxCliqueHeurPar<DefaultStructs>::test;
            using MaxCliqueHeurPar<DefaultStructs>::testMax;
    };

    class MaxClique: public MaxCliquePar< Koala::LocalGrAdjMatrSettings > {};
    //class MaxClique: public MaxCliquePar< Koala::AlgsDefaultSettings > {};

    //NEW:
    template< class DefaultStructs > class MinVertCoverHeurPar
	{
	public:

		template< class GraphType, class ChoiceFunction, class OutputIterator, class VertContainer >
			static unsigned getWMin( const GraphType &g, OutputIterator out, ChoiceFunction choose,
				const VertContainer & vertTab );

		template< class GraphType, class OutputIterator, class ChoiceFunction, class VertContainer >
			static unsigned getWMax( const GraphType &g, OutputIterator out, ChoiceFunction choose,
				const VertContainer &vertTab );

		template< class GraphType, typename Iterator >
			static bool test( const GraphType &g, Iterator first, Iterator last );

		template< class GraphType, typename Iterator >
			static bool testMin( const GraphType &g, Iterator first, Iterator last, bool stabilitytest=true );

    protected:

        template< class GraphType, typename Iterator, typename IterOut >
        static int vertSetMinus(const GraphType &g, Iterator first, Iterator last,IterOut out);

	};

//	class MinVertCoverHeur: public MinVertCoverHeurPar< Koala::AlgsDefaultSettings > {};
	class MinVertCoverHeur: public MinVertCoverHeurPar< Koala::LocalGrAdjMatrSettings > {};

    //NEW:
    template< class DefaultStructs > class MinVertCoverPar : private MinVertCoverHeurPar<DefaultStructs>
    {
      public:

        template< class GraphType, class OutputIterator > static int
            get( GraphType & g, OutputIterator out, int maxSize = std::numeric_limits< int >::max());
        template< class GraphType, class OutputIterator > static int
            getSome( GraphType & g, OutputIterator out, int maxSize);

            using MinVertCoverHeurPar<DefaultStructs>::test;
            using MinVertCoverHeurPar<DefaultStructs>::testMin;
    };

    class MinVertCover: public MinVertCoverPar< Koala::LocalGrAdjMatrSettings > {};
    //class MaxClique: public MaxCliquePar< Koala::AlgsDefaultSettings > {};

#include "mis.hpp"
}

#endif
