/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Matrix4x4.cpp
*
*	Comments	-	See Matrix4x4.h
*
**************************************************************************************/

#include "../Include/Matrix4x4.h"

namespace Pulse
{
	const Matrix4x4 Matrix4x4::IDENTITY(	1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 0.0f, 1.0f );

	const Matrix4x4 Matrix4x4::ZERO(	0.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 0.0f );
}