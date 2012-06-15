/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConsoleScreenBufferWin32.cpp
*
*	Comments	-	See ConsoleScreenBufferWin32.h
*
**************************************************************************************/

#ifdef __PSX_WINDOWS_CONSOLE

#include "../Include/ConsoleScreenBufferWin32.h"
#include "../Include/String.h"

namespace Pulse
{

	ScreenBuffer::ScreenBuffer( void )
		: m_hOutput( PSX_NULL ), m_pScreenBuffer( PSX_NULL )
	{
		
	}

	ScreenBuffer::~ScreenBuffer( void )
	{
		if ( m_hOutput == PSX_NULL )
			Shutdown();
	}

	EErrorCode::Type ScreenBuffer::Initialize( void )
	{
		m_hOutput = GetStdHandle( STD_OUTPUT_HANDLE );

		if ( m_hOutput == PSX_NULL )
		{
			PSX_PushLog1( "Failed to get console output handle." );
			return EErrorCode::GRAPHICS;
		}

		if ( GetConsoleScreenBufferInfo(m_hOutput, &m_screenInfo) == 0 )
		{
			PSX_PushLog1( "Failed to get console screen info." );
			return EErrorCode::GRAPHICS;
		}

		if ( (m_pScreenBuffer = new CHAR_INFO[m_screenInfo.srWindow.Right * m_screenInfo.srWindow.Bottom]) == PSX_NULL )
		{
			PSX_PushLog1( "Failed to create screen buffer." );
			return EErrorCode::GRAPHICS;
		}

		ClearBuffer();
		
		return EErrorCode::NONE;
	}

	void ScreenBuffer::Shutdown( void )
	{
		m_hOutput = PSX_NULL;
		delete [] m_pScreenBuffer;
		m_pScreenBuffer = PSX_NULL;
	}

	void ScreenBuffer::SetText( POS_T x, POS_T y, CHAR ch )
	{
		POS_T index = x + y * m_screenInfo.srWindow.Right;
		PSX_Assert( index >= 0 && (index < m_screenInfo.dwSize.X * m_screenInfo.dwSize.Y), "Invalid X/Y position" );
		m_pScreenBuffer[index].Char.AsciiChar = ch;
	}

	void ScreenBuffer::SetAttribute( POS_T x, POS_T y, INT attr )
	{
		POS_T index = x + y * m_screenInfo.srWindow.Right;
		int attribute = 0;
		PSX_Assert( index >= 0 && (index < m_screenInfo.dwSize.X * m_screenInfo.dwSize.Y), "Invalid X/Y position" );

		if ( attr & EConsoleAttribute::FG_BLUE ) 
			attribute |= FOREGROUND_BLUE;
		if ( attr & EConsoleAttribute::FG_GREEN ) 
			attribute |= FOREGROUND_GREEN;
		if ( attr & EConsoleAttribute::FG_RED ) 
			attribute |= FOREGROUND_RED;
		if ( attr & EConsoleAttribute::FG_INTENSITY ) 
			attribute |= FOREGROUND_INTENSITY;
		if ( attr & EConsoleAttribute::BG_BLUE ) 
			attribute |= BACKGROUND_BLUE;
		if ( attr & EConsoleAttribute::BG_GREEN ) 
			attribute |= BACKGROUND_GREEN;
		if ( attr & EConsoleAttribute::BG_RED ) 
			attribute |= BACKGROUND_RED;
		if ( attr & EConsoleAttribute::BG_INTENSITY ) 
			attribute |= BACKGROUND_INTENSITY;
		if ( attr & EConsoleAttribute::REVERSE_VIDEO ) 
			attribute |= COMMON_LVB_REVERSE_VIDEO;

		m_pScreenBuffer[index].Attributes = attribute;
	}

	void ScreenBuffer::Set( POS_T x, POS_T y, CHAR ch, INT attr )
	{
		SetText( x, y, ch );
		SetAttribute( x, y, attr );
	}

	void ScreenBuffer::DrawString( POS_T x, POS_T y, const CHAR *pStr )
	{
		SIZE_T len = PSX_StrLen( pStr );

		for ( UINT i = 0; i < len; ++i )
			SetText( x + i, y, pStr[i] );
	}

	void ScreenBuffer::ClearBuffer( void )
	{
		PSX_ZeroMem( m_pScreenBuffer, sizeof(CHAR_INFO) * m_screenInfo.srWindow.Right * m_screenInfo.srWindow.Bottom );
	}

	void ScreenBuffer::DisplayBuffer( void )
	{
		COORD zeroZero = {0,0};
		COORD bufferSize = {m_screenInfo.srWindow.Right,m_screenInfo.srWindow.Bottom};
		SMALL_RECT rect = m_screenInfo.srWindow;
		int ret = WriteConsoleOutput(m_hOutput,m_pScreenBuffer,
			bufferSize,zeroZero,
			&rect);

		if ( ret == 0 )
		{
		
			DWORD dw = GetLastError();
			int x = 0;
		}
	}

}

#endif /* __PSX_WINDOWS_CONSOLE */