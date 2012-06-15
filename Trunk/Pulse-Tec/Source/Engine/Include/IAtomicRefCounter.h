/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- IAtomicRefCounter.h
*
*	Description - Atomic Interface class that simulates COM.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/22/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_ATOMIC_REF_COUNTER_INTERFACE_H_
#define _PSX_ATOMIC_REF_COUNTER_INTERFACE_H_

#include "PulseSTD.h"
#include "AtomicInt.h"

namespace Pulse
{

	class IAtomicRefCounter
	{
	public:

		// Get reference count.
		INT GetRefCount()
		{
			return m_refCount;
		}

		// Constructor. Initialize count to 0 by default
		IAtomicRefCounter( INT initialCount = 0 ) 
			: m_refCount(a_iInitialCount)
		{
			PSX_Assert( initialCount >= 0, "Count should never be negative" );
		}

		// Constructor. Initialize new count to 0
		IAtomicRefCounter( IAtomicRefCounter &counter )
			: m_refCount(0)
		{
		}

		// Destructor.
		~IAtomicRefCounter()
		{
		}

		// Increment reference count. Returns the new ref value
		INT AddRef( void ) const
		{
			UINT uiRes = ++m_refCount;
			PSX_Assert(uiRes > 0, "Count should never be negative");
			return uiRes;
		}

		// Decrement reference count. Returns the new ref value
		INT Release( void ) const
		{
			UINT uiRes = --m_refCount;
			PSX_Assert(uiRes >= 0, "Count should never be negative");
			return uiRes;
		}

		// Copy. Initialize new count to 0
		void operator = ( IAtomicRefCounter &counter )
		{
			m_refCount = 0;
		}

	private:

		// Internal counter
		mutable AtomicInt m_refCount;
	};

}

#endif /* _PSX_ATOMIC_REF_COUNTER_INTERFACE_H_ */