#ifndef KOALA__EQUITABLECOLORING__H__
#define KOALA__EQUITABLECOLORING__H__

#include "../base/defs.h"
#include "../graph/view.h"
#include "../algorithm/search.h"
#include "vertex.h"
#include "edge.h"

namespace Koala {

template<class DefaultStructs>
class EquitVertColoringPar {
protected:
	struct CompDiff { //connected components
		int comp, diff;
		CompDiff(): comp(0), diff(0) {}
		CompDiff(int c, int d): comp(c), diff(d) {}
		friend bool operator<(const CompDiff &a, const CompDiff &b) {
			return a.diff>b.diff || (a.diff==b.diff&&a.comp<b.comp);
		}
	};

public:
	template<typename Graph, typename ColorMap>
	static int repair1(const Graph &graph, ColorMap &colors);

	template<typename Graph, typename ColorMap>
	static int repair2(const Graph &graph, ColorMap &colors);

	template<typename Graph, typename ColorMap>
	static int test(const Graph &graph, const ColorMap &colors);
};
class EquitVertColoring: public EquitVertColoringPar<AlgsDefaultSettings> {};


template<class DefaultStructs>
class EquitEdgeColoringPar {
	struct CountStruct {
		int cnt;
		int listEdge;
	};
public:
	template<typename Graph, typename ColorMap>
	static int repair(const Graph &graph, ColorMap &colors);

	template<typename Graph, typename ColorMap>
	static int test(const Graph &graph, const ColorMap &colors);
};
class EquitEdgeColoring: public EquitEdgeColoringPar<AlgsDefaultSettings> {};

#include "equitable.hpp"

}

#endif
