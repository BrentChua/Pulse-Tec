/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsMapping.h
*
*	Description -	
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/13/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GRAPHICS_MAPPING_H_
#define _PSX_GRAPHICS_MAPPING_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "String.h"

namespace Pulse
{
	class GraphicsMapping
	{
	public:

		static EGraphicsFormat::Type ToGraphicsFormat( String format );

		static EInputClassification::Type ToInputClassification( String inputClassification );
	};
}

#endif /* _PSX_GRAPHICS_MAPPING_H_ */