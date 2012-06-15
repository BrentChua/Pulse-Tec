/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConsoleApplication.cpp
*
*	Comments	-	See ConsoleApplication.h
*
**************************************************************************************/

#include "../Include/ConsoleApplicationWin32.h"
#include "../Include/SmartPointer.h"
#include "../Include/Math.h"
#include "../Include/MemoryManager.h"

#if defined( PSX_PLATFORM_WINDOWS_CONSOLE )

using namespace Pulse;

namespace Pulse
{
	extern Pulse::IApplication * PSX_OnCreateApplication( void );
	extern void PSX_OnReleaseApplication( Pulse::IApplication *pApp );
}

// Windows main console entry point
int main( int argNum, char **ppArguments )
{
	//int tmpDbgFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
	//tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	//_CrtSetDbgFlag(tmpDbgFlag);

	// User sets the Application object
	//IApplication *pApp = NULL;
	ConsoleApplication *pApp = dynamic_cast<ConsoleApplication *>(PSX_OnCreateApplication());
	PSX_Assert( pApp, "Application pointer is invalid(NULL)." );
	Pulse::INT ret = pApp->Run( argNum, ppArguments );
	PSX_OnReleaseApplication( pApp );
	
	//_CrtDumpMemoryLeaks();

	return ret;
}

namespace Pulse
{
	INT ConsoleApplication::Run( INT argNum, CHAR **ppArguments )
	{
		INT ret;

		IApplication::InitializeSystems();

		// Initialize screen buffer
		m_inputDevice.Initialize();
		m_screenBuffer.Initialize();

		ret =  Main( argNum, ppArguments );

		m_screenBuffer.Shutdown();

		IApplication::ShutdownSystems();


		return ret;
	}

	void ConsoleApplication::Update( void )
	{
		m_inputDevice.Update();
	}

};

#endif /* defined( PSX_PLATFORM_WINDOWS_CONSOLE ) */