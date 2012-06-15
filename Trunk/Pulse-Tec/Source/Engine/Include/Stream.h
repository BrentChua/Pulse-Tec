/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Stream.h
*
*	Description -	Base Stream class interface. Input and Output streams will derive
*						from here.
*
*	Comments	-	This is needed to abstract the IO streams... And I don't like it
*						working on this right now...
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	08/07/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_STREAM_H_
#define _PSX_STREAM_H_

#include "PulseSTD.h"
#include <stdio.h>

namespace Pulse
{

	class Stream
	{
	public:

		enum SEEK_OPTION {
			SEEKOP_BEGIN	=	SEEK_SET,
			SEEKOP_END		=	SEEK_END,
			SEEKOP_CURRENT	=	SEEK_CUR,
		};

		virtual ~Stream( void ) { }
		
		// NOTE: This works for both read and write.
		virtual Stream & Seek( POS_T pos, SEEK_OPTION option ) = 0;
		virtual Stream & Seek( POS_T pos ) = 0;
		virtual Stream & Seek64( POS_T64 pos, SEEK_OPTION option ) = 0;
		virtual Stream & Seek64( POS_T64 pos ) = 0;

		virtual POS_T Tell( void ) = 0;
		virtual POS_T64 Tell64( void ) = 0;

		virtual BOOL IsWritable( void ) = 0;
		
		virtual BOOL IsReadable( void ) = 0;

	};

	class IStream : public virtual Stream
	{
	public:

		virtual ~IStream( void ) { }
		
		// NOTE: Read and Write methods are templated so that we don't have to typecast the pointer to (CHAR *).
		//			This also calls the non-tempalted Read().
		// WARNING: Doing this method also accepts double, tripple, etc... pointers. This will result in an 
		//	undefined behavior. YOU SHOULD ONLY PASS SINGLE POINTERS.
		template < typename T >
		SIZE_T Read( T *pOutput, SIZE_T size ) { return Read( reinterpret_cast<BYTE*>(pOutput), size ); }
		
		virtual SIZE_T Read( BYTE *pOutput, SIZE_T size ) = 0;

		template < typename T >
		SIZE_T ReadString( T *pOutput, SIZE_T size ) { return ReadString( reinterpret_cast<CHAR *>(pOutput), size ); }

		virtual SIZE_T ReadString( CHAR *pOutput, SIZE_T size ) = 0;

		virtual SIZE_T Read( CHAR *pVal ) = 0;
		#if !defined( PSX_UNICODE )
		virtual SIZE_T Read( UCHAR *pVal ) = 0;
		#endif /* PSX_UNICODE  */
		#if defined( PSX_UNICODE )
			virtual SIZE_T Read( Char *pVal ) = 0;
		#endif /* PSX_UNICODE  */
		virtual SIZE_T Read( INT *pVal ) = 0;
		virtual SIZE_T Read( UINT *pVal ) = 0;
		virtual SIZE_T Read( SHORT *pVal ) = 0;
		virtual SIZE_T Read( USHORT *pVal ) = 0;
		virtual SIZE_T Read( LONG *pVal ) = 0;
		virtual SIZE_T Read( ULONG *pVal ) = 0;
		virtual SIZE_T Read( LONGLONG *pVal ) = 0;
		virtual SIZE_T Read( ULONGLONG *pVal ) = 0;
		virtual SIZE_T Read( FLOAT *pVal ) = 0;
		virtual SIZE_T Read( DOUBLE *pVal ) = 0;

		virtual IStream & operator >> ( CHAR &val ) = 0;
		#if !defined( PSX_UNICODE )
		virtual IStream & operator >> ( UCHAR &val ) = 0;
		#endif /* PSX_UNICODE */
		#if defined( PSX_UNICODE )
			virtual IStream & operator >> ( Char &val ) = 0;
		#endif /* PSX_UNICODE */
		virtual IStream & operator >> ( INT &val ) = 0;
		virtual IStream & operator >> ( UINT &val ) = 0;
		virtual IStream & operator >> ( SHORT &val ) = 0;
		virtual IStream & operator >> ( USHORT &val ) = 0;
		virtual IStream & operator >> ( LONG &val ) = 0;
		virtual IStream & operator >> ( ULONG &val ) = 0;
		virtual IStream & operator >> ( LONGLONG &val ) = 0;
		virtual IStream & operator >> ( ULONGLONG &val ) = 0;
		virtual IStream & operator >> ( FLOAT &val ) = 0;
		virtual IStream & operator >> ( DOUBLE &val ) = 0;

		virtual const CHAR * GetLine( String &str ) = 0;

		virtual BOOL IsOpen( void ) = 0;
		virtual BOOL IsEOF( void ) = 0;
		virtual BOOL IsOK( void ) = 0; 
		virtual BOOL IsError( void ) = 0;

	};

	class OStream : public virtual Stream
	{
	public:
		
		virtual ~OStream( void ) { }

		// NOTE: Read and Write methods are templated so that we don't have to typecast the pointer to (CHAR *).
		//			This also calls the non-tempalted Write().
		// WARNING: Doing this method also accepts double, tripple, etc... pointers. This will result in an 
		//	undefined behavior. YOU SHOULD ONLY PASS SINGLE POINTERS. 
		template < typename T >
		SIZE_T Write( const T *pStr, SIZE_T size ) { return Write( reinterpret_cast<const BYTE*>(pStr), size ); }

		virtual SIZE_T Write( const BYTE *pBuffer, SIZE_T size ) = 0;

		template < typename T >
		SIZE_T WriteString( const T *pStr, SIZE_T size ) { return WriteString( reinterpret_cast<const CHAR *>(pStr), size ); }

		virtual SIZE_T WriteString( const CHAR *pStr, SIZE_T size ) = 0;

		virtual SIZE_T Write( const CHAR *pVal ) = 0;
		#if !defined( PSX_UNICODE )
		virtual SIZE_T Write( const UCHAR *pVal ) = 0;
		#endif /* PSX_UNICODE  */
		#if defined( PSX_UNICODE )
			virtual SIZE_T Write( const Char *pVal ) = 0;
		#endif /* PSX_UNICODE  */
		virtual SIZE_T Write( const INT *pVal ) = 0;
		virtual SIZE_T Write( const UINT *pVal ) = 0;
		virtual SIZE_T Write( const SHORT *pVal ) = 0;
		virtual SIZE_T Write( const USHORT *pVal ) = 0;
		virtual SIZE_T Write( const LONG *pVal ) = 0;
		virtual SIZE_T Write( const ULONG *pVal ) = 0;
		virtual SIZE_T Write( const LONGLONG *pVal ) = 0;
		virtual SIZE_T Write( const ULONGLONG *pVal ) = 0;
		virtual SIZE_T Write( const FLOAT *pVal ) = 0;
		virtual SIZE_T Write( const DOUBLE *pVal ) = 0;

		virtual OStream & operator << ( const CHAR &val ) = 0;
		#if !defined( PSX_UNICODE )
		virtual OStream & operator << ( const UCHAR &val ) = 0;
		#endif /* PSX_UNICODE */
		#if defined( PSX_UNICODE )
			virtual OStream & operator << ( const Char &val ) = 0;
		#endif /* PSX_UNICODE */
		virtual OStream & operator << ( const INT &val ) = 0;
		virtual OStream & operator << ( const UINT &val ) = 0;
		virtual OStream & operator << ( const SHORT &val ) = 0;
		virtual OStream & operator << ( const USHORT &val ) = 0;
		virtual OStream & operator << ( const LONG &val ) = 0;
		virtual OStream & operator << ( const ULONG &val ) = 0;
		virtual OStream & operator << ( const LONGLONG &val ) = 0;
		virtual OStream & operator << ( const ULONGLONG &val ) = 0;
		virtual OStream & operator << ( const FLOAT &val ) = 0;
		virtual OStream & operator << ( const DOUBLE &val ) = 0;

		virtual OStream & Flush( void ) = 0;

	};

}

#endif /* _PSX_STREAM_H_ */