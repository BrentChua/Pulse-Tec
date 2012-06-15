/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Clock.h
*
*	Description -	Clock class.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/10/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_CLOCK_H_
#define _PSX_CLOCK_H_

#include "PulseSTD.h"
#include "System.h"

namespace Pulse
{

	class Clock
	{
	public:

		Clock( F32 startTimeinSeconds = 0.0f );

		void Reset( F32 startTimeinSeconds = 0.0f );

		void UpdateSeconds( F32 dtSeconds );
		void UpdateCycles( U64 dtCycles );

		U64 GetTimeCycles( void ) const;
		F64 GetTimeSeconds( void ) const;

		U32 GetDeltaCycles( void ) const;
		F32 GetDeltaSeconds( void ) const;

		F32 CalculateDeltaSeconds( Clock *pClock );

		void SetPaused( BOOL bPaused );
		void Pause( void );
		void Resume( void );
		BOOL IsPaused( void ) const;
		BOOL IsNotPaused( void ) const;

		void SetTimeScale( F32 scale );
		F32 GetTimeScale( void ) const;

		void StepForward( void );
		void StepBackward( void );

		// NOTE: Conversion utilities 
		static F32	CyclesToSeconds( const U64 timeCycles );
		static F64	CyclesToSecondsF64( const U64 timeCycles );
		static U64	SecondsToCycles( const F32 timeSeconds );

	private:

		// NOTE: Init and shutdown only called once by Engine.
		friend class Engine;
		friend class System;
		static void Initialize( void );
		static void Shutdown( void );

		U64		m_timeCycles;
		U64		m_lastTimeCycles;
		U64		m_deltaTimeCycles;
		F32		m_timeScale;
		BOOL	m_bPaused;

		static U64 m_cyclesPerSecond;
		static F64 m_secondsPerCycle;
	};

	PSX_FORCE_INLINE F32 Clock::CyclesToSeconds( const U64 timeCycles ) 
	{
		return (F32)((F64)timeCycles * m_secondsPerCycle); 
	}

	PSX_FORCE_INLINE F64 Clock::CyclesToSecondsF64( const U64 timeCycles ) 
	{
		return (F64)timeCycles * m_secondsPerCycle; 
	}

	PSX_FORCE_INLINE U64 Clock::SecondsToCycles( const F32 timeSeconds ) 
	{
		return (U64)(timeSeconds * m_cyclesPerSecond); 
	}

	PSX_INLINE Clock::Clock( F32 startTimeinSeconds /*= 0.0f*/ )
	{
		Reset( startTimeinSeconds );
	}

	PSX_INLINE void Clock::Initialize( void )
	{
		m_cyclesPerSecond = System::GetPerformanceFrequency();
		m_secondsPerCycle = 1.0 / (F64)m_cyclesPerSecond;
	}

	PSX_INLINE void Clock::Shutdown( void )
	{
		// nada
	}

	PSX_INLINE void Clock::Reset( F32 startTimeinSeconds /*= 0.0f*/ )
	{
		m_timeCycles		= SecondsToCycles( startTimeinSeconds );
		m_lastTimeCycles	= m_timeCycles;
		m_deltaTimeCycles	= 0;
		m_timeScale			= 1.0f;
		m_bPaused			= FALSE;
	}

	PSX_INLINE void Clock::UpdateSeconds( F32 dtSeconds )
	{
		if ( !m_bPaused )
		{
			m_lastTimeCycles = m_timeCycles;
			m_timeCycles += SecondsToCycles( dtSeconds * m_timeScale );
			m_deltaTimeCycles = m_timeCycles - m_lastTimeCycles;
		}
	}

	PSX_INLINE void Clock::UpdateCycles( U64 dtCycles )
	{
		if ( !m_bPaused )
		{
			m_lastTimeCycles = m_timeCycles;
			m_timeCycles += SecondsToCycles( CyclesToSeconds( dtCycles ) * m_timeScale );
			m_deltaTimeCycles = m_timeCycles - m_lastTimeCycles;
		}
	}

	PSX_INLINE U64 Clock::GetTimeCycles( void ) const
	{
		return m_timeCycles;
	}

	PSX_INLINE F64 Clock::GetTimeSeconds( void ) const
	{
		return CyclesToSecondsF64( m_timeCycles );
	}

	PSX_INLINE U32 Clock::GetDeltaCycles( void ) const
	{
		return (U32)m_deltaTimeCycles;
	}
	
	PSX_INLINE F32 Clock::GetDeltaSeconds( void ) const
	{
		return CyclesToSeconds( m_deltaTimeCycles );
	}

	PSX_INLINE F32 Clock::CalculateDeltaSeconds( Clock *pClock )
	{
		return CyclesToSeconds( pClock->m_timeCycles - m_timeCycles );
	}

	PSX_INLINE void Clock::SetPaused( BOOL bPaused )
	{
		m_bPaused = bPaused;
	}

	PSX_INLINE void Clock::Pause( void )
	{
		m_bPaused = TRUE;
	}

	PSX_INLINE void Clock::Resume( void )
	{
		m_bPaused = FALSE;
	}

	PSX_INLINE BOOL Clock::IsPaused( void ) const
	{
		return m_bPaused == TRUE;
	}

	PSX_INLINE BOOL Clock::IsNotPaused( void ) const
	{
		return m_bPaused == FALSE;
	}

	PSX_INLINE void Clock::SetTimeScale( F32 scale )
	{
		m_timeScale = scale;
	}

	PSX_INLINE F32 Clock::GetTimeScale( void ) const
	{
		return m_timeScale;
	}

	PSX_INLINE void Clock::StepForward( void )
	{
		//1/30th of a second
		if ( !m_bPaused )
			UpdateSeconds( 1.0f / 30.0f );
	}

	PSX_INLINE void Clock::StepBackward( void )
	{
		//1/30th of a second
		if ( !m_bPaused )
			UpdateSeconds( -1.0f / 30.0f );
	}

}

#endif /* _PSX_CLOCK_H_ */