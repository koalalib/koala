#include <iostream>
#include <string>

using namespace std;


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

//template <class T> struct Klasa;
//template <> struct Klasa<int> {};
//
//template <class T> struct Opak {
//    typedef bool typ;
//};
//
//template <> struct Opak<char> {
//    typedef int typ;
//};
//
//template <class T>
//   Klasa<typename Opak<T>::typ> fun(T) {}
//
//void fun(...) {};

int main()
{



    int* b;
    int a=(int)b;

    konwtest(a,b);

//    cout << "\n" << a << ' ' << b;


    return 0;
}

