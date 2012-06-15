/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Matrix3x3.h
*
*	Description -	#x3 matrix class.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/23/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_MATRIX_3X3_H_
#define _PSX_MATRIX_3X3_H_

#include "PulseSTD.h"
#include "Math.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Pulse
{
	class Matrix3x3
	{
	public:

		Matrix3x3( void ) { }
		Matrix3x3( const FLOAT *pArr );
		explicit Matrix3x3( FLOAT _m11, FLOAT _m12, FLOAT _m13, 
							FLOAT _m21, FLOAT _m22, FLOAT _m23,
							FLOAT _m31, FLOAT _m32, FLOAT _m33 );
		~Matrix3x3( void ) { }

		void Set( const Matrix3x3 &mat );

		void Set( FLOAT _m11, FLOAT _m12, FLOAT _m13, 
					FLOAT _m21, FLOAT _m22, FLOAT _m23,
					FLOAT _m31, FLOAT _m32, FLOAT _m33 );

		void BuildIdentity( void );

		void BuildZero( void );

		void Negate( void );

		void Invert( void );

		const FLOAT GetDeterminant( void );

		FLOAT GetMinor( UINT r0, UINT r1, UINT c0, UINT c1 ) const;

		void Transpose( void );

		void Add( const Matrix3x3 &mat );

		void Subtract( const Matrix3x3 &mat );

		void Multiply( const Matrix3x3 &mat );

		void Multiply( const FLOAT val );

		void Divide( const FLOAT val );

		void BuildRotationX( const FLOAT angle );
		void BuildRotationY( const FLOAT angle );
		void BuildRotationZ( const FLOAT angle );
		void BuildRotation( const Vector3 &axis, const FLOAT angle );
		void BuildRotationYawPitchRoll( const Vector3 &euler );
		void BuildRotationYawPitchRoll( FLOAT yaw, FLOAT pitch, FLOAT roll );

		void BuildScale( const Vector3 &scale );
		void BuildScale( FLOAT x, FLOAT y, FLOAT z );
		
		BOOL IsIdentity( void );

		BOOL IsZero( void );

		// Overloaded operators
		Matrix3x3 & operator += ( const Matrix3x3 &rhs );
		Matrix3x3 & operator -= ( const Matrix3x3 &rhs );
		Matrix3x3 & operator *= ( const Matrix3x3 &rhs );

		const Matrix3x3 operator - ( void );

		BOOL operator == ( const Matrix3x3 &rhs );

		FLOAT * operator [] ( UINT i );

		friend const Matrix3x3 operator + ( const Matrix3x3 &lhs, const Matrix3x3 &rhs );
		friend const Matrix3x3 operator - ( const Matrix3x3 &lhs, const Matrix3x3 &rhs );
		friend const Matrix3x3 operator * ( const Matrix3x3 &lhs, const Matrix3x3 &rhs );
		friend const Vector3 operator * ( const Vector3 &lhs, const Matrix3x3 &rhs );
		friend const Matrix3x3 operator * ( const Matrix3x3& lhs, const FLOAT rhs );
		friend const Matrix3x3 operator / ( const Matrix3x3 &lhs, FLOAT rhs );

	private:

		Matrix3x3 GetAdjoint( void ) const;

	public:

		union
		{
			struct
			{
				FLOAT	_11, _12, _13;
				FLOAT	_21, _22, _23;
				FLOAT	_31, _32, _33;
				FLOAT	_41, _42, _43; 
			};
			FLOAT _m[3][3];
			FLOAT m[9];
		};

		static const Matrix3x3 IDENTITY;
		static const Matrix3x3 ZERO;

	};

	//Matrix3x3( void ) { }
	
	PSX_INLINE Matrix3x3::Matrix3x3( const FLOAT *pArr )
		: _11( pArr[0] ), _12( pArr[1] ), _13( pArr[2] ),
			_21( pArr[3] ), _22( pArr[4] ), _23( pArr[5] ),
			_31( pArr[6] ), _32( pArr[7] ), _33( pArr[8] )
	{

	}

	PSX_INLINE Matrix3x3::Matrix3x3( FLOAT _m11, FLOAT _m12, FLOAT _m13, 
										FLOAT _m21, FLOAT _m22, FLOAT _m23,
										FLOAT _m31, FLOAT _m32, FLOAT _m33 )
		: _11( _m11 ), _12( _m12 ), _13( _m13 ),
			_21( _m21 ), _22( _m22 ), _23( _m23 ),
			_31( _m31 ), _32( _m32 ), _33( _m33 )

	{

	}

	//~Matrix3x3( void ) { }

	PSX_INLINE void Matrix3x3::Set( const Matrix3x3 &mat )
	{
		*this = mat;
	}

	PSX_INLINE void Matrix3x3::Set( FLOAT _m11, FLOAT _m12, FLOAT _m13, 
									FLOAT _m21, FLOAT _m22, FLOAT _m23,
									FLOAT _m31, FLOAT _m32, FLOAT _m33 )
	{
		_11 = _m11; _12 = _m12; _13 = _m13;
		_21 = _m21; _22 = _m22; _23 = _m23;
		_31 = _m31; _32 = _m32; _33 = _m33;
	}

	PSX_INLINE void Matrix3x3::BuildIdentity( void )
	{
		PSX_ZeroMem( m, sizeof( m ) );
		_11 = _22 = _33 = 1.0f;
	}

	PSX_INLINE void Matrix3x3::BuildZero( void )
	{
		PSX_ZeroMem( m, sizeof( m ) );
	}

	PSX_INLINE void Matrix3x3::Negate( void )
	{
		_11 = -_11; _12 = -12; _13 = -_13;
		_21 = -_21; _22 = -22; _23 = -_23;
		_31 = -_31; _32 = -32; _33 = -_33;
	}

	PSX_INLINE void Matrix3x3::Invert( void )
	{
		FLOAT det = GetDeterminant();

		if ( !det )
			return;

		Matrix3x3 adj = GetAdjoint();
		adj.Transpose();

		*this = adj * ( 1.0f / det );
	}

	PSX_INLINE const FLOAT Matrix3x3::GetDeterminant( void )
	{
		return (_11 * (_22 * _33 - _23 * _32) +
				_12 * (_23 * _31 - _21 * _33) +
				_13 * (_21 * _32 - _22 * _31));
	}

	PSX_INLINE FLOAT Matrix3x3::GetMinor( UINT r0, UINT r1, UINT c0, UINT c1 ) const
	{
		// Sarrus method
		return (_m[r0][c0] * _m[r1][c1] - _m[r0][c1] * _m[r1][c0]);
	}

	PSX_INLINE void Matrix3x3::Transpose( void )
	{
		Matrix3x3 transMat(*this);

		transMat._11 = _11; 
		transMat._12 = _21; 
		transMat._13 = _31;

		transMat._21 = _12; 
		transMat._22 = _22; 
		transMat._23 = _32;

		transMat._31 = _13; 
		transMat._32 = _23; 
		transMat._33 = _33;

		*this = transMat;
	}

	PSX_INLINE void Matrix3x3::Add( const Matrix3x3 &mat )
	{
		_11 += mat._11;
		_12 += mat._12;
		_13 += mat._13;

		_21 += mat._21;
		_22 += mat._22;
		_23 += mat._23;

		_31 += mat._31;
		_32 += mat._32;
		_33 += mat._33;
	}

	PSX_INLINE void Matrix3x3::Subtract( const Matrix3x3 &mat )
	{
		_11 -= mat._11;
		_12 -= mat._12;
		_13 -= mat._13;

		_21 -= mat._21;
		_22 -= mat._22;
		_23 -= mat._23;

		_31 -= mat._31;
		_32 -= mat._32;
		_33 -= mat._33;
	}

	PSX_INLINE void Matrix3x3::Multiply( const Matrix3x3 &mat )
	{
		Matrix3x3 ret;

		ret._11 = _11 * mat._11 + _12 * mat._21 + _13 * mat._31;
		ret._12 = _11 * mat._12 + _12 * mat._22 + _13 * mat._32;
		ret._13 = _11 * mat._13 + _12 * mat._23 + _13 * mat._33;

		ret._21 = _21 * mat._11 + _22 * mat._21 + _23 * mat._31;
		ret._22 = _21 * mat._12 + _22 * mat._22 + _23 * mat._32;
		ret._23 = _21 * mat._13 + _22 * mat._23 + _23 * mat._33;

		ret._31 = _31 * mat._11 + _32 * mat._21 + _33 * mat._31;
		ret._32 = _31 * mat._12 + _32 * mat._22 + _33 * mat._32;
		ret._33 = _31 * mat._13 + _32 * mat._23 + _33 * mat._33;

		(*this) = mat;
	}

	PSX_INLINE void Matrix3x3::Multiply( const FLOAT val )
	{
		_11 *= val; _12 *= val; _13 *= val;
		_21 *= val; _22 *= val; _23 *= val;
		_31 *= val; _32 *= val; _33 *= val;
	}

	PSX_INLINE void Matrix3x3::Divide( const FLOAT val )
	{
		_11 /= val; _12 /= val; _13 /= val;
		_21 /= val; _22 /= val; _23 /= val;
		_31 /= val; _32 /= val; _33 /= val;
	}

	PSX_INLINE void Matrix3x3::BuildRotationX( const FLOAT angle )
	{
		BuildIdentity();

		FLOAT cs = Math::Cosf( angle );
		FLOAT sn = Math::Sinf( angle );

		_22 = cs;
		_23 = sn;
		_32 = -sn;
		_33 = cs;
	}

	PSX_INLINE void Matrix3x3::BuildRotationY( const FLOAT angle )
	{
		BuildIdentity();

		FLOAT cs = Math::Cosf( angle );
		FLOAT sn = Math::Sinf( angle );

		_11 = cs;
		_13 = -sn;
		_31 = sn;
		_31 = cs;
	}

	PSX_INLINE void Matrix3x3::BuildRotationZ( const FLOAT angle )
	{
		BuildIdentity();

		FLOAT cs = Math::Cosf( angle );
		FLOAT sn = Math::Sinf( angle );

		_11 = cs;
		_12 = sn;
		_21 = -sn;
		_22 = cs;
	}

	PSX_INLINE void Matrix3x3::BuildRotation( const Vector3 &axis, const FLOAT angle )
	{
		FLOAT cs = Math::Cosf( angle );
		FLOAT sn = Math::Sinf( angle );
		FLOAT sum = 1.0f - cs;

		FLOAT xy = axis.x * axis.y;
		FLOAT xz = axis.x * axis.z;
		FLOAT yz = axis.y * axis.z;

		_11 = axis.x * axis.x * sum + cs;
		_12 = xy * sum - axis.z * sn;
		_13 = xz * sum + axis.y * sn;

		_21 = xy * sum + axis.z * sn;
		_22 = axis.y * axis.y * sum + cs;
		_23 = yz * sum - axis.x * sn;

		_31 = xz * sum - axis.y * sn;
		_32 = yz * sum + axis.x * sn;
		_33 = axis.z * axis.z * sum + cs;
	}

	PSX_INLINE void Matrix3x3::BuildRotationYawPitchRoll( const Vector3 &euler )
	{
		FLOAT sy = Math::Sinf(euler.z), cy = Math::Cosf(euler.z);
		FLOAT sp = Math::Sinf(euler.x), cp = Math::Cosf(euler.x);
		FLOAT sr = Math::Sinf(euler.y), cr = Math::Cosf(euler.y);

		_11 = cy*cr + sy*sp*sr;  _12 = sr*cp; _13 = -sy*cr + cy*sp*sr;
		_21 = -cy*sr + sy*sp*cr; _22 = cr*cp; _23 = sr*sy + cy*sp*cr;
		_31 = sy*cp;             _32 = -sp;   _33 = cy*cp;
	}

	PSX_INLINE void Matrix3x3::BuildRotationYawPitchRoll( FLOAT yaw, FLOAT pitch, FLOAT roll )
	{
		FLOAT sy = Math::Sinf(yaw),		cy = Math::Cosf(yaw);
		FLOAT sp = Math::Sinf(pitch),	cp = Math::Cosf(pitch);
		FLOAT sr = Math::Sinf(roll),	cr = Math::Cosf(roll);

		_11 = cy*cr + sy*sp*sr;  _12 = sr*cp; _13 = -sy*cr + cy*sp*sr;
		_21 = -cy*sr + sy*sp*cr; _22 = cr*cp; _23 = sr*sy + cy*sp*cr;
		_31 = sy*cp;             _32 = -sp;   _33 = cy*cp;
	}

	PSX_INLINE void Matrix3x3::BuildScale( const Vector3 &scale )
	{
		BuildIdentity();
		_11 = scale.x;
		_22 = scale.y;
		_33 = scale.z;
	}

	PSX_INLINE void Matrix3x3::BuildScale( FLOAT x, FLOAT y, FLOAT z )
	{
		BuildIdentity();
		_11 = x;
		_22 = y;
		_33 = z;
	}

	PSX_INLINE BOOL Matrix3x3::IsIdentity( void )
	{
		return *this == IDENTITY;
	}

	PSX_INLINE BOOL Matrix3x3::IsZero( void )
	{
		return *this == ZERO;
	}

	// Overloaded operators
	PSX_INLINE Matrix3x3 & Matrix3x3::operator += ( const Matrix3x3 &rhs )
	{
		Add( rhs );
		return *this;
	}

	PSX_INLINE Matrix3x3 & Matrix3x3::operator -= ( const Matrix3x3 &rhs )
	{
		Subtract( rhs );
		return *this;
	}

	PSX_INLINE Matrix3x3 & Matrix3x3::operator *= ( const Matrix3x3 &rhs )
	{
		Multiply( rhs );
		return *this;
	}

	PSX_INLINE const Matrix3x3 Matrix3x3::operator - ( void )
	{
		Matrix3x3 ret(*this);
		ret.Negate();
		return ret;
	}

	PSX_INLINE BOOL Matrix3x3::operator == ( const Matrix3x3 &rhs )
	{
		return  _11 == rhs._11 && _12 == rhs._12 && _13 == rhs._13 &&
				_21 == rhs._21 && _22 == rhs._22 && _23 == rhs._23 &&
				_31 == rhs._31 && _32 == rhs._32 && _33 == rhs._33;
	}

	PSX_INLINE FLOAT * Matrix3x3::operator [] ( UINT i )
	{
		if ( i > 3 )
			return (FLOAT*)_m[i];
		else
			return (FLOAT*)_m[0];
	}

	PSX_INLINE const Matrix3x3 operator + ( const Matrix3x3 &lhs, const Matrix3x3 &rhs )
	{
		Matrix3x3 ret(lhs);
		ret.Add(rhs);
		return ret;
	}

	PSX_INLINE const Matrix3x3 operator - ( const Matrix3x3 &lhs, const Matrix3x3 &rhs )
	{
		Matrix3x3 ret(lhs);
		ret.Subtract(rhs);
		return ret;
	}

	PSX_INLINE const Matrix3x3 operator * ( const Matrix3x3 &lhs, const Matrix3x3 &rhs )
	{
		Matrix3x3 ret(lhs);
		ret.Multiply(rhs);
		return ret;
	}

	PSX_INLINE const Vector3 operator * ( const Vector3 &lhs, const Matrix3x3 &rhs )
	{
		Vector3 ret;

		ret.x = lhs.x * rhs._11 + lhs.y * rhs._21 + lhs.z * rhs._31;
		ret.y = lhs.x * rhs._12 + lhs.y * rhs._22 + lhs.z * rhs._32;
		ret.z = lhs.x * rhs._13 + lhs.y * rhs._23 + lhs.z * rhs._33;

		return ret;
	}

	PSX_INLINE const Matrix3x3 operator * ( const Matrix3x3& lhs, const FLOAT rhs )
	{
		Matrix3x3 ret( lhs );
		ret.Multiply( rhs );
		return ret;
	}

	PSX_INLINE const Matrix3x3 operator / ( const Matrix3x3 &lhs, FLOAT rhs )
	{
		Matrix3x3 ret(lhs);
		ret.Divide( rhs );
		return ret;
	}

	PSX_INLINE Matrix3x3 Matrix3x3::GetAdjoint( void ) const
	{
		return Matrix3x3(	GetMinor(1, 2, 1, 2),
							-GetMinor(1, 2, 0, 2),
							GetMinor(1, 2, 0, 1),

							-GetMinor(0, 2, 1, 2),
							GetMinor(0, 2, 0, 2),
							-GetMinor(0, 2, 0, 1),

							GetMinor(0, 1, 1, 2),
							-GetMinor(0, 1, 0, 2),
							GetMinor(0, 1, 0, 1 ));
	}

}

#endif /* _PSX_MATRIX_3X3_H_ */