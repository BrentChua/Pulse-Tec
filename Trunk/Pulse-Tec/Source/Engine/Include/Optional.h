/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Optional.h
*
*	Description -	Formalizes return values whether it is valid or not.
*
*	Comments	-	Optional template class stores a boolean value to determine its
*						validity. And the returned data, if valid.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/29/2009		-	File creation
**************************************************************************************/

#ifndef _PSX_OPTIONAL_H_
#define _PSX_OPTIONAL_H_

#include "PulseSTD.h"

namespace Pulse
{
	class OptionalEmpty { };

	template < typename T >
	class Optional
	{
	public:

		Optional( void );
		Optional( T const & obj );
		Optional( Optional const & obj );
		Optional( OptionalEmpty const & );
		~Optional( void );

		Optional & operator = ( T const & rhs );
		Optional & operator = ( Optional const & rhs );
		BOOL const operator == ( Optional const & rhs );
		BOOL const operator < ( Optional const & rhs );
		BOOL const operator > ( Optional const & rhs );

		// Data accessors
		T const & operator * ( void ) const;
		T & operator * ( void );
		T const * const operator -> ( void ) const;
		T * const operator -> ( void );

		void Clear( void );

		BOOL const IsValid( void ) const;
		BOOL const IsInvalid( void ) const;

	private:

		// Utility Functions
		T const * const GetT( void ) const;
		T * const GetT( void );
		void ConstructData( T const & obj );
		void DestroyData( void );

		BOOL m_bValid;
		char m_data[ sizeof( T ) ];

	};

	template < typename T >
	Optional< T >::Optional( void )
		: m_bValid( FALSE )
	{
	}

	template < typename T >
	Optional< T >::Optional( T const & obj )
		: m_bValid( TRUE )
	{
		ConstructData( obj );
	}

	template < typename T >
	Optional< T >::Optional( Optional const & obj )
		: m_bValid( FALSE )
	{
		if ( obj.IsValid() )
		{
			ConstructData( *obj );
			m_bValid = TRUE;
		}
	}

	template < typename T >
	Optional< T >::Optional( OptionalEmpty const & )
		: m_bValid( FALSE )
	{
	}

	template < typename T >
	Optional< T >::~Optional( void )
	{
		if ( IsValid() )
			DestroyData();
	}

	template < typename T >
	Optional<T> & Optional< T >::operator = ( T const & rhs )
	{
		if ( m_bValid )
		{
			*GetT() = rhs;
		}
		else
		{
			ConstructData( rhs );
			m_bValid = TRUE;
		}

		return *this;
	}

	template < typename T >
	Optional<T> & Optional< T >::operator = ( Optional const & rhs )
	{
		if (this == &rhs)
			return *this;

		if ( IsValid() )
		{
			DestroyData();
			m_bValid = FALSE;
		}

		if ( rhs.IsValid() )
		{
			ConstructData( *rhs );
			m_bValid = TRUE;
		}

		return *this;
	}

	template < typename T >
	BOOL const Optional< T >::operator == ( Optional const & rhs )
	{
		if ( !IsValid() && !rhs.IsValid() )
			return TRUE;
		if ( IsValid() ^ rhs.IsValid() )
			return FALSE;

		return **this == *rhs;
	}

	template < typename T >
	BOOL const Optional< T >::operator < ( Optional const & rhs )
	{
		if ( !IsValid() && !rhs.IsValid() )
			return FALSE;

		if ( !IsValid() )
			return TRUE;

		if ( rhs.IsValid() )
			return FALSE;

		return **this < *rhs;
	}

	template < typename T >
	BOOL const Optional< T >::operator > ( Optional const & rhs )
	{
		return !(*this < rhs);
	}

	// Data accessors
	template < typename T >
	T const & Optional< T >::operator * ( void ) const
	{
		PSX_Assert( IsValid(), "Optional data not valid." );
		return *GetT();
	}

	template < typename T >
	T & Optional< T >::operator * ( void )
	{
		PSX_Assert( IsValid(), "Optional data not valid." );
		return *GetT();
	}

	template < typename T >
	T const * const Optional< T >::operator -> ( void ) const
	{
		PSX_Assert( IsValid(), "Optional data not valid." );
		return GetT();
	}

	template < typename T >
	T * const Optional< T >::operator -> ( void )
	{
		PSX_Assert( IsValid(), "Optional data not valid." );
		return GetT();
	}

	template < typename T >
	void Optional< T >::Clear( void )
	{
		if ( IsValid() )
		{
			m_bValid = FALSE;
			DestroyData();
		}
	}

	template < typename T >
	BOOL const Optional< T >::IsValid( void ) const
	{
		return m_bValid;
	}

	template < typename T >
	BOOL const Optional< T >::IsInvalid( void ) const
	{
		return !IsValid();
	}

	template < typename T >
	T const * const Optional< T >::GetT( void ) const
	{
		return reinterpret_cast< T const * const >(m_data);
	}

	template < typename T >
	T * const Optional< T >::GetT( void )
	{
		return reinterpret_cast< T * const >(m_data);
	}

	template < typename T >
	void Optional< T >::ConstructData( T const & obj )
	{
		new (GetT()) T(obj);
	}

	template < typename T >
	void Optional< T >::DestroyData( void )
	{
		GetT()->~T();
	}
}

#endif /* _PSX_OPTIONAL_H_ */