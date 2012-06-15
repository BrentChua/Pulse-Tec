/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	InputAssemblerStage.cpp
*
*	Comments	-	See InputAssemblerStage.h
*
**************************************************************************************/
#include "../Include/InputAssemblerStage.h"
#include "../Include/Renderer.h"
#include "../Include/VertexBuffer.h"
#include "../Include/IndexBuffer.h"
#include "../Include/InputLayout.h"
#include "../Include/DX11Mapping.h"

namespace Pulse
{

	void InputAssemblerState::ClearStates( void )
	{
		m_pInputLayout = PSX_NULL;
		m_topology = ETopology::UNKNOWN;
	}

	InputAssemblerStage::InputAssemblerStage( void )
		: IPipelineStage()
	{
		ClearStates();
	}

	InputAssemblerStage::~InputAssemblerStage( void )
	{

	}

	void InputAssemblerStage::SetInputLayout( InputLayout *pLayout )
	{
		if ( m_currentState.m_pInputLayout != pLayout )
		{
			m_desiredState.m_pInputLayout = pLayout; 
			m_bUpdateInputLayout = TRUE;
		}
		else
			m_bUpdateInputLayout = FALSE;
	}

	void InputAssemblerStage::SetTopology( ETopology::Type topology )
	{
		if ( m_currentState.m_topology != topology )
		{
			m_desiredState.m_topology = topology;
			m_bUpdateTopology = TRUE;
		}
		else
			m_bUpdateTopology = FALSE;
	}

	void InputAssemblerStage::SetVertexBuffer( UINT slot, VertexBuffer *pVB, UINT offset )
	{
		m_pD3DVertexBuffers[slot]	= (ID3D11Buffer*)pVB->GetNakedBuffer();
		m_vertexStrides[slot]		= pVB->GetDescription()->vertexSize;
		m_vertexOffsets[slot]		= offset;
		m_bUpdateVertexBuffers		= TRUE;
	}

	void InputAssemblerStage::SetIndexBuffer( IndexBuffer *pIB, UINT offset )
	{ 
		m_pIndexBuffer = pIB;
		m_indexOffset = offset;
	}

	void InputAssemblerStage::ClearStates( void )
	{
		m_currentState.ClearStates();
		m_desiredState.ClearStates();

		PSX_ZeroMem( m_pD3DVertexBuffers, sizeof(m_pD3DVertexBuffers) );
		PSX_ZeroMem( m_vertexStrides, sizeof(m_vertexStrides) );
		PSX_ZeroMem( m_vertexOffsets, sizeof(m_vertexOffsets) );
		m_pIndexBuffer = PSX_NULL;
		m_indexOffset = 0;

		m_bUpdateInputLayout = TRUE;
		m_bUpdateTopology = TRUE;
		m_bUpdateVertexBuffers = TRUE;
	}

	void InputAssemblerStage::BindTopology( void )
	{
		if ( m_bUpdateTopology == FALSE )
			return;

		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		pDC->IASetPrimitiveTopology( DX11Mapping::ToDXTopology(m_desiredState.m_topology) );

		m_currentState.m_topology = m_desiredState.m_topology;
		m_bUpdateTopology = FALSE;
	}

	void InputAssemblerStage::BindInputLayout( void )
	{
		if ( m_bUpdateInputLayout == FALSE )
			return;

		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		pDC->IASetInputLayout( (ID3D11InputLayout*)m_desiredState.m_pInputLayout->GetNakedInputLayout() );
	
		m_currentState.m_pInputLayout = m_desiredState.m_pInputLayout;
		m_bUpdateInputLayout = FALSE;
	}

	void InputAssemblerStage::BindVertexBuffer( void )
	{
		if ( m_bUpdateVertexBuffers )
		{
			ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
			pDC->IASetVertexBuffers( 0, PSX_NUM_VERTEX_INPUT_SLOT, (ID3D11Buffer**)m_pD3DVertexBuffers, m_vertexStrides, m_vertexOffsets ); 
			m_bUpdateVertexBuffers = FALSE;
		}
	}

	void InputAssemblerStage::BindIndexBuffer( void )
	{
		ID3D11DeviceContext *pDC = (ID3D11DeviceContext*)m_pRenderer->GetNakedDeviceContext();
		DXGI_FORMAT indexFormat;
		
		if ( m_pIndexBuffer->GetDescription()->indexSize == 2 )
			indexFormat = DXGI_FORMAT_R16_UINT;
		else
			indexFormat = DXGI_FORMAT_R32_UINT;

		pDC->IASetIndexBuffer( (ID3D11Buffer*)m_pIndexBuffer->GetNakedBuffer(), indexFormat, m_indexOffset );
	}
}
