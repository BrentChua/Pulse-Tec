/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterMatrixArray.h
*
*	Description -	A matrix array render parameter.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/15/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDER_PARAMETER_MATRIX_ARRAY_H_
#define _PSX_RENDER_PARAMETER_MATRIX_ARRAY_H_

#include "PulseSTD.h"
#include "IParameter.h"
#include "Matrix4x4.h"

namespace Pulse
{
	class ParameterMatrixArray : public IParameter
	{
	public:

		ParameterMatrixArray( SIZE_T numMatrices ) { m_numMatrices = numMatrices; m_pMatrices = new Matrix4x4[numMatrices]; }

		virtual ~ParameterMatrixArray( void ) { delete [] m_pMatrices; }

		virtual void SetData( void *pData ) { PSX_MemCopyPerByte( &m_pMatrices, pData, sizeof(Matrix4x4) * m_numMatrices ); }

		virtual void SetData( IParameter *pParam );

		void SetMatrixArray( const Matrix4x4 *pMatrices );

		const Matrix4x4 * GetMatrixArray( void ) const { return m_pMatrices; }

		const SIZE_T GetNumMatrices( void ) const { return m_numMatrices; }

		virtual EParameter::Type GetType( void ) const { return EParameter::MATRIX_ARRAY; }

	private:

		SIZE_T		m_numMatrices;
		Matrix4x4	*m_pMatrices;
	};
}

#endif /* _PSX_RENDER_PARAMETER_MATRIX_ARRAY_H_ */