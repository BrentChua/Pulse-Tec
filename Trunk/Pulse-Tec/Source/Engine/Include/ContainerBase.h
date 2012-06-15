/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ContainerBase.h
*
*	Description -	Base classes and interface for the containers.
*
*	Comments	-	
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/1/2009	-	Creation of iterator interface
**************************************************************************************/

#ifndef _PSX_CONTAINER_BASE_H_
#define _PSX_CONTAINER_BASE_H_

#include "PulseSTD.h"

namespace Pulse
{

	// NOTE: Currently not used!!!
	template < typename T >
	class IBaseIterator
	{
	public:

		IBaseIterator( void ) { };

		virtual bool IsValid( void ) = 0;
		virtual void Reset( void ) = 0;
		virtual void Next( void ) = 0;
		virtual T & Get( void ) = 0;

		virtual T & operator * ( void ) = 0;
		virtual T * operator -> ( void ) = 0;

		virtual ~IBaseIterator( void ) { };
	};
};

#endif /* _PSX_CONTAINER_BASE_H_ */