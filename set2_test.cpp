#include <iostream>
#include "Koala/algorithm/search.h"

using namespace Koala;
using namespace std;

        template <class T> // typ elementu ciagow skladowych
        class CompStoreTool {
            public:

                std::vector<int> idx;
                std::vector<T> data;
            public:

                typedef T ValueType;
                typedef std::back_insert_iterator<std::vector<int> > CIterType;
                typedef std::back_insert_iterator<std::vector<T> > VIterType;

                CompStoreTool() { clear(); }
                void clear() { idx.clear(); data.clear(); idx.push_back(0); }
                int size() const // liczba zebranych ciagow
                {
                    if (idx.empty()) return 0;
                    return idx.size()-1;
                }
                int size(int i) const // dlugosc i-tego ciagu
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    return idx[i+1]-idx[i];
                }
                int lenght() const // laczna dlugosc wszystkich ciagow
                {   int res=0;
                    for(int i=0;i<size();i++) res+=size(i);
                    return res;
                }
                T* operator[](int i) // wskaznik poczatku i-tego ciagu
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    return &data[idx[i]];
                }
                const T* operator[](int i) const // wskaznik poczatku i-tego ciagu
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    return &data[idx[i]];
                }

                T* insert(int i) // umieszcza nowy ciag pusty na pozycji i. Zwraca wskaznik jego poczatku
                {   assert(i>=0 && i<=size()); //TODO: throw
                    idx.insert(idx.begin()+i,idx[i]);
                    return operator[](i);
                }

                void del(int i) // kasuje ciag na pozycji i
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    int delta=size(i);
                    data.erase(data.begin()+idx[i],data.begin()+idx[i+1]);
                    for(int j=i+1;j<idx.size();j++) idx[j]-=delta;
                    idx.erase(idx.begin()+i);
                }

                T* resize(int i, int asize) // zmienia dlugosc i-tego ciagu. Zwraca wskaznik jego poczatku
                {   assert(i>=0 && i<=this->size()-1 && asize>=0); //TODO: throw
                    if (asize==size(i)) return this->operator[](i);
                    if (asize>size(i))
                    {   int delta=asize-size(i);
                        data.insert(data.begin()+idx[i+1],delta,T());
                        for(int j=i+1;j<idx.size();j++) idx[j]+=delta;
                    } else
                    {   int delta=size(i)-asize;
                        data.erase(data.begin()+(idx[i+1]-delta),data.begin()+idx[i+1]);
                        for(int j=i+1;j<idx.size();j++) idx[j]-=delta;
                    }
                    return this->operator[](i);
                }

                SearchStructs::CompStore<std::back_insert_iterator<std::vector<int> >,std::back_insert_iterator<std::vector<T> > >
                    input() { idx.clear(); data.clear();
                        return SearchStructs::compStore(std::back_inserter(idx),std::back_inserter(data));
                    }

        };


template<class T> void print(const CompStoreTool<T>& cont)
{
    cout << "\nsize: " << cont.size() << "\tlen:" <<cont.lenght();;
    for(int i=0;i<cont.size();i++)
    {
        cout << "\n"<<cont.size(i) << ":";
        for(int j=0;j<cont.size(i);j++) cout << ' ' << cont[i][j];
    }
}

#include "main.hpp"

    CompStoreTool<char> cont;
    CompStoreTool<int> rev;
    char tab[]={'.','.','.','.','.','.','.','.','.','.','.','.','.','.'};

    cont.insert(0);cont.insert(1);cont.insert(2);
    cont.resize(0,2);
//    cont.resize(0,1);
//    cont[0][0]='d';
    cont.resize(2,4);
    cont[0][0]='a'; cont[0][1]='b';
    cont[1][0]='c';
    cont[2][0]='a';cont[2][1]='b';cont[2][2]='c';cont[2][3]='a';
    cont.insert(0);cont.insert(2);cont.insert(4);
    cont.resize(0,1);cont[0][0]='d';

    print(cont);
    cout << endl;
    cout << endl << SearchStructs::revCompStore(cont.idx.begin(),&cont.data[0],6,rev.input(),tab);
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
