/**************************************************************************************
*
*	Copyright (C) 2009-2010 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Hashtable.h
*
*	Description -	Hashtable or unsorted map?
*
*	Comments	- 
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/15/2009	-	Creation of this file
*			MrCodeSushi	-	04/29/2010	-	HashTable rewrite
**************************************************************************************/

#ifndef _PSX_HASH_TABLE_H_
#define _PSX_HASH_TABLE_H_

#include "PTLBase.h"
#include "String.h"
#include "List.h"
#include "Array.h"

namespace Pulse
{

	namespace _Private
	{
		template < typename T >
		class DefaultHashMethod
		{
		public:

			DefaultHashMethod( INT numBuckets = 100 ) : m_numBuckets( numBuckets ) { }

			UINT Hash( const T &key ) const;

			INT GetNumBuckets( void ) { return m_numBuckets; }
			void SetNumBuckets( INT numBuckets ) { m_numBuckets = numBuckets; }

		private:

			INT m_numBuckets;
		};

		template < typename T >
		PSX_INLINE  UINT DefaultHashMethod<T>::Hash( const T &key ) const
		{
			int nBytes = sizeof(key);
			UINT result =0;

			for( int i=0; i < nBytes; ++i )
				result += *((BYTE*)(&key)+i);

			return (result%m_numBuckets);
		}

		template < >
		class DefaultHashMethod< String >
		{
		public:

			DefaultHashMethod( INT numBuckets = 100 ) : m_numBuckets( numBuckets ) { }

			UINT Hash( const String &key ) const {
				return PSX_SuperFastHash( key.GetBuffer(), key.GetLength() ) % m_numBuckets;
			}

			INT GetNumBuckets( void ) { return m_numBuckets; }
			void SetNumBuckets( INT numBuckets ) { m_numBuckets = numBuckets; }

		private:

			INT m_numBuckets;

		};

	}

	template< typename K, typename T, typename Trait = PSX_Equal<K>, typename Hash = _Private::DefaultHashMethod<K> >
	class HashTable
	{
	public:

		typedef K Key;
		typedef T Type;
		typedef Trait Compare;
		typedef PSX_Pair< const Key, Type > EntryPair;
		typedef List< EntryPair > TypeList;
		typename typedef TypeList::Iterator ListIterator;
		typedef Array<TypeList> Table;
		typedef HashTable< Key, Type, Compare, Hash > ThisHashTable;

		class Iterator
		{
		public:

			Iterator( void );
			Iterator( const ThisHashTable *pHashTable, SIZE_T currBucket, ListIterator iter );

			EntryPair & operator * ( void );
			EntryPair & operator * ( void ) const;
			EntryPair * operator -> ( void );
			EntryPair * operator -> ( void ) const;

			Iterator & operator ++ ( void );
			Iterator operator ++ ( int );
			Iterator & operator -- ( void );
			Iterator operator -- ( int );

			BOOL operator == ( const Iterator &rhs ) const;
			BOOL operator != ( const Iterator &rhs ) const;

			Iterator & operator = ( Iterator &rhs );

		private:

			void Increment( void );
			void Decrement( void );
			
			const ThisHashTable *m_pHashTable;
			SIZE_T				m_currBucket;
			ListIterator		m_listIter;

		};

		typedef Iterator ConstIterator;

		HashTable( void );
		HashTable( SIZE_T bucketSize );
		HashTable( const HashTable &hash );
		~HashTable( void );

		//template <class InputIterator>
		//HashTable(InputIterator first, InputIterator last, const Compare& compare=Compare(),
		//const Hash& hash=Hash());

		ListIterator Insert( const EntryPair &entry );
		//iterator insert(iterator position, const value_type& value);
		//template <class InputIterator> void insert(InputIterator first, InputIterator last);

		ListIterator Remove( const Key &key );
		//void Remove(iterator position);
		//void Remove(iterator first, iterator last);

		//void Swap(HashTable <Key, T, Compare, Hash>& hashIn);
		void Clear( void );

		Iterator		Find( const Key& key );
		ConstIterator	Find( const Key& key ) const;
		
		SIZE_T			GetSize(const Key& key) const;

		BOOL IsEmpty( void ) const;

		SIZE_T GetSize( void ) const;
		//SIZE_T GetMaxSize( void ) const;

		Type & operator [] ( const Key& key );
		HashTable & operator = ( HashTable &rhs );

		Iterator IteratorBegin();
		Iterator IteratorEnd();
		ConstIterator IteratorBegin() const;
		ConstIterator IteratorEnd() const;

	private:

		ListIterator FindElement( const Key &key, INT &bucket ) const;

		Table	*m_pTable;
		SIZE_T	m_size;
		Compare	m_compare;
		Hash	m_hash;
		
	};

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE HashTable< K, T, Trait, Hash >::Iterator::Iterator( void )
		: m_pHashTable( NULL ), m_currBucket( 0 )
	{

	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE HashTable< K, T, Trait, Hash >::Iterator::Iterator( const HashTable< K, T, Trait, Hash > *pHashTable, SIZE_T currBucket, ListIterator iter  )
		: m_pHashTable( pHashTable ), m_currBucket( currBucket ), m_listIter( iter )
	{

	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::EntryPair & HashTable< K, T, Trait, Hash >::Iterator::operator * ( void )
	{
		return (*m_listIter);
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::EntryPair & HashTable< K, T, Trait, Hash >::Iterator::operator * ( void ) const
	{
		return (*m_listIter);
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::EntryPair * HashTable< K, T, Trait, Hash >::Iterator::operator -> ( void )
	{
		return m_listIter.operator->();
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::EntryPair * HashTable< K, T, Trait, Hash >::Iterator::operator -> ( void ) const
	{
		return m_listIter.operator->();
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename  HashTable< K, T, Trait, Hash >::Iterator & HashTable< K, T, Trait, Hash >::Iterator::operator ++ ( void )
	{
		Increment();
		return *this;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::Iterator HashTable< K, T, Trait, Hash >::Iterator::operator ++ ( int )
	{
		Iterator iter( *this );
		Increment();
		return iter;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::Iterator & HashTable< K, T, Trait, Hash >::Iterator::operator -- ( void )
	{
		Decrement();
		return *this;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::Iterator HashTable< K, T, Trait, Hash >::Iterator::operator -- ( int )
	{
		Iterator iter( *this );
		Decrement();
		return iter;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE BOOL HashTable< K, T, Trait, Hash >::Iterator::operator == ( const Iterator &rhs ) const
	{
		return m_pHashTable == rhs.m_pHashTable &&  m_currBucket == rhs.m_currBucket;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE void HashTable< K, T, Trait, Hash >::Iterator::Increment( void )
	{
		if ( m_listIter == (*m_pHashTable->m_pTable)[m_currBucket].IteratorEnd() )
		{
			for ( SIZE_T i = m_currBucket + 1; i < m_pHashTable->GetSize(); ++i )
			{
				if ( (*m_pHashTable->m_pTable)[i].GetSize() > 0 )
				{
					m_currBucket = i;
					m_listIter = (*m_pHashTable->m_pTable)[i].IteratorBegin();
					return;
				}
			}
			
			*this = m_pHashTable->IteratorEnd();
			return;
		}
		else
			++m_listIter;

		return;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE void HashTable< K, T, Trait, Hash >::Iterator::Decrement( void )
	{
		if ( m_listIter == (*m_pHashTable->m_pTable)[m_currBucket].IteratorEnd() )
		{
			for ( INT i = m_currBucket - 1; i > 0; --i )
			{
				if ( (*m_pHashTable->m_pTable)[i].GetSize() )
				{
					m_currBucket = i;
					m_listIter = (*m_pHashTable->m_pTable)[i].IteratorLast();
					return;
				}


			}
		}
		else
			--m_listIter;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE BOOL HashTable< K, T, Trait, Hash >::Iterator::operator != ( const Iterator &rhs ) const
	{
		return !(*this == rhs);
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::Iterator & HashTable< K, T, Trait, Hash >::Iterator::operator = ( Iterator &rhs )
	{
		m_pHashTable = rhs.m_pHashTable;
		m_currBucket = rhs.m_currBucket;
		m_listIter = rhs.m_listIter;

		return *this;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	HashTable< K, T, Trait, Hash >::HashTable( void )
	{
		m_size = m_hash.GetNumBuckets();
		m_pTable = new Table( m_hash.GetNumBuckets() );
		PSX_Assert( m_pTable, "Failed to allocate memory." );
	}

	template< typename K, typename T, typename Trait , typename Hash >
	HashTable< K, T, Trait, Hash >::HashTable( SIZE_T bucketSize )
		: m_size( 0 )
	{
		m_hash.SetNumBuckets( bucketSize );
		m_pTable = new Table( bucketSize );
		PSX_Assert( m_pTable, "Failed to allocate memory." );
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE HashTable< K, T, Trait, Hash >::HashTable( const HashTable &hash)
	{
		m_pTable = new Table( *hash.m_pTable );
	}
	
	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE HashTable< K, T, Trait, Hash >::~HashTable( void )
	{
		delete m_pTable;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::ListIterator HashTable< K, T, Trait, Hash >::Insert( const EntryPair &entry )
	{
		INT bucket;
		ListIterator iter = FindElement( entry.first, bucket );
		TypeList *pList = &(*m_pTable)[bucket];

		if ( iter == pList->IteratorEnd() )
		{
			pList->PushBack( entry );
			++m_size;
			return pList->IteratorBeginLast();
		}
		
		iter->second = entry.second;
		return iter;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::ListIterator HashTable< K, T, Trait, Hash >::Remove( const Key &key )
	{
		INT bucket;
		ListIterator iter = FindElement( key, bucket );
		TypeList *pList = &(*m_pTable)[bucket];

		if ( iter != pList->IteratorEnd() )
		{
			iter = pList->Remove( iter );
			--m_size;
			return iter;
		}

		return pList->IteratorEnd();
	}

	template< typename K, typename T, typename Trait , typename Hash >
	void HashTable< K, T, Trait, Hash >::Clear( void )
	{
		SIZE_T size = m_pTable->GetSize();
		
		for ( SIZE_T i = 0; i < size; ++i )
			(*m_pTable)[i].Clear();

		m_size = 0;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::Iterator HashTable< K, T, Trait, Hash >::Find( const Key& key )
	{
		INT bucket;
		ListIterator iter = FindElement( key, bucket );
		if ( iter == (*m_pTable)[bucket].IteratorEnd() )
			return IteratorEnd();

		return Iterator( this, bucket, iter );
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::ConstIterator HashTable< K, T, Trait, Hash >::Find( const Key& key ) const
	{
		INT bucket;
		ListIterator iter = FindElement( key, bucket );
		if ( iter == (*m_pTable)[bucket].IteratorEnd() )
			return IteratorEnd();

		return ConstIterator( this, bucket, iter);
	}

	template< typename K, typename T, typename Trait , typename Hash >
	SIZE_T HashTable< K, T, Trait, Hash >::GetSize(const Key& key) const
	{
		bucket = m_hash.Hash( key );
		return (*m_pTable)[bucket].GetSize();
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::ListIterator HashTable< K, T, Trait, Hash >::FindElement( const Key &key, INT &bucket ) const
	{
		bucket = m_hash.Hash( key );
		TypeList *pList = &(*m_pTable)[bucket];
		ListIterator iter = pList->IteratorBegin();
		
		while ( iter != pList->IteratorEnd() )
		{
			if ( m_compare( (*iter).first, key )  )
			{
				return iter;
			}
			++iter;
		}

		return pList->IteratorEnd();
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE BOOL HashTable< K, T, Trait, Hash >::IsEmpty( void ) const
	{
		return m_size == 0;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE SIZE_T HashTable< K, T, Trait, Hash >::GetSize( void ) const
	{
		return m_size;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::Type& HashTable< K, T, Trait, Hash >::operator [] ( const Key& key )
	{
		INT bucket;
		ListIterator iter = FindElement( key, bucket );

		if ( iter.IsValid() )
			return iter->second;

		TypeList *pList = &(*m_pTable)[bucket];
		pList->PushBack( EntryPair( key, Type() ) );

		return pList->IteratorBeginLast()->second;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	HashTable< K, T, Trait, Hash > & HashTable< K, T, Trait, Hash >::operator = ( HashTable &rhs )
	{
		if ( this == &rhs )
			return *this;

		delete m_pTable;
		m_pTable = new Table( *rhs.m_pTable );
		PSX_Assert( m_pTable, "Failed to allocate memory." );
		m_size = rhs.m_size;
		return *this;
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::Iterator HashTable< K, T, Trait, Hash >::IteratorBegin()
	{
		if ( !m_size )
			return IteratorEnd();

		SIZE_T size = m_pTable->GetSize();

		for ( SIZE_T i = 0; i < size; ++i )
		{
			if ( (*m_pTable)[i].GetSize() )
			{
				return Iterator( this, i, (*m_pTable)[i].IteratorBegin() );
			}
		}

		return IteratorEnd();
	}

	template< typename K, typename T, typename Trait , typename Hash >
	PSX_INLINE typename HashTable< K, T, Trait, Hash >::Iterator HashTable< K, T, Trait, Hash >::IteratorEnd()
	{
		return Iterator( this, m_pTable->GetSize() - 1, (*m_pTable)[m_pTable->GetSize() - 1].IteratorEnd() );
		//return Iterator( this, m_pTable->GetSize(), (*m_pTable)[m_pTable->GetSize()].IteratorEnd() );
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::ConstIterator HashTable< K, T, Trait, Hash >::IteratorBegin() const
	{
		if ( !m_size )
			return IteratorEnd();

		SIZE_T size = m_pTable->GetSize();

		for ( SIZE_T i = 0; i < size; ++i )
		{
			if ( (*m_pTable)[i].GetSize() )
			{
				return Iterator( this, i, (*m_pTable)[i].IteratorBegin() );
			}
		}

		return IteratorEnd();
	}

	template< typename K, typename T, typename Trait , typename Hash >
	typename HashTable< K, T, Trait, Hash >::ConstIterator HashTable< K, T, Trait, Hash >::IteratorEnd() const
	{
		return Iterator( this, m_pTable->GetSize() - 1, (*m_pTable)[m_pTable->GetSize() - 1].IteratorEnd() );
	}

}

#endif /* _PSX_HASH_TABLE_H_ */