#ifndef KOALA__VERTEXCOLORING__H__
#define KOALA__VERTEXCOLORING__H__

/* ------------------------------------------------------------------------- *
 * vertex.h
 *
 * Klasyczne kolorowanie wierzcholkowe
 * ------------------------------------------------------------------------- */

#include "../base/defs.h"
#include "../graph/view.h"
#include "../container/heap.h"
#include "../algorithm/search.h"
#include <algorithm>
#include <cassert>

namespace Koala {

/* ------------------------------------------------------------------------- *
 * SeqVertColoring
 *
 * Vertex coloring
 * ------------------------------------------------------------------------- */

class VertColoringTest {
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PVertex->int
	//if for any vertex v of the graph colors[v]<0 then we assume that v is not colored

	// search for maximal color
	template< typename Graph, typename ColorMap >
	static int maxColor(const Graph &graph, const ColorMap &colors);

	template<typename Graph, typename ColorMap>
	static bool testPart(const Graph &g, const ColorMap &colors);
	template<typename Graph, typename ColorMap>
	static bool test(const Graph &g, const ColorMap &colors);
};

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
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors,
		typename Graph::PVertex vert);

	// greedy vertex coloring (with colors interchange)
	template<typename Graph, typename ColorMap>
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PVertex vert);
	// greedy vertex coloring (with colors interchange)
	// interchange occurs only if new color exceed maxCol limit
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PVertex vert, int maxCol);

	// greedy vertices range coloring
	template< typename Graph, typename ColorMap, typename VInIter >
	static int greedy(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// greedy vertices range coloring (with colors interchange)
	template< typename Graph, typename ColorMap, typename VInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// greedy vertices range coloring (with colors interchange after exceeding maxCol)
	template< typename Graph, typename ColorMap, typename VInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	// greedy graph coloring
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors);

	// greedy graph coloring (with colors interchange)
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors);

	// greedy graph coloring (with colors interchange after exceeding maxCol)
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		int maxCol);

	// graph coloring - Largest First method
	template< typename Graph, typename ColorMap >
	static int lf(const Graph &graph, ColorMap &colors);

	// graph coloring - Largest First method (with colors interchange)
	template< typename Graph, typename ColorMap >
	static int lfInter(const Graph &graph, ColorMap &colors);

	// graph coloring - Largest First method
	//  (with colors interchange after exceeding maxCol)
	template< typename Graph, typename ColorMap >
	static int lfInter(const Graph &graph, ColorMap &colors,
		int maxCol);

	// vertices range coloring - Largest First method
	template < typename Graph, typename ColorMap, typename VInIter >
	static int lf(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Largest First method (with colors interchange)
	template < typename Graph, typename ColorMap, typename VInIter >
	static int lfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Largest First method
	//  (with colors interchange after exceeding maxCol)
	template < typename Graph, typename ColorMap, typename VInIter >
	static int lfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	//vertices in LF order;
	template<typename Graph, typename VInIter, typename VOutIter>
	static int lfSort(const Graph &graph, VInIter beg, VInIter end,
		VOutIter out);

	// graph coloring - Smallest Last method
	template<typename Graph, typename ColorMap>
	static int sl(const Graph &graph, ColorMap &colors);

	// graph coloring - Smallest Last method (with colors interchange)
	template< typename Graph, typename ColorMap >
	static int slInter(const Graph &graph, ColorMap &colors);

	// graph coloring - Smallest Last method
	//  (with colors interchange after exceeding maxCol)
	template< typename Graph, typename ColorMap >
	static int slInter(const Graph &graph,
		ColorMap &colors, int maxCol);

	// vertices range coloring - Smallest Last method
	template<typename Graph, typename ColorMap, typename VInIter>
	static int sl(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Smallest Last method (with colors interchange)
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Smallest Last method
	//  (with colors interchange after exceeding maxCol)
	template< typename Graph, typename ColorMap, typename VInIter >
	static int slInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	//vertices in SL order;
	template<typename Graph, typename VInIter, typename VOutIter>
	static int slSort(const Graph &graph, VInIter beg, VInIter end,
		VOutIter out);

	// graph coloring - Saturation Largest First method
	template< typename Graph, typename ColorMap >
	static int slf(const Graph &graph, ColorMap &colors);

	// graph coloring - Saturation Largest First method (with colors interchange)
	template< typename Graph, typename ColorMap >
	static int slfInter(const Graph &graph, ColorMap &colors);

	// graph coloring - Saturation Largest First method
	//  (with colors interchange after exceeding maxCol)
	template< typename Graph, typename ColorMap >
	static int slfInter(const Graph &graph, ColorMap &colors, int maxCol);

	// vertices range coloring - Saturation Largest First method
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slf(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Saturation Largest First method
	//  (with colors interchange)
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end);

	// vertices range coloring - Saturation Largest First method
	//  (with colors interchange)
	template < typename Graph, typename ColorMap, typename VInIter >
	static int slfInter(const Graph &graph, ColorMap &colors,
		VInIter beg, VInIter end, int maxCol);

	//method recolors all graph (don't take the input coloring into account)
	template<typename Graph, typename ColorMap>
	static int brooks(const Graph &graph, ColorMap &colors);
};

class SeqVertColoring: public SeqVertColoringPar<AlgsDefaultSettings> {};

template<class DefaultStructs>
class GisVertColoringPar: public VertColoringTest {
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PVertex->int
	//if for any vertex v of the graph colors[v]<0 then we assume that v is not colored
	//methods recolor colored vertices
	template<typename Graph, typename ColorMap, typename VIter>
	static int color(const Graph &graph, ColorMap &colors, VIter beg, VIter end);

	template<typename Graph, typename ColorMap>
	static int color(const Graph &graph, ColorMap &colors);
};

class GisVertColoring: public GisVertColoringPar<AlgsDefaultSettings> {};


#include "vertex.hpp"

}

#endif
