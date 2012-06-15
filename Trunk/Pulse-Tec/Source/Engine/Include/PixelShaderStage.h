/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PixelShaderStage.h
*
*	Description -	Pixel shader stage
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/30/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PIXEL_SHADER_STAGE_H_
#define _PSX_PIXEL_SHADER_STAGE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "IShaderStage.h"

namespace Pulse
{
	class PixelShaderStage : public IShaderStage
	{
	public:

		PixelShaderStage( void );

		virtual ~PixelShaderStage( void );

		virtual void BindShader( void );
		virtual void BindConstantBuffers( void );
		virtual void BindSamplerStates( void );
		virtual void BindShaderResourceViews( void );
	};
}

#endif /* _PSX_PIXEL_SHADER_STAGE_H_ */