// BinomHeapNode

template< class Key > inline void BinomHeapNode< Key >::insert( BinomHeapNode< Key > *A )
{
    A->parent = this;
    A->next = child;
    child = A;
    degree++;
}

template< class Key > bool BinomHeapNode< Key >::check() const
{
    unsigned degree = 0;
    for( BinomHeapNode< Key > *child = this->child; child; child = child->next,degree++ )
        if (child->parent != this || !child->check())
            return false;
    return degree == this->degree;
}

template< class Key, class Compare, class Allocator > BinomHeap< Key,Compare,Allocator >
    &BinomHeap< Key,Compare,Allocator >::operator=( const BinomHeap< Key,Compare,Allocator > &X )
{   
    if (this == &X) return *this;
    clear();
    root = minimum = 0;
    nodes = X.nodes;
    function = X.function;
    if (!nodes) return *this;
    root = copy( X.root,0 );

    Node *A = root, *B = X.root;
    while (B != X.minimum) 
    {
        A = A->next;
        B = B->next;
    }
    minimum = A;

    return *this;
}

template< class Key, class Compare, class Allocator > template <class InputIterator>
    void BinomHeap< Key,Compare,Allocator >::assign( InputIterator first, InputIterator last )
{   
    int len = 0;
    clear();
    for( InputIterator i = first; i != last; ++i ) ++len;
    assign2( first,len );
}

template< class Key, class Compare, class Allocator > typename BinomHeap< Key,Compare,Allocator >::Node
    *BinomHeap< Key,Compare,Allocator >::newNode( Key key )
{
    if (!allocator) return new Node( key );
    Node *res = allocator->template allocate< BinomHeapNode< Key > >();
    res->key = key;
    return res;
}

template< class Key, class Compare, class Allocator > void BinomHeap< Key,Compare,Allocator >::delNode( Node *node )
{
    if (!allocator) delete node;
    else allocator->deallocate( node );
}

template< class Key, class Compare, class Allocator > template< class InputIterator >
    void BinomHeap< Key,Compare,Allocator >::assign2( InputIterator &first, int len )
{
    if (len == 1)
    { 
        push( *first );
        ++first;
    }
    if (len <= 1) return;
    int half = len/2;
    BinomHeap< Key,Compare,Allocator > heap( *this );
    assign2( first,half );
    heap.assign2( first,len - half );
    merge( heap );
}

template< class Key, class Compare,class Allocator > inline
    BinomHeap< Key,Compare,Allocator >::BinomHeap( const BinomHeap< Key,Compare,Allocator > &other ):
    root( 0 ), minimum( 0 ), nodes( other.nodes ), allocator( other.allocator ), function( other.function )
{
    if (!other.nodes) return;
    root = copy( other.root,0 );

    Node *A = root, *B = other.root;
    while (B != other.minimum) {
        A = A->next;
        B = B->next;
    }
    minimum = A;
}

template< class Key, class Compare, class Allocator >
    typename BinomHeap< Key,Compare,Allocator >::Node *BinomHeap< Key,Compare,Allocator >::copy( Node *A, Node *parent )
{
    Node *B = A, *C = newNode( B->key ), *D = C;
    D->parent = parent;
    D->child = B->child ? copy( B->child,B ) : 0;
    while (B->next)
    {
        B = B->next;
        D = D->next = newNode( B->key );
        D->parent = parent;
        D->child = B->child ? copy(B->child, B) : 0;
    }
    D->next = 0;

    return C;
}

template< class Key, class Compare, class Allocator > Key BinomHeap< Key,Compare,Allocator >::top() const
{   
    koalaAssert( minimum,ContExcOutpass );
    return minimum->key;
}

template< class Key, class Compare, class Allocator > typename BinomHeap< Key,Compare,Allocator>::Node
    *BinomHeap< Key,Compare,Allocator >::push( const Key &key )
{
    nodes++;
    Node *A = newNode( key );

    if (root == 0) return root = minimum = A;

    root = join( root,A );
    if (function( A->key,minimum->key )) minimum = A;

    while (minimum->parent) minimum = minimum->parent;

    return A;
}

template< class Key, class Compare, class Allocator > void BinomHeap< Key,Compare,Allocator >::pop()
{
    nodes--;

    if(root == minimum) root = root->next;
    else
    {
        Node *A = root;
        while (A->next != minimum) A = A->next;
        A->next = minimum->next;
    }

    if(nodes == 0)
    {
        delNode( minimum );
        minimum = 0;
        return;
    }

    Node *child = minimum->child;
    if (child)
    {
        for( Node *A = child; A; A = A->next ) A->parent = 0;
        root = root ? join( root,reverse( child )) : reverse( child );
    }

    delNode( minimum );
    minimum = root;
    if (minimum)
        for( Node *A = root->next; A; A = A->next )
            if (function( A->key,minimum->key )) minimum = A;
}

template< class Key, class Compare, class Allocator >
    void BinomHeap< Key,Compare,Allocator >::decrease( Node *A, const Key &key )
{
    koalaAssert( !function( A->key,key ),ContExcWrongArg );
    if (!function( A->key,key ) && !function( key,A->key )) return;

    A->key = key;
    if (function( key,minimum->key )) minimum = A;

    if (!A->parent || function( A->parent->key,A->key )) return;

    Node *start = 0, *previous = 0, *B = A, *C = A->parent, *D;
    while (C)
    {
        D = C->child;
        C->child = B->next;
        if (B == D)
        {
            D = C;
            C->degree--;
        }
        else
        {
            Node *E = D;
            while (B != E->next) 
            {
                E = E->next;
                C->degree--;
            }
            E->next = C;
            C->degree -= 2;
        }
        B->next = start;
        B = C;
        C = C->parent;
        start = previous;
        previous = D;
    }

    if (B == root) root = root->next;
    else
    {
        C = root;
        while (B != C->next) C = C->next;
        C->next = B->next;
    }
    B->next = start;
    start = previous;

    if(start)
    {
        for( B = start; B; B = B->next ) B->parent = 0;
        root = root ? join( root,reverse( start )) : reverse( start );
    }
    A->parent = 0;
    A->next = 0;
    root = root ? join( root,A ) : A;

    while (minimum->parent) minimum = minimum->parent;
}

template< class Key, class Compare, class Allocator > void BinomHeap< Key,Compare,Allocator >::del( Node *A )
{   
    koalaAssert( nodes,ContExcOutpass );
    nodes--;

    if (nodes == 0)
    {
        delNode( A );
        root = minimum = 0;
        return;
    }

    Node *start = A->child, *previous = A->child, *next, *B = A, *C = A->parent;
    while (C)
    {
        next = C->child;
        C->child = B->next;
        if (B == next)
        {
            next = C;
            C->degree--;
        }
        else
        {
            Node *D = next;
            while (B != D->next)
            {
                D = D->next;
                C->degree--;
            }
            D->next = C;
            C->degree -= 2;
        }
        B->next = start;
        B = C;
        C = C->parent;
        start = previous;
        previous = next;
    }

    if (B == root) root = root->next;
    else
    {
        C = root;
        while (B != C->next) C = C->next;
        C->next = B->next;
    }
    B->next = start;
    start = previous;

    if (start)
    {
        for( B = start; B; B = B->next ) B->parent = 0;
        root = root ? join( root,reverse( start )) : reverse( start );
    }

    if (minimum == A)
    {
        minimum = root;
        for( B = root->next; B; B = B->next )
            if (function( B->key,minimum->key )) minimum = B;
    }
    else while (minimum->parent) minimum = minimum->parent;
    delNode( A );
}

template< class Key, class Compare, class Allocator > void BinomHeap< Key,Compare,Allocator >::merge( BinomHeap &heap )
{
    if(!heap.root || root == heap.root) return;
    else if (root)
    {
        root = join( root,heap.root );
        if (function( heap.minimum->key,minimum->key )) minimum = heap.minimum;
        nodes += heap.nodes;
    }
    else
    {
        root = heap.root;
        minimum = heap.minimum;
        nodes = heap.nodes;
    }
    heap.root = heap.minimum = 0;
    heap.nodes = 0;

    while (minimum->parent) minimum = minimum->parent;
}

template< class Key, class Compare, class Allocator > void BinomHeap< Key,Compare,Allocator >::clear( Node *n )
{
    if (n->next) clear( n->next );
    if (n->child) clear( n->child );
    delNode( n );
}

template< class Key, class Compare, class Allocator > void BinomHeap< Key,Compare,Allocator >::clear()
{
    if (root) clear( root );
    root = minimum = 0;
    nodes = 0;
}

template< class Key, class Compare, class Allocator > bool BinomHeap< Key,Compare,Allocator >::check() const
{
    Node *A = root;
    while (A)
    {
        if (A->parent || !A->check()) return false;
        A = A->next;
    }
    return true;
}

template< class Key, class Compare, class Allocator > inline typename BinomHeap< Key,Compare,Allocator>::Node
    *BinomHeap< Key,Compare,Allocator >::join( Node *A, Node *B )
{
    Node *start, *C;
    if (A->degree <= B->degree)
    { 
        start = C = A;
        A = A->next;
    }
    else
    {
        start = C = B;
        B = B->next;
    }
    while (A && B)
    {
        if (A->degree <= B->degree)
        {
            C->next = A;
            A = A->next;
        }
        else
        {
            C->next = B;
            B = B->next;
        }
        C = C->next;
    }
    C->next = A ? A : B;

    for( A = 0, B = start, C = B->next; C; C = B->next )
        if (B->degree != C->degree || (C->next && C->degree == C->next->degree))
        {
            A = B;
            B = C;
        }
        else if (function( B->key,C->key ))
        {
            B->next = C->next;
            B->insert( C );
        }
        else
        {
            if (A) A->next = C;
            else start = C;
            C->insert( B );
            B = C;
        }
    return start;
}

template< class Key, class Compare, class Allocator > inline typename BinomHeap< Key,Compare,Allocator >::Node
    *BinomHeap< Key,Compare,Allocator >::reverse( Node *A )
{
    Node *B = A->next, *C;
    A->next = 0;
    while (B)
    {
        C = B->next;
        B->next = A;
        A = B;
        B = C;
    }
    return A;
}

template< class Key, class Compare, class Allocator > inline typename BinomHeap< Key,Compare,Allocator>::Node
    *BinomHeap< Key,Compare,Allocator >::cut( Node *A )
{
    Node *B = A->next, *C;
    A->next = 0;
    while (B)
    {
        C = B->next;
        B->next = A;
        A = B;
        B = C;
    }
    return A;
}

template< class Key > inline void FibonHeapNode< Key >::init( const Key &_key )
{   
    parent = child = 0;
    flag = 0;
    key=_key;
    previous = next = this;
}

template< class Key > inline void FibonHeapNode< Key >::insert( FibonHeapNode< Key > *A )
{
    next->previous = A->previous;
    A->previous->next = next;
    next = A;
    A->previous = this;
}

template< class Key > inline void FibonHeapNode< Key >::remove()
{
    previous->next = next;
    next->previous = previous;
    previous = next = this;
}

template< class Key > bool FibonHeapNode< Key >::check() const
{
    FibonHeapNode< Key > *child = this->child;
    unsigned degree = 0;

    if (!child) return flag < 2;

    do
    {
        if (child->previous->next != child || child->next->previous != child || child->parent != this || !child->check())
            return false;
        child = child->next;
        degree++;
    }
    while (child != this->child) ;
    return degree == (flag >> 1);
}

template< class Key, class Compare, class Allocator > typename FibonHeap< Key,Compare,Allocator >::Node
    *FibonHeap< Key,Compare,Allocator >::newNode( Key key )
{
    if (!allocator) return new Node( key );
    Node *res = allocator->template allocate< FibonHeapNode< Key > >();
    res->init( key );
    return res;
}

template< class Key, class Compare, class Allocator > void FibonHeap< Key,Compare,Allocator >::delNode( Node *node )
{
    if (!allocator) delete node;
    else allocator->deallocate( node );
}

template< class Key, class Compare, class Allocator > FibonHeap< Key,Compare,Allocator >
    &FibonHeap< Key,Compare,Allocator >::operator=( const FibonHeap< Key,Compare,Allocator > &X )
{   
    if (this == &X) return *this;
    clear();
    root=0;
    nodes = X.nodes;
    function = X.function;
    if (!nodes) return *this;
    root = copy( X.root,0 );

    return *this;
}

template< class Key, class Compare, class Allocator > template< class InputIterator >
    void FibonHeap< Key,Compare,Allocator >::assign( InputIterator first, InputIterator last )
{
    clear();
    for( ; first != last; first++ ) push( *first );
}

template< class Key, class Compare,class Allocator > inline
    FibonHeap< Key,Compare,Allocator >::FibonHeap( const FibonHeap< Key,Compare,Allocator > &other ):
        root( 0 ), nodes( other.nodes ), allocator( other.allocator ), function( other.function )
{   
    if (!other.nodes) return;
    root = copy( other.root,0 );
}

template< class Key, class Compare, class Allocator > typename FibonHeap< Key,Compare,Allocator>::Node
    *FibonHeap< Key,Compare,Allocator >::copy( Node *A, Node *parent )
{
    Node *B = A, *C = newNode( B->key ), *D = C, *E;
    D->parent = parent;
    D->child = B->child ? copy( B->child,B ) : 0;
    while (B->next != A)
    {
        B = B->next;
        E = D;
        D = D->next = newNode( B->key );
        D->previous = E;
        D->parent = parent;
        D->child = B->child ? copy( B->child,B ) : 0;
    }
    D->next = C;
    C->previous = D;

    return C;
}

template< class Key, class Compare, class Allocator > Key FibonHeap< Key,Compare,Allocator >::top() const
{   
    koalaAssert( root,ContExcOutpass );
    return root->key;
}

template< class Key, class Compare, class Allocator > typename FibonHeap< Key,Compare,Allocator >::Node
    *FibonHeap< Key,Compare,Allocator >::push( const Key &key )
{
    nodes++;

    Node *A = newNode( key );
    if (!root) return root = A;

    root->insert( A );
    if (function( A->key,root->key )) root = A;
    return A;
}

template< class Key, class Compare, class Allocator > void FibonHeap< Key,Compare,Allocator >::pop()
{   
    koalaAssert( nodes,ContExcOutpass );
    nodes--;

    Node *A = root->child, *B;
    if (A)
    {
        B = A;
        do
        {
            B->parent = 0;
            B = B->next;
        } while (A != B);
        root->insert( A );
    }

    if (!nodes)
    {
        delNode( root );
        root = 0;
        return;
    }

    Node *LOCALARRAY( _degrees,(sizeof( unsigned ) << 3) );
    for( int i = 0; i <(sizeof( unsigned ) << 3); i++ ) _degrees[i] = 0;
    Node **degrees = _degrees, *C;
    unsigned degree_max = 0, degree = 0;
    for( A = root->next, B = A->next; A != root; degrees[degree] = A, A = B, B = A->next )
    {
        while (degrees[degree = A->flag >> 1])
        {
            C = degrees[degree];
            if (function( C->key,A->key ))
            {
                C = A;
                A = degrees[degree];
            }
            degrees[degree] = 0;
            C->remove();
            C->parent = A;
            C->flag &= ~1;
            if (A->child)
            {
                A->flag += 2;
                A->child->insert( C );
            }
            else
            {
                A->flag = 2;
                A->child = C;
            }
        }

        if (degree > degree_max) degree_max = degree;
    }
    root->remove();
    delNode( root );

    for( degree = 0; degree <= degree_max; degree++ )
        if (degrees[degree])
        {
            root = degrees[degree];
            degrees[degree] = 0;
            degree++;
            break;
        }
    for( ; degree <= degree_max; degree++ )
        if (degrees[degree])
        {
            if (function( degrees[degree]->key,root->key )) root = degrees[degree];
            degrees[degree] = 0;
        }
}

template< class Key, class Compare, class Allocator >
    void FibonHeap< Key,Compare,Allocator >::decrease( Node *A, const Key &key )
{
    koalaAssert( !function( A->key,key ),ContExcWrongArg );
    if (!function( A->key,key ) && !function( key,A->key )) return;

    A->key = key;
    Node *B = A->parent;
    if (!B)
    {
        if (function( key,root->key )) root = A;
        return;
    }
    else if (!function( key,B->key )) return;

    while (1)
    {
        if (A == A->next) B->child = 0;
        else
        {
            if (A == B->child) B->child = A->next;
            A->remove();
            A->flag &= ~1;
        }
        B->flag -= 2;
        root->insert( A );
        A->parent = 0;
        if (function( A->key,root->key )) root = A;

        if (!B->parent) return;
        if (!(B->flag & 1))
        {
            B->flag |= 1;
            return;
        }
        A = B;
        B = B->parent;
    }
}

template< class Key, class Compare, class Allocator > void FibonHeap< Key,Compare,Allocator >::del( Node *A )
{
    Node *B = A->parent, *C = A;
    if (!B)
    {
        root = A;
        pop();
        return;
    }

    while (1)
    {
        if (A == A->next) B->child = 0;
        else
        {
            if (A == B->child) B->child = A->next;
            A->remove();
            A->flag &= ~1;
        }
        B->flag -= 2;
        root->insert( A );
        A->parent = 0;

        if (!B->parent) break;
        if (!(B->flag & 1))
        {
            B->flag |= 1;
            break;
        }
        A = B;
        B = B->parent;
    }

    root = C;
    pop();
}

template< class Key, class Compare, class Allocator > void FibonHeap< Key,Compare,Allocator >::merge( FibonHeap &heap )
{
    if(!heap.root || root == heap.root) return;
    else if (root)
    {
        root->insert( heap.root );
        if (function( heap.root->key,root->key )) root = heap.root;
        nodes += heap.nodes;
    }
    else
    {
        root = heap.root;
        nodes = heap.nodes;
    }
    heap.root = 0;
    heap.nodes = 0;
}

template< class Key, class Compare, class Allocator > void FibonHeap< Key,Compare,Allocator >::clear()
{
    if (root) clear( root );
    root = 0;
    nodes = 0;
}

template< class Key, class Compare, class Allocator > void FibonHeap< Key,Compare,Allocator >::clear( Node *n )
{
    if (n->child) clear( n->child );
    if (n->previous != n)
    {
        n->next->previous = n->previous;
        clear( n->next );
    }
    delNode( n );
}

template< class Key, class Compare, class Allocator > bool FibonHeap< Key,Compare,Allocator >::check() const
{
    if (!root) return true;

    Node *A = root;
    do
    {
        if (A->next->previous != A || A->parent || !A->check()) return false;
        A = A->next;
    } while (A != root);

    return true;
}
