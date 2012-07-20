template< typename Element > inline
    Set< Element >::Set( const Set<Element> &s ):
        std::set< Element >( s ) { }

template< typename Element > inline
    Set< Element >::Set( const std::set< Element > &s ):
        std::set< Element >( s ) { }

template< typename Element > inline
    Set< Element >::Set( const Element *t, unsigned s ):
        std::set< Element >( t,t + s ) { }

template< typename Element >
    template <class Iter>
    Set< Element >::Set( Iter b, Iter e ):
        std::set< Element >( b,e ) { }

template< typename Element > inline
    Set< Element >::Set( const std::vector< Element > &v ):
        std::set< Element >( v.begin(),v.end() ) { }

template< typename Element > inline
    void Set< Element >::assign( const Element *t, unsigned s )
    {
        this->clear();
        std::set<Element>::insert( t,t + s );
    }

template< typename Element >
    template <class Iter>
    void Set< Element >::assign( Iter b, Iter e )
    {
        this->clear();
        std::set< Element >::insert( b,e );
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator=( const Element &e )
    {
        std::set< Element >::clear();
        std::set< Element >::insert(e);
        return *this;
    }

template< typename Element > inline
    bool Set< Element >::operator!() const
    {
        return this->size() == 0;
    }

template< typename Element >
    bool Set< Element >::subsetOf( const Set< Element > &s ) const
    {
        if (this->size() > s.size()) return false;
        typename std::set< Element >::const_iterator i = this->begin(), j = s.begin();
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
        typename std::set< Element >::const_iterator i = s1.begin(), j = s2.begin();
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
        return std::set< Element >::insert( e ).second;
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator+=( const Element &e )
    {
        std::set< Element >::insert( e );
        return *this;
    }

template< typename Element > inline
    bool Set< Element >::del( const Element &e ) {
        return std::set< Element >::erase( e );
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator-=( const Element &e )
    {
        std::set< Element >::erase( e );
        return *this;
    }

template< typename Element > inline
    bool Set< Element >::isElement( const Element &e ) const
    {
        return this->find( e ) != this->end();
    }

template< typename Element >
    Set< Element > operator+( const Set< Element > &s1, const Set< Element > &s2 )
    {
        std::vector<Element> res;
        std::set_union( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
        return res;
    }

template< typename Element >
    std::ostream &operator<<( std::ostream &is, const Set< Element > &s )
    {
        is << "{";
        typename std::set< Element >::const_iterator i = s.begin();
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
        std::vector<Element> res;
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
        std::vector<Element> res;
        std::set_difference( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
        return res;
    }

template< typename Element >
    Set< Element > &Set< Element >::operator-=( const Set< Element > &s ) {
        if (this != &s) *this= *this - s;
        else this->clear();
        return *this;
    }

template< typename Element > inline
    Set< Element > operator^( const Set< Element > &s1, const Set< Element > &s2 )
    {
        std::vector<Element> res;
        std::set_symmetric_difference( s1.begin(),s1.end(),s2.begin(),s2.end(),std::back_inserter( res ) );
        return res;
    }

template< typename Element > inline
    Set< Element > &Set< Element >::operator^=( const Set &s )
    {
        return *this = *this ^ s;
    }

template< typename Element > template< class Funktor >
    Set< Element > Set< Element >::subset( Funktor fun ) const
    {
        Set< Element > subs;
        typename std::set< Element >::const_iterator i = this->begin();
        for( ; i != this->end(); i++ )
            if (fun( *i )) subs += *i;
        return subs;
    }

template< typename Element > template< class Funktor >
    void Set< Element >::truncate( Funktor fun )
    {
        *this = subset( fun );
    }

template< typename Element > template< class Iter >
     int Set< Element >::getElements( Iter out ) const
     {
         typename std::set< Element >::const_iterator i = this->begin();
         for( ; i != this->end(); i++ )
         {
             *out = *i;
             ++out;
         }
         return this->size();
     }

template< typename Element >
    Element Set< Element >::first() const
    {   if (this->size()==0) return this->badValue();//(Element)0;
        return *this->begin();
    }

template< typename Element >
    Element Set< Element >::last() const
    {   if (this->size()==0) return this->badValue();//(Element)0;;
        return *(--this->end());
    }

template< typename Element >
    Element Set< Element >::next( const Element &a ) const
    {   if (this->isBad(a)) return first();
        typename std::set< Element >::const_iterator i = this->find( a );
        assert( i != this->end() ); // TODO: throw
        i++;
        assert( i != this->end() ); // TODO: throw
        return *i;
    }

template< typename Element >
    Element Set< Element >::prev( const Element &a ) const
    {   if (this->isBad(a)) return last();
        typename std::set< Element >::const_iterator i = this->find( a );
        assert( i != this->end() && i != this->begin() ); // TODO: throw
        i--;
        return *i;
    }




