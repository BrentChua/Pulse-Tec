/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RenderEffect.h
*
*	Description -	
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/05/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RENDER_TECHINIQUE_H_
#define _PSX_RENDER_TECHINIQUE_H_

#include "PulseSTD.h"
#include "RenderPass.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"
#include "String.h"

namespace Pulse
{
	struct RenderEffectDesc
	{
	public:

		RenderEffectDesc( void ) { }

		RenderEffectDesc( const CHAR *pFilename ) { SetFilename( pFilename ); }

		void SetFilename( const CHAR *pFilename ) { m_filename = pFilename; }

		String m_filename;
	};

	class RenderEffect : public IResourcePoolItem
	{
	public:

		virtual ~RenderEffect( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

		RenderEffectDesc * GetDescription( void ) { return &m_desc; }

		RenderPass * GetRenderPass( INT index ) { return m_renderPasses[index]; }

		SIZE_T GetNumPasses( void ) { return m_renderPasses.GetSize(); }

	private:

		PSX_MakeStoragePoolFriend( RenderEffect );

		RenderEffect( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

		void AddRenderPass( RenderPass *pRenderPass );

		void ClearRenderPasses( void );

	private:

		typedef Array<RenderPass*> RenderPasses;

		RenderEffectDesc m_desc;
		RenderPasses m_renderPasses;
	};
}

#endif /* _PSX_RENDER_TECHINIQUE_H_ */