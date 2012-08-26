#ifndef KOALA_G6_H
#define KOALA_G6_H

/* g6.h
 *
 */

#include <map>
#include <string>
#include <vector>

#include "../graph/graph.h"

#define LIMIT_LO (63)
#define LIMIT_HI (126)

namespace Koala
{
    namespace IO
    {

    /* KoalaG6Exception
     *
     */
    class KoalaG6Exception
    {
      public:
        int error;
        
        KoalaG6Exception( int e ): error( e ) { }
    };

    template< class Graph > void readG6( Graph &, const char * );
    template< class Graph > void readG6( Graph &, std::string );

    template< class Graph > void writeG6( const Graph &, std::string & );
    template< class Graph > int writeG6( const Graph &, char *, int );

#include "g6.hpp"
    }
}

#endif
