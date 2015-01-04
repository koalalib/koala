#ifndef KOALA__INTERVALCOLORING__H__
#define KOALA__INTERVALCOLORING__H__

/* ------------------------------------------------------------------------- *
 * interval.h
 *
 * Kolorowanie grafow wazonych interwalami
 * ------------------------------------------------------------------------- */

#include "../base/defs.h"
#include "../graph/view.h"

namespace Koala {


//NEW: zamiana struktur zagniezdzonych Color -> Segment (simple.h)

/** \brief The methods for interval coloring of graphs (parametrized).
 *  \ingroup color */
template<class DefaultStructs>
class IntervalVertColoringPar {
public:
	//Weights: Graph::PVertex -> int (length of interval)
	//ColorMap: Graph::PVertex -> Segment

	typedef Segment Color;

	//color vertex with set of consecutive nonnegative integers (or interval)
	// the set has cardinality weights[vert]
	//@return largest integer contained in added color
	/** \brief Color vertex greedily.
	 *
	 *  The method colors greedily the uncolored vertex with a set of consecutive nonnegative integers.
	 *  The set cardinality equals weights[vert]. The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PVert->int),
	 *     which assigns the expected size of interval to the vertex.
	 *  \param colors the associative container (PVert->Segment)
	 *     which assigns a structure \a Segment to the vertex.
	 *  \param vert the considered vertex.
	 *  @return the largest added color i.e. the \a max field in the interval or -1 if was colored. */
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, typename Graph::PVertex vert);

	//@return largest integer contained in added colors
	/** \brief Color vertices greedily.
	 *
	 *  The method colors greedily the uncolored vertices, given by the sequence \a beg and \a end, with sets of consecutive nonnegative integers.
	 *  Each set cardinality equals respectively weights[vert]. The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PVert->int),
	 *     which assigns the expected size of interval to the vertex.
	 *  \param colors the associative container (PVert->Segment)
	 *     which assigns a structure \a color to the vertex.
	 *  \param beg the first element of the container with vertices that are to be colored.
	 *  \param end the past-the-end element of the container with vertices that are to be colored.
	 *  \return the largest added color i.e. the maximal \a max field in an added interval  or -1 if all vertices were colored. */
	template<typename Graph, typename Weights, typename ColorMap, typename VIter>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, VIter beg, VIter end);

	//@return largest integer contained in added color
	/** \brief Color all vertices greedily.
	 *
	 *  The method colors greedily  the uncolored vertices of graph with sets of consecutive nonnegative integers.
	 *  Each set cardinality equals respectively weights[vert]. The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PVert->int),
	 *     which assigns the expected size of interval to the vertex.
	 *  \param colors the associative container (PVert->Segment)
	 *     which assigns a structure \a Segment to the vertex.
	 *  \return the largest added color i.e. the maximal \a max field in an added interval  or -1 if the graph was colored. */
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors);

	/** \brief Heuristic LI
	 *
	 *  The method intervally colors all  the uncolored vertices from the container given by the iterators \a beg and \a end. The heuristic LI is used.  The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the map (PVertex->int) which assigns the expected length of interval to vertex.
	 *  \param beg the iterator to the first element in the container of vertices that are to be colored.
	 *  \param end the iterator to the past-the-end element in the container of vertices that are to be colored.
	 *  \param colors the associative container (PVert->Segment)
	 *     which assigns a structure \a Color to the vertex.
	 *  \return the largest color used.	 */
	template<typename Graph, typename Weights, typename ColorMap, typename VIter>
	static int li(const Graph &graph,
			const Weights &weights, ColorMap &colors, VIter beg, VIter end);

	/** \brief Heuristic LI
	 *
	 *  The method intervally colors all  the uncolored vertices of \a graph. The heuristic LI is used.  The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the map (PVertex->int) which assigns the expected length of interval to vertex.
	 *  \param colors the associative container (PVert->Segment)
	 *     which assigns a structure \a Segment to the vertex.
	 *  \return the largest color used.	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static int li(const Graph &graph,
			const Weights &weights, ColorMap &colors);

	//testing if graph is properly colored
	/** \brief Test partial coloring
	 *
	 *  The method tests if the partial coloring from the associative table \a colors is a prober interval coloring of \a graph.
	 *  \param graph the considered graph.
	 *  \param weights the map (PVertex->int) which assigns the  expected length of interval.
	 *  \param colors the associative container (PVert->Segment)
	 *     which assigns a structure \a Segment to the vertex.
	 *  \return true if the partial coloring is proper, false otherwise.	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static bool testPart(const Graph &graph, const Weights &weights,
			const ColorMap &colors);

	/** \brief Test coloring
	 *
	 *  The method tests if the coloring form associative table \a colors is a proper and complete interval coloring of \a graph.
	 *  \param graph the considered graph.
	 *  \param weights the map (PVertex->int) which assigns the expected length of interval.
	 *  \param colors the associative container (PVert->Segment)
	 *     which assigns a structure \a Segment to the vertex.
	 *  \return true if the coloring is proper and complete, false otherwise.	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static bool test(const Graph &graph, const Weights &weights,
			const ColorMap &colors);

	/** \brief Get maximal color.
	 *
	 *  The method the maximal color used in coloring represented by the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the map (PVertex->int) which assigns the expected length of interval.
	 *  \param colors the associative container (PVert->color)
	 *     which assigns a structure \a color to the vertex.
	 *  \return the maximal color used or -1 if none of vertices is colored.	 */
	template<typename Graph, typename ColorMap>
	static int maxColor(const Graph &graph, const ColorMap &colors);
private:
	template<typename Graph, typename Weights, typename ColorMap>
	static Segment simulColor(const Graph &graph, const Weights &weights,
			const ColorMap &colors, typename Graph::PVertex vert);
};
/** \brief The methods for interval coloring of graphs (default).
 *  \ingroup color */
class IntervalVertColoring: public IntervalVertColoringPar<AlgsDefaultSettings> {};

/** \brief The methods for interval edge coloring of graphs (parametrized).
 *  \ingroup color */
template<class DefaultStructs>
class IntervalEdgeColoringPar {
public:
	//Weights: Graph::PEdge -> int (length of interval)
	//ColorMap: Graph::PEdge -> Segment
	typedef Segment Color;

	//color edge with set of consecutive nonnegative integers (or interval)
	// the set has cardinality weights[edge]
	//@return largest integer contained in added color
	/** \brief Color edge greedily.
	 *
	 *  The method colors greedily the \a edge with a set of consecutive nonnegative integers as long as it haven't been already colored.
	 *  The set cardinality equals weights[edge]. The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PEdge->int),
	 *     which assigns the expected size of interval to the edge.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \param edge the considered edge.
	 *  \return the largest added color i.e. the \a max field in the interval or -1 if the edge was already colored. */
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, typename Graph::PEdge edge);

	//@return largest integer contained in added color
	/** \brief Color edges greedily.
	 *
	 *  The method colors greedily the uncolored edges, given by the sequence \a beg and \a end, with sets of consecutive nonnegative integers.
	 *  Each set cardinality equals respectively weights[edge]. The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PEdge->int),
	 *     which assigns the expected size of interval to the edge.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \param beg the first element of the container with edges that are to be colored.
	 *  \param end the past-the-end element of the container with edges that are to be colored.
	 *  \return the largest added color i.e. the maximal \a max field in an added interval  or -1 if all the edge were already colored. */
	template<typename Graph, typename Weights, typename ColorMap, typename EIter>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, EIter beg, EIter end);

	//@return largest integer contained in added color
	/** \brief Color graph edges greedily.
	 *
	 *  The method colors greedily the uncolored edges of graph with sets of consecutive nonnegative integers.
	 *  Each set cardinality equals respectively weights[edge]. The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PEdge->int),
	 *     which assigns the expected size of interval to the edge.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \return the largest added color i.e. the maximal \a max field in an added interval or -1 if the graph was already colored.
	 *
	 *  [See example](examples/coloring/edgeIntervalColor.html).
	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights, ColorMap &colors);

	//regula LF: podany zakres lub wszystkie niepokolorowane wierzcholki koloruj zachlannie wg. sekwencji nierosnacych wag
	/** \brief Heuristic LF.
	 *
	 *  The method colors the uncolored edges, given by the sequence \a beg and \a end, with sets of consecutive nonnegative integers.
	 *  Each set cardinality equals respectively weights[edge].
	 *  The edges are arranged in order of non-increasing weights. Then edges are colored in the given sequence.
	 *  The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PEdge->int),
	 *     which assigns the expected size of interval to the edge.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \param beg the first element of the container with edges that are to be colored.
	 *  \param end the past-the-end element of the container with edges that are to be colored.
	 *  \return the largest added color i.e. the maximal \a max field in an added interval. */
	template<typename Graph, typename Weights, typename ColorMap, typename EIter>
	static int lf(const Graph &graph, const Weights &weights,
			ColorMap &colors, EIter beg, EIter end);

	/** \brief Heuristic LF.
	 *
	 *  The method colors the uncolored edges of graph, with sets of consecutive nonnegative integers.
	 *  Each set cardinality equals respectively weights[edge].
	 *  The edges are arranged in order of non-increasing weights. Then edges are colored in the given sequence.
	 *  The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the associative container (PEdge->int),
	 *     which assigns the expected size of interval to the edge.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \return the largest added color i.e. the maximal \a max field in an added interval.
	 *
	 *  [See example](examples/coloring/edgeIntervalColor.html).
	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static int lf(const Graph &graph, const Weights &weights, ColorMap &colors);

	/** \brief Heuristic LI
	 *
	 *  The method intervally colors all the uncolored edges from the container given by the iterators \a beg and \a end. The heuristic LI is used.  The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the map (PEdge->int) which assigns the expected length of interval to edge.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \param beg the iterator to the first element in the container of edges that are to be colored.
	 *  \param end the iterator to the past-the-end element in the container of edges that are to be colored.
	 *  \return the largest color used.	 */
	template<typename Graph, typename Weights, typename ColorMap, typename EIter>
	static int li(const Graph &graph, const Weights &weights,
			ColorMap &colors, EIter beg, EIter end);

	/** \brief Heuristic LI
	 *
	 *  The method intervally colors all the uncolored edges of the graph. The heuristic LI is used.  The result is stored up in the map \a colors.
	 *  \param graph the considered graph.
	 *  \param weights the map (PEdge->int) which assigns the expected length of interval to edge.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \return the largest color used.
	 *
	 *  [See example](examples/coloring/edgeIntervalColor.html).
	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static int li(const Graph &graph, const Weights &weights, ColorMap &colors);

	//testing if graph is properly colored
	/** \brief Test partial interval edge coloring.
	 *
	 *  The method tests if the partial edge coloring form associative table \a colors is a prober interval edge coloring of \a graph.
	 *  \param graph the considered graph.
	 *  \param weights the map (PEdge->int) which assigns the expected length of interval.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \return true if the partial edge coloring is proper, false otherwise.	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static bool testPart(const Graph &graph, const Weights &weights, const ColorMap &colors);

	/** \brief Test interval edge coloring.
	 *
	 *  The method tests if the edge coloring form associative table \a colors is a prober interval edge coloring of \a graph.
	 *  \param graph the considered graph.
	 *  \param weights the map (PEdge->int) which assigns the expected length of interval.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \return true if the edge coloring is proper, false otherwise.	 */
	template<typename Graph, typename Weights, typename ColorMap>
	static bool test(const Graph &graph, const Weights &weights, const ColorMap &colors);

	/** \brief Get maximal color.
	 *
	 *  The method the maximal color used in edge coloring represented by the map \a colors.
	 *  \param graph the considered graph.
	 *  \param colors the associative container (PEdge->Segment)
	 *     which assigns a structure \a Segment to the edge.
	 *  \return the maximal color used or -1 if none of vertices is colored.	 */
	template<typename Graph, typename ColorMap>
	static int maxColor(const Graph &graph, const ColorMap &colors);

	// - ekstremalne wartosci wazonego stopnia w grafie z wagami na krawedziach
	/** \brief Get weighted degree.
	 *
	 *  The method the maximal weighted degree of graph. The weighted degree of a vertex is the sum of all the weights of edges incident to the vertex.
	 *  \param graph the considered graph.
	 *  \param weights the map (PEdge->int) which assigns the  expected length of interval.
	 *  \return the maximal weighted degree.	 */
	template<typename Graph, typename Weights>
	static int getWDegs(const Graph &graph, const Weights &weights);
private:
	template<typename Graph, typename Weights, typename ColorMap>
	static Segment simulColor(const Graph &graph, const Weights &weights,
			const ColorMap &colors, typename Graph::PEdge edge);
};
/** \brief The methods for interval edge coloring of graphs (default).
 *  \ingroup color */
class IntervalEdgeColoring: public IntervalEdgeColoringPar<AlgsDefaultSettings> {};

#include "interval.hpp"

}

#endif
