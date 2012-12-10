#ifndef KOALA_GRAPHBASE_H
#define KOALA_GRAPHBASE_H

/* grconst.h
 *
 */

template< class GraphType > class ConstGraphMethods;
template< class GraphType > struct GraphInternalTypes;

template< class GraphType > struct GraphInternalTypes< ConstGraphMethods< GraphType > >
{
    typedef typename GraphInternalTypes< GraphType >::Vertex Vertex;
    typedef typename GraphInternalTypes< GraphType >::PVertex PVertex;
    typedef typename GraphInternalTypes< GraphType >::Edge Edge;
    typedef typename GraphInternalTypes< GraphType >::PEdge PEdge;
    typedef typename GraphInternalTypes< GraphType >::VertInfoType VertInfoType;
    typedef typename GraphInternalTypes< GraphType >::EdgeInfoType EdgeInfoType;
    typedef typename GraphInternalTypes< GraphType >::GraphSettings GraphSettings;
};


/* ConstGraphMethods
 * Klasa wprowadzajaca poprzez mechanizm CRTP metody operujace na strukturze typu const GraphType do tejze struktury
 * Wykorzystywana do wzbogacania interfejsu Graph, Subgraph oraz widokow na graf
 */
template< class GraphType > class ConstGraphMethods
{
  public:
    typedef typename GraphInternalTypes< ConstGraphMethods< GraphType > >::Vertex Vertex;
    typedef typename GraphInternalTypes< ConstGraphMethods< GraphType > >::PVertex PVertex;
    typedef typename GraphInternalTypes< ConstGraphMethods< GraphType > >::Edge Edge;
    typedef typename GraphInternalTypes< ConstGraphMethods< GraphType > >::PEdge PEdge;
    typedef typename GraphInternalTypes< ConstGraphMethods< GraphType > >::VertInfoType VertInfoType;
    typedef typename GraphInternalTypes< ConstGraphMethods< GraphType > >::EdgeInfoType EdgeInfoType;
    typedef typename GraphInternalTypes< ConstGraphMethods< GraphType > >::GraphSettings GraphSettings;

  protected:
    const GraphType &self;

    struct Parals3
    {
        PVertex v1,v2;
        EdgeDirection direct;
        int nr;
        PEdge edge;

        Parals3( PVertex av1, PVertex av2, EdgeDirection adirect, int anr, PEdge aedge):
            v1( av1 ), v2( av2 ), direct( adirect ), nr( anr ), edge( aedge )
            { }
        Parals3()
            { }
    };

    struct Parals3cmp
    {
        bool operator()( Parals3 a, Parals3 b )
        {
            return (a.v1 < b.v1) || (a.v1 == b.v1 && a.v2 < b.v2) ||
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct < b.direct ) ||
                (a.v1 == b.v1 && a.v2 == b.v2 && a.direct == b.direct && a.nr < b.nr);
        }
    };

  public:
    ConstGraphMethods(): self( (const GraphType &)*this )
        { }
    ConstGraphMethods( const ConstGraphMethods & ): self( (const GraphType &)*this )
        { }
    ConstGraphMethods &operator=( const ConstGraphMethods & )
        { return *this; }

//    Uwaga: metody, ktore musza byc dostarczane przez GraphType:
//         int getVertNo() const;
//         PVertex getVertNext( PVertex ) const;
//         PVertex getVertPrev( PVertex ) const;
//         int getEdgeNo( EdgeDirection  = EdAll ) const;
//         PEdge getEdgeNext( PEdge , EdgeDirection  = EdAll ) const;
//         PEdge getEdgePrev( PEdge , EdgeDirection  = EdAll ) const;
//         int getEdgeNo( PVertex , EdgeDirection  = EdAll) const;
//         PEdge getEdgeNext( PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         PEdge getEdgePrev( PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         int getEdgeNo( PVertex , PVertex , EdgeDirection = EdAll ) const;
//         PEdge getEdgeNext( PVertex , PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         PEdge getEdgePrev( PVertex , PVertex , PEdge , EdgeDirection  = EdAll ) const;
//         EdgeType getEdgeType( PEdge  ) const;
//         std::pair< PVertex,PVertex > getEdgeEnds( PEdge ) const;
//         PVertex getEdgeEnd1( PEdge ) const;
//         PVertex getEdgeEnd2( PEdge  ) const;
//         EdgeDirection getEdgeDir( PEdge , PVertex);

//    Uwaga: zasady ogolne obslugi zbiorow wierz/krawedzi struktury grafowej. Zestaw metod postaci:
//    int get...No(... arg ...) - dlugosc listy
//    template <class Iter> int get...(...) - zapis na podany iterator calej listy, zwraca jej dlugosc
//    Set<Element> get...Set(...) - wszystkie elementy listy zwracane w formie zbioru
//    Jesli te elementy struktury sa zorganizowane w liste, poruszamy sie po niej uzywajac dodatkowo
//    Element get...(...) - pierwszy element listy, NULL w przypadku listy pustej
//    Element get...Last(...) - ostatni element listy, NULL w przypadku listy pustej
//    Element get...Next(... arg ...) - kolejny po arg element listy, NULL jesli arg byl ostatni
//    Element get...Prev(... arg ...) - poprzedni po arg element listy, NULL jesli arg byl pierwszy
//        Natomiast zbior wejsciowy pobieramy z zakresu miedzy dwoma iteratorami lub przez Set



    // Lista wierzcholkow grafu
    // Liczba wierzcho³ków (rz¹d grafu).
    inline int getVertNo() const
        { return self.getVertNo(); }
    inline PVertex getVertNext( PVertex v ) const
        { return self.getVertNext( v ); }
    inline PVertex getVertPrev( PVertex v ) const
        { return self.getVertPrev( v ); }
    PVertex getVert() const
        { return self.getVertNext( (PVertex)0 ); }
    PVertex getVertLast() const
        { return self.getVertPrev( (PVertex)0 ); }
    template< class OutputIterator > int getVerts( OutputIterator ) const;
    Set< PVertex > getVertSet() const;

    // Liczba krawêdzi typu zawartego w podanej masce
    inline int getEdgeNo( EdgeType mask = EdAll ) const
        { return self.getEdgeNo( mask ); }
    inline PEdge getEdgeNext( PEdge e, EdgeType mask = EdAll ) const
        { return self.getEdgeNext( e,mask ); }
    inline PEdge getEdgePrev( PEdge e, EdgeType mask = EdAll ) const
        { return self.getEdgePrev( e,mask ); }
    template< class OutputIterator > int getEdges( OutputIterator, EdgeType = EdAll ) const;
    Set< PEdge > getEdgeSet( EdgeType = EdAll ) const;
    PEdge getEdge( EdgeType mask = EdAll ) const
        { return self.getEdgeNext( (PEdge)0,mask ); }
    PEdge getEdgeLast( EdgeType mask = EdAll ) const
        { return self.getEdgePrev( (PEdge)0,mask ); }

    // lista krawedzi sasiednich do v o orientacji wzgledem v zawartej w masce
    inline int getEdgeNo( PVertex v, EdgeDirection mask = EdAll) const
        { return self.getEdgeNo( v,mask ); }
    inline PEdge getEdgeNext( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
        { return self.getEdgeNext( v,e,mask ); }
    inline PEdge getEdgePrev( PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
        { return self.getEdgePrev( v,e,mask ); }
    PEdge getEdge( PVertex vert, EdgeDirection mask= EdAll ) const
        { return self.getEdgeNext( vert,(PEdge)0,mask ); }
    PEdge getEdgeLast( PVertex vert, EdgeDirection mask = EdAll ) const
        { return self.getEdgePrev( vert,(PEdge)0,mask );}
    template< class OutputIterator > int getEdges( OutputIterator, PVertex, EdgeDirection = EdAll ) const;
    Set< PEdge > getEdgeSet( PVertex, EdgeDirection = EdAll ) const;

    // lista krawedzi laczacych podane wierzcholki w sposob zawarty w masce
    inline int getEdgeNo( PVertex u, PVertex v, EdgeDirection mask = EdAll ) const
        { return self.getEdgeNo( u,v,mask ); }
    inline PEdge getEdgeNext( PVertex u, PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
        { return self.getEdgeNext( u,v,e,mask ); }
    inline PEdge getEdgePrev( PVertex u, PVertex v, PEdge e, EdgeDirection mask = EdAll ) const
        { return self.getEdgePrev( u,v,e,mask ); }
    PEdge getEdge( PVertex vert1, PVertex vert2, EdgeDirection mask = EdAll ) const
        { return self.getEdgeNext( vert1,vert2,(PEdge)0,mask ); }
    PEdge getEdgeLast( PVertex vert1, PVertex vert2, EdgeDirection mask = EdAll ) const
        { return self.getEdgePrev( vert1,vert2,(PEdge)0,mask ); }
    Set< PEdge > getEdgeSet( PVertex, PVertex, EdgeDirection = EdAll ) const;
    template< class OutputIterator > int getEdges( OutputIterator, PVertex, PVertex, EdgeDirection = EdAll ) const;

    // zbior wierzcholkow/krawedzi spelniajacych podany chooser
    template< class OutputIterator, class VChooser2 > int getVerts( OutputIterator out, VChooser2 ch ) const;
    template< class VChooser2 > Set< PVertex > getVertSet( VChooser2 ch ) const;

    template< class OutputIterator, class EChooser2 > int getEdges( OutputIterator out, EChooser2 ch ) const;
    template< class EChooser2 > Set< PEdge > getEdgeSet( EChooser2 ch ) const;

    // podobnie, ale podajemy pare chooserow (dla wierzcholka i krawedzi). Parametr bool=true - krawedz ma spelniac
    // nie tylko swoj chooser, ale takze oba konce chooser dla wierzcholkow
    template< class OutputV, class OutputE, class VChooser2, class EChooser2 >
        std::pair< int,int > getChosen( std::pair< OutputV,OutputE >,std::pair< VChooser2,EChooser2 >, bool = true) const;
    template<class VChooser2,class EChooser2 >
        std::pair< Set< PVertex >,Set< PEdge > > getChosenSets( std::pair< VChooser2,EChooser2 >, bool = true) const;

    // Wierzcho³ek/krawedz o podanym numerze.
    PVertex vertByNo( int ) const;
    PEdge edgeByNo( int ) const;
    // Numer wierzcho³ka/krawedzi, -1 w razie braku
    int vertPos( PVertex ) const;
    int edgePos( PEdge ) const;
    // czy graf zawiera dany element
    bool has( PVertex ) const;
    bool has( PEdge ) const;

    // typ krawedzi
    inline EdgeType getEdgeType( PEdge e ) const
        { return self.getEdgeType( e ); }
    // synonim poprzedniej
    EdgeType getType( PEdge e ) const
        { return self.getEdgeType( e ); }

    // wierzcholki koncowe krawedzi
    inline std::pair< PVertex,PVertex > getEdgeEnds( PEdge edge ) const
        { return self.getEdgeEnds( edge ); }
    // synonim poprzedniej
    std::pair< PVertex,PVertex > getEnds( PEdge edge ) const
        { return self.getEdgeEnds( edge ); }
    inline PVertex getEdgeEnd1( PEdge edge ) const
        { return self.getEdgeEnd1( edge ); }
    inline PVertex getEdgeEnd2( PEdge edge ) const
        { return self.getEdgeEnd2( edge ); }

    // orientacja krawedzi wzgledem jej konca
    inline EdgeDirection getEdgeDir( PEdge edge, PVertex v ) const
        { return self.getEdgeDir( edge,v ); }
    // czy wierzcholek jest koncem krawedzi
    bool isEdgeEnd( PEdge edge, PVertex vert ) const
        { return edge && edge->isEnd( vert ); }
    // synonim
    bool isEnd( PEdge edge, PVertex vert ) const
        { return edge && edge->isEnd( vert ); }
    // drugi koniec krawedzi
    PVertex getEdgeEnd( PEdge edge, PVertex vert) const;
    // synonim
    PVertex getEnd( PEdge edge, PVertex vert) const;

    // czy krawedzie sa incydentne
    inline bool incid( PEdge edge1, PEdge edge2 ) const;

    // pobranie pol info
    VertInfoType getVertInfo( PVertex v ) const;
    EdgeInfoType getEdgeInfo( PEdge e ) const;

    // sasiedztwo "otwarte" tj. wierzcholki widoczne z danego v poprzez krawedzie o orientacji wzgledem v zgodnej z maska
    template< class OutputIterator > int getNeighs( OutputIterator, PVertex, EdgeDirection = EdAll ) const;
    Set< PVertex > getNeighSet( PVertex, EdgeDirection = EdAll ) const;
    int getNeighNo( PVertex vert, EdgeDirection mask = EdAll ) const
        { return this->getNeighs( blackHole,vert,mask ); }
    // sasiedztwo "domkniete" tj. jw. ale v jest zawsze dolaczany
    Set< PVertex > getClNeighSet( PVertex, EdgeDirection = EdAll ) const;
    template< class OutputIterator > int getClNeighs( OutputIterator, PVertex, EdgeDirection = EdAll ) const;
    int getClNeighNo( PVertex vert, EdgeDirection direct = EdAll ) const
        { return this->getClNeighs( blackHole,vert,direct ); }

    // stopien wierzcholka, uwzgledniamy krawedzie wychodzace z vert w sposob okreslony maska. Roznica w porownaniu z
    // getEdgeNo(vert,direct) - petle jesli zliczane, to podwojnie
    inline int deg( PVertex vert, EdgeDirection direct = EdAll ) const
        { return self.getEdgeNo( vert,direct ) + ((direct & EdLoop) ? self.getEdgeNo( vert,EdLoop ): 0); }
    // maks/min stopien
    inline int Delta( EdgeDirection direct = EdAll ) const
        { return std::max( 0,this->maxDeg( direct ).second ); }
    inline int delta( EdgeDirection direct = EdAll ) const
        { return std::max( 0,this->minDeg( direct ).second );  }
    // j.w. ale zwracany jest takze wierzcholek realizujacy ekstremum. (NULL,-1) przy braku wierzcholkow
    std::pair< PVertex,int > minDeg( EdgeDirection = EdAll ) const;
    std::pair< PVertex,int > maxDeg( EdgeDirection = EdAll ) const;

    // zapis "macierzy sasiedztwa" z uwglednieniem krawedzi typu zawartego w masce do 2-wym. tablicy assocjacyjnej
    // (PVertex,PVertex)-> cos konwertowalne z bool
    template< class Cont > void getAdj( Cont &, EdgeType mask = EdAll ) const;

    // Informacja o tym, czy krawêdzie s¹ równoleg³e.
    // rownoleglosc: 3 typy relacji rownowaznosci, okreslane maska bitowa
    // zawsze krawedz jest rownolegla do tak samo skierowanej krawedzi tego samego typu
    //reltype - dopuszczalne tylko jednobitowe: EdDirIn, EdDirOut lub EdUndir (taki jest sens parametru maski we wszystkich metodach dot. rownoleglosci)
    //EdDirOut - luk nieskierowany jest rownolegly tylko do tak samo jak on skierowanego luku o tych samych koncach
    //EdDirIn - jest on takze rownolegly do odwrotnie skierowanego luku o tych samych koncach
    //EdUndir - jest on takze rownolegly do krawedzi nieskierowanej o tych samych koncach
    bool areParallel( PEdge, PEdge, EdgeDirection = EdUndir ) const;
    // zbior krawedzi rownoleglych do zadanej z wykluczeniem jej samej
    template< class OutputIterator > int getParals( OutputIterator, PEdge, EdgeDirection = EdUndir ) const;
    Set< PEdge > getParalSet( PEdge, EdgeDirection = EdUndir ) const;
    // liczba krawedzi rownoleglych do danej wliczajac ja sama
    int mu( PEdge edge, EdgeDirection reltype = EdUndir ) const
        { return this->getParals( blackHole,edge,reltype ) + 1; }
    // jej maks. wartosc po wszystkich krawedziach
    int mu( EdgeDirection reltype = EdUndir ) const
        { return maxMu( reltype ).second; }

    // jw. wraz z krawedzia realizujaca to ekstremum
    std::pair< PEdge,int > maxMu( EdgeDirection = EdUndir ) const;

    // rozdziela dany zbior krawedzi na dwa. W pierwszym jest po jednym reprezentancie z kazdej klasy relacji
    // rownoleglosci. Drugi to pozostale krawedzie z tego zbioru
    template< class IterOut1, class IterOut2, class Iterator >
        std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 >,Iterator,Iterator, EdgeType = EdUndir ) const;
    // wersja odporna na ew. powtorzenia - ignorowane w ciagu wejsciowym
    template< class IterOut1, class IterOut2, class Iterator >
        std::pair< int,int > findParals2( std::pair< IterOut1,IterOut2 >,Iterator,Iterator, EdgeType = EdUndir ) const;
    template< class IterOut1, class IterOut2 >
        std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 > out, const Set< PEdge > &eset,
            EdgeType relType = EdUndir ) const
            { return this->findParals( out, eset.begin(),eset.end(),relType ); }
    // zbiorem wejsciowym sa wszystkie krawedzie przy wierzcholku
    template< class IterOut1, class IterOut2 >
        std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 >, PVertex, EdgeType = EdUndir ) const;
    // zbiorem wejsciowym sa wszystkie krawedzie miedzy para wierzcholkow
    template< class IterOut1, class IterOut2 >
        std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 >, PVertex, PVertex, EdgeType = EdUndir ) const;
    // zbiorem wejsciowym sa wszystkie krawedzie
    template< class IterOut1, class IterOut2 >
        std::pair< int,int > findParals( std::pair< IterOut1,IterOut2 >, EdgeType = EdUndir ) const;

    // zbior krawedzi wychodzacych z ktoregos z wierzcholkow z podanego zbioru w sposob podany pierwsza maska
    // druga maska filtruje, czy maja to byc krawedzie prowadzace do tego zbioru (Loop), poza ten zbior (ustawiony bit
    // inny niz Loop), czy wszystkie (ustawiony Loop i ktorys inny)
    template< class Iterator,class OutIter >
        int getIncEdges( OutIter, Iterator, Iterator, EdgeDirection = EdAll, EdgeType = Loop ) const;
    template< class Iterator >
        Set< PEdge > getIncEdgeSet( Iterator beg, Iterator end, EdgeDirection = EdAll, EdgeType = Loop ) const;
    template< class OutIter >
        int getIncEdges( OutIter out, const Set< PVertex > &vset, EdgeDirection type = EdAll, EdgeType kind = Loop ) const
        { return this->getIncEdges( out,vset.begin(),vset.end(),type,kind ); }
    Set< PEdge > getIncEdgeSet( const Set< PVertex > &, EdgeDirection = EdAll, EdgeType = Loop ) const;

    // Podobnie j.w. ale tym razem pobieramy drugie konce (tj. wierzcholki) od takich krawedzi.
    template< class Iterator,class OutIter >
        int getIncVerts( OutIter, Iterator, Iterator, EdgeDirection = EdAll,EdgeType = Loop ) const;
    template< class Iterator >
        Set< PVertex > getIncVertSet( Iterator beg, Iterator end, EdgeDirection = EdAll, EdgeType = Loop ) const;
    template< class OutIter >
        int getIncVerts( OutIter out, const Set< PVertex > &vset, EdgeDirection type = EdAll, EdgeType kind = Loop ) const
        { return this->getIncVerts( out,vset.begin(),vset.end(),type,kind ); }
    Set< PVertex > getIncVertSet( const Set< PVertex > &, EdgeDirection = EdAll, EdgeType = Loop ) const;
};

#include "grconst.hpp"

#endif
