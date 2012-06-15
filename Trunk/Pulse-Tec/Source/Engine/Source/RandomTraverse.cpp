/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RandomTraverse.cpp
*
*	Comments	-	See RandomTraverse.h
*
**************************************************************************************/

#include "../Include/RandomTraverse.h"
#include "../Include/Math.h"

namespace Pulse
{
	INT RandomTraverse::m_primeArray[] =
	{
		2, 3, 5, 7, 
		11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 
		53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 
		101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 
		151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 
		211, 223, 227, 229, 233, 239, 241,
		251, 257, 263, 269, 271, 277, 281, 283, 293, 
		307, 311, 313, 317, 331, 337, 347, 349, 
		353, 359, 367, 373, 379, 383, 389, 397, 
		401, 409, 419, 421, 431, 433, 439, 443, 449, 
		457, 461, 463, 467, 479, 487, 491, 499, 
		503, 509, 521, 523, 541, 547, 
		557, 563, 569, 571, 577, 587, 593, 599, 
		601, 607, 613, 617, 619, 631, 641, 643, 647, 
		653, 659, 661, 673, 677, 683, 691, 
		701, 709, 719, 727, 733, 739, 743, 
		751, 757, 761, 769, 773, 787, 797, 
		809, 811, 821, 823, 827, 829, 839, 
		853, 857, 859, 863, 877, 881, 883, 887, 
		907, 911, 919, 929, 937, 941, 947, 
		953, 967, 971, 977, 983, 991, 997, 

		// after 1000, for space efficiency reasons, 
		// we choose to include fewer prime numbers the bigger the number get.

		1009, 1051, 1103, 1151, 1201, 1259, 1301, 1361, 1409, 1451, 
		1511, 1553, 1601, 1657, 1709, 1753, 1801, 1861, 1901, 1951, 
		2003, 2053, 2111, 2113, 2153, 2203, 2251, 
		2309, 2351, 2411, 2459, 2503, 2551, 2557, 
		2609, 2657, 2707, 2753, 2767, 2801, 2851, 2903, 2953,
		3001, 3061, 3109, 3163, 3203, 3251, 3301, 3359, 3407, 3457, 
		3511, 3557, 3607, 3659, 3701, 3761, 3803, 3851, 3907, 3967, 
		4001, 4051, 4111, 4153, 4201, 4253, 4327, 4357, 4409, 4451, 
		4507, 4561, 4603, 4651, 4703, 4751, 4801, 4861, 4903, 4951, 

		// begin to skip even more primes

		5003, 5101, 5209, 5303, 5407, 5501, 5623, 5701, 5801, 5903, 
		6007, 6101, 6211, 6301, 6421, 6521, 6607, 6701, 6803, 6907, 
		7001, 7103, 7207, 7307, 7411, 7507, 7603, 7703, 7817, 7901, 
		8009, 8101, 8209, 8311, 8419, 8501, 8609, 8707, 8803, 8923, 
		9001, 9103, 9203, 9311, 9403, 9511, 9601, 9719, 9803, 9901, 

		// and even more
		10007, 10501, 11003, 11503, 12007, 12503, 13001, 13513, 14009, 14503, 
		15013, 15511, 16033, 16519, 17011, 17509, 18013, 18503, 19001, 19501, 
		20011, 20507, 21001, 21503, 22003, 22501, 23003, 23509, 24001, 24509

		// if you need more - go get them yourself!!!!

		// Create a bigger array of prime numbers by using this web site: 
		// http://www.rsok.com/~jrm/printprimes.html

	};

	void RandomTraverse::ResetElements( INT numElements )
	{
		PSX_Assert( numElements, "numElements needs to be greater than 0." );

		m_maxElements = numElements;

		INT a = Math::Rand(13) + 1;
		INT b = Math::Rand(7) + 1;
		INT c = Math::Rand(5) + 1;

		// Quadratic equation
		m_skip = ( a * m_maxElements * m_maxElements ) + ( b * m_maxElements ) + c;
		m_skip &= ~0xc0000000;	// Kepp skip from becoming too large.

		Restart();

		m_pCurrPrime = m_primeArray;
		INT s = sizeof(m_primeArray) / sizeof( m_primeArray[0] );

		// Make sure our highest prime number is bigger than our numElements.
		// If not then go back to the website to get some more yummy primes!
		PSX_Assert( m_primeArray[s-1] > m_maxElements, "Not enough prime numbers. Get more bigger prime numbers!" );

		while ( *m_pCurrPrime < m_maxElements )
			++m_pCurrPrime;

		INT test = m_skip % *m_pCurrPrime;

		if ( !test )
			++m_skip;
	}

	INT RandomTraverse::GetNext( void )
	{
		if ( IsDone() )
			return -1;

		BOOL done = FALSE;
		INT nextMember = m_currPos;

		while ( !done )
		{
			nextMember = nextMember + m_skip;
			nextMember %= *m_pCurrPrime;
			m_searches++;

			if ( nextMember < m_maxElements )
			{
				m_currPos = nextMember;
				done = TRUE;
			}
		}

		return m_currPos;
	}
}