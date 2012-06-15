#include "../Include/Pulse.h"

#ifdef PSX_PLATFORM_WINDOWS_CONSOLE

namespace Pulse
{
	struct Player
	{
		int x,y;
	};

	struct AI
	{
		AI( void );

		int x, y;
		int xDir, yDir;

		void Update( void );
	};

	class ConsoleDemo : public ConsoleApplication, public ConsoleInputDeviceListener
	{
		virtual INT Main( INT argNum, CHAR **ppArgs );

		virtual void OnMouseEvent( MouseEvent *pEvent );

		virtual void OnKeyboardEvent( KeyboardEvent *pEvent );

		int x, y;
		Player m_player;
	};
}

#endif /* PSX_PLATFORM_WINDOWS_CONSOLE */