/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ComputeShader.cpp
*
*	Comments	-	See ComputeShader.h
*
**************************************************************************************/
#include "../Include/ComputeShader.h"
#include "../Include/Engine.h"

namespace Pulse
{
	ComputeShader::ComputeShader( void )
	{
	}

	ComputeShader::~ComputeShader( void )
	{
	}

	EErrorCode::Type ComputeShader::OnInitialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();
		HRESULT hr = 0;

		hr = pDevice->CreateComputeShader( m_pCompiledShader->GetBufferPointer(), 
			m_pCompiledShader->GetBufferSize(), PSX_NULL, &m_pComputeShader );

		return FAILED(hr) ? EErrorCode::GRAPHICS : EErrorCode::OKAY;
	}

	void ComputeShader::OnCleanup( void )
	{
		PSX_SafeRelease( m_pComputeShader );
	}
}