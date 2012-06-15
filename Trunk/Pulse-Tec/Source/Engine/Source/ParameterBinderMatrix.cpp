/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderMatrix.cpp
*
*	Comments	-	See ParameterBinderMatrix.h
*
**************************************************************************************/
#include "../Include/ParameterBinderMatrix.h"
#include "../Include/ParameterMatrix.h"

namespace Pulse
{
	void ParameterBinderMatrix::BindValue( void )
	{
		m_pMatrixParam->SetMatrix( m_pData );
	}

}