/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Array.h
*
*	Description -	Array class.
*
*	Comments	-	TODO: Re-Implement iterator!!!!.
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	10/29/2009	-	Creation of vector class
*			MrCodeSushi	-	11/1/2009	-	Vector Iterator <- That's is not how you implement one...
**************************************************************************************/

#ifndef _PSX_ARRAY_H_
#define _PSX_ARRAY_H_

#include "PulseSTD.h"
#include "PTLBase.h"

namespace Pulse
{

	template < typename T >
	class Array
	{
	public:

		typedef Array<T> ThisArray;
		
		class Iterator
		{
		public:

			Iterator( void );
			Iterator( ThisArray *pArray, INT index );

			T & operator * ( void );
			T & operator * ( void ) const;
			T * operator -> ( void );
			T * operator -> ( void ) const;

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
			
			ThisArray *m_pArray;
			INT m_index;

		};

		/* Vector Functions */
		Array( VOID );
		Array( SIZE_T size );
		Array( const Array &vec );
		~Array( VOID );

		SIZE_T GetSize( VOID );
		const SIZE_T GetSize( VOID ) const;
		SIZE_T GetCapacity( VOID );
		T & GetHead( VOID );
		const T & GetHead( VOID ) const;
		T & GetTail( VOID );
		const T & GetTail( VOID ) const;

		T & operator [] ( SIZE_T index );
		const T & operator [] ( SIZE_T index ) const;

		VOID Clear( VOID );

		// Doesn't delete memory. Gets overriden in the next push.
		VOID FastClear( VOID ); 

		VOID PushBack( const T &obj );
		VOID PopBack( VOID );
		const Array & operator = ( const Array  &rhs );
		VOID Resize( SIZE_T size );
		VOID Reserve( SIZE_T capacity );

		VOID Remove( const T &obj );
		VOID Remove( SIZE_T index );

		typedef const Iterator * ConstIterator;

		Iterator IteratorBegin( void );
		ConstIterator IteratorBegin( void ) const;
		Iterator IteratorEnd( void );
		ConstIterator IteratorEnd( void ) const;

	private:

		SIZE_T	m_size;
		SIZE_T	m_capacity;
		T		*m_pArray;

	};
};

#include "Array.inl"

#endif /* _PSX_ARRAY_H_ */
