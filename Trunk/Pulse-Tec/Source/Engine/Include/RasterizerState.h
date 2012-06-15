/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	RasterizerState.h
*
*	Description -	Rasterizer state
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/18/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RASTERIZER_STATE_H_
#define _PSX_RASTERIZER_STATE_H_

#include "PulseSTD.h"
#include "GraphicsInfrastructure.h"
#include "GraphicsTypes.h"
#include "ResourcePoolItem.h"
#include "StoragePool.h"

namespace Pulse
{

	struct RasterizerStateDesc : public BaseResDesc
	{
		RasterizerStateDesc( void );

		void SetDefault( void );

		virtual EResDescType::Type GetDescType( void ) { return EResDescType::RASTERIZER_STATE; }

	public:
		
		EFillMode::Type fillMode;
		ECullMode::Type cullMode;
		BOOL            frontCounterClockwise;	//Determines if a triangle is front- or back-facing. If this parameter is TRUE, a triangle will be considered front-facing if its vertices are counter-clockwise on the render target and considered back-facing if they are clockwise. If this parameter is FALSE, the opposite is true.
		INT             depthBias;				// Depth value added to a given pixel. For info about depth bias, see Depth Bias.
		FLOAT           depthBiasClamp;			// Maximum depth bias of a pixel. For info about depth bias, see Depth Bias.
		FLOAT           slopeScaledDepthBias;	// Scalar on a given pixel's slope. For info about depth bias, see Depth Bias.
		BOOL            depthClipEnable;		// Enable clipping based on distance. The hardware always performs x and y clipping of rasterized coordinates. When DepthClipEnable is set to the default–TRUE, the hardware also clips the z value (that is, the hardware performs the last step of the following algorithm).
		BOOL            scissorEnable;			// Enable scissor-rectangle culling. All pixels outside an active scissor rectangle are culled.
		BOOL            multisampleEnable;		// Specifies whether to use the quadrilateral or alpha line anti-aliasing algorithm on multisample antialiasing (MSAA) render targets. Set to TRUE to use the quadrilateral line anti-aliasing algorithm and to FALSE to use the alpha line anti-aliasing algorithm. For more info about this member, see Remarks.
		BOOL            antialiasedLineEnable;	// Specifies whether to enable line antialiasing; only applies if doing line drawing and MultisampleEnable is FALSE. For more info about this member, see Remarks.
	};

	/* Remarks
	Note  For feature levels 9.1, 9.2, 9.3, and 10.0, if you set MultisampleEnable to FALSE, the runtime renders all points, lines, and triangles without anti-aliasing even for render targets with a sample count greater than 1. For feature levels 10.1 and higher, the setting of MultisampleEnable has no effect on 
	points and triangles with regard to MSAA and impacts only the selection of the line-rendering algorithm as shown in this table:

	Line-rendering algorithm	MultisampleEnable	AntialiasedLineEnable
	Aliased	FALSE	FALSE
	Alpha antialiased	FALSE	TRUE
	Quadrilateral	TRUE	FALSE
	Quadrilateral	TRUE	TRUE
 
	The settings of the MultisampleEnable and AntialiasedLineEnable members apply only 
	to multisample antialiasing (MSAA) render targets (that is, render targets with sample counts greater than 1). 
	Because of the differences in feature-level behavior and as long as you aren’t performing any line drawing or don’t mind that lines render as quadrilaterals, we recommend that you always set MultisampleEnable to 
	TRUE whenever you render on MSAA render targets.
	*/

	class RasterizerState : public IResourcePoolItem
	{
	public:

		virtual ~RasterizerState( void );

		// These defines the base interface for all derived resources
		virtual BOOL CreateResource( BaseResDesc *pDesc );
		virtual BOOL DestroyResource( void );
		virtual BOOL DisableResource( void );
		virtual BOOL RestoreResource( void );	

	private:

		PSX_MakeStoragePoolFriend( RasterizerState );

		RasterizerState( void );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

	private:

		RasterizerStateDesc m_desc;
		ID3D11RasterizerState *m_pRasterizerState;
	};
}

#endif /* _PSX_RASTERIZER_STATE_H_ */