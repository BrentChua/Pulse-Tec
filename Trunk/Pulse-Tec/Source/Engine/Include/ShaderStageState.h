/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ShaderStageState.h
*
*	Description -	Shader stage state cache
*
*	Comments	-	Base class.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/29/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_SHADER_STAGE_STATE_H_
#define _PSX_SHADER_STAGE_STATE_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "GraphicsInfrastructure.h"

namespace Pulse
{
	class ShaderStageState
	{
	public:

		void ClearStates( void );

	//private:

		// Shader stage cache
		Shader				*m_pShader;
		ConstantBuffer		*m_pConstantBuffers[PSX_NUM_CONSTANT_BUFFER_SLOT];
		SamplerState		*m_pSamplerStates[PSX_NUM_SAMPLER_SLOT];
		GraphicsResource	*m_pShaderResources[PSX_NUM_INPUT_RESOURCE_SLOT];

		ID3D11Buffer				*m_pD3DConstantBuffers[PSX_NUM_CONSTANT_BUFFER_SLOT];
		ID3D11SamplerState			*m_pD3DSamplerStates[PSX_NUM_SAMPLER_SLOT];
		ID3D11ShaderResourceView	*m_pD3DShaderResources[PSX_NUM_INPUT_RESOURCE_SLOT];
	};
}

#endif /* _PSX_SHADER_STAGE_STATE_H_ */