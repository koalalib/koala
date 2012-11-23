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
//#define KOALA_SET_ON_HASHSET

#include <string>
#include <iostream>
#include <sstream>

//#define KOALA_DONT_USE_ALLOCA

#include "../../Koala/graph/graph.h"
#include "../../Koala/graph/view.h"
#include "../../Koala/classes/detect.h"
#include "../../Koala/classes/create.h"
#include "../../Koala/algorithm/mis.h"
#include "../../Koala/io/g6.h"
#include "../../Koala/io/text.h"


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



#include "../../main.hpp"

    char napis[20],napis2[20];

    {   cout <<"Undirected (g6 or matrix format):\n";
        MyGraph g;
        MyGraph::PVertex A,B,C,D,tabv[10]={0,0,0,0,0,0,0};


        IO::readG6(g,"FhCKG");

        IO::writeGraphText(g, cout, IO::RG_VertexLists );


    cout <<  "\n" << g.getVertSet() << "\n" << MISPar< Settings >::get(g,tabv) << "\n";
     for(int i=0;i<10;i++) cout << tabv[i] << ' ';


    }

}

