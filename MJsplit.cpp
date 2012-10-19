#include<iostream>
#include<string.h>

#include "Koala/io/g6.h"
#include "Koala/classes/detect.h"
#include "Koala/graph/graph.h"


using namespace std;
using namespace Koala;
using namespace Koala::IO;

#include "main.hpp"

    Graph<char,std::string> g;
    Graph<char,std::string>::PVertex tabv[9];
    int tab[9][9]={{0, 0, 0, 0, 0, 0, 1, 0, 1}, {0, 0, 0, 0, 0, 0, 1, 1, 0}, {0, 0, 0,
  0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0,
  0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0, 0, 0,
  0}, {0, 1, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0}};

    //readG6(g,"H??EAA?"); //read dziala dobrze bo sprawdzilem
    for(int i=0;i<9;i++) tabv[i]=g.addVert('A'+i);
    for(int i=0;i<8;i++) for(int j=i+1;j<9;j++) \
        if (!tab[i][j]) g.addEdge(tabv[j],tabv[i]);
    //cout << IsIt::split(g) << endl;
    //cout << IsIt::cochordal(g) << endl; //tu to  samo
    cout << IsIt::chordal(g)  << endl;   //tu dobrze
}
/*Jest problem z wieloma grafami np. grafem H??EAA? na 9 wierzcholkach:
{{0, 0, 0, 0, 0, 0, 1, 0, 1}, {0, 0, 0, 0, 0, 0, 1, 1, 0}, {0, 0, 0,
  0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0,
  0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0, 0, 0,
  0}, {0, 1, 0, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 0, 0, 0}}

A koala odpowiada cos takiego:
terminate called after throwing an instance of 'Koala::Error::ContExcFull'
Aborted

*/
