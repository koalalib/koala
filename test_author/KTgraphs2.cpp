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
		//SearchStructs::CompStoreTool<GraphType::PVertex> outV;

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
