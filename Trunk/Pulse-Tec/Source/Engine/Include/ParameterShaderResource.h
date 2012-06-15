/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterShaderResource.h
*
*	Description -	.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/30/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PARAMETER_SHADER_RESOURCE_H_
#define _PSX_PARAMETER_SHADER_RESOURCE_H_

#include "PulseSTD.h"
#include "IParameter.h"

namespace Pulse
{
	class ParameterShaderResource : public IParameter
	{
	public:

		ParameterShaderResource( void ) { }

		ParameterShaderResource( ParameterShaderResource &rhs );

		virtual ~ParameterShaderResource( void ) { }

		virtual void SetData( void *pData );

		virtual void SetData( IParameter *pParam );

		void SetResource( GraphicsResource *pResource ) { m_pResource = pResource; }

		virtual EParameter::Type GetType( void ) const { return EParameter::SHADER_RESOURCE; }

	private:

		GraphicsResource *m_pResource;

	};
}

#endif /* _PSX_PARAMETER_SHADER_RESOURCE_H_ */