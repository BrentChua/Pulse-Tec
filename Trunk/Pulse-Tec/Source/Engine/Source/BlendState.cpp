/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	BlendState.cpp
*
*	Comments	-	See BlendState.h
*
**************************************************************************************/
#include "../Include/BlendState.h"
#include "../Include/GraphicsDevice.h"

namespace Pulse
{
	BlendStateDesc::BlendStateDesc( void )
	{
		SetDefault();
	}

	void BlendStateDesc::SetDefault( void )
	{
		bAlphaToCoverageEnable = FALSE;
		bIndependentBlendEnable = FALSE;

		for ( INT i = 0; i < 8; ++i )
		{
			renderTargets[i].blendEnable			= FALSE;
			renderTargets[i].srcBlend				= EBlendType::ONE;
			renderTargets[i].destBlend				= EBlendType::ZERO;
			renderTargets[i].blendOp				= EBlendOp::ADD;
			renderTargets[i].srcBlendAlpha			= EBlendType::ONE;
			renderTargets[i].destBlendAlpha			= EBlendType::ZERO;
			renderTargets[i].blendOpAlpha			= EBlendOp::ADD;
			renderTargets[i].renderTargetWriteMask	= EColorWriteEnable::ALL;
		}
	}

	BlendState::BlendState( void )
	{
	}

	BlendState::~BlendState( void )
	{

	}


	BOOL BlendState::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::BLEND_STATE )
			return FALSE;

		m_desc = *((BlendStateDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL BlendState::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL BlendState::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL BlendState::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type BlendState::Initialize( void )
	{
		ID3D11Device *pDevice = (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice();

		// Translate desc to D3D11_BLEND_DESC
		D3D11_BLEND_DESC desc;

		desc.AlphaToCoverageEnable = m_desc.bAlphaToCoverageEnable;
		desc.IndependentBlendEnable = m_desc.bIndependentBlendEnable;

		for ( SIZE_T i = 0; i < 8; ++i )
		{
			desc.RenderTarget[i].BlendEnable			= m_desc.renderTargets[i].blendEnable;
			desc.RenderTarget[i].SrcBlend				= DX11Mapping::ToDXBlend( m_desc.renderTargets[i].srcBlend );
			desc.RenderTarget[i].DestBlend				= DX11Mapping::ToDXBlend( m_desc.renderTargets[i].destBlend );
			desc.RenderTarget[i].BlendOp				= DX11Mapping::ToDXBlendOp( m_desc.renderTargets[i].blendOp );
			desc.RenderTarget[i].SrcBlendAlpha			= DX11Mapping::ToDXBlend( m_desc.renderTargets[i].srcBlendAlpha );
			desc.RenderTarget[i].DestBlendAlpha			= DX11Mapping::ToDXBlend( m_desc.renderTargets[i].destBlendAlpha );
			desc.RenderTarget[i].BlendOpAlpha			= DX11Mapping::ToDXBlendOp( m_desc.renderTargets[i].blendOpAlpha );
			desc.RenderTarget[i].RenderTargetWriteMask	= DX11Mapping::ToDXWriteEnable( m_desc.renderTargets[i].renderTargetWriteMask );
		}

		if ( E_OUTOFMEMORY == pDevice->CreateBlendState( &desc, &m_pBlendState ) )
		{
			PSX_PushError( "Graphics card out of memory." );
			return EErrorCode::GRAPHICS;
		}

		return EErrorCode::NONE;
	}

	void BlendState::Cleanup( void )
	{
		PSX_SafeRelease( m_pBlendState );
	}

}