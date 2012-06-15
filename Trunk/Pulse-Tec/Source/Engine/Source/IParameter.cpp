/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IParameter.cpp
*
*	Comments	-	See IParameter.h
*
**************************************************************************************/
#include "../Include/IParameter.h"

namespace Pulse
{
	IParameter::IParameter( void )
		: m_changeValue(0)
	{

	}
	
	IParameter * IParameter::CreateCopy( void )
	{
		IParameter *pNewParam = NULL;

		// TODO: finish this implementation.
		switch( GetType() )
		{
		case EParameter::VECTOR:
			break;
		case EParameter::MATRIX:
			break;
		case EParameter::MATRIX_ARRAY:
			break;
		case EParameter::SHADER_RESOURCE:
			break;
		case EParameter::UNORDERED_ACCESS:
			break;
		case EParameter::CONSTANT_BUFFER:
			break;
		case EParameter::SAMPLER:
			break;
		case EParameter::ENTITY:
			break;
		}

		pNewParam->SetData( this );
		return pNewParam;
	}
}