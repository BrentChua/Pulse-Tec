/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterMatrix.h
*
*	Description -	A matrix render parameter.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/15/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PARAMETER_MATRIX_H_
#define _PSX_PARAMETER_MATRIX_H_

#include "PulseSTD.h"
#include "IParameter.h"
#include "Matrix4x4.h"

namespace Pulse
{
	class ParameterMatrix : public IParameter
	{
	public:

		ParameterMatrix( void ) { m_mat.BuildIdentity(); }

		ParameterMatrix( ParameterMatrix &rhs );

		virtual ~ParameterMatrix( void ) { }

		virtual void SetData( void *pData ) { PSX_MemCopyPerByte( &m_mat, pData, sizeof(Matrix4x4) ); }

		virtual void SetData( IParameter *pParam );

		void SetMatrix( const Matrix4x4 *pMat ) { m_mat = *pMat; }

		const Matrix4x4 * GetMatrix( void ) const { return &m_mat; }

		virtual EParameter::Type GetType( void ) const { return EParameter::MATRIX; }

	private:

		Matrix4x4 m_mat;
	};

}

#endif /* _PSX_PARAMETER_MATRIX_H_ */