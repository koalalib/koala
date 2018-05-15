#include <iostream>
//#include <cstdint> - no i wlasnie nie kompiluje sie w standardzie ponizej 2011 :-(
#include <string>
#include <cstdio>

#define KOALA_SET_ON_HASHSET

#include "Koala/graph/graph.h"
#include "Koala/algorithm/conflow.h"
#include "Koala/io/text.h"

#define int16_t		short int

using namespace std;
using namespace Koala;

		template< class DType, class CType = DType > struct EdgeLabs
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			typedef DType CapacType;/**<\brief Type of capacity and flow variables.*/
			// typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
			typedef CType CostType;/**<\brief Type of cost variables.*/

			// przepustowosc (dana wejsciowa), wartosc znalezionego przeplywu w kierunku getEdgeEnd1->getEdgeEnd2
			CapacType capac/**\brief Capacity of edge.*/,flow/** \brief Actual flow through edge*/;
			// koszt jednostkowy przeplywu przez luk (dana wejsciowa)
			CostType cost;/**<\brief Cost of unit size flow.*/

			// agrs: przepustowosc i koszt krawedzi
			/** \brief Empty constructor.*/
			EdgeLabs():
					capac(0 ),
					flow( 0 ),
					cost( 0 )
				{ }
			/** \brief Constructor.*/
			EdgeLabs( CapacType arg):
					capac( arg ), flow( 0 ),
					cost(0)
				{ }
			/** \brief Constructor
			 *
			 *  By default assigns zero capacity and zero cost of unit flow.
			 *  \param arg the capacity of edge.
			 *  \param arg2 the cost of unit flow.*/
			EdgeLabs( CapacType arg, CostType arg2):
					capac( arg ), flow( 0 ),
					cost( arg2 )
				{ }
		};

		// j.w. ale nadaje domyslne jednostkowe przepustowosci i koszty
		/** \brief Edge information for unit capacity  flow problems algorithms.*/
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
					capac(1 ),
					flow( 0 ),
					cost( 1 )
				{ }
			UnitEdgeLabs( CapacType arg):
					capac( arg ), flow( 0) ,
					cost(1)
				{ }
			/** \brief Constructor.
			 *
			 *  By default assigns unit capacity and unit cost of unit flow.
			 *  \param arg the capacity of edge.
			 *  \param arg2 the cost of unit flow.*/
			UnitEdgeLabs( CapacType arg, CostType arg2):
					capac( arg ), flow( 0 ),
					cost( arg2 )
				{ }
		};

		// rekord wynikowy opisujacy rozciecie krawedziowe w grafie miedzy para wierzcholkow
		/** \brief Output structure for problem of cut-set between two vertices.*/
		template< class CapacType > struct EdgeCut
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			CapacType capac;/**< \brief Capacity type.*/
			// liczba wierzcholkow osiagalnych z poczatkowego po usunieciu rozciecia
			int vertNo;/**< \brief Number of vertices reachable after deletion of the cut-set.*/
			// liczba krawedzi rozciecia
			int edgeNo;/**<\brief Number of edges in the cut set.*/

			/** \brief Constructor.*/
			EdgeCut(): capac( 0 ),
						vertNo( 0 ), edgeNo( 0 )
				{ }
		};

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
				capac( 0 )
				{ }
			/**\brief Constructor.*/
			GHTreeEdge( typename GraphType::PVertex f ): first( f ), second( 0 ),
				capac( 0 )
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
		/** \brief The input structure for vertex in transhipment problem.*/
		template< class DType > struct TrsVertLoss
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			typedef DType CapacType;/**<\brief Capacity type.*/
			// maksymalny i minimalny dopuszczalny przy danym wierzcholku laczny wplyw
			CapacType hi/**\brief Possible exes in vertex*/,lo/**\brief possible deficit in vertex*/;

			//(dopuszczalne dodatnie i ujemne np. przeplyw to trans. z hi=lo=0 wszedzie poza zrodlem i ujsciem)
			/**\brief Constructor.*/
			TrsVertLoss():
				hi(0),
				lo(0)
				{ }
			/**\brief Constructor.*/
			TrsVertLoss( CapacType alo):
				hi(0),
				lo( alo )
				{ }
			/**\brief Constructor*/
			TrsVertLoss( CapacType alo, CapacType ahi): hi( ahi ), lo( alo )
				 { }
		};

		// rekord  opisujacy krawedz dla wyszukiwania transhipmentow
		/**\brief The input structure for edge in transhipment problem.*/
		template< class DType, class CType = DType > struct TrsEdgeLabs
		{
			// typ liczbowy przepustowosci luku i objetosci przeplywu
			typedef DType CapacType;/**<\brief Type of capacity, balance and flow variables.*/
			// typ kosztu jednostkowego przeplywu przez luk i kosztu przeplywu
			typedef CType CostType;/**<\brief Type of cost variables.*/
			// wymagane gorne i dolne ograniczenie na wielkosc przeplywu przez ta krawedz.
			//Dla nieskierowanych wymaga sie lo=0
			// TODO: sprawdzic, czy moga byc ujemne dla lukow
			CapacType hi/**\brief Possible exes*/,lo/**\brief possible deficit*/;
			// wartosc znalezionego przeplywu (transship.) w kierunku getEdgeEnd1->getEdgeEnd2
			CapacType flow;/**<\brief Actual flow through edge*/
			// koszt jednostkowy przeplywu dla luku, jesli wymagany to wymagany nieujemny z wyjatkiem petli
			CostType cost;/**<\brief Cost of unit size flow.*/

			/** \brief Empty constructor.*/
			TrsEdgeLabs():
				hi(0),
				lo(0),
				cost(0)
			{ }
			/** \brief Constructor.*/
			TrsEdgeLabs( CapacType alo):
				hi(0),
				lo( alo ),
				cost(0)
			{ }
			/** \brief Constructor.*/
			TrsEdgeLabs( CapacType alo, CapacType ahi):
				hi( ahi ),
				lo( alo ),
				cost(0)
			{ }
			/** \brief Constructor.*/
			TrsEdgeLabs( CapacType alo,
				CapacType ahi,
				CostType c): hi( ahi ), lo( alo ), cost( c )
			{ }
		};



struct OpisE {
    string name;
    int cap;

    OpisE(int arg=0): cap (arg), name("") {}
};

std::ostream& operator<<(std::ostream& os,OpisE arg) { os << arg.name; return os; }

Koala::Graph<char,OpisE> g;
Koala::Graph<char,OpisE>::PVertex A,B,C,D,E,F,V,U,S,T,G,tabV[20],*tabVit;
Koala::Graph<char,OpisE>::PEdge tabE[20];

Koala::AssocArray<Koala::Graph<char,OpisE>::PEdge,EdgeLabs<int16_t,int> >  edgeCont;
//Koala::AssocArray<Koala::Graph<char,OpisE>::PVertex,Koala::Flow::VertLabs<Koala::Graph<char,OpisE>,int> > vertCont;

Koala::AssocArray<Koala::Graph<char,OpisE>::PEdge,TrsEdgeLabs<int16_t,int> > tedgeCont;
Koala::AssocArray<Koala::Graph<char,OpisE>::PVertex,TrsVertLoss<int16_t> > tvertCont;


void dijTest()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');G=g.addVert('G');

    g.addArc(S,A,OpisE(20));g.addArc(S,B,OpisE(50));g.addArc(B,A,OpisE(10));
    g.addArc(A,D,OpisE(50));g.addArc(A,C,OpisE(60));g.addArc(B,C,OpisE(30));
    g.addArc(C,D,OpisE(10));g.addArc(C,T,OpisE(20));
    g.addArc(D,T,OpisE(40));g.addArc(D,E,OpisE(80));g.addArc(E,T,OpisE(40));
    g.addArc(F,S,OpisE(40));
    g.addArc(T,G,OpisE(100));


    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}

void dijTest2()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addArc(S,A,OpisE(2));g.addArc(S,B,OpisE(2));g.addArc(S,C,OpisE(2));
    g.addArc(A,D,OpisE(5));g.addArc(A,E,OpisE(1));g.addArc(B,D,OpisE(1));
    g.addArc(B,F,OpisE(1));g.addArc(C,E,OpisE(1));
    g.addArc(B,E,OpisE(3));g.addArc(D,T,OpisE(3));g.addArc(E,T,OpisE(2));
    g.addArc(F,T,OpisE(4));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void dijTest3()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    g.addEdge(S,A,OpisE(20));g.addEdge(S,B,OpisE(50));g.addEdge(B,A,OpisE(10));
    g.addEdge(D,A,OpisE(50));g.addEdge(A,C,OpisE(60));g.addEdge(B,C,OpisE(30));
    g.addEdge(C,D,OpisE(10));g.addEdge(C,T,OpisE(20));
    g.addEdge(D,T,OpisE(40));g.addEdge(D,E,OpisE(80));g.addEdge(E,T,OpisE(40));

//    g.addArc(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArc(D,F,OpisE(40));g.addArc(S,C,OpisE(40));g.addArc(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void costTest()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
    F=g.addVert('F');

    edgeCont[g.addArc(S,A,OpisE(0))]=EdgeLabs<int16_t,int>(18,-3);
    edgeCont[g.addArc(S,C,OpisE(50))]=EdgeLabs<int16_t,int>(20,8);
    edgeCont[g.addArc(A,C,OpisE(10))]=EdgeLabs<int16_t,int>(15,-4);
    edgeCont[g.addArc(B,A,OpisE(50))]=EdgeLabs<int16_t,int>(20,5);
    edgeCont[g.addArc(C,B,OpisE(60))]=EdgeLabs<int16_t,int>(12,8);
    edgeCont[g.addArc(B,T,OpisE(30))]=EdgeLabs<int16_t,int>(14,5);
    edgeCont[g.addArc(C,T,OpisE(10))]=EdgeLabs<int16_t,int>(17,3);
//    edgeCont[g.addLoop(C,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,1);
//    edgeCont[g.addLoop(B,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,-10);

//    edgeCont[g.addArc(S,T,OpisE(40))]=Koala::Flow::EdgeLabs<int>(100,1);

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArc(D,F,OpisE(40));g.addArc(S,C,OpisE(40));g.addArc(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void costTest2()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    edgeCont[g.addEdge(A,S,OpisE(0))]=EdgeLabs<int16_t,int>(18,3);
    edgeCont[g.addEdge(S,C,OpisE(50))]=EdgeLabs<int16_t,int>(20,8);
    edgeCont[g.addEdge(A,C,OpisE(10))]=EdgeLabs<int16_t,int>(15,4);
    edgeCont[g.addEdge(B,A,OpisE(50))]=EdgeLabs<int16_t,int>(20,5);
    edgeCont[g.addEdge(C,B,OpisE(60))]=EdgeLabs<int16_t,int>(12,8);
    edgeCont[g.addEdge(B,T,OpisE(30))]=EdgeLabs<int16_t,int>(14,5);
    edgeCont[g.addEdge(C,T,OpisE(10))]=EdgeLabs<int16_t,int>(17,3);
//    edgeCont[g.addLoop(B,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,-10);

//    g.addArc(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArc(D,F,OpisE(40));g.addArc(S,C,OpisE(40));g.addArc(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void costCostTest()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');//B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    (edgeCont[g.addArc(S,A,OpisE(0))]=EdgeLabs<int16_t,int>(2,-3)).flow=0;
    (edgeCont[g.addArc(S,A,OpisE(0))]=EdgeLabs<int16_t,int>(2,-4)).flow=2;
    (edgeCont[g.addEdge(A,T,OpisE(50))]=EdgeLabs<int16_t,int>(2,3)).flow=2;
    (edgeCont[g.addEdge(A,T,OpisE(10))]=EdgeLabs<int16_t,int>(2,4)).flow=0;
    (edgeCont[g.addLoop(A,OpisE(10))]=EdgeLabs<int16_t,int>(1,0)).flow=1;
}

void costTest3()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    edgeCont[g.addArc(S,A,OpisE(0))]=EdgeLabs<int16_t,int>(18,3);
    edgeCont[g.addArc(S,C,OpisE(50))]=EdgeLabs<int16_t,int>(20,8);
    edgeCont[g.addArc(A,C,OpisE(10))]=EdgeLabs<int16_t,int>(15,4);
    edgeCont[g.addArc(A,B,OpisE(50))]=EdgeLabs<int16_t,int>(20,5);
    edgeCont[g.addArc(C,B,OpisE(60))]=EdgeLabs<int16_t,int>(12,8);
    edgeCont[g.addArc(B,T,OpisE(30))]=EdgeLabs<int16_t,int>(14,5);
    edgeCont[g.addArc(C,T,OpisE(10))]=EdgeLabs<int16_t,int>(17,3);


    edgeCont[g.addArc(A,S,OpisE(0))]=EdgeLabs<int16_t,int>(18,3);
    edgeCont[g.addArc(C,S,OpisE(50))]=EdgeLabs<int16_t,int>(20,8);
    edgeCont[g.addArc(C,A,OpisE(10))]=EdgeLabs<int16_t,int>(15,4);
    edgeCont[g.addArc(B,A,OpisE(50))]=EdgeLabs<int16_t,int>(20,5);
    edgeCont[g.addArc(B,C,OpisE(60))]=EdgeLabs<int16_t,int>(12,8);
    edgeCont[g.addArc(T,B,OpisE(30))]=EdgeLabs<int16_t,int>(14,5);
    edgeCont[g.addArc(T,C,OpisE(10))]=EdgeLabs<int16_t,int>(17,3);

//    edgeCont[g.addLoop(B,OpisE(10))]=Koala::Flow::EdgeLabs<int>(1,-10);

//    g.addArc(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArc(D,F,OpisE(40));g.addArc(S,C,OpisE(40));g.addArc(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void transTest()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    S=g.addVert('S');T=g.addVert('T');
//    F=g.addVert('F');

    tedgeCont[g.addArc(S,A,OpisE(0))]=TrsEdgeLabs<int16_t,int>(0,18,3);
    tedgeCont[g.addArc(S,C,OpisE(50))]=TrsEdgeLabs<int16_t,int>(0,20,8);
    tedgeCont[g.addArc(A,C,OpisE(10))]=TrsEdgeLabs<int16_t,int>(0,15,4);
    tedgeCont[g.addEdge(B,A,OpisE(50))]=TrsEdgeLabs<int16_t,int>(0,20,5);
    tedgeCont[g.addArc(C,B,OpisE(60))]=TrsEdgeLabs<int16_t,int>(0,12,8);
    tedgeCont[g.addArc(B,T,OpisE(30))]=TrsEdgeLabs<int16_t,int>(0,14,5);
    tedgeCont[g.addArc(C,T,OpisE(10))]=TrsEdgeLabs<int16_t,int>(0,17,3);
    tedgeCont[g.addLoop(C,OpisE(10))]=TrsEdgeLabs<int16_t,int>(1,10,-1);
//    edgeCont[g.addLoop(B,OpisE(10))]=EdgeLabs<int>(1,-10);

//    g.addArc(T,F,OpisE(40));

//    g.addEdge(E,C,OpisE(40));
//    g.addEdge(T,F,OpisE(40));//g.addArc(D,F,OpisE(40));g.addArc(S,C,OpisE(40));g.addArc(B,D,OpisE(40));

    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c",ePt->getEnds().first->info,ePt->getEnds().second->info);
            ePt->info.name=buf;
        }


    for(Koala::Graph<char,OpisE>::PVertex ePt=g.getVert();ePt;ePt=g.getVertNext(ePt))
        tvertCont[ePt]=TrsVertLoss<int16_t>(0,0);
    tvertCont[S].lo=-34;
    tvertCont[S].hi=-31;
    tvertCont[T].lo=31;
    tvertCont[T].hi=100;
}


void ghTest()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');
    D=g.addVert('D');E=g.addVert('E');
    F=g.addVert('F');//G=g.addVert('G');

    g.addEdge(B,A,OpisE(1));g.addEdge(A,C,OpisE(7));g.addEdge(B,C,OpisE(1));
    g.addEdge(B,D,OpisE(3));g.addEdge(B,E,OpisE(2));g.addEdge(E,C,OpisE(4));
    g.addEdge(E,D,OpisE(1));g.addEdge(D,F,OpisE(6));
    g.addEdge(E,F,OpisE(2));


    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
        {   char buf[20];
            sprintf(buf,"%c%c:%d",ePt->getEnds().first->info,ePt->getEnds().second->info,ePt->info.cap);
            edgeCont[ePt].capac=ePt->info.cap;
            ePt->info.name=buf;
        }

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


void ghTest2()
{   g.clear();edgeCont.clear();
//    vertCont.clear();

    for(int i=0;i<10;i++) g.addVert();
    for(U=g.getVert();U!=g.getVertLast();U=g.getVertNext(U))
        for(V=g.getVertNext(U);V;V=g.getVertNext(V))
        if (rand()%5==0) edgeCont[g.addEdge(U,V)].capac=rand()%10;

    U=A; V=E;
    for(int i=0;i<20;i++) {tabV[i]=0; tabE[i]=0; }
    tabVit=tabV;
}


#include "main.hpp"

//    g.makeAdjMatrix();
    dijTest();
    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);
    cout << "\n***=\n" << boolalpha;
//    cout << Koala::Flow::usedCap(g,edgeCont,g.getEdge(B,C),B,true) << '\n';



//    edgeCont[g.getEdge(A,D)].flow=40;edgeCont[g.getEdge(A,C)].flow=50;

    cout << "\n***\n";
//    for(Koala::Graph<char,OpisE>::PEdge ePt=g.getEdge();ePt;ePt=g.getEdgeNext(ePt))
//        {   char buf[20]; sprintf(buf," %d",edgeCont[ePt].flow); ePt->info.name.append(buf); }
//
//    Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists,std::make_pair(true,true));
//

    cout << "\n***\n" << boolalpha;
    cout << Koala::Flow::maxFlow(g,edgeCont,S,T) << '\n';
    assert(Koala::Flow::vertFlow(g,edgeCont,S)==Koala::Flow::maxFlow(g,edgeCont,S,T));
//    cout << Koala::Flow::layerFlow(g,edgeCont,S,T) << '\n';
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';


    costTest2();
    cout << "\n\n-----------\n\n"<< Koala::Flow::maxFlow(g,edgeCont,S,T) << "\n";
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "cost=" << Koala::Flow::flowCost(g,edgeCont) << "\n";
    cout << "\ntestMinCost=" << Koala::Flow::testMinCost(g,edgeCont) << "\n\n";

    Koala::Flow::clearFlow(g,edgeCont);
    std::pair<int,int>  pres=Koala::Flow::minCostFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "vol: " << pres.second << " cost: " << pres.first << "\n\n";
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\ntestMinCost=" << Koala::Flow::testMinCost(g,edgeCont) << "\n\n";



    Koala::Flow::clearFlow(g,edgeCont);
    pres=Koala::Flow::minCostFlow(g,edgeCont,S,T,6);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "vol: " << pres.second << " cost: " << pres.first << "\n\n";
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\ntestMinCost=" << Koala::Flow::testMinCost(g,edgeCont) << "\n\n";


    pres=Koala::Flow::minCostFlow(g,edgeCont,S,T,26);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "vol: " << pres.second << " cost: " << pres.first << "\n\n";
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\ntestMinCost=" << Koala::Flow::testMinCost(g,edgeCont) << "\n\n";



//    Koala::Flow::clearFlow(g,edgeCont);
    pres=Koala::Flow::minCostFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "vol: " << pres.second << " cost: " << pres.first << "\n\n";
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\ntestMinCost=" << Koala::Flow::testMinCost(g,edgeCont) << "\n\n";


    Koala::Flow::clearFlow(g,edgeCont);
    pres=Koala::Flow::minCostFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "vol: " << pres.second << " cost: " << pres.first << "\n\n";
    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << edgeCont[e].flow << '\n';
    cout << "\ntestMinCost=" << Koala::Flow::testMinCost(g,edgeCont) << "\n\n";

    transTest();
    cout << "\n\ntttttttttt\n\n"<< Koala::Flow::transship(g,tedgeCont,tvertCont) << "\n";

    for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info << ": flow=" << tedgeCont[e].flow << '\n';

    cout << "\n\n*************\n\n";
    dijTest();
    int a=Koala::Flow::maxFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    Koala::Flow::clearFlow(g,edgeCont);
    cout << "maxf=" << a << '\n';
    for(int i=0;i<=a+1;i++)
    {
        Koala::Flow::clearFlow(g,edgeCont);
        int b=Koala::Flow::maxFlow(g,edgeCont,S,T,i);
        assert(Koala::Flow::testFlow(g,edgeCont,S,T));
        cout << (b==i) << '\n';
    }

    cout << "\n\n*************\n\n";
    costTest2();
    a=Koala::Flow::maxFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "maxf=" << a << '\n';
    for(int i=0;i<=a+1;i++)
    {   int last=-10;
        Koala::Flow::clearFlow(g,edgeCont);
        std::pair<int,int> b=Koala::Flow::minCostFlow(g,edgeCont,S,T,i);
        assert(Koala::Flow::testFlow(g,edgeCont,S,T));
        for(Koala::Graph<char,OpisE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e)) edgeCont[e].flow=0;
//        std::pair<int,int> bb=Koala::Flow::minCostFlow2(g,edgeCont,S,T,i);
//        assert(Koala::Flow::testFlow(g,edgeCont,S,T));
//        assert(b==bb);
        cout << (b.second==i) << ' '<< (b.second>=last) << ' '<< b.first <<'\n';
        last=b.second;
    }

    costTest2();
    Koala::Flow::maxFlow(g,edgeCont,S,T);
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
//    edgeCont[g.getEdge(S,A)].flow=2;
//    edgeCont[g.getEdge(A,B)].flow=2;
//    edgeCont[g.getEdge(B,T)].flow=2;
//    edgeCont[g.getEdge(S,C)].flow=2;
//    edgeCont[g.getEdge(C,T)].flow=2;
    int b;
//    cout << (b=Koala::Flow::minMeanCycle(g,edgeCont,Koala::Flow::outPath(tabV,tabE)));
//    for(int i=0;i<b;i++) cout << tabV[i]->info;
//    cout << endl;
//    for(int i=0;i<b;i++) cout << tabE[i]->info.name << ' ';
        cout << "\n\nHHHHHHHHHHHHHHHH\n\n";
    ghTest();
    Koala::Flow::GHTreeEdge<Koala::Graph<char,OpisE>,int16_t> pairTab[1000];

    cout << "\n\n";
    Koala::Flow::gHTree(g,edgeCont,pairTab);

    cout << "\n\n";
    for(int i=0;i<g.getVertNo()-1;i++)
        std::cout << pairTab[i].first->info << pairTab[i].second->info << ' ' <<pairTab[i].capac <<'\n';

    cout << g.getVertNo();
    cout << "\n\nAAAAAAAAAAAAAAAA\n\n";

    ghTest2();
    Koala::Graph<char,int> ghcopy;
    Koala::Graph<char,int>::PEdge ecopytab[100];
    int gtmin,truemin;
    Koala::AssocTable<std::map<Koala::Graph<char,OpisE>::PVertex,Koala::Graph<char,int>::PVertex> > ghlink;


    {   ghcopy.clear(); ghlink.clear();
        for(Koala::Graph<char,OpisE>::PVertex v=g.getVert();v;v=g.getVertNext(v))
            ghlink[v]=ghcopy.addVert();

        Koala::Flow::gHTree(g,edgeCont,pairTab);

        for(int i=0;i<g.getVertNo()-1;i++)
            ghcopy.addEdge(ghlink[pairTab[i].first],ghlink[pairTab[i].second],pairTab[i].capac);

    }
    cout << g.getVertNo() << ' ' << g.getEdgeNo() << "\n\n";

    for(U=g.getVert();U!=g.getVertLast();U=g.getVertNext(U))
        for(V=g.getVertNext(U);V;V=g.getVertNext(V))
    //U=A; V=D;
    {   int len=Koala::BFS::findPath(ghcopy,ghlink[U],ghlink[V],Koala::BFS::outPath(blackHole,ecopytab));
        gtmin=std::numeric_limits<int>::max();
        for(int i=0;i<len;i++) gtmin=std::min(gtmin,ecopytab[i]->info);
        truemin=Koala::Flow::minEdgeCut(g,edgeCont,U,V,Koala::Flow::outCut(blackHole,blackHole)).capac;

//        std::cout << truemin << ' ' << gtmin << "\n";
        assert(truemin==gtmin);

    }

    cout << g.getVertNo() << ' ' << g.getEdgeNo();

    costCostTest();
    assert(Koala::Flow::testFlow(g,edgeCont,S,T));
    cout << "\ncost=" << Koala::Flow::flowCost(g,edgeCont) << "\n";
    cout << "\ntestMinCost=" << Koala::Flow::testMinCost(g,edgeCont) << "\n\n";


    return 0;
}
