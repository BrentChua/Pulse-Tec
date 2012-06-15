/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Random.h
*
*	Description -	Random Number Generator.
*
*	Comments	-	Based on Mersenne Twister pseudorandom number generator.
*					Originally developed by Takuji Nishimura and Makoto Matsumoto.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/29/2009		-	File creation
**************************************************************************************/

#ifndef _PSX_RANDOM_NUMBER_GENERATOR_H_
#define _PSX_RANDOM_NUMBER_GENERATOR_H_

#include "PulseSTD.h"

namespace Pulse
{

	class Random
	{
	public:

		Random( void );

		/********************************************************************************
		*	Function Name :	Rand
		*	Description	  :	Returns a random number from 0 - (n-1).
		*	In			  :	n	-	Max random number to generate.
		*	Out			  :	
		*	Return		  :	UINT - Random number from 0- (n-1).
		*	Example		  : 
		********************************************************************************/
		UINT Rand( UINT n = 0xFfffffff );

		/********************************************************************************
		*	Function Name :	RandF
		*	Description	  :	Returns a floating point random number from 0.0f - 1.0f
		*	In			  :	
		*	Out			  :	
		*	Return		  :	FLOAT - Random floating point number from 0.0f 0 1.0f
		*	Example		  : 
		********************************************************************************/
		FLOAT RandF( void );

		void Seed( UINT n );
		UINT GetSeed( void );

		void Randomize( void );

	private:

		// Period parameters
		enum {	RANDOM_N			= 624,
				RANDOM_M			= 397,
				RANDOM_MATRIX_A		= 0x9908b0df,   /* constant vector a			*/
				RANDOM_UPPER_MASK	= 0x80000000,	/* most significant w-r bits	*/
				RANDOM_LOWER_MASK	= 0x7fffffff	/* least significant r bits		*/
		};

		UINT	m_randomSeed;
		ULONG	m_MTwist[RANDOM_N];
		INT		m_MTI;

	};

	PSX_INLINE UINT Random::GetSeed( void )
	{
		return m_randomSeed;
	}

}

#endif /* _PSX_RANDOM_NUMBER_GENERATOR_H_ */