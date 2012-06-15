/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	HullShader.cpp
*
*	Comments	-	See HullShader.h
*
**************************************************************************************/
#include "../Include/HullShader.h"
#include "../Include/Engine.h"

namespace Pulse
{
	HullShader::HullShader( void )
	{
	}

	HullShader::~HullShader( void )
	{

	}

	EErrorCode::Type HullShader::OnInitialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();
		HRESULT hr = 0;

		hr = pDevice->CreateHullShader( m_pCompiledShader->GetBufferPointer(), 
			m_pCompiledShader->GetBufferSize(), PSX_NULL, &m_pHullShader );

		return FAILED(hr) ? EErrorCode::GRAPHICS : EErrorCode::OKAY;
	}

	void HullShader::OnCleanup( void )
	{
		PSX_SafeRelease( m_pHullShader );
	}
}