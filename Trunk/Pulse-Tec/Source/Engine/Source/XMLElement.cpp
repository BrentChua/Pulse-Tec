/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	XMLElement.cpp
*
*	Comments	-	See XMLElement.h
*
**************************************************************************************/
#include "../Include/XMLElement.h"

namespace Pulse
{
	XMLElement::XMLElement( void )
		: m_pNode( PSX_NULL )
	{
	}

	XMLElement::XMLElement( DOMNode *pNode )
		: m_pNode( pNode )
	{
	}
	
	XMLElement::~XMLElement( void )
	{
		m_pNode = PSX_NULL;
	}

	String XMLElement::GetID( void ) const
	{
		return GetAttribute( "id" );
	}

	String XMLElement::GetAttribute( const Char *pAttribute ) const
	{
		_TRY_BEGIN
			const DOMElement& element = static_cast<const DOMElement&>(*m_pNode);
			XMLCh* nameKey = XMLString::transcode(pAttribute);
			char* temp = XMLString::transcode(element.getAttribute(nameKey));

			String result = temp;
			
			// Release resources
			XMLString::release(&nameKey);
			XMLString::release(&temp);

			return result;
		_CATCH(std::bad_cast)
			PSX_PushError("Cast could not be performed!");
			return PSX_String("");
		_CATCH_END
	}

	String XMLElement::GetTagName( void ) const
	{
		_TRY_BEGIN
			short type = m_pNode->getNodeType();
			char* temp = NULL;
			if(type == DOMNode::ELEMENT_NODE)
			{
				const DOMElement& element = static_cast<const DOMElement&>(*m_pNode);
				temp = XMLString::transcode(element.getTagName());
			}
			else if(type == DOMNode::TEXT_NODE)
			{
				const DOMText& element = static_cast<const DOMText&>(*m_pNode);
				temp = XMLString::transcode(element.getData());
			}

			if(temp)
			{
				String result = temp;
				XMLString::release(&temp);

				return result;
			}
		_CATCH(std::bad_cast)
			PSX_PushError( "Cast could not be performed!" );
		_CATCH_END

		return PSX_String("");
	}

	String XMLElement::GetValue( void ) const
	{
		_TRY_BEGIN
			short type = m_pNode->getNodeType();
			char* temp = NULL;

			if(type == DOMNode::ELEMENT_NODE)
			{
				const DOMElement& element = static_cast<const DOMElement&>(*m_pNode);
				temp = XMLString::transcode(element.getTextContent());
			}

			if(temp)
			{
				String result = temp;
				XMLString::release(&temp);

				return result;
			}
		_CATCH(std::bad_cast)
			PSX_PushError("Cast could not be performed!");
		_CATCH_END

		return PSX_String("");
	}

	const SIZE_T XMLElement::GetNumChildren( void ) const
	{
		return m_pNode->getChildNodes()->getLength();
	}

	XMLElementPtr XMLElement::GetNextSibling( void ) const
	{
		if(m_pNode->getNextSibling())
			return (new XMLElement(m_pNode->getNextSibling()));
		else
			return PSX_NULL;
	}

	XMLElementPtr XMLElement::GetFirstChild( void ) const
	{
		if(m_pNode->hasChildNodes())
			return (new XMLElement(m_pNode->getFirstChild()));
		else
			return PSX_NULL;
	}

	XMLElementPtr XMLElement::GetChild( UINT index ) const
	{
		if(m_pNode->getChildNodes()->getLength() > index)
			return (new XMLElement(m_pNode->getChildNodes()->item(index)));
		else
			return PSX_NULL;
	}

	XMLElementPtr XMLElement::GetParent( void ) const
	{
		if(m_pNode->getParentNode())
			return (new XMLElement(m_pNode->getParentNode()));
		else
			return PSX_NULL;
	}

}