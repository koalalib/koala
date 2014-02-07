#define NOBOOST
//#define NOGRAPHLOG
//#define PROGRESS			// show progress tags
//#define NOCLASSES			// no class testing
//#define NOALGS			// no algorithms
//#define NOSLOW			// no slow algs at all
#define MAXSLOW			30	// max number of verts for slow algs
#define NOMISMATCHWARNING	true

#define INP_GRAPH_TXT		-1
#define INP_FILE_SMALL		0
#define INP_FILE_BIG		1
#define INP_GRAPH_RANDOM	2
#define INP_ROLL		99

//#define INPUT_SOURCE		INP_GRAPH_TXT
#define INPUT_SOURCE		INP_ROLL
//#define INPUT_SOURCE		INP_GRAPH_RANDOM

//#define MINGSIZE		11
//#define MAXGSIZE		20
//#define INTERVAL		1000

#define MINGSIZE		0
#define MAXGSIZE		250
#define INTERVAL		100

//#define BREAK()			return 1;
#define BREAK()


#include<conio.h>
#include<ctime>
#include<random>
#include<cstdio>
#include<cstdlib>
#include<fstream>

#include"./koala/io/g6.h"
#include"./koala/io/text.h"
#include"./koala/graph/graph.h"
#include"./koala/classes/create.h"
#include"./koala/classes/detect.h"
#include"./koala/algorithm/matching.h"
#include"./koala/base/rnd.h"

#ifndef NOBOOST
#include"./boost/graph/adjacency_list.hpp"
#include"./boost/graph/strong_components.hpp"
#include"./boost/graph/biconnected_components.hpp"
#include"./boost/graph/max_cardinality_matching.hpp"
#endif

#include<windows.h>


using namespace std;
using namespace Koala;
using namespace Koala::IO;
#ifndef NOBOOST
using namespace boost;
#endif


double GetTPS() {
	LARGE_INTEGER fq;
	QueryPerformanceFrequency(&fq);
	return fq.QuadPart;
	};

double tps = GetTPS();

double GetTime() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart / tps;
	};


typedef Graph<int, int> KGraph;
int kints[MAXGSIZE];
KGraph::PVertex kverts[MAXGSIZE];
KGraph::PEdge kedges[MAXGSIZE * MAXGSIZE / 2];
#ifndef NOBOOST
struct edge_component_t {
	enum { num = 555 };
	typedef edge_property_tag kind;
	} edge_component;
typedef adjacency_list<vecS, vecS, undirectedS,
no_property, property < edge_component_t, std::size_t >
> BGraph;
graph_traits<BGraph>::vertex_descriptor bedges[MAXGSIZE * MAXGSIZE / 2];
std::vector<int> component(MAXGSIZE);
#endif


struct Hashs {
	template<class K, class V> class AssocCont {
	public:
		typedef Koala::HashMap<K, V> Type;
		};
	};


#ifdef NOGRAPHLOG
#define GLOG(id)
#else
#define GLOG(id)	graphs << id;
#endif
#ifdef PROGRESS
#define PLOG(id)	cerr << "\r " << id << " \r";
#else
#define PLOG(id)
#endif
#define BASECAT(a, ...)	a ## __VA_ARGS__
#define CAT(a, ...)	BASECAT(a, ...)
#define STABLE_0(cls)
#define STABLE_1(Cls)	t = IsIt::Cls::maxStable(kg, kverts);		\
			if(indepSet < 0) indepSet = t;			\
			else if(t != indepSet && !NOMISMATCHWARNING) {	\
				cerr << "ERROR: " #Cls " independent set mismatch: " << indepSet << " vs " << t << endl;	\
				errors << g6 << " indep_set_mismatch " << indepSet << " " << t << endl;				\
				BREAK();				\
				};
#define VERTCOV_0(cls)
#define VERTCOV_1(Cls)	t = IsIt::Cls::minVertCover(kg, kverts);	\
			if(minVC < 0) minVC = t;			\
			else if(t != minVC && !NOMISMATCHWARNING) {	\
				cerr << "ERROR: " #Cls " min verex cover mismatch: " << minVC << " vs " << t << endl;	\
				errors << g6 << " min_vert_cover_mismatch " << indepSet << " " << t << endl;		\
				BREAK();				\
				};
#define CLIQUE_0(Cls)
#define CLIQUE_1(Cls)	t = IsIt::Cls::maxClique(kg, kverts);		\
			if(maxCliq < 0) maxCliq = t;			\
			else if(t != maxCliq && !NOMISMATCHWARNING) {	\
				cerr << "ERROR: " #Cls " max clique mismatch: " << maxCliq << " vs " << t << endl;	\
				errors << g6 << " max_clique_mismatch " << indepSet << " " << t << endl;		\
				BREAK();				\
				};					\

#define C_STABLE(q, cls)	BASECAT(STABLE_, q)(cls)
#define C_VERTCOV(q, cls)	BASECAT(VERTCOV_, q)(cls)
#define C_CLIQUE(q, cls)	BASECAT(CLIQUE_, q)(cls)


#define TEST(id, Cls, test, is, vc, cq)					\
		PLOG(id);						\
		bool BASECAT(r_, test) = IsIt::test(kg);		\
		if(BASECAT(r_, test)) {					\
			classes[id]++;					\
			GLOG("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[id]);	\
			C_STABLE(is, Cls)				\
			C_VERTCOV(vc, Cls)				\
			C_CLIQUE(cq, Cls)				\
		} else {						\
			GLOG("0");					\
			};
#define TEST2(id, Cls, test, is, vc, cq)					\
		PLOG(id);						\
		bool BASECAT(r_, test) = Cls::test(kg);			\
		if(BASECAT(r_, test)) {					\
			classes[id]++;					\
			GLOG("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[id]);	\
			C_STABLE(is, Cls)				\
			C_VERTCOV(vc, Cls)				\
			C_CLIQUE(cq, Cls)				\
		} else {						\
			GLOG("0");					\
			};

#define KCATCH(e)	catch(e &ex) { cerr << "\nException: " #e << endl; return 1; }
#define TRYKOALA	try {
#define CATCHKOALA	} \
	KCATCH(Koala::Error::AlgExcWrongConn)	\
	KCATCH(Koala::Error::AlgExcWrongMask)	\
	KCATCH(Koala::Error::AlgExcNullEdge)	\
	KCATCH(Koala::Error::AlgExcNullVert)	\
	KCATCH(Koala::Error::AlgExcWrongArg)	\
	KCATCH(Koala::Error::AlgExc)	\
	KCATCH(Koala::Error::GraphExcWrongMask)	\
	KCATCH(Koala::Error::GraphExcNullEdge)	\
	KCATCH(Koala::Error::GraphExcNullVert)	\
	KCATCH(Koala::Error::GraphExcWrongConn)	\
	KCATCH(Koala::Error::GraphExcWrongArg)	\
	KCATCH(Koala::Error::GraphExc)	\
	KCATCH(Koala::Error::ContExcPoolNotEmpty)	\
	KCATCH(Koala::Error::ContExcFull)	\
	KCATCH(Koala::Error::ContExcOutpass)	\
	KCATCH(Koala::Error::ContExcWrongArg)	\
	KCATCH(Koala::Error::ContExc)	\
	KCATCH(Koala::Error::ExcWrongMask)	\
	KCATCH(Koala::Error::ExcWrongConn)	\
	KCATCH(Koala::Error::ExcNullEdge)	\
	KCATCH(Koala::Error::ExcNullVert)	\
	KCATCH(Koala::Error::ExcWrongArg)	\
	KCATCH(Koala::Error::ExcBase);

int main() {
	int n, m, r1, r2, input, classes[128] = {0};
	int dummyCounter = 0;
	long seed;
	double t, bsum[32] = {0}, ksum[32] = {0};
	KGraph kg;
	string g6;
	fstream pl, inp, errors;
	StdRandGen<> rgen;

	seed = time(NULL);
//	seed = 1390337831;

	std::mt19937 gener(seed);
	std::uniform_int_distribution<int> ngen(MINGSIZE, MAXGSIZE);
	cerr << "SEED = " << seed << "\n";

#ifndef NOGRAPHLOG
	fstream graphs("graphs.log", ios_base::out);
#endif
	errors.open("errors.log", ios_base::out);

	int k = 0;
	int k_blocks = 0;
	input = INPUT_SOURCE;
	if(input == INP_ROLL) input = INP_FILE_SMALL;
	while(!kbhit()) {
		kg.clear();
#if INPUT_SOURCE == INP_FILE_SMALL || INPUT_SOURCE == INP_ROLL
		if(input == INP_FILE_SMALL) {
			if(!inp.is_open()) {
				cerr << "starting in-small.g6\n";
				inp.open("in-small.g6", ios_base::in);
				};
			if(inp >> g6) readG6(kg, g6);
			else {
				inp.close();
#if INPUT_SOURCE == INP_ROLL
				input = INP_FILE_BIG;
#else
				return 0;
#endif
				};
			};
#endif
#if INPUT_SOURCE == INP_FILE_BIG || INPUT_SOURCE == INP_ROLL
		if(input == INP_FILE_BIG) {
			if(!inp.is_open()) {
				cerr << "starting in-big.g6\n";
				inp.open("in-big.g6", ios_base::in);
				};
			if(inp >> g6) readG6(kg, g6);
			else {
				inp.close();
#if INPUT_SOURCE == INP_ROLL
				input = INP_GRAPH_RANDOM;
#else
				return 0;
#endif
				};
			};
#endif

#if INPUT_SOURCE == INP_GRAPH_RANDOM || INPUT_SOURCE == INP_ROLL
		if(input == INP_GRAPH_RANDOM) {
			n = ngen(gener);
			std::uniform_int_distribution<int> mgen(0, n * (n - 1) / 2);
//			std::uniform_int_distribution<int> mgen(0, n * min(n - 1, 2) / 2);
//			std::uniform_int_distribution<int> mgen(0, n * min(n - 1, 12) / 2);
//			std::uniform_int_distribution<int> mgen(0, n * min(n - 1, 30) / 2);
			m = mgen(gener);
			Creator::erdRen2(rgen,kg, n, m);
			};
#endif

#if INPUT_SOURCE == INP_GRAPH_TXT
		inp.open("graph.txt", ios_base::in);
		readGraphText(kg, inp, RG_VertexLists);
		inp.close();
#endif

		int z;
		KGraph::PVertex v;
		for(z = 0, v = kg.getVert(); v; v = kg.getVertNext(v), ++z) v->info = z;

#if INPUT_SOURCE != INP_GRAPH_TXT
		if(input != INP_FILE_BIG && input != INP_FILE_SMALL) {
			pl.open("graph.txt", ios_base::out);
			writeGraphText(kg, pl, RG_VertexLists | RG_VInfo);
			pl.close();
			};
#endif
#ifndef NOGRAPHLOG
		g6.clear();
		writeG6(kg, g6);
		graphs << g6 << " ";
#endif
		TRYKOALA

		// Koala matching
		PLOG("A1");
		t = GetTime();
		r1 = Matching::findMax(kg, kedges);
//		r1 = MatchingPar<Hashs>::findMax(kg, kedges);
		ksum[0] += GetTime() - t;
		if(!Matching::test(kg, kedges, kedges + r1)) {
			cerr << "ERROR: Matching::test failed\n";
			return 1;
			};
#ifndef NOBOOST
		// boost
		BGraph bg(n);
		for(KGraph::PEdge e = kg.getEdge(); e; e = kg.getEdgeNext(e)) {
			add_edge(e->getEnd1()->info, e->getEnd2()->info, bg);
			};
		t = GetTime();
		checked_edmonds_maximum_cardinality_matching(bg, bedges);
		r2 = matching_size(bg, bedges);
		bsum[0] += GetTime() - t;
		// verify
		if(r1 != r2) { cerr << "ERROR: matching mismatch; Koala = " << r1 << "  boost = " << r2 << "\n"; return 1; };
#endif
/*		// Koala SCC (needs directed graph)
		t = GetTime();
		r1 = SCC::split(kg, SearchStructs::compStore(kints, kverts), blackHole);
		ksum[1] += GetTime() - t;
#ifndef NOBOOST // boost
		t = GetTime();
		r2 = strong_components(bg, make_iterator_property_map(component.begin(), get(vertex_index, bg)));
		bsum[1] += GetTime() - t;
		// verify
		if(r1 != r2) { cerr << "ERROR: SCC mismatch; Koala = " << r1 << "  boost = " << r2 << "\n"; return 1; };
#endif
*/
		// Koala Blocks (needs directed graph)
		PLOG("A2");
		t = GetTime();
		r1 = Blocks::split(kg, blackHole, blackHole, SearchStructs::compStore(kints, kverts), blackHole);
		ksum[1] += GetTime() - t;
#ifndef NOBOOST // boost
		if(kg.getVertNo() > 1 && IsIt::connected(kg)) {
			property_map<BGraph, edge_component_t>::type blcomponent = get(edge_component, bg);
			t = GetTime();
			r2 = biconnected_components(bg, blcomponent);
			bsum[1] += GetTime() - t;
			// verify
			if(r1 != r2) { cerr << "ERROR: Blocks mismatch; Koala = " << r1 << "  boost = " << r2 << "\n"; return 1; };
			k_blocks++;
			};
#endif

#ifndef NOALGS
#ifndef NOSLOW
		if(kg.getVertNo() <= MAXSLOW) {	// SLOW
			PLOG("A3");
			Modules::Partition mp_res = Modules::split(kg, SearchStructs::compStore(kints, kverts), blackHole);
			if(mp_res.type == mpPrime) dummyCounter++;	// to keep Modules::split in code after optimization
			}
#endif

		PLOG("A4"); dummyCounter += LexBFS::scan(kg, blackHole, kverts);
		PLOG("A5"); dummyCounter += BFS::scan(kg, blackHole, kverts);
		PLOG("A6"); dummyCounter += DFSPreorder::scan(kg, blackHole, kverts);
		PLOG("A7"); dummyCounter += DFSPostorder::scan(kg, blackHole, kverts);
#ifndef NOSLOW
		if(kg.getVertNo() <= MAXSLOW) {
			PLOG("A8");
			dummyCounter += Connect::minVertCut(kg, kverts);
			if(kg.getVertNo() > 0 && g6 != "@") {
				PLOG("A9");
				Connect::EdgeCut<KGraph> ec_res = Connect::minEdgeCut(kg, kedges);
				if(ec_res.first != NULL) dummyCounter++;
				};
			};
#endif	// NOSLOW
#endif	// NOALGS

#ifndef NOCLASSES	// classes
		int indepSet = -1;
		int minVC = -1;
		int maxCliq = -1;
		int t;

		TEST(1, Bipartite, bipartite, 1, 1, 0);
		TEST(2, Chordal, chordal, 1, 1, 1);
		TEST(3, Cograph, cograph, 1, 1, 1);
// BUGS		TEST(4, Comparability, comparability, 1, 1, 1);
		TEST(5, XXX, connected, 0, 0, 0);
		TEST(6, XXX, tree, 0, 0, 0);
		TEST(7, XXX, forest, 0, 0, 0);
		TEST(8, XXX, clique, 0, 0, 0);
		TEST(9, XXX, cliques, 0, 0, 0);
		TEST(10, XXX, regular, 0, 0, 0);
		TEST(11, XXX, path, 0, 0, 0);
		TEST(12, XXX, caterpillar, 0, 0, 0);
		TEST(13, XXX, cycle, 0, 0, 0);
		TEST(14, XXX, matching, 0, 0, 0);
		TEST(15, XXX, subcubic, 0, 0, 0);
		TEST(16, XXX, cubic, 0, 0, 0);
		TEST(17, XXX, block, 0, 0, 0);
		TEST(18, XXX, almostTree, 0, 0, 0);
		TEST(19, XXX, compBipartite, 0, 0, 0);
		TEST(20, XXX, compMPartite, 0, 0, 0);
		TEST(21, XXX, cochordal, 0, 0, 0);
		TEST(22, XXX, split, 0, 0, 0);
		TEST(23, XXX, interval, 0, 0, 0);
#ifndef NOSLOW
		if(kg.getVertNo() <= MAXSLOW) { TEST(24, XXX, cocomparability, 0, 0, 0); };
		if(kg.getVertNo() <= MAXSLOW) { TEST(25, XXX, prime, 0, 0, 0); };
#endif
		TEST2(26, Euler, hasCycle, 0, 0, 0);
		TEST2(27, Euler, hasPath, 0, 0, 0);
#endif

#ifdef PROGRESS
		PLOG("OK");
#endif

		++k;
		int interval = INTERVAL;
		if(input != INP_GRAPH_RANDOM) interval *= 10;
		if(k % interval == 0) {
			cerr << "NG:" << k;
#ifndef NOBOOST
			cerr << " T:K/B[ms]:";
			cerr << " MT:" << ksum[0] * 1000 / k << " vs " << bsum[0] * 1000 / k;
			cerr << " SC:" << ksum[1] * 1000 / k_blocks << " vs " << bsum[1] * 1000 / k_blocks;
#endif
#ifndef NOCLASSES
			for(int zz = 1; zz < 26; zz++) cerr << " " << classes[zz];
#endif
			cerr << "\n";
			};
#ifndef NOGRAPHLOG
		graphs << endl;
#endif
		CATCHKOALA
		};

#ifndef NOGRAPHLOG
	graphs.close();
#endif
	errors.close();
	return dummyCounter % 1000 != 0;
	};
