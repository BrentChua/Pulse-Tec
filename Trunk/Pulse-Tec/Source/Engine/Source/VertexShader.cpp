/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	VertexShader.cpp
*
*	Comments	-	See VertexShader.h
*
**************************************************************************************/
#include "../Include/VertexShader.h"
#include "../Include/Engine.h"

namespace Pulse
{
	VertexShader::VertexShader( void )
	{
	}

	VertexShader::~VertexShader( void )
	{
	}

	EErrorCode::Type VertexShader::OnInitialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();
		HRESULT hr = 0;

		hr = pDevice->CreateVertexShader( m_pCompiledShader->GetBufferPointer(), 
			m_pCompiledShader->GetBufferSize(), PSX_NULL, &m_pVertexShader );

		return FAILED(hr) ? EErrorCode::GRAPHICS : EErrorCode::OKAY;
	}

	void VertexShader::OnCleanup( void )
	{
		PSX_SafeRelease( m_pVertexShader );
	}
}