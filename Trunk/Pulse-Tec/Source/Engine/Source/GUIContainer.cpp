/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIContainer.cpp
*
*	Comments	-	See GUIContainer.h
*
**************************************************************************************/
#include "../Include/GUIContainer.h"
#include "../Include/ScreenElement.h"
#include "../Include/Renderer.h"

namespace Pulse
{
	GUIContainer::GUIContainer( FLOAT width , FLOAT height )
	{
		Initialize();

		//DisplayMode *pMode = Renderer::GetInstance()->GetDisplayMode();
		//SetSize( pMode->width, pMode->height );
	}

	GUIContainer::~GUIContainer( void )
	{

	}

	void GUIContainer::Update( DOUBLE time, FLOAT dt )
	{
		OnUpdate( time, dt );

		Base::Update( dt );
	}

	void GUIContainer::Render( void )
	{
		// Basically the same as ScreenElement except that we don't add ourself.
		Renderer *pRenderer = Renderer::GetInstance();

		RenderBottomChildren();
		
		// Then children second
		ChildList::Iterator iter = m_children.IteratorBegin();
		ChildList::Iterator iterEnd = m_children.IteratorEnd();

		while ( iter != iterEnd )
		{
			//pRenderer->AddToRender( *iter );
			(*iter)->Render();
			++iter;
		}

		RenderTopChildren();
	}

	EErrorCode::Type GUIContainer::Initialize( void )
	{
		SetHitTesting( TRUE );

		return EErrorCode::NONE;
	}

}