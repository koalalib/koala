#include <iostream>
#include <string>
#include "Koala/base/defs.h"
#include "Koala/container/assoctab.h"

using namespace std;
using namespace Koala;


#define DEB printf("%d\n",__LINE__); fflush(stdout);


// testy, przyklad uzycia
struct InfA {
    std::string name;
    void* point;
    InfA(std::string aname) : name(aname) {}
    InfA() {}
};

InfA *A=new InfA("A"), *B=new InfA("B"), *C=new InfA("C");
InfA *tab[3]={A,B,C};


std::map<InfA*,int> amap;
AssocTabInterface<std::map<InfA*,int> > imap=amap;

struct InfB {
    std::string name;
    int* pointer;
    InfB() {}
};

class VertexA {
    public:
    InfA info;
} *AA =new VertexA;

class VertexB {
    public:
    InfB info;
} *BB =new VertexB;


template <class T> void bht2(T& arg)
{
    if (!isBlackHole(arg)) std::cout << (char)arg[0] << "\n";
    else std::cout << "Zakazane\n";
}


void bht()
{   int i;
    std::string  s="a";
    blackHole="ala";
    (*blackHole)="ala";
//    (blackHole<std::string>())++;
//    blackHole()[i]='a';
//    blackHole()(4,s)='b';
    std::cout << isBlackHole(i) <<"\n";
    std::cout << isBlackHole(blackHole) <<"\n";
//    std::cout << isBlackHole(blackHole<std::string>()) <<"\n";
//    bht2(i);
    bht2(blackHole);
}


#include "main.hpp"

    bht();

    stdChoose(true);

    AA->info.name="A"; AA->info.point=0;
    BB->info.name="B"; BB->info.pointer=0;

    std::map<VertexA*,VertexB*> a2b;
    std::map<VertexB*,VertexA*> b2a;
    AssocTabInterface<std::map<VertexB*,VertexA*> > ib2a=b2a;


    stdLink(&InfA::point,ib2a)(AA,BB);


    std::cout << AA->info.name << " pointing: " << ((void*)AA->info.point==(void*)BB) << " mapsize: "
            << a2b.size() << " " << (a2b.size() ? a2b[AA]->info.name : string("")) << std::endl;

    std::cout << BB->info.name << " pointing: " << ((void*)BB->info.pointer==(void*)AA) << " mapsize: "
            << b2a.size() << " " << (b2a.size() ? b2a[BB]->info.name : string("")) << std::endl;

    AssocValChooser<AssocTable<std::map<InfA*,int> > > b;

    return 0;
}

