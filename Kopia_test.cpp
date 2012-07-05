#include <iostream>
#include <string>

#include "Koala/container/hashcont.h"
#include "Koala/container/assoctab.h"

//#include "Koala/base/def_struct.h"
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
void t( Cont /*AssocArray<Vert*,std::string>*/ & arr)
{   std::cout << "size:" << arr.size() << std::endl;
    for(Vert* k=arr.firstKey();k;k=arr.nextKey(k)) std::cout << "(" << k->name << "," << arr[k] << ")";
    std::cout << std::endl;
    for(Vert* k=arr.lastKey();k;k=arr.prevKey(k)) std::cout << "(" << k->name << "," << arr[k] << ")";
    std::cout << std::endl;
}
//
//template <class A, class B, AssocMatrixType aType, class D, class E>
//void t(AssocMatrix<A,B,aType,D,E>& m)
//{  std::cout << "size:" << m.size();
//   for(std::pair<Vert*,Vert*> k=m.firstKey();k.first || k.second;k=m.nextKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)<<" ";
//   std::cout << std::endl;
//   for(std::pair<Vert*,Vert*> k=m.lastKey();k.first || k.second;k=m.prevKey(k)) std::cout<< "(" << k.first->name <<","<< k.second->name<<"):" <<m(k.first,k.second)<<" ";
//   std::cout << std::endl;
//   for(int i=0;i<4;i++) for (int j=0;j<4;j++) if (m.hasKey(vtab[i],vtab[j])) std::cout << vtab[i]->name <<"," <<vtab[j]->name << " ";
//   std::cout << std::endl;
//   for(int i=0;i<4;i++)  if (m.hasInd(vtab[i])) std::cout << vtab[i]->name << " ";
//   std::cout<<std::endl;
//}


int main() {

    std::map<char,int> imap;
    imap['a']=1;
    imap['b']=2;
    imap['c']=3;
    const std::map<char,int> simap=imap;



    AssocTabInterface<std::map<char,int> > inter=imap,inter2=imap;
    AssocTabConstInterface< std::map<char,int> > sinter=imap,
        sinter2=simap,
        sinter3=inter,sinter4=sinter3;

     AssocTabConstInterface<std::map<char,int> > cinter=simap;

     assocTabInterf(imap);
     assocTabInterf(simap);

     AssocTable<std::map<char,int> > amap=imap;




    #define  zmienna amap

    char i=zmienna.firstKey();
    do {
        cout << boolalpha << '(' << i << ',' << zmienna[i] << ','<< zmienna.hasKey(i)<<')';
        i=zmienna.nextKey(i);
    }
    while (i!=zmienna.lastKey());

    {
        AssocArray<Vert*,std::string> a(5);

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

//        delete B;
        t(a);
        t(b);
        t(a);
        t(b);

        AssocArray<Vert*,std::string> a2=a;
        AssocArray<Vert*,std::string> a3;
        a3=a;

        const AssocArray<Vert*,std::string> cb=b;
        t(cb);

        cout << cb[A];
    }

    {

        PseudoAssocArray<Vert*,std::string, AssocTable<std::map<Vert*,int > > > a(5);

        t(a);
        a[C]="Ca";
        a[B]="Ba";
        t(a);
        PseudoAssocArray<Vert*,std::string, AssocTable<std::map<Vert*,int > > > b;
        b[C]="x";
        b=a;
        t(a);
        t(b);
        b[A]="Ab";

//        delete B;
        t(a);
        t(b);
        t(a);
        t(b);

        PseudoAssocArray<Vert*,std::string, AssocTable<std::map<Vert*,int > > > a2=a;
        PseudoAssocArray<Vert*,std::string, AssocTable<std::map<Vert*,int > > > a3;
        a3=a;

        const PseudoAssocArray<Vert*,std::string, AssocTable<std::map<Vert*,int > > > cb=b;
        t(cb);

        cout << cb[A];
    }

    {
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

    }


}

