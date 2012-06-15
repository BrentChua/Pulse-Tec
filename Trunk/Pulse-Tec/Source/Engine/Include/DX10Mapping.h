/**************************************************************************************
*
*	Copyright (C) 2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	DX10Mapping.h
*
*	Description -	Singleton class that maps Pulse types to D3D10 types.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			CodeSushi	-	03/23/2010	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_MAP_TO_DX10_H_
#define _PSX_MAP_TO_DX10_H_

#include "PulseSTD.h"
#include "GraphicsDataTypes.h"

#include <d3d10.h>
#include <d3dx10.h>

namespace Pulse
{
	class DX10Mapping
	{
	public:

		static DXGI_FORMAT ToTextureFormat( TEXTURE_FORMAT format );
		static TEXTURE_FORMAT FromTextureFormat( DXGI_FORMAT format );

		static D3D10_USAGE ToUsage( TEXTURE_USAGE usage );

		static UINT ToBindFlags( TEXTURE_USAGE usage );

		static UINT ToCPUAccessFlags( TEXTURE_USAGE usage );

		static D3DX10_FILTER_FLAG ToFilter( TEXTURE_FILTER type );

	};
}

#endif /* _PSX_MAP_TO_DX10_H_ */