/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	StringID.h
*
*	Description -	Creates a unique identifier from a given string.
*
*	Comments	-
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	03/07/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_HASHED_STRING_H_
#define _PSX_HASHED_STRING_H_

#include "PulseSTD.h"
#include "String.h"

namespace Pulse
{
	class StringID
	{
	public:

		StringID( void );

		explicit StringID( const CHAR * const pIdentStr );

		ULONG GetID( void ) const;

		const CHAR * const GetString( void ) const;

		void SetString( const CHAR *pNewIDStr );

		static void * HashName( const CHAR * pIdentStr );

		BOOL operator < ( const StringID &rhs ) const;

		BOOL operator > ( const StringID &rhs ) const;

		BOOL operator == ( const StringID &rhs ) const;

	private:

		void	*m_ident;
		String	m_stringID;

	};

	PSX_INLINE StringID::StringID( void )
		: m_ident( PSX_NULL )
	{
		
	}

	PSX_INLINE StringID::StringID( const CHAR * const pIdentStr )
		: m_ident(HashName(pIdentStr)), m_stringID( pIdentStr )
	{

	}

	PSX_INLINE ULONG StringID::GetID( void ) const
	{
		return reinterpret_cast<ULONG>(m_ident);
	}

	PSX_INLINE const CHAR * const StringID::GetString( void ) const
	{
		return m_stringID.GetBuffer();
	}

	PSX_INLINE void StringID::SetString( const CHAR *pNewIDStr )
	{
		m_ident = HashName( pNewIDStr );
		m_stringID = pNewIDStr;
	}

	PSX_INLINE void * StringID::HashName( const CHAR * pIdentStr )
	{
		return reinterpret_cast<void*>( PSX_SuperFastHash(pIdentStr, PSX_StrLen(pIdentStr)) );
	}

	PSX_INLINE BOOL StringID::operator < ( const StringID &rhs ) const
	{
		return GetID() < rhs.GetID();
	}

	PSX_INLINE BOOL StringID::operator > ( const StringID &rhs ) const
	{
		return GetID() > rhs.GetID();
	}

	PSX_INLINE BOOL StringID::operator == ( const StringID &rhs ) const
	{
		return GetID() == rhs.GetID();
	}

}

#endif /* _PSX_HASHED_STRING_H_ */