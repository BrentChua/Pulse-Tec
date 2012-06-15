/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	FileIO.h
*
*	Description -	Wrapper for fStream operations.
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	01/10/2010		-	FileIO creation
**************************************************************************************/

#ifndef PSX_FILEIO_H_
#define PSX_FILEIO_H_

#include "PulseSTD.h"
#include "Stream.h"
#include <stdio.h>

namespace Pulse
{
	class FileIO : public IStream, public OStream
	{
	public:

		typedef INT FILEOP;

		enum {
			FILEOP_READ			=	0x01, 
			FILEOP_WRITE		=	0x02,
			FILEOP_BINARY		=	0x04,
			FILEOP_TRUNCATE		=	0x08,
			FILEOP_APPENED		=	0x10,
			FILEOP_GOTO_END		=	0x20,
			FILEOP_FORCE_DWORD	=	0x7fffffff
		};

		//enum SEEK_OPTION {
		//	SEEKOP_BEGIN	=	SEEK_SET,
		//	SEEKOP_END		=	SEEK_END,
		//	SEEKOP_CURRENT	=	SEEK_CUR,
		//};

		typedef Stream::SEEK_OPTION SEEK_OPTION;

		FileIO( void );
		FileIO( const CHAR *pFileName, FILEOP options );
		~FileIO( void );

		BOOL Open( const CHAR *pFileName, FILEOP options );
		void Close( void );

		// NOTE: Read and Write methods are templated so that we don't have to typecast the pointer to (CHAR *).
		// WARNING: Doing this method also accepts double, tripple, etc... pointers. This will result in an 
		//	undefined behavior. YOU SHOULD ONLY PASS SINGLE POINTERS.
		template < typename T >
		SIZE_T Write( const T *pStr, SIZE_T size );
		virtual SIZE_T Write( const BYTE *pStr, SIZE_T size );
		
		template < typename T >
		SIZE_T Read( T *pOutput, SIZE_T size );
		virtual SIZE_T Read( BYTE *pOutput, SIZE_T size );

		template < typename T >
		SIZE_T WriteString( const T *pStr, SIZE_T size );
		SIZE_T WriteString( const CHAR *pStr, SIZE_T size );
		SIZE_T WriteString( const CHAR *pStr );

		template < typename T >
		SIZE_T ReadString( T *pOutput, SIZE_T size );
		SIZE_T ReadString( CHAR *pOutput, SIZE_T size );

		virtual SIZE_T Write( const CHAR *pVal );
		#if !defined( PSX_UNICODE )
		virtual SIZE_T Write( const UCHAR *pVal );
		#endif /* PSX_UNICODE  */
		#if defined( PSX_UNICODE )
			virtual SIZE_T Write( const Char *pVal );
		#endif /* PSX_UNICODE  */
		virtual SIZE_T Write( const INT *pVal );
		virtual SIZE_T Write( const UINT *pVal );
		virtual SIZE_T Write( const SHORT *pVal );
		virtual SIZE_T Write( const USHORT *pVal );
		virtual SIZE_T Write( const LONG *pVal );
		virtual SIZE_T Write( const ULONG *pVal );
		virtual SIZE_T Write( const LONGLONG *pVal );
		virtual SIZE_T Write( const ULONGLONG *pVal );
		virtual SIZE_T Write( const FLOAT *pVal );
		virtual SIZE_T Write( const DOUBLE *pVal );

		virtual SIZE_T Read( CHAR *pVal );
		#if !defined( PSX_UNICODE )
		virtual SIZE_T Read( UCHAR *pVal );
		#endif /* PSX_UNICODE  */
		#if defined( PSX_UNICODE )
			virtual SIZE_T Read( Char *pVal );
		#endif /* PSX_UNICODE  */
		virtual SIZE_T Read( INT *pVal );
		virtual SIZE_T Read( UINT *pVal );
		virtual SIZE_T Read( SHORT *pVal );
		virtual SIZE_T Read( USHORT *pVal );
		virtual SIZE_T Read( LONG *pVal );
		virtual SIZE_T Read( ULONG *pVal );
		virtual SIZE_T Read( LONGLONG *pVal );
		virtual SIZE_T Read( ULONGLONG *pVal );
		virtual SIZE_T Read( FLOAT *pVal );
		virtual SIZE_T Read( DOUBLE *pVal );

		virtual FileIO & operator << ( const CHAR &val );
		#if !defined( PSX_UNICODE )
		virtual FileIO & operator << ( const UCHAR &val );
		#endif /* PSX_UNICODE */
		#if defined( PSX_UNICODE )
			virtual FileIO & operator << ( const Char &val );
		#endif /* PSX_UNICODE */
		virtual FileIO & operator << ( const INT &val );
		virtual FileIO & operator << ( const UINT &val );
		virtual FileIO & operator << ( const SHORT &val );
		virtual FileIO & operator << ( const USHORT &val );
		virtual FileIO & operator << ( const LONG &val );
		virtual FileIO & operator << ( const ULONG &val );
		virtual FileIO & operator << ( const LONGLONG &val );
		virtual FileIO & operator << ( const ULONGLONG &val );
		virtual FileIO & operator << ( const FLOAT &val );
		virtual FileIO & operator << ( const DOUBLE &val );

		virtual FileIO & operator >> ( CHAR &val );
		#if !defined( PSX_UNICODE )
		virtual FileIO & operator >> ( UCHAR &val );
		#endif /* PSX_UNICODE */
		#if defined( PSX_UNICODE )
			virtual FileIO & operator >> ( Char &val );
		#endif /* PSX_UNICODE */
		virtual FileIO & operator >> ( INT &val );
		virtual FileIO & operator >> ( UINT &val );
		virtual FileIO & operator >> ( SHORT &val );
		virtual FileIO & operator >> ( USHORT &val );
		virtual FileIO & operator >> ( LONG &val );
		virtual FileIO & operator >> ( ULONG &val );
		virtual FileIO & operator >> ( LONGLONG &val );
		virtual FileIO & operator >> ( ULONGLONG &val );
		virtual FileIO & operator >> ( FLOAT &val );
		virtual FileIO & operator >> ( DOUBLE &val );

		const CHAR * GetLine( String &str );

		// NOTE: This works for both read and write.
		FileIO & Seek( POS_T pos, SEEK_OPTION option );
		FileIO & Seek( POS_T pos );
		FileIO & Seek64( POS_T64 pos, SEEK_OPTION option );
		FileIO & Seek64( POS_T64 pos );

		POS_T Tell( void );
		POS_T64 Tell64( void );

		virtual FileIO & Flush( void ); // The toilet?

		BOOL IsOpen( void );
		BOOL IsEOF( void );
		BOOL IsOK( void ); // Check for the bad failbit
		BOOL IsError( void );
		virtual BOOL IsWritable( void );
		virtual BOOL IsReadable( void );

		void ClearError( void );

	private:

		FILEOP  m_fileOP;
		FILE	*m_pFStream;

	};

	PSX_INLINE FileIO::FileIO( void )
		: m_pFStream( NULL )
	{

	}

	PSX_INLINE FileIO::FileIO( const CHAR *pFileName, FILEOP options )
		: m_pFStream( NULL )
	{
		Open( pFileName, options );
	}

	PSX_INLINE FileIO::~FileIO( void )
	{
		Close();
	}

	PSX_INLINE void FileIO::Close( void )
	{
		if ( m_pFStream )
		{
			// Automaticaly flushes the stream buffer
			fclose( m_pFStream );
			m_pFStream = NULL;
			m_fileOP = 0;
		}
	}

	template < typename T >
	PSX_INLINE SIZE_T FileIO::Write( const T *pStr, SIZE_T size )
	{
		return Write( (const BYTE *)pStr, size );
	}

	template < typename T >
	PSX_INLINE SIZE_T FileIO::Read( T *pOutput, SIZE_T size )
	{
		return Read( (BYTE*)pOutput, size );
	}

	PSX_INLINE SIZE_T FileIO::Write( const BYTE *pStr, SIZE_T size )
	{
		return fwrite( (void *)pStr, sizeof(Char), size, m_pFStream );
	}

	PSX_INLINE SIZE_T FileIO::Read( BYTE *pOutput, SIZE_T size )
	{
		return fread( (void*)pOutput, sizeof(Char), size, m_pFStream );
	}

	PSX_INLINE SIZE_T FileIO::Write( const CHAR *pVal )
	{
		return Write( &pVal, sizeof(CHAR) );
	}

#if !defined(PSX_UNICODE)
	PSX_INLINE SIZE_T FileIO::Write( const UCHAR *pVal )
	{
		return Write( pVal, sizeof(UCHAR) );
	}
#endif

	#if defined( PSX_UNICODE )
		PSX_INLINE SIZE_T FileIO::Write( const Char *pVal )
		{
			return Write( pVal, sizeof(Char) );
		}
	#endif /* PSX_UNICODE  */

	PSX_INLINE SIZE_T FileIO::Write( const INT *pVal )
	{
		return Write( pVal, sizeof(INT) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const UINT *pVal )
	{
		return Write( pVal, sizeof(UINT) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const SHORT *pVal )
	{
		return Write( pVal, sizeof(SHORT) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const USHORT *pVal )
	{
		return Write( pVal, sizeof(USHORT) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const LONG *pVal )
	{
		return Write( pVal, sizeof(LONG) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const ULONG *pVal )
	{
		return Write( pVal, sizeof(ULONG) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const LONGLONG *pVal )
	{
		return Write( pVal, sizeof(LONGLONG) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const ULONGLONG *pVal )
	{
		return Write( pVal, sizeof(ULONGLONG) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const FLOAT *pVal )
	{
		return Write( pVal, sizeof(FLOAT) );
	}

	PSX_INLINE SIZE_T FileIO::Write( const DOUBLE *pVal )
	{
		return Write( pVal, sizeof(DOUBLE) );
	}

	PSX_INLINE SIZE_T FileIO::Read( CHAR *pVal )
	{
		return Read( pVal, sizeof(CHAR) );
	}

#if !defined(PSX_UNICODE)
	PSX_INLINE SIZE_T FileIO::Read( UCHAR *pVal )
	{
		return Read( pVal, sizeof(UCHAR) );
	}
#endif

	#if defined( PSX_UNICODE )
		PSX_INLINE SIZE_T FileIO::Read( Char *pVal )
		{
			return Read( pVal, sizeof(Char) );
		}
	#endif /* PSX_UNICODE  */

	PSX_INLINE SIZE_T FileIO::Read( INT *pVal )
	{
		return Read( pVal, sizeof(INT) );
	}

	PSX_INLINE SIZE_T FileIO::Read( UINT *pVal )
	{
		return Read( pVal, sizeof(UINT) );
	}

	PSX_INLINE SIZE_T FileIO::Read( SHORT *pVal )
	{
		return Read( pVal, sizeof(SHORT) );
	}

	PSX_INLINE SIZE_T FileIO::Read( USHORT *pVal )
	{
		return Read( pVal, sizeof(USHORT) );
	}

	PSX_INLINE SIZE_T FileIO::Read( LONG *pVal )
	{
		return Read( pVal, sizeof(LONG) );
	}

	PSX_INLINE SIZE_T FileIO::Read( ULONG *pVal )
	{
		return Read( pVal, sizeof(ULONG) );
	}

	PSX_INLINE SIZE_T FileIO::Read( LONGLONG *pVal )
	{
		return Read( pVal, sizeof(LONGLONG) );
	}

	PSX_INLINE SIZE_T FileIO::Read( ULONGLONG *pVal )
	{
		return Read( pVal, sizeof(ULONGLONG) );
	}

	PSX_INLINE SIZE_T FileIO::Read( FLOAT *pVal )
	{
		return Read( pVal, sizeof(FLOAT) );
	}

	PSX_INLINE SIZE_T FileIO::Read( DOUBLE *pVal )
	{
		return Read( pVal, sizeof(DOUBLE) );
	}

	template < typename T >
	PSX_INLINE SIZE_T FileIO::WriteString( const T *pStr, SIZE_T size )
	{
		//return fwrite( (void *)pStr, sizeof(CHAR), size, m_pFStream );
		return WriteString( (const CHAR *)pStr, size );
	}

	PSX_INLINE SIZE_T FileIO::WriteString( const CHAR *pStr, SIZE_T size )
	{
		return fwrite( (void *)pStr, sizeof(CHAR), size, m_pFStream );
	}

	PSX_INLINE SIZE_T FileIO::WriteString( const CHAR *pStr )
	{
		return fwrite( (void *)pStr, sizeof(CHAR), PSX_StrLen( pStr ), m_pFStream );
	}

	template < typename T >
	PSX_INLINE SIZE_T FileIO::ReadString( T *pOutput, SIZE_T size )
	{
		//return fread( (void*)pOutput, sizeof(CHAR), size, m_pFStream );
		return ReadString( (CHAR*)pOutput, size );
	}

	PSX_INLINE SIZE_T FileIO::ReadString( CHAR *pOutput, SIZE_T size )
	{
		return fread( (void*)pOutput, sizeof(CHAR), size, m_pFStream );
	}

	PSX_INLINE FileIO & FileIO::operator << ( const CHAR &val )
	{
		Write( &val );
		return *this;
	}

#if !defined(PSX_UNICODE)
	PSX_INLINE FileIO & FileIO::operator << ( const UCHAR &val )
	{
		Write( &val );
		return *this;
	}
#endif

	#if defined( PSX_UNICODE )
		PSX_INLINE FileIO & FileIO::operator << ( const Char &val )
		{
			Write( &val );
			return *this;
		}
	#endif /* PSX_UNICODE */

	PSX_INLINE FileIO & FileIO::operator << ( const INT &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const UINT &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const SHORT &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const USHORT &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const LONG &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const ULONG &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const LONGLONG &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const ULONGLONG &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const FLOAT &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator << ( const DOUBLE &val )
	{
		Write( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( CHAR &val )
	{
		Read( &val );
		return *this;
	}

#if !defined(PSX_UNICODE)
	PSX_INLINE FileIO & FileIO::operator >> ( UCHAR &val )
	{
		Read( &val );
		return *this;
	}
#endif

	#if defined( PSX_UNICODE )
		PSX_INLINE FileIO & FileIO::operator >> ( Char &val )
		{
			Read( &val );
			return *this;
		}
	#endif /* PSX_UNICODE */

	PSX_INLINE FileIO & FileIO::operator >> ( INT &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( UINT &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( SHORT &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( USHORT &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( LONG &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( ULONG &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( LONGLONG &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( ULONGLONG &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( FLOAT &val )
	{
		Read( &val );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::operator >> ( DOUBLE &val )
	{
		Read( &val );
		return *this;
	}

	// NOTE: This works for both read and write.
	PSX_INLINE FileIO & FileIO::Seek( POS_T pos, SEEK_OPTION option )
	{
		fseek( m_pFStream, pos, option );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::Seek( POS_T pos )
	{
		fseek( m_pFStream, pos, SEEKOP_BEGIN );
		return *this;
	}

	PSX_INLINE FileIO & FileIO::Seek64( POS_T64 pos, SEEK_OPTION option )
	{
		_fseeki64( m_pFStream, pos, option );
		return *this;
	}
	
	PSX_INLINE FileIO & FileIO::Seek64( POS_T64 pos )
	{
		_fseeki64( m_pFStream, pos, SEEKOP_BEGIN );
		return *this;
	}

	PSX_INLINE POS_T FileIO::Tell( void )
	{
		return ftell( m_pFStream );
	}

	PSX_INLINE POS_T64 FileIO::Tell64( void )
	{
		return _ftelli64( m_pFStream );
	}

	PSX_INLINE FileIO & FileIO::Flush( void ) // The toilet?
	{
		fflush( m_pFStream );
		return *this;
	}

	PSX_INLINE BOOL FileIO::IsEOF( void )
	{	
		return feof( m_pFStream ) != 0;
	}

	PSX_INLINE BOOL FileIO::IsOpen( void )
	{
		return m_pFStream != NULL;
	}

	PSX_INLINE BOOL FileIO::IsOK( void )
	{
		return !IsError();
	}

	PSX_INLINE BOOL FileIO::IsError( void )
	{
		return (ferror( m_pFStream ) == 0);
	}

	PSX_INLINE BOOL FileIO::IsWritable( void )
	{
		return (m_fileOP & FILEOP_WRITE) != 0;
	}

	PSX_INLINE BOOL FileIO::IsReadable( void )
	{
		return (m_fileOP & FILEOP_READ) != 0;
	}

	PSX_INLINE void FileIO::ClearError( void )
	{
		clearerr( m_pFStream );
	}

}

#endif /* PSX_FILEIO_H_ */