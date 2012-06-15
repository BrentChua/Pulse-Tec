/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConstantBuffer.cpp
*
*	Comments	-	See ConstantBuffer.h
*
**************************************************************************************/
#include "../Include/ConstantBuffer.h"
#include "../Include/Engine.h"

namespace Pulse
{
	ConstantBufferDesc::ConstantBufferDesc( void )
	{
		bufferSize = 0;
		bufferUsage = EBufferUsage::UNKNOWN;
		bAutoUpdate = TRUE;
		pData = PSX_NULL;
	}

	void ConstantBufferDesc::SetAsDynamicConstantBuffer( SIZE_T _bufferSize, BOOL _bAutoUpdate )
	{
		bufferSize = _bufferSize;
		bAutoUpdate = _bAutoUpdate;
		bufferUsage = EBufferUsage::DYNAMIC;
	}

	void ConstantBufferDesc::SetAsStaticConstantBuffer( SIZE_T _bufferSize, void *pInitialData, BOOL _bAutoUpdate )
	{
		bufferSize = _bufferSize;
		bAutoUpdate = _bAutoUpdate;
		bufferUsage = EBufferUsage::STATIC;
		pData = pInitialData;
	}

	ConstantBuffer::ConstantBuffer( void )
	{
	}

	ConstantBuffer::~ConstantBuffer( void )
	{

	}

	BOOL ConstantBuffer::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::CONSTANT_BUFFER )
			return FALSE;

		if ( ((ConstantBufferDesc*)pDesc)->bufferUsage == EBufferUsage::STATIC &&
			((ConstantBufferDesc*)pDesc)->pData == PSX_NULL )
		{
			PSX_PushError( "Can't create static constant buffer without providing an initial data." );
			return FALSE;
		}

		m_desc = *((ConstantBufferDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL ConstantBuffer::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL ConstantBuffer::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL ConstantBuffer::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type ConstantBuffer::Initialize( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();

		HRESULT hr;
		D3D11_BUFFER_DESC desc;

		desc.ByteWidth				= m_desc.bufferSize;
		desc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
		desc.MiscFlags				= 0;
		desc.StructureByteStride	= 0;

		if ( m_desc.bufferUsage == EBufferUsage::DYNAMIC )
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

		m_desc.pData = PSX_NULL;

		if ( FAILED(hr) )
		{
			PSX_PushError( "Failed to create D3D11 constant buffer." );
			return EErrorCode::GRAPHICS;
		}

		CreateResourceViews( m_pBuffer, desc.BindFlags );

		return EErrorCode::OKAY;
	}

	void ConstantBuffer::Cleanup( void )
	{
		ReleaseResourceViews();
		ReleaseBuffer();
	}
}