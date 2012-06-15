#include "ViewDemo.h"

namespace Pulse
{
	SampleView::SampleView( void )
	{
		//AddScreen( new SampleScreen );
	}

	SampleView::~SampleView( void )
	{

	}

	EErrorCode::Type SampleView::OnRender( DOUBLE time, FLOAT deltaTime )
	{
		//Renderer *pRenderer = Renderer::GetInstance();
		//pRenderer->ClearBuffer( EClearBufferType::DEPTH | EClearBufferType::RENDER_TARGET );

		////pGUIRect->Render();
		//pImage->Render();
		//pImage2->Render();
		//pWindow->Render();


		return EErrorCode::NONE;
	}

	void SampleView::OnUpdate( DOUBLE time, FLOAT deltaTime )
	{

	}
}