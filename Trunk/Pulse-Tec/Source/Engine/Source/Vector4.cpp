/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Vector4.cpp
*
*	Comments	-	See Vector4.h
*
**************************************************************************************/

#include "../Include/Vector4.h"

namespace Pulse
{
	// Initialize static members
	const Vector4 Vector4::UNIT_X(1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::UNIT_Y(0.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::UNIT_Z(0.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::UNIT_W(0.0f, 0.0f, 0.0f, 1.0f);

	const Vector4 Vector4::NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f, 0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_W(0.0f, 0.0f, 0.0f, -1.0f);

	const Vector4 Vector4::ZERO_VECTOR(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::ONE_VECTOR(1.0f, 1.0f, 1.0f, 1.0f);

	const Vector4 Vector4::INFINITY_VECTOR(1e9, 1e9, 1e9, 1e9);
	const Vector4 Vector4::NEGATIVE_INFINITY_VECTOR(-1e9, -1e9, -1e9, -1e9);
}