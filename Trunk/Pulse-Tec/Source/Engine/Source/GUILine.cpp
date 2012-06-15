/**************************************************************************************
*
*	Copyright (C) 2011 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GUILine.cpp
*
*	Comments	-	See GUILine.h
*
**************************************************************************************/
#include "../Include/GUILine.h"
#include "../Include/VertexBuffer.h"
#include "../Include/GraphicsResourceManager.h"

namespace Pulse
{
	GUILine::GUILine( void )
		: GUI(), m_bNeedUpdate( FALSE ), m_VBNumVerts( 0 )
	{
		SetScissorRectEnable( FALSE );

		// Force init to create material.
		Initialize();
	}

	GUILine::GUILine( SIZE_T vertexSize )
		: GUI(), m_bNeedUpdate( FALSE ), m_VBNumVerts( 0 )
	{
		m_points.Reserve( vertexSize );

		// Create material and vertex buffer
		Initialize();
	}

	GUILine::~GUILine( void )
	{

	}

	void GUILine::AddPoint( FLOAT x, FLOAT y, FLOAT z, Color color )
	{
		Vertex1P1D vert;
		vert.position.Set( x, y, z );
		vert.diffuse = color.dwColor;

		m_points.PushBack( vert );

		if ( m_points.GetSize() > m_VBNumVerts )
			PSX_SafeRelease( m_pVB );

		m_bNeedUpdate = TRUE;
	}

	void GUILine::ClearPoints( void )
	{
		m_points.Clear();
		//PSX_SafeRelease( m_pVB );
		m_bNeedUpdate = TRUE;
	}

	void GUILine::SetPoint( UINT index, FLOAT x, FLOAT y, FLOAT z, Color color )
	{
		if ( index >= m_points.GetSize() )
			return;

		m_points[index].position = Vector3( x, y, z );
		m_points[index].diffuse = color.dwColor;

		m_bNeedUpdate = TRUE;
	}

	EErrorCode::Type GUILine::Initialize( void )
	{
		String str;
		GraphicsResourceManager *pMan = GraphicsResourceManager::GetInstance();

		if ( m_pMaterial == PSX_NULL )
		{
			// Create material
			str.Format( "Material%p", this );
			m_pMaterial = pMan->CreateMaterial( str.GetBuffer() );
			m_pMaterial->SetRenderTechnique( pMan->FindRenderTechnique(COMMON_RT_2DLINE) );
		}

		PSX_SafeRelease( m_pVB );

		if ( m_points.GetSize() == 0 )
			return EErrorCode::OKAY;

		// Create vertex buffer
		Vertex1P1D *pVerts = new Vertex1P1D[m_points.GetSize()];
		VertexBufferDescription desc;
		desc.bufferType = EVertexBufferType::DYNAMIC;
		desc.primitiveType = EPrimitiveType::LINE_STRIP;
		desc.vertexSize = PSX_SizeOf( Vertex1P1D );
		desc.numVertices = m_points.GetSize();
		desc.numPrimitives = desc.numVertices - 1;
		desc.pData = pVerts;
		desc.pDecl = pMan->GetVertexDeclaration( COMMON_VD_1P1D );

		// We need to flip the y value
		for ( SIZE_T i = 0; i < m_points.GetSize(); ++i )
		{
			pVerts[i] = m_points[i];
			pVerts[i].position.y = -pVerts[i].position.y;
		}

		str.Format( "VertexBuffer%p", this );
		m_pVB = pMan->CreateVertexBuffer( str.GetBuffer(), &desc );

		PSX_SafeDeleteArray( desc.pData );

		if ( m_pVB == PSX_NULL )
			return EErrorCode::GRAPHICS;

		m_bNeedUpdate	= FALSE;
		m_VBNumVerts	= m_points.GetSize();

		return EErrorCode::OKAY;
	}

	void GUILine::UpdateVBVertices( void )
	{
		SIZE_T numPoints = m_points.GetSize();
		UINT i = 0;
		Vertex1P1D lastPoint;
		Vertex1P1D *pVerts;

		if ( numPoints == 0 )
			lastPoint.position = Vector3( FLT_MAX, FLT_MAX, FLT_MAX );
		else
			lastPoint = m_points[numPoints - 1];
		
		m_pVB->Lock( (void**)&pVerts );

		for ( ; i < numPoints; ++i )
		{
			pVerts[i] = m_points[i];
			pVerts[i].position.y = -pVerts[i].position.y;
		}

		for ( ; i < m_VBNumVerts; ++i )
		{
			pVerts[i] = lastPoint;
		}

		m_pVB->Unlock();

		m_bNeedUpdate = FALSE;
	}

	void GUILine::Bind( void )
	{
		if ( m_pVB == PSX_NULL )
			Initialize();
		else if ( m_bNeedUpdate )
			UpdateVBVertices();

		Base::Bind();
	}

}