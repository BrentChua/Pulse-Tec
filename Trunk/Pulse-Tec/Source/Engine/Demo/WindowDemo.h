#include "../Include/Pulse.h"
#include "../Include/XMLParser.h"
#include "../Include/View.h"
#include "../Include/Logic.h"

#include "LogicDemo.h"
#include "ViewDemo.h"


namespace Pulse
{
	class WindowDemo : public WindowApplication
	{
	public:

		WindowDemo( void );
		virtual BOOL OnStart( void );
		virtual void OnUpdate( DOUBLE time, FLOAT dt, void *pContext );
		virtual void OnShutDown( void );

	private:

		SampleLogic *m_pLogic;
	};
	
}