/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsBuffer.cpp
*
*	Comments	-	See GraphicsBuffer.h
*
**************************************************************************************/
#include "../Include/GraphicsBuffer.h"
#include "../Include/GraphicsDevice.h"

namespace Pulse
{
	GraphicsBuffer::GraphicsBuffer( void )
		: m_pBuffer( PSX_NULL )
	{

	}

	GraphicsBuffer::~GraphicsBuffer( void )
	{

	}

	void GraphicsBuffer::ReleaseBuffer( void )
	{
		PSX_SafeRelease( m_pBuffer );
	}

}