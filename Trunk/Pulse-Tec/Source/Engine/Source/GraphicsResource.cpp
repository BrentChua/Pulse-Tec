/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsResource.cpp
*
*	Comments	-	See GraphicsResource.h
*
**************************************************************************************/
#include "../Include/GraphicsResource.h"
#include "../Include/GraphicsDevice.h"

namespace Pulse
{

	GraphicsResource::GraphicsResource( void )
		: m_pSRV( PSX_NULL ), m_pRTV( PSX_NULL ), 
		m_pDSV( PSX_NULL ), m_pUAV( PSX_NULL )
	{

	}

	GraphicsResource::~GraphicsResource( void )
	{

	}

	void GraphicsResource::CreateResourceViews( ID3D11Resource *pResource, UINT bindFlags )
	{
		ID3D11Device *pDevice = (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice();

		ReleaseResourceViews();

		if ( bindFlags & D3D11_BIND_SHADER_RESOURCE )
			pDevice->CreateShaderResourceView( pResource, PSX_NULL, &m_pSRV );

		if ( bindFlags & D3D11_BIND_RENDER_TARGET )
			pDevice->CreateRenderTargetView( pResource, PSX_NULL, &m_pRTV );

		if ( bindFlags & D3D11_BIND_DEPTH_STENCIL )
			pDevice->CreateDepthStencilView( pResource, PSX_NULL, &m_pDSV );

		if ( bindFlags & D3D11_BIND_UNORDERED_ACCESS )
			pDevice->CreateUnorderedAccessView( pResource, PSX_NULL, &m_pUAV );
	}

	void GraphicsResource::ReleaseResourceViews( void )
	{
		PSX_SafeRelease( m_pSRV );
		PSX_SafeRelease( m_pRTV );
		PSX_SafeRelease( m_pDSV );
		PSX_SafeRelease( m_pUAV );
	}

}