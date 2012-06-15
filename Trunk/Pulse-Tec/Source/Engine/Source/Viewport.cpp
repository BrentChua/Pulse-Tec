/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Viewport.cpp
*
*	Comments	-	See Viewport.h
*
**************************************************************************************/
#include "../Include/Viewport.h"

namespace Pulse
{
	Viewport::Viewport( void )
	{

	}

	Viewport::~Viewport( void )
	{

	}

	BOOL Viewport::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::VIEWPORT )
			return FALSE;

		m_desc = *((ViewportDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL Viewport::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL Viewport::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL Viewport::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type Viewport::Initialize( void )
	{
		m_viewport.TopLeftX = m_desc.topLeftX;
		m_viewport.TopLeftY = m_desc.topLeftY;
		m_viewport.Width	= m_desc.width;
		m_viewport.Height	= m_desc.height;
		m_viewport.MinDepth = m_desc.minDepth;
		m_viewport.MaxDepth = m_desc.maxDepth;

		return EErrorCode::NONE;
	}

	void Viewport::Cleanup( void )
	{

	}
}