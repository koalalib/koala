template< class Graph > void readG6( Graph &graph, const char *str_graph )
{
    int mask = 0x20;
    char *ch = (char*)str_graph;

    int vert_no = 0;
    if (*ch < LIMIT_HI)
    {
        vert_no = *ch - LIMIT_LO;
        ++ch;
    }
    else
    {
        vert_no = *ch - LIMIT_LO;
        ++ch;
        vert_no = (vert_no << 6) | (*ch - LIMIT_LO);
        ++ch;
        vert_no = (vert_no << 6) | (*ch - LIMIT_LO);
        ++ch;
    }
    std::vector< typename Graph::Vertex * > vert_vect( vert_no );
    for( int i = 0; i < vert_no; i++ ) vert_vect[i] = graph.addVert();
    for( int i = 1; i < vert_no; i++ )
    {
        int j = 0;
        while (j < i)
        {
            if (!mask)
            {
                ++ch;
                // TODO: zamienic na zwracanie false przez funkcje
                if (!(*ch)) throw KoalaG6Exception( 2 );
                if (*ch < LIMIT_LO || *ch > LIMIT_HI) throw KoalaG6Exception( 1 );
                *ch -= LIMIT_LO;
                mask = 0x20;
            }

            if (*ch & mask) graph.addEdge( vert_vect[i],vert_vect[j] );
            mask >>= 1;
            j++;
        }
    }
}

template< class Graph > void readG6( Graph &graph, std::string str_graph )
{
    readG6( graph,str_graph.c_str() );
}

template< class Graph > void writeG6( const Graph &graph, std::string &str_graph )
{
    str_graph = "";

    int order = graph.getVertNo();
    int order2 = (order * (order - 1)) >> 1;
    if (order < LIMIT_HI) str_graph.push_back( order + LIMIT_LO );
    else {
        str_graph.push_back( (order >> 12) + LIMIT_LO );
        str_graph.push_back( ((order >> 6) & 0x3f) + LIMIT_LO );
        str_graph.push_back( (order & 0x3f) + LIMIT_LO );
    }

    std::map< typename Graph::PVertex,int > ids;
    typename Graph::PVertex vert = graph.getVert();
    int id = 0;
    while (vert)
    {
        ids[vert] = id;
        vert = graph.getVertNext( vert );
        id++;
    }

    unsigned char *tab = (unsigned char *)calloc( (order2 >> 3) + 1,1 );
    typename Graph::PEdge edge = graph.getEdge( EdUndir );
    unsigned char masks[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    while (edge)
    {
        std::pair< typename Graph::PVertex,typename Graph::PVertex >
            verts = graph.getEdgeEnds( edge );
        int id1 = ids[verts.first];
        int id2 = ids[verts.second];
        int nr = (id1 > id2) ? ( (id1 - 1) * id1 + id2 ) : ( (id2 - 1) * id2 + id1 );
        nr >>= 1;
        tab[nr >> 3] |= masks[nr & 0x07];
        edge = graph.getEdgeNext( edge,EdUndir );
    }
    int tmp_order = 0;
    unsigned char *ch_lo = tab, *ch_hi;
    while (1)
    {
        str_graph.push_back( ((*ch_lo) >> 2) + LIMIT_LO );
        tmp_order += 6;
        if (tmp_order >= order2) break;
        ch_hi = ch_lo;
        ch_lo++;

        str_graph.push_back( (((*ch_hi & 0x03) << 4) | (*ch_lo >> 4)) + LIMIT_LO );
        tmp_order += 6;
        if (tmp_order >= order2) break;
        ch_hi = ch_lo;
        ch_lo++;

        str_graph.push_back( (((*ch_hi & 0x0f) << 2)|(*ch_lo >> 6)) + LIMIT_LO );
        tmp_order += 6;
        if (tmp_order >= order2) break;

        str_graph.push_back( (*ch_lo & 0x3f) + LIMIT_LO );
        tmp_order += 6;
        if (tmp_order >= order2) break;
        ch_lo++;
    }
    free( tab );
}

template< class Graph > int writeG6( const Graph &graph, char *str_graph, int len_str )
{
    if (len_str < 4) return 0;

    int mask = 0;

    int order = graph.getVertNo();
    int order2 = (order * (order - 1)) >> 1;
    if (order < LIMIT_HI)
    {
        *str_graph = (char)(order + LIMIT_LO);
        str_graph++;
    }
    else
    {
        *str_graph = (char)((order >> 12) + LIMIT_LO);
        str_graph++;
        *str_graph = (char)(((order >> 6) & 0x3f) + LIMIT_LO);
        str_graph++;
        *str_graph = (char)((order & 0x3f) + LIMIT_LO);
        str_graph++;
    }

    std::map< typename Graph::PVertex,int > ids;
    typename Graph::PVertex vert = graph.getVert();
    int id = 0;
    while (vert)
    {
        ids[vert] = id;
        vert = graph.getVertNext( vert );
        id++;
    }

    unsigned char *tab = (unsigned char *)calloc( (order2 >> 3) + 1,1 );
    typename Graph::PEdge edge = graph.getEdge( EdUndir );
    unsigned char masks[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    while (edge)
    {
        std::pair< typename Graph::PVertex,typename Graph::PVertex >
            verts = graph.getEdgeEnds( edge );
        int id1 = ids[verts.first];
        int id2 = ids[verts.second];
        int nr = (id1 > id2) ? ( (id1 - 1) * id1 + id2 ) : ( (id2 - 1) * id2 + id1 );
        nr >>= 1;
        tab[nr >> 3] |= masks[nr & 0x07];
        edge = graph.getEdgeNext( edge,EdUndir );
    }
    int tmp_order = 0;
    unsigned char *ch_lo = tab, *ch_hi;
    int tmp_len = 0;
    while (tmp_len < len_str)
    {
        *str_graph = (char)(((*ch_lo) >> 2) + LIMIT_LO);
        str_graph++;
        tmp_order += 6;
        if (tmp_order >= order2 || tmp_len > len_str) break;
        ch_hi = ch_lo;
        ch_lo++;

        *str_graph = (char)((((*ch_hi & 0x03) << 4) | (*ch_lo >> 4)) + LIMIT_LO);
        str_graph++;
        tmp_order += 6;
        if (tmp_order >= order2 || tmp_len > len_str) break;
        ch_hi = ch_lo;
        ch_lo++;

        *str_graph = (char)((((*ch_hi & 0x0f) << 2) | (*ch_lo >> 6)) + LIMIT_LO);
        str_graph++;
        tmp_order += 6;
        if (tmp_order >= order2 || tmp_len > len_str) break;

        *str_graph = (char)((*ch_lo & 0x3f) + LIMIT_LO);
        str_graph++;
        tmp_order += 6;
        if (tmp_order >= order2 || tmp_len > len_str) break;
        ch_lo++;
    }
    free( tab );
    if (tmp_len < len_str) *str_graph = 0;
    return tmp_len;
}
