

template< class GraphType, class  VInfoGen, class  EInfoGen>
 std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
Creator::addVertices(GraphType& g, int n, int num, EdgeDirection dir, VInfoGen vInfoGen, EInfoGen eInfoGen)
{
	int i = 0;
	typename GraphType::PVertex pVertexFirst = NULL, pVertexLast = NULL;
	EdgeDirection isLoop = dir & EdLoop;

	if (n > 0){
		pVertexFirst = g.addVert(vInfoGen(num));
		if (isLoop)
		{
			//adding loop
			g.addEdge(pVertexFirst, pVertexFirst, eInfoGen(num, num, EdLoop), EdLoop);
		}
	}

	for(i = 1; i < n; i++){
		pVertexLast = g.addVert( vInfoGen(++num) );
		if (isLoop)
		{
			//adding loop
			g.addEdge(pVertexLast, pVertexLast, eInfoGen(num, num, EdLoop), EdLoop);
		}

	}
	return std::make_pair(pVertexFirst,pVertexLast);

}


template< class GraphType, class  VInfoGen, class  EInfoGen>
std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
Creator::addVertices2Tab(GraphType& g, typename GraphType::PVertex *vTab, int n, int num, EdgeDirection dir, VInfoGen vInfoGen, EInfoGen eInfoGen)
{
	int i = 0;
	typename GraphType::PVertex pVertexFirst = NULL, pVertexLast = NULL;
	EdgeDirection isLoop = dir & EdLoop;

	if (n > 0){
		pVertexFirst = g.addVert(vInfoGen(num));
		if (isLoop)
		{
			//adding loop
			g.addEdge(pVertexFirst, pVertexFirst, eInfoGen(num, num, EdLoop), EdLoop);
		}
		vTab[0] = pVertexFirst;
	}

	for(i = 1; i < n; i++){
		pVertexLast = g.addVert( vInfoGen(++num) );
		if (isLoop)
		{
			//adding loop
			g.addEdge(pVertexLast, pVertexLast, eInfoGen(num, num, EdLoop), EdLoop);
		}
		vTab[i] = pVertexLast;
	}
	return std::make_pair(pVertexFirst, pVertexLast);
}


template< class GraphType, class  EInfoGen >
typename GraphType::PEdge
Creator::addEdges(GraphType& g, typename GraphType::PVertex v1, typename GraphType::PVertex v2, int num1, int num2, EdgeDirection dir, EInfoGen eInfoGen)
{

	typename GraphType::PEdge pEdge1 = NULL, pEdge2 = NULL, pEdge3 = NULL, pEdge = NULL;
	if (dir & EdUndir)
	{
		pEdge1 = g.addEdge(v1, v2, eInfoGen(num1, num2, EdUndir), EdUndir);
	}
	if (dir & EdDirIn)
	{
		pEdge2 = g.addEdge(v1, v2, eInfoGen(num1, num2, EdDirIn), EdDirIn);
	}
	if (dir & EdDirOut)
	{
		pEdge3 = g.addEdge(v1, v2, eInfoGen(num1, num2, EdDirOut), EdDirOut);
	}

	pEdge = pEdge1 ? pEdge1 : pEdge2;
	pEdge = pEdge ? pEdge : pEdge3;

	return pEdge;
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::clique(GraphType&  g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir )
{

	if (n<=0)
		return NULL;

	typename GraphType::PVertex LOCALARRAY( vTab, n );

	addVertices2Tab(g, vTab, n, 0, dir, vInfoGen, eInfoGen);

	//do not even try to generate edges
	if (dir == EdNone || dir == EdLoop)
		return vTab[0];

	for (int i = 0; i<n; i++)
	{
		for (int j = i+1; j<n; j++)
		{
			addEdges(g, vTab[i], vTab[j], i, j, dir, eInfoGen);
		}
	}

	return vTab[0];
}

template< class GraphType>
typename GraphType::PVertex Creator::clique(GraphType&  g,int n, EdgeDirection dir)
{
	return clique(g,n,Koala::ConstFunctor< typename GraphType::VertInfoType>(),Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::path(GraphType&  g,int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{

	typename GraphType::PVertex firstVertex = NULL, pathVertex = NULL, tempVertex = NULL;

	firstVertex = addVertices(g,1,0,dir,vInfoGen,eInfoGen).first;
	if (n < 2)
		return firstVertex;

	pathVertex = firstVertex;

	for(int i = 1; i < n; i++)
	{
		tempVertex = addVertices(g,1,i,dir,vInfoGen,eInfoGen).first;
		addEdges(g,pathVertex,tempVertex,i-1,i,dir,eInfoGen);
		pathVertex = tempVertex;
	}

	return firstVertex;
}

template< class GraphType>
typename GraphType::PVertex Creator::path(GraphType&  g, int n, EdgeDirection dir)
{
	return path(g, n, Koala::ConstFunctor< typename GraphType::VertInfoType>(),Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}

template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::cycle(GraphType&  g,int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{
	typename GraphType::PVertex firstVertex = NULL, pathVertex = NULL, tempVertex = NULL;

	firstVertex = addVertices(g,1,0,dir,vInfoGen,eInfoGen).first;
	if (n < 2)
		return firstVertex;

	pathVertex = firstVertex;

	for(int i = 1; i < n; i++)
	{
		tempVertex = addVertices(g,1,i,dir,vInfoGen,eInfoGen).first;
		addEdges(g,pathVertex,tempVertex,i-1,i,dir,eInfoGen);
		pathVertex = tempVertex;
	}

	addEdges(g,tempVertex, firstVertex,n-1, 0, dir,eInfoGen);
	return firstVertex;
}

template< class GraphType>
typename GraphType::PVertex Creator::cycle(GraphType& g, int n, EdgeDirection dir)
{
	return cycle(g, n, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::fan(GraphType&  g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection centerDir, EdgeDirection borderDir)
{
	typename GraphType::PVertex centrVertex = NULL, pathVertex = NULL, tempVertex = NULL;

	centrVertex = addVertices(g,1,0,centerDir,vInfoGen,eInfoGen).first;
	if (n < 2)
		return centrVertex;

	pathVertex = addVertices(g,1,1,borderDir,vInfoGen,eInfoGen).first;

	addEdges(g,centrVertex,pathVertex,0,1,centerDir,eInfoGen);

	for(int i = 2; i < n; i++)
	{
		tempVertex = addVertices(g,1,i,borderDir,vInfoGen,eInfoGen).first;
		addEdges(g,pathVertex,tempVertex,i-1,i,borderDir,eInfoGen);
		addEdges(g,centrVertex,tempVertex,0,i,centerDir,eInfoGen);

		pathVertex = tempVertex;
	}

	return centrVertex;
}

template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::fan(GraphType&  g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{
	return fan(g, n, vInfoGen, eInfoGen, dir, dir);
}

template< class GraphType>
typename GraphType::PVertex Creator::fan(GraphType&  g, int n, EdgeDirection dir)
{
	return fan(g, n, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}

template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::wheel(GraphType&  g, int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{
	return wheel(g, n, vInfoGen, eInfoGen, dir, dir);
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::wheel(GraphType&  g,int n, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection centerDir, EdgeDirection borderDir)
{
	typename GraphType::PVertex centrVertex = NULL, pathVertex = NULL, tempVertex = NULL, p1Vertex = NULL;

	centrVertex = addVertices(g,1,0,centerDir,vInfoGen,eInfoGen).first;
	if (n < 2)
		return centrVertex;

	pathVertex = addVertices(g,1,1,borderDir,vInfoGen,eInfoGen).first;
	p1Vertex = pathVertex;

	addEdges(g,centrVertex,pathVertex,0,1,centerDir,eInfoGen);

	if (n < 3)
		return centrVertex;

	for(int i = 2; i < n; i++)
	{
		tempVertex = addVertices(g,1,i,borderDir,vInfoGen,eInfoGen).first;
		addEdges(g,pathVertex,tempVertex,i-1,i,borderDir,eInfoGen);
		addEdges(g,centrVertex,tempVertex,0,i,centerDir,eInfoGen);

		pathVertex = tempVertex;
	}

	addEdges(g,tempVertex, p1Vertex,n-1,1,borderDir,eInfoGen);

	return centrVertex;
}

template< class GraphType>
typename GraphType::PVertex Creator::wheel(GraphType&  g, int n, EdgeDirection dir)
{
	return wheel(g, n, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}

template< class GraphType, class  VInfoGen, class  EInfoGen >
std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
Creator::compBipartite(GraphType& g, int n1, int n2, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{
	typename GraphType::PVertex p1Vertex = NULL, p2Vertex = NULL;
	typename GraphType::PVertex LOCALARRAY( vTab1, n1 );
	typename GraphType::PVertex LOCALARRAY( vTab2, n2 );

	addVertices2Tab(g,vTab1,n1,0,dir,vInfoGen,eInfoGen);
	addVertices2Tab(g,vTab2,n2,n1,dir,vInfoGen,eInfoGen);

	for (int i = 0; i < n1; i++)
	{
		for (int j = 0; j < n2; j++)
		{
			addEdges(g,vTab1[i],vTab2[j],i,n1+j,dir,eInfoGen);
		}
	}

	return std::make_pair(p1Vertex,p2Vertex) ;
}

template< class GraphType >
std::pair<typename GraphType::PVertex, typename GraphType::PVertex>
Creator::compBipartite(GraphType& g, int n1, int n2, EdgeDirection dir)
{
	return compBipartite(g, n1, n2, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}


template< class GraphType, class  VInfoGen, class  EInfoGen, class IterInsert >
typename GraphType::PVertex
Creator::compKPartite(GraphType& g, int K, int k, IterInsert it, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{

	int LOCALARRAY( card, K+1);

	for(int i = 0; i < K; i++) card[i] = k;
	card[K]= 0;

	return compKPartite(g, card, card+K, it, vInfoGen, eInfoGen, dir);
}

template< class GraphType, class IterInsert >
typename GraphType::PVertex
Creator::compKPartite(GraphType& g, int K, int k, IterInsert it, EdgeDirection dir)
{
	return compKPartite(g, K, k, it, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}


template< class GraphType, class  VInfoGen, class  EInfoGen, class Iter, class IterInsert >
typename GraphType::PVertex
Creator::compKPartite(GraphType& g, Iter begin, Iter end, IterInsert it, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{
	int size = 0;
	int k = 0, i = 0;

	int n=0;
	for( Iter iter = begin; iter != end; iter++ )
	{
		size++;
		k = *iter;
		n += k;
	}

	typename GraphType::PVertex LOCALARRAY( vTab,n );
	std::pair<typename GraphType::PVertex, typename GraphType::PVertex> LOCALARRAY( partTab,size );
	int LOCALARRAY( partSize,size );

	addVertices2Tab(g,vTab,n,0,dir,vInfoGen,eInfoGen);

	int j = 0;
	for( Iter iter = begin; iter != end; iter++)
	{
		//the cardinality of the paricular partition
		k = *iter;

		partTab[i] = std::make_pair(vTab[j],vTab[j+k-1]);
		partSize[i] = k;

		*it = partTab[i];
		it++;

		i++;
		j += k;
	}

	int nextPart = 0, startPart = 0, endPart = 0;

	for (i = 0; i < size-1; i++)
	{
		endPart = startPart+partSize[i];

		for (int v1 = startPart; v1 < endPart; v1++)
		{
			for(int v2 = endPart ; v2 < n; v2++)
			{
				addEdges(g, vTab[v1], vTab[v2], v1, v2, dir, eInfoGen);
			}
		}
		startPart += partSize[i];
	}

	return vTab[0];
}

template< class GraphType, class Iter, class IterInsert >
typename GraphType::PVertex
Creator::compKPartite(GraphType& g, Iter begin, Iter end, IterInsert it, EdgeDirection dir)
{
	return compKPartite(g, begin, end, it,  Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex Creator::petersenGraph(GraphType&  g, VInfoGen vInfoGen, EInfoGen eInfoGen)
{
	typename GraphType::PVertex LOCALARRAY( vert, 10);

	addVertices2Tab(g,vert,10,0,EdUndir,vInfoGen,eInfoGen);

	addEdges(g,vert[0],vert[1],0,1,EdUndir,eInfoGen);
	addEdges(g,vert[1],vert[2],1,2,EdUndir,eInfoGen);
	addEdges(g,vert[2],vert[3],2,3,EdUndir,eInfoGen);
	addEdges(g,vert[3],vert[4],3,4,EdUndir,eInfoGen);
	addEdges(g,vert[0],vert[4],0,4,EdUndir,eInfoGen);

	addEdges(g,vert[5],vert[6],5,6,EdUndir,eInfoGen);
	addEdges(g,vert[6],vert[7],6,7,EdUndir,eInfoGen);
	addEdges(g,vert[7],vert[8],7,8,EdUndir,eInfoGen);
	addEdges(g,vert[8],vert[9],8,9,EdUndir,eInfoGen);
	addEdges(g,vert[5],vert[9],5,9,EdUndir,eInfoGen);

	addEdges(g,vert[0],vert[8],0,8,EdUndir,eInfoGen);
	addEdges(g,vert[1],vert[6],1,6,EdUndir,eInfoGen);
	addEdges(g,vert[2],vert[9],2,9,EdUndir,eInfoGen);
	addEdges(g,vert[3],vert[7],3,7,EdUndir,eInfoGen);
	addEdges(g,vert[4],vert[5],4,5,EdUndir,eInfoGen);

	return vert[0];
}

template< class GraphType>
typename GraphType::PVertex Creator::petersenGraph(GraphType&  g)
{
	return petersenGraph(g, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>());
}

template< class GraphType, class  VInfoGen, class  EInfoGen, class Iter>
typename GraphType::PVertex
Creator::regTree(GraphType& g, Iter begin, Iter end, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{
	int lvl = 0, childSize = 0, d = 0, num = 1, n = 0;

	for( Iter iter = begin; iter != end; iter++ ) d++;

	int LOCALARRAY( vertNum, d+1);

	num = 1;
	lvl = 1;
	vertNum[0] = 1; // only the root is on level 0
	for( Iter iter = begin; iter != end; iter++)
	{
		childSize = *iter;
		num *= childSize;
		vertNum[lvl] = num;

		n+= num;
		lvl++;
	}
	typename GraphType::PVertex LOCALARRAY( vTab, n+1);

	addVertices2Tab(g,vTab,n+1,0,dir,vInfoGen,eInfoGen);

	lvl=0;
	int parentNum = 0, childNum = 1;
	for( Iter iter = begin; iter != end; iter++)
	{
		childSize = *iter;
		for(int i = 0; i < vertNum[lvl]; i++)
		{
			for (int j = 0; j < childSize; j++)
			{
				addEdges(g,vTab[parentNum],vTab[childNum],parentNum,childNum,dir,eInfoGen);
				childNum++;
			}
			parentNum++;
		}
		lvl++;
	}
	return vTab[0];
}


template< class GraphType, class Iter>
typename GraphType::PVertex
Creator::regTree(GraphType& g, Iter begin, Iter end, EdgeDirection dir)
{
	return regTree(g, begin, end, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}

//height = 0 - there is only the root
template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex
Creator::regTree(GraphType& g, int deg, int height, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{
	int LOCALARRAY( degrees, height+1);
	for(int i = 0; i < height; i++)
	{
		degrees[i]=deg;
	}
	return regTree(g, degrees, degrees+height, vInfoGen, eInfoGen, dir);

}

template< class GraphType>
typename GraphType::PVertex
Creator::regTree(GraphType& g, int deg, int height, EdgeDirection dir)
{
	return regTree(g, deg, height, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir);
}

template< class GraphType, class  VInfoGen, class  EInfoGen, class Iter>
typename GraphType::PVertex
Creator::caterpillar(GraphType& g, Iter begin, Iter end, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection pathDir, EdgeDirection legDir)
{
	int pathNum = 0, legNum = 0, counter = 0, i = 0;
	typename GraphType::PVertex pathVertex = NULL, tempVertex = NULL;

	for( Iter iter = begin; iter != end; iter++ ) pathNum++;
	if (pathNum < 1)
		return NULL;

	typename GraphType::PVertex LOCALARRAY( pathTab, pathNum + 1);

	pathTab[0] = addVertices(g, 1, 0, pathDir, vInfoGen, eInfoGen).first;
	pathVertex = pathTab[0];

	for(i = 1; i < pathNum; i++)
	{
		tempVertex = addVertices(g, 1, i, pathDir, vInfoGen, eInfoGen).first;
		pathTab[i] = tempVertex;
		addEdges(g, pathVertex, tempVertex, i-1, i, pathDir, eInfoGen);
		pathVertex = tempVertex;
	}

	counter = pathNum;
	i = 0;
	for( Iter iter = begin; iter != end; iter++ )
	{
		pathVertex = pathTab[i];
		legNum = *iter;
		for (int j = 0; j < legNum; j++)
		{
			tempVertex = addVertices(g, 1, counter, legDir, vInfoGen, eInfoGen).first;
			addEdges(g, pathVertex, tempVertex, i, counter, legDir, eInfoGen);
			counter++;
		}
		i++;
	}

	return pathTab[0];
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex
Creator::caterpillar(GraphType& g, int pathVertNum, int legNnm, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection pathDir, EdgeDirection legDir)
{
	int LOCALARRAY( legs, pathVertNum + 1);
	for(int i = 0; i < pathVertNum; i++)
	{
		legs[i] = legNnm;
	}
	return caterpillar(g, legs, legs + pathVertNum, vInfoGen, eInfoGen, pathDir, legDir);

}

template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex
Creator::caterpillar(GraphType& g, int pathVertNum, int legNnm, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeDirection dir)
{

	return caterpillar(g, pathVertNum, legNnm, vInfoGen, eInfoGen, dir, dir);
}

template< class GraphType>
typename GraphType::PVertex
Creator::caterpillar(GraphType& g, int pathVertNum, int legNnm, EdgeDirection dir)
{
	return caterpillar(g, pathVertNum, legNnm, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), dir, dir);
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex
Creator::erdRen1(GraphType& g, int n, double p, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType eType){

	return Creator::erdRen1 (g, n, p, vInfoGen, eInfoGen, (unsigned int)time(NULL), eType);
}

template< class GraphType >
typename GraphType::PVertex
Creator::erdRen1(GraphType& g, int n, double p, EdgeType eType){

	return Creator::erdRen1 (g, n, p,  Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), (unsigned int)time(NULL), eType);
}

template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex
Creator::erdRen1(GraphType& g, int n, double p, VInfoGen vInfoGen, EInfoGen eInfoGen, unsigned int randSeed, EdgeType eType)
{   if (eType == EdDirOut) eType =Directed;
    koalaAssert(eType == Undirected||eType == Directed,AlgExcWrongMask);
	double rnd;
	srand (randSeed);

	typename GraphType::PVertex LOCALARRAY( vTab, n);
	for (int i = 0; i < n; i++)
		vTab[i] = g.addVert(vInfoGen(i));

	for (int i = 0; i < n; i++)
	{
		for(int j = i+1; j < n; j++)
		{
			rnd = (double)rand()/(double)RAND_MAX;
			if (rnd <= p)
			{
				if(eType == Undirected)
					g.addEdge(vTab[i],vTab[j],eInfoGen(i, j, EdUndir),EdUndir);
				if(eType == Directed)
					g.addEdge(vTab[i],vTab[j],eInfoGen(i, j, EdDirIn),EdDirIn);
			}
			if(eType == Directed)
			{
				rnd = (double)rand()/(double)RAND_MAX;
				if (rnd <= p)
					g.addEdge(vTab[j],vTab[i],eInfoGen(j, i, EdDirIn),EdDirIn);
			}
		}
	}
	return vTab[0];
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex
Creator::erdRen2(GraphType& g,int n, int m, VInfoGen vInfoGen, EInfoGen eInfoGen, EdgeType eType){

	return Creator::erdRen2(g, n, m, vInfoGen, eInfoGen, (unsigned int)time(NULL), eType);
}

template< class GraphType>
typename GraphType::PVertex
Creator::erdRen2(GraphType& g,int n, int m, EdgeType eType){

	return Creator::erdRen2(g, n, m, Koala::ConstFunctor< typename GraphType::VertInfoType>(), Koala::ConstFunctor<typename GraphType::EdgeInfoType>(), (unsigned int)time(NULL), eType);
}


template< class GraphType, class  VInfoGen, class  EInfoGen>
typename GraphType::PVertex
Creator::erdRen2(GraphType& g, int n, int m, VInfoGen vInfoGen, EInfoGen eInfoGen, unsigned int randSeed, EdgeType eType)
{   if (eType == EdDirOut) eType =Directed;

	srand (randSeed);
	int maxEgdesNum = 0, j=0, k = 0,  v1num=0, v2num=0;
	std::pair<int,int> temp;

	typename GraphType::PVertex LOCALARRAY(vTab,n);
	for (int i = 0; i < n; i++)
		vTab[i] = g.addVert(vInfoGen(i));

	if(eType == Undirected)
		maxEgdesNum = (n*(n-1))/2;
	else{
		koalaAssert(eType == Directed,AlgExcWrongMask);
		maxEgdesNum = n*(n-1);
	}
	koalaAssert(m <= maxEgdesNum,AlgExcWrongArg);

	std::pair<int,int> LOCALARRAY( edges,maxEgdesNum);

	if(eType == Undirected)
	{
		for (int i = 0; i < n; i++)
		{
			for (j = i+1; j < n; j++)
			{
				edges[k].first=i;
				edges[k].second=j;
				k++;
			}
		}
	}
	else
	{
		//edges are directed
		for (int i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				if (i == j) continue;
				edges[k].first = i;
				edges[k].second = j;
				k++;
			}
		}
	}

	//choose m edges at random from uniform distriburion
	for (int i = 0; i < m; i++)
	{
		j = random(i,maxEgdesNum-1);
		temp = edges[i];
		edges[i] = edges[j];
		edges[j] = temp;
	}

	for(int i = 0; i < m; i++)
	{
		v1num = edges[i].first;
		v2num = edges[i].second;
		if(eType == Undirected)
			g.addEdge(vTab[v1num],vTab[v2num],eInfoGen(v1num, v2num, EdUndir),EdUndir);
		else
			g.addEdge(vTab[v1num],vTab[v2num],eInfoGen(v1num, v2num, EdDirIn),EdDirIn);
	}
	return vTab[0];
}


 int Creator::random(int begin, int end)
{
	int range = (end-begin)+1;
	return begin + int(range*rand()/(RAND_MAX + 1.0));

}
