
/** \defgroup DMbase Basic (main) structures
 *
 *  Graph is a directed pair <tt>(V, E)</tt> where \p V is a set of Vertexes and \p E is a set of Edges.
 */
/*  - list structure + adj
 *  - edge/vert info ???
 *  - viewer*/

/** \defgroup DMgraph Graph structure
 *  \ingroup DMbase*/

/** \defgroup DMdef Auxiliary structures
 *  \ingroup DMbase*/ 

/** \defgroup DMexception Exceptions
 *  \ingroup DMbase*/

/** \defgroup DMview Views on graph
 *
 *  Views of graph are one of the main ideas of Koala library. However using Koala without them is completely possible and does not reduce the functionality. 
 * 
 *  Views allow to work with parts of graph or its modified version without creating a copy of it. If you generate for example subgraph, only part of graph is visible in your view. You can work on it but the whole structure is just behind. Hence, modifying viewed graph means modifying the whole graph as well. 
 *
 *  Since views of views can be created, whole hierarchy can be created via this mechanism.
 *
 *  View exists as long as the superior object (graph or view) exist. If an object (graph or view)  becomes unplugged or destructed, all the descendants also are unplugged. However, the structure remain valid and can be plugged to another graph or view.
 *  \ingroup DMbase*/

/** \defgroup DMchooser Chooser object functions
 * 
 *  All the choosers are the object functions designed to choose some objects (like edges or vertexes) automatically. 
 *
 *  For each chooser in Koala there should be a call function operator overloaded:\n 
 *  <tt>template< class Elem, class Graph > bool operator()( Elem *elem, const Graph &gr )</tt>\n
 *  where \a elem is the considered (tested) element, and \a gr is the current graph in which \a elem is placed.\n
 *  Each chooser should also have type \p ChooserSelfType defined if logic operations (&&, ||, !) on this chooser are going to be used.
 *
 *  Koala delivers not only a standard for choosers but also a number of ready-to-use choosers that can be scanned through in dis module.
 *   
 *  \ingroup DMbase */


/** \defgroup DMlinker Linkers of elements
 *
 *  Some algorithms generate new graphs or subgraphs and it is important to recognize the origins of the new element. 
 *  For example we need to know from which edge in the initial graph the vertex of the line graph origins. 
 *  In such cases linkers determine the way the algorithm automatically connect the initial element with the new-created one.
 *
 *  Each linker should overload the function call operator which takes the pointer to destination and source element. That is how to connection between the destination and the source is created.
 * \ingroup DMbase */

/** \defgroup DMcaster Casters of infos
 *
 *  Some algorithms generate new graphs or subgraphs. Then the problem of generating info objects for new elements arises. In such cases casters are useful.
 *
 *  Casters usually overload call function operator with two or three parameters. ex.
 *  <tt> void operator()( InfoDest &dest, InfoSour1 sour1, InfoSour2 sour2 ) </tt>\n
 *  where \p dest is the reference to created object, \p sour1, and \a sour2 are the object from which now info is generated. 
 * \ingroup DMbase */

/** \defgroup cont Containers and data structures
 */
/** \defgroup DMsimplecont Simple containers
 *  \ingroup cont
 */

/** \defgroup detect Recognition and generation of graph classes
 *   
 *   Many graph classes can be detected in polynomial time. Such methods can be found here. 
 *   Furthermore if the family the graph belongs to is recognized, there are plenty of algorithms which solve various graph problems optimally in reasonable time. Many of those algorithms are implemented here. \n
 *   On the other hand methods grouped in this module allow to easily generate various instances that are the representatives of graph classes.
 *
 */

/** \defgroup algor Algorithms
 *   
 *    
 *   
 */

/** \defgroup search Searching algorithms
 *   
 *    \ingroup algor
 *   
 */

/** \defgroup DMschedule Scheduling algorithms
 *   
 *    \ingroup algor
 *   
 */

/** \defgroup DMmis Minimum independent set
 *   
 *    \ingroup algor
 *   
 */

/** \defgroup DMmatch Matching algorithms
 *   
 *    \ingroup algor
 *   
 */
/** \defgroup DMweight Weighted algorithms
 *   
 *    \ingroup algor
 *   
 */
/** \defgroup DMflow Flow algorithms
 *
 *  \ingroup algor*/
/** \defgroup DMconnect Connectivity testing
 *   \ingroup algor*/

/** \defgroup color Coloring algorithms
 *
 *
 */

/* \defgroup ecolor Edge coloring algorithms
 *
 *  
 */

/** \defgroup io Input/Output
 *
 */

/** \defgroup iotxt Text Input/Output  methods
 *  \ingroup io */

/** \defgroup DMiog6 G6 standard Input/Output  methods
 *
 *   To get more information about G6 <a href = "http://cs.anu.edu.au/~bdm/data/formats.html"> click here </a>.
 *   \ingroup io */
/** \defgroup DMiographml GraphML Input/Output  methods
 *
 *  
 *  \ingroup io */
/** \defgroup DMiographmlA Object info casters for GraphML
 *
 *   \ingroup DMiographml */

/** \defgroup DMiotxtformat Text Input/Output formats
 *  \ingroup iotxt
 *
 * Supported formats:
 * --
 *  RG_VertexLists:
 *  ---------------
 *  n\n
 *  <vertex-1> <k_1> <edge-1> <edge-2> ... <edge-k_1>\n
 *  ...\n
 *  <vertex-n> <k_n> <edge-1> <edge-2> ... <edge-k_n>\n
 *
 *  first line defines number of vertices
 *  each other line describes edges adjacent to vertex-i
 *
 *  Each edge should appear exactly once (since parallel edges are allowed).\n
 *  To describe P2 use:\n
 *  2\n
 *  0 1 -1\n
 *  1 0\n
 *  the following text:\n
 *  2\n
 *  0 1 -1\n
 *  1 1 -0\n
 *  gives graph with two vertices and two parallel edges between them\n
 *
 *  vertex-i can have one of the following formats:\n
 *  <vertex-id> ( <vertex-info> ) @ <output-id>\n
 *  where:
 *  - <vertex-id>: vertex identifier (nonnegative integer in range 0..n-1)
 *  - <vertex-info>: string with matched parentheses (use \ to escape unmatched ones)
 *                 describing the VertexInfo (istream >> VertexInfo should work)
 *                 (optional)
 *  - <output-id>: key in the output map of the vertex (nonnegative integer, optional)
 *
 *  edge-i can have of the following formats:
 *  - <type> <vertex-id> ( <edge-info> ) @ <output-id>
 *  - <type>: defines edge direction: - (undirected), > (to vertex-id), < (from vertex-id), * (loop)
 *  - <vertex-id>: vertex identifier of the second end of the edge (loop should have starting vertex repeated here)
 *  - <edge-info>: string with matched parentheses (use \ to escape unmatched ones)
 *               describing the EdgeInfo (istream >> EdgeInfo should work)
 *               (optional)
 *  <output-id>: key in the output map of the edge (nonnegative integer, optional)
 *
 *  examples:
 *
 *  code                |  interpretation                           |        
 * -------------------- | ----------------------------------------- |
 * 0 3 -1 -2 -3   	    | undirected edges (0, 1), (0, 2), (0, 3)   |
 * 0 4 >1 >2 -3 *0      | directed edges (0, 1), (0, 2), undirected edge (0, 3) and loop (0, 0) |
 * 3(v3) 1 <0(e0)@5     | vertex 3 has vertex info "v3", add directed edge (0, 3) with edge info "e0" and remember that edge under key 5 in output edge map |
 * 2(vertex-2)@1 0	    | vertex with info "vertex-2" and no edges, remembered under key 1 in output vertex map |
 *
 * EdgeList:
 * ---------
 *  a list of edges, followed by a list of vertices:\n
 *  n m\n
 *  <vertex-id-1a> <direction> <vertex-id-1b> <edge-info> <output-id>\n
 *  ...\n
 *  <vertex-id-ma> <direction> <vertex-id-mb> <edge-info> <output-id>\n
 *  <vertex-1> <vertex-info> <output-id>\n
 *  ...\n
 *  <vertex-n> <vertex-info> <output-id>\n
 *
 *  first line defines number of vertices (n) and number of edges (m)
 *  following m lines describe edges and another n lines define vertices
 *
 *  vertex-i has the same format as in VertexLists
 *
 *  - <vertex-id-ia>, <vertex-id-ib>	- identifiers of edge ends (integers from range 0..n-1)
 *  - <direction>				- "<", ">" (directed edge, < means edge ib to ia, > means edge from ia to ib)
 *   					  "-" (undirected edge)
 *   					  "*" (loop)
 *
 *  examples:\n
 *  Code                  |  Description                                                                      |
 * ---------------------- | --------------------------------------------------------------------------------- |
 *  1 - 2		          | undirected edge between 1 to 2                                                    |
 *  1 - 2 (edge-0)	      | undirected edge between 1 to 2 with info "edge-0"                                 |
 *  1 - 2 @3		      | undirected edge between 1 to 2 to be remembered under key 3                       |
 *  1 - 2 (edge-0) @3	  | undirected edge between 1 to 2 to be remembered under key 3 with info "edge-0"    |
 *  1 > 2		          | directed edge from 1 to 2                                                         |
 *  1 < 2		          | directed edge from 2 to 1                                                         |
 *  1 * 1		          | loop attached to vertex 1                                                         |
 *
 */

/** 
 */

to do:
kontenery
adjmatrix
vievs
graph wybrane metody

do korekty:
podklasy detectów
set 
dopisane graph.h