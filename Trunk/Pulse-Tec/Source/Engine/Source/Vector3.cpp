/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Vector3.cpp
*
*	Comments	-	See Vector3.h
*
**************************************************************************************/

#include "../Include/Vector3.h"

namespace Pulse
{
	// Static members initialization
	const Vector3 Vector3::UNIT_X(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::UNIT_Y(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::UNIT_Z(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f);
	const Vector3 Vector3::NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f);

	const Vector3 Vector3::ZERO_VECTOR(0.0f, 0.0f, 0.0f);
	const Vector3 Vector3::ONE_VECTOR(1.0f, 1.0f, 1.0f);

	const Vector3 Vector3::INFINITY_VECTOR(1e9, 1e9, 1e9);
	const Vector3 Vector3::NEGATIVE_INFINITY_VECTOR(-1e9, -1e9, -1e9);
}