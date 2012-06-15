/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	String.cpp
*
*	Comments	-	See String.h
*
**************************************************************************************/

#include "../Include/String.h"
#include <stdarg.h>
#include <cstdio>

namespace Pulse
{
	PSX_REGISTRY_OBJECT_START( String, ERegistryPriority::Normal, ERegistryPriority::Normal )
		PSX_REGISTRY_ON_TEST()
		{
			return EErrorCode::NONE;

			String str1 = PSX_String("Hello World.");
			PSX_Assert( str1.GetLength() == 12, "Length should be 12." );

			SIZE_T index = str1.FindFirstOf( PSX_String("l") );
			PSX_Assert( index == 2, "Index should be 2." );

			index = str1.FindFirstNotOf( PSX_String("Hello World.") );
			PSX_Assert( index == PSX_NPOS, "Should be PSX_NPOS." );

			index = str1.FindFirstNotOf( PSX_String("Hel") );
			PSX_Assert( index == 4, "index should be 4." );

			String str2 = str1.SubString( index );
			PSX_Assert( PSX_StrCmp( str2.GetBuffer(), PSX_String("o World.") ) == 0, "Substring method failed." );

			String str3;
			str3.Format( PSX_String("Hello crab %i"), 10 );
			PSX_Assert( PSX_StrCmp( str3.GetBuffer(), PSX_String("Hello crab 10") ) == 0, "String::Format() failed testing." );

			index = str3.Find( PSX_String("crab") );
			PSX_Assert( index == 6, "'index' should be 6." );

			index = str3.Find( PSX_String("crab  ") );
			PSX_Assert( index == PSX_NPOS, "'index' should be PSX_NPOS." );

			str3 = PSX_String( "First First" );

			index = str3.Find( PSX_String("First") );
			PSX_Assert( index == 0, "'index' should be 0." );

			index = str3.Find( PSX_String("First"), 1 );
			PSX_Assert( index == 6, "'index' should be 6." );

			return EErrorCode::NONE;
		}
	PSX_REGISTRY_OBJECT_END();


	String String::SubString( const SIZE_T start, const SIZE_T count ) const
	{
		PSX_Assert( start + count <= m_length, "Characters to copy is over than string length." );
	
		if ( start + count <= m_length )
		{
			SIZE_T newCount = count;

			// If count is 0, copy until the end of string
			if ( count == 0 )
				newCount = m_length - start;

			String newString;
			UINT newSize = newCount + 1; // +1 for null at the end
			CHAR *pNewBuffer = new CHAR[newSize]; 

			CHAR *pPtr = m_pBuffer + start;
			PSX_StrCpy( pNewBuffer, pPtr, newCount );
			pNewBuffer[ newCount ] = NULL;

			PSX_SafeDelete( newString.m_pBuffer );
			newString.m_length = count;
			newString.m_bufferLength = newSize;
			newString.m_pBuffer = pNewBuffer;

			return newString;
		}
		else
			return String();
	}
	
	SIZE_T String::Find( const CHAR *pStr, const SIZE_T off ) const
	{
		PSX_Assert( off < m_length, "Invalid 'off' parameter." );
		PSX_Assert( pStr, "Invalid 'pStr' parameter." );

		SIZE_T strLen = PSX_StrLen( pStr );
		SIZE_T index = off;
		SIZE_T subIndex;
		SIZE_T i;

		for ( ; index < m_length; ++index )
		{
			// Is the current index same character in the first element of pStr?
			if ( m_pBuffer[index] == *pStr )
			{
				// Now continue comparing until the end of pStr
				for ( subIndex = index + 1, i = 1; i < strLen; ++i, ++subIndex )
				{
					// We don't want to go buffer-overrun.
					if ( subIndex >= m_length )
						return PSX_NPOS;

					if ( m_pBuffer[subIndex] != pStr[i] )
						break;
				}

				if ( i == strLen )
					return index;
			}
		}

		return PSX_NPOS;
	}

	SIZE_T String::FindFirstOf( const CHAR ch, const SIZE_T off ) const
	{
		PSX_Assert( off < m_length, "Invalid off parameter." );

		SIZE_T currPos = off;

		for ( ; currPos < m_length; ++currPos )
		{
			if ( m_pBuffer[currPos] == ch )
				return currPos;
		}

		return PSX_NPOS;
	}

	SIZE_T String::FindFirstOf( const CHAR *pStr, const SIZE_T off ) const
	{
		PSX_Assert( off < m_length, "Invalid off parameter." );

		SIZE_T currPos = off;
		SIZE_T checkIter = 0;
		CHAR letterToCheck;

		for ( ; currPos < m_length; ++currPos )
		{
			letterToCheck = m_pBuffer[currPos];

			while ( pStr[checkIter] != PSX_NULL )
			{
				if ( letterToCheck == pStr[checkIter] )
					return currPos;

				++checkIter;
			}

			checkIter = 0;
		}

		return PSX_NPOS;
	}

	SIZE_T String::FindFirstNotOf( const CHAR ch, const SIZE_T off ) const
	{
		PSX_Assert( off < m_length, "Invalid off parameter." );

		SIZE_T currPos = off;

		for ( ; currPos < m_length; ++currPos )
		{
			if ( m_pBuffer[currPos] != ch )
				return currPos;
		}

		return PSX_NPOS;
	}

	SIZE_T String::FindFirstNotOf( const CHAR *pStr, const SIZE_T off ) const
	{
		PSX_Assert( off < m_length, "Invalid off parameter." );

		SIZE_T currPos = off;
		SIZE_T checkIter = 0;
		BOOL bFoundInList = FALSE; // This tells if we've hit a character that's in pStr
		CHAR letterToCheck;

		for ( ; currPos < m_length; ++currPos )
		{
			letterToCheck = m_pBuffer[currPos];

			while ( pStr[checkIter] != PSX_NULL )
			{
				if ( letterToCheck == pStr[checkIter] )
				{
					bFoundInList = TRUE;
					break;
				}

				++checkIter;
			}

			if ( bFoundInList == FALSE )
				return currPos;

			checkIter = 0;
			bFoundInList = FALSE;
		}

		return PSX_NPOS;
	}

	SIZE_T String::FindLastOf( const CHAR ch, const SIZE_T offFromLast ) const
	{
		PSX_Assert( m_pBuffer, "This string is empty." );
		PSX_Assert( offFromLast < m_length, "Specified offFromLast parameter invalid." );

		SIZE_T iter = m_length - offFromLast - 1; // Start one character before null.

		while ( iter < m_length )
		{
			if ( m_pBuffer[iter] == ch )
				return iter;

			if ( iter > 0 )
				--iter;
			else
				break;
		}

		return PSX_NPOS;
	}
	
	SIZE_T String::FindLastOf( const CHAR *pStr, const SIZE_T offFromLast ) const
	{
		PSX_Assert( m_pBuffer, "This string is empty." );
		PSX_Assert( offFromLast < m_length, "Specified offFromLast parameter invalid." );

		SIZE_T iter = m_length - offFromLast - 1; // Start one character before null.
		SIZE_T checkIter = 0;
		BOOL bFoundInList = FALSE; // This tells if we've hit a character that's in pStr
		CHAR letterToCheck;

		while ( iter < m_length )
		{
			letterToCheck = m_pBuffer[iter];

			while ( pStr[checkIter] != PSX_NULL )
			{
				if ( letterToCheck == pStr[checkIter] )
					return iter;

				++checkIter;
			}

			checkIter = 0;

			if ( iter > 0 )
				--iter;
			else
				break;
		}

		return PSX_NPOS;
	}

	SIZE_T String::FindLastNotOf( const CHAR ch, const SIZE_T offFromLast ) const
	{
		PSX_Assert( m_pBuffer, "This string is empty." );
		PSX_Assert( offFromLast < m_length, "Specified offFromLast parameter invalid." );

		SIZE_T iter = m_length - offFromLast - 1; // Start one character before null.

		while ( iter < m_length )
		{
			if ( m_pBuffer[iter] != ch )
				return iter;

			if ( iter > 0 )
				--iter;
			else
				break;
		}

		return PSX_NPOS;
	}

	SIZE_T String::FindLastNotOf( const CHAR *pStr, const SIZE_T offFromLast ) const
	{
		PSX_Assert( m_pBuffer, "This string is empty." );
		PSX_Assert( offFromLast < m_length, "Specified offFromLast parameter invalid." );

		SIZE_T iter = m_length - offFromLast - 1; // Start one character before null.
		SIZE_T checkIter = 0;
		BOOL bFoundInList = FALSE; // This tells if we've hit a character that's in pStr
		CHAR letterToCheck;

		while ( iter < m_length )
		{
			letterToCheck = m_pBuffer[iter];

			while ( pStr[checkIter] != PSX_NULL )
			{
				if ( letterToCheck == pStr[checkIter] )
				{
					bFoundInList = TRUE;
					break;
				}

				++checkIter;
			}

			if ( bFoundInList == FALSE )
				return iter;

			bFoundInList = FALSE;
			checkIter = 0;

			if ( iter > 0 )
				--iter;
			else
				break;
		}

		return PSX_NPOS;
	}

/* -- http://www.cplusplus.com/reference/clibrary/cstdio/sprintf/ --

pFormat
		Pointer to an array of char elements where the resulting C string is stored.
format(...)
	C string that contains the text to be written to the buffer.
	It can optionally contain embedded format tags that are substituted by the values specified in subsequent argument(s) and formatted as requested.
	The number of arguments following the format parameters should at least be as much as the number of format tags.
	The format tags follow this prototype:

	%[flags][width][.precision][length]specifier

specifier					Output													Example
	c			Character																a
	d or i		Signed decimal integer													392
	e			Scientific notation (mantise/exponent) using e character				3.9265e+2
	E			Scientific notation (mantise/exponent) using E character				3.9265E+2
	f			Decimal floating point													392.65
	g			Use the shorter of %e or %f												392.65
	G			Use the shorter of %E or %f												392.65
	o			Signed octal															610
	s			String of characters													sample
	u			Unsigned decimal integer												7235
	x			Unsigned hexadecimal integer											7fa
	X			Unsigned hexadecimal integer (capital letters)							7FA
	p			Pointer address															B800:0000
	n			Nothing printed. The argument must be a pointer to a signed int, where 
					the number of characters written so far is stored.	
	%			A % followed by another % character will write % to the string.

flags						description
	-			Left-justify within the given field width; Right justification is the default (see width sub-specifier).
	+			Forces to preceed the result with a plus or minus sign (+ or -) even for positive numbers. By default, only negative numbers are preceded with a - sign.
	(space)		If no sign is going to be written, a blank space is inserted before the value.
	#			Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X respectively for values different than zero.
					Used with e, E and f, it forces the written output to contain a decimal point even if no digits would follow. By default, if no digits follow, no decimal point is written.
					Used with g or G the result is the same as with e or E but trailing zeros are not removed.
	0			Left-pads the number with zeroes (0) instead of spaces, where padding is specified (see width sub-specifier).

width							description
	(number)	Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger.
	*			The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.


.precision						description
	.number		For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.
					For e, E and f specifiers: this is the number of digits to be printed after the decimal point.
					For g and G specifiers: This is the maximum number of significant digits to be printed.
					For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.
					For c type: it has no effect.
					When no precision is specified, the default is 1. If the period is specified without an explicit value for precision, 0 is assumed.
	.*			The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.


length	description
	h			The argument is interpreted as a short int or unsigned short int (only applies to integer specifiers: i, d, o, u, x and X).
	l			The argument is interpreted as a long int or unsigned long int for integer specifiers (i, d, o, u, x and X), and as a wide character or wide character string for specifiers c and s.
	L			The argument is interpreted as a long double (only applies to floating point specifiers: e, E, f, g and G).	
*/
	const CHAR * String::Format( const CHAR *pFormat, ... )
	{
		static const INT BUFFER_SIZE = 2048;

		CHAR buffer[BUFFER_SIZE];
		va_list argPtr;

		PSX_Assert( pFormat, "pFormat string can't be null." );

		if ( pFormat != NULL )
		{
			INT n;

			// Parse the variable argument list
			va_start( argPtr, pFormat );
#if defined(PSX_UNICODE)
			n = vswprintf( buffer, pFormat, argPtr );
#else
			n = vsprintf( buffer, pFormat, argPtr );
#endif
			va_end( argPtr );

			Copy( buffer, n );
		}

		return GetBuffer();
	}

	String String::ToUnicode( const Char *pStr )
	{
		return String(pStr);
	}

	const Char * String::ToMultiByte( const CHAR *pString, Char *pBuffer, SIZE_T bufferSize )
	{
	
		WideCharToMultiByte(CP_ACP, 0, pString, -1, pBuffer, bufferSize, NULL, NULL);

		return pBuffer;
	}

	const String & String::operator = ( const CHAR *pStr )
	{
		if ( pStr == m_pBuffer )
			return *this;
		
		m_length = PSX_StrLen( pStr );
		
		// Do we need to delete then recreate the buffer?
		if ( m_length >= m_bufferLength )
		{
			m_bufferLength = m_length + 1;

			if ( m_pBuffer )
				delete [] m_pBuffer;

			m_pBuffer = new CHAR[ m_bufferLength ];
			PSX_Assert( m_pBuffer, "Failed to allocate memory." );
		}

		// No need to delete and recreate buffer if its big enough
		PSX_MemCopy( m_pBuffer, pStr, m_length );
		m_pBuffer[m_length] = NULL;

		return *this;
	}

	const String & String::operator += ( const String &rhs )
	{

		if ( !rhs.GetLength() )
			return *this;

		CHAR *pTempBuff;
		UINT totalLength;

		pTempBuff = m_pBuffer;
		totalLength = m_length + rhs.m_length;

		if ( totalLength + 1 > m_bufferLength )
		{
			m_bufferLength = totalLength + 1;

			m_pBuffer = new CHAR[ m_bufferLength ];
			PSX_Assert( m_pBuffer, "Failed to allocate memory for m_pBuffer." );

			PSX_MemCopy( m_pBuffer, pTempBuff, m_length );
		}

		PSX_MemCopy( m_pBuffer + m_length, rhs.m_pBuffer, rhs.m_length );
		m_length = totalLength;
		m_pBuffer[m_length] = NULL;

		if ( pTempBuff )
			delete [] pTempBuff;

		return *this;

	}

	const String & String::operator += ( const CHAR *pStr )
	{
		CHAR *pTempBuff;
		UINT strLen;
		UINT totalLength;

		PSX_Assert( pStr, "rhs has an invalid string buffer." );

		pTempBuff = m_pBuffer;
		strLen = PSX_StrLen( pStr );
		totalLength = m_length + strLen;

		if ( totalLength + 1 > m_bufferLength )
		{
			m_bufferLength = totalLength + 1;

			m_pBuffer = new CHAR[ m_bufferLength ];
			PSX_Assert( m_pBuffer, "Failed to allocate memory for m_pBuffer." );

			PSX_MemCopy( m_pBuffer, pTempBuff, m_length );
		}

		PSX_MemCopy( m_pBuffer + m_length, pStr, strLen );
		m_length = totalLength;
		m_pBuffer[m_length] = NULL;

		if ( pTempBuff )
			delete [] pTempBuff;

		return *this;
	}

	
	SHORT String::ToSHORT( void ) const
	{
		SHORT ret;
		PSX_StringToSHORT( *this, ret );
		return ret;
	}

	USHORT String::ToUSHORT( void ) const
	{
		USHORT ret;
		PSX_StringToUSHORT( *this, ret );
		return ret;
	}

	INT String::ToINT( void ) const
	{
		INT ret;
		PSX_StringToINT( *this, ret );
		return ret;
	}

	UINT String::ToUINT( void ) const
	{
		UINT ret;
		PSX_StringToUINT( *this, ret );
		return ret;
	}

	FLOAT String::ToFLOAT( void ) const
	{
		FLOAT ret;
		PSX_StringToFLOAT( *this, ret );
		return ret;
	}

	DOUBLE String::ToDOUBLE( void ) const
	{
		DOUBLE ret;
		PSX_StringToDOUBLE( *this, ret );
		return ret;
	}

	void String::Copy( const CHAR *pStr )
	{
		if ( pStr == m_pBuffer )
			return;

		SIZE_T len = PSX_StrLen( pStr );
		Copy( pStr, len );
	}

	void String::Copy( const CHAR *pStr, SIZE_T len )
	{
		if ( pStr == m_pBuffer )
			return;

		m_length = len;

		// Do we need to delete then recreate the buffer?
		if ( m_length >= m_bufferLength )
		{
			m_bufferLength = m_length + 1;

			if ( m_pBuffer )
				delete [] m_pBuffer;

			m_pBuffer = new CHAR[ m_bufferLength ];
			PSX_Assert( m_pBuffer, "Failed to allocate memory." );
		}

		// No need to delete and recreate buffer if its big enough
		PSX_MemCopy( m_pBuffer, pStr, m_length );
		m_pBuffer[m_length] = NULL;

	}

};