#ifndef KOALA_ADJMATRIX_H
#define KOALA_ADJMATRIX_H

namespace Koala
{

template< class VertInfo , class EdgeInfo , EdgeType EdAllow>
struct AdjMatrixParals {

    typename Koala::Edge< VertInfo,EdgeInfo,EdAllow > *first, *last;
    int degree;

    AdjMatrixParals(): first( NULL ), last( NULL ), degree( 0 ) {}
};


template< class VertInfo , class EdgeInfo , EdgeType EdAllow,EdgeType Flag> class AdjMatrix;

template <class VertInfo , class EdgeInfo , EdgeType EdAllow>
class AdjMatrix<VertInfo , EdgeInfo , EdAllow,AdjMatrixAllowed> {
    public:
        typename GraphClassDefaultSettings:: template AdjMatrixDirEdges
            <typename Koala::Vertex< VertInfo,EdgeInfo,EdAllow > *,AdjMatrixParals< VertInfo,EdgeInfo,EdAllow > >
                ::Type dirs;
        typename GraphClassDefaultSettings:: template AdjMatrixUndirEdges
            <typename Koala::Vertex< VertInfo,EdgeInfo,EdAllow > *,AdjMatrixParals< VertInfo,EdgeInfo,EdAllow > >
                ::Type undirs;

        AdjMatrix( int asize = 0 ):
            dirs( (EdAllow&EdDirOut)? asize : 0 ),
            undirs( (EdAllow&EdUndir)? asize : 0 ) { }

        void clear() { dirs.clear(); undirs.clear(); }
        void defrag() { dirs.defrag(); undirs.defrag(); }
        void add(  Koala::Edge< VertInfo,EdgeInfo,EdAllow > * );
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >&
            vald(Koala::Vertex< VertInfo,EdgeInfo,EdAllow > * u,Koala::Vertex< VertInfo,EdgeInfo,EdAllow > * v)
            { return dirs(u,v); }
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >&
            valund(Koala::Vertex< VertInfo,EdgeInfo,EdAllow > * u,Koala::Vertex< VertInfo,EdgeInfo,EdAllow > * v)
            { return undirs(u,v); }
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >* undirspresentValPtr(
                Koala::Vertex< VertInfo,EdgeInfo,EdAllow > *  v1,Koala::Vertex< VertInfo,EdgeInfo,EdAllow > * v2 )
            { return undirs.presentValPtr(v1,v2 );}
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >* dirspresentValPtr(
                Koala::Vertex< VertInfo,EdgeInfo,EdAllow > *  v1,Koala::Vertex< VertInfo,EdgeInfo,EdAllow > * v2 )
            { return dirs.presentValPtr(v1,v2 );}
};

template <class VertInfo , class EdgeInfo , EdgeType EdAllow>
class AdjMatrix<VertInfo , EdgeInfo , EdAllow,0> {
    public:

        AdjMatrix( int ) {}

        void clear() {}
        void defrag() {}
        void add( void* ) {}
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >& vald(void* ,void*)
            { return *(AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >*)(_KoalaEmptyEdgePoiner); }
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >& valund(void* ,void*)
            { return *(AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >*)(_KoalaEmptyEdgePoiner); }
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >* undirspresentValPtr(void*,void*)
            { return (AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >*)(_KoalaEmptyEdgePoiner);}
        AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >* dirspresentValPtr(void*,void*)
            { return (AdjMatrixParals< VertInfo,EdgeInfo,EdAllow >*)(_KoalaEmptyEdgePoiner);}
};


template<class VertInfo, class EdgeInfo, EdgeType EdAllow>
void AdjMatrix<VertInfo, EdgeInfo,EdAllow,AdjMatrixAllowed>::add
    (Edge<VertInfo,EdgeInfo,EdAllow>* edge)
{   if (!edge) return;
    if (edge->type==Directed)
    {   std::pair<typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex,typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex>
			ends(edge->vert[0].vert, edge->vert[1].vert);
	AdjMatrixParals<VertInfo, EdgeInfo,EdAllow>& pole=dirs(ends.first,ends.second);
	edge->pParal() = pole.last;
	edge->nParal() = NULL;
	if(edge->pParal())
	    edge->pParal()->nParal() = edge;
	else
	    pole.first = edge;
	pole.last = edge;
	pole.degree++;
    }
    else if (edge->type==Undirected)
    {   std::pair<typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex,typename Graph<VertInfo,EdgeInfo,EdAllow>::PVertex>
			ends(edge->vert[0].vert, edge->vert[1].vert);
	AdjMatrixParals<VertInfo, EdgeInfo,EdAllow>& pole=undirs(ends.first,ends.second);
	edge->pParal() = pole.last;
	edge->nParal() = NULL;
	if(edge->pParal())
	    edge->pParal()->nParal() = edge;
	else
	    pole.first = edge;
	pole.last = edge;
	pole.degree++;
    }
}


}

#endif
