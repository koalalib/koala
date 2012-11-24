//KG: GLOWNE USTAWIENIA TYPOW STRUKTUR:
//KG: SPRAWDZIC ZAROWNO DLA true JAK I false
#define AdjAllow    false

//KG: SPRAWDZIC WE WSZYSTKICH 3 PRZYPADKACH TJ. GDY DOKLADNIE JEDNA Z PONIZSZYCH LINII JEST NIEZAKOMENTOWANA
#define ArrayMapTest
//#define StdMapTest
//#define HashMapTest

//KG: reasumujac: 6 rodzajow testow (j.w.) dla kazdej procedury



//KG: natomiast z ponizszych dwoch defineow odkomentowany powinien byc zaden lub jeden (tj. 3 rodzaje testow)
//#define KOALA_SET_ON_VECTOR
//#define KOALA_SET_ON_HASHSET

#include <string>
#include <iostream>
#include <sstream>

//#define KOALA_DONT_USE_ALLOCA

#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"
#include "Koala/classes/detect.h"
#include "Koala/classes/create.h"
#include "Koala/algorithm/mis.h"
#include "Koala/io/g6.h"
#include "Koala/io/text.h"


using namespace std;
using namespace Koala;


class DefSettings : public AlgsDefaultSettings {};

class StdMapSettings : public AlgsDefaultSettings {

      public:

        template< class A, class B > class AssocCont
        {
          public:
            typedef AssocTable < std::map<A,B> > Type;

        };

        template< class A, class B, AssocMatrixType type > class TwoDimAssocCont
        {
          public:
            typedef AssocMatrix<A,B,type,std::vector< BlockOfAssocMatrix<B> >,PseudoAssocArray<A,int,AssocTable<std::map<A,int> > > > Type;
        };

};


class HashMapSettings : public AlgsDefaultSettings {

      public:

        template< class A, class B > class AssocCont
        {
          public:
            typedef AssocTable < BiDiHashMap<A,B> > Type;

        };

        template< class A, class B, AssocMatrixType type > class TwoDimAssocCont
        {
          public:
            typedef AssocMatrix<A,B,type,std::vector< BlockOfAssocMatrix<B> >,PseudoAssocArray<A,int,AssocTable<BiDiHashMap<A,int> > > > Type;
        };
};


template< EdgeType edAllow, bool adjMatrixAllowed > class GrDefSettings :
    public GrDefaultSettings<edAllow, adjMatrixAllowed>
    {};

template< EdgeType edAllow, bool adjMatrixAllowed > class GrStdMapSettings :
    public GrDefaultSettings<edAllow, adjMatrixAllowed>
{
      public:

        template< class A, class B > class VertAssocCont
        {
          public:
            typedef AssocTable<std::map<A,B> > Type;
        };

        template< class A, class B > class ExtVertAssocCont
        {
          public:
            typedef AssocTable<std::map<A,B> > Type;
        };

        template< class K, class V > class AdjMatrixDirEdges
        {
          public:
             typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;
        };

        template< class K, class V > class AdjMatrixUndirEdges
        {
          public:
            typedef AssocMatrix<K,V,AMatrTriangle,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;
        };
};


template< EdgeType edAllow, bool adjMatrixAllowed > class GrHashMapSettings :
    public GrDefaultSettings<edAllow, adjMatrixAllowed>
{
      public:

        template< class A, class B > class VertAssocCont
        {
          public:
            typedef AssocTable<BiDiHashMap<A,B> > Type;
        };

        template< class A, class B > class ExtVertAssocCont
        {
          public:
            typedef AssocTable<BiDiHashMap<A,B> > Type;
        };

        template< class K, class V > class AdjMatrixDirEdges
        {
          public:
             typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
        };

        template< class K, class V > class AdjMatrixUndirEdges
        {
          public:
            typedef AssocMatrix<K,V,AMatrTriangle,std::vector< BlockOfAssocMatrix<V> >,PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
        };
};


#if defined(ArrayMapTest)
#define  Settings DefSettings
#define  GrSettings GrDefSettings
#endif
#if defined(StdMapTest)
#define  Settings StdMapSettings
#define  GrSettings GrStdMapSettings
#endif
#if defined(HashMapTest)
#define  Settings HashMapSettings
#define  GrSettings GrHashMapSettings
#endif


typedef Graph<int,int,GrSettings<EdUndir|EdLoop,AdjAllow> > MyGraph;
typedef Graph<int,int,GrSettings<EdDirIn|EdDirOut|EdLoop,AdjAllow> > MyDigraph;


class FibonHeapSettings : public Settings {

        public:
        template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
            class HeapCont
        {
          public:
            typedef FibonHeap< Key,Compare,Allocator > Type;
            typedef FibonHeapNode< Key > NodeType;
        };
};


class BinomHeapSettings : public Settings {

        public:
        template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
            class HeapCont
        {
          public:
            typedef BinomHeap< Key,Compare,Allocator > Type;
            typedef BinomHeapNode< Key > NodeType;
        };
};


class PairHeapSettings : public Settings {

        public:
        template< class Key, class Compare = std::less< Key >, class Allocator = Privates::DefaultCPPAllocator >
            class HeapCont
        {
          public:
            typedef PairHeap< Key,Compare,Allocator > Type;
            typedef PairHeapNode< Key > NodeType;
        };
};

int readDigraph(MyDigraph& g,const char *napis)
{
    int n=1;
    for(int i=0;napis[i]!='}';i++)
        if (napis[i]==',') n++;
    MyDigraph::PVertex LOCALARRAY(tabv,n);
    for(int i=0;i<n;i++)
        tabv[i]=g.addVert(i);
    string str=napis;
    stringstream input(str);
    char c;

    input >> c; assert(c=='{');
    for(int i=0;i<n;i++)
    {   input >> c; assert(c=='{');
        for(int j=0;j<n;j++)
        {   int flag;
            input >> flag >> c;
            if (flag!=0 && i==j) g.addLoop(tabv[i],flag);
            if (flag!=0 && i!=j) g.addArc(tabv[i],tabv[j],flag);
        }
        input >> c;
    }
    return n;
}

//int readGraph(MyGraph& g,const char *napis)
//{
//    int n=1;
//    for(int i=0;napis[i]!='}';i++)
//        if (napis[i]==',') n++;
//    MyGraph::PVertex LOCALARRAY(tabv,n);
//    for(int i=0;i<n;i++)
//        tabv[i]=g.addVert(i);
//    string str=napis;
//    stringstream input(str);
//    char c;
//
//    input >> c; assert(c=='{');
//    for(int i=0;i<n;i++)
//    {   input >> c; assert(c=='{');
//        for(int j=0;j<n;j++)
//        {   int flag;
//            input >> flag >> c;
//            if (flag!=0 && i==j) g.addLoop(tabv[i],flag);
//            if (flag!=0 && i<j) g.addLink(tabv[i],tabv[j],flag);
//        }
//        input >> c;
//    }
//    return n;
//}

//bool bipartite(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::bipartite(g);
//    cout << int(res);
//    return res;
//}
//
//bool chordal(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::chordal(g);
//    cout << int(res);
//    return res;
//}
//
//template< class T > struct ChordTestChooser
//{
//    T p;
//
//    typedef ChordTestChooser< T > ChoosersSelfType;
//
//    ChordTestChooser( const T arg ): p( arg ) { }
//
//    template< class Graph > bool operator()( typename Graph::PVertex v, const Graph & ) const { return v->info.isElement( p ); }
//};
//
//
//bool chordalRepr(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::chordal(g);
//    cout << int(res);
//    if (res)//xxx
//    {
//        Settings::LocalGraph< Set<MyGraph::PVertex> ,char,EdUndir >::Type tree;
//        tree.makeAdjMatrix();
//        MyGraph::PVertex LOCALARRAY(tabv,g.getVertNo()*g.getVertNo());
//        int LOCALARRAY(tabpos,g.getVertNo()+1);
//        std::pair<int,int> LOCALARRAY(tabpar,g.getVertNo()+1);
//
//        int cl=IsItPar<Settings>::Chordal::maxCliques(g,SearchStructs::compStore(tabpos,tabv),tabpar);
//            for(int i=0;i<cl;i++)
//            {   tree.addVert();
//                for(int j=tabpos[i];j<tabpos[i+1];j++) tree.getVertLast()->info.add(tabv[j]);
//            }
//        for(int i=0;i<cl-1;i++) tree.addLink(tree.vertByNo(tabpar[i].first),tree.vertByNo(tabpar[i].second));
//        assert(IsItPar< Settings >::tree(tree));
//
//        for(MyGraph::PVertex u=g.getVert();u;u=g.getVertNext(u))
//            assert(IsItPar< Settings >::connected(makeSubgraph(tree,ChordTestChooser<MyGraph::PVertex>(u) & stdChoose(true))));
//
//        for( MyGraph::PVertex v=g.getVert();v!=g.getVertLast();v=g.getVertNext(v))
//            for( MyGraph::PVertex u=g.getVertNext(v);u;u=g.getVertNext(u))
//                assert(
//                       bool(g.getEdge(v,u))==
//                       bool(makeSubgraph(tree,(ChordTestChooser<MyGraph::PVertex>(u)&&ChordTestChooser<MyGraph::PVertex>(v))&stdChoose(true)).getVert())
//                       );
//    }
//    return res;
//}
//
//bool comparability(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::comparability(g);
//    cout << int(res);
//    return res;
//}
//
//bool comparabilityRepr(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::comparability(g);
//    cout << int(res);
//    if (res)
//    {
//        Settings::LocalGraph<char,char,EdAll >::Type lg;
//        lg.makeAdjMatrix();
//        lg.copy(g);
//        assert(IsItPar< Settings >::Comparability::getDirs(lg));
//        RelDiagramPar< Settings >:: transClousure(lg);
//        assert(!lg.getEdgeNo(EdLoop));
//        assert(DAGAlgsPar< Settings >::isDAG(lg));
//    }
//    return res;
//}
//
//bool interval(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::interval(g);
//    cout << int(res);
//    return res;
//}
//
//bool intervalRepr(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    Settings::AssocCont<MyGraph::PVertex,IsItPar< Settings >::Interval::Segment >::Type vmap;
//    bool res=IsItPar< Settings >::Interval::graph2segs(g,vmap);
//    cout << int(res);
//    if (res)
//        for( MyGraph::PVertex v=g.getVert();v!=g.getVertLast();v=g.getVertNext(v))
//            for( MyGraph::PVertex u=g.getVertNext(v);u;u=g.getVertNext(u))
//            {
//                assert(((bool)g.getEdge(u,v))==IsItPar< Settings >::Interval::touch(vmap[u],vmap[v]));
//            }
//    return res;
//}
//
//bool planar(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::planar(g);
//    cout << int(res);
//    return res;
//}
//
//bool outerplanar(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::outerplanar(g);
//    cout << int(res);
//    return res;
//}
//
bool split(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::split(g);
    cout << int(res);
    return res;
}
//
//bool cograph(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    bool res=IsItPar< Settings >::cograph(g);
//    cout << int(res);
//    return res;
//}

//int mis(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    MISPar< Settings >::get(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int clique(const char* g6)
//{
//    MyGraph g;
////    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    g.neg(EdUndir);
//    MISPar< Settings >::get(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//
//int misBipartite(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    IsItPar< Settings >::Bipartite::maxStable(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int misChordal(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    IsItPar< Settings >::Chordal::maxStable(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int cliqueChordal(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    IsItPar< Settings >::Chordal::maxClique(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int misComparability(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    IsItPar< Settings >::Comparability::maxStable(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int cliqueComparability(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    IsItPar< Settings >::Comparability::maxClique(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//
//int misCograph(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    IsItPar< Settings >::Cograph::maxStable(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int cliqueCograph(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    IsItPar< Settings >::Cograph::maxClique(g,setInserter(res));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int matching(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PEdge LOCALARRAY(tabe,g.getEdgeNo());
//    int res=MatchingPar< Settings >::findMax(g,blackHole,tabe);
//    cout << '{';
//    for(int i=0;i<res;i++)
//    {
//        cout << '{';
//        std::pair<int,int> pair=pairMinMax(g.vertPos(g.getEdgeEnd1(tabe[i])),g.vertPos(g.getEdgeEnd2(tabe[i])));
//        cout << pair.first << ',' << pair.second;
//        cout << '}';
//        if (i!=res-1) cout << ',';
//    }
//    cout << '}';
//    return res;
//}

//int componentsBFS(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PVertex LOCALARRAY(tabv,g.getVertNo());
//    int LOCALARRAY(tabpos,g.getVertNo()+1);
//    int res=BFSPar< Settings >::split(g,SearchStructs::compStore(tabpos,tabv));
//    cout << '{';
//    for(int i=0;i<res;i++)
//    {
//        cout << '{';
//        for(int j=tabpos[i];j<tabpos[i+1];j++)
//        {
//            cout << g.vertPos(tabv[j]);
//            if (j!=tabpos[i+1]-1) cout << ',';
//        }
//        cout << '}';
//        if (i!=res-1) cout << ',';
//    }
//    cout << '}';
//    return res;
//}
//
//int componentsDFSPre(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PVertex LOCALARRAY(tabv,g.getVertNo());
//    int LOCALARRAY(tabpos,g.getVertNo()+1);
//    int res=DFSPreorderPar< Settings >::split(g,SearchStructs::compStore(tabpos,tabv));
//    cout << '{';
//    for(int i=0;i<res;i++)
//    {
//        cout << '{';
//        for(int j=tabpos[i];j<tabpos[i+1];j++)
//        {
//            cout << g.vertPos(tabv[j]);
//            if (j!=tabpos[i+1]-1) cout << ',';
//        }
//        cout << '}';
//        if (i!=res-1) cout << ',';
//    }
//    cout << '}';
//    return res;
//}
//
//int componentsDFSPost(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PVertex LOCALARRAY(tabv,g.getVertNo());
//    int LOCALARRAY(tabpos,g.getVertNo()+1);
//    int res=DFSPostorderPar< Settings >::split(g,SearchStructs::compStore(tabpos,tabv));
//    cout << '{';
//    for(int i=0;i<res;i++)
//    {
//        cout << '{';
//        for(int j=tabpos[i];j<tabpos[i+1];j++)
//        {
//            cout << g.vertPos(tabv[j]);
//            if (j!=tabpos[i+1]-1) cout << ',';
//        }
//        cout << '}';
//        if (i!=res-1) cout << ',';
//    }
//    cout << '}';
//    return res;
//}
//
//int componentsLex(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PVertex LOCALARRAY(tabv,g.getVertNo());
//    int LOCALARRAY(tabpos,g.getVertNo()+1);
//    int res=LexBFSPar< Settings >::split(g,SearchStructs::compStore(tabpos,tabv));
//    cout << '{';
//    for(int i=0;i<res;i++)
//    {
//        cout << '{';
//        for(int j=tabpos[i];j<tabpos[i+1];j++)
//        {
//            cout << g.vertPos(tabv[j]);
//            if (j!=tabpos[i+1]-1) cout << ',';
//        }
//        cout << '}';
//        if (i!=res-1) cout << ',';
//    }
//    cout << '}';
//    return res;
//}
//
//int blocks(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PVertex LOCALARRAY(tabv,g.getVertNo());
//    int LOCALARRAY(tabpos,g.getVertNo()+g.getEdgeNo()+1);
//    int res=BlocksPar< Settings >::split(g,blackHole,blackHole,SearchStructs::compStore(tabpos,tabv),blackHole);
//    cout << '{';
//    for(int i=0;i<res;i++)
//    {
//        cout << '{';
//        for(int j=tabpos[i];j<tabpos[i+1];j++)
//        {
//            cout << g.vertPos(tabv[j]);
//            if (j!=tabpos[i+1]-1) cout << ',';
//        }
//        cout << '}';
//        if (i!=res-1) cout << ',';
//    }
//    cout << '}';
//    return res;
//}
//
//int distUndir(const char* g6)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    int u=rand() % g.getVertNo(),v=rand() % g.getVertNo();
//    int res=BFSPar< Settings >::getPath(g,g.vertByNo(u),g.vertByNo(v),PathStructs::outPath(blackHole,blackHole));
//    cout << '{' << res << ',' << u << ',' << v << '}';
//    return res;
//}
//
//int distDir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo(),v=rand() % g.getVertNo();
//    int res=BFSPar< Settings >::getPath(g,g.vertByNo(u),g.vertByNo(v),PathStructs::outPath(blackHole,blackHole));
//    cout << '{' << res << ',' << u << ',' << v << '}';
//    return res;
//}
//
//int strongComponents(const char* napis)
//{
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    MyDigraph::PVertex LOCALARRAY(tabv,g.getVertNo());
//    int LOCALARRAY(tabpos,g.getVertNo()+1);
//    int res=SCCPar< Settings >::split(g,SearchStructs::compStore(tabpos,tabv),blackHole);
//    cout << '{';
//    for(int i=0;i<res;i++)
//    {
//        cout << '{';
//        for(int j=tabpos[i];j<tabpos[i+1];j++)
//        {
//            cout << g.vertPos(tabv[j]);
//            if (j!=tabpos[i+1]-1) cout << ',';
//        }
//        cout << '}';
//        if (i!=res-1) cout << ',';
//    }
//    cout << '}';
//    return res;
//}
//
//bool isDAG(const char* napis)
//{
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    bool res=DAGAlgsPar< Settings >::isDAG(g);
//    cout << res;
//    return res;
//}
//
//bool eulerCycleDir(const char* napis)
//{
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    MyDigraph::PVertex LOCALARRAY(tabv,g.getEdgeNo()+1);
//    bool res=EulerPar< Settings >::getDirCycle(g,PathStructs::outPath(tabv,blackHole));
//    if (!res)
//    {
//        cout << 0;
//        return false;
//    }
//    cout << '{';
//    for(int i=0;i<g.getEdgeNo();i++)
//    {
//        cout << g.vertPos(tabv[i]);
//        if (i!=(g.getEdgeNo()-1)) cout << ',';
//    }
//    cout << '}';
//    return true;
//}
//
//bool eulerPathDir(const char* napis)
//{
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    MyDigraph::PVertex LOCALARRAY(tabv,g.getEdgeNo()+1);
//    bool res=EulerPar< Settings >::getDirPath(g,PathStructs::outPath(tabv,blackHole));
//    if (!res)
//    {
//        cout << 0;
//        return false;
//    }
//    cout << '{';
//    for(int i=0;i<g.getEdgeNo();i++)
//    {
//        cout << g.vertPos(tabv[i]);
//        if (i!=(g.getEdgeNo()-1)) cout << ',';
//    }
//    cout << '}';
//    return true;
//}
//
//bool eulerCycleUndir(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PVertex LOCALARRAY(tabv,g.getEdgeNo()+1);
//    bool res=EulerPar< Settings >::getCycle(g,PathStructs::outPath(tabv,blackHole));
//    if (!res)
//    {
//        cout << 0;
//        return false;
//    }
//    cout << '{';
//    for(int i=0;i<g.getEdgeNo();i++)
//    {
//        cout << g.vertPos(tabv[i]);
//        if (i!=(g.getEdgeNo()-1)) cout << ',';
//    }
//    cout << '}';
//    return true;
//}
//
//bool eulerPathUndir(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    IO::readG6(g,g6);
//    MyGraph::PVertex LOCALARRAY(tabv,g.getEdgeNo()+1);
//    bool res=EulerPar< Settings >::getPath(g,PathStructs::outPath(tabv,blackHole));
//    if (!res)
//    {
//        cout << 0;
//        return false;
//    }
//    cout << '{';
//    for(int i=0;i<g.getEdgeNo();i++)
//    {
//        cout << g.vertPos(tabv[i]);
//        if (i!=(g.getEdgeNo()-1)) cout << ',';
//    }
//    cout << '}';
//    return true;
//}
//
//void bellmanFordDir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo();
//    Settings::AssocCont<MyDigraph::PEdge,BellmanFordPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::AssocCont<MyDigraph::PVertex,BellmanFordPar< Settings >::VertLabs<int,MyDigraph> >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    BellmanFordPar <Settings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void bellmanFordUndir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyGraph g;
//    g.makeAdjMatrix();
//    readGraph(g,napis);
//    int u=rand() % g.getVertNo();
//    Settings::AssocCont<MyGraph::PEdge,BellmanFordPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::AssocCont<MyGraph::PVertex,BellmanFordPar< Settings >::VertLabs<int,MyGraph> >::Type vmap;
//    for(MyGraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    BellmanFordPar <Settings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void floydDir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo();
//    Settings::AssocCont<MyDigraph::PEdge,FloydPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::TwoDimAssocCont<MyDigraph::PVertex,FloydPar< Settings >::VertLabs<int,MyDigraph>,AMatrFull >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    FloydPar <Settings>::distances(g,vmap,emap);
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap(g.vertByNo(u),g.vertByNo(i)).distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void floydUndir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyGraph g;
//    g.makeAdjMatrix();
//    readGraph(g,napis);
//    int u=rand() % g.getVertNo();
//    Settings::AssocCont<MyGraph ::PEdge,FloydPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::TwoDimAssocCont<MyGraph ::PVertex,FloydPar< Settings >::VertLabs<int,MyGraph >,AMatrFull >::Type vmap;
//    for(MyGraph ::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    FloydPar <Settings>::distances(g,vmap,emap);
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap(g.vertByNo(u),g.vertByNo(i)).distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//
//void dijkstraDir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo();
//    Settings::AssocCont<MyDigraph::PEdge,DijkstraPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::AssocCont<MyDigraph::PVertex,DijkstraPar< Settings >::VertLabs<int,MyDigraph> >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraPar <Settings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void dijkstraUndir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyGraph g;
//    g.makeAdjMatrix();
//    readGraph(g,napis);
//    int u=rand() % g.getVertNo();
//    Settings::AssocCont<MyGraph::PEdge,DijkstraPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::AssocCont<MyGraph::PVertex,DijkstraPar< Settings >::VertLabs<int,MyGraph> >::Type vmap;
//    for(MyGraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraPar <Settings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void dijkstraFibonDir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo();
//    FibonHeapSettings::AssocCont<MyDigraph::PEdge,DijkstraHeapPar< FibonHeapSettings >::EdgeLabs<int> >::Type emap;
//    FibonHeapSettings::AssocCont<MyDigraph::PVertex,DijkstraHeapPar< FibonHeapSettings >::VertLabs<int,MyDigraph> >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraHeapPar <FibonHeapSettings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void dijkstraFibonUndir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyGraph g;
//    g.makeAdjMatrix();
//    readGraph(g,napis);
//    int u=rand() % g.getVertNo();
//    FibonHeapSettings::AssocCont<MyGraph::PEdge,DijkstraHeapPar< FibonHeapSettings >::EdgeLabs<int> >::Type emap;
//    FibonHeapSettings::AssocCont<MyGraph::PVertex,DijkstraHeapPar< FibonHeapSettings >::VertLabs<int,MyGraph> >::Type vmap;
//    for(MyGraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraHeapPar <FibonHeapSettings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void dijkstraBinomDir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo();
//    BinomHeapSettings::AssocCont<MyDigraph::PEdge,DijkstraHeapPar< BinomHeapSettings >::EdgeLabs<int> >::Type emap;
//    BinomHeapSettings::AssocCont<MyDigraph::PVertex,DijkstraHeapPar< BinomHeapSettings >::VertLabs<int,MyDigraph> >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraHeapPar <BinomHeapSettings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void dijkstraBinomUndir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyGraph g;
//    g.makeAdjMatrix();
//    readGraph(g,napis);
//    int u=rand() % g.getVertNo();
//    BinomHeapSettings::AssocCont<MyGraph::PEdge,DijkstraHeapPar< BinomHeapSettings >::EdgeLabs<int> >::Type emap;
//    BinomHeapSettings::AssocCont<MyGraph::PVertex,DijkstraHeapPar< BinomHeapSettings >::VertLabs<int,MyGraph> >::Type vmap;
//    for(MyGraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraHeapPar <BinomHeapSettings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//
//void dijkstraPairDir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo();
//    PairHeapSettings::AssocCont<MyDigraph::PEdge,DijkstraHeapPar< PairHeapSettings >::EdgeLabs<int> >::Type emap;
//    PairHeapSettings::AssocCont<MyDigraph::PVertex,DijkstraHeapPar< PairHeapSettings >::VertLabs<int,MyDigraph> >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraHeapPar <PairHeapSettings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//void dijkstraPairUndir(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyGraph g;
//    g.makeAdjMatrix();
//    readGraph(g,napis);
//    int u=rand() % g.getVertNo();
//    PairHeapSettings::AssocCont<MyGraph::PEdge,DijkstraHeapPar< PairHeapSettings >::EdgeLabs<int> >::Type emap;
//    PairHeapSettings::AssocCont<MyGraph::PVertex,DijkstraHeapPar< PairHeapSettings >::VertLabs<int,MyGraph> >::Type vmap;
//    for(MyGraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DijkstraHeapPar <PairHeapSettings>::distances(g,vmap,emap,g.vertByNo(u));
//
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//int kruskal(const char* napis)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    readGraph(g,napis);
//    MyGraph::PEdge LOCALARRAY(tabe,g.getEdgeNo());
//    Settings::AssocCont<MyGraph::PEdge,KruskalPar< Settings >::EdgeLabs<int> >::Type emap;
//    for(MyGraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].weight=e->info;
//    KruskalPar< Settings >::Result<int> res=KruskalPar< Settings >::getMinForest(g,emap,tabe,blackHole);
//    cout << '{';
//    for(int i=0;i<res.edgeNo;i++)
//    {
//        cout << '{';
//        std::pair<int,int> pair=pairMinMax(g.vertPos(g.getEdgeEnd1(tabe[i])),g.vertPos(g.getEdgeEnd2(tabe[i])));
//        cout << pair.first << ',' << pair.second;
//        cout << '}';
//        if (i!=res.edgeNo-1) cout << ',';
//    }
//    cout << '}';
//    return res.weight;
//}
//
//void critPath(const char* napis)
//{
//    //KG: jesli testy nie maja byc w kolko te same, zadbaj o ustawienie nasionka funkcja srand()
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    int u=rand() % g.getVertNo();
//    Settings::AssocCont<MyDigraph::PEdge,DAGCritPathPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::AssocCont<MyDigraph::PVertex,DAGCritPathPar< Settings >::VertLabs<int,MyDigraph> >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DAGCritPathPar <Settings>::critPathLength(g,vmap,emap,g.vertByNo(u));
//    cout << '{' << u <<",{";
//    for(int i=0;i<g.getVertNo();i++)
//    {
//        cout << vmap[g.vertByNo(i)].distance ;
//        if (i!=g.getVertNo()-1) cout << ',';
//    }
//    cout << "}}";
//}
//
//int critPath2(const char* napis)
//{
//    MyDigraph g;
//    g.makeAdjMatrix();
//    readDigraph(g,napis);
//    Settings::AssocCont<MyDigraph::PEdge,DAGCritPathPar< Settings >::EdgeLabs<int> >::Type emap;
//    Settings::AssocCont<MyDigraph::PVertex,DAGCritPathPar< Settings >::VertLabs<int,MyDigraph> >::Type vmap;
//    for(MyDigraph::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
//        emap[e].length=e->info;
//    DAGCritPathPar <Settings>::critPathLength(g,vmap,emap,(MyDigraph::PVertex)0);
//    MyDigraph::PVertex umax=g.getVert();
//    for(MyDigraph::PVertex u=g.getVertNext(umax);u;u=g.getVertNext(u))
//        if (vmap[u].distance>vmap[umax].distance)
//            umax=u;
//    MyDigraph::PVertex LOCALARRAY(tabv,g.getVertNo());
//    int res=DAGCritPathPar <Settings>::getPath(g,vmap,umax,PathStructs::outPath(tabv,blackHole));
//    cout << '{';
//    for(int i=0;i<=res;i++)
//    {
//        cout << g.vertPos(tabv[i]);
//        if (i!=res) cout << ',';
//    }
//    cout << '}';
//    return vmap[umax].distance;
//}


#include "main.hpp"

    char napis[20],napis2[20];

    {   cout <<"Undirected (g6 or matrix format):\n";
        MyGraph g;
        MyGraph::PVertex A,B,C,D,tabv[100]={0,0,0,0,0,0,0};
//        A=g.addVert(0);B=g.addVert(1);C=g.addVert(2);D=g.addVert(3);
//        g.addEdge(C,D);g.addEdge(A,B);g.addEdge(B,C);
//        g.addEdge(A,D);
//
//        g.addEdge(C,A);
//        g.addEdge(B,D);

//        IO::writeG6(g,napis,20);

//        IO::readG6(g,"JA?A???????");

//        g.clear();
//        kruskal("{{0,3,2},{3,0,5},{2,5,1}}");
//        dijkstraPairUndir("{{0,3,2},{3,0,5},{2,5,1}}");

//        IO::writeGraphText(g, cout, IO::RG_VertexLists );
//        const int u=1, v=3, w=6;
//        const int u=0, v=1, w=2;
//        for(int i=0;i<11;i++) tabv[i]=g.addVert();
//        for(int i=0;i<10;i++) for(int j=i+1;j<11;j++)
//        {
//            if ((i==u) && (j==v)) continue;
//            if ((i==u) && (j==w)) continue;
//            g.addEdge(tabv[i],tabv[j]);
//        }

//        g.addEdge(tabv[1],tabv[3]);g.addEdge(tabv[1],tabv[6]);
//        g.neg(EdUndir);

//    g.makeAdjMatrix();
    split("L~~~~n~~~~~~~~");


    }

//    {   cout <<"\nDirected (matrix format):\n";
//        MyDigraph g;
//        MyDigraph::PVertex A,B,C,D;
//
//        critPath2("{{0,3,0},{0,0,4},{0,0,0}}");
//    }
//    {   cout <<"\nOthers:\n";
//        Graph<char,int,GrDefaultSettings<Directed,true> > digraph;
//        digraph.makeAdjMatrix();
//        digraph.addVert('C');digraph.addVert('D');
//        digraph.addArc(digraph.getVert(),digraph.getVertLast());
//                IO::writeGraphText(digraph, cout, IO::RG_VertexLists | IO::RG_VInfo);
//
//        cout << "\n***************\n";
//
//        Graph<char,int,GrDefaultSettings<Undirected,true> > graph;
//        graph.makeAdjMatrix();
//        graph.addVert('A');graph.addVert('B');
//        graph.addEdge(graph.getVert(),graph.getVertLast());
//                IO::writeGraphText(graph, cout, IO::RG_VertexLists | IO::RG_VInfo);
//        cout << "\n***************\n";
//
////        Graph<char,int,GrDefaultSettings<Undirected,true> > graph2;
////        graph2.makeAdjMatrix();
////        graph2.move(graph);
////                IO::writeGraphText(graph2, cout, IO::RG_VertexLists | IO::RG_VInfo);
////        cout << "\n***************\n";
////                IO::writeGraphText(graph, cout, IO::RG_VertexLists | IO::RG_VInfo);
////        cout << "\n***************\n";
//
//        graph.substitute(graph.getVertLast(),makeUndirView(digraph),stdChoose(true)&stdChoose(true),stdCast()&stdCast());
//                IO::writeGraphText(graph, cout, IO::RG_VertexLists | IO::RG_VInfo);
//        cout << "\n***************\n";
//
//    }



}

