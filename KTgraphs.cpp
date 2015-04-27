#include <cassert>
#include <fstream>

#include "Koala/graph/graph.h"
#include "Koala/io/g6.h"
#include"./Koala/io/text.h"
#include "Koala/classes/detect.h"

#define TESTS 100
#define ROWS 10
#define COLUMNS 10

/*#undef assert
#define assert_error_report_helper(cond) "assertion failed: " #cond
#define assert(cond) { if(!(cond)) { cout << "line " << __LINE__ << ": " << assert_error_report_helper((cond)) << endl; } }*/

using namespace std;
using namespace Koala;
using namespace Koala::IO;

typedef Graph<string, string> GraphType;
typedef AssocArray<GraphType::PVertex, int>  VertMap;
typedef AssocArray<GraphType::PEdge, int>  EdgeMap;
typedef AssocArray<GraphType::PVertex, Blocks::VertData >  VertDataMap;

void create_graph(GraphType &G, int n, double p)
{
	vector<GraphType::PVertex> V;
	for (int i = 0; i < n; i++)
		V.push_back(G.addVert());

	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (rand() < RAND_MAX * p)
				G.addEdge(V[i], V[j]);
}

void create_multigraph(GraphType &G, int n, double p)
{
	vector<GraphType::PVertex> V;
	for (int i = 0; i < n; i++)
		V.push_back(G.addVert());

	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if (rand() < RAND_MAX * p)
			{
				G.addEdge(V[i], V[j]);
				if (rand() < RAND_MAX * p)
					G.addEdge(V[i], V[j]);
			}

	for (int i = 0; i < n; i++)
		if (rand() < RAND_MAX * p)
			G.addEdge(V[i], V[i]);
}

int create_kpartite_graph(GraphType &G, vector<int> &n)
{
	int k = rand() % ROWS + 1;
	vector<vector<GraphType::PVertex> > V(k);

	for (int i = 0; i < k; i++)
	{
		int size = (rand() % COLUMNS) + 1;
		n.push_back(size);

		for (int j = 0; j < size; j++)
			V[i].push_back(G.addVert());

		for (int j = 0; j < size; j++)
			for (int a = 0; a < i; a++)
				for (int b = 0; b < V[a].size(); b++)
					G.addEdge(V[i][j], V[a][b]);
	}

	//TODO: mix

	sort(n.begin(), n.end());

	return k;
}

void test_kpartite()
{
	GraphType G;
	vector<int> n;
	VertMap tabV;
	SearchStructs::CompStoreTool<GraphType::PVertex> outV;

	srand(0);
	for (int t = 0; t < TESTS; t++)
	{
		auto k = create_kpartite_graph(G, n);
		tabV.clear();

		auto test = IsIt::CompKPartite::split(G, tabV, outV.input());
		// FIX: Tu jest b³¹d - czasami outV wyrzuca o jedn¹ partycjê wiêcej - pust¹!
		if (test != k)
        {
                cout << "test=" << test << " k=" << k << endl;
            		assert(test == k);
        }


		vector<int> out_n;
		for (int i = 0; i < outV.size(); i++)
		{
			out_n.push_back(outV.size(i));

			Set<GraphType::PVertex> tabV;
			for (int j = 0; j < outV.size(i); j++)
			{
				auto it = outV[i] + j;
				for (auto e = G.getEdge(*it); e; e = G.getEdgeNext(*it, e))
					assert(!tabV.count(G.getEdgeEnd(e, *it)));
				tabV.insert(*it);
			}
		}
		sort(out_n.begin(), out_n.end());

		assert(out_n.size() == n.size());
		for (int i = 0; i < out_n.size(); i++)
			assert(out_n[i] == n[i]);

		G.clear(), n.clear();
	}

	/* TODO: Wybrac kilka grafow nie bedacych compMParite Hi o roznych rzedach
	n'. Dla grafu losowanego jak wyzej losujemy podzbior wierzcholkow
	mocy n', na ktorym zaindukowujemy podgraf Hi. sprawdzamy
	compMPartite == false.*/

	cout << "Koniec testow kPartite" << endl;
}

void find_primes(vector<GraphType> &H, const string &name)
{
	GraphType G;
	ifstream file(name.c_str());
	string line;

	while (file >> line)
	{
		auto ans = IO::readG6(G, line.c_str());
		if(IsIt::prime(G))
			H.push_back(G);
		G.clear();
	}
}

void expand_prime(GraphType &G, VertMap &tabV)
{
	auto n = G.getVertNo();
	vector<GraphType::PVertex> V(n);

	G.getVerts(V.begin());
	for(int i = 0; i < n; i++)
	{
		GraphType X;
		create_graph(X, rand() % COLUMNS + 1, 0.5);

		for (GraphType::PVertex v = G.substitute(V[i], X); v; v = G.getVertNext(v))
			tabV[v] = i;
	}

	//TODO: mix
}

void test_prime()
{
	GraphType G;
	vector<GraphType> H;
	VertMap tabV;

	srand(0);
	find_primes(H, "graph07c.g6");
	for (int t = 0; t < TESTS; t++)
	{
		G += H[t % H.size()];
		auto n = G.getVertNo();
		expand_prime(G, tabV);

		SearchStructs::CompStoreTool<GraphType::PVertex> outV;
		auto out = Modules::split(G, outV.input(), blackHole);
		assert(out.type == mpPrime);
		assert(out.size == n);

		vector<int> components(n, 0);
		for (int i = 0; i < outV.size(); i++)
		{
			auto expected = tabV[outV[i][0]];
			assert(!components[expected]);
			for (int j = 0; j < outV.size(i); j++)
				assert(tabV[outV[i][j]] == expected);
			components[expected] = 1;
		}

		G.clear();
	}

	cout << "Koniec testow Prime" << endl;
}

int create_blocks(GraphType &G)
{
	vector<GraphType> H(ROWS);
	for (int i = 0; i < ROWS; i++)
	{
		create_multigraph(H[i], COLUMNS, 0.5);

		auto nG = G.getVertNo(), nH = H[i].getVertNo();
		G += H[i];

		if (i != 0)
		{
			vector<GraphType::PVertex> V(nG + nH);
			G.getVerts(V.begin());

			auto uG = V[rand() % nG], uH = V[nG + rand() % nH], vG = V[rand() % nG], vH = V[nG + rand() % nH];
			while (uG == vG)
				vG = V[rand() % nG];
			while (uH == vH)
				vH = V[nG + rand() % nH];
			G.glue(uG, uH);
			G.glue(vG, vH);
		}
	}

	//TODO: mix

	return G.getVertNo();
}

void test_blocks()
{
	GraphType G, H;

	srand(0);
//	for (int t = 0; t < TESTS; t++)
	{
//	    GraphType G1;
//	    create_blocks(G1);//G.copy(G1);
//        ofstream pliko("plik_err.txt");
//        writeGraphText(G1, pliko, RG_EdgeList);
//        pliko.close();
//
//	    for(GraphType::PVertex v=G1.getVert();v;v=G1.getVertNext(v)) G.addVert();
//	    GraphType::PVertex tab[G.getVertNo()];
//	    G.getVerts(tab);
//	    for(GraphType::PEdge e=G1.getEdge();e;e=G1.getEdgeNext(e))
//	    {
//	        int a,b;
//	        G.addLink(tab[a=G1.vertPos(G1.getEdgeEnd1(e))],tab[b=G1.vertPos(G1.getEdgeEnd2(e))]);
//	        assert(a>=0 && b>=00 && a!=b && a<G.getVertNo() && b<G.getVertNo());
//	    }

    ifstream pliki;
    G.clear();
	pliki.open("plik_err.txt");
	cout << boolalpha << readGraphText(G, pliki, RG_EdgeList) << endl ;
	pliki.close();

		auto n = G.getVertNo();
		cout << G.getVertNo() << " " << G.getEdgeNo() <<" " << G.mu() << "!\n";
		for(int i=0;i<n;i++) cout << i<<".deg=" << G.deg(G.vertByNo(i))<< ' ';
		writeGraphText(G, cout, RG_VertexLists);

		VertDataMap tabV;
		EdgeMap tabE;
		SearchStructs::CompStoreTool<GraphType::PVertex> outV;

		//auto cnt = Blocks::splitComp(G, G.getVert(), tabV, tabE, outV.input(), blackHole);

        GraphType::PVertex comptab[10000];
        int comptabind[1000];
        auto cnt = Blocks::splitComp(G, G.getVert(), tabV, tabE, Koala::Blocks::compStore(comptabind,comptab), blackHole);

		vector<Set<GraphType::PVertex> > comps(cnt);
		vector<Set<int> > comps_int(cnt);
//		for (int i = 0; i < outV.size(); i++)
//			for (int j = 0; j < outV.size(i); j++)
//				comps[i].insert(outV[i][j]);
		for (int i = 0; i < cnt; i++)
        {   cout << "size=" << comptabind[i+1]-comptabind[i];
			for (int j = comptabind[i]; j < comptabind[i+1]; j++)
				{ comps[i].insert(comptab[j]);
                    comps_int[i].insert(G.vertPos(comptab[j]));
				}

        }


		//FIX: data.blockNo i tabV[v] s¹ ró¿nie uporz¹dkowane!
		/*for (auto v = G.getVert(); v; v = G.getVertNext(v))
		{
			auto &data = tabV[v];
			auto &test = comps[data.blockNo - 1];
			assert(test.count(v));
		}*/

		for (auto e = G.getEdge(); e; e = G.getEdgeNext(e))
		{
			auto &test = comps[tabE[e]];
			assert(test.count(G.getEdgeEnd1(e)));
			assert(test.count(G.getEdgeEnd2(e)));
		}

		for (int i = 0; i < cnt; i++)
			for (int j = i + 1; j < cnt; j++)
			{
				auto test = comps[i];
				test *= comps[j];
				Set<int> ci;

				cout << boolalpha << "\n cnt=" << cnt<< " i,j=" << i << ' '<< j <<' '<< comps_int[i] << '\t'<<comps_int[j]
				<<','<< G.vertPos((G.getVertSet()-comps[j]).first()) <<
				comps[i].subsetOf(G.getVertSet()) << comps[j].subsetOf(G.getVertSet()) << comps[i].subsetOf(comps[j])<<
				comps[i].size() << ' ' << comps[j].size() ;//<< (comps[i]==G.getVertSet()) << (comps[j]==G.getVertSet());
				assert(test.size() <= 1);
			}

		for (int i = 0; i < cnt; i++)
		{
			H.copy(G, make_pair(stdChoose(comps[i]), stdChoose(true)));

			vector<GraphType::PEdge> edges(H.getEdgeNo());
			H.getEdges(edges.begin());

			//FIX: grafy indukowane przez comps[i] w ogóle nie s¹ spójne!
			/*assert(IsIt::connected(H));
			for (int i = 0; i < edges.size(); i++)
			{
				auto u = H.getEdgeEnd1(edges[i]), v = H.getEdgeEnd2(edges[i]);
				H.delEdge(edges[i]);
				assert(IsIt::empty(H) || IsIt::connected(H));
				H.addEdge(u, v);
			}*/
			H.clear();
		}

		G.clear();
	}

	cout << "Koniec testow Blocks" << endl;
}

int main()
{
	//test_kpartite();
	//test_prime();
	test_blocks();

	system("pause");

	return 0;
}
