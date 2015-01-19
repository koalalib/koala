#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <cassert>
#include "Koala/graph/graph.h"
#include"./Koala/io/text.h"
#include "Koala/algorithm/search.h"
#include "Koala/algorithm/weights.h"
#include "Koala/algorithm/matching.h"
#include "Koala/coloring/vertex.h"
#include "Koala/coloring/edge.h"
#include "Koala/classes/detect.h"
#include "Koala/classes/create.h"
#include "Koala/coloring/interval.h"


using namespace std;
using namespace Koala;
using namespace Koala::IO;

typedef Graph<char,string> MyGraph;
typedef Graph<char,string>::PVertex PVertex;
typedef Graph<char,string>::PEdge PEdge;


template <class T>
bool printInfo(T arg)
{ if (arg) cout << arg->info; else cout <<"NULL"; }


//Dla czegos z Koali, co moze byc udostepnione jako serwis, mamy 2 procedury: nazwaExamp - odpowiada serwisowi, oraz nazwaMain tj. jego
//wywolanie dla przykladowych danch i obsluga wynikow. Serwis stara sie wykrywac mozliwe bledy w danych (tu warunki sa czasem ostrzejsze
//niz w bibliotece). costamExamp czasem dubluje zwracane wyniki np. podaje je w postaci ciagu i mapy. Idea byla taka,ze gdy ta procedura
//zwraca cos w mapie, ta wartosc jest traktowana jako cecha wierz/kraw. i moze byc wyswietlana w Zgredzie, a nie tylko jego konsoli
//tekstowej. Przynajmniej na poczatku staralem sie tego trzymac

struct paramsRes {
    int n,m,mdir,mundir,mloop,Delta,DeltaIn,DeltaOut,delta,deltaIn,deltaOut,maxmu;
    PVertex VDelta,VDeltaIn,VDeltaOut,Vdelta,VdeltaIn,VdeltaOut;
    PEdge Emaxmu;
};

paramsRes paramsExamp(const MyGraph& g,AssocArray<PVertex,int>& degs, AssocArray<PVertex,int>& degsin,
                      AssocArray<PVertex,int>& degsout, AssocArray<PEdge,int>& mus)
{
    paramsRes res;
    if (!(res.n=g.getVertNo())) return res; //Bledne dane, generalnie zakladam, ze powinny byc podawane grafy z n>0
    res.m=g.getEdgeNo(); res.mundir=g.getEdgeNo(EdUndir);res.mdir=g.getEdgeNo(EdDirIn|EdDirOut);
    res.mloop=g.getEdgeNo(EdLoop);
    std::pair< PVertex,int > pres = g.maxDeg(); res.Delta=pres.second; res.VDelta= pres.first;
    pres = g.maxDeg(EdDirOut); res.DeltaOut=pres.second; res.VDeltaOut= pres.first;
    pres = g.maxDeg(EdDirIn); res.DeltaIn=pres.second; res.VDeltaIn= pres.first;
    pres = g.minDeg(); res.delta=pres.second; res.Vdelta= pres.first;
    pres = g.minDeg(EdDirOut); res.deltaOut=pres.second; res.VdeltaOut= pres.first;
    pres = g.minDeg(EdDirIn); res.deltaIn=pres.second; res.VdeltaIn= pres.first;
    std::pair< PEdge,int > eres=g.maxMu(); res.maxmu=eres.second; res.Emaxmu=eres.first;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v))
    {
        degs[v]=g.deg(v); degsin[v]=g.deg(v,EdDirIn); degsout[v]=g.deg(v,EdDirOut);
    }
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) mus[e]=g.mu(e);
    return res;
}

void paramsMain()
{
    MyGraph g;
    PVertex A,B,C;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    g.addEdge(A,B,"ab");g.addEdge(A,C,"ac");g.addArc(B,C,"bc");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    AssocArray<PVertex,int> degs,degsin,degsout;
    AssocArray<PEdge,int> mus;

    paramsRes res = paramsExamp(g,degs,degsin,degsout,mus);
    if (res.n==0) { cout << "bledne dane"; return; }
    cout << res.n<< ' ' << res.m<< ' ' << res.mdir<< ' ' << res.mundir<< ' ' << res.mloop<< ' ' << res.Delta<< ' ' << res.DeltaIn<< ' ' << res.DeltaOut<< ' ' << res.delta<< ' ' << res.deltaIn<< ' ' << res.deltaOut<< ' ' << res.maxmu;
    printInfo(res.VDelta);printInfo(res.VDeltaIn);printInfo(res.VDeltaOut);printInfo(res.Vdelta);printInfo(res.VdeltaIn);printInfo(res.VdeltaOut); printInfo(res.Emaxmu);
    for(PVertex v=g.getVert();v;v=g.getVertNext(v))
    {
        cout << '\n'<< v->info << ':' << degs[v] << degsin[v] <<degsout[v];
    }
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) cout << '\n'<< e->info << ':' << mus[e];
}

int dfsOneVertExamp(const MyGraph& g,PVertex start, PVertex pre[], PVertex post[],
                    AssocArray<PVertex,DFSPreorder::VisitVertLabs<MyGraph> >&  vlabs)
{
    if (! start || !g.has(start)) return -1; //bledne dane
    int res=DFSPreorder::scanAttainable(g,start,vlabs,pre,EdDirOut|EdUndir);
    DFSPostorder::scanAttainable(g,start,blackHole,post,EdDirOut|EdUndir);
    return res;
}

void dfsOneVertMain()
{
    MyGraph g;
    PVertex A,B,C,D;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
    g.addEdge(A,B,"ab");g.addEdge(A,C,"ac");g.addArc(B,C,"bc");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(pre,g.getVertNo());
    PVertex LOCALARRAY(post,g.getVertNo());
    AssocArray<PVertex,DFSPreorder::VisitVertLabs<MyGraph> >  vlabs;
    int res=dfsOneVertExamp(g,A,pre,post,vlabs);
    if (res==-1) { cout << "bledne dane"; return; }
    cout << res << endl;
    for(int i=0;i<res;i++) printInfo(pre[i]);
    cout << endl;
    for(int i=0;i<res;i++)  printInfo(post[i]);
    cout << "\nDrzewo przeszukiwania wierzcholkow osiagalnych ze start" << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (vlabs.hasKey(v))
    {
        cout << v->info << ": vPrev "; printInfo(vlabs[v].vPrev);
        cout << " ePrev "; printInfo(vlabs[v].ePrev);
        cout << endl;
    }
}

int bfsOneVertExamp(const MyGraph& g,PVertex start, PVertex pre[],AssocArray<PVertex,BFS::VisitVertLabs<MyGraph> >&  vlabs)
{
    if (! start || !g.has(start)) return -1; //bledne dane
    assert(g.has(start));
    int res=BFS::scanAttainable(g,start,vlabs,pre,EdDirOut|EdUndir);
    return res;
}

void bfsOneVertMain()
{
    MyGraph g;
    PVertex A,B,C,D;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
    g.addEdge(A,B,"ab");g.addEdge(A,C,"ac");g.addArc(B,C,"bc");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(pre,g.getVertNo());
    AssocArray<PVertex,BFS::VisitVertLabs<MyGraph> >  vlabs;
    int res=bfsOneVertExamp(g,A,pre,vlabs);
    if (res==-1) { cout << "bledne dane"; return; }
    cout << res << endl;
    for(int i=0;i<res;i++) printInfo(pre[i]);
    cout << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (vlabs.hasKey(v))
    {
        cout << v->info << ": " <<vlabs[v].distance <<" vPrev "; printInfo(vlabs[v].vPrev);
        cout << " ePrev "; printInfo(vlabs[v].ePrev);
        cout << endl;
    }
}

//Nie wiem, czy jest sens udostepniania sciezek z innych strategii - w bfs akurat sa to sciezki najkrotsze
int bfsGetPathExamp(const MyGraph& g,PVertex start, PVertex end, PVertex vout[], PEdge eout[])
{
    if (! start || !g.has(start) || ! end || !g.has(end)) return -2; //bledne dane
    int res=BFS::getPath(g,start,end,BFS::outPath(vout,eout),EdUndir | EdDirOut);
    return res;
}

void bfsGetPathMain()
{
    MyGraph g;
    PVertex A,B,C,D;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
    g.addEdge(A,B,"ab");g.addArc(B,C,"bc");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(vout,g.getVertNo());
    PEdge LOCALARRAY(eout,g.getEdgeNo());
    int res=bfsGetPathExamp(g,A,C,vout,eout);
    if (res==-2) { cout << "bledne dane"; return; }
    if (res==-1) cout << "No";
    else
    {   cout << res << endl;
        for(int i=0;i<res+1;i++) cout << vout[i]->info;
        cout << endl;
        for(int i=0;i<res;i++) cout << eout[i]->info << ' ';

    }
}

struct bfsScanRes
{
    int compno,eforestno,cyclno;
};

//I to samo proponuje  powtorzyc dla BFS -> LexBFS
bfsScanRes bfsScanExamp(const MyGraph& g,PVertex vout[], PEdge eout[], AssocArray<PVertex,int >&  comps,
                        SearchStructs::CompStoreTool<PVertex>::InputType compout)
{   //Zdaje sobie sprawe, ze ten kod wiele razy liczy to samo :-)
    bfsScanRes res;
    if (!g.getVertNo()) { res.compno=-2; return res; }
    AssocArray<PVertex,BFS::VisitVertLabs<MyGraph> >  vlabs;
    res.compno=BFS::scan(g, vlabs,vout,EdUndir | EdDirOut| EdDirIn);    //Podobnie jak w Koali, tu ignorujemy kierunek lukow skanujac wszystko
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) comps[v]=vlabs[v].component;
    res.eforestno=BFS::getUsedEdges(g,vlabs,eout);
    res.cyclno=BFS::cyclNo(g);
    BFS::split(g,blackHole,compout);
    return res;
}

//I to samo proponuje  powtorzyc dla BFS -> LexBFS. Nie wiem, czy jest sens skanowac caly graf innymi strategiami
void bfsScanMain()
{
    MyGraph g;
    PVertex A,B,C,D,E;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D'); E=g.addVert('E');
    g.addEdge(A,B,"ab");g.addArc(D,C,"dc");g.addLoop(B,"bb");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(vout,g.getVertNo());
    PEdge LOCALARRAY(eout,g.getEdgeNo());
    AssocArray<PVertex,int >  sklad1;
    SearchStructs::CompStoreTool<PVertex> sklad2;
    bfsScanRes res=bfsScanExamp(g,vout,eout,sklad1,sklad2.input());
     if (res.compno==-2) { cout << "bledne dane"; return; }
    cout << "compno: " << res.compno << " cyclno: " << res.cyclno << " kraw. lasu spinajacego: " << res.eforestno << endl;
    cout << "Vorder out: ";
    for(int i=0;i<g.getVertNo();i++) cout << vout[i]->info << ' ';
    cout << "las spinajacy: ";
    for(int i=0;i<res.eforestno;i++) cout << eout[i]->info << ' ';
    cout << "\n wierzcholki w skladowych\n";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << sklad1[v] <<'\n';
    cout << "\n Zawartosc skladowych";
    for(int i=0;i<sklad2.size();i++)
    {   cout << '\n'<< i << ':';
        for (int j=0;j<sklad2.size(i);j++) cout << sklad2[i][j]->info << ' ';
    }
}

int isDagExamp(const MyGraph& g,PVertex vout[],  AssocArray<PVertex,int >&  vnum)
{
    if (!g.getVertNo()) return -1;
    DAGAlgs::topOrd(g,vout);
    if (!DAGAlgs::isDAG(g,vout,vout+g.getVertNo())) return false;
    for(int i=0;i<g.getVertNo();i++) vnum[vout[i]]=i;
    return true;
}

void isDagMain()
{
    MyGraph g;
    PVertex A,B,C,D,E;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D'); E=g.addVert('E');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");g.addArc(A,C,"ac");g.addArc(E,D,"ed");
    //g.addArc(C,A,"ed");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(vout,g.getVertNo());
    AssocArray<PVertex,int >  vnum;
    int res=isDagExamp(g,vout,vnum);
    if (res<0) { cout << "bledne dane"; return; }
    if (res==0) { cout << "Nie jest DAG" ; return; }
    cout << "TopOrd: ";
    for(int i=0;i<g.getVertNo();i++) cout << vout[i]->info << ' ';
    cout << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << ":" << vnum[v] << endl;
}

int dagHasseExamp(const MyGraph& g,PEdge eout[],  AssocArray<PEdge,bool >&  vhass)
{
    int res=0;
    if (!DAGAlgs::isDAG(g) || !g.getVertNo()) return -1; //Bledne dane
    DAGAlgs::transEdges(g,assocInserter(vhass,constFun(false)));
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (!vhass.hasKey(e))
    {
        vhass[e]=true;
        eout[res++]=e;
    }
    return res;
}

void dagHasseMain()
{
    MyGraph g;
    PVertex A,B,C,D,E;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D'); E=g.addVert('E');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");g.addArc(A,C,"ac");g.addArc(E,D,"ed");
    //g.addArc(C,A,"ed");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PEdge LOCALARRAY(out,g.getEdgeNo());
    AssocArray<PEdge,bool >  czyhasse;
    int res;
    if ((res=dagHasseExamp(g,out,czyhasse))==-1) { cout << "Nie jest DAG,  bledne dane" ; return; }
    cout << "Krawedzie diagramu Hassego: ";
    for(int i=0;i<res;i++) cout << out[i]->info << ' ';
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (czyhasse[e]) cout << '\n' << e->info;
}

int sccExamp(const MyGraph& g, AssocArray<PVertex,int >&  comps, SearchStructs::CompStoreTool<PVertex>::InputType compout)
{
    if (!g.getVert()) return -1; //Bledne dane
    return SCC::split(g,compout,comps);
}

//I to samo proponuje  powtorzyc dla BFS -> LexBFS
void sccMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D'); E=g.addVert('E');F=g.addVert('F');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");g.addArc(C,A,"ca");g.addEdge(E,D,"ed");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    AssocArray<PVertex,int >  sklad1;
    SearchStructs::CompStoreTool<PVertex> sklad2;
    int res;
    if ((res=sccExamp(g,sklad1,sklad2.input()))==-1) { cout << "bledne dane" ; return; }
    cout << "compno: " << res<< endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << sklad1[v] <<' ';
    cout << "\n Zawartosc skladowych";
    for(int i=0;i<sklad2.size();i++)
    {   cout << '\n'<< i << ':';
        for (int j=0;j<sklad2.size(i);j++) cout << sklad2[i][j]->info << ' ';

    }
}

int biconExamp(const MyGraph& g, AssocArray<PEdge,int >&  comps, SearchStructs::CompStoreTool<PVertex>::InputType compout)
{
    if (!g.getVert() || g.getEdgeNo(Directed)) return -1; //Proponuje tak dac w zgredzie i webserv, zeby nie mylilo
    int res=Blocks::split(g, blackHole,comps,compout,blackHole);
    return res;
}

void biconMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F,G,H,I;
   A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    F=g.addVert('F');
    G=g.addVert('G');    H=g.addVert('H');    I=g.addVert('I');

    g.addEdge(A,B,"ab");    g.addEdge(A,C,"ac");    g.addEdge(B,D,"bd");    g.addEdge(B,C,"bc");    g.addEdge(C,D,"cd");
    g.addEdge(C,E,"ce");    g.addEdge(C,F,"cf");    g.addEdge(D,G,"dg");    g.addEdge(G,H,"gh");    g.addEdge(G,H,"gh");
    g.addLoop(G,"gg");

    g.addEdge(E,F,"ef");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    AssocArray<PEdge,int >  sklad1;
    SearchStructs::CompStoreTool<PVertex> sklad2;
    int res=biconExamp(g,sklad1,sklad2.input());
    if (res==-1) { cout << "Bledny graf"; return; }
    cout << "bicompno: " << res<< endl;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) cout << e->info << sklad1[e] <<' ';
    cout << "\n Zawartosc skladowych 2-spojnych";
    for(int i=0;i<sklad2.size();i++)
    {   cout << '\n'<< i << ':';
        for (int j=0;j<sklad2.size(i);j++) cout << sklad2[i][j]->info << ' ';

    }
}

int undirEulerExamp(const MyGraph& g,PVertex vout[],PEdge eout[])
{
    if (!g.getVert()) return -1;
    if (g.getEdgeNo(Directed)) return -1;//proponuje tak przyjac w webservs i Zgredzie, inaczej (ignorowanie lukow) bedzie mylace
    else
    if (Euler::hasCycle(g))
    {
        Euler::getCycle(g,PathStructs::outPath(vout,eout));
        return 1;
    } else if (Euler::hasPath(g))
    {
        Euler::getPath(g,PathStructs::outPath(vout,eout));
        return 2;
    }
    else return 0;
}

void undirEulerMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");
    g.addEdge(D,C,"dc");

    PEdge LOCALARRAY(eout,g.getEdgeNo());
    PVertex LOCALARRAY(vout,g.getEdgeNo()+1);
    int res=undirEulerExamp(g,vout,eout);
    if (res==-1) { cout << "Bledny graf"; return; }
    if (res==0) cout << "Brak";
    else if (res==1) {
        cout << "Cykl:";
        for(int i=0;i<g.getEdgeNo(Undirected|Loop);i++) cout << eout[i]->info << ' ';
        cout << endl;
        for(int i=0;i<g.getEdgeNo(Undirected|Loop);i++) cout << vout[i]->info << ' ';
    } else {
        cout << "Sciezka:";
        for(int i=0;i<g.getEdgeNo(Undirected|Loop);i++) cout << eout[i]->info << ' ';
        cout << endl;
        for(int i=0;i<g.getEdgeNo(Undirected|Loop)+1;i++) cout << vout[i]->info << ' ';
    }
}

int dirEulerExamp(const MyGraph& g,PVertex vout[],PEdge eout[])
{
     if (!g.getVert()) return -1;
    if (g.getEdgeNo(Undirected)) return -1;//proponuje tak przyjac w webservs i Zgredzie, inaczej (ignorowanie lukow) bedzie mylace
    else
    if (Euler::hasDirCycle(g))
    {
        Euler::getDirCycle(g,PathStructs::outPath(vout,eout));
        return 1;
    } else if (Euler::hasDirPath(g))
    {
        Euler::getDirPath(g,PathStructs::outPath(vout,eout));
        return 2;
    }
    else return 0;
}

void dirEulerMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addArc(A,B,"ab");
    g.addArc(C,A,"ca");
    g.addArc(B,D,"bd");
    g.addArc(D,C,"dc");

    PEdge LOCALARRAY(eout,g.getEdgeNo());
    PVertex LOCALARRAY(vout,g.getEdgeNo()+1);
    int res=dirEulerExamp(g,vout,eout);
    if (res==-1) { cout << "Bledny graf"; return; }
    if (res==0) cout << "Brak";
    else if (res==1) {
        cout << "Cykl:";
        for(int i=0;i<g.getEdgeNo(Directed|Loop);i++) cout << eout[i]->info << ' ';
        cout << endl;
        for(int i=0;i<g.getEdgeNo(Directed|Loop);i++) cout << vout[i]->info << ' ';
    } else {
        cout << "Sciezka:";
        for(int i=0;i<g.getEdgeNo(Directed|Loop);i++) cout << eout[i]->info << ' ';
        cout << endl;
        for(int i=0;i<g.getEdgeNo(Directed|Loop)+1;i++) cout << vout[i]->info << ' ';
    }
}

Modules::Partition modulesExamp(const MyGraph& g, AssocArray<PVertex,int >&  comps,
                                SearchStructs::CompStoreTool<PVertex>::InputType compout)
{
    if (g.getEdgeNo(Directed|Loop) || g.mu()>1 || !g.getVert()) return Modules::Partition(-1,(ModPartType)-1);//Bledny graf
    return Modules::split(g,compout,comps);
}

void modulesMain()
{
    MyGraph g;
    PVertex A,B,C,D,E;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
    g.addEdge(A,B,"ab");g.addEdge(D,C,"dc");//g.addLoop(B,"bb");
    g.addEdge(A,C,"ac");//g.addEdge(B,D,"bd");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    AssocArray<PVertex,int >  sklad1;
    SearchStructs::CompStoreTool<PVertex> sklad2;
    Modules::Partition res=modulesExamp(g,sklad1,sklad2.input());
    if (res.size==-1)
    { cout << "Bledny graf"; return; }
    cout << "Typ roota drzewa dekompozycji " << res.type << endl;
    cout << "compno: " <<  res.size<< endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << sklad1[v] <<'\n';
    cout << "\n Zawartosc modulow";
    for(int i=0;i<sklad2.size();i++)
    {   cout << '\n'<< i << ':';
        for (int j=0;j<sklad2.size(i);j++) cout << sklad2[i][j]->info << ' ';

    }
}

int dijkstraTreeExamp(const MyGraph& g,PVertex start, const AssocArray<PEdge,Dijkstra::EdgeLabs<int> >& lengs,
                                        AssocArray<PVertex,Dijkstra::VertLabs<int,MyGraph> >& vlabs )
{
    if (!start || ! g.has(start)) return -1;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (lengs[e].length<0) return -1; //bledne dane
    DijkstraHeap::distances(g,vlabs,lengs,start);
    return vlabs.size();
}

void dijkstraTreeMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');
    AssocArray<PEdge,Dijkstra::EdgeLabs<int> > lengs;
    AssocArray<PVertex,Dijkstra::VertLabs<int,MyGraph> > vlabs;
    lengs[g.addEdge(A,B,"ab")].length=1;
    lengs[g.addEdge(B,D,"bd")].length=5;lengs[g.addEdge(A,C,"ac")].length=4;lengs[g.addEdge(D,C,"dc")].length=2;
    lengs[g.addEdge(C,E,"ce")].length=1;lengs[g.addEdge(B,C,"bc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(D,E,"de",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(F,D,"fd",Koala::EdDirOut)].length=2;
    int res=dijkstraTreeExamp(g,A,lengs,vlabs);
    if (res==-1) { cout << "Bledne dane"; return; }
    cout << "Wierzcholkow osiagalnych ze startowego: " << res;
    cout << " Drzewo najkrotszych sciezek: \n";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (vlabs.hasKey(v))
        {
            cout << v->info << ": dist " << vlabs[v].distance << " vPrev: ";
            printInfo(vlabs[v].vPrev);
            cout <<  " ePrev: ";printInfo(vlabs[v].ePrev); cout << endl;
        }
}

Dijkstra::PathLengths<int> dijkstraPathExamp(const MyGraph& g,PVertex start, PVertex end, const AssocArray<PEdge,Dijkstra::EdgeLabs<int> >& lengs,
                  Dijkstra::OutPath<PVertex*,PEdge*> out )
{
    Dijkstra::PathLengths<int> blad(std::numeric_limits< int >::min(),-2); //Bledne dane
    if (!start || ! g.has(start)) return blad;
    if (!end || ! g.has(end)) return blad;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (lengs[e].length<0) return blad; //bledne dane
    return DijkstraHeap::findPath(g,lengs,start,end,out);
}

void dijkstraPathMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');
    AssocArray<PEdge,Dijkstra::EdgeLabs<int> > lengs;
    lengs[g.addEdge(A,B,"ab")].length=1;
    lengs[g.addEdge(B,D,"bd")].length=5;lengs[g.addEdge(A,C,"ac")].length=4;lengs[g.addEdge(D,C,"dc")].length=2;
    lengs[g.addEdge(C,E,"ce")].length=1;lengs[g.addEdge(B,C,"bc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(D,E,"de",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(F,D,"fd",Koala::EdDirOut)].length=2;
    PVertex LOCALARRAY(vout,g.getVertNo());
    PEdge LOCALARRAY(eout,g.getVertNo());
    Dijkstra::PathLengths<int> res=dijkstraPathExamp(g,A,E,lengs,Dijkstra::outPath(vout,eout));
    if (res.edgeNo==-2) { cout << "Bledne dane"; return; }
    if (res.edgeNo==-1) { cout << "Brak polaczenia"; return; }
    cout << "Dlugosc sciezki: " << res.length << endl;
    for(int i=0;i<res.edgeNo+1;i++) cout << vout[i]->info;
    cout << endl;
    for(int i=0;i<res.edgeNo;i++) cout << eout[i]->info << ' ';
}

int bellmanTreeExamp(const MyGraph& g,PVertex start, const AssocArray<PEdge,Dijkstra::EdgeLabs<int> >& lengs,
                                        AssocArray<PVertex,Dijkstra::VertLabs<int,MyGraph> >& vlabs )
{
    if (!start || ! g.has(start)) return -1; //bledne dane
    if (BellmanFord::distances(g,vlabs,lengs,start)<0) return -1;
    return vlabs.size();
}

void bellmanTreeMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');
    AssocArray<PEdge,Dijkstra::EdgeLabs<int> > lengs;
    AssocArray<PVertex,Dijkstra::VertLabs<int,MyGraph> > vlabs;
    lengs[g.addEdge(A,B,"ab")].length=1;
    lengs[g.addEdge(B,D,"bd")].length=5;lengs[g.addEdge(A,C,"ac")].length=4;lengs[g.addEdge(D,C,"dc")].length=2;
    lengs[g.addEdge(C,E,"ce")].length=1;lengs[g.addEdge(B,C,"bc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(D,E,"de",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(F,D,"fd",Koala::EdDirOut)].length=2;
    int res=bellmanTreeExamp(g,A,lengs,vlabs);
    if (res==-1) { cout << "Bledne dane"; return; }
    cout << "Wierzcholkow osiagalnych ze startowego: " << res;
    cout << " Drzewo najkrotszych sciezek: \n";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (vlabs.hasKey(v))
        {
            cout << v->info << ": dist " << vlabs[v].distance << " vPrev: ";
            printInfo(vlabs[v].vPrev);
            cout <<  " ePrev: ";printInfo(vlabs[v].ePrev); cout << endl;
        }
}

BellmanFord::PathLengths<int> bellmanPathExamp(const MyGraph& g,PVertex start, PVertex end, const AssocArray<PEdge,Dijkstra::EdgeLabs<int> >& lengs,
                  Dijkstra::OutPath<PVertex*,PEdge*> out )
{
    BellmanFord::PathLengths<int> blad(std::numeric_limits< int >::min(),-2); //Bledne dane
    if (!start || ! g.has(start)) return blad;
    if (!end || ! g.has(end)) return blad;
    return BellmanFord::findPath(g,lengs,start,end,out);
}

void bellmanPathMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');
    AssocArray<PEdge,Dijkstra::EdgeLabs<int> > lengs;
    lengs[g.addEdge(A,B,"ab")].length=1;
    lengs[g.addEdge(B,D,"bd")].length=5;lengs[g.addEdge(A,C,"ac")].length=4;lengs[g.addEdge(D,C,"dc")].length=2;
    lengs[g.addEdge(C,E,"ce")].length=1;lengs[g.addEdge(B,C,"bc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(D,E,"de",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(F,D,"fd",Koala::EdDirOut)].length=2;
    PVertex LOCALARRAY(vout,g.getVertNo());
    PEdge LOCALARRAY(eout,g.getVertNo());
    BellmanFord::PathLengths<int> res=bellmanPathExamp(g,A,E,lengs,Dijkstra::outPath(vout,eout));
    if (res.edgeNo==-2) { cout << "Bledne dane"; return; }
    if (res.edgeNo==-1) { cout << "Brak polaczenia"; return; }
    cout << "Dlugosc sciezki: " << res.length << endl;
    for(int i=0;i<res.edgeNo+1;i++) cout << vout[i]->info;
    cout << endl;
    for(int i=0;i<res.edgeNo;i++) cout << eout[i]->info << ' ';
}

int critPathTreeExamp(const MyGraph& g,PVertex start, const AssocArray<PEdge,Dijkstra::EdgeLabs<int> >& lengs,
                                        AssocArray<PVertex,DAGCritPath::VertLabs<int,MyGraph> >& vlabs )
{
    if (!start || ! g.has(start) || ! DAGAlgs::isDAG(g)) return -1; //bledne dane
    DAGCritPath::critPathLength(g,vlabs,lengs,start);
    return vlabs.size();
}

void critPathTreeMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');
    AssocArray<PEdge,Dijkstra::EdgeLabs<int> > lengs;
    AssocArray<PVertex,DAGCritPath::VertLabs<int,MyGraph> > vlabs;
    lengs[g.addEdge(A,B,"ab",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(B,D,"bd",Koala::EdDirOut)].length=5;lengs[g.addEdge(A,C,"ac",Koala::EdDirOut)].length=4;
    lengs[g.addEdge(D,C,"dc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(C,E,"ce",Koala::EdDirOut)].length=1;lengs[g.addEdge(B,C,"bc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(D,E,"de",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(F,D,"fd",Koala::EdDirOut)].length=2;
    int res=critPathTreeExamp(g,A,lengs,vlabs);
    if (res==-1) { cout << "Bledne dane"; return; }
    cout << "Wierzcholkow osiagalnych ze startowego: " << res;
    cout << " Drzewo najduzszych sciezek sciezek: \n";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v))
        if (vlabs.hasKey(v))
        {
            cout << v->info << ": dist " << vlabs[v].distance << " vPrev: ";
            printInfo(vlabs[v].vPrev);
            cout <<  " ePrev: ";printInfo(vlabs[v].ePrev); cout << endl;
        }
}

DAGCritPath::PathLengths<int> critPathPathExamp(const MyGraph& g,PVertex start, PVertex end, const AssocArray<PEdge,Dijkstra::EdgeLabs<int> >& lengs,
                  Dijkstra::OutPath<PVertex*,PEdge*> out )
{
    DAGCritPath::PathLengths<int> blad(std::numeric_limits< int >::min(),-2); //Bledne dane
    if (!start || ! g.has(start)) return blad;
    if (!end || ! g.has(end)) return blad;
    if (!DAGAlgs::isDAG(g)) return blad;
    return DAGCritPath::findPath(g,lengs,start,end,out);
}

void critPathPathMain()
{
   MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');
    AssocArray<PEdge,Dijkstra::EdgeLabs<int> > lengs;
    AssocArray<PVertex,DAGCritPath::VertLabs<int,MyGraph> > vlabs;
    lengs[g.addEdge(A,B,"ab",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(B,D,"bd",Koala::EdDirOut)].length=5;lengs[g.addEdge(A,C,"ac",Koala::EdDirOut)].length=4;
    lengs[g.addEdge(D,C,"dc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(C,E,"ce",Koala::EdDirOut)].length=1;lengs[g.addEdge(B,C,"bc",Koala::EdDirOut)].length=2;
    lengs[g.addEdge(D,E,"de",Koala::EdDirOut)].length=1;
    lengs[g.addEdge(F,D,"fd",Koala::EdDirOut)].length=2;
    PVertex LOCALARRAY(vout,g.getVertNo());
    PEdge LOCALARRAY(eout,g.getVertNo());
    DAGCritPath::PathLengths<int> res=critPathPathExamp(g,A,E,lengs,Dijkstra::outPath(vout,eout));
    if (res.edgeNo==-2) { cout << "Bledne dane"; return; }
    if (res.edgeNo==-1) { cout << "Brak polaczenia"; return; }
    cout << "Dlugosc sciezki: " << res.length << endl;
    for(int i=0;i<res.edgeNo+1;i++) cout << vout[i]->info;
    cout << endl;
    for(int i=0;i<res.edgeNo;i++) cout << eout[i]->info << ' ';
}

Kruskal::Result<int> kruskalExamp(const MyGraph& g,const AssocArray<PEdge,Kruskal::EdgeLabs<int> >& lengs,PEdge out[],bool maks )
{
    Kruskal::Result<int> blad; blad.edgeNo=-1;
    if (!g.getVert()) return blad;
    if (maks) return Kruskal::getMaxForest(g,lengs,out,blackHole);
    else return Kruskal::getMinForest(g,lengs,out,blackHole);
}

void kruskalMain()
{
   MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');F=g.addVert('F');
    AssocArray<PEdge,Kruskal::EdgeLabs<int> > lengs;
    lengs[g.addEdge(A,B,"ab",Koala::EdDirOut)].weight=1;
    lengs[g.addEdge(B,D,"bd",Koala::EdDirOut)].weight=5;lengs[g.addEdge(A,C,"ac",Koala::EdDirOut)].weight=4;
    lengs[g.addEdge(D,C,"dc",Koala::EdDirOut)].weight=2;
    lengs[g.addEdge(C,E,"ce",Koala::EdDirOut)].weight=1;lengs[g.addEdge(B,C,"bc",Koala::EdDirOut)].weight=2;
    lengs[g.addEdge(D,E,"de",Koala::EdDirOut)].weight=1;
    lengs[g.addEdge(F,D,"fd",Koala::EdDirOut)].weight=2;
    PEdge LOCALARRAY(eout,g.getVertNo()-1);
    bool maks=false;
    Kruskal::Result<int> res=kruskalExamp(g,lengs,eout,maks);
    if (res.edgeNo==-1) { cout << "Bledne dane"; return; }
    cout << ((maks)? "Maksymalna ": "Minimalna ")<<" waga lasu " <<  res.weight << endl;
    for(int i=0;i<res.edgeNo;i++) cout << eout[i]->info << ' ';
}

int maxMachingExamp(const MyGraph& g,PEdge out[])
{
    if(!g.getVert()) return -1;
    return Matching::findMax(g,out);
}

void maxMachingMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");
    g.addEdge(D,C,"dc");

    PEdge LOCALARRAY(eout,g.getEdgeNo());
    int res=maxMachingExamp(g,eout);
    if (res==-1) { cout << "Bledne dane"; return; }
    cout <<"rozmiar: " << res << endl;
    for(int i=0;i<res;i++) cout << eout[i]->info << ' ';
}

int verColorGreedyExamp(const MyGraph& g,AssocArray<PVertex,int >& colors,PVertex* beg, PVertex* end,bool interchange)
{
    Set<PVertex> s(beg,end);
    if (!g.getVert() || end-beg!=g.getVertNo() || s!=g.getVertSet()) return -1; //bledne dane
    if (!interchange) return SeqVertColoring::greedy(g,colors,beg,end);
    else return SeqVertColoring::greedyInter(g,colors,beg,end);
}

void verColorGreedyMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");
    PVertex LOCALARRAY(tabv,g.getVertNo());
    g.getVerts(tabv);
    std::random_shuffle ( tabv,tabv+g.getVertNo() ); //jesli webserv. pozwalaja na latwe podanie permutacji wierzcholkow, proponuje by
    //w nich kolorowac wg. podanej przez usera sekwencji, a w Zgredzie wg. jednej. naturalnej

    AssocArray<PVertex,int > colors;
    bool useInterchange=true;
    int res=verColorGreedyExamp(g,colors,tabv,tabv+g.getVertNo(),useInterchange);
    if (res==-1) { cout << "Bledne dane"; return; }
    cout << "Najw. barwa: " <<res << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << colors[v] << ' ';
}

int verColorLFExamp(const MyGraph& g,AssocArray<PVertex,int >& colors,bool interchange)
{
    if (!g.getVert()) return -1; //bledne dane
    if (!interchange) return SeqVertColoring::lf(g,colors);
    else return SeqVertColoring::lfInter(g,colors);
}

void verColorLFMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");

    AssocArray<PVertex,int > colors;
    bool useInterchange=true;
    cout << "Najw. barwa (lub -1 w razie bledu): " <<verColorLFExamp(g,colors,useInterchange) << endl; //juz mi sie nie chce wklepywac obslugi bledu, ale wiadom ...
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << colors[v] << ' ';
}

int verColorSLExamp(const MyGraph& g,AssocArray<PVertex,int >& colors,bool interchange)
{ if (!g.getVert()) return -1; //bledne dane
    if (!interchange) return SeqVertColoring::sl(g,colors);
    else return SeqVertColoring::slInter(g,colors);
}

void verColorSLMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");

    AssocArray<PVertex,int > colors;
    bool useInterchange=true;
    cout << "Najw. barwa (lub -1 w razie bledu): " <<verColorSLExamp(g,colors,useInterchange) << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << colors[v] << ' ';
}

int verColorSLFExamp(const MyGraph& g,AssocArray<PVertex,int >& colors,bool interchange)
{ if (!g.getVert()) return -1; //bledne dane
    if (!interchange) return SeqVertColoring::slf(g,colors);
    else return SeqVertColoring::slfInter(g,colors);
}

void verColorSLFMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");

    AssocArray<PVertex,int > colors;
    bool useInterchange=true;
    cout << "Najw. barwa (lub -1 w razie bledu): " <<verColorSLFExamp(g,colors,useInterchange) << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << colors[v] << ' ';
}

int verColorBrooksExamp(const MyGraph& g,AssocArray<PVertex,int >& colors)
{ if (!g.getVert()) return -1; //bledne dane
    return SeqVertColoring::brooks(g,colors);
}

void verColorBrooksMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");

    AssocArray<PVertex,int > colors;
    cout << "Najw. barwa (lub -1 w razie bledu): " <<verColorBrooksExamp(g,colors) << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << colors[v] << ' ';
}

int edgeColorGreedyExamp(const MyGraph& g,AssocArray<PEdge,int >& colors,PEdge* beg, PEdge* end,bool interchange)
{
    Set<PEdge> s(beg,end);
    if (!g.getVert() || end-beg!=g.getEdgeNo(Directed|Undirected) || s!=g.getEdgeSet(Directed|Undirected)) return -1; //bledne dane
    if (!interchange) return SeqEdgeColoring::greedy(g,colors,beg,end);
    else return SeqEdgeColoring::greedyInter(g,colors,beg,end);
}

void edgeColorGreedyMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");
    PEdge LOCALARRAY(tabv,g.getEdgeNo(Directed|Undirected));
    g.getEdges(tabv,Directed|Undirected);
    std::random_shuffle ( tabv,tabv+g.getEdgeNo(Directed|Undirected) ); //jesli webserv. pozwalaja na latwe podanie permutacji krawedzi, proponuje by
    //w nich kolorowac wg. podanej przez usera sekwencji, a w Zgredzie wg. jednej.
    AssocArray<PEdge,int > colors;
    bool useInterchange=true;
    cout << "Najw. barwa (lub -1 w razie bledu): " <<edgeColorGreedyExamp(g,colors,tabv,tabv+g.getEdgeNo(Directed|Undirected),useInterchange) << endl;
    for(PEdge v=g.getEdge();v;v=g.getEdgeNext(v)) cout << v->info << colors[v] << ' ';
}

int edgeVizingColorExamp(const MyGraph& g,AssocArray<PEdge,int >& colors)
{if (!g.getVert()) return -1; //bledne dane
    return SeqEdgeColoring::vizing(g,colors);
}

void edgeVizingColorExamp()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");
    AssocArray<PEdge,int > colors;
    cout << "Najw. barwa (lub -1 w razie bledu): " <<edgeVizingColorExamp(g,colors) << endl;
    for(PEdge v=g.getEdge();v;v=g.getEdgeNext(v)) cout << v->info << colors[v] << ' ';
}


int vertIntervalColorGreedyExamp(const MyGraph& g,const AssocArray<PVertex,int >& grubosci,
                                AssocArray<PVertex,Segment >& colors,PVertex* beg, PVertex* end)
{
    Set<PVertex> s(beg,end);
    if (!g.getVert() || end-beg!=g.getVertNo() || s!=g.getVertSet()) return -2; //bledne dane
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) if (grubosci[v]<=0) return -2;
    return IntervalVertColoring::greedy(g,grubosci,colors,beg,end);
}

void vertIntervalColorGreedyMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    AssocArray<PVertex,int > grub;
    grub[A=g.addVert('A')]=2;    grub[B=g.addVert('B')]=3;    grub[C=g.addVert('C')]=1;    grub[D=g.addVert('D')]=1;
    grub[E=g.addVert('E')]=2;      grub[F=g.addVert('F')]=4;
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");
    PVertex LOCALARRAY(tabv,g.getVertNo());
    g.getVerts(tabv);
    std::random_shuffle ( tabv,tabv+g.getVertNo() ); //jesli webserv. pozwalaja na latwe podanie permutacji wierzcholkow, proponuje by
    //w nich kolorowac wg. podanej przez usera sekwencji, a w Zgredzie wg. jednej. naturalnej

    AssocArray<PVertex,Segment > colors;
    int res=vertIntervalColorGreedyExamp(g,grub,colors,tabv,tabv+g.getVertNo());
    if (res==-2) { cout << "Bledne dane"; return; }
    cout << "Najw. barwa: " <<res << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << ":{" << colors[v].left<< ",...," << colors[v].right<< "}\n";
}

int edgeIntervalColorGreedyExamp(const MyGraph& g,const AssocArray<PEdge,int >& grubosci, AssocArray<PEdge,Segment >& colors,
                                 PEdge* beg, PEdge* end)
{
    Set<PEdge> s(beg,end);
    if (!g.getVert() || end-beg!=g.getEdgeNo(Directed|Undirected) || s!=g.getEdgeSet(Directed|Undirected)) return -2; //bledne dane
    for(PEdge v=g.getEdge(Directed|Undirected);v;v=g.getEdgeNext(v,Directed|Undirected)) if (grubosci[v]<=0) return -2;
    return IntervalEdgeColoring::greedy(g,grubosci,colors,beg,end);
}

void edgeIntervalColorGreedyMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    AssocArray<PEdge,int > grub;
    grub[g.addEdge(A,B,"ab")]=5;
    grub[g.addEdge(A,C,"ac")]=3;
    grub[g.addEdge(B,D,"bd")]=1;grub[g.addEdge(A,D,"ad")]=2;
    grub[g.addEdge(D,C,"dc")]=2;
    PEdge LOCALARRAY(tabv,g.getEdgeNo(Directed|Undirected));
    g.getEdges(tabv,Directed|Undirected);
    std::random_shuffle ( tabv,tabv+g.getEdgeNo(Directed|Undirected) ); //jesli webserv. pozwalaja na latwe podanie permutacji krawedzi, proponuje by
    //w nich kolorowac wg. podanej przez usera sekwencji, a w Zgredzie wg. jednej.
    AssocArray<PEdge,Segment > colors;
    int res =edgeIntervalColorGreedyExamp(g,grub,colors,tabv,tabv+g.getEdgeNo(Directed|Undirected));
    if (res==-2) { cout << "Bledne dane"; return; }
    for(PEdge v=g.getEdge();v;v=g.getEdgeNext(v)) cout << v->info << ":{" << colors[v].left<< ",...," << colors[v].right<< "}\n";
}

int vertIntervalColorLIExamp(const MyGraph& g,const AssocArray<PVertex,int >& grubosci,
                                AssocArray<PVertex,Segment >& colors)
{
    if (!g.getVert() ) return -2; //bledne dane
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) if (grubosci[v]<=0) return -2;
    return IntervalVertColoring::li(g,grubosci,colors);
}

void vertIntervalColorLIMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    AssocArray<PVertex,int > grub;
    grub[A=g.addVert('A')]=2;    grub[B=g.addVert('B')]=3;    grub[C=g.addVert('C')]=1;    grub[D=g.addVert('D')]=1;
    grub[E=g.addVert('E')]=2;      grub[F=g.addVert('F')]=4;
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");
    AssocArray<PVertex,Segment > colors;
    int res=vertIntervalColorLIExamp(g,grub,colors);
    if (res==-2) { cout << "Bledne dane"; return; }
    cout << "Najw. barwa: " <<res << endl;
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << ":{" << colors[v].left<< ",...," << colors[v].right<< "}\n";
}

int edgeIntervalColorLIExamp(const MyGraph& g,const AssocArray<PEdge,int >& grubosci, AssocArray<PEdge,Segment >& colors)
{
    if (!g.getVertNo()) return -2; //bledne dane
    for(PEdge v=g.getEdge(Directed|Undirected);v;v=g.getEdgeNext(v,Directed|Undirected)) if (grubosci[v]<=0) return -2;
    return IntervalEdgeColoring::li(g,grubosci,colors);
}

void edgeIntervalColorLIMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    AssocArray<PEdge,int > grub;
    grub[g.addEdge(A,B,"ab")]=5;
    grub[g.addEdge(A,C,"ac")]=3;
    grub[g.addEdge(B,D,"bd")]=1;grub[g.addEdge(A,D,"ad")]=2;
    grub[g.addEdge(D,C,"dc")]=2;
    AssocArray<PEdge,Segment > colors;
    int res =edgeIntervalColorLIExamp(g,grub,colors);
    if (res==-2) { cout << "Bledne dane"; return; }
    for(PEdge v=g.getEdge();v;v=g.getEdgeNext(v)) cout << v->info << ":{" << colors[v].left<< ",...," << colors[v].right<< "}\n";
}

int edgeIntervalColorLFExamp(const MyGraph& g,const AssocArray<PEdge,int >& grubosci, AssocArray<PEdge,Segment >& colors)
{
    if (!g.getVertNo()) return -2; //bledne dane
    for(PEdge v=g.getEdge(Directed|Undirected);v;v=g.getEdgeNext(v,Directed|Undirected)) if (grubosci[v]<=0) return -2;
    return IntervalEdgeColoring::lf(g,grubosci,colors);
}

void edgeIntervalColorLFMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    AssocArray<PEdge,int > grub;
    grub[g.addEdge(A,B,"ab")]=5;
    grub[g.addEdge(A,C,"ac")]=3;
    grub[g.addEdge(B,D,"bd")]=1;grub[g.addEdge(A,D,"ad")]=2;
    grub[g.addEdge(D,C,"dc")]=2;
    AssocArray<PEdge,Segment > colors;
    int res =edgeIntervalColorLFExamp(g,grub,colors);
    if (res==-2) { cout << "Bledne dane"; return; }
    for(PEdge v=g.getEdge();v;v=g.getEdgeNext(v)) cout << v->info << ":{" << colors[v].left<< ",...," << colors[v].right<< "}\n";
}


bool invExamp( MyGraph& g)
{
    if (!g.getVert()) return false;
    RelDiagram::inv(g);
    return true;
}

void invMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");
    g.addEdge(B,D,"bd");
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
   cout << "\n****\n";
   if (invExamp(g)) writeGraphText(g, cout, RG_VertexLists|RG_Info);
}

bool repairExamp( MyGraph& g)
{
    if (!g.getVert()) return false;
    RelDiagram::repair(g);
    return true;
}

void repairMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");
    g.addEdge(B,D,"bd");
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
   cout << "\n****\n";
   if (repairExamp(g)) writeGraphText(g, cout, RG_VertexLists|RG_Info);
}


bool reflClousureExamp( MyGraph& g)
{
    if (!g.getVert()) return false;
    RelDiagram::reflClousure(g,"nowa");
    return true;
}

void reflClousureMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");
    g.addEdge(B,D,"bd");
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
   cout << "\n****\n";
   if (reflClousureExamp(g)) writeGraphText(g, cout, RG_VertexLists|RG_Info);
}

bool symmClousureExamp( MyGraph& g)
{
    if (!g.getVert()) return false;
    RelDiagram::symmClousure(g,"nowa");
    return true;
}

void symmClousureMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");
    g.addEdge(B,D,"bd");
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
   cout << "\n****\n";
   if (symmClousureExamp(g)) writeGraphText(g, cout, RG_VertexLists|RG_Info);
}

bool transClousureExamp( MyGraph& g)
{
    if (!g.getVert()) return false;
    RelDiagram::symmClousure(g,"nowa");
    return true;
}

void transClousureMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addArc(A,B,"ab");g.addArc(B,C,"bc");
    g.addEdge(B,D,"bd");
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
   cout << "\n****\n";
   if (transClousureExamp(g)) writeGraphText(g, cout, RG_VertexLists|RG_Info);
}

bool undirExamp( const MyGraph &g, bool allowmulti)
{
    return IsIt::undir(g,allowmulti);
}

void undirMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');      F=g.addVert('F');
    g.addEdge(A,B,"ab");g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");
    bool allow=false;
    cout << boolalpha<< undirExamp(g,allow);
}

bool connectedExamp( const MyGraph &g, bool allowmulti)
{
    return IsIt::connected(g,allowmulti);
}

void connectedMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addEdge(A,B,"ab");g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");
    bool allow=true;
    cout << boolalpha<< connectedExamp(g,allow);
}

bool forestExamp( const MyGraph &g)
{
    return IsIt::forest(g);
}

void forestMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addEdge(A,B,"ab");g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");g.addEdge(A,D,"ad");
    g.addEdge(D,C,"dc");

    cout << boolalpha<< forestExamp(g);
}

bool cliquesExamp( const MyGraph &g)
{
    return IsIt::cliques(g);
}

void cliquesMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,C,"bc");

    cout << boolalpha<< cliquesExamp(g);
}

struct IsPathRes {
    bool res;
    std::pair<PVertex,PVertex> ends;
};

IsPathRes pathExample(const MyGraph &g)
{
    IsPathRes res;
    res.ends=IsIt::Path::ends(g);
    res.res=res.ends.first;
    return res;
}

void pathMain()
{
   MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");
    IsPathRes res=pathExample(g);
    if (!res.res) cout << "Nie sciezka";
    else cout << "Sciezka o koncach " << res.ends.first->info <<res.ends.second->info;
}

IsPathRes caterpillarExample(const MyGraph &g)
{
    IsPathRes res;
    res.ends=IsIt::Caterpillar::spineEnds(g);
    res.res=res.ends.first;
    return res;
}

void caterpillarMain()
{
   MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D'); E=g.addVert('E');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,D,"bd");
    IsPathRes res=caterpillarExample(g);
    if (!res.res) cout << "Nie gasienica";
    else cout << "gasienica o koncach grzbietu " << res.ends.first->info <<res.ends.second->info;
}

bool blockExamp( const MyGraph &g)
{
    return IsIt::block(g);
}

void blockMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,C,"bc");

    cout << boolalpha<< blockExamp(g);
}

int compMPartiteExamp(const MyGraph& g, AssocArray<PVertex,int >&  comps, SearchStructs::CompStoreTool<PVertex>::InputType compout)
{
    return IsIt::CompMPartite::split(g,comps,compout);
}


void compMPartiteMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D'); E=g.addVert('E');//F=g.addVert('F');
    g.addEdge(A,C,"ac");g.addEdge(B,C,"bc");g.addEdge(D,A,"ad");g.addEdge(B,D,"bd");
    g.addEdge(E,A,"ae");g.addEdge(B,E,"be");g.addEdge(E,C,"ce");g.addEdge(D,E,"de");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    AssocArray<PVertex,int >  sklad1;
    SearchStructs::CompStoreTool<PVertex> sklad2;
    int res=compMPartiteExamp(g,sklad1,sklad2.input());
    if (res==-1) { cout << "Nie"; return; }
    cout << "Pelny " << res<< "-dzielny\n";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << sklad1[v] <<' ';
    cout << "\n Zawartosc skladowych";
    for(int i=0;i<sklad2.size();i++)
    {   cout << '\n'<< i << ':';
        for (int j=0;j<sklad2.size(i);j++) cout << sklad2[i][j]->info << ' ';

    }
}

std::pair<int,int> biartiteExamp(const MyGraph& g, AssocArray<PVertex,int >&  comps,PVertex out[],bool allowmulti,
                                  bool findmaxstable, PVertex mis[])
{ //ale mysle, ze wyjsciowe sekwencje mis tez mozna by pokazac na kanwie w zgredzie
    std::pair<int,int> res;
    res.first=IsIt::Bipartite::getPart(g,assocInserter(comps,constFun(0)),allowmulti);
    if (res.first==-1) return res; //niedwudzielny
    comps.getKeys(out);
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) if (!comps.hasKey(v)) comps[v]=1;
    if (findmaxstable) res.second=IsIt::Bipartite::maxStable(g,mis);
    return res;
}


void biartiteExamp()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D'); E=g.addVert('E');//F=g.addVert('F');
    g.addEdge(A,C,"ac");g.addEdge(B,C,"bc");g.addEdge(D,A,"ad");g.addEdge(B,D,"bd");
    g.addEdge(E,A,"ae");g.addEdge(B,E,"be");//g.addEdge(E,C,"ce");g.addEdge(D,E,"de");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    AssocArray<PVertex,int >  sklad1;
    PVertex LOCALARRAY(out,g.getVertNo());
    PVertex LOCALARRAY(mis,g.getVertNo());
    bool szukamymaxstable=true,allow=false;
    std::pair<int,int> res=biartiteExamp(g,sklad1,out,allow,szukamymaxstable,mis);
    if (res.first==-1) {cout <<"Niedwudzielny"; return; }
    cout <<"Dwudzielny, partycja:";
    for(int i=0;i<res.first;i++) cout << out[i]->info << ' ';
    cout <<"\nPrzynaleznosc wierzcholkow do part.:";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << sklad1[v] <<' ';
    if (szukamymaxstable)
    {
      cout <<"\nmis:";   for(int i=0;i<res.second;i++) cout << mis[i]->info << ' ';
    }
}

bool splitExamp( const MyGraph &g)
{
    return IsIt::split(g);
}

void splitMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,C,"bc");

    cout << boolalpha<< splitExamp(g);
}

bool cographExamp( const MyGraph &g)
{
    return IsIt::cograph(g);
}

void cographMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');    B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');
    g.addEdge(A,B,"ab");
    g.addEdge(A,C,"ac");
    g.addEdge(B,C,"bc");

    cout << boolalpha<< cographExamp(g);
}

bool intervalExamp(const MyGraph& g, AssocArray<PVertex,Segment >&  repr)
{
    return IsIt::Interval::graph2segs(g,repr);
}

void intervalMain()
{
    MyGraph g;
    PVertex A,B,C,D,E,F;
    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D'); E=g.addVert('E');//F=g.addVert('F');
    g.addEdge(A,C,"ac");g.addEdge(B,C,"bc");g.addEdge(D,A,"ad");g.addEdge(B,D,"bd");
    g.addEdge(B,A,"ab");
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    AssocArray<PVertex,Segment >  repr;
    if (!intervalExamp(g,repr)) { cout << "nie"; return ;}
    cout <<"Reprezentacja przedzialowa: \n";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << ": ["<< repr[v].left << ';' << repr[v].right << "]\n";
}

struct IsChordRes {
    bool res;
    int missize,cliquesize;
};

IsChordRes chordalExamp(const MyGraph& g, AssocArray<PVertex,int >&  ord1, PVertex ord2[], bool findmaxclique, PVertex clique[],
                        bool findmis, PVertex mis[],bool findallmaxcliques,SearchStructs::CompStoreTool<PVertex>::InputType compout,
                        bool koloruj, AssocArray<PVertex,int >&  colors)
{ //ale mysle, ze wyjsciowe sekwencje clique i mis tez mozna by pokazac na kanwie w zgredzie
    IsChordRes res;
    if (!(res.res=IsIt::Chordal::getOrder( g, ord2 ))) return res;
    for(int i=0;i<g.getVertNo();i++) ord1[ord2[i]]=i;
    if (findmaxclique) res.cliquesize=IsIt::Chordal::maxClique(g,ord2,ord2+g.getVertNo(),clique);
    if (findmis) res.missize=IsIt::Chordal::maxStable(g,mis);
    if (findallmaxcliques) IsIt::Chordal::maxCliques(g,ord2,ord2+g.getVertNo(),compout,blackHole);
    if (koloruj) SeqVertColoring::greedy(g,colors,ord2,ord2+g.getVertNo());
    return res;
}

void chordalMain()
{
        MyGraph g;
        PVertex u1,u2,u3,u4,u5,u6,u7,u8,u9,u10;
   u1=g.addVert('A');u2=g.addVert('B');    u3=g.addVert('C');    u4=g.addVert('D');    u5=g.addVert('E');    u6=g.addVert('F');
    u7=g.addVert('G');    u8=g.addVert('H');    u9=g.addVert('I');
    g.addEdge(u1,u4);        g.addEdge(u1,u6);        g.addEdge(u3,u4);        g.addEdge(u5,u4);        g.addEdge(u4,u6);
    g.addEdge(u5,u6);        g.addEdge(u2,u7);g.addEdge(u7,u3);        g.addEdge(u1,u8);        g.addEdge(u2,u3);
//    g.addEdge(u2,u3);
    PVertex LOCALARRAY(ord,g.getVertNo());
    PVertex LOCALARRAY(mis,g.getVertNo());
    PVertex LOCALARRAY(clique,g.getVertNo());
    AssocArray<PVertex,int> numeracja,colors;
    SearchStructs::CompStoreTool<PVertex> sklad2;
    bool szukajkliki=true,szukajmisa=true,szukajklik=true,koloruj=true;
    IsChordRes res=chordalExamp(g,numeracja, ord,szukajkliki,clique,szukajmisa,mis,szukajklik,sklad2.input(),koloruj, colors);
    if (!res.res) { cout << "niechordal"; return; }
    cout << "Chordal elim. order: ";//lub jej revers, trza ustalic
    for(int i=0;i<g.getVertNo();i++) cout << ord[i]->info << ' ';
    cout << "\nNumer w porzadku: ";
    for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << numeracja[v] <<' ';
    if (szukajkliki)
    {
        cout << "\nmax klika o rozmiarze " << res.cliquesize << ':';
        for(int i=0;i<res.cliquesize;i++) cout << clique[i]->info << ' ';
    }
    if (szukajmisa)
    {
        cout << "\nmax ind. set o rozmiarze " << res.missize << ':';
        for(int i=0;i<res.missize;i++) cout << mis[i]->info << ' ';
    }
    if (szukajklik)
    {
        cout << "\n Zawartosc niepowiekszalnych klik:";
        for(int i=0;i<sklad2.size();i++)
        {   cout << '\n'<< i << ':';
            for (int j=0;j<sklad2.size(i);j++) cout << sklad2[i][j]->info << ' ';
        }
    }
    if (koloruj)
    {
        cout << "\n Optymalne pokolorowanie:\n";
        for(PVertex v=g.getVert();v;v=g.getVertNext(v)) cout << v->info << colors[v] <<' ';
    }
}

struct IsComparabilityRes {
    bool res;
    int missize,cliquesize;
};

IsComparabilityRes comparabilityExamp(const MyGraph& g, AssocArray<PEdge,EdgeDirection >&  dirs, AssocArray<PVertex,int >& colors,
                                 PVertex clique[], bool findmis, PVertex mis[])
{//ale mysle, ze wyjsciowe sekwencje clique i mis tez mozna by pokazac na kanwie w zgredzie
    IsComparabilityRes res;
    if (!(res.res=((res.cliquesize=IsIt::Comparability::explore( g, dirs,colors, clique))!=-1) )) return res;
    if (findmis) res.missize=IsIt::Comparability::maxStable(g,mis);
    return res;
}

void comparabilityMain()
{
            MyGraph g;
       PVertex u1,u2,u3,u4,u5,u6,u7,u8,u9,u10;
   u1=g.addVert('A');u2=g.addVert('B');    u3=g.addVert('C');    u4=g.addVert('D');    u5=g.addVert('E');    u6=g.addVert('F');
    u7=g.addVert('G');    u8=g.addVert('H');    u9=g.addVert('I');
    g.addEdge(u1,u4);        g.addEdge(u1,u6);        g.addEdge(u3,u4);        g.addEdge(u5,u4);        g.addEdge(u4,u6);
    g.addEdge(u5,u6);        g.addEdge(u2,u7);g.addEdge(u7,u3);        g.addEdge(u1,u8);        g.addEdge(u2,u3);
//    g.addEdge(u2,u3);
    PVertex LOCALARRAY(mis,g.getVertNo());
    PVertex LOCALARRAY(clique,g.getVertNo());
    AssocArray<PVertex,int> colors;
    AssocArray<PEdge,EdgeDirection> dirs;
    bool szukajmisa=true;
    IsComparabilityRes res=comparabilityExamp(g,dirs, colors,clique,szukajmisa,mis);
    if (!res.res) { cout << "niecomparability"; return; }
    cout << "\nOrientacja krawedzi:\n";
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        if (dirs[e]==EdDirOut) cout << g.getEdgeEnd1(e)->info << g.getEdgeEnd2(e)->info << ':' << g.getEdgeEnd1(e)->info << '>' << g.getEdgeEnd2(e)->info << endl;
        else cout << g.getEdgeEnd1(e)->info << g.getEdgeEnd2(e)->info<< ':' << g.getEdgeEnd2(e)->info << '>' << g.getEdgeEnd1(e)->info << endl;
    cout << "\nmax klika o rozmiarze rownym liczbie chromatycznej " << res.cliquesize << ':';
    for(int i=0;i<res.cliquesize;i++) cout << clique[i]->info << ' ';

    cout << "\nKolorowanie optymalne: ";
    for(PVertex e=g.getVert();e;e=g.getVertNext(e)) cout << e->info << ':' << colors[e] << ' ';
    if (szukajmisa)
    {
        cout << "\nmax ind. set o rozmiarze " << res.missize << ':';
        for(int i=0;i<res.missize;i++) cout << mis[i]->info << ' ';
    }
}

int maxFlowExamp(const MyGraph& g, AssocArray<PEdge,Flow::EdgeLabs<int> >& cont, PVertex start, PVertex end, int limit)
{
    if (!start || !end || !g.has(start) || !g.has(end) || start==end || limit<0) return -1;//bledne dane
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (cont[e].capac <0) return -1;//bledne dane
    return Flow::maxFlow(g,cont,start,end,limit); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
}

void maxFlowMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H,start,end;
       AssocArray<PEdge,Flow::EdgeLabs<int> > capacs(100);
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    end=F=g.addVert('F');
    G=g.addVert('G');    start=H=g.addVert('H');
    capacs[g.addEdge(H,A,"ha")].capac=2;        capacs[g.addEdge(H,C,"hc")].capac=2;        capacs[g.addEdge(H,D,"hd")].capac=2;
    capacs[g.addEdge(A,G,"ag",EdDirOut)].capac=5;        capacs[g.addEdge(A,B,"ab")].capac=1;
    capacs[g.addEdge(C,G,"cg",EdDirOut)].capac=1;        capacs[g.addEdge(C,B,"cb",EdDirOut)].capac=3;
    capacs[g.addEdge(C,E,"ce",EdDirOut)].capac=1;        capacs[g.addEdge(G,F,"gf")].capac=3;        capacs[g.addEdge(B,F,"bf")].capac=2;
    capacs[g.addEdge(E,F,"ef")].capac=4;
    capacs[g.addEdge(F,H,"fh",EdDirOut)].capac=3; capacs[g.addLoop(C,"cc")].capac=2; capacs[g.addLoop(G,"gg")].capac=4;
    capacs[g.addEdge(D,B,"db")].capac=1;
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    int res,limit=10;
    if ((res=maxFlowExamp(g,capacs,start,end,limit))==-1) { cout << "Bledne dane"; return; }
    assert(Flow::testFlow(g,capacs,start,end));
    assert(Flow::vertFlow(g,capacs,start)==res);
    cout << "Osiagnieto przeplyw o obj. " << res;
    if (res<limit) cout <<" - jest on maksymalny, a " << limit << " jest nieosiagalny";
    cout << endl;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
    {   cout << e->info << ':' << ((g.getEdgeType(e)!=Undirected) ? capacs[e].flow : std::max(capacs[e].flow,-capacs[e].flow));
        if (g.getEdgeType(e)==Undirected && capacs[e].flow>0) cout << " w kierunku " << g.getEdgeEnd1(e)->info << '>' <<g.getEdgeEnd2(e)->info;
        if (g.getEdgeType(e)==Undirected && capacs[e].flow<0) cout << " w kierunku " << g.getEdgeEnd2(e)->info << '>' <<g.getEdgeEnd1(e)->info;
        cout << endl;
    }
}

std::pair<int,int> minCostFlowExamp(const MyGraph& g, AssocArray<PEdge,Flow::EdgeLabs<int> >& cont, PVertex start, PVertex end, int limit)
{
    if (!start || !end || !g.has(start) || !g.has(end) || start==end || limit<0) return make_pair(-1,-1);//bledne dane
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (cont[e].capac <0) return make_pair(-1,-1);//bledne dane
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (cont[e].cost <0) return make_pair(-1,-1);//bledne dane - to chyba zbedne zalozenie, ale na razie zostawiam
    return Flow::minCostFlow(g,cont,start,end,limit); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
}

void minCostFlowMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H,start,end;
       AssocArray<PEdge,Flow::EdgeLabs<int> > capacs(100);
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    end=F=g.addVert('F');
    G=g.addVert('G');    start=H=g.addVert('H');
    capacs[g.addEdge(H,A,"ha")].capac=2;        capacs[g.addEdge(H,C,"hc")].capac=2;        capacs[g.addEdge(H,D,"hd")].capac=2;
    capacs[g.addEdge(A,G,"ag",EdDirOut)].capac=5;        capacs[g.addEdge(A,B,"ab")].capac=1;
    capacs[g.addEdge(C,G,"cg",EdDirOut)].capac=1;        capacs[g.addEdge(C,B,"cb",EdDirOut)].capac=3;
    capacs[g.addEdge(C,E,"ce",EdDirOut)].capac=1;        capacs[g.addEdge(G,F,"gf")].capac=3;        capacs[g.addEdge(B,F,"bf")].capac=2;
    capacs[g.addEdge(E,F,"ef")].capac=4;
    capacs[g.addEdge(F,H,"fh",EdDirOut)].capac=3; capacs[g.addLoop(C,"cc")].capac=2; capacs[g.addLoop(G,"gg")].capac=4;
    capacs[g.addEdge(D,B,"db")].capac=1;
    int i=0;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e),i++) capacs[e].cost=2+ ((11*i)%7);//czyli random
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    int limit=50;
    std::pair<int,int> res;
    if ((res=minCostFlowExamp(g,capacs,start,end,limit)).second==-1) { cout << "Bledne dane"; return; }
    assert(Flow::testFlow(g,capacs,start,end));
    assert(Flow::vertFlow(g,capacs,start)==res.second);
    cout << "Osiagnieto przeplyw o obj. " << res.second;
    if (res.second<limit) cout <<" - jest on maksymalny, a " << limit << " jest nieosiagalny";
    cout << endl;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
    {   cout << e->info << ':' << ((g.getEdgeType(e)!=Undirected) ? capacs[e].flow : std::max(capacs[e].flow,-capacs[e].flow));
        if (g.getEdgeType(e)==Undirected && capacs[e].flow>0) cout << " w kierunku " << g.getEdgeEnd1(e)->info << '>' <<g.getEdgeEnd2(e)->info;
        if (g.getEdgeType(e)==Undirected && capacs[e].flow<0) cout << " w kierunku " << g.getEdgeEnd2(e)->info << '>' <<g.getEdgeEnd1(e)->info;
        cout << endl;
    }
    cout << "Sposrod wszystkich przeplywow o obj. " << res.second << " ten ma mimimalny koszt = " << res.first;
    assert(Flow::flowCost(g,capacs)==res.first);
}

Flow::EdgeCut<int> minEdgeCutExamp(const MyGraph& g, AssocArray<PEdge,Flow::EdgeLabs<int> >& cont, PVertex start, PVertex end,
                                    Flow::OutCut<PVertex*,PEdge*> out)
//Mozna tez udostepnic dla samego multigrafu mieszanego (bez cont) nadajac wowczas cont[..wszystkie ...].capac=1;
{
    Flow::EdgeCut<int> blad; blad.capac=-1;
    if (!start || !end || !g.has(start) || !g.has(end) || start==end ) return blad;//bledne dane
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (cont[e].capac <0) return blad;//bledne dane
    return Flow::minEdgeCut(g,cont,start,end,out); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
}

void minEdgeCutMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H,start,end;
       AssocArray<PEdge,Flow::EdgeLabs<int> > capacs(100);
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    end=F=g.addVert('F');
    G=g.addVert('G');    start=H=g.addVert('H');
    capacs[g.addEdge(H,A,"ha")].capac=2;        capacs[g.addEdge(H,C,"hc")].capac=2;        capacs[g.addEdge(H,D,"hd")].capac=2;
    capacs[g.addEdge(A,G,"ag",EdDirOut)].capac=5;        capacs[g.addEdge(A,B,"ab")].capac=1;
    capacs[g.addEdge(C,G,"cg",EdDirOut)].capac=1;        capacs[g.addEdge(C,B,"cb",EdDirOut)].capac=3;
    capacs[g.addEdge(C,E,"ce",EdDirOut)].capac=1;        capacs[g.addEdge(G,F,"gf")].capac=3;        capacs[g.addEdge(B,F,"bf")].capac=2;
    capacs[g.addEdge(E,F,"ef")].capac=4;
    capacs[g.addEdge(F,H,"fh",EdDirOut)].capac=3; capacs[g.addLoop(C,"cc")].capac=2; capacs[g.addLoop(G,"gg")].capac=4;
    capacs[g.addEdge(D,B,"db")].capac=1;
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(tabV,g.getVertNo());
    PEdge LOCALARRAY(tabE,g.getEdgeNo());
    Flow::EdgeCut<int> res;
    if ((res=minEdgeCutExamp(g,capacs,start,end,Flow::outCut(tabV,tabE))).capac==-1) { cout << "Bledne dane"; return; }
    cout << "Rozciecie kraw. o minimalnej obj. z " << start->info << " do " << end->info << " ma objetosc " << res.capac << " a jego krawedzie: ";
    for(int i=0;i<res.edgeNo;i++) cout << tabE[i]-> info << ' ';
    cout << "\nPo ich usunieciu z " << start->info << " osiagalne w sieci beda ";
    for(int i=0;i<res.vertNo;i++) cout << tabV[i]-> info << ' ';
}

Flow::EdgeCut2<MyGraph,int> minEdgeCutExamp(const MyGraph& g, AssocArray<PEdge,Flow::EdgeLabs<int> >& cont, Flow::OutCut<PVertex*,PEdge*> out)
//Mozna tez udostepnic dla samego multigrafu mieszanego (bez cont) nadajac wowczas cont[..wszystkie ...].capac=1;
{
    Flow::EdgeCut2<MyGraph,int> blad; blad.capac=-1;
    if (g.getVertNo()<2) return blad;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (cont[e].capac <0) return blad;//bledne dane
    return Flow::minEdgeCut(g,cont,out); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
}

void minEdgeCut2Main()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
       AssocArray<PEdge,Flow::EdgeLabs<int> > capacs(100);
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    F=g.addVert('F');
    G=g.addVert('G');    H=g.addVert('H');
    capacs[g.addEdge(H,A,"ha")].capac=2;        capacs[g.addEdge(H,C,"hc")].capac=2;        capacs[g.addEdge(H,D,"hd")].capac=2;
    capacs[g.addEdge(A,G,"ag",EdDirOut)].capac=5;        capacs[g.addEdge(A,B,"ab")].capac=1;
    capacs[g.addEdge(C,G,"cg",EdDirOut)].capac=1;        capacs[g.addEdge(C,B,"cb",EdDirOut)].capac=3;
    capacs[g.addEdge(C,E,"ce",EdDirOut)].capac=1;        capacs[g.addEdge(G,F,"gf")].capac=3;        capacs[g.addEdge(B,F,"bf")].capac=2;
    capacs[g.addEdge(E,F,"ef")].capac=4;
    capacs[g.addEdge(F,H,"fh",EdDirOut)].capac=3; capacs[g.addLoop(C,"cc")].capac=2; capacs[g.addLoop(G,"gg")].capac=4;
    capacs[g.addEdge(D,B,"db")].capac=1;
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(tabV,g.getVertNo());
    PEdge LOCALARRAY(tabE,g.getEdgeNo());
    Flow::EdgeCut2<MyGraph,int> res;
    if ((res=minEdgeCutExamp(g,capacs,Flow::outCut(tabV,tabE))).capac==-1) { cout << "Bledne dane"; return; }
    cout << "Rozciecie kraw. sieci o minimalnej obj. ma objetosc " << res.capac <<
    " Rozcina ono siec pomiedzy wierzcholkami " << res.first->info << " a " << res.second->info << " a jego krawedzie: ";
    for(int i=0;i<res.edgeNo;i++) cout << tabE[i]-> info << ' ';
    cout << "\nPo ich usunieciu z " << res.first->info << " osiagalne w sieci beda ";
    for(int i=0;i<res.vertNo;i++) cout << tabV[i]-> info << ' ';
}

bool gomoryHuTreeExamp(MyGraph& g, AssocArray<PEdge,Flow::EdgeLabs<int> >& cont, Flow::GHTreeEdge<MyGraph,int>* out)
{
    if (g.getEdgeNo(Directed|Loop) || g.getVertNo()<2) return false; //bledne dane
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) if (cont[e].capac <0) return false;//bledne dane
    Flow::findGHTree(g,cont,out); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
    return true;
}

void gomoryHuTreeMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
       AssocArray<PEdge,Flow::EdgeLabs<int> > capacs(100);
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    F=g.addVert('F');
    G=g.addVert('G');    H=g.addVert('H');
    capacs[g.addEdge(H,A,"ha")].capac=2;        capacs[g.addEdge(H,C,"hc")].capac=2;        capacs[g.addEdge(H,D,"hd")].capac=2;
    capacs[g.addEdge(A,G,"ag",EdUndir)].capac=5;        capacs[g.addEdge(A,B,"ab")].capac=1;
    capacs[g.addEdge(C,G,"cg",EdUndir)].capac=1;        capacs[g.addEdge(C,B,"cb",EdUndir)].capac=3;
    capacs[g.addEdge(C,E,"ce",EdUndir)].capac=1;        capacs[g.addEdge(G,F,"gf")].capac=3;        capacs[g.addEdge(B,F,"bf")].capac=2;
    capacs[g.addEdge(E,F,"ef")].capac=4;
    capacs[g.addEdge(F,H,"fh",EdUndir)].capac=3; //capacs[g.addLoop(C,"cc")].capac=2; capacs[g.addLoop(G,"gg")].capac=4;
    capacs[g.addEdge(D,B,"db")].capac=1;
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    Flow::GHTreeEdge<MyGraph,int> LOCALARRAY(out,g.getVertNo()-1);
    if (!gomoryHuTreeExamp(g,capacs,out)) { cout << "Bledne dane"; return; }
    cout << "Krawedzie drzewa GH: \n";
    for(int i=0;i<g.getVertNo()-1;i++)
        cout << out[i].first->info << out[i].second->info  << " objetosc: " << out[i].capac << endl;
}

//int tylko dla zglaszania bledow
int transshipExamp(MyGraph& g, AssocArray<PEdge,Flow::TrsEdgeLabs<int> >& cont,
                   const AssocArray<PVertex,Flow::TrsVertLoss<int> >& vcont)
{
    if (g.getVertNo()<2) return -1; //bledne dane
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        if (cont[e].lo<0 || cont[e].hi<cont[e].lo || (g.getEdgeType(e)==Undirected && cont[e].lo!=0)) return -1;//bledne dane
    for(PVertex e=g.getVert();e;e=g.getVertNext(e)) if (vcont[e].hi<vcont[e].lo) return -1;
    return Flow::transship(g,cont,vcont); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
}

void transshipMain()
{
           MyGraph g;
       PVertex A,B,C,D,E,F,G,H,start,end;
       AssocArray<PEdge,Flow::TrsEdgeLabs<int> > edgecont;
       AssocArray<PVertex,Flow::TrsVertLoss<int> > vertcont;
    vertcont[A=g.addVert('A')]=Flow::TrsVertLoss<int>(0,0);
    vertcont[B=g.addVert('B')]=Flow::TrsVertLoss<int>(0,0); vertcont[C=g.addVert('C')]=Flow::TrsVertLoss<int>(0,0);
    vertcont[D=g.addVert('D')]=Flow::TrsVertLoss<int>(0,0); vertcont[E=g.addVert('E')]=Flow::TrsVertLoss<int>(0,0);
    vertcont[end=F=g.addVert('F')]=Flow::TrsVertLoss<int>(5,7); vertcont[G=g.addVert('G')]=Flow::TrsVertLoss<int>(0,0);
    vertcont[start=H=g.addVert('H')]=Flow::TrsVertLoss<int>(-7,-4);
    edgecont[g.addEdge(H,A,"ha")]=Flow::TrsEdgeLabs<int>(0,2);edgecont[g.addEdge(H,C,"hc")]=Flow::TrsEdgeLabs<int>(0,2);
    edgecont[g.addEdge(H,D,"hd")]=Flow::TrsEdgeLabs<int>(0,2);
    edgecont[g.addEdge(A,G,"ag",EdDirOut)]=Flow::TrsEdgeLabs<int>(0,5);edgecont[g.addEdge(A,B,"ab")]=Flow::TrsEdgeLabs<int>(0,1);
    edgecont[g.addEdge(C,G,"cg",EdDirOut)]=Flow::TrsEdgeLabs<int>(0,1);
    edgecont[g.addEdge(C,B,"cb",EdDirOut)]=Flow::TrsEdgeLabs<int>(1,3);
    edgecont[g.addEdge(C,E,"ce",EdDirOut)]=Flow::TrsEdgeLabs<int>(0,1);edgecont[g.addEdge(G,F,"gf")]=Flow::TrsEdgeLabs<int>(0,3);
    edgecont[g.addEdge(B,F,"bf")]=Flow::TrsEdgeLabs<int>(0,2);
    edgecont[g.addEdge(E,F,"ef")]=Flow::TrsEdgeLabs<int>(0,4);
    edgecont[g.addEdge(F,H,"fh",EdDirOut)]=Flow::TrsEdgeLabs<int>(0,3); edgecont[g.addLoop(C,"cc")]=Flow::TrsEdgeLabs<int>(0,2);
    edgecont[g.addLoop(G,"gg")]=Flow::TrsEdgeLabs<int>(0,4);
    edgecont[g.addEdge(D,B,"db")]=Flow::TrsEdgeLabs<int>(0,1);
    writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    int res=transshipExamp(g,edgecont,vertcont);
    if (res==-1)  { cout << "Bledne dane"; return; }
    if (!res) { cout << "Brak rozwiazania"; return; }
    cout << "Znaleziono rozwiazanie";
    cout << endl;
    for(PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
    {   cout << e->info << ':' << ((g.getEdgeType(e)!=Undirected) ? edgecont[e].flow : std::max(edgecont[e].flow,-edgecont[e].flow));
        if (g.getEdgeType(e)==Undirected && edgecont[e].flow>0) cout << " w kierunku " << g.getEdgeEnd1(e)->info << '>' <<g.getEdgeEnd2(e)->info;
        if (g.getEdgeType(e)==Undirected && edgecont[e].flow<0) cout << " w kierunku " << g.getEdgeEnd2(e)->info << '>' <<g.getEdgeEnd1(e)->info;
        cout << endl;
    }
}

int minVertCutExamp(const MyGraph& g,PVertex start, PVertex end,PVertex* out)
{
    if (!start || !end || !g.has(start) || !g.has(end) || start==end || g.getEdge(start,end,EdUndir|EdDirOut) ) return -1;//bledne dane
    return Connect::minVertCut(g,start,end,out); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
}

void minVertCutMain()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H,start,end;
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    end=F=g.addVert('F');
    G=g.addVert('G');    start=H=g.addVert('H');
    g.addEdge(H,A,"ha");        g.addEdge(H,C,"hc");        g.addEdge(H,D,"hd");
    g.addEdge(A,G,"ag",EdDirOut);5;        g.addEdge(A,B,"ab");1;
    g.addEdge(C,G,"cg",EdDirOut);1;        g.addEdge(C,B,"cb",EdDirOut);3;
    g.addEdge(C,E,"ce",EdDirOut);1;        g.addEdge(G,F,"gf");3;        g.addEdge(B,F,"bf");2;
    g.addEdge(E,F,"ef");4;
    g.addEdge(F,H,"fh",EdDirOut);3; g.addLoop(C,"cc");2; g.addLoop(G,"gg");4;
    g.addEdge(D,B,"db");1;
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(tabV,g.getVertNo());
    int res;
    if ((res=minVertCutExamp(g,start,end,tabV))==-1) { cout << "Bledne dane"; return; }
    cout << "Najmniejsze rozciecie wierzch  z " << start->info << " do " << end->info << " ma " << res << " wierzcholkow. \nSa to: ";
    for(int i=0;i<res;i++) cout << tabV[i]-> info << ' ';
}

int minVertCutExamp(const MyGraph& g,PVertex* out)
//Mozna tez udostepnic dla samego multi grafu mieszanego (bez cont) nadajac wowczas cont[..wszystkie ...].capac=1;
{
    if (g.getVertNo()<2) return -1;
    return Connect::minVertCut(g,out); //W zasadzie nalezy sie zdecydowac na jakis FlowPar z flagami wyboru algorytmow
}

void minVertCut2Main()
{
            MyGraph g;
       PVertex A,B,C,D,E,F,G,H;
   A=g.addVert('A');B=g.addVert('B');    C=g.addVert('C');    D=g.addVert('D');    E=g.addVert('E');    F=g.addVert('F');
    G=g.addVert('G');    H=g.addVert('H');
    g.addEdge(H,A,"ha");        g.addEdge(H,C,"hc");        g.addEdge(H,D,"hd");
    g.addEdge(A,G,"ag",EdDirOut);5;        g.addEdge(A,B,"ab");1;
    g.addEdge(C,G,"cg",EdDirOut);1;        g.addEdge(C,B,"cb",EdDirOut);3;
    g.addEdge(C,E,"ce",EdDirOut);1;        g.addEdge(G,F,"gf");3;        g.addEdge(B,F,"bf");2;
    g.addEdge(E,F,"ef");4;
    g.addEdge(F,H,"fh",EdDirOut);3; g.addLoop(C,"cc");2; g.addLoop(G,"gg");4;
    g.addEdge(D,B,"db");1;
//    g.clear();g.addEdge(g.addVert('Z'),g.addVert('A'));
   writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout <<"\n******\n";
    PVertex LOCALARRAY(tabV,g.getVertNo());
    int res;
    if ((res=minVertCutExamp(g,tabV))==-1) { cout << "Graf nie ma rozciecia wierzcholkowego"; return; }
    cout << "Najmniejsze rozciecie wierzch ma " << res << " wierzcholkow. \nSa to: ";
    for(int i=0;i<res;i++) cout << tabV[i]-> info << ' ';
}


#include "main.hpp"

    minVertCut2Main();

    return 0;
}

