/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	FileIO.cpp
*
*	Comments	-	See FileIO.h
*
**************************************************************************************/

#include "../Include/FileIO.h"
#include "../Include/String.h"

namespace Pulse
{
	PSX_REGISTRY_OBJECT_START( FileIO, ERegistryPriority::Normal, ERegistryPriority::BelowNormal )
		PSX_REGISTRY_ON_TEST()
		{
			int x = 0;
		}

	PSX_REGISTRY_OBJECT_END()

	BOOL FileIO::Open( const CHAR *pFileName, FILEOP options )
	{
		Close();

		m_fileOP = options;
		CHAR mode[15];

		if ( (options & FILEOP_READ) && (options & FILEOP_WRITE) )
		{
			if ( options & FILEOP_TRUNCATE )
				PSX_StringFormat( mode, PSX_String("w+") );
			else if ( options & FILEOP_APPENED )
				PSX_StringFormat( mode, PSX_String("a+") );
			else
				PSX_StringFormat( mode, PSX_String("r+") );		// File must exist
		}
		else if ( options & FILEOP_READ )
			PSX_StringFormat( mode, PSX_String("r") );			// File must exist
		else if ( options & FILEOP_WRITE )
		{
			if ( options & FILEOP_APPENED )
				PSX_StringFormat( mode, PSX_String("a") );
			else
				PSX_StringFormat( mode, PSX_String("w") );
		}

		if ( options & FILEOP_BINARY )
			PSX_StrCat( mode + PSX_StrLen( mode ), PSX_String("b") );

		#if defined( PSX_UNICODE )
			m_pFStream = _wfopen( pFileName, mode );
		#else
			m_pFStream = fopen( pFileName, mode ); 
		#endif /* defined( PSX_UNICODE ) */

		if ( !m_pFStream )
			return FALSE;

		if ( options & FILEOP_GOTO_END )
			Seek( 0, SEEKOP_END );

		return true;
	}

	const CHAR * FileIO::GetLine( String &str )
	{
		CHAR pBuffer[ 1024 ];
		CHAR *pRet;
		SIZE_T startSize = str.GetLength();
		SIZE_T newLength;

		str.Clear();
		
		// We want to continue reading until we've reached a newline
		do {
			
#if defined(PSX_UNICODE)
			pRet = fgetws( pBuffer, sizeof(pBuffer), m_pFStream );
#else
			pRet = fgets( pBuffer, sizeof(pBuffer), m_pFStream );
#endif
			if ( pRet )
				str = str + pRet;
			else
				break;

			newLength = str.GetLength();

		} while ( newLength && str[newLength  - 1] != PSX_NEWLINE );
		
		return startSize != newLength ? str.GetBuffer() : NULL;
	}

}