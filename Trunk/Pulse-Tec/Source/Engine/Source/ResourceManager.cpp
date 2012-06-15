/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourceManager.cpp
*
*	Comments	-	See ResourceManager.h
*
**************************************************************************************/

#include "../Include/ResourceManager.h"

namespace Pulse
{
	IResourcePool * ResourceManager::UnregisterResourcePool( ResourceRegisterCode code )
	{
		ResourcePoolTypeMap::Iterator findIter =  m_resPoolMap.Find( code );

		if ( findIter == m_resPoolMap.IteratorEnd() )
			return NULL;

		IResourcePool *pResPool		= findIter->second;
		ResourcePoolList *pResList	= &m_resPoolList[ code.type ];
		ResourcePoolList::Iterator iter = pResList->IteratorBegin();
		ResourcePoolList::Iterator iterEnd = pResList->IteratorEnd();

		while ( iter != iterEnd )
		{
			if ( (*iter) == pResPool )
			{
				pResList->Remove( iter );
				return pResPool;
			}

			++iter;
		}

		return NULL;
	}

	void ResourceManager::CleanResourceType( EResourceType::Type type )
	{
		ResourcePoolList *pList =  &m_resPoolList[ type ];
		ListIter iter = pList->IteratorBegin();
		ListIter iterEnd = pList->IteratorEnd();

		while ( iter != iterEnd )
		{
			(*iter)->CleanResources();
			++iter;
		}
	}

	void ResourceManager::DestroyResourceType( EResourceType::Type type )
	{
		ResourcePoolList *pList =  &m_resPoolList[ type ];
		ListIter iter = pList->IteratorBegin();
		ListIter iterEnd = pList->IteratorEnd();

		while ( iter != iterEnd )
		{
			(*iter)->DestroyAllResources();
			++iter;
		}
	}

	void ResourceManager::DisableResourceType( EResourceType::Type type )
	{
		ResourcePoolList *pList =  &m_resPoolList[ type ];
		ListIter iter = pList->IteratorBegin();
		ListIter iterEnd = pList->IteratorEnd();

		while ( iter != iterEnd )
		{
			(*iter)->DisableAllResources();
			++iter;
		}
	}

	void ResourceManager::RestoreResrouceType( EResourceType::Type type )
	{
		ResourcePoolList *pList =  &m_resPoolList[ type ];
		ListIter iter = pList->IteratorBegin();
		ListIter iterEnd = pList->IteratorEnd();

		while ( iter != iterEnd )
		{
			(*iter)->RestoreAllResources();
			++iter;
		}
	}

	IResourcePool * ResourceManager::FindResourcePool( ResourceRegisterCode code )
	{
		MapIter findIter = m_resPoolMap.Find( code );
		
		if ( findIter == m_resPoolMap.IteratorEnd() )
			return NULL;
		
		return findIter->second;
	}

	HRes ResourceManager::FindResourceHandle( ResourceRegisterCode code, const String *pName )
	{
		IResourcePool *pPool = FindResourcePool( code );

		if ( pPool == NULL )
			return INVALID_RESOURCE_HANDLE;

		return pPool->FindResourceHandle( pName );
	}

	IResourcePoolItem * ResourceManager::FindResource( ResourceRegisterCode code, const String *pName )
	{
		IResourcePool *pPool = FindResourcePool( code );
		HRes hRes;

		if ( pPool == NULL )
			return NULL;

		hRes = pPool->FindResourceHandle( pName );

		if ( hRes == INVALID_RESOURCE_HANDLE )
			return NULL;

		return pPool->GetResourceGeneric( hRes );
	}

	IResourcePoolItem * ResourceManager::FindResource( ResourceRegisterCode code, HRes hRes )
	{
		IResourcePool *pPool = FindResourcePool( code );

		if ( pPool == NULL || hRes == INVALID_RESOURCE_HANDLE )
			return NULL;

		return pPool->GetResourceGeneric( hRes );
	}
}