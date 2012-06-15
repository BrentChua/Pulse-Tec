/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	System.h
*
*	Description -	System class that encapsulates system information.
*
*	Comments	-	TODO: Update CPU name lists... Its relatively old...
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/26/2009		-	File creation
**************************************************************************************/

#ifndef _PSX_SYSTEM_H_
#define _PSX_SYSTEM_H_

#include "Platform.h"
#include "Classes.h"

namespace Pulse
{

	class System
	{
	public:

		/********************************************************************************
		*	Function Name :	Initialize
		*	Description	  :	Preliminary system initializations.
		*	In			  :	
		*	Out			  :	
		*	Return		  :	BOOL - True if initialization is succesful.
		*	Example		  : 
		********************************************************************************/
		static BOOL Initialize( void );

		/********************************************************************************
		*	Function Name :	Shutdown
		*	Description	  :	
		*	In			  :	
		*	Out			  :	
		*	Return		  :	BOOL - True if initialization is succesful.
		*	Example		  : 
		********************************************************************************/
		static void Shutdown( void );

		/********************************************************************************
		*	Function Name :	Update
		*	Description	  :	Updates internal system values.
		*	In			  :	
		*	Out			  :	
		*	Return		  :	void
		*	Example		  : 
		********************************************************************************/
		static void Update( void );

		/********************************************************************************
		*	Function Name :	CheckHardDiskSpace
		*	Description	  :	Checks system hard drive if we have enough disk space.
		*	In			  :	bytesNeeded	-	Needed bytes by the application.
		*	Out			  :	
		*	Return		  :	BOOL - True if we have free disk space. Otherwise false.
		*	Example		  : 
		*					if ( System:CheckHardDiskSpace( PSX_GIGABYTE ) == FALSE )
		*						PSX_Assert( FALSE, "Not enough free disk space." );	
		********************************************************************************/
		static BOOL CheckHardDiskSpace( const ULONGLONG bytesNeeded );

		/********************************************************************************
		*	Function Name :	GetCPUSpeed
		*	Description	  :	Measures then returns the speed of the CPU.
		*	In			  :	
		*	Out			  :	
		*	Return		  :	INT - True if we have free disk space. Otherwise false.
		*	Example		  : 
		*					if ( System:GetCPUSpeed() == 1333 ) // 1.33Ghz
		*					 { ... }
		********************************************************************************/
		static INT GetCPUSpeed( void );

		/********************************************************************************
		*	Function Name :	CheckPhysicalRAM
		*	Description	  :	Checks available free Physical memory.
		*	In			  :	RAMNeeded - Bytes needed
		*	Out			  :	
		*	Return		  :	BOOL - True if we have enough free memory. Otherwise false.
		*	Example		  : 
		********************************************************************************/
		static BOOL CheckPhysicalRAM( const SIZE_T RAMNeeded );

		/********************************************************************************
		*	Function Name :	CheckVirtualRAM
		*	Description	  :	Checks available free virtual memory.
		*	In			  :	RAMNeeded - Bytes needed
		*	Out			  :	
		*	Return		  :	BOOL - True if we have enough free virtual memory. 
		*							Otherwise false.
		*	Example		  : 
		********************************************************************************/
		static BOOL CheckVirtualRAM( const SIZE_T RAMNeeded );

		/********************************************************************************
		*	Function Name :	CheckMemory
		*	Description	  :	Checks available free physical and virtual memory.
		*	In			  :	physicalRAM - Physical bytes needed in bytes.
		*					virtualRAM	- virtual memory needed in bytes.
		*	Out			  :	
		*	Return		  :	BOOL - True if we have enough free memory. Otherwise false.
		*	Example		  : 
		********************************************************************************/
		static BOOL CheckMemory( const SIZE_T physicalRAM, const SIZE_T virtualRAM );

		/********************************************************************************
		*	Function Name :	GetAppDirectory
		*	Description	  :	Returns the current directory path of the working application.
		*	In			  :	CHAR * pBuff - string buffer
		*					SIZE_T size	 - size of the buffer
		*	Out			  :	CHAR * pBuff - Will contain the application's directory
		*	Return		  :	CHAR * - Contains the directory path (*pBuff).
		*	Example		  : 
		********************************************************************************/
		static CHAR * GetAppDirectory( CHAR *pBuff, SIZE_T size );

		
		/********************************************************************************
		*	Function Name :	IsDirectoryExist
		*	Description	  :	Checks if a directory exists.
		*	In			  :	CHAR * pDir - Path and name of the folder to check.
		*	Out			  :	
		*	Return		  :	BOOL   - True if it exists. Otherwise false.
		*	Example		  : 
		********************************************************************************/
		static BOOL IsDirectoryExist( const CHAR *pDir );

		/********************************************************************************
		*	Function Name :	CreateDirectory
		*	Description	  :	Creates a directory.
		*	In			  :	CHAR * pPathName - Path and name of the folder to be created
		*	Out			  :	
		*	Return		  :	BOOL   - True if successful. Otherwise false.
		*	Example		  : 
		********************************************************************************/
		static BOOL CreateDirectory( const CHAR *pPathName );

		/********************************************************************************
		*	Function Name :	IsFileExist
		*	Description	  :	Checks if a  file exists.
		*	In			  :	CHAR * pFilePath - Path of filename to check.
		*	Out			  :	
		*	Return		  :	BOOL   - True if it exists. Otherwise false.
		*	Example		  : 
		********************************************************************************/
		static BOOL IsFileExist( const CHAR *pFilePath );

		/********************************************************************************
		*	Function Name :	GetTickCount
		*	Description	  :	Retrieves the number of milliseconds that have elapsed 
		*						since the system was started, up to 49.7 days.
		*	In			  :	
		*	Out			  :	
		*	Return		  :	ULONG	- Number of milliseconds.
		*	Example		  : 
		********************************************************************************/
		static ULONG GetTickCount( void );

		/********************************************************************************
		*	Function Name :	GetTickCount64
		*	Description	  :	Retrieves the number of milliseconds that have elapsed since
		*						the system was started. This lasts almost 200 years!
		*	In			  :	
		*	Out			  :	
		*	Return		  :	ULONGLONG	- number of milliseconds.
		*	Example		  : 
		********************************************************************************/
		static ULONGLONG GetTickCount64( void );

		/********************************************************************************
		*	Function Name :	GetFileSize
		*	Description	  :	Returns the size of a file.
		*	In			  :	CHAR * pFilePath - Path of filename to check.
		*	Out			  :	
		*	Return		  :	SIZE_T   - Size of file max of 4GB. 0 if doesn't exist or
		*						greater tha n 4GB.
		*	Example		  : 
		********************************************************************************/
		static SIZE_T GetFileSize( const CHAR *pFilePath );

		/********************************************************************************
		*	Function Name :	GetFileSize64
		*	Description	  :	Returns the size of a file..
		*	In			  :	CHAR * pFilePath - Path of filename to check.
		*	Out			  :	
		*	Return		  :	SIZE_T   - Size of file. 0 if doesn't exist.
		*	Example		  : 
		********************************************************************************/
		static SIZE_T64 GetFileSize64( const CHAR *pFilePath );

		/********************************************************************************
		*	Function Name :	GetPerformanceCounter
		*	Description	  :	Returns the current time (tick) value.
		*	In			  :
		*	Out			  :	
		*	Return		  :	INT64	- Current time value.
		*	Example		  : 
		********************************************************************************/
		static U64 GetPerformanceCounter( void );

		/********************************************************************************
		*	Function Name :	GetPerformanceFrequency
		*	Description	  :	Returns how many counts or ticks per second.
		*	In			  :
		*	Out			  :	
		*	Return		  :	INT64	- x ticks per second.
		*	Example		  : 
		********************************************************************************/
		static U64 GetPerformanceFrequency( void );

		/********************************************************************************
		*	Function Name :	GetAppTimeCounter
		*	Description	  :	Total accumulated cycles from application start.
		*	In			  :
		*	Out			  :	
		*	Return		  :	INT64	- Total accumulated cycles from application start.
		*	Example		  : 
		********************************************************************************/
		static U64 GetAppTimeCounter( void );

		/********************************************************************************
		*	Function Name :	GetAppTime
		*	Description	  :	Total accumulated time from application start.
		*	In			  :
		*	Out			  :	
		*	Return		  :	F64
		*	Example		  : 
		********************************************************************************/
		static F64 GetAppTime( void );

		/********************************************************************************
		*	Function Name :	GetSystemDate
		*	Description	  :	Returns the Operating System's date in string format.
		*	In			  :
		*	Out			  :	
		*	Return		  :	CHAR * - Returns pDateStr
		*	Example		  : 
		********************************************************************************/
		static CHAR * GetSystemDate( CHAR *pDateStr );

		/********************************************************************************
		*	Function Name :	GetSystemTime
		*	Description	  :	Returns the Operating System's time in string format.
		*	In			  :
		*	Out			  :	
		*	Return		  :	CHAR * - returns pTimeStr
		*	Example		  : 
		********************************************************************************/
		static CHAR * GetSystemTime( CHAR *pTimeStr );

		/********************************************************************************
		*	Function Name :	GetNumHardwareThreads
		*	Description	  :	Returns the number of hardware threads found.
		*	In			  :
		*	Out			  :	
		*	Return		  :	SIZE_T - number of threads
		*	Example		  : 
		********************************************************************************/
		static SIZE_T GetNumHardwareThreads( void );

	private:

		struct CPUInfo;

		static void GetCPUInfo( CPUInfo *pCPUInfo );
		static void GetCPUName( Char *pName, INT n, const Char *pVendor );

	private:

		struct CPUInfo
		{
			BOOL bSSE;        // Streaming SIMD Extensions
			BOOL bSSE2;       // STreaming SIMD Extensions 2
			BOOL b3DNOW;      // 3DNow! (vendor independant)
			BOOL b3DNOWEX;    // 3DNow! (AMD specific extensions)
			BOOL bMMX;        // MMX support
			BOOL bMMXEX;      // MMX (AMD specific extensions)
			BOOL bEXT;        // extended features available
			Char vendor[13];  // vendor name
			Char name[48];    // cpu name
		};

		static CPUInfo	m_CPUInfo;
		static INT64	m_initialCounter; //Starting frequency counter to track time

	};

	namespace _Private
	{

		/********************************************************************************
		*	Class Name	: CPUSpeed
		*	Description	: Singleton class for measuring CPU performance.
		*				  Unfortunately, this implementation is only for Microsoft platform.
		*	Comments	: TODO: Implement methods for other platforms
		********************************************************************************/
		class CPUSpeed
		{
		public:

			static INT GetCPUSpeed( void );

		private:

#if defined( PSX_PLATFORM_WINDOWS )
			// Methods by Michael Lyons at Microsoft
			enum { SLEEPTIME = 0 };
			static INT StartTimingCPU( void );
			static void UpdateCPUTime( void );
			static INT CalcCPUSpeed( void );
#else
	#error Private CPU methods for this platform needs to be defined.
#endif /* defined( PSX_PLATFORM_WINDOWS ) */
			
			static INT m_milliseconds;
			static INT64 m_ticks;
			static INT m_milliseconds0;
			static INT64 m_ticks0;

		};
	}

	PSX_INLINE ULONG System::GetTickCount( void )
	{
		#ifdef PSX_PLATFORM_WINDOWS
			return static_cast<ULONG>(::GetTickCount());
		#endif
	}

	PSX_INLINE ULONGLONG System::GetTickCount64( void )
	{
		#ifdef PSX_PLATFORM_WINDOWS
			return static_cast<ULONGLONG>(::GetTickCount64());
		#endif
	}

	PSX_INLINE INT System::GetCPUSpeed( void )
	{
		return _Private::CPUSpeed::GetCPUSpeed();
	}


	PSX_INLINE U64 System::GetPerformanceCounter( void )
	{
		static INT64 count;
		#ifdef PSX_PLATFORM_WINDOWS
			QueryPerformanceCounter( (LARGE_INTEGER*)&count );
		#endif /*PSX_PLATFORM_WINDOWS*/
		return count;
	}

	PSX_INLINE U64 System::GetPerformanceFrequency( void )
	{
		static INT64 freq;
		#ifdef PSX_PLATFORM_WINDOWS
			QueryPerformanceFrequency( (LARGE_INTEGER*)&freq );
		#endif /*PSX_PLATFORM_WINDOWS*/
		return freq;
	}
}

#endif /* _PSX_SYSTEM_H_ */