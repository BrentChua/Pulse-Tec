/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Logic.cpp
*
*	Comments	-	See Logic.h
*
**************************************************************************************/

#include "../Include/Logic.h"
#include "../Include/View.h"

namespace Pulse
{
	Logic::Logic( void )
		: m_pProcessManager( PSX_NULL ), m_state( EApplicationState::STARTUP )
	{

	}

	Logic::~Logic( void )
	{
		Shutdown();
	}

	EErrorCode::Type Logic::Initialize( void )
	{
		m_pProcessManager = new ProcessManager;
		m_state = EApplicationState::STARTUP;

		m_newActorID = 1;

		if ( m_pProcessManager == PSX_NULL )
			return EErrorCode::LOGIC;

		return EErrorCode::NONE;
	}

	void Logic::Shutdown( void )
	{
		PSX_SafeDelete( m_pProcessManager );
		m_state = EApplicationState::STARTUP;
	}

	void Logic::ChangeState( EApplicationState::Type state )
	{
		m_state = state;
		OnChangeState( state );
	}

	void Logic::AddView( View *pView )
	{
		m_viewList.PushBack( pView );
	}

	void Logic::RemoveView( View *pView )
	{
		m_viewList.Remove( pView );
	}

	BOOL Logic::OnMessageProc( AppMsg *pMsg )
	{
		ViewList::Iterator iter = m_viewList.IteratorBegin();
		ViewList::Iterator iterEnd = m_viewList.IteratorEnd();

		while ( iter != iterEnd )
		{
			if ( (*iter)->OnMessageProc( pMsg ) )
				return TRUE;

			++iter;
		}

		return FALSE;
	}

	void Logic::UpdateViews( DOUBLE time, FLOAT dt )
	{
		ViewList::Iterator iter = m_viewList.IteratorBegin();
		ViewList::Iterator iterEnd = m_viewList.IteratorEnd();

		while ( iter != iterEnd )
		{
			(*iter)->Update( time, dt );
			++iter;
		}
	}

	void Logic::RenderViews( DOUBLE time, FLOAT dt )
	{
		ViewList::Iterator iter = m_viewList.IteratorBegin();
		ViewList::Iterator iterEnd = m_viewList.IteratorEnd();

		while ( iter != iterEnd )
		{
			(*iter)->Render( time, dt );
			++iter;
		}
	}

	void Logic::Update( DOUBLE time, FLOAT dt )
	{
		OnUpdate( time, dt );
		UpdateViews( time, dt );

		switch( m_state )
		{
		case EApplicationState::STARTUP:
			// Do some pre initialization work
			ChangeState( EApplicationState::INITIALIZE );
			break;
		case EApplicationState::INITIALIZE:
			ChangeState( EApplicationState::RUNNING );
			break;
		case EApplicationState::RUNNING:
			m_pProcessManager->Update( dt );
			break;
		case EApplicationState::SHUTDOWN:
			break;
		default:
			PSX_Assert( FALSE, "Undefined logic state." );
			break;
		};

	}

	void Logic::Render( DOUBLE time, FLOAT dt )
	{
		//OnRender
		RenderViews( time, dt );
	}

}