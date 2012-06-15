/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	WindowApplicationWin32.cpp
*
*	Comments	-	See WindowApplicationWin32.h
*
**************************************************************************************/

#include "../Include/WindowApplicationWin32.h"
#include "../Include/InputSystem.h"
#include "../Include/MemoryManager.h"
#include "../Include/Math.h"
#include "../Include/Engine.h"
#include <stdio.h>

#if defined( PSX_PLATFORM_WINDOWS_WINDOW )

// Windows window application entry-point
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd )
{
	// Call Pulse Window application entry-point
	return Pulse::WindowEntryPoint( hInstance, hPrevInstance, lpCmdLine, nShowCmd );
}

namespace Pulse
{
	WindowApplication *gpApp;

	#define WS_NORMAL_WINDOW (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX))
	#define WS_FULLSCREEN_WINDOW (WS_POPUP)

	INT WindowEntryPoint( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd )
	{
		using namespace Pulse;

		WindowApplication *pApp = NULL;
		Pulse::INT ret;

		pApp = (WindowApplication*)PSX_OnCreateApplication();

		if ( pApp == NULL )
		{
			// Failed to create application.
			PSX_OnReleaseApplication( pApp );
			return -1; // return error. We need to have some kind of error code here...
		}

		if ( !pApp->Initialize( hInstance ) )
		{
			// Failed to initialize application
			PSX_OnReleaseApplication( pApp );
			return -1; // return error. We need to have some kind of error code here...
		}

		pApp->ShowWindow( nShowCmd );

		// No arguments by default...
		ret = pApp->Run( 0, NULL );
		PSX_OnReleaseApplication( pApp );

		return ret;
	}

	//WindowApplication::WindowApplication( void )
	//	: m_hInstance( NULL ), m_appPosX( 0 ), m_appPosY( 0 ), m_appWidth( 0 ), m_appHeight( 0 ),
	//		m_bWindowed( FALSE ), m_dt( 0.0f ), m_totalTime( 0.0f ), m_bShowFPS(  FALSE ), m_paused( FALSE )
	//{

	//}

	BOOL WindowApplication::Initialize( const HINSTANCE hInst  )
	{
		Engine *pEngine = Engine::GetInstance();
		const EngineSettings *pSettings = pEngine->GetEngineSettings();

		gpApp			= this;
		m_hInstance		= hInst;
		m_appName		= pSettings->appName;
		m_appPosX		= pSettings->posX;
		m_appPosY		= pSettings->posY;
		m_appWidth		= pSettings->displaySettings.width;
		m_appHeight		= pSettings->displaySettings.height;
		m_bWindowed		= pSettings->displaySettings.bWindowed;
		m_bShowFPS		= FALSE;
		m_paused		= FALSE;
		m_bAllowResize	= TRUE;
		m_bMinimized	= FALSE;
		m_bQuitting		= FALSE;

		if ( !gpApp->RegisterClass() )
			return FALSE;

		pEngine->SetTopLevelHWND( m_hWnd );

		// Now initialize our application(game) engine
		Engine::GetInstance()->Initialize();

		return TRUE;
	}

	Pulse::BOOL WindowApplication::RegisterClass( void )
	{
		// We can only create one instance of this application
		if ( !IsOnlyInstance() ) 
			return FALSE;

		PSX_ZeroMem( &m_wndClass, sizeof( WNDCLASSEX ) );

		// This is a definition for the window we're creating.
		m_wndClass.cbSize			= sizeof( WNDCLASSEX );
		m_wndClass.style			= CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW /*| CS_DBLCLKS*/;
		m_wndClass.lpfnWndProc		= WindowApplication::WndProc;
		m_wndClass.cbClsExtra		= 0;
		m_wndClass.cbWndExtra		= 0;
		m_wndClass.hInstance		= m_hInstance;
		m_wndClass.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
		m_wndClass.hCursor			= LoadCursor( NULL, IDC_ARROW );
		m_wndClass.hbrBackground	= (HBRUSH)GetStockObject( BLACK_BRUSH );
		m_wndClass.lpszClassName	= m_appName.GetBuffer();
		m_wndClass.lpszMenuName		= NULL;
		m_wndClass.hIconSm			= NULL;

		// Register window class
		if ( !_RegisterWindowClass() ) return FALSE;

		//					Left,	Top,	Right,			Bottom
		//RECT clientRect = { m_appPosX, m_appPosY, m_appPosX + m_appWidth, m_appPosY + m_appHeight }; // Not used.

		// Create window. This won't show the window visually yet.
		if ( !_CreateWindow() ) return FALSE;

		return TRUE;
	}

	Pulse::BOOL WindowApplication::SetWindowPos( INT newX, INT newY )
	{
		if ( IsFullScreen() )
			return FALSE; // Set Pos denied if in fullscreen mode.

		m_appPosX = newX;
		m_appPosY = newY;

		::SetWindowPos( m_hWnd, 0, m_appPosX, m_appPosY, 0, 0,
			SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOZORDER );

		return TRUE;
	}

	void WindowApplication::FixWindowSize( void )
	{
		if ( IsFullScreen() == FALSE )
		{
			// Windowed
			RECT r = { m_appPosX, m_appPosY, m_appPosX + m_appWidth, m_appPosY + m_appHeight };
			SetWindowLong( m_hWnd, GWL_STYLE, WS_NORMAL_WINDOW );
			::SetWindowPos( m_hWnd, HWND_NOTOPMOST, m_appPosX, m_appPosY, GetWidth(), GetHeight(), SWP_NOACTIVATE | SWP_SHOWWINDOW );

			// Adjust window rect
			AdjustWindowRectEx( &r, GetWindowLongPtr( m_hWnd, GWL_STYLE), FALSE, 
				GetWindowLongPtr( m_hWnd, GWL_EXSTYLE ) );

			INT deltaX( m_appPosX - r.left ), deltaY( m_appPosY - r.top );
			r.left += deltaX; r.right += deltaX;
			r.top += deltaY; r.bottom += deltaY;

			::SetWindowPos( m_hWnd, 0, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER );
		}
	}

	void WindowApplication::ResetWindowSize( UINT newWidth, UINT newHeight )
	{
		if ( IsFullScreen() || !m_bAllowResize )
			return;

		m_appWidth = newWidth;
		m_appHeight = newHeight;

		// Windowed
		RECT r = { m_appPosX, m_appPosY, m_appPosX + m_appWidth, m_appPosY + m_appHeight };
		SetWindowLong( m_hWnd, GWL_STYLE, WS_NORMAL_WINDOW );
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST, m_appPosX, m_appPosY, m_appWidth, m_appHeight, /*SWP_NOACTIVATE |*/ SWP_SHOWWINDOW );

	}

	void WindowApplication::ResetWindow( void )
	{
		// Setup the style of the window
		if ( IsFullScreen() == FALSE )
		{
			// Windowed
			RECT r = { m_appPosX, m_appPosY, m_appPosX + m_appWidth, m_appPosY + m_appHeight };
			SetWindowLong( m_hWnd, GWL_STYLE, WS_NORMAL_WINDOW );
			::SetWindowPos( m_hWnd, HWND_NOTOPMOST, m_appPosX, m_appPosY, GetWidth(), GetHeight(), SWP_NOACTIVATE | SWP_SHOWWINDOW );

			// Adjust window rect
			AdjustWindowRectEx( &r, GetWindowLongPtr( m_hWnd, GWL_STYLE), FALSE, 
				GetWindowLongPtr( m_hWnd, GWL_EXSTYLE ) );

			INT deltaX( m_appPosX - r.left ), deltaY( m_appPosY - r.top );
			r.left += deltaX; r.right += deltaX;
			r.top += deltaY; r.bottom += deltaY;

			::SetWindowPos( m_hWnd, 0, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER );
		}
		else
		{
			// Fullscreen
			// SetMenu( hWnd, NULL ); // We already don't have a menu
			SetWindowLong( m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP );
			::SetWindowPos( m_hWnd, NULL, 0, 0, m_appWidth, m_appHeight, SWP_NOZORDER );
		}
	}

	void WindowApplication::CenterWindow( void )
	{
		if ( IsFullScreen() )
			return;	// Do nothing in fullscreen mode.

		m_appPosX		= GetSystemMetrics( SM_CXSCREEN )/2 - m_appWidth/2;
		m_appPosY		= GetSystemMetrics( SM_CYSCREEN )/2 - m_appHeight/2;

		FixWindowSize();
	}

	Pulse::BOOL WindowApplication::IsFullScreen( void )
	{
		return m_bWindowed == FALSE;
	}

	Pulse::BOOL WindowApplication::IsOnlyInstance( void )
	{
		::HWND hWnd = FindWindow( m_appName.GetBuffer(), NULL );

		if ( hWnd ) 
		{
			::ShowWindow( hWnd, SW_SHOWNORMAL );
			SetFocus( hWnd );
			SetForegroundWindow( hWnd );
			SetActiveWindow( hWnd );
			return FALSE;
		}
			
		return TRUE;
	}

	void WindowApplication::ShowCursor( Pulse::BOOL bShow /*= TRUE*/ )
	{
		::ShowCursor( bShow );
	}

	void WindowApplication::HideCursor( void )
	{
		ShowCursor( FALSE );
	}

	void WindowApplication::CenterCursor( void )
	{
		POINT absPnt;
		POINT relPnt;
		POINT delta;
		::GetCursorPos( &absPnt );
		::GetCursorPos( &relPnt );
		ScreenToClient( m_hWnd, &relPnt );
		delta.x = absPnt.x - relPnt.x;
		delta.y = absPnt.y - relPnt.y;
		absPnt.x = delta.x + (m_appWidth / 2);
		absPnt.y = delta.y + (m_appHeight / 2);
		SetCursorPos( absPnt.x, absPnt.y );
	}

	INT WindowApplication::GetAppCursorPos( POINT &point )
	{
		if ( !::GetCursorPos( &point ) ) return FALSE;
		return ::ScreenToClient( m_hWnd, &point );
	}

	INT WindowApplication::GetCursorPos( POINT &point )
	{
		return ::GetCursorPos( &point );
	}

	Pulse::BOOL WindowApplication::ToggleFullscreen( void )
	{
		#if 0
			HDC hdc = GetDC( m_hWnd );
			INT refreshRate = GetDeviceCaps( hdc, VREFRESH );
			INT bitsPerPix = GetDeviceCaps( hdc, BITSPIXEL );
			ReleaseDC( m_hWnd, hdc );

			if ( IsFullScreen() ) // Restore back to non-fullscreen mode.
			{
				if ( !SetDisplayMode( 0, 0, 0, 0 ) )
					return FALSE; // Failed to switch back to non-fullscreen mode.

				// Set back to an overloapped window
				SetWindowLongPtr( m_hWnd, GWL_STYLE, WS_NORMAL_WINDOW );

				// Set menu back if we have a menu bar here...

				::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
					SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);

				RECT rCRect;
				rCRect.left = m_appPosX; rCRect.right = m_appPosX + m_appWidth;
				rCRect.top = m_appPosY; rCRect.bottom = m_appPosY + m_appHeight;
				
				// We don't have a meny so set it to FALSE
				/*AdjustWindowRectEx( &rCRect, GetWindowLongPtr( m_hWnd, GWL_STYLE), FALSE, 
					GetWindowLongPtr( m_hWnd, GWL_EXSTYLE ) );*/

				::SetWindowPos( m_hWnd, 0, rCRect.left, rCRect.top, 
					rCRect.right - rCRect.left, rCRect.bottom - rCRect.top, SWP_NOZORDER );

				// ShowCursur if we want to.
			}
			else // Go Full-Screen.
			{
				if ( !SetDisplayMode( m_appWidth, m_appHeight, bitsPerPix, refreshRate ) )
					return FALSE; // Failed to go to fullscreen mode.

				// In full screen, the window needs to be a popup window.
				SetWindowLongPtr( m_hWnd, GWL_STYLE, WS_FULLSCREEN_WINDOW );

				// This disables the menu.
				// SetMenu(m_hWnd, NULL);

				// Make the window topmost
				::SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, 
					SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );

				::SetWindowPos( m_hWnd, 0, 0, 0, 
					GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), SWP_NOZORDER ); 
			}

			// Toggle bolean then finally call onReset
			m_bWindowed = !m_bWindowed;
			return _OnDeviceReset();

		#else
			
			// Toggle fullscreen boolean
			m_bWindowed = !m_bWindowed;
			//ResetWindow();

			//if ( m_bWindowed )
			//	ShowWindow();

			return _OnDeviceReset();
			 
		#endif
	}

	void WindowApplication::ShowWindow( INT shwCmd /*= 1*/ )
	{
		::ShowWindow( m_hWnd, shwCmd );
		UpdateWindow( m_hWnd );

		//SetWindowPos( m_appPosX, m_appPosY );
		FixWindowSize();
	}

	void WindowApplication::SetApplicationName( const CHAR *pAppName )
	{
		m_appName = pAppName;
		SetWindowText( m_hWnd, m_appName.GetBuffer() );
	}

	INT WindowApplication::Run( INT argNum, CHAR **ppArguments )
	{
		// This is where all the magic starts.
		MSG message;
		PSX_ZeroMem( &message, sizeof( MSG ) );

		// Fire OnInitialize and OnStart event
		if ( !_Initialize() ) return -1;
		if ( !OnStart() ) return -1;

		while ( message.message != WM_QUIT )
		{
			if ( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) )
			{
				// This code handles message events
				TranslateMessage( &message );
				DispatchMessage( &message );
				continue;
			}

			if ( m_bQuitting )
				continue;

			// Process app code if we don't have any windows messages.
			System::Update();
			
			if ( !m_bMinimized )
			{
				if ( !m_paused ) 
				{
					_OnUpdate();
					Engine::GetInstance()->Update();
				}
				
				OnRender();
			}

			// Handle FPS debug info
			if ( m_bWindowed && m_bShowFPS ) 
				HandleFPS();

		}

		return (INT)message.wParam;
	}


	/* Protected Methods */
	Pulse::BOOL WindowApplication::SetDisplayMode( const INT width, const INT height, const INT bitsPerPix, const DWORD refreshRate )
	{
		DEVMODE dm;

		dm.dmSize = sizeof( DEVMODE );

		// If all are zeroes, return to original mode
		if ( !width && !height && !bitsPerPix && !refreshRate )
		{
			ChangeDisplaySettings( NULL, 0 );
			return TRUE;
		}
		
		DWORD i = 0;

		// Enumerate available display settings
		while ( EnumDisplaySettings( NULL, i++, &dm ) )
		{
			// NOTE: Just look for greater than or equal to refreshRate for the refresh rate.
			if(dm.dmPelsWidth == width && dm.dmPelsHeight == height &&
				dm.dmBitsPerPel == bitsPerPix && dm.dmDisplayFrequency >= refreshRate)
			{
				if(ChangeDisplaySettings(&dm, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)
				{
					ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	Pulse::BOOL WindowApplication::_RegisterWindowClass( void )
	{
		if ( ::RegisterClassEx( &m_wndClass ) == 0 )
		{
			PSX_PushError( "Failed to register Win32 window class." );
			return FALSE;
		}
		
		return TRUE;
	}


	Pulse::BOOL WindowApplication::_CreateWindow( void )
	{
		m_hWnd = ::CreateWindowEx( 0, m_wndClass.lpszClassName, m_appName.GetBuffer(), WS_NORMAL_WINDOW, 
					m_appPosX, m_appPosY, m_appWidth, m_appHeight, NULL, NULL, m_hInstance, NULL );

		if ( m_hWnd == 0 )
		{
			PSX_PushError( "Failed to create Win32 Window." );
			return FALSE;
		}

		return TRUE;
	}

	Pulse::BOOL WindowApplication::_OnDeviceReset( void )
	{
		return OnDeviceReset();
	}

	void WindowApplication::_OnUpdate( )
	{
		// Finally update main application
		OnUpdate( m_appTime.GetTimeSeconds(), m_appTime.GetDeltaSeconds(), NULL );
	}

	void WindowApplication::_ShutDown( void )
	{
		OnShutDown();
	}

	BOOL WindowApplication::_Initialize( void )
	{
		// NOTE: Do some win specific initialization code.
		//	Most of the systems are already initialized in the Engine class.

		return OnInitialize();
	}

	/* Event messages function handlers */
	LRESULT WindowApplication::OnWMCreateWindow( void )
	{
		// Default does nothing but return true
		return TRUE;
	}

	LRESULT WindowApplication::OnWMClose( void )
	{
		// Fires internal OnExit event
		_ShutDown();
		DestroyWindow( GetHWND() );
		return 0;
	}

	LRESULT WindowApplication::OnWMDestroy( void )
	{
		PostQuitMessage( 0 );
		return 0;
	}

	LRESULT WindowApplication::OnWMMove( void )
	{
		RECT desiredRect = { LOWORD( lParam ), HIWORD( lParam ), LOWORD( lParam ) + m_appWidth, HIWORD( lParam ) + m_appHeight };

		AdjustWindowRectEx( &desiredRect, GetWindowLongPtr( m_hWnd, GWL_STYLE ), FALSE, GetWindowLongPtr( m_hWnd, GWL_EXSTYLE ) );
		
		m_appPosX = desiredRect.left;
		m_appPosY = desiredRect.top;

		return 0;
	}

	LRESULT WindowApplication::OnWMPaint( void )
	{
		return DefWindowProc( m_hWnd, msg, wParam, lParam );
	}

	LRESULT WindowApplication::OnWMGainFocus()
	{

		m_paused = FALSE;

		// Should return in fullscreen if it originally was
		if ( m_bWindowed == FALSE )
		{ 
			m_bMinimized = FALSE;
			 ::ShowWindow( m_hWnd, 1 );
			 ::UpdateWindow( m_hWnd );
			// SetFocus( m_hWnd );
		}

		return 0;
	}

	LRESULT WindowApplication::OnWMLoseFocus()
	{
		m_paused = TRUE;

		// Minimize if we're in fullscreen
		if ( m_bWindowed == FALSE && !m_bMinimized )
		{ 
			m_bMinimized = TRUE;
			// Fake non-fullscreen while minimized
			::ShowWindow( m_hWnd, SW_SHOWMINIMIZED );
		}

		return 0;
	}

	LRESULT WindowApplication::OnWMQuit( void )
	{
		return DefWindowProc( m_hWnd, msg, wParam, lParam );
	}

	/* Windows Procedures */
	LRESULT CALLBACK WindowApplication::WndProc( ::HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		gpApp->msg = msg;
		gpApp->wParam = wParam;
		gpApp->lParam = lParam;

		// Messages will be delegated to other functions "On...()" as if it fired an event.
		switch( msg )
		{
		case WM_CREATE:
			return gpApp->OnWMCreateWindow();
		case WM_PAINT:
			return gpApp->OnWMPaint();
		case WM_ACTIVATEAPP:
			if ( wParam )
				return gpApp->OnWMGainFocus();
			else
				return gpApp->OnWMLoseFocus();
			break;
		case WM_SIZE:
			if( wParam == SIZE_MINIMIZED )
			{
				// gpApp->OnWMLoseFocus();
				gpApp->m_bMinimized = TRUE;
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				if( gpApp->m_bMinimized )  // Restoring from minimized state?
				{
					//gpApp->OnWMGainFocus();
					gpApp->m_bMinimized = FALSE;
				}
				else if ( gpApp->m_bWindowed )
				{
					 //gpApp->m_bMinimized = FALSE;
					 //::ShowWindow( gpApp->m_hWnd, SW_MINIMIZE );
					//gppp->m_bMinimized = TRUE;
					// gpApp->ShowWindow( SW_MINIMIZE );
				}
			}
			return 0;
		case WM_MOVE:
			return gpApp->OnWMMove();
		case WM_SYSCOMMAND:
			// Eat Alt-F4.
			if ( wParam == SC_CLOSE )
				//return 0; // Allow SC_CLOSE for now...
				break;
			else
				break;
		case WM_CLOSE:
			return gpApp->OnWMClose();
		case WM_QUIT:
			return gpApp->OnWMQuit();
		//case WM_KILLFOCUS:
		case WM_DESTROY:
			return gpApp->OnWMDestroy();
		}

		// Pass it to default procedure if unhandled.
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	/* Private Methods */

	void WindowApplication::HandleFPS( void )
	{
		static UINT		totalFrames = 0;
		static FLOAT	elapsedTime = 0.0f;

		elapsedTime += GetElapsedTime();
		++totalFrames;

		if ( elapsedTime >= 1.0f )
		{
			CHAR str[255];

			m_totalFrames = totalFrames;
			wsprintf( str,PSX_String("%s - FPS: %d"), m_appName.GetBuffer(), totalFrames );
			//sprintf_s( str, 255,PSX_String("FPS: %d"), totalFrames );
			SetWindowText( m_hWnd, str );

			elapsedTime = 0.0f;
			totalFrames = 0;
		}
	}

	void WindowApplication::Terminate( void )
	{
		if ( m_bQuitting == FALSE )
		{
			m_bQuitting = TRUE;
			PostMessage( GetHWND(), WM_CLOSE, 0, 0 );
		}
	}
}

#endif /* defined( PSX_PLATFORM_WINDOWS_WINDOW ) */