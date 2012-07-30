#include <iostream>
#include "Koala/algorithm/search.h"

using namespace Koala;
using namespace std;

void print(const SearchStructs::VectCompStore<char>& cont)
{
    cout << "\nsize: " << cont.size() << "\tlen:" <<cont.lenght();;
    for(int i=0;i<cont.size();i++)
    {
        cout << "\n"<<cont.size(i) << ":";
        for(int j=0;j<cont.size(i);j++) cout << ' ' << cont[i][j];
    }
}

int main() {

    SearchStructs::VectCompStore<char> cont;
    print(cont);

    cont.insert(0);
    print(cont);

    cont.insert(1);
    print(cont);

    cont.resize(0,2);
    cont[0][0]='a'; cont[0][1]='b';
    print(cont);

    cont.resize(1,1);
    cont[1][0]='c';
    print(cont);

    cont.insert(2);
    cont.resize(2,1);
    cont[2][0]='e';
    print(cont);

    cont.resize(1,2);
    cont[1][1]='f';
    print(cont);

    cont.resize(1,0);
    print(cont);


    cont.del(1);
    print(cont);

    cont.resize(0,3);
    cont[0][2]='d';
    print(cont);

    cont.del(0);
    print(cont);




	return 0;
}
