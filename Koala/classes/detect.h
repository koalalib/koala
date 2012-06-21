#ifndef KOALA_DETECTOR_H
#define KOALA_DETECTOR_H

#include "..\graph\graph.h"
#include "..\algorithm\search.h"
#include "..\container\joinsets.h"


namespace Koala
{


template <class DefaultStructs>
class IsItPar : public SearchStructs {

	public:

    template <class GraphType>
	static bool zero(const GraphType& g)
	{
	    return !g.getVertNo();
	}

    template <class GraphType>
	static bool undir(const GraphType& g,bool allowmulti=false)
	{   if (allowmulti) return g.getVertNo()>0 && g.getEdgeNo(EdDirIn|EdDirOut)==0; // undir
	    int i=0;            // simple
	    if (!g.getVertNo() || g.getEdgeNo(EdDirIn|EdDirOut|EdLoop)) return false;
	    std::pair<typename GraphType::PVertex,typename GraphType::PVertex>  LOCALARRAY(tabE,g.getEdgeNo());
	    for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
	    {
	        std::pair<typename GraphType::PVertex,typename GraphType::PVertex> pair=g.getEdgeEnds(e);
	        tabE[i++]=std::make_pair(std::min(pair.first,pair.second),std::max(pair.first,pair.second));
	    }
	    DefaultStructs::sort( tabE,tabE + g.getEdgeNo() );
	    for(i=1;i<g.getEdgeNo();i++) if (tabE[i-1]==tabE[i]) return false;
	    return true;
	}

    template <class GraphType>
	static bool connected(const GraphType& g, bool allowmulti=false)
	{
	    if (!undir(g,allowmulti)) return false;
	    return BFSPar<DefaultStructs>::scanAttainable(g,g.getVert(),blackHole)==g.getVertNo();
	}

	template <class GraphType>
	static bool empty(const GraphType& g)
	{
	    return g.getVertNo() && !g.getEdgeNo();
	}


	template <class GraphType>
	static bool clique(const GraphType& g)
	{
	    return (g.getEdgeNo()==g.getVertNo()*(g.getVertNo()-1)/2) && undir(g,false);
	}

	template <class GraphType>
	static bool cliques(const GraphType& g)
	{   if (!undir(g,false)) return false;
	    int LOCALARRAY(comptab,g.getVertNo()+1);
	    int e=0,comp=BFSPar<DefaultStructs>::getComponents(g,SearchStructs::compStore(comptab,blackHole),EdUndir);
        for(int i=1;i<=comp;i++) e+=(comptab[i]-comptab[i-1])*(comptab[i]-comptab[i-1]-1)/2;
	    return e==g.getEdgeNo();
	}

	template <class GraphType>
	static bool tree(const GraphType& g)
	{
	    return connected(g,true) && (g.getVertNo()-1==g.getEdgeNo());
	}

	template <class GraphType>
	static bool forest(const GraphType& g)
	{
	    return undir(g,true) && !BFSPar<DefaultStructs>::cyclNo(g);
	}

	template <class GraphType>
	static bool regular(const GraphType& g,bool allowmulti=false)
	{
	    if (!undir(g,allowmulti)) return false;
	    int deg=g.deg(g.getVert());
	    for(typename GraphType::PVertex v=g.getVertNext(g.getVert());v;v=g.getVertNext(v))
            if (g.deg(v)!=deg) return false;
        return true;
	}

    class Path {
        public:

        template <class GraphType>
        static std::pair<typename GraphType::PVertex,typename GraphType::PVertex> getEnds(const GraphType& g)
        {   std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
                    null=std::make_pair(typename GraphType::PVertex(0),typename GraphType::PVertex(0)),
                    res=null;
            if (!tree(g)) return null;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                switch (g.deg(v))
                {
                    case 0: return make_pair(v,v);
                    case 1: if (!res.first) res.first=v;
                            else    {
                                        if (!res.second) { res.second=v; return res; }
                                        else return null;
                                    }
                    case 2: break;
                    default : return null;
                }
        }
    };

    template <class GraphType>
	static bool path(const GraphType& g)
	{
	    return Path::getEnds(g).first;
	}

    class Caterpillar {
        public:

        template <class GraphType>
        static std::pair<typename GraphType::PVertex,typename GraphType::PVertex> getSpineEnds(const GraphType& g)
        {   std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
                    null=std::make_pair(typename GraphType::PVertex(0),typename GraphType::PVertex(0));
            if (!connected(g,false)) return null;
            return Path::getEnds(makeSubgraph(g,std::make_pair(
                                        notChoose(orChoose(vertDegChoose(0, EdUndir),vertDegChoose(1, EdUndir))),
                                        stdChoose(true))));
        }
    };

    template <class GraphType>
	static bool caterpillar(const GraphType& g)
	{
	    return Caterpillar::getSpineEnds(g).first;
	}

	template <class GraphType>
	static bool cycle(const GraphType& g,bool allowmulti=false)
	{
	    return connected(g,allowmulti) && g.deg(g.getVert())==2 && regular(g,true);
	}

	template <class GraphType>
	static bool matching(const GraphType& g)
	{
	    return undir(g,true) && g.Delta()<=1;
	}

	template <class GraphType>
	static bool subcubic(const GraphType& g,bool allowmulti=false)
	{
	    return undir(g,allowmulti) && g.Delta()<=3;
	}

	template <class GraphType>
	static bool cubic(const GraphType& g,bool allowmulti=false)
	{
	    return  g.getVert() && g.deg(g.getVert())==3 && regular(g,allowmulti);
	}

	template <class GraphType>
	static bool block(const GraphType& g)
	{   if (!undir(g,false)) return false;
	    int LOCALARRAY(comptab,g.getVertNo()+g.getEdgeNo()+1);
	    int e=0,comp=BlocksPar<DefaultStructs>::split(g,blackHole,blackHole,SearchStructs::compStore(comptab,blackHole),blackHole,EdUndir);
        for(int i=1;i<=comp;i++) e+=(comptab[i]-comptab[i-1])*(comptab[i]-comptab[i-1]-1)/2;
	    return e==g.getEdgeNo();
	}

	template <class GraphType>
	static int almostTree(const GraphType& g)
	{   if (!undir(g,false)) return -1;
	    int LOCALARRAY(comptab,g.getVertNo()+g.getEdgeNo()+1);
	    int LOCALARRAY(compE,g.getVertNo()+g.getEdgeNo()+1);
	    for(int i=0;i<g.getVertNo()+g.getEdgeNo()+1;i++) compE[i]=0;
	    typename DefaultStructs:: template AssocCont<
            typename GraphType::PEdge, int >::Type edgeCont(g.getEdgeNo());
	    int res=0,comp=BlocksPar<DefaultStructs>::split(g,blackHole,edgeCont,SearchStructs::compStore(comptab,blackHole),blackHole,EdUndir);
        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) compE[edgeCont[e]]++;
        for(int i=0;i<comp;i++) res=std::max(res,compE[i]-(comptab[i+1]-comptab[i]-1));
	    return res;
	}

    class Bipartite {
        public:

        template <class GraphType,class Iter>
        static int getPart(const GraphType& g,Iter out,bool allowmulti=false)
        {   if (!undir(g,allowmulti) || g.getEdgeNo(EdLoop)) return -1;
            typename DefaultStructs:: template AssocCont<
                    typename GraphType::PVertex, SearchStructs::VisitVertLabs<GraphType > >
                            ::Type vertCont(g.getVertNo());
            BFSPar<DefaultStructs>::scan(g,blackHole,EdUndir,vertCont);
            for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            {   std::pair<typename GraphType::PVertex,typename GraphType::PVertex> ends=g.getEdgeEnds(e);
                if ((vertCont[ends.first].distance &1)==(vertCont[ends.second].distance &1)) return -1;
            }
            int licz=0;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                if (vertCont[v].distance^1) { licz++; *out=v; ++out; }
            return licz;
        }
    };

    template <class GraphType>
	static bool bipartite(const GraphType& g,bool allowmulti=false)
	{
	    return Bipartite::getPart(g,blackHole,allowmulti)!=-1;
	}

    class CompBipartite {
        public:

        template <class GraphType,class Iter>
        static int getPart(const GraphType& g,Iter out)
        {   if (!undir(g,false) || g.getEdgeNo(EdLoop)) return -1;
            typename DefaultStructs:: template AssocCont<
                    typename GraphType::PVertex, SearchStructs::VisitVertLabs<GraphType > >
                            ::Type vertCont(g.getVertNo());
            BFSPar<DefaultStructs>::scan(g,blackHole,EdUndir,vertCont);
            for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            {   std::pair<typename GraphType::PVertex,typename GraphType::PVertex> ends=g.getEdgeEnds(e);
                if ((vertCont[ends.first].distance &1)==(vertCont[ends.second].distance &1)) return -1;
            }
            int licz=0;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                if (vertCont[v].distance^1) licz++;
            if (licz*(g.getVertNo()-licz)!=g.getEdgeNo(EdUndir)) return -1;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                if (vertCont[v].distance^1) { *out=v; ++out; };
            return licz;
        }
    };

    template <class GraphType>
	static bool compBipartite(const GraphType& g)
	{
	    return CompBipartite::getPart(g,blackHole)!=-1;
	}


    class CompMPartite {
        public:

        template <class GraphType, class Iter,class VIter>
        static int getParts(const GraphType& g,Iter iter,VIter viter)
        {
            if (!undir(g,false)) return -1;
            typename DefaultStructs:: template AssocCont<
                typename GraphType::PVertex, int >::Type colors(g.getVertNo());
            int LOCALARRAY(tabC,g.getVertNo());
            int i,licz=0,maxc=0;

            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            {   for(i=0;i<g.getVertNo();i++) tabC[i]=1;
                for(typename GraphType::PEdge e=g.getEdge(v,EdUndir);e;e=g.getEdgeNext(v,e,EdUndir))
                if (colors.hasKey(g.getEdgeEnd(e,v))) tabC[colors[g.getEdgeEnd(e,v)]]=0;
                for(i=0;!tabC[i];i++);
                maxc=std::max(maxc,colors[v]=i);
            }
            for(i=0;i<=maxc;i++) tabC[i]=0;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v)) tabC[colors[v]]++;
            for(i=1;i<=maxc;i++) for(int j=0;j<i;j++) licz+=tabC[i]*tabC[j];
            if (licz!=g.getEdgeNo(EdUndir)) return -1;

            licz=0;
            for(i=0;i<=maxc;i++) { *iter=licz; ++iter; licz+=tabC[i];  }
            *iter=g.getVertNo(); ++iter;

            for(i=0;i<=maxc;i++)
               for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v)) if (colors[v]==i)
               { *viter=v; ++viter; }
            return maxc+1;
        }
    };

    template <class GraphType>
	static bool compMPartite(const GraphType& g)
	{
	    return CompMPartite::getParts(g,blackHole,blackHole)!=-1;
	}

    class Chordal {
    protected:

        static void RadixSort(std::pair<int, int> *src, int size, int n,
                      int *hints, int *out) {
            int i, j;
            int LOCALARRAY(bucket, n);
            int LOCALARRAY(pos, n);
            int LOCALARRAY(next, size);

            for(i = 0; i < n; i++) bucket[i] = -1;

            // podzia³ na kube³ki wg. .second
            for(i = 0; i < size; i++) {
                j = src[i].second;
                next[i] = bucket[j];
                bucket[j] = i;
                };

            // policzenie pocz¹tku ka¿dego zbioru RN w tablicy out
            for(i = 0; i < n; i++) pos[i] = hints[i];

            // wpisanie kube³ków do tablicy
            for(i = 0; i < n; i++) {
                for(j = bucket[i]; j >= 0; j = next[j])
                    out[pos[src[j].first]++] = src[j].second;
                };
            };


        // porz¹dkuje wêz³y drzewa zdefiniowanego przez relacjê parent
        // wszyscy potomkowie p wyst¹pi¹ przed wyst¹pieniem p, ale
        // nie koniecznie bêdzie to kolejnoœæ wygenerowana przez DFSa na
        // drzewie, np. dla:
        //     D
        //    / \
        //   C   E
        //  / \
        // A   B
        // DFSPostorder da: A B C E D
        // a poni¿sza funkcja mo¿e zwróciæ np.: A E B C D
        static void SemiPostOrderTree(int *parent, int n, int *out) {
            int i, k;
            int LOCALARRAY(sons, n);
            for(i = 0; i < n; i++) sons[i] = 0;
            for(i = 0; i < n; i++) {
                if(parent[i] >= 0) sons[parent[i]]++;
                };
            for(i = 0; i < n; i++) {
                if(sons[i] != 0) continue;
                k = i;
                do {
                    *out = k;
                    ++out;
                    sons[k] = -1;
                    k = parent[k];
                    if(k >= 0) sons[k]--;
                    if(sons[k] != 0) break;
                } while(k >= 0);
                };
            };

        public:

        template<class Graph, class VIter1, class VIter2>
        static bool getOrder(const Graph &g,VIter1 oiter, VIter2 riter) {
            if (!undir(g,false)) return false;
            int i, m, n, p, ui, vi;
            int x, px, xp, pxp;
            bool fail;
            n = g.getVertNo();
            m = g.getEdgeNo(EdUndir);

            int LOCALARRAY(parent, n);
            int LOCALARRAY(postOrd, n);
            int LOCALARRAY(RNp, n + 1);
            int LOCALARRAY(RN2, n + m);
            typename Graph::PEdge e;
            typename Graph::PVertex u, v;
            typename Graph::PVertex LOCALARRAY(pi, n);
            std::pair<int, int> LOCALARRAY(RN, n + m);
            typename DefaultStructs::template AssocCont<typename Graph::PVertex, int>::Type vidx(n);

            LexBFSPar<DefaultStructs>::scan(g, pi);
            std::reverse(pi, pi + n);

            for(i = 0; i < n; i++) vidx[pi[i]] = i;

            // let RN(x) be its neighbors to the right
            for(ui = 0, p = 0; ui < n; ui++) {
                u = pi[ui];
                RNp[ui] = p;
                for(e = g.getEdge(u, EdUndir );
                    e != NULL;
                    e = g.getEdgeNext(u, e)) {
                    v = g.getEdgeEnd(e, u);
                    vi = vidx[v];
                    if(vi <= ui) continue;
                    RN[p++] = std::make_pair(ui, vi);
                    };
                };
            RNp[n] = p;

            RadixSort(RN, p, n, RNp, RN2);

            // let parent(x) be the leftmost member of RN(x) in pi
            for(i = 0; i < n; i++) {
                if(RNp[i] < RNp[i + 1]) parent[i] = RN2[RNp[i]];
                else parent[i] = -1;
                };

            fail = false;

            // let T be the the defined by the parent pointers
            SemiPostOrderTree(parent, n, postOrd);

            // for each vertex in T in postorder
            for(i = 0; i < n; i++) {
                x = postOrd[i];
                //check that (RN(x) \ parent(x)) sub RN(parent(x))
                xp = RNp[x] + 1;
                if(parent[x] < 0) continue;
                px = parent[x];
                pxp = RNp[px];

                for(; xp < RNp[x + 1] && pxp < RNp[px + 1];) {
                    if(RN2[xp] == RN2[pxp]) {	// match
                        xp++;
                        pxp++;
                        continue;
                    } else if(RN2[xp] > RN2[pxp]) {	// mismatch
                        pxp++;
                        continue;
                    } else {			// mismatch
                        fail = true;
                        break;
                        };
                    };
                if(xp < RNp[x + 1]) fail = true;

                if(fail) return false;
                };

            if (!isBlackHole(oiter))
                for(int i=0;i<g.getVertNo();i++) { *oiter=pi[i];++oiter;}
            if (!isBlackHole(riter))
                for(int i=g.getVertNo()-1;i>=0;i--) { *riter=pi[i];++riter;}

            return true;
        };

        template<class Graph, class VIter, class VIterOut, class QIter,class QTEIter>
        static int maxCliques(const Graph &g,VIter begin, VIter end, CompStore< QIter,VIterOut > out,QTEIter qte)
        {   int i,j,licze=0,res,no;
            VIter vi;
            Set<typename Graph::PVertex> LOCALARRAY(tabs,g.getVertNo());
            typename Graph::PVertex LOCALARRAY(vbuf,g.getVertNo());
            int LOCALARRAY(sno,g.getVertNo());
            typename DefaultStructs:: template AssocCont<
                typename Graph::PVertex, bool >::Type tabf(g.getVertNo());
            for(i=0,vi=begin;vi!=end;++vi,++i)
            {   tabs[i]=*vi;
                typename Graph::PVertex u;
                for(typename Graph::PEdge e=g.getEdge(*vi,EdUndir);e;e=g.getEdgeNext(*vi,e,EdUndir))
                    if (tabf.hasKey(u=g.getEdgeEnd(e,*vi))) tabs[i]+=u;
                tabf[*vi]=true;
            }
            for(i=0;i<g.getVertNo()-1;i++) for(j=i+1;j<g.getVertNo();j++)
                if (tabs[i].subsetOf(tabs[j]))
                    { tabs[i].clear(); licze++; break; }
            *out.compIter=0;++out.compIter;
            for(no=j=i=0,vi=begin;vi!=end;++vi,++i) if (!tabs[i].empty())
            {   tabs[i].getElements(vbuf); sno[no++]=i;
                for(int k=0;k<tabs[i].size();k++) { *out.vertIter=vbuf[k]; ++out.vertIter; }
                j+=tabs[i].size();*out.compIter=j;++out.compIter;
            }
            res=g.getVertNo()-licze;
            if (!isBlackHole(qte) && res>1)
            {   g.getVerts(vbuf);
                JoinableSets<typename Graph::PVertex, typename DefaultStructs::template AssocCont<typename Graph::PVertex,
                    JSPartDesrc<typename Graph::PVertex> *>::Type > sets;
                sets.clear(); sets.resize(g.getVertNo());
                for(typename Graph::PVertex v = g.getVert(); v ; v = g.getVertNext(v)) sets.makeSinglet(v);
                std::pair<int, std::pair<int,int> > LOCALARRAY(edges,res*(res-1)/2);
                int l=0;
                for(int i=0;i<res-1;i++) for(int j=i+1;j<res;j++)
                    edges[l++]=std::make_pair((tabs[sno[i]]*tabs[sno[j]]).size(),std::make_pair(i,j));
                DefaultStructs::sort(edges,edges+l);std::reverse(edges,edges+l);
                int edgeNo=res-1;
                for(int i = 0; edgeNo>0; i++)
                {   std::pair<int, int> ends=edges[i].second;
                    if(sets.getSetId(vbuf[sno[ends.first]]) != sets.getSetId(vbuf[sno[ends.second]]))
                    {
                        sets.join(vbuf[sno[ends.first]], vbuf[sno[ends.second]]);
                        *qte = ends; ++qte; edgeNo--;
                    };
                };

            }
            return res;
        }

        template<class Graph, class VIterOut, class QIter,class QTEIter>
        static int maxCliques(const Graph &g,CompStore< QIter,VIterOut > out, QTEIter qte)
        {   typename Graph::PVertex LOCALARRAY(vbuf,g.getVertNo());
            if (!getOrder(g,blackHole,vbuf)) return -1;
            return maxCliques(g,vbuf,vbuf+g.getVertNo(),out,qte);
        }


    };

    template <class GraphType>
	static bool chordal(const GraphType& g)
	{
	    return Chordal::getOrder(g,blackHole,blackHole);
	}

//    protected:
//
//	    class CoChordal : public Chordal {
//        public:
//
//        template<class Graph>
//        static bool CoChordalTest(const Graph &g) {
//            if (!undir(g,false)) return false;
//            int i, m, n, p, ui, vi;
//            int x, px, xp, pxp;
//            bool fail;
//            n = g.getVertNo();
//            m = g.getEdgeNo(EdUndir);
//
//            int LOCALARRAY(parent, n);
//            int LOCALARRAY(postOrd, n);
//            int LOCALARRAY(RNp, n + 1);
//            int LOCALARRAY(RN2, n + m);
//            typename Graph::PEdge e;
//            typename Graph::PVertex u, v;
//            typename Graph::PVertex LOCALARRAY(pi, n);
//            std::pair<int, int> LOCALARRAY(RN, n + m);
//            typename DefaultStructs::template AssocCont<typename Graph::PVertex, int>::Type vidx(n);
//
//            for(i = 0, v = g.getVert(); v != NULL; v = g.getVertNext(v))
//                pi[i++] = v;
//            Privates::CoLexBFS::order2(g, n, pi, EdUndir, pi);
//            std::reverse(pi, pi + n);
//
//            for(i = 0; i < n; i++) vidx[pi[i]] = i;
//
//            // let RN(x) be its neighbors to the right
//            for(ui = 0, p = 0; ui < n; ui++) {
//                u = pi[ui];
//                RNp[ui] = p;
//                for(e = g.getEdge(u, EdUndir );
//                    e != NULL;
//                    e = g.getEdgeNext(u, e)) {
//                    v = g.getEdgeEnd(e, u);
//                    vi = vidx[v];
//                    if(vi <= ui) continue;
//                    RN[p++] = std::make_pair(ui, vi);
//                    };
//                };
//            RNp[n] = p;
//
//            Chordal::RadixSort(RN, p, n, RNp, RN2);
//
//            // let parent(x) be the leftmost non-neighbour to the right in pi
//            for(i = 0; i < n; i++) {
//                if(RNp[i] < RNp[i + 1]) {
//                    if(RN2[RNp[i]] > i + 1) parent[i] = i + 1;
//                    else {
//                        int j = RNp[i];
//                        while(j < RNp[i + 1] - 1) {
//                            if(RN2[j] + 1 < RN2[j + 1]) {
//                                parent[i] = RN2[j] + 1;
//                                break;
//                                };
//                            j++;
//                            };
//                        if(j == RNp[i + 1] - 1)
//                            parent[i] = RN2[j] + 1;
//                        };
//                    if(parent[i] >= n) parent[i] = -1;
//                } else {
//                    if(i < n - 1) parent[i] = i + 1;
//                    else parent[i] = -1;
//                    };
//                };
//
//            fail = false;
//
//            // let T be the the defined by the parent pointers
//            Chordal::SemiPostOrderTree(parent, n, postOrd);
//
//            // for each vertex in T in postorder
//            for(i = 0; i < n; i++) {
//                x = postOrd[i];
//                //check that RN(parent(x)) sub RN(x)
//                xp = RNp[x];
//                if(parent[x] < 0) continue;
//                px = parent[x];
//                pxp = RNp[px];
//
//                for(; xp < RNp[x + 1] && pxp < RNp[px + 1];) {
//                    if(RN2[xp] == RN2[pxp]) {	// match
//                        xp++;
//                        pxp++;
//                        continue;
//                    } else if(RN2[xp] < RN2[pxp]) {	// mismatch
//                        xp++;
//                        continue;
//                    } else {			// mismatch
//                        fail = true;
//                        break;
//                        };
//                    };
//                if(pxp < RNp[parent[x] + 1]) fail = true;
//
//                if(fail) return false;
//                };
//
//            return true;
//            };
//
//	    };
//
//    public:
//
//    template <class GraphType>
//	static bool cochordal(const GraphType& g)
//	{
//	    return CoChordal::CoChordalTest(g);
//	}
//
//    template <class GraphType>
//	static bool split(const GraphType& g)
//	{
//	    return chordal(g)&& cochordal(g);
//	}


    class Comparability {

        protected:

            template<class Graph>
            class AdjStruct {
            public:
                struct Node {
                    int v;
                    int cls;
                    Privates::List_iterator<Node> inv;
                    Node() { v = 0; cls = 0; };
                    Node(int _v, int _c, Privates::List_iterator<Node> _i) {
                        v = _v;
                        cls = _c;
                        inv = _i;
                        };
                    bool operator <(const Node &b) { return v < b.v; };
                };

                typedef Privates::List_iterator<Node> Ptr;

                AdjStruct(size_t s,Privates::List<Node,Privates::ListBlockListAllocator<Node> >* adata): datasize(s),data(adata) {
                    };

                Ptr Begin(int idx)	{ return data[idx].begin(); };
                Ptr End(int idx)	{ return data[idx].end(); };


                template<class VertMap>
                void Init(const Graph &g, VertMap &vertexToIndex) {
                    int u, v;
                    typename Graph::PEdge e;
                    Privates::List_iterator<Node> a, b;
                    std::pair<typename Graph::PVertex, typename Graph::PVertex> p;
                    int CompUndefined = DefaultStructs::template NumberTypeBounds<int>::plusInfty();

                    for(e = g.getEdge(EdUndir); e != NULL; e = g.getEdgeNext(e, EdUndir)) {
                        p = g.getEdgeEnds(e);
                        u = vertexToIndex[p.first];
                        v = vertexToIndex[p.second];
                        data[u].push_front(Node());
                        data[v].push_front(Node());
                        a = data[u].begin();
                        b = data[v].begin();
                        a->v = v;
                        a->cls = CompUndefined;
                        a->inv = b;
                        b->v = u;
                        b->cls = CompUndefined;
                        b->inv = a;
                        };
                    for(int i = 0; i < datasize; i++) data[i].sort();
                };

        //		void Dump() {
        //			typename std::list<Node>::iterator it;
        //			for(int i = 0; i < datasize; i++) {
        //				printf("%d:", i);
        //				for(it = data[i].begin(); it != data[i].end(); ++it) {
        //					printf("  (%d,%d, (%d,%d))", it->v, it->cls, it->inv->v, it->inv->cls);
        //					};
        //				printf("\n");
        //				};
        //			};

        //		Table<list<Node, Allocator>, Allocator> data;
                Privates::List<Node,Privates::ListBlockListAllocator<Node> > *data;
                int datasize;
        };  //AdjStruct

        template<class Graph>
        struct CTState {
            AdjStruct<Graph> cls;
            bool flag;
            int k;
            int ud;
            CTState(size_t n,Privates::List<typename AdjStruct<Graph>::Node,Privates::ListBlockListAllocator<typename AdjStruct<Graph>::Node> > * acls)
                : cls(n,acls)		{};
        };

        template<class Graph>
        static void Explore(const Graph &g, int i, int j,
                    CTState<Graph> &state) {
            int m;
            typename AdjStruct<Graph>::Ptr ip, jp;
            typename AdjStruct<Graph>::Ptr ie, je;
            ip = state.cls.Begin(i);
            jp = state.cls.Begin(j);
            ie = state.cls.End(i);
            je = state.cls.End(j);
            // for each m \in ADJ(i) such that
            // 	[ m \notin ADJ(j) or |cls(j, m)| < k] do
            while(ip != ie) {
                m = ip->v;
                while(jp != je && jp->v < m) ++jp;
                if(jp != je) {
                    if(jp->v == m && abs(jp->cls) >= state.k) {
                        ++ip;
                        continue;
                        };
                    };
                // if CLASS(i, m) is undefined
                if(ip->cls == state.ud) {
                    ip->cls = state.k;
                    ip->inv->cls = -state.k;
                    Explore(g, i, m, state);
                } else if(ip->cls == -state.k) {
                    ip->cls = state.k;
                    state.flag = true;
                    Explore(g, i, m, state);
                    };
                ++ip;
                };

            ip = state.cls.Begin(i);
            jp = state.cls.Begin(j);
            ie = state.cls.End(i);
            je = state.cls.End(j);
            // for each m \in ADJ(v) such that
            // 	[ m \notin ADJ(u) or |cls(u, m)| < k] do
            while(jp != je) {
                m = jp->v;
                while(ip != ie && ip->v < m) ++ip;
                if(ip != ie) {
                    if(ip->v == m && abs(ip->cls) >= state.k) {
                        ++jp;
                        continue;
                        };
                    };
                // if CLASS(m, j) is undefined
                if(jp->inv->cls == state.ud) {
                    jp->inv->cls = state.k;
                    jp->cls = -state.k;
                    Explore(g, m, j, state);
                } else if(jp->inv->cls == -state.k) {
                    jp->inv->cls = state.k;
                    state.flag = true;
                    Explore(g, m, j, state);
                    };
                ++jp;
                };
        };

        template<class Graph,  class VMap, class UVMap>
        static void InitState(const Graph &g, CTState<Graph> &state, VMap &vidx, UVMap &idxv) {
            int i = 0, n = g.getVertNo();
            typename Graph::PVertex v;
            for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) {
                if (!isBlackHole(idxv)) idxv[i] = v;
                vidx[v] = i++;
                };
            state.flag = false;
            state.k = 0;
            state.cls.Init(g, vidx);
            state.ud = DefaultStructs::template NumberTypeBounds<int>::plusInfty();
        };

        template<class Graph>
        static bool ComparabilityTool(const Graph &g, CTState<Graph> &state) {
            int i, n;
            typename Graph::PEdge e;
            typename Graph::PVertex u, v;
            typename AdjStruct<Graph>::Ptr ip, ie;
            std::pair<typename Graph::PVertex, typename Graph::PVertex> p;

            n = g.getVertNo();

    //		state.cls.Dump();

            for(i = 0; i < n; i++) {
                ip = state.cls.Begin(i);
                ie = state.cls.End(i);
                for(; ip != ie; ++ip) {
                    if(ip->cls != state.ud) continue;
                    state.k++;
                    ip->cls = state.k;
                    ip->inv->cls = -state.k;
                    Explore<Graph>(g, i, ip->v, state);
                    };
                };
    //		state.cls.Dump();
            return !state.flag;
            };


        template<class Graph>
        static int Height(CTState<Graph> &state, int *height, int i) {
            int h, m = 0;
            typename AdjStruct<Graph>::Ptr ip, ie;
            if(height[i] != 0) return height[i];
            ip = state.cls.Begin(i);
            ie = state.cls.End(i);
            for(; ip != ie; ++ip) {
                if(ip->cls < 0) continue;
                h = height[ip->v];
                if(h == 0) h = Height(state, height, ip->v);
                if(h > m) m = h;
                };
            height[i] = m + 1;
            return m + 1;
            };

        template<class Graph, class VMap, class OutIter>
        static void GetClique(CTState<Graph> &state, int *height,
                      int i, VMap &idxv, OutIter &out) {
            int h;
            typename AdjStruct<Graph>::Ptr ip, ie;
            h = height[i] - 1;
            *out = idxv[i];
            out++;
            while(h > 0) {
                ip = state.cls.Begin(i);
                ie = state.cls.End(i);
                for(; ip != ie; ++ip) {
                    if(ip->cls < 0) continue;
                    if(height[ip->v] == h) {
                        i = ip->v;
                        *out = idxv[i];
                        ++out;
                        h--;
                        break;
                        };
                    };
                };
            };

        public:
            /* M.C. Golumbic
             * The Complexity of Comparability Graph Recognition and Coloring
             * Computing 18, 199-208 (1977)
             */

            template<class Graph,class DirMap, class OutMap, class OutIter>
            static int explore(const Graph &g, DirMap& adirmap,OutMap &aheightmap,OutIter cliqueiter)
            {
                if (!undir(g,false)) return -1;
                int b,i,m,h,n = g.getVertNo();
                if (n==1)
                {   if (!isBlackHole(aheightmap)) aheightmap[g.getVert()] = 0;
                    *cliqueiter = g.getVert(); ++cliqueiter;
                    //return True;
                    return 1;
                }

                typename DefaultStructs:: template AssocCont<typename Graph::PEdge,
                EdgeDirection >::Type localdirmap;
                typename BlackHoleSwitch<DirMap,typename DefaultStructs::template AssocCont<typename Graph::PEdge,
                EdgeDirection >::Type >::Type &
                    dirmap=
                BlackHoleSwitch<DirMap,typename DefaultStructs:: template AssocCont<typename Graph::PEdge,
                EdgeDirection >::Type >::get(adirmap,localdirmap);
                if (isBlackHole(adirmap)) dirmap.reserve(g.getEdgeNo());


                typename Graph::PVertex LOCALARRAY(idxv, n);
//                std::list<typename AdjStruct<Graph>::Node> LOCALARRAY(statebuf,n);
                Privates::ListBlockListAllocator<typename AdjStruct<Graph>::Node> pula(2*g.getEdgeNo());    // TODO: size?
                Privates::List<typename AdjStruct<Graph>::Node,Privates::ListBlockListAllocator<typename AdjStruct<Graph>::Node> >
                    LOCALARRAY(statebuf,n);

                for(int i=0;i<n;i++) statebuf[i].init(pula);
                CTState<Graph> state(n,statebuf);

                typename DefaultStructs::template AssocCont<typename Graph::PVertex, int>::Type vidx(g.getVertNo());

                InitState(g, state, vidx, idxv);

                if (!ComparabilityTool(g, state)) return -1;
                int LOCALARRAY(height, n);
                typename AdjStruct<Graph>::Ptr ip, ie;

                for(i = 0; i < n; i++) height[i] = 0;
                for(m = 0, i = 0; i < n; i++) {
                    h = Height(state, height, i);
                    if(h > m) { m = h; b = i; };
                };

                typename DefaultStructs:: template AssocCont<typename Graph::PVertex,
                int >::Type localheightmap;
                typename BlackHoleSwitch<OutMap,typename DefaultStructs::template AssocCont<typename Graph::PVertex,
                int >::Type >::Type &
                    heightmap=
                BlackHoleSwitch<OutMap,typename DefaultStructs:: template AssocCont<typename Graph::PVertex,
                int >::Type >::get(aheightmap,localheightmap);
                if (isBlackHole(aheightmap)) heightmap.reserve(g.getVertNo());


                for(i = 0; i < n; i++) heightmap[idxv[i]] = height[i] - 1;
                for(typename Graph::PEdge e=g.getEdge(EdUndir);e;e=g.getEdgeNext(e))
                {
                    assert(heightmap[g.getEdgeEnds(e).first]!=heightmap[g.getEdgeEnds(e).second]);
                    if (heightmap[g.getEdgeEnds(e).first]<heightmap[g.getEdgeEnds(e).second]) dirmap[e]=EdDirIn;
                    else dirmap[e]=EdDirOut;
                }
                if (!isBlackHole(cliqueiter)) GetClique(state, height, b, idxv, cliqueiter);
                return m;
            };

            template<class Graph,class DirMap>
            static bool getDirs(const Graph &g, DirMap& adirmap)
            {   return explore(g,adirmap,blackHole,blackHole)!=-1; }

            template<class Graph>
            static bool getDirs(Graph &g)
            {   int m=g.getEdgeNo();
                typename Graph::PEdge LOCALARRAY(tab,m);
                typename DefaultStructs:: template AssocCont<typename Graph::PEdge,
                EdgeDirection >::Type dir(m);
                if (explore(g,dir,blackHole,blackHole)==-1) return false;
                g.getEdges(tab);
                for(int i=0;i<m;i++)
                {   g.chEdgeType(tab[i],Directed);
                    if (dir[tab[i]]==EdDirIn) g.revertEdge(tab[i]);
                }
                return true;
            }

            template<class Graph,class OutMap>
            static int color(const Graph &g, OutMap& avmap)
            {   return explore(g,blackHole,avmap,blackHole); }

            template<class Graph,class OutIter>
            static int maxClique(const Graph &g, OutIter iter)
            {   return explore(g,blackHole,blackHole,iter); }

    };

    template <class GraphType>
	static bool comparability(const GraphType& g)
	{
	    return Comparability::explore(g,blackHole,blackHole,blackHole)!=-1;;
	}

    class Interval {

        public:

            struct Segment {
                int left, right;
                Segment(int l=0,int r=1) : left(l), right(r) { /*assert(l<r);*/ }
            };

            static bool touch(Segment a, Segment b)
            {   return std::max(a.left,b.left)<= std::min(a.right,b.right); }

        template<class GraphType,class Iter,class IterOut, class VInfoGen, class EInfoGen>
        static void segs2graph(GraphType &g,Iter begin, Iter end,IterOut out,VInfoGen vinfo, EInfoGen einfo)
        {   int licz=0,i=0,j=0;
            Iter it;
            for(it=begin;it!=end;++it)
            {   assert((*it).left<(*it).right); licz++;    }
            typename GraphType::PVertex LOCALARRAY(tabv,licz);
            for(it=begin;it!=end;++it)
                tabv[i++]=g.addVert(vinfo(i));
            for(i=0, it=begin;it!=end;++it,++i)
            {   Iter it2=it;it2++;j=i+1;
                for(;it2!=end;++it2,j++) if (touch(*it,*it2)) g.addEdge(tabv[i],tabv[j],einfo(i,j),EdUndir);
            }
            for(i=0;i<licz;i++)
            { *out=tabv[i]; ++out;  }
        }

        template<class GraphType,class Iter,class IterOut>
        static void segs2graph(GraphType &g,Iter begin, Iter end,IterOut out)
        {  segs2graph(g,begin,end,out,
                            ConstFunctor<typename GraphType::VertInfoType>(),
                            ConstFunctor<typename GraphType::EdgeInfoType>());
        }


        template<class GraphType,class IntMap>
        static bool graph2segs(const GraphType &g,IntMap& outmap) {
            if (!undir(g,false)) return false;
            unsigned int i, n;
            n = g.getVertNo();


            typename GraphType::PVertex LOCALARRAY(sigma, n);
            typename GraphType::PVertex LOCALARRAY(sigmap, n);
            typename GraphType::PVertex LOCALARRAY(sigmapp, n);
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex, IvData>::Type data(g.getVertNo());


            Privates::ListBlockListAllocator<Privates::List_iterator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > > allocat(2*g.getVertNo()+2); //TODO: size?
            Privates::ListBlockListAllocator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > allocat2(2*g.getVertNo()+2); //TODO: size?
            Privates::ListBlockListAllocator<typename Sets::Elem> allocat3(2*g.getVertNo()*g.getVertNo()); //TODO: size?


//            ListDefaultCPPAllocator allocat3;
            std::pair<typename Sets::Entry,
                typename Sets::Entry::iterator> LOCALARRAY(Abuf,g.getVertNo());
            std::pair<typename Sets::Entry,
                typename Sets::Entry::iterator> LOCALARRAY(Bbuf,g.getVertNo());
            for(int i=0;i<g.getVertNo();i++)
                { Abuf[i].first.init(allocat3); Bbuf[i].first.init(allocat3);}
            Sets A(Abuf,g.getVertNo(),allocat3),B(Bbuf,g.getVertNo(),allocat3);

    //		A.resize(g.getVertNo(), allocator);
    //		B.resize(g.getVertNo(), allocator);

            LexBFSPar<DefaultStructs>::scan(g, sigmap);

            reverse(sigmap, n);
            LexBFSPar<DefaultStructs>::order2(g, n, sigmap, EdUndir, sigmapp);

            reverse(sigmapp, n);
            LexBFSPar<DefaultStructs>::order2(g, n, sigmapp, EdUndir, sigma);

            reverse(sigma, n);
            LexBFSPar<DefaultStructs>::order2(g, n, sigma, EdUndir, sigmap);

            CalculateI(g, sigmap, data, &IvData::posSigmap, &IvData::ip);
            BuildSet(g, A, data, &IvData::posSigmap, &IvData::ip);

            reverse(sigmap, n);
            LexBFSPar<DefaultStructs>::order2(g, n, sigmap, EdUndir, sigmapp);

            CalculateI(g, sigmapp, data, &IvData::posSigmapp, &IvData::ipp);
            BuildSet(g, B, data, &IvData::posSigmapp, &IvData::ipp);

            reverse(sigmapp, n);
            LBFSStar(g, A, B, data, sigmap, sigmapp, sigma,allocat,allocat2);

            for(i = 0; i < n; i++) data[sigma[i]].posSigma = i;

            if(IsUmbrellaFree(g, data, sigma)) {
                if(!isBlackHole(outmap)) {
		    CalculateI(g, sigma, data, &IvData::posSigma, &IvData::ip);
                    for(int i=0;i<g.getVertNo();i++) { outmap[sigma[i]] = Segment(i, data[sigma[i]].ip); }
		    };
                return true;
	    	};
            return false;
            };

        protected:

//            template<class Allocator>
            struct Sets {
//            private:
//                template<template<class, class> class Cont, class Alloc>
                struct Elem {
                    int value;
                    Privates::List<Elem, Privates::ListBlockListAllocator<Elem> > *cont;
                    Privates::List_iterator<Elem> next;
                    Elem(int _v = -1): value(_v), next(), cont(NULL) {};
                    };
//                typedef TElem<List, Allocator> Elem;
//                typedef List<Elem, ListDefaultCPPAllocator> Entry;
                typedef Privates::List<Elem, Privates::ListBlockListAllocator<Elem> > Entry;

//            public:
                Sets(std::pair<Entry, Privates::List_iterator<Elem> >* data,size_t n,Privates::ListBlockListAllocator<Elem> &a): m_data(data)
                {
                    Entry e(a);
            //		m_data.resize(n, std::make_pair(e, e.end()));
                    for(size_t i = 0; i < n; i++) {
                        m_data[i]=std::make_pair(e, e.end());
                        m_data[i].second = m_data[i].first.end();
                        };
                };

                // ustawia n
            //	void resize(size_t n, Allocator &a) {
            //		Entry e(a);
            ////		m_data.resize(n, std::make_pair(e, e.end()));
            //		for(size_t i = 0; i < n; i++) {
            //		    m_data[i]=std::make_pair(e, e.end());
            //			m_data[i].second = m_data[i].first.end();
            //			};
            //		};

                // dodaje trg do zbioru id
                void add(int id, int trg) {
                    m_data[id].first.push_back(Elem(trg));
                    m_data[id].first.back().cont = &(m_data[id].first);
                    m_data[id].first.back().next = m_data[trg].second;
                    m_data[trg].second = m_data[id].first.end().prev();
                    };

                // usuwa id ze wszystkich zbiorów
                void remove(int id) {
                    Privates::List_iterator<Elem> it, t;
                    for(it = m_data[id].second; it != m_data[id].first.end(); ) {
                        t = it;
                        it = it->next;
                        t->cont->erase(t);
                        };
                    };

                // iloœæ elementów w zbiorze id
                int count(int id) {
                    return m_data[id].first.size();
                    };

                // czy zbiór id pusty
                bool empty(int id) {
                    return m_data[id].first.empty();
                    };

                // pierwszy element ze zbioru id (w praktyce -- dowolny)
                int first(int id) {
                    return m_data[id].first.front().value;
                    };

            //	void dump() {
            //		for(int i = 0; i < m_data.size(); i++) {
            //			typename Entry::iterator it;
            //			printf("%d:", i);
            //			for(it = m_data[i].first.begin(); it != m_data[i].first.end(); ++it) {
            //				printf(" %d", it->value);
            //				};
            //			printf("\n");
            //			};
            //		};

//            private:
                // para: lista dla v + wskaŸnik na g³owê v na innych listach
            //	Table<std::pair<Entry, typename Entry::iterator> > m_data;
                    std::pair<Entry, Privates::List_iterator<Elem> >* m_data;
            }; //Sets


            struct IvData {
            unsigned int posSigmap;
            unsigned int ip;
            unsigned int posSigmapp;
            unsigned int ipp;
            unsigned int posSigma;
            };


            template<class GraphType, class Map>
            static void CalculateI(const GraphType &g,
                           typename GraphType::PVertex *order,
                           Map &data,
                           unsigned int IvData::*pos,
                           unsigned int IvData::*ifn) {
                int i, n, pu, pv;
                typename GraphType::PEdge e;
                typename GraphType::PVertex u, v;
                n = g.getVertNo();
                for(i = 0; i < n; i++) data[order[i]].*pos = i;
                for(i = 0; i < n; i++) {
                    u = order[i];
                    pv = pu = i;
                    for(e = g.getEdge(u, EdUndir); e != NULL; e = g.getEdgeNext(u, e, EdUndir)) {
                        v = g.getEdgeEnd(e, u);
                        if(data[v].*pos > pv) pv = data[v].*pos;
                        };
                    data[u].*ifn = pv;
                    };
                };


            // BuildSet
            template<class GraphType, class Map, class SetsType>
            static void BuildSet(const GraphType &g,
                         SetsType &sets,
                         Map &data,
                         unsigned int IvData::*order,
                         unsigned int IvData::*ifn) {
                unsigned int i, n, vord, zord;
                typename GraphType::PEdge e;
                typename GraphType::PVertex z, v;
                n = g.getVertNo();

                for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) {
                    vord = data[v].*order;
                    for(e = g.getEdge(v, EdUndir); e != NULL; e = g.getEdgeNext(v, e, EdUndir)) {
                        z = g.getEdgeEnd(e, v);
                        zord = data[z].*order;
                        if(zord < vord && data[z].*ifn > vord) {
                            sets.add(vord, zord);
                            };
                        };
                    };
                };


            template<class GraphType>
            struct OrderData {
                typename GraphType::PVertex v;
                int vertId;	// kogo jest s¹siadem (numer s¹siada w porz¹dku)
                int orderId;	// numer w porz¹dku
                };

            struct LBSData {
                bool visiteda;
                bool visitedb;
                int aOrder;
                int bOrder;
                };

            // LBFSStar
            template<class GraphType, class MapType, class SetsType, class OutIter, class Alloc1, class Alloc2>
            static void LBFSStar(const GraphType &g,
                         SetsType &A,
                         SetsType &B,
                         MapType &data,
                         typename GraphType::PVertex *sigmap,
                         typename GraphType::PVertex *sigmapp,
                         OutIter out,
                         Alloc1& allocat,Alloc2& allocat2) {
                int i, j, o;
                unsigned int n, m, aidx, bidx;
                typename GraphType::PEdge e;
                typename GraphType::PVertex u, v, av, bv;

                n = g.getVertNo();
                if(n == 0) return;

                m = g.getEdgeNo(EdUndir);
                int LOCALARRAY(firsta, n + 1);
                int LOCALARRAY(firstb, n + 1);
                OrderData<GraphType> LOCALARRAY(neigha, m * 2);
                OrderData<GraphType> LOCALARRAY(neighb, m * 2);
                OrderData<GraphType> LOCALARRAY(neigh2, m * 2);


                //ListBlockListAllocator<List_iterator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > > allocat(2*g.getVertNo()+2);
                //ListBlockListAllocator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > allocat2(2*g.getVertNo()+2);
                typename LexBFSPar<DefaultStructs>::template
                    LexVisitContainer<GraphType,
                        Privates::ListBlockListAllocator<Privates::List_iterator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > >,
                        Privates::ListBlockListAllocator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > >
                alpha(allocat,allocat2,g.getVertNo()),beta(allocat,allocat2,g.getVertNo());

//                typename LexBFSPar<DefaultStructs>::template LexVisitContainer<GraphType, ListDefaultCPPAllocator,ListDefaultCPPAllocator> alpha(pula,pula);
//                typename LexBFSPar<DefaultStructs>::template LexVisitContainer<GraphType, ListDefaultCPPAllocator,ListDefaultCPPAllocator> beta(pula,pula);
                typename DefaultStructs::template AssocCont<typename GraphType::PVertex, LBSData>::Type vertData(g.getVertNo());

                for(i = 0; i < n; i++) {
                    vertData[sigmap[i]].aOrder = i;
                    vertData[sigmapp[i]].bOrder = i;
                    };

                i = j = 0;
                for(o = 0; o < n; o++) {
                    u = sigmap[o];
                    firsta[i] = j;
                    for(e = g.getEdge(u, EdUndir); e != NULL; e = g.getEdgeNext(u, e, EdUndir)) {
                        v = g.getEdgeEnd(e, u);
                        neigha[j].v = v;
                        neigha[j].orderId = vertData[v].aOrder;
                        neigha[j].vertId = o;
                        j++;
                        };
                    i++;
                    };
                firsta[i] = j;

                LexBFSPar<DefaultStructs>::StableRadixSort(neigha, j, n, &OrderData<GraphType>::orderId, neigh2);
                LexBFSPar<DefaultStructs>::StableRadixSort(neigh2, j, n, &OrderData<GraphType>::vertId, neigha);

                i = j = 0;
                for(o = 0; o < n; o++) {
                    u = sigmapp[o];
                    firstb[i] = j;
                    for(e = g.getEdge(u, EdUndir); e != NULL; e = g.getEdgeNext(u, e, EdUndir)) {
                        v = g.getEdgeEnd(e, u);
                        neighb[j].v = v;
                        neighb[j].orderId = vertData[v].bOrder;
                        neighb[j].vertId = o;
                        j++;
                        };
                    i++;
                    };
                firstb[i] = j;

                LexBFSPar<DefaultStructs>::StableRadixSort(neighb, j, n, &OrderData<GraphType>::orderId, neigh2);
                LexBFSPar<DefaultStructs>::StableRadixSort(neigh2, j, n, &OrderData<GraphType>::vertId, neighb);

                alpha.initialize(g, n, sigmap);
                beta.initialize(g, n, sigmapp);

                while(!alpha.empty()) {
                    av = alpha.top();
                    bv = beta.top();

                    aidx = data[av].posSigmap;
                    bidx = data[bv].posSigmapp;
                    if(data[av].ip > aidx) { u = bv; }
                    else if(data[bv].ipp > bidx) { u = av; }
                    else if(B.empty(bidx) || !A.empty(aidx)) { u = bv; }
                    else {
                        v = sigmapp[B.first(bidx)];
                        if(data[v].ip == aidx) { u = bv; }
                        else { u = av; };
                        };

                    if(av == bv) {
                        alpha.pop();
                        beta.pop();
                    } else if(u == av) {
                        alpha.pop();
                        beta.remove(u);
                    } else {
                        alpha.remove(u);
                        beta.pop();
                        };


                    vertData[u].visiteda = true;
                    vertData[u].visitedb = true;
                    *out = u;
                    ++out;

                    j = vertData[u].aOrder;
                    for(i = firsta[j]; i < firsta[j + 1]; i++) {
                        v = neigha[i].v;
                        if(vertData[v].visiteda) continue;
                        vertData[v].visiteda = true;
                        alpha.move(v);
                        };

                    j = vertData[u].bOrder;
                    for(i = firstb[j]; i < firstb[j + 1]; i++) {
                        v = neighb[i].v;
                        if(vertData[v].visitedb) continue;
                        vertData[v].visitedb = true;
                        beta.move(v);
                        };


                    alpha.done();
                    beta.done();
                    };
                };


            // UmbrellaTestVertex
            template<class GraphType, class Map>
            static bool UmbrellaTestVertex(const GraphType &g,
                               typename GraphType::PVertex u,
                               Map &data,
                               typename GraphType::PVertex *order) {
                int base, d, o;
                typename GraphType::PEdge e;
                typename GraphType::PVertex v;

                d = g.deg(u, EdUndir);
                if(d == 0) return true;

                bool LOCALARRAY(T, d);

                for(o = 0; o < d; o++) T[o] = false;
                base = data[u].posSigma + 1;	// siebie nie zaznaczamy: 0 to pierwszy s¹siad

                for(e = g.getEdge(u, EdUndir); e != NULL; e = g.getEdgeNext(u, e, EdUndir)) {
                    v = g.getEdgeEnd(e, u);
                    o = data[v].posSigma;
                    if(o < base) continue;
                    if(o - base >= d) return false;
                    T[o - base] = true;
                    };
                d--;
                while(d >= 0 && T[d] == false) d--;
                for(o = 0; o <= d; o++) { if(T[o] == false) return false; };
                return true;
                };


            // IsUmbrellaFree
            template<class GraphType, class Map>
            static bool IsUmbrellaFree(const GraphType &g,
                           Map &data,
                           typename GraphType::PVertex *order) {
                typename GraphType::PVertex v;
                for(v = g.getVert(); v != NULL; v = g.getVertNext(v)) {
                    if(!UmbrellaTestVertex(g, v, data, order)) return false;
                    };
                return true;
                };


            // reverse
            template<class T>
            static void reverse(T *tab, size_t n) {
                size_t i, n2;
                T t;
                n2 = n >> 1;
                for(i = 0; i < n2; i++) {
                    t = tab[i];
                    tab[i] = tab[n - 1 - i];
                    tab[n - 1 - i] = t;
                    };
                };


    };

    template <class GraphType>
	static bool interval(const GraphType& g)
	{
	    return Interval::graph2segs(g,blackHole);
	}

};

class IsIt : public IsItPar<AlgorithmsDefaultSettings> {};

}
#endif
