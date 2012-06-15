/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConsoleInputDeviceWin32.cpp
*
*	Comments	-	See ConsoleInputDeviceWin32.h
*
**************************************************************************************/
#include "../Include/ConsoleInputDeviceWin32.h"

#ifdef PSX_PLATFORM_WINDOWS_CONSOLE

namespace Pulse
{

	BOOL ConsoleInputDeviceListener::IsHitLetterKey( KeyboardEvent *pEvent )
	{
		if(( pEvent->key >=65 && pEvent->key <= 90) || pEvent->key == VK_OEM_7)
			return true;
		return false;
	}

	BOOL ConsoleInputDeviceListener::IsHitBackKey( KeyboardEvent *pEvent )
	{
		if(pEvent->key == VK_BACK)
			return true;
		return false;
	}

	BOOL ConsoleInputDeviceListener::IsHitReturnKey( KeyboardEvent *pEvent )
	{
		if(pEvent->key == VK_RETURN)
			return true;
		return false;
	}

	ConsoleInputDevice::ConsoleInputDevice( void )
	{

	}

	ConsoleInputDevice::~ConsoleInputDevice( void )
	{

	}

	EErrorCode::Type ConsoleInputDevice::Initialize( void )
	{
		m_hInput = GetStdHandle( STD_INPUT_HANDLE );
		//m_hOutput = GetStdHandle( STD_OUTPUT_HANDLE );

		SetConsoleMode( m_hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT );

		return EErrorCode::NONE;
	}

	void ConsoleInputDevice::Update( void )
	{
		DWORD events = 0;

		GetNumberOfConsoleInputEvents( m_hInput, &events );

		if ( events == 0 )
			return;

		INPUT_RECORD inputRecord;

		ReadConsoleInput( m_hInput, &inputRecord, 1, &events );

		if ( inputRecord.EventType == MOUSE_EVENT )
		{
			MouseEvent event;

			event.x = inputRecord.Event.MouseEvent.dwMousePosition.X;
			event.y = inputRecord.Event.MouseEvent.dwMousePosition.Y;
			event.buttonState = (EMouseButtonState::Type)inputRecord.Event.MouseEvent.dwButtonState;

			m_pListener->OnMouseEvent( &event );
		}
		else if ( inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown )
		{
			KeyboardEvent event;
			event.key = inputRecord.Event.KeyEvent.wVirtualKeyCode;

			m_pListener->OnKeyboardEvent( &event );
		}

	}

}

#endif /* PSX_PLATFORM_WINDOWS_CONSOLE */