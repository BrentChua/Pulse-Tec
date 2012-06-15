/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	SortedLList.h
*
*	Description -	Sorted Linked-list container class. Single-ended linked-list.
*					Also known as Priority list or priority queue.
*
*	Comments	-	Default comparer uses the PSX_LessThan functor object.
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/09/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_SORTED_LINKED_LIST_H_
#define _PSX_SORTED_LINKED_LIST_H_

#include "PulseSTD.h"
#include "PTLBase.h"
#include "LList.h"

namespace Pulse
{

	template < typename T, typename Trait = PSX_LessThan< T > >
	class SortedLList : public LList< T >
	{
	public:

		void Insert( const T &elem );
		void Push( const T &elem );

	private:

		// Hide ambiguous methods for this class
		void PushFront( const T &elem ) { }
		void PushBack( const T &elem ) { }
		void PushAfter( Iterator &iter, const T &elem ) { }
		void Insert( Iterator &iter, const T &elem );

		Trait m_compare;
	};

	template < typename T, typename Trait >
	void SortedLList< T, Trait >::Insert( const T &elem )
	{
		Iterator prev;
		Iterator iter( m_pHead );

		// If empty then push front.
		if ( m_size == 0 )
		{
			LList::PushFront( elem );
			return;
		}

		while ( iter.IsValid() && m_compare( elem, *iter ) )
		{
			prev = iter;
			++iter;
		}

		if ( iter.IsValid() )
			LList< T >::Insert( iter, elem );
		else
			LList::PushAfter( prev, elem );
	}


	template < typename T, typename Trait >
	PSX_INLINE void SortedLList< T, Trait >::Push( const T &elem )
	{
		Insert( elem );
	}

	// We need to override LList::Insert( iter, elem ) because we can't
	// arbitrarily insert elements in the sorted list
	// Currently set in private because of ambiguity.
	template < typename T, typename Trait >
	PSX_INLINE void SortedLList< T, Trait >::Insert( Iterator &iter, const T &elem )
	{
		Insert( elem );
	}
};

#endif /* _PSX_SORTED_LINKED_LIST_H_ */