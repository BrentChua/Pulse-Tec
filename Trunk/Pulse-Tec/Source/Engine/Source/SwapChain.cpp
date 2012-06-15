/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	SwapChain.cpp
*
*	Comments	-	See SwapChain.h
*
**************************************************************************************/

#include "../Include/SwapChain.h"
#include "../Include/WindowApplicationWin32.h"
#include "../Include/DX11Mapping.h"
#include "../Include/Renderer.h"
#include "../Include/Texture2D.h"
#include "../Include/GraphicsResourceManager.h"

namespace Pulse
{
	SwapChain::SwapChain( void )
	{

	}

	SwapChain::~SwapChain( void )
	{

	}

	BOOL SwapChain::CreateResource( BaseResDesc *pDesc )
	{
		if ( pDesc->GetDescType() != EResDescType::SWAP_CHAIN )
			return FALSE;

		m_desc = *((SwapChainDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL SwapChain::DestroyResource( void )
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL SwapChain::DisableResource( void )
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL SwapChain::RestoreResource( void )
	{
		SetFlagRestored();	
		return TRUE;
	}

	EErrorCode::Type SwapChain::Initialize( void )
	{
		// Initialize renderer
		GraphicsInfrastructure *pGI = GraphicsInfrastructure::GetInstance();
		DXGIFactoryPtr pFactory = pGI->GetFactory();

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc.Width = m_desc.width;
		swapChainDesc.BufferDesc.Height = m_desc.height;	
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_desc.refreshRate.numerator;	
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_desc.refreshRate.denominator;	
		swapChainDesc.BufferDesc.Format = DX11Mapping::ToDXBufferFormat( m_desc.format );
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDesc.SampleDesc.Count = m_desc.sampleDesc.count;
		swapChainDesc.SampleDesc.Quality = m_desc.sampleDesc.quality;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = gpApp->GetHWND();
		swapChainDesc.Windowed = m_desc.bWindowed;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		HRESULT hr;
		hr = pFactory->CreateSwapChain( (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice(), &swapChainDesc, &m_pSwapChain );

		if ( FAILED(hr) )
		{
			PSX_PushError( "Failed to create graphics swap chain." );
			return EErrorCode::GRAPHICS;
		}

		ID3D11Texture2D *pSwapChainbuffer;
		m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pSwapChainbuffer );

		if ( FAILED( hr ) )
		{
			PSX_PushError( "Failed to get swap chain buffer." );
			return EErrorCode::GRAPHICS;
		}

		// Encapsulate swap chain buffer to texture 2d
		GraphicsResourceManager *pMan = GraphicsResourceManager::GetInstance();
		Texture2DDesc desc;
		desc.SetAsSwapChainBuffer( pSwapChainbuffer );
		m_pSwapChainBuffer = pMan->CreateTexture2D( PSX_String("SwapChainBuffer"), &desc );
		pSwapChainbuffer->Release();

		if ( m_pSwapChain == PSX_NULL )
		{
			PSX_PushError( "Failed to get swap chain buffer Texture2D." );
			return EErrorCode::GRAPHICS;
		}
		
		return EErrorCode::NONE;
	}

	void SwapChain::Cleanup( void )
	{
		ReleaseResourceViews();
		PSX_SafeRelease( m_pSwapChainBuffer );
		PSX_SafeRelease( m_pSwapChain );
	}
}