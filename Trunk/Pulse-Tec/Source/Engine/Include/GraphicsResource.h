/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsResource.h
*
*	Description -	Base class containing resource views.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	4/30/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GRAPHICS_RESOURC_H_
#define _PSX_GRAPHICS_RESOURC_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "GraphicsInfrastructure.h"

namespace Pulse
{

	// NOTE: May have to be renamed to better reflect the resources 
	//	that has resource view.
	class GraphicsResource
	{
	public:

		GraphicsResource( void );

		virtual ~GraphicsResource( void );

		void * GetNakedShaderResource( void ) { return (void*)m_pSRV; }

		void * GetNakedRenderTargetResource( void ) { return (void*)m_pRTV; }
	
		void * GetNakedDepthStencilResource( void ) { return (void*)m_pDSV; }
	
	protected:

		void CreateResourceViews( ID3D11Resource *pResource, UINT BindFlags );

		void ReleaseResourceViews( void );

	protected:

		friend Renderer;

		ID3D11ShaderResourceView	*m_pSRV;
		ID3D11RenderTargetView		*m_pRTV;
		ID3D11DepthStencilView		*m_pDSV;
		ID3D11UnorderedAccessView	*m_pUAV;

	};

}

#endif /* _PSX_GRAPHICS_RESOURC_H_ */