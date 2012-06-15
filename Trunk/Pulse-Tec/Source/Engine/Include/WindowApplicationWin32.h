/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	WindowApplication.h
*
*	Description -	Encapsulates windows code stuff.
*
*	Comments	-	Base class for encapsulating application entry point.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/16/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_WINDOW_APPLICATION_H_
#define _PSX_WINDOW_APPLICATION_H_

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

#include "WindowsBaseApplication.h"
#include "PulseSTD.h"
#include "PTLBase.h"
#include "String.h"
#include "Clock.h"


#if defined(PSX_PLATFORM_WINDOWS_WINDOW)

namespace Pulse
{
	class WindowApplication;

	extern WindowApplication *gpApp;

	// User needs to define these two functions to create user derived application.
	extern IApplication * PSX_OnCreateApplication( void );
	extern void PSX_OnReleaseApplication( IApplication *pApp );

	class WindowApplication : public WindowsBaseApplication
	{
	private:

		// Pulse Window Application entry-point
		friend INT WindowEntryPoint( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd );
	
	public:

		// This constructor is not needed. Initialize() handles all the necessary setups.
		//virtual WindowApplication( void );
		virtual ~WindowApplication( void ) { }

	protected:

		INT Run( INT argNum, CHAR **ppArguments );

		BOOL Initialize( const HINSTANCE hInst );
		BOOL RegisterClass( void );

		// Windows app methods
		void ShowWindow( INT shwCmd = 1  );
		void SetApplicationName( const CHAR *pAppName );

		BOOL SetWindowPos( INT newX, INT newY );
		void FixWindowSize( void );
		void ResetWindowSize( UINT newWidth, UINT newHeight );
		void ResetWindow( void );
		
		void CenterWindow( void );
		BOOL ToggleFullscreen( void );
		BOOL IsFullScreen( void );

		BOOL IsOnlyInstance( void );

		void ShowCursor( Pulse::BOOL bShow = TRUE );
		void HideCursor( void );
		void CenterCursor( void );

		INT GetAppCursorPos( POINT &point );
		INT GetCursorPos( POINT &point );

		const INT GetWidth( void );
		const INT GetHeight( void );

		FLOAT GetElapsedTime( void );
		DOUBLE GetTotalTime( void );

		// Debug method. Measure performance in Frames Per Second.
		void ShowFPS( Pulse::BOOL showFPS );
		
		/* Functions that would probably never be redefined but just in case... */

		// This function is deprecated. This functionality is now being handled by the choosen graphics API.
		Pulse::BOOL SetDisplayMode( const INT width, const INT height, const INT bitsPerPix, const DWORD refreshRate );

		// These functions shouldn't prolly be virtuals....
		virtual Pulse::BOOL _RegisterWindowClass( void );
		virtual Pulse::BOOL _CreateWindow( void );
		virtual Pulse::BOOL _OnDeviceReset( void );
		virtual void _OnUpdate( void );
		virtual void _ShutDown( void );
		virtual BOOL _Initialize( void );

		// Event messages function handlers
		virtual LRESULT OnWMCreateWindow( void );
		virtual LRESULT OnWMClose( void );
		virtual LRESULT OnWMDestroy( void );
		virtual LRESULT OnWMMove( void );
		virtual LRESULT OnWMPaint( void );
		virtual LRESULT OnWMGainFocus( void );
		virtual LRESULT OnWMLoseFocus( void );
		virtual LRESULT OnWMQuit( void );

	protected:

		// Call this 
		void Terminate( void );

		/* Common methods the derived app class needs to redefine. */
		
		// OnInitialize may be deprecated. Redundant function because it is called just before OnStart()
		virtual BOOL OnInitialize( void )								{ return TRUE; }
		virtual BOOL OnStart( void )									{ return TRUE; }
		virtual void OnUpdate( DOUBLE time, FLOAT dt, void *pContext )	{ }
		virtual void OnRender( void )									{ }
		virtual void OnShutDown( void )									{ }
		virtual BOOL OnDeviceReset( void )								{ return TRUE;  }	// This is called when _OnDeviceReset is triggered.

	protected:

		class InputSystem *m_pInputManager;

	private:

		// Used by the graphics device. 
		// If using TempGraphicsDevices like DX window resize isn't allowed to be used by the App.
		void SetAllowResize( BOOL bAllowResize ) { m_bAllowResize = bAllowResize; }

	protected:	// Derived application classes shouldn't touch these...

		friend class TempGraphicsDevice;
		friend class Renderer;
		friend class InputSystem;
		friend class InputDevicesLayer;
		friend class SwapChain;

		static LRESULT CALLBACK WndProc( ::HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	
		void HandleFPS( void );
		void UpdateVKeyMap( void );
		HINSTANCE GetHInstance( void ) const;
		::HWND GetHWND( void );	

		HINSTANCE	m_hInstance;		// Instance of this app.
		::HWND		m_hWnd;				// Handle to this window app

		// WindowApplication Info
		WNDCLASSEX	m_wndClass;			// Window class definition
		String		m_appName;			// Name of the application

		// Store WndProc param vars here for easier access of On...function events.
		MSG			m_msg;				// Current message. Place it here to avoid creating everytime.
		UINT		msg;
		WPARAM		wParam;
		LPARAM		lParam;

		// WindowApplication run-time Info
		Pulse::BOOL m_bWindowed;		// Boolean to let us know whether we're in fullscreen mode or not.
		Clock		m_appTime;			// Global application time.

		INT			m_appPosX, m_appPosY;		// WindowApplication position
		UINT		m_appWidth, m_appHeight;	// WindowApplication size

		BOOL		m_bShowFPS;			// Bolean for seting FPS debug performance info
		BOOL		m_paused;			// If the window is active don't run the program.
		BOOL		m_bAllowResize;
		BOOL		m_bMinimized;
		BOOL		m_bQuitting;

		UINT m_totalFrames;
	};

	// This is the same as IsKeyDown
	#define KEYDOWN(vkcode) (GetAsyncKeyState(vkcode)&0x8000 ? true : false)

	PSX_INLINE HINSTANCE WindowApplication::GetHInstance( void ) const
	{
		return m_hInstance;
	}

	PSX_INLINE ::HWND WindowApplication::GetHWND( void )
	{
		return m_hWnd;
	}

	PSX_INLINE FLOAT WindowApplication::GetElapsedTime( void )
	{
		return m_appTime.GetDeltaSeconds();
	}

	PSX_INLINE DOUBLE WindowApplication::GetTotalTime( void )
	{
		return m_appTime.GetTimeSeconds();
	}

	PSX_INLINE const INT WindowApplication::GetWidth( void )
	{
		return m_appWidth;
	}

	PSX_INLINE const INT WindowApplication::GetHeight( void )
	{
		return m_appHeight;
	}

	PSX_INLINE void WindowApplication::ShowFPS( Pulse::BOOL showFPS )
	{
		m_bShowFPS = showFPS;
	}
}

#endif /* PSX_PLATFORM_WINDOWS_WINDOW */ 

#endif /* _PSX_WINDOW_APPLICATION_H_ */