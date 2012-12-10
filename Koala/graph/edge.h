#ifndef KOALA_EDGE_H
#define KOALA_EDGE_H

namespace Koala
{
    /* EdgeConst
     *
     */
    class EdgeConst
    {
      public:
        //do not change constans
        enum { V_U = 0, V_out = 0, V_loop = 0};
        enum { V_V = 1, V_in = 1, V_Nloop = 1};
    };

    namespace Privates
    {
        /* NormalParalLink
         *
         */
        template< class VertInfo, class EdgeInfo, class Settings > class NormalParalLink
        {
          protected:
            Edge< VertInfo,EdgeInfo,Settings > *n,*p;

          public:
            NormalParalLink(): n( NULL ), p( NULL )
                { }

            Edge< VertInfo,EdgeInfo,Settings > *&nParal()
                { return n; }
            Edge< VertInfo,EdgeInfo,Settings > *&pParal()
                { return p; }
        };

        /* EmptyParalLink
         *
         */
        template< class VertInfo, class EdgeInfo, class Settings > struct EmptyParalLink
        {
            EmptyParalLink() { }

            DummyVar< Edge< VertInfo,EdgeInfo,Settings > * > nParal()
                { return DummyVar< Edge< VertInfo,EdgeInfo,Settings > * >(); }
            DummyVar< Edge< VertInfo,EdgeInfo,Settings > * > pParal()
                { return DummyVar< Edge< VertInfo,EdgeInfo,Settings > * >(); }
        };

        /* ParalLink
         *
         */
        template< class VertInfo, class EdgeInfo, class Settings, bool Present > struct ParalLink;
        template< class VertInfo, class EdgeInfo, class Settings > struct ParalLink< VertInfo,EdgeInfo,Settings,true >:
            public NormalParalLink< VertInfo,EdgeInfo,Settings > { };
        template< class VertInfo, class EdgeInfo, class Settings > struct ParalLink< VertInfo,EdgeInfo,Settings,false >:
            public EmptyParalLink< VertInfo,EdgeInfo,Settings > { };
    }

    /* Edge
     * Klasa krawedzi glownej struktury grafu
     */
    template< class VertInfo = EmptyVertInfo, class EdgeInfo = EmptyEdgeInfo,
        class Settings = GrDefaultSettings< EdAll,true > > class Edge: public EdgeConst,
        public Settings::template EdgeAdditData< VertInfo,EdgeInfo,Settings >,
        public Privates::ParalLink< VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed >
    {
        friend class Graph< VertInfo,EdgeInfo,Settings >;
        friend class Vertex< VertInfo,EdgeInfo,Settings >;
        friend class AdjMatrix< VertInfo,EdgeInfo,Settings,Settings::AdjMatrixAllowed >;

      public:
        // Additional user information in the edge.
        EdgeInfo info;

        // zwracaja wierzcholki koncowe krawedzi
        std::pair< Vertex< VertInfo,EdgeInfo,Settings > *,Vertex< VertInfo,EdgeInfo,Settings > * > getEnds()
            { return std::make_pair( vert[0].vert,vert[1].vert ); }
        Vertex< VertInfo,EdgeInfo,Settings > *getEnd1()
            { return vert[0].vert; }
        Vertex< VertInfo,EdgeInfo,Settings > *getEnd2()
            { return vert[1].vert; }
        // czy podany wierzcholek jest koncem krawedzi
        bool isEnd(Vertex<VertInfo,EdgeInfo,Settings>* v);

        // zwroc drugi koniec krawedzi
        Vertex< VertInfo,EdgeInfo,Settings > *getEnd( Vertex< VertInfo,EdgeInfo,Settings > *v );

        // sposob orientacji krawedzi wzgledem jej podanego konca
        EdgeDirection getDir( Vertex< VertInfo,EdgeInfo,Settings > *v );
        EdgeType getType()
            { return type; }
        EdgeInfo getInfo()
            { return info; }

        void setInfo( const EdgeInfo &info )
            { this->info=info; }

    private:
        // powiazania do list krawedzi tego samego rodzaju przy wierzcholku koncowym tej krawedzi
        //0==U==out; 1==V==in;
        struct EdgeLink {
            Vertex< VertInfo,EdgeInfo,Settings > *vert;
            Edge *next,*prev;

            EdgeLink(): vert( NULL ), next( NULL ), prev( NULL )
                { }
        } vert[2];

        // klasa jest niekopiowalna, obiekty mozna tworzyc i usuwac jedynie z metod klas zaprzyjaznionych
        /** Standard constructor. */
        Edge(): info( ), next( NULL ), prev( NULL ), type( Detached )
            { }
        /** Constructor sets info variable. */
        Edge( const EdgeInfo &infoExt ): info( infoExt ), next( NULL ), prev( NULL ), type( Detached )
            { }

        // klasa niekopiowalna
        Edge( const Edge &X )
            { }
        Edge &operator=( const Edge &X )
            { }
        ~Edge()
            { }

        // powiazania na liscie wszystkich krawedzi grafu
        Edge *next,*prev;
        // typ krawedzi
        EdgeType type;
    };

#include "edge.hpp"
}

#endif
