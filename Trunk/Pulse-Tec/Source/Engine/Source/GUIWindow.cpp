/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIWindow.cpp
*
*	Comments	-	See GUIWindow.h
*
**************************************************************************************/
#include "../Include/GUIWindow.h"
#include "../Include/GUIFrame.h"
#include "../Include/Vector3.h"

namespace Pulse
{

	const LONG GUIWindow::m_sidePadding = 5;
	const LONG GUIWindow::m_tittlePadding = 15;

	GUIWindow::GUIWindow( POS_T x, POS_T y, SIZE_T w, SIZE_T h )
		: m_frame( x, y, w, h ), m_bLButtonDown( FALSE ), 
		m_windowTittle( PSX_String("Resources/Fonts/helvetica.fnt") ),
		m_buttonClose( PSX_String("Resources/GUI/Close.xml") )
	{
		SetSize( (FLOAT)w, (FLOAT)h );

		Initialize();


		SetBordered( TRUE );
	}

	GUIWindow::~GUIWindow( void )
	{

	}

	void GUIWindow::Render( void )
	{
		//RenderBottomChildren();

		//m_frame.Render();

		Base::Render();

		//m_innerBorder.Render();

		//RenderTopChildren();
	}

	void GUIWindow::SetSize( FLOAT w, FLOAT h )
	{
		Base::SetSize( w, h );

		UpdateScissorRect();

		m_innerBorder.ClearPoints();
		m_innerBorder.AddPoint( (FLOAT)m_scissorRect.left + m_sidePadding, (FLOAT)m_scissorRect.top + m_sidePadding + m_tittlePadding, 0.5f, Color::GRAY );
		m_innerBorder.AddPoint( (FLOAT)m_scissorRect.right - m_sidePadding, (FLOAT)m_scissorRect.top + m_sidePadding + m_tittlePadding, 0.5f, Color::GRAY );
		m_innerBorder.AddPoint( (FLOAT)m_scissorRect.right - m_sidePadding, (FLOAT)m_scissorRect.bottom - m_sidePadding, 0.5f, Color::GRAY );
		m_innerBorder.AddPoint( (FLOAT)m_scissorRect.left + m_sidePadding, (FLOAT)m_scissorRect.bottom - m_sidePadding , 0.5f, Color::GRAY );
		m_innerBorder.AddPoint( (FLOAT)m_scissorRect.left + m_sidePadding, (FLOAT)m_scissorRect.top + m_sidePadding + m_tittlePadding, 0.5f, Color::GRAY );

		m_container.SetSize( w - m_sidePadding * 2, h - m_sidePadding * 2 - m_tittlePadding );
	}

	void GUIWindow::Update( FLOAT dt )
	{

	}

	void GUIWindow::OnMouseLButtonDown( Point2 &mousePos )
	{
		m_bLButtonDown = TRUE; 
		m_oldMousePos.x = mousePos.x;
		m_oldMousePos.y = mousePos.y;
	}

	void GUIWindow::OnMouseMove( Point2 &mousePos )
	{
		if ( m_bLButtonDown )
		{
			FLOAT deltaX = mousePos.x - m_oldMousePos.x;
			FLOAT deltaY = mousePos.y - m_oldMousePos.y;
			
			Translate( deltaX, deltaY );
			m_oldMousePos = mousePos;
		}
	}

	EErrorCode::Type GUIWindow::Initialize( void )
	{
		//m_frame.SetSize( m_size.w, m_size.h ); // FIXME: This doesn't work.
		m_frame.SetZOrder( EGUIZOrder::BOTTOM );
		ForceObjectAttachToWindow( &m_frame );

		//m_innerBorder.SetZOrder( EGUIZOrder::BOTTOM );
		//m_innerBorder.Attach( this );

		// Set Window tittle
		m_windowTittle.SetZOrder( EGUIZOrder::BOTTOM );
		m_windowTittle.SetText( "Sample Untitled Window" );
		m_windowTittle.SetSize( 13 );
		m_windowTittle.SetColor( Color::BLACK );
		//m_windowTittle.SetScissorRectEnable( FALSE );
		m_windowTittle.SetPosition( 3.0f, 5.0f );
		ForceObjectAttachToWindow( &m_windowTittle );

		// Close button
		m_buttonClose.SetZOrder( EGUIZOrder::BOTTOM );
		m_buttonClose.SetSize( 21.0f, 17.0f );
		m_buttonClose.SetPosition( m_size.w - 32.0f, 1.0f );
		//m_buttonClose.SetAlphaTesting( TRUE );
		m_buttonClose.SetAlphaBlending( TRUE );
		//m_buttonClose.SetAlpha( 0.50f );
		m_buttonClose.SetBordered( FALSE );
		ForceObjectAttachToWindow( &m_buttonClose );

		// Inner Border
		m_innerBorder.SetZOrder( EGUIZOrder::TOPMOST );
		ForceObjectAttachToWindow( &m_innerBorder );

		// Container
		m_container.SetPosition( m_sidePadding, m_sidePadding + m_tittlePadding );
		m_container.SetParentFocusEnabled( TRUE );
		ForceObjectAttachToWindow( &m_container );

		SetHitTesting( TRUE );

		return EErrorCode::OKAY;
	}

	void GUIWindow::AddChild( ScreenElement *pObj )
	{
		//((GUI*)pObj)->SetBordered( TRUE );
		//Base::AddChild( pObj );

		// Added objects will be stored in the contianer,
		//	which is a child of GUIWindow
		((GUI*)pObj)->SetParent( &m_container ); // override parent
		m_container.AddChild( pObj );
	}

	void GUIWindow::ForceObjectAttachToWindow( GUI *pGUI )
	{
		pGUI->SetParent( this );
		Base::AddChild( pGUI );

		pGUI->UpdateBorderDimension();
		pGUI->UpdateScissorRect();
	}

	void GUIWindow::UpdateScissorRect( void )
	{
		GUI::UpdateScissorRect();

		//m_scissorRect.left += m_sidePadding;
		//m_scissorRect.top += m_sidePadding + m_tittlePadding;
		//m_scissorRect.right -= m_sidePadding;
		//m_scissorRect.bottom -= m_sidePadding;
	}

	void GUIWindow::Bind( void )
	{
		Base::Bind();
	}

	void GUIWindow::Unbind( void )
	{
		Base::Unbind();
	}

}