#include "ConsoleDemo.h"

#ifdef PSX_PLATFORM_WINDOWS_CONSOLE

#include <iostream>
using namespace std;

namespace Pulse
{
	PSX_REGISTER_APPLICATION_CREATE( ConsoleDemo )
	PSX_REGISTER_APPLICATION_RELEASE( )

	AI::AI( void )
	: x( 0 ), y( 0 ), xDir( -1 ), yDir( -1 )
	{

	}

	void AI::Update( void )
	{
		if ( x >= 79 || x < 1 )
			xDir = -xDir;

		if ( y >= 24 || y < 1 )
			yDir = -yDir;

		x += xDir;
		y += yDir;
	}

	// This is a test code sampling 
	class Test
	{
	public:

		Test( void ) 
		{ 
			//cout << "Test created." << endl; 
		}
	};

	PSX_REGISTRY_OBJECT_START( Test, ERegistryPriority::Normal, ERegistryPriority::Normal )
		PSX_REGISTRY_ON_TEST()
		{
			//cout << "This is a test." << endl;
			return EErrorCode::NONE;
		}
	PSX_REGISTRY_OBJECT_END();


	INT ConsoleDemo::Main( INT argNum, CHAR **ppArgs )
	{

		// Init
		m_inputDevice.SetListener( this );
		m_player.x = m_player.y = 0;
		AI ai;

		//cout << "Hello world!" << endl;

		//m_screenBuffer.Set( 0, 0, '8', EConsoleAttribute::BG_BLUE);
		//m_screenBuffer.Set( 1, 0, '8', EConsoleAttribute::BG_GREEN);
		//m_screenBuffer.Set( 2, 0, '8', EConsoleAttribute::BG_RED);
		//m_screenBuffer.Set( 3, 0, '8', EConsoleAttribute::BG_RED);

		
		while ( true )
		{
			Update();

			ai.Update();

			// Render Loop
			m_screenBuffer.ClearBuffer();
			for ( int i = 0; i < 24; ++i )
			{
				for ( int j = 0; j < 79; ++j ) // width
				{
					//char buffer[10];
					//itoa( x, buffer, 10 ); 
					//m_screenBuffer.SetText( j, i, *buffer);
					//m_screenBuffer.SetText( j, i, (char)(32 * x + y));
					//m_screenBuffer.SetAttribute( j, i, 80 * Math::Sinf( x + y) * 100);
					//m_screenBuffer.SetAttribute( j, i, 80 * i + j);
					m_screenBuffer.SetAttribute( j, i, x * y);
				}
			}

			// Draw player
			m_screenBuffer.SetText( m_player.x, m_player.y, 'X' );
			m_screenBuffer.SetText( ai.x, ai.y, 'Y' );

			// Sample string
			m_screenBuffer.DrawString( 5,0, "Hellow WOrld!" );

			m_screenBuffer.DisplayBuffer();
				Sleep( 50 );
		}

		return 0;
	}

	void ConsoleDemo::OnMouseEvent( MouseEvent *pEvent )
	{
		x = pEvent->x;
		y = pEvent->y;
		//pEvent->buttonState = EMouseButtonState::;
		char buffer[10];
		itoa( pEvent->x, buffer, 10 ); 
		//m_screenBuffer.Set( 10, 0, *buffer, EConsoleAttribute::BG_BLUE );
	}

	void ConsoleDemo::OnKeyboardEvent( KeyboardEvent *pEvent )
	{
		if ( pEvent->key == VK_UP )
			m_player.y = m_player.y - 1;

		if ( pEvent->key == VK_DOWN )
			m_player.y = m_player.y + 1;

		if ( pEvent->key == VK_LEFT )
			m_player.x = m_player.x - 1;

		if ( pEvent->key == VK_RIGHT )
			m_player.x = m_player.x + 1;

	}

}

#endif /* PSX_PLATFORM_WINDOWS_CONSOLE */