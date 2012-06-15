/**************************************************************************************
*
*	Copyright (C) 2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	TempGraphicsDevice.cpp
*
*	Comments	-	See TempGraphicsDevice.h
*
**************************************************************************************/


#include "../Include/TempGraphicsDevice.h"
#include "../Include/WindowApplicationWin32.h"
#include "../Include/PTLBase.h"

#if defined( PSX_PLATFORM_WINDOWS_WINDOW )

namespace Pulse
{

	TempGraphicsDevice *gpTempGraphicsDevice = NULL;

	BOOL TempGraphicsDevice::Initialize( void )
	{
		if ( m_pFactory )
			return TRUE;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pFactory );
		PSX_Assert( !FAILED(hr), "Failed to create DXGIFactory." );

		return EnumerateDevices();
	}
	
	BOOL TempGraphicsDevice::EnumerateDevices( void )
	{
		BOOL	ret = TRUE;
		m_enumDisplayModes.Clear();

		// Get default adapter
		IDXGIAdapter *pAdapter = NULL;
		if ( m_pFactory->EnumAdapters( 0, &pAdapter ) == DXGI_ERROR_NOT_FOUND )
		{
			ret = FALSE;
			goto RETURN_EnumerateDevices;
		}

		// default output
		IDXGIOutput *pOutput = NULL;
		if ( pAdapter->EnumOutputs( 0, &pOutput ) == DXGI_ERROR_NOT_FOUND )
		{
			ret = FALSE;
			goto RETURN_EnumerateDevices;
		}
			
		// Display Enum params
		UINT numModes			= 0;
		DXGI_FORMAT format		= DXGI_FORMAT_R8G8B8A8_UNORM;
		UINT flags				= DXGI_ENUM_MODES_INTERLACED;
		DXGI_MODE_DESC *pModes	= NULL;

		pOutput->GetDisplayModeList( format, flags, &numModes, NULL );
		pModes = new DXGI_MODE_DESC[ numModes ];
		PSX_Assert( pModes, "Out of memory." );
		pOutput->GetDisplayModeList( format, flags, &numModes, pModes );

		// Store enumerated modes
		for ( SIZE_T i = 0; i < numModes; ++i )
		{
			GraphicsDisplayMode newMode;

			newMode.height		= pModes[i].Height;
			newMode.width		= pModes[i].Width;
			newMode.refreshRate = pModes[i].RefreshRate.Numerator / pModes[i].RefreshRate.Denominator;

			m_enumDisplayModes.PushBack( newMode );
		}

		// Make sure we don't leak any memory
RETURN_EnumerateDevices:
		PSX_SafeDeleteArray( pModes );
		PSX_SafeRelease( pOutput );
		PSX_SafeRelease( pAdapter );

		return ret;
	}

	BOOL TempGraphicsDevice::GetDeviceEnums( GraphicsDisplayMode *pModes, SIZE_T *pNumModes )
	{
		if ( *pNumModes == 0 )
		{
			*pNumModes = m_enumDisplayModes.GetSize();
			return TRUE;
		}

		// Needs to be exactly the size of m_enumDisplayModes
		if ( *pNumModes != m_enumDisplayModes.GetSize() )
			return FALSE;

		for ( SIZE_T i = 0; i < m_enumDisplayModes.GetSize(); ++i )
			pModes[i] = m_enumDisplayModes[i]; 

		return TRUE;
	}

	BOOL TempGraphicsDevice::FindClosestDisplayMode( const GraphicsDisplayMode *pMode, GraphicsDisplayMode *pClosestMatch )
	{
		PSX_Assert( pMode, "pMode == null." );
		GraphicsDisplayMode *pCurrBestMode = NULL;
		FLOAT aspectRatio = (FLOAT)pMode->width / (FLOAT)pMode->height;

		for ( SIZE_T i = 0; i < m_enumDisplayModes.GetSize(); ++i )
		{
			// Find next closest resolution
			if ( pMode->width <= m_enumDisplayModes[i].width && pMode->height <= m_enumDisplayModes[i].height )
			{
				// Stick with the current resolution. 
				// If we get past our selected best resolution then choose the last one.
				if ( pCurrBestMode && 
					pCurrBestMode->width != m_enumDisplayModes[i].width &&
					pCurrBestMode->height != m_enumDisplayModes[i].height )
				{
					*pClosestMatch = m_enumDisplayModes[i - 1];
					return TRUE;
				}

				// Best matching mode will be based on the same aspect ratio
				FLOAT abs = aspectRatio - ((FLOAT)m_enumDisplayModes[i].width / (FLOAT)m_enumDisplayModes[i].height);
				abs = PSX_Abs( abs );
				if ( abs <= 0.10f && pMode->refreshRate <= m_enumDisplayModes[i].refreshRate )
				{
					*pClosestMatch = m_enumDisplayModes[i];
					return TRUE;
				}
				else
					pCurrBestMode = &m_enumDisplayModes[i];
			}
		}

		// Unfortunately we can't find the closest so we'll just assigned the highest resolution instead...
		*pClosestMatch = *pCurrBestMode;
		return TRUE;
	}

	BOOL TempGraphicsDevice::CreateDevice( TempGraphicsDeviceDesc *pDesc )
	{
		HRESULT hr;
		m_graphicsDesc = *pDesc;

		DXGI_SWAP_CHAIN_DESC swapDesc;
		PSX_ZeroMem( &swapDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );

		// Buffer description
		swapDesc.BufferDesc.Width					= m_graphicsDesc.mode.width;
		swapDesc.BufferDesc.Height					= m_graphicsDesc.mode.height;
		swapDesc.BufferDesc.RefreshRate.Numerator	= m_graphicsDesc.mode.refreshRate;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

		// No multisampling ( well, at least for now... )
		swapDesc.SampleDesc.Count	= 1;
		swapDesc.SampleDesc.Quality = 0;

		// The rest of the description
		swapDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount	= 1;
		swapDesc.OutputWindow	= gpApp->m_hWnd;
		swapDesc.Windowed		= TRUE; // If application wants full-screen, switch it later based on MSDN
		swapDesc.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Flags			= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		UINT createDeviceFlags = NULL;
#ifdef PSX_DEBUG
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif /* PSX_DEBUG */

		// Force resize the window to pDesc's size first
		gpApp->SetAllowResize( TRUE );
		gpApp->ResetWindowSize( m_graphicsDesc.mode.width, m_graphicsDesc.mode.height );
		gpApp->FixWindowSize();
		gpApp->SetAllowResize( FALSE );

		// Get default adapter
		IDXGIAdapter *pAdapter = NULL;
		if ( m_pFactory->EnumAdapters( 0, &pAdapter ) == DXGI_ERROR_NOT_FOUND )
		{
			PSX_Assert( FALSE, "Failed to create IDXGIAdapter." );
		}

		// Create device
		hr = D3D10CreateDevice( pAdapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, &m_pDevice );
		if ( FAILED( hr ) ) return FALSE;

		// Then swapchain
		hr = m_pFactory->CreateSwapChain( m_pDevice, &swapDesc, &m_pSwapchain );
		if ( FAILED( hr ) )
		{
			m_pDevice->Release();
			return FALSE;
		}

		// Disable Alt-Enter 
		m_pFactory->MakeWindowAssociation( gpApp->m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES );

		// Are we creating a full-screen app?
		if ( m_graphicsDesc.windowed == FALSE )
			ToggleFullscreen();

		OnReset();

		//// Create default effects
		//LoadEffectFromFile( "Resources/Effects/Unlit.fx", &m_pFXUnlit );
		//m_pFXTechUnlit	 = m_pFXUnlit->GetTechniqueByName( "UnlitTech" );
		//m_pFXVarUnlitWVP = m_pFXUnlit->GetVariableByName( "gWVP" )->AsMatrix();
		//m_pFXTechUnlit->GetPassByIndex(0)->

		//// Instantiate Input layouts
		//D3D10_INPUT_ELEMENT_DESC vertexLayout1P1D[] = {
		//	{ PSX_String("POSITION"), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//	{ PSX_String("DIFFUSE"), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//};

		//D3D10_INPUT_ELEMENT_DESC vertexLayout1P1N[] = {
		//	{ PSX_String("POSITION"), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//	{ PSX_String("NORMAL"), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//};

		//D3D10_INPUT_ELEMENT_DESC vertexLayout1P1N1A1D1S[] = {
		//	{ PSX_String("POSITION"), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//	{ PSX_String("NORMAL"), 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//	{ PSX_String("AMBIENT"), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//	{ PSX_String("DIFUSE"), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//	{ PSX_String("SPECULAR"), 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D10_INPUT_PER_VERTEX_DATA, 0  },
		//};

		//m_pDevice->CreateInputLayout( vertexLayout1P1D, 2,  

		gpTempGraphicsDevice = this;

		return TRUE;
	}

	void TempGraphicsDevice::ToggleFullscreen( void )
	{
		m_graphicsDesc.windowed = !m_graphicsDesc.windowed;
		if ( !m_graphicsDesc.windowed )
		{
			gpApp->SetAllowResize( TRUE );
			gpApp->ResetWindowSize( m_graphicsDesc.mode.width, m_graphicsDesc.mode.height );
			//gpApp->FixWindowSize();
			gpApp->SetAllowResize( FALSE );
		}
		m_pSwapchain->SetFullscreenState( !m_graphicsDesc.windowed, NULL );
		OnReset();
	}

	void TempGraphicsDevice::OnReset( void )
	{
		HRESULT hr;
		PSX_SafeRelease( m_pRenderTargetView );
		PSX_SafeRelease( m_pDepthStencilView );
		PSX_SafeRelease( m_pDepthStencil );

		//hr = m_pSwapchain->ResizeBuffers( 1, m_graphicsDesc.mode.width, m_graphicsDesc.mode.height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH );
		//PSX_Assert( !FAILED(hr), "Failed to resize SwapChain buffers." );

		// Get view for render target
		ID3D10Texture2D *pBackBuffer;
		m_pSwapchain->GetBuffer( 0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&pBackBuffer) );
		m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
		pBackBuffer->Release();

		// create Depth/Stencil buffer and view
		D3D10_TEXTURE2D_DESC depthStencilDesc;
		PSX_ZeroMem( &depthStencilDesc, sizeof( D3D10_TEXTURE2D_DESC ) );
		depthStencilDesc.Width				= m_graphicsDesc.mode.width;
		depthStencilDesc.Height				= m_graphicsDesc.mode.height;
		depthStencilDesc.MipLevels			= 1;
		depthStencilDesc.ArraySize			= 1;
		depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count	= 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage				= D3D10_USAGE_DEFAULT;
		depthStencilDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags		= 0;
		depthStencilDesc.MiscFlags			= 0;

		hr = m_pDevice->CreateTexture2D( &depthStencilDesc, 0, &m_pDepthStencil );
		PSX_Assert( !FAILED(hr), "Failed to create depth/stencil." );
		hr = m_pDevice->CreateDepthStencilView( m_pDepthStencil, NULL, &m_pDepthStencilView );
		PSX_Assert( !FAILED(hr), "Failed to create depth/stencil resource view." );

		// Bind the render targer and dept/stencil view to the pipeline
		m_pDevice->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

		// Set viewport transform
		D3D10_VIEWPORT vp;
		vp.TopLeftX	= 0;
		vp.TopLeftY	= 0;
		vp.Width	= m_graphicsDesc.mode.width;
		vp.Height	= m_graphicsDesc.mode.height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		m_pDevice->RSSetViewports( 1, &vp );
	}

	//BOOL TempGraphicsDevice::LoadEffectFromFile( const CHAR *pFileName, ID3D10Effect **ppEffect )
	//{
	//	HRESULT hr;
	//	ID3D10Blob *pErrors = NULL;
	//	UINT shaderFlags = D3D10_SHADER_IEEE_STRICTNESS;
	//	D3DX10CreateEffectFromFile( pFileName, NULL, NULL, PSX_String("fx_4_0"), shaderFlags, 0, m_pDevice, NULL, NULL, ppEffect, &pErrors, &hr );
	//	PSX_Assert( !FAILED(hr), "Failed to create effect file." );

	//	if ( pErrors )
	//	{
	//		MessageBox( NULL, (Char*)pErrors->GetBufferPointer(), PSX_String("Warning"), MB_OK );
	//		pErrors->Release();
	//	}

	//	return TRUE;
	//}

	//void TempGraphicsDevice::OnRelease( void )
	//{

	//}

}

#endif /* PSX_PLATFORM_WINDOWS_WINDOW */