/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterSystem.cpp
*
*	Comments	-	See ParameterSystem.h
*
**************************************************************************************/
#include "../Include/ParameterSystem.h"

namespace Pulse
{
	ParameterSystem::ParameterSystem( void )
	{
	}

	ParameterSystem::~ParameterSystem(	void )
	{
		ClearData();
	}

	IParameter * ParameterSystem::GetParameter( const CHAR *pName )
	{
		return m_renderParams[ pName ];
	}

	ParameterVector * ParameterSystem::GetVector( const CHAR *pName )
	{
		IParameter *pParam = GetParameter( pName );

		if ( pParam )
		{
			PSX_Assert(pParam->GetType() == EParameter::VECTOR, "Parameter should be a vector type." );
			return (ParameterVector*)pParam;
		}

		delete pParam;
		pParam = new ParameterVector;
		m_renderParams[pName] = pParam;
		return (ParameterVector*)pParam;
	}

	ParameterMatrix * ParameterSystem::GetMatrix( const CHAR *pName )
	{
		IParameter *pParam = GetParameter( pName );

		if ( pParam )
		{
			PSX_Assert(pParam->GetType() == EParameter::MATRIX, "Parameter should be a matrix type." );
			return (ParameterMatrix*)pParam;
		}

		delete pParam;
		pParam = new ParameterMatrix;
		m_renderParams[pName] = pParam;
		return (ParameterMatrix*)pParam;
	}

	ParameterMatrixArray * ParameterSystem::GetMatrixArray( const CHAR *pName, UINT numMatrices )
	{
		IParameter *pParam = GetParameter( pName );

		if ( pParam )
		{
			PSX_Assert(pParam->GetType() == EParameter::MATRIX_ARRAY, "Parameter should be a matrix array type." );
			if ( ((ParameterMatrixArray*)pParam)->GetNumMatrices() == numMatrices )
				return (ParameterMatrixArray*)pParam;
		}

		delete pParam;
		pParam = new ParameterMatrixArray(numMatrices);
		pParam->SetName( pName );
		m_renderParams[pName] = pParam;
		return (ParameterMatrixArray*)pParam;
	}

	ParameterShaderResource * ParameterSystem::GetShaderResource( const CHAR *pName )
	{
		IParameter *pParam = GetParameter( pName );

		if ( pParam )
		{
			PSX_Assert(pParam->GetType() == EParameter::SHADER_RESOURCE, "Parameter should be a shader resource type." );
			return (ParameterShaderResource*)pParam;
		}

		delete pParam;
		pParam = new ParameterShaderResource;
		m_renderParams[pName] = pParam;
		return (ParameterShaderResource*)pParam;
	}

	ParameterConstantBuffer * ParameterSystem::GetConstantBuffer( const CHAR *pName )
	{
		IParameter *pParam = GetParameter( pName );

		if ( pParam )
		{
			PSX_Assert( pParam->GetType() == EParameter::CONSTANT_BUFFER, "Parameter should be a constant buffer type." );
			return (ParameterConstantBuffer*)pParam;
		}

		delete pParam;
		pParam = new ParameterConstantBuffer;
		m_renderParams[pName] = pParam;
		return (ParameterConstantBuffer*)pParam;
	}

	void ParameterSystem::ClearData( void )
	{
		RenderParameterList::Iterator iter = m_renderParams.IteratorBegin();
		RenderParameterList::Iterator iterEnd = m_renderParams.IteratorEnd();

		while ( iter != iterEnd )
		{
			PSX_SafeDelete( iter->second );
			++iter;
		}

		m_renderParams.Clear();
	}

}