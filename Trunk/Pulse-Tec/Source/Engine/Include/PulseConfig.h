/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PulseConfig.h
*
*	Description -	Contains configurations and configuration macros.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/28/2009		-	Created PulseConfig file.
**************************************************************************************/

#ifndef _PULSECONFIG_H_
#define _PULSECONFIG_H_

// NOTE: Pulse Engine has a custom memory manager. Set to 0 to turn off custom Memory Manager.
#define PSX_USE_MEMORY_MANAGER 0

#define PSX_DISABLE_ZLIB_MAINS 1

#if defined( _UNICODE )
	#include <wchar.h> // Wide charater set
	#define PSX_UNICODE 1
#endif

#if defined( _DEBUG )
	#define PSX_DEBUG 1
#else
	#define PSX_RELEASE 1
#endif

#if defined( _WIN32 ) || defined( _WIN64 ) // Windows

	#define PSX_PLATFORM_WINDOWS	1

	#define WIN32_LEAN_AND_MEAN
	#define WIN32_EXTRA_LEAN
	#include <windows.h>	

	// Disable warnings about unsafe std functions...
	#pragma warning(disable : 4996)
	// warning C4985: 'operator new[]': attributes not present on previous declaration.
	#pragma warning(disable : 4985)


	// Console or a window application
	#if defined(__PSX_WINDOWS_CONSOLE) 
		#define PSX_PLATFORM_WINDOWS_CONSOLE
	#elif defined(__PSX_WINDOWS_WINDOW)
		#define PSX_PLATFORM_WINDOWS_WINDOW
	#endif
	
#endif

#endif _PULSECONFIG_H_