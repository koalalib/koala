#ifndef __HASHCONTAINERS__H__
#define __HASHCONTAINERS__H__

#include<stddef.h>
#include<new>
#include<string>
#include<utility>
#include<iterator>
#include <cassert>
#include <limits>

#ifndef __GNUC__
typedef unsigned __int64	uint64_t;
typedef unsigned __int32	uint32_t;
#endif

namespace Koala {

#ifdef HASHSETDEBUG
#define COLLISION()		collisions++;
#else
#define COLLISION()
#endif

#define HASHSETEMPTYPTR		((void *)0)
#define HASHSETNONEXTPTR	((void *)1)
#define HASHSETSENTRYPTR	((void *)2)
#define HASHSETVALIDPTR		((void *)3)


template<class T, class H, class A> class HashSet;
template<class T, class H, class A> class HashSet_const_iterator;


/*
 * HashDefaultCPPAllocator
 */
class HashDefaultCPPAllocator {
public:
	template<class T> T *allocate()			{ return new T(); };
	template<class T> T *allocate(size_t n)		{ return new T[n]; };
	template<class T> void deallocate(T *p)		{ delete p; };
	template<class T> void deallocate(T *p, size_t)	{ delete[] p; };
	};


/*
 * HSNode
 */
template<class KeyType>
struct HSNode {
	KeyType key;
	HSNode *next;
	};


/*
 * HashSetTableList
 */
template<class KeyType>
class HashSetTableList {
public:
	HashSetTableList<KeyType> *next;
	size_t size;
	HSNode<KeyType> array[];
	};


/*
 * HashSet_const_iterator
 */
template<class KeyType,
	 class HashFunction,
	 class Allocator>
class HashSet_const_iterator: public std::forward_iterator_tag {
public:
	typedef std::forward_iterator_tag iterator_category;
	typedef KeyType value_type;
	typedef ptrdiff_t difference_type;
	typedef KeyType *pointer;
	typedef KeyType &reference;

	HashSet_const_iterator()					{ };
	HashSet_const_iterator(const HashSet_const_iterator<KeyType, HashFunction, Allocator> &it)	{ *this = it; };
	~HashSet_const_iterator()					{ };

	KeyType &operator *()						{ return m_cur->key; };
	KeyType *operator ->()						{ return &(m_cur->key); };

	HashSet_const_iterator &operator++()			{ advance(); return *this; };
	HashSet_const_iterator operator++(int)			{ HashSet_const_iterator rv(*this); advance(); return rv; };
	HashSet_const_iterator &operator =(const HashSet_const_iterator<KeyType, HashFunction, Allocator> &it)	{ m_slot = it.m_slot; m_cur = it.m_cur; return *this; };

	bool operator ==(const HashSet_const_iterator<KeyType, HashFunction, Allocator> &it)	{ return m_slot == it.m_slot && m_cur == it.m_cur; };
	bool operator !=(const HashSet_const_iterator<KeyType, HashFunction, Allocator> &it)	{ return m_slot != it.m_slot || m_cur != it.m_cur; };
private:
	HashSet_const_iterator(HSNode<KeyType> *slot)			{ m_slot = m_cur = slot; advance_if_needed(); };
	HashSet_const_iterator(HSNode<KeyType> *slot, bool)		{ m_slot = m_cur = slot; };

	void advance_if_needed() {
		if(m_cur->next == HASHSETEMPTYPTR) advance();
		};

	void advance() {
		if(m_cur->next <= HASHSETNONEXTPTR) {
			m_slot++;
			while(m_slot->next == HASHSETEMPTYPTR) m_slot++;
			m_cur = m_slot;
		} else m_cur = m_cur->next;
		};

	HSNode<KeyType> *m_slot, *m_cur;
	friend class HashSet<KeyType, HashFunction, Allocator>;
	};


/*
 * default hash functions
 */
template<class KeyType>
class DefaultHashFun	{};


/*
 * Int32Hash
 *  default hash for int and pointer types
 */
template<class KeyType>
class Int32Hash {
public:
	size_t operator ()(KeyType key, size_t m) const {
		return ((uint64_t)(uint32_t)(((uint32_t)key) * 2654435769u) * m) >> 32;
		};
	};

/*
 * StringHash
 *  default hash for strings
 */
class StringHash {
public:
	size_t operator ()(const std::string &key, size_t m) const {
		size_t v = 0, i, len;
		for(len = key.size(), i = 0; i < len; i++) {
			v += (size_t)key[i];
			v *= 2654435769u;
			};
		return ((uint64_t)v * m) >> 32;
		};
	};

/*
 * CStringHash
 *  default hash for char *, const char *, wchar_t *, const wchar_t *
 */
template<class CTYPE>
class CStringHash {
public:
	size_t operator ()(const CTYPE *key, size_t m) const {
		size_t i, v = 0;
		for(i = 0; key[i]; i++) {
			v += (size_t)key[i];
			v *= 2654435769u;
			};
		return ((uint64_t)v * m) >> 32;
		};
	};

template<> class DefaultHashFun<int>: public Int32Hash<int>			{ };
template<> class DefaultHashFun<long>: public Int32Hash<long>			{ };
template<> class DefaultHashFun<short>: public Int32Hash<short>			{ };
template<> class DefaultHashFun<unsigned int>: public Int32Hash<unsigned int>	{ };
template<> class DefaultHashFun<unsigned long>: public Int32Hash<unsigned long>	{ };
template<> class DefaultHashFun<unsigned short>: public Int32Hash<unsigned short>	{ };
template<class T> class DefaultHashFun<T *>: public Int32Hash<T *>		{ };

template<> class DefaultHashFun<char *>: public CStringHash<char>			{ };
template<> class DefaultHashFun<const char *>: public CStringHash<char>		{ };
template<> class DefaultHashFun<wchar_t *>: public CStringHash<wchar_t>		{ };
template<> class DefaultHashFun<const wchar_t *>: public CStringHash<wchar_t>	{ };
template<> class DefaultHashFun<std::string>: public StringHash			{ };


/*
 * HashSet
 * KeyType - type of element
 * HashFunction - hash object
 * 	-> size_t operator()(const KeyType &key, size_t m) const
 * 	   or size_t operator()(KeyType key, size_t m) const
 */
template<class KeyType,
	 class HashFunction = DefaultHashFun<KeyType>,
	 class Allocator = HashDefaultCPPAllocator>
class HashSet {
public:
	static const size_t node_size = sizeof(HSNode<KeyType>);

	typedef KeyType value_type;
	typedef KeyType key_type;
	typedef KeyType &reference;
	typedef const KeyType &const_reference;
	typedef KeyType *pointer;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;

	typedef HashSet_const_iterator<KeyType, HashFunction, Allocator> iterator;
	typedef HashSet_const_iterator<KeyType, HashFunction, Allocator> const_iterator;

public:
	HashSet(): m_table(NULL), m_tables(NULL)	{ };
	HashSet(size_t size)				{ initialize(size); };
	HashSet(const HashSet &t) 			{ initialize(t.m_size); *this = t; };
	template<class HF, class Alloc>
	HashSet(const HashSet<KeyType, HF, Alloc> &t) 	{ initialize(t.m_size); *this = t; };

	~HashSet()					{ free(true); };

	HashSet &operator =(const HashSet &t) {
		iterator it, e;
		if(m_table == NULL) initialize(t.m_size);
		clear();
		for(it = t.begin(), e = t.end(); it != e; ++it) insert(*it);
		return *this;
		};

	template<class SetType>
	HashSet &operator =(const SetType &t) {
		typename SetType::iterator it, e;
		if(m_table == NULL) initialize(t.size());
		clear();
		for(it = t.begin(), e = t.end(); it != e; ++it) insert(*it);
		return *this;
		};

	iterator begin()		{ return iterator(m_table); };
	iterator end()			{ return iterator(m_table + m_size, true); };
	const_iterator begin() const	{ return const_iterator(m_table); };
	const_iterator end() const	{ return const_iterator(m_table + m_size, true); };

	size_t size() const		{ return m_count; };
	size_t capacity() const		{ return (size_t)(-1); };
	size_t slots() const		{ return m_size; };

	bool empty() const		{ return m_count == 0; };

	iterator find(const KeyType &key)			{ return Find(key); };
	const_iterator find(const KeyType &key) const		{ return const_iterator(Find(key)); };
	std::pair<iterator, bool> insert(const KeyType &key)	{ return find_or_insert(key); };

	void resize(size_t size) {
		free(true);
		initialize(size);
		};


	void clear() {
		free(false);
		m_count = 0;
		m_firstFree = NULL;
		};


	std::pair<iterator, bool> find_or_insert(const KeyType &key) {
		HSNode<KeyType> *c, *p;
		c = m_table + hashfn(key, m_size);

		if(c->next == HASHSETEMPTYPTR) {
			new (&(c->key)) key_type(key);
			c->next = (HSNode<KeyType> *)HASHSETNONEXTPTR;
			m_count++;
			return std::make_pair(iterator(c, true), true);
			};
		if(c->key == key) return std::make_pair(iterator(c, true), false);

		COLLISION();
		p = c->next;
		while((void *)p >= HASHSETVALIDPTR) {
			if(p->key == key) return std::make_pair(iterator(p, true), false);
			COLLISION();
			p = p->next;
			};

		p = make_overflow_node();
		new (&(p->key)) key_type(key);
		p->next = c->next;
		c->next = p;
		m_count++;
		return std::make_pair(iterator(p), true);
		};

	bool contains(const KeyType &key) const {
		HSNode<KeyType> *c;
		c = m_table + hashfn(key, m_size);
		if(c->next == HASHSETEMPTYPTR) return false;
		if(c->key == key) return true;
		c = c->next;
		while(c >= HASHSETVALIDPTR) {
			if(c->key == key) return true;
			COLLISION();
			c = c->next;
			};
		return false;
		};


	void erase(const KeyType &key) {
		HSNode<KeyType> *c, *p;
		c = m_table + hashfn(key, m_size);
		p = NULL;
		if(c->next == HASHSETEMPTYPTR) return;
		while(c >= HASHSETVALIDPTR) {
			if(c->key != key) { COLLISION(); p = c; c = c->next; continue; };
			if(p != NULL) {	// is not first -> have previous
				c->key.~KeyType();
				p->next = c->next;
				c->next = m_firstFree;
				m_firstFree = c;
			} else if(c->next != HASHSETNONEXTPTR) {// have next
				p = c->next;

				c->key = p->key;
//				c->key.~key_type();
//				new (&(c->key)) key_type(key);
				p->key.~KeyType();

				c->next = p->next;
				p->next = m_firstFree;
				m_firstFree = p;
			} else {	// if first and don't have next
				c->key.~KeyType();
				c->next = (HSNode<KeyType> *)HASHSETEMPTYPTR;
				};
			m_count--;
			return;
			};
		};


	void erase(const_iterator pos) {
		erase(pos.m_cur->key);
		};

private:
	HSNode<KeyType> *make_overflow_node() {
		HSNode<KeyType> *rv;
		HashSetTableList<KeyType> *l;

		if(m_firstFree >= HASHSETVALIDPTR) {	// free slot on free list
			rv = m_firstFree;
			m_firstFree = m_firstFree->next;
			return rv;
			};

		if(m_overflowFirst >= m_overflowSize) {	// no free slot on overflow area
			if(m_overflowSize == 0) m_overflowSize = m_size / 2;
			else m_overflowSize *= 2;
			l = CreateTable(m_overflowSize);
			l->next = m_tables->next;
			m_tables->next = l;
			m_overflowFirst = 0;
			};

		// use one slot from overflow area
		rv = m_tables->next->array + m_overflowFirst;
		m_overflowFirst++;
		return rv;
		};


	void initialize(size_t size) {
#ifdef HASHSETDEBUG
		collisions = 0;
#endif
		if(size < 4) size = 4;
		m_count = 0;
		m_size = size;
		m_tables = CreateTable(size + 1);
		m_tables->next = NULL;
		m_table = m_tables->array;
		m_table[size].next = (HSNode<KeyType> *)HASHSETSENTRYPTR;
		m_firstFree = NULL;
		m_overflowFirst = 0;
		m_overflowSize = 0;
		for(size_t i = 0; i < m_size; i++) {
			m_table[i].next = (HSNode<KeyType> *)HASHSETEMPTYPTR;
			};
		};

	void free(bool deleteFirst) {
		HashSetTableList<KeyType> *t, *c;
		if(m_tables == NULL) return;
		eraseAll();
		c = m_tables;
		if(!deleteFirst) c = c->next;
		while(c != NULL) {
			t = c;
			c = c->next;
			allocate.deallocate((char *)t, 0);
			};
		};


	void eraseAll() {
		HSNode<KeyType> *p;
		for(size_t i = 0; i < m_size; i++) {
			p = m_table + i;
			if(p->next != (HSNode<KeyType> *)HASHSETEMPTYPTR) {
				while(p >= (HSNode<KeyType> *)HASHSETVALIDPTR) {
					p->key.~KeyType();
					p = p->next;
					};
				m_table[i].next = (HSNode<KeyType> *)HASHSETEMPTYPTR;
				};
			};
		};


	iterator Find(const KeyType &key) const {
		HSNode<KeyType> *c;
		c = m_table + hashfn(key, m_size);

		if(c->next == HASHSETEMPTYPTR) return end();
		if(c->key == key) return iterator(c, true);
		c = c->next;
		while(c >= HASHSETVALIDPTR) {
			if(c->key == key) return iterator(c, true);
			c = c->next;
			};
		return end();
		};

	HashSetTableList<KeyType> *CreateTable(size_t size) {
		HashSetTableList<KeyType> *p;
		size_t n;
		n = sizeof(HashSetTableList<KeyType>) + size * sizeof(HSNode<KeyType>) / sizeof(char);
		p = (HashSetTableList<KeyType> *)allocate.template allocate<char>(n);
		p->size = size;
		return p;
		};

#ifdef HASHSETDEBUG
public:
	mutable size_t collisions;
#endif

private:
	HSNode<KeyType> *m_table;
	size_t m_count;
	size_t m_size;
	HSNode<KeyType> *m_firstFree;
	HashSetTableList<KeyType> *m_tables;
	size_t m_overflowFirst;
	size_t m_overflowSize;
	HashFunction hashfn;
	Allocator allocate;
	};


template<class KeyType, class ValueType, class HashFunction, class Allocator>
class HashMap;

template<class KeyType, class ValueType, class HashFunction, class Allocator>
class BiDiHashMap;

template<class KeyType, class ValueType, class HashFunction, class Allocator>
class BiDiHashMap_const_iterator;


template<class KeyType, class ValueType>
class HashMapPair {
public:
	KeyType first;
	ValueType second;

	HashMapPair(): first(), second()						{ };
	HashMapPair(const HashMapPair<KeyType, ValueType> &p): first(p.first), second(p.second)	{ };
	HashMapPair(const KeyType &f, const ValueType &s): first(f), second(s)	{ };

	HashMapPair<KeyType, ValueType> &operator = (const HashMapPair<KeyType, ValueType> &p) {
		first = p.first;
		second = p.second;
		return *this;
		};

	bool operator ==(const HashMapPair<KeyType, ValueType> &other) const { return first == other.first; };
	bool operator !=(const HashMapPair<KeyType, ValueType> &other) const { return first != other.first; };
	};

/*
template<class KeyType, class ValueType>
HashMapPair<KeyType, ValueType> makeHashMapPair(const KeyType &f, const ValueType &s) {
	return HashMapPair<KeyType, ValueType>(f, s);
	};
*/

template<class HashFn, class K, class V>
class HashMapHashWrapper {
public:
	size_t operator ()(const HashMapPair<K, V> &key, size_t m) const {
		return hashfn(key.first, m);
		};
private:
	HashFn hashfn;
	};


/*
 * SetToMap adapter
 */
template<class KeyType,
	 class ValueType,
	 class PairType,
	 class SetType>
class SetToMap: public SetType {
public:
	typedef KeyType key_type;
	typedef ValueType data_type;
	typedef PairType value_type;
	typedef typename SetType::iterator iterator;
	typedef typename SetType::const_iterator const_iterator;

	SetToMap(): SetType(), m_defaultValue()				{ };
	SetToMap(ValueType defVal): SetType(), m_defaultValue(defVal)	{ };
	SetToMap(const SetToMap &m): SetType((const SetType &)m), m_defaultValue(m.m_defaultValue)	{ };

	~SetToMap()							{ };

	SetToMap &operator =(const SetToMap &t) {
		*(SetType *)this = (const SetType &)t;
		return *this;
		};

	ValueType &operator [](const KeyType &key) {
		std::pair<iterator, bool> res = insert(key, m_defaultValue);
		return const_cast<ValueType &>(res.first->second);
		};

	const ValueType &operator [](const KeyType &key) const {
		typename SetType::iterator it = find(PairType(key, m_defaultValue));
		if(it == this->end()) return m_defaultValue;
		return it->second;
		};


	std::pair<typename SetType::iterator, bool> insert(const KeyType &key, const ValueType &value)
				{ return SetType::insert(PairType(key, value)); };
	std::pair<typename SetType::iterator, bool> insert(const std::pair<KeyType, ValueType> &p)
				{ return SetType::insert(PairType(p.first, p.second)); };

	typename SetType::iterator find(const KeyType &key)		{ return SetType::find(PairType(key, m_defaultValue)); };
	typename SetType::const_iterator find(const KeyType &key) const	{ return SetType::find(PairType(key, m_defaultValue)); };

	void erase(const KeyType &k)		{ SetType::erase(find(k)); };
	void erase(const_iterator it)		{ SetType::erase(find(it->first)); };

protected:
	ValueType m_defaultValue;
	};


/*
 * HashMap
 */
template<typename KeyType,
	 typename ValueType,
	 class HashFunction = DefaultHashFun<KeyType>,
	 class Allocator = HashDefaultCPPAllocator>
class HashMap: public SetToMap<KeyType, ValueType,
			       HashMapPair<KeyType, ValueType>,
			       HashSet<HashMapPair<KeyType, ValueType>,
			       	       HashMapHashWrapper<HashFunction, KeyType, ValueType>,
				       Allocator>
			       > {
private:
	typedef SetToMap<KeyType, ValueType,
			 HashMapPair<KeyType, ValueType>,
			 HashSet<HashMapPair<KeyType, ValueType>,
			     	 HashMapHashWrapper<HashFunction, KeyType, ValueType>,
				 Allocator> > baseType;
public:
	HashMap(size_t size = 1021): baseType()				{ baseType::resize(size); };
	HashMap(size_t size, const ValueType &defVal): baseType(defVal) { baseType::resize(size); };
	HashMap(const HashMap &t): baseType((const baseType &)t)	{ };

	~HashMap()							{ };
	};


/*
 * BiDiHashMapPair
 */
template<class KeyType,
	 class ValueType>
class BiDiHashMapPair {
public:
	KeyType first;
	ValueType second;
	mutable const BiDiHashMapPair *prev;
	mutable const BiDiHashMapPair *next;

public:
	BiDiHashMapPair(): first(), second(), prev(NULL), next(NULL)	{ };
	BiDiHashMapPair(const KeyType &k, const ValueType &v): first(k), second(v), prev(NULL), next(NULL) 	{ };
	BiDiHashMapPair(const BiDiHashMapPair &k): first(k.first), second(k.second), prev(k.prev), next(k.next)	{ };

	BiDiHashMapPair &operator =(const BiDiHashMapPair &k) {
		first = k.first;
		second = k.second;
		prev = k.prev;
		next = k.next;
		if(prev) prev->next = this;
		if(next) next->prev = this;
		return *this;
		};

	bool operator ==(const BiDiHashMapPair &other) const	{ return first == other.first; };
	bool operator !=(const BiDiHashMapPair &other) const	{ return first != other.first; };

	};

template<class HashFn, class KeyType, class ValueType>
class BiDiHashMapHashWrapper {
public:
	size_t operator ()(const BiDiHashMapPair<KeyType, ValueType> &key, size_t m) const {
		return hashfn(key.first, m);
		};
private:
	HashFn hashfn;
	};

/*
 * BiDiHashMap_const_iterator
 */
template<class KeyType,
	 class ValueType,
	 class HashFunction,
	 class Allocator>
class BiDiHashMap_const_iterator: public std::bidirectional_iterator_tag {
private:
	typedef SetToMap<KeyType, ValueType,
			 BiDiHashMapPair<KeyType, ValueType>,
			 HashSet<BiDiHashMapPair<KeyType, ValueType>,
				 BiDiHashMapHashWrapper<HashFunction, KeyType, ValueType>,
				 Allocator> > mapBase;
public:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef BiDiHashMapPair<KeyType, ValueType> value_type;
	typedef ptrdiff_t difference_type;
	typedef const BiDiHashMapPair<KeyType, ValueType> *pointer;
	typedef BiDiHashMapPair<KeyType, ValueType> &reference;

	BiDiHashMap_const_iterator()					{ };
	BiDiHashMap_const_iterator(const BiDiHashMap_const_iterator &it){ *this = it; };
	BiDiHashMap_const_iterator(typename mapBase::const_iterator &it)	{ m_cur = &(*it); };

	~BiDiHashMap_const_iterator()					{ };

	reference operator *()				{ return *m_cur; };
	pointer operator ->()				{ return m_cur; };

	BiDiHashMap_const_iterator &operator++()	{ m_cur = m_cur->next; return *this; };
	BiDiHashMap_const_iterator operator++(int)	{ BiDiHashMap_const_iterator rv(*this); m_cur = m_cur->next; return rv; };
	BiDiHashMap_const_iterator &operator--()	{ m_cur = m_cur->prev; return *this; };
	BiDiHashMap_const_iterator operator--(int)	{ BiDiHashMap_const_iterator rv(*this); m_cur = m_cur->prev; return rv; };

	BiDiHashMap_const_iterator &operator =(const BiDiHashMap_const_iterator &it)	{ m_cur = it.m_cur; return *this; };
	BiDiHashMap_const_iterator &operator =(typename mapBase::const_iterator &it)	{ m_cur = &(*it); return *this; };

	bool operator ==(const BiDiHashMap_const_iterator<KeyType, ValueType, HashFunction, Allocator> &it)	{ return m_cur == it.m_cur; };
	bool operator !=(const BiDiHashMap_const_iterator<KeyType, ValueType, HashFunction, Allocator> &it)	{ return m_cur != it.m_cur; };
private:
	BiDiHashMap_const_iterator(const BiDiHashMapPair<KeyType, ValueType> *elem)	{ m_cur = elem; };

	const BiDiHashMapPair<KeyType, ValueType> *m_cur;
	friend class BiDiHashMap<KeyType, ValueType, HashFunction, Allocator>;
	};



/*
 * BiDiHashMap
 */
template<typename KeyType,
	 typename ValueType,
	 class HashFunction = DefaultHashFun<KeyType>,
	 class Allocator = HashDefaultCPPAllocator>
class BiDiHashMap: public SetToMap<KeyType, ValueType,
				   BiDiHashMapPair<KeyType, ValueType>,
				   HashSet<BiDiHashMapPair<KeyType, ValueType>,
				   	   BiDiHashMapHashWrapper<HashFunction, KeyType, ValueType>,
				           Allocator>
				   > {
private:
	typedef SetToMap<KeyType, ValueType,
			 BiDiHashMapPair<KeyType, ValueType>,
			 HashSet<BiDiHashMapPair<KeyType, ValueType>,
				 BiDiHashMapHashWrapper<HashFunction, KeyType, ValueType>,
			         Allocator> > baseType;

public:
	typedef BiDiHashMap_const_iterator<KeyType, ValueType, HashFunction, Allocator> iterator;
	typedef BiDiHashMap_const_iterator<KeyType, ValueType, HashFunction, Allocator> const_iterator;

	BiDiHashMap(size_t size = 1021): baseType()				{ baseType::resize(size); initialize(); };
	BiDiHashMap(size_t size, const ValueType &defVal): baseType(defVal)	{ baseType::resize(size); initialize(); };
	BiDiHashMap(const BiDiHashMap &t): baseType(t.m_defaultValue)		{ baseType::resize(t.slots()); initialize(); *this = t; };

	~BiDiHashMap()			{ };

	BiDiHashMap &operator =(const BiDiHashMap &t) {
		iterator it, e;
		clear();
		for(it = t.begin(), e = t.end(); it != e; ++it) insert(it->first, it->second);
		return *this;
		};

	template<class MapType>
	BiDiHashMap &operator =(const MapType &t) {
		typename MapType::iterator it, e;
		clear();
		for(it = t.begin(), e = t.end(); it != e; ++it) insert(it->first, it->second);
		return *this;
		};


	iterator begin()		{ return iterator(m_begin.next); };
	iterator end()			{ return iterator(&m_end); };
	const_iterator begin() const	{ return const_iterator(m_begin.next); };
	const_iterator end() const	{ return const_iterator(&m_end); };

	ValueType &operator [](const KeyType &key) {
		std::pair<iterator, bool> res = insert(key, baseType::m_defaultValue);
		return const_cast<ValueType &>(res.first->second);
		};

	const ValueType &operator [](const KeyType &key) const {
		typename baseType::iterator it = find(PairType(key, baseType::m_defaultValue));
		if(it == this->end()) return baseType::m_defaultValue;
		return it->second;
		};

	std::pair<iterator, bool> insert(const KeyType &key, const ValueType &value) {
		std::pair<typename baseType::iterator, bool> res = baseType::insert(key, value);
		if(res.second) AddToList(res.first.operator ->());
		return std::make_pair(iterator(res.first), res.second);
		};

	std::pair<iterator, bool> insert(const std::pair<KeyType, ValueType> &elem) {
		std::pair<typename baseType::iterator, bool> res = baseType::insert(elem);
		if(res.second) AddToList(res.first.operator ->());
		return std::make_pair(iterator(res.first), res.second);
		};

	const_iterator find(const KeyType &key) const {
		typename baseType::const_iterator fnd = baseType::find(key);
		if(fnd == baseType::end()) return end();
		return const_iterator(fnd);
		};


	void resize(size_t size)	{ baseType::resize(size); initialize(); };
	void clear()			{ baseType::clear(); initialize(); };

	void erase(const KeyType &key)	{ erase(find(key)); };

	void erase(const_iterator pos) {
		DelFromList(pos.operator ->());
		baseType::erase(pos->first);
		};

private:
	void initialize() {
		m_begin.prev = &m_begin;
		m_begin.next = &m_end;
		m_end.prev = &m_begin;
		m_end.next = &m_end;
		};

	void AddToList(const BiDiHashMapPair<KeyType, ValueType> *ptr) {
		ptr->prev = &m_begin;
		ptr->next = m_begin.next;
		m_begin.next = ptr;
		ptr->next->prev = ptr;
		};

	void DelFromList(const BiDiHashMapPair<KeyType, ValueType> *ptr) {
		ptr->prev->next = ptr->next;
		ptr->next->prev = ptr->prev;
		};

private:
	BiDiHashMapPair<KeyType, ValueType> m_begin, m_end;
	};


template <class T> class AssocTabInterface;

template< class K, class V> class AssocTabInterface< BiDiHashMap< K,V > >
{
    public:
        AssocTabInterface( BiDiHashMap< K,V > &acont ): cont( acont ) {}

        typedef K KeyType;
        typedef V ValType;

        bool hasKey( K arg ) { return cont.find( arg ) != cont.end(); }
        bool delKey( K );

        K firstKey();
        K lastKey();
        K prevKey( K );
        K nextKey( K );

        V &operator[]( K arg ) { return cont[arg]; }
        unsigned size() { return cont.size(); }
        bool empty() { return this->size() == 0; }
        void clear() { cont.clear(); }
        template< class Iterator > int getKeys( Iterator );

        int capacity () { return std::numeric_limits<int>::max(); }
        void reserve(int) {  }


        BiDiHashMap< K,V > &cont;
} ;

template< class K, class V >
bool AssocTabInterface< BiDiHashMap< K,V > >::delKey( K arg )
{
    typename BiDiHashMap< K,V >::iterator pos = cont.find( arg );
    if (pos == cont.end()) return false;
    cont.erase( pos );
    return true;
}

template< class K, class V >
K AssocTabInterface< BiDiHashMap< K,V > >::firstKey()
{
    if (cont.begin() == cont.end()) return (K)0;
    return cont.begin()->first;
}

template< class K, class V >
K AssocTabInterface< BiDiHashMap< K,V > >::lastKey()
{
    typename BiDiHashMap< K,V >::iterator pos;
    if (cont.begin() == (pos = cont.end())) return (K)0;
    pos--;
    return pos->first;
}

template< class K, class V >
K AssocTabInterface< BiDiHashMap< K,V > >::prevKey( K arg )
{
    if (!arg) return lastKey();
    typename BiDiHashMap< K,V >::iterator pos = cont.find( arg );
    assert( pos != cont.end() );
    if (pos == cont.begin()) return (K)0;
    pos--;
    return pos->first;
}

template< class K, class V >
K AssocTabInterface< BiDiHashMap< K,V > >::nextKey( K arg )
{
    if (!arg) return firstKey();
    typename BiDiHashMap< K,V >::iterator pos = cont.find( arg );
    assert( pos != cont.end() );
    pos++;
    if (pos == cont.end()) return (K)0;
    return pos->first;
}

template< class K, class V > template <class Iterator>
int AssocTabInterface< BiDiHashMap< K,V > >::getKeys( Iterator iter )
{
    for( K key = firstKey(); key; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}

}

#endif
