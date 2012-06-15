/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinder.h
*
*	Description -	Binds values to parameter objects given by ParameterSystem.
*
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/10/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PARAMETER_BINDER_H_
#define _PSX_PARAMETER_BINDER_H_

#include "PulseSTD.h"
#include "IParameter.h"

namespace Pulse
{
	class IParameterBinder
	{
	public:

		IParameterBinder( void );

		IParameterBinder( IParameter *pParam );

		virtual ~IParameterBinder( void  );

		IParameter * GetParameter( void ) { return m_pParam; }

		virtual void SetParameter( IParameter *pParam );

		virtual void BindValue( void ) = 0;

		virtual EParameter::Type GetType( void ) = 0;

	protected:

		union
		{
			IParameter				*m_pParam;

			// For easy usage of IParameter in derived classes.
			ParameterMatrix			*m_pMatrixParam;
			ParameterMatrixArray	*m_pMatrixArrayParam;
			ParameterShaderResource *m_pShaderResourceParam;
			ParameterVector			*m_pVectorParam;
		};

	};
}

#endif /* _PSX_PARAMETER_BINDER_H_ */