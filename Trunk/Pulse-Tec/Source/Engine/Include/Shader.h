/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Shader.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/06/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_SHADER_H_
#define _PSX_SHADER_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "GraphicsBuffer.h"
#include "String.h"
#include "Array.h"

namespace Pulse
{

	struct ShaderDesc : public BaseResDesc
	{
	public:

		ShaderDesc( void );

		void SetAsVertexShader( const CHAR *pFilename, const CHAR *pFunction = PSX_String("Main") );	
		void SetAsHullShader( const CHAR *pFilename, const CHAR *pFunction = PSX_String("Main") );	
		void SetAsDomainShader( const CHAR *pFilename, const CHAR *pFunction = PSX_String("Main") );	
		void SetAsGeometryShader( const CHAR *pFilename, const CHAR *pFunction = PSX_String("Main") );
		void SetAsPixelShader( const CHAR *pFilename, const CHAR *pFunction = PSX_String("Main") );	

		void SetAsComputeShader( const CHAR *pFilename, const CHAR *pFunction, const CHAR *pProfile )	{ SetDescription( EShaderType::COMPUTE, pFilename, pFunction, pProfile ); }

		void SetDescription( EShaderType::Type shaderType, const CHAR *pFilename, const CHAR *pFunction, const CHAR *pProfile );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::SHADER; }

		EShaderType::Type	type;		// Shader type
		String				filename;	// file name
		String				function;	// Function it'll look for in the file
		String				profile;	// Shader profile(version) it will compile against

	};

	struct SignatureParameterDesc
	{
		SignatureParameterDesc( const CHAR *pSemanticName = PSX_String(""), UINT _semanticIndex = 0,
			UINT _registerIndex = 0, ESystemValueType::Type _sysValType = ESystemValueType::UNDEFINED, 
			ERegisterComponentType::Type _componentType = ERegisterComponentType::UNKNOWN,
			BYTE _mask = 0, BYTE _readWriteMask = 0 )
		{
			Set( pSemanticName, _semanticIndex, _registerIndex, _sysValType, _componentType, _mask, _readWriteMask );
		}

		void Set( const CHAR *pSemanticName, UINT _semanticIndex, UINT _registerIndex, 
			ESystemValueType::Type _sysValType, ERegisterComponentType::Type _componentType, BYTE _mask, BYTE _readWriteMask )
		{
			semanticName	= pSemanticName;
			semanticIndex	= _semanticIndex;
			registerIndex	= _registerIndex;
			systemValueType = _sysValType;
			componentType	= _componentType;
			mask			= _mask;
			readWriteMask	= _readWriteMask;
		}

		String							semanticName;
		UINT							semanticIndex;
		UINT							registerIndex;
		ESystemValueType::Type			systemValueType;
		ERegisterComponentType::Type	componentType;
		BYTE							mask;
		BYTE							readWriteMask;
	};

	// Shader variable description
	struct ShaderVariableDesc
	{
		ShaderVariableDesc( void )
		{
			offset = 0;
			size = 0;
			flags = 0;
			defaultValue = 0;
			startTexture = -1;
			textureSize = 0;
			startSampler = -1;
			samplerSize = 0;
		}

		void Set( const CHAR *pName, UINT _offset, SIZE_T _size, UINT _flags, PVOID _defaultValue,
			UINT _startTexture, SIZE_T _textureSize, UINT _startSampler, UINT _samplerSize )
		{
			name = pName;
			offset = _offset;
			size = _size;
			flags = _flags;
			defaultValue = _defaultValue;
			startTexture = _startTexture;
			textureSize = _textureSize;
			startSampler = _startSampler;
			samplerSize = _samplerSize;
		}

		String	name;			// Name of the variable
		UINT	offset;			// Offset in constant buffer's backing store
		SIZE_T	size;			// Size of variable (in bytes)
		UINT	flags;			// Variable flags
		PVOID	defaultValue;	// Raw pointer to default value
		UINT	startTexture;	// First texture index (or -1 if no textures used)
		SIZE_T	textureSize;	// Number of texture slots possibly used.
		UINT	startSampler;	// First sampler index (or -1 if no textures used)
		UINT	samplerSize;	// Number of sampler slots possibly used.
	};

	// Shader variable's type
	struct ShaderVariableType
	{
		ShaderVariableType( void )
		{
			classType = EShaderVariableClass::SCALAR;
			variableType = EShaderVariableType::_VOID;
			numRows = 0;
			numColumns = 0;
			numElements = 0;
			numMembers = 0;
			offset = 0;
		}

		void Set( EShaderVariableClass::Type _classType, EShaderVariableType::Type _varType, UINT _numRows,
			UINT _numColumns, UINT _numElements, UINT _numMembers, UINT _offset, const CHAR *pName )
		{
			classType = _classType;
			variableType = _varType;
			numRows = _numRows;
			numColumns = _numColumns;
			numElements = _numElements;
			numMembers = _numMembers;
			offset = _offset;
			name = pName;
		}

		EShaderVariableClass::Type classType;	// identifies the variable class as one of scalar, vector, matrix or object.
		EShaderVariableType::Type variableType; // The variable type
		UINT numRows;							 // Number of rows (for matrices, 1 for other numeric, 0 if not applicable)
		UINT numColumns;						 // Number of columns (for vectors & matrices, 1 for other numeric, 0 if not applicable)
		UINT numElements;						 // Number of elements (0 if not an array)
		UINT numMembers;						 // Number of members (0 if not a structure)
		UINT offset;							 // Offset from the start of structure (0 if not a structure member)
		String name;							 // Name of type, can be NULL
	};

	struct ShaderVariable
	{
		ShaderVariableDesc desc; // Variable desciption
		ShaderVariableType type; // Type description
	};

	// Shader buffer description
	struct ShaderBufferDesc
	{
		ShaderBufferDesc()
		{
			bufferType = EShaderBufferType::CONSTANT_BUFFER;
			numVariables = 0;
			bufferSize = 0;
			flags = 0;
		}

		void Set( const CHAR *pName, EShaderBufferType::Type _bufferType, SIZE_T _numVars, SIZE_T _bufferSize, UINT _flags )
		{
			name = pName;
			bufferType = _bufferType;
			numVariables = _numVars;
			bufferSize = _bufferSize;
			flags = _flags;
		}

		String					name;			// The name of the buffer.
		EShaderBufferType::Type bufferType;		// The intended use of the constant data.
		SIZE_T					numVariables;	// Number of unique variables.
		SIZE_T					bufferSize;		// Buffer size in bytes.
		UINT					flags;			// Shader buffer properties.
	};

	struct ConstantBufferLayout
	{
		typedef Array<ShaderVariable> VariableList;
		
		ShaderBufferDesc	desc;
		VariableList		variables;
		// TODO: Variable params

	};

	class Shader : public IResourcePoolItem, public GraphicsBuffer
	{
	public:

		virtual ~Shader( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

		virtual EShaderType::Type GetType( void ) = 0;

		void * GetCompiledShader( void ) { return (void*)m_pCompiledShader; }

		virtual void * GetNakedShader( void ) = 0;

	protected:

		Shader( void );

		EErrorCode::Type CompileShader( void );

		EErrorCode::Type QueryParameters( void );

		virtual EErrorCode::Type OnInitialize( void ) { return EErrorCode::OKAY; }

		virtual void OnCleanup( void ) { }

	private:

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	protected:

		typedef Array<SignatureParameterDesc> SignatureParameters;

		ShaderDesc			m_desc;
		D3D11_SHADER_DESC	m_d3dShaderDesc;
		ID3DBlob			*m_pCompiledShader;
		SignatureParameters	m_inputSignature;
		SignatureParameters m_outputSignature;
	};

}

#endif /* _PSX_SHADER_H_ */