/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUIFrame.cpp
*
*	Comments	-	See GUIFrame.h
*
**************************************************************************************/
#include "../Include/GUIFrame.h"
#include "../Include/GraphicsResourceManager.h"
#include "../Include/GraphicsTypes.h"

namespace Pulse
{
	GUIFrame::GUIFrame( POS_T x, POS_T y, SIZE_T w, SIZE_T h )
		: GUI()
	{
		Initialize();

		SetPosition( (FLOAT)x, (FLOAT)y );
		SetSize( (FLOAT)w, (FLOAT)h );
	}

	GUIFrame::GUIFrame( SIZE_T w, SIZE_T h )
		: GUI()
	{
		Initialize();
		SetSize( (FLOAT)w, (FLOAT)h );
	}


	GUIFrame::~GUIFrame( void )
	{

	}

	EErrorCode::Type GUIFrame::Initialize( void )
	{
		String name;
		GraphicsResourceManager *pMan = GraphicsResourceManager::GetInstance();
		
		// Create vertex buffer
		Vertex1P1D *pQuad = new Vertex1P1D[4];
		VertexBufferDescription desc;
		desc.bufferType = EVertexBufferType::STATIC;
		desc.primitiveType = EPrimitiveType::TRIANGLE_STRIP;
		desc.vertexSize = PSX_SizeOf( Vertex1P1D );
		desc.numVertices = 4;
		desc.numPrimitives = desc.numVertices - 2;
		desc.pData = pQuad;
		desc.pDecl = pMan->GetVertexDeclaration( COMMON_VD_1P1D );

		pQuad[0].position	= Vector3( -1.0f, 1.0f, 0.0f );
		pQuad[0].diffuse	= 0xFFFFFFFF;

		pQuad[1].position	= Vector3( 1.0f, 1.0f, 0.0f );
		pQuad[1].diffuse	= 0xFFFFFFFF;

		pQuad[2].position	= Vector3( -1.0f, -1.0f, 0.0f );
		pQuad[2].diffuse	= 0xFFFFFFFF;

		pQuad[3].position	= Vector3( 1.0f, -1.0f, 0.0f );
		pQuad[3].diffuse	= 0xFFFFFFFF;

		m_pVB = pMan->CreateVertexBuffer( COMMON_VB_1P1D, &desc );
		delete [] pQuad;

		if ( m_pVB == PSX_NULL )
			return EErrorCode::GRAPHICS;

		// Create material
		name.Format( "Material%p", this );
		m_pMaterial = pMan->CreateMaterial( name.GetBuffer() );
		m_pMaterial->SetRenderTechnique( pMan->FindRenderTechnique( COMMON_RT_2DFRAME ) );
		SetAlphaBlending( TRUE );

		m_frameColor.SetRGBA( 89, 212, 216, 255 );
		SetAlpha( 0.80f );

		return EErrorCode::NONE;
	}
}