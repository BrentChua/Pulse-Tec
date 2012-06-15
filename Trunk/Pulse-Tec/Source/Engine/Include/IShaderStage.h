/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IShaderStage.h
*
*	Description -	A customizable stage in the graphics pipeline that runs using shaders.
*
*	Comments	-	Base class.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/19/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_SHADER_STAGE_H_
#define _PSX_SHADER_STAGE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "IPipelineStage.h"
#include "ShaderStageState.h"

namespace Pulse
{
	class IShaderStage : public IPipelineStage
	{
	public:

		IShaderStage( void );

		virtual ~IShaderStage( void );

		void SetShader( Shader *pShader );
		void SetConstantBuffer( UINT slot, ConstantBuffer *pConstantBuffer );
		void SetSamplerState( UINT slot, SamplerState *pSampler );
		void SetShaderResource( UINT slot, GraphicsResource *pGraphicsResource );
		//void SetUnordererdAccess();

		virtual void ClearStates( void );

		virtual void BindShader( void ) { }
		virtual void BindConstantBuffers( void ) { }
		virtual void BindSamplerStates( void ) { }
		virtual void BindShaderResourceViews( void ) { }
		//virtual void BindUnordererdAccessViews( void ) { }

	protected:

		ShaderStageState m_stateCache;

	};
}

#endif /* _PSX_SHADER_STAGE_H_ */