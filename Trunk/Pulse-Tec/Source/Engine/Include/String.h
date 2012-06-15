/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	String.h
*
*	Description -	String class.
*
*	Comments	-	TODO: We need to implement an iterator.
*					THERE IS A BUG! Don't go changing a subscript in the array less
*						than the length to null. It will invalidate the length!!!
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/28/2009	-	Creation of String class
**************************************************************************************/

#ifndef _PULSE_STRING_H_
#define _PULSE_STRING_H_
	
#include "PulseSTD.h"
#include <stdio.h>
#include <sstream>

namespace Pulse
{
	class String
	{
	public:

		String( VOID );
		String( const String &str );
		String( const CHAR *pStr );

#if defined(PSX_UNICODE)
		String( const Char *pStr );
#endif
		~String( VOID );

		const CHAR * GetBuffer( VOID ) const;

		const SIZE_T GetLength( VOID) const;

		// NOTE: Copies a sub-string out of the original string.
		//	If count is 0, it copies until the end of the string.
		String SubString( const SIZE_T start, const SIZE_T count = 0 ) const;

		SIZE_T Find( const CHAR ch, const SIZE_T off = 0 ) const;
		SIZE_T Find( const CHAR *pStr, const SIZE_T off = 0 ) const;

		SIZE_T FindFirstOf( const CHAR ch, const SIZE_T off = 0 ) const;
		SIZE_T FindFirstOf( const CHAR *pStr, const SIZE_T off = 0 ) const;

		SIZE_T FindFirstNotOf( const CHAR ch, const SIZE_T off = 0 ) const;
		SIZE_T FindFirstNotOf( const CHAR *pStr, const SIZE_T off = 0 ) const;

		// NOTE: These aren't implemented yet...
		SIZE_T FindLastOf( const CHAR ch, const SIZE_T offFromLast = 0 ) const;
		SIZE_T FindLastOf( const CHAR *pStr, const SIZE_T offFromLast = 0 ) const;

		SIZE_T FindLastNotOf( const CHAR ch, const SIZE_T offFromLast = 0 ) const;
		SIZE_T FindLastNotOf( const CHAR *pStr, const SIZE_T offFromLast = 0 ) const;

		// NOTE: See definition for more info.
		const CHAR * Format( const CHAR *pFormat, ... );

		BOOL IsEmpty( void );

		void Clear( void );

		const String & operator = ( const CHAR *pStr );
		const String & operator = ( const String &str );

		const String & operator += ( const String &rhs );
		const String & operator += ( const CHAR *pStr );

		CHAR operator [] ( UINT i ) const;
		CHAR & operator [] ( UINT i );

		/* Friend functions */
		friend const String operator + ( const String &lhs, const String &rhs );
		friend const String operator + ( const String &lhs, const CHAR *rhs );
		friend BOOL operator == ( const String &lhs, const String &rhs );
		friend BOOL operator == ( const String &lhs, const CHAR *rhs );
		friend BOOL operator != ( const String &lhs, const String &rhs );
		friend BOOL operator < ( const String &lhs, const String &rhs );
		friend BOOL operator <= ( const String &lhs, const String &rhs );
		friend BOOL operator > ( const String &lhs, const String &rhs );
		friend BOOL operator >= ( const String &lhs, const String &rhs );

		// Some useful utilities
		template < typename T >
		static String ToString( const T &t );

		template < >
		static String ToString<BOOL>( const BOOL &t );

		template < typename T >
		static BOOL StringTo( const String &str, T &val );
		
		template < typename T >
		static BOOL StringTo( const CHAR *pStr, T &val );

		template < >
		static BOOL StringTo<BOOL>( const String &str, BOOL &val );

		template < >
		static BOOL StringTo<BOOL>( const CHAR *pStr, BOOL &val );

		template < >
		static BOOL StringTo<const CHAR*>( const String &str, const CHAR * &val );

		// NOTE: Well... This is already a const CHAR why would we want it to convert to the same thing?
		//template < >
		//static BOOL StringTo<const CHAR*>( const CHAR *pStr, const CHAR * &val );

		static String ToUnicode( const Char *pStr );

		static const Char * ToMultiByte( const CHAR *pString, Char *pBuffer, SIZE_T bufferSize );

		SHORT ToSHORT( void ) const;

		USHORT ToUSHORT( void ) const;

		INT ToINT( void ) const;

		UINT ToUINT( void ) const;

		FLOAT ToFLOAT( void ) const;

		DOUBLE ToDOUBLE( void ) const;

	private:

		void Copy( const CHAR *pStr );
		void Copy( const CHAR *pStr, SIZE_T len );
		
		SIZE_T	m_length;
		SIZE_T	m_bufferLength;
		CHAR	*m_pBuffer;
	};

	/* Inlined Implementations */
	
	PSX_INLINE String::String( VOID ) : m_length( 0 ), m_bufferLength( 0 ), m_pBuffer( 0 )
	{
		// m_pBuffer needs to contain something!
		*this = PSX_String("");
	}

	PSX_INLINE String::String( const String &str ) : m_length( 0 ), m_bufferLength( 0 ), m_pBuffer( 0 )
	{
		*this = str;
	}

	PSX_INLINE String::String( const CHAR *pStr ) : m_length( 0 ), m_bufferLength( 0 ), m_pBuffer( 0 )
	{
		*this = pStr;
	}

#if defined(PSX_UNICODE)
	PSX_INLINE String::String( const Char *pStr ) : m_length( 0 ), m_bufferLength( 0 ), m_pBuffer( 0 )
	{
		// Convert ANSI to Unicode
		size_t len = strlen( pStr ) + 1;
		CHAR *pBuffer = new CHAR[len];
		MultiByteToWideChar( CP_ACP, PSX_NULL, pStr, -1, pBuffer, len );

		*this = pBuffer;

		delete [] pBuffer;
	}
#endif

	PSX_INLINE String::~String( VOID )
	{
		if ( m_pBuffer )
			delete [] m_pBuffer;
	}

	PSX_INLINE const CHAR * String::GetBuffer( VOID ) const
	{
		return m_pBuffer;
	}

	PSX_INLINE const SIZE_T String::GetLength( VOID) const
	{
		return m_length;
	}

	PSX_INLINE SIZE_T String::Find( const CHAR ch, const SIZE_T off ) const
	{
		return FindFirstOf( ch, off );
	}

	PSX_INLINE BOOL String::IsEmpty( void )
	{
		return m_length == 0;
	}

	PSX_INLINE void String::Clear( void )
	{
		m_length = 0;
		m_pBuffer[0] = PSX_NULL;
	}

	PSX_INLINE const String & String::operator = ( const String &str )
	{
		if ( this == &str )
			return *this;

		return (*this = str.m_pBuffer);
	}

	PSX_INLINE CHAR String::operator [] ( UINT i ) const
	{
		PSX_Assert( ( i >= 0 ) && ( i < m_length ), "Index Out Of Bound." );
		return m_pBuffer[i];
	}

	PSX_INLINE CHAR & String::operator [] ( UINT i )
	{
		PSX_Assert( ( i >= 0 ) && ( i < m_length ), "Index Out Of Bound." );
		return m_pBuffer[i];
	}

	/* Friend Functions */
	PSX_INLINE const String operator + ( const String &lhs, const String &rhs )
	{
		String newStr( lhs );
		newStr += rhs;
		return newStr;
	}

	PSX_INLINE const String operator + ( const String &lhs, const CHAR *rhs )
	{
		String newStr( lhs );
		newStr += rhs;
		return newStr;
	}

	PSX_INLINE BOOL operator == ( const String &lhs, const String &rhs )
	{
		PSX_Assert( lhs.GetBuffer() && rhs.GetBuffer(), "lhs or rhs is invalid." );
		return !PSX_StrCmp( lhs.GetBuffer(), rhs.GetBuffer() );
	}

	PSX_INLINE BOOL operator == ( const String &lhs, const CHAR *rhs )
	{
		PSX_Assert( lhs.GetBuffer() && rhs, "lhs or rhs is invalid." );
		return !PSX_StrCmp( lhs.GetBuffer(), rhs );
	}

	PSX_INLINE BOOL operator != ( const String &lhs, const String &rhs )
	{
		PSX_Assert( lhs.GetBuffer() && rhs.GetBuffer(), "lhs or rhs is invalid." );
		return PSX_StrCmp( lhs.GetBuffer(), rhs.GetBuffer() ) != 0;
	}

	PSX_INLINE BOOL operator < ( const String &lhs, const String &rhs )
	{
		PSX_Assert( lhs.GetBuffer() && rhs.GetBuffer(), "lhs or rhs is invalid." );
		return ( PSX_StrCmp( lhs.GetBuffer(), rhs.GetBuffer() ) < 0 );
	}

	PSX_INLINE BOOL operator <= ( const String &lhs, const String &rhs )
	{
		PSX_Assert( lhs.GetBuffer() && rhs.GetBuffer(), "lhs or rhs is invalid." );
		return ( PSX_StrCmp( lhs.GetBuffer(), rhs.GetBuffer() ) <= 0 );
	}
	PSX_INLINE BOOL operator > ( const String &lhs, const String &rhs )
	{
		PSX_Assert( lhs.GetBuffer() && rhs.GetBuffer(), "lhs or rhs is invalid." );
		return ( PSX_StrCmp( lhs.GetBuffer(), rhs.GetBuffer() ) > 0 );
	}

	PSX_INLINE BOOL operator >= ( const String &lhs, const String &rhs )
	{
		PSX_Assert( lhs.GetBuffer() && rhs.GetBuffer(), "lhs or rhs is invalid." );
		return ( PSX_StrCmp( lhs.GetBuffer(), rhs.GetBuffer() ) >= 0 );
	}

	template < typename T >
	PSX_INLINE String String::ToString( const T &t )
	{
#if defined(PSX_UNICODE)
		std::wstringstream ss;
#else
		std::stringstream ss;
#endif
		ss << t;
		return ss.str().c_str();
	}

	template < >
	PSX_INLINE String String::ToString<BOOL>( const BOOL &val )
	{
		return val ? PSX_String("true") : PSX_String("false");
	}

	template < typename T >
	PSX_INLINE BOOL String::StringTo( const String &str, T &val )
	{
		return StringTo<T>(str.GetBuffer(), val);
	}

	template < typename T >
	PSX_INLINE BOOL String::StringTo( const CHAR *pStr, T &val )
	{
#if defined(PSX_UNICODE)
		std::wistringstream iss( pStr );
#else
		std::istringstream iss( pStr );
#endif
		iss >> val;
		return !iss.fail() && iss.eof();
	}

	template < >
	BOOL String::StringTo<BOOL>( const String &str, BOOL &val )
	{
		if ( str == PSX_String("1") || str == PSX_String("true" ) )
		{
			val = TRUE;
			return TRUE;
		}
		else if ( str == PSX_String("0") || str == PSX_String("false" ) )
		{
			val = FALSE;
			return FALSE;
		}
		else
			return FALSE;
	}

	template < >
	BOOL String::StringTo<BOOL>( const CHAR *pStr, BOOL &val )
	{
		if ( PSX_StrCmp( pStr, PSX_String("1") ) == 0 || PSX_StrCmp( pStr, PSX_String("true") ) == 0 )
		{
			val = TRUE;
			return TRUE;
		}
		else if ( PSX_StrCmp( pStr, PSX_String("0") ) == 0 || PSX_StrCmp( pStr, PSX_String("false") ) == 0 )
		{
			val = FALSE;
			return FALSE;
		}
		else
			return FALSE;
	}

	template < >
	PSX_INLINE BOOL String::StringTo<const CHAR*>( const String &str, const CHAR * &val )
	{
		val = str.GetBuffer();
		return TRUE;
	}

};

#endif _PULSE_STRING_H_