#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>


#include "Koala/container/set.h"
#include "Koala/container/assoctab.h"
#include "Koala/container/joinsets.h"
#include "Koala/base/def_struct.h"

using namespace std;
using namespace Koala;

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


   Koala::Set<int> s(tab,4);
   Koala::SetInserter<Koala::Set<int> > iter=s;
   Koala::SetInserter<Koala::Set<int> > iter2=iter;
   iter=12; iter=-4;
   iter++; ++iter; *iter;
   iter2=110;
   setInserter(s)=200;
   iter2=setInserter(s);
   iter2=300;

   cout << s;


    std::map<int,float> mapa;
    Koala::AssocTabInterface<std::map<int,float> > imap=mapa;


    assocInserter(mapa,EmptyStruct<int>())=4.1;
    assocInserter(imap)=make_pair(4.0,5);
    imap[5]=7;
    imap.clear();
    cout << " ala "<< assocTabInterf(mapa) << "ala " << endl;
    cout << mapa.size();

    cout << "\n*******\n";
    JoinableSets<int, AssocTable<std::map<int, JSPartDesrc*> > > jset;
    jset.resize(5);jset.resize(5);


    Set<JSPartDesrc*> sdesc;
    Set<int> sint;

    cout << jset << endl;
    sdesc.clear(); sint.clear();
    cout << jset.getElements(setInserter(sint));cout << sint << endl;
    cout << jset.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    cout << jset << endl;
//    sint.clear();cout << jset.getSet(3,setInserter(sint));cout << sint << endl;
    cout << "size: " << jset.size() << " parts: " << jset.getSetNo() << endl;
    cout << jset.makeSinglet(1) << endl;
    cout << jset.makeSinglet(2) << endl;
    cout << jset.makeSinglet(3) << endl;
    sdesc.clear(); sint.clear();
    cout << jset.getElements(setInserter(sint));cout << sint << endl;
    cout << jset.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    sint.clear();cout << jset.getSet(3,setInserter(sint));cout << sint << endl;
    cout << jset << endl;

    cout << "size: " << jset.size() << " parts: " << jset.getSetNo() << endl;
    cout << "join " << jset.join(1,3) << endl;
    sdesc.clear(); sint.clear();
    cout << jset.getElements(setInserter(sint));cout << sint << endl;
    cout << jset.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    sint.clear();cout << jset.getSet(3,setInserter(sint));cout << sint << endl;
    cout << jset << endl;
    cout << jset.getSetId(1) << endl;
    cout << jset.getSetId(2) << endl;
    cout << jset.getSetId(3) << endl;
    cout << "join " << jset.join(1,2) << endl;
    sdesc.clear(); sint.clear();
    cout << jset.getElements(setInserter(sint));cout << sint << endl;
    cout << jset.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    sint.clear();cout << jset.getSet(3,setInserter(sint));cout << sint << endl;
    cout << jset << endl;

    cout << "size: " << jset.size() << " parts: " << jset.getSetNo() << endl;
    cout << jset.getSetId(1) << endl;
    cout << jset.getSetId(2) << endl;
    cout << jset.getSetId(3) << endl;
    cout << jset.getSetId(4) << endl;


}
