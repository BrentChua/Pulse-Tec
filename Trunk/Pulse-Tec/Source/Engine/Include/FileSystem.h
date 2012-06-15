/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	FileSystem.h
*
*	Description -	Singleton ResourceFile manager class.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	08/05/2010	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_FILE_SYSTEM_MANAGER_H_
#define _PSX_FILE_SYSTEM_MANAGER_H_

#include "PulseSTD.h"
#include "ResourceFile.h"
#include "FileIO.h"
#include "Singleton.h"
#include "Array.h"
#include "SmartPointer.h"
#include "String.h"

namespace Pulse
{

	PSX_EnumBegin( FileType )
		DEFAULT,
		MESH,
		TEXTURE,
		SOUND,
		VIDEO,
	PSX_EnumEnd()

	class FileSystem : public Singleton< FileSystem >
	{
	public:

		friend Singleton;

		// This is an interface class to handle both versions of file handles coming from either
		//	the file system or an external file
		class FileHandle
		{
		public:

			virtual ~FileHandle( void ) { }

			virtual BOOL IsValid( void ) = 0;

			virtual const CHAR * GetName( void ) const = 0;

			virtual SIZE_T		GetFileSize( void ) = 0;
			virtual SIZE_T64	GetFileSize64( void ) = 0;

			virtual BOOL ReadEntireFile( BYTE *pBuffer ) = 0;
		};

	private:

		// Internal FileHandle 
		class FSHFileHandle : public FileHandle
		{
		public: 

			FSHFileHandle( ResourceFile::FSHFile fsFile );

			virtual ~FSHFileHandle( void );

			virtual BOOL IsValid( void );

			virtual const CHAR * GetName( void ) const;

			virtual SIZE_T GetFileSize( void );
			virtual SIZE_T64 GetFileSize64( void );

			virtual BOOL ReadEntireFile( BYTE *pBuffer );

		private:

			ResourceFile::FSHFile m_fsFile;

		};

		class FileFileHandle : public FileHandle
		{
		public:

			FileFileHandle( const CHAR *pFilename );
			virtual ~FileFileHandle( void );

			virtual BOOL IsValid( void );

			virtual const CHAR * GetName( void ) const;

			virtual SIZE_T GetFileSize( void );
			virtual SIZE_T64 GetFileSize64( void ); // Same as GetFileSize()

			virtual BOOL ReadEntireFile( BYTE *pBuffer );

		private:

			FileIO	m_fileIO;
			SIZE_T	m_fileSize;
			String	m_name;
		};

	public:

		typedef SmartPointer< FileHandle > FileHandlePtr;

	public:

		// File System public interface

		EErrorCode::Type AddResourceFile( const CHAR *pFilename );

		void AddDirectoryPath( const CHAR *pDirPath );

		FileHandlePtr OpenFile( const CHAR *pFileName );

	private:

		FileSystem( void );
		~FileSystem( void );

		typedef Array< ResourceFile * > FileSystemList;
		typedef Array< const CHAR * > DirPathList;

		FileSystemList	m_FSList;
		DirPathList		m_dirPathList;

	};

	typedef FileSystem::FileHandlePtr FileHandlePtr;

	PSX_INLINE FileSystem::FSHFileHandle::FSHFileHandle( ResourceFile::FSHFile fsFile )
		: m_fsFile( fsFile )
	{

	}

	PSX_INLINE FileSystem::FSHFileHandle::~FSHFileHandle( void )
	{

	}

	PSX_INLINE BOOL FileSystem::FSHFileHandle::IsValid( void )
	{
		return m_fsFile.IsValid();
	}

	PSX_INLINE const CHAR * FileSystem::FSHFileHandle::GetName( void ) const
	{
		return m_fsFile.GetName();
	}

	PSX_INLINE SIZE_T FileSystem::FSHFileHandle::GetFileSize( void )
	{
		return m_fsFile.GetFileSize();
	}

	PSX_INLINE SIZE_T64 FileSystem::FSHFileHandle::GetFileSize64( void )
	{
		return m_fsFile.GetFileSize64();
	}

	PSX_INLINE BOOL FileSystem::FSHFileHandle::ReadEntireFile( BYTE *pBuffer )
	{
		return m_fsFile.ReadEntireFile( pBuffer );
	}

	PSX_INLINE FileSystem::FileFileHandle::FileFileHandle( const CHAR *pFilename )
		: m_fileIO( pFilename, FileIO::FILEOP_READ | FileIO::FILEOP_BINARY ), m_name( pFilename )
	{
		m_fileSize = System::GetFileSize( pFilename );
	}

	PSX_INLINE FileSystem::FileFileHandle::~FileFileHandle( void )
	{
		m_fileIO.Close();
	}

	PSX_INLINE BOOL FileSystem::FileFileHandle::IsValid( void )
	{
		return m_fileIO.IsOpen() && m_fileIO.IsReadable();
	}

	PSX_INLINE const CHAR * FileSystem::FileFileHandle::GetName( void ) const
	{
		return m_name.GetBuffer();
	}

	PSX_INLINE SIZE_T FileSystem::FileFileHandle::GetFileSize( void )
	{
		return m_fileSize;
	}

	PSX_INLINE SIZE_T64 FileSystem::FileFileHandle::GetFileSize64( void )
	{
		return m_fileSize;
	}

}

#endif /* _PSX_FILE_SYSTEM_MANAGER_H_ */