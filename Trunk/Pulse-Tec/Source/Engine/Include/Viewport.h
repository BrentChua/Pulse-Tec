/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Viewport.h
*
*	Description -	Viewport
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/21/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_VIEWPORT_H_
#define _PSX_VIEWPORT_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"

namespace Pulse
{
	struct ViewportDesc : public BaseResDesc
	{

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::VIEWPORT; }

		FLOAT topLeftX;
		FLOAT topLeftY;
		FLOAT width;
		FLOAT height;
		FLOAT minDepth;
		FLOAT maxDepth;
	};

	class Viewport : public IResourcePoolItem
	{
	public:
		
		virtual ~Viewport( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	
	
	private:

		PSX_MakeStoragePoolFriend( Viewport );

		Viewport( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		ViewportDesc m_desc;
		D3D11_VIEWPORT m_viewport;

	};
}

#endif /* _PSX_VIEWPORT_H_ */
