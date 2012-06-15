/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	IndexBuffer.h
*
*	Description -	Index buffer
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/23/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_INDEX_BUFFER_H_
#define _PSX_INDEX_BUFFER_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "GraphicsResource.h"
#include "GraphicsBuffer.h"

namespace Pulse
{
	struct IndexBufferDesc : public BaseResDesc
	{
		IndexBufferDesc( void );

		void SetAsDynamicIndexBuffer( SIZE_T _numIndices, void *pInitialData = PSX_NULL );
		
		void SetAsStaticIndexBuffer( SIZE_T _numIndices, void *pInitialData );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::INDEX_BUFFER; }

		SIZE_T					indexSize;
		SIZE_T					numIndices;
		EBufferUsage::Type		bufferUsage;
		void					*pData;

	};

	class IndexBuffer : public IResourcePoolItem, public GraphicsResource, public GraphicsBuffer
	{
	public:

		virtual ~IndexBuffer( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

		void * GetNakedIndexBuffer( void ) { return (void*)m_pBuffer; }

		IndexBufferDesc * GetDescription( void ) { return &m_desc; }

	private:

		IndexBuffer( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		PSX_MakeStoragePoolFriend( IndexBuffer );

		IndexBufferDesc		m_desc;
		ID3D11Buffer	*m_pBuffer;

		// Cache Index format
	};
}

#endif /* _PSX_INDEX_BUFFER_H_ */