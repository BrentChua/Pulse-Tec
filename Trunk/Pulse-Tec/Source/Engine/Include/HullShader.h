/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	HullShader.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_HULL_SHADER_H_
#define _PSX_HULL_SHADER_H_

#include "Shader.h"

namespace Pulse
{
	class HullShader : public Shader
	{
	public:

		virtual ~HullShader( void );

		virtual EShaderType::Type GetType( void ) { return EShaderType::HULL; }

		virtual void * GetNakedShader( void ) { return (void*)m_pHullShader; }

	private:

		HullShader( void );

		virtual EErrorCode::Type OnInitialize( void );

		virtual void OnCleanup( void );

	private:

		PSX_MakeStoragePoolFriend( HullShader );

		ID3D11HullShader *m_pHullShader;

	};
}

#endif /* _PSX_HULL_SHADER_H_ */