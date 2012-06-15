/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourceFile.h
*
*	Description -	Object-Oriented Pulse Virtual File System.
*
*	Comments	-	TODO: Change hard coded separators into a macro define or soemthing...
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	01/03/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_FILE_SYSTEM_H_
#define _PSX_FILE_SYSTEM_H_

#include "PulseSTD.h"
#include "Signature.h"
#include "SmartPointer.h"
#include "List.h"
#include "String.h"
#include "Array.h"
#include "Map.h"
#include "PTLBase.h"
#include "Optional.h"

#include <fstream>

namespace Pulse
{
	
	// A straight-forward fstream wrapper...
	// NOTE: This FStream wrapper can only handle files less than 4GB. This is due to
	// the ANSI design of datatype size_t. For working with files greater than 4GB use FileIO 
	// in FileIO.h instead.
	class FileIO;
	class File
	{
	public:

		typedef Char FILE_OPTION;

		enum {
			FILEOP_READ			=	std::ios_base::in, 
			FILEOP_WRITE		=	std::ios_base::out,
			FILEOP_BINARY		=	std::ios_base::binary,
			FILEOP_TRUNCATE		=	std::ios_base::trunc,
			FILEOP_APPENED		=	std::ios_base::app,
			FILEOP_GOTO_END		=	std::ios_base::ate,
		};

		enum SEEK_OPTION {
			SEEKOP_BEGIN	=	std::ios_base::beg,
			SEEKOP_END		=	std::ios_base::end,
			SEEKOP_CURRENT	=	std::ios_base::cur,
		};

		File( void );
		File( const CHAR *pFileName, FILE_OPTION options );
		~File( void ) { m_fileIO.close(); }

		BOOL Open( const CHAR *pFileName, FILE_OPTION options );
		void Close( void );

		// NOTE: Read and Write methods are templated so that we don't have to typecast the pointer to (CHAR *).
		// WARNING: Doing this method also accepts double, tripple, etc... pointers. This will result in an 
		//	undefined behavior. YOU SHOULD ONLY PASS SINGLE POINTERS.
		template < typename T >
		File & Write( const T *pStr, SIZE_T size );
		template < typename T >
		File & Read( T *pOutput, SIZE_T size );

		template < typename T >
		File & operator << ( const T & obj );
		template < typename T >
		File & operator >> ( T & obj );

		// NOTE: This works for both read and write.
		File & Seek( POS_T pos, SEEK_OPTION option );
		File & Seek( POS_T pos );

		POS_T Tell( void );

		File & Flush( void ); // The toilet?

		BOOL IsOpen( void );
		BOOL IsEOF( void );
		//BOOL IsOK( void ); // Check for the bad failbit
		BOOL IsWritable( void );
		BOOL IsReadable( void );

	public:
		
		Char m_fileOptions;

		#if defined( PSX_UNICODE )
			std::wfstream m_fileIO;
		#else
			std::fstream m_fileIO;
		#endif /* defined( PSX_UNICODE ) */

	};

	class ResourceFile
	{
	public:

		enum FILTER_TYPE { 
			FILTER_TYPE_DEFAULT		= 0x00000000,
			FILTER_TYPE_ZLIB_TEST	= 0x00000001, // NOTE: This zlib test filter doesn't support partial reads.
			// NOTE: Add additional filters here.
			//	Each filter takes one bit of space. 0, 1, 2, 4, 8...
			FILTER_TYPE_FORCE_DWORD = 0x7fffffff,
		};

		typedef FILTER_TYPE (*OnProcessFileCallback)( const CHAR *pFileName, const CHAR *pFilePath );
		typedef DWORD				FilterFlag;
		typedef Optional<PTR_T>		OHFile;

		// This class is a File Handle given by the ResourceFile that references an internal file inside a packed file.
		class FSHFile
		{
		public:

			FSHFile( void ) : m_pFileSystem( PSX_NULL ), m_ohFile( OptionalEmpty() ), m_localCurrPos( 0 ) { }
			FSHFile( ResourceFile *pFileSystem, OHFile &ohFile );

			BOOL IsValid( void );
			BOOL IsInvalid( void );

			const CHAR * GetName( void ) const;

			const SIZE_T	GetFileSize( void ) const;
			const SIZE_T64	GetFileSize64( void ) const;

			OHFile GetOHFile( void ) { return m_ohFile; }

			void SetPos( UINT pos );
			UINT GetPos( void );

			BOOL ReadFile( SIZE_T size, BYTE *pBuffer );
			BOOL ReadEntireFile( BYTE *pBuffer );

		private:


			ResourceFile *m_pFileSystem;
			OHFile	m_ohFile;
			// This is used for streaming technologies taking advantage of this File System.
			UINT	m_localCurrPos; 
		};

		friend FSHFile;

		ResourceFile( void );
		~ResourceFile( void );

		BOOL Open( const CHAR *pFileName, BOOL deleteTableEntries = TRUE );
		void Close( void );
		BOOL IsOpen( void ) const;
		
		FSHFile		FindFile( const CHAR *pPath ) const;
		SIZE_T		GetFileSize( const OHFile *pOHFile ) const;
		SIZE_T64	GetFileSize64( const OHFile *pOHFile ) const;
		BOOL		ReadFile( const OHFile *pOHFile, BYTE *pBuff );
		BOOL		ReadFile( const OHFile *pOHFile, const UINT pos, const SIZE_T size, BYTE *pBuffer ); 
		//SIZE_T		ReadFile( const OHFile *pOHFile, BYTE *pBuff, SIZE_T readStart, SIZE_T readSize );

		BOOL Create( const CHAR *pDirectory, const CHAR *pOutputPath, OnProcessFileCallback pOnProcessFileCallback = NULL, FilterFlag fBitFlag = FILTER_TYPE_DEFAULT );
		BOOL Unpack( const CHAR *pPAKPath, const CHAR *pOutputPath );

	private:

		// Internal file system data types.
		struct 	FileHeader;
		struct 	DirEntry;
		struct 	DirFileEntry;
		class	IDataFilter;
		class	DataFilter;
		class	ZLibTest;			// Test filter using zlib
		class 	IReader;
		class 	IWriter;
		class 	FileReader;
		class 	MemoryReader;
		class 	FileWriter;
		class 	MemoryWriter;

		// Internal bookeeping typedefs for generating or managing a package file.
		typedef SmartPointer< File >							FilePointer;
		typedef SmartPointer< IDataFilter >						FilterPointer;
		typedef String											FilePath;
		typedef String											DirPath;
		typedef String											Directory;
		typedef SmartPointer< FilePath >						FilePathPointer;
		typedef SmartPointer< DirPath >							DirPathPointer;
		typedef SmartPointer< Directory >						DirPointer;
		typedef List< FilePathPointer >							FilePathList;
		typedef List< DirPathPointer >							DirPathList;
		typedef List< DirPointer >								DirList;
		typedef SmartPointer< DirFileEntry >					FileEntryPointer;
		typedef PSX_Pair< FileEntryPointer, DirPathPointer >	FileEntryPair;
		typedef SmartPointer< DirEntry >						DirEntryPointer;
		typedef	List< DirEntryPointer >							PAKGenDirList;
		typedef List< FileEntryPair >							PAKGenFilePairList;
		typedef List< FileEntryPointer >						PAKGenFileList;
		typedef PSX_Pair< FILTER_TYPE, FilterPointer >			FilterPair;
		typedef Map< FILTER_TYPE, FilterPointer >				FilterMap;
		typedef PSX_Pair< String, FileEntryPointer >			FileMapPair;
		// TODO: We need to change this so that we can easily query directories.
		//	Or add a new container strictly for directory query feature.
		typedef Map< String, FileEntryPointer >					FileEntryMap;

		// Internal functions used to manage Pulse file data
		BOOL InitializeFilters( FilterFlag fFlag );
		void ReleaseFilters( void );
		BOOL _GenerateTableEntries( const CHAR *pFolderPath, const CHAR *pPAKPath );
		BOOL _CreatePulseFile( const CHAR *pOutputPath );
		BOOL ReadPAKInfo( const CHAR *pPAKFilePath, FileHeader *pPAKHeader, PAKGenDirList *pPAKDirList, PAKGenFileList *pPAKFileList );
		BOOL VerifyPulseHeader( struct FileHeader *pHeader );
		BOOL VerifyHeaderSignature( const Signature *pSig );
		BOOL VerifyHeaderFormat( const BYTE *pFormat );
		void Encode( DirFileEntry *pFileEntry, IReader *pReader, IWriter *pWriter );
		void Decode( DirFileEntry *pFileEntry, IReader *pReader, IWriter *pWriter );
		void ReleaseResources( void );

	private:

		PSX_DECLARE_SIGNATURE( SIGNATURE );

		FileHeader				*m_pHeader;
		PAKGenDirList			*m_pGenDirs;	
		PAKGenFilePairList		*m_pGenFiles;		// Used in creating Pulse File
		PAKGenFileList			*m_pGenFileList;	// Used in opening Pulse File
		FileEntryMap			*m_pFileMap;		// Used in loading Pulse File
		FileIO					*m_pPulseFile;		// Used in reading loaded Pulse File
		BOOL					m_bLoaded;
		FilterMap				m_filters;
		OnProcessFileCallback	m_pOnProcessFile;	// Callback for selecting a filter when a file is about to be processed

	};

	typedef ResourceFile::FSHFile FSHFile;

	// File inline definitions

	PSX_INLINE File::File( void )
		: m_fileOptions( 0 )
	{
	}

	PSX_INLINE File::File( const CHAR *pFileName, File::FILE_OPTION options )
	{
		PSX_Assert( pFileName, "Invalid filename specified." );
		Open( pFileName, options );
	}

	PSX_INLINE BOOL File::Open( const CHAR *pFileName, File::FILE_OPTION options )
	{
		m_fileOptions = options;
		m_fileIO.open( pFileName, options );
		return m_fileIO.is_open();
	}

	PSX_INLINE void File::Close( void )
	{
		m_fileOptions = 0;
		m_fileIO.clear(); // Not required in most OS but just to make sure...
		m_fileIO.close();
	}

	template < typename T >
	PSX_INLINE File & File::Write( const T *pStr, SIZE_T size )
	{
		m_fileIO.write( (const CHAR*)pStr, size );
		return *this;
	}

	template < typename T >
	PSX_INLINE File & File::Read( T *pOutput, SIZE_T size )
	{
		PSX_Assert( pOutput, "pOutput should point to memory." );
		if ( pOutput )
			m_fileIO.read( (CHAR*)pOutput, size );
		return *this;
	}

	template < typename T >
	PSX_INLINE File & File::operator << ( const T & obj )
	{
		m_fileIO << obj;
		return *this;
	}

	template < typename T >
	PSX_INLINE File & File::operator >> ( T & obj )
	{
		m_fileIO >> obj;
		return *this;
	}

	PSX_INLINE File & File::Seek( POS_T pos, SEEK_OPTION option )
	{
		m_fileIO.seekg( pos, option );
		return *this;
	}

	PSX_INLINE File & File::Seek( POS_T pos )
	{
		m_fileIO.seekg( pos );
		return *this;
	}

	PSX_INLINE POS_T File::Tell( void ) 
	{ 
		return (POS_T)m_fileIO.tellg(); 
	}

	PSX_INLINE File & File::Flush( void )
	{
		m_fileIO.flush();
		return *this;
	}

	PSX_INLINE BOOL File::IsOpen( void )
	{
		return m_fileIO.is_open();
	}
		
	PSX_INLINE BOOL File::IsEOF( void )
	{ 
		return m_fileIO.eof();
	}

	PSX_INLINE BOOL File::IsWritable( void )
	{
		return (m_fileOptions & FILEOP_WRITE) != 0;
	}

	PSX_INLINE BOOL File::IsReadable( void )
	{
		return (m_fileOptions & FILEOP_READ) != 0;
	}
}

#endif /* _PSX_FILE_SYSTEM_H_ */