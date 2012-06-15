/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIText.cpp
*
*	Comments	-	See GUIText.h
*
**************************************************************************************/
#include "../Include/GUIText.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/Font.h"

namespace Pulse
{

	GUIText::GUIText( const CHAR *pFont )
		: Base(), m_fontFile(pFont)
	{
		Initialize();

		//m_pMaterial->SetAlphaTesting( TRUE );
		//m_pMaterial->SetAlpha( 1.01f);
		//SetAlpha( 1.01f );
		//SetAlphaBlending( TRUE );
	}

	GUIText::~GUIText( void )
	{
		PSX_SafeRelease( m_pFont );
	}

	void GUIText::Attach( ScreenElement *pParent )
	{
		Base::Attach( pParent );

		// Reformat processed text
		SetText( m_text.GetBuffer() );
	}

	void GUIText::SetText( const CHAR *pText )
	{
		m_text = pText;
		Rect r;
		const Rect *pRect = GetScissorRect();

		if ( pRect )
		{
			r.left = 0;
			r.top = 0;
			r.right = pRect->right - pRect->left;
			r.bottom = pRect->bottom - pRect->top;
		}
		else
		{
			r.left = 0;
			r.top = 0;
			r.right = 800;
			r.bottom = 600;
		}

		FontText text;
		text.pRect = &r;
		text.string = m_text;
		text.color = Color::WHITE;
		text.size = m_fontSize;
		m_processedString.Clear();
		m_textSize = m_pFont->ProcessString( &text, &m_processedString );
		//Base::SetSize( size.w, size.h );
	}

	void GUIText::SetSize( SIZE_T size ) 
	{ 
		m_fontSize = size; 

		if ( m_text.IsEmpty() == FALSE )
			SetText( m_text.GetBuffer() );
	}


	SIZE_T GUIText::GetDefaultSize( void ) 
	{ 
		return m_pFont->GetDefaultFontSize(); 
	}

	EErrorCode::Type GUIText::Initialize( void )
	{
		String name;
		GraphicsResourceManager *pMan = GraphicsResourceManager::GetInstance();

		FontDescription desc( m_fontFile.GetBuffer() );
		name.Format( "Font %s", m_fontFile.GetBuffer() );
		m_pFont = pMan->CreateFont( name.GetBuffer(), &desc );

		if ( m_pFont == PSX_NULL )
			return EErrorCode::GRAPHICS;
		
		m_fontSize = m_pFont->GetDefaultFontSize();
		m_textColor = Color::WHITE;
			
		return EErrorCode::OKAY;
	}

	void GUIText::UpdateScissorRect( void )
	{
		Vector3 globalPos		= GetGlobalPosition();

		m_scissorRect.left		= (LONG)globalPos.x - (LONG)m_anchor.x;
		m_scissorRect.top		= (LONG)globalPos.y - (LONG)m_anchor.y;
		m_scissorRect.right		= (LONG)m_scissorRect.left + (LONG)(m_textSize.w);
		m_scissorRect.bottom	= (LONG)m_scissorRect.top + (LONG)(m_textSize.h);
	}

	// Used by the renderer
	void GUIText::Bind( void )
	{
		Base::Bind();

		// Override some stuff by using Font
		m_pFont->SetProcessedString( &m_processedString );
		m_pFont->Bind();
	}

	// Used by the renderer
	void GUIText::Unbind( void )
	{
		Base::Unbind();
		m_pFont->Unbind();
	}

	//
	void GUIText::_Render( void )
	{
		m_pFont->Render();
	}

}