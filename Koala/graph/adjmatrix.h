#ifndef KOALA_ADJMATRIX_H
#define KOALA_ADJMATRIX_H

namespace Koala
{

namespace Privates {

template< class VertInfo , class EdgeInfo , class Settings>
struct AdjMatrixParals {

    typename Koala::Edge< VertInfo,EdgeInfo, Settings > *first, *last;
    int degree;

    AdjMatrixParals(): first( NULL ), last( NULL ), degree( 0 ) {}
};

}

template< class VertInfo , class EdgeInfo , class Settings,bool Flag> class AdjMatrix;

template <class VertInfo , class EdgeInfo , class Settings>
class AdjMatrix<VertInfo , EdgeInfo , Settings,true> {
    public:
        typename Settings:: template AdjMatrixDirEdges
            <typename Koala::Vertex< VertInfo,EdgeInfo,Settings > *,Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings > >
                ::Type dirs;
        typename Settings:: template AdjMatrixUndirEdges
            <typename Koala::Vertex< VertInfo,EdgeInfo,Settings > *,Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings > >
                ::Type undirs;

        AdjMatrix( int asize = 0 ):
            dirs( (Settings::EdAllow&EdDirOut)? asize : 0 ),
            undirs( (Settings::EdAllow&EdUndir)? asize : 0 ) { }

        void clear() { dirs.clear(); undirs.clear(); }
        void defrag() { dirs.defrag(); undirs.defrag(); }
        void add(  Koala::Edge< VertInfo,EdgeInfo,Settings > * );
        void delVert(Koala::Vertex< VertInfo,EdgeInfo,Settings > * u)
        {   if (Settings::EdAllow & EdUndir) undirs.delInd(u);
            if (Settings::EdAllow & (EdDirIn|EdDirOut)) dirs.delInd(u);
        }
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >&
            vald(Koala::Vertex< VertInfo,EdgeInfo,Settings > * u,Koala::Vertex< VertInfo,EdgeInfo,Settings > * v)
            { return dirs(u,v); }
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >&
            valund(Koala::Vertex< VertInfo,EdgeInfo,Settings > * u,Koala::Vertex< VertInfo,EdgeInfo,Settings> * v)
            { return undirs(u,v); }
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >* undirspresentValPtr(
                Koala::Vertex< VertInfo,EdgeInfo,Settings > *  v1,Koala::Vertex< VertInfo,EdgeInfo,Settings > * v2 )
            { return undirs.valPtr(v1,v2 );}
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >* dirspresentValPtr(
                Koala::Vertex< VertInfo,EdgeInfo,Settings > *  v1,Koala::Vertex< VertInfo,EdgeInfo,Settings > * v2 )
            { return dirs.valPtr(v1,v2 );}
};

template <class VertInfo , class EdgeInfo , class Settings>
class AdjMatrix<VertInfo , EdgeInfo , Settings,false> {
    public:

        AdjMatrix( int ) {}

        void clear() {}
        void defrag() {}
        void add( void* ) {}
        void delVert(void* ) {}
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >& vald(void* ,void*)
            { return *(Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >*)(_KoalaEmptyEdgePoiner); }
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >& valund(void* ,void*)
            { return *(Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >*)(_KoalaEmptyEdgePoiner); }
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >* undirspresentValPtr(void*,void*)
            { return (Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >*)(_KoalaEmptyEdgePoiner);}
        Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >* dirspresentValPtr(void*,void*)
            { return (Privates::AdjMatrixParals< VertInfo,EdgeInfo,Settings >*)(_KoalaEmptyEdgePoiner);}
};


template<class VertInfo, class EdgeInfo, class Settings>
void AdjMatrix<VertInfo, EdgeInfo,Settings,true>::add
    (Edge<VertInfo,EdgeInfo,Settings>* edge)
{   if (!edge) return;
    if (edge->type==Directed)
    {   std::pair<typename Graph<VertInfo,EdgeInfo,Settings>::PVertex,typename Graph<VertInfo,EdgeInfo,Settings>::PVertex>
			ends(edge->vert[0].vert, edge->vert[1].vert);
	Privates::AdjMatrixParals<VertInfo, EdgeInfo,Settings>& pole=dirs(ends.first,ends.second);
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
    {   std::pair<typename Graph<VertInfo,EdgeInfo,Settings>::PVertex,typename Graph<VertInfo,EdgeInfo,Settings>::PVertex>
			ends(edge->vert[0].vert, edge->vert[1].vert);
	Privates::AdjMatrixParals<VertInfo, EdgeInfo,Settings>& pole=undirs(ends.first,ends.second);
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
