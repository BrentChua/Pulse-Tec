/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	XMLParser.cpp
*
*	Comments	-	See XMLParser.h
*
**************************************************************************************/
#include "../Include/XMLParser.h"
#include "../Include/String.h"
#include "../Include/FileSystem.h"
#include "../Include/Engine.h"
#include "../Include/ResourceCache.h"
#include "../Include/XMLElement.h"

#if PSX_DEBUG
	#pragma comment(lib, "External/Xerces/lib/xerces-c_3D.lib")
#else
	#pragma comment(lib, "External/Xerces/lib/xerces-c_3.lib")
#endif


namespace Pulse
{

	PSX_REGISTRY_OBJECT_START( XMLParser, ERegistryPriority::Normal, ERegistryPriority::Normal )
		PSX_REGISTRY_ON_TEST()
		{
			//XMLParser *pParser = XMLParser::GetInstance();
			//pParser->Initialize();

			//pParser->Load( "Resources/XML/personal.xml" );


			//pParser->Shutdown();
			return EErrorCode::NONE;
		}
	PSX_REGISTRY_OBJECT_END();

	XMLParser::DOMErrorHandler XMLParser::m_errorHandler;
	XercesDOMParser * XMLParser::m_pParser = PSX_NULL;
	BOOL XMLParser::m_bValidation = TRUE;

	XMLParser::~XMLParser( void )
	{

	}

	EErrorCode::Type XMLParser::Initialize( void )
	{
		XMLPlatformUtils::Initialize();
		m_pParser = new XercesDOMParser();

		// Configure parser
		SetValidate(FALSE);

		m_pParser->cacheGrammarFromParse(true);
		m_pParser->setErrorHandler(&m_errorHandler);

		return EErrorCode::NONE;
	}

	void XMLParser::Shutdown( void )
	{
		m_pParser->resetCachedGrammarPool();
		PSX_SafeDelete(m_pParser);
		XMLPlatformUtils::Terminate();
	}

	XMLElementPtr XMLParser::Load( const CHAR *pFilename )
	{
		//Load xml file
		FileHandlePtr hFile = FileSystem::GetInstance()->OpenFile( pFilename );
		if ( !hFile )
		{
			PSX_PushError( "Failed to load texture file %s.", pFilename );
			return PSX_NULL;
		}
		ResHandlePtr hResource =  Engine::GetInstance()->GetResourceCache()->GetHandle( hFile );

		_TRY_BEGIN
			int bufferSize = MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)hResource->GetBuffer(), hResource->GetSize(), 0, 0 ); 
			wchar_t *pWideBuffer = new wchar_t[bufferSize];
			char *pTemp;
			const char *pBufferID = "buffer";

			MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)hResource->GetBuffer(), hResource->GetSize(), pWideBuffer, bufferSize ); 
			 pTemp = XMLString::transcode((XMLCh*)pWideBuffer );
			 delete [] pWideBuffer;
			
			MemBufInputSource src( (const XMLByte*)pTemp, bufferSize, pBufferID );
			
			m_pParser->parse( src );
			DOMNode *pNode = m_pParser->getDocument();

			if ( !pNode )
			{
				PSX_PushError( "Data cannot be parsed correctly" );
				XMLString::release( &pTemp );
				return PSX_NULL;
			}

			XMLString::release( &pTemp );

			XERCES_CPP_NAMESPACE::DOMDocument* pDocument = static_cast<XERCES_CPP_NAMESPACE::DOMDocument*>(pNode);

			return new XMLElement( pDocument->getDocumentElement() );
		
		_CATCH(std::bad_cast)
			PSX_PushError("Cast could not be performed!");
			return PSX_NULL;
		_CATCH_END
		
		return PSX_NULL;
	}

	void XMLParser::SetValidate( BOOL validate )
	{
		m_pParser->setDoSchema( validate );
		m_pParser->setDoSchema( validate );

		if ( validate )
			m_pParser->setValidationScheme(XercesDOMParser::Val_Always);
		else
			m_pParser->setValidationScheme(XercesDOMParser::Val_Never);

		m_bValidation = validate;
	}


	//----------------------------------------------------------------------
	
	void XMLParser::DOMErrorHandler::warning(const SAXParseException& e)
	{
		PSX_PushWarning( "%ls on line %d", e.getMessage(), e.getLineNumber() );
	}

	void XMLParser::DOMErrorHandler::error(const SAXParseException& e)
	{
		PSX_PushError( "%ls on line %d", e.getMessage(), e.getLineNumber() );
	}

	void XMLParser::DOMErrorHandler::fatalError(const SAXParseException& e)
	{
		PSX_PushError( "%ls on line %d", e.getMessage(), e.getLineNumber() );
	}

}