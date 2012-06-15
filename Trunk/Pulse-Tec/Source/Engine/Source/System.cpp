/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	System.cpp
*
*	Comments	-	See System.h
*
**************************************************************************************/

#include "../Include/System.h"
#include "../Include/PTLBase.h"
#include "../Include/FileIO.h"
#include "../Include/Clock.h"
#include "../Include/Clock.h"

#include <direct.h>
#include <ctime>

#if defined( PSX_PLATFORM_WINDOWS )
#include <Mmsystem.h>
#pragma comment (lib, "Winmm.lib" )
#endif /* defined( PSX_PLATFORM_WINDOWS ) */

namespace Pulse
{
	System::CPUInfo System::m_CPUInfo;
	INT64 System::m_initialCounter = 0;

	namespace _Private
	{

		INT CPUSpeed::m_milliseconds	= 0;
		INT64 CPUSpeed::m_ticks			= 0;
		INT CPUSpeed::m_milliseconds0	= 0;
		INT64 CPUSpeed::m_ticks0		= 0;

		// Windows specific CPUSpeed check implementations
#if defined( PSX_PLATFORM_WINDOWS )

		INT CPUSpeed::GetCPUSpeed( void )
		{
			static INT CPU_SPEED = 0;

			// Make sure that the delay only happens once.
			if ( CPU_SPEED != 0 )
				return CPU_SPEED;

			if ( StartTimingCPU() )
				return 0;

			// This will lock the application for 1 second
			do
			{
				UpdateCPUTime();
				Sleep( SLEEPTIME );
			} while( m_milliseconds < 1000 );

			CPU_SPEED = CalcCPUSpeed();
			return CPU_SPEED;
		}

		INT CPUSpeed::StartTimingCPU( void )
		{
			// detect ability to get info
			__asm
			{
				pushfd					; push extended flags
				pop		eax				; store eflags into eax
				mov		ebx, eax		; save EBX for testing later
				xor		eax, (1<<21)	; switch bit 32
				push	eax				; push eflags
				popfd					; pop them again
				pushfd					; push extended flags
				pop		eax				; store eflags into eax
				cmp		eax, ebx		; see if bit 32 has changed
				jz		noCPUID			;make sure it's now on
			}

			// Make ourselves high priority just for the time between when we measure the time and the CPU ticks
			ULONG priorityClass = GetPriorityClass( GetCurrentProcess() );
			INT threadPriority	= GetThreadPriority( GetCurrentThread() );
			SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS );
			SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL );

			// Start timing 
			m_milliseconds0 = (INT)timeGetTime();

			__asm
			{
				lea		ecx, m_ticks0			; get the offset
				mov		dword ptr [ecx], 0		; zero the memory
				mov		dword ptr [ecx+4], 0	;
				rdtsc							; read time-stamp counter
				mov		[ecx], eax				; store the negative
				mov		[ecx+4], edx			; in the variable
			}

			// restore thread priority
			SetThreadPriority( GetCurrentThread(), threadPriority );
			SetPriorityClass( GetCurrentProcess(), priorityClass );

			return 0;

			noCPUID:
				return -1;
		};

		void CPUSpeed::UpdateCPUTime( void )
		{
			// Make ourselves high priority just for the time between 
			//	when we measure the time and the CPU ticks.
			ULONG priorityClass = GetPriorityClass( GetCurrentProcess() );
			INT threadPriority = GetThreadPriority( GetCurrentThread() );
			SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS );
			SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL );

			// Get the times
			m_milliseconds	= -m_milliseconds0;
			m_ticks			= -m_ticks0;

			m_milliseconds += (INT)timeGetTime();

			__asm
			{
				lea		ecx, m_ticks	; get the offset
				rdtsc					; read time-stamp counter
				add		[ecx], eax		; add tick count
				adc		[ecx+4], edx	;
			}

			// Restore thread priority
			SetThreadPriority( GetCurrentThread(), threadPriority );
			SetPriorityClass( GetCurrentProcess(), priorityClass );
		};

		INT CPUSpeed::CalcCPUSpeed( void )
		{
			// get the actual CPU speed in MHz, and then 
			//	find the one in the CPU speed list that is closest.
			const struct tagCPUSPEEDS
			{
				FLOAT fSpeed;
				INT iSpeed;
			} CPUSpeeds[] = 
			{
				// valid CPU speeds that are not integrally divisble by 16.67MHz
				{ 60.00f, 60	},
				{ 75.00f, 75	},
				{ 90.00f, 90	},
				{ 120.00f, 120	},
				{ 180.00f, 180	},
			};

			// Find the closest one
			FLOAT	fSpeed	= ((FLOAT)m_ticks) / ((FLOAT)m_milliseconds * 1000.0f);
			INT		iSpeed	= CPUSpeeds[0].iSpeed;
			FLOAT	fDiff	= (FLOAT)PSX_Abs( fSpeed - CPUSpeeds[0].fSpeed );

			for ( INT i = 1; i < sizeof( CPUSpeeds ) / sizeof( CPUSpeeds[0] ); ++i )
			{
				FLOAT fTmpDiff = (FLOAT)PSX_Abs( fSpeed - CPUSpeeds[i].fSpeed );

				if ( fTmpDiff < fDiff )
				{
					iSpeed = CPUSpeeds[i].iSpeed;
					fDiff = fTmpDiff;
				}
			}

			// Now calculate the nearest multiple of fIncr speed.

			// Now if the closest one is not within one incr, calculate the
			//	nearest multiple of fIncr speed and see if that's closer.
			const FLOAT fIncr	= 16.66666666666666666666667f;
			const INT iIncr		= 4267;	// fIncr << 8

			if ( fDiff > fIncr )
			{
				// Get the number of fIncr quantums the speed is
				INT iQuantums = (INT)((fSpeed/fIncr) + 0.5f);
				FLOAT fQuantumSpeed = (FLOAT)iQuantums * fIncr;
				FLOAT fTmpDiff = (FLOAT)PSX_Abs(fQuantumSpeed - fSpeed);

				if ( fTmpDiff < fDiff )
				{
					iSpeed = (iQuantums * iIncr) >> 8;
					fDiff = fTmpDiff;
				}
			}

			return iSpeed;
		}

#endif /* defined( PSX_PLATFORM_WINDOWS ) */
	
	};

	BOOL System::Initialize( void )
	{
		Log::Initialize();
		Log::SetFileLogging( FALSE );
		Clock::Initialize();

		GetCPUInfo( &m_CPUInfo );

		m_initialCounter = GetPerformanceCounter();

		return TRUE;
	}

	void System::Shutdown( void )
	{
		PSX_ZeroMem( &m_CPUInfo, sizeof( CPUInfo ) );

		Clock::Shutdown();
	}

	void System::Update( void )
	{

	}

	BOOL System::CheckHardDiskSpace( const ULONGLONG bytesNeeded )
	{
		// Check for enough free disk space on the current disk.
		const INT drive = _getdrive();
		_diskfree_t diskFree;

		_getdiskfree( drive, &diskFree );
		const UINT64 neededClusters = bytesNeeded / ( diskFree.sectors_per_cluster * diskFree.bytes_per_sector );

		return diskFree.avail_clusters > neededClusters;
	}

#if defined( PSX_PLATFORM_WINDOWS )
	BOOL System::CheckPhysicalRAM( const SIZE_T RAMNeeded )
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(status);
		GlobalMemoryStatusEx( &status );

		if ( status.ullTotalPhys < RAMNeeded )
			return FALSE;

		if ( status.ullAvailPhys < RAMNeeded )
			return FALSE;

		// Test if we can allocate one contiguous block of memory
		char *pBuff = (char*)malloc(RAMNeeded);

		if ( !pBuff )
			return FALSE;

		free( pBuff );

		return TRUE;
	}

	BOOL System::CheckVirtualRAM( const SIZE_T RAMNeeded )
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(status);
		GlobalMemoryStatusEx( &status );

		if ( status.ullAvailVirtual < RAMNeeded )
			return FALSE;

		// Test if we can allocate one contiguous block of memory
		char *pBuff = (char*)malloc(RAMNeeded);

		if ( !pBuff )
			return FALSE;

		free(pBuff);

		return TRUE;
			
	}

	BOOL System::CheckMemory( const SIZE_T physicalRAM, const SIZE_T virtualRAM )
	{
		return CheckPhysicalRAM( physicalRAM ) && CheckVirtualRAM( virtualRAM );
	}

	CHAR * System::GetAppDirectory( CHAR *pBuff, SIZE_T size )
	{
		PSX_Assert( pBuff, "pBuff is NULL." );

		#if defined( PSX_PLATFORM_WINDOWS )
			GetModuleFileName( NULL, pBuff, size );
		# else
			#error Implement GetAppDirectory here for this current platform
		#endif

		// Remove application name
		CHAR *pPtr = pBuff + PSX_StrLen( pBuff );

		while ( pPtr != pBuff && *pPtr != PSX_String('\\') )
			--pPtr;
		
		*pPtr = NULL;

		return pBuff;
	}

	BOOL System::IsDirectoryExist( const CHAR *pDir )
	{
		PSX_Assert( pDir, "pDir is NULL." );
		DWORD attr = GetFileAttributes( pDir );
		return ( attr != 0xFFFFFFFF && attr & FILE_ATTRIBUTE_DIRECTORY );
	}

	BOOL System::CreateDirectory( const CHAR *pPathName )
	{

		// Check to make sure the directories exists
		CHAR *pFolderCheck = new CHAR[PSX_MAX_PATH];
		CHAR *pPtr = const_cast<CHAR*>( pPathName );

		while ( pPtr = PSX_StrChr( pPtr, PSX_String( '\\' ) ) )
		{
			++pPtr;
			PSX_StrCpy( pFolderCheck , pPathName, pPtr - pPathName );
			pFolderCheck [ pPtr - pPathName ] = NULL;

			// Create if it doesn't exist
			if ( !System::IsDirectoryExist( pFolderCheck ) )
			{
				if ( !::CreateDirectory( pFolderCheck, NULL ) )
				{
					if ( GetLastError() == ERROR_ALREADY_EXISTS )
						continue;
					else
						return FALSE;
				}
			}
		}

		// This can stop in the directory just before the last one (lacking last separator).
		// Attempt to create the last one.
		if ( !System::IsDirectoryExist( pPathName ) )
			if ( !::CreateDirectory( pPathName, NULL ) )
				return GetLastError() == ERROR_ALREADY_EXISTS ? TRUE : FALSE;

		return TRUE;
	}



#else
	// TODO: Implement platform specific functions...
#endif /* defined( PSX_PLATFORM_WINDOWS ) */

	BOOL System::IsFileExist( const CHAR *pFilePath )
	{
		FileIO testFile( pFilePath, FileIO::FILEOP_READ );
		return testFile.IsOpen();
	}

	SIZE_T System::GetFileSize( const CHAR *pFilePath )
	{
		BOOL                        fOk;
		WIN32_FILE_ATTRIBUTE_DATA   fileInfo;

		if (NULL == pFilePath)
			return 0;

		fOk = (GetFileAttributesEx(pFilePath, GetFileExInfoStandard, (void*)&fileInfo) != 0);

		if (!fOk)
			return 0;
		
		if ( fileInfo.nFileSizeHigh )
			return 0;

		return fileInfo.nFileSizeLow;
	}
	
	SIZE_T64 System::GetFileSize64( const CHAR *pFilePath )
	{
		BOOL                        fOk;
		WIN32_FILE_ATTRIBUTE_DATA   fileInfo;

		if (NULL == pFilePath)
			return 0;

		fOk = (GetFileAttributesEx(pFilePath, GetFileExInfoStandard, (void*)&fileInfo) != 0);

		if (!fOk)
			return 0;


		return (SIZE_T64)fileInfo.nFileSizeLow + (SIZE_T64)fileInfo.nFileSizeHigh;
	}

	U64 System::GetAppTimeCounter( void )
	{
		U64 currTime = GetPerformanceCounter();
		U64 dt = currTime - m_initialCounter;
		return dt;
	}

	F64 System::GetAppTime( void )
	{
		return Clock::CyclesToSecondsF64( GetAppTimeCounter() );
	}

	CHAR * System::GetSystemDate( CHAR *pDateStr )
	{
		#if defined( PSX_UNICODE )
			return _wstrdate( pDateStr ) ;
		#else
			return _strdate( pDateStr );
		#endif
	}
	
	CHAR * System::GetSystemTime( CHAR *pTimeStr )
	{
		#if defined( PSX_UNICODE )
			return _wstrtime( pTimeStr ) ;
		#else
			return _strtime( pTimeStr );
		#endif
	}

	// NOTE: There is a function that acurately returns
	//	the number of hardware threads but only works
	//	in Vista and above...
	SIZE_T System::GetNumHardwareThreads( void )
	{
		SYSTEM_INFO si;

		GetSystemInfo( &si );

		return si.dwNumberOfProcessors;
	}

	void System::GetCPUInfo( System::CPUInfo *pCPUInfo )
	{
		PSX_Assert( pCPUInfo, "pCPUInfo specified NULL." );

		CPUInfo info;
		char *pStr = info.vendor;
		int n=1;
		int *pn = &n;

		// set all values to 0 (false)
		memset(&info, 0, sizeof(CPUInfo));

		// 1: See if we can get CPUID and vendor 's name then
		//    check for SSE and MMX Support (vendor independant)
		__try {
			_asm {
				mov  eax, 0          // eax=0 => CPUID returns vendor name
					CPUID                // perform CPUID function

					mov  esi,     pStr
					mov  [esi],   ebx    // first 4 chars
					mov  [esi+4], edx    // next for chars
					mov  [esi+8], ecx    // last 4 chars

					mov  eax, 1          // EAX=1 => CPUID returns feature bits
					CPUID                // perform CPUID (puts feature info to EDX)

					test edx, 04000000h  // test bit 26 for SSE2
					jz   _NOSSE2         // if test failed jump
					mov  [info.bSSE2], 1 // set to true

_NOSSE2: test edx, 02000000h  // test bit 25 for SSE
		 jz   _NOSSE          // if test failed jump
		 mov  [info.bSSE], 1  // set to true

_NOSSE:  test edx, 00800000h  // test bit 23 for MMX
		 jz   _EXIT1          // if test failed jump
		 mov  [info.bMMX], 1  // set to true
_EXIT1:  // nothing to do anymore
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER) {
			//if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION)
				//return info;         // cpu inactive
			*pCPUInfo = info;            // unexpected exception occurred
			return;
		}

		// 2: See if we can get extended info (vendor independant)
		_asm {
			mov  eax, 80000000h     // EAX=0x8 => CPUID returns extended features
				CPUID
				cmp  eax, 80000000h     // must be greater than 0x80
				jbe  _EXIT2             // below or equal 0x80 then jump away
				mov [info.bEXT], 1      // set to true

				mov  eax, 80000001h     // => CPUID will copy ext. feat. bits to EDX
				CPUID
				test edx, 80000000h     // 0x8 indicates 3DNow!support
				jz   _EXIT2             // if failed jump away
				mov  [info.b3DNOW], 1   // set to true
_EXIT2:
		}

		// 3: Get vendor specific stuff
		//    INTEL: CPU id
		//    AMD:   CPU id, 3dnow_ex, mmx_ex
		if ( (strncmp(info.vendor, "GenuineIntel", 12)==0) && info.bEXT) {       // INTEL
			_asm {

				mov  eax, 1             // => CPUID will copy ext. feat. info
					CPUID                   //    to EDX and brand id to EBX
					mov  esi,   pn          // get brand id which is only supported
					mov  [esi], ebx         // by processors > PIII/Celeron
			}
			int m=0;
			memcpy(&m, pn, sizeof(char)); // id only needs lowest 8 bits
			n = m;
		}
		else if ( (strncmp(info.vendor, "AuthenticAMD", 12)==0) && info.bEXT) {  // AMD

			_asm {
				mov  eax, 1             // => CPUID will copy ext. feat. info
					CPUID                   //    to EDX and brand id to EAX
					mov  esi,   pn          // get cpu type
					mov  [esi], eax

					mov  eax, 0x80000001    // => CPUID will copy ext. feat. bits
					CPUID                   //    to EDX and cpu type to EAX

					test edx, 0x40000000    // 0x4 indicates AMD extended 3DNow!
					jz   _AMD1              // if failed jump away
					mov  [info.b3DNOWEX], 1 // set to true
_AMD1:   test edx, 0x00400000    // 0x004 indicates AMD extended MMX
		 jz   _AMD2              // if fail jump away
		 mov  [info.bMMXEX], 1   // set to true
_AMD2:
			}
		}

		else {
			if (info.bEXT)
				; /* UNKNOWN VENDOR */
			else
				; /* lack of extended features */
		}

		info.vendor[13] = '\0';                // end of string
		GetCPUName(info.name, n, info.vendor); // select cpu name

		*pCPUInfo = info;
	}

	void System::GetCPUName( Char *pName, INT n, const Char *pVendor )
	{
		// Intel processors
		if (strncmp(pVendor, "GenuineIntel", 12)==0) {
			switch (n) {
		 case 0: {
			 sprintf(pName, "< Pentium III/Celeron");
				 } break;
		 case 1: {
			 sprintf(pName, "Pentium Celeron (1)");
				 } break;
		 case 2: {
			 sprintf(pName, "Pentium III (2)");
				 } break;
		 case 3: {
			 sprintf(pName, "Pentium III Xeon/Celeron");
				 } break;
		 case 4: {
			 sprintf(pName, "Pentium III (4)");
				 } break;
		 case 6: {
			 sprintf(pName, "Pentium III-M");
				 } break;
		 case 7: {
			 sprintf(pName, "Pentium Celeron (7)");
				 } break;
		 case 8: {
			 sprintf(pName, "Pentium IV (Genuine)");
				 } break;
		 case 9: {
			 sprintf(pName, "Pentium IV");
				 } break;
		 case 10: {
			 sprintf(pName, "Pentium Celeron (10)");
				  } break;
		 case 11: {
			 sprintf(pName, "Pentium Xeon / Xeon-MP");
				  } break;
		 case 12: {
			 sprintf(pName, "Pentium Xeon-MP");
				  } break;
		 case 14: {
			 sprintf(pName, "Pentium IV-M / Xeon");
				  } break;
		 case 15: {
			 sprintf(pName, "Pentium Celeron (15)");
				  } break;

		 default: break;
			}
		}
		// AMD processors
		else if (strncmp(pVendor, "AuthenticAMD", 12)==0) {
			switch (n) {
		 case 1660: {
			 sprintf(pName, "Athlon / Duron (Model-7)");
					} break;
		 case 1644: {
			 sprintf(pName, "Athlon / Duron (Model-6)");
					} break;
		 case 1596: {
			 sprintf(pName, "Athlon / Duron (Model-3)");
					} break;
		 case 1612: {
			 sprintf(pName, "Athlon (Model-4)");
					} break;
		 case 1580: {
			 sprintf(pName, "Athlon (Model-2)");
					} break;
		 case 1564: {
			 sprintf(pName, "Athlon (Model-1)");
					} break;
		 case 1463: {
			 sprintf(pName, "K6-III (Model-9)");
					} break;
		 case 1420: {
			 sprintf(pName, "K6-2 (Model-8)");
					} break;
		 case 1404: {
			 sprintf(pName, "K6 (Model-7)");
					} break;
		 case 1388: {
			 sprintf(pName, "K6 (Model-6)");
					} break;
		 case 1340: {
			 sprintf(pName, "K5 (Model-3)");
					} break;
		 case 1324: {
			 sprintf(pName, "K5 (Model-2)");
					} break;
		 case 1308: {
			 sprintf(pName, "K5 (Model-1)");
					} break;
		 case 1292: {
			 sprintf(pName, "K5 (Model-0)");
					} break;

		 default: { sprintf(pName, "older than K5"); break; }
			}
		}
		return;
	}

}