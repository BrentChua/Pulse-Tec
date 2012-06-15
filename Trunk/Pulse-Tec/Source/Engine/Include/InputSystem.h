/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	HID.h
*
*	Description -	A system to abstract platform specific HIDs.
*
*	Comments	-	Contains InputSystem to manage HID objects in use.
*					TODO: Abstract device interfaces needs to support input listeners.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/11/2010	-	Creation of this file
*			MrCodeSushi	-	02/12/2010	-	InputSystem class
*			MrCodeSushi	-	02/13/2010	-	InputDevicesLayer class with Keyboard DInput implementation.
*			MrCodeSushi	-	02/15/2010	-	Concrete MouseDeviceDX implemented.
**************************************************************************************/

#ifndef _PSX_HUMAN_INTERFACE_DEVICE_H_
#define _PSX_HUMAN_INTERFACE_DEVICE_H_

#include "PulseSTD.h"
#include "Singleton.h"
#include "ISystem.h"
#include "NonCopyable.h"
#include "IRefCounter.h"
#include "Allocators.h"
#include "Array.h"
#include "Queue.h"
#include "EventManager.h"
#include "AppMsg.h"

namespace Pulse
{
	PSX_EnumBegin( EInputDeviceType )
		KEYBOARD,
		MOUSE,
	PSX_EnumEnd()

	class InputSystem : public Singleton<InputSystem>
	{	
	public:

		enum HID_TYPE { 
			HID_KEYBOARD, 
			HID_MOUSE 
		};
		enum HID_FLAGS { 
			FLAG_BACKGROUND		= 0x01,
			FLAG_EXCLUSIVE		= 0x02, 
			FLAG_FOREGROUND		= 0x04, 
			FLAG_NONEXCLUSIVE	= 0x08, 
			FLAG_NOWINKEY		= 0x10, // Windows specific
			FLAG_FORCE_DWORD	= 0x7FFFFFFF
		};

		virtual ~InputSystem( void ) { };

		IDevice * GetDevice( EInputDeviceType::Type devType );

	private:

		friend Singleton;

		friend Singleton;
		friend class Engine;
		friend class WindowApplication;
		friend class IDevice;
		
		BOOL Initialize( SIZE_T devicePtrPoolSize = 5 );
		void Shutdown( void );

		// NOTE: Now used internally to accomodate the listener-dispatcher design pattern
		class IDevice * GetDeviceInterface( HID_TYPE type, I32 flag );

		void Update( void );

		// Invalidates all allocated devices resizes DevicePtrPool
		void Reset( SIZE_T devicePtrPoolSize );
		void RemoveDevice( class IDevice *pDevice );

		friend class IDevice;
		friend void _PrivateRemoveDevice( class IDevice *pDevice );

		InputSystem( void );

		void QueueInputMessage( AppMsg &msg );
		void DispatchMessages( void );

		typedef FixedBlockAllocator< sizeof(class IDevice *) > DevicePtrPool;
		typedef Array<IDevice*> DeviceList;
		typedef Queue<AppMsg>	MessageQueue;
		
		DevicePtrPool	m_devicePtrPool;
		class IDevice	*m_pAllocDevList;
		BOOL			m_bInitialized;
		DeviceList		m_deviceList;
		EventManager	m_dispatcher;
		MessageQueue	m_messages;
	
	};

	// This abstracts ALL code specific HIDs.
	class InputDevicesLayer
	{
	private:

		// Only the InputSystem can access this static class.
		friend InputSystem;

		static BOOL Initialize( void );
		static void Shutdown( void );

		static IDevice * CreateDevice( InputSystem::HID_TYPE type, I32 flags = 0 );

	private:

	};

	class IDevice : NonCopyable, public IRefCounter
	{
	public:

		virtual InputSystem::HID_TYPE GetDeviceType( void ) = 0;
		
		virtual void Poll( void ) = 0;

	protected:

		friend InputSystem;

		virtual void DestroySelf( void ) = 0;

		virtual ~IDevice( void ) { }

		void QueueInputMessage( AppMsg &msg ) { InputSystem::GetInstance()->QueueInputMessage( msg ); }

	private:

		friend InputSystem;

		// All devices requires a pointer to the next device to for a list.
		// This is used internally by the InputSystem.
		IDevice *m_pNext;
	};

#ifdef PSX_PLATFORM_WINDOWS_WINDOW
	
		class IKeyboardDevice : public IDevice
		{
		public:

			virtual BOOL IsKeyDown( I32 key ) = 0;

			virtual BOOL IsKeyUp( I32 key ) = 0;

			virtual BOOL IsBufferedKeyDown( I32 key ) = 0;

			virtual BOOL IsBufferedKeyUp( I32 key ) = 0;

			virtual InputSystem::HID_TYPE GetDeviceType( void ) { return InputSystem::HID_KEYBOARD; }
		
		protected:

			virtual ~IKeyboardDevice( void ) { }

			friend class InputSystem;
		};

		// KEYBOARD KEY VALUES
		// WARNING: THESE ARE HARD CODED VALUES COPIED FROM dinput.h.
		//	Make sure to update these values if the ones in DirectInput changes.
		#define PIK_ESCAPE          0x01
		#define PIK_1               0x02
		#define PIK_2               0x03
		#define PIK_3               0x04
		#define PIK_4               0x05
		#define PIK_5               0x06
		#define PIK_6               0x07
		#define PIK_7               0x08
		#define PIK_8               0x09
		#define PIK_9               0x0A
		#define PIK_0               0x0B
		#define PIK_MINUS           0x0C    /* - on main keyboard */
		#define PIK_EQUALS          0x0D
		#define PIK_BACK            0x0E    /* backspace */
		#define PIK_TAB             0x0F
		#define PIK_Q               0x10
		#define PIK_W               0x11
		#define PIK_E               0x12
		#define PIK_R               0x13
		#define PIK_T               0x14
		#define PIK_Y               0x15
		#define PIK_U               0x16
		#define PIK_I               0x17
		#define PIK_O               0x18
		#define PIK_P               0x19
		#define PIK_LBRACKET        0x1A
		#define PIK_RBRACKET        0x1B
		#define PIK_RETURN          0x1C    /* Enter on main keyboard */
		#define PIK_LCONTROL        0x1D
		#define PIK_A               0x1E
		#define PIK_S               0x1F
		#define PIK_D               0x20
		#define PIK_F               0x21
		#define PIK_G               0x22
		#define PIK_H               0x23
		#define PIK_J               0x24
		#define PIK_K               0x25
		#define PIK_L               0x26
		#define PIK_SEMICOLON       0x27
		#define PIK_APOSTROPHE      0x28
		#define PIK_GRAVE           0x29    /* accent grave */
		#define PIK_LSHIFT          0x2A
		#define PIK_BACKSLASH       0x2B
		#define PIK_Z               0x2C
		#define PIK_X               0x2D
		#define PIK_C               0x2E
		#define PIK_V               0x2F
		#define PIK_B               0x30
		#define PIK_N               0x31
		#define PIK_M               0x32
		#define PIK_COMMA           0x33
		#define PIK_PERIOD          0x34    /* . on main keyboard */
		#define PIK_SLASH           0x35    /* / on main keyboard */
		#define PIK_RSHIFT          0x36
		#define PIK_MULTIPLY        0x37    /* * on numeric keypad */
		#define PIK_LMENU           0x38    /* left Alt */
		#define PIK_SPACE           0x39
		#define PIK_CAPITAL         0x3A
		#define PIK_F1              0x3B
		#define PIK_F2              0x3C
		#define PIK_F3              0x3D
		#define PIK_F4              0x3E
		#define PIK_F5              0x3F
		#define PIK_F6              0x40
		#define PIK_F7              0x41
		#define PIK_F8              0x42
		#define PIK_F9              0x43
		#define PIK_F10             0x44
		#define PIK_NUMLOCK         0x45
		#define PIK_SCROLL          0x46    /* Scroll Lock */
		#define PIK_NUMPAD7         0x47
		#define PIK_NUMPAD8         0x48
		#define PIK_NUMPAD9         0x49
		#define PIK_SUBTRACT        0x4A    /* - on numeric keypad */
		#define PIK_NUMPAD4         0x4B
		#define PIK_NUMPAD5         0x4C
		#define PIK_NUMPAD6         0x4D
		#define PIK_ADD             0x4E    /* + on numeric keypad */
		#define PIK_NUMPAD1         0x4F
		#define PIK_NUMPAD2         0x50
		#define PIK_NUMPAD3         0x51
		#define PIK_NUMPAD0         0x52
		#define PIK_DECIMAL         0x53    /* . on numeric keypad */
		#define PIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
		#define PIK_F11             0x57
		#define PIK_F12             0x58
		#define PIK_F13             0x64    /*                     (NEC PC98) */
		#define PIK_F14             0x65    /*                     (NEC PC98) */
		#define PIK_F15             0x66    /*                     (NEC PC98) */
		#define PIK_KANA            0x70    /* (Japanese keyboard)            */
		#define PIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
		#define PIK_CONVERT         0x79    /* (Japanese keyboard)            */
		#define PIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
		#define PIK_YEN             0x7D    /* (Japanese keyboard)            */
		#define PIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
		#define PIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
		#define PIK_PREVTRACK       0x90    /* Previous Track (PIK_CIRCUMFLEX on Japanese keyboard) */
		#define PIK_AT              0x91    /*                     (NEC PC98) */
		#define PIK_COLON           0x92    /*                     (NEC PC98) */
		#define PIK_UNDERLINE       0x93    /*                     (NEC PC98) */
		#define PIK_KANJI           0x94    /* (Japanese keyboard)            */
		#define PIK_STOP            0x95    /*                     (NEC PC98) */
		#define PIK_AX              0x96    /*                     (Japan AX) */
		#define PIK_UNLABELED       0x97    /*                        (J3100) */
		#define PIK_NEXTTRACK       0x99    /* Next Track */
		#define PIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
		#define PIK_RCONTROL        0x9D
		#define PIK_MUTE            0xA0    /* Mute */
		#define PIK_CALCULATOR      0xA1    /* Calculator */
		#define PIK_PLAYPAUSE       0xA2    /* Play / Pause */
		#define PIK_MEDIASTOP       0xA4    /* Media Stop */
		#define PIK_VOLUMEDOWN      0xAE    /* Volume - */
		#define PIK_VOLUMEUP        0xB0    /* Volume + */
		#define PIK_WEBHOME         0xB2    /* Web home */
		#define PIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
		#define PIK_DIVIDE          0xB5    /* / on numeric keypad */
		#define PIK_SYSRQ           0xB7
		#define PIK_RMENU           0xB8    /* right Alt */
		#define PIK_PAUSE           0xC5    /* Pause */
		#define PIK_HOME            0xC7    /* Home on arrow keypad */
		#define PIK_UP              0xC8    /* UpArrow on arrow keypad */
		#define PIK_PRIOR           0xC9    /* PgUp on arrow keypad */
		#define PIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
		#define PIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
		#define PIK_END             0xCF    /* End on arrow keypad */
		#define PIK_DOWN            0xD0    /* DownArrow on arrow keypad */
		#define PIK_NEXT            0xD1    /* PgDn on arrow keypad */
		#define PIK_INSERT          0xD2    /* Insert on arrow keypad */
		#define PIK_DELETE          0xD3    /* Delete on arrow keypad */
		#define PIK_LWIN            0xDB    /* Left Windows key */
		#define PIK_RWIN            0xDC    /* Right Windows key */
		#define PIK_APPS            0xDD    /* AppMenu key */
		#define PIK_POWER           0xDE    /* System Power */
		#define PIK_SLEEP           0xDF    /* System Sleep */
		#define PIK_WAKE            0xE3    /* System Wake */
		#define PIK_WEBSEARCH       0xE5    /* Web Search */
		#define PIK_WEBFAVORITES    0xE6    /* Web Favorites */
		#define PIK_WEBREFRESH      0xE7    /* Web Refresh */
		#define PIK_WEBSTOP         0xE8    /* Web Stop */
		#define PIK_WEBFORWARD      0xE9    /* Web Forward */
		#define PIK_WEBBACK         0xEA    /* Web Back */
		#define PIK_MYCOMPUTER      0xEB    /* My Computer */
		#define PIK_MAIL            0xEC    /* Mail */
		#define PIK_MEDIASELECT     0xED    /* Media Select */

		/*
		 *  Alternate names for keys, to facilitate transition from DOS.
		 */
		#define PIK_BACKSPACE       PIK_BACK            /* backspace */
		#define PIK_NUMPADSTAR      PIK_MULTIPLY        /* * on numeric keypad */
		#define PIK_LALT            PIK_LMENU           /* left Alt */
		#define PIK_CAPSLOCK        PIK_CAPITAL         /* CapsLock */
		#define PIK_NUMPADMINUS     PIK_SUBTRACT        /* - on numeric keypad */
		#define PIK_NUMPADPLUS      PIK_ADD             /* + on numeric keypad */
		#define PIK_NUMPADPERIOD    PIK_DECIMAL         /* . on numeric keypad */
		#define PIK_NUMPADSLASH     PIK_DIVIDE          /* / on numeric keypad */
		#define PIK_RALT            PIK_RMENU           /* right Alt */
		#define PIK_UPARROW         PIK_UP              /* UpArrow on arrow keypad */
		#define PIK_PGUP            PIK_PRIOR           /* PgUp on arrow keypad */
		#define PIK_LEFTARROW       PIK_LEFT            /* LeftArrow on arrow keypad */
		#define PIK_RIGHTARROW      PIK_RIGHT           /* RightArrow on arrow keypad */
		#define PIK_DOWNARROW       PIK_DOWN            /* DownArrow on arrow keypad */
		#define PIK_PGDN            PIK_NEXT            /* PgDn on arrow keypad */

		/*
		 *  Alternate names for keys originally not used on US keyboards.
		 */
		#define PIK_CIRCUMFLEX      PIK_PREVTRACK       /* Japanese keyboard */


		class IMouseDevice : public IDevice
		{
		public:

			virtual INT GetXPos( void ) = 0;
			virtual INT GetYPos( void ) = 0;
			virtual INT GetZPos( void ) = 0;

			virtual INT GetDeltaX( void ) = 0;
			virtual INT GetDeltaY( void ) = 0;
			virtual INT GetDeltaZ( void ) = 0;

			virtual BOOL IsButtonDown( INT button ) = 0;
			virtual BOOL IsButtonUp( INT button ) = 0;

			virtual BOOL IsBufferedButtonDown( INT button ) = 0;
			virtual BOOL IsBufferedButtonUp( INT button ) = 0;

			virtual InputSystem::HID_TYPE GetDeviceType( void ) { return InputSystem::HID_MOUSE; }

		protected:

			virtual ~IMouseDevice( void ) { }

			friend class InputSystem;

		};

		#define PIM_LBUTTON 0 /* Left Mouse Button */
		#define PIM_RBUTTON 1 /* Right Mouse Button */
		#define PIM_MBUTTON 2 /* Middle Mouse Button */
		#define PIM_BUTTON3 3 /* 3rd button */
		#define PIM_BUTTON4 4 /* 4th button */
		#define PIM_BUTTON5 5 /* 5th button */
		#define PIM_BUTTON6 6 /* 6th button */
		#define PIM_BUTTON7 7 /* 7th button */
		
#elif defined( PSX_PLATFORM_WINDOWS_CONSOLE )
#else
	#error No Human-Interface-Device found.
#endif /* PSX_PLATFORM_WINDOWS_WINDOW */

}

#endif /* _PSX_HUMAN_INTERFACE_DEVICE_H_ */