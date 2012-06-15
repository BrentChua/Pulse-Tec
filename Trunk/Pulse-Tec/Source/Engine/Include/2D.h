/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	2D.h
*
*	Description -	Math classes for 2D stuff.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/20/2011	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_POINT2_H_
#define _PSX_POINT2_H_

#include "PulseSTD.h"

namespace Pulse
{
	struct Point2
	{
	public:

		Point2( FLOAT _x = 0, FLOAT _y = 0 ) : x(_x), y(_y) { }

		void * GetData( void ) { return &x; }

	public:

		union
		{
			struct
			{
				FLOAT x, y;
			};

			struct
			{
				FLOAT w, h;
			};
		};
	};

	typedef Point2 Size2;

	struct Rect
	{
		Rect() { }

		Rect( LONG _left, LONG _top, LONG _right, LONG _bottom )
			: left(_left), top(_top), right(_right), bottom(_bottom)
		{
		}

		LONG left;
		LONG top;
		LONG right;
		LONG bottom;
	};
}

#endif /* _PSX_POINT2_H_ */