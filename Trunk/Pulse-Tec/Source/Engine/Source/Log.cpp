/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Error.cpp
*
*	Comments	-	See Log.h
*
**************************************************************************************/

#include "../Include/PulseSTD.h"
#include "../Include/String.h"
#include "../Include/Log.h"
#include "../Include/List.h"
#include "../Include/FileIO.h"

namespace Pulse
{
	PSX_REGISTRY_OBJECT_START( Log, ERegistryPriority::VeryHigh, ERegistryPriority::Normal )
		PSX_REGISTRY_ON_TEST()
		{
			//PSX_PushLog3( "Hello world. Testing VA_ARGS. A number follows: %d", 5 );
		}
		PSX_REGISTRY_ON_TEST_INITIALIZE()
		{
			EErrorCode::Type err = Log::Initialize();
			Log::SetFileLogging( TRUE );
			return err;
		}
		PSX_REGISTRY_ON_TEST_SHUTDOWN()
		{
			Log::Shutdown();
		}
	PSX_REGISTRY_OBJECT_END()

	ELogType::Type Log::m_logLevel = ELogType::L_LOG3;
	Log::ErrorList *Log::m_pErrorList = PSX_NULL;
	SIZE_T Log::m_maxLogSize = 30;
	BOOL Log::m_bFileLogging = FALSE;
	FileIO *Log::m_pLogFile = PSX_NULL;
	LogBuffer Log::m_emptyLog( PSX_String("No log messages available."), PSX_String(""), PSX_String(""), 0 );

	LogBuffer::LogBuffer( void )
		: m_bufferSize( 0 ), m_lineNumber( 0 ), m_pFileName( 0 ), m_pFuncName( NULL ), m_pBuffer( NULL )
	{
	}

	LogBuffer::LogBuffer( const LogBuffer &buffer )
		: m_bufferSize( 0 ), m_lineNumber( 0 ), m_pFileName( 0 ), m_pBuffer( NULL )
	{
		Copy( buffer.m_pBuffer, buffer.m_pFileName, buffer.m_pFuncName, buffer.m_lineNumber );
	}

	LogBuffer::LogBuffer( const CHAR *pMessage, const CHAR *pFileName, const CHAR *pFuncName, const SIZE_T lineNumber )
		: m_bufferSize( 0 ), m_lineNumber( 0 ), m_pFileName( 0 ), m_pFuncName( NULL ), m_pBuffer( NULL )
	{
		Copy( pMessage, pFileName, pFuncName, lineNumber );
	}

	LogBuffer::~LogBuffer( void )
	{
		PSX_SafeDelete( m_pBuffer );
	}

	void LogBuffer::Copy( const CHAR *pMessage, const CHAR *pFileName, const CHAR *pFuncName, const SIZE_T lineNumber )
	{
		if ( m_pBuffer == pMessage )
			return;

		m_lineNumber = lineNumber;
		m_pFileName = pFileName;
		m_pFuncName = pFuncName;

		SIZE_T len = PSX_StrLen( pMessage ) + PSX_StrLen( pFuncName ) + 1;

		if ( m_bufferSize < len )
		{
			PSX_SafeDelete( m_pBuffer );
			m_pBuffer = new CHAR[len];
		}

		PSX_StrCpy( m_pBuffer, pMessage, len );
		m_pBuffer[len - 1] = PSX_NULL;
	}

	EErrorCode::Type Log::Initialize( void )
	{
		PSX_SafeDelete( m_pErrorList );
		Log::m_pErrorList = new Log::ErrorList;
		PSX_Assert( m_pErrorList, "Failed to allocate memory." );

		return m_pErrorList ? EErrorCode::NONE : EErrorCode::MEMORY;
	}

	void Log::Shutdown( void )
	{
		SetFileLogging( FALSE );
		PSX_SafeDelete( m_pErrorList );
	}

	void Log::SetFileLogging( BOOL bLogging )
	{
		if ( m_bFileLogging == bLogging )
			return;

		m_bFileLogging = bLogging;

		if ( m_bFileLogging ) // Turn logging on
		{
			m_pLogFile = new FileIO( PSX_String("Log.txt"), FileIO::FILEOP_TRUNCATE | FileIO::FILEOP_WRITE );
			
			// Start marker
			m_pLogFile->WriteString( PSX_String("------------------------------ BEGIN LOGGING ------------------------------\n") );
			
			// Date and time
			CHAR date[15], time[15];
			System::GetSystemDate( date );
			System::GetSystemTime( time );
			m_pLogFile->WriteString( PSX_String("------------------------- DATE: ") );
			m_pLogFile->WriteString( date );
			m_pLogFile->WriteString( PSX_String(" | TIME: ") );
			m_pLogFile->WriteString( time );
			m_pLogFile->WriteString( PSX_String(" -------------------------\n") );
		}
		else // Turn logging off
		{
			// End marker
			m_pLogFile->WriteString( PSX_String("------------------------------ END LOGGING ------------------------------\n\n") );

			m_pLogFile->Close();
			PSX_SafeDelete( m_pLogFile );
		}

	}

	void Log::PushLog( ELogType::Type type, const CHAR *pFileName, const CHAR *pFuncName, const SIZE_T lineNumber, const CHAR *pError, ... )
	{
		if ( type > m_logLevel )
			return;

		CHAR *tempMsg;
		SIZE_T len = 0;
		va_list arg_ptr;

		va_start( arg_ptr, pError );
		#if defined(PSX_UNICODE)
				len = _vscwprintf( pError, arg_ptr );
				tempMsg = new CHAR[len + 1];
				vswprintf( tempMsg, pError, arg_ptr );
		#else
				len = _vscprintf( pError, arg_ptr );
				tempMsg = new CHAR[len + 1];
				vsprintf( tempMsg, pError, arg_ptr );
		#endif /* defined(PSX_UNICODE) */

		LogBuffer log(tempMsg, pFileName, pFuncName, lineNumber);

		PSX_SafeDeleteArray( tempMsg );

		m_pErrorList->PushFront( log );

		if ( m_pErrorList->GetSize() > m_maxLogSize )
			m_pErrorList->PopBack();

		if ( m_bFileLogging )
		{
			m_pLogFile->WriteString( log.GetBuffer() );
			m_pLogFile->WriteString( PSX_String("\n") );
			m_pLogFile->Flush();
		}

		// Display in output window
		OutputDebugString( log.GetBuffer() );
		OutputDebugString( PSX_String("\n") );
	}

	void Log::PushLog( ELogType::Type type, const CHAR *pError, ... )
	{
		if ( type > m_logLevel )
			return;

		CHAR *tempMsg;
		SIZE_T len = 0;
		va_list arg_ptr;

		va_start( arg_ptr, pError );
		#if defined(PSX_UNICODE)
				len = _vscwprintf( pError, arg_ptr );
				tempMsg = new CHAR[len + 1];
				vswprintf( tempMsg, pError, arg_ptr );
		#else
				len = _vscprintf( pError, arg_ptr );
				tempMsg = new CHAR[len + 1];
				vsprintf( tempMsg, pError, arg_ptr );
		#endif /* defined(PSX_UNICODE) */

		// Display in output window
		OutputDebugString( tempMsg );
		OutputDebugString( PSX_String("\n") );

		PSX_SafeDeleteArray( tempMsg );
	}

	const LogBuffer & Log::PopLog( LogBuffer *pBuffer )
	{
		if ( !m_pErrorList->GetSize() )
			return m_emptyLog;

		*pBuffer = m_pErrorList->GetFront();
		m_pErrorList->PopFront();
		return *pBuffer;
	}

	const LogBuffer & Log::PeekLog( LogBuffer *pBuffer )
	{
		*pBuffer = m_pErrorList->GetFront();
		return *pBuffer;
	}

}