/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	DomainShader.cpp
*
*	Comments	-	See DomainShader.h
*
**************************************************************************************/
#include "../Include/DomainShader.h"
#include "../Include/Engine.h"

namespace Pulse
{
	DomainShader::DomainShader( void )
	{
	}

	DomainShader::~DomainShader( void )
	{
	}

	EErrorCode::Type DomainShader::OnInitialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();
		HRESULT hr = 0;

		hr = pDevice->CreateDomainShader( m_pCompiledShader->GetBufferPointer(), 
			m_pCompiledShader->GetBufferSize(), PSX_NULL, &m_pDomainShader );

		return FAILED(hr) ? EErrorCode::GRAPHICS : EErrorCode::OKAY;
	}

	void DomainShader::OnCleanup( void )
	{
		PSX_SafeRelease( m_pDomainShader );
	}
}