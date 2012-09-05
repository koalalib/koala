#include <stdio.h>
#include <time.h>
#include "koala/io/g6.h"
#include "koala/classes/create.h"

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
	Koala::Creator::erdRen1(g1, n, ro, Gene(), Gene(), time(NULL), Koala::EdUndir);

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
	return 0;
}
