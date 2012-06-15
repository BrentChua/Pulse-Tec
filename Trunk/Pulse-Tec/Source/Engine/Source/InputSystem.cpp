/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	HID.cpp
*
*	Comments	-	See HID.h
*
**************************************************************************************/

#include "../Include/InputSystem.h"
#include "../Include/AppMsg.h"
#include "../Include/Engine.h"

#ifdef PSX_PLATFORM_WINDOWS_WINDOW
	// TODO: Add XInput here...
	#define DIRECTINPUT_VERSION 0x0800
	#include <dinput.h>
	#include <windows.h>
	#pragma comment(lib, "dinput8.lib" )
	#pragma comment( lib, "dxguid.lib" )
	#include "../Include/WindowApplicationWin32.h"
#endif /* PSX_PLATFORM_WINDOWS_WINDOW */

namespace Pulse
{
	PSX_INLINE void _PrivateRemoveDevice( IDevice *pDevice )
	{
		InputSystem::GetInstance()->RemoveDevice( pDevice );
	}


#ifdef PSX_PLATFORM_WINDOWS_WINDOW
	static IDirectInput8 *m_pDI = NULL;

	class KeyboardDeviceDX : public IKeyboardDevice
	{
	public:

		virtual INT AddRef( void );

		virtual INT Release( void );

		virtual void DestroySelf( void );

		virtual void Poll( void );

		virtual BOOL IsKeyDown( INT key );
	
		virtual BOOL IsKeyUp( INT key );
		
		virtual BOOL IsBufferedKeyDown( INT key );

		virtual BOOL IsBufferedKeyUp( INT key );


	private:

		void ClearKeyMaps( void );
		
		friend InputDevicesLayer;
		
		KeyboardDeviceDX( IDirectInputDevice8 *pKeyboardDevice );
		virtual ~KeyboardDeviceDX( void ) { }

		struct KeyMap 
		{	
			BYTE m_KeyBuffer[256];

			BYTE operator [] ( const SIZE_T i ) const { return m_KeyBuffer[i]; }
			BYTE & operator [] ( const SIZE_T i ) { return m_KeyBuffer[i]; }
			KeyMap operator ^ ( KeyMap &rhs );
			KeyMap operator & ( KeyMap &rhs );
			KeyMap operator ~ ( void );
		};
		// We're wasting so much space...
		struct KeyStates
		{
			KeyStates( void ) : m_pKeyStates(m_states), m_pPrevKeyStates(m_states+1) { }

			void SwapStates( void ) { PSX_Swap( m_pKeyStates, m_pPrevKeyStates ); }

			void FixPointers( void ) { m_pKeyStates = (m_states); m_pPrevKeyStates = (m_states+1); }

			KeyMap m_states[2];
			KeyMap	*m_pKeyStates;
			KeyMap	*m_pPrevKeyStates;
			KeyMap	m_keyDowns;
			KeyMap	m_keyUps;
		};

		IDirectInputDevice8 *m_pKeyboardDevice;
		INT					m_refCount;
		KeyStates			m_keyStates;
		//HANDLE				m_eventHandle;
		//BOOL				m_bUpdateStates;

	};

	PSX_INLINE KeyboardDeviceDX::KeyMap KeyboardDeviceDX::KeyMap::operator ^ ( KeyboardDeviceDX::KeyMap &rhs )
	{
		KeyMap retKeyMap;
		for( SIZE_T count(0), i(0); count < 32; ++count, i = count * 8 ) //  256 / 8 = 32	
			*((I64*)(retKeyMap.m_KeyBuffer + i)) = (*(I64*)(m_KeyBuffer+i)) ^ (*(I64*)(rhs.m_KeyBuffer+i));
		return retKeyMap;
	}

	PSX_INLINE KeyboardDeviceDX::KeyMap KeyboardDeviceDX::KeyMap::operator & ( KeyMap &rhs )
	{
		KeyMap retKeyMap;
		for( SIZE_T count(0), i(0); count < 32; ++count, i = count * 8 ) //  256 / 8 = 32	
			*((I64*)(retKeyMap.m_KeyBuffer + i)) = (*(I64*)(m_KeyBuffer+i)) & (*(I64*)(rhs.m_KeyBuffer+i));
		return retKeyMap;
	}

	PSX_INLINE KeyboardDeviceDX::KeyMap KeyboardDeviceDX::KeyMap::operator ~ ( void )
	{
		KeyMap retKeyMap;
		for( SIZE_T count(0), i(0); count < 32; ++count, i = count * 8 ) //  256 / 8 = 32	
			*((I64*)(retKeyMap.m_KeyBuffer + i)) = ~(*(I64*)(m_KeyBuffer+i));
		return retKeyMap;
	}

	PSX_INLINE KeyboardDeviceDX::KeyboardDeviceDX( IDirectInputDevice8 *pKeyboardDevice )
		:	m_pKeyboardDevice(pKeyboardDevice), m_refCount(1)
	{
		PSX_Assert( pKeyboardDevice, "KeyboardDX device is NULL." );
		ClearKeyMaps();
		//m_bUpdateStates = FALSE;

		//// Create Event
		//m_eventHandle = CreateEvent( NULL, FALSE, FALSE, NULL );
		//m_pKeyboardDevice->SetEventNotification( m_eventHandle );
	}

	PSX_FORCE_INLINE INT KeyboardDeviceDX::AddRef( void )
	{
		++m_refCount;
		return m_refCount;
	}

	PSX_INLINE INT KeyboardDeviceDX::Release( void )
	{
		if ( --m_refCount == 0 )
		{
			m_pKeyboardDevice->Unacquire();
			m_pKeyboardDevice->Release();

			_PrivateRemoveDevice( this );
			delete this;
		}
		return m_refCount;
	}

	void KeyboardDeviceDX::DestroySelf( void )
	{
		while ( m_refCount > 1 )
			Release();

		Release(); // Refcount is one when it gets here. Final Release destroys itself.
	}

	PSX_FORCE_INLINE BOOL KeyboardDeviceDX::IsKeyDown( INT key )
	{
		return ((*m_keyStates.m_pKeyStates)[key] & 0x80) != 0;
	}

	PSX_FORCE_INLINE BOOL KeyboardDeviceDX::IsKeyUp( INT key )
	{
		return ((*m_keyStates.m_pKeyStates)[key] & 0x80) == 0;
	}

	PSX_FORCE_INLINE BOOL KeyboardDeviceDX::IsBufferedKeyDown( INT key )
	{
		return (m_keyStates.m_keyDowns[key] & 0x80) != 0;
	}

	PSX_FORCE_INLINE BOOL KeyboardDeviceDX::IsBufferedKeyUp( INT key )
	{
		return (m_keyStates.m_keyUps[key] & 0x80) != 0;
	}

	void KeyboardDeviceDX::Poll( void )
	{
		static KeyMap prevKeyState = *m_keyStates.m_pKeyStates;
		//DWORD dwResult = WaitForSingleObject( m_eventHandle, 0 );

		if ( /*dwResult == WAIT_OBJECT_0*/ TRUE )
		{
			// get device state
			m_keyStates.SwapStates();
			HRESULT hr = m_pKeyboardDevice->GetDeviceState( sizeof(KeyMap), m_keyStates.m_pKeyStates );

			if ( FAILED( hr ) )
			{
				ClearKeyMaps();
				m_pKeyboardDevice->Acquire();
				return;
			}

			// We really just care about the last bit of BYTE value for each key
			KeyMap keyChanges		= *m_keyStates.m_pKeyStates ^ *m_keyStates.m_pPrevKeyStates;
			m_keyStates.m_keyDowns	= keyChanges & *m_keyStates.m_pKeyStates;
			m_keyStates.m_keyUps	= keyChanges & ~(*m_keyStates.m_pKeyStates);

			//m_bUpdateStates = TRUE;
		}
		else if ( /*m_bUpdateStates*/ FALSE )
		{
			// We really just care about the last bit of BYTE value for each key
			KeyMap keyChanges		= *m_keyStates.m_pKeyStates ^ *m_keyStates.m_pPrevKeyStates;
			m_keyStates.m_keyDowns	= keyChanges & *m_keyStates.m_pKeyStates;
			m_keyStates.m_keyUps	= keyChanges & ~(*m_keyStates.m_pKeyStates);

			//m_bUpdateStates = FALSE;
		}

		// Check for any events to generate
		for ( UINT i = 0; i < 256; ++i )
		{
			InputSystem *pInput = InputSystem::GetInstance();

			if ( prevKeyState[i] != m_keyStates.m_pKeyStates->operator[](i) )
			{
				AppMsg msg;

				// Check if KeyUp or KeyDown
				if ( (prevKeyState[i] & 0x80) != 0 )
					msg.msg = EAppMessage::KEYUP; // Generate Key up
				else
					msg.msg = EAppMessage::KEYDOWN; // Generate key down
				
				msg.bMsg1 = i;
				
				QueueInputMessage( msg );
			}
		}

	}

	PSX_FORCE_INLINE void KeyboardDeviceDX::ClearKeyMaps( void )
	{
		PSX_ZeroMem( &m_keyStates, sizeof(KeyStates) );
		// Fix pointers
		m_keyStates.FixPointers();
	}

	class MouseDeviceDX : public IMouseDevice
	{
	public:

		virtual INT GetXPos( void );
		virtual INT GetYPos( void );
		virtual INT GetZPos( void );
			
		virtual INT GetDeltaX( void );
		virtual INT GetDeltaY( void );
		virtual INT GetDeltaZ( void );

		virtual BOOL IsButtonDown( INT button );
		virtual BOOL IsButtonUp( INT button );

		virtual BOOL IsBufferedButtonDown( INT button );
		virtual BOOL IsBufferedButtonUp( INT button );

		virtual INT AddRef( void );
		virtual INT Release( void );
		virtual void DestroySelf( void );
		virtual void Poll( void );

	private:

		void ClearStateData( void );

		MouseDeviceDX( ::HWND hWnd, LPDIRECTINPUTDEVICE8 pMouseDevice );
		virtual ~MouseDeviceDX( void ) { }

		friend InputDevicesLayer;

		struct MouseState
		{
			DIMOUSESTATE2 m_mouseState;

			MouseState operator ^ ( MouseState &rhs );
			MouseState operator & ( MouseState &rhs );
			MouseState operator ~ ( void );
		};
		struct ButtonStates
		{
			MouseState	m_currMouseState;
			MouseState	m_prevMouseState;
			MouseState	m_buttonDowns;
			MouseState	m_buttonUps;
		};

		ButtonStates			m_mouseStates;
		LPDIRECTINPUTDEVICE8	m_pMouseDevice;
		LONG					m_xPos;
		LONG					m_yPos;
		LONG					m_zPos;
		INT						m_refCount;

		::HWND m_hWnd;
	};

	PSX_INLINE MouseDeviceDX::MouseState MouseDeviceDX::MouseState::operator ^ ( MouseState &rhs )
	{
		MouseState newState;
		*(I64*)(&newState.m_mouseState.lX) = *(I64*)(&m_mouseState.lX) ^ *(I64*)(&rhs.m_mouseState.lX);
		*(I64*)(&newState.m_mouseState.lZ) = *(I64*)(&m_mouseState.lZ) ^ *(I64*)(&rhs.m_mouseState.lZ);
		*(I32*)(&newState.m_mouseState.rgbButtons[4]) = *(I32*)(&m_mouseState.rgbButtons[4]) ^ *(I32*)(&rhs.m_mouseState.rgbButtons[4]);
		return newState;
	}

	PSX_INLINE MouseDeviceDX::MouseState MouseDeviceDX::MouseState::operator & ( MouseState &rhs )
	{
		MouseState newState;
		*(I64*)(&newState.m_mouseState.lX) = *(I64*)(&m_mouseState.lX) & *(I64*)(&rhs.m_mouseState.lX);
		*(I64*)(&newState.m_mouseState.lZ) = *(I64*)(&m_mouseState.lZ) & *(I64*)(&rhs.m_mouseState.lZ);
		*(I32*)(&newState.m_mouseState.rgbButtons[4]) = *(I32*)(&m_mouseState.rgbButtons[4]) & *(I32*)(&rhs.m_mouseState.rgbButtons[4]);
		return newState;
	}

	PSX_INLINE MouseDeviceDX::MouseState MouseDeviceDX::MouseState::operator ~ ( void )
	{
		MouseState newState;
		*(I64*)(&newState.m_mouseState.lX) = ~(*(I64*)(&m_mouseState.lX));
		*(I64*)(&newState.m_mouseState.lZ) = ~(*(I64*)(&m_mouseState.lZ));
		*(I32*)(&newState.m_mouseState.rgbButtons[4]) = ~(*(I32*)(&m_mouseState.rgbButtons[4]));
		return newState;
	}

	MouseDeviceDX::MouseDeviceDX( ::HWND hWnd, LPDIRECTINPUTDEVICE8 pMouseDevice )
		: m_pMouseDevice( pMouseDevice ), m_refCount( 1 )
	{
		ClearStateData();
		PSX_Assert( pMouseDevice, "Invalid mouse device." );

		m_hWnd = hWnd;
		POINT mousePnt;
		GetCursorPos( &mousePnt );
		ScreenToClient( hWnd, &mousePnt );
		m_xPos = mousePnt.x;
		m_yPos = mousePnt.y;
		m_zPos = 0;
	}

	PSX_INLINE INT MouseDeviceDX::GetXPos( void )
	{
		return m_xPos;
	}

	PSX_INLINE INT MouseDeviceDX::GetYPos( void )
	{
		return m_yPos;
	}

	PSX_INLINE INT MouseDeviceDX::GetZPos( void )
	{
		return m_zPos;
	}

	PSX_INLINE INT MouseDeviceDX::GetDeltaX( void )
	{
		return m_mouseStates.m_currMouseState.m_mouseState.lX;
	}
	
	PSX_INLINE INT MouseDeviceDX::GetDeltaY( void )
	{
		return m_mouseStates.m_currMouseState.m_mouseState.lY;
	}

	PSX_INLINE INT MouseDeviceDX::GetDeltaZ( void )
	{
		return m_mouseStates.m_currMouseState.m_mouseState.lZ;
	}

	PSX_INLINE BOOL MouseDeviceDX::IsButtonDown( INT button )
	{
		return (m_mouseStates.m_currMouseState.m_mouseState.rgbButtons[button] & 0x80) != 0;
	}

	PSX_INLINE BOOL MouseDeviceDX::IsButtonUp( INT button )
	{
		return (m_mouseStates.m_currMouseState.m_mouseState.rgbButtons[button] & 0x80) == 0;
	}

	PSX_INLINE BOOL MouseDeviceDX::IsBufferedButtonDown( INT button )
	{
		return (m_mouseStates.m_buttonDowns.m_mouseState.rgbButtons[button] & 0x80) != 0;
	}

	PSX_INLINE BOOL MouseDeviceDX::IsBufferedButtonUp( INT button )
	{
		return (m_mouseStates.m_buttonUps.m_mouseState.rgbButtons[button] & 0x80) != 0;
	}

	PSX_FORCE_INLINE void MouseDeviceDX::ClearStateData( void )
	{
		PSX_ZeroMem( &m_mouseStates, sizeof(DIMOUSESTATE2) );
	}

	PSX_INLINE INT MouseDeviceDX::AddRef( void )
	{
		++m_refCount;
		return m_refCount;
	}
	
	PSX_INLINE INT MouseDeviceDX::Release( void )
	{
		if ( --m_refCount == 0 )
		{
			m_pMouseDevice->Unacquire();
			m_pMouseDevice->Release();
			_PrivateRemoveDevice( this );
			delete this;
		}
		return m_refCount;
	}
	
	PSX_INLINE void MouseDeviceDX::DestroySelf( void )
	{
		while ( m_refCount > 1 )
			Release();

		Release(); // Refcount is one when it gets here. Final Release destroys itself.
	}

	void MouseDeviceDX::Poll( void )
	{
		m_mouseStates.m_prevMouseState = m_mouseStates.m_currMouseState;
		HRESULT hr = m_pMouseDevice->GetDeviceState( sizeof(DIMOUSESTATE2), &m_mouseStates.m_currMouseState.m_mouseState );

		if ( FAILED( hr ) )
		{
			ClearStateData();
			m_pMouseDevice->Acquire();
			return;
		}

		POINT mousePos;
		::GetCursorPos( &mousePos );
		ScreenToClient( m_hWnd, &mousePos);

		if ( m_xPos != mousePos.x || m_yPos != mousePos.y )
		{
			AppMsg msg;
			
			msg.msg = EAppMessage::MOUSEMOVE;
			msg.lMsg1 = mousePos.x;
			msg.lMsg2 = mousePos.y;
			QueueInputMessage( msg );
		}

		m_xPos = mousePos.x;
		m_yPos = mousePos.y;
		m_zPos += m_mouseStates.m_currMouseState.m_mouseState.lZ;

		MouseState stateChanges		= m_mouseStates.m_currMouseState ^ m_mouseStates.m_prevMouseState;
		m_mouseStates.m_buttonDowns = stateChanges & m_mouseStates.m_currMouseState;
		m_mouseStates.m_buttonUps	= stateChanges & (~m_mouseStates.m_currMouseState);

		AppMsg msg;
		msg.msg = EAppMessage::UNKNOWN;
		msg.lMsg1 = mousePos.x;
		msg.lMsg2 = mousePos.y;

		// Left Button
		if ( m_mouseStates.m_prevMouseState.m_mouseState.rgbButtons[0] != m_mouseStates.m_currMouseState.m_mouseState.rgbButtons[0] )
		{
			if ( m_mouseStates.m_currMouseState.m_mouseState.rgbButtons[0] & 0x80 )
				msg.msg = EAppMessage::MOUSELBUTTONDOWN;
			else 
				msg.msg = EAppMessage::MOUSELBUTTONUP;

			QueueInputMessage( msg );
		}

		// Right Button
		if ( m_mouseStates.m_prevMouseState.m_mouseState.rgbButtons[1] != m_mouseStates.m_currMouseState.m_mouseState.rgbButtons[1] )
		{
			if ( m_mouseStates.m_currMouseState.m_mouseState.rgbButtons[1] & 0x80 )
				msg.msg = EAppMessage::MOUSERBUTTONDOWN;
			else 
				msg.msg = EAppMessage::MOUSERBUTTONUP;

			QueueInputMessage( msg );
		}

		// TODO: Support Middle Button

	

	}

#endif /* PSX_PLATFORM_WINDOWS_WINDOW */

	InputSystem::InputSystem( void )
	{
		//m_devicePtrPool;
		m_pAllocDevList = NULL;
		m_bInitialized = FALSE;
		m_deviceList.Resize( EInputDeviceType::EnumCount );
	}

	IDevice * InputSystem::GetDevice( EInputDeviceType::Type devType )
	{
		IDevice *pDevice;

		switch( devType )
		{
		case EInputDeviceType::KEYBOARD:
			pDevice = m_deviceList[EInputDeviceType::KEYBOARD];
			break;
		case EInputDeviceType::MOUSE:
			pDevice = m_deviceList[EInputDeviceType::MOUSE];
			break;
		default:
			PSX_PushError( "Invalid input device type specified in devType." );
			return PSX_NULL;
		}

		if ( pDevice == PSX_NULL )
			PSX_PushError( "Device not initialized." );

		return pDevice;
	}

	BOOL InputSystem::Initialize( SIZE_T devicePtrPoolSize /*= 10*/ )
	{
		if ( m_bInitialized )
			return TRUE;

		InputDevicesLayer::Initialize();
		Reset( devicePtrPoolSize );
		m_bInitialized = TRUE;

		// Create Default keyboard and mouse devices
		m_deviceList[EInputDeviceType::KEYBOARD] = GetInstance()->GetDeviceInterface( HID_KEYBOARD, 
			FLAG_FOREGROUND | FLAG_NONEXCLUSIVE );

		m_deviceList[EInputDeviceType::MOUSE] = GetInstance()->GetDeviceInterface( HID_MOUSE, 
			FLAG_FOREGROUND | FLAG_NONEXCLUSIVE );

		// Initialize event manager
		//m_dispatcher.

		return TRUE;
	}

	void InputSystem::Shutdown( void )
	{
		if ( !m_bInitialized )
			return;

		while ( m_pAllocDevList )
			m_pAllocDevList->DestroySelf();
		InputDevicesLayer::Shutdown();
		m_bInitialized = FALSE;
	}

	IDevice * InputSystem::GetDeviceInterface( HID_TYPE type, INT flag )
	{
		IDevice *pNewDevice;
		pNewDevice = InputDevicesLayer::CreateDevice( type, flag );

		if ( !pNewDevice )
			return NULL;

		if ( m_pAllocDevList == NULL )
		{
			m_pAllocDevList = pNewDevice;
			pNewDevice->m_pNext = NULL;
		}
		else
		{
			pNewDevice->m_pNext = m_pAllocDevList;
			m_pAllocDevList = pNewDevice;
		}

		return pNewDevice;
	}

	void InputSystem::Update( void )
	{
		IDevice *pDevice = m_pAllocDevList;

		while( pDevice )
		{
			pDevice->Poll();
			pDevice = pDevice->m_pNext;
		}

		DispatchMessages();
	}

	void InputSystem::Reset( SIZE_T devicePtrPoolSize )
	{
		while ( m_pAllocDevList )
			m_pAllocDevList->DestroySelf();

		m_devicePtrPool.Reset( devicePtrPoolSize );
	}

	void InputSystem::RemoveDevice( class IDevice *pDevice )
	{
		if ( m_pAllocDevList == pDevice )
		{
			m_pAllocDevList = m_pAllocDevList->m_pNext;
			return;
		}

		IDevice *pPrevDev = m_pAllocDevList;
		IDevice *pCurrDev = m_pAllocDevList->m_pNext;

		while ( pCurrDev )
		{
			if ( pCurrDev == pDevice )
			{
				pPrevDev->m_pNext = pCurrDev->m_pNext;
				return;
			}
		}
	}

	BOOL InputDevicesLayer::Initialize( void )
	{
	#ifdef PSX_PLATFORM_WINDOWS_WINDOW
		// Get DirectInput instance
		if ( m_pDI )
			return TRUE;

		HRESULT hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, 
			IID_IDirectInput8, (void**)&m_pDI, NULL );

		if ( FAILED( hr ) ) // TODO: Log error here
			return FALSE;

	#endif /* PSX_PLATFORM_WINDOWS_WINDOW */

		return TRUE;
	}

	void InputDevicesLayer::Shutdown()
	{
	#ifdef PSX_PLATFORM_WINDOWS_WINDOW
		PSX_SafeRelease( m_pDI );
	#endif /* PSX_PLATFORM_WINDOWS_WINDOW */
	}

	IDevice * InputDevicesLayer::CreateDevice( InputSystem::HID_TYPE type, INT flags /*= 0*/ )
	{
		IDevice *pNewDevice = NULL;
		// Determine on what target platform we're in
	#ifdef PSX_PLATFORM_WINDOWS_WINDOW
		HRESULT hr;

		// Format flag to use
		DWORD deviceFlags = 0;

		if ( flags & InputSystem::FLAG_BACKGROUND )
			deviceFlags = DISCL_BACKGROUND;
		if ( flags & InputSystem::FLAG_EXCLUSIVE )
			deviceFlags |= DISCL_EXCLUSIVE;
		if ( flags & InputSystem::FLAG_FOREGROUND )
			deviceFlags |= DISCL_FOREGROUND;
		if ( flags & InputSystem::FLAG_NONEXCLUSIVE )
			deviceFlags |= DISCL_NONEXCLUSIVE;
		if ( flags & InputSystem::FLAG_NOWINKEY )
			deviceFlags |= DISCL_NOWINKEY;
		
		switch( type )
		{
		case InputSystem::HID_KEYBOARD:
			{
				LPDIRECTINPUTDEVICE8  pDIKB;
				hr = m_pDI->CreateDevice( GUID_SysKeyboard, &pDIKB, NULL );
				if ( FAILED( hr ) )
					return NULL;

				hr = pDIKB->SetDataFormat( &c_dfDIKeyboard );
				if ( FAILED( hr ) )
				{
					pDIKB->Release();
					return NULL;
				}

				hr = pDIKB->SetCooperativeLevel( gpApp->GetHWND(), deviceFlags );
				if ( FAILED( hr ) )
				{
					pDIKB->Release();
					return NULL;
				}

				if ( pDIKB->Acquire() == S_FALSE )
				{
					pDIKB->Unacquire();
					pDIKB->Release();
					return NULL;
				}

				pNewDevice = new KeyboardDeviceDX( pDIKB );
				if ( !pNewDevice )
				{
					pDIKB->Unacquire();
					pDIKB->Release();
					return NULL;
				}
			}
			break;
		case InputSystem::HID_MOUSE:
			{
				LPDIRECTINPUTDEVICE8  pDIMouse;

				hr = m_pDI->CreateDevice( GUID_SysMouse, &pDIMouse, NULL );
				if ( FAILED( hr ) )
					return NULL;

				hr = pDIMouse->SetDataFormat( &c_dfDIMouse2 );
				if ( FAILED( hr ) )
				{
					pDIMouse->Release();
					return NULL;
				}

				hr = pDIMouse->SetCooperativeLevel( gpApp->GetHWND(), deviceFlags );
				if ( FAILED( hr ) )
				{
					pDIMouse->Release();
					return NULL;
				}

				if ( pDIMouse->Acquire() == S_FALSE )
				{
					pDIMouse->Unacquire();
					pDIMouse->Release();
					return NULL;
				}

				gpApp->CenterCursor();
				pNewDevice = new MouseDeviceDX( gpApp->GetHWND(), pDIMouse );
				if ( !pNewDevice )
				{
					pDIMouse->Unacquire();
					pDIMouse->Release();
					return NULL;
				}
			}
			break;
		};
	#endif /* PSX_PLATFORM_WINDOWS_WINDOW */

		return pNewDevice;
	}

	void InputSystem::QueueInputMessage( AppMsg &msg )
	{
		m_messages.Enqueue( msg );
	}

	void InputSystem::DispatchMessages( void )
	{
		Engine *pEngine = Engine::GetInstance();
		
		while( m_messages.GetSize() )
			pEngine->MessageProc( &m_messages.Dequeue() );
	}
}