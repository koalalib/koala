#include <iostream>
#include <string>
#include "Koala/graph/graph.h"
#include"./Koala/io/text.h"

using namespace std;
using namespace Koala;

struct InfoV1 {
    int a;
    string name;

    InfoV1(int n, string aname) : a(n), name(aname) {}
    InfoV1(){}
};

struct InfoV2 {
    int b;
    string napis;

    InfoV2(int n, string aname) : b(n), napis(aname) {}
    InfoV2(){}
};

int inc(char a)
{
    return a+3;
}

bool chtest( Vertex<char,char> *elem, const Graph<char,char> &graph )
{
    return elem!=graph.getVert();
}

InfoV2 vtrans(InfoV1);
InfoV2 vtrans(InfoV1 arg) { return InfoV2(0,arg.name); }

InfoV2 (*vt)(InfoV1)=vtrans;
typedef InfoV2 (*FPoint)(InfoV1);
FPoint vt2=vt;

class VFunktor {
    public:

    int a;
    InfoV2 operator() (InfoV1 arg) { return vtrans(arg); }
} vFunktor;

struct InfoE1 {
    int a;
    string name;

    InfoE1(int n, string aname) : a(n), name(aname) {}
    InfoE1() {}
} ein1;

struct InfoE2 {
    int b;
    string napis;

    InfoE2(int n, string aname) : b(n), napis(aname) {}
    InfoE2(){}
} ein2;


namespace Privates {

    namespace KonwTestSpace {


        template <class Dest,class Source> struct Przec {

            struct Lapacz {
                int a;
//                template <class T>
//                Lapacz(T) : a(1) {}
                    Lapacz(Source) : a(1) {}
            };


            static char przec(Dest,int)
            {
                cout << "\nkonwersja";
                return 'A';
            }

            static double przec(Lapacz,...)
            {
                cout << "\nniekonwersja";
                return 12.3;
            }

        };

        template <class Dest,class Sour> struct Przec<Dest*,Sour*> {

            template <class T>
            static char przec(T,int)
            {
                cout << "\nkonwersja";
                return 'A';
            }

        };


        template <int arg> struct Cast {

            template <class Sour,class Dest>
            static void make(Dest& d,const Sour& s)
            {
                cout << "\nBrak konwersji\n";
                d=Dest();
            }
        };

        template <> struct Cast<1> {

            template <class Sour,class Dest>
            static void make(Dest& d,const Sour& s)
            {
                cout << "\nKonwersja\n";
                d=(Dest)s;
            }
        };

    }
}

struct KonwTest {

    template <class Sour,class Dest>
    void operator()(Dest& d,const Sour& s)
    {
        ::Privates::KonwTestSpace::Cast<sizeof(::Privates::KonwTestSpace::Przec<Dest,Sour>::przec(s,12))>::make(d,s);
    }
} konwtest;

void przec(int,int)
{
    cout << "\nkonwersja";
}

struct Lapacz {
    int a;
    template <class T>
    Lapacz(T) : a(1) {}

};


struct AA {
    int a;

    AA(int arg=13) : a(arg) {}
    operator int() const
    {
        return a;
    }
};

struct CCC {
    int a;

    CCC(int arg=3): a(arg) {}

//    operator BBB() const
//    {
//        return BBB(a);
//    }
};


struct DDD {
    int a;

    DDD(): a(15) {}

    operator double() const
    {
        return 12;
    }
} ddd;


std::ostream &operator <<(std::ostream &sout, const CCC &p)
{
    sout << p.a;
    return sout;
}


struct BBB {
    int a;

    BBB(int arg=2): a(arg) {}
    BBB(CCC c) : a(c.a) {}

};


std::ostream &operator <<(std::ostream &sout, const BBB &p)
{
    sout << p.a;
    return sout;
}




void przec(Lapacz,...)
{
    cout << "\nniekonwersja";
}

//template <class T>
//void przec(T)
//{
//    cout << "\nniekonwersja";
//}



#include "main.hpp"



    Graph<InfoV1,InfoE1> g1,g1p=g1;
    Graph<InfoV2,InfoE2> g2,g2p;

    AssocArray<Vertex<InfoV1,InfoE1>*,Vertex<InfoV2,InfoE2>* > m12;
//    AssocTabInterface<AssocArray<Vertex<InfoV1,InfoE1>*,Vertex<InfoV2,InfoE2>* > > am12=m12;
    map<Vertex<InfoV2,InfoE2>*,Vertex<InfoV1,InfoE1>* > m21;
    AssocTabInterface< map<Vertex<InfoV2,InfoE2>*,Vertex<InfoV1,InfoE1>* > > am21=m21;

    map<Edge<InfoV1,InfoE1>*,Edge<InfoV2,InfoE2>* > n12;
    AssocTabInterface<map<Edge<InfoV1,InfoE1>*,Edge<InfoV2,InfoE2>* > > an12=n12;
    map<Edge<InfoV2,InfoE2>*,Edge<InfoV1,InfoE1>* > n21;
    AssocTabInterface< map<Edge<InfoV2,InfoE2>*,Edge<InfoV1,InfoE1>* > > an21=n21;



    Vertex<InfoV1,InfoE1> *A=g1.addVert(InfoV1(0,"A"));
    Vertex<InfoV1,InfoE1> *B=g1.addVert(InfoV1(1,"B"));
    Vertex<InfoV1,InfoE1> *C=g1.addVert(InfoV1(2,"C"));
    Vertex<InfoV1,InfoE1> *D=g1.addVert(InfoV1(2,"D"));
    Edge<InfoV1,InfoE1> *e=g1.addEdge(B,C,InfoE1(0,"BC"));
    Edge<InfoV1,InfoE1> *f=g1.addArc(B,D,InfoE1(0,"BD"));
    Edge<InfoV1,InfoE1> *h=g1.addArc(A,D,InfoE1(0,"BC"));
    Edge<InfoV1,InfoE1> *k=g1.addEdge(A,B,InfoE1(0,"BC"));

    Vertex<InfoV2,InfoE2>* A2=g2.addVert(),*B2=g2.addVert();
    g2.addArc(A2,B2);
    g2.substitute(B2,g1,stdChoose(true)&stdChoose(true),stdCast(&vtrans)&valCast()
            , stdLink(am21,m12) & stdLink(an21,an12));

    g2p.copy(g1);
    std::cout << "copy\n";

    //stdLink(&InfoE2::b);

    for(Vertex<InfoV2,InfoE2>* p=g2.getVert();p;p=g2.getVertNext(p))
        cout << ((void*)p) << "  " << p->info.napis << "  " << (void*)m21[p]<<  endl;
    cout << n12.size();

    std::map<Vertex<InfoV1,InfoE1>*,Vertex<InfoV2,InfoE2>* > saa;
    AssocArray<Vertex<InfoV1,InfoE1>*,Vertex<InfoV2,InfoE2>* > aa,aa2;
    aa2=aa;
    assocKeyChoose((aa));
    extAssocChoose((&aa),(Vertex<InfoV2,InfoE2>*) 0);
    stdChoose(Set<int*>());

    Graph<InfoV2,InfoE2> g2b=g2;
    cout << endl << g2b.getVertNo() << endl;
    g2b.copy(g2);
    cout << g2b.getVertNo() << endl;
    g2b=g2;
    cout << g2b.getVertNo() << endl;
    g2b+=g2b;
    cout << g2b.getVertNo() << endl;

//    cout << endl << g2.getVertNo() << endl << g2.getEdgeNo(A2,EdDirIn);
//        Vertex<InfoV1,InfoE1>* tab[6]={C,D,C,B,0,B};
//
//    Edge<InfoV1,InfoE1>* tabe[3]={g1.getEdge(),0,g1.getEdge()};
//    cout << endl << g1.glue(g1.getVertSet(),D)->info.name;
//    cout << "\n" << g1.getVertNo();
//        cout << endl << g1.glue(C,C)->info.name;
//
//
//
//    cout << endl << g1.getVertNo() << " " << g1.getEdgeNo() << " " << g1.getEdgeNo(A,EdUndir) << " " << g1.getEdgeNo(A,EdDirOut);
//
//    cout << endl << g1.hasAdjMatrix(); cout << " " << g1.makeAdjMatrix(); cout <<" " << g1.hasAdjMatrix();
//    cout << " " << g1.defragAdjMatrix(); cout << " " << g1.delAdjMatrix(); cout <<" " << g1.hasAdjMatrix ();
//        g1p.makeAdjMatrix();
//    g1p.move(g1);
//    cout << endl << g1p.hasAdjMatrix(); cout << " " << g1p.makeAdjMatrix(); cout <<" " << g1p.hasAdjMatrix();
//    cout << " " << g1p.defragAdjMatrix();
//    cout << " " << g1p.delAdjMatrix();
//    cout <<" " << g1p.hasAdjMatrix ();
//    cout << " Koniec\n";
//    cout << g1p.getEdgeSet() << endl;
//
//    Set<Edge<InfoV1,InfoE1>*> vset;
////    vset+=A; vset+=D;
//    cout << endl <<g1.getEdgeNo() << endl;
//    cout << g1.ch2Archs(g1.getEdgeSet());
//    cout << endl <<g1.getEdgeNo() << endl;
    {


        cout << "\n\n------------------\n";

        string s="ala";
        AA aa(15);
        int b=aa;
        przec(s,12);

        cout << "\n\n------------------\n";
        const char* ss="Ala";
        string dd;
        konwtest(dd,ss);


        cout << "\n" << dd << ' ' << ss;

        cout << "\n\n------------------\n";
#define SourType int
#define SourVal1 12
#define SourVal2 12
#define SourVal3 12
#define DestType void*


        Graph<SourType,SourType> g,gg;
        Graph<SourType,SourType>::PVertex A,B;
        A=g.addVert(SourVal1);B=g.addVert(SourVal2);
        g.addEdge(A,B,SourVal3);
        Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

        cout << "\n\n--\n\n";
        Graph<DestType,DestType> h;
        h.copy(g,stdChoose(true) & stdChoose(true));
        gg.copy(g);
//            h.copy(g);
        Koala::IO::writeGraphText(h, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

    }

    {
        cout << "\n\n!!!!!!!!!!!!!!\n\n";
//        Graph<DDD,DDD> g;
//        g.addVert(ddd);g.addVert(ddd);
//        Koala::IO::writeGraphText(g, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);

//        Graph<char,char> g;
//        g.addEdge(g.addVert('A'),g.addVert('B'),'C');
//
//        Graph<int,int> h;
//
//        h.copy(g,stdFChoose(&chtest)& stdChoose(true),stdCast(&inc) & hardCast());
//        Koala::IO::writeGraphText(h, cout, Koala::IO::RG_VertexLists|Koala::IO::RG_Info);
    }

    return 0;
}

