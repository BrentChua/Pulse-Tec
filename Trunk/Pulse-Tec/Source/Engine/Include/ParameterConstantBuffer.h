/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterConstantBuffer.h
*
*	Description -	Constant buffer render parameter.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/20/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PARAMETER_CONSTANT_BUFFER_H_
#define _PSX_PARAMETER_CONSTANT_BUFFER_H_

#include "PulseSTD.h"
#include "IParameter.h"
#include "ConstantBuffer.h"

namespace Pulse
{
	class ParameterConstantBuffer : public IParameter
	{
	public:

		ParameterConstantBuffer( void ) : m_pConstantBuffer( PSX_NULL ) { }

		ParameterConstantBuffer( ParameterConstantBuffer &rhs );

		virtual ~ParameterConstantBuffer( void );

		virtual void SetData( void *pData );

		virtual void SetData( IParameter *pParam );

		virtual EParameter::Type GetType( void ) const { return EParameter::CONSTANT_BUFFER; }

	private:

		ConstantBuffer *m_pConstantBuffer;
	};
}

#endif /* _PSX_PARAMETER_CONSTANT_BUFFER_H_ */