/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	MemoryManager.h
*
*	Description -	Different types of allocators to fit your needs!
*
*	Comments	- StackAllocator, DoubleEndedStackAllocator, PoolAllocator, FixedBlockAllocator,
*					DoubleBufferedAllocator, PackedAllocator.
*				  FixedBlockAllocator is almost exactly the same as pool allocator but only accepts
*					size(blockSize) instead of type(T).
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	01/23/2010	-	Added StackAllocator and DoubleEndedStackAllocator.
*			MrCodeSushi	-	01/25/2010	-	Added PoolAllocator and DoubleBufferedAllocator.
*			MrCodeSushi	-	01/28/2010	-	Added PackedAllocator(defragment support).
*			MrCodeSushi	-	02/03/2010	-	Added FixedSizeBlockAllocator and HeapAllocator
**************************************************************************************/

#ifndef _PSX_ALLOCATORS_H_
#define _PSX_ALLOCATORS_H_

#include "PulseSTD.h"
#include "MemoryManager.h"
#include "PTLBase.h"

namespace Pulse
{
	class StackAllocator
	{
	public:

		typedef SIZE_T Marker;

		StackAllocator( void );

		explicit StackAllocator( SIZE_T size );

		~StackAllocator( void );

		void Reset( SIZE_T size );

		void * Alloc( SIZE_T size );

		void PinMarker( void );

		Marker GetMarker( void );

		SIZE_T GetFreeMemory( void );

		void FreeToMarker( void );

		void Clear();

		BOOL IsAddressOwned( void *pAddr );

	private:

		StackAllocator( const StackAllocator & );
		StackAllocator & operator = ( const StackAllocator & );

		Marker	m_marker;
		SIZE_T	m_bufferSize;
		SIZE_T	m_bytesAllocated;
		BYTE	*m_pBuffer;
	};


	PSX_INLINE StackAllocator::~StackAllocator( void )
	{
		PSX_Dealloc( m_pBuffer );
	}

	PSX_INLINE void StackAllocator::PinMarker( void )
	{
		m_marker = m_bytesAllocated;
	}

	PSX_INLINE StackAllocator::Marker StackAllocator::GetMarker( void )
	{
		return m_marker;
	}

	PSX_INLINE SIZE_T StackAllocator::GetFreeMemory( void )
	{
		return m_bufferSize - m_bytesAllocated;
	}

	PSX_INLINE void StackAllocator::FreeToMarker( void )
	{
		m_bytesAllocated = m_marker;
	}

	PSX_INLINE void StackAllocator::Clear()
	{
		m_marker = m_bytesAllocated = 0;
	}

	PSX_INLINE BOOL StackAllocator::IsAddressOwned( void *pAddr )
	{
		return (pAddr >= m_pBuffer) && pAddr < (m_pBuffer + m_bufferSize);
	}


	class DoubleEndedStackAllocator
	{
	public:

		typedef SIZE_T Marker;

		explicit DoubleEndedStackAllocator( SIZE_T size );
		~DoubleEndedStackAllocator( void );

		void * AllocLower( SIZE_T size );
		void * AllocUpper( SIZE_T size );

		void PinLowerMarker( void );
		void PinUpperMarker( void );

		Marker GetLowerMarker( void );
		Marker GetUpperMarker( void );

		SIZE_T GetFreeMemory( void );

		void FreeToMarkerLower( void );
		void FreeToMarkerUpper( void );

		void ClearLower( void );
		void ClearUpper( void );
		void Clear( void );

		PSX_INLINE BOOL IsAddressOwned( void *pAddr )
		{
			return (pAddr >= m_pBuffer) && pAddr < (m_pBuffer + m_bufferSize);
		}

	private:

		BYTE * GetEndBuffer( void );

		DoubleEndedStackAllocator( const DoubleEndedStackAllocator & );
		DoubleEndedStackAllocator & operator = ( const DoubleEndedStackAllocator & );

		Marker	m_lowerMarker;
		Marker	m_upperMarker;
		SIZE_T	m_bufferSize;
		SIZE_T	m_lowerBytesAllocated;
		SIZE_T	m_upperBytesAllocated;
		BYTE	*m_pBuffer;

	};

	PSX_INLINE DoubleEndedStackAllocator::~DoubleEndedStackAllocator( void )
	{
		PSX_Dealloc( m_pBuffer );
	}

	PSX_INLINE SIZE_T DoubleEndedStackAllocator::GetFreeMemory( void )
	{
		return ( m_lowerBytesAllocated + m_upperBytesAllocated ) - m_bufferSize;
	}

	PSX_INLINE BYTE * DoubleEndedStackAllocator::GetEndBuffer( void )
	{
		return m_pBuffer + m_bufferSize;
	}

	PSX_INLINE void DoubleEndedStackAllocator::PinLowerMarker( void )
	{
		m_lowerMarker = m_lowerBytesAllocated;
	}

	PSX_INLINE void DoubleEndedStackAllocator::PinUpperMarker( void )
	{
		m_upperMarker = m_upperBytesAllocated;
	}

	PSX_INLINE DoubleEndedStackAllocator::Marker DoubleEndedStackAllocator::GetLowerMarker( void )
	{
		return m_lowerMarker;
	}
	
	PSX_INLINE DoubleEndedStackAllocator::Marker DoubleEndedStackAllocator::GetUpperMarker( void )
	{
		return m_upperMarker;
	}

	PSX_INLINE void DoubleEndedStackAllocator::FreeToMarkerLower( void )
	{
		m_lowerBytesAllocated = m_lowerMarker;
	}

	PSX_INLINE void DoubleEndedStackAllocator::FreeToMarkerUpper( void )
	{
		m_upperBytesAllocated = m_upperMarker;
	}

	PSX_INLINE void DoubleEndedStackAllocator::ClearLower( void )
	{
		m_lowerMarker = m_lowerBytesAllocated = 0;
	}

	PSX_INLINE void DoubleEndedStackAllocator::ClearUpper( void )
	{
		m_upperMarker = m_upperBytesAllocated = 0;
	}

	PSX_INLINE void DoubleEndedStackAllocator::Clear( void )
	{
		ClearLower();
		ClearUpper();
	}

	/* DoubleBufferedAllocator */
	class DoubleBufferedAllocator
	{
	public:

		explicit DoubleBufferedAllocator( SIZE_T size );

		void SwapBuffers( void );

		void ClearCurrentBuffer( void );

		void * Alloc( SIZE_T size );

		PSX_INLINE BOOL IsAddressOwned( void *pAddr )
		{
			return (m_stacks[0].IsAddressOwned( pAddr )) || (m_stacks[1].IsAddressOwned( pAddr ));
		}

	private:

		U32				m_currStack;
		StackAllocator	m_stacks[2];
	};

	PSX_INLINE void DoubleBufferedAllocator::SwapBuffers( void )
	{
		m_currStack = !m_currStack;
	}

	PSX_INLINE void DoubleBufferedAllocator::ClearCurrentBuffer( void )
	{
		m_stacks[m_currStack].Clear();
	}

	PSX_INLINE void * DoubleBufferedAllocator::Alloc( SIZE_T size )
	{
		return m_stacks[m_currStack].Alloc( size );
	}

	/* PoolAllocator */
	template < typename T, UINT alignment = 1 >
	class PoolAllocator
	{
	public:

		PoolAllocator( void );
		
		explicit PoolAllocator( SIZE_T numObj );

		~PoolAllocator( void );

		BOOL Initialize( SIZE_T numObj );

		void Reset( SIZE_T numObj );

		T * Alloc( void );

		void Free( T & obj );
		void Free( T * obj );

		void Clear( void );

		PSX_INLINE BOOL IsAddressOwned( void *pAddr )
		{
			return (pAddr >= m_pPool) && pAddr < (m_pPool + (m_sizeOffset * m_numObj));
		}

	private:

		void Release( void );

		PoolAllocator( const PoolAllocator & );
		PoolAllocator & operator = ( const PoolAllocator & );

		struct FreeNode
		{
			FreeNode *m_pNext;
		};

		SIZE_T		m_sizeOffset;
		SIZE_T		m_numObj;
		PTR_T		m_pPool;
		FreeNode	*m_pFreeList;
	};

	template < typename T, UINT alignment >
	PoolAllocator< T, alignment >::PoolAllocator( void )
		: m_sizeOffset( 0 ), m_numObj(0), m_pPool( NULL ), m_pFreeList( NULL )
	{
	}
	
	template < typename T, UINT alignment >
	PoolAllocator< T, alignment >::PoolAllocator( SIZE_T numObj )
		: m_sizeOffset( 0 ), m_numObj(numObj), m_pPool( NULL ), m_pFreeList( NULL )
	{
		PSX_Validate( Initialize( numObj ), "Failed to initialize PoolAllocator." );
	}

	template < typename T, UINT alignment >
	PSX_INLINE PoolAllocator< T, alignment >::~PoolAllocator( void )
	{
		(alignment == 1) ? PSX_Dealloc( m_pPool ) : PSX_DeallocAligned( m_pPool );
	}

	template < typename T, UINT alignment >
	PSX_INLINE void PoolAllocator< T, alignment >::Reset( SIZE_T numObj )
	{
		Release();
		Initialize( numObj );
	}

	template < typename T, UINT alignment >
	T * PoolAllocator< T, alignment >::Alloc( void )
	{
		if ( !m_pFreeList )
			return NULL;

		T *pNewAlloc = reinterpret_cast<T*>(m_pFreeList);
		m_pFreeList = m_pFreeList->m_pNext;

		new (pNewAlloc) T;
		return pNewAlloc;
	}

	template < typename T, UINT alignment >
	PSX_INLINE void PoolAllocator< T, alignment >::Free( T & obj )
	{
		Free( &obj );
	}

	template < typename T, UINT alignment >
	PSX_INLINE void PoolAllocator< T, alignment >::Free( T * pObj )
	{
		pObj->~T();
		FreeNode *pNode = reinterpret_cast< FreeNode * >(pObj);
		pNode->m_pNext = m_pFreeList;
		m_pFreeList = pNode;
	}

	template < typename T, UINT alignment >
	void PoolAllocator< T, alignment >::Clear( void )
	{
		// Store objects in free list
		BYTE *pPoolByte = reinterpret_cast< BYTE* >(m_pPool);
		FreeNode *pCurrNode;
		m_pFreeList = NULL;
		for ( SIZE_T i = 0; i < m_numObj; ++i )
		{
			pCurrNode = reinterpret_cast<FreeNode*>(pPoolByte + (m_sizeOffset * i));
			pCurrNode->m_pNext = m_pFreeList;
			m_pFreeList = pCurrNode;
		}
	}

	template < typename T, UINT alignment >
	BOOL PoolAllocator< T, alignment >::Initialize( SIZE_T numObj )
	{
		Release();

		PSX_Assert( numObj, "Can't have 0 number of objects." );

		if ( numObj == 0 )
			return FALSE;

		m_sizeOffset		= sizeof(T) > sizeof(PVOID) ? sizeof(T) : sizeof(PVOID);
		SIZE_T totalSize	= m_sizeOffset * numObj;
		m_numObj			= numObj;
		m_pPool				= ((alignment == 1) ? PSX_Malloc(totalSize) : PSX_MallocAligned(totalSize, alignment));

		PSX_Assert( m_pPool, "Out of memory." );

		if ( !m_pPool )
		{
			Release();
			return FALSE;
		}

		Clear(); // Reset free list

		return TRUE;
	}

	template < typename T, UINT alignment >
	void PoolAllocator< T, alignment >::Release( void )
	{
		(alignment == 1) ? PSX_Dealloc( m_pPool ) : PSX_DeallocAligned( m_pPool );
		m_pPool = NULL;
		m_pFreeList = NULL;
	}

	/* FixedBlockAllocator */
	template < SIZE_T blockSize, UINT alignment = 1 >
	class FixedBlockAllocator
	{
	public:

		FixedBlockAllocator( void );

		explicit FixedBlockAllocator( SIZE_T numObj );

		~FixedBlockAllocator( void );

		BOOL Initialize( SIZE_T numObj );

		void Reset( SIZE_T numObj );

		void * Alloc( void );

		void Free( void * pAddr );

		void Clear( void );

		PSX_INLINE BOOL IsAddressOwned( void *pAddr )
		{
			return (pAddr >= m_pPool) && pAddr < ((BYTE*)m_pPool + (m_sizeOffset * m_numObj));
		}

	private:

		void Release( void );

		FixedBlockAllocator( const FixedBlockAllocator & );
		FixedBlockAllocator & operator = ( const FixedBlockAllocator & );

		struct FreeNode
		{
			FreeNode *m_pNext;
		};

		SIZE_T		m_sizeOffset;
		SIZE_T		m_numObj;
		PTR_T		m_pPool;
		FreeNode	*m_pFreeList;
	};

	template < SIZE_T blockSize, UINT alignment >
	FixedBlockAllocator< blockSize, alignment >::FixedBlockAllocator( void )
		: m_sizeOffset( 0 ), m_numObj(0), m_pPool( NULL ), m_pFreeList( NULL )
	{
	}

	template < SIZE_T blockSize, UINT alignment >
	FixedBlockAllocator< blockSize, alignment >::FixedBlockAllocator( SIZE_T numObj )
		: m_sizeOffset( 0 ), m_numObj(numObj), m_pPool( NULL ), m_pFreeList( NULL )
	{
		PSX_Validate( Initialize( numObj ), "Failed to initialize FixedBlockAllocator." );
	}

	template < SIZE_T blockSize, UINT alignment >
	PSX_INLINE FixedBlockAllocator< blockSize, alignment >::~FixedBlockAllocator( void )
	{
		(alignment == 1) ? PSX_Dealloc( m_pPool ) : PSX_DeallocAligned( m_pPool );
	}

	template < SIZE_T blockSize, UINT alignment >
	PSX_INLINE void FixedBlockAllocator< blockSize, alignment >::Reset( SIZE_T numObj )
	{
		Initialize( numObj );
	}

	template < SIZE_T blockSize, UINT alignment >
	void * FixedBlockAllocator< blockSize, alignment >::Alloc( void )
	{
		if ( !m_pFreeList )
			return NULL;

		void *pNewAlloc = m_pFreeList;
		m_pFreeList = m_pFreeList->m_pNext;

		return pNewAlloc;
	}

	template < SIZE_T blockSize, UINT alignment >
	PSX_INLINE void FixedBlockAllocator< blockSize, alignment >::Free( void * pAddr )
	{
		FreeNode *pNode = reinterpret_cast< FreeNode * >(pAddr);
		pNode->m_pNext = m_pFreeList;
		m_pFreeList = pNode;
	}

	template < SIZE_T blockSize, UINT alignment >
	void FixedBlockAllocator< blockSize, alignment >::Clear( void )
	{
		// Store objects in free list
		BYTE *pPoolByte = reinterpret_cast< BYTE* >(m_pPool);
		FreeNode *pCurrNode;
		m_pFreeList = NULL;
		for ( SIZE_T i = 0; i < m_numObj; ++i )
		{
			pCurrNode = reinterpret_cast<FreeNode*>(pPoolByte + (m_sizeOffset * i));
			pCurrNode->m_pNext = m_pFreeList;
			m_pFreeList = pCurrNode;
		}
	}

	template < SIZE_T blockSize, UINT alignment >
	BOOL FixedBlockAllocator< blockSize, alignment >::Initialize( SIZE_T numObj )
	{
		Release();

		PSX_Assert( numObj, "Can't have 0 number of objects." );
		if ( numObj == 0 )
			return FALSE;

		m_sizeOffset		= blockSize > sizeof(PVOID) ? blockSize : sizeof(PVOID);
		SIZE_T totalSize	= m_sizeOffset * numObj;
		m_numObj			= numObj;
		m_pPool				= ((alignment == 1) ? PSX_Malloc(totalSize) : PSX_MallocAligned(totalSize, alignment));

		PSX_Assert( m_pPool, "Out of memory." );

		if ( m_pPool == PSX_NULL )
		{
			Release();
			return FALSE;
		}

		// Reset free list
		Clear();

		return TRUE;
	}

	template < SIZE_T blockSize, UINT alignment >
	void FixedBlockAllocator< blockSize, alignment >::Release( void )
	{
		(alignment == 1) ? PSX_Dealloc( m_pPool ) : PSX_DeallocAligned( m_pPool );
		m_pPool = NULL;
		m_pFreeList = NULL;
	}

	// General-purpose heap allocator
	class HeapAllocator
	{
		struct BlockHeader
		{
			BOOL		m_bUsed;
			SIZE_T		m_size;
			BlockHeader *m_pNext;
			BlockHeader *m_pPrev;
		};

		struct BlockFooter
		{
			BOOL	m_bUsed;
			SIZE_T	m_size;
		};

	public:

		HeapAllocator( void );

		HeapAllocator( SIZE_T bufferSize );

		~HeapAllocator( void );

		BOOL Initialize( SIZE_T bufferSize );

		void * Alloc( SIZE_T size );

		void Free( void *pAddr );

		const SIZE_T GetAvailableMemory( void ) const;
		const SIZE_T GetTotalAllocatobleMemory( void ) const;
		const SIZE_T GetBufferSize( void ) const;

		PSX_INLINE BOOL IsAddressOwned( void *pAddr )
		{
			return (pAddr >= m_pBuffer) && pAddr < (m_pBuffer + m_bufferSize);
		}

	private:

		void * FirstFit( SIZE_T size );
		void * BestFit( SIZE_T size ); 

		void Release( void );

		friend class PackedAllocator;

		static const SIZE_T	m_headerSize;
		static const SIZE_T	m_footerSize;
		static const SIZE_T	m_headerFooterSize;

		SIZE_T		m_bufferSize;
		SIZE_T		m_availableMemory;
		BlockHeader *m_pFreeList;
		BYTE		*m_pBuffer;
		BYTE		*m_pBufferEnd;

	};

	PSX_INLINE HeapAllocator::HeapAllocator( void )
		: m_bufferSize( 0 ), m_availableMemory( 0 ), m_pFreeList( NULL ),
		m_pBuffer( NULL ), m_pBufferEnd( NULL )
	{

	}

	PSX_INLINE HeapAllocator::HeapAllocator( SIZE_T bufferSize )
		: m_bufferSize( bufferSize ), m_availableMemory( 0 ), m_pFreeList( NULL ),
		m_pBuffer( NULL ), m_pBufferEnd( NULL )
	{
		PSX_Validate( Initialize( bufferSize ), "Failed to initialize HeapAllocator." );
	}

	PSX_INLINE HeapAllocator::~HeapAllocator( void )
	{
		Release();
	}

	PSX_INLINE const SIZE_T HeapAllocator::GetAvailableMemory( void ) const
	{
		return m_availableMemory;
	}

	PSX_INLINE const SIZE_T HeapAllocator::GetTotalAllocatobleMemory( void ) const
	{
		return (m_bufferSize - m_headerFooterSize);
	}

	PSX_INLINE const SIZE_T HeapAllocator::GetBufferSize( void ) const
	{
		return m_bufferSize;
	}

	// Allocator that defrags memory to prevent fragmentation
	class PackedAllocator
	{
	private:

		struct AllocEntry
		{
			//explicit AllocEntry( void ) { }

			PTR_T		m_pAddress;
			AllocEntry	*m_pPrev;
			AllocEntry	*m_pNext;
		};

		typedef HeapAllocator::BlockHeader BlockHeader;
		typedef HeapAllocator::BlockHeader BlockFooter;

	public:

		template < typename T >
		class HPointer
		{
		public:

			HPointer( AllocEntry * const pEntry ) : m_pAllocEntry( pEntry ) { }
			HPointer( HPointer< void > &hPtr ) { *this = hPtr; }
			HPointer( void ) : m_pAllocEntry( NULL ) { }

			// WARNING : Do not store the returned pointer. Only use it temporarily.
			//	The data may have been moved to a different address since the last frame.
			T * GetTempPtr( void ) { return m_pAllocEntry ? reinterpret_cast<T*>(m_pAllocEntry->m_pAddress) : NULL; }

			HPointer & operator = ( HPointer<void> &hPtr )
			{
				m_pAllocEntry = hPtr.m_pAllocEntry;
				return *this;
			}

		private:

			friend PackedAllocator;

			AllocEntry		*m_pAllocEntry;
		};

		PackedAllocator( SIZE_T size, SIZE_T numTableEnties = 1024 );

		~PackedAllocator( void );

		HPointer< void > Alloc( SIZE_T size );

		template < typename T >
		void Free( HPointer< T > &HPointer );
		template < typename T >
		void Free( HPointer< T > *pHPointer );

		void Defrag( const UINT count = 0 );

		const SIZE_T GetAvailableMemory( void ) const;
		const SIZE_T GetTotalAllocatobleMemory( void ) const;
		const SIZE_T GetBufferSize( void ) const;

	private:

		void * RawAlloc( SIZE_T size );

		void RawDealloc( void *pAddr );

		void * FirstFit( SIZE_T size );

		BOOL Initialize( SIZE_T size, SIZE_T numTableEnties );

		void Release( void );

		FixedBlockAllocator< sizeof(AllocEntry) > m_pooltable;
		AllocEntry	*m_pAllocList;
		
		HeapAllocator m_heap;
	};

	template < typename T >
	PSX_FORCE_INLINE void PackedAllocator::Free( HPointer< T > &HPointer )
	{
		Free( &HPointer );
	}

	template < typename T >
	void PackedAllocator::Free( HPointer< T > *pHPointer )
	{
		AllocEntry *pEntry = pHPointer->m_pAllocEntry;
		PSX_Assert( pEntry->m_pAddress, "Invalid pHPointer internal data address. (pEntry->m_pAddress == NULL)" );

		RawDealloc( pEntry->m_pAddress );

		if ( pEntry->m_pPrev == pEntry )
		{
			m_pAllocList = NULL;
		}
		else
		{
			if ( pEntry == m_pAllocList )
				m_pAllocList = pEntry->m_pNext;

			if ( pEntry->m_pPrev )
				pEntry->m_pPrev->m_pNext = pEntry->m_pNext;

			if ( pEntry->m_pNext )
				pEntry->m_pNext->m_pPrev = pEntry->m_pPrev;
		}

		m_pooltable.Free( reinterpret_cast<void*>(pEntry) );

		// NOTE: We may want to trash the data in pHPointer in debug mode...
		# if( PSX_DEBUG )
			pHPointer->m_pAllocEntry = NULL;
		#endif /*if(PSX_DEBUG )*/
	}
	
	PSX_INLINE const SIZE_T PackedAllocator::GetAvailableMemory( void ) const
	{
		return m_heap.GetAvailableMemory();
	}

	PSX_INLINE const SIZE_T PackedAllocator::GetTotalAllocatobleMemory( void ) const
	{
		return m_heap.GetTotalAllocatobleMemory();
	}

	PSX_INLINE const SIZE_T PackedAllocator::GetBufferSize( void ) const
	{
		return m_heap.GetBufferSize();
	}

	PSX_INLINE void PackedAllocator::Release( void )
	{
		m_pooltable.Clear();
		m_pAllocList		= NULL;
	}

	PSX_INLINE void * PackedAllocator::RawAlloc( SIZE_T size )
	{
		return m_heap.Alloc( size );
	}

	PSX_INLINE void PackedAllocator::RawDealloc( void *pAddr )
	{
		return m_heap.Free( pAddr );
	}
}

#endif /* _PSX_ALLOCATORS_H_ */