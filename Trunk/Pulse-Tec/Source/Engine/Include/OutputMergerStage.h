/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	OutputMerger.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/01/2012	-	Creation of this file
**************************************************************************************/
#ifndef PSX_OUTPUT_MERGER_STAGE_H_
#define PSX_OUTPUT_MERGER_STAGE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "IPipelineStage.h"

namespace Pulse
{
	struct OutputMergerState
	{
		void ClearState( void );

		BlendState			*m_pBlendState;
		FLOAT				m_blendFactors[4]; // rgba
		UINT				m_sampleMask;
		DepthStencilState	*m_pDepthStencilState;
		UINT				m_stencilRef;
		GraphicsResource	*m_pRenderTargets[PSX_NUM_RENDER_TARGET_SLOT];
		GraphicsResource	*m_pDepthStencil;
		
		ID3D11RenderTargetView *m_pD3DRenderTargets[PSX_NUM_RENDER_TARGET_SLOT];

	};

	class OutputMergerStage : public IPipelineStage
	{
	public:

		OutputMergerStage( void );

		virtual ~OutputMergerStage( void );

		void SetBlendState( BlendState *pBlendState );

		void SetBlendFactors( FLOAT r, FLOAT g, FLOAT b, FLOAT a );

		void SetBlendFactors( const FLOAT *pBlendFactors );

		void SetSampleMask( const UINT sampleMask );

		void SetDepthStencilState( DepthStencilState *pDepthStencilState );

		void SetStencilRef( UINT ref );

		void SetDepthStencil( GraphicsResource *pRT );

		void SetRenderTarget( UINT slot, GraphicsResource *pRT );

		virtual void BindStates( void );

		virtual void ClearStates( void );

	protected:

		void BindBlendState( void );

		void BindDepthStencilState( void );

		void BindRenderTargets( void );

	private:

		OutputMergerState m_desiredState;
		OutputMergerState m_currentState;

		BOOL	m_bBindBlendState;
		BOOL	m_bBindBlendFactors;
		BOOL	m_bBindSampleMask;
		BOOL	m_bBindDepthStencilState;
		BOOL	m_bBindStencilRef;
		BOOL	m_bBindDepthStencil;
		BOOL	m_bBindRenderTargets;

	};
}

#endif /* PSX_OUTPUT_MERGER_STAGE_H_ */