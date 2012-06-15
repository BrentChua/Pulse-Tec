/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ProcessManager.cpp
*
*	Comments	-	See ProcessManager.h
*
**************************************************************************************/

#include "../Include/ProcessManager.h"

namespace Pulse
{
	void Process::Start( void )
	{
		if ( !IsActive() )
		{
			Enable();
			OnStart();
			m_bPaused = FALSE;
		}
	}

	void Process::Stop( void )
	{
		if ( IsActive() )
		{
			Disable();
			OnStop();
			m_bPaused = TRUE;
		}
	}

	void Process::Pause( void )
	{
		if ( m_bPaused == FALSE )
		{
			OnPause();
			m_bPaused = TRUE;
		}
	}

	void Process::Resume( void )
	{
		if ( m_bPaused == TRUE )
		{
			OnResume();
			m_bPaused = FALSE;
		}
	}

	void Process::Update( FLOAT dt )
	{
		// Update this process first
		if ( m_bInitialUpdate )
		{
			OnInitialize();
			Start();
			m_bInitialUpdate = FALSE;
		}

		OnUpdate( dt );

		// Check if we have something attached and 
		// see if this is in sync with this current process.
		if ( m_bSyncProcess && HasAttachment() )
		{
			if ( m_pAttachedProcess->IsDead() )
			{
				// Detach current process and attach if there's a child process
				if ( m_pAttachedProcess->HasAttachment() )
				{
					ProcessPointer pAttachedProcess( GetAttachment() );
					m_pAttachedProcess->OnTerminate();
					m_bSyncProcess = m_pAttachedProcess->m_bSyncProcess;
					m_pAttachedProcess->DetachProcess();
					AttachProcess( pAttachedProcess->GetAttachment() );
				}
				else
				{
					m_bSyncProcess = FALSE;
					m_pAttachedProcess->OnTerminate();
					DetachProcess();
				}
			}
			else if ( m_pAttachedProcess->IsRunning() )
				m_pAttachedProcess->Update( dt );
		}
	}

	// Process Manager Function Definitions
	/////////////////////////////////////////

	void ProcessManager::Update( FLOAT dt )
	{
		ProcessList::Iterator iter = m_processList.IteratorBegin();
		ProcessList::Iterator iterEnd = m_processList.IteratorEnd();
		Process::ProcessPointer pProcess;

		while ( iter != iterEnd )
		{
			pProcess = *iter;

			if ( pProcess->IsDead() )
			{
				if ( pProcess->HasAttachment() )
				{
					Process::ProcessPointer pAttachedProcess( pProcess->GetAttachment() );
					pProcess->DetachProcess();

					AddProcess( pAttachedProcess );
				}

				// Call OnStop, OnTerminate then remove from the process list.
				pProcess->OnStop();
				pProcess->OnTerminate();
				iter = m_processList.Remove( iter );
				
				continue;
			}
			
			if ( pProcess->IsRunning() )
				pProcess->Update( dt );

			++iter;
		}
	}

	ProcessManager::~ProcessManager( void )
	{
		// Force the remaining process to terminate
		ProcessList::Iterator iter = m_processList.IteratorBegin();
		ProcessList::Iterator iterEnd = m_processList.IteratorEnd();
		Process::ProcessPointer pProcess;

		while ( iter != iterEnd )
		{
			pProcess = *iter;

			if ( pProcess->HasAttachment() )
			{
				Process::ProcessPointer pAttachedProcess( pProcess->GetAttachment() );
				pProcess->DetachProcess();

				AddProcess( pAttachedProcess );
			}

			// Call OnStop and OnTerminate then remove from the process list.
			pProcess->OnStop();
			pProcess->OnTerminate();
			iter = m_processList.Remove( iter );

			continue;

			++iter;
		}
	}

};