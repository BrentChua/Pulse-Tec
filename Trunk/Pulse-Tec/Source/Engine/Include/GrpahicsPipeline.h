/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsPipeline.h
*
*	Description -	Manager for managing different stages of the graphics pipeline.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/19/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GRAPHICS_PIPELINE_H_
#define _PSX_GRAPHICS_PIPELINE_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "InputAssemblerStage.h"
#include "VertexShaderStage.h"
#include "PixelShaderStage.h"
#include "OutputMergerStage.h"

namespace Pulse
{

	class GraphicsPipeline
	{
	public:

		GraphicsPipeline( Renderer *pRenderer );

		~GraphicsPipeline( void );

		// Setup stages
		void SetInputAssemblerStage( InputAssemblerStage *pStage );
		void SetVertexShaderStage( VertexShaderStage *pStage );
		void SetPixelShaderStage( PixelShaderStage *pStage );
		void SetOutputMergerStage( OutputMergerStage *pStage );

		// Input Assembler
		void IASetInputLayout( InputLayout *pLayout );
		void IASetTopology( ETopology::Type topology );
		void IASetVertexBuffer( UINT slot, VertexBuffer *pVB, UINT offset = 0 );
		void IASetIndexBuffer( IndexBuffer *pIB, UINT offset = 0 );

		// Vertex Shader
		void VSSetShader( Shader *pShader );
		void VSSetConstantBuffer( UINT slot, ConstantBuffer *pConstantBuffer );
		void VSSetSamplerState( UINT slot, SamplerState *pSampler );
		void VSSetShaderResource( UINT slot, GraphicsResource *pGraphicsResource );

		// Pixel Shader
		void PSSetShader( Shader *pShader );
		void PSSetConstantBuffer( UINT slot, ConstantBuffer *pConstantBuffer );
		void PSSetSamplerState( UINT slot, SamplerState *pSampler );
		void PSSetShaderResource( UINT slot, GraphicsResource *pGraphicsResource );

		// Output Merger
		void OMSetBlendState( BlendState *pBlendState );
		void OMSetBlendFactors( FLOAT r, FLOAT g, FLOAT b, FLOAT a );
		void OMSetBlendFactors( const FLOAT *pBlendFactors );
		void OMSetSampleMask( const UINT sampleMask );
		void OMSetDepthStencilState( DepthStencilState *pDepthStencilState );
		void OMSetStencilRef( UINT ref );
		void OMSetDepthStencil( GraphicsResource *pRT );
		void OMSetRenderTarget( UINT slot, GraphicsResource *pRT );

		// Pipeline executions
		void Draw( Geometry *pGeometry );

	private:

		void BindStateChanges( void );

	private:

		void SetStageRenderer( IPipelineStage *pStage );

		Renderer *m_pRenderer;

		union
		{
			struct
			{
				InputAssemblerStage *m_pInputAssembler;
				VertexShaderStage	*m_pVertexShader;
				HullShaderStage		*m_pHullShader;
				TesellatorStage		*m_pTesellator;
				DomainShaderStage	*m_pDomainShader;
				GeometryShaderStage	*m_pGeometryShader;
				StreamOutputStage	*m_pStreamOutput;
				RasterizerStage		*m_pRasterizer;
				PixelShaderStage	*m_pPixelShader;
				OutputMergerStage	*m_pOutputMerger;
			};

			IPipelineStage *m_pStages[EPipelineStage::EnumCount];
		};
	};

	PSX_INLINE void GraphicsPipeline::IASetInputLayout( InputLayout *pLayout )
	{
		m_pInputAssembler->SetInputLayout( pLayout );
	}

	PSX_INLINE void GraphicsPipeline::IASetTopology( ETopology::Type topology )
	{
		m_pInputAssembler->SetTopology( topology );
	}

	PSX_INLINE void GraphicsPipeline::IASetVertexBuffer( UINT slot, VertexBuffer *pVB, UINT offset )
	{
		m_pInputAssembler->SetVertexBuffer( slot, pVB, offset );
	}

	PSX_INLINE void GraphicsPipeline::IASetIndexBuffer( IndexBuffer *pIB, UINT offset )
	{
		m_pInputAssembler->SetIndexBuffer( pIB, offset );
	}

	PSX_INLINE void GraphicsPipeline::VSSetShader( Shader *pShader )
	{
		m_pVertexShader->SetShader( pShader );
	}

	PSX_INLINE void GraphicsPipeline::VSSetConstantBuffer( UINT slot, ConstantBuffer *pConstantBuffer )
	{
		m_pVertexShader->SetConstantBuffer( slot, pConstantBuffer );
	}

	PSX_INLINE void GraphicsPipeline::VSSetSamplerState( UINT slot, SamplerState *pSampler )
	{
		m_pVertexShader->SetSamplerState( slot, pSampler );
	}

	PSX_INLINE void GraphicsPipeline::VSSetShaderResource( UINT slot, GraphicsResource *pGraphicsResource )
	{
		m_pVertexShader->SetShaderResource( slot, pGraphicsResource );
	}

	PSX_INLINE void GraphicsPipeline::PSSetShader( Shader *pShader )
	{
		m_pPixelShader->SetShader( pShader );
	}

	PSX_INLINE void GraphicsPipeline::PSSetConstantBuffer( UINT slot, ConstantBuffer *pConstantBuffer )
	{
		m_pPixelShader->SetConstantBuffer( slot, pConstantBuffer );
	}

	PSX_INLINE void GraphicsPipeline::PSSetSamplerState( UINT slot, SamplerState *pSampler )
	{
		m_pPixelShader->SetSamplerState( slot, pSampler );
	}

	PSX_INLINE void GraphicsPipeline::PSSetShaderResource( UINT slot, GraphicsResource *pGraphicsResource )
	{
		m_pPixelShader->SetShaderResource( slot, pGraphicsResource );
	}

	PSX_INLINE void GraphicsPipeline::OMSetBlendState( BlendState *pBlendState )
	{
		m_pOutputMerger->SetBlendState( pBlendState );
	}

	PSX_INLINE void GraphicsPipeline::OMSetBlendFactors( FLOAT r, FLOAT g, FLOAT b, FLOAT a )
	{
		m_pOutputMerger->SetBlendFactors( r, g, b, a );
	}

	PSX_INLINE void GraphicsPipeline::OMSetBlendFactors( const FLOAT *pBlendFactors )
	{
		m_pOutputMerger->SetBlendFactors( pBlendFactors );
	}

	PSX_INLINE void GraphicsPipeline::OMSetSampleMask( const UINT sampleMask )
	{
		m_pOutputMerger->SetSampleMask( sampleMask );
	}

	PSX_INLINE void GraphicsPipeline::OMSetDepthStencilState( DepthStencilState *pDepthStencilState )
	{
		m_pOutputMerger->SetDepthStencilState( pDepthStencilState );
	}

	PSX_INLINE void GraphicsPipeline::OMSetStencilRef( UINT ref )
	{
		m_pOutputMerger->SetStencilRef( ref );
	}

	PSX_INLINE void GraphicsPipeline::OMSetDepthStencil( GraphicsResource *pRT )
	{
		m_pOutputMerger->SetDepthStencil( pRT );
	}

	PSX_INLINE void GraphicsPipeline::OMSetRenderTarget( UINT slot, GraphicsResource *pRT )
	{
		m_pOutputMerger->SetRenderTarget( slot, pRT );
	}
}

#endif /* _PSX_GRAPHICS_PIPELINE_H_ */