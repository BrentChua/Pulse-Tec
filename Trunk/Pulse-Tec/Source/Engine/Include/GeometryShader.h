/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GeometryShader.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GEOMETRY_SHADER_H_
#define _PSX_GEOMETRY_SHADER_H_

#include "Shader.h"

namespace Pulse
{
	class GeometryShader : public Shader
	{
	public:

		virtual ~GeometryShader( void );

		virtual EShaderType::Type GetType( void ) { return EShaderType::GEOMETRY; }

		virtual void * GetNakedShader( void ) { return (void*)m_pGeometryShader; }

	private:

		GeometryShader( void );

		virtual EErrorCode::Type OnInitialize( void );

		virtual void OnCleanup( void );

	private:

		PSX_MakeStoragePoolFriend( GeometryShader );

		ID3D11GeometryShader *m_pGeometryShader;

	};
}

#endif /* _PSX_GEOMETRY_SHADER_H_ */