/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderShaderResource.h
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
#include "GraphicsResource.h"

namespace Pulse
{
	class ParameterBinderShaderResource : public IParameterBinder
	{
	public:

		ParameterBinderShaderResource( void );

		virtual ~ParameterBinderShaderResource( void ) { }

		void SetData( GraphicsResource *pData ) { m_pData = pData; }

		virtual void BindValue( void );

		virtual EParameter::Type GetType( void ) { return EParameter::SHADER_RESOURCE; }

	private:

		GraphicsResource *m_pData;

	};
}

#endif /* _PSX_PARAMETER_BINDER_MATRIX_H_ */