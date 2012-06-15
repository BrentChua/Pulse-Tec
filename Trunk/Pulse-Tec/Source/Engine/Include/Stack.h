/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Stack.h
*
*	Description -	Stack container class.
*
*	Comments	-	TODO: Change container from vector to Pulse List(List.h)
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/07/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_STACK_H_
#define _PSX_STACK_H_

#include "PulseSTD.h"
#include "Array.h"

namespace Pulse
{
	template < typename T >
	class Stack
	{
	public:

		Stack( void );
		Stack( const SIZE_T stackSize );

		void Push( T const &elem );
		void Pop( void );
		T Top();
		const T Top() const;
		T TopAndPop( void );
		bool IsEmpty();

		typedef T * Iterator;
		typedef const T * ConstIterator;

	private:

		Array< T > m_array;
	};

	template < typename T >
	Stack< T >::Stack( void )
	{

	}

	template < typename T >
	Stack< T >::Stack( const SIZE_T stackSize )
	{
		m_array.Resize( stackSize );
	}

	template < typename T >
	PSX_INLINE void Stack< T >::Push( T const &elem )
	{
		m_array.PushBack( elem );
	}

	template < typename T >
	PSX_INLINE void Stack< T >::Pop( void )
	{
		m_array.PopBack();
	}

	template < typename T >
	PSX_INLINE T Stack< T >::Top( void )
	{
		return m_array.GetTail();
	}

	template < typename T >
	PSX_INLINE const T Stack< T >::Top( void ) const
	{
		return m_array.GetTail();
	}

	template < typename T >
	PSX_INLINE T Stack< T >::TopAndPop( void )
	{
		// I think this is still a good candidate for making this inline.
		T temp = Top();
		Pop();
		return temp;
	}

	template < typename T >
	bool Stack< T >::IsEmpty()
	{
		return m_array.GetSize() == (SIZE_T)0 ? true : false ;
	}

};

#endif /* _PSX_STACK_H_ */