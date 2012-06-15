/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PipelineState.h
*
*	Description -	Base class to represent a stage in the rendering pipeline.
*
*	Comments	-	Base class.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/23/2012	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_PIPELINE_STATE_H_
#define _PSX_PIPELINE_STATE_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"

namespace Pulse
{
	class IPipelineStage
	{
	public:

		IPipelineStage( void );

		virtual ~IPipelineStage( void );

		void SetRenderer( Renderer *pRenderer );

		virtual void BindStates( void ) { }

		virtual void ClearStates( void ) { }

		PSX_FORCE_INLINE InputAssemblerStage	*	AsInputAssemblerStage( void )	{ return (InputAssemblerStage*)this;	}
		PSX_FORCE_INLINE VertexShaderStage		*	AsVertexShaderStage( void )		{ return (VertexShaderStage*)this;		}
		PSX_FORCE_INLINE HullShaderStage		*	AsHullShaderStage( void )		{ return (HullShaderStage*)this;		}
		PSX_FORCE_INLINE TesellatorStage		*	AsTesellatorStage( void )		{ return (TesellatorStage*)this;		}
		PSX_FORCE_INLINE DomainShaderStage		*	AsDomainShaderStage( void )		{ return (DomainShaderStage*)this;		}
		PSX_FORCE_INLINE GeometryShaderStage	*	AsGeometryShaderStage( void )	{ return (GeometryShaderStage*)this;	}
		PSX_FORCE_INLINE StreamOutputStage		*	AsStreamOutputStage( void )		{ return (StreamOutputStage*)this;		}
		PSX_FORCE_INLINE RasterizerStage		*	AsRasterizerStage( void )		{ return (RasterizerStage*)this;		}
		PSX_FORCE_INLINE PixelShaderStage		*	AsPixelShaderStage( void )		{ return (PixelShaderStage*)this;		}
		PSX_FORCE_INLINE OutputMergerStage		*	AsOutputMergerStage( void )		{ return (OutputMergerStage*)this;		}

		virtual EPipelineStage::Type GetType( void ) = 0;

	protected:

		Renderer *m_pRenderer;

	};
}

#endif /* _PSX_PIPELINE_STATE_H_ */