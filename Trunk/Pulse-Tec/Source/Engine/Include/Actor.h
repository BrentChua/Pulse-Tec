/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Actor.h
*
*	Description -	Objects that interacts with the application. 
*
*	Comments	-	Disabled for now. This needs to be reworked.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/05/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_ACTOR_H_
#define _PSX_ACTOR_H_

#include "PulseSTD.h"
#include "SmartPointer.h"
#include "HashTable.h"
#include "Array.h"
#include "String.h"
#include "StringID.h"
#include "XMLParser.h"
#include "Map.h"

namespace Pulse
{

	class Actor
	{
	public:

		Actor( void );

		~Actor( void );

		EErrorCode::Type Initialize( XMLElementPtr *pElem );
		
		void Update( float dt );

		void Destroy( void );

		const CHAR * GetType( void ) { return m_type.GetBuffer(); }

		void SetType( const CHAR *pType ) { m_type = pType; }

		ActorComponentPtr GetComponent( const CHAR *pComponentName );

		ActorComponentPtr GetComponent( ComponentID id );

		void AddComponent( ActorComponentPtr pComponent );

		void RemoveComponent( ComponentID id );

		void RemoveComponent( const CHAR *pComponentName );

	private:

		typedef Array<ActorComponentPtr> ActorComponentList;
		typedef Map< ComponentID, ActorComponentPtr> ActorComponentMap;

		String				m_name;
		String				m_type;
		BOOL				m_bEnabled;
		ActorComponentList	m_components;
		ActorComponentMap	m_componentMap;

	};

}

#endif /* _PSX_ACTOR_H_ */