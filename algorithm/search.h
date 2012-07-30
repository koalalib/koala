#ifndef KOALA_DEF_SEARCH_H
#define KOALA_DEF_SEARCH_H

/* ------------------------------------------------------------------------- *
 * search.h
 *
 * ------------------------------------------------------------------------- */

#include "../base/def_struct.h"
#include "../graph/view.h"


namespace Koala {

/* ------------------------------------------------------------------------- *
 * SearchStructs
 *
 * ------------------------------------------------------------------------- */


struct PathStructs {
    // Do odczytu sciezki miedzy wierzcholkiem a korzeniem, gdy droga wyznaczona jest w postaci
    // tablicy asocjacyjnej PVertex -> rekord z polami vPrev, ePrev (wierzcholek poprzedni i krawedz do niego).
    // Przydatne w roznych algorytmach najkrotszej sciezki
    // Uzytkownik podaje, pare iteratorow, gdzie wpisac wierzcholki i krawedzie najkrotszej sciezki
    template <class VIter, class EIter> struct OutPath {
            VIter vertIter;
            EIter edgeIter;

            OutPath(VIter av, EIter ei) : vertIter(av), edgeIter(ei) {}
        };

        // funkcja tworzaca, analogia make_pair
        // Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
    template <class VIter, class EIter>
    static OutPath<VIter,EIter> outPath(VIter av, EIter ei) { return OutPath<VIter,EIter>(av,ei); }

};


struct ShortPathStructs : public PathStructs {

    template <class GraphType, class VertContainer, class VIter, class EIter>
    static int // zwraca liczbe krawedzi sciezki
        getOutPath(const GraphType& g,
            const VertContainer& vertTab, // tablica asoc. przypisujaca wierzcholkom rekordy z ustawionymi
            // wskaznikami do poprzedniego wierzcholka (vPrev) i krawedzi don prowadzacej (ePrev, NULL dla korzenia)
            // W tej formie zwraca wynik wiele procedur tworzacych w grafie drzewo (las) skierowany do korzenia np. Dijkstra, BFS itp
            OutPath<VIter,EIter> iters, // miejsce gdzie sciezka zostanie zapisana
            typename GraphType::PVertex end, // sciezka prowadzi "pod prad" tj. od korzenia do tego wierzcholka
            typename GraphType::PVertex start=0) // ew. wczesniejszy punkt koncowy na sciezce miedzy end a korzeniem
        {   assert(end); // TODO: throw
            typename GraphType::PVertex u,v=vertTab[end].vPrev;
            typename GraphType::PEdge  e=vertTab[end].ePrev;
            typename GraphType::PVertex LOCALARRAY(tabV,g.getVertNo());
            typename GraphType::PEdge LOCALARRAY(tabE,g.getVertNo());
            int len=0;

            if (end!=start) for(;v;len++)
               { tabV[len]=v; tabE[len]=e; e=vertTab[v].ePrev; v=(v==start) ? 0 : vertTab[v].vPrev; }

            for(int i=len-1;i>=0;i--)
                { *iters.vertIter=tabV[i];*iters.edgeIter=tabE[i];++iters.vertIter;++iters.edgeIter; }
            *iters.vertIter=end;++iters.vertIter;
            return len;
    }

    // Zapisuje na podany iterator wszystkie krawedzie nalezace do drzewa (lasu) tj. uzyte jako wartosci pol ePrev
    // Zwraca ich liczbe
    template <class GraphType, class VertContainer,class Iter>
    static int getUsedEdges(const GraphType &g,
                            const VertContainer& vertTab // ten sam sens, co wyzej
                            ,Iter iter)
    {   int l=0;
        if (vertTab.empty()) return 0;
        for(typename VertContainer::KeyType v=vertTab.firstKey();;v=vertTab.nextKey(v))
        {   typename GraphType::PEdge e;
            if (v && (e=vertTab[v].ePrev)) {*iter=e; ++iter; l++; }
            if (v==vertTab.lastKey()) break;
        }
        return l;
    }

    template <class GraphType, class VertContainer>
    // jw. ale zwraca zbior krawedzi
    static Set<typename GraphType::PEdge> getUsedEdgesSet(const GraphType &g,const VertContainer& vertTab)
    {   Set<typename GraphType::PEdge> res;
        getUsedEdges(g,vertTab,setInserter(res));
        return res;
    }

};


class SearchStructs {

    public:
        // Struktura wartosci przypisywanej wierzcholkowi w procedurach przeszukiwania grafu
        template< class GraphType > struct VisitVertLabs
        {
            typename GraphType::PVertex vPrev; // rodzic danego wierzcholka w drzewie (lesie), NULL dla korzenia
            typename GraphType::PEdge ePrev; // krawedz prowadzaca do rodzica

            int distance, // odleglosc od korzenia (liczba krawedzi)
                component;  // numer skladowej spojnosci (od 0)

            VisitVertLabs(
                typename GraphType::PVertex vp = 0,
                typename GraphType::PEdge ep = 0,
                int dist = std::numeric_limits<int>::max(),
                int comp = -1 );

        } ;

//        Para iteratorow wyjsciowych do kodowania ciagu ciagow (zazwyczaj wierzcholki/kraw)
//        kolejne elementy kolejnych ciagow wysylane sa na vertIter
//        na compIter wyrzucane sa inty: pozycje (numeracja od 0) poczatkow kolejnych ciagow
//        oraz na koniec sumaryczna dlugosc wszystkich ciagow
//        (a wiec lacznie o 1 wiecej liczb, niz ilosc ciagow)
        template< class CIter, class VIter > struct CompStore
        {
            CIter compIter;
            VIter vertIter;

            CompStore( CIter ac, VIter av ): vertIter( av ), compIter( ac ) { }
        } ;

        // Funkcja tworzaca, analogia make_pair
        // Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
        template< class CIter, class VIter >
        static CompStore< CIter,VIter > compStore( CIter, VIter );

//        CompStore moze wspolpracowac z dowolnymi sekwencyjnymi kontenerami, ale ponizsza struktura
//        ulatwia obrobke takich danych
        template <class T> // typ elementu ciagow skladowych
        class VectCompStore {
            private:
                std::vector<int> idx;
                std::vector<T> data;
            public:
                VectCompStore() { clear(); }
                void clear() { idx.clear(); data.clear(); idx.push_back(0); }
                int size() const // liczba zebranych ciagow
                {
                    if (idx.empty()) return 0;
                    return idx.size()-1;
                }
                int size(int i) const // dlugosc i-tego ciagu
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    return idx[i+1]-idx[i];
                }
                int lenght() const // laczna dlugosc wszystkich ciagow
                {   int res=0;
                    for(int i=0;i<size();i++) res+=size(i);
                    return res;
                }
                T* operator[](int i) // wskaznik poczatku i-tego ciagu
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    return &data[idx[i]];
                }
                const T* operator[](int i) const // wskaznik poczatku i-tego ciagu
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    return &data[idx[i]];
                }

                T* insert(int i) // umieszcza nowy ciag pusty na pozycji i. Zwraca wskaznik jego poczatku
                {   assert(i>=0 && i<=size()); //TODO: throw
                    idx.insert(idx.begin()+i,idx[i]);
                    return operator[](i);
                }

                void del(int i) // kasuje ciag na pozycji i
                {   assert(i>=0 && i<=this->size()-1); //TODO: throw
                    int delta=size(i);
                    data.erase(data.begin()+idx[i],data.begin()+idx[i+1]);
                    for(int j=i+1;j<idx.size();j++) idx[j]-=delta;
                    idx.erase(idx.begin()+i);
                }

                T* resize(int i, int asize) // zmienia dlugosc i-tego ciagu. Zwraca wskaznik jego poczatku
                {   assert(i>=0 && i<=this->size()-1 && asize>=0); //TODO: throw
                    if (asize==size(i)) return this->operator[](i);
                    if (asize>size(i))
                    {   int delta=asize-size(i);
                        data.insert(data.begin()+idx[i+1],delta,T());
                        for(int j=i+1;j<idx.size();j++) idx[j]+=delta;
                    } else
                    {   int delta=size(i)-asize;
                        data.erase(data.begin()+(idx[i+1]-delta),data.begin()+idx[i+1]);
                        for(int j=i+1;j<idx.size();j++) idx[j]-=delta;
                    }
                    return this->operator[](i);
                }

                //Umiesczamy wywolanie funkcji w miejsu compStore a pozniej przetwarzamy zebrane ciagi
                // czysci kontener
                CompStore<std::back_insert_iterator<std::vector<int> >,std::back_insert_iterator<std::vector<T> > >
                    input() { idx.clear(); data.clear();
                        return compStore(std::back_inserter(idx),std::back_inserter(data));
                    }
        };

} ;

/* ------------------------------------------------------------------------- *
 * Visitors
 *
 * ------------------------------------------------------------------------- */

// Ocet jeden wie, co tu sie dzieje :-)

/** visitor's code is called by searching algorithms (BFS, DFS, LexBFS)
 * visitor should inherit from one of:
 * - simple_visitor_tag
 * - simple_preorder_visitor_tag
 * - simple_postorder_visitor_tag
 * - complex_visitor_tag
 * and one of:
 * - component_visitor_tag
 * - no_component_visitor_tag
 *
 * simple visitors (inheriting from simple_*_tag) have operator() called
 * with the following arguments:
 * template< class GraphType >
 * bool operator()(const GraphType &g,
 * 		   typename GraphType::PVertex u,
 * 		   VisitVertLabs< GraphType > &data)
 * where g - graph containing vertex, u - visited vertex, data - visited vertex's data
 * returning true allows to continue search, retrurning false terminate
 * searching algorithm
 * simple_preorder_visitor_tag indicate to visit vertex before its neighbourrs
 * simple_postorder_visitor_tag indicate to visit vertex after its neighbourrs
 * simple_visitor_tag do not specify the order
 *
 * complex visitors are notified by a searching algorithm using following methods:
 * (g - graph containing vertex, u - visited vertex,
 *  data - visited vertex's data, e - visited edge, v - vertex from which we
 *  visit edge e)
 *
 * template< class GraphType >
 * bool visitVertexPre(const GraphType &g,
 * 		       typename GraphType::PVertex u,
 * 		       VisitVertLabs< GraphType > &data);
 * called before visiting u's neighbours
 * returning false prevents visiting u's neighbours
 *
 * template< class GraphType >
 * bool visitVertexPost(const GraphType &g,
 * 			typename GraphType::PVertex u,
 * 			VisitVertLabs< GraphType > &data);
 * called after visiting u's neighbours
 * returning false terminates searching
 *
 * template< class GraphType >
 * bool visitEdgePre(const GraphType &g,
 * 		     typename GraphType::PEdge e,
 * 		     typename GraphType::PVertex v);
 * called before visiting other end of e
 * returning false prevents visiting vertex on the other end of e
 *
 * template< class GraphType >
 * bool visitEdgePost(const GraphType &g,
 * 		      typename GraphType::PEdge e,
 * 		      typename GraphType::PVertex v);
 * called after visiting other end of e
 * returning false terminates searching
 *
 * visitors with component_visitor_tag are notified about components with
 * methods:
 * template< class GraphType >
 * bool beginComponent(const GraphType &g, unsigned compid);
 * template< class GraphType >
 * bool endComponent(const GraphType &g, unsigned compid);
 * where g is the visited graph and compid is the component number (starting from 0)
 * return values are ignored (TODO: u¿yæ albo zmieniæ na void)
 *
 */
class Visitors: public SearchStructs {
    public:
        class component_visitor_tag { } ;
        class no_component_visitor_tag { } ;
        class simple_visitor_tag { } ;
        class simple_preorder_visitor_tag: public simple_visitor_tag { } ;
        class simple_postorder_visitor_tag: public simple_visitor_tag { } ;
        class complex_visitor_tag { } ;

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_preorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_postorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPre(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, ... );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_preorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, simple_postorder_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitVertexPost(
            const GraphType &, Visitor &, typename GraphType::PVertex,
            VisitVertLabs< GraphType > &, ... );

        template< class GraphType, class Visitor >
        static bool visitEdgePre(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePre(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, simple_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePre(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, ... );

        template< class GraphType, class Visitor >
        static bool visitEdgePost(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, complex_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePost(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, simple_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool visitEdgePost(
            const GraphType &, Visitor &, typename GraphType::PEdge,
            typename GraphType::PVertex, ... );

        template< class GraphType, class Visitor >
        static bool beginComponent(
            const GraphType &, Visitor &, unsigned, component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool beginComponent(
            const GraphType &, Visitor &, unsigned, no_component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool beginComponent(
            const GraphType &g, Visitor &v, unsigned comp, ... ) { return true; }

        static bool beginComponent( ... ) { return true; }

        template< class GraphType, class Visitor >
        static bool endComponent(
            const GraphType &, Visitor &, unsigned, component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool endComponent(
            const GraphType &, Visitor &, unsigned, no_component_visitor_tag & );

        template< class GraphType, class Visitor >
        static bool endComponent(
            const GraphType &g, Visitor &v, unsigned, ... ) { return true; }

        static bool endComponent( ... ) { return true; }

        /*
         * EmptyVisitor
         */

        class EmptyVisitor: public simple_postorder_visitor_tag,
            public no_component_visitor_tag
        {
            public:
                EmptyVisitor() { }

                template< class GraphType >
                bool operator()(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs< GraphType > & r) { return true; }
         } ;

        /*
         * EndVertVisitor
         * terminate algorithm when reaching given vertex
         */

        class EndVertVisitor: public complex_visitor_tag,
            public no_component_visitor_tag
        {
            public:
                EndVertVisitor( void *arg ): ptr( arg ) { }

                template< class GraphType >
                bool visitVertexPre(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs< GraphType > &data ) { return true; }

                template< class GraphType >
                bool visitVertexPost(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs< GraphType > &v ) { return true; }

                template< class GraphType >
                bool visitEdgePre(
                    const GraphType &g, typename GraphType::PEdge e,
                    typename GraphType::PVertex u ) { return (void*)u != ptr; }

                template< class GraphType >
                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

            private:
                void *ptr;
        } ;

        /*
         * StoreTargetToVertIter
         */

        template< class VertIter >
        class StoreTargetToVertIter: public simple_postorder_visitor_tag,
            public no_component_visitor_tag
        {
            public:
                StoreTargetToVertIter( VertIter &i ): m_iter( i ) { }

                template< class GraphType >
                bool operator()(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

            private:
                VertIter &m_iter;
        } ;

        /*
         * StoreCompVisitor
         */

        template< class CompIter, class VertIter >
        class StoreCompVisitor: public simple_postorder_visitor_tag,
            public component_visitor_tag
        {
            public:
                typedef struct _State
                {
                    CompStore< CompIter,VertIter > iters;
                    unsigned p, id;

                    _State( CompStore< CompIter,VertIter > i );
                } State;

                StoreCompVisitor( State &st ): m_st( st ) { }

            template< class GraphType >
            bool operator()(
                const GraphType &, typename GraphType::PVertex,
                VisitVertLabs< GraphType > & );

            template< class GraphType >
            bool beginComponent( const GraphType &, unsigned );

            template< class GraphType >
            bool endComponent( const GraphType &, unsigned );

            private:
                State &m_st;
        } ;

        /*
         * ComplexPreorderVisitor
         */

        template< class Visitor >
        class ComplexPreorderVisitor: public complex_visitor_tag
        {
            public:
                ComplexPreorderVisitor( Visitor &v ): visit( v ) { }

                template< class GraphType >
                bool visitVertexPre(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                template< class GraphType >
                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                template< class GraphType >
                bool visitEdgePre(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

                template< class GraphType >
                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

            private:
                Visitor &visit;
        } ;

        /*
         * ComplexPostorderVisitor
         */

        template< class Visitor >
        class ComplexPostorderVisitor: public complex_visitor_tag
        {
            public:
                ComplexPostorderVisitor( Visitor &v ): visit( v ) { }

                template< class GraphType >
                bool visitVertexPre(
                    const GraphType &g, typename GraphType::PVertex u,
                    VisitVertLabs<GraphType> &data ) { return true; }

                template< class GraphType >
                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                template< class GraphType >
                bool visitEdgePre(
                    const GraphType &g, typename GraphType::PEdge e,
                    typename GraphType::PVertex u ) { return true; }

                template< class GraphType >
                bool visitEdgePost(
                    const GraphType &g, typename GraphType::PEdge e,
                    typename GraphType::PVertex u ) { return true; }

            private:
                Visitor &visit;
        } ;

};


// Ogolna implementacja przeszukiwania grafu roznymi strategiami (DFS, BFS, LexBFS)
// Strategie dostarcza metoda visitBase z klasy SearchImpl, ktora odwiedza wierzcholki stosownie do swojej klasy
// DefaultStructs dostarcza wytycznych dla wewnetrznych procedur por. np. AlgsDefaultSettings z def_struct.h
template< class SearchImpl, class DefaultStructs >
class GraphSearchBase: public ShortPathStructs, public SearchStructs
{
    protected:

        // Typowy kontener dla wierzcholkow uzyteczny w tych procedurach
        // mapa PVertex -> VisitVertLabs< GraphType >
        template< class GraphType >
        class VisitedMap: public DefaultStructs:: template AssocCont<
            typename GraphType::PVertex, VisitVertLabs< GraphType > >::Type
            {   public:
                VisitedMap(int asize) : // inicjujemy przewidywanym potrzebnym rozmiarem
                    DefaultStructs:: template AssocCont<typename GraphType::PVertex, VisitVertLabs< GraphType > >
                        ::Type ( asize) {}
            } ;
    public:

            typedef SearchImpl SearchStrategy;

	/** visit all vertices in a graph tj. w kolejnosci zgodnej ze strategia SearchImpl
	 * @param[in] g graph containing vertices to visit
	 * @param[in] visited container to store data (np. map PVertex -> VisitVertLabs), BlackHole niedozwolony.
	 Po wykonaniu postac lasu przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component zawiera numer skladowej lasu
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
	 * @return number of components
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitAllBase(
            const GraphType &g, VertContainer &visited, Visitor visitor, EdgeDirection dir);

	/** visit all vertices in the same component as a given vertex tj. w kolejnosci zgodnej ze strategia SearchImpl
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[out] out iterator to write visited vertices to tj. w kolejnosci zgodnej ze strategia SearchImpl
	 * @param[in] dir direction of edges to consider + uwaga jak wyzej, ponadto bit petli ignorowany
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs) , BlackHole niedozwolony
	 Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component=0
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertContainer, class Iter >
        static int scanAttainable(
            const GraphType &, typename GraphType::PVertex, Iter,
            EdgeDirection, VertContainer & );

	/** visit all vertices in the same component as a given vertex
	Tzn. dziala jak wyzej, ale na wlasnej wewnetrznej mapie wierzcholkow
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[out] out iterator to write vertices to tj. w kolejnosci zgodnej ze strategia SearchImpl
	 * @param[in] dir direction of edges to consider + uwaga jak wyzej, ponadto bit petli ignorowany
	 * @return number of visited vertices
	 */
        template< class GraphType, class VertIter >
        static int scanAttainable(
            const GraphType &g, typename GraphType::PVertex src, VertIter out,
            EdgeDirection dir = EdUndir | EdDirOut );

	/** visit all vertices in a graph
	 * @param[in] g graph containing vertices to visit
	 * @param[out] out iterator to write visited vertices to tj. w kolejnosci zgodnej ze strategia SearchImpl
	 * @param[in] dir direction of edges to consider, petle ignorowane
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
	 	 Po wykonaniu postac lasu przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component zawiera numer skladowej lasu
    * @param[in] sym dla true maska
        jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
	 * @return number of components
	 */
        template< class GraphType, class VertContainer, class VertIter >
        static int scan(
            const GraphType &g, VertIter out, EdgeDirection dir, VertContainer &visited, bool sym=true);

	/** visit all vertices in a graph
	 * @param[in] g graph containing vertices to visit
	 * @param[out] out iterator to write vertices to, tj. w kolejnosci zgodnej ze strategia SearchImpl
        Petle ignorowane a maska
        jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
	 * @param[in] dir direction of edges to consider
	 * @return number of components
	 */
        template< class GraphType, class VertIter >
        static int scan(
            const GraphType &g, VertIter out,
            EdgeDirection dir = EdDirOut | EdUndir | EdDirIn );

	/** Liczba cyklomatyczna podgrafu zlozonego z krawedzi zgodnych z maska */
        template< class GraphType>
        static int cyclNo(
            const GraphType &, EdgeDirection = EdAll );

	/** return all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem, pod prad lub w obie strony,
	 ponadto bit petli ignorowany
	 * @return set of vertices in the component
	 */
        template< class GraphType >
        static Set< typename GraphType::PVertex > getAttainableSet(
            const GraphType &g, typename GraphType::PVertex src,
            EdgeDirection dir = EdDirOut | EdUndir );

	/** find a path between vertices
	 * @param[in] g graph to search path in
	 * @param[in] src starting vertex
	 * @param[in] dest target vertex
	 * @param[out] path found path
	 * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem, pod prad lub w obie strony,
	  petle ignorowane
	 * @return length of the path, -1 gdy nie ma polaczenia
	 */
        template< class GraphType, class VertIter, class EdgeIter >
        static int getPath(
            const GraphType &g, typename GraphType::PVertex src,
            typename GraphType::PVertex dest, OutPath< VertIter,EdgeIter >path,
            EdgeDirection dir = EdUndir | EdDirOut );

	/** split graph into components
	 * @param[in] g graph to split
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[in] dir direction of edges to consider, petle ignorowane a maska
        jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), blackHole niedozwolony
	 Po wykonaniu postac lasu przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component zawiera numer skladowej lasu
	 * @return number of components
	 */
        template< class GraphType, class VertContainer, class CompIter, class VertIter >
        static int getComponents(
            const GraphType &g, CompStore< CompIter,VertIter > out,
	    EdgeDirection dir, VertContainer &visited );


	/** split graph into components
	Tzn. jak wyzej, tylko dziala na wewnetrznej lokalnej mapie
	 * @param[in] g graph to split
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[in] dir direction of edges to consider, petle ignorowane a maska
        jest symetryzowana tj. jesli krawedzie skierowane sa przejezdne, to w obie strony
	 * @return number of components
	 */
        template< class GraphType, class CompIter, class VertIter >
        static int getComponents(
            const GraphType &g, CompStore< CompIter,VertIter > out,
            EdgeDirection dir = EdUndir | EdDirOut | EdDirIn );


} ;

/*
 * DFSParamBlock
 */

template< class GraphType, class VertContainer, class Visitor >
struct DFSParamBlock: public SearchStructs
{
    const GraphType &g;
    VertContainer &visited;
    Visitor &visit;
    EdgeDirection mask;
    int component;

    DFSParamBlock(
        const GraphType &, VertContainer &, Visitor &, EdgeDirection, int );
} ;

/*
 * DFSBase
 */

template< class SearchImpl, class DefaultStructs >
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DFSBase: public GraphSearchBase< SearchImpl , DefaultStructs >
{
    protected:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsDoVisit(
            DFSParamBlock< GraphType,VertContainer,Visitor > &,
            typename GraphType::PVertex, unsigned );

    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
	 Po wykonaniu  postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component jest ustawiane na compid
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsVisitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

/*
 * Depth-First-Search
 */

template <class DefaultStructs >
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DFSPar: public DFSBase< DFSPar<DefaultStructs >, DefaultStructs >
{
    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
	 	 Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component jest ustawiane na compid
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class DFS : public DFSPar<AlgsDefaultSettings> {};

/*
 * Preorder Depth-First-Search
 */

template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DFSPreorderPar: public DFSBase< DFSPreorderPar<DefaultStructs>, DefaultStructs >
{
    protected:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPreVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::complex_visitor_tag & );

        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPreVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::simple_visitor_tag & );

    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
	 Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component jest ustawiane na compid
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class DFSPreorder : public DFSPreorderPar<AlgsDefaultSettings> {};

/*
 * Postorder Depth-First-Search
 */

template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DFSPostorderPar: public DFSBase< DFSPostorderPar<DefaultStructs >, DefaultStructs >
{
    protected:
        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPostVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::complex_visitor_tag & );

        template< class GraphType, class VertContainer, class Visitor >
        static int dfsPostVisitBase(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int, Visitors::simple_visitor_tag & );

    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
	 Po wykonaniu  postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component jest ustawiane na compid
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to considertzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class DFSPostorder: public DFSPostorderPar<AlgsDefaultSettings> {};

/*
 * Breadth-First-Search
 */

template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class BFSPar: public GraphSearchBase< BFSPar<DefaultStructs >, DefaultStructs >
{
    protected:
        template< class GraphType, class VertContainer, class Visitor >
        static int bfsDoVisit(
            const GraphType &, typename GraphType::PVertex, VertContainer &,
            Visitor, EdgeDirection, int );

    public:
	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
	 Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component jest ustawiane na compid
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider tzn. przegladany jest podgraf zlozony z krawedzi
        zgodnych z maska, a pozostale kraw. ignorowane. Mozna
	 sobie zazyczyc, by kraw. skierowane byly przejezde zgodnie z ich zwrotem (EdDirOut), pod prad (EdDirIn) lub w obie strony (Directed)
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
	 */
        template< class GraphType, class VertContainer, class Visitor >
        static int visitBase(
            const GraphType &g, typename GraphType::PVertex src, VertContainer &visited,
            Visitor visitor, EdgeDirection dir, int compid);
} ;

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class BFS: public BFSPar<AlgsDefaultSettings> {};

/*
 * lexicographical Breadth-First-Search
 */



template<class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class LexBFSPar: public GraphSearchBase<LexBFSPar<DefaultStructs>,
					 DefaultStructs> {
public:
    template <class Graph>
    struct LVCNode {
			typename Graph::PVertex v;
			Privates::List_iterator<LVCNode> block;

			LVCNode(typename Graph::PVertex _v = NULL): v(_v) { };
			LVCNode(typename Graph::PVertex _v,Privates::List_iterator<LVCNode> it): v(_v), block(it) { };
			};

	template<class Graph, class Allocator, class ContAllocator>
	class LexVisitContainer {
	public:

		typedef LVCNode<Graph> Node;

		class Container: public Privates::List<Node,ContAllocator>	{
		    public:
            Container(ContAllocator& a) : Privates::List<Node,ContAllocator>(a) {}
		};

		Container m_data;
		Privates::List_iterator<Node>  m_openBlock;
		Privates::List<Privates::List_iterator<Node>,Allocator> m_splits;
		typename DefaultStructs::template AssocCont<typename Graph::PVertex, Privates::List_iterator<Node> >::Type m_vertexToPos;

	public:
		LexVisitContainer(Allocator& a, ContAllocator& ca,int n): m_data(ca), m_splits(a), m_openBlock(), m_vertexToPos(n)	{ }
		~LexVisitContainer()						{ clear(); }

		void clear() {
			m_data.clear();
			m_splits.clear();
			};

		void initialize(const Graph &g) {
			clear();
			m_data.push_back(Node(NULL));
			m_data.back().block = m_data.end();
			m_openBlock = m_data.begin();
			};

		void initialize(const Graph &g, size_t n, typename Graph::PVertex *tab) {
			initialize(g);
			for(size_t i = 0; i < n; i++) push(tab[i]);
			};

		void initializeAddAll(const Graph &g) {
			typename Graph::PVertex v;
			initialize(g);
			for(v = g.getVert(); v != NULL; v = g.getVertNext(v))
				push(v);
			};

		void cleanup() {
			if(m_data.size() < 2) return;
			while(m_data.begin().next()->v == NULL) {
				if(m_data.begin() == m_openBlock) m_openBlock = m_data.end();
				m_data.pop_front();
				};
			};

		bool empty() {
			cleanup();
			return m_data.size() < 2;
			};

		typename Graph::PVertex top() {
			cleanup();
			return m_data.begin().next()->v;
			};

		void pop() {
			m_data.erase(m_data.begin().next());
			cleanup();
			};

		void push(typename Graph::PVertex v) {
			if(m_openBlock == m_data.end()) {
				m_data.push_back(Node(NULL));
				m_data.back().block = m_data.end().prev();
				};
			m_data.push_back(Node(v, m_openBlock));
			m_vertexToPos[v] = m_data.end().prev();
			};

		void move(typename Graph::PVertex v) {
			Privates::List_iterator<Node> grp, newGrp;
			Privates::List_iterator<Node> elem;
			if(!m_vertexToPos.hasKey(v)) push(v);
			elem = m_vertexToPos[v];
			grp = elem->block;
			newGrp = grp->block;
			if(newGrp == m_data.end()) {
				newGrp = m_data.insert_before(grp, Node(NULL));
				grp->block = newGrp;
				m_splits.push_back(grp);
				newGrp->block = m_data.end();
				};
			m_data.move_before(grp, elem);
			elem->block = newGrp;
			};

		void remove(typename Graph::PVertex v) {
			m_data.erase(m_vertexToPos[v]);
			};

		void done() {
			Privates::List_iterator<Privates::List_iterator<Node> > it, e;
			for(it = m_splits.begin(), e = m_splits.end(); it != e; ++it) {
				(*it)->block = m_data.end();
				};
			m_splits.clear();
			};

		void dump() {
			Privates::List_iterator<Node> it;
			for(it = m_data.begin(); it != m_data.end(); ++it) {
				if(it->v == NULL) printf(" |");
				else printf(" %p", it->v);
				};
			printf("\n");
			};
		};

    protected:

    template<class GraphType>
	struct OrderData {
		typename GraphType::PVertex v;
		int vertId;	// kogo jest sÂ¹siadem (numer sÂ¹siada w porzÂ¹dku)
		int orderId;	// numer w porzÂ¹dku
		};

    public:

	/** visit all vertices in the same component as a given vertex
	 * @param[in] g graph containing vertices to visit
	 * @param[in] src given vertex
	 * @param[in] visited container to store data (map PVertex -> VisitVertLabs), BlackHole niedozwolony
	 	 Po wykonaniu postac drzewa przeszukiwania opisuja pola vPrev, ePrev przypisane wierzcholkom, distance = odleglosc od korzenia,
    component jest ustawiane na component
	 * @param[in] visitor visitor called for each vertex
	 * @param[in] dir direction of edges to consider, LexBFS akceptuje tylko maski symetryczne
	 * @param[in] compid component identifier (give 0 if don't know)
	 * @return number of visited vertices lub -number jesli przeszukiwanie przerwano z polecenia visitora
	 */
	template<class GraphType,
		 class VertContainer,
		 class Visitor>
	static int visitBase(const GraphType & g,
			     typename GraphType::PVertex start,
			     VertContainer &visited,
			     Visitor visit,
			     EdgeDirection mask,
			     int component) {
		unsigned int depth, n, retVal;
		typename GraphType::PEdge e;
		typename GraphType::PVertex u, v;
//		DefaultCPPAllocator allocat;
//        DefaultCPPAllocator2 allocat2;
//		LexVisitContainer<GraphType, DefaultCPPAllocator,DefaultCPPAllocator2> cont(allocat,allocat2);

        if (DefaultStructs::ReserveOutAssocCont) visited.reserve(g.getVertNo());
        assert(((mask&Directed)==0)||((mask&Directed)==Directed)); // TODO: throw
		Privates::BlockListAllocator<Privates::ListNode<Privates::List_iterator<LVCNode<GraphType> > > > allocat(g.getVertNo()+1); //TODO: size? - spr
        Privates::BlockListAllocator<Privates::ListNode<LVCNode<GraphType> > > allocat2(2*g.getVertNo()+1); //TODO: size? - spr 2n+1 -> n+1 - oj! raczej nie!
		LexVisitContainer<GraphType, Privates::BlockListAllocator<Privates::ListNode<Privates::List_iterator<LVCNode<GraphType> > > >,Privates::BlockListAllocator<Privates::ListNode<LVCNode<GraphType> > > >
            cont(allocat,allocat2,g.getVertNo());


		n = g.getVertNo();
		if(n == 0) return 0;
		if(start == NULL) start = g.getVert();

		cont.initialize(g);

		visited[start] = SearchStructs::VisitVertLabs<GraphType>(NULL, NULL, 0, component);
		cont.push(start);
		retVal = 0;

		while(!cont.empty()) {
//			printf("before: ");
//			cont.dump();
			u = cont.top();
			depth = visited[u].distance;
			visited[u].component = component;

			if(!Visitors::visitVertexPre(g, visit, u, visited[u], visit)) {
				retVal++;
				continue;
				};
			cont.pop();

			for(e = g.getEdge(u, mask); e != NULL; e = g.getEdgeNext(u, e, mask)) {
				v = g.getEdgeEnd(e, u);
				if(!Visitors::visitEdgePre(g, visit, e, u, visit)) continue;
//				if(visited.hasKey(v)) continue;
//				visited[v] = SearchStructs::VisitVertLabs<GraphType>(u, e, depth + 1, component);
				if(visited.hasKey(v)) {
					if(visited[v].component == -1) {
						cont.move(v);
						};
					continue;
					};
				visited[v] = SearchStructs::VisitVertLabs<GraphType>(u, e, depth + 1, -1);
				cont.move(v);
				if(!Visitors::visitEdgePost(g, visit, e, u, visit)) return -retVal;
				};
			cont.done();
//			printf("after:  ");
//			cont.dump();

			retVal++;
			if(!Visitors::visitVertexPost(g, visit, u, visited[u], visit))
				return -retVal;
			};
		return retVal;
		};


	/** order vertices with LexBFS order, starting with a given sequence
	 * @param[in] g graph
	 * @param[in] in number of vertices in table tab
	 * @param[in] tab table containing initial order of vertices
	 * @param[in] mask direction of edges to consider, LexBFS akceptuje tylko maski symetryczne
	 * @param[out] out iterator to write ordered vertices
	 * @return number of vertices written to out
	 */
	template<class GraphType,
		 class OutVertIter>
	static int order2(const GraphType & g,
			  size_t in,
			  typename GraphType::PVertex *tab,
			  EdgeDirection mask,
			  OutVertIter out) {

		int i, j, o, n, m, retVal;
		EdgeDirection bmask = mask;
		typename GraphType::PEdge e;
		typename GraphType::PVertex u, v;
		typename DefaultStructs::template AssocCont<typename GraphType::PVertex, std::pair<int, int> >::Type orderData(g.getVertNo());
		Privates::BlockListAllocator<Privates::ListNode<Privates::List_iterator<LVCNode<GraphType> > > > allocat(g.getVertNo()+1); //TODO: size? - spr
        Privates::BlockListAllocator<Privates::ListNode<LVCNode<GraphType> > > allocat2(2*g.getVertNo()+1); // j.w. 2n+1 -> n + 1 - oj! raczej nie!
		LexVisitContainer<GraphType, Privates::BlockListAllocator<Privates::ListNode<Privates::List_iterator<LVCNode<GraphType> > > >,Privates::BlockListAllocator<Privates::ListNode<LVCNode<GraphType> > > >
            cont(allocat,allocat2,g.getVertNo());

		bmask &= ~EdLoop;
//		if(bmask & EdDirOut) bmask &= ~EdDirIn; //TODO: watpliwe
        assert(((bmask&Directed)==0)||((bmask&Directed)==Directed)); // TODO: throw

		n = g.getVertNo();
		assert(in == n);
		m = g.getEdgeNo(bmask);
		int LOCALARRAY(first, n + 1);
		OrderData<GraphType> LOCALARRAY(neigh, m * 2);
		OrderData<GraphType> LOCALARRAY(neigh2, m * 2);

		for(o = 0; o < n; o++) orderData[tab[o]].second = o;

		i = j = 0;
		for(o = 0; o < n; o++) {
			u = tab[o];
			first[i] = j;
			orderData[u].first = 0;
			orderData[u].second = o;
			for(e = g.getEdge(u, bmask); e != NULL; e = g.getEdgeNext(u, e, bmask)) {
				v = g.getEdgeEnd(e, u);
				neigh[j].v = v;
				neigh[j].orderId = orderData[v].second;
				neigh[j].vertId = o;
				j++;
				};
			i++;
			};
		first[i] = j;

		LexBFSPar<DefaultStructs>::StableRadixSort(neigh, j, n, &OrderData<GraphType>::orderId, neigh2);
		LexBFSPar<DefaultStructs>::StableRadixSort(neigh2, j, n, &OrderData<GraphType>::vertId, neigh);

		retVal = 0;
		cont.initialize(g, in, tab);

//		cont.dump();

		while(!cont.empty()) {
			u = cont.top();
			cont.pop();
			orderData[u].first = 2;
			*out = u;
			++out;
			++retVal;

			j = orderData[u].second;
			for(i = first[j]; i < first[j + 1]; i++) {
				v = neigh[i].v;
//				if(orderData[v].first) continue;
//				orderData[v].first = true;
				if(orderData[v].first > 0) {
					if(orderData[v].first == 1) {
						cont.move(v);
						};
					continue;
					};
				orderData[v].first = 1;
				cont.move(v);
				};
			cont.done();
//		cont.dump();
			};
		return retVal;
		};

	template<class T>
	static void StableRadixSort(T *data,
				    int n,
				    int nb,
				    int T::*field,
				    T *out) {
		int LOCALARRAY(bucketFirst, nb);
		int LOCALARRAY(next, n);
		int i, bp;
		for(i = 0; i < nb; i++) bucketFirst[i] = -1;
		for(i = 0; i < n; i++) {
			bp = data[i].*field;
			if(bucketFirst[bp] < 0) {
				next[i] = i;
			} else {
				next[i] = next[bucketFirst[bp]];
				next[bucketFirst[bp]] = i;
				};
			bucketFirst[bp] = i;
			};
		for(bp = 0; bp < nb; bp++) {
			i = bucketFirst[bp];
			if(i < 0) continue;
			do {
				i = next[i];
				*out = data[i];
				++out;
			} while(i != bucketFirst[bp]);
			};
		};

	};


// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class LexBFS: public LexBFSPar<AlgsDefaultSettings> {};


/*
 * Cheriyan/Mehlhorn/Gabow algorithm
 */

// Wyszukiwanie skladowych silnie spojnych grafu skierowanego
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class SCCPar: protected SearchStructs
{
    protected:
        struct SCCData {
            int preIdx;
            bool assigned;
            SCCData( int p = 0, bool a = false ): preIdx( p ), assigned( a ) { }
        } ;

        template< class GraphType, class CompIter, class VertIter, class CompMap >
        class SCCState
        {
            public:
                SCCState(
                    CompStore< CompIter,VertIter >, CompMap &,
                    typename GraphType::PVertex *, typename GraphType::PVertex*,
                    int );

                void addVert( typename GraphType::PVertex );
                void newComp();

                typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,SCCData >::Type vmap;
                StackInterface< typename GraphType::PVertex* > s, p;
                CompStore< CompIter,VertIter > iters;
                CompMap &compMap;
                unsigned idx;
                int count, c;
        } ;

        template< class GraphType, class CompIter, class VertIter, class CompMap >
        class SCCVisitor: public Visitors::complex_visitor_tag,
            public Visitors::no_component_visitor_tag
        {
            public:
                SCCVisitor( SCCState< GraphType,CompIter,VertIter,CompMap > & );

                bool visitVertexPre(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                bool visitEdgePre(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

            private:
                SCCState< GraphType,CompIter,VertIter,CompMap > &state;
        } ;

    public:
	/** split graph into strongly connected components, uwzglednia krawedzie wszystkich typow
	 * @param[in] g graph to split
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	 * @param[out] vtoc map (PVertex -> int indexOfItsComponent(zero based)), lub BlackHole
	 * @return number of components
	 */
        template< class GraphType, class CompIter, class VertIter, class CompMap >
        static int get(
            const GraphType &g, CompStore< CompIter,VertIter > out, CompMap & vtoc);

//    Korzysta z mapy CompMap z poprzedniej procedury. Wyrzuca na iterator wartosci std::pair<int,int> - wszystkie
//    pary numerow skladowych silnie spojnych, ktore sa polaczone choc jednym bezposrednim lukiem. Zwraca dlugos
//    ciagu wynikowego
        template< class GraphType, class CompMap, class PairIter >
        static int connections(const GraphType &, CompMap &, PairIter );
} ;

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class SCC : public SCCPar<AlgsDefaultSettings> {};

// Procedury na digrafach acykliczych
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class DAGAlgsPar: protected SearchStructs
{
    public:

        // wyrzuca na iterator wierzcholki grafu w porzadku topologicznym
        template< class GraphType, class VertIter >
        static void topOrd( const GraphType &, VertIter );

        // sprawdza, czy graf jest DAGiem korzystajac z podanego para iteratorow ciagu wierzcholkow z wyjscia poprzedniej procedury
        template< class GraphType, class Iter >
        static bool isDAG( const GraphType &, Iter, Iter );

        // sprawdza, czy graf jest DAGiem, samodzielna
        template< class GraphType >
        static bool isDAG( const GraphType & );

        // wyrzuca na iterator wszystkie luki przechodnie DAGa, zwraca dlugosc ciagu
        template< class GraphType, class Iter >
        static int transEdges(const GraphType &, Iter);

        // usuwa luki przechodnie DAGa
        template< class GraphType>
        static void makeHasse(GraphType & );

} ;

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class DAGAlgs : public DAGAlgsPar<AlgsDefaultSettings> {};

/*
 * Blocks -- biconnected components
 */

//Wykrywanie skladowych 2-spojnych (blokow) grafu
template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class BlocksPar : protected SearchStructs
{
    protected:
        template< class GraphType >
        struct BiConVData {
            unsigned depth;
            unsigned lowpoint;
            unsigned count;
            bool iscut;
            int visited;
            int sons;
            int vblFirst;
        } ;

        struct VertBlockList {
            int block;
            int next;
        } ;

        template< class GraphType, class CompIter, class VertIter, class EdgeMap >
        class BiConState
        {
            public:
                typedef typename GraphType::PVertex PVertex;

                BiConState(
                    CompStore< CompIter,VertIter >, EdgeMap &, EdgeDirection,
                        std::pair< typename GraphType::PEdge *,int >,
                        VertBlockList *,int );

                void addVert( typename GraphType::PVertex );
                void addEdge( typename GraphType::PEdge );

                void newComp();

                typename DefaultStructs:: template AssocCont<
                    typename GraphType::PVertex,BiConVData< GraphType > >::Type vmap;
                typename DefaultStructs:: template AssocCont<
                    typename GraphType::PEdge,bool >::Type emap;
                StackInterface< typename GraphType::PEdge* > estk;
                CompStore< CompIter,VertIter > iters;
                EdgeMap &outEMap;
                VertBlockList *vbl;
                int vblAlloc;
                unsigned idx;
                int count;
                EdgeDirection mask;
        } ;

        template< class GraphType, class CompIter, class VertIter, class EdgeMap >
        class BiConVisitor: public Visitors::complex_visitor_tag,
            public Visitors::no_component_visitor_tag
        {
            public:
                BiConVisitor( BiConState< GraphType,CompIter,VertIter,EdgeMap > & );

                bool visitVertexPre(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                bool visitVertexPost(
                    const GraphType &, typename GraphType::PVertex,
                    VisitVertLabs< GraphType > & );

                bool visitEdgePre(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

                bool visitEdgePost(
                    const GraphType &, typename GraphType::PEdge,
                    typename GraphType::PVertex );

            private:
                BiConState< GraphType,CompIter,VertIter,EdgeMap > &state;
        } ;

        template< class State, class VertMap, class VertBlockIter >
        static void storeBlocksData(
            State &, VertBlockList *, VertMap &, VertBlockIter & );

    public:
        struct VertData { // wynikowa etykieta wierzcholka
            int blockNo;    // w ilu blokach lezy ten wierzcholek
            int firstBlock; // pozycja pierwszego w sekwencji numerow blokow (por. viter nizej) bloku zawierajacego
            // ten wierzcholek (jego pozostale bloki wystepuja kolejno za nim)
            VertData( int b = 0, int f = -1 ): blockNo( b ), firstBlock( f ) { }
        } ;

	/** split graph into blocks
	 * @param[in] g graph to split, uwzglednia wszystkie krawedzie traktujac luki jak nieskierowane
	 * @param[out] vmap mapa PVertex->VertData powiazana z ciagiem zapisanym na viter (lub BlackHole)
	 * @param[out] emap mapa PEdge->int - w ktorym bloku znajduje sie dana krawedz (lub BlackHole)
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
        CompStore do ktorego zapisuje sie ciagi wierzcholkow z kolejnych blokow
	 * @param[out] viter Iterator na ktory wypisywana jest ciag numerow blokow (byc moze z powtorzeniami) - por. VertData
	 * @return number of biconnected components
	 */
        template< class GraphType, class VertDataMap, class EdgeDataMap,
            class CompIter, class VertIter, class VertBlockIter >
        static int split(
            const GraphType &g, VertDataMap &, EdgeDataMap &,
            CompStore< CompIter,VertIter > out, VertBlockIter );


	/** split a component containing given vertex into blocks
	 * @param[in] g graph to split, uwzglednia wszystkie krawedzie traktujac luki jak nieskierowane
	 * @param[in] src given vertex
	 * @param[out] vmap mapa PVertex->VertData powiazana z ciagiem zapisanym na viter (lub BlackHole)
	 * @param[out] emap mapa PEdge->int - w ktorym bloku znajduje sie dana krawedz (lub BlackHole)
	 * @param[out] out pair of output iterators (elements of first iterator will point to first vertex in component in second iterator)
	         CompStore do ktorego zapisuje sie ciagi wierzcholkow z kolejnych blokow
	 * @param[out] viter Iterator na ktory wypisywana jest ciag numerow blokow (byc moze z powtorzeniami) - por. VertData
	 * @return number of biconnected components in a component containing given vertex
	 */
        template< class GraphType, class VertDataMap, class EdgeDataMap,
            class CompIter, class VertIter, class VertBlockIter >
        static int splitComp(
            const GraphType &g, typename GraphType::PVertex src, VertDataMap &vmap,
            EdgeDataMap &emap, CompStore< CompIter,VertIter > out, VertBlockIter viter);

        // wyrzuca na iterator ciag wierzcholkow tworzacych rdzen grafu tj. podgraf pozostajacy po sukcesywnym
        // usuwaniu wierzcholkow stopnia < 2. Zwraca dlugosc sekwencji
        template<class GraphType,class Iterator>
        static int core(const GraphType &g,Iterator out)
        {   const EdgeType mask=EdAll;
            typename DefaultStructs:: template AssocCont<
                typename GraphType::PVertex, int >::Type degs(g.getVertNo());
            std::pair<int, typename GraphType::PVertex> LOCALARRAY(buf,2*g.getVertNo());
            PriQueueInterface< std::pair<int, typename GraphType::PVertex>*,
                    std::greater<std::pair<int, typename GraphType::PVertex> > > q(buf,2*g.getVertNo());
            if (!g.getVertNo()) return 0;
            for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
                q.push(std::make_pair(degs[v]=g.deg(v,mask),v));
            while(!q.empty() && q.top().first<=1)
            {   if (!degs.hasKey(q.top().second)) { q.pop(); continue; }
                typename GraphType::PVertex v=q.top().second,u;
                typename GraphType::PEdge e=g.getEdge(v,mask);
                degs.delKey(v);q.pop();
                if (e && degs.hasKey(u=g.getEdgeEnd(e,v))) q.push(std::make_pair(--degs[u],u));
            }
            if (!isBlackHole(out)) for(typename GraphType::PVertex v=degs.firstKey();v;v=degs.nextKey(v))
            {   *out=v; ++ out;   }
            return degs.size();
        }

} ;

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class Blocks : public BlocksPar<AlgsDefaultSettings> {};

/*
 * Eulerian cycle and path
 */

template <class DefaultStructs>
// DefaultStructs - wytyczne dla wewnetrznych procedur
class EulerPar: public PathStructs, protected SearchStructs {
protected:
	template<class GraphType>
	struct EulerState {
		const GraphType &g;
		StackInterface<std::pair<typename GraphType::PVertex, typename GraphType::PEdge> *> stk;
		typename DefaultStructs:: template AssocCont<typename GraphType::PEdge, bool>::Type edgeVisited;
		EdgeDirection mask;

		EulerState(const GraphType &_g,
			   std::pair<typename GraphType::PVertex, typename GraphType::PEdge> *_stk,
			   int nv,
			   EdgeDirection m):
			g(_g), stk(_stk, nv), edgeVisited(_g.getEdgeNo()), mask(m)
                {};
		};

	template<class GraphType>
	static void eulerEngine(typename GraphType::PVertex u,
				typename GraphType::PEdge ed,
				EulerState<GraphType> &state) {
		typename GraphType::PEdge e;
		typename GraphType::PVertex v;
		for(e = state.g.getEdge(u, state.mask);
		    e != NULL;
		    e = state.g.getEdgeNext(u, e, state.mask)) {
			if(state.edgeVisited.hasKey(e)) continue;
			state.edgeVisited[e] = true;
//			if(state.g.getEdgeType(e) == Loop) {
//				state.stk.push(std::make_pair(u, e));
//				continue;
//				};
			v = state.g.getEdgeEnd(e, u);
			eulerEngine(v, e, state);
			};
		state.stk.push(std::make_pair(u, ed));
		};


	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static void eulerResult(EulerState<GraphType> &state,
				OutPath<VertIter, EdgeIter> &out) {
		std::pair<typename GraphType::PVertex,
			  typename GraphType::PEdge> p;
		p = state.stk.top();
		state.stk.pop();
		*(out.vertIter) = p.first;
		++(out.vertIter);
		while(!state.stk.empty()) {
			p = state.stk.top();
			state.stk.pop();
			*(out.vertIter) = p.first;
			++(out.vertIter);
			*(out.edgeIter) = p.second;
			++(out.edgeIter);
			};
		};

    template<class GraphType>
	static std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
    ends(const GraphType &g,EdgeType mask)//=Undirected
    {
        EdgeDirection symmask = mask | ((mask&(EdDirIn|EdDirOut)) ? EdDirIn|EdDirOut : 0);
        bool dir= (mask&(EdDirIn|EdDirOut))==EdDirIn || (mask&(EdDirIn|EdDirOut))==EdDirOut;
        assert(!(dir && (mask&EdUndir)));
        std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
                    zero((typename GraphType::PVertex)0,(typename GraphType::PVertex)0),
                    res((typename GraphType::PVertex)0,(typename GraphType::PVertex)0);
        typename GraphType::PVertex x;
        int licz=0;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if (g.getEdge(v,symmask)) { licz++; x=v; }
        if (licz==0) return zero;
        if (licz!=BFSPar<DefaultStructs>::scanAttainable(g,x,blackHole,symmask & ~EdLoop)) return zero;
        for(typename GraphType::PVertex v=g.getVert();v;v=g.getVertNext(v))
            if (!dir)
            {
                if (g.deg(v,symmask)&1)
                    if (res.first==0) res.first=v;
                    else if (res.second==0) res.second=v;
                        else return zero;
            }
            else
                switch (g.deg(v,EdDirOut)-g.deg(v,EdDirIn))
                {   case 1: if (res.first==0) res.first=v; else return zero; break;
                    case 0: break;
                    case -1:if (res.second==0) res.second=v; else return zero; break;
                    default: return zero;
                }

        if (res.first)
            if (dir && (mask&EdDirIn)) return std::make_pair(res.second,res.first);
            else return res;
        else return std::pair<typename GraphType::PVertex,typename GraphType::PVertex>(x,x) ;
    }


public:

    // Uwaga: wersje bez dir uwzgledniaja jedynie podgraf zlozony z petli i krawedzi nieskierowanych ignorujac reszte
    // Wersje z dir uwzgledniaja jedynie podgraf zlozony z petli i lukow ignorujac reszte

    template<class GraphType>
    // para zawierajaca 2 razy ten sam wierzcholek - jesli graf ma nieskierowany cykl Eulera
    // para zawierajaca 2 rozne wierzcholki - konce nieskierowanej sciezki Eulera - jesli ta istnieje
    // (NULL,NULL) w przciwnym razie
	static std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
    ends(const GraphType &g)
    {
        return ends(g,EdUndir|EdLoop);
    }


    template<class GraphType>
    // para zawierajaca 2 razy ten sam wierzcholek - jesli graf ma skierowany cykl Eulera
    // para zawierajaca 2 rozne wierzcholki - konce skierowanej sciezki Eulera - jesli ta istnieje
    // (NULL,NULL) w przciwnym razie
	static std::pair<typename GraphType::PVertex,typename GraphType::PVertex>
    dirEnds(const GraphType &g)
    {
        return ends(g,EdDirOut|EdLoop);
    }


	/** test if graph has an undirected Eulerian cycle
	 * @param[in] g graph
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasCycle(const GraphType &g) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdUndir|EdLoop);
		return res.first!=0 && res.first==res.second;
	};

	/** test if graph has a directed Eulerian cycle
	 * @param[in] g graph
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasDirCycle(const GraphType &g) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdDirOut|EdLoop);
		return res.first!=0 && res.first==res.second;
	};


	/** test if graph has an undirected Eulerian path
	 * @param[in] g graph
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasPath(const GraphType &g) {
        std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdUndir|EdLoop);
		return res.first!=0 && res.first!=res.second;
    };

	/** test if graph has a directed Eulerian path
	 * @param[in] g graph
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasDirPath(const GraphType &g) {
        std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdDirOut|EdLoop);
		return res.first!=0 && res.first!=res.second;
    };


	/** test if graph has an undirected Eulerian path starting at vertex u
	 * @param[in] g graph
	 * @param[in] u starting vertex
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasPath(const GraphType &g,
			    typename GraphType::PVertex u) {
        assert(u); // TODO: throw
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdUndir|EdLoop);
//		bool dir= (mask&(EdDirIn|EdDirOut))==EdDirIn || (mask&(EdDirIn|EdDirOut))==EdDirOut;
		return (res.first==u ||(/* !dir &&*/ res.second==u));
		};

	/** test if graph has an directed Eulerian path starting at vertex u
	 * @param[in] g graph
	 * @param[in] u starting vertex
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasDirPath(const GraphType &g,
			    typename GraphType::PVertex u) {
        assert(u); // TODO: throw
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdDirOut|EdLoop);
//		bool dir= (mask&(EdDirIn|EdDirOut))==EdDirIn || (mask&(EdDirIn|EdDirOut))==EdDirOut;
		return res.first==u;// ||(!dir && res.second==u));
		};


	/** test if graph has an undirected Eulerian cycle containing vertex u
	 * @param[in] g graph
	 * @param[in] u given vertex
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasCycle(const GraphType &g,
			    typename GraphType::PVertex u) {
        assert(u); // TODO: throw
		return hasCycle(g) && g.deg(u,EdUndir|EdLoop);
		};

	/** test if graph has an directed Eulerian cycle containing vertex u
	 * @param[in] g graph
	 * @param[in] u given vertex
	 * @param[in] mask type of edges to consider
	 * @return true if it has, false otherwise */
	template<class GraphType>
	static bool hasDirCycle(const GraphType &g,
			    typename GraphType::PVertex u) {
        assert(u); // TODO: throw
		return hasDirCycle(g) && g.deg(u,EdDirOut|EdLoop);
		};


	/** get undirected Eulerian cycle
	 * @param[in] g graph
	 * @param[out] out found cycle
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getCycle(const GraphType &g,
			    OutPath<VertIter, EdgeIter> out) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdUndir|EdLoop);
		if (res.first==0 || res.first!=res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, EdUndir|EdLoop);
		eulerEngine<GraphType>(res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};

	/** get directed Eulerian cycle
	 * @param[in] g graph
	 * @param[out] out found cycle
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getDirCycle(const GraphType &g,
			    OutPath<VertIter, EdgeIter> out) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdDirOut|EdLoop);
		if (res.first==0 || res.first!=res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, EdDirOut|EdLoop);
		eulerEngine<GraphType>(res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};


	/** get undirected Eulerian cycle
	 * @param[in] g graph
	 * @param[in] prefstart preferred starting vertex, ale jesli cykl E. nie
	  przechodzi przez ten wierzcholek, preferencja bedzie zignorowana
	 * @param[out] out found cucle
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getCycle(const GraphType &g,
			    typename GraphType::PVertex prefstart,
			    OutPath<VertIter, EdgeIter> out) {

		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdUndir|EdLoop);
		if (res.first==0 || res.first!=res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, EdUndir|EdLoop);
		eulerEngine<GraphType>(g.getEdge(prefstart,EdUndir|EdLoop) ? prefstart : res.first, NULL, state);
		eulerResult(state, out);
		return true;
        };

	/** get directed Eulerian cycle
	 * @param[in] g graph
	 * @param[in] prefstart preferred starting vertex, ale jesli cykl E. nie
	  przechodzi przez ten wierzcholek, preferencja bedzie zignorowana
	 * @param[out] out found cucle
	 * @return true if graph has an Eulerian cycle, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getDirCycle(const GraphType &g,
			    typename GraphType::PVertex prefstart,
			    OutPath<VertIter, EdgeIter> out) {

		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdDirOut|EdLoop);
		if (res.first==0 || res.first!=res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, EdDirOut|EdLoop);
		eulerEngine<GraphType>(g.getEdge(prefstart,EdDirOut|EdLoop) ? prefstart : res.first, NULL, state);
		eulerResult(state, out);
		return true;
        };



	/** get undirected Eulerian path
	 * @param[in] g graph
	 * @param[out] out found path
	 * @return true if graph has an Eulerian path, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getPath(const GraphType &g,
			   OutPath<VertIter, EdgeIter> out) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdUndir|EdLoop);
		if (res.first==0 || res.first==res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, EdUndir|EdLoop);
		eulerEngine<GraphType>(res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};


	/** get directed Eulerian path
	 * @param[in] g graph
	 * @param[out] out found path
	 * @return true if graph has an Eulerian path, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getDirPath(const GraphType &g,
			   OutPath<VertIter, EdgeIter> out) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdDirOut|EdLoop);
		if (res.first==0 || res.first==res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, EdDirOut|EdLoop);
		eulerEngine<GraphType>(res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};


	/** get undirected Eulerian path
	 * @param[in] g graph
	 * @param[in] prefstart preferred starting vertex, ale jesli sciezka E. nie
	  zaczyna sie od tego wierzcholka, preferencja bedzie zignorowana
	 * @param[out] out found path
	 * @param[in] mask type of edges to consider
	 * @return true if graph has an Eulerian path, false otherwise */
	template<class GraphType,
		 class VertIter,
		 class EdgeIter>
	static bool getPath(const GraphType &g,
			    typename GraphType::PVertex prefstart,
			   OutPath<VertIter, EdgeIter> out) {
		std::pair<typename GraphType::PVertex,typename GraphType::PVertex> res=ends(g,EdUndir|EdLoop);
		if (res.first==0 || res.first==res.second) return false;
		std::pair<typename GraphType::PVertex, typename GraphType::PEdge> LOCALARRAY(_vstk, g.getVertNo() + g.getEdgeNo()); //TODO: size? - spr
		EulerState<GraphType> state(g, _vstk, g.getVertNo() + g.getEdgeNo() + 1, EdUndir|EdLoop);
//		bool dir= (mask&(EdDirIn|EdDirOut))==EdDirIn || (mask&(EdDirIn|EdDirOut))==EdDirOut;
		eulerEngine<GraphType>((prefstart==res.second) ? res.second : res.first, NULL, state);
		eulerResult(state, out);
		return true;
		};

};

// wersja dzialajaca na DefaultStructs=AlgsDefaultSettings
class Euler : public EulerPar<AlgsDefaultSettings> {};

#include "../algorithm/search.hpp"




}

#endif
