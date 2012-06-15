/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Logic.h
*
*	Description -	The application logic where all data structures related to the
*						applications are located. 
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/16/2011	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_LOGIC_H_
#define _PSX_LOGIC_H_

#include "PulseSTD.h"
#include "ProcessManager.h"
#include "List.h"

namespace Pulse
{
	PSX_EnumBegin( EApplicationState )
		STARTUP,
		INITIALIZE,
		RUNNING,
		SHUTDOWN,
	PSX_EnumEnd()

	class Logic
	{
	public:

		Logic( void );

		virtual ~Logic( void );

		virtual EErrorCode::Type Initialize( void );

		virtual void Shutdown( void );

		virtual EErrorCode::Type Load( void ) { return EErrorCode::NONE; }

		virtual void OnUpdate( DOUBLE time, FLOAT dt ) { }

		// NOTE: Might want to add an OnRender() function here?

		virtual void OnChangeState( EApplicationState::Type state ) { }

		void AddProcess( Process::ProcessPointer pProcess );

		ActorID GetNewActorID( void ) { return m_newActorID++; }

		// NOTE: 0 is invalid
		ActorID GetLastActorID( void ) { return m_newActorID - 1; }

		void AddView( View *pView );

		void RemoveView( View *pView );

		BOOL OnMessageProc( AppMsg *pMsg );

	private:

		friend class Engine;

		void UpdateViews( DOUBLE time, FLOAT dt );

		void RenderViews( DOUBLE time, FLOAT dt );

		void Update( DOUBLE time, FLOAT dt );

		void Render( DOUBLE time, FLOAT dt );

		void ChangeState( EApplicationState::Type state );

	protected:

		typedef List<View*> ViewList;

		ProcessManager			*m_pProcessManager;
		EApplicationState::Type	m_state;
		ActorID					m_newActorID;
		ViewList				m_viewList;

	};

	PSX_INLINE void Logic::AddProcess( Process::ProcessPointer pProcess )
	{
		m_pProcessManager->AddProcess( pProcess );
	}

}

#endif /* _PSX_LOGIC_H_ */