/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Set.h
*
*	Description -	Implementation of the STL Set.
*
*	Comments	-	This is an indirect implementation of Mark Allen Weiss' STL Set.
*					Iterator should use parent pointer instead of path.
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/12/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_SET_H_
#define _PSX_SET_H_

#include "PTLBase.h"
#include "List.h"

namespace Pulse
{
	template < typename T, typename Trait = PSX_LessThan< T > >
	class Set
	{
	public:


		/* Set Tree Node Class */
		class Node
		{
		public:

			Node( const T &elem = T(), Node *pLeft = NULL, Node *pRight = NULL, INT level = 1 );

		private:
			T	m_elem;
			INT m_level;
			Node *m_pLeft;
			Node *m_pRight;

			friend class Set;
			friend class Iterator;
		};

		/* Iterator Class */
		class Iterator
		{
		public:

			Iterator( void );
			Iterator( const Iterator &iter );
			T & operator * ( void );
			const T & operator * ( void ) const;
			T * operator -> ( void );
			const T * operator -> ( void ) const;

			Iterator & operator ++ ( void );
			Iterator operator ++ ( INT );

			bool operator == ( const Iterator &rhs ) const;
			bool operator != ( const Iterator &rhs ) const;

		protected:

			friend class Set;

			Iterator( const Set &source );

			Node *m_pRoot;
			Node *m_pCurrent;

			List< Node * > m_path;

			void AssertIsInitialized( void ) const;
			void AssertIsValid( void ) const;
			void AssertCanAdvance( void ) const;
			void AssertCanRetreat( void ) const;

			T & Retrieve( void ) const;

			void GoLeft( void );
			void GoRight( void );
			void GoRoot( void );

			bool HasLeft( void ) const;
			bool HasRight( void ) const;

			void Advance( void );

		};

		typedef PSX_Pair< Iterator, bool > ReturnPair;

		Set( void );
		Set( const Set &rhs );
		~Set( void );

		const Set & operator = ( const Set &rhs );

		Iterator IteratorBegin( void );
		Iterator IteratorEnd( void ) const;

		int GetSize( void ) const;
		bool IsEmpty( void ) const;

		Iterator LowerBound( const T &obj ) /*const*/;
		Iterator UpperBound( const T &obj ) /*const*/;
		Iterator Find( const T &obj ) /*const*/;
		Iterator Find( const T &obj ) const;
		ReturnPair Insert( const T &obj );
		int Remove( const Iterator &iter );
		int Remove( const T &obj );
		void Clear( void );


		friend class Iterator;

	private:

		void Init( void );
		void MakeEmpty( void );
		Iterator Bound( const T &obj, bool lower ) /*const*/;

		// Recursive routines
		bool Insert( const T &obj, Node *&pNode );
		void Remove( const T &obj, Node *&pNode );
		void MakeEmpty( Node *&pNode );

		// Rotations
		void Skew( Node *&pNode ) const;
		void Split( Node *&pNode ) const;
		void RotateWithLeftChild( Node *&pNode ) const;
		void RotateWithRightChild( Node *&pNode ) const;
		Node * Clone( Node *pNode ) const;

		int m_size;
		Node *m_pRoot;
		Node *m_pNullNode;
		Trait m_compare; // Default is less than

	};

	/* Node Implementation */
	template < typename T, typename Trait >
	Set< T, Trait >::Node::Node( const T &elem /*= T()*/, Node *pLeft /*= NULL*/, Node *pRight /*= NULL*/, INT level /*= 1*/ ) 
		: m_elem( elem ), m_level( level ), m_pLeft( pLeft ), m_pRight( pRight )
	{

	}

	/* Set Implementation */
	template < typename T, typename Trait >
	Set< T, Trait >::Set( void )
	{
		Init();
	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Init( void )
	{
		m_size = 0;
		m_pNullNode = new Node;
		m_pNullNode->m_pLeft = m_pNullNode->m_pRight = m_pNullNode;
		m_pNullNode->m_level = 0;
		m_pRoot = m_pNullNode;
	}

	template < typename T, typename Trait >
	Set< T, Trait >::~Set( void )
	{
		MakeEmpty();
		delete m_pNullNode;
	}

	template < typename T, typename Trait >
	void Set< T, Trait >::MakeEmpty( void )
	{
		MakeEmpty( m_pRoot );
		m_size = 0;
	}

	template < typename T, typename Trait >
	Set< T, Trait >::Set( const Set &rhs )
	{
		Init();
		*this = rhs;
	}

	template < typename T, typename Trait >
	typename const Set< T, Trait > & Set< T, Trait >::operator = ( const Set &rhs )
	{
		if ( this == &rhs )
			return *this;

		MakeEmpty();
		m_pRoot = Clone( rhs.m_pRoot );
		m_size = rhs.m_size;

		return *this;
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::IteratorBegin( void )
	{
		if ( IsEmpty() )
			return IteratorEnd();

		Iterator iter( *this );
		iter.GoRoot();
		while( iter.HasLeft() )
			iter.GoLeft();
		
		return iter;
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::IteratorEnd( void ) const
	{
		return Iterator( *this );
	}

	template < typename T, typename Trait >
	INT Set< T, Trait >::GetSize( void ) const
	{
		return m_size;
	}

	template < typename T, typename Trait >
	bool Set< T, Trait >::IsEmpty( void ) const
	{
		return m_size == 0;
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::Find( const T &obj ) /*const*/
	{
		if ( IsEmpty() )
			return IteratorEnd();

		Iterator iter( *this );
		iter.GoRoot();

		while ( iter.m_pCurrent != m_pNullNode )
		{
			// default is lessthan
			if ( m_compare( obj, *iter ) )
				iter.GoLeft();
			else if ( m_compare( *iter, obj ) )
				iter.GoRight();
			else
				return iter;
		}

		return IteratorEnd();
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::Find( const T &obj ) const
	{
		if ( IsEmpty() )
			return IteratorEnd();

		Iterator iter( *this );
		iter.GoRoot();

		while ( iter.m_pCurrent != m_pNullNode )
		{
			// default is lessthan
			if ( m_compare( obj, *iter ) )
				iter.GoLeft();
			else if ( m_compare( *iter, obj ) )
				iter.GoRight();
			else
				return iter;
		}

		return IteratorEnd();
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::Bound( const T &obj, bool lower ) /*const*/
	{
		if ( IsEmpty() )
			return Iterator( *this );

		Iterator iter( *this );
		iter.GoRoot();
		Node *pLastLeft = NULL;

		while ( iter.m_pCurrent != m_pNullNode )
		{
			if ( m_compare( obj, *iter ) )
			{
				pLastLeft = iter.m_pCurrent;
				iter.GoLeft();
			}
			else if ( lower && !m_compare( *iter, obj ) ) // LoweBound and exact match
				return iter;
			else
				iter.GoRight();
		}

		if( pLastLeft == NULL )
			return Iterator( *this );

		while ( iter.m_path.GetBack() != pLastLeft )
			iter.m_path.PopBack();

		iter.m_path.PopBack();

		iter.m_pCurrent = pLastLeft;
		return iter;
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::LowerBound( const T &obj ) /*const*/
	{
		return Bound( obj, true );
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::UpperBound( const T &obj ) /*const*/
	{
		return Bound( obj, false );
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::ReturnPair Set< T, Trait >::Insert( const T &obj )
	{
		bool result = Insert( obj, m_pRoot );
		
		if ( result )
			++m_size;

		return ReturnPair( Find( obj ), result );
	}

	template < typename T, typename Trait >
	int Set< T, Trait >::Remove( const Iterator &iter )
	{
		return Remove( *iter );
	}

	template < typename T, typename Trait >
	int Set< T, Trait >::Remove( const T &obj )
	{
		if ( Find( obj ) == IteratorEnd() )
			return 0;

		Remove( obj, m_pRoot );
		--m_size;

		return 1;
	}

	template < typename T, typename Trait >
	PSX_FORCE_INLINE void Set< T, Trait >::Clear( void )
	{
		MakeEmpty();
	}

		/* Set Internal Methods Implementation */

	/**
	* Internal method to insert into a subtree.
	* obj is the item to insert.
	* pNode is the node that roots the tree.
	* Set the new root.
	*/
	template < typename T, typename Trait >
	bool Set< T, Trait >::Insert( const T &obj, Node *&pNode )
	{
		bool result = TRUE;

		if ( pNode == m_pNullNode )
			pNode = new Node( obj, m_pNullNode, m_pNullNode );
		else if ( m_compare( obj, pNode->m_elem ) )
			result = Insert( obj, pNode->m_pLeft );
		else if ( m_compare( pNode->m_elem, obj ) )
			result = Insert( obj, pNode->m_pRight );
		else
			return false; // This is a duplicate. Just do nothing.

		Skew( pNode );
		Split( pNode );
		return result;
	}

	/**
	* Internal method to remove from a subtree.
	* obj is the item to remove.
	* pNode is the node that roots the tree.
	* Set the new root.
	*/
	template < typename T, typename Trait >
	void Set< T, Trait >::Remove( const T &obj, Node *&pNode )
	{
		static Node *pLastNode, *pDeleteNode = m_pNullNode;

		if ( pNode != m_pNullNode )
		{
			// Step 1: Seach down the tree and set lastNode and deleteNode
			pLastNode = pNode;
			if ( m_compare( obj, pNode->m_elem ) )
				Remove( obj, pNode->m_pLeft );
			else
			{
				pDeleteNode = pNode;
				Remove( obj, pNode->m_pRight );
			}

			// Step 2: If at the bottom of the tree and
			//         x is present, we remove it
			if( pNode == pLastNode )
			{
				if( pDeleteNode == m_pNullNode || m_compare( obj, pDeleteNode->m_elem )
					|| m_compare(  pDeleteNode->m_elem, obj ) )
					return;   // Item not found; do nothing

				pDeleteNode->m_elem = pNode->m_elem;
				pDeleteNode = m_pNullNode;
				pNode = pNode->m_pRight;
				delete pLastNode;
			}
			else if( pNode->m_pLeft->m_level < pNode->m_level - 1 || pNode->m_pRight->m_level < pNode->m_level - 1 )
			// Step 3: Otherwise, we are not at the bottom; rebalance
			{
				if( pNode->m_pRight->m_level > --pNode->m_level )
					pNode->m_pRight->m_level = pNode->m_level;

				Skew( pNode );
				Skew( pNode->m_pRight );
				Skew( pNode->m_pRight->m_pRight );
				Split( pNode );
				Split( pNode->m_pRight );
			}
		}
	}

	/*
	* Make subtree empty
	*/
	template < typename T, typename Trait >
	void Set< T, Trait >::MakeEmpty( Node *&pNode )
	{
		if ( pNode != m_pNullNode )
		{
			MakeEmpty( pNode->m_pLeft );
			MakeEmpty( pNode->m_pRight );
			delete pNode;
		}
		pNode = m_pNullNode;
	}

	/**
	* Rotate binary tree node with left child.
	*/
	template < typename T, typename Trait >
	void Set< T, Trait >::RotateWithLeftChild( Node *&pNode ) const
	{
		Node *pNode1 = pNode->m_pLeft;
		pNode->m_pLeft = pNode1->m_pRight;
		pNode1->m_pRight = pNode;
		pNode = pNode1;
	}

	/**
	* Rotate binary tree node with right child.
	*/
	template < typename T, typename Trait >
	void Set< T, Trait >::RotateWithRightChild( Node *&pNode ) const
	{
		Node *pNode2 = pNode->m_pRight;
		pNode->m_pRight = pNode2->m_pLeft;
		pNode2->m_pLeft = pNode;
		pNode = pNode2;
	}

	/**
	* Skew primitive for AA-trees.
	* pNode is the node that roots the tree.
	*/
	template < typename T, typename Trait >
	void Set< T, Trait >::Skew( Node *&pNode) const
	{
		if ( pNode->m_pLeft->m_level == pNode->m_level )
			RotateWithLeftChild( pNode );
	}

	/**
	* Split primitive for AA-trees.
	* pNode is the node that roots the tree.
	*/
	template < typename T, typename Trait >
	void Set< T, Trait >::Split( Node *&pNode) const
	{
		if ( pNode->m_pRight->m_pRight->m_level == pNode->m_level )
		{
			RotateWithRightChild( pNode );
			++pNode->m_level;
		}
	}

	/**
	* Internal method to clone subtree.
	*/
	template < typename T, typename Trait >
	typename Set< T, Trait >::Node * Set< T, Trait >::Clone( Node *pNode) const
	{
		if ( pNode == pNode->m_pLeft ) // Can't test against Null Node!
			return m_pNullNode;
		else
			return new Node( pNode->m_elem, Clone( pNode->m_pLeft ), Clone( pNode->m_pRight ), pNode->m_level );
	}

	/* Iterator Implementation */
	template < typename T, typename Trait >
	Set< T, Trait >::Iterator::Iterator( ) : m_pRoot( NULL ), m_pCurrent( NULL )
	{

	}

	template < typename T, typename Trait >
	Set< T, Trait >::Iterator::Iterator( const Iterator &iter )
		: m_pRoot( iter.m_pRoot ), m_pCurrent( iter.m_pCurrent ), m_path( iter.m_path )
	{

	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Iterator::AssertIsInitialized( void ) const
	{
		PSX_Assert( m_pRoot, "Iterator uninitialized." );
	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Iterator::AssertIsValid( void ) const
	{
		AssertIsInitialized();
	}

	template < typename T, typename Trait >
	T & Set< T, Trait >::Iterator::Retrieve( void ) const
	{
		AssertIsValid();
		PSX_Assert( m_pCurrent, "Iterator Out-Of-Bounds. Cannot perform *end() in set" );
		
		return m_pCurrent->m_elem;
	}

	template < typename T, typename Trait >
	T & Set< T, Trait >::Iterator::operator * ( void )
	{
		return Retrieve();
	}

	template < typename T, typename Trait >
	const T & Set< T, Trait >::Iterator::operator * ( void ) const
	{
		return Retrieve();
	}

	template < typename T, typename Trait >
	T * Set< T, Trait >::Iterator::operator -> ( void )
	{
		return &Retrieve();
	}

	template < typename T, typename Trait >
	const T * Set< T, Trait >::Iterator::operator -> ( void ) const
	{
		return &Retrieve();
	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Iterator::AssertCanAdvance( void ) const
	{
		AssertIsInitialized();
		PSX_Assert( m_pCurrent, "Cannot perform ++end() in set" );
	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Iterator::Advance( void )
	{
		if ( HasRight() )
		{
			GoRight();
			while( HasLeft() )
				GoLeft();

			return;
		}

		Node *pParent;

		for ( ; !m_path.IsEmpty(); m_pCurrent = pParent )
		{
			pParent = m_path.GetBack();
			m_path.PopBack();
			if ( pParent->m_pLeft == m_pCurrent )
			{
				m_pCurrent = pParent;
				return;
			}
		}
		m_pCurrent = NULL;
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator & Set< T, Trait >::Iterator::operator ++ ( void )
	{
		AssertCanAdvance();
		Advance();
		return *this;
	}

	template < typename T, typename Trait >
	typename Set< T, Trait >::Iterator Set< T, Trait >::Iterator::operator ++ ( INT )
	{
		Iterator old = *this;
		++(*this);
		return old;
	}

	template < typename T, typename Trait >
	bool Set< T, Trait >::Iterator::operator == ( const Iterator &rhs ) const
	{
		return m_pRoot == rhs.m_pRoot && m_pCurrent == rhs.m_pCurrent;
	}

	template < typename T, typename Trait >
	bool Set< T, Trait >::Iterator::operator != ( const Iterator &rhs ) const
	{
		return !(*this == rhs);
	}

	template < typename T, typename Trait >
	Set< T, Trait >::Iterator::Iterator(const Set &source)
		: m_pRoot( source.m_pRoot ), m_pCurrent( NULL )
	{

	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Iterator::GoLeft( void )
	{
		m_path.PushBack( m_pCurrent );
		m_pCurrent = m_pCurrent->m_pLeft;
	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Iterator::GoRight( void )
	{
		m_path.PushBack( m_pCurrent );
		m_pCurrent = m_pCurrent->m_pRight;
	}

	template < typename T, typename Trait >
	void Set< T, Trait >::Iterator::GoRoot( void )
	{
		m_pCurrent = m_pRoot;
		while ( !m_path.IsEmpty() )
			m_path.PopBack();
	}

	template < typename T, typename Trait >
	bool Set< T, Trait >::Iterator::HasLeft( void ) const
	{
		return m_pCurrent->m_pLeft != m_pCurrent->m_pLeft->m_pLeft;
	}

	template < typename T, typename Trait >
	bool Set< T, Trait >::Iterator::HasRight( void ) const
	{
		return m_pCurrent->m_pRight != m_pCurrent->m_pRight->m_pRight;
	}


}

#endif /* _PSX_SET_H_ */