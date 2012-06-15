/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	DepthStencilState.h
*
*	Description -	Depth and stencil buffer state settings
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/08/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_DEPTH_STENCIL_STATE_H_
#define _PSX_DEPTH_STENCIL_STATE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"

namespace Pulse
{
	// Stencil operations that can be performed based on the results of stencil test.
	struct StencilOp
	{
		EStencilOp::Type		failOp;			// The stencil operation to perform when stencil testing fails.
		EStencilOp::Type		depthFailOp;	//  operation to perform when stencil testing passes and depth testing fails.
		EStencilOp::Type		passOp;			// operation to perform when stencil testing and depth testing both pass.
		EComparisonFunc::Type	func;			// A function that compares stencil data against existing stencil data
	};

	struct DepthStencilStateDesc : public BaseResDesc
	{
		DepthStencilStateDesc( void );

		void SetDefault( void );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::CONSTANT_BUFFER; }

		BOOL					bDepthEnable;
		EBlendWriteMask::Type	depthWriteMask;		// Identify a portion of the depth-stencil buffer that can be modified by depth data
		EComparisonFunc::Type	depthFunc;			// compares depth data against existing depth data.
		BOOL					bStencilEnable;
		U8						stencilReadMask;	// Use EStencilReadMask // Identify a portion of the depth-stencil buffer for reading stencil data.
		U8						stencilWriteMask;	// Use EStencilWriteMask // Identify a portion of the depth-stencil buffer for writing stencil data.
		StencilOp				frontFace;
		StencilOp				backFace;
	};

	class DepthStencilState : public IResourcePoolItem
	{
	public:

		virtual ~DepthStencilState( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

		void * GetNakedDepthStencilState( void ) { return (void*)m_pDepthStencilState; }

	private:
		
		PSX_MakeStoragePoolFriend( DepthStencilState );

		DepthStencilState( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		DepthStencilStateDesc	m_desc;
		ID3D11DepthStencilState *m_pDepthStencilState;

	};
}

#endif /* _PSX_DEPTH_STENCIL_STATE_H_ */