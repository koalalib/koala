#ifndef LIST_H
#define LIST_H

#include<iterator>
#include<ostream>

namespace Koala {

template<class T, class Allocator> class List;
template<class T, class Allocator> class List_iterator;
template<class T, class Allocator> class List_const_iterator;


/*
 * ListDefaultCPPAllocator
 */
class ListDefaultCPPAllocator {
public:
	template<class T> T *allocate()			{ return new T(); };
	template<class T> void deallocate(T *p)		{ delete p; };
	template<class T> T *allocate(size_t n)		{ return new T[n]; };
	template<class T> void deallocate(T *p, size_t)	{ delete[] p; };
	};


/*
 * List_iterator
 */
template<class T, class Allocator>
class List_iterator {
public:
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

	friend class List<T, Allocator>;
	friend class List_const_iterator<T, Allocator>;

private:
	List_iterator(typename List<T, Allocator>::Node *p): ptr(p)	{};

private:
	typename List<T, Allocator>::Node *ptr;
	};


/*
 * List_const_iterator
 */
template<class T, class Allocator>
class List_const_iterator {
public:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T *pointer;
	typedef T &reference;

	List_const_iterator(): ptr(NULL)				{};
	List_const_iterator(const List_const_iterator &i): ptr(i.ptr)	{};
	List_const_iterator(const List_iterator<T, Allocator> &i): ptr(i.ptr)		{};

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

	friend class List<T, Allocator>;

private:
	List_const_iterator(typename List<T, Allocator>::Node *p): ptr(p)	{};

private:
	typename List<T, Allocator>::Node *ptr;
	};


/*
 * List
 * lista podobna do stl-owej
 * size ma z³o¿onoœæ O(1) (std::list ma O(n))
 * interfejs jest ubo¿szy (nie z przyczyn technicznych -- czêœæ metod nie
 * by³a potrzebna)
 * dodatkowe metody:
 * insert_after(iterator, value)
 * insert_before(iterator, value)
 * move_after(iterator, iterator) -- przeniesienie wewn¹trz jednej listy
 * move_before(iterator, iterator) -- przeniesienie wewn¹trz jednej listy
 *
 */
template<class T, class Allocator = ListDefaultCPPAllocator>
class List {
private:
	struct Node;

	struct BaseNode {
		Node *next;
		Node *prev;
		};

	struct Node: public BaseNode {
		T elem;
		};

public:
	typedef T value_type;
	typedef List_iterator<T, Allocator> iterator;
	typedef List_const_iterator<T, Allocator> const_iterator;

	List(): allocator()				{ Zero(); };
	List(const Allocator &a): allocator(a)		{ Zero(); };
	List(const List &lst)				{ Zero(); *this = lst; };
	~List()						{ clear(); };

	iterator begin()			{ return iterator(m_root.next); };
	const_iterator begin() const		{ return const_iterator(m_root.next); };
	iterator end()				{ return iterator((Node *)(&m_root)); };
	const_iterator end() const		{ return const_iterator((Node *)&m_root); };

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

	// przesuniêcie wewn¹trz kontenera, a NIE miêdzy kontenerami!
	void move_before(iterator pos, iterator elem) {
		_unlink(elem.ptr);
		_link_before(pos.ptr, elem.ptr);
		};

	// przesuniêcie wewn¹trz kontenera, a NIE miêdzy kontenerami!
	void move_after(iterator pos, iterator elem) {
		_unlink(elem.ptr);
		_link_after(pos.ptr, elem.ptr);
		};

	void erase(iterator pos)				{ _erase(pos.ptr); };

	List &operator = (const List<T, Allocator> &lst) {
		const_iterator it, e;
		if(this == &lst) return *this;
		clear();
		for(it = lst.begin(), e = lst.end(); it != e; ++it) push_back(*it);
		return *this;
		};

	void swap(List &o) {
		size_t t;
		Node *p;
		p = m_root.next; m_root.next = o.m_root.next; o.m_root.next = p;
		p = m_root.prev; m_root.prev = o.m_root.prev; o.m_root.prev = p;
		t = m_count; m_count = o.m_count; o.m_count = t;
		};

	void sort() {
		Node *a, *b, *pa;
		size_t i;
		if(m_count < 2) return;
		if(m_count == 2) {
			if(m_root.prev->elem < m_root.next->elem) {
				a = m_root.next;
				b = m_root.prev;
				b->next = a;
				b->prev = (Node *)&m_root;
				a->prev = b;
				a->next = (Node *)&m_root;
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
		List<T, Allocator> other(b, m_root.prev, m_count - m_count / 2);
		m_count = m_count / 2;
		m_root.prev = a;
		a->next = (Node *)&m_root;
		sort();
		other.sort();
		merge(other);
		};

	// czyœci zawartoœæ o
	void merge(List &o) {
		Node *a, *b, *e, *ae, *be;
		if(&o == this) return;
		if(o.empty()) return;
		if(empty()) { swap(o); return; };
		a = m_root.next;
		b = o.m_root.next;
		ae = (Node *)&m_root;
		be = (Node *)&(o.m_root);
		if(a->elem < b->elem) {
			m_root.next = e = a;
			a->prev = (Node *)&m_root;
			a = a->next;
		} else {
			m_root.next = e = b;
			b->prev = (Node *)&m_root;
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
			o.m_root.prev->next = (Node *)&m_root;
		} else {
			e->next = a;
			a->prev = e;
			};
		m_count = m_count + o.m_count;
		o.Zero();
		};

	friend class List_iterator<T, Allocator>;
	friend class List_const_iterator<T, Allocator>;

private:
	List(Node *n, Node *p, size_t c): allocator() {
		m_root.next = n;
		m_root.prev = p;
		n->prev = (Node *)&m_root;
		p->next = (Node *)&m_root;
		m_count = c;
		};

	void Zero() {
		m_root.next = (Node *)&m_root;
		m_root.prev = (Node *)&m_root;
		m_count = 0;
		};

	Node *NewElem() {
		return allocator.template allocate<typename List<T, Allocator>::Node>();
		};

	Node *_find(const T &v) {
		Node *n = m_root.next;
		while(n != NULL) {
			if(n->elem == v) break;
			n = n->next;
			};
		return n;
		};

	void _link_before(Node *ptr, Node *p) {
		p->next = ptr;
		p->prev = ptr->prev;
		p->next->prev = p;
		p->prev->next = p;
		};

	void _link_after(Node *ptr, Node *p) {
		p->next = ptr->next;
		p->prev = ptr;
		p->next->prev = p;
		p->prev->next = p;
		};

	void _unlink(Node *p) {
		p->prev->next = p->next;
		p->next->prev = p->prev;
		};

	Node *_insert_before(Node *ptr, const T &v) {
		Node *p = NewElem();
		if(p == NULL) return NULL;
		_link_before(ptr, p);
		p->elem = v;
		m_count++;
		return p;
		};

	Node *_insert_after(Node *ptr, const T &v) {
		Node *p = NewElem();
		if(p == NULL) return NULL;
		_link_after(ptr, p);
		p->elem = v;
		m_count++;
		return p;
		};

	void _erase(Node *p) {
		if(empty()) return;
		if(p == (Node *)&m_root) return;
		_unlink(p);
		allocator.deallocate(p);
		m_count--;
		};


private:
	BaseNode m_root;
	size_t m_count;
	Allocator allocator;
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

}

#endif
