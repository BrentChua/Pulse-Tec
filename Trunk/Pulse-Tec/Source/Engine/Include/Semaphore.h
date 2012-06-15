/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- Semaphore.h
*
*	Description - Semaphore class
*
*	Comments	- First file written in 2012!
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	01/01/2012	-	Creation of this file.
**************************************************************************************/
#ifndef _PSX_SEMAPHORE_H_
#define _PSX_SEMAPHORE_H_

#include "Pulse.h"
#include "SmartPointer.h"

namespace Pulse
{
	class Semaphore;
	typedef SmartPointer<Semaphore> SemaphorePtr;

	class Semaphore
	{
	public:

		static SemaphorePtr Create( UINT initialCount, UINT maximumCount, const CHAR *pName = PSX_NULL );

		~Semaphore( void );

		BOOL WaitForSignaled( DWORD milliSeconds = INFINITE );

		void Release( UINT count = 1 );

	private:

		Semaphore( Semaphore & );

		Semaphore & operator = ( Semaphore &rhs );

		Semaphore( UINT initialCount, UINT maximumCount, const CHAR *pName = PSX_NULL );

	private:

		HANDLE m_semaphore;
	};


}

#endif /* _PSX_SEMAPHORE_H_ */