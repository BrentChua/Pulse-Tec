/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IApplication.h
*
*	Description -	Base abstract application layer class. Platform specific applications
*					will need to derive from this class.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/16/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_BASE_APPLICATION_H_
#define _PSX_BASE_APPLICATION_H_

#include "PulseSTD.h"
#include "Engine.h"

namespace Pulse
{
	class IApplication
	{
	public:

		IApplication();
		virtual INT Run( INT argNum, CHAR **ppArguments ) = 0;
		virtual ~IApplication( void );

		virtual const CHAR * GetApplicationDataDirectory( const CHAR *appDirectory ) = 0;

	protected:

		BOOL InitializeSystems( void );

		void ShutdownSystems( void );

	private:

	};
};


#endif /* _PSX_BASE_APPLICATION_H_ */