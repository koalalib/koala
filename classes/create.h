#ifndef KOALA_CREATOR_H
#define KOALA_CREATOR_H

#include "..\graph\graph.h"
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


		/* The function generates a random graph on n vertices according to Erdos�Renyi model G(n, p).
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


		/* The function generates a random graph on n vertices according to Erdos�Renyi model G(n, p).
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


		/* The function generates a random graph on n vertices according to Erdos�Renyi model G(n, m).
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


		/* The function generates a random graph on n vertices according to Erdos�Renyi model G(n, m).
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

}
#endif
