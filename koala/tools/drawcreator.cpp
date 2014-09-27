
#include "drawcreator.h"

using namespace Koala;

std::string DrawCreator::X_PARAM = "x";
std::string DrawCreator::Y_PARAM = "y";

DrawCreator::ParSetPathFun::ParSetPathFun(int vertexNum, double asize, bool ahoriz) : n(vertexNum), size(asize), horiz(ahoriz) {}

Koala::IO::ParSet DrawCreator::ParSetPathFun::operator() (int num)
{
	Koala::IO::ParSet parSet;
	assert(n > 0);
	if (n == 1)
	{
		parSet.set(DrawCreator::X_PARAM, (double)(size / 2.0));
		parSet.set(DrawCreator::Y_PARAM, (double)(size / 2.0));
		return parSet;
	}

	double p1 = size / 2.0;
	double p2 = ((double)num / (double)(n - 1)) * size;
	if (horiz)
	{
		parSet.set(DrawCreator::X_PARAM, p2);
		parSet.set(DrawCreator::Y_PARAM, p1);
	}
	else
	{
		parSet.set(DrawCreator::X_PARAM, p1);
		parSet.set(DrawCreator::Y_PARAM, p2);
	}
	return parSet;
}

DrawCreator::ParSetCircleFun::ParSetCircleFun(int vertexNum, double asize) : n(vertexNum), size(asize) {}

Koala::IO::ParSet DrawCreator::ParSetCircleFun::operator() (int num)
{
	Koala::IO::ParSet parSet;
	assert(n > 0);
	if (n == 1)
	{
		parSet.set(DrawCreator::X_PARAM, (double)(size / 2.0));
		parSet.set(DrawCreator::Y_PARAM, (double)(size / 2.0));
		return parSet;
	}

	double r = size / 2.0;
	double alfa = 2.0 * PI * num / (double)n;
	double x = r * cos(alfa);
	double y = r * sin(alfa);
	x += r;
	y = -y + r;
	parSet.set(DrawCreator::X_PARAM, x);
	parSet.set(DrawCreator::Y_PARAM, y);
	return parSet;
}

DrawCreator::ParSetPetersenFun::ParSetPetersenFun(double asize) : size(asize) {	}

Koala::IO::ParSet DrawCreator::ParSetPetersenFun::operator() (int num) {
	Koala::IO::ParSet parSet;
	int alfaNum;
	double r;
	double alfa0 = PI / 2.0;
	double delta = size / 2.0;
	if (num < 5) {
		//outer circle
		alfaNum = num;
		r = size / 2.0;
	}
	else {
		//inner star
		switch (num)
		{
		case 8: alfaNum = 0; break;
		case 6: alfaNum = 1; break;
		case 9: alfaNum = 2; break;
		case 7: alfaNum = 3; break;
		case 5: alfaNum = 4; break;
		}
		r = size / 4.0;
	}

	double alfa = alfa0 - 2.0 * PI * alfaNum / 5.0;
	double x = r * cos(alfa);
	double y = r * sin(alfa);
	x += delta;
	y = -y + delta;
	parSet.set(DrawCreator::X_PARAM, x);
	parSet.set(DrawCreator::Y_PARAM, y);
	return parSet;
}

DrawCreator::ParSetWheelFun::ParSetWheelFun(int vertexNum, double asize) : n(vertexNum), size(asize) { }

Koala::IO::ParSet DrawCreator::ParSetWheelFun::operator() (int num) {
	Koala::IO::ParSet parSet;
	assert(n > 0);
	if (n < 4)
	{
		ParSetCircleFun circleFun(n, size);
		return circleFun(num);
	}

	if (num == 0)
	{
		parSet.set(DrawCreator::X_PARAM, (double)(size / 2.0));
		parSet.set(DrawCreator::Y_PARAM, (double)(size / 2.0));
		return parSet;
	}

	ParSetCircleFun circleFun(n - 1, size);
	return circleFun(num - 1);
}

DrawCreator::ParSetFanFun::ParSetFanFun(int vertexNum, double asize) : n(vertexNum), size(asize) { }

Koala::IO::ParSet DrawCreator::ParSetFanFun::operator() (int num) {
	Koala::IO::ParSet parSet;
	assert(n > 0);
	if (n < 4)
	{
		ParSetCircleFun circleFun(n, size);
		return circleFun(num);
	}

	if (num == 0)
	{
		parSet.set(DrawCreator::X_PARAM, (double)(size / 2.0));
		parSet.set(DrawCreator::Y_PARAM, (double)(size));
		return parSet;
	}

	double r = size / 2.0;
	double alfa = PI *  (num - 1) / (double)(n - 2);
	double x = r * cos(alfa);
	double y = r * sin(alfa);
	x += r;
	y = -y + r;
	parSet.set(DrawCreator::X_PARAM, x);
	parSet.set(DrawCreator::Y_PARAM, y);
	return parSet;
}

DrawCreator::ParSetCompBipartFun::ParSetCompBipartFun(int vertexNum1, int vertexNum2, double asize) :
n1(vertexNum1), n2(vertexNum2), size(asize) { }

Koala::IO::ParSet DrawCreator::ParSetCompBipartFun::operator() (int num) {
	Koala::IO::ParSet parSet;
	assert(n1 > 0);
	assert(n2 > 0);
	double x, y;
	if (num < n1)
	{
		//first partition
		x = 0;
		if (n1 == 1) y = size / 2.0;
		else y = num * (size / (double)(n1 - 1));
	}
	else
	{
		//second partition
		x = size;
		if (n2 == 1) y = size / 2.0;
		else y = (num - n1) * (size / (double)(n2 - 1));
	}
	parSet.set(DrawCreator::X_PARAM, x);
	parSet.set(DrawCreator::Y_PARAM, y);
	return parSet;
}

DrawCreator::ParSetCompKPartFun::ParSetCompKPartFun(const std::vector<int> &partitions, double asize) : size(asize)
{

	int K = 0; //number of partitions
	int n = 0, k, i, vertNum, partNum;
	for (std::vector<int>::const_iterator iter = partitions.begin(); iter != partitions.end(); iter++)
	{
		n += *iter;
	}
	K = partitions.size();
	assert(K > 0);

	pt = new std::pair<std::vector<Koala::IO::ParSet>, int>();
	pt->first.reserve(n); //reserve space for n points
	pt->second = 1;

	if (K == 1)
	{
		//single partition -> complete graph
		ParSetCircleFun parSetCircleFun(*partitions.begin(), size);
		for (i = 0; i < n; i++)
		{
			pt->first.push_back(parSetCircleFun(i));
		}
	}
	else if (K == 2)
	{
		//two partitons - > compete bipartite graph
		ParSetCompBipartFun parSetCompBipartFun(*partitions.begin(), *(partitions.begin() + 1), size);
		for (i = 0; i < n; i++)
		{
			pt->first.push_back(parSetCompBipartFun(i));
		}
	}
	else
	{
		//more than two partitions

		double r, alfaFree, alfaUsed, alfa1, alfa2, x, x1, x2, y, y1, y2;
		Koala::IO::ParSet parSet;
		alfaFree = (2 * PI) / 1.5;
		alfaUsed = (2 * PI) - alfaFree;
		r = size / 2.0;

		partNum = 1; // from 1 to K
		for (std::vector<int>::const_iterator iter = partitions.begin(); iter != partitions.end(); iter++)
		{
			k = *iter;
			alfa2 = (2 * PI / K) * partNum;
			alfa1 = alfa2 - alfaUsed / K;
			x1 = r * cos(alfa1);
			x2 = r * cos(alfa2);
			y1 = r * sin(alfa1);
			y2 = r * sin(alfa2);
			for (vertNum = 1; vertNum <= k; vertNum++)
			{
				if (k == 1)
				{
					x = (x1 + x2) / 2.0;
					y = (y1 + y2) / 2.0;

				}
				else
				{
					x = x1 + ((x2 - x1) / (k - 1)) * (vertNum - 1);
					y = y1 + ((y2 - y1) / (k - 1)) * (vertNum - 1);
				}

				x += r;
				y = -y + r;

				parSet.set(DrawCreator::X_PARAM, x);
				parSet.set(DrawCreator::Y_PARAM, y);
				pt->first.push_back(parSet);
			}
			partNum++;
		}
	}
}

DrawCreator::ParSetCompKPartFun::~ParSetCompKPartFun()
{
	if (!(--pt->second))
		delete pt;
}

DrawCreator::ParSetCompKPartFun::ParSetCompKPartFun(const DrawCreator::ParSetCompKPartFun& x) : size(x.size), pt(x.pt)
{
	pt->second++;
}

DrawCreator::ParSetCompKPartFun& DrawCreator::ParSetCompKPartFun::operator=(const ParSetCompKPartFun& x)
{
	if (&x == this) return *this;
	this->~ParSetCompKPartFun();
	size = x.size;
	pt = x.pt;
	pt->second++;
	return *this;
}

Koala::IO::ParSet DrawCreator::ParSetCompKPartFun::operator() (int num) {
	return pt->first[num];
}

DrawCreator::ParSetTreeFun::ParSetTreeFun(const std::vector<int> &degrees, double asize) : size(asize) {

	int lvl, childSize, num, n, d;
	bool isDegOdd;
	height = degrees.size();
	
	int LOCALARRAY(vertNum, height + 1);
	int LOCALARRAY(deg, height);
	num = 1;
	lvl = 1;
	n = 1;
	vertNum[0] = 1; // only the root is on level 0
	for (std::vector<int>::const_iterator iter = degrees.begin(); iter != degrees.end(); iter++)
	{
		childSize = *iter;
		num *= childSize;
		vertNum[lvl] = num;
		n += num;
		deg[lvl - 1] = childSize;
		lvl++;
	}

	pt = new std::pair<std::vector<std::pair<double, double> >, int>();
	pt->first.reserve(n); //reserve space for n points
	pt->second = 1;

	//case n == 1 is handled in operator() explicitely
	if (n > 1)
	{
		std::pair< double, double > LOCALARRAY(pointTab, n);
		double x, y, deltaY = size / height;
		int j, v, vc, vChild;

		//we are drawing the deepest level (leaves)
		if (vertNum[height] == 1)
		{
			pointTab[n - 1] = std::make_pair(size / 2.0, size);
		}
		else
		{
			j = 0;
			double deltaX = size / (vertNum[height] - 1);
			for (int i = n - vertNum[height]; i < n; i++)
			{
				pointTab[i] = std::make_pair(j * deltaX, size);
				j++;
			}
		}
		//here we have already drawn the deepsest level

		//start drawing on higher levels
		vChild = n;
		for (int l = height - 1; l >= 0; l--)
		{
			//we are drawing on l level based on already computed valuse for l+1 level
			vChild = vChild - vertNum[l + 1]; //set start vertex on l+1 level
			v = vChild - vertNum[l]; //set start vertex on l  level
			vc = vChild;
			while (v < vChild)
			{
				d = deg[l];
				isDegOdd = ((d & 1) == 1); //czy nieparzysty

				int m = vc + d / 2;
				y = pointTab[m].second - deltaY;
				if (isDegOdd) x = pointTab[m].first;
				else x = (pointTab[m - 1].first + pointTab[m].first) / 2.0;
				pointTab[v] = std::make_pair(x, y);
				v++;
				vc += d;
			}
		}

		for (int i = 0; i < n; i++) pt->first.push_back(pointTab[i]);
	}
}

DrawCreator::ParSetTreeFun::~ParSetTreeFun()
{
	if (!(--pt->second))
		delete pt;
}

DrawCreator::ParSetTreeFun::ParSetTreeFun(const DrawCreator::ParSetTreeFun& x) : height(x.height), size(x.size), pt(x.pt)
{
	pt->second++;
}

DrawCreator::ParSetTreeFun& DrawCreator::ParSetTreeFun::operator=(const ParSetTreeFun& x)
{
	if (&x == this) return *this;
	this->~ParSetTreeFun();
	height = x.height;
	size = x.size;
	pt = x.pt;
	pt->second++;
	return *this;
}

Koala::IO::ParSet DrawCreator::ParSetTreeFun::operator() (int num) {

	if (height == 0)
		return ParSetCircleFun(1, size)(num);

	Koala::IO::ParSet parSet;
	parSet.set(DrawCreator::X_PARAM, pt->first[num].first);
	parSet.set(DrawCreator::Y_PARAM, pt->first[num].second);
	return parSet;
}

DrawCreator::ParSetCaterpillarFun::ParSetCaterpillarFun(const std::vector<int> &legs, double asize) : size(asize)
{
	int legNum, pathVertNum = legs.size();
	double x, y, xc;
	int n = 0;
	for (std::vector<int>::const_iterator iter = legs.begin(); iter != legs.end(); iter++)
	{
		n++;
		n += *iter;
	}
	assert(pathVertNum > 0);
	pt = new std::pair<std::vector<std::pair<double, double> >, int>();
	pt->first.reserve(n); //reserve space for n points
	pt->second = 1;
	double deltaX = size / pathVertNum;
	double legsAreaWidh = 0.7 * deltaX;

	if (pathVertNum == 1)
	{
		//central path has only one vertex
		legNum = *legs.begin();

		//drawing the singe vertex on the cental path
		x = size / 2.0;
		y = size / 2.0;
		pt->first.push_back(std::make_pair(x, y));

		//drawing legs	
		for (int i = 0; i < legNum; i++)
		{
			xc = size / 2.0;
			if ((i & 1) == 0) y = size;
			else y = 0;
			x = calcXvalue(i, legNum, xc, legsAreaWidh);
			pt->first.push_back(std::make_pair(x, y));
		}
	}
	else
	{
		//central path has at least two vertices
		//drawing the central path
		for (int i = 0; i < pathVertNum; i++)
		{
			x = (i + 0.5) * deltaX;
			y = size / 2.0;
			pt->first.push_back(std::make_pair(x, y));
		}

		//drawing legs	
		int pathCenterNum = 0; //0-based index of the corresponding vertex on the cental path
		for (std::vector<int>::const_iterator iter = legs.begin(); iter != legs.end(); iter++)
		{
			legNum = *iter;
			for (int i = 0; i < legNum; i++)
			{
				xc = (pathCenterNum + 0.5) * deltaX;
				if ((i & 1) == 0) y = size;
				else y = 0;
				x = calcXvalue(i, legNum, xc, legsAreaWidh);
				pt->first.push_back(std::make_pair(x, y));
			}
			pathCenterNum++;
		}
	}
}

DrawCreator::ParSetCaterpillarFun::~ParSetCaterpillarFun()
{
	if (!(--pt->second))
		delete pt;
}

DrawCreator::ParSetCaterpillarFun::ParSetCaterpillarFun(const ParSetCaterpillarFun& x) : size(x.size), pt(x.pt)
{
	pt->second++;
}

DrawCreator::ParSetCaterpillarFun& DrawCreator::ParSetCaterpillarFun::operator = (const ParSetCaterpillarFun& x)
{
	if (&x == this) return *this;
	this->~ParSetCaterpillarFun();
	size = x.size;
	pt = x.pt;
	pt->second++;
	return *this;
}

Koala::IO::ParSet DrawCreator::ParSetCaterpillarFun::operator() (int num)
{
	Koala::IO::ParSet parSet;
	parSet.set(DrawCreator::X_PARAM, pt->first[num].first);
	parSet.set(DrawCreator::Y_PARAM, pt->first[num].second);
	return parSet;
}

double DrawCreator::ParSetCaterpillarFun::calcXvalue(int pointNum, int legNum, double xc, double legsAreaWidh)
{
	//we are using 0-based index here - so first leg has index 0
	double x;
	int idx = pointNum / 2;
	int maxLegInLine = legNum / 2;
	if ((legNum & 1) == 1 && (pointNum & 1) == 0) maxLegInLine++;

	if (maxLegInLine < 2) x = xc;
	else x = xc - legsAreaWidh / 2.0 + idx * (legsAreaWidh / (maxLegInLine - 1));

	return x;
}


DrawCreator::ParSetGraph::PVertex DrawCreator::empty(DrawCreator::ParSetGraph &g, int n, double size){
	return Creator::empty(g, n, DrawCreator::ParSetCircleFun(n, size));
}

DrawCreator::ParSetGraph::PVertex DrawCreator::clique(DrawCreator::ParSetGraph &g, int n, double size, EdgeDirection dir){
	return Creator::clique(g, n, DrawCreator::ParSetCircleFun(n, size), Koala::ConstFunctor<DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::cycle(DrawCreator::ParSetGraph &g, int n, double size, EdgeDirection dir){
	return Creator::cycle(g, n, DrawCreator::ParSetCircleFun(n, size), Koala::ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::petersen(DrawCreator::ParSetGraph &g, double size){
	return Creator::petersen(g, DrawCreator::ParSetPetersenFun(size), Koala::ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >());
}

DrawCreator::ParSetGraph::PVertex DrawCreator::path(DrawCreator::ParSetGraph &g, int n, double size, bool horizontal, EdgeDirection dir){
	return Creator::path(g, n, DrawCreator::ParSetPathFun(n, size, horizontal), Koala::ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::wheel(DrawCreator::ParSetGraph &g, int n, double size, EdgeDirection centerDir, EdgeDirection borderDir){
	return Creator::wheel(g, n, DrawCreator::ParSetWheelFun(n, size), Koala::ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), centerDir, borderDir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::fan(DrawCreator::ParSetGraph &g, int n, double size, EdgeDirection centerDir, EdgeDirection borderDir){
	return Creator::fan(g, n, DrawCreator::ParSetFanFun(n, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), centerDir, borderDir);
}

std::pair< DrawCreator::ParSetGraph::PVertex, DrawCreator::ParSetGraph::PVertex > DrawCreator::compBipartite(DrawCreator::ParSetGraph &g, int n1, int n2, double size, EdgeDirection dir) {
	return Creator::compBipartite(g, n1, n2, DrawCreator::ParSetCompBipartFun(n1, n2, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::compKPartite(DrawCreator::ParSetGraph &g, int K, int k, double size, EdgeDirection dir) {
	std::vector<int> partitions(K, k);
	return Creator::compKPartite(g, K, k, blackHole, DrawCreator::ParSetCompKPartFun(partitions, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::compKPartite(DrawCreator::ParSetGraph &g, const std::vector<int> &partitions, double size, EdgeDirection dir) {
	return Creator::compKPartite(g, partitions.begin(), partitions.end(), blackHole, DrawCreator::ParSetCompKPartFun(partitions, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::regTree(DrawCreator::ParSetGraph &g, int deg, int height, double size, EdgeDirection dir) {
	std::vector<int> degrees(height, deg);
	return Creator::regTree(g, deg, height, DrawCreator::ParSetTreeFun(degrees, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::tree(DrawCreator::ParSetGraph &g, const std::vector<int> &degrees, double size, EdgeDirection dir) {
	return Creator::regTree(g, degrees.begin(), degrees.end(), DrawCreator::ParSetTreeFun(degrees, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::caterpillar(DrawCreator::ParSetGraph &g, int pathVertNum, int legNum, double size, EdgeDirection dir) {
	std::vector<int> legs(pathVertNum, legNum);
	return Creator::caterpillar(g, pathVertNum, legNum, DrawCreator::ParSetCaterpillarFun(legs, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), dir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::caterpillar(DrawCreator::ParSetGraph &g, const std::vector<int> &legs, double size, EdgeDirection pathDir, EdgeDirection legDir) {
	return Creator::caterpillar(g, legs.begin(), legs.end(), DrawCreator::ParSetCaterpillarFun(legs, size), ConstFunctor< DrawCreator::ParSetGraph::EdgeInfoType >(), pathDir, legDir);
}

DrawCreator::ParSetGraph::PVertex DrawCreator::undir(DrawCreator::ParSetGraph &g, DrawCreator::ParSetGraph &lg, double size, std::string copyVertParam, std::string copyEdgeParam) {
	DrawCreator::ParSetGraph temp;
	DrawCreator::lineGraphTemp(g, temp, size, size);
	return LineGraphPar<AlgsDefaultSettings>::undir(temp, lg, std::make_pair(DrawLineGraphCaster(copyEdgeParam), CopyParamLineGraphCaster(copyVertParam)));
}

DrawCreator::ParSetGraph::PVertex DrawCreator::dir(DrawCreator::ParSetGraph &g, DrawCreator::ParSetGraph &lg, double size, std::string copyVertParam, std::string copyEdgeParam) {
	DrawCreator::ParSetGraph temp;
	DrawCreator::lineGraphTemp(g, temp, size, size);
	return LineGraphPar<AlgsDefaultSettings>::dir(temp, lg, std::make_pair(DrawLineGraphCaster(copyEdgeParam), CopyParamLineGraphCaster(copyVertParam)));
}

DrawCreator::ParSetGraph::PVertex DrawCreator::cart(const DrawCreator::ParSetGraph &g1, const DrawCreator::ParSetGraph &g2, DrawCreator::ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam, std::pair<std::string, std::string> copyEdgeParam) {
	DrawCreator::ParSetGraph g1Temp;
	DrawCreator::ParSetGraph g2Temp;
	DrawCreator::productTemp(g1, g2, g1Temp, g2Temp, sizeX, sizeY);
	return ProductPar<AlgsDefaultSettings>::cart(g1Temp, g2Temp, g, std::make_pair(DrawProductCaster(copyVertParam), CopyParamProductCaster(copyEdgeParam)));
}

DrawCreator::ParSetGraph::PVertex DrawCreator::lex(const DrawCreator::ParSetGraph &g1, const DrawCreator::ParSetGraph &g2, DrawCreator::ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam, std::pair<std::string, std::string> copyEdgeParam) {
	DrawCreator::ParSetGraph g1Temp;
	DrawCreator::ParSetGraph g2Temp;
	DrawCreator::productTemp(g1, g2, g1Temp, g2Temp, sizeX, sizeY);
	return ProductPar<AlgsDefaultSettings>::lex(g1Temp, g2Temp, g, std::make_pair(DrawProductCaster(copyVertParam), CopyParamProductCaster(copyEdgeParam)));
}

DrawCreator::ParSetGraph::PVertex DrawCreator::tensor(const DrawCreator::ParSetGraph &g1, const DrawCreator::ParSetGraph &g2, DrawCreator::ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam, std::pair<std::string, std::string> copyEdgeParam) {
	DrawCreator::ParSetGraph g1Temp;
	DrawCreator::ParSetGraph g2Temp;
	DrawCreator::productTemp(g1, g2, g1Temp, g2Temp, sizeX, sizeY);
	return ProductPar<AlgsDefaultSettings>::tensor(g1Temp, g2Temp, g, std::make_pair(DrawProductCaster(copyVertParam), CopyParamProductCaster(copyEdgeParam)));
}

DrawCreator::ParSetGraph::PVertex DrawCreator::strong(const DrawCreator::ParSetGraph &g1, const DrawCreator::ParSetGraph &g2, DrawCreator::ParSetGraph &g, double sizeX, double sizeY, std::pair<std::string, std::string> copyVertParam, std::pair<std::string, std::string> copyEdgeParam) {
	DrawCreator::ParSetGraph g1Temp;
	DrawCreator::ParSetGraph g2Temp;
	DrawCreator::productTemp(g1, g2, g1Temp, g2Temp, sizeX, sizeY);
	return ProductPar<AlgsDefaultSettings>::strong(g1Temp, g2Temp, g, std::make_pair(DrawProductCaster(copyVertParam), CopyParamProductCaster(copyEdgeParam)));
}

void DrawCreator::productTemp(const DrawCreator::ParSetGraph &g1, const DrawCreator::ParSetGraph &g2, DrawCreator::ParSetGraph &temp1, DrawCreator::ParSetGraph &temp2, double sizeX, double sizeY) {
	double frac = 0.20;
	DrawCreator::Rect rect1, rect2;
	temp1.copy(g1);
	temp2.copy(g2);
	DrawCreator::findRect(temp1, rect1);
	DrawCreator::findRect(temp2, rect2);
	double dx1 = rect1.x2 - rect1.x1;
	double dy1 = rect1.y2 - rect1.y1;
	double dx2 = rect2.x2 - rect2.x1;
	double dy2 = rect2.y2 - rect2.y1;
	double sizeX1, sizeY1, sizeX2, sizeY2;
	double maxY1 = frac * sizeY;
	double maxX2 = frac * sizeX;

	if (dx2 < maxX2) sizeX2 = dx2;
	else sizeX2 = maxX2;
	sizeX1 = sizeX - sizeX2;

	if (dy1 < maxY1) sizeY1 = dy1;
	else sizeY1 = maxY1;
	sizeY2 = sizeY - sizeY1;

	DrawCreator::scale(temp1, sizeX1, sizeY1, rect1);
	DrawCreator::scale(temp2, sizeX2, sizeY2, rect2);
}

 void DrawCreator::findRect(const DrawCreator::ParSetGraph &g, DrawCreator::Rect &rect)
{
	double xMin = 0, yMin = 0, xMax = 0, yMax = 0, x, y;
	bool first = true;
	for (DrawCreator::ParSetGraph::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		Koala::IO::ParSet vInfo = g.getVertInfo(v);
		x = getValueAsDouble(vInfo, Koala::DrawCreator::X_PARAM);
		y = getValueAsDouble(vInfo, Koala::DrawCreator::Y_PARAM);

		if (first)
		{
			xMin = x; xMax = x;
			yMin = y; yMax = y;
			first = false;
		}
		else
		{
			if (x > xMax) xMax = x;
			if (x < xMin) xMin = x;
			if (y > yMax) yMax = y;
			if (y < yMin) yMin = y;
		}
	}
	rect.x1 = xMin;
	rect.y1 = yMin;
	rect.x2 = xMax;
	rect.y2 = yMax;
	return;
}


double DrawCreator::getValueAsDouble(const Koala::IO::ParSet & parSet, const std::string & name) {
	double val = 0;
	if (parSet.isDouble(name))
	{
		val = parSet.getDouble(name);
	}
	else if (parSet.isInt(name))
	{
		val = (double)parSet.getInt(name);
	}
	else
	{
		koalaAssert(false, AlgExcWrongArg);
	}
	return val;
}


void  DrawCreator::scaleEq(DrawCreator::ParSetGraph & g, double sizeX, double sizeY, const DrawCreator::Rect &rect) {

	double xMin, yMin, xMax, yMax, x, y, dx, dy;
	double kx = 0, ky = 0, k = 0;
	double xMid = sizeX / 2;
	double yMid = sizeY / 2;

	xMin = rect.x1;
	yMin = rect.y1;
	xMax = rect.x2;
	yMax = rect.y2;

	dx = xMax - xMin;
	dy = yMax - yMin;

	if (dx < EPSILON) dx = 0;
	if (dy < EPSILON) dy = 0;

	if (dx > 0) kx = sizeX / dx;
	if (dy > 0) ky = sizeY / dy;

	if (kx > 0 && ky > 0)	k = kx < ky ? kx : ky;
	else if (kx > 0) k = kx;
	else if (ky > 0) k = ky;

	for (DrawCreator::ParSetGraph::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		Koala::IO::ParSet vInfo = g.getVertInfo(v);
		x = getValueAsDouble(vInfo, Koala::DrawCreator::X_PARAM);
		y = getValueAsDouble(vInfo, Koala::DrawCreator::Y_PARAM);

		if (dx == 0) x = xMid;
		else x = (x - xMin) * k;

		if (dy == 0) y = yMid;
		else y = (y - yMin) * k;

		vInfo.set(Koala::DrawCreator::X_PARAM, x);
		vInfo.set(Koala::DrawCreator::Y_PARAM, y);
		g.setVertInfo(v, vInfo);
	}
	return;
}

void  DrawCreator::scale(DrawCreator::ParSetGraph & g, double sizeX, double sizeY, const DrawCreator::Rect &rect) {

	double xMin, yMin, xMax, yMax, x, y, dx, dy;
	double kx = 0, ky = 0;

	xMin = rect.x1;
	yMin = rect.y1;
	xMax = rect.x2;
	yMax = rect.y2;

	dx = xMax - xMin;
	dy = yMax - yMin;

	if (dx < EPSILON) dx = 0;
	if (dy < EPSILON) dy = 0;

	if (dx > 0) kx = sizeX / dx;
	if (dy > 0) ky = sizeY / dy;

	for (DrawCreator::ParSetGraph::PVertex v = g.getVert(); v; v = g.getVertNext(v))
	{
		Koala::IO::ParSet vInfo = g.getVertInfo(v);
		x = getValueAsDouble(vInfo, Koala::DrawCreator::X_PARAM);
		y = getValueAsDouble(vInfo, Koala::DrawCreator::Y_PARAM);
		x = (x - xMin) * kx;
		y = (y - yMin) * ky;
		vInfo.set(Koala::DrawCreator::X_PARAM, x);
		vInfo.set(Koala::DrawCreator::Y_PARAM, y);
		g.setVertInfo(v, vInfo);
	}
	return;
}

void DrawCreator::fieldCopy(const Koala::IO::ParSet &in, Koala::IO::ParSet &out, const std::string &name)
{
	Koala::IO::PSType type = in.getType(name);
	switch (type)
	{
	case Koala::IO::PST_Bool: out.set(name, in.getBool(name)); break;
	case Koala::IO::PST_Int: out.set(name, in.getInt(name)); break;
	case Koala::IO::PST_Double: out.set(name, in.getDouble(name)); break;
	case Koala::IO::PST_String: out.set(name, in.getString(name)); break;
	default: break;
	}
	return;
}

void  DrawCreator::lineGraphTemp(const DrawCreator::ParSetGraph & in, DrawCreator::ParSetGraph & out, double sizeX, double sizeY) {
	out.copy(in);
	Rect rect;
	DrawCreator::findRect(out, rect);
	DrawCreator::scaleEq(out, sizeX, sizeY, rect);
	double xMid = sizeX / 2;
	double yMid = sizeY / 2;

	int m = out.getEdgeNo(), mp, totParals, k;
	double xp, yp, xk, yk, dx, dy;
	Koala::IO::ParSet v1Info, v2Info, eInfo;
	DrawCreator::ParSetGraph::PEdge LOCALARRAY(edges, m);
	DrawCreator::ParSetGraph::PEdge LOCALARRAY(edgesParals, m);
	std::pair<int, int> sizep = out.findParals(std::make_pair(edges, blackHole), EdUndir);

	for (int i = 0; i < sizep.first; ++i) {
		std::pair<DrawCreator::ParSetGraph::PVertex, DrawCreator::ParSetGraph::PVertex> verts = out.getEdgeEnds(edges[i]);
		eInfo = out.getEdgeInfo(edges[i]);
		mp = out.getParals(edgesParals, edges[i], EdUndir);
		totParals = mp + 1;
		v1Info = out.getVertInfo(verts.first);
		xp = DrawCreator::getValueAsDouble(v1Info, Koala::DrawCreator::X_PARAM);
		yp = DrawCreator::getValueAsDouble(v1Info, Koala::DrawCreator::Y_PARAM);
		if (out.getEdgeType(edges[i]) != EdLoop)
		{
			v2Info = out.getVertInfo(verts.second);
			xk = DrawCreator::getValueAsDouble(v2Info, Koala::DrawCreator::X_PARAM);
			yk = DrawCreator::getValueAsDouble(v2Info, Koala::DrawCreator::Y_PARAM);

			dx = (xk - xp) / (totParals + 1);
			dy = (yk - yp) / (totParals + 1);
			k = 1;
		}
		else
		{
			//how we should handle loops?
			dx = (xMid - xp) / totParals;
			dy = (yMid - yp) / totParals;
			k = 0;
		}

		eInfo.set(Koala::DrawCreator::X_PARAM, xp + k * dx);
		eInfo.set(Koala::DrawCreator::Y_PARAM, yp + k * dy);
		out.setEdgeInfo(edges[i], eInfo);

		++k;
		for (int j = 0; j < mp; ++j)
		{
			eInfo = out.getEdgeInfo(edgesParals[j]);
			eInfo.set(Koala::DrawCreator::X_PARAM, xp + k * dx);
			eInfo.set(Koala::DrawCreator::Y_PARAM, yp + k * dy);
			out.setEdgeInfo(edgesParals[j], eInfo);
			++k;
		}
	}
	return;
}

DrawCreator::CopyParamProductCaster::CopyParamProductCaster(std::pair<std::string, std::string> copyParam) : param(copyParam) {}

void DrawCreator::CopyParamProductCaster::operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, const Koala::IO::ParSet &src2)
{
	if (param.first.compare("") != 0) fieldCopy(src1, dest, param.first);
	if (param.second.compare("") != 0) fieldCopy(src2, dest, param.second);
}

void DrawCreator::CopyParamProductCaster::operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, Koala::BlackHole b)
{
	if (param.first.compare("") != 0) fieldCopy(src1, dest, param.first);
}

void DrawCreator::CopyParamProductCaster::operator()(Koala::IO::ParSet& dest, Koala::BlackHole b, const Koala::IO::ParSet &src2)
{
	if (param.second.compare("") != 0) fieldCopy(src2, dest, param.second);
}

DrawCreator::CopyParamLineGraphCaster::CopyParamLineGraphCaster(std::string copyParam) : param(copyParam) {}

void DrawCreator::CopyParamLineGraphCaster::operator()(Koala::IO::ParSet& dest, Koala::IO::ParSet src)
{
	if (param.compare("") != 0) fieldCopy(src, dest, param);
}

DrawCreator::DrawProductCaster::DrawProductCaster(std::pair<std::string, std::string> copyParam) : CopyParamProductCaster(copyParam) {}

void DrawCreator::DrawProductCaster::operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, const Koala::IO::ParSet &src2)
{
	dest.set(DrawCreator::X_PARAM, src1.getDouble(DrawCreator::X_PARAM) + src2.getDouble(DrawCreator::X_PARAM));
	dest.set(DrawCreator::Y_PARAM, src1.getDouble(DrawCreator::Y_PARAM) + src2.getDouble(DrawCreator::Y_PARAM));
	this->CopyParamProductCaster::operator()(dest, src1, src2);
}

void DrawCreator::DrawProductCaster::operator()(Koala::IO::ParSet& dest, const Koala::IO::ParSet &src1, Koala::BlackHole b)
{
	this->CopyParamProductCaster::operator()(dest, src1, b);
}

void DrawCreator::DrawProductCaster::operator()(Koala::IO::ParSet& dest, Koala::BlackHole b, const Koala::IO::ParSet &src2)
{
	this->CopyParamProductCaster::operator()(dest, b, src2);
}

DrawCreator::DrawLineGraphCaster::DrawLineGraphCaster(std::string copyParam) : CopyParamLineGraphCaster(copyParam) {}

void DrawCreator::DrawLineGraphCaster::operator()(Koala::IO::ParSet& dest, Koala::IO::ParSet src)
{
	dest.set(DrawCreator::X_PARAM, src.getDouble(DrawCreator::X_PARAM));
	dest.set(DrawCreator::Y_PARAM, src.getDouble(DrawCreator::Y_PARAM));
	this->CopyParamLineGraphCaster::operator()(dest, src);
}



