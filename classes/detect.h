#ifndef KOALA_DETECTOR_H
#define KOALA_DETECTOR_H

#include "..\container\simple.h"
#include "..\graph\graph.h"
#include "..\graph\view.h"
#include "..\algorithm\search.h"
#include "..\algorithm\factor.h"
#include "..\algorithm\conflow.h"
#include "..\container\joinsets.h"


namespace Koala
{


//Algorytmy rozpoznawania rodzin grafow. Dla rodziny family jest metoda rozpoznajaca bool family(const& graph).
//Jesli family ma sens takze dla multigrafow, bool family(const& graph, bool allowmulti) gdzie flaga bool podaje
//czy zezwalay na krawedzie rownolegle i petle. Jesli dla klasy family sa jakies szczegolne procedury (tylko dla grafow tego typu),
//wprowadza sie dodatkowo podklase Family z metodami realizujacymi te funkcje. Generalnie nie maja one obowiazku
// sprawdzania poprawnosci danych (tj. przynaleznosci grafu do tej rodziny), choc czasem tak czynia.

template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class IsItPar : public SearchStructs {

	public:


    // Generalnie zaklada sie, ze poprawny (dla IsItPar) graf wejsciowy nie ma petli ani lukow oraz n>0
    // Ta metoda jest wyjatkiem, sprawdza czy n=0
    template <class GraphType>
	static bool zero(const GraphType& g)
	{
	    return !g.getVertNo();
	}

    // czy graf jest prosty (tj. nie ma krawedzi rownoleglych)
    template <class GraphType>
	static bool undir(const GraphType& g,bool allowmulti=false)
	{   if (allowmulti) return g.getVertNo()>0 && g.getEdgeNo(EdDirIn|EdDirOut)==0; // undir
	    int i=0,m;            // simple
	    if (!g.getVertNo() || g.getEdgeNo(EdDirIn|EdDirOut|EdLoop)) return false;
	    std::pair<typename GraphType::PVertex,typename GraphType::PVertex>  LOCALARRAY(tabE,m=g.getEdgeNo());
	    for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
	        tabE[i++]=pairMinMax(g.getEdgeEnds(e));
	    DefaultStructs::sort( tabE,tabE + m );
	    for(i=1;i<m;i++) if (tabE[i-1]==tabE[i]) return false;
	    return true;
	}

    // czy spojny
    template <class GraphType>
	static bool connected(const GraphType& g, bool allowmulti=false)
	{
	    if (!undir(g,allowmulti)) return false;
	    return BFSPar<DefaultStructs>::scanAttainable(g,g.getVert(),blackHole)==g.getVertNo();
	}

    // czy bezkrawedziowy
	template <class GraphType>
	static bool empty(const GraphType& g)
	{
	    return g.getVertNo() && !g.getEdgeNo();
	}

    // klika
	template <class GraphType>
	static bool clique(const GraphType& g)
	{   int n=g.getVertNo();
	    return (g.getEdgeNo()==n*(n-1)/2) && undir(g,false);
	}

    // suma rozlacznych klik
	template <class GraphType>
	static bool cliques(const GraphType& g)
	{   if (!undir(g,false)) return false;
	    int LOCALARRAY(comptab,g.getVertNo()+1);
	    int e=0,comp=BFSPar<DefaultStructs>::split(g,SearchStructs::compStore(comptab,blackHole),EdUndir);
        for(int i=1;i<=comp;i++) e+=(comptab[i]-comptab[i-1])*(comptab[i]-comptab[i-1]-1)/2;
	    return e==g.getEdgeNo();
	}

    // drzewo
	template <class GraphType>
	static bool tree(const GraphType& g)
	{
	    return connected(g,true) && (g.getVertNo()-1==g.getEdgeNo());
	}

    // las
	template <class GraphType>
	static bool forest(const GraphType& g)
	{
	    return undir(g,true) && !BFSPar<DefaultStructs>::cyclNo(g);
	}

    // regularny
	template <class GraphType>
	static bool regular(const GraphType& g,bool allowmulti=false)
	{
	    if (!undir(g,allowmulti)) return false;
	    typename GraphType::PVertex v=g.getVert();
	    int deg=g.deg(v);
	    for(v=g.getVertNext(v);v;v=g.getVertNext(v))
            if (g.deg(v)!=deg) return false;
        return true;
	}

    class Path {
        public:

        // Konce podanej sciezki, (NULL,NULL) w razie bledu
        template <class GraphType>
        static std::pair<typename GraphType::PVertex,typename GraphType::PVertex> ends(const GraphType& g)
        {   std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
                    null=std::make_pair(typename GraphType::PVertex(0),typename GraphType::PVertex(0)),
                    res=null;
            if (!tree(g)) return null;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                switch (g.deg(v))
                {
                    case 0: return make_pair(v,v);
                    case 1: if (!res.first) res.first=v;
                            else    if (!res.second) res.second=v;
                                    else return null;

                    case 2: break;
                    default : return null;
                }
            return res;
        }
    };

    // sciezka
    template <class GraphType>
	static bool path(const GraphType& g)
	{
	    return Path::ends(g).first;
	}

    class Caterpillar {
        public:

        // Konce grzbietu gasienicy, (NULL,NULL) w razie bledu
        template <class GraphType>
        static std::pair<typename GraphType::PVertex,typename GraphType::PVertex> spineEnds(const GraphType& g)
        {   std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
                    null=std::make_pair(typename GraphType::PVertex(0),typename GraphType::PVertex(0));
            if (!connected(g,false)) return null;
            return Path::ends(makeSubgraph(g,std::make_pair(
                                        notChoose(orChoose(vertDegChoose(0, EdUndir),vertDegChoose(1, EdUndir))),
                                        stdChoose(true))));
        }
    };

    // gasienica
    template <class GraphType>
	static bool caterpillar(const GraphType& g)
	{
	    return Caterpillar::spineEnds(g).first;
	}

    // cykl,
	template <class GraphType>
	static bool cycle(const GraphType& g,bool allowmulti=false)
	{
	    return connected(g,allowmulti) && g.deg(g.getVert())==2 && regular(g,true);
	}

    // zbior niezalezny krawedzi
	template <class GraphType>
	static bool matching(const GraphType& g)
	{
	    return undir(g,true) && g.Delta()<=1;
	}

    // podkubiczny
	template <class GraphType>
	static bool subcubic(const GraphType& g,bool allowmulti=false)
	{
	    return undir(g,allowmulti) && g.Delta()<=3;
	}

    // kubiczny
	template <class GraphType>
	static bool cubic(const GraphType& g,bool allowmulti=false)
	{
	    return  g.getVert() && g.deg(g.getVert())==3 && regular(g,allowmulti);
	}

    // graf o wszystkich skladowych 2-spojnych bedacych klikami
	template <class GraphType>
	static bool block(const GraphType& g)
	{   if (!undir(g,false)) return false;
        int m;
	    int LOCALARRAY(comptab,g.getVertNo()+(m=g.getEdgeNo())+1);
	    int e=0,comp=BlocksPar<DefaultStructs>::split(g,blackHole,blackHole,SearchStructs::compStore(comptab,blackHole),blackHole,EdUndir);
        for(int i=1;i<=comp;i++) e+=(comptab[i]-comptab[i-1])*(comptab[i]-comptab[i-1]-1)/2;
	    return e==m;
	}

	template <class GraphType>
	// maks. liczba cyklomatyczna skladowej 2-spojnej grafu prostego. -1 w razie bledu.
	static int almostTree(const GraphType& g)
	{   if (!undir(g,false)) return -1;
        int n,m;
	    int LOCALARRAY(comptab,(n=g.getVertNo())+(m=g.getEdgeNo())+1);
	    int LOCALARRAY(compE,n+m+1);
	    for(int i=0;i<n+m+1;i++) compE[i]=0;
	    typename DefaultStructs:: template AssocCont<
            typename GraphType::PEdge, int >::Type edgeCont(n);
	    int res=0,comp=BlocksPar<DefaultStructs>::split(g,blackHole,edgeCont,SearchStructs::compStore(comptab,blackHole),blackHole,EdUndir);
        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) compE[edgeCont[e]]++;
        for(int i=0;i<comp;i++) res=std::max(res,compE[i]-(comptab[i+1]-comptab[i]-1));
	    return res;
	}

    class Bipartite {
        public:

        template <class GraphType,class Iter>
        // wypisuje na iterator wierzcholki jednej partycji grafu dwudzielnego. Zwraca licznosc partycji (-1 w razie bledu)
        static int getPart(const GraphType& g,Iter out,bool allowmulti=false)
        {   if ((!undir(g,allowmulti)) || g.getEdgeNo(EdLoop)) return -1;
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
                if ((vertCont[v].distance&1)==0) { licz++; if (!isBlackHole(out)) {*out=v; ++out; } }
            return licz;
        }

        template <class GraphType,class Iter>
        // znajduje najwiekszy zbior niezalezny, zwraca jego rozmiar
        static int maxStable(const GraphType &g, Iter out)
        {   Set<typename GraphType::PVertex> res;
            minVertCover(g,setInserter(res));
            res=g.getVertSet()-res;
            res.getElements(out);
            return res.size();
        }

        template <class GraphType,class Iter>
        // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
        static int minVertCover(const GraphType &g, Iter out)
        {	int n;
                typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, Koala::Matching::VertLabs<GraphType >
                >::Type vertTab(n=g.getVertNo());

            typename DefaultStructs:: template AssocCont<
            typename GraphType::PEdge, bool>::Type matching(n/2);

            Set<typename GraphType::PVertex> setL,setR;
            koalaAssert(-1!=getPart(g,setInserter(setL),true),AlgExcWrongArg);
            setR=g.getVertSet()-setL;
            int matchno=MatchingPar<DefaultStructs>::findMax(g,vertTab,assocInserter(matching,constFun(true)));

            Set<typename GraphType::PVertex> setT,setnew;

            //do zbioru setT dodajemy wszystkie wierzcholki wolne z setL
            for (typename GraphType::PVertex it=setL.first(); it; it=setL.next(it))
            {
                if (vertTab[it].vMatch == 0)
                    setT +=it;
            }

            //dla kazdego wierzcholka wolnego z setL, dodajemy do setT, jego sasiadow z setR oraz wierzcholki skojarzone z tymi sasiadami
//            for (typename Set<typename GraphType::PVertex>::iterator itT=setT.begin(); itT!=setT.end(); ++itT)
//                for (typename Set<typename GraphType::PVertex>::iterator itR=vertTabNeights[*itT].begin(); itR!=vertTabNeights[*itT].end(); ++itR)
//                    if (vertTab[*itT].vMatch != *itR)
//                    {
//                        setT.insert(*itR);
//                        for (typename Set<typename GraphType::PVertex>::iterator itL=vertTabNeights[*itR].begin(); itL!=vertTabNeights[*itR].end(); ++itL)
//                            if (vertTab[*itR].vMatch == *itL)
//                                setT.insert(*itL);
//                    }
            while(1)
            {
                setnew.clear();
                for (typename GraphType::PVertex itT=setT.first(); itT; itT=setT.next(itT))
                if (setL.isElement(itT))
                for (typename GraphType::PEdge e=g.getEdge(itT,EdUndir);e;e=g.getEdgeNext(itT,e,EdUndir))
                if (g.getEdgeEnd(e,itT)!=vertTab[itT].vMatch)

                {   typename GraphType::PVertex itR=g.getEdgeEnd(e,itT);
                    setnew+=itR;
    //                for (typename GraphType::PVertex itR=vertTabNeights[itT].first(); itR; itR=vertTabNeights[itT].next(itR))
                    for (typename GraphType::PEdge f=g.getEdge(itR,EdUndir);f;f=g.getEdgeNext(itR,f,EdUndir))
                    if (matching.hasKey(f)) setnew+=g.getEdgeEnd(f,itR);
                }
                if (setnew.subsetOf(setT)) break;
                setT+=setnew;
            }

//            std::cout <<"\nBreakpoint:"<<setT.size();
//            setT = (setL ^ setT); //w setT znajduja sie teraz wierzcholki nalezace do pokrycia wierzcholkowego
             setT = (setL ^ setT);
//             std::cout <<"\nBreakpoint:"<<setT.size();

            //wpisanie wyniku w strumien wyjsciowy wierzcholkow
            if (!isBlackHole(out))
            {
                for (typename GraphType::PVertex it=setT.first(); it; it=setT.next(it))
                {
                    *out = it; ++out;
                }
            }
            return setT.size();
        }

        protected:

    template <class GraphType>
    static void
        splitVert (
            const GraphType & g,
            Set<typename GraphType::PVertex> &setV1,
            Set<typename GraphType::PVertex> &setV2)
        {
            typename DefaultStructs:: template AssocCont<
                    typename GraphType::PVertex, SearchStructs::VisitVertLabs<GraphType > >
                            ::Type vertCont(g.getVertNo());
            BFSPar<DefaultStructs>::scan(g,blackHole,EdUndir,vertCont);
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                if ((vertCont[v].distance&1)==0) setV1+=v; else setV2+=v;
        }

    };


    // czy dwudzielny
    template <class GraphType>
	static bool bipartite(const GraphType& g,bool allowmulti=false)
	{
	    return Bipartite::getPart(g,blackHole,allowmulti)!=-1;
	}

    class CompBipartite {
        public:
        // wypisuje na iterator wierzcholki jednej partycji grafu pelnego dwudzielnego. Zwraca licznosc partycji (-1 w razie bledu)
        template <class GraphType,class Iter>
        static int getPart(const GraphType& g,Iter out)
        {   int n=g.getVertNo();
            typename GraphType::PVertex LOCALARRAY(tabE,n);
            int licz=Bipartite::getPart(g,tabE,false);
            if (licz==-1) return -1;
            if (licz*(n-licz)!=g.getEdgeNo(EdUndir)) return -1;
            if (!isBlackHole(out)) for(int i=0;i<licz;i++) { *out=tabE[i]; ++out; };
            return licz;
        }
    };

    // czy pelny dwudzielny
    template <class GraphType>
	static bool compBipartite(const GraphType& g)
	{
	    return CompBipartite::getPart(g,blackHole)!=-1;
	}


    class CompMPartite {
        public:

        template <class GraphType, class Iter,class VIter>
        // wyrzuca na out ciagi wierzcholkow tworzacych partycje grafu pelnego M-dzielnego. Zwraca liczbe partycji M lub -1 w razie bledu
        static int split(const GraphType& g,CompStore< Iter,VIter > out)
        {
            if (!undir(g,false)) return -1;
            int n;
            typename DefaultStructs:: template AssocCont<
                typename GraphType::PVertex, int >::Type colors(n=g.getVertNo());
            int LOCALARRAY(tabC,n);
            int i,licz=0,maxc=0;

            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            {   for(i=0;i<n;i++) tabC[i]=1;
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
            if (!isBlackHole(out.compIter))
            {
                for(i=0;i<=maxc;i++) { *out.compIter=licz; ++out.compIter; licz+=tabC[i];  }
                *out.compIter=n; ++out.compIter;
            }
            if (!isBlackHole(out.vertIter))
                for(i=0;i<=maxc;i++)
                    for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                        if (colors[v]==i)
                            { *out.vertIter=v; ++out.vertIter; }
            return maxc+1;
        }
    };

    // czy pelny M-dzielny dla pewnego M>0
    template <class GraphType>
	static bool compMPartite(const GraphType& g)
	{
	    return CompMPartite::split(g,compStore(blackHole,blackHole))!=-1;
	}

/* M. Habib, R. McConnel, C. Paul, L.Viennot
 * Lex-BFS and Partition Refinement, with Applications to Transitive
 * Orientation, Interval Graph Recognition and Consecutive Ones Testing
 */
    // Obsluga chordali
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

        template <class Graph>
        struct QTRes {
            int size;
            Set<typename Graph::PVertex> trees;
            QTRes() : size(0) {}
        };


        public:

        template<class Graph, class VIter2>
        // wyrzuca na iterator odwrotny perf. ellimination order chordal grafu tj. porzadek doklejania nowych wierzcholkow za podkliki
        // false gdy graf nie byl chordal
        static bool getOrder(const Graph &g,VIter2 riter) {
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

            if (!isBlackHole(riter))
                for(int i=g.getVertNo()-1;i>=0;i--) { *riter=pi[i];++riter;}

            return true;
        };

        // Tworzy reprezentacje chordala w postaci drzewa jego maksymalnych klik
        // zwraca liczbe maksymalnych klik
        // TODO: masakra! uproscic
        template<class Graph, class VIter, class VIterOut, class QIter,class QTEIter>
        static int maxCliques(const Graph &g, // badany graf
            VIter begin, VIter end, // zakres iteratorow z ktorego pobieramy rev. perf. elim. order jego wierzcholkow
            // (por. getOrder)
            CompStore< QIter,VIterOut > out, // wyjsciowe iteratory na ciagi maksymalnych klik
            QTEIter qte) // wyjsciowe iteratory na pary std::pair<int, int> - pary numerow klik bedacych koncami
            // "krawedzi" tego drzewa
        {   int i,j,licze=0,res,no,n;
            VIter vi;
            Set<typename Graph::PVertex> LOCALARRAY(tabs,n=g.getVertNo());
            typename Graph::PVertex LOCALARRAY(vbuf,n);
            int LOCALARRAY(sno,n);
            typename DefaultStructs:: template AssocCont<
                typename Graph::PVertex, bool >::Type tabf(n);
            for(i=0,vi=begin;vi!=end;++vi,++i)
            {   tabs[i]=*vi;
                typename Graph::PVertex u;
                for(typename Graph::PEdge e=g.getEdge(*vi,EdUndir);e;e=g.getEdgeNext(*vi,e,EdUndir))
                    if (tabf.hasKey(u=g.getEdgeEnd(e,*vi))) tabs[i]+=u;
                tabf[*vi]=true;
            }
            for(i=0;i<n-1;i++) for(j=i+1;j<n;j++)
                if (tabs[i].subsetOf(tabs[j]))
                    { tabs[i].clear(); licze++; break; }
            *out.compIter=0;++out.compIter;
            for(no=j=i=0,vi=begin;vi!=end;++vi,++i) if (!tabs[i].empty())
            {   tabs[i].getElements(vbuf); sno[no++]=i;
                for(int k=0;k<tabs[i].size();k++) { *out.vertIter=vbuf[k]; ++out.vertIter; }
                j+=tabs[i].size();*out.compIter=j;++out.compIter;
            }
            res=n-licze;
            if (!isBlackHole(qte) && res>1)
            {   g.getVerts(vbuf);
                JoinableSets<typename Graph::PVertex, typename DefaultStructs::template AssocCont<typename Graph::PVertex,
                    JSPartDesrc<typename Graph::PVertex> *>::Type > sets;
                sets.resize(n);
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
                        *qte = pairMinMax(ends); ++qte; edgeNo--;
                    };
                };

            }
            return res;
        }

        // j.w. ale samodzielna. Pobiera tylko graf, jesli nie byl chordal, zwraca -1
        template<class Graph, class VIterOut, class QIter,class QTEIter>
        static int maxCliques(const Graph &g,CompStore< QIter,VIterOut > out, QTEIter qte)
        {   int n;
            typename Graph::PVertex LOCALARRAY(vbuf,n=g.getVertNo());
            if (!getOrder(g,vbuf)) return -1;
            return maxCliques(g,vbuf,vbuf+n,out,qte);
        }

        // znajduje najwieksza klike w grafie, zwraca jej rozmiar
        template<class Graph, class VIter, class VIterOut>
        static int maxClique(const Graph &g, // badany graf
            VIter begin, VIter end, // zakres iteratorow z ktorego pobieramy rev. perf. elim. order jego wierzcholkow
            // (por. getOrder)
            VIterOut out) // wyjsciow iterator na elementy maksymalnej kliki
        {   int maxsize=1;
            typename Graph::PVertex u;
            typename Graph::PVertex maxv=*begin;
            typename DefaultStructs:: template AssocCont<
                typename Graph::PVertex, bool >::Type tabf(g.getVertNo());
            VIter vi=begin;tabf[maxv]=true;
            for(++vi;vi!=end;++vi)
            {   int siz=1;
                for(typename Graph::PEdge e=g.getEdge(*vi,EdUndir);e;e=g.getEdgeNext(*vi,e,EdUndir))
                    if (tabf.hasKey(u=g.getEdgeEnd(e,*vi))) siz++;
                tabf[*vi]=true;
                if (siz>maxsize) { maxsize=siz; maxv=*vi;}
            }
            tabf.clear();
            for(vi=begin;*vi!=maxv;++vi) tabf[*vi]=true;
            int licz=0;
            for(typename Graph::PEdge e=g.getEdge(maxv,EdUndir);e;e=g.getEdgeNext(maxv,e,EdUndir))
                    if (tabf.hasKey(u=g.getEdgeEnd(e,*vi)))
                    {   *out=u; ++out; ++licz; }
            *out=maxv; ++out; ++licz;
            return licz;
        }

        // j.w. ale samodzielna. Pobiera tylko graf, jesli nie byl chordal, zwraca -1
        template<class Graph, class VIterOut>
        static int maxClique(const Graph &g,VIterOut out)
        {   int n;
            typename Graph::PVertex LOCALARRAY(vbuf,n=g.getVertNo());
            if (!getOrder(g,vbuf)) return -1;
            return maxClique(g,vbuf,vbuf+n,out);
        }

        // znajduje najwiekszy zbior niezalezny (wypuszczany na out), zwraca jego rozmiar
        // korzysta z rozkladu na drzewo klik por. maxCliques
        template <class Graph,  class QIter,class VIter, class QTEIter,class IterOut>
        static int maxStable(const Graph& g, //badany graf
                             int qn, // liczba maksymalnych klik
                             QIter begin, // iteratory poczatkow ciagow ciagow wierzcholkow z maks. klik (por. compStore)
                             VIter vbegin,
                             QTEIter ebegin, // poczatek ciagu std::pair<int,int> z numerami klik-koncow krawedzi drzewa klikowego
                             IterOut out ) // iterator wyjsciowy na wierzcholki
        {   typename DefaultStructs:: template AssocCont<
                typename Graph::PVertex, QTRes<Graph> >::Type LOCALARRAY(tabtab,qn);
            QTRes<Graph>  LOCALARRAY(tabnull,qn);
            typedef typename DefaultStructs::template LocalGraph<
                std::pair<typename DefaultStructs:: template AssocCont<
                typename Graph::PVertex, QTRes<Graph> >::Type*,QTRes<Graph>*>,
                char,EdAll,true >:: Type ImageGraph;
            ImageGraph tree;
            typename ImageGraph::PVertex LOCALARRAY(treeverts,qn);
            QIter it=begin,it2=it;it2++;
            for(int i=0;i<qn;i++,it++,it2++)
            {   int size;
                (treeverts[i]=tree.addVert(std::make_pair(tabtab+i,tabnull+i)))->info.first
                            ->reserve(size=(*it2-*it));
                for(int j=0;j<size;j++,vbegin++) (*treeverts[i]->info.first)[*vbegin];

            }
            for(int i=0;i<qn-1;i++,ebegin++) tree.addEdge(treeverts[(*ebegin).first],treeverts[(*ebegin).second]);
            typename DefaultStructs:: template AssocCont<
                typename ImageGraph::PVertex,typename SearchStructs::template VisitVertLabs<ImageGraph > >::Type search(qn);
            DFSPostorderPar<DefaultStructs>::scanAttainable(tree,tree.getVert(),treeverts,EdUndir,search);
            for(typename ImageGraph::PVertex u=tree.getVert();u;u=tree.getVertNext(u)) if (search[u].ePrev)
                tree.ch2Dir(search[u].ePrev,u,EdDirOut);

            for(int i=0;i<qn;i++)
            {   typename ImageGraph::PVertex vert=treeverts[i];
                vert->info.second->size=0;vert->info.second->trees.clear();
                for(typename ImageGraph::PEdge e=tree.getEdge(vert,EdDirIn);e;e=tree.getEdgeNext(vert,e,EdDirIn))
                {   typename ImageGraph::PVertex child=tree.getEdgeEnd(e,vert);
                    int maxs=child->info.second->size,tmpsize;
                    Set< typename Graph::PVertex> *maxset=&child->info.second->trees;
                    for(typename Graph::PVertex key=child->info.first->firstKey();key;key=child->info.first->nextKey(key))
                    if ((!vert->info.first->hasKey(key)) && (tmpsize=(*child->info.first)[key].size)>maxs)
                        { maxs=tmpsize; maxset=&(*child->info.first)[key].trees; }
                    vert->info.second->size+=maxs;vert->info.second->trees+=*maxset;
                }
                typename ImageGraph::PVertex child;
                for(typename Graph::PVertex key=vert->info.first->firstKey();key;key=vert->info.first->nextKey(key))
                {   vert->info.first->operator[](key).size=1;
                    vert->info.first->operator[](key).trees.clear();vert->info.first->operator[](key).trees+=key;
                    for(typename ImageGraph::PEdge e=tree.getEdge(vert,EdDirIn);e;e=tree.getEdgeNext(vert,e,EdDirIn))
                    if ((child=tree.getEdgeEnd(e,vert))->info.first->hasKey(key))
                    {   (*vert->info.first)[key].size+=(*child->info.first)[key].size-1;
                        (*vert->info.first)[key].trees+=(*child->info.first)[key].trees;
                    } else
                    {   int maxs=child->info.second->size,tmpsize;
                        Set< typename Graph::PVertex> *maxset=&child->info.second->trees;
                        for(typename Graph::PVertex childkey=child->info.first->firstKey();childkey;childkey=child->info.first->nextKey(childkey))
                        if ((!vert->info.first->hasKey(childkey)) && (tmpsize=(*child->info.first)[childkey].size)>maxs)
                        { maxs=tmpsize; maxset=&(*child->info.first)[childkey].trees; }

                        vert->info.first->operator[](key).size+=maxs;
                        vert->info.first->operator[](key).trees+=*maxset;
                    }
                }
            }

            typename ImageGraph::PVertex root=treeverts[qn-1];
            int maxs=root->info.second->size,tmpsize;
            Set< typename Graph::PVertex> *maxset=&root->info.second->trees;
            for(typename Graph::PVertex key=root->info.first->firstKey();key;key=root->info.first->nextKey(key))
            if  ((tmpsize=(root->info.first->operator[](key).size))>maxs)
            {   maxs=tmpsize;
                maxset=&root->info.first->operator[](key).trees;
            }
            maxset->getElements(out);
            return maxset->size();
        return tree.getVertNo();
        }


        // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
        // sens parametrow j.w.
        template <class Graph,  class QIter,class VIter, class QTEIter,class IterOut>
        static int minVertCover(const Graph& g,int qn,QIter begin,VIter vbegin, QTEIter ebegin,IterOut out )
        {   Set<typename Graph::PVertex> res;
            maxStable(g,qn,begin,vbegin, ebegin,setInserter(res));
            res=g.getVertSet()-res;
            res.getElements(out);
            return res.size();
        }

        // znajduje najwiekszy zbior niezalezny (wypuszczany na out), zwraca jego rozmiar lub -1 w razie bledu
        // samodzielna
        template <class Graph,class IterOut>
        static int maxStable(const Graph& g, //badany graf
                             IterOut out ) // iterator wyjsciowy na wierzcholki
        {   int n=g.getVertNo();
            typename Graph::PVertex LOCALARRAY(vbegin,n*n);
            int LOCALARRAY(begin,n+1);
            std::pair<int,int> LOCALARRAY(ebegin,n);
            int qn=maxCliques(g,compStore(begin,vbegin),ebegin);
            if (qn==-1) return -1;
            return maxStable(g,qn,begin,vbegin,ebegin,out);
        }

        // znajduje najwieksze pokrycie wierzcholkowe (wypuszczane na out), zwraca jego rozmiar lub -1 w razie bledu
        // samodzielna
        template <class Graph,class IterOut>
        static int minVertCover(const Graph& g, //badany graf
                             IterOut out ) // iterator wyjsciowy na wierzcholki
        {   Set<typename Graph::PVertex> res;
            maxStable(g,setInserter(res));
            res=g.getVertSet()-res;
            res.getElements(out);
            return res.size();
        }

    };

    /** test if graph is chordal
     * @param[in] g graph to test
     * @return true if graph is chordal, false otherwise */
    template <class GraphType>
	static bool chordal(const GraphType& g)
	{
	    return Chordal::getOrder(g,blackHole);
	}

    // czy graf jest dopelnieniem chordala
    template <class GraphType>
	static bool cochordal(const GraphType& g)
	{
	    if (!undir(g,true) || g.getEdgeNo(Loop)>0) return false;
	    typedef typename DefaultStructs::template LocalGraph<typename GraphType::PVertex,char,Undirected,true >:: Type ImageGraph;
        ImageGraph cg;
        for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u)) cg.addVert(u);
        for(typename ImageGraph::PVertex u=cg.getVert();u!=cg.getVertLast();u=cg.getVertNext(u))
            for(typename ImageGraph::PVertex v=cg.getVertNext(u);v;v=cg.getVertNext(v))
                if (!g.getEdge(u->info,v->info,EdUndir)) cg.addEdge(u,v,EdUndir);

//	    typename DefaultStructs::LocalGraph::Type cg;
//	    typename DefaultStructs:: template TwoDimTriangleAssocCont<
//            typename GraphType::PVertex, bool >::Type matr(g.getVertNo());
//        typename DefaultStructs:: template AssocCont<
//            typename GraphType::PVertex, typename DefaultStructs::LocalGraph::Type::PVertex >::Type vmap(g.getVertNo());
//        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v)) vmap[v]=cg.addVert();
//        for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//            if (matr(g.getEdgeEnds(e))) return false;
//            else matr(g.getEdgeEnds(e))=true;
//        for(typename GraphType::PVertex v=g.getVert();v!=g.getVertLast();v=g.getVertNext(v))
//            for(typename GraphType::PVertex u=g.getVertNext(v);u;u=g.getVertNext(u))
//                if (!matr(u,v)) cg.addEdge(vmap[v],vmap[u]);

//	    cg.copy(g);cg.neg(Undirected);
	    return chordal(cg);
	}

    // czy splitgraph
	template <class GraphType>
	static bool split(const GraphType& g)
	{
	    return chordal(g) && cochordal(g);
	}


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

                AdjStruct(size_t s,Privates::List<Node,Privates::BlockListAllocator<Privates::ListNode<Node> > >* adata): datasize(s),data(adata) {
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

                    for(int i = 0; i < datasize; i++)
                    {   data[i].sort();                  }
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
                Privates::List<Node,Privates::BlockListAllocator<Privates::ListNode<Node > > > *data;
                int datasize;
        };  //AdjStruct

        template<class Graph>
        struct CTState {
            AdjStruct<Graph> cls;
            bool flag;
            int k;
            int ud;
            CTState(size_t n,Privates::List<typename AdjStruct<Graph>::Node,Privates::BlockListAllocator<Privates::ListNode<typename AdjStruct<Graph>::Node> > > * acls)
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

        class FlowDefaultStructs : public DefaultStructs {
        public:

            enum { useFulkersonFord=false  };

        };


        public:
            /* M.C. Golumbic
             * The Complexity of Comparability Graph Recognition and Coloring
             * Computing 18, 199-208 (1977)
             */
            // Kompleksowa obsluga comparability grafu
            // zwraca liczbe chromatyczna lub -1 jesli graf nie byl comparability
            // dirmap - wysciowa tablica asocjacyjna PVertex->EdgeDirection z przykladowa comparability orientation krawedzi
            // (kierunek krawedzi miedzy getEdgeEnd1 a getEdgeEnd2). Lub BlackHole.
            // aheightmap- wysciowa tablica asocjacyjna PVertex->int z optymalnym pokolorowaniem wierzcholkowym. Lub BlackHole.
            // cliqueiter - iterator wyjsciowy, na ktory zostaje zapisana najwieksza klika
            // TODO: tyle, ze poki co nie dziala!
            template<class Graph,class DirMap, class OutMap, class OutIter>
            static int explore(const Graph &g, DirMap& dirmap,OutMap &aheightmap,OutIter cliqueiter)
            {
                if (!undir(g,false)) return -1;
                int b,i,m,h,n = g.getVertNo();
                if (n==1)
                {   if (!isBlackHole(aheightmap)) aheightmap[g.getVert()] = 0;
                    *cliqueiter = g.getVert(); ++cliqueiter;
                    //return True;
                    return 1;
                }

//                typename DefaultStructs:: template AssocCont<typename Graph::PEdge,
//                EdgeDirection >::Type localdirmap;
//                typename BlackHoleSwitch<DirMap,typename DefaultStructs::template AssocCont<typename Graph::PEdge,
//                EdgeDirection >::Type >::Type &
//                    dirmap=
//                BlackHoleSwitch<DirMap,typename DefaultStructs:: template AssocCont<typename Graph::PEdge,
//                EdgeDirection >::Type >::get(adirmap,localdirmap);
                if (DefaultStructs::ReserveOutAssocCont) dirmap.reserve(g.getEdgeNo());


                typename Graph::PVertex LOCALARRAY(idxv, n);
//                std::list<typename AdjStruct<Graph>::Node> LOCALARRAY(statebuf,n);
                Privates::BlockListAllocator<Privates::ListNode<typename AdjStruct<Graph>::Node> > pula(2*g.getEdgeNo());    // TODO: size?
                Privates::List<typename AdjStruct<Graph>::Node,Privates::BlockListAllocator<Privates::ListNode<typename AdjStruct<Graph>::Node> > >
                    LOCALARRAY(statebuf,n);

                for(i=0;i<n;i++) statebuf[i].init(pula);
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
                if (isBlackHole(aheightmap)||DefaultStructs::ReserveOutAssocCont) heightmap.reserve(n);


                for(i = 0; i < n; i++) heightmap[idxv[i]] = height[i] - 1;

                if (!isBlackHole(dirmap))
                    for(typename Graph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
                    {   // TODO: w finalnej wersji usunac assert
                        assert(heightmap[g.getEdgeEnd1(e)]!=heightmap[g.getEdgeEnd2(e)]);
                        if (heightmap[g.getEdgeEnd1(e)]<heightmap[g.getEdgeEnd2(e)]) dirmap[e]=EdDirIn;
                        else dirmap[e]=EdDirOut;
                    }
                if (!isBlackHole(cliqueiter)) GetClique(state, height, b, idxv, cliqueiter);
                return m;
            };

            // sprawdza, czy graf byl comparability
            // adirmap - wysciowa tablica asocjacyjna PVertex->EdgeDirection z przykladowa comparability orientation krawedzi
            // (kierunek krawedzi miedzy getEdgeEnd1 a getEdgeEnd2). Lub BlackHole
            template<class Graph,class DirMap>
            static bool getDirs(const Graph &g, DirMap& adirmap)
            {   return explore(g,adirmap,blackHole,blackHole)!=-1; }

            // sprawdza, czy graf byl comparability. Jesli tak, nadaje krawedziom wlasciwa orientacje
            template<class Graph>
            static bool getDirs(Graph &g)
            {   int m=g.getEdgeNo();
                typename Graph::PEdge LOCALARRAY(tab,m);
                typename DefaultStructs:: template AssocCont<typename Graph::PEdge,
                EdgeDirection >::Type dir(m);
                if (explore(g,dir,blackHole,blackHole)==-1) return false;
                g.getEdges(tab);
                for(int i=0;i<m;i++)
                    g.ch2Dir(tab[i],(dir[tab[i]]==EdDirOut) ? g.getEdgeEnd1(tab[i]) : g.getEdgeEnd2(tab[i]));
                return true;
            }

            // zwraca liczbe chromatyczna lub -1 jesli graf nie byl comparability
            // avmap - wysciowa tablica asocjacyjna PVertex->int z optymalnym pokolorowaniem wierzcholkowym. Lub BlackHole.
            template<class Graph,class OutMap>
            static int color(const Graph &g, OutMap& avmap)
            {   return explore(g,blackHole,avmap,blackHole); }

	    /** find a largest clique in a comparability graph
	     * @param[in] g graph
	     * @param[out] iter iterator to write clique's vertices. Lub BlackHole.
	     * @return number of vertices in the clique */
            template<class Graph,class OutIter>
            static int maxClique(const Graph &g, OutIter iter)
            {   return explore(g,blackHole,blackHole,iter); }


                // znajduje najwiekszy zbior niezalezny, zwraca jego rozmiar
        template<class GraphType, class VIterOut>
        static int maxStable(const GraphType &g,VIterOut out)
        {   typedef typename DefaultStructs::template LocalGraph<char,typename GraphType::PVertex,Directed,false >:: Type ImageGraph;
            int n=g.getVertNo(),m=g.getEdgeNo();
            ImageGraph cg;
            typename DefaultStructs:: template AssocCont<
                typename GraphType::PVertex, typename ImageGraph::PEdge >::Type mapa(n);
            typename DefaultStructs:: template AssocCont<
                typename GraphType::PEdge, EdgeDirection >::Type dirs(m);
            typename DefaultStructs:: template AssocCont<
                typename ImageGraph::PVertex, typename FlowPar<FlowDefaultStructs>::template TrsVertLoss<int> >
                    ::Type vertcont(2+2*n);
            typename DefaultStructs:: template AssocCont<
                typename ImageGraph::PEdge, typename FlowPar<FlowDefaultStructs>::template  TrsEdgeLabs<int> >
                    ::Type edgecont(3*n+m);
            typename ImageGraph::PVertex start=cg.addVert(),end=cg.addVert();
            for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u))
            {   typename ImageGraph::PVertex x=cg.addVert();
                typename ImageGraph::PVertex y=cg.addVert();
                vertcont[x]=vertcont[y]= typename FlowPar<FlowDefaultStructs>::template TrsVertLoss<int>();
                edgecont[mapa[u]=cg.addArch(x,y,u)]=typename FlowPar<FlowDefaultStructs>::template
                        TrsEdgeLabs<int>(1,1);
                edgecont[cg.addArch(start,x,(typename GraphType::PVertex)0)]=
                    edgecont[cg.addArch(y,end,(typename GraphType::PVertex)0)]=
                    typename FlowPar<FlowDefaultStructs>::template TrsEdgeLabs<int>(0,1);
            }
            getDirs(g,dirs);
            for(typename GraphType::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
            {   typename GraphType::PVertex u,v;
                if (dirs[e]==EdDirOut) { u=g.getEdgeEnd1(e); v=g.getEdgeEnd2(e);}
                else { u=g.getEdgeEnd2(e); v=g.getEdgeEnd1(e);}
                edgecont[cg.addArch(cg.getEdgeEnd2(mapa[u]),cg.getEdgeEnd1(mapa[v]),(typename GraphType::PVertex)0)]
                    =typename FlowPar<FlowDefaultStructs>::template TrsEdgeLabs<int>(0,1);
            }
            int a=0,b=n,c;
            while (b-a>1)
            {   c=(a+b)/2;
                vertcont[end]=typename FlowPar<FlowDefaultStructs>::template TrsVertLoss<int>(0,c);
                vertcont[start]=typename FlowPar<FlowDefaultStructs>::template TrsVertLoss<int>(-c,0);
                if (FlowPar<FlowDefaultStructs>::transship(cg,edgecont,vertcont)) b=c; else a=c;
            }
                vertcont[end]=typename FlowPar<FlowDefaultStructs>::template TrsVertLoss<int>(0,b-1);
                vertcont[start]=typename FlowPar<FlowDefaultStructs>::template TrsVertLoss<int>(-b+1,0);

            if (!isBlackHole(out)) for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u))
            {
                edgecont[mapa[u]]=typename FlowPar<FlowDefaultStructs>::template TrsEdgeLabs<int>(0,0);
                if (FlowPar<FlowDefaultStructs>::transship(cg,edgecont,vertcont))
                { *out=u; ++out;
                    edgecont[mapa[u]]=typename FlowPar<FlowDefaultStructs>::template TrsEdgeLabs<int>(1,1);
                }
            }
            return b;
        }

        template <class GraphType,class Iter>
        // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
        static int minVertCover(const GraphType &g, Iter out)
        {   Set<typename GraphType::PVertex> res;
            maxStable(g,setInserter(res));
            res=g.getVertSet()-res;
            res.getElements(out);
            return res.size();
        }


    };

    /** test if graph is a comparability graph
     * @param[in] g graph to test
     * @return true if graph is a comparability graph, false otherwise */
    template <class GraphType>
	static bool comparability(const GraphType& g)
	{
	    return Comparability::explore(g,blackHole,blackHole,blackHole)!=-1;
	}

    // czy graf jest dopelnieniem comparability
    template <class GraphType>
	static bool cocomparability(const GraphType& g)
	{
	    if (!undir(g,true) || g.getEdgeNo(Loop)>0) return false;
	    typedef typename DefaultStructs::template LocalGraph<typename GraphType::PVertex,char,Undirected,true >:: Type ImageGraph;
        ImageGraph cg;
        for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u)) cg.addVert(u);
        for(typename ImageGraph::PVertex u=cg.getVert();u!=cg.getVertLast();u=cg.getVertNext(u))
            for(typename ImageGraph::PVertex v=cg.getVertNext(u);v;v=cg.getVertNext(v))
                if (!g.getEdge(u->info,v->info,EdUndir)) cg.addEdge(u,v,EdUndir);


	    return comparability(cg);
	}


    class Interval : protected LexBFSPar<DefaultStructs> {

        public:

            // Struktura reprezentujaca przedzial domkniety na prostej o koncach calkowitych (dopuszczalna dlugosc 0)
            struct Segment {
                int left, right;
                Segment(int l=0,int r=1) : left(l), right(r) { /*assert(l<r);*/ }
            };

            // czy dwa takie przedzialy tna sie niepusto
            static bool touch(Segment a, Segment b)
            {   return std::max(a.left,b.left)<= std::min(a.right,b.right); }

        // konwersja zbior przedzialow-> interval graph
        // pobiera spomiedzy 2 iteratorow ciag przedzialow (struktur typu segment)
        // dopisuje do danego grafu odpowiadajacy mu graf przeciec (zwraca pierwszy stworzony wierzcholek)
        // pola info wierzcholkow i krawedzi tworzy funktorami vinfo(int), einfo(int,int) - argumenty to
        // numery przedzialow w ciagu wejsciowym
        // na iterator out zwraca sekwencje stworzonych wierzcholkow zgodna z cagiem przedzialow
        template<class GraphType,class Iter,class IterOut, class VInfoGen, class EInfoGen>
        static typename GraphType::PVertex
        segs2graph(GraphType &g,Iter begin, Iter end,IterOut out,VInfoGen vinfo, EInfoGen einfo)
        {   typename GraphType::PVertex res=0,tmp;
            int licz=0,i=0,j=0;
            Iter it;
            for(it=begin;it!=end;++it)

            {   koalaAssert((*it).left<=(*it).right,AlgExcWrongArg); licz++;    }
            if (!licz) return 0;
            typename GraphType::PVertex LOCALARRAY(tabv,licz);
            for(it=begin;it!=end;++it)
            {   tmp=tabv[i++]=g.addVert(vinfo(i));
                if (!res) res=tmp;
            }
            for(i=0, it=begin;it!=end;++it,++i)
            {   Iter it2=it;it2++;j=i+1;
                for(;it2!=end;++it2,j++) if (touch(*it,*it2)) g.addEdge(tabv[i],tabv[j],einfo(i,j),EdUndir);
            }
            for(i=0;i<licz;i++)
            { *out=tabv[i]; ++out;  }
            return res;
        }


        // j.w. ale polom info nadawane sa wartosci domyslne
        template<class GraphType,class Iter,class IterOut>
        static typename GraphType::PVertex segs2graph(GraphType &g,Iter begin, Iter end,IterOut out)
        {  return segs2graph(g,begin,end,out,
                            ConstFunctor<typename GraphType::VertInfoType>(),
                            ConstFunctor<typename GraphType::EdgeInfoType>());
        }

	/** convert interval graph to its interval representation
	 * @param[in] g graph
	 * @param[out] outmap map (PVertex -> Segment), Lub BlackHole.
	 * @return true if g is interval, false otherwise */
        template<class GraphType,class IntMap>
        static bool graph2segs(const GraphType &g,IntMap& outmap) {
            if (!undir(g,false)) return false;
            unsigned int i, n;
            n = g.getVertNo();


            typename GraphType::PVertex LOCALARRAY(sigma, n);
            typename GraphType::PVertex LOCALARRAY(sigmap, n);
            typename GraphType::PVertex LOCALARRAY(sigmapp, n);
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex, IvData>::Type data(n);


            Privates::BlockListAllocator<Privates::ListNode<Privates::List_iterator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > > > allocat(n+2); //TODO: size?
            Privates::BlockListAllocator<Privates::ListNode<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > > allocat2(2*n+2); //TODO: size?
            Privates::BlockListAllocator<Privates::ListNode<typename Sets::Elem> > allocat3(2*n*n); //TODO: size?


//            DefaultCPPAllocator allocat3;
            std::pair<typename Sets::Entry,
                typename Sets::Entry::iterator> LOCALARRAY(Abuf,n);
            std::pair<typename Sets::Entry,
                typename Sets::Entry::iterator> LOCALARRAY(Bbuf,n);
            for(i=0;i<n;i++)
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
                    if(DefaultStructs::ReserveOutAssocCont) outmap.reserve(n);
		    CalculateI(g, sigma, data, &IvData::posSigma, &IvData::ip);
                    for(int i=0;i<n;i++) { outmap[sigma[i]] = Segment(i, data[sigma[i]].ip); }
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
                    Privates::List<Elem, Privates::BlockListAllocator<Privates::ListNode<Elem> > > *cont;
                    Privates::List_iterator<Elem> next;
                    Elem(int _v = -1): value(_v), next(), cont(NULL) {};
                    };
//                typedef TElem<List, Allocator> Elem;
//                typedef List<Elem, DefaultCPPAllocator> Entry;
                typedef Privates::List<Elem, Privates::BlockListAllocator<Privates::ListNode<Elem> > > Entry;

//            public:
                Sets(std::pair<Entry, Privates::List_iterator<Elem> >* data,size_t n,Privates::BlockListAllocator<Privates::ListNode<Elem> > &a): m_data(data)
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


                //BlockListAllocator<List_iterator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > > allocat(2*g.getVertNo()+2);
                //BlockListAllocator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > allocat2(2*g.getVertNo()+2);
                typename LexBFSPar<DefaultStructs>::template
                    LexVisitContainer<GraphType,
                        Privates::BlockListAllocator< Privates::ListNode<Privates::List_iterator<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > > >,
                        Privates::BlockListAllocator<Privates::ListNode<typename LexBFSPar<DefaultStructs>::template LVCNode<GraphType> > > >
                alpha(allocat,allocat2,n),beta(allocat,allocat2,n);

//                typename LexBFSPar<DefaultStructs>::template LexVisitContainer<GraphType, DefaultCPPAllocator,DefaultCPPAllocator> alpha(pula,pula);
//                typename LexBFSPar<DefaultStructs>::template LexVisitContainer<GraphType, DefaultCPPAllocator,DefaultCPPAllocator> beta(pula,pula);
                typename DefaultStructs::template AssocCont<typename GraphType::PVertex, LBSData>::Type vertData(n);

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

    /** test if graph is an interval graph
     * @param[in] g graph
     * @return true if g is interval, false otherwise */
    template <class GraphType>
	static bool interval(const GraphType& g)
	{
//	    return Interval::graph2segs(g,blackHole);
        return chordal(g) && cocomparability(g);
	}

    // czy pierwszy
    template <class GraphType>
	static bool prime(const GraphType& g)
	{   if (!undir(g,false)) return false;
        int n=g.getVertNo();
        if (n<4) return false;
	    typename ModulesPar<DefaultStructs>::Partition res=
            ModulesPar<DefaultStructs>::split(g,compStore(blackHole,blackHole),blackHole);
        return (res.type==mpPrime) && (res.size==n);
	}


	class Cograph {
	     template <class Defs> friend class IsItPar;

        public:

        template<class GraphType, class VIterOut>
        static int maxClique(const GraphType &g,VIterOut out)
        {     typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, bool >::Type subset(g.getVertNo());
            for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u)) subset[u];
            Set<typename GraphType::PVertex> res=maxClique2(g,subset);
            for(typename GraphType::PVertex u=res.first();u;u=res.next(u))
            { *out=u; ++out; }
            return res.size();
        }

        template<class GraphType, class VIterOut>
        static int maxStable(const GraphType &g,VIterOut out)
        {     typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, bool >::Type subset(g.getVertNo());
            for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u)) subset[u];
            Set<typename GraphType::PVertex> res=maxStable2(g,subset);
            for(typename GraphType::PVertex u=res.first();u;u=res.next(u))
            { *out=u; ++out; }
            return res.size();
        }

        template <class GraphType,class Iter>
        // znajduje najmniejsze pokrycie wierzcholkowe, zwraca jego rozmiar
        static int minVertCover(const GraphType &g, Iter out)
        {   Set<typename GraphType::PVertex> res;
            maxStable(g,setInserter(res));
            res=g.getVertSet()-res;
            res.getElements(out);
            return res.size();
        }


//        TODO: template<class Graph, class AssocTab>
//        static int color(const Graph &g,AssocTab out);

        protected:

        template <class GraphType, class Assoc>
        static bool cograph(const GraphType& ag,Assoc& subset)
        {   Subgraph<GraphType, AssocHasChooser<Assoc*>, BoolChooser>
                g=makeSubgraph(ag,std::make_pair(extAssocKeyChoose(&subset),stdChoose(true)));
            int n;
            if ((n=g.getVertNo())==1) return true;
            typename GraphType::PVertex LOCALARRAY(tabv,n);
            int LOCALARRAY(tabc,n+1);
            typename ModulesPar<DefaultStructs>::Partition parts=ModulesPar<DefaultStructs>::split(g,compStore(tabc,tabv),blackHole,true);
            if (parts.type==mpPrime) return false;
            for(int i=0;i<parts.size;i++)
            {   subset.clear();
                for(int j=tabc[i];j<tabc[i+1];j++) subset[tabv[j]];
                if (!cograph(ag,subset)) return false;
            }
            return true;
        }

        template <class GraphType, class Assoc>
        static Set<typename GraphType::PVertex> maxClique2(const GraphType& ag,Assoc& subset)
        {   Subgraph<GraphType, AssocHasChooser<Assoc*>, BoolChooser>
                g=makeSubgraph(ag,std::make_pair(extAssocKeyChoose(&subset),stdChoose(true)));
            int n;
            if ((n=g.getVertNo())==1) return g.getVertSet();
            Set<typename GraphType::PVertex> res,tmp;
            typename GraphType::PVertex LOCALARRAY(tabv,n);
            int LOCALARRAY(tabc,n+1);
            typename ModulesPar<DefaultStructs>::Partition parts=ModulesPar<DefaultStructs>::split(g,compStore(tabc,tabv),blackHole,true);
            koalaAssert(parts.type!=mpPrime,AlgExcWrongArg);
            for(int i=0;i<parts.size;i++)
            {   subset.clear();
                for(int j=tabc[i];j<tabc[i+1];j++) subset[tabv[j]];
                tmp=maxClique2(ag,subset);
                if (parts.type==mpConnected) res+=tmp;
                else if (tmp.size()>res.size()) res=tmp;
            }
            return res;
        }


        template <class GraphType, class Assoc>
        static Set<typename GraphType::PVertex> maxStable2(const GraphType& ag,Assoc& subset)
        {   Subgraph<GraphType, AssocHasChooser<Assoc*>, BoolChooser>
                g=makeSubgraph(ag,std::make_pair(extAssocKeyChoose(&subset),stdChoose(true)));
            int n;
            if ((n=g.getVertNo())==1) return g.getVertSet();
            Set<typename GraphType::PVertex> res,tmp;
            typename GraphType::PVertex LOCALARRAY(tabv,n);
            int LOCALARRAY(tabc,n+1);
            typename ModulesPar<DefaultStructs>::Partition parts=ModulesPar<DefaultStructs>::split(g,compStore(tabc,tabv),blackHole,true);
            koalaAssert(parts.type!=mpPrime,AlgExcWrongArg);
            for(int i=0;i<parts.size;i++)
            {   subset.clear();
                for(int j=tabc[i];j<tabc[i+1];j++) subset[tabv[j]];
                tmp=maxStable2(ag,subset);
                if (parts.type==mpDisconnected) res+=tmp;
                else if (tmp.size()>res.size()) res=tmp;
            }
            return res;
        }


	};


    public:


    // czy cograph
    template <class GraphType>
	static bool cograph(const GraphType& g)
	{   if (!undir(g,false)) return false;
        typename DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, bool >::Type subset(g.getVertNo());
        for(typename GraphType::PVertex u=g.getVert();u;u=g.getVertNext(u)) subset[u];
        	    typename ModulesPar<DefaultStructs>::Partition res=
            ModulesPar<DefaultStructs>::split(g,compStore(blackHole,blackHole),blackHole);
        return Cograph::cograph(g,subset);
	}

};


// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class IsIt : public IsItPar<AlgsDefaultSettings> {};

}
#endif
