/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ConstantBuffer.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/24/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_CONSTANT_BUFFER_H_
#define _PSX_CONSTANT_BUFFER_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "GraphicsResource.h"
#include "GraphicsBuffer.h"
#include "ResourcePoolItem.h"

namespace Pulse
{
	struct ConstantBufferDesc : public BaseResDesc
	{
		ConstantBufferDesc( void );

		void SetAsDynamicConstantBuffer( SIZE_T _bufferSize, BOOL _bAutoUpdate = TRUE );

		void SetAsStaticConstantBuffer( SIZE_T _bufferSize, void *pInitialData, BOOL _bAutoUpdate = TRUE );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::CONSTANT_BUFFER; }

		SIZE_T				bufferSize;
		EBufferUsage::Type	bufferUsage;
		BOOL				bAutoUpdate;
		void				*pData;

	};

	class ConstantBuffer : public IResourcePoolItem, public GraphicsResource, public GraphicsBuffer
	{
	public:

		virtual ~ConstantBuffer( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );

	private:

		ConstantBuffer( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

		void * Map( void );

		void UnMap( void );

	private:

		PSX_MakeStoragePoolFriend( ConstantBuffer );

		ConstantBufferDesc	m_desc;
	};
}

#endif /* _PSX_CONSTANT_BUFFER_H_ */