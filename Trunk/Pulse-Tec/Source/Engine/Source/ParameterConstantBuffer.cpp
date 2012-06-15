/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterConstantBuffer.cpp
*
*	Comments	-	See ParameterConstantBuffer.h
*
**************************************************************************************/
#include "../Include/ParameterConstantBuffer.h"

namespace Pulse
{
	ParameterConstantBuffer::ParameterConstantBuffer( ParameterConstantBuffer &rhs )
		: IParameter( rhs ), m_pConstantBuffer( rhs.m_pConstantBuffer )
	{
		m_pConstantBuffer->AddRef();
	}

	ParameterConstantBuffer::~ParameterConstantBuffer( void )
	{
		PSX_SafeRelease( m_pConstantBuffer );
	}

	void ParameterConstantBuffer::SetData( void *pData )
	{
		PSX_SafeRelease( m_pConstantBuffer );
		m_pConstantBuffer = (ConstantBuffer*)pData;
		m_pConstantBuffer->AddRef();
	}

	void ParameterConstantBuffer::SetData( IParameter *pParam )
	{
		if ( pParam )
		{
			if ( pParam->GetType() == EParameter::CONSTANT_BUFFER )
			{
				PSX_SafeRelease( m_pConstantBuffer );
				m_pConstantBuffer = ((ParameterConstantBuffer*)pParam)->m_pConstantBuffer;
				m_pConstantBuffer->AddRef();
			}
		}
	}

}