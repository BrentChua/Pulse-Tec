/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	InputAssembler.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/31/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_INPUT_ASSEMBLER_H_
#define _PSX_INPUT_ASSEMBLER_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "IPipelineStage.h"

namespace Pulse
{
	struct InputAssemblerState
	{

		void ClearStates( void );

		InputLayout				*m_pInputLayout;
		ETopology::Type			m_topology;

	};
	
	class InputAssemblerStage : public IPipelineStage
	{
	public:

		InputAssemblerStage( void );

		virtual ~InputAssemblerStage( void );

		void SetInputLayout( InputLayout *pLayout );

		void SetTopology( ETopology::Type topology );

		void SetVertexBuffer( UINT slot, VertexBuffer *pVB, UINT offset = 0 );

		void SetIndexBuffer( IndexBuffer *pIB, UINT offset = 0 );

		virtual void ClearStates( void );

		void BindTopology( void );

		void BindInputLayout( void );

		void BindVertexBuffer( void );

		void BindIndexBuffer( void );

	private:


		InputAssemblerState m_currentState;
		InputAssemblerState m_desiredState;

		ID3D11Buffer	*m_pD3DVertexBuffers[PSX_NUM_VERTEX_INPUT_SLOT];
		UINT			m_vertexStrides[PSX_NUM_VERTEX_INPUT_SLOT];
		UINT			m_vertexOffsets[PSX_NUM_VERTEX_INPUT_SLOT];
		IndexBuffer		*m_pIndexBuffer;
		UINT			m_indexOffset;
		
		BOOL m_bUpdateInputLayout;
		BOOL m_bUpdateTopology;
		BOOL m_bUpdateVertexBuffers;
	};
}

#endif /* _PSX_INPUT_ASSEMBLER_H_ */