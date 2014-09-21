#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "Koala/base/defs.h"
#include "Koala/graph/graph.h"
#include "Koala/graph/view.h"
#include "Koala/classes/create.h"
#include "Koala/io/text.h"




using namespace std;
using namespace Koala;


struct OpV1 {
    std::string name;
    OpV1(std::string arg="") : name(arg) {}
} ;

struct OpV2 {
    std::string name;
    OpV2(std::string arg="") : name(arg) {}
} ;

struct OpE1 {
    std::string name;
    OpE1(std::string arg="") : name(arg) {}
} ;

struct OpE2 {
    std::string name;
    OpE2(std::string arg="") : name(arg) {}
} ;


struct OpV {
    std::string name;
    OpV(std::string arg="") : name(arg) {}
} ;

struct OpE {
    std::string name;
    OpE(std::string arg="") : name(arg) {}
} ;

struct Caster {
    template <class InfoDest, class InfoSour1,class InfoSour2>
	void operator()(InfoDest& dest,InfoSour1 sour1,InfoSour2 sour2)
        { dest.name=std::string("[")+sour1.name+" ; "+ sour2.name + std::string("]"); }
    template <class InfoDest, class InfoSour1>
	void operator()(InfoDest& dest,InfoSour1 sour1,Koala::BlackHole b)
        { dest.name=std::string("[")+sour1.name+" ; "+  std::string("NULL]"); }
    template <class InfoDest, class InfoSour1>
	void operator()(InfoDest& dest,Koala::BlackHole b,InfoSour1 sour2)
        { dest.name=std::string("[NULL ; ")+  sour2.name+ std::string("]"); }
} cast;

struct TripCaster {
    template <class InfoDest, class InfoSour1,class InfoSour2>
	void operator()(InfoDest& dest,InfoSour1 sour1,InfoSour2 sour2)
        { dest.name=std::string("[")+sour1.name+" ; "+ sour2.name + std::string("]"); }
} tripcast;

struct SingleCaster {

	template <class InfoDest, class InfoSour>
	void operator()(InfoDest& dest,InfoSour sour)
        { dest.name=std::string("[")+sour.name+  std::string("]"); }

} singlecast;


struct DummyCaster {
    template <class InfoDest, class InfoSour1,class InfoSour2>
	void operator()(InfoDest& dest,InfoSour1 sour1,InfoSour2 sour2)
        { dest=InfoDest();  }

} dummycast;

AssocTable<std::map<Vertex<OpV,OpE> *,Vertex<OpV1,OpE1> *> > vtov1map;
AssocTable<std::map<Vertex<OpV,OpE> *,Vertex<OpV2,OpE2> *> > vtov2map;
AssocTable<std::map<Edge<OpV,OpE> *,Edge<OpV1,OpE1> *> > etoe1map;
AssocTable<std::map<Edge<OpV,OpE> *,Edge<OpV2,OpE2> *> > etoe2map;


Graph<OpV1,OpE1> g1;
Graph<OpV1,OpE1>::PVertex u1=g1.addVert(std::string("u1")),v1=g1.addVert(std::string("v1")),w1;
Graph<OpV1,OpE1>::PEdge e1=g1.addEdge(u1,v1,std::string("u1-v1")),f1;

Graph<OpV2,OpE2> g2;
Graph<OpV2,OpE2>::PVertex u2=g2.addVert(std::string("u2")),v2=g2.addVert(std::string("v2")),w2;
Graph<OpV2,OpE2>::PEdge e2=g2.addEdge(u2,v2,std::string("u2-v2")),f2;

Graph<OpV,OpE> g;
Graph<OpV,OpE>::PVertex u=g.addVert(std::string("u")),v=g.addVert(std::string("v")),w;
Graph<OpV,OpE>::PEdge e=g.addEdge(u,v,std::string("u>v"));



#include "main.hpp"
   string strN="N";

    g.clear();
    cout << Product::strong(g1,g2,g,
                          make_pair(tripcast,complexCast(tripcast,singlecast,singlecast)),
                          make_pair(stdLink(vtov1map,vtov2map),stdLink(etoe1map,etoe2map))
                                                 )->info.name << endl;

    std::cout << g.getVertNo() << endl;
    for(Graph<OpV,OpE>::PVertex u=g.getVert();u;u=g.getVertNext(u))
        cout << u->info.name << " : " << vtov1map[u]->info.name << ' '<< vtov2map[u]->info.name<< endl;

    std::cout << endl<< endl<< g.getEdgeNo() << endl;
    for(Graph<OpV,OpE>::PEdge e=g.getEdge();e;e=g.getEdgeNext(e))
        cout << e->info.name << " laczy " << g.getEdgeEnd1(e)->info.name << " & "<<  g.getEdgeEnd2(e)->info.name <<" : " <<
        (etoe1map[e] ? etoe1map[e]->info.name  : "NULL")

         << ' '<< (etoe2map[e] ? etoe2map[e]->info.name  : "NULL")
         << "\t" <<
        ((g.getEdgeType(e)== EdLoop) ?   'o' : ((g.getEdgeType(e)== Directed) ?   '>' : '-')) << endl ;

    Product::strong(g1,g2,g,make_pair(cast,cast));
    cout << endl << g.getVertNo() << ' ' << g.getEdgeNo();
    Product::strong(g1,g2,g);
    cout << endl << g.getVertNo() << ' ' << g.getEdgeNo();



    return EXIT_SUCCESS;
}
