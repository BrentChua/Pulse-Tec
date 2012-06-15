/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinderVector.h
*
*	Description -	.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/10/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PARAMETER_BINDER_VECTOR_H_
#define _PSX_PARAMETER_BINDER_VECTOR_H_

#include "Pulse.h"
#include "GraphicsTypes.h"
#include "IParameterBinder.h"
#include "Vector4.h"

namespace Pulse
{
	class ParameterBinderVector : public IParameterBinder
	{
	public:

		ParameterBinderVector( void );

		virtual ~ParameterBinderVector( void ) { }

		void SetData( Vector4 *pData ) { m_pData = pData; }

		virtual void BindValue( void );

		virtual EParameter::Type GetType( void ) { return EParameter::VECTOR; }

	private:

		Vector4 *m_pData;

	};
}

#endif /* _PSX_PARAMETER_BINDER_VECTOR_H_ */