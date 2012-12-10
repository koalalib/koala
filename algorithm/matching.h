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
		static void Write(BlackHole &c, V u, V v, E e)
			{};
	};

};

/* MatchingPar
 * algorytm szukajacy najliczniejszego skojarzenia w grafie dowolnym
 */
/** \brief Matching (parametrized).
 *
 *  The set of structures and methods allowing to find maximal matching in graph.
 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
 *  \ingroup DMmatch
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

		CyclicList(Allocator& alloc=0):  allocator(&alloc), m_cur(0)
			{};
		~CyclicList()
			{ erase(); };
		T &curr()
			{ return m_cur->elem; };

		iterator cur()
			{ return iterator(m_cur); };

		void clear()
			{ while(m_cur != 0) erase(); };
		void next()
			{ m_cur = m_cur->next; };
		void prev()
			{ m_cur = m_cur->prev; };

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
			{
				this->QueueInterface< std::pair<T, bool> * >::push(std::pair<T, bool>(v,b));
			}
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
		/** \brief Auxiliary matching structure for vertices. */
		template< class GraphType > struct VertLabs
		{
				typename GraphType::PVertex vMatch;/**< \brief Matched vertex. */
				typename GraphType::PEdge eMatch;/**< \brief Matched edge.*/
				VertLabs(): vMatch( 0 ), eMatch( 0 )
					{}
		};

	/** \brief Find maximum matching.
	 *
	 *  The method finds maximum matching in a given \a g.
	 *  @param[in] g the considered graph.
	 *  @param[out] matching the list of edges in found matching.
	 *  @return the actual number of edges in found matching.
	 *
	 *  [See example](examples/matching/matching_findMax.html).
	 */
	template< class GraphType, class EIterOut >
	static int findMax( const GraphType &g, EIterOut matching)
	{
		return findMax( g,blackHole,matching );
	}

	/** \brief Find maximum matching .
	 *
	 *  The method finds maximum matching in a given \a g.
	 *  @param[in] g the considered graph.
	 *  @param[out] vertTab an associative container from PVertex to VertLabs, which keeps matched edges and vertices (BlackHole possible).
	 *  @param[out] matching the list of edges in found matching.
	 *  @param[in] matchSize the desired size of a matching, leave out or -1 for a maximum
	 *  @return the actual number of edges in found matching
	 *
	 *  [See example](examples/matching/matching_findMax.html).
	 */
	template< class GraphType, class VertContainer, class EIterOut >
	static int findMax( const GraphType &g,
				VertContainer &vertTab,
				EIterOut matching,
				int matchSize = -1 )
	{
		typename GraphType::PEdge edges[1];
		return matchingTool(g, vertTab, edges, edges, matching, matchSize, false);
	}

	/** \brief Find maximum matching.
	 *
	 *  The method finds maximum matching in a given \a g. The initial matching is passed to function via iterators \a initialBegin and \a initialEnd
	 *  \param[in] g the considered graph.
	 *  \param[out] vertTab an associative container from PVertex to VertLabs which keeps matched edges and vertices (BlackHole possible).
	 *  @param[in] initialBegin the iterator to the beginning of given initial matching  (list of edges).
	 *  @param[in] initialEnd the iterator to the past-the-end element of given initial matching  (list of edges).
	 *  @param[out] matching the list of edges in found matching.
	 *  @param[in] matchSize the desired size of a matching, leave out or set to -1 for a maximum.
	 *  @return  the actual number of edges in found matching. Additionally, the method can return -1 if initial matching is invalid.
	 *
	 *  [See example](examples/matching/matching_findMax.html).
	 */
	template< class GraphType, class VertContainer, class EIterIn, class EIterOut >
	static int findMax( const GraphType &g,
				VertContainer &vertTab,
				EIterIn initialBegin,
				EIterIn initialEnd,
				EIterOut matching, int matchSize = -1 )
	{
		return matchingTool(g, vertTab, initialBegin, initialEnd, matching, matchSize, false);
	}

	/** \brief Find minimal edge cover.
	 *
	 *  The method finds a minimal edge cover of a given \a g.
	 *  @param[in] g the considered graph.
	 *  @param[out] cover the list of edges in found cover.
	 *  @return the actual number of edges in found cover.
	 *
	 *  [See example](examples/matching/matching_minEdgeCover.html).
	 */
	template< class GraphType, class EIterOut >
	static int minEdgeCover( const GraphType &g, EIterOut cover)
	{
		typename GraphType::PEdge edges[1];
		return matchingTool(g, blackHole, edges, edges, cover, -1, true);
	}

	/** \brief Greedy matching.
	 *
	 *  Fast but inaccurate procedure searches greedily for a maximum (in the sense of inclusion) matching.
	 *  \param[in] g the considered graph.
	 *  \param[out] avertTab an associative container from PVertex to VertLabs which keeps matched edges and vertices (BlackHole possible).
	 *  @param[out] edgeIterOut the list of edges in found matching.
	 *  @param[in] matchSize the desired size of a matching, leave out or -1 for a maximum.
	 *  @return  the size of found matching.
	 *
	 *  [See example](examples/matching/matching_greedy.html).
	 */
	template< class GraphType, class VertContainer, class EIterOut > static int greedy( const GraphType &g,
		VertContainer &avertTab, EIterOut edgeIterOut, int matchSize = -1 );
	// wlasciwa procedura - zachlannie szuka skojarzenia, rozwarzajac krawedzie podane na wejsciu
	/**\brief Greedy matching.
	 *
	 *  The method searches greedily for a matching in the graph \a g. The edges are taken form the sequence given by iterators \a edgeIterInBegin and \a edgesiIterInEnd.
	 *  \param[in] g the considered graph.
	 *  \param[out] avertTab an associative container from PVertex to VertLabs which keeps matched edges and vertices (BlackHole possible).
	 *  \param edgeIterInBegin the iterator to the first element of the edge sequence used by the greedy algorithm.
	 *  \param edgeIterInEnd the iterator to the past-the-end element of the edge sequence used by the greedy algorithm.
	 *  @param[out] edgeIterOut the list of edges in found matching.
	 *  @param[in] matchSize the desired size of a matching, leave out or set to -1 for a maximum.
	 *  @return  the size of found matching.
	 *
	 *  [See example](examples/matching/matching_greedy.html).
	 */
	template< class GraphType, class VertContainer, class EIterIn, class EIterOut > static int greedy(
		const GraphType &g, VertContainer &avertTab, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd,
		EIterOut edgeIterOut, int matchSize = -1 );

	// wlasciwa procedura - testuje czy podane na wejsciu krawedzie tworza skojarzenie
	/** \brief Test if matching.
	 *
	 *  The method tests if the given set of edges is a matching in a graph.
	 *  \param g the considered graph.
	 *  \param edgeIterInBegin the iterator to the first element of the container with the edges of tested set.
	 *  \param edgeIterInEnd the iterator to the past-the-end element of the container with the edges of tested set.
	 *  \return true if the edge set form a matching, false otherwise.
	 *
	 *  [See example](examples/matching/matching_test.html).
	 */
	template< class GraphType, class EIterIn > static bool test( const GraphType &g, EIterIn edgeIterInBegin,
		EIterIn edgeIterInEnd );
};
	/* Matching
	 *
	 */
	/** \brief Matching (default).
	 *
	 *  The set of structures and methods allowing to find maximal matching in graph
	 *  (DefaultStructs = AlgsDefaultSettings).
	 *  \ingroup DMmatch	 */
	class Matching: public MatchingPar< AlgsDefaultSettings > { };

	/* StableMatchingPar
	 * algorytmy dla problemu stabilnego skojarzenia
	 */
	/** \brief Stable matching (parametrized).
	 *
	 *  The set of structures and methods allowing to find stable maximal matching in graph.
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *  \ingroup DMmatch
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
		/** \brief Auxiliary matching structure for vertices. */
		template< class GraphType > struct VertLabs
		{
			//wierzcholek skojarzany lub 0 w razie braku
			typename GraphType::PVertex vMatch;/**< \brief Matched vertex. */
			// i krawedz skojarzenia
			typename GraphType::PEdge eMatch;/**< \brief Matched edge.*/

			/**\brief Constructor*/
			VertLabs( typename GraphType::PVertex v = 0, typename GraphType::PEdge e = 0 ): vMatch( v ), eMatch( e )
				{ }

			template <class T> void copy(T& arg) const
			{
				arg.vMatch=vMatch;
				arg.eMatch=eMatch;
			}

			void copy(BlackHole&) const
				{ }

		};

		// wlasciwa procedura - testuje czy podane na wejsciu krawedzie tworza skojarzenie stabilne
		// wynik.first- odpowiedz. Jesli jest to matching ale istnieje krawedz rozbijajaca, zostaje ona zwrocona w .second
		// compare, funktor podajacy preferencje wierzcholkow odnosnie ich krawedzi, wolanie
		// bool compare(v,e1,e2) jest true jesli e2 jest lepsze od e1 z punktu widzenia ich koncowki v
		/** \brief Test if stable matching.
		 *
		 *  The method tests if the given set of edges is a stable matching in a graph.
		 *  \param g the considered graph.
		 *  \param compare the object function compares edges and gets preferable edge from the vertex point of view. <tt> bool compare(v,e1,e2)</tt> returns true if \p e2 is better then \p e1 looking from \p v.  
		 *  \param edgeIterInBegin the iterator to the first element of the container with the edges of tested set.
		 *  \param edgeIterInEnd the iterator to the past-the-end element of the container with the edges of tested set.
		 *  \return the standard pair consisting of the bool value (pair true if the edge set form a stable matching, false otherwise) and the breaking edge.
		 */	
		template< class GraphType, class EIterIn, class Comp > static std::pair< bool,typename GraphType::PEdge >
			test( const GraphType &g, Comp compare, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd );
		//TODO: niepewny, przetestowac poprawnosc na duzych przykladach
		/** \brief Find stable matching in bipartite graph.
		 *
		 *  The method finds the stable matching in bipartite graph.
		 *  \param g the considered graph.
		 *  \param begin the iterator to the first element of the container with vertexes.
		 *  \param end the iterator to the past-the-end element of the container with vertexes.
		 *  \param compare the object function compares edges and gets preferable edge from the vertex point of view. <tt> bool compare(v,e1,e2)</tt> returns true if \p e2 is better then \p e1 looking from \p v.  
		 *  \param verttab an associative container from PVertex to VertLabs, which keeps matched edges and vertices (BlackHole possible).
		 *  \param out  the list of edges in found matching.
		 *  \return the number of edges in found strong matching.
		 */
		template< class GraphType, class VIterIn, class Comp, class vertCont, class EIterOut >
			static int bipartFind( const GraphType &g, VIterIn begin, VIterIn end, Comp compare, vertCont &verttab,
				EIterOut out );
	};

	/* StableMatching
	 *
	 */
	/** \brief Stable matching (default).
	 *
	 *  The set of structures and methods allowing to find maximal matching in bipartite graph
	 *  (DefaultStructs = AlgsDefaultSettings).
	 *  \ingroup DMmatch	 */
	class StableMatching: public StableMatchingPar< AlgsDefaultSettings > { };

#include "matching.hpp"
}

#endif