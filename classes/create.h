#ifndef KOALA_CREATOR_H
#define KOALA_CREATOR_H

#include <cstdlib>
#include <ctime>

#include "../base/exception.h"
#include "../base/rnd.h"
#include "../graph/graph.h"
#include "../algorithm/weights.h"
#include "../container/hashcont.h"

namespace Koala
{

namespace Privates {

        template< class A,class B> class StdMapWithDefault : public std::map<A,B>
        {
            B def;
            public:
            StdMapWithDefault(int arg,B _def) : def(_def) {}

            B& operator[](A key)
            {
//                if (this->find(key)==this->end()) return std::map<A,B>::operator[](key)=def;
//                return std::map<A,B>::operator[](key);
                return this->insert(std::make_pair(key,def)).first->second;
            }
        };
}

	/** \brief Create graphs.
	 *
	 *  The utility class for creating various types of graphs. Created graphs are added to graph as a new component.
	 *  Most of the methods return a pointer indicating the first vertex that has been created.
	 *  Common variables used in edge/vertex generators:
	 *  - g the input/output graph to which the new-created graph is added.
	 *  - num, num1, num2 are integers, which indicate numbers of vertices.
	 *  - dir is of EdgeDirection type and indicate the type and direction of corresponding edge.
	 *  - vInfoGen - the generator for info objects for vertices. It is used in form: vInfoGen(num). Indexes start with 0.
	 *  - eInfoGen - the generator for info objects for edges. It is used in form: eInfoGen(num1, num2, dir). Indexes start with 0.
	 *  \ingroup detect
	 *
	 *  [See example](examples/create/example_Creator.html).*/
	class Creator{
	public:

		/** \brief Create empty graph.
		*
		*  The method generates an empty graph.
		*
		*  \param g  the input/output graph,
		*  \param n the number of vertices to create,
		*  \param vInfoGen  the vertex info generator. The method uses it in form: vInfoGen(num),
		*  \retrun the pointer to the first added vertex.*/
		template< class GraphType, class VInfoGen >
		static typename GraphType::PVertex empty(GraphType &g, int n, VInfoGen vInfoGen);

		/** \brief Create empty graph.
		*
		*  The function generates an empty graph.
		*
		*  \param g  the input/output graph,
		*  \param n  the number of vertices to create,
		*  \retrun the pointer to the first added vertex.*/
		template< class GraphType >
		static typename GraphType::PVertex empty(GraphType &g, int n);

		/** \brief Create clique.
		 *
		 *  The method generates and adds a clique, i.e. for every two vertices, there is created a connection
		 *  according to Koala::EdgeDirection mask. Additionally if the mask contains a loop then a loop is attached to each vertex.
		 *
		 *  \param g  the input/output graph,
		 *  \param n  number of vertices to create,
		 *  \param vInfoGen  generator for info objects for vertices used in form: vInfoGen(num),
		 *  \param eInfoGen  generator for info objects for edges used in form: eInfoGen(num1, num2, dir),
		 *  \param dir Koala::EdgeDirection mask.
		 *  \retrun the pointer to the first added vertex.*/
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex clique( GraphType &g, int n,  VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection dir = EdUndir );

		/** \brief Create clique
		 *
		 *  The function generates a clique, i.e. for every two vertices, there is created a connection
		 *  according to EdgeDirection mask. Additionally, if the mask contains a loop then a loop is attached
		 *  to each vertex. Info objects for vertices and edges are empty.
		 *
		 *  \param g  the input/output graph,
		 *  \param n  number of vertices to create,
		 *  \param dir  edges direction mask.
		 *  \retrun the pointer to the first added vertex.*/
		template< class GraphType >
			static typename GraphType::PVertex clique( GraphType &g, int n, EdgeDirection dir = EdUndir );

		/** \brief Create path.
		 *
		 *  The function generates a path. Additionally, if the mask contains a loop, then a loop is attached
		 *  to each vertex. The following connections are created: 0-1, 1-2, 2-3,..., (n-2)-(n-1), where numbers represent vertices.
		 *  \param g the input/output graph,
		 *  \param n  number of vertices to create,
		 *  \param vInfoGen  generator for info objects for vertices,
		 *  \param  eInfoGen  generator for info objects for edges,
		 *  \param dir edges direction mask.
		 *  \retrun the pointer to the first added vertex.*/
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex path( GraphType &g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection dir = EdUndir );

		/* It is a simpler version of the above function*/
		/** \brief Create path.
		 *
		 *  The function generates a path. Additionally, if the mask contains a loop, then a loop is attached
		 *  to each vertex. The following connections are created: 0-1, 1-2, 2-3,..., (n-2)-(n-1), where numbers represent vertices.
		 *  \param g the input/output graph,
		 *  \param n  number of vertices to create,
		 *  \param dir - edges direction mask.
		 *  \retrun the pointer to the first added vertex. */
		template< class GraphType >
			static typename GraphType::PVertex path( GraphType &g, int n, EdgeDirection dir = EdUndir );

		/** \brief Create cycle.
		 *
		 *  The function generates a cycle. Additionally, if the mask contains a loop, then a loop is attached
		 *  to each vertex. The following connections are created: 0-1, 1-2, 2-3,..., (n-2)-(n-1), (n-1)-0, where the numbers represent vertices.
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param dir - Koala::EdgesDirection mask. However for n==1 each mask EdUndir, EdDirIn, EdDirOut generate loop.
		 *  \retrun the pointer to the first added vertex.*/
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex cycle( GraphType &g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection dir = EdUndir );

		/* It is a simpler version of the above function*/
		/** \brief Create cycle.
		 *
		 *  The function generates a cycle. Additionally, if the mask contains a loop, then a loop is attached
		 *  to each vertex. The following connections are created: 0-1, 1-2, 2-3,..., (n-2)-(n-1), (n-1)-0, where the numbers represent vertices.
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param dir - Koala::EdgesDirection mask. However, for n==1 each mask EdUndir, EdDirIn, EdDirOut generate loop.
		 *  \retrun the pointer to the first added vertex. */
		template< class GraphType >
			static typename GraphType::PVertex cycle( GraphType &g, int n, EdgeDirection dir = EdUndir );

		/* Central vertex has number 0, the rest of vertices is numbered from 1 to n-1.
		The following connections are created:
		 - connections of the central vertex: 0-1, 0-2, 0-(n-2), 0-(n-1),
		 - connections of non-central vertices: 1-2, 2-3,..., (n-2)-(n-1).
		Numbers represent vertices.*/
		/** \brief Create fan.
		 *
		 *  A fan graph F<sub>n</sub> is defined as the graph  N<sub>1</sub>+ P<sub>n-1</sub>,
		 *  where N<sub>1</sub> is the empty graph containing one vertex and P<sub>n-1</sub> is the path graph on n-1 vertices.
		 *  The function generates an usual fan graph containing n vertices.
		 *
		 *  The central vertex is created as the first vertex.  
		 *  The following connections are created:
		 *  - connections of the central vertex: 0-1, 0-2, 0-(n-2), 0-(n-1),
		 *  - connections of non-central vertices: 1-2, 2-3,..., (n-2)-(n-1).
		 *
		 *  where numbers represent vertices.
		 * 
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param n - the number of vertices to create,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param centerDir - direction mask for edges connected to central vertex.
		 *  \param borderDir - direction mask for edges between border vertices.
		 *  \return the pointer that indicates the first (and also central) vertex.*/
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex fan( GraphType &g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection centerDir, EdgeDirection borderDir );

		/** \brief Create fan.
		 *
		 *  A fan graph F<sub>n</sub> is defined as the graph  N<sub>1</sub>+ P<sub>n-1</sub>,
		 *  where N<sub>1</sub> is the empty graph containing one vertex and P<sub>n-1</sub> is the path graph on n-1 vertices.
		 *  The function generates an usual fan graph containing n vertices.
		 *
		 *  The central vertex is created as the first vertex.  
		 *  The following connections are created:
		 *  - connections of the central vertex: 0-1, 0-2, 0-(n-2), 0-(n-1),
		 *  - connections of non-central vertices: 1-2, 2-3,..., (n-2)-(n-1).
		 *
		 *  where numbers represent vertices.
		 * 
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param  n - number of vertices to create,
		 *  \param  vInfoGen - generator for info objects for vertices,
		 *  \param  eInfoGen - generator for info objects for edges,
		 *  \param  dir - edges direction mask.
		 *  \return the pointer that indicates the first (and also central) vertex.*/
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex fan( GraphType &g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection dir = EdUndir );

		/* It is a simpler version of the above function*/
		/** \brief Create fan.
		 *
		 *  A fan graph F<sub>n</sub> is defined as the graph  N<sub>1</sub>+ P<sub>n-1</sub>,
		 *  where N<sub>1</sub> is the empty graph containing one vertex and P<sub>n-1</sub> is the path graph on n-1 vertices.
		 *  The function generates an usual fan graph containing n vertices.
		 *
		 *  The central vertex is created as the first vertex.  
		 *  The following connections are created:
		 *  - connections of the central vertex: 0-1, 0-2, 0-(n-2), 0-(n-1),
		 *  - connections of non-central vertices: 1-2, 2-3,..., (n-2)-(n-1).
		 *
		 *  where numbers represent vertices.
		 * 
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param dir - edges direction mask.
		 *  \return the pointer that indicates the first (and also central) vertex. */
		template< class GraphType >
			static typename GraphType::PVertex fan( GraphType &g, int n, EdgeDirection dir = EdUndir );

		/** \brief Create wheel.
		 *
		 *  The function generates a wheel. A wheel graph W<sub>n</sub> is a graph with \a n vertices,
		 *  formed by connecting a single vertex to all vertices of an (n-1)-cycle. The central vertex is created as the first vertex.
		 *
		 *  The following connections are created:
		 *  - connections of the central vertex: 0-1, 0-2, 0-(n-2), 0-(n-1),
		 *  - connections of non-central vertices: 1-2, 2-3,..., (n-2)-(n-1), (n-1)-0.
		 *
		 *  Where 0 represents the central vertex and the remaining numbers stand for the other vertices.
		 *
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param  n - the number of vertices to create,
		 *  \param  vInfoGen - generator for info objects for vertices,
		 *  \param  eInfoGen - generator for info objects for edges,
		 *  \param  centerDir - direction mask for edges connected to central vertex,
		 *  \param  borderDir - direction mask for edges between border vertices.
		 *  \return the pointer that indicates the first (and also central) vertex. */
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex wheel( GraphType &g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection centerDir, EdgeDirection borderDir );

		/** \brief Create wheel.
		 *
		 *  The function generates a wheel. A wheel graph W<sub>n</sub> is a graph with \a n vertices,
		 *  formed by connecting a single vertex to all vertices of an (n-1)-cycle. The central vertex is created as the first vertex.
		 *
		 *  The following connections are created:
		 *  - connections of the central vertex: 0-1, 0-2, 0-(n-2), 0-(n-1),
		 *  - connections of non-central vertices: 1-2, 2-3,..., (n-2)-(n-1), (n-1)-0.
		 *
		 *  Where 0 represents the central vertex and the remaining numbers stand for the other vertices.
		 *
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param dir - edges direction mask.
		 *  \return the pointer that indicates the first (and also central) vertex.  */
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex wheel( GraphType &g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection dir = EdUndir);

		/* It is a simpler version of the above function*/
		/** \brief Create wheel.
		 *
		 *  The function generates a wheel. A wheel graph W<sub>n</sub> is a graph with \a n vertices,
		 *  formed by connecting a single vertex to all vertices of an (n-1)-cycle. The central vertex is created as the first vertex.
		 *
		 *  The following connections are created:
		 *  - connections of the central vertex: 0-1, 0-2, 0-(n-2), 0-(n-1),
		 *  - connections of non-central vertices: 1-2, 2-3,..., (n-2)-(n-1), (n-1)-0.
		 *
		 *  Where 0 represents the central vertex and the remaining numbers stand for the other vertices.
		 *
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param dir - edges direction mask.
		 *  \return the pointer that indicates the first (and also central) vertex. */
		template< class GraphType >
			static typename GraphType::PVertex wheel( GraphType &g, int n, EdgeDirection dir = EdUndir );

		/** \brief Create complete bipartite graph.
		 *
		 *  The function generates a complete bipartite graph K<sub>n1,n2</sub>. 
		 *  Vertices in the first partition are numbered form 0 to (n1-1), vertices from the second partition are numbered form n1 to (n1+n2-1).
		 *  All vertices from the first partition are connected to all vertices from the second partition.
		 *
		 *  Additionally, if the mask contains a loop then, a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param n1 - number of vertices to create in the first partition,
		 *  \param n1 - number of vertices to create in the second partition,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param dir - edges direction mask.
		 *  \return a pair of pointers. The pointers indicate the first vertices in the partitions.  */
		template< class GraphType, class VInfoGen, class EInfoGen >
			static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
			compBipartite( GraphType &g, int n1, int n2, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir );

		/* It is a simpler version of the above function*/
		/** \brief Create complete bipartite graph.
		 *
		 *  The function generates a complete bipartite graph K<sub>n1,n2</sub>. 
		 *  Vertices in the first partition are numbered form 0 to (n1-1), vertices from the second partition are numbered form n1 to (n1+n2-1).
		 *  All vertices from the first partition are connected to all vertices from the second partition.
		 *
		 *  Additionally, if the mask contains a loop then, a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param n1 - number of vertices to create in the first partition,
		 *  \param n1 - number of vertices to create in the second partition,
		 *  \param dir - edges direction mask.
		 *  \return a pair of pointers. The pointers indicate the first vertices in the partitions. */
		template< class GraphType >
			static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
			compBipartite( GraphType &g, int n1, int n2, EdgeDirection dir = EdUndir );

		/** \brief Create complete K-partite graph.
		 *
		 *  The function generates a complete K-partite graph. Each of K partitions has the same number of vertices
		 *  specified by the input parameter k. 
		 *
		 *  Vertices in the first partition are numbered form 0 to (k-1),\n
		 *  vertices from the second partition are numbered form k to (2k-1),\n
		 *  vertices form k-th partition are number from (K-1)*k to (K*k-1).\n
		 *  All vertices from a particular partition are connected to all vertices from other partitions.
		 *
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g the input/output graph,
		 *  \param K number of partition,
		 *  \param k number of vertices to create in each of the K partitions,
		 *  \param[out] itIns insert iterator, the function uses it to return K pairs of pointers; the first pointer in each pair indicate the first
		 *    vertex in a particular partition of the created graph, the second pointer indicates the last vertex in the partition,
		 *  \param vInfoGen generator for info objects for vertices,
		 *  \param  eInfoGen generator for info objects for edges,
		 *  \param dir edges direction mask.
		 *  \retrun the pointer to the first added vertex.*/
		template< class GraphType, class  VInfoGen, class  EInfoGen, class IterInsert>
			static typename GraphType::PVertex compKPartite( GraphType &g, int K, int k, IterInsert itIns, VInfoGen vInfoGen,
				EInfoGen eInfoGen, EdgeDirection dir = EdUndir);

		/* It is a simpler version of the above function*/
		/** \brief Create complete K-partite graph.
		 *
		 *  The function generates a complete K-partite graph. Each of K partitions has the same number of vertices
		 *  specified by the input parameter k. 
		 *
		 *  Vertices in the first partition are numbered form 0 to (k-1),\n
		 *  vertices from the second partition are numbered form k to (2k-1),\n
		 *  vertices form k-th partition are number from (K-1)*k to (K*k-1).\n
		 *  All vertices from a particular partition are connected to all vertices from other partitions.
		 *
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g the input/output graph,
		 *  \param K number of partition,
		 *  \param k number of vertices to create in each of the K partitions,
		 *  \param[out] itIns insert iterator, the function uses it to return K pairs of pointers; the first pointer in each pair indicate the first
		 *    vertex in a particular partition of the created graph, the second pointer indicates the last vertex in the partition,
		 *  \param dir edges direction mask.
		 *  \retrun the pointer to the first added vertex. */
		template< class GraphType, class IterInsert >
			static typename GraphType::PVertex compKPartite( GraphType &g, int K, int k, IterInsert itIns,
				EdgeDirection dir = EdUndir );

		/** \brief Create complete K-partite graph.
		 *
		 *  The function generates a complete K-partite graph. The cardinalities of partitions are specified by a sequence of integers.
		 *  The sequence is defined by two iterators: \a begin and \a end that are passed to the function as input parameters.
		 *
		 *  Vertices are created in the order determined by begin iterator, i.e.\n
		 *  vertices in the first partition are numbered from 0 to (k_1-1),\n
		 *  vertices in the second partition are numbered from k_1 to (k_1+k_2-1),\n
		 *  vertices in K-th partition are numbered from k_1+k_2+...+k_{K-1} to (k_1+k_2+...+k_{K-1}+k_K-1),\n
		 *  where k_i is the number of vertices in i-th partition.\n
		 *  All vertices from a particular partition are connected to all vertices from other partitions.
		 *
		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g the input/output graph,
		 *  \param begin input iterator; it should indicate the integer that corresponds to the cardinality of the first partition,
		 *  \param end iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
		 *    in the container (similar to stl vector::end() method),
		 *  \param[out] itIns insert iterator, the function uses it to return K pairs of pointers; the first pointer in each pair indicate the first
		 *    vertex in a particular partition of the created graph, the second pointer indicates the last vertex in the partition,
		 *  \param vInfoGen generator for info objects for vertices,
		 *  \param eInfoGen generator for info objects for edges,
		 *  \param dir edges direction mask.
		 *  \retrun the pointer to the first added vertex.*/
		template< class GraphType, class VInfoGen, class EInfoGen, class Iter, class IterInsert >
			static typename GraphType::PVertex compKPartite( GraphType &g, Iter begin, Iter end, IterInsert itIns,
				VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir = EdUndir );

		/* It is a simpler version of the above function*/
		/** \brief Create complete K-partite graph.
		 *
		 *  The function generates a complete K-partite graph. The cardinalities of partitions are specified by a sequence of integers.
		 *  The sequence is defined by two iterators: \a begin and \a end that are passed to the function as input parameters.
		 *
		 *  Vertices are created in the order determined by begin iterator, i.e.\n
		 *  vertices in the first partition are numbered from 0 to (k_1-1),\n
		 *  vertices in the second partition are numbered from k_1 to (k_1+k_2-1),\n
		 *  vertices in K-th partition are numbered from k_1+k_2+...+k_{K-1} to (k_1+k_2+...+k_{K-1}+k_K-1),\n
		 *  where k_i is the number of vertices in i-th partition.\n
		 *  All vertices from a particular partition are connected to all vertices from other partitions.
		 *
		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g the input/output graph,
		 *  \param begin input iterator; it should indicate the integer that corresponds to the cardinality of the first partition,
		 *  \param end iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
		 *    in the container (similar to stl vector::end() method),
		 *  \param[out] itIns insert iterator, the function uses it to return K pairs of pointers; the first pointer in each pair indicate the first
		 *    vertex in a particular partition of the created graph, the second pointer indicates the last vertex in the partition,
		 *  \param dir edges direction mask.
		 *  \retrun the pointer to the first added vertex. */
		template< class GraphType, class Iter, class IterInsert > static typename GraphType::PVertex
			compKPartite( GraphType &g, Iter begin, Iter end, IterInsert itIns, EdgeDirection dir = EdUndir );

		/** \brief Create Petersen graph.
		 *
		 *  The function generates the Petersen graph, i.e., it creates the following undirected edges:\n
		 *   0 - 1, 1 - 2, 2 - 3, 3 - 4, 4 - 0,\n
		 *   5 - 6, 6 - 7, 7 - 8, 8 - 9, 9 - 5,\n
		 *   0 - 8, 1 - 6, 2 - 9, 3 - 7, 4 - 5.\n
		 *  Where the numbers represent vertices.
		 *  \param g - the input/output graph,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges.
		 *  \retrun the pointer to the first added vertex.*/
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex petersen( GraphType &g, VInfoGen vInfoGen, EInfoGen eInfoGen );

		/* It is a simpler version of the above function*/
		/** \brief Create Petersen graph.
		 *
		 *  The function generates the Petersen graph, i.e., it creates the following undirected edges:\n
		 *   0 - 1, 1 - 2, 2 - 3, 3 - 4, 4 - 0,\n
		 *   5 - 6, 6 - 7, 7 - 8, 8 - 9, 9 - 5,\n
		 *   0 - 8, 1 - 6, 2 - 9, 3 - 7, 4 - 5.\n
		 *  Where the numbers represent vertices.
		 *  \param g - the input/output graph,
		 *  \retrun the pointer to the first added vertex. */
		template< class GraphType > static typename GraphType::PVertex petersen( GraphType &g );

		/** \brief Create regular tree.
		 *
		 *  The function generates a tree in which each non-leaf vertex has the same degree specified by the parameter \a deg.
		 *  The height of the tree is defined by the parameter \a height. 
		 *
		 *  Vertices are created in order from the root through all vertices on a particular level to leaves, i.e.:
		 *  - root has number 0,
		 *  - children of the root are numbered from 1 to deg,
		 *  - grandchildren are numbered from (deg+1) to (deg+deg^2),
		 *  - vertices on next levels are numbered analogically.
		 *  
		 *  Edges:
		 *  - root is connected to vertices from 1 to deg,
		 *  - vertex number 1 is connected to vertices from (deg+1) to (2*deg),
		 *  - vertex number 2 is connected to vertices from (2*deg+1) to (3*deg),
		 *  - vertex number deg is connected to vertices from (deg*deg+1) to (deg*(deg+1)),
		 *  - connections on next levels are created analogically.
		 *
		 *  For example for a tree with height = 2 and deg = 3 the structure is as follows:
		 *  - root vertex: 0,
		 *  - vertices of heigh = 1: 1, 2, 3,
		 *  - vertices of heigh = 2: 4, 5, 6, 7, 8, 9, 10, 11, 12.
		 *  
		 *  Edges:
		 *  - 0-1, 0-2, 0-3,
		 *  - 1-4, 1-5, 1-6, 2-7, 2-8, 2-9, 3-10, 3-11, 3-12.
		 *  
		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param deg - degree of non-leaf vertices
		 *  \param height - the height of the tree, height = 0 means that there is only the root,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param dir - edges direction mask.
		 *  \return a pointer that indicates the root. */
		template< class GraphType, class VInfoGen, class EInfoGen>
			static typename GraphType::PVertex regTree( GraphType &g, int deg, int height, VInfoGen vInfoGen,
				EInfoGen eInfoGen, EdgeDirection dir = EdUndir);

		/* It is a simpler version of the above function*/
		/** \brief Create regular tree.
		 *
		 *  The function generates a tree in which each non-leaf vertex has the same degree specified by the parameter \a deg.
		 *  The height of the tree is defined by the parameter \a height. 
		 *
		 *  Vertices are created in order from the root through all vertices on a particular level to leaves, i.e.:
		 *  - root has number 0,
		 *  - children of the root are numbered from 1 to deg,
		 *  - grandchildren are numbered from (deg+1) to (deg+deg^2),
		 *  - vertices on next levels are numbered analogically.
		 *  
		 *  Edges:
		 *  - root is connected to vertices from 1 to deg,
		 *  - vertex number 1 is connected to vertices from (deg+1) to (2*deg),
		 *  - vertex number 2 is connected to vertices from (2*deg+1) to (3*deg),
		 *  - vertex number deg is connected to vertices from (deg*deg+1) to (deg*(deg+1)),
		 *  - connections on next levels are created analogically.
		 *
		 *  For example for a tree with height = 2 and deg = 3 the structure is as follows:
		 *  - root vertex: 0,
		 *  - vertices of height = 1: 1, 2, 3,
		 *  - vertices of height = 2: 4, 5, 6, 7, 8, 9, 10, 11, 12.
		 *  
		 *  Edges:
		 *  - 0-1, 0-2, 0-3,
		 *  - 1-4, 1-5, 1-6, 2-7, 2-8, 2-9, 3-10, 3-11, 3-12.
		 *  
		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param deg - degree of non-leaf vertices
		 *  \param height - the height of the tree, height = 0 means that there is only the root,
		 *  \param dir - edges direction mask.
		 *  \return the pointer that indicates the root. */
		template< class GraphType > static typename GraphType::PVertex
			regTree( GraphType &g, int deg, int height, EdgeDirection dir = EdUndir );

		/** \brief Create regular tree.
		 *
		 *  The function generates a tree in which each non-leaf vertex on the same level has the same degree.
		 *  The degree of vertices on the particular level is specified by the input sequence of integers.
		 *  The sequence is defined by two iterators: \a begin and \a end that are passed to the function as input parameters.
		 *  The first integer in the sequence corresponds to degree of the root, next element corresponds to the degree of
		 *  children of the root, etc.
		 *
		 *  Vertices are created in order from the root through all vertices on a particular level to leaves, i.e.:
		 *  - root has number 0,
		 *  - children of the root are numbered from 1 to deg_0,
		 *  - grandchildren are numbered from (deg_0+1) to (deg_0+deg_0*deg_1), where deg_i is the degree of vertices of height i,
		 *  - vertices on next levels are numbered analogically.
		 *  
		 *  Edges:
		 *  - root is connected to vertices from 1 to deg0,
		 *  - vertex number 1 is connected to vertices from (deg0+1) to (deg0+deg1),
		 *  - vertex number 2 is connected to vertices from (deg0+deg1+1) to (deg0+2*deg1),
		 *  - vertex number deg is connected to vertices from (deg0+(deg0-1)*deg1+1) to (deg0+deg0*deg1),
		 *  - connections on next levels are created analogically.
		 *
		 *  For example for a tree with height = 2 and deg0 = 2, deg1=3 the structure is as follows:
		 *  - root vertex: 0,
		 *  - vertices of height = 1: 1, 2,
		 *  - vertices of height = 2: 3, 4, 5, 6, 7, 8.
		 *
		 *  Edges:
		 *  - 0-1, 0-2,
		 *  - 1-3, 1-4, 1-5, 2-6, 2-7, 2-8.
		 *
 		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param begin - input iterator; it should indicate the integer that corresponds to the degree of the root of the tree,
		 *  \param end - iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
		 *    in the container (similar to stl vector::end() method),
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param dir - edges direction mask.
		 *  \return the pointer that indicates the root. */
		template< class GraphType, class VInfoGen, class EInfoGen, class Iter >
			static typename GraphType::PVertex regTree( GraphType &g, Iter begin, Iter end, VInfoGen vInfoGen,
				EInfoGen eInfoGen, EdgeDirection dir = EdUndir);

		/* It is a simpler version of the above function*/
		/** \brief Create regular tree.
		 *
		 *  The function generates a tree in which each non-leaf vertex on the same level has the same degree.
		 *  The degree of vertices on the particular level is specified by the input sequence of integers.
		 *  The sequence is defined by two iterators: \a begin and \a end that are passed to the function as input parameters.
		 *  The first integer in the sequence corresponds to degree of the root, next element corresponds to the degree of
		 *  children of the root, etc.
		 *
		 *  Vertices are created in order from the root through all vertices on a particular level to leaves, i.e.:
		 *  - root has number 0,
		 *  - children of the root are numbered from 1 to deg_0,
		 *  - grandchildren are numbered from (deg_0+1) to (deg_0+deg_0*deg_1), where deg_i is the degree of vertices of height i,
		 *  - vertices on next levels are numbered analogically.
		 *  
		 *  Edges:
		 *  - root is connected to vertices from 1 to deg0,
		 *  - vertex number 1 is connected to vertices from (deg0+1) to (deg0+deg1),
		 *  - vertex number 2 is connected to vertices from (deg0+deg1+1) to (deg0+2*deg1),
		 *  - vertex number deg is connected to vertices from (deg0+(deg0-1)*deg1+1) to (deg0+deg0*deg1),
		 *  - connections on next levels are created analogically.
		 *
		 *  For example for a tree with height = 2 and deg0 = 2, deg1=3 the structure is as follows:
		 *  - root vertex: 0,
		 *  - vertices of height = 1: 1, 2,
		 *  - vertices of height = 2: 3, 4, 5, 6, 7, 8.
		 *
		 *  Edges:
		 *  - 0-1, 0-2,
		 *  - 1-3, 1-4, 1-5, 2-6, 2-7, 2-8.
		 *
 		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param begin - input iterator; it should indicate the integer that corresponds to the degree of the root of the tree,
		 *  \param end - iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
		 *    in the container (similar to stl vector::end() method),
		 *  \param dir - edges direction mask.
		 *  \return the pointer that indicates the root.
		 */
		template< class GraphType, class Iter >
			static typename GraphType::PVertex regTree( GraphType &g, Iter begin, Iter end, EdgeDirection dir = EdUndir );


		/* WEN: First, vertices on the central path are created and numbered from 0 to (pathVertNum-1),
		where pathVertNum is the number of vertices on the central path. Next, legs are created so that:
		- first are all legs that should be connected to the first vertex on the central path are created,
		- next vertices connected to the second vertex on the central path are crated,
		- finally all vertex connected to the last vertex on the central path are created.

		For example for caterpillar having pathVertNum = 3 and legNum = [2,3,4] the structure is as follows:
		- vertices on the central path: 0, 1, 2,
		- legs: 3, 4, 5, 6, 7, 8, 9, 10, 11.
		Edges:
		- central path: 0-1, 1-2,
		- legs: 0-3, 0-4, 1-5, 1-6, 1-7, 2-8, 2-9, 2-10, 2-11.
		*/
		/** \brief Create caterpillar.
		 *
		 *  The function generates a caterpillar. "A caterpillar is a tree in which every graph vertex
		 *  is on a central stalk or only one graph edge away from the stalk
		 *  (in other words, removal of its endpoints leaves a path graph..." (based on http://mathworld.wolfram.com/Caterpillar.html)
		 *  The graph is created in two phases. In the first phase a central path is created. In the next phase
		 *  (leaves or legs) are created and attached to vertices on the path. The number of legs for each vertex
		 *  on the path is specified by the input sequence of integers.
		 *  The sequence is defined by two iterators: \a begin and \a end that are passed to the function as input parameters.
		 *  The first integer in the sequence corresponds to the number of legs of the first vertex on the central path,
		 *  next element corresponds to the number of legs for the next vertex and so on.
		 *  Additionally, if the mask contains a loop, then a loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param  begin - input iterator; it should indicate the integer that describe the number of legs for the first vertex on the central path,
		 *  \param  end - iterator that should indicate the end of the sequence of integers, i.e., it should indicate the past-the-end element
		 *    in the container (similar to stl vector::end() method),
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param  eInfoGen - generator for info objects for edges,
		 *  \param  pathDir - direction mask for edges that form the central path,
		 *  \param  legDir - direction mask for edges that correspond to legs (leaves) of the caterpillar.
		 *  \returns the pointer that indicates the first vertex on the central path.
		 */
		template< class GraphType, class VInfoGen, class EInfoGen, class Iter >
			static typename GraphType::PVertex caterpillar( GraphType &g, Iter begin, Iter end, VInfoGen vInfoGen,
				EInfoGen eInfoGen, EdgeDirection pathDir, EdgeDirection legDir);

		/* WEN: First, vertices on the central path are created and numbered from 0 to (pathVertNum-1),
		where pathVertNum is the number of vertices on the central path. Next, (pathVertNum*legNum) legs are created so that:
		- first alegNum legs that should be connected to the first vertex on the central path are created,
		- next vertices connected to the second vertex on the central path are crated,
		- finally all vertex connected to the last vertex on the central path are created.

		For example for caterpillar having pathVertNum = 3 and legNum = 3 the structure is as follows:
		- vertices on the central path: 0, 1, 2,
		- legs: 3, 4, 5, 6, 7, 8, 9, 10, 11.
		Edges:
		- central path: 0-1, 1-2,
		- legs: 0-3, 0-4, 0-5, 1-6, 1-7, 1-8, 2-9, 2-10, 2-11.
		*/
		/** \brief Create caterpillar.
		 *
		 *  The function generates a caterpillar. "A caterpillar is a tree in which every graph vertex
		 *  is on a central stalk or only one graph edge away from the stalk
		 *  (in other words, removal of its endpoints leaves a path graph..." (based on http://mathworld.wolfram.com/Caterpillar.html)
		 *  The graph is created in two phases. In the first phase a central path is created. In the next phase
		 *  (leaves or legs) are created and attached to vertices on the path. The number of legs for each vertex
		 *  on the path is specified by the parameter \a legNnm.
		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param pathVertNum - number of vertices on the central path,
		 *  \param  legNnm - number of legs that should be attached to each vertex on the central path,
		 *  \param  vInfoGen - generator for info objects for vertices,
		 *  \param  eInfoGen - generator for info objects for edges,
		 *  \param  pathDir - direction mask for edges that form the central path,
		 *  \param  legDir - direction mask for edges that correspond to legs (leaves) of the caterpillar.
		 *  \returns the pointer that indicates the first vertex on the central path.
		 */
		template< class GraphType, class VInfoGen, class EInfoGen > static typename GraphType::PVertex
			caterpillar( GraphType &g, int pathVertNum, int legNnm, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeDirection pathDir, EdgeDirection legDir);

		/** \brief Create caterpillar.
		 *
		 *  The function generates a caterpillar. "A caterpillar is a tree in which every graph vertex
		 *  is on a central stalk or only one graph edge away from the stalk
		 *  (in other words, removal of its endpoints leaves a path graph..." (based on http://mathworld.wolfram.com/Caterpillar.html)
		 *  The graph is created in two phases. In the first phase a central path is created. In the next phase
		 *  (leaves or legs) are created and attached to vertices on the path. The number of legs for each vertex
		 *  on the path is specified by the parameter \a legNnm.
		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param pathVertNum - number of vertices on the central path,
		 *  \param legNnm - number of legs that should be attached to each vertex on the central path,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param dir - edges direction mask.
		 *  \returns the pointer that indicates the first vertex on the central path.
		 */
		template< class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex caterpillar( GraphType &g, int pathVertNum, int legNnm, VInfoGen vInfoGen,
				EInfoGen eInfoGen, EdgeDirection dir = EdUndir);

		/* It is a simpler version of the above function*/
		/** \brief Create caterpillar.
		 *
		 *  The function generates a caterpillar. "A caterpillar is a tree in which every graph vertex
		 *  is on a central stalk or only one graph edge away from the stalk
		 *  (in other words, removal of its endpoints leaves a path graph..." (based on http://mathworld.wolfram.com/Caterpillar.html)
		 *  The graph is created in two phases. In the first phase a central path is created. In the next phase
		 *  (leaves or legs) are created and attached to vertices on the path. The number of legs for each vertex
		 *  on the path is specified by the parameter \a legNnm.
		 *  Additionally if the mask contains a loop then the loop is attached to each vertex.
		 *  \param g - the input/output graph,
		 *  \param pathVertNum - number of vertices on the central path,
		 *  \param legNnm - number of legs that should be attached to each vertex on the central path,
		 *  \param dir - edges direction mask.
		 *  \returns the pointer that indicates the first vertex on the central path.
		 */
		template< class GraphType > static typename GraphType::PVertex
			caterpillar( GraphType &g, int pathVertNum, int legNnm, EdgeDirection dir = EdUndir );

        //NEW: nowy pierwszy parametr w generatorach grafow losowych - generator losowy
		/** \brief Create random graph.
		 *
		 *  The function generates a random graph on \a n vertices according to ErdosRenyi model G(\a n,\a p).
		 *  Each edge is included in the graph with probability \a p independent from every other edge.
		 *  If the type of the graph is set to directed, then each of the two possible (opposite directed) edges
		 *  between two particular vertices is drawn independently. (to initialize pseudo-random number generator use srand() from stdlib.h).
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param p - probability of edge's creation,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param type - the type of edges in the graph, i.e., directed or undirected.
		 *  \retrun the pointer to the first added vertex.*/
		template< class RndGen,class GraphType, class VInfoGen, class EInfoGen >
			static typename GraphType::PVertex erdRen1( RndGen& rgen,GraphType &g, int n, double p, VInfoGen vInfoGen,
				EInfoGen eInfoGen, EdgeType type = Undirected );

		/* It is a simpler version of the above function*/
		/** \brief Create random graph.
		 *
		 *  The function generates a random graph on \a n vertices according to ErdosRenyi model G(\a n, \a p).
		 *  Each edge is included in the graph with probability \a p independent from every other edge.
		 *  If the type of the graph is set to directed, then each of the two possible (opposite directed) edges
		 *  between two particular vertices is drawn independently.
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param p - probability of edge's creation,
		 *  \param type - the type of edges in the graph, i.e., directed or undirected.
		 *  \retrun the pointer to the first added vertex. */
		template< class RndGen, class GraphType >
			static typename GraphType::PVertex erdRen1( RndGen& rgen, GraphType &g, int n, double p, EdgeType type = Undirected );

		/** \brief Create random graph.
		 *
		 *  The function generates a random graph on \a n vertices according to ErdosRenyi model G(\a n, \a m).
		 *  The graph contains \a m edges chosen uniformly at random from the collection of all possible edges, i.e.,
		 *   - in the case of undirected graphs the collection contains \a n(\a n-1)/2 edges,
		 *   - in the case of directed graphs the collection contains \a n(\a n-1) edges.
		 *
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param m - number of edges to create,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param type - the type of edges in the graph, i.e., directed or undirected.
		 *  \retrun the pointer to the first added vertex.*/
		template< class RndGen, class GraphType, class VInfoGen, class EInfoGen > static typename GraphType::PVertex
			erdRen2( RndGen& rgen,GraphType &g, int n, int m, VInfoGen vInfoGen, EInfoGen eInfoGen,
				EdgeType type = Undirected);

		/* It is a simpler version of the above function*/
		/** \brief Create random graph.
		 *
		 *  The function generates a random graph on \a n vertices according to ErdosRenyi model G(\a n, \a m).
		 *  The graph contains \a m edges chosen uniformly at random from the collection of all possible edges, i.e.,
		 *   - in the case of undirected graphs the collection contains \a n(\a n-1)/2 edges,
		 *   - in the case of directed graphs the collection contains \a n(\a n-1) edges.
		 *
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param m - number of edges to create,
		 *  \param type - the type of edges in the graph, i.e., directed or undirected.
		 *  \retrun the pointer to the first added vertex. */
		template< class RndGen,class GraphType >
			static typename GraphType::PVertex erdRen2( RndGen& rgen,GraphType &g, int n, int m, EdgeType type = Undirected );

		//NEW
		/* This method generates a random graph according to the Barab\'asi - Albert (BA) model [1].
		Since the above paper defines a family of models, here the precise variant of BA model described by B. Bollob\'as in [2] has been implemented.
		The description of this variant taken from [3] is following (d is the number of edges added in each iteration of the process):
		"Assume d=1, then the i-th vertex is attached to the j-th vertex, j<=i, with probability d(j) / [m(i)+1], if j<i, and 1/ [m(i)+1], if i= j,
		where d(j) is the current degree of vertex j and m(i)= \sum_{j=0}^{i-1}d(j) is twice the number of edges already created. (...) For d>1, the graph
		evolves as if d=1 until nd vertices have been created, and	hen intervals of d consecutive vertices are contracted into one."
		Note that the result of the above procedure is a multigraph, i.e., it may contin loops and parallel edges.
		The implementation is based on pseudo-code given in [3].

		References:
		[1] "Emergence of Scaling in Random Networks", A.-L. Barabasi and R. Albert, Science, vol. 286 no. 5439 pp. 509-512, 1999.
		[2] "Random Graphs (Cambridge Studies in Advanced Mathematics)",B. Bollob\'as, 2001.
		[3] "Efficient generation of large random networks", V. Batagelj and U. Brandes, Physical Review E, vol. 71, 036113, 2005.

		Parameters:
			rgen - random number generator,
			g - the input/output graph,
			n - number of vertices to create,
			k - number of edges that are added to the graph at each stage of the algorithm,
			vInfoGen - generator for info objects for vertices,
			eInfoGen - generator for info objects for edges,
			type - the type of edges in the graph, i.e., undirected or directed, so that a newer vertex (with higher index) points to older vertex.
			shuffle - determines whether the vertices should be introduced to the graph in random order.
		Function returns the pointer to the first vertex.
		*/

		//NEW: teraz type=Directed (krawedzie skierowane o losowej orientacji),Undirected (wiadomo),EdDirIn EdDirOut (starsze -> mlodszych), EdDirOut (mlodsze -> starszych)
		template< class RndGen, class GraphType, class VInfoGen, class EInfoGen  >
		static typename GraphType::PVertex barAlb(RndGen& rgen, GraphType &g, int n, int k, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection type = Undirected, bool shuffle = false);

		//NEW
		/* It is a simpler version of the above function*/

		template< class RndGen, class GraphType >
		static typename GraphType::PVertex barAlb(RndGen& rgen, GraphType &g, int n, int k, EdgeDirection type = Undirected, bool shuffle = false);

        //NEW: wytyczne dla modeli Watts–Strogatz - mozna zmienic wewnetrzne domyslne hashSet/Map (o kluczach int lub pair<int,int>) na stlowe
        class WattStrogDefaultSettings
        {
            public:
                template< class A> class Set
                {
                    public:
                    typedef Koala::HashSet< A> Type;
                    //Druga mozliwosc
                    //typedef std::set< A> Type;
                };

                template <class A>
                static void reserveSet(Koala::HashSet< A>&s, int size)
                {
                    s.reserve(size);
                }
                template <class A>
                static void reserveSet(std::set< A>&s, int size) {}


                template< class A,class B> class Map
                {
                    public:
                        //typedef Koala::HashMap<A, B> Type;
                        //Druga mozliwosc
                        typedef Privates::StdMapWithDefault<A, B> Type;
                };
        };

		//NEW
		/*
		This method generates a random graph according to the Watts–Strogatz model (WS) model [1].
		The description of this variant taken from [1] is following:
		"We start with a ring of n vertices, each connected to its k nearest neighbours by undirected edges. (...)
		We choose a vertex and the edge that connects it to its nearest neighbour in a clockwise sense. With probability p, we reconnect
		this edge to a vertex chosen uniformly at random over the entire ring, with duplicate edges forbidden; otherwise we leave the edge in place.
		We repeat this process by moving clockwise around the ring, considering each vertex in turn until one lap is completed. Next, we consider
		the edges that connect vertices to their second-nearest neighbours clockwise. As before, we randomly rewire each of these edges with probability p,
		and continue this process, circulating around the ring and proceeding outward to more distant neighbours after each lap, until each edge
		in the original lattice has been considered once. (As there are nk/2 edges in the entire graph, the rewiring process stops after k/2 laps."

		The implementation examines all edges whether they should stay in place or should be rewired (connected to other vertex).
		An edge is rewired with probability beta. If during random choice form 1..n possible vertices while rewiring an edge {u,v} for vertex v
		a forbidden vertex x is chosen, i.e. x=u or x=v or an edge {v,x} already exists, then next random choice is performed. This procedure is
		repeated until free (not forbidden) vertex is found. This may result in long running time for dense graphs, however in practise this should not be
		a problem because as reported in [1] in most cases graphs are rather sparse, i.e., n >> k >> ln(n) >> 1.
		Note that the second implementation of WS model, i.e., wattStrog2 does not have this vulnerability.

		References:
		[1] "Collective dynamics of 'small-world' networks",D.J. Watts and S.H. Strogatz, Nature vol. 393, pp. 440-442, 1998.

		Parameters:
			rgen - random number generator,
			g - the input/output graph,
			n - number of vertices to create,
			k - the initial degree of vertices, should be an even integer,
			beta - the probability 0<= beta <=1 of rewiring initial edges,
			vInfoGen - generator for info objects for vertices,
			eInfoGen - generator for info objects for edges,
			type - the type of edges in the graph, i.e., undirected or directed (direction is set uniformly randomly).
			shuffle - determines whether the vertices should be introduced to the graph in random order.
		Function returns the pointer to the first vertex.
		*/
        //NEW: najogolniejsza wersja - z wytycznymi
		template< class Settings, class RndGen, class GraphType, class VInfoGen, class EInfoGen >
		static typename GraphType::PVertex wattStrog1(RndGen& rgen, GraphType &g, int n, int k, double beta, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType type = Undirected, bool shuffle = false);

		template< class RndGen, class GraphType, class VInfoGen, class EInfoGen >
		static typename GraphType::PVertex wattStrog1(RndGen& rgen, GraphType &g, int n, int k, double beta, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType type = Undirected, bool shuffle = false)
        {
            return wattStrog1<WattStrogDefaultSettings>( rgen, g, n,k, beta, vInfoGen, eInfoGen, type , shuffle);
        }

		//NEW
		/* It is a simpler version of the above function*/
		template< class RndGen, class GraphType >
		static typename GraphType::PVertex wattStrog1(RndGen& rgen, GraphType &g, int n, int k, double beta, EdgeType type = Undirected, bool shuffle = false);

		//NEW
		/* This is an optimized version of wattStrog1 method so that no retrials is performed while randomly rewiring edges.
		 This algorithm is based on the following concepts:
			- virtual Fisher-Yates shuffle [1],
			- virtual Fisher-Yates shuffle with deselection [2].

		Here for each vertex a "virtual" table of possible (not forbidden) vertices is maintain, so each time a free vertex is randomly chosen.
		The "virtual" table is realized by hash maps and special counters related to each vertex.

		References:
			[1] "Efficient generation of large random networks", V. Batagelj and U. Brandes, Physical Review E, vol. 71, 036113, 2005.
			[2] "An Efficient Generator for Clustered Dynamic Random Networks", R. G\"orke, R. Kluge, A. Schumm, C. Staudt and D Wagner, LNCS 7659, pp. 219-233, 2012.

		Parameters:
			rgen - random number generator,
			g - the input/output graph,
			n - number of vertices to create,
			k - the initial degree of vertices, should be an even integer,
			beta - the probability 0<= beta <=1 of rewiring initial edges,
			vInfoGen - generator for info objects for vertices,
			eInfoGen - generator for info objects for edges,
			type - the type of edges in the graph, i.e., undirected or directed (direction is set uniformly randomly).
			shuffle - determines whether the vertices should be introduced to the graph in random order.
		Function returns the pointer to the first vertex.
		*/
		template< class Settings, class RndGen, class GraphType, class VInfoGen, class EInfoGen >
		static typename GraphType::PVertex wattStrog2(RndGen& rgen, GraphType &g, int n, int k, double beta, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType type = Undirected, bool shuffle = false);

		template< class RndGen, class GraphType, class VInfoGen, class EInfoGen >
		static typename GraphType::PVertex wattStrog2(RndGen& rgen, GraphType &g, int n, int k, double beta, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType type = Undirected, bool shuffle = false)
        {
            return wattStrog2<WattStrogDefaultSettings>( rgen, g, n,k, beta, vInfoGen, eInfoGen, type , shuffle);
        }

		//NEW
		/* It is a simpler version of the above function*/
		template< class RndGen, class GraphType >
		static typename GraphType::PVertex wattStrog2(RndGen& rgen, GraphType &g, int n, int k, double beta, EdgeType type = Undirected, bool shuffle = false);

	protected:
		/** \brief Add vertices.
		 *
		 *  The function adds \a n vertices to the graph \a g and if edge direction mask contains the loop constant it also adds loops to
		 *  each of the created vertices.
		 *  \param g - the input/output graph,
		 *  \param n - number of vertices to create,
		 *  \param num - the value that is passed to vInfoGen for the first vertex. The value is incremented and passed
		 *     to vInfoGen generator for the next vertex and so on,
		 *  \param dir - edges direction mask,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \returns a pair of pointers. The pointers indicate the first and the last vertex.
		 */
		template< class GraphType, class VInfoGen, class EInfoGen >
			static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
				addVertices( GraphType &g, int n, int num, EdgeDirection dir, VInfoGen vInfoGen, EInfoGen eInfoGen );

		/** \brief Add vertices
		 *
		 *  The function adds \a n vertices to the graph \a g and if edge direction mask contains the loop constant it also adds loops to
		 *  each of the created vertices. All created vertices are sorted in the table \a vTab[]. The table should be allocated by caller
		 *  and its size should be at least equal to \a n.
		 *  \param g - the input/output graph,
		 *  \param vTab - table to insert n pointers that point created vertices,
		 *  \param n - number of vertices to create,
		 *  \param num - the value that is passed to vInfoGen for the first vertex. The value is incremented and passed
		 *     to vInfoGen generator for the next vertex and so on,
		 *  \param dir - edges direction mask,
		 *  \param vInfoGen - generator for info objects for vertices,
		 *  \param eInfoGen - generator for info objects for edges,
		 *  \param type - the type of edges in the graph, i.e., directed or undirected.
		 *  \returns a pair of pointers. The pointers indicate the first and the last vertex.
		 */
		template< class GraphType, class VInfoGen, class EInfoGen >
			static std::pair< typename GraphType::PVertex,typename GraphType::PVertex >
			addVertices2Tab( GraphType &g, typename GraphType::PVertex *vTab, int n, int num, EdgeDirection dir,
				VInfoGen vInfoGen, EInfoGen eInfoGen );

		/** \brief Add edges.
		 *
		 *  The function create edges (edge and arcs depending on the direction \a dir) between two different vertices. In order to generate an info for an edge
		 *  the eInfoGen generator is invoked in the following forms (depending on direction mask):
		 *   - eInfoGen(eInfoGen(num1, num2, EdUndir),
		 *   - eInfoGen(num1, num2, EdDirIn),
		 *   - eInfoGen(num1, num2, EdDirOut).
		 *
		 *  \param g - the input/output graph,
		 *  \param v1 - the first vertex,
		 *  \param v2 - the second vertex,
		 *  \param num1 - the value that is passed to eInfoGen generator,
		 *  \param num2 - the value that is passed to eInfoGen generator,
		 *  \param dir - edges direction mask,
		 *  \param eInfoGen - generator for info objects for edges.
		 *  \returns a pointer that indicates the created edge.
		 */
		template< class GraphType, class EInfoGen > static typename GraphType::PEdge
			addEdges( GraphType &g, typename GraphType::PVertex v1, typename GraphType::PVertex v2, int num1, int num2,
				EdgeDirection dir, EInfoGen eInfoGen );

		/** \brief Get random.
		 *
		 *  The function generates a pseudo-random number \a num (from uniform distribution) such that \a begin <= \a num <= \a end.
		 */
        template< class RndGen >
		static int random(RndGen& rgen, int begin, int end );

		//NEW
		/*
		 This function generates a pseudo-random real number r (from uniform distribution) such that 0 <= r < 1.
		*/
		template< class RndGen >
		static double random(RndGen& rgen);

		//NEW
		/*
		 This function randomly (according to uniform distribution) permutes given table.
		*/
		template< class RndGen >
		static void simpleShuffle(RndGen& rgen, int tab[], int size);

		//NEW
		/*
		This is a helper function used in wattStrog2 generator. It is responsible for marking vertex r
		as selected on the list of vertices represented by replace. This method is based on the concept taken from [1], i.e.,
		vertices of num < i, where i is a border index, are treated as selected, vertices of num >=i are treated as free (unselected),
		exceptions from this rule are stored in replace map.

		References:
		[1] "An Efficient Generator for Clustered Dynamic Random Networks", R. G\"orke, R. Kluge, A. Schumm, C. Staudt and D Wagner, LNCS 7659, pp. 219-233, 2012.

		Parameters:
			replaceInfo - a pair of pointer to a map of replacements for vertices and a border index i,
			vertices of num < i are treated as selected, vertices of num >=i are treated as free (unselected), exceptions from this rule are stored in replace map.
			r - index of vertex that should be selected.
		*/
		template <class Map>
		inline static void select(std::pair<Map *, int> & replaceInfo, int r);

        //NEW
        /*
        This is a helper function used in wattStrog2 generator. It is responsible for marking vertex r
        as unselected on the list of vertices represented by replace. This method is based on the concept taken from [1], i.e.,
        vertices of num < i, where i is a border index, are treated as selected, vertices of num >=i are treated as free (unselected),
        exceptions from this rule are stored in replace map.

        References:
        [1] "An Efficient Generator for Clustered Dynamic Random Networks", R. G\"orke, R. Kluge, A. Schumm, C. Staudt and D Wagner, LNCS 7659, pp. 219-233, 2012.

        Parameters:
        replaceInfo - a pair of pointer to a map of replacements for vertices and a border index i,
        vertices of num < i are treated as selected, vertices of num >=i are treated as free (unselected), exceptions from this rule are stored in replace map.
        r - index of vertex that should be removed (unselected).
        */
        template <class Map>
        inline static void remove(std::pair<Map *, int> & replaceInfo, int r);
	};

	// Operacje na grafie traktowanym jak diagram relacji w zbiorze wierzcholkow
	/* RelDiagramPar
	 * DefaultStructs - wytyczne dla wewnetrznych procedur
	 */
	/** \brief Binary relation operations.
	 *
	 *  The set of methods which consider a graph as a binary relation. WEN: na zbiorze wierzcholkow
	 *  \tparam DefaultStructs parameter allows to adjust the settings for internal procedures.
	 *  \ingroup detect
	 */
	template< class DefaultStructs > class RelDiagramPar
	{ 	    //WEN: cyt. z ebook: Krawędzie pozostające w wyniku nie są modyfikowane, w szczególności nie zmieniają swych inf. W każdej poniższych metod, jeśli wejściem był diagram, wyjściem też będzie diagram.
	public:

		// doprowadza diagram do zwyklej postaci, bez krawedzi nieskierowanych ani rownoleglych
		/** \brief Normalize.
		 *
		 *  Method allows to normalize graph i.e. it deletes all the undirected and parallel edges.
		 *  Graph is modified in a way to create a digraph WEN: nie wiem, czy to slowo bedzie definiowane, ale w ebooku jest
		 rozroznienie miedzy diagramem i reprezentowaniem relacji of relation.
		 *  \param g the modified graph.
		 */
		template< class Graph > static void repair( Graph &g );

		// wpisz relacje pusta na istniejacych wierzcholkach
		/** \brief Create empty relation.
		 *
		 *  All the edges are deleted from graph in order to create empty relation.
		 *  \param g the modified graph.
		 */
		template< class Graph > static void empty( Graph &g ) { g.clearEdges(); }

		// wpisz relacje kazdy z kazdym na istniejacych wierzcholkach. Mozna podac pole info wprowadzanych krawedzi
		/** \brief Create total relation.
		 *
		 *  A graph which represents (WEN: a moze diagam? por. z ebook) the total relation is created on the same set of vertexes. The final graph consists each possible directed pair between the vertices of graph. (edges in initial graph if existed are deleted)
		 WEN: graf nie zawiera par :-) terminologia!
		 *  \param g the modified graph.
		 *  \param info the EdgeInfoType object copied to the info of each new-created edge.
		 */
		template< class Graph > static void
			total( Graph &g, const typename Graph::EdgeInfoType &einfo = typename Graph::EdgeInfoType() );

		// zamienia w relacje odwrotna
		/** \brief Inverse
		 *
		 *  Each arc in graph is inversed. WEN: czyli zmiana repr. relacji na repr. relacji odwrotnej
		 *  \param g the modified graph.
		 */
		template< class Graph > static void inv( Graph &g ) { g.rev(); }

		// przeprowadza domkniecie zwrotne. Mozna podac pole info wprowadzanych krawedzi
		/** \brief Reflexive closure.
		 *
		 *  The function adds the minimal number of arc WEN: chyba loops in order to make the relation (represented by the graph \a g) reflexive.
		 *  \param g the modified graph.
		 *  \param info the EdgeInfoType object copied to the info of each new-created edge. */
		template< class Graph > static void
			reflClousure( Graph &g, const typename Graph::EdgeInfoType &einfo= typename Graph::EdgeInfoType() );

		// przeprowadza domkniecie symetryczne. Mozna podac pole info wprowadzanych krawedzi
		/** \brief Symmetric closure. WEN: tu sie cos nie zgadza (2 wersje metody). I nizej tez
		 *
		 *  The function adds the minimal number of arc in order to make the relation (represented by the graph \a g) symmetric.
		 *  \param g the modified graph.
		 *  \param info the EdgeInfoType object copied to the info of each new-created edge.*/
		template< class Graph > static void
			symmClousure( Graph &g)
			{ symmClousure(g, typename Graph::EdgeInfoType()); };

		template< class Graph > static void
			symmClousure( Graph &g, const typename Graph::EdgeInfoType &einfo);
//			symmClousure( Graph &g, const typename Graph::EdgeInfoType &einfo = typename Graph::EdgeInfoType() );

		// przeprowadza domkniecie przechodnie. Mozna podac pole info wprowadzanych krawedzi
		/** \brief Transitiv clousure.
		 *
		 *  The function adds the minimal number of arc WEN: i loops in order to make the relation (represented by the graph \a g) transitive.
		 *  \param g the modified graph.
		 */
		template< class Graph > static void
			transClousure( Graph &g)
			{ transClousure(g, typename Graph::EdgeInfoType()); };

		/** \brief Transitive closure.
		 *
		 *  The function adds the minimal number of arc WEN: i loops in order to make the relation (represented by the graph \a g) transitive.
		 *  \param g the modified graph.
		 *  \param info the EdgeInfoType object copied to the info of each new-created edge. */
		template< class Graph > static void
			transClousure( Graph &g, const typename Graph::EdgeInfoType &einfo);

        //NEW: potegowanie grafu (wykladnik wyk)
        template< class Graph > static void
			pow( Graph &g, int wyk, const typename Graph::EdgeInfoType &einfo, bool noNewDir=true);

        template< class Graph > static void
			pow( Graph &g, int wyk)
			{ pow(g,wyk,typename Graph::EdgeInfoType(),true); }


		//    Oferuje te same operacje na relacjach reprezentowanych inaczej niz przez graf.
		//    Wersje 2-argumentowe: kontener obslugiwany jak tablica 2-wymiarowa o wartosciach konwertowalnych z bool
		//        indeksowana od 0 kolejnymi liczbami. Parametr size podaje liczbe elementow
		//    Wersje 3-argumentowe: kontener o wartosciach konwertowalnych z bool i dostepie przez 2-argumentowy
		//        operator(), iteratory podaja zakres przedzialu elementow
		/** \brief Methods for matrix representation.
		 *
		 *  The same set of methods is offered for a matrix representation.\n
		 *  Functions with 2 parameters are for a container that can be managed as a two dimensional table with values convertible to bool.\n
		 *  Functions which take three parameters are for containers with access via overloaded operator() for two parameters. Iterators give the range of elements.
		 */
		struct MatrixForm
		{
			template< class Cont > static void empty( Cont &cont, int size );
			template< class Cont, class Iter > static void empty( Cont &cont, Iter beg, Iter end );
			template< class Cont > static void total( Cont &cont, int size );
			template< class Cont, class Iter > static void total( Cont &cont, Iter beg, Iter end );
			template< class Cont > static void inv( Cont &cont, int size );
			template< class Cont, class Iter > static void inv( Cont &cont, Iter beg, Iter end );
			template< class Cont > static void reflClousure( Cont &cont, int size );
			template< class Cont, class Iter > static void reflClousure( Cont &cont, Iter beg, Iter end );
			template< class Cont > static void symmClousure( Cont &cont, int size );
			template< class Cont, class Iter > static void symmClousure( Cont &cont, Iter beg, Iter end );
			template< class Cont > static void transClousure( Cont &cont, int size );
			template< class Cont, class Iter > static void transClousure( Cont &cont, Iter beg, Iter end );
		};
	} ;

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Binary relation operations.
	 *
	 *  The set of methods which consider a graph as a binary relation.
	 *  The version with default setting.
	 *  \ingroup detect	 */
	class RelDiagram: public RelDiagramPar< AlgsDefaultSettings > { };

	/* LineGraphPar
	 * Kreator linegrafow
	 * DefaultStructs - wytyczne dla wewnetrznych procedur
	 */
	/** \brief Linegraph creator (parametrized).
	 *
	 *  The class allows to generate the line graph WEN: oraz skierowany of a graph.
	 *  To change options of used algorithms class \a DefaultStructs should be modified.
	 *  \ingroup detect
	 */
	template< class DefaultStructs > class LineGraphPar
	{
	protected:
		template< class Graph >
			static bool open( const Graph &g, typename Graph::PEdge e, typename Graph::PVertex v, typename Graph::PEdge f );
		template <class Graph>
			static bool openDir( const Graph &g, typename Graph::PEdge e, typename Graph::PVertex v, typename Graph::PEdge f);

	public:
		//        Dopisuje do lg nieskierowany graf krawedziowy tworzony na podstawie g.
		//        Para casterow wyznacza infa nowych wierzcholkow na podstawie inf oryginalnych krawedzi
		//        i infa nowych krawedzi na podstawie inf wierzcholkow, w ktorych stykaly sie oryginalne krawedzie
		//        Pierwszy Linker laczy krawedzie g z odpowiadajacymi im wierzcholkami w lg
		//         Drugi laczy wierzcholki g z polaczeniami wierzcholkow w lg (ktore zachodza w tych wierzcholkach)
		//      Zwraca pierwszy utworzony wierzcholek
		/** \brief Create undirected linegraph.
		 *
		 *  A linegraph of the graph \a g is created and added to the graph \a lg. (\a g and \a lg shouldn't refer to the same object.)
		 *  \param g an initial graph.
		 *  \param[out] lg the linegraph of \a g is added here.
		 *  \param casters a standard pair of casters. \n
		 *    The first one create information for the linegraph vertices basing on edges WEN: tej z ktorej sie wywodzi of \a g.\n
		 *    The second caster generates information for the linegraph edges basing on vertices of \a g. WEN: odsylacz do ebooka - ktory wierzcholek g jest utozsamiany z ktora kraw. linegrafu
		 *  \param linkers a standard pair of linkers. \n
		 *    The first one links the edges of \a g with the vertices of \a lg. WEN: ale dotychczasowe wierzcholki lg sa linkowane z NULL\n
		 WEN: sprawdzic z ksiazka, czy aby nie odwrotnie (spojnosc okreslen), to nizej tez
		 *    The second on links the vertices of \a g with the edges of \a lg. WEN: odsylacz do ebooka - ktory wierzcholek g jest wiazany z ktora kraw. lg. Nie uwzglednione tu wierzcholki linkuje sie do NULL
		 *  \return the first created vertex of \a lg.
		 */
		template< class GraphIn, class GraphOut, class VCaster, class ECaster, class VLinker, class ELinker >
			static typename GraphOut::PVertex undir( const GraphIn &g, GraphOut &lg, std::pair< VCaster,ECaster > casters,
				std::pair< VLinker,ELinker > linkers );
		// jw. bez linkerow
		/** \brief Create undirected linegraph.
		 *  WEN: te same co wyzej (jesli sie lapia)
		 *  A linegraph of the graph \a g is created and added to the graph \a lg. (\a g and \a lg shouldn't refer to the same object.)
		 *  \param g an initial graph.
		 *  \param[out] lg the linegraph of \a g is added here.
		 *  \param casters a standard pair of casters. \n
		 *    The first one create information for the linegraph vertices basing on edges of \a g.\n
		 *    The second caster generates information for the linegraph edges basing on vertices of \a g.
		 *  \return the first created vertex of \a lg.
		 */
		template< class GraphIn, class GraphOut, class VCaster, class ECaster >
			static typename GraphOut::PVertex undir( const GraphIn &g, GraphOut &lg, std::pair< VCaster,ECaster > casters );

		// jw. ale tylko tworzy linegraph
		/** \brief Create undirected linegraph.
		 *
		 *  A linegraph of the graph \a g is created and added to the graph \a lg. (\a g and \a lg shouldn't refer to the same object.)
		 *  \param g an initial graph.
		 *  \param[out] lg the linegraph of \a g is added here.
		 *  \return the first created vertex of \a lg.
		 */
		template< class GraphIn, class GraphOut >
			static typename GraphOut::PVertex undir( const GraphIn &g, GraphOut &lg );

		//NEW: to samo z hardCast
		template< class GraphIn, class GraphOut >
			static typename GraphOut::PVertex undir2( const GraphIn &g, GraphOut &lg );



		//        Dopisuje do lg skierowany graf krawedziowy tworzony na podstawie g.
		//        Para casterow wyznacza infa nowych wierzcholkow na podstawie inf oryginalnych krawedzi
		//        i infa nowych lukow na podstawie inf wierzcholkow, w ktorych stykaly sie oryginalne krawedzie
		//        Pierwszy Linker laczy krawedzie g z odpowiadajacymi im wierzcholkami w lg
		//         Drugi laczy wierzcholki g z polaczeniami wierzcholkow w lg (ktore zachodza w tych wierzcholkach)
		//      Zwraca pierwszy utworzony wierzcholek
		/** \brief Create directed linegraph.
		 *  WEN: to samo, co w undir
		 *  A directed linegraph of the graph \a g is created and added to the graph \a lg. (\a g and \a lg shouldn't refer to the same object.)
		 *  \param g an initial graph.
		 *  \param[out] lg the linegraph of \a g is added here.
		 *  \param casters a standard pair of casters. \n
		 *    The first one create information for the linegraph vertices basing on edges of \a g.\n
		 *    The second caster generates information for the linegraph edges basing on vertices of \a g.
		 *  \param linkers a standard pair of linkers. \n
		 *    The first one links the edges of \a g with the vertices of \a lg.\n
		 *    The second on links the vertices of \a g with the edges of \a lg.
		 *  \return the first created vertex of \a lg.
		 */
		template< class GraphIn, class GraphOut, class VCaster, class ECaster, class VLinker, class ELinker >
			static typename GraphOut::PVertex dir( const GraphIn &g, GraphOut &lg, std::pair< VCaster,ECaster > casters,
				std::pair< VLinker,ELinker > linkers );
		// jw. bez linkerow
		/** \brief Create directed linegraph.
		 *  WEN: to samo, co w undir
		 *  A directed linegraph of the graph \a g is created and added to the graph \a lg. (\a g and \a lg shouldn't refer to the same object.)
		 *  \param g an initial graph.
		 *  \param[out] lg the linegraph of \a g is added here.
		 *  \param casters a standard pair of casters. \n
		 *     The first one create information for the linegraph vertices basing on edges of \a g.\n
		 *     The second caster generates information for the linegraph edges basing on vertices of \a g.
		 *  \return the first created vertex of \a lg.
		 */
		template< class GraphIn, class GraphOut, class VCaster, class ECaster >
			static typename GraphOut::PVertex dir( const GraphIn &g, GraphOut &lg, std::pair< VCaster,ECaster > casters )
			{
				return dir( g,lg,casters,std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}
		// jw. ale tylko tworzy linegraph
		/** \brief Create directed linegraph.
		 *
		 *  A directed linegraph of the graph \a g is created and added to the graph \a lg. (\a g and \a lg shouldn't refer to the same object.)
		 *  \param g an initial graph.
		 *  \param[out] lg the linegraph of \a g is added here.
		 *  \return the first created vertex of \a lg.
		 */
		template< class GraphIn, class GraphOut >
			static typename GraphOut::PVertex dir( const GraphIn &g, GraphOut &lg )
			{
				return dir( g,lg,std::make_pair( stdCast(  ),stdCast( ) ),
				std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}

        //NEW: to samo z hardCast
		template< class GraphIn, class GraphOut >
			static typename GraphOut::PVertex dir2( const GraphIn &g, GraphOut &lg )
			{
				return dir( g,lg,std::make_pair( hardCast(  ),hardCast( ) ),
				std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}

	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Linegraph creator.
	 *
	 *  The class allows to generate the line graph of a graph.
	 *  Works like \a LineGraphPar but the without the possibility of parameterizing. The class \a AlgsDefaultSettings is used.
	 *  \ingroup detect
	 *
	 *  [See example](examples/create/example_LineGraph.html).
	 */
	class LineGraph: public LineGraphPar< AlgsDefaultSettings > { };

	/* ComplexCaster
	 * Caster zlozony z jednego castera 3-argumentowego i dwoch 2-argumentowych
	 */
	/* \brief Complex caster for products.
	 *
	 * Useful if some entities are generated from one source and other need two sources. Which is the case is some products  of graphs for edges.
	 * \ingroup detect*/

	 namespace Privates {

        template <class Caster, int> struct ComplexCastTwoArgCaster;

        template <class Caster> struct ComplexCastTwoArgCaster<Caster,0>
        {
            template< class InfoDest, class InfoSour1, class InfoSour2 >
            inline void cast(Caster& cast,InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) const
            { cast( dest,sour1,sour2 ); }
        };

        template <class Caster> struct ComplexCastTwoArgCaster<Caster,1>
        {
            template< class InfoDest, class InfoSour1, class InfoSour2 >
            inline void cast(Caster& cast,InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) const
            { cast( dest,sour1 ); }
        };

        template <class Caster> struct ComplexCastTwoArgCaster<Caster,2>
        {
            template< class InfoDest, class InfoSour1, class InfoSour2 >
            inline void cast(Caster& cast,InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) const
            { cast( dest,sour2 ); }
        };

	 }

    //NEW: 4-ty argument szablonu. int=0 - jak bylo, int =1 to twoarg jest 3-argumentowy, a w wywolaniu
    // operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) rzutuje z sour1, int =2 tj. ale z sour2
    //Dzieki temu mozna tworzyc ComplexCaster poslugujac sie tylko casterami 2-argumentowymi
    template< class TwoArg, class FirstArg, class SecondArg,int ver> struct ComplexCaster
	{

	    typedef ComplexCaster< TwoArg, FirstArg, SecondArg, ver > CastersSelfType;

		mutable TwoArg twoarg;/**\brief Two argument caster function object.*/
		mutable FirstArg firstarg;/**\brief First argument caster function object.*/
		mutable SecondArg secondarg;/**\brief Second argument caster function object.*/
		/**\brief Constructor*/
		ComplexCaster( TwoArg t = TwoArg(), FirstArg f = FirstArg(), SecondArg s = SecondArg() ):
			twoarg( t ), firstarg( f ), secondarg( s )
			{ }

		// jesli podano oba argumenty, zastosuj twoarg
		/**\brief Cast two sources to one destination.*/
		template< class InfoDest, class InfoSour1, class InfoSour2 >
			void operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 )
			{ Privates::ComplexCastTwoArgCaster<TwoArg,ver>().cast(twoarg, dest,sour1,sour2 ); }

		// jesli podano pierwszy argument, zastosuj firstarg
		/**\brief Cast first source (second BlackHole) to destination.*/
		template< class InfoDest, class InfoSour1 >
			void operator()( InfoDest &dest, InfoSour1 sour1, Koala::BlackHole b )
			{ firstarg( dest,sour1 ); }

		// jesli podano drugi argument, zastosuj secondarg
		/**\brief Cast second source (first BlackHole) to destination.*/
		template< class InfoDest, class InfoSour1 >
			void operator()( InfoDest &dest, Koala::BlackHole b, InfoSour1 sour2 )
			{ secondarg( dest,sour2 ); }
	};


	// funkcja tworzaca - podajemy castery skladowe
	/**\brief Generating function for ComplexCaster
	 * \ingroup detect*/
	template< class TwoArg, class FirstArg, class SecondArg > ComplexCaster< TwoArg, FirstArg,SecondArg,0 >
		complexCast( TwoArg t, FirstArg f, SecondArg s )
		{ return ComplexCaster< TwoArg,FirstArg,SecondArg,0 >( t,f,s ); }


    //NEW: ale dla int=1
	// funkcja tworzaca - podajemy castery skladowe
	/**\brief Generating function for ComplexCaster
	 * \ingroup detect*/
	template< class TwoArg, class FirstArg, class SecondArg > ComplexCaster< TwoArg, FirstArg,SecondArg,1 >
		complexCast1( TwoArg t, FirstArg f, SecondArg s )
		{ return ComplexCaster< TwoArg,FirstArg,SecondArg,1 >( t,f,s ); }


    //NEW: ale dla int=2
	// funkcja tworzaca - podajemy castery skladowe
	/**\brief Generating function for ComplexCaster
	 * \ingroup detect*/
	template< class TwoArg, class FirstArg, class SecondArg > ComplexCaster< TwoArg, FirstArg,SecondArg,2 >
		complexCast2( TwoArg t, FirstArg f, SecondArg s )
		{ return ComplexCaster< TwoArg,FirstArg,SecondArg,2 >( t,f,s ); }


	/* ProductPar
	 * Kreator iloczynow grafow
	 * DefaultStructs - wytyczne dla wewnetrznych procedur
	 */
	/** \brief Product creator (parametrized).
	 *
	 *  The class allows to generate different versions of the product of two graphs.
	 *  Parametrization of algorithms and structures may be introduced via the template parameter class \a DefaultStructs.
	 *  \ingroup detect
	 */
	template< class DefaultStructs > class ProductPar
	{
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

		//WEN: OK, ale zdajemy sobie sprawe, ze opis dzialania casterow i inkerow wszedzie nizej jest do d... Opisalem to w schemacie
		// i gdzies (np. w ebooku) to sie musi pojawic, a jesli nie tu, to stad przynajmniej musi byc inteligentnie tamze polinkowane
		/** \brief Generate Cartesian product.
		 *
		 *  The Cartesian product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \param link the standard pair of linkers which connect the entities of the initial graphs with the target entities.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster, class VLinker, class ELinker >
			static typename Graph::PVertex cart( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast, std::pair< VLinker,ELinker > link );
		// j.w. ale bez linkerow
			/** \brief Generate Cartesian product.
		 *
		 *  The Cartesian product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster >
			static typename Graph::PVertex cart( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast )
			{
				return cart( g1,g2,g,cast,std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}
		// j.w. ale tylko tworzy iloczyn
		/** \brief Generate Cartesian product.
		 *
		 *  The Cartesian product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph >
			static typename Graph::PVertex cart( const Graph1 &g1, const Graph2 &g2, Graph &g )
			{
				return cart( g1,g2,g,std::make_pair( stdCast( false ),stdCast( false ) ),
				std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}

		//    Dopisuje iloczyn    leksykograficzny
		//    grafow g1 i g2 do g. Zwraca pierwszy wprowadzony wierzcholek. Sens parametrow - j.w.
		/** \brief Generate lexicographic product.
		 *
		 *  The lexicographic product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \param link the standard pair of linkers which connect the entities of the initial graphs with the target entities.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster, class VLinker, class ELinker >
			static typename Graph::PVertex lex( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast, std::pair< VLinker,ELinker > link );
		// j.w. ale bez linkerow
		/** \brief Generate lexicographic product.
		 *
		 *  The lexicographic product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster >
			static typename Graph::PVertex lex( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast )
			{
				return lex( g1,g2,g,cast,std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}
		// j.w. ale tylko tworzy iloczyn
		/** \brief Generate lexicographic product.
		 *
		 *  The lexicographic product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph >
			static typename Graph::PVertex lex( const Graph1 &g1, const Graph2 &g2, Graph &g )
			{
				return lex( g1,g2,g,std::make_pair( stdCast( false ),stdCast( false ) ),
				std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}

		//    Dopisuje iloczyn    tensorowy
		//    grafow g1 i g2 do g. Zwraca pierwszy wprowadzony wierzcholek. Sens parametrow - j.w.
		/** \brief Generate tensor product.
		 *
		 *  The tensor product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \param link the standard pair of linkers which connect the entities of the initial graphs with the target entities.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster, class VLinker, class ELinker >
			static typename Graph::PVertex tensor( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast, std::pair< VLinker,ELinker > link );
		// j.w. ale bez linkerow
		/** \brief Generate tensor product.
		 *
		 *  The tensor product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster >
			static typename Graph::PVertex tensor( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast )
			{
				return tensor( g1,g2,g,cast,std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}
		// j.w. ale tylko tworzy iloczyn
		/** \brief Generate tensor product.
		 *
		 *  The tensor product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph >
			static typename Graph::PVertex tensor( const Graph1 &g1, const Graph2 &g2, Graph &g )
			{
				return tensor( g1,g2,g,std::make_pair( stdCast( false ),stdCast( false ) ),
				std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}

		//    Dopisuje strong
		//    grafow g1 i g2 do g. Zwraca pierwszy wprowadzony wierzcholek. Sens parametrow - j.w.
		/** \brief Generate strong product.
		 *
		 *  The strong product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \param link the standard pair of linkers which connect the entities of the initial graphs with the target entities.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster, class VLinker, class ELinker >
			static typename Graph::PVertex strong( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast, std::pair< VLinker,ELinker > link );
		// j.w. ale bez linkerow
		/** \brief Generate strong product.
		 *
		 *  The strong product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \param cast a standard pair of casters used for generating info members.
		 *     The first caster creates \a info for a new vertex basing on the two original vertices.
		 *     The second caster creates \a info for an edge basing on two initial edges.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph, class VCaster, class ECaster >
			static typename Graph::PVertex strong( const Graph1 &g1, const Graph2 &g2, Graph &g,
				std::pair< VCaster,ECaster > cast )
			{
				return strong( g1,g2,g,cast,std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}
		// j.w. ale tylko tworzy iloczyn
		/** \brief Generate strong product.
		 *
		 *  The strong product of graphs \a g1 and \a g2 is created and added to the graph \a g. (\a g shouldn't refer to the same object as \a g1 or \a g2.)
		 *  \param g1 the first reference graph.
		 *  \param g2 the second reference graph.
		 *  \param[out] g the product of \a g1 and \a g2 is added here.
		 *  \return the first created vertex.
		 */
		template< class Graph1, class Graph2, class Graph >
			static typename Graph::PVertex strong( const Graph1 &g1, const Graph2 &g2, Graph &g )
			{
				return strong( g1,g2,g,std::make_pair( stdCast( false ),stdCast( false ) ),
				std::make_pair( stdLink( false,false ),stdLink( false,false ) ) );
			}
	};

	// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
	/** \brief Product creator (default).
	 *
	 *  The class allows to generate different versions of the product of two graphs.
	 *  The version with the default options.
	 *  \ingroup detect
	 *
	 *  [See example](examples/create/example_Product.html).
	 */
	class Product: public ProductPar< AlgsDefaultSettings > { };

#include "create.hpp"
}

#endif
