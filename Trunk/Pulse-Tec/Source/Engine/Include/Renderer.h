/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Renderer.h
*
*	Description -	Graphics Renderer.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDERER_H_
#define _PSX_RENDERER_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "GraphicsDevice.h"

namespace Pulse
{
	struct RendererDesc
	{
		
		void SetAsImmediate( ERendererProcessingType::Type	_processingType, UINT _adapter, UINT _adapterOutput, BOOL _bDebug,
			BOOL _bSoftwareRendererFallback, DisplayMode _displaySettings );
		
		void SetAsDeferred( void );

		ERendererProcessingType::Type	processingType;
		EDeviceContext::Type			deviceContextType;
		UINT							adapter;
		UINT							adapterOutput; // Device to output to such as a monitor.
		BOOL							bDebug;
		BOOL							bSoftwareRendererFallback;
		DisplayMode						displaySettings;
	};

	class Renderer
	{
	public:

		Renderer( void );

		virtual ~Renderer( void );

		EErrorCode::Type Initialize( RendererDesc  *pDesc );

		void Cleanup( void );

		GraphicsDevice * GetDevice( void ) { return m_pDevice; }

		void * GetNakedDevice( void ) { return m_pDevice->GetNakedDevice(); }

		EDeviceContext::Type GetDeviceContextType( void ) const { return m_desc.deviceContextType; }

		void * GetNakedDeviceContext( void ) { return (void*)m_pDeviceContext; }
		
		MappedResource MapResource( GraphicsBuffer *pBuffer, UINT subresourceIndex, EResourceMapping::Type map, UINT flags = 0 );

		void UnmapResource( GraphicsBuffer *pBuffer, UINT subresourceIndex );

	private:

		friend GraphicsPipeline;

		// Internal methods used by the graphics pipeline
		void DrawIndexed( UINT indexCount, UINT startIndexLocation, INT baseVertexLocation );

	private:

		RendererDesc		m_desc;
		GraphicsDevice		*m_pDevice;
		ID3D11DeviceContext *m_pDeviceContext; // This can be deferred or immedate
		SwapChain			*m_pSwapChain;
	};
}

#endif /* _PSX_RENDERER_H_ */