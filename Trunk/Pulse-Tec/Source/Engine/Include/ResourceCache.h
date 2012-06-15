/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourceCache.h
*
*	Description -	Resource cache. Note that a resource pool is used to manage
*						usuable game data(i.e. video textures, mesh, vertex buffers, etc.)
*						while Resource cache manages "raw" data.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	01/30/2010	-	Creation of this file.
*			MrCodeSushi	-	09/12/2010	-	Re-write ResourceCache.
**************************************************************************************/

#ifndef _PSX_RESOURCE_CACHE_H_
#define _PSX_RESOURCE_CACHE_H_

#include "PulseSTD.h"
#include "FileSystem.h"
#include "String.h"
#include "Map.h"
#include "List.h"
#include "SmartPointer.h"
#include "NonCopyable.h"
#include "HashTable.h"

namespace Pulse
{
	PSX_EnumBegin( EHResType )
		DEFAULT,
		SOUND,
	PSX_EnumEnd()

	class ResHandle
	{
	public:

		friend ResourceCache;
		typedef FileSystem::FileHandle FSHFile;

		ResHandle( FSHFile *pHFile, BYTE *pBuffer, ResourceCache *pResCache );

		virtual ~ResHandle( void );

		const CHAR * GetName( void ) const { return m_name.GetBuffer(); }

		const SIZE_T GetSize( void ) const { return m_size; }

		const BYTE * GetBuffer( void ) const { return m_pBuffer; }
		
		BYTE * GetBuffer( void ) { return m_pBuffer; }

	private:

		virtual void Load( FSHFile *pHFile );

		String			m_name;
		SIZE_T			m_size;
		BYTE			*m_pBuffer;
		ResourceCache	*m_pResCache;
	};

	typedef SmartPointer< ResHandle > ResHandlePtr;

	class ResourceCache
	{
	public:

		friend ResHandle;
		typedef FileSystem::FileHandlePtr FileHandlePtr;

		ResourceCache( const UINT cacheSizeMB );
		virtual ~ResourceCache( void );

		ResHandlePtr GetHandle( FileHandlePtr pHFile, EHResType::Type type = EHResType::DEFAULT );

	private:

		ResHandlePtr Find( FileHandlePtr pHFile );
		ResHandlePtr Load( FileHandlePtr pHFile, EHResType::Type type );
		void Update( ResHandlePtr handle );
		BYTE * Allocate( SIZE_T size ); 
		BOOL MakeRoom( SIZE_T size );
		void FreeOneResource( void );
		void MemoryFreed( const SIZE_T size );

	private:

		typedef List< ResHandlePtr > ResList;
		typedef Map< String, ResHandlePtr > ResMap;

		ResList m_LRU;
		ResMap m_resMap;

		SIZE_T m_cacheSize;	// Total memory size
		SIZE_T m_allocated;	// Total memory allocated

	};

	PSX_INLINE void ResourceCache::MemoryFreed( const SIZE_T size )
	{
		m_allocated += size;
	}

}

#endif /*_PSX_RESOURCE_CACHE_H_*/