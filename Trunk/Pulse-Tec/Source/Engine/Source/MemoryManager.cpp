/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	MemoryManager.cpp
*
*	Comments	-	See MemoryManager.h
*
**************************************************************************************/

#include "../External/dlmalloc/malloc.h"
#include <assert.h>
#include "../Include/PulseConfig.h"
#include "../Include/Registry.h"
#include "../Include/MemoryManager.h"
#include "../Include/System.h"
#include "../Include/Allocators.h"


namespace Pulse
{
	PSX_REGISTRY_OBJECT_START( MemoryManager, ERegistryPriority::Highest, ERegistryPriority::Highest )
		PSX_REGISTRY_ON_INITIALIZE()
		{
			#if PSX_USE_MEMORY_MANAGER
				if ( !MemoryManager::Initialize() )
					return EErrorCode::MEMORY;
			#endif /* PSX_USE_MEMORY_MANAGER */
		}
		PSX_REGISTRY_ON_SHUTDOWN()
		{
			#if PSX_USE_MEMORY_MANAGER
				MemoryManager::Shutdown();
			#endif /* PSX_USE_MEMORY_MANAGER */
		}
	PSX_REGISTRY_OBJECT_END()
	
	struct MemoryManager::BlockHeader
	{
		BOOL		m_bUsed;
		SIZE_T		m_size;
		BlockHeader *m_pNext;
		BlockHeader *m_pPrev;
	};

	struct MemoryManager::BlockFooter
	{
		BOOL	m_bUsed;
		SIZE_T	m_size;
	};

	SIZE_T MemoryManager::m_memoryBudget		= 0;
	SIZE_T MemoryManager::m_availableMemory		= 0;
	SIZE_T MemoryManager::m_headerSize			= 0;
	SIZE_T MemoryManager::m_footerSize			= 0;
	SIZE_T MemoryManager::m_headerFooterSize	= 0;
	MemoryManager::BlockHeader * MemoryManager::m_pFreeBlock			= NULL;
	MemoryManager::BlockHeader * MemoryManager::m_pStartingMemoryBlock	= NULL;
	MemoryManager::BlockHeader * MemoryManager::m_pEndingMemoryBlock	= NULL;
	MemoryManager::SearchByPolicy MemoryManager::SearchMemory			= NULL;
	MemoryManager::FixedBlockAllocator4 * MemoryManager::m_pFixedAlloc4		= NULL;
	MemoryManager::FixedBlockAllocator8 * MemoryManager::m_pFixedAlloc8		= NULL;
	MemoryManager::FixedBlockAllocator16 * MemoryManager::m_pFixedAlloc16	= NULL;
	MemoryManager::FixedBlockAllocator32 * MemoryManager::m_pFixedAlloc32	= NULL;
	MemoryManager::FixedBlockAllocator64 * MemoryManager::m_pFixedAlloc64	= NULL;

	BOOL MemoryManager::Initialize( SIZE_T memBudget /*= PSX_MEGABYTE * 512*/ )
	{

#if (PSX_USE_MEMORY_MANAGER == 0)
		return System::CheckMemory( memBudget, 0 );
#else
		// Can we allocate memBudget amount of memory from the RAM?
		if ( !System::CheckMemory( memBudget, 0 ) )
			return FALSE;

		// Check if we have already initialized
		if ( m_pStartingMemoryBlock )
			return FALSE;

		m_memoryBudget		= static_cast<SIZE_T>(memBudget);
		m_headerSize		= sizeof( BlockHeader );
		m_footerSize		= sizeof( BlockFooter );
		m_headerFooterSize	= sizeof( BlockHeader ) + sizeof( BlockFooter );
		m_availableMemory	= m_memoryBudget - m_headerFooterSize;

		m_pStartingMemoryBlock	= (BlockHeader*)malloc( m_memoryBudget );
		m_pEndingMemoryBlock	= (BlockHeader*)((Char*)m_pStartingMemoryBlock + m_memoryBudget);
		m_pFreeBlock			= m_pStartingMemoryBlock;
		PSX_Assert( m_pFreeBlock, "Failed to allocate memory block for the memory manager." );

		m_pFreeBlock->m_bUsed	= FALSE;
		m_pFreeBlock->m_pNext	= m_pFreeBlock;
		m_pFreeBlock->m_pPrev	= m_pFreeBlock;
		m_pFreeBlock->m_size	= m_memoryBudget;

		BlockFooter *pFooter	= (BlockFooter*)((Char*)m_pFreeBlock + m_memoryBudget - m_footerSize);
		pFooter->m_bUsed		= FALSE;
		pFooter->m_size			= m_memoryBudget;

		SearchMemory = (SearchByPolicy)&BestFit;

		// We can now start initializing our other allocators here.
		// Our other allocators will get memory from the general heap.
		m_pFixedAlloc4	= new(FORCE_GENERAL_HEAP) FixedBlockAllocator4;
		m_pFixedAlloc8	= new(FORCE_GENERAL_HEAP) FixedBlockAllocator8;
		m_pFixedAlloc16 = new(FORCE_GENERAL_HEAP) FixedBlockAllocator16;
		m_pFixedAlloc32 = new(FORCE_GENERAL_HEAP) FixedBlockAllocator32;
		m_pFixedAlloc64 = new(FORCE_GENERAL_HEAP) FixedBlockAllocator64;

		m_pFixedAlloc4->Initialize( 1024 ); 
		m_pFixedAlloc8->Initialize( 1024 ); 
		m_pFixedAlloc16->Initialize( 1024 );
		m_pFixedAlloc32->Initialize( 1024 );
		m_pFixedAlloc64->Initialize( 1024 );

		return TRUE;
#endif /* PSX_USE_MEMORY_MANAGER */
	}

	void MemoryManager::Shutdown( void )
	{
		// We can't delete these allocators normally because it would
		// fall to itself to deallocate.
		m_pFixedAlloc64->~FixedBlockAllocator();
		_Deallocate( m_pFixedAlloc64 );
		m_pFixedAlloc32->~FixedBlockAllocator();
		_Deallocate( m_pFixedAlloc32 );
		m_pFixedAlloc16->~FixedBlockAllocator();
		_Deallocate( m_pFixedAlloc16 );
		m_pFixedAlloc8->~FixedBlockAllocator();
		_Deallocate( m_pFixedAlloc8 );
		m_pFixedAlloc4->~FixedBlockAllocator();
		_Deallocate( m_pFixedAlloc4 );

		free( (void*)m_pStartingMemoryBlock );

		m_memoryBudget			= 0;
		m_availableMemory		= 0;
		m_headerSize			= 0;
		m_footerSize			= 0;
		m_headerFooterSize		= 0;
		m_pFreeBlock			= NULL;
		m_pStartingMemoryBlock	= NULL;
		m_pEndingMemoryBlock	= NULL;
		SearchMemory			= NULL;
	}

	void * MemoryManager::Alloc( SIZE_T reqSize, MEMORY_TYPE type /*= HEAP */ )
	{
		void *ptr = NULL;

		switch ( type )
		{
		case HEAP:

			if ( reqSize <= 4 )
				ptr = m_pFixedAlloc4->Alloc();
			if ( !ptr && reqSize <= 8 )
				ptr = m_pFixedAlloc8->Alloc();
			if ( !ptr && reqSize <= 16 )
				ptr = m_pFixedAlloc16->Alloc();
			if ( !ptr && reqSize <= 32 )
				ptr = m_pFixedAlloc32->Alloc();
			if ( !ptr && reqSize <= 64 )
				ptr = m_pFixedAlloc64->Alloc();
			if ( !ptr )
				ptr = _Allocate( reqSize );

			break;

		case FORCE_GENERAL_HEAP:

			ptr = _Allocate( reqSize );
			
			break;

		default:
			PSX_Assert( FALSE, "Invalid memory type." );
		}

		return ptr;
	}

	void MemoryManager::Free( void *pAddr )
	{
		if ( pAddr == NULL )
			return;

		// General heap address check should be performed last!
		if ( m_pFixedAlloc64->IsAddressOwned( pAddr ) )
			m_pFixedAlloc64->Free( pAddr );
		else if ( m_pFixedAlloc32->IsAddressOwned( pAddr ) )
			m_pFixedAlloc32->Free( pAddr );
		else if ( m_pFixedAlloc16->IsAddressOwned( pAddr ) )
			m_pFixedAlloc16->Free( pAddr );
		else if ( m_pFixedAlloc8->IsAddressOwned( pAddr ) )
			m_pFixedAlloc8->Free( pAddr );
		else if ( m_pFixedAlloc4->IsAddressOwned( pAddr ) )
			m_pFixedAlloc4->Free( pAddr );
		else if ( IsAddressOwned( pAddr ) )
			_Deallocate( pAddr );
		else
			PSX_Assert( FALSE, "Memory address is not owned by the memory manager." );
	}

	void * MemoryManager::_Allocate( SIZE_T reqSize )
	{
		SIZE_T totalReqSize = reqSize + m_headerFooterSize;
		BlockHeader *pHeader = (BlockHeader*)(SearchMemory)( totalReqSize );
		
		if ( pHeader == NULL )
			return NULL; // Out of memory

		Char *pAllocated = (Char*)pHeader;
		BlockFooter *pFooter = (BlockFooter*)(pAllocated + pHeader->m_size - m_footerSize);
		SIZE_T size = pHeader->m_size;

		// Exact fit
		if ( size == totalReqSize )
		{
			ConsumeWholeBlock:
				
				pHeader->m_bUsed = TRUE;
				pFooter->m_bUsed = TRUE;

				// Is this the last in the list?
				if ( pHeader->m_pNext == pHeader )
					m_pFreeBlock = NULL;
				else
				{
					pHeader->m_pPrev->m_pNext = pHeader->m_pNext;
					pHeader->m_pNext->m_pPrev = pHeader->m_pPrev;
					m_pFreeBlock = pHeader->m_pNext;
				}

				// Update available memory
				m_availableMemory -= (pHeader->m_size + m_headerFooterSize);

				return (void*)(pAllocated + m_headerSize);
		}

		// Size is greater than needed.
		// See if it can be split into two blocks.
		if ( size >= totalReqSize + m_headerFooterSize )
		{
			// Split the block into two blocks ( one used, one free )
			BlockHeader *pUsedHeader = pHeader;
			BlockFooter *pUsedFooter = (BlockFooter*)(pAllocated + reqSize + m_headerSize);
			BlockHeader *pFreeHeader = (BlockHeader*)((Char*)pUsedFooter + m_footerSize);
			BlockFooter *pFreeFooter = pFooter;

			pUsedHeader->m_bUsed = TRUE;
			pUsedFooter->m_bUsed = TRUE;
			pFreeHeader->m_bUsed = FALSE;
			pFreeFooter->m_bUsed = FALSE;

			// Updated size to reflect the new splits
			pUsedHeader->m_size = totalReqSize;
			pUsedFooter->m_size = totalReqSize;
			pFreeHeader->m_size = size - totalReqSize;
			pFreeFooter->m_size = size - totalReqSize;

			if ( pHeader->m_pNext == pHeader )
			{
				pFreeHeader->m_pNext = pFreeHeader;
				pFreeHeader->m_pPrev = pFreeHeader;
			}
			else
			{
				pFreeHeader->m_pNext = pHeader->m_pNext;
				pFreeHeader->m_pPrev = pHeader->m_pPrev;
				pHeader->m_pPrev->m_pNext = pFreeHeader;
				pHeader->m_pNext->m_pPrev = pFreeHeader;
			}

			m_pFreeBlock = pFreeHeader;

			// Update available memory
			m_availableMemory -= pUsedHeader->m_size;

			return (void*)(pAllocated + m_headerSize);
		}

		// If we get here that means the block isn't big enough to split into two blocks
		goto ConsumeWholeBlock; 
	}

	void MemoryManager::_Deallocate( void *pAddr)
	{
		if ( pAddr == NULL )
			return;

		BlockHeader *pHeader = (BlockHeader*)((Char*)pAddr - m_headerSize);
		BlockFooter *pFooter = (BlockFooter*)((Char*)pHeader + pHeader->m_size - m_footerSize);

		pHeader->m_bUsed = FALSE;
		pFooter->m_bUsed = FALSE;

		m_availableMemory += (pHeader->m_size - m_headerFooterSize);

		if ( m_pFreeBlock == NULL )
		{
			pHeader->m_pNext = pHeader;
			pHeader->m_pPrev = pHeader;
			m_pFreeBlock = pHeader;

			return;
		}

		// Check adjacent memory if free then coalesce
		
		BlockFooter *pLeftFooter = (BlockFooter*)((Char*)pHeader - m_footerSize);
		if ( pHeader != m_pStartingMemoryBlock && pLeftFooter->m_bUsed == FALSE )
		{
			m_availableMemory += m_headerFooterSize;

			// Coalesce left memory block
			BlockHeader *pLeftHeader = (BlockHeader*)((Char*)pLeftFooter - pLeftFooter->m_size + m_footerSize);

			pLeftHeader->m_size += pHeader->m_size;
			pFooter->m_size = pLeftHeader->m_size;

			if ( pLeftHeader->m_pNext == pLeftHeader )
				return; // This is the only block left in the free list

			// Move to the next free block if freeblock points to this one
			if ( pLeftHeader == m_pFreeBlock )
				m_pFreeBlock = m_pFreeBlock->m_pPrev;

			// Remove from the free list for coalescing to the right block and adding back to the free list.
			pHeader = pLeftHeader;
			pHeader->m_pPrev->m_pNext = pHeader->m_pNext;
			pHeader->m_pNext->m_pPrev = pHeader->m_pPrev;
		}

		BlockHeader *pRightHeader = (BlockHeader*)((Char*)pHeader + pHeader->m_size);
		if ( pRightHeader != m_pEndingMemoryBlock && pRightHeader->m_bUsed == FALSE )
		{
			m_availableMemory += m_headerFooterSize;

			BlockFooter *pRightFooter = (BlockFooter*)((Char*)pRightHeader + pRightHeader->m_size - m_footerSize);

			pHeader->m_size += pRightHeader->m_size;
			pRightFooter->m_size = pHeader->m_size;

			if ( pRightHeader->m_pNext == pRightHeader )
			{
				pHeader->m_pNext = pHeader;
				pHeader->m_pPrev = pHeader;
				m_pFreeBlock = pHeader;
				return;
			}

			// Move to the next free block if freeblock points to this one
			if ( pRightHeader == m_pFreeBlock )
				m_pFreeBlock = pRightHeader->m_pNext;

			// Set this up for adding back to the free list
			pRightHeader->m_pPrev->m_pNext = pRightHeader->m_pNext;
			pRightHeader->m_pNext->m_pPrev = pRightHeader->m_pPrev;
		}

		// Add back to the free list
		pHeader->m_pPrev = m_pFreeBlock->m_pPrev;
		pHeader->m_pNext = m_pFreeBlock;
		m_pFreeBlock->m_pPrev->m_pNext = pHeader;
		m_pFreeBlock->m_pPrev = pHeader;
		m_pFreeBlock = pHeader;
	}

	void * MemoryManager::AllocAligned( SIZE_T reqSize, SIZE_T alignment )
	{
		ADDRESS_T expandedSize = reqSize + alignment;

		// Allocate unalignd block of memory
		ADDRESS_T rawAddress = reinterpret_cast<SIZE_T>( _Allocate( expandedSize ) );

		// calculate misalignment
		ADDRESS_T mask				= static_cast<SIZE_T>(alignment - 1);
		ADDRESS_T misalignment		= rawAddress & mask;
		ADDRESS_T adjustment		= alignment - misalignment;
		ADDRESS_T alignedAddress	= rawAddress + adjustment;

		// Use one byte just before the aligned address to store the adjustment
		BYTE * pAdjustment = reinterpret_cast<BYTE *>(alignedAddress - 1);
		*pAdjustment = adjustment;

		return reinterpret_cast<void*>(alignedAddress);
	}

	void MemoryManager::DeallocateAligned( void *pAddr )
	{
		#ifdef PSX_DEBUG
			PSX_Assert( IsAddressOwned( pAddr ), "Supposedly aligned memory block is not owned by memory manager." );
		#endif /*PSX_DEBUG*/

		ADDRESS_T alignedAddr	= reinterpret_cast<ADDRESS_T>(pAddr);
		BYTE	  *pAdjustment	= reinterpret_cast<BYTE*>(alignedAddr - 1);
		ADDRESS_T rawAddress	= alignedAddr - *pAdjustment;

		_Deallocate( reinterpret_cast<PVOID>(rawAddress) );
	}

	// Search the list until we find a block that's at least as big as the required size
	void * FirstFit( SIZE_T reqSize )
	{
		MemoryManager::BlockHeader *pHeader = MemoryManager::m_pFreeBlock;

		if ( pHeader == NULL )
			return NULL;

		do
		{
			if ( /*pHeader->m_bUsed == FALSE &&*/ pHeader->m_size >= reqSize )
			{
				MemoryManager::m_pFreeBlock = pHeader;
				return (void*)pHeader;
			}

			pHeader = pHeader->m_pNext;

		} while( pHeader != MemoryManager::m_pFreeBlock );

		// If we get here, that means we don't have a big enough size.
		return NULL;
	}

	void * BestFit( SIZE_T reqSize )
	{
		// Find the smallest possible size
		SIZE_T reqAllocSize						= reqSize;
		MemoryManager::BlockHeader *pHeader		= MemoryManager::m_pFreeBlock;
		MemoryManager::BlockHeader *pCandidate	= NULL;

		if ( pHeader == NULL )
			return NULL;

		do
		{
			if ( pHeader->m_size >= reqAllocSize )
			{
				// Otherwise, just store and continue searching if we find a more suitable fit
				if ( pCandidate == NULL || pHeader->m_size < pCandidate->m_size )
				{
					// Return if exact fit
					if ( pHeader->m_size == reqSize ) 
						return (void*)pHeader;

					pCandidate = pHeader;
				}
			}

			pHeader = pHeader->m_pNext;

		} while( pHeader != MemoryManager::m_pFreeBlock );

		// If pCandidate is not null, then we have an available size. Just not an exact fit.
		return pCandidate;
	}


};

using namespace Pulse;

#if PSX_USE_MEMORY_MANAGER

void * operator new( size_t size )
{
	return MemoryManager::Alloc( size );
}

void * operator new( size_t size, MemoryManager::MEMORY_TYPE type )
{
	return MemoryManager::Alloc( size, type );
}

void * operator new[]( size_t size ) throw()
{
	return MemoryManager::Alloc( size );
}

void * operator new[]( size_t size, MemoryManager::MEMORY_TYPE type ) throw()
{
	return MemoryManager::Alloc( size, type );
}

void operator delete( void *pAddr )
{
	MemoryManager::Free( pAddr );
}

void operator delete( void *pAddr, Pulse::MemoryManager::MEMORY_TYPE type )
{ PSX_Assert(FALSE, "Dummy operator delete with MEMORY_TYPE shouldn't be called."); }

void operator delete[]( void *pAddr ) throw()
{
	MemoryManager::Free( pAddr );
}

void operator delete[]( void *pAddr, Pulse::MemoryManager::MEMORY_TYPE type )
{ PSX_Assert(FALSE, "Dummy operator delete[] with MEMORY_TYPE shouldn't be called."); }

#endif /* PSX_USE_MEMORY_MANAGER */