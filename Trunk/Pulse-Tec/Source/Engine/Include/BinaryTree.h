/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	BinaryTree.h
*
*	Description -	Binary tree container class.
*
*	Comments	-	
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/10/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_BINARY_TREE_H_
#define _PSX_BINARY_TREE_H_

#include "PulseSTD.h"
#include "PTLBase.h"

namespace Pulse
{
	template < typename T >
	class BinaryTree
	{
	public:

		// Binary Node
		struct Node
		{
			BinaryNode( const T &elem, Node *pLeft = NULL, Node *pRight = NULL );

			Node * Duplicate( void );

			T m_elem;
			Node *m_pLeft;
			Node *m_pRight;
		};

		class Iterator
		{
		public:

			Iterator( Node *pNode = NULL );

			bool IsValid( void );

			Iterator & ProceedLeft( void );
			Iterator & ProceedRight( void );
		
		private:

			Node *m_pNode;
		};

		BinaryTree( void );
		~BinaryTree( void );

		void Merge( const T &elem, BinaryTree &tree1, BinaryTree &tree2 );
		void Clear();
		void Clear( Node *& pRoot );

		INT GetHeight( void );
		INT GetHeight( Node *pRoot );
		SIZE_T GetSize( void );
		SIZE_T GetSize( Node *pRoot );

		bool IsEmpty( void );


	private:

		Node *m_pRoot;

	};


	/* Binary Node Implementation */
	template < typename T >
	PSX_INLINE  BinaryTree< T >::Node::Node( const T &elem, Node *pLeft /*= NULL*/, Node *pRight /*= NULL*/ ) :
	m_elem( elem ), m_pLeft( pLeft ), m_pRight( pRight )
	{

	}

	template < typename T >
	typename BinaryTree< T >::Node * BinaryTree< T >::Node::Duplicate( void )
	{
		Node *pRoot = new Node( m_pElem );
		PSX_Assert( pRoot, "Failed to allocate memory." );

		if ( m_pLeft )
			pRoot->m_pLeft = m_pLeft->Duplicate();
		if ( m_pRight )
			pRoot->m_pRight = m_pRight->Duplicat();

		return pRoot;
	}

	/* Iterator Implementation */
	template < typename T >
	PSX_INLINE BinaryTree< T >::Iterator::Iterator( Node *pNode /*= NULL*/ ) : m_pNode( pNode )
	{

	}

	template < typename T >
	PSX_INLINE bool BinaryTree< T >::Iterator::IsValid( void )
	{
		return m_pNode != NULL;
	}

	template < typename T >
	typename BinaryTree< T >::Iterator & BinaryTree<  T >::ProceedLeft( void )
	{
		PSX_Assert( m_pNode, "m_pNode is null. Cannot proceed left child." );
		m_pNode = m_pNode->m_pLeft;
		return *this;
	}

	template < typename T >
	typename BinaryTree< T >::Iterator & BinaryTree< T >::ProceedRight( void )
	{
		PSX_Assert( m_pNode, "m_pNode is null. Cannot proceed left child." );
		m_pNode = m_pNode->m_pRight;
		return *this;
	}

	/* Binary Tree Implementation */

	template < typename T >
	BinaryTree< T >::BinaryTree( void ) : m_pRoot( NULL )
	{

	}

	template < typename T >
	BinaryTree< T >::~BinaryTree( void )
	{
		Clear();
	}

	template < typename T >
	void BinaryTree< T >::Merge( const T &elem, BinaryTree &tree1, BinaryTree &tree2 )
	{
		if ( tree1.m_pRoot == tree2.m_pRoot /*&& tree1.m_pRoot != NULL*/ )
		{
			PSX_Assert( false, "Cannot mergre tree with itself." );
			return;
		}

		Node *oldRoot = m_pRoot;

		m_pRoot = new Node( elem, tree1.m_pRoot, tree2.m_pRoot );
		PSX_Assert( m_pRoot, "Failed to allocate memory." );

		if ( this != &tree1 && this != &tree2 )
			Clear();
	}

	template < typename T >
	PSX_INLINE void BinaryTree< T >::Clear( void )
	{
		Clear( m_pRoot );
	}

	template < typename T >
	void BinaryTree< T >::Clear( Node *& pRoot )
	{
		if ( pRoot )
		{
			Clear( pRoot->m_pLeft );
			Clear( pRoot->m_pRight );
			delete pRoot;
		}
	}

	template < typename T >
	PSX_INLINE INT BinaryTree< T >::GetHeight( void )
	{
		return GetHeight( m_pRoot );
	}

	template < typename T >
	INT BinaryTree< T >::GetHeight( Node *pRoot )
	{
		if ( pRoot )
			return 1 + PSX_Max( GetHeight( pRoot->m_pLeft ), GetHeight( pRoot->m_pRight ) );
		else
			return -1;
	}

	template < typename T >
	PSX_INLINE SIZE_T BinaryTree< T >::GetSize( void )
	{
		return GetSize( m_pRoot );
	}

	template < typename T >
	SIZE_T BinaryTree< T >::GetSize( Node *pRoot )
	{
		if ( pRoot )
			return 1 + GetSize( pRoot->m_pLeft ) + GetSize( pRoot->m_pRight );
		else
			return 0;
	}

	template < typename T >
	PSX_INLINE bool BinaryTree< T >::IsEmpty( void )
	{
		return m_pRoot == NULL;
	}

};

#endif /* _PSX_BINARY_TREE_H_ */