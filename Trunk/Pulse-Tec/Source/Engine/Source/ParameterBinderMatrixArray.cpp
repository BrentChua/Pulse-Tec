/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderMatrixArray.cpp
*
*	Comments	-	See ParameterBinderMatrixArray.h
*
**************************************************************************************/
#include "../Include/ParameterBinderMatrixArray.h"
#include "../Include/ParameterMatrixArray.h"

namespace Pulse
{
	ParameterBinderMatrixArray::ParameterBinderMatrixArray( void )
		: m_numMatrices( 0 ), m_pData( PSX_NULL )
	{
		
	}

	void ParameterBinderMatrixArray::SetParameter( IParameter *pParam )
	{
		if ( pParam->GetType() == GetType() && 
			((ParameterMatrixArray*)pParam)->GetNumMatrices() == GetNumMatrices() )
		{
			m_pParam = pParam;
		}
		else
		{
			PSX_PushWarning( "Invalid Parameter type or number of matrices." );
		}
	}

	void ParameterBinderMatrixArray::BindValue( void )
	{
		m_pMatrixArrayParam->SetMatrixArray( m_pData );
	}

}