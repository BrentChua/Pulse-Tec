/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IndexBuffer.cpp
*
*	Comments	-	See IndexBuffer.h
*
**************************************************************************************/
#include "../Include/IndexBuffer.h"
#include "../Include/Engine.h"

namespace Pulse
{
	IndexBufferDesc::IndexBufferDesc( void )
	{
		indexSize = 0;
		numIndices = 0;
		bufferUsage = EBufferUsage::UNKNOWN;
		pData = PSX_NULL;
	}

	void IndexBufferDesc::SetAsDynamicIndexBuffer( SIZE_T _numIndices, void *pInitialData )
	{
		indexSize		= numIndices <= PSX_USHORT_MAX ? sizeof(USHORT) : sizeof(DWORD);
		numIndices		= _numIndices;
		bufferUsage		= EBufferUsage::DYNAMIC;
		pData			= pInitialData;
	}

	void IndexBufferDesc::SetAsStaticIndexBuffer( SIZE_T _numIndices, void *pInitialData )
	{
		indexSize		= numIndices <= PSX_USHORT_MAX ? sizeof(USHORT) : sizeof(DWORD);
		numIndices		= _numIndices;
		bufferUsage		= EBufferUsage::STATIC;
		pData			= pInitialData;
	}

	IndexBuffer::IndexBuffer( void )
	{

	}

	IndexBuffer::~IndexBuffer( void )
	{

	}

	BOOL IndexBuffer::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::INDEX_BUFFER )
			return FALSE;

		if ( ((IndexBufferDesc*)pDesc)->bufferUsage == EBufferUsage::STATIC &&
			((IndexBufferDesc*)pDesc)->pData == PSX_NULL )
		{
			PSX_PushError( "Can't create static index buffer without providing an initial data." );
			return FALSE;
		}

		m_desc = *((IndexBufferDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}
	
	BOOL IndexBuffer::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}
	
	BOOL IndexBuffer::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}
	
	BOOL IndexBuffer::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type IndexBuffer::Initialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();

		HRESULT hr;
		D3D11_BUFFER_DESC desc;

		desc.ByteWidth = m_desc.indexSize * m_desc.numIndices;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		
		if ( m_desc.bufferUsage == EBufferUsage::STATIC )
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.CPUAccessFlags = 0;
		}
		else
		{
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = m_desc.pData;
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;
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

		if ( FAILED(hr) )
		{
			PSX_PushError( "Failed to create D3D11 index buffer." );
			return EErrorCode::GRAPHICS;
		}

		CreateResourceViews( m_pBuffer, desc.BindFlags );

		m_desc.pData = PSX_NULL;

		return EErrorCode::OKAY;
	}

	void IndexBuffer::Cleanup( void )
	{
		ReleaseResourceViews();
		PSX_SafeRelease( m_pBuffer );
	}

}