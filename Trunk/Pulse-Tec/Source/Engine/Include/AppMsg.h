/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	AppMsg.h
*
*	Description -	Application message struct for carrying application message info. 
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/01/2010	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_APP_MSG_H_
#define _PSX_APP_MSG_H_

#include "PulseSTD.h"

namespace Pulse
{
	PSX_EnumBegin( EAppMessage )
		// Add: Add Application messages here
		UNKNOWN,

		// User Input Messages
		KEYDOWN,
		KEYUP,
		MOUSEMOVE,
		MOUSELBUTTONDOWN,
		MOUSELBUTTONUP,
		MOUSERBUTTONDOWN,
		MOUSERBUTTONUP,

		// GUI Messages
		PARENTMOVE,
		PARENTRESIZE,
	PSX_EnumEnd()

	struct AppMsg
	{
		AppMsg( void ) { PSX_ZeroMem( this, sizeof(AppMsg) ); }

		HANDLE				hWnd;
		EAppMessage::Type	msg;
		union
		{
			struct
			{
				CHAR chMsg1;
				CHAR chMsg2;
			};

			struct
			{
				// BYTE or unsigned CHAR
				BYTE bMsg1;
				BYTE bMsg2;
			};

			struct
			{
				U32	uMsg1;
				U32	uMsg2;
			};
			
			struct
			{
				I32	nMsg1;
				I32	nMsg2;
			};

			struct
			{
				FLOAT fMsg1;
				FLOAT fMsg2;
			};

			struct
			{
				LONG lMsg1;
				LONG lMsg2;
			};
		};
	};
}

#endif /* _PSX_APP_MSG_H_ */