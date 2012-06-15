/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Matrix4x4.h
*
*	Description -	4x4 matrix class.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	02/24/2010		-	File creation
**************************************************************************************/

#ifndef _MATRIX_4X4_H_
#define _MATRIX_4X4_H_

#include "PulseSTD.h"
#include "Math.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Pulse
{

	class Matrix4x4
	{
	public:

		Matrix4x4( void ) { }
		explicit Matrix4x4( FLOAT _m11, FLOAT _m12, FLOAT _m13, FLOAT _m14,
							FLOAT _m21, FLOAT _m22, FLOAT _m23, FLOAT _m24,
							FLOAT _m31, FLOAT _m32, FLOAT _m33, FLOAT _m34,
							FLOAT _m41, FLOAT _m42, FLOAT _m43, FLOAT _m44 );
		~Matrix4x4( void ) { }

		void Set( FLOAT _m11, FLOAT _m12, FLOAT _m13, FLOAT _m14,
				  FLOAT _m21, FLOAT _m22, FLOAT _m23, FLOAT _m24,
				  FLOAT _m31, FLOAT _m32, FLOAT _m33, FLOAT _m34,
				  FLOAT _m41, FLOAT _m42, FLOAT _m43, FLOAT _m44 );

		void BuildIdentity( void );
		void BuildZero( void );

		void Negate( void );

		void Invert( void );

		FLOAT GetDeterminant( void ) const;

		FLOAT GetMinor( UINT r0, UINT r1, UINT r2, UINT c0, UINT c1, UINT c2 ) const;

		void Transpose( void );

		void Add( const Matrix4x4 &rhs );
		void Subtract( const Matrix4x4 &rhs );
		void Multiply( const Matrix4x4 &rhs );
		friend Vector4 Multiply( const Vector4 &lhs, const Matrix4x4 &rhs );
		friend Vector3 Multiply( const Vector3 &lhs, const Matrix4x4 &rhs );
		void Multiply( FLOAT rhs );
		void Divide( FLOAT rhs );

		void BuildTranslation( const Vector3 &trans );
		void BuildTranslation( const Vector3 *pTrans ) { BuildTranslation( *pTrans ); }
		void BuildTranslation( FLOAT x, FLOAT y, FLOAT z );

		void SetTranslation( const Vector3 &trans );
		void SetTranslation( FLOAT x, FLOAT y, FLOAT z );

		Vector3 & GetTranslation( void );
		const Vector3 GetTranslation( void ) const;

		void BuildRotationX( FLOAT angle );
		void BuildRotationY( FLOAT angle );
		void BuildRotationZ( FLOAT angle );
		void BuildRotation( const Vector3 &axis, FLOAT angle );
		
		void BuildRotationYawPitchRoll( const Vector3 &euler );
		void BuildRotationPitchYawRoll( const Vector3 &rot );
		void BuildRotationYawPitchRoll( FLOAT yaw, FLOAT pitch, FLOAT roll );

		void BuildScale( const Vector3 &scale );
		void BuildScale( FLOAT x, FLOAT y, FLOAT z );
		void BuildScale( FLOAT s );

		void BuildLookAt( const Vector3 &eyePos, const Vector3 &target, const Vector3 &up );

		void BuildOrthogonalProjection( FLOAT _width, FLOAT _height, FLOAT _near, FLOAT _far );
		void BuildPerspectiveProjection( FLOAT _fovy, FLOAT _aspect, FLOAT _near, FLOAT _far );

		BOOL IsIdentity( void ) const;
		BOOL IsZero( void ) const;

		void * GetData( void ) { return m; }

		// Overloaded operators
		Matrix4x4 & operator += ( const Matrix4x4 &rhs );
		Matrix4x4 & operator -= ( const Matrix4x4 &rhs );
		Matrix4x4 & operator *= ( const Matrix4x4 &rhs );
		Matrix4x4 & operator *= ( FLOAT rhs );

		Matrix4x4 operator - ( void );

		BOOL operator == ( const Matrix4x4 &rhs ) const;

		FLOAT * operator [] ( UINT i );

		friend const Matrix4x4 operator + ( const Matrix4x4 &lhs, const Matrix4x4 &rhs );
		friend const Matrix4x4 operator - ( const Matrix4x4 &lhs, const Matrix4x4 &rhs );
		friend const Matrix4x4 operator * ( const Matrix4x4 &lhs, const Matrix4x4 &rhs );
		friend const Vector4 operator * ( const Vector4 &lhs, const Matrix4x4 &rhs );
		friend const Vector3 operator * ( const Vector3 &lhs, const Matrix4x4 &rhs );
		friend const Matrix4x4 operator * ( FLOAT lhs, const Matrix4x4 &rhs );
		friend const Matrix4x4 operator * ( const Matrix4x4 &lhs, FLOAT rhs );

	private:

		Matrix4x4 GetAdjoint() const;

	public:

		union
		{
			struct
			{
				FLOAT _11, _12, _13, _14;
				FLOAT _21, _22, _23, _24;
				FLOAT _31, _32, _33, _34;
				FLOAT _41, _42, _43, _44;
			};
			FLOAT _m[4][4];
			FLOAT m[16];
		};

		static const Matrix4x4 IDENTITY;
		static const Matrix4x4 ZERO;

	};

	PSX_INLINE Matrix4x4::Matrix4x4( FLOAT _m11, FLOAT _m12, FLOAT _m13, FLOAT _m14,
							FLOAT _m21, FLOAT _m22, FLOAT _m23, FLOAT _m24,
							FLOAT _m31, FLOAT _m32, FLOAT _m33, FLOAT _m34,
							FLOAT _m41, FLOAT _m42, FLOAT _m43, FLOAT _m44 )
		: _11(_m11), _12(_m12), _13(_m13), _14(_m14),
		  _21(_m21), _22(_m22), _23(_m23), _24(_m24),
		  _31(_m31), _32(_m32), _33(_m33), _34(_m34),
		  _41(_m41), _42(_m42), _43(_m43), _44(_m44) 
	{

	}

	PSX_INLINE void Matrix4x4::Set( FLOAT _m11, FLOAT _m12, FLOAT _m13, FLOAT _m14,
				  FLOAT _m21, FLOAT _m22, FLOAT _m23, FLOAT _m24,
				  FLOAT _m31, FLOAT _m32, FLOAT _m33, FLOAT _m34,
				  FLOAT _m41, FLOAT _m42, FLOAT _m43, FLOAT _m44 )
	{
		_11 = _m11; _12 = _m12; _13 = _m13; _14 = _m14;
		_21 = _m21; _22 = _m22; _23 = _m23; _24 = _m24;
		_31 = _m31; _32 = _m32; _33 = _m33; _34 = _m34;
		_41 = _m41; _42 = _m42; _43 = _m43; _44 = _m44;
	}

	PSX_INLINE void Matrix4x4::BuildIdentity( void )
	{
		*this = IDENTITY;
	}

	PSX_INLINE void Matrix4x4::BuildZero( void )
	{
		*this = ZERO;
	}

	PSX_INLINE void Matrix4x4::Negate( void )
	{
		_11 = -_11;
		_12 = -_12;
		_13 = -_13;
		_14 = -_14;

		_21 = -_21;
		_22 = -_22;
		_23 = -_23;
		_24 = -_24;

		_31 = -_31;
		_32 = -_32;
		_33 = -_33;
		_34 = -_34;

		_41 = -_41;
		_42 = -_42;
		_43 = -_43;
		_44 = -_44;
	}

	PSX_INLINE void Matrix4x4::Invert( void )
	{
		FLOAT det = GetDeterminant();

		if ( !det )
			return;

		Matrix4x4 adj = GetAdjoint();
		adj.Transpose();

		*this = adj * ( 1.0f / det );
	}

	PSX_INLINE FLOAT Matrix4x4::GetDeterminant( void ) const
	{
		// Laplace theorem
		return ( _11 * GetMinor(1, 2, 3, 1, 2, 3) -
				 _21 * GetMinor(0, 2, 3, 1, 2, 3) +
				 _31 * GetMinor(0, 1, 3, 1, 2, 3) -
				 _41 * GetMinor(0, 1, 2, 1, 2, 3) );
	}

	PSX_INLINE FLOAT Matrix4x4::GetMinor( UINT r0, UINT r1, UINT r2, UINT c0, UINT c1, UINT c2 ) const
	{
		// Sarrus method
		return ( _m[r0][c0] * (_m[r1][c1] * _m[r2][c2] - _m[r2][c1] * _m[r1][c2]) +
				 _m[r0][c1] * (_m[r1][c2] * _m[r2][c0] - _m[r1][c0] * _m[r2][c2]) +
				 _m[r0][c2] * (_m[r1][c0] * _m[r2][c1] - _m[r1][c1] * _m[r2][c0]) );
	}

	PSX_INLINE void Matrix4x4::Transpose( void )
	{
		Matrix4x4 mat = *this;
		
		//_11 = mat._11;
		_12 = mat._21;
		_13 = mat._31;
		_14 = mat._41;

		_21 = mat._12;
		//_22 = mat._22;
		_23 = mat._32;
		_24 = mat._42;

		_31 = mat._13;
		_32 = mat._23;
		//_33 = mat._33;
		_34 = mat._43;

		_41 = mat._14;
		_42 = mat._24;
		_43 = mat._34;
		//_44 = mat._44;
	}

	PSX_INLINE void Matrix4x4::Add( const Matrix4x4 &rhs )
	{
		_11 += rhs._11;
		_12 += rhs._12;
		_13 += rhs._13;
		_14 += rhs._14;

		_21 += rhs._21;
		_22 += rhs._22;
		_23 += rhs._23;
		_24 += rhs._24;

		_31 += rhs._31;
		_32 += rhs._32;
		_33 += rhs._33;
		_34 += rhs._34;

		_31 += rhs._31;
		_32 += rhs._32;
		_33 += rhs._33;
		_34 += rhs._34;
	}

	PSX_INLINE void Matrix4x4::Subtract( const Matrix4x4 &rhs )
	{
		_11 -= rhs._11;
		_12 -= rhs._12;
		_13 -= rhs._13;
		_14 -= rhs._14;

		_21 -= rhs._21;
		_22 -= rhs._22;
		_23 -= rhs._23;
		_24 -= rhs._24;

		_31 -= rhs._31;
		_32 -= rhs._32;
		_33 -= rhs._33;
		_34 -= rhs._34;

		_31 -= rhs._31;
		_32 -= rhs._32;
		_33 -= rhs._33;
		_34 -= rhs._34;
	}

	PSX_INLINE void Matrix4x4::Multiply( const Matrix4x4 &rhs )
	{
		Matrix4x4 mat( *this );

		_11 = mat._11 * rhs._11 + mat._12 * rhs._21 + mat._13 * rhs._31 + mat._14 * rhs._41;
		_12 = mat._11 * rhs._12 + mat._12 * rhs._22 + mat._13 * rhs._32 + mat._14 * rhs._42;
		_13 = mat._11 * rhs._13 + mat._12 * rhs._23 + mat._13 * rhs._33 + mat._14 * rhs._43;
		_14 = mat._11 * rhs._14 + mat._12 * rhs._24 + mat._13 * rhs._34 + mat._14 * rhs._44;

		_21 = mat._21 * rhs._11 + mat._22 * rhs._21 + mat._23 * rhs._31 + mat._24 * rhs._41;
		_22 = mat._21 * rhs._12 + mat._22 * rhs._22 + mat._23 * rhs._32 + mat._24 * rhs._42;
		_23 = mat._21 * rhs._13 + mat._22 * rhs._23 + mat._23 * rhs._33 + mat._24 * rhs._43;
		_24 = mat._21 * rhs._14 + mat._22 * rhs._24 + mat._23 * rhs._34 + mat._24 * rhs._44;

		_31 = mat._31 * rhs._11 + mat._32 * rhs._21 + mat._33 * rhs._31 + mat._34 * rhs._41;
		_32 = mat._31 * rhs._12 + mat._32 * rhs._22 + mat._33 * rhs._32 + mat._34 * rhs._42;
		_33 = mat._31 * rhs._13 + mat._32 * rhs._23 + mat._33 * rhs._33 + mat._34 * rhs._43;
		_34 = mat._31 * rhs._14 + mat._32 * rhs._24 + mat._33 * rhs._34 + mat._34 * rhs._44;

		_41 = mat._41 * rhs._11 + mat._42 * rhs._21 + mat._43 * rhs._31 + mat._44 * rhs._41;
		_42 = mat._41 * rhs._12 + mat._42 * rhs._22 + mat._43 * rhs._32 + mat._44 * rhs._42;
		_43 = mat._41 * rhs._13 + mat._42 * rhs._23 + mat._43 * rhs._33 + mat._44 * rhs._43;
		_44 = mat._41 * rhs._14 + mat._42 * rhs._24 + mat._43 * rhs._34 + mat._44 * rhs._44;
	}

	PSX_INLINE Vector4 Multiply( const Vector4 &lhs, const Matrix4x4 &rhs )
	{
		Vector4 ret;
		
		ret.x = lhs.x * rhs._11 + lhs.y * rhs._21 + lhs.z * rhs._31 + lhs.w * rhs._41;
		ret.y = lhs.x * rhs._12 + lhs.y * rhs._22 + lhs.z * rhs._32 + lhs.w * rhs._42;
		ret.z = lhs.x * rhs._13 + lhs.y * rhs._23 + lhs.z * rhs._33 + lhs.w * rhs._43;
		ret.w = lhs.x * rhs._14 + lhs.y * rhs._24 + lhs.z * rhs._34 + lhs.w * rhs._44;
		
		return ret;
	}
		
	PSX_INLINE Vector3 Multiply( const Vector3 &lhs, const Matrix4x4 &rhs )
	{
		Vector3 ret;
		
		ret.x = lhs.x * rhs._11 + lhs.y * rhs._21 + lhs.z * rhs._31;
		ret.y = lhs.x * rhs._12 + lhs.y * rhs._22 + lhs.z * rhs._32;
		ret.z = lhs.x * rhs._13 + lhs.y * rhs._23 + lhs.z * rhs._33;
		
		return ret;
	}

	PSX_INLINE void Matrix4x4::Multiply( FLOAT rhs )
	{
		_11 *= rhs;
		_12 *= rhs;
		_13 *= rhs;
		_14 *= rhs;

		_21 *= rhs;
		_22 *= rhs;
		_23 *= rhs;
		_24 *= rhs;

		_31 *= rhs;
		_32 *= rhs;
		_33 *= rhs;
		_34 *= rhs;

		_41 *= rhs;
		_42 *= rhs;
		_43 *= rhs;
		_44 *= rhs;
	}

	PSX_INLINE void Matrix4x4::Divide( FLOAT rhs )
	{
		_11 /= rhs;
		_12 /= rhs;
		_13 /= rhs;
		_14 /= rhs;

		_21 /= rhs;
		_22 /= rhs;
		_23 /= rhs;
		_24 /= rhs;

		_31 /= rhs;
		_32 *= rhs;
		_33 *= rhs;
		_34 *= rhs;

		_41 *= rhs;
		_42 *= rhs;
		_43 *= rhs;
		_44 *= rhs;
	}

	PSX_INLINE void Matrix4x4::BuildTranslation( const Vector3 &trans )
	{
		BuildIdentity();
		_41 = trans.x;
		_42 = trans.y;
		_43 = trans.z;
	}

	PSX_INLINE void Matrix4x4::BuildTranslation( FLOAT x, FLOAT y, FLOAT z )
	{
		BuildIdentity();
		_41 = x;
		_42 = y;
		_43 = z;
	}

	PSX_INLINE void Matrix4x4::SetTranslation( const Vector3 &trans )
	{
		_41 = trans.x;
		_42 = trans.y;
		_43 = trans.z;
	}

	PSX_INLINE void Matrix4x4::SetTranslation( FLOAT x, FLOAT y, FLOAT z )
	{
		_41 = x;
		_42 = y;
		_43 = z;
	}

	PSX_INLINE Vector3 & Matrix4x4::GetTranslation( void )
	{
		return *(Vector3*)(_m[3]);
	}

	PSX_INLINE const Vector3 Matrix4x4::GetTranslation( void ) const
	{
		return Vector3( _41, _42, _43 );
	}

	PSX_INLINE void Matrix4x4::BuildRotationX( FLOAT angle )
	{
		BuildIdentity();

		FLOAT cs = Math::Cosf( angle );
		FLOAT sn = Math::Sinf( angle );

		_22 = cs;
		_23 = sn;
		_32 = -sn;
		_33 = cs;
	}

	PSX_INLINE void Matrix4x4::BuildRotationY( FLOAT angle )
	{
		BuildIdentity();

		FLOAT cs = Math::Cosf( angle );
		FLOAT sn = Math::Sinf( angle );

		_11 = cs;
		_13 = -sn;
		_31 = sn;
		_33 = cs;
	}

	PSX_INLINE void Matrix4x4::BuildRotationZ( FLOAT angle )
	{
		BuildIdentity();

		FLOAT cs = Math::Cosf( angle );
		FLOAT sn = Math::Sinf( angle );

		_11 = cs;
		_12 = sn;
		_21 = -sn;
		_22 = cs;
	}

	PSX_INLINE void Matrix4x4::BuildRotation( const Vector3 &axis, FLOAT angle )
	{
		BuildIdentity();

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
		
	PSX_INLINE void Matrix4x4::BuildRotationYawPitchRoll( const Vector3 &euler )
	{
		FLOAT sy = Math::Sinf(euler.z), cy = Math::Cosf(euler.z);
		FLOAT sp = Math::Sinf(euler.x), cp = Math::Cosf(euler.x);
		FLOAT sr = Math::Sinf(euler.y), cr = Math::Cosf(euler.y);

		_11 = cy*cr + sy*sp*sr;  _12 = sr*cp; _13 = -sy*cr + cy*sp*sr; _14 = 0.0f;
		_21 = -cy*sr + sy*sp*cr; _22 = cr*cp; _23 = sr*sy + cy*sp*cr;  _24 = 0.0f;
		_31 = sy*cp;             _32 = -sp;   _33 = cy*cp;             _34 = 0.0f;
		_41 = 0.0f;              _42 = 0.0f;  _43 = 0.0f;			   _44 = 1.0f;
	}

	PSX_INLINE void Matrix4x4::BuildRotationPitchYawRoll( const Vector3 &rot )
	{
		BuildRotationYawPitchRoll( rot.x, rot.y, rot.z );
	}

	PSX_INLINE void Matrix4x4::BuildRotationYawPitchRoll( FLOAT yaw, FLOAT pitch, FLOAT roll )
	{
		FLOAT sy = Math::Sinf(yaw),		cy = Math::Cosf(yaw);
		FLOAT sp = Math::Sinf(pitch),	cp = Math::Cosf(pitch);
		FLOAT sr = Math::Sinf(roll),	cr = Math::Cosf(roll);

		_11 = cy*cr + sy*sp*sr;  _12 = sr*cp; _13 = -sy*cr + cy*sp*sr; _14 = 0.0f;
		_21 = -cy*sr + sy*sp*cr; _22 = cr*cp; _23 = sr*sy + cy*sp*cr;  _24 = 0.0f;
		_31 = sy*cp;             _32 = -sp;   _33 = cy*cp;             _34 = 0.0f;
		_41 = 0.0f;              _42 = 0.0f;  _43 = 0.0f;			   _44 = 1.0f;
	}

	PSX_INLINE void Matrix4x4::BuildScale( const Vector3 &scale )
	{
		BuildIdentity();
		_11 = scale.x;
		_22 = scale.y;
		_33 = scale.z;
	}

	PSX_INLINE void Matrix4x4::BuildScale( FLOAT x, FLOAT y, FLOAT z )
	{
		BuildIdentity();
		_11 = x;
		_22 = y;
		_33 = z;
	}

	PSX_INLINE void Matrix4x4::BuildScale( FLOAT s )
	{
		BuildIdentity();
		_11 = s;
		_22 = s;
		_33 = s;
	}

	PSX_INLINE void Matrix4x4::BuildLookAt( const Vector3 &eyePos, const Vector3 &target, const Vector3 &up )
	{
		Vector3 z = Math::Normalize(target - eyePos);
		Vector3 x = Math::Normalize(up.CrossProduct(z));
		Vector3 y = z.CrossProduct(x);

		_11 = x.x;
		_12 = y.x;
		_13 = z.x;
		_14 = 0.0f;

		_21 = x.y;
		_22 = y.y;
		_23 = z.y;
		_24 = 0.0f;

		_31 = x.z;
		_32 = y.z;
		_33 = z.z;
		_34 = 0.0f;

		_41 = -x.DotProduct(eyePos);
		_42 = -y.DotProduct(eyePos);
		_43 = -z.DotProduct(eyePos);
		_44 = 1.0f;
	}

	PSX_INLINE void Matrix4x4::BuildOrthogonalProjection( FLOAT _width, FLOAT _height, FLOAT _near, FLOAT _far )
	{
		BuildIdentity();

		_11 = 2.0f / _width;
		_22 = 2.0f / _height;
		_33 = 1.0f / (_far - _near);
		_43 = _near / (_near - _far);
	}

	PSX_INLINE void Matrix4x4::BuildPerspectiveProjection( FLOAT _fovy, FLOAT _aspect, FLOAT _near, FLOAT _far )
	{
		BuildZero();

		FLOAT sinFOV = Math::Sinf( _fovy * 0.5f );
		FLOAT cosFOV = Math::Cosf( _fovy * 0.5f );

		FLOAT h = ( cosFOV / sinFOV );
		FLOAT w = h / _aspect;
		FLOAT q = _far / (_far - _near);

		_11 = w;
		_22 = h;
		_33 = q;
		_34 = 1.0f;
		_43 = -q * _near;

		//FLOAT yScale = 1.0f / Math::Tanf( _fovy * 0.5f );
		//FLOAT xScale = yScale / _aspect;
		//FLOAT q = _far / (_far - _near);

		//_11 = xScale;
		//_22 = yScale;
		//_33 = q;
		//_34 = 1.0f;
		//_43 = -(_near * q);

	}

	PSX_INLINE BOOL Matrix4x4::IsIdentity( void ) const
	{
		return *this == IDENTITY;
	}

	PSX_INLINE BOOL Matrix4x4::IsZero( void ) const
	{
		return *this == ZERO;
	}

	// Overloaded operators
	PSX_INLINE Matrix4x4 & Matrix4x4::operator += ( const Matrix4x4 &rhs )
	{
		Add( rhs );
		return *this;
	}

	PSX_INLINE Matrix4x4 & Matrix4x4::operator -= ( const Matrix4x4 &rhs )
	{
		Subtract( rhs );
		return *this;
	}

	PSX_INLINE Matrix4x4 & Matrix4x4::operator *= ( const Matrix4x4 &rhs )
	{
		Multiply( rhs );
		return *this;
	}

	PSX_INLINE Matrix4x4 & Matrix4x4::operator *= ( FLOAT rhs )
	{
		Multiply( rhs );
		return *this;
	}

	PSX_INLINE Matrix4x4 Matrix4x4::operator - ( void )
	{
		Matrix4x4 ret( *this );
		ret.Negate();
		return ret;
	}

	PSX_INLINE BOOL Matrix4x4::operator == ( const Matrix4x4 &rhs ) const
	{
		return  _11 == rhs._11 && _12 == rhs._12 && _13 == rhs._13 && _14 == rhs._14 &&
				_21 == rhs._21 && _22 == rhs._22 && _23 == rhs._23 && _24 == rhs._24 &&
				_31 == rhs._31 && _32 == rhs._32 && _33 == rhs._33 && _34 == rhs._34 &&
				_41 == rhs._41 && _42 == rhs._42 && _43 == rhs._43 && _44 == rhs._44;
	}

	PSX_INLINE FLOAT * Matrix4x4::operator [] ( UINT i )
	{
		PSX_Assert( i < 4, "Invalid index value." );
		return _m[i];
	}

	PSX_INLINE const Matrix4x4 operator + ( const Matrix4x4 &lhs, const Matrix4x4 &rhs )
	{
		Matrix4x4 ret( lhs );
		ret.Add( rhs );
		return ret;
	}

	PSX_INLINE const Matrix4x4 operator - ( const Matrix4x4 &lhs, const Matrix4x4 &rhs )
	{
		Matrix4x4 ret(lhs);
		ret.Subtract(rhs);
		return ret;
	}

	PSX_INLINE const Matrix4x4 operator * ( const Matrix4x4 &lhs, const Matrix4x4 &rhs )
	{
		Matrix4x4 ret(lhs);
		ret.Multiply(rhs);
		return ret;
	}

	PSX_INLINE const Vector4 operator * ( const Vector4 &lhs, const Matrix4x4 &rhs )
	{
		return Multiply( lhs, rhs );
	}

	PSX_INLINE const Vector3 operator * ( const Vector3 &lhs, const Matrix4x4 &rhs )
	{
		return Multiply( lhs, rhs );
	}

	PSX_INLINE const Matrix4x4 operator * ( FLOAT lhs, const Matrix4x4 &rhs )
	{
		Matrix4x4 ret(rhs);
		ret.Multiply( lhs );
		return ret;
	}

	PSX_INLINE const Matrix4x4 operator * ( const Matrix4x4 &lhs, FLOAT rhs )
	{
		Matrix4x4 ret(lhs);
		ret.Multiply( rhs );
		return ret;
	}

	PSX_INLINE Matrix4x4 Matrix4x4::GetAdjoint() const
	{
		return Matrix4x4( GetMinor(1, 2, 3, 1, 2, 3),
						  -GetMinor(1, 2, 3, 0, 2, 3),
						  GetMinor(1, 2, 3, 0, 1, 3),
						  -GetMinor(1, 2, 3, 0, 1, 2),
						 
						  -GetMinor(0, 2, 3, 1, 2, 3),
						  GetMinor(0, 2, 3, 0, 2, 3),
						  -GetMinor(0, 2, 3, 0, 1, 3),
						  GetMinor(0, 2, 3, 0, 1, 2),
						 
						  GetMinor(0, 1, 3, 1, 2, 3),
						  -GetMinor(0, 1, 3, 0, 2, 3),
						  GetMinor(0, 1, 3, 0, 1, 3),
						  -GetMinor(0, 1, 3, 0, 1, 2),
						 
						  -GetMinor(0, 1, 2, 1, 2, 3),
						  GetMinor(0, 1, 2, 0, 2, 3),
						  -GetMinor(0, 1, 2, 0, 1, 3),
						  GetMinor(0, 1, 2, 0, 1, 2) );
	}
}

#endif /* _MATRIX_4X4_H_ */