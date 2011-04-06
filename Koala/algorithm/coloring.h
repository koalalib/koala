#ifndef __COLORING__H__
#define __COLORING__H__

#include "../base/def_struct.h"
#include "../graph/subgraph.h"
#include "search.h"
#include <algorithm>
#include <cassert>

namespace Koala {

/* ------------------------------------------------------------------------- *
    SeqVertColoring

        Zachłanne wierzchołkowe kolorowanie grafów.
 * ------------------------------------------------------------------------- */

class SeqVertColoring {
    protected:
        template< typename Pair > struct LfCmp
        {
            bool operator() ( Pair a, Pair b )
            {
                return a.second > b.second || (a.second == b.second && a.first < b.first);
            }
        } ;

        template< typename Pair > struct SlCmp
        {
            bool operator() ( Pair a, Pair b )
            {
                return a.second < b.second || (a.second == b.second && a.first > b.first);
            }
        } ;

        template< typename Triple > struct SlfCmp
        {
            bool operator() ( Triple a, Triple b )
            {
                return a.second.first > b.second.first ||
                    (a.second.first == b.second.first
                    && (a.second.second > b.second.second
                    || (a.second.second == b.second.second && a.first < b.first)));
            }
        } ;

        // Stopień nasycenia wierzchołka.
        template< typename Graph, typename ColorMap >
        static int satDeg(const Graph &g, ColorMap &c, typename Graph::PVertex v )
        {
            int sd = 0, s = g.getEdgeNo( v,EdDirIn|EdDirOut|EdUndir ) + 1;
            bool LOCALARRAY( ns,s );
            for( int i = 0; i < s; i++ ) ns[i] = false;

            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do
            {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && c[u] >= 0 && c[u] < s) ns[c[u]] = true;
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir ));

            for( int i = 0; i < s; i++ ) sd += ns[i];
            return sd;
        }

    public:
        // Kolorowanie pojedynczego wierzchołka.
        template< typename Graph, typename ColorMap >
        static int greedy(const Graph &g, ColorMap &c, typename Graph::PVertex v )
        {
            if (c.hasKey( v ) && c[v] >= 0) return -1;

            int c1 = 0, s = g.getEdgeNo( v,EdDirIn|EdDirOut|EdUndir ) + 1;
            bool LOCALARRAY( ns,s );
            for( int i = 0; i < s; i++ ) ns[i] = false;

            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do
            {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && c[u] >= 0 && c[u] < s) ns[c[u]] = true;
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir ));

            while (ns[c1]) c1++;
            return c[v] = c1;
        }

        // Kolorowanie podanego zakresu wierzchołków.
        template< typename Graph, typename ColorMap, typename Iterator >
        static int greedy(const Graph &g, ColorMap &c, Iterator b, Iterator e )
        {
            int c1 = -1;
            while (b != e)
            {
                int c2 = greedy( g,c,*b++ );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu.
        template< typename Graph, typename ColorMap >
        static int greedy(const Graph &g, ColorMap &c )
        {
            int c1 = -1;
            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
            {
                int c2 = greedy( g,c,v );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu, metoda Largest First.
        template< typename Graph, typename ColorMap >
        static int lf(const Graph &g, ColorMap &c )
        {
            typedef std::pair< typename Graph::PVertex,int > Pair;

            int n = 0;
            Pair LOCALARRAY( vs,g.getVertNo() );

            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
                if (!c.hasKey( v ) || c[v] < 0)
                {
                    vs[n].first = v;
                    vs[n].second = g.deg( vs[n++].first,EdDirIn|EdDirOut|EdUndir );
                }
            sort( vs,vs + n,LfCmp< Pair >() );

            int c1 = -1;
            for( int i = 0; i < n; i++ ) {
                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie podanego zakresu wierzchołków, metoda Largest First.
        template < typename Graph, typename ColorMap, typename Iterator >
        static int lf(const Graph &g, ColorMap &c , Iterator b, Iterator e )
        {
            typedef std::pair< typename Graph::PVertex,int > Pair;

            int n = 0, m = 0;
            for( Iterator i = b; i != e; ++i ) n++;
            Pair LOCALARRAY( vs,n );

            for( ; b != e;  b++ )
                if (!c.hasKey( *b ) || c[*b] < 0)
                {
                    vs[m].first = *b;
                    vs[m].second = g.deg( vs[m++].first,EdDirIn|EdDirOut|EdUndir );
                }
            sort( vs,vs + m,LfCmp< Pair >() );
            m = unique( vs,vs + n ) - vs;

            int c1 = -1;
            for( int i = 0; i < m; i++ ) {
                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu, metoda Smallest Last.
        template< typename Graph, typename ColorMap >
        static int sl(const Graph &g, ColorMap &c )
        {
            typedef std::pair< typename Graph::PVertex,int > Pair;
            Pair LOCALARRAY( vs,g.getVertNo() * (g.Delta() + 1)+2 );

            AssocArray< typename Graph::PVertex,int > vd;
            int n = 0;

            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
                if (!c.hasKey( v ) || c[v] < 0)
                {
                    vd[v] = g.deg( v,EdDirIn|EdDirOut|EdUndir );
                    vs[n++] = std::make_pair( v,vd[v] );
                }

            PriQueueInterface< Pair*,SlCmp< Pair > > pq( vs,vs + n,g.getVertNo() * (g.Delta() + 1) );
            typename Graph::PVertex LOCALARRAY( cs,n );

            int m = n - 1;
            while (!pq.empty()) {
                Pair p = pq.top();
                pq.pop();

                if (vd.hasKey( p.first )) {
                    typename Graph::PEdge e = g.getEdge( p.first,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,p.first );
                        if (vd.hasKey( u )) --vd[u];
                    } while (e = g.getEdgeNext( p.first,e,EdDirIn|EdDirOut|EdUndir ));

                    e = g.getEdge( p.first,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,p.first );
                        if (vd.hasKey( u )) pq.push( std::make_pair( u,vd[u] ) );
                    } while (e = g.getEdgeNext( p.first,e,EdDirIn|EdDirOut|EdUndir ));

                    cs[m--] = p.first;
                    vd.delKey( p.first );
                }
            }
            return greedy( g,c,cs,cs + n );
        }

        // Kolorowanie podanego zakresu wierzchołków, metoda Smallest Last.
        template < typename Graph, typename ColorMap, typename Iterator >
        static int sl(const Graph &g, ColorMap &c , Iterator b, Iterator e )
        {
            typedef std::pair< typename Graph::PVertex,int > Pair;
            Pair LOCALARRAY( vs,g.getVertNo() * (g.Delta() + 1) +2);

            AssocArray< typename Graph::PVertex,int > vd;
            int n = 0;

            for( Iterator i = b; i != e; ++i )
                if ((!c.hasKey( *i ) || c[*i] < 0) && !vd.hasKey( *i ))
                {
                    vd[*i] = g.deg( *i,EdDirIn|EdDirOut|EdUndir );
                    vs[n++] = std::make_pair( *i,vd[*i] );
                }

            PriQueueInterface< Pair*,SlCmp< Pair > > pq( vs,vs + n,g.getVertNo() * (g.Delta() + 1) );
            typename Graph::PVertex LOCALARRAY( cs,n );

            int m = n - 1;
            while (!pq.empty()) {
                Pair p = pq.top();
                pq.pop();

                if (vd.hasKey( p.first )) {
                    typename Graph::PEdge e = g.getEdge( p.first,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,p.first );
                        if (vd.hasKey( u )) --vd[u];
                    } while (e = g.getEdgeNext( p.first,e,EdDirIn|EdDirOut|EdUndir ));

                    e = g.getEdge( p.first,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,p.first );
                        if (vd.hasKey( u )) pq.push( std::make_pair( u,vd[u] ) );
                    } while (e = g.getEdgeNext( p.first,e,EdDirIn|EdDirOut|EdUndir ));

                    cs[m--] = p.first;
                    vd.delKey( p.first );
                }
            }
            return greedy( g,c,cs,cs + n );
        }

        // Kolorowanie całego grafu, metoda Saturation Largest First.
        template< typename Graph, typename ColorMap >
        static int slf(const Graph &g, ColorMap &c )
        {
            typedef std::pair< typename Graph::PVertex,std::pair< int,int > > Triple;

            int n = 0;
            Triple LOCALARRAY( vs,g.getVertNo() );

            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
                if (!c.hasKey( v ) || c[v] < 0)
                {
                    vs[n].first = v;
                    vs[n].second.first = satDeg( g,c,v );
                    vs[n].second.second = g.deg( vs[n++].first,EdDirIn|EdDirOut|EdUndir );
                }

            int c1 = -1;
            for( int i = 0; i < n; i++ )
            {
                sort( vs + i,vs + n,SlfCmp< Triple >() );

                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;

                for( int j = i; j < n; j++ )
                    if (g.getEdge( vs[i].first,vs[j].first ))
                        vs[j].second.first = satDeg( g,c,vs[j].first );
            }
            return c1;
        }

        // Kolorowanie podanego zakresu wierzchołków, metoda Saturation Largest First.
        template < typename Graph, typename ColorMap, typename Iterator >
        static int slf(const Graph &g, ColorMap &c , Iterator b, Iterator e )
        {
            typedef std::pair< typename Graph::PVertex,std::pair< int,int > > Triple;

            int n = 0, m = 0;
            for( Iterator i = b; i != e; ++i ) n++;
            Triple LOCALARRAY( vs,n );

            for( ; b != e;  b++ )
                if (!c.hasKey( *b ) || c[*b] < 0)
                {
                    vs[n].first = *b;
                    vs[n].second.first = satDeg( g,c,*b );
                    vs[n].second.second = g.deg( vs[n++].first,EdDirIn|EdDirOut|EdUndir );
                }

            sort( vs,vs + m,SlfCmp< Triple >() );
            m = unique( vs,vs + n ) - vs;

            int c1 = -1;
            for( int i = 0; i < m; i++ )
            {
                sort( vs + i,vs + m,SlfCmp< Triple >() );

                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;

                for( int j = i; j < m; j++ )
                    if (g.getEdge( vs[i].first,vs[j].first ))
                        vs[j].second.first = satDeg( g,c,vs[j].first );
            }
            return c1;
        }

        protected:
            struct Stala {
                int val;
                Stala(int arg) : val(arg) {}
                template <typename T>
                    int operator() (T) { return val; }
            };

        public: //TODO:: protected - do uzytku wewnetrznego

        template < typename Graph, typename ColorMap, typename CompMap >
        static int interchangeComponents(const Graph &g, ColorMap &c, typename Graph::PVertex v, CompMap& map, int c1,int c2)
        {   assert(v && !c.hasKey(v));
            int l=0;
            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && (c[u] == c1 || c[u] == c2) && !map.hasKey(u))
                {
                    BFS::scanAttainable( makeSubgraph(const_cast<Graph&>(g),std::make_pair(assocChoose(c,c1)||assocChoose(c,c2),stdChoose(true))),
                                   u,assocInserter(map,Stala(l)),EdDirIn|EdDirOut|EdUndir);
                    l++;
                }
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir )) ;

            return l;
        }


} ;

}

#endif
