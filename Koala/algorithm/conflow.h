#ifndef KOALA_DEF_FLOWS_H
#define KOALA_DEF_FLOWS_H

/* conflow.h
 *
 */

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <vector>
#include <utility>

#include "../algorithm/search.h"
#include "../base/defs.h"
#include "../graph/view.h"

namespace Koala
{

//WEN: Uwaga globalna do calego pliku: wszystkie procedury tutaj uwzgledniaja krawedzie (z przypisanymi przepustowosciami) wszystkich rodzajow
// rownoleglosci dozwolone. Jesli gdzies w wejsciu do proc. podajemy wyroznione wierzcholki start end, musza byc rozne, bo poleci wyjatek
// - to powinno byc w opisach metod (chyba ze przy procedurze juz teraz jawnie napisano inaczej).
//Zwracam uwage, ze edgeTab czesto nie jest ani tylko [in] ani [out] np. dostarcza przepustowosci lukow i zbiera znaleziony flow.
//Obowiazuje do konca pliku

    namespace Privates {

		// tworzy w ig pomocnicza siec na podstawie g. Mapa images laczy wierzcholki g z parami odpowiednich krawedzi w ig
		template< class GraphType, class ImageType, class Linker >
			static void flowsMakeImage( const GraphType &g, ImageType &ig, Linker &images, EdgeType mask );

    }

	/* FlowAlgsDefaultSettings
	 * Domyslne wytyczne dla procedur przeplywowych:
	 * czy do wyznaczania maks. przeplywu uzywac FulkersonaForda, czy MKM
	 * czy do wyznaczania najtanszego przeplywu uzywac sciezek powiekszajacych (pseudowielomianowa), czy
	 * cykli z min. srednia dlugoscia (wielomianowa)
	 */
	/** \brief The default settings for flow algorithms.
	 *
	 *  \tparam FF if true the Fulkerson-Ford algorithm will be used, otherwise the MKM algorithm is chosen.
	 *  \tparam costFF if true the augmenting paths are used for calculating the smallest cost of flow (pseudopolynomial),
	 *    otherwise the cycles with minimal average length are used (polynomial).
	 //WEN: to oczywiscie dot. bezposrednio uzywanych procedur na przeplyw oraz ich wewnetrznych wywolan w innych procedurach klas z tego pliku
	 *  \ingroup DMflow*/
	template< bool FF = false, bool costFF = true  > class FlowAlgsDefaultSettings: public AlgsDefaultSettings
	{
	public:
	    //WEN: opis znaczenia tych nazw i skad sie pojawiaja
		enum { useFulkersonFord = FF };
		enum { useCostAugmPath = costFF };
	};


    //NEW: wydzielone z FlowPar
    struct FlowStructs {

		// rekord z danymi (in-out) opisujacy krawedz
		/** \brief Edge information for flow WEN: i ogolniej transship. problems algorithms.*/
		template< class DType, class CType = DType > struct EdgeLabs
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			typedef DType CapacType;/**<\brief Type of capacity and flow variables. */
			// typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
			typedef CType CostType;/**<\brief Type of cost variables.*/

			// przepustowosc (dana wejsciowa), wartosc znalezionego przeplywu w kierunku getEdgeEnd1->getEdgeEnd2
			CapacType capac/**\brief Capacity of edge. WEN: pole wejsciowe, musi miec wartosc >=0 inaczej leci wyjatek z procedury*/,
                    flow/** \brief Actual flow through edge WEN: pole wynikowe, interpetacja: prawdziwy przeplyw
                    przez krawedz (a wiec nieujemny) to jego wartosc bezwzgledna, bo dla Undirected znak flow okresla kierunek
                    + -> od End1 do End2 - -> odwr. */;
			// koszt jednostkowy przeplywu przez luk (dana wejsciowa)
			CostType cost;/**<\brief Cost of unit size flow. WEN: pole wejsciowe ignorowane dla zagadnien niekosztowych */

			// agrs: przepustowosc i koszt krawedzi
			/** \brief Empty constructor.*/
			EdgeLabs():
					capac(NumberTypeBounds< CapacType >::zero() ),
					flow( NumberTypeBounds< CapacType >::zero() ),
					cost( NumberTypeBounds< CostType >::zero() )
				{ }
			/** \brief Constructor.*/
			EdgeLabs( CapacType arg):
					capac( arg ), flow( NumberTypeBounds< CapacType >::zero() ),
					cost(NumberTypeBounds< CostType >::zero())
				{ }
			/** \brief Constructor
			 *
			 *  By default assigns zero capacity and zero cost of unit flow.
			 *  \param arg the capacity of edge.
			 *  \param arg2 the cost of unit flow.*/
			EdgeLabs( CapacType arg, CostType arg2):
					capac( arg ), flow( NumberTypeBounds< CapacType >::zero() ),
					cost( arg2 )
				{ }
		};

		// j.w. ale nadaje domyslne jednostkowe przepustowosci i koszty
		/** \brief Edge information for unit capacity  flow problems algorithms.
		WEN: wszystko jak w poprzedniej, tyle ze capac==1 jest domyslny */
		template< class DType, class CType = DType > struct UnitEdgeLabs
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			typedef DType CapacType;/**<\brief Type of capacity variables.*/
			// typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
			typedef CType CostType;/**<\brief Type of cost variables.*/

			// przepustowosc (dana wejsciowa), wartosc znalezionego przeplywu w kierunku getEdgeEnd1->getEdgeEnd2
			CapacType capac/**\brief Capacity of edge.*/,flow/** \brief Actual flow through edge*/;
			// koszt jednostkowy przeplywu przez luk (dana wejsciowa)
			CostType cost;/**<\brief Cost of unit size flow.*/

			// agrs: przepustowosc i koszt krawedzi
			/**\brief Constructor.*/
			UnitEdgeLabs():
					capac(NumberTypeBounds< CapacType >::one() ),
					flow( NumberTypeBounds< CapacType >::zero() ),
					cost( NumberTypeBounds< CostType >::one() )
				{ }
			UnitEdgeLabs( CapacType arg):
					capac( arg ), flow( NumberTypeBounds< CapacType >::zero() ),
					cost(NumberTypeBounds< CostType >::one())
				{ }
			/** \brief Constructor.
			 *
			 *  By default assigns unit capacity and unit cost of unit flow.
			 *  \param arg the capacity of edge.
			 *  \param arg2 the cost of unit flow.*/
			UnitEdgeLabs( CapacType arg, CostType arg2):
					capac( arg ), flow( NumberTypeBounds< CapacType >::zero() ),
					cost( arg2 )
				{ }
		};

		// rekord wynikowy opisujacy rozciecie krawedziowe w grafie miedzy para wierzcholkow
		/** \brief Output structure for problem of cut-set between two vertices.*/
		template< class CapacType > struct EdgeCut
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			CapacType capac;/**< \brief Capacity type. WEN: type??? nie, objetosc znalezionego rozciecia */
			// liczba wierzcholkow osiagalnych z poczatkowego po usunieciu rozciecia
			int vertNo;/**< \brief Number of vertices reachable WEN: from source! bo cut jest zawsze miedzy pewnymi vers
			 start!=end after deletion of the cut-set.*/
			// liczba krawedzi rozciecia
			int edgeNo;/**<\brief Number of edges in the cut set.*/

			/** \brief Constructor.*/
			EdgeCut(): capac( NumberTypeBounds< CapacType >::zero() ),
						vertNo( 0 ), edgeNo( 0 )
				{ }
		};

		// j.w. ale podaje rowniez 2 wierzcholki - z obu czesci rozcietego grafu
		/** \brief Output structure for cut-set problem. WEN: w przypadku gdy szukamy po wszystkich parach start!=end w grafie*/
		template< class GraphType, class CapacType > struct EdgeCut2: public EdgeCut< CapacType >
		{
			typename GraphType::PVertex first/**\brief Starting vertex*/,second/**\brief Terminal vertex*/;

			EdgeCut2() : first(0), second(0)
			{}
		};

		// Do odczytu rozciecia krawedziowego miedzy para wierzcholkow
		// Uzytkownik podaje, pare iteratorow, gdzie wpisac krawedzie znalezionego rozciecia
		// i wierzcholki osiagalne ze startowego po usunieciu krawedzi rozciecia

		/** \brief Auxiliary class to represent the edge cut. WEN: przejmuje wyniki */
		template< class VIter, class EIter > struct OutCut
		{
			VIter vertIter;/**<\brief Iterator WEN: inserter to the container with vertexes (accessible WEN: ale skad after the cut)*/
			EIter edgeIter;/**<\brief Iterator WEN: inserter to the container with edges of the cat.*/
			/**\brief Constructor*/
			OutCut( VIter av, EIter ei ): vertIter( av ), edgeIter( ei ) { }
		};

		// funkcja tworzaca, analogia make_pair
		// Jesli wyniki nas nie interesuja, zawsze (chyba) mozna podawac BlackHole
		/**\brief Generating function for the OutCut object.
		 *  WEN: a opis argumentow
		 *  [See example](examples/flow/example_Flow.html).
		 */
		template< class VIter, class EIter > static OutCut< VIter,EIter > outCut( VIter av, EIter ei )
			{ return OutCut< VIter,EIter >( av,ei ); }

		// "krawedz" drzewa Gomory-Hu
		/** \brief Auxiliary edge structure for Gomory-Hu trees. WEN: wynikowa*/
		template< class GraphType, class CType > struct GHTreeEdge
		{
			// typ liczbowy przepustowosci luku
			typedef CType CapacType;/**<\brief Capacity type*/
			// koncowki krawedzi
			typename GraphType::PVertex first/**\brief First vertex of GHTree edge*/,second/**\brief Second vertex of GHTree edge*/;
			// przepustowosc "krawedzi"
			CapacType capac;/**< \brief Capacity.*/

			// args: oba wierzcholki koncowe i przepustowosc
			/**\brief Constructor.*/
			GHTreeEdge( ): first( 0 ), second( 0 ),
				capac( NumberTypeBounds< CapacType >::zero() )
				{ }
			/**\brief Constructor.
			 *
			 *  Assigns the both ends of edge and capacity.
			 *  \param f the first vertex of edge.
			 *  \param s the second vertex of edge.
			 *  \param c the capacity of edge.*/
			GHTreeEdge( typename GraphType::PVertex f, typename GraphType::PVertex s, CapacType c  ):
				first( f ), second( s ), capac( c )
				{ }

		};

		// Transshipment - uogolnienie przeplywu (por. Schrijver)
		// rekord wejsciowy opisujacy wierzcholek dla wyszukiwania transhipmentow
		/** \brief The input structure for vertex in transhipment problem. WEN: to jest etykieta przypisywana wierzcholkom na wejsciu */
		template< class DType > struct TrsVertLoss
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			typedef DType CapacType;/**<\brief Capacity type.*/
			// maksymalny i minimalny dopuszczalny przy danym wierzcholku laczny wplyw (tj. bilans wierzcholka na plus)
			//WEN: ponizszy opis jest bez sensu, trans. to uogolnienie przeplywu, w ktorym ten przeplyw moze sie
			//nie bilansowac przy kazdym wierzcholku tj. podajemy max. i min. dopuszczalny bilans wierzcholka
			//(np. w przeplywie wszedzie sa 0 z wyjatkiem start i end.)
			CapacType hi/**\brief Possible exes in vertex*/,lo/**\brief possible deficit in vertex*/;

			//(dopuszczalne dodatnie i ujemne np. przeplyw to trans. z hi=lo=0 wszedzie poza zrodlem i ujsciem)
			/**\brief Constructor.*/
			TrsVertLoss():
				hi(NumberTypeBounds< CapacType >::zero()),
				lo(NumberTypeBounds< CapacType >::zero())
				{ }
			/**\brief Constructor.*/
			TrsVertLoss( CapacType ahi):
				hi(ahi),
				lo( NumberTypeBounds< CapacType >::zero() )
				{ }
			/**\brief Constructor*/
			TrsVertLoss( CapacType alo, CapacType ahi): hi( ahi ), lo( alo )
				 { }
		};

		// rekord  opisujacy krawedz dla wyszukiwania transhipmentow
		/**\brief The input WEN: output tez structure for edge in transhipment problem. WEN: to jest etykieta przypisywana krawedziom na wejsciu*/
		template< class DType, class CType = DType > struct TrsEdgeLabs
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			typedef DType CapacType;/**<\brief Type of capacity, balance and flow variables.*/
			// typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
			typedef CType CostType;/**<\brief Type of cost variables.*/
			// wymagane gorne i dolne ograniczenie na wielkosc przeplywu przez ta krawedz.
			// TODO: sprawdzic, czy moga byc ujemne dla lukow
			//WEN: belkot! jak edge moze miec exec/deficit??? trans. to uogolnienie przeplywu, w ktorym na wejsciu dla
			//krawedzi podajemy max i min dopuszczalna wartosc przeplywu przez nia przechodzacego (np. w przeplywie hi=capac lo=0) oba >=0
			//Te 2 pola sa wejsciowe Dla Undirected wymaga sie lo=0
			CapacType hi/**\brief Possible exes*/,lo/**\brief possible deficit*/;
			// wartosc znalezionego przeplywu (transship.) w kierunku getEdgeEnd1->getEdgeEnd2
			CapacType flow;/**<\brief Actual flow through edge WEN: pole wynikowe */
			// koszt jednostkowy przeplywu dla luku, jesli wymagany to wymagany nieujemny z wyjatkiem petli
			CostType cost;/**<\brief Cost of unit size flow. WEN: pole wejsciowe ignorowane dla problemow nie-kosztowych */

			/** \brief Empty constructor.*/
			TrsEdgeLabs():
				hi(NumberTypeBounds< CapacType >::zero()),
				lo(NumberTypeBounds< CapacType >::zero()),
				cost(NumberTypeBounds< CostType >::zero())
			{ }
			/** \brief Constructor.*/
			TrsEdgeLabs( CapacType ahi):
				hi(ahi ),
				lo( NumberTypeBounds< CapacType >::zero() ),
				cost(NumberTypeBounds< CostType >::zero())
			{ }
			/** \brief Constructor.*/
			TrsEdgeLabs( CapacType alo, CapacType ahi):
				hi( ahi ),
				lo( alo ),
				cost(NumberTypeBounds< CostType >::zero())
			{ }
			/** \brief Constructor.*/
			TrsEdgeLabs( CapacType alo,
				CapacType ahi,
				CostType c): hi( ahi ), lo( alo ), cost( c )
			{ }
		};

    };

	/* FlowPar
	 * Algorytmy znajdowania przeplywu, najwiekszego przeplywu, najtanszego przeplywu, rozciec, transhipments itd.
	 * DefaultStructs - wytyczne dla wewnetrznych procedur
	 */
	/** \brief Flow algorithms (parametrized).
	 *
	 *  The class provides the algorithms for finding flow, maximal flow, minimal cost flow, cuts and solutions for transshipment problem.
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *    See FlowAlgsDefaultSettings and AlgsDefaultSettings.
	 *  \ingroup DMflow */
	template< class DefaultStructs > class FlowPar: public PathStructs, public FlowStructs
	{
//maxFlow
//	maxFlowFF
//		BFSFlow
//			usedCap
//		addFlow
//	maxFlowMKM
//		layerFlow
//			layers
//				-BFSFlow
//			findPot
//				-usedCap
//			onevert
//				push
//					-usedCap
//					addFlow
//				-findPot
//minCostFlow
//	minCostFlowFF
//		BellmanFordFlow
//			usedCapCost
//			costFlow
//	minCostFlowGT
//		minMeanCycle
//			-usedCapCost
//			-costFlow
//		-addFlow
	protected:
		// rekord pomocniczy opisujacy wierzcholek
		template< class GraphType, class CapacType > struct VertLabs
		{
			// do BFS w sieci tymczasowej
			int distance,backdist;
			typename GraphType::PVertex vPrev;
			typename GraphType::PEdge  ePrev;

			// potencjaly przesylu (na uzytek maxFlowMKM)
			CapacType mass,inPot,outPot;
			// czy wierzcholek jest w sieci warstwowej (na uzytek maxFlowMKM)
			bool used;

			VertLabs( typename GraphType::PVertex pv = 0, typename GraphType::PEdge pe = 0,
				int d = std::numeric_limits< int >::max(), int bd = std::numeric_limits< int >::max() ):
					distance( d ), backdist( bd ),vPrev( pv ), ePrev( pe ),
					mass( NumberTypeBounds< CapacType >::zero() ),
					inPot( NumberTypeBounds< CapacType >::plusInfty() ),
					outPot( NumberTypeBounds< CapacType >::plusInfty() ),
					used( false )
			{ }
		};

		// pomocnicza etykieta wierzcholka na uzytek alg. Dijkstry w sieci tymczasowej
		template< class GraphType, class CostType > struct VertLabsCost
		{
			CostType distance;
			typename GraphType::PVertex vPrev;
			typename GraphType::PEdge  ePrev;

			VertLabsCost():
				distance( NumberTypeBounds< CostType >::plusInfty() ),
				vPrev( 0 ), ePrev( 0 )
			{ }
		};

		// przepustowosc uzyteczna krawedzi w sieci tymczasowej z danymi przeplywami
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			usedCap( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PEdge e,
				typename GraphType::PVertex v, bool out );
		// zwiekszanie przeplywu przez krawedz e od delta w kierunku od v lub do v (zmienna out)
		template< class GraphType, class EdgeContainer > static void
			addFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PEdge e,
				typename GraphType::PVertex v, typename EdgeContainer::ValType::CapacType delta, bool out );
		// BFS w sieci tymczasowej przy danych przepustowosciach i przeplywie
		// Sekwencja wyjsciowa wierzcholkow idzie na iter
		// Poruszamy sie "do przodu" lub "do tylu" (out) odpowiednio uzywajac pol distance lub backdistance rekodru
		template< class GraphType, class VertContainer, class EdgeContainer, class Iter > static bool
			BFSFlow( const GraphType &g, EdgeContainer &edgeTab, VertContainer &visited,
				typename GraphType::PVertex first, typename GraphType::PVertex last, bool out, Iter &iter );
		// pomocnicza dla maxFlowMKM
		// identyfikacja wierzcholkow warstw miedzy first a last sieci tymczasowej (pola used, distance, backdistane)
		// zwraca: czy jest przejscie first->last
		template< class GraphType, class VertContainer, class EdgeContainer, class Iter > static bool
			layers( const GraphType &g, EdgeContainer &edgeTab, VertContainer &visited,
				typename GraphType::PVertex first, typename GraphType::PVertex last, Iter &iterout );
		// pomocnicza dla maxFlowMKM
		// wyznaczanie potencjalow przeplywu wierzcholkow w sieci warstwowej miedzy ends
		template< class GraphType, class VertContainer, class EdgeContainer > static void
			findPot( const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab,
				typename GraphType::PVertex fends,typename GraphType::PVertex sends,
				typename GraphType::PVertex v, bool pin, bool pout );
		// na uzytek maxFlowMKM
		// przepchniecie przeplywu z wierzcholka v do przodu lub do tylu
		template< class GraphType, class VertContainer, class EdgeContainer > static void
			push( const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab, typename GraphType::PVertex v,
				bool front );
		// na uzytek maxFlowMKM
		template< class GraphType, class VertContainer, class EdgeContainer > static
			typename EdgeContainer::ValType::CapacType onevert( const GraphType &g, EdgeContainer &edgeTab,
				VertContainer &vertTab, typename GraphType::PVertex *tab, int size,
				typename EdgeContainer::ValType::CapacType limit );
		// na uzytek MKM, znajduje maksymalny (ale nie przekraczajacy limit) przeplyw miedzy start a end
		template< class GraphType, class EdgeContainer, class VertContainer > static
			typename EdgeContainer::ValType::CapacType layerFlow( const GraphType &g, EdgeContainer &edgeTab,
			VertContainer &vertTab, typename GraphType::PVertex start, typename GraphType::PVertex end,
			typename EdgeContainer::ValType::CapacType limit );
		// Znajdowanie maksymalnego (ale nie przekraczajacego limit) przeplywu miedzy start a end
		// Algorytm MKM
		// Zwraca wielkosc przeplywu
		// Parametry: badany graf, tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol capac (wymagane nieujemne),
		// wyniki zwracane do pol flow. gorny limit wielkosci znalezionego przeplywu
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			maxFlowMKM( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType limit );
		// Znajdowanie maksymalnego (ale nie przekraczajacego limit) przeplywu miedzy start a end
		// Algorytm FulkersonaFodra
		// Zwraca wielkosc przeplywu. Parametry: badany graf, tablica asocjacyjna PEdge->EdgeLabs. Korzystamy z pol
		// capac (wymagane nieujemne), wyniki zwracane do pol flow, gorny limit wielkosci znalezionego przeplywu
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			maxFlowFF( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType limit );
		// podobna do usedCap na uzytek przeplywow z kosztem
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			usedCapCost( const GraphType &g, EdgeContainer& edgeTab, typename GraphType::PEdge e,
				typename GraphType::PVertex v );
		// koszt jednostkowy przeplywu przez krawedz od danej koncowki w sieci tymczasowej
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CostType
			costFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PEdge e,
				typename GraphType::PVertex v );

		// Bellman-Ford w sieci tymczasowej z kosztami lukow skorygowanymi przez potencjaly wierzcholkow
		// Zwraca: czy istnieje przejscie start->end
		template< class GraphType, class VertContainer, class EdgeContainer > static bool
			BellmanFordFlow( const GraphType &g, EdgeContainer &edgeTab, VertContainer &vertTab,
				typename GraphType::PVertex start, typename GraphType::PVertex end );
		// znajdowanie w sieci tymczasowej cyklu o najmniejszej sredniej dlugosci krawedzi
		// pominiecie wierzcholka koncowego: liczymy odleglosci ze start do wszystkich wierzcholkow
		// TODO: nie testowane, sprawdzic!
		template< class GraphType, class EdgeContainer, class EIter, class VIter >
			static int minMeanCycle( const GraphType &g, EdgeContainer &edgeTab, OutPath< VIter,EIter > iters );
		// znajdowanie przeplywu start->end o maksymalnej objetosci (ale nie przekraczajacej limitu val) i najmniejszym koszcie
		// procedura pseudowielomianowa (sciezki powiekszajace)
		// zwraca pare wielkosc przeplywu
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			minCostFlowFF( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType val =
				 NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty() );

		// procedura o interfejsie j.w. ale wielomianowa
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			minCostFlowGT( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType val =
				NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty() );

		// TODO: nieefektywna, zrezygnowac z Setow
		template< class GraphType, class EdgeContainer, class AssocSub >
			static void ghtree( GraphType &g, EdgeContainer &edgeTab,
				Set< typename GraphType::PVertex > &V, Set< typename GraphType::PVertex > &R,
				GHTreeEdge< GraphType,typename EdgeContainer::ValType::CapacType > *out, AssocSub& vsub );


	public:
		// objetosc przeplywu przy wierzcholku v
		// type=EdDirOut - objetosc wyplywajaca, type=EdDirIn - objetosc wplywajaca, type=EdUndir - bilans wyplywajacy
		/** \brief Get flow through vertex.
		 *
		 *  The method extracts the size of flow in a vertex from the associative container edgeTab. WEN: w zasadzie to nie musi byc flow, cokolwiek
		 po prostu pola flow w etykietach nie musza sie bilansowac
		 *  \param[in] g the considered graph.
		 *  \param edgeTab the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (keeping: capacity, flow and cost
            WEN: nie tak, sa w tej klasie 3 struktury labelow krawedzi, wymienic ktore ) to each edge.
		 *  \param[in] v the considers vertex.
		 *  \param[in] type the flag decides about the type of considered flow:
		 *   - EdDirOut - outflow, WEN: z uwzg. ew. krazacego w petlach
		 *   - EdDirIn - inflow, WEN: z uwzg. ew. krazacego w petlach
		 *   - EdUndir - flow balance. WEN: tj. powyzszy EdDirOut - pow. EdDirIn WEN: inne wartosci zakazane
		 *  \return the size of the flow. */
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			vertFlow( const GraphType &g, const EdgeContainer &edgeTab, typename GraphType::PVertex v,
				EdgeDirection type = EdUndir );

		// sprawdzanie poprawnosci przeplywu S->T
		/** \brief Test flow.
		 *
		 *  The method test if the flow given by the associative container edgeTab is a proper flow from \a S to \a T for graph \a g.
		 *  \param[in] g the reference graph.
		 *  \param[in] edgeTab the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (WEN: nie tak. Jakie? por. wyzej keeping: capacity, flow and cost) to each edge.
		 *  \param[in] S the starting vertex.
		 *  \param[in] T the termianl vertex.
		 *  \return true if the \a edgeTab is an appropriate flow, false otherwise. WEN: tj. vertFlow==0 wszedzie moze poza S i T, capac nie sa naruszone,  */
		template< class GraphType, class EdgeContainer > static bool testFlow( const GraphType &g,
			const EdgeContainer &edgeTab, typename GraphType::PVertex S, typename GraphType::PVertex T );

		// znajduje maksymalny przeplyw start->end (ale nie wiekszy, niz limit)
		// zwraca jego wielkosc
		/** \brief Get maximal flow.
		 *
		 *  For a given graph, the method calculates the maximal (WEN: ta akurat tylko max or at least \a limit size) flow from \a start to \a end.
		 *  \param[in] g the considered graph.
		 *  \param[out] edgeTab  the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (WEN: zle, jw. keeping: capacity, flow and cost)
            to each edge. Array provides both input (capacity) and output (flow) data.
		 *  \param[in] start the starting vertex.
		 *  \param[in] end the terminal vertex.
		 *  \return the size of the achieved flow.
		 *
		 *  [See example](examples/flow/example_Flow.html).
		 */
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			maxFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end)
			{
				return maxFlow(g, edgeTab, start, end,
                        NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty() );
			}
		/** \brief Get maximal flow.
		 *
		 *  For a given graph, the method calculates the maximal (or at least \a limit size) flow from \a start to \a end.
		 *  \param[in] g the considered graph.
		 *  \param[out] edgeTab  the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (WEN: por. wyzej keeping: capacity, flow and cost) to each edge. Array provides both input (capacity) and output (flow) data.
		 *  \param[in] start the starting vertex.
		 *  \param[in] end the terminal vertex.
		 *  \param[in] limit the upper bound of flow size. After reaching the limit WEN: lub maks. mozliwa wartosc the method terminates. If default of infinity then the maximal flow is searched.
            WEN: parametr ten musi byc >=0
		 *  \return the size of the achieved flow.         */
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CapacType
			maxFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType limit);
		//NEW:
		// zwraca koszt podanego przeplywu lub transship.
		template< class GraphType, class EdgeContainer > static typename EdgeContainer::ValType::CostType
			flowCost( const GraphType &g, const EdgeContainer &edgeTab );

		// znajduje najtanszy przeplyw start->end o maks. wielkosci (ale nie wiekszy, niz val)
		// zwraca (jego koszt, wielkosc)
		/** \brief Get minimal cost flow.
		 *
		 *  The method gets the minimum cost flow for the graph \a g WEN: ale wsrod tych o maks. objetosci.
		 *  \param[in] g the reference graph.
		 *  \param[out] edgeTab the the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (WEN: jw.  keeping: capacity, flow and cost) to each edge. Array provides both input (capacity) and output (flow) data.
		 *  \param[in] start the starting vertex.
		 *  \param[in] end the terminal vertex.
		 *  \return the standard pair (cost, size) of the achieved flow.
		 *
		 *  [See example](examples/flow/example_Flow.html).
		 */
		template< class GraphType, class EdgeContainer > static
			std::pair< typename EdgeContainer::ValType::CostType,typename EdgeContainer::ValType::CapacType >
			minCostFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end)
			{
				return minCostFlow(g, edgeTab, start, end,
                       NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty() );
			}
		/** \brief Get minimal cost flow.
		 *
		 *  The method gets the minimum cost flow for the graph \a g .
		 *  \param[in] g the reference graph.
		 *  \param[out] edgeTab the the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (WEN: jw. keeping: capacity, flow and cost) to each edge. Array provides both input (capacity) and output (flow) data.
		 *  \param[in] start the starting vertex.
		 *  \param[in] end the terminal vertex.
		 *  \param[in] val the upper bound of flow size. WEN: musi byc >=0. After reaching the limit \a val WEN: lub maks mozliwa wartosc the method terminates. If default of infinity then the maximal flow is searched.
		 *  \return the standard pair (cost, size) of the achieved flow.         */
		template< class GraphType, class EdgeContainer > static
			std::pair< typename EdgeContainer::ValType::CostType,typename EdgeContainer::ValType::CapacType >
			minCostFlow( const GraphType &g, EdgeContainer &edgeTab, typename GraphType::PVertex start,
				typename GraphType::PVertex end, typename EdgeContainer::ValType::CapacType val);

		// znajdowanie minimalnego (pod wzgledem objetosci) rozciecia krawedziowego start-end
		/** \brief Get minimal weighted cut-set.
		 *
		 *  The method calculated the minimal (concerning WEN: sumaryczna the capacities) cut-set between vertices \a start and \a end.
		 *  \param[in] g the reference graph.
		 *  \param[in] edgeTab the the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (keeping: capacity, flow and cost) to each edge.
		 *  \param[in] start the starting vertex.
		 *  \param[in] end the terminal vertex.
		 *  \param[out] iters the pair of iterators WEN: insertery to the containers with the reachable WEN: from start vertices (after subtraction of cut) and the edges of output  cut-set.
		 *  \return the EdgeCut structure, which keeps the size of cut set WEN: i jego tj. minimalna mozliwa capacity and the number of reachable vertices.
		 *
		 *  [See example](examples/flow/example_Flow.html).
		 */
		template< class GraphType, class EdgeContainer, class VIter, class EIter > static
			EdgeCut< typename EdgeContainer::ValType::CapacType > minEdgeCut( const GraphType &g, EdgeContainer &edgeTab,
				typename GraphType::PVertex start, typename GraphType::PVertex end, OutCut< VIter,EIter > iters )
				// Implementacja przeniesiona do czesci definicyjnej ze wzgledu na bledy kompilatorow VS <2010
				{
					EdgeCut< typename EdgeContainer::ValType::CapacType > res;
					typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
						VertLabs< GraphType,typename EdgeContainer::ValType::CapacType > >::Type vertTab( g.getVertNo() );
					res.capac = maxFlow( g,edgeTab,start,end );
					BFSFlow( g,edgeTab,vertTab,start,end,true,blackHole );
					for( typename GraphType::PVertex v = g.getVert(); v; v = g.getVertNext( v ) )
						if (std::numeric_limits< int >::max() > vertTab[v].distance)
						{
							res.vertNo++;
							if (!isBlackHole( iters.vertIter ))
							{
								*iters.vertIter = v;
								++iters.vertIter;
							}
							for( typename GraphType::PEdge e = g.getEdge( v,EdDirOut | EdUndir ); e;
								e = g.getEdgeNext( v,e,EdDirOut | EdUndir ) )
								if (vertTab[g.getEdgeEnd( e,v )].distance == std::numeric_limits< int >::max())
								{
									res.edgeNo++;
									if (!isBlackHole( iters.edgeIter ))
									{
										*iters.edgeIter = e;
										++iters.edgeIter;
									}
							}
						}
					return res;
				}
		// j.w. ale szuka najmniejszego rozciecia miedzy kazda para wierzcholkow (zwracane w polach first, second)
		/** \brief Get minimal weighted cut-set.
		 *  WEN: jw.
		 *  The method calculated the minimal (concerning the capacities) cut-set of graph.
		 *  \param[in] g the reference graph. WEN: co najmniej 2-wierzcholkowy
		 *  \param[in] edgeTab the associative array (PEdge -> EdgeLabs) which assigns EdgeLabs structure (keeping: capacity, flow and cost) to each edge.
		 *  \param[out] iters the pair of iterators to the containers with the reachable vertices (after subtraction of cut) and the edges of output  cut-set.
		 *  \return the EdgeCut2 structure whit the size of minimal cut-set, the number of reachable vertices and the stating and WEN: sa rozne terminating points. */
		template< class GraphType, class EdgeContainer, class VIter, class EIter > static
			EdgeCut2< GraphType,typename EdgeContainer::ValType::CapacType > minEdgeCut( const GraphType &g,
				EdgeContainer &edgeTab, OutCut< VIter,EIter > iters )
				// Implementacja przeniesiona do czesci definicyjnej ze wzgledu na bledy kompilatorow VS <2010
				{
					int n,m;
					koalaAssert( g.getVertNo() >= 2,AlgExcWrongArg );
					EdgeCut< typename EdgeContainer::ValType::CapacType > res,buf;
					typename GraphType::PVertex a,b;
					typename GraphType::PVertex LOCALARRAY( vres,(n = g.getVertNo()) - 1 );
					typename GraphType::PVertex LOCALARRAY( vbuf,n - 1 );
					typename GraphType::PEdge LOCALARRAY( eres,m = g.getEdgeNo(Directed | Undirected) );
					typename GraphType::PEdge LOCALARRAY( ebuf,m );
					res.capac = NumberTypeBounds< typename EdgeContainer::ValType::CapacType >::plusInfty();

					for( typename GraphType::PVertex s = g.getVert(); s != g.getVertLast(); s = g.getVertNext( s ) )
						for( typename GraphType::PVertex t = g.getVertNext( s ); t; t = g.getVertNext( t ) )
						{
							if (isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
								buf = minEdgeCut( g,edgeTab,s,t,outCut( blackHole,blackHole ) );
							else if (isBlackHole( iters.vertIter ) && !isBlackHole( iters.edgeIter ))
								buf = minEdgeCut( g,edgeTab,s,t,outCut( blackHole,ebuf ) );
							else if (!isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
								buf = minEdgeCut( g,edgeTab,s,t,outCut( vbuf,blackHole ) );
							else buf = minEdgeCut( g,edgeTab,s,t,outCut( vbuf,ebuf ) );
							if (buf.capac < res.capac)
							{
								res = buf;
								a = s;
								b = t;
								if (!isBlackHole( iters.vertIter ))
									for( int i = 0; i < buf.vertNo; i++ ) vres[i] = vbuf[i];
								if (!isBlackHole( iters.edgeIter ))
									for( int i = 0; i < buf.edgeNo; i++ ) eres[i] = ebuf[i];
							}
							if (g.getEdgeNo( EdDirIn | EdDirOut ))
							{
								if (isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
									buf = minEdgeCut( g,edgeTab,t,s,outCut( blackHole,blackHole ) );
								else if (isBlackHole( iters.vertIter ) && !isBlackHole( iters.edgeIter ))
									buf = minEdgeCut( g,edgeTab,t,s,outCut( blackHole,ebuf ) );
								else if (!isBlackHole( iters.vertIter ) && isBlackHole( iters.edgeIter ))
									buf = minEdgeCut( g,edgeTab,t,s,outCut( vbuf,blackHole ) );
								else buf = minEdgeCut( g,edgeTab,t,s,outCut( vbuf,ebuf ) );
								if (buf.capac < res.capac)
								{
									res = buf;
									a = t;
									b = s;
									if (!isBlackHole( iters.vertIter ))
										for( int i = 0; i < buf.vertNo; i++ ) vres[i] = vbuf[i];
									if (!isBlackHole( iters.edgeIter ))
										for( int i = 0; i < buf.edgeNo; i++ ) eres[i] = ebuf[i];
								}
							}
						}
					if (!isBlackHole( iters.vertIter ))
						for( int i = 0; i < res.vertNo; i++ )
						{
							*iters.vertIter = vres[i];
							++iters.vertIter;
						}
					if (!isBlackHole( iters.edgeIter ))
						for( int i = 0; i < res.edgeNo; i++ )
						{
							*iters.edgeIter = eres[i];
							++iters.edgeIter;
						}
					EdgeCut2< GraphType,typename EdgeContainer::ValType::CapacType > res2;
					res2.capac = res.capac;
					res2.edgeNo = res.edgeNo;
					res2.vertNo = res.vertNo;
					res2.first = a;
					res2.second = b;
					return res2;
				}
		// szuka transship. w grafie o podanych warunkach na wierzcholki i krawedzie
		// zwraca false w razie braku
		/** \brief Solve transshipment problem WEN: tzn?
		 *
		 *  \param[in] g the considered graph.
		 *  \param[out] edgeTab the associative array (PEdge -> EdgeLabs) which assigns EdgeLabs structure (keeping: capacity, flow and cost WEN: tutaj to takze hi i lo, ktory konkretnie label ze zdef. struktur?) to each edge. Both input and output data are sent via this array.
		 *  \param[in] vertTab the associative array (PVert -> TrsVertLoss) which assigns TrsVertLoss structure (keeping: maximal excess and deficit) to each vertex.
		 *  \return true if transshipment was found, false otherwise.*/
		template< class GraphType, class EdgeContainer, class VertContainer > static bool transship( GraphType &g,
			EdgeContainer &edgeTab, const VertContainer &vertTab );

        //NEW: wersja ogolniejsza, dla wierzcholkow o bilansie 0 mozna definiowac wymagania odnosnie przeplywu przez
        // ten wierzcholek, analogicznie jak dla krawedzi tj. vertTab2: PVertex ->TrsEdgeLabs
        //Podobnie jest z minCostTraship
		template< class GraphType, class EdgeContainer, class VertContainer, class VertContainer2  >
            static bool transship(const GraphType &g,
			EdgeContainer &edgeTab, const VertContainer &vertTab,  VertContainer2 &vertTab2);


		// szuka najtanszego transship. w grafie o podanych warunkach na wierzcholki i krawedzie
		// zwraca jego koszt lub nieskonczonosc w razie braku
		/** \brief Solve transshipment problem WEN: ale kosztowy
		 *  WEN: jw.
		 *  The method finds minimum cost transshipment problem for a given graph and initial constraints (on edges and vertices).
		 *  \param[in] g the considered graph.
		 *  \param[out] edgeTab the associative array (PEdge -> EdgeLabs) which assigns EdgeLabs structure (keeping: capacity, flow and cost) to each edge. Both input and output data are sent via this array.
		 *  \param[in] vertTab the associative array (PVert -> TrsVertLoss) which assigns TrsVertLoss structure (keeping: maximal excess and deficit) to each vertex.
		 *  \return the cost of achieved transshipment or infinity if there isn't any.*/
		template< class GraphType, class EdgeContainer, class VertContainer > static
			typename EdgeContainer::ValType::CostType minCostTransship( GraphType &g, EdgeContainer &edgeTab,
				const VertContainer &vertTab );

        //NEW: wersja ogolniejsza, por. wyzej
        template< class GraphType, class EdgeContainer, class VertContainer, class VertContainer2 > static
			typename EdgeContainer::ValType::CostType minCostTransship(const GraphType &g, EdgeContainer &edgeTab,
				const VertContainer &vertTab,  VertContainer2 &vertTab2 );

		// znajduje drzewo Gomory-Hu grafu
		/** \brief Get Gomory-Hu tree.
		 *
		 *  The method calculates the Gomory-Hu tree of undirected graph \a g.
		 *  \param g the considered graph., WEN: musi byc undirected ew. z petlami, bo one nie maja znaczenia, z n>1
		 *  \param edgeTab  the the associative table (PEdge -> EdgeLabs) which assigns EdgeLabs structure (keeping: capacity, flow and cost, WEN: czyli ktore? - tu liczy sie tylko pole wejsciowe capacity) to each edge.
		 *  \param out the iterator of the container with output edges of Gomory-Hu tree in form of GHTreeEdge. WEN: dokladnie inserter na te struktury
                There is exactly n-1 of them.*/
		template< class GraphType, class EdgeContainer, class IterOut > static void findGHTree( GraphType &g,
			EdgeContainer& edgeTab, IterOut out );
	};

	// wersja dzialajaca na DefaultStructs=FlowAlgsDefaultSettings
	/** \brief Flow algorithms (default).
	 *
	 *  The class provides the algorithms for finding flow, maximal flow, minimal cost flow, cuts and sollutions for transshipment problem.
	 *  Simpler version (WEN: bo dziala na domyslnych wytycznych z wyjatkiem jednej zmienialnej flagi) of the class FlowPar in which only type of flow algorithm and costflow algorithm can be chosen. WEN: no wlasnie ten drugi jest nie do zmiany!
	 *  \tparam FF if true the Fulkerson-Ford algirithm will be used, otherwise the MKM algorithm is choosen.
	 *  \tparam WEN: bzdura, tu jest ustawione na domyslny we FlowAlgsDefaultSettings tj. true, przeciez jest tylko jeden param szablonu
        costFF if treu the augmenting paths are used for calculating the smalest cost of flow (pseudoplynomial). If false???
	 *  \ingroup DMflow */
	template< bool FF > class FlowPar2: public FlowPar< FlowAlgsDefaultSettings< FF > > { };

	// i z domyslnyna flaga wyboru algorytmow
	/** \brief Flow algorithms (default).
	 *
	 *  The class provides the algorithms for finding flow, maximal flow, minimal cost flow, cuts and sollutions for transshipment problem.
	 *  Simpler version (WEN: bo dziala na domyslnych wytycznych) of the class FlowPar in which MKM and augmenting paths algorithms are used.
	 *  \ingroup DMflow */
	class Flow: public FlowPar2< false > { };

	//NEW: wydzielone z ConnectPar
	struct ConnectStructs {

		// rekord wyjsciowy opisujacy rozciecie krawedziowe w grafie
		/** \brief The output structure for edge cut problem in graph.*/
		template< class GraphType > struct EdgeCut
		{
			// dwa wierzcholki po obu stronach rozciecia
			typename GraphType::PVertex first/**\brief Vertex on one side of cut WEN: startowy*/,second/**\brief Vertex on the other side of cut WEN: koncowy - wszak problem nie jest symetryczny w nieskierowanych*/;
			// liczba krawedzi
			int edgeNo;/**\brief The number of edges in the cut set.*/
			/**\brief Constructor.*/
			EdgeCut() : first(0), second(0), edgeNo(0)
				{ }
		};

	};

	/* ConnectPar
	 * Procedury badania spojnosci grafu (bez wag na wierz/kraw)
	 * DefaultStructs - wytyczne dla wewnetrznych procedur
	 */
	/** \brief Connectivity testing algorithms (parametrized).
	 *
	 *  The class consists of some methods calculating and testing connectivity.
	 *  \tparam DefaultStructs the class decides about the basic structures and algorithm. Can be used to parametrize algorithms.
	 *    See FlowAlgsDefaultSettings and AlgsDefaultSettings.
	 *  \ingroup DMconnect
	 */
	template< class DefaultStructs > class ConnectPar: public SearchStructs, public ConnectStructs
	//WEN: oczywiscie slowa path skas gdzies w dokumentacji do jej metod rozumiemy jako marszruty bez powtorzen uwzgledniajace orientacje przechodzonych krawedzi
	{
	protected:
		struct EdgeLabs
		{
			int capac,flow;

			EdgeLabs(): capac( 1 ), flow( 0 )
				{ }
		};

	public:
		// znajduje najmniejsze rozciecie krawedziowe miedzy start->end
		// zwraca jego wielkosc, krawedzie sa wypisywane na iter
		/** \brief Get minimal cut-set.
		 *
		 *  The method gets the minimum cut-set between vertices \a star and \a end.
		 *  \param[in] g the considered graph.
		 *  \param[in] start the first (starting) reference vertex.
		 *  \param[in] end the second (terminal) reference vertex.
		 *  \param[out] iter the iterator WEN: inserter of the container with all the edges of the cut-set.
		 *  \return the size of minimal cut. */
		template< class GraphType, class EIter > static int minEdgeCut( const GraphType &g,
			typename GraphType::PVertex start, typename GraphType::PVertex end, EIter iter );

		// znajduje najmniejsze rozciecie krawedziowe miedzy para roznych wierzcholkow
		// krawedzie sa wypisywane na iter
		/** \brief Get minimal cut-set.
		 *
		 *  The method gets the minimum cut-set in the graph.
		 *  \param[in] g the considered graph.
		 *  \param[out] iter the iterator WEN: inserter of the container with all the edges of the cut.
		 *  \return the EdgeCut structure, which keeps the number of edges in the cut and one vertex on the first side of cut and one on the other side. WEN: rozne */
		template< class GraphType, class EIter > static EdgeCut< GraphType > minEdgeCut( const GraphType &g, EIter iter )
		// Implementacja przeniesiona do czesci definicyjnej ze wzgledu na bledy kompilatorow VS <2010
		{
			EdgeCut< GraphType > res;
			typename DefaultStructs:: template AssocCont< typename GraphType::PEdge,
				typename FlowPar< DefaultStructs >:: template EdgeLabs< int > >::Type edgeLabs( g.getEdgeNo() );
			for( typename GraphType::PEdge e = g.getEdge(); e; e = g.getEdgeNext( e ) ) edgeLabs[e].capac = 1;
			typename FlowPar< DefaultStructs >:: template EdgeCut2< GraphType,int > res2 =
				FlowPar< DefaultStructs >:: template minEdgeCut( g,edgeLabs,FlowPar< DefaultStructs >::template
					outCut( blackHole,iter ) );
			res.edgeNo = res2.capac;
			res.first = res2.first;
			res.second = res2.second;
			return res;
		}
		// znajduje najwiekszy zbior krawedziowo rozlaczych sciezek start->end
		// zwraca ich liczbe. Parametry: badany graf,
		// iteratory wyjsciowe na kolejne wierz znalezionych sciezek
		// iteratory wyjsciowe na kolejne kraw znalezionych sciezek
		/** \brief Get set of edge disjointed paths.
		 * WEN: zgodnie z tw. Mengele (czy jak mu tam bylo) ich liczba odpowiada wynikowi z odpowiedniego minEdgeCut
		 *  The method gets the maximal set of edge disjoint paths between vertices \a start and \a end.
		 *  \param[in] g the considered graph.
		 *  \param[in] start the first (starting) reference vertex.
		 *  \param[in] end the second (terminal) reference vertex.
		 *  \param[out] voutiter a SearchStructs::CompStore object that keeps the output paths in the form of vertex sequences.
		 *  \param[out] eoutiter a SearchStructs::CompStore object that keeps the output paths in the form of edge sequences.
		 *  \return the number of edge disjont paths between \a start and \a end. */
		template< class GraphType, class VIter, class EIter, class LenIterV, class LenIterE > static int
			edgeDisjPaths( GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
			CompStore< LenIterV,VIter > voutiter, CompStore< LenIterE,EIter > eoutiter );

        //NEW: wersje bez wybranych CompStorow
		template< class GraphType, class EIter, class LenIterE > static int
			edgeDisjPaths( GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
			BlackHole, CompStore< LenIterE,EIter > eoutiter )
        {   return edgeDisjPaths(g,start,end,CompStore< BlackHole,BlackHole>( blackHole,blackHole ),eoutiter); }

        template< class GraphType, class VIter,  class LenIterV > static int
			edgeDisjPaths( GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
			CompStore< LenIterV,VIter > voutiter, BlackHole )
        {   return edgeDisjPaths(g,start,end,voutiter,CompStore< BlackHole,BlackHole>( blackHole,blackHole )); }

        template< class GraphType> static int
			edgeDisjPaths( GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end, BlackHole, BlackHole )
        {   return edgeDisjPaths(g,start,end,CompStore< BlackHole,BlackHole>( blackHole,blackHole ),CompStore< BlackHole,BlackHole>( blackHole,blackHole )); }


		// znajduje najmniejsze rozciecie wierzcholkowe miedzy start->end
		// zwraca jego wielkosc (-1 w razie braku), wierzcholki sa wypisywane na iter
		/** \brief Find smallest vertex cut.
		 *
		 *  The method finds the smallest vertex cut separating vertices \a start and \a end. WEN: a wiec nie zawierajace tych dwoch
		 *  \param[in] g the considered graph.
		 *  \param[in] start the first (starting) reference vertex.
		 *  \param[in] end the second (terminal) reference vertex.
		 *  \param[out] iter the iterator WEN: inserter of the container with the vertices of vertex cut.
		 *  \return the number of vertices in the cut or -1 if there isn't any WEN bo jest krawedz EdDirOut | EdUndir ze start do end. */
		template< class GraphType, class VIter > static int minVertCut( const GraphType &g,
			typename GraphType::PVertex start, typename GraphType::PVertex end, VIter iter );

		// znajduje najmniejsze rozciecie wierzcholkowe w grafie
		// zwraca jego wielkosc (-1 w razie braku), wierzcholki sa wypisywane na iter
		/** \brief Find smallest vertex cut.
		 *
		 *  The method finds the smallest vertex cut. WEN: po wszystkich parach start != end
		 *  \param[in] g the considered graph.
		 *  \param[out] iter the iterator of the container with the vertices of vertex cut.
		 *  \return the number of vertices in the cut or -1 if there isn't any.*/
		template< class GraphType, class VIter > static int minVertCut( const GraphType &g, VIter iter );

		// znajduje najwiekszy zbior wewnetrznie wierzcholkowo rozlaczych sciezek start->end
		// zwraca ich liczbe
		/** \brief Get set of vertex disjointed paths.
		 * WEN: naruszamy tw. Mengele (czy jak mu tam bylo): tu zezwanamy na bezposrednie krawedzie start->end typu EdUndir|EdDirOut i kazda z nich rowniez zaliczamy
		 jako wynikowa sciezke
		 *  The method gets the maximal set of vertex disjoint WEN: internally paths between vertices \a start and \a end
		 *    (obviously vertices \a start and \a end are shared for all paths).
		 *  \param[in] g the considered graph.
		 *  \param[in] start the first (starting) reference vertex.
		 *  \param[in] end the second (terminal) reference vertex.
		 *  \param[out] voutiter a SearchStructs::CompStore object that keeps the output path in the form of vertex sequences.
		 *  \param[out] eoutiter a SearchStructs::CompStore object that keeps the output path in the form of edge sequences.
		 *  \return the number of vertex disjont paths between \a start and \a end.*/
		template< class GraphType, class VIter, class EIter, class LenIterV, class LenIterE > static int
			vertDisjPaths( const GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
			CompStore< LenIterV,VIter > voutiter, CompStore< LenIterE,EIter > eoutiter );

        //NEW: wersje bez wybranych CompStorow
		template< class GraphType, class EIter, class LenIterE > static int
			vertDisjPaths( const GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
			BlackHole, CompStore< LenIterE,EIter > eoutiter )
        {   return vertDisjPaths(g,start,end,CompStore< BlackHole,BlackHole>( blackHole,blackHole ),eoutiter); }

        template< class GraphType, class VIter,  class LenIterV > static int
			vertDisjPaths( const GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end,
			CompStore< LenIterV,VIter > voutiter, BlackHole )
        {   return vertDisjPaths(g,start,end,voutiter,CompStore< BlackHole,BlackHole>( blackHole,blackHole )); }

        template< class GraphType> static int
			vertDisjPaths( const GraphType &g, typename GraphType::PVertex start, typename GraphType::PVertex end, BlackHole, BlackHole )
        {   return vertDisjPaths(g,start,end,CompStore< BlackHole,BlackHole>( blackHole,blackHole ),CompStore< BlackHole,BlackHole>( blackHole,blackHole )); }

	};

	// wersja dzialajaca na DefaultStructs=FlowAlgsDefaultSettings<false, true>
	/** \brief Connectivity testing algorithms (default).
	 *
	 *  The class consists of some methods calculating connectivity.
	 *  Simpler version of the class ConnectPar in which MKM and augmenting paths algorithms are used by default. WEN: a druga flaga wyboru procedury jest true
	 *  \ingroup DMconnect */
	class Connect: public ConnectPar< FlowAlgsDefaultSettings< false > > { };

#include "conflow.hpp"
}

#endif
