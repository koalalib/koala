#define NDEBUG
#include <iostream>
#include <string>

#include "Koala/container/hashcont.h"
#include "Koala/container/assoctab.h"

#include "Koala/base/defs.h"
using namespace std;
using namespace Koala;


#define DEB printf("%d\n",__LINE__); fflush(stdout);


class ADSettings
	{
	public:
		// Typ klasy tablicy asocjacyjnej przypisuj¹cej np. wierzcho³kowi/krawêdzi wartoœci typu B.
		/** \brief Type of associative container
		 *
		 *  \tparam A key type.
		 *  \tparam B mapped value type. */
		template< class A, class B > class AssocCont
		{
		public:
			typedef AssocArray< A,B > Type;/**<\brief Define own if intend to change.*/
			//  Nie usuwac komentarzy (przykladowa uzycia)
			//   typedef AssocTable < BiDiHashMap<A,B> > Type;
			//   typedef Privates::PseudoAssocArray<A,B,AssocTable<std::map<A,int> > > Type;
		};
	};


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

template <AssocMatrixType aType, class D>
void t(const Assoc2DimTable<aType,D>& m)
{
    Vert* smiec[200];
//    std::cout << "\n*a\n";
    std::cout << "size: " << m.size() << "\nWpisy:\n" << std::boolalpha;
//    std::cout << "\n*b\n"<< std::endl;
   for(std::pair<Vert*,Vert*> k=m.firstKey();k.first || k.second;k=m.nextKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)
        <<" "// <<(m.valPtr(k.first,k.second)==&m(k.first,k.second))
        <<"   ";
    std::cout << "\nWpisy (back):\n";
   for(std::pair<Vert*,Vert*> k=m.lastKey();k.first || k.second;k=m.prevKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)
        <<" " //<<(m.valPtr(k.first,k.second)==&m(k.first,k.second))
        <<"   ";
    std::cout << "\nPozycje:\n";
   for(int i=0;i<4;i++) if (vtab[i]) for (int j=0;j<4;j++) if (vtab[j] && m.hasKey(vtab[i],vtab[j])) std::cout << vtab[i]->name <<"," <<vtab[j]->name << "   ";
    std::cout << "\nIndeksy:\n";
    for(int i=0;i<4;i++) if (vtab[i] && m.hasInd(vtab[i])) std::cout <<vtab[i]->name << "   ";
    std::cout << "\n";
    int isize=m.template getInds< ::ADSettings >(smiec);
    for(int i=0;i<isize;i++) std::cout <<smiec[i]->name << "   ";
    std::cout << "\n-------------\n";

}

template <class A, class B, AssocMatrixType aType, class D, class E>
void t(const SimpleAssocMatrix<A,B,aType,D,E>& m)
{
    Vert* smiec[200];
    std::pair<Vert*,Vert*> smiec2[200];
//    std::cout << "\n*a\n";
    std::cout << "size: " << m.size() << std::boolalpha;
//    std::cout << "\n*b\n"<< std::endl;
    std::cout << "\nPozycje:\n";
   for(int i=0;i<4;i++) if (vtab[i]) for (int j=0;j<4;j++) if (vtab[j] && m.hasKey(vtab[i],vtab[j]))
        std::cout << '('<< vtab[i]->name <<"," <<vtab[j]->name << "): " <<m(vtab[i],vtab[j]) << ' '
//                <<(std::make_pair(vtab[i],vtab[j])==m.key(vtab[i],vtab[j]))
                <<"    ";
    std::cout << "\nKlucze:\n";
    int isize=m.getKeys(smiec2);
    for(int i=0;i<isize;i++) std::cout <<smiec2[i].first->name <<',' << smiec2[i].second->name  << "   ";
    std::cout << "\nIndeksy:\n";
    for(int i=0;i<4;i++) if (vtab[i] && m.hasInd(vtab[i])) std::cout <<vtab[i]->name << "   ";
    std::cout << "\n";
    isize=m.getInds(smiec);
    for(int i=0;i<isize;i++) std::cout <<smiec[i]->name << "   ";
    std::cout << "\n-------------\n";

}


//		AMatrFull, AMatrNoDiag, AMatrTriangle, AMatrClTriangle

#define AType AMatrClTriangle


#include "main.hpp"






    SimpleAssocMatrix<Vert*,std::string,AType> m(5),m2;
    m(A,B)="Wpis AB";
//    m(B,A)="Wpis BA";
    m(A,A)="Wpis AA";
    m(A,B)="Wpis AB v2";
    m(C,C)="Wpis CC";
    m(A,C)="Wpis AA v1";
    m(C,A)="Wpis CA";
    m(C,A)="Wpis CA v2";
    t(m);

//    m2=m;
    Delete(C);
    t(m);
    //Delete(A);
//    m.defrag();
//    t(m);
//    m(B,D)="Wpis AB";
//    m(D,B)="Wpis BA";
//    m(D,D)="Wpis AA";
//    t(m);
//    m(C,A)="Wpis CA v2";
//    AssocMatrix<Vert*,std::string,AType> m2;
//    m2=m;
//        m.clear();
//    t(m);
//
//    t(m2);
//    m=m2;
//    t(m);
//
//    AssocMatrix<Vert*,std::string,AType> am;
//    am=m;
//    t(am);
//    m3=am;
//    t(m3);

//    cout << m.valPtr(C,D);;

    //. getInds< ::AlgsDefaultSettings>(tabsmiec);
    Assoc2DimTable<AType,std::map<std::pair<Vert*,Vert*>,std::string> > mp;
    mp=m;


    return 0;
}

