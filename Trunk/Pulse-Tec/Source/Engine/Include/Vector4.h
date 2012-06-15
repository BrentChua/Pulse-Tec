/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Vector4.h
*
*	Description -	4D vector class.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/23/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_VECTOR_4_H_
#define _PSX_VECTOR_4_H_

#include "PulseSTD.h"
#include "Math.h"
#include "Vector3.h"
#include "Vector2.h"

namespace Pulse
{

	class Vector4
	{
	public:

		Vector4( void ) { }
		explicit Vector4( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _w );
		explicit Vector4( const Vector3 &vec );
		explicit Vector4( const Vector3 &vec, FLOAT _w );
		explicit Vector4( const Vector2 &vec );
		explicit Vector4( const Vector2 &vec, FLOAT _z, FLOAT _w );
		//explicit Vector4( const Quaternion &quat ); // TODO: Implement quaternion
		//~Vector4() { }

		void Set( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _w );

		void Add( const Vector4 &vec );
		void Add( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _w );
		void Add( FLOAT num );

		void Subtract( const Vector4 &vec );

		void Multiply( const FLOAT val );

		void Divide( const FLOAT val );

		void Normalize( void );

		void Negate( void );

		FLOAT GetLength( void );

		FLOAT DotProduct( const Vector4 &vec );

		//void Multiply( const Matrix4x4 &mat ); // TODO: Implement matrix4x4

		Vector4 & operator += ( const Vector4 &rhs );
		Vector4 & operator -= ( const Vector4 &rhs );
		Vector4 & operator *= ( const FLOAT rhs );
		//Vector4 & operator *= ( const Matrix4x4 &rhs ); // TODO: Implement matrix4x4
		Vector4 & operator /= ( const FLOAT rhs );

		const Vector4 operator - ( void );

		BOOL operator == ( const Vector4 &rhs ) const;

		FLOAT & operator [] ( UINT i );
		const FLOAT operator [] ( UINT i ) const;

		friend const Vector4 operator + ( const Vector4 &lhs, const Vector4 &rhs );
		friend const Vector4 operator - ( const Vector4 &lhs, const Vector4 &rhs );
		friend const FLOAT operator * ( const Vector4 &lhs, const Vector4 &rhs );
		friend const Vector4 operator * ( const FLOAT lhs, const Vector4 &rhs );
		friend const Vector4 operator * ( const Vector4 &lhs, const FLOAT &rhs );
		friend const Vector4 operator / ( const Vector4 &lhs, const FLOAT rhs );

		friend Vector4 Math::Normalize( const Vector4 &vec );

	public:

		union
		{
			struct { FLOAT x, y, z, w; };
			FLOAT v[4];
		};

		// Base constants that makes up the Euclidean 4-Dimensional space.
		static const Vector4 UNIT_X;
		static const Vector4 UNIT_Y;
		static const Vector4 UNIT_Z;
		static const Vector4 UNIT_W;
		static const Vector4 NEGATIVE_UNIT_X;
		static const Vector4 NEGATIVE_UNIT_Y;
		static const Vector4 NEGATIVE_UNIT_Z;
		static const Vector4 NEGATIVE_UNIT_W;

		static const Vector4 ZERO_VECTOR;
		static const Vector4 ONE_VECTOR;

		static const Vector4 INFINITY_VECTOR;
		static const Vector4 NEGATIVE_INFINITY_VECTOR;
	};

	//Vector4( void ) { }
	PSX_INLINE Vector4::Vector4( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _w )
		: x(_x), y(_y), z(_z), w(_w)
	{
	}

	PSX_INLINE Vector4::Vector4( const Vector3 &vec )
		: x(vec.x), y(vec.y), z(vec.z), w(0.0f)
	{
	}

	PSX_INLINE Vector4::Vector4( const Vector3 &vec, FLOAT _w )
		: x(vec.x), y(vec.y), z(vec.z), w(_w)
	{
	}

	PSX_INLINE Vector4::Vector4( const Vector2 &vec )
		: x(vec.x), y(vec.y), z(0.0f), w(0.0f)
	{
	}
	PSX_INLINE Vector4::Vector4( const Vector2 &vec, FLOAT _z, FLOAT _w )
		: x(vec.x), y(vec.y), z(_z), w(_w)
	{
	}

	//explicit Vector4( const Quaternion &quat ); // TODO: Implement quaternion
	//~Vector4() { }

	PSX_INLINE void Vector4::Set( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _w )
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	PSX_INLINE void Vector4::Add( const Vector4 &vec )
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
	}

	PSX_INLINE void Vector4::Add( FLOAT _x, FLOAT _y, FLOAT _z, FLOAT _w )
	{
		x += _x;
		y += _y;
		z += _z;
		w += _w;
	}

	PSX_INLINE void Vector4::Add( FLOAT num )
	{
		x += num;
		y += num;
		z += num;
		w += num;
	}

	PSX_INLINE void Vector4::Subtract( const Vector4 &vec )
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
	}

	PSX_INLINE void Vector4::Multiply( const FLOAT val )
	{
		x *= val;
		y *= val;
		z *= val;
		w *= val;
	}

	PSX_INLINE void Vector4::Divide( const FLOAT val )
	{
		x /= val;
		y /= val;
		z /= val;
		w /= val;
	}

	PSX_INLINE void Vector4::Normalize( void )
	{
		__asm
		{
			mov		esi, this
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
		}

		// These conditions make it running slower (still faster than non-SSE
		// way), but the results are more precise.
		if(1.0f - x < 0.00025f)
			x = 1.0f;
		if(1.0f - y < 0.00025f)
			y = 1.0f;
		if(1.0f - z < 0.00025f)
			z = 1.0f;
		if(1.0f - w < 0.00025f)
			w = 1.0f;

		//FLOAT len = Math::Sqrtf( x*x + y*y + z*z + w*w );
		//FLOAT inv = 1.0f / len;
		//x *= inv;
		//y *= inv;
		//z *= inv;
		//w *= inv;
	}

	PSX_INLINE void Vector4::Negate( void )
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

	PSX_INLINE FLOAT Vector4::GetLength( void )
	{
		FLOAT len;
		FLOAT* pLen = &len;

		__asm
		{
			mov		ecx, pLen
				mov		esi, this
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
		}

		return len;

		//return Math::Sqrtf( x*x + y*y + z*z + w*w );
	}

	PSX_INLINE FLOAT Vector4::DotProduct( const Vector4 &vec )
	{
		return (x * vec.x) +
				(y * vec.y) + 
				(z * vec.z) +
				(w * vec.w);
	}

	//void Multiply( const Matrix4x4 &mat ); // TODO: Implement matrix4x4

	PSX_INLINE Vector4 & Vector4::operator += ( const Vector4 &rhs )
	{
		Add( rhs );
		return *this;
	}

	PSX_INLINE Vector4 & Vector4::operator -= ( const Vector4 &rhs )
	{
		Subtract( rhs );
		return *this;
	}

	PSX_INLINE Vector4 & Vector4::operator *= ( const FLOAT rhs )
	{
		Multiply( rhs );
		return *this;
	}

	//Vector4 & operator *= ( const Matrix4x4 &rhs ); // TODO: Implement matrix4x4

	PSX_INLINE Vector4 & Vector4::operator /= ( const FLOAT rhs )
	{
		Divide( rhs );
		return *this;
	}

	PSX_INLINE const Vector4 Vector4::operator - ( void )
	{
		return Vector4( -x, -y, -z, -w );
	}

	PSX_INLINE BOOL Vector4::operator == ( const Vector4 &rhs ) const
	{
		return (x == rhs.x) &&
				(y == rhs.y) &&
				(z == rhs.z) &&
				(w == rhs.w);
	}

	PSX_INLINE FLOAT & Vector4::operator [] ( UINT i )
	{
		if ( i < 4 )
			return v[i];
		else
			return v[0];
	}

	PSX_INLINE const FLOAT Vector4::operator [] ( UINT i ) const
	{
		if ( i < 4 )
			return v[i];
		else
			return v[0];
	}

	PSX_INLINE const Vector4 operator + ( const Vector4 &lhs, const Vector4 &rhs )
	{
		return Vector4( lhs.x + rhs.x,
						lhs.y + rhs.y,
						lhs.z + rhs.z,
						lhs.w + rhs.w );
	}

	PSX_INLINE const Vector4 operator - ( const Vector4 &lhs, const Vector4 &rhs )
	{
		return Vector4( lhs.x - rhs.x,
						lhs.y - rhs.y,
						lhs.z - rhs.z,
						lhs.w - rhs.w );
	}

	PSX_INLINE const FLOAT operator * ( const Vector4 &lhs, const Vector4 &rhs )
	{
		return (lhs.x * rhs.x) +
				(lhs.y * rhs.y) +
				(lhs.z * rhs.z) +
				(lhs.w * rhs.w);
	}

	PSX_INLINE const Vector4 operator * ( const FLOAT lhs, const Vector4 &rhs )
	{
		return Vector4( lhs * rhs.x, 
						lhs * rhs.y, 
						lhs * rhs.z, 
						lhs * rhs.w );
	}

	PSX_INLINE const Vector4 operator * ( const Vector4 &lhs, const FLOAT &rhs )
	{
		return Vector4( lhs.x * rhs, 
						lhs.y * rhs, 
						lhs.z * rhs, 
						lhs.w * rhs );
	}

	PSX_INLINE const Vector4 operator / ( const Vector4 &lhs, const FLOAT rhs )
	{
		return Vector4( lhs.x / rhs,
						lhs.y / rhs,
						lhs.z / rhs,
						lhs.w / rhs );
	}

	PSX_INLINE Vector4 Math::Normalize( const Vector4 &vec )
	{
		Vector4 ret(vec);
		ret.Normalize();
		return ret;
	}

}

#endif /* _PSX_VECTOR_4_H_ */