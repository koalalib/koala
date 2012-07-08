#ifndef SIMPLE_STRUCTS_H
#define SIMPLE_STRUCTS_H

#include <limits>
#include <cassert>
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>


namespace Koala {


template <class Container> class StackInterface;

template <class T> class StackInterface<T*> {
    T* buf;
    int siz,maxsize;

    public:
        typedef T ElemType;

        StackInterface(T* bufor, int max) : buf(bufor), maxsize(max), siz(0) {}
        int size() { return siz; }
        bool empty() { return siz==0; }
        void push(const T& val) { assert(siz<maxsize); buf[siz++]=val; }
        void pop() { assert(siz); siz--; }
        T& top() { assert(siz); return buf[siz-1]; }
        void clear() { siz=0; }
};


template <class Container> class QueueInterface;

template <class T> class QueueInterface<T*> {
    T* buf;
    int beg,end,maxsize;

    int prev(int x) { return (x) ? x-1 : maxsize; }
    int next(int x) { return (x==maxsize) ? 0 : x+1; }
    public:
        typedef T ElemType;

        // wymaga elementu nadmiarowego!
        QueueInterface(T* bufor, int max) : buf(bufor), maxsize(max), beg(0), end(0) {}

        int size() { return (beg<=end) ? end-beg : maxsize+1-(beg-end); }
        bool empty() { return beg==end; }
        void push(const T& val) { buf[end]=val; end=next(end); assert(end!=beg); }
        void pop() { assert(beg!=end); beg=next(beg); }
        T& front() { assert(!empty()); return buf[beg]; }
        T& top() { assert(!empty()); return buf[beg]; }
        T& back() { assert(!empty()); return buf[prev(end)]; }
        void clear() { beg=end=0; }
};


template <class Container> class VectorInterface;

template <class T> class VectorInterface<T*> {
    T *start,*limit;
    int siz;

    public:
    typedef T value_type;

    VectorInterface(T* bufor, int max) : start(bufor), limit(bufor+max), siz(0) {}
//    void link(T* newbuf, int newmax) { start=newbuf; limit=start+newmax; siz=0; }

    T* begin() { return start ; }
    T* end()   { return start+siz ; }
    int size() { return siz ; }
    int max_size() { return limit-start; }
    int capacity() { return limit-start ; }
    bool empty() { return siz==0 ; }
    void reserve(int arg) { assert(arg<=max_size()); }
    void resize(int arg) {   assert(arg<=max_size()); while(siz<arg) push_back(T()); }

    T& operator[] (int pos) { return at(pos); }
    T& at(int pos) { assert(pos>=0 && pos<capacity()); return start[pos]; }
    T& front() { return at(0); }
    T& back() { return at(siz-1); }

    void push_back(const T& arg) { assert(siz<capacity()); start[siz++]=arg; }
    void pop_back() { assert(siz); siz--; }
    static void revert(T* f,T *l)
    {   T z; l--; while(f<l) { z=*f;*f=*l;*l=z; f++;l--; }  }
    template <class InputIterator>
        void assign (InputIterator first, InputIterator last)
    { clear(); for(;first!=last;first++) push_back(*first);  }
    void insert (T* where, int n, const T& x )
    {   assert(siz+n<=max_size());
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

template <class T, class Comp> class PriQueueInterface<T*,Comp> {
    T* buf;
    int siz,maxsize;
    Comp comp;

    public:
        typedef T ElemType;

        PriQueueInterface(T* bufor, int max, Comp acomp=Comp()) : buf(bufor), maxsize(max), siz(0), comp(acomp) {}
        PriQueueInterface(T* bufor, T* end, int max, Comp acomp=Comp()) :
            buf(bufor), maxsize(max), siz(end-bufor), comp(acomp)
        { assert(siz>=0 && siz<=maxsize); std::make_heap(buf,end,comp); }
        int size() { return siz; }
        bool empty() { return siz==0; }
        void push(const T& val) { assert(siz<maxsize); buf[siz++]=val; std::push_heap(buf,buf+siz,comp); }
        void pop() { assert(siz); std::pop_heap(buf,buf+siz,comp); siz--; }
        T top() { assert(siz); return buf[0]; }
        void clear() { siz=0; }
};


}


#endif