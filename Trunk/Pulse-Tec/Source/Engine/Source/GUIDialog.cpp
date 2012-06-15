/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIDialog.cpp
*
*	Comments	-	See GUIDialog.h
*
**************************************************************************************/
#include "../Include/GUIDialog.h"

namespace Pulse
{

	GUIDialog::GUIDialog( void )
	{

	}

	GUIDialog::~GUIDialog( void )
	{
		OnCleanup();
	}

	EErrorCode::Type GUIDialog::Initialize( void )
	{
		return OnInitialize();
	}

}