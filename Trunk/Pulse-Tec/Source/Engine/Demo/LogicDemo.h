#pragma once

#include "../Include/Pulse.h"
#include "../Include/Logic.h"

namespace Pulse
{
	class SampleLogic : public Logic
	{
	public:
		SampleLogic( void ) { }
		virtual ~SampleLogic( void ) { }
		virtual void OnUpdate( DOUBLE time, FLOAT dt );
	};


}