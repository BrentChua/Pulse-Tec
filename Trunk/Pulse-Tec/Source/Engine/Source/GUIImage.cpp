/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIImage.cpp
*
*	Comments	-	See GUIImage.h
*
**************************************************************************************/

#ifdef __PSX_WINDOWS_WINDOW

#include "../Include/GUIImage.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/Renderer.h"

namespace Pulse
{
	GUIImage::GUIImage( const CHAR *pFileName )
		: m_pTexture( PSX_NULL )
	{
		if ( pFileName != PSX_NULL )
		{
			m_fileName = pFileName;

			Initialize();

			Size2 size = GetTextureSize();

			m_textureRect.left = 0;
			m_textureRect.top = 0;
			m_textureRect.right = (LONG)size.w;
			m_textureRect.bottom = (LONG)size.h;
		}
	}
		
	GUIImage::~GUIImage( void )
	{
		PSX_SafeRelease( m_pTexture );
	}

	//EErrorCode::Type GUIImage::OnRestore( void )
	EErrorCode::Type GUIImage::Initialize( void )
	{
		Base::Initialize();

		String name;
		GraphicsResourceManager *pMan = GraphicsResourceManager::GetInstance();

		// Create texture
		TextureDescription textureDesc;
		textureDesc.filename = m_fileName;
		m_pTexture = pMan->CreateTexture( m_fileName.GetBuffer(), &textureDesc );

		if ( m_pTexture == PSX_NULL )
			return EErrorCode::GRAPHICS;

		SetSize( (FLOAT)m_pTexture->GetTextureDescription()->width, 
			(FLOAT)m_pTexture->GetTextureDescription()->height );

		if ( m_pVB == PSX_NULL )
			return EErrorCode::GRAPHICS;

		// Create material
		name.Format( "Material%p", this );
		m_pMaterial = pMan->CreateMaterial( name.GetBuffer() );
		m_pMaterial->SetRenderTechnique( pMan->FindRenderTechnique(COMMON_RT_2D) );
		m_pMaterial->AddTextureSampler( TextureSampler(m_pTexture) );

		SetHitTesting( FALSE );

		return EErrorCode::NONE;
	}

	Size2 GUIImage::GetTextureSize( void )
	{
		if ( m_pTexture == PSX_NULL )
			return Size2( 0.0f, 0.0f );

		TextureDescription *pDesc = m_pTexture->GetTextureDescription();
		return Size2( static_cast<FLOAT>(pDesc->width), static_cast<FLOAT>(pDesc->height) );
	}

	void GUIImage::SetTextureRect( Rect *pRect )
	{
		m_textureRect = *pRect;
	}

	//EErrorCode::Type GUIImage::OnMsgProc( AppMsg msg )
	//{
	//	return EErrorCode::NONE;
	//}
}

#endif /* __PSX_WINDOWS_WINDOW */
