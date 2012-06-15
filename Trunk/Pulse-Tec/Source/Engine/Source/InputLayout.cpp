/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	InputLayout.cpp
*
*	Comments	-	See InputLayout.h
*
**************************************************************************************/
#include "../Include/InputLayout.h"
#include "../Include/GraphicsDevice.h"
#include "../Include/Shader.h"
#include "../Include/XMLParser.h"
#include "../Include/DX11Mapping.h"
#include "../Include/GraphicsMapping.h"

namespace Pulse
{
	InputLayoutDesc::InputLayoutDesc( void )
		: pShaderForSignature( PSX_NULL )
	{
		
	}

	void InputLayoutDesc::AddElement( const CHAR *pSemanticName,UINT semanticIndex, EGraphicsFormat::Type format,
		UINT inputSlot, UINT alignedByteOffset, EInputClassification::Type inputClassification, UINT instanceDataStepRate )
	{
		InputElementDesc newDesc;

		newDesc.semanticName			= pSemanticName;
		newDesc.semanticIndex			= semanticIndex;
		newDesc.format					= format;
		newDesc.inputSlot				= inputSlot;
		newDesc.alignedByteOffset		= alignedByteOffset;
		newDesc.inputClassification		= inputClassification;
		newDesc.instanceDataStepRate	= instanceDataStepRate;

		elementDescList.PushBack( newDesc );
	}

	void InputLayoutDesc::AddElementAsPerVertexData( const CHAR *pSemanticName,UINT semanticIndex, EGraphicsFormat::Type format,
		UINT inputSlot, UINT alignedByteOffset )
	{
		InputElementDesc newDesc;

		newDesc.semanticName			= pSemanticName;
		newDesc.semanticIndex			= semanticIndex;
		newDesc.format					= format;
		newDesc.inputSlot				= inputSlot;
		newDesc.alignedByteOffset		= alignedByteOffset;
		newDesc.inputClassification		= EInputClassification::PER_VERTEX_DATA;
		newDesc.instanceDataStepRate	= 0;

		elementDescList.PushBack( newDesc );
	}

	void InputLayoutDesc::AddElementAsPerInstanceData( const CHAR *pSemanticName,UINT semanticIndex, EGraphicsFormat::Type format,
		UINT inputSlot, UINT alignedByteOffset, UINT instanceDataStepRate )
	{
		InputElementDesc newDesc;

		newDesc.semanticName			= pSemanticName;
		newDesc.semanticIndex			= semanticIndex;
		newDesc.format					= format;
		newDesc.inputSlot				= inputSlot;
		newDesc.alignedByteOffset		= alignedByteOffset;
		newDesc.inputClassification		= EInputClassification::PER_INSTANCE_DATA;
		newDesc.instanceDataStepRate	= instanceDataStepRate;

		elementDescList.PushBack( newDesc );
	}

	void InputLayoutDesc::SetFilename( const CHAR *pFilename )
	{
		m_filename = pFilename;
	}

	InputElementDesc * InputLayoutDesc::GetElement( UINT index )
	{
		return &elementDescList[index];
	}

	const SIZE_T InputLayoutDesc::GetSize( void ) const
	{
		return elementDescList.GetSize();
	}

	void InputLayoutDesc::RemoveElement( UINT index )
	{
		elementDescList.Remove( index );
	}

	void InputLayoutDesc::ClearElements( void )
	{
		elementDescList.Clear();
	}

	InputLayout::InputLayout( void )
		: m_pInputLayout( PSX_NULL )
	{

	}

	InputLayout::~InputLayout( void )
	{

	}

	BOOL InputLayout::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::INPUT_LAYOUT )
			return FALSE;

		m_desc = *((InputLayoutDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL InputLayout::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL InputLayout::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL InputLayout::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type InputLayout::Initialize( void )
	{
		// Check if we're loading InputLayout from XML file
		if ( m_desc.GetSize() == 0 && m_desc.m_filename.GetLength() != 0 )
		{
			XMLElementPtr pNode = XMLParser::Load( m_desc.m_filename.GetBuffer() );
		
			if ( pNode.IsNull() )
			{
				PSX_PushError( "Failed to load InputLayout file." );
				return EErrorCode::GRAPHICS;
			}

			if ( pNode->GetTagName() != PSX_String("InputLayout") )
			{
				PSX_PushError( "Invalid InputLayout file info." );
				return EErrorCode::GRAPHICS;
			}

			XMLElementPtr pChild = pNode->GetFirstChild();
			String tag;

			while( pChild )
			{
				tag = pChild->GetTagName();

				if ( tag == PSX_String("Element") )
				{
					String						semanticName		 = pChild->GetAttribute("semanticName");
					UINT						semanticIndex		 = pChild->GetAttribute("semanticIndex").ToUINT();
					EGraphicsFormat::Type		format				 = GraphicsMapping::ToGraphicsFormat( pChild->GetAttribute("format") );
					UINT						inputSlot			 = pChild->GetAttribute("inputSlot").ToUINT();
					UINT						alignedByteOffset	 = pChild->GetAttribute("alignedByteOffset").ToUINT();
					EInputClassification::Type	inputClass			 = GraphicsMapping::ToInputClassification( pChild->GetAttribute("inputClassification") );
					UINT						instanceDataStepRate = pChild->GetAttribute("instanceDataStepRate").ToUINT();
					
					m_desc.AddElement( semanticName.GetBuffer(), semanticIndex, format, inputSlot, alignedByteOffset,
						inputClass, instanceDataStepRate );
				}
				else
				{
					PSX_PushWarning( "Invalid tag name: %s. Skipping this node.", tag.GetBuffer() );
				}

				pChild = pChild->GetNextSibling();
			}

		}

		if ( m_desc.GetSize() )
		{
			ID3D11Device *pDevice = (ID3D11Device*)GraphicsDevice::GetInstance()->GetNakedDevice();

			// Convet to D3D desc
			Char buffer[256];
			D3D11_INPUT_ELEMENT_DESC *pDescArray = new D3D11_INPUT_ELEMENT_DESC[m_desc.GetSize()];

			for ( SIZE_T i = 0; i < m_desc.GetSize(); ++i )
			{
				D3D11_INPUT_ELEMENT_DESC *pDesc = &pDescArray[i];
				InputElementDesc *pElem = m_desc.GetElement( i );

				pDesc->SemanticName			= String::ToMultiByte( pElem->semanticName.GetBuffer(), buffer, 256 );
				pDesc->SemanticIndex		= pElem->semanticIndex;
				pDesc->Format				= DX11Mapping::ToDXBufferFormat( pElem->format );
				pDesc->InputSlot			= pElem->inputSlot;
				pDesc->AlignedByteOffset	= pElem->alignedByteOffset;
				pDesc->InputSlotClass		= DX11Mapping::ToDXInputClassification( pElem->inputClassification );
				pDesc->InstanceDataStepRate = pElem->instanceDataStepRate;
			}

			ID3DBlob *pCompiledShader = (ID3DBlob*)m_desc.pShaderForSignature->GetCompiledShader();
		
			if ( pDevice->CreateInputLayout( pDescArray, m_desc.GetSize(), pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pInputLayout ) != S_OK )
			{
				PSX_PushError( "Failed to create Input layout." );
				return EErrorCode::GRAPHICS;
			}
		}
		else
		{
			PSX_PushError("Invalid InputLayout description.");
			return EErrorCode::GRAPHICS;
		}

		return EErrorCode::NONE;
	}

	void InputLayout::Cleanup( void )
	{
		PSX_SafeRelease( m_pInputLayout );
	}

	void InputLayout::ParseElement( XMLElementPtr pNode )
	{

	}

}