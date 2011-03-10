#ifndef __COLORING__H__
#define __COLORING__H__

#include "../base/def_struct.h"

namespace Koala {

/* ------------------------------------------------------------------------- *
    GreedyColoring

        Zachłanne wierzchołkowe kolorowanie grafów, realizowane zgodnie z
   kolejnością, w jakiej wierzchołki są przechowywane w grafie.
 * ------------------------------------------------------------------------- */

class SeqVertColoring {
    public:
        template < typename Graph, typename ColorMap >
        static int greedy( Graph &g, ColorMap &c, typename Graph::PVertex v ) {
            if (c.hasKey( v ) && c[v] >= 0) return -1;

            int s;
            bool LOCALARRAY(ns,(s=g.getEdgeNo( v ,EdDirIn|EdDirOut|EdUndir)+1));
            for(int i=0;i<s;i++) ns[i]=false;

            for(typename Graph::PEdge E=g.getEdge(v,EdDirIn|EdDirOut|EdUndir);
                E;E=g.getEdgeNext(v,E,EdDirIn|EdDirOut|EdUndir))
            {   typename Graph::PVertex u=g.getEdgeEnd(E,v);
                if (c.hasKey(u) && c[u]>=0 && c[u]<s) ns[c[u]]=true;
            }

            int c1;
            for(c1=0;ns[c1];c1++);
            c[v] = c1;
            return c1;
        }

        template < typename Graph, typename ColorMap, typename Iterator >
        static int greedy( Graph &g, ColorMap &c, Iterator b, Iterator e ) {
            int c1 = -1;
            while (b != e) {
                int c2 = greedy( g,c,*b++ );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }

        template < typename Graph, typename ColorMap>
        static int greedy( Graph &g, ColorMap &c ) {
            int c1 = -1;
            for( typename Graph::PVertex v = g.getVert(); v; v = g.getVertNext( v ) ) {
                int c2 = greedy( g,c,v );
                if (c2 > c1) c1 = c2;
            }
            return c1;
        }
} ;

// TODO
// class LFColoring: public GreedyColoring {
//    } ;

//    class SLColoring: public GreedyColoring {
//    } ;

//    class SLFColoring: public GreedyColoring {
//    }

}

#endif
