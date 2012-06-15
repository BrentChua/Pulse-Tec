/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterVector.cpp
*
*	Comments	-	See ParameterVector.h
*
**************************************************************************************/
#include "../Include/ParameterVector.h"

namespace Pulse
{
	ParameterVector::ParameterVector( ParameterVector &rhs )
		: IParameter( rhs )
	{
		m_vec = rhs.m_vec;
	}

	ParameterVector::~ParameterVector( void )
	{

	}

	void ParameterVector::SetData( IParameter *pParam )
	{
		if ( pParam )
		{
			if ( pParam->GetType() == EParameter::VECTOR )
			{
				m_vec = ((ParameterVector*)pParam)->m_vec;
			}
		}
	}
}