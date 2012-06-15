/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	WindowBaseApplication.cpp
*
*	Comments	-	See WindowBaseApplication.h
*
**************************************************************************************/

#include <shlobj.h>
#include <string.h>
#include "../Include/WindowsBaseApplication.h"
#include "../Include/String.h"

namespace Pulse
{
	const CHAR * WindowsBaseApplication::GetApplicationDataDirectory( const CHAR *appDirectory )
	{
		HRESULT hr;

		static CHAR applicationDataDirectory[PSX_MAX_PATH];
		CHAR userDataPath[PSX_MAX_PATH];

		hr = SHGetSpecialFolderPath( NULL, userDataPath, CSIDL_APPDATA, TRUE );

		PSX_StrCpy( applicationDataDirectory, userDataPath, PSX_MAX_PATH );
		PSX_StrCat( applicationDataDirectory, PSX_String("/") );
		PSX_StrCat( applicationDataDirectory, appDirectory );

		// Does our directory exist?
		if ( 0xffffffff == GetFileAttributes( applicationDataDirectory ) )
		{
			// Directory doesn't exist. Manually make the directory ourselves.
			CHAR current[PSX_MAX_PATH];
			CHAR myAppData[PSX_MAX_PATH];

			PSX_StrCpy( current, userDataPath, PSX_MAX_PATH );
			PSX_StrCpy( myAppData, appDirectory, PSX_MAX_PATH );

			CHAR token[PSX_MAX_PATH];
			token[0] = PSX_String('\0');

			do
			{
				CHAR *pLeft = PSX_StrChr( myAppData, PSX_String('\\') );
				if ( pLeft == NULL )
				{
					PSX_StrCpy( token, myAppData, PSX_MAX_PATH );
					myAppData[0] = PSX_String('\0');
				}
				else
				{
					PSX_StrCpy( token, myAppData, pLeft - myAppData );
					token[pLeft-myAppData] = PSX_String('\0');
					PSX_StrCpy( myAppData, pLeft+1, PSX_MAX_PATH );
				}

				if ( PSX_StrLen( token ) )
				{
					PSX_StrCat( current, PSX_String("/") );
					PSX_StrCat( current, token );

					if ( FALSE == CreateDirectory( current, NULL ) )
					{
						int error = GetLastError();
						if (error != ERROR_ALREADY_EXISTS)
						{
							return NULL;
						}
					}
				}
			} while( PSX_StrLen( myAppData ) );
		}

		PSX_StrCat( applicationDataDirectory, PSX_String("/") );
		
		return applicationDataDirectory;
	}
};