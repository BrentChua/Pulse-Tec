/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RenderPass.cpp
*
*	Comments	-	See RenderPass.h
*
**************************************************************************************/
#include "../Include/RenderPass.h"
#include "../Include/InputLayout.h"
#include "../Include/GraphicsResourceManager.h"

namespace Pulse
{
	RenderPass::RenderPass( void )
		: m_pInputLayout( PSX_NULL )
	{
		ClearShaders( FALSE );
	}

	RenderPass::RenderPass( VertexShader *pVS, PixelShader *pPS )
	{
		ClearShaders( FALSE );
	}

	RenderPass::~RenderPass( void )
	{
		PSX_SafeRelease( m_pInputLayout );
		ClearShaders( TRUE );
	}

	void RenderPass::SetInputLayout( InputLayout *pInputLayout )
	{
		PSX_SafeRelease(m_pInputLayout);

		m_pInputLayout = pInputLayout; 
		
		if ( pInputLayout )
			pInputLayout->AddRef();
	}

	void RenderPass::SetVertexShader( VertexShader *pShader )
	{ 
		PSX_SafeRelease(m_pVertexShader);

		m_pVertexShader = pShader;

		if ( pShader )
			pShader->AddRef();
	}

	void RenderPass::SetVertexShader( Shader *pShader )
	{
		if ( pShader->GetType() == EShaderType::VERTEX )
			SetVertexShader( (VertexShader*)pShader );
	}

	void RenderPass::SetHullShader( HullShader *pShader ) 
	{ 
		PSX_SafeRelease(m_pHullShader);

		m_pHullShader = pShader; 

		if ( pShader )
			pShader->AddRef();
	}

	void RenderPass::SetHullShader( Shader *pShader )
	{
		if ( pShader->GetType() == EShaderType::HULL )
			SetVertexShader( (HullShader*)pShader );
	}

	void RenderPass::SetDomainShader( DomainShader *pShader ) 
	{ 
		PSX_SafeRelease(m_pDomainShader);

		m_pDomainShader = pShader; 

		if ( pShader )
			pShader->AddRef();
	}

	void RenderPass::SetDomainShader( Shader *pShader )
	{
		if ( pShader->GetType() == EShaderType::DOMAIN_ )
			SetVertexShader( (DomainShader*)pShader );
	}

	void RenderPass::SetGeometryShader( GeometryShader *pShader ) 
	{ 
		PSX_SafeRelease(m_pGeometryShader);

		m_pGeometryShader = pShader; 

		if ( pShader )
			pShader->AddRef();
	}

	void RenderPass::SetGeometryShader( Shader *pShader )
	{
		if ( pShader->GetType() == EShaderType::GEOMETRY )
			SetVertexShader( (GeometryShader*)pShader );
	}

	void RenderPass::SetPixelShader( PixelShader *pShader ) 
	{ 
		PSX_SafeRelease(m_pPixelShader);

		m_pPixelShader = pShader; 

		if ( pShader )
			pShader->AddRef();
	}

	void RenderPass::SetPixelShader( Shader *pShader )
	{
		if ( pShader->GetType() == EShaderType::PIXEL )
			SetVertexShader( (PixelShader*)pShader );
	}

	void RenderPass::SetComputeShader( ComputeShader *pShader ) 
	{ 
		PSX_SafeRelease(m_pComputeShader);

		m_pComputeShader = pShader; 

		if ( pShader )
			pShader->AddRef();
	}

	void RenderPass::SetComputeShader( Shader *pShader )
	{
		if ( pShader->GetType() == EShaderType::COMPUTE )
			SetComputeShader( (ComputeShader*)pShader );
	}

	void RenderPass::Update( void )
	{
	/*	for ( INT i = 0; i < EShaderType::EnumCount; ++i )
			if ( m_pShaders[i] )
				m_pShaders[i]->up*/
	}

	void RenderPass::ClearShaders( bool bCheckRelease )
	{
		if ( bCheckRelease )
		{
			for ( INT i = 0; i < EShaderType::EnumCount; ++i )
				if ( m_pShaders[i] )
					m_pShaders[i]->Release();
		}

		PSX_ZeroMem( m_pShaders, sizeof(m_pShaders) );
	}

	void RenderPass::GenerateInputLayout( void )
	{
		if ( m_pInputLayout != PSX_NULL )
			return;

		if ( m_pVertexShader == PSX_NULL )
			return;

		//GraphicsResourceManager::GetInstance()->CreateInputL
	}

}
