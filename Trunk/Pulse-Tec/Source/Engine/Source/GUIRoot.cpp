/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIRoot.cpp
*
*	Comments	-	See GUIRoot.h
*
**************************************************************************************/
#include "../Include/GUIRoot.h"
#include "../Include/Renderer.h"

namespace Pulse
{

	GUIRoot::GUIRoot( void )
	{
	}

	GUIRoot::~GUIRoot( void )
	{
	}

	EErrorCode::Type GUIRoot::Initialize( void )
	{
		DisplayMode *pMode = Renderer::GetInstance()->GetDisplayMode();

		// GUIRoot takes the entire render screen and update the scissor rect
		//	since it needs to display all its children.
		this->SetSize( (FLOAT)pMode->width, (FLOAT)pMode->height );

		SetScissorRectEnable( FALSE );

		return EErrorCode::NONE;
	}


}