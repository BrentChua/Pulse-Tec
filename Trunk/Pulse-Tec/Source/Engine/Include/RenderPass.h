/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RenderPass.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/05/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDER_PASS_H_
#define _PSX_RENDER_PASS_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "ResourcePoolItem.h"
#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "Array.h"

namespace Pulse
{
	class RenderPass
	{
	public:

		RenderPass( void );

		RenderPass( VertexShader *pVS, PixelShader *pPS );

		virtual ~RenderPass( void ); 

		void SetInputLayout( InputLayout *pInputLayout );

		void SetVertexShader( VertexShader *pShader );
		void SetVertexShader( Shader *pShader );

		void SetHullShader( HullShader *pShader );
		void SetHullShader( Shader *pShader );

		void SetDomainShader( DomainShader *pShader );
		void SetDomainShader( Shader *pShader );

		void SetGeometryShader( GeometryShader *pShader );
		void SetGeometryShader( Shader *pShader );

		void SetPixelShader( PixelShader *pShader );
		void SetPixelShader( Shader *pShader );

		void SetComputeShader( ComputeShader *pShader );
		void SetComputeShader( Shader *pShader );

		void Update( void );

	private:

		friend RenderEffect;

		void ClearShaders( bool bCheckRelease );

		void GenerateInputLayout( void );

	private:

		InputLayout *m_pInputLayout;

		union
		{
			Shader *m_pShaders[EShaderType::EnumCount];

			struct
			{
				VertexShader	*m_pVertexShader;
				HullShader		*m_pHullShader;
				DomainShader	*m_pDomainShader;
				GeometryShader	*m_pGeometryShader;
				PixelShader		*m_pPixelShader;
				ComputeShader	*m_pComputeShader;
			};
		};

	};
}

#endif /* _PSX_RENDER_PASS_H_ */