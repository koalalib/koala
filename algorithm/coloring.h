#ifndef KOALA__COLORING__H__
#define KOALA__COLORING__H__

/* ------------------------------------------------------------------------- *
 * coloring.h
 *
 * Różne metody kolorowania grafów.
 * ------------------------------------------------------------------------- */

#include "../base/def_struct.h"
#include "../graph/view.h"
#include "../container/heap.h"
#include "search.h"
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
	struct VertDeg {
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
	struct VertDegSat {
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

	template<typename Graph, typename ColorMap>
	static int brooks(const Graph &graph, ColorMap &colors);
};

class SeqVertColoring: public SeqVertColoringPar<AlgsDefaultSettings> {};

template<class DefaultStructs>
class GisVertColoringPar: public VertColoringTest {
public:
	template<typename Graph, typename ColorMap, typename VIter>
	static int color(const Graph &graph, ColorMap &colors, VIter beg, VIter end);

	template<typename Graph, typename ColorMap>
	static int color(const Graph &graph, ColorMap &colors);
};

class GisVertColoring: public GisVertColoringPar<AlgsDefaultSettings> {};

template<class DefaultStructs>
class EdgeColoringTest {
public:
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
		struct {
			int vc[2];
			typename Graph::PEdge ec[2];
			int freeColor;
		} *tabVert;
		int *colorToList; //colorToList[notColored] is a list of uncolored edges
		struct {
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
	// Vizing method for edge coloring - complexity O(nm) - n=|V|, m=|E|
	template<typename Graph, typename ColorMap>
	static int vizingSimple(const Graph &graph, ColorMap &colors);
	// Vizing method for edge coloring of multigraphs - complexity O(m(n+d^2)) - n=|V|, m=|V|, d=degree
	template<typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors);

	template <typename Graph, typename ColorMap>
	static int vizing(const Graph &graph, ColorMap &colors,
		typename Graph::PEdge edge);

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

template<class DefaultStructs>
class MaxMatchColoringPar: public EdgeColoringTest<DefaultStructs> {
public:
	template<typename Graph, typename EInIter, typename EOutIter>
	static int getMatching(const Graph &graph, EInIter beg, EInIter end, EOutIter out);
	template<typename Graph, typename EOutIter>
	static int getMatching(const Graph &graph, EOutIter out);
	template<typename Graph, typename ColorMap, typename EInIter>
	static int color(const Graph &graph, ColorMap &colors, EInIter beg, EInIter end);
	template<typename Graph, typename ColorMap>
	static int color(const Graph &graph, ColorMap &colors);
};

class MaxMatchColoring: public MaxMatchColoringPar<AlgsDefaultSettings> {};

template<class DefaultStructs>
class ListVertColoringPar {
public:
	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static bool colorChoose(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			typename Graph::PVertex vert, ColorChooser chooser );
	//, gdzie Mapa:PVertex->Set<int> a CholorChooser choose to obiekt funkcyjny podany przez uzytkownika, dzialajacy jako:
	//int chooser(const Graph&,PVertex V,const& Set<int>) - dla niepusteg zbioru kolorow dozwolnych przy v wybiera jeden element.
	//Funkcja color zwraca true jesli udalo sie wybrac poprawny kolor

	template<typename Graph, typename ColLists, typename ColorMap>
	static bool color(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, typename Graph::PVertex vert);
	//takie samo dzialanie, jakby z funktorem zwracajacym jakikolwiek element dozwolonego zbioru barw

	template<typename Graph, typename ColLists, typename ColorMap,
			typename VIter, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, VIter beg, VIter end, ColorChooser chooser);

	template<typename Graph, typename ColLists, typename ColorMap, typename VIter>
	static int color(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, VIter beg, VIter end);
	// - dla zakresow kolejno kolorowanych wierzcholkow.	Zawraca: czy udalo sie pokolorowac caly zakres?

	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, ColorChooser chooser);

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
	//set of the numbers used in the lists elements
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
	//methods similar to LisVertColoring
public:
	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static bool colorChoose(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			typename Graph::PEdge edge, ColorChooser chooser);

	template<typename Graph, typename ColLists, typename ColorMap>
	static bool color(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, typename Graph::PEdge edge);
	//takie samo dzialanie, jakby z funktorem zwracajacym jakikolwiek element dozwolonego zbioru barw

	template<typename Graph, typename ColLists, typename ColorMap,
			typename EIter, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			EIter beg, EIter end, ColorChooser chooser);

	template<typename Graph, typename ColLists, typename ColorMap, typename EIter>
	static int color(const Graph &graph, const ColLists &colLists, ColorMap &colors,
			EIter beg, EIter end);
	// - dla zakresow kolejno kolorowanych wierzcholkow.	Zwraca: czy udalo sie pokolorowac caly zakres?

	template<typename Graph, typename ColLists, typename ColorMap, typename ColorChooser>
	static int colorChoose(const Graph &graph, const ColLists &colLists,
			ColorMap &colors, ColorChooser chooser);

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

template<class DefaultStructs>
class IntervalVertColoringPar {
public:
	//Weights: Graph::PVertex -> int (length of interval)
	//ColorMap: Graph::PVertex -> Color
	struct Color { //interval [min, max)
		int min, max;
		Color(int l=0, int r=1): min(l), max(r) { koalaAssert(l<=r,ContExcWrongArg); }
		Color(const Color &in): min(in.min), max(in.max) { }
		int size() const {return max - min;}
		friend bool operator<(const Color &a, const Color &b) {
			return a.min<b.min || (a.min==b.min && a.max<b.max);
		}
	};

	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, typename Graph::PVertex vert);

	template<typename Graph, typename Weights, typename ColorMap, typename VIter>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, VIter beg, VIter end);

	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors);

	//Regula LI:
	//1. Znajdź jeszcze niepokolorowany wierzchołek v o najmniejszej możliwej liczbie c,
	//  takiej że nadanie v przedziału {c,...,c+w(v)–1} nie spowoduje konfliktu z wierzchołkami już pokolorowanymi.
	//  Przypisz v ten przedział. Remisy rozstrzygaj wybierajac wierzcholek o mniejszej wadze.
	//2. Powtarzaj punkt 1 aż do wyczerpania się niepokolorowanych wierzchołków.
	//- dane jest pokolorowanie czesciowe, badamy zakres niepokolorowanych wierzcholkow
	template<typename Graph, typename Weights, typename ColorMap, typename VIter>
	static int li(const Graph &graph,
			const Weights &weights, ColorMap &colors, VIter beg, VIter end);

	//- badamy wszystkie niepokolorowane wierzcholki
	template<typename Graph, typename Weights, typename ColorMap>
	static int li(const Graph &graph,
			const Weights &weights, ColorMap &colors);

	//- test poprawnosci podanego kolorowania
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
	struct Color { //interval [min, max)
		int min, max;
		Color(int l=0, int r=1): min(l), max(r) { koalaAssert(l<=r,ContExcWrongArg); }
		Color(const Color &in): min(in.min), max(in.max) { }
		int size() const {return max - min;}
		friend bool operator<(const Color &a, const Color &b) {
			return a.min<b.min || (a.min==b.min && a.max<b.max);
		}
	};
	//Kolorowanie sekwencyjne krawedzi przedzialami liczb calkowitych nieujemnych,
	//funkcja wagi krawedzi (liczby naturalne) podaje moc jej przedzialu.
	template<typename Graph, typename Weights, typename ColorMap>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, typename Graph::PEdge edge);

	template<typename Graph, typename Weights, typename ColorMap, typename EIter>
	static int greedy(const Graph &graph, const Weights &weights,
			ColorMap &colors, EIter beg, EIter end);

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

	//- test poprawnosci podanego kolorowania
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

#include "coloring.hpp"

}

#endif
