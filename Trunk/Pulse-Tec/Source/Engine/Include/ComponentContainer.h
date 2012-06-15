/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ComponentContainer.h
*
*	Description -	Container for storing components
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/15/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_COMPONENT_CONTAINER_H_
#define _PSX_COMPONENT_CONTAINER_H_

#include "PulseSTD.h"
#include "Component.h"
#include "Map.h"

namespace Pulse
{
	class ComponentContainer
	{
	public:

		ComponentContainer( void );

		~ComponentContainer( void );

		void Update( FLOAT dt );

		Component * GetComponent( const CHAR *pComponentName );

		Component * GetComponent( ComponentID id );

		void RemoveComponent( const CHAR *pComponentName );

		void RemoveComponent( ComponentID id );

	private:

	};
}

#endif /* _PSX_COMPONENT_CONTAINER_H_ */
