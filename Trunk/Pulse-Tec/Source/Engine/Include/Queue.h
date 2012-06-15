/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Queue.h
*
*	Description -	Queue container class.
*
*	Comments	-	TODO: Change container from vector to Pulse List(List.h)
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/07/2009	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_QUEUE_H_
#define _PSX_QUEUE_H_

#include "PulseSTD.h"
#include "Array.h"

namespace Pulse
{
	template < typename T >
	class Queue
	{
	public:

		Queue( void );

		Queue( SIZE_T startSize );

		bool IsEmpty();
		void MakeEmpty();

		T Dequeue( void );
		void Enqueue( const T &obj );

		SIZE_T GetSize( void );

	private:

		void Increment( SIZE_T &iter );
		void DoubleQueue( void );

		Array< T > m_array;
		SIZE_T m_size;
		SIZE_T m_front;
		SIZE_T m_back;

	};

	/* Queue Implementation */

	template < typename T >
	Queue< T >::Queue( void ): m_array( 1 )
	{
		MakeEmpty();
	}

	template < typename T >
	Queue< T >::Queue( SIZE_T startSize )
		: m_array( startSize == 0 ? 1 : startSize )
	{
		MakeEmpty();
	}

	template < typename T >
	bool Queue< T >::IsEmpty()
	{
		return m_size == 0;
	}

	template < typename T >
	void Queue< T >::MakeEmpty()
	{
		m_size = 0;
		m_front = 0;
		//m_back = m_array.GetSize() - 1;
		m_back = 0;
	}

	template < typename T >
	T Queue< T >::Dequeue( void )
	{
		PSX_Assert( !IsEmpty(), "Attempting to dequeue empty container." );
		
		--m_size;
		T front = m_array[ m_front ];
		Increment( m_front );

		return front;
	}

	template < typename T >
	void Queue< T >::Enqueue( const T &obj )
	{
		// The first enqueue is always guaranteed since the array's
		// size is default to one.
		m_array[m_back] = obj;

		if ( ++m_size == m_array.GetSize() )
			DoubleQueue();
		Increment( m_back );
	}

	template < typename T >
	void Queue< T >::Increment( SIZE_T &iter )
	{
		if ( ++iter == m_array.GetSize() )
			iter = 0;
	}

	template < typename T >
	void Queue< T >::DoubleQueue( void )
	{
		m_array.Resize( m_array.GetCapacity() * 2 + 1 );
		
		if ( m_front != 0 )
		{
			for ( SIZE_T i = 0; i < m_front; ++i )
				m_array[ m_size + i ] = m_array[ i ];
			
			// Reset m_back
			m_back = m_front + m_size;
		}
	}

	template < typename T >
	SIZE_T Queue<T>::GetSize( void )
	{
		return m_size;
	}

};

#endif /* _PSX_QUEUE_H_ */