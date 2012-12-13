#include<iostream>
#include<string.h>

#include "../../Koala/io/g6.h"
#include "../../Koala/graph/graph.h"
#include "../../Koala/graph/view.h"
#include "../../Koala/algorithm/search.h"


namespace Koala {

template <class DefaultStructs> class IsPar
{

    protected:

    class Planar {
        public:

          template <class Cont>
            struct AssocArrSwitch {

                typedef Cont Type;
            };

            template <class K, class E, class Cont>
            struct AssocArrSwitch<AssocArray<K,E,Cont> > {

                typedef AssocTable<std::map< K, E> > Type;
            };


        template <class GraphType>
        struct DFSOutput
        {
            int licznik;
            int cflag;
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex, char>::Type cycle;
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex, int>::Type num;
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex, typename GraphType::PVertex>::Type father;

            DFSOutput(int n) : licznik(0), cflag(0), cycle(n), num(n), father(n)
            { }
        };

        template <class GraphType>
         static void dfs(typename GraphType::PVertex v,typename GraphType::PVertex u,GraphType &sg,DFSOutput<GraphType> &dfso)
         {
             dfso.num[v]=dfso.licznik;
             dfso.licznik++;

             typename GraphType::PVertex w;
             for(typename GraphType::PEdge e=sg.getEdge(v,EdUndir);(e!=NULL)&&(dfso.cflag!=1);e=sg.getEdgeNext(v,e,EdUndir))
             {
                 w=sg.getEdgeEnd(e,v);
                 if(!dfso.num.hasKey(w))
                 {
                     dfso.father[w]=v;
                     dfs(w,v,sg,dfso);

                 }
                 else if((dfso.num[w]<dfso.num[v])&&(w!=u))
                 {
                         dfso.cflag=1;
                         dfso.cycle[w];
                         while(v!=w)
                         {
                             dfso.cycle[v];
                             v=dfso.father[v];
                         }
                         return;
                  }
             }
         }

        template <class SubgraphA>
        static bool bicomponentplanarity(SubgraphA &sg)
        {

            typename SubgraphA::PVertex x0=NULL,x=sg.getVert();
            int n=sg.getVertNo(),m=sg.getEdgeNo();
            DFSOutput<SubgraphA> dfso(n);
//            dfso.licznik=0;
//            dfso.cflag=0;
            dfso.father[x]=x0;
            dfs(x,x0,sg,dfso);

            typename DefaultStructs::template AssocCont<typename SubgraphA::PVertex, char>::Type & hv=dfso.cycle;
            typename DefaultStructs::template AssocCont<typename SubgraphA::PVertex, char>::Type
                hvcr(n), componentV(n), vis(n), fragmV(n), face1(n), face2(n);


            typename DefaultStructs::template AssocCont<typename SubgraphA::PEdge, char>::Type
                he(m), componentE(m), fragmE(m);

            std::vector<typename DefaultStructs::template AssocCont<typename SubgraphA::PVertex, char>::Type > faces;
            std::vector<typename DefaultStructs::template AssocCont<typename SubgraphA::PVertex,char>::Type > fragmentsV;
            std::vector<typename DefaultStructs::template AssocCont<typename SubgraphA::PEdge,char>::Type > fragmentsE;
            std::vector<Set<typename SubgraphA::PVertex> > attachmentsV;


            for(typename SubgraphA::PVertex v=hv.firstKey();v!=NULL;v=hv.nextKey(v))
            {
                if(hv.nextKey(v)!=NULL)
                   he[sg.getEdge(v,hv.nextKey(v),EdUndir)];
                else
                   he[sg.getEdge(v,hv.firstKey(),EdUndir)];
            }

            faces.push_back(hv);
            faces.push_back(hv);

            for(typename SubgraphA::PVertex v=sg.getVert();v!=NULL;v=sg.getVertNext(v))
                if(!hv.hasKey(v))
                    hvcr[v];

        //    AssocArray<typename SubgraphA::PEdge, int> heinc;
            for(typename SubgraphA::PEdge e=sg.getEdge(EdUndir);e!=NULL;e=sg.getEdgeNext(e,EdUndir))
                if(!he.hasKey(e)&&hv.hasKey(sg.getEdgeEnd1(e))&&hv.hasKey(sg.getEdgeEnd2(e)))
                {
        //            heinc[e];
                    componentV[sg.getEdgeEnd1(e)];
                    componentV[sg.getEdgeEnd2(e)];
                    fragmentsV.push_back(componentV);
                    attachmentsV.push_back(sg.getVertSet(extAssocKeyChoose(&componentV)));
                    componentV.clear();
                    componentE[e];
                    fragmentsE.push_back(componentE);
                    componentE.clear();
                }

            Subgraph<SubgraphA,AssocHasChooser<typename DefaultStructs::template AssocCont<typename SubgraphA::PVertex, char>::Type *> ,BoolChooser> sgcr=makeSubgraph(sg,std::make_pair(extAssocKeyChoose(&hvcr),stdChoose(true)));

            typename SubgraphA::PVertex LOCALARRAY(tabV,n);
            typename SubgraphA::PEdge LOCALARRAY(tabE,m);


            int componentsize=0;
            int edgenumber=0;
            typename SubgraphA::PVertex vcf;
            Set<typename SubgraphA::PVertex> attachV;
            for(typename SubgraphA::PVertex v=sgcr.getVert();v!=NULL;v=sgcr.getVertNext(v))
            {
                if(!vis.hasKey(v))
                {
                    componentsize=DFSPreorderPar<DefaultStructs>::scanAttainable(sgcr,v,tabV,EdUndir);
                    for(int i=0;i<componentsize;i++)
                    {
                        vis[tabV[i]];
                        componentV[tabV[i]];
                         for(typename SubgraphA::PEdge e=sg.getEdge(tabV[i],EdUndir);e!=NULL;e=sg.getEdgeNext(tabV[i],e,EdUndir))
                            if(hv.hasKey(vcf=sg.getEnd(e,tabV[i])))
                            {
                               attachV+=vcf;
                               componentV[vcf];
                               componentE[e];
                    }

                    }

                    edgenumber=sgcr.getIncEdges(tabE,tabV,tabV+componentsize,EdUndir);
                    for(int i=0;i<edgenumber;i++)
                        componentE[tabE[i]];

                    attachmentsV.push_back(attachV);
                    fragmentsV.push_back(componentV);
                    fragmentsE.push_back(componentE);

                    attachV.clear();
                    componentV.clear();
                    componentE.clear();
                }
            }

            int pathlength=0;
            int fragmentnumber=fragmentsV.size()-1;
            Subgraph<SubgraphA,AssocHasChooser<typename DefaultStructs::template AssocCont<typename SubgraphA::PVertex, char>::Type *> ,AssocHasChooser<typename DefaultStructs::template AssocCont<typename SubgraphA::PEdge, char>::Type*> > sgp; //podgraf do szukania sciezki
            if(fragmentsV.empty()==true)
              return true;
            else
            {
                if(fragmentsV[fragmentnumber].size()==2)
                {
                    pathlength=1;
                    tabV[0]=fragmentsV[fragmentnumber].firstKey();
                    tabV[1]=fragmentsV[fragmentnumber].lastKey();
                    tabE[0]=fragmentsE[fragmentnumber].firstKey();
                }
                else
                {
                  fragmV=fragmentsV[fragmentnumber];
                  for(typename SubgraphA::PVertex v=attachmentsV[fragmentnumber].next(attachmentsV[fragmentnumber].first());v!=attachmentsV[fragmentnumber].last();v=attachmentsV[fragmentnumber].next(v))
                      fragmV.delKey(v);
                  fragmE=fragmentsE[fragmentnumber];
                  sgp=makeSubgraph(sg,std::make_pair(extAssocKeyChoose(&fragmV),extAssocKeyChoose(&fragmE)));
                  pathlength=Koala::DFSPreorderPar<DefaultStructs>::getPath(sgp,attachmentsV[fragmentnumber].first(),attachmentsV[fragmentnumber].last(),Koala::PathStructs::outPath(tabV,tabE),EdAll);
                }
            }

                  int facenumber=faces.size()-1;
                  typename SubgraphA::PVertex pv;
                  for(pv=faces[facenumber].firstKey();(pv!=tabV[0])&&(pv!=tabV[pathlength]);pv=faces[facenumber].nextKey(pv))
                         face1[pv];
                  if(pv==tabV[0])
                  {
                          for(int i=0;i<pathlength;i++)
                              face1[tabV[i]];
                          for(typename SubgraphA::PVertex v=tabV[pathlength];v!=NULL;v=faces[facenumber].nextKey(v))
                                face1[v];
                          for(typename SubgraphA::PVertex v=tabV[0];v!=tabV[pathlength];v=faces[facenumber].nextKey(v))
                                face2[v];
                          for(int i=pathlength;i>0;i--)
                                face2[tabV[i]];
                  }
                  else
                  {
                          for(int i=pathlength;i>0;i--)
                                face1[tabV[i]];
                          for(typename SubgraphA::PVertex v=tabV[0];v!=NULL;v=faces[facenumber].nextKey(v))
                                face1[v];
                          for(typename SubgraphA::PVertex v=tabV[pathlength];v!=tabV[0];v=faces[facenumber].nextKey(v))
                                face2[v];
                          for(int i=0;i<pathlength;i++)
                              face2[tabV[i]];
                  }

                  faces[facenumber]=face1;
                  faces.push_back(face2);

            int k;
            int fb;
            typename std::vector<Set<typename SubgraphA::PVertex> >::iterator itat;
            Set<typename SubgraphA::PVertex> faceset;

            for(int f=3;f<m-n+2;f++)
            {
                for(int i=0;i<pathlength;i++)
                {
                    he[tabE[i]];
                    fragmentsE[fragmentnumber].delKey(tabE[i]);
                }

                if(pathlength>1)
                {
                    fragmE=fragmentsE[fragmentnumber];

                    for(int i=1;i<pathlength;i++)
                    {
                        hv[tabV[i]];
                        fragmentsV[fragmentnumber].delKey(tabV[i]);
                    }

                    for(typename SubgraphA::PVertex v=attachmentsV[fragmentnumber].first();v!=attachmentsV[fragmentnumber].last();v=attachmentsV[fragmentnumber].next(v))
                        fragmentsV[fragmentnumber].delKey(v);
                    fragmentsV[fragmentnumber].delKey(attachmentsV[fragmentnumber].last());

                    for(typename SubgraphA::PEdge e=fragmentsE[fragmentnumber].firstKey();e!=NULL;e=fragmentsE[fragmentnumber].nextKey(e))
                    {
                        if(!he.hasKey(e)&&hv.hasKey(sg.getEdgeEnd1(e))&&hv.hasKey(sg.getEdgeEnd2(e)))
                        {
                           attachV.clear();
                           componentV.clear();
                           componentE.clear();
                           attachV+=sg.getEdgeEnd1(e);
                           attachV+=sg.getEdgeEnd2(e);
                           componentV[sg.getEdgeEnd1(e)];
                           componentV[sg.getEdgeEnd2(e)];
                           componentE[e];

                           fragmE.delKey(e);

                           if(!componentV.empty())
                           {
                                fragmentsV.push_back(componentV);
                                fragmentsE.push_back(componentE);
                                attachmentsV.push_back(attachV);
                           }
                       }
                     }


                     vis.clear();
                     fragmV=fragmentsV[fragmentnumber];
                     sgp=makeSubgraph(sg,std::make_pair(extAssocKeyChoose(&fragmV),extAssocKeyChoose(&fragmE)));

                     for(typename SubgraphA::PVertex v=sgp.getVert();v!=NULL;v=sgp.getVertNext(v))
                     {
                             if(!vis.hasKey(v))
                             {

                                attachV.clear();
                                componentV.clear();
                                componentE.clear();
                                componentsize=DFSPreorderPar<DefaultStructs>::scanAttainable(sgp,v,tabV,EdUndir);
                                for(int i=0;i<componentsize;i++)
                                {
                                    vis[tabV[i]];
                                    componentV[tabV[i]];
                                    for(typename SubgraphA::PEdge e=sg.getEdge(tabV[i],EdUndir);e!=NULL;e=sg.getEdgeNext(tabV[i],e,EdUndir))
                                       if(hv.hasKey(vcf=sg.getEnd(e,tabV[i])))
                                       {
                                             attachV+=vcf;
                                             componentV[vcf];
                                             componentE[e];
                               }
                                }

                                edgenumber=sgp.getIncEdges(tabE,tabV,tabV+componentsize,EdUndir);
                                for(int i=0;i<edgenumber;i++)
                                    componentE[tabE[i]];
                                if(!componentV.empty())
                                {
                                    fragmentsV.push_back(componentV);
                                    fragmentsE.push_back(componentE);
                                    attachmentsV.push_back(attachV);
                                }
                             }
                      }


                }


                if(fragmentnumber!=fragmentsV.size()-1)
                {
                    fragmentsV[fragmentnumber]=fragmentsV.back();
                    fragmentsE[fragmentnumber]=fragmentsE.back();
                    attachmentsV[fragmentnumber]=attachmentsV.back();
                }

                fragmentsV.pop_back();
                fragmentsE.pop_back();
                attachmentsV.pop_back();

                k=0;
                fragmentnumber=0;
                for(itat=attachmentsV.begin();itat!=attachmentsV.end();++itat)
                {
                    fb=0;
                    facenumber=0;
                    for(int j=0;j<faces.size();j++)
                    {
                        faceset=sg.getVertSet(extAssocKeyChoose(&faces[j]));
                        if((*itat).subsetOf(faceset))
                        {
                            fb++;
                            facenumber=j;
                            fragmentnumber=k;
                        }
                    }
                    if(fb==0)
                        return false;
                    if(fb==1)
                        break;
                    k++;
                }

                pathlength=0;
                if(fragmentsV[fragmentnumber].size()==2)
                {
                    pathlength=1;
                    tabV[0]=fragmentsV[fragmentnumber].firstKey();
                    tabV[1]=fragmentsV[fragmentnumber].lastKey();
                    tabE[0]=fragmentsE[fragmentnumber].firstKey();
                }
                else
                {
                    fragmV=fragmentsV[fragmentnumber];
                    fragmE=fragmentsE[fragmentnumber];
                    for(typename SubgraphA::PVertex v=attachmentsV[fragmentnumber].next(attachmentsV[fragmentnumber].first());v!=attachmentsV[fragmentnumber].last();v=attachmentsV[fragmentnumber].next(v))
                        fragmV.delKey(v);
                    sgp=makeSubgraph(sg,std::make_pair(extAssocKeyChoose(&fragmV),extAssocKeyChoose(&fragmE)));
                    pathlength=Koala::DFSPreorderPar<DefaultStructs>::getPath(sgp,attachmentsV[fragmentnumber].first(),attachmentsV[fragmentnumber].last(),Koala::PathStructs::outPath(tabV,tabE),EdAll);
                }

                face1.clear();
                face2.clear();
                for(pv=faces[facenumber].firstKey();(pv!=tabV[0])&&(pv!=tabV[pathlength]);pv=faces[facenumber].nextKey(pv))
                    face1[pv];
                if(pv==tabV[0])
                {
                    for(int i=0;i<pathlength;i++)
                        face1[tabV[i]];
                    for(typename SubgraphA::PVertex v=tabV[pathlength];v!=NULL;v=faces[facenumber].nextKey(v))
                        face1[v];
                    for(typename SubgraphA::PVertex v=tabV[0];v!=tabV[pathlength];v=faces[facenumber].nextKey(v))
                        face2[v];
                    for(int i=pathlength;i>0;i--)
                        face2[tabV[i]];
                }
                else
                {
                    for(int i=pathlength;i>0;i--)
                        face1[tabV[i]];
                    for(typename SubgraphA::PVertex v=tabV[0];v!=NULL;v=faces[facenumber].nextKey(v))
                        face1[v];
                    for(typename SubgraphA::PVertex v=tabV[pathlength];v!=tabV[0];v=faces[facenumber].nextKey(v))
                        face2[v];
                    for(int i=0;i<pathlength;i++)
                        face2[tabV[i]];
                  }

                  faces[facenumber]=face1;
                  faces.push_back(face2);

            }
            return true;
        }
    };

    public:

    template <class Graph>
    static bool planar(const Graph &g)
    {

        //if (!undir( g,true )) return false;
        typename Graph::PEdge e;
        typename Graph::PVertex LOCALARRAY(tabV,g.getVertNo()+g.getEdgeNo(EdUndir));
        int LOCALARRAY(tabcomp,g.getVertNo()+1);
        typename DefaultStructs:: template AssocCont< typename Graph::PEdge,char >::Type eds(g.getEdgeNo(EdUndir));

        makeSubgraph(g,stdChoose(true) & edgeTypeChoose(EdUndir)).findParals(std::make_pair(assocInserter(eds,constFun('A')),blackHole));

        int liczbaBlokow=BlocksPar<DefaultStructs>::split(makeSubgraph(g,stdChoose(true)&extAssocKeyChoose(&eds)),blackHole,blackHole,SearchStructs::compStore(tabcomp,tabV),blackHole);

        typename DefaultStructs:: template AssocCont< typename Graph::PVertex,char >::Type subgrTab(g.getVertNo());

        int liczbaWierzcholkow=0;
        int liczbaKrawedzi=0;

        for(int i=0;i<liczbaBlokow;i++)
        {
            subgrTab.clear();
            for(int j=tabcomp[i];j<tabcomp[i+1];j++) subgrTab[tabV[j]];

            typename DefaultStructs::template LocalGraph< char,char,Undirected>::Type sg;
            sg.copy(g,std::make_pair(extAssocKeyChoose(&subgrTab),extAssocKeyChoose(&eds)));
            liczbaWierzcholkow=sg.getVertNo();
            liczbaKrawedzi=sg.getEdgeNo();
            if((liczbaWierzcholkow>=5)&&(liczbaKrawedzi>=9))
            {
                if(liczbaKrawedzi>(3*liczbaWierzcholkow-6))
                    return false;
                if(Planar::bicomponentplanarity(sg)==false)
                    return(false);
            }
        }
        return true;
    }

};

}

using namespace std;
using namespace Koala;
using namespace Koala::IO;

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


#define ArrayMapTest
//#define StdMapTest
//#define HashMapTest


#if defined(ArrayMapTest)
#define  Settings DefSettings
#endif
#if defined(StdMapTest)
#define  Settings StdMapSettings
#endif
#if defined(HashMapTest)
#define  Settings HashMapSettings
#endif


int main(int argc, char *argv[]) {
    Graph<char,std::string> g;
   // string txt=argv[1];
    //readG6(g, txt);
   Graph<char,std::string>::PVertex A,B,C,D,E,F,G,H,I;//,J,K,L; // to beda wierzcholki

     A=g.addVert('A');
     cout << "1:"<<IsPar<Settings>::planar(g) << endl;
     B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
     E=g.addVert('E');F=g.addVert('F');G=g.addVert('G');H=g.addVert('H');
     I=g.addVert('I');//J=g.addVert('J');K=g.addVert('K');L=g.addVert('L');


     g.putVert(g.addEdge(A,B,"ab",EdUndir));
     cout << "1:"<<IsPar<Settings>::planar(g) << endl;
     g.addEdge(B,C,"bc",EdUndir);g.addEdge(A,C,"ac",EdUndir);
     g.addEdge(D,A,"da",EdUndir);g.addEdge(D,B,"db",EdUndir);g.addEdge(D,C,"dc",EdUndir);
     g.addEdge(A,E,"ae",EdUndir);g.addEdge(B,E,"be",EdUndir);g.addEdge(E,C,"ec",EdUndir);g.addEdge(E,D,"ed",EdUndir);
   cout << "0:"<<IsPar<Settings>::planar(g) << endl;

   g.del(A);
   cout << "1:"<<IsPar<Settings>::planar(g) << endl;
   {    g.clear();
        const char* napis="GFBkpS";
        IO::readG6(g,napis);
        cout << "1:"<<IsPar<Settings>::planar(g) << endl;
   }
   {    g.clear();
        const char* napis="HG@VgLM";
        IO::readG6(g,napis);
        cout << "1:"<<IsPar<Settings>::planar(g) << endl;
   }
   {    g.clear();
        const char* napis="ICxLWKGj_";
        IO::readG6(g,napis);
        cout << "0:"<<IsPar<Settings>::planar(g) << endl;
   }
   {    g.clear();
        const char* napis="HeewBv?";
        IO::readG6(g,napis);
        cout << "0:"<<IsPar<Settings>::planar(g) << endl;
   }


}

/*
 UWAGI:
 * W szablonie dla grafów skierowanych będzie trzeba zignorowac skierowania krawedzi oraz pętle.
 */
