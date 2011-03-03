#ifndef KOALA_JOINABLE_SETS_H
#define KOALA_JOINABLE_SETS_H

#include<map>
#include<cassert>
#include "Set.h"
#include "localarray.h"


namespace Koala {

template <class Klucz>
struct JSPartDesrc {
    JSPartDesrc *parent,*next,*first,*last;
    unsigned int deg,size;
    Klucz key;
};

template<class ITEM, class AssocContainer=AssocArray<ITEM, JSPartDesrc<ITEM> *> >
class JoinableSets {
protected:

	AssocContainer mapa;
	JSPartDesrc<ITEM> *bufor;
	size_t siz,part_no,maxsize;


public:
	typedef JSPartDesrc<ITEM>* Repr;
	typedef ITEM ElemType;

	JoinableSets(unsigned int n = 0): mapa()	{ siz = 0; bufor = NULL; resize(n); };
	~JoinableSets()					{ resize(0); };


	JoinableSets(const JoinableSets<ITEM,AssocContainer> &s)
            : mapa(), siz(s.siz), part_no(s.part_no), maxsize(s.maxsize)
    {   if (!maxsize)  bufor=0;
        else bufor = new JSPartDesrc<ITEM> [maxsize];
        for(int i=0;i<siz;i++)
        {   bufor[i].deg=s.bufor[i].deg; bufor[i].size=s.bufor[i].size; bufor[i].key=s.bufor[i].key;
            mapa[bufor[i].key]=bufor+i;
            bufor[i].parent=bufor+(s.bufor[i].parent-s.bufor);
            bufor[i].first=bufor+(s.bufor[i].first-s.bufor);
            bufor[i].last=bufor+(s.bufor[i].last-s.bufor);
            bufor[i].next=(s.bufor[i].next) ? bufor+(s.bufor[i].next-s.bufor) : 0;
        }

    };

	JoinableSets &operator =(const JoinableSets<ITEM,AssocContainer> &s)
	{   if (&s==this) return *this;
        resize(s.maxsize);
        mapa.clear();siz=s.siz; part_no=s.part_no;
        for(int i=0;i<siz;i++)
        {   bufor[i].deg=s.bufor[i].deg; bufor[i].size=s.bufor[i].size; bufor[i].key=s.bufor[i].key;
            mapa[bufor[i].key]=bufor+i;
            bufor[i].parent=bufor+(s.bufor[i].parent-s.bufor);
            bufor[i].first=bufor+(s.bufor[i].first-s.bufor);
            bufor[i].last=bufor+(s.bufor[i].last-s.bufor);
            bufor[i].next=(s.bufor[i].next) ? bufor+(s.bufor[i].next-s.bufor) : 0;
        }
		return *this;
	};


	void resize(unsigned int n) {
    // niszczy zawartosc
		delete bufor;
		mapa.clear();
		siz = part_no=0;
		if(n == 0) {
			bufor = NULL;
			maxsize=0;
		} else {
			bufor = new JSPartDesrc<ITEM> [n];
			maxsize=n;
        };
	};

    int size() { return siz; }
    bool empty() { return siz==0; }
    bool operator!() { return empty(); }
    void clear() { resize(0); }
    int getSetNo() { return part_no; }
    template <class Iter>
            int getElements(Iter iter) { mapa.getKeys(iter); return siz; }

    template <class Iter>
            int getSetIds(Iter iter)
            { for(int i=0;i<siz;i++)
                if (bufor[i].parent==bufor+i)
                { *iter=bufor+i; ++iter; };
                return part_no;
            }

    template <class Iter>
            int getSet(typename JoinableSets<ITEM>::Repr s, Iter iter)
            {   if (!s) return 0;
                s=getSetId(s);
                for(typename JoinableSets<ITEM>::Repr p=s->first;p;p=p->next)
                    { *iter=p->key; ++iter; }
                return s->size;
            }

    template <class Iter>
            int getSet(const ITEM &i, Iter iter)
            {
                return getSet(getSetId(i),iter);
            }

    int size(typename JoinableSets<ITEM>::Repr s)
    {   if (!s) return 0;
        return getSetId(s)->size;
    }

    int size(const ITEM &i)
    {   typename JoinableSets<ITEM>::Repr s=getSetId(i);
        return (s) ? s->size : 0;
    }

	typename JoinableSets<ITEM>::Repr makeSinglet(const ITEM &i) {
		if (mapa.hasKey(i)) return 0;
		assert(siz<maxsize);
		typename JoinableSets<ITEM>::Repr r = bufor + siz++;
		r->first=r->last=r->parent=r; r->next=0; r->deg=0; r->size=1; r->key=i;
		mapa[i] = r;
		part_no++;
		return r;
	};

	typename JoinableSets<ITEM>::Repr getSetId(const ITEM &i) {
		if (!mapa.hasKey(i)) return 0;
		return getSetId(mapa[i]);
	};

	typename JoinableSets<ITEM>::Repr getSetId(typename JoinableSets<ITEM>::Repr s) {
	    if (!s) return 0;
		typename JoinableSets<ITEM>::Repr p;
		p = s->parent;
		if(p == s) return p;
		return s->parent = getSetId(s->parent);
	};

	typename JoinableSets<ITEM>::Repr join(typename JoinableSets<ITEM>::Repr a, typename JoinableSets<ITEM>::Repr b) {
		if (!a || !b) return 0;
		typename JoinableSets<ITEM>::Repr res;
		a = getSetId(a);
		b = getSetId(b);
		if (a==b) return 0;
		part_no--;
		if(a->deg < b->deg) res= a->parent = b;
		else {
			res = b->parent = a;
			if(a->deg == b->deg) a->deg += 1;
        };
        res->size=a->size+b->size;
        a->last->next=b->first;
        res->first=a->first; res->last=b->last;
        return res;
	};

	typename JoinableSets<ITEM>::Repr join(const ITEM &a, const ITEM &b) {
		if (!mapa.hasKey(a) || !mapa.hasKey(b)) return 0;
		return join(mapa[a], mapa[b]);
	};

	typename JoinableSets<ITEM>::Repr join(typename JoinableSets<ITEM>::Repr a, const ITEM &b) {
		if (!mapa.hasKey(b) || !a) return 0;
		return join(a, mapa[b]);
	};

	typename JoinableSets<ITEM>::Repr join(const ITEM &a, typename JoinableSets<ITEM>::Repr b) {
		if (!mapa.hasKey(a) || !b) return 0;
		return join(mapa[a], b);
    };

};


template< typename Element, typename Cont >
    std::ostream& operator<<(std::ostream& is,JoinableSets<Element,Cont> &s)
    {   is << "{";
        int l=s.getSetNo();
        JSPartDesrc<Element>* LOCALARRAY(tab,l);
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
