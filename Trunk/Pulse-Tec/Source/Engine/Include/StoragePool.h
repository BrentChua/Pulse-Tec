/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	StoragePool.h
*
*	Description -	Resource management. Note that a storage pool is used to manage
*						usuable game data(i.e. video textures, mesh, vertex buffers, etc.)
*						while Resource cache manages "raw" data from disk source.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	04/02/2010	-	Creation of this file.
**************************************************************************************/

#ifndef _PSX_STORAGE_POOL_H_
#define _PSX_STORAGE_POOL_H_

#include "PulseSTD.h"
#include "MemoryManager.h"
#include "Math.h"
#include "List.h"

namespace Pulse
{
	// Resource Pool Type definitions
	#define INVALID_RESOURCE_HANDLE 0xFFFFFFFF 
	typedef SIZE_T HRes;

	// Resource group is like a pool allocator only it acts more like an array.
	template < typename T >
	class Storage
	{
	public:

		enum { 
			INVALID_HANDLE = 0xFFFFFFFF 
		};

		Storage( void );
		Storage( SIZE_T resCountSize );
		~Storage( void );

		void Create( SIZE_T resCountSize );
		void Destroy( void );

		HRes Add( const T &res );
		void Free( HRes hRes );

		const HRes NextHandle( void );

		SIZE_T		GetSize( void ) const;
		SIZE_T		GetNumFree( void ) const;
		SIZE_T		GetNumUsed( void ) const;
		const HRes	GetNextHandle( void ) const;
		BOOL		IsOpen( HRes hRes ) const;

		T &			GetMember( HRes hRes );
		const T &	GetMember( HRes hRes ) const;
		T *			GetMemberPtr( HRes hRes );
		const T *	GetMemberPtr( HRes hRes ) const;

	private:

		BOOL	m_bInitialized;
		SIZE_T	m_numOpen;
		SIZE_T	m_numSlots;
		SIZE_T	m_nextOpen;
		SIZE_T	*m_pNextOpenList;
		T		*m_pResourceList;

	};

	template < typename T >
	PSX_INLINE Storage<T>::Storage( void )
	{
		PSX_ZeroMem( this, sizeof(Storage<T>) );
	}

	template < typename T >
	PSX_INLINE Storage<T>::Storage( SIZE_T resCountSize )
	{
		PSX_ZeroMem( this, sizeof(Storage) );
		Create( resCountSize );
	}

	template < typename T >
	PSX_INLINE Storage< T >::~Storage( void )
	{
		Destroy();
	}

	template < typename T >
	void Storage<T>::Create( SIZE_T resCountSize )
	{
		PSX_Assert( resCountSize, "Can't have 0 number of resource slots." );
		Destroy();

		m_numOpen	= resCountSize;
		m_numSlots	= resCountSize;
		m_nextOpen	= 0;

		m_pNextOpenList =  PSX_MallocAligned( m_numSlots * sizeof(SIZE_T), 32 );
		PSX_Assert( m_pNextOpenList, "Failed to allocate memory." );

		m_pResourceList = new T[m_numSlots];
		PSX_Assert( m_pResourceList, "Failed to allocate memory." );

		SIZE_T i = 0;
		for( ; i < m_numSlots-1; ++i )
			m_pNextOpenList[i] = i + 1;

		m_pNextOpenList[i] = i;
		m_bInitialized = TRUE;
	}

	template < typename T >
	PSX_INLINE void Storage<T>::Destroy( void )
	{
		PSX_SafeDeleteArray( m_pResourceList );
		PSX_SafeDeallocAligned( m_pNextOpenList );
		m_nextOpen = m_numSlots = m_numOpen = 0;
		m_bInitialized = FALSE;
	}

	template < typename T >
	PSX_INLINE HRes Storage<T>::Add( const T &res )
	{
		HRes slot = NextHandle();
		m_pResourceList[ slot ] = res;

		return slot;
	}

	template < typename T >
	PSX_INLINE void Storage<T>::Free( HRes hRes )
	{
		PSX_Assert( !IsOpen( hRes ), "Invalid index to release." );

		m_pNextOpenList[hRes] = m_numOpen ? m_nextOpen : hRes;
		++m_numOpen;
		m_nextOpen = hRes;
	}

	template < typename T >
	PSX_INLINE const HRes Storage<T>::NextHandle( void )
	{
		HRes slot = m_nextOpen;
		m_nextOpen = m_pNextOpenList[m_nextOpen];
		--m_numOpen;

		PSX_Assert( IsOpen( slot ), "Invalid index." );

		m_pNextOpenList[slot] = INVALID_HANDLE;
		return slot;
	}

	template < typename T >
	PSX_INLINE SIZE_T Storage<T>::GetSize( void ) const
	{
		return m_numSlots;
	}

	template < typename T >
	PSX_INLINE SIZE_T Storage<T>::GetNumFree( void ) const
	{
		return m_numOpen;
	}

	template < typename T >
	PSX_INLINE SIZE_T Storage<T>::GetNumUsed( void ) const
	{
		return m_numSlots - m_numOpen;
	}

	template < typename T >
	PSX_INLINE const HRes Storage<T>::GetNextHandle( void ) const
	{
		return m_nextOpen;
	}

	template < typename T >
	PSX_INLINE BOOL Storage<T>::IsOpen( HRes hRes ) const
	{
		PSX_Assert( hRes < m_numSlots, "Invalid index." );
		return m_pNextOpenList[hRes] != INVALID_HANDLE;
	}

	template < typename T >
	PSX_INLINE T & Storage<T>::GetMember( HRes hRes )
	{
		PSX_Assert( hRes < m_numSlots, "Invalid index." );
		return m_pResourceList[hRes];
	}

	template < typename T >
	const T & Storage<T>::GetMember( HRes hRes ) const
	{
		PSX_Assert( hRes < m_numSlots, "Invalid index." );
		return m_pResourceList[hRes];
	}

	template < typename T >
	PSX_INLINE T * Storage<T>::GetMemberPtr( HRes hRes )
	{
		PSX_Assert( hRes < m_numSlots, "Invalid index." );
		return &m_pResourceList[hRes];
	}

	template < typename T >
	PSX_INLINE const T * Storage<T>::GetMemberPtr( HRes hRes ) const
	{
		PSX_Assert( hRes < m_numSlots, "Invalid index." );
		return &m_pResourceList[hRes];
	}

	class IStoragePool
	{
	public:

		typedef void (*FOREACHCALLBACK)( IStoragePool *pStoragePool, HRes hRes, void *pMember );

		virtual ~IStoragePool( void ) { }

		virtual void Create( void ) = 0;
		virtual void Create( SIZE_T growSize ) = 0;
		virtual void Destroy( void ) = 0;

		// HRes Add( const T &member ) = 0;
		virtual void ReleaseMember( HRes hRes ) = 0;
		virtual void Clear( void ) = 0;
		virtual HRes NextHandle( void ) = 0;
		virtual void ForEach( FOREACHCALLBACK function ) = 0;

		virtual BOOL IsHandleValid( HRes hRes ) const = 0;
		virtual BOOL IsInitialized() const = 0;

		// TODO: Add const accessors here
		// T & GetMember( HRes hRes ) = 0;
		// T * GetPtr( HRes hRes ) = 0;
		virtual void * GetPtrGeneric( HRes hRes ) = 0;

		virtual const SIZE_T GetSize( void ) const = 0;
		virtual const SIZE_T GetNumUsed( void ) const = 0;
		virtual const SIZE_T GetNumFree( void ) const = 0;

	private:

	};

	template< typename T >
	class StoragePool : public IStoragePool
	{
	public:

		typedef IStoragePool::FOREACHCALLBACK FOREACHCALLBACK;

		typedef Storage< T > StorageGroup;
		typedef List< StorageGroup * > MemberGroupList;

		StoragePool( void );
		StoragePool( SIZE_T growSize );
		~StoragePool( void );

		void Create( void );
		void Create( SIZE_T growSize );
		void Destroy( void );

		HRes AddMember( const T &member );
		void ReleaseMember( HRes hRes );
		void Clear( void );
		HRes NextHandle( void );
		void ForEach( FOREACHCALLBACK function );

		BOOL IsHandleValid( HRes hRes ) const;
		BOOL IsInitialized()const;

		// TODO: Add const accessors here
		T & GetMember( HRes hRes );
		T * GetPtr( HRes hRes );
		void * GetPtrGeneric( HRes hRes );

		const SIZE_T GetSize( void ) const;
		const SIZE_T GetNumUsed( void ) const;
		const SIZE_T GetNumFree( void ) const;

	private:

		explicit StoragePool( const StoragePool &pool ) { }
		StoragePool & operator = (const StoragePool & rhs ) { }

		StorageGroup  		*	AddGroup( void );
		StorageGroup  		*	FindOpenGroup( UINT * pGroupNumber );
		StorageGroup  		*	GetGroup( UINT index );
		const StorageGroup 	*	GetGroup( UINT index ) const;

		INT  GetGroupNumber( HRes handle )const;
		INT  GetItemIndex( HRes handle )const;
		HRes BuildHandle( INT group, HRes hResIndex )const;

		BOOL			m_bInitialized;

		MemberGroupList m_groupList;

		SIZE_T	m_growSize;
		SIZE_T	m_totalMembers;
		SIZE_T	m_totalOpen;
		SIZE_T	m_groupElemCount;
		SIZE_T	m_indexMask;
		INT		m_indexShift;
	};

	template< typename T >
	PSX_INLINE StoragePool<T>::StoragePool( void )
		: m_bInitialized( FALSE ), m_growSize(0), m_totalMembers( 0 ), m_totalOpen( 0 ),
		m_groupElemCount( 0 ), m_indexMask( 0 ), m_indexShift( 0 )
	{
		m_groupList.Clear();
	}

	template< typename T >
	PSX_INLINE StoragePool<T>::StoragePool( SIZE_T growSize )
	{
		Create( growSize );
	}

	template< typename T >
	PSX_INLINE StoragePool<T>::~StoragePool( void )
	{
		Destroy();
	}

	template< typename T >
	PSX_INLINE void StoragePool<T>::Create( void )
	{
		Create( m_growSize );
	}

	template< typename T >
	PSX_INLINE void StoragePool<T>::Create( SIZE_T growSize )
	{
		Destroy();
		
		m_bInitialized		= TRUE;
		m_growSize			= growSize;
		m_totalMembers		= 0;
		m_totalOpen			= 0;
		m_groupElemCount	= Math::NearestPowerOfTwo( growSize );
		m_indexShift		= PSX_LowestBitSet( m_groupElemCount );
		m_indexShift		= Math::Clamp( m_indexShift, 1, 31 ); // Don't you mean 31?
		m_groupElemCount	= 1 << m_indexShift;
		m_indexMask			= m_groupElemCount - 1;
	}

	template< typename T >
	PSX_INLINE void StoragePool<T>::Destroy( void )
	{
		Clear();
		m_bInitialized = FALSE;
	}

	template< typename T >
	PSX_INLINE HRes StoragePool<T>::AddMember( const T &member )
	{
		UINT groupNumber = 0;
		StorageGroup *pOpenGroup = FindOpenGroup( &groupNumber );
		HRes bareHRes = pOpenGroup->Add( member );

		--m_totalOpen;
		return BuildHandle( groupNumber, bareHRes );
	}

	template< typename T >
	PSX_FORCE_INLINE void StoragePool<T>::ReleaseMember( HRes hRes )
	{
		if ( IsHandleValid( hRes ) )
		{
			INT groupIndex = GetGroupNumber( hRes );
			INT itemIndex = GetItemIndex( hRes );

			StorageGroup *pGroup = GetGroup( groupIndex );
			pGroup->Free( itemIndex );

			++m_totalOpen;

			// Check if we can remove the last group
			StorageGroup *pLastGroup = m_groupList.GetBack();
			if ( pLastGroup->GetNumFree() == m_groupElemCount )
			{
				pLastGroup->Destroy();
				delete pLastGroup;
				m_groupList.PopBack();
				m_totalMembers -= m_groupElemCount;
				m_totalOpen -= m_groupElemCount;
			}
		}
	}

	template< typename T >
	PSX_INLINE void StoragePool<T>::Clear( void )
	{
		StorageGroup *pGroup;
		for ( MemberGroupList::Iterator iter = m_groupList.IteratorBegin(); iter != m_groupList.IteratorEnd(); ++iter )
		{
			pGroup = *iter;
			pGroup->Destroy();
			delete pGroup;
		}

		m_groupList.Clear();
		m_totalOpen = 0;
	}

	template< typename T >
	PSX_INLINE HRes StoragePool<T>::NextHandle( void )
	{
		UINT groupNumber = 0;
		StorageGroup *pOpenGroup = FindOpenGroup( &groupNumber );
		
		HRes bareHRes = pOpenGroup->NextHandle();
		--m_totalOpen;

		return BuildHandle( groupNumber, bareHRes );
	}

	template< typename T >
	PSX_INLINE void StoragePool<T>::ForEach( typename StoragePool<T>::FOREACHCALLBACK function )
	{
		INT		groupNumber		= 0;
		SIZE_T	callbackCount	= 0;
		HRes	hResIndex;

		StorageGroup *pGroup;
		for ( MemberGroupList::Iterator iter = m_groupList.IteratorBegin(); iter != m_groupList.IteratorEnd(); ++iter )
		{
			pGroup = *iter;
			callbackCount = pGroup->GetNumUsed();
			hResIndex = 0;

			while ( callbackCount && hResIndex < m_groupElemCount )
			{
				if ( pGroup->IsOpen( hResIndex ) )
				{
					(*function)( this, BuildHandle( groupNumber, hResIndex ), reinterpret_cast<void*>(pGroup->GetMemberPtr(hResIndex)) );
					--callbackCount;
				}
				++hResIndex;
			}
			++groupNumber;
		}
	}

	template< typename T >
	PSX_INLINE BOOL StoragePool<T>::IsHandleValid( HRes hRes ) const
	{
		if ( hRes != StorageGroup::INVALID_HANDLE )
		{
			PSX_Assert( m_groupList.GetSize(), "Group list is empty." );
			const StorageGroup *pGroup = GetGroup( GetGroupNumber(hRes) );
			return !pGroup->IsOpen( GetItemIndex( hRes ) );
		}
		return FALSE;
	}

	template< typename T >
	PSX_FORCE_INLINE BOOL StoragePool<T>::IsInitialized() const
	{
		return m_bInitialized;
	}

	template< typename T >
	PSX_INLINE T & StoragePool<T>::GetMember( HRes hRes )
	{
		PSX_Assert( m_groupList.GetSize(), "Group List is empty." );
		StorageGroup *pGroup = GetGroup( GetGroupNumber( hRes ) );
		HRes hResIndex = GetItemIndex( hRes );
		return pGroup->GetMember( hResIndex );
	}

	template< typename T >
	PSX_INLINE T * StoragePool<T>::GetPtr( HRes hRes )
	{
		PSX_Assert( m_groupList.GetSize(), "Group List is empty." );
		StorageGroup *pGroup = GetGroup( GetGroupNumber( hRes ) );
		HRes hResIndex = GetItemIndex( hRes );
		return pGroup->GetMemberPtr( hResIndex );
	}

	template< typename T >
	PSX_FORCE_INLINE void * StoragePool<T>::GetPtrGeneric( HRes hRes )
	{
		return reinterpret_cast<void*>(GetPtr(hRes) );
	}

	template< typename T >
	PSX_FORCE_INLINE const SIZE_T StoragePool<T>::GetSize( void ) const
	{
		return m_totalMembers;
	}

	template< typename T >
	PSX_FORCE_INLINE const SIZE_T StoragePool<T>::GetNumUsed( void ) const
	{
		return m_totalMembers - m_totalOpen;
	}

	template< typename T >
	PSX_FORCE_INLINE const SIZE_T StoragePool<T>::GetNumFree( void ) const
	{
		return m_totalOpen;
	}

	template< typename T >
	PSX_INLINE typename StoragePool<T>::StorageGroup * StoragePool<T>::AddGroup( void )
	{
		StorageGroup *pGroup = new StorageGroup( m_groupElemCount );
		PSX_Assert( pGroup, "Failed to allocate memory." );
		m_groupList.PushBack( pGroup );

		m_totalMembers += m_groupElemCount;
		m_totalOpen += m_groupElemCount;

		return pGroup;
	}
	
	template< typename T >
	PSX_INLINE typename StoragePool<T>::StorageGroup * StoragePool<T>::FindOpenGroup( UINT * pGroupNumber )
	{
		StorageGroup *pGroup;
		*pGroupNumber = 0;

		for ( MemberGroupList::Iterator iter = m_groupList.IteratorBegin(); iter != m_groupList.IteratorEnd(); ++iter )
		{
			pGroup = *iter;

			if ( pGroup->GetNumFree() )
				return pGroup;
			++(*pGroupNumber);
		}

		// Empty so add a new group. Before we add make sure we don't 
		//	get pass our max handle value limit.
		PSX_Assert( (m_groupList.GetSize() + 1) * m_groupElemCount < PSX_UINT_MAX, "StoragePool is full." );
		
		return AddGroup();
	}
	
	template< typename T >
	PSX_INLINE typename StoragePool<T>::StorageGroup * StoragePool<T>::GetGroup( UINT index )
	{
		PSX_Assert( index < m_groupList.GetSize(), "Invalid group index value." );

		// Find and return the desired group
		MemberGroupList::Iterator iter = m_groupList.IteratorBegin();

		while ( index )
		{
			++iter;
			--index;
		}

		return *iter;
	}
	
	template< typename T >
	PSX_INLINE const typename StoragePool<T>::StorageGroup * StoragePool<T>::GetGroup( UINT index ) const
	{
		PSX_Assert( index < m_groupList.GetSize(), "Invalid group index value." );

		// Find and return the desired group
		MemberGroupList::Iterator iter = m_groupList.IteratorBegin();

		while ( index )
		{
			++iter;
			--index;
		}

		return *iter;
	}

	template< typename T >
	PSX_FORCE_INLINE INT StoragePool<T>::GetGroupNumber( HRes handle ) const
	{
		return handle >> m_indexShift;
	}

	template< typename T >
	PSX_FORCE_INLINE INT StoragePool<T>::GetItemIndex( HRes handle ) const
	{
		return handle & m_indexMask;
	}

	template< typename T >
	PSX_FORCE_INLINE HRes StoragePool<T>::BuildHandle( INT group, HRes hResIndex ) const
	{
		return (group << m_indexShift) + hResIndex;
	}

}

#endif /* _PSX_STORAGE_POOL_H_ */