// MatchingPar

template< class DefaultStructs > template< class GraphType, class AssocTab >
    void MatchingPar< DefaultStructs >::fillVertTabNeights( const GraphType &g, AssocTab &vertTabNeights )
{
    //przegladamy wszystkie wierzcholki w grafie, znajdujemy incydentne z U krawedzie i odajemy do zbioru sasiadow U - nowo znalezionego sasiada
    for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
        vertTabNeights[U] = g.getNeighSet( U,EdUndir | EdDirIn | EdDirOut );
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class AssocTab > int
    MatchingPar< DefaultStructs >::firstMatching( const GraphType &g, VertContainer &vertTab, AssocTab &vertTabNeights,
        int matchSize )
{
    // liczba wierzcholkow w grafie
    int vertNo = g.getVertNo();
    // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
    int expo = vertNo;
    typename GraphType::PVertex V;
    typename GraphType::PVertex LOCALARRAY( tabV,vertNo );
    for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
    {
        vertTab[U].vLabB = U;
        //jezeli osiagnelismy skojarzenie o zadanym rozmiarze - koniec
        if (matchSize == (vertNo - expo) / 2) break;
        if (vertTab[U].vMatch == 0)
        {
            for (V = vertTabNeights[U].first(); V; V = vertTabNeights[U].next( V ) )
            {
                if (vertTab[V].vMatch == 0)
                {
                    vertTab[U].vMatch = V;
                    vertTab[V].vMatch = U;
                    expo -= 2; break;
                }
            }
        }
    }
    //liczba wolnych wierzcholkow
    return expo;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EIterIn > int
    MatchingPar< DefaultStructs >::firstMaching( const GraphType &g, VertContainer &vertTab, EIterIn begin, EIterIn end )
{
    // liczba wierzcholkow w grafie
    int vertNo = g.getVertNo();
    // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
    int expo = vertNo;
    typename GraphType::PVertex V,U;
    typename GraphType::PEdge E;
    for( EIterIn it = begin; it != end; ++it )
    {
        E = *it;
        U = g.getEdgeEnd1( E );
        V = g.getEdgeEnd2( E );
        vertTab[V].vMatch = U;
        vertTab[U].vMatch = V;
        expo -= 2;
    }
    for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
        vertTab[U].vLabB = U;
    //liczba wolnych wierzcholkow
    return expo;
}

template< class DefaultStructs > template< class GraphType, class VertContainer > void
    MatchingPar< DefaultStructs >::clearLabels( const GraphType &g, VertContainer &vertTab )
{
    for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
    {
        vertTab[U].vLabS = 0;
        vertTab[U].vLabT[0] = 0;
        vertTab[U].vLabT[1] = 0;
        vertTab[U].vLabB = U;
        vertTab[U].bS = false;
        vertTab[U].bT = false;
    }
}

template< class DefaultStructs > template< class GraphType, class VertContainer > int
    MatchingPar< DefaultStructs >::augmentation( const GraphType &g, VertContainer &vertTab,
        typename GraphType::PVertex tabPath1[],  int len1, typename GraphType::PVertex tabPath2[], int len2, int expo )
{
    //przebiegamy po wszystkich wierzcholkach drogi powiekszajacej naprzemiennej
    //te krawedzie, ktore naleza do drogi powiekszajacej i nie naleza do skojarzenia - teraz zostana dodane do skojarzenia
    //te krawedzie, ktore naleza do drogi powiekszajace i do skojarzenia - zostana wyrzucone ze skojarzenia
    for( int i = len1 - 1; i > 0; i-- )
        if (vertTab[tabPath1[i]].vMatch != tabPath1[i-1])
            vertTab[tabPath1[i]].vMatch = tabPath1[i-1];
        else vertTab[tabPath1[i]].vMatch = tabPath1[i+1];

    if (vertTab[tabPath1[0]].vMatch != tabPath2[0])
    {
        vertTab[tabPath1[0]].vMatch = tabPath2[0];
        vertTab[tabPath2[0]].vMatch = tabPath1[0];
    }
    else
    {
        vertTab[tabPath1[0]].vMatch = tabPath1[1];
        vertTab[tabPath2[0]].vMatch = tabPath2[1];
    }
    for ( int i = 0; i < len2 - 1; i++ )
        if (vertTab[tabPath2[i]].vMatch != tabPath2[i+1])
            vertTab[tabPath2[i]].vMatch = tabPath2[i+1];
        else vertTab[tabPath2[i]].vMatch = tabPath2[i-1];
    expo -= 2;
    return expo;
}

template< class DefaultStructs > template< class PVertex > void MatchingPar< DefaultStructs >::reverseTable(
    PVertex tabPath[], int from, int to )
{
    PVertex vTmp;
    int n = (int)((to - from) / 2) + from;

    for ( int i = from; i <= n; i++ )
    {
        vTmp = tabPath[i];
        tabPath[i] = tabPath[to - i + from];
        tabPath[to - i + from] = vTmp;
    }
}

template< class DefaultStructs > template< class GraphType, class VertContainer > typename GraphType::PVertex
    MatchingPar< DefaultStructs >::backtracking( const GraphType &g, VertContainer &vertTab,
        typename GraphType::PVertex tabPath[], int &iCurr, typename GraphType::PVertex vStart,
        bool isS, typename GraphType::PVertex vStop )
{
    typename GraphType::PVertex vCurr = vStart;

    while (vCurr != vStop && vertTab[vCurr].vLabS != vCurr)
    {
        //dodajemy aktualnie rozpatrywany wierzcholek do tablicy
        tabPath[iCurr] = vCurr;
        iCurr++;
        isS = !isS;

        //jezeli sprawdzamy w vCurr ceche S to nst, rozpatrywany bedzie wierzcholek, na ktory wskazuje cecha vCurr.vLabS
        if (!isS) vCurr = vertTab[vCurr].vLabS;
        //sprawdzamy w vCurr ceche T
        else if (isS)
        {
            //jezeli cecha T jest pojedyncza to nst, rozpatrywany bedzie wierzecholek, na ktory wskazuje cecha vCurr.vLabT[0]
            if (vertTab[vCurr].vLabT[1] == 0) vCurr = vertTab[vCurr].vLabT[0];
            //jezeli cecha T jest podwojna, trzeba wywolac ponownie funkcje backtracking
            else
            {
                typename GraphType::PVertex vNewStop = vCurr;
                //po powrocie z funkcji backtracking bedziemy kontynuowac od wierzcholka, na ktory wskazuje cecha T1
                vCurr = vertTab[vCurr].vLabT[0];
                int iFrom = (int)iCurr;
                backtracking( g,vertTab,tabPath,iCurr,vertTab[vNewStop].vLabT[1],true,vNewStop );
                int iTo = (int)(iCurr - 1);
                reverseTable( tabPath,iFrom,iTo );
            }
        }
    }
    if (vertTab[vCurr].vLabS == vCurr)
    {
        iCurr++;
        tabPath[iCurr - 1] = vCurr;
    }
    return (tabPath[iCurr - 1]);
}

template< class DefaultStructs > template< class GraphType, class VertContainer > void
    MatchingPar< DefaultStructs >::blossoming( const GraphType &g, VertContainer &vertTab,
        typename GraphType::PVertex tabPath1[], int len1, typename GraphType::PVertex tabPath2[], int len2 )
{
    //mamy dwie tablice:
    //tabPath1: i=ip,ip-1,...,i2,i1=r
    //tabPath2: j=iq,iq-1,...,j2,j1=r
    //szukamy podstawy kielicha vBase=im=jm oraz indeksu m, ktory wskazuje ktorym wierzcholkiem z kolei od korzenia jest podstawa
    //istnieje m takie, ze [im=jm and (im=i xor jm=j xor im+1 != jm+1)]
    typename GraphType::PVertex vBase;
    int len, m = 0;
    if (len1 > len2)
    {
        len = len2;
        vBase = tabPath2[0];
    }
    else
    {
        len = len1;
        vBase = tabPath1[0];
    }

    if (len1 < len2)
    {
        //root
        vBase = tabPath1[len1 - 1];
        for( int i = len1 - 1; i > 0; i-- )
            if (tabPath1[i - 1] != tabPath2[i - 1 + len2 - len1] && tabPath1[i] == tabPath2[i + len2 - len1])
            {
                m = len1 - i - 1;
                vBase = tabPath1[i];
                break;
            }
    }
    else
    {
        //root
        vBase = tabPath2[len2 - 1];
        for( int i = len2 - 1; i > 0; i-- )
            if (tabPath2[i - 1] != tabPath1[i - 1 + len1 - len2] && tabPath2[i] == tabPath1[i + len1 - len2])
            {
                m = len2 - i - 1;
                vBase = tabPath2[i];
                break;
            }
    }

    //uzupelniamy brakujace cechy vLabS i vLabT w kielichu
    //uzupelnienie cech dla wierzcholow nalezach do kielicha i do tabPath1
    for( int i = len1 - m - 2; i > 0; i-- )
    {
        //(i nie ma S -> (i,i-1) in M) and i-1 nie ma T
        if (vertTab[tabPath1[i]].vLabS == 0 && vertTab[tabPath1[i-1]].vLabT[0] == 0)
            vertTab[tabPath1[i]].vLabS = tabPath1[i - 1];
        //(i nie ma T -> (i,i-1) not in M) and i-1 nie ma S
        else if (vertTab[tabPath1[i]].vLabT[0] == 0 && vertTab[tabPath1[i - 1]].vLabS == 0)
            vertTab[tabPath1[i]].vLabT[0] = tabPath1[i - 1];
        //[(i nie ma T -> (i,i-1) not in M) and i-1 ma S] -> i musial byc podstawa w jakims poprzednim kielichow zawierajacych i-1
        else if (vertTab[tabPath1[i]].vLabT[0] == 0 && vertTab[tabPath1[i - 1]].vLabS != 0)
        {
            //szukamy ostatniego wierzcholka w ciagu nalezacego do tego samego kielicha co i
            typename GraphType::PVertex vOldBase = vertTab[tabPath1[i]].vLabB;
            int last;
            if (vertTab[tabPath1[0]].vLabB == vOldBase) last = 0;
            else
                for( int j = i - 1; j > 0; j-- )
                    if (vertTab[tabPath1[j - 1]].vLabB != vOldBase)
                    {
                        last = j;
                        break;
                    }
            vertTab[tabPath1[i]].vLabT[0] = tabPath1[last - 1];
            vertTab[tabPath1[i]].vLabT[1] = tabPath1[last];
        }
    }

    //uzupelnienie cech dla wierzcholow nalezach do kielicha i do tabPath2
    for( int i = len2 - m - 2; i > 0; i-- )
    {
        //(i nie ma S -> (i,i-1) in M) and i-1 nie ma T
        if (vertTab[tabPath2[i]].vLabS == 0 && vertTab[tabPath2[i - 1]].vLabT[0] == 0)
            vertTab[tabPath2[i]].vLabS = tabPath2[i - 1];
        //(i nie ma T -> (i,i-1) not in M) and i-1 nie ma S
        else if (vertTab[tabPath2[i]].vLabT[0] == 0 && vertTab[tabPath2[i - 1]].vLabS == 0)
            vertTab[tabPath2[i]].vLabT[0] = tabPath2[i - 1];
        //[(i nie ma T -> (i,i-1) not in M) && i-1 ma S] -> i musial byc podstawa w jakims poprzednim kielichow zawierajacych i-1
        else if (vertTab[tabPath2[i]].vLabT[0] == 0 && vertTab[tabPath2[i - 1]].vLabS != 0)
        {
            typename GraphType::PVertex vOldBase = vertTab[tabPath2[i]].vLabB;
            int last;
            if (vertTab[tabPath2[0]].vLabB == vOldBase) last = 0;
            else
                for( int j = i - 1; j > 0; j-- )
                {
                    if (vertTab[tabPath2[j - 1]].vLabB != vOldBase)
                    {
                        last = j;
                        break;
                    }
                }
            vertTab[tabPath2[i]].vLabT[0] = tabPath2[last - 1];
            vertTab[tabPath2[i]].vLabT[1] = tabPath2[last];
        }
    }

    //na koniec krawedz (tabPath1[0],tabPath2[0])
    //tabPath1[0] nie ma S
    if (vertTab[tabPath1[0]].vLabS == 0) vertTab[tabPath1[0]].vLabS = tabPath2[0];
    //tabPath1[0] nie ma T1
    else if (vertTab[tabPath1[0]].vLabT[0] == 0) vertTab[tabPath1[0]].vLabT[0] = tabPath2[0];
    //tabPath2[0] nie ma S
    if (vertTab[tabPath2[0]].vLabS == 0) vertTab[tabPath2[0]].vLabS = tabPath1[0];
    //tabPath2[0] nie ma T1
    else if (vertTab[tabPath2[0]].vLabT[0] == 0) vertTab[tabPath2[0]].vLabT[0] = tabPath1[0];

    //wszystkie wierzcholki nalezace do kielicha musza dostac ceche vLabB=vBase
    //dla tabPath1 ip,ip-1,...im+1,im
    //dla tabPath2 jq,jq-1,...jm+1
    if (len1 < len2)
    {
        for( int i = 0; i < len1 - m; i++ )
        {
            vertTab[tabPath1[i]].vLabB = vBase;
            vertTab[tabPath2[i]].vLabB = vBase;
        }
        for( int i = len1 - m; i < len2 - m - 1; i++ )
            vertTab[tabPath2[i]].vLabB = vBase;
    }
    else
    {
        for( int i = 0; i < len2 - m; i++ )
        {
            vertTab[tabPath1[i]].vLabB = vBase;
            vertTab[tabPath2[i]].vLabB = vBase;
        }
        for( int i = len2 - m; i < len1 - m - 1; i++ )
            vertTab[tabPath1[i]].vLabB = vBase;
    }
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class AssocTab > int
    MatchingPar< DefaultStructs >::labeling( const GraphType &g, VertContainer &vertTab, AssocTab &vertTabNeights,
    bool isS, typename GraphType::PVertex vCurr, int expo )
{
    int n;
    typename GraphType::PVertex vNext;
    typename GraphType::PVertex LOCALARRAY( tabV,n = g.getVertNo() );

    //cecha S
    if (isS)
    {
        //oznaczamy ceche jako zbadana
        vertTab[vCurr].bS = true;
        //przegladamy wszystkie krawedzie incydentne z wierzcholkiem vCurr
        for( vNext = vertTabNeights[vCurr].first(); vNext; vNext = vertTabNeights[vCurr].next( vNext ) )
        {
            // dla S w vCurr rozwazamy krawedzie E = (vCurr, vNext) nie nalezace do matchingu  oraz maja nalezec do roznych kielichow
            if (vertTab[vNext].vMatch != vCurr && vertTab[vNext].vLabB != vertTab[vCurr].vLabB)
            {
                //jezeli vNext ma ceche S
                if (vertTab[vNext].vLabS != 0)
                {
                    typename GraphType::PVertex LOCALARRAY( tabPathCurr,n );
                    typename GraphType::PVertex LOCALARRAY( tabPathNext,n );

                    int  iLenCurr = 0, iLenNext = 0;
                    //korzenie rozne - znalezlismy droge powiekszajaca
                    if (backtracking( g,vertTab,tabPathCurr,iLenCurr,vCurr,true,0 ) !=
                        backtracking( g,vertTab,tabPathNext,iLenNext,vNext,true,0) )
                    {
                        expo = augmentation( g,vertTab,tabPathCurr,iLenCurr,tabPathNext,iLenNext,expo );
                        break;
                    }
                    //korzen ten sam - znalezlismy klich
                    else blossoming( g,vertTab,tabPathCurr,iLenCurr,tabPathNext,iLenNext );
                }
                //vNext nie ma cechy S i nie ma T, nadamy mu T
                //vNext dostaje ceche T: vCurr
                //jezeli mialby vNext ceche T to mielibysmy cykl parzysty - takie nam nie przeszkadzaja
                else if (vertTab[vNext].vLabT[0] == 0) vertTab[vNext].vLabT[0] = vCurr;
            }
        }
    }
    //cecha T
    else
    {
        //oznaczamy ceche T jako zbadana
        vertTab[vCurr].bT = true;
        //przegladamy wszystkie krawedzie incydentne z wierzcholkiem vCurr
        for( typename GraphType::PEdge E = g.getEdge( vCurr,EdUndir | EdDirIn | EdDirOut ); E;
            E = g.getEdgeNext( vCurr,E,EdUndir | EdDirIn | EdDirOut ) )
        {
            //sasiad wierzcholka vCurr
            vNext = g.getEdgeEnd( E,vCurr );
            if (vertTab[vNext].vMatch == vCurr && vertTab[vNext].vLabB != vertTab[vCurr].vLabB)
            {
                //jezeli vNext ma ceche T
                if (vertTab[vNext].vLabT[0] != 0)
                {
                    typename GraphType::PVertex LOCALARRAY( tabPathCurr,n );
                    typename GraphType::PVertex LOCALARRAY( tabPathNext,n );
                    int  iLenCurr = 0, iLenNext = 0;
                    //korzenie rozne - znalezlismy droge powiekszajaca
                    if (backtracking( g,vertTab,tabPathCurr,iLenCurr,vCurr,false,0) !=
                        backtracking( g,vertTab,tabPathNext,iLenNext,vNext,false,0 ))
                    {
                        expo = augmentation( g,vertTab,tabPathCurr,iLenCurr,tabPathNext,iLenNext,expo );
                        break;
                    }
                    //korzen ten sam - znalezlismy kielich
                    else blossoming( g,vertTab,tabPathCurr,iLenCurr,tabPathNext,iLenNext );
                }
                //vNext nie ma cechy S i nie ma T, nadamy mu S
                //vNext dostaje ceche T: vCurr
                //jezeli mialby vNext ceche S to mielibysmy cykl parzysty - takie nam nie przeszkadzaja
                else if (vertTab[vNext].vLabS == 0) vertTab[vNext].vLabS = vCurr;
            }
        }
    }
    return expo;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class AssocTab > int
    MatchingPar< DefaultStructs >::edmonsAlg( const GraphType &g, VertContainer &vertTab, AssocTab &vertTabNeights,
        int matchSize, int &expo )
{
    int vertNo = g.getVertNo();
    //if true - istnieja niezbadane cechy
    //jezeli mamy conajmniej 2 wolne wierzcholki oraz nie osieglelismy zadanego rozmiaru matchingu
    //jest wiecej niz jeden wolny wierzcholek i istnieje niezbadana cecha
    bool isLabel = true;

    while (expo >= 2 && (matchSize > (vertNo - expo) / 2 || matchSize == -1) && isLabel)
    {
        //nadajemy wszystkim wierzcholkom wolnym ceche S: S
        for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
            //U ma ceche S:U, U jest wolny i jest rootem
            if (vertTab[U].vMatch == 0) vertTab[U].vLabS = U;

        //nie znaleziono drogi powiekszajacej
        bool noAugment = true;

        do
        {
            isLabel = false;
            //bierzemy pierwsza z brzegu niezbadana ceche i ja badamy
            typename GraphType::PVertex vCurr;
            int expo_new;
            for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
            {
                if (vertTab[U].vLabS != 0 && !vertTab[U].bS)
                {
                    vCurr = U;
                    expo_new = labeling( g,vertTab,vertTabNeights,true,vCurr,expo );
                    isLabel = true;
                    break;
                }
                else if (vertTab[U].vLabT[0] != 0 && !vertTab[U].bT)
                {
                    vCurr = U;
                    expo_new = labeling( g,vertTab,vertTabNeights,false,vCurr,expo );
                    isLabel = true;
                    break;
                }
            }
            if (expo > expo_new)
            {
                expo = expo_new;
                noAugment = false;
                clearLabels( g,vertTab );
            }
        } while (isLabel && noAugment);
    }
    return (vertNo - expo) / 2;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EIterOut > int
    MatchingPar< DefaultStructs >::printResultToIterators( const GraphType &g, VertContainer &vertTab,
        EIterOut edgeIterOut, bool ifEdgeCover )
{
    int n;
    typename GraphType::PVertex V, LOCALARRAY( tabV,n = g.getVertNo() );
    typename GraphType::PEdge E;
    //pomocnicze, by do wynikow nie doataly sie duplikaty krawedzi
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,typename GraphType::PVertex>::Type
        tabVTmp( n );
    int licz = 0;
    for( typename GraphType::PVertex U = g.getVert(); U; U = g.getVertNext( U ) )
    {
        if (vertTab[U].vMatch != 0 && tabVTmp[U] != U)
        {
            V = vertTab[U].vMatch;
            *edgeIterOut = vertTab[V].eMatch = vertTab[U].eMatch =
                g.getEdge( U,V,Koala::EdUndir | Koala::EdDirIn | Koala::EdDirOut );
            ++edgeIterOut;
            licz++;
            //oznaczenie wierzcholka V, ze krawedz z nim incydentna zostala dodana do strumienia wyjsciowego
            tabVTmp[V] = V;
        }
        else if (ifEdgeCover && vertTab[U].vMatch == 0)
        {
            E = g.getEdge( U,Koala::EdUndir | Koala::EdDirIn | Koala::EdDirOut );
            if (E)
            {
                *edgeIterOut = E;
                ++edgeIterOut;
                licz++;
            }
        }
    }
    return licz;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EIterOut > int
    MatchingPar< DefaultStructs >::findMax( const GraphType &g, VertContainer &avertTab, EIterOut edgeIterOut,
        int matchSize )
{
    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type
        localvertTab;
    typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
        VertLabs< GraphType > >::Type >::Type &vertTab = BlackHoleSwitch< VertContainer,typename DefaultStructs::
        template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type >::get( avertTab,localvertTab );
    int n = g.getVertNo();
    if (DefaultStructs::ReserveOutAssocCont || isBlackHole( avertTab )) vertTab.reserve( n );

    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
        Set< typename GraphType::PVertex > >::Type vertTabNeights( n );
    fillVertTabNeights( g,vertTabNeights );

    //funkcja tworzy skojarzenie poczatkowe, zwraca liczbe wierzcholkow wolnych w grafie
    int expo = firstMatching( g,vertTab,vertTabNeights,matchSize );
    //Algorytm Edmondsa znajduje skojarzenie o zadanym rozmiarze matchSize, zwraca znaleziony rozmiar skojarzenia
    matchSize = edmonsAlg( g,vertTab,vertTabNeights,matchSize,expo );
    //wpisanie wynikow do edgeIterOut
    assert( matchSize == printResultToIterators( g,vertTab,edgeIterOut ) );
    //zwracamy rozmiar matchingu
    return matchSize;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EIterIn, class EIterOut >
    int MatchingPar< DefaultStructs >::findMax( const GraphType &g, VertContainer &avertTab, EIterIn vertIterInBegin,
    EIterIn vertIterInEnd, EIterOut edgeIterOut, int matchSize )
{
    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,VertLabs< GraphType> >::Type localvertTab;
    typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
        VertLabs< GraphType > >::Type >::Type &vertTab = BlackHoleSwitch< VertContainer,typename DefaultStructs::
        template AssocCont<typename GraphType::PVertex,VertLabs< GraphType > >::Type >::get( avertTab,localvertTab );
    int n = g.getVertNo();
    if (DefaultStructs::ReserveOutAssocCont || isBlackHole( avertTab )) vertTab.reserve( n );

    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
        Set< typename GraphType::PVertex> >::Type vertTabNeights( n );
    fillVertTabNeights( g,vertTabNeights );

    //funkcja tworzy skojarzenie poczatkowe, na podstawie otrzymanego strumienia wierzcholkowi
    int expo = firstMaching( g,vertTab,vertIterInBegin,vertIterInEnd );
    //Algorytm Edmondsa znajduje skojarzenie o zadanym rozmiarze matchSize, zwraca znaleziony rozmiar skojarzenia
    matchSize = edmonsAlg( g,vertTab,vertTabNeights,matchSize,expo );
    //wpisanie wynikow do edgeIterOut
    assert( matchSize == printResultToIterators( g,vertTab,edgeIterOut ) );
    //zwracamy rozmiar matchingu
    return matchSize;
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EIterOut >
    int MatchingPar< DefaultStructs >::minEdgeCover( const GraphType &g, VertContainer &avertTab, EIterOut edgeIterOut )
{
    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type localvertTab;
    typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
        VertLabs< GraphType> >::Type >::Type &vertTab = BlackHoleSwitch< VertContainer,typename DefaultStructs::
        template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type >::get( avertTab,localvertTab );
    int n = g.getVertNo();
    if (DefaultStructs::ReserveOutAssocCont ||isBlackHole( avertTab )) vertTab.reserve( n );
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,
        Set< typename GraphType::PVertex > >::Type vertTabNeights( n );
    fillVertTabNeights( g,vertTabNeights );

    //funkcja tworzy skojarzenie poczatkowe, zwraca liczbe wierzcholkow wolnych w grafie
    int expo = firstMatching( g,vertTab,vertTabNeights,-1 );
    //Algorytm Edmondsa znajduje skojarzenie o maksymalnym rozmiarze, zwraca znaleziony rozmiar skojarzenia
    edmonsAlg( g,vertTab,vertTabNeights,-1,expo );
    //wpisanie wynikow do edgeIterOut
    return printResultToIterators(g,vertTab,edgeIterOut,true);
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EIterOut >
    int MatchingPar< DefaultStructs >::greedy( const GraphType &g, VertContainer &avertTab, EIterOut edgeIterOut,
        int matchSize )
{
    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type localvertTab;
    typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
        VertLabs< GraphType > >::Type >::Type &vertTab = BlackHoleSwitch< VertContainer,typename DefaultStructs::
        template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type >::get( avertTab,localvertTab );

    // liczba wierzcholkow w grafie
    int vertNo = g.getVertNo();
    if (DefaultStructs::ReserveOutAssocCont ||isBlackHole( avertTab )) vertTab.reserve( vertNo );
    // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
    int expo = vertNo;
    typename GraphType::PVertex U,V;

    //przegladamy krawedzie grafu
    for( typename GraphType::PEdge E = g.getEdge( EdUndir | EdDirIn | EdDirOut ); E;
        E = g.getEdgeNext( E,EdUndir | EdDirIn | EdDirOut ) )
    {
        //jezeli osiagniemy zadany rozmiar skojarzenia - koniec
        if (matchSize == (vertNo - expo) / 2) break;
        U = g.getEdgeEnd1( E );
        V = g.getEdgeEnd2( E );
        //jezeli U i V sa wolne tzn. ze mozna E dodac do skojarzenia
        if (vertTab[U].vMatch == 0 && vertTab[V].vMatch == 0)
        {
            vertTab[U].vMatch = V;
            vertTab[V].vMatch = U;
            *edgeIterOut = vertTab[V].eMatch = vertTab[U].eMatch = E;
            ++edgeIterOut;
            expo -= 2;
        }
    }
    //zwracamy rozmiar matchingu
    return ((vertNo - expo) / 2);
}

template< class DefaultStructs > template< class GraphType, class VertContainer, class EIterIn, class EIterOut >
    int MatchingPar< DefaultStructs >::greedy( const GraphType &g, VertContainer &avertTab, EIterIn edgeIterInBegin,
        EIterIn edgeIterInEnd, EIterOut edgeIterOut, int matchSize )
{
    typename DefaultStructs::template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type localvertTab;
    typename BlackHoleSwitch< VertContainer,typename DefaultStructs::template AssocCont< typename GraphType::PVertex,
        VertLabs< GraphType > >::Type >::Type &vertTab = BlackHoleSwitch< VertContainer,typename DefaultStructs::
        template AssocCont< typename GraphType::PVertex,VertLabs< GraphType > >::Type >::get( avertTab,localvertTab );

    // liczba wierzcholkow w grafie
    int vertNo = g.getVertNo();
    if (DefaultStructs::ReserveOutAssocCont ||isBlackHole( avertTab )) vertTab.reserve( vertNo );
    // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
    int expo = vertNo;
    typename GraphType::PVertex U,V;

    //przeglasamy podane krawedzie
    for( EIterIn itE = edgeIterInBegin; itE != edgeIterInEnd; ++itE )
    {
        //jezeli osiagniemy zadany rozmiar skojarzenia - koniec
        if (matchSize == (vertNo - expo) / 2) break;
        U = g.getEdgeEnd1( *itE );
        V = g.getEdgeEnd2( *itE );
        //jezeli U i V sa wolne tzn. ze mozna E dodac do skojarzenia
        if (vertTab[U].vMatch == 0 && vertTab[V].vMatch == 0)
        {
            vertTab[U].vMatch = V;
            vertTab[V].vMatch = U;
            *edgeIterOut = vertTab[V].eMatch = vertTab[U].eMatch = *itE;
            ++edgeIterOut;
            expo -= 2;
        }
    }
    //zwracamy rozmiar matchingu
    return ((vertNo - expo) / 2);
}

template< class DefaultStructs > template< class GraphType, class EIterIn > bool MatchingPar< DefaultStructs >::test(
    const GraphType &g, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd )
{
    // liczba wierzcholkow w grafie
    int vertNo = g.getVertNo();
    // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
    int expo = vertNo;
    typename GraphType::PVertex U,V;
     //jezeli true - wierzcholek nalezy do matchingu
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,bool >::Type vertTabMatch( vertNo );

    //przeglasamy podane krawedzie
    for( EIterIn itE = edgeIterInBegin; itE != edgeIterInEnd; ++itE )
    {
        U = g.getEdgeEnd1( *itE );
        V = g.getEdgeEnd2( *itE );
        //jezeli ktorykolwiek z wierzcholkow tworzacych krawedz nie jest wolny to nie mamy matchingu
        if (vertTabMatch.hasKey( U ) || vertTabMatch.hasKey( V )) return false;
        vertTabMatch[U] = vertTabMatch[V] = true;
    }
    return true;
}

// StableMatchingPar

template< class DefaultStructs > template< class GraphType, class EIterIn, class Comp >
    std::pair< bool,typename GraphType::PEdge > StableMatchingPar< DefaultStructs >::test( const GraphType &g,
        Comp compare, EIterIn edgeIterInBegin, EIterIn edgeIterInEnd )
{
    std::pair< bool,typename GraphType::PEdge > res;
    res.second = 0;
    if (!(res.first = MatchingPar< DefaultStructs >::template test( g,edgeIterInBegin,edgeIterInEnd ))) return res;
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,typename GraphType::PEdge >::Type
        match( g.getVertNo() );
    for( ; edgeIterInBegin != edgeIterInEnd; ++edgeIterInBegin )
        match[g.getEdgeEnd1( *edgeIterInBegin )] = match[g.getEdgeEnd2( *edgeIterInBegin )] = *edgeIterInBegin;
    for( typename GraphType::PEdge e = g.getEdge( Directed | Undirected ); e; e = g.getEdgeNext( e,Directed | Undirected ) )
        if (match[g.getEdgeEnd1( e )] != e)
            if ((!match[g.getEdgeEnd1( e )] || compare( g.getEdgeEnd1( e ),match[g.getEdgeEnd1( e )],e )) &&
               (!match[g.getEdgeEnd2( e )] || compare( g.getEdgeEnd2( e ),match[g.getEdgeEnd2( e )],e )))
                return std::make_pair( false,e );
    return std::make_pair( true,(typename GraphType::PEdge)0 );
}

template< class DefaultStructs > template< class GraphType, class VIterIn, class Comp, class vertCont, class EIterOut >
    int StableMatchingPar< DefaultStructs >::bipartFind( const GraphType &g, VIterIn begin, VIterIn end, Comp compare,
        vertCont &verttab, EIterOut out )
{
    int licz = 0, n1 = 0, n;
    for( VIterIn it = begin; it != end; ++it )
        if (g.deg( *it,Directed | Undirected )) n1++;
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,typename GraphType::PEdge* >::Type
        bufs( n1 );
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,int > ::Type love( n1 );
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,char > ::Type free( n1 );
    typename GraphType::PEdge LOCALARRAY( incids,g.getEdgeNo( Directed | Undirected ) );
    typename DefaultStructs:: template AssocCont< typename GraphType::PVertex,typename GraphType::PEdge >::Type
        match( (n=g.getVertNo()) - n1 );

    for( VIterIn it = begin; it != end; ++it )
        if (g.deg( *it,Directed | Undirected ))
        {
            love[*it] = 0;
            free[*it];
            int deg = g.getEdges( bufs[*it] = incids + licz,*it,Directed | Undirected );
            DefaultStructs::sort( incids + licz,incids + licz + deg,SortCmp< GraphType,Comp >( *it,compare ) );
            licz += deg;
        }

    while (!free.empty())
    {
        typename GraphType::PVertex u = free.firstKey(),v,w;
        typename GraphType::PEdge e = bufs[u][love[u]];
        v = g.getEdgeEnd( e,u );
        if (match.hasKey( v ))
            if (compare( v,match[v],e ))
            {
                w = g.getEdgeEnd( match[v],v );
                if (++love[w] < g.deg( w,Directed | Undirected )) free[w];
                match.delKey( v );
            }
        if (!match.hasKey( v ))
        {
            match[v] = e;
            free.delKey( u );
        }
        else if (++love[u] == g.deg( u,Directed | Undirected )) free.delKey( u );
    }
    licz = 0;
    if (DefaultStructs::ReserveOutAssocCont) verttab.reserve(n);
    for( typename GraphType::PVertex u = match.firstKey(); u; u = match.nextKey( u ) )
    {
        licz++;
        *out = match[u];
        ++out;
        if (!isBlackHole( verttab ))
        {
            VertLabs< GraphType > lu( u,match[u] ), ln( g.getEdgeEnd( match[u],u ),match[u] );
            verttab[u] = lu;
            verttab[g.getEdgeEnd( match[u],u )] = ln;
        }
    }
    return licz;
}
