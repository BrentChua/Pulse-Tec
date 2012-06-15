/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsMaping.cpp
*
*	Comments	-	See GraphicsMaping.h
*
**************************************************************************************/
#include "../Include/GraphicsMapping.h"
#include "../Include/String.h"

namespace Pulse
{
	EGraphicsFormat::Type GraphicsMapping::ToGraphicsFormat( String format )
	{
		if ( format == PSX_String("UNKNOWN") )
			return EGraphicsFormat::UNKNOWN;
		if ( format == PSX_String("R32G32B32A32_TYPELESS") )       
			return EGraphicsFormat::R32G32B32A32_TYPELESS;
		if ( format == PSX_String("R32G32B32A32_FLOAT") )          
			return EGraphicsFormat::R32G32B32A32_FLOAT;
		if ( format == PSX_String("R32G32B32A32_UINT") )           
			return EGraphicsFormat::R32G32B32A32_UINT;
		if ( format == PSX_String("R32G32B32A32_SINT") )           
			return EGraphicsFormat::R32G32B32A32_SINT;
		if ( format == PSX_String("R32G32B32_TYPELESS") )          
			return EGraphicsFormat::R32G32B32_TYPELESS;
		if ( format == PSX_String("R32G32B32_FLOAT") )             
			return EGraphicsFormat::R32G32B32_FLOAT;
		if ( format == PSX_String("R32G32B32_UINT") )              
			return EGraphicsFormat::R32G32B32_UINT;
		if ( format == PSX_String("R32G32B32_SINT") )              
			return EGraphicsFormat::R32G32B32_SINT;
		if ( format == PSX_String("R16G16B16A16_TYPELESS") )       
			return EGraphicsFormat::R16G16B16A16_TYPELESS;
		if ( format == PSX_String("R16G16B16A16_FLOAT") )          
			return EGraphicsFormat::R16G16B16A16_FLOAT;
		if ( format == PSX_String("R16G16B16A16_UNORM") )          
			return EGraphicsFormat::R16G16B16A16_UNORM;
		if ( format == PSX_String("R16G16B16A16_UINT") )           
			return EGraphicsFormat::R16G16B16A16_UINT;
		if ( format == PSX_String("R16G16B16A16_SNORM") )          
			return EGraphicsFormat::R16G16B16A16_SNORM;
		if ( format == PSX_String("R16G16B16A16_SINT") )           
			return EGraphicsFormat::R16G16B16A16_SINT;
		if ( format == PSX_String("R32G32_TYPELESS") )             
			return EGraphicsFormat::R32G32_TYPELESS;
		if ( format == PSX_String("R32G32_FLOAT") )                
			return EGraphicsFormat::R32G32_FLOAT;
		if ( format == PSX_String("R32G32_UINT") )                 
			return EGraphicsFormat::R32G32_UINT;
		if ( format == PSX_String("R32G32_SINT") )                 
			return EGraphicsFormat::R32G32_SINT;
		if ( format == PSX_String("R32G8X24_TYPELESS") )           
			return EGraphicsFormat::R32G8X24_TYPELESS;
		if ( format == PSX_String("D32_FLOAT_S8X24_UINT") )        
			return EGraphicsFormat::D32_FLOAT_S8X24_UINT;
		if ( format == PSX_String("R32_FLOAT_X8X24_TYPELESS") )    
			return EGraphicsFormat::R32_FLOAT_X8X24_TYPELESS;
		if ( format == PSX_String("X32_TYPELESS_G8X24_UINT") )     
			return EGraphicsFormat::X32_TYPELESS_G8X24_UINT;
		if ( format == PSX_String("R10G10B10A2_TYPELESS") )        
			return EGraphicsFormat::R10G10B10A2_TYPELESS;
		if ( format == PSX_String("R10G10B10A2_UNORM") )           
			return EGraphicsFormat::R10G10B10A2_UNORM;
		if ( format == PSX_String("R10G10B10A2_UINT") )            
			return EGraphicsFormat::R10G10B10A2_UINT;
		if ( format == PSX_String("R11G11B10_FLOAT") )             
			return EGraphicsFormat::R11G11B10_FLOAT;
		if ( format == PSX_String("R8G8B8A8_TYPELESS") )           
			return EGraphicsFormat::R8G8B8A8_TYPELESS;
		if ( format == PSX_String("R8G8B8A8_UNORM") )              
			return EGraphicsFormat::R8G8B8A8_UNORM;
		if ( format == PSX_String("R8G8B8A8_UNORM_SRGB") )         
			return EGraphicsFormat::R8G8B8A8_UNORM_SRGB;
		if ( format == PSX_String("R8G8B8A8_UINT") )               
			return EGraphicsFormat::R8G8B8A8_UINT;
		if ( format == PSX_String("R8G8B8A8_SNORM") )              
			return EGraphicsFormat::R8G8B8A8_SNORM;
		if ( format == PSX_String("R8G8B8A8_SINT") )               
			return EGraphicsFormat::R8G8B8A8_SINT;
		if ( format == PSX_String("R16G16_TYPELESS") )             
			return EGraphicsFormat::R16G16_TYPELESS;
		if ( format == PSX_String("R16G16_FLOAT") )                
			return EGraphicsFormat::R16G16_FLOAT;
		if ( format == PSX_String("R16G16_UNORM") )                
			return EGraphicsFormat::R16G16_UNORM;
		if ( format == PSX_String("R16G16_UINT") )                 
			return EGraphicsFormat::R16G16_UINT;
		if ( format == PSX_String("R16G16_SNORM") )                
			return EGraphicsFormat::R16G16_SNORM;
		if ( format == PSX_String("R16G16_SINT") )                 
			return EGraphicsFormat::R16G16_SINT;
		if ( format == PSX_String("R32_TYPELESS") )                
			return EGraphicsFormat::R32_TYPELESS;
		if ( format == PSX_String("D32_FLOAT") )                   
			return EGraphicsFormat::D32_FLOAT;
		if ( format == PSX_String("R32_FLOAT") )                   
			return EGraphicsFormat::R32_FLOAT;
		if ( format == PSX_String("R32_UINT") )                    
			return EGraphicsFormat::R32_UINT;
		if ( format == PSX_String("R32_SINT") )                    
			return EGraphicsFormat::R32_SINT;
		if ( format == PSX_String("R24G8_TYPELESS") )              
			return EGraphicsFormat::R24G8_TYPELESS;
		if ( format == PSX_String("D24_UNORM_S8_UINT") )           
			return EGraphicsFormat::D24_UNORM_S8_UINT;
		if ( format == PSX_String("R24_UNORM_X8_TYPELESS") )       
			return EGraphicsFormat::R24_UNORM_X8_TYPELESS;
		if ( format == PSX_String("X24_TYPELESS_G8_UINT") )        
			return EGraphicsFormat::X24_TYPELESS_G8_UINT;
		if ( format == PSX_String("R8G8_TYPELESS") )               
			return EGraphicsFormat::R8G8_TYPELESS;
		if ( format == PSX_String("R8G8_UNORM") )                  
			return EGraphicsFormat::R8G8_UNORM;
		if ( format == PSX_String("R8G8_UINT") )                   
			return EGraphicsFormat::R8G8_UINT;
		if ( format == PSX_String("R8G8_SNORM") )                  
			return EGraphicsFormat::R8G8_SNORM;
		if ( format == PSX_String("R8G8_SINT") )                   
			return EGraphicsFormat::R8G8_SINT;
		if ( format == PSX_String("R16_TYPELESS") )                
			return EGraphicsFormat::R16_TYPELESS;
		if ( format == PSX_String("R16_FLOAT") )                   
			return EGraphicsFormat::R16_FLOAT;
		if ( format == PSX_String("D16_UNORM") )                   
			return EGraphicsFormat::D16_UNORM;
		if ( format == PSX_String("R16_UNORM") )                   
			return EGraphicsFormat::R16_UNORM;
		if ( format == PSX_String("R16_UINT") )                    
			return EGraphicsFormat::R16_UINT;
		if ( format == PSX_String("R16_SNORM") )                   
			return EGraphicsFormat::R16_SNORM;
		if ( format == PSX_String("R16_SINT") )                    
			return EGraphicsFormat::R16_SINT;
		if ( format == PSX_String("R8_TYPELESS") )                 
			return EGraphicsFormat::R8_TYPELESS;
		if ( format == PSX_String("R8_UNORM") )                    
			return EGraphicsFormat::R8_UNORM;
		if ( format == PSX_String("R8_UINT") )                     
			return EGraphicsFormat::R8_UINT;
		if ( format == PSX_String("R8_SNORM") )                    
			return EGraphicsFormat::R8_SNORM;
		if ( format == PSX_String("R8_SINT") )                     
			return EGraphicsFormat::R8_SINT;
		if ( format == PSX_String("A8_UNORM") )                    
			return EGraphicsFormat::A8_UNORM;
		if ( format == PSX_String("R1_UNORM") )                    
			return EGraphicsFormat::R1_UNORM;
		if ( format == PSX_String("R9G9B9E5_SHAREDEXP") )          
			return EGraphicsFormat::R9G9B9E5_SHAREDEXP;
		if ( format == PSX_String("R8G8_B8G8_UNORM") )             
			return EGraphicsFormat::R8G8_B8G8_UNORM;
		if ( format == PSX_String("G8R8_G8B8_UNORM") )             
			return EGraphicsFormat::G8R8_G8B8_UNORM;
		if ( format == PSX_String("BC1_TYPELESS") )                
			return EGraphicsFormat::BC1_TYPELESS;
		if ( format == PSX_String("BC1_UNORM") )                   
			return EGraphicsFormat::BC1_UNORM;
		if ( format == PSX_String("BC1_UNORM_SRGB") )              
			return EGraphicsFormat::BC1_UNORM_SRGB;
		if ( format == PSX_String("BC2_TYPELESS") )                
			return EGraphicsFormat::BC2_TYPELESS;
		if ( format == PSX_String("BC2_UNORM") )                   
			return EGraphicsFormat::BC2_UNORM;
		if ( format == PSX_String("BC2_UNORM_SRGB") )              
			return EGraphicsFormat::BC2_UNORM_SRGB;
		if ( format == PSX_String("BC3_TYPELESS") )                
			return EGraphicsFormat::BC3_TYPELESS;
		if ( format == PSX_String("BC3_UNORM") )                   
			return EGraphicsFormat::BC3_UNORM;
		if ( format == PSX_String("BC3_UNORM_SRGB") )              
			return EGraphicsFormat::BC3_UNORM_SRGB;
		if ( format == PSX_String("BC4_TYPELESS") )                
			return EGraphicsFormat::BC4_TYPELESS;
		if ( format == PSX_String("BC4_UNORM") )                   
			return EGraphicsFormat::BC4_UNORM;
		if ( format == PSX_String("BC4_SNORM") )                   
			return EGraphicsFormat::BC4_SNORM;
		if ( format == PSX_String("BC5_TYPELESS") )                
			return EGraphicsFormat::BC5_TYPELESS;
		if ( format == PSX_String("BC5_UNORM") )                   
			return EGraphicsFormat::BC5_UNORM;
		if ( format == PSX_String("BC5_SNORM") )                   
			return EGraphicsFormat::BC5_SNORM;
		if ( format == PSX_String("B5G6R5_UNORM") )                
			return EGraphicsFormat::B5G6R5_UNORM;
		if ( format == PSX_String("B5G5R5A1_UNORM") )              
			return EGraphicsFormat::B5G5R5A1_UNORM;
		if ( format == PSX_String("B8G8R8A8_UNORM") )              
			return EGraphicsFormat::B8G8R8A8_UNORM;
		if ( format == PSX_String("B8G8R8X8_UNORM") )              
			return EGraphicsFormat::B8G8R8X8_UNORM;
		if ( format == PSX_String("R10G10B10_XR_BIAS_A2_UNORM") )  
			return EGraphicsFormat::R10G10B10_XR_BIAS_A2_UNORM;
		if ( format == PSX_String("B8G8R8A8_TYPELESS") )           
			return EGraphicsFormat::B8G8R8A8_TYPELESS;
		if ( format == PSX_String("B8G8R8A8_UNORM_SRGB") )         
			return EGraphicsFormat::B8G8R8A8_UNORM_SRGB;
		if ( format == PSX_String("B8G8R8X8_TYPELESS") )           
			return EGraphicsFormat::B8G8R8X8_TYPELESS;
		if ( format == PSX_String("B8G8R8X8_UNORM_SRGB") )         
			return EGraphicsFormat::B8G8R8X8_UNORM_SRGB;
		if ( format == PSX_String("BC6H_TYPELESS") )               
			return EGraphicsFormat::BC6H_TYPELESS;
		if ( format == PSX_String("BC6H_UF16") )                   
			return EGraphicsFormat::BC6H_UF16;
		if ( format == PSX_String("BC6H_SF16") )                   
			return EGraphicsFormat::BC6H_SF16;
		if ( format == PSX_String("BC7_TYPELESS") )                
			return EGraphicsFormat::BC7_TYPELESS;
		if ( format == PSX_String("BC7_UNORM") )                   
			return EGraphicsFormat::BC7_UNORM;
		if ( format == PSX_String("BC7_UNORM_SRGB") )              
			return EGraphicsFormat::BC7_UNORM_SRGB;
		if ( format == PSX_String("AYUV") )                        
			return EGraphicsFormat::AYUV;
		if ( format == PSX_String("Y410") )                        
			return EGraphicsFormat::Y410;
		if ( format == PSX_String("Y416") )                        
			return EGraphicsFormat::Y416;
		if ( format == PSX_String("NV12") )                        
			return EGraphicsFormat::NV12;
		if ( format == PSX_String("P010") )                        
			return EGraphicsFormat::P010;
		if ( format == PSX_String("P016") )                        
			return EGraphicsFormat::P016;
		if ( format == PSX_String("_420_OPAQUE") )                 
			return EGraphicsFormat::_420_OPAQUE;
		if ( format == PSX_String("YUY2") )                        
			return EGraphicsFormat::YUY2;
		if ( format == PSX_String("Y210") )                        
			return EGraphicsFormat::Y210;
		if ( format == PSX_String("Y216") )                        
			return EGraphicsFormat::Y216;
		if ( format == PSX_String("NV11") )                        
			return EGraphicsFormat::NV11;
		if ( format == PSX_String("AI44") )                        
			return EGraphicsFormat::AI44;
		if ( format == PSX_String("IA44") )                        
			return EGraphicsFormat::IA44;
		if ( format == PSX_String("P8") )                          
			return EGraphicsFormat::P8;
		if ( format == PSX_String("A8P8") )                        
			return EGraphicsFormat::A8P8;
		if ( format == PSX_String("B4G4R4A4_UNORM") )               
			return EGraphicsFormat::B4G4R4A4_UNORM;
		
		PSX_PushError( "Invalid Graphics format specified: %s.", format.GetBuffer() );

		return EGraphicsFormat::UNKNOWN;
	}

	EInputClassification::Type GraphicsMapping::ToInputClassification( String inputClassification )
	{
		if ( inputClassification == PSX_String("PER_VERTEX_DATA") )
			return EInputClassification::PER_VERTEX_DATA;

		if ( inputClassification == PSX_String("PER_INSTANCE_DATA") )
			return EInputClassification::PER_INSTANCE_DATA;

		PSX_PushError( "Invalid InputClassification specified: %s.", inputClassification.GetBuffer() );
		
		return EInputClassification::PER_VERTEX_DATA;
	}

}