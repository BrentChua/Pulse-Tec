/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	SwapChain.h
*
*	Description -	
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/22/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_SWAP_CHAIN_H_
#define _PSX_SWAP_CHAIN_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsResource.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"

namespace Pulse
{
	struct SwapChainDesc : public BaseResDesc
	{
		SwapChainDesc( SIZE_T _width = 0, SIZE_T _height = 0, Rational _refreshRate = Rational(), 
			EGraphicsFormat::Type _format = EGraphicsFormat::UNKNOWN, SampleDesc _sampling = SampleDesc(), BOOL _bWindowed = TRUE )
			:width(_width), height(_height), refreshRate( _refreshRate ), format(_format),
			sampleDesc( _sampling ), bWindowed(_bWindowed)
		{
		}



		virtual EResDescType::Type GetDescType( void ) { return EResDescType::SWAP_CHAIN; }

		SIZE_T					width;
		SIZE_T					height;
		Rational				refreshRate;
		EGraphicsFormat::Type	format;
		SampleDesc				sampleDesc;		
		BOOL					bWindowed;
	};

	class SwapChain : public IResourcePoolItem, public GraphicsResource
	{
	public:

		virtual ~SwapChain( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

	private:

		PSX_MakeStoragePoolFriend( SwapChain );

		SwapChain( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

		SwapChainDesc	m_desc;
		IDXGISwapChain	*m_pSwapChain;
		Texture2D		*m_pSwapChainBuffer;
	};
}

#endif /* _PSX_SWAP_CHAIN_H_ */