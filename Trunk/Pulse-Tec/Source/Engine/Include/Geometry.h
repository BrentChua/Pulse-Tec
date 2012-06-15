/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Geometry.h
*
*	Description -	Geometry.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/05/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_GEOMETRY_H_
#define _PSX_GEOMETRY_H_

#include "PulseSTD.h"
#include "GraphicsTypes.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Array.h"
#include "Material.h"

namespace Pulse
{
	struct GeometrySubset
	{
		Material	*m_pMaterial;
		UINT		m_offset;
		UINT		vertexCount;
		
	};

	class Geometry
	{
	public:

		const SIZE_T GetNumSubsets( void ) { return m_geoSubsets.GetSize(); }

		GeometrySubset * GetSubset( UINT index ) { return m_geoSubsets[index]; }

		InputLayout * GetInputLayout( void ) { return m_pInputLayout; }

		VertexBuffer * GetVertexBuffer( void ) { return m_pVertexBuffer; }

		IndexBuffer * GetIndexBuffer( void ) { return m_pIndexBuffer; }


	private:

		typedef Array<GeometrySubset*> GeometrySubsets;

		InputLayout			*m_pInputLayout;
		ETopology::Type		m_topologyType;
		VertexBuffer		*m_pVertexBuffer;
		IndexBuffer			*m_pIndexBuffer;
		GeometrySubsets		m_geoSubsets;

	};
}

#endif /* _PSX_GEOMETRY_H_ */