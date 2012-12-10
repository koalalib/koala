// ConstGraphMethods

template< class GraphType > typename ConstGraphMethods< GraphType >::PVertex
	ConstGraphMethods< GraphType >::getEdgeEnd( PEdge edge, PVertex vert ) const
{
	koalaAssert( edge,GraphExcNullEdge );
	return edge->getEnd( vert );
}

template< class GraphType > typename ConstGraphMethods< GraphType >::PVertex
	ConstGraphMethods< GraphType >::getEnd( PEdge edge, PVertex vert ) const
{
	koalaAssert( edge,GraphExcNullEdge );
	return edge->getEnd( vert );
}

template< class GraphType > inline bool ConstGraphMethods< GraphType >::incid( PEdge edge1, PEdge edge2 ) const
{
	if (!edge2) return false;
	return (isEdgeEnd( edge1,self.getEdgeEnd1( edge2 ) ) || isEdgeEnd( edge1,self.getEdgeEnd2( edge2 ) ) );
}

template< class GraphType > typename ConstGraphMethods< GraphType >::VertInfoType
	ConstGraphMethods< GraphType >::getVertInfo( PVertex v ) const
{
	koalaAssert( v,GraphExcNullVert );
	return v->getInfo();
}

template< class GraphType > typename ConstGraphMethods< GraphType >::EdgeInfoType
	ConstGraphMethods< GraphType >::getEdgeInfo( PEdge e ) const
{
	koalaAssert( e,GraphExcNullEdge );
	return e->getInfo();
}

template< class GraphType > template< class OutputIterator >
	int ConstGraphMethods< GraphType >::getVerts( OutputIterator out) const
{
	int ans = 0;
	PVertex vert = this->getVert();
	while (vert)
	{
		*out = vert;
		++out;
		++ans;
		vert = self.getVertNext( vert );
	}
	return ans;
}

template< class GraphType >
	Set< typename ConstGraphMethods< GraphType >::PVertex > ConstGraphMethods< GraphType >::getVertSet() const
{
	Set< typename GraphType::PVertex > res;
	getVerts( setInserter( res ) );
	return res;
}

template< class GraphType > template< class OutputIterator, class VChooser2 >
	int ConstGraphMethods< GraphType >::getVerts( OutputIterator out, VChooser2 ch ) const
{
	int licz=0;
	for( typename GraphType::PVertex v = this->getVert(); v; v = self.getVertNext( v ) )
		if (ch( v,*this ))
		{
			*out = v;
			++out;
			++licz;
		}
	return licz;
}

template< class GraphType > template< class VChooser2 > Set< typename ConstGraphMethods< GraphType >::PVertex >
	ConstGraphMethods< GraphType >::getVertSet( VChooser2 ch ) const
{
	Set< PVertex > s;
	getVerts( setInserter( s ),ch );
	return s;
}

template< class GraphType >
	typename ConstGraphMethods< GraphType >::PVertex ConstGraphMethods< GraphType >::vertByNo( int idx ) const
{
	PVertex tmp_vert = this->getVert();
	while (idx && tmp_vert)
	{
		tmp_vert = self.getVertNext( tmp_vert );
		--idx;
	}
	return tmp_vert;
}

template< class GraphType >
	int ConstGraphMethods< GraphType >::vertPos( typename ConstGraphMethods< GraphType >::PVertex vert ) const
{
	int idx = 0;
	koalaAssert( vert,GraphExcNullVert );
	PVertex tmp_vert = this->getVert();
	while (tmp_vert && tmp_vert != vert)
	{
		tmp_vert = self.getVertNext( tmp_vert );
		++idx;
	}
	return tmp_vert ? idx : -1;
}

template< class GraphType >
	bool ConstGraphMethods< GraphType >::has( typename ConstGraphMethods< GraphType >::PVertex vert ) const
{
	for( PVertex tmp_vert = this->getVert(); tmp_vert; tmp_vert = this->getVertNext( tmp_vert) )
		if (tmp_vert == vert) return true;
	return false;
}

template< class GraphType >
	bool ConstGraphMethods< GraphType >::has( typename ConstGraphMethods< GraphType >::PEdge edge ) const
{
	for( PEdge tmp_edge = this->getEdge(); tmp_edge; tmp_edge = this->getEdgeNext( tmp_edge) )
		if (tmp_edge == edge) return true;
	return false;
}

template< class GraphType > template< class OutputIterator >
	int ConstGraphMethods< GraphType >::getEdges( OutputIterator out, EdgeDirection direct ) const
{
	int ans = 0;
	PEdge edge = this->getEdge();
	while (edge) {
		if (self.getEdgeType( edge ) & direct)
		{
			*out = edge;
			++out;
			++ans;
		}
		edge = self.getEdgeNext( edge );
	}
	return ans;
}

template< class GraphType > template< class OutputIterator >
int ConstGraphMethods< GraphType >::getEdges( OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
	int ans = 0;
	PEdge edge = this->getEdge( vert,direct );
	while (edge) {
		*out = edge;
		++out;
		++ans;
		edge = self.getEdgeNext( vert,edge,direct );
	}
	return ans;
}

template< class GraphType > template< class OutputIterator > int
	ConstGraphMethods< GraphType >::getEdges( OutputIterator out, PVertex vert1, PVertex vert2, EdgeDirection direct ) const
{
	int ans = 0;
	PEdge edge = this->getEdge( vert1,vert2,direct );
	while (edge) {
		*out = edge;
		++out;
		++ans;
		edge = self.getEdgeNext( vert1,vert2,edge,direct );
	}
	return ans;
}

template< class GraphType > template< class OutputIterator,class EChooser2 >
	int ConstGraphMethods< GraphType >::getEdges( OutputIterator out, EChooser2 ch ) const
{
	int licz = 0;
	for( PEdge e = this->getEdge(); e; e = self.getEdgeNext( e ) )
		if (ch( e,*this ))
		{
			*out = e;
			++out;
			++licz;
		}
	return licz;
}

template< class GraphType> template< class EChooser2 >
	Set< typename ConstGraphMethods< GraphType >::PEdge > ConstGraphMethods< GraphType >::getEdgeSet( EChooser2 ch ) const
{
	Set< PEdge > s;
	this->getEdges( setInserter( s ),ch );
	return s;
}
template< class GraphType > Set< typename ConstGraphMethods< GraphType>::PEdge >
	ConstGraphMethods< GraphType >::getEdgeSet( EdgeType mask ) const
{
	Set< PEdge > s;
	this->getEdges( setInserter( s ),mask );
	return s;
}

template< class GraphType > Set< typename ConstGraphMethods< GraphType >::PEdge >
	ConstGraphMethods< GraphType >::getEdgeSet( PVertex vert, EdgeDirection mask ) const
{
	Set< PEdge > res;
	getEdges( setInserter( res ),vert,mask );
	return res;
}

template< class GraphType > Set< typename ConstGraphMethods< GraphType >::PEdge >
	ConstGraphMethods< GraphType >::getEdgeSet( PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{
	Set< PEdge > res;
	getEdges( setInserter( res ),vert1,vert2,mask );
	return res;
}

template< class GraphType > typename ConstGraphMethods< GraphType >::PEdge
	ConstGraphMethods< GraphType >::edgeByNo( int idx ) const
{
	PEdge tmp_edge = this->getEdge();
	while (idx && tmp_edge)
	{
		tmp_edge = self.getEdgeNext( tmp_edge );
		--idx;
	}
	return tmp_edge;
}


template< class GraphType >
	int ConstGraphMethods< GraphType >::edgePos( typename ConstGraphMethods< GraphType >::PEdge edge ) const
{
	int idx = 0;
	koalaAssert( edge,GraphExcNullEdge );
	PEdge tmp_edge = this->getEdge();
	while (tmp_edge && tmp_edge != edge)
	{
		tmp_edge = self.getEdgeNext( tmp_edge );
		++idx;
	}
	return tmp_edge ? idx : -1;
}

template< class GraphType> template< class OutputV, class OutputE, class VChooser2, class EChooser2 > std::pair< int,int >
	ConstGraphMethods< GraphType >::getChosen( std::pair< OutputV,OutputE > out,
		std::pair< VChooser2,EChooser2 > chs, bool chosenends ) const
{
	int vlicz = this->getVerts( out.first,chs.first ), elicz;
	if (!chosenends) elicz = this->getEdges( out.second,chs.second );
	else elicz = this->getEdges( out.second,(edgeFirstEndChoose( chs.first ) && edgeSecondEndChoose( chs.first )) &&
		chs.second );
	return std::make_pair( vlicz,elicz );
}

template< class GraphType> template< class VChooser2, class EChooser2 >
	std::pair< Set< typename ConstGraphMethods< GraphType >::PVertex >,
		Set< typename ConstGraphMethods< GraphType >::PEdge > >
	ConstGraphMethods< GraphType>::getChosenSets( std::pair< VChooser2,EChooser2 > chs, bool chosenends ) const
{
	Set< PVertex > sv;
	Set< PEdge > se;
	this->getChosen( std::make_pair( setInserter( sv ),setInserter( se ) ),chs,chosenends );
	return std::make_pair( sv,se );
}

template< class GraphType > template< class Cont >
void ConstGraphMethods< GraphType >::getAdj( Cont &cont, EdgeType mask ) const
{
	if (GraphSettings::ReserveOutAssocCont) cont.reserve( self.getVertNo() );
	std::pair< PVertex,PVertex > ends;
	for( PEdge e = this->getEdge( mask ); e; e = self.getEdgeNext( e,mask ) )
	{
		ends = self.getEdgeEnds( e );
		cont( ends.first,ends.second ) = true;
		if (self.getEdgeType( e ) == Undirected) cont( ends.second,ends.first ) = true;
	}
}

template< class GraphType > std::pair< typename ConstGraphMethods< GraphType >::PVertex,int >
	ConstGraphMethods< GraphType >::maxDeg( EdgeDirection direct ) const
{
	std::pair< PVertex,int > res( (PVertex)0,-1 );
	if (!self.getVertNo()) return res;
	else
	{
		res.first = this->getVert();
		res.second = this->deg( res.first,direct );
	}
	int degree;
	PVertex vert = self.getVertNext( res.first );
	for( ; vert; vert = self.getVertNext( vert ) )
		if ((degree = this->deg( vert,direct )) > res.second)
			res = std::make_pair( vert,degree );
	return res;
}

template< class GraphType > std::pair< typename ConstGraphMethods< GraphType >::PVertex,int >
	ConstGraphMethods< GraphType >::minDeg( EdgeDirection direct ) const
{
	std::pair< PVertex,int > res( (PVertex)0,-1 );
	if (!self.getVertNo()) return res;
	else
	{
		res.first = this->getVert();
		res.second = this->deg( res.first,direct );
	}
	int degree;
	PVertex vert = self.getVertNext( res.first );
	for( ; vert; vert = self.getVertNext( vert ) )
		if ((degree = this->deg( vert,direct )) < res.second)
			res = std::make_pair( vert,degree );
	return res;
}

template< class GraphType > template< class OutputIterator >
	int ConstGraphMethods< GraphType>::getNeighs( OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
	koalaAssert( vert,GraphExcNullVert );
	PVertex LOCALARRAY( ans,self.root().getEdgeNo( vert,direct ) );
	int size = 0, res = 0;
	PEdge edge = this->getEdge( vert,direct );
	while (edge)
	{
		ans[size++] = this->getEdgeEnd( edge,vert );
		edge = self.getEdgeNext( vert,edge,direct );
	}
	GraphSettings::sort( ans,ans + size );
	for( int i = 0; i < size; i++ )
		if (i == 0 || ans[i - 1] != ans[i])
		{
			*out = ans[i];
			++out;
			++res;
		}
	return res;
}

template< class GraphType > Set< typename ConstGraphMethods< GraphType >::PVertex >
	ConstGraphMethods< GraphType>::getNeighSet( PVertex vert, EdgeDirection mask ) const
{
	Set< PVertex > res;
	this->getNeighs( setInserter( res ),vert,mask );
	return res;
}

template< class GraphType > Set< typename ConstGraphMethods< GraphType >::PVertex >
	ConstGraphMethods< GraphType >::getClNeighSet( PVertex vert, EdgeDirection direct ) const
{
	Set< PVertex > ans = this->getNeighSet( vert,direct );
	ans.add( vert );
	return ans;
}

template< class GraphType > template< class OutputIterator >
	int ConstGraphMethods< GraphType>::getClNeighs( OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
	koalaAssert( vert,GraphExcNullVert );
	PVertex LOCALARRAY( ans,self.root().getEdgeNo( vert,direct ) + 1 );
	ans[0] = vert;
	int size = 1, res = 0;
	PEdge edge = this->getEdge( vert,direct );
	while (edge)
	{
		ans[size++] = this->getEdgeEnd( edge,vert );
		edge = self.getEdgeNext( vert,edge,direct );
	}
	GraphSettings::sort( ans,ans + size );
	for( int i = 0; i < size; i++ )
		if (i == 0 || ans[i - 1] != ans[i])
		{
			*out = ans[i];
			++out;
			++res;
		}
	return res;
}

template< class GraphType> bool ConstGraphMethods< GraphType>::areParallel(
	typename ConstGraphMethods< GraphType >::PEdge e1,
	typename ConstGraphMethods< GraphType >::PEdge e2, EdgeDirection reltype ) const
{
	//TODO: uproscic kod - switch po reltype
	koalaAssert( e1 && e2,GraphExcNullEdge );
	koalaAssert( reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir,GraphExcWrongMask );
	std::pair< PVertex,PVertex > ends1 = self.getEdgeEnds( e1 ),ends2 = self.getEdgeEnds( e2 );
	if (e1 == e2) return true;
	else if (self.getEdgeType( e1 ) == Loop) return self.getEdgeType( e2 ) == Loop && ends1.first == ends2.first;
	else if (self.getEdgeType( e2 ) == Loop) return false;
	else if ((self.getEdgeType( e1 ) == Undirected && self.getEdgeType( e2 ) == Undirected)
		|| (self.getEdgeType( e1 ) != self.getEdgeType( e2 ) && reltype == EdUndir)
		|| (self.getEdgeType( e1 ) == Directed && self.getEdgeType( e2 ) == Directed
			&& (reltype == EdUndir || reltype == EdDirIn)))
		return (ends1.first == ends2.first && ends1.second == ends2.second)
				|| (ends1.second == ends2.first && ends1.first == ends2.second);
	else return self.getEdgeType( e1 ) == self.getEdgeType( e2 ) && ends1.first == ends2.first &&
		ends1.second == ends2.second;
}

template< class GraphType > template< class OutputIterator >
int ConstGraphMethods< GraphType >::getParals( OutputIterator iter, PEdge edge, EdgeDirection reltype ) const
// TODO: nieefektywne, mozna bezposrednio
{
	koalaAssert( edge,GraphExcNullEdge );
	koalaAssert(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir,GraphExcWrongMask );
	int licz = 0;
	std::pair< PVertex,PVertex > ends = self.getEdgeEnds( edge );
	for( PEdge e = this->getEdge( ends.first,ends.second,EdAll ); e; e = self.getEdgeNext( ends.first,ends.second,e,EdAll ) )
		if (e != edge && this->areParallel( e,edge,reltype ))
		{
			*iter = e;
			++iter;
			licz++;
		}
	return licz;
}

template< class GraphType > Set< typename ConstGraphMethods< GraphType >::PEdge >
	ConstGraphMethods< GraphType >::getParalSet( typename ConstGraphMethods< GraphType >::PEdge edge,
		EdgeDirection reltype) const
{
	Set< PEdge > res;
	this->getParals( setInserter( res ),edge,reltype );
	return res;
}

template< class GraphType > std::pair< typename ConstGraphMethods< GraphType >::PEdge,int >
	ConstGraphMethods< GraphType >::maxMu( EdgeDirection reltype ) const
{
	koalaAssert( (reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir),GraphExcWrongMask );
	std::pair< PEdge,int > res( (PEdge)0,0 );
	int m = self.getEdgeNo( EdAll );
	if (!m) return res;
	Parals3 LOCALARRAY( edges,self.root().getEdgeNo( EdAll ) );
	int i = 0, l = 0, nr = 0;
	PEdge edge;
	for( PEdge e = this->getEdge( EdAll ); e; e = self.getEdgeNext( e,EdAll ) )
	{
		std::pair< PVertex,PVertex > ends = pairMinMax( self.getEdgeEnd1( e ), self.getEdgeEnd2( e ) );
		edges[i++] = Parals3( ends.first,ends.second,self.getEdgeDir( e,ends.first ),nr++,e );
	}
	GraphSettings::sort( edges,edges + i,Parals3cmp() );
	for( i = 0; i < m; i++ )
	{
		if (i ==0 || !this->areParallel( edges[i-1].edge,edges[i].edge,reltype ))
		{
			l = 1;
			edge = edges[i].edge;
		}
		else l++;
		if (l > res.second) res = std::make_pair( edge,l );
	}
	return res;
}

template< class GraphType > template< class Iterator, class OutIter > int
	ConstGraphMethods< GraphType >::getIncEdges( OutIter out, Iterator beg, Iterator end,
		EdgeDirection type, EdgeType kind ) const
{
	int licze = 0;
	typename GraphSettings:: template VertAssocCont< PVertex,bool >::Type vset( self.getVertNo() );
	for( Iterator i = beg; i != end; ++i ) vset[*i] = true;
	for( PVertex v = vset.firstKey(); v; v = vset.nextKey( v ) )
		for( PEdge e = this->getEdge( v,type ); e; e = self.getEdgeNext( v,e,type ) )
			if (((kind & Loop) && (((self.getEdgeDir( e,this->getEdgeEnd( e,v )) & type) == 0) ||
				this->getEdgeEnd( e,v ) >= v) && vset.hasKey( this->getEdgeEnd( e,v ))) ||
				((kind & (Directed | Undirected)) && !vset.hasKey( this->getEdgeEnd( e,v ) )))
			{
				*out = e;
				++out;
				++licze;
			}
	return licze;
}

template< class GraphType > template< class Iterator > Set< typename ConstGraphMethods< GraphType >::PEdge >
	ConstGraphMethods< GraphType >::getIncEdgeSet( Iterator beg, Iterator end, EdgeDirection type, EdgeType kind ) const
{
	Set< PEdge > res;
	this->getIncEdges( setInserter( res ),beg,end,type,kind );
	return res;
}

template< class GraphType > Set< typename ConstGraphMethods< GraphType >::PEdge >
	ConstGraphMethods< GraphType >::getIncEdgeSet( const Set< typename ConstGraphMethods< GraphType >::PVertex> &vset,
		EdgeDirection type, EdgeType kind ) const
{
	Set< PEdge > res;
	getIncEdges( setInserter( res ),vset.begin(),vset.end(),type,kind );
	return res;
}

template< class GraphType > template< class Iterator, class OutIter >
	int ConstGraphMethods< GraphType >::getIncVerts( OutIter out, Iterator beg, Iterator end, EdgeDirection type,
		EdgeType kind) const
{
	int liczv = 0;
	typename GraphSettings:: template VertAssocCont< PVertex,bool >::Type vset( self.getVertNo() );
	for( Iterator i = beg; i != end; ++i ) vset[*i] = true;
	PVertex LOCALARRAY( tabv,2 * self.getEdgeNo( type ) );
	for( PVertex v = vset.firstKey(); v; v = vset.nextKey( v ) )
		for( PEdge e = this->getEdge( v,type ); e; e = self.getEdgeNext( v,e,type ) )
			if (((kind & Loop) && vset.hasKey( this->getEdgeEnd( e,v ) ) ) ||
				((kind & (Directed | Undirected)) && !vset.hasKey( this->getEdgeEnd( e,v ) ) ))
				tabv[liczv++] = this->getEdgeEnd( e,v );
	GraphSettings::sort( tabv,tabv + liczv );
	liczv = std::unique( tabv,tabv + liczv ) - tabv;
	for( int i=0; i < liczv; i++ )
	{
		*out = tabv[i];
		++out;
	}
	return liczv;
}

template< class GraphType > template< class Iterator > Set< typename ConstGraphMethods< GraphType >::PVertex >
	ConstGraphMethods< GraphType >::getIncVertSet( Iterator beg, Iterator end, EdgeDirection type, EdgeType kind ) const
{
	Set< PVertex > res;
	this->getIncVerts( setInserter( res ),beg,end,type,kind );
	return res;
}

template< class GraphType > Set< typename ConstGraphMethods< GraphType >::PVertex >
	ConstGraphMethods< GraphType >::getIncVertSet( const Set< typename ConstGraphMethods< GraphType >::PVertex> &vset,
		EdgeDirection type, EdgeType kind) const
{
	Set< PVertex > res;
	this->getIncVerts( setInserter( res ),vset.begin(),vset.end(),type,kind );
	return res;
}

template< class GraphType > template< class IterOut1, class IterOut2, class Iterator >
	std::pair< int,int > ConstGraphMethods< GraphType>::findParals2( std::pair< IterOut1,IterOut2 > out,
		Iterator begin, Iterator end, EdgeType relType ) const
{
	int size = 0;
	for( Iterator iter = begin; iter != end; iter++ ) size++;
	PEdge LOCALARRAY( buf,size );
	size = 0;
	for( Iterator iter = begin; iter != end; iter++ ) if (*iter) buf[size++] = *iter;
	GraphSettings::sort( buf,buf + size );
	size = std::unique( buf,buf + size ) - buf;
	return this->findParals(out, buf,buf + size,relType );
}

template< class GraphType > template< class IterOut1, class IterOut2, class Iterator >
	std::pair< int,int > ConstGraphMethods< GraphType>::findParals(
		std::pair< IterOut1,IterOut2 > out, Iterator begin, Iterator end, EdgeType reltype ) const
{
	std::pair< int,int > res( 0,0 );
	koalaAssert( (reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir),GraphExcWrongMask );
	int size = 0;
	for( Iterator iter = begin; iter != end; iter++ ) size++;
	if (!size) return res;
	Parals3 LOCALARRAY( edges,size );
	int i = 0, nr = 0;
	for( Iterator iter = begin; iter != end; iter++ )
	{
		PEdge e = *iter;
		std::pair< typename GraphType::PVertex,typename GraphType::PVertex > vv = pairMinMax( getEdgeEnd1( e ),getEdgeEnd2( e ) );
		edges[i++] = Parals3( vv.first,vv.second,self.getEdgeDir( e,vv.first ),nr++,e );
	}
	GraphSettings::sort( edges,edges + size,Parals3cmp() );
	for( i = 0; i < size; i++ )
	{
		if (i == 0 || !this->areParallel( edges[i-1].edge,edges[i].edge,reltype ))
		{
			*out.first = edges[i].edge;
			++out.first;
			res.first++;
		}
		else
		{
			*out.second = edges[i].edge;
			++out.second;
			res.second++;
		}
	}
	return res;
}


template< class GraphType > template< class IterOut1, class IterOut2 > std::pair< int,int >
	ConstGraphMethods< GraphType >::findParals( std::pair< IterOut1,IterOut2 > out, PVertex vert, EdgeType relType ) const
{
	PEdge LOCALARRAY( buf,self.getEdgeNo( vert,EdAll ) );
	int size = this->getEdges( buf,vert,EdAll );
	return this->findParals( out,buf,buf + size,relType );
}

template< class GraphType > template< class IterOut1, class IterOut2 >
	std::pair<int,int> ConstGraphMethods< GraphType >::findParals(
		std::pair< IterOut1,IterOut2 > out, PVertex vert1, PVertex vert2, EdgeType relType ) const
// TODO: nieefektywne, mozna bezposrednio
{
	PEdge LOCALARRAY( buf,std::min( self.getEdgeNo( vert1,EdAll ),self.getEdgeNo( vert2,EdAll ) ) );
	int size = this->getEdges( buf,vert1,vert2,EdAll );
	return this->findParals( out,buf,buf + size,relType );
}

template< class GraphType > template< class IterOut1, class IterOut2 > std::pair< int,int >
	ConstGraphMethods< GraphType >::findParals( std::pair< IterOut1,IterOut2 > out, EdgeType relType ) const
{
	PEdge LOCALARRAY( buf,self.getEdgeNo( EdAll ) );
	this->getEdges( buf,EdAll );
	return this->findParals( out, buf,buf + getEdgeNo( EdAll ),relType );
}
