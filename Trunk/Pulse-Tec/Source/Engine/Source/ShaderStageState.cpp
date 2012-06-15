/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ShaderStageState.cpp
*
*	Comments	-	See ShaderStageState.h
*
**************************************************************************************/
#include "../Include/ShaderStageState.h"

namespace Pulse
{
	void ShaderStageState::ClearStates( void )
	{
		m_pShader = PSX_NULL;
		PSX_ZeroMem( m_pConstantBuffers, sizeof(ConstantBuffer*) * PSX_NUM_CONSTANT_BUFFER_SLOT );
		PSX_ZeroMem( m_pSamplerStates, sizeof(SamplerState*) * PSX_NUM_SAMPLER_SLOT );
		PSX_ZeroMem( m_pShaderResources, sizeof(GraphicsResource*) * PSX_NUM_INPUT_RESOURCE_SLOT );

		PSX_ZeroMem( m_pD3DConstantBuffers, sizeof(ID3D11Buffer*) * PSX_NUM_CONSTANT_BUFFER_SLOT );
		PSX_ZeroMem( m_pD3DSamplerStates, sizeof(ID3D11SamplerState*) * PSX_NUM_SAMPLER_SLOT );
		PSX_ZeroMem( m_pD3DShaderResources, sizeof(ID3D11ShaderResourceView*) * PSX_NUM_INPUT_RESOURCE_SLOT );
	}

}