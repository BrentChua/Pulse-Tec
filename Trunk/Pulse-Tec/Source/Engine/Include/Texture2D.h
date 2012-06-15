/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Texture2D.h
*
*	Description -	Class representing a 2D texture.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/11/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_TEXTURE_2D_H_
#define _PSX_TEXTURE_2D_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsResource.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"
#include "String.h"

namespace Pulse
{
	struct Texture2DDesc : public BaseResDesc
	{
	public:

		Texture2DDesc( void );

		void SetAsDepthBuffer( SIZE_T _width = 0, SIZE_T _height = 0 );

		void SetAsRenderTarget( SIZE_T _width = 0, SIZE_T _height = 0 );

		void SetAsSwapChainBuffer( void *pSwapChainBuffer );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::TEXTURE2D; }

		String				filename;
		EGraphicsFormat::Type format;
		ETextureUsage::Type	usage;
		SIZE_T				width;
		SIZE_T				height;
		SIZE_T				arraySize;
		BOOL				bGenerateMips;
		BOOL				bTextureCube;
		void				*pData;
	};

	class Texture2D : public IResourcePoolItem, public GraphicsResource
	{
	public:

		virtual ~Texture2D( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );				

	private:

		PSX_MakeStoragePoolFriend( Texture2D );

		Texture2D( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

		UINT GetDescMipLevels( void );

		D3D11_USAGE GetDescUsage( void );

		UINT GetDescBindFlags( void );

		UINT GetDescCPUUsageFlags( void );

		UINT GetDescMiscFlags( void );

	private:

		Texture2DDesc	m_desc;
		ID3D11Texture2D *m_pTexture;

	};
}

#endif /* _PSX_TEXTURE_2D_H_ */