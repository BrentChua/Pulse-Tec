/**************************************************************************************
*
*	Copyright (C) 2009 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Singelton.h
*
*	Description -	Template class for for a singleton pattern.
*
*	Comments	-	Restricts the derived class to one and only one instance of itself.
*					IMPORTANT: Derived classes needs to make sure they set constructor
*								to private or protected.
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	12/14/2009	-	Creation of this file
**************************************************************************************/

#ifndef _PSX_SINGLETON_H_
#define _PSX_SINGLETON_H_

#include "PulseSTD.h"

namespace Pulse
{
	template < typename T >
	class Singleton
	{
	public:

		static T * GetInstance( void );
		static void DeleteInstance( void );

	protected:

		Singleton( void );
		virtual ~Singleton( void );

	private:

		Singleton( Singleton & );
		Singleton & operator = ( Singleton & );

		static T *m_pInstance;
	};

	template < typename T >
	T * Singleton< T >::m_pInstance = NULL;

	template < typename T >
	PSX_INLINE T * Singleton< T >::GetInstance( void )
	{
		if ( m_pInstance == NULL )
		{
			m_pInstance = new T;
			PSX_Assert( m_pInstance, "Failed to allocate memory." );
		}

		return m_pInstance;
	}

	template < typename T >
	PSX_INLINE void Singleton< T >::DeleteInstance( void )
	{
		PSX_SafeDelete( m_pInstance );
	}

	template < typename T >
	PSX_FORCE_INLINE Singleton< T >::Singleton( void )
	{

	}

	template < typename T >
	PSX_FORCE_INLINE Singleton< T >::~Singleton( void )
	{

	}
}

#endif /* _PSX_SINGLETON_H_ */