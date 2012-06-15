/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Math.cpp
*
*	Comments	-	See Math.h
*
**************************************************************************************/

#include "../Include/Math.h"

namespace Pulse
{

	// Define static Math memebers...
	Math::RFPF Math::_Sinf = Math::NonTable_Sinf;
	Math::RFPF Math::_Cosf = Math::NonTable_Cosf;
	Math::RFPF Math::_Tanf = Math::NonTable_Tanf;
	Math::RFPF Math::_ATanf = Math::NonTable_ATanf;

	const FLOAT Math::FLOATEPSILON	= 0.000001f;
	const FLOAT Math::PI			= 3.141592654f;
	const FLOAT Math::PIOVER180		= 3.141592654f / 180.0f;
	const FLOAT Math::_180OVERPI	= 180.0f / 3.141592654f;
	
	Random Math::m_rand;
	FastFloat Math::m_fastFloat;

	BOOL Math::m_bUseSSE	= TRUE;
	BOOL Math::m_bUseTable	= TRUE;

	BOOL Math::Initialize( void )
	{
		m_rand.Randomize();
		m_fastFloat.CreateSinTable();

		return TRUE;
	}

	void Math::Shutdown( void )
	{
		m_fastFloat.CleanTable();
	}

	void Math::SetUsingTable( BOOL bSet )
	{
		m_bUseTable = bSet;

		if ( bSet )
		{
			_Sinf  = Math::NonTable_Sinf;
			_Cosf  = Math::NonTable_Cosf;
			_Tanf  = Math::NonTable_Tanf;
			_ATanf = Math::NonTable_ATanf;
		}
		else
		{
			_Sinf  = Math::Table_Sinf;
			_Cosf  = Math::Table_Cosf;
			_Tanf  = Math::Table_Tanf;
			_ATanf = Math::Table_ATanf;
		}
	}

}