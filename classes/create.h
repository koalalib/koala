#ifndef KOALA_CREATOR_H
#define KOALA_CREATOR_H

#include "..\graph\graph.h"
#include "..\algorithm\weights.h"
#include <cstdlib>
#include <ctime>

namespace Koala
{

/* Utility class for creating different types of graphs.
Most of the methods return a pointer indicating the first vertex that has been created.
Common variables used in edge/vertex generators:
	- num, num1, num2 are integers, which indicate numbers of vertices.
	- dir is of EdgeDirection type and indicate the type of corresponding edge.
    - vInfoGen - generator for info objects for vertices used in form: vInfoGen(num),
    - eInfoGen - generator for info objects for edges used in form: eInfoGen(num1, num2, dir),
*/

class Creator{

	public:

		/* The function generates a clique, i.e. for every two vertices there is created (or not) a connection
		according to EdgeDirection mask. Additionally if the mask contains a loop then a loop is attached
		to each vertex.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  vInfoGen - generator for info objects for vertices used in form: vInfoGen(num),
		  eInfoGen - generator for info objects for edges used in form: eInfoGen(num1, num2, dir),
		  dir - edges direction mask.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static typename GraphType::PVertex clique(GraphType& g, int n,  VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* The function generates a clique, i.e. for every to vertices there is created (or not) a connection
		according to EdgeDirection mask. Additionally, if the mask contains a loop then a loop is attached
		to each vertex. Info objects for vertices and edges are empty.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  dir - edges direction mask.
		*/
		template< class GraphType >
		static typename GraphType::PVertex clique(GraphType& g, int n, EdgeDirection dir = EdUndir);


		/* The function generates a path. Additionally, if the mask contains a loop, then a loop is attached
		to each vertex.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static typename GraphType::PVertex path(GraphType& g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType >
		static typename GraphType::PVertex path(GraphType& g, int n, EdgeDirection dir = EdUndir);


		/* The function generates a cycle. Additionally, if the mask contains a loop, then a loop is attached
		to each vertex.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static typename GraphType::PVertex cycle(GraphType& g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType>
		static typename GraphType::PVertex cycle(GraphType& g, int n, EdgeDirection dir = EdUndir);


		/* The function generates an usual fan graph containing  n vertices. A fan graph F(n) is defined as the graph  N_1+ P(n-1),
		where N_1 is the empty graph containing one vertex and P(n-1) is the path graph on n-1 vertices.
		The center vertex is created as the first vertex. Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  centerDir - direction mask for edges connected to center vertex.
		  borderDir - direction mask for edges between border vertices.
		The function returns a pointer that indicates the first (and also center) vertex.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static typename GraphType::PVertex fan(GraphType& g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection centerDir, EdgeDirection borderDir);


		/* The function generates an usual fan graph containing  n vertices. A fan graph F(n) is defined as the graph  N_1+ P(n-1),
		where N_1 is the empty graph containing one vertex and P(n-1) is the path graph on n-1 vertices.
		The center vertex is created as the first vertex. Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		The function returns a pointer that indicates the first (and also center) vertex.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static typename GraphType::PVertex fan(GraphType& g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType >
		static typename GraphType::PVertex fan(GraphType& g, int n, EdgeDirection dir = EdUndir);

		/* The function generates a wheel. A wheel graph W_n is a graph with n vertices,
		formed by connecting a single vertex to all vertices of an (n-1)-cycle. The center vertex is created as the first vertex.
		Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  centerDir - direction mask for edges connected to center vertex,
		  borderDir - direction mask for edges between border vertices.
		  The function returns a pointer that indicates the first (and also center) vertex.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static typename GraphType::PVertex wheel(GraphType& g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection centerDir, EdgeDirection borderDir);


		/* The function generates a wheel. A wheel graph W_n is a graph with n vertices,
		formed by connecting a single vertex to all vertices of an (n-1)-cycle. The center vertex is created as the first vertex.
		Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		  The function returns a pointer that indicates the first (and also center) vertex.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static typename GraphType::PVertex wheel(GraphType& g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType >
		static typename GraphType::PVertex wheel(GraphType& g, int n, EdgeDirection dir = EdUndir);


		/* The function generates a complete bipartite graph K_n1,n2. Additionally, if the mask contains a loop then, a loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  n1 - number of vertices to create in the first partition,
		  n1 - number of vertices to create in the second partition,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		The function returns a pair of pointers. The pointers indicate first vertices in the partitions.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
		compBipartite(GraphType& g, int n1, int n2, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType>
		static std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
		compBipartite(GraphType& g, int n1, int n2, EdgeDirection dir = EdUndir);


		/* The function generates a complete K-partite graph. Each of K partitions has the same number of vertices
		specified by the input parameter k. Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  K - number of partition,
		  k - number of vertices to create in each of the K partitions,
		  itIns - (output) insert iterator, the function uses it to return K pairs of pointers; the first pointer in each pair indicate the first
				vertex in a particular partition of the created graph, the second pointer indicates the last vertex in the partition,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen, class IterInsert>
		static typename GraphType::PVertex
		compKPartite(GraphType& g, int K, int k, IterInsert itIns, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType, class IterInsert>
		static typename GraphType::PVertex
		compKPartite(GraphType& g, int K, int k, IterInsert itIns, EdgeDirection dir = EdUndir);


		/* The function generates a complete K-partite graph. The cardinalities of partitions are specified by a sequence of integers.
		The sequence is defined by two iterators: begin and end that are passed to the function as input parameters.
		Additionally if the mask contains a loop then the loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  begin - input iterator; it should indicate the integer that corresponds to the cardinality of the first partition,
		  end - iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
				in the container (similar to stl vector::end() method),
		  itIns - (output) insert iterator, the function uses it to return K pairs of pointers; the first pointer in each pair indicate the first
				vertex in a particular partition of the created graph, the second pointer indicates the last vertex in the partition,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen, class Iter, class IterInsert >
		static typename GraphType::PVertex
		compKPartite(GraphType& g, Iter begin, Iter end, IterInsert itIns, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType, class Iter, class IterInsert >
		static typename GraphType::PVertex
		compKPartite(GraphType& g, Iter begin, Iter end, IterInsert itIns, EdgeDirection dir = EdUndir);


		/* The function generates the Petersen graph, i.e., it creates the following undirected edges:
			0 - 1, 1 - 2, 2 - 3, 3 - 4, 4 - 0,
			5 - 6, 6 - 7, 7 - 8, 8 - 9, 9 - 5,
			0 - 8, 1 - 6, 2 - 9, 3 - 7, 4 - 5.
		Parameters:
		  g - an input graph,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex petersenGraph(GraphType& g, VInfoGen vInfoGen, EInfoGen eInfoGen);


		/* It is a simpler version of the above function*/
		template< class GraphType >
		static typename GraphType::PVertex petersenGraph(GraphType& g);


		/* The function generates a tree in which each non-leaf vertex has the same degree specified by the parameter deg.
		 The height of the tree is defined by the parameter height. Additionally if the mask contains a loop then the loop is attached to each vertex.
		 Parameters:
		  g - an input graph,
		  deg - degree of non-leaf vertices
		  height - the height of the tree, height = 0 means that there is only the root,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		  The function returns a pointer that indicates the root.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex regTree(GraphType& g, int deg, int height, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType >
		static typename GraphType::PVertex regTree(GraphType& g, int deg, int height, EdgeDirection dir = EdUndir);


	    /* The function generates a tree in which each non-leaf vertex on the same level has the same degree.
		The degree of vertices on the particular level is specified by the input sequence of integers.
		The sequence is defined by two iterators: begin and end that are passed to the function as input parameters.
		The first integer in the sequence corresponds to degree of the root, next element corresponds to the degree of
		children of the root, etc.
		Additionally if the mask contains a loop then the loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  begin - input iterator; it should indicate the integer that corresponds to the degree of the root of the tree,
		  end - iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
				in the container (similar to stl vector::end() method),
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		  The function returns a pointer that indicates the root.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen, class Iter>
		static typename GraphType::PVertex regTree(GraphType& g, Iter begin, Iter end, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType, class Iter>
		static typename GraphType::PVertex regTree(GraphType& g, Iter begin, Iter end, EdgeDirection dir = EdUndir);


		/* The function generates a caterpillar. "A caterpillar is a tree in which every graph vertex
		is on a central stalk or only one graph edge away from the stalk
		(in other words, removal of its endpoints leaves a path graph..." (based on http://mathworld.wolfram.com/Caterpillar.html)
		The graph is created in two phases. In the first phase a central path is created. In the next phase
		(leaves or legs) are created and attached to vertices on the path. The number of legs for each vertex
		on the path is specified by the input sequence of integers.
		The sequence is defined by two iterators: begin and end that are passed to the function as input parameters.
		The first integer in the sequence corresponds to the number of legs of the first vertex on the central path,
		next element corresponds to the number of legs for the next vertex and so on.
		Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		Parameters:
		  g - an input graph,
		  begin - input iterator; it should indicate the integer that describe the number of legs for the first vertex on the central path,
		  end - iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
				in the container (similar to stl vector::end() method),
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  pathDir - direction mask for edges that form the central path,
		  legDir - direction mask for edges that correspond to legs (leaves) of the caterpillar.
		The function returns a pointer that indicates the first vertex on the central path.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen, class Iter>
		static typename GraphType::PVertex
		caterpillar(GraphType& g, Iter begin, Iter end, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection pathDir, EdgeDirection legDir);


		/*  The function generates a caterpillar. "A caterpillar is a tree in which every graph vertex
		is on a central stalk or only one graph edge away from the stalk
		(in other words, removal of its endpoints leaves a path graph..." (based on http://mathworld.wolfram.com/Caterpillar.html)
		The graph is created in two phases. In the first phase a central path is created. In the next phase
		(leaves or legs) are created and attached to vertices on the path. The number of legs for each vertex
		on the path is specified by the input sequence of integers.
		Additionally if the mask contains a loop then the loop is attached to each vertex.
		 Parameters:
		  g - an input graph,
		  pathVertNum - number of vertices on the central path,
		  legNnm - number of legs that should be attached to each vertex on the central path,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  pathDir - direction mask for edges that form the central path,
		  legDir - direction mask for edges that correspond to legs (leaves) of the caterpillar.
		The function returns a pointer that indicates the first vertex on the central path.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex
		caterpillar(GraphType& g, int pathVertNum, int legNnm, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection pathDir, EdgeDirection legDir);


		/*  The function generates a caterpillar. "A caterpillar is a tree in which every graph vertex
		is on a central stalk or only one graph edge away from the stalk
		(in other words, removal of its endpoints leaves a path graph..." (based on http://mathworld.wolfram.com/Caterpillar.html)
		The graph is created in two phases. In the first phase a central path is created. In the next phase
		(leaves or legs) are created and attached to vertices on the path. The number of legs for each vertex
		on the path is specified by the input sequence of integers.
		Additionally if the mask contains a loop then the loop is attached to each vertex.
		 Parameters:
		  g - an input graph,
		  pathVertNum - number of vertices on the central path,
		  legNnm - number of legs that should be attached to each vertex on the central path,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  dir - edges direction mask.
		The function returns a pointer that indicates the first vertex on the central path.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex
		caterpillar(GraphType& g, int pathVertNum, int legNnm, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir);


		/* It is a simpler version of the above function*/
		template< class GraphType>
		static typename GraphType::PVertex
		caterpillar(GraphType& g, int pathVertNum, int legNnm, EdgeDirection dir = EdUndir);


		/* The function generates a random graph on n vertices according to Erdos–Renyi model G(n, p).
		Each edge is included in the graph with probability p independent from every other edge.
		If the type of the graph is set to directed, then each of the two possible (opposite directed) edges
		between two particular vertices is drawn independently.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  p - probability of edge's creation,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  randSeed - seed for pseudorandom generator,
		  type - the type of edges in the graph, i.e., directed or undirected.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex erdRen1(GraphType& g, int n, double p, VInfoGen vInfoGen, EInfoGen eInfoGen, unsigned int randSeed, EdgeType type = Undirected );


		/* The function generates a random graph on n vertices according to Erdos–Renyi model G(n, p).
		Each edge is included in the graph with probability p independent from every other edge.
		If the type of the graph is set to directed, then each of the two possible (opposite directed) edges
		between two particular vertices is drawn independently. Current time is used as a seed for pseudorandom number generator.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  p - probability of edge's creation,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  type - the type of edges in the graph, i.e., directed or undirected.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex erdRen1(GraphType& g, int n, double p, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType type = Undirected );


		/* It is a simpler version of the above function*/
		template< class GraphType>
		static typename GraphType::PVertex erdRen1(GraphType& g, int n, double p, EdgeType type = Undirected );


		/* The function generates a random graph on n vertices according to Erdos–Renyi model G(n, m).
		The graph contains m edges chosen uniformly at random from the collection of all possible edges, i.e.,
		 - in the case of undirected graphs the collection contains n(n-1)/2 edges,
		 - in the case of directed graphs the collection contains n(n-1) edges.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  m - number of edges to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  randSeed - seed for pseudorandom generator,
		  type - the type of edges in the graph, i.e., directed or undirected.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex erdRen2(GraphType& g, int n, int m, VInfoGen vInfoGen, EInfoGen eInfoGen, unsigned int randSeed, EdgeType type = Undirected);


		/* The function generates a random graph on n vertices according to Erdos–Renyi model G(n, m).
		The graph contains m edges chosen uniformly at random from the collection of all possible edges, i.e.,
		 - in the case of undirected graphs the collection contains n(n-1)/2 edges,
		 - in the case of directed graphs the collection contains n(n-1) edges.
		Current time is used as a seed for pseudorandom number generator.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  m - number of edges to create,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  randSeed - seed for pseudorandom generator,
		  type - the type of edges in the graph, i.e., directed or undirected.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static typename GraphType::PVertex erdRen2(GraphType& g, int n, int m, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType type = Undirected);


		/* It is a simpler version of the above function*/
		template< class GraphType>
		static typename GraphType::PVertex erdRen2(GraphType& g, int n, int m, EdgeType type = Undirected);

//		Creator();
//		~Creator();

	protected:

		/* The function adds n vertices to the graph g and if edge direction mask contains the loop constant it also adds loops to
		each of the created vertices.
		Parameters:
		  g - an input graph,
		  n - number of vertices to create,
		  num - the value that is passed to vInfoGen for the first vertex. The value is incremented and passed
		  to vInfoGen generator for the next vertex and so on,
		  dir - edges direction mask,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  type - the type of edges in the graph, i.e., directed or undirected.
		  The function returns a pair of pointers. The pointers indicate the first and the last vertex.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen >
		static std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
			addVertices(GraphType& g, int n, int num, EdgeDirection dir, VInfoGen vInfoGen, EInfoGen eInfoGen);


		/* The function adds n vertices to the graph g and if edge direction mask contains the loop constant it also adds loops to
		each of the created vertices. All created vertices are sorted in the table vTab[]. The table should be allocated by caller
		and its size should be at least equal to n.
		Parameters:
		  g - an input graph,
		  vTab - table to insert n pointers that point created vertices,
		  n - number of vertices to create,
		  num - the value that is passed to vInfoGen for the first vertex. The value is incremented and passed
		  to vInfoGen generator for the next vertex and so on,
		  dir - edges direction mask,
		  vInfoGen - generator for info objects for vertices,
		  eInfoGen - generator for info objects for edges,
		  type - the type of edges in the graph, i.e., directed or undirected.
		The function returns a pair of pointers. The pointers indicate the first and the last vertex.
		*/
		template< class GraphType, class  VInfoGen, class  EInfoGen>
		static std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
		addVertices2Tab(GraphType& g, typename GraphType::PVertex* vTab, int n, int num, EdgeDirection dir, VInfoGen vInfoGen, EInfoGen eInfoGen);


		/* The function create an edges between two different vertices. In order to generate an info for an edge
			the eInfoGen generator is invoked in the following forms (depending on direction mask):
			- eInfoGen(eInfoGen(num1, num2, EdUndir),
			- eInfoGen(num1, num2, EdDirIn),
			- eInfoGen(num1, num2, EdDirOut).
		Parameters:
		  g - an input graph,
		  v1 - the first vertex,
		  v2 - the second vertex,
		  num1 - the value that is passed to eInfoGen generator,
		  num2 - the value that is passed to eInfoGen generator,
		  dir - edges direction mask,
		  eInfoGen - generator for info objects for edges.
		The function returns a pointer that indicates the created edge.
		*/
		template< class GraphType, class  EInfoGen >
		static typename GraphType::PEdge
			addEdges(GraphType& g, typename GraphType::PVertex v1, typename GraphType::PVertex v2, int num1, int num2, EdgeDirection dir, EInfoGen eInfoGen);


		/* The function generates a pseudorandom number num (from uniform distribution) such that begin <= num <= end.*/
		static int random(int begin, int end);

};

#include "create.hpp"

// Operacje na grafie traktowanym jak diagram relacji w zbiorze wierzcholkow
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class RelDiagramPar {
    public:

    template <class Graph>
    static void repair(Graph& g)
    // doprowadza diagram do zwyklej postaci, bez krawedzi nieskierowanych ani rownoleglych
    {   g.ch2Archs();g.delAllParals(EdDirOut);  }

    template <class Graph>
    // wpisz relacje pusta na istniejacych wierzcholkach
    static void empty(Graph& g)
    {   g.clearEdges(); }

    template <class Graph>
    // wpisz relacje kazdy z kazdym na istniejacych wierzcholkach. Mozna podac pole info wprowadzanych krawedzi
    static void total(Graph& g,const typename Graph::EdgeInfoType& einfo=typename Graph::EdgeInfoType())
    {
        g.clearEdges();
        for(typename Graph::PVertex u=g.getVert();u;u=g.getVertNext(u))
        {   g.addLoop(u,einfo);
            for(typename Graph::PVertex v=g.getVertNext(u);v;v=g.getVertNext(v))
            {
                g.addArch(u,v,einfo);g.addArch(v,u,einfo);
            }
        }
    }

    template <class Graph>
    // zamienia w relacje odwrotna
    static void inv(Graph& g)
    {   g.revert(); }

    template <class Graph>
    // przeprowadza domkniecie zwrotne. Mozna podac pole info wprowadzanych krawedzi
    static void reflClousure(Graph& g,const typename Graph::EdgeInfoType& einfo=typename Graph::EdgeInfoType())
    {
        for(typename Graph::PVertex u=g.getVert();u;u=g.getVertNext(u))
            if (!g.getEdge(u,EdLoop)) g.addLoop(u,einfo);
    }

    template <class Graph>
    // przeprowadza domkniecie symetryczne. Mozna podac pole info wprowadzanych krawedzi
    static void symmClousure(Graph& g,const typename Graph::EdgeInfoType& einfo=typename Graph::EdgeInfoType())
    {   typename DefaultStructs::template TwoDimAssocCont<typename Graph::PVertex,bool,AMatrNoDiag> :: Type matr(g.getVertNo());
        typename Graph::PEdge e,enext;
        for(e=g.getEdge(Directed|Undirected);e;e=g.getEdgeNext(e,Directed|Undirected))
        {   matr(g.getEdgeEnd1(e),g.getEdgeEnd2(e))=true;
            if (g.getEdgeType(e)==Undirected)
                matr(g.getEdgeEnd2(e),g.getEdgeEnd1(e))=true;
        }
        for(e=g.getEdge(Directed);e;e=enext)
        {   enext=g.getEdgeNext(e,Directed);
            if (matr(g.getEdgeEnd1(e),g.getEdgeEnd2(e))!=matr(g.getEdgeEnd2(e),g.getEdgeEnd1(e)))
            {   g.addArch(g.getEdgeEnd2(e),g.getEdgeEnd1(e),einfo);
                matr(g.getEdgeEnd1(e),g.getEdgeEnd2(e))=false;
            }
        }
    }


    template <class Graph>
    // przeprowadza domkniecie przechodnie. Mozna podac pole info wprowadzanych krawedzi
    static void transClousure(Graph& g,const typename Graph::EdgeInfoType& einfo=typename Graph::EdgeInfoType())
    {   typename DefaultStructs::template TwoDimAssocCont<typename Graph::PVertex, typename FloydPar<DefaultStructs>
                        ::template VertLabs<int,Graph>,AMatrFull > :: Type matr(g.getVertNo());
//        UnitLengthsEdgeCont econt;
        typename FloydPar<DefaultStructs>::template UnitLengthEdges<int> econt;
        FloydPar<DefaultStructs>::distances(g,matr,econt);
        for(typename Graph::PVertex u=g.getVert();u;u=g.getVertNext(u))
            for(typename Graph::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if(u!=v && matr(u,v).distance>1 && DefaultStructs::template NumberTypeBounds<int>::plusInfty()
                                >matr(u,v).distance) g.addArch(u,v,einfo);
        for(typename Graph::PEdge e=g.getEdge(Directed|Undirected);e;e=g.getEdgeNext(e,Directed|Undirected))
            if ((g.getEdgeType(e)==Undirected)||
                ((g.getEdgeType(e)==Directed)&& DefaultStructs::template NumberTypeBounds<int>::plusInfty()
		    > matr(g.getEdgeEnd2(e),g.getEdgeEnd1(e)).distance))
            {
                if (!g.getEdge(g.getEdgeEnd1(e),EdLoop)) g.addLoop(g.getEdgeEnd1(e),einfo);
                if (!g.getEdge(g.getEdgeEnd2(e),EdLoop)) g.addLoop(g.getEdgeEnd2(e),einfo);
            }
    }

    struct MatrixForm {
//    Oferuje te same operacje na relacjach reprezentowanych inaczej niz przez graf.
//    Wersje 2-argumentowe: kontener obslugiwany jak tablica 2-wymiarowa o wartosciach konwertowalnych z bool
//        indeksowana od 0 kolejnymi liczbami. Parametr size podaje liczbe elementow
//    Wersje 3-argumentowe: kontener o wartosciach konwertowalnych z bool i dostepie przez 2-argumentowy
//        operator(), iteratory podaja zakres przedzialu elementow

        template <class Cont>
        static void empty(Cont& cont, int size)
        {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  cont[i][j]=false;  }

        template <class Cont, class Iter>
        static void empty(Cont& cont, Iter beg,Iter end)
        {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  cont(*i,*j)=false;  }

        template <class Cont>
        static void total(Cont& cont, int size)
        {   for(int i=0;i<size;++i) for(int j=0;j<size;++j)  cont[i][j]=true;   }

        template <class Cont,class Iter>
        static void total(Cont& cont, Iter beg,Iter end)
        {   for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)  cont(*i,*j)=true; }

        template <class Cont>
        static void inv(Cont& cont, int size)
        {   for(int i=0;i<size-1;++i) for(int j=i+1;j<size;++j)
                if (cont[i][j]!=cont[j][i])
                {  bool tmp=(bool)cont[i][j];   cont[i][j]=cont[j][i]; cont[j][i]=tmp; }
        }

        template <class Cont,class Iter>
        static void inv(Cont& cont, Iter beg,Iter end)
        {   for(Iter i=beg;i!=end;++i)
            {   Iter j=i;
                for(++j;j!=end;++j) if (cont(*i,*j)!=cont(*j,*i))
                {  bool tmp=(bool)cont(*i,*j);   cont(*i,*j)=cont(*j,*i); cont(*j,*i)=tmp; }
            }
        }

        template <class Cont>
        static void reflClousure(Cont& cont, int size)
        {   for(int i=0;i<size;++i) cont[i][i]=true;    }

        template <class Cont, class Iter>
        static void reflClousure(Cont& cont,Iter beg, Iter end)
        {   for(Iter i=beg;i!=end;++i) cont(*i,*i)=true; }

        template <class Cont>
        static void symmClousure(Cont& cont, int size)
        {   for(int i=0;i<size-1;++i) for(int j=i+1;j<size;++j)  cont[i][j]=cont[j][i]=cont[i][j] || cont[j][i];    }

        template <class Cont,class Iter>
        static void symmClousure(Cont& cont,Iter beg, Iter end)
        {   for(Iter i=beg;i!=end;++i) for(Iter j=i;j!=end;++j)
                if (i!=j) cont(*i,*j)=cont(*j,*i)= cont(*i,*j)||cont(*j,*i);    }

        template <class Cont>
        static void transClousure(Cont& cont,int size)
        {   for(int k=0;k<size;++k) for(int i=0;i<size;++i) for(int j=0;j<size;++j)
                cont[i][j]=cont[i][j] || (cont[i][k] && cont[k][j]);
        }

        template <class Cont, class Iter>
        static void transClousure(Cont& cont,Iter beg, Iter end)
        {   for(Iter k=beg;k!=end;++k) for(Iter i=beg;i!=end;++i) for(Iter j=beg;j!=end;++j)
                cont(*i,*j)=
                    cont(*i,*j) || (cont(*i,*k) && cont(*k,*j));
        }

    };

};


// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class RelDiagram : public RelDiagramPar<AlgsDefaultSettings> {};


// Kreator linegrafow
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class LineGraphPar {
        protected:

        template <class Graph>
        static bool open(const Graph& g,typename Graph::PEdge e,typename Graph::PVertex v, typename Graph::PEdge f)
            {
                return (e!=f) && g.isEdgeEnd(e,v) && g.isEdgeEnd(f,v);
            }

        template <class Graph>
        static bool openDir(const Graph& g,typename Graph::PEdge e,typename Graph::PVertex v, typename Graph::PEdge f)
            {
                return  (e!=f) && g.isEdgeEnd(e,v) && g.isEdgeEnd(f,v)
                        && (g.getEdgeDir(e,v)!=EdDirOut) && (g.getEdgeDir(f,v)!=EdDirIn);
            }

        public:

//        Dopisuje do lg nieskierowany graf krawedziowy tworzony na podstawie g.
//        Para casterow wyznacza infa nowych wierzcholkow na podstawie inf oryginalnych krawedzi
//        i infa nowych krawedzi na podstawie inf wierzcholkow, w ktorych stykaly sie oryginalne krawedzie
//        Pierwszy Linker laczy krawedzie g z odpowiadajacymi im wierzcholkami w lg
//         Drugi laczy wierzcholki g z polaczeniami wierzcholkow w lg (ktore zachodza w tych wierzcholkach)
//      Zwraca pierwszy utworzony wierzcholek
        template <class GraphIn,class GraphOut,class VCaster, class ECaster,class VLinker,class ELinker>
        static typename GraphOut::PVertex undir(const GraphIn& g,GraphOut& lg,
                                                std::pair< VCaster,ECaster > casters,std::pair<VLinker,ELinker> linkers)
        {   typename DefaultStructs::template AssocCont<typename GraphIn::PEdge,typename GraphOut::PVertex>
                :: Type e2vtab(g.getEdgeNo());
            typename DefaultStructs::template AssocCont<typename GraphIn::PVertex,char>
                :: Type vset(g.getVertNo());
            typename GraphOut::PVertex res=lg.getVertLast();
            for(typename GraphOut::PVertex v=lg.getVert();v;v=lg.getVertNext(v))
                linkers.first(v,(typename GraphIn::PEdge)NULL);
            for(typename GraphIn::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            {   typename GraphOut::VertInfoType vinfo;
                casters.first(vinfo,g.getEdgeInfo(e));
                linkers.first(e2vtab[e]=lg.addVert(vinfo),e);
            }

            for(typename GraphIn::PVertex v=g.getVert();v;v=g.getVertNext(v))
                for(typename GraphIn::PEdge e=g.getEdge(v);e;e=g.getEdgeNext(v,e))
                    for(typename GraphIn::PEdge f=g.getEdgeNext(v,e);f;f=g.getEdgeNext(v,f))
                        if (open(g,e,v,f) &&
                            (!open(g,e,g.getEdgeEnd(e,v),f) ||(g.getEdgeEnd(e,v)>=v) ))
                            {   typename GraphOut::EdgeInfoType einfo;
                                casters.second(einfo,g.getVertInfo(v));
                                linkers.second(lg.addEdge(e2vtab[e],e2vtab[f],einfo),v);
                                vset[v];
                            }
            for(typename GraphIn::PVertex v=g.getVert();v;v=g.getVertNext(v)) if (!vset.hasKey(v))
                linkers.second((typename GraphOut::PEdge)NULL,v);
            return lg.getVertNext(res);
        }

        // jw. bez linkerow
        template <class GraphIn,class GraphOut,class VCaster, class ECaster>
        static typename GraphOut::PVertex undir(const GraphIn& g,GraphOut& lg,
                                                std::pair< VCaster,ECaster > casters)
        {   return undir(g,lg,casters,std::make_pair(stdLink( false,false ),stdLink( false,false )));  }

        // jw. ale tylko tworzy linegraph
        template <class GraphIn,class GraphOut>
        static typename GraphOut::PVertex undir(const GraphIn& g,GraphOut& lg)
        {   return undir(g,lg,std::make_pair(stdCast( false ),stdCast( false )),std::make_pair(stdLink( false,false ),stdLink( false,false )));  }


//        Dopisuje do lg skierowany graf krawedziowy tworzony na podstawie g.
//        Para casterow wyznacza infa nowych wierzcholkow na podstawie inf oryginalnych krawedzi
//        i infa nowych lukow na podstawie inf wierzcholkow, w ktorych stykaly sie oryginalne krawedzie
//        Pierwszy Linker laczy krawedzie g z odpowiadajacymi im wierzcholkami w lg
//         Drugi laczy wierzcholki g z polaczeniami wierzcholkow w lg (ktore zachodza w tych wierzcholkach)
//      Zwraca pierwszy utworzony wierzcholek
        template <class GraphIn,class GraphOut,class VCaster, class ECaster,class VLinker, class ELinker>
        static typename GraphOut::PVertex dir(const GraphIn& g,GraphOut& lg,
                                                std::pair< VCaster,ECaster > casters,std::pair<VLinker,ELinker > linkers)
        {   typename DefaultStructs::template AssocCont<typename GraphIn::PEdge,typename GraphOut::PVertex>
                :: Type e2vtab(g.getEdgeNo());
            typename DefaultStructs::template AssocCont<typename GraphIn::PVertex,char>
                :: Type vset(g.getVertNo());
            typename GraphOut::PVertex res=lg.getVertLast();
            for(typename GraphOut::PVertex v=lg.getVert();v;v=lg.getVertNext(v))
                linkers.first(v,(typename GraphIn::PEdge)NULL);
            for(typename GraphIn::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            {   typename GraphOut::VertInfoType vinfo;
                casters.first(vinfo,g.getEdgeInfo(e));
                linkers.first(e2vtab[e]=lg.addVert(vinfo),e);
            }

            for(typename GraphIn::PVertex v=g.getVert();v;v=g.getVertNext(v))
                for(typename GraphIn::PEdge e=g.getEdge(v);e;e=g.getEdgeNext(v,e))
                    for(typename GraphIn::PEdge f=g.getEdge(v);f;f=g.getEdgeNext(v,f))
                        if (openDir(g,e,v,f))
                            {   typename GraphOut::EdgeInfoType einfo;
                                casters.second(einfo,g.getVertInfo(v));
                                linkers.second(lg.addArch(e2vtab[e],e2vtab[f],einfo),v);
                                vset[v];
                            }
            for(typename GraphIn::PVertex v=g.getVert();v;v=g.getVertNext(v)) if (!vset.hasKey(v))
                linkers.second((typename GraphOut::PEdge)NULL,v);
            return lg.getVertNext(res);
        }


        // jw. bez linkerow
        template <class GraphIn,class GraphOut,class VCaster, class ECaster>
        static typename GraphOut::PVertex dir(const GraphIn& g,GraphOut& lg,
                                                std::pair< VCaster,ECaster > casters)
        {   return dir(g,lg,casters,std::make_pair(stdLink( false,false ),stdLink( false,false )));  }

        // jw. ale tylko tworzy linegraph
        template <class GraphIn,class GraphOut>
        static typename GraphOut::PVertex dir(const GraphIn& g,GraphOut& lg)
        {   return dir(g,lg,std::make_pair(stdCast( false ),stdCast( false )),std::make_pair(stdLink( false,false ),stdLink( false,false )));  }


};

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class LineGraph : public LineGraphPar<AlgsDefaultSettings> {};



// Caster zlozony z jednego castera 3-argumentowego i dwoch 2-argumentowych
template <class TwoArg, class FirstArg, class SecondArg> struct ComplexCaster {
    mutable TwoArg twoarg;
    mutable FirstArg firstarg;
    mutable SecondArg secondarg;
    ComplexCaster(TwoArg t=TwoArg(),FirstArg f=FirstArg(),SecondArg s=SecondArg())
        : twoarg(t), firstarg(f), secondarg(s) {}

    // jesli podano oba argumenty, zastosuj twoarg
    template <class InfoDest, class InfoSour1,class InfoSour2>
	void operator()(InfoDest& dest,InfoSour1 sour1,InfoSour2 sour2)
    { twoarg(dest,sour1,sour2);  }

    // jesli podano pierwszy argument, zastosuj firstarg
    template <class InfoDest, class InfoSour1>
	void operator()(InfoDest& dest,InfoSour1 sour1,Koala::BlackHole b)
    { firstarg(dest,sour1);  }

    // jesli podano drugi argument, zastosuj secondarg
    template <class InfoDest, class InfoSour1>
	void operator()(InfoDest& dest,Koala::BlackHole b,InfoSour1 sour2)
    { secondarg(dest,sour2);  }
};

// funkcja tworzaca - podajemy castery skladowe
template <class TwoArg, class FirstArg, class SecondArg>
ComplexCaster<TwoArg, FirstArg,SecondArg> complexCast(TwoArg t,FirstArg f,SecondArg s)
{ return ComplexCaster<TwoArg, FirstArg,SecondArg>(t,f,s); }



// Kreator iloczynow grafow
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class ProductPar {
    public:

//    Dopisuje iloczyn    kartezjanski
//    grafow g1 i g2 do g. Zwraca pierwszy wprowadzony wierzcholek. cast to para casterow 3-argumentowych.
//    cast.first tworzy obiekt info nowego wierzcholka na podstawie dwoch inf wierzcholkow zrodlowych
//    cast.second tworzy obiekt info nowej krawedzi na podstawie dwoch inf krawedzi zrodlowych
//    (w przypadku braku odpowiedniego argumentu w wywolaniu wstawiany jest blackHole)
//    link to para linkerow laczacych elementy grafow zrodlowych z wynikiem
//    link.first.first - przywiazuje do wierzcholka wynikowego wierzcholek pierwszego grafu lub NULL w razie braku
//    link.first.second - przywiazuje do wierzcholka wynikowego wierzcholek drugiego grafu lub NULL w razie braku
//    link.second.first - przywiazuje do krawedzi wynikowej krawedz pierwszego grafu lub NULL w razie braku
//    link.second.second - przywiazuje do krawedzi wynikowej krawedz drugiego grafu lub NULL w razie braku
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster,class VLinker,class ELinker >
    static typename Graph::PVertex cart(const Graph1& g1,const Graph2& g2,Graph& g,
                                            std::pair<VCaster,ECaster> cast,std::pair<VLinker,ELinker> link)
    {   typename Graph::PVertex res=g.getVertLast();
        int n1=g1.getVertNo(),n2=g2.getVertNo();
        for(typename Graph::PVertex v=g.getVert();v;v=g.getVertNext(v))
            { link.first.first()(v,(typename Graph1::PVertex)0);link.first.second()(v,(typename Graph2::PVertex)0); }
        for(typename Graph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            { link.second.first()(e,(typename Graph1::PEdge)0);link.second.second()(e,(typename Graph2::PEdge)0); }
        typename DefaultStructs::template AssocCont<typename Graph1::PVertex,int>:: Type indv1(g1.getVertNo());
        typename DefaultStructs::template AssocCont<typename Graph2::PVertex,int>:: Type indv2(g2.getVertNo());
        int l=0;
        for(typename Graph1::PVertex v=g1.getVert();v;v=g1.getVertNext(v),l++) indv1[v]=l;
        l=0;
        for(typename Graph2::PVertex v=g2.getVert();v;v=g2.getVertNext(v),l++) indv2[v]=l;
        typename Graph::PVertex LOCALARRAY (tabv,n1*n2);

        int l1,l2;
        typename Graph1::PVertex v1;typename Graph2::PVertex v2;
        for(l1=0, v1=g1.getVert();v1;v1=g1.getVertNext(v1),l1++)
            for(l2=0, v2=g2.getVert();v2;v2=g2.getVertNext(v2),l2++)
            {   typename Graph::VertInfoType vinfo;
                cast.first(vinfo,g1.getVertInfo(v1),g2.getVertInfo(v2));
                typename Graph::PVertex v=tabv[n1*l1+l2]=g.addVert(vinfo);
                link.first.first()(v,v1);link.first.second()(v,v2);
            }

        for(l1=0, v1=g1.getVert();v1;v1=g1.getVertNext(v1),l1++)
            for(typename Graph2::PEdge e=g2.getEdge();e;e=g2.getEdgeNext(e))
            {   std::pair<typename Graph2::PVertex,typename Graph2::PVertex> ends=g2.getEdgeEnds(e);
                EdgeDirection dir=g2.getEdgeType(e);
                typename Graph::EdgeInfoType einfo;
                cast.second(einfo,blackHole,g2.getEdgeInfo(e));
                typename Graph::PEdge eres=
                    g.addEdge(
                          tabv[n1*indv1[v1]+indv2[ends.first]],
                          tabv[n1*indv1[v1]+indv2[ends.second]],
                        einfo,dir);
                link.second.first()(eres,(typename Graph1::PEdge)0);
                link.second.second()(eres,e);
            }

        for(l2=0, v2=g2.getVert();v2;v2=g2.getVertNext(v2),l2++)
            for(typename Graph1::PEdge e=g1.getEdge();e;e=g1.getEdgeNext(e))
            {   std::pair<typename Graph1::PVertex,typename Graph1::PVertex> ends=g1.getEdgeEnds(e);
                EdgeDirection dir=g1.getEdgeType(e);
                typename Graph::EdgeInfoType einfo;
                cast.second(einfo,g1.getEdgeInfo(e),blackHole);
                typename Graph::PEdge eres=
                    g.addEdge(
                          tabv[n1*indv1[ends.first]+indv2[v2]],
                          tabv[n1*indv1[ends.second]+indv2[v2]],
                        einfo,dir);
                link.second.first()(eres,e);
                link.second.second()(eres,(typename Graph2::PEdge)0);
            }

        return g.getVertNext(res);
    }

    // j.w. ale bez linkerow
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster >
    static typename Graph::PVertex cart(const Graph1& g1,const Graph2& g2,Graph& g,std::pair<VCaster,ECaster> cast)
    {
        return cart(g1,g2,g,cast,std::make_pair(stdLink(false,false),stdLink(false,false)));
    }

    // j.w. ale tylko tworzy iloczyn
    template <class Graph1,class Graph2,class Graph >
    static typename Graph::PVertex cart(const Graph1& g1,const Graph2& g2,Graph& g)
    {
        return cart(g1,g2,g,std::make_pair(stdCast(false),stdCast(false)),std::make_pair(stdLink(false,false),stdLink(false,false)));
    }


//    Dopisuje iloczyn    leksykograficzny
//    grafow g1 i g2 do g. Zwraca pierwszy wprowadzony wierzcholek. Sens parametrow - j.w.
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster,class VLinker,class ELinker >
    static typename Graph::PVertex lex(const Graph1& g1,const Graph2& g2,Graph& g,
                                            std::pair<VCaster,ECaster> cast,std::pair<VLinker,ELinker> link)
    {   typename Graph::PVertex res=g.getVertLast();
        int n1=g1.getVertNo(),n2=g2.getVertNo();
        for(typename Graph::PVertex v=g.getVert();v;v=g.getVertNext(v))
            { link.first.first()(v,(typename Graph1::PVertex)0);link.first.second()(v,(typename Graph2::PVertex)0); }
        for(typename Graph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            { link.second.first()(e,(typename Graph1::PEdge)0);link.second.second()(e,(typename Graph2::PEdge)0); }
        typename DefaultStructs::template AssocCont<typename Graph1::PVertex,int>:: Type indv1(g1.getVertNo());
        typename DefaultStructs::template AssocCont<typename Graph2::PVertex,int>:: Type indv2(g2.getVertNo());
        int l=0;
        for(typename Graph1::PVertex v=g1.getVert();v;v=g1.getVertNext(v),l++) indv1[v]=l;
        l=0;
        for(typename Graph2::PVertex v=g2.getVert();v;v=g2.getVertNext(v),l++) indv2[v]=l;
        typename Graph::PVertex LOCALARRAY (tabv,n1*n2);

        int l1,l2;
        typename Graph1::PVertex v1;typename Graph2::PVertex v2;
        for(l1=0, v1=g1.getVert();v1;v1=g1.getVertNext(v1),l1++)
            for(l2=0, v2=g2.getVert();v2;v2=g2.getVertNext(v2),l2++)
            {   typename Graph::VertInfoType vinfo;
                cast.first(vinfo,g1.getVertInfo(v1),g2.getVertInfo(v2));
                typename Graph::PVertex v=tabv[n1*l1+l2]=g.addVert(vinfo);
                link.first.first()(v,v1);link.first.second()(v,v2);
            }

        for(l1=0, v1=g1.getVert();v1;v1=g1.getVertNext(v1),l1++)
            for(typename Graph2::PEdge e=g2.getEdge();e;e=g2.getEdgeNext(e))
            {   std::pair<typename Graph2::PVertex,typename Graph2::PVertex> ends=g2.getEdgeEnds(e);
                EdgeDirection dir=g2.getEdgeType(e);
                typename Graph::EdgeInfoType einfo;
                cast.second(einfo,blackHole,g2.getEdgeInfo(e));
                typename Graph::PEdge eres=
                    g.addEdge(
                          tabv[n1*indv1[v1]+indv2[ends.first]],
                          tabv[n1*indv1[v1]+indv2[ends.second]],
                        einfo,dir);
                link.second.first()(eres,(typename Graph1::PEdge)0);
                link.second.second()(eres,e);
            }

        {   typename Graph2::PVertex v1;
            for(typename Graph1::PEdge e=g1.getEdge();e;e=g1.getEdgeNext(e))
            for(l1=0, v1=g2.getVert();v1;v1=g2.getVertNext(v1),l1++)
                for(l2=0, v2=g2.getVert();v2;v2=g2.getVertNext(v2),l2++)
                {   std::pair<typename Graph1::PVertex,typename Graph1::PVertex> ends=g1.getEdgeEnds(e);
                    EdgeDirection dir=g1.getEdgeType(e);
                    if (dir==Loop) dir=(v1==v2) ? EdLoop : EdUndir;
                    typename Graph::EdgeInfoType einfo;
                    cast.second(einfo,g1.getEdgeInfo(e),blackHole);
                    typename Graph::PEdge eres=
                        g.addEdge(
                          tabv[n1*indv1[ends.first]+indv2[v1]],
                          tabv[n1*indv1[ends.second]+indv2[v2]],
                        einfo,dir);
                    link.second.first()(eres,e);
                    link.second.second()(eres,(typename Graph2::PEdge)0);
                }
        }

        return g.getVertNext(res);
    }

    // j.w. ale bez linkerow
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster >
    static typename Graph::PVertex lex(const Graph1& g1,const Graph2& g2,Graph& g,std::pair<VCaster,ECaster> cast)
    {
        return lex(g1,g2,g,cast,std::make_pair(stdLink(false,false),stdLink(false,false)));
    }

    // j.w. ale tylko tworzy iloczyn
    template <class Graph1,class Graph2,class Graph >
    static typename Graph::PVertex lex(const Graph1& g1,const Graph2& g2,Graph& g)
    {
        return lex(g1,g2,g,std::make_pair(stdCast(false),stdCast(false)),std::make_pair(stdLink(false,false),stdLink(false,false)));
    }


//    Dopisuje iloczyn    tensorowy
//    grafow g1 i g2 do g. Zwraca pierwszy wprowadzony wierzcholek. Sens parametrow - j.w.
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster,class VLinker,class ELinker >
    static typename Graph::PVertex tensor(const Graph1& g1,const Graph2& g2,Graph& g,
                                            std::pair<VCaster,ECaster> cast,std::pair<VLinker,ELinker> link)
    {   typename Graph::PVertex res=g.getVertLast();
        int n1=g1.getVertNo(),n2=g2.getVertNo();
        for(typename Graph::PVertex v=g.getVert();v;v=g.getVertNext(v))
            { link.first.first()(v,(typename Graph1::PVertex)0);link.first.second()(v,(typename Graph2::PVertex)0); }
        for(typename Graph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            { link.second.first()(e,(typename Graph1::PEdge)0);link.second.second()(e,(typename Graph2::PEdge)0); }
        typename DefaultStructs::template AssocCont<typename Graph1::PVertex,int>:: Type indv1(g1.getVertNo());
        typename DefaultStructs::template AssocCont<typename Graph2::PVertex,int>:: Type indv2(g2.getVertNo());
        int l=0;
        for(typename Graph1::PVertex v=g1.getVert();v;v=g1.getVertNext(v),l++) indv1[v]=l;
        l=0;
        for(typename Graph2::PVertex v=g2.getVert();v;v=g2.getVertNext(v),l++) indv2[v]=l;
        typename Graph::PVertex LOCALARRAY (tabv,n1*n2);

        int l1,l2;
        typename Graph1::PVertex v1;typename Graph2::PVertex v2;
        for(l1=0, v1=g1.getVert();v1;v1=g1.getVertNext(v1),l1++)
            for(l2=0, v2=g2.getVert();v2;v2=g2.getVertNext(v2),l2++)
            {   typename Graph::VertInfoType vinfo;
                cast.first(vinfo,g1.getVertInfo(v1),g2.getVertInfo(v2));
                typename Graph::PVertex v=tabv[n1*l1+l2]=g.addVert(vinfo);
                link.first.first()(v,v1);link.first.second()(v,v2);
            }


            for(typename Graph1::PEdge e1=g1.getEdge();e1;e1=g1.getEdgeNext(e1))
            for(typename Graph2::PEdge e2=g2.getEdge();e2;e2=g2.getEdgeNext(e2))
                {   std::pair<typename Graph1::PVertex,typename Graph1::PVertex> ends1=g1.getEdgeEnds(e1);
                    std::pair<typename Graph2::PVertex,typename Graph2::PVertex> ends2=g2.getEdgeEnds(e2);
                    EdgeDirection dir1=g1.getEdgeType(e1), dir2=g2.getEdgeType(e2);
                    typename Graph::EdgeInfoType einfo;
                    cast.second(einfo,g1.getEdgeInfo(e1),g2.getEdgeInfo(e2));
                    typename Graph::PEdge eres1=0,eres2=0;
                    if (dir1==Undirected && dir2==Undirected)
                    {   eres1=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo,EdUndir);
                        eres2=
                        g.addEdge(
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo,EdUndir);
                    }
                    if (dir1==Undirected && dir2==Directed)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo);
                        eres2=
                        g.addArch(
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo);
                    }
                    if (dir1==Directed && dir2==Undirected)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo);
                        eres2=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo);
                    }
                    if (dir1==Directed && dir2==Directed)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo);
                    }
                    if (dir1==Undirected && dir2==Loop)
                    {   eres1=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo,EdUndir);
                        eres2=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo,EdUndir);
                    }
                    if (dir1==Loop && dir2==Undirected)
                    {   eres1=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo,EdUndir);
                        eres2=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo,EdUndir);
                    }
                    if (dir1==Loop && dir2==Loop)
                    {   eres1=
                        g.addLoop(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                        einfo);
                    }
                    if (dir1==Directed && dir2==Loop)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo);
                    }
                    if (dir1==Loop && dir2==Directed)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo);
                    }

                    if (eres1) { link.second.first()(eres1,e1); link.second.second()(eres1,e2); }
                    if (eres2) { link.second.first()(eres2,e1); link.second.second()(eres2,e2); }
                }

        return g.getVertNext(res);
    }

    // j.w. ale bez linkerow
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster >
    static typename Graph::PVertex tensor(const Graph1& g1,const Graph2& g2,Graph& g,std::pair<VCaster,ECaster> cast)
    {
        return tensor(g1,g2,g,cast,std::make_pair(stdLink(false,false),stdLink(false,false)));
    }

    // j.w. ale tylko tworzy iloczyn
    template <class Graph1,class Graph2,class Graph >
    static typename Graph::PVertex tensor(const Graph1& g1,const Graph2& g2,Graph& g)
    {
        return tensor(g1,g2,g,std::make_pair(stdCast(false),stdCast(false)),std::make_pair(stdLink(false,false),stdLink(false,false)));
    }


//    Dopisuje strong
//    grafow g1 i g2 do g. Zwraca pierwszy wprowadzony wierzcholek. Sens parametrow - j.w.
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster,class VLinker,class ELinker >
    static typename Graph::PVertex strong(const Graph1& g1,const Graph2& g2,Graph& g,
                                            std::pair<VCaster,ECaster> cast,std::pair<VLinker,ELinker> link)
    {   typename Graph::PVertex res=g.getVertLast();
        int n1=g1.getVertNo(),n2=g2.getVertNo();
        for(typename Graph::PVertex v=g.getVert();v;v=g.getVertNext(v))
            { link.first.first()(v,(typename Graph1::PVertex)0);link.first.second()(v,(typename Graph2::PVertex)0); }
        for(typename Graph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            { link.second.first()(e,(typename Graph1::PEdge)0);link.second.second()(e,(typename Graph2::PEdge)0); }
        typename DefaultStructs::template AssocCont<typename Graph1::PVertex,int>:: Type indv1(g1.getVertNo());
        typename DefaultStructs::template AssocCont<typename Graph2::PVertex,int>:: Type indv2(g2.getVertNo());
        int l=0;
        for(typename Graph1::PVertex v=g1.getVert();v;v=g1.getVertNext(v),l++) indv1[v]=l;
        l=0;
        for(typename Graph2::PVertex v=g2.getVert();v;v=g2.getVertNext(v),l++) indv2[v]=l;
        typename Graph::PVertex LOCALARRAY (tabv,n1*n2);

        int l1,l2;
        typename Graph1::PVertex v1;typename Graph2::PVertex v2;
        for(l1=0, v1=g1.getVert();v1;v1=g1.getVertNext(v1),l1++)
            for(l2=0, v2=g2.getVert();v2;v2=g2.getVertNext(v2),l2++)
            {   typename Graph::VertInfoType vinfo;
                cast.first(vinfo,g1.getVertInfo(v1),g2.getVertInfo(v2));
                typename Graph::PVertex v=tabv[n1*l1+l2]=g.addVert(vinfo);
                link.first.first()(v,v1);link.first.second()(v,v2);
            }


        for(l1=0, v1=g1.getVert();v1;v1=g1.getVertNext(v1),l1++)
            for(typename Graph2::PEdge e=g2.getEdge();e;e=g2.getEdgeNext(e))
            {   std::pair<typename Graph2::PVertex,typename Graph2::PVertex> ends=g2.getEdgeEnds(e);
                EdgeDirection dir=g2.getEdgeType(e);
                typename Graph::EdgeInfoType einfo;
                cast.second(einfo,blackHole,g2.getEdgeInfo(e));
                typename Graph::PEdge eres=
                    g.addEdge(
                          tabv[n1*indv1[v1]+indv2[ends.first]],
                          tabv[n1*indv1[v1]+indv2[ends.second]],
                        einfo,dir);
                link.second.first()(eres,(typename Graph1::PEdge)0);
                link.second.second()(eres,e);
            }

        for(l2=0, v2=g2.getVert();v2;v2=g2.getVertNext(v2),l2++)
            for(typename Graph1::PEdge e=g1.getEdge();e;e=g1.getEdgeNext(e))
            {   std::pair<typename Graph1::PVertex,typename Graph1::PVertex> ends=g1.getEdgeEnds(e);
                EdgeDirection dir=g1.getEdgeType(e);
                typename Graph::EdgeInfoType einfo;
                cast.second(einfo,g1.getEdgeInfo(e),blackHole);
                typename Graph::PEdge eres=
                    g.addEdge(
                          tabv[n1*indv1[ends.first]+indv2[v2]],
                          tabv[n1*indv1[ends.second]+indv2[v2]],
                        einfo,dir);
                link.second.first()(eres,e);
                link.second.second()(eres,(typename Graph2::PEdge)0);
            }

            for(typename Graph1::PEdge e1=g1.getEdge();e1;e1=g1.getEdgeNext(e1))
            for(typename Graph2::PEdge e2=g2.getEdge();e2;e2=g2.getEdgeNext(e2))
                {   std::pair<typename Graph1::PVertex,typename Graph1::PVertex> ends1=g1.getEdgeEnds(e1);
                    std::pair<typename Graph2::PVertex,typename Graph2::PVertex> ends2=g2.getEdgeEnds(e2);
                    EdgeDirection dir1=g1.getEdgeType(e1), dir2=g2.getEdgeType(e2);
                    typename Graph::EdgeInfoType einfo;
                    cast.second(einfo,g1.getEdgeInfo(e1),g2.getEdgeInfo(e2));
                    typename Graph::PEdge eres1=0,eres2=0;
                    if (dir1==Undirected && dir2==Undirected)
                    {   eres1=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo,EdUndir);
                        eres2=
                        g.addEdge(
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo,EdUndir);
                    }
                    if (dir1==Undirected && dir2==Directed)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo);
                        eres2=
                        g.addArch(
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo);
                    }
                    if (dir1==Directed && dir2==Undirected)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo);
                        eres2=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo);
                    }
                    if (dir1==Directed && dir2==Directed)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.second]],
                        einfo);
                    }
                    if (dir1==Undirected && dir2==Loop)
                    {   eres1=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo,EdUndir);
                        eres2=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo,EdUndir);
                    }
                    if (dir1==Loop && dir2==Undirected)
                    {   eres1=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo,EdUndir);
                        eres2=
                        g.addEdge(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo,EdUndir);
                    }
                    if (dir1==Loop && dir2==Loop)
                    {   eres1=
                        g.addLoop(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                        einfo);
                    }
                    if (dir1==Directed && dir2==Loop)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.second]+indv2[ends2.first]],
                        einfo);
                    }
                    if (dir1==Loop && dir2==Directed)
                    {   eres1=
                        g.addArch(
                          tabv[n1*indv1[ends1.first]+indv2[ends2.first]],
                          tabv[n1*indv1[ends1.first]+indv2[ends2.second]],
                        einfo);
                    }

                    if (eres1) { link.second.first()(eres1,e1); link.second.second()(eres1,e2); }
                    if (eres2) { link.second.first()(eres2,e1); link.second.second()(eres2,e2); }
                }

        return g.getVertNext(res);
    }

    // j.w. ale bez linkerow
    template <class Graph1,class Graph2,class Graph,class VCaster,class ECaster >
    static typename Graph::PVertex strong(const Graph1& g1,const Graph2& g2,Graph& g,std::pair<VCaster,ECaster> cast)
    {
        return strong(g1,g2,g,cast,std::make_pair(stdLink(false,false),stdLink(false,false)));
    }

    // j.w. ale tylko tworzy iloczyn
    template <class Graph1,class Graph2,class Graph >
    static typename Graph::PVertex strong(const Graph1& g1,const Graph2& g2,Graph& g)
    {
        return strong(g1,g2,g,std::make_pair(stdCast(false),stdCast(false)),std::make_pair(stdLink(false,false),stdLink(false,false)));
    }


};

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class Product : public ProductPar<AlgsDefaultSettings> {};

}
#endif
