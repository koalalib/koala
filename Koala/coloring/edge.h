#ifndef KOALA__EDGECOLORING__H__
#define KOALA__EDGECOLORING__H__

/* ------------------------------------------------------------------------- *
 * edge.h
 *
 * kolorowanie klasyczne krawedzi.
 * ------------------------------------------------------------------------- */

#include "../base/defs.h"
#include "../graph/view.h"
#include "../container/heap.h"
#include "../algorithm/search.h"
#include <algorithm>
#include <cassert>

namespace Koala {



template<class DefaultStructs>
class EdgeColoringTest {
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PEdge->int
	//if for any edge e of the graph colors[e]<0 then we assume that e is not colored
	template<typename Graph, typename ColorMap>
	static int maxColor(const Graph &graph, const ColorMap &colors);

	template <typename Graph, typename ColorMap>
	static bool testPart(const Graph &graph, const ColorMap &colors);
	template <typename Graph, typename ColorMap>
	static bool test(const Graph &graph, const ColorMap &colors);
};

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
	template<typename Graph, typename ColorMap>
	static int vizingSimple(const Graph &graph, ColorMap &colors);
	// Vizing method for edge coloring of multigraphs - complexity O(m(n+d^2)) - n=|V|, m=|V|, d=degree
	template<typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors);

	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, int maxCol);

	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	template<typename Graph, typename ColorMap>
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, int maxCol);

	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedy(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end);

	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end);

	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end, int maxCol);

	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors);

	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors);

	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		int maxCol);
};

class SeqEdgeColoring: public SeqEdgeColoringPar<AlgsDefaultSettings> {};


#include "edge.hpp"

}

#endif
