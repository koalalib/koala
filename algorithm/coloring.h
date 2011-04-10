#ifndef __COLORING__H__
#define __COLORING__H__

#include "../base/def_struct.h"
#include "../graph/subgraph.h"
#include "../container/set.h"
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
                return a.second > b.second || (a.second == b.second &&
                    a.first < b.first);
            }
        } ;

        template< typename Pair > struct SlCmp
        {
            bool operator() ( Pair a, Pair b )
            {
                return a.second < b.second || (a.second == b.second &&
                    a.first > b.first);
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
        static int satDeg( const Graph &g, ColorMap &c, typename Graph::PVertex v )
        {
            int sd = 0, s = g.getVertNo(), cs = 0;
            int LOCALARRAY( ns,s );

            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do
            {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && c[u] >= 0) ns[cs++] = c[u];
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir ));

            std::make_heap( ns,ns + cs );
            std::sort_heap( ns,ns + cs );
            return std::unique( ns,ns + cs ) - ns;
        }

        struct Const
        {
            int val;

            Const( int arg ): val( arg ) {}
            template< typename T > int operator()( T ) { return val; }
        } ;

    public: //TODO: tylko na potrzeby testu

        // Składowe spójności podgrafu pokolorowanego podanymi kolorami, mające
        // wierzchołki w sąsiedztwie v.
        template< typename Graph, typename ColorMap, typename CompMap >
        static int interchangeComponents(
            const Graph &g, ColorMap &c, typename Graph::PVertex v, CompMap &map,
            int c1, int c2 )
        {
            assert( v && !c.hasKey( v ) );

            int l = 0;
            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do
            {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && (c[u] == c1 || c[u] == c2) && !map.hasKey( u ))
                {
                    //FIXME
                    // Tu wstawić poprawioną wersję poniższego polecenia:

                    // BFS::scanAttainable( makeSubgraph( g,std::make_pair(
                    // assocChoose( c,c1 ) || assocChoose( c,c2 ), stdChoose( true ) ) ),
                    // blackHole(),assocInserter( map,Const( l ) ),u,EdDirIn|EdDirOut|EdUndir );
                    BFS::scanAttainable( makeSubgraph(const_cast<Graph&>(g),std::make_pair(assocChoose(c,c1)||assocChoose(c,c2),stdChoose(true))),
                                   u,assocInserter(map,Const(l)),EdDirIn|EdDirOut|EdUndir);

                    l++;
                }
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir )) ;
            return l;
        }

        protected:

        // Zamiana kolorów wokół wierzchołka w celu zwolnienia koloru.
        template< typename Graph, typename ColorMap >
        static int colorInterchange(
            const Graph &g, ColorMap &c, typename Graph::PVertex v )
        {
            int oc = -1;
            if (c.hasKey( v )) {
                oc = c[v];
                c.delKey( v );
            }

            int s = g.getEdgeNo( v,EdDirIn|EdDirOut|EdUndir ) + 1, i = 0;
            int LOCALARRAY( vc,s );
            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do
            {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && c[u] > 0) vc[i++] = c[u];
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir )) ;

            std::make_heap( vc,vc + i );
            std::sort_heap( vc,vc + i );
            i = std::unique( vc,vc + i ) - vc;

            for( int c1 = 0; c1 < i; c1++ )
                for( int c2 = c1 + 1; c2 < i; c2++ )
                {
                    typename Koala::AssocArray< typename Graph::PVertex,int > m;
                    int cn = interchangeComponents( g,c,v,m,c1,c2 );

                    bool LOCALARRAY( mc,cn );
                    bool found = false;
                    for( int j = 0; j < cn; j++ ) mc[j] = false;

                    e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,v );
                        if (m.hasKey( u ) && c[u] == c1) mc[m[u]] = true;
                    } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir )) ;

                    e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,v );
                        if (m.hasKey( u ) && c[u] == c2 && mc[m[u]])
                        {
                            found = true;
                            break;
                        }
                    } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir )) ;

                    if (!found) {
                        typename Graph::PVertex u = m.firstKey();

                        for( ; u; u = m.nextKey( u ) )
                            if (mc[m[u]]) c[u] = (c[u] == c1) ? c2 : c1;

                        return c[v] = c1;
                    }
                }

            return c[v] = oc;
        }

        // Maksymalny wykorzystany kolor.
        template< typename Graph, typename ColorMap >
        static int maxColor( const Graph &g, ColorMap &c ) {
            typename ColorMap::KeyType k = c.firstKey();

            int c1 = -1;
            for( ; k; k = c.nextKey( k ) )
                if (c[k] > c1) c1 = c[k];
            return c1;
        }

    public:
        // Zachłanne kolorowanie pojedynczego wierzchołka.
        template< typename Graph, typename ColorMap >
        static int greedy( const Graph &g, ColorMap &c, typename Graph::PVertex v )
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

        // Zachłanne kolorowanie pojedynczego wierzchołka, z zamianą kolorów, jeśli
        // uzyskany kolor przekracza podany limit.
        template< typename Graph, typename ColorMap >
        static int greedyInter(
            const Graph &g, ColorMap &c, typename Graph::PVertex v, int l )
        {
            int c1 = greedy( g,c,v );
            return (c1 <= l) ? c1 : colorInterchange( g,c,v );
        }

        // Kolorowanie podanego zakresu wierzchołków.
        template< typename Graph, typename ColorMap, typename Iterator >
        static int greedy( const Graph &g, ColorMap &c, Iterator b, Iterator e )
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
        static int greedy( const Graph &g, ColorMap &c )
        {
            int c1 = -1;
            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
            {
                int c2 = greedy( g,c,v );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
        static int greedyWithInterchange( const Graph &g, ColorMap &c )
        {
            int c1 = -1, l = maxColor( g,c );
            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
            {
                int c2 = greedyInter( g,c,v,l );
                if (c2 > l) l = c2;
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu (z zamianą kolorów).
        template< typename Graph, typename ColorMap >
        static int greedyWithInterchange( const Graph &g, ColorMap &c, int l )
        {
            int c1 = -1;
            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
            {
                int c2 = greedyInter( g,c,v,l );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu, metoda Largest First.
        template< typename Graph, typename ColorMap >
        static int lf( const Graph &g, ColorMap &c )
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
            std::make_heap( vs,vs + n,LfCmp< Pair >() );
            std::sort_heap( vs,vs + n,LfCmp< Pair >() );

            int c1 = -1;
            for( int i = 0; i < n; i++ ) {
                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie podanego zakresu wierzchołków, metoda Largest First.
        template < typename Graph, typename ColorMap, typename Iterator >
        static int lf( const Graph &g, ColorMap &c , Iterator b, Iterator e )
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
            std::make_heap( vs,vs + m,LfCmp< Pair >() );
            std::sort_heap( vs,vs + m,LfCmp< Pair >() );
            m = std::unique( vs,vs + n ) - vs;

            int c1 = -1;
            for( int i = 0; i < m; i++ ) {
                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu, metoda Smallest Last.
        template< typename Graph, typename ColorMap >
        static int sl( const Graph &g, ColorMap &c )
        {
            typedef std::pair< typename Graph::PVertex,int > Pair;
            Pair LOCALARRAY( vs,g.getVertNo() * (g.Delta() + 1) );

            AssocArray< typename Graph::PVertex,int > vd;
            int n = 0;

            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
                if (!c.hasKey( v ) || c[v] < 0)
                {
                    vd[v] = g.deg( v,EdDirIn|EdDirOut|EdUndir );
                    vs[n++] = std::make_pair( v,vd[v] );
                }

            PriQueueInterface< Pair *,SlCmp< Pair > >
                pq( vs,vs + n,g.getVertNo() * (g.Delta() + 1) );
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
        static int sl( const Graph &g, ColorMap &c , Iterator b, Iterator e )
        {
            typedef std::pair< typename Graph::PVertex,int > Pair;
            Pair LOCALARRAY( vs,g.getVertNo() * (g.Delta() + 1) );

            AssocArray< typename Graph::PVertex,int > vd;
            int n = 0;

            for( Iterator i = b; i != e; ++i )
                if ((!c.hasKey( *i ) || c[*i] < 0) && !vd.hasKey( *i ))
                {
                    vd[*i] = g.deg( *i,EdDirIn|EdDirOut|EdUndir );
                    vs[n++] = std::make_pair( *i,vd[*i] );
                }

            PriQueueInterface< Pair *,SlCmp< Pair > >
                pq( vs,vs + n,g.getVertNo() * (g.Delta() + 1) );
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
        static int slf( const Graph &g, ColorMap &c )
        {
            typedef std::pair< typename Graph::PVertex, std::pair< int,int > > Triple;

            int n = 0;
            Triple LOCALARRAY( vs,g.getVertNo() * (g.Delta() + 1) );
            AssocArray< typename Graph::PVertex,Set< int > > sd;

            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
                if (!c.hasKey( v ) || c[v] < 0)
                {
                    Set< int > sc;

                    typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,v );
                        if (c.hasKey( u ) && c[u] >= 0) sc += c[u];
                    } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir ));

                    sd[v] = sc;
                    vs[n].first = v;
                    vs[n].second.first = sc.size();
                    vs[n].second.second = g.deg( vs[n++].first,EdDirIn|EdDirOut|EdUndir );
                }

            PriQueueInterface< Triple *,SlfCmp< Triple > >
                pq( vs,vs + n,g.getVertNo() * (g.Delta() + 1) );

            int c1 = -1;
            while (!pq.empty())
            {
                Triple t = pq.top();
                pq.pop();

                if (!c.hasKey( t.first ) || c[t.first] < 0)
                {
                    int c2 = greedy( g,c,t.first );
                    if (c2 > c1) c1 = c2;

                    typename Graph::PEdge e = g.getEdge( t.first,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,t.first );
                        if (!sd[u].isElement( c2 ))
                        {
                            sd[u] += c2;
                            pq.push( std::make_pair( u,
                                std::make_pair( t.second.first + 1,t.second.second ) ) );
                        }

                    } while (e = g.getEdgeNext( t.first,e,EdDirIn|EdDirOut|EdUndir ));
                }
            }
            return c1;
        }

        // Kolorowanie podanego zakresu wierzchołków, metoda Saturation Largest First.
        template < typename Graph, typename ColorMap, typename Iterator >
        static int slf( const Graph &g, ColorMap &c , Iterator b, Iterator e )
        {
            typedef std::pair< typename Graph::PVertex, std::pair< int,int > > Triple;

            int n = 0;
            Triple LOCALARRAY( vs,g.getVertNo() * (g.Delta() + 1) );
            AssocArray< typename Graph::PVertex,Set< int > > sd;

            for( ; b != e;  b++ )
                if ((!c.hasKey( *b ) || c[*b] < 0) && !sd.hasKey( *b ))
                {
                    Set< int > sc;

                    typename Graph::PEdge e = g.getEdge( *b,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,*b );
                        if (c.hasKey( u ) && c[u] >= 0) sc += c[u];
                    } while (e = g.getEdgeNext( *b,e,EdDirIn|EdDirOut|EdUndir ));

                    sd[*b] = sc;
                    vs[n].first = *b;
                    vs[n].second.first = sc.size();
                    vs[n].second.second = g.deg( vs[n++].first,EdDirIn|EdDirOut|EdUndir );
                }

            PriQueueInterface< Triple *,SlfCmp< Triple > >
                pq( vs,vs + n,g.getVertNo() * (g.Delta() + 1) );

            int c1 = -1;
            while (!pq.empty())
            {
                Triple t = pq.top();
                pq.pop();

                if (!c.hasKey( t.first ) || c[t.first] < 0)
                {
                    int c2 = greedy( g,c,t.first );
                    if (c2 > c1) c1 = c2;

                    typename Graph::PEdge e = g.getEdge( t.first,EdDirIn|EdDirOut|EdUndir );
                    do
                    {
                        typename Graph::PVertex u = g.getEdgeEnd( e,t.first );
                        if (!sd[u].isElement( c2 ))
                        {
                            sd[u] += c2;
                            pq.push( std::make_pair( u,
                                std::make_pair( t.second.first + 1,t.second.second ) ) );
                        }

                    } while (e = g.getEdgeNext( t.first,e,EdDirIn|EdDirOut|EdUndir ));
                }
            }
            return c1;
        }
} ;

}

#endif
