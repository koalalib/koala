//#include "graph.h"
#include "Koala/io/g6.h"
#include <iostream>
#include <stdio.h>

using namespace Koala;
using namespace Koala::IO;
using namespace std;

int main() {
	Graph<int,int> graph;
	string txt = "G?r@`_";
	readG6(graph, txt);
	writeG6(graph, txt);
	cout << txt << endl;
	graph.clear();
	txt = "B?";
	readG6(graph, txt);
	writeG6(graph, txt);
	cout << txt <<endl;
	writeG6(graph, txt);
	graph.clear();
	readG6(graph, txt);
	cout << txt <<endl;
	writeG6(graph, txt);
	graph.clear();
	readG6(graph, txt);
	cout << txt <<endl;
	return 0;
}
