//KG: GLOWNE USTAWIENIA TYPOW STRUKTUR:
//KG: SPRAWDZIC ZAROWNO DLA true JAK I false
#define AdjAllow    true

//KG: SPRAWDZIC WE WSZYSTKICH 3 PRZYPADKACH TJ. GDY DOKLADNIE JEDNA Z PONIZSZYCH LINII JEST NIEZAKOMENTOWANA
#define ArrayMapTest
//#define StdMapTest
//#define HashMapTest

//KG: reasumujac: 6 rodzajow testow (j.w.) dla kazdej procedury



//KG: natomiast z ponizszych dwoch defineow odkomentowany powinien byc zaden lub jeden (tj. 3 rodzaje testow)
//#define KOALA_SET_ON_VECTOR
#define KOALA_SET_ON_HASHSET

#include <string>
#include <iostream>
#include <sstream>

//#define KOALA_DONT_USE_ALLOCA

#include "../Koala/graph/graph.h"
#include "../Koala/graph/view.h"
#include "../Koala/classes/detect.h"
#include "../Koala/classes/create.h"
#include "../Koala/algorithm/mis.h"
#include "../Koala/io/g6.h"
#include "../Koala/io/text.h"


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
            typedef AssocMatrix<A,B,type,std::vector< Privates::BlockOfAssocMatrix<B> >,Koala::Privates::PseudoAssocArray<A,int,AssocTable<std::map<A,int> > > > Type;
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
            typedef AssocMatrix<A,B,type,std::vector< Privates::BlockOfAssocMatrix<B> >,Koala::Privates::PseudoAssocArray<A,int,AssocTable<BiDiHashMap<A,int> > > > Type;
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
             typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< Privates::BlockOfAssocMatrix<V> >,Koala::Privates::PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;
        };

        template< class K, class V > class AdjMatrixUndirEdges
        {
          public:
            typedef AssocMatrix<K,V,AMatrTriangle,std::vector< Privates::BlockOfAssocMatrix<V> >,Koala::Privates::PseudoAssocArray<K,int,AssocTable<std::map<K,int> > > > Type;
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
             typedef AssocMatrix<K,V,AMatrNoDiag,std::vector< Privates::BlockOfAssocMatrix<V> >,Koala::Privates::PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
        };

        template< class K, class V > class AdjMatrixUndirEdges
        {
          public:
            typedef AssocMatrix<K,V,AMatrTriangle,std::vector< Privates::BlockOfAssocMatrix<V> >,Koala::Privates::PseudoAssocArray<K,int,AssocTable<BiDiHashMap<K,int> > > > Type;
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
        template< class Key, class Compare = std::less< Key > >
            class HeapCont
        {
          public:
            typedef FibonHeap< Key,Compare > Type;
            typedef FibonHeapNode< Key > NodeType;
        };
};


class BinomHeapSettings : public Settings {

        public:
        template< class Key, class Compare = std::less< Key > >
            class HeapCont
        {
          public:
            typedef BinomHeap< Key,Compare > Type;
            typedef BinomHeapNode< Key > NodeType;
        };
};


class PairHeapSettings : public Settings {

        public:
        template< class Key, class Compare = std::less< Key > >
            class HeapCont
        {
          public:
            typedef PairHeap< Key,Compare> Type;
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

int readGraph(MyGraph& g,const char *napis)
{
    int n=1;
    for(int i=0;napis[i]!='}';i++)
        if (napis[i]==',') n++;
    MyGraph::PVertex LOCALARRAY(tabv,n);
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
            if (flag!=0 && i<j) g.addLink(tabv[i],tabv[j],flag);
        }
        input >> c;
    }
    return n;
}

bool bipartite(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::bipartite(g);
    cout << int(res);
    return res;
}

bool chordal(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::chordal(g);
    cout << int(res);
    return res;
}

template< class T > struct ChordTestChooser
{
    T p;

    typedef ChordTestChooser< T > ChoosersSelfType;

    ChordTestChooser( const T arg ): p( arg ) { }

    template< class Graph > bool operator()( typename Graph::PVertex v, const Graph & ) const { return v->info.isElement( p ); }
};


bool chordalRepr(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::chordal(g);
    cout << int(res);
    if (res)
    {
        Settings::LocalGraph< Set<MyGraph::PVertex> ,char,EdUndir >::Type tree;
        tree.makeAdjMatrix();
        MyGraph::PVertex LOCALARRAY(tabv,g.getVertNo()*g.getVertNo());
        int LOCALARRAY(tabpos,g.getVertNo()+1);
        std::pair<int,int> LOCALARRAY(tabpar,g.getVertNo()+1);

        int cl=IsItPar<Settings>::Chordal::maxCliques(g,SearchStructs::compStore(tabpos,tabv),tabpar);
            for(int i=0;i<cl;i++)
            {   tree.addVert();
                for(int j=tabpos[i];j<tabpos[i+1];j++) tree.getVertLast()->info.add(tabv[j]);
            }
        for(int i=0;i<cl-1;i++) tree.addLink(tree.vertByNo(tabpar[i].first),tree.vertByNo(tabpar[i].second));
        assert(IsItPar< Settings >::tree(tree));

        for(MyGraph::PVertex u=g.getVert();u;u=g.getVertNext(u))
            assert(IsItPar< Settings >::connected(makeSubgraph(tree,ChordTestChooser<MyGraph::PVertex>(u) & stdChoose(true))));

        for( MyGraph::PVertex v=g.getVert();v!=g.getVertLast();v=g.getVertNext(v))
            for( MyGraph::PVertex u=g.getVertNext(v);u;u=g.getVertNext(u))
                assert(
                       bool(g.getEdge(v,u))==
                       bool(makeSubgraph(tree,(ChordTestChooser<MyGraph::PVertex>(u)&&ChordTestChooser<MyGraph::PVertex>(v))&stdChoose(true)).getVert())
                       );
    }
    return res;
}

bool comparability(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::comparability(g);
    cout << int(res);
    return res;
}

bool comparabilityRepr(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::comparability(g);
    cout << int(res);
    if (res)
    {
        Settings::LocalGraph<char,char,EdAll >::Type lg;
        lg.makeAdjMatrix();
        lg.copy(g);
        assert(IsItPar< Settings >::Comparability::getDirs(lg));
        RelDiagramPar< Settings >:: transClosure(lg);
        assert(!lg.getEdgeNo(EdLoop));
        assert(DAGAlgsPar< Settings >::isDAG(lg));
    }
    return res;
}

bool interval(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::interval(g);
    cout << int(res);
    return res;
}

bool intervalRepr(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    Settings::AssocCont<MyGraph::PVertex,Segment >::Type vmap;
    bool res=IsItPar< Settings >::Interval::graph2segs(g,vmap);
    cout << int(res);
    if (res)
        for( MyGraph::PVertex v=g.getVert();v!=g.getVertLast();v=g.getVertNext(v))
            for( MyGraph::PVertex u=g.getVertNext(v);u;u=g.getVertNext(u))
            {
                assert(((bool)g.getEdge(u,v))==touch(vmap[u],vmap[v]));
            }
    return res;
}

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

bool split(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::split(g);
    cout << int(res);
    return res;
}

bool cograph(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    IO::readG6(g,g6);
    bool res=IsItPar< Settings >::cograph(g);
    cout << int(res);
    return res;
}

//int mis(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    MaxStablePar< Settings >::get(g,setInserter(res));
//    assert(MaxStablePar< Settings >::test(g,res.begin(),res.end()));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int mis_SimpleOut(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    MaxStablePar< Settings >::get(g,setInserter(res));
//    assert(MaxStablePar< Settings >::test(g,res.begin(),res.end()));
//    cout << res.size();
//    return res.size();
//}
//
//int clique(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    g.neg(EdUndir);
//    MaxStablePar< Settings >::get(g,setInserter(res));
//    assert(MaxStablePar< Settings >::test(g,res.begin(),res.end()));
//    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
//        ires+=g.vertPos(i);
//    cout << ires;
//    return res.size();
//}
//
//int clique_simpleOut(const char* g6)
//{
//    MyGraph g;
//    g.makeAdjMatrix();
//    Set<MyGraph::PVertex> res;
//    Set<int> ires;
//    IO::readG6(g,g6);
//    g.neg(EdUndir);
//    MaxStablePar< Settings >::get(g,setInserter(res));
//    assert(MaxStablePar< Settings >::test(g,res.begin(),res.end()));
//    cout << res.size();
//    return res.size();
//}

int misBipartite(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    Set<MyGraph::PVertex> res;
    Set<int> ires;
    IO::readG6(g,g6);
    IsItPar< Settings >::Bipartite::maxStable(g,setInserter(res));
    assert(MaxStableHeurPar< Settings >::test(g,res.begin(),res.end()));
    for(MyGraph::PVertex i=res.first();i;i=res.next(i))
        ires+=g.vertPos(i);
    cout << ires;
    return res.size();
}

int misBipartite_SimpleOut(const char* g6)
{
    MyGraph g;
    g.makeAdjMatrix();
    Set<MyGraph::PVertex> res;
    Set<int> ires;
    IO::readG6(g,g6);
    IsItPar< Settings >::Bipartite::maxStable(g,setInserter(res));
    assert(MaxStableHeurPar< Settings >::test(g,res.begin(),res.end()));
    cout << res.size();
    return res.size();
}


#include "../main.hpp"

    char napis[20],napis2[20];

    {   cout <<"Undirected (g6 or matrix format):\n";
        MyGraph g;
        MyGraph::PVertex A,B,C,D;
//        A=g.addVert(0);B=g.addVert(1);C=g.addVert(2);D=g.addVert(3);
//        g.addEdge(C,D);g.addEdge(A,B);g.addEdge(B,C);
//        g.addEdge(A,D);
//
//        g.addEdge(C,A);
//        g.addEdge(B,D);

//        IO::writeG6(g,napis,20);
        const char* napis="GR@CHS";



IO::readG6(g,"UTKkQZdx[n?QAGhmJt@fLRXWBr}rHTYaK_xG@IWO");
            Set<MyGraph::PVertex> res;
            A=g.addVert();//B=g.addVert();
//            cout << MaxStableHeurPar< Settings >::get(g,setInserter(res)) << endl;
            cout << res <<endl <<g.getVertSet()<<endl<<endl;

//        g.clear();
//        kruskal("{{0,3,2},{3,0,5},{2,5,1}}");
//        dijkstraPairUndir("{{0,3,2},{3,0,5},{2,5,1}}");
        IO::writeGraphText(g, cout, IO::RG_VertexLists | IO::RG_EInfo);
//        mis("GR@CHS");

//componentsLex("j~~~~~~~~~v~~~~~~~~~lz~~~~|~~~~~~~~~~|~~~~}~v~||~~~~~~~~^z~~~~~v~~~~~~~~\
//|~~~n~~~~~~~~~^~~~~~~~~~n~~~~~~~~~~~~~~~|n~~~}^~z~~|~~~~~~~~~~~~~~~~~~\
//z~~n~~~~~w");
//IO::readG6(g,"M???O?KGA_@?_?O??");
//cout << boolalpha << IsIt::bipartite(g);
//mis_SimpleOut("UTKkQZdx[n?QAGhmJt@fLRXWBr}rHTYaK_xG@IWO");
//matching_SimpleOut("__??O@?C?SO???WG?OD@G?`_@??os?oG??C??J??_???_GQSOB?OCW@C_?_?_?????_OG?EO?G?AG???WAac");

    }


}

