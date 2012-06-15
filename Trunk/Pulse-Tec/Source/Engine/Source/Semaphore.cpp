/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Semaphore.cpp
*
*	Comments	-	See Semaphore.h
*
**************************************************************************************/
#include "../Include/Semaphore.h"

namespace Pulse
{
	Semaphore::Semaphore( UINT initialCount, UINT maximumCount, const CHAR *pName )
	{
		m_semaphore = CreateSemaphore( NULL, initialCount, maximumCount, pName );

	}

	SemaphorePtr Semaphore::Create( UINT initialCount, UINT maximumCount, const CHAR *pName )
	{
		return new Semaphore( initialCount, maximumCount, pName );
	}

	Semaphore::~Semaphore( void )
	{
		CloseHandle( m_semaphore );
	}

	BOOL Semaphore::WaitForSignaled( DWORD milliSeconds )
	{
		DWORD ret = WaitForSingleObject( m_semaphore, milliSeconds );
		return (ret == WAIT_OBJECT_0);
	}

	void Semaphore::Release( UINT count )
	{
		ReleaseSemaphore( m_semaphore, count, NULL );
	}
}