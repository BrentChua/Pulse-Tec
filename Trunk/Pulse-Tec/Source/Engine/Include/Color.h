/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Color.h
*
*	Description -	Color class encapsulation
*
*	Comments	- 
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/27/2010	-			File creation.
*			MrCodeSushi	-	03/15/2010	-			Re-wrote the Color class.
**************************************************************************************/

#ifndef _PSX_COLOR_H_
#define _PSX_COLOR_H_

#include "PulseSTD.h"
#include "PTLBase.h"

namespace Pulse
{
	PSX_EnumBegin( EColorType )
		BLUE,
		GREEN,
		RED,
		ALPHA,
	PSX_EnumEnd()

	class Color
	{
	public:

		Color( BYTE r = 255, BYTE g = 255, BYTE b = 255, BYTE a = 255 );

		Color( const DWORD argb );

		EErrorCode::Type Initialize( void );

		void Cleanup( void );

		void Inverse( void );

		void Saturate( FLOAT val );

		BYTE GetAlpha( void );
		BYTE GetRed( void );
		BYTE GetGreen( void );
		BYTE GetBlue( void );

		FLOAT GetAlphaF( void );
		FLOAT GetRedF( void );
		FLOAT GetGreenF( void );
		FLOAT GetBlueF( void );

		void SetAlpha( BYTE alpha );
		void SetRed( BYTE red );
		void SetGreen( BYTE green );
		void SetBlue( BYTE blue );

		void SetAlpha( FLOAT alpha );
		void SetRed( FLOAT red );
		void SetGreen( FLOAT green );
		void SetBlue( FLOAT blue );
		
		void SetARGB( const DWORD _color );
		void SetARGB( BYTE alpha, BYTE red, BYTE green, BYTE blue );

		void SetRGBA( const DWORD _color );
		void SetRGBA( BYTE red, BYTE green, BYTE blue, BYTE alpha );

		DWORD GetARGB( void );

		BOOL operator == ( const Color &rhs );

		BOOL operator != ( const Color &rhs );

	public:

		union
		{
			// ARGB format
			BYTE color[EColorType::EnumCount];
			DWORD dwColor;

			struct
			{
				BYTE blue;
				BYTE green;
				BYTE red;
				BYTE alpha;
			};
		};

		static const Color BLACK;
		static const Color WHITE;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color GRAY;
		//static const Color SILVER;


	};

	PSX_INLINE Color::Color( BYTE r, BYTE g, BYTE b, BYTE a )
		: dwColor( ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | ((b & 0xff)) )
	{

	}

	PSX_INLINE Color::Color( const DWORD argb )
	{
		SetARGB( argb );
	}

	PSX_INLINE EErrorCode::Type Color::Initialize( void )
	{
		return EErrorCode::NONE;
	}

	PSX_INLINE void Color::Cleanup( void )
	{
	}

	PSX_INLINE void Color::Inverse( void )
	{
		color[EColorType::ALPHA]	= 255 - color[EColorType::ALPHA];
		color[EColorType::RED]		= 255 - color[EColorType::RED];
		color[EColorType::GREEN]	= 255 - color[EColorType::GREEN];
		color[EColorType::BLUE]		= 255 - color[EColorType::BLUE];
	}

	PSX_INLINE void Color::Saturate( FLOAT val )
	{
		// TODO: Get back on this later...
	}

	PSX_INLINE BYTE Color::GetAlpha( void )
	{
		return color[EColorType::ALPHA];
	}

	PSX_INLINE BYTE Color::GetRed( void )
	{
		return color[EColorType::RED];
	}
	
	PSX_INLINE BYTE Color::GetGreen( void )
	{
		return color[EColorType::GREEN];
	}
	
	PSX_INLINE BYTE Color::GetBlue( void )
	{
		return color[EColorType::BLUE];
	}

	PSX_INLINE FLOAT Color::GetAlphaF( void )
	{
		return (FLOAT)color[EColorType::ALPHA] / 255.0f;
	}

	PSX_INLINE FLOAT Color::GetRedF( void )
	{
		return (FLOAT)color[EColorType::RED] / 255.0f;
	}

	PSX_INLINE FLOAT Color::GetGreenF( void )
	{
		return (FLOAT)color[EColorType::GREEN] / 255.0f;
	}

	PSX_INLINE FLOAT Color::GetBlueF( void )
	{
		return (FLOAT)color[EColorType::BLUE] / 255.0f;
	}

	PSX_INLINE void Color::SetAlpha( BYTE alpha )
	{
		color[EColorType::ALPHA] = alpha;
	}

	PSX_INLINE void Color::SetRed( BYTE red )
	{
		color[EColorType::RED] = red;
	}

	PSX_INLINE void Color::SetGreen( BYTE green )
	{
		color[EColorType::GREEN] = green;
	}

	PSX_INLINE void Color::SetBlue( BYTE blue )
	{
		color[EColorType::BLUE] = blue;
	}

	PSX_INLINE void Color::SetAlpha( FLOAT alpha )
	{
		color[EColorType::ALPHA] = (BYTE)(alpha * 255);
	}

	PSX_INLINE void Color::SetRed( FLOAT red )
	{
		color[EColorType::RED] = (BYTE)(red * 255);
	}

	PSX_INLINE void Color::SetGreen( FLOAT green )
	{
		color[EColorType::GREEN] = (BYTE)(green * 255);
	}

	PSX_INLINE void Color::SetBlue( FLOAT blue )
	{
		color[EColorType::BLUE] = (BYTE)(blue * 255);
	}

	PSX_INLINE void Color::SetARGB( const DWORD _color )
	{
		dwColor = _color;
	}

	PSX_INLINE void Color::SetARGB( BYTE alpha, BYTE red, BYTE green, BYTE blue )
	{
		SetAlpha( alpha );
		SetRed( red );
		SetGreen( green );
		SetBlue( blue );
	}

	PSX_INLINE void Color::SetRGBA( const DWORD _color )
	{
		dwColor = _color;
		PSX_Swap( color[EColorType::ALPHA], color[EColorType::BLUE] );
	}
	
	PSX_INLINE void Color::SetRGBA( BYTE red, BYTE green, BYTE blue, BYTE alpha )
	{
		SetRed( red );
		SetGreen( green );
		SetBlue( blue );
		SetAlpha( alpha );
	}

	PSX_INLINE DWORD Color::GetARGB( void )
	{
		return dwColor;
	}

	PSX_INLINE BOOL Color::operator == ( const Color &rhs )
	{
		return dwColor == rhs.dwColor;
	}

	PSX_INLINE BOOL Color::operator != ( const Color &rhs )
	{
		return !(*this == rhs);
	}

}

#endif /* _PSX_COLOR_H_ */