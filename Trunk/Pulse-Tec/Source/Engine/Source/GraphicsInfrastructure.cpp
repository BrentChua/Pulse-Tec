/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsInfrastructure.cpp
*
*	Comments	-	See GraphicsInfrastructure.h
*
**************************************************************************************/

#ifndef __PSX_WINDOWS_CONSOLE

#include "../Include/GraphicsInfrastructure.h"

#include "../Include/DX11Mapping.h"

// Library imports
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx11.lib" )
//#pragma comment( lib, "d3dx10.lib" )
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "d3dcompiler.lib" )
//#pragma comment( lib, "d3d9.lib" )

namespace Pulse
{
	GraphicsInfrastructure::GraphicsInfrastructure( void )
	{
		Initialize();
	}

	GraphicsInfrastructure::~GraphicsInfrastructure( void )
	{

	}

	EErrorCode::Type GraphicsInfrastructure::Initialize( void )
	{
		CreateDXGIFactory1( __uuidof(DXGIFactoryPtr::Type), (void**)m_pGIFactory.GetPointer() );
		
		return EErrorCode::OKAY;
	}

	void GraphicsInfrastructure::Cleanup( void )
	{
		m_pGIFactory.Reset();
	}

	const SIZE_T GraphicsInfrastructure::GetNumAdapters( void )
	{
		SIZE_T numAdapters = 0;
		IDXGIAdapter1 *pAdapter;
		
		while ( m_pGIFactory->EnumAdapters1( numAdapters, &pAdapter ) != DXGI_ERROR_NOT_FOUND )
		{
			++numAdapters;
			pAdapter->Release();
		}

		return numAdapters;
	}

	const SIZE_T GraphicsInfrastructure::GetNumAdpaterOutputs( UINT adapter )
	{
		if ( adapter >= GetNumAdapters() )
			return EErrorCode::INDEX_OUT_OF_BOUND;

		UINT outputIndex = 0;
		IDXGIAdapter1 *pAdapter;
		HRESULT hr;

		if ( m_pGIFactory->EnumAdapters1( adapter, &pAdapter ) != DXGI_ERROR_NOT_FOUND )
		{
			IDXGIOutput *pOutput;

			while ( pAdapter->EnumOutputs( outputIndex, &pOutput ) != DXGI_ERROR_NOT_FOUND )
			{
				++outputIndex;
				pOutput->Release();
			}

			pAdapter->Release();
		}
		else
			return EErrorCode::GRAPHICS;

		return outputIndex;
	}

	EErrorCode::Type GraphicsInfrastructure::GetAvailableDisplayModes( UINT adapter, UINT adapterOutput, EGraphicsFormat::Type format, DisplayModeList *pOut )
	{
		if ( pOut == PSX_NULL )
			return EErrorCode::INVALID_PARAMETER;

		if ( adapter >= GetNumAdapters() )
				return EErrorCode::INDEX_OUT_OF_BOUND;

		IDXGIAdapter1 *pAdapter;

		if ( m_pGIFactory->EnumAdapters1( adapter, &pAdapter ) != DXGI_ERROR_NOT_FOUND )
		{
			IDXGIOutput *pOutput;
			UINT numModes;

			if ( pAdapter->EnumOutputs( adapterOutput, &pOutput ) != DXGI_ERROR_NOT_FOUND )
			{
				// TODO: Update to GetDisplayModeList1 in DX11.1
				// Get number of display modes
				pOutput->GetDisplayModeList( DX11Mapping::ToDXBufferFormat( format ), NULL, &numModes, NULL );
				
				if ( numModes )
				{
					DXGI_MODE_DESC *pModes = new DXGI_MODE_DESC[numModes];

					// Now get list of display modes
					pOutput->GetDisplayModeList( DX11Mapping::ToDXBufferFormat( format ), NULL, &numModes, pModes );

					for ( UINT i = 0; i < numModes; ++i )
					{
						if ( pModes[i].Scaling != DXGI_MODE_SCALING_UNSPECIFIED )
							continue;

						if ( pOut->GetSize() && 
							pModes[i].Width == pOut->GetTail().width &&
							pModes[i].Height == pOut->GetTail().height )
							continue;

						FLOAT rate = Math::Ceiling( (FLOAT)pModes[i].RefreshRate.Numerator / (FLOAT)pModes[i].RefreshRate.Denominator );
						if ( rate < 60.0f )
							continue;

						DisplayMode newMode;

						newMode.adapter = adapter;
						newMode.adapterOutput = adapterOutput;
						newMode.width = pModes[i].Width;
						newMode.height = pModes[i].Height;
						newMode.refreshRate = Rational( (FLOAT)pModes[i].RefreshRate.Numerator, (FLOAT)pModes[i].RefreshRate.Denominator );
						newMode.format = DX11Mapping::ToBufferFormat( pModes[i].Format );
						//pModes[i].ScanlineOrdering; // Ignore scanline ordering
						//pModes[i].Scaling; // Ignore scaling

						pOut->PushBack( newMode );
					}

				}

				pOutput->Release();

			}

			pAdapter->Release();
		}
		else
			return EErrorCode::GRAPHICS;

		return EErrorCode::OKAY;
	}

	EErrorCode::Type GraphicsInfrastructure::GetAvailableDisplayModes( UINT adapter, UINT adapterOutput, EGraphicsFormat::Type format, FLOAT minWidth, FLOAT minHeight, FLOAT ratio, DisplayModeList *pOut )
	{
		if ( pOut == PSX_NULL )
			return EErrorCode::INVALID_PARAMETER;

		if ( adapter >= GetNumAdapters() )
				return EErrorCode::INDEX_OUT_OF_BOUND;

		IDXGIAdapter1 *pAdapter;

		if ( m_pGIFactory->EnumAdapters1( adapter, &pAdapter ) != DXGI_ERROR_NOT_FOUND )
		{
			const FLOAT epsilon = 0.002f;
			IDXGIOutput *pOutput;
			UINT numModes;

			if ( pAdapter->EnumOutputs( adapterOutput, &pOutput ) != DXGI_ERROR_NOT_FOUND )
			{
				// TODO: Update to GetDisplayModeList1 in DX11.1
				// Get number of display modes
				pOutput->GetDisplayModeList( DX11Mapping::ToDXBufferFormat( format ), NULL, &numModes, NULL );
				
				if ( numModes )
				{
					DXGI_MODE_DESC *pModes = new DXGI_MODE_DESC[numModes];

					// Now get list of display modes
					pOutput->GetDisplayModeList( DX11Mapping::ToDXBufferFormat( format ), NULL, &numModes, pModes );

					for ( UINT i = 0; i < numModes; ++i )
					{
						if ( pModes[i].Scaling != DXGI_MODE_SCALING_UNSPECIFIED )
							continue;

						if ( pOut->GetSize() && 
							pModes[i].Width == pOut->GetTail().width &&
							pModes[i].Height == pOut->GetTail().height )
							continue;

						FLOAT rate = Math::Ceiling( (FLOAT)pModes[i].RefreshRate.Numerator / (FLOAT)pModes[i].RefreshRate.Denominator );
						if ( rate < 60.0f )
							continue;

						// Check if it meets the screen requirement
						if ( pModes[i].Width < minWidth || pModes[i].Height < minHeight )
							continue;

						// Check ratio requirement
						FLOAT modeRatio = (FLOAT)pModes[i].Width / (FLOAT)pModes[i].Height;
						if ( modeRatio > ratio + epsilon || modeRatio < ratio - epsilon )
							continue;

						DisplayMode newMode;

						newMode.adapter = adapter;
						newMode.adapterOutput = adapterOutput;
						newMode.width = pModes[i].Width;
						newMode.height = pModes[i].Height;
						newMode.refreshRate = Rational( (FLOAT)pModes[i].RefreshRate.Numerator, (FLOAT)pModes[i].RefreshRate.Denominator );
						newMode.format = DX11Mapping::ToBufferFormat( pModes[i].Format );
						//pModes[i].ScanlineOrdering; // Ignore scanline ordering
						//pModes[i].Scaling; // Ignore scaling

						pOut->PushBack( newMode );
					}

				}

				pOutput->Release();

			}

			pAdapter->Release();
		}
		else
			return EErrorCode::GRAPHICS;

		return EErrorCode::OKAY;
	}

	Vector2 GraphicsInfrastructure::GetCurrentResolution( UINT adapter, UINT adapterOutput )
	{
		DXGI_OUTPUT_DESC desc;
		IDXGIAdapter1 *pAdapter;

		if ( m_pGIFactory->EnumAdapters1( adapter, &pAdapter ) != DXGI_ERROR_NOT_FOUND )
		{
			IDXGIOutput *pOutput;
			UINT numModes;

			if ( pAdapter->EnumOutputs( adapterOutput, &pOutput ) != DXGI_ERROR_NOT_FOUND )
			{
				pOutput->GetDesc( &desc );

				pOutput->Release();

			}

			pAdapter->Release();
		}

		return Vector2( static_cast<FLOAT>(desc.DesktopCoordinates.right - desc.DesktopCoordinates.left),
			static_cast<FLOAT>(desc.DesktopCoordinates.bottom - desc.DesktopCoordinates.top) );
	}

}

#endif