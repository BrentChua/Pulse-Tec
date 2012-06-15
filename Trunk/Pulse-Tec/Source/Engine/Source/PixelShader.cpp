/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PixelShader.cpp
*
*	Comments	-	See PixelShader.h
*
**************************************************************************************/
#include "../Include/PixelShader.h"
#include "../Include/Engine.h"

namespace Pulse
{
	PixelShader::PixelShader( void )
	{
	}

	PixelShader::~PixelShader( void )
	{
	}

	EErrorCode::Type PixelShader::OnInitialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();
		HRESULT hr = 0;

		hr = pDevice->CreatePixelShader( m_pCompiledShader->GetBufferPointer(), 
			m_pCompiledShader->GetBufferSize(), PSX_NULL, &m_pPixelShader );

		return FAILED(hr) ? EErrorCode::GRAPHICS : EErrorCode::OKAY;
	}

	void PixelShader::OnCleanup( void )
	{
		PSX_SafeRelease( m_pPixelShader );
	}
}