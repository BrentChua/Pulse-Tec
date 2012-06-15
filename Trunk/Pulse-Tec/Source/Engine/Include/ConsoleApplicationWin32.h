/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConsoleApplication.h
*
*	Description -	Base abstract console application layer class.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/17/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_CONSOLE_APPLICATION_H_
#define _PSX_CONSOLE_APPLICATION_H_

#include "PulseSTD.h"
#include "WindowsBaseApplication.h"
#include "ConsoleScreenBufferWin32.h"
#include "ConsoleInputDeviceWin32.h"

int main( int argNum, char **ppArguments );

namespace Pulse
{

	class ConsoleApplication: public WindowsBaseApplication
	{
	public:
		
		ConsoleApplication( void ) { }
		virtual ~ConsoleApplication( void ) { }


	protected:

		friend int ::main( INT argNum, CHAR **ppArguments );

		INT Run( INT argNum, CHAR **ppArguments );

		virtual INT Main( INT argNum, CHAR **ppArgs ) = 0;

		void Update( void );

		//// Basic event callbacks
		//virtual BOOL OnInitialize( void );
		//virtual BOOL OnStart( void );
		//virtual void OnTick( void );
		//virtual BOOL OnExit ( void );

	protected:

		ScreenBuffer		m_screenBuffer;
		ConsoleInputDevice	m_inputDevice;

	};

	//PSX_INLINE ConsoleApplication::ConsoleApplication( void )
	//	:	m_bRunning( FALSE )
	//{

	//}

	//PSX_INLINE BOOL ConsoleApplication::OnInitialize( void ) 
	//{
	//	m_bRunning = TRUE;
	//	return TRUE;
	//}

	//PSX_INLINE BOOL ConsoleApplication::OnStart( void )
	//{
	//	return TRUE;
	//}

	//PSX_INLINE void ConsoleApplication::OnTick( void )
	//{
	//	m_bRunning = FALSE;
	//}

	//PSX_INLINE BOOL ConsoleApplication::OnExit ( void )
	//{
	//	return TRUE;
	//}

}

#endif /* _PSX_CONSOLE_APPLICATION_H_ */