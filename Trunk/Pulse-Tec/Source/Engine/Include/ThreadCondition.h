/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- ThreadCondition.h
*
*	Description - A condition object that works in conjunction with MutexLocker.
*
*	Comments	- Implementation details were taken from boost thread library (http://www.boost.org/).
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/22/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_THREAD_CONDITION_H_
#define _PSX_THREAD_CONDITION_H_

#include "PulseSTD.h"
#include "Mutex.h"
#include "NonCopyable.h"

#include <windows.h>

namespace Pulse
{

	class ThreadCondition : public NonCopyable
	{
	public:

		ThreadCondition( void );

		~ThreadCondition( void );

		// NOTE: If there is a thread waiting on "this", change that thread's state to ready.
		//	Otherwise, there is no effect.
		void NotifyOne( void );

		// NOTE: Change the state of all threads waiting on "this" to ready.
		//	If there is no waiting threads, NotifyAll has no effect.
		void NotifyAll( void );

		// NOTE: Releases the lock on the mutex object associated with lock, blocks the current 
		//	thread of execution until readied by a call to Notify...(), and then re-acquire the lock.
		template < typename ScopedLock >
		void Wait( ScopedLock *pLock );

		// NOTE: Releases the lock on the mutex object associated with lock, blocks the current thread
		//	of execution until readied by a call to Notify...(), or until time milliseconds is reached.
		template < typename ScopedLock >
		BOOL TimedWait( ScopedLock *pLock, UINT milliseconds );

	private:

		// NOTE: Called before wait
		void EnterWait( void );

		// NOTE: Wait...
		BOOL Wait( UINT milliseconds );

	private:

		HANDLE	m_hGateSemaphore;	// Internal gate Semaphore
		HANDLE	m_hQueueSemaphore;	// Internal queue Semaphore
		Mutex	m_mutex;
		UINT	m_numGone;			// Number of threads that timed out and never made it to m_hQueueSemaphore
		UINT	m_numBlocked;		// Number of blocked threads on the condition
		UINT	m_numWaiting;		// Number of threads no longer waiting for the condition but still waiting to be removed from the queue

	};

	template < typename ScopedLock >
	void ThreadCondition::Wait( ScopedLock *pLock )
	{
		PSX_Assert( pLock->IsLocked(), "Scoped lock should be locked on entering this condition." );
		
		// Wait a mutex for condition to be signaled
		EnterWait();

		pLock->Unlock();
		PSX_ValidateNoMsg( Wait( INFINITE ) == TRUE );
		pLock->Lock();
	}

	template < typename ScopedLock >
	BOOL ThreadCondition::TimedWait( ScopedLock *pLock, UINT milliseconds )
	{
		PSX_Assert( pLock->IsLocked(), "Scope lock should be locked on entering this condition." );

		// Wait on mutex for condition to be signaled or timed out
		EnterWait();

		pLock->Unlock();
		BOOL bRet = Wait( milliseconds );
		pLock->Lock();

		return bRet;
	}

}

#endif /* _PSX_THREAD_CONDITION_H_ */