/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Engine.cpp
*
*	Comments	-	See Engine.h
*
**************************************************************************************/

#include "../Include/Engine.h"
#include "../Include/System.h"
#include "../Include/Math.h"
#include "../Include/ProcessManager.h"
#include "../Include/EventManager.h"
#include "../Include/PulseScript.h"
#include "../Include/FileSystem.h"
#include "../Include/ResourceFile.h"
#include "../Include/ResourceCache.h"
#include "../Include/InputSystem.h"
#include "../Include/Clock.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/Logic.h"
#include "../Include/View.h"
#include "../Include/XMLParser.h"
#include "../Include/GraphicsInfrastructure.h"
#include "../Include/Renderer.h"

namespace Pulse
{

	PSX_REGISTRY_OBJECT_START( Engine, ERegistryPriority::Highest, ERegistryPriority::Highest )
		PSX_REGISTRY_ON_INITIALIZE()
		{
			// We should probably manually initialize this...
			//if ( Engine::GetInstance()->Initialize(  ) )
			//	return EErrorCode::NONE;
			//else
			//	return EErrorCode::UNKNOWN;
		}
		PSX_REGISTRY_ON_SHUTDOWN()
		{
			Engine::GetInstance()->Shutdown();
		}
	PSX_REGISTRY_OBJECT_END();

	static U64 m_lastPerfCounter;

	Engine::Engine( void )
		: m_pSysClock( NULL ), m_pFileSystem( NULL ), m_pResCache( NULL ), m_pProcessManager( NULL ), m_pEventManager( PSX_NULL ),
		m_pScriptParser( NULL ), m_pInputSystem( NULL ), m_pRenderer( PSX_NULL ), m_pLogic( PSX_NULL ), m_pView( PSX_NULL )
	{
		m_lastPerfCounter = System::GetPerformanceCounter();

		//PSX_ZeroMem( &m_settings, sizeof(EngineSettings) );

		#ifdef PSX_PLATFORM_WINDOWS_WINDOW
			m_hWnd = NULL;
		#endif /* PSX_PLATFORM_WINDOWS_WINDOW */
	}

	Engine::~Engine( void )
	{

	}

	EErrorCode::Type Engine::Initialize( void )
	{
		// TODO: Use PulseScript parser to load in engine settings
		EngineSettings settings;
		m_settings = settings;

		// Initialize low level systems first
		System::Initialize();

		// Setup memory manager

		// Check minimum memory and CPU speed requirements
		if ( !System::CheckMemory( m_settings.memoryManagerSizeInMB, m_settings.memoryManagerSizeInMB * 2 ) )
		{
			PSX_Assert( FALSE, "Not enough free memory. Failed to initialize game engine." );
			return EErrorCode::MEMORY;
		}

		m_pSysClock			= new Clock;
		m_pProcessManager	= new ProcessManager;
		m_pEventManager		= new EventManager;
		m_pFileSystem		= FileSystem::GetInstance();
		m_pScriptParser		= new PulseScript;
		m_pInputSystem		= InputSystem::GetInstance();
		m_pResCache			= new ResourceCache( m_settings.resourceCacheSizeInMB );

		Math::Initialize();
		XMLParser::Initialize();
		m_pInputSystem->Initialize();

		// Initialize renderer
		EErrorCode::Type retCode = InitializeRenderer();

		if ( !m_pSysClock || !m_pFileSystem || !m_pInputSystem || !m_pProcessManager || retCode != EErrorCode::NONE )
		{
			PSX_Assert( FALSE, "Failed to initialize engine systems." );
			return EErrorCode::UNKNOWN;
		}

		return EErrorCode::NONE;
	}

#ifdef __PSX_WINDOWS_WINDOW
	EErrorCode::Type Engine::InitializeRenderer( void )
	{
		GraphicsInfrastructure *pGI = GraphicsInfrastructure::GetInstance();

		Vector2 currentResolution = pGI->GetCurrentResolution( 0, 0 );
		FLOAT ratio = currentResolution.x / currentResolution.y;

		// Enumerate available display modes
		SIZE_T numAdapters = pGI->GetNumAdapters();
		SIZE_T numAdapterOutputs = pGI->GetNumAdpaterOutputs(0);
		DisplayModeList modeList;
		pGI->GetAvailableDisplayModes( 0, 0, EGraphicsFormat::R8G8B8A8_UNORM_SRGB, 0, 0, 16.0f/9.0f, &modeList );

		// Create renderer
		m_pGraphicsDevice = GraphicsDevice::GetInstance();
		m_pRenderer = CreateRenderer( m_settings.rendererType );

		//if ( m_pRenderer->Initialize() != EErrorCode::NONE )
		//{
		//	PSX_PushError( "Failed to initialize renderer." );
		//	return EErrorCode::GRAPHICS;
		//}

		//m_pRenderer->SetDisplayMode( &m_settings.displaySettings );
		//m_pRenderer->CreateDevice();

		return EErrorCode::NONE;
	}
#elif defined(__PSX_WINDOWS_CONSOLE)
	EErrorCode::Type Engine::InitializeRenderer( void )
	{
		// Nothing to initialize for console
		return EErrorCode::NONE;
	}
#endif __PSX_WINDOWS_WINDOW

	
	void Engine::Shutdown( void )
	{
		PSX_SafeDelete( m_pResCache );
		m_pFileSystem = PSX_NULL;
		m_pInputSystem = NULL;
		PSX_SafeDelete( m_pScriptParser );
		PSX_SafeDelete( m_pEventManager );
		PSX_SafeDelete( m_pProcessManager );
		PSX_SafeDelete( m_pSysClock );

		XMLParser::Shutdown();
		Clock::Shutdown();
		Math::Shutdown();
		InputSystem::GetInstance()->Shutdown();
		InputSystem::DeleteInstance();
		FileSystem::DeleteInstance();
		Log::Shutdown();
		System::Shutdown();
	}

	void Engine::Update( void )
	{
		// Update system clock
		U64 m_currPerfCounter = System::GetPerformanceCounter();
		m_pSysClock->UpdateCycles( m_currPerfCounter - m_lastPerfCounter );
		m_lastPerfCounter = m_currPerfCounter;

		InputSystem::GetInstance()->Update();

		// TODO: This shouldn't be hard coded.
		if ( m_pLogic )
		{
			m_pLogic->Update( m_pSysClock->GetTimeSeconds(), m_pSysClock->GetDeltaSeconds() );
			m_pLogic->Render( m_pSysClock->GetTimeSeconds(), m_pSysClock->GetDeltaSeconds() );
		}

		// Run Process manager
		m_pProcessManager->Update( m_pSysClock->GetDeltaSeconds() );

		// Run event manager
		// TODO: In the future, we can compute allowable time for the event manager to take
		//		to avoid eating to much time and cause frame lag.
		m_pEventManager->ProcessEvents();
	}

#ifdef __PSX_WINDOWS_WINDOW
	Renderer * Engine::CreateRenderer( ERendererType::Type type )
	{
		Renderer *pRenderer;
		RendererDesc desc;

		// Create graphics device and immediate renderer
		// We can create deferred renderer later for other threads.
		desc.SetAsImmediate( m_settings.rendererProcessing, m_settings.displaySettings.adapter,
			m_settings.displaySettings.adapterOutput, TRUE, TRUE, m_settings.displaySettings );
		
		switch( type )
		{
		case ERendererType::DX9:
			//return Renderer::GetInstance();
			break;
		case ERendererType::DX11:
			// TODO: Implement DX11 renderer
			pRenderer = new Renderer();
			break;
		case ERendererType::UNKNOWN:
		default:
			PSX_PushError( "Unknown renderer type." );
			break;
			return PSX_NULL;
		}

		if ( pRenderer->Initialize( &desc ) != EErrorCode::NONE )
		{
			PSX_PushError( "Failed to initialize renderer." );
			return PSX_NULL;
		}

		return pRenderer;
	}
#elif defined(__PSX_WINDOWS_CONSOLE)
	Renderer * Engine::CreateRenderer( ERendererType::Type type )
	{
		// No renderer for console
		return PSX_NULL;
	}
#endif __PSX_WINDOWS_WINDOW

	BOOL Engine::MessageProc( AppMsg *pMsg )
	{
		if ( m_pLogic )
		{
			if ( m_pLogic->OnMessageProc( pMsg ) )
				return TRUE;
		}

		return FALSE;
	}

	void Engine::OnUpdate( void )
	{

	}
	
	void Engine::OnRender( void )
	{

	}

	void Engine::SetApplicationLogic( Logic *pLogic ) 
	{ 
		m_pLogic = pLogic;
		pLogic->Initialize();
	}


}