/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourceManager.h
*
*	Description -	Resource management system.
*
*	Comments	-	
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/25/10	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_RESOURCE_MANAGER_H_
#define _PSX_RESOURCE_MANAGER_H_

#include "PulseSTD.h"
#include "Singleton.h"
#include "ResourcePool.h"
#include "ResourcePoolItem.h"

namespace Pulse
{
	PSX_EnumBegin( EResourceType )
		VIDEO,
		AUDIO,
		CUSTOM,
	PSX_EnumEnd()
	
	PSX_EnumBegin( EVideoResourceType )
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D,
		RENDER_TARGET,

		SWAP_CHAIN,
		
		VERTEX_BUFFER,
		INDEX_BUFFER,

		VERTEX_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER,
		COMPUTE_SHADER,

		INPUT_LAYOUT,

		RENDER_SET,
		MODEL,
		IMAGE,
		FONT,
		MATERIAL,

	PSX_EnumEnd()

	class ResourceManager : public Singleton< ResourceManager >
	{
	public:

		~ResourceManager( void );

		// Resource item management interface
		void RegisterResourcePool( IResourcePool *pResPool, ResourceRegisterCode code );
		IResourcePool * UnregisterResourcePool( ResourceRegisterCode code );

		void CleanAll( void );
		void DestroyAll( void );
		void DisableAll( void );
		void RestoreAll( void );

		void CleanResourceType( EResourceType::Type type );
		void DestroyResourceType( EResourceType::Type type );
		void DisableResourceType( EResourceType::Type type );
		void RestoreResrouceType( EResourceType::Type type );

		IResourcePool * FindResourcePool( ResourceRegisterCode code );
		HRes FindResourceHandle( ResourceRegisterCode code, const String *pName );
		IResourcePoolItem * FindResource( ResourceRegisterCode code, const String *pName );
		IResourcePoolItem * FindResource( ResourceRegisterCode code, HRes hRes );

	private:

		friend Singleton;

		explicit ResourceManager( void ) { }
		explicit ResourceManager( ResourceManager & ) { }
		ResourceManager & operator = ( ResourceManager & ) { }

		typedef Map< ResourceRegisterCode, IResourcePool* > ResourcePoolTypeMap;
		typedef ResourcePoolTypeMap::Iterator MapIter;
		typedef List< IResourcePool* > ResourcePoolList;
		typedef ResourcePoolList::Iterator ListIter;

		ResourcePoolList m_resPoolList[EResourceType::EnumCount];
		ResourcePoolTypeMap m_resPoolMap;

	};

	PSX_INLINE ResourceManager::~ResourceManager( void )
	{
		DestroyAll();
	}

	PSX_INLINE void ResourceManager::RegisterResourcePool( IResourcePool *pResPool, ResourceRegisterCode code )
	{
		m_resPoolMap[ code ] = pResPool;
		m_resPoolList[ code.type ].PushBack( pResPool );
	}

	PSX_INLINE void ResourceManager::CleanAll( void )
	{
		for ( INT i = 0; i < EResourceType::EnumCount; ++i )
			CleanResourceType( (EResourceType::Type)i );
	}

	PSX_INLINE void ResourceManager::DestroyAll( void )
	{
		for ( INT i = 0; i < EResourceType::EnumCount; ++i )
			DestroyResourceType( (EResourceType::Type)i );
	}

	PSX_INLINE void ResourceManager::DisableAll( void )
	{
		for ( INT i = 0; i < EResourceType::EnumCount; ++i )
			DisableResourceType( (EResourceType::Type)i );
	}

	PSX_INLINE void ResourceManager::RestoreAll( void )
	{
		for ( INT i = 0; i < EResourceType::EnumCount; ++i )
			RestoreResrouceType( (EResourceType::Type)i );
	}

}

#endif /* _PSX_RESOURCE_MANAGER_H_ */