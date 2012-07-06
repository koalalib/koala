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
    Privates::BlockListVectInerfTest<Container> error; // wymuszony blad kompilacji
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
//    cont[pos].blob=Element(); //TODO:???
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
    for( int i = firstPos(); i != -1; i = nextPos( i ) ) buf[asiz++] = this->operator[]( i );
    clear();
    for( int i = 0; i < asiz; i++ ) this->operator[]( newPos() ) = buf[i];
}
