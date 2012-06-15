/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Engine.h
*
*	Description -	Contains application's main sub-systems. 
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/01/2010	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_ENGINE_H_
#define _PSX_ENGINE_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "Singleton.h"
#include "String.h"
#include "AppMsg.h"
#include "Renderer.h"

namespace Pulse
{
	// TODO: Load up these engine settings from an XML file
	struct EngineSettings
	{
		// General settings
		String					appName;
		INT						posX;
		INT						posY;

		// Memory Management
		BOOL					bUseCustomMemorymanager;
		SIZE_T					memoryManagerSizeInMB;
		SIZE_T					resourceCacheSizeInMB;
		
		// Renderer settings
		ERendererType::Type		rendererType;
		ERendererProcessingType::Type rendererProcessing;
		BOOL					bSoftwareRendererFallback;
		DisplayMode				displaySettings;

		EngineSettings( void )
		{
			bUseCustomMemorymanager = FALSE;
			memoryManagerSizeInMB	= 256; 
			resourceCacheSizeInMB	= 50; 
			posX					= 0; 
			posY					= 0;
			appName					= PSX_String("Sample Pulse Application"); 
			rendererType			= ERendererType::DX11;
			rendererProcessing		= ERendererProcessingType::HARDWARE;
			bSoftwareRendererFallback = TRUE;
		}

	};

	class Engine : public Singleton< Engine >
	{
	public:

		EErrorCode::Type Initialize( void );

		void Shutdown( void );

		const EngineSettings * GetEngineSettings( void ) { return &m_settings; }

		const DisplayMode * GetDisplaySettings( void ) { return &m_settings.displaySettings; }

		void Update( void );

		const Clock * GetClock( void );

		ResourceCache * GetResourceCache( void ) { return m_pResCache; }

		// TODO: Might accept a thread index once we have deferred renderers
		Renderer * GetRenderer( void ) { return m_pRenderer; }

		#ifdef PSX_PLATFORM_WINDOWS_WINDOW
			void SetTopLevelHWND( ::HWND hWnd ) { m_hWnd = hWnd; }
			::HWND GetTopLevelHWND( void ) { return m_hWnd; }
		#endif /* PSX_PLATFORM_WINDOWS_WINDOW */

		// NOTE: Rule of Thumb: TRUE if message has been eaten, FALSE otherwise.
		BOOL MessageProc( AppMsg *pMsg );
		
		// NOTE: Not currently used...
		void OnUpdate( void );
		void OnRender( void );

		///////////////////////////////
		void SetApplicationLogic( Logic *pLogic );

		Logic * GetApplicationLogic( void ) { return m_pLogic; }

	protected:

		friend Singleton;

		Engine( void );
		~Engine( void );

		EErrorCode::Type InitializeRenderer( void );

		Renderer * CreateRenderer( ERendererType::Type type );

		#ifdef PSX_PLATFORM_WINDOWS_WINDOW
			::HWND m_hWnd;
		#endif /* PSX_PLATFORM_WINDOWS_WINDOW */

		EngineSettings	m_settings;

		Clock			*m_pSysClock;
		FileSystem		*m_pFileSystem;
		ResourceCache	*m_pResCache;	// Cache loaded resource files, from FileSystem, in ResourceCache
		ProcessManager	*m_pProcessManager;
		EventManager	*m_pEventManager;
		PulseScript		*m_pScriptParser;
		InputSystem		*m_pInputSystem;
		GraphicsDevice	*m_pGraphicsDevice;
		Renderer		*m_pRenderer;
		// TODO: Add Sound Manager

		// Resource manager

		Logic *m_pLogic;
		View *m_pView; // NOTE: Set as HumanView for now. Allow for multiple views in the future

	};

	PSX_INLINE const Clock * Engine::GetClock( void )
	{
		return m_pSysClock;
	}

}

#endif /* _PSX_ENGINE_H_ */