/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Vector4.h
*
*	Description -	4D vector class.
*
*	Comments	-	Memoization
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/24/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_FAST_FLOAT_H_
#define _PSX_FAST_FLOAT_H_

#include "PulseSTD.h"

namespace Pulse
{
	class FastFloat
	{
	public:

		static void CreateSinTable( SIZE_T sinTableSize = 512 );
		static void CleanTable( void );

		static INT FloatToInt( const FLOAT val );

		static FLOAT Sinf( const FLOAT &phi );
		static FLOAT Cosf( const FLOAT &phi );
		static FLOAT Absf( const FLOAT &val );

		static SIZE_T GetSinTableSize( void );

	private:

		FastFloat( void ) { }
		~FastFloat( void ) { }

		friend class Math;

		typedef union
		{
			INT i;
			FLOAT f;
		} I32F32;

		static SIZE_T	m_sinTableSize;
		static FLOAT	*m_pSinTable;
		static FLOAT	m_modifier;
		static FLOAT	m_quarter;
		static INT		m_mask;

		static CONST FLOAT FTOIBIAS;
	};

	PSX_INLINE void FastFloat::CleanTable( void )
	{
		PSX_SafeDeleteArray( m_pSinTable );
		m_sinTableSize = 0;
	}

	PSX_INLINE INT FastFloat::FloatToInt( const FLOAT val )
	{
		I32F32 n;
		I32F32 bias;

		if( val > 0.0f )
			bias.i = (23 + 127) << 23;
		else
			bias.i = ((23 + 127) << 23) + (1 << 22);

		n.f = val;

		n.f += bias.f;
		n.i -= bias.i;

		return n.i;
	}

	PSX_INLINE FLOAT FastFloat::Sinf( const FLOAT &phi )
	{
		INT i;
		I32F32 temp;

		temp.f = phi * m_modifier + FTOIBIAS;
		i = temp.i & m_mask;

		return m_pSinTable[i];
	}
	
	PSX_INLINE FLOAT FastFloat::Cosf( const FLOAT &phi )
	{
		INT i;
		I32F32 temp;

		temp.f = phi * m_modifier + FTOIBIAS + m_quarter;
		i = temp.i & m_mask;

		return m_pSinTable[i];
	}
	
	PSX_INLINE FLOAT FastFloat::Absf( const FLOAT &val )
	{
		I32F32 temp;
		temp.f = val;
		temp.i &=  0x7fffffff;

		return temp.f;
	}

	PSX_INLINE SIZE_T FastFloat::GetSinTableSize( void )
	{
		return m_sinTableSize;
	}
	
}

#endif /* _PSX_FAST_FLOAT_H_ */