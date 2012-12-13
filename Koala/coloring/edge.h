#ifndef KOALA__EDGECOLORING__H__
#define KOALA__EDGECOLORING__H__

/* ------------------------------------------------------------------------- *
 * edge.h
 *
 * kolorowanie klasyczne krawedzi.
 * ------------------------------------------------------------------------- */

#include "../base/defs.h"
#include "../graph/view.h"

namespace Koala {


/** \brief Test edge coloring.
 *
 *  The class collects methods that test edge coloring.
 *  \ingroup color
 */
template<class DefaultStructs>
class EdgeColoringTest {
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PEdge->int
	//if for any edge e of the graph colors[e]<0 then we assume that e is not colored
	/** \brief Get maximal color.
	 *
	 *  The method checks what is the maximal used color.
	 *  \param graph the tested graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *   If <tt>colors[e] < 0</tt> we should assume that the edge \a e is not colored.
	 *  \return the maximal used color.	 */
	template<typename Graph, typename ColorMap>
	static int maxColor(const Graph &graph, const ColorMap &colors);

	/** \brief Test partial edge coloring.
	 *
	 *  The method checks if the map given by the associative container \a colors is a partial coloring of the graph. 
	 *  \param graph the tested graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *   If <tt>colors[e] < 0</tt> we should assume that the edge \a e is not colored.
	 *  \return true it the partial coloring is proper, false otherwise.	 */
	template <typename Graph, typename ColorMap>
	static bool testPart(const Graph &graph, const ColorMap &colors);
	
	/** \brief Test edge coloring.
	 *
	 *  The method checks if the map given by the associative container \a colors is a proper coloring of the graph. 
	 *  \param graph the tested graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *   If <tt>colors[e] < 0</tt> we should assume that the edge \a e is not colored.
	 *  \return true it the coloring is proper, false otherwise.
	 *
	 *  [See example](examples/coloring/edgeColorGreedy.html).
	 */
	template <typename Graph, typename ColorMap>
	static bool test(const Graph &graph, const ColorMap &colors);
};

/** \brief Edge coloring methods (parametrized).
 *  \ingroup color */
template<class DefaultStructs>
class SeqEdgeColoringPar: public EdgeColoringTest<DefaultStructs> {
protected:
	template<typename Graph, typename ColorMap>
	struct VizingState {
		const Graph &graph;
		const int delta;
		ColorMap &colors;
		int maxColor;
		const int notColored;
		typename DefaultStructs::template
			AssocCont<typename Graph::PVertex, int>::Type vertToTab;
		typename DefaultStructs::template
			AssocCont<typename Graph::PEdge, int>::Type edgeToList;
		int colSubgraph[2];
		struct TabVert {
			int vc[2];
			typename Graph::PEdge ec[2];
			int freeColor;
		} *tabVert;
		int *colorToList; //colorToList[notColored] is a list of uncolored edges
		struct ListEdgeCol {
			int next, prev;
			typename Graph::PEdge edge;
		} *listEdgeCol; //0 element is a guard

		VizingState(const Graph &g, ColorMap &c, int maxCol);
		~VizingState();
		inline void setColor(typename Graph::PEdge edge, int color);
		inline void changeColor(typename Graph::PEdge edge, int oldColor, int newColor);
		//void initVS();
		inline void subgraph(int color1, int color2);
		inline int altPathWalk(int ivert, int col);
		inline int altPathRecoloring(int ivert, int col);
	};
	template<typename Graph, typename ColorMap>
	static void vizingSimple(VizingState<Graph, ColorMap> &vState,
		typename Graph::PEdge edge);
	template<typename Graph, typename ColorMap>
	static void vizing(VizingState<Graph, ColorMap> &vState,
		typename Graph::PEdge edge);

	template< typename Graph, typename ColorMap >
	static int colorInterchange(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PEdge->int
	//if for any edge e of the graph colors[e]<0 then we assume that e is not colored

	// Vizing method for edge coloring - complexity O(nm) - n=|V|, m=|E|
	/** \brief Vizing method for simple graphs.
	 *
	 *  The method colors the edges of a simple graph using the Vizing method. The complexity of the algorithm is O(nm), where n = |V| and m = |E|.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *   If <tt>colors[e] < 0</tt> we should assume that the edge \a e is not colored.
	 *  \return the number of the highest used color.
	 *
	 *  [See example](examples/coloring/edgeColorVizing.html).
	 */
	template<typename Graph, typename ColorMap>
	static int vizingSimple(const Graph &graph, ColorMap &colors);

	// Vizing method for edge coloring of multigraphs - complexity O(m(n+d^2)) - n=|V|, m=|V|, d=degree
	/** \brief Vizing method for multigraphs.
	 *
	 *  The method colors the edges of a multigraph using the Vizing method. The complexity of the algorithm is O(m(n+d<sup>2</sup>)), 
	 *  where n = |V|, m = |E| and d is the graph degree.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer numbers) to edges.
	 *   If colors[e] < 0 we should assume that \a e is not colored.
	 *  \return the number of the highest used color.
	 *
	 *  [See example](examples/coloring/edgeColorVizing.html).
	 */
	template<typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors);

	/** \brief Coloring of edge (Vizing method for multigraphs).
	 *
	 *  The method colors the edge \a edge of a multigraph \a graph using the Vizing method. 
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param edge the colored edge.
	 *   If<tt>colors[e] < 0</tt> we should assume that the edge \a e is not colored.
	 *  \return the number of used color or  -1 if \a edge was colored. */
	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	/** \brief Coloring of edge (Vizing method for multigraphs).
	 *
	 *  The method colors the edge \a edge of a multigraph \a graph using the Vizing method. The method tries to use color not greater then \a maxCol  if necessary method recolors other edges. 
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param edge the colored edge.
	 *  \param maxCol the maximal color expected.
	 *  \return the number of used color or -1 if \a edge was colored.*/
	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, int maxCol);

	/** \brief Greedy coloring of edge.
	 *
	 *  The method assigns the minimal (concerning already colored edges) color to \a edge.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param edge the colored edge.
	 *  \return the color of colored edge or -1 if \a edge was colored.
	 *
	 *  [See example](examples/coloring/edgeColorGreedy.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	/** \brief Greedy coloring of edge.
	 *
	 *  The method assigns the minimal (concerning already colored edges) color to \a edge. It attempts not to use color greater than maximal already used. If it is not possible, other edges are tried to be recolored. 
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param edge the colored edge.
	 *  \return the color of colored edge or -1 if \a edge was colored.*/
	template<typename Graph, typename ColorMap>
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	/** \brief Greedy coloring of edge.
	 *
	 *  The method assigns the minimal (concerning already colored edges) color to \a edge. If attempts not to use color greater than \a maxCol.  If not possible the method tries to recolor other edges as long as the assigned color is greater then the maximal one already used on other edge. 
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer numbers) to edges.
	 *  \param edge the colored edge.
	 *  \param maxCol the maximal expected color.
	 *  \return the color of the colored edge or -1 if \a edge was colored.*/
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, int maxCol);

	/** \brief Greedy coloring of edges.
	 *
	 *  The method assigns the minimal (concerning already colored edges) colors to the edge sequence given by iterators \a beg and \a end.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param beg the iterator of the first element in the container with edges to be colored.
	 *  \param end the iterator of the past-the-end element in the container with edges to be colored.
	 *  \return the maximal used color or -1 if all edges from container were colored.
	 *
	 *  [See example](examples/coloring/edgeColorGreedy.html).
	 */
	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedy(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end);

	/** \brief Greedy coloring of edges.
	 *
	 * The method assigns the minimal (concerning already colored edges) colors to edge from the sequence given by the iterators \a beg and \a end. It attempts not to use color greater than the maximal already used one.  If it is not possible already colored edges are tried to be recolored.  
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer numbers) to edges.
	 *  \param beg the iterator of the first element in the container with edges to be colored.
	 *  \param end the iterator of the past-the-end element in the container with edges to be colored.
	 *  \return the maximal used color or -1 if all edges from container were colored.  */
	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end);

	/** \brief Greedy coloring of edges.
	 *
	 * The method assigns the minimal (concerning already colored edges) colors to the edge sequence given by iterators \a beg and \a end. It attempts not to use color greater than \a maxCol. If not possible the method try to recolor other edges as long as the color assigned to \a edge is greater than the maximal color of already colored edges. 
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param beg the iterator of the first element in the container with edges to be colored.
	 *  \param end the iterator of the past-the-end element in the container with edges to be colored.
	 *  \param maxCol the maximal expected color.
	 *  \return the maximal used color or -1 if all the edges from container were colored.  */
	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end, int maxCol);

	/** \brief Greedy edge coloring.
	 *
	 *  The method assigns the minimal (concerning already colored edges) colors to the edges.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \return the maximal used color or -1 if all edges from container were colored.
	 *
	 *  [See example](examples/coloring/edgeColorGreedy.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors);

	/** \brief Greedy edge coloring with interchange.
	 *
	 *  The method assigns the minimal (concerning already colored edges) colors to the edges. The method tries not to use a color greater than the already used one if not possible recoloring is introduced.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \return the maximal used color or -1 if all the edges were colored. */
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors);

	/** \brief Greedy edge coloring with interchange.
	 *
	 *  The method assigns the minimal (concerning already colored edges) colors to all edges. It tries not to use a color greater than maxCol if it is not possible recoloring is introduced.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param maxCol the maximal color.
	 *  \return the maximal used color or -1 if all the edges were colored. */
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		int maxCol);
};

/** \brief Edge coloring methods (default).
 *  \ingroup color */
class SeqEdgeColoring: public SeqEdgeColoringPar<AlgsDefaultSettings> {};


#include "edge.hpp"

}

#endif
