/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsTypes.h
*
*	Description -	Graphics/Renderer data types. 
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/16/2010	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_GRAPHICS_TYPES_H_
#define _PSX_GRAPHICS_TYPES_H_

#include "PulseSTD.h"
#include "Vector3.h"
#include "Color.h"
#include "String.h"
#include "2D.h"
#include "Rational.h"
#include "Array.h"

namespace Pulse
{
	PSX_EnumBegin( ERendererType )
		UNKNOWN,
		DX9, // Truncated. Use DX11
		DX11,
	PSX_EnumEnd()

	PSX_EnumBegin( ERendererProcessingType )
		UNKNOWN,
		HARDWARE,
		SOFTWARE, // Reference - full software implementation
	PSX_EnumEnd()

	PSX_EnumBegin( EDeviceContext )
		IMMEDIATE,
		DEFERRED,
	PSX_EnumEnd()

	PSX_EnumBegin( EPipelineStage )
		InputAssembler,
		VertexShader,
		HullShader,
		Tesellator,
		DomainShader,
		GeometryShader,
		StreamOutput,
		Rasterizer,
		PixelShader,
		OutputMerger,
	PSX_EnumEnd()

	PSX_EnumBegin( ETextureAddressingMode )
		WRAP          = 1, // Tile the texture at every (u,v) integer junction. For example, for u values between 0 and 3, the texture is repeated three times.
		MIRROR        = 2, // Flip the texture at every (u,v) integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on.
		CLAMP         = 3, // Texture coordinates outside the range [0.0, 1.0] are set to the texture color at 0.0 or 1.0, respectively.
		BORDER        = 4, // Texture coordinates outside the range [0.0, 1.0] are set to the border color specified in D3D11_SAMPLER_DESC or HLSL code.
		MIRROR_ONCE   = 5, // Similar to D3D11_TEXTURE_ADDRESS_MIRROR and D3D11_TEXTURE_ADDRESS_CLAMP. Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value.
		_UNUSED_	  = 4,
	PSX_EnumEnd()

	PSX_EnumBegin( ETextureFilterType )
		UNKNOWN,
		NONE,
		POINT,			// Point filtering
		LINEAR,		// Bilinear filtering
		ANISOTROPIC,	// Anisotropic filtering
	PSX_EnumEnd()

	PSX_EnumBegin( EBlendWriteMask )
		ZERO,	// Turn off writes to the depth-stencil buffer.
		ALL,	// Turn on writes to the depth-stencil buffer.
	PSX_EnumEnd()

	PSX_EnumBegin( EStencilOp )
		KEEP       = 1,	// Keep the existing stencil data.
		ZERO       = 2,	// Set the stencil data to 0.
		REPLACE    = 3,	// Set the stencil data to the reference value set by calling ID3D11DeviceContext::OMSetDepthStencilState.
		INCR_SAT   = 4,	// Increment the stencil value by 1, and clamp the result.
		DECR_SAT   = 5,	// Decrement the stencil value by 1, and clamp the result.
		INVERT     = 6, // Invert the stencil data.
		INCR       = 7,	// Increment the stencil value by 1, and wrap the result if necessary.
		DECR       = 8,	// Increment the stencil value by 1, and wrap the result if necessary.
		_UNUSED_   = 7, // To make the count correct
	PSX_EnumEnd()

	PSX_EnumBegin( EStencilReadMask )
		DEFAULT = 0xff,
		_UNUSED_ = 0,
	PSX_EnumEnd()

	PSX_EnumBegin( EStencilWriteMask )
		DEFAULT = 0xff,
		_UNUSED_ = 0,
	PSX_EnumEnd()

	PSX_EnumBegin( EComparisonFunc )
		NEVER           = 1,	// Never pass the comparison.
		LESS            = 2,	// If the source data is less than the destination data, the comparison passes.
		EQUAL           = 3,	// If the source data is equal to the destination data, the comparison passes.
		LESS_EQUAL      = 4,	// If the source data is less than or equal to the destination data, the comparison passes.
		GREATER         = 5,	// If the source data is greater than the destination data, the comparison passes.
		NOT_EQUAL       = 6,	// If the source data is not equal to the destination data, the comparison passes.
		GREATER_EQUAL   = 7,	// If the source data is greater than or equal to the destination data, the comparison passes.
		ALWAYS          = 8,	// Always pass the comparison.
		_UNUSED_2		= 7, // To make the count correct
	PSX_EnumEnd()

	PSX_EnumBegin( EBlendPixelType )
		SOURCE,
		DESTINATION,
	PSX_EnumEnd()

	PSX_EnumBegin( EBlendType )
		ZERO               = 1,
		ONE                = 2,
		SRC_COLOR          = 3,
		INV_SRC_COLOR      = 4,
		SRC_ALPHA          = 5,
		INV_SRC_ALPHA      = 6,
		DEST_ALPHA         = 7,
		INV_DEST_ALPHA     = 8,
		DEST_COLOR         = 9,
		INV_DEST_COLOR     = 10,
		SRC_ALPHA_SAT      = 11,
		BLEND_FACTOR       = 14,
		INV_BLEND_FACTOR   = 15,
		SRC1_COLOR         = 16,
		INV_SRC1_COLOR     = 17,
		SRC1_ALPHA         = 18,
		INV_SRC1_ALPHA     = 19,
		_UNUSED_		   = 18, // To make the count correct
	PSX_EnumEnd()

	PSX_EnumBegin( EBlendOp )
		ADD            = 1,
		SUBTRACT       = 2,
		REV_SUBTRACT   = 3,
		MIN            = 4,
		MAX            = 5,
		_UNUSED_	   = 4,
	PSX_EnumEnd()

	// These flags can be combined with bitwise OR.
	PSX_EnumBegin( EColorWriteEnable )
		NONE	= 0,
		RED     = 1,
		GREEN   = 2,
		BLUE    = 4,
		ALPHA   = 8,
		ALL     = ( RED | GREEN | BLUE | ALPHA ),
		_UNUSED_ = 5, // To make the count correct
	PSX_EnumEnd()

	PSX_EnumBegin( EFillMode )
		WIREFRAME	= 2,
		SOLID		= 3,
		_UNUSED_	= 1,
	PSX_EnumEnd()

	PSX_EnumBegin( ECullMode )
		NONE	= 1,
		FRONT	= 2,	// Do not draw triangles that are front-facing.
		BACK	= 3,	// Do not draw triangles that are back-facing.
		_UNUSED_ = 2,
	PSX_EnumEnd()

	PSX_EnumBegin( EFilter )
		MIN_MAG_MIP_POINT                            = 0,	// Use point sampling for minification, magnification, and mip-level sampling.
		MIN_MAG_POINT_MIP_LINEAR                     = 0x1, // Use point sampling for minification and magnification; use linear interpolation for mip-level sampling.
		MIN_POINT_MAG_LINEAR_MIP_POINT               = 0x4, // Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling.
		MIN_POINT_MAG_MIP_LINEAR                     = 0x5, // Use point sampling for minification; use linear interpolation for magnification and mip-level sampling.
		MIN_LINEAR_MAG_MIP_POINT                     = 0x10, // Use linear interpolation for minification; use point sampling for magnification and mip-level sampling.
		MIN_LINEAR_MAG_POINT_MIP_LINEAR              = 0x11, // Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling.
		MIN_MAG_LINEAR_MIP_POINT                     = 0x14, // Use linear interpolation for minification and magnification; use point sampling for mip-level sampling.
		MIN_MAG_MIP_LINEAR                           = 0x15, // Use linear interpolation for minification, magnification, and mip-level sampling.
		ANISOTROPIC                                  = 0x55, // Use anisotropic interpolation for minification, magnification, and mip-level sampling.
		COMPARISON_MIN_MAG_MIP_POINT                 = 0x80, // Use point sampling for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_MAG_POINT_MIP_LINEAR          = 0x81, // Use point sampling for minification and magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT    = 0x84, // Use point sampling for minification; use linear interpolation for magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_POINT_MAG_MIP_LINEAR          = 0x85, // Use point sampling for minification; use linear interpolation for magnification and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_LINEAR_MAG_MIP_POINT          = 0x90, // Use linear interpolation for minification; use point sampling for magnification and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR   = 0x91, // Use linear interpolation for minification; use point sampling for magnification; use linear interpolation for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_MAG_LINEAR_MIP_POINT          = 0x94, // Use linear interpolation for minification and magnification; use point sampling for mip-level sampling. Compare the result to the comparison value.
		COMPARISON_MIN_MAG_MIP_LINEAR                = 0x95, // Use linear interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
		COMPARISON_ANISOTROPIC                       = 0xd5, // Use anisotropic interpolation for minification, magnification, and mip-level sampling. Compare the result to the comparison value.
		TEXT_1BIT                                    = 0x80000000, // For use in pixel shaders with textures that have the R1_UNORM format.
		_UNUSED_									 = 18,
	PSX_EnumEnd()

	PSX_EnumBegin( ERenderMode )
		UNKNOWN,
		_2D,
		_3D,
	PSX_EnumEnd()

	//struct RenderState
	//{
	//	UINT				globalStates;
	//	FLOAT				zBias;
	//	FLOAT				gamma;
	//	BOOL				blight;
	//	BOOL				bAlphaBlend;
	//	BOOL				bSeparateAlphaBlend;
	//	BOOL				bZWrite;
	//	BOOL				bZBuffer;
	//	BOOL				bAlphaTest;
	//	BOOL				bScissorTest;
	//	Rect				scissorRect;
	//	DWORD				alphaTestRef;
	//	EBlendType::Type	blendSource;
	//	EBlendType::Type	blendDest;
	//	EBlendType::Type	alphaBlendSource;
	//	EBlendType::Type	alphaBlendDest;
	//	EBlendOp::Type		blendOp;
	//	ECullMode::Type		cullMode;
	//	EFillMode::Type		fillMode;
	//	Color				ambient;				// ambient light color
	//	
	//	RenderState( void )
	//		: globalStates( 0 ), zBias( 0.0f ), gamma( 1.0f ), blight( TRUE ), bAlphaBlend( FALSE ),
	//		bSeparateAlphaBlend( FALSE ), bZWrite( TRUE ), bZBuffer( TRUE ), bAlphaTest( FALSE ), 
	//		bScissorTest( FALSE ), alphaTestRef( 1 ), blendSource( EBlendType::ONE ), blendDest( EBlendType::ZERO ),
	//		alphaBlendSource( EBlendType::ONE ), alphaBlendDest( EBlendType::ZERO ), blendOp( EBlendOp::ADD ),
	//		cullMode( ECullMode::CW ), fillMode( EFillMode::SOLID ), ambient( Color::BLACK )
	//	{
	//		scissorRect.left = scissorRect.top = scissorRect.right = scissorRect.bottom = 0;
	//	}
	//};

	PSX_EnumBegin( EGraphicsCaps )
		MAX_ANISOTROPY,			// Maximum anisotropy for anisotropic texture filtering
		MIN_ANISOTROPY,			// Anisotropy for minifying textures is supported
		MAG_ANISOTROPY,			// Anisotropy for magnifying textures is supported
		MAX_CLIPPLANES,			// How many clipping planes could be defined and used
		MAX_INDICES_COUNT,		// How many vertex indices can be used in a buffer
		MAX_PRIMITIVE_COUNT,	//  How many primitives can be used in one draw call
		MAX_LIGHTS,				// Maximum number of active lights being active at the same time
		MAX_RTS,				// How many simultanous render targets can be used
		MAX_TEXTURE_HEIGHT,		// Maximum texture height
		MAX_TEXTURE_WIDTH,		// Maximum texture width
		VERTEX_SHADER,			// What version of vertex shaders is supported
		GEOMETRY_SHADER,		// What version of geometry shaders is supported
		PIXEL_SHADER,			// What version of pixel shaders is supported
	PSX_EnumEnd()

	struct ETextureFilter
	{
		enum Type
		{
			MIN = (1<<0),	// Minification filter
			MAG = (1<<1),	// Magnification filter
			MIP = (1<<2),	// Mip-mapping filter
			EnumCount = 3,
		};
	};

	// Describes multi-sampling parameters for a resource.
	struct SampleDesc
	{

		SampleDesc( UINT _count = 1, UINT _quality = 0 )
		: count( _count ), quality( _quality )
		{
		}

		UINT count;
		UINT quality;
	};

	PSX_EnumBegin( EClearBufferType )
		UNKNOWN			= (1 << 0),
		DEPTH			= (1 << 1),
		STENCIL			= (1 << 2),
		RENDER_TARGET	= (1 << 3),
	PSX_EnumEnd()

	PSX_EnumBegin( ETopology )
		UNKNOWN			= 0,
		POINT_LIST		= 1,
		LINE_LIST		= 2,
		LINE_STRIP		= 3,
		TRIANGLE_LIST	= 4,
		TRIANGLE_STRIP	= 5,
	PSX_EnumEnd()

	PSX_EnumBegin( EVertexBufferType )
		UNKNOWN,
		STATIC,
		DYNAMIC,
	PSX_EnumEnd()

	PSX_EnumBegin( EVertexElementUsage )
		POSITION,
		NORMAL,
		BINORMAL,
		TANGENT,
		TEXCOORD,
		COLOR,
		FOG,
		DEPTH,
		SAMPLE,
		BLEND_WEIGHTS,
		BLEND_INDICES,
	PSX_EnumEnd()

	struct VertexElement
	{
		UINT						stream;		// The stream index the vertex component is associated.
		UINT						offset;		// The offset in bites from the start of the vertex structure.
		EVertexElementUsage::Type	usage;		// What the element will be used for.
		UINT						usageIndex;	// [0, 15] Used to indentify multiple vertex components of the same usage. 
	};

	struct Vertex1P
	{
		Vector3 position;
	};

	struct Vertex1P1D
	{
		Vector3 position;
		DWORD	diffuse;
	};

	struct Vertex1P1T
	{
		Vector3 position;
		Vector2 texCoord;
	};

	struct Vertex1P1D1T
	{
		Vector3 position;
		DWORD	diffuse;
		Vector2 texCoord;
	};

	// Shader stuff
	///////////////

	PSX_EnumBegin( EShaderType )
		UNKNOWN,
		VERTEX,
		HULL,
		DOMAIN_,
		GEOMETRY,
		PIXEL,
		COMPUTE,
		_UNUSED_	= 5, // Fix enum count
	PSX_EnumEnd()

	PSX_EnumBegin( EShaderStage )
		VERTEX_STAGE,
		HULL_STAGE,
		DOMAIN_STAGE,
		GEOMETRY_STAGE,
		PIXEL_STAGE,
		COMPUTE_STAGE,
	PSX_EnumEnd()

	PSX_EnumBegin( EBufferUsage )
		UNKNOWN,
		STATIC,
		DYNAMIC,
	PSX_EnumEnd()

	// The intended use of a constant buffer
	PSX_EnumBegin( EShaderBufferType )
		CONSTANT_BUFFER = 0,
		TEXTURE_BUFFER = (CONSTANT_BUFFER + 1),
		INTERFACE_POINTERS = (TEXTURE_BUFFER + 1),
		RESOURCE_BIND_INFO = (INTERFACE_POINTERS + 1),
	PSX_EnumEnd()

	PSX_EnumBegin( EShaderVariableClass )
		SCALAR,
		VECTOR,
		MATRIX_ROWS,
		MATRIX_COLUMNS,
		OBJECT,
		STRUCT,
		INTERFACE_CLASS,
		INTERFACE_POINTER,
	PSX_EnumEnd()

	PSX_EnumBegin( EShaderVariableType )
		_VOID                       = 0,
		BOOL                        = 1,
		INT                         = 2,
		FLOAT                       = 3,
		STRING                      = 4,
		TEXTURE                     = 5,
		TEXTURE1D                   = 6,
		TEXTURE2D                   = 7,
		TEXTURE3D                   = 8,
		TEXTURECUBE                 = 9,
		SAMPLER                     = 10,
		PIXELSHADER                 = 15,
		VERTEXSHADER                = 16,
		UINT                        = 19,
		UINT8                       = 20,
		GEOMETRYSHADER              = 21,
		RASTERIZER                  = 22,
		DEPTHSTENCIL                = 23,
		BLEND                       = 24,
		BUFFER                      = 25,
		CBUFFER                     = 26,
		TBUFFER                     = 27,
		TEXTURE1DARRAY              = 28,
		TEXTURE2DARRAY              = 29,
		RENDERTARGETVIEW            = 30,
		DEPTHSTENCILVIEW            = 31,
		TEXTURE2DMS                 = 32,
		TEXTURE2DMSARRAY            = 33,
		TEXTURECUBEARRAY            = 34,
		HULLSHADER                  = 35,
		DOMAINSHADER                = 36,
		INTERFACE_POINTER           = 37,
		COMPUTESHADER               = 38,
		DOUBLE                      = 39,
		RWTEXTURE1D,
		RWTEXTURE1DARRAY,
		RWTEXTURE2D,
		RWTEXTURE2DARRAY,
		RWTEXTURE3D,
		RWBUFFER,
		BYTEADDRESS_BUFFER,
		RWBYTEADDRESS_BUFFER,
		STRUCTURED_BUFFER,
		RWSTRUCTURED_BUFFER,
		APPEND_STRUCTURED_BUFFER,
		CONSUME_STRUCTURED_BUFFER,
	PSX_EnumEnd()

	PSX_EnumBegin( EGraphicsFormat )
		UNKNOWN                      = 0,
		R32G32B32A32_TYPELESS        = 1,
		R32G32B32A32_FLOAT           = 2,
		R32G32B32A32_UINT            = 3,
		R32G32B32A32_SINT            = 4,
		R32G32B32_TYPELESS           = 5,
		R32G32B32_FLOAT              = 6,
		R32G32B32_UINT               = 7,
		R32G32B32_SINT               = 8,
		R16G16B16A16_TYPELESS        = 9,
		R16G16B16A16_FLOAT           = 10,
		R16G16B16A16_UNORM           = 11,
		R16G16B16A16_UINT            = 12,
		R16G16B16A16_SNORM           = 13,
		R16G16B16A16_SINT            = 14,
		R32G32_TYPELESS              = 15,
		R32G32_FLOAT                 = 16,
		R32G32_UINT                  = 17,
		R32G32_SINT                  = 18,
		R32G8X24_TYPELESS            = 19,
		D32_FLOAT_S8X24_UINT         = 20,
		R32_FLOAT_X8X24_TYPELESS     = 21,
		X32_TYPELESS_G8X24_UINT      = 22,
		R10G10B10A2_TYPELESS         = 23,
		R10G10B10A2_UNORM            = 24,
		R10G10B10A2_UINT             = 25,
		R11G11B10_FLOAT              = 26,
		R8G8B8A8_TYPELESS            = 27,
		R8G8B8A8_UNORM               = 28,
		R8G8B8A8_UNORM_SRGB          = 29,
		R8G8B8A8_UINT                = 30,
		R8G8B8A8_SNORM               = 31,
		R8G8B8A8_SINT                = 32,
		R16G16_TYPELESS              = 33,
		R16G16_FLOAT                 = 34,
		R16G16_UNORM                 = 35,
		R16G16_UINT                  = 36,
		R16G16_SNORM                 = 37,
		R16G16_SINT                  = 38,
		R32_TYPELESS                 = 39,
		D32_FLOAT                    = 40,
		R32_FLOAT                    = 41,
		R32_UINT                     = 42,
		R32_SINT                     = 43,
		R24G8_TYPELESS               = 44,
		D24_UNORM_S8_UINT            = 45,
		R24_UNORM_X8_TYPELESS        = 46,
		X24_TYPELESS_G8_UINT         = 47,
		R8G8_TYPELESS                = 48,
		R8G8_UNORM                   = 49,
		R8G8_UINT                    = 50,
		R8G8_SNORM                   = 51,
		R8G8_SINT                    = 52,
		R16_TYPELESS                 = 53,
		R16_FLOAT                    = 54,
		D16_UNORM                    = 55,
		R16_UNORM                    = 56,
		R16_UINT                     = 57,
		R16_SNORM                    = 58,
		R16_SINT                     = 59,
		R8_TYPELESS                  = 60,
		R8_UNORM                     = 61,
		R8_UINT                      = 62,
		R8_SNORM                     = 63,
		R8_SINT                      = 64,
		A8_UNORM                     = 65,
		R1_UNORM                     = 66,
		R9G9B9E5_SHAREDEXP           = 67,
		R8G8_B8G8_UNORM              = 68,
		G8R8_G8B8_UNORM              = 69,
		BC1_TYPELESS                 = 70,
		BC1_UNORM                    = 71,
		BC1_UNORM_SRGB               = 72,
		BC2_TYPELESS                 = 73,
		BC2_UNORM                    = 74,
		BC2_UNORM_SRGB               = 75,
		BC3_TYPELESS                 = 76,
		BC3_UNORM                    = 77,
		BC3_UNORM_SRGB               = 78,
		BC4_TYPELESS                 = 79,
		BC4_UNORM                    = 80,
		BC4_SNORM                    = 81,
		BC5_TYPELESS                 = 82,
		BC5_UNORM                    = 83,
		BC5_SNORM                    = 84,
		B5G6R5_UNORM                 = 85,
		B5G5R5A1_UNORM               = 86,
		B8G8R8A8_UNORM               = 87,
		B8G8R8X8_UNORM               = 88,
		R10G10B10_XR_BIAS_A2_UNORM   = 89,
		B8G8R8A8_TYPELESS            = 90,
		B8G8R8A8_UNORM_SRGB          = 91,
		B8G8R8X8_TYPELESS            = 92,
		B8G8R8X8_UNORM_SRGB          = 93,
		BC6H_TYPELESS                = 94,
		BC6H_UF16                    = 95,
		BC6H_SF16                    = 96,
		BC7_TYPELESS                 = 97,
		BC7_UNORM                    = 98,
		BC7_UNORM_SRGB               = 99,
		AYUV                         = 100,
		Y410                         = 101,
		Y416                         = 102,
		NV12                         = 103,
		P010                         = 104,
		P016                         = 105,
		_420_OPAQUE                  = 106,
		YUY2                         = 107,
		Y210                         = 108,
		Y216                         = 109,
		NV11                         = 110,
		AI44                         = 111,
		IA44                         = 112,
		P8                           = 113,
		A8P8                         = 114,
		B4G4R4A4_UNORM               = 115,      
	PSX_EnumEndForceUINT()

	PSX_EnumBegin( EInputClassification )
		PER_VERTEX_DATA		= 0,	// Input data is per-vertex data.
		PER_INSTANCE_DATA	= 1,	// Input data is per-instance data.
	PSX_EnumEnd()

	PSX_EnumBegin( ETextureUsage )
		UNKNOWN,
		NORMAL,			// GPU read/write
		STATIC,			// GPU read. Requires initialization.
		RENDERTARGET,	//
		SWAPCHAINBUFFER, // Swap chain buffer render target.
		DEPTHBUFFER, 
		DYNAMIC,		// GPU read and CPU write
		STAGING,		// Data transfer(copy) from GPU to CPU
	PSX_EnumEnd()

	PSX_EnumBegin( ESystemValueType )
		UNDEFINED                       = 0,
		POSITION                        = 1,
		CLIP_DISTANCE                   = 2,
		CULL_DISTANCE                   = 3,
		RENDER_TARGET_ARRAY_INDEX       = 4,
		VIEWPORT_ARRAY_INDEX            = 5,
		VERTEX_ID                       = 6,
		PRIMITIVE_ID                    = 7,
		INSTANCE_ID                     = 8,
		IS_FRONT_FACE                   = 9,
		SAMPLE_INDEX                    = 10,
		FINAL_QUAD_EDGE_TESSFACTOR      = 11,
		FINAL_QUAD_INSIDE_TESSFACTOR    = 12,
		FINAL_TRI_EDGE_TESSFACTOR       = 13,
		FINAL_TRI_INSIDE_TESSFACTOR     = 14,
		FINAL_LINE_DETAIL_TESSFACTOR    = 15,
		FINAL_LINE_DENSITY_TESSFACTOR   = 16,
		TARGET                          = 64,
		DEPTH                           = 65,
		COVERAGE                        = 66,
		DEPTH_GREATER_EQUAL             = 67,
		DEPTH_LESS_EQUAL                = 68,
	PSX_EnumEnd()

	PSX_EnumBegin( ERegisterComponentType )
		UNKNOWN = 0,
		UINT32	= 1,
		SINT32	= 2,
		FLOAT32	= 3,
	PSX_EnumEnd()

	struct ShaderConstantSemantic
	{
		typedef void (*SemanticFunction)( void *, SIZE_T );

		String				name;			// Semantic name
		String				dataType;		// Data type associated with
		SemanticFunction	semanticFunc;	// Function called for semantic

		ShaderConstantSemantic( void ) : semanticFunc( NULL )
		{

		}
	};

	PSX_EnumBegin( EShaderConstantType )
		FLOAT,	// 32-bit floating point value
		INT,	// 32-bit signed integer
		BOOL,	// true or false
		//UINT,	// 32-bit unsigned integer
		//DOUBLE,	// 64-bit floating point vlaue.
	PSX_EnumEnd()

	struct ShaderConstantType
	{
		String				name;
		SIZE_T				dataCount;
		EShaderConstantType type;
	};

	PSX_EnumBegin( EDrawOrder )
		PRE,
		SCENE,
		POST,
	PSX_EnumEnd()

	PSX_EnumBegin( EAnchorPoint )
		UPPER_LEFT,
		UPPER_RIGHT,
		CENTER,
		CENTER_LEFT,
		CENTER_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
	PSX_EnumEnd()

	// Common Render Techniques
	#define COMMON_RT_2D		PSX_String("RenderTechnique2D")
	#define COMMON_RT_2DFONT	PSX_String("RenderTechnique2DFont")
	#define COMMON_RT_2DFRAME	PSX_String("RenderTechnique2DFrame")
	#define COMMON_RT_2DQUAD	PSX_String("RenderTechnique2DQuad")
	#define COMMON_RT_2DLINE	PSX_String("RenderTechnique2DLine")

	// Common Vertex Declarations
	#define COMMON_VD_1P		PSX_String( "VertexDeclaration1P" )
	#define COMMON_VD_1P1D		PSX_String( "VertexDeclaration1P1D" )
	#define COMMON_VD_1P1T		PSX_String( "VertexDeclaration1P1T" )
	#define COMMON_VD_1P1D1T	PSX_String( "VertexDeclaration1P1D1T" )

	// Common Vertex Buffers
	#define COMMON_VB_1P			PSX_String( "VertexBuffer1P" )
	#define COMMON_VB_1P1D			PSX_String( "VertexBuffer1P1D" )
	#define COMMON_VB_1P1D1T		PSX_String( "VertexBuffer1P1D1T" )
	#define COMMON_VB_SCREEN_QUAD	PSX_String( "VertexBufferScreenQuad" )

	// Common Materials
	#define COMMON_MATERIAL_QUAD PSX_String( "MaterialQuad" )
	
	// Render Types
	PSX_EnumBegin( ERenderType )
		GUI,
		GUI_FRAME,
		SCENE,
	PSX_EnumEnd()

	struct MultiSampleDesc
	{
		UINT count; // Number of multisamples per pixel
		UINT quality; // Image quality level. value between 0 and ID3D11Device::CheckMultisampleQualityLevels() - 1.
	};
	struct DisplayMode
	{
		UINT 					adapter;
		UINT					adapterOutput;
		UINT 					width;
		UINT 					height;
		Rational				refreshRate;
		EGraphicsFormat::Type	format;

		BOOL 					bWindowed;
		BOOL 					bVSync;
		ERendererProcessingType::Type	processingType;

		//RenderState				renderState;

		//ERenderMode::Type		renderMode;

		// Texture filtering settings
		ETextureFilterType::Type filterMin[8];
		ETextureFilterType::Type filterMag[8];
		ETextureFilterType::Type filterMip[8];

		ETextureAddressingMode::Type textureAddressingMode[8];


		DisplayMode( void )
			: adapter( 0 ), adapterOutput(0), width( 800 ), height( 450 ), 
			refreshRate( 60.0f, 1.0f ), format(EGraphicsFormat::R8G8B8A8_UNORM), bWindowed( TRUE ),
			bVSync( FALSE ), processingType( ERendererProcessingType::HARDWARE )
		{
			for ( SIZE_T i = 0; i < 8; ++i )
			{
				filterMin[i] = filterMag[i] = filterMin[i] = ETextureFilterType::LINEAR;
				textureAddressingMode[i] = ETextureAddressingMode::CLAMP;
			}
		}
	};

	struct MappedResource
	{
		void *pData;
		UINT RowPitch;
		UINT DepthPitch;
	};

	PSX_EnumBegin( EResourceMapping )
		READ				= 1,
		WRITE				= 2,
		READ_WRITE			= 3,
		WRITE_DISCARD		= 4,
		WRITE_NO_OVERWRITE	= 5,
		__UNUSED__			= 4, // This is added to make numEnums correct
	PSX_EnumEnd()

	#define PSX_NUM_CONSTANT_BUFRER_SLOTS D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT

}

#endif /* _PSX_GRAPHICS_TYPES_H_ */