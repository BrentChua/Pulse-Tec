/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IPipelineStage.cpp
*
*	Comments	-	See IPipelineStage.h
*
**************************************************************************************/
#include "../Include/IPipelineStage.h"
#include "../Include/Renderer.h"

namespace Pulse
{
	IPipelineStage::IPipelineStage( void ) 
		: m_pRenderer(PSX_NULL) 
	{

	}

	IPipelineStage::~IPipelineStage( void )
	{

	}

	void IPipelineStage::SetRenderer( Renderer *pRenderer ) 
	{ 
		m_pRenderer = pRenderer;
	}

}