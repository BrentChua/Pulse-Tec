/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	List.h
*
*	Description -	List container class. Double ended linked-list.
*
*	Comments	-	NOTE: Doesn't have const iterator. 
*					typedef const Iterator ConstIterator; is not what we're looking for.
*					
*					TODO: Implement Find()
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/08/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_LIST_H_
#define _PSX_LIST_H_

#include "PulseSTD.h"
#include "PTLBase.h"

namespace Pulse
{
	template < typename T >
	class List
	{
	public:

		typedef List< T > ThisList;

		struct Node
		{
			Node( const Node &node );
			Node( const T &elem, Node *pPrev = NULL, Node *pNext = NULL );

			T m_elem;
			Node *m_pPrev;
			Node *m_pNext;
		};

		class Iterator
		{
		public:

			Iterator( void );
			Iterator( const List<T> *pList, Node &node );
			Iterator( const List<T> *pList, Node *pNode );

			BOOL IsValid( void );

			Iterator & Begin( void );
			Iterator & End( void );

			Iterator & operator ++ ( void );
			Iterator operator ++ ( INT );
			Iterator & operator -- ( void );
			Iterator operator -- ( INT );

			bool operator == ( const Iterator &rhs );
			bool operator != ( const Iterator &rhs );

			T & operator * ();
			T * operator -> ();

		private:

			friend class List;

			const ThisList *m_pList;
			Node *m_pNode;

		};

		typedef const Iterator	ConstIterator;
		typedef T&				Reference;
		typedef const T&		ConstReference;

		List( void );
		List( const List &list );
		~List( void );

		void PushBack( const T &elem );
		void PushFront( const T &elem );
		void PopBack( void );
		void PopFront( void );

		Reference		GetFront( void );
		ConstReference	GetFront( void ) const;
		Reference		GetBack( void );
		ConstReference	GetBack( void ) const;

		const SIZE_T	GetSize( void ) const;

		void Clear( void );
		bool IsEmpty( void ) const;

		Iterator	Remove( const T &elem );
		Iterator	Remove( Iterator iter );
		Iterator	Remove( Iterator first, Iterator centinel );
		Iterator	Insert( Iterator iter, const T &elem );

		List & operator = ( const List &rhs );

		Iterator IteratorBegin( void );
		ConstIterator IteratorBegin( void ) const;
		Iterator IteratorBeginLast( void );
		ConstIterator IteratorBeginLast( void ) const;
		Iterator IteratorEnd( void );
		ConstIterator IteratorEnd( void ) const;

	private:

		Node *m_pHead;
		Node *m_pTail;
		SIZE_T m_size;

	};

	template < typename T >
	PSX_INLINE List< T >::Node::Node( const Node &node ) :
		m_elem( node.m_elem ), m_pPrev( node.m_pPrev ), m_pNext( node.m_pNext )
	{

	}

	template < typename T >
	PSX_INLINE List< T >::Node::Node( const T &elem, Node *pPrev /* = NULL */, Node *pNext /* = NULL */ ) :
		m_elem( elem ), m_pPrev( pPrev ), m_pNext( pNext )
	{

	}

	template < typename T >
	PSX_INLINE List< T >::Iterator::Iterator( void ) : m_pList( NULL ), m_pNode( NULL )
	{

	}

	template < typename T >
	PSX_INLINE List< T >::Iterator::Iterator( const List<T> *pList, Node &node ) 
		: m_pList( pList ), m_pNode( &node )
	{

	}

	template < typename T >
	PSX_INLINE List< T >::Iterator::Iterator( const List<T> *pList, Node *pNode ) 
		: m_pList( pList ), m_pNode( pNode )
	{

	}

	template < typename T >
	PSX_INLINE BOOL List< T >::Iterator::IsValid( void )
	{
		return m_pNode != NULL;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Iterator & Begin( void )
	{
		*this = m_pList->IteratorBegin();
		return *this;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Iterator & End( void )
	{
		*this = m_pList->IteratorBeginLast();
		return *this;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Iterator & List< T >::Iterator::operator ++ ( void )
	{
		m_pNode = m_pNode->m_pNext;
		return *this;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Iterator List< T >::Iterator::operator ++ ( INT )
	{
		Iterator iter( m_pNode );
		m_pNode = m_pNode->m_pNext;
		return iter;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Iterator & List< T >::Iterator::operator -- ( void )
	{
		m_pNode = m_pNode->m_pPrev;
		return *this;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Iterator List< T >::Iterator::operator -- ( INT )
	{
		Iterator iter( m_pNode );
		m_pNode = m_pNode->m_pNext;
		return iter;
	}

	template < typename T >
	PSX_INLINE bool List< T >::Iterator::operator == ( const Iterator &rhs )
	{
		return m_pNode == rhs.m_pNode;
	}

	template < typename T >
	PSX_INLINE bool List< T >::Iterator::operator != ( const Iterator &rhs )
	{
		return m_pNode != rhs.m_pNode;
	}

	template < typename T >
	PSX_INLINE T & List< T >::Iterator::operator * ()
	{
		return GetRefGeneric<T>( m_pNode->m_elem );
	}
	
	template < typename T >
	PSX_INLINE T * List< T >::Iterator::operator -> ()
	{
		return GetPtrGeneric<T>( m_pNode->m_elem );
	}
	
	template < typename T >
	PSX_INLINE List< T >::List( void ) : m_pHead( NULL ), m_pTail( NULL ), m_size( 0 )
	{

	}

	template < typename T >
	PSX_INLINE List< T >::List( const List &list ) : m_pHead( NULL ), m_pTail( NULL ), m_size( 0 )
	{
		this->operator =( list );
	}
	
	template < typename T >
	PSX_INLINE List< T >::~List( void )
	{
		Clear();
	}

	template < typename T >
	void List< T >::PushBack( const T &elem )
	{
		if ( m_pTail )
		{
			Node *pNewNode = new Node( elem, m_pTail, PSX_NULL );
			PSX_Assert( pNewNode, "Failed to allocate memory." );
			m_pTail->m_pNext = pNewNode;
			m_pTail = pNewNode;
		}
		else // if tail is null then so is head
		{
			m_pHead = new Node( elem, NULL, NULL );
			PSX_Assert( m_pHead, "Failed to allocate memory." );
			m_pTail = m_pHead;
		}
		++m_size;
	}

	template < typename T >
	void List< T >::PushFront( const T &elem )
	{
		if ( m_pHead )
		{
			Node *pNewNode = new Node( elem, NULL, m_pHead );
			PSX_Assert( pNewNode, "Failed to allocate memory." );
			m_pHead->m_pPrev = pNewNode;
			m_pHead = pNewNode;
		}
		else // if head is null then so is tail
		{
			m_pHead = new Node( elem, NULL, NULL );
			PSX_Assert( m_pHead, "Failed to allocate memory." );
			m_pTail = m_pHead;
		}
		++m_size;
	}

	template < typename T >
	void List< T >::PopBack( void )
	{
		if ( m_pTail )
		{
			Node *pNode = m_pTail;

			m_pTail = m_pTail->m_pPrev;
			
			if ( m_pTail )
				m_pTail->m_pNext = NULL;
			else
				m_pHead = NULL;
			
			delete pNode;
			
			--m_size;
		}
	}

	template < typename T >
	void List< T >::PopFront( void )
	{
		PSX_Assert( m_pHead, "Head is null." );
		Node *pNode = m_pHead;
		m_pHead = m_pHead->m_pNext;
		if ( m_pHead )
			m_pHead->m_pPrev = NULL;
		else
			m_pTail = NULL;
		delete pNode;
		--m_size;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Reference List< T >::GetFront( void )
	{
		PSX_Assert( m_pHead, "Head is null." );
		return m_pHead->m_elem;
	}

	template < typename T >
	PSX_INLINE typename List< T >::ConstReference List< T >::GetFront( void ) const
	{
		PSX_Assert( m_pHead, "Head is null." );
		return m_pHead->m_elem;
	}

	template < typename T >
	PSX_INLINE typename List< T >::Reference List< T >::GetBack( void )
	{
		PSX_Assert( m_pTail, "Tail is null." );
		return m_pTail->m_elem;
	}

	template < typename T >
	PSX_INLINE typename List< T >::ConstReference List< T >::GetBack( void ) const
	{
		PSX_Assert( m_pTail, "Tail is null." );
		return m_pTail->m_elem;
	}

	template < typename T >
	void List< T >::Clear( void )
	{
		Node *pNode;
		while ( m_pHead )
		{
			pNode = m_pHead;
			m_pHead = m_pHead->m_pNext;
			delete pNode;
		}

		m_pHead = m_pTail = NULL;
		m_size = 0;
	}

	template < typename T >
	PSX_INLINE bool List< T >::IsEmpty( void ) const
	{
		return m_size == 0;
	}

	template < typename T >
	typename List< T >::Iterator List< T >::Remove( const T &elem )
	{
		Node * pCurNode = m_pHead;

		while ( pCurNode != NULL )
		{
			if ( pCurNode->m_elem == elem )
				return Remove( Iterator(this, pCurNode) );

			pCurNode = pCurNode->m_pNext;
		}

		return Iterator();
	}

	template < typename T >
	typename List< T >::Iterator List< T >::Remove( Iterator iter )
	{
		PSX_Assert( iter.IsValid(), "Iter is null." );

		Node *pPrev = iter.m_pNode->m_pPrev;
		Node *pNext = iter.m_pNode->m_pNext;

		// Reconnect pointers
		if ( m_pTail == iter.m_pNode )
			m_pTail = m_pTail->m_pPrev;
		if ( m_pHead == iter.m_pNode )
			m_pHead = m_pHead->m_pNext;
			
		if ( pPrev )
			pPrev->m_pNext = pNext;
		if ( pNext )
			pNext->m_pPrev = pPrev;

		delete iter.m_pNode;
		--m_size;

		return Iterator( this, pNext );
	}

	// first - Position of the first element removed from the list.
	// centinel - Position just beyond the last element to be removed.
	template < typename T >
	typename List< T >::Iterator List< T >::Remove( Iterator first, Iterator centinel )
	{
		PSX_Assert( first == centinel, "First iterator cannot be last iterator. Last is the position just beyond the last element to be removed." );
		PSX_Assert( first, "First iterator is NULL." );

		Node *pNode;
		Node *pPrev = first->m_pPrev;

		// rewire pointers if necessary
		if ( m_pTail == centinel->m_pPrev )
			m_pTail = first->m_pPrev;
		if ( m_pHead == first )
			m_pHead = centinel;

		if ( pPrev )
			pPrev->m_pNext = centinel;
		if ( centinel )
			centinel->m_pPrev = pPrev;

		while ( first != centinel )
		{
			pNode = first;
			first = first->m_pNext;
			delete pNode;
			--m_size;
		}

		return centinel;
	}

	template < typename T >
	typename List< T >::Iterator List< T >::Insert( Iterator iter, const T &elem )
	{
		PSX_Assert( iter, "Iter is null." );
		PSX_Assert( elem, "Elem is null." );

		Node *pPrev = iter->m_pPrev;

		// Create new node then rewire pointers
		Node *pNode = new Node( elem, pPrev, iter );
		iter->m_pPrev = pNode;

		if ( pPrev )
			pPrev->m_pNext = pNode;

		if ( m_pHead == iter )
			m_pHead = pNode;

		++m_size;

		return pNode;
	}

	template < typename T >
	PSX_FORCE_INLINE const SIZE_T List< T >::GetSize( void ) const
	{
		return m_size;
	}

	template < typename T >
	typename List< T > & List< T >::operator = ( const List &rhs )
	{
		if ( this == &rhs )
			return *this;

		Clear();

		List *listRhs = (List*)&rhs;

		if ( listRhs->GetSize() == 0 )
			return *this;

		Iterator iter = listRhs->IteratorBegin();

		while( iter.IsValid() )
		{
			PushBack( *iter );
			++iter;
		}

		m_size = rhs.m_size;

		return *this;
	}


	template < typename T >
	PSX_FORCE_INLINE typename List< T >::Iterator List< T >::IteratorBegin( void )
	{
		return Iterator( this, *m_pHead );
	}

	template < typename T >
	PSX_FORCE_INLINE typename const List< T >::Iterator List< T >::IteratorBegin( void ) const
	{
		return Iterator( this, *m_pHead );
	}

	template < typename T >
	PSX_FORCE_INLINE typename List< T >::Iterator List< T >::IteratorBeginLast( void )
	{
		return Iterator( this, *m_pTail );
	}

	template < typename T >
	PSX_FORCE_INLINE typename const List< T >::Iterator List< T >::IteratorBeginLast( void ) const
	{
		return Iterator( this, *m_pTail );
	}

	template < typename T >
	PSX_FORCE_INLINE typename List< T >::Iterator List< T >::IteratorEnd( void )
	{
		// It is expected that the centinel iterator for this container is NULL.
		// Otherwise, there is something terribly wrong.
		return Iterator();
	}

	template < typename T >
	PSX_FORCE_INLINE typename const List< T >::Iterator List< T >::IteratorEnd( void ) const
	{
		// It is expected that the centinel iterator for this container is NULL.
		// Otherwise, there is something terribly wrong.
		return Iterator();
	}


};

#endif /* _PSX_LIST_H_ */