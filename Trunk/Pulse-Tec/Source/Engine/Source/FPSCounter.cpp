/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	FPSCounter.cpp
*
*	Comments	-	See FPSCounter.h
*
**************************************************************************************/

#include "../Include/FPSCounter.h"
#include "../Include/System.h"

namespace Pulse
{
	FPSCounter::FPSCounter( FLOAT meanTime )
		: m_totalTime( 0.0f ), m_meanTime( meanTime ), m_totalFrames( 0 ), m_lastFPS( 0 )
	{

	}

	void FPSCounter::Update( FLOAT dt )
	{
		m_totalTime += dt;
		++m_totalFrames;

		if ( m_totalTime >= m_meanTime )
		{
			// Compute FPS now!
			m_lastFPS = UINT(FLOAT(m_totalFrames) / m_totalTime);

			// Reset counters
			m_totalFrames = 1;
			m_totalTime -= m_meanTime;
		}
	}

	void FPSCounter::Update( I64 cycles )
	{
		Update( Clock::CyclesToSeconds( cycles ) );
	}

	void FPSCounter::Update( U64 cycles )
	{
		Update( Clock::CyclesToSeconds( cycles ) );
	}

	void FPSCounter::Update( I32 cycles )
	{
		Update( Clock::CyclesToSeconds( cycles ) );
	}

	void FPSCounter::Update( U32 cycles )
	{
		Update( Clock::CyclesToSeconds( cycles ) );
	}

	
	FPSAutoCounter::FPSAutoCounter( FLOAT meanTime )
		: FPSCounter( meanTime ), m_lastTimeCycle( System::GetAppTimeCounter() )
	{

	}

	void FPSAutoCounter::Update( void )
	{
		U64 m_currCycle = System::GetAppTimeCounter();
		FPSCounter::Update( m_currCycle - m_lastTimeCycle );
		m_lastTimeCycle = m_currCycle;
	}

}