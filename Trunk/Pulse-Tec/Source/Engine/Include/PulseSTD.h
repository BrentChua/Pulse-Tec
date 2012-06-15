/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PulseSTD.h
*
*	Description -	Contains the base definitions and preprocessor settings.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/18/2009		-	File creation
*			MrCodeSushi	-	10/22/2009		-	Fast asm memsets and strlen function
*			MrCodeSushi	-	05/29/2010		-	Global registry and class forwarding
**************************************************************************************/

#ifndef _PULSE_BASE_H_
#define _PULSE_BASE_H_

#include <memory.h>
#include <assert.h>
#include <cstring>
#include <stdlib.h>

#include "PulseConfig.h"
#include "Platform.h"
#include "System.h"
#include "Log.h"

//#include "PTLBase.h"
#include "Registry.h"
#include "Classes.h"

#include "GlobalDirectory.h"

namespace Pulse
{
	/* Utility Macros and Fast Functions Here */
	#define PSX_CacheLineAligned __declspec(align(64))

	#define PSX_SizeOf( d ) sizeof( d )
	
	#if defined( PSX_UNICODE )
		#define PSX_String( str ) L##str
	#else
		#define PSX_String( str ) str
	#endif

	// ToString and To(Type) macros needs String.h
	#define PSX_ToString( num ) String::ToString( num )
	
	#define PSX_StringToSHORT( str, valRef )	String::StringTo<SHORT>( str, valRef )
	#define PSX_StringToUSHORT( str, valRef )	String::StringTo<USHORT>( str, valRef )
	#define PSX_StringToINT( str, valRef )		String::StringTo<INT>( str, valRef )
	#define PSX_StringToUINT( str, valRef )		String::StringTo<UINT>( str, valRef )
	#define PSX_StringToFLOAT( str, valRef )	String::StringTo<FLOAT>( str, valRef )
	#define PSX_StringToDOUBLE( str, valRef )	String::StringTo<DOUBLE>( str, valRef )
	#define PSX_StringToBOOL( str, valRef )		String::StringTo<BOOL>( str, valRef )
	#define PSX_StringToCString( str, valRef )	String::StringTo<const CHAR*>( str, valRef )

	#define PSX_NEWLINE PSX_String( '\n' )

	#define PSX_IsByteAligned( p, align ) ( ( ( (U32)p ) & ( align - 1 ) ) == 0 )
	#define PSX_AlignMemory( addr, align ) ( ( (U32)addr % align == 0 ) ? addr : ((U8*)addr) + ( align - ( (U32)addr % align ) ) )

	#if PSX_DEBUG // When we develop a more sophisticated debug manager, we should move this in its own place
		#define PSX_Assert( _exp, _msg )									\
			if ( (_exp) ) { }												\
			else															\
			{																\
				/* Report Error here */										\
				assert( (_exp) && PSX_String(" | ") && __LINE__ &&			\
					PSX_String(" | ") && __FILE__ && PSX_String(" | ") &&	\
					PSX_String(_msg) );										\
			}

		#define PSX_AssertNoMsg( _exp ) \
			if ( (_exp) ) { }			\
			else						\
			{							\
				assert( (_exp) );		\
			}		
	#else
		#define PSX_Assert( _exp, _msg )	;
		#define PSX_AssertNoMsg( _exp )		;
	#endif

	#define PSX_Validate( _exp, _msg ) \
	{	\
		BOOL bRet = (_exp);	\
		PSX_Assert( bRet, _msg );	\
	}

	#define PSX_ValidateNoMsg( _exp ) PSX_Validate( _exp, "" )

	#define PSX_AssertUnreachableCode() PSX_Assert( FALSE, "Default constructor is unreachable code." )

	#define PSX_GetFileFuncAndLineString() ( (String(PSX_String("File: \"")) + PSX_FILE) + (String(PSX_String("\" Function: ")) + PSX_FUNCTION) + (String(PSX_String("\" Line: ")) + String::ToString(PSX_LINE)) )
	
	#define PSX_GetFileFuncAndLineCString() PSX_GetFileFuncAndLineString().GetBuffer()

	// NOTE: Deprecated - Using this causes to add more debug
	// code in release mode.. We don't want that performance overhead.
	//#define PSX_Assert( _exp, _msg )						\
	//	if ( _exp ) { }										\
	//	else												\
	//	{													\
	//		throw Pulse::Exception( PSX_String(##_msg##) );	\
	//	}

	#define PSX_SafeDelete( _ptr )			if ( _ptr ) { delete _ptr;					_ptr = NULL; }
	#define PSX_SafeDeleteArray( _ptr )		if ( _ptr ) { delete [] _ptr;				_ptr = NULL; }
	#define PSX_SafeRelease( _ptr )			if ( _ptr ) { _ptr->Release();				_ptr = NULL; }
	#define PSX_SafeDeallocAligned( _ptr )	if ( _ptr ) { PSX_DeallocAligned( _ptr );	_ptr = NULL; }

	// Enum declaration macro
	#define PSX_EnumBegin( enumName )	\
		struct enumName					\
		{								\
			enum Type				\
			{			

	#define PSX_EnumEnd()	\
				EnumCount,	\
			};				\
		};

	#define PSX_EnumEndForceUINT()	\
					ForceUINT = 0xffffffff,	\
				PSX_EnumEnd()

	PSX_FORCE_INLINE VOID PSX_MemSetQuad(VOID *dest, UINT data, U32 count)
	{
		// this function fills or sets unsigned 32-bit aligned memory
		// count is number of quads
		_asm
		{
			mov edi, dest;	edi points to destination memory
			mov ecx, count; number of 32-bit words to move
			mov eax, data;	32-bit data
			rep stosd;		move data
		} 

	}

	PSX_FORCE_INLINE VOID PSX_MemSetWord(VOID *dest, USHORT data, U32 count)
	{
		// this function fills or sets unsigned 16-bit aligned memory
		// count is number of words
		_asm
		{
			mov edi, dest;	edi points to destination memory
			mov ecx, count; number of 16-bit words to move
			mov ax, data;	16-bit data
			rep stosw;		move data
		}
	}

	#define PSX_MemSet( _dest, _val, _size ) memset( _dest, _val, _size )
	#define PSX_ZeroMem( _dest, _size ) ZeroMemory( _dest, _size )

	#ifdef PSX_PLATFORM_WINDOWS
		#if defined( PSX_UNICODE )
			PSX_FORCE_INLINE SIZE_T PSX_StrLen( const CHAR *pStr )
			{
				return wcslen( pStr );
			}
		#else
			PSX_FORCE_INLINE __declspec(align(16)) SIZE_T __stdcall PSX_StrLen(const CHAR* pStr)
			{
				if ( pStr == NULL )
					return 0;

				__declspec(align(16)) int result = 0;
				__asm
				{
					xor eax, eax
						mov edi, [pStr]
					mov ecx, eax
						dec ecx
						repne scasb
						not ecx
						dec ecx
						mov [result], ecx
				}
				return result;
			}
		#endif
		
	#else
		PSX_INLINE UINT PSX_StrLen( const CHAR *pStr )
		{
			PSX_Assert( pStr, "pStr string is NULL." );

			/*
			// By Peter - Fast StrLen version
			// NOTE: This is Unsafe. No check for access violation at end of memory address
			UINT len = 0;
			UINT x;
			for(;;) {
				x = *(unsigned*)s;
				if((x & 0xFF) == 0) return len;
				if((x & 0xFF00) == 0) return len + 1;
				if((x & 0xFF0000) == 0) return len + 2;
				if((x & 0xFF000000) == 0) return len + 3;
				s += 4, len += 4;
			}
			*/

			UINT i = 0;
			while ( pStr )
			{
				++i;
				++pStr;
			};

			return i;
		}
	#endif

	// Size of the string.
	// Important if using UNICODE
	PSX_INLINE SIZE_T PSX_StrSize( const CHAR *pStr )
	{
		return PSX_StrLen( pStr ) * sizeof( CHAR );
	}

	PSX_FORCE_INLINE INT PSX_StrCmp( CONST CHAR *pStr1, CONST CHAR *pStr2 )
	{
		#if defined( PSX_UNICODE )
			return (*pStr1 != *pStr2 ? *pStr1 - *pStr2 : wcscmp( pStr1, pStr2 ) );
		#else
			return (*pStr1 != *pStr2 ? *pStr1 - *pStr2 : strcmp( pStr1, pStr2 ) );
		#endif
	}

	PSX_FORCE_INLINE CHAR const * PSX_StrCat( CHAR *pDest, const CHAR *pSrc )
	{
		#if defined( PSX_UNICODE )
			return wcscat( pDest, pSrc );
		#else
			return strcat( pDest, pSrc );
		#endif
	}

	PSX_FORCE_INLINE CHAR * PSX_StrCpy( CHAR *pDest, CONST CHAR *pSrc, SIZE_T count )
	{
		#if defined( PSX_UNICODE )
				return wcsncpy( pDest, pSrc, count );
		#else
				return strncpy( pDest, pSrc, count );
		#endif
	}

	PSX_FORCE_INLINE const CHAR * PSX_StrChr( CONST CHAR *pStr, INT c )
	{
		#if defined( PSX_UNICODE )
			return wcschr( pStr, c );
		#else
			return strchr( pStr, c );
		#endif
	}

	PSX_FORCE_INLINE CHAR * PSX_StrChr( CHAR *pStr, INT c )
	{
		#if defined( PSX_UNICODE )
			return wcschr( pStr, c );
		#else
			return strchr( pStr, c );
		#endif
	}

	void PSX_StringToLower( const CHAR *pIn, CHAR *pOut );

	void PSX_StringToUpper( const CHAR *pIn, CHAR *pOut );

	// NOTE: Using the default case sensitivity to true is faster as it avoids allocating and copying of new memory.
	BOOL PSX_StringContains( const CHAR *pString, const CHAR *pToken, BOOL bCaseSensitive = TRUE );

	#if defined( PSX_UNICODE )
		#define PSX_StringFormat( _pBuff, _pFormat, ... ) wsprintf( _pBuff, _pFormat, __VA_ARGS__ )
	#else
		#define PSX_StringFormat( _pBuff, _pFormat, ... ) sprintf( _pBuff, _pFormat, __VA_ARGS__ )
	#endif

	// NOTE: MemCopy and MemCompare changes depending on the choosen character set.
	//	Use MemCopyPerByte to always use non-wide (8bit) characters
	#if defined( PSX_UNICODE )
		#define PSX_MemCopy( _dest, _source, _count ) wmemcpy( _dest, _source, _count )
	#else
		#define PSX_MemCopy( _dest, _source, _count ) memcpy( _dest, _source, _count )
	#endif

	#define PSX_MemCopyPerByte( _dest, _source, _count ) memcpy( _dest, _source, _count )

	#if defined( PSX_UNICODE )
		#define PSX_MemCompare( _buf1, _buf2, _size ) wmemcmp( _buf1, _buf2, _size )
	#else
		#define PSX_MemCompare( _buf1, _buf2, _size ) memcmp( _buf1, _buf2, _size )
	#endif /* defined( PSX_UNICODE ) */

	#define PSX_MemComparePerByte( _buf1, _buf2, _size ) memcmp( _buf1, _buf2, _size )

	PSX_INLINE INT PSX_Rand( void )
	{
		return rand();
	}

	PSX_INLINE INT PSX_Rand( INT min, INT max )
	{
		return rand() % ( ( max + 1 ) - min ) + min;
	}

	UINT PSX_SuperFastHash( CONST CHAR *pData, UINT len );

	// Bit manipulation macros
	#define PSX_GetBit(x)				(1<<(x))
	#define PSX_TestBit(flag, bit)		(((flag) & PSX_GetBit((bit))) !=0)
	#define PSX_TestBitAll(flag, set)	(((flag) & (set)) == (set))
	#define PSX_TestBitIfAny(flag, set) (((flag) & (set)) !=0)
	#define PSX_SetBit(flag, bit)		((flag) |= PSX_GetBit((bit)))
	#define PSX_SetAll(flag, set)		((flag) |= (set))
	#define PSX_ClearBit(flag, bit) 	((flag) &= ~PSX_GetBit((bit)))
	#define PSX_ClearAll(flag, set) 	((flag) &= ~(set))

	// Floating Point Macros
	// reinterpret a float as an int32
	#define PSX_FloatToInt(f) (*reinterpret_cast<const INT*>(&(f))) 

	// reinterpret an int32 as a float
	#define PSX_IntToFloat(i) (*reinterpret_cast<const FLOAT*>(&(i))) 

	// return 0 or -1 based on the sign of the float
	#define PSX_FloatSign(f) (PSX_FloatToInt(f)>>31) 

	// extract the 8 bits of exponent as a signed integer
	// by masking out this bits, shifting down by 23,
	// and subtracting the bias value of 127
	#define PSX_FloatGetExponent(f) (((PSX_FloatToInt(f)&0x7fffffff)>>23)-127) 

	// return 0 or -1 based on the sign of the exponent
	#define PSX_FloatGetExponentSign(f) (PSX_FloatGetExponent(f)>>31) 

	// get the 23 bits of mantissa without the implied bit
	#define PSX_FloatGetPureMantissa(f) ((PSX_FloatToInt(f)&0x7fffff)) 

	// get the 23 bits of mantissa with the implied bit replaced
	#define PSX_FloatGetMantissa(f) (PSX_FloatGetPureMantissa(f) | (1<<23)) 

	#define FLOATONEBITS 0x3F800000

	// PSX_FlipSign is a helper Macro to
	// invert the sign of i if flip equals -1, 
	// if flip equals 0, it does nothing
	#define PSX_FlipSign(i, flip) ((i^ flip) - flip)

	UINT PSX_NextPrime( UINT n );

	BOOL PSX_IsPrime( UINT n );

	// TODO: Move these to a math header or something...
	#define PSX_PI 3.141592654f
	#define PSX_DegToRad( _deg ) ( _deg * (PSX_PI / 180.0f) )
	#define PSX_RadToDeg( _rad ) ( _rad * (180.0f / PSX_PI) )

	/////////// TODO: This doesn't prolly belong in here... ///////////
	PSX_INLINE U16 PSX_SwapU16( U16 val )
	{
		return ( val & 0x00FF ) << 8 |
			( val & 0xFF00 ) >> 8;
	}
	PSX_INLINE U32 PSX_SwapU32( U32 val )
	{
		return ( val & 0x000000FF ) << 24 |
			( val & 0x0000FF00 ) << 8 |
			( val & 0x00FF0000 ) >> 8 |
			( val & 0xFF000000 ) >> 24;
	}
	PSX_INLINE F32 PSX_SwapF32( F32 val )
	{
		union U32F32
		{
			U32 m_U32;
			F32 m_F32;
		};

		U32F32 u;
		u.m_F32 = val;

		// endian-swap as an integer
		u.m_U32 = PSX_SwapU32( u.m_U32 );

		return u.m_F32;
	}
	////////////////////////////////////////////////////////////////////

	template < typename T >
	PSX_INLINE INT PSX_HighestBitSet( T input )
	{
		register int result;
		PSX_Assert( input, "Zero is invalid input." );
		PSX_Assert( sizeof( T ), "Only works in 32bit data only." );
		_asm bsr eax, input
		_asm mov result, eax
		return result;
	}

	template<>
	PSX_INLINE INT PSX_HighestBitSet ( BYTE input)
	{
		register int result;
		assert(input); // zero is invalid input!
		_asm mov dl, input // copy into a 32bit reg
		_asm and edx, 0xff // keep only the bits we want
		_asm bsr eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}
	template<>
	PSX_INLINE INT PSX_HighestBitSet ( Char input)
	{
		register int result;
		assert(input); // zero is invalid input!
		_asm mov dl, input // copy into a 32bit reg
		_asm and edx, 0xff // keep only the bits we want
		_asm bsr eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}

	template<>
	PSX_INLINE INT PSX_HighestBitSet ( USHORT input)
	{
		register int result;
		assert(input); // zero is invalid input!
		_asm mov dx, input // copy into a 32bit reg
		_asm and edx, 0xffff // keep only the bits we want
		_asm bsr eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}

	template<>
	PSX_INLINE INT PSX_HighestBitSet ( SHORT input)
	{
		register int result;
		assert(input); // zero is invalid input!
		_asm mov dx, input // copy into a 32bit reg
		_asm and edx, 0xffff // keep only the bits we want
		_asm bsr eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}

	template<>
	PSX_INLINE INT PSX_HighestBitSet ( FLOAT f )
	{
		register int result;
		register UINT input = PSX_FloatToInt(f);
		assert(input); // zero is invalid input!
		_asm bsr eax, input
		_asm mov result, eax
		return result;
	}

	template< typename T >
	PSX_INLINE int PSX_LowestBitSet( T input )
	{
		register INT result;
		assert(input); // zero is invalid input!
		assert(sizeof(T)==4); // 32bit data only!
		_asm bsf eax, input
		_asm mov result, eax
		return result;
	}

	template<>
	PSX_INLINE int PSX_LowestBitSet ( BYTE input)
	{
		register INT result;
		assert(input); // zero is invalid input!
		_asm mov dl, input // copy into a 32bit reg
		_asm and edx, 0xff // keep only the bits we want
		_asm bsf eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}
	template<>
	PSX_INLINE int PSX_LowestBitSet ( Char input )
	{
		register INT result;
		assert(input); // zero is invalid input!
		_asm mov dl, input // copy into a 32bit reg
		_asm and edx, 0xff // keep only the bits we want
		_asm bsf eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}

	template<>
	PSX_INLINE INT PSX_LowestBitSet ( WORD input )
	{
		register INT result;
		assert(input); // zero is invalid input!
		_asm mov dx, input // copy into a 32bit reg
		_asm and edx, 0xffff // keep only the bits we want
		_asm bsf eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}
	template<>
	PSX_INLINE INT PSX_LowestBitSet ( SHORT input)
	{
		register INT result;
		assert(input); // zero is invalid input!
		_asm mov dx, input // copy into a 32bit reg
		_asm and edx, 0xffff // keep only the bits we want
		_asm bsf eax, edx // perform the scan
		_asm mov result, eax
		return result;
	}

	template<>
	PSX_INLINE INT PSX_LowestBitSet ( FLOAT f )
	{
		register INT result;
		register UINT input = PSX_FloatToInt(f);
		assert(input); // zero is invalid input!
		_asm bsf eax, input
		_asm mov result, eax
		return result;
	}

	template < typename T >
	PSX_INLINE BOOL PSX_IsPowerOfTwo( T input )
	{
		return (input > 0 && PSX_HighestBitSet( input ) && PSX_LowestBitSet( input ));
	}

	template <>
	PSX_INLINE BOOL PSX_IsPowerOfTwo( FLOAT input )
	{
		return !PSX_FloatGetPureMantissa(input);
	}

	template < typename T >
	PSX_INLINE T PSX_NearestPowerOfTwo( T input )
	{
		// The least power-of-two value is 1
		if ( input <= 1 ) return 1;

		INT highestBit = PSX_HighestBitSet( input );
		INT roundingTest = input & ( 1 << (highestBit - 1) );
		if ( roundingTest ) ++highestBit;
		return static_cast<T>(1 << highestBit);
	}

	template <>
	PSX_INLINE FLOAT PSX_NearestPowerOfTwo( FLOAT input )
	{
		INT result = PSX_FloatToInt( input );

		// if the value is negative, or less than 1.0f, return 1.0f
		// this mask test for the sign bit and the exponents sign in one step
		if (result & 0xc0000000) return 1.0f;

		// if anything is in the high bit of the mantissa, 
		// use it to add one to the exponent
		result += (result & 0x800000)<<1;

		// trim away the mantissa
		result &= ~((1<<23)-1);

		// convert back to floating-point as we return
		return (PSX_IntToFloat(result));		
	}

	template < typename T >
	PSX_INLINE T PSX_CeilingPowerOfTwo( T input )
	{
		// the least possible power-of-two value is 1
		if (input <= (T)1) return 1;

		INT highestBit = PSX_HighestBitSet(index);
		INT mask = input & ((1<< highestBit)-1); 
		highestBit += mask & 1;
		return static_cast<T>(1<<highestBit);
	}

	template<>
	PSX_INLINE FLOAT PSX_CeilingPowerOfTwo( FLOAT input )
	{
		// convert the value to an int
		INT result = PSX_FloatToInt(input);

		// if the value is negative, or less than 1.0f, return 1.0f
		// this mask test for the sign bit and the exponents sign in one step
		if (result & 0xc0000000) return 1.0f;

		// if anything is in the mantissa, round up
		result += 0x7fffff;

		// trim away the mantissa
		result &= ~((1<<23)-1);

		// convert back to floating-point as we return
		return (PSX_IntToFloat(result));
	}

	template < typename T >
	PSX_INLINE T PSX_FloorPowerOfTwo( T input )
	{
		// the least possible power-of-two value is 1
		if (input <= (T)1) return 1;

		INT highestBit = PSX_HighestBitSet(input);
		return static_cast<T>(1<<highestBit);
	}

	template<>
	PSX_INLINE FLOAT PSX_FloorPowerOfTwo( FLOAT input )
	{
		// convert the value to an int
		INT result = PSX_FloatToInt(input);

		// if the value is negative, or less than 1.0f, return 1.0f
		// this mask test for the sign bit and the exponents sign in one step
		if (result & 0xc0000000) return 1.0f;

		// trim away the mantissa
		result &= ~((1<<23)-1);

		// convert back to floating-point as we return
		return (PSX_IntToFloat(result));
	}

};


#endif /* _PULSE_BASE_H_ */