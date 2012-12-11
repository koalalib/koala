#ifndef KOALA_LOCALTABLE_H
#define KOALA_LOCALTABLE_H

/* this file defines LOCALARRAY macro
 *
 * it is intended to declare local tables
 * instead of:
 *    TYPE tableName[size]
 * write:
 *    TYPE LOCALARRAY(tableName, size)
 * tableName will be a TYPE* pointer to memory block for size objects of
 * type TYPE
 *
 * advantages:
 *   - portable method of declaring variable-sized arrays on all systems
 *     (e.g. Visual Studio will not accept int table[3 * n] but will
 *      accept int LOCALARRAY(table, 3 * n) )
 *   - as fast as stack-based local arrays (small creation & destruction
 *     overhead)
 *   - safer than stack-based local arrays (int table[n] will overflow the
 *     stack for large n, LOCALARRAY will use heap-allocated memory for
 *     large arrays; see KOALA_STACK_THRESHOLD)
 *   - will work for anonymous structures (UNDER GCC WILL NOT CALL IT'S CONSTRUCTOR):
 *     eg. struct {int a, b;} LOCALARRAY(table, 10);
 *
 * KOALA_STACK_THRESHOLD
 *   the maximum size of a single table allocated on stack
 *   this value should not exceed the stack size (usually 1MB on Windows
 *   system, about 10MB on Linux system)
 *
 * KOALA_DONT_USE_ALLOCA
 *   define this symbol to prevent Koala from using alloca function
 */

//#define KOALA_DONT_USE_ALLOCA

#ifndef KOALA_STACK_THRESHOLD
#define KOALA_STACK_THRESHOLD       8192
#endif

#include <new>
#include <malloc.h>

#ifdef __INTEL_COMPILER
#include <alloca.h>
#endif

namespace Koala
{
	/*
	* LocalTableMgr
	* an object handling initialization and deletion of an allocated memory
	*/
	class LocalTableMgr
	{
	public:
		LocalTableMgr() { }
		~LocalTableMgr() { m_killer( m_ptr,m_size ); }

		// if local == true, ptr is a alloca-ted memory for size elements of type T
		// if local == false, ptr is NULL, size is the number of elements to allocate
		// returns a pointer to allocated and initialized memory
		template< class T > T *Bind( T *ptr, size_t size, bool local );

		// fallback Bind for types not matched by the template
		// (e.g. anonymous structs in GCC)
		void *Bind( void *ptr, size_t size, bool local );

#ifdef __BORLANDC__
#pragma argsused
#endif

		template< class T > static void StackKiller( void *ptr, size_t size );
		template< class T > static void HeapKiller( void *ptr, size_t ) { delete[] (T *)ptr; }
		size_t GetSize() { return m_size; };

	private:
		// pointer to allocated array
		void *m_ptr;
		// number of items allocated
		size_t m_size;

		void (*m_killer)( void *, size_t );
	};

	template<> void LocalTableMgr::StackKiller< void >( void *, size_t ) { }
	template<> void LocalTableMgr::HeapKiller< void >( void *ptr, size_t ) { free( ptr ); }

	template< class T > inline T *k__cast( T *, void *p2 ) { return (T *)p2; }
	inline void *k__cast( void *, void *p2 ) { return (void *)p2; }

// just to prevent a warning
#ifdef __BORLANDC__
#define LA_SETTONULL
#else
#define LA_SETTONULL    = NULL
#endif

// Intel Compiler, GCC-likes, Visual Studio and Borland have alloca
#if !defined(KOALA_DONT_USE_ALLOCA) && \
	(defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__) || \
	 defined(__INTEL_COMPILER) || defined(__GCC__))

// name = NULL a dopiero potem name = cast ... 縠by unikn规 warningu
// od VS o wykorzystaniu niezainicjowanej zmiennej
#define LOCALARRAY(name, size)                          \
	*name LA_SETTONULL;                         \
	*(void **)&name = ((size) * sizeof(*name) < KOALA_STACK_THRESHOLD)  \
		? Koala::k__cast(name, alloca((size) * sizeof(*name)))      \
		: NULL;                             \
	Koala::LocalTableMgr name##_KILLER;                 \
	*(void **)&name = name##_KILLER.Bind(name, (size) * sizeof(*name), ((size) * sizeof(*name) < KOALA_STACK_THRESHOLD));

// assume others don't know about alloca
#else

#define LOCALARRAY(name, size)              \
	*name LA_SETTONULL;             \
	Koala::LocalTableMgr name##_KILLER;     \
	*(void **)&name = name##_KILLER.Bind(name, (size) * sizeof(*name), false);
#endif

#include "localarray.hpp"
}

#endif
