/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Vector2.cpp
*
*	Comments	-	See Vector2.h
*
**************************************************************************************/

#include "../Include/Vector2.h"

namespace Pulse
{
	const Vector2 Vector2::UNIT_X(1.0f, 0.0f);
	const Vector2 Vector2::UNIT_Y(0.0f, 1.0f);
	const Vector2 Vector2::NEGATIVE_UNIT_X(-1.0f, 0.0f);
	const Vector2 Vector2::NEGATIVE_UNIT_Y(0.0f, -1.0f);

	const Vector2 Vector2::ZERO_VECTOR(0.0f, 0.0f);
	const Vector2 Vector2::ONE_VECTOR(1.0f, 1.0f);

	const Vector2 Vector2::INFINITY_VECTOR(1e9, 1e9);
	const Vector2 Vector2::NEGATIVE_INFINITY_VECTOR(-1e9, -1e9);
}