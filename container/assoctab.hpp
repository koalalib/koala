// ----------------------------------------------------------------------------
// AssocTabInterface< M >


template< class K, class V >
bool AssocTabConstInterface<std::map< K,V > >::delKey( K arg )
{
    typename std::map< K,V >::iterator pos = _cont().find( arg );
    if (pos == _cont().end()) return false;
    _cont().erase( pos );
    return true;
}


template< class K, class V >
K AssocTabConstInterface<std::map< K,V > >::firstKey() const
{
    if (cont.begin() == cont.end()) return (K)0;
    return cont.begin()->first;
}

template< class K, class V >
K AssocTabConstInterface<std::map< K,V > >::lastKey() const
{
    typename std::map< K,V >::const_iterator pos;
    if (cont.begin() == (pos = cont.end())) return (K)0;
    pos--;
    return pos->first;
}

template< class K, class V >
K AssocTabConstInterface<std::map< K,V > >::prevKey( K arg ) const
{
    if (!arg) return lastKey();
    typename std::map< K,V >::const_iterator pos = cont.find( arg );
    koalaAssert( pos != cont.end(),ContExcOutpass );
    if (pos == cont.begin()) return (K)0;
    pos--;
    return pos->first;
}

template< class K, class V >
K AssocTabConstInterface<std::map< K,V > >::nextKey( K arg ) const
{
    if (!arg) return firstKey();
    typename std::map< K,V >::const_iterator pos = cont.find( arg );
    koalaAssert( pos != cont.end(),ContExcOutpass );
    pos++;
    if (pos == cont.end()) return (K)0;
    return pos->first;
}

template< class K, class V > template <class Iterator>
int AssocTabConstInterface<std::map< K,V > >::getKeys( Iterator iter ) const
{
    for( K key = firstKey(); key; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}





// ----------------------------------------------------------------------------
// AssocTable< T >

template< class T >
AssocTable< T > &AssocTable< T >::operator=( const AssocTable< T > &X )
{
    if (this == &X) return *this;
    cont = X.cont;
    return *this;
}

template< class T >
AssocTable< T > &AssocTable< T >::operator=( const T &X )
{
    if (&cont == &X) return *this;
    cont = X;
    return *this;
}



AssocKeyContReg &AssocKeyContReg::operator=( const AssocKeyContReg &X )
{
    if (&X != this) next = 0;
    return *this;
}

AssocContReg *AssocKeyContReg::find( AssocContBase *cont )
{
    AssocContReg *res;
    for( res = this; res->next; res= &(res->next->getReg( res->nextPos )) )
        if (res->next == cont) return res;
    return NULL;
}

void AssocKeyContReg::deregister()
{
    std::pair< AssocContBase *,int >
        a = std::pair< AssocContBase *,int >( next,nextPos ), n;
    next = 0;
    while (a.first)
    {
        AssocContReg *p = &a.first->getReg( a.second );
        n = std::pair< AssocContBase *,int >( p->next,p->nextPos );
        a.first->DelPosCommand( a.second );
        a = n;
    }
}

template< class Klucz, class Elem, class Container >
AssocArray< Klucz,Elem,Container >::AssocArray(
    const AssocArray< Klucz,Elem,Container > &X ):
    tab(X.tab)
{
    for( int i = tab.firstPos(); i != -1; i = tab.nextPos( i ) )
    {
        tab[i].assocReg = tab[i].key->assocReg;
        tab[i].key->assocReg.next = this;
        tab[i].key->assocReg.nextPos = i;
    }
}

template< class Klucz, class Elem, class Container >
AssocArray< Klucz,Elem,Container > &
AssocArray< Klucz,Elem,Container >::operator=(
    const AssocArray< Klucz,Elem,Container > &X )
{
    if (&X == this) return *this;
    clear();
    tab = X.tab;
    for( int i = tab.firstPos(); i != -1; i = tab.nextPos( i ) )
    {
        tab[i].assocReg = tab[i].key->assocReg;
        tab[i].key->assocReg.next = this;
        tab[i].key->assocReg.nextPos = i;
    }
    return *this;
}

template< class Klucz, class Elem, class Container >
int AssocArray< Klucz,Elem,Container >::keyPos( Klucz v )  const
{
    if (!v) return -1;
    AssocContReg *preg = v->assocReg.find( const_cast<AssocArray< Klucz,Elem,Container >*> (this) );
    if (preg) return preg->nextPos;
    else return -1;
}

template< class Klucz, class Elem, class Container >
bool AssocArray< Klucz,Elem,Container >::delKey( Klucz v )
{
    int x;
    if (!v) return false;
    AssocContReg *preg = v->assocReg.find( this );
    if (!preg) return false;
    x = preg->nextPos;
    *preg = tab[x].assocReg;
    tab.delPos( x );
    return true;
}

template< class Klucz, class Elem, class Container >
Klucz AssocArray< Klucz,Elem,Container >::firstKey()  const
{
    if (tab.empty()) return 0;
    else return tab[tab.firstPos()].key;
}

template< class Klucz, class Elem, class Container >
Klucz AssocArray< Klucz,Elem,Container >::lastKey()  const
{
    if (tab.empty()) return 0;
    else return tab[tab.lastPos()].key;
}

template< class Klucz, class Elem, class Container >
Klucz AssocArray< Klucz,Elem,Container >::nextKey( Klucz v )  const
{
    if (!v) return firstKey();
    int x= keyPos( v ); //TODO: poprawiony blad, uaktualnic
    koalaAssert(x != -1,ContExcOutpass);
    if ((x = tab.nextPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class Container >
Klucz AssocArray< Klucz,Elem,Container >::prevKey( Klucz v )  const
{
    if (!v) return lastKey();
    int x= keyPos( v );//TODO: poprawiony blad, uaktualnic
    koalaAssert(x  != -1,ContExcOutpass);
    if ((x = tab.prevPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class Container >
Elem &AssocArray< Klucz,Elem,Container >::operator[]( Klucz v )
{
    koalaAssert( v ,ContExcWrongArg);
    int x = keyPos( v );
    if (x == -1)
    {
        x = tab.newPos();
        tab[x].key = v;
        tab[x].assocReg = v->assocReg;
        v->assocReg.next = this;
        v->assocReg.nextPos = x;
    }
    return tab[x].val;
}

template< class Klucz, class Elem, class Container >
Elem AssocArray< Klucz,Elem,Container >::operator[]( Klucz v ) const
{
    koalaAssert( v,ContExcWrongArg );
    int x = keyPos( v );
    if (x == -1) return Elem();
    return tab[x].val;
}

template< class Klucz, class Elem, class Container >
void AssocArray< Klucz,Elem,Container >::defrag()
{
    tab.defrag();
    for( int i = 0; i < tab.size(); i++ )
        tab[i].key->assocReg.find( this )->nextPos = i;
}

template< class Klucz, class Elem, class Container >
void AssocArray< Klucz,Elem,Container >::clear()
{
    for( Klucz v = firstKey(); v; v = firstKey() ) delKey( v );
}

template< class Klucz, class Elem, class Container > template< class Iterator >
int AssocArray< Klucz,Elem,Container >::getKeys( Iterator iter )  const
{
    for( Klucz key = firstKey(); key; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}


template< class Klucz, class Elem, class AssocCont,class Container >
int PseudoAssocArray< Klucz,Elem,AssocCont,Container >::keyPos( Klucz v ) const
{
    if (!v) return -1;
    if (!assocTab.hasKey(v)) return -1;
    return assocTab[v];
}

template< class Klucz, class Elem, class AssocCont,class Container >
bool PseudoAssocArray< Klucz,Elem,AssocCont,Container >::delKey( Klucz v )
{
    if (!v) return false;
    if (!assocTab.hasKey(v)) return false;
    tab.delPos( assocTab[v] );
    assocTab.delKey(v);
    return true;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::firstKey()  const
{
    if (tab.empty()) return 0;
    else return tab[tab.firstPos()].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::lastKey()  const
{
    if (tab.empty()) return 0;
    else return tab[tab.lastPos()].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::nextKey( Klucz v )  const
{
    if (!v) return firstKey();
    int x= keyPos( v ); // TODO: poprawiony blad, uaktualnic
    koalaAssert(x  != -1,ContExcOutpass);
    if ((x = tab.nextPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::prevKey( Klucz v )  const
{
    if (!v) return lastKey();
    int x= keyPos( v ); // TODO: poprawiony blad, uaktualnic
    koalaAssert(x  != -1,ContExcOutpass);
    if ((x = tab.prevPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Elem &PseudoAssocArray< Klucz,Elem,AssocCont,Container >::operator[]( Klucz v )
{
    koalaAssert( v ,ContExcWrongArg);
    int x = keyPos( v );
    if (x == -1)
    {
        tab[x = tab.newPos()].key = v;
        assocTab[v]=x;
    }
    return tab[x].val;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Elem PseudoAssocArray< Klucz,Elem,AssocCont,Container >::operator[]( Klucz v ) const
{
    koalaAssert( v ,ContExcOutpass);
    int x = keyPos( v );
    if (x == -1) return Elem();
    return tab[x].val;
}

template< class Klucz, class Elem, class AssocCont,class Container >
void PseudoAssocArray< Klucz,Elem,AssocCont,Container >::defrag()
{
    tab.defrag();
    for( int i = 0; i < tab.size(); i++ ) assocTab[tab[i].key] = i;
}


template< class Klucz, class Elem, class AssocCont, class Container > template< class Iterator >
int PseudoAssocArray< Klucz,Elem,AssocCont,Container >::getKeys( Iterator iter )  const
{
    for( Klucz key = firstKey(); key; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}


inline int AssocMatrixAddr< AMatrFull >::wsp2pos( std::pair< int,int > w )  const
{
    int mfs = std::max( w.first,w.second );
    return mfs * mfs + mfs + w.second - w.first;
}

inline std::pair< int,int > AssocMatrixAddr< AMatrFull >::pos2wsp( int pos ) const
{
    int x = (int)sqrt( (double)pos );
    if (x * x + x - pos > 0) return std::pair< int,int >( x,pos - x * x );
    else return std::pair< int,int >( x * x + 2 * x - pos,x );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrFull >::key( Klucz u, Klucz v ) const
{
    return std::pair< Klucz,Klucz >( u,v );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrFull >::key( std::pair< Klucz,Klucz> k ) const
{
    return k;
}

inline int AssocMatrixAddr< AMatrNoDiag >::wsp2pos( std::pair< int,int > w ) const
{
    int mfs = std::max( w.first,w.second );
    return mfs * mfs + w.second - w.first - ((w.first > w.second) ? 0 : 1);
}

inline std::pair< int,int > AssocMatrixAddr< AMatrNoDiag >::pos2wsp( int pos ) const
{
    int x = (int)sqrt( (double)pos );
    if (pos - x * x - x >= 0) return std::pair< int,int >( x + 1,pos - x * x - x );
    else return std::pair< int,int >( x * x + x - 1 - pos,x );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrNoDiag >::key( Klucz u, Klucz v ) const
{
    return std::pair< Klucz,Klucz >( u,v );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrNoDiag >::key( std::pair< Klucz,Klucz > k ) const
{
    return k;
}

inline int AssocMatrixAddr< AMatrClTriangle >::wsp2pos( std::pair< int,int > w ) const
{
    if (w.first < w.second)
    {
        int z = w.first;
        w.first = w.second;
        w.second = z;
    }
    return w.first * (w.first + 1) / 2 + w.second;
}

inline std::pair< int,int > AssocMatrixAddr< AMatrClTriangle >::pos2wsp( int pos ) const
{
    int x = (int)sqrt( (double)2 * pos ), xx = pos - x * (x + 1) / 2;
    if (xx >= 0) return std::pair< int,int >( x,xx );
    else return std::pair< int,int >( x - 1,xx + x );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrClTriangle >::key( Klucz u, Klucz v ) const
{
    return (pairMinMax( u,v ) );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrClTriangle >::key(
    std::pair< Klucz,Klucz > k ) const
{
    return pairMinMax( k.first,k.second );
}

inline int AssocMatrixAddr< AMatrTriangle >::wsp2pos( std::pair< int,int > w ) const
{
    if (w.first < w.second)
    {
        int z = w.first;
        w.first = w.second;
        w.second = z;
    }
    return w.first * (w.first - 1) / 2 + w.second;
}

inline std::pair< int,int > AssocMatrixAddr< AMatrTriangle >::pos2wsp( int pos ) const
{
    int x = (int)sqrt( (double)2 * pos ), xx = pos - x * (x + 1) / 2;
    if (xx >= 0) return std::pair< int,int >( x + 1,xx );
    else return std::pair< int,int >( x,xx + x );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrTriangle >::key( Klucz u, Klucz v ) const
{
    return (pairMinMax( u,v ) );
}

template< class Klucz >
inline std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrTriangle >::key(
    std::pair< Klucz,Klucz > k ) const
{
    return pairMinMax( k.first,k.second );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocIndex::AssocIndex(
    int asize ):
    IndexContainer( asize )
{
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocIndex::AssocIndex(
    int asize,typename AssocMatrixInternalTypes<Klucz,Elem>::IndexBlockType* indbuf):
    IndexContainer( asize,indbuf )
{
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
int AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocIndex::klucz2pos(
    Klucz v )
{
    if (!v) return -1;
    return IndexContainer::keyPos( v );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
Klucz
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocIndex::pos2klucz(
    int arg )
{
    if (arg == -1) return 0;
    return IndexContainer::tab[arg].key;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
void
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocIndex::DelPosCommand(
    int pos )
{
    int LOCALARRAY( tabpos,size() );
    int l = 0;
    int i = IndexContainer::tab.firstPos();
    for( ; i != -1; i = IndexContainer::tab.nextPos( i ) )
        tabpos[l++] = i;
    for( l--; l >= 0; l-- )
    {
        owner->delPos( std::pair< int,int >( pos,tabpos[l] ) );
        if ((aType == AMatrNoDiag || aType == AMatrFull) && (pos != tabpos[l]))
            owner->delPos( std::pair< int,int >( tabpos[l],pos ) );
    }
    IndexContainer::tab.delPos( pos );
    Klucz k,n;
    for( k = IndexContainer ::firstKey(); k; k = n )
    {
        n = IndexContainer::nextKey( k );
        if (!this->operator[]( k )) IndexContainer::delKey( k );
    }
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
void AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::delPos(
    std::pair< int,int > wsp )
{
    if (!AssocMatrixAddr< aType >::correctPos( wsp.first,wsp.second )) return;
    int x;
    if (!bufor[x = AssocMatrixAddr< aType >::wsp2pos( wsp )].present()) return;
    if (bufor[x].next != -1) bufor[bufor[x].next].prev = bufor[x].prev;
    else last = bufor[x].prev;
    if (bufor[x].prev != -1) bufor[bufor[x].prev].next = bufor[x].next;
    else first = bufor[x].next;
    bufor[x] = BlockOfAssocMatrix< Elem >();
    siz--;
    --index.tab[wsp.first].val;
    --index.tab[wsp.second].val;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocMatrix( int asize,void* p, void* q ):
    index( asize ),
    siz( 0 ),
    first( -1 ),
    last( -1 )
{
    bufor.clear();
    bufor.reserve( AssocMatrixAddr< aType >::bufLen( asize ) );
    index.owner = this;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocMatrix(
    const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > &X ):
    index( X.index ),
    bufor( X.bufor ),
    siz( X.siz ),
    first( X.first ),
    last( X.last )
{
    index.owner = this;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
AssocMatrix<Klucz,Elem,aType,Container,IndexContainer> &
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::operator=(
    const AssocMatrix< Klucz,Elem,aType,Container,IndexContainer > &X )
{
    if (&X == this) return *this;
    index = X.index;
    bufor = X.bufor;
    siz = X.siz;
    first = X.first;
    last = X.last;
    index.owner = this;
    return *this;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
bool AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::delInd( Klucz v )
{
    if (!hasInd( v )) return false;
    Klucz LOCALARRAY(tab,index.size());
    int i = 0;
    for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) ) tab[i++] = x;
    for( i--; i >= 0; i-- )
    {
        delKey( v,tab[i] );
        if ((aType == AMatrNoDiag || aType == AMatrFull) && (v != tab[i]))
            delKey( tab[i],v );
    }
    index.delKey( v );
    return true;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
template <class Elem2, class ExtCont>
int AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::slice1(Klucz v, ExtCont & tab ) const
{
    if (!index.hasKey( v )) return 0;
    int licz = 0;
    for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) )
        if (hasKey( v,x ))
        {
            tab[x] = this->operator()( v,x );
            licz++;
        }
    return licz;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
template <class Elem2, class ExtCont>
int AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::slice2(Klucz v, ExtCont &tab ) const
{
    if (!index.hasKey( v )) return 0;
    int licz = 0;
    for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) )
        if (hasKey( x,v ))
        {
            tab[x] = this->operator()( x,v );
            licz++;
        }
    return licz;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
bool AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::hasKey( Klucz u, Klucz v ) const
{
    if (!u || !v) return false;
    if (!AssocMatrixAddr< aType >::correctPos( u,v )) return false;
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    if (wsp.first == -1 || wsp.second == -1) return false;
    return bufor[AssocMatrixAddr< aType >::wsp2pos( wsp )].present();
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
bool AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::delKey( Klucz u, Klucz v )
{
    if (!u || !v) return false;
    if (!AssocMatrixAddr< aType >::correctPos( u,v )) return false;
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    if (wsp.first == -1 || wsp.second == -1) return false;
    int x;
    if  (bufor[x = AssocMatrixAddr< aType >::wsp2pos( wsp )].present())
    {
        if (bufor[x].next != -1) bufor[bufor[x].next].prev = bufor[x].prev;
        else last = bufor[x].prev;
        if (bufor[x].prev != -1) bufor[bufor[x].prev].next = bufor[x].next;
        else first = bufor[x].next;
        bufor[x] = BlockOfAssocMatrix< Elem >();
        siz--;
        if (--index[u] == 0) index.delKey( u );
        if (--index[v] == 0) index.delKey( v );
        return true;
    }
    return false;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
Elem &AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::operator()(
    Klucz u, Klucz v )
{
    koalaAssert( u && v && AssocMatrixAddr< aType >::correctPos( u,v ),ContExcWrongArg );
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    if (wsp.first == -1)
    {
        index[u] = 0;
        wsp.first = index.klucz2pos( u );
    }
    if (wsp.second == -1)
    {
        index[v] = 0;
        wsp.second = index.klucz2pos( v );
    }
    bufor.resize( std::max( (int)bufor.size(),AssocMatrixAddr< aType >::bufLen( index.size() ) ) );
    int x = AssocMatrixAddr< aType >::wsp2pos( wsp );
    if (!bufor[x].present())
    {
        if ((bufor[x].prev = last) == -1) first = x;
        else bufor[bufor[x].prev].next = x;
        bufor[x].next = -1;
        last = x;
        index[u]++;
        index[v]++;
        siz++;
    }
    return bufor[x].val;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
Elem AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::operator()(
    Klucz u, Klucz v ) const
{
    koalaAssert( u && v && AssocMatrixAddr< aType >::correctPos( u,v ),ContExcWrongArg );
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    if (wsp.first == -1 || wsp.second == -1) return Elem();
    int x = AssocMatrixAddr< aType >::wsp2pos( wsp );
    if (!bufor[x].present()) return Elem();
    return bufor[x].val;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
Elem* AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::valPtr(
    Klucz u, Klucz v )
{
    koalaAssert( u && v && AssocMatrixAddr< aType >::correctPos( u,v ) ,ContExcWrongArg);
    std::pair< int,int > wsp =
        std::pair<int,int>( index.klucz2pos( u ),index.klucz2pos( v ) );
    if (wsp.first == -1 || wsp.second == -1) return NULL;
    int pos;
    if (!bufor[pos = AssocMatrixAddr< aType >::wsp2pos( wsp )].present()) return NULL;
    return &bufor[pos].val;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::firstKey() const
{
    if (!siz) return std::pair< Klucz,Klucz >( (Klucz)0,(Klucz)0 );
    std::pair< int,int > wsp = AssocMatrixAddr< aType >::pos2wsp( first );
    return AssocMatrixAddr< aType >::key(
        std::pair< Klucz,Klucz >( index.pos2klucz( wsp.first ),
                                  index.pos2klucz( wsp.second ) ) );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::lastKey() const
{
    if (!siz) return std::pair< Klucz,Klucz >( (Klucz)0,(Klucz)0 );
    std::pair< int,int > wsp = AssocMatrixAddr< aType >::pos2wsp( last );
    return AssocMatrixAddr< aType >::key(
        std::pair< Klucz,Klucz >( index.pos2klucz( wsp.first ),
                                  index.pos2klucz( wsp.second ) ) );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::nextKey( Klucz u, Klucz v ) const
{
    if (!u || !v) return firstKey();
    koalaAssert( AssocMatrixAddr< aType >::correctPos( u,v ),ContExcWrongArg);
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    koalaAssert( wsp.first != -1 && wsp.second != -1 ,ContExcOutpass);
    int x = AssocMatrixAddr< aType >::wsp2pos( wsp );
    koalaAssert( bufor[x].present() ,ContExcOutpass);
    x = bufor[x].next;
    if (x == -1) return std::pair< Klucz,Klucz >( (Klucz)0,(Klucz)0 );
    wsp = AssocMatrixAddr< aType >::pos2wsp( x );
    return AssocMatrixAddr< aType >::key(
        std::pair< Klucz,Klucz >( index.pos2klucz( wsp.first ),
                                  index.pos2klucz( wsp.second ) ) );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::nextKey(
    std::pair< Klucz,Klucz > k )  const
{
    return nextKey( k.first,k.second );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::prevKey( Klucz u, Klucz v )  const
{
    if (!u || !v) return lastKey();
    koalaAssert( AssocMatrixAddr< aType >::correctPos( u,v ),ContExcWrongArg);
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    koalaAssert( wsp.first != -1 && wsp.second != -1,ContExcOutpass);
    int x = AssocMatrixAddr< aType >::wsp2pos( wsp );
    koalaAssert( bufor[x].present() ,ContExcOutpass);
    x = bufor[x].prev;
    if (x == -1) return std::pair< Klucz,Klucz >( (Klucz)0,(Klucz)0 );
    wsp = AssocMatrixAddr< aType >::pos2wsp( x );
    return AssocMatrixAddr< aType >::key(
        std::pair< Klucz,Klucz >(index.pos2klucz( wsp.first ),
                                 index.pos2klucz( wsp.second ) ) );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::prevKey(
    std::pair< Klucz,Klucz > k )  const
{
    return prevKey( k.first,k.second );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
void AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::clear()
{
    index.clear();
    int in;
    for( int i = first; i != -1; i = in )
    {
        in = bufor[i].next;
        bufor[i] = BlockOfAssocMatrix< Elem >();
    }
    siz = 0;
    first = last = -1;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
void AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::reserve( int arg )
{
    index.reserve( arg );
    bufor.reserve( AssocMatrixAddr< aType >::bufLen( arg ) );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
void AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::defrag()
{
    DefragMatrixPom LOCALARRAY( tab,siz );
    int i=0;
    for( int pos = first; pos != -1; pos = bufor[pos].next )
    {
        tab[i].val = bufor[pos].val;
        std::pair< int,int > wsp = AssocMatrixAddr< aType >::pos2wsp( pos );
        tab[i].u = index.pos2klucz( wsp.first );
        tab[i].v = index.pos2klucz( wsp.second );
        i++;
    }
    bufor.clear();
    index.clear();
    index.defrag();
    siz = 0;
    first = last = -1;
    for( int ii = 0; ii < i ; ii++ ) this->operator()( tab[ii].u,tab[ii].v ) = tab[ii].val;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
template <class Iterator>
int AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::getKeys( Iterator iter )  const
{
    for( std::pair< Klucz,Klucz > key = firstKey(); key.first; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}

template<class Klucz, class Elem, AssocMatrixType aType, class C,class IC >
std::ostream &operator<<(std::ostream &out,const AssocMatrix< Klucz,Elem,aType,C,IC > & cont )
{
    out << '{';
    int siz = cont.size();
    std::pair<typename AssocMatrix< Klucz,Elem,aType,C,IC >::KeyType,typename AssocMatrix< Klucz,Elem,aType,C,IC >::KeyType>
        key = cont.firstKey();
    for( ; siz; siz-- )
    {
        out << '(' << key.first << ',' << key.second << ':'<< cont(key) << ')';
        if (siz>1)
        {
            key = cont.nextKey( key );
            out << ',';
        }
    }
    out << '}';
    return out;
}


template< class T > template< class K, class V >
AssocInserter< T > &AssocInserter< T >::operator=( const std::pair< K,V > &pair )
{
    (*container)[(typename T::KeyType)pair.first] =
        (typename T::ValType)pair.second;
    return *this;
}

template< class T, class Fun > template< class K >
AssocFunktorInserter< T,Fun > &AssocFunktorInserter< T,Fun >::operator=( const K &arg )
{
    (*container)[(typename T::KeyType)arg] =
        (typename T::ValType)funktor( arg );
    return *this;
}

template< class T, class F >
AssocFunktorInserter< T,F > assocInserter( T &x, F f )
{
    return AssocFunktorInserter< T,F >( x,f );
}

