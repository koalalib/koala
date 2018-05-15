
#include <ctime>
#include <iostream>
#include "Koala/coloring/list.h"
#include <cstdlib>

using namespace std;

typedef Koala::Graph<int, int> G;
typedef Koala::AssocArray<G::PEdge, Koala::Set<int> > GLists;


int main() {
	int curTime = time(NULL);
	cout << curTime << endl;
	srand(curTime);

	int n1, n2, p;
	cout << "Number of vertices in the first partition:";
	cin >> n1;
	cout << "Number of vertices in the second partition:";
	cin >> n2;
	cout << "Density [\%]:";
	cin >> p;

	G g;
	G::PVertex v;
	G::PVertex LOCALARRAY(vert, n1+n2);
	G::PEdge e;
	Koala::AssocArray<G::PEdge, int> col;
	GLists glis;
	Koala::Set<int> lista;


	for(int i=0; i<n1+n2; i++)
		vert[i] = g.addVert(i);

	int cnt = 0;
	p = n1*n2*p/100;
	for(int i=0; i<p; i++) {
		int u1 = rand()%n1;
		int u2 = rand()%n2 + n1;
		e = g.addEdge(vert[u1], vert[u2], cnt++);
	}

	int d = 0;
	for(v = g.getVert(); v; v = g.getVertNext(v) ) {
		if (g.deg(v) > d)
			d = g.deg(v);
	}

	for(int i=1; i<=d; i++)
		lista.add(i);

	for(e = g.getEdge(); e; e = g.getEdgeNext(e))
		glis[e] = lista;

	curTime = clock();
	int odp = Koala::ListEdgeColoringPar<Koala::AlgsDefaultSettings>::colorBipartite(g, glis, col);
	cout << "colEdge: " << odp << "; edgeNo: " << g.getEdgeNo() << "; d=" << d << "; colors number: " << col.size();
	cout << "; time: " << clock() - curTime;
	odp = Koala::ListEdgeColoringPar<Koala::AlgsDefaultSettings>::test(g, glis, col);
	cout << "; test: " << odp << endl;

    return 0;
}
