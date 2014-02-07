
#include<iostream>
#include<string.h>


#include"Koala/graph/graph.h"
#include"Koala/graph/view.h"
#include "Koala/algorithm/search.h"

using namespace std;
using namespace Koala;

//Powiedzmy, ze wszystko bedziesz na razie pisal tak, aby dzialalo w main i ew. okolicznych fukcjach.
//Pozniej zamkniemy to w klasie biblioteki.

//Graf jest obiektem, wierzcholki i krawedzie reprezentuja pewne jego obiekty wewnetrzne, do ktorych nie masz
//bezposredniego dostepu. Dlatego wierzcholki i krawedzie na ktorych dzialasz, to tak na prawde wskazniki
//(od tych wewnetrzncyh obiektow) udostepniane przez graf. Definiujac typ grafu podajesz takze typy etykiet
//przypisanych wierzcholkom i krawedziom. Sa one polami o nazwie info wierzcholka/krawedzi - jedynymi bezposrednio
//dostepnymi polami tych obiektow.




#include "main.hpp"

//1) W mainie utworzenie grafu i przekazanie go
//Zalozmy, ze tworzysz graf o wierzcholkach z etykieta char i krawedziach z etykieta string

    Graph<char,std::string> g; // poczatkowo jest pusty

//Dodaj, usuñ krawêdŸ(krawêdzie), wierzcho³ek(wierzcho³ki).
//Zalozmy, ze tworzysz graf zlozony z 2 cykli ABCD i AEFG oraz wierzcho³kiem H doczepionym do G

	Graph<char,std::string>::PVertex A,B,C,D,E,F,G,H,I; // to beda wierzcholki

    A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
    E=g.addVert('E');F=g.addVert('F');G=g.addVert('G');H=g.addVert('H');

    g.addEdge(A,B,"ab",EdUndir);g.addEdge(C,B,"bc",EdUndir);g.addEdge(C,D,"cd",EdUndir);g.addEdge(A,D,"ad",EdUndir);
    g.addEdge(A,E,"ae",EdUndir);g.addEdge(E,F,"ef",EdUndir);g.addEdge(F,G,"fg",EdUndir);g.addEdge(A,G,"ag",EdUndir);
    g.addEdge(G,H,"gh",EdUndir);

// o to jest za duzo:

    I=g.addVert('I');g.addEdge(I,G,"zbedna krawedz przy I",EdUndir);
    Graph<char,std::string>::PEdge e0;
    e0=g.addEdge(A,C,"zbedna krawedz",EdUndir);

// usuwamy:
    g.delEdge(e0);
// usuwamy wierzcholek, z jego wszystkimi krawedziami
    g.delVert(I);
// 1) Czy jest krawêdzi¹.
//nie rozumiem pytania. Liczby krawedzi i wierzcholkow grafu:

    std::cout << "n=" << g.getVertNo() << " m=" << g.getEdgeNo(EdUndir) << std::endl;
// PrzejdŸ po wszystkich krawêdziach, wierzcho³kach grafu.
//Po listach wierzcholkow krawedzi chodzisz metodami z Next/Prev w nazwach. Gdy nie ma odpowiedniego elementu
//grafu, wskaznik jest zerowy

    Graph<char,std::string>::PVertex v;
    Graph<char,std::string>::PEdge e;

// sprawdzanie istnienia krawedzi: tego nie zalecamy, bo graf domyslnie jest lista sasiadow, wiec koszt czasowy
// jest liniowy:

    if (g.getEdge(A,B,EdUndir)!=NULL) std::cout << "\njest krawedz " << A->info << B->info << std::endl;
    else std::cout << "\nnie ma krawedzi " << A->info << B->info << std::endl;

    if (g.getEdge(A,C,EdUndir)!=NULL) std::cout << "\njest krawedz " << A->info << C->info << std::endl;
    else std::cout << "\nnie ma krawedzi " << A->info << C->info << std::endl;


//    Przechodzimy po wierzcholkach wypisujac ich etykiety
    for(v=g.getVert();v!=NULL;v=g.getVertNext(v)) std::cout << v->info << ' ';
    std::cout << std::endl;
//  to samo z krawedziami
    for(e=g.getEdge(EdUndir);e!=NULL;e=g.getEdgeNext(e,EdUndir)) std::cout << e->info << ' ';
    std::cout << std::endl;
// 4) PrzejdŸ po wszystkich krawêdziach wierzcho³ka, podzbioru wierzcho³ków.
//  to samo z krawedziami sasiednimi do A

    for(e=g.getEdge(A,EdUndir);e!=NULL;e=g.getEdgeNext(A,e,EdUndir)) std::cout << e->info << ' ';

//1) Tworzenie zbioru
//2) Dodawanie, usuwanie ze zbioru
//    Wewnatrz algorytmow chyba najlepiej uzywac zwyklych tablic lokalnych operujac funkcjami STLa
  int max=6;
  int LOCALARRAY (tabint,max); // makro zastepujace int tabint[max] - nie wszystkie kompilatory akceptuja nieustalony
  //rozmiar rablicy;

  tabint[0]=1;tabint[1]=2;tabint[2]=-1;tabint[3]=2;tabint[4]=1;tabint[5]=-1;
  std::sort(tabint,tabint+max);
  int setsize=std::unique(tabint,tabint+max)-tabint;
  std::cout << "\nRozmiar zbioru to " << setsize << " a elementy: ";
  for(int i=0;i<setsize;i++) std::cout << tabint[i] << ' ';
  std::cout << std::endl;

  // mozna i tak, ale to wolniejsze:

  Set<int> intset1,intset2;
  intset1.add(1);intset1.add(3);
  intset2.add(2);intset2.add(3);intset2.add(4); intset2.add(5);
  intset2.del(4);
  // postac zbiorow
    std::cout << intset1 << ' ' << intset2 << std::endl;
  //ich suma i czesc wspolna;
    Set<int> intset3,intset4;
    intset3=intset1+intset2;intset4=intset1*intset2;
    std::cout << intset3 << ' ' << intset4 << std::endl;

// Jeœli chodzi o zbiory to myœlisz, ¿e zbiór krawêdzi lepiej w zbiorze czy w grafie trzymaæ dodatkowym?
// wolalbym nie, bo kopiowanie grafow nie jest efektywne

// 5) Podgraf indukowany grafu
// rozumiem, ze chodzi o krawedzie indukowane przez zbior wierzcholkow

    Graph<char,std::string>::PVertex tabV[100]={A,E,F,G,H}; // zestaw wierzcholkow
    Graph<char,std::string>::PEdge LOCALARRAY(tabe,g.getEdgeNo(EdUndir)); // tu zbierzemy krawedzie;

    int licze=g.getIncEdges(tabe,tabV,tabV+5,EdUndir,EdLoop);
    for(int i=0;i<licze;i++) std::cout << tabe[i]->info << ' ';
    std::cout << std::endl;

//6) DFS z numeracj¹ z algorytmu, zbiór krawêdzi drzewa oraz zbiór krawêdzi zwrotnych
// za tablice przechowujaca wynikowy ciag wierzcholkow posluzy tabV
// Jesli chcemy tylko numeracje, przy skanowaniu poczawszy od A to np.

    int licz=DFSPreorder::scanAttainable(g,A,blackHole,tabV,EdUndir);
    for(int i=0;i<licz;i++) std::cout << tabV[i]->info << ' ';
    std::cout <<  std::endl;

// a teraz pozostale elementy
// najpierw zalozmy tablice asocjacyjna przypisujaca wierzcholkom struktury przechowujace wyniki z algorytmu

    AssocArray<Graph<char,std::string>::PVertex,
            SearchStructs::VisitVertLabs<Graph<char,std::string> > > vertCont(g.getVertNo()); // podajemy rozmiar

    licz=DFSPostorder::scanAttainable(g,A,vertCont,tabV,EdUndir); // jesli chcemy przebiec caly graf, a nie tylko
    // skladowa zawierajaca A, mozna uzyc analogicznej funkcji scan
    std::cout << "Sekwencja: ";
    for(int i=0;i<licz;i++) std::cout << tabV[i]->info << ' ';
    std::cout <<  std::endl;
    for(int i=0;i<licz;i++) if (tabV[i]!=A)
        std::cout << "Wierzcholek " << tabV[i]-> info << " : rodzic " << vertCont[tabV[i]].vPrev->info
            << " krawedz drzewa " << vertCont[tabV[i]].ePrev->info << " odleglosc od roota "
            << vertCont[tabV[i]].distance << std::endl;

// Dwuspójne sk³adowe (zbiory wierzcho³ków)
    int LOCALARRAY(tabcomp,g.getVertNo()+1); // tablica na pozycje kolejnych blokow
    AssocArray<Graph<char,std::string>::PEdge, int> edgeCont(g.getEdgeNo());    // i przynaleznosc krawedzi do nich
    int liczc=Blocks::splitComp(g,A,blackHole,edgeCont,SearchStructs::compStore(tabcomp,tabV),blackHole);
// znow jesli chcesz rozbic caly graf, a nie tylko skladowa z A, uzywasz split
    std::cout << "Liczba blokow: " << liczc << '\n';
    for(int i=0;i<liczc;i++)
    {
        std::cout << i << ':';
        for(int j=tabcomp[i];j<tabcomp[i+1];j++) std::cout << tabV[j]->info;
        std::cout <<  std::endl;
    }
    std::cout <<  std::endl;
    for(e=g.getEdge(EdUndir);e!=NULL;e=g.getEdgeNext(e,EdUndir))
        std::cout << "\nKrawedz "<< e->info << " lezy w bloku o numerze " << edgeCont[e];
     std::cout <<  "\n\n";

// dzialanie na bloku nr 1 oryginalnego grafu, mozna dzialac tylko metodami const.
// Jesli chcesz je koniecznie modyfikowac, chyba rzeczywiscie lepiej zrobic kopie:
    AssocArray<Graph<char,std::string>::PVertex, char> subgrTab(g.getVertNo());
    int nrBloku=1;
    for(int j=tabcomp[nrBloku];j<tabcomp[nrBloku+1];j++) subgrTab[tabV[j]]; // odznaczam w tablicy zbior wierzcholkow
    // jesli potrzebujesz go tylko na chwile, mozna dzialac na wyniku funkcji
    std::cout << makeSubgraph(g,std::make_pair(extAssocKeyChoose(&subgrTab),stdChoose(true))).getVertNo() << std::endl;
    // w przeciwnym razie lepiej go zapisac:
    Subgraph<Graph<char,std::string>,AssocHasChooser<AssocArray<Graph<char,std::string>::PVertex, char>*> ,BoolChooser>
        subgr= makeSubgraph(g,std::make_pair(extAssocKeyChoose(&subgrTab),stdChoose(true)));
    // i dale wio ..
    for(v=subgr.getVert();v!=NULL;v=subgr.getVertNext(v)) std::cout << v-> info << ' ';
    std::cout << std::endl;
    for(e=subgr.getEdge(EdUndir);e!=NULL;e=subgr.getEdgeNext(e,EdUndir)) std::cout << e-> info << ' ';

    return 0;
}
