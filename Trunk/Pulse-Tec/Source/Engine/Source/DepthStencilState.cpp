/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	DepthStencilState.cpp
*
*	Comments	-	See DepthStencilState.h
*
**************************************************************************************/
#include "../Include/DepthStencilState.h"
#include "../Include/GraphicsDevice.h"

namespace Pulse
{
	DepthStencilStateDesc::DepthStencilStateDesc( void )
	{
		SetDefault();
	}

	void DepthStencilStateDesc::SetDefault( void )
	{
		bDepthEnable		= TRUE;
		depthWriteMask		= EBlendWriteMask::ALL;
		depthFunc			= EComparisonFunc::LESS;
		bStencilEnable		= FALSE;
		stencilReadMask		= EStencilReadMask::DEFAULT;
		stencilWriteMask	= EStencilWriteMask::DEFAULT;
		
		frontFace.failOp		= EStencilOp::KEEP;
		frontFace.depthFailOp	= EStencilOp::KEEP;
		frontFace.passOp		= EStencilOp::KEEP;
		frontFace.func			= EComparisonFunc::ALWAYS;

		backFace.failOp			= EStencilOp::KEEP;
		backFace.depthFailOp	= EStencilOp::KEEP;
		backFace.passOp			= EStencilOp::KEEP;
		backFace.func			= EComparisonFunc::ALWAYS;
	}

	DepthStencilState::DepthStencilState( void )
		: m_pDepthStencilState( PSX_NULL )
	{
	
	}

	DepthStencilState::~DepthStencilState( void )
	{

	}

	BOOL DepthStencilState::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::BLEND_STATE )
			return FALSE;

		m_desc = *((DepthStencilStateDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL DepthStencilState::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL DepthStencilState::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL DepthStencilState::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type DepthStencilState::Initialize( void )
	{
		ID3D11Device *pDevice = (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice();

		D3D11_DEPTH_STENCIL_DESC desc;

		desc.DepthEnable		= m_desc.bDepthEnable;
		desc.DepthWriteMask		= DX11Mapping::ToDXDepthWriteMask( m_desc.depthWriteMask );
		desc.DepthFunc			= DX11Mapping::ToDXComparisonFunc( m_desc.depthFunc );
		desc.StencilEnable		= m_desc.bStencilEnable;
		desc.StencilReadMask	= DX11Mapping::ToDXStencilReadMask( m_desc.stencilReadMask );
		desc.StencilWriteMask	= DX11Mapping::ToDXStencilWriteMask( m_desc.stencilWriteMask );
		
		desc.FrontFace.StencilFailOp		= DX11Mapping::ToDXStencilOp( m_desc.frontFace.failOp );
		desc.FrontFace.StencilDepthFailOp	= DX11Mapping::ToDXStencilOp( m_desc.frontFace.depthFailOp );
		desc.FrontFace.StencilPassOp		= DX11Mapping::ToDXStencilOp( m_desc.frontFace.passOp );
		desc.FrontFace.StencilFunc			= DX11Mapping::ToDXComparisonFunc( m_desc.frontFace.func );

		desc.BackFace.StencilFailOp			= DX11Mapping::ToDXStencilOp( m_desc.backFace.failOp );
		desc.BackFace.StencilDepthFailOp	= DX11Mapping::ToDXStencilOp( m_desc.backFace.depthFailOp );
		desc.BackFace.StencilPassOp			= DX11Mapping::ToDXStencilOp( m_desc.backFace.passOp );
		desc.BackFace.StencilFunc			= DX11Mapping::ToDXComparisonFunc( m_desc.backFace.func );

		if ( pDevice->CreateDepthStencilState( &desc, &m_pDepthStencilState ) != S_OK )
		{
			PSX_PushError( "Failed to create depth stencil state." );
			return EErrorCode::GRAPHICS;
		}

		return EErrorCode::NONE;
	}

	void DepthStencilState::Cleanup( void )
	{
		PSX_SafeRelease( m_pDepthStencilState );
	}

}