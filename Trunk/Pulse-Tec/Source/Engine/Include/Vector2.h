/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Vector2.h
*
*	Description -	2D vector class.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/22/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_VECTOR_2_H_
#define _PSX_VECTOR_2_H_

#include "PulseSTD.h"
#include "Math.h"
#include "String.h"

namespace Pulse
{
	class Vector2
	{
	public:

		Vector2( void ) { }
		explicit Vector2( FLOAT _x, FLOAT _y );
		//~Vector2( void );

		void Set( FLOAT _x, FLOAT _y );

		void Add( const Vector2 &vec );
		void Add( const FLOAT _x, const FLOAT _y );
		void Add( FLOAT _val );

		void Subtract( const Vector2 &vec );

		void Multiply( const FLOAT _val );
		void Divide( const FLOAT _val );

		void Normalize( void );
		void Negate( void );
		FLOAT GetLength( void ) const;
		FLOAT DotProduct( const Vector2 &vec ) const;

		// overloaded operators

		Vector2& operator += ( const Vector2& rhs );
		Vector2& operator -= ( const Vector2& rhs );
		Vector2& operator *= ( const FLOAT _val );

		friend Vector2 operator + ( const Vector2 &lhs, const Vector2 &rhs );
		friend Vector2 operator - ( const Vector2 &lhs, const Vector2 &rhs );
		friend const FLOAT operator * ( const Vector2 &lhs, const Vector2 &rhs );
		friend Vector2 operator * ( const FLOAT &lhs, const Vector2 &rhs );
		friend Vector2 operator * ( const Vector2 &lhs, const FLOAT &rhs );
		friend const BOOL operator == ( const Vector2 &lhs, const Vector2 &rhs );

		const Vector2 operator - ( void );

		FLOAT & operator [] ( UINT i );
		const FLOAT operator [] ( UINT i ) const;

		void * GetData( void ) { return &v; }

		// Friends
		friend Vector2 Math::Normalize( const Vector2 &vec );

	public:

		union {
			struct {
				FLOAT x;
				FLOAT y;
			};
			FLOAT v[2];
		};

		// Base constants that makes up the euclidean 2-Dimensional space.
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;

		static const Vector2 ZERO_VECTOR;
		static const Vector2 ONE_VECTOR;

		static const Vector2 INFINITY_VECTOR;
		static const Vector2 NEGATIVE_INFINITY_VECTOR;

	};

	PSX_INLINE Vector2::Vector2( FLOAT _x, FLOAT _y )
		: x( _x ), y( _y )
	{

	}

	PSX_INLINE void Vector2::Set( FLOAT _x, FLOAT _y )
	{
		x = _x;
		y = _y;
	}

	PSX_INLINE void Vector2::Add( const Vector2 &vec )
	{
		x += vec.x;
		y += vec.y;
	}

	PSX_INLINE void Vector2::Add( const FLOAT _x, const FLOAT _y )
	{
		x += _x;
		y += _y;
	}

	PSX_INLINE void Vector2::Add( FLOAT _val )
	{
		x += _val;
		y += _val;
	}

	PSX_INLINE void Vector2::Subtract( const Vector2 &vec )
	{
		x -= vec.x;
		y -= vec.y;
	}

	PSX_INLINE void Vector2::Multiply( const FLOAT _val )
	{
		x *= _val;
		y *= _val;
	}

	PSX_INLINE void Vector2::Divide( const FLOAT _val )
	{
		x /= _val;
		y /= _val;
	}

	PSX_INLINE void Vector2::Normalize( void )
	{
		FLOAT len = Math::Sqrtf( x * x + y * y );

		float inv = 1.0f / len;
		x *= len;
		y *= len;
	}

	PSX_INLINE void Vector2::Negate( void )
	{
		x = -x;
		y = -y;
	}

	PSX_INLINE FLOAT Vector2::GetLength( void ) const
	{
		return Math::Sqrtf( x*x + y*y );
	}

	PSX_INLINE FLOAT Vector2::DotProduct( const Vector2 &vec ) const
	{
		return x * vec.x + y * vec.y;
	}
	
	PSX_INLINE Vector2& Vector2::operator += ( const Vector2& rhs )
	{
		Add( rhs );
		return *this;
	}

	PSX_INLINE Vector2& Vector2::operator -= ( const Vector2& rhs )
	{
		Subtract( rhs );
		return *this;
	}

	PSX_INLINE Vector2& Vector2::operator *= ( const FLOAT _val )
	{
		Multiply( _val );
		return *this;
	}

	PSX_INLINE Vector2 operator + ( const Vector2 &lhs, const Vector2 &rhs )
	{
		return Vector2( lhs.x + rhs.x, lhs.y + rhs.y );
	}

	PSX_INLINE Vector2 operator - ( const Vector2 &lhs, const Vector2 &rhs )
	{
		return Vector2( lhs.x - rhs.x, lhs.y - rhs.y );
	}

	PSX_INLINE const FLOAT operator * ( const Vector2 &lhs, const Vector2 &rhs )
	{
		return lhs.DotProduct( rhs );
	}

	PSX_INLINE Vector2 operator * ( const FLOAT &lhs, const Vector2 &rhs )
	{
		return Vector2( lhs * rhs.x, lhs * rhs.y );
	}

	PSX_INLINE Vector2 operator * ( const Vector2 &lhs, const FLOAT &rhs )
	{
		return Vector2( lhs.x * rhs, lhs.y * rhs );
	}

	PSX_INLINE const BOOL operator == ( const Vector2 &lhs, const Vector2 &rhs )
	{
		return ( lhs.x == rhs.x && lhs.y == rhs.y );
	}

	PSX_INLINE const Vector2 Vector2::operator - ( void )
	{
		return Vector2( -x, -y );
	}

	PSX_INLINE FLOAT & Vector2::operator [] ( UINT i )
	{
		if ( i < 2 )
			return v[i];
		else
		{
			PSX_PushError( "Index should be less than 2. Index value is: %d. Returning v[i].", i );
			return v[0];
		}
	}

	PSX_INLINE const FLOAT Vector2::operator [] ( UINT i ) const
	{
		if ( i < 2 )
			return v[i];
		else
		{
			PSX_PushError( "Index should be less than 2. Index value is: %d. Returning v[i].", i );
			return v[0];
		}
	}
	
	PSX_INLINE Vector2 Math::Normalize( const Vector2 &vec )
	{
		Vector2 retVec( vec );
		retVec.Normalize();
		return retVec;
	}

}

#endif /* _PSX_VECTOR_2_H_ */