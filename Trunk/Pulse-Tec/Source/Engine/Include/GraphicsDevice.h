/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsDevice.h
*
*	Description -	Graphics Renderer Device managed the creation of graphics resources.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDERER_DEVICE_H_
#define _PSX_RENDERER_DEVICE_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "DX11Mapping.h"
#include "GraphicsInfrastructure.h"
#include "2D.h"
#include "Singleton.h"

namespace Pulse
{
	class GraphicsDevice : public Singleton<GraphicsDevice>
	{
	public:

		virtual ~GraphicsDevice( void );

		EErrorCode::Type Initialize( RendererDesc *pDesc );

		void Cleanup( void );

		void * GetNakedDevice( void ) { return (void*)m_pDevice; }

	private:

		GraphicsDevice( void );

		GraphicsDevice( GraphicsDevice & );

	private:

		friend Singleton<GraphicsDevice>;

		ID3D11Device *m_pDevice;
		ID3D11DeviceContext *m_pDeviceContext;

		SIZE_T m_UIWidth;
		SIZE_T m_UIHeight;

	};
}

#endif /* _PSX_RENDERER_DEVICE_H_ */