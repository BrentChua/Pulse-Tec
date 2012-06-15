/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	FastFloat.cpp
*
*	Comments	-	See FastFloat.h
*
**************************************************************************************/

#include "../Include/FastFloat.h"
#include "../Include/Math.h"
#include <math.h>

namespace Pulse
{
	SIZE_T FastFloat::m_sinTableSize	= 0;
	FLOAT *FastFloat::m_pSinTable		= NULL;
	FLOAT FastFloat::m_modifier			= 0.0f;
	FLOAT FastFloat::m_quarter			= 0.0f;
	INT FastFloat::m_mask				= 0;
	const FLOAT FastFloat::FTOIBIAS		= 12582912.0f;

	void FastFloat::CreateSinTable( SIZE_T sinTableSize /*= 512*/ )
	{
		CleanTable();

		m_sinTableSize = sinTableSize;
		m_mask = sinTableSize - 1;
		m_modifier = static_cast<FLOAT>( sinTableSize / (2.0f * Math::PI) );
		m_quarter = static_cast<FLOAT>( sinTableSize * 0.25f );

		m_pSinTable = new FLOAT[ sinTableSize ];
		PSX_Assert( m_pSinTable, "Failed to allocated memory." );	
		for ( SIZE_T i = 0; i < sinTableSize; ++i )
			m_pSinTable[i] = (FLOAT)sin((DOUBLE)i * 2.0 * (DOUBLE)Math::PI / (DOUBLE)m_sinTableSize );
	}


}