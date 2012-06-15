/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ProcessManager.h
*
*	Description -	Simple cooperative Multitasker.
*
*	Comments	-	This file contains two classes. Process and ProcessManager.
*					Process is used by the ProcessManager to handle updatable systems
*						derived from Process base class.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/27/2009		-	File creation
**************************************************************************************/

#ifndef _PSX_PROCESS_MANAGER_H_
#define _PSX_PROCESS_MANAGER_H_

#include "PulseSTD.h"
#include "NonCopyable.h"
#include "SmartPointer.h"
#include "List.h"

namespace Pulse
{
	class Process : NonCopyable
	{
	public:

		typedef SmartPointer< Process > ProcessPointer;

		Process( INT type, UINT order = 0 );
		virtual ~Process( void );

		void Start( void );
		void Stop( void );
		void Pause( void );
		void Resume( void );
		BOOL IsPaused( void );
		void TogglePaused( void );
		void Terminate( void );

		BOOL IsActive( void );

		/********************************************************************************
		*	Function Name :	IsRunning
		*	Description	  :	Checks both if Active and NOT paused.
		*	In			  :	
		*	Out			  :	
		*	Return		  :	BOOL - True if it's Active and NOT paused. Otherwise false.
		*	Example		  : 
		********************************************************************************/
		BOOL IsRunning( void );

		BOOL IsDead( void );

		ProcessPointer AttachProcess( ProcessPointer pProcess, BOOL bSynchronize = FALSE );
		void DetachProcess( void );
		ProcessPointer GetAttachment( void );
		BOOL HasAttachment( void );

	protected:

		// Derived classes can override these functions.
		virtual void OnInitialize( void )	{ }
		virtual void OnStart( void )		{ }
		virtual void OnStop( void )			{ }
		virtual void OnUpdate( FLOAT dt )	{ }
		virtual void OnPause( void )		{ }
		virtual void OnResume( void )		{ }
		virtual void OnTerminate( void )	{ }

	private:

		void Disable( void );
		void Enable( void );

		// Used by the MemoryManager
		void Update( FLOAT dt );

		friend class ProcessManager;

		INT				m_type;
		UINT			m_order;
		BOOL			m_bPaused;
		BOOL			m_bActive;
		BOOL			m_bKill;
		BOOL			m_bInitialUpdate;

		// Attached Process
		ProcessPointer	m_pAttachedProcess;
		BOOL			m_bSyncProcess;

	};


	class ProcessManager : NonCopyable
	{
	public:

		void Update( FLOAT dt );
		
		void AddProcess( Process::ProcessPointer pProcess );

		void RemoveProcess( Process::ProcessPointer pProcess );

		BOOL HasProcesses( void );

		~ProcessManager( void );

	private:

		typedef List< Process::ProcessPointer > ProcessList;

		ProcessList m_processList;

	};

	// Process Inlined Definitions
	//////////////////////////////

	PSX_INLINE Process::Process( INT type, UINT order /* = 0 */ )
		: m_type( type ), m_order( order ) , m_bPaused( FALSE ), m_bActive( TRUE ), m_bKill( FALSE ),
		m_bInitialUpdate( TRUE ), m_pAttachedProcess( NULL ), m_bSyncProcess( FALSE )
	{
	}

	PSX_INLINE Process::~Process( void )
	{
	}

	PSX_INLINE BOOL Process::IsPaused( void )
	{
		return m_bPaused;
	}

	PSX_INLINE void Process::TogglePaused( void )
	{
		IsPaused() ? Resume() : Pause();
	}

	PSX_INLINE void Process::Terminate( void )
	{
		m_bKill = TRUE;
	}

	PSX_INLINE void Process::Disable( void )
	{
		m_bActive = FALSE;
	}

	PSX_INLINE void Process::Enable( void )
	{
		m_bActive = TRUE;
	}

	PSX_INLINE BOOL Process::IsActive( void )
	{
		return m_bActive;
	}

	PSX_INLINE BOOL Process::IsRunning( void )
	{
		return IsActive() && !IsPaused();
	}

	PSX_INLINE BOOL Process::IsDead( void )
	{
		return m_bKill;
	}

	PSX_INLINE Process::ProcessPointer Process::AttachProcess( ProcessPointer pProcess, BOOL bSynchronize /* = FALSE */ )
	{
		m_pAttachedProcess = pProcess;
		m_bSyncProcess = bSynchronize;
		return pProcess;
	}
	
	PSX_INLINE void Process::DetachProcess( void )
	{
		m_pAttachedProcess = NULL;
	}

	PSX_INLINE Process::ProcessPointer Process::GetAttachment( void )
	{
		return m_pAttachedProcess;
	}

	PSX_INLINE BOOL Process::HasAttachment( void )
	{
		return !m_pAttachedProcess.IsNull();
	}

	// ProcessManager
	///////////////////

	PSX_INLINE void ProcessManager::AddProcess( Process::ProcessPointer pProcess )
	{
		m_processList.PushBack( pProcess );
	}
	
	PSX_INLINE BOOL ProcessManager::HasProcesses( void )
	{
		return m_processList.GetSize() > 0 ? TRUE : FALSE;
	}

	PSX_INLINE void ProcessManager::RemoveProcess( Process::ProcessPointer pProcess )
	{
		m_processList.Remove( pProcess );
	}


}

#endif /* _PSX_PROCESS_MANAGER_H_ */