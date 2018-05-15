#include <iostream>
#include <string>
//#include <unordered_set>


#include "Koala/container/hashcont.h"
#include "Koala/container/assoctab.h"

#include "Koala/base/defs.h"
// komentarz
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

void Delete(Vert* v)
{
    delete v;
    for(int i=0;i<4;i++) if (vtab[i]==v) vtab[i]=0;
}

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
{
//    std::cout << "\n*a\n";
    std::cout << "size:" << m.size();
//    std::cout << "\n*b\n"<< std::endl;
   for(std::pair<Vert*,Vert*> k=m.firstKey();k.first || k.second;k=m.nextKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)<<" ";
//   std::cout << "\n*c\n";
   for(std::pair<Vert*,Vert*> k=m.lastKey();k.first || k.second;k=m.prevKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)<<" ";
//   std::cout << "\n*d\n";
   for(int i=0;i<4;i++) if (vtab[i]) for (int j=0;j<4;j++) if (vtab[j] && m.hasKey(vtab[i],vtab[j])) std::cout << vtab[i]->name <<"," <<vtab[j]->name << " ";
//   std::cout << "\n*e\n";
   for(int i=0;i<4;i++)  if (vtab[i] && m.hasInd(vtab[i])) std::cout << vtab[i]->name << " ";
//   std::cout<<5 <<std::endl;
}


template <class Cont>
struct AssocArrSwitch {

    typedef Cont Type;
};


template <class K, class E, class Cont>
struct AssocArrSwitch<AssocArray<K,E,Cont> > {

    typedef AssocTable<std::map< K, E> > Type;
};

int xxx=0;

#include "main.hpp"

    Privates::BlockOfBlockList< Privates::BlockOfAssocArray< Vert*,std::string > > bufor[10];
    std::map<int,std::string> mapa;
    AssocTabInterface<std::map<int,std::string> > amap=mapa;
//    AssocTabInterface<std::map<int,std::string> > aaa=assocTabInterf(mapa);


    int xxxtab[xxx];

    AssocArray<Vert*,std::string> a(5);
    Privates::BlockOfAssocMatrix< std::string > matbuf[100];
    Privates::BlockOfBlockList< Privates::BlockOfAssocArray< Vert*,int > > indbuf[100];

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
//    Delete( B);
    t(a);
    t(b);
    a.clear();
    t(a);
    t(b);
    std::cout << "\nCap=" << b.capacity() <<"\n";


//    Delete( B);Delete( A);
    b.defrag();
    t(b);
    std::cout << "\nCap=" << b.capacity() <<"\n";
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

 AssocTable< BiDiHashMap<Vert*,std::string> > hha;
 hha.reserve(5);


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
//    Delete (B);
    t(ha);
    t(hb);
    ha.clear();
    t(ha);
    t(hb);


    AssocTable< BiDiHashMap<Vert*,std::string> > ha2=ha;


    std::cout << "\nMatrixy:\n";
    AssocMatrix<Vert*,std::string,AMatrClTriangle> m,m1;

    m(A,B)="\nWpis wspolny";
    m(A,A)="\nWpis pojedynczy";
    m(C,C)="\nWpis";
    t(m);
    m.delKey(C,C);
    m.defrag();
    m1=m;
    m.defrag();
    t(m);
//    Delete (D);
    m.defrag();
    t(m);
    m1.defrag();
    t(m1);

    const AssocMatrix<Vert*,std::string,AMatrClTriangle> cm=m;
    t(m);


    std::cout << "\nMatrixy2:\n";
    AssocMatrix<Vert*,std::string,AMatrClTriangle,std::vector< Privates::BlockOfAssocMatrix< std::string > >
         ,Koala::Privates::PseudoAssocArray<Vert*,int,AssocTable<std::map<Vert*,int> > > > pm,pm1;

    pm(A,B)="Wpis wspolny";
    pm(A,A)="Wpis pojedynczy";
    pm(C,C)="Wpis";
    t(pm);
    pm.delKey(C,C);
    pm.defrag();
    pm1=pm;
    pm.defrag();
    t(pm);
//    Delete (D);
    pm.defrag();
    t(pm);
    pm1.defrag();
    t(pm1);


    std::cout << "\nMatrixy3:\n";
    AssocMatrix<Vert*,std::string,AMatrClTriangle>
        lpm(4);


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
//    Delete (D);
    lpm.defrag();
    t(lpm);
    pm1.defrag();
    t(lpm);
//    lpm.clear();
//    std::cout << lpm << "\n";

//    AssocTable<std::map<std::string,int> > atab;
//    AssocTabInterface< AssocTable<std::map<std::string,int> > > iatab=atab,tatab2=iatab;
    std::cout << "\n*************\n";
    Koala::Privates::PseudoAssocArray<Vert*,std::string,AssocTable<std::map<Vert*,int> > > pst,pst2(1);
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
    Koala::Privates::PseudoAssocArray<Vert*,std::string,AssocTable<std::map<Vert*,int> > > pst3=pst;
    t(pst3);
//    assocTabInterf(pst3).getKeys(blackHole);

    std::cout << "\n***Locals***\n";


    AssocArray<Vert*,std::string>
        vitab(3);



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
    Delete(C);
    t(aaa);
    t(cona);

    cout << "\n" << pairMinMax(5,5).first << ' ' << pairMinMax(4,5).second;

    AssocTable<HashMap<int,std::string> > hmp;
    !hmp;
    hmp[3]="A";hmp[6]="B";;hmp[7]="C";
    cout << endl << hmp[3] << hmp.size();
    hmp.reserve(200000);
    cout << hmp[3] << hmp.size();
    {
        cout << "\n(((((((((((((((1\n\n";
        std::map<Vert*,std::string> mapa,mapa2;
        mapa[A]="A";
        AssocTabConstInterface<std::map<Vert*,std::string> > inf(mapa);
        AssocTabInterface<std::map<Vert*,std::string> > inf2(mapa),inf3(inf2),inf4(mapa2);
        std::cout << "\n\n\n*******\n\n\n" << Privates::asssocTabInterfTest(mapa) << ' ' <<
            Privates::asssocTabInterfTest(inf) << ' ' << Privates::asssocTabInterfTest(inf2) << ' ' << (&mapa) << ' ' << (Privates::asssocTabInterfTest(inf2)==&mapa)<< "\n\n\n*******\n\n\n";
        inf3[B]="B";
        inf3=inf;
        t(inf3);
        inf4=inf2;
        t(assocTabInterf(mapa2));

        AssocTable<std::map<Vert*,std::string> > tab,tab2=tab;
        AssocTable<std::map<int,std::string> > imapa;

        AssocArray<Vert*,std::string>  aa,aa2(aa);
        aa[B]="BB";
        aa2=inf;
        t(aa2);
        cout << "\n\n";
        t(assocTabInterf(aa2));
        AssocTable<AssocArray<Vert*,std::string> > tabaa2;
        tabaa2=assocTab(aa2);
        AssocTabInterface<AssocArray<Vert*,std::string> > intaa2(aa2);
        intaa2[A]="Ala_";
        cout << "\n\n";
        t(intaa2);

        AssocArrSwitch<AssocArray<Vert*,std::string> >::Type  hmp2;
        hmp2=aa;

        Koala::Privates::PseudoAssocArray<Vert*,std::string,AssocTable<std::map<Vert*,int> > >  pa;
        pa[B]="BB";
        Koala::Privates::PseudoAssocArray<Vert*,std::string, AssocTable<std::map<Vert*,int> > >  pa2(pa);
        pa2=pa;
        aa=pa2;
        t(aa);
        AssocTable<BiDiHashMap<Vert*,std::string> > hmapa;
        hmapa=pa;
        t(hmapa);

        AssocMatrix<Vert*,std::string,AMatrFull,std::vector< Privates::BlockOfAssocMatrix< std::string > >
         ,Koala::Privates::PseudoAssocArray<Vert*,int,AssocTable<std::map<Vert*,int> > > > mat,mat2;
        mat(A,B)="AB";
        mat2=mat;
        t(mat2);
        AssocMatrix<Vert*,std::string,AMatrFull> mat3;
//        mat3(A,B)="AB";
        mat3=mat;
        t(mat3);


    }
    {
        cout << "\n\n\n";
        AssocTable<BiDiHashMap<int,std::string> > hmp;
        hmp[3]="A";hmp[6]="B";;hmp[7]="C";
        hmp=hmp;
        cout << hmp.size() << ' ' << hmp[3] << hmp[6];

    }
    {
        SimpleAssocMatrix<Vert*,std::string,AMatrFull> m;
        AssocArray<Vert*,std::string> vec;

        Vert *A=new Vert("Ala"),*B=new Vert("Basia"),*C=new Vert("Celina"),*D=new Vert("Dagmara");

        cout << "\n slice:\n\n";
        m(A,B)="\nWpis wspolny";
        m(A,A)="\nWpis pojedynczy";
        m(C,C)="\nWpis";
		m.slice1(C, vec);
        t(vec);
    }
//    unordered_set<std::string> haset;
//    haset.insert("ala");
//    unordered_set<std::string>::iterator hait=haset.begin();
//    ++hait;--hait;

    return 0;
}

