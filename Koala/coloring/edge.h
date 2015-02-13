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

//WEN: Uwaga! globalna! Wszedzie widze takie cudo: "If \t colors[v] < 0 we should assume that the e is not colored."
//Jesli jest taki ficzer, to mam wrazenie, ze samo pisanie o tym jest mylace.
//Przede wszystkim: kolory - liczby int >=0, mapa kolorow - czesciowe (lub calkowite - one z reszta jest szczegolnym przypadkiem czesciowego) pokolorowanie.
//Tzn. pokolorowane (na wej/wyj) sa te krawedzie, ktore sa kluczami w mapie, reszta nie.
//Dalej, rozwazane sa krawedzie typow Directed|Undirected - ich rodzaj jest ignorowany (po prostu krawedzie),
//Loopy sa ignorowane. Rownoleglosci dozwolone, chyba ze przy konkrentej metodzie napisano inaczej.
//Wejsciowe przedzialy iteratorow na krawedzie nie powinny zawierac petli ani powtorzen ani
//w przypadku kolorowanek edge juz pokolorowanych przed uruchomieniem metody (chyba ze w komentarzu jest inaczej)
//Metody ...Inter jesli dostaja na wejsciu pokolorowanie czesciowe, moga je zmienic tj. nie wykonuja przedluzenia funkcji
//(jesli gdzies przy nich wklepalem inny wen-koment, to sie kajam)
//Dotyczy calosci pliku. Amen.



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

	 *  The method finds the maximal used color.
	 *  \param graph the tested graph.
	 *  \param colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \return the maximal used color or -1 for empty coloring. */
	template<typename Graph, typename ColorMap>
	static int maxColor(const Graph &graph, const ColorMap &colors);

	/** \brief Test partial edge coloring.
	 *
	 *  The method test of the partial coloring given by the map \a colors is proper for the graph.
	 *  \param g the considered graph
	 *  \param colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \return true if the partial coloring is proper, false otherwise.
	 */
	template <typename Graph, typename ColorMap>
	static bool testPart(const Graph &g, const ColorMap &colors);

	/** \brief Test edge coloring.
	 *
	 *  The method test of the coloring given by the map \a colors is proper and complete for the graph.
	 *  \param g the considered graph
	 *  \param colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \return true if the coloring is proper and complete, false otherwise.
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
		const int notColored; //color>=0 && color<notColored
		typename DefaultStructs::template
			AssocCont<typename Graph::PVertex, int>::Type vertToTab;
		typename DefaultStructs::template
			AssocCont<typename Graph::PEdge, int>::Type edgeToList;
		//subgraph induced by 2 colors
		int colSubgraph[2];
		struct TabVert {
			int vc[2]; //2 neighbors
			typename Graph::PEdge ec[2]; //2 incident edges; ec[i] edge colored by color colSubgraph[i]
			int freeColor; //used only in vizingSimple (for simple graphs)
		} *tabVert;
		//to quick create a subgraph induced by a coloring we need lists:
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
		typename Graph::PEdge edge, typename Graph::PVertex vert);
	template<typename Graph, typename ColorMap>
	static void vizing(VizingState<Graph, ColorMap> &vState,
		typename Graph::PEdge edge, typename Graph::PVertex vert);

	template< typename Graph, typename ColorMap >
	static int colorInterchange(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);
public:
	//for all methods @param colors is a map(AssocTabInterface) Graph::PEdge->int
	//if for any edge e of the graph colors[e]<0 then we assume that e is not colored

	// Vizing method for edge coloring - complexity O(nm) - n=|V|, m=|E|
	/** \brief Vizing method for simple graphs.
	 *
	 *  The method colors uncolored edges of a simple graph using the Vizing method. The complexity of the algorithm is O(nm), where n = |V| and m = |E|.
	 *  \param g the considered graph.
	 *  \param colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \return the maximal assigned color or -1 if the edges were already colored.
	 *
	 *  [See example](examples/coloring/edgeColorVizing.html).
	 */
	template<typename Graph, typename ColorMap>
	static int vizingSimple(const Graph &g, ColorMap &colors);

	// Vizing method for edge coloring of multigraphs - complexity O(m(n+d^2)) - n=|V|, m=|V|, d=degree
	/** \brief Vizing method for multigraphs.
	 *
	 *  The method colors the edges of a multigraph using the Vizing method. The complexity of the algorithm is O(m(n+d<sup>2</sup>)),
	 *  where n = |V|, m = |E| and d is the graph degree.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer numbers) to edges.
	 	 WEN: wartosci poczatkowe ignorowane, ale jeszcze dopytac AJ czy jego procedura miala wspomagac sie ew. niepustym pokolorowaniem poczatkow.ego
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
	 *  \param vert the vertex from which it can make fan recoloring
	 *  \return the number of used color or  -1 if \a edge was colored. WEN: i wtedy color sie nie zmienia*/
	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, typename Graph::PVertex vert);

	/** \brief Coloring of edge (Vizing method for multigraphs).
	 *
	 *  The method colors the edge \a edge of a multigraph \a graph using the Vizing method.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param edge the colored edge.
	 *   If<tt>colors[e] < 0</tt> we should assume that the edge \a e is not colored.
	 *  \return the number of used color or  -1 if \a edge was colored. WEN: i wtedy color sie nie zmienia*/
	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	/** \brief Coloring of edge (Vizing method for multigraphs).
	 *
	 *  The method colors the edge \a edge of a multigraph \a graph using the Vizing method. The method tries to use color not greater then \a maxCol  if necessary method recolors other edges.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param edge the colored edge.
	 *  \param vert the vertex from which it can make fan recoloring
	 *  \param maxCol the maximal color expected. WEN: belkot, o co tu chodzi?
	 *  \return the number of used color or -1 if \a edge was colored. WEN: i wtedy color sie nie zmienia */
	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, typename Graph::PVertex vert, int maxCol);

	/** \brief Coloring of edge (Vizing method for multigraphs).
	 *
	 *  The method colors the edge \a edge of a multigraph \a graph using the Vizing method. The method tries to use color not greater then \a maxCol  if necessary method recolors other edges.
	 *  \param graph the considered graph.
	 *  \param colors the associative table (PEdge->int) that associates colors (integer nonnegative numbers) to edges.
	 *  \param edge the colored edge.
	 *  \param maxCol the maximal color expected. WEN: belkot, o co tu chodzi?
	 *  \return the number of used color or -1 if \a edge was colored. WEN: i wtedy color sie nie zmienia */
	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, int maxCol);

	/** \brief Greedy coloring of an edge.
	 *
	 *  The method colors uncolored \a edge with the smallest possible color (concerning colors of other edges).
	 *  \param g the considered graph.
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \param edge the colored edge.
	 *  \return the assigned color or -1 if the edge was already colored.
	 *
	 *  [See example](examples/coloring/edgeColorGreedy.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &g, ColorMap &colors,
		typename Graph::PEdge edge);

	/** \brief Greedy coloring of an edge (with interchanges).
	 *
	 *  The method colors uncolored edge with the smallest possible color (concerning colors of other edges).
	 *  The method tries to assign only colors from the set of already used colors.
	 *  If not possible, recolorings are introduced.
	 *  If recolorings fail i.e. each color lower or equal than the maximal is forbidden then a new color will set to the edge.
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \param edge the colored edge.
	 *  \return the assigned color or -1 if the edge was already colored.
	 */
	template<typename Graph, typename ColorMap>
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

	/** \brief Greedy coloring of an edge (with interchanges).
	 *
	 *  The method colors uncolored edge with the smallest possible color (concerning colors of other edges).
	 *  The method tries to assign only colors smaller or equal than \a maxCol.
	 *  If not possible, recolorings are introduced.
	 *  If recolorings fail i.e. each color lower or equal than \a maxCol is forbidden then a new color will set to the edge.
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \param edge the colored edge.
	 *  \param maxCol threshold for the interchange feature.
	 *  \return the assigned color or -1 if the edge was already colored.
	 */
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge, int maxCol);

	/** \brief Greedy coloring of edges.
	 *
	 *  The method colors uncolored edges from the sequence with the smallest possible color (concerning colors of other edges).
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \param beg the first element in the sequence of edges.
	 *  \param end the past-the-end element in the sequence of edges.
	 *  \return the maximal assigned color or -1 if the edges were already colored. WEN: dla przedzialu pustego po prostu
	 jw* WEN: Metoda odporna na powtorzenia elementow zakresu (kolejne wystapienia ignorowane) oraz elementy pokolorowane wstepnie jeszcze przed jej uruchomieniem (takie wystapienia ignorowane)
	 *  [See example](examples/coloring/edgeColorGreedy.html).
	 */
	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedy(const Graph &g, ColorMap &colors,
		EInIter beg, EInIter end);

	/** \brief Greedy coloring of edges (with interchanges).

	 *  The method colors uncolored edges from the sequence with the smallest possible color (concerning colors of other edges).
	 *  The method tries to assign only colors from the set of already used colors.
	 *  Each time if it is not possible, recolorings are introduced.
	 *  If recolorings don't create a free color lower or equal than the maximal a new color is set to the edge.
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \param beg the first element in the sequence of edges.
	 *  \param end the past-the-end element in the sequence of edges.
	 *  \return the maximal assigned color or -1 if the edges were already colored.
	 */
	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end);

	/** \brief Greedy coloring of edges (with interchanges).

	 *  The method colors uncolored edges from the sequence with the smallest possible color (concerning colors of other edges).
	 *  The method tries to assign only colors not greater than \a maxCol.
	 *  Each time if it is not possible, recolorings are introduced.
	 *  If recolorings don't create a free color lower or equal than \a maxCol a new color is set to the edge.
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \param beg the first element in the sequence of edges.
	 *  \param end the past-the-end element in the sequence of edges.
	 *  \param maxCol threshold for the interchange feature.
	 *  \return the maximal assigned color or -1 if the edges were already colored.
	 */
	template< typename Graph, typename ColorMap, typename EInIter >
	static int greedyInter(const Graph &graph, ColorMap &colors,
		EInIter beg, EInIter end, int maxCol);

	/** \brief Greedy coloring of edges.

	 *  The method colors uncolored edges from the graph with the smallest possible color (concerning colors of other edges).
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \return the maximal assigned color or -1 if the edges were already colored.
	 *
	 *  [See example](examples/coloring/edgeColorGreedy.html).
	 */
	template< typename Graph, typename ColorMap >
	static int greedy(const Graph &graph, ColorMap &colors);

	/** \brief Greedy coloring of edges (with interchanges).
	 *
	 *  The method colors uncolored edges from the graph with the smallest possible color (concerning colors of other edges).
	 *  The method tries to assign only colors from the set of already used colors.
	 *  Each time if it is not possible, recolorings are introduced.
	 *  If recolorings don't create a free color lower or equal than the maximal a new color is set to the edge.
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \return the maximal assigned color or -1 if the edges were already colored.
	 */
	template< typename Graph, typename ColorMap >
	static int greedyInter(const Graph &graph, ColorMap &colors);

	/** \brief Greedy coloring of edges (with interchanges).
	 *
	 *  The method colors uncolored edges from the graph with the smallest possible color (concerning colors of other edges).
	 *  The method tries to assign only colors not greater than \a maxCol.
	 *  Each time if it is not possible, recolorings are introduced.
	 *  If recolorings don't create a free color lower or equal than \a maxCol a new color is set to the edge.
	 *  \param g the considered graph
	 *  \param[in,out] colors the associative container (PEdge->int) that associates edges with colors (nonnegative integer).
	 *  \param maxCol threshold for the interchange feature.
	 *  \return the maximal assigned color or -1 if the edges were already colored.
	 */
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
