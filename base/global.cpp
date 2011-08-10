#include <cassert>
#include <iterator>


namespace Koala {

//Jesli metoda chce iterator do zapisu ciagu, a nas sam ciag nie interesuje, tylko inne efekty uboczne procedury
struct BlackHole : public std::iterator<std::output_iterator_tag,void,void,void,void> {
    template <class T>
    BlackHole& operator=(T) { return *this; }
    BlackHole& operator* () { return *this; }
    BlackHole& operator++() { return *this; }
    BlackHole operator++ (int) { return *this; }
    BlackHole() {}

    // rowniez moze sluzyc jako zaslepka dla nie interesujacego nas kontenera asocjacyjnego wymaganego w procedurze
    template <class T>
        BlackHole& operator[](T) { assert(0); return *this; }
    template <class T, class R>
        BlackHole& operator()(T,R) { assert(0); return *this; }
    template <class T>
        operator T() { assert(0); return T(); }

};

BlackHole blackHole;
int _KoalaEmptyVertDegree=0;
void* _KoalaEmptyEdgePoiner=0;



}



