/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Allocators.cpp
*
*	Comments	-	See Allocators.h
*
**************************************************************************************/

#include "../Include/Allocators.h"

namespace Pulse
{
	// StackAllocator
	StackAllocator::StackAllocator( void )
		: m_marker( 0 ), m_bufferSize( 0 ), m_bytesAllocated( 0 ), m_pBuffer( NULL )
	{

	}

	StackAllocator::StackAllocator( SIZE_T size )
		: m_marker( 0 ), m_bufferSize( size ), m_bytesAllocated( 0 )
	{
		m_pBuffer = PSX_Malloc( size );
		PSX_Assert( m_pBuffer, "Not enough memory." );
	}

	void StackAllocator::Reset( SIZE_T size )
	{
		Clear();
		PSX_Dealloc( m_pBuffer );
		m_bufferSize = size;
		m_pBuffer = PSX_Malloc( size );
		PSX_Assert( m_pBuffer, "Not enough memory." );
	}

	void * StackAllocator::Alloc( SIZE_T size )
	{
		if ( size > (m_bufferSize - m_bytesAllocated) )
			return NULL;

		void *pAllocMem = m_pBuffer + m_bytesAllocated;
		m_bytesAllocated += size;

		return pAllocMem;
	}

	// DoubleEndedStackAllocator
	DoubleEndedStackAllocator::DoubleEndedStackAllocator( SIZE_T size )
		: m_lowerMarker( 0 ), m_upperMarker( size ), m_bufferSize( size ), m_lowerBytesAllocated( 0 ), 
		m_upperBytesAllocated( 0 )
	{
		m_pBuffer = PSX_Malloc( size );

		PSX_Assert( m_pBuffer, "Out of memory." );
	}

	void * DoubleEndedStackAllocator::AllocLower( SIZE_T size )
	{
		if ( size > GetFreeMemory() )
			return NULL;

		void *pAllocMem = m_pBuffer + m_lowerBytesAllocated;
		m_lowerBytesAllocated += size;

		return pAllocMem;
	}

	void * DoubleEndedStackAllocator::AllocUpper( SIZE_T size )
	{
		if ( size > GetFreeMemory() )
			return NULL;

		m_upperBytesAllocated += size;
		return static_cast<void*>(GetEndBuffer() - m_upperBytesAllocated);
	}

	DoubleBufferedAllocator::DoubleBufferedAllocator( SIZE_T size )
		: m_currStack( 0 )
	{
		m_stacks[0].Reset( size );
		m_stacks[1].Reset( size );
	}

	// HeapAllocator
	const SIZE_T HeapAllocator::m_headerSize			= sizeof( BlockHeader );
	const SIZE_T HeapAllocator::m_footerSize			= sizeof( BlockFooter );
	const SIZE_T HeapAllocator::m_headerFooterSize		= sizeof( BlockHeader ) + sizeof( BlockFooter );

	#define GetFooter( _pHeader ) reinterpret_cast<BlockFooter*>(((BYTE*)(_pHeader)) + _pHeader->m_size - m_footerSize)
	#define GetMemory( _pHeader ) reinterpret_cast<void *>(((BYTE*)_pHeader) + m_pHeaderSize)

	void * HeapAllocator::Alloc( SIZE_T size )
	{
		SIZE_T totalReqSize = size + m_headerFooterSize;
		//BlockHeader *pHeader = (BlockHeader*)FirstFit( totalReqSize );
		BlockHeader *pHeader = (BlockHeader*)BestFit( totalReqSize );

		if ( pHeader == NULL )
			return NULL; // Out of memory

		BYTE *pAllocated = (BYTE*)pHeader;
		BlockFooter *pFooter = (BlockFooter*)(pAllocated + pHeader->m_size - m_footerSize);
		SIZE_T blockSize = pHeader->m_size;

		// Exact fit
		if ( blockSize == totalReqSize )
		{
		ConsumeWholeBlock:

			pHeader->m_bUsed = TRUE;
			pFooter->m_bUsed = TRUE;

			// Is this the last in the list?
			if ( pHeader->m_pNext == pHeader )
				m_pFreeList = NULL;
			else
			{
				pHeader->m_pPrev->m_pNext = pHeader->m_pNext;
				pHeader->m_pNext->m_pPrev = pHeader->m_pPrev;
				m_pFreeList = pHeader->m_pNext;
			}

			// Update available memory
			m_availableMemory -= (pHeader->m_size + m_headerFooterSize);

			return (void*)(pAllocated + m_headerSize);
		}

		// Size is greater than needed.
		// See if it can be split into two blocks.
		if ( blockSize >= totalReqSize + m_headerFooterSize )
		{
			// Split the block into two blocks ( one used, one free )
			BlockHeader *pUsedHeader = pHeader;
			BlockFooter *pUsedFooter = (BlockFooter*)(pAllocated + size + m_headerSize);
			BlockHeader *pFreeHeader = (BlockHeader*)((BYTE*)pUsedFooter + m_footerSize);
			BlockFooter *pFreeFooter = pFooter;

			pUsedHeader->m_bUsed = TRUE;
			pUsedFooter->m_bUsed = TRUE;
			pFreeHeader->m_bUsed = FALSE;
			pFreeFooter->m_bUsed = FALSE;

			// Updated blockSize to reflect the new splits
			pUsedHeader->m_size = totalReqSize;
			pUsedFooter->m_size = totalReqSize;
			pFreeHeader->m_size = blockSize - totalReqSize;
			pFreeFooter->m_size = blockSize - totalReqSize;

			if ( pHeader->m_pNext == pHeader )
			{
				pFreeHeader->m_pNext = pFreeHeader;
				pFreeHeader->m_pPrev = pFreeHeader;
			}
			else
			{
				pFreeHeader->m_pNext		= pHeader->m_pNext;
				pFreeHeader->m_pPrev		= pHeader->m_pPrev;
				pHeader->m_pPrev->m_pNext	= pFreeHeader;
				pHeader->m_pNext->m_pPrev	= pFreeHeader;
			}

			m_pFreeList = pFreeHeader;

			// Update available memory
			m_availableMemory -= pUsedHeader->m_size;

			return (void*)(pAllocated + m_headerSize);
		}

		// If we get here that means the block isn't big enough to split into two blocks
		goto ConsumeWholeBlock;  
	}

	void HeapAllocator::Free( void *pAddr )
	{
		if ( pAddr == NULL )
			return;

		BlockHeader *pHeader = (BlockHeader*)((BYTE*)pAddr - m_headerSize);
		BlockFooter *pFooter = (BlockFooter*)((BYTE*)pHeader + pHeader->m_size - m_footerSize);

		pHeader->m_bUsed = FALSE;
		pFooter->m_bUsed = FALSE;

		m_availableMemory += (pHeader->m_size - m_headerFooterSize);

		if ( m_pFreeList == NULL )
		{
			pHeader->m_pNext = pHeader;
			pHeader->m_pPrev = pHeader;
			m_pFreeList = pHeader;

			return;
		}

		// Check adjacent memory if free then coalesce

		BlockFooter *pLeftFooter = (BlockFooter*)((BYTE*)pHeader - m_footerSize);
		if ( (BYTE*)pHeader != m_pBuffer && pLeftFooter->m_bUsed == FALSE )
		{
			m_availableMemory += m_headerFooterSize;

			// Coalesce left memory block
			BlockHeader *pLeftHeader = (BlockHeader*)((BYTE*)pLeftFooter - pLeftFooter->m_size + m_footerSize);

			pLeftHeader->m_size += pHeader->m_size;
			pFooter->m_size = pLeftHeader->m_size;

			if ( pLeftHeader->m_pNext == pLeftHeader )
				return; // This is the only block left in the free list

			// Move to the next free block if freeblock points to this one
			if ( pLeftHeader == m_pFreeList )
				m_pFreeList = m_pFreeList->m_pPrev;

			// Remove from the free list for coalescing to the right block and adding back to the free list.
			pHeader = pLeftHeader;
			pHeader->m_pPrev->m_pNext = pHeader->m_pNext;
			pHeader->m_pNext->m_pPrev = pHeader->m_pPrev;
		}

		BlockHeader *pRightHeader = (BlockHeader*)((BYTE*)pHeader + pHeader->m_size);
		if ( (BYTE*)pRightHeader != m_pBufferEnd && pRightHeader->m_bUsed == FALSE )
		{
			m_availableMemory += m_headerFooterSize;

			BlockFooter *pRightFooter = (BlockFooter*)((BYTE*)pRightHeader + pRightHeader->m_size - m_footerSize);

			pHeader->m_size += pRightHeader->m_size;
			pRightFooter->m_size = pHeader->m_size;

			if ( pRightHeader->m_pNext == pRightHeader )
			{
				pHeader->m_pNext = pHeader;
				pHeader->m_pPrev = pHeader;
				m_pFreeList = pHeader;
				return;
			}

			// Move to the next free block if freeblock points to this one
			if ( pRightHeader == m_pFreeList )
				m_pFreeList = pRightHeader->m_pNext;

			// Set this up for adding back to the free list
			pRightHeader->m_pPrev->m_pNext = pRightHeader->m_pNext;
			pRightHeader->m_pNext->m_pPrev = pRightHeader->m_pPrev;
		}

		// Add back to the free list
		pHeader->m_pPrev = m_pFreeList->m_pPrev;
		pHeader->m_pNext = m_pFreeList;
		m_pFreeList->m_pPrev->m_pNext = pHeader;
		m_pFreeList->m_pPrev = pHeader;
		m_pFreeList = pHeader;
	}

	BOOL HeapAllocator::Initialize( SIZE_T bufferSize )
	{
		SIZE_T allocSize = bufferSize < m_headerFooterSize ? m_headerFooterSize : bufferSize ;
		BlockFooter *pFooter;

		// Make sure we don't leak any resources.
		Release();

		m_pBuffer = PSX_Malloc( allocSize );
		PSX_Assert( m_pBuffer, "Out of memory." );
		m_pBufferEnd = m_pBuffer + allocSize;

		m_bufferSize		= allocSize;
		m_availableMemory	= allocSize - m_headerFooterSize;
		m_pFreeList			= reinterpret_cast<BlockHeader*>(m_pBuffer);

		m_pFreeList->m_bUsed	= FALSE;
		m_pFreeList->m_size		= allocSize;
		m_pFreeList->m_pNext	= m_pFreeList;
		m_pFreeList->m_pPrev	= m_pFreeList;

		pFooter				= (BlockFooter*)((BYTE*)m_pFreeList + m_pFreeList->m_size - m_footerSize);
		pFooter->m_bUsed	= FALSE;
		pFooter->m_size		= allocSize;

		return TRUE;
	}

	void HeapAllocator::Release( void )
	{
		m_bufferSize		= 0;
		m_availableMemory	= 0;

		PSX_Dealloc( m_pBuffer );	
		m_pBuffer = NULL;
		m_pFreeList = NULL;
		m_pBufferEnd = NULL;
	}


	void * HeapAllocator::FirstFit( SIZE_T size )
	{
		SIZE_T reqSize = size;
		BlockHeader *pHeader = m_pFreeList;

		if ( pHeader == NULL )
			return NULL;

		do
		{
			if ( pHeader->m_size >= reqSize )
				return (void*)pHeader;

			pHeader = pHeader->m_pNext;

		} while( pHeader != m_pFreeList );

		// If we get here, that means we don't have a big enough size.
		return NULL;
	}

	void * HeapAllocator::BestFit( SIZE_T size )
	{
		// Find the smallest possible size
		SIZE_T reqSize			= size;
		BlockHeader *pHeader	= m_pFreeList;
		BlockHeader *pCandidate = NULL;

		if ( pHeader == NULL )
			return NULL;

		do
		{
			if ( pHeader->m_size >= reqSize )
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

		} while( pHeader != m_pFreeList );

		// If pCandidate is not null, then we have an available size. Just not an exact fit.
		return pCandidate;
	}

#undef GetFooter
#undef GetMemory

	// PackedAllocator
	PackedAllocator::PackedAllocator( SIZE_T size, SIZE_T numTableEnties /*= 1024*/ )
		: m_pAllocList(NULL)
	{
		PSX_Validate( Initialize( size, numTableEnties ), "Failed to initialize PackedAllocator." );
	}

	PackedAllocator::~PackedAllocator( void )
	{
		Release();
	}

	void PackedAllocator::Defrag( const UINT count /*= 0*/ )
	{
		if ( !m_pAllocList )
			return;

		AllocEntry *pEntry = m_pAllocList;
		BlockHeader *pCurrHeader;
		BlockHeader *pPrevHeader;
		BlockFooter *pPrevFooter;
		BlockFooter *pNewUsedFooter;
		BlockHeader	*pNewFreeHeader;
		BlockFooter *pNewFreeFooter;
		SIZE_T usedBlockSize;
		SIZE_T freeBlockSize;
		SIZE_T loop = count ? -1 : count;

		for( SIZE_T i = 0; i < count; ++i )
		{
			pCurrHeader = (BlockHeader*)((BYTE*)pEntry->m_pAddress - HeapAllocator::m_headerSize);

			if ( pCurrHeader == (BlockHeader*)m_heap.m_pBuffer )
				goto NextEntry;

			pPrevFooter = (BlockFooter*)((BYTE*)pCurrHeader - HeapAllocator::m_footerSize);

			if ( pPrevFooter->m_bUsed )
				goto NextEntry;

			// Switch memory blocks

			pPrevHeader		= (BlockHeader*)((BYTE*)pPrevFooter - pPrevFooter->m_size + HeapAllocator::m_footerSize);
			pNewFreeFooter	= (BlockFooter*)((BYTE*)pCurrHeader + pCurrHeader->m_size - HeapAllocator::m_footerSize);

			usedBlockSize = pCurrHeader->m_size;
			freeBlockSize = pPrevFooter->m_size;

			// Move memory
			memcpy( (BYTE*)pPrevHeader + HeapAllocator::m_headerSize, (BYTE*)pCurrHeader + HeapAllocator::m_headerSize, usedBlockSize - HeapAllocator::m_headerFooterSize );

			// Then fix data info (the switched free and used block)
			pPrevHeader->m_bUsed	= TRUE;
			pPrevHeader->m_size		= usedBlockSize;
			pNewUsedFooter			= (BlockFooter*)((BYTE*)pPrevHeader + usedBlockSize - HeapAllocator::m_footerSize);
			pNewUsedFooter->m_bUsed = TRUE;
			pNewUsedFooter->m_size	= usedBlockSize;

			pNewFreeHeader			= (BlockHeader*)((BYTE*)pNewUsedFooter + HeapAllocator::m_footerSize);
			pNewFreeHeader->m_bUsed = FALSE;
			pNewFreeHeader->m_size	= freeBlockSize;
			pNewFreeFooter->m_bUsed = FALSE;
			pNewFreeFooter->m_size	= freeBlockSize;

			// Fix pointers
			if ( pPrevHeader->m_pPrev == pPrevHeader )
			{
				pNewFreeHeader->m_pPrev = pNewFreeHeader;
				pNewFreeHeader->m_pNext = pNewFreeHeader;
				m_heap.m_pFreeList = pNewFreeHeader;
			}
			else
			{
				pNewFreeHeader->m_pPrev = pPrevHeader->m_pPrev;
				pNewFreeHeader->m_pNext = pPrevHeader->m_pNext;

				pPrevHeader->m_pPrev->m_pNext = pNewFreeHeader;
				pPrevHeader->m_pNext->m_pPrev = pNewFreeHeader;

				// Make sure freelist doesn't get lost
				if ( pPrevHeader == m_heap.m_pFreeList )
					m_heap.m_pFreeList = pPrevHeader->m_pPrev;
			}

			// Update alloc entry's new address
			pEntry->m_pAddress = (BYTE*)pPrevHeader + HeapAllocator::m_headerSize;

		NextEntry:
			pEntry = pEntry->m_pNext;

			// We've finished the entire list
			if ( pEntry == m_pAllocList )
				return;
		}

		// Save last pointer so we don't have to  start from the top
		m_pAllocList = pEntry;
	}

	PackedAllocator::HPointer< void > PackedAllocator::Alloc( SIZE_T size )
	{
		HPointer< void > hPtr( reinterpret_cast<AllocEntry*>(m_pooltable.Alloc()) );
		PSX_Assert( hPtr.m_pAllocEntry, "m_pooltable out of memory." );
		
		PSX_Validate( (hPtr.m_pAllocEntry->m_pAddress = RawAlloc( size )) != PSX_NULL, "PackedAllocator out of memory." );

		if ( hPtr.m_pAllocEntry )
		{
			if ( m_pAllocList ) // If not NULL
			{
				hPtr.m_pAllocEntry->m_pPrev		= m_pAllocList->m_pPrev;
				hPtr.m_pAllocEntry->m_pNext		= m_pAllocList;
				m_pAllocList->m_pPrev->m_pNext	= hPtr.m_pAllocEntry;
				m_pAllocList->m_pPrev			= hPtr.m_pAllocEntry;
				m_pAllocList					= hPtr.m_pAllocEntry;
				return hPtr;
			}
			else
			{
				hPtr.m_pAllocEntry->m_pPrev = hPtr.m_pAllocEntry;
				hPtr.m_pAllocEntry->m_pNext = hPtr.m_pAllocEntry;
				m_pAllocList				= hPtr.m_pAllocEntry;
				return hPtr;
			}
		}

		return NULL;
	}

	BOOL PackedAllocator::Initialize( SIZE_T size, SIZE_T numTableEnties )
	{
		PSX_Assert( numTableEnties, "numTableEntries can't be zero." );

		if ( numTableEnties )
		{
			// Make sure we don't leak any resources.
			Release();

			SIZE_T allocSize = size < HeapAllocator::m_headerFooterSize ? HeapAllocator::m_headerFooterSize : size;

			m_pooltable.Reset( numTableEnties );

			m_heap.Initialize( allocSize );

			return TRUE;
		}

		return FALSE;
	}

}