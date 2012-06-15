#pragma once

#include "../Include/Pulse.h"

namespace Pulse
{
	class SampleView : public HumanView
	{
	public:
		SampleView( void );
		~SampleView( void );
		virtual EErrorCode::Type	OnRender( DOUBLE time, FLOAT deltaTime );
		virtual void				OnUpdate( DOUBLE time, FLOAT deltaTime );

	private:

	};
}