/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Random.cpp
*
*	Comments	-	See Random.h
*
**************************************************************************************/

#include "../Include/Random.h"
#include <time.h>

/* Tempering parameters */   
#define TEMPERING_MASK_B			0x9d2c5680
#define TEMPERING_MASK_C			0xefc60000
#define TEMPERING_SHIFT_U( _bit )  	( _bit >> 11)
#define TEMPERING_SHIFT_S( _bit )  	( _bit << 7)
#define TEMPERING_SHIFT_T( _bit )  	( _bit << 15)
#define TEMPERING_SHIFT_L( _bit )  	( _bit >> 18)

namespace Pulse
{
	Random::Random( void )
	{
		m_randomSeed = 1;
		m_MTI = RANDOM_N + 1;
	}

	// Retursn a number from 0 to n ( excluding n )
	UINT Random::Rand( UINT n /* = 0xFfffffff */ )
	{
		ULONG y;
		static ULONG mag01[2] = { 0x0, RANDOM_MATRIX_A };

		if ( n == 0 )
			return 0;
		
		// mag01[x] = x * MATRIX_A for x = 0, 1 */
		if ( m_MTI >= RANDOM_N )
		{
			int kk;

			if ( m_MTI == RANDOM_N + 1 ) /* If sgenrand() has not been called, */
				Seed( 4357 );			 /* a default initial seed is used. */

			for ( kk = 0; kk < RANDOM_N - RANDOM_M; ++kk ) 
			{
				y = ( m_MTwist[kk] & RANDOM_UPPER_MASK ) | ( m_MTwist[ kk + 1 ] & RANDOM_LOWER_MASK );
				m_MTwist[kk] = m_MTwist[ kk + RANDOM_M ] ^ (y >> 1) ^ mag01[ y & 0x1 ];
			}

			for ( ; kk < RANDOM_N - 1; ++kk )
			{
				y = ( m_MTwist[kk] & RANDOM_UPPER_MASK ) | ( m_MTwist[ kk + 1 ] & RANDOM_LOWER_MASK );
				m_MTwist[kk] = m_MTwist[ kk + ( RANDOM_M - RANDOM_N ) ] ^ ( y >> 1 ) ^ mag01[ y & 0x1 ];
			}

			y = ( m_MTwist[ RANDOM_N - 1 ] & RANDOM_UPPER_MASK ) | ( m_MTwist[0] & RANDOM_LOWER_MASK);
			m_MTwist[ RANDOM_N - 1 ] = m_MTwist[ RANDOM_M - 1 ] ^ ( y >> 1 ) ^ mag01[y & 0x1 ];

			m_MTI = 0;
		}

		y = m_MTwist[m_MTI++];
		y ^= TEMPERING_SHIFT_U(y);
		y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
		y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
		y ^= TEMPERING_SHIFT_L(y);

		return (y%n);
	}

	FLOAT Random::RandF( void )
	{
		FLOAT r = (FLOAT)Rand( 0xffffffff );
		FLOAT div = (FLOAT)0xffffffff;
		return ( r / div ) + 0.5f;
	}

	void Random::Seed( UINT n )
	{
		/* setting initial seeds to mt[N] using         */
		/* the generator Line 25 of Table 1 in          */
		/* [KNUTH 1981, The Art of Computer Programming */
		/*    Vol. 2 (2nd Ed.), pp102]                  */
		m_MTwist[0]= n & 0xffffffff;
		for ( m_MTI = 1; m_MTI < RANDOM_N; m_MTI++ )
			m_MTwist[m_MTI] = ( 69069 * m_MTwist[ m_MTI-1 ] ) & 0xffffffff;

		m_randomSeed = n;
	}

	void Random::Randomize()
	{
		Seed( (UINT)time( NULL ) );
	}
}

#undef TEMPERING_MASK_B		
#undef TEMPERING_MASK_C		
#undef TEMPERING_SHIFT_U
#undef TEMPERING_SHIFT_S
#undef TEMPERING_SHIFT_T
#undef TEMPERING_SHIFT_L
