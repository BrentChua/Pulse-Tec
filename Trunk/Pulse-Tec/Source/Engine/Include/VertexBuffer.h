/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	VertexBuffer.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/05/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_VERTEX_BUFFER_H_
#define _PSX_VERTEX_BUFFER_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "GraphicsResource.h"
#include "GraphicsBuffer.h"
#include "String.h"

namespace Pulse
{
	struct VertexBufferDesc : public BaseResDesc
	{
		VertexBufferDesc( void );

		void SetAsDynamicVertexBuffer( SIZE_T _vertexSize, SIZE_T _numVertices, void *pVertexData = PSX_NULL );
		
		void SetAsStaticVertexBuffer( SIZE_T _vertexSize, SIZE_T _numVertices, void *pVertexData );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::VERTEX_BUFFER; }
		
		EVertexBufferType::Type bufferType;
		SIZE_T					vertexSize;
		SIZE_T					numVertices;
		SIZE_T					numPrimitives;
		void					*pData;
	};

	class VertexBuffer : public IResourcePoolItem, public GraphicsResource, public GraphicsBuffer
	{
	public:

		virtual ~VertexBuffer( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

		void * GetNakedVertexBuffer( void ) { return (void*)m_pBuffer; }

		VertexBufferDesc * GetDescription( void ) { return &m_desc; }

	private:

		VertexBuffer( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		PSX_MakeStoragePoolFriend( VertexBuffer );

		VertexBufferDesc	m_desc;
		ID3D11Buffer		*m_pBuffer;

	};
}

#endif /* _PSX_VERTEX_BUFFER_H_ */