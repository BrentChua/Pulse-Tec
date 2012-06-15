/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	FileSystem.cpp
*
*	Comments	-	See FileSystem.h
*
**************************************************************************************/

#include "../Include/FileSystem.h"
#include "../Include/String.h"
#include "../Include/System.h"

namespace Pulse
{
	BOOL FileSystem::FileFileHandle::ReadEntireFile( BYTE *pBuffer )
	{
		PSX_AssertNoMsg( pBuffer );

		m_fileIO.Seek( 0, FileIO::SEEKOP_BEGIN );
		return m_fileIO.Read( pBuffer, m_fileSize ) == m_fileSize;
	}

	FileSystem::FileSystem( void )
	{

	}

	FileSystem::~FileSystem( void )
	{
		FileSystemList::Iterator iter = m_FSList.IteratorBegin();
		FileSystemList::Iterator iterEnd = m_FSList.IteratorEnd();

		while ( iter != iterEnd )
		{
			delete *iter;
			++iter;
		}
	}

	EErrorCode::Type FileSystem::AddResourceFile( const CHAR *pFilename )
	{
		PSX_AssertNoMsg( pFilename );

		if ( pFilename == PSX_NULL )
		{
			PSX_PushError( "ResourceFile object can't be null." );
			return EErrorCode::FILE_NOT_AVAILABLE;
		}

		if ( System::IsFileExist( pFilename ) )
		{
			ResourceFile *pNewFS = new ResourceFile;

			if ( !pNewFS->Open( pFilename ) )
			{
				PSX_PushError( "Failed to open file system." );
				return EErrorCode::FILE_SYSTEM_MANAGER;
			}

			m_FSList.PushBack( pNewFS );
		}
		else
		{
			PSX_PushError( "Invalid file path." );
			return EErrorCode::FILE_NOT_AVAILABLE;
		}

		return EErrorCode::NONE;
	}

	void FileSystem::AddDirectoryPath( const CHAR *pDirPath )
	{
		m_dirPathList.PushBack( pDirPath );
	}

	FileHandlePtr FileSystem::OpenFile( const CHAR *pFileName )
	{
		BOOL bFoundFile = FALSE;
		ResourceFile *pFS;
		String fullPath;
		ResourceFile::FSHFile fsFile;

		// Find in the File System list first
		for ( SIZE_T i = 0; i < m_FSList.GetSize(); ++i )
		{
			pFS = m_FSList[i];

			fsFile = pFS->FindFile( pFileName );
			
			if ( fsFile.IsValid() )
			{
				bFoundFile = TRUE;
				break;
			}
			
			for ( SIZE_T j = 0; j < m_dirPathList.GetSize(); ++j )
			{
				fullPath = m_dirPathList[j];
				fullPath += PSX_String("/");
				fullPath += pFileName;

				fsFile = pFS->FindFile( fullPath.GetBuffer() );

				if ( fsFile.IsValid() )
				{
					bFoundFile = TRUE;
					break;
				}
			}

			if ( bFoundFile )
				break;
		}

		// If the file has been found then simply return. Otherwise look it up as an external file
		if ( bFoundFile )
			return new FSHFileHandle( fsFile );

		// Find the file externally

		if ( System::IsFileExist( pFileName ) )
		{
			return new FileFileHandle( pFileName );
		}

		for ( SIZE_T i = 0; i < m_dirPathList.GetSize(); ++i )
		{
			fullPath = m_dirPathList[i];
			fullPath += PSX_String("/");
			fullPath += pFileName;

			if ( System::IsFileExist( fullPath.GetBuffer() ) )
			{
				return new FileFileHandle( fullPath.GetBuffer() );
			}
		}

		return PSX_NULL;
	}

}