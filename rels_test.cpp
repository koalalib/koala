#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "Koala/container/assoctab.h"
#include "Koala/graph/graph.h"
#include "Koala/graph/subgraph.h"
#include "Koala/base/def_struct.h"


using namespace std;
using namespace Koala;


bool tab[10][10],ttab[10][10]={{1,1},{1}},tttab[10][10]={{0,1},{1,1}};
bool (*wtab)[10]=tab;
vector<vector<bool> > vec;
vector<bool> tab2[10];
vector<bool> *wtab2=tab2;

class FunInterf {
    public:

    bool (*tab)[10];
    FunInterf(bool (*wsk)[10]) : tab(wsk) {}
    bool& operator()(int a,int b) const { return tab[a][b]; }
};

#define rel loctab

int main()
{   vec.resize(10); for(int i=0;i<10;i++) vec[i].resize(10);
    for(int i=0;i<10;i++) tab2[i].resize(10);
//    tab3.resize(10);
    cout << boolalpha;

    int a,b;
    a=10; b=20;
    bool loctab[100][100];

    int cont[5]={0,1,2,3,4};
    FunInterf interf(tab),iinterf(ttab),iiinterf(tttab);
    (FunInterf(tab))(2,3)=true;

    cout<< "\nempty:\n";
    RelClosure::empty(interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\ntotal:\n";
    RelClosure::empty(interf,cont,cont+5);
    RelClosure::total(interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\nand:\n";
    RelClosure::empty(interf,cont,cont+5);
    RelClosure::reland(iinterf,iiinterf,interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\nor:\n";
    RelClosure::empty(interf,cont,cont+5);
    RelClosure::relor(iinterf,iiinterf,interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\nxor:\n";
    RelClosure::empty(interf,cont,cont+5);
    RelClosure::relxor(iinterf,iiinterf,interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\nnot:\n";
    RelClosure::empty(interf,cont,cont+5);
    RelClosure::relnot(iinterf,interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\nrefl:\n";
    RelClosure::empty(interf,cont,cont+5);
    tab[0][1]=tab[0][0]=tab[1][2]=true;
    RelClosure::reflexive(interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\nsym:\n";
    RelClosure::empty(interf,cont,cont+5);
    tab[0][1]=tab[0][0]=tab[1][2]=true;
    RelClosure::symmetric(interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout<< "\ntran:\n";
    RelClosure::empty(interf,cont,cont+5);
    tab[0][1]=tab[0][0]=tab[1][2]=true;
    RelClosure::transitive(interf,cont,cont+5);
    RelClosure::print(interf,cont,cont+5,cout);

    cout << "\n-------------\n";
    Graph<char> g;
    Graph<char>::PVertex U,V,W;

    std::pair<Graph<char>::PVertex,Graph<char>::PVertex> ends;
    AssocMatrix<Graph<char>::PVertex,bool,AMatrFull> matr;

    U=g.addVert('A');V=g.addVert('B'); W=g.addVert('C');
    g.addArch(U,V);g.addEdge(V,W);g.addLoop(W);

    makeSubgraph(g,make_pair(stdChoose(true),stdChoose(true))).getAdj(matr);

    for(ends=matr.firstKey();ends.first;ends=matr.nextKey(ends))
        cout << "<" << ends.first->info << ',' << ends.second->info << ">\n";

    return EXIT_SUCCESS;
}
