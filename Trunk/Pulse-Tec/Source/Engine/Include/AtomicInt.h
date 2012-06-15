/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- AtomicInt.h
*
*	Description - 
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/21/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_ATOMIC_INT_H_
#define _PSX_ATOMIC_INT_H_

#include <intrin.h>
#include "PulseSTD.h"

// Interlock intrinsics declaration

// To declare an interlocked function for use as an intrinsic, First, the function must be declared with the 
// leading underscore. Second, the new function must appear in a #pragma intrinsic statement. For convenience,
// the intrinsic versions of the functions may be #define'd to appear in the source code without the leading 
// underscore.
extern "C" long  __cdecl _InterlockedCompareExchange(long volatile* a_vpDest, long a_Exchange, long a_Comp);
extern "C" long  __cdecl _InterlockedExchange(long volatile* a_vpTarget, long a_Value);
extern "C" long  __cdecl _InterlockedExchangeAdd(long volatile* a_vpAddend, long a_Value);
extern "C" long  __cdecl _InterlockedIncrement(long volatile* a_vpAddend);
extern "C" long  __cdecl _InterlockedDecrement(long volatile* a_vpAddend);

// Specifies that calls to functions specified in the pragma's argument list are intrinsic. The compiler 
// generates intrinsic functions as inline code, not as function calls.
#pragma intrinsic (_InterlockedCompareExchange)
#pragma intrinsic (_InterlockedExchange)
#pragma intrinsic (_InterlockedExchangeAdd)
#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)

namespace Pulse
{

	class AtomicInt
	{
	public:

		AtomicInt( INT val );

		INT operator ++ ( INT );

		INT operator -- ( INT );

		INT operator ++ ( void );

		INT operator -- ( void );

		void operator = ( INT a_iExchange );

		INT CompareExchange( INT a_iComperand, INT a_iExchange );

		INT GetValue( void ) const;

		operator INT( void ) const;

	private:

		// Atomically modifed value. Declared mutable such that function that emulate atomic reads 
		// reads can be implemented using standard _InterlockedXXX functions 
		mutable LONG m_val;

	};

	PSX_FORCE_INLINE AtomicInt::AtomicInt( INT val ) 
		: m_val(val)
	{
	}

	PSX_FORCE_INLINE INT AtomicInt::operator ++ ( INT )
	{
		return _InterlockedExchangeAdd( &m_val, 1 );
	}

	PSX_FORCE_INLINE INT AtomicInt::operator -- ( INT )
	{
		return _InterlockedExchangeAdd(&m_val, -1);
	}

	PSX_FORCE_INLINE INT AtomicInt::operator ++ ( void )
	{
		return _InterlockedIncrement( &m_val );
	}

	PSX_FORCE_INLINE INT AtomicInt::operator -- ( void )
	{
		return _InterlockedDecrement( &m_val );
	}

	PSX_FORCE_INLINE void AtomicInt::operator = ( INT a_iExchange )
	{
		_InterlockedExchange( &m_val, a_iExchange );
	}

	PSX_FORCE_INLINE INT AtomicInt::CompareExchange( INT a_iComperand, INT a_iExchange )
	{
		return _InterlockedCompareExchange( &m_val, a_iExchange, a_iComperand );
	}

	PSX_FORCE_INLINE INT AtomicInt::GetValue( void ) const
	{
		return _InterlockedExchangeAdd( &m_val, 0 );
	}

	PSX_FORCE_INLINE AtomicInt::operator INT( void ) const
	{
		return GetValue();
	}

}

#endif /* _PSX_ATOMIC_INT_H_ */