/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	VertexShaderStage.h
*
*	Description -	Vertex shader stage
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/29/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_VERTEX_SHADER_STAGE_H_
#define _PSX_VERTEX_SHADER_STAGE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "IShaderStage.h"

namespace Pulse
{
	class VertexShaderStage : public IShaderStage
	{
	public:

		VertexShaderStage( void );

		virtual ~VertexShaderStage( void );

		virtual void BindShader( void );
		virtual void BindConstantBuffers( void );
		virtual void BindSamplerStates( void );
		virtual void BindShaderResourceViews( void );
	};
}

#endif /* _PSX_VERTEX_SHADER_STAGE_H_ */