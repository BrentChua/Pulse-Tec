/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GuiInterfaces.cpp
*
*	Comments	-	See GuiInterfaces.h
*
**************************************************************************************/

#include "../Include/GUIBase.h"
#include "../Include/Renderer.h"
#include "../Include/VertexBuffer.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/Vector2.h"
#include "../Include/GUILine.h"

namespace Pulse
{

	GUIBase::GUIBase( void )
		: m_size( 0.0f, 0.0f ), m_anchor( 0.0f, 0.0f ), m_pVB( PSX_NULL ),
		m_bEnableScissorRect( TRUE ), m_alpha( 1.0f ), m_bVisible( TRUE ),
		m_bBordered( FALSE ), m_pBorder( PSX_NULL )
	{
		PSX_ZeroMem( &m_scissorRect, sizeof(RECT) );
	}

	GUIBase::~GUIBase( void )
	{
		PSX_SafeRelease( m_pVB );
	}

	void GUIBase::SetPosition( Vector2 *pos )
	{
		ScreenElement::SetPosition( &Vector3( pos->x, pos->y, m_pos.z ) );

		UpdateScissorRect();
		UpdateBorderDimension();
	}

	void GUIBase::SetAlpha( FLOAT alpha ) 
	{ 
		m_alpha = alpha; 
		
		if ( IsBordered() )
			m_pBorder->SetAlpha( alpha );
	}

	void GUIBase::Bind( void )
	{
		ScreenElement::Bind(); 
		
		if ( m_pVB )
			m_pVB->Bind();
		
		Renderer::GetInstance()->GetDevice()->SetScissorRect( GetScissorRect() );
	}
	
	void GUIBase::Unbind( void )
	{
		ScreenElement::Unbind();

		if ( m_pVB )
			m_pVB->Unbind();
	}

	void GUIBase::SetSize( FLOAT w, FLOAT h )
	{
		m_size.x = w;
		m_size.y = h;

		UpdateScissorRect();
		UpdateBorderDimension();
	}

	const Rect * GUIBase::GetScissorRect( void ) const
	{
		if ( m_pParent )
		{
			GUIBase *pGUI = dynamic_cast<GUIBase *>(m_pParent);

			if ( pGUI->IsScissorRectEnabled() )
				return &pGUI->m_scissorRect;
			else
				return pGUI->GetScissorRect();
		}
		else
			return PSX_NULL;
	}

	void GUIBase::SetBordered( BOOL bBorder )
	{
		if ( m_bBordered == bBorder )
			return;

		PSX_SafeDelete( m_pBorder );

		m_pBorder = new GUILine;

		m_bBordered = bBorder;

		UpdateBorderDimension();

	}

	void GUIBase::Render( void )
	{
		Base::Render();

		if ( IsBordered() )
			m_pBorder->Render();
	}

	void GUIBase::UpdateBorderDimension( void )
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

			m_pBorder->AddPoint( left,	top,	0.0f );
			m_pBorder->AddPoint( right,	top,	0.0f );
			m_pBorder->AddPoint( right,	bottom,	0.0f );
			m_pBorder->AddPoint( left,	bottom,	0.0f );
			m_pBorder->AddPoint( left,	top,	0.0f );
		}
		else
		{
			m_pBorder->AddPoint( pos.x,						pos.y, pos.z );
			m_pBorder->AddPoint( pos.x + pSize->w,			pos.y, pos.z );
			m_pBorder->AddPoint( pos.x + pSize->w,			pos.y + pSize->h, pos.z );
			m_pBorder->AddPoint( pos.x, pos.y + pSize->h,	pos.z );
			m_pBorder->AddPoint( pos.x, pos.y,				pos.z );
		}
	}

	void GUIBase::UpdateScissorRect( void )
	{
		Vector3 globalPos		= GetGlobalPosition();

		m_scissorRect.left		= (LONG)globalPos.x - (LONG)m_anchor.x;
		m_scissorRect.top		= (LONG)globalPos.y - (LONG)m_anchor.y;
		m_scissorRect.right		= (LONG)m_scissorRect.left + (LONG)(m_size.w * m_scale);
		m_scissorRect.bottom	= (LONG)m_scissorRect.top + (LONG)(m_size.h * m_scale);
	}

	EErrorCode::Type GUIBase::Initialize( void )
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

	void GUIBase::AddChild( ScreenElement *pObj )
	{
		Base::AddChild( pObj );
	}

	void GUIBase::RemoveChild( ScreenElement *pObj )
	{
		Base::RemoveChild( pObj );
	}

	void GUIBase::RemoveAllChildren( void )
	{
		Base::RemoveAllChildren();

	}

}