/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsResourceManager.cpp
*
*	Comments	-	See GraphicsResourceManager.h
*
**************************************************************************************/

#ifdef __PSX_WINDOWS_WINDOW

#include "../Include/GraphicsResourceManager.h"
#include "../Include/GraphicsTypes.h"

namespace Pulse
{

	GraphicsResourceManager::GraphicsResourceManager( void )
	{

	}

	GraphicsResourceManager::~GraphicsResourceManager( void )
	{

	}
		
	Texture2D * GraphicsResourceManager::CreateTexture2D( const CHAR *pName, Texture2DDesc *pDesc )
	{
		Texture2D *pTex = m_2Dtextures.CreateResource( pName, pDesc );

		return pTex;
	}

	SwapChain * GraphicsResourceManager::CreateSwapChain( const CHAR *pName, SwapChainDesc *pDesc )
	{
		return m_swapChains.CreateResource( pName, pDesc );
	}

	VertexShader * GraphicsResourceManager::CreateVertexShader( const CHAR *pName, ShaderDesc *pDesc )
	{
		return m_vertexShaders.CreateResource( pName, pDesc );
	}

	HullShader * GraphicsResourceManager::CreateHullShader( const CHAR *pName, ShaderDesc *pDesc )
	{
		return m_hullShaders.CreateResource( pName, pDesc );
	}

	DomainShader * GraphicsResourceManager::CreateDomainShader( const CHAR *pName, ShaderDesc *pDesc )
	{
		return m_domainShaders.CreateResource( pName, pDesc );
	}

	GeometryShader * GraphicsResourceManager::CreateGeometryShader( const CHAR *pName, ShaderDesc *pDesc )
	{
		return m_geometryShaders.CreateResource( pName, pDesc );
	}

	PixelShader * GraphicsResourceManager::CreatePixelShader( const CHAR *pName, ShaderDesc *pDesc )
	{
		return m_pixelShaders.CreateResource( pName, pDesc );
	}

	ComputeShader * GraphicsResourceManager::CreateComputeShader( const CHAR *pName, ShaderDesc *pDesc )
	{
		return m_computeShaders.CreateResource( pName, pDesc );
	}

	InputLayout * GraphicsResourceManager::CreateInputLayout( const CHAR *pName, InputLayoutDesc *pDesc )
	{
		return m_inputLayouts.CreateResource( pName, pDesc );
	}

	EErrorCode::Type GraphicsResourceManager::Initialize( void )
	{
		// TODO: We can transfer the growSize to a script to make it dynamic.
		ResourceManager *pResMan = ResourceManager::GetInstance();

		// Initialize and register sub-resource-managers
		m_swapChains.Create( 5 );
		pResMan->RegisterResourcePool( &m_swapChains, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::SWAP_CHAIN ) );
		m_2Dtextures.Create( 20 );
		pResMan->RegisterResourcePool( &m_2Dtextures, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::TEXTURE2D ) );
		m_vertexShaders.Create( 20 );
		pResMan->RegisterResourcePool( &m_vertexShaders, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::VERTEX_SHADER ) );
		m_hullShaders.Create( 20 );
		pResMan->RegisterResourcePool( &m_hullShaders, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::HULL_SHADER ) );
		m_domainShaders.Create( 20 );
		pResMan->RegisterResourcePool( &m_domainShaders, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::DOMAIN_SHADER ) );
		m_geometryShaders.Create( 20 );
		pResMan->RegisterResourcePool( &m_geometryShaders, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::GEOMETRY_SHADER ) );
		m_pixelShaders.Create( 20 );
		pResMan->RegisterResourcePool( &m_pixelShaders, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::PIXEL_SHADER ) );
		m_computeShaders.Create( 20 );
		pResMan->RegisterResourcePool( &m_computeShaders, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::COMPUTE_SHADER ) );
		m_inputLayouts.Create( 20 );
		pResMan->RegisterResourcePool( &m_inputLayouts, ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::INPUT_LAYOUT ) );
		return EErrorCode::OKAY;
	}

	void GraphicsResourceManager::Cleanup( void )
	{
		ResourceManager *pResMan = ResourceManager::GetInstance();

		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::SWAP_CHAIN ) );
		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::TEXTURE2D ) );
		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::VERTEX_BUFFER) );
		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::HULL_SHADER ) );
		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::DOMAIN_SHADER ) );
		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::GEOMETRY_SHADER ) );
		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::PIXEL_SHADER ) );
		pResMan->UnregisterResourcePool( ResourceRegisterCode( EResourceType::VIDEO, EVideoResourceType::COMPUTE_SHADER ) );
	}

	
}

#endif __PSX_WINDOWS_WINDOW
