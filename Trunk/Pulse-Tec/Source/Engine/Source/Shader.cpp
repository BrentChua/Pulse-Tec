/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Shader.cpp
*
*	Comments	-	See Shader.h
*
**************************************************************************************/
#include "../Include/Shader.h"
#include "../Include/Engine.h"
#include "../Include/FileSystem.h"
#include "../Include/ResourceCache.h"
#include "../Include/String.h"

namespace Pulse
{
	ShaderDesc::ShaderDesc( void )
	{
		type = EShaderType::UNKNOWN;
	}

	void ShaderDesc::SetAsVertexShader( const CHAR *pFilename, const CHAR *pFunction )		
	{
		SetDescription( EShaderType::VERTEX, pFilename, pFunction, PSX_String("vs_5_0") );
	}

	void ShaderDesc::SetAsHullShader( const CHAR *pFilename, const CHAR *pFunction )		
	{ 
		SetDescription( EShaderType::HULL, pFilename, pFunction, PSX_String("hs_5_0") );
	}
	
	void ShaderDesc::SetAsDomainShader( const CHAR *pFilename, const CHAR *pFunction )		
	{ 
		SetDescription( EShaderType::DOMAIN_, pFilename, pFunction, PSX_String("ds_5_0") );
	}

	void ShaderDesc::SetAsGeometryShader( const CHAR *pFilename, const CHAR *pFunction )	
	{ 
		SetDescription( EShaderType::GEOMETRY, pFilename, pFunction, PSX_String("gs_5_0") );
	}

	void ShaderDesc::SetAsPixelShader( const CHAR *pFilename, const CHAR *pFunction )		
	{ 
		SetDescription( EShaderType::PIXEL, pFilename, pFunction, PSX_String("ps_5_0") );
	}

	void ShaderDesc::SetDescription( EShaderType::Type shaderType, const CHAR *pFilename, const CHAR *pFunction, const CHAR *pProfile )
	{
		type = shaderType;
		filename = pFilename;
		function = pFunction;
		profile = pProfile;
	}

	Shader::Shader( void )
	{
		m_pCompiledShader = PSX_NULL;
	}

	Shader::~Shader( void )
	{

	}

	BOOL Shader::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::SHADER )
			return FALSE;

		m_desc = *((ShaderDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL Shader::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL Shader::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL Shader::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type Shader::CompileShader( void )
	{
		Renderer *pRenderer = Engine::GetInstance()->GetRenderer();
		ID3D11Device *pDevice = (ID3D11Device*)pRenderer->GetNakedDevice();
		ID3DBlob* pErrorMessages = NULL;
		HRESULT hr = 0;

		UINT flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef PSX_DEBUG
		flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif
		FileHandlePtr hFile = FileSystem::GetInstance()->OpenFile( m_desc.filename.GetBuffer() );

		if ( hFile->IsValid() == FALSE )
		{
			PSX_PushError( "Failed to open shader file, %s", m_desc.filename.GetBuffer() );
			return EErrorCode::GRAPHICS;
		}

		ResHandlePtr hResource = Engine::GetInstance()->GetResourceCache()->GetHandle( hFile );

		// NOTE: We might need the pDefine later on...
		Char asciiBuffer[1024];
		if ( FAILED( D3DX11CompileFromMemory( (LPCSTR)hResource->GetBuffer(), hResource->GetSize(),
			String::ToMultiByte( hFile->GetName(), asciiBuffer, 1024 ), PSX_NULL, PSX_NULL,
			String::ToMultiByte( m_desc.function.GetBuffer(), asciiBuffer, 1024 ), 
			String::ToMultiByte( m_desc.profile.GetBuffer(), asciiBuffer, 1024 ),
			flags, 0, PSX_NULL, &m_pCompiledShader, &pErrorMessages, &hr) ) )
		{
			String unicode( (Char*)pErrorMessages->GetBufferPointer() );
			PSX_PushError( "Failed to compile shader \"%s\" with error message: %s", hFile->GetName(), unicode.GetBuffer() );
			pErrorMessages->Release();

			return EErrorCode::GRAPHICS;
		}

		return EErrorCode::OKAY;
	}

	EErrorCode::Type Shader::QueryParameters( void )
	{
		ID3D11ShaderReflection *pReflector;
		HRESULT hr;

		hr = D3DReflect( m_pCompiledShader->GetBufferPointer(), 
			m_pCompiledShader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector );
	
		if ( FAILED( hr ) )
		{
			PSX_PushError( "Failed to query shader parameters." );
			return EErrorCode::GRAPHICS;
		}

		// Get the top level shader information, including the number of constant buffers,
		// as well as the number bound resources (constant buffers + other objects), the
		// number of input elements, and the number of output elements for the shader.
		pReflector->GetDesc( &m_d3dShaderDesc );

		// Get the input and output signature description arrays. These can be used later
		//	for verification of linking shader stages together.
		for ( UINT i = 0; i < m_d3dShaderDesc.InputParameters; ++i )
		{
			D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
			pReflector->GetInputParameterDesc( 0, &inputDesc );
			SignatureParameterDesc inputParam( String::ToUnicode(inputDesc.SemanticName).GetBuffer(), 
				inputDesc.SemanticIndex, inputDesc.Register, DX11Mapping::ToSystemValue(inputDesc.SystemValueType), 
				DX11Mapping::ToRegisterComponent(inputDesc.ComponentType), inputDesc.Mask, inputDesc.ReadWriteMask );
			m_inputSignature.PushBack( inputParam );
		}
		for ( UINT i = 0; i < m_d3dShaderDesc.OutputParameters; ++i )
		{
			D3D11_SIGNATURE_PARAMETER_DESC outputDesc;
			pReflector->GetInputParameterDesc( 0, &outputDesc );
			SignatureParameterDesc outputParam( String::ToUnicode(outputDesc.SemanticName).GetBuffer(), 
				outputDesc.SemanticIndex, outputDesc.Register, DX11Mapping::ToSystemValue(outputDesc.SystemValueType), 
				DX11Mapping::ToRegisterComponent(outputDesc.ComponentType), outputDesc.Mask, outputDesc.ReadWriteMask );
			m_outputSignature.PushBack( outputParam );
		}

		// Get the constant buffer information, which will be used for setting parameters
		//	for use by this shader at rendering time.
		for ( UINT i = 0; i < m_d3dShaderDesc.ConstantBuffers; ++i )
		{
			ID3D11ShaderReflectionConstantBuffer *pConstantBuffer = pReflector->GetConstantBufferByIndex( i );
		
			D3D11_SHADER_BUFFER_DESC desc;
			pConstantBuffer->GetDesc( &desc );

			if ( desc.Type == D3D_CT_CBUFFER || desc.Type == D3D_CT_TBUFFER )
			{
				ConstantBufferLayout bufferLayout;
				bufferLayout.desc.Set( String::ToUnicode(desc.Name).GetBuffer(), DX11Mapping::ToShaderBufferType(desc.Type),
					desc.Variables, desc.Size, desc.uFlags );
				
				for ( SIZE_T j = 0; j < desc.Variables; ++j )
				{
					ID3D11ShaderReflectionVariable *pVariable = pConstantBuffer->GetVariableByIndex(j);
					ShaderVariable shaderVariable;

					// Get variable desc
					D3D11_SHADER_VARIABLE_DESC desc;
					pVariable->GetDesc( &desc );
					shaderVariable.desc.Set( String::ToUnicode(desc.Name).GetBuffer(), desc.StartOffset, desc.Size, 
						desc.uFlags, desc.DefaultValue, desc.StartTexture, desc.TextureSize, desc.StartSampler, desc.SamplerSize );
				
					// Get variable type
					ID3D11ShaderReflectionType *pType = pVariable->GetType();
					D3D11_SHADER_TYPE_DESC typeDesc;
					pType->GetDesc( &typeDesc );
					shaderVariable.type.Set( DX11Mapping::ToShaderVariableClass( typeDesc.Class ), DX11Mapping::ToShaderVariableType(typeDesc.Type),
						typeDesc.Rows, typeDesc.Columns, typeDesc.Elements, typeDesc.Members, typeDesc.Offset, String::ToUnicode(typeDesc.Name).GetBuffer() );
				
					bufferLayout.variables.PushBack(shaderVariable);

					// Get the references to the parameters for binding to these variables.

				}


			}
		}

		pReflector->Release();

		return EErrorCode::OKAY;
	}

	EErrorCode::Type Shader::Initialize( void )
	{
		EErrorCode::Type retCode;

		if ( (retCode = CompileShader()) != EErrorCode::OKAY )
			return retCode;

		EErrorCode::Type ret = OnInitialize();

		if ( ret != EErrorCode::OKAY )
		{
			Cleanup();
			return EErrorCode::GRAPHICS;
		}

		if ( (retCode = QueryParameters()) != EErrorCode::OKAY )
			return retCode;

		return EErrorCode::OKAY;
	}

	void Shader::Cleanup( void )
	{
		PSX_SafeRelease( m_pCompiledShader );
		OnCleanup();
	}
}