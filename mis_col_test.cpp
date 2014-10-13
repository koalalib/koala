#include <iostream>
#include <string>
#include <cassert>
#include <map>
#include <iterator>

#include <ctime>

#include "Koala/graph/graph.h"
#include "Koala/coloring/vertex.h"
#include "Koala/algorithm/mis.h"

using namespace std;

int main()
{
	Koala::Graph<int,int> G, G2;
	Koala::Graph<int, int>::PVertex V[] = { G.addVert(0), G.addVert(1), G.addVert(2), G.addVert(3),
		G.addVert(4), G.addVert(5), G.addVert(6), G.addVert(7), G.addVert(8), G.addVert(9) };

	Koala::AssocArray<Koala::Graph<int, int>::PVertex,int > S;
	//map<Koala::Graph<int, int>::PVertex,int > S;

	// Vertex Coloring

	// Graf trudny dla algorytmu LF
	G.addEdge(V[0], V[2]), G.addEdge(V[0], V[3]), G.addEdge(V[0], V[5]), G.addEdge(V[0], V[6]),
	G.addEdge(V[1], V[2]), G.addEdge(V[1], V[4]), G.addEdge(V[1], V[5]), G.addEdge(V[1], V[6]),
	G.addEdge(V[2], V[3]), G.addEdge(V[2], V[4]), G.addEdge(V[3], V[4]);

	assert(Koala::VertColoring::color(G, S, 3) == 3), S.clear();
	assert(Koala::VertColoring::color(G, S) == 3), S.clear();
	assert(Koala::VertColoring::color(G, S, 2) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmu SL
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[2]), G.addEdge(V[0], V[4]), G.addEdge(V[0], V[6]),
	G.addEdge(V[1], V[2]), G.addEdge(V[1], V[3]), G.addEdge(V[1], V[7]), G.addEdge(V[2], V[3]),
	G.addEdge(V[2], V[4]), G.addEdge(V[3], V[5]), G.addEdge(V[3], V[7]), G.addEdge(V[4], V[5]),
	G.addEdge(V[4], V[6]), G.addEdge(V[5], V[6]), G.addEdge(V[5], V[7]), G.addEdge(V[6], V[7]);

	assert(Koala::VertColoring::color(G, S, 4) == 4), S.clear();
	assert(Koala::VertColoring::color(G, S) == 4), S.clear();
	assert(Koala::VertColoring::color(G, S, 3) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmu SLF
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[2]), G.addEdge(V[0], V[3]), G.addEdge(V[0], V[4]),
	G.addEdge(V[1], V[2]), G.addEdge(V[1], V[3]), G.addEdge(V[1], V[5]), G.addEdge(V[4], V[6]),
	G.addEdge(V[4], V[7]), G.addEdge(V[5], V[6]), G.addEdge(V[5], V[7]), G.addEdge(V[6], V[7]);

	assert(Koala::VertColoring::color(G, S, 3) == 3), S.clear();
	assert(Koala::VertColoring::color(G, S) == 3), S.clear();
	assert(Koala::VertColoring::color(G, S, 2) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmu SLI
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[2]), G.addEdge(V[0], V[3]), G.addEdge(V[0], V[4]),
	G.addEdge(V[1], V[2]), G.addEdge(V[1], V[6]), G.addEdge(V[1], V[7]), G.addEdge(V[1], V[8]),
	G.addEdge(V[2], V[5]), G.addEdge(V[2], V[6]), G.addEdge(V[2], V[8]), G.addEdge(V[3], V[4]),
	G.addEdge(V[3], V[5]), G.addEdge(V[3], V[7]), G.addEdge(V[3], V[8]), G.addEdge(V[4], V[5]),
	G.addEdge(V[4], V[6]), G.addEdge(V[4], V[7]), G.addEdge(V[5], V[6]), G.addEdge(V[5], V[8]),
	G.addEdge(V[6], V[7]), G.addEdge(V[7], V[8]);

	assert(Koala::VertColoring::color(G, S, 4) == 4), S.clear();
	assert(Koala::VertColoring::color(G, S) == 4), S.clear();
	assert(Koala::VertColoring::color(G, S, 3) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmu GIS
	G.addEdge(V[0], V[4]), G.addEdge(V[1], V[4]), G.addEdge(V[2], V[5]), G.addEdge(V[3], V[5]);
	G.addEdge(V[4], V[5]);

	assert(Koala::VertColoring::color(G, S) == 2), S.clear();
	assert(Koala::VertColoring::color(G, S, 1) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmów LF i SL
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[2]), G.addEdge(V[0], V[4]), G.addEdge(V[0], V[6]),
	G.addEdge(V[1], V[3]), G.addEdge(V[1], V[5]), G.addEdge(V[1], V[7]), G.addEdge(V[2], V[3]),
	G.addEdge(V[2], V[4]), G.addEdge(V[2], V[5]), G.addEdge(V[2], V[6]), G.addEdge(V[3], V[4]),
	G.addEdge(V[3], V[5]), G.addEdge(V[3], V[7]), G.addEdge(V[4], V[6]), G.addEdge(V[4], V[7]),
	G.addEdge(V[5], V[6]), G.addEdge(V[5], V[7]), G.addEdge(V[6], V[7]);

	assert(Koala::VertColoring::color(G, S, 4) == 4), S.clear();
	assert(Koala::VertColoring::color(G, S) == 4), S.clear();
	assert(Koala::VertColoring::color(G, S, 3) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmów LF, SL i SLF
	G.addEdge(V[0], V[2]), G.addEdge(V[0], V[3]), G.addEdge(V[0], V[4]), G.addEdge(V[0], V[5]),
	G.addEdge(V[1], V[2]), G.addEdge(V[1], V[3]), G.addEdge(V[1], V[4]), G.addEdge(V[1], V[5]),
	G.addEdge(V[2], V[6]), G.addEdge(V[2], V[8]), G.addEdge(V[3], V[4]), G.addEdge(V[3], V[6]),
	G.addEdge(V[4], V[6]), G.addEdge(V[5], V[6]), G.addEdge(V[5], V[7]), G.addEdge(V[5], V[9]),
	G.addEdge(V[7], V[8]), G.addEdge(V[7], V[9]), G.addEdge(V[8], V[9]);

	for(int i = 20; i >= 3; i--)
	{
		int chi = Koala::VertColoring::color(G, S, i);
		assert(3 <= chi && chi <= i), S.clear();
	}
	assert(Koala::VertColoring::color(G, S) == 3), S.clear();
	assert(Koala::VertColoring::color(G, S, 2) == -1), S.clear();
	G.clearEdges();

	// Graf dość trudny dla algorytmu RSI
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[4]), G.addEdge(V[0], V[5]), G.addEdge(V[1], V[2]),
	G.addEdge(V[2], V[3]), G.addEdge(V[3], V[4]), G.addEdge(V[3], V[5]), G.addEdge(V[4], V[5]);

	for(int i = 20; i >= 3; i--)
	{
		int chi = Koala::VertColoring::color(G, S, i);
		assert(3 <= chi && chi <= i), S.clear();
	}
	assert(Koala::VertColoring::color(G, S, 2) == -1), S.clear();
	G2=G; G2+=G; G2.addVert(); G.glue(G.getVert(),G.copy(G2));
	assert(Koala::VertColoring::color(G, S) == 3), S.clear();
	assert(Koala::VertColoring::color(G, S, 2) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmu LFI
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[4]), G.addEdge(V[0], V[5]), G.addEdge(V[0], V[6]),
	G.addEdge(V[1], V[2]), G.addEdge(V[1], V[7]), G.addEdge(V[1], V[8]), G.addEdge(V[2], V[3]),
	G.addEdge(V[2], V[7]), G.addEdge(V[2], V[8]), G.addEdge(V[3], V[4]), G.addEdge(V[3], V[5]),
	G.addEdge(V[3], V[6]), G.addEdge(V[4], V[5]);

	for(int i = 20; i >= 3; i--)
	{
		int chi = Koala::VertColoring::color(G, S, i);
		assert(3 <= chi && chi <= i), S.clear();
	}
	assert(Koala::VertColoring::color(G, S) == 3), S.clear();
	assert(Koala::VertColoring::color(G, S, 2) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmów LF, SL, SLF, LFI, SLI i SLFI
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[2]), G.addEdge(V[0], V[3]), G.addEdge(V[0], V[5]),
	G.addEdge(V[0], V[6]), G.addEdge(V[0], V[7]), G.addEdge(V[1], V[2]), G.addEdge(V[1], V[3]),
	G.addEdge(V[1], V[4]), G.addEdge(V[1], V[6]), G.addEdge(V[1], V[7]), G.addEdge(V[2], V[3]),
	G.addEdge(V[2], V[4]), G.addEdge(V[2], V[5]), G.addEdge(V[2], V[7]), G.addEdge(V[3], V[4]),
	G.addEdge(V[3], V[5]), G.addEdge(V[3], V[6]), G.addEdge(V[4], V[5]), G.addEdge(V[4], V[7]),
	G.addEdge(V[4], V[8]), G.addEdge(V[4], V[9]), G.addEdge(V[5], V[6]), G.addEdge(V[5], V[8]),
	G.addEdge(V[5], V[9]), G.addEdge(V[6], V[7]), G.addEdge(V[6], V[8]), G.addEdge(V[6], V[9]),
	G.addEdge(V[7], V[8]), G.addEdge(V[7], V[9]), G.addEdge(V[8], V[9]);

	for(int i = 20; i >= 5; i--)
	{
		int chi = Koala::VertColoring::color(G, S, i);
		assert(5 <= chi && chi <= i), S.clear();
	}
	assert(Koala::VertColoring::color(G, S) == 5), S.clear();
	assert(Koala::VertColoring::color(G, S, 4) == -1), S.clear();
	G.clearEdges();

	// Graf trudny dla algorytmów LF, SL, SLF i GIS
	G.addEdge(V[0], V[1]), G.addEdge(V[0], V[4]), G.addEdge(V[0], V[5]), G.addEdge(V[0], V[6]),
	G.addEdge(V[0], V[8]), G.addEdge(V[1], V[2]), G.addEdge(V[1], V[4]), G.addEdge(V[1], V[5]),
	G.addEdge(V[1], V[7]), G.addEdge(V[1], V[8]), G.addEdge(V[2], V[3]), G.addEdge(V[2], V[4]),
	G.addEdge(V[2], V[6]), G.addEdge(V[2], V[7]), G.addEdge(V[2], V[9]), G.addEdge(V[3], V[5]),
	G.addEdge(V[3], V[6]), G.addEdge(V[3], V[7]), G.addEdge(V[3], V[8]), G.addEdge(V[4], V[8]),
	G.addEdge(V[5], V[6]), G.addEdge(V[5], V[7]), G.addEdge(V[5], V[8]), G.addEdge(V[5], V[9]),
	G.addEdge(V[6], V[9]), G.addEdge(V[7], V[9]), G.addEdge(V[8], V[9]);

	for(int i = 20; i >= 4; i--)
	{
		int chi = Koala::VertColoring::color(G, S, i);
		assert(4 <= chi && chi <= i), S.clear();
	}
	assert(Koala::VertColoring::color(G, S) == 4), S.clear();
	assert(Koala::VertColoring::color(G, S, 3) == -1), S.clear();
	G.clear();

	// Maximum Independent Set
	int size = 30;
	Koala::Graph<int, int>::PVertex *IS = new Koala::Graph<int, int>::PVertex[size];
	Koala::Graph<int, int>::PVertex *U = new Koala::Graph<int, int>::PVertex[size];

	int p = 4, q = 3;
	for(int i = 0; i < q * p; i++)
		U[i] = G.addVert(i);
	U[q * p] = G.addVert(-1), U[q * p + 1] = G.addVert(-2), U[q * p + 2] = G.addVert(-3);

	for(int i = 0; i < q; i++)
		for(int j = 0; j < p; j++)
			G.addEdge(U[i * p + j], U[i * p + ((j + 1) % p)]);
	for(int i = 0; i <= q * p; i++)
		G.addEdge(U[q * p + 1], U[i]), G.addEdge(U[q * p + 2], U[i]);

	assert(Koala::MIS::get(G, IS) == 7);
	for(int i = 0; i < 6; i++)
		assert(Koala::MIS::getSome(G, IS, i) >= i && Koala::MIS::getSome(G, IS, i) < 7);
	assert(Koala::MIS::getSome(G, IS, 10) == -1);
	assert(Koala::MIS::get(G, IS, 6) == 7);
	assert(Koala::MIS::get(G, IS, 10) == -1);
	G.clear();

	for(int i = 0; i < size; i++)
		U[i] = G.addVert(i);
	/*for(int i = 0; i < size / 3; i++)
		G.addEdge(U[3 * i], U[3 * i + 1]), G.addEdge(U[3 * i + 2], U[3 * i + 1]), G.addEdge(U[3 * i], U[3 * i + 2]);
	for(int i = 10; i < 30; i++)
		printf("%d\n", Koala::MIS::get(G, IS, i));
	*/

	for(int test = 0; test < 5; test++)
	{
		srand(test);
		for(int i = 0; i < size; i++)
			for(int j = i + 1; j < size; j++)
				if(rand() % 5 == 0)
					G.addEdge(U[i], U[j]);

		clock_t startTime = clock();
		Koala::MIS::get(G, IS);
		cout << ((double) clock() - startTime) / (double)CLOCKS_PER_SEC << endl;
		G.clearEdges();
	}
	G.clear();

	// Graf GP(12, 4)
	int N = 12, K = 4;
	for(int i = 0; i < 2 * N; i++)
		U[i] = G.addVert(i);
	for(int i = 0; i < N; i++)
		G.addEdge(U[i], U[(i + 1) % N]), G.addEdge(U[i], U[i + N]), G.addEdge(U[i + N], U[(i + K) % N + N]);

	for(int test = 0; test < size; test++)
	{
		clock_t startTime = clock();
		int isSize = Koala::MIS::get(G, IS, test);
		double isTime = ((double) clock() - startTime) / (double)CLOCKS_PER_SEC;
		cout << isSize << "\t" << isTime << endl;
	}

//    G2=G; G+=G2;
	assert(Koala::MIS::get(G, IS) == 9);
	for(int i = 0; i < 9; i++)
		printf("%d ", G.getVertInfo(IS[i]));
    Koala::Graph<std::string,std::string> Gstr;
    Gstr.copy(G);
    Koala::Graph<std::string, std::string>::PVertex ISstr[100];
    assert(Koala::MIS::get(Gstr, ISstr) == 9);

    std::vector<Koala::Graph<int, int>::PVertex> vec;
    std::back_insert_iterator<std::vector<Koala::Graph<int, int>::PVertex> > viter=std::back_inserter(vec);
    *viter=G.getVert();viter++;
    *viter=G.getVert();viter++;
    *viter=G.getVert();viter++;
    std::cout << '\n' << vec.size();
    vec.clear();vec.resize(100);
    assert(Koala::MIS::get(G, vec.begin()) == 9);
    vec.clear();
    viter=std::back_inserter(vec);
    assert(Koala::MIS::get(G, viter) == 9);


	S.clear(), G.clear(), delete[] IS, delete[] U;

	return 0;
}
