#ifndef KOALA_EXCEPTION_H
#define KOALA_EXCEPTION_H

#include <cassert>
#include <cstdlib>
#include <cstring>

#ifndef KOALA_EXCEPTION_BUF_SIZE
#define KOALA_EXCEPTION_BUF_SIZE 200
#endif

// Stala czasu kompilacji KOALA_DONT_CHECK_ERRORS wylacza sprawdzanie bledow i rzucanie wyjatkow przez Koale
// Ten sam efekt ma wylaczenie makra assert
//#define KOALA_DONT_CHECK_ERRORS

#if defined(NDEBUG)
#define KOALA_DONT_CHECK_ERRORS
#endif

// makro koalaAssert - pierwszy argument sprawdzany warunek, drugi typ rzucanego wyjatku
#if defined(KOALA_DONT_CHECK_ERRORS)
#define koalaAssert(descr,type) {}
#else
#define koalaAssert(descr,type) { if (!(descr)) throw Koala::Error::type(#descr,__FILE__,__LINE__); }
#endif

namespace Koala {

namespace Error {



// klasa podstawowa koalowego wyjatku
class ExcBase {
    char buf[KOALA_EXCEPTION_BUF_SIZE];
    int _line;

    public:

    inline ExcBase(const char* adesc="", const char* afile="", int aline=-1) : _line(aline)//, w(buf+std::strlen(adesc)+1)
    // parametry konstruktora - to co maja zwracac metody line, descr i file
    {
        assert(std::strlen(adesc)+std::strlen(afile)+2<=KOALA_EXCEPTION_BUF_SIZE);
        std::strcpy(buf,adesc);
        std::strcpy(buf+std::strlen(adesc)+1,afile);
    }

    // wiersz w kodzie, gdzie wystapil blad
    inline int line() const { return _line; }
    // opis wyjatku
    inline  const char* descr() const { return buf; }
    // plik zrodlowy, w ktorym wystapil blad
    inline const char* file() const { return buf+std::strlen(buf)+1; }
    // nazwa typu wyjatku
    inline  const char* type() const { return "ExcBase"; }
};

// znacznik bledu niepoprawnego argumentu
class ExcWrongArg : virtual public ExcBase {
    public:

    inline ExcWrongArg(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
    inline const char* type() const { return "ExcWrongArg"; }
};

// podano zerowy wierzcholek
class ExcNullVert : public ExcWrongArg {
    public:

    inline ExcNullVert(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
    inline const char* type() const { return "ExcNullVert"; }
};

class ExcNullEdge : public ExcWrongArg {
    public:

    inline ExcNullEdge(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
    inline const char* type() const { return "ExcNullEdge"; }
};

// bledne polaczenie w grafie miedzy wierzcholkami lub krawedziami
class ExcWrongConn : public ExcWrongArg {
    public:

    inline ExcWrongConn(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
    inline const char* type() const { return "ExcWrongConn"; }
};

// podana zla maske typu krawedzi
class ExcWrongMask : public ExcWrongArg {
    public:

    inline ExcWrongMask(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
    inline const char* type() const { return "ExcWrongMask"; }; // podano zerowa krawedz

};

// klasa podstawiwa bledu struktury danych
class ContExc : virtual public ExcBase {
    public:

    inline ContExc(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
    inline const char* type() const { return "ContExc"; }
};

// zly argument metody kontenera
class ContExcWrongArg : public ContExc, public ExcWrongArg {
    public:

    inline ContExcWrongArg(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "ContExcWrongArg"; }

};

// przepelnienie  kontenera
class ContExcFull : public ContExc {
    public:

    inline ContExcFull(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "ContExcFull"; }

};

// pusty  kontener
//class ContExcEmpty : public ContExc {
//    public:
//
//    inline ContExcEmpty(const char* adesc="", const char* afile="", int aline=-1) : ContExc(adesc,afile,aline) {}
//     inline const char* type() const { return "ContExcEmpty"; }
//
//};

// przekroczenie zakresu kontenera
class ContExcOutpass : public ContExcWrongArg {
    public:

    inline ContExcOutpass(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "ContExcOutpass"; }

};


// klasa podstawiwa bledu struktury grafu
class GraphExc : virtual public ExcBase {
    public:

    inline GraphExc(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "GraphExc"; }

};

// zly argument metody struktury grafu
class GraphExcWrongArg : public GraphExc, public ExcWrongArg {
    public:

    inline GraphExcWrongArg(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "GraphExcWrongArg"; }

};

// zly argument metody struktury grafu, bledne polaczenie
class GraphExcWrongConn : public GraphExc, public ExcWrongConn {
    public:

    inline GraphExcWrongConn(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "GraphExcWrongConn"; }

};


// zly argument metody struktury grafu, podano zerowy wierzcholek
class GraphExcNullVert : public GraphExc, public ExcNullVert {
    public:

    inline GraphExcNullVert(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "GraphExcNullVert"; }

};


// zly argument metody struktury grafu, podano zerowa krawedz
class GraphExcNullEdge : public GraphExc, public ExcNullEdge {
    public:

    inline GraphExcNullEdge(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "GraphExcNullEdge"; }

};

// zly argument metody struktury grafu, podano zla maske typu
class GraphExcWrongMask : public GraphExc, public ExcWrongMask {
    public:

    inline GraphExcWrongMask(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "GraphExcWrongMask"; }

};


// klasa podstawiwa bledu algorytmu biblioteki
class AlgExc : virtual public ExcBase {
    public:

    inline AlgExc(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "AlgExc"; }

};

// zly argument algorytmu biblioteki
class AlgExcWrongArg : public AlgExc, public ExcWrongArg {
    public:

    inline AlgExcWrongArg(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "AlgExcWrongArg"; }

};

// zly argument algorytmu biblioteki, podano zerowy wierzcholek
class AlgExcNullVert : public AlgExc, public ExcNullVert {
    public:

    inline AlgExcNullVert(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "AlgExcNullVert"; }

};


// zly argument algorytmu biblioteki, podano zerowa krawedz
class AlgExcNullEdge : public AlgExc, public ExcNullEdge {
    public:

    inline AlgExcNullEdge(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "AlgExcNullEdge"; }

};

// zly argument algorytmu biblioteki, podano zla maske typu
class AlgExcWrongMask : public AlgExc, public ExcWrongMask {
    public:

    inline AlgExcWrongMask(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "AlgExcWrongMask"; }

};

// zly argument metody struktury grafu, bledne polaczenie
class AlgExcWrongConn : public AlgExc, public ExcWrongConn {
    public:

    inline AlgExcWrongConn(const char* adesc="", const char* afile="", int aline=-1) : ExcBase(adesc,afile,aline) {}
     inline const char* type() const { return "GraphExcWrongConn"; }

};


}
}

#endif
