/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	GraphicsBuffer.h
*
*	Description -	Base class containing DX11 buffer.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/17/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GRAPHICS_BUFFER_H_
#define _PSX_GRAPHICS_BUFFER_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "GraphicsInfrastructure.h"

namespace Pulse
{

	class GraphicsBuffer
	{
	public:

		GraphicsBuffer( void );

		virtual ~GraphicsBuffer( void );

		void * GetNakedBuffer( void ) { return (void*)m_pBuffer; }

	protected:

		void ReleaseBuffer( void );

	protected:

		friend Renderer;

		ID3D11Buffer *m_pBuffer;

	};

}

#endif /* _PSX_GRAPHICS_BUFFER_H_ */