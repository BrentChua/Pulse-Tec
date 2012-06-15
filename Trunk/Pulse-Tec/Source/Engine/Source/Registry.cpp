/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Registry.cpp
*
*	Comments	-	See Registry.h
*
**************************************************************************************/

#include "../Include/PulseSTD.h"
#include "../Include/SortedLList.h"
#include "../Include/Registry.h"

namespace Pulse
{
	Registry::ObjectList *Registry::m_pObjectList = NULL;

	IRegistryObject::IRegistryObject( ERegistryPriority::Type primaryPriority, ERegistryPriority::Type secondaryPriority, BOOL bRegisterObject )
		: m_primaryPriority( primaryPriority ), m_secondaryPriority( secondaryPriority )
	{
		if ( bRegisterObject )
			Registry::RegisterObject( this );
	}

	IRegistryObject::~IRegistryObject( void )
	{
		Registry::UnregisterObject( this );
	}
	
	BOOL IRegistryObject::IsLessPriority( const IRegistryObject *plhs, const IRegistryObject *prhs )
	{
		return ( plhs->m_primaryPriority < prhs->m_primaryPriority ) ||
			( plhs->m_primaryPriority == prhs->m_primaryPriority && 
			plhs->m_secondaryPriority < prhs->m_secondaryPriority );
	}

	EErrorCode::Type Registry::ExecuteOperation( ERegistryOperation::Type op )
	{
		if ( m_pObjectList == NULL )
			m_pObjectList = new ObjectList;

		EErrorCode::Type errorCode;
		ObjectList::Iterator iter	 = m_pObjectList->IteratorBegin();
		ObjectList::Iterator iterEnd = m_pObjectList->IteratorEnd();
	
		while ( iter != iterEnd )
		{
			errorCode = (*iter)->DoRegistryOperation( op );

			if ( errorCode != EErrorCode::NONE )
				return errorCode;

			++iter;
		}

		return errorCode;
	}

	void Registry::RegisterObject( IRegistryObject *pObj )
	{
		if ( m_pObjectList == NULL )
			m_pObjectList = new ObjectList;

		m_pObjectList->Insert( pObj );
	}

	void Registry::UnregisterObject( IRegistryObject *pObj )
	{
		PSX_Assert( m_pObjectList, "Registry object list uninitialized." );
		m_pObjectList->Remove( pObj );

		if ( m_pObjectList->GetSize() == 0 )
			PSX_SafeDelete( m_pObjectList );
	}



}