/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Texture2D.cpp
*
*	Comments	-	See Texture2D.h
*
**************************************************************************************/
#include "../Include/Texture2D.h"
#include "../Include/Engine.h"
#include "../Include/FileSystem.h"
#include "../Include/ResourceCache.h"
#include "../Include/GraphicsDevice.h"

namespace Pulse
{
	Texture2DDesc::Texture2DDesc( void )
	{
		format = EGraphicsFormat::UNKNOWN;
		usage = ETextureUsage::UNKNOWN;
		width = 0;
		height = 0;
		arraySize = 1;
		bGenerateMips = FALSE;
		bTextureCube = FALSE;
		pData = PSX_NULL;
	}

	void Texture2DDesc::SetAsDepthBuffer( SIZE_T _width, SIZE_T _height )
	{
		Engine *pEngine = Engine::GetInstance();
		const DisplayMode *pMode = pEngine->GetDisplaySettings();

		width = _width == 0 ? pMode->width: _width;
		height = _height == 0 ? pMode->height : _height;
		format = EGraphicsFormat::D32_FLOAT;
		usage = ETextureUsage::DEPTHBUFFER;
	}

	void Texture2DDesc::SetAsRenderTarget( SIZE_T _width, SIZE_T _height )
	{
		Engine *pEngine = Engine::GetInstance();
		const DisplayMode *pMode = pEngine->GetDisplaySettings();

		width = _width == 0 ? pMode->width: _width;
		height = _height == 0 ? pMode->height : _height;
		format = EGraphicsFormat::R8G8B8A8_SNORM;
		usage = ETextureUsage::RENDERTARGET;
	}

	void Texture2DDesc::SetAsSwapChainBuffer( void *pSwapChainBuffer )
	{
		Engine *pEngine = Engine::GetInstance();
		const DisplayMode *pMode = pEngine->GetDisplaySettings();

		usage = ETextureUsage::SWAPCHAINBUFFER;
		pData = (void*)pSwapChainBuffer;
	}

	Texture2D::Texture2D( void )
	{
	}

	Texture2D::~Texture2D( void )
	{
	}

	BOOL Texture2D::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::TEXTURE2D )
			return FALSE;

		m_desc = *((Texture2DDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}
	
	BOOL Texture2D::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}
	
	BOOL Texture2D::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}
	
	BOOL Texture2D::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type Texture2D::Initialize( void )
	{
		ID3D11Device *pDevice = (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice();

		if ( m_desc.filename.IsEmpty() == FALSE ) // Load texture
		{
			// Load texture in memory
			FileHandlePtr hFile = FileSystem::GetInstance()->OpenFile( m_desc.filename.GetBuffer() );

			if ( !hFile->IsValid() )
			{
				PSX_PushError( "Failed to load texture file: %s", m_desc.filename.GetBuffer() );
				return EErrorCode::FILE_NOT_AVAILABLE;
			}

			ResHandlePtr hResource = Engine::GetInstance()->GetResourceCache()->GetHandle( hFile );

			HRESULT hr = D3DX11CreateTextureFromMemory( pDevice, hResource->GetBuffer(), hResource->GetSize(), PSX_NULL,
				PSX_NULL, (ID3D11Resource**)&m_pTexture, PSX_NULL );
		
			if ( FAILED( hr ) )
			{
				PSX_PushError( "Failed to load texture file: %s", m_desc.filename.GetBuffer() );
				return EErrorCode::GRAPHICS;
			}

			D3D11_TEXTURE2D_DESC desc;
			m_pTexture->GetDesc( &desc );

			m_desc.width = desc.Width;
			m_desc.height = desc.Height;
			m_desc.format = DX11Mapping::ToBufferFormat( desc.Format );
		}
		else	// Create texture
		{
			D3D11_TEXTURE2D_DESC desc;

			if ( m_desc.usage == ETextureUsage::SWAPCHAINBUFFER )
			{
				m_pTexture = (ID3D11Texture2D*)m_desc.pData;
				m_pTexture->AddRef();

				D3D11_TEXTURE2D_DESC texDesc;
				m_pTexture->GetDesc( &texDesc );

				m_desc.format = DX11Mapping::ToBufferFormat( texDesc.Format );
				//m_desc.usage = ETextureUsage::SWAPCHAINBUFFER;
				m_desc.width = texDesc.Width;
				m_desc.height = texDesc.Height;
				m_desc.arraySize = texDesc.ArraySize;
				m_desc.bGenerateMips = texDesc.MipLevels == 1 ? FALSE : TRUE;
				m_desc.pData = PSX_NULL;

			}
			else
			{
				desc.Width = m_desc.width;
				desc.Height = m_desc.height;
				desc.MipLevels = GetDescMipLevels();
				desc.ArraySize = m_desc.arraySize;
				desc.Format = DX11Mapping::ToDXBufferFormat( m_desc.format );
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = GetDescUsage();
				desc.BindFlags = GetDescBindFlags();
				desc.CPUAccessFlags = GetDescCPUUsageFlags();
				desc.MiscFlags = GetDescMiscFlags();

				D3D11_SUBRESOURCE_DATA data;
				data.pSysMem = m_desc.pData;

				HRESULT hr = pDevice->CreateTexture2D( &desc, PSX_NULL, &m_pTexture );
		
				if ( FAILED( hr ) )
				{
					PSX_PushError( "Failed to create texture." );
					return EErrorCode::GRAPHICS;
				}
			}
		}

		D3D11_TEXTURE2D_DESC desc;
		m_pTexture->GetDesc( &desc );
		CreateResourceViews( (ID3D11Resource*)m_pTexture, desc.BindFlags );

		return EErrorCode::OKAY;
	}

	void Texture2D::Cleanup( void )
	{
		ReleaseResourceViews();
		PSX_SafeRelease( m_pTexture );
	}

	UINT Texture2D::GetDescMipLevels( void )
	{
		if ( m_desc.usage == ETextureUsage::RENDERTARGET )
			return 1;
		else if ( m_desc.usage == ETextureUsage::DYNAMIC )
			return 1;
		else if ( m_desc.usage == ETextureUsage::STAGING )
			return 1;
		else if ( m_desc.usage == ETextureUsage::NORMAL )
			return 0;
		else if ( m_desc.usage == ETextureUsage::STATIC )
			return 0;
		else if ( m_desc.usage == ETextureUsage::DEPTHBUFFER )
			return 0;
		else
			return 0;

	}

	D3D11_USAGE Texture2D::GetDescUsage( void )
	{
		switch ( m_desc.usage )
		{
		case ETextureUsage::DYNAMIC:
			return D3D11_USAGE_DYNAMIC;
		case ETextureUsage::STAGING:
			return D3D11_USAGE_STAGING;
		case ETextureUsage::STATIC:
			return D3D11_USAGE_IMMUTABLE;
		case ETextureUsage::DEPTHBUFFER:
		case ETextureUsage::RENDERTARGET:
		case ETextureUsage::NORMAL:
		default:
			return D3D11_USAGE_DEFAULT;
		}
	}

	UINT Texture2D::GetDescBindFlags( void )
	{
		UINT flags = 0;
		
		if ( m_desc.usage == ETextureUsage::RENDERTARGET )
			flags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		else if ( m_desc.usage == ETextureUsage::DYNAMIC )
			flags = D3D11_BIND_SHADER_RESOURCE;
		else if ( m_desc.usage == ETextureUsage::STAGING )
			flags = D3D11_BIND_SHADER_RESOURCE; // NOTE: Not sure what for staging
		else if ( m_desc.usage == ETextureUsage::NORMAL )
			flags = D3D11_BIND_SHADER_RESOURCE;
		else if ( m_desc.usage == ETextureUsage::STATIC )
			flags = D3D11_BIND_SHADER_RESOURCE;
		else if  ( m_desc.usage == ETextureUsage::DEPTHBUFFER )
			flags =  D3D11_BIND_DEPTH_STENCIL;

		return flags;
	}

	UINT Texture2D::GetDescCPUUsageFlags( void )
	{
		switch ( m_desc.usage )
		{
		case ETextureUsage::STAGING:
			return D3D11_CPU_ACCESS_READ;
		case ETextureUsage::DYNAMIC:
			return D3D11_CPU_ACCESS_WRITE;
		case ETextureUsage::STATIC:
		case ETextureUsage::DEPTHBUFFER:
		case ETextureUsage::RENDERTARGET:
		case ETextureUsage::NORMAL:
		default:
			return 0;
		}
	}

	UINT Texture2D::GetDescMiscFlags( void )
	{
		UINT flags = 0;

		if ( m_desc.bGenerateMips )
			flags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

		if ( m_desc.bTextureCube )
			flags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		return flags;
	}

}