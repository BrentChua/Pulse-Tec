/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsDevice.cpp
*
*	Comments	-	See GraphicsDevice.h
*
**************************************************************************************/
#include "../Include/GraphicsDevice.h"
#include "../Include/Renderer.h"
#include "../Include/GraphicsResourceManager.h"


namespace Pulse
{
	GraphicsDevice::GraphicsDevice( void )
		: m_pDevice( PSX_NULL ), m_pDeviceContext( PSX_NULL )
	{
	}

	GraphicsDevice::~GraphicsDevice( void )
	{
	}

	EErrorCode::Type GraphicsDevice::Initialize( RendererDesc *pDesc )
	{
		// Only create this for immediate render context
		if ( pDesc->deviceContextType != EDeviceContext::IMMEDIATE )
			return EErrorCode::GRAPHICS;

		GraphicsInfrastructure *pGI = GraphicsInfrastructure::GetInstance();
		DXGIFactoryPtr pFactory = pGI->GetFactory();
		IDXGIAdapter1 *pAdapter;
		UINT createFlags = 0;
		HRESULT hr;

		// Get adapter
		pFactory->EnumAdapters1( pDesc->adapter, &pAdapter );

		if ( pDesc->bDebug )
			createFlags |= D3D11_CREATE_DEVICE_DEBUG;

		D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		D3D_FEATURE_LEVEL retFet;
		hr = D3D11CreateDevice( pAdapter, 
			DX11Mapping::ToDXProcessingType(ERendererProcessingType::UNKNOWN),
			//DX11Mapping::ToDXProcessingType(pDesc->processingType),
			PSX_NULL,
			createFlags,
			featureLevel,
			1,
			D3D11_SDK_VERSION,
			&m_pDevice,
			&retFet,
			&m_pDeviceContext );

		if ( FAILED(hr) )
		{
			PSX_PushError( "Failed to create GraphicsDevice object." );
		
			if ( pDesc->processingType == ERendererProcessingType::HARDWARE && pDesc->bSoftwareRendererFallback )
			{
				PSX_PushLog1( "Attempting to create software device renderer." );

				// TODO: This fallback create device doesn't work. Fix this.
				hr = D3D11CreateDevice( pAdapter, 
					D3D_DRIVER_TYPE_REFERENCE,
					PSX_NULL,
					createFlags,
					featureLevel,
					1,
					D3D11_SDK_VERSION,
					&m_pDevice,
					&retFet,
					&m_pDeviceContext );

				if ( FAILED(hr ) )
					PSX_PushError( "Failed to create software renderer device. Something went terribly wrong!" );
			}
		}

		pAdapter->Release();

		// It's now safe to initialize graphics resource manager
		if ( GraphicsResourceManager::GetInstance()->Initialize() != EErrorCode::OKAY )
		{
			PSX_PushError( "Failed to initialize Graphics Resource Manager." );
			return EErrorCode::GRAPHICS;
		}

		return m_pDevice ? EErrorCode::OKAY : EErrorCode::GRAPHICS;
	}

}