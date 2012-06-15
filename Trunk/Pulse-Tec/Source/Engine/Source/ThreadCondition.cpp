/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ThreadCondition.cpp
*
*	Comments	-	See ThreadCondition.h
*
**************************************************************************************/

#include "../Include/ThreadCondition.h"

namespace Pulse
{

	ThreadCondition::ThreadCondition( void )
		: m_hGateSemaphore( CreateSemaphore( NULL, 1, 1, NULL ) ),
		m_hQueueSemaphore( CreateSemaphore( NULL, 0, PSX_INT_MAX, NULL ) ),
		m_numGone( 0 ), m_numBlocked( 0 ), m_numWaiting( 0 )
	{

	}

	ThreadCondition::~ThreadCondition( void )
	{
		CloseHandle( m_hGateSemaphore );
		CloseHandle( m_hQueueSemaphore );
	}

	void ThreadCondition::NotifyOne( void )
	{
		UINT numSignals = 0;

		{
			MutexLocker lock( m_mutex );

			if ( m_numWaiting != 0 )
			{
				if ( m_numBlocked == 0 )
					return;

				++m_numWaiting;
				--m_numBlocked;
				numSignals = 1;
			}
			else
			{
				PSX_ValidateNoMsg( WaitForSingleObject( m_hGateSemaphore, INFINITE ) == WAIT_OBJECT_0 );

				if ( m_numBlocked > m_numGone )
				{
					if ( m_numGone != 0 )
					{
						m_numBlocked -= m_numGone;
						m_numGone = 0;
					}

					numSignals = m_numWaiting = 1;
					--m_numBlocked;
				}
				else
				{
					PSX_ValidateNoMsg( ReleaseSemaphore( m_hGateSemaphore, 1, NULL ) != 0 );
				}
			}
		}

		if ( numSignals != 0 )
			PSX_ValidateNoMsg( ReleaseSemaphore( m_hQueueSemaphore, numSignals, NULL ) != 0 );
	}

	void ThreadCondition::NotifyAll( void )
	{
		UINT numSignals = 0;

		{
			MutexLocker lock( m_mutex );

			if ( m_numWaiting != 0 )
			{
				if ( m_numBlocked == 0 )
					return;

				numSignals = m_numBlocked;
				m_numWaiting += m_numBlocked;
				m_numBlocked = 0;
			}
			else
			{
				PSX_ValidateNoMsg( WaitForSingleObject( m_hGateSemaphore, INFINITE ) == WAIT_OBJECT_0 );

				if ( m_numBlocked > m_numGone )
				{
					if ( m_numGone != 0 )
					{
						m_numBlocked -= m_numGone;
						m_numGone = 0;
					}

					numSignals = m_numBlocked;
					m_numWaiting = m_numBlocked;
					m_numBlocked = 0;
				}
				else
				{
					PSX_ValidateNoMsg( ReleaseSemaphore( m_hGateSemaphore, 1, NULL ) != 0 );
				}
			}
		}

		if ( numSignals != 0 )
			PSX_ValidateNoMsg( ReleaseSemaphore( m_hQueueSemaphore, numSignals, NULL ) != 0 );
	}

	void ThreadCondition::EnterWait( void )
	{
		PSX_ValidateNoMsg( WaitForSingleObject( m_hGateSemaphore, INFINITE ) == WAIT_OBJECT_0 );
		++m_numBlocked;
		PSX_ValidateNoMsg( ReleaseSemaphore( m_hGateSemaphore, 1, NULL) != 0 );
	}

	BOOL ThreadCondition::Wait( UINT milliseconds )
	{
		UINT wasWaiting;
		UINT wasGone;
		
		BOOL bRet = WaitForSingleObject( m_hQueueSemaphore, milliseconds ) == WAIT_OBJECT_0;

		{
			MutexLocker lock( m_mutex );

			wasWaiting = m_numWaiting;
			wasGone = m_numGone;

			if ( wasWaiting != 0 )
			{
				if ( bRet == FALSE )
				{
					if ( m_numBlocked != 0 )
					{
						--m_numBlocked;
					}
					else
					{
						++m_numGone;
					}
				}

				if ( --m_numWaiting == 0 )
				{
					if ( m_numBlocked != 0 )
					{
						PSX_ValidateNoMsg( ReleaseSemaphore( m_hGateSemaphore, 1, NULL ) != 0 );
						wasWaiting = 0;
					}
					else if ( m_numGone != 0 )
					{
						m_numGone = 0;
					}
				}
			}
			else if ( ++m_numGone == (PSX_UINT_MAX / 2 ) )
			{
				PSX_ValidateNoMsg( WaitForSingleObject( m_hGateSemaphore, INFINITE ) == WAIT_OBJECT_0 );
				m_numBlocked -= m_numGone;
				PSX_ValidateNoMsg( ReleaseSemaphore( m_hGateSemaphore, 1, NULL ) != 0 );

				m_numGone = 0;
			}
		}

		if ( wasWaiting == 1 )
		{
			while ( wasGone )
			{
				PSX_ValidateNoMsg( WaitForSingleObject( m_hQueueSemaphore, INFINITE ) == WAIT_OBJECT_0 );
				--wasGone;
			}
		}

		return bRet;
	}
}