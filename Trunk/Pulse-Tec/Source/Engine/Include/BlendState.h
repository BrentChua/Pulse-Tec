/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	BlendState.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/01/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_BLEND_STATE_H_
#define _PSX_BLEND_STATE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"

namespace Pulse
{
	struct RenderTargetBlendDesc
	{
		BOOL				blendEnable; // Enable (or disable) blending.
		EBlendType::Type	srcBlend;
		EBlendType::Type	destBlend;
		EBlendOp::Type		blendOp;
		EBlendType::Type	srcBlendAlpha;
		EBlendType::Type	destBlendAlpha;
		EBlendOp::Type		blendOpAlpha;
		BYTE				renderTargetWriteMask; // Use EColorWriteEnable.
	};

	struct BlendStateDesc : public BaseResDesc
	{

		BlendStateDesc( void );

		void SetDefault( void );
		
		virtual EResDescType::Type GetDescType( void ) { return EResDescType::BLEND_STATE; }

		BOOL					bAlphaToCoverageEnable;	// Specifies whether to use alpha-to-coverage as a multisampling technique when setting a pixel to a render target. For more info about using alpha-to-coverage, see Alpha-To-Coverage.
		BOOL					bIndependentBlendEnable;	// Specifies whether to enable independent blending in simultaneous render targets. Set to TRUE to enable independent blending. If set to FALSE, only the RenderTarget[0] members are used; RenderTarget[1..7] are ignored.
		RenderTargetBlendDesc	renderTargets[8];
	};

	class BlendState : public IResourcePoolItem
	{
	public:

		virtual ~BlendState( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

		BlendStateDesc * GetDescription( void ) { return &m_desc; }

		void * GetNakedBlendState( void ) { return (void*)m_pBlendState; }

	private:

		PSX_MakeStoragePoolFriend( BlendState );

		BlendState( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		BlendStateDesc m_desc;
		ID3D11BlendState *m_pBlendState;
	
	};
}

#endif /* _PSX_BLEND_STATE_H_ */