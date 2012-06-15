/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderVector.cpp
*
*	Comments	-	See ParameterBinderVector.h
*
**************************************************************************************/
#include "../Include/ParameterBinderVector.h"
#include "../Include/ParameterVector.h"

namespace Pulse
{
	ParameterBinderVector::ParameterBinderVector( void )
		: m_pData( PSX_NULL )
	{
		
	}

	void ParameterBinderVector::BindValue( void )
	{
		m_pVectorParam->SetVector( m_pData );
	}

}