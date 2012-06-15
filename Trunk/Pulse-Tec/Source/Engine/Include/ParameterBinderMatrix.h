/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderMatrix.h
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
	class ParameterBinderMatrix : public IParameterBinder
	{
	public:

		ParameterBinderMatrix( void ) { }

		virtual ~ParameterBinderMatrix( void ) { }

		void SetData( Matrix4x4 *pData ) { m_pData = pData; }

		virtual void BindValue( void );

		virtual EParameter::Type GetType( void ) { return EParameter::MATRIX; }

	private:

		Matrix4x4 *m_pData;

	};
}

#endif /* _PSX_PARAMETER_BINDER_MATRIX_H_ */