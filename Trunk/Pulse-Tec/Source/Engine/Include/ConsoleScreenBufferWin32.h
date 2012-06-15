/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConsoleScreenBufferWin32.h
*
*	Description -	Handy class that encapsulates all drawigng code to the console screen buffer.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/13/2011	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_CONSOLE_SCREEN_BUFFER_WIN32_H_
#define _PSX_CONSOLE_SCREEN_BUFFER_WIN32_H_

#include <windows.h>

#include "PulseSTD.h"

namespace Pulse
{
	struct EConsoleAttribute 
	{
		enum Type
		{
			FG_BLUE		 	= 0x00000001,
			FG_GREEN	 	= 0x00000002,
			FG_RED		 	= 0x00000004,
			FG_INTENSITY 	= 0x00000008,
			BG_BLACK	 	= 0x00000010,
			BG_BLUE		 	= 0x00000020,
			BG_GREEN	 	= 0x00000040,
			BG_RED		 	= 0x00000080,
			BG_INTENSITY 	= 0x00000100,
			REVERSE_VIDEO	= 0x00000200,
		};
	};

	class ScreenBuffer
	{
	public:

		ScreenBuffer( void );

		~ScreenBuffer( void );

		EErrorCode::Type Initialize( void );

		void Shutdown( void );

		void SetText( POS_T x, POS_T y, CHAR ch );
		void SetAttribute( POS_T x, POS_T y, INT attr );
		void Set( POS_T x, POS_T y, CHAR ch, INT attr );

		void DrawString( POS_T x, POS_T y, const CHAR *pStr );

		void ClearBuffer( void );
		void DisplayBuffer( void );

	private:

		HANDLE						m_hOutput;
		CONSOLE_SCREEN_BUFFER_INFO	m_screenInfo;     // To store the info of the running command prompt screen
		CHAR_INFO					*m_pScreenBuffer;

	};
}

#endif /* _PSX_CONSOLE_SCREEN_BUFFER_WIN32_H_ */