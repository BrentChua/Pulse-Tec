/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IApplication.cpp
*
*	Comments	-	See IApplication.h
*
**************************************************************************************/

#include "../Include/IApplication.h"
#include "../Include/Registry.h"

namespace Pulse
{
	IApplication::IApplication() 
	{ 
		InitializeSystems();
	}

	IApplication::~IApplication( void )
	{
		ShutdownSystems();
	}

	BOOL IApplication::InitializeSystems( void )
	{
		EErrorCode::Type errorCode;
		#ifdef PSX_DEBUG
			// Run test before initialization
			errorCode = Registry::ExecuteOperation( ERegistryOperation::TEST_INITIALIZE );
			if ( errorCode != EErrorCode::NONE ) return FALSE;
			errorCode = Registry::ExecuteOperation( ERegistryOperation::TEST );
			if ( errorCode != EErrorCode::NONE ) return FALSE;
			errorCode = Registry::ExecuteOperation( ERegistryOperation::TEST_SHUTDOWN );
			if ( errorCode != EErrorCode::NONE ) return FALSE;
		#endif /* PSX_DEBUG */

		errorCode = Registry::ExecuteOperation( ERegistryOperation::INITIALIZE );
		return errorCode != EErrorCode::NONE;
	}

	void IApplication::ShutdownSystems( void )
	{
		Registry::ExecuteOperation( ERegistryOperation::SHUTDOWN );
	}
}