/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Vector3.h
*
*	Description -	3D vector class.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/22/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_VECTOR_3_H_
#define _PSX_VECTOR_3_H_

#include "PulseSTD.h"
#include "Math.h"
#include "Vector2.h"

namespace Pulse
{
	class Vector3
	{
	public:

		Vector3( void ) { }
		explicit Vector3( FLOAT _x, FLOAT _y, FLOAT _z );
		explicit Vector3( const Vector2 &vec );
		explicit Vector3( const Vector2 &vec, FLOAT _z );
		//~Vector3( void ) { }

		void Set( FLOAT _x, FLOAT _y, FLOAT _z );

		void Add( FLOAT _x, FLOAT _y, FLOAT _z );
		void Add( const Vector3 &vec );
		void Add ( FLOAT _val );

		void Subtract( const Vector3 &vec );

		void Multiply( const Matrix3x3 &mat );
		void Multiply( const FLOAT _val );
		
		void Divide( const FLOAT _val );

		void Normalize( void );
		void Negate( void );

		FLOAT GetLength( void );

		FLOAT DotProduct( const Vector3 &vec ) const;
		Vector3 CrossProduct( const Vector3 &vec ) const;

		// TODO: Implement reflect method
		//Vector3 Reflect( const Plane &plane ) const;

		// Overloaded operators
		const Vector3 operator - ( void );

		Vector3 & operator += ( const Vector3 &rhs );
		Vector3 & operator -= ( const Vector3 &rhs );
		Vector3 & operator *= ( const Matrix3x3 &rhs );
		Vector3 & operator *= ( const FLOAT rhs );
		Vector3 & operator /= ( const FLOAT rhs );

		friend const Vector3 operator + ( const Vector3 &lhs, const Vector3 &rhs );
		friend const Vector3 operator - ( const Vector3 &lhs, const Vector3 &rhs );
		friend const Vector3 operator / ( const Vector3 &lhs, const FLOAT rhs );
		friend const FLOAT operator * ( const Vector3 &lhs, const Vector3 &rhs );
		friend const Vector3 operator * ( const FLOAT lhs, const Vector3 &rhs );
		friend const Vector3 operator * ( const Vector3 &lhs, const FLOAT rhs );
		friend const BOOL operator == ( const Vector3 &lhs, const Vector3 &rhs );

		FLOAT & operator [] ( UINT i );
		const FLOAT operator [] ( UINT i ) const;

		friend Vector3 Math::Normalize( const Vector3 &vec );
		friend const Vector3 operator * ( const Vector3 &lhs, const Matrix3x3 &rhs );

	public:

		union
		{
			struct { FLOAT x, y, z; };
			FLOAT v[3];
		};

		// Base constants that makes up the Euclidean 3-Dimensional space.
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;

		static const Vector3 ZERO_VECTOR;
		static const Vector3 ONE_VECTOR;

		static const Vector3 INFINITY_VECTOR;
		static const Vector3 NEGATIVE_INFINITY_VECTOR;

	};

	PSX_INLINE Vector3::Vector3( FLOAT _x, FLOAT _y, FLOAT _z )
		: x(_x), y(_y), z(_z)
	{

	}

	PSX_INLINE Vector3::Vector3( const Vector2 &vec )
		: x(vec.x), y(vec.y), z(0.0f)
	{

	}

	PSX_INLINE Vector3::Vector3( const Vector2 &vec, FLOAT _z )
		: x(vec.x), y(vec.y), z(_z)
	{

	}

	PSX_INLINE void Vector3::Set( FLOAT _x, FLOAT _y, FLOAT _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}

	PSX_INLINE void Vector3::Add( FLOAT _x, FLOAT _y, FLOAT _z )
	{
		x += _x;
		y += _y;
		z += _z;
	}

	PSX_INLINE void Vector3::Add( const Vector3 &vec )
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	PSX_INLINE void Vector3::Add ( FLOAT _val )
	{
		x += _val;
		y += _val;
		z += _val;
	}

	PSX_INLINE void Vector3::Subtract( const Vector3 &vec )
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	PSX_INLINE void Vector3::Multiply( const Matrix3x3 &mat )
	{
		*this = *this * mat;
	}

	PSX_INLINE void Vector3::Multiply( const FLOAT _val )
	{
		x *= _val;
		y *= _val;
		z *= _val;
	}

	PSX_INLINE void Vector3::Divide( const FLOAT _val )
	{
		x /= _val;
		y /= _val;
		z /= _val;
	}

	PSX_INLINE void Vector3::Normalize( void )
	{
		__asm
		{
			mov		esi, this
				push	eax
				mov		eax, 0
				xchg	[esi+12], eax
				movups	xmm0, [esi]
			movaps	xmm2, xmm0
				mulps	xmm0, xmm0
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 4Eh
				addps	xmm0, xmm1
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 11h
				addps	xmm0, xmm1

				rsqrtps	xmm0, xmm0
				mulps	xmm2, xmm0
				movups	[esi], xmm2

				xchg	[esi+12], eax
				pop		eax
		}

	if(1.0f - x < 0.00025f)
		x = 1.0f;
	if(1.0f - y < 0.00025f)
		y = 1.0f;
	if(1.0f - z < 0.00025f)
		z = 1.0f;

		//FLOAT len = Math::Sqrtf((x * x) + (y * y) + (z * z));
		//if(length != 0.0f)
		//{
		//	FLOAT inv = 1.0f / length;
		//	x *= inv;
		//	y *= inv;
		//	z *= inv;
		//}
	}

	PSX_INLINE void Vector3::Negate( void )
	{
		x = -x;
		y = -y;
		z = -z;
	}

	PSX_INLINE FLOAT Vector3::GetLength( void )
	{
		FLOAT len;
		FLOAT *pRet = &len;

		__asm
		{
			mov		ecx, pRet
				mov		esi, this
				push	eax
				mov		eax, 0
				xchg	[esi+12], eax
				movups	xmm0, [esi]
			mulps	xmm0, xmm0
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 4Eh
				addps	xmm0, xmm1
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 11h
				addps	xmm0, xmm1
				sqrtss	xmm0, xmm0
				movss	[ecx], xmm0

				xchg	[esi+12], eax
				pop		eax
		}

		return len;

		//return Math::Sqrtf(x*x+y*y+z*z);
	}

	PSX_INLINE FLOAT Vector3::DotProduct( const Vector3 &vec ) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	PSX_INLINE Vector3 Vector3::CrossProduct( const Vector3 &vec ) const
	{
		Vector3 ret;
		ret.x = y * vec.z - z * vec.y;
		ret.y = z * vec.x - x * vec.z;
		ret.z = x * vec.y - y * vec.x;
		return ret;
	}

	// TODO: Implement reflect method
	//Vector3 Reflect( const Plane &plane ) const;

	// Overloaded operators
	PSX_INLINE const Vector3 Vector3::operator - ( void )
	{
		return Vector3( -x, -y, -z );
	}

	PSX_INLINE Vector3 & Vector3::operator += ( const Vector3 &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	PSX_INLINE Vector3 & Vector3::operator -= ( const Vector3 &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	PSX_INLINE Vector3 & Vector3::operator *= ( const Matrix3x3 &rhs )
	{
		*this = *this * rhs;
		return *this;
	}

	PSX_INLINE Vector3 & Vector3::operator *= ( const FLOAT rhs )
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	PSX_INLINE Vector3 & Vector3::operator /= ( const FLOAT rhs )
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	PSX_INLINE const Vector3 operator + ( const Vector3 &lhs, const Vector3 &rhs )
	{
		return Vector3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
	}

	PSX_INLINE const Vector3 operator - ( const Vector3 &lhs, const Vector3 &rhs )
	{
		return Vector3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
	}

	PSX_INLINE const Vector3 operator / ( const Vector3 &lhs, const FLOAT rhs )
	{
		return Vector3( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
	}

	PSX_INLINE const FLOAT operator * ( const Vector3 &lhs, const Vector3 &rhs )
	{
		return lhs.DotProduct( rhs );
	}

	PSX_INLINE const Vector3 operator * ( const FLOAT lhs, const Vector3 &rhs )
	{
		return Vector3( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z );
	}

	PSX_INLINE const Vector3 operator * ( const Vector3 &lhs, const FLOAT rhs )
	{
		return Vector3( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
	}

	PSX_INLINE const BOOL operator == ( const Vector3 &lhs, const Vector3 &rhs )
	{
		return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
	}

	PSX_INLINE FLOAT & Vector3::operator [] ( UINT i )
	{
		if ( i < 3 )
			return v[i];
		else
		{
			PSX_PushError( "Index should be less than 3. Index value is: %d. Returning v[i].", i );
			return v[0];
		}
	}

	PSX_INLINE const FLOAT Vector3::operator [] ( UINT i ) const
	{
		if ( i < 3 )
			return v[i];
		else
		{
			PSX_PushError( "Index should be less than 3. Index value is: %d. Returning v[i].", i );
			return v[0];
		}
	}

	PSX_INLINE Vector3 Math::Normalize( const Vector3 &vec )
	{
		Vector3 ret(vec);
		ret.Normalize();
		return ret;
	}

}

#endif /* _PSX_VECTOR_3_H_ */