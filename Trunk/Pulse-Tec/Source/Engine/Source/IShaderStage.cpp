/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IShaderStage.cpp
*
*	Comments	-	See IShaderStage.h
*
**************************************************************************************/
#include "../Include/IShaderStage.h"
#include "../Include/ConstantBuffer.h"
#include "../Include/SamplerState.h"
#include "../Include/GraphicsResource.h"

namespace Pulse
{
	IShaderStage::IShaderStage( void )
		: IPipelineStage( )
	{
		ClearStates();
	}

	IShaderStage::~IShaderStage( void )
	{

	}

	void IShaderStage::SetShader( Shader *pShader )
	{
		m_stateCache.m_pShader = pShader;
	}

	void IShaderStage::SetConstantBuffer( UINT slot, ConstantBuffer *pConstantBuffer )
	{
		m_stateCache.m_pConstantBuffers[slot] = pConstantBuffer;
		m_stateCache.m_pD3DConstantBuffers[slot] = (ID3D11Buffer*)pConstantBuffer->GetNakedBuffer();
	}

	void IShaderStage::SetSamplerState( UINT slot, SamplerState *pSampler )
	{
		m_stateCache.m_pSamplerStates[slot] = pSampler;
		m_stateCache.m_pD3DSamplerStates[slot] = (ID3D11SamplerState*)pSampler->GetNakedSamplerState();
	}

	void IShaderStage::SetShaderResource( UINT slot, GraphicsResource *pGraphicsResource )
	{
		m_stateCache.m_pShaderResources[slot] = pGraphicsResource;
		m_stateCache.m_pD3DShaderResources[slot] = (ID3D11ShaderResourceView*)pGraphicsResource->GetNakedShaderResource();
	}

	//void SetUnordererdAccess();

	void IShaderStage::ClearStates( void )
	{
		m_stateCache.ClearStates();
	}

}