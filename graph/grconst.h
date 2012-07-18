#ifndef KOALA_GRAPHBASE_H
#define KOALA_GRAPHBASE_H

template <class GraphType> class ConstGraphMethods;

template < class GraphType >
struct GraphInternalTypes<ConstGraphMethods<GraphType> > {
        typedef typename GraphInternalTypes<GraphType>::Vertex Vertex;
        typedef typename GraphInternalTypes<GraphType>::PVertex PVertex;
        typedef typename GraphInternalTypes<GraphType>::Edge Edge;
        typedef typename GraphInternalTypes<GraphType>::PEdge PEdge;
        typedef typename GraphInternalTypes<GraphType>::VertInfoType VertInfoType;
        typedef typename GraphInternalTypes<GraphType>::EdgeInfoType EdgeInfoType;
        typedef typename GraphInternalTypes<GraphType>::GraphSettings GraphSettings;
};



template <class GraphType>
class ConstGraphMethods {
    public:

        typedef typename GraphInternalTypes< ConstGraphMethods<GraphType>  >::Vertex Vertex;
        typedef typename GraphInternalTypes< ConstGraphMethods<GraphType>  >::PVertex PVertex;
        typedef typename GraphInternalTypes< ConstGraphMethods<GraphType>  >::Edge Edge;
        typedef typename GraphInternalTypes< ConstGraphMethods<GraphType>  >::PEdge PEdge;
        typedef typename GraphInternalTypes< ConstGraphMethods<GraphType>  >::VertInfoType VertInfoType;
        typedef typename GraphInternalTypes< ConstGraphMethods<GraphType>  >::EdgeInfoType EdgeInfoType;
        typedef typename GraphInternalTypes< ConstGraphMethods<GraphType>  >::GraphSettings GraphSettings;

    protected:
        const GraphType& self;

        struct Parals3 {
            PVertex v1,v2;
            EdgeDirection direct;
            PEdge edge;

            Parals3(PVertex av1,PVertex av2,EdgeDirection adirect,PEdge aedge) :
                    v1(av1), v2(av2), direct(adirect), edge(aedge) {}
            Parals3() {}
        };

        struct Parals3cmp {
            bool operator()(Parals3 a, Parals3 b)
            {
                return (a.v1 < b.v1) ||
                (a.v1 == b.v1 && a.v2 < b.v2) ||
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct < b.direct ) ||
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct == b.direct && a.edge<b.edge);
            }

        };

    public:

    ConstGraphMethods() : self((const GraphType&)*this) {}
    ConstGraphMethods(const ConstGraphMethods&) : self((const GraphType&)*this) {}
    ConstGraphMethods& operator=(const ConstGraphMethods&) { return *this;}

        inline int getVertNo() const { return self.getVertNo(); }
        inline PVertex getVertNext( PVertex v) const { return self.getVertNext(v); }
        inline PVertex getVertPrev( PVertex v) const { return self.getVertPrev(v); }

        inline int getEdgeNo( EdgeDirection mask = EdAll ) const { return self.getEdgeNo(mask); }
        inline PEdge getEdgeNext( PEdge e, EdgeDirection mask = EdAll ) const { return self.getEdgeNext(e,mask); }
        inline PEdge getEdgePrev( PEdge e, EdgeDirection mask = EdAll ) const { return self.getEdgePrev(e,mask); }

        inline int getEdgeNo( PVertex v, EdgeDirection mask = EdAll) const { return self.getEdgeNo(v,mask); }
        inline PEdge getEdgeNext( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const { return self.getEdgeNext(v,e,mask); }
        inline PEdge getEdgePrev( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const { return self.getEdgePrev(v,e,mask); }

        inline int getEdgeNo( PVertex u, PVertex v, EdgeDirection mask = EdAll ) const { return self.getEdgeNo(u,v,mask); }
        inline PEdge getEdgeNext( PVertex u, PVertex v, PEdge e, EdgeDirection mask = EdAll ) const { return self.getEdgeNext(u,v,e,mask); }
        inline PEdge getEdgePrev( PVertex u, PVertex v, PEdge e, EdgeDirection mask = EdAll ) const { return self.getEdgePrev(u,v,e,mask); }

        inline EdgeType getEdgeType( PEdge e ) const { return self.getEdgeType(e); }
        inline std::pair< PVertex,PVertex >
            getEdgeEnds( PEdge edge ) const { return self.getEdgeEnds(edge); }
        inline PVertex getEdgeEnd1( PEdge edge ) const { return self.getEdgeEnd1(edge); }
        inline PVertex getEdgeEnd2( PEdge edge ) const { return self.getEdgeEnd2(edge); }
        inline EdgeDirection getEdgeDir( PEdge edge, PVertex v) const { return self.getEdgeDir(edge,v); }


    PVertex getVert() const { return self.getVertNext( (PVertex)0 ); }
    PVertex getVertLast() const { return self.getVertPrev( (PVertex)0 ); }
    PEdge getEdge( EdgeDirection mask = EdAll ) const { return self.getEdgeNext( (PEdge)0,mask ); }
    PEdge getEdgeLast( EdgeDirection mask = EdAll ) const { return self.getEdgePrev( (PEdge)0,mask ); }
    PEdge getEdge( PVertex vert, EdgeDirection mask= EdAll ) const { return self.getEdgeNext( vert,(PEdge)0,mask );}
    PEdge getEdgeLast( PVertex vert, EdgeDirection mask = EdAll ) const { return self.getEdgePrev( vert,(PEdge)0,mask );}
    PEdge getEdge( PVertex vert1, PVertex vert2, EdgeDirection mask = EdAll ) const
    { return self.getEdgeNext( vert1,vert2,(PEdge)0,mask ); }
    PEdge getEdgeLast( PVertex vert1, PVertex vert2, EdgeDirection mask = EdAll ) const
    { return self.getEdgePrev( vert1,vert2,(PEdge)0,mask ); }


    template< class OutputIterator > int getVerts( OutputIterator ) const;
    Set< PVertex > getVertSet() const;
    template< class OutputIterator,class VChooser2 > int getVerts(OutputIterator out, VChooser2 ch) const;
    template< class VChooser2> Set< PVertex > getVertSet(VChooser2 ch)    const;


    PVertex vertByNo( int ) const;
    int vertPos( PVertex ) const;
    PEdge edgeByNo( int ) const;
    int edgePos( PEdge ) const;


    template <class OutputIterator> int getEdges(OutputIterator, EdgeDirection = EdAll ) const;
    template< class OutputIterator,class EChooser2 > int getEdges(OutputIterator out, EChooser2 ch) const;
    template< class OutputIterator > int getEdges(OutputIterator, PVertex, EdgeDirection = EdAll ) const;
    template< class OutputIterator > int getEdges(OutputIterator, PVertex, PVertex, EdgeDirection = EdAll ) const;


    Set< PEdge > getEdgeSet( EdgeType = EdAll ) const;
    template< class EChooser2> Set< PEdge > getEdgeSet(EChooser2 ch)    const;
    Set< PEdge > getEdgeSet( PVertex, EdgeDirection = EdAll ) const;
    Set< PEdge > getEdgeSet( PVertex, PVertex, EdgeDirection = EdAll ) const;


    template< class OutputV,class OutputE,class VChooser2,class EChooser2 >
        std::pair<int,int> getChosen(std::pair<OutputV,OutputE>,std::pair<VChooser2,EChooser2>,bool =true) const;
    template<class VChooser2,class EChooser2 >
        std::pair<Set<PVertex>,Set<PEdge> >getChosenSets(std::pair<VChooser2,EChooser2>,bool =true) const;


    template <class Cont> void getAdj(Cont &,EdgeType mask=EdAll) const;


    VertInfoType getVertInfo( PVertex v ) const { assert(v); return v->getInfo(); }
    EdgeInfoType getEdgeInfo( PEdge e ) const { assert(e); return e->getInfo(); }
    bool isEdgeEnd( PEdge edge, PVertex vert ) const { return ((!edge) ?  false: edge->isEnd(vert)); }
    bool isEnd( PEdge edge, PVertex vert ) const { return (!edge) ?  false : edge->isEnd(vert); }
    EdgeType getType( PEdge e ) const { return self.getEdgeType(e); }
    std::pair<PVertex,PVertex > getEnds( PEdge edge ) const { return self.getEdgeEnds(edge); }
    PVertex getEdgeEnd( PEdge edge, PVertex vert) const { return (!edge) ?  0 : edge->getEnd(vert);}
    PVertex getEnd( PEdge edge, PVertex vert) const  { return (!edge) ?  0 : edge->getEnd(vert);}
    inline bool incid( PEdge edge1, PEdge edge2 ) const
    {   return edge1 && edge2 && (isEdgeEnd(edge1,self.getEdgeEnd1(edge2))|| isEdgeEnd(edge1,self.getEdgeEnd2(edge2))); }


    inline int deg( PVertex vert, EdgeDirection direct = EdAll ) const
    {   return self.getEdgeNo( vert,direct ) + ((direct & EdLoop) ? self.getEdgeNo( vert,EdLoop ): 0);  }
    inline int Delta( EdgeDirection direct= EdAll ) const
    {   return std::max( 0,this->maxDeg( direct ).second );   }
    inline int delta( EdgeDirection direct= EdAll ) const
    {   return std::max( 0,this->minDeg( direct ).second );  }
    std::pair< PVertex,int > minDeg( EdgeDirection = EdAll ) const;
    std::pair< PVertex,int > maxDeg( EdgeDirection = EdAll ) const;


    template< class OutputIterator > int getNeigh(OutputIterator, PVertex, EdgeDirection = EdAll ) const;
    Set< PVertex > getNeighSet( PVertex, EdgeDirection = EdAll ) const;
    int getNeighNo( PVertex, EdgeDirection = EdAll ) const;
    Set< PVertex > getClNeighSet(PVertex, EdgeDirection = EdAll ) const;
    template< class OutputIterator > int getClNeigh(OutputIterator, PVertex, EdgeDirection = EdAll ) const;
    int getClNeighNo( PVertex, EdgeDirection = EdAll ) const;


    bool areParallel( PEdge, PEdge, EdgeDirection = EdUndir ) const;
    template< class OutputIterator > int getParals( OutputIterator, PEdge, EdgeDirection = EdUndir ) const;
    Set<PEdge> getParalSet(PEdge, EdgeDirection = EdUndir) const;
    int mu(PEdge, EdgeDirection = EdUndir ) const;
    int mu( EdgeDirection = EdUndir ) const;
    std::pair< PEdge,int > maxMu( EdgeDirection = EdUndir ) const;

    template <class Iterator,class OutIter>
        int getIncEdges(OutIter,Iterator, Iterator, EdgeDirection = EdAll,EdgeType=Loop ) const;
    template <class Iterator>
        Set< PEdge > getIncEdgeSet(Iterator beg, Iterator end, EdgeDirection = EdAll,EdgeType=Loop ) const;
    template <class OutIter>
        int getIncEdges(OutIter,const Set<PVertex>&, EdgeDirection = EdAll,EdgeType=Loop ) const;
    Set< PEdge > getIncEdgeSet(const Set<PVertex>&, EdgeDirection = EdAll, EdgeType=Loop ) const;


    template <class Iterator,class OutIter> int getIncVerts(OutIter,Iterator, Iterator, EdgeDirection = EdAll,EdgeType=Loop ) const;
    template <class Iterator> Set< PVertex > getIncVertSet(Iterator beg, Iterator end, EdgeDirection = EdAll,EdgeType=Loop ) const;
    template <class OutIter> int getIncVerts(OutIter,const Set<PVertex>&, EdgeDirection = EdAll,EdgeType=Loop ) const;
    Set< PVertex > getIncVertSet(const Set<PVertex>&, EdgeDirection = EdAll,EdgeType=Loop ) const;


    template<class IterOut1, class IterOut2, class Iterator >
        std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,Iterator,Iterator, EdgeType = EdUndir ) const;
    template<class IterOut1, class IterOut2, class Iterator >
        std::pair<int,int> findParals2(std::pair<IterOut1,IterOut2>,Iterator,Iterator, EdgeType = EdUndir ) const;
    template<class IterOut1, class IterOut2 >
        std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,const Set<PEdge>&, EdgeType = EdUndir ) const;
    template<class IterOut1, class IterOut2 >
        std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,PVertex, EdgeType = EdUndir ) const;
    template<class IterOut1, class IterOut2 >
        std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>,PVertex,PVertex, EdgeType = EdUndir ) const;
    template<class IterOut1, class IterOut2 >
        std::pair<int,int> findParals(std::pair<IterOut1,IterOut2>, EdgeType = EdUndir ) const;

};

template< class GraphType > template< class OutputIterator >
int ConstGraphMethods< GraphType>::getVerts( OutputIterator  out) const
{
    int ans = 0;
    PVertex vert = this->getVert();
    while (vert)
    {
        *out = vert;
        ++out;
        ++ans;
        vert = self.getVertNext(vert);
    }
    return ans;
}

template< class GraphType >
Set< typename ConstGraphMethods< GraphType>::PVertex >
ConstGraphMethods< GraphType>::getVertSet() const
{
    Set< typename GraphType::PVertex > res;
    getVerts( setInserter( res ) );
    return res;
}

template< class GraphType > template< class OutputIterator,class VChooser2 >
int ConstGraphMethods< GraphType>::getVerts(OutputIterator out, VChooser2 ch) const
{   int licz=0;
    for(typename GraphType::PVertex v=this->getVert();v;v=self.getVertNext(v))
        if (ch(v,*this))
            {  *out=v; ++out; ++licz;   }
    return licz;
}

template< class GraphType >  template< class VChooser2 >
Set< typename ConstGraphMethods< GraphType>::PVertex >
ConstGraphMethods< GraphType>::getVertSet(VChooser2 ch)    const
{
    Set<PVertex> s;
    getVerts(setInserter(s),ch);
    return s;
}

template< class GraphType >
typename  ConstGraphMethods< GraphType>::PVertex
ConstGraphMethods< GraphType>::vertByNo( int idx ) const
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
int ConstGraphMethods< GraphType>::vertPos( typename  ConstGraphMethods< GraphType>::PVertex vert ) const
{
    int idx = 0;
    PVertex tmp_vert = this->getVert();
    while (tmp_vert && tmp_vert != vert)
    {
        tmp_vert = self.getVertNext( tmp_vert );
        ++idx;
    }
    return tmp_vert ? idx : -1;
}

template< class GraphType > template< class OutputIterator >
int ConstGraphMethods< GraphType>::getEdges(OutputIterator out, EdgeDirection direct ) const
{
    int ans = 0;
    PEdge edge = this->getEdge();
    while (edge) {
        if (self.getEdgeType(edge) & direct)
        {
            *out = edge;
            ++out;
            ++ans;
        }
        edge = self.getEdgeNext(edge);
    }
    return ans;
}

template< class GraphType > template< class OutputIterator >
int ConstGraphMethods< GraphType>::getEdges(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
    int ans = 0;
    PEdge edge = this->getEdge(vert,direct);
    while (edge) {
            *out = edge;
            ++out;
            ++ans;
        edge = self.getEdgeNext(vert,edge,direct);
    }
    return ans;
}

template< class GraphType > template< class OutputIterator >
int ConstGraphMethods< GraphType>::getEdges(
    OutputIterator out, PVertex vert1, PVertex vert2, EdgeDirection direct ) const
{
    int ans = 0;
    PEdge edge = this->getEdge(vert1,vert2,direct);
    while (edge) {
            *out = edge;
            ++out;
            ++ans;
        edge = self.getEdgeNext(vert1,vert2,edge,direct);
    }
    return ans;
}

template< class GraphType> template< class OutputIterator,class EChooser2 >
int ConstGraphMethods< GraphType>::getEdges(OutputIterator out, EChooser2 ch) const
{   int licz=0;
    for(PEdge e=this->getEdge();e;e=self.getEdgeNext(e))
        if (ch(e,*this))
            {  *out=e; ++out; ++licz;   }
    return licz;
}

template< class GraphType> template< class EChooser2 >
Set< typename  ConstGraphMethods< GraphType>::PEdge >
ConstGraphMethods< GraphType>::getEdgeSet(EChooser2 ch)    const
{
    Set<PEdge> s;
    this->getEdges(setInserter(s),ch);
    return s;
}


template< class GraphType>
Set< typename  ConstGraphMethods< GraphType>::PEdge >
ConstGraphMethods< GraphType>::getEdgeSet(EdgeType mask)    const
{
    Set<PEdge> s;
    this->getEdges(setInserter(s),mask);
    return s;
}

template< class GraphType>
Set< typename  ConstGraphMethods< GraphType>::PEdge >
ConstGraphMethods< GraphType>::getEdgeSet( PVertex vert, EdgeDirection mask ) const
{
    Set< PEdge > res;
    getEdges( setInserter( res ),vert,mask );
    return res;
}

template< class GraphType>
Set< typename  ConstGraphMethods< GraphType>::PEdge >
ConstGraphMethods< GraphType>::getEdgeSet(
    PVertex vert1, PVertex vert2, EdgeDirection mask ) const
{
    Set< PEdge > res;
    getEdges( setInserter( res ),vert1,vert2,mask );
    return res;
}


template< class GraphType>
typename  ConstGraphMethods< GraphType>::PEdge
ConstGraphMethods< GraphType>::edgeByNo( int idx ) const
{
    PEdge tmp_edge = this->getEdge();
    while (idx && tmp_edge)
    {
        tmp_edge = self.getEdgeNext( tmp_edge );
        --idx;
    }
    return tmp_edge;
}


template< class GraphType>
int ConstGraphMethods< GraphType>::edgePos( typename  ConstGraphMethods< GraphType>::PEdge edge ) const
{
    int idx = 0;
    PEdge tmp_edge = this->getEdge();
    while (tmp_edge && tmp_edge != edge)
    {
        tmp_edge = self.getEdgeNext( tmp_edge );
        ++idx;
    }
    return tmp_edge ? idx : -1;
}

template< class GraphType> template< class OutputV,class OutputE,class VChooser2,class EChooser2 >
std::pair<int,int>
ConstGraphMethods< GraphType>::getChosen(std::pair<OutputV,OutputE> out,std::pair<VChooser2,EChooser2> chs,bool chosenends) const
{   int vlicz=this->getVerts(out.first,chs.first), elicz;
    if (!chosenends) elicz=this->getEdges(out.second,chs.second);
    else elicz=this->getEdges(out.second,(edgeFirstEndChoose(chs.first) && edgeSecondEndChoose(chs.first))&&chs.second);
    return std::make_pair(vlicz,elicz);
}

template< class GraphType> template<class VChooser2,class EChooser2 >
std::pair<Set<typename ConstGraphMethods<GraphType>::PVertex>,Set<typename ConstGraphMethods<GraphType>::PEdge> >
ConstGraphMethods< GraphType>::getChosenSets(std::pair<VChooser2,EChooser2> chs,bool chosenends) const
{   Set<PVertex> sv;
    Set<PEdge> se;
    this->getChosen(std::make_pair(setInserter(sv),setInserter(se)),chs,chosenends);
    return std::make_pair(sv,se);
}

template< class GraphType>  template <class Cont>
void
ConstGraphMethods< GraphType>::getAdj(Cont& cont,EdgeType mask) const
{
    std::pair<PVertex,PVertex> ends;
    for(PEdge e=this->getEdge(mask);e;e=self.getEdgeNext(e,mask))
    {   ends=self.getEdgeEnds(e);
        cont(ends.first,ends.second)=true;
        if (self.getEdgeType(e)==Undirected)
            cont(ends.second,ends.first)=true;
    }
}

template< class GraphType>
std::pair< typename ConstGraphMethods<GraphType>::PVertex,int >
ConstGraphMethods< GraphType>::maxDeg( EdgeDirection direct ) const
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


template< class GraphType>
std::pair< typename ConstGraphMethods<GraphType>::PVertex,int >
ConstGraphMethods< GraphType>::minDeg( EdgeDirection direct ) const
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


template< class GraphType> template< class OutputIterator >
int ConstGraphMethods< GraphType>::getNeigh(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
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

template< class GraphType>
Set<  typename ConstGraphMethods<GraphType>::PVertex >
ConstGraphMethods< GraphType>::getNeighSet(
    PVertex vert, EdgeDirection mask ) const
{
    Set< PVertex > res;
    this->getNeigh( setInserter( res ),vert,mask );
    return res;
}

template< class GraphType>
int ConstGraphMethods< GraphType>::getNeighNo(
    PVertex vert, EdgeDirection mask ) const
{
    return this->getNeigh( blackHole,vert,mask );
}

template< class GraphType>
Set<  typename ConstGraphMethods<GraphType>::PVertex >
ConstGraphMethods< GraphType>::getClNeighSet(
    PVertex vert, EdgeDirection direct ) const
{
    Set< PVertex > ans = this->getNeighSet( vert,direct );
    ans.add( vert );
    return ans;
}

template< class GraphType> template< class OutputIterator >
int ConstGraphMethods< GraphType>::getClNeigh(
    OutputIterator out, PVertex vert, EdgeDirection direct ) const
{
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

template< class GraphType>
int ConstGraphMethods< GraphType>::getClNeighNo(
    PVertex vert, EdgeDirection direct ) const
{
    return this->getClNeigh( blackHole,vert,direct );
}

template< class GraphType>
bool ConstGraphMethods< GraphType>::areParallel(
    typename ConstGraphMethods< GraphType>::PEdge e1, typename ConstGraphMethods< GraphType>::PEdge e2, EdgeDirection reltype ) const
{
    if(!(e1 && e2 && (reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir))) return false;
    std::pair<PVertex,PVertex > ends1 = self.getEdgeEnds( e1 ),ends2 = self.getEdgeEnds( e2 );
    if (e1 == e2) return true;
    else if (self.getEdgeType(e1) == Loop) return self.getEdgeType(e2) == Loop && ends1.first == ends2.first;
    else if (self.getEdgeType(e2) == Loop) return false;
    else if ((self.getEdgeType(e1) == Undirected && self.getEdgeType(e2) == Undirected)
        || (self.getEdgeType(e1) != self.getEdgeType(e2) && reltype == EdUndir)
        || (self.getEdgeType(e1) == Directed && self.getEdgeType(e2) == Directed
            && (reltype == EdUndir || reltype == EdDirIn)))
        return (ends1.first == ends2.first && ends1.second == ends2.second)
                || (ends1.second == ends2.first && ends1.first == ends2.second);
    else return self.getEdgeType(e1) == self.getEdgeType(e2) && ends1.first == ends2.first
        && ends1.second == ends2.second;
}


template< class GraphType> template <class OutputIterator>
int ConstGraphMethods< GraphType>::getParals(  OutputIterator iter, PEdge edge, EdgeDirection reltype) const
{
    if (!edge || !(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir)) return 0;
    int licz=0;
    std::pair<PVertex,PVertex> ends=self.getEdgeEnds(edge);
    for(PEdge e=this->getEdge(ends.first,ends.second,EdAll);e;e=self.getEdgeNext(ends.first,ends.second,e,EdAll))
        if (e!=edge && this->areParallel(e,edge,reltype))
        {   *iter=e;
            ++iter;
            licz++;
        }
    return licz;
}

template< class GraphType>
Set< typename ConstGraphMethods<GraphType>::PEdge >
ConstGraphMethods< GraphType>::getParalSet( typename ConstGraphMethods<GraphType>::PEdge  edge, EdgeDirection reltype) const
{
    Set<PEdge > res;
    this->getParals(setInserter(res),edge,reltype);
    return res;
}

template< class GraphType>
inline int ConstGraphMethods< GraphType>::mu( PEdge edge, EdgeDirection reltype) const
{
    if (!(edge && (reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir))) return 0;
    return this->getParals(blackHole,edge,reltype)+1;
}

template< class GraphType>
inline int ConstGraphMethods< GraphType>::mu( EdgeDirection reltype) const
{
    return maxMu(reltype).second;
}

template< class GraphType>
std::pair< typename ConstGraphMethods< GraphType>::PEdge,int >
ConstGraphMethods< GraphType>::maxMu( EdgeDirection reltype) const
{
    std::pair< PEdge,int > res((PEdge)0,0);
    if (!(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir) || !self.getEdgeNo(EdAll)) return res;
    Parals3 LOCALARRAY(edges,self.root().getEdgeNo(EdAll));
    int i=0,l=0;
    PEdge edge;
    for (PEdge e=this->getEdge(EdAll); e; e=self.getEdgeNext(e,EdAll) )
        edges[i++]=Parals3(std::min(self.getEdgeEnd1(e),self.getEdgeEnd2(e)),
                           std::max(self.getEdgeEnd1(e),self.getEdgeEnd2(e)),
                           self.getEdgeDir(e,std::min(self.getEdgeEnd1(e),self.getEdgeEnd2(e))),e);
    GraphSettings::sort(edges,edges+i,Parals3cmp());
    for(i=0;i<self.getEdgeNo(EdAll);i++)
    {
        if (i==0 || !this->areParallel(edges[i-1].edge,edges[i].edge,reltype))
        {
            l=1; edge=edges[i].edge;
        }
        else l++;
        if (l>res.second) res=std::make_pair(edge,l);
    }
    return res;
}

template< class GraphType> template <class Iterator, class OutIter>
int ConstGraphMethods< GraphType>::getIncEdges(OutIter out,Iterator beg, Iterator end, EdgeDirection type,EdgeType kind ) const
{   int licze=0;
    typename GraphSettings:: template VertAssocCont <PVertex,bool>::Type vset(self.getVertNo());
    for(Iterator i=beg;i!=end;++i ) vset[*i]=true;
    for(PVertex v=vset.firstKey();v;v=vset.nextKey(v))
        for(PEdge e=this->getEdge(v,type );e;e=self.getEdgeNext(v,e,type ))
            if (    ((kind&Loop) && ( ((self.getEdgeDir(e,this->getEdgeEnd(e,v))&type)==0) || this->getEdgeEnd(e,v)>=v )
                        && vset.hasKey(this->getEdgeEnd(e,v))) ||
                    ((kind&(Directed|Undirected))  && !vset.hasKey(this->getEdgeEnd(e,v)))    )
//    for(typename Graph< VertInfo,EdgeInfo,Settings >::PEdge e=getEdge(type );e;e=getEdgeNext(e,type ))
//        if (vset.hasKey(getEdgeEnd1(e)) && vset.hasKey(getEdgeEnd2(e)))
            { *out=e; ++out; ++ licze; }
    return licze;
}

template< class GraphType> template <class Iterator>
Set<typename ConstGraphMethods< GraphType>::PEdge>
ConstGraphMethods< GraphType>::getIncEdgeSet(Iterator beg, Iterator end, EdgeDirection type,EdgeType kind ) const
{
    Set<PEdge> res;
    this->getIncEdges(setInserter(res),beg,end,type,kind);
    return res;
}

template< class GraphType> template <class OutIter>
int ConstGraphMethods< GraphType>::
getIncEdges(OutIter out,const Set<typename ConstGraphMethods< GraphType>::PVertex>& vset, EdgeDirection type,EdgeType kind ) const
{
    return this->getIncEdges(out,vset.begin(),vset.end(),type,kind);
}

template< class GraphType>
Set<typename ConstGraphMethods< GraphType>::PEdge>
ConstGraphMethods< GraphType>::getIncEdgeSet(const Set<typename ConstGraphMethods< GraphType>::PVertex>& vset, EdgeDirection type,EdgeType kind ) const
{
    Set<PEdge> res;
    getIncEdges(setInserter(res),vset.begin(),vset.end(),type,kind);
    return res;
}

template< class GraphType> template <class Iterator, class OutIter>
int ConstGraphMethods< GraphType>::getIncVerts(OutIter out,Iterator beg, Iterator end, EdgeDirection type,EdgeType kind) const
{   int liczv=0;
    typename GraphSettings:: template VertAssocCont <PVertex,bool>::Type vset(self.getVertNo());
    for(Iterator i=beg;i!=end;++i ) vset[*i]=true;
    PVertex LOCALARRAY(tabv,2*self.getEdgeNo(type));
    for(PVertex v=vset.firstKey();v;v=vset.nextKey(v))
        for(PEdge e=this->getEdge(v,type );e;e=self.getEdgeNext(v,e,type ))
            if (    ((kind&Loop) && vset.hasKey(this->getEdgeEnd(e,v))) ||
                    ((kind&(Directed|Undirected))  && !vset.hasKey(this->getEdgeEnd(e,v)))    )
                    tabv[liczv++]=this->getEdgeEnd(e,v);
    GraphSettings::sort( tabv,tabv + liczv );
    liczv = std::unique( tabv,tabv + liczv ) - tabv;
    for(int i=0;i<liczv;i++)
    {   *out=tabv[i]; ++out; }
    return liczv;
}

template< class GraphType> template <class Iterator>
Set<typename ConstGraphMethods< GraphType>::PVertex>
ConstGraphMethods< GraphType>::getIncVertSet(Iterator beg, Iterator end, EdgeDirection type,EdgeType kind ) const
{
    Set<PVertex> res;
    this->getIncVerts(setInserter(res),beg,end,type,kind);
    return res;
}

template< class GraphType> template <class OutIter>
int ConstGraphMethods< GraphType>::getIncVerts(OutIter out,const Set<typename ConstGraphMethods< GraphType>::PVertex>& vset, EdgeDirection type,EdgeType kind ) const
{
    return this->getIncVerts(out,vset.begin(),vset.end(),type,kind);
}

template< class GraphType>
Set<typename ConstGraphMethods< GraphType>::PVertex>
ConstGraphMethods< GraphType>::getIncVertSet(const Set<typename ConstGraphMethods< GraphType>::PVertex>& vset, EdgeDirection type,EdgeType kind) const
{
    Set<PVertex> res;
    this->getIncVerts(setInserter(res),vset.begin(),vset.end(),type,kind);
    return res;
}

template< class GraphType>
template< class IterOut1, class IterOut2, class Iterator  >
std::pair<int,int> ConstGraphMethods< GraphType>::findParals2(
        std::pair<IterOut1,IterOut2> out,Iterator begin,Iterator end, EdgeType relType) const
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

template< class GraphType>
template< class IterOut1, class IterOut2, class Iterator  >
std::pair<int,int> ConstGraphMethods< GraphType>::findParals(
        std::pair<IterOut1,IterOut2> out,Iterator begin,Iterator end, EdgeType reltype) const
{
    std::pair< int,int > res(0,0);
    if (!(reltype == EdDirIn || reltype == EdDirOut || reltype == EdUndir)) return std::make_pair(-1,-1);
    int size = 0;
    for( Iterator iter = begin; iter != end; iter++ ) size++;
    if (!size) return res;
    Parals3 LOCALARRAY(edges,size);
    int i=0;
    for( Iterator iter = begin; iter != end; iter++ )
    {   PEdge e=*iter;
        PVertex v1=std::min(getEdgeEnd1(e),getEdgeEnd2(e)),v2=std::max(getEdgeEnd1(e),getEdgeEnd2(e));
        edges[i++]=Parals3(v1,v2,self.getEdgeDir(e,v1),e);
    }
    GraphSettings::sort(edges,edges+size,Parals3cmp());
    for(i=0;i<size;i++)
    {
        if (i==0 || !this->areParallel(edges[i-1].edge,edges[i].edge,reltype))
        {
            *out.first=edges[i].edge; ++out.first; res.first++;
        }
        else
        {
            *out.second=edges[i].edge; ++out.second; res.second++;
        }
    }
    return res;
}

template< class GraphType>
template< class IterOut1, class IterOut2  >
std::pair<int,int> ConstGraphMethods< GraphType>::findParals(
        std::pair<IterOut1,IterOut2> out,const Set<typename ConstGraphMethods< GraphType>::PEdge>& eset, EdgeType relType) const
{
    return this->findParals(out, eset.begin(),eset.end(),relType );
}

template< class GraphType>
template< class IterOut1, class IterOut2  >
std::pair<int,int> ConstGraphMethods< GraphType>::findParals(
        std::pair<IterOut1,IterOut2> out,PVertex vert, EdgeType relType) const
{   PEdge LOCALARRAY( buf,self.getEdgeNo(vert,EdAll) );
    int size=this->getEdges(buf,vert,EdAll);
    return this->findParals(out, buf,buf+size,relType );
}

template< class GraphType>
template< class IterOut1, class IterOut2  >
std::pair<int,int> ConstGraphMethods< GraphType>::findParals(
        std::pair<IterOut1,IterOut2> out,PVertex vert1, PVertex vert2,EdgeType relType) const
{   PEdge LOCALARRAY( buf,std::min(self.getEdgeNo(vert1,EdAll),self.getEdgeNo(vert2,EdAll)) );
    int size=this->getEdges(buf,vert1,vert2,EdAll);
    return this->findParals(out, buf,buf+size,relType );
}


template< class GraphType>
template< class IterOut1, class IterOut2  >
std::pair<int,int> ConstGraphMethods< GraphType>::findParals(
        std::pair<IterOut1,IterOut2> out, EdgeType relType) const
{   PEdge LOCALARRAY( buf,self.getEdgeNo(EdAll) );
    this->getEdges(buf,EdAll);
    return this->findParals(out, buf,buf+getEdgeNo(EdAll),relType );
}


#endif
