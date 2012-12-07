#include <iostream>
#include "Koala/algorithm/search.h"

using namespace Koala;
using namespace std;


template<class T> void print(const SearchStructs::CompStoreTool<T>& cont)
{
    cout << "\nsize: " << cont.size() << "\tlen:" <<cont.length();;
    for(int i=0;i<cont.size();i++)
    {
        cout << "\n"<<cont.size(i) << ":";
        for(int j=0;j<cont.size(i);j++) cout << ' ' << cont[i][j];
    }
}


template <class Elem> class RekSet {

        Elem a;
        Koala::Set<RekSet<Elem>* > children;

    public:

    RekSet() : a(0), children() {}

    void clear()
        { a=0; children.clear(); }

    RekSet& operator+=(RekSet<Elem>& X)
    {
        if (&X==this) return *this;
        children+=& X;
        return *this;
    }

    RekSet& operator+=(Elem e)
    {
        assert(!a);
        a=e;
        return *this;
    }

    int size()
    {
        int res=(a) ? 1 : 0;
        for(RekSet<Elem>* w=children.first();w;w=children.next(w))
                res+=w->size();
        return res;
    }

    template <class Iter>
    void getElements(Iter& out)
    {
        if (a)
        {
            *out=a; ++out;
        }
        for(RekSet<Elem>* w=children.first();w;w=children.next(w))
            w->getElements(out);
    }

};

#include "main.hpp"

    RekSet<char> rs1,rs2,rs3;
    char text[10]={0},*ptext=text;

    cout << rs1.size();
//    rs1+='A';
    rs2+='B';
    rs3+='C';
    rs1+=rs2; rs2+=rs3;
    cout << ' ' << rs1.size() << endl;
    rs1.getElements(ptext);

    for(int i=0;i<10;i++) if (text[i]) cout << text[i] << ' ';
    cout << "\n-----------\n";


    SearchStructs::CompStoreTool<char> cont;
    SearchStructs::CompStoreTool<int> rev;
    char tab[]={'.','.','.','.','.','.','.','.','.','.','.','.','.','.'};

    cont.insert(0);
//    cout << "\ninsert\n";
    cont.insert(0);
//    cout << "\ninsert\n";
    cont.insert(0);
//    cout << "\ninsert\n";
    cont.resize(0,2);
//    cout << "\nresize\n";
    cont.resize(1,1);
    cont[1];
//    cont[0][0]='d';
    cont.resize(2,4);
    cout << "\nresize\n";
    cont[0][0]='a'; cont[0][1]='b';
    cont[1][0]='c';
    cont[2][0]='a';cont[2][1]='b';cont[2][2]='c';cont[2][3]='a';
    cont.insert(0);cont.insert(2);cont.insert(4);
    cont.resize(0,1);cont[0][0]='d';
    cout << "\n!"<< cont.size(1) <<"\n";
//    cont.del(0);cont.del(0);cont.del(0);cont.del(0);cont.del(0);cont.del(0);
//    cont.insert(0);cont.insert(0);
//    cont.resize(0,1);cont.resize(1,1);
//    cont[0][0]='a';
//    cont[1][0]='c';

    print(cont);

    cout << endl;
    vector<int> vec;
    char *ptr=0;
    vec.push_back(0);
    for(int i=0;i<cont.size();i++)
    {   vec.push_back(vec.back()+cont.size(i));
        if (!ptr) ptr=cont[i];
    }




    cout << endl << SearchStructs::revCompStore(vec.begin(),ptr,cont.size(),rev.input(),tab);
    cout << endl;
    for(int i=0;i<10;i++) cout << tab[i] << ' ';
    cout << endl;
    print(rev);


//    cont.insert(0);
//    print(cont);
//
//    cont.insert(1);
//    print(cont);
//
//    cont.resize(0,2);
//    cont[0][0]='a'; cont[0][1]='b';
//    print(cont);
//
//    cont.resize(1,1);
//    cont[1][0]='c';
//    print(cont);
//
//    cont.insert(2);
//    cont.resize(2,1);
//    cont[2][0]='e';
//    print(cont);
//
//    cont.resize(1,2);
//    cont[1][1]='f';
//    print(cont);
//
//    cont.resize(1,0);
//    print(cont);
//
//
//    cont.del(1);
//    print(cont);
//
//    cont.resize(0,3);
//    cont[0][2]='d';
//    print(cont);
//
//    cont.del(0);
//    print(cont);




	return 0;
}
