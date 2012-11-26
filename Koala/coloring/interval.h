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


template<class DefaultStructs>
class IntervalVertColoringPar {
public:
	//Weights: Graph::PVertex -> int (length of interval)
	//ColorMap: Graph::PVertex -> Color
	struct Color { //interval [min, max]
		int min, max;
		Color(int l=0, int r=1): min(l), max(r) { koalaAssert(l<=r,ContExcWrongArg); }
		Color(const Color &in): min(in.min), max(in.max) { }
		int size() const {return max - min + 1;}
		friend bool operator<(const Color &a, const Color &b) {
			return a.min<b.min || (a.min==b.min && a.max<b.max);
		}
	};

	//color vertex with set of consecutive nonnegative integers (or interval)
	// the set has cardinality weights[vert]
	//@return largest integer contained in added color
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, typename Graph::PVertex vert);

	//@return largest integer contained in added colors
	template<typename Graph, typename Weights, typename ColorMap, typename VIter>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, VIter beg, VIter end);

	//@return largest integer contained in added color
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors);

	template<typename Graph, typename Weights, typename ColorMap, typename VIter>
	static int li(const Graph &graph,
			const Weights &weights, ColorMap &colors, VIter beg, VIter end);

	template<typename Graph, typename Weights, typename ColorMap>
	static int li(const Graph &graph,
			const Weights &weights, ColorMap &colors);

	//testing if graph is properly colored
	template<typename Graph, typename Weights, typename ColorMap>
	static bool testPart(const Graph &graph, const Weights &weights,
			const ColorMap &colors);
	template<typename Graph, typename Weights, typename ColorMap>
	static bool test(const Graph &graph, const Weights &weights,
			const ColorMap &colors);

	template<typename Graph, typename ColorMap>
	static int maxColor(const Graph &graph, const ColorMap &colors);
private:
	template<typename Graph, typename Weights, typename ColorMap>
	static Color simulColor(const Graph &graph, const Weights &weights,
			const ColorMap &colors, typename Graph::PVertex vert);
};

class IntervalVertColoring: public IntervalVertColoringPar<AlgsDefaultSettings> {};

template<class DefaultStructs>
class IntervalEdgeColoringPar {
public:
	//Weights: Graph::PEdge -> int (length of interval)
	//ColorMap: Graph::PEdge -> Color
	struct Color { //interval [min, max]
		int min, max;
		Color(int l=0, int r=1): min(l), max(r) { koalaAssert(l<=r,ContExcWrongArg); }
		Color(const Color &in): min(in.min), max(in.max) { }
		int size() const {return max - min + 1;}
		friend bool operator<(const Color &a, const Color &b) {
			return a.min<b.min || (a.min==b.min && a.max<b.max);
		}
	};
	//color edge with set of consecutive nonnegative integers (or interval)
	// the set has cardinality weights[edge]
	//@return largest integer contained in added color
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, typename Graph::PEdge edge);

	//@return largest integer contained in added color
	template<typename Graph, typename Weights, typename ColorMap, typename EIter>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, EIter beg, EIter end);

	//@return largest integer contained in added color
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights, ColorMap &colors);

	//regula HF: podany zakres lub wszystkie niepokolorowane wierzcholki koloruj zachlannie wg. sekwencji nierosnacych wag
	template<typename Graph, typename Weights, typename ColorMap, typename EIter>
	static int hf(const Graph &graph, const Weights &weights,
			ColorMap &colors, EIter beg, EIter end);

	template<typename Graph, typename Weights, typename ColorMap>
	static int hf(const Graph &graph, const Weights &weights, ColorMap &colors);

	template<typename Graph, typename Weights, typename ColorMap, typename EIter>
	static int li(const Graph &graph, const Weights &weights,
			ColorMap &colors, EIter beg, EIter end);

	template<typename Graph, typename Weights, typename ColorMap>
	static int li(const Graph &graph, const Weights &weights, ColorMap &colors);

	//testing if graph is properly colored
	template<typename Graph, typename Weights, typename ColorMap>
	static bool testPart(const Graph &graph, const Weights &weights, const ColorMap &colors);
	template<typename Graph, typename Weights, typename ColorMap>
	static bool test(const Graph &graph, const Weights &weights, const ColorMap &colors);

	template<typename Graph, typename ColorMap>
	static int maxColor(const Graph &graph, const ColorMap &colors);

	// - ekstremalne wartosci wazonego stopnia w grafie z wagami na krawedziach
	template<typename Graph, typename Weights>
	static int getWDegs(const Graph &graph, const Weights &weights);
private:
	template<typename Graph, typename Weights, typename ColorMap>
	static Color simulColor(const Graph &graph, const Weights &weights,
			const ColorMap &colors, typename Graph::PEdge edge);
};

class IntervalEdgeColoring: public IntervalEdgeColoringPar<AlgsDefaultSettings> {};

#include "interval.hpp"

}

#endif
