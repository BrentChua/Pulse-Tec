/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderShaderResource.cpp
*
*	Comments	-	See ParameterBinderShaderResource.h
*
**************************************************************************************/
#include "../Include/ParameterBinderShaderResource.h"
#include "../Include/ParameterShaderResource.h"

namespace Pulse
{
	ParameterBinderShaderResource::ParameterBinderShaderResource( void )
		: m_pData( PSX_NULL )
	{
		
	}

	void ParameterBinderShaderResource::BindValue( void )
	{
		m_pShaderResourceParam->SetResource( m_pData );
	}

}