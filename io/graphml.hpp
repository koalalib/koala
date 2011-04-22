GraphMLKeyVal::GraphMLKeyVal()
{
    is_def = false;
    type = None;
    forKey = All;
    count = 0;
}

std::string GraphMLKeyVal::print( bool isUsr )
{
    char tmp_ch[40];
    if (isUsr)
    {
        switch (type)
        {
            case GraphMLKeyVal::Bool:
                return pUsr.intVal ? "1" : "0";
            case GraphMLKeyVal::Int:
                sprintf( tmp_ch,"%d",pUsr.intVal );
                return tmp_ch;
            case GraphMLKeyVal::Long:
                sprintf( tmp_ch,"%lld",pUsr.longVal );
                return tmp_ch;
            case GraphMLKeyVal::Float:
            case GraphMLKeyVal::Double:
                sprintf( tmp_ch,"%lf",pUsr.dblVal );
                return tmp_ch;
        }
        return sUsr;
    }
    else
    {
        switch (type)
        {
            case GraphMLKeyVal::Bool:
                return pDef.intVal ? "1" : "0";
            case GraphMLKeyVal::Int:
                sprintf( tmp_ch,"%d",pDef.intVal );
                return tmp_ch;
            case GraphMLKeyVal::Long:
                sprintf( tmp_ch,"%lld",pDef.longVal );
                return tmp_ch;
            case GraphMLKeyVal::Float:
            case GraphMLKeyVal::Double:
                sprintf( tmp_ch,"%lf",pDef.dblVal );
                return tmp_ch;
        }
        return sDef;
    }
}

bool GraphMLKeyVal::setDef( const char *val )
{
    if (!val) return false;
    switch (type)
    {
        case GraphMLKeyVal::Bool:
            if (val[0] == '1' || val[0] == 't') pDef.intVal = 1;
            else pDef.intVal = 0;
            break;
            
        case GraphMLKeyVal::Int:
            pDef.intVal = atoi( val );
            break;
            
        case GraphMLKeyVal::Long:
            pDef.longVal = atoll( val );
            break;
            
        case GraphMLKeyVal::Float:
        case GraphMLKeyVal::Double:
        {
            char *endP;
            pDef.dblVal = strtod( val,&endP );
        }
            break;
            
        case GraphMLKeyVal::NotDefined:
        case GraphMLKeyVal::String:
            sDef = val;
            break;
            
        default: return false;
    }
    is_def = true;
    return true;
}

bool GraphMLKeyVal::setUsr( const char *val )
{
    if (!val) return false;
    switch (type)
    {
        case GraphMLKeyVal::Bool:
            if (val[0] == '1' || val[0] == 't') pUsr.intVal = 1;
            else pUsr.intVal = 0;
            break;
            
        case GraphMLKeyVal::Int:
                pUsr.intVal = atoi( val );
                break;
                
        case GraphMLKeyVal::Long:
                pUsr.longVal = atoll( val );
                break;
                
        case GraphMLKeyVal::Float:
        case GraphMLKeyVal::Double:
        {
            char *endP;
            pUsr.dblVal = strtod( val,&endP );
        }
            break;
            
        case GraphMLKeyVal::NotDefined:
        case GraphMLKeyVal::String:
            sUsr = val;
            break;
            
        default: return false;
    }
    return true;
}

KeysHolderBase::KeysHolderBase()
{
    count = 0;
    defs = new GraphMLKeys;
}

KeysHolderBase::KeysHolderBase( const KeysHolderBase &khBase )
{
    this->count = khBase.count;
    this->defs = khBase.defs;
}

KeysHolderBase::~KeysHolderBase()
{
    if (this->defs)
    {
        delete this->defs;
        this->defs = NULL;
    }
}

void KeysHolderBase::next()
{
    ++this->count;
}

void KeysHolderBase::clearCount()
{
    this->count = 0;

    GraphMLKeys::iterator iter = this->defs->begin();
    while (iter != this->defs->end())
    {
        iter->count = 0;
        ++iter;
    }
}

void KeysHolderBase::clear()
{
    this->count = 0;
    this->defs->clear();
}

void KeysHolderBase::touch( int id )
{
    this->defs->at( id - 1 ).count = this->count;
}

bool KeysHolderBase::modified( int id )
{
    return this->defs->at( id - 1 ).count == this->count;
}

std::string KeysHolderBase::print( int id, bool isUsr )
{
    return this->defs->at( id - 1 ).print( isUsr );
}

int KeysHolderBase::getKeysNo()
{
    return this->defs->size();
}

int KeysHolderBase::getCount()
{
    return this->count;
}

int KeysHolderBase::createKey( const char *keyId, const char *keyName )
{
    GraphMLKeyVal val;
    val.id = keyId;
    if (keyName) val.name = keyName;
    defs->push_back( val );
    return defs->size();
}

int KeysHolderBase::createKey( const char *keyName )
{
    int len_defs = defs->size();
    char keyId[30];
    srand( time( NULL ) );
    bool loop_go;
    do
    {
        loop_go = false;
        sprintf( keyId,"k%04X",rand() );
        for( int i = 0; i < len_defs; i++ )
            if (defs->at( i ).id == keyId)
            {
               loop_go = true;
               break;
            }
    } while (loop_go);
    GraphMLKeyVal val;
    val.id = keyId;
    val.name = keyName;
    defs->push_back( val );
    return defs->size();
}

int KeysHolderBase::newVal( GraphMLKeyVal &val )
{
    this->defs->push_back( val );
    return this->defs->size();
}

int KeysHolderBase::getIdFromKeyId( const char *keyId )
{
    int len_defs = defs->size();
    for( int i = 0; i < len_defs; i++ )
        if (defs->at( i ).id == keyId) return i + 1;
    return 0;
}

int KeysHolderBase::getId( const char *keyName )
{
    int len_defs = defs->size();
    for( int i = 0; i < len_defs; i++ )
        if (defs->at( i ).name == keyName) return i + 1;
    return 0;
}

std::string KeysHolderBase::getKeyId( int id )
{
    return defs->at( id - 1 ).id;
}

std::string KeysHolderBase::getKeyName( int id )
{
    return defs->at( id - 1 ).name;
}

GraphMLKeyVal::Type KeysHolderBase::getKeyType( int id )
{
    return defs->at( id - 1 ).type;
}

GraphMLKeyVal::ForKey KeysHolderBase::getKeyFor( int id )
{
    return defs->at( id - 1 ).forKey;
}

std::string KeysHolderBase::getString( int id )
{
    if (defs->at( id - 1 ).count == this->count)
        return defs->at( id - 1 ).sUsr;
    return defs->at( id - 1 ).sDef;
}

int KeysHolderBase::getInt( int id )
{
    if (defs->at( id - 1 ).count == this->count)
        return defs->at( id - 1 ).pUsr.intVal;
    return defs->at( id - 1 ).pDef.intVal;
}

int64_t KeysHolderBase::getLong( int id )
{
    if (defs->at( id - 1 ).count == this->count)
        return defs->at( id - 1 ).pUsr.longVal;
    return defs->at( id - 1 ).pDef.longVal;
}

double KeysHolderBase::getDouble( int id )
{
    if (defs->at( id - 1 ).count == this->count)
        return defs->at( id - 1 ).pUsr.dblVal;
    return defs->at( id - 1 ).pDef.dblVal;
}

void KeysHolderBase::setKeyType( int id, GraphMLKeyVal::Type type )
{
    defs->at( id - 1 ).type = type;
}
void KeysHolderBase::setKeyFor( int id, GraphMLKeyVal::ForKey forKey )
{
    defs->at( id - 1 ).forKey = forKey;
}

void KeysHolderBase::setVal( int id, const char *val ) 
{
    if (defs->at( id - 1 ).type == GraphMLKeyVal::None)
        defs->at( id - 1 ).type = GraphMLKeyVal::NotDefined;
    defs->at( id - 1 ).sUsr = val;
}

void KeysHolderBase::setVal( int id, std::string &val )
{
    if (defs->at( id - 1 ).type == GraphMLKeyVal::None)
        defs->at( id - 1 ).type = GraphMLKeyVal::String;
    defs->at( id - 1 ).sUsr = val;
}

void KeysHolderBase::setVal( int id, int val )
{
    if (defs->at( id - 1 ).type == GraphMLKeyVal::None)
        defs->at( id - 1 ).type = GraphMLKeyVal::Int;
    defs->at( id - 1 ).pUsr.intVal = val;
}

void KeysHolderBase::setVal( int id, int64_t val )
{
    if (defs->at( id - 1 ).type == GraphMLKeyVal::None)
        defs->at( id - 1 ).type = GraphMLKeyVal::Long;
    defs->at( id - 1 ).pUsr.longVal = val;
}

void KeysHolderBase::setVal( int id, double val )
{
    if (defs->at( id - 1 ).type == GraphMLKeyVal::None)
        defs->at( id - 1 ).type = GraphMLKeyVal::Double;
    defs->at( id - 1 ).pUsr.dblVal = val;
}

void KeysHolderBase::setValUsr( int id, const char *val )
{
    if (defs->at( id - 1 ).type == GraphMLKeyVal::None)
        defs->at( id - 1 ).type = GraphMLKeyVal::NotDefined;
    defs->at( id - 1 ).setUsr( val );
}

void KeysHolderBase::setValDef( int id, const char *val )
{
    if (defs->at( id - 1 ).type == GraphMLKeyVal::None)
        defs->at( id - 1 ).type = GraphMLKeyVal::NotDefined;
    defs->at( id - 1 ).setDef( val );
}

KeysHolderRead::KeysHolderRead( const KeysHolderBase &khBase ):
    KeysHolderBase( khBase )
{
    this->type = 0;
}

KeysHolderRead::~KeysHolderRead()
{
    this->defs = NULL;
}

bool KeysHolderRead::valid( int id )
{
    if (id <= 0 || id > KeysHolderBase::getKeysNo()) return false;
    GraphMLKeyVal::ForKey forKey = KeysHolderBase::getKeyFor( id );
    switch (type)
    {
        case 0: return true;
        case 1: 
            if (forKey == GraphMLKeyVal::All || forKey == GraphMLKeyVal::Graph)
                return true;
            return false;
        case 2:
            if (forKey == GraphMLKeyVal::All || forKey == GraphMLKeyVal::Node)
                return true;
            return false;
        case 3:
            if (forKey == GraphMLKeyVal::All || forKey == GraphMLKeyVal::Edge)
                return true;
            return false;
    }
    return false;
}

int KeysHolderRead::getId( const char *keyName )
{
    int id = KeysHolderBase::getId( keyName );
    if (!valid( id )) return 0;
    return id;
}

std::string KeysHolderRead::getKeyId( int id )
{
    return KeysHolderBase::getKeyId( id );
}

std::string KeysHolderRead::getKeyName( int id )
{
    return KeysHolderBase::getKeyName( id );
}

GraphMLKeyVal::Type KeysHolderRead::getKeyType( int id ) 
{
    return KeysHolderBase::getKeyType( id );
}

GraphMLKeyVal::ForKey KeysHolderRead::getKeyFor( int id ) 
{
    return KeysHolderBase::getKeyFor( id );
}

bool KeysHolderRead::getBool( int id ) 
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getInt( id );
}

int KeysHolderRead::getInt( int id ) 
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getInt( id );
}

int64_t KeysHolderRead::getLong( int id ) 
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getLong( id );
}

double KeysHolderRead::getDouble( int id )
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getDouble( id );
}

std::string KeysHolderRead::getString( int id )
{
    if (!valid( id )) return "";
    return KeysHolderBase::getString( id );
}

KeysHolderWrite::KeysHolderWrite( const KeysHolderBase &khBase ):
    KeysHolderBase( khBase )
{
    type = 0;
}

KeysHolderWrite::~KeysHolderWrite()
{
    this->defs = NULL;
}

bool KeysHolderWrite::valid( int id )
{
    if (id <= 0 || id > this->defs->size()) return false;
    GraphMLKeyVal::ForKey forKey = KeysHolderBase::getKeyFor( id );
    switch (this->type)
    {
        case 0: return true;
        case 1:
            if (forKey == GraphMLKeyVal::All || forKey==GraphMLKeyVal::Graph)
                    return true;
            return false;
        case 2:
            if (forKey == GraphMLKeyVal::All || forKey == GraphMLKeyVal::Node)
                return true;
            return false;
        case 3:
            if (forKey == GraphMLKeyVal::All || forKey == GraphMLKeyVal::Edge)
                return true;
            return false;
    }
    return false;
}

int KeysHolderWrite::getIdCr( const char *keyName )
{
    int id = KeysHolderBase::getId( keyName );
    if (!id) id = KeysHolderBase::createKey( keyName );
    else if (!valid( id )) return 0;
    return id;
}

int KeysHolderWrite::getIdNoCr( const char *keyName )
{
    int id = KeysHolderBase::getId( keyName );
    if (!valid( id )) return 0;
    return id;
}

std::string KeysHolderWrite::getKeyId( int id )
{
    return KeysHolderBase::getKeyId( id );
}

std::string KeysHolderWrite::getKeyName( int id )
{
    return KeysHolderBase::getKeyName( id );
}

GraphMLKeyVal::Type KeysHolderWrite::getKeyType( int id ) 
{
    return KeysHolderBase::getKeyType( id );
}

GraphMLKeyVal::ForKey KeysHolderWrite::getKeyFor( int id )
{
    return KeysHolderBase::getKeyFor( id );
}

bool KeysHolderWrite::setNotDef( int id, const char *val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderWrite::setBool( int id, bool val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderWrite::setInt( int id, int val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderWrite::setLong( int id, int64_t val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderWrite::setDouble( int id, double val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderWrite::setString( int id, const char *val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

KeysHolderGraph::KeysHolderGraph(): KeysHolderBase() { }

KeysHolderGraph::~KeysHolderGraph() { }

int KeysHolderGraph::getIdCr( const char *keyName )
{
    int id = KeysHolderBase::getId( keyName );
    if (!id) id = KeysHolderBase::createKey( keyName );
    else if (!valid( id )) return 0;
    return id;
}

int KeysHolderGraph::getIdNoCr( const char *keyName )
{
    int id = KeysHolderBase::getId( keyName );
    if (!valid( id )) return 0;
    return id;
}

bool KeysHolderGraph::valid( int id )
{
    if (id <= 0 || id > this->defs->size()) return false;
    GraphMLKeyVal::ForKey forKey = KeysHolderBase::getKeyFor( id );
    if (forKey == GraphMLKeyVal::All || forKey == GraphMLKeyVal::Graph)
        return true;
    return false;
}

std::string KeysHolderGraph::getKeyId( int id )
{
    return KeysHolderBase::getKeyId( id );
}

std::string KeysHolderGraph::getKeyName( int id )
{
    return KeysHolderBase::getKeyName( id );
}

GraphMLKeyVal::Type KeysHolderGraph::getKeyType( int id )
{
    return KeysHolderBase::getKeyType( id );
}

GraphMLKeyVal::ForKey KeysHolderGraph::getKeyFor( int id )
{
    return KeysHolderBase::getKeyFor( id );
}

bool KeysHolderGraph::setBool( int id, bool val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderGraph::setInt( int id, int val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderGraph::setLong( int id, int64_t val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderGraph::setDouble( int id, double val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderGraph::setString( int id, const char *val )
{
    if (!valid( id )) return false;
    KeysHolderBase::setVal( id,val );
    KeysHolderBase::touch( id );
    return true;
}

bool KeysHolderGraph::getBool( int id )
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getInt( id );
}

int KeysHolderGraph::getInt( int id )
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getInt( id );
}

int64_t KeysHolderGraph::getLong( int id )
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getLong( id );
}

double KeysHolderGraph::getDouble( int id )
{
    if (!valid( id )) return 0;
    return KeysHolderBase::getDouble( id );
}

std::string KeysHolderGraph::getString( int id )
{
    if (!valid( id )) return "";
    return KeysHolderBase::getString( id );
}

GraphML::GraphML()
{ 
    doc = NULL;
}

GraphML::~GraphML()
{
    if (doc) delete doc;
}

int GraphML::graphNo()
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    int grNo = 0;
    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph)
    {
        ++grNo;
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    }
    return grNo;
}

std::string GraphML::getGraphName( int graphNr )
{
    if (!doc || doc->Error() || graphNr < 0) return "";

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return "";

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && graphNr)
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
        --graphNr;
    }
    if (!xmlGraph) return "";
    return xmlGraph->Attribute( "id" );
}

int GraphML::isGraphName( const char *graphName )
{
    if (!doc || doc->Error()) return -1;
    if (!graphName || !*graphName) return -1;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return -1;

    int retVal = 0;
    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName ))
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
        ++retVal;
    }
    if (!xmlGraph) return -1;
    return retVal;
}

bool GraphML::readFile( const char *fileName )
{
    if (doc) delete doc;
    doc = new TiXmlDocument( fileName );
    if (!doc) return false;

    doc->LoadFile();
    if (doc->Error())
    {
        delete doc;
        doc = NULL;
        return false;
    }
    readKeys();
    return true;
}

bool GraphML::writeFile( const char *fileName )
{
    if (!doc && !createInitial()) return false;
    writeKeys();
    return doc->SaveFile( fileName );
}

template< class Graph >
bool GraphML::readGraph( Graph &graph, TiXmlElement *xmlGraph )
{
    if (!xmlGraph) return false;

    bool isDirected = true;
    {
        const char *edgeDef = xmlGraph->Attribute( "edgedefault" );
        if (!strcmp( edgeDef,"undirected" )) isDirected = false;
    }

    std::map< std::string,typename Graph::Vertex * > verts;

    TiXmlElement *xmlVert = xmlGraph->FirstChildElement( "node" );
    while (xmlVert)
    {
        const char *id = xmlVert->Attribute( "id" );
        verts[id] = graph.addVert();
        xmlVert = xmlVert->NextSiblingElement( "node" );
    }

    TiXmlElement *xmlEdge = xmlGraph->FirstChildElement( "edge" );
    while (xmlEdge)
    {
        const char *source = xmlEdge->Attribute( "source" );
        const char *target = xmlEdge->Attribute( "target" );
        if (!strcmp( source,target )) graph.addLoop( verts[source] );
        else
        {
            bool edgeDirect = isDirected;
            const char *isEdgeDir = xmlEdge->Attribute( "directed" );
            if (isEdgeDir)
            {
                if (isEdgeDir[0] == 't' || isEdgeDir[0] == '1')
                    edgeDirect = true;
                else edgeDirect = false;
            }
            if (edgeDirect) graph.addArch( verts[source],verts[target] );
            else graph.addEdge( verts[source],verts[target] );
        }
        xmlEdge = xmlEdge->NextSiblingElement( "edge" );
    }
    return true;
}

template< class Graph, class InfoVertex, class InfoEdge >
bool GraphML::readGraph(
    Graph &graph, TiXmlElement *xmlGraph, InfoVertex infoVert, InfoEdge infoEdge )
{
    if (!xmlGraph) return false;

    KeysHolderRead khRead = keysHolder;
    bool isDirected = true;
    const char *edgeDef = xmlGraph->Attribute( "edgedefault" );
    if (!strcmp( edgeDef,"undirected" )) isDirected = false;

    std::map< std::string,typename Graph::Vertex * > verts;
    khRead.clearCount();

    khRead.type = 2;
    TiXmlElement *xmlVert = xmlGraph->FirstChildElement( "node" );
    while (xmlVert)
    {
        khRead.next();
        TiXmlElement *xmlKey = xmlVert->FirstChildElement( "data" );
        while (xmlKey)
        {
            const char *keyId = xmlKey->Attribute( "key" );
            const char *valUsr = xmlKey->GetText();
            int id = khRead.getIdFromKeyId( keyId );
            khRead.setValUsr( id,valUsr );
            khRead.touch( id );
            xmlKey = xmlKey->NextSiblingElement( "data" );
        }
        const char *id = xmlVert->Attribute( "id" );
        verts[id] = graph.addVert( infoVert( &khRead ) );
        xmlVert = xmlVert->NextSiblingElement( "node" );
    }

    khRead.type = 3;
    TiXmlElement *xmlEdge = xmlGraph->FirstChildElement( "edge" );
    while (xmlEdge)
    {
        khRead.next();
        TiXmlElement *xmlKey = xmlEdge->FirstChildElement( "data" );
        while (xmlKey)
        {
            const char *keyId = xmlKey->Attribute( "key" );
            const char *valUsr = xmlKey->GetText();
            int id = khRead.getIdFromKeyId( keyId );
            khRead.setValUsr( id,valUsr );
            khRead.touch( id );
            xmlKey = xmlKey->NextSiblingElement( "data" );
        }

        const char *source = xmlEdge->Attribute( "source" );
        const char *target = xmlEdge->Attribute( "target" );
        if (!strcmp( source,target )) graph.addLoop( verts[source],infoEdge( &khRead ) );
        else
        {
            bool edgeDirect = isDirected;
            const char *isEdgeDir = xmlEdge->Attribute( "directed" );
            if (isEdgeDir)
            {
                if (isEdgeDir[0] == 't' || isEdgeDir[0] == '1')
                    edgeDirect = true;
                else
                    edgeDirect = false;
            }
            if (edgeDirect)
                graph.addArch( verts[source],verts[target],infoEdge( &khRead ) );
            else graph.addEdge( verts[source],verts[target],infoEdge( &khRead ) );
        }
        xmlEdge = xmlEdge->NextSiblingElement( "edge" );
    }
    return true;
}

template< class Graph >
bool GraphML::readGraph( Graph &graph, const char *graphName )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName ))
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    }
    if (!xmlGraph) return false;
    return readGraph( graph,xmlGraph );
}

template< class Graph >
bool GraphML::readGraph( Graph &graph, int graphNr )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && graphNr)
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
        --graphNr;
    }
    if (!xmlGraph) return false;
    return readGraph( graph,xmlGraph );
}

template< class Graph, class InfoVertex, class InfoEdge >
bool GraphML::readGraph(
    Graph &graph, const char *graphName, InfoVertex infoVert, InfoEdge infoEdge )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName ))
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    }
    if (!xmlGraph) return false;
    return readGraph( graph,xmlGraph,infoVert,infoEdge );
}

template< class Graph, class InfoVertex, class InfoEdge >
bool GraphML::readGraph(
    Graph &graph, int graphNr, InfoVertex infoVert, InfoEdge infoEdge )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && graphNr)
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
        --graphNr;
    }
    if (!xmlGraph) return false;
    return readGraph( graph,xmlGraph,infoVert,infoEdge );
}

template< class Graph >
bool GraphML::writeGraph( const Graph &graph, const char *graphName )
{
    if (!graphName || !*graphName) return false;
    if (!doc && !createInitial()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName )) 
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    }
    if (xmlGraph) return false;
    xmlGraph = new TiXmlElement( "graph" );
    xmlGraph->SetAttribute( "id",graphName );
    xmlGraph->SetAttribute( "edgedefault","undirected" );

    xmlGraphs->LinkEndChild( xmlGraph );

    typename Graph::Vertex *vert = graph.getVert();
    while (vert)
    {
        char adress[30];
        sprintf( adress,"n%08X",(int)vert );
        TiXmlElement *xmlVert = new TiXmlElement( "node" );
        xmlVert->SetAttribute( "id",adress );
        xmlGraph->LinkEndChild( xmlVert );
        vert = graph.getVertNext( vert );
    }
    typename Graph::Edge *edge = graph.getEdge();
    while (edge)
    {
        char adress[30];
        sprintf( adress,"e%08X",(int)edge );
        TiXmlElement *xmlEdge = new TiXmlElement( "edge" );
        xmlEdge->SetAttribute( "id",adress );

        std::pair< typename Graph::Vertex *,typename Graph::Vertex * >
            verts = graph.getEdgeEnds( edge );
        sprintf( adress,"n%08X",(int)verts.first );
        xmlEdge->SetAttribute( "source",adress );
        sprintf( adress,"n%08X",(int)verts.second );
        xmlEdge->SetAttribute( "target",adress );
        if (graph.getType( edge ) == Directed)
            xmlEdge->SetAttribute( "directed","true" );
        xmlGraph->LinkEndChild( xmlEdge );

        edge = graph.getEdgeNext( edge );
    }
    return true;
}

template< class Graph, class InfoVertex, class InfoEdge >
bool GraphML::writeGraph(
    const Graph &graph, const char *graphName, InfoVertex infoVert, InfoEdge infoEdge )
{
    if (!graphName || !*graphName) return false;
    if (!doc && !createInitial()) return false;
    if (!readKeys()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName )) 
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    if (xmlGraph) return false;

    xmlGraph = new TiXmlElement( "graph" );
    xmlGraph->SetAttribute( "id",graphName );
    xmlGraph->SetAttribute( "edgedefault","undirected" );
    KeysHolderWrite khWrite = keysHolder;
    khWrite.clearCount();

    khWrite.type = 2;
    typename Graph::Vertex *vert = graph.getVert();
    while (vert)
    {
        khWrite.next();
        char adress[30];
        sprintf( adress,"n%08X",vert );
        TiXmlElement *xmlVert = new TiXmlElement( "node" );
        xmlGraph->LinkEndChild( xmlVert );
        xmlVert->SetAttribute( "id",adress );

        infoVert( vert,&khWrite );
        for( int i = 1; i <= khWrite.getKeysNo(); i++ )
            if (khWrite.modified( i ))
            {
                TiXmlElement *xmlKey= new TiXmlElement( "data" );
                xmlVert->LinkEndChild( xmlKey );
                xmlKey->SetAttribute( "key",khWrite.getKeyId( i ).c_str() );
                xmlKey->LinkEndChild(
                    new TiXmlText( khWrite.print( i,true ).c_str() ) );
            }
        vert = graph.getVertNext( vert );
    }

    khWrite.type = 3;
    typename Graph::Edge *edge = graph.getEdge();
    while (edge)
    {
        khWrite.next();
        char adress[30];
        sprintf( adress,"e%08X",edge );
        TiXmlElement *xmlEdge = new TiXmlElement( "edge" );
        xmlGraph->LinkEndChild( xmlEdge );
        xmlEdge->SetAttribute( "id",adress );

        infoEdge( edge,&khWrite );
        for( int i = 1; i <= khWrite.getKeysNo(); ++i )
            if (khWrite.modified( i )) 
            {
                TiXmlElement *xmlKey= new TiXmlElement( "data" );
                xmlEdge->LinkEndChild( xmlKey );
                xmlKey->SetAttribute( "key",khWrite.getKeyId( i ).c_str() );
                xmlKey->LinkEndChild(
                    new TiXmlText( khWrite.print( i,true ).c_str() ) );
            }

        std::pair< typename Graph::Vertex *,typename Graph::Vertex * >
            verts = graph.getEdgeEnds(edge);
        sprintf( adress,"n%08X",verts.first );
        xmlEdge->SetAttribute( "source",adress );
        sprintf( adress,"n%08X",verts.second );
        xmlEdge->SetAttribute( "target",adress );
        if (graph.getType( edge ) == Directed)
            xmlEdge->SetAttribute( "directed","true" );
        edge = graph.getEdgeNext( edge );
    }
    xmlGraphs->LinkEndChild( xmlGraph );
    return true;
}

bool GraphML::createInitial()
{
    if (doc) return true;
    doc = new TiXmlDocument();
    TiXmlDeclaration *xmlDecl = new TiXmlDeclaration( "1.0","UTF-8","" );
    doc->LinkEndChild( xmlDecl );

    TiXmlElement *xmlGraphs = new TiXmlElement( "graphml" );
    xmlGraphs->SetAttribute( "xmlns","http://graphml.graphdrawing.org/xmlns" );
    xmlGraphs->SetAttribute( "xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance" );
    xmlGraphs->SetAttribute( "xsi:schemaLocation",
        "http://graphml.graphdrawing.org/xmlns "
        "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd" );
    doc->LinkEndChild( xmlGraphs );
    return true;
}

void GraphML::clear()
{
    if (doc) delete doc;
    doc = NULL;
    keysHolder.clear();
}

bool GraphML::deleteGraph( const char *graphName )
{
    if (!doc || doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName ))
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    if (!xmlGraph) return false;

    return xmlGraphs->RemoveChild( xmlGraph );
}

bool GraphML::readKeys()
{
    if (!doc) return false;
    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    keysHolder.clear();

    TiXmlElement *xmlKey = xmlGraphs->FirstChildElement( "key" );
    while (xmlKey)
    {
        const char *keyId = xmlKey->Attribute( "id" );
        const char *keyFor = xmlKey->Attribute( "for" );
        const char *keyType = xmlKey->Attribute( "attr.type" );
        const char *keyName = xmlKey->Attribute( "attr.name" );

        GraphMLKeyVal::ForKey forKey;
        if (!keyFor) continue;
        else if (!strcmp( keyFor,"all" )) forKey = GraphMLKeyVal::All;
        else if (!strcmp( keyFor,"node" )) forKey = GraphMLKeyVal::Node;
        else if (!strcmp( keyFor,"edge" )) forKey = GraphMLKeyVal::Edge;
        else if (!strcmp( keyFor,"graph" )) forKey = GraphMLKeyVal::Graph;
        else continue;

        int id = keysHolder.createKey( keyId,keyName );
        keysHolder.setKeyFor( id,forKey );

        GraphMLKeyVal::Type typeKey = GraphMLKeyVal::None;
        if (!strcmp( keyType,"boolean" )) typeKey = GraphMLKeyVal::Bool;
        else if (!strcmp( keyType,"int" )) typeKey = GraphMLKeyVal::Int;
        else if (!strcmp( keyType,"long" )) typeKey = GraphMLKeyVal::Long;
        else if (!strcmp( keyType,"float" )) typeKey = GraphMLKeyVal::Float;
        else if (!strcmp( keyType,"double" )) typeKey = GraphMLKeyVal::Double;
        else if (!strcmp( keyType,"string" )) typeKey = GraphMLKeyVal::String;

        keysHolder.setKeyType( id,typeKey );

        TiXmlElement *xmlDefault = xmlKey->FirstChildElement( "default" );
        if (xmlDefault) keysHolder.setValDef( id,xmlDefault->GetText() );
        xmlKey = xmlKey->NextSiblingElement( "key" );
    }
    return true;
}

void GraphML::writeKeys()
{
    if (!doc) return;
    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return;

    keysHolder.clearCount();
    keysHolder.next();

    TiXmlElement *xmlKey = xmlGraphs->FirstChildElement( "key" );
    while (xmlKey)
    {
        const char *keyId = xmlKey->Attribute( "id" );
        int id = keysHolder.getIdFromKeyId( keyId );
        if (id) keysHolder.touch( id );

        xmlKey = xmlKey->NextSiblingElement( "key" );
    }

    TiXmlElement *tmpElem = new TiXmlElement( "key" );

    TiXmlNode *xmlFirst = xmlGraphs->FirstChild();
    GraphMLKeys::iterator iter = keysHolder.defs->begin();
    for( ; iter != keysHolder.defs->end(); ++iter )
    {
        if (iter->count == keysHolder.count) continue;

        if (xmlFirst) xmlFirst = xmlGraphs->InsertBeforeChild( xmlFirst,*tmpElem );
        else xmlFirst = xmlGraphs->InsertEndChild( *tmpElem );
        TiXmlElement *xmlNewKey = xmlFirst->ToElement();
        xmlNewKey->SetAttribute( "id",iter->id.c_str() );

        switch (iter->forKey)
        {
            case GraphMLKeyVal::Graph:
                xmlNewKey->SetAttribute( "for","graph" );
                break;
                
            case GraphMLKeyVal::Node:
                xmlNewKey->SetAttribute( "for","node" );
                break;
                    
            case GraphMLKeyVal::Edge:
                xmlNewKey->SetAttribute( "for","edge" );
                break;
                    
            case GraphMLKeyVal::All:
            default:
                xmlNewKey->SetAttribute( "for","all" );
        }

        switch (iter->type)
        {
            case GraphMLKeyVal::Bool:
                xmlNewKey->SetAttribute( "attr.type","boolean" );
                break;
                
            case GraphMLKeyVal::Int:
                xmlNewKey->SetAttribute( "attr.type","int" );
                break;
                
            case GraphMLKeyVal::Long:
                xmlNewKey->SetAttribute( "attr.type","long" );
                break;
                
            case GraphMLKeyVal::Float:
                xmlNewKey->SetAttribute( "attr.type","float" );
                break;
                
            case GraphMLKeyVal::Double:
                xmlNewKey->SetAttribute( "attr.type","double" );
                break;
                
            case GraphMLKeyVal::String:
                xmlNewKey->SetAttribute( "attr.type","string" );
                break;
        }
        if (!iter->name.empty())
            xmlNewKey->SetAttribute( "attr.name",iter->name.c_str() );
        if (iter->is_def)
        {
            TiXmlElement *xmlDef = new TiXmlElement( "default" );
            xmlNewKey->LinkEndChild( xmlDef );
            char tmp_ch[40];
            switch (iter->type)
            {
                case GraphMLKeyVal::Bool:
                    xmlDef->LinkEndChild(
                        new TiXmlText(iter->pDef.intVal ? "1" : "0") );
                    break;
                    
                case GraphMLKeyVal::Int:
                    sprintf( tmp_ch,"%d",iter->pDef.intVal );
                    xmlDef->LinkEndChild( new TiXmlText( tmp_ch ) );
                    break;
                    
                case GraphMLKeyVal::Long:
                    sprintf( tmp_ch,"%lld",iter->pDef.longVal );
                    xmlDef->LinkEndChild( new TiXmlText( tmp_ch ) );
                    break;
                    
                case GraphMLKeyVal::Float:
                case GraphMLKeyVal::Double:
                    sprintf( tmp_ch,"%lf",iter->pDef.dblVal );
                    xmlDef->LinkEndChild( new TiXmlText( tmp_ch ) );
                    
                default:
                    xmlDef->LinkEndChild( new TiXmlText( iter->sDef.c_str() ) );
            }
        }
    }
    delete tmpElem;
}

bool GraphML::readGraphParam( const char *graphName, KeysHolderGraph &kHolderG )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName )) 
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    if (!xmlGraph) return false;
    return readGraphParam( xmlGraph,kHolderG );
}

bool GraphML::readGraphParam( int graphNr, KeysHolderGraph &kHolderG )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && graphNr) 
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
        --graphNr;
    }
    if (!xmlGraph) return false;
    return readGraphParam( xmlGraph,kHolderG );
}

bool GraphML::readGraphParam( TiXmlElement *xmlGraph, KeysHolderGraph &kHolderG )
{
    kHolderG.clear();
    kHolderG.next();
    this->keysHolder.next();
    TiXmlElement *xmlGraphData = xmlGraph->FirstChildElement( "data" );
    while (xmlGraphData)
    {
        const char *keyId = xmlGraphData->Attribute( "key" );
        const char *valUsr = xmlGraphData->GetText();
        int id = this->keysHolder.getIdFromKeyId( keyId );
        this->keysHolder.setValUsr( id,valUsr );
        int newId = kHolderG.newVal( this->keysHolder.defs->at( id - 1 ) );
        kHolderG.touch( newId );

        xmlGraphData = xmlGraphData->NextSiblingElement( "data" );
    }
    return true;
}

bool GraphML::writeGraphParam( const char *graphName, KeysHolderGraph &kHolderG )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && strcmp( xmlGraph->Attribute( "id" ),graphName )) 
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
    if (!xmlGraph) return false;
    return writeGraphParam( xmlGraph,kHolderG );
}

bool GraphML::writeGraphParam( int graphNr, KeysHolderGraph &kHolderG )
{
    if (!doc) return false;
    if (doc->Error()) return false;

    TiXmlNode *xmlGraphs = doc->RootElement();
    if (!xmlGraphs) return false;

    TiXmlElement *xmlGraph = xmlGraphs->FirstChildElement( "graph" );
    while (xmlGraph && graphNr)
    {
        xmlGraph = xmlGraph->NextSiblingElement( "graph" );
        --graphNr;
    }
    if (!xmlGraph) return false;
    return writeGraphParam( xmlGraph,kHolderG );
}

bool GraphML::writeGraphParam( TiXmlElement *xmlGraph, KeysHolderGraph &kHolderG )
{
    TiXmlElement *xmlGraphData = xmlGraph->FirstChildElement( "data" );
    while (xmlGraphData)
    {
        xmlGraph->RemoveChild( xmlGraphData );
        xmlGraphData = xmlGraph->FirstChildElement( "data" );
    }

    for( int i = 1; i <= kHolderG.getKeysNo(); ++i ) 
    {
        if (!kHolderG.modified( i )) continue;
        int j = 1;
        for( ; j <= this->keysHolder.getKeysNo(); ++j ) 
            if (kHolderG.getKeyName( i ) == this->keysHolder.getKeyName( j ))
                    break;
        if (j > this->keysHolder.getKeysNo())
            this->keysHolder.newVal( kHolderG.defs->at( i - 1 ) );
        else
        {
            if (this->keysHolder.getKeyFor( j ) != GraphMLKeyVal::All
                && this->keysHolder.getKeyFor( j ) != GraphMLKeyVal::Graph)
                    continue;
        }
        TiXmlElement *xmlKey = new TiXmlElement( "data" );
        xmlGraph->LinkEndChild( xmlKey );
        xmlKey->SetAttribute( "key",kHolderG.getKeyId( i ).c_str() );
        xmlKey->LinkEndChild( new TiXmlText( kHolderG.print( i,true ).c_str() ) );
    }
    return true;
}
