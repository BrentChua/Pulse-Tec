/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	MemoryManager.h
*
*	Description -	Basic memory manager.
*
*	Comments	-	I just want to note that there is not error checking feature yet...
*					 So be very careful if you're using this and doing silly things like
*					 memory overrun, double-free, leaking memory, etc...
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/18/2009	-	Creation of MemoryManager
*			MrCodeSushi	-	12/23/2009	-	Re-writing the Memory manager for the 7th time.
*			MrCodeSushi	-	01/17/2010	-	Added functions for zlib alloc and free support.
**************************************************************************************/

#ifndef PSX_MEMORY_MANAGER_H_
#define PSX_MEMORY_MANAGER_H_

#include "PulseSTD.h"

namespace Pulse
{
	// Specific allocators
	template < SIZE_T blockSize, UINT alignment >
	class FixedBlockAllocator;

	class MemoryManager
	{
	public:

		enum MEMORY_TYPE { HEAP, FORCE_GENERAL_HEAP };

		static BOOL Initialize( SIZE_T memBudget = PSX_MEGABYTE * 512 );
		static void Shutdown( void );

		static void * Alloc( SIZE_T reqSize, MEMORY_TYPE type = HEAP );
		static void Free( void *pAddr );

		static void * AllocAligned( SIZE_T reqSize, SIZE_T alignment );
		static void DeallocateAligned( void *pAddr );

		static SIZE_T GetAvailableMemory( void );
		static SIZE_T GetTotalMemory( void );

		// NOTE: For zlib alloc and dealloc use exculsively
		static void *zlibAlloc( void *pOpaque, UINT items, UINT size );
		static void zlibFree( void *pOpaque, void *pAdr );

	private:

		// This can't create an instance of itself
		MemoryManager( void ) { }
		MemoryManager( MemoryManager & ) { }
		~MemoryManager( void ) { }
		MemoryManager & operator = ( MemoryManager & ) { return *this; }

		// General Heap alloc/dealloc methods.
		static void * _Allocate( SIZE_T reqSize );
		static void _Deallocate( void *pAddr );

		// WARNING: This should be called last! After all the fixed size check
		static BOOL IsAddressOwned( void *pAddr )
		{
			return pAddr >= m_pStartingMemoryBlock && pAddr < m_pEndingMemoryBlock;
		}

		// General Heap
		typedef SIZE_T ADDRESS_T;
		typedef void * (*SearchByPolicy)( SIZE_T size );

		friend void * FirstFit( SIZE_T reqSize );
		friend void * BestFit( SIZE_T reqSize );

		struct BlockHeader;
		struct BlockFooter;

		static SIZE_T m_memoryBudget;
		static SIZE_T m_availableMemory;
		static SIZE_T m_headerSize;
		static SIZE_T m_footerSize;
		static SIZE_T m_headerFooterSize;
		static BlockHeader *m_pFreeBlock;
		static BlockHeader *m_pStartingMemoryBlock;
		static BlockHeader *m_pEndingMemoryBlock;
		static SearchByPolicy SearchMemory;

		// General fixed block allocators
		typedef FixedBlockAllocator<4,1>  FixedBlockAllocator4;
		typedef FixedBlockAllocator<8,1>  FixedBlockAllocator8;
		typedef FixedBlockAllocator<16,1> FixedBlockAllocator16;
		typedef FixedBlockAllocator<32,1> FixedBlockAllocator32;
		typedef FixedBlockAllocator<64,1> FixedBlockAllocator64;

		static FixedBlockAllocator4		*m_pFixedAlloc4;
		static FixedBlockAllocator8		*m_pFixedAlloc8;
		static FixedBlockAllocator16	*m_pFixedAlloc16;
		static FixedBlockAllocator32	*m_pFixedAlloc32;
		static FixedBlockAllocator64	*m_pFixedAlloc64;
		
	};

	PSX_INLINE SIZE_T MemoryManager::GetAvailableMemory( void )
	{
		return m_availableMemory;
	}

	PSX_INLINE SIZE_T MemoryManager::GetTotalMemory( void )
	{
		return m_memoryBudget;
	}

	// For zlib alloc and dealloc
	PSX_FORCE_INLINE void * MemoryManager::zlibAlloc( void *pOpaque, UINT items, UINT size )
	{
		#if ( PSX_USE_MEMORY_MANAGER == 1 )
			return Alloc( static_cast<SIZE_T>(size * items) );
		#else
			return malloc( static_cast<SIZE_T>(size * items) );
		#endif
	}

	PSX_FORCE_INLINE void MemoryManager::zlibFree( void *pOpaque, void *pAdr )
	{
		#if ( PSX_USE_MEMORY_MANAGER == 1 )
			Free( pAdr );
		#else
			free( pAdr );
		#endif
	}

	// Implicitly converts to the desired pointer data type
	class PVoidToT
	{
	public:

		PSX_FORCE_INLINE PVoidToT( PVOID pVoid )
			: m_pVoid( pVoid ) 
		{ }

		template < typename T >
		PSX_FORCE_INLINE operator T * () { return reinterpret_cast<T*>(m_pVoid); }

	private:
		PVOID m_pVoid;
	};

	// Memory function macros
	#if ( PSX_USE_MEMORY_MANAGER == 1 )
		#define PSX_MallocAligned( _size, _alignment )	PVoidToT(MemoryManager::AllocAligned( _size, _alignment ))
		#define PSX_DeallocAligned( _ptr )				MemoryManager::DeallocateAligned( (PVOID)##_ptr )
		#define PSX_Malloc( _size )						PVoidToT(MemoryManager::Alloc( _size ))
		#define PSX_Dealloc( _ptr )						MemoryManager::Free( (PVOID)##_ptr )
	#elif defined(PSX_PLATFORM_WINDOWS)									
		#define PSX_MallocAligned( _size, _alignment )	PVoidToT(_aligned_malloc( _size, _alignment ))
		#define PSX_DeallocAligned( _ptr )				_aligned_free( (VOID*)_ptr )
		#define PSX_Malloc( _size )						PVoidToT(malloc( _size ))
		#define PSX_Dealloc( _ptr ) free(				(VOID*)##_ptr )		
	#else
		#error Non-windows platform. Malloc and dealloc not implemented
	#endif

}

#if PSX_USE_MEMORY_MANAGER


void * operator new( size_t size );
void * operator new( size_t size, Pulse::MemoryManager::MEMORY_TYPE type );
void * operator new[]( size_t size ) throw();
void * operator new[]( size_t size, Pulse::MemoryManager::MEMORY_TYPE type ) throw();
void operator delete( void *pAddr );
void operator delete( void *pAddr, Pulse::MemoryManager::MEMORY_TYPE type );
void operator delete[]( void *pAddr ) throw();
void operator delete[]( void *pAddr, Pulse::MemoryManager::MEMORY_TYPE type ) throw();

#endif /* PSX_USE_MEMORY_MANAGER */

#endif /* PSX_MEMORY_MANAGER_H_ */