#ifndef DEF_MIS_H
#define DEF_MIS_H

#include "../base/defs.h"
#include "../base/rnd.h"
#include "../graph/graph.h"
#include "search.h"


namespace Koala
{

	/** \brief Vertex choosing strategies (for maximal independent set) .
	 *
	 *  The namespace contains functors
	 *  that chooses a vertex basing on some specific rules.
	 *  Such rules could be: first vertex, random vertex, vertex that meets
	 *  the specific requirements (has the largest degree for example).
	 *  These functions are used within \a getWMin and \a getWMax methods in Koala::MISHeuristicPar
	 *  to choose one vertex in each algorithm step.
	 *
	 *  Each object function overload two parameter call function operator that takes \a g the considered graph and associative container \a vertTab, which assigns weight to each vertex. The functor returns one chosen vertex.
	 *  \ingroup DMmis */
	namespace MISStrategy
	{

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
		class First
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
		/** \brief Get minimum degree vertex functor.
		 *
		 *  The for a graph (weights are ignored) functor returns vertex with minimal degree. It is advised to use with method \a getWMin.*/
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
		/** \brief Get minimum degree and maximum weight vertex functor.
		*
		*  The for a graph and weights functor returns vertex for which W(v)/( deg(v) +1) is maximal. It is advised to use with method \a getWMin.*/
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
		/** \brief Get minimum degree vertex with weight functor.
		 *
		 *  The for a graph and weights functor returns vertices \a v for which Σ<sub>u ∈ N[v]</sub>W(u)/( deg(u) +1) ≤ W(v) . It is advised to use with method \a getWMin.*/
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
		/** \brief Get maximum weight and minimum sum of neighbors weights vertex functor.
		 *
		 *  The for a graph and weights functor returns vertex for which W(v)/Σ<sub>u ∈ N[v]</sub>W(u) is maximal. It is advised to use with method \a getWMin.*/
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
		/** \brief Get maximum degree vertex functor.
		 *
		 *  The for a graph gets the vertex with maximum degree. It is advised to use this functor with function \a getWMax.*/
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
		/** \brief Get maximum degree and minimal weight vertex functor.
		 *
		 *  The for a graph gets the vertex for which the function W(v) / (deg(v)*(deg(v)-1)) is minimal. It is advised to use this functor with function \a getWMax.*/
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
		/** \brief Get maximum degree and minimal weight vertex functor.
		 *
		 *  The for a graph and weights gets the vertices v that satisfy  Σ<sub>u ∈ N[v]</sub> W(u) / (deg(u)*(deg(u)-1)) ≥ W(v) /( deg(v) + 1 ). It is advised to use this functor with function \a getWMax.*/
		class GGWMax
		{
		public:
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
	template< class DefaultStructs > class MISHeuristicPar
	{
	public:
		/** \brief Search maximal independent set (WMin technique).
		 *
		 *  The method searches for maximal independent set using the following heuristic,
		 *  In each step (until the graph has no more vertices):
		 *   - chooses a vertex according to the choice function (Koala::MISStrategy),
		 *   - adds this vertex to the independent set,
		 *   - removes the closed neighborhood of this vertex.
		 *
		 *  The method outputs the independent set which is maximum in the sense of inclusion.\n
		 *  \param g the considered graph.
		 *  \param out the iterator to the container with the output set of vertices.
		 *  \param choose the strategy (\ref Koala::MISStrategy) of choosing vertices (one in each step) .
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
		 *   - chooses a vertex according to the choice function (\ref Koala::MISStrategy) ,
		 *   - removes the chosen vertex with adjacent edges.
		 *
		 *  The method outputs the remaining independent vertices.
		 *  \param g the considered graph.
		 *  \param out the iterator to the container with the output set of vertices.
		 *  \param choose the strategy (\ref Koala::MISStrategy)  of choosing vertices (one in each step).
		 *  \param vertTab the associative container that assigns weight to each vertex.
		 *  \return the number of vertices in the output set \a out.
		 *  \n
		 *
		 *  [See example](examples/mis/example_mis_getWMax.html).
		 */
		template< class GraphType, class OutputIterator, class ChoiceFunction, class VertContainer >
			static unsigned getWMax( const GraphType &g, OutputIterator out, ChoiceFunction choose,
				const VertContainer &vertTab );

		/** \brief Test if stable
		 *
		 * Determinate if a set of vertices is independent.
		 *  \param g     - graph to process
		 *  \param first - first vertex from the potentially independent set
		 *  \param last  - last vertex from the potentially independent set
		 * \return true is the given set is independent, false otherwise. */
		template< class GraphType, typename Iterator >
			static bool isStable( const GraphType &g, Iterator first, Iterator last );

		/**\brief Test if max stable.
		 *
		 *Determinate if a set of vertices is maximal (in the sense of inclusion) independent.
		 * \param  g     - graph to process
		 * \param first - first vertex from the potentially independent set
		 * \param last  - last vertex from the potentially independent set
		 * \retrun true is the given set is maximal (in the sense of inclusion) independent, false otherwise.*/
		template< class GraphType, typename Iterator >
			static bool isMaxStable( const GraphType &g, Iterator first, Iterator last, bool stabilitytest=true );

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
	class MISHeuristic: public MISHeuristicPar< Koala::AlgsDefaultSettings > {};


#include "mis.hpp"
}

#endif
