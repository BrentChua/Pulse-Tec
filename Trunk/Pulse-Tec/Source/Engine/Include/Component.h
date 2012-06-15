/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Component.h
*
*	Description -	 
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/05/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_ACTOR_COMPONENT_H_
#define _PSX_ACTOR_COMPONENT_H_

#include "PulseSTD.h"
#include "SmartPointer.h"
#include "HashTable.h"
#include "Array.h"
#include "String.h"
#include "StringID.h"
#include "XMLParser.h"

namespace Pulse
{
	class Component
	{
	public:

		Component( void ) { }

		virtual ~Component( void ) { }

		virtual const CHAR * GetName( void ) const = 0;

		virtual ComponentID GetID( void ) const = 0;

		virtual EErrorCode::Type Initialize( XMLElementPtr *pElem );

		virtual void Update( FLOAT dt ) { }
	
	protected:

		BOOL		m_bEnabled;

	};

}

#endif /* _PSX_ACTOR_COMPONENT_H_ */