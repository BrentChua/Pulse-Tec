/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RasterizerState.cpp
*
*	Comments	-	See RasterizerState.h
*
**************************************************************************************/
#include "../Include/RasterizerState.h"
#include "../Include/GraphicsDevice.h"

namespace Pulse
{
	RasterizerStateDesc::RasterizerStateDesc( void )
	{
		SetDefault();
	}

	void RasterizerStateDesc::SetDefault( void )
	{
		fillMode				= EFillMode::SOLID;
		cullMode				= ECullMode::BACK;
		frontCounterClockwise	= FALSE;
		depthBias				= 0;
		depthBiasClamp			= 0.0f;
		slopeScaledDepthBias	= 0.0f;
		depthClipEnable			= TRUE;
		scissorEnable			= FALSE;
		multisampleEnable		= FALSE;
		antialiasedLineEnable	= FALSE;
	}

	RasterizerState::RasterizerState( void )
		: m_pRasterizerState(PSX_NULL)
	{
	}

	RasterizerState::~RasterizerState( void )
	{

	}


	BOOL RasterizerState::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::RASTERIZER_STATE )
			return FALSE;

		m_desc = *((RasterizerStateDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL RasterizerState::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL RasterizerState::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL RasterizerState::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type RasterizerState::Initialize( void )
	{
		ID3D11Device *pDevice = (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice();

		D3D11_RASTERIZER_DESC desc;

		desc.FillMode				= DX11Mapping::ToDXFillMode( m_desc.fillMode );
		desc.CullMode				= DX11Mapping::ToDXCullMode( m_desc.cullMode );
		desc.FrontCounterClockwise	= m_desc.frontCounterClockwise;
		desc.DepthBias				= m_desc.depthBias;
		desc.DepthBiasClamp			= m_desc.depthBiasClamp;
		desc.SlopeScaledDepthBias	= m_desc.slopeScaledDepthBias;
		desc.DepthClipEnable		= m_desc.depthClipEnable;
		desc.ScissorEnable			= m_desc.scissorEnable;
		desc.MultisampleEnable		= m_desc.multisampleEnable;
		desc.AntialiasedLineEnable	= m_desc.antialiasedLineEnable;

		if ( pDevice->CreateRasterizerState( &desc, &m_pRasterizerState ) != S_OK )
		{
			PSX_PushError( "Failed to create rasterizer state." );
			return EErrorCode::GRAPHICS;
		}

		return EErrorCode::NONE;
	}

	void RasterizerState::Cleanup( void )
	{
		PSX_SafeRelease( m_pRasterizerState );
	}
}