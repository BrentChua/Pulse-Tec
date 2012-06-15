#include "WindowDemo.h"

#ifdef PSX_PLATFORM_WINDOWS_WINDOW

namespace Pulse
{
	
	PSX_REGISTER_APPLICATION_CREATE( WindowDemo );
	PSX_REGISTER_APPLICATION_RELEASE();

	WindowDemo::WindowDemo( void )
	{

	}

	BOOL WindowDemo::OnStart( void )
	{
		Engine *pEngine = Engine::GetInstance();

		m_pLogic = new SampleLogic;
		pEngine->SetApplicationLogic( m_pLogic );

		m_pLogic->AddView( new SampleView );

		//this->ShowFPS( TRUE );
		//ToggleFullscreen();

		return TRUE;
	}

	void WindowDemo::OnUpdate( DOUBLE time, FLOAT dt, void *pContext )
	{


	}

	void WindowDemo::OnShutDown( void )
	{

	}
}

#endif /* PSX_PLATFORM_WINDOWS_WINDOW */