/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Application.h
*
*	Description -	Includes the proper platform specific application class.
*
*	Comments	-	Either ConsoleApplication or WindowApplication will be included.
*					Applications needs to define PSX_OnSetApplication().
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/17/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_APPLICATION_H_
#define _PSX_APPLICATION_H_

#include "PulseSTD.h"
#include "IApplication.h"

namespace Pulse
{
	// NOTE: User needs to define these two functions to create user derived application.
	Pulse::IApplication * PSX_OnCreateApplication( void );
	void PSX_OnReleaseApplication( Pulse::IApplication *pApp );

	// NOTE: Macros defined application can use instead of manually typing the definitions.
	#define PSX_REGISTER_APPLICATION_CREATE( _appName )		\
		IApplication * PSX_OnCreateApplication( void )	\
		{													\
			return new _appName;							\
		}													

	#define PSX_REGISTER_APPLICATION_RELEASE()					\
		void PSX_OnReleaseApplication( IApplication *pApp )	\
		{														\
			delete pApp;										\
		}
}

#if defined( PSX_PLATFORM_WINDOWS )
	#include "WindowsBaseApplication.h"
	#if defined( PSX_PLATFORM_WINDOWS_WINDOW )
		#include "WindowApplicationWin32.h"
	#elif defined( PSX_PLATFORM_WINDOWS_CONSOLE )
		#include "ConsoleApplicationWin32.h"
	#else
		#error No windows application type to include. Make sure PulseConfig.h is set to include the proper application type.
	#endif
#else
	#error Unsopported platform.
#endif

namespace Pulse
{
};

#endif /* _PSX_APPLICATION_H_ */