
template< typename Element > inline
     Set< Element >::Set( const Set< Element > &s ):
        std::vector< Element >( s ) { }

template< typename Element > inline
Set< Element >::Set( const std::set< Element > &s ):
        std::vector< Element >( s.begin(),s.end() ) { }

template< typename Element > inline
    Set< Element >::Set( const Element *t, unsigned s )
    {
        this->assign( t,t + s );
    }

template< typename Element >
    template <class Iter> Set< Element >::Set( Iter b, Iter e )
    {
       this->assign( b,e );
    }

template< typename Element > inline
    Set< Element >::Set( const std::vector< Element > &v )
    {
        this->assign(v.begin(),v.end());
    }


template< typename Element > inline
    void Set< Element >::assign( const Element *t, unsigned s )
    {
        this->assign( t,t+s );
    }

template< typename Element >
    template< class Iter > void Set< Element >::assign( Iter b, Iter e )
    {
        this->clear();
        for( Iter i = b; i != e; ++i ) push_back( *i );
        std::make_heap( std::vector< Element >::begin(),
            std::vector< Element >::begin() + std::vector< Element >::size() );
        std::sort_heap( std::vector< Element >::begin(),
            std::vector< Element >::begin() + std::vector< Element >::size() );
        resize( std::unique( std::vector< Element >::begin(),
            std::vector< Element >::begin() + std::vector< Element >::size() )
            - std::vector< Element >::begin() );
    }

template< typename Element >
    template <class Iter> void Set< Element >::insert( Iter b, Iter e )
    {
        Set< Element > s( b,e );
        *this += s;
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator=( const Element &e )
    {
        std::vector< Element >::clear();
        std::vector< Element >::push_back( e );
        return *this;
    }

template< typename Element > inline
    bool Set< Element >::operator!() const
    {
        return this->size() == 0;
    }

template< typename Element > inline
    bool Set< Element >::empty() const
    {
        return this->size() == 0;
    }

template< typename Element > inline
    unsigned Set< Element >::size() const
    {
        return std::vector< Element >::size();
    }

template< typename Element > inline
    void Set< Element >::clear()
    {
        return std::vector< Element >::clear();
    }


template< typename Element >
    bool Set< Element >::subsetOf( const Set< Element > &s ) const
    {
        if (this->size() > s.size()) return false;
        typename std::vector< Element >::const_iterator i = this->begin(), j = s.begin();
        while (i != this->end() && j != s.end())
        {
            if (*i < *j) return false;
            if (*i == *j) i++;
            j++;
        }
        return i == this->end();
    }

template< typename Element > inline
    bool Set< Element >::supersetOf( const Set<Element> &s ) const
    {
        return s.subsetOf( *this );
    }

template< typename Element >
    bool operator==( const Set< Element > &s1, const Set< Element > &s2 )
    {
        if (s1.size() != s2.size()) return false;
        typename std::vector< Element >::const_iterator i = s1.begin(), j = s2.begin();
        while (i != s1.end() && j != s2.end() && *i++ == *j++ ) ;
        return (i == s1.end()) && (j == s2.end());
    }

template< typename Element > inline
    bool operator!=( const Set< Element > &s1, const Set< Element > &s2 )
    {
        return !(s1 == s2);
    }

template< typename Element > inline
    bool Set< Element >::add( const Element &e )
    {
        unsigned l = 0, r = size() - 1;
        while (l <= r && r < size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) return false;
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        std::vector< Element >::insert( std::vector< Element >::begin() + l,e );
        return true;
    }

template< typename Element > inline
    Set< Element > & Set< Element >::operator+=( const Element &e )
    {
        this->add( e );
        return *this;
    }

template< typename Element > inline
    bool Set< Element >::del( const Element &e )
    {
        unsigned l = 0, r = size() - 1;
        while (l <= r && r < size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) {
                std::vector< Element >::erase( std::vector< Element >::begin() + c );
                return true;
            }
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        return false;
    }

template< typename Element > inline
    Set< Element > & Set< Element >::operator-=( const Element &e )
    {
        this->del( e );
        return *this;
    }

template< typename Element > inline
    bool Set< Element >::isElement( const Element &e ) const
    {
        unsigned l = 0, r = size() - 1;
        while (l <= r && r < size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) return true;
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        return false;
    }

template< typename Element >
    Set< Element > operator+( const Set< Element > &s1, const Set< Element > &s2 )
    {
        std::vector< Element > res;
        std::set_union( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
        return res;
    }

template< typename Element >
    std::ostream &operator<<( std::ostream &is, const Set< Element > &s )
    {
        is << "{";
        typename std::vector<Element>::const_iterator i = s.begin();
        for( ; i != s.end(); i++ )
        {
            if (i != s.begin()) is << ",";
            is << *i;
        }
        is << "}";
        return is;
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator+=( const Set< Element > &s )
    {
        if (&s == this) return *this;
        *this = *this + s;
        return *this;
    }

template< typename Element >
    Set< Element > operator*( const Set< Element > &s1, const Set< Element > &s2 )
    {
        std::vector< Element > res;
        std::set_intersection( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
        return res;
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator*=( const Set< Element > &s )
    {
        if (&s == this) return *this;
        return *this = *this * s;
    }

template< typename Element > inline
    Set< Element > operator-( const Set< Element > &s1, const Set< Element > &s2 )
    {
        std::vector< Element > res;
        std::set_difference( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
        return res;
    }

template< typename Element >
    Set< Element > &Set< Element >::operator-=( const Set< Element > &s )
    {
        if (this != &s) *this = *this - s;
        else this->clear();
        return *this;
    }

template< typename Element > inline
    Set< Element > operator^( const Set< Element > &s1, const Set< Element > &s2 )
    {
        std::vector< Element > res;
        std::set_symmetric_difference( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
        return res;
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator^=( const Set &s )
    {
        return *this = *this ^ s;
    }

template< typename Element >
    template< class Funktor > Set< Element > Set< Element >::subset( Funktor fun ) const
    {
        Set< Element > subs;
        typename std::vector< Element >::const_iterator i = this->begin();
        for( ; i != this->end(); i++ )
            if (fun( *i )) subs += *i;
        return subs;
    }

template< typename Element >
    template <class Funktor> void Set< Element >::truncate( Funktor fun )
    {
        *this = subset( fun );
    }

template< typename Element >
    template< class Iter > int Set< Element >::getElements( Iter out ) const
    {
        typename std::vector< Element >::const_iterator i = this->begin();
        for( ; i != this->end(); i++ )
        {
            *out = *i;
            ++out;
        }
        return this->size();
    }

template< typename Element > inline
    Element Set< Element >::first() const
    {   if (size()==0) return (Element)0;
        return *begin();
    }

template< typename Element > inline
   Element Set< Element >::last() const
   {   if (size()==0) return (Element)0;
       return *std::vector< Element >::rbegin();
   }

template< typename Element > inline
    Element Set< Element >::prev( const Element &e ) const
    {   if (!e) return last();
        unsigned l = 0, r = size() - 1;
        while (l <= r && r < size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) return this->at( c - 1 );
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        return this->at( l );
    }

template< typename Element > inline
    Element Set< Element >::next( const Element &e ) const
    {   if (!e) return first();
        unsigned l = 0, r = size() - 1;
        while (l <= r && r < size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) return this->at( c + 1 );
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        return this->at( l + 1 );
    }

//template< class Element >
//SetInserter< Set< Element > >:: &operator=( const Element &value )
//{
//    (*container) += value;
//    return *this;
//}

template< class Element >
SetInserter< Set< Element > > setInserter( Set< Element > &x )
{
    return SetInserter< Set< Element > >( x );
}
