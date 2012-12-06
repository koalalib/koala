#ifndef KOALA_DEF_FACTOR_H
#define KOALA_DEF_FACTOR_H

#include "../base/defs.h"
#include "../container/privates.h"
#include "../container/joinsets.h"
#include "../graph/view.h"


namespace Koala
{
namespace Privates {

    template<class Cont>
    class MatchingBlackHoleWriter {
    public:
        template<class V, class E>
        static void Write(Cont &vertTab, V u, V v, E e) {
            vertTab[u].vMatch = v;
            vertTab[u].eMatch = e;
            vertTab[v].vMatch = u;
            vertTab[v].eMatch = e;
            };
    };

    template<>
    class MatchingBlackHoleWriter<BlackHole> {
    public:
        template<class V, class E>
        static void Write(BlackHole &c, V u, V v, E e) {};
    };

};

    /* MatchingPar
     * algorytm szukajacy najliczniejszego skojarzenia w grafie dowolnym
     */
template< class DefaultStructs > class MatchingPar
{
      private:

        template<class GraphType>
        struct PrvVertLabs {
            typename GraphType::PVertex labS;
            typename GraphType::PVertex labT1;
            typename GraphType::PVertex labT2;
            typename GraphType::PVertex mate;
            bool labTB;
        };

        template<class GraphType>
        class MatchingData: public DefaultStructs::template
                AssocCont< typename GraphType::PVertex, PrvVertLabs<GraphType> >::Type {};

        template<class T>
        struct Node {
                Node *next;
                Node *prev;
                T elem;
        };

        template<class T, class Allocator = Privates::DefaultCPPAllocator>
        class CyclicList {
        public:

            class iterator {
                public:
                    iterator(): m_ptr(0)		{};
                    iterator(Node<T> *n): m_ptr(n)	{};
                    iterator next()			{ return iterator(m_ptr->next); };
                    iterator prev()			{ return iterator(m_ptr->prev); };
                    void moveNext()			{ m_ptr = m_ptr->next; };
                    void movePrev()			{ m_ptr = m_ptr->prev; };
                    T &operator *()			{ return m_ptr->elem; };
                    T *operator ->()		{ return &(m_ptr->elem); };
                    bool operator ==(const iterator &i)	{ return m_ptr == i.m_ptr; };
                    bool operator !=(const iterator &i)	{ return m_ptr != i.m_ptr; };
                private:
                    Node<T> *m_ptr;
            };

            CyclicList(Allocator& alloc=0): m_cur(0), allocator(&alloc) {};
            ~CyclicList()	{ erase(); };
            T &curr()	{ return m_cur->elem; };

            iterator cur()	{ return iterator(m_cur); };

            void clear()	{ while(m_cur != 0) erase(); };
            void next()	{ m_cur = m_cur->next; };
            void prev()	{ m_cur = m_cur->prev; };

            void add_before(const T &v) {
                //Node<T> *n = new Node<T>;
                Node<T> *n = allocator->template allocate<Node<T> >();
                n->elem = v;
                if(m_cur == 0) {
                    n->prev = n;
                    n->next = n;
                    m_cur = n;
                    return;
                } else {
                    n->prev = m_cur->prev;
                    n->next = m_cur;
                    n->next->prev = n;
                    n->prev->next = n;
                    };
            };

            void erase() {
                Node<T> *t;
                if(m_cur == 0) return;
                t = m_cur->next;
                if(t == m_cur) //{ delete m_cur; t = 0; }
                    { allocator-> deallocate( m_cur); t = 0; }
                else {
                    m_cur->prev->next = m_cur->next;
                    m_cur->next->prev = m_cur->prev;
                    //delete m_cur;
                    allocator-> deallocate( m_cur);
                    };
                m_cur = t;
            };

            void Conc(CyclicList &l) {
                Node<T> *e, *ee, *p2, *p3, *p4, *e0;
                e = m_cur->prev;
                ee = l.m_cur->prev;
                e0 = ee;
                p2 = l.m_cur->next;
                p3 = p2->next;
                m_cur->prev = p2;
                p2->next = m_cur;
                while(p3 != l.m_cur) {
                    p2->prev = p3; p4 = p3->next; p3->next = p2;
                    p2 = p2;
                    p3 = p4;
                    };
                ee->prev = e;
                e->next = ee;
                m_cur = e0;
                l.m_cur = 0;
            };


            Allocator* allocator;

        private:
            Node<T> *m_cur;

        };


        template<class T>
        class SimpleQueue : public QueueInterface< std::pair<T, bool> * >
        {
            public:

                SimpleQueue(std::pair<T, bool> *p, int size) : QueueInterface< std::pair<T, bool> * > (p,size)
                    {}

                void push(const T &v, bool b)
                { this->QueueInterface< std::pair<T, bool> * >::push(std::pair<T, bool>(v,b)); }
        };

	template<class GraphType,class CList>
	static void BackRec(MatchingData<GraphType> &data,
			    typename GraphType::PVertex &vert,
			    bool &st,
			    CList &path);

	template<class GraphType,class CList>
	static void BackT(MatchingData<GraphType> &data,
			  typename GraphType::PVertex &vert,
			  bool &st,
			  CList &path);

	template<class GraphType, class CList>
	static typename GraphType::PVertex Backtracking(MatchingData<GraphType> &data,
							typename GraphType::PVertex vert,
							bool st,
							CList &path);

	template<class GraphType,class CList>
	static void Augmentation(MatchingData<GraphType> &data,
				 JoinableSets<typename GraphType::PVertex> &sets,
				 CList &pathl,
				 CList &pathr,
				 bool &noaugment,
				 int &expo);

	template<class GraphType, class CList>
	static void Relabel(MatchingData<GraphType> &data,
			    JoinableSets<typename GraphType::PVertex> &sets,
			    typename CList::iterator start,
			    CList &path,
			    SimpleQueue<typename GraphType::PVertex> &q,
			    CList &otherPath);

	template<class PVERT,class CListIterator>
	static void BaseChange(JoinableSets<PVERT> &sets,
			       typename JoinableSets<PVERT>::Repr &base,
			       CListIterator e1,
			       CListIterator e2);

	template<class GraphType, class CList>
	static void Blossoming(MatchingData<GraphType> &data,
			       JoinableSets<typename GraphType::PVertex> &sets,
			       CList &pathl,
			       CList &pathr,
			       SimpleQueue<typename GraphType::PVertex> &q);

    template< class GraphType, class VertContainer, class EIterIn, class EIterOut >
	static int matchingTool( const GraphType &g,
				 VertContainer &vertTab,
				 EIterIn initialBegin,
				 EIterIn initialEnd,
				 EIterOut matching,
				 int matchSize = -1,
				 bool makeCover = false);

      public:

        template< class GraphType > struct VertLabs
        {
                typename GraphType::PVertex vMatch;
                typename GraphType::PEdge eMatch;
                VertLabs(): vMatch( 0 ), eMatch( 0 ) {}
        };

	/** find maximum matching in a given graph
	 * @param[in] graph
	 * @param[out] matching list of edges in found matching
	 * @return actual number of edges in found matching */
        template< class GraphType, class EIterOut >
	static int findMax( const GraphType &g, EIterOut matching)
		{ return findMax( g,blackHole,matching ); }

	/** find maximum matching in a given graph
	 * @param[in] graph
	 * @param[out] map from PVertex to VertLabs or BlackHole
	 * @param[out] matching list of edges in found matching
	 * @param[in] matchSize desired size of a matching, leave out for a maximum
	 * @return actual number of edges in found matching */
        template< class GraphType, class VertContainer, class EIterOut >
	static int findMax( const GraphType &g,
			    VertContainer &vertTab,
			    EIterOut matching,
			    int matchSize = -1 )
		{
		    typename GraphType::PEdge edges[1];
            return matchingTool(g, vertTab, edges, edges, matching, matchSize, false);
        }

	/**
	 * @param[in] initialBegin iterator to give an initial list of edges in a matching
	 * @param[in] initialEnd
	 * @return additionally, can return -1 if initial matching is invalid */
        template< class GraphType, class VertContainer, class EIterIn, class EIterOut >
	static int findMax( const GraphType &g,
			    VertContainer &vertTab,
			    EIterIn initialBegin,
			    EIterIn initialEnd,
			    EIterOut matching, int matchSize = -1 )
		{ return matchingTool(g, vertTab, initialBegin, initialEnd, matching, matchSize, false); }

	/** find minimim edge cover of a given graph
	 * @param[in] graph
	 * @param[out] cover list of edges in found cover
	 * @return actual number of edges in found cover */
        template< class GraphType, class EIterOut >
	static int minEdgeCover( const GraphType &g, EIterOut cover)
		{ typename GraphType::PEdge edges[1];
		  return matchingTool(g, blackHole, edges, edges, cover, -1, true);
        }

        template< class GraphType, class VertContainer, class EIterOut > static int greedy( const GraphType &g,
            VertContainer &avertTab, EIterOut edgeIterOut, int matchSize = -1 );
        // wlasciwa procedura - zachlannie szuka skojarzenia, rozwarzajac krawedzie podane na wejsciu
        template< class GraphType, class VertContainer, class EIterIn, class EIterOut > static int greedy(
            const GraphType &g, VertContainer &avertTab, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd,
            EIterOut edgeIterOut, int matchSize = -1 );
        // wlasciwa procedura - testuje czy podane na wejsciu krawedzie tworza skojarzenie
        template< class GraphType, class EIterIn > static bool test( const GraphType &g, EIterIn edgeIterInBegin,
            EIterIn edgeIterInEnd );
};
    /* Matching
     *
     */
    class Matching: public MatchingPar< AlgsDefaultSettings > { };

    /* StableMatchingPar
     * algorytmy dla problemu stabilnego skojarzenia
     */
    template< class DefaultStructs > class StableMatchingPar
    {
      protected:
        template< class Graph, class Cmp > struct SortCmp
        {
            mutable typename Graph::PVertex v;
            mutable Cmp comp;

            SortCmp( typename Graph::PVertex av, Cmp acmp ): v( av ), comp( acmp )
                { }
            template< class T > bool operator()( T e,T f ) const
                { return !comp( v,e,f ); }
        };

      public:
        template< class GraphType > struct VertLabs
        {
            //wierzcholek skojarzany lub 0 w razie braku
            typename GraphType::PVertex vMatch;
            // i krawedz skojarzenia
            typename GraphType::PEdge eMatch;

            VertLabs( typename GraphType::PVertex v = 0, typename GraphType::PEdge e = 0 ): vMatch( v ), eMatch( e )
                { }
        };

        // wlasciwa procedura - testuje czy podane na wejsciu krawedzie tworza skojarzenie stabilne
        // wynik.first- odpowiedz. Jesli jest to matching ale istnieje krawedz rozbijajaca, zostaje ona zwrocona w .second
        // compare, funktor podajacy preferencje wierzcholkow odnosnie ich krawedzi, wolanie
        // bool compare(v,e1,e2) jest true jesli e2 jest lepsze od e1 z punktu widzenia ich koncowki v
        template< class GraphType, class EIterIn, class Comp > static std::pair< bool,typename GraphType::PEdge >
            test( const GraphType &g, Comp compare, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd );
        template< class GraphType, class VIterIn, class Comp, class vertCont, class EIterOut >
            static int bipartFind( const GraphType &g, VIterIn begin, VIterIn end, Comp compare, vertCont &verttab,
                EIterOut out );
    };

    /* StableMatching
     *
     */
    class StableMatching: public StableMatchingPar< AlgsDefaultSettings > { };

#include "matching.hpp"
}

#endif
