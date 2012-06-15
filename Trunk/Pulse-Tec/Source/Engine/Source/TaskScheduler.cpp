/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	TaskScheduler.cpp
*
*	Comments	-	See TaskScheduler.h
*
**************************************************************************************/
#include "../Include/TaskScheduler.h"
#include "../Include/System.h"

namespace Pulse
{
	void WorkerThreadMain( void *pWorkerThread )
	{
		((WorkerThread*)pWorkerThread)->ThreadProc();
	}

	TaskScheduler::~TaskScheduler( void )
	{
		Shutdown();
	}

	TaskScheduler::TaskScheduler( void )
	{
		if ( EErrorCode::OKAY != Initialize() )
		{
			PSX_PushError( "Failed to initialize Task scheduler." );
		}
	}

	EErrorCode::Type TaskScheduler::Initialize( void )
	{
		// Get number of threads
		m_numThreads = System::GetNumHardwareThreads();



		return EErrorCode::OKAY;
	}

	void TaskScheduler::Shutdown( void )
	{

	}
}