/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	DomainShader.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_DOMAIN_SHADER_H_
#define _PSX_DOMAIN_SHADER_H_

#include "Shader.h"

namespace Pulse
{
	class DomainShader : public Shader
	{
	public:

		virtual ~DomainShader( void );

		virtual EShaderType::Type GetType( void ) { return EShaderType::DOMAIN_; }

		virtual void * GetNakedShader( void ) { return (void*)m_pDomainShader; }

	private:

		DomainShader( void );

		virtual EErrorCode::Type OnInitialize( void );

		virtual void OnCleanup( void );

	private:

		PSX_MakeStoragePoolFriend(DomainShader);

		ID3D11DomainShader *m_pDomainShader;

	};
}

#endif /* _PSX_DOMAIN_SHADER_H_ */