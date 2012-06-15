/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIButton.cpp
*
*	Comments	-	See GUIButton.h
*
**************************************************************************************/
#include "../Include/GUIButton.h"
#include "../Include/Texture.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/XMLParser.h"
#include "../Include/XMLElement.h"

namespace Pulse
{
	GUIButton::GUIButton( const CHAR *pButtonFile )
	{
		m_buttonFile = pButtonFile;

		Initialize();

	}

	GUIButton::~GUIButton( void )
	{
		PSX_SafeRelease( m_pTexture );
	}

	//void GUIButton::Attach( ScreenElement *pParent )
	//{

	//}

	//void GUIButton::Detach( void )
	//{

	//}

	//void GUIButton::SetAlpha( FLOAT alpha )
	//{
	//	Base::SetAlpha( alpha );

	//	for ( INT i = 0; i < EGUIState::EnumCount; ++i )
	//		m_stateImages[i]->SetAlpha( alpha );
	//}

	//void GUIButton::SetAlphaTesting( BOOL bEnable ) 
	//{
	//	for ( INT i = 0; i < EGUIState::EnumCount; ++i )
	//		m_stateImages[i]->SetAlphaTesting( bEnable );
	//}

	//BOOL GUIButton::IsAlphaTestingEnabled( void ) 
	//{
	//	return m_stateImages[0]->IsAlphaTestingEnabled(); 
	//}

	Size2 GUIButton::GetTextureSize( void )
	{
		TextureDescription *pDesc = m_pTexture->GetTextureDescription();
		return Size2( (FLOAT)pDesc->width, (FLOAT)pDesc->height );
	}

	void GUIButton::SetTextureRect( Rect *pRect )
	{
		for ( SIZE_T i = 0; i < EGUIState::EnumCount; ++i )
			m_textureRects[i] = *pRect;
	}

	const Rect * GUIButton::GetTextureRect( void )
	{
		return &m_textureRects[GetControlState()];
	}

	void GUIButton::OnStateChange( EGUIState::Type state )
	{
		//RemoveAllChildren();
		//m_stateImages[state]->Attach( this ); 
		//m_pMaterial->ClearTextureSamplers();
		//m_pMaterial->AddTextureSampler( TextureSampler(m_stateImages[state]) );

		//// Set size based on the current state image
		//SetSize( (FLOAT)m_stateImages[GetControlState()]->GetTextureDescription()->width, 
		//	(FLOAT)m_stateImages[GetControlState()]->GetTextureDescription()->height );
	}

	EErrorCode::Type GUIButton::Initialize( void )
	{
		Base::Initialize();

		GraphicsResourceManager *pGMan = GraphicsResourceManager::GetInstance();

		XMLElementPtr pNode= XMLParser::Load( m_buttonFile.GetBuffer() );

		if ( pNode.IsNull() )
		{
			PSX_PushError( "Failed to load button definition xml file, %s.", m_buttonFile.GetBuffer() );
			return EErrorCode::FILE;
		}

		XMLElementPtr pChild = pNode->GetFirstChild();
		String tag;

		while ( pChild )
		{
			tag = pChild->GetTagName();

			if ( tag == PSX_String("Information") )
			{
				m_textureFile = PSX_String("Resources/GUI/") + pChild->GetAttribute( PSX_String("file") );
				TextureDescription desc( m_textureFile.GetBuffer() );
				m_pTexture = pGMan->CreateTexture( m_textureFile.GetBuffer(), &desc );
			}
			else if ( tag == PSX_String("States") )
				ParseStates( pChild );

			pChild = pChild->GetNextSibling();
		}

		// Create material
		String name;
		name.Format( "Material%p", this );
		m_pMaterial = pGMan->CreateMaterial( name.GetBuffer() );
		m_pMaterial->SetRenderTechnique( pGMan->FindRenderTechnique(COMMON_RT_2D) );
		m_pMaterial->AddTextureSampler( TextureSampler(m_pTexture) );

		SetState( EGUIState::ACTIVE );

		SetSize( (FLOAT)m_textureRects[GetControlState()].right - (FLOAT)m_textureRects[GetControlState()].left, 
			(FLOAT)m_textureRects[GetControlState()].bottom - (FLOAT)m_textureRects[GetControlState()].top);

		SetBordered( TRUE );

		SetHitTesting( TRUE );

		return EErrorCode::OKAY;
	}

	void GUIButton::ParseStates( XMLElementPtr pNode )
	{
		XMLElementPtr pChild = pNode->GetFirstChild();
		String tag;

		while( pChild )
		{
			tag = pChild->GetTagName();
			EGUIState::Type state;
			BOOL bSetData = TRUE;

			if ( tag == PSX_String("Active") )
				state = EGUIState::ACTIVE;
			else if ( tag == PSX_String("Highlighted") )
				state = EGUIState::HIGHLIGHTED;
			else if ( tag == PSX_String("Pressed") )
				state = EGUIState::PRESSED;
			else if ( tag == PSX_String("Disabled") )
				state = EGUIState::DISABLED;
			else 
				bSetData = FALSE;

			if ( bSetData )
			{
				tag = pChild->GetAttribute(PSX_String("x"));
				m_textureRects[state].left = tag.ToINT();

				tag = pChild->GetAttribute(PSX_String("y"));
				m_textureRects[state].top = tag.ToINT();

				tag = pChild->GetAttribute(PSX_String("width"));
				m_textureRects[state].right = m_textureRects[state].left + tag.ToINT();

				tag = pChild->GetAttribute(PSX_String("height"));
				m_textureRects[state].bottom = m_textureRects[state].top + tag.ToINT();
			}

			pChild = pChild->GetNextSibling();
		}
	}

	void GUIButton::Bind( void )
	{
		Base::Bind();

		m_pTexture->Bind();
	}

	void GUIButton::Unbind( void )
	{
		//m_stateImages[GetControlState()]->Unbind();
	
		Base::Unbind();
	}

	void GUIButton::_Render( void )
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