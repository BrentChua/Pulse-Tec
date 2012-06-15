/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	OutputMergerStage.cpp
*
*	Comments	-	See OutputMergerStage.h
*
**************************************************************************************/
#include "../Include/OutputMergerStage.h"
#include "../Include/Renderer.h"
#include "../Include/BlendState.h"
#include "../Include/GraphicsResource.h"
#include "../Include/DepthStencilState.h"
#include "../Include/DX11Mapping.h"

namespace Pulse
{
	void OutputMergerState::ClearState( void )
	{
		m_pBlendState = PSX_NULL;
		m_blendFactors[0] = 1.0f;
		m_blendFactors[1] = 1.0f;
		m_blendFactors[2] = 1.0f;
		m_blendFactors[3] = 1.0f;
		m_sampleMask = 0xffffffff;
		m_pDepthStencilState = PSX_NULL;
		m_stencilRef = 1;
		PSX_ZeroMem( m_pRenderTargets, sizeof(m_pRenderTargets) );
		m_pDepthStencil = PSX_NULL;
		PSX_ZeroMem( m_pD3DRenderTargets, sizeof(m_pD3DRenderTargets) );
	}

	OutputMergerStage::OutputMergerStage( void )
	{
		ClearStates();
	}

	OutputMergerStage::~OutputMergerStage( void )
	{

	}

	void OutputMergerStage::SetBlendState( BlendState *pBlendState )
	{
		
		if ( m_currentState.m_pBlendState != pBlendState )
		{
			m_desiredState.m_pBlendState = pBlendState;
			m_bBindBlendState = TRUE;
		}
		else 
			m_bBindBlendState = FALSE;

	}

	void OutputMergerStage::SetBlendFactors( FLOAT r, FLOAT g, FLOAT b, FLOAT a )
	{
		if ( m_currentState.m_blendFactors[0] != r || m_currentState.m_blendFactors[1] != g ||
			m_currentState.m_blendFactors[2] != b || m_currentState.m_blendFactors[3] != a )
		{
			m_desiredState.m_blendFactors[0] = r;
			m_desiredState.m_blendFactors[1] = g;
			m_desiredState.m_blendFactors[2] = b;
			m_desiredState.m_blendFactors[3] = a;

			m_bBindBlendFactors = TRUE;
		}
		else
			m_bBindBlendFactors = FALSE;

	}

	void OutputMergerStage::SetBlendFactors( const FLOAT *pBlendFactors )
	{
		if ( m_currentState.m_blendFactors[0] != pBlendFactors[0] || 
			m_currentState.m_blendFactors[1] != pBlendFactors[1] ||
			m_currentState.m_blendFactors[2] != pBlendFactors[2] || 
			m_currentState.m_blendFactors[3] != pBlendFactors[3] )
		{

			PSX_MemCopyPerByte( m_desiredState.m_blendFactors, pBlendFactors, sizeof(FLOAT) * 4 );
			m_bBindBlendFactors = TRUE;
		}
		else
			m_bBindBlendFactors = FALSE;

	}

	void OutputMergerStage::SetSampleMask( const UINT sampleMask )
	{
		if ( m_currentState.m_sampleMask != sampleMask )
		{
			m_desiredState.m_sampleMask = sampleMask;
			m_bBindSampleMask = TRUE;
		}
		else
			m_bBindSampleMask = FALSE;
	}

	void OutputMergerStage::SetDepthStencilState( DepthStencilState *pDepthStencilState )
	{
		if ( m_currentState.m_pDepthStencilState != pDepthStencilState )
		{
			m_desiredState.m_pDepthStencilState = pDepthStencilState;
			m_bBindDepthStencilState = TRUE;
		}
		else
			m_bBindDepthStencilState = FALSE;
	}

	void OutputMergerStage::SetStencilRef( UINT ref )
	{
		if ( m_currentState.m_stencilRef != ref )
		{
			m_desiredState.m_stencilRef = ref;
			m_bBindStencilRef = TRUE;
		}
		else
			m_bBindStencilRef = FALSE;
	}

	void OutputMergerStage::SetRenderTarget( UINT slot, GraphicsResource *pRT )
	{
		if ( m_currentState.m_pRenderTargets[slot] != pRT )
		{
			m_desiredState.m_pRenderTargets[slot] = pRT;
			m_desiredState.m_pD3DRenderTargets[slot] = (ID3D11RenderTargetView*)pRT->GetNakedRenderTargetResource();
			m_bBindRenderTargets = TRUE;
		}
	}
	
	void OutputMergerStage::SetDepthStencil( GraphicsResource *pRT )
	{
		if ( m_currentState.m_pDepthStencil != pRT )
		{
			m_desiredState.m_pDepthStencil = pRT;
			m_bBindDepthStencil = TRUE;
		}
		else
			m_bBindDepthStencil = FALSE;
	}

	void OutputMergerStage::BindStates( void )
	{
		BindBlendState();
		BindDepthStencilState();
		BindRenderTargets();
	}

	void OutputMergerStage::ClearStates( void )
	{
		m_desiredState.ClearState();
		m_currentState.ClearState();
		m_bBindBlendState			= FALSE;
		m_bBindBlendFactors			= FALSE;
		m_bBindSampleMask			= FALSE;
		m_bBindDepthStencilState	= FALSE;
		m_bBindRenderTargets		= FALSE;
	}

	void OutputMergerStage::BindBlendState( void )
	{
		if ( m_bBindBlendState || m_bBindBlendFactors || m_bBindSampleMask )
		{
			ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
			
			pDC->OMSetBlendState( (ID3D11BlendState*)m_desiredState.m_pBlendState->GetNakedBlendState(), m_desiredState.m_blendFactors, m_desiredState.m_sampleMask );

			m_currentState.m_pBlendState = m_desiredState.m_pBlendState;
			PSX_MemCopyPerByte( m_currentState.m_blendFactors, m_desiredState.m_blendFactors, sizeof(m_currentState.m_blendFactors) );
			m_currentState.m_sampleMask = m_desiredState.m_sampleMask;

			m_bBindBlendState	= FALSE;
			m_bBindBlendFactors = FALSE;
			m_bBindSampleMask	= FALSE;
		}
	}

	void OutputMergerStage::BindDepthStencilState( void )
	{
		if ( m_bBindDepthStencilState || m_bBindStencilRef )
		{
			ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
			pDC->OMSetDepthStencilState( (ID3D11DepthStencilState*)m_desiredState.m_pDepthStencilState->GetNakedDepthStencilState(), m_desiredState.m_stencilRef );

			m_currentState.m_pDepthStencilState = m_desiredState.m_pDepthStencilState;
			m_currentState.m_stencilRef = m_desiredState.m_stencilRef;

			m_bBindDepthStencilState = FALSE;
			m_bBindStencilRef = FALSE;
		}
	}

	void OutputMergerStage::BindRenderTargets( void )
	{
		if ( m_bBindRenderTargets )
		{
			ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
			pDC->OMSetRenderTargets( PSX_NUM_RENDER_TARGET_SLOT, m_desiredState.m_pD3DRenderTargets, 
				(ID3D11DepthStencilView*)m_desiredState.m_pDepthStencil->GetNakedDepthStencilResource() );
		
			PSX_MemCopyPerByte( m_currentState.m_pRenderTargets, m_desiredState.m_pRenderTargets, sizeof(m_currentState.m_pRenderTargets) );
			m_currentState.m_pDepthStencil = m_desiredState.m_pDepthStencil;
			PSX_MemCopyPerByte( m_currentState.m_pD3DRenderTargets, m_desiredState.m_pD3DRenderTargets, sizeof(m_currentState.m_pD3DRenderTargets) );

			m_bBindRenderTargets = FALSE;
		}
	}

}