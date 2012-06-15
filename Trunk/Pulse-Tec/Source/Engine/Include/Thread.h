/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- Thread.h
*
*	Description - Thread object implementation..
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/18/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_THREAD_H_
#define _PSX_THREAD_H_

#include "PulseSTD.h"
#include "NonCopyable.h"
#include "SmartPointer.h"
#include "String.h"

// Undefine MS Yield macro in order to use this as a function name
#undef Yield

namespace Pulse
{
	class Thread;
	typedef SmartPointer< Thread > ThreadPtr;

	class Thread : private NonCopyable
	{
	public:

		// NOTE: Define thread entry function.
		typedef void (*ThreadFunc)(void*);

		// NOTE: The thread must have been joined before this funcion is called except if 
		//	the thread object was created from an existing OS thread.
		~Thread( void );

		// NOTE: Create a new thread that will execute entryFunc function.
		static ThreadPtr Create( const CHAR *pName, ThreadFunc threadFunc, void *pArgs );

		// NOTE: Attach a thread object to an existing OS thread. Only one thread
		//	could be attached at a time to an OS thread.
		static ThreadPtr Attach( const CHAR *pName );

		// NOTE: Join thread execution
		void Join( void );

		// NOTE: Comparison operators
		BOOL operator == ( const Thread &rhs ) const;
		BOOL operator != ( const Thread &rhs ) const;
	
		// NOTE: Sleep the calling thread for milliseconds
		static void Sleep( UINT milliseconds );

		// NOTE: Yield processing time to another thread
		static void Yield( void );

		// NOTE: Get current thread ThreadSmartPtr
		static ThreadPtr GetCurrent( void );
	
	private:

		// NOTE: Constructors are private to ensure proper public functions (Attah/Create) are used.

		// NOTE: Construt a Thread to be attached to the calling thread.
		Thread( const CHAR *pName );

		// NOTE: Construt a new thread.
		Thread( const CHAR *pName, ThreadFunc threadFunc, void *pArgs );

		// NOTE: Internal operator used to start thread entry function.
		void operator() ( void );

		// NOTE: Get thread name.
		const CHAR * GetName( void ) const;

	private:

		friend UINT __stdcall ThreadMainEntry( void *pThreadParam );
		
		// Declares a Thread* in the thread local storage to store all Thread object.
		static __declspec(thread) Thread * m_pThread;

		HANDLE		m_hThread;		// Thread handle
		UINT		m_threadUID;	// Thread Unique ID
		ThreadFunc	m_pThreadFunc;	// Thread function entry
		void		*m_pArgs;		// List of arguments
		String		m_name;			// Thread name
		BOOL		m_bJoinable;	// Is joinable

	};

	PSX_INLINE const CHAR * Thread::GetName( void ) const
	{
		return m_name.GetBuffer();
	}

}

#endif /* _PSX_THREAD_H_ */