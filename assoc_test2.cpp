#include <iostream>
#include <string>

#include "Koala/container/hashcont.h"
#include "Koala/container/assoctab.h"

#include "Koala/base/defs.h"
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


#include "main.hpp"

    Privates::BlockOfBlockList< BlockOfAssocArray< Vert*,std::string > > bufor[10];
    std::map<int,std::string> mapa;
    AssocTabInterface<std::map<int,std::string> > amap=mapa;
//    AssocTabInterface<std::map<int,std::string> > aaa=assocTabInterf(mapa);



    AssocArray<Vert*,std::string> a(5,(int*)bufor);
    BlockOfAssocMatrix< std::string > matbuf[100];
    Privates::BlockOfBlockList< BlockOfAssocArray< Vert*,int > > indbuf[100];

    Vert * tab[5];




    std::cout << "\nMatrixy:\n";
    AssocMatrix<Vert*,std::string,AMatrClTriangle> m,m1(0,(Koala::Privates::AssocMatrixVectIntSwitch<AssocMatrix<Vert*,std::string,AMatrClTriangle> >::BufType)matbuf,(Koala::Privates::AssocMatrixVectIntSwitch<AssocMatrix<Vert*,std::string,AMatrClTriangle> >::BufType)indbuf);

    m(A,B)="Wpis wspolny";
    m(A,A)="Wpis pojedynczy";
    m(C,C)="Wpis";
    Delete (A);

    t(m);


    return 0;
}

