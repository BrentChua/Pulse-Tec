/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Platform.h
*
*	Description -	Platform specific macros, basic type declerations, etc.
*
*	Comments	-	Currently only supports Windows platform.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/11/2009	-	Platform for windows and basic types
**************************************************************************************/
#ifndef _PSX_PLATFORM_H_
#define _PSX_PLATFORM_H_

#include "PulseConfig.h"

namespace Pulse
{
	//#if defined( PSX_PLATFORM_WINDOWS_WINDOW )
	
	//#endif
		
	#define CONST const
	#define NULL 0
	#define VOID void 
	#define PVOID void *

	#if defined( PSX_PLATFORM_WINDOWS ) // Windows
		#define PSX_INLINE				__inline
		#define PSX_FORCE_INLINE		__forceinline
		#define PSX_ALIGN( alignment )	__declspec( align( alignment ) )
		#define PSX_ALIGN_OF( type )	__alignof( type )

		#define PSX_LINE __LINE__

		#if defined( PSX_UNICODE )
			#define PSX_FILE __FILEW__
		#else
			#define PSX_FILE __FILE__
		#endif /* PSX_UNICODE */

		#if defined( PSX_UNICODE )
			#define PSX_FUNCTION __FUNCTIONW__
		#else
			#define PSX_FUNCTION __FUNCTION__
		#endif /* PSX_UNICODE */

		#define PSX_KILOBYTE	1024						// 1 Kilobyte = 1024 bytes
		#define PSX_MEGABYTE	(PSX_KILOBYTE * 1024)		// 1 Megabyte = 1 kilobyte * 1024
		#define PSX_GIGABYTE	(PSX_MEGABYTE * 1024)		// 1 Gigabyte = 1 Megabyte * 1024

		/* Limits */
		#include <limits.h>
		#define PSX_MAX_PATH	MAX_PATH		/* 260										*/
		#define PSX_CHAR_BIT	CHAR_BIT		/* 8										*/
		#define PSX_SCHAR_MIN	SCHAR_MIN		/* -128										*/
		#define PSX_SCHAR_MAX	SCHAR_MAX		/* 127										*/
		#define PSX_UCHAR_MAX	UCHAR_MAX		/* 255 (0xff)								*/
		#define PSX_CHAR_MIN	CHAR_MIN		/* –128; 0 if /J option used				*/
		#define PSX_CHAR_MAX	CHAR_MAX		/* 127; 255 if /J option used				*/
		#define PSX_MB_LEN_MAX	MB_LEN_MAX		/* 5										*/
		#define PSX_SHORT_MIN	SHRT_MIN		/* –32768									*/
		#define PSX_SHORT_MAX	SHRT_MAX		/* 32767									*/
		#define PSX_USHORT_MAX	USHRT_MAX		/* 65535 (0xffff)							*/
		#define PSX_INT_MIN		INT_MIN			/* –2147483648								*/
		#define PSX_INT_MAX		INT_MAX			/* 2147483647								*/
		#define PSX_UINT_MAX	UINT_MAX		/* 4294967295 (0xffffffff)					*/
		#define PSX_LONG_MIN	LONG_MIN		/* –2147483648								*/
		#define PSX_LONG_MAX	LONG_MAX		/* 2147483647								*/
		#define PSX_ULONG_MAX	ULONG_MAX		/* 4294967295 (0xffffffff)					*/
		#define PSX_INT64_MIN	_I64_MIN		/* -9223372036854775808						*/
		#define PSX_INT64_MAX	_I64_MAX		/* 9223372036854775807						*/
		#define PSX_UINT64_MAX	_UI64_MAX		/* 18446744073709551615 (0xffffffffffffffff)*/
		#define PSX_LINE_MAX	 (PSX_UINT_MAX - 1) // I'm not exactly sure if this changes in x64-bit OS.

		#define PSX_NULL 0

		// Pulse specific limits
		#if defined( _WIN64 )
			#define PSX_SIZE_T_MAX	PSX_UINT64_MAX
			#define PSX_POS_T_MAX	PSX_INT64_MAX
			#define PSX_POS_T_MIN	PSX_INT64_MIN
		#else
			#define PSX_SIZE_T_MAX	PSX_UINT_MAX
			#define PSX_POS_T_MAX	PSX_INT_MAX
			#define PSX_POS_T_MIN	PSX_INT_MIN
		#endif

		typedef char				I8;
		typedef unsigned char		U8;
		typedef short				I16;
		typedef unsigned short		U16;
		typedef int					I32;
		typedef unsigned int		U32;
		typedef __int64				I64;
		typedef unsigned __int64	U64;
		typedef float				F32;
		typedef double				F64;

		#ifdef TRUE
			#undef TRUE
		#endif
		#ifdef FALSE
			#undef FALSE
		#endif

		#ifdef PSX_PLATFORM_WINDOWS_WINDOW
			typedef bool	BOOL;
			#define TRUE	true
			#define FALSE	false
		#else
			typedef bool BOOL;
			#define TRUE	true
			#define FALSE	false
		#endif
		
		#if defined( PSX_UNICODE )
				//#define CHAR	wchar_t				
				//#define UCHAR	unsigned wchar_t

				typedef wchar_t CHAR;
				typedef wchar_t UCHAR;
		#else
				//#define CHAR	char				
				//#define UCHAR	unsigned char
				typedef char CHAR;
				typedef unsigned char UCHAR;
		#endif

		// We need a simple char data type.
		// CHAR changes to wchar_t when in UNICODE
		typedef char			Char;
		typedef unsigned char	UChar;

		typedef I32					INT;
		typedef U32					UINT;
		typedef I16					SHORT;
		typedef U16					USHORT;
		typedef long				LONG;
		typedef unsigned long		ULONG;
		typedef long long			LONGLONG;
		typedef unsigned long long	ULONGLONG;
		typedef F32					FLOAT;
		typedef F64					DOUBLE;
		typedef long double			LONGDOUBLE;
		typedef UChar				BYTE;
		typedef ULONG				DWORD;
		typedef USHORT				WORD;

		// TODO: We need a 128 bit type... So how do we do that?

		#if defined( _WIN64 )
			typedef U64		SIZE_T;
			typedef I64		POS_T;
			typedef VOID *	PTR_T;
			typedef	I64		INT_PTR;
			typedef	U64		UINT_PTR;
			typedef I64		LONG_PTR;
		#else
			typedef U32		SIZE_T;
			typedef I32		POS_T;
			typedef VOID *	PTR_T;
			typedef	I32		INT_PTR;
			typedef	U32		UINT_PTR;
			typedef LONG	LONG_PTR;
		#endif

		typedef void*		HANDLE;
		typedef LONG		HRESULT;
		typedef UINT_PTR	WPARAM;
		typedef LONG_PTR	LPARAM;

		// If we need 64bit precision for size and pos
		typedef U64 SIZE_T64;
		typedef LONGLONG POS_T64;

		// Constants
		static const SIZE_T PSX_NPOS = -1;

	#else
		#error Unsupported Platform.
	#endif


};

#endif /* _PSX_PLATFORM_H_ */
