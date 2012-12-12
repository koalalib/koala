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
	/** \brief Exceptions */
	namespace Error
	{
		// Klasa bazowa dla wyjątków Koali.
		/** \brief Exception base.
		 *
		 * The base class for Koala exceptions.
		 * \ingroup DMexception */
		class ExcBase
		{
			char buf[KOALA_EXCEPTION_BUF_SIZE];
			int _line;

		public:
			// Parametry konstruktora określają, co mają zwracać metody line, descr i file.
			ExcBase( const char *adesc = "", const char *afile = "", int aline = -1);

			// Wiersz w kodzie, gdzie wystapił błąd.
			inline int line() const
				{ return _line; }
			// Opis wyjątku.
			inline const char *descr() const
				{ return buf; }
			// Plik źrodłowy, w którym wystąpił błąd.
			inline const char *file() const
				{ return buf + std::strlen( buf ) + 1; }
			// Nazwa typu wyjątku.
			inline const char *type() const
				{ return "ExcBase"; }
		};

		/* ExcWrongArg
		 * Wyjątek typu "niepoprawny argument".
		 */
		/** \brief Wrong argument exception.
		 *
		 * \ingroup DMexception */
		class ExcWrongArg: virtual public ExcBase
		{
		public:
			inline ExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
					{ }
			inline const char *type() const
				{ return "ExcWrongArg"; }
		};

		/* ExcNullVert
		 * Wyjątek typu "podano zerowy wierzchołek".
		 */
		/** \brief NULL vertex pointer exception.
		 *
		 * \ingroup DMexception */
		class ExcNullVert: public ExcWrongArg
		{
		public:
			inline ExcNullVert( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ExcNullVert"; }
		};

		/* ExcNullEdge
		 * Wyjątek typu "podano zerową krawędź".
		 */
		/** \brief NULL edge pointer exception.
		 *
		 * \ingroup DMexception */
		class ExcNullEdge: public ExcWrongArg
		{
		public:
			inline ExcNullEdge( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ExcNullEdge"; }
		};

		/* ExcWrongConn
		 * Wyjątek typu "błędne połączenie w grafie między wierzchołkami lub krawędziami".
		 */
		/** \brief Wrong vertex - edge connection exception.
		 *
		 * \ingroup DMexception */
		class ExcWrongConn: public ExcWrongArg
		{
		public:
			inline ExcWrongConn( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ExcWrongConn"; }
		};

		/* ExcWrongMask
		 * Wyjątek typu "zła maska typu krawędzi".
		 */
		/** \brief Wrong edge type exception.
		 *
		 * \ingroup DMexception */
		class ExcWrongMask: public ExcWrongArg
		{
		public:
			inline ExcWrongMask( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ExcWrongMask"; }
		};

		/* ContExc
		 * Klasa bazowa dla błędów struktury danych.
		 */
		/** \brief Base class for exceptions in containers.
		 *
		 * \ingroup DMexception */
		class ContExc: virtual public ExcBase
		{
		public:
			inline ContExc( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ContExc"; }
		};

		/* ContExcWrongArg
		 * Wyjątek typu "zły argument metody kontenera".
		 */
		/** \brief  Container method incorrect argument exception.
		 *
		 * \ingroup DMexception */
		class ContExcWrongArg: public ContExc, public ExcWrongArg
		{
		public:
			inline ContExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ContExcWrongArg"; }
		};

		/* ContExcFull
		 * Wyjątek typu "przepełnienie kontenera".
		 */
		/** \brief  Container overflow exception.
		 *
		 * \ingroup DMexception */
		class ContExcFull: public ContExc
		{
		public:
			inline ContExcFull( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ContExcFull"; }
		};

		/* ContExcOutpass
		 * Wyjątek typu "przekroczenie zakresu kontenera".
		 */
		/** \brief  Container rage outpass exception.
		 *
		 * \ingroup DMexception */
		class ContExcOutpass: public ContExcWrongArg
		{
		public:
			inline ContExcOutpass( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "ContExcOutpass"; }
		};

		/* GraphExc
		 * Klasa bazowa dla blędów struktury grafu.
		 */
		/** \brief Base class for exceptions in graphs.
		 *
		 * \ingroup DMexception */
		class GraphExc: virtual public ExcBase
		{
		public:
			inline GraphExc( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "GraphExc"; }
		};

		/* GraphExcWrongArg
		 * Wyjątek typu "zły argument metody struktury grafu".
		 */
		/** \brief Wrong argument of graph structure method exception.
		 *
		 * \ingroup DMexception */
		class GraphExcWrongArg: public GraphExc, public ExcWrongArg
		{
		public:
			inline GraphExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "GraphExcWrongArg"; }
		};

		/* GraphExcWrongConn
		 * Wyjątek typu "zły argument metody struktury grafu, błędne połączenie".
		 */
		/** \brief Wrong argument of graph structure method (incorrect connection)  exception.
		 *
		 * \ingroup DMexception */
		class GraphExcWrongConn: public GraphExc, public ExcWrongConn
		{
		public:
			inline GraphExcWrongConn( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "GraphExcWrongConn"; }
		};

		/* GraphExcNullVert
		 * Wyjątek typu "zły argument metody struktury grafu, podano zerowy wierzchołek".
		 */
		/** \brief Wrong argument of graph structure method (NULL pointer of vertex)  exception.
		 *
		 * \ingroup DMexception */
		class GraphExcNullVert: public GraphExc, public ExcNullVert
		{
		public:
			inline GraphExcNullVert( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "GraphExcNullVert"; }
		};

		/* GraphExcNullEdge
		 * Wyjątek typu "zły argument metody struktury grafu, podano zerową krawędź".
		 */
		/** \brief Wrong argument of graph structure method (NULL pointer of edge)  exception.
		 *
		 * \ingroup DMexception */
		class GraphExcNullEdge: public GraphExc, public ExcNullEdge
		{
		public:
			inline GraphExcNullEdge( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "GraphExcNullEdge"; }
		};

		/* GraphExcWrongMask
		 * Wyjątek typu "zły argument metody struktury grafu, podano złą maskę typu".
		 */
		/** \brief Wrong argument of graph structure method (incorrect edge type)  exception.
		 *
		 * \ingroup DMexception */
		class GraphExcWrongMask: public GraphExc, public ExcWrongMask
		{
		public:
			inline GraphExcWrongMask( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "GraphExcWrongMask"; }
		};

		/* AlgExc
		 * Klasa bazowa dla błędów algorytmów biblioteki.
		 */
		/** \brief Base class for exceptions in algorithms.
		 *
		 * \ingroup DMexception */
		class AlgExc: virtual public ExcBase
		{
		public:
			inline AlgExc( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "AlgExc"; }
		};

		/* AlgExcWrongArg
		 * Wyjątek typu "zły argument algorytmu biblioteki".
		 */
		/** \brief Wrong argument of algorithm exception.
		 *
		 * \ingroup DMexception */
		class AlgExcWrongArg: public AlgExc, public ExcWrongArg
		{
		public:
			inline AlgExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "AlgExcWrongArg"; }
		};

		/* AlgExcNullVert
		 * Wyjątek typu "zły argument algorytmu biblioteki, podano zerowy wierzchołek".
		 */
		/** \brief Wrong argument of algorithm exception (NULL vertex pointer).
		 *
		 * \ingroup DMexception */
		class AlgExcNullVert: public AlgExc, public ExcNullVert
		{
		public:
			inline AlgExcNullVert( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "AlgExcNullVert"; }
		};

		/* AlgExcNullEdge
		 * Wyjątek typu "zły argument algorytmu biblioteki, podano zerową krawędź".
		 */
		/** \brief Wrong argument of algorithm exception (NULL edge pointer).
		 *
		 * \ingroup DMexception */
		class AlgExcNullEdge: public AlgExc, public ExcNullEdge
		{
		public:
			inline AlgExcNullEdge( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "AlgExcNullEdge"; }
		};

		/* AlgExcWrongMask
		 * Wyjątek typu "zły argument algorytmu biblioteki, podano zlą maskę typu".
		 */
		/** \brief Wrong argument of algorithm exception (incorrect edge type).
		 *
		 * \ingroup DMexception */
		class AlgExcWrongMask: public AlgExc, public ExcWrongMask
		{
		public:
			inline AlgExcWrongMask( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "AlgExcWrongMask"; }
		};

		/* AlgExcWrongConn
		 * Wyjątek typu "zły argument metody struktury grafu, błędne połączenie".
		 */
		/** \brief Wrong argument of algorithm exception (incorrect connection).
		 *
		 * \ingroup DMexception */
		class AlgExcWrongConn: public AlgExc, public ExcWrongConn
		{
		public:
			inline AlgExcWrongConn( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
				{ }
			inline const char *type() const
				{ return "GraphExcWrongConn"; }
		};
	}
}

#include "exception.hpp"
#endif