/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IParameter.h
*
*	Description -	A single parameter object used for the rendering parameter system.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/15/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDER_PARAMETER_H_
#define _PSX_RENDER_PARAMETER_H_

#include "PulseSTD.h"
#include "StringID.h"

namespace Pulse
{
	PSX_EnumBegin( EParameter )
		VECTOR,
		MATRIX,
		MATRIX_ARRAY,
		SHADER_RESOURCE,
		UNORDERED_ACCESS,
		CONSTANT_BUFFER,
		SAMPLER,
		ENTITY,
		PSX_EnumEnd()

	class IParameter
	{
	public:

		IParameter( void );

		IParameter( IParameter &rhs ) { m_name = rhs.m_name; }

		virtual ~IParameter() { }

		IParameter * CreateCopy( void );

		void SetName( const CHAR *pName ) { m_name.SetString( pName ); }

		const CHAR * GetName( void ) const { m_name.GetString(); }

		const UINT GetID( void ) const { return m_name.GetID(); }

		virtual void SetData( void *pData ) = 0;

		// Derived classes should be careful about this function. Make sure
		//	pParam is the same type as the data being written to.
		virtual void SetData( IParameter *pParam ) = 0;

		virtual EParameter::Type GetType( void ) const = 0;

		// Convenience function to let subclasses be referred by their parent
		//	to update their internal values.
		//virtual void UpdateValue( IParameter* pParameter ) = 0;

	protected:

		StringID m_name;
		UINT	m_changeValue;
	};

}

#endif /* _PSX_RENDER_PARAMETER_H_ */