/** \mainpage KOALA Library
 *  \ref description
 *  
 *  \ref wiki
 *
 * <a href = "../koala.zip"> KOALA sources </a> 
 *
 * <a href = "modules.html">Documentation</a> of the KOALA library and <a href = "examples/index.html">examples</a> of the codes using it
 *
 * <a class="el" href = "http://www.kaims.pl/koala/zgred/">Graph editor Zgred</a>
 *
 * <a href = "../GraphEditor.zip">Program GraphEditor</a>
 * 
 *  \ref authors
 */
/** \page description Introduction and general description
 * \section sec1 Introduction
 * KOALA is a library of C++ templates, implementing a broad set of procedures in the fields of algorithmic graph theory and network problems in discrete optimization. In its construction, KOALA is based on paradigms of object-oriented programming and generic programming. The base project assumptions are: a) versatility of the library – its procedures are adapted to a broad range of various thematic areas of graph algorithmics b) interface procedures, which are convenient and friendly for the user. Thanks to that, KOALA reconciles two apparently contradictory priorities: the created code uses (and also allows the user to use) progressive, advanced C++ language mechanisms, at the same time allowing the programmer to use KOALA codes, knowing object C++ and templates only in a basic range.
 *
 * \section sec2 The template of the basic structure of the graph
 * The main data structure, that is, the class Koala::Graph (module %graph/graph.h \ref DMgraph) 
 * represents the template of a mixed multigraph, in which multiple edges (that is parallel), undirected, directed and loops can occur simultaneously. The choice of the type of the edge and its orientation taking into consideration for specific operations are performed using specific bit masks (constants of types EdgeType, EdgeDirection or their logic sums – see definitions in the module %base/defs.h) 
 * given for procedures in arguments (usually equipped with the most often applied default values). In processing graphs in the natural way a necessity arises for labeling of vertices and edges by the data of various kinds, thus the graph template allows to place labels of arbitrarily chosen types (both simple and defined by the user) – they are template parameters (arguments VertInfo, EdgeInfo of the Graph class). These labels appear as the only public fields (named info) of the objects of vertices and edges. Objects representing vertices and edges are nested in the graphs and manager by it – from the user’s point of view, they are unequivocally identified by their pointers (internal types PVertex, PEdge of the Graph template – see the module %graph/graph.h Koala::Graph).
 * The internal structure of the graph in its construction is based on dynamic bidirectional lists: vertices, edges and separate lists of incident edges for each vertex. These lists are managed by the same object of the graph in a way invisible for the programmer. A rule has been agreed of the invariability of the pointer of the element of the graph by graph operations directly on this element not affecting (which eliminates typical, for example for STL containers, problems with invalidating of the iterators during changing a part of the content of the container). Thanks to the list structure, the majority of simple operations of reading and modifying a graph are realized in a constant time. For the reasons of efficiency (fast access to the lists of edges joining the given pair of vertices) on request of the user, the graph object can create auxiliary generalized adjacency matrix – the graph uses it during realizing selected methods in a transparent way, not changing the functionality of the interface. The interface of the graph object offers a wide range of methods running for example the following operations: removing vertices and edges, iterative review in both directions of lists of these components, reading parameters (for example the order and the size of the graph, the degrees of the vertices), review of the open and closed neighborhoods, and also procedures operating on whole graphs – copying the whole graph or some its subgraph (eventually, on a request of the user, creating links between elements of the copy and the original graph – see objects of families Chooser, Caster, Linker of the %base/defs.h module \ref DMchooser, \ref DMlinker, \ref DMcaster),
 or substitution of another graph for an indicated vertex. The complete interface of the graph contains around 160 methods or their templates. 
 *
 * \section sec3 Algorithms of the library
 * KOALA offers many effective algorithms (of polynomial complexity) solving computational problems of various topics of graph theory (\ref color).
 *	<ol>
 * <li>Chromatic graph theory
 *  <ol><li> Classical vertex coloring (the module %coloring/vertex.h)
 *     - Heuristics of sequential coloring: greedy, smallest last, largest first, saturation largest first and their versions with interchange (of colors).
 *     - Coloring with Δ colors (compare with the Brooks Theorem), heuristics GIS (Johnson 1974).</li>
 *   <li>Classical edge multigraph coloring (the module %coloring/edge.h)
 *     - Heuristics of sequential coloring: greedy and with color interchange.
 *     - Coloring with  Δ+m colors (compare with the Vizing Theorem).</li>
 *   <li>List vertex and edge multigraph coloring (the module %coloring/list.h): sequential heuristics with a possibility to determine the method of choosing the color from the list.</li>
 *   <li>Interval coloring of vertices with assigned numerical weights (the module %coloring/interval.h): sequential heuristics, list interval (Kubale 1988).</li>
 *   <li> Interval coloring of edges with assigned numerical weights (the module %coloring/interval.h): sequential heuristics, 4-approximate algorithm longest first (Krawczyk, Kubale 1985), 2-approximate algorithm list interval (Coffman et al. 1985).
Majority of the above procedures appear also in the version adapted for multigraphs initially, partially colored.</li>
 *   <li>Equitable coloring (the module %coloring/equitable.h): two heuristics transforming given vertex coloring into an equitable coloring (Furmañczyk, Kubale 2005), similarly for edge coloring without increasing the number of colors used (exact algorithm).</li></ol> </li>
 * <li> Analyzing the structure of a graph (the module %algorithm/search.h \ref detect):
 *   -# Traversing of multigraphs using the methods DFS (preorder or postorder), BFS, lexicographic BFS – all with the possibilities of obtaining the sequence of vertices, division of a graph into connected components, finding a spanning forest, the cyclomatic number, distance and path of vertices to the root in the resultant spanning tree and also entering to the algorithm own visiting objects (visitors).
 *   -# Decomposition of an undirected multigraph into 2-connected components and a directed multigraph into strongly connected components.
 *   -# Procedures for acyclic digraphs: test of the lack of cycles, topological order of vertices, determining transitive edges and Hasse diagram
 *   -# Decomposition of an undirected graph into modules (the highest level of the modular decomposition tree).</li>
 * <li> Optimization problems for multigraphs with numerical weights on edges (the module %algorithm/weights.h):
 *   -# Determining the distance between vertices and determining an optimal path: Dijkstra algorithm (simple or with a priority queue), Bellman-Ford and Floyd (that is with allowing negative weights), longest paths in an acyclic digraph (critical paths tree).
 *   -# The lightest or the heaviest spanning tree (Kruskal).</li>
 * <li> Flow problems (with a generalization to the so called transhipment - compare with Schrijver 2003) and cuts in networks with edges with nominal capacity (the module %algorithm/conflow.h):
 *   -# Maximal flow (Fulkersona-Forda and Dinits methods), the cheapest maximal flow or with a given capacity (a pseudo-polynomial algorithm and polynomial procedure - see Goldberg, Tarjan 1989).
 *   -# Minimal edge cut between a given pair of vertices, or in the whole network.
 *   -# Gomory-Hu tree of an undirected graph.
 *   -# For multigraphs without weights:
 *     - A minimal vertex cut or edge cut between a pair of vertices or in the whole network.
 *     - The largest set of paths, which are vertex-disjoint or edge-disjoint with given ends (compare with the Menger theorem)</li>
 * <li>	Task scheduling on parallel identical processors (the module %algorithm/schedule.h \ref DMschedule)
 *	  <ol><li> Tasks with precedence constrains (in the form of an acyclic digraph):
 *     - Minimizing the length of a schedule in models: -|prec|C<sub>max</sub> (the so called critical path method), P2|p<sub>j</sub>=1,prec|C<sub>max</sub> (Coffman-Graham algorithm), P|p<sub>j</sub>j=1,intree|C<sub>max</sub> (the Hu method), P|intree|C<sub>max</sub> (2-approximate list algorithm with a given order of the tasks on the list).
 *     - Due date criteria, models: 1|p<sub>j</sub>=1,prec,pmtn|L<sub>max</sub> (extended Liu algorithm), P|p<sub>j</sub>=1,intree|Lmax (the Brucker method).</li>
 *   <li> Independent tasks, models: 1||ΣC<sub>i</sub>, 1||ΣU<sub>i</sub> (the Hodgson method),</li>
 *   <li> Determining classical measures of the schedule cost: C<sub>max</sub>, L<sub>max</sub>, ΣC<sub>i</sub>, ΣU<sub>i</sub>, ΣT<sub>i</sub>.</li></ol></li>
 * <li> Independent set of a graph 
 * -# Edge maximal (the module %algorithm/matching.h): 2-approximate algorithm (greedy) and exact (Lawler 1976).
 * -# Stable matching in a bipartite graph (the module %algorithm/matching.h),
 * -# Vertex maximal: a series of polynomial heuristics (the module %algorithm/mis.h).</li>
 * <li> Recognizing and decomposition of families of graphs or multigraphs (the module %classes/detect.h):
 * -# Simple classes: empty graphs, cliques, paths, caterpillar, cycles, trees, forests, connected, complete bipartite, complete k-partite, regular, subcubic, block, almost k-trees, bipartite (with determining a maximum independent set).
 * -# Other: chordal graphs (with determining the representation of the tree of maximal cliques and a maximum independent set), comparability graphs (with determining the acyclic orientation of the edges, maximum clique and independent set), interval (with determining the interval representation), split graphs, cographs (with determining a maximum clique and a maximum independent set), prime graphs (in the sense of modular decomposition).</li>
 * <li>Operations on graphs (the module %classes/create.h):
 * -# For diagrams of relations: reflexive, symmetric and transitive closures;
 * -# Calculating line graphs (for directed and undirected graphs);
 * -# Products of graphs: Cartesian, tensor, lexicographic, strong (with the possibility of preserving links between graph components – arguments of the operation, and elements of the resultant graph).</li>
 * <li> Graph generators with given parameters for selected families (the module %classes/create.h): cliques, paths, cycles, fans, wheels, caterpillars, Petersen graph, complete k-partite, regular trees, random graphs or digraphs according to both Erdos-Renyi models.</li></ol>
 * 
 * Algorithms of the library are available as static methods of classes grouping them into consistent thematic parts. Usually they appear in several versions (overloading) with different functionalities. KOALA does not demand from the user to use the STL containers, but it can cooperate with STL structures. Procedures loading sequences or multi-sets (usually vertices or edges) are implemented as templates reading data within the ranges determined by iterators (used for reading) of undetermined type – one can use there iterators delivered by the object data structures (for example STL containers), and also pointers to table fields. Similar output sequences are supported by inserting iterators or pointers of the beginning of the output table. Complex labels assigned to vertices or edges in different computational problems (for example for the Bellman-Ford algorithm, the input edge lengths and the output distances of the vertices to the source node) are operated by object structures of associative tables mapping vertices or edges to appropriate labels (the module %container/assoctab.h, by default the KOALA code uses effective container AssocArray achieving the access time O(1) under certain circumstances respected by all functions of the library). Structures of labels characteristic for a given algorithm are defined in the classes containing it.
 * 
 * For an advanced user, KOALA offers also the possibility of flexible modification of some behaviors of procedures and types of their internal data structures – what can affect the efficiency of their execution. For this purpose, a mechanism of policy-based design for templates of procedures was used: class XXX being a set of procedures always has the template form XXXPar<DefaultStructs>, the parameter DefaultStructs of which contains the policy modifying the operation of the algorithms. Default policy of the procedures of the library are determined by the class AlgsDefaultSettings (the module %base/defs.h), while the standard shape of the object Graph and functioning of its methods are determined by the template GrDefaultSettings (the module %graph/graph.h).
 * 
 * \section sec4 Auxiliary elements
 * <ol><li>	Support of the text formats of graph coding:
 * -# Graph databases together with data (of various types) assigned to the vertices, edges and graphs:
 *     - Subset of the standard GraphML (the module %io/graphml.h (\ref DMiographml) with the usage of the external package <a href = "http://www.grinninglizard.com/tinyxml/">TinyXML</a>). 
 *     - Two new, readable for a human text formats, patterned on the list of neighbors and the sequence of edges (the module %io/text.h \ref iotxt).
 * -# Popular text code g6, applied for large sets of simple graphs (the module %io/g6.h \ref DMiog6).</li>
 * <li> A collection of templates of data structures, for example:
 * -# Associative tables storing values assigned to the vertices or edges (the module %container/assoctab.h).
 *     - Three kinds of one-dimensional tables of common interface (provided by the wrapper AssocTable): basic (AssocArray), two based on the hash table (HashMap, BiDiHashMap) and the possibility to use the STL map (by the above wrapper).
 *     - Four kinds of two-dimensional tables (AssocMatrix): for keys being ordered or unordered pairs (with taking the diagonal into account, or without doing that).
 * -# Effective heaps with fast merging (the module %container/heap.h): binomial heap, Fibonacci heap, pairing heap.
 * -# Views on a multigraph, that is, objects operating on vertices and edges of the original graph, but allowing for different perception of its structure without the necessity to copy it (the module %graph/view.h): the view on a subgraph, on an undirected graph or on a graph with reverted directed edges.
 * -# Very convenient in usage (at the cost of lower effectiveness) templates of sets with implementation of, for example, operations of algebra of sets in the form of overloaded operators (the module %container/set.h). KOALA offers three types of sets with a common interface (chosen by the processor constants): based on an ordered table, hash table and wrapper for the container from STL.
 * -# The structure of the quickly-joinable partitions (the module %container/joinsets.h). </li></ol>
 * 
 * \section sec5 Graph visualization and editing
 * In KOALA, two tool programs are available for the multigraph visualization and editing with the possibility of assigning different types of attributes of vertices and edges, operating the above mentioned text formats, and also allowing the visualization of the performance of the algorithm and acting as the GUI for programs using KOALA library (the communication with another process on the same computer or in the form of a client-server web application).
 * 1.	Working in a web browser, multifunctional graph editor <a href = "http://www.kaims.pl/koala/zgred/">Zgred</a> (web application, JavaScript).
 * 2.	Stationary program designed for editing graph databases GraphEditor (Java NetBeans 7 project).
 * 
 * \section sec6 Summary
 * KOALA is one of the widest, open source platforms for analyzing graph and network structures, serviceable for direct applying in practice, or as a research tool. The package was developed within the research project of the Polish National Science Centre N N519 405837.
 * 
 * The created software is open source. However, while using it in an own application or for research, a citation will be appreciated.
 * 
 */

/** \page wiki Wikipedia
 */

/** \page links Links
 * 
 * - <a href = "../koala.zip"> KOALA sources </a>
 * - <a href = "modules.html">Documentation</a> of the KOALA library and <a href = "examples/index.html">examples</a> of the codes using it
 * - <a href = "http://www.kaims.pl/koala/zgred/">Graph editor Zgred</a>
 * - <a href = "../GraphEditor.zip">Program GraphEditor</a>
 * 
 * �	Contact (email): koala@kaims.pl
 */

/** \page authors Authors 
 * 
 * - <a href = "http://kaims.pl/~giaro/">Krzysztof Giaro</a> - project Manager, designer, programmer, code auditor
 * - <a href = "http://www.kaims.pl/~kmocet">Krzysztof Ocetkiewicz</a> - designer, programmer, tester
 * - <a href = "http://kaims.pl/~jendrek/">Andrzej Jastrzebski</a> - designer, programmer
 * - <a href = "http://www.eti.pg.gda.pl/katedry/kams/pracownicy/Krzysztof.Turowski/">Krzysztof Turowski</a> - programmer
 * - <a href = "http://www.kaims.pl/~robert/">Robert Janczewski</a> - programmer
 * - <a href = "http://www.kaims.pl/~obszar/">Pawel Obszarski</a> - documentation developer
 * - <a href = "http://www.kaims.pl/~goluch/">Tomasz Goluch</a> - documentation developer
 * - <a href = "http://www.eti.pg.gda.pl/katedry/kams/pracownicy/Marcin.Jurkiewicz/">Marcin Jurkiewicz</a> - tester
 */