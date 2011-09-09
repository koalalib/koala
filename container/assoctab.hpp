// ----------------------------------------------------------------------------
// AssocTabInterface< M >

template< class K, class V >
bool AssocTabInterface< std::map< K,V > >::delKey( K arg )
{
    typename std::map< K,V >::iterator pos = cont.find( arg );
    if (pos == cont.end()) return false;
    cont.erase( pos );
    return true;
}

template< class K, class V >
K AssocTabInterface< std::map< K,V > >::firstKey()
{
    if (cont.begin() == cont.end()) return (K)0;
    return cont.begin()->first;
}

template< class K, class V >
K AssocTabInterface< std::map< K,V > >::lastKey()
{
    typename std::map< K,V >::iterator pos;
    if (cont.begin() == (pos = cont.end())) return (K)0;
    pos--;
    return pos->first;
}

template< class K, class V >
K AssocTabInterface< std::map< K,V > >::prevKey( K arg )
{
    if (!arg) return lastKey();
    typename std::map< K,V >::iterator pos = cont.find( arg );
    assert( pos != cont.end() );
    if (pos == cont.begin()) return (K)0;
    pos--;
    return pos->first;
}

template< class K, class V >
K AssocTabInterface< std::map< K,V > >::nextKey( K arg )
{
    if (!arg) return firstKey();
    typename std::map< K,V >::iterator pos = cont.find( arg );
    assert( pos != cont.end() );
    pos++;
    if (pos == cont.end()) return (K)0;
    return pos->first;
}

template< class K, class V > template <class Iterator>
int AssocTabInterface< std::map< K,V > >::getKeys( Iterator iter )
{
    for( K key = firstKey(); key; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}

template< class T > template< class Iterator >
int AssocTabInterface< AssocTabInterface< T > >::getKeys( Iterator iter )
{
    return cont.getKeys( iter );
}

template< class T >
AssocTabInterface< T > assocTabInterf( T &cont )
{
    return AssocTabInterface< T >( cont );
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

template< class T > template< class Iterator >
int AssocTable< T >::getKeys( Iterator iter )
{
    return inter.getKeys( iter );
}

template< class T >
AssocTable< T > assocTab( const T &cont )
{
    return AssocTable<T>( cont );
}

template< class T > template< class Iterator >
int AssocTabInterface< AssocTable< T > >::getKeys( Iterator iter )
{
    return cont.getKeys( iter );
}

template< class Element, class Container >
BlockList< Element,Container >::BlockList( int asize ):
    siz( 0 ),
    first( -1 ),
    last( -1 ),
    ffree( -1 )
{
    cont.clear();
    cont.reserve( asize );
}

template< class Element, class Container >
BlockList< Element,Container >::BlockList( const BlockList< Element,Container > &X ):
    siz( X.siz ),
    first( X.first ),
    last( X.last ),
    ffree( X.ffree ),
    cont( X.cont )
{
}

template< class Element, class Container >
BlockList< Element,Container > &
BlockList< Element,Container >::operator=( const BlockList< Element,Container > &X )
{
    if (&X == this) return *this;
    Privates::BlockListVectInerfTest<Container> error;
    siz = X.siz;
    first = X.first;
    last = X.last;
    ffree = X.ffree;
    cont = X.cont;
    return *this;
}

template< class Element, class Container >
bool BlockList< Element,Container >::ready( int pos )
{
    return pos >= 0 && pos < cont.size() && cont[pos].prev != -2;
}

template< class Element, class Container >
void BlockList< Element,Container >::clear()
{
    cont.clear();
    siz = 0;
    ffree = first = last = -1;
}

template< class Element, class Container >
int BlockList< Element,Container >::nextPos( int i )
{
    if (i == -1) return first;
    return cont[i].next;
}

template< class Element, class Container >
int BlockList< Element,Container >::prevPos( int i )
{
    if (i == -1) return last;
    return cont[i].prev;
}

template< class Element, class Container >
int BlockList< Element,Container >::newPos( int x )
{
    int pos;
    if (ffree != -1)
    {
        pos = ffree;
        ffree = cont[ffree].next;
    }
    else
    {
        cont.push_back( BlockOfBlockList<Element>() );
        pos = siz;
    }
    siz++;
    cont[pos].next = x;
    cont[pos].prev = (x == -1) ? last : cont[x].prev;
    if (x == -1) last = pos;
    else cont[x].prev = pos;
    if (x == first) first = pos;
    else cont[cont[pos].prev].next = pos;
    return pos;
}

template< class Element, class Container >
void BlockList< Element,Container >::delPos( int pos )
{
    if (pos == -1) return;
    if (cont[pos].next != -1) cont[cont[pos].next].prev = cont[pos].prev;
    else last = cont[pos].prev;
    if (cont[pos].prev != -1) cont[cont[pos].prev].next = cont[pos].next;
    else first = cont[pos].next;
    cont[pos].next = ffree;
    ffree = pos;
    siz--;
    cont[pos].blob = Element();
    cont[pos].prev = -2;
}

template< class Element, class Container >
void BlockList< Element,Container >::defrag()
{
    int asiz = 0;
    Element LOCALARRAY( buf,siz );
    for( int i = firstPos(); i != -1; i = nextPos( i ) ) buf[asiz++] = operator[]( i );
    clear();
    for( int i = 0; i < asiz; i++ ) operator[]( newPos() ) = buf[i];
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
int AssocArray< Klucz,Elem,Container >::keyPos( Klucz v )
{
    if (!v) return -1;
    AssocContReg *preg = v->assocReg.find( this );
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
Klucz AssocArray< Klucz,Elem,Container >::firstKey()
{
    if (tab.empty()) return 0;
    else return tab[tab.firstPos()].key;
}

template< class Klucz, class Elem, class Container >
Klucz AssocArray< Klucz,Elem,Container >::lastKey()
{
    if (tab.empty()) return 0;
    else return tab[tab.lastPos()].key;
}

template< class Klucz, class Elem, class Container >
Klucz AssocArray< Klucz,Elem,Container >::nextKey( Klucz v )
{
    if (!v) return firstKey();
    int x;
    assert((x = keyPos( v )) != -1);
    if ((x = tab.nextPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class Container >
Klucz AssocArray< Klucz,Elem,Container >::prevKey( Klucz v )
{
    if (!v) return lastKey();
    int x;
    assert((x = keyPos( v )) != -1);
    if ((x = tab.prevPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class Container >
Elem &AssocArray< Klucz,Elem,Container >::operator[]( Klucz v )
{
    assert( v );
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
int AssocArray< Klucz,Elem,Container >::getKeys( Iterator iter )
{
    for( Klucz key = firstKey(); key; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}


template< class Klucz, class Elem, class AssocCont,class Container >
int PseudoAssocArray< Klucz,Elem,AssocCont,Container >::keyPos( Klucz v )
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
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::firstKey()
{
    if (tab.empty()) return 0;
    else return tab[tab.firstPos()].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::lastKey()
{
    if (tab.empty()) return 0;
    else return tab[tab.lastPos()].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::nextKey( Klucz v )
{
    if (!v) return firstKey();
    int x;
    assert((x = keyPos( v )) != -1);
    if ((x = tab.nextPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Klucz PseudoAssocArray< Klucz,Elem,AssocCont,Container >::prevKey( Klucz v )
{
    if (!v) return lastKey();
    int x;
    assert((x = keyPos( v )) != -1);
    if ((x = tab.prevPos( x )) == -1) return 0;
    return tab[x].key;
}

template< class Klucz, class Elem, class AssocCont,class Container >
Elem &PseudoAssocArray< Klucz,Elem,AssocCont,Container >::operator[]( Klucz v )
{
    assert( v );
    int x = keyPos( v );
    if (x == -1)
    {
        tab[x = tab.newPos()].key = v;
        assocTab[v]=x;
    }
    return tab[x].val;
}

template< class Klucz, class Elem, class AssocCont,class Container >
void PseudoAssocArray< Klucz,Elem,AssocCont,Container >::defrag()
{
    tab.defrag();
    for( int i = 0; i < tab.size(); i++ ) assocTab[tab[i].key] = i;
}


template< class Klucz, class Elem, class AssocCont, class Container > template< class Iterator >
int PseudoAssocArray< Klucz,Elem,AssocCont,Container >::getKeys( Iterator iter )
{
    for( Klucz key = firstKey(); key; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}


int AssocMatrixAddr< AMatrFull >::wsp2pos( std::pair< int,int > w )
{
    int mfs = std::max( w.first,w.second );
    return mfs * mfs + mfs + w.second - w.first;
}

std::pair< int,int > AssocMatrixAddr< AMatrFull >::pos2wsp( int pos )
{
    int x = (int)sqrt( (double)pos );
    if (x * x + x - pos > 0) return std::pair< int,int >( x,pos - x * x );
    else return std::pair< int,int >( x * x + 2 * x - pos,x );
}

template< class Klucz >
std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrFull >::key( Klucz u, Klucz v )
{
    return std::pair< Klucz,Klucz >( u,v );
}

template< class Klucz >
std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrFull >::key( std::pair< Klucz,Klucz> k )
{
    return key( k.first,k.second );
}

int AssocMatrixAddr< AMatrNoDiag >::wsp2pos( std::pair< int,int > w )
{
    int mfs = std::max( w.first,w.second );
    return mfs * mfs + w.second - w.first - ((w.first > w.second) ? 0 : 1);
}

std::pair< int,int > AssocMatrixAddr< AMatrNoDiag >::pos2wsp( int pos )
{
    int x = (int)sqrt( (double)pos );
    if (pos - x * x - x >= 0) return std::pair< int,int >( x + 1,pos - x * x - x );
    else return std::pair< int,int >( x * x + x - 1 - pos,x );
}

template< class Klucz >
std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrNoDiag >::key( Klucz u, Klucz v )
{
    return std::pair< Klucz,Klucz >( u,v );
}

template< class Klucz >
std::pair< Klucz,Klucz > key( std::pair< Klucz,Klucz > k )
{
    return key( k.first,k.second );
}

int AssocMatrixAddr< AMatrClTriangle >::wsp2pos( std::pair< int,int > w )
{
    if (w.first < w.second)
    {
        int z = w.first;
        w.first = w.second;
        w.second = z;
    }
    return w.first * (w.first + 1) / 2 + w.second;
}

std::pair< int,int > AssocMatrixAddr< AMatrClTriangle >::pos2wsp( int pos )
{
    int x = (int)sqrt( (double)2 * pos ), xx = pos - x * (x + 1) / 2;
    if (xx >= 0) return std::pair< int,int >( x,xx );
    else return std::pair< int,int >( x - 1,xx + x );
}

template< class Klucz >
std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrClTriangle >::key( Klucz u, Klucz v )
{
    return std::pair< Klucz,Klucz >( std::min( u,v ),std::max( u,v ) );
}

template< class Klucz >
std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrClTriangle >::key(
    std::pair< Klucz,Klucz > k )
{
    return key( k.first,k.second );
}

int AssocMatrixAddr< AMatrTriangle >::wsp2pos( std::pair< int,int > w )
{
    if (w.first < w.second)
    {
        int z = w.first;
        w.first = w.second;
        w.second = z;
    }
    return w.first * (w.first - 1) / 2 + w.second;
}

std::pair< int,int > AssocMatrixAddr< AMatrTriangle >::pos2wsp( int pos )
{
    int x = (int)sqrt( (double)2 * pos ), xx = pos - x * (x + 1) / 2;
    if (xx >= 0) return std::pair< int,int >( x + 1,xx );
    else return std::pair< int,int >( x,xx + x );
}

template< class Klucz >
std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrTriangle >::key( Klucz u, Klucz v )
{
    return std::pair< Klucz,Klucz >( std::min( u,v ),std::max( u,v ) );
}

template< class Klucz >
std::pair< Klucz,Klucz > AssocMatrixAddr< AMatrTriangle >::key(
    std::pair< Klucz,Klucz > k )
{
    return key( k.first,k.second );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocIndex::AssocIndex(
    int asize ):
    IndexContainer( asize )
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
        if (!operator[]( k )) IndexContainer::delKey( k );
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
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::AssocMatrix( int asize ):
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
int AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::slice1(Klucz v, ExtCont & tab )
{
    if (!index.hasKey( v )) return 0;
    int licz = 0;
    for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) )
        if (hasKey( v,x ))
        {
            tab[x] = operator()( v,x );
            licz++;
        }
    return licz;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
template <class Elem2, class ExtCont>
int AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::slice2(Klucz v, ExtCont &tab )
{
    if (!index.hasKey( v )) return 0;
    int licz = 0;
    for( Klucz x = index.firstKey(); x; x = index.nextKey( x ) )
        if (hasKey( x,v ))
        {
            tab[x] = operator()( x,v );
            licz++;
        }
    return licz;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
bool AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::hasKey( Klucz u, Klucz v )
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
    assert( u && v && AssocMatrixAddr< aType >::correctPos( u,v ) );
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
Elem* AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::presentValPtr(
    Klucz u, Klucz v )
{
    if (!u || !v) return NULL;
    if (!AssocMatrixAddr< aType >::correctPos( u,v )) return NULL;
    std::pair< int,int > wsp =
        std::pair<int,int>( index.klucz2pos( u ),index.klucz2pos( v ) );
    if (wsp.first == -1 || wsp.second == -1) return NULL;
    int pos;
    if (!bufor[pos = AssocMatrixAddr< aType >::wsp2pos( wsp )].present()) return NULL;
    return &bufor[pos].val;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::firstKey()
{
    if (!siz) return std::pair< Klucz,Klucz >( (Klucz)0,(Klucz)0 );
    std::pair< int,int > wsp = AssocMatrixAddr< aType >::pos2wsp( first );
    return AssocMatrixAddr< aType >::key(
        std::pair< Klucz,Klucz >( index.pos2klucz( wsp.first ),
                                  index.pos2klucz( wsp.second ) ) );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::lastKey()
{
    if (!siz) return std::pair< Klucz,Klucz >( (Klucz)0,(Klucz)0 );
    std::pair< int,int > wsp = AssocMatrixAddr< aType >::pos2wsp( last );
    return AssocMatrixAddr< aType >::key(
        std::pair< Klucz,Klucz >( index.pos2klucz( wsp.first ),
                                  index.pos2klucz( wsp.second ) ) );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::nextKey( Klucz u, Klucz v )
{
    if (!u || !v) return firstKey();
    assert( AssocMatrixAddr< aType >::correctPos( u,v ));
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    assert( wsp.first != -1 && wsp.second != -1 );
    int x = AssocMatrixAddr< aType >::wsp2pos( wsp );
    assert( bufor[x].present() );
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
    std::pair< Klucz,Klucz > k )
{
    return nextKey( k.first,k.second );
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
std::pair< Klucz,Klucz >
AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::prevKey( Klucz u, Klucz v )
{
    if (!u || !v) return lastKey();
    assert( AssocMatrixAddr< aType >::correctPos( u,v ));
    std::pair< int,int > wsp =
        std::pair< int,int >( index.klucz2pos( u ),index.klucz2pos( v ) );
    assert( wsp.first != -1 && wsp.second != -1);
    int x = AssocMatrixAddr< aType >::wsp2pos( wsp );
    assert( bufor[x].present() );
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
    std::pair< Klucz,Klucz > k )
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
    for( int ii = 0; ii < i ; ii++ ) operator()( tab[ii].u,tab[ii].v ) = tab[ii].val;
}

template< class Klucz, class Elem, AssocMatrixType aType, class Container, class IndexContainer >
template <class Iterator>
int AssocMatrix< Klucz,Elem,aType,Container,IndexContainer >::getKeys( Iterator iter )
{
    for( std::pair< Klucz,Klucz > key = firstKey(); key.first; key = nextKey( key ) )
    {
        *iter = key;
        iter++;
    }
    return size();
}

template< class T > template< class K, class V >
AssocInserter< T > &AssocInserter< T >::operator=( const std::pair< K,V > &pair )
{
    (*container)[(typename AssocTabInterface< T >::KeyType)pair.first] =
        (typename AssocTabInterface< T >::ValType)pair.second;
    return *this;
}

template< class T, class Fun > template< class K >
AssocFunktorInserter< T,Fun > &AssocFunktorInserter< T,Fun >::operator=( const K &arg )
{
    (*container)[(typename AssocTabInterface< T >::KeyType)arg] =
        (typename AssocTabInterface< T >::ValType)funktor( arg );
    return *this;
}

template< class T, class F >
AssocFunktorInserter< T,F > assocInserter( T &x, F f )
{
    return AssocFunktorInserter< T,F >( x,f );
}

template< typename T > std::ostream &operator<<(
    std::ostream &out, AssocTabInterface< T > cont )
{
    out << '{';
    int siz = cont.size();
    typename AssocTabInterface< T >::KeyType key = cont.firstKey();
    for( ; siz; siz-- )
    {
        out << '(' << key << ',' << cont[key] << ')';
        if (key != cont.lastKey())
        {
            key = cont.nextKey( key );
            out << ',';
        }
    }
    out << '}';
    return out;
}
