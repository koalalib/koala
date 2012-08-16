#ifndef KOALA_SIMPLE_STRUCTS_H
#define KOALA_SIMPLE_STRUCTS_H

#include <limits>
#include <cassert>
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>

#include "../base/exception.h"


//Proste interfejsy udajace kontenery stosu, vectora, kolejki FIFO i kolejki priorytetowej z elementami typu T
// Interfejs wzorowany na STLu
//Dzialaja dostarczonej w konstruktorze na zewnetrznej tablicy gotowych elementow typu T o podanym rozmiarze
//(np. tablica statyczna, dynamiczna new lub LOCALARRAY). Ew. przekraczanie zakresow testowane assertami.
// Interfejsy sa obiektami niekopiowalnymi.
//Przydatne wewnatrz procedur


namespace Koala {


// tworzy pare z min. i max. podanych elementow
template <class T>
std::pair<T,T> pairMinMax(T a, T b)
{
    std::pair<T,T> res;
    if (a<=b) { res.first=a; res.second=b; }
    else { res.first=b; res.second=a; }
    return res;
}

template <class T>
std::pair<T,T> pairMinMax(std::pair<T,T> arg)
{
    return pairMinMax(arg.first,arg.second);
}

//TODO: przewidywano interfejsy innych typow, niz tylko dzialajace na buforze tablicowym, stad deklaracja ogolna
//i specjalizacja klas dla T*. Jednak raczej pozostaniemy przy tablicach. Nalezy zamienic te szablony np. na
//template <class T> class StackInterface<T> (bez specjalizacji). Wymaga modyfikacji deklaracji we wszystkich modulach
//biblioteki korzystajacych z tych interfejsow!



template <class Container> class StackInterface;

template <class T> class StackInterface<T*> {
    T* buf;
    int siz,maxsize;

    StackInterface(const StackInterface<T*>&) {}
    StackInterface<T*>& operator=(const StackInterface<T*>&) {return *this;}

    public:
        typedef T ElemType;

        StackInterface(T* bufor, int max) : buf(bufor), maxsize(max), siz(0) {}
        int size() { return siz; }
        bool empty() { return siz==0; }
        bool operator!() const { return empty(); }
        void push(const T& val) { koalaAssert(siz<maxsize,ContExcFull); buf[siz++]=val; }
        void pop() { koalaAssert(siz,ContExcOutpass); siz--; }
        T& top() { koalaAssert(siz,ContExcOutpass); return buf[siz-1]; }
        void clear() { siz=0; }
    template <class InputIterator>
        void assign (InputIterator first, InputIterator last)
    { clear(); for(;first!=last;first++) push(*first);  }
};


template <class Container> class QueueInterface;

template <class T> class QueueInterface<T*> {
    T* buf;
    int beg,end,maxsize;

    QueueInterface(const QueueInterface<T*>&) {}
    QueueInterface<T*>& operator=(const QueueInterface<T*>&) {return *this;}

    int prev(int x) { return (x) ? x-1 : maxsize; }
    int next(int x) { return (x==maxsize) ? 0 : x+1; }
    public:
        typedef T ElemType;

        // Uwaga: wymaga elementu nadmiarowego tj. bufora o jeden dluzszego, niz maksymalny rozmiar
        QueueInterface(T* bufor, int max) : buf(bufor), maxsize(max), beg(0), end(0) {}

        int size() { return (beg<=end) ? end-beg : maxsize+1-(beg-end); }
        bool empty() { return beg==end; }
        bool operator!() const { return empty(); }
        void push(const T& val) { buf[end]=val; end=next(end); koalaAssert(end!=beg,ContExcFull); }
        void pop() { koalaAssert(beg!=end,ContExcOutpass); beg=next(beg); }
        T& front() { koalaAssert(!empty(),ContExcOutpass); return buf[beg]; }
        T& top() { koalaAssert(!empty(),ContExcOutpass); return buf[beg]; }
        T& back() { koalaAssert(!empty(),ContExcOutpass); return buf[prev(end)]; }
        void clear() { beg=end=0; }
    template <class InputIterator>
        void assign (InputIterator first, InputIterator last)
    { clear(); for(;first!=last;first++) push(*first);  }

};


template <class Container> class VectorInterface;

template <class T> class VectorInterface<T*> {
    T *start,*limit;
    int siz;

    VectorInterface(const VectorInterface<T*>&) {}
    VectorInterface<T*>& operator=(const VectorInterface<T*>&) {return *this;}

    public:
    typedef T value_type;

    VectorInterface(T* bufor, int max) : start(bufor), limit(bufor+max), siz(0) {}

    T* begin() { return start ; }
    T* end()   { return start+siz ; }
    int size() { return siz ; }
    int max_size() { return limit-start; }
    int capacity() { return limit-start ; }
    bool empty() { return siz==0 ; }
    bool operator!() const { return empty(); }
    void reserve(int arg) { koalaAssert(arg<=max_size(),ContExcFull); }
    void resize(int arg) {  koalaAssert(arg<=max_size(),ContExcFull); while(siz<arg) push_back(T()); }

    T& operator[] (int pos) { return at(pos); }
    T& at(int pos) { koalaAssert(pos>=0 ,ContExcOutpass); koalaAssert(pos<capacity(),ContExcFull);return start[pos]; }
    T& front() { return at(0); }
    T& back() { return at(siz-1); }

    void push_back(const T& arg) { koalaAssert(siz<capacity(),ContExcFull); start[siz++]=arg; }
    void pop_back() { koalaAssert(siz,ContExcOutpass); siz--; }
    static void revert(T* f,T *l)
    {   T z; l--; while(f<l) { z=*f;*f=*l;*l=z; f++;l--; }  }
    template <class InputIterator>
        void assign (InputIterator first, InputIterator last)
    { clear(); for(;first!=last;first++) push_back(*first);  }
    void insert (T* where, int n, const T& x )
    {   koalaAssert(siz+n<=max_size(),ContExcFull);
        for(int i=siz-1;i>=where-start;i--) start[i+n]=start[i];
        for(int i=0;i<n;i++) where[i]=x;
        siz+=n;
    }
    void insert (T* where,const T& x) { insert(where,1,x); }
    template <class InputIterator>
        void insert (T* where, InputIterator first, InputIterator last)
    {   int ile=0;
        for(;first!=last;first++) { push_back(*first); ile++; } if (!ile) return;
        revert(where,start+siz); revert(where,where+ile); revert(where+ile,start+siz);
    }
    void erase(T* f, T* l)
    {   int ile=l-f; for(;f+ile<start+siz;f++) *f=f[ile];
        siz-=ile;
    }
    void erase(T* f) { erase(f,f+1); }
    void clear() { siz=0; }

    void clearBuf(T val=T()) { for(T* i=start;i<limit;i++) *i=val; }
};


template <class Container, class Comp> class PriQueueInterface;

// TODO: po usunieciu specjalizacji dac wartosc domyslna Comp=std::greater<T>
template <class T, class Comp> class PriQueueInterface<T*,Comp> {
    T* buf;
    int siz,maxsize;
    Comp comp;

    PriQueueInterface(const PriQueueInterface<T*,Comp>&) {}
    const PriQueueInterface<T*,Comp>& operator=(const PriQueueInterface<T*,Comp>&) {return *this; }

    public:
        typedef T ElemType;

        PriQueueInterface(T* bufor, int max, Comp acomp=Comp()) : buf(bufor), maxsize(max), siz(0), comp(acomp) {}
        PriQueueInterface(T* bufor, T* end, int max, Comp acomp=Comp()) :
            buf(bufor), maxsize(max), siz(end-bufor), comp(acomp)
        { koalaAssert(siz>=0 && siz<=maxsize,ContExcWrongArg); std::make_heap(buf,end,comp); }
        int size() { return siz; }
        bool empty() { return siz==0; }
        bool operator!() const { return empty(); }
        void push(const T& val) { koalaAssert(siz<maxsize,ContExcFull); buf[siz++]=val; std::push_heap(buf,buf+siz,comp); }
        void pop() { koalaAssert(siz,ContExcOutpass); std::pop_heap(buf,buf+siz,comp); siz--; }
        T top() { koalaAssert(siz,ContExcOutpass); return buf[0]; }
        void clear() { siz=0; }
    template <class InputIterator>
        void assign (InputIterator first, InputIterator last)
    { clear(); for(siz=0;first!=last;first++,siz++)
        { buf[siz]=(*first);  koalaAssert(siz<maxsize,ContExcFull); }
        std::make_heap(buf,buf+siz,comp);
    }

};


}


#endif
