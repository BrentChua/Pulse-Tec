/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterSystem.h
*
*	Description -	Provides a way to set render parameters to the shaders in the
*						graphics pipeline.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/05/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDER_PARAMETER_SYSTEM_H_
#define _PSX_RENDER_PARAMETER_SYSTEM_H_

#include "PulseSTD.h"
#include "IParameter.h"
#include "HashTable.h"
#include "String.h"
#include "ParameterVector.h"
#include "ParameterMatrix.h"
#include "ParameterMatrixArray.h"
#include "ParameterShaderResource.h"
#include "ParameterConstantBuffer.h"

namespace Pulse
{
	class ParameterSystem
	{
	public:

		ParameterSystem( void );
		
		~ParameterSystem( void );

		//void SetParameter( IParameter *pParam );

		IParameter * GetParameter( const CHAR *pName );

		ParameterVector * GetVector( const CHAR *pName );

		ParameterMatrix * GetMatrix( const CHAR *pName );

		ParameterMatrixArray * GetMatrixArray( const CHAR *pName, UINT numMatrices );

		ParameterShaderResource * GetShaderResource( const CHAR *pName );

		ParameterConstantBuffer * GetConstantBuffer( const CHAR *pName );

		void ClearData( void );

	private:

		typedef HashTable< String, IParameter * > RenderParameterList;
		
		RenderParameterList m_renderParams; 

	};
}

#endif /* _PSX_RENDER_PARAMETER_SYSTEM_H_ */