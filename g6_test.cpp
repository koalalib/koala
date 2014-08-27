#include <stdio.h>
#include <time.h>
#include <iomanip>
#include "Koala/io/g6.h"
#include "Koala/classes/create.h"

struct Gene {
	int operator()(int i) {return i;}
	int operator()(int i, int j, Koala::EdgeDirection x) {return i+j;}
};

char txt[10000];
int main(int argc, char **argv) {
	typedef Koala::Graph<int,int> Graph;
	typedef  Graph::PVertex Vert;
	typedef  Graph::PEdge Edge;
	Graph g1, g2;

	int n=20;
	double ro = 0.1;
	if(argc>1) {
		n = atoi(argv[1]);
		if(n<=0) n=10;
	}
	if(argc>2) {
		ro = atof(argv[2]);
		if(ro<0||ro>1) ro = 0.1;
	}
	Koala::StdRandGen<> rgen;
	Koala::Creator::erdRen1(rgen,g1, n, ro, Gene(), Gene(),  Koala::EdUndir);

	int ans = Koala::IO::writeG6(g1, txt, 9999);
	printf("%d %s\n", ans, txt);
	ans = Koala::IO::readG6(g2, txt);
	printf("%d %s\n", ans, txt);

	for(Vert vv = g1.getVert(); vv; vv = g1.getVertNext(vv)) {
		printf(" %2d : ", vv->info);
		for(Edge ee = g1.getEdge(vv); ee; ee = g1.getEdgeNext(vv, ee)) {
			Vert uu = g1.getEdgeEnd(ee, vv);
			printf("%d ", uu->info);
		}
		printf(";\n");
	}
	printf("\n");

	int ii=0;
	for(Vert vv = g2.getVert(); vv; vv = g2.getVertNext(vv))
		vv->info = ii++;

	for(Vert vv = g2.getVert(); vv; vv = g2.getVertNext(vv)) {
		printf(" %2d : ", vv->info);
		for(Edge ee = g2.getEdge(vv); ee; ee = g2.getEdgeNext(vv, ee)) {
			Vert uu = g2.getEdgeEnd(ee, vv);
			printf("%d ", uu->info);
		}
		printf(";\n");
	}

    typedef   int Type;
    Koala::StdRandGen<Type> rg;

    std::cout << "\n" << std::numeric_limits< Type >::max() << ' ' << rg.maxRand <<"\n" ;

    for(int i=0;i<1000;i++)
        std::cout << std::setprecision(4) <<(double)rg.rand()/std::numeric_limits< Type >::max() << ' ';
     std::cout << "\n\n";
     for(int i=0;i<1000;i++) std::cout<<rg.rand(1) << " ";

	return 0;
}
