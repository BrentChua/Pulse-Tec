/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	XMLParser.h
*
*	Description -	XML Parser Wrapper
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	08/08/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_XML_PARSER_H_
#define _PSX_XML_PARSER_H_

#include "PulseSTD.h"
#include "Singleton.h"
#include "XMLElement.h"

//#define PLATFORM_IMPORT     __declspec(dllimport)

#ifndef __MSXML_LIBRARY_DEFINED__
#define __MSXML_LIBRARY_DEFINED__
#endif

#include <Xercesc/Util/PlatformUtils.hpp>
#include <Xercesc/Parsers/XercesDOMParser.hpp>
#include <Xercesc/Util/XMLString.hpp>
#include <Xercesc/Framework/MemBufInputSource.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

XERCES_CPP_NAMESPACE_USE

namespace Pulse
{
	class XMLParser /*: public Singleton< XMLParser >*/
	{
	public:
		
		static EErrorCode::Type Initialize( void );

		static void Shutdown( void );

		static XMLElementPtr Load( const CHAR *pFilename );

	private:

		friend class Singleton<XMLParser>;

		XMLParser( void ) { }
		XMLParser( XMLParser & ) { }
		virtual ~XMLParser( void );
		XMLParser & operator = ( XMLParser &rhs );

	private:

		static void SetValidate( BOOL validate );

	private:

		class DOMErrorHandler: public DefaultHandler
		{
		public:
			void warning(const SAXParseException& e);
			void error(const SAXParseException& e);
			void fatalError(const SAXParseException& e);
		};

		static DOMErrorHandler m_errorHandler;
		static XercesDOMParser* m_pParser;					///< Xerces DOM parser
		static BOOL m_bValidation;

	};
}

#endif /* _PSX_XML_PARSER_H_ */