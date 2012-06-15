/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RandomTraverse.h
*
*	Description -	Pseudo-random set traversal class.
*
*	Comments	-	Passes through ALL the members of the set exactly ONCE.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/30/2009		-	File creation
**************************************************************************************/

#ifndef _PSX_RANDOM_TRAVERSAL_H_
#define _PSX_RANDOM_TRAVERSAL_H_

#include "PulseSTD.h"

namespace Pulse
{
	class RandomTraverse
	{
	public:

		RandomTraverse( void );
		RandomTraverse( INT numElements );
		INT GetNext( void );
		BOOL IsDone( void );
		void Restart( void );
		void ResetElements( INT numElements );
		void Clear( void );

	private:

		static INT m_primeArray[];
		INT m_skip;
		INT m_currPos;
		INT m_maxElements;
		INT *m_pCurrPrime;
		INT m_searches;

	};

	PSX_INLINE RandomTraverse::RandomTraverse( void )
		: m_currPos( 0 ), m_searches( 2 ), m_pCurrPrime( m_primeArray )
	{

	}

	PSX_INLINE RandomTraverse::RandomTraverse( INT numElements )
	{
		ResetElements( numElements );
	}

	PSX_INLINE BOOL RandomTraverse::IsDone( void )
	{
		return m_searches == *m_pCurrPrime;
	}

	PSX_INLINE void RandomTraverse::Restart( void )
	{
		m_currPos = 0;
		m_searches = 0;
	}

	PSX_INLINE void RandomTraverse::Clear( void )
	{
		m_searches = 2;
		m_pCurrPrime = m_primeArray;
	}

}

#endif /* _PSX_RANDOM_TRAVERSAL_H_ */