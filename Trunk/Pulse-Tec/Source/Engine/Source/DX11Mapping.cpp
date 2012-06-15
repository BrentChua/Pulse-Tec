/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	DX11Mapping.cpp
*
*	Comments	-	See DX11Mapping.h
*
**************************************************************************************/

#ifndef __PSX_WINDOWS_CONSOLE

#include "../Include/DX11Mapping.h"

namespace Pulse
{
	D3D_DRIVER_TYPE DX11Mapping::ToDXProcessingType( ERendererProcessingType::Type type )
	{
		switch ( type )
		{
		case ERendererProcessingType::HARDWARE:
			return D3D_DRIVER_TYPE_HARDWARE;
		case ERendererProcessingType::SOFTWARE:
			return D3D_DRIVER_TYPE_REFERENCE;
		default:
			return D3D_DRIVER_TYPE_UNKNOWN;
		}
	}

	ERendererProcessingType::Type DX11Mapping::ToProcessingType( D3D_DRIVER_TYPE type )
	{
		switch ( type )
		{
		case D3D_DRIVER_TYPE_HARDWARE:
			return ERendererProcessingType::HARDWARE;
		case D3D_DRIVER_TYPE_REFERENCE:
			return ERendererProcessingType::SOFTWARE;
		default:
			return ERendererProcessingType::UNKNOWN;
		}
	}

	EGraphicsFormat::Type DX11Mapping::ToBufferFormat( DXGI_FORMAT format )
	{
		return (EGraphicsFormat::Type)format;
	}

	D3D11_PRIMITIVE_TOPOLOGY DX11Mapping::ToDXTopology( ETopology::Type topology )
	{
		switch( topology )
		{
			case ETopology::UNKNOWN:
				return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
			case ETopology::POINT_LIST:
				return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
			case ETopology::LINE_LIST:
				return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
			case ETopology::LINE_STRIP:
				return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case ETopology::TRIANGLE_LIST:
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case ETopology::TRIANGLE_STRIP:
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			default:
				PSX_Assert( FALSE, "Undefined topology type." );
		}

		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}

	ESystemValueType::Type DX11Mapping::ToSystemValue( D3D_NAME systemValue )
	{
		return (ESystemValueType::Type)systemValue;
	}

	ERegisterComponentType::Type DX11Mapping::ToRegisterComponent( D3D_REGISTER_COMPONENT_TYPE regCompType )
	{
		return (ERegisterComponentType::Type)regCompType;
	}

	EShaderBufferType::Type DX11Mapping::ToShaderBufferType( D3D_CBUFFER_TYPE type )
	{
		return (EShaderBufferType::Type)type;
	}

	EShaderVariableClass::Type DX11Mapping::ToShaderVariableClass( D3D_SHADER_VARIABLE_CLASS _class )
	{
		return (EShaderVariableClass::Type)_class;
	}

	EShaderVariableType::Type DX11Mapping::ToShaderVariableType( D3D_SHADER_VARIABLE_TYPE type )
	{
		return (EShaderVariableType::Type)type;
	}

	D3D11_MAP DX11Mapping::ToDXMapping( EResourceMapping::Type mapType )
	{
		switch( mapType )
		{
		case EResourceMapping::READ:
			return D3D11_MAP_READ;
		case EResourceMapping::WRITE:
			return D3D11_MAP_WRITE;
		case EResourceMapping::WRITE_DISCARD:
			return D3D11_MAP_WRITE_DISCARD;
		case EResourceMapping::WRITE_NO_OVERWRITE:
			return D3D11_MAP_WRITE_NO_OVERWRITE;
		default:
			PSX_PushError( "Unknown EResourceMapping type." );
		}

		return D3D11_MAP_WRITE_DISCARD;
	}

	D3D11_BLEND DX11Mapping::ToDXBlend( EBlendType::Type type )
	{
		switch( type )
		{
		case EBlendType::ZERO:
			return D3D11_BLEND_ZERO;
		case EBlendType::ONE:
			return D3D11_BLEND_ONE;
		case EBlendType::SRC_COLOR:
			return D3D11_BLEND_SRC_COLOR;
		case EBlendType::INV_SRC_COLOR:
			return D3D11_BLEND_INV_SRC_COLOR;
		case EBlendType::SRC_ALPHA:
			return D3D11_BLEND_SRC_ALPHA;
		case EBlendType::INV_SRC_ALPHA:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case EBlendType::DEST_ALPHA:
			return D3D11_BLEND_DEST_ALPHA;
		case EBlendType::INV_DEST_ALPHA:
			return D3D11_BLEND_INV_DEST_ALPHA;
		case EBlendType::DEST_COLOR:
			return D3D11_BLEND_DEST_COLOR;
		case EBlendType::INV_DEST_COLOR:
			return D3D11_BLEND_INV_DEST_COLOR;
		case EBlendType::SRC_ALPHA_SAT:
			return D3D11_BLEND_SRC_ALPHA_SAT;
		case EBlendType::BLEND_FACTOR:
			return D3D11_BLEND_BLEND_FACTOR;
		case EBlendType::INV_BLEND_FACTOR:
			return D3D11_BLEND_INV_BLEND_FACTOR;
		case EBlendType::SRC1_COLOR:
			return D3D11_BLEND_SRC1_COLOR;
		case EBlendType::INV_SRC1_COLOR:
			return D3D11_BLEND_INV_SRC1_COLOR;
		case EBlendType::SRC1_ALPHA:
			return D3D11_BLEND_SRC1_ALPHA;
		case EBlendType::INV_SRC1_ALPHA:
			return D3D11_BLEND_INV_SRC1_ALPHA;
		default:
			PSX_PushError( "Unknown EBLendType." );
		}

		return D3D11_BLEND_ZERO;
	}

	D3D11_BLEND_OP DX11Mapping::ToDXBlendOp( EBlendOp::Type type )
	{
		switch( type )
		{
		case EBlendOp::ADD:
			return D3D11_BLEND_OP_ADD;
		case EBlendOp::SUBTRACT:
			return D3D11_BLEND_OP_SUBTRACT;
		case EBlendOp::REV_SUBTRACT:
			return D3D11_BLEND_OP_REV_SUBTRACT;
		case EBlendOp::MIN:
			return D3D11_BLEND_OP_MIN;
		case EBlendOp::MAX:
			return D3D11_BLEND_OP_MAX;
		default:
			PSX_PushError( "Unknown EBlendOp type." );
		}

		return D3D11_BLEND_OP_ADD;
	}

	BYTE DX11Mapping::ToDXWriteEnable( BYTE _EColorWriteEnable )
	{
		BYTE ret = 0;

		if ( _EColorWriteEnable == EColorWriteEnable::ALL )
			return D3D11_COLOR_WRITE_ENABLE_ALL;

		if (  _EColorWriteEnable & EColorWriteEnable::RED )
			ret |= D3D11_COLOR_WRITE_ENABLE_RED;

		if (  _EColorWriteEnable & EColorWriteEnable::GREEN )
			ret |= D3D11_COLOR_WRITE_ENABLE_GREEN;

		if (  _EColorWriteEnable & EColorWriteEnable::BLUE )
			ret |= D3D11_COLOR_WRITE_ENABLE_BLUE;

		if (  _EColorWriteEnable & EColorWriteEnable::ALPHA )
			ret |= D3D11_COLOR_WRITE_ENABLE_ALPHA;

		return ret;
	}

	D3D11_DEPTH_WRITE_MASK DX11Mapping::ToDXDepthWriteMask( EBlendWriteMask::Type mask )
	{
		switch( mask )
		{
		case EBlendWriteMask::ZERO:
			return D3D11_DEPTH_WRITE_MASK_ZERO;
		case EBlendWriteMask::ALL:
			return D3D11_DEPTH_WRITE_MASK_ALL;
		default:
			PSX_PushError( "Invalid mask type." );
		}

		return D3D11_DEPTH_WRITE_MASK_ALL;
	}

	D3D11_COMPARISON_FUNC DX11Mapping::ToDXComparisonFunc( EComparisonFunc::Type func )
	{
		switch( func )
		{
		case EComparisonFunc::NEVER:
			return D3D11_COMPARISON_NEVER;
		case EComparisonFunc::LESS:
			return D3D11_COMPARISON_LESS;
		case EComparisonFunc::EQUAL:
			return D3D11_COMPARISON_EQUAL;
		case EComparisonFunc::LESS_EQUAL:
			return D3D11_COMPARISON_LESS_EQUAL;
		case EComparisonFunc::GREATER:
			return D3D11_COMPARISON_GREATER ;
		case EComparisonFunc::NOT_EQUAL:
			return D3D11_COMPARISON_NOT_EQUAL;
		case EComparisonFunc::GREATER_EQUAL:
			return D3D11_COMPARISON_GREATER_EQUAL;
		case EComparisonFunc::ALWAYS:
			return D3D11_COMPARISON_ALWAYS;
		default:
			PSX_PushError( "Invalid comparison function type." );
		}

		return D3D11_COMPARISON_NEVER;
	}

	BYTE DX11Mapping::ToDXStencilReadMask( BYTE stencilMask )
	{
		BYTE ret = 0;

		if ( stencilMask == EStencilReadMask::DEFAULT )
			ret = D3D11_DEFAULT_STENCIL_READ_MASK;
		else
			PSX_PushError( "Invalid stencilMask." );

		return ret;
	}

	BYTE DX11Mapping::ToDXStencilWriteMask( BYTE stencilMask )
	{
		BYTE ret = 0;

		if ( stencilMask == EStencilWriteMask::DEFAULT )
			ret = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		else
			PSX_PushError( "Invalid stencilMask." );

		return ret;
	}

	D3D11_STENCIL_OP DX11Mapping::ToDXStencilOp( EStencilOp::Type op )
	{
		switch( op )
		{
		case EStencilOp::KEEP:
			return D3D11_STENCIL_OP_KEEP;
		case EStencilOp::ZERO:
			return D3D11_STENCIL_OP_ZERO;
		case EStencilOp::REPLACE:
			return D3D11_STENCIL_OP_REPLACE;
		case EStencilOp::INCR_SAT:
			return D3D11_STENCIL_OP_INCR_SAT;
		case EStencilOp::DECR_SAT:
			return D3D11_STENCIL_OP_DECR_SAT;
		case EStencilOp::INVERT:
			return D3D11_STENCIL_OP_INVERT;
		case EStencilOp::INCR:
			return D3D11_STENCIL_OP_INCR;
		case EStencilOp::DECR:
			return D3D11_STENCIL_OP_DECR;
		default:
			PSX_PushError( "Unknown stencil operation type." );
		}

		return D3D11_STENCIL_OP_KEEP;
	}

	DXGI_FORMAT DX11Mapping::ToDXBufferFormat( EGraphicsFormat::Type format )
	{
		switch( format )
		{
		case EGraphicsFormat::UNKNOWN:
			case EGraphicsFormat::R32G32B32A32_TYPELESS:
				return DXGI_FORMAT_R32G32B32A32_TYPELESS;
			case EGraphicsFormat::R32G32B32A32_FLOAT:
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case EGraphicsFormat::R32G32B32A32_UINT:
				return DXGI_FORMAT_R32G32B32A32_UINT;
			case EGraphicsFormat::R32G32B32A32_SINT:			
				return DXGI_FORMAT_R32G32B32A32_SINT;
			case EGraphicsFormat::R32G32B32_TYPELESS:			
				return DXGI_FORMAT_R32G32B32_TYPELESS;
			case EGraphicsFormat::R32G32B32_FLOAT:			
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case EGraphicsFormat::R32G32B32_UINT:			
				return DXGI_FORMAT_R32G32B32_UINT;
			case EGraphicsFormat::R32G32B32_SINT:			
				return DXGI_FORMAT_R32G32B32_SINT;
			case EGraphicsFormat::R16G16B16A16_TYPELESS:			
				return DXGI_FORMAT_R16G16B16A16_TYPELESS;
			case EGraphicsFormat::R16G16B16A16_FLOAT:			
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case EGraphicsFormat::R16G16B16A16_UNORM:			
				return DXGI_FORMAT_R16G16B16A16_UNORM;
			case EGraphicsFormat::R16G16B16A16_UINT:			
				return DXGI_FORMAT_R16G16B16A16_UINT;
			case EGraphicsFormat::R16G16B16A16_SNORM:			
				return DXGI_FORMAT_R16G16B16A16_SNORM;
			case EGraphicsFormat::R16G16B16A16_SINT:			
				return DXGI_FORMAT_R16G16B16A16_SINT;
			case EGraphicsFormat::R32G32_TYPELESS:			
				return DXGI_FORMAT_R32G32_TYPELESS;
			case EGraphicsFormat::R32G32_FLOAT:			
				return DXGI_FORMAT_R32G32_FLOAT;
			case EGraphicsFormat::R32G32_UINT:			
				return DXGI_FORMAT_R32G32_UINT;
			case EGraphicsFormat::R32G32_SINT:			
				return DXGI_FORMAT_R32G32_SINT;
			case EGraphicsFormat::R32G8X24_TYPELESS:			
				return DXGI_FORMAT_R32G8X24_TYPELESS;
			case EGraphicsFormat::D32_FLOAT_S8X24_UINT:			
				return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			case EGraphicsFormat::R32_FLOAT_X8X24_TYPELESS:			
				return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			case EGraphicsFormat::X32_TYPELESS_G8X24_UINT:			
				return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
			case EGraphicsFormat::R10G10B10A2_TYPELESS:			
				return DXGI_FORMAT_R10G10B10A2_TYPELESS;
			case EGraphicsFormat::R10G10B10A2_UNORM:			
				return DXGI_FORMAT_R10G10B10A2_UNORM;
			case EGraphicsFormat::R10G10B10A2_UINT:			
				return DXGI_FORMAT_R10G10B10A2_UINT;
			case EGraphicsFormat::R11G11B10_FLOAT:			
				return DXGI_FORMAT_R11G11B10_FLOAT;
			case EGraphicsFormat::R8G8B8A8_TYPELESS:			
				return DXGI_FORMAT_R8G8B8A8_TYPELESS;
			case EGraphicsFormat::R8G8B8A8_UNORM:			
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case EGraphicsFormat::R8G8B8A8_UNORM_SRGB:			
				return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			case EGraphicsFormat::R8G8B8A8_UINT:			
				return DXGI_FORMAT_R8G8B8A8_UINT;
			case EGraphicsFormat::R8G8B8A8_SNORM:			
				return DXGI_FORMAT_R8G8B8A8_SNORM;
			case EGraphicsFormat::R8G8B8A8_SINT:			
				return DXGI_FORMAT_R8G8B8A8_SINT;
			case EGraphicsFormat::R16G16_TYPELESS:			
				return DXGI_FORMAT_R16G16_TYPELESS;
			case EGraphicsFormat::R16G16_FLOAT:			
				return DXGI_FORMAT_R16G16_FLOAT;
			case EGraphicsFormat::R16G16_UNORM:			
				return DXGI_FORMAT_R16G16_UNORM;
			case EGraphicsFormat::R16G16_UINT:			
				return DXGI_FORMAT_R16G16_UINT;
			case EGraphicsFormat::R16G16_SNORM:			
				return DXGI_FORMAT_R16G16_SNORM;
			case EGraphicsFormat::R16G16_SINT:			
				return DXGI_FORMAT_R16G16_SINT;
			case EGraphicsFormat::R32_TYPELESS:			
				return DXGI_FORMAT_R32_TYPELESS;
			case EGraphicsFormat::D32_FLOAT:			
				return DXGI_FORMAT_D32_FLOAT;
			case EGraphicsFormat::R32_FLOAT:			
				return DXGI_FORMAT_R32_FLOAT;
			case EGraphicsFormat::R32_UINT:			
				return DXGI_FORMAT_R32_UINT;
			case EGraphicsFormat::R32_SINT:			
				return DXGI_FORMAT_R32_SINT;
			case EGraphicsFormat::R24G8_TYPELESS:			
				return DXGI_FORMAT_R24G8_TYPELESS;
			case EGraphicsFormat::D24_UNORM_S8_UINT:			
				return DXGI_FORMAT_D24_UNORM_S8_UINT;
			case EGraphicsFormat::R24_UNORM_X8_TYPELESS:			
				return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			case EGraphicsFormat::X24_TYPELESS_G8_UINT:			
				return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
			case EGraphicsFormat::R8G8_TYPELESS:			
				return DXGI_FORMAT_R8G8_TYPELESS;
			case EGraphicsFormat::R8G8_UNORM:			
				return DXGI_FORMAT_R8G8_UNORM;
			case EGraphicsFormat::R8G8_UINT:			
				return DXGI_FORMAT_R8G8_UINT;
			case EGraphicsFormat::R8G8_SNORM:			
				return DXGI_FORMAT_R8G8_SNORM;
			case EGraphicsFormat::R8G8_SINT:			
				return DXGI_FORMAT_R8G8_SINT;
			case EGraphicsFormat::R16_TYPELESS:			
				return DXGI_FORMAT_R16_TYPELESS;
			case EGraphicsFormat::R16_FLOAT:			
				return DXGI_FORMAT_R16_FLOAT;
			case EGraphicsFormat::D16_UNORM:			
				return DXGI_FORMAT_D16_UNORM;
			case EGraphicsFormat::R16_UNORM:			
				return DXGI_FORMAT_R16_UNORM;
			case EGraphicsFormat::R16_UINT:			
				return DXGI_FORMAT_R16_UINT;
			case EGraphicsFormat::R16_SNORM:			
				return DXGI_FORMAT_R16_SNORM;
			case EGraphicsFormat::R16_SINT:			
				return DXGI_FORMAT_R16_SINT;
			case EGraphicsFormat::R8_TYPELESS:			
				return DXGI_FORMAT_R8_TYPELESS;
			case EGraphicsFormat::R8_UNORM:			
				return DXGI_FORMAT_R8_UNORM;
			case EGraphicsFormat::R8_UINT:			
				return DXGI_FORMAT_R8_UINT;
			case EGraphicsFormat::R8_SNORM:			
				return DXGI_FORMAT_R8_SNORM;
			case EGraphicsFormat::R8_SINT:			
				return DXGI_FORMAT_R8_SINT;
			case EGraphicsFormat::A8_UNORM:			
				return DXGI_FORMAT_A8_UNORM;
			case EGraphicsFormat::R1_UNORM:			
				return DXGI_FORMAT_R1_UNORM;
			case EGraphicsFormat::R9G9B9E5_SHAREDEXP:			
				return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
			case EGraphicsFormat::R8G8_B8G8_UNORM:			
				return DXGI_FORMAT_R8G8_B8G8_UNORM;
			case EGraphicsFormat::G8R8_G8B8_UNORM:			
				return DXGI_FORMAT_G8R8_G8B8_UNORM;
			case EGraphicsFormat::BC1_TYPELESS:			
				return DXGI_FORMAT_BC1_TYPELESS;
			case EGraphicsFormat::BC1_UNORM:			
				return DXGI_FORMAT_BC1_UNORM;
			case EGraphicsFormat::BC1_UNORM_SRGB:			
				return DXGI_FORMAT_BC1_UNORM_SRGB;
			case EGraphicsFormat::BC2_TYPELESS:			
				return DXGI_FORMAT_BC2_TYPELESS;
			case EGraphicsFormat::BC2_UNORM:			
				return DXGI_FORMAT_BC2_UNORM;
			case EGraphicsFormat::BC2_UNORM_SRGB:			
				return DXGI_FORMAT_BC2_UNORM_SRGB;
			case EGraphicsFormat::BC3_TYPELESS:			
				return DXGI_FORMAT_BC3_TYPELESS;
			case EGraphicsFormat::BC3_UNORM:			
				return DXGI_FORMAT_BC3_UNORM;
			case EGraphicsFormat::BC3_UNORM_SRGB:			
				return DXGI_FORMAT_BC3_UNORM_SRGB;
			case EGraphicsFormat::BC4_TYPELESS:			
				return DXGI_FORMAT_BC4_TYPELESS;
			case EGraphicsFormat::BC4_UNORM:			
				return DXGI_FORMAT_BC4_UNORM;
			case EGraphicsFormat::BC4_SNORM:			
				return DXGI_FORMAT_BC4_SNORM;
			case EGraphicsFormat::BC5_TYPELESS:			
				return DXGI_FORMAT_BC5_TYPELESS;
			case EGraphicsFormat::BC5_UNORM:			
				return DXGI_FORMAT_BC5_UNORM;
			case EGraphicsFormat::BC5_SNORM:			
				return DXGI_FORMAT_BC5_SNORM;
			case EGraphicsFormat::B5G6R5_UNORM:			
				return DXGI_FORMAT_B5G6R5_UNORM;
			case EGraphicsFormat::B5G5R5A1_UNORM:			
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			case EGraphicsFormat::B8G8R8A8_UNORM:			
				return DXGI_FORMAT_B8G8R8A8_UNORM;
			case EGraphicsFormat::B8G8R8X8_UNORM:			
				return DXGI_FORMAT_B8G8R8X8_UNORM;
			case EGraphicsFormat::R10G10B10_XR_BIAS_A2_UNORM:			
				return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
			case EGraphicsFormat::B8G8R8A8_TYPELESS:			
				return DXGI_FORMAT_B8G8R8A8_TYPELESS;
			case EGraphicsFormat::B8G8R8A8_UNORM_SRGB:			
				return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
			case EGraphicsFormat::B8G8R8X8_TYPELESS:			
				return DXGI_FORMAT_B8G8R8X8_TYPELESS;
			case EGraphicsFormat::B8G8R8X8_UNORM_SRGB:			
				return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
			case EGraphicsFormat::BC6H_TYPELESS:			
				return DXGI_FORMAT_BC6H_TYPELESS;
			case EGraphicsFormat::BC6H_UF16:			
				return DXGI_FORMAT_BC6H_UF16;
			case EGraphicsFormat::BC6H_SF16:			
				return DXGI_FORMAT_BC6H_SF16;
			case EGraphicsFormat::BC7_TYPELESS:			
				return DXGI_FORMAT_BC7_TYPELESS;
			case EGraphicsFormat::BC7_UNORM:			
				return DXGI_FORMAT_BC7_UNORM;
			case EGraphicsFormat::BC7_UNORM_SRGB:			
				return DXGI_FORMAT_BC7_UNORM_SRGB;

				// NOTE: Not implemented in the current (June 2010?) SDK yet.
			/*case EGraphicsFormat::AYUV:			
				return DXGI_FORMAT_AYUV;
			case EGraphicsFormat::Y410:			
				return DXGI_FORMAT_Y410;
			case EGraphicsFormat::Y416:			
				return DXGI_FORMAT_Y416;
			case EGraphicsFormat::NV12:			
				return DXGI_FORMAT_NV12;
			case EGraphicsFormat::P010:			
				return DXGI_FORMAT_P010;
			case EGraphicsFormat::P016:			
				return DXGI_FORMAT_P016;
			case EGraphicsFormat::_420_OPAQUE:			
				return DXGI_FORMAT_420_OPAQUE;
			case EGraphicsFormat::YUY2:			
				return DXGI_FORMAT_YUY2;
			case EGraphicsFormat::Y210:			
				return DXGI_FORMAT_Y210;
			case EGraphicsFormat::Y216:			
				return DXGI_FORMAT_Y216;
			case EGraphicsFormat::NV11:			
				return DXGI_FORMAT_NV11;
			case EGraphicsFormat::AI44:			
				return DXGI_FORMAT_AI44;
			case EGraphicsFormat::IA44:			
				return DXGI_FORMAT_IA44;
			case EGraphicsFormat::P8:			
				return DXGI_FORMAT_P8;
			case EGraphicsFormat::A8P8:			
				return DXGI_FORMAT_A8P8;
			case EGraphicsFormat::B4G4R4A4_UNORM:			
				return DXGI_FORMAT_B4G4R4A4_UNORM;*/
			default:
				PSX_PushError( "Unsopported Graphcis Format." );
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	D3D11_INPUT_CLASSIFICATION DX11Mapping::ToDXInputClassification( EInputClassification::Type inputClass )
	{
		switch( inputClass )
		{
		case EInputClassification::PER_VERTEX_DATA:
			return D3D11_INPUT_PER_VERTEX_DATA;
		case EInputClassification::PER_INSTANCE_DATA:
			return D3D11_INPUT_PER_INSTANCE_DATA;
		default:
			PSX_PushError( "Unknown input classification." );
		}

		return D3D11_INPUT_PER_VERTEX_DATA;
	}


	D3D11_FILL_MODE DX11Mapping::ToDXFillMode( EFillMode::Type fillMode )
	{
		switch( fillMode )
		{
		case EFillMode::WIREFRAME:
			return D3D11_FILL_WIREFRAME;
		case EFillMode::SOLID:
			return D3D11_FILL_SOLID;
		default:
			PSX_PushError( "Unknown fill mode." );
		}

		return D3D11_FILL_SOLID;
	}

	D3D11_CULL_MODE DX11Mapping::ToDXCullMode( ECullMode::Type cullMode )
	{
		switch( cullMode )
		{
		case ECullMode::NONE:
			return D3D11_CULL_NONE;
		case ECullMode::FRONT:
			return D3D11_CULL_FRONT;
		case ECullMode::BACK:
			return D3D11_CULL_BACK;
		default:
			PSX_PushError( "Unknown cull mode." );
		}

		return D3D11_CULL_BACK;
	}

	D3D11_FILTER DX11Mapping::ToDXFilter( EFilter::Type filter )
	{
		switch( filter )
		{
			case EFilter::MIN_MAG_MIP_POINT:
				return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case EFilter::MIN_MAG_POINT_MIP_LINEAR:
				return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			case EFilter::MIN_POINT_MAG_LINEAR_MIP_POINT:
				return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case EFilter::MIN_POINT_MAG_MIP_LINEAR:
				return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			case EFilter::MIN_LINEAR_MAG_MIP_POINT:
				return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case EFilter::MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case EFilter::MIN_MAG_LINEAR_MIP_POINT:
				return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case EFilter::MIN_MAG_MIP_LINEAR:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			case EFilter::ANISOTROPIC:
				return D3D11_FILTER_ANISOTROPIC;
			case EFilter::COMPARISON_MIN_MAG_MIP_POINT:
				return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			case EFilter::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
				return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			case EFilter::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
				return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case EFilter::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
				return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			case EFilter::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
				return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			case EFilter::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
				return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case EFilter::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
				return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			case EFilter::COMPARISON_MIN_MAG_MIP_LINEAR:
				return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			case EFilter::COMPARISON_ANISOTROPIC:
				return D3D11_FILTER_COMPARISON_ANISOTROPIC;
			case EFilter::TEXT_1BIT:
				PSX_PushError( "TEXT_1BIT not supported in DX11." );
				break;
			default:
				PSX_PushError( "Unsupported filter." );
		}

		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}

	D3D11_TEXTURE_ADDRESS_MODE DX11Mapping::ToDXTextureAddressMode( ETextureAddressingMode::Type address )
	{
		switch( address )
		{
		case ETextureAddressingMode::WRAP:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		case ETextureAddressingMode::MIRROR:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case ETextureAddressingMode::CLAMP:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case ETextureAddressingMode::BORDER:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		case ETextureAddressingMode::MIRROR_ONCE:
			return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		default:
			PSX_PushError( "Unsupported texture addressing mode." );
		}

		return D3D11_TEXTURE_ADDRESS_WRAP;
	}

}

#endif