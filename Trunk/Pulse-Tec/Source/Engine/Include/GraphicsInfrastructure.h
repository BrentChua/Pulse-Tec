/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsInfrastructure.h
*
*	Description -	Provides methods for enumerating graphics adapters and monitors,
*					enumerating display modes, choosing buffer formats.
*
*	Comments	-	The DXGI also allows sharing of resources between processes.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/07/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GRAPHICS_INFRASTRUCTURE_H_
#define _PSX_GRAPHICS_INFRASTRUCTURE_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "Singleton.h"
#include "COMPointer.h"

#include <dxgi.h>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <d3dx11.h>
#include <D3DX11core.h>
#include <D3D11Shader.h>
//#include <d3d9.h>

namespace Pulse
{
	typedef COMPointer<IDXGIFactory1> DXGIFactoryPtr;

	// TODO: Move these constants in a separate header file
	#define PSX_NUM_CONSTANT_BUFFER_SLOT	D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT
	#define PSX_NUM_SAMPLER_SLOT 			D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT
	#define PSX_NUM_INPUT_RESOURCE_SLOT		D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT
	#define PSX_NUM_VERTEX_INPUT_SLOT		D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT 
	#define PSX_NUM_RENDER_TARGET_SLOT		D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
	
	class GraphicsInfrastructure : public Singleton<GraphicsInfrastructure>
	{
	public:

		virtual ~GraphicsInfrastructure( void );

		const SIZE_T GetNumAdapters( void );

		const SIZE_T GetNumAdpaterOutputs( UINT adapter );

		EErrorCode::Type GetAvailableDisplayModes( UINT adapter, UINT adapterOutput, EGraphicsFormat::Type format, 
			DisplayModeList *pOut );

		EErrorCode::Type GetAvailableDisplayModes( UINT adapter, UINT adapterOutput, EGraphicsFormat::Type format, 
			FLOAT minWidth, FLOAT minHeight, FLOAT ratio, DisplayModeList *pOut );

		Vector2 GetCurrentResolution( UINT adapter = 0, UINT adapterOutput = 0 );

		static const UINT GetNumConstantBufferSlots( void ) { return PSX_NUM_CONSTANT_BUFFER_SLOT; }
		
		static const UINT GetNumSamplerStateSlots( void ) { return PSX_NUM_SAMPLER_SLOT; }

		static const UINT GetNumShaderResourceSlots( void ) { return PSX_NUM_INPUT_RESOURCE_SLOT; }

		// DX11 Specific
		DXGIFactoryPtr GetFactory( void ) { return m_pGIFactory; }

	private:

		friend Singleton;

		GraphicsInfrastructure( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		DXGIFactoryPtr m_pGIFactory;

	};
}

#endif / _PSX_GRAPHICS_INFRASTRUCTURE_H_ /