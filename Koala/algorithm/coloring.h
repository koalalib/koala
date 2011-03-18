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
    public:

        // Kolorowanie pojedynczego wierzchołka.
        template < typename Graph, typename ColorMap >
        static int greedy( Graph &g, ColorMap &c, typename Graph::PVertex v ) {
            if (c.hasKey( v ) && c[v] >= 0) return -1;

            int c1 = 0, s = g.getEdgeNo( v,EdDirIn|EdDirOut|EdUndir ) + 1;
            bool LOCALARRAY( ns,s );
            for( int i = 0; i < s; i++ ) ns[i] = false;

            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && c[u] >= 0 && c[u] < s) ns[c[u]] = true;
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir )) ;

            while (ns[c1]) c1++;
            return c[v] = c1;
        }

        // Kolorowanie podanego zakresu wierzchołków.
        template < typename Graph, typename ColorMap, typename Iterator >
        static int greedy( Graph &g, ColorMap &c, Iterator b, Iterator e ) {
            int c1 = -1;
            while (b != e) {
                int c2 = greedy( g,c,*b++ );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        // Kolorowanie całego grafu.
        template < typename Graph, typename ColorMap >
        static int greedy( Graph &g, ColorMap &c ) {
            int c1 = -1;
            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) ) {
                int c2 = greedy( g,c,v );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        protected:

        template <typename Pair> struct LfCmp {
                bool operator() (  Pair a, Pair b )
                { return a.second > b.second ||  (a.second == b.second && a.first < b.first); }
            } ;

        public:

        // Kolorowanie całego grafu, metoda Largest First.
        template < typename Graph, typename ColorMap >
        static int lf( Graph &g, ColorMap &c ) {
            typedef std::pair< typename Graph::PVertex,int > Pair;

            int n = 0;
            Pair LOCALARRAY( vs,g.getVertNo() );

            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
                if (!c.hasKey( v ) || c[v] < 0) {
                    vs[n].first = v;
                    vs[n].second = g.deg( vs[n++].first,EdDirIn|EdDirOut|EdUndir );
                }

            sort( vs,vs + n,LfCmp<Pair>() );

            int c1 = -1;
            for( int i = 0; i < n; i++ ) {
                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        template < typename Graph, typename ColorMap, typename Iterator >
        static int lf( Graph &g, ColorMap &c , Iterator b, Iterator e) {
            typedef std::pair< typename Graph::PVertex,int > Pair;

            int n = 0;
            for(Iterator i=b;i!=e;++i) n++;
            Pair LOCALARRAY( vs,n );
            n=0;

            for(; b!=e;  b++ )
                if (!c.hasKey( *b ) || c[*b] < 0) {
                    vs[n].first = *b;
                    vs[n].second = g.deg( vs[n++].first,EdDirIn|EdDirOut|EdUndir );
                }

            sort( vs,vs + n, LfCmp<Pair>() ); n=unique(vs,vs+n)-vs;

            int c1 = -1;
            for( int i = 0; i < n; i++ ) {
                int c2 = greedy( g,c,vs[i].first );
                if (c2 > c1) c1 = c2;
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
        static int interchangeComponents(Graph &g, ColorMap &c, typename Graph::PVertex v, CompMap& map, int c1,int c2)
        {   assert(v && !c.hasKey(v));
            int l=0;
            typename Graph::PEdge e = g.getEdge( v,EdDirIn|EdDirOut|EdUndir );
            do {
                typename Graph::PVertex u = g.getEdgeEnd( e,v );
                if (c.hasKey( u ) && (c[u] == c1 || c[u] == c2) && !map.hasKey(u))
                {
                    BFS::scanAttainable( makeSubgraph(g,std::make_pair(assocChoose(c,c1)||assocChoose(c,c2),stdChoose(true))),
                                   blackHole(),assocInserter(map,Stala(l)),u,EdDirIn|EdDirOut|EdUndir);
                    l++;
                }
            } while (e = g.getEdgeNext( v,e,EdDirIn|EdDirOut|EdUndir )) ;

            return l;
        }


} ;

}

#endif
