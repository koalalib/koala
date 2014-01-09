template< class T > std::pair< T,T > pairMinMax( T a, T b )
{
	std::pair< T,T > res;
	if (a <= b)
	{
		res.first = a;
		res.second = b;
	}
	else
	{
		res.first = b;
		res.second = a;
	}
	return res;
}

template< class T > std::pair< T,T > pairMaxMin( T a, T b )
{
	std::pair< T,T > res;
	if (a >= b)
	{
		res.first = a;
		res.second = b;
	}
	else
	{
		res.first = b;
		res.second = a;
	}
	return res;
}

template< class T > void StackInterface< T * >::push( const T &val )
{
	koalaAssert( siz < maxsize,ContExcFull );
	buf[siz++] = val;
}

template< class T > void StackInterface< T * >::pop()
{
	koalaAssert( siz,ContExcOutpass );
	siz--;
}

template< class T > T &StackInterface< T * >::top()
{
	koalaAssert( siz,ContExcOutpass );
	return buf[siz - 1];
}

template< class T > template< class InputIterator >
	void StackInterface< T * >::assign( InputIterator first, InputIterator last)
{
	clear();
	for( ; first != last; first++ ) push( *first );
}

template< class T > void QueueInterface< T * >::push( const T &val )
{
	buf[end] = val;
	end = next( end );
	koalaAssert( end != beg,ContExcFull );
}

template< class T > void QueueInterface< T * >::pushFront( const T &val )
{
	beg = prev( beg );
	koalaAssert( end != beg,ContExcFull );
	buf[beg] = val;
}


template< class T > void QueueInterface< T * >::pop()
{
	koalaAssert( beg != end,ContExcOutpass );
	beg = next( beg );
}

template< class T > void QueueInterface< T * >::popBack()
{
	koalaAssert( beg != end,ContExcOutpass );
	end = prev( end );
}


template< class T > T &QueueInterface< T * >::front()
{
	koalaAssert( !empty(),ContExcOutpass );
	return buf[beg];
}

template< class T > T &QueueInterface< T * >::top()
{
	koalaAssert( !empty(),ContExcOutpass );
	return buf[beg];
}

template< class T > T &QueueInterface< T * >::back()
{
	koalaAssert( !empty(),ContExcOutpass );
	return buf[prev( end )];
}

template< class T > template< class InputIterator >
	void QueueInterface< T * >::assign( InputIterator first, InputIterator last )
{
	clear();
	for( ; first != last; first++ ) push( *first );
}


template< class T, class Comp >
	PriQueueInterface< T *,Comp >::PriQueueInterface( T* bufor, T* end, int max, Comp acomp ):
		buf( bufor ), maxsize( max ), siz( end - bufor ), comp( acomp )
{
	koalaAssert( siz >= 0 && siz <= maxsize,ContExcWrongArg );
	std::make_heap( buf,end,comp );
}

template< class T, class Comp > void PriQueueInterface< T *,Comp >::push( const T &val )
{
	koalaAssert( siz < maxsize,ContExcFull );
	buf[siz++] = val;
	std::push_heap( buf,buf + siz,comp);
}

template< class T, class Comp > void PriQueueInterface< T *,Comp >::pop()
{
	koalaAssert( siz,ContExcOutpass );
	std::pop_heap( buf,buf + siz,comp );
	siz--;
}

template< class T, class Comp > T PriQueueInterface< T *,Comp >::top() const
{
	koalaAssert( siz,ContExcOutpass );
	return buf[0];
}

template< class T, class Comp > template< class InputIterator >
	void PriQueueInterface< T *,Comp >::assign( InputIterator first, InputIterator last )
{
	clear();
	for( siz = 0; first != last; first++,siz++ )
	{
		buf[siz] = (*first);
		koalaAssert( siz < maxsize,ContExcFull );
	}
	std::make_heap( buf,buf + siz,comp );
}
