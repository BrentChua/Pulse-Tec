/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	FPSCounter.h
*
*	Description -	Frames-Per-Second counter.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/26/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_FPS_COUNTER_H_
#define _PSX_FPS_COUNTER_H_

#include "PulseSTD.h"
#include "Clock.h"
#include "NonCopyable.h"

namespace Pulse
{
	class FPSCounter : NonCopyable
	{
	public:

		// NOTE: Set the mean time (time between every update).
		FPSCounter( FLOAT meanTime = 0.5f );

		// NOTE: Update to next frame, compute the fps if mean time has been elapsed.
		void Update( FLOAT dt );
		void Update( I64 cycles );
		void Update( U64 cycles );
		void Update( I32 cycles );
		void Update( U32 cycles );

		// NOTE: Get the last computed fps.
		UINT GetFPS( void ) const;
		
	private:

		FLOAT	m_totalTime;
		FLOAT	m_meanTime;
		UINT	m_totalFrames;
		UINT	m_lastFPS;

	};

	PSX_INLINE UINT FPSCounter::GetFPS( void ) const
	{
		return m_lastFPS;
	}

	class FPSAutoCounter : private FPSCounter
	{
	public:

		FPSAutoCounter( FLOAT meanTime = 1.0f );

		void Update( void );

		UINT GetFPS( void ) const;

	private:

		U64 m_lastTimeCycle;

	};
}

#endif /* _PSX_FPS_COUNTER_H_ */