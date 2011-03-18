#ifndef KOALA_ASSOCTAB
#define KOALA_ASSOCTAB

#include <map>
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "localarray.h"

//TODO: ustalic hierarchie koalowych wyjatkow i zastapic asserty

namespace Koala {

// bez implementacji ogolnej, tylko czesciowe specjalizacje dla kontenerow, ktorym ma dostarczac wspolnego interfejsu
template <class Container> class AssocTabInterface;


template <class K, class V> class AssocTabInterface<std::map<K,V> > {
    std::map<K,V>& cont;
    public:
    AssocTabInterface(std::map<K,V>& acont) : cont(acont) {}

    typedef K KeyType;
    typedef V ValType;

    bool hasKey(K arg) { return cont.find(arg)!=cont.end(); }
    bool delKey(K arg) // false gdy klucza nie bylo
    {   typename std::map<K,V>::iterator pos=cont.find(arg);
        if (pos==cont.end()) return false;
        cont.erase(pos); return true;
    }
    K firstKey() // null dla pustego kontenera
    {   if (cont.begin()==cont.end()) return (K)0;
        return cont.begin()->first;
    }
    K lastKey() // j.w.
    {   typename std::map<K,V>::iterator pos;
        if (cont.begin()==(pos=cont.end())) return (K)0;
        pos--; return pos->first;
    }
    K prevKey(K arg) // blad gdy klucza nie bylo, null gdy nie ma wczesniejszego
    {   typename std::map<K,V>::iterator pos=cont.find(arg);
        assert(pos!=cont.end());
        if (pos==cont.begin()) return (K)0;
        pos--; return pos->first;
    }
    K nextKey(K arg) // podobnie j.w.
    {   typename std::map<K,V>::iterator pos=cont.find(arg);
        assert(pos!=cont.end());
        pos++; if (pos==cont.end()) return (K)0;
        return pos->first;
    }
    // tu akurat sa dziedziczone, ale dla kompletnosci dopisuje:
    V& operator[] (K arg) { return cont[arg]; };
    unsigned size() { return cont.size(); }
    bool empty() { return this->size()==0; }
    void clear() { cont.clear(); }
    template <class Iterator>
        int getKeys(Iterator iter)
        {   for(K key=firstKey();key;key=nextKey(key))
            {   *iter=key;
                iter++;
            }
            return size();
        }
};


template <class T> class AssocTabInterface<AssocTabInterface<T> > {
    AssocTabInterface<T>& cont;
    public:
    AssocTabInterface(AssocTabInterface<T>& acont) : cont(acont) {}

    typedef typename AssocTabInterface<T>::KeyType KeyType;
    typedef typename AssocTabInterface<T>::ValType ValType;

    bool hasKey(KeyType arg) { return cont.hasKey(arg); }
    bool delKey(KeyType arg) {   return cont.delKey(arg); }
    KeyType firstKey() { return cont.firstKey(); }
    KeyType lastKey()  { return cont.lastKey(); }
    KeyType prevKey(KeyType  arg) {return cont.prevKey(arg); }
    KeyType nextKey(KeyType  arg) {return cont.nextKey(arg); }
    ValType& operator[] (KeyType  arg) { return cont[arg]; };
    unsigned size() { return cont.size(); }
    bool empty() { return cont.empty(); }
    void clear() { cont.clear(); }

    template <class Iterator> int getKeys(Iterator iter) { return cont.getKeys(iter); }
};


template <class T>
AssocTabInterface<T> assocTabInterf(T& cont) { return AssocTabInterface<T>(cont); }


template <class T> class AssocTable {
    AssocTabInterface<T> inter;
    public:
    T cont;

    AssocTable() : cont(), inter(cont) {}
    AssocTable(const T& acont) : cont(acont), inter(cont) {}
    AssocTable(const AssocTable<T>& X)
        : cont(X.cont), inter(cont) {}
    AssocTable<T>& operator=(const AssocTable<T> &X )
    {   if (this==&X) return *this;
        cont=X.cont;
        return *this;
    }
    AssocTable<T>& operator=(const T& X)
    {   if (&cont==&X) return *this;
        cont=X;
        return *this;
    }

    typedef typename AssocTabInterface<T>::KeyType KeyType;
    typedef typename AssocTabInterface<T>::ValType ValType;

    bool hasKey(KeyType arg) { return inter.hasKey(arg); }
    bool delKey(KeyType arg) {   return inter.delKey(arg); }
    KeyType firstKey() { return inter.firstKey(); }
    KeyType lastKey()  { return inter.lastKey(); }
    KeyType prevKey(KeyType  arg) {return inter.prevKey(arg); }
    KeyType nextKey(KeyType  arg) {return inter.nextKey(arg); }
    ValType& operator[] (KeyType  arg) { return inter[arg]; };
    unsigned size() { return inter.size(); }
    bool empty() { return inter.empty(); }
    void clear() { inter.clear(); }

    template <class Iterator> int getKeys(Iterator iter) { return inter.getKeys(iter); }

};

template <class T>
AssocTable<T> assocTab(const T& cont) { return AssocTable<T>(cont); }

template <class T> class AssocTabInterface<AssocTable<T> > {
    AssocTable<T>& cont;
    public:
    AssocTabInterface(AssocTable<T>& acont) : cont(acont) {}

    typedef typename AssocTable<T>::KeyType KeyType;
    typedef typename AssocTable<T>::ValType ValType;

    bool hasKey(KeyType arg) { return cont.hasKey(arg); }
    bool delKey(KeyType arg) {   return cont.delKey(arg); }
    KeyType firstKey() { return cont.firstKey(); }
    KeyType lastKey()  { return cont.lastKey(); }
    KeyType prevKey(KeyType  arg) {return cont.prevKey(arg); }
    KeyType nextKey(KeyType  arg) {return cont.nextKey(arg); }
    ValType& operator[] (KeyType  arg) { return cont[arg]; };
    unsigned size() { return cont.size(); }
    bool empty() { return cont.empty(); }
    void clear() { cont.clear(); }

    template <class Iterator> int getKeys(Iterator iter) { return cont.getKeys(iter); }
};

//TODO: to tylko prototypy na szybko, wiec prosze o uwagi i uwazny audyt kodu.
//Cel: 1- i 2-wymiarowe tablice asocjacyjne, ktorych kluczami sa wierzcholki lub krawedzie. Ma dzialac
//w czasie praktycznie O(1), do uzytku w naszych algorytmach, zastanawiam sie czy udostepniac rowniez
//uzytkownikom. Zakladam, ze glebokosc stosu wywolan naszych procedur nie jest duza, wiec dany
//klucz bedzie wystepowal w co najwyzej kilku kontenerach na raz.

// pomocnicza, do uzytku wewnetrznego
template <class Element> struct BlockOfBlockList {
    public:
    Element blob;
    int prev,next;
};

// do uzytku wewnetrznego, pomocnicza wobec dalszych klas. Zwykla lista wolnych blokow, w ktorych
//mozna umieszczac wartosci typu Element. Bloki sa przydzielane i zwalniane, dostep jak do tablicy
//operatorem [](int) (przydzielony blok ma swoj numer). Mozna przegladac iterujac po kolejnych
//zaalokowanych blokach. Poniewaz 0 jest legalnym numerem, role NULLa pelni -1

template <class Element, class Container = std::vector<BlockOfBlockList<Element> > >
class BlockList {
    protected:
    //public:
        int siz, first, last, ffree;
        // struktura moze sama doalokowywac sobie pamiec w razie potrzeby, lub korzystac z zewnetrznego
        // kontenera. Wyobrazam sobie, ze w czesto wywolywanej procedurze, aby nie wolac zbyt czesto
        // new, za wczasu i na wlasna odpowiedzialnosc tworzymy bufor wystarczajacych rozmiarow
        // np. tablice lokalna i kolejnym obiektom kazemy dzialac na niej. Oczywiscie to tylko opcja.
        Container cont;
    public:

    typedef Element ElementType;
    typedef BlockOfBlockList<Element> BlockType;

    BlockList(int asize=0) :  siz(0), first(-1), last(-1), ffree(-1)
    {   cont.clear(); cont.reserve(asize); }
    BlockList(const BlockList<Element,Container>& X) :  siz(X.siz), first(X.first),
                                last(X.last), ffree(X.ffree), cont(X.cont) {}
    BlockList<Element,Container>& operator=(const BlockList<Element,Container>& X)
    {   if (&X==this) return *this;
        siz=X.siz; first=X.first; last=X.last; ffree=X.ffree; cont=X.cont;
        return *this;
    }

    bool ready(int pos) { return pos>=0 && pos<cont.size() && cont[pos].prev!=-2; }
    void clear() { cont.clear(); siz=0; ffree=first=last=-1; }
    int size() { return siz; }
    bool empty() { return !siz; }
    void reserve(int arg) { cont.reserve(arg); }
    bool full() { return ffree==-1; } // tzn. nastepna alokacja wykorzysta nowy blok
    int capacity() { return cont.capacity(); }
    int firstPos()  { return first; }
    int lastPos()   { return last; }
    int nextPos(int i)
    {   if (i==-1) return first;
    //   assert(ready(i)); // TODO: wywalic
        return cont[i].next;
    }
    int prevPos(int i)
    {   if (i==-1) return last;
    //    assert(ready(i)); // TODO: wywalic
        return cont[i].prev;
    }
    Element& operator[] (int i)
    {
    // assert(ready(i)); // TODO: wywalic
        return cont[i].blob;
    }
    int newPos(int x=-1) // alokuje nowy blok wstawiajac na liscie przed zajetym blokiem o numerze x.
    // Domyslnie na koniec.
    {
    // assert(x==-1 || ready(x)); // TODO: wywalic
        int pos;
        if (ffree!=-1) { pos=ffree; ffree=cont[ffree].next; }
            else { cont.push_back(BlockOfBlockList<Element>()); pos=siz; }
        siz++;
        cont[pos].next=x; cont[pos].prev=(x==-1) ? last : cont[x].prev;
        if (x==-1) last=pos; else cont[x].prev=pos;
        if (x==first) first=pos; else cont[cont[pos].prev].next=pos;
        return pos;
    }
    void delPos(int pos) // zwalnia blok,
    {   if(pos==-1) return;
    // assert(ready(pos)); // TODO: wywalic
        if (cont[pos].next!=-1) cont[cont[pos].next].prev=cont[pos].prev;
            else last=cont[pos].prev;
        if (cont[pos].prev!=-1) cont[cont[pos].prev].next=cont[pos].next;
            else first=cont[pos].next;
        cont[pos].next=ffree; ffree=pos; siz--;
        cont[pos].blob=Element(); cont[pos].prev=-2;
    }
    void defrag()
    {   int asiz=0;
        Element LOCALARRAY(buf,siz);
        for(int i=firstPos();i!=-1;i=nextPos(i)) buf[asiz++]=operator[](i);
        clear();
        for(int i=0;i<asiz;i++) operator[](newPos())=buf[i];
    }
    bool test();
};


struct AssocContReg;

// po tym musi dziedziczyc klasa kontenera asocjacyjnego
class AssocContBase {
    public:
    virtual void DelPosCommand(int)=0; // komunikat o zgonie klucza przechowywanego pod dana pozycja
    virtual AssocContReg& getReg(int)=0; // adres elementu listy kontenerow przechowujacych klucz z danej pozycji
};


// element listy kontenerow zawierajacych pewien klucz
struct AssocContReg {
    int nextPos; // pozycja, na ktorej u siebie ten klucz przechowuje kontener next
    AssocContBase* next;
};

// pole publiczne assocReg tego typu musi zawierac kazdy obiekt, na ktory wskazniki moga byc kluczami
class AssocKeyContReg : public AssocContReg {
    public:

    AssocKeyContReg() { next=0; }
    AssocKeyContReg(const AssocKeyContReg&) { next=0; };
    AssocKeyContReg& operator=(const AssocKeyContReg& X) { if (&X!=this) next=0; return *this; }

    // znajdz na liscie rekord opisujacy polozenie tego klucza w kontenerze kont
    AssocContReg* find(AssocContBase* cont)
    {   AssocContReg* res;
        for(res=this;res->next;res=&(res->next->getReg(res->nextPos)))
            if (res->next==cont) return res;
        return 0;
    }

    // obiekt klucza znika, wiec wyrejestrowuje sie ze wszystkich kontenerow, w ktorych byl wpisany
    ~AssocKeyContReg()
    {   std::pair<AssocContBase*,int> a=std::pair<AssocContBase*,int>(next,nextPos),n;
        next=0;
        while(a.first)
        {   AssocContReg *p=&a.first->getReg(a.second);
            n=std::pair<AssocContBase*,int>(p->next,p->nextPos);
            a.first->DelPosCommand(a.second);
            a=n;
        }
    }
};


template <class Klucz, class Elem> struct BlockOfAssocArray {
        Elem val;
        Klucz key;
        AssocContReg assocReg; // nastepny kontener zawierajacy key i pozycja, na ktorej on tam jest
};


// Klucz tablicy asocjacyjnej musi wskazywac obiekt zawierajacy pole AssocKeyContReg assocReg -
// poczatek listy kontenerow zawierajacych ten klucz
template <class Klucz> class KluczTest {
    public:

    KluczTest(Klucz v=0) { AssocKeyContReg* ptr= &v->assocReg; }
};


// wlasciwa tablica asocjacyjna, odpowiednik std::map
template <class Klucz, class Elem, class Container = std::vector<BlockOfBlockList<BlockOfAssocArray<Klucz,Elem> > > >
class AssocArray : public AssocContBase, public KluczTest<Klucz> {
    protected:
    //public:

    BlockList<BlockOfAssocArray<Klucz,Elem>, std::vector<BlockOfBlockList<BlockOfAssocArray<Klucz,Elem> > > > tab;

    // obsluga komunikatu o zniknieciu obiektu posiadanego klucza
    virtual void DelPosCommand(int pos) { tab.delPos(pos); }
    virtual AssocContReg& getReg(int pos)   { return tab[pos].assocReg; }

    public:

    typedef Klucz KeyType;
    typedef Elem ValType;
    typedef BlockOfBlockList<BlockOfAssocArray<Klucz,Elem> > BlockType;


    AssocArray(int asize=0) : tab(asize) {}

    AssocArray(const AssocArray<Klucz,Elem,Container>& X) : tab(X.tab)
    {   for(int i=tab.firstPos();i!=-1;i=tab.nextPos(i))
        {   tab[i].assocReg=tab[i].key->assocReg;
            tab[i].key->assocReg.next=this; tab[i].key->assocReg.nextPos=i;
        }
    }

    AssocArray<Klucz,Elem,Container>& operator= (const AssocArray<Klucz,Elem,Container>& X)
    {   if (&X==this) return *this;
        clear(); tab=X.tab;
        for(int i=tab.firstPos();i!=-1;i=tab.nextPos(i))
        {   tab[i].assocReg=tab[i].key->assocReg;
            tab[i].key->assocReg.next=this; tab[i].key->assocReg.nextPos=i;
        }
        return *this;
    }

    // interfejs tablicy asocjacyjnej
    int size() { return tab.size(); }
    bool empty() { return tab.empty(); }
    void reserve(int arg) { tab.reserve(arg); }
    int capacity() { return tab.capacity(); }
    bool hasKey(Klucz v) { return keyPos(v)!=-1; }
    // numer bloku danego klucza
    int keyPos(Klucz v)
    { if (!v) return -1;
      AssocContReg* preg=v->assocReg.find(this);
      if (preg) return preg->nextPos; else return -1;
    }

    bool delKey(Klucz v)
    {   int x; if (!v) return false;
        AssocContReg* preg=v->assocReg.find(this);
        if (!preg) return false;
        x=preg->nextPos; *preg=tab[x].assocReg;
        tab.delPos(x); return true;
    }

    Klucz firstKey()
    {  if (tab.empty()) return 0; else return tab[tab.firstPos()].key; }

    Klucz lastKey()
    { if (tab.empty()) return 0; else return tab[tab.lastPos()].key; }

    Klucz nextKey(Klucz v)
    {   if (!v) return firstKey();
        int x; assert((x=keyPos(v))!=-1);
        if ((x=tab.nextPos(x))==-1) return 0;
        return tab[x].key;
    }

    Klucz prevKey(Klucz v)
    {   if (!v) return lastKey();
        int x; assert((x=keyPos(v))!=-1);
        if ((x=tab.prevPos(x))==-1) return 0;
        return tab[x].key;
    }

    Elem& operator[] (Klucz v)
    {   assert(v);
        int x=keyPos(v);
        if (x==-1)
        {   x=tab.newPos(); tab[x].key=v;
            tab[x].assocReg=v->assocReg; v->assocReg.next=this; v->assocReg.nextPos=x;
        }
     //   assert(tab[x].key==v); // TODO: wywalic
        return tab[x].val;
    }

    void defrag()
    {   tab.defrag();
        for(int i=0;i<tab.size();i++) tab[i].key->assocReg.find(this)->nextPos=i;
    }

    void clear() { for(Klucz v=firstKey();v;v=firstKey()) delKey(v); }

    template <class Iterator>
        int getKeys(Iterator iter)
        {   for(Klucz key=firstKey();key;key=nextKey(key))
            {   *iter=key;
                iter++;
            }
            return size();
        }

    ~AssocArray() { clear(); }

} ;


template <class K, class V, class C> class AssocTabInterface<AssocArray<K,V,C> > {
    AssocArray<K,V,C>& cont;
    public:
    AssocTabInterface(AssocArray<K,V,C>& acont) : cont(acont) {}

    typedef K KeyType;
    typedef V ValType;

    bool hasKey(KeyType arg) { return cont.hasKey(arg); }
    bool delKey(KeyType arg) {   return cont.delKey(arg); }
    KeyType firstKey() { return cont.firstKey(); }
    KeyType lastKey()  { return cont.lastKey(); }
    KeyType prevKey(KeyType  arg) {return cont.prevKey(arg); }
    KeyType nextKey(KeyType  arg) {return cont.nextKey(arg); }
    ValType& operator[] (KeyType  arg) { return cont[arg]; };
    unsigned size() { return cont.size(); }
    bool empty() { return cont.empty(); }
    void clear() { cont.clear(); }

    template <class Iterator> int getKeys(Iterator iter) { return cont.getKeys(iter); }

};


// Taka tablica moze od biedy rowniez pelnic funkcje zbioru (kluczy) z dostepem do elementu w czasie O(1) i liniowych
// czasach operacji mnogosciowych. Przynajmniej dopoki takich zbiorow jest malo.


struct DummyVal {};

template <class Klucz, class Container>
void sum(AssocArray<Klucz,DummyVal,Container>& a1, AssocArray<Klucz,DummyVal,Container>& a2, AssocArray<Klucz,DummyVal,Container>& res)
{   AssocArray<Klucz,DummyVal,Container> ares=a1;
    for(Klucz k=a2.firstKey();k;k=a2.nextKey(k)) ares[k];
    res=ares;
}

template <class Klucz, class Container>
void diff(AssocArray<Klucz,DummyVal,Container>& a1, AssocArray<Klucz,DummyVal,Container>& a2, AssocArray<Klucz,DummyVal,Container>& res)
{   AssocArray<Klucz,DummyVal,Container> ares=a1;
    for(Klucz k=a2.firstKey();k;k=a2.nextKey(k)) ares.delKey(k);
    res=ares;
}


//Cel: 2-wymiarowa macierz asocjacyjna, inaczej: indeksem jest para kluczy. Mozliwe 4 rodzaje indeksowania:
//- dowolna para uporzadkowana kluczy
//- j.w. ale para tych samych kluczy jest zabroniona (macierz bez przekatnej)
//- para nieuporzadkowana kluczy, a wiec indeks (u,v) jest automatycznie utozsamiany z (v,u). Te same dwa
//klucze (u,u) zabronione (w sam raz do wykorzystania w macierzy sasiedztwa w klasie grafu)
//- j.w. ale dopuszczmy rowne klucze (u,u)
enum AssocMatrixType { AMatrFull, AMatrNoDiag, AMatrTriangle, AMatrClTriangle };

// Tak na prawde macierz bedzie przechowywac wartosci w jednowymiarowym buforze o rozmiarze dostosowanym
// do liczby wystepujacych kluczy. Ponizsze klasy pomocnicze obsluguja adresowanie
template <AssocMatrixType> class AssocMatrixAddr;


template <> class AssocMatrixAddr<AMatrFull> {
    public:

    // dlugosc bufora przy n roznych kluczach
    static int bufLen(int n)  { return n*n; }
    // gdzie w buforze lezy pozycja o podanych wspolrzednych
    int wsp2pos(std::pair<int,int> w)
        { int mfs=std::max(w.first,w.second); return mfs*mfs+mfs+w.second-w.first; }
    // ... i odwrotna konwersja
    std::pair<int,int> pos2wsp(int pos)
        {   int x=(int)sqrt((double)pos);
            if (x*x+x-pos>0) return std::pair<int,int>(x,pos-x*x);
            else return std::pair<int,int>(x*x+2*x-pos,x);
        }
    // czy dana para kluczy jest dopuszczalna w tym typie macierzy
    template <class T>
        bool correctPos(T,T) { return true; }
    // dla macierzy utozsamiajacych pewne klucze tj. indeksowanych parami nieuporzadkowanymi wazne jest,
    // by obiekt przynajmniej zwracal je zawsze w tym samym formacie. My mozemy indeksowac i przez (u,v) i (v,u)
    template <class Klucz>
        std::pair<Klucz,Klucz> key(Klucz u, Klucz v) { return std::pair<Klucz,Klucz>(u,v); }
    template <class Klucz>
    // wszedzie, gdzie mozna uzyc dwoch kluczy, mozna tez posluzyc sie std::para
        std::pair<Klucz,Klucz> key(std::pair<Klucz, Klucz> k) { return key(k.first,k.second); }
};


template <> class AssocMatrixAddr<AMatrNoDiag> {
    public:

    static int bufLen(int n)  { return n*(n-1); }
    int wsp2pos(std::pair<int,int> w)
        { int mfs=std::max(w.first,w.second); return mfs*mfs+w.second-w.first-((w.first>w.second) ? 0 : 1); }
    std::pair<int,int> pos2wsp(int pos)
        {   int x=(int)sqrt((double)pos);
            if (pos-x*x-x>=0) return std::pair<int,int>(x+1,pos-x*x-x);
            else return std::pair<int,int>(x*x+x-1-pos,x);
        }
    template <class T>
        bool correctPos(T u,T v) { return u!=v; }
    template <class Klucz>
        std::pair<Klucz,Klucz> key(Klucz u, Klucz v) { return std::pair<Klucz,Klucz>(u,v); }
    template <class Klucz>
        std::pair<Klucz,Klucz> key(std::pair<Klucz, Klucz> k) { return key(k.first,k.second); }
};


template <> class AssocMatrixAddr<AMatrClTriangle> {
    public:

    static int bufLen(int n)  { return n*(n+1)/2; }
    int wsp2pos(std::pair<int,int> w)
        { if (w.first<w.second) { int z=w.first; w.first=w.second; w.second=z; }
            return w.first*(w.first+1)/2+w.second;
        }
    std::pair<int,int> pos2wsp(int pos)
        {   int x=(int)sqrt((double)2*pos),xx=pos-x*(x+1)/2;
            if (xx>=0) return std::pair<int,int>(x,xx);
            else return std::pair<int,int>(x-1,xx+x);
        }
    template <class T>
        bool correctPos(T,T) { return true; }
    template <class Klucz>
        std::pair<Klucz,Klucz> key(Klucz u, Klucz v) { return std::pair<Klucz,Klucz>(std::min(u,v),std::max(u,v)); }
    template <class Klucz>
        std::pair<Klucz,Klucz> key(std::pair<Klucz,Klucz> k) { return key(k.first,k.second); }
};


template <> class AssocMatrixAddr<AMatrTriangle> {
    public:

    static int bufLen(int n)  { return n*(n-1)/2; }
    int wsp2pos(std::pair<int,int> w)
        { if (w.first<w.second) { int z=w.first; w.first=w.second; w.second=z; }
            return w.first*(w.first-1)/2+w.second; }
    std::pair<int,int> pos2wsp(int pos)
        {   int x=(int)sqrt((double)2*pos),xx=pos-x*(x+1)/2;
            if (xx>=0) return std::pair<int,int>(x+1,xx);
            else return std::pair<int,int>(x,xx+x);
        }
    template <class T>
        bool correctPos(T u,T v) { return u!=v; }
    template <class Klucz>
        std::pair<Klucz,Klucz> key(Klucz u, Klucz v) { return std::pair<Klucz,Klucz>(std::min(u,v),std::max(u,v)); }
    template <class Klucz>
        std::pair<Klucz,Klucz> key(std::pair<Klucz,Klucz> k) { return key(k.first,k.second); }
};

// rekord wewnetrznego bufora, zawiera wartosc przypisana parze kluczy. Zajete pozycje w buforze
// sa powiazane w liste, aby mozna bylo po nich iterowac.
template <class Elem> struct BlockOfAssocMatrix {
    Elem val;
    int next,prev;
    bool present() { return next || prev; }
    BlockOfAssocMatrix() : next(0), prev(0), val() {}
};


// Wlasciwa macierz asocjacyjna
template <class Klucz, class Elem, AssocMatrixType aType,
          class Container = std::vector<BlockOfAssocMatrix<Elem> >,
          class IndexContainer = std::vector<BlockOfBlockList<BlockOfAssocArray<Klucz,int> > > >
class AssocMatrix : public AssocMatrixAddr<aType> {

        // wewnetrzny indeks przechowujacy klucze (pojedyncze) uzyte gdzies w macierzy
        // Jego rekord zawiera tez licznik, w ilu uzytych parach indeksow wystepuje dany klucz
        class AssocIndex : public AssocArray<Klucz,int,IndexContainer> {
            friend  class AssocMatrix<Klucz,Elem,aType,Container,IndexContainer>;
            public:
                AssocMatrix<Klucz,Elem,aType,Container,IndexContainer>* owner;

                AssocIndex(int asize=0)
                    : AssocArray<Klucz,int,IndexContainer>(asize) { }

                // tlumaczy podany klucz na jego wsplrzedna w indeksie
                int klucz2pos(Klucz v)
                    { if (!v) return -1; return AssocArray<Klucz,int,IndexContainer>::keyPos(v); }
                // ... i konwersja odwrotna
                Klucz pos2klucz(int arg)
                    { if (arg==-1) return 0; return AssocArray<Klucz,int,IndexContainer>::tab[arg].key; }
                // obsluga komunikatu o zniknieciu obiektu uzywanego klucza
                virtual void DelPosCommand(int pos)
                {   int LOCALARRAY(tabpos,size());
                    int l=0;
                    for(int i=AssocArray<Klucz,int,IndexContainer>::tab.firstPos();
                            i!=-1;
                            i=AssocArray<Klucz,int,IndexContainer>::tab.nextPos(i)) tabpos[l++]=i;
                    for (l--;l>=0;l--)
                    {   owner->delPos(std::pair<int,int>(pos,tabpos[l]));
                        if ((aType==AMatrNoDiag || aType==AMatrFull)&&(pos!=tabpos[l]))
                            owner->delPos(std::pair<int,int>(tabpos[l],pos));
                    }
                    AssocArray<Klucz,int,IndexContainer>::tab.delPos(pos);
                    Klucz k,n;
                    for(k=AssocArray<Klucz,int,IndexContainer>::firstKey();k;k=n)
                    {   n=AssocArray<Klucz,int,IndexContainer>::nextKey(k);
                        if (!operator[](k)) AssocArray<Klucz,int,IndexContainer>::delKey(k);
                    }
                }

        } index;
	friend class AssocIndex;

        Container bufor;
        int siz,first,last;

                void delPos(std::pair<int,int> wsp)
                {   if (!AssocMatrixAddr<aType>::correctPos(wsp.first,wsp.second)) return;
                    int x;
                    if  (!bufor[x=AssocMatrixAddr<aType>::wsp2pos(wsp)].present()) return;
                    if (bufor[x].next!=-1) bufor[bufor[x].next].prev=bufor[x].prev;
                        else last=bufor[x].prev;
                    if (bufor[x].prev!=-1) bufor[bufor[x].prev].next=bufor[x].next;
                        else first=bufor[x].next;
                    bufor[x]=BlockOfAssocMatrix<Elem>();
                    siz--;
                    --index.tab[wsp.first].val;
                    --index.tab[wsp.second].val;
                }

    public:

    typedef Klucz KeyType;
    typedef Elem ValType;
    typedef BlockOfAssocMatrix<Elem> BlockType;
    typedef BlockOfBlockList<BlockOfAssocArray<Klucz,int> > IndexBlockType;

    AssocMatrix(int asize=0)
            : index(asize), siz(0), first(-1), last(-1)
    {   bufor.clear(); bufor.reserve(AssocMatrixAddr<aType>::bufLen(asize)); index.owner=this; }

    AssocMatrix(const AssocMatrix<Klucz,Elem,aType,Container,IndexContainer>& X) :
            index(X.index), bufor(X.bufor), siz(X.siz), first(X.first), last(X.last)
    {   index.owner=this;   }

    AssocMatrix<Klucz,Elem,aType,Container,IndexContainer>&
        operator=(const AssocMatrix<Klucz,Elem,aType,Container,IndexContainer>& X)
    {   if (&X==this) return *this;
        index=X.index; bufor=X.bufor; siz=X.siz; first=X.first; last=X.last;
        index.owner=this;
        return *this;
    }

    // obsluga pojedynczych kluczy wykorzystywanych w parach zawartych w tablicy. Po nich tez mozna iterowac
    bool hasInd(Klucz v) { return index.hasKey(v); }

    bool delInd(Klucz v)
    {   if (!hasInd(v)) return false;
        Klucz tab[index.size()];       // TODO: localarray
        int i=0;
        for(Klucz x=index.firstKey();x;x=index.nextKey(x)) tab[i++]=x;
        for (i--;i>=0;i--)
        {   delKey(v,tab[i]);
            if ((aType==AMatrNoDiag || aType==AMatrFull)&&(v!=tab[i])) delKey(tab[i],v);
        }
        index.delKey(v); return true;
    }

    Klucz firstInd() { return index.firstKey(); }
    Klucz lastInd()    { return index.lastKey(); }
    Klucz nextInd(Klucz v) {  return index.nextKey(v); }
    Klucz prevInd(Klucz v) {  return index.prevKey(v); }
    int indSize() { return index.size(); }

    // zrzucenie do zewnetrznej 1-wymiarowej tablicy asocjacyjnej przypisan w parach posiadajacych v
    // na jednej ze wspolrzednych
    template <class Elem2, class ExtCont>
    int slice1(Klucz v,AssocArray<Klucz,Elem2,ExtCont>& tab)
    {   if (!index.hasKey(v)) return 0;
        int licz=0;
        for(Klucz x=index.firstKey();x;x=index.nextKey(x))
            if (hasKey(v,x)) { tab[x]=operator()(v,x); licz++; }
        return licz;
    }

    template <class Elem2, class ExtCont>
    int slice2(Klucz v,AssocArray<Klucz,Elem2,ExtCont>& tab)
    {   if (!index.hasKey(v)) return 0;
        int licz=0;
        for(Klucz x=index.firstKey();x;x=index.nextKey(x))
            if (hasKey(x,v)) { tab[x]=operator()(x,v); licz++; }
        return licz;
    }

    // operacje wlasciwe na macierzy
    bool hasKey(Klucz u, Klucz v)
    {   if (!u || !v) return false;
        if (!AssocMatrixAddr<aType>::correctPos(u,v)) return false;
        std::pair<int,int> wsp=std::pair<int,int>(index.klucz2pos(u),index.klucz2pos(v));
        if (wsp.first==-1 || wsp.second==-1) return false;
        return bufor[AssocMatrixAddr<aType>::wsp2pos(wsp)].present();
    }
    bool hasKey(std::pair<Klucz,Klucz> k) { return hasKey(k.first,k.second); }

    bool delKey(Klucz u, Klucz v)
    {   if (!u || !v) return false;
        if (!AssocMatrixAddr<aType>::correctPos(u,v)) return false;
        std::pair<int,int> wsp=std::pair<int,int>(index.klucz2pos(u),index.klucz2pos(v));
        if (wsp.first==-1 || wsp.second==-1) return false;
        int x;
        if  (bufor[x=AssocMatrixAddr<aType>::wsp2pos(wsp)].present())
        {   if (bufor[x].next!=-1) bufor[bufor[x].next].prev=bufor[x].prev;
                else last=bufor[x].prev;
            if (bufor[x].prev!=-1) bufor[bufor[x].prev].next=bufor[x].next;
                else first=bufor[x].next;
            bufor[x]=BlockOfAssocMatrix<Elem>();
            siz--;
            if (--index[u]==0) index.delKey(u);
            if (--index[v]==0) index.delKey(v);
            return true;
        }
        return false;
    }
    bool delKey(std::pair<Klucz,Klucz> k) { return delKey(k.first,k.second); }

    Elem& operator() (Klucz u, Klucz v)
    {   assert(u && v && AssocMatrixAddr<aType>::correctPos(u,v));
        std::pair<int,int> wsp=std::pair<int,int>(index.klucz2pos(u),index.klucz2pos(v));
        if (wsp.first==-1) { index[u]=0;wsp.first=index.klucz2pos(u); }
        if (wsp.second==-1) { index[v]=0; wsp.second=index.klucz2pos(v); }
        bufor.resize(std::max((int)bufor.size(),AssocMatrixAddr<aType>::bufLen(index.size())));
        int x=AssocMatrixAddr<aType>::wsp2pos(wsp);
        if (!bufor[x].present())
        {   if ((bufor[x].prev=last)==-1) first=x;
                else bufor[bufor[x].prev].next=x;
            bufor[x].next=-1; last=x;
            index[u]++;index[v]++;siz++;
        }
        return bufor[x].val;
    }
    Elem& operator() (std::pair<Klucz,Klucz> k) { return operator()(k.first,k.second); }

    Elem* presentValPtr(Klucz u, Klucz v)
    {   if (!u || !v) return 0;
        if (!AssocMatrixAddr<aType>::correctPos(u,v)) return 0;
        std::pair<int,int> wsp=std::pair<int,int>(index.klucz2pos(u),index.klucz2pos(v));
        if (wsp.first==-1 || wsp.second==-1) return 0;
        int pos;
        if (!bufor[pos=AssocMatrixAddr<aType>::wsp2pos(wsp)].present()) return 0;
        return &bufor[pos].val;
    }

    std::pair<Klucz,Klucz> firstKey()
    {   if (!siz) return std::pair<Klucz,Klucz>((Klucz)0,(Klucz)0);
        std::pair<int,int> wsp=AssocMatrixAddr<aType>::pos2wsp(first);
        return AssocMatrixAddr<aType>::key(std::pair<Klucz,Klucz>(index.pos2klucz(wsp.first),index.pos2klucz(wsp.second)));
    }

    std::pair<Klucz,Klucz> lastKey()
    {   if (!siz) return std::pair<Klucz,Klucz>((Klucz)0,(Klucz)0);
        std::pair<int,int> wsp=AssocMatrixAddr<aType>::pos2wsp(last);
        return AssocMatrixAddr<aType>::key(std::pair<Klucz,Klucz>(index.pos2klucz(wsp.first),index.pos2klucz(wsp.second)));
    }

    std::pair<Klucz,Klucz> nextKey(Klucz u, Klucz v)
    {   if (!u || !v) return firstKey();
        assert(AssocMatrixAddr<aType>::correctPos(u,v));
        std::pair<int,int> wsp=std::pair<int,int>(index.klucz2pos(u),index.klucz2pos(v));
        assert(wsp.first!=-1 && wsp.second!=-1);
        int x=AssocMatrixAddr<aType>::wsp2pos(wsp);
        assert(bufor[x].present());
        x=bufor[x].next; if (x==-1) return std::pair<Klucz,Klucz>((Klucz)0,(Klucz)0);
        wsp=AssocMatrixAddr<aType>::pos2wsp(x);
        return AssocMatrixAddr<aType>::key(std::pair<Klucz,Klucz>(index.pos2klucz(wsp.first),index.pos2klucz(wsp.second)));
    }
    std::pair<Klucz,Klucz> nextKey(std::pair<Klucz,Klucz> k) { return nextKey(k.first,k.second); }

    std::pair<Klucz,Klucz> prevKey(Klucz u, Klucz v)
    {   if (!u || !v) return lastKey();
        assert(AssocMatrixAddr<aType>::correctPos(u,v));
        std::pair<int,int> wsp=std::pair<int,int>(index.klucz2pos(u),index.klucz2pos(v));
        assert(wsp.first!=-1 && wsp.second!=-1);
        int x=AssocMatrixAddr<aType>::wsp2pos(wsp);
        assert(bufor[x].present());
        x=bufor[x].prev; if (x==-1) return std::pair<Klucz,Klucz>((Klucz)0,(Klucz)0);
        wsp=AssocMatrixAddr<aType>::pos2wsp(x);
        return AssocMatrixAddr<aType>::key(std::pair<Klucz,Klucz>(index.pos2klucz(wsp.first),index.pos2klucz(wsp.second)));
    }
    std::pair<Klucz,Klucz> prevKey(std::pair<Klucz,Klucz> k) { return prevKey(k.first,k.second); }

    int size() { return siz; }
    bool empty() { return !siz; }
    void clear()
    { index.clear();
      int in; for(int i=first;i!=-1;i=in) { in=bufor[i].next; bufor[i]=BlockOfAssocMatrix<Elem>(); }
      siz=0;first=last=-1;
    }
    void reserve(int arg) { index.reserve(arg); bufor.reserve(AssocMatrixAddr<aType>::bufLen(arg)); }

        protected:
            struct DefragMatrixPom {
                    Klucz u, v;
                    Elem val;
            };

    public:
    void defrag()
    {   DefragMatrixPom LOCALARRAY(tab,siz);
        int i=0;
        for(int pos=first;pos!=-1;pos=bufor[pos].next)
        {   tab[i].val=bufor[pos].val;
            std::pair<int,int> wsp=AssocMatrixAddr<aType>::pos2wsp(pos);
            tab[i].u=index.pos2klucz(wsp.first); tab[i].v=index.pos2klucz(wsp.second);
            i++;
        }
        bufor.clear();index.clear(); index.defrag(); siz=0; first=last=-1;
        for(int ii=0;ii<i;ii++)  operator()(tab[ii].u,tab[ii].v)=tab[ii].val;
    }

    template <class Iterator>
        int getKeys(Iterator iter)
        {   for(std::pair<Klucz,Klucz> key=firstKey();key.first;key=nextKey(key))
            {   *iter=key;
                iter++;
            }
            return size();
        }


};

template <class T> class AssocInserter : public std::iterator<std::output_iterator_tag,void,void,void,void>
{
protected:
    T* container;
    AssocTabInterface<T> test;
public:

  typedef T container_type;
  AssocInserter (T& x)
    : container(&x), test(x) {}
  template <class K,class V>
  AssocInserter<T>& operator= (const std::pair<K,V>  & pair)
    { (*container)[(typename AssocTabInterface<T>::KeyType)pair.first]
        =(typename AssocTabInterface<T>::ValType)pair.second;
        return *this;
    }
  AssocInserter<T>& operator* ()
    { return *this; }
  AssocInserter<T> & operator++ ()
    { return *this; }
  AssocInserter<T> operator++ (int)
    { return *this; }
};

template <class T>
AssocInserter<T> assocInserter(T& x) { return AssocInserter<T>(x); }

template <class T, class Fun> class AssocFunktorInserter : public std::iterator<std::output_iterator_tag,void,void,void,void>
{
protected:
    T* container;
    Fun funktor;
    Koala::AssocTabInterface<T> test;
public:

  typedef T container_type;
  typedef Fun FunktorType;
  AssocFunktorInserter (T& x,Fun f )
    : container(&x), funktor(f), test(x) {}
  template <class K>
  AssocFunktorInserter<T,Fun>& operator= (const K & arg)
    { (*container)[(typename AssocTabInterface<T>::KeyType)arg]=
        (typename AssocTabInterface<T>::ValType)funktor(arg); return *this; }
  AssocFunktorInserter<T,Fun>& operator* ()
    { return *this; }
  AssocFunktorInserter<T,Fun> & operator++ ()
    { return *this; }
  AssocFunktorInserter<T,Fun> operator++ (int)
    { return *this; }
};

template <class T, class F>
AssocFunktorInserter<T,F> assocInserter(T& x,F f) { return AssocFunktorInserter<T,F>(x,f); }


template< typename T >
    std::ostream& operator<<(std::ostream& out,AssocTabInterface<T> cont)
{   out << '{';
    int siz=cont.size();
    typename AssocTabInterface<T>::KeyType key=cont.firstKey();
    for(;siz;siz--)
    {   out << '(' << key << ',' << cont[key] << ')';
        if (key!=cont.lastKey())
        { key=cont.nextKey(key); out << ','; }
    }
    out << '}';
    return out;
}



}

#endif


