#ifndef KOALA_G6_H
#define KOALA_G6_H

/* ------------------------------------------------------------------------- *
 * g6.h
 *
 * ------------------------------------------------------------------------- */

#include "../graph/graph.h"
#include <string>
#include <vector>
#include <map>

#define LIMIT_LO (63)
#define LIMIT_HI (126)

namespace Koala {
namespace IO {

template< class Graph > bool readG6( Graph &, const char * );
template< class Graph > bool readG6( Graph &, std::string );

template< class Graph > void writeG6( const Graph &, std::string & );
template< class Graph > int writeG6( const Graph &, char *, int );

#include "g6.hpp"

}
}

#endif
