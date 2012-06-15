/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ISystem.h
*
*	Description -	Abstract Interface class for systems/managers containing static
*						Initialize() and Shutdown() functions.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/12/2010	-	Creation of this file
**************************************************************************************/

namespace Pulse
{
	class ISystem
	{
	public:

		virtual BOOL Initialize( void ) = 0;

		virtual void Shutdown( void ) = 0;
		
	};
}