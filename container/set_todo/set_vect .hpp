template< typename Element > inline
    Set< Element >::Set( const Set &s ):
        std::vector< Element >( s ) { }

template< typename Element > inline
    Set< Element >::Set( const std::set< Element > &s ):
        std::vector< Element >( s.begin(),s.end() ) { }
 
template< typename Element >
    Set< Element >::Set( const Element *t, unsigned s ) {
        while (s--) *this += t[s];
    } 

template< typename Element >
    Set< Element >::Set( const std::vector< Element > &v ) {
        const_iterator i = v.begin();        
        while (i != v.end()) *this += *i++;
    }

template< typename Element > inline
    unsigned Set< Element >::cardinality() const {
        return this->size();
    }

template< typename Element > inline
    bool Set< Element >::isEmpty() const {
        return cardinality() == 0;
    }

template< typename Element >
    bool Set< Element >::operator<=( const Set< Element > &s ) const {
        if (cardinality() > s.cardinality()) return false;
        const_iterator i = begin(), j = s.begin();
        while (i != end() && j != s.end()) {
            if (*i < *j) return false;
            if (*i == *j) i++;
            j++;
        }
        return i == end();
    }

template< typename Element > inline
    bool Set< Element >::operator<( const Set &s ) const {
        return (cardinality() < s.cardinality()) && (*this <= s);
    }

template< typename Element > inline
    bool Set< Element >::operator>=( const Set &s ) const {
        return s <= *this;
    }

template< typename Element > inline
    bool Set< Element >::operator>( const Set &s ) const {
        return (cardinality() > s.cardinality()) && (s <= *this);
    }

template< typename Element >
    bool Set< Element >::operator==( const Set &s ) const {
        if (cardinality() != s.cardinality()) return false;
        const_iterator i = begin(), j = s.begin();
        while (i != end() && j != end() && *i++ == *j++ ) ;
        return (i == end()) && (j == s.end());
    }

template< typename Element > inline
    bool Set< Element >::operator!=( const Set &s ) const {
        return !(*this == s);
    }

template< typename Element >
    bool Set< Element >::operator+=( const Element &e ) {
        unsigned l = 0, r = this->size() - 1;
        while (l <= r && r < this->size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) return false;
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        insert( std::vector< Element >::begin() + l,e );
        return true;
    }

template< typename Element >
    bool Set< Element >::operator-=( const Element &e ) {
        unsigned l = 0, r = this->size() - 1;
        while (l <= r && r < this->size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) {
                erase( begin() + c );
                return true;
            }
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        return false;
    }

template< typename Element >
    bool Set< Element >::isElement( const Element &e ) {
        unsigned l = 0, r = this->size() - 1;
        while (l <= r && r < this->size()) {
            unsigned c = (l + r) >> 1;
            if (this->at( c ) == e) return true;
            if (this->at( c ) < e) l = c + 1;
            else r = c - 1;
        }
        return false;
    }

template< typename Element >
    Set< Element > Set< Element >::operator+( const Set< Element > &s ) {
        Set< Element > sum;
        const_iterator i = begin(), j = s.begin();
        while (i != end() && j != s.end()) {
            if (*i < *j) sum.push_back( *i++ );
            else if (*i > *j) sum.push_back( *j++ );
            else {
                sum.push_back( *i++ );
                j++;
            }
        }
        while (i != end()) sum.push_back( *i++ );
        while (j != s.end()) sum.push_back( *j++ );
        return sum;
    }
 
template< typename Element > inline
    Set< Element > &Set< Element >::operator+=( const Set< Element > &s ) {
        return *this = *this + s;
    }

template< typename Element >
    Set< Element > Set< Element >::operator*( const Set< Element > &s ) {
        Set< Element > isec;
        const_iterator i = begin(), j = s.begin();
        while (i != end() && j != s.end()) {
            if (*i < *j) i++;
            else if (*i > *j) j++;
            else {
                isec.push_back( *i++ );
                j++;
            }
        }
        return isec;
    }
     
template< typename Element > inline
    Set< Element > &Set< Element >::operator*=( const Set< Element > &s ) {
        return *this = *this * s;
    }
 
template< typename Element >
    Set< Element > Set< Element >::operator-( const Set< Element > &s ) {
        Set< Element > diff;
        const_iterator i = begin(), j = s.begin();
        while (i != end() && j != s.end()) {
            if (*i < *j) diff.push_back( *i++ );
            else if (*i > *j) j++;
            else {
                i++;
                j++;
            }
        }
        while (i != end()) diff.push_back( *i++ );
        return diff;
    }
    
template< typename Element > inline
    Set< Element > &Set< Element >::operator-=( const Set< Element > &s ) {
        return *this = *this - s;
    }

template< typename Element > inline
    Set< Element > Set< Element >::operator^( const Set &s ) {
        Set< Element > sum = *this + s, isect = *this * s;
        return sum - isect;
    }
    
template< typename Element > inline
    Set< Element > &Set< Element >::operator^=( const Set &s ) {
        return *this = *this ^ s;
    }
