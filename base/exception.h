#ifndef KOALA_EXCEPTION_H
#define KOALA_EXCEPTION_H

/** \file exception.h
 *
 */

#include <cassert>
#include <cstdlib>
#include <cstring>

// dlugosc bufora wewnatrz wyjatku przeznaczonego na jego tekstowy opis
/**\def KOALA_EXCEPTION_BUF_SIZE 
 * \brief Size of exception text buffer. 
 *  \ingroup DMexception */
#ifndef KOALA_EXCEPTION_BUF_SIZE
	#define KOALA_EXCEPTION_BUF_SIZE 150
#endif

// Stała KOALA_DONT_CHECK_ERRORS wyłącza sprawdzanie błędów i rzucanie wyjątków przez Koalę.
// Wylaczenie standardowego assert przez NDEBUG automatycznie wylacza koalowa kontrole
/** \def KOALA_DONT_CHECK_ERRORS 
 *  \brief Macro switching of exception testing.
 *  \ingroup DMexception */
#if defined(NDEBUG)
	#define KOALA_DONT_CHECK_ERRORS
#endif

// Makro koalaAssert() - pierwszy argument sprawdzany warunek, drugi typ rzucanego wyjątku. Automatycznie rzuca wyjatek z podanym w konstr. opisem zwracajacym jego typ i polozenie
//To wlasnie to makro jest ew. wylaczane stala czasu kompilacji KOALA_DONT_CHECK_ERRORS
/** \def koalaAssert( descr,type )
 *  \brief Koala macro for throwing exceptions.
 *  
 *  The macro takes token \a descr as parameter and type of Error \a type. Macro also uses information about from where (file name and line number) the exception is thrown.
 *  \ingroup DMexception */
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
	    //w opisie warto zwrocic uwage na 2-wymiarowa hierarchie klas wyjatkow Koali: sam typ wyjatku pokazuje, czy polecialo z metody kontenera, metody grafu, czy algorytmu
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
			/** \brief Constructor
			 *
			 * \param adesc Error description
			 * \param afile file name where the exception is thrown.
			 * \param aline the line number of the exception occurrence.*/
			inline ExcBase( const char *adesc = "", const char *afile = "", int aline = -1);

			/**\brief Get exception occurrence line number.*/
			inline int line() const
				{ return _line; }
			/**\brief Get exception description.*/
			inline const char *descr() const
				{ return buf; }
			// Plik źrodłowy, w którym wystąpił błąd.
			/**\brief Get source file name where the exception is thrown.*/
			inline const char *file() const
				{ return buf + std::strlen( buf ) + 1; }
			// Nazwa typu wyjątku.
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ExcWrongArg( const char *adesc = "", const char *afile = "", int aline = -1):
				ExcBase( adesc,afile,aline )
					{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ExcNullVert(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ExcNullEdge(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ExcWrongConn(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ExcWrongMask(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ContExc(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ContExcWrongArg(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline ContExcFull(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
			inline const char *type() const
				{ return "ContExcFull"; }
		};


		//NEW: blad destruktora puli obiektow SimplArrPool (simple.h), gdy nie wszystkie zaalokowane
		//obiekty zostaly zdealokowane
		class ContExcPoolNotEmpty: public ContExc
		{
		public:
			/**\copydoc ExcBase::ExcBase*/
			inline ContExcPoolNotEmpty(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
			inline const char *type() const
				{ return "ContExcPoolNotEmpty"; }
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
			/**\copydoc ExcBase::ExcBase*/
			inline ContExcOutpass(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline GraphExc(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline GraphExcWrongArg(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline GraphExcWrongConn(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline GraphExcNullVert(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline GraphExcNullEdge(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline GraphExcWrongMask(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline AlgExc(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline AlgExcWrongArg(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline AlgExcNullVert(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline AlgExcNullEdge(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline AlgExcWrongMask(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
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
			/**\copydoc ExcBase::ExcBase*/
			inline AlgExcWrongConn(const char *adesc = "", const char *afile = "", int aline = -1) :
				ExcBase( adesc,afile,aline )
				{ }
			/**\brief Get exception type.*/
			inline const char *type() const
				{ return "GraphExcWrongConn"; }
		};
	}
}

#include "exception.hpp"
#endif
