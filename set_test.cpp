#include <iostream>
#include <cstdlib>
#include <ctime>


#include "Koala/container/set.h"

using namespace std;

int fun(int a) { return !(a&1);}

class Funktor {
    public:
    bool operator()(int a) { return fun(a); }
};

int main()
{
    int tab[]={3,0,3,4};
    int tab2[5];

    std::set<int> stdset;

    stdset.insert(5);


   Koala::Set<int> s(tab,4),s2;
   s2.add(0);s2.add(7);
   s^=stdset;
   cout << s.empty();


}
