/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderMatrixArray.h
*
*	Description -	.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/10/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PARAMETER_BINDER_MATRIX_H_
#define _PSX_PARAMETER_BINDER_MATRIX_H_

#include "Pulse.h"
#include "GraphicsTypes.h"
#include "IParameterBinder.h"
#include "Matrix4x4.h"

namespace Pulse
{
	class ParameterBinderMatrixArray : public IParameterBinder
	{
	public:

		ParameterBinderMatrixArray( void );

		virtual ~ParameterBinderMatrixArray( void ) { }

		void SetData( SIZE_T numMatrices, Matrix4x4 *pData ) { m_numMatrices = numMatrices; m_pData = pData; }

		SIZE_T GetNumMatrices( void ) { return m_numMatrices; }

		virtual void SetParameter( IParameter *pParam );

		virtual void BindValue( void );

		virtual EParameter::Type GetType( void ) { return EParameter::MATRIX_ARRAY; }

	private:

		SIZE_T		m_numMatrices;
		Matrix4x4	*m_pData;

	};
}

#endif /* _PSX_PARAMETER_BINDER_MATRIX_H_ */