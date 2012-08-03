#ifndef KOALA_DEF_FACTOR_H
#define KOALA_DEF_FACTOR_H

#include "../base/def_struct.h"
#include "../graph/view.h"



namespace Koala {



//algorytm szukajacy najliczniejszego skojarzenia w grafie dowolnym
template <class DefaultStructs>
class MatchingPar {

    public:

    template <class GraphType> struct VertLabs {

        typename GraphType::PVertex vMatch;   //wierzcholek skojarzany
        typename GraphType::PVertex vLabT[2]; //cecha T (VertLabs[Vi].T=Vj oznacza, ze istnieje parzysta droga naprzemienna z wierzcholka wolnego Vr do Vi, gdzie poprzednikiem Vi jest Vj)
        typename GraphType::PVertex vLabS;    //cecha S (VertLabs[Vi].S=Vj -||- nieparzysta droga -||-)
        typename GraphType::PVertex vLabB;    //cecha b (VertLabs[Vi].b=Vj oznacza, ze najbardziej zewnetrzny kielich, do ktorego nalezy wierzcholek Vi jest reprezentowany przez wierzcholek Vj, ktory jest podstawa tego kielicha)
        bool bS;                              //jezeli S niezbadana true, jezeli nie ma S lub S zbadana false (moze powinnam to trzymac na liscie cech?)
        bool bT;                              //jezeli T niezbadana true, jezeli nie ma T lub T zbadana false (moze to powinnam trzymac na liscie cech?)
        VertLabs() : vMatch(0),  vLabS(0), vLabB(0), bS(false), bT(false)
        { vLabT[0]=vLabT[1]=0; };
    };

    protected:

    //procedura wypelnia vertTabNeights - kazdemu wierzcholkowi przybisuje zbior jego sasiadow
    template <class GraphType,class AssocTab>
    static void
        fillVertTabNeights(
            const GraphType & g,
            AssocTab & vertTabNeights)
        {
            //przegladamy wszystkie wierzcholki w grafie, znajdujemy incydentne z U krawedzie i odajemy do zbioru sasiadow U - nowo znalezionego sasiada
            for (typename GraphType::PVertex U = g.getVert(); U; U=g.getVertNext(U))
//                for (typename GraphType::PEdge E=g.getEdge(U,EdUndir|EdDirIn|EdDirOut);E;E=g.getEdgeNext(U,E,EdUndir|EdDirIn|EdDirOut))
                    vertTabNeights[U]=g.getNeighSet (U,EdUndir|EdDirIn|EdDirOut);
        }

    //skojarzenie poczatkowe
    template <class GraphType, class VertContainer,class AssocTab>
    static int
        firstMatching(
            const GraphType & g,
            VertContainer & vertTab,
            AssocTab& vertTabNeights,
            int matchSize)
        {
            int vertNo = g.getVertNo();        // liczba wierzcholkow w grafie
            int expo = vertNo;                 // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
            typename GraphType::PVertex V;
            typename GraphType::PVertex LOCALARRAY(tabV,vertNo);
            for (typename GraphType::PVertex U = g.getVert(); U; U=g.getVertNext(U))
            {
                vertTab[U].vLabB = U;
                if (matchSize == (vertNo-expo)/2) break; //jezeli osiagnelismy skojarzenie o zadanym rozmiarze - koniec
                if (vertTab[U].vMatch == 0)
                {
                    for (V=vertTabNeights[U].first(); V; V=vertTabNeights[U].next(V))
                    {
//                        V = *it; //sasiad wierzcholka U
                        if (vertTab[V].vMatch == 0)
                        {
                            vertTab[U].vMatch = V;
                            vertTab[V].vMatch = U;
                            expo -= 2; break;
                        }
                    }
                }
            }
            return expo; //liczba wolnych wierzcholkow
        }

    //utworzenie skojarzenia poczatkowego ze podanego na wejsciu strumienia wierzcholkow tworzacych skojarzone krawedzie
    template <class GraphType, class VertContainer, class EIterIn>
    static int
        firstMaching(
            const GraphType & g,
            VertContainer & vertTab,
            EIterIn begin,
            EIterIn end)
        {
            int vertNo = g.getVertNo();        // liczba wierzcholkow w grafie
            int expo = vertNo;                 // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
            typename GraphType::PVertex V, U;
            typename GraphType::PEdge E;
            for (EIterIn it = begin; it!=end; ++it)
            {
                E = *it; U = g.getEdgeEnd1(E); V = g.getEdgeEnd2(E);
                vertTab[V].vMatch = U; vertTab[U].vMatch = V;
                expo -= 2 ;
            }
            for (typename GraphType::PVertex U = g.getVert(); U; U=g.getVertNext(U))
                vertTab[U].vLabB = U;
            return expo; //liczba wolnych wierzcholkow
        }

     //funcja czysci wszystkie etykiety na wierzcholkach
    template <class GraphType, class VertContainer>
    static void
        clearLabels(
            const GraphType & g,
            VertContainer& vertTab)
        {
            for (typename GraphType::PVertex U = g.getVert(); U; U=g.getVertNext(U))
            {
                vertTab[U].vLabS = 0;
                vertTab[U].vLabT[0] = 0;
                vertTab[U].vLabT[1] = 0;
                vertTab[U].vLabB = U;
                vertTab[U].bS = false;
                vertTab[U].bT = false;
            }
        }

    //funkcja realizuje powiekszenie skojarzenia, gdy zostala znaleziona naprzeminna droga powiekszajaca
    template <class GraphType, class VertContainer>
    static int
        augmentation(
            const GraphType & g,
            VertContainer& vertTab,
            typename GraphType::PVertex tabPath1[],
            int len1,
            typename GraphType::PVertex tabPath2[],
            int len2,
            int expo)
        {
            //przebiegamy po wszystkich wierzcholkach drogi powiekszajacej naprzemiennej
            //te krawedzie, ktore naleza do drogi powiekszajacej i nie naleza do skojarzenia - teraz zostana dodane do skojarzenia
            //te krawedzie, ktore naleza do drogi powiekszajace i do skojarzenia - zostana wyrzucone ze skojarzenia
            for (int i=len1-1; i>0; i--)
            {
                if (vertTab[tabPath1[i]].vMatch != tabPath1[i-1])
                    vertTab[tabPath1[i]].vMatch = tabPath1[i-1];
                else vertTab[tabPath1[i]].vMatch = tabPath1[i+1];
            }

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
            for (int i=0; i<len2-1; i++)
            {
                if (vertTab[tabPath2[i]].vMatch != tabPath2[i+1])
                    vertTab[tabPath2[i]].vMatch = tabPath2[i+1];
                else vertTab[tabPath2[i]].vMatch = tabPath2[i-1];
            }
            expo -=2;
            return expo;
        }//end augmentation

//reverse table
template <class PVertex>
     static void
        reverseTable(
            PVertex tabPath[],
            int from,
            int to)
        {
            PVertex vTmp;
            int n = (int)((to-from)/2)+from;

            for (int i=from; i<=n; i++)
            {
                vTmp = tabPath[i];
                tabPath[i] = tabPath[to-i+from];
                tabPath[to-i+from] = vTmp;
            }
        }

    //funkcja, ktora zwraca korzen oraz zapisuje do tabPath liste wierzcholkow na drodze od vCurr do Root
    template <class GraphType, class VertContainer>
    static typename GraphType::PVertex
        backtracking(
            const GraphType & g,
            VertContainer& vertTab,
            typename GraphType::PVertex tabPath[],  //tablica do ktorej zapisujemy kolejne wierzcholki na sciezce do korzenia
            int & iCurr,                            //pozycja w tablicy tabPath do ktorej teraz bedziemy zapisywac kolejny wierzcholek
            typename GraphType::PVertex vStart,     //od tego wierzcholka zaczynamy wedrowke
            bool isS,                               //jezeli true - zaczynamy od cechy S w vStart, jezeli false to od cechy T
            typename GraphType::PVertex vStop)      //warunek stopu - gdy dotrzemy do wierzcholka vStop konczymy wedrowke po wierzcholkach w danym kierunku,jezeli vStop==0 to konczymy gdy dotrzemy do korzenia
        {
            typename GraphType::PVertex vCurr = vStart;

            while (vCurr!=vStop and vertTab[vCurr].vLabS!=vCurr)
            {
                tabPath[iCurr] = vCurr; //dodajemy aktualnie rozpatrywany wierzcholek do tablicy
                iCurr++;
                isS = !isS;

                if (!isS) //jezeli sprawdzamy w vCurr ceche S to nst, rozpatrywany bedzie wierzcholek, na ktory wskazuje cecha vCurr.vLabS
                    vCurr = vertTab[vCurr].vLabS;
                else if (isS)  //sprawdzamy w vCurr ceche T
                {
                    if (vertTab[vCurr].vLabT[1]==0) //jezeli cecha T jest pojedyncza to nst, rozpatrywany bedzie wierzecholek, na ktory wskazuje cecha vCurr.vLabT[0]
                        vCurr = vertTab[vCurr].vLabT[0];
                    else  //jezeli cecha T jest podwojna, trzeba wywolac ponownie funkcje backtracking
                    {
                        typename GraphType::PVertex vNewStop = vCurr;
                        vCurr = vertTab[vCurr].vLabT[0]; //po powrocie z funkcji backtracking bedziemy kontynuowac od wierzcholka, na ktory wskazuje cecha T1
                        int iFrom = (int)iCurr;
                        backtracking (g, vertTab, tabPath, iCurr, vertTab[vNewStop].vLabT[1], true, vNewStop);
                        int iTo = (int)(iCurr-1);
                        reverseTable (tabPath, iFrom, iTo);
                    }
                }
            }
            if (vertTab[vCurr].vLabS==vCurr)
            {
                iCurr++;
                tabPath[iCurr-1] = vCurr;
            }
            return (tabPath[iCurr-1]);
        }//end backtracking

    //obrobka znalezionego kielicha
    template <class GraphType, class VertContainer>
    static void
        blossoming(
            const GraphType & g,
            VertContainer& vertTab,
            typename GraphType::PVertex tabPath1[],
            int len1,
            typename GraphType::PVertex tabPath2[],
            int len2)
        {
            //mamy dwie tablice:
            //tabPath1: i=ip,ip-1,...,i2,i1=r
            //tabPath2: j=iq,iq-1,...,j2,j1=r
            //szukamy podstawy kielicha vBase=im=jm oraz indeksu m, ktory wskazuje ktorym wierzcholkiem z kolei od korzenia jest podstawa
            //istnieje m takie, ze [im=jm and (im=i xor jm=j xor im+1 != jm+1)]
            typename GraphType::PVertex vBase;
            int len, m = 0;
            if (len1>len2) {len = len2; vBase = tabPath2[0];}
            else {len = len1; vBase = tabPath1[0];}

            if (len1<len2)
            {
                vBase = tabPath1[len1-1]; //root
                for (int i=len1-1; i>0; i--)
                    if (tabPath1[i-1]!=tabPath2[i-1+len2-len1] and tabPath1[i]==tabPath2[i+len2-len1])
                    {
                        m = len1-i-1;
                        vBase = tabPath1[i];
                        break;
                    }
            }
            else
            {
                vBase = tabPath2[len2-1]; //root
                for (int i=len2-1; i>0; i--)
                    if (tabPath2[i-1]!=tabPath1[i-1+len1-len2] and tabPath2[i]==tabPath1[i+len1-len2])
                    {
                        m = len2-i-1;
                        vBase = tabPath2[i];
                        break;
                    }
            }

            //uzupelniamy brakujace cechy vLabS i vLabT w kielichu

            //uzupelnienie cech dla wierzcholow nalezach do kielicha i do tabPath1
            for (int i=len1-m-2; i>0; i--)
            {
                if (vertTab[tabPath1[i]].vLabS==0 and vertTab[tabPath1[i-1]].vLabT[0]==0) //(i nie ma S -> (i,i-1) in M) and i-1 nie ma T
                    vertTab[tabPath1[i]].vLabS=tabPath1[i-1];
                else if (vertTab[tabPath1[i]].vLabT[0]==0 and vertTab[tabPath1[i-1]].vLabS==0) //(i nie ma T -> (i,i-1) not in M) and i-1 nie ma S
                    vertTab[tabPath1[i]].vLabT[0]=tabPath1[i-1];
                else if (vertTab[tabPath1[i]].vLabT[0]==0 and vertTab[tabPath1[i-1]].vLabS!=0) //[(i nie ma T -> (i,i-1) not in M) and i-1 ma S] -> i musial byc podstawa w jakims poprzednim kielichow zawierajacych i-1
                {
                    //szukamy ostatniego wierzcholka w ciagu nalezacego do tego samego kielicha co i
                    typename GraphType::PVertex vOldBase = vertTab[tabPath1[i]].vLabB;
                    int last;
                    if (vertTab[tabPath1[0]].vLabB == vOldBase) last = 0;
                    else
                        for (int j=i-1; j>0; j--)
                            if (vertTab[tabPath1[j-1]].vLabB != vOldBase)
                            {
                                last = j;
                                break;
                            }
                    vertTab[tabPath1[i]].vLabT[0] = tabPath1[last-1];
                    vertTab[tabPath1[i]].vLabT[1] = tabPath1[last];
               }
            }

            //uzupelnienie cech dla wierzcholow nalezach do kielicha i do tabPath2
            for (int i=len2-m-2; i>0; i--)
            {
                if (vertTab[tabPath2[i]].vLabS==0 and vertTab[tabPath2[i-1]].vLabT[0]==0) //(i nie ma S -> (i,i-1) in M) and i-1 nie ma T
                    vertTab[tabPath2[i]].vLabS=tabPath2[i-1];
                else if (vertTab[tabPath2[i]].vLabT[0]==0 and vertTab[tabPath2[i-1]].vLabS==0) //(i nie ma T -> (i,i-1) not in M) and i-1 nie ma S
                    vertTab[tabPath2[i]].vLabT[0]=tabPath2[i-1];
                else if (vertTab[tabPath2[i]].vLabT[0]==0 and vertTab[tabPath2[i-1]].vLabS!=0) //[(i nie ma T -> (i,i-1) not in M) and i-1 ma S] -> i musial byc podstawa w jakims poprzednim kielichow zawierajacych i-1
                {
                    typename GraphType::PVertex vOldBase = vertTab[tabPath2[i]].vLabB;
                    int last;
                    if (vertTab[tabPath2[0]].vLabB == vOldBase) last = 0;
                    else
                        for (int j=i-1; j>0; j--)
                        {
                            if (vertTab[tabPath2[j-1]].vLabB != vOldBase)
                            {
                                last = j;
                                break;
                            }
                        }
                    vertTab[tabPath2[i]].vLabT[0] = tabPath2[last-1];
                    vertTab[tabPath2[i]].vLabT[1] = tabPath2[last];
                }
            }

            //na koniec krawedz (tabPath1[0],tabPath2[0])
            if (vertTab[tabPath1[0]].vLabS==0) //tabPath1[0] nie ma S
                vertTab[tabPath1[0]].vLabS = tabPath2[0];
            else if (vertTab[tabPath1[0]].vLabT[0]==0) //tabPath1[0] nie ma T1
                vertTab[tabPath1[0]].vLabT[0] = tabPath2[0];
            if (vertTab[tabPath2[0]].vLabS==0) //tabPath2[0] nie ma S
                vertTab[tabPath2[0]].vLabS = tabPath1[0];
            else if (vertTab[tabPath2[0]].vLabT[0]==0) //tabPath2[0] nie ma T1
                vertTab[tabPath2[0]].vLabT[0] = tabPath1[0];

            //wszystkie wierzcholki nalezace do kielicha musza dostac ceche vLabB=vBase
            //dla tabPath1 ip,ip-1,...im+1,im
            //dla tabPath2 jq,jq-1,...jm+1
            if (len1<len2)
            {
                for (int i=0; i<len1-m; i++)
                {
                     vertTab[tabPath1[i]].vLabB = vBase;
                     vertTab[tabPath2[i]].vLabB = vBase;
                }
                for (int i=len1-m; i<len2-m-1; i++)
                    vertTab[tabPath2[i]].vLabB = vBase;
            }
            else
            {
                for (int i=0; i<len2-m; i++)
                {
                     vertTab[tabPath1[i]].vLabB = vBase;
                     vertTab[tabPath2[i]].vLabB = vBase;
                }
                for (int i=len2-m; i<len1-m-1; i++)
                    vertTab[tabPath1[i]].vLabB = vBase;
            }

        }//end blossoming

    //etykietowanie kolejnych wierzcholkow
    template <class GraphType, class VertContainer, class AssocTab>
    static int
        labeling(
            const GraphType & g,
            VertContainer& vertTab,
            AssocTab& vertTabNeights,
            bool isS, //if true - S, if false - T
            typename GraphType::PVertex vCurr,
            int expo)
        {
            typename GraphType::PVertex vNext;
            typename GraphType::PVertex LOCALARRAY(tabV,g.getVertNo());

            if (isS) //cecha S
            {
                vertTab[vCurr].bS = true; //oznaczamy ceche jako zbadana
                //przegladamy wszystkie krawedzie incydentne z wierzcholkiem vCurr
                for (vNext=vertTabNeights[vCurr].first(); vNext; vNext=vertTabNeights[vCurr].next(vNext))
                {
//                    vNext = *it; //sasiad wierzcholka vCurr
                    if (vertTab[vNext].vMatch!=vCurr and vertTab[vNext].vLabB!=vertTab[vCurr].vLabB) // dla S w vCurr rozwazamy krawedzie E = (vCurr, vNext) nie nalezace do matchingu  oraz maja nalezec do roznych kielichow
                    {
                        if (vertTab[vNext].vLabS!=0) //jezeli vNext ma ceche S
                        {
                            typename GraphType::PVertex LOCALARRAY(tabPathCurr,g.getVertNo());
                            typename GraphType::PVertex LOCALARRAY(tabPathNext,g.getVertNo());

                            int  iLenCurr = 0, iLenNext = 0;
                            if (backtracking(g, vertTab, tabPathCurr, iLenCurr, vCurr, true, 0) != backtracking(g, vertTab, tabPathNext, iLenNext, vNext, true, 0)) //korzenie rozne - znalezlismy droge powiekszajaca
                            {
                                expo = augmentation (g, vertTab, tabPathCurr, iLenCurr, tabPathNext, iLenNext, expo);
                                break;
                            }
                            else //korzen ten sam - znalezlismy klich
                                blossoming (g, vertTab, tabPathCurr, iLenCurr, tabPathNext, iLenNext);
                        }
                        else if (vertTab[vNext].vLabT[0]==0) //vNext nie ma cechy S i nie ma T, nadamy mu T
                           vertTab[vNext].vLabT[0] = vCurr; //vNext dostaje ceche T: vCurr
                        //jezeli mialby vNext ceche T to mielibysmy cykl parzysty - takie nam nie przeszkadzaja
                    }
                }
            }
            else //cecha T
            {
                vertTab[vCurr].bT = true; //oznaczamy ceche T jako zbadana
                //przegladamy wszystkie krawedzie incydentne z wierzcholkiem vCurr
                for (typename GraphType::PEdge E=g.getEdge(vCurr,EdUndir|EdDirIn|EdDirOut);E;E=g.getEdgeNext(vCurr,E,EdUndir|EdDirIn|EdDirOut))
                {
                    vNext = g.getEdgeEnd (E,vCurr); //sasiad wierzcholka vCurr
                    if (vertTab[vNext].vMatch==vCurr and vertTab[vNext].vLabB!=vertTab[vCurr].vLabB)
                    {
                       if (vertTab[vNext].vLabT[0]!=0) //jezeli vNext ma ceche T
                        {
                            typename GraphType::PVertex LOCALARRAY(tabPathCurr,g.getVertNo());
                            typename GraphType::PVertex LOCALARRAY(tabPathNext,g.getVertNo());
                            int  iLenCurr = 0, iLenNext = 0;
                            if (backtracking(g, vertTab, tabPathCurr, iLenCurr, vCurr, false, 0) != backtracking(g, vertTab, tabPathNext, iLenNext, vNext, false, 0)) //korzenie rozne - znalezlismy droge powiekszajaca
                            {
                                expo = augmentation (g, vertTab, tabPathCurr, iLenCurr, tabPathNext, iLenNext, expo);
                                break;
                            }
                            else //korzen ten sam - znalezlismy kielich
                                blossoming (g, vertTab, tabPathCurr, iLenCurr, tabPathNext, iLenNext);
                        }
                        else if (vertTab[vNext].vLabS==0) //vNext nie ma cechy S i nie ma T, nadamy mu S
                            vertTab[vNext].vLabS = vCurr; //vNext dostaje ceche T: vCurr
                        //jezeli mialby vNext ceche S to mielibysmy cykl parzysty - takie nam nie przeszkadzaja
                    }
                }
            }
            return expo;
        }//end labeling

    //algorytm Edmondsa
    template <class GraphType, class VertContainer,class AssocTab>
    static int
        edmonsAlg(
            const GraphType & g,
            VertContainer& vertTab,
            AssocTab& vertTabNeights,
            int matchSize,
            int & expo)
    {
        int vertNo = g.getVertNo();
        bool isLabel = true; //if true - istnieja niezbadane cechy
        //jezeli mamy conajmniej 2 wolne wierzcholki oraz nie osieglelismy zadanego rozmiaru matchingu
        //jest wiecej niz jeden wolny wierzcholek i istnieje niezbadana cecha

        while (expo>=2 and (matchSize > (vertNo-expo)/2 or matchSize == -1) and isLabel)
        {
            //nadajemy wszystkim wierzcholkom wolnym ceche S: S
            for (typename GraphType::PVertex U=g.getVert(); U; U=g.getVertNext(U))
                if (vertTab[U].vMatch==0) vertTab[U].vLabS = U; //U ma ceche S:U, U jest wolny i jest rootem

            bool noAugment=true; //nie znaleziono drogi powiekszajacej

            do
            {
                isLabel = false;
                //bierzemy pierwsza z brzegu niezbadana ceche i ja badamy
                typename GraphType::PVertex vCurr;
                int expo_new;
                for (typename GraphType::PVertex U=g.getVert(); U; U=g.getVertNext(U))
                {
                    if (vertTab[U].vLabS!=0 and !vertTab[U].bS)
                    {
                        vCurr = U;
                        expo_new = labeling (g, vertTab, vertTabNeights, true, vCurr, expo);
                        isLabel = true;
                        break;
                    }
                    else if (vertTab[U].vLabT[0]!=0 and !vertTab[U].bT)
                    {
                        vCurr = U;
                        expo_new = labeling (g, vertTab, vertTabNeights, false, vCurr, expo);
                        isLabel = true;
                        break;
                    }
                }
                if (expo > expo_new) {expo = expo_new; noAugment = false; clearLabels (g, vertTab);}

            }while (isLabel and noAugment);
        }
        return (vertNo-expo)/2;
    }//end edmonsAlg

    //wpisanie wyniku do edgeIterOut dla maxMatching and minEdgeCover
    template <class GraphType, class VertContainer, class EIterOut>
    static void
        printResultToIterators(
            const GraphType & g,
            VertContainer & vertTab,
            EIterOut edgeIterOut,
            bool ifEdgeCover = false) //jezeli ifEdgeCover ustawiony (true) to rozszerzemy skojarzenie do pokrycia krawedziowego
    {
        typename GraphType::PVertex V, LOCALARRAY(tabV,g.getVertNo());
        typename GraphType::PEdge E;
        typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                typename GraphType::PVertex> ::Type tabVTmp(g.getVertNo()); //pomocnicze, by do wynikow nie doataly sie duplikaty krawedzi

        if (!isBlackHole(edgeIterOut)) //na wyjsciu ma byc strumien krawedzi
        {
            for (typename GraphType::PVertex U = g.getVert(); U; U=g.getVertNext(U))
            {
                if (vertTab[U].vMatch != 0 and tabVTmp[U] != U)
                {
                    V = vertTab[U].vMatch;
                    *edgeIterOut = g.getEdge(U,V,Koala::EdUndir|Koala::EdDirIn|Koala::EdDirOut); ++edgeIterOut;
                    tabVTmp[V] = V; //oznaczenie wierzcholka V, ze krawedz z nim incydentna zostala dodana do strumienia wyjsciowego
                }
                else if(ifEdgeCover and vertTab[U].vMatch == 0)
                {
                    E = g.getEdge (U,Koala::EdUndir|Koala::EdDirIn|Koala::EdDirOut);
                    *edgeIterOut = E; ++edgeIterOut;
                }
            }
        }
    }//end printMatchResultsToIterators

    public:

    // wlasciwa procedura - szuka skojarzenia o zadanym rozmiarze matchSize, zwraca znaleziony rozmiar skojarzenia
    template <class GraphType, class VertContainer, class EIterOut>
    static int
        find (
            const GraphType & g,
            VertContainer & avertTab,
            EIterOut edgeIterOut, //iterator wyjsciowy, na ktory wyrzucamy krawedzie matchingu
            int matchSize = -1) //Do ilu krawedzi wlacznie szukac. Tj. ilo-krawedziowy matching znalezc lub najwiekszy, gdy jest mniejszy od tego parametru. Gdy matchSize=-1 to znajdz najwiekszy.
        {
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type localvertTab;
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::Type &
                        vertTab=
                    BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::get(avertTab,localvertTab);

            if (isBlackHole(avertTab)) vertTab.reserve(g.getVertNo());

            typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                Set<typename GraphType::PVertex> >::Type    vertTabNeights(g.getVertNo());
//            std::map <typename GraphType::PVertex, Set<typename GraphType::PVertex> > vertTabNeights; //mapa przypisujaca wierzcholkowi - zbior jego sasiadow
            fillVertTabNeights (g,vertTabNeights);

            int expo = firstMatching(g,vertTab,vertTabNeights,matchSize); //funkcja tworzy skojarzenie poczatkowe, zwraca liczbe wierzcholkow wolnych w grafie
            matchSize = edmonsAlg(g,vertTab,vertTabNeights,matchSize,expo); //Algorytm Edmondsa znajduje skojarzenie o zadanym rozmiarze matchSize, zwraca znaleziony rozmiar skojarzenia
            printResultToIterators(g,vertTab,edgeIterOut); //wpisanie wynikow do edgeIterOut
            return matchSize; //zwracamy rozmiar matchingu
        }//end find

    // wlasciwa procedura - szuka skojarzenia o zadanym rozmiarze matchSize, zaczynajac od podanego skojarzenia poczatkowego, zwraca znaleziony rozmiar skojarzenia
    template <class GraphType, class VertContainer, class EIterIn, class EIterOut>
    static int
        find (
            const GraphType & g,
            VertContainer & avertTab,
            EIterIn vertIterInBegin, //iterator wejsciowy, zawiera poczatek strumienia z wierzcholkami tworzacymi poczatkowy matching
            EIterIn vertIterInEnd,   //iterator wejsciowy, zawiera koniec strumienia z wierzcholkami tworzacymi poczatkowy matching
            EIterOut edgeIterOut, //iterator wyjsciowy, na ktory wyrzucamy krawedzie matchingu
            int matchSize = -1) //Do ilu krawedzi wlacznie szukac. Tj. ilo-krawedziowy matching znalezc lub najwiekszy, gdy jest mniejszy od tego parametru. Gdy matchSize=-1 to znajdz najwiekszy.
        {
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type localvertTab;
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::Type &
                        vertTab=
                    BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::get(avertTab,localvertTab);

            if (isBlackHole(avertTab)) vertTab.reserve(g.getVertNo());

            typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                Set<typename GraphType::PVertex> >::Type    vertTabNeights(g.getVertNo());
//            std::map <typename GraphType::PVertex, Set<typename GraphType::PVertex> > vertTabNeights; //mapa przypisujaca wierzcholkowi - zbior jego sasiadow
            fillVertTabNeights (g,vertTabNeights);

            int expo = firstMaching(g, vertTab, vertIterInBegin, vertIterInEnd);//funkcja tworzy skojarzenie poczatkowe, na podstawie otrzymanego strumienia wierzcholkow
            matchSize = edmonsAlg(g,vertTab,vertTabNeights,matchSize,expo); //Algorytm Edmondsa znajduje skojarzenie o zadanym rozmiarze matchSize, zwraca znaleziony rozmiar skojarzenia
            printResultToIterators(g,vertTab,edgeIterOut); //wpisanie wynikow do edgeIterOut
            return matchSize; //zwracamy rozmiar matchingu
        }//end find

    // wlasciwa procedura - szuka minimalnego pokrycia krawedziowego
    // najpierw realizuje Edmondsa - szuka maksymalnego skojarzenia, nastepnie rozszerza je do pokrycia krawedziowego
    template <class GraphType, class VertContainer, class EIterOut>
    static int
        minEdgeCover (
            const GraphType & g,
            VertContainer & avertTab,
            EIterOut edgeIterOut) //iterator wyjsciowy, na ktory wyrzucamy krawedzie nalezace do pokrycia krawedziowego
        {
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type localvertTab;
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::Type &
                        vertTab=
                    BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::get(avertTab,localvertTab);

            if (isBlackHole(avertTab)) vertTab.reserve(g.getVertNo());
            typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                Set<typename GraphType::PVertex> >::Type    vertTabNeights(g.getVertNo());
//            std::map <typename GraphType::PVertex, Set<typename GraphType::PVertex> > vertTabNeights; //mapa przypisujaca wierzcholkowi - zbior jego sasiadow
            fillVertTabNeights (g,vertTabNeights);

            int expo = firstMatching(g,vertTab,vertTabNeights,-1); //funkcja tworzy skojarzenie poczatkowe, zwraca liczbe wierzcholkow wolnych w grafie
            int edgeCoverSize = edmonsAlg(g,vertTab,vertTabNeights,-1,expo) + expo; //Algorytm Edmondsa znajduje skojarzenie o maksymalnym rozmiarze, zwraca znaleziony rozmiar skojarzenia
            printResultToIterators(g,vertTab,edgeIterOut,true); //wpisanie wynikow do edgeIterOut
            return edgeCoverSize; //zwracamy rozmiar matchingu
        }//end minEdgeCover

    // wlasciwa procedura - zachlannie szuka skojarzenia
    template <class GraphType, class VertContainer, class EIterOut>
    static int
        greedy (
            const GraphType & g,
            VertContainer & avertTab,
            EIterOut edgeIterOut, //iterator wyjsciowy, na ktory wyrzucamy krawedzie matchingu
            int matchSize = -1)
        {
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type localvertTab;
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::Type &
                        vertTab=
                    BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::get(avertTab,localvertTab);

            if (isBlackHole(avertTab)) vertTab.reserve(g.getVertNo());

            int vertNo = g.getVertNo();        // liczba wierzcholkow w grafie
            int expo = vertNo;                 // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
            typename GraphType::PVertex U,V;

            for (typename GraphType::PEdge E=g.getEdge(EdUndir|EdDirIn|EdDirOut);E;E=g.getEdgeNext(E,EdUndir|EdDirIn|EdDirOut))  //przegladamy krawedzie grafu
            {
                if (matchSize == (vertNo-expo)/2) break; //jezeli osiagniemy zadany rozmiar skojarzenia - koniec
                U = g.getEdgeEnd1(E); V = g.getEdgeEnd2(E); //E = (U,V)
                if (vertTab[U].vMatch == 0 and vertTab[V].vMatch == 0) //jezeli U i V sa wolne tzn. ze mozna E dodac do skojarzenia
                {
                    vertTab[U].vMatch = V;
                    vertTab[V].vMatch = U;
                    *edgeIterOut = E; ++edgeIterOut;
                    expo -= 2;
                }
            }
            return ((vertNo - expo) / 2); //zwracamy rozmiar matchingu
        }

    // wlasciwa procedura - zachlannie szuka skojarzenia, rozwarzajac krawedzie podane na wejsciu
    template <class GraphType, class VertContainer, class EIterIn, class EIterOut>
    static int
        greedy (
            const GraphType & g,
            VertContainer & avertTab,
            EIterIn edgeIterInBegin, //iterator wejsciowy, zawiera poczatek strumienia z wierzcholkami, ktore algorytm na rozwazyc
            EIterIn edgeIterInEnd, //iterator wejsciowy, zawiera koniec strumienia z wierzcholkami, ktore algorytm na rozwazyc
            EIterOut edgeIterOut, //iterator wyjsciowy, na ktory wyrzucamy krawedzie matchingu
            int matchSize = -1)
        {
            typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type localvertTab;
            typename BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::Type &
                        vertTab=
                    BlackHoleSwitch<VertContainer,typename DefaultStructs::template AssocCont<typename GraphType::PVertex,
                    VertLabs<GraphType> >::Type >::get(avertTab,localvertTab);

            if (isBlackHole(avertTab)) vertTab.reserve(g.getVertNo());

            int vertNo = g.getVertNo();        // liczba wierzcholkow w grafie
            int expo = vertNo;                 // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
            typename GraphType::PVertex U,V;

            for (EIterIn itE = edgeIterInBegin; itE!=edgeIterInEnd; ++itE)  //przeglasamy podane krawedzie
            {
                if (matchSize == (vertNo-expo)/2) break; //jezeli osiagniemy zadany rozmiar skojarzenia - koniec
                U = g.getEdgeEnd1(*itE); V = g.getEdgeEnd2(*itE); //E = (U,V)
                if (vertTab[U].vMatch == 0 and vertTab[V].vMatch == 0) //jezeli U i V sa wolne tzn. ze mozna E dodac do skojarzenia
                {
                    vertTab[U].vMatch = V;
                    vertTab[V].vMatch = U;
                    *edgeIterOut = *itE; ++edgeIterOut;
                    expo -= 2;
                }
            }
            return ((vertNo - expo) / 2); //zwracamy rozmiar matchingu
        }

    // wlasciwa procedura - testuje czy podane na wejsciu krawedzie tworza skojarzenie
    template <class GraphType, class EIterIn>
    static bool
        test (
            const GraphType & g,
            EIterIn edgeIterInBegin, //iterator wejsciowy, zawiera poczatek strumienia z krawedziemi, ktore maja byc przetestowane
            EIterIn edgeIterInEnd) //iterator wejsciowy, zawiera koniec strumienia z krawedziemi, ktore maja byc przetestowane
        {
            int vertNo = g.getVertNo();        // liczba wierzcholkow w grafie
            int expo = vertNo;                 // expo zawierac bedzie liczba wierzcholkow wolnych w grafie
            typename GraphType::PVertex U,V;
            typename DefaultStructs:: template AssocCont<typename GraphType::PVertex,
                bool >::Type vertTabMatch(g.getVertNo()); //jezeli true - wierzcholek nalezy do matchingu

            for (EIterIn itE = edgeIterInBegin; itE!=edgeIterInEnd; ++itE)  //przeglasamy podane krawedzie
            {
                U = g.getEdgeEnd1(*itE); V = g.getEdgeEnd2(*itE); //E = (U,V)
                if (vertTabMatch.hasKey(U) or vertTabMatch.hasKey(V)) return false; //jezeli ktorykolwiek z wierzcholkow tworzacych krawedz nie jest wolny to nie mamy matchingu
                vertTabMatch[U]=vertTabMatch[V] = true;
            }
            return true;
        }
};

class Matching : public MatchingPar<AlgsDefaultSettings> {};

}

#endif

