/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUISystem.cpp
*
*	Comments	-	See GUISystem.h
*
**************************************************************************************/
#include "../Include/GUISystem.h"
#include "../Include/Screen.h"
#include "../Include/AppMsg.h"

namespace Pulse
{
	GUISystem::GUISystem( void )
	{
	}

	GUISystem::~GUISystem( void )
	{
	
	}

	EErrorCode::Type GUISystem::Initialize( void )
	{
		return EErrorCode::NONE;
	}

	void GUISystem::Shutdown( void )
	{

	}

	void GUISystem::Update( DOUBLE time, FLOAT dt )
	{
		ScreenList::Iterator iter = m_screenList.IteratorBeginLast();
		ScreenList::Iterator iterEnd = m_screenList.IteratorEnd();

		for ( ; iter != iterEnd; --iter )
		{
			(*iter)->Update( time, dt );
		}
	}

	void GUISystem::Render( void )
	{
		ScreenList::Iterator iter = m_screenList.IteratorBegin();
		ScreenList::Iterator iterEnd = m_screenList.IteratorEnd();

		for ( ; iter != iterEnd; ++iter )
		{
			(*iter)->Render();
		}
	}

	void GUISystem::AddScreen( Screen *pScreen )
	{
		pScreen->Initialize();
		m_screenList.PushBack( pScreen );
	}

	void GUISystem::RemoveScreen( Screen *pScreen )
	{
		pScreen->Cleanup();
		m_screenList.Remove( pScreen );
	}

	BOOL GUISystem::OnMessageProc( AppMsg *pMsg )
	{
		switch( pMsg->msg )
		{
		case EAppMessage::KEYDOWN:
		case EAppMessage::KEYUP:
		case EAppMessage::MOUSEMOVE:
		case EAppMessage::MOUSELBUTTONDOWN:
		case EAppMessage::MOUSELBUTTONUP:
		case EAppMessage::MOUSERBUTTONDOWN:
		case EAppMessage::MOUSERBUTTONUP:
			m_screenList.GetBack()->OnMessageProc( pMsg );
			return TRUE;
		}

		return FALSE;
	}
}