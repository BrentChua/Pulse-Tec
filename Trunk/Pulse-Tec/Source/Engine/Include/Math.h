/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Math.h
*
*	Description -	Encapsulates common math functions.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/30/2009		-	File creation
**************************************************************************************/

#ifndef _PSX_MATH_H_
#define _PSX_MATH_H_

#include "PulseSTD.h"
#include "Random.h"
#include "FastFloat.h"
#include <math.h>

namespace Pulse
{
	class Math
	{
	public:

		static BOOL Initialize( void );
		static void Shutdown( void );

		static void SetSSE( BOOL bSet );
		static BOOL IsUsingSSE( void );
		static void SetUsingTable( BOOL bSet );
		static BOOL IsUsingTable( void );

		static UINT GetRandSeed( void );
		static void RandSeed( UINT seed );
		static void RandomizeRand( void );
		static UINT Rand( UINT mod = 0xffffffff );
		static INT Rand( UINT min, UINT max );
		static FLOAT RandF( void );

		static FLOAT Sqrtf( FLOAT _val );

		static FLOAT RadToDeg( FLOAT angle );
		static FLOAT DegToRad( FLOAT angle );

		static INT CompareFloat( FLOAT lhs, FLOAT rhs );

		static Vector2 Normalize( const Vector2 &vec );
		static Vector3 Normalize( const Vector3 &vec );
		static Vector4 Normalize( const Vector4 &vec );

		static FLOAT Sinf( FLOAT angle );
		static FLOAT Cosf( FLOAT angle );
		static FLOAT Tanf( FLOAT angle );
		static FLOAT ATanf( FLOAT angle );

		template < typename T >
		static T Abs( const T &v );
		template <>
		static INT Abs( const INT &v );

		template < typename T >
		static T Clamp( T value, T low, T high );

		template < typename T >
		static const T & Minimum( const T &a, const T &b );
		template < typename T >
		static const T & Maximum( const T &a, const T &b );

		template < typename T >
		static BOOL IsPowerOfTwo( T val );
		template < typename T >
		static T NearestPowerOfTwo( T val );
		template < typename T >
		static T CeilingPowerOfTwo( T val );
		template < typename T >
		static T FloorPowerOfTwo( T val );
		
		static FLOAT Ceiling( FLOAT val );
		static DOUBLE Ceiling( DOUBLE val );
		static LONGDOUBLE Ceiling( LONGDOUBLE val );

		static FLOAT Floor( FLOAT val );
		static DOUBLE Floor( DOUBLE val );
		static LONGDOUBLE Floor( LONGDOUBLE val );

	private:

		static FLOAT NonTable_Sinf( FLOAT angle );
		static FLOAT NonTable_Cosf( FLOAT angle );
		static FLOAT NonTable_Tanf( FLOAT angle );
		static FLOAT NonTable_ATanf( FLOAT angle );

		static FLOAT Table_Sinf( FLOAT angle );
		static FLOAT Table_Cosf( FLOAT angle );
		static FLOAT Table_Tanf( FLOAT angle );
		static FLOAT Table_ATanf( FLOAT angle );	

		typedef FLOAT (*RFPF)(FLOAT);

		static RFPF _Sinf;
		static RFPF _Cosf;
		static RFPF _Tanf;
		static RFPF _ATanf;

	public:

		static const FLOAT FLOATEPSILON;
		static const FLOAT PI;
		static const FLOAT PIOVER180;
		static const FLOAT _180OVERPI;

	private:
		
		static Random		m_rand;
		static FastFloat	m_fastFloat;
		
		static BOOL m_bUseSSE;
		static BOOL m_bUseTable;

	};

	PSX_INLINE void Math::SetSSE( BOOL bSet )
	{
		m_bUseSSE = bSet;
	}

	PSX_INLINE BOOL Math::IsUsingSSE( void )
	{
		return m_bUseSSE;
	}

	PSX_INLINE BOOL Math::IsUsingTable( void )
	{
		return m_bUseTable;
	}

	PSX_INLINE UINT Math::Rand( UINT mod /* = 0xffffffff  */ )
	{
		return m_rand.Rand( mod );
	}

	PSX_INLINE INT Math::Rand( UINT min, UINT max )
	{
		return m_rand.Rand() % ( (max + 1) - min ) + min;
	}

	PSX_INLINE FLOAT Math::RandF( void )
	{
		return m_rand.RandF();
	}

	PSX_INLINE FLOAT Math::Sqrtf( FLOAT _val )
	{
		PSX_Assert( _val > 0.0f, "Invalid _val value." );

		__asm
		{
			fld	_val;	// s0 = num
			fsqrt;		// s0 = sqrtf(_num)
		}
	}

	PSX_INLINE FLOAT Math::RadToDeg( FLOAT angle )
	{
		return angle * _180OVERPI;
	}
	
	PSX_INLINE FLOAT Math::DegToRad( FLOAT angle )
	{
		return angle * PIOVER180;
	}

	PSX_INLINE INT Math::CompareFloat( FLOAT lhs, FLOAT rhs )
	{
		FLOAT delta = lhs - rhs;
		if ( fabs( delta ) <= FLOATEPSILON )
			return 0;
		else
			return (delta < 0.0f) ? -1 : 1;
	}

	PSX_FORCE_INLINE FLOAT Math::Sinf( FLOAT angle )
	{
		return (*_Sinf)(angle);
	}

	PSX_FORCE_INLINE FLOAT Math::Cosf( FLOAT angle )
	{
		return (*_Cosf)(angle);
	}

	PSX_FORCE_INLINE FLOAT Math::Tanf( FLOAT angle )
	{
		return (*_Tanf)(angle);
	}

	PSX_FORCE_INLINE FLOAT Math::ATanf( FLOAT angle )
	{
		return (*ATanf)(angle);
	}

	template < typename T >
	PSX_FORCE_INLINE const T & Math::Minimum( const T &a, const T &b )
	{
		return PSX_Min( a, b );
	}

	template < typename T >
	PSX_FORCE_INLINE const T & Math::Maximum( const T &a, const T &b )
	{
		return PSX_Max( a, b );
	}

	template < typename T >
	PSX_INLINE T Math::Abs( const T& v )
	{
		return (v < 0 ? -v : v);
	}

	template < typename T >
	PSX_INLINE T Math::Clamp( T value, T low, T high )
	{
		if ( value < low )
			return low;

		if ( value > high )
			return high;

		return value;
	}

	template <>
	PSX_INLINE INT Math::Abs( const INT &v )
	{
		INT const mask = v >> ( sizeof(INT) * PSX_CHAR_BIT - 1 );
		return ((v + mask) ^ mask);
	}

	template < typename T >
	PSX_FORCE_INLINE BOOL Math::IsPowerOfTwo( T val )
	{
		return PSX_IsPowerOfTwo( val );
	}

	template < typename T >
	PSX_FORCE_INLINE T Math::NearestPowerOfTwo( T val )
	{
		return PSX_NearestPowerOfTwo( val );
	}

	template < typename T >
	PSX_FORCE_INLINE T Math::CeilingPowerOfTwo( T val )
	{
		return PSX_CeilingPowerOfTwo( val );
	}

	template < typename T >
	PSX_FORCE_INLINE T Math::FloorPowerOfTwo( T val )
	{
		return PSX_FloorPowerOfTwo( val );
	}

	PSX_FORCE_INLINE FLOAT Math::Ceiling( FLOAT val )
	{
		return ceilf( val );
	}

	PSX_FORCE_INLINE DOUBLE Math::Ceiling( DOUBLE val )
	{
		return ceil( val );
	}

	PSX_FORCE_INLINE LONGDOUBLE Math::Ceiling( LONGDOUBLE val )
	{
		return ceill( val );
	}

	PSX_FORCE_INLINE FLOAT Math::Floor( FLOAT val )
	{
		return floorf( val );
	}

	PSX_FORCE_INLINE DOUBLE Math::Floor( DOUBLE val )
	{
		return floor( val );
	}

	PSX_FORCE_INLINE LONGDOUBLE Math::Floor( LONGDOUBLE val )
	{
		return floorl( val );
	}

	PSX_INLINE FLOAT Math::NonTable_Sinf( FLOAT angle )
	{
		return sinf( angle );
	}

	PSX_INLINE FLOAT Math::NonTable_Cosf( FLOAT angle )
	{
		return cosf( angle );
	}

	PSX_INLINE FLOAT Math::NonTable_Tanf( FLOAT angle )
	{
		return tanf( angle );
	}

	PSX_INLINE FLOAT Math::NonTable_ATanf( FLOAT angle )
	{
		return atanf( angle );
	}

	PSX_INLINE FLOAT Math::Table_Sinf( FLOAT angle )
	{
		return m_fastFloat.Sinf(angle);
	}

	PSX_INLINE FLOAT Math::Table_Cosf( FLOAT angle )
	{
		return m_fastFloat.Cosf( angle );
	}

	PSX_INLINE FLOAT Math::Table_Tanf( FLOAT angle )
	{
		return tanf( angle );
	}

	PSX_INLINE FLOAT Math::Table_ATanf( FLOAT angle )
	{
		// Not memoi-ized.
		return atanf( angle );
	}


}

#endif /* _PSX_MATH_H_ */