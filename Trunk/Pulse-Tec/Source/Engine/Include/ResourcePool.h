/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourcePool.h
*
*	Description -	This is an extension of the StoragePool to handle resources.
*
*	Comments	-	TODO: Add save resource method
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/09/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_RESOURCE_POOL_H_
#define _PSX_RESOURCE_POOL_H_

#include "StoragePool.h"
#include "ResourcePoolItem.h"
#include "String.h"
#include "Map.h"

namespace Pulse
{

	class IResourcePool
	{
	public:
		virtual ~IResourcePool( void ) { }

		virtual void Create( SIZE_T growSize ) = 0;
		virtual void Destroy( void ) = 0;

		virtual void CleanResources( void ) = 0;

		virtual void DisableAllResources( void ) = 0;
		virtual void DestroyAllResources( void ) = 0;
		virtual void RestoreAllResources( void ) = 0;

		virtual BOOL IsInitialize( void ) = 0;

		virtual IResourcePoolItem * CreateResourceGeneric( const String *pResName, BaseResDesc *pDesc ) = 0;
		//virtual IResourcePoolItem * CreateResourceFromFileGeneric( const String *pFilename, BaseResDesc *pDesc ) = 0;

		virtual void DestroyResource( IResourcePoolItem *pRes ) = 0;

		virtual IResourcePoolItem * GetResourceGeneric( HRes hRes ) = 0;
		virtual IResourcePoolItem * FindResourceGeneric( const String *pResName ) = 0;

		virtual HRes			FindResourceHandle( const String *pResName ) = 0;
		virtual const String *	FindResourceName( HRes hRes ) const = 0;

		virtual void SetResourceName( HRes hRes, const CHAR *pNewName ) = 0;

		virtual void RegisterPool( ResourceRegisterCode type ) = 0;
		virtual void UnregisterPool( void ) = 0;
	};

	template < typename T >
	class ResourcePool : public IResourcePool
	{
	public:

		ResourcePool( void );
		ResourcePool( ResourceRegisterCode code );
		ResourcePool( SIZE_T growSize );
		ResourcePool( ResourceRegisterCode code, SIZE_T growSize );
		virtual ~ResourcePool( void );

		// NOTE: growSize is changed to the nearest power of two
		virtual void Create( SIZE_T growSize );
		virtual void Destroy( void );

		virtual void CleanResources( void ); // Delete items no longer referenced
		virtual void RestoreAllResources( void );
		virtual void DisableAllResources( void );
		virtual void DestroyAllResources( void );

		virtual BOOL IsInitialize( void );

		T * CreateResource( const String *pResName, BaseResDesc *pDesc );
		T * CreateResource( const CHAR *pResName, BaseResDesc *pDesc );
		//T * CreateResourceFromFile( const String *pFilename );
		//T * CreateResourceFromFile( const CHAR *pFilename );

		T * GetResource( HRes hRes );
		T * FindResource( const String *pResName ); // NULL if not found
		T * FindResource( const CHAR *pResName );

		virtual HRes			FindResourceHandle( const String *pResName );
		virtual const String *	FindResourceName( HRes hRes ) const;

		virtual void SetResourceName( HRes hRes, const CHAR *pNewName );

		virtual void RegisterPool( ResourceRegisterCode type );
		virtual void UnregisterPool( void );

	private:

		virtual IResourcePoolItem * CreateResourceGeneric( const String *pResName, BaseResDesc *pDesc );
		//virtual IResourcePoolItem * CreateResourceFromFileGeneric( const String *pFilename, BaseResDesc *pDesc );

		virtual void DestroyResource( IResourcePoolItem *pRes ); // ResoucePoolItems calls their Release method.

		virtual IResourcePoolItem * GetResourceGeneric( HRes hRes );
		virtual IResourcePoolItem * FindResourceGeneric( const String *pResName );

		// Static data callbacks
		static void CallbackClean( IStoragePool *pStoragePool, HRes hRes, void *pRes );
		static void CallbackRestore( IStoragePool *pStoragePool, HRes hRes, void *pRes );
		static void CallbackDisable( IStoragePool *pStoragePool, HRes hRes, void *pRes );
		static void CallbackDestroy( IStoragePool *pStoragePool, HRes hRes, void *pRes );

		HRes InternalCreateResource( const String *pResName, BaseResDesc *pDesc );
		void InternalDestroyResource( HRes hRes );
		IResourcePoolItem * InternalGetResource( HRes hRes );
		
		typedef Map< String, HRes > ResourceNameMap;
		typedef StoragePool< T > DataPool;

		ResourceRegisterCode	m_regCode;
		ResourceNameMap			m_resNameMap;
		DataPool				m_dataPool;
	};

	template < typename T >
	PSX_INLINE ResourcePool<T>::ResourcePool( void )
		: m_regCode( ResourceRegisterCode::INVALID_CODE )
	{
		
	}

	template < typename T >
	PSX_INLINE ResourcePool<T>::ResourcePool( ResourceRegisterCode code )
		: m_regCode( code )
	{

	}

	template < typename T >
	PSX_INLINE ResourcePool<T>::ResourcePool( SIZE_T growSize )
		: m_regCode( ResourceRegisterCode::INVALID_CODE )
	{
		Create( growSize );
	}

	template < typename T >
	PSX_INLINE ResourcePool<T>::ResourcePool( ResourceRegisterCode code, SIZE_T growSize )
		: m_regCode( code )
	{
		Create( growSize );
	}

	template < typename T >
	PSX_INLINE ResourcePool<T>::~ResourcePool( void )
	{
		Destroy();
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::Create( SIZE_T growSize )
	{
		m_dataPool.Create( growSize );
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::Destroy( void )
	{
		m_dataPool.Destroy();
		m_resNameMap.Clear();
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::CleanResources( void )
	{
		m_dataPool.ForEach( CallbackClean );
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::RestoreAllResources( void )
	{
		m_dataPool.ForEach( CallbackRestore );
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::DisableAllResources( void )
	{
		m_dataPool.ForEach( CallbackDisable );
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::DestroyAllResources( void )
	{
		m_dataPool.ForEach( CallbackDestroy );
	}

	template < typename T >
	PSX_INLINE BOOL ResourcePool<T>::IsInitialize( void )
	{
		return m_dataPool.IsInitialized();
	}
	
	template < typename T >
	PSX_INLINE IResourcePoolItem * ResourcePool<T>::CreateResourceGeneric( const String *pResName, BaseResDesc *pDesc )
	{
		HRes hRes = InternalCreateResource( pResName, pDesc );

		if ( hRes == INVALID_RESOURCE_HANDLE )
			return NULL;
		
		IResourcePoolItem *pRes = GetResourceGeneric( hRes );
		
		// Check if resource has been successfully created
		if ( pRes->IsCreated() == FALSE )
		{
			while ( pRes->Release() );
			
			return PSX_NULL;
		}

		pRes->SetResourcePool( this );
		pRes->SetResourceHandle( hRes );
		pRes->SetResourceCode( m_regCode );

		return pRes;
	}

	//template < typename T >
	//PSX_INLINE IResourcePoolItem * ResourcePool<T>::CreateResourceFromFileGeneric( const String *pFilename, BaseResDesc *pDesc )
	//{
	//	IResourcePoolItem *pRes = FindResourceGeneric( pFilename );

	//	if ( pRes )
	//		return pRes;

	//	pRes = CreateResourceGeneric( pFilename, pDesc );

	//	if ( !pRes )
	//		return NULL;

	//	if ( pRes->LoadResource() )
	//	{
	//		pRes->AddRef();
	//		return pRes;
	//	}

	//	pRes->Release();
	//	return NULL;
	//}

	template < typename T >
	PSX_FORCE_INLINE T * ResourcePool<T>::CreateResource( const String *pResName, BaseResDesc *pDesc )
	{
		return (T*)CreateResourceGeneric( pResName, pDesc );
	}

	template < typename T >
	PSX_FORCE_INLINE T * ResourcePool<T>::CreateResource( const CHAR *pResName, BaseResDesc *pDesc )
	{
		return (T*)CreateResource( &String(pResName), pDesc );
	}

	//template < typename T >
	//PSX_FORCE_INLINE T * ResourcePool<T>::CreateResourceFromFile( const String *pFilename )
	//{
	//	return (T*)CreateResourceFromFileGeneric( pFilename );
	//}

	//template < typename T >
	//PSX_FORCE_INLINE T * ResourcePool<T>::CreateResourceFromFile( const CHAR *pFilename )
	//{
	//	return (T*)CreateResourceFromFile( &String(pFilename) );
	//}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::DestroyResource( IResourcePoolItem *pRes )
	{
		if ( pRes )
		{
			INT refCount = pRes->GetRefCount();

			PSX_Assert( refCount == 0, "Destroying resource item with non-zero ref count." );

			if ( refCount <= 0 )
				InternalDestroyResource( pRes->GetResourceHandle() );
		}
	}

	template < typename T >
	PSX_INLINE IResourcePoolItem * ResourcePool<T>::GetResourceGeneric( HRes hRes )
	{
		IResourcePoolItem *pResItem = InternalGetResource( hRes );
		
		if ( pResItem )
			pResItem->AddRef();
		
		return pResItem;
	}

	template < typename T >
	PSX_INLINE IResourcePoolItem * ResourcePool<T>::FindResourceGeneric( const String *pResName )
	{
		HRes hRes = INVALID_RESOURCE_HANDLE;

		ResourceNameMap::Iterator findIter = m_resNameMap.Find( *pResName );

		if ( findIter == m_resNameMap.IteratorEnd() )
			return NULL;

		hRes = findIter->second;

		if ( hRes != INVALID_RESOURCE_HANDLE )
			return GetResourceGeneric( hRes );

		return NULL;
	}

	template < typename T >
	PSX_FORCE_INLINE T * ResourcePool<T>::GetResource( HRes hRes )
	{
		return (T*)GetResourceGeneric( hRes );
	}

	template < typename T >
	PSX_FORCE_INLINE T * ResourcePool<T>::FindResource( const String *pResName )
	{
		return (T*)FindResourceGeneric( pResName );
	}

	template < typename T >
	PSX_FORCE_INLINE T * ResourcePool<T>::FindResource( const CHAR *pResName )
	{
		return (T*)FindResourceGeneric( &String(pResName) );
	}

	template < typename T >
	PSX_INLINE HRes ResourcePool<T>::FindResourceHandle( const String *pResName )
	{
		HRes hRes(INVALID_RESOURCE_HANDLE);
		ResourceNameMap::Iterator iter = m_resNameMap.Find( *pResName );
		if ( iter != m_resNameMap.IteratorEnd() )
			hRes = iter->second;

		return hRes;
	}
	
	template < typename T >
	PSX_INLINE const String * ResourcePool<T>::FindResourceName( HRes hRes ) const
	{
		ResourceNameMap::Iterator iter = m_resNameMap.IteratorBegin();
		while ( iter != m_resNameMap.IteratorEnd() )
		{
			if ( hRes == iter->second )
				return &iter->first;
			++iter;
		}

		return NULL;
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::SetResourceName( HRes hRes, const CHAR *pNewName )
	{
		ResourceNameMap::Iterator iter = m_resNameMap.IteratorBegin();
		while ( iter != m_resNameMap.IteratorEnd() )
		{
			if ( hRes == iter->second )
			{
				m_resNameMap.Remove( iter );
				break;
			}
			++iter;
		}

		m_resNameMap[ String(pNewName) ] = hRes;
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::RegisterPool( ResourceRegisterCode type )
	{
		m_regCode = type;
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::UnregisterPool( void )
	{
		m_regCode.SetInvalid();
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::CallbackClean( IStoragePool *pStoragePool, HRes hRes, void *pRes )
	{
		if ( ((IResourcePoolItem*)pRes)->GetRefCount() == 0 )
			pStoragePool->ReleaseMember( hRes );
	}

	template < typename T >
	PSX_INLINE void ResourcePool<T>::CallbackRestore( IStoragePool *pStoragePool, HRes hRes, void *pRes )
	{
		((IResourcePoolItem*)pRes)->RestoreResource();
	}
	
	template < typename T >
	PSX_INLINE void ResourcePool<T>::CallbackDisable( IStoragePool *pStoragePool, HRes hRes, void *pRes )
	{
		((IResourcePoolItem*)pRes)->DisableResource();
	}
	
	template < typename T >
	PSX_INLINE void ResourcePool<T>::CallbackDestroy( IStoragePool *pStoragePool, HRes hRes, void *pRes )
	{
		((IResourcePoolItem*)pRes)->DestroyResource();
	}

	template < typename T >
	PSX_INLINE HRes ResourcePool<T>::InternalCreateResource( const String *pResName, BaseResDesc *pDesc )
	{
		ResourceNameMap::Iterator findIter = m_resNameMap.Find( *pResName );

		if ( findIter != m_resNameMap.IteratorEnd() )
		{
			PSX_PushLog3( "Resource name %s already exists.", pResName->GetBuffer() );
			return findIter->second;
		}

		HRes hRes = m_dataPool.NextHandle();

		m_resNameMap[*pResName] = hRes;

		IResourcePoolItem *pRes = (IResourcePoolItem*)m_dataPool.GetPtr( hRes );
		new (pRes) T();
		pRes->CreateResource( pDesc );

		return hRes;
	}
	
	template < typename T >
	PSX_INLINE void ResourcePool<T>::InternalDestroyResource( HRes hRes )
	{
		// Inform resource to destroy itself
		IResourcePoolItem *pRes = (IResourcePoolItem*)m_dataPool.GetPtr( hRes );
		pRes->DestroyResource();
		((T*)pRes)->~T();

		// Remove name from the map
		ResourceNameMap::Iterator iter = m_resNameMap.IteratorBegin();

		while ( iter != m_resNameMap.IteratorEnd() )
		{
			if ( iter->second == hRes )
			{
				m_resNameMap.Remove( iter );
				break;
			}
			++iter;
		};

		m_dataPool.ReleaseMember( hRes );
	}

	template < typename T >
	PSX_INLINE IResourcePoolItem * ResourcePool<T>::InternalGetResource( HRes hRes )
	{
		return reinterpret_cast<IResourcePoolItem*>(m_dataPool.GetPtrGeneric( hRes ));
	}
}

#endif /* _PSX_RESOURCE_POOL_H_ */