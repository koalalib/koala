#ifndef KOALA__VERTEXCOLORING__H__
#define KOALA__VERTEXCOLORING__H__

/* ------------------------------------------------------------------------- *
 * vertex.h
 *
 * Klasyczne kolorowanie wierzcholkowe
 * ------------------------------------------------------------------------- */

#include "../base/defs.h"
#include "../graph/view.h"
#include "../algorithm/search.h"

#include <set>

namespace Koala {

/* ------------------------------------------------------------------------- *
 * SeqVertColoring
 *
 * Vertex coloring
 * ------------------------------------------------------------------------- */
/** \brief Methods for testing vertex coloring.
 *
 *  \ingroup color */
class VertColoringTest {
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PVertex->int
	//if for any vertex v of the graph colors[v]<0 then we assume that v is not colored

	// search for maximal color
	/** \brief Get maximal color.
	 *
	 *  The method finds the maximal used color.
	 *  \param graph the tested graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If \t colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color.	 */
	template< typename Graph, typename ColorMap >
	static int maxColor(const Graph &graph, const ColorMap &colors);

	/** \brief Test partial coloring.
	 *
	 *  The method test of the partial coloring given by the map \a colors is proper for the graph \a g.
	 *  \param g the considered graph
	 *  \param colors the associative container (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return true if the partial coloring is proper, false otherwise.
	 */
	template<typename Graph, typename ColorMap>
	static bool testPart(const Graph &g, const ColorMap &colors);

	/** \brief Test coloring.
	 *
	 *  The method test of the coloring given by the map \a colors is proper and complete for the graph \a g.
	 *  \param g the considered graph
	 *  \param colors the associative container (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return true if the coloring is proper and complete, false otherwise.
	 */
	template<typename Graph, typename ColorMap>
	static bool test(const Graph &g, const ColorMap &colors);
};
/** \brief Sequential graph coloring algorithms (parametrised).
 *  \ingroup color */
template<class DefaultStructs>
class SeqVertColoringPar: public VertColoringTest {
protected:
	template<typename Vertex>
	struct VertDeg { //is used in sl (Smallest last)
		VertDeg() {}
		VertDeg(const Vertex &_v, int d): v(_v), deg(d) {}
		VertDeg(const VertDeg<Vertex> &a): v(a.v), deg(a.deg) {}
		Vertex v;
		int deg;
		friend bool operator==(const VertDeg<Vertex> &a, const VertDeg<Vertex> &b) {
			return a.v==b.v && a.deg==b.deg;
		}
	};

	template<typename Vertex>
	struct VertDegSat { //is used in slf (saturated largest first)
		VertDegSat() {}
		VertDegSat(const Vertex &_v, int d, int s): v(_v), deg(d), sat(s) {}
		VertDegSat(const VertDegSat<Vertex> &a): v(a.v), deg(a.deg), sat(a.sat) {}
		Vertex v;
		int deg, sat;
		friend bool operator==(const VertDegSat<Vertex> &a, const VertDegSat<Vertex> &b) {
			return a.v==b.v && a.deg==b.deg && a.sat==b.sat;
		}
	};

	template<typename S> struct LfCmp {
		bool operator() (const S &a, const S &b);
	};

	template<typename S> struct SlCmp {
		bool operator() (const S &a, const S &b);
	};

	template<typename S> struct SlfCmp {
		bool operator() (const S &a, const S &b);
	};

	template< typename Graph, typename ColorMap >
	static int satDeg(const Graph &graph, const ColorMap &colors,
		typename Graph::PVertex vert);

	template< typename Graph, typename ColorMap, typename CompMap >
	static int interchangeComponents(const Graph &graph, const ColorMap &colors,
		typename Graph::PVertex vert, CompMap &map, int c1, int c2 );

	template< typename Graph, typename ColorMap >
	static int colorInterchange(const Graph &graph, ColorMap &colors,
		typename Graph::PVertex vert);

	//vertices in LF order;
	//@param beg,end Random access iterator
	template<typename Graph, typename VInOutIter>
	static int lfSort(const Graph &graph, VInOutIter beg, VInOutIter end);

	//vertices in SL order;
	//@param beg,end Random access iterator
	template<typename Graph, typename VInOutIter>
	static int slSort(const Graph &graph, VInOutIter beg, VInOutIter end);

	//brooks things
	template<typename Graph, typename ColorMap>
	struct BrooksState {
		const Graph &graph;
		ColorMap &colors;
		typename Graph::PVertex *vertStack;
		typename Graph::PEdge *edgeStack;
		int begVert, endVert, curVertStack;
		int begEdge, endEdge, curEdgeStack;
		typename DefaultStructs::template
			AssocCont<typename Graph::PVertex, int>::Type vertDepth;
		BrooksState(const Graph &g, ColorMap &c);
		~BrooksState();
		void biconnected(int bVert, int bEdge);
	};

	//DFS step for Brooks algorithm
	template<typename Graph, typename ColorMap>
	static int brooks(BrooksState<Graph, ColorMap> &bState,
		typename Graph::PVertex vert, int depth);
	//Create biconnected subgraphs and color them.
	//Method is using brooksBiconnectedColor and brooksBiconnectedTest
	template<typename Graph, typename ColorMap>
	static void brooksBiconnected(BrooksState<Graph, ColorMap> &bState);
	template<typename Graph>
	static void brooksBiconnectedColor(const Graph &graph, typename Graph::PVertex vert);
	//Checks if (graph-vertExc) is biconnected (DFS)
	template<typename Graph>
	static bool brooksBiconnectedTest(const Graph &graph,
		typename Graph::PVertex vertExc);
	//DFS step for brooksBiconnectedTest(Graph, Vert);
	template<typename Graph>
	static int brooksBiconnectedTest(const Graph &graph,
		typename Graph::PVertex vertExc, typename Graph::PVertex vert);

public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PVertex->int
	//if for any vertex v of the graph colors[v]<0 then we assume that v is not colored
	//if it is not mentioned methods colors only uncolored vertices

	// greedy vertex coloring
	/** \brief Greedy coloring of vertex.
	 *
	 *  The method colors \a vert with the smallest possible color (concerning other vertices colors).
	 *  \param g the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If \t colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param vert the colored vertex.
	 *  \return the assigned color or -1 if the vertex was already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedy.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors,
		typename Graph::PVertex vert);

	// greedy vertex coloring (with colors interchange)
	/** \brief Greedy coloring of vertex (with interchanges).
	 *
	 *  The method colors \a vert with the smallest possible color (concerning other vertices colors).
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If \t colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param vert the colored vertex.
	 *  \return the assigned color or -1 if the vertex was already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedyinter.html).
	 */
	template<typename Graph, typename ColorMap>
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PVertex vert);

	// greedy vertex coloring (with colors interchange)
	// interchange occurs only if new color exceed maxCol limit
	/** \brief Greedy coloring of vertex (with interchanges).
	 *
	 *  The method colors \a vert with the smallest possible color (concerning other vertices colors).
	 *  The method tries to assign only colors smaller than \a maxCol.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param vert the colored vertex.
	 *  \param maxCol the maximal expected color.
	 *  \return the assigned color or -1 if the vertex was already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedyinter.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PVertex vert, int maxCol);

	// greedy vertices range coloring
	/** \brief Greedy coloring of vertices.
	 *
	 *  The method colors vertices from the sequence, defined by \a beg and \a end, with the smallest possible color (concerning other vertices colors).
	 *  \param graph the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the first element in the sequence of vertices.
	 *  \param end the past-the-end element in the sequence of vertices.
	 *  \return the the maximal assigned color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedy.html).
	 */
	template< typename Graph, typename ColorMap, typename VInIter >
	static int greedy(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// greedy vertices range coloring (with colors interchange)
	/** \brief Greedy coloring of vertices (with interchanges).
	 *
	 *  The method colors vertices from the sequence, defined by \a beg and \a end, with the smallest possible color (concerning other vertices colors).
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the first element in the sequence of vertices.
	 *  \param end the past-the-end element in the sequence of vertices.
	 *  \return the the maximal assigned color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedyinter.html).
	 */
	template< typename Graph, typename ColorMap, typename VInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// greedy vertices range coloring (with colors interchange after exceeding maxCol)
	/** \brief Greedy coloring of vertices (with interchanges).
	 *
	 *  The method colors vertices from the sequence, defined by \a beg and \a end, with the smallest possible color (concerning other vertices colors).
	 *  The method tries to assign only colors not greater than maxCol or from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the first element in the sequence of vertices.
	 *  \param end the past-the-end element in the sequence of vertices.
	 *  \param maxCol the maximum expected color.
	 *  \return the the maximal assigned color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedyinter.html).
	 */
	template< typename Graph, typename ColorMap, typename VInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	// greedy graph coloring
	/** \brief Greedy coloring of vertices.
	 *
	 *  The method colors vertices from the \a graph, with the smallest possible color (concerning other vertices colors).
	 *  \param g the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the the maximal assigned color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedy.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors);

	// greedy graph coloring (with colors interchange)
	/** \brief Greedy coloring of vertices (with interchanges).
	 *
	 *  The method colors vertices from the \a graph, with the smallest possible color (concerning other vertices colors).
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the the maximal assigned color or -1 it the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedyinter.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors);

	// greedy graph coloring (with colors interchange after exceeding maxCol)
	/** \brief Greedy coloring of vertices (with interchanges).
	 *
	 *  The method colors vertices from the \a graph, with the smallest possible color (concerning other vertices colors).
	 *  The method tries to assign only colors not greater than maxCol.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param maxCol the maximal expected color.
	 *  \return the the maximal assigned color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_LF.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		int maxCol);

	// graph coloring - Largest First method
	/** \brief Largest First coloring.
	 *
	 *  The method colors the uncolored vertices of \a graph with the largest first algorithm.
	 *  \param graph the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_greedyinter.html).
	 */
	template< typename Graph, typename ColorMap >
	static int lf(const Graph &graph, ColorMap &colors);

	// graph coloring - Largest First method (with colors interchange)
	/** \brief Largest First coloring.
	 *
	 *  The method colors the uncolored vertices of \a graph with the largest first algorithm.
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_LF.html).
	 */
	template< typename Graph, typename ColorMap >
	static int lfInter(const Graph &graph, ColorMap &colors);

	// graph coloring - Largest First method
	//  (with colors interchange after exceeding maxCol)
	/** \brief Largest First coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices of \a graph with the largest first algorithm.
	 *  The method tries to assign only colors not greater than maxCol.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param maxCol the maximal expected color.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_LFI.html).
	 */
	template< typename Graph, typename ColorMap >
	static int lfInter(const Graph &graph, ColorMap &colors,
		int maxCol);

	// vertices range coloring - Largest First method
	/** \brief Largest First coloring.
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the largest first algorithm.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_LF.html).
	 */
	template < typename Graph, typename ColorMap, typename VInIter >
	static int lf(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Largest First method (with colors interchange)
	/** \brief Largest First coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the largest first algorithm.
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_LFI.html).
	 */
	template < typename Graph, typename ColorMap, typename VInIter >
	static int lfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Largest First method
	//  (with colors interchange after exceeding maxCol)
	/** \brief Largest First coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the largest first algorithm.
	 *  The method tries to assign only colors not greater than maxCol.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \param maxCol the maximal expected color.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_LFI.html).
	 */
	template < typename Graph, typename ColorMap, typename VInIter >
	static int lfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	//vertices in LF order;
	/** \brief Get LF order.
	 *
	 *  For vertices from the container given by \a beg and \a end, the method writes down to the iterator \a out the sequence congruent with LF method.
	 *  \param graph the considered graph.
	 *  \param beg the iterator to the first element in the container with input vertices.
	 *  \param end the iterator to the past-the-end element in the container with input vertices.
	 *  \param out the iterator to the output sequence of vertices.
	 *  \return the number of vertices in the output sequence.	 */
	template<typename Graph, typename VInIter, typename VOutIter>
	static int lfSort(const Graph &graph, VInIter beg, VInIter end,
		VOutIter out);

	// graph coloring - Smallest Last method
	/** \brief Smallest Last coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices of the graph \a graph with the smallest last algorithm.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color of -1 it the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SL.html).
	 */
	template<typename Graph, typename ColorMap>
	static int sl(const Graph &graph, ColorMap &colors);

	// graph coloring - Smallest Last method (with colors interchange)
	/** \brief Smallest Last coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices of the \a graph with the smallest last algorithm.
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLI.html).
	 */
	template< typename Graph, typename ColorMap >
	static int slInter(const Graph &graph, ColorMap &colors);

	// graph coloring - Smallest Last method
	//  (with colors interchange after exceeding maxCol)
	/** \brief Smallest Last coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices of the \a graph  with the smallest last algorithm.
	 *  The method tries to assign only colors not greater than maxCol.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param maxCol the maximal expected color.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLI.html).
	 */
	template< typename Graph, typename ColorMap >
	static int slInter(const Graph &graph,
		ColorMap &colors, int maxCol);

	// vertices range coloring - Smallest Last method
	/** \brief Smallest Last coloring.
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the smallest last algorithm.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SL.html).
	 */
	template<typename Graph, typename ColorMap, typename VInIter>
	static int sl(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Smallest Last method (with colors interchange)
	/** \brief Smallest Last coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the smallest last algorithm.
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLI.html).
	 */
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Smallest Last method
	//  (with colors interchange after exceeding maxCol)
	/** \brief Smallest Last coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the smallest last algorithm.
	 *  The method tries to assign only colors not greater than maxCol.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \param maxCol the maximal expected color.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLI.html).
	 */
	template< typename Graph, typename ColorMap, typename VInIter >
	static int slInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	//vertices in SL order;
	/** \brief Get SL order.
	 *
	 *  For vertices from the container given by \a beg and \a end, the method writes down to the iterator \a out the sequence congruent with SL method.
	 *  \param graph the considered graph.
	 *  \param beg the iterator to the first element in the container with input vertices.
	 *  \param end the iterator to the past-the-end element in the container with input vertices.
	 *  \param out the iterator to the output sequence of vertices.
	 *  \return the number of vertices in the output sequence.	 */
	template<typename Graph, typename VInIter, typename VOutIter>
	static int slSort(const Graph &graph, VInIter beg, VInIter end,
		VOutIter out);

	// graph coloring - Saturation Largest First method
	/** \brief Saturation Largest First coloring.
	 *
	 *  The method colors the uncolored vertices from the \a graph with the saturation largest first algorithm.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLF.html).
	 */
	template< typename Graph, typename ColorMap >
	static int slf(const Graph &graph, ColorMap &colors);

	// graph coloring - Saturation Largest First method (with colors interchange)
	/** \brief Saturation Largest First coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the graph with the saturation largest first algorithm.
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLFI.html).
	 */
	template< typename Graph, typename ColorMap >
	static int slfInter(const Graph &graph, ColorMap &colors);

	// graph coloring - Saturation Largest First method
	//  (with colors interchange after exceeding maxCol)
	/** \brief Saturation Largest First coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the graph with the saturation largest first algorithm.
	 *  The method tries to assign only colors not greater than maxCol or from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param maxCol the maximal expected color.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLFI.html).
	 */
	template< typename Graph, typename ColorMap >
	static int slfInter(const Graph &graph, ColorMap &colors, int maxCol);

	// vertices range coloring - Saturation Largest First method
	/** \brief Saturation Largest First coloring.
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the saturation largest first algorithm.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLF.html).
	 */
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slf(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Saturation Largest First method
	//  (with colors interchange)
	/** \brief Saturation Largest First coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the Saturation Largest First algorithm.
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  \param graph the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLFI.html).
	 */
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Saturation Largest First method
	//  (with colors interchange)
	/** \brief Saturation Largest First coloring (with interchange).
	 *
	 *  The method colors the uncolored vertices from the container defined by \a beg and \a end with the Saturation Largest First algorithm.
	 *  The method tries to assign only colors not greater than maxCol.
	 *  If not possible, recolorings are introduced.
	 *  \param g the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \param maxCol the maximal expected color.
	 *  \return the maximal used color or -1 if the vertices were already colored.
	 *
	 *  [See example](examples/coloring/coloring_SLFI.html).
	 */
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	//method recolors all graph (don't take the input coloring into account)
	/** \brief Brooks coloring.
	 *
	 * The method colors the graph using Brooks algorithm. The method ignores the coloring given by map \a colors.
	 *  \param g the considered graph.
	 *  \param colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color.
	 *
	 *  [See example](examples/coloring/coloring_brooks.html).
	 */
	template<typename Graph, typename ColorMap>
	static int brooks(const Graph &graph, ColorMap &colors);
};

class SeqVertColDefaultSettings: public AlgsDefaultSettings
{
	public:
		template< class A, class B, AssocMatrixType type > class TwoDimAssocCont
		{
		public:
			typedef Assoc2DimTable< type,std::map<std::pair<A,A>, B> > Type;/**<\brief Define own if intend to change.*/
			// Nie usuwac komentarza (przykladowe uzycia)
			// typedef AssocMatrix<A,B,type,std::vector< Privates::BlockOfAssocMatrix<B> >,Privates::PseudoAssocArray<A,int,AssocTable<BiDiHashMap<A,int> > > > Type;
		};

};


/** \brief Sequential graph coloring algorithms (default).
 *  \ingroup color */
class SeqVertColoring: public SeqVertColoringPar<SeqVertColDefaultSettings> {};

/** \brief Coloring methods using maximal independent set (parametrized).
 *
 *  \ingroup color
 */
template<class DefaultStructs>
class GisVertColoringPar: public VertColoringTest {
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PVertex->int
	//if for any vertex v of the graph colors[v]<0 then we assume that v is not colored
	//methods recolor colored vertices
	/** \brief Color vertices of graph using maximal independent set.
	 *
	 *  The method colors vertices from the sequence given by \a beg and \a end. The partial coloring given by the \a map colors is ignored.
	 *  \param graph the colored graph.
	 *  \param[out] colors the associative array (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \param beg the iterator to the first element of the container with input vertices.
	 *  \param end the iterator to the past-the-end element of the container with input vertices.
	 *  \return the maximal used color.
	 *
	 *  [See example](examples/coloring/coloring_GIS.html).
	 */
	template<typename Graph, typename ColorMap, typename VIter>
	static int color(const Graph &graph, ColorMap &colors, VIter beg, VIter end);

	/** \brief Color vertices of graph using maximal independent set.
	 *
	 *  The method colors vertices of \a graph. The partial coloring given by the map \a colors is ignored.
	 *  \param graph the colored graph.
	 *  \param[out] colors the associative container (PVert->int) that associates vertices with colors (integer nonnegative numbers).
	 *   If colors[v] < 0 we should assume that the vertex \a v is not colored.
	 *  \return the maximal used color.
	 *
	 *  [See example](examples/coloring/coloring_GIS.html).
	 */
	template<typename Graph, typename ColorMap>
	static int color(const Graph &graph, ColorMap &colors);
};
/** \brief Coloring methods using maximal independent set (default).
 *
 *  \ingroup color
 */
class GisVertColoring: public GisVertColoringPar<AlgsDefaultSettings> {};

//NEW: kolorowanie wierzcholkowe klasyczne - procedury niewielomianowe
template<class DefaultStructs>
class VertColoringPar: public VertColoringTest {
	public:
	    //NEW: znajduje optymalne pokolorowanie
		template<typename Graph, typename ColorMap>
		static int color(const Graph &graph, ColorMap &colors);
		//NEW: znajduje pokolorowanie o najwiekszym kolorze <=maxColor. Jesli w trakcie pracy
		//wykryje, ze jest to niemozliwe, zwraca -1
		template<typename Graph, typename ColorMap>
		static int color(const Graph &graph, ColorMap &colors, int maxColor);
	private:
		template<typename Graph, typename ColorMap>
		static int colorIterative(const Graph &graph, ColorMap &colors, int maxColor, int upperBound);
};

class VertColoring: public VertColoringPar<AlgsDefaultSettings> {};

#include "vertex.hpp"

}

#endif
