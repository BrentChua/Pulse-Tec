/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	VertexShader.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_VERTEX_SHADER_H_
#define _PSX_VERTEX_SHADER_H_

#include "Shader.h"

namespace Pulse
{
	class VertexShader : public Shader
	{
	public:

		virtual ~VertexShader( void );

		virtual EShaderType::Type GetType( void ) { return EShaderType::VERTEX; }

		virtual void * GetNakedShader( void ) { return (void*)m_pVertexShader; }

	private:

		VertexShader( void );

		virtual EErrorCode::Type OnInitialize( void );

		virtual void OnCleanup( void );

	private:

		PSX_MakeStoragePoolFriend( VertexShader );

		ID3D11VertexShader *m_pVertexShader;

	};
}

#endif /* _PSX_VERTEX_SHADER_H_ */