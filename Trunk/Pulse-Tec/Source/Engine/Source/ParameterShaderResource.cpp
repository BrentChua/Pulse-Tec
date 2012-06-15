/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterShaderResource.cpp
*
*	Comments	-	See ParameterShaderResource.h
*
**************************************************************************************/
#include "../Include/ParameterShaderResource.h"

namespace Pulse
{
	ParameterShaderResource::ParameterShaderResource( ParameterShaderResource &rhs )
		: IParameter( rhs )
	{
		m_pResource = rhs.m_pResource;
	}

	void ParameterShaderResource::SetData( void *pData )
	{
		m_pResource = (GraphicsResource*)pData;
	}

	void ParameterShaderResource::SetData( IParameter *pParam )
	{
		if ( pParam )
		{
			if ( pParam->GetType() == GetType() )
			{
				m_pResource = ((ParameterShaderResource*)pParam)->m_pResource;
			}
		}
	}

}