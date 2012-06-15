/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	BlendState.cpp
*
*	Comments	-	See BlendState.h
*
**************************************************************************************/
#include "../Include/SamplerState.h"
#include "../Include/GraphicsDevice.h"

namespace Pulse
{
	SamplerStateDesc::SamplerStateDesc( void )
	{
		SetDefault();
	}

	void SamplerStateDesc::SetDefault( void )
	{
		filter = EFilter::MIN_MAG_LINEAR_MIP_POINT;
		addressU = ETextureAddressingMode::CLAMP;
		addressV = ETextureAddressingMode::CLAMP;
		addressW = ETextureAddressingMode::CLAMP;
		mipLODBias = 0.0f;
		maxAnisotropy = 16;
		comparisonFunc = EComparisonFunc::NEVER;
		borderColor[0] = 0.0f;
		borderColor[1] = 0.0f;
		borderColor[2] = 0.0f;
		borderColor[3] = 0.0f;
		minLOD = -FLT_MAX;
		maxLOD = FLT_MAX;
	}

	SamplerState::SamplerState( void )
		: m_pSamplerState( PSX_NULL )
	{
	}

	SamplerState::~SamplerState( void )
	{

	}


	BOOL SamplerState::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::SAMPLER_STATE )
			return FALSE;

		m_desc = *((SamplerStateDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL SamplerState::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL SamplerState::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL SamplerState::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type SamplerState::Initialize( void )
	{
		ID3D11Device *pDevice = (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice();

		D3D11_SAMPLER_DESC desc;

		desc.Filter			= DX11Mapping::ToDXFilter( m_desc.filter );
		desc.AddressU		= DX11Mapping::ToDXTextureAddressMode( m_desc.addressU );
		desc.AddressV		= DX11Mapping::ToDXTextureAddressMode( m_desc.addressV );
		desc.AddressW		= DX11Mapping::ToDXTextureAddressMode( m_desc.addressW );
		desc.MipLODBias		= m_desc.mipLODBias;
		desc.MaxAnisotropy	= m_desc.maxAnisotropy;
		desc.ComparisonFunc = DX11Mapping::ToDXComparisonFunc( m_desc.comparisonFunc );
		PSX_MemCopyPerByte( desc.BorderColor, m_desc.borderColor, sizeof(m_desc.borderColor) );
		desc.MinLOD			= m_desc.minLOD;
		desc.MaxLOD			= m_desc.maxLOD;

		if ( pDevice->CreateSamplerState( &desc, &m_pSamplerState ) != S_OK )
		{
			PSX_PushError( "Failed to create sampler state." );
			return EErrorCode::GRAPHICS;
		}

		return EErrorCode::OKAY;
	}

	void SamplerState::Cleanup( void )
	{
		PSX_SafeRelease( m_pSamplerState );
	}

}