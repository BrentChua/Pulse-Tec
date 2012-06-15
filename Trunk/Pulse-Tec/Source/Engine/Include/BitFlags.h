/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	- BitFlags.h
*
*	Description - Class handling low level bit manipulations.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/11/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_BIT_FLAGS_H_
#define _PSX_BIT_FLAGS_H_

#include "PulseSTD.h"

namespace Pulse
{
	template < typename T >
	class BitFlags
	{
	public:

		BitFlags( void );
		BitFlags( T _bitFlags );

		BOOL operator == ( const BitFlags &rhs ) const;
		BOOL operator != ( const BitFlags &rhs ) const;

		void Set( T _bitFlags );
		void Clear( void );
		void SetFlags( T _bitFlags );
		void ClearFlags( T _bitFlags );
		void SetBit( INT bit );
		void ClearBit( INT bit );
		void SetBit( INT bit, BOOL bOn );

		BOOL IsAllCleared( void ) const;
		BOOL TestBit( INT bit ) const;
		BOOL TestFlags( T _bitFlags ) const;
		BOOL TestAny( T _bitFlags ) const;

		SIZE_T GetTotalBits( void ) const;
		SIZE_T GetTotalSet( void ) const;

	public:

		T bitFlags;

	};

	typedef BitFlags<U8>	BitFlagsU8;
	typedef BitFlags<U16>	BitFlagsU16;
	typedef BitFlags<U32>	BitFlagsU32;

	template < typename T >
	PSX_INLINE BitFlags<T>::BitFlags( void )
		: bitFlags( 0 )
	{

	}

	template < typename T >
	PSX_INLINE BitFlags<T>::BitFlags( T _bitFlags )
		: bitFlags( _bitFlags )
	{

	}

	template < typename T >
	PSX_INLINE BOOL BitFlags<T>::operator == ( const BitFlags &rhs ) const
	{
		return bitFlags == rhs.bitFlags;
	}

	template < typename T >
	PSX_INLINE BOOL BitFlags<T>::operator != ( const BitFlags &rhs ) const
	{
		return bitFlags != rhs.bitFlags;
	}

	template < typename T >
	PSX_INLINE void BitFlags<T>::Set( T _bitFlags )
	{
		bitFlags = _bitFlags;
	}

	template < typename T >
	PSX_INLINE void BitFlags<T>::Clear( void )
	{
		bitFlags = 0;
	}

	template < typename T >
	PSX_INLINE void BitFlags<T>::SetFlags( T _bitFlags )
	{
		bitFlags |= _bitFlags;
	}

	template < typename T >
	PSX_INLINE void BitFlags<T>::ClearFlags( T _bitFlags )
	{
		bitFlags &= ~_bitFlags;
	}

	template < typename T >
	PSX_INLINE void BitFlags<T>::SetBit( INT bit )
	{
		bitFlags |= (1<<bit);
	}

	template < typename T >
	PSX_INLINE void BitFlags<T>::ClearBit( INT bit )
	{
		bitFlags &= ~(1<<bit);
	}

	template < typename T >
	PSX_INLINE void BitFlags<T>::SetBit( INT bit, BOOL bOn )
	{
		if ( bOn )
			SetBit( bit );
		else
			ClearBit( bit );
	}

	template < typename T >
	PSX_INLINE BOOL BitFlags<T>::IsAllCleared( void ) const
	{
		return bitFlags == static_cast<T>(0);
	}

	template < typename T >
	PSX_INLINE BOOL BitFlags<T>::TestBit( INT bit ) const
	{
		return (bitFlags & (1 << bit)) != 0;
	}

	template < typename T >
	PSX_INLINE BOOL BitFlags<T>::TestFlags( T _bitFlags ) const
	{
		return (bitFlags & _bitFlags) == _bitFlags;
	}

	template < typename T >
	PSX_INLINE BOOL BitFlags<T>::TestAny( T _bitFlags ) const
	{
		return bitFlags & _bitFlags;
	}

	template < typename T >
	PSX_INLINE SIZE_T BitFlags<T>::GetTotalBits( void ) const
	{
		// Multiply by 8.
		return sizeof(T) << 3;
	}

	template < typename T >
	PSX_INLINE SIZE_T BitFlags<T>::GetTotalSet( void ) const
	{
		SIZE_T totalSet = 0;
		SIZE_T bitCount = GetTotalBits();
		T testValue		= bitFlags;

		for ( SIZE_T i = bitCount; i; --i )
		{
			totalSet += testValue & 1;
			testValue >>= 1;
		}

		return totalSet;
	}

}

#endif /* _PSX_BIT_FLAGS_H_ */