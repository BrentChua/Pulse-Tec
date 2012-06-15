/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourceCache.cpp
*
*	Comments	-	See ResourceCache.h
*
**************************************************************************************/

#include "../Include/ResourceCache.h"
#include "../Include/System.h"
#include "../Include/FileIO.h"

namespace Pulse
{

	ResHandle::ResHandle( FSHFile *pHFile, BYTE *pBuffer, ResourceCache *pResCache )
		: m_name( pHFile->GetName() ), m_size( pHFile->GetFileSize() ), m_pBuffer( pBuffer ), m_pResCache( pResCache ) 
	{

	}

	ResHandle::~ResHandle( void )
	{
		if ( m_pBuffer )
			delete [] m_pBuffer;

		m_pResCache->MemoryFreed( m_size );
	}

	void ResHandle::Load( FSHFile *pHFile )
	{
		pHFile->ReadEntireFile( m_pBuffer );
	}

	ResourceCache::ResourceCache( const UINT cacheSizeMB )
	{
		m_cacheSize = cacheSizeMB * PSX_MEGABYTE;
		m_allocated = 0;
	}
	
	ResourceCache::~ResourceCache( void )
	{
		while ( !m_LRU.IsEmpty() )
			FreeOneResource();			
	}

	ResHandlePtr ResourceCache::GetHandle( FileHandlePtr pHFile, EHResType::Type type  )
	{
		ResHandlePtr resHandle = Find( pHFile );

		if ( resHandle.IsNull() )
			return Load( pHFile, type );

		Update( resHandle );
		return resHandle;
	}

	ResHandlePtr ResourceCache::Find( FileHandlePtr pHFile )
	{
		ResMap::Iterator findIter = m_resMap.Find( pHFile->GetName() );
	
		if ( findIter != m_resMap.IteratorEnd() )
			return findIter->second;

		return NULL;
	}

	ResHandlePtr ResourceCache::Load( FileHandlePtr pHFile, EHResType::Type type )
	{
		BYTE *pBuffer = Allocate( pHFile->GetFileSize() );

		if ( pBuffer == NULL )
			return NULL; // Out of memory

		// Create a new resource and add it to the lru and map
		ResHandlePtr handle( new ResHandle( pHFile, pBuffer, this ) );
		
		if ( handle.IsNull() )
			return NULL;

		handle->Load( pHFile );

		m_LRU.PushFront( handle );
		m_resMap[ handle->GetName() ] = handle;

		return handle;
	}

	void ResourceCache::Update( ResHandlePtr handle )
	{
		m_LRU.Remove( handle );
		m_LRU.PushFront( handle );
	}

	BYTE * ResourceCache::Allocate( SIZE_T size )
	{
		if ( !MakeRoom( size ) )
			return NULL;

		BYTE *pMem = new BYTE[size];

		if ( pMem )
			m_allocated += size;

		return pMem;
	}

	BOOL ResourceCache::MakeRoom( SIZE_T size )
	{
		if ( size > m_cacheSize )
			return FALSE;

		// Attempt to free up some memory to accomodate new one
		while ( size > ( m_cacheSize - m_allocated ) )
		{
			// The cache is empty, and there's still not enough room.
			if ( m_LRU.IsEmpty() )
				return FALSE;

			FreeOneResource();
		}

		return TRUE;		
	}

	void ResourceCache::FreeOneResource( void )
	{
		// Free one resource - Remove from the lru and resMap
		ResList::Iterator gonner = m_LRU.IteratorBeginLast();
		ResHandlePtr handle = *gonner;
		m_LRU.PopBack();
		m_resMap.Remove( handle->GetName() );
	}


}