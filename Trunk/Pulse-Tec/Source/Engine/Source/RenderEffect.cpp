/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RenderEffect.cpp
*
*	Comments	-	See RenderEffect.h
*
**************************************************************************************/
#include "../Include/RenderEffect.h"
#include "../Include/XMLParser.h"
#include "../Include/GraphicsResourceManager.h"

namespace Pulse
{
	RenderEffect::RenderEffect( void )
	{

	}

	RenderEffect::~RenderEffect( void )
	{
		Cleanup();
	}

	BOOL RenderEffect::CreateResource( BaseResDesc *pDesc )	
	{
		if ( pDesc->GetDescType() != EResDescType::BLEND_STATE )
			return FALSE;

		m_desc = *((RenderEffectDesc*)pDesc);

		if ( Initialize() == EErrorCode::OKAY )
			SetFlagCreated();

		return IsCreated();	
	}

	BOOL RenderEffect::DestroyResource( void )				
	{
		Cleanup();

		SetFlagDestroyed();	
		return TRUE;	
	}

	BOOL RenderEffect::DisableResource( void )				
	{
		SetFlagDisabled();
		return TRUE;	
	}

	BOOL RenderEffect::RestoreResource( void )				
	{ 
		SetFlagRestored();	
		return TRUE;	
	}

	EErrorCode::Type RenderEffect::Initialize( void )
	{
		if ( m_desc.m_filename.GetLength() == 0 )
		{
			PSX_PushError( "No filename specified to load an effect.");
			return EErrorCode::GRAPHICS;
		}

		XMLElementPtr pNode = XMLParser::Load( m_desc.m_filename.GetBuffer() );

		if ( pNode.IsNull() )
		{
			PSX_PushError( "Can't find filename %s to load render effect.", m_desc.m_filename.GetBuffer() );
			return EErrorCode::GRAPHICS;
		}

		if ( pNode->GetTagName() != PSX_String( "Effect" ) )
		{
			PSX_PushError( "Invalid tag name. Should have an \"Effect\" tag name." );
			return EErrorCode::GRAPHICS;
		}

		XMLElementPtr pChild = pNode->GetFirstChild();
		String tag;
		RenderPass *pNewPass;

		while ( pChild )
		{
			if ( pChild->GetTagName() == PSX_String("RenderPass") )
			{
				GraphicsResourceManager *pGM = GraphicsResourceManager::GetInstance();
				ShaderDesc desc;
				Shader *pShader;
				pNewPass				= new RenderPass;
				String inputLayout		= pChild->GetAttribute( "inputLayout" );
				String vertexShader		= pChild->GetAttribute( "vertexShader" );
				String hullShader		= pChild->GetAttribute( "hullShader" );
				String domainShader		= pChild->GetAttribute( "domainShader" );
				String geometryShader	= pChild->GetAttribute( "geometryShader" );
				String pixelShader		= pChild->GetAttribute( "pixelShader" );
				String computeShader	= pChild->GetAttribute( "computeShader" );

				if ( inputLayout.GetLength() )
				{
					InputLayoutDesc desc( inputLayout.GetBuffer() );
					InputLayout *pIL = pGM->CreateInputLayout( inputLayout.GetBuffer(), &desc );
					pNewPass->SetInputLayout( pIL );
					pIL->Release();
				}
				else
				{
					PSX_PushError( "No inputLayout associated with this effect." );
					delete pNewPass;
					return EErrorCode::GRAPHICS;
				}

				if ( vertexShader.GetLength() )
				{
					desc.SetAsVertexShader( vertexShader.GetBuffer() );
					pShader = pGM->CreateVertexShader( vertexShader.GetBuffer(), &desc );
					pNewPass->SetVertexShader( pShader );
					pShader->Release();
				}

				if ( pixelShader.GetLength() )
				{
					desc.SetAsPixelShader( pixelShader.GetBuffer() );
					pGM->CreatePixelShader( pixelShader.GetBuffer(), &desc );
					pNewPass->SetPixelShader( pShader );
					pShader->Release();
				}

				// TODO: Add the rest of the shaders to load.
			
				
			}
			else
				PSX_PushWarning( "Invalid non RenderPass tag name. Skipping element." );
		
			pChild = pChild->GetNextSibling();
		}

		return EErrorCode::OKAY;
	}

	void RenderEffect::Cleanup( void )
	{
		ClearRenderPasses();
	}

	void RenderEffect::AddRenderPass( RenderPass *pRenderPass )
	{
		if ( pRenderPass )
			m_renderPasses.PushBack( pRenderPass );
		else
			PSX_PushError( "pRenderPass can't be null." );
	}

	void RenderEffect::ClearRenderPasses( void )
	{
		for ( SIZE_T i = 0; i < m_renderPasses.GetSize(); ++i )
			delete m_renderPasses[i];

		m_renderPasses.Clear();
	}

}