/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsPipeline.cpp
*
*	Comments	-	See GraphicsPipeline.h
*
**************************************************************************************/
#include "../Include/GrpahicsPipeline.h"
#include "../Include/Renderer.h"
#include "../Include/Geometry.h"


namespace Pulse
{
	GraphicsPipeline::GraphicsPipeline( Renderer *pRenderer )
		: m_pRenderer( pRenderer )
	{

	}

	GraphicsPipeline::~GraphicsPipeline( void )
	{

	}

	void GraphicsPipeline::SetInputAssemblerStage( InputAssemblerStage *pStage )
	{
		m_pInputAssembler = pStage;
		SetStageRenderer( pStage );
	}

	void GraphicsPipeline::SetVertexShaderStage( VertexShaderStage *pStage )
	{
		m_pVertexShader = pStage;
		SetStageRenderer( pStage );
	}

	void GraphicsPipeline::SetPixelShaderStage( PixelShaderStage *pStage )
	{
		m_pPixelShader = pStage;
		SetStageRenderer( pStage );
	}

	void GraphicsPipeline::SetOutputMergerStage( OutputMergerStage *pStage )
	{
		m_pOutputMerger = pStage;
		SetStageRenderer( pStage );
	}

	void GraphicsPipeline::SetStageRenderer( IPipelineStage *pStage )
	{
		if ( pStage )
			pStage->SetRenderer( m_pRenderer );
	}

	void GraphicsPipeline::Draw( Geometry *pGeometry )
	{
		InputLayout *pIL = pGeometry->GetInputLayout();
		VertexBuffer *pVB = pGeometry->GetVertexBuffer();
		IndexBuffer *pIB = pGeometry->GetIndexBuffer();

		if ( pIL == PSX_NULL || pVB == PSX_NULL )
			return;

		SIZE_T numSubSets = pGeometry->GetNumSubsets();
		GeometrySubset *pSubset;
		Material *pMaterial;

		for ( SIZE_T i = 0; i < numSubSets; ++i )
		{
			pSubset = pGeometry->GetSubset(i);

			if ( pSubset->m_pMaterial == PSX_NULL )
				continue;

			//pMaterial->

		}

		//m_pRenderer->DrawIndexed(
	}

	void GraphicsPipeline::BindStateChanges( void )
	{

	}

}