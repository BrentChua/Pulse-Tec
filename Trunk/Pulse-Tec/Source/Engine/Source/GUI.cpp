/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUI.cpp
*
*	Comments	-	See GUI.h
*
**************************************************************************************/
#include "../Include/GUI.h"
#include "../Include/VertexBuffer.h"
#include "../Include/GUILine.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/Renderer.h"
#include "../Include/AppMsg.h"
#include "../Include/GUIWindow.h"

namespace Pulse
{
	GUI::GUI( void )
		:  m_zOrder(EGUIZOrder::NORMAL), m_size( 1.0f, 1.0f ), m_anchor( 0.0f, 0.0f ), 
		m_bHitTest( FALSE ), m_bHaveParentFocus( FALSE ), m_pVB( PSX_NULL ), 
		m_bEnableScissorRect( TRUE ),m_bBordered( FALSE ), m_pBorder( PSX_NULL ), 
		m_state(EGUIState::ACTIVE), m_pControlFocus( PSX_NULL ), m_pControlOver( PSX_NULL )
	{
		PSX_ZeroMem( &m_scissorRect, sizeof(RECT) );

		m_childGroups[EGUIZOrder::TOPMOST] = &m_topZChildren;
		m_childGroups[EGUIZOrder::NORMAL] = &m_children;
		m_childGroups[EGUIZOrder::BOTTOM] = &m_bottomZhChildren;

	}

	GUI::~GUI( void )
	{
		PSX_SafeRelease( m_pVB );

	}

	void GUI::Attach( ScreenElement *pParent )
	{
		Base::Attach( pParent );

		UpdateBorderDimension();
		UpdateScissorRect();
	}

	void GUI::SetPosition( Vector2 *pos )
	{
		LONG deltaX = (LONG)(pos->x - m_pos.x);
		LONG deltaY = (LONG)(pos->y - m_pos.y);

		ScreenElement::SetPosition( &Vector3( pos->x, pos->y, m_pos.z ) );

		UpdateScissorRect();
		UpdateBorderDimension();

		AppMsg msg;
		msg.msg = EAppMessage::PARENTMOVE;
		msg.lMsg1 = deltaX;
		msg.lMsg2 = deltaY;
		PropagateMessage( &msg );
	}

	BOOL GUI::IsPointInRect( Point2 *pPoint )
	{
		FLOAT right = m_pos.x + GetWidth();
		FLOAT bottom = m_pos.y + GetHeight();

		//PSX_PrintLog( "Mouse x: %f Mouse y: %f", pPoint->x, pPoint->y );
		//PSX_PrintLog( "Rect x: %f Rect y: %f", m_pos.x, m_pos.y );

		// Bound rect inside the parent's scissor rect
		const Rect *pRect = GetScissorRect();

		if ( pRect )
		{
			Rect newRect = *pRect;
			Vector3 globalPos = m_pParent->GetGlobalPosition();

			newRect.left	-= (LONG)globalPos.x;
			newRect.top		-= (LONG)globalPos.y;
			newRect.right	-= (LONG)globalPos.x;
			newRect.bottom	-= (LONG)globalPos.y;

			if ( newRect.left < m_pos.x  )
				newRect.left = (LONG)m_pos.x;

			if ( newRect.top < m_pos.y )
				newRect.top = (LONG)m_pos.y;

			if ( newRect.right > right )
				newRect.right = (LONG)right;

			if ( newRect.bottom > bottom )
				newRect.bottom = (LONG)bottom;

			if ( pPoint->x < newRect.left || pPoint->x > newRect.right ||
				pPoint->y < newRect.top || pPoint->y > newRect.bottom )
				return FALSE;
		}
		else
		{
			if ( pPoint->x < m_pos.x || pPoint->x > right ||
				pPoint->y < m_pos.y || pPoint->y > bottom )
				return FALSE;
		}

		return TRUE;
	}

	Point2 GUI::PointToLocal( Point2 *pPoint )
	{
		Vector3 gPos = GetGlobalPosition();
		Point2 ret( pPoint->x - gPos.x, pPoint->y - gPos.y );
		return ret;
	}

	void GUI::RemoveChild( ScreenElement *pObj )
	{
		EGUIZOrder::Type order = ((GUI*)pObj)->GetZOrder();

		switch( order )
		{
		case EGUIZOrder::BOTTOM:
			m_bottomZhChildren.Remove( pObj );
			break;
		case EGUIZOrder::NORMAL:
			Base::RemoveChild( pObj );
			break;
		case EGUIZOrder::TOPMOST:
			m_topZChildren.Remove( pObj );
			break;
		}
	}

	void GUI::SetSize( FLOAT w, FLOAT h )
	{
		m_size.x = w;
		m_size.y = h;

		UpdateScissorRect();
		UpdateBorderDimension();
		
		//AppMsg msg;
		//msg.
	}

	void GUI::SetWidth( FLOAT w )
	{
		SetSize( w, GetHeight() );
	}

	void GUI::SetHeight( FLOAT h )
	{
		SetSize( GetWidth(), h );
	}

	void GUI::SetAlpha( FLOAT alpha ) 
	{ 
		ScreenElement::SetAlpha( alpha );

		if ( IsBordered() )
			m_pBorder->SetAlpha( alpha );
	}

	const Rect * GUI::GetScissorRect( void ) const
	{
		if ( m_pParent && IsScissorRectEnabled() )
		{
			GUI *pGUI = dynamic_cast<GUI *>(m_pParent);

			if ( pGUI )
			{
				if ( pGUI->IsScissorRectEnabled() )
					return &pGUI->m_scissorRect;
				else
					return pGUI->GetScissorRect();
			}
		}

		return PSX_NULL;
	}

	void GUI::SetBordered( BOOL bBorder )
	{
		if ( m_bBordered == bBorder )
			return;

		PSX_SafeDelete( m_pBorder );

		m_pBorder = new GUILine;

		m_bBordered = bBorder;

		UpdateBorderDimension();

	}

	void GUI::Update( FLOAT dt )
	{
		for ( INT i = 0; i < EGUIZOrder::EnumCount; ++i )
		{
			ChildList *pList = m_childGroups[i];
			ChildList::Iterator iter = pList->IteratorBegin();
			ChildList::Iterator iterEnd = pList->IteratorEnd();

			while ( iter != iterEnd )
			{
				(*iter)->Update( dt );
				++iter;
			}
		}
	}

	void GUI::Render( void )
	{
		Renderer *pRenderer = Renderer::GetInstance();

		// Self first, because he's greedy!
		pRenderer->AddToRender( this );

		RenderBottomChildren();

		// Normal
		ChildList::Iterator iter = m_children.IteratorBegin();
		ChildList::Iterator iterEnd = m_children.IteratorEnd();

		while ( iter != iterEnd )
		{
			//pRenderer->AddToRender( *iter );
			(*iter)->Render();
			++iter;
		}

		RenderTopChildren();

		if ( IsBordered() )
			m_pBorder->Render();
	}

	BOOL GUI::OnMessageProc( AppMsg *pMsg )
	{
		BOOL result = FALSE;

		EGUIState::Type state = GetControlState();

		switch( pMsg->msg )
		{
		case EAppMessage::KEYDOWN:
			OnKeyDown( (UCHAR)pMsg->bMsg1 );
			result = TRUE;
			break;
		case EAppMessage::KEYUP:
			OnKeyUp( (UCHAR)pMsg->bMsg1 );
			result = TRUE;
			break;
		case EAppMessage::MOUSEMOVE:
			{
				INT i = -1;
				BOOL bBreakLoop = FALSE;
				Point2 mousePos( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 );
				Point2 localMousePos = PointToLocal( &mousePos );

				if ( state != EGUIState::PRESSED && state != EGUIState::DISABLED )
				{
					for ( SIZE_T j = 0; j < EGUIZOrder::EnumCount; ++j )
					{
						ChildList *pChildList = m_childGroups[j];

						for ( i = pChildList->GetSize() - 1; i >= 0; --i )
						{
							GUI *pChild = (GUI*)(*pChildList)[i];

							if ( pChild->GetControlType() != EGUIType::FRAME && 
								pChild->IsHitTestEnabled() &&
								pChild->IsPointInRect( &localMousePos ) )
							{
								if ( m_pControlFocus == PSX_NULL || 
									m_pControlFocus->GetControlState() != EGUIState::PRESSED )
								{
									if ( m_pControlOver && pChild != m_pControlOver )
										m_pControlOver->SetState( EGUIState::ACTIVE );

									// Trigger GUI event
									if ( m_pControlOver != pChild && pChild->IsHitTestEnabled() )
									{
										GUIEvent event;
										event.pGUI = pChild;
										event.mousePos = mousePos;
										event.type = EGUIEventType::ENTER;
										TriggerEvent( &event );
									}

									m_pControlOver = pChild;

									if ( m_pControlOver->GetControlState() != EGUIState::PRESSED )
										m_pControlOver->SetState( EGUIState::HIGHLIGHTED );

									if ( m_pControlFocus != pChild && pChild->GetControlType() == EGUIType::CONTAINER )
										pChild->OnMessageProc( pMsg );
								}
								else
									m_pControlOver = pChild;

								bBreakLoop = TRUE;

								break;
							}
						}

						if ( bBreakLoop )
							break;
					}
				}

				if ( i == -1 )
				{
					if ( m_pControlOver )
					{
						if ( m_pControlFocus == PSX_NULL || 
							m_pControlFocus && m_pControlFocus->GetControlState() != EGUIState::PRESSED )
						{
							m_pControlOver->SetState( EGUIState::ACTIVE );
						}

						// Trigger GUI event
						GUIEvent event;
						event.pGUI = m_pControlOver;
						event.mousePos = mousePos;
						event.type = EGUIEventType::EXIT;
						TriggerEvent( &event );

						m_pControlOver = PSX_NULL;

					}
					else if ( m_pControlFocus == PSX_NULL || m_pControlFocus && 
						m_pControlFocus->GetControlState() != EGUIState::PRESSED )
					{
						OnMouseMove( mousePos );

						// Trigger GUI event
						GUIEvent event;
						event.pGUI = PSX_NULL;
						event.mousePos = mousePos;
						event.type = EGUIEventType::MOUSEMOVED;
						TriggerEvent( &event );
					}
				}

				result = TRUE;
			}
			break;
		case EAppMessage::MOUSELBUTTONDOWN:
			{
				INT i;
				BOOL bBreakLoop = FALSE;
				Point2 mousePos( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 );
				Point2 localMousePos = PointToLocal( &mousePos );

				for ( SIZE_T j = 0; j < EGUIZOrder::EnumCount; ++j )
				{
					ChildList *pChildList = m_childGroups[j];

					for ( i = pChildList->GetSize() - 1; i >= 0; --i )
					{
						GUI *pChild = (GUI*)(*pChildList)[i];

						if ( pChild->GetControlType() != EGUIType::FRAME &&
							pChild->IsHitTestEnabled() && pChild->IsPointInRect( &localMousePos ) )
						{
							// From highlighted to active
							if ( m_pControlFocus && m_pControlFocus->GetControlState() == EGUIState::HIGHLIGHTED )
								m_pControlFocus->SetState( EGUIState::ACTIVE );

							m_pControlFocus = pChild;
							m_pControlFocus->SetState( EGUIState::PRESSED );

							// Only bring to front if in normal Z ordering
							if ( j == EGUIZOrder::NORMAL )
							{
								pChildList->Remove( pChild );
								pChildList->PushBack( pChild );
							}

							bBreakLoop = TRUE;
							break;
						}
					}

					if ( bBreakLoop )
						break;
				}

				if ( i == -1 )
				{
					m_pControlFocus = PSX_NULL;
					OnMouseLButtonDown( Point2( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 ) );
				}

			}
			result = TRUE;
			break;
		case EAppMessage::MOUSELBUTTONUP:
			{
				Point2 mousePos( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 );
				Point2 localMousePos = PointToLocal( &mousePos );

				if ( m_pControlFocus )
				{
					if ( m_pControlFocus == m_pControlOver )
					{
						m_pControlFocus->SetState( EGUIState::HIGHLIGHTED );

						if ( m_pControlFocus->IsHitTestEnabled() )
						{
							GUIEvent event;
							event.pGUI = m_pControlFocus;
							event.mousePos = mousePos;
							event.type = EGUIEventType::CLICKED;
							TriggerEvent( &event );
						}
					}
					else
					{
						m_pControlFocus->SetState( EGUIState::ACTIVE );

						AppMsg msg = *pMsg;
						msg.msg = EAppMessage::MOUSEMOVE;
						this->OnMessageProc( &msg );
					}
				}
				else
				{

					OnMouseLButtonUp( Point2( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 ) );
				}
			}
			result = TRUE;
			break;
		case EAppMessage::MOUSERBUTTONDOWN:
			OnMouseRButtonDown( Point2( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 ) );
			result = TRUE;
			break;
		case EAppMessage::MOUSERBUTTONUP:
			OnMouseRButtonUp( Point2( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 ) );
			result = TRUE;
			break;
		case EAppMessage::PARENTMOVE:
			UpdateBorderDimension();
			UpdateScissorRect();

			PropagateMessage( pMsg );
			break;
		default:
			// hmm....
			break;
		}

		if ( m_pControlFocus )
			m_pControlFocus->OnMessageProc( pMsg );

		return TRUE;
	}

	EErrorCode::Type GUI::Initialize( void )
	{
		GraphicsResourceManager *pMan = GraphicsResourceManager::GetInstance();

		// Create vertex buffer
		Vertex1P1D1T quad[4];
		VertexBufferDescription desc;
		desc.bufferType = EVertexBufferType::STATIC;
		desc.primitiveType = EPrimitiveType::TRIANGLE_STRIP;
		desc.vertexSize = PSX_SizeOf( Vertex1P1D1T );
		desc.numVertices = 4;
		desc.numPrimitives = desc.numVertices - 2;
		desc.pData = quad;
		desc.pDecl = pMan->GetVertexDeclaration( COMMON_VD_1P1D1T );

		quad[0].position	= Vector3( -1.0f, 1.0f, 0.0f );
		quad[0].diffuse	= 0xFFFFFFFF;
		quad[0].texCoord	= Vector2( 0.0f, 0.0f );

		quad[1].position	= Vector3( 1.0f, 1.0f, 0.0f );
		quad[1].diffuse	= 0xFFFFFFFF;
		quad[1].texCoord	= Vector2( 1.0f, 0.0f );

		quad[2].position	= Vector3( -1.0f, -1.0f, 0.0f );
		quad[2].diffuse	= 0xFFFFFFFF;
		quad[2].texCoord	= Vector2( 0.0f, 1.0f );

		quad[3].position	= Vector3( 1.0f, -1.0f, 0.0f );
		quad[3].diffuse	= 0xFFFFFFFF;
		quad[3].texCoord	= Vector2( 1.0f, 1.0f );

		m_pVB = pMan->CreateVertexBuffer( COMMON_VB_1P1D1T, &desc );

		if ( m_pVB == PSX_NULL )
			return EErrorCode::GRAPHICS;

		return EErrorCode::NONE;
	}

	void GUI::AddChild( ScreenElement *pObj )
	{
		EGUIZOrder::Type order = ((GUI*)pObj)->GetZOrder();

		switch( order )
		{
		case EGUIZOrder::BOTTOM:
			m_bottomZhChildren.PushBack( pObj );
			break;
		case EGUIZOrder::NORMAL:
			Base::AddChild( pObj );
			break;
		case EGUIZOrder::TOPMOST:
			m_topZChildren.PushBack( pObj );
			break;
		}
	}

	void GUI::RenderBottomChildren( void )
	{
		Renderer *pRenderer = Renderer::GetInstance();

		// Then children second
		ChildList::Iterator iter = m_bottomZhChildren.IteratorBegin();
		ChildList::Iterator iterEnd = m_bottomZhChildren.IteratorEnd();

		while ( iter != iterEnd )
		{
			//pRenderer->AddToRender( *iter );
			(*iter)->Render();
			++iter;
		}
	}

	void GUI::RenderTopChildren( void )
	{
		Renderer *pRenderer = Renderer::GetInstance();

		// Then children second
		ChildList::Iterator iter = m_topZChildren.IteratorBegin();
		ChildList::Iterator iterEnd = m_topZChildren.IteratorEnd();

		while ( iter != iterEnd )
		{
			//pRenderer->AddToRender( *iter );
			(*iter)->Render();
			++iter;
		}
	}

	void GUI::PropagateMessage( AppMsg *pMsg )
	{
		for ( INT i = m_bottomZhChildren.GetSize() - 1; i >= 0; --i )
		{
			GUI *pChild = (GUI*)m_bottomZhChildren[i];

			pChild->OnMessageProc( pMsg );
		}

		for ( INT i = m_children.GetSize() - 1; i >= 0; --i )
		{
			GUI *pChild = (GUI*)m_children[i];

			pChild->OnMessageProc( pMsg );
		}

		for ( INT i = m_topZChildren.GetSize() - 1; i >= 0; --i )
		{
			GUI *pChild = (GUI*)m_topZChildren[i];

			pChild->OnMessageProc( pMsg );
		}
	}

	void GUI::UpdateBorderDimension( void )
	{
		if ( IsBordered() == FALSE )
			return;

		m_pBorder->ClearPoints();

		Vector3 pos = GetGlobalPosition();
		Size2 *pSize = GetSize();

		const Rect *pRect = GetScissorRect();

		if ( pRect )
		{
			FLOAT left, top, right, bottom;

			if ( pos.x < (FLOAT)pRect->left )
				left = (FLOAT)pRect->left;
			else if ( pos.x > (FLOAT)pRect->right )
				left = (FLOAT)pRect->right;
			else
				left = pos.x;

			if ( pos.y < (FLOAT)pRect->top )
				top = (FLOAT)pRect->top;
			else if ( pos.y > (FLOAT)pRect->bottom )
				top = (FLOAT)pRect->bottom;
			else
				top = pos.y;

			if ( pos.x + pSize->w < (FLOAT)pRect->left )
				right = (FLOAT)pRect->left;
			else if ( pos.x + pSize->w > (FLOAT)pRect->right )
				right = (FLOAT)pRect->right;
			else
				right = pos.x + pSize->w;

			if ( pos.y + pSize->h < (FLOAT)pRect->top )
				bottom = (FLOAT)pRect->top;
			else if ( pos.y + pSize->h > (FLOAT)pRect->bottom )
				bottom = (FLOAT)pRect->bottom;
			else
				bottom = pos.y + pSize->h;

			m_pBorder->AddPoint( left,	top,	0.0f, Color::GRAY );
			m_pBorder->AddPoint( right,	top,	0.0f, Color::GRAY );
			m_pBorder->AddPoint( right,	bottom,	0.0f, Color::GRAY );
			m_pBorder->AddPoint( left,	bottom,	0.0f, Color::GRAY );
			m_pBorder->AddPoint( left,	top,	0.0f, Color::GRAY );
		}
		else
		{
			m_pBorder->AddPoint( pos.x,						pos.y, pos.z, Color::GRAY );
			m_pBorder->AddPoint( pos.x + pSize->w,			pos.y, pos.z, Color::GRAY );
			m_pBorder->AddPoint( pos.x + pSize->w,			pos.y + pSize->h, pos.z, Color::GRAY );
			m_pBorder->AddPoint( pos.x, pos.y + pSize->h,	pos.z, Color::GRAY );
			m_pBorder->AddPoint( pos.x, pos.y,				pos.z, Color::GRAY );
		}
	}

	void GUI::UpdateScissorRect( void )
	{
		Vector3 globalPos		= GetGlobalPosition();

		m_scissorRect.left		= (LONG)globalPos.x - (LONG)m_anchor.x;
		m_scissorRect.top		= (LONG)globalPos.y - (LONG)m_anchor.y;
		m_scissorRect.right		= (LONG)m_scissorRect.left + (LONG)(m_size.w * m_scale);
		m_scissorRect.bottom	= (LONG)m_scissorRect.top + (LONG)(m_size.h * m_scale);
	}

	void GUI::FixZOrdering( void )
	{
		// Get Lowest level of children
	}

	void GUI::SetState( EGUIState::Type state )
	{
		m_state = state;
		OnStateChange( state );
	}

	void GUI::TriggerEvent( GUIEvent *pEvent )
	{
		EGUIType::Type type = GetControlType();

		// TODO: Add dialog
		if ( type == EGUIType::DIALOG || type ==EGUIType::SCREEN )
		{
			OnGUIEvent( pEvent );
		}
		else if ( m_pParent )
		{
			((GUI*)m_pParent)->TriggerEvent( pEvent );
		}
		else
		{
			PSX_PushWarning( "GUI event lost." );
		}
	}

	void GUI::Bind( void )
	{
		Renderer *pRenderer = Renderer::GetInstance();
		pRenderer->SetActiveRenderable( this );

		if ( m_pMaterial )
			m_pMaterial->Bind();
		else
			Renderer::GetInstance()->SetActiveMaterial( PSX_NULL );

		if ( m_pVB )
			m_pVB->Bind();

		Renderer::GetInstance()->GetDevice()->SetScissorRect( GetScissorRect() );
	}

	void GUI::Unbind( void )
	{
		if ( m_pMaterial )
			m_pMaterial->Unbind();

		if ( m_pVB )
			m_pVB->Unbind();
	}

	void GUI::_Render( void )
	{
		if ( m_pMaterial && m_pVB )
		{
			RenderTechnique *pTech = m_pMaterial->GetRenderTechnique();

			if ( pTech )
			{
				for ( SIZE_T i = 0; i < pTech->GetNumPasses(); ++i )
				{
					pTech->Begin(i);
					m_pVB->Render();
					pTech->End();
				}
			}
		}
	}

}