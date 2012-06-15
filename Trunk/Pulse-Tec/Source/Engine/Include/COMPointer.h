/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	COMPointer.h
*
*	Description -	A smart pointer for COM objects (i.e DX objects).
*
*	Comments	-	
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	06/12/2010	-	Creation of this file
**************************************************************************************/
#ifndef _PSX_COM_POINTER_H_
#define _PSX_COM_POINTER_H_

#include "PulseSTD.h"

namespace Pulse
{
	template < typename T >
	class COMPointer
	{
	public:

		typedef T Type;
		typedef T * TypePointer;

		COMPointer( void );
		COMPointer( const COMPointer< T > &COMPointer );
		COMPointer( const T *pData );
		virtual ~COMPointer();

		const T & operator * ( void ) const;
		T & operator * ( void );
		T * operator -> ( void ) const;

		operator T * ( void ) const;
		operator T & ( void ) const;

		BOOL operator == ( const COMPointer< T > &rhs ) const;

		COMPointer & operator = ( const COMPointer< T > &rhs );
		virtual COMPointer & operator = ( const T *pData );

		void Reset( const T *pData );
		void Reset( void );

		T ** GetPointer();

		//const SIZE_T GetRefCount( void );

		BOOL IsNull( void );

	protected:

		void AddRef( void );
		void Release( void );

		T * m_pCOMObject;

	};

	template < typename T >
	COMPointer<T>::COMPointer( void )
		: m_pCOMObject( NULL )
	{
	}

	template < typename T >
	PSX_INLINE COMPointer<T>::COMPointer( const COMPointer< T > &COMPointer )
		: m_pCOMObject( (T*)COMPointer.m_pCOMObject )
	{
		AddRef();
	}

	template < typename T >
	PSX_INLINE COMPointer<T>::COMPointer( const T *pData )
		: m_pCOMObject( pData )
	{
		AddRef(); 
	}
	
	template < typename T >
	PSX_INLINE COMPointer<T>::~COMPointer()
	{
		Release();
	}

	template < typename T >
	PSX_INLINE const T & COMPointer<T>::operator * ( void ) const
	{
		return *m_pCOMObject;
	}
	
	template < typename T >
	PSX_INLINE T & COMPointer<T>::operator * ( void )
	{
		return *m_pCOMObject;
	}

	template < typename T >
	PSX_INLINE T * COMPointer<T>::operator -> ( void ) const
	{
		return m_pCOMObject;
	}

	template < typename T >
	PSX_INLINE COMPointer<T>::operator T * ( void ) const
	{
		return m_pCOMObject;
	}
	
	template < typename T >
	PSX_INLINE COMPointer<T>::operator T & ( void ) const
	{
		return *m_pCOMObject;
	}

	template < typename T >
	PSX_INLINE BOOL COMPointer<T>::operator == ( const COMPointer< T > &rhs ) const
	{
		return this == &rhs;
	}

	template < typename T >
	PSX_INLINE COMPointer<T> & COMPointer<T>::operator = ( const COMPointer< T > &rhs )
	{
		if ( this == &rhs )
			return;

		Release();

		m_pCOMObject = rhs.m_pCOMObject;
		AddRef();

		return *this;
	}

	template < typename T >
	PSX_INLINE COMPointer<T> & COMPointer<T>::operator = ( const T *pData )
	{
		if ( m_pCOMObject == pData )
			return *this;

		Release();

		m_pCOMObject = const_cast<T*>(pData);
		AddRef();

		return *this;
	}

	template < typename T >
	PSX_INLINE void COMPointer<T>::Reset( const T *pData )
	{
		Release();

		m_pCOMObject = pData;
		AddRef();
	}
	
	template < typename T >
	PSX_INLINE void COMPointer<T>::Reset( void )
	{
		Release();
		m_pCOMObject = NULL;
	}

	template < typename T >
	PSX_INLINE T ** COMPointer<T>::GetPointer() 
	{ 
		Release();
		return &m_pCOMObject; 
	}

	template < typename T >
	PSX_FORCE_INLINE BOOL COMPointer<T>::IsNull( void )
	{
		return m_pCOMObject == NULL;
	}

	template < typename T >
	PSX_FORCE_INLINE void COMPointer<T>::AddRef( void )
	{
		if ( m_pCOMObject )
			m_pCOMObject->AddRef();
	}

	template < typename T >
	PSX_FORCE_INLINE void COMPointer<T>::Release( void )
	{
		PSX_SafeRelease( m_pCOMObject );
	}

}

#endif /* _PSX_COM_POINTER_H_ */