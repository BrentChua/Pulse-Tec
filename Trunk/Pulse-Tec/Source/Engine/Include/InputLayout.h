/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	InputLayout.h
*
*	Description -	Input layout describing the vertex input for the graphics pipeline.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/10/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_INPUT_LAYOUT_H_
#define _PSX_INPUT_LAYOUT_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"
#include "Array.h"
#include "String.h"

namespace Pulse
{
	struct InputElementDesc 
	{
		String						semanticName;			// The HLSL semantic associated with this element in a shader input-signature.
		UINT						semanticIndex;			// The semantic index for the element. A semantic index modifies a semantic, with an integer index number. A semantic index is only needed in a case where there is more than one element with the same semantic. For example, a 4x4 matrix would have four components each with the semantic name "matrix". however each of the four components would have different smenatic indices (0, 1, 2, 3 ).
		EGraphicsFormat::Type		format;					// The data type of the element data
		UINT						inputSlot;				// An integer value that identifies the input-assembler (see input slot). Valid values are between 0 and 15 (defined in D3D11.h).
		UINT						alignedByteOffset;		// Optional. Offset (in bytes) between each element. Use D3D11_APPEND_ALIGNED_ELEMENT for convenience to define the current element directly after the previous one, including any packing if necessary.
		EInputClassification::Type	inputClassification;	// Identifies the input data class for a single input slot (see D3D11_INPUT_CLASSIFICATION).
		UINT						instanceDataStepRate;	// The number of instances to draw using the same per-instance data before advancing in the buffer by one element. This value must be 0 for an element that contains per-vertex data (the slot class is set to D3D11_INPUT_PER_VERTEX_DATA).
	};

	/* Sample
	{
		{ "POSITION",	0, R32G32B32_FLOAT, 0, 0,	PER_VERTEX_DATA, 0 },
		{ "TEXTURE0",	0, R32G32_FLOAT,	0, 12,	PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, R32G32B32_FLOAT, 1, 0,	PER_VERTEX_DATA, 0 },
		{ "MATRIX",		0, R32G32B32_FLOAT, 2, 0,	PER_VERTEX_DATA, 0 },
		{ "MATRIX",		1, R32G32B32_FLOAT, 2, 12,	PER_VERTEX_DATA, 0 },
		{ "MATRIX",		2, R32G32B32_FLOAT, 2, 24,	PER_VERTEX_DATA, 0 },
		{ "MATRIX",		3, R32G32B32_FLOAT, 2, 36,	PER_VERTEX_DATA, 0 },
	};
	*/

	struct InputLayoutDesc : public BaseResDesc
	{
	public:
		
		InputLayoutDesc( void );

		InputLayoutDesc( const CHAR *pFilename ) { SetFilename( pFilename ); }

		void SetShaderForLayoutSignature( Shader *pShader ) { pShaderForSignature = pShader; }

		void AddElement( const CHAR *pSemanticName,UINT semanticIndex, EGraphicsFormat::Type format,
			UINT inputSlot, UINT alignedByteOffset, EInputClassification::Type inputClassification, UINT instanceDataStepRate );

		void AddElementAsPerVertexData( const CHAR *pSemanticName,UINT semanticIndex, EGraphicsFormat::Type format,
			UINT inputSlot, UINT alignedByteOffset );

		void AddElementAsPerInstanceData( const CHAR *pSemanticName,UINT semanticIndex, EGraphicsFormat::Type format,
			UINT inputSlot, UINT alignedByteOffset, UINT instanceDataStepRate );

		void SetFilename( const CHAR *pFilename );

		const CHAR * GetFilename( void ) { return m_filename.GetBuffer(); }

		InputElementDesc * GetElement( UINT index );

		const SIZE_T GetSize( void ) const;

		void RemoveElement( UINT index );

		void ClearElements( void );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::INPUT_LAYOUT; }

	private:

		friend InputLayout;

		typedef Array<InputElementDesc> ElementDescList;

		Shader			*pShaderForSignature;	// Any shader using specified input layout.
		ElementDescList elementDescList;
		String			m_filename;
	};

	class InputLayout : public IResourcePoolItem
	{
	public:

		virtual ~InputLayout( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

		void * GetNakedInputLayout( void ) { return (void*)m_pInputLayout; }

	private:

		PSX_MakeStoragePoolFriend( InputLayout );

		InputLayout( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

		// File load
		void ParseElement( XMLElementPtr pNode );

	private:

		InputLayoutDesc		m_desc;
		ID3D11InputLayout	*m_pInputLayout;

	};


}

#endif /* _PSX_INPUT_LAYOUT_H_ */