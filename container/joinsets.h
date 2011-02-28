#ifndef KOALA_JOINABLE_SETS_H
#define KOALA_JOINABLE_SETS_H

#include<map>
#include<cassert>
#include "Set.h"
#include "localarray.h"


namespace Koala {


struct JSPartDesrc {
    JSPartDesrc *parent;
    unsigned int deg;
    JSPartDesrc()		{ parent = this; deg = 0; };
};

template<class ITEM, class AssocContainer=AssocArray<ITEM, JSPartDesrc *> >
class JoinableSets {
protected:

	AssocContainer m_map;
	JSPartDesrc *m_nodes;
	size_t m_dp,part_no,maxsize;


public:
	typedef JSPartDesrc *Repr;
	typedef ITEM ElemType;

	JoinableSets(unsigned int n = 0): m_map()	{ m_dp = 0; m_nodes = NULL; resize(n); };
	~JoinableSets()					{ resize(0); };

// TODO:
//	JoinableSets(const JoinableSets<ITEM> &s)	{ *this = s; };
//	JoinableSets &operator =(const JoinableSets<ITEM> &s) {
//
//		return *this;
//	};


	bool resize(unsigned int n) {
    // niszczy zawartosc
		delete m_nodes;
		m_map.clear();
		m_dp = part_no=0;
		if(n == 0) {
			m_nodes = NULL;
			maxsize=0;
			return true;
		} else {
			m_nodes = new JSPartDesrc [n];
			maxsize=n;
			return m_nodes != NULL;
        };
	};

    int size() { return m_dp; }
    bool empty() { return m_dp==0; }
    bool operator!() { return empty(); }
    void clear() { resize(0); }
    int getSetNo() { return part_no; }
    template <class Iter>
            int getElements(Iter iter) { m_map.getKeys(iter); return size(); }

    template <class Iter>
            int getSetIds(Iter iter)
            { for(int i=0;i<m_dp;i++)
                if (m_nodes[i].parent==m_nodes+i)
                { *iter=m_nodes+i; ++iter; };
                return part_no;
            }

    //TODO: to sie powinno dziac w czasie liniowym od rozmiaru partycji, a nie calego zbioru
    template <class Iter>
            int getSet(typename JoinableSets<ITEM>::Repr s, Iter iter)
            {   assert(s);
                s=getSetId(s);
                int siz=m_map.size(),licz=0;
                typename AssocContainer::KeyType key=m_map.firstKey();
                for(;siz;siz--)
                {   if (getSetId(m_map[key])==s) { licz++; *iter=key; ++iter; }
                    if (key!=m_map.lastKey()) key=m_map.nextKey(key);
                }
                return licz;
            }

    template <class Iter>
            int getSet(const ITEM &i, Iter iter)
            {
                return getSet(getSetId(i),iter);
            }


	typename JoinableSets<ITEM>::Repr makeSinglet(const ITEM &i) {
		if (m_map.hasKey(i)) return 0;
		assert(m_dp<maxsize);
		typename JoinableSets<ITEM>::Repr r = m_nodes + m_dp++;
		m_map[i] = r;
		part_no++;
		return r;
	};

	typename JoinableSets<ITEM>::Repr getSetId(const ITEM &i) {
		if (!m_map.hasKey(i)) return 0;
		return getSetId(m_map[i]);
	};

	typename JoinableSets<ITEM>::Repr getSetId(typename JoinableSets<ITEM>::Repr s) {
	    assert(s);
		typename JoinableSets<ITEM>::Repr p;
		p = s->parent;
		if(p == s) return p;
		return s->parent = getSetId(s->parent);
	};

	typename JoinableSets<ITEM>::Repr join(typename JoinableSets<ITEM>::Repr a, typename JoinableSets<ITEM>::Repr b) {
		a = getSetId(a);
		b = getSetId(b);
		if (a==b) return 0;
		part_no--;
		if(a->deg < b->deg) return a->parent = b;
		else {
			b->parent = a;
			if(a->deg == b->deg) a->deg += 1;
			return a;
        };
	};

	typename JoinableSets<ITEM>::Repr join(const ITEM &a, const ITEM &b) {
		if (!m_map.hasKey(a) || !m_map.hasKey(b)) return 0;
		return join(m_map[a], m_map[b]);
	};

	typename JoinableSets<ITEM>::Repr join(typename JoinableSets<ITEM>::Repr a, const ITEM &b) {
		if (!m_map.hasKey(b)) return 0;
		return join(a, m_map[b]);
	};

	typename JoinableSets<ITEM>::Repr join(const ITEM &a, typename JoinableSets<ITEM>::Repr b) {
		if (!m_map.hasKey(a)) return 0;
		return join(m_map[a], b);
    };

};


template< typename Element, typename Cont >
    std::ostream& operator<<(std::ostream& is,JoinableSets<Element,Cont> &s)
    {   is << "{";
        int l=s.getSetNo();
        JSPartDesrc* LOCALARRAY(tab,l);
        s.getSetIds(tab);
        for(int i=0;i<l;i++)
        {   Set<Element> zb;
            s.getSet(tab[i],setInserter(zb));
            is<<zb;
            if (i<l-1) is << ',';
        }
        is<<"}";
        return is;
    }



} // namespace Koala

#endif
