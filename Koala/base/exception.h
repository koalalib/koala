#ifndef KOALA_EXCEPTION_H
#define KOALA_EXCEPTION_H

/* exception.h
 * 
 */

#include <cassert>
#include <cstdlib>
#include <cstring>

#ifndef KOALA_EXCEPTION_BUF_SIZE
    #define KOALA_EXCEPTION_BUF_SIZE 200
#endif

// Stała KOALA_DONT_CHECK_ERRORS wyłącza sprawdzanie błędów i rzucanie wyjątków przez Koalę.
#if defined(NDEBUG)
    #define KOALA_DONT_CHECK_ERRORS
#endif

// Makro koalaAssert() - pierwszy argument sprawdzany warunek, drugi typ rzucanego wyjątku.
#if defined(KOALA_DONT_CHECK_ERRORS)
    #define koalaAssert( descr,type ) {}
#else
    #define koalaAssert( descr,type ) { if (!(descr)) throw Koala::Error::type( #descr,__FILE__,__LINE__ ); }
#endif

namespace Koala
{
    namespace Error
    {
        /* ExcBase
         * Klasa bazowa dla wyjątków Koali.                                          
         */
        class ExcBase
        {
            char buf[KOALA_EXCEPTION_BUF_SIZE];
            int _line;

          public:
            // Parametry konstruktora określają, co mają zwracać metody line, descr i file.
            ExcBase( const char *adesc = "", const char *afile = "", int aline = -1);

            // Wiersz w kodzie, gdzie wystapił błąd.
            inline int line() const { return _line; }
            // Opis wyjątku.
            inline const char *descr() const { return buf; }
            // Plik źrodłowy, w którym wystąpił błąd.
            inline const char *file() const { return buf + std::strlen( buf ) + 1; }
            // Nazwa typu wyjątku.
            inline const char *type() const { return "ExcBase"; }
        };
        
        /* ExcWrongArg
         * Wyjątek typu "niepoprawny argument".
         */
        class ExcWrongArg: virtual public ExcBase
        {
          public:
            inline ExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ExcWrongArg"; }
        };

        /* ExcNullVert
         * Wyjątek typu "podano zerowy wierzchołek".
         */
        class ExcNullVert: public ExcWrongArg
        {
          public:
            inline ExcNullVert( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ExcNullVert"; }
        };

        /* ExcNullEdge
         * Wyjątek typu "podano zerową krawędź".
         */
        class ExcNullEdge: public ExcWrongArg
        {
          public:
            inline ExcNullEdge( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ExcNullEdge"; }
        };
        
        /* ExcWrongConn
         * Wyjątek typu "błędne połączenie w grafie między wierzchołkami lub krawędziami".
         */
        class ExcWrongConn: public ExcWrongArg
        {
          public:
            inline ExcWrongConn( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ExcWrongConn"; }
        };

        /* ExcWrongMask
         * Wyjątek typu "zła maska typu krawędzi".
         */
        class ExcWrongMask: public ExcWrongArg
        {
          public:
            inline ExcWrongMask( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ExcWrongMask"; }
        };

        /* ContExc
         * Klasa bazowa dla błędów struktury danych.
         */
        class ContExc: virtual public ExcBase
        {
          public:
            inline ContExc( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ContExc"; }
        };
        
        /* ContExcWrongArg
         * Wyjątek typu "zły argument metody kontenera".
         */
        class ContExcWrongArg: public ContExc, public ExcWrongArg
        {
          public:
            inline ContExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ContExcWrongArg"; }
        };
        
        /* ContExcFull
         * Wyjątek typu "przepełnienie kontenera".
         */
        class ContExcFull: public ContExc
        {
          public:
            inline ContExcFull( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ContExcFull"; }
        };

        /* ContExcOutpass
         * Wyjątek typu "przekroczenie zakresu kontenera".
         */
        class ContExcOutpass: public ContExcWrongArg
        {
          public:
            inline ContExcOutpass( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "ContExcOutpass"; }
        };

        /* GraphExc
         * Klasa bazowa dla blędów struktury grafu.
         */
        class GraphExc: virtual public ExcBase
        {
          public:
            inline GraphExc( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "GraphExc"; }
        };

        /* GraphExcWrongArg
         * Wyjątek typu "zły argument metody struktury grafu".
         */
        class GraphExcWrongArg: public GraphExc, public ExcWrongArg
        {
          public:
            inline GraphExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "GraphExcWrongArg"; }
        };

        /* GraphExcWrongConn
         * Wyjątek typu "zły argument metody struktury grafu, błędne połączenie".
         */
        class GraphExcWrongConn: public GraphExc, public ExcWrongConn
        {
          public:
            inline GraphExcWrongConn( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "GraphExcWrongConn"; }
        };
        
        /* GraphExcNullVert
         * Wyjątek typu "zły argument metody struktury grafu, podano zerowy wierzchołek".
         */
        class GraphExcNullVert: public GraphExc, public ExcNullVert
        {
          public:
            inline GraphExcNullVert( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "GraphExcNullVert"; }
        };
        
        /* GraphExcNullEdge
         * Wyjątek typu "zły argument metody struktury grafu, podano zerową krawędź".
         */
        class GraphExcNullEdge: public GraphExc, public ExcNullEdge
        {
          public:
            inline GraphExcNullEdge( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "GraphExcNullEdge"; }
        };

        /* GraphExcWrongMask
         * Wyjątek typu "zły argument metody struktury grafu, podano złą maskę typu".
         */
        class GraphExcWrongMask: public GraphExc, public ExcWrongMask
        {
          public:
            inline GraphExcWrongMask( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "GraphExcWrongMask"; }
        };
        
        /* AlgExc
         * Klasa bazowa dla błędów algorytmów biblioteki.
         */
        class AlgExc: virtual public ExcBase
        {
          public:
            inline AlgExc( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "AlgExc"; }
        };
        
        /* AlgExcWrongArg
         * Wyjątek typu "zły argument algorytmu biblioteki".
         */
        class AlgExcWrongArg: public AlgExc, public ExcWrongArg
        {
          public:
            inline AlgExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "AlgExcWrongArg"; }
        };
        
        /* AlgExcNullVert
         * Wyjątek typu "zły argument algorytmu biblioteki, podano zerowy wierzchołek".
         */
        class AlgExcNullVert: public AlgExc, public ExcNullVert
        {
          public:
            inline AlgExcNullVert( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "AlgExcNullVert"; }
        };
        
        /* AlgExcNullEdge
         * Wyjątek typu "zły argument algorytmu biblioteki, podano zerową krawędź".
         */
        class AlgExcNullEdge: public AlgExc, public ExcNullEdge
        {
          public:
            inline AlgExcNullEdge( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "AlgExcNullEdge"; }
        };

        /* AlgExcWrongMask
         * Wyjątek typu "zły argument algorytmu biblioteki, podano zlą maskę typu".
         */
        class AlgExcWrongMask: public AlgExc, public ExcWrongMask
        {
          public:
            inline AlgExcWrongMask( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "AlgExcWrongMask"; }
        };
        
        /* AlgExcWrongConn
         * Wyjątek typu "zły argument metody struktury grafu, błędne połączenie".
         */
        class AlgExcWrongConn: public AlgExc, public ExcWrongConn
        {
          public:
            inline AlgExcWrongConn( const char *adesc = "", const char *afile = "", int aline = -1):
                ExcBase( adesc,afile,aline ) { }
            inline const char *type() const { return "GraphExcWrongConn"; }
        };
    }
}

#include "exception.hpp"
#endif
