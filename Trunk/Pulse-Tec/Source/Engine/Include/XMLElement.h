/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	XMLElement.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	08/10/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_XML_NODE_H_
#define _PSX_XML_NODE_H_

#include "PulseSTD.h"
#include "String.h"
#include "SmartPointer.h"

#include <Xercesc/DOM/DOM.hpp>

XERCES_CPP_NAMESPACE_USE

namespace Pulse
{
	class XMLElement
	{
	public:

		XMLElement( void );
		XMLElement( DOMNode *pNode );
		virtual ~XMLElement( void );

		void SetNode( XERCES_CPP_NAMESPACE::DOMDocument *pNode ) { m_pNode = pNode; }

		String GetID( void ) const;

		String GetAttribute( const Char *pAttribute ) const;

		String GetTagName( void ) const;

		String GetValue( void ) const;

		const SIZE_T GetNumChildren( void ) const;

		// Returns null if it doesn't exist
		XMLElementPtr GetNextSibling( void ) const;

		// Returns null if it doesn't exist
		XMLElementPtr GetFirstChild( void ) const;

		// Returns null if it doesn't exist
		XMLElementPtr GetChild( UINT index ) const;

		// Returns null if it doesn't exist
		XMLElementPtr GetParent( void ) const;

	private:

		DOMNode *m_pNode;

	};
}

#endif /* _PSX_XML_NODE_H_ */