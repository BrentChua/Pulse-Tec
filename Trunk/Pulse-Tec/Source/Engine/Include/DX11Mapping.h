/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	DX11Mapping.h
*
*	Description -	Maps generic enumerations to DX11 enumerations.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/07/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_DX11_MAPPING_H_
#define _PSX_DX11_MAPPING_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"

#include <D3D11.h>
#include <D3Dcommon.h>
#include <DXGIFormat.h>

namespace Pulse
{
	class DX11Mapping
	{
	public:

		static D3D_DRIVER_TYPE ToDXProcessingType( ERendererProcessingType::Type type );

		static ERendererProcessingType::Type ToProcessingType( D3D_DRIVER_TYPE type );

		static DXGI_FORMAT ToDXBufferFormat( EGraphicsFormat::Type format );
		
		static EGraphicsFormat::Type ToBufferFormat( DXGI_FORMAT format );

		static D3D11_PRIMITIVE_TOPOLOGY ToDXTopology( ETopology::Type topology );

		static ESystemValueType::Type ToSystemValue( D3D_NAME systemValue );

		static ERegisterComponentType::Type ToRegisterComponent( D3D_REGISTER_COMPONENT_TYPE regCompType );

		static EShaderBufferType::Type ToShaderBufferType( D3D_CBUFFER_TYPE type );
	
		static EShaderVariableClass::Type ToShaderVariableClass( D3D_SHADER_VARIABLE_CLASS _class );
	
		static EShaderVariableType::Type ToShaderVariableType( D3D_SHADER_VARIABLE_TYPE type );
	
		static D3D11_MAP ToDXMapping( EResourceMapping::Type mapType );

		static D3D11_BLEND ToDXBlend( EBlendType::Type type );

		static D3D11_BLEND_OP ToDXBlendOp( EBlendOp::Type type );

		static BYTE ToDXWriteEnable( BYTE _EColorWriteEnable );

		static D3D11_DEPTH_WRITE_MASK ToDXDepthWriteMask( EBlendWriteMask::Type mask );

		static D3D11_COMPARISON_FUNC ToDXComparisonFunc( EComparisonFunc::Type func );

		static BYTE ToDXStencilReadMask( BYTE stencilMask );

		static BYTE ToDXStencilWriteMask( BYTE stencilMask );

		static D3D11_STENCIL_OP ToDXStencilOp( EStencilOp::Type op );

		static D3D11_INPUT_CLASSIFICATION ToDXInputClassification( EInputClassification::Type inputClass );

		static D3D11_FILL_MODE ToDXFillMode( EFillMode::Type fillMode );
		
		static D3D11_CULL_MODE ToDXCullMode( ECullMode::Type cullMode );
	
		static D3D11_FILTER ToDXFilter( EFilter::Type filter );

		static D3D11_TEXTURE_ADDRESS_MODE ToDXTextureAddressMode( ETextureAddressingMode::Type address );
	};
}

#endif /* _PSX_DX11_MAPPING_H_ */