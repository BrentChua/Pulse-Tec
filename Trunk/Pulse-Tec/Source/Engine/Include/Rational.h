/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Rational.h
*
*	Description -	Represents a rational number. 
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/07/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_RATIONAL_H_
#define _PSX_RATIONAL_H_

#include "PulseSTD.h"

namespace Pulse
{
	struct Rational
	{
	public:
		
		Rational( FLOAT _numerator = 0.0f, FLOAT _denominator = 0.0f )
			: numerator(_numerator), denominator(_denominator)
		{

		}

		FLOAT numerator;
		FLOAT denominator;
	};
}

#endif /* _PSX_RATIONAL_H_ */