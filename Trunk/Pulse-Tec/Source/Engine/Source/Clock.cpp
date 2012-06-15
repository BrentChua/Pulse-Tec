/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Clock.cpp
*
*	Comments	-	See Clock.h
*
**************************************************************************************/

#include "../Include/Clock.h"

namespace Pulse
{
	U64 Clock::m_cyclesPerSecond = 0;
	F64 Clock::m_secondsPerCycle = 0.0f;
}