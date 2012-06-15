/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	VertexShaderStage.cpp
*
*	Comments	-	See VertexShaderStage.h
*
**************************************************************************************/
#include "../Include/VertexShaderStage.h"
#include "../Include/Renderer.h"
#include "../Include/VertexShader.h"

namespace Pulse
{
	VertexShaderStage::VertexShaderStage( void )
		: IShaderStage()
	{

	}

	VertexShaderStage::~VertexShaderStage( void )
	{

	}

	void VertexShaderStage::BindShader( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		ID3D11VertexShader *pShader = (ID3D11VertexShader*)m_stateCache.m_pShader->GetNakedShader();

		if ( m_stateCache.m_pShader->GetType() == EShaderType::VERTEX )
			pDC->VSSetShader( pShader, 0, 0 );
		else
			PSX_PushError( "Invalid shader type. Must bspecify a vertex shader type." );
	}
	
	void VertexShaderStage::BindConstantBuffers( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		pDC->VSSetConstantBuffers( 0, PSX_NUM_CONSTANT_BUFFER_SLOT, m_stateCache.m_pD3DConstantBuffers );
	}
	
	void VertexShaderStage::BindSamplerStates( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		pDC->VSSetSamplers( 0,PSX_NUM_SAMPLER_SLOT, m_stateCache.m_pD3DSamplerStates );
	}
	
	void VertexShaderStage::BindShaderResourceViews( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		pDC->VSSetShaderResources( 0, PSX_NUM_INPUT_RESOURCE_SLOT, m_stateCache.m_pD3DShaderResources );
	}
}