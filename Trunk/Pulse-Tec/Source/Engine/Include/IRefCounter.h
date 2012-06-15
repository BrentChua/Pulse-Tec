/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- IRefCounter.h
*
*	Description - Interface class that simulates COM.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/12/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_INTERFACE_REFERENCE_COUNTER
#define _PSX_INTERFACE_REFERENCE_COUNTER

#include "PulseSTD.h"

namespace Pulse
{
	class IRefCounter
	{
	public:

		virtual ~IRefCounter() { };

		virtual INT AddRef() = 0;

		virtual INT Release() = 0;

	};

	class RefCounter : public IRefCounter
	{
	public:

		RefCounter( void ) : m_refCount( 0 ) 
		{
		
		}

		virtual ~RefCounter() 
		{ 
			PSX_Assert( m_refCount == 0, "m_refCount not == 0" );
		}

		virtual INT AddRef() 
		{
			PSX_Assert( m_refCount != PSX_INT_MAX, "m_refCount has reached maximum value." );
			return ++m_refCount; 
		}

		virtual INT Release() 
		{ 
			PSX_Assert( m_refCount != 0, "m_refCount already 0." ); 
			return --m_refCount; 
		}

		virtual INT GetRefCount( void )
		{
			return m_refCount;
		}

	private:

		 INT m_refCount;
	};
}

#endif /* _PSX_INTERFACE_REFERENCE_COUNTER */