/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	PriorityQueue.h
*
*	Description -	Priority Queue class.
*
*	Comments	-	
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/16/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_PRIORITY_QUEUE_H_
#define _PSX_PRIORITY_QUEUE_H_

#include "PulseSTD.h"
#include "PTLBase.h"
#include "Array.h"

namespace Pulse
{

	template < typename T, typename Trait = PSX_LessThan< T > >
	class PriorityQueue
	{
	public:

		PriorityQueue( void );

		SIZE_T GetSize( void ) const;
		bool IsEmpty( void ) const;

		const T & GetTop( void ) const;

		void Push( const T &obj );
		void Pop( void );


	private:

		Array< T > m_array;
		Trait m_compare;
	};

	template < typename T, typename Trait >
	PriorityQueue< T, Trait >::PriorityQueue( void ) : m_array( 1 ), m_compare( Trait() )
	{

	}

	template < typename T, typename Trait >
	SIZE_T PriorityQueue< T, Trait >::GetSize( void ) const
	{
		return m_array.GetSize() - 1;
	}

	template < typename T, typename Trait >
	bool PriorityQueue< T, Trait >::IsEmpty( void ) const
	{
		return GetSize() == 0;
	}

	template < typename T, typename Trait >
	const T & PriorityQueue< T, Trait >::GetTop( void ) const
	{
		PSX_Assert( !IsEmpty(), "Underflow exception. Container is empty." ); 

		return m_array[1];
	}

	template < typename T, typename Trait >
	void PriorityQueue< T, Trait >::Push( const T &obj )
	{
		m_array.PushBack( obj );
		m_array[0] = obj; // Initialize Sentinel

		// Percolate up
		SIZE_T hole = GetSize();

		// Default is lessthan
		for ( ; m_compare( m_array[ hole / 2 ], obj ); hole /= 2 )
			m_array[ hole ] = m_array[ hole / 2 ];

		m_array[ hole ] = obj;
	}

	template < typename T, typename Trait >
	void PriorityQueue< T, Trait >::Pop( void )
	{
		PSX_Assert( !IsEmpty(), "Underflow exception. Container is emtpy." );

		SIZE_T hole = 1;
		SIZE_T child;

		T temp = m_array.GetTail(); m_array.PopBack();
		SIZE_T size = GetSize();

		for ( ; hole * 2 <= size; hole = child )
		{
			child = hole * 2;
			if ( child != size && m_compare( m_array[child], m_array[child + 1] ) )
				++child;
			if ( m_compare( temp, m_array[ child ] ) )
				m_array[ hole ] = m_array[ child ];
			else
				break;
		}

		if ( !IsEmpty() )
			m_array[ hole ] = temp;
	}
}

#endif /* _PSX_PRIORITY_QUEUE_H_ */