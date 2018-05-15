#include<math.h>
#include<stdio.h>

#define KOALA_USES_ALLOCA
#include"Koala/graph/graph.h"
#include"Koala/container/localarray.h"

using namespace Koala;

typedef struct _intpair {
	int a, b;
	} intpair;

unsigned int c = 0;
template<class A, class B>
struct S {
	A a[64];
	B b[64];
        S()	{ printf("create S %d\n", m_c = c++); };
        ~S()	{ printf("delete S %d\n", m_c); };
        unsigned int m_c;
	};

template<class GraphType>
void TT(GraphType &g, int n) {
    typename GraphType::PVertex LOCALARRAY(tabV,g.getVertNo());
    };


#include "main.hpp"
	S<int, double> LOCALARRAY(ht, 16);
	S<int, double> LOCALARRAY(st, 2);
	int LOCALARRAY(iht, 8192);
	int LOCALARRAY(ist, 128);

#if _MSC_VER >= 1500 	// Visual Studio 2008 or better
	struct { int a, b; } LOCALARRAY(ast, 5);
	printf("%d vs %d\n", sizeof(intpair), ast_KILLER.GetSize());
#endif

	printf("%p\n", ht);
	printf("%p\n", st);
	printf("%p\n", iht);
	printf("%p\n", ist);
	int i = (int)(100000 * sin(0.3));
	int LOCALARRAY(dst, i);
	printf("%p\n", dst);

	Graph<int, int> g;
	TT(g, 12);
#warning kom
	return 0;
	};
