/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Actor.cpp
*
*	Comments	-	See Actor.h
*
**************************************************************************************/

#if 0 
#include "../Include/Actor.h"
#include "../Include/Component.h"

namespace Pulse
{

	Actor::Actor( void )
		: m_bEnabled( TRUE )
	{

	}

	Actor::~Actor( void )
	{

	}

	EErrorCode::Type Actor::Initialize( XMLElementPtr *pElem )
	{
		return EErrorCode::NONE;
	}

	void Actor::Update( FLOAT dt )
	{
		for ( SIZE_T i = 0; i < m_components.GetSize(); ++i )
			m_components[i]->Update( dt );
	}

	void Actor::Destroy( void )
	{
		m_components.Clear();
	}

	ActorComponentPtr Actor::GetComponent( const CHAR *pComponentName )
	{
		// Check if already added
		for ( SIZE_T i = 0; i < m_components.GetSize(); ++i )
		{
			if ( PSX_StrCmp( m_components[i]->GetName(), pComponentName ) )
				return m_components[i];
		}

		return ActorComponentPtr();
	}

	ActorComponentPtr Actor::GetComponent( ComponentID id )
	{
		ActorComponentMap::Iterator iter = m_componentMap.Find( id );

		if ( iter == m_componentMap.IteratorEnd() )
			return iter->second;

		return ActorComponentPtr();
	}

	void Actor::AddComponent( ActorComponentPtr pComponent )
	{
		// Check if already added
		if ( m_componentMap.Find(pComponent->GetID()) != m_componentMap.IteratorEnd() )
			return;
		
		m_components.PushBack( pComponent );
		m_componentMap[pComponent->GetID()] = pComponent;
	}

	void Actor::RemoveComponent( ComponentID id )
	{
		// Check if already added
		ActorComponentMap::Iterator iter = m_componentMap.Find(id);

		if ( iter == m_componentMap.IteratorEnd() )
			return;

		for ( SIZE_T i = 0; i < m_components.GetSize(); ++i )
		{
			if ( PSX_StrCmp( m_components[i]->GetName(), iter->second->GetName() ) )
			{
				m_components.Remove(i);
				return;
			}
		}

		m_componentMap.Remove( iter );
	}

	void Actor::RemoveComponent( const CHAR *pComponentName )
	{
		for ( SIZE_T i = 0; i < m_components.GetSize(); ++i )
		{
			if ( PSX_StrCmp( m_components[i]->GetName(), pComponentName ) )
			{
				m_componentMap.Remove( m_components[i]->GetID() );
				m_components.Remove(i);
				break;
			}
		}
	}
}

#endif