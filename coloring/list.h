#ifndef KOALA__LISTCOLORING__H__
#define KOALA__LISTCOLORING__H__

/* ------------------------------------------------------------------------- *
 * list.h
 *
 * Kolorowanie listowe grafow
 * ------------------------------------------------------------------------- */

#include "../base/defs.h"
#include "../graph/view.h"
#include "../container/heap.h"
#include "../algorithm/search.h"
#include <algorithm>
#include <cassert>

namespace Koala {

template<class DefaultStructs>
class ListVertColoringPar {
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PVertex->int
	//if for any edge e of the graph colors[e]<0 then we assume that e is not colored
	//for all methods @param colLists is a map(AssocTabInterface) Graph::PVertex->Set<int>
	// the Set<int> have to have Koala::Set interface

	//@param chooser - function object
	//   int chooser(const Graph &, const Set<int>, const ColorMap&, Graph::PVertex vert);
	//   the return value should be in the Set<int>
	//@return true if the chooser returns correct value
	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static bool colorChoose(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			typename Graph::PVertex vert, ColorChooser chooser );

	//like method above with chooser=FirstFit
	template<typename Graph, typename ColLists, typename ColorMap>
	static bool color(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, typename Graph::PVertex vert);

	//@return number of colored vertices
	template<typename Graph, typename ColLists, typename ColorMap,
			typename VIter, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, VIter beg, VIter end, ColorChooser chooser);

	//@return number of colored vertices
	template<typename Graph, typename ColLists, typename ColorMap, typename VIter>
	static int color(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, VIter beg, VIter end);

	//@return number of colored vertices
	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, ColorChooser chooser);

	//@return number of colored vertices
	template<typename Graph, typename ColLists, typename ColorMap>
	static int color(const Graph &graph, const ColLists &colLists, ColorMap &colors);

	//testing if graph is properly colored
	template<typename Graph, typename ColLists, typename ColorMap>
	static bool testPart(const Graph &graph,const ColLists &colLists, const ColorMap &colors);
	template<typename Graph, typename ColLists, typename ColorMap>
	static bool test(const Graph &graph,const ColLists &colLists, const ColorMap &colors);

	//extremal elements of the lists
	template<typename Graph, typename ColLists>
	static std::pair<int,int> listMinMax(const Graph &graph, const ColLists &colLists);

	//sorted sequence of the numbers used in the lists elements (unique numbers)
	//@return the sequence length
	template<typename Graph, typename ColLists, typename Iter>
	static int listColors(const Graph &graph, const ColLists &colLists, Iter out);
	//@return set with the numbers used in the lists elements
	template<typename Graph, typename ColLists>
	static Set<int> listColorsSet(const Graph &graph, const ColLists &colLists);
protected:
	struct FirstFit {
		template<typename Graph, typename ColList, typename ColorMap>
		int operator()(const Graph &graph, const ColList &colList,
				const ColorMap &colors, typename Graph::PVertex vert);
	};
};

class ListVertColoring: public ListVertColoringPar<AlgsDefaultSettings> {};

template<class DefaultStructs>
class ListEdgeColoringPar {
	//methods similar to ListVertColoring
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PEdge->int
	//if for any edge e of the graph colors[e]<0 then we assume that e is not colored
	//for all methods @param colLists is a map(AssocTabInterface) Graph::PEdge->Set<int>
	// the Set<int> have to have Koala::Set interface

	//@param chooser - function object
	//   int chooser(const Graph &, const Set<int>, const ColorMap&, Graph::PEdge edge);
	//   the return value should be in the Set<int>
	//@return true if the chooser returns correct value
	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static bool colorChoose(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			typename Graph::PEdge edge, ColorChooser chooser);

	//like method above with chooser=FirstFit
	template<typename Graph, typename ColLists, typename ColorMap>
	static bool color(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, typename Graph::PEdge edge);
	//takie samo dzialanie, jakby z funktorem zwracajacym jakikolwiek element dozwolonego zbioru barw

	//@return number of colored edges
	template<typename Graph, typename ColLists, typename ColorMap,
			typename EIter, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			EIter beg, EIter end, ColorChooser chooser);

	//@return number of colored edges
	template<typename Graph, typename ColLists, typename ColorMap, typename EIter>
	static int color(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			EIter beg, EIter end);

	//@return number of colored edges
	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, ColorChooser chooser);

	//@return number of colored edges
	template<typename Graph, typename ColLists, typename ColorMap>
	static int color(const Graph &graph, const ColLists &colLists, ColorMap &colors);

	//testing if graph is properly colored
	template<typename Graph, typename ColLists, typename ColorMap>
	static bool testPart(const Graph &graph, const ColLists &colLists, const ColorMap &colors);
	template<typename Graph, typename ColLists, typename ColorMap>
	static bool test(const Graph &graph, const ColLists &colLists, const ColorMap &colors);

	//extremal elements of the lists
	template<typename Graph, typename ColLists>
	static std::pair<int,int> listMinMax(const Graph &graph, const ColLists &colLists);

	//sorted sequence of the numbers used in the lists elements (unique numbers)
	//@return the sequence length
	template<typename Graph, typename ColLists, typename Iter>
	static int listColors(const Graph &graph, const ColLists &colLists, Iter out);

	//set of the numbers used in the lists elements
	template<typename Graph, typename ColLists>
	static Set<int> listColorsSet(const Graph &graph, const ColLists &colLists);
protected:
	struct FirstFit {
		template<typename Graph, typename ColList, typename ColorMap>
		int operator()(const Graph &graph, const ColList &colList,
				const ColorMap &colors, typename Graph::PEdge edge);
	};
};

class ListEdgeColoring: public ListEdgeColoringPar<AlgsDefaultSettings> {};


#include "list.hpp"

}

#endif
