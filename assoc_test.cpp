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
void t(const Cont /*AssocArray<Vert*,std::string>*/ & arr)
{   std::cout << "size:" << arr.size() << std::endl;
    for(Vert* k=arr.firstKey();k;k=arr.nextKey(k)) std::cout << "(" << k->name << "," << arr[k] << ")";
    std::cout << std::endl;
    for(Vert* k=arr.lastKey();k;k=arr.prevKey(k)) std::cout << "(" << k->name << "," << arr[k] << ")";
    std::cout << std::endl;
}

template <class A, class B, AssocMatrixType aType, class D, class E>
void t(const AssocMatrix<A,B,aType,D,E>& m)
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

    BlockOfBlockList< BlockOfAssocArray< Vert*,std::string > > bufor[10];
    std::map<int,std::string> mapa;
    AssocTabInterface<std::map<int,std::string> > amap=mapa;
//    AssocTabInterface<std::map<int,std::string> > aaa=assocTabInterf(mapa);



    AssocArray<Vert*,std::string> a(5,(int*)bufor);
    BlockOfAssocMatrix< std::string > matbuf[100];
    BlockOfBlockList< BlockOfAssocArray< Vert*,int > > indbuf[100];

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
    AssocArray<Vert*,std::string> a3;
    a3=a;


//    AssocTable<AssocArray<Vert*,std::string> > ax(20);
//    ax.reserve(30);
//    ax.capacity();

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
    AssocMatrix<Vert*,std::string,AMatrClTriangle> m,m1(0,(AssocMatrixVectIntSwitch<AssocMatrix<Vert*,std::string,AMatrClTriangle> >::BufType)matbuf,(AssocMatrixVectIntSwitch<AssocMatrix<Vert*,std::string,AMatrClTriangle> >::BufType)indbuf);

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

    const AssocMatrix<Vert*,std::string,AMatrClTriangle> cm=m;
    t(m);

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


    std::cout << "\nMatrixy3:\n";
    AssocMatrix<Vert*,std::string,AMatrClTriangle,VectorInterface< BlockOfAssocMatrix< std::string > *>
         ,AssocArray<Vert*,int,VectorInterface< BlockOfBlockList< BlockOfAssocArray< Vert*,int > > *> > >
        lpm(4,(AssocMatrixVectIntSwitch<AssocMatrix<Vert*,std::string,AMatrClTriangle,VectorInterface< BlockOfAssocMatrix< std::string > *>
         ,AssocArray<Vert*,int,VectorInterface< BlockOfBlockList< BlockOfAssocArray< Vert*,int > > *> > > >::BufType)matbuf,(AssocMatrixVectIntSwitch<AssocMatrix<Vert*,std::string,AMatrClTriangle,VectorInterface< BlockOfAssocMatrix< std::string > *>
         ,AssocArray<Vert*,int,VectorInterface< BlockOfBlockList< BlockOfAssocArray< Vert*,int > > *> > > >::IndBufType)indbuf);


    lpm(A,B)="Wpis skasowany";
    lpm(A,B).clear();
    lpm(A,B)="Wpis wspolny";
    lpm(A,A)="Wpis pojedynczy";
    lpm(C,C)="Wpis";
    lpm(D,D)="Wpis2";
    t(lpm);

    lpm.delKey(C,C);
    t(lpm);
    std::cout <<"\ndefr\n";
    lpm.defrag();
//    pm1=pm;
//    lpm.defrag();
    t(lpm);
//    delete D;
    lpm.defrag();
    t(lpm);
    pm1.defrag();
    t(lpm);
//    lpm.clear();
//    std::cout << lpm << "\n";

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

    std::cout << "\n***Locals***\n";


    AssocArray<Vert*,std::string,VectorInterface<BlockOfBlockList< BlockOfAssocArray< Vert*,std::string > >*> >
        vitab(3,bufor);



    t(vitab);
    vitab[A]="Ala";
    t(vitab);
    vitab[B]="Basia";
    t(vitab);
    vitab[C]="Celina";
//    vitab[D]="Celina";
    t(vitab);
    vitab.delKey(B);
    t(vitab);
    vitab.delKey(A);
    t(vitab);
    vitab[A]="Ania";
    t(vitab);
    vitab.defrag();
    t(vitab);
    vitab.clear();
    t(vitab);
    vitab[D]="Dagmara";
    t(vitab);

    std::cout << "\n\n" << AssocArrayVectIntSwitch<AssocArray<Vert*,std::string,VectorInterface<BlockOfBlockList< BlockOfAssocArray< Vert*,std::string > >*> >  >::isAAVI();

    cout << "\n\n\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n";

#define TYPE AssocArray<Vert*,std::string>
//#define TYPE AssocTable<std::map<Vert*,std::string> >
//#define TYPE AssocTable<BiDiHashMap<Vert*,std::string> >

    TYPE  aaa;
    aaa[A]="Ala";aaa[C]="celina";
    const TYPE   cona=aaa;
    t(cona);
    cona[D]="A";
    aaa[D]="A";
    t(cona);
    t(aaa);
    aaa.delKey(D);
//    cona.delKey(D);
    cout<< aaa << endl<<endl;

    std::map<Vert*,std::string> ama;
    ama[D]="AAA";
    const std::map<Vert*,std::string> cama=ama;
    cout << assocTabInterf(cama);

    //assocInserter(assocTabInterf(ama))=std::make_pair(A,"AAA");
    t(assocTabInterf(ama));

    cout << endl<<endl;
    aaa=cona;
    t(cona);
    delete C;
    t(aaa);
    t(cona);

//    AssocMatrix<Vert*,std::string,AMatrFull> mmm;
//    mmm(C,D)="CD";
//    const AssocMatrix<Vert*,std::string,AMatrFull> conm=mmm;
//    t(mmm);
//    t(conm);
//    mmm(A,C)="AC";
//    conm(A,C)="AC";
//    cout << endl;
//    t(mmm);
//    cout << "\nconst " <<endl;
//    t(conm);
//    mmm.delKey(A,C);
////    conm.delKey(A,C);
//    t(mmm);
//    t(conm);
//
//    cout << mmm;

}

