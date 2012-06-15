/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	VertexShaderStage.cpp
*
*	Comments	-	See VertexShaderStage.h
*
**************************************************************************************/
#include "../Include/PixelShaderStage.h"
#include "../Include/Renderer.h"
#include "../Include/PixelShader.h"

namespace Pulse
{
	PixelShaderStage::PixelShaderStage( void )
		: IShaderStage()
	{

	}

	PixelShaderStage::~PixelShaderStage( void )
	{

	}

	void PixelShaderStage::BindShader( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		ID3D11PixelShader *pShader = (ID3D11PixelShader*)m_stateCache.m_pShader->GetNakedShader();

		if ( m_stateCache.m_pShader->GetType() == EShaderType::PIXEL )
			pDC->PSSetShader( pShader, 0, 0 );
		else
			PSX_PushError( "Invalid Shader type to bind. Must specify a pixel shader type." );
	}
	
	void PixelShaderStage::BindConstantBuffers( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();

		pDC->PSSetConstantBuffers( 0, PSX_NUM_CONSTANT_BUFFER_SLOT, m_stateCache.m_pD3DConstantBuffers );
	}
	
	void PixelShaderStage::BindSamplerStates( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();

		pDC->PSSetSamplers( 0, PSX_NUM_SAMPLER_SLOT, m_stateCache.m_pD3DSamplerStates );
	}
	
	void PixelShaderStage::BindShaderResourceViews( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		pDC->PSSetShaderResources( 0, PSX_NUM_INPUT_RESOURCE_SLOT, m_stateCache.m_pD3DShaderResources );
	}
}