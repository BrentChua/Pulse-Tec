/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ParameterBinder.cpp
*
*	Comments	-	See ParameterBinder.h
*
**************************************************************************************/
#include "../Include/IParameterBinder.h"

namespace Pulse
{
	IParameterBinder::IParameterBinder( void )
		: m_pParam( PSX_NULL )
	{

	}

	IParameterBinder::IParameterBinder( IParameter *pParam )
	{
		SetParameter( pParam );
	}

	IParameterBinder::~IParameterBinder( void  )
	{
		
	}

	void IParameterBinder::SetParameter( IParameter *pParam )
	{
		if ( pParam->GetType() == GetType() )
			m_pParam = pParam;
		else
			PSX_Assert( FALSE, "Invalid parameter set to ParameterBinder." );
	}
}