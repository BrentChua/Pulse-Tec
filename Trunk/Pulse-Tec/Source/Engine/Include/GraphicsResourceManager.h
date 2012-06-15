/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsResourceManager.h
*
*	Description -	Graphics resource management system.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/25/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_VERTEX_MANAGER_H_
#define _PSX_VERTEX_MANAGER_H_

#include "PulseSTD.h"
#include "ResourcePool.h"
#include "Singleton.h"
#include "HashTable.h"
#include "GraphicsTypes.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "SwapChain.h"
#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "InputLayout.h"

namespace Pulse
{

	class GraphicsResourceManager : public Singleton< GraphicsResourceManager >
	{
	public:

		virtual ~GraphicsResourceManager( void );

		// TODO: Add Create resources methods here
		Texture2D * CreateTexture2D( const CHAR *pName, Texture2DDesc *pDesc );
	
		SwapChain * CreateSwapChain( const CHAR *pName, SwapChainDesc *pDesc );

		VertexShader * CreateVertexShader( const CHAR *pName, ShaderDesc *pDesc );

		HullShader * CreateHullShader( const CHAR *pName, ShaderDesc *pDesc );

		DomainShader * CreateDomainShader( const CHAR *pName, ShaderDesc *pDesc );

		GeometryShader * CreateGeometryShader( const CHAR *pName, ShaderDesc *pDesc );

		PixelShader * CreatePixelShader( const CHAR *pName, ShaderDesc *pDesc );

		ComputeShader * CreateComputeShader( const CHAR *pName, ShaderDesc *pDesc );
	
		InputLayout * CreateInputLayout( const CHAR *pName, InputLayoutDesc *pDesc );

	private:

		friend GraphicsDevice;

		GraphicsResourceManager( void );

		virtual EErrorCode::Type Initialize( void );

		virtual void Cleanup( void );

	private:
		
		friend Singleton;

		ResourcePool< SwapChain > m_swapChains;
		ResourcePool< Texture2D > m_2Dtextures;
		ResourcePool< VertexShader > m_vertexShaders;
		ResourcePool< HullShader > m_hullShaders;
		ResourcePool< DomainShader > m_domainShaders;
		ResourcePool< GeometryShader > m_geometryShaders;
		ResourcePool< PixelShader > m_pixelShaders;
		ResourcePool< ComputeShader > m_computeShaders;
		
		ResourcePool< InputLayout > m_inputLayouts;

	};


}

#endif /* _PSX_VERTEX_BUFFER_MANAGER_H_ */