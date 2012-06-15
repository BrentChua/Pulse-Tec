/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Matrix3x3.cpp
*
*	Comments	-	See Matrix3x3.h
*
**************************************************************************************/

#include "../Include/Matrix3x3.h"

namespace Pulse
{
	const Matrix3x3 Matrix3x3::IDENTITY(	1.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f,
											0.0f, 0.0f, 1.0f );

	const Matrix3x3 Matrix3x3::ZERO(	0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.0f );
}