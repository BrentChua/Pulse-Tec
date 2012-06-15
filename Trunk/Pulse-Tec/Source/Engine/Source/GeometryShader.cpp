/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GeometryShader.cpp
*
*	Comments	-	See GeometryShader.h
*
**************************************************************************************/
#include "../Include/GeometryShader.h"
#include "../Include/Engine.h"

namespace Pulse
{
	GeometryShader::GeometryShader( void )
	{

	}

	GeometryShader::~GeometryShader( void )
	{

	}

	EErrorCode::Type GeometryShader::OnInitialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();
		HRESULT hr = 0;

		hr = pDevice->CreateGeometryShader( m_pCompiledShader->GetBufferPointer(), 
			m_pCompiledShader->GetBufferSize(), PSX_NULL, &m_pGeometryShader );

		return FAILED(hr) ? EErrorCode::GRAPHICS : EErrorCode::OKAY;
	}

	void GeometryShader::OnCleanup( void )
	{
		PSX_SafeRelease( m_pGeometryShader );
	}
}