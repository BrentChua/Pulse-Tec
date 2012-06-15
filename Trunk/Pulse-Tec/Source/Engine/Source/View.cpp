/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUISystem.cpp
*
*	Comments	-	See GUISystem.h
*
**************************************************************************************/

#include "../Include/View.h"

namespace Pulse
{

	HumanView::HumanView( void )
	{
		m_bFullSpeedRendering = FALSE;
		m_lastTime = 0;
	}
	
	HumanView::~HumanView( void )
	{
	}

	EErrorCode::Type HumanView::OnRender( DOUBLE time, FLOAT deltaTime )
	{
		return EErrorCode::NONE;
	}

	void HumanView::OnUpdate( DOUBLE time, FLOAT deltaTime )
	{
	}

	BOOL HumanView::OnMessageProc( AppMsg *pMsg )
	{
		BOOL result;
		switch( pMsg->msg )
		{
		case EAppMessage::KEYDOWN:
		case EAppMessage::KEYUP:
		case EAppMessage::MOUSEMOVE:
		case EAppMessage::MOUSELBUTTONDOWN:
		case EAppMessage::MOUSELBUTTONUP:
		case EAppMessage::MOUSERBUTTONDOWN:
		case EAppMessage::MOUSERBUTTONUP:
			//result = m_GUISystem.OnMessageProc( pMsg );
			break;
		default:
			// hmm....
			break;
		}

		return result;
	}

	ApplicationViewType::Type HumanView::GetType( void ) const
	{
		return ApplicationViewType::HUMAN;
	}

	GameViewID HumanView::GetID( void ) const
	{
		return 0;
	}

	void HumanView::OnAttach( GameViewID viewID, Optional<ProgramID> programID )
	{

	}

	//void HumanView::AddScreen( Screen *pScreen )
	//{
	//}
	//
	//void HumanView::RemoveScreen( Screen *pScreen )
	//{
	//}

	void HumanView::Update( DOUBLE time, FLOAT deltaTime )
	{
	
		OnUpdate( time, deltaTime );
	}

	EErrorCode::Type HumanView::Render( DOUBLE time, FLOAT deltaTime )
	{
		const FLOAT REFRESHRATE = 1.0f/60.0f; 

		m_lastTime += deltaTime;

		if ( m_bFullSpeedRendering || m_lastTime > REFRESHRATE )
		{
			
			OnRender( time, deltaTime );
			//m_GUISystem.Render();

			//pRenderer->Render();

			m_lastTime = m_lastTime - REFRESHRATE;
		}

		return EErrorCode::NONE;
	}

}