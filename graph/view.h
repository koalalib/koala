#ifndef KOALA_SUBGRAPH_H
#define KOALA_SUBGRAPH_H

//Widoki na graf pozwalaja operowac na czesci danego grafu lub na jego przeksztalconej formie
//jak na zwyklym grafie stalym (const) bez wykonywania jego kopii. Metody widoku odwoluja sie
//do grafu macierzystego i dzialaja na jego wierz/kraw. Mozna tworzych hierarchicznie widoki
//do widokow itd.
// Widoki dzialaja poprawnie, o ile ich struktury nadrzedne nadal zyja.
// Smierc ktorejs ze struktur nadrzednych powoduje odlaczenie od niej wszystkich dzieci,
// niemniej nizsze hierarchiczne powiazania utworzone przez te obiekty pozostaja
// (np. ich ponowne podlaczenie do innego grafu sprawi, ze nizej podpiete widoki beda dzialac).


#include "graph.h"

namespace Koala
{

/* ------------------------------------------------------------------------- *
 * SubGraph< Graph,VChooser,EChooser >
 *
 * ??
 *
 * ------------------------------------------------------------------------- */

template < class Graph, class VChooser, class EChooser  > class Subgraph;

template< class GraphType> struct GraphInternalTypes;
template < class Graph, class VChooser, class EChooser  >
struct GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> > {
        typedef typename Graph::Vertex Vertex;
        typedef typename Graph::PVertex PVertex;
        typedef typename Graph::Edge Edge;
        typedef typename Graph::PEdge PEdge;
        typedef typename Graph::VertInfoType VertInfoType;
        typedef typename Graph::EdgeInfoType EdgeInfoType;
        typedef typename Graph::GraphSettings GraphSettings;

};


// Klasa podgrafu struktury grafowej typu Graph, do podgrafu wybierane sa wierzcholki spelniajace
// chooser vchoose i krawedzie spelniajace echoose z obydwoma koncami spelniajacymi vchoose
template< class Graph, class VChooser, class EChooser >
class Subgraph: public SubgraphBase,
                public ConstGraphMethods<Subgraph< Graph, VChooser, EChooser>  >
{
    public:

         // choosery predykatow definiujacych podgraf
         mutable VChooser vchoose;
         mutable EChooser echoose;

        // te typy sa takie same, jak w grafie nadrzednym
        typedef typename GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> >::Vertex Vertex;
        typedef typename GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> >::PVertex PVertex;
        typedef typename GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> >::Edge Edge;
        typedef typename GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> >::PEdge PEdge;
        typedef typename GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> >::VertInfoType VertInfoType;
        typedef typename GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> >::EdgeInfoType EdgeInfoType;
        typedef typename GraphInternalTypes<Subgraph< Graph, VChooser, EChooser> >::GraphSettings GraphSettings;

        typedef Subgraph< Graph,VChooser,EChooser > GraphType; // typ tej struktury
        typedef typename Graph::RootGrType RootGrType; // typ najwyzszego wierzcholka hierarchii
        typedef Graph ParentGrType; // typ grafu nadrzednego

    // Konstruktory
        Subgraph() {} // tworzy podgraf nie podlaczony do zadnego grafu
        // tworzy podgraf danego grafu i ustawia wartosci obu chooserow
        Subgraph(
            const Graph &,
            std::pair< VChooser,EChooser > = std::make_pair( VChooser(),EChooser() ) );
        Subgraph( std::pair< VChooser,EChooser > ); // ustawia wartosci obu chooserow, tworzy podgraf niepodlaczony
        void setChoose( const std::pair< VChooser,EChooser > & ); // ustawienie wartosci chooserow

        // dopuszczalne typy krawedzi
        static EdgeType allowedEdgeTypes() { return ParentGrType::allowedEdgeTypes(); }

        void plug(const Graph &g ) { SubgraphBase::link( &g ); } // rozlacza sie od rodzica (jesli taki istnial) i podlacza do podanego grafu
        bool unplug() { return SubgraphBase::unlink(); } // sprawia, ze podgraf staje sie niepodlaczony do zadnego rodzica
        const RootGrType* getRootPtr() const // adres najwyzszej struktury hierarchii lub NULL w razie braku
        {
            return parent ? (( const ParentGrType*)parent)->getRootPtr() : NULL;
        }
        const ParentGrType* getParentPtr() const { return (const ParentGrType*)parent; } // adres grafu, do ktorego obiekt jest bezposrednio podlaczony, lub NULL w razie braku
        // j.w. ale referencje
        const RootGrType &root() const
        {
            const RootGrType *res = getRootPtr();
            assert( res ); // TODO: throw
            return *res;
        }
        const ParentGrType &up() const
        {
            const ParentGrType *res = getParentPtr();
            assert( res ); // TODO: throw
            return *res;
        }


        // Test przynaleznosci wierzcholka z rodzica do podgrafu. Dla NULL zawsze prawda.
        // w przeciwnym razie sprawdzany jest predykat vchoose. Flaga true wymusza tez analogiczne sprawdzanie we wszystkich strukturach az do korzenia
        bool good( PVertex, bool = false ) const;
        // j.w. ale dla krawedzi - sprawdzany jest predykat echoose i vchoose dla obu koncow
        bool good( PEdge, bool = false ) const;


        // metody dla ConstGraphMethods
        int getVertNo() const;
        PVertex getVertNext( PVertex ) const;
        PVertex getVertPrev( PVertex ) const;

        int getEdgeNo( EdgeDirection = EdAll ) const;
        PEdge getEdgeNext( PEdge, EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PEdge, EdgeDirection = EdAll ) const;

        PEdge getEdgeNext( PVertex, PEdge, EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PVertex, PEdge, EdgeDirection = EdAll ) const;
        int getEdgeNo( PVertex, EdgeDirection = EdAll) const;

        PEdge getEdgeNext( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        PEdge getEdgePrev( PVertex, PVertex, PEdge, EdgeDirection = EdAll ) const;
        int getEdgeNo( PVertex, PVertex, EdgeDirection = EdAll ) const;


        EdgeType getEdgeType( PEdge e ) const { return up().getEdgeType(e); }
        std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const { return up().getEdgeEnds(edge); }
        PVertex getEdgeEnd1( PEdge edge ) const { return up().getEdgeEnd1(edge); }
        PVertex getEdgeEnd2( PEdge edge ) const { return up().getEdgeEnd2(edge); }
        EdgeDirection getEdgeDir( PEdge edge, PVertex v) const { return up().getEdgeDir(edge,v); }

    protected:

        template <class T>
        static bool isEdgeTypeChooser(const T& x, Koala::EdgeDirection& val) { return false; }
        static bool isEdgeTypeChooser(const EdgeTypeChooser&  x, Koala::EdgeDirection& val) { val=x.mask; return true; }

        template <class T>
        static bool isBoolChooser(const T& x, bool& val) { return false; }
        static bool isBoolChooser(const BoolChooser&  x, bool& val) { val=x.val; return true; }

};

// zwraca podgraf danego grafu
template< class Graph, class VChooser, class EChooser >
Subgraph< Graph,VChooser,EChooser > makeSubgraph(
     const Graph &, const std::pair< VChooser,EChooser > & );




template < class Graph > class UndirView;

template< class GraphType> struct GraphInternalTypes;
template < class Graph  >
struct GraphInternalTypes<UndirView< Graph> > {
        typedef typename Graph::Vertex Vertex;
        typedef typename Graph::PVertex PVertex;
        typedef typename Graph::Edge Edge;
        typedef typename Graph::PEdge PEdge;
        typedef typename Graph::VertInfoType VertInfoType;
        typedef typename Graph::EdgeInfoType EdgeInfoType;
        typedef typename Graph::GraphSettings GraphSettings;

};


// widok na graf typu Graph, przy czym wszystkie krawedzie rozne od petli sa widziane jako nieskierowane
template< class Graph >
class UndirView: public SubgraphBase,
                public ConstGraphMethods<UndirView< Graph> >
{
    public:

        // ten sam sens typow, co dla podgrafu
        typedef typename GraphInternalTypes<UndirView< Graph> >::Vertex Vertex;
        typedef typename GraphInternalTypes<UndirView< Graph> >::PVertex PVertex;
        typedef typename GraphInternalTypes<UndirView< Graph> >::Edge Edge;
        typedef typename GraphInternalTypes<UndirView< Graph> >::PEdge PEdge;
        typedef typename GraphInternalTypes<UndirView< Graph> >::VertInfoType VertInfoType;
        typedef typename GraphInternalTypes<UndirView< Graph> >::EdgeInfoType EdgeInfoType;
        typedef typename GraphInternalTypes<UndirView< Graph> >::GraphSettings GraphSettings;

        typedef UndirView< Graph> GraphType;
        typedef typename Graph::RootGrType RootGrType;
        typedef Graph ParentGrType;

    // Konstruktory
        UndirView() {} // tworzy widok niepodlaczony do zadnej struktury
        UndirView(const Graph & g) { SubgraphBase::link( &g ); };

        static EdgeType allowedEdgeTypes()
        { return (((~EdLoop)&ParentGrType::allowedEdgeTypes()) ? Undirected :0 )
                 | ((EdLoop&ParentGrType::allowedEdgeTypes()) ? EdLoop : 0 ); }
        const RootGrType* getRootPtr() const { return parent ? (( const ParentGrType*)parent)->getRootPtr() : NULL; }
        const ParentGrType* getParentPtr() const { return (const ParentGrType*)parent; }
        const ParentGrType &up() const { const ParentGrType *res = getParentPtr(); assert( res ); return *res; }
        const RootGrType &root() const { const RootGrType *res = getRootPtr(); assert( res ); return *res; }
        void plug(const Graph &g ) { SubgraphBase::link( &g ); } // rozlacza obiekt od jego rodzica (jesli taki istnial) i podlacza do podanego grafu
        bool unplug() { return SubgraphBase::unlink(); } // sprawia, ze podgraf staje sie niepodlaczony do zadnego rodzica


        // do wspolpracy z podgrafami
        bool good( PVertex, bool = false ) const { return true; }
        bool good( PEdge, bool = false ) const { return true; }

        // na uzytek ConstGraphMethods
        int getVertNo() const { return up().getVertNo(); }
        PVertex getVertNext( PVertex v) const { return up().getVertNext(v); }
        PVertex getVertPrev( PVertex v) const { return up().getVertPrev(v); }

        int getEdgeNo( EdgeDirection mask = EdAll ) const { return up().getEdgeNo(transl(mask)); }
        PEdge getEdgeNext( PEdge e, EdgeDirection mask = EdAll ) const { return up().getEdgeNext(e,transl(mask)); }
        PEdge getEdgePrev( PEdge e, EdgeDirection mask = EdAll ) const { return up().getEdgePrev(e,transl(mask)); }

        PEdge getEdgeNext( PVertex v, PEdge e, EdgeDirection mask= EdAll ) const
        { return up().getEdgeNext(v,e,transl(mask)); }
        PEdge getEdgePrev( PVertex v, PEdge e, EdgeDirection mask= EdAll ) const
        { return up().getEdgePrev(v,e,transl(mask)); }
        int getEdgeNo( PVertex v, EdgeDirection mask= EdAll) const { return up().getEdgeNo(v,transl(mask)); }

        PEdge getEdgeNext( PVertex v, PVertex u, PEdge e, EdgeDirection mask= EdAll ) const
        { return up().getEdgeNext(v,u,e,transl(mask)); }
        PEdge getEdgePrev( PVertex v, PVertex u, PEdge e, EdgeDirection mask= EdAll ) const
        { return up().getEdgePrev(v,u,e,transl(mask)); }
        int getEdgeNo( PVertex v, PVertex u, EdgeDirection mask= EdAll ) const { return up().getEdgeNo(v,u,transl(mask)); }


        EdgeType getEdgeType( PEdge e ) const { return (up().getEdgeType(e)==EdLoop) ? EdLoop : EdUndir; }
        std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const { return up().getEdgeEnds(edge); }
        PVertex getEdgeEnd1( PEdge edge ) const { return up().getEdgeEnd1(edge); }
        PVertex getEdgeEnd2( PEdge edge ) const { return up().getEdgeEnd2(edge); }
        EdgeDirection getEdgeDir( PEdge edge, PVertex v) const
        { EdgeDirection dir= up().getEdgeDir(edge,v); return (dir==EdNone||dir==EdLoop) ? dir : EdUndir; }

    protected:

        static EdgeDirection transl(EdgeDirection mask)
        { return ((mask&EdLoop) ? EdLoop : 0) | ((mask&EdUndir) ? (Directed|Undirected) : 0); }


};

// Tworzy widok dla podanego grafu
template< class Graph>
UndirView< Graph> makeUndirView( const Graph & g)
{
    return UndirView< Graph>( g );
}



template < class Graph > class RevView;

template< class GraphType> struct GraphInternalTypes;
template < class Graph  >
struct GraphInternalTypes<RevView< Graph> > {
        typedef typename Graph::Vertex Vertex;
        typedef typename Graph::PVertex PVertex;
        typedef typename Graph::Edge Edge;
        typedef typename Graph::PEdge PEdge;
        typedef typename Graph::VertInfoType VertInfoType;
        typedef typename Graph::EdgeInfoType EdgeInfoType;
        typedef typename Graph::GraphSettings GraphSettings;

};

// widok grafu typy Graph z odwroconymi wszystkimi krawedziami skierowanymi
template< class Graph>
class RevView: public SubgraphBase,
                public ConstGraphMethods<RevView< Graph> >
{
    public:

        typedef typename GraphInternalTypes<RevView< Graph> >::Vertex Vertex;
        typedef typename GraphInternalTypes<RevView< Graph> >::PVertex PVertex;
        typedef typename GraphInternalTypes<RevView< Graph> >::Edge Edge;
        typedef typename GraphInternalTypes<RevView< Graph> >::PEdge PEdge;
        typedef typename GraphInternalTypes<RevView< Graph> >::VertInfoType VertInfoType;
        typedef typename GraphInternalTypes<RevView< Graph> >::EdgeInfoType EdgeInfoType;
        typedef typename GraphInternalTypes<RevView< Graph> >::GraphSettings GraphSettings;

        typedef RevView< Graph> GraphType;
        typedef typename Graph::RootGrType RootGrType;
        typedef Graph ParentGrType;

    // Konstruktory
        RevView() {}
        RevView(const Graph & g) { SubgraphBase::link( &g ); };


        static EdgeType allowedEdgeTypes()  { return ParentGrType::allowedEdgeTypes(); }
        const RootGrType* getRootPtr() const { return parent ? (( const ParentGrType*)parent)->getRootPtr() : NULL; }
        const ParentGrType* getParentPtr() const { return (const ParentGrType*)parent; }
        const ParentGrType &up() const { const ParentGrType *res = getParentPtr(); assert( res ); return *res; }
        const RootGrType &root() const { const RootGrType *res = getRootPtr(); assert( res ); return *res; }
        void plug(const Graph &g ) { SubgraphBase::link( &g ); } // rozlacza obiekt od jego rodzica (jesli taki istnial) i podlacza do podanego grafu
        bool unplug() { return SubgraphBase::unlink(); } // sprawia, ze podgraf staje sie niepodlaczony do zadnego rodzica


        // do wspolpracy z podgrafami
        bool good( PVertex, bool = false ) const { return true; }
        bool good( PEdge, bool = false ) const { return true; }

        // na uzytek ConstGraphMethods
        int getVertNo() const { return up().getVertNo(); }
        PVertex getVertNext( PVertex v) const { return up().getVertNext(v); }
        PVertex getVertPrev( PVertex v) const { return up().getVertPrev(v); }

        int getEdgeNo( EdgeDirection mask = EdAll ) const { return up().getEdgeNo(mask); }
        PEdge getEdgeNext( PEdge e, EdgeDirection mask = EdAll ) const
        {   return up().getEdgeNext(e,(mask));   }
        PEdge getEdgePrev( PEdge e, EdgeDirection mask = EdAll ) const
        {   return up().getEdgePrev(e,(mask)); }

        PEdge getEdgeNext( PVertex v, PEdge e, EdgeDirection mask= EdAll ) const
//          nie usuwac komentarza - waham sie co do wersji
//        { return up().getEdgeNext(v,e,transl(mask)); }
        { return getNext(v,e,transl(mask)); }
        PEdge getEdgePrev( PVertex v, PEdge e, EdgeDirection mask= EdAll ) const
//          nie usuwac komentarza - waham sie co do wersji
//        { return up().getEdgePrev(v,e,transl(mask)); }
        { return getPrev(v,e,transl(mask)); }
        int getEdgeNo( PVertex v, EdgeDirection mask= EdAll) const { return up().getEdgeNo(v,transl(mask)); }

        PEdge getEdgeNext( PVertex v, PVertex u, PEdge e, EdgeDirection mask= EdAll ) const
//          nie usuwac komentarza - waham sie co do wersji
//        { return up().getEdgeNext(v,u,e,transl(mask)); }
        { return getNext(v,u,e,transl(mask)); }
        PEdge getEdgePrev( PVertex v, PVertex u, PEdge e, EdgeDirection mask= EdAll ) const
//          nie usuwac komentarza - waham sie co do wersji
//        { return up().getEdgePrev(v,u,e,transl(mask)); }
        { return getPrev(v,u,e,transl(mask)); }
        int getEdgeNo( PVertex v, PVertex u, EdgeDirection mask= EdAll ) const { return up().getEdgeNo(v,u,transl(mask)); }


        EdgeType getEdgeType( PEdge e ) const { return up().getEdgeType(e); }

        std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const
        {   std::pair< PVertex,PVertex > res=up().getEdgeEnds(edge);
            switch (up().getEdgeType(edge))
            {   case EdNone:
                case Loop :
                case Undirected : return res;
                default : return std::make_pair(res.second,res.first);
            }
        }

        PVertex getEdgeEnd1( PEdge edge ) const
        {   std::pair< PVertex,PVertex > res=up().getEdgeEnds(edge);
            switch (up().getEdgeType(edge))
            {   case EdNone:
                case Loop :
                case Undirected : return res.first;
                default : return res.second;
            }
        }

        PVertex getEdgeEnd2( PEdge edge ) const
        {   std::pair< PVertex,PVertex > res=up().getEdgeEnds(edge);
            switch (up().getEdgeType(edge))
            {   case EdNone:
                case Loop :
                case Undirected : return res.second;
                default : return res.first;
            }
        }

        EdgeDirection getEdgeDir( PEdge edge, PVertex v) const
        {   EdgeDirection dir= up().getEdgeDir(edge,v);
            switch (dir)
            {
                case EdDirIn: return EdDirOut;
                case EdDirOut: return EdDirIn;
                default : return dir;
            }
        }

    protected:

        static EdgeDirection transl(EdgeDirection mask)
        {   EdgeDirection dirmask= mask&Directed;
            switch (dirmask)
            {
                case Directed:
                case 0 : break;
                case EdDirIn : dirmask=EdDirOut; break;
                case EdDirOut : dirmask=EdDirIn; break;
            }
            return (mask&(~Directed)) | dirmask;
        }

        static EdgeDirection nextDir(EdgeDirection dir)
        {   switch (dir)
            {
                case EdLoop: return EdUndir;
                case EdUndir: return EdDirOut;
                case EdDirOut: return EdDirIn;
            }
            return EdNone;
        }

        static EdgeDirection prevDir(EdgeDirection dir)
        {   switch (dir)
            {
                case EdDirIn: return EdDirOut;
                case EdDirOut: return EdUndir;
                case EdUndir: return EdLoop;
            }
            return EdNone;
        }

        PEdge getNext(PVertex vert, PEdge edge, EdgeDirection direct ) const
        {
            assert(vert && !(edge && !this->isEdgeEnd( edge,vert ))); // TODO: throw;
            if (!direct) return NULL;
            EdgeDirection type = up().getEdgeDir( edge,vert );
            EdgeDirection nexttype = (type == EdNone) ? EdLoop : nextDir(type);
            PEdge res;
            if (edge && (type & direct)) res = up().getEdgeNext(vert,edge,type);
            else res = 0;
            if (res) return res;
            switch (nexttype)
            {
                case EdLoop:
                    if (direct & EdLoop) res = up().getEdgeNext(vert,(PEdge)0,EdLoop);
                    if (res) return res;
                case EdUndir:
                    if (direct & EdUndir) res = up().getEdgeNext(vert,(PEdge)0,EdUndir);
                    if (res) return res;
                case EdDirOut:
                    if (direct & EdDirOut) res = up().getEdgeNext(vert,(PEdge)0,EdDirOut);
                    if (res) return res;
                case EdDirIn:
                    if (direct & EdDirIn) res = up().getEdgeNext(vert,(PEdge)0,EdDirIn);
            }
            return res;
        }

        PEdge getPrev(PVertex vert, PEdge edge, EdgeDirection direct ) const
        {
            assert(vert && !(edge && !this->isEdgeEnd( edge,vert ))); // TODO: throw;
            if (!direct) return NULL;
            EdgeDirection type = up().getEdgeDir( edge,vert );
            EdgeDirection nexttype = (type == EdNone) ? EdDirIn : prevDir(type);
            PEdge res;
            if (edge && (type & direct)) res = up().getEdgePrev(vert,edge,type);
            else res = 0;
            if (res) return res;
            switch (nexttype)
            {
                case EdDirIn:
                    if (direct & EdDirIn) res = up().getEdgePrev(vert,(PEdge)0,EdDirIn);
                    if (res) return res;
                case EdDirOut:
                    if (direct & EdDirOut) res = up().getEdgePrev(vert,(PEdge)0,EdDirOut);
                    if (res) return res;
                case EdUndir:
                    if (direct & EdUndir) res = up().getEdgePrev(vert,(PEdge)0,EdUndir);
                    if (res) return res;
                case EdLoop:
                    if (direct & EdLoop) res = up().getEdgePrev(vert,(PEdge)0,EdLoop);
                    if (res) return res;
            }
            return res;
        }

        PEdge getPrev(PVertex vert1, PVertex vert2, PEdge edge, EdgeDirection direct ) const
        {
            do
                edge = getPrev( vert1,edge,direct );
            while (edge && up().getEdgeEnd( edge,vert1 ) != vert2);
            return edge;
        }


        PEdge getNext(PVertex vert1, PVertex vert2, PEdge edge, EdgeDirection direct ) const
        {
            do
                edge = getNext( vert1,edge,direct );
            while (edge && up().getEdgeEnd( edge,vert1 ) != vert2);
            return edge;
        }

};


// tworzy widok do podanego grafu
template< class Graph>
RevView< Graph> makeRevView( const Graph & g)
{
    return RevView< Graph>( g );
}



#include "view.hpp"

}

#endif