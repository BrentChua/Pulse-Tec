/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIControl.cpp
*
*	Comments	-	See GUIControl.h
*
**************************************************************************************/
#include "../Include/GUIControl.h"

namespace Pulse
{

	GUIControl::GUIControl( void ) 
		: ScreenElement(), m_state(EGUIControlState::ACTIVE), m_pControlFocus( PSX_NULL ),
		m_pControlOver( PSX_NULL )
	{ 

	}

	BOOL GUIControl::OnMessageProc( AppMsg *pMsg )
	{
		BOOL result = FALSE;

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
				Point2 mousePos( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 );
				Point2 localMousePos = PointToLocal( &mousePos );
				OnMouseMove( mousePos );
				
				for ( SIZE_T i = 0; i < m_children.GetSize(); ++i )
				{
					ScreenElement *pChild = m_children[i];

					if ( pChild ->IsPointInRect( &localMousePos ) )
					{
						if ( m_pControlOver && pChild != m_pControlOver )
							m_pControlOver->SetState( EGUIControlState::ACTIVE );

						m_pControlOver = pChild;

						if ( m_pControlOver->GetControlState() != EGUIControlState::PRESSED )
							m_pControlOver->SetState( EGUIControlState::HIGHLIGHTED );

						break;
					}
				}

				result = TRUE;
			}
			break;
		case EAppMessage::MOUSELBUTTONDOWN:
			OnMouseLButtonDown( Point2( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 ) );
			result = TRUE;
			break;
		case EAppMessage::MOUSELBUTTONUP:
			OnMouseLButtonUp( Point2( (FLOAT)pMsg->lMsg1, (FLOAT)pMsg->lMsg2 ) );
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
		default:
			// hmm....
			break;
		}

		return TRUE;
	}

	void GUIControl::SetState( EGUIControlState::Type state )
	{
		m_state = state;
		OnStateChange( state );
	}

}