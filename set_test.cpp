#include <iostream>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>

//#define KOALA_SET_ON_VECTOR

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
    int tab[]={3,0,3,4,7,7};
    int tab2[5];



    PriQueueInterface<int*,std::less<int> > q(tab,4);

    cout << q.size() << endl;
    q.push(4);
    q.push(4);
    q.push(2);
    q.push(5);
    q.pop();q.pop();q.pop();q.pop();
    q.push(5);
    cout << q.top() << endl;
    cout << q.size() << endl;
    cout << endl << endl;





    std::map<int,float> mapa;
    Koala::AssocTabInterface<std::map<int,float> > imap=mapa;


    assocInserter(mapa,EmptyStruct<int>())=4.1;
    assocInserter(imap)=make_pair(4.0,5);
    imap[5]=7;
    imap.clear();
    cout << " ala "<< assocTabInterf(mapa) << "ala " << endl;
    cout << mapa.size();

    cout << "\n*******\n";
    JoinableSets<int, AssocTable<std::map<int, JSPartDesrc<int>*> > > jset0;
    jset0.resize(5);
    JoinableSets<int, AssocTable<std::map<int, JSPartDesrc<int>*> > > jset=jset0;


    Set<JSPartDesrc<int>*> sdesc;
    Set<int> sint;

    cout << jset << endl;
    sdesc.clear(); sint.clear();
    cout << jset.getElements(setInserter(sint));cout << sint << endl;
    cout << jset.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    cout << jset << endl;
    sint.clear();cout << jset.getSet(3,setInserter(sint));cout << sint << endl;

    cout << "size: " << jset.size() << " parts: " << jset.getSetNo() << endl;
    cout << jset.makeSinglet(1) << endl;
    cout << jset.makeSinglet(2) << endl;
    cout << jset.makeSinglet(3) << endl;
    sdesc.clear(); sint.clear();
    cout << jset.getElements(setInserter(sint));cout << sint << endl;
    cout << jset.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    sint.clear();cout << jset.getSet(3,setInserter(sint));cout << sint << endl;
    cout << jset << endl;

    JoinableSets<int, AssocTable<std::map<int, JSPartDesrc<int>*> > > jset2=jset;
    cout << "jset2\n";
    cout << "size: " << jset2.size() << " parts: " << jset2.getSetNo() << endl;
    cout << jset2.makeSinglet(1) << endl;
    cout << jset2.makeSinglet(2) << endl;
    cout << jset2.makeSinglet(3) << endl;
    sdesc.clear(); sint.clear();
    cout << jset2.getElements(setInserter(sint));cout << sint << endl;
    cout << jset2.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    sint.clear();cout << jset2.getSet(3,setInserter(sint));cout << sint << endl;
    cout << jset2 << endl;

    cout << "size: " << jset2.size() << " parts: " << jset2.getSetNo() << endl;
    cout << "join " << jset2.join(1,3) << endl;
    sdesc.clear(); sint.clear();

    cout << jset2.getElements(setInserter(sint));cout << sint << endl;
    cout << "!" << sdesc << endl;
    cout << jset2.getSetIds(setInserter(sdesc));cout << jset2.getSetNo() <<" " << sdesc.size() << sdesc << endl;
    sint.clear();cout << jset2.getSet(3,setInserter(sint));cout << sint << endl;
    cout << jset2 << endl;
    cout << jset2.getSetId(1) << endl;
    cout << jset2.getSetId(2) << endl;
    cout << jset2.getSetId(3) << endl;

    jset=jset2;

    cout << "join " << jset.join(1,2) << endl;
    sdesc.clear(); sint.clear();
    cout << jset.getElements(setInserter(sint));cout << sint << endl;
    cout << jset.getSetIds(setInserter(sdesc));cout << sdesc << endl;
    sint.clear();cout << jset.getSet(3,setInserter(sint));cout << sint << endl;
    cout << jset << endl;
    cout << "***\n";

    cout << "size: " << jset.size() << " parts: " << jset.getSetNo() <<  " " << jset << endl;
    cout << jset.getSetId(1) << endl;
    cout << jset.getSetId(2) << endl;
    cout << jset.getSetId(3) << endl;
    cout << jset.getSetId(4) << endl;
    jset.makeSinglet(4);
    cout << jset.getSet(5,setInserter(sint));

    cout << cout << endl;

    int tab3[]={1,3,5,7}, tab4[4];
    Set<int> iiset;

    copy(tab3,tab3+4,blackHole());
//    iiset.assign(tab4,4);
    cout << iiset;


}
