/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConsoleInputDeviceWin32.h
*
*	Description -	Console input device.
*
*	Comments	-	Objects listen to this device to receive keyboard mouse input.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/29/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_CONSOLE_KEYBOARD_INPUT_WIN32_H_
#define _PSX_CONSOLE_KEYBOARD_INPUT_WIN32_H_

#include "PulseSTD.h"
#include <windows.h>

namespace Pulse
{
	PSX_EnumBegin( EMouseButtonState )
		LEFT	= FROM_LEFT_1ST_BUTTON_PRESSED,
		RIGHT	= RIGHTMOST_BUTTON_PRESSED,
		BUTTON_2 = FROM_LEFT_2ND_BUTTON_PRESSED,
		BUTTON_3 = FROM_LEFT_3RD_BUTTON_PRESSED,
		BUTTON_4 = FROM_LEFT_4TH_BUTTON_PRESSED,
	PSX_EnumEnd()

	struct MouseEvent
	{
		SHORT x, y;
		EMouseButtonState::Type buttonState;
	};

	struct KeyboardEvent
	{
		SHORT key;
	};

	class ConsoleInputDeviceListener
	{
	public:

		virtual void OnMouseEvent( MouseEvent *pEvent ) = 0;

		virtual void OnKeyboardEvent( KeyboardEvent *pEvent ) = 0;

		virtual ~ConsoleInputDeviceListener( void ) { }

	protected:

		// Helper methods
		BOOL IsHitLetterKey( KeyboardEvent *pEvent );
		BOOL IsHitBackKey( KeyboardEvent *pEvent );
		BOOL IsHitReturnKey( KeyboardEvent *pEvent );

	};

	class ConsoleInputDevice
	{
	public:

		ConsoleInputDevice( void );
		
		~ConsoleInputDevice( void );

		EErrorCode::Type Initialize( void );

		void SetListener( ConsoleInputDeviceListener *pListener ) { m_pListener = pListener; }

		void Update( void );

	private:

		typedef ConsoleInputDeviceListener Listener;

		HANDLE m_hInput;
		HANDLE m_hOutput;

		// HACK: Currently only accepts one listener.
		//	Let it listen to more listeneres.
		Listener *m_pListener;

	};
}

#endif /* _PSX_CONSOLE_KEYBOARD_INPUT_WIN32_H_ */