// SetInserter
template< class Element > SetInserter< Set< Element > >
    &SetInserter< Set< Element > >::operator=( const Element &value )
{ 
    (*container) += value; 
    return *this;
}

template< class ValType, class ArgType, class Funktor > Set< ValType > imageSet( const Set< ArgType > &arg, Funktor f )
{   
    Set< ValType > res;
    for( ArgType i = arg.first(); !arg.isBad( i ); i = arg.next( i ) ) res += (ValType)f( i );
    return res;
}

template< class ValType, class ArgType, class Funktor >
    Set< ArgType > preimageSet( const Set< ValType > &arg, const Set< ArgType > &domain, Funktor f )
{   
    Set< ArgType > res;
    for( ArgType i = domain.first(); !domain.isBad( i ) ; i = domain.next( i ) )
        if (arg.isElement( (ValType)f( i ) )) res += i;
    return res;
}

template< typename Element > bool operator==( const Set< Element > &s1, const Set< Element > &s2 )
{
    if (s1.size() != s2.size()) return false;
    typename std::vector< Element >::const_iterator i = s1.begin(), j = s2.begin();
    while (i != s1.end() && j != s2.end() && *i++ == *j++ ) ;
    return (i == s1.end()) && (j == s2.end());
}

template< typename Element > Set< Element > operator+( const Set< Element > &s1, const Set< Element > &s2 )
{
    std::vector< Element > res;
    std::set_union( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
    return res;
}

template< typename Element > Set< Element > operator*( const Set< Element > &s1, const Set< Element > &s2 )
{
    std::vector< Element > res;
    std::set_intersection( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
    return res;
}

template< typename Element > inline Set< Element > operator-( const Set< Element > &s1, const Set< Element > &s2 )
{
    std::vector< Element > res;
    std::set_difference( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
    return res;
}

template< typename Element > inline Set< Element > operator^( const Set< Element > &s1, const Set< Element > &s2 )
{
    std::vector< Element > res;
    std::set_symmetric_difference( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
    return res;
}
