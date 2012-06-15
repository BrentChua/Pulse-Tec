/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	LList.h
*
*	Description -	Linked-list container class. Single-ended linked-list.
*
*	Comments	-	
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/09/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_LINKED_LIST_H_
#define _PSX_LINKED_LIST_H_

#include "PulseSTD.h"

namespace Pulse
{

	template < typename T >
	class LList
	{
	public:

		struct Node
		{
			Node( const T &elem, Node *pNext );
			T m_elem;
			Node *m_pNext;
		};

		class Iterator
		{
		public:

			Iterator( void );
			Iterator( Node &node );
			Iterator( Node *pNode );

			Iterator & operator ++ ( void );
			Iterator operator ++ ( INT );
			
			// These decrement iterators are slow and I choose not to implement these methods.
			//Iterator & operator -- ( void );
			//Iterator operator -- ( INT );

			bool IsValid( void );

			bool operator == ( const Iterator &rhs );
			bool operator != ( const Iterator &rhs );

			T & operator * ();
			T * operator -> ();

		private:

			friend class LList;

			Node *m_pNode;
			Node *m_pPrevNode;
		};

		LList( void );
		//LList( const LList &lList );
		~LList( void );

		T & GetFront( void );
		const T & GetFront( void ) const;

		void PushFront( const T &elem );
		void PushBack( const T &elem );
		void PushAfter( Iterator &iter, const T &elem );

		void PopFront( void );

		SIZE_T GetSize( void );

		bool IsEmpty( void );
		void Clear( void );

		Iterator Find( const T &elem );
		Iterator Find( const Iterator &iter );
		Iterator FindPrevious( const T &elem );
		Iterator FindPrevious( const Iterator &iter );

		void Insert( Iterator &iter, const T &elem );
		Iterator Remove( const T &elem );
		Iterator Remove( const Iterator &iter );

		LList & operator = ( const LList &rhs );

		Iterator IteratorBegin( void );
		Iterator IteratorEnd( void );

	protected:

		Node *m_pHead;
		SIZE_T m_size;

	};

	/* Node Implementation */

	template < typename T >
	PSX_INLINE LList< T >::Node::Node( const T &elem, Node *pNext ) : m_elem( elem ), m_pNext( pNext )
	{

	}

	/* Iterator Implementation */

	template < typename T >
	PSX_INLINE LList< T >::Iterator::Iterator( void ) : m_pNode( NULL )
	{

	}

	template < typename T >
	PSX_INLINE LList< T >::Iterator::Iterator( Node &node ) : m_pNode( &node )
	{

	}

	template < typename T >
	PSX_INLINE LList< T >::Iterator::Iterator( Node *pNode ) : m_pNode( pNode )
	{

	}

	template < typename T >
	PSX_INLINE bool LList< T >::Iterator::IsValid( void )
	{
		return m_pNode != NULL;
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator & LList< T >::Iterator::operator ++ ( void )
	{
		m_pNode = m_pNode->m_pNext;
		return *this;
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::Iterator::operator ++ ( INT )
	{
		Iterator iter( *this );
		m_pNode = m_pNode->m_pNext;
		return iter;
	}

	template < typename T >
	PSX_INLINE bool LList< T >::Iterator::operator == ( const Iterator &rhs )
	{
		return m_pNode == rhs.m_pNode;
	}

	template < typename T >
	PSX_INLINE bool LList< T >::Iterator::operator != ( const Iterator &rhs )
	{
		return !(*this == rhs);
	}

	template < typename T >
	PSX_INLINE T & LList< T >::Iterator::operator * ()
	{
		return m_pNode->m_elem;
	}

	template < typename T >
	PSX_INLINE T * LList< T >::Iterator::operator -> ()
	{
		return &m_pNode->m_elem;
	}

	/* LList Implementation */

	template < typename T >
	PSX_INLINE  LList< T >::LList( void ) : m_pHead( NULL ), m_size( 0 )
	{

	}

	template < typename T >
	PSX_INLINE LList< T >::~LList( void )
	{
		Clear();
	}

	template < typename T >
	PSX_INLINE T & LList< T >::GetFront( void )
	{
		return m_pHead->m_elem;
	}

	template < typename T >
	PSX_INLINE const T & LList< T >::GetFront( void ) const
	{
		return m_pHead->m_elem;
	}

	template < typename T >
	void LList< T >::PushFront( const T &elem )
	{
		Node *pNode = new Node( elem, m_pHead );
		PSX_Assert( pNode, "Failed to allocate new memory." );
		m_pHead = pNode;
		++m_size;
	}

	// NOTE: This is a slow process.
	template < typename T >
	void LList< T >::PushBack( const T &elem )
	{
		if ( m_size == 0 )
		{
			PushFront( elem );
			return;
		}

		Iterator iter(m_pHead);

		while ( iter.m_pNode->m_pNext )
			++iter;

		PushAfter( iter, elem );
	}

	template < typename T >
	PSX_INLINE void LList< T >::PushAfter( Iterator &iter, const T &elem )
	{
		PSX_Assert( iter.IsValid(), "Iter not valid." );
		iter.m_pNode->m_pNext = new Node( elem, iter.m_pNode->m_pNext );
		PSX_Assert( iter.m_pNode->m_pNext, "Failed to allocate memory." );
		++m_size;
	}


	template < typename T >
	void LList< T >::PopFront( void )
	{
		PSX_Assert( m_pHead, "Head is null." );

		Node *pNode = m_pHead;
		m_pHead = m_pHead->m_pNext;
		
		--m_size;
		delete pNode;
	}

	template < typename T >
	PSX_INLINE SIZE_T LList< T >::GetSize( void )
	{
		return m_size;
	}

	template < typename T>
	PSX_INLINE bool LList< T >::IsEmpty( void )
	{
		return m_size == 0;
	}

	template < typename T >
	void LList< T >::Clear( void )
	{
		Node *pNode;

		while ( m_pHead )
		{
			pNode = m_pHead;
			m_pHead = m_pHead->m_pNext;
			delete pNode;
		}
		m_size = 0;
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::Find( const T &elem )
	{
		Node *pNode = m_pHead;

		while ( pNode && pNode->m_elem != elem )
			pNode = pNode->m_pNext;

		return Iterator( pNode );
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::Find( const Iterator &iter )
	{
		return Find( iter.m_pNode->m_elem; );
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::FindPrevious( const T &elem )
	{
		Node *pNode = m_pHead;

		if ( !pNode )
			return Iterator( NULL );

		while ( pNode->m_pNext && pNode->m_pNext->m_elem != elem )
			pNode = pNode->m_pNext;

		return Iterator( pNode->m_pNext != NULL ? pNode : NULL );
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::FindPrevious( const Iterator &iter )
	{
		return FindPrevious( iter.m_pNode->m_elem );
	}

	template < typename T >
	void LList< T >::Insert( Iterator &iter, const T &elem )
	{
		Node *pNode = new Node( elem, iter.m_pNode );
		PSX_Assert( pNode, "Failed to allocate memory." );

		if ( iter.m_pNode == m_pHead )
			m_pHead = pNode;
		else 
		{
			Iterator prev = FindPrevious( iter );

			// This check shouldn't be needed but its there just to make sure
			if ( prev.IsValid() )
				prev.m_pNode->m_pNext = pNode;
		}
		++m_size;
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::Remove( const T &elem )
	{
		Iterator iter = Find( elem );
		return Remove( iter );
	}

	template < typename T >
	typename LList< T >::Iterator LList< T >::Remove( const Iterator &iter )
	{
		Iterator _iter = iter;

		if ( !_iter.IsValid() )
			return Iterator( NULL );

		if ( _iter.m_pNode == m_pHead )
			m_pHead = m_pHead->m_pNext;
		else
		{
			Iterator prev = FindPrevious( _iter );

			// This check shouldn't be needed but its there just to make sure
			if( prev.IsValid() )
				prev.m_pNode->m_pNext = iter.m_pNode->m_pNext;
		}

		Iterator iterNext(++_iter);
		delete iter.m_pNode;
		--m_size;

		return iterNext;
	}

	template < typename T >
	typename LList< T > & LList< T >::operator = ( const LList &rhs )
	{
		if ( this == &rhs )
			return *this;

		Iterator iter = rhs.IteratorBegin();
		Clear();

		if ( rhs.m_size == 0 )
			return *this;

		while ( iter.IsValid() )
		{
			m_pHead = new Node( *iter, m_pHead );
			++iter;
		}

		m_size = rhs.m_size;
		return *this;
	}


	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::IteratorBegin( void )
	{
		return m_pHead;
	}

	template < typename T >
	PSX_INLINE typename LList< T >::Iterator LList< T >::IteratorEnd( void )
	{
		return NULL;
	}

};

#endif /* _PSX_LINKED_LIST_H_ */