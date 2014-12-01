#ifndef KOALA_DRAWCREATOR_H
#define KOALA_DRAWCREATOR_H

#include "../base/defs.h"
#include "../io/text.h"
#include "../classes/create.h"
#include "../graph/graph.h"

#define PI 3.14159265358979323846
#define EPSILON 0.000001

namespace Koala
{
	//NEW: Graph<ParSet, ParSet> generators
	/* The utility class for creating different types of graphs that can be visualized in Zgred.
	   Methods of the class assume that the info objects related to vertices and edges 
	   are of type Koala::IO::ParSet. The methods create infos for vertices containing
	   x,y coordinates. 
		
	   Names of the public methods correspond to methods defined in create.h header file,
	   in particular they correspond to methods of the same names defined in classes: Creator, LineGraph and ProductPar.
	   Similarly, the names of the parameters of the methods correspond to parameters of related methods defined in above-mentioned classes.

		Parameters of the methods:
		- GraphType &g - in/out graph, in most cases should be of type Graph<ParSet, ParSet>,
		- int n - the number of vertices of the graph that should be created,
		- double size - the size of the square in which the graph should be placed, i.e., the graph
		is drawn in a square with top left corner at (0,0) and bottom right corner at (size,size).
		- EdgeDirection dir - direction of edges to creates (compare the description for original methods),
	 */
	
	class DrawCreator {
		typedef Graph<Koala::IO::ParSet, Koala::IO::ParSet> ParSetGraph;
	
	public:
		//name of the parameter  that used to indicate X axis in communication with Zgred
		static std::string X_PARAM; 
		//name of the parameter  that used to indicate Y axis in communication with Zgred
		static std::string Y_PARAM; 
		
		static ParSetGraph::PVertex empty(ParSetGraph &g, int n, double size);
		
		static ParSetGraph::PVertex clique(ParSetGraph &g, int n, double size, EdgeDirection dir = EdUndir);
		
		static ParSetGraph::PVertex cycle(ParSetGraph &g, int n, double size, EdgeDirection dir = EdUndir);
		
		static ParSetGraph::PVertex petersen(ParSetGraph &g, double size);

		/*
		 - bool horizontal - whether to draw the path vertically (true) or horizontally (false)
		*/
		
		static ParSetGraph::PVertex path(ParSetGraph &g, int n, double size, bool horizontal = true, EdgeDirection dir = EdUndir);
		
		static ParSetGraph::PVertex wheel(ParSetGraph &g, int n, double size, EdgeDirection centerDir = EdUndir, EdgeDirection borderDir = EdUndir);
		
		static ParSetGraph::PVertex fan(ParSetGraph &g, int n, double size, EdgeDirection centerDir = EdUndir, EdgeDirection borderDir = EdUndir);
	
		static std::pair< typename ParSetGraph::PVertex, typename ParSetGraph::PVertex > compBipartite(ParSetGraph &g, int n1, int n2, double size, EdgeDirection dir = EdUndir);
	
		static ParSetGraph::PVertex compKPartite(ParSetGraph &g, int K, int k, double size, EdgeDirection dir = EdUndir);

		/*
		- std::vector<int> &partitions - sizes of partitions to create, the size of the vector defienes the number of partitions
		*/
		static ParSetGraph::PVertex compKPartite(ParSetGraph &g, const std::vector<int> &partitions, double size, EdgeDirection dir = EdUndir);
		
		static ParSetGraph::PVertex regTree(ParSetGraph &g, int deg, int height, double size, EdgeDirection dir = EdUndir);

		/*
		- std::vector<int> &degrees - degrees of vertices on partivuar levels degs[0] defines degree of the root,
									degs[1] defiend degrees of childeres of the root, and so on...,
									the size of the vector defienes the height of the tree, empty vectors means there is only the root
		*/
		static ParSetGraph::PVertex tree(ParSetGraph &g, const std::vector<int> &degrees, double size, EdgeDirection dir = EdUndir);

		static ParSetGraph::PVertex caterpillar(ParSetGraph &g, int pathVertNum, int legNum, double size, EdgeDirection dir = EdUndir);
		
		/*
		- std::vector<int> &legs - number of legs attached to consequent vertices on the central path, the size of the vector defienes the number of vertices in central path 
		*/
		static ParSetGraph::PVertex caterpillar(ParSetGraph &g, const std::vector<int> &legs, double size, EdgeDirection pathDir = EdUndir, EdgeDirection legDir = EdUndir);

		template <class RndGen>
		static ParSetGraph::PVertex erdRen1(RndGen& rgen, ParSetGraph &g, int n, double p, double size, EdgeType type = Undirected);

		template <class RndGen>
		static ParSetGraph::PVertex erdRen2(RndGen& rgen, ParSetGraph &g, int n, int m, double size, EdgeType type = Undirected);

		template <class RndGen>
		static ParSetGraph::PVertex barAlb(RndGen& rgen, ParSetGraph &g, int n, int k, double size, EdgeDirection type = Undirected, bool shuffle = false);

		template <class RndGen>
		static ParSetGraph::PVertex wattStrog1(RndGen& rgen, ParSetGraph &g, int n, int k, double beta, double size, EdgeType type = Undirected, bool shuffle = false);

		template <class RndGen>
		static ParSetGraph::PVertex wattStrog2(RndGen& rgen, ParSetGraph &g, int n, int k, double beta, double size, EdgeType type = Undirected, bool shuffle = false);

		/* Line graphs */

		/*
		Methods for drawing line graphs. 
		First input graph is scaled to fill in the square of top left corner = (0,0) and bottom right corner = (size, size).
		Both dimensions x,y are scaled using the same value.
		If there is an edge between vertices (x1,y1) and (x2,y2) then the vertex corresponding to the edge
		in the line graph receives coordinates ((x1+x2)/2, (y1+y2)/2). If there are parallel edges, then the line between
		source vertices is divided equally and the vertices corresponding to the edges are placed in division points.
		Vertices corresponding to loops are placed on the line between a particular source vertex and the middle point (size/2, size/2)
		in such a way that the first loops is placed in the same point as its source vertex.
		Parameters:
		g - input graph,
		lg - line graph of g,
		size - the size of the square where the line graph is drawn,
		copyVertParam - the name of the parameter in vertex infos of g which should be copied to edge infos in lg,
						empty string ("") means that copying should not be performed.
		copyEdgeParam - the name of the parameter in edge infos of g which should be copied to vertex infos in lg,
						empty string ("") means that copying should not be performed.
		*/
		
		static ParSetGraph::PVertex undir(ParSetGraph &g, ParSetGraph &lg, double size, std::string copyVertParam = "", std::string copyEdgeParam = "");

		static ParSetGraph::PVertex dir(ParSetGraph &g, ParSetGraph &lg, double size, std::string copyVertParam = "", std::string copyEdgeParam = "");

		/* Products */
		/*
		Methods for drawing graph products. 
		The first graph is placed on the top of output rectangle along the X axis.
		The second graph is placed on the left of output rectangle along the Y axis.
		Let dX, dY means the size of the graph in X and Y axis, respectively. 
		
		If the Y size of the first graph is > 0.2*sizeY then the graph is rescaled so as dY1 = 0.2*sizeY.		
		If the X size of the second graph is > 0.2*sizeX then the graph is  rescaled so as dX2 = 0.2*sizeX.
		The destined sizes of the graphs are defined so that the product graph fills tightly the output rectangle, i.e.:
		dX1 = sizeX - dX2,
		dY2 = sizeY - dY1.
		Here, X,Y dimensions are scaled using different values.	
		The vertex in the product graph obtained from vertices (x1,y1), (x2,y2) is plated at  (x1+x2,y1+y2).
		
		g1 - the first input graph,
		g2 - the second input graph,
		g - output graph product,
		sizeX - X size of the rectangle in which the product graph should be drawn, 
		sizeX - y size of the rectangle in which the product graph should be drawn,
		copyVertParam - pair of names of parameters that should be copied from vertex infos of input graphs
						to product graph infos. The first string corresponds to the first graph,
						second string corresponds to the second graph. Empty string means that copying should
						not be performed.
		copyEdgeParam - pair of names of parameters that should be copied from edge infos of input graphs
						to product graph infos. The first string corresponds to the first graph,
						second string corresponds to the second graph. Empty string means that copying should
						not be performed.					
		*/
		static ParSetGraph::PVertex cart(const ParSetGraph &g1, const ParSetGraph &g2, ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam = std::make_pair("", ""), std::pair<std::string, std::string> copyEdgeParam = std::make_pair("", ""));

		static ParSetGraph::PVertex lex(const ParSetGraph &g1, const ParSetGraph &g2, ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam = std::make_pair("", ""), std::pair<std::string, std::string> copyEdgeParam = std::make_pair("", ""));

		static ParSetGraph::PVertex tensor(const ParSetGraph &g1, const ParSetGraph &g2, ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam = std::make_pair("", ""), std::pair<std::string, std::string> copyEdgeParam = std::make_pair("", ""));

		static ParSetGraph::PVertex strong(const ParSetGraph &g1, const ParSetGraph &g2, ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam = std::make_pair("", ""), std::pair<std::string, std::string> copyEdgeParam = std::make_pair("", ""));

	protected:
		/*
		* Functors from creating Koala::IO::ParSet info for vertices
		*/

		/*
		* Drawing vertices in vertical or horizontal line.
		*/
		class ParSetPathFun
		{
		protected:
			int n; // number of vertices 
			double size; //the size of the square where the graph should be placed
			bool horiz;	//if true then drawing a path horizontally, if false - vertically
		public:
			ParSetPathFun(int vertexNum, double asize, bool ahoriz);
			Koala::IO::ParSet operator() (int num);
		};
		
		/*
		* Drawing vertices on a circle of radius = size/2.
		*/
		class ParSetCircleFun
		{
		protected:
			int n; // number of vertices 
			double size; //the size of the square where the graph should be placed
		public:
			ParSetCircleFun(int vertexNum, double asize);
			Koala::IO::ParSet operator() (int num);
		};

		/*
		* Drawing the Petersen graph.
		*/
		class ParSetPetersenFun
		{
		protected:
			double size; //the size of the square where the graph should be placed
		public:
			ParSetPetersenFun(double asize);
			Koala::IO::ParSet operator() (int num);
		};

		/*
		* Drawing a wheel graph.
		*/
		class ParSetWheelFun
		{
		protected:
			int n; // number of vertices 
			double size; //the size of the square where the graph should be placed
		public:
			ParSetWheelFun(int vertexNum, double asize);
			Koala::IO::ParSet operator() (int num);
		};

		/*
		* Drawing a fan graph.
		*/
		class ParSetFanFun
		{
		protected:
			int n; // number of vertices 
			double size; //the size of the square where the graph should be placed
		public:
			ParSetFanFun(int vertexNum, double asize);
			Koala::IO::ParSet operator() (int num);
		};

		/*
		* Drawing a complete bipartite graph.
		*/
		class ParSetCompBipartFun
		{
		protected:
			int n1; // number of vertices in the first partition
			int n2; // number of vertices in the second partition
			double size; //the size of the square where the graph should be placed
		public:
			ParSetCompBipartFun(int vertexNum1, int vertexNum2, double asize);
			Koala::IO::ParSet operator() (int num);
		};

		/*
		* Drawing a complete k-partite graph.
		*/
		class ParSetCompKPartFun
		{
		protected:
			double size; //the size of the square where the graph should be placed
			std::pair<std::vector<Koala::IO::ParSet>, int> *pt;
		public:
			ParSetCompKPartFun(const std::vector<int> &partitions, double asize);
			~ParSetCompKPartFun();
			ParSetCompKPartFun(const ParSetCompKPartFun& x);
			ParSetCompKPartFun& operator=(const ParSetCompKPartFun& x);
			Koala::IO::ParSet operator() (int num);
		};
		
		/*
		* Drawing a tree graph.
		*/
		class ParSetTreeFun
		{
		protected:
			int height;
			double size; //the size of the square where the graph should be placed
			std::pair<std::vector<std::pair<double, double> >, int> *pt;
		public:
			ParSetTreeFun(const std::vector<int> &degrees, double asize);
			~ParSetTreeFun();
			ParSetTreeFun(const ParSetTreeFun& x);
			ParSetTreeFun& operator=(const ParSetTreeFun& x);
			Koala::IO::ParSet operator() (int num);
		};
		
		/*
		* Drawing a caterpillar graph.
		*/
		class ParSetCaterpillarFun 
		{
		protected:
			double size; //the size of the square where the graph should be placed
			std::pair<std::vector<std::pair<double, double> >, int> *pt;
		public:
			ParSetCaterpillarFun(const std::vector<int> &legs, double asize);
			~ParSetCaterpillarFun();
			ParSetCaterpillarFun(const ParSetCaterpillarFun& x);
			ParSetCaterpillarFun& operator=(const ParSetCaterpillarFun& x);
			Koala::IO::ParSet operator() (int num);
		protected:
			double calcXvalue(int pointNum, int legNum, double xc, double legsAreaWidh);
		};

		/*
		* Casters used to creating Koala::IO::ParSet info for line graphs
		*/
		
		/*
		This caster is responsible for copying the value and its type of ParSet field
		specified by copyParam from input PasSet object to output ParSet object.
		In particular it is used to copy value from vertex info of given graph to
		edge infos of its line graph.
		*/
		class CopyParamLineGraphCaster
		{
		protected:
			std::string param;	
		public:
			CopyParamLineGraphCaster(std::string copyParam = "");
			void operator()(Koala::IO::ParSet& dest, Koala::IO::ParSet src);
		};
		
		/*
		This caster is responsible for copy x,y coordinates from edge infos of given graph
		to vertex infos of its line graph. Additionally, copying of selected parameter is 
		also performed (similar to CopyParamLineGraphCaster).
		*/
		class DrawLineGraphCaster : public CopyParamLineGraphCaster
		{
		public:
			DrawLineGraphCaster(std::string copyParam = "");
			void operator()(Koala::IO::ParSet& dest, Koala::IO::ParSet src);
		};

		/*
		* Casters used to creating Koala::IO::ParSet info for graph products
		*/
		
		/*
		This caster is responsible for copying the value and its type of ParSet field
		specified by copyParam from input PasSet object to output ParSet object.
		*/
		class CopyParamProductCaster
		{
		protected:
			std::pair<std::string, std::string> param;

		public:
			CopyParamProductCaster(std::pair<std::string, std::string> copyParam = std::make_pair("", ""));
			void operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, const Koala::IO::ParSet &src2);
			void operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, Koala::BlackHole b);
			void operator()(Koala::IO::ParSet& dest, Koala::BlackHole b, const Koala::IO::ParSet &src2);
		};
		
		/*
		This caster is responsible for computing and setting x,y coordinates of graph product vertices
		based on x,y coordinates of input graphs vertices. The output coordinates are sums of 
		respective coordinates in input graphs.	Additionally, copying of selected parameter is 
		also performed (similar to CopyParamProductCaster).
		*/
		class DrawProductCaster : public CopyParamProductCaster
		{
		public:
			DrawProductCaster(std::pair<std::string, std::string> copyParam = std::make_pair("", ""));
			void operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, const Koala::IO::ParSet &src2);
			void operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, Koala::BlackHole b);
			void operator()(Koala::IO::ParSet& dest, Koala::BlackHole b, const Koala::IO::ParSet &src2);
		};

		/*
		* Helper structure for storing coordinates of rectangles
		*/
		typedef struct rect {
			double x1;	//x - axis coordinate of top left corner
			double y1;	//y - axis coordinate of top left corner
			double x2;	//x - axis coordinate of bottom right corner
			double y2;	//y - axis coordinate of bottom right corner
		} Rect;

		/*
		* Helper methods.
		*/
		
		/*
		 Returns the value of filed of name specified by name parameter of given ParSet object converted to double from parse
		*/
		static double getValueAsDouble(const Koala::IO::ParSet &parSet, const std::string &name);
		
		/*
		 Copies the type and the value of filed specified by name parameter from ParSet in object to ParSet out object. 
		*/	
		static void fieldCopy(const Koala::IO::ParSet &in, Koala::IO::ParSet &out, const std::string &name);
		
		/*
		 Computes the x,y coordinates of top left and bottom right corners of the smallest rectangle containing given graph.
		*/	

		static void findRect(const ParSetGraph &g, Rect &rect);
		
		/*
		Perform linear scaling of the input graph into the rectangle of top left corner = (0,0) and bottom right corner = (sizeX,sizeY).
		This method keeps aspect ratio, i.e., both x and y coordinates are scaled using the same value.
		*/
		static void scaleEq(ParSetGraph &g, double sizeX, double sizeY, const Rect &rect);
		/*
		Perform linear scaling of the input graph into the rectangle of top left corner = (0,0) and bottom right corner = (sizeX,sizeY).
		This method does not keep aspect ratio, i.e., x,y coordinates can be scaled using different values.
		*/
		static void scale(ParSetGraph &g, double sizeX, double sizeY, const Rect &rect);
	
		/*
		Creates a template graph for line graph creating methods.
		 1. This method performs scaling (using ecaleEq method) of original graph into rectangle defined by sizeX and sizeY.
		 2. Next, x,y coordinates are set in ParSet edge infos, this values will be used to define
			the positions of vertices corresponding to the edges in line graph of the graph.
		*/
		static void lineGraphTemp(const ParSetGraph &in, ParSetGraph &out, double sizeX, double sizeY);
		
		/*
		Creates a template graph for graph product methods. 
		This method performs scaling (using scale method) of original graphs into rectangle defined by sizeX and sizeY.
		*/
		static void productTemp(const ParSetGraph &g1, const ParSetGraph &g2, ParSetGraph &temp1, ParSetGraph &temp2, double sizeX, double sizeY);
	};

}

template<class RndGen>
Koala::DrawCreator::ParSetGraph::PVertex Koala::DrawCreator::erdRen1(RndGen& rgen, Koala::DrawCreator::ParSetGraph &g, int n, double p, double size, Koala::EdgeType type) {
	return Koala::Creator::erdRen1(rgen, g, n, p, Koala::DrawCreator::ParSetCircleFun(n, size), Koala::ConstFunctor< Koala::DrawCreator::ParSetGraph::EdgeInfoType >(), type);
}

template<class RndGen>
Koala::DrawCreator::ParSetGraph::PVertex Koala::DrawCreator::erdRen2(RndGen& rgen, Koala::DrawCreator::ParSetGraph &g, int n, int m, double size, Koala::EdgeType type) {
	return Koala::Creator::erdRen2(rgen, g, n, m, Koala::DrawCreator::ParSetCircleFun(n, size), Koala::ConstFunctor < Koala::DrawCreator::ParSetGraph::EdgeInfoType >(), type);
}

template <class RndGen>
Koala::DrawCreator::ParSetGraph::PVertex Koala::DrawCreator::barAlb(RndGen& rgen, ParSetGraph &g, int n, int k, double size, EdgeDirection type, bool shuffle) {
	return Koala::Creator::barAlb(rgen, g, n, k, Koala::DrawCreator::ParSetCircleFun(n, size), Koala::ConstFunctor < Koala::DrawCreator::ParSetGraph::EdgeInfoType >(), type, shuffle);
}

template <class RndGen>
Koala::DrawCreator::ParSetGraph::PVertex Koala::DrawCreator::wattStrog1(RndGen& rgen, ParSetGraph &g, int n, int k, double beta, double size, EdgeType type, bool shuffle) {
	return Koala::Creator::wattStrog1(rgen, g, n, k, beta, Koala::DrawCreator::ParSetCircleFun(n, size), Koala::ConstFunctor < Koala::DrawCreator::ParSetGraph::EdgeInfoType >(), type, shuffle);
}

template <class RndGen>
Koala::DrawCreator::ParSetGraph::PVertex Koala::DrawCreator::wattStrog2(RndGen& rgen, ParSetGraph &g, int n, int k, double beta, double size, EdgeType type, bool shuffle) {
	return Koala::Creator::wattStrog2(rgen, g, n, k, beta, Koala::DrawCreator::ParSetCircleFun(n, size), Koala::ConstFunctor < Koala::DrawCreator::ParSetGraph::EdgeInfoType >(), type, shuffle);
}

#endif