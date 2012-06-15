/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	TaskScheduler.h
*
*	Description -	Tasking system that propagates tasks to worker threads.
*
*	Comments	-	Task scheduler creates n threads where n is the number of physical
*						CPU cores.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/30/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_TASK_SCHEDULER_H_
#define _PSX_TASK_SCHEDULER_H_

#include "PulseSTD.h"
#include "PulseThreading.h"
#include "Singleton.h"
#include "Queue.h"
#include "SmartPointer.h"

namespace Pulse
{
	class TaskScheduler;

	class PSX_CacheLineAligned TaskCompletion
	{
		TaskCompletion( void )
			: m_status(0)
		{
		}

		BOOL IsBusy( void ) const
		{
			return m_status > 0;
		}

		void SetBusy( void )
		{
			// We increment so that it tracks how many threads marked
			//	this task as busy.
			++m_status;
		}

		void SetComplete( void )
		{
			// Once all threads are done working and sets it back to 
			//	complete it returns back to 0.
			--m_status;
		}

		BOOL IsComplete( void ) const
		{
			return m_status == 0;
		}

	private:

		AtomicInt m_status;
	};

	class ThreadTask
	{
	public:

		// Automatically creates a TasCompletion for you
		ThreadTask( void );

		// When duplicating ThreadTasks that shares one pCompletion
		ThreadTask( TaskCompletion *pCompletion );

		virtual BOOL Run( WorkerThread *pThread ) = 0;

		virtual bool Split( WorkerThread *pThread );

	private:

		BOOL m_bCompletionOwner;
		TaskCompletion *m_pCompletion;
	};

	class WorkerThread 
	{
	public:

		WorkerThread( TaskScheduler *pScheduler );

	private:

		friend static void WorkerThreadMain( void *pWorkerThread );

		DWORD ThreadProc( void );

		void Idle( void );

		BOOL PushTask( ThreadTask *pTask );

		BOOL PopTask( ThreadTask **ppTask );

		BOOL StealTasks( void );

		BOOL GiveUpSomeWork( WorkerThread *pIdleThread );

	private:

		typedef Queue< ThreadTask * > TaskList;

		Mutex			m_taskMutex;
		TaskScheduler	*m_pScheduler;
		Thread			m_thread;
		TaskList		m_tasks;

	};

	class TaskScheduler : public Singleton<TaskScheduler>
	{
	public:

		virtual ~TaskScheduler( void );

		SIZE_T GetNumThreads( void );

	private:

		TaskScheduler( void );

		EErrorCode::Type Initialize( void );

		void Shutdown( void );

	private:

		friend Singleton<TaskScheduler>;

		friend Engine;

		WorkerThread *m_pThreads;
		SIZE_T m_numThreads;
		SemaphorePtr m_sleepNotifier;

	};
}

#endif /* _PSX_TASK_SCHEDULER_H_ */