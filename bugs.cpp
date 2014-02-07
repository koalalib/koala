#define NDEBUG
#include"./koala/io/g6.h"
#include"./koala/io/text.h"
#include"./koala/graph/graph.h"
#include"./koala/classes/detect.h"

using namespace Koala;
using namespace Koala::IO;
using namespace std;

#define KCATCH(e)	catch(e &ex) { cout << "Exception: " #e << endl; }

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
	int res;
	const char *t;
	Graph<int, int> g;
	Graph<int, int>::PVertex verts[1024];

	// error 1
	// leci wyj¹tek
	// detect.hpp: 928 -> conflow.hpp: 862 -> graph.hpp: 275
//	t = "27 0 3 -6 -23 -24 1 1 -5 2 2 -25 -6 3 0 4 0 5 2 -9 -25 6 2 -18 -7 7 1 -8 8 2 -21 -12 9 1 -18 10 0 11 1 -23 12 1 -26 13 1 -18 14 0 15 0 16 0 17 0 18 0 19 0 20 0 21 1 -22 22 0 23 0 24 0 25 0 26 0 ";
//	g.clear();
//	readGraphText(g, t, RG_VertexLists);
//	res = IsIt::Comparability::maxStable(g, verts);
//	cout << "res is " << res << "\n";

	// errors 2-11 - rozne wyniki  dla podanych parametrow
	// niektore od razu, niektore po wlaczeniu optymalizacji
	// i zdefiniowaniu NDEBUG (wylacza to kod w koalaAssertach

	// error 2; powinno byc 6
	t = "H??E@_m";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. indep. set = " << IsIt::Bipartite::maxStable(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. indep. set = " << IsIt::Chordal::maxStable(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. indep. set = " << IsIt::Cograph::maxStable(g, verts) << endl;
	CATCHKOALA

	// error 3
	t = "H??E@_m";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. vert cover = " << IsIt::Bipartite::minVertCover(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. vert cover = " << IsIt::Chordal::minVertCover(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. vert cover = " << IsIt::Cograph::minVertCover(g, verts) << endl;
	CATCHKOALA

	// error 4
	t = "I???E?oJO";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. indep. set = " << IsIt::Bipartite::maxStable(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. indep. set = " << IsIt::Chordal::maxStable(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. indep. set = " << IsIt::Cograph::maxStable(g, verts) << endl;
	CATCHKOALA

	// error 5
	t = "I???E?oJO";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. vert cover = " << IsIt::Bipartite::minVertCover(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. vert cover = " << IsIt::Chordal::minVertCover(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. vert cover = " << IsIt::Cograph::minVertCover(g, verts) << endl;
	CATCHKOALA

	// error 6
	t = "I???E?wHo";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. indep. set = " << IsIt::Bipartite::maxStable(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. indep. set = " << IsIt::Chordal::maxStable(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. indep. set = " << IsIt::Cograph::maxStable(g, verts) << endl;
	CATCHKOALA

	// error 7
	t = "I???E?wHo";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. vert cover = " << IsIt::Bipartite::minVertCover(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. vert cover = " << IsIt::Chordal::minVertCover(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. vert cover = " << IsIt::Cograph::minVertCover(g, verts) << endl;
	CATCHKOALA

	// error 8
	t = "I??CB?WDO";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. indep. set = " << IsIt::Bipartite::maxStable(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. indep. set = " << IsIt::Chordal::maxStable(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. indep. set = " << IsIt::Cograph::maxStable(g, verts) << endl;
	CATCHKOALA

	// error 9
	t = "I??CB?WDO";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. vert cover = " << IsIt::Bipartite::minVertCover(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. vert cover = " << IsIt::Chordal::minVertCover(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. vert cover = " << IsIt::Cograph::minVertCover(g, verts) << endl;
	CATCHKOALA

	// error 10
	t = "I??E@_KI_";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. indep. set = " << IsIt::Bipartite::maxStable(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. indep. set = " << IsIt::Chordal::maxStable(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. indep. set = " << IsIt::Cograph::maxStable(g, verts) << endl;
	CATCHKOALA

	// error 11
	t = "I??E@_KI_";
	g.clear();
	readG6(g, t);
	cout << "g6 = " << t << endl;
	TRYKOALA
	if(IsIt::bipartite(g)) cout << " Bip. vert cover = " << IsIt::Bipartite::minVertCover(g, verts) << endl;
	if(IsIt::chordal(g)) cout << " Chord. vert cover = " << IsIt::Chordal::minVertCover(g, verts) << endl;
	if(IsIt::cograph(g)) cout << " Cogr. vert cover = " << IsIt::Cograph::minVertCover(g, verts) << endl;
	CATCHKOALA

	return 0;
	};
