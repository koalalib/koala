#ifndef KOALA__EQUITABLECOLORING__H__
#define KOALA__EQUITABLECOLORING__H__

#include "../graph/view.h"
#include "./vertex.h"
#include "./edge.h"

//TODO: niestety equitable powinno rozkladac sprawiedliwie kolory w podanym przedziale 1,2,...,l a nie
//w zbiorze wykorzystanych barw.
//TODO: laczenie sprawiedliwych pokolorowan skladowych spojnych w jedno pokolorowanie sprawiedliwe

namespace Koala {
/** \brief Equitable vertex coloring.
* \ingroup color */
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
	/**\brief Repair coloring.
	*
	*  The method modifies the coloring given by the map \a colors. It takes the color that is most commonly used, and tries to recolor it with lower color. If not possible new color is used. The procedure is repeated until graph is equitably colored.
	* \param graph the colored graph.
	* \param colors the associative container PVertex -> int, which assigns color (nonnegative integer) to vertices.
	* \return the highest color used (nonnegative integer).*/
	template<typename Graph, typename ColorMap>
	static int repair1(const Graph &graph, ColorMap &colors);

	/**\brief Repair coloring.
	*
	* The method modifies the coloring given by the map \a colors. It takes vertexes colored in most commonly used color and the set colored in the least commonly used one and try to interchange colors between them. If not possible new color is used. The procedure is repeated until graph is equitably colored.
	* \param graph the colored graph.
	* \param colors the associative container PVertex -> int, which assigns color (nonnegative integer) to vertices.
	* \return the highest color used (nonnegative integer). */
	template<typename Graph, typename ColorMap>
	static int repair2(const Graph &graph, ColorMap &colors);

	/**\brief Test if proper equitable coloring.
	*
	* The method tests if the coloring given by the associative container form a proper equitable coloring in \a graph.
	* \param graph the colored graph.
	* \param colors the associative container PVertex -> int, which assigns color (nonnegative integer) to vertices.
	* \return the difference between the number of vertexes in most and the least commonly used color. If the graph is equitably colored 0 or 1 is returned.*/
	template<typename Graph, typename ColorMap>
	static int test(const Graph &graph, const ColorMap &colors);
};
/** \brief Equitable vertex coloring (default).
* \ingroup color */
class EquitVertColoring: public EquitVertColoringPar<AlgsDefaultSettings> {};

/** \brief Equitable edge coloring.(parametrized)
* \ingroup color */
template<class DefaultStructs>
class EquitEdgeColoringPar {
	struct CountStruct {
		int cnt;
		int listEdge;
	};
public:
	/**\brief Repair coloring/
	*
	* The method converts a coloring given by \a colors to equitable coloring (new colors are not used).
	* \param graph the colored graph.
	* \param colors the associative container PEdge -> int, which assigns color (nonnegative integer) to edges.
	* \return the difference between the number of edges in most and the least commonly used color.
	*
	*  [See example](examples/coloring/edgeEquitableColor.html).
	*/
	template<typename Graph, typename ColorMap>
	static int repair(const Graph &graph, ColorMap &colors);

	/**\brief Test if proper equitable coloring.
	*
	* The method tests if the coloring given by the associative container form a proper equitable edge coloring in \a graph.
	* \param graph the colored graph.
	* \param colors the associative container PEdge -> int, which assigns color (nonnegative integer) to edges.
	* \return the difference between the number of edges in most and the least commonly used color. If the graph is equitably colored 0 or 1 is returned.*/
	template<typename Graph, typename ColorMap>
	static int test(const Graph &graph, const ColorMap &colors);
};
/** \brief Equitable edge coloring (default).
* \ingroup color */
class EquitEdgeColoring: public EquitEdgeColoringPar<AlgsDefaultSettings> {};

#include "equitable.hpp"

}

#endif
