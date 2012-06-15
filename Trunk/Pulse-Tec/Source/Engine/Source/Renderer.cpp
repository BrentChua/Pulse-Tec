/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Renderer.cpp
*
*	Comments	-	See Renderer.h
*
**************************************************************************************/
#include "../Include/Renderer.h"
#include "../Include/GraphicsDevice.h"
#include "../Include/WindowApplicationWin32.h"
#include "../Include/SwapChain.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/GraphicsBuffer.h"

namespace Pulse
{

	void RendererDesc::SetAsImmediate( ERendererProcessingType::Type	_processingType, UINT _adapter, UINT _adapterOutput, BOOL _bDebug,
		BOOL _bSoftwareRendererFallback, DisplayMode _displaySettings )
	{
		processingType = _processingType;
		deviceContextType = EDeviceContext::IMMEDIATE;
		adapter = _adapter;
		adapterOutput = _adapterOutput;
		bDebug = _bDebug;
		bSoftwareRendererFallback = _bSoftwareRendererFallback;
		displaySettings = _displaySettings;
	}

	void RendererDesc::SetAsDeferred( void )
	{
		deviceContextType = EDeviceContext::DEFERRED;
	}

	Renderer::Renderer( void )
		: m_pDevice( PSX_NULL ), m_pDeviceContext( PSX_NULL ), m_pSwapChain( PSX_NULL )
	{

	}

	Renderer::~Renderer( void )
	{
	}

	EErrorCode::Type Renderer::Initialize( RendererDesc  *pDesc )
	{
		EErrorCode::Type ret;
		HRESULT hr;
		m_desc = *pDesc;

		// Get graphics device
		m_pDevice = GraphicsDevice::GetInstance();
		
		// We can only have one immediate renderer. The rest are created with
		//	a deferred renderer context
		if ( m_desc.deviceContextType == EDeviceContext::IMMEDIATE )
		{
			if ( m_pDevice->GetNakedDevice() != PSX_NULL )
			{
				 PSX_PushError( "Immediate renderer already created." );
				return EErrorCode::GRAPHICS;
			}

			// Initialize graphics device for the first time.
			if ( (ret = m_pDevice->Initialize( &m_desc )) != EErrorCode::OKAY )
			{
				PSX_SafeDelete( m_pDevice );
				return ret;
			}

			SwapChainDesc swapChainDesc( m_desc.displaySettings.width, m_desc.displaySettings.height, m_desc.displaySettings.refreshRate,
				m_desc.displaySettings.format, SampleDesc(), m_desc.displaySettings.bWindowed );
			GraphicsResourceManager::GetInstance()->CreateSwapChain( PSX_String("SwapChain"), &swapChainDesc );
		}
		else
		{	// Create deferred device renderer context
			
			ID3D11Device *pDevice = (ID3D11Device*)m_pDevice->GetNakedDevice();
			
			// Check if we already have an immediate context made
			if ( pDevice == PSX_NULL )
			{
				PSX_PushError( "Creating a deferred renderer withouth an Immediate render context. Create immediate render context first." );
				return EErrorCode::GRAPHICS;
			}

			HRESULT result = pDevice->CreateDeferredContext( 0, &m_pDeviceContext );

			switch( result )
			{
			case DXGI_ERROR_DEVICE_REMOVED:
				PSX_PushError( " if the video card has been physically removed from the system, or a driver upgrade for the video card has occurred. If this error occurs, you should destroy and recreate the device." );
				break;
			case DXGI_ERROR_INVALID_CALL:
				PSX_PushError( " the CreateDeferredContext method cannot be called from the current context. For example, if the device was created with the D3D11_CREATE_DEVICE_SINGLETHREADED value, CreateDeferredContext returns DXGI_ERROR_INVALID_CALL." );
				break;
			case E_INVALIDARG:
				PSX_PushError( "the ContextFlags parameter is invalid." );
				break;
			case E_OUTOFMEMORY:
				PSX_PushError( "the application has exhausted available memory." );
				break;
			}

		}

		return EErrorCode::OKAY;
	}

	void Renderer::Cleanup( void )
	{
		PSX_SafeRelease( m_pSwapChain );
		PSX_SafeDelete( m_pDevice );
	}

	MappedResource Renderer::MapResource( GraphicsBuffer *pBuffer, UINT subresourceIndex, EResourceMapping::Type map, UINT flags )
	{
		D3D11_MAPPED_SUBRESOURCE data;
		MappedResource ret;

		m_pDeviceContext->Map( pBuffer->m_pBuffer, subresourceIndex, DX11Mapping::ToDXMapping( map ), 0, &data );

		ret.pData = data.pData;
		ret.RowPitch = data.RowPitch;
		ret.DepthPitch = data.DepthPitch;

		return ret;
	}

	void Renderer::UnmapResource( GraphicsBuffer *pBuffer, UINT subresourceIndex )
	{
		m_pDeviceContext->Unmap( pBuffer->m_pBuffer, subresourceIndex );
	}

}