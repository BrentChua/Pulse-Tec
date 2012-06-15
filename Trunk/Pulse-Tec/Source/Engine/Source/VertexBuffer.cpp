/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	VertexBuffer.cpp
*
*	Comments	-	See VertexBuffer.h
*
**************************************************************************************/
#include "../Include/VertexBuffer.h"
#include "../Include/Engine.h"
#include "../Include/ResourceCache.h"

namespace Pulse
{
	VertexBufferDesc::VertexBufferDesc( void )
	{
		bufferType = EVertexBufferType::UNKNOWN;
		vertexSize = 0;
		numVertices = 0;
		numPrimitives = 0;
		pData = PSX_NULL;
	}

	void VertexBufferDesc::SetAsDynamicVertexBuffer( SIZE_T _vertexSize, SIZE_T _numVertices, void *pVertexData )
	{
		bufferType = EVertexBufferType::DYNAMIC;
		vertexSize = _vertexSize;
		numVertices = _numVertices;
		pData = pVertexData;
	}

	void VertexBufferDesc::SetAsStaticVertexBuffer( SIZE_T _vertexSize, SIZE_T _numVertices, void *pVertexData )
	{
		// pVertexData must be valid
		if ( !pVertexData )
			return;

		bufferType = EVertexBufferType::STATIC;
		vertexSize = _vertexSize;
		numVertices = _numVertices;
		pData = pVertexData;
	}

	VertexBuffer::VertexBuffer( void )
	{
	}

	VertexBuffer::~VertexBuffer( void )
	{

	}

	BOOL VertexBuffer::CreateResource( BaseResDesc *pDesc )
	{
		if ( pDesc->GetDescType() != EResDescType::VERTEX_BUFFER )
			return FALSE;

		m_desc = *((VertexBufferDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();
	}
	
	BOOL VertexBuffer::DestroyResource( void )
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}
	
	BOOL VertexBuffer::DisableResource( void )
	{
		SetFlagDisabled();
		return TRUE;	
	}
	
	BOOL VertexBuffer::RestoreResource( void )
	{
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type VertexBuffer::Initialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();

		HRESULT hr;
		D3D11_BUFFER_DESC desc;

		desc.ByteWidth = m_desc.vertexSize * m_desc.numVertices;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		
		if ( m_desc.bufferType == EVertexBufferType::DYNAMIC )
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.CPUAccessFlags = 0;
		}

		if ( m_desc.pData )
		{
			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = m_desc.pData;
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;

			hr = pDevice->CreateBuffer( &desc, &data, &m_pBuffer );
		}
		else
			hr = pDevice->CreateBuffer( &desc, PSX_NULL, &m_pBuffer );

		if ( FAILED( hr ) )
		{
			PSX_PushError( "Failed to create vertex buffer." );
			return EErrorCode::GRAPHICS;
		}
		
		CreateResourceViews( m_pBuffer, desc.BindFlags );

		// pData is only temporary
		m_desc.pData = PSX_NULL;

		return EErrorCode::OKAY;
	}

	void VertexBuffer::Cleanup( void )
	{
		ReleaseResourceViews();
		PSX_SafeRelease(m_pBuffer);
	}

}