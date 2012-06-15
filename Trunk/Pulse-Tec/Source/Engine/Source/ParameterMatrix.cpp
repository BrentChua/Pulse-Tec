/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterMatrix.cpp
*
*	Comments	-	See ParameterMatrix.h
*
**************************************************************************************/
#include "../Include/ParameterMatrix.h"

namespace Pulse
{
	ParameterMatrix::ParameterMatrix( ParameterMatrix &rhs )
		: IParameter( rhs )
	{
		m_mat = rhs.m_mat;
	}

	void ParameterMatrix::SetData( IParameter *pParam )
	{
		if ( pParam )
		{
			if ( pParam->GetType() == EParameter::MATRIX )
			{
				m_mat = ((ParameterMatrix*)pParam)->m_mat;
			}
		}
	}
}