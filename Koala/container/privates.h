#ifndef KOALA_PRIVATE_STRUCTS
#define KOALA_PRIVATE_STRUCTS

#include <map>
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include "localarray.h"
#include "simple.h"

namespace Koala
{


template <class Container> class VectorInterface;

namespace Privates {

template< class Element > struct BlockOfBlockList
{
    public:
        Element blob;
        int prev, next;
} ;


template <class T> class BlockListVectInerfTest {};

template <class T> class BlockListVectInerfTest<VectorInterface<T> >
{   public:
    BlockListVectInerfTest(bool) {}
    private:
    BlockListVectInerfTest(const BlockListVectInerfTest<VectorInterface<T> >&) {}
};


// Struktura alokatora wolnych blokow (lista) typu Element zorganizowanych w kontenerze w rodzaju tablicy
// Pomocna w AssocArray i alokatorach
template< class Element, class Container = std::vector< BlockOfBlockList< Element > > >
class BlockList : protected BlockListVectInerfTest<Container>
{
    protected:
        int siz, first, last, ffree;
        Container cont;

    public:
        typedef Element ElementType;
        typedef BlockOfBlockList< Element > BlockType;

        BlockList( int = 0 );
        BlockList( const BlockList< Element,Container > & );
        BlockList< Element,Container > &operator=(
            const BlockList< Element,Container > & );

        // tylko dla Container==VectorInterface<BlockOfBlockList< Element > *>
        // BlockList moze tez dzialac na dostarczonej z zewnatrz tablicy ustalonego rozmiaru, na ktorej operuje wowczas
        // poprzez VectorInterface, wylapujac bledy ew. przepelnienia
        BlockList(BlockOfBlockList< Element >* wsk, int asize)
        :   cont(wsk,asize) ,siz( 0 ),first( -1 ),last( -1 ),ffree( -1 ),
            Privates::BlockListVectInerfTest<Container>(true)
        { cont.clear(); }

        bool ready( int );
        void clear();
        int size() { return siz; }
        bool empty() { return !siz; }
        void reserve( int arg ) { cont.reserve( arg ); }
        bool full() { return ffree == -1; }
        int capacity() { return cont.capacity(); }
        int firstPos()  { return first; }
        int lastPos()   { return last; }
        int nextPos( int );
        int prevPos( int );
        Element &operator[]( int i ) { return cont[i].blob; }
        int newPos( int = -1 );
        void delPos( int );
        void defrag();
        bool test();
};


    // Domyslny alokator pamieci uzywajacy new
    class DefaultCPPAllocator {
    public:
        template<class T> T *allocate()			{ return new T(); };
        template<class T> void deallocate(T *p)		{ delete p; };
        template<class T> T *allocate(size_t n)		{ return new T[n]; };
        template<class T> void deallocate(T *p, size_t)	{ delete[] p; };
        } ;



    // ten alokator potrafi dostarczac jedynie zmiennych typu Element, operuje
    // na tworzonej na poczatku tablicy o podnym rozmiarze
    // Poprzez BlockList kontroluje przepelnienia
    template <class Element>
    class BlockListAllocator
    {

        private:

            BlockOfBlockList< Element >* wsk;
            BlockList<Element,VectorInterface<BlockOfBlockList< Element > *> > *manager;

            BlockListAllocator(const BlockListAllocator& ){}
            const BlockListAllocator& operator=(const BlockListAllocator&) {}

            template <class U,class W> class TestEqTypes;
            template <class U> class TestEqTypes<U,U> {};

            Element* alloc() { return &(manager->operator[](manager->newPos())); }

            void dealloc(Element* ptr)
            {   if (!ptr) return;
                int pos=(((char*)ptr)-((char*)wsk))/sizeof(BlockOfBlockList< Element >);
                assert(manager->ready(pos));
                manager->delPos(pos);
            }



        public:
            BlockListAllocator(int n)
            {   wsk=new BlockOfBlockList< Element >[n];
                manager=new BlockList<Element,VectorInterface<BlockOfBlockList< Element > *> >(wsk,n);
            }

            ~BlockListAllocator()
            {
                delete manager;
                delete [] wsk;
            }

            template<class U> U *allocate()
           { TestEqTypes<U,Element> x; return alloc(); };
           template<class U> void deallocate(U *p)
           { TestEqTypes<U,Element> x; dealloc(p); };
    };



    template <class T> struct ListNode;
    template <class T>	struct BaseListNode {
            ListNode<T> *next;
            ListNode<T> *prev;
            };
    template <class T>	struct ListNode: public BaseListNode<T> {
            T elem;
            };

    /*
     * List_iterator
     */
    template<class T>
    struct List_iterator {
    //public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;

        List_iterator(): ptr(NULL)				{};
        List_iterator(const List_iterator &i): ptr(i.ptr)	{};

        List_iterator next()					{ return List_iterator(ptr->next); };
        List_iterator prev()					{ return List_iterator(ptr->prev); };

        List_iterator &operator =(const List_iterator &i)	{ ptr = i.ptr; return *this; };

        bool operator ==(const List_iterator &i) const		{ return ptr == i.ptr; };
        bool operator !=(const List_iterator &i) const		{ return ptr != i.ptr; };

        T &operator *()			{ return ptr->elem; };
        const T &operator *() const	{ return ptr->elem; };
        T *operator ->()		{ return &(ptr->elem); };
        const T *operator ->() const	{ return &(ptr->elem); };

        List_iterator &operator++() {
            ptr = ptr->next;
            return *this;
            };

        List_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
            };

        List_iterator operator++(int) {
            List_iterator it;
            it = *this;
            ++(*this);
            return it;
            };

        List_iterator operator--(int) {
            List_iterator it;
            it = *this;
            --(*this);
            return it;
            };

    //	friend class List<T, Allocator>;
    //	friend class List_const_iterator<T, Allocator>;

    //private:
        List_iterator(ListNode<T> *p): ptr(p)	{};

    //private:
        ListNode<T> *ptr;
        };


    /*
     * List_const_iterator
     */
    template<class T>
    struct List_const_iterator {
    //public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;

        List_const_iterator(): ptr(NULL)				{};
        List_const_iterator(const List_const_iterator &i): ptr(i.ptr)	{};
        List_const_iterator(const List_iterator<T> &i): ptr(i.ptr)		{};

        List_const_iterator next()					{ return List_const_iterator(ptr->next); };
        List_const_iterator prev()					{ return List_const_iterator(ptr->prev); };

        List_const_iterator &operator =(const List_const_iterator &i)	{ ptr = i.ptr; return *this; };

        bool operator ==(const List_const_iterator &i) const		{ return ptr == i.ptr; };
        bool operator !=(const List_const_iterator &i) const		{ return ptr != i.ptr; };

        T &operator *()			{ return ptr->elem; };
        const T &operator *() const	{ return ptr->elem; };
        T *operator ->()		{ return &(ptr->elem); };
        const T *operator ->() const	{ return &(ptr->elem); };

        List_const_iterator &operator++() {
            ptr = ptr->next;
            return *this;
            };

        List_const_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
            };

        List_const_iterator operator++(int) {
            List_const_iterator it;
            it = *this;
            ++(*this);
            return it;
            };

        List_const_iterator operator--(int) {
            List_const_iterator it;
            it = *this;
            --(*this);
            return it;
            };

    //	friend class List<T, Allocator>;

    //private:
        List_const_iterator(ListNode<T> *p): ptr(p)	{};

    //private:
        ListNode<T> *ptr;
        };


    /*
     * List
     * lista podobna do stl-owej
     * size ma zÂ³oÂ¿onoÅ“Ã¦ O(1) (std::list ma O(n))
     * interfejs jest uboÂ¿szy (nie z przyczyn technicznych -- czÃªÅ“Ã¦ metod nie
     * byÂ³a potrzebna)
     * dodatkowe metody:
     * insert_after(iterator, value)
     * insert_before(iterator, value)
     * move_after(iterator, iterator) -- przeniesienie wewnÂ¹trz jednej listy
     * move_before(iterator, iterator) -- przeniesienie wewnÂ¹trz jednej listy
     *
     */
    template<class T, class Allocator = DefaultCPPAllocator>
    class List {
    public:
        typedef T value_type;
        typedef List_iterator<T> iterator;
        typedef List_const_iterator<T> const_iterator;
    //	typedef ListNode<T> ListNode;

    	List(): allocator()	{ Zero(); }; // niebezpieczny, do tworzenia tablic list, nalezy natychmiast wykonac init
    	void init(Allocator &a) { allocator=&a; }
        List(Allocator &a): allocator(&a)		{ Zero(); };
        List(const List &lst): allocator(lst.allocator)	{ Zero(); *this = lst; };
        ~List()						{ clear(); };

        iterator begin()			{ return iterator(m_root.next); };
        const_iterator begin() const		{ return const_iterator(m_root.next); };
        iterator end()				{ return iterator((ListNode<T> *)(&m_root)); };
        const_iterator end() const		{ return const_iterator((ListNode<T> *)&m_root); };

        T &front()				{ return m_root.next->elem; };
        const T &front() const			{ return m_root.next->elem; };

        T &back()				{ return m_root.prev->elem; };
        const T &back() const			{ return m_root.prev->elem; };

        bool empty() const			{ return m_count == 0; };

        size_t size() const			{ return m_count; };

        iterator find(const T &v) 		{ return iterator(_find(v)); };
        const_iterator find(const T &v) const	{ return const_iterator(_find(v)); };

        void push_front(const T &v)		{ _insert_before(m_root.next, v); };
        void pop_front()			{ _erase(m_root.next); };
        void push_back(const T &v)		{ _insert_after(m_root.prev, v); };
        void pop_back()				{ _erase(m_root.prev); };

        void clear()				{ while(!empty()) pop_front(); };

        iterator insert(const_iterator pos, const T &v)		{ return iterator(_insert_before(pos.ptr, v)); };
        iterator insert_before(const_iterator pos, const T &v)	{ return iterator(_insert_before(pos.ptr, v)); };
        iterator insert_after(const_iterator pos, const T &v)	{ return iterator(_insert_after(pos.ptr, v)); };

        // przesuniÃªcie wewnÂ¹trz kontenera, a NIE miÃªdzy kontenerami!
        void move_before(iterator pos, iterator elem) {
            _unlink(elem.ptr);
            _link_before(pos.ptr, elem.ptr);
            };

        // przesuniÃªcie wewnÂ¹trz kontenera, a NIE miÃªdzy kontenerami!
        void move_after(iterator pos, iterator elem) {
            _unlink(elem.ptr);
            _link_after(pos.ptr, elem.ptr);
            };

        void erase(iterator pos)				{ _erase(pos.ptr); };

        template <class Alloc>
        void copy(const List<T, Alloc> &lst) {
            const_iterator it, e;
            if(this == (List*)(&lst)) return;
            clear();
            for(it = lst.begin(), e = lst.end(); it != e; ++it) push_back(*it);
            };

        List &operator = (const List<T, Allocator> &lst) {
            const_iterator it, e;
            if(this == &lst) return *this;
            if (!allocator) allocator=lst.allocator;
            clear();
            for(it = lst.begin(), e = lst.end(); it != e; ++it) push_back(*it);
            return *this;
            };

        void swap(List &o) {
            size_t t;
            ListNode<T> *p;
            p = m_root.next; m_root.next = o.m_root.next; o.m_root.next = p;
            p = m_root.prev; m_root.prev = o.m_root.prev; o.m_root.prev = p;
            t = m_count; m_count = o.m_count; o.m_count = t;
            };

        void sort() {
            ListNode<T> *a, *b, *pa;
            size_t i;
            if(m_count < 2) return;
            if(m_count == 2) {
                if(m_root.prev->elem < m_root.next->elem) {
                    a = m_root.next;
                    b = m_root.prev;
                    b->next = a;
                    b->prev = (ListNode<T> *)&m_root;
                    a->prev = b;
                    a->next = (ListNode<T> *)&m_root;
                    m_root.next = b;
                    m_root.prev = a;
                    };
                return;
                };
            a = b = m_root.next;
            i = m_count / 2;
            while(i) { b = b->next; i--; };
            a = b->prev;
            a->next = NULL;
            b->prev = NULL;
            List<T, Allocator> other(b, m_root.prev, m_count - m_count / 2,allocator);
            m_count = m_count / 2;
            m_root.prev = a;
            a->next = (ListNode<T> *)&m_root;
            sort();
            other.sort();
            merge(other);
            };

        // czyÅ“ci zawartoÅ“Ã¦ o
        void merge(List &o) {
            ListNode<T> *a, *b, *e, *ae, *be;
            if(&o == this) return;
            if(o.empty()) return;
            if(empty()) { swap(o); return; };
            a = m_root.next;
            b = o.m_root.next;
            ae = (ListNode<T> *)&m_root;
            be = (ListNode<T> *)&(o.m_root);
            if(a->elem < b->elem) {
                m_root.next = e = a;
                a->prev = (ListNode<T> *)&m_root;
                a = a->next;
            } else {
                m_root.next = e = b;
                b->prev = (ListNode<T> *)&m_root;
                b = b->next;
                };
            if(a != ae && b != be) {
                while(true) {
                    if(a->elem < b->elem) {
                        e->next = a;
                        a->prev = e;
                        e = a;
                        if((a = a->next) == NULL) break;
                    } else {
                        e->next = b;
                        b->prev = e;
                        e = b;
                        if((b = b->next) == be) break;
                        };
                    };
                };
            if(a == ae) {
                e->next = b;
                b->prev = e;
                m_root.prev = o.m_root.prev;
                o.m_root.prev->next = (ListNode<T> *)&m_root;
            } else {
                e->next = a;
                a->prev = e;
                };
            m_count = m_count + o.m_count;
            o.Zero();
            };

        friend struct List_iterator<T>;
        friend struct List_const_iterator<T>;

    private:
        List(ListNode<T> *n, ListNode<T> *p, size_t c,Allocator *a): allocator(a) {
            m_root.next = n;
            m_root.prev = p;
            n->prev = (ListNode<T> *)&m_root;
            p->next = (ListNode<T> *)&m_root;
            m_count = c;
            };

        void Zero() {
            m_root.next = (ListNode<T> *)&m_root;
            m_root.prev = (ListNode<T> *)&m_root;
            m_count = 0;
            };

        ListNode<T> *NewElem() {
            return allocator->template allocate<ListNode<T> >();
            };

        ListNode<T> *_find(const T &v) {
            ListNode<T> *n = m_root.next;
            while(n != NULL) {
                if(n->elem == v) break;
                n = n->next;
                };
            return n;
            };

        void _link_before(ListNode<T> *ptr, ListNode<T> *p) {
            p->next = ptr;
            p->prev = ptr->prev;
            p->next->prev = p;
            p->prev->next = p;
            };

        void _link_after(ListNode<T> *ptr, ListNode<T> *p) {
            p->next = ptr->next;
            p->prev = ptr;
            p->next->prev = p;
            p->prev->next = p;
            };

        void _unlink(ListNode<T> *p) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            };

        ListNode<T> *_insert_before(ListNode<T> *ptr, const T &v) {
            ListNode<T> *p = NewElem();
            if(p == NULL) return NULL;
            _link_before(ptr, p);
            p->elem = v;
            m_count++;
            return p;
            };

        ListNode<T> *_insert_after(ListNode<T> *ptr, const T &v) {
            ListNode<T> *p = NewElem();
            if(p == NULL) return NULL;
            _link_after(ptr, p);
            p->elem = v;
            m_count++;
            return p;
            };

        void _erase(ListNode<T> *p) {
            if(empty()) return;
            if(p == (ListNode<T> *)&m_root) return;
            _unlink(p);
            allocator->deallocate(p);
            m_count--;
            };


    private:
        BaseListNode<T> m_root;
        size_t m_count;
        Allocator* allocator;
        };


    template<class T, class Allocator>
    std::ostream &operator <<(std::ostream &strm, const List<T, Allocator> &lst) {
        typename List<T, Allocator>::const_iterator b, e, it;
        strm << "[";
        b = lst.begin();
        e = lst.end();
        for(it = b; it != e; ++it) {
            if(it != b) strm << ", ";
            else strm << " ";
            strm << *it;
            };
        strm << " ]";
        return strm;
        };



#include "privates.hpp"

}

}

#endif
