
#include<stdio.h>
#include<string.h>

#include<string>
#include <fstream>

#include"./Koala/graph/graph.h"
#include"./Koala/io/g6.h"
#include"./Koala/io/text.h"
#include"./Koala/container/set.h"

using namespace std;
using namespace Koala;
using namespace Koala::IO;

Graph<char,string>::PEdge tabE[10];
std::pair<Graph<char,string>::PEdge,Graph<char,string>::PVertex> tabPair[10];

struct Dummy {
    int a;
};

struct bezop {
    int a,b;
};

struct Lapacz {
    int a;

    template <class T>
    Lapacz& operator=(const T&){}
};

AssocTable<std::map<int,Lapacz> > mapint1,mapint2;
Set<int> setint;


template <class T>
void printInfo(T* w)
{
    if (w ) cout << w->info; else cout << "NULL";
}

//template <class A, class B>
//void operator << (const A&,const B&) {}

#include "main.hpp"
	string buf;
	Graph<char,string> g,g2;
	Graph<char,string>::PVertex A,B,C,D,E,F,tabV[10];
    Graph<char,string>::PEdge d,e,f,h,p,q,x,y,z;
    char text[10000];

    AssocTable<std::map<Graph<char,string>::PVertex,int> > v2int;
    AssocTable<std::map<Graph<char,string>::PEdge,int> > e2int;
    std::map<int,Graph<char,string>::PVertex> int2v;
    std::map<int,Graph<char,string>::PEdge> int2e;


	A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
//	C=g.addVert();B=g.addVert();A=g.addVert();D=g.addVert();


	d=g.addEdge(A,B,"ab");
	e=g.addEdge(A,C,"ac");
	f=g.addArc(B,C,"bc");
	h=g.addLoop(C,"cc");
	g.addLoop(C,"cc2");
	g.addLoop(C,"cc3");
	x=g.addArc(A,C,"ac1");
	y=g.addArc(C,A,"ac2");
	z=g.addEdge(C,B,"bc2");
	g.addArc(C,A,"ac3");
	g.addArc(A,C,"ac4");
//	g.addEdge(A,B);g.addArc(B,C);g.addLoop(C);
    z=g.addLoop(D,"DD");g.addLoop(D,"dd");


	cout << std::boolalpha;
	cout << writeGraphText(g, cout, RG_VertexLists);
	cout << endl;

    cout << g.getParals(tabE,z,EdUndir);
    for(int i=0;i<10;i++) if (tabE[i]) std::cout<< "{"<< g.getEdgeEnds(tabE[i]).first->info <<
                            "," << g.getEdgeEnds(tabE[i]).second->info << "}";

//	for(z=g.getEdge(C,EdUndir|EdDirIn|EdDirOut);z;z=g.getEdgeNext(C,z,EdUndir|EdDirIn|EdDirOut))
//        cout << z->info << "\n";

//    Graph<char,string>::PEdge tabe[5]={e,e,x,f,y};
//    Set<Graph<char,string>::PEdge> s; s+=x; s+=y; s+=z;

//	cout << "\n:" <<g.delParals(C,EdUndir) << endl;
	cout << writeGraphText(g, cout, RG_VertexLists);

    cout << "\n******************\n";


    std::pair<bool,bool> form =std::make_pair(false,false);
    RG_Format eform = (RG_Format)(((form.first) ? RG_VInfo : 0)+((form.second) ? RG_EInfo : 0));

    cout << writeGraphText(g, cout, RG_VertexLists);
    cout << endl;
    cout << writeGraphText(g, cout, (RG_Format)(RG_VertexLists|eform));
    cout << endl;cout << endl;

    cout << writeGraphText(g, text, 10000, RG_EdgeList|RG_Info);
    cout << "text=" << text << "text"<<endl;
    g.clear();
    cout << readGraphText(g, text, RG_EdgeList|RG_Info);
    cout << writeGraphText(g, cout, RG_VertexLists|RG_Info);
    cout << endl;cout << endl;


//	writeGraphText(g, cout, RG_UndirectedVertexLists);
//	cout << endl;
//    writeGraphText(g, cout, RG_UndirectedVertexLists,form);
//	cout << endl;cout << endl;

	writeGraphText(g, cout, RG_EdgeList);
	cout << endl << endl;
    writeGraphText(g, cout, RG_EdgeList+eform);
	cout << endl;cout << endl;

	cout << "\n----------------\n";

    v2int[g.getVertNext(g.getVert())]=99;
    e2int[g.getEdge(EdLoop)]=102;

	writeGraphText(g, cout, RG_VertexLists|RG_Info,v2int,e2int);
    cout << "\n!!!!!!!!!!!!!!!!\n";
    {


//    typedef bezop Char;
        typedef char Char;
        typedef bezop String;
    //    typedef string String;

//        Graph<Char,String> _g;
//        _g.copy(g);
//        AssocTable<std::map<Graph<Char,String>::PVertex,int> > _v2int;
//        AssocTable<std::map<Graph<Char,String>::PEdge,int> > _e2int;
//        _v2int[_g.getVertNext(_g.getVert())]=99;
//        _e2int[_g.getEdge(EdLoop)]=102;
//        writeGraphText(_g, cout, RG_VertexLists|RG_Info,_v2int,_e2int);
//


        int format = RG_EdgeList;
//        int format = RG_VertexLists;
        cout << "\ncccccccccccccccc\n";
        char tekst[1000]={0};
        writeGraphText(g, tekst, 1000, format|RG_Info
                       ,v2int,e2int);
        cout << '\n' << tekst << '\n';

    typedef bezop Char3;
//        typedef char Char3;
        typedef bezop String3;
//        typedef string String3;

        Graph<Char3,String3> g3;
        readGraphText(g3, tekst, format|RG_Info
                      ,mapint1,mapint2);
        writeGraphText(g3, cout,  format|RG_Info);

        mapint1.getKeys(setInserter(setint));
        cout << endl << endl << setint;
        setint.clear();
        mapint2.getKeys(setInserter(setint));
        cout << endl << endl << setint;


//         return 0; //qqq

    }
	writeGraphText(g, cout, RG_EdgeList|RG_Info,blackHole,blackHole);
	ofstream pliko("plik.txt");
//	writeGraphText(g, pliko, RG_VertexLists,form,v2int,e2int);
	writeGraphText(g, pliko, RG_EdgeList+eform,v2int,e2int);
//	pliko << '\n';
//	writeGraphText(g, pliko, RG_VertexLists,std::make_pair(true,true),v2int,e2int);
	writeGraphText(g, pliko, RG_EdgeList|RG_Info);
	pliko.close();
	cout << "\n----------------\n";

	ifstream pliki("plik.txt");
	g.clear();
	readGraphText(g, pliki, RG_EdgeList);
	g.clear();
//	readGraphText(g, pliki, RG_VertexLists,int2v,int2e);
	readGraphText(g, pliki, RG_EdgeList);
	pliki.close();
	writeGraphText(g, cout, RG_VertexLists);

    cout << "\n----------------\n";

#define VT float
#define ET double
#define Mask RG_VertexLists


    Graph<VT,ET> gr;
    gr.addVert();gr.addVert();
    gr.addArc(gr.getVert(),gr.getVertLast());
    ofstream pliko2("plik2.txt");
    writeGraphText(gr, cout, Mask|RG_Info );
	writeGraphText(gr, pliko2, Mask);
	pliko2.close();
    cout << endl;
	ifstream pliki2("plik2.txt");
	gr.clear();
	readGraphText(gr, pliki2, Mask);
	pliki2.close();
	g.clear();

	writeGraphText(gr, cout, Mask|RG_Info );
	pliki.clear();
	pliki.open("plik.txt");
	readGraphText(g, pliki, RG_EdgeList,int2v,int2e);
    cout << "\n!!!!!!!!!!!!!!!!\n";
	writeGraphText(g, cout, RG_EdgeList|RG_Info );
    g.clear();
	readGraphText(g, pliki, RG_EdgeList,blackHole,blackHole);
	writeGraphText(g, cout, RG_EdgeList );


    cout << "\n\n" << int2v[99]->info << ' ' << int2e[102]->info;


//	Graph<> gc; gc.copy(g);
//	writeGraphText(gc, cout, RG_VertexLists);

//
//	writeGraphText(g, cout, RG_UndirectedEdgeList);
//	cout << endl;
//    writeGraphText(g, cout, RG_UndirectedEdgeList,form);
//	cout << endl;cout << endl;

        bezop bz1,bz2;
        {   char tab[100];
            g.clear();g2.clear();
            cout << "\n\n*******\n";
            A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
            g.addEdge(B,A,"ab");g.addArc(B,C,"bc");g.addArc(B,A,"b>a");g.addArc(C,D,"cd");
            g.addEdge(D,A,"ad");
            g.addLoop(B,"bb");
            g.getVerts(tabV);
            g2.addVert('E');
            writeGraphText(g, cout, RG_VertexLists|RG_Info);
            cout << "\n"<< A<<"\n\n";
            writeGraphText(g2, cout, RG_VertexLists|RG_Info);
            cout  << "\n*******\n";
            cout << "movev:" << g2.move2(g,tabV,tabV+2,EdDirOut).second;

           writeGraphText(g, cout, RG_VertexLists|RG_Info);
            cout << "\n\n";
            writeGraphText(g2, cout, RG_VertexLists|RG_Info);
            cout << "\n"<< A << "\n";
//             << sizeof(Directed) << ' '<<sizeof(EdDirIn|EdDirOut)
//            << "\n" << int(EdAll&(~Undirected)) << ' '<<int(EdAll^Undirected);

//            g.pickVert(B);
//            writeGraphText(g, cout, RG_VertexLists|RG_Info);

//            cout << "\n" << g.getVertNo() << "\n*******\n";
            //return 0;

        }
        {
            std::pair<Graph<char,string>::PEdge,Graph<char,string>::PVertex> para;
            g.clear();
            cout << "!!!!!!!!!!\n";
            A=g.addVert('A');B=g.addVert('B');C=g.addVert('C');D=g.addVert('D');
           	d=g.addEdge(A,B,"ab");
            e=g.addEdge(A,C,"ac");
            f=g.addArc(A,D,"ad");
            h=g.addLoop(A,"AA");
            x=0;
            do
            {
                y=g.getEdgePrev(A,x);
//                if (!x) break;
                printInfo(x); cout << "  e: ";
                printInfo(g.getEdgeVertPrev(A,x).first);
                cout <<"  v:";
                printInfo(g.getEdgeVertPrev(A,x).second);
                cout <<"\n"; x=y;

            } while(y);
            cout <<"\n" << g.getEdgeVertNo(A) << "\n";
            printInfo(g.getEdgeVert(A,EdUndir).first); cout << ' '; printInfo(g.getEdgeVert(A,EdUndir).second);
            cout <<"\n";
            printInfo(g.getEdgeVertLast(A,EdUndir).first); cout << ' '; printInfo(g.getEdgeVertLast(A,EdUndir).second);

            cout <<"\n\n";
            int res=g.getEdgeVerts(tabPair,A);
            for(int i=0;i<res;i++)
            {
                printInfo(tabPair[i].first); cout << ' '; printInfo(tabPair[i].second);cout <<"\n";
            }
            g2.move(g);

            cout << "\n" <<
            //A->getGraph() << d->getGraph() <<
                &g2 << " " << g2.getEdgeNo() <<"\n";
            g2.makeAdjMatrix();
            g2.delAdjMatrix();
            cout << g2.hasAdjMatrix() << g2.allowedAdjMatrix() ;

            g2.getVertNo();

        }
        {
            g.clear();
            cout << "\n!!!!!!!!!!\n";
            A=g.addVert('A');B=g.addVert('B');
           	d=g.addEdge(A,B,EdDirOut|EdLoop);
            writeGraphText(g, cout, RG_VertexLists|RG_Info);
        }

//    cin << bz1;


return 0;
}
