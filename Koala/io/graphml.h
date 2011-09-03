#ifndef KOALA_GRAPHML_H
#define KOALA_GRAPHML_H

/* ------------------------------------------------------------------------- *
 * graphml.h
 *
 * ------------------------------------------------------------------------- */

#include "../graph/graph.h"
#include "../tinyxml/tinyxml.h"
#include "../base/kstd.h"
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>

namespace Koala {
namespace IO {

class GraphML;

/* ------------------------------------------------------------------------- *
 * GraphMLKeyVal
 *
 * ------------------------------------------------------------------------- */

struct GraphMLKeyTypes {
    enum Type { None, NotDefined, Bool, Int, Long, Float, Double, String };
    enum ForKey { All, Graph, Node, Edge };
};


namespace Privates {

struct GraphMLKeyVal : public GraphMLKeyTypes {
    std::string id, name;
    GraphMLKeyTypes::Type type;
    GraphMLKeyTypes::ForKey forKey;
    bool is_def;
    union {
        int intVal;
        double dblVal;
        int64_t longVal;
    } pDef, pUsr;
    std::string sDef, sUsr;
    int count;
    GraphMLKeyVal();
    std::string print( bool );
    bool setDef( const char * );
    bool setUsr( const char * );
} ;

typedef std::vector< GraphMLKeyVal > GraphMLKeys;

/* ------------------------------------------------------------------------- *
 * KeysHolderBase
 *
 * ------------------------------------------------------------------------- */

class KeysHolderBase {
    friend class Koala::IO::GraphML;
    private:

        int count;

        void next();
        void clearCount();
        void setValUsr();
        void clear();

    protected:
        GraphMLKeys *defs;

        void touch( int );
        bool modified( int );
        std::string print( int, bool );

        int createKey( const char *, const char * );
        int createKey( const char * );
        int newVal( GraphMLKeyVal & );

        int getId( const char * );
        int getIdFromKeyId( const char * );
        std::string getKeyId( int );
        std::string getKeyName( int );

        GraphMLKeyTypes::Type getKeyType( int );
        GraphMLKeyTypes::ForKey getKeyFor( int );

        std::string getString( int );
        int getInt( int );
        int64_t getLong( int );
        double getDouble( int );

        void setKeyType( int, GraphMLKeyVal::Type );
        void setKeyFor( int, GraphMLKeyVal::ForKey );

        void setVal( int, const char * );
        void setVal( int, std::string & );
        void setVal( int, int );
        void setVal( int, int64_t );
        void setVal( int, double );

        void setValUsr( int, const char * );
        void setValDef( int, const char * );

    public:
        KeysHolderBase();
        KeysHolderBase( const KeysHolderBase & );
        ~KeysHolderBase();

        int getKeysNo();
        int getCount();
} ;

}


/* ------------------------------------------------------------------------- *
 * KeysHolderRead
 *
 * ------------------------------------------------------------------------- */

class KeysHolderRead: public Privates::KeysHolderBase
{
    private:
        friend class GraphML;
        char type;

    public:
        KeysHolderRead( const Privates::KeysHolderBase & );
        ~KeysHolderRead();

        int getId( const char * );
        bool valid( int );

        std::string getKeyId( int );
        std::string getKeyName( int );
        GraphMLKeyTypes::Type getKeyType( int );
        GraphMLKeyTypes::ForKey getKeyFor( int );

        bool getBool( int );
        int getInt( int );
        int64_t getLong( int );
        double getDouble( int );
        std::string getString( int );
} ;

/* ------------------------------------------------------------------------- *
 * KeysHolderWrite
 *
 * ------------------------------------------------------------------------- */

class KeysHolderWrite : public Privates::KeysHolderBase {
    private:
        friend class GraphML;
        char type;

    public:
        KeysHolderWrite( const Privates::KeysHolderBase & );
        ~KeysHolderWrite();

        int getIdCr( const char * );
        int getIdNoCr( const char * );
        bool valid( int );

        std::string getKeyId( int );
        std::string getKeyName( int );
        GraphMLKeyTypes::Type getKeyType( int );
        GraphMLKeyTypes::ForKey getKeyFor( int );

        bool setNotDef( int, const char * );
        bool setBool( int, bool );
        bool setInt( int, int );
        bool setLong( int, int64_t );
        bool setDouble( int, double );
        bool setString( int, const char * );
} ;

/* ------------------------------------------------------------------------- *
 * KeysHolderGraph
 *
 * ------------------------------------------------------------------------- */

class KeysHolderGraph: public Privates::KeysHolderBase
{
    private:
        friend class GraphML;

    public:
        KeysHolderGraph();
        ~KeysHolderGraph();

        int getIdCr( const char * );
        int getIdNoCr( const char * );
        bool valid( int );

        std::string getKeyId( int );
        std::string getKeyName( int );
        GraphMLKeyTypes::Type getKeyType( int );
        GraphMLKeyTypes::ForKey getKeyFor( int );

        bool setBool( int, bool );
        bool setInt( int, int );
        bool setLong( int, int64_t );
        bool setDouble( int, double );
        bool setString( int, const char * );

        bool getBool( int );
        int getInt( int );
        int64_t getLong( int );
        double getDouble( int );
        std::string getString( int );
} ;

/* ------------------------------------------------------------------------- *
 * GraphML
 *
 * ------------------------------------------------------------------------- */

class GraphML
{
    public:
        GraphML();
        ~GraphML();

        bool readFile( const char * );
        bool writeFile( const char * );
        void clear();
        int graphNo();

        std::string getGraphName( int );
        int isGraphName( const char * );

        template< class Graph > bool readGraph( Graph &, const char * );
        template< class Graph > bool readGraph( Graph &, int );
        template< class Graph, class InfoVertex, class InfoEdge >
            bool readGraph( Graph &, const char *, InfoVertex, InfoEdge );
        template< class Graph, class InfoVertex, class InfoEdge >
            bool readGraph( Graph &, int, InfoVertex, InfoEdge );

        template< class Graph > bool writeGraph( const Graph &, const char * );
        template< class Graph, class InfoVertex, class InfoEdge >
            bool writeGraph( const Graph &, const char *, InfoVertex, InfoEdge );

        bool deleteGraph( const char * );
        bool readGraphParam( const char *, KeysHolderGraph & );
        bool readGraphParam( int, KeysHolderGraph & );
        bool writeGraphParam( const char *, KeysHolderGraph & );
        bool writeGraphParam( int, KeysHolderGraph & );

    private:
        Privates::KeysHolderBase keysHolder;
        TiXmlDocument *doc;

        template< class Graph > bool readGraph( Graph &, TiXmlElement * );
        template< class Graph, class InfoVertex, class InfoEdge >
            bool readGraph( Graph &, TiXmlElement *, InfoVertex, InfoEdge );
        bool readGraphParam( TiXmlElement *, KeysHolderGraph & );
        bool writeGraphParam( TiXmlElement *, KeysHolderGraph & );
        bool createInitial();
        bool readKeys();
        void writeKeys();
} ;

#include "graphml.hpp"
}
}

#endif
