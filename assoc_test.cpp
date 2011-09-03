#include <iostream>
#include <string>

#include "Koala/container/hashcont.h"
#include "Koala/container/assoctab.h"

#include "Koala/base/def_struct.h"
using namespace std;
using namespace Koala;


#define DEB printf("%d\n",__LINE__); fflush(stdout);

class Vert  {
    public:

    Vert(std::string aname="") : name(aname) {}
    std::string name;
    int a;
    AssocKeyContReg assocReg;
};


Vert *A=new Vert("Ala"),*B=new Vert("Basia"),*C=new Vert("Celina"),*D=new Vert("Dagmara");

Vert* vtab[4]={A,B,C,D};

template <class Cont>
void t(Cont /*AssocArray<Vert*,std::string>*/ & arr)
{   std::cout << "size:" << arr.size() << std::endl;
    for(Vert* k=arr.firstKey();k;k=arr.nextKey(k)) std::cout << "(" << k->name << "," << arr[k] << ")";
    std::cout << std::endl;
    for(Vert* k=arr.lastKey();k;k=arr.prevKey(k)) std::cout << "(" << k->name << "," << arr[k] << ")";
    std::cout << std::endl;
}

template <class A, class B, AssocMatrixType aType, class D, class E>
void t(AssocMatrix<A,B,aType,D,E>& m)
{  std::cout << "size:" << m.size();
   for(std::pair<Vert*,Vert*> k=m.firstKey();k.first || k.second;k=m.nextKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)<<" ";
   std::cout << std::endl;
   for(std::pair<Vert*,Vert*> k=m.lastKey();k.first || k.second;k=m.prevKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)<<" ";
   std::cout << std::endl;
   for(int i=0;i<4;i++) for (int j=0;j<4;j++) if (m.hasKey(vtab[i],vtab[j])) std::cout << vtab[i]->name <<"," <<vtab[j]->name << " ";
   std::cout << std::endl;
   for(int i=0;i<4;i++)  if (m.hasInd(vtab[i])) std::cout << vtab[i]->name << " ";
   std::cout<<std::endl;
}


int main() {

    std::map<int,std::string> mapa;
    AssocTabInterface<std::map<int,std::string> > amap=mapa;



    AssocArray<Vert*,std::string> a;
    Vert * tab[5];



    t(a);
    a[C]="Ca";
    a[B]="Ba";
    t(a);
    AssocArray<Vert*,std::string> b;
    b[C]="x";
    b=a;
    t(a);
    t(b);
    b[A]="Ab";
//    delete B;
    t(a);
    t(b);
    a.clear();
    t(a);
    t(b);

    AssocArray<Vert*,std::string> a2=a;

    AssocTable<AssocArray<Vert*,std::string> > ax(20);
    ax.reserve(30);
    ax.capacity();

    std::cout << "\n----------------\n";

    BiDiHashMap<int,std::string> hmapa;
    AssocTabInterface<BiDiHashMap<int,std::string> > hamap=hmapa;


    AssocTable< BiDiHashMap<Vert*,std::string> > ha(10);



    t(ha);
    ha[C]=std::string("Ca");
    ha[B]="Ba";
    t(ha);
    AssocTable< BiDiHashMap<Vert*,std::string> > hb;
    hb[C]="x";
    hb=ha;
    t(ha);
    t(hb);
    hb[A]="Ab";
//    delete B;
    t(ha);
    t(hb);
    ha.clear();
    t(ha);
    t(hb);


    AssocTable< BiDiHashMap<Vert*,std::string> > ha2=ha;


    std::cout << "\nMatrixy:\n";
    AssocMatrix<Vert*,std::string,AMatrClTriangle> m,m1;

    m(A,B)="Wpis wspolny";
    m(A,A)="Wpis pojedynczy";
    m(C,C)="Wpis";
    t(m);
    m.delKey(C,C);
    m.defrag();
    m1=m;
    m.defrag();
    t(m);
//    delete D;
    m.defrag();
    t(m);
    m1.defrag();
    t(m1);

    std::cout << "\nMatrixy2:\n";
    AssocMatrix<Vert*,std::string,AMatrClTriangle,std::vector< BlockOfAssocMatrix< std::string > >
         ,PseudoAssocArray<Vert*,int,AssocTable<std::map<Vert*,int> > > > pm,pm1;

    pm(A,B)="Wpis wspolny";
    pm(A,A)="Wpis pojedynczy";
    pm(C,C)="Wpis";
    t(pm);
    pm.delKey(C,C);
    pm.defrag();
    pm1=pm;
    pm.defrag();
    t(pm);
//    delete D;
    pm.defrag();
    t(pm);
    pm1.defrag();
    t(pm1);

//    AssocTable<std::map<std::string,int> > atab;
//    AssocTabInterface< AssocTable<std::map<std::string,int> > > iatab=atab,tatab2=iatab;
    std::cout << "\n*************\n";
    PseudoAssocArray<Vert*,std::string,AssocTable<std::map<Vert*,int> > > pst,pst2(1);
    t(pst);
    std::cout << "\nCapac:" << pst.capacity() << '\n';
    pst[A]="ala";
    t(pst);
    std::cout << "\nCapac:" << pst.capacity() << '\n';
    pst[B]="basia";
    t(pst);
    std::cout << "\nCapac:" << pst.capacity() << '\n';
    pst[A]="alicja";
    t(pst);
    std::cout << "\nCapac:" << pst.capacity() << '\n';
    pst[C]="celina";
//    pst.delKey(B);
    pst[D]="dagmara";
    t(pst);
    std::cout << "\nCapac:" << pst.capacity() << '\n';
    pst.defrag();
    t(pst);
    std::cout << "\nCapac:" << pst.capacity() << '\n';
    PseudoAssocArray<Vert*,std::string,AssocTable<std::map<Vert*,int> > > pst3=pst;
    t(pst3);
//    assocTabInterf(pst3).getKeys(blackHole);

}

