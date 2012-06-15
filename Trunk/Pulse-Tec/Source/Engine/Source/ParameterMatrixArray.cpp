/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterMatrixArray.cpp
*
*	Comments	-	See ParameterMatrix.h
*
**************************************************************************************/
#include "../Include/ParameterMatrixArray.h"

namespace Pulse
{
	void ParameterMatrixArray::SetMatrixArray( const Matrix4x4 *pMatrices )
	{ 
		for ( SIZE_T i = 0; i < m_numMatrices; ++i ) 
			m_pMatrices[i] = pMatrices[i]; 
	}

	void ParameterMatrixArray::SetData( IParameter *pParam )
	{
		if ( pParam )
		{
			if ( pParam->GetType() == EParameter::MATRIX_ARRAY )
			{
				for ( SIZE_T i = 0; i < m_numMatrices; ++i ) 
					m_pMatrices[i] = ((ParameterMatrixArray*)pParam)->m_pMatrices[i]; 
			}
		}
	}

}