/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PixelShader.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PIXEL_SHADER_H_
#define _PSX_PIXEL_SHADER_H_

#include "Shader.h"

namespace Pulse
{
	class PixelShader : public Shader
	{
	public:

		virtual ~PixelShader( void );

		virtual EShaderType::Type GetType( void ) { return EShaderType::PIXEL; }

		virtual void * GetNakedShader( void ) { return (void*)m_pPixelShader; }

	private:

		PixelShader( void );

		virtual EErrorCode::Type OnInitialize( void );

		virtual void OnCleanup( void );

	private:

		PSX_MakeStoragePoolFriend( PixelShader );

		ID3D11PixelShader *m_pPixelShader;

	};
}

#endif /* _PSX_PIXEL_SHADER_H_ */