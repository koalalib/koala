#include<stdio.h>
#include<string.h>

#include<string>

#include"./koala/graph/graph.h"
#include"./koala/io/text.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;


bool Test(const char *prompt, const char *buf, const char *res) {
	if(strcmp(buf, res) != 0) {
		printf("error!\n");
		printf("%s: is:\n%s\ninstead of:\n", prompt, buf, res);
		return false;
		};
	return true;
	};

int main() {
	int i;
	bool rv;
	char buf[65536];
	string buf2;
	Graph<int, int> g; g.makeAdjMatrix();
	Graph<int, int>::Vertex *v, *verts2[32], *verts[2] = {NULL, NULL};

	rv = true;

	readGraphText(g, "0(0)@1 2 1(101) 2(102)\n"
			 "1(1) 1 2(112)\n"
			 "2(2) 0\n"
			 "3(3)@0 0\n", RG_UndirectedVertexLists, verts);

	for(i = 0, v = g.getVert(); v != NULL; v = g.getVertNext(v)) verts2[i++] = v;
	if(verts[0] != verts2[3] || verts[1] != verts2[0]) {
		printf("vertex output-id error\n");
		rv |= false;
		};

	writeGraphText(g, buf, 65536, RG_UndirectedVertexLists);
	rv |= Test("UndirectedVertexLists", buf,
		   "0(0) 2 u1(101) u2(102)\n"
		   "1(1) 1 u2(112)\n"
		   "2(2) 0\n"
		   "3(3) 0\n");

	writeGraphText(g, buf, 65536, RG_DirectedVertexLists);
	rv |= Test("DirectedVertexLists", buf,
		   "0(0) 2 u1(101) u2(102)\n"
		   "1(1) 1 u2(112)\n"
		   "2(2) 0\n"
		   "3(3) 0\n");

	writeGraphText(g, buf, 65536, RG_UndirectedEdgeList);
	rv |= Test("UndirectedEdgeList", buf,
		   "0 <-> 1(101)\n"
		   "0 <-> 2(102)\n"
		   "1 <-> 2(112)\n"
		   "0(0)\n"
		   "1(1)\n"
		   "2(2)\n"
		   "3(3)\n");

	writeGraphText(g, buf, 65536, RG_DirectedEdgeList);
	rv |= Test("DirectedEdgeList", buf,
		   "0 <-> 1(101)\n"
		   "0 <-> 2(102)\n"
		   "1 <-> 2(112)\n"
		   "0(0)\n"
		   "1(1)\n"
		   "2(2)\n"
		   "3(3)\n");


	Graph<string, string> g2; g2.makeAdjMatrix();
	readGraphText(g2, "0(v0) 2 1(e01) 2(e02)\n"
			 "1(v1) 1 2(e12)\n"
			 "3(v2) 0\n"
			 "3(v3) 0\n", RG_DirectedVertexLists);

	writeGraphText(g2, buf2, RG_UndirectedVertexLists);
	rv |= Test("UndirectedVertexLists", buf2.c_str(),
		   "0(v0) 2 d1(e01) d2(e02)\n"
		   "1(v1) 1 d2(e12)\n"
		   "2() 0\n"
		   "3(v3) 0\n");

	writeGraphText(g2, buf2, RG_DirectedVertexLists);
	rv |= Test("DirectedVertexLists", buf2.c_str(),
		   "0(v0) 2 d1(e01) d2(e02)\n"
		   "1(v1) 1 d2(e12)\n"
		   "2() 0\n"
		   "3(v3) 0\n");

	writeGraphText(g2, buf2, RG_UndirectedEdgeList);
	rv |= Test("UndirectedEdgeList", buf2.c_str(),
		   "0 -> 1(e01)\n"
		   "0 -> 2(e02)\n"
		   "1 -> 2(e12)\n"
		   "0(v0)\n"
		   "1(v1)\n"
		   "2()\n"
		   "3(v3)\n");

	writeGraphText(g2, buf2, RG_DirectedEdgeList);
	rv |= Test("UndirectedEdgeList", buf2.c_str(),
		   "0 -> 1(e01)\n"
		   "0 -> 2(e02)\n"
		   "1 -> 2(e12)\n"
		   "0(v0)\n"
		   "1(v1)\n"
		   "2()\n"
		   "3(v3)\n");
	if(rv == false) {
		printf("THERE WERE ERRORS!\n");
	} else {
		printf("ok\n");
		};
	return 0;
	};
