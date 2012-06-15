/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ComputeShader.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_COMPUTE_SHADER_H_
#define _PSX_COMPUTE_SHADER_H_

#include "Shader.h"

namespace Pulse
{
	class ComputeShader : public Shader
	{
	public:

		virtual ~ComputeShader( void );

		virtual EShaderType::Type GetType( void ) { return EShaderType::PIXEL; }

		virtual void * GetNakedShader( void ) { return (void*)m_pComputeShader; }

	private:

		ComputeShader( void );

		virtual EErrorCode::Type OnInitialize( void );

		virtual void OnCleanup( void );

	private:

		PSX_MakeStoragePoolFriend(ComputeShader);

		ID3D11ComputeShader *m_pComputeShader;

	};
}

#endif /* _PSX_COMPUTE_SHADER_H_ */