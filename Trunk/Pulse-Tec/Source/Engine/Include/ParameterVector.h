/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterVector.h
*
*	Description -	A vector render parameter.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/15/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PARAMETER_VECTOR_H_
#define _PSX_PARAMETER_VECTOR_H_

#include "PulseSTD.h"
#include "IParameter.h"
#include "Vector4.h"

namespace Pulse
{
	class ParameterVector : public IParameter
	{
	public:

		ParameterVector( void ) { m_vec = Vector4::ZERO_VECTOR; }

		ParameterVector( ParameterVector &rhs );

		virtual ~ParameterVector( void );

		virtual void SetData( void *pData ) { PSX_MemCopyPerByte( &m_vec, pData, sizeof(Vector4) ); }

		virtual void SetData( IParameter *pParam );

		void SetVector( const Vector4 *pVec ) { m_vec = *pVec; }

		const Vector4 * GetVector( void ) const { return &m_vec; }

		virtual EParameter::Type GetType( void ) const { return EParameter::VECTOR; }

	private:

		Vector4 m_vec;
	};
}

#endif /* _PSX_PARAMETER_VECTOR_H_ */