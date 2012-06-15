/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Thread.cpp
*
*	Comments	-	See Thread.h
*
**************************************************************************************/

#include "../Include/Thread.h"
#include <process.h>

namespace Pulse
{
	// NOTE: Declares a thread pointer in the thread local storage to store calling thread Thread object.
	__declspec(thread) Thread * Thread::m_pThread = NULL;

	// NOTE: Used to associate a name to a thread that can be visible in the debugger.
	class ThreadNameInfo
	{
	public:

		ThreadNameInfo( const CHAR *pName )
			: m_type( 0x1000 ), m_pName( pName ), m_threadID( 0xffffffff ), m_flags( 0 )
		{
			// Send name via an exception to the debugger
			__try 
			{ 
				RaiseException( 0x406D1388, 0, sizeof(ThreadNameInfo) / sizeof(DWORD), (ULONG_PTR const *)this ); 
			}
			__except(EXCEPTION_CONTINUE_EXECUTION){}
		}

	private:

		DWORD		m_type;		// Must be 0x1000
		const CHAR *m_pName;	// Pointer to name (in user address space)
		DWORD		m_threadID;	// Thread ID (-1 = caller thread)
		DWORD		m_flags;	// Reserved for future use, must be zero

	};

	Thread::Thread( const CHAR *pName )
		: m_hThread( NULL ), m_threadUID( 0 ), m_pThreadFunc( NULL ),
		m_pArgs( NULL ), m_name( pName ), m_bJoinable( FALSE )
	{
		int bRet = DuplicateHandle( GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &m_hThread,
			SYNCHRONIZE | THREAD_QUERY_INFORMATION | THREAD_SET_INFORMATION, FALSE, 0 );
		PSX_Assert( bRet, "Thread failed to duplicate." );
	
		// Get thread UID
		m_threadUID = GetCurrentThreadId();
		// Set current thread pointer
		PSX_Assert( m_pThread == NULL, "m_pThread is not null." );
		m_pThread = this;
		ThreadNameInfo info( GetName() );
	}

	UINT __stdcall ThreadMainEntry( void *pThreadParam )
	{
		PSX_Assert( pThreadParam != NULL, "pArgs shouldn't be null." );

		// Fetch thread object
		Thread *pThread = (Thread*)pThreadParam;

		PSX_Assert( Thread::m_pThread == NULL, "m_pThread is already used." );
		Thread::m_pThread = pThread;

		// Create name RAII object
		ThreadNameInfo name( pThread->GetName() );

		// Execute thread
		(*pThread)();

		// Thread is dying. Reset current thread pointer as nobody could get it anymore.
		PSX_Assert( Thread::m_pThread != NULL, "m_pThread can't be null." );
		Thread::m_pThread = NULL;

		return 0;
	}

	void Thread::operator()( void )
	{
		(*m_pThreadFunc)( m_pArgs );
	}

	Thread::Thread( const CHAR *pName, ThreadFunc pThreadFunc, void *pArgs )
		: m_hThread( NULL ), m_threadUID( 0 ), m_pThreadFunc( pThreadFunc ),
			m_pArgs( pArgs ), m_name( pName ), m_bJoinable( TRUE )
	{
		m_hThread =  (HANDLE)_beginthreadex( NULL, 64<<10, ThreadMainEntry, this, 0, &m_threadUID );

		PSX_Assert( m_hThread, "Failed to create thread." );
	}

	Thread::~Thread( void )
	{
		PSX_Assert( m_bJoinable == FALSE, "This thread should be joined before being destroyed." );

		// Release thread handle
		if ( m_hThread != NULL )
		{
			CloseHandle( m_hThread );
			m_hThread = NULL;
		}
	}

	ThreadPtr Thread::Create( const CHAR *pName, ThreadFunc pThreadFunc, void *pArgs )
	{
		PSX_Assert( pThreadFunc != NULL, "Invalid pass pThreadFunc." );

		// Instantiate new thread object
		ThreadPtr pThread = new Thread( pName, pThreadFunc, pArgs );
		PSX_Assert( pThread.GetRefCount() >= 1, "Invalid ref count while creating a thread." );

		// Return new thread
		return pThread;
	}

	ThreadPtr Thread::Attach( const CHAR *pName )
	{
		PSX_Assert( m_pThread == PSX_NULL, "This thread is already attached to a Thread object." );

		// Instantiate a new thread object
		ThreadPtr pThread( new Thread( pName ) );

		return pThread;
	}

	void Thread::Join( void )
	{
		// See race condition comment below
		PSX_Assert( m_bJoinable, "This thread can not be joined." );

		// Just infinitely wait for thread termination
		if ( WaitForSingleObject( m_hThread, INFINITE ) != WAIT_OBJECT_0 )
		{
			PSX_Assert( FALSE, "WaitForSingleObject INFINITE on a thread should always return true." );
		}

		// Thread might have been joined by two threads
		PSX_Assert( m_bJoinable, "This thread isn't joinable." );

		// Thread is not joinable any more
		// This isnt a race condition since any race that could occur would have us in undefined behavior
		// territory any way.
		m_bJoinable = FALSE;
	}

	void Thread::Sleep( UINT milliseconds )
	{
		if ( milliseconds )
			::Sleep( milliseconds );
	}

	void Thread::Yield( void )
	{
		// Specify 0 to indicate this thread should be suspended to allow other waiting threads
		// to execute.
		::Sleep( 0 );
	}

	ThreadPtr Thread::GetCurrent( void )
	{
		PSX_Assert( Thread::m_pThread != NULL, "Invalid function call, no thread object pointer associated to this thread." );
		return m_pThread;
	}
}