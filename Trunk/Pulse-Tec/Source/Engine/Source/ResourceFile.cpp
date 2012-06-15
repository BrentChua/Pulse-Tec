/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	ResourceFile.cpp
*
*	Comments	-	See ResourceFile.h
*
**************************************************************************************/

#include "../Include/ResourceFile.h"
#include "../Include/System.h"
#include "../Include/String.h"
#include "../Include/FileIO.h"

#include "../External/zlib/zlib.h"
#include "../Include/MemoryManager.h"

#include <iostream>

namespace Pulse
{

	PSX_DEFINE_SIGNATURE( 
		ResourceFile::SIGNATURE, 
		0x7c7769f4, 
		0x14ad, 0x42e0, 
		0x84, 0xb2, 0x9e, 0xb3, 0x85, 0xee, 0xdb, 0x9a 
	);

	#pragma pack(1)
	struct ResourceFile::FileHeader
	{
		Signature	m_signature;		// 16 bit GUID signature check.
		BYTE		m_ID[4];			// 3 letter file format for format check.
		DWORD		m_version;			// Version of this Pulse File.
		WORD		m_numDirs;			// Number of directories.
		WORD		m_numFiles;			// Number of files.
		I32			m_filterBitField;	// Max of 32 possible filter algorithms to choose from.
		SIZE_T64	m_size;				// Size of the file.
		POS_T64		m_dirDiskStart;
		POS_T64		m_fileDiskStart;

		FileHeader( void )
		{
			PSX_ZeroMem( this, sizeof( FileHeader ) );
		}

		void WriteData( ResourceFile::IWriter *pWriter );
		void ReadData( ResourceFile::IReader *pReader );
	};
	#pragma pack()

	struct ResourceFile::DirEntry
	{
		struct PAKData
		{
			WORD m_nameLen;
		};

		String m_name;
		PAKData m_PAKData;

		void WriteData( ResourceFile::IWriter *pWriter );
		void ReadData( ResourceFile::IReader *pReader );
	};

	struct ResourceFile::DirFileEntry
	{
		#pragma pack( 1 )
		struct PAKData
		{
			SIZE_T64	m_size;
			SIZE_T64	m_compressedSize;
			SIZE_T64	m_diskStart;
			DWORD		m_filterBit;
			DWORD		m_pathIndex;		// Points to the position of the dirpath entries
			DWORD		m_nameLen;
			BYTE		_padd[4];

			PAKData( void ) { PSX_ZeroMem( this, sizeof( PAKData ) ); }
		};
		#pragma pack()

		String	m_name;
		PAKData m_PAKData;

		void WriteData( ResourceFile::IWriter *pWriter );
		void ReadData( ResourceFile::IReader *pReader );
	};

	class ResourceFile::IDataFilter
	{
	public:

		virtual ~IDataFilter() { }

		virtual BOOL IsPartialReadSupported( void ) = 0;

		virtual BOOL Encode( class IReader *pReader, class IWriter *pWriter ) = 0;
		virtual BOOL Decode( class IReader *pReader, class IWriter *pWriter ) = 0;
	};

	class ResourceFile::DataFilter : public ResourceFile::IDataFilter
	{
	public:

		virtual ~DataFilter() { }

		virtual BOOL IsPartialReadSupported( void ) { return TRUE; }

		virtual BOOL Encode( class IReader *pReader, class IWriter *pWriter );
		virtual BOOL Decode( class IReader *pReader, class IWriter *pWriter );
	};

	class ResourceFile::ZLibTest : public ResourceFile::IDataFilter
	{
	public:

		enum { MEMORY_CHUNK = 16384 };	// Order of 128K bytes
		enum { LEVEL = 9 };				// 9 - Maximum but slowest compression/decompression. 

		ZLibTest( void );
		virtual ~ZLibTest( void );

		virtual BOOL IsPartialReadSupported( void ) { return FALSE; }

		virtual BOOL Encode( class IReader *pReader, class IWriter *pWriter );	// Deflate
		virtual BOOL Decode( class IReader *pReader, class IWriter *pWriter );	// Inflate

	private:

		BYTE *m_pIn; 
		BYTE *m_pOut;
		z_stream m_strm;

	};

	class ResourceFile::IReader
	{
	public:
		virtual ~IReader( void ) { }
		virtual SIZE_T Read( BYTE *pBuffer, SIZE_T size ) = 0;
		virtual BOOL IsDone( void ) = 0;
		virtual SIZE_T64 BytesLeft( void ) = 0;
	};

	class ResourceFile::IWriter
	{
	public:
		virtual ~IWriter( void ) { }
		virtual SIZE_T Write( BYTE *pBuffer, SIZE_T size ) = 0;
	};

	class ResourceFile::FileReader : public IReader
	{
	public:

		FileReader( void );
		virtual SIZE_T Read( BYTE *pBuffer, SIZE_T size );
		virtual BOOL IsDone( void );
		void SetFileStream( FileIO *pFile );
		void SetReadLimit( SIZE_T64 byteSize );
		virtual SIZE_T64 BytesLeft( void );

	private:

		SIZE_T64 m_fileSize;
		SIZE_T64 m_bytesRead;
		BOOL	m_bLimitRead;
		FileIO	*m_pFile;

	};

	class ResourceFile::MemoryReader : public IReader
	{
	public:

		virtual SIZE_T Read( BYTE *pBuffer, SIZE_T size );
		void SetBuffer( BYTE *pBuffer ) { m_pBuffer = pBuffer; }

		// TODO: Not implemented. Fix This!!!
		virtual SIZE_T64 BytesLeft( void ) { PSX_Assert( FALSE, "MemoryReader::BytesLeft() not implemented." ); return 0; } 

	private:

		BYTE	*m_pBuffer;
		// TODO: Implement methods to keep track of buffer; counter and size...
		//SIZE_T
	};

	class ResourceFile::FileWriter : public IWriter
	{
	public:

		virtual SIZE_T Write( BYTE *pBuffer, SIZE_T size ) { return m_pFile->Write( pBuffer, size ); }
		void SetFileStream( FileIO *pFile ) { m_pFile = pFile; }

	private:

		FileIO	*m_pFile;	
	};

	class ResourceFile::MemoryWriter : public IWriter
	{
	public:

		void SetBuffer( BYTE *pBuffer ) { m_pBuffer = pBuffer; m_curPos = 0; }
		virtual SIZE_T Write( BYTE *pBuffer, SIZE_T size ) { memcpy( m_pBuffer + m_curPos, pBuffer, size ); m_curPos += size; return size; }

	private:

		BYTE	*m_pBuffer;
		POS_T   m_curPos;
	};

/**************************************************************************************
///////////////////////////////// METHODS STARTS HERE /////////////////////////////////
**************************************************************************************/

	ResourceFile::FSHFile::FSHFile( ResourceFile *pFileSystem, ResourceFile::OHFile &ohFile )
		: m_pFileSystem( pFileSystem ), m_ohFile( ohFile ), m_localCurrPos( 0 )
	{

	}

	BOOL ResourceFile::FSHFile::IsValid( void )
	{
		return m_ohFile.IsValid();
	}

	BOOL ResourceFile::FSHFile::IsInvalid( void )
	{
		return m_ohFile.IsInvalid();
	}

	const CHAR * ResourceFile::FSHFile::GetName( void ) const
	{
		return ((DirFileEntry*)(m_ohFile.operator->()))->m_name.GetBuffer();
	}

	const SIZE_T ResourceFile::FSHFile::GetFileSize( void ) const
	{
		return m_pFileSystem->GetFileSize( &m_ohFile );
	}

	const SIZE_T64 ResourceFile::FSHFile::GetFileSize64( void ) const
	{
		return m_pFileSystem->GetFileSize64( &m_ohFile );
	}

	void ResourceFile::FSHFile::SetPos( UINT pos )
	{
		ResourceFile::DirFileEntry *pFileEntry = reinterpret_cast< ResourceFile::DirFileEntry * >( *m_ohFile );
		SIZE_T64 compressedSize = pFileEntry->m_PAKData.m_compressedSize;

		if ( pos > compressedSize )
			m_localCurrPos = (UINT)compressedSize;
		else
			m_localCurrPos = pos;
	}

	UINT ResourceFile::FSHFile::GetPos( void )
	{
		return m_localCurrPos;
	}

	BOOL ResourceFile::FSHFile::ReadFile( SIZE_T size, BYTE *pBuffer )
	{
		ResourceFile::DirFileEntry *pFileEntry = reinterpret_cast< ResourceFile::DirFileEntry * >( *m_ohFile );
		SIZE_T64 compressedSize = pFileEntry->m_PAKData.m_compressedSize;

		if ( m_pFileSystem->ReadFile( &m_ohFile, m_localCurrPos, size, pBuffer ) )
		{
			UINT newPos = m_localCurrPos + size;
			m_localCurrPos =  newPos < compressedSize ? newPos : (UINT)compressedSize;
			return TRUE;
		}

		return FALSE;
	}

	BOOL ResourceFile::FSHFile::ReadEntireFile( BYTE *pBuffer )
	{
		return m_pFileSystem->ReadFile( &m_ohFile, pBuffer );
	}

	void ResourceFile::FileHeader::WriteData( ResourceFile::IWriter *pWriter )
	{
		pWriter->Write( (BYTE*)this, sizeof( FileHeader ) );
	}
	
	void ResourceFile::FileHeader::ReadData( ResourceFile::IReader *pReader )
	{
		pReader->Read( (BYTE*)this, sizeof( FileHeader ) );
	}

	void ResourceFile::DirEntry::WriteData( ResourceFile::IWriter *pWriter )
	{
		// Write PAKData info
		pWriter->Write( reinterpret_cast< BYTE *>(&m_PAKData), sizeof( PAKData ) );
		// Then the remaining other info like strings and stuff
		pWriter->Write( (BYTE *)m_name.GetBuffer(), PSX_StrSize( m_name.GetBuffer() ) );
	}

	void ResourceFile::DirEntry::ReadData( ResourceFile::IReader *pReader )
	{
		CHAR tempStr[ PSX_MAX_PATH ];

		// Read PAKData info
		pReader->Read( reinterpret_cast< BYTE *>(&m_PAKData), sizeof( PAKData ) );
		// Then remaining other info like strings
		pReader->Read( reinterpret_cast< BYTE *>(tempStr), m_PAKData.m_nameLen * sizeof( CHAR ) );
		tempStr[ m_PAKData.m_nameLen ] = PSX_String( '\0' );

		m_name = tempStr;
	}

	void ResourceFile::DirFileEntry::WriteData( ResourceFile::IWriter *pWriter )
	{
		pWriter->Write( reinterpret_cast< BYTE * >(&m_PAKData), sizeof( PAKData ) );
		pWriter->Write( (BYTE*)m_name.GetBuffer(), PSX_StrSize( m_name.GetBuffer() ) );
	}

	void ResourceFile::DirFileEntry::ReadData( ResourceFile::IReader *pReader )
	{
		CHAR tempStr[ PSX_MAX_PATH ];

		// Read PAKData info
		pReader->Read( reinterpret_cast< BYTE *>(&m_PAKData), sizeof( PAKData ) );
		// Then remaining other info like strings
		pReader->Read( reinterpret_cast< BYTE *>(tempStr), m_PAKData.m_nameLen * sizeof( CHAR ) );
		tempStr[ m_PAKData.m_nameLen ] = PSX_String( '\0' );

		m_name = tempStr;
	}

	BOOL ResourceFile::DataFilter::Encode( ResourceFile::IReader *pReader, ResourceFile::IWriter *pWriter )
	{
		PSX_Assert( pReader && pWriter, "Parameter is NULL." );

		if ( !pReader || !pWriter )
			return FALSE;

		const SIZE_T	bufferSize		= 16384;
		SIZE_T			readWriteSize	= bufferSize;
		BYTE			buffer[ bufferSize ];

		while( !pReader->IsDone() )
		{
			readWriteSize = readWriteSize < pReader->BytesLeft() ? bufferSize : (SIZE_T)pReader->BytesLeft();
			
			pReader->Read( buffer, readWriteSize );
			pWriter->Write( buffer, readWriteSize );
		}

		return TRUE;
	}

	BOOL ResourceFile::DataFilter::Decode( ResourceFile::IReader *pReader, ResourceFile::IWriter *pWriter )
	{
		PSX_Assert( pReader && pWriter, "Parameter is NULL." );
		
		if ( !pReader || !pWriter )
			return FALSE;

		const SIZE_T	bufferSize		= 16384;
		SIZE_T			readWriteSize	= bufferSize;
		BYTE			buffer[ bufferSize ];

		while( !pReader->IsDone() )
		{
			readWriteSize = readWriteSize < pReader->BytesLeft() ? bufferSize : (SIZE_T)pReader->BytesLeft();
			
			pReader->Read( buffer, readWriteSize );
			pWriter->Write( buffer, readWriteSize );
		}

		return TRUE;
	}

	ResourceFile::ZLibTest::ZLibTest( void )
	{
		m_pIn  = new BYTE[ MEMORY_CHUNK ];
		m_pOut = new BYTE[ MEMORY_CHUNK ];

		PSX_Assert( m_pIn || m_pOut, "Out of memory." );
	}

	ResourceFile::ZLibTest::~ZLibTest( void )
	{
		delete [] m_pOut;
		delete [] m_pIn;
	}

	BOOL ResourceFile::ZLibTest::Encode( ResourceFile::IReader *pReader, ResourceFile::IWriter *pWriter )
	{
		INT ret, flush;
		unsigned have;

		// Allocate deflate state
		m_strm.zalloc = MemoryManager::zlibAlloc;
		m_strm.zfree  = MemoryManager::zlibFree;
		m_strm.opaque = Z_NULL;

		ret = deflateInit( &m_strm, LEVEL );

		if ( ret != Z_OK )
		{
			PSX_Assert( FALSE, "Failed to initialize ZlibTest." );
			goto zlibEncodeFail;
		}

		// Compress
		do
		{
			m_strm.avail_in = pReader->Read( m_pIn, MEMORY_CHUNK );
			// TODO: error check here...

			flush = pReader->IsDone() ? Z_FINISH : Z_NO_FLUSH;
			m_strm.next_in = m_pIn;

			// Run deflate on input until output buffer not full, 
			// finish compression if all of source has been read in
			do
			{
				m_strm.avail_out = MEMORY_CHUNK;
				m_strm.next_out = m_pOut;
				ret = deflate( &m_strm, flush );	// no bad return value
				PSX_Assert( ret != Z_STREAM_ERROR, "Error executing deflate compression." );
				have = MEMORY_CHUNK - m_strm.avail_out;
				if ( pWriter->Write( m_pOut, have ) != have /* || pWriter->IsError()(not implemented) */ )
					goto zlibEncodeFail;

			} while ( m_strm.avail_out == 0 );
			if ( m_strm.avail_in != 0 )
			{
				PSX_Assert( FALSE, "Compression failed." ); // All input should be used
				goto zlibEncodeFail;
			}

			// Done when the last data in file is processed
		} while ( flush != Z_FINISH );
		if ( ret != Z_STREAM_END )
		{
			PSX_Assert( FALSE, "Error executing deflate compression." );	// Steam should be complete
			goto zlibEncodeFail;
		}

		deflateEnd( &m_strm );
		return TRUE;

	zlibEncodeFail:

		deflateEnd( &m_strm );
		return FALSE;
	}

	BOOL ResourceFile::ZLibTest::Decode( class IReader *pReader, class IWriter *pWriter )	// Inflate
	{
		INT  ret;
		UINT have;

		m_strm.zalloc = MemoryManager::zlibAlloc;
		m_strm.zfree  = MemoryManager::zlibFree;
		m_strm.opaque = Z_NULL;
		m_strm.avail_in = 0;
		m_strm.next_in = Z_NULL;

		ret = inflateInit( &m_strm );
		if ( ret != Z_OK )
		{
			PSX_Assert( FALSE, "Failed to initialize ZlibTest." );
			goto zlibDecodeFail;
		}
		// decompress until stream is done or EOF
		do
		{
			m_strm.avail_in = pReader->Read( m_pIn, MEMORY_CHUNK );
			// TODO: Do error check here
			if ( m_strm.avail_in == 0 )
				break;

			m_strm.next_in = m_pIn;

			// Run inflate() on input until output buffer not full
			do
			{
				m_strm.avail_out = MEMORY_CHUNK;
				m_strm.next_out = m_pOut;

				ret = inflate( &m_strm, Z_NO_FLUSH );
				PSX_Assert( ret != Z_STREAM_ERROR, "Error executing inflate()." );
				
				switch( ret ) 
				{
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;	// And fall through
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					goto zlibDecodeFail;
				}

				have = MEMORY_CHUNK - m_strm.avail_out;

				if ( pWriter->Write( m_pOut, have ) != have /* || pWriter->IsError() */ )
					goto zlibDecodeFail;

			} while ( m_strm.avail_out == 0 );

		} while ( ret != Z_STREAM_END );

		inflateEnd( &m_strm );
		return ret == Z_STREAM_END ? TRUE : FALSE;

	zlibDecodeFail:

		inflateEnd( &m_strm );
		return FALSE;
	}

	ResourceFile::FileReader::FileReader( void ) 
		: m_fileSize( 0 ), m_bytesRead( 0 ), m_bLimitRead( FALSE ), m_pFile( 0 ) 
	{

	}
	
	SIZE_T ResourceFile::FileReader::Read( BYTE *pBuffer, SIZE_T size ) 
	{
		if ( m_bLimitRead && m_bytesRead >= m_fileSize )
			return 0;

		// Fix size if it is over the limit
		if ( m_bLimitRead && size > (m_fileSize - m_bytesRead) )
			size = static_cast<SIZE_T>(m_fileSize - m_bytesRead);

		m_bytesRead += size;
		return m_pFile->Read( pBuffer, size ); 
	}
	
	BOOL ResourceFile::FileReader::IsDone( void )
	{
		if ( m_bLimitRead && m_bytesRead >= m_fileSize )
			return TRUE;
		
		return m_pFile->IsEOF(); 
	}

	void ResourceFile::FileReader::SetFileStream( FileIO *pFile ) 
	{
		m_pFile = pFile; 
		m_bLimitRead = FALSE; 
	}
	
	void ResourceFile::FileReader::SetReadLimit( SIZE_T64 byteSize )
	{
		m_fileSize = byteSize;
		m_bytesRead = 0;
		m_bLimitRead = TRUE;
	}

	SIZE_T64 ResourceFile::FileReader::BytesLeft( void ) 
	{
		return  m_bLimitRead ? m_fileSize - m_bytesRead : 0;
	}

	ResourceFile::ResourceFile( void )
		:	m_pHeader( NULL ), m_pGenDirs( NULL ), m_pGenFiles( NULL ), 
		m_pGenFileList( NULL ), m_pFileMap( NULL ), m_pPulseFile( NULL ),
		m_bLoaded( FALSE )
	{

	}

	ResourceFile::~ResourceFile( void )
	{
		ReleaseResources();
	}

	BOOL ResourceFile::Open( const CHAR *pFileName, BOOL deleteTableEntries /*= TRUE */ )
	{
		DWORD	currPathIndex;
		String	stringID;
		BOOL	bAddSeparator;
		PAKGenDirList::Iterator		dirIter;
		PAKGenDirList::Iterator		dirIterEnd;
		PAKGenFileList::Iterator 	fileIter;
		PAKGenFileList::Iterator 	fileIterEnd;

		ReleaseResources();

		m_pHeader		= new FileHeader;
		m_pGenDirs		= new PAKGenDirList;
		m_pGenFileList	= new PAKGenFileList;
		m_pFileMap		= new FileEntryMap;
		m_pPulseFile	= new FileIO;

		if ( !(m_pHeader && m_pGenDirs && m_pGenFileList) )
		{
			PSX_Assert( FALSE, "Failed to allocate memory." );
			ReleaseResources();
			return FALSE;
		}

		if ( !ReadPAKInfo( pFileName, m_pHeader, m_pGenDirs, m_pGenFileList ) )
			goto FailLoad;

		if ( !InitializeFilters( m_pHeader->m_filterBitField ) )
			goto FailLoad;

		// Generate File Map info
		currPathIndex = 0;
		dirIter		= m_pGenDirs->IteratorBegin();
		dirIterEnd	= m_pGenDirs->IteratorEnd();
		fileIter	= m_pGenFileList->IteratorBegin();
		fileIterEnd	= m_pGenFileList->IteratorEnd();

		// Root files don't have directories prefixed
		bAddSeparator = FALSE;

		while ( fileIter != fileIterEnd )
		{
			if ( currPathIndex != (*fileIter)->m_PAKData.m_pathIndex )
			{
				++currPathIndex;
				++dirIter;

				// Error check
				if ( dirIter == dirIterEnd )
				{
					PSX_Assert( FALSE, "Error Pulse File." );
					ReleaseResources();
					return FALSE;
				}

				bAddSeparator = (*dirIter)->m_name.GetLength() ? TRUE : FALSE;
			}

			// Concatenate string then make this as an ID
			if ( bAddSeparator )
				stringID = (*dirIter)->m_name + PSX_String( "/" ) + (*fileIter)->m_name;
			else
				stringID = (*fileIter)->m_name;
			
			m_pFileMap->Insert( FileMapPair( stringID, *fileIter ) );

			++fileIter;
		}

		if ( !m_pPulseFile->Open( pFileName, FileIO::FILEOP_READ | FileIO::FILEOP_BINARY ) )
			goto FailLoad;

		m_bLoaded = TRUE;

		// We can save memory by releasing table entries
		if ( deleteTableEntries )
		{
			//PSX_SafeDelete( m_pHeader );
			PSX_SafeDelete( m_pGenDirs );
			PSX_SafeDelete( m_pGenFileList );
		}

		return TRUE;

	FailLoad:

		ReleaseResources();

		return FALSE;
	}

	void ResourceFile::Close( void )
	{
		ReleaseResources();
	}

	BOOL ResourceFile::IsOpen( void ) const
	{
		return m_bLoaded;
	}

	ResourceFile::FSHFile ResourceFile::FindFile( const CHAR *pPath ) const
	{
		FileEntryMap::Iterator iter = m_pFileMap->Find( pPath );
		
		if ( iter == m_pFileMap->IteratorEnd() )
			return FSHFile( const_cast<ResourceFile*>(this), OHFile() );
		
		return FSHFile( const_cast<ResourceFile*>(this), OHFile(reinterpret_cast<PTR_T>((&(*(*iter).second)))) );
	}

	SIZE_T ResourceFile::GetFileSize( const OHFile *pOHFile ) const
	{
		SIZE_T64 size = GetFileSize64( pOHFile );

		if ( size > PSX_SIZE_T_MAX )
			return 0; // Call GetFileSize64()

		return static_cast< SIZE_T >( size );
	}

	SIZE_T64 ResourceFile::GetFileSize64( const OHFile *pOHFile ) const
	{
		if ( !m_bLoaded || pOHFile->IsInvalid() )
			return 0;

		DirFileEntry *pFileEntry = reinterpret_cast< DirFileEntry * >( **pOHFile );
		return (*pFileEntry).m_PAKData.m_size;
	}

	BOOL ResourceFile::ReadFile( const OHFile *pOHFile, BYTE *pBuff )
	{
		if ( !IsOpen() || pOHFile->IsInvalid() )
			return FALSE;

		// Just use pointer to avoid additional processing by SmartPointer<>
		FileReader reader;
		MemoryWriter writer;
		DirFileEntry *pFileEntry =  ((DirFileEntry *)(**pOHFile));

		// NOTE: Enter critical section here
		m_pPulseFile->Seek64( pFileEntry->m_PAKData.m_diskStart );
		reader.SetFileStream( m_pPulseFile );
		reader.SetReadLimit( pFileEntry->m_PAKData.m_compressedSize );
		writer.SetBuffer( pBuff );
		
		Decode( pFileEntry, &reader, &writer );
		// NOTE: Leave critical section

		return TRUE;
	}

	// NOTE: Partial read version of ReadFile
	BOOL ResourceFile::ReadFile( const OHFile *pOHFile, const UINT pos, const SIZE_T size, BYTE *pBuffer )
	{
		if ( !IsOpen() || pOHFile->IsInvalid() )
			return FALSE;

		FileReader reader;
		MemoryWriter writer;
		DirFileEntry *pFileEntry = ((DirFileEntry *)(**pOHFile));
		SIZE_T64 compressedSize = pFileEntry->m_PAKData.m_compressedSize;
		FILTER_TYPE filterBit = static_cast<FILTER_TYPE>(pFileEntry->m_PAKData.m_filterBit);

		// Check if the used filter supports partial read
		if ( !m_filters[filterBit]->IsPartialReadSupported() )
			return FALSE;
		
		// Read position check and remaining read size check
		if ( pos >= compressedSize || size > compressedSize - pos )
			return FALSE;

		// Just use pointer to avoid additional processing by SmartPointer<>

		// NOTE: Enter critical section here
		m_pPulseFile->Seek64( pFileEntry->m_PAKData.m_diskStart + pos );
		reader.SetFileStream( m_pPulseFile );
		reader.SetReadLimit( size );
		writer.SetBuffer( pBuffer );

		Decode( pFileEntry, &reader, &writer );
		// NOTE: Leave critical section

		return TRUE;
	}

	BOOL ResourceFile::Create( const CHAR *pDirectory, const CHAR *pOutputPath, OnProcessFileCallback pOnProcessFileCallback /* = NULL */, FilterFlag fBitFlag /*= FILTER_TYPE_DEFAULT*/ )
	{
		String	directory	= pDirectory;
		BOOL	bReturn		= TRUE;

		// Make sure pDirectory isn't empty and the path exists
		if ( !pDirectory || !System::IsDirectoryExist( pDirectory ) )
			return FALSE;

		ReleaseResources();

		InitializeFilters( fBitFlag );

		m_pOnProcessFile = pOnProcessFileCallback;

		m_pHeader	= new FileHeader;
		m_pGenDirs	= new PAKGenDirList;
		m_pGenFiles	= new PAKGenFilePairList;

		PSX_Assert( m_pHeader && m_pGenDirs && m_pGenFiles, "Failed to allocate memory." );

		// Setup header information
		m_pHeader->m_signature = ResourceFile::SIGNATURE;
		PSX_MemCopyPerByte( m_pHeader->m_ID, PSX_String("pfs"), 4 );
		m_pHeader->m_version = 0x000100; // TODO:  Create macro to generate pfs version
		m_pHeader->m_filterBitField = fBitFlag;

		// Get the relative root directory ( choosen directly )
		INT len = PSX_StrLen( directory.GetBuffer() );
		CHAR *pPtr = const_cast< CHAR*>( directory.GetBuffer() + len );
		CHAR *pRootSeparator = const_cast< CHAR*>(directory.GetBuffer() + 2); // get the first separator( i.e c:\ )
		String pPAKPath;

		// Look for a seperator that's not the root
		while ( pPtr != pRootSeparator && *pPtr != PSX_String('\\') )
			--pPtr;

		// If not the root separator then we need to remove the last separator in the string.
		if ( pPtr != pRootSeparator && PSX_StrLen( pPtr + 1 ) == 0 )
		{
			directory[ pPtr - directory.GetBuffer() ] = PSX_String('\0');

			// Look again for the next seperator that's not the root
			while ( pPtr != pRootSeparator && *pPtr != PSX_String('\\') )
				--pPtr;
		}
		
		// Move one char forward starting either to the first char of dir name or null(empty).
		++pPtr;

		// There could be no directory.
		// Then pPAKPath is empty
		if ( PSX_StrLen( pPtr ) )
			pPAKPath = pPAKPath + pPtr;

		// Generate pack folder and file entries then create the Pulse pack File
		if ( !_GenerateTableEntries( directory.GetBuffer(), pPAKPath.GetBuffer() ) )
		{
			bReturn = FALSE;
			goto EndCreatePAK;
		}

		if ( !_CreatePulseFile( pOutputPath ) )
		{
			bReturn = FALSE;
			goto EndCreatePAK;
		}

	EndCreatePAK:
		ReleaseFilters();
		PSX_SafeDelete( m_pGenFiles );
		PSX_SafeDelete( m_pGenDirs );
		PSX_SafeDelete( m_pHeader );

		return bReturn;
	}

	BOOL ResourceFile::Unpack( const CHAR *pPAKPath, const CHAR *pOutputPath )
	{
		FileIO						pulseFile;
		FileIO						createFile;
		FileReader					fileReader;
		FileWriter					fileWriter;
		BOOL						bReturn = TRUE;
		BOOL						bAddSeparator = TRUE;
		PAKGenDirList::Iterator		dirIter;
		PAKGenDirList::Iterator		dirIterEnd;
		PAKGenFileList::Iterator 	fileIter;
		PAKGenFileList::Iterator 	fileIterEnd;
		CHAR						catPath[PSX_MAX_PATH];
		DWORD						currPathIndex;
		FilterPointer				pFilter;

		ReleaseResources();

		if ( *(pOutputPath + PSX_StrLen( pOutputPath ) - 1) == PSX_String( '\\' ) )
			bAddSeparator = FALSE;

		// Allocate memory to store pack information
		m_pHeader		= new FileHeader;
		m_pGenDirs		= new PAKGenDirList;
		m_pGenFileList	= new PAKGenFileList;

		PSX_Assert( m_pHeader && m_pGenDirs && m_pGenFileList, "Failed to allocate memory." );

		// Read header and entries and automatically verify header.
		if ( !ReadPAKInfo( pPAKPath, m_pHeader, m_pGenDirs, m_pGenFileList ) )
		{
			bReturn = FALSE;
			goto UnpackPAKEnd;
		}	

		pulseFile.Open( pPAKPath, FileIO::FILEOP_READ | FileIO::FILEOP_BINARY );
		if ( pulseFile.IsOpen() == FALSE )
		{
			bReturn = FALSE;
			goto UnpackPAKEnd;
		}

		if ( !InitializeFilters( m_pHeader->m_filterBitField ) )
		{
			bReturn = FALSE;
			goto UnpackPAKEnd;
		}
		
		// Make sure folder save path exists
		if ( !System::CreateDirectory( pOutputPath ) )
		{
			bReturn = FALSE;
			goto UnpackPAKEnd;
		}

		// We're ready to create folders and files
		fileReader.SetFileStream( &pulseFile );

		// Create folders
		dirIter = m_pGenDirs->IteratorBegin();
		dirIterEnd = m_pGenDirs->IteratorEnd();

		while ( dirIter != dirIterEnd )
		{
			PSX_StrCpy( catPath, pOutputPath, PSX_MAX_PATH );

			if ( bAddSeparator )
				PSX_StrCat( catPath, PSX_String("/") );

			PSX_StrCat( catPath, (*dirIter)->m_name.GetBuffer() );

			System::CreateDirectory( catPath );

			++dirIter;
		}

		// Start creating files. This is a little tricky to implement...
		currPathIndex = 0;
		fileReader.SetFileStream( &pulseFile );
		dirIter = m_pGenDirs->IteratorBegin();
		fileIter = m_pGenFileList->IteratorBegin();
		fileIterEnd = m_pGenFileList->IteratorEnd();

		while ( fileIter != fileIterEnd )
		{
			// currPathIndex tells us on what directory we're creating files in
			while ( currPathIndex != (*fileIter)->m_PAKData.m_pathIndex )
			{
				++currPathIndex;
				++dirIter;

				// Error check
				PSX_Assert( !(dirIter == dirIterEnd), "Error Pulse File." );
			}

			PSX_StrCpy( catPath, pOutputPath, PSX_MAX_PATH );

			if ( bAddSeparator )
				PSX_StrCat( catPath, PSX_String("/") );
				
			PSX_StrCat( catPath, (*dirIter)->m_name.GetBuffer() );

			// Be aware that a dirEntry could contain an empty string...
			if ( (*dirIter)->m_name.GetLength() )
				PSX_StrCat( catPath, PSX_String("/") );

			// Then filename
			PSX_StrCat( catPath, (*fileIter)->m_name.GetBuffer() );

			createFile.Open( catPath, FileIO::FILEOP_WRITE | FileIO::FILEOP_BINARY );

			PSX_Assert( createFile.IsOpen(), "Failed to open file." );

			// Setup reader, writer and set seek to read file data
			pulseFile.Seek64( (*fileIter)->m_PAKData.m_diskStart );
			fileReader.SetReadLimit( (*fileIter)->m_PAKData.m_size );
			fileWriter.SetFileStream( &createFile );

			// Finally write data with the choosen filter.
			Decode( &(**fileIter), &fileReader, &fileWriter );

			createFile.Close();
			++fileIter;
		}

	UnpackPAKEnd:
		pulseFile.Close();
		ReleaseResources();

		return bReturn;
	}

	BOOL ResourceFile::InitializeFilters( FilterFlag fFlag )
	{
		ReleaseFilters();

		// Default filter always exists
		FilterPointer defaultFilter = new DataFilter;
		m_filters.Insert( FilterPair( FILTER_TYPE_DEFAULT, defaultFilter ) );
		
		// Custom filters starts here
		if ( fFlag & FILTER_TYPE_ZLIB_TEST )
			m_filters.Insert( FilterPair( FILTER_TYPE_ZLIB_TEST, new ZLibTest ) );
				
		return TRUE;
	}

	void ResourceFile::ReleaseFilters( void )
	{
		m_filters.Clear();
	}

	BOOL ResourceFile::_GenerateTableEntries( const CHAR *pFolderPath, const CHAR *pPAKPath )
	{
		// Iterate through each file in the folder
		DirEntryPointer pNewDir;
		WIN32_FIND_DATA nextFile;
		HANDLE			hFind;
		DirList			tempFolderPaths;	// Temporarily store found folders here	
		String			dirBuff;
		String			newDirPAKPath;
		String			newDirFolderPath;
		DirPathPointer	fileDirPath;		// File entries needs this when we're about to parse each file later
		BOOL			bAddSeparator = TRUE;
		BOOL			bReturn = TRUE;
		DirPathList::Iterator iter;	
		DirPathList::Iterator iterEnd;


		fileDirPath =  new DirPath;
		*fileDirPath = pFolderPath;

		if ( *(pFolderPath + PSX_StrLen( pFolderPath ) - 1) == PSX_String( '\\' ) )
			bAddSeparator = FALSE;

		// Insert new directory
		pNewDir = new DirEntry;

		if ( pPAKPath && PSX_StrLen( pPAKPath ) )
		{
			pNewDir->m_PAKData.m_nameLen = PSX_StrLen( pPAKPath );
			pNewDir->m_name				 = pPAKPath;
		}
		else
		{
			// Insert anyway since we're now using path index for file entries.
			pNewDir->m_PAKData.m_nameLen = 0;
			pNewDir->m_name				 = PSX_String("");
		}

		// Put in the list then Increment num directories
		m_pGenDirs->PushBack( pNewDir );
		++m_pHeader->m_numDirs;

		// Prepare string path to allow for file searching
		dirBuff = String(pFolderPath) + PSX_String("/*");
		PSX_ZeroMem( &nextFile, sizeof( WIN32_FIND_DATA ) );
		hFind = FindFirstFile( dirBuff.GetBuffer(), &nextFile );

		if ( hFind == INVALID_HANDLE_VALUE )
			return FALSE;
		
		do
		{
			// Don't include the following attributes and directories
			if ( PSX_StrCmp( nextFile.cFileName, PSX_String(".") ) == 0 ||
				PSX_StrCmp( nextFile.cFileName, PSX_String("..") ) == 0 ||
				(nextFile.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)		||
				(nextFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) )
				continue;

			// If a directory...
			if ( nextFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// Store Folder name and traverse them later
				DirPointer newFolder = new Directory;

				*newFolder = nextFile.cFileName;
				tempFolderPaths.PushBack( newFolder );
			}
			else // If a file...
			{
				// New File entry
				FileEntryPointer	pNewFileEntry	= new DirFileEntry;
				DirPathPointer		pnewFileDirPath	= new DirPath;

				// Copy File entry info
				pNewFileEntry->m_PAKData.m_size		 = nextFile.nFileSizeLow;
				pNewFileEntry->m_PAKData.m_size		 += nextFile.nFileSizeHigh; // Zero if not greater than DWORD
				pNewFileEntry->m_name				 = nextFile.cFileName;
				pNewFileEntry->m_PAKData.m_nameLen	 = pNewFileEntry->m_name.GetLength();
				pNewFileEntry->m_PAKData.m_pathIndex = m_pHeader->m_numDirs - 1; // zero based index

				// Store pointer to the file's directory path
				pnewFileDirPath = fileDirPath;

				// Finally insert in the list
				m_pGenFiles->PushBack( FileEntryPair( pNewFileEntry, pnewFileDirPath ) );

				// Update header information
				++m_pHeader->m_numFiles;
			}

		} while ( FindNextFile( hFind, &nextFile ) != 0 );

		// If not ERROR_NO_MORE_FILES then something bad happened.
		if ( GetLastError() != ERROR_NO_MORE_FILES )
		{
			FindClose( hFind );
			bReturn = FALSE;
			goto EndGenerateTable;
		}

		// This find handle isn't needed anymore.
		FindClose( hFind );

		// Now traverse through each of the folders
		iter	= tempFolderPaths.IteratorBegin();
		iterEnd	= tempFolderPaths.IteratorEnd();

		while ( iter != iterEnd )
		{
			// Prepare the included directory
			// Don't add separator if there's already one at the end.
			if ( bAddSeparator )
			{
				newDirFolderPath	= String( pFolderPath ) + PSX_String("/") + (*iter)->GetBuffer();
				newDirPAKPath		= String( pPAKPath ) + PSX_String("/") + (*iter)->GetBuffer();
			}
			else
			{
				newDirFolderPath	= String( pFolderPath ) + (*iter)->GetBuffer();
				newDirPAKPath		= String( pPAKPath ) + (*iter)->GetBuffer();
			}

			if ( !_GenerateTableEntries( newDirFolderPath.GetBuffer(), newDirPAKPath.GetBuffer() ) )
			{
				bReturn = FALSE;
				goto EndGenerateTable;
			}

			++iter;
		}

	EndGenerateTable:
		tempFolderPaths.Clear();

		return bReturn;
	}

	BOOL ResourceFile::ReadPAKInfo( const CHAR *pPAKFilePath, FileHeader *pPAKHeader, PAKGenDirList *pPAKDirList, PAKGenFileList *pPAKFileList )
	{
		FileIO				pakFile;
		DirEntryPointer		pNewDir;
		FileEntryPointer	pNewFile;
		FileReader			fileReader;

		pakFile.Open( pPAKFilePath, FileIO::FILEOP_READ | FileIO::FILEOP_BINARY );
		if ( pakFile.IsOpen() == FALSE )
			return FALSE;

		// Read Pulse File Header first then table entries
		pakFile.Seek( 0 - sizeof( FileHeader ), FileIO::SEEKOP_END );
		fileReader.SetFileStream( &pakFile );
		pPAKHeader->ReadData( &fileReader );

		// Do verification checks
		if ( !VerifyPulseHeader( pPAKHeader ) )
			return FALSE;

		// Read directory entries
		pakFile.Seek64( pPAKHeader->m_dirDiskStart, FileIO::SEEKOP_BEGIN );
		for ( SIZE_T i = 0; i < pPAKHeader->m_numDirs; ++i )
		{
			pNewDir = new DirEntry;
			pNewDir->ReadData( &fileReader );
			pPAKDirList->PushBack( pNewDir );
		}

		// Read file entries
		pakFile.Seek64( pPAKHeader->m_fileDiskStart, FileIO::SEEKOP_BEGIN );
		for ( SIZE_T i = 0; i < m_pHeader->m_numFiles; ++i )
		{
			pNewFile = new DirFileEntry;
			pNewFile->ReadData( &fileReader );
			pPAKFileList->PushBack( pNewFile );
		}

		pakFile.Close();
		return TRUE;
	}

	BOOL ResourceFile::_CreatePulseFile( const CHAR *pOutputPath )
	{
		FileIO				fFileRead;			// File Stream for reading files
		FileIO				fPAKWrite;			// File Stream for writing to the Pulse pack File
		FileReader			fileReader;			// Used for the filters
		FileWriter			fileWriter;			// Used for the filters
		FileEntryPointer	pFileEntry;
		DirPathPointer		pFileDirPath;
		String				filePath;			// Will contain the path of a file to be read in the disk
		POS_T64				lastWritePos;
		PAKGenFilePairList::Iterator	fileIter;
		PAKGenFilePairList::Iterator	fileIterEnd;
		PAKGenDirList::Iterator			dirIter;
		PAKGenDirList::Iterator			dirIterEnd;

		// Check to make sure the directories exists
		CHAR *pFolderCheck = new CHAR[PSX_MAX_PATH];
		CHAR *pPtr = const_cast< CHAR*>( pOutputPath );

		while ( pPtr = PSX_StrChr( pPtr, PSX_String( '\\' ) ) )
		{
			++pPtr;
			PSX_StrCpy( pFolderCheck , pOutputPath, pPtr - pOutputPath );
			pFolderCheck [ pPtr - pOutputPath ] = NULL;

			// Create if it doesn't exist
			if ( !System::IsDirectoryExist( pFolderCheck ) )
				CreateDirectory( pFolderCheck, NULL );
		}
		
		PSX_SafeDelete( pFolderCheck );

		// Prepare to create the Pulse pack File
		fPAKWrite.Open( pOutputPath, FileIO::FILEOP_WRITE | FileIO::FILEOP_BINARY | FileIO::FILEOP_TRUNCATE );
		if ( !fPAKWrite.IsOpen() )
			return FALSE;

		fileWriter.SetFileStream( &fPAKWrite );
		
		lastWritePos	= 0;
		fileIter		= m_pGenFiles->IteratorBegin();
		fileIterEnd		= m_pGenFiles->IteratorEnd();

		// Write file data
		while ( fileIter != fileIterEnd )
		{
			pFileEntry		= (*fileIter).first;
			pFileDirPath	= (*fileIter).second;
			filePath		= *pFileDirPath + PSX_String("/") + pFileEntry->m_name.GetBuffer();
			
			// Open file for reading
			fFileRead.Open( filePath.GetBuffer(), FileIO::FILEOP_READ | FileIO::FILEOP_BINARY );
			PSX_Assert( fFileRead.IsOpen(), "Failed to open a file." );

			fileReader.SetFileStream( &fFileRead );
			fileReader.SetReadLimit( pFileEntry->m_PAKData.m_size );

			if ( m_pOnProcessFile )
				pFileEntry->m_PAKData.m_filterBit = m_pOnProcessFile( pFileEntry->m_name.GetBuffer(), pFileDirPath->GetBuffer() );

			Encode( &(*pFileEntry), &fileReader, &fileWriter );

			fFileRead.Close();

			// Update file entry info and other bookeeping vars
			pFileEntry->m_PAKData.m_diskStart		= lastWritePos;
			pFileEntry->m_PAKData.m_compressedSize	= fPAKWrite.Tell64() - lastWritePos;
			
			lastWritePos = fPAKWrite.Tell64();
			
			++fileIter;
		}

		// Write folder entry table
		m_pHeader->m_dirDiskStart = fPAKWrite.Tell64();
		dirIter = m_pGenDirs->IteratorBegin();
		dirIterEnd = m_pGenDirs->IteratorEnd();
		
		while ( dirIter != dirIterEnd )
		{
			(*dirIter)->WriteData( &fileWriter );
			++dirIter;
		}

		// Write file entry table
		m_pHeader->m_fileDiskStart = fPAKWrite.Tell64();
		fileIter		= m_pGenFiles->IteratorBegin();
		fileIterEnd		= m_pGenFiles->IteratorEnd();

		while ( fileIter != fileIterEnd )
		{
			(*fileIter).first->WriteData( &fileWriter );
			++fileIter;
		}

		// Then finally the Pulse File header
		m_pHeader->WriteData( &fileWriter );

		fPAKWrite.Close();

		return TRUE;
	}

	BOOL ResourceFile::VerifyPulseHeader( FileHeader *pHeader )
	{
		return VerifyHeaderSignature( &pHeader->m_signature ) && VerifyHeaderFormat( pHeader->m_ID );
	}

	BOOL ResourceFile::VerifyHeaderSignature( const Signature *pSig )
	{
		return pSig ? *pSig == SIGNATURE : FALSE;
	}

	BOOL ResourceFile::VerifyHeaderFormat( const BYTE *pFormat )
	{
		return PSX_MemComparePerByte( pFormat, PSX_String( "pfs" ), 4 ) == 0;
	}

	PSX_INLINE void ResourceFile::Encode( DirFileEntry *pFileEntry, IReader *pReader, IWriter *pWriter )
	{
		m_filters[ static_cast<FILTER_TYPE>(pFileEntry->m_PAKData.m_filterBit) ]->Encode( pReader, pWriter );
	}

	PSX_INLINE void ResourceFile::Decode( DirFileEntry *pFileEntry, IReader *pReader, IWriter *pWriter )
	{
		m_filters[ static_cast<FILTER_TYPE>(pFileEntry->m_PAKData.m_filterBit) ]->Decode( pReader, pWriter );
	}

	void ResourceFile::ReleaseResources( void )
	{
		m_bLoaded = FALSE;
		ReleaseFilters();
		PSX_SafeDelete( m_pPulseFile );
		PSX_SafeDelete( m_pFileMap );
		PSX_SafeDelete( m_pGenFileList );
		PSX_SafeDelete( m_pGenFiles );
		PSX_SafeDelete( m_pGenDirs );
		PSX_SafeDelete( m_pHeader );
	}

}