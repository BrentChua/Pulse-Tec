/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Set.h
*
*	Description -	Implementation of the STL Map.
*
*	Comments	-	This is an indirect implementation of Mark Allen Weiss' STL Map.
*					
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	11/14/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_MAP_H_
#define _PSX_MAP_H_

#include "Set.h"

namespace Pulse
{
	/*
	*	K - Key
	*	T - Type
	*	Trait = (default) Less Thans 
	*/
	template < typename K, typename T, typename Trait = PSX_LessKV< PSX_Pair< K, T >, PSX_LessThan< K > > >
	class Map
	{
	public:

		typedef PSX_Pair< K, T >							KVPair;
		typedef Set< KVPair, PSX_LessKV< KVPair, Trait > >	SetType;
		typedef typename SetType::Iterator					Iterator;

		Iterator IteratorBegin( void );
		const Iterator IteratorBegin( void ) const;
		Iterator IteratorEnd( void );
		const Iterator IteratorEnd( void ) const;

		INT GetSize( void );
		bool IsEmpty( void );
		
		T & operator [] ( const K &key );

		Iterator LowerBound( const K &key );
		Iterator UpperBound( const K &key );
		Iterator Find( const K &key );
		Iterator Find( const K &key ) const;
		PSX_Pair< Iterator, bool > Insert( const KVPair &pair );
		INT Remove( const Iterator &iter );
		INT Remove( const K &key );

		void Clear( void );

	private:
		SetType m_set;

	};

	template < typename K, typename T, typename Trait >
	PSX_INLINE typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::IteratorBegin( void )
	{
		return m_set.IteratorBegin();
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE const  typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::IteratorBegin( void ) const
	{
		return (*((SetType*)&m_set)).IteratorBegin();
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::IteratorEnd( void )
	{ 
		return m_set.IteratorEnd();
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE const typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::IteratorEnd( void ) const
	{ 
		return (*((SetType*)&m_set)).IteratorEnd();
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE INT Map< K, T, Trait >::GetSize( void )
	{
		return m_set.GetSize();
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE bool Map< K, T, Trait >::IsEmpty( void )
	{
		return m_set.IsEmpty();
	}

	template < typename K, typename T, typename Trait >
	T & Map< K, T, Trait >::operator [] ( const K &key )
	{
		KVPair pair( key );

		Iterator iter = m_set.Find( pair );
		if ( iter == m_set.IteratorEnd() )
			iter = m_set.Insert( pair ).first;

		return *(T *)&(*iter).second;
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::LowerBound( const K &key )
	{
		return m_set.LowerBound( KVPair( key ) );
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::UpperBound( const K &key )
	{
		return m_set.UpperBound( KVPair( key ) );
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::Find( const K &key )
	{
		return m_set.Find( KVPair( key ) );
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE typename Map< K, T, Trait >::Iterator Map< K, T, Trait >::Find( const K &key ) const
	{
		return m_set.Find( KVPair( key ) );
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE PSX_Pair< typename Map< K, T, Trait >::Iterator, bool > Map< K, T, Trait >::Insert( const KVPair &pair )
	{
		return m_set.Insert( pair );
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE INT Map< K, T, Trait >::Remove( const Iterator &iter )
	{
		return m_set.Remove( iter );
	}

	template < typename K, typename T, typename Trait >
	PSX_INLINE INT Map< K, T, Trait >::Remove( const K &key )
	{
		return m_set.Remove( KVPair( key ) );
	}

	template < typename K, typename T, typename Trait >
	PSX_FORCE_INLINE void Map< K, T, Trait >::Clear( void )
	{
		m_set.Clear();
	}

}

#endif /* _PSX_MAP_H_ */