/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Array.inl
*
*	Comments	-	See Array.h
*
**************************************************************************************/


namespace Pulse
{

	template < typename T >
	Array<T>::Iterator::Iterator( void )
		: m_pArray( PSX_NULL ), m_index(-1)
	{
	}

	template < typename T >
	Array<T>::Iterator::Iterator( ThisArray *pArray, INT index )
		: m_pArray( pArray ), m_index(index)
	{
	}

	template < typename T >
	T & Array<T>::Iterator::operator * ( void )
	{
		return (*m_pArray)[m_index];
	}

	template < typename T >
	T & Array<T>::Iterator::operator * ( void ) const
	{
		return (*m_pArray)[m_index];
	}

	template < typename T >
	T * Array<T>::Iterator::operator -> ( void )
	{
		return GetPtrGeneric<T>((*m_pArray)[m_index]);
	}

	template < typename T >
	T * Array<T>::Iterator::operator -> ( void ) const
	{
		return GetPtrGeneric<T>((*m_pArray)[m_index]);
	}

	template < typename T >
	typename Array<T>::Iterator & Array<T>::Iterator::operator ++ ( void )
	{
		if ( (SIZE_T)(m_index + 1) < m_pArray->GetSize() )
			++m_index;
		else 
			m_index = m_pArray->GetSize();

		return *this;
	}

	template < typename T >
	typename Array<T>::Iterator Array<T>::Iterator::operator ++ ( int )
	{
		Iterator iter(*this);

		if ( (SIZE_T)(m_index + 1) < m_pArray->GetSize() )
			++m_index;
		else 
			m_index = m_pArray->GetSize();

		return iter;
	}

	template < typename T >
	typename Array<T>::Iterator & Array<T>::Iterator::operator -- ( void )
	{
		if ( m_index - 1 >= 0 )
			--m_index;
		else
			m_index = -1;

		return *this;
	}

	template < typename T >
	typename Array<T>::Iterator Array<T>::Iterator::operator -- ( int )
	{
		Iterator iter(*this);

		if ( m_index - 1 >= 0 )
			--m_index;
		else
			m_index = -1;

		return iter;
	}

	template < typename T >
	BOOL Array<T>::Iterator::operator == ( const Iterator &rhs ) const
	{
		if ( m_pArray == rhs.m_pArray )
		{
			if ( m_index == rhs.m_index )
				return TRUE;
			else if ( (m_index == -1 || m_index == m_pArray->GetSize()) &&
				rhs.m_index == -1 || rhs.m_index == rhs.m_pArray->GetSize() )
				return TRUE;
		}
	
		return FALSE;
	}

	template < typename T >
	BOOL Array<T>::Iterator::operator != ( const Iterator &rhs ) const
	{
		return !(*this == rhs);
	}

	template < typename T >
	typename Array<T>::Iterator & Array<T>::Iterator::operator = ( Iterator &rhs )
	{
		m_pArray = rhs.m_pArray;
		m_index = rhs.m_index;

		return *this;
	}

	// Array
	template < typename T >
	PSX_INLINE Array< T >::Array( VOID ) : m_size( 0 ), m_capacity( 0 ), m_pArray( NULL )
	{
	}

	template < typename T >
	PSX_INLINE Array< T >::Array( SIZE_T size ) : m_size( size ), m_capacity( size )
	{
		m_pArray = new T[m_capacity];
	}

	template < typename T >
	PSX_INLINE Array< T >::Array( const Array &vec ) : m_size( 0 ), m_capacity( 0 ), m_pArray( NULL )
	{
		*this = vec;
	}

	template < typename T >
	PSX_INLINE Array< T >::~Array( VOID )
	{
		PSX_SafeDeleteArray( m_pArray );
	}

	template < typename T >
	PSX_INLINE SIZE_T Array< T >::GetSize( VOID )
	{
		return m_size;
	}

	template < typename T >
	PSX_INLINE const SIZE_T Array< T >::GetSize( VOID ) const
	{
		return m_size;
	}

	template < typename T >
	PSX_INLINE SIZE_T Array< T >::GetCapacity( VOID )
	{
		return m_capacity;
	}

	template < typename T >
	PSX_INLINE T & Array< T >::GetHead( VOID )
	{
		PSX_Assert( m_pArray, "Array container is empty." );
		return *m_pArray;
	}

	template < typename T >
	PSX_INLINE const T & Array< T >::GetHead( VOID ) const
	{
		PSX_Assert( m_pArray, "Array container is empty." );
		return *m_pArray;
	}

	template < typename T >
	PSX_INLINE T & Array< T >::GetTail( VOID )
	{
		PSX_Assert( m_pArray, "Array container is empty." );
		return m_pArray[m_size - 1];
	}

	template < typename T >
	PSX_INLINE const T & Array< T >::GetTail( VOID ) const
	{
		PSX_Assert( m_pArray, "Array container is empty." );
		return m_pArray[m_size - 1];
	}

	template < typename T >
	PSX_INLINE T & Array< T >::operator [] ( SIZE_T index )
	{
		PSX_Assert( index >= 0 && index < m_size , "Index out of range.");
		return m_pArray[index];
	}

	template < typename T >
	PSX_INLINE const T & Array< T >::operator [] ( SIZE_T index ) const
	{
		PSX_Assert( index >= 0 && index < m_size , "Index out of range.");
		return m_pArray[index];
	}

	template < typename T >
	PSX_INLINE VOID Array< T >::Clear( VOID )
	{
		m_size = 0;
		m_capacity = 0;
		PSX_SafeDeleteArray( m_pArray );
	}

	template < typename T >
	PSX_INLINE VOID Array< T >::FastClear( VOID )
	{
		m_size = 0;
	}

	template < typename T >
	PSX_INLINE VOID Array< T >::PushBack( const T &obj )
	{
		if ( m_size >= m_capacity )
			Reserve( m_capacity * 2 + 1 );

		m_pArray[m_size++] = obj;
	}

	template < typename T >
	PSX_INLINE VOID Array< T >::PopBack( VOID )
	{
		PSX_Assert( m_size > 0, "No more items to pop." );
		--m_size;
	}

	template < typename T >
	const Array< T > & Array< T >::operator = ( const Array  &rhs )
	{
		if ( this != &rhs )
		{
			PSX_SafeDeleteArray( m_pArray );

			m_size = rhs.m_size;
			m_capacity = rhs.m_capacity;
			m_pArray = new T[ rhs.m_capacity ];

			PSX_Assert( m_pArray, "Failed to allocate memory" );

			for ( SIZE_T i = 0; i < m_size; ++i )
				m_pArray[i] = rhs[i];
		}

		return *this;
	}

	template < typename T >
	PSX_INLINE VOID Array< T >::Resize( SIZE_T size )
	{
		if ( size > m_capacity )
			Reserve( size );

		m_size = size;
	}

	template < typename T >
	VOID Array< T >::Reserve( SIZE_T capacity )
	{
		T *pOldArray = m_pArray;
		SIZE_T numToCopy = ( capacity < m_size ) ? capacity : m_size;

		m_pArray = new T[capacity];
		PSX_Assert( m_pArray, "Failed to allocate new memory." );

		for ( SIZE_T i = 0; i < numToCopy; ++i )
			m_pArray[i] = pOldArray[i];

		delete [] pOldArray;

		m_size = numToCopy;
		m_capacity = capacity;
	}

	template < typename T >
	VOID Array<T>::Remove( const T &obj )
	{
		SIZE_T i = 0;

		for ( ; i < m_size; ++i )
		{
			if ( m_pArray[i] != obj )
				continue;
			else
			{
				for ( ++i; i < m_size; ++i )
					m_pArray[i - 1] = m_pArray[i];

				--m_size;
				
				break;
			}
		}

	}

	template < typename T >
	VOID Array<T>::Remove( SIZE_T index )
	{
		PSX_Assert( index < m_size, "Invalid index." );

		if ( index != m_size - 1 )
			PSX_MemCopyPerByte( m_pArray + index, m_pArray + index + 1, (m_size - index - 1) * sizeof(T) );
	}

	template < typename T >
	PSX_FORCE_INLINE typename Array< T >::Iterator Array< T >::IteratorBegin( void )
	{
		return Iterator( this, 0 );
	}

	template < typename T >
	PSX_FORCE_INLINE typename Array< T >::ConstIterator Array< T >::IteratorBegin( void ) const
	{
		return m_pArray;
	}

	template < typename T >
	PSX_FORCE_INLINE typename Array< T >::Iterator Array< T >::IteratorEnd( void )
	{
		return Iterator( this, GetSize() );
	}

	template < typename T >
	PSX_FORCE_INLINE typename Array< T >::ConstIterator Array< T >::IteratorEnd( void ) const
	{
		return Iterator( this, GetSize() );
	}
};