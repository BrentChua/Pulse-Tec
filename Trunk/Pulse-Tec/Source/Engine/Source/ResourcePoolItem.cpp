/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourcePoolItem.cpp
*
*	Comments	-	See ResourcePoolItem.h
*
**************************************************************************************/

#include "../Include/ResourcePoolItem.h"
#include "../Include/ResourcePool.h"

namespace Pulse
{
	const String * IResourcePoolItem::GetResourceName( void ) const
	{
		if ( m_pResPool )
			return m_pResPool->FindResourceName( m_hRes );

		return NULL;
	}

	void IResourcePoolItem::SetResourceName( const CHAR *pName )
	{
		if ( m_pResPool )
			return m_pResPool->SetResourceName( m_hRes, pName );
	}

	INT IResourcePoolItem::Release( void )
	{
		INT refCount = RefCounter::Release();
		if ( refCount == 0 )
		{
			m_pResPool->DestroyResource( this );
		}

		return refCount;
	}

}