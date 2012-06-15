/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Mutex.cpp
*
*	Comments	-	See Mutex.h
*
**************************************************************************************/

#include "../Include/Mutex.h"

namespace Pulse
{
	BOOL Mutex::TryLock( void )
	{
		if ( TryEnterCriticalSection( &m_criticalSection ) != 0 )
		{
			PSX_Assert( !m_bLocked, "Mutex should be unlock at destruction time." );
			m_bLocked = TRUE;
			return TRUE;
		}

		return FALSE;
	}
}