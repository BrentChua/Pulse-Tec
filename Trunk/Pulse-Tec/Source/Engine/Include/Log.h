/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Log.h
*
*	Description -	Contains error code.
*
*	Comments	-	
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	08/13/10	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_ERROR_H_
#define _PSX_ERROR_H_

#include "Platform.h"
#include "Classes.h"

namespace Pulse
{
	struct EErrorCode
	{
		enum Type
		{
			UNKNOWN,				// Unknown error
			NONE,					// No error
			OKAY = NONE,			// SUCCEEDED // More like 1 or true
			_ERROR,					// Generic error code. Use Logging functionality for additional error message
			MEMORY,					// Not enough memory
			FILE,					// Invalid File operation
			FILE_NOT_AVAILABLE,		// Invalid path or filename
			ARGS,					// Invalid arguments
			SCRIPT,					// Script error
			FILE_SYSTEM_MANAGER,	// File System Manager
			GRAPHICS,				// Graphics related error
			LOGIC,					// Error that normally happens in the application logic
			INDEX_OUT_OF_BOUND,
			INVALID_PARAMETER,
			OS,						
		};
	};

	struct ELogType
	{
		enum Type
		{
			L_DEBUG,	// Generic debug message
			L_ERROR,	// Fatal error message.
			L_WARNING,	// Warnings that may cause untanted behavior.
			L_LOG1,		// Level 1 log message
			L_LOG2,		// Level 2 log message
			L_LOG3,		// Level 3 log message
			EnumCount,
		};
	};

	class LogBuffer
	{
	public:

		LogBuffer( void );
		LogBuffer( const LogBuffer &buffer );
		LogBuffer( const CHAR *pMessage, const CHAR *pFileName, const CHAR *pFuncName, const SIZE_T lineNumber );
		~LogBuffer( void );

		LogBuffer & operator = ( const LogBuffer &buffer );

		const CHAR * GetBuffer( void );

	private:

		void Copy( const CHAR *pMessage, const CHAR *pFileName, const CHAR *pFuncName, const SIZE_T lineNumber );

		// NOTE: We're not allowing this assignment operator.
		LogBuffer & operator = ( const CHAR *pMessage );

		SIZE_T m_bufferSize;
		SIZE_T m_lineNumber;
		const CHAR *m_pFileName;
		const CHAR *m_pFuncName;
		CHAR *m_pBuffer;
	};

	PSX_INLINE LogBuffer & LogBuffer::operator = ( const LogBuffer &buffer )
	{
		Copy( buffer.m_pBuffer, buffer.m_pFileName, buffer.m_pFuncName, buffer.m_lineNumber );
		return *this;
	}

	//PSX_INLINE LogBuffer & LogBuffer::operator = ( const CHAR *pMessage )
	//{
	//	Copy( pMessage );
	//	return *this;
	//}

	PSX_INLINE const CHAR * LogBuffer::GetBuffer( void )
	{
		return m_pBuffer;
	}

	class Log
	{
	public:

		static void PushLog( ELogType::Type type, const CHAR *pFileName, const CHAR *pFuncName, const SIZE_T lineNumber, const CHAR *pError, ... );
		
		static void PushLog( ELogType::Type type, const CHAR *pError, ... );

		static const LogBuffer & PopLog( LogBuffer *pBuffer = &LogBuffer() );

		static const LogBuffer & PeekLog( LogBuffer *pBuffer = &LogBuffer() );

		static EErrorCode::Type Initialize( void );
		
		static void Shutdown( void );

		static void SetFileLogging( BOOL bLogging );

		static BOOL IsFileLoggingEnabled( void ) { return m_bFileLogging; }

		static void SetLogSize( SIZE_T size ) { m_maxLogSize = size; };

		static const SIZE_T GetLogSize( void ) { return m_maxLogSize; }

		static void SetLogFilterLevel( ELogType::Type level ) { m_logLevel = level; }

		static ELogType::Type GetLogFilterLevel( void ) { return m_logLevel; }

	private:

		typedef List< LogBuffer >	ErrorList;
		typedef ELogType::Type		LogLevel;

		static LogLevel		m_logLevel;
		static ErrorList	*m_pErrorList;
		static SIZE_T		m_maxLogSize;
		static BOOL			m_bFileLogging;
		static FileIO		*m_pLogFile;

		static LogBuffer	m_emptyLog;
	};

	#define _PSX_PushLog( logType, errorMsg, ... )	Log::PushLog( logType, PSX_FILE, PSX_FUNCTION, PSX_LINE, errorMsg, __VA_ARGS__ )
	#define PSX_PushError( errorMsg, ... )			_PSX_PushLog( ELogType::L_ERROR, (PSX_GetFileFuncAndLineString() + String(PSX_String(" Message: ")) + String(PSX_String(##errorMsg##))).GetBuffer(), __VA_ARGS__ )
	#define PSX_PushWarning( errorMsg, ... )		_PSX_PushLog( ELogType::L_WARNING, (PSX_GetFileFuncAndLineString() + String(PSX_String(" Message: ")) + String(PSX_String(##errorMsg##))).GetBuffer(), __VA_ARGS__ )
	#define PSX_PushLog1( errorMsg, ... )			_PSX_PushLog( ELogType::L_LOG1, (PSX_GetFileFuncAndLineString() + String(PSX_String(" Message: ")) + String(PSX_String(##errorMsg##))).GetBuffer(), __VA_ARGS__ )
	#define PSX_PushLog2( errorMsg, ... )			_PSX_PushLog( ELogType::L_LOG2, (PSX_GetFileFuncAndLineString() + String(PSX_String(" Message: ")) + String(PSX_String(##errorMsg##))).GetBuffer(), __VA_ARGS__ )
	#define PSX_PushLog3( errorMsg, ... )			_PSX_PushLog( ELogType::L_LOG3, (PSX_GetFileFuncAndLineString() + String(PSX_String(" Message: ")) + String(PSX_String(##errorMsg##))).GetBuffer(), __VA_ARGS__ )
	#define PSX_PopLog()							Log::PopLog()
	#define PSX_PeekLog()							Log::PeekLog()
	
	// General Output
	#define PSX_PrintLog( errorMsg, ... )				Log::PushLog( ELogType::L_DEBUG, errorMsg, __VA_ARGS__ )

}

#endif /* _PSX_ERROR_H_ */