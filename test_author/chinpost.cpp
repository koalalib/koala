#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include "Koala/algorithm/weights.h"
#include "Koala/classes/detect.h"
#include"./Koala/io/text.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

Koala::Graph<std::string,char> g;
Koala::Graph<std::string,char>::PVertex A,B,C,D,E,F,V,U,tabV[20];
Koala::Graph<std::string,char>::PEdge tabE[20];




Koala::AssocArray<Koala::Graph<std::string,char>::PEdge,Koala::Dijkstra::EdgeLabs<int> >  edgeCont;


//Koala::SimpleAssocMatrix<Koala::Graph<std::string,char>::PVertex,//int,
//    Koala::Dijkstra::VertLabs<int, Koala::Graph<std::string,char> >,
//        //Koala::Dijkstra::VertLabs<int, Koala::Graph<std::string,char> >,
//            Koala::AMatrTriangle> twoDimVertCont;
//void chinTest()
//{
//    g.clear();
//    A=g.addVert("A");B=g.addVert("B");C=g.addVert("C");D=g.addVert("D");
//    E=g.addVert("E");F=g.addVert("F");
//    for(U=g.getVert();U!=g.getVertLast();U=g.getVertNext(U))
//        for(V=g.getVertNext(U);V;V=g.getVertNext(V)) twoDimVertCont(U,V).distance=0;
//    twoDimVertCont(A,B).distance=3;
//    twoDimVertCont(A,C).distance=2;twoDimVertCont(A,D).distance=2;
//    twoDimVertCont(C,B).distance=2;twoDimVertCont(D,B).distance=1;twoDimVertCont(C,D).distance=1;
//}




void chinTest1()
{
    g.clear(); edgeCont.clear();
    Koala::Graph<std::string,char>::PVertex
        ab=g.addVert("ab"), bc=g.addVert("bc"), cd=g.addVert("cd"), b=g.addVert("b"),
        d=g.addVert("d")
        ;

    edgeCont[g.addLink(ab,bc,'A')].length=7;
    edgeCont[g.addLink(bc,b,'B')].length=3;
    edgeCont[g.addLink(ab,cd,'C')].length=3;
    edgeCont[g.addLink(cd,d,'D')].length=4;

    V=ab;
}

void chinTest2()
{
    g.clear(); edgeCont.clear();
    Koala::Graph<std::string,char>::PVertex
        abd=g.addVert("abd"), abc=g.addVert("abc"), ceg=g.addVert("ceg"), def=g.addVert("def"),
        fhi=g.addVert("fhi"), ghj=g.addVert("ghj"), ijk=g.addVert("ijk"), k=g.addVert("k")
        ;

    edgeCont[g.addLink(abd,abc,'A')].length=7;
    edgeCont[g.addLink(abd,abc,'B')].length=3;
    edgeCont[g.addLink(abc,ceg,'C')].length=3;
    edgeCont[g.addLink(abd,def,'D')].length=4;
    edgeCont[g.addLink(def,ceg,'E')].length=5;
    edgeCont[g.addLink(def,fhi,'F')].length=1;
    edgeCont[g.addLink(ghj,ceg,'G')].length=8;
    edgeCont[g.addLink(fhi,ghj,'H')].length=1;
    edgeCont[g.addLink(fhi,ijk,'I')].length=6;
    edgeCont[g.addLink(ghj,ijk,'J')].length=1;
    edgeCont[g.addLink(k,ijk,'K')].length=3;

    V=ghj;

}

void chinTest3()
{
    g.clear(); edgeCont.clear();
    Koala::Graph<std::string,char>::PVertex
        ab=g.addVert("ab"), bc=g.addVert("bc"), cd=g.addVert("cd"), da=g.addVert("da"),
        ef=g.addVert("ef"), fg=g.addVert("fg"), gh=g.addVert("gh")
//        fhi=g.addVert("fhi"), ghj=g.addVert("ghj"), ijk=g.addVert("ijk"), k=g.addVert("k")
        ;

    edgeCont[g.addLink(ab,bc,'B')].length=7;
    edgeCont[g.addLink(bc,cd,'C')].length=3;
    edgeCont[g.addLink(cd,da,'D')].length=3;
    edgeCont[g.addLink(da,ab,'A')].length=4;
   edgeCont[g.addLink(ab,ef,'E')].length=2;
   edgeCont[g.addLink(fg,ef,'F')].length=3;
   edgeCont[g.addLink(fg,gh,'G')].length=1;
   edgeCont[g.addLink(ef,gh,'H')].length=2;
   edgeCont[g.addLoop(gh,'P')].length=3;
   V=da;

}

template <class Vert,class AssocMatr>
void minMatchRek( Vert *tabv,int m, const AssocMatr& weights, int& res, int vpos, int respos,
                 int curres,std::pair<int,int>* best, std::pair<int,int>* current,bool* matched)
{

    int u,v;
    if (respos==m)
    {
        for(int i=0;i<m;i++) best[i]=current[i];
        res=curres;
        return;
    }
    for( v=vpos;matched[v];v++);
    for(u=v+1; u<2*m;u++) if (!matched[u])
    {
        int newres=curres+weights(tabv[u],tabv[v]).distance;
        if (newres>=res) continue;
        matched[u]=matched[v]=true;
        current[respos]=std::make_pair(u,v);
        minMatchRek(tabv,m,weights,res,v+1,respos+1,newres,best,current,matched);
        matched[u]=matched[v]=false;
    }
}

template <class Vert,class AssocMatr,class IterOut>
int minMatch( Vert tabv[],int m, const AssocMatr& weights, IterOut out)
{
    if (!m) return 0;
    int res=std::numeric_limits< int >::max();
    std::pair<int,int> LOCALARRAY(best,m);
    std::pair<int,int> LOCALARRAY(current,m);
    bool LOCALARRAY(matched,2*m);
    for(int i=0;i<2*m;i++) matched[i]=false;
    minMatchRek(tabv,m,weights,res,0,0,0,best,current,matched);
    for(int i=0;i<m;i++)
        { * out=best[i]; ++ out; }
    return res;
}

template <class Graph,class EdgeCont,class VIter,class EIter>
std::pair<int,int> // first=waga cyklu, second=liczba krawedzi
chinPost(Graph& g, EdgeCont& eCont, typename Graph::PVertex start, PathStructs::OutPath<VIter,EIter> out)
{
    assert(g.getVertNo() && g.getEdgeNo() && !g.getEdgeNo(Directed) && IsIt::connected(g,true) && g.has(start));
    std::pair<int,int> res;
    typename Graph::PVertex LOCALARRAY(oddv,g.getVertNo());
    int oddn=0;
    for( typename Graph::PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (g.deg(v) & 1) oddv[oddn++]=v;
    Set<typename Graph::PEdge > enews;
    Koala::SimpleAssocMatrix<typename Graph::PVertex,Koala::Dijkstra::VertLabs<int, Graph >, Koala::AMatrFull>
            twoDimVertCont(g.getVertNo() * (oddn!=0));
    typename Graph::PVertex LOCALARRAY(pathv,g.getVertNo());
    typename Graph::PEdge LOCALARRAY(pathe,g.getVertNo()-1);
    if (oddn)
    {
        std::pair<int,int> LOCALARRAY(pairs,oddn/2);
        Koala::All2AllDists::floyd(g,twoDimVertCont,eCont);
        res.first=minMatch(oddv,oddn/2,twoDimVertCont,pairs);
        for(int i=0;i<oddn/2;i++)
        {
            typename Graph::PEdge e=g.addLink(oddv[pairs[i].first],oddv[pairs[i].second]);
            enews+=e; eCont[e].length=twoDimVertCont(oddv[pairs[i].first],oddv[pairs[i].second]).distance;
        }
    }
    typename Graph::PVertex LOCALARRAY(eulerv,g.getEdgeNo()+1);
    typename Graph::PEdge LOCALARRAY(eulere,g.getEdgeNo());
    assert(Euler::getCycle(g,start,PathStructs::outPath(eulerv,eulere)));
    *out.vertIter=start; ++out.vertIter;
    for(int i=0;i<g.getEdgeNo();i++)
        if (!enews.isElement(eulere[i]))
        {
            *out.vertIter=eulerv[i+1]; ++out.vertIter;
            *out.edgeIter=eulere[i]; ++out.edgeIter;
            res.second++; res.first+=eCont[eulere[i]].length;
        } else
        {
            eCont.delKey(eulere[i]);
            int l=Koala::All2AllDists::getPath(g,twoDimVertCont,eulerv[i],eulerv[i+1],Koala::All2AllDists::outPath(pathv,pathe));
            res.second+=l;
            for(int j=0;j<l;j++)
            {
                *out.vertIter=pathv[j+1]; ++out.vertIter;
                *out.edgeIter=pathe[j]; ++out.edgeIter;
            }
        }
    g.delEdges(enews.begin(),enews.end());
    return res;
}

#include "main.hpp"


    chinTest2();
    writeGraphText(g, cout, RG_EdgeList|RG_Info);

    Koala::PathStructs::OutPathTool<Koala::Graph<std::string,char>, std::vector > chinpath;
    std::pair<int,int> wynik=chinPost(g,edgeCont,V,chinpath.input());
    cout << "\n\nWaga:" << wynik.first << " dlugosc:" << wynik.second << endl;
    for(int i=0;i<chinpath.length();i++)
        cout << i << ':' << ' ' << chinpath.vertex(i)->info << "  " << chinpath.edge(i)->info << '(' << edgeCont[chinpath.edge(i)].length<< ')'<< endl;

    return 0;
}
