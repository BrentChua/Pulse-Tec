/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Mutex.h
*
*	Description -	Mutex.
*
*	Comments	-	Take note that the mutex class here uses a CRITICAL_SECTION internally.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/14/2010	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_MUTEX_H_
#define _PSX_MUTEX_H_

#include "PulseSTD.h"
#include "NonCopyable.h"
#include <windows.h>


namespace Pulse
{
	class Mutex : private NonCopyable
	{
	public:

		Mutex( void );

		~Mutex( void );

	protected:

		template<BOOL> friend class _MutexLocker;

		void Lock( void );

		BOOL TryLock( void );

		void Unlock( void );

	private:

		CRITICAL_SECTION m_criticalSection;
		BOOL m_bLocked;

	};

	PSX_INLINE Mutex::Mutex( void )
		: m_bLocked( FALSE )
	{
		InitializeCriticalSection( &m_criticalSection );
	}

	PSX_INLINE Mutex::~Mutex( void )
	{
		PSX_Assert( !m_bLocked, "Mutex should already be unlocked." );
		DeleteCriticalSection( &m_criticalSection );
	}

	PSX_INLINE void Mutex::Lock( void )
	{
		EnterCriticalSection( &m_criticalSection );
		PSX_Assert( !m_bLocked, "Mutex should be unlock at destruction time." );
		m_bLocked = TRUE;
	}

	PSX_INLINE void Mutex::Unlock( void )
	{
		PSX_Assert( m_bLocked, "Unlocking a mutex that isn't unlocked." );
		m_bLocked = FALSE;
		LeaveCriticalSection( &m_criticalSection );
	}

	// RAII
	template < BOOL bTryLock >
	class _MutexLocker : private NonCopyable
	{
	public:

		// NOTE: Lock mutex according to bBlockOnCreate value.
		_MutexLocker( Mutex *pMutex, BOOL bBlockOnCreate  = TRUE ) 
			: m_pMutex( pMutex ), m_bLocked( bBlockOnCreate )
		  {
			  // Early out if not lock required
			  if( bBlockOnCreate == FALSE )
				  return;

			  // Lock or try to lock according to template argument
			  if( bTryLock == TRUE )
				  m_bLocked = m_pMutex->TryLock();
			  else
				  m_pMutex->Lock();
		  }

		// NOTE: Lock mutex according to bBlockOnCreate value.
		_MutexLocker( Mutex &mutex, BOOL bBlockOnCreate  = TRUE ) 
			: m_pMutex(&mutex), m_bLocked( bBlockOnCreate )
		{
			// Early out if not lock required
			if( bBlockOnCreate == FALSE )
				return;

			// Lock or try to lock according to template argument
			if( bTryLock == TRUE )
				m_bLocked = m_pMutex->TryLock();
			else
				m_pMutex->Lock();
		}

		  // NOTE: Destruct a locker object. Unlock the mutex if it is locked
		  ~_MutexLocker( void )
		  {
			  // Only unlock locked mutex
			  if( IsLocked() == TRUE )
				  m_pMutex->Unlock();
		  }

		  // NOTE: Lock this mutex
		  void Lock( void )
		  {
			  PSX_Assert( m_bLocked == FALSE, "Mutex scope is already locked" );

			  m_pMutex->Lock();
			  m_bLocked = true;
		  }

		  // NOTE: Try to lock this mutex without waiting for its ownership. Return true if lock succeeded, false otherwise
		  BOOL TryLock( void )
		  {
			  PSX_Assert( m_bLocked == FALSE, "Mutex scope is already locked" );
			  m_bLocked = m_pMutex->TryLock();
			  return m_bLocked;
		  }

		  // NOTE: Unlock this mutex
		  void Unlock( void )
		  {
			  PSX_Assert( m_bLocked == TRUE, "Mutex scope isn't locked" );
			  m_pMutex->Unlock();
			  m_bLocked = false;
		  }

		  // NOTE: Get if mutex is locked by this locker
		  BOOL IsLocked( void )
		  {
			  return m_bLocked;
		  }

	private:

		Mutex *m_pMutex;

		// Lock status
		BOOL m_bLocked;
	};

	typedef _MutexLocker< TRUE > MutexLocker;
	typedef _MutexLocker< FALSE > MutexTryLocker;
}

#endif /* _PSX_MUTEX_H_ */